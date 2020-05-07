#include "lcd_controller.h"
#include "lcd.h"
#include "../s3c2440_soc.h"

#define HCLK 100

void jz2440_lcd_pin_init(void) {
	/* 初始化引脚 : 背光引脚 */
	GPBCON &= ~0x3;
	GPBCON |= 0x01;

	/* LCD专用引脚 */
	GPCCON = 0xaaaaaaaa;
	GPDCON = 0xaaaaaaaa;

	/* PWREN */
	GPGCON |= (3<<8);
}

/* 根据传入的LCD参数设置LCD控制器 */
void s3c2440_lcd_controller_init(p_lcd_params plcdparams) {
    unsigned int addr;

    jz2440_lcd_pin_init();
    //int clkval = (double)HCLK / plcdparams -> time_seq.vclk / 2 - 1; 不好编译
    //clkval = 100 / 10 / 2 - 1 = 4
    int clkval = 4;
    int bppmode = plcdparams -> bpp == 8  ? 0xb :\
				  plcdparams -> bpp == 16 ? 0xc :\
				  0xd;  /* 0xd: 24,32bpp */
	LCDCON1 = (clkval << 8) | (3 << 5) | (bppmode << 1) ;

    LCDCON2 = 	((plcdparams->time_seq.tvb - 1)<<24) | \
	            ((plcdparams->yres - 1)<<14)         | \
				((plcdparams->time_seq.tvf - 1)<<6)  | \
				((plcdparams->time_seq.tvp - 1)<<0);

    LCDCON3 =	((plcdparams->time_seq.thb - 1)<<19) | \
				((plcdparams->xres - 1)<<8)		      | \
				((plcdparams->time_seq.thf - 1)<<0);

    LCDCON4 =	((plcdparams->time_seq.thp - 1)<<0);

    int pixelplace = plcdparams->bpp == 32 ? (0) : \
	             plcdparams->bpp == 16 ? (1) : \
	             (1<<1);  /* 8bpp */
	
	LCDCON5 = (plcdparams->pins_pol.vclk<<10) |\
	          (plcdparams->pins_pol.rgb<<7)   |\
	          (plcdparams->pins_pol.hsync<<9) |\
	          (plcdparams->pins_pol.vsync<<8) |\
 			  (plcdparams->pins_pol.de<<6)    |\
			  (plcdparams->pins_pol.pwren<<5) |\
			  (1<<11) | pixelplace;

    addr = plcdparams->fb_base & ~(1<<31);//去除最高位
	LCDSADDR1 = (addr >> 1);//赋值fb_base的[31:1]

    addr = plcdparams->fb_base + plcdparams->xres*plcdparams->yres*plcdparams->bpp/8;
	addr >>=1;
	addr &= 0x1fffff;
	LCDSADDR2 = addr;
}

void s3c2440_lcd_controller_enalbe(void) {
	/* 背光引脚 : GPB0 */
	GPBDAT |= (1<<0);
	
	/* pwren    : 给LCD提供AVDD  */
	LCDCON5 |= (1<<3);
	
	/* LCDCON1'BIT 0 : 设置LCD控制器是否输出信号 */
	LCDCON1 |= (1<<0);
}

void s3c2440_lcd_controller_disable(void) {
	/* 背光引脚 : GPB0 */
	GPBDAT &= ~(1<<0);

	/* pwren	: 给LCD提供AVDD  */
	LCDCON5 &= ~(1<<3);

	/* LCDCON1'BIT 0 : 设置LCD控制器是否输出信号 */
	LCDCON1 &= ~(1<<0);
}

struct lcd_controller s3c2440_lcd_controller = {
	.name    = "s3c2440",
	.init    = s3c2440_lcd_controller_init,
	.enable  = s3c2440_lcd_controller_enalbe,
	.disable = s3c2440_lcd_controller_disable,
};

void s3c2440_lcd_contoller_add(void) {
	register_lcd_controller(&s3c2440_lcd_controller);
}