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
// #include "oledfont.h"

static struct i2c_driver oled_driver;
static struct i2c_client *new_client;

static int auto_major;

static unsigned short ignore[] = {I2C_CLIENT_END};
static unsigned short normal_addr[] = {0x3c,I2C_CLIENT_END};

static struct i2c_client_address_data addr_data = {
	.normal_i2c = normal_addr,  /* 要发出S信号和地址信号并得到ACK信号才能确定是否存在这个设备 */
	.probe      = ignore,
	.ignore     = ignore,
	//.forces   = forces,  /* 强制认为存在这个设备 */
};
static ssize_t oled_write(struct file *file, const char __user *buf, size_t size, loff_t *offset){
	unsigned char val[64];

	printk("oled_write\r\n");

	copy_from_user(val, buf, 64);

	oled_show_string(0, 0, val);

    return 0;
}

int oled_open (struct inode * inode, struct file *file) {
	printk("oled_open\r\n");
	register_client(new_client);
	
	init_oled();
    
    OLED_Clear();

    return 0;
}

static struct file_operations oled_fops = {
	.owner = THIS_MODULE,
	//.read  = oled_read,
	.write = oled_write,
	.open  = oled_open,
};
static struct class *cls;

static int oled_detect (struct i2c_adapter *adapter, int address, int kind) {
	/* 构造一个i2c_client结构体：以后收发数据时会用到它 */
	new_client = kzalloc(sizeof(struct i2c_client),GFP_KERNEL);
	new_client->addr    = address;
	new_client->adapter = adapter;
	new_client->driver  = &oled_driver;
	strcpy(new_client->name,"oled");

	i2c_attach_client(new_client);

	auto_major = register_chrdev(0,"oled",&oled_fops);
	cls = class_create(THIS_MODULE,"oled");
	class_device_create(cls,NULL,MKDEV(auto_major,0),NULL,"oled");
		
	return 0;
}

static int oled_attach(struct i2c_adapter *adapter) {

	return i2c_probe(adapter, &addr_data, oled_detect);	
}

static int oled_detach(struct i2c_client *client) {
	class_device_destroy(cls,MKDEV(auto_major,0));
	class_destroy(cls);

	unregister_chrdev(auto_major,"oled");

	i2c_detach_client(client);
	kfree(i2c_get_clientdata(client));
	return 0;
}

static struct i2c_driver oled_driver = {
	.driver = {
		.name = "oled" ,
	},
	
	.attach_adapter = oled_attach,
	.detach_client  = oled_detach,    
};

static int s3c_i2c_init(void) {
	i2c_add_driver(&oled_driver);

	return 0;
}

static void s3c_i2c_exit(void) {
	i2c_del_driver(&oled_driver);
}

module_init(s3c_i2c_init);
module_exit(s3c_i2c_exit);

MODULE_LICENSE("GPL");