obj-m += virtual_imu.o
virtual_imu-objs := virtual_imu_core.o virtual_imu_sysfs.o

KERNEL_DIR ?= /lib/modules/$(shell uname -r)/ build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
