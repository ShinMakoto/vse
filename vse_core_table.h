/*
 * vse_core_table.h : VSE's core table definition
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

#ifndef __VSE_CORE_TABLE_H_INCLUDED__
#define __VSE_CORE_TABLE_H_INCLUDED__

#include <linux/kernel_stat.h>
#include <linux/spinlock.h>

#define NOT_VM 0xff
#define THRESHOLD 40

struct vse_core_table
{
	__u8 load;
	__u8 vm_id;
	__u8 flow_num;
};

struct vse_vm_attribute
{
	__u8 vm_id;
	struct task_struct *task;
};

struct vse_vm_entry
{
	struct vse_vm_attribute attr;
	struct list_head list;
};

struct cpu_status
{
	cputime64_t user;
	cputime64_t nice;
	cputime64_t system;
	cputime64_t softirq;
};

extern struct vse_core_table *core_table;
extern bool vse_init_core_table(void);
extern void vse_clean_up_core_table(void);
extern int vse_core_selection(void);

#endif /* __VSE_CORE_TABLE_H_INCLUDED__ */
