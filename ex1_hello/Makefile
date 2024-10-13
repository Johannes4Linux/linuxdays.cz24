# Kernel header makefile compiles hello_kernel.c automatically to hello_kernel.o
obj-m += hello_kernel.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
