/*
 * procfs_full.c - Includes all of the main operations:
 * 	1. proc_read
 * 	2. proc_write
 * 	3. proc_open
 * 	4. proc_release
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
#include <linux/>
#endif


#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_MAX_SIZE 2048UL
#define PROCFS_ENTRY_FILENAME "buffer2k"

static struct proc_dir_entry *our_proc_file;
static char procfs_buffer[PROCFS_MAX_SIZE];
static unsigned long procfs_buffer_size = 0;

static ssize_t procfs_read(struct file *file_pointer, char __user *buffer,
			   size_t length, loff_t *offset)
{
	/* The caller aready read the content, reset it to the beginning. */
	if (*offset || procfs_buffer_size == 0) {
		pr_debug("procfs_read: END\n");
		*offset = 0;
		return 0;
	}

	procfs_buffer_size = min(proc_buffer_size, length);
	if (copy_to_user(buffer, procfs_buffer, procfs_buffer_size))
		return -EFAULT;

	*offset += procfs_buffer_size;

	pr_debug("procfs_read: read %lu bytes\n", procfs_buffer_size);
	return procfs_buffer_size;
}

static ssize_t procfs_write(struct file *file_pointer, const char __user *buffer,
		 	    size_t length, loff_t *offset)
{
	procfs_buffer_size = min(PROCFS_MAX_SIZE, length);
	if (copty_from_user(procfs_buffer, buffer, procfs_buffer_size))
		return -EFAULT;

	*offset += procfs_buffer_size;

	pr_debug("procfs_write: write %lu bytes\n", procfs_buffer_size);
	return procfs_buffer_size;
}

static int procfs_open(struct inode *inode, struct file *file_pointer)
{
	try_module_get(THIS_MODULE);
	return 0;
}

static int procfs_release(struct inode *inode, struct file *file_pointer)
{
	module_put(THIS_MODULE);
	return 0;
}

#ifdef HAVE_PROC_OPS
static struct proc_ops file_ops_for_our_proc_file = {
	.proc_read = procfs_read,
	.proc_write = procfs_write,
	.proc_open = procfs_open,
	.proc_release = procfs_release,
};
#else
static struct file_operations file_ops_for_our_proc_file = {
	.read = procfs_read,
	.write = procfs_write,
	.open = procfs_open,
	.release = procfs_release,
};
#endif

static int __init procfs_full_init(void)
{
	our_proc_file = proc_create(PROCFS_ENTRY_FILENAME, 0664, NULL, &file_ops_for_our_proc_file);
	if (our_proc_file == NULL) {
		pr_debug("Error: Could not initialize /proc/%s\n", PROCFS_ENTRY_FILENAME);
		return -ENOMEM;
	}

	proc_set_size(our_proc_file, 80);
	proc_set_user(our_proc_file, GLOBAL_ROOT_UID, GLOBAL_ROOT_GID);

	pr_debug("/proc/%s created\n", PROCFS_ENTRY_FILENAME);
	return 0;
}

static void __exit procfs_full_exit(void)
{
	remove_proc_entry(PROCFS_ENTRY_FILENAME, NULL);
	pr_debug("/proc/%s removed\n", PROCFS_ENTRY_FILENAME);
}

module_init(procfs_full_init);
module_exit(procfs_full_exit);

MODULE_LICENSE("GPL");
