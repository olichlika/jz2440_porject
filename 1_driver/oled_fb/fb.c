#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fb.h>
#include <linux/spi/spi.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/delay.h>

#include <linux/moduleparam.h>
#include <linux/kmod.h>
#include <linux/kthread.h>


#define X 128
#define Y 64

#define OLED_CMD 0x0 //写命令
#define OLED_DATA 0x1 //写数据

static struct fb_info *fbi;

struct fb_ops fops = {
};
typedef struct {
    struct task_struct *thread; //记录线程对象的地址，此线程专用于把显存数据发送到屏的驱动ic
} lcd_data_t;

static void show_fb(struct fb_info *fbi);

int thread_func(void *data) {
    struct fb_info *fbi = (struct fb_info *)data;
    lcd_data_t *ldata = fbi->par;
    while (1) {
        if(kthread_should_stop())
            break;
        show_fb(fbi);
    }
    return 0;
}
int myfb_init() {
    //u32 vmem_size;
    //u8 *vmem;
    lcd_data_t *data;

    // vmem_size = X * Y / 8;

    // vmem = vmalloc(vmem_size);

    /* 1. 分配一个fb_info */
	fbi = framebuffer_alloc(sizeof(lcd_data_t), NULL);

    data = fbi -> par;

    /* 2. 设置 */
	/* 2.1 设置固定的参数 */
    strcpy(fbi -> fix.id, "myfb");
    fbi -> fix.smem_len = X * Y * 8 / 8;
    // s3c_lcd->fix.type     = FB_TYPE_PACKED_PIXELS;
	// s3c_lcd->fix.visual   = FB_VISUAL_TRUECOLOR; /* TFT */
    fbi -> fix.line_length = X * 8 / 8;

    /* 2.2 设置可变的参数 */
    fbi->var.xres = X;
    fbi->var.yres = Y;
	fbi->var.xres_virtual = X;
	fbi->var.yres_virtual = Y;
    //s3c_lcd->var.bits_per_pixel = 16;

    fbi->var.red.length = 1;
	fbi->var.red.offset = 0;
	fbi->var.green.length = 1;
	fbi->var.green.offset = 0;
	fbi->var.blue.length = 1;
	fbi->var.blue.offset = 0;

    //s3c_lcd->var.activate       = FB_ACTIVATE_NOW;

    /* 2.3 设置操作函数 */
    fbi->fbops = &fops;

    /* 2.4 其他的设置 */
	//s3c_lcd->pseudo_palette = pseudo_palette;

    //s3c_lcd->screen_size   = 480*272*16/8;
    fbi->screen_base = dma_alloc_writecombine(NULL, fbi->fix.smem_len, &fbi->fix.smem_start, GFP_KERNEL);

    register_framebuffer(fbi);
    data -> thread = kthread_run(thread_func, fbi, "practice task");
    return 0;
}

int myfb_exit() {
    lcd_data_t *data = fbi->par;

    kthread_stop(data->thread); //让刷图线程退出

    unregister_framebuffer(fbi);
    dma_free_writecombine(NULL, fbi->fix.smem_len, fbi->screen_base, fbi->fix.smem_start);
    framebuffer_release(fbi);
}

void show_fb(struct fb_info *fbi) {
    uint8_t OLEDData[8][128];
    int x, y;
    u8 k;
    u8 *p = (u8 *)(fbi->screen_base);
    u8 index, offset;
    uint8_t i,j;

    for(y = 0; y < fbi->var.yres; y++) {
        for (x = 0; x < fbi->var.xres; x++) {
            k = p[y*fbi->var.xres+x];//取出8位数据
            index = y / 8;
	        offset = y % 8;
            if(k == 0){
                OLEDData[index][x] &= ~(1 << offset);
            } else {
                OLEDData[index][x] |= (1 << offset);
            }            
        }
    }

	for(i=0; i<8; i++) {  
		oled_write_byte(0xb0+i, OLED_CMD);
		oled_write_byte(0x00, OLED_CMD); 
		oled_write_byte(0x10, OLED_CMD);    
		for(j=0; j<128; j++){
			oled_write_byte(OLEDData[i][j], OLED_DATA);		
		}
	}
}