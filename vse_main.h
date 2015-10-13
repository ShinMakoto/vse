#ifndef __VSE_CORE_H_INCLUDED__
#define __VSE_CORE_H_INCLUDED__

#undef pr_debug
#define pr_debug(fmt, ...) \
	  pr_info("%s:%d %s  " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

extern int (*vse_handler)(struct sk_buff *skb, struct net_device *dev);
/* extern struct sk_buff *g_skb; */

#endif /* __VSE_FLOW_ENTRY_H_INCLUDED__ */
