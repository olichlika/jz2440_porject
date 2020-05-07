#include "i2c_controller.h"
#include "s3c2440_soc.h"

static p_i2c_msg p_cur_msg;

void i2c_interrupt_func(int irq) {

    unsigned int iicstat = IICSTAT;//读取

    p_cur_msg->cnt_transferred++;

    /* 每传输完一个数据将产生一个中断 */

	/* 对于每次传输, 第1个中断是"已经发出了设备地址" */

	if (p_cur_msg->flags == 0) {	/* write */
        /* 对于第1个中断, 它是发送出设备地址后产生的
		 * 需要判断是否有ACK
		 * 有ACK : 设备存在
		 * 无ACK : 无设备, 出错, 直接结束传输
		 */
        if (p_cur_msg->cnt_transferred == 0)  {/* 第1次中断 */
            if (iicstat & (1<<0)) { //没有响应
                /* no ack */
				/* 停止传输 */
				IICSTAT = 0xd0;
				IICCON &= ~(1<<4);
				p_cur_msg->err = -1;
				printf("tx err, no ack!\n\r");
				return;
            }
        }

		if (p_cur_msg->cnt_transferred < p_cur_msg->len) {
			/* 对于其他中断, 要继续发送下一个数据
			 */
			IICDS = p_cur_msg -> buf[p_cur_msg -> cnt_transferred];
			IICCON &= ~(1<<4);
		} else {
			/* 停止传输 */
			IICSTAT = 0xd0;
			IICCON &= ~(1<<4);
		}
    } else {
        //read

    }
}

void s3c2440_i2c_con_init(void) {
    /* 配置IIC引脚 */
	GPECON &= ~((3<<28) | (3<<30));
	GPECON |= ((2<<28) | (2<<30));

	IICCON = (1<<7) | (0<<6) | (1<<5) | (30<<0);

	register_irq(27, i2c_interrupt_func);
}

int do_master_tx(p_i2c_msg msg) {
p_cur_msg = msg;
	
	msg->cnt_transferred = -1;
	msg->err = 0;
	
	/* 设置寄存器启动传输 */
	/* 1. 配置为 master tx mode */
	IICSTAT = 0x10;
		
	/* 2. 把从设备地址写入IICDS */
	IICDS = msg->addr<<1;
	
	/* 3. IICSTAT = 0xf0 , 数据即被发送出去, 将导致中断产生 */
	IICSTAT = 0xf0;

	printf("start to transmit ...\n\r");

	/* 后续的传输由中断驱动 */

	/* 循环等待中断处理完毕 */
	while (!msg->err && msg->cnt_transferred != msg->len);

	if (msg->err)
		return -1;
	else
		return 0;
}

int do_master_rx(p_i2c_msg msg) {

}

int s3c2440_master_xfer(p_i2c_msg msgs, int num){
    int i;
	int err;
	
	for (i = 0; i < num; i++) {
		if (msgs[i].flags == 0)/* write */
		{
			err = do_master_tx(&msgs[i]);
			if (err)
			{
				printf("do_master_tx err!\n\r");
				return err;
			}
		}
		else
		{
			err = do_master_rx(&msgs[i]);
			if (err)
			{
				printf("do_master_rx err!\n\r");
				return err;
			}
		}
		if (err)
			return err;
    }
    return 0;
}

static i2c_controller s3c2440_i2c_con = {
	.name = "s3c2440",
	.init = s3c2440_i2c_con_init,
	.master_xfer = s3c2440_master_xfer,
};

void s3c2440_i2c_con_add(void) {
	register_i2c_controller(&s3c2440_i2c_con);
}