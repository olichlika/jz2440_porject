
.text
.global _start

_start:

	/* 设置内存: sp 栈 */
	ldr sp, =0x34000000
	//ldr sp, =4096  /* nand启动 */
//	ldr sp, =0x40000000+4096  /* nor启动 */

	/* 调用main */
	bl main

halt:
	b halt
	
