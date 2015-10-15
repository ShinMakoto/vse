/*
 * vse_main.c : Module's main functions
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
#include "vse_dev.h"
#include "openflow.h"

#undef pr_debug
#define pr_debug(fmt, ...) \
	  pr_info("%s:%d %s  " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

static int vse_do_handle(struct sk_buff *skb, struct net_device *dev);
static int __init vse_init(void);
static void __exit vse_cleanup(void);


void vse_handle_data(struct sk_buff *skb, struct net_device *dev)
{
	struct vse_flow_entry *entry;

	if (! skb_mac_header_was_set(skb)) {
		skb_reset_mac_header(skb);
	}

	entry = vse_look_up_flow_entry(skb);

	if (entry == NULL) {
		goto done;
	}

	vse_do_rps(skb, dev, entry);

done:;
}

static int vse_do_handle(struct sk_buff *skb, struct net_device *dev)
{
	vse_handle_data(skb, dev);

	return 0;
}

static int __init vse_init(void)
{
	pr_debug("VSE module has been loaded\n");

	vse_handler = vse_do_handle;

	register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

	/* vse_init_core_table(); */

	return 0;
}


static void __exit vse_cleanup(void)
{
	pr_info("Clean up VSE\n");

	vse_handler = NULL;

	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);

	vse_clean_up_flow_table();

	/* vse_clean_up_core_table(); */
}

module_init(vse_init);
module_exit(vse_cleanup);

MODULE_DESCRIPTION("VSE module");
MODULE_LICENSE("GPL");
