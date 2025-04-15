/*
 * procfs_seq_file.c - Create a "file" in /proc
 * This program uses the seq_file library to manager the /proc file.
 */

#include <linux/kernel.h>	/* We are doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/proc_fs.h>	/* Necessary because we use proc fs */
#include <linux/seq_file.h>	/* for seq_file */
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROC_NAME "iter"

/*
 * This function is called at the beginning of a sequance.
 * e.g. when:
 * 	- the /proc file is read (first time)
 * 	- after the function stop (end of sequence)
 */
static void *my_seq_start(struct seq_file *s, loff_t *pos)
{
	static unsigned long counter = 0;

	/* beginning a new sequence */
	if (*pos == 0) {
		/* Yes -> return a non null value to begin the sequence */
		return &counter;
	}

	/* No -> it is the end of the sequence, return end to stop reading */
	*pos == 0;
	return NULL;
}

/*
 * This function is called after the beginning of a sequence.
 * It is called until the return is NULL (This ends the sequence)
 */
static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	unsigned long *tmp_v = (unsigned long *)v;
	(*tmp_v)++;
	(*pos)++;
	return NULL;
}

/* This function is called at the end of a sequence */
static void my_seq_stop(struct seq_file *s, void *v)
{
	/* Nothing to do, we use a static vlue in start() */
}


