#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>

#define CONFIG_SYS_SDRAM_BASE 0x80000000
#define CONFIG_SYS_INIT_SP_ADDR (CONFIG_SYS_SDRAM_BASE + SZ_2M)

// #define CONFIG_SYS_LOAD_ADDR (CONFIG_SYS_SDRAM_BASE + SZ_2M)

// #define CONFIG_SYS_MALLOC_LEN SZ_8M

// #define CONFIG_SYS_BOOTM_LEN SZ_64M

#define CONFIG_STANDALONE_LOAD_ADDR 0x80600000

#define RISCV_MMODE_TIMERBASE 0x2000000
#define RISCV_MMODE_TIMER_FREQ 1000000

#define RISCV_SMODE_TIMER_FREQ 1000000

/* Environment options */

#define CONFIG_SZTULBX_YSYXLBX_KERNEL_START    0x80600000
#define CONFIG_SZTULBX_YSYXLBX_KERNEL_FDT_ADDR 0x83000000

#ifndef CONFIG_SPL_BUILD
#define BOOT_TARGET_DEVICES(func) \
    func(SZTULBX, sztulbx, 0)          

#include <config_distro_bootcmd.h>

#define BOOTENV_DEV_SZTULBX(devtypeu, devtypel, instance) \
    "bootcmd_sztulbx,ysyxlbx="                            \
    "if env exists kernel_start; then "                   \
    "booti ${kernel_start} - ${kernel_fdt_addr};"          \
    "fi;\0"

#define BOOTENV_DEV_NAME_SZTULBX(devtypeu, devtypel, instance) \
    "sztulbx,ysyxlbx"

#define CFG_EXTRA_ENV_SETTINGS      \
    "fdt_high=0xffffffffffffffff\0"    \
    "initrd_high=0xffffffffffffffff\0" \
    "kernel_addr_r=0x84000000\0"       \
    "fdt_addr_r=0x88000000\0"          \
    "scriptaddr=0x88100000\0"          \
    "pxefile_addr_r=0x88200000\0"      \
    "ramdisk_addr_r=0x88300000\0" BOOTENV
#endif

#endif /* __CONFIG_H */

