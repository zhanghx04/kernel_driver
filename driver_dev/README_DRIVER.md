[**<<< Back to Main >>>**](../README.md)

# Driver Development

> Learning source: **Linux Device Driver, 3rd Edition**

## How to build and run the module
```bash
# module name: module_name

make
sudo insmod module_name.ko

# verify the module is loaded
lsmod 
lsmod | grep module_name

# print the module information
sudo dmesg | tail

# Remove the module
sudo rmmod module_name

# Check the module is removed
lsmod | grep module_name
```