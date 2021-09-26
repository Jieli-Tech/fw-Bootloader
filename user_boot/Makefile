# 总的 Makefile，用于调用目录下各个子工程对应的 Makefile
# 支持的目标
# make bd19_uboot
# make sh54_uboot
# make br34_uboot
# make br23_uboot
# make br30_uboot
# make br25_uboot
# make sh55_uboot

.PHONY: all clean bd19_uboot sh54_uboot br34_uboot br23_uboot br30_uboot br25_uboot sh55_uboot clean_bd19_uboot clean_sh54_uboot clean_br34_uboot clean_br23_uboot clean_br30_uboot clean_br25_uboot clean_sh55_uboot

all: bd19_uboot sh54_uboot br34_uboot br23_uboot br30_uboot br25_uboot sh55_uboot
	@echo +ALL DONE

clean: clean_bd19_uboot clean_sh54_uboot clean_br34_uboot clean_br23_uboot clean_br30_uboot clean_br25_uboot clean_sh55_uboot
	@echo +CLEAN DONE

bd19_uboot:
	$(MAKE) -C cpu/bd19 -f Makefile

clean_bd19_uboot:
	$(MAKE) -C cpu/bd19 -f Makefile clean

sh54_uboot:
	$(MAKE) -C cpu/sh54 -f Makefile

clean_sh54_uboot:
	$(MAKE) -C cpu/sh54 -f Makefile clean

br34_uboot:
	$(MAKE) -C cpu/br34 -f Makefile

clean_br34_uboot:
	$(MAKE) -C cpu/br34 -f Makefile clean

br23_uboot:
	$(MAKE) -C cpu/br23 -f Makefile

clean_br23_uboot:
	$(MAKE) -C cpu/br23 -f Makefile clean

br30_uboot:
	$(MAKE) -C cpu/br30 -f Makefile

clean_br30_uboot:
	$(MAKE) -C cpu/br30 -f Makefile clean

br25_uboot:
	$(MAKE) -C cpu/br25 -f Makefile

clean_br25_uboot:
	$(MAKE) -C cpu/br25 -f Makefile clean

sh55_uboot:
	$(MAKE) -C cpu/sh55 -f Makefile

clean_sh55_uboot:
	$(MAKE) -C cpu/sh55 -f Makefile clean
