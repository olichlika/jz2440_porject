KERNEL_DIR = /root/jz2440_v3/linux-2.6.22.6
CROSS_COMPILE = /root/jz2440_v3/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-
ARCH = arm

objs += ts.o

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=`pwd` modules
	#$(CROSS_COMPILE)gcc -o test test.c
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=`pwd` modules clean
	rm -rf test *.symvers

obj-m += ts.o