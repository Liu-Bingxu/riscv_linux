#
# SPDX-License-Identifier: BSD-2-Clause
#

carray-platform_override_modules-$(CONFIG_PLATFORM_SZTULBX_YSYXLBX) += sztulbx_ysyxlbx
platform-objs-$(CONFIG_PLATFORM_SZTULBX_YSYXLBX) += sztulbx/ysyxlbx.o

ifeq ($(CONFIG_PLATFORM_SZTULBX_YSYXLBX), y)
FW_JUMP_FDT_ADDR=0x9ff80000
FW_FDT_PATH = $(platform_build_dir)/sztulbx/ysyxlbx_sbi.dtb
FW_JUMP_ADDR = $(CONFIG_SZTU_LBX_JUMP_ADDR)
PLATFORM_RISCV_ABI = lp64
PLATFORM_RISCV_ISA = rv64imac_zicsr_zifencei
endif

