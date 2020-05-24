
#include "s3c2440_soc.h"
#include "timer.h"

#define TIMER_NUM  32 //32个任务

p_timer_desc p_timer_desc_array[TIMER_NUM];

int register_timer(p_timer_desc ptimerdesc) {
	int i;
	for (i = 0; i < TIMER_NUM; i++) {
		if (!p_timer_desc_array[i]) {
			p_timer_desc_array[i] = ptimerdesc;
			// printf("register_timer\r\n");
			return i;
		}		
	}
	return -1;
}


void timer_irq(void) {
	int i;
	for (i = 0; i < TIMER_NUM; i++) {
		if (p_timer_desc_array[i]) {
			// printf("timer_irq\r\n");
			p_timer_desc_array[i] -> timer_func();
		}
	}
}

void timer_init(void) {
	/* 设置TIMER0的时钟 */
	/* Timer clk = PCLK / {prescaler value+1} / {divider value} 
	             = 50000000/(49+1)/16
	             = 62500
	 */
	TCFG0 = 49;  /* Prescaler 0 = 49, 用于timer0,1 */
	TCFG1 &= ~0xf;
	TCFG1 |= 3;  /* MUX0 : 1/16 */

	/* 设置TIMER0的初值 */
	TCNTB0 = 625;  /* 10Ms中断一次 */

	/* 加载初值, 启动timer0 */
	TCON |= (1<<1);   /* Update from TCNTB0 & TCMPB0 */

	/* 设置为自动加载并启动 */
	TCON &= ~(1<<1);
	TCON |= (1<<0) | (1<<3);  /* bit0: start, bit3: auto reload */

	/* 设置中断 */
	register_irq(10, timer_irq);
}


