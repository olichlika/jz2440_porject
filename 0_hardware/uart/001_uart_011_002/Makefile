CROSS_COMPILE = /root/jz2440_v3/gcc-3.4.5-glibc-2.3.6/bin/arm-linux-
all:
	$(CROSS_COMPILE)gcc -c -o led.o led.c
	$(CROSS_COMPILE)gcc -c -o uart.o uart.c
	$(CROSS_COMPILE)gcc -c -o main.o main.c
	$(CROSS_COMPILE)gcc -c -o start.o start.S
	$(CROSS_COMPILE)ld -Ttext 0x30000000 start.o led.o uart.o main.o -o uart.elf
	$(CROSS_COMPILE)objcopy -O binary -S uart.elf uart.bin
	$(CROSS_COMPILE)objdump -D uart.elf > uart.dis
clean:
	rm *.bin *.o *.elf *.dis
	
