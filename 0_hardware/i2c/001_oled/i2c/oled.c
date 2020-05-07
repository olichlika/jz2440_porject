#include "i2c_controller.h"

#define OLED_ADDR 0x3c
#define OLED_CMD 0x0 //写命令
#define OLED_DATA 0x1 //写数据

int oled_write_byte(unsigned char data, int cmd) {
	i2c_msg msg;
	int err;
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
    msg.addr  = OLED_ADDR;
    msg.flags = 0; /* write */
    msg.len   = 2;
    msg.buf   = buf;
    msg.err   = 0;
    msg.cnt_transferred = -1;

    /* 调用i2c_transfer */
    err = i2c_transfer(&msg, 1);
    if (err)
        return err;	
	return 0;
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