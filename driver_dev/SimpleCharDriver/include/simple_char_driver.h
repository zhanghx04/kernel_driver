#ifndef SIMPLE_CHAR_DRIVER_H
#define SIMPLE_CHAR_DRIVER_H

#include <linux/module.h>   // for module management
#include <linux/kernel.h>   // for kernel functions
#include <linux/init.h>     // for module initialization
#include <linux/fs.h>       // for file operations
#include <linux/uaccess.h>  // for copy_to_user and copy_from_user
#include <linux/device.h>   // for device management

#define DEVICE_NAME "simple_char_dev"
#define CLASS_NAME "simple_char"
#define BUFFER_SIZE 256

// Function prototypes
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

/*
    Note:
    - inode is a pointer to a struct inode, which represents an open file.
    - ssize_t is a signed integer type that is the same size as size_t.
    - loff_t is a signed integer type that is the same size as off_t
*/

#endif // SIMPLE_CHAR_DRIVER_H