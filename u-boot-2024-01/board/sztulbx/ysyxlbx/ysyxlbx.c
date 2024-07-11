#include <common.h>
#include <dm.h>
#include <env.h>
#include <fdtdec.h>
#include <image.h>
#include <log.h>
#include <init.h>
#include <console.h>
#include <serial.h>
#include "stdio.h"

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
    return 0;
}

int board_late_init(void)
{
    env_set_hex("kernel_start",
                CONFIG_SZTULBX_YSYXLBX_KERNEL_START);
    env_set_hex("kernel_fdt_addr",
                CONFIG_SZTULBX_YSYXLBX_KERNEL_FDT_ADDR);
    // printf("ram top is 0x%08llx", gd->ram_top);
    // printf("arch dtb is 0x%08llx", gd->arch.firmware_fdt_addr);
    return 0;
}

phys_size_t get_effective_memsize(void)
{
    return 384 * 1024 * 1024;
}


