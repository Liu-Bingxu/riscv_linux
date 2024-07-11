#include <libfdt.h>
#include <platform_override.h>
#include <sbi/sbi_error.h>
#include <sbi/sbi_bitops.h>
#include <sbi/sbi_system.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_timer.h>
#include <sbi/riscv_io.h>
#include <sbi_utils/fdt/fdt_helper.h>
#include <sbi_utils/reset/fdt_reset.h>
#include <sbi_utils/i2c/fdt_i2c.h>


static void sztulbx_ysyxlbx_fw_init(void *fdt, const struct fdt_match *match){
	volatile uint32_t *next_addr = (uint32_t *)(uint32_t)CONFIG_SZTU_LBX_JUMP_ADDR;
	(*next_addr)		     = 0x00100073;
}

static const struct fdt_match sztulbx_ysyxlbx_match[] = {
	{ .compatible = "sztulbx" },
	{ .compatible = "sztulbx, ysyxlbx" },
	{},
};

const struct platform_override sztulbx_ysyxlbx = {
	.match_table            = sztulbx_ysyxlbx_match,
	.fw_init                = sztulbx_ysyxlbx_fw_init,
	// .final_init	            = sztulbx_ysyxlbx_final_init,
};
