/*
 * vse_main.h : A VSE handler definition for mlx4_en driver
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

#ifndef __VSE_CORE_H_INCLUDED__
#define __VSE_CORE_H_INCLUDED__

#undef pr_debug
#define pr_debug(fmt, ...) \
	  pr_info("%s:%d %s  " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

extern int (*vse_handler)(struct sk_buff *skb, struct net_device *dev);

#endif /* __VSE_FLOW_ENTRY_H_INCLUDED__ */
