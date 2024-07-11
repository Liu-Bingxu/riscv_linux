#include "lowlevelboot.h"

.macro loop,cunt
    li		t1,	0xffff
    li		t2,	\cunt
1:
	nop
	addi    t1, t1, -1
	bne		t1, x0, 1b
    li		t1,	0xffff
	addi    t2, t2, -1
	bne		t2, x0, 1b
.endm

.macro load_data,_src_start,_dst_start,_dst_end
	bgeu	\_dst_start, \_dst_end, 2f
1:
	REG_L   t0, (\_src_start)
	REG_S   t0, (\_dst_start)
	addi    \_src_start, \_src_start, RISCV_SZPTR
	addi    \_dst_start, \_dst_start, RISCV_SZPTR
	bltu    \_dst_start, \_dst_end, 1b
2:
.endm

	.section .data
	.align 4
	.globl  _pen
	.type	_pen,%object
_pen:
	RISCV_PTR   1

	.section .text
	.globl _start
	.type _start,@function
_start:
    csrr    a0, mhartid
	beq		a0, zero, _no_wait
_loop:
	loop	0x1000
	lla		t0, _pen
	REG_L   t0, 0(t0)
	beq		t0, zero, _run
	j       _loop
_no_wait:
	lla		t0, _pen
	li		t1, 1
	REG_S	t1, 0(t0)
	//load opensbi_fw.bin 
	//[OPENSBI_BIN_SRC_START:OPENSBI_BIN_SRC_END] --> [OPENSBI_BIN_DEST_START:OPENSBI_BIN_DEST_END]
    li		a0,	OPENSBI_BIN_SRC_START
    li		a1,	OPENSBI_BIN_DEST_START
	li		a2,	OPENSBI_BIN_DEST_END
	load_data a0,a1,a2

	//load qemu_sbi.dtb
	//[0x20080000:0x20100000] --> [0xBFF00000:0xBFF80000]
    // li		a0,	0x2008
	// slli	a0,	a0, 16
    // li		a1,	0xbff
	// slli	a1,	a1, 20
	// li		a2,	0x800
	// slli	a2,	a2, 4
	// add 	a2,	a1, a2
	// load_data a0,a1,a2

	//load trusted_fw.bin
	//[0x20400000:0x20800000] --> [0xBF800000:0xBFC00000]
    // li		a0,	0x204
	// slli	a0,	a0, 20
    // li		a1,	0xbf8
	// slli	a1,	a1, 20
    // li		a2,	0xbfc
	// slli	a2,	a2, 20
	// load_data a0,a1,a2

	//load qemu_uboot.dtb
	//[UBOOT_DTB_SRC_START:UBOOT_DTB_SRC_END] --> [UBOOT_DTB_DEST_START:UBOOT_DTB_DEST_END]
    li		a0,	UBOOT_DTB_SRC_START
    li		a1,	UBOOT_DTB_DEST_START
    li		a2,	UBOOT_DTB_DEST_END
	load_data a0,a1,a2

	//load u-boot.bin
	//[UBOOT_BIN_SRC_START:UBOOT_BIN_SRC_END] --> [UBOOT_BIN_DEST_START:UBOOT_BIN_DEST_END]
    li		a0,	UBOOT_BIN_SRC_START
    li		a1,	UBOOT_BIN_DEST_START
    li		a2,	UBOOT_BIN_DEST_END
	load_data a0,a1,a2

_run:
    csrr    a0, mhartid
    li		a1,	0
	li		t0,	OPENSBI_BIN_DEST_START
	lla		t1, _pen
	REG_S	zero, 0(t1)
    jr      t0


    .end
