
#include "s3c2440_soc.h"
#include "uart.h"
#include "init.h"

int main(void) {
	unsigned char c;
	
	//uart0_init();
	puts("Hello, world1111!\n\r");
	lcd_test();
	// puts("Hello, world1111!\n\r");
	// lcd_test();
	while(1)
	{
		c = getchar();
		if (c == '\r')
		{
			putchar('\n');
		}

		if (c == '\n')
		{
			putchar('\r');
		}

		putchar(c);
	}
	return 0;
}



