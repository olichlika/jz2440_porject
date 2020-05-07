#include <linux/fs.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/bcd.h>
#include <linux/smp_lock.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/smp_lock.h>
#include <asm/system.h>

#include "led_opr.h"

static int major;
static struct class *led_class;

struct led_operations *p_led_opr;

static int led_open(struct inode * inode, struct file * file) {
	/* get minor number */
	int minor = MINOR(inode -> i_rdev);

	/* int (*init)(int which); */
	p_led_opr -> init(minor);
	
	return 0;
}

static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
	char status;
	
	/* get minor number */
	int minor = MINOR(file -> f_dentry -> d_inode -> i_rdev);

	//printk("buf:%s \n", buf);
	
	copy_from_user(&status, buf, 1);

	//printk("status:%d \n", status);
		
	/* int (*ctl)(int which, char status); */
	p_led_opr -> ctl(minor, status);
	
	return 0;
}

static int led_release(struct inode * inode, struct file * file) {
	return 0;
}
static const struct file_operations led_fops = {
	.owner	= THIS_MODULE,
	.write	= led_write,
	.open	= led_open,
	.release = led_release,
};

static int __init led_init(void) {
	int i;

	//printk("led_init\n");
	
	led_class = class_create(THIS_MODULE, "led_class");
	if (IS_ERR(led_class))
		return PTR_ERR(led_class);

	major = register_chrdev(0, "led", &led_fops);
	if (major < 0) {
		class_destroy(led_class);
		return major;
	}

	p_led_opr = get_board_led_opr();

	for(i = 0; i < p_led_opr -> num; i++){
		class_device_create(led_class, NULL, MKDEV(major, i), NULL, "100ask_led%d", i);/* create deviece in /dev/* */
	}
	return 0;
}

static void __exit led_exit(void) {
	//printk("led_exit\n");
	int i;
	for(i = 0; i < p_led_opr -> num; i++){
		device_destroy(led_class,  MKDEV(major, i));
	}
	unregister_chrdev(major, "led");
	class_destroy(led_class);
};

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("Dual BSD/GPL");


