#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <linux/string.h>

#include "led_opr.h"

struct gpio {
	volatile unsigned int gpxcon;
	volatile unsigned int gpxdat;
};

/* GPIOB引脚操作结构体 */
static struct gpio *gpiof;


static int board_led_init(int which) {
	//printk("led%d init\n", which);
		
	/* init gpiof*/
	if(!gpiof){
		gpiof = ioremap(0x56000050, sizeof(struct gpio));
	}

	switch(which) {
		case 0:
			gpiof -> gpxcon &= ~(3 << 8);/* clear bit*/
			gpiof -> gpxcon |= (1 << 8);
			break;
		case 1:
			gpiof -> gpxcon &= ~(3 << 10);/* clear bit*/
			gpiof -> gpxcon |= (1 << 10);
			break;
		case 2:
			gpiof -> gpxcon &= ~(3 << 12);/* clear bit*/
			gpiof -> gpxcon |= (1 << 12);
			break;
		default:;
	}
	
	return 0;
}

static int board_led_ctl(int which, char status) {
	//printk("led%d %s\n", which, status);
	/*
	if(status == 0) {
		printk("led%d off\n", which);
	} else {
		printk("led%d on\n", which);
	}*/
	
	switch(which){
		case 0:
			if(status == 0) {
				gpiof -> gpxdat |= 1 << 4;
			} else {
				gpiof -> gpxdat &= ~(1 << 4);
			}

			break;
		case 1:
			if(status == 0) {
				gpiof -> gpxdat |= 1 << 5;
			} else {
				gpiof -> gpxdat &= ~(1 << 5);
			}

			break;
		case 2:
			if(status == 0) {
				gpiof -> gpxdat |= 1 << 6;
			} else {
				gpiof -> gpxdat &= ~(1 << 6);
			}

			break;
		default:;
			
	}
	return 0;
}

static struct led_operations board_led_opr = {
	.num = 3,
	.init = board_led_init,
	.ctl = board_led_ctl,
};

struct led_operations * get_board_led_opr() {
	return &board_led_opr;
}