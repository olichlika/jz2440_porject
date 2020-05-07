#include <linux/module.h>
#include <linux/version.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/irq.h>

#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>

/* define a struct */
struct pin_desc{
	unsigned int pin;
	unsigned int key_val;
};

struct pin_desc pins_desc[6] = {
	{S3C2410_GPF0, KEY_L},
	{S3C2410_GPF2, KEY_S},
	{S3C2410_GPG3, KEY_ENTER}
};


static struct timer_list buttons_timer;

static struct pin_desc *irq_pd; /* record the pin status when interruption is happen */

static struct input_dev *buttons_dev;

static irqreturn_t buttons_irq(int irq, void *dev_id) {
	irq_pd = (struct pin_desc *)dev_id;
	mod_timer(&buttons_timer, jiffies + HZ / 5);
	
    return IRQ_RETVAL(IRQ_HANDLED);
}

static void buttons_timer_function(unsigned long data) {
	struct pin_desc * pindesc = irq_pd;
	unsigned int pinval;
	
	if(!pindesc)
		return;

	pinval = s3c2410_gpio_getpin(pindesc -> pin);

	if(pinval) {
		input_event(buttons_dev, EV_KEY, pindesc->key_val, 0);
		input_sync(buttons_dev);
	} else {
		input_event(buttons_dev, EV_KEY, pindesc->key_val, 1);
		input_sync(buttons_dev);
	}
}


static int __init button_init(void) {
	int i;

	/* init timer */
	init_timer(&buttons_timer);
	buttons_timer.function = buttons_timer_function;
	add_timer(&buttons_timer);

	/* 1. 分配一个input_dev结构体 */
	buttons_dev = input_allocate_device();

	/* 2. 设置 */
	/* 2.1 能产生哪类事件 */
	set_bit(EV_KEY, buttons_dev -> evbit);
	set_bit(EV_REP, buttons_dev -> evbit);
	
	/* 2.2 能产生这类操作里的哪些事件: L,S,ENTER,LEFTSHIT */
	set_bit(KEY_L, buttons_dev -> keybit);
	set_bit(KEY_S, buttons_dev -> keybit);
	set_bit(KEY_ENTER, buttons_dev -> keybit);
	
	/* 3. 注册 */
	input_register_device(buttons_dev);
	
	request_irq(IRQ_EINT0, buttons_irq, IRQT_BOTHEDGE, "KEY1", &pins_desc[0]);
	request_irq(IRQ_EINT2, buttons_irq, IRQT_BOTHEDGE, "KEY2", &pins_desc[1]);
	request_irq(IRQ_EINT11, buttons_irq, IRQT_BOTHEDGE, "KEY3", &pins_desc[2]);	
	/*
	button_class = class_create(THIS_MODULE, "button_class");
	if (IS_ERR(button_class))
		return PTR_ERR(button_class);

	major = register_chrdev(0, "button", &button_fops);
	if (major < 0) {
		class_destroy(button_class);
		return major;
	}
	*/

	//device_create(button_class, NULL, MKDEV(major, 0), NULL, "100ask_buttons");/* create deviece in /dev/* */
	
	
	return 0;
}

static void __exit button_exit(void) {
	/* free interruption */
	free_irq(IRQ_EINT0, &pins_desc[0]);
	free_irq(IRQ_EINT2, &pins_desc[1]);
	free_irq(IRQ_EINT11, &pins_desc[2]);
	
	del_timer(&buttons_timer);
	input_unregister_device(buttons_dev);
	input_free_device(buttons_dev);	
};

module_init(button_init);
module_exit(button_exit);
MODULE_LICENSE("Dual BSD/GPL");


