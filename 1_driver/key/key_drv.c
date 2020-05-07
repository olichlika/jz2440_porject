#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>


/* define a struct */
struct pin_desc{
	unsigned int pin;
	unsigned int key_val;
};

struct pin_desc pins_desc[6] = {
	{0, 0x01},
	{0, 0x02},
	{0, 0x03},
};

static int major;
static struct class *button_class;

/* key value that return to application */
static unsigned char key_val;

static DECLARE_WAIT_QUEUE_HEAD(button_waitq);

/* interruption flag */
static volatile int ev_press = 0;

static irqreturn_t buttons_irq(int irq, void *dev_id) {
	struct pin_desc * p_pin_desc = (struct pin_desc *)dev_id;
	
    //printk("key_val = %d\n", p_pin_desc -> key_val);
	key_val = p_pin_desc -> key_val;

	ev_press = 1;                  /* flag set */
    wake_up_interruptible(&button_waitq);   /* wake up threading */
	
	
    return IRQ_RETVAL(IRQ_HANDLED);
}

static int button_open(struct inode * inode, struct file * file) {
	/* get minor number */
	int minor = MINOR(inode -> i_rdev);

	request_irq(IRQ_EINT0, buttons_irq, IRQ_TYPE_EDGE_FALLING, "KEY1", &pins_desc[0]);
	request_irq(IRQ_EINT2, buttons_irq, IRQ_TYPE_EDGE_FALLING, "KEY2", &pins_desc[1]);
	request_irq(IRQ_EINT11, buttons_irq, IRQ_TYPE_EDGE_FALLING, "KEY3", &pins_desc[2]);
	
	return 0;
}

static ssize_t button_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
	char status;
	
	/* get minor number */
	int minor = MINOR(file -> f_dentry -> d_inode -> i_rdev);
	
	return 0;
}

static ssize_t button_read(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
	if(count != 1)
		return -EINVAL;

	/* if not wakeup, sleep here */
	wait_event_interruptible(button_waitq, ev_press);
	
	copy_to_user(buf, &key_val, 1);

	ev_press = 0;/* clear flag */
	
	return 0;
}


static int button_release(struct inode * inode, struct file * file) {
	/* free interruption */
	free_irq(IRQ_EINT0, &pins_desc[0]);
	free_irq(IRQ_EINT2, &pins_desc[1]);
	free_irq(IRQ_EINT11, &pins_desc[2]);
	
	return 0;
}
static const struct file_operations button_fops = {
	.owner	= THIS_MODULE,
	.write	= button_write,
	.read	= button_read,
	.open	= button_open,
	.release = button_release,
};

static int __init button_init(void) {
	int i;

	button_class = class_create(THIS_MODULE, "button_class");
	if (IS_ERR(button_class))
		return PTR_ERR(button_class);

	major = register_chrdev(0, "button", &button_fops);
	if (major < 0) {
		class_destroy(button_class);
		return major;
	}

	//p_button_opr = get_board_button_opr();

	class_device_create(button_class, NULL, MKDEV(major, 0), NULL, "100ask_buttons");/* create deviece in /dev/* */
	
	return 0;
}

static void __exit button_exit(void) {

	device_destroy(button_class,  MKDEV(major, 0));

	unregister_chrdev(major, "button");
	class_destroy(button_class);
};

module_init(button_init);
module_exit(button_exit);
MODULE_LICENSE("Dual BSD/GPL");


