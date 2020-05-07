KERNEL_DIR = /root/jz2440_v3/linux-2.6.22.6
CROSS_COMPILE = /root/jz2440_v3/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-
ARCH = arm
all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=`pwd` modules
	$(CROSS_COMPILE)gcc -o lcd_test lcd_test.c
test:
	@echo $(wildcard *.c)
	@echo $(wildcard src/*.c)
	@echo $(notdir $(wildcard src/*.c))
echo:
	echo hello world
	@echo hello world
m:
	@echo `pwd`
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=`pwd` modules clean
	rm -rf led_drv_test *.symvers

obj-m += lcd.o