#ifndef __LOWLEVELBOOT_H__
#define __LOWLEVELBOOT_H__

#if __riscv_xlen == 64
#define RISCV_PTR		.dword
#define RISCV_SZPTR		8
#define RISCV_LGPTR		3
#define REG_L           ld
#define REG_S           sd
#elif __riscv_xlen == 32
#define RISCV_PTR		.word
#define RISCV_SZPTR		4
#define RISCV_LGPTR		2
#define REG_L           lw
#define REG_S           sw
#else
#error "Unexpected __SIZEOF_POINTER__"
#endif

#define OPENSBI_BIN_SRC_START   0x80100000
#define OPENSBI_BIN_SRC_END     0x80200000

#define OPENSBI_BIN_DEST_START  0x9ff00000
#define OPENSBI_BIN_DEST_END    0xa0000000

#define UBOOT_DTB_SRC_START     0x80080000
#define UBOOT_DTB_SRC_END       0x80100000

#define UBOOT_DTB_DEST_START    0x83000000
#define UBOOT_DTB_DEST_END      0x83080000

#define UBOOT_BIN_SRC_START     0x80200000
#define UBOOT_BIN_SRC_END       0x80600000

#define UBOOT_BIN_DEST_START    0x88000000
#define UBOOT_BIN_DEST_END      0x88400000

#endif


