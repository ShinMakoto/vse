/*
 * vse_flow_table.c : OpenFlow based flow table funtions
 *
 * Copyright 2015 Shin Muramatsu <muramatsu@matlab.nitech.ac.jp> Nagoya Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/kernel_stat.h>
#include <linux/netdevice.h>
#include <linux/ip.h>

#include "vse_flow_table.h"
#include "vse_rps.h"
#include "openflow.h"

static LIST_HEAD(flow_table); /* OpenFlow-based flow table */

static struct vse_flow_entry *vse_create_entry(void);
static void vse_set_entry_match(struct vse_match *vse, const struct vse_match *data);
static bool vse_do_match(struct sk_buff *skb, const struct vse_match *match);
static void vse_delete_entry(struct vse_flow_entry *entry);
static __be32 vse_get_vxlan_vni(struct sk_buff *skb);
static struct ethhdr *vse_get_inner_ethhdr(struct sk_buff *skb);

static __u32 vse_get_vxlan_vni(struct sk_buff *skb)
{
	__be32 *vni;

	vni = skb->data + 20 + 8 + 4; /* IP header and UDP header and reserved filed of VXLAN header */

	return (htonl(*vni & 0xfffffff0) >> 8);
}

static struct ethhdr *vse_get_inner_ethhdr(struct sk_buff *skb)
{
	return (struct ethhdr *)(skb->data + 20 + 8 + 8);
}

static struct vse_flow_entry *vse_create_entry(void)
{
	struct vse_flow_entry *entry;

	entry = kzalloc(sizeof(struct vse_flow_entry), GFP_ATOMIC);
	if (unlikely(! entry)) {
		pr_err("Can't allocate memory\n");
	}
	
	return entry;
}

static void vse_delete_entry(struct vse_flow_entry *entry)
{
	if (entry) {
		if (entry->list.next || entry->list.prev) {
			list_del(&entry->list);
			entry->list.next = entry->list.prev = NULL;
		}
		kfree(entry);
	}
}

static void vse_set_entry_match(struct vse_match *vse, const struct vse_match *data)
{
	vse->wildcards = data->wildcards;

	/* vm_id */
	if (~vse->wildcards & OFPFW_VM_ID) {
		vse->vm_id = data->vm_id;
	}

	/* dl_dst */
	if(~vse->wildcards & OFPFW_DL_DST){
		memcpy(vse->dl_dst, data->dl_dst, ETH_ALEN);
	}

	/* VXLAN vx_vni */
	if(~vse->wildcards & OFPFW_VXLAN_VNI){
		memcpy(&vse->vx_vni, &data->vx_vni, 4);
	}

	/*INNER dl_dst */
	if(~vse->wildcards & OFPFW_DL_INDST){
		memcpy(vse->dl_indst, data->dl_indst, ETH_ALEN);
	}
}

bool vse_add_table_entry(const struct vse_match *data)
{
	struct vse_flow_entry *entry;
	
	entry = vse_create_entry();
	if (unlikely(! entry)) {
		pr_err("Can't create flow entry\n");
		return false;
	}

	vse_set_entry_match(&entry->match, data);
	
	entry->irq = NO_IRQ;

	list_add(&entry->list, &flow_table);

	return true;
}

bool vse_add_table_entry_and_irq(const struct vse_match *data, const __u8 irq)
{
	struct vse_flow_entry *entry;
	
	entry = vse_create_entry();
	if (unlikely(! entry)) {
		pr_err("Can't create flow entry\n");
		return false;
	}

	vse_set_entry_match(&entry->match, data);
	entry->irq = irq;

	list_add(&entry->list, &flow_table);

	return true;
}

static struct list_head *vse_get_flow_table(void)
{
    return &flow_table;
}

void vse_clean_up_flow_table(void)
{
	struct vse_flow_entry *entry;

	while (! list_empty(&flow_table)) {
		entry = list_entry(flow_table.next, struct vse_flow_entry, list);
		vse_delete_entry(entry);
	}
}

static bool vse_do_match(struct sk_buff *skb, const struct vse_match *match)
{
	struct ethhdr *dl, *in_dl;
	struct iphdr *nw;
	__be32 vx_vni;
	__u32 wildcards;
	
	wildcards = match->wildcards;
	skb_reset_network_header(skb);

	dl = eth_hdr(skb);
	in_dl = vse_get_inner_ethhdr(skb);
	vx_vni = vse_get_vxlan_vni(skb);

	/* pr_info("wildcards %x", wildcards); */
	/* pr_info("%x:%x:%x:%x:%x:%x", dl->h_source[0],dl->h_source[1],dl->h_source[2],dl->h_source[3],dl->h_source[4],dl->h_source[5]); */
	/* pr_info("%x:%x:%x:%x:%x:%x", match->in_dl->h_source[0],match->in_dl->h_source[1],match->in_dl->h_source[2],match->dl_indst[3],match->dl_indst[4],match->dl_indst[5]); */
	/* pr_info("vx_vni %d, match->vx_vni %d", vx_vni, match->vx_vni); */
	
	if (~wildcards & OFPFW_VM_ID) {
		if (((match->vm_id ^ (nw->tos >> 2)) & 0x7f)) {
			return false;	
		} 
		wildcards |= OFPFW_VM_ID;
	}
	if (~wildcards & OFPFW_DL_DST) {
		if(memcmp(match->dl_dst, dl->h_dest, ETH_ALEN) != 0){
			return false;
		}
		wildcards |= OFPFW_DL_DST;
	}
	if(~wildcards & OFPFW_VXLAN_VNI) {
		if(memcmp(&match->vx_vni, &vx_vni, 4) != 0){
			return false;
		}
		wildcards |= OFPFW_VXLAN_VNI;
	}
	if (~wildcards & OFPFW_DL_INDST) {
		if(memcmp(match->dl_indst, in_dl->h_dest, ETH_ALEN) != 0){
			return false;
		}
		wildcards |= OFPFW_DL_INDST;
	}

	return (wildcards == OFPFW_ALL);
}

struct vse_flow_entry *vse_look_up_flow_entry(struct sk_buff *skb)
{
	struct list_head *flow_table;
	struct list_head *p;
	struct vse_flow_entry *entry;

	flow_table = vse_get_flow_table();

	entry = NULL;
	list_for_each(p, flow_table){
		entry = list_entry(p, struct vse_flow_entry, list);
		if (vse_do_match(skb, &entry->match)) {
			/* pr_info("match!!"); */
			break; /* Matched !!!!*/
		}
		entry = NULL;
	}
	
	return entry;
}

