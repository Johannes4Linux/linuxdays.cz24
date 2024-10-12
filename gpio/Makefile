obj-m += gpio_test.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	gpioset 0 11=0 9=0 25=0 8=0
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
