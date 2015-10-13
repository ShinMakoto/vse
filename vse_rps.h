#ifndef __VSE_RPS_H_INCLUDED__
#define __VSE_RPS_H_INCLUDED__

#include "vse_flow_table.h"

#define NO_IRQ 0xff

extern bool vse_check_rps_table(struct sk_buff *skb, struct net_device *dev);
extern bool vse_do_rps(struct sk_buff *skb, struct net_device *dev, struct vse_flow_entry *entry);
#endif /* __VSE_RPS_H_INCLUDED__ */
