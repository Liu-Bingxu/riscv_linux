(1). linux
1. 添加sztulbx_blk驱动支持
    分别修改driver/block/Kconfig driver/block/Makefile和添加driver/block/sztulbx_blk.c文件
(2). opensbi
1. 添加platform/generic/sztulbx平台
2. 修改platform/generic/configs/defconfig默认配置打开sztulbx平台支持
3. 修改platform/generic/Kconfig添加sztulbx平台选项
4. 修改platform/generic/platform.c关闭console初始化时尝试semihost的操作
(3). uboot
1. 添加uboot/arch/riscv/dts/ysyxlbx_uboot.dts文件，描述板子
2. 修改uboot/arch/riscv/dts/Makefile，添加ysyxlbx的设备树编译
3. 修改uboot/arch/riscv/Kconfig，添加ysyxlbx的板子支持
4. 添加uboot/board/sztulbx，添加sztulbx板子
5. 添加uboot/configs/sztulbx_ysyxlbx_defconfig，默认配置
6. 添加uboot/include/configs/sztulbx_ysyxlbx.h，默认配置头文件



