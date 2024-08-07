# Makefile for building the hello kernel module

# Define the kernel build directory (use the defaul if not specified) 
KERNELDIR := /lib/modules/$(shell uname -r)/build

# Define the module name
obj-m := hello.o

# Define the target
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

# Clean target
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean 

