#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <asm-arm/uaccess.h>
#include <linux/mutex.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "oledfont.h"

#define OLED_ADDR 0x3c
#define OLED_CMD 0x0 //写命令
#define OLED_DATA 0x1 //写数据

static struct i2c_client *new_client;

int oled_write_byte(unsigned char data, int cmd) {
	struct i2c_msg msg[1];
	int ret;
	char buf[2];

    if(cmd == OLED_CMD){
        //写命令
        buf[0] = 0x00;
    } else {
        //写数据
        buf[0] = 0x40;
    }
    
    buf[1] = data;

    /* 构造i2c_msg */
    msg[0].addr  = new_client->addr;
    msg[0].flags = 0; /* write */
    msg[0].len   = 2;
    msg[0].buf   = buf;

    /* 调用i2c_transfer */
    //ret = i2c_transfer(&msg, 1);
	ret = i2c_transfer(new_client -> adapter, msg, 1);
    if(ret == 1)
		return 2;
	else
		return -EIO;
}

void register_client(struct i2c_client *p) {
	new_client = p;
}


void init_oled(void){
	oled_write_byte(0xAE, OLED_CMD);//--display off
	oled_write_byte(0x00, OLED_CMD);//---set low column address
	oled_write_byte(0x10, OLED_CMD);//---set high column address
	oled_write_byte(0x40, OLED_CMD);//--set start line address  
	oled_write_byte(0xB0, OLED_CMD);//--set page address
	oled_write_byte(0x81, OLED_CMD);// contract control
	oled_write_byte(0x01, OLED_CMD);//--128   
	oled_write_byte(0xA1, OLED_CMD);//set segment remap 
	oled_write_byte(0xA6, OLED_CMD);//--normal / reverse
	oled_write_byte(0xA8, OLED_CMD);//--set multiplex ratio(1 to 64)
	oled_write_byte(0x3F, OLED_CMD);//--1/32 duty
	oled_write_byte(0xC8, OLED_CMD);//Com scan direction
	oled_write_byte(0xD3, OLED_CMD);//-set display offset
	oled_write_byte(0x00, OLED_CMD);//
	
	oled_write_byte(0xD5, OLED_CMD);//set osc division
	oled_write_byte(0x80, OLED_CMD);//
	
	oled_write_byte(0xD8, OLED_CMD);//set area color mode off
	oled_write_byte(0x05, OLED_CMD);//
	
	oled_write_byte(0xD9, OLED_CMD);//Set Pre-Charge Period
	oled_write_byte(0xF1, OLED_CMD);//
	
	oled_write_byte(0xDA, OLED_CMD);//set com pin configuartion
	oled_write_byte(0x12, OLED_CMD);//
	
	oled_write_byte(0xDB, OLED_CMD);//set Vcomh
	oled_write_byte(0x30, OLED_CMD);//
	
	oled_write_byte(0x8D, OLED_CMD);//set charge pump enable
	oled_write_byte(0x14, OLED_CMD);//
	
	oled_write_byte(0xAF, OLED_CMD);//--turn on oled panel	
}

void OLED_Clear(void) {
    int i, j;
    for(i = 0; i < 8; i++) {
		oled_write_byte(0xb0+i, OLED_CMD);//设置页地址（0~7）
		oled_write_byte(0x00, OLED_CMD);//设置显示位置―列低地址
		oled_write_byte(0x10, OLED_CMD);//设置显示位置―列高地址 
        for(j = 0; j < 128; j++) {
			oled_write_byte(0, OLED_DATA);
        }
    }
}

void oled_set_pos(int x, int y) {
    oled_write_byte(0xb0 + y, OLED_CMD);
	oled_write_byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	oled_write_byte((x & 0x0f), OLED_CMD);
}

void oled_draw_point(int x, int y) {
	oled_set_pos(x, y);
	oled_write_byte(1, OLED_DATA);
}

void oled_show_char(int x, int y, unsigned char chr, unsigned char Char_Size) {      	
	unsigned char c = 0, i = 0;	
	c = chr - ' ';//得到偏移后的值			
	if(x > 128 - 1){
		x=0;y=y+2;
	}
	if(Char_Size == 16) {
		oled_set_pos(x, y);	
		for(i = 0; i < 8; i++)
			oled_write_byte(F8X16[c*16+i], OLED_DATA);
		oled_set_pos(x ,y + 1);
		for(i = 0; i < 8; i++)
			oled_write_byte(F8X16[c*16+i+8], OLED_DATA);
	} else {	
		oled_set_pos(x, y);
		for(i = 0; i < 6; i++)
			oled_write_byte(F6x8[c*6+i], OLED_DATA);	
	}
}

void oled_show_string(int x, int y, unsigned char *chr) {
	unsigned char j=0;
	while (chr[j]!='\0') {		
		oled_show_char(x, y, chr[j], 8);
		x += 6;
		if(x > 122) {
			x = 0;
			y += 2;
		}
		j++;
	}
}