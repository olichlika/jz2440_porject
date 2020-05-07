KERNEL_DIR = /root/jz2440_v3/linux-2.6.22.6
CROSS_COMPILE = /root/jz2440_v3/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-
ARCH = arm

objs += i2c/oled.o
# objs += i2c/i2c_controller.o
#objs += i2c/i2c_test.o
# objs += i2c/s3c2440_i2c_controller.o
objs += s3c_i2c_oled.o

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=`pwd` modules
	$(CROSS_COMPILE)gcc -o s3c_i2c_oled_test s3c_i2c_oled_test.c
clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_DIR) M=`pwd` modules clean
	rm -rf s3c_i2c_oled_test *.symvers

100ask_oled-y := $(objs)
obj-m += 100ask_oled.o