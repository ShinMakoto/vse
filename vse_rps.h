/*
 * vse_rps.h : Definition for RPS handler of VSE
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

#ifndef __VSE_RPS_H_INCLUDED__
#define __VSE_RPS_H_INCLUDED__

#include "vse_flow_table.h"

#define NO_IRQ 0xff

extern bool vse_check_rps_table(struct sk_buff *skb, struct net_device *dev);
extern bool vse_do_rps(struct sk_buff *skb, struct net_device *dev, struct vse_flow_entry *entry);
#endif /* __VSE_RPS_H_INCLUDED__ */
