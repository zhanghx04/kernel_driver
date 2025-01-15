# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
ifneq ($(KERNELRELEASE),)
    obj-m := simple_char_driver.o
    simple_char_driver-objs := src/simple_char_driver.o

# Otherwise we were called directly from the command line.
# Invoke the kernel build system.
else
    KERNEL_SOURCE := /lib/modules/$(shell uname -r)/build
    PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(PWD) clean
	rm -f test_driver

# Compile the test program
test: src/test_driver.c
	gcc -Wall -Wextra -o test_driver src/test_driver.c

endif