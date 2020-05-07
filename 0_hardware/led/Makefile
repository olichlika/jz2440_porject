CROSS_COMPILE = /root/jz2440_v3/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-
ARCH = arm
all:
	$(CROSS_COMPILE)gcc -c -o led.o led.c
	$(CROSS_COMPILE)gcc -c -o start.o start.S
	$(CROSS_COMPILE)ld -Ttext 0x30000000 start.o led.o -o led.elf
	$(CROSS_COMPILE)objcopy -O binary -S led.elf led.bin
	$(CROSS_COMPILE)objdump -D led.elf > led.dis
clean:
	rm *.bin *.o *.elf *.dis