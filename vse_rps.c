/*
 * vse_rps.c : Controlling RPS functions
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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/kernel_stat.h>
#include <linux/netdevice.h>
#include <linux/ip.h>

#include "vse_main.h"
#include "vse_rps.h"
#include "vse_flow_table.h"
#include "openflow.h"

static void vse_set_rxhash(struct sk_buff *skb, const __u8 irq);
static struct netdev_rx_queue *vse_get_rxqueue(struct sk_buff *skb, struct net_device *dev);
static bool vse_set_sock_flow_table(struct sk_buff *skb, struct net_device *dev, const __u8 irq);

static void vse_set_rxhash(struct sk_buff *skb, const __u8 irq)
{
	skb->rxhash = irq;
}

static struct netdev_rx_queue *vse_get_rxqueue( struct sk_buff *skb,  struct net_device *dev)
{
        struct netdev_rx_queue *rxqueue = NULL;
        struct netdev_rps_info *rpinfo = &netdev_extended(dev)->rps_data;

	rcu_read_lock();

        if (skb_rx_queue_recorded(skb)) {
                u16 index = skb_get_rx_queue(skb);
                if (unlikely(index >= netdev_extended(dev)->real_num_rx_queues)) {
                        WARN_ONCE(netdev_extended(dev)->real_num_rx_queues > 1,
                                  "%s received packet on queue %u, but number "
                                  "of RX queues is %u\n",
                                  dev->name, index, netdev_extended(dev)->real_num_rx_queues);
                        goto done;
                }
                rxqueue = rpinfo->_rx + index;
        } else
                rxqueue = rpinfo->_rx;
done:
	rcu_read_unlock();
	return rxqueue;
}

static bool vse_set_sock_flow_table(struct sk_buff *skb, struct net_device *dev, const __u8 irq)
{
	struct netdev_rx_queue *rxqueue = NULL;
	struct rps_dev_flow_table *flow_table = NULL;
	struct rps_sock_flow_table *sock_flow_table = NULL;

	rcu_read_lock();

	rxqueue = vse_get_rxqueue(skb, dev);
	if (rxqueue == NULL) {
		rcu_read_unlock();
		return false;
	}
	
	flow_table = rcu_dereference(rxqueue->rps_flow_table);
	sock_flow_table = rcu_dereference(rps_sock_flow_table);

	if (flow_table && sock_flow_table) {
		struct rps_dev_flow *rflow = &flow_table->flows[irq & flow_table->mask];
		rflow->cpu = irq;
		sock_flow_table->ents[irq & sock_flow_table->mask] = irq;
		/* pr_info("Set sock flow table"); */
		/* pr_info("rflow->cpu %d ,sock_flow_table->entries %d", rflow->cpu, sock_flow_table->ents[irq & sock_flow_table->mask]); */
	}

	rcu_read_unlock();
	return true;
}

bool vse_check_rps_table(struct sk_buff *skb, struct net_device *dev)
{
	struct netdev_rx_queue *rxqueue = NULL;
	struct rps_dev_flow_table *flow_table = NULL;
	struct rps_sock_flow_table *sock_flow_table = NULL;

	rcu_read_lock();

	rxqueue = vse_get_rxqueue(skb, dev);
	if (rxqueue == NULL) {
		pr_err("Not set rps functions\n");
		rcu_read_unlock();
		return false;
	}
	
	flow_table = rcu_dereference(rxqueue->rps_flow_table);
	sock_flow_table = rcu_dereference(rps_sock_flow_table);

	if (! (flow_table && sock_flow_table)) {
		pr_err("Not set rps functions\n");
		rcu_read_unlock();
		return false;
	}	

	rcu_read_unlock();
	return true;
}

bool vse_do_rps(struct sk_buff *skb, struct net_device *dev, struct vse_flow_entry *entry)
{
	if (entry->irq == NO_IRQ){ /* first paket of the flow */
		/* entry->irq = vse_core_selection(); */
	}

	vse_set_sock_flow_table(skb, dev, entry->irq);
	vse_set_rxhash(skb, entry->irq);

	return true;
}
