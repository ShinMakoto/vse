#ifndef __VSE_FLOW_TABLE_H_INCLUDED__
#define __VSE_FLOW_TABLE_H_INCLUDED__

#include <linux/if_ether.h>

struct vse_match
{
	__u32 wildcards;
	__u8 vm_id;
	__u8 dl_dst[ETH_ALEN];
	__u8 dl_src[ETH_ALEN];
	__u8 nw_src[16];
	__u8 nw_dst[16];
	__u8 nw_tos;
	__u8 nw_proto;
	__u32 vx_vni;
	__u8 dl_indst[ETH_ALEN];
	__u8 dl_insrc[ETH_ALEN];
};

struct list_head;

struct vse_flow_entry
{
	struct vse_match match;
	__u8 irq;
	struct list_head list;
};

extern bool vse_add_table_entry(const struct vse_match *data);
extern bool vse_add_table_entry_and_irq(const struct vse_match *data, const __u8 irq);
extern struct vse_flow_entry *vse_look_up_flow_entry(struct sk_buff *skb);
extern void vse_clean_up_flow_table(void);
#endif /* __VSE_FLOW_TABLE_H_INCLUDED__ */
