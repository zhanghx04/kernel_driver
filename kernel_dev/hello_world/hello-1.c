/*
 * hello-1.c - The simpleest kernel module
 * */

#include <linux/module.h>	// Needed by all modules
#include <linux/printk.h>	// Needed for pr_info()
#include <linux/kernel.h>
	
int init_module(void) {
	pr_info("Hello world 1. \n");

	/* A non 0 return means init_module failed; the module cannot be loaded */
	return 0;
}

void cleanup_module(void) {
	pr_info("Goodbye world 1. \n");
}

MODULE_LICENSE("GPL");
