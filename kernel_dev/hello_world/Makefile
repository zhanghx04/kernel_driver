VERSION = 6
PATCHLEVEL = 11
SUBLEVEL = 0
EXTRAVERSION = -rc2

obj-m += hello-1.o
obj-m += hello-2.o
obj-m += hello-3.o
obj-m += hello-4.o

PWD := $(CURDIR)

CC := x86_64-linux-gnu-gcc-13

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules CC=$(CC)
clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean