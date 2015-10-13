#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/timer.h>
#include "vse_core_table.h"

struct vse_core_table *core_table;
static struct cpu_status *prev, *now;
static struct timer_list core_table_timer;
static void vse_set_core_table_timer(void);

static void vse_get_cpu_status(struct cpu_status *stat)
{
	int i;

	for_each_possible_cpu(i) {
		stat[i].user = kstat_cpu(i).cpustat.user;
		stat[i].nice = kstat_cpu(i).cpustat.nice;
		stat[i].system = kstat_cpu(i).cpustat.system;
	}
}

static void vse_get_core_table_load(void)
{
	cputime64_t user, nice, system;
	int i;

	for_each_online_cpu(i) {
		user = now[i].user - prev[i].user;
		nice = now[i].nice - prev[i].nice;
		system = now[i].system - prev[i].system;

		core_table[i].load = ((unsigned long long)cputime64_to_clock_t(user)
				      + (unsigned long long)cputime64_to_clock_t(nice)
				      + (unsigned long long)cputime64_to_clock_t(system));

		printk("%d ", core_table[i].vm_id);
	}
	printk("\n");
}

static void vse_copy_prev_status(void)
{
	int i;
	
	for_each_online_cpu(i) {
		prev[i].user = now[i].user;
		prev[i].nice = now[i].nice;
		prev[i].system = now[i].system;
	}
}

static void vse_update_core_table(void)
{
	vse_get_cpu_status(now);
	vse_get_core_table_load();
	vse_copy_prev_status();
	vse_set_core_table_timer();
}

static void vse_set_core_table_timer(void)
{
	init_timer(&core_table_timer);
	core_table_timer.expires = jiffies + HZ;
	core_table_timer.data = 0;
	core_table_timer.function = vse_update_core_table;
	add_timer(&core_table_timer);
}

extern int vse_core_selection(void)
{
	int i;
	int irq = smp_processor_id();
	
	for_each_online_cpu(i) {
		if (core_table[i].load < THRESHOLD && core_table[i].vm_id != NOT_VM) {
			irq = i;
			break;
		}
	}

	return irq;
}

extern bool vse_init_core_table(void)
{
	int i;

	/* initialize core table */
	core_table = kzalloc(sizeof(struct vse_core_table) * num_online_cpus(), GFP_KERNEL);
	if (core_table == NULL) {
		pr_err("Cannot init core table");
		return false;
	}
	
	for_each_online_cpu(i) {
		core_table[i].vm_id = NOT_VM;
	}

	prev = kzalloc(sizeof(struct cpu_status) * num_online_cpus(), GFP_KERNEL);
	if (prev == NULL) {
		kfree(core_table);
		pr_err("Cannot init core table");
		return false;
	}

	now = kzalloc(sizeof(struct cpu_status) * num_online_cpus(), GFP_KERNEL);
	if (prev == NULL) {
		kfree(core_table);
		kfree(prev);
		pr_err("Cannot init core table");
		return false;
	}

	/* initialize core table timer */
	vse_set_core_table_timer();

	return true;
}

extern void vse_clean_up_core_table(void)
{
	del_timer_sync(&core_table_timer);
	kfree(core_table);
	kfree(prev);
	kfree(now);
}
