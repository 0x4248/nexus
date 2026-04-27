/*
 * Proc
 *
 * I try writing to proc
 *
 * 0x4248
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mm.h>
#include <linux/sysinfo.h>


static int demo_show(struct seq_file *m, void *v)
{
	struct sysinfo si;
	si_meminfo(&si);
	seq_printf(m, "totalram = %lu\n", si.totalram);

	return 0;
}

static int demo_open(struct inode *inode, struct file *file)
{
	return single_open(file, demo_show, NULL);
}

static const struct proc_ops demo_fops = {
	.proc_open	= demo_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
};

static int __init demo_init(void)
{
	if (!proc_create("demo_proc", 0444, NULL, &demo_fops))
		return -ENOMEM;

	return 0;
}

static void __exit demo_exit(void)
{
	remove_proc_entry("demo_proc", NULL);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("0x4248");
MODULE_DESCRIPTION("A hello world driver");
