/*
 * vse_dev.c : APIs for vse_set_entry
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
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "openflow.h"
#include "vse_flow_table.h"
#include "vse_core_table.h"
#include "vse_rps.h"
#include "vse_dev.h"

#define SUCCESS 0

static int Device_Open = 0;
struct vse_match entry_match; /* setting arguments */
__u8 irq;

static void vse_set_vm_id(unsigned long arg);
static void vse_set_dl_indst1(unsigned long arg);
static void vse_set_dl_indst2(unsigned long arg);
static void vse_set_dl_indst3(unsigned long arg);
static void vse_set_dl_indst4(unsigned long arg);
static void vse_set_dl_indst5(unsigned long arg);
static void vse_set_dl_indst6(unsigned long arg);
static void vse_set_dl_dst1(unsigned long arg);
static void vse_set_dl_dst2(unsigned long arg);
static void vse_set_dl_dst3(unsigned long arg);
static void vse_set_dl_dst4(unsigned long arg);
static void vse_set_dl_dst5(unsigned long arg);
static void vse_set_dl_dst6(unsigned long arg);
static void vse_set_vni(unsigned long arg);
static void vse_set_irq(unsigned long arg);

static void vse_set_vm_id(unsigned long arg)
{
	get_user(entry_match.vm_id, (__u8*)arg);
	entry_match.wildcards &= ~OFPFW_VM_ID;
	pr_info("id %d", entry_match.vm_id);
}

static void vse_set_dl_indst1(unsigned long arg)
{
	get_user(entry_match.dl_indst[0], (__u8*)arg);
}
static void vse_set_dl_indst2(unsigned long arg)
{
	get_user(entry_match.dl_indst[1], (__u8*)arg);
}
static void vse_set_dl_indst3(unsigned long arg)
{
	get_user(entry_match.dl_indst[2], (__u8*)arg);
}
static void vse_set_dl_indst4(unsigned long arg)
{
	get_user(entry_match.dl_indst[3], (__u8*)arg);
}
static void vse_set_dl_indst5(unsigned long arg)
{
	get_user(entry_match.dl_indst[4], (__u8*)arg);
}
static void vse_set_dl_indst6(unsigned long arg)
{
	get_user(entry_match.dl_indst[5], (__u8*)arg);
	entry_match.wildcards &= ~OFPFW_DL_INDST;
	pr_info("dl_indst %x:%x:%x:%x:%x:%x",
		entry_match.dl_indst[0],entry_match.dl_indst[1],
		entry_match.dl_indst[2],entry_match.dl_indst[3],
		entry_match.dl_indst[4],entry_match.dl_indst[5]);
}

static void vse_set_dl_dst1(unsigned long arg)
{
	get_user(entry_match.dl_dst[0], (__u8*)arg);
}
static void vse_set_dl_dst2(unsigned long arg)
{
	get_user(entry_match.dl_dst[1], (__u8*)arg);
}
static void vse_set_dl_dst3(unsigned long arg)
{
	get_user(entry_match.dl_dst[2], (__u8*)arg);
}
static void vse_set_dl_dst4(unsigned long arg)
{
	get_user(entry_match.dl_dst[3], (__u8*)arg);
}
static void vse_set_dl_dst5(unsigned long arg)
{
	get_user(entry_match.dl_dst[4], (__u8*)arg);
}
static void vse_set_dl_dst6(unsigned long arg)
{
	get_user(entry_match.dl_dst[5], (__u8*)arg);
	entry_match.wildcards &= ~OFPFW_DL_DST;
	pr_info("dl_dst %x:%x:%x:%x:%x:%x",
		entry_match.dl_dst[0],entry_match.dl_dst[1],
		entry_match.dl_dst[2],entry_match.dl_dst[3],
		entry_match.dl_dst[4],entry_match.dl_dst[5]);
}

static void vse_set_vni(unsigned long arg)
{
	get_user(entry_match.vx_vni, (__u32*)arg);
	entry_match.wildcards &= ~OFPFW_VXLAN_VNI;
	pr_info("vni %d", entry_match.vx_vni);
}

static void vse_set_irq(unsigned long arg)
{
	get_user(irq, (__u8*)arg);
	pr_info("irq %d", irq);
}

static int device_open(struct inode *inode, struct file *file)
{
	if (Device_Open)
		return -EBUSY;

	Device_Open++;

	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;

	module_put(THIS_MODULE);
	return SUCCESS;
}

int vse_device_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case VSE_IOCTL_INIT_WILDCARDS: {
		pr_info("init entry");
		memset(&entry_match, 0, sizeof(struct vse_match));
		entry_match.wildcards = OFPFW_ALL;
		irq = NO_IRQ;
		break;
	}
	case VSE_IOCTL_SET_VM_ID: {
		vse_set_vm_id(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_INDST_1: {
		vse_set_dl_indst1(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_INDST_2: {
		vse_set_dl_indst2(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_INDST_3: {
		vse_set_dl_indst3(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_INDST_4: {
		vse_set_dl_indst4(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_INDST_5: {
		vse_set_dl_indst5(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_INDST_6: {
		vse_set_dl_indst6(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_DST_1: {
		vse_set_dl_dst1(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_DST_2: {
		vse_set_dl_dst2(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_DST_3: {
		vse_set_dl_dst3(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_DST_4: {
		vse_set_dl_dst4(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_DST_5: {
		vse_set_dl_dst5(arg);
		break;
	}
	case VSE_IOCTL_SET_DL_DST_6: {
		vse_set_dl_dst6(arg);
		break;
	}
	case VSE_IOCTL_SET_VNI: {
		vse_set_vni(arg);
		break;
	}
	case VSE_IOCTL_SET_IRQ: {
		vse_set_irq(arg);
	}
	case VSE_IOCTL_SET_ENTRY: {
		vse_add_table_entry_and_irq(&entry_match, irq);
		break;
	}
		
	}
	return SUCCESS;
}

struct file_operations Fops = {
	.owner = THIS_MODULE,
	.ioctl = vse_device_ioctl,
	.open = device_open,
	.release = device_release,
};
