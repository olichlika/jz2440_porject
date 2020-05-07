CROSS_COMPILE = /root/jz2440_v3/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-
#/root/toolschain/4.4.3/bin/arm-none-linux-gnueabi-
#/root/jz2440_v3/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-
objs = start.o uart.o init.o nand_flash.o main.o exception.o interrupt.o

objs += my_printf.o
objs += string_utils.o
objs += lib1funcs.o
#objs += lcd/font.o
#objs += lcd/framebuffer.o
#objs += lcd/geometry.o
objs += lcd/lcd.o
objs += lcd/lcd_4_3.o
objs += lcd/lcd_controller.o
objs += lcd/s3c2440_lcd_controller.o
objs += lcd/lcd_test.o

all: $(objs)
	$(CROSS_COMPILE)ld -T lcd.lds $^ -o lcd.elf
	$(CROSS_COMPILE)objcopy -O binary -S lcd.elf lcd.bin
	$(CROSS_COMPILE)objdump -D lcd.elf > lcd.dis
clean:
	rm *.bin *.o *.elf *.dis
	
%.o : %.c
	$(CROSS_COMPILE)gcc -march=armv4 -c -o $@ $<

%.o : %.S
	$(CROSS_COMPILE)gcc -march=armv4 -c -o $@ $<	
	
