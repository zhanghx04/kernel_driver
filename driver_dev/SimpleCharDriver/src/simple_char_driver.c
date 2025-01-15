#include "../include/simple_char_driver.h"

// Global variables
static int major_number;  // major number assigned to our device driver
static char message[BUFFER_SIZE] = {0};
static short message_size;
static struct class *simple_char_class = NULL;
static struct device *simple_char_device = NULL;

// Module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Haoxiang Zhang");
MODULE_DESCRIPTION("Simple Character Device Driver");
MODULE_VERSION("1.0");

// File operations structure
static struct file_operations fops = {
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write
};

// Device open function
/*
    Note:
    - inode is a pointer to a struct inode, which represents an open file.
    - file is a pointer to a struct file, which represents an open file.
*/
static int dev_open(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "SimpleCharDriver: Device opened\n");
    return 0;
}

// Device read function
/**
 * @brief Read message from device driver to user space
 * @param filep: pointer to the file structure
 * @param buffer: pointer to the buffer to store the message
 * @param len: length of the message to read
 * @param offset: offset of the message to read
 * @return: number of bytes read
 */
static ssize_t dev_read(struct file * filep, char *buffer, size_t len, loff_t *offset) {
    int error_count = 0;

    // If we have already sent the message, return 0
    if (*offset > 0) {
        return 0;
    }

    // Copy the message to user space 
    error_count = copy_to_user(buffer, message, message_size);

    if (error_count == 0) {
        printk(KERN_INFO "SimpleCharDriver: Sent %d characters to user\n", message_size);
        *offset += message_size;
        return message_size;
    } else {
        printk(KERN_ERR "SimpleCharDriver: Failed to send %d characters to user\n", error_count);
        return -EFAULT;
    }
}

// Device write function
static ssize_t dev_write(struct file * filep, const char *buffer, size_t len, loff_t *offseet) {
    // Ensure we don't overflow the buffer
    // Max length of message is BUFFER_SIZE
    if (len > BUFFER_SIZE) {
        len = BUFFER_SIZE;
    }

    // Clear the buffer
    memset(message, 0, BUFFER_SIZE);

    // Copy data from user space to kernel space
    if (copy_from_user(message, buffer, len)) {
        printk(KERN_ERR "SimpleCharDriver: Failed to write data to kernel space\n");
        return -EFAULT;
    }

    message_size = len;
    printk(KERN_INFO "SimpleCharDriver: Received %d characters from user\n", message_size);

    return len;
}

// Device release functikon
static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "SimpleCharDriver: Device closed\n");
    return 0;
}


// Module initialization fucntion
static int __init simple_char_init(void) {
    printk(KERN_INFO "SimpleCharDriver: initializing\n");

    /*
        Register major number
        Note:
        - register_chrdev(major_number, device_name, &file_operations)
        - major_number: major number to assign to the device driver
        - device_name: name of the device driver
        - file_operations: pointer to the file operations structure
    */
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    
    // Major numebr is negative if the registration faild  
    if (major_number < 0) {
        printk(KERN_ALERT "SimpleCharDriver: Failed to register major number\n");
        return major_number;
    }


    // Register device class
    simple_char_class = class_create(CLASS_NAME);
    if (IS_ERR(simple_char_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "SimpleCharDriver: Failed to register device class\n");
        return PTR_ERR(simple_char_class);
    }
    
    // Register device device
    simple_char_device = device_create(simple_char_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(simple_char_device)) {
        class_destroy(simple_char_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "SimpleCharDriver: Failed to create device\n");
        return PTR_ERR(simple_char_device);
    }

    printk(KERN_INFO "SimpleCharDriver: Device registered successfully\n");
    return 0;
}

// Module exit functikon
static void __exit simple_char_exit(void) {
    device_destroy(simple_char_class, MKDEV(major_number, 0));
    class_unregister(simple_char_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "SimpleCharDriver: Goodbye\n");
}

module_init(simple_char_init);
module_exit(simple_char_exit);