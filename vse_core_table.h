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
