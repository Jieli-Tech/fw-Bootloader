# 总的 Makefile，用于调用目录下各个子工程对应的 Makefile
# 注意： Linux 下编译方式：
# 1. 从 http://pkgman.jieliapp.com/doc/all 处找到下载链接
# 2. 下载后，解压到 /opt/jieli 目录下，保证
#   /opt/jieli/common/bin/clang 存在（注意目录层次）
# 3. 确认 ulimit -n 的结果足够大（建议大于8096），否则链接可能会因为打开文件太多而失败
#   可以通过 ulimit -n 8096 来设置一个较大的值
# 支持的目标
# make bd19_uboot
# make sh54_uboot
# make br34_uboot
# make br28_uboot
# make wl82_uboot
# make br23_uboot
# make br30_uboot
# make br25_uboot
# make sh55_uboot

.PHONY: all clean bd19_uboot sh54_uboot br34_uboot br28_uboot wl82_uboot br23_uboot br30_uboot br25_uboot sh55_uboot clean_bd19_uboot clean_sh54_uboot clean_br34_uboot clean_br28_uboot clean_wl82_uboot clean_br23_uboot clean_br30_uboot clean_br25_uboot clean_sh55_uboot

all: bd19_uboot sh54_uboot br34_uboot br28_uboot wl82_uboot br23_uboot br30_uboot br25_uboot sh55_uboot
	@echo +ALL DONE

clean: clean_bd19_uboot clean_sh54_uboot clean_br34_uboot clean_br28_uboot clean_wl82_uboot clean_br23_uboot clean_br30_uboot clean_br25_uboot clean_sh55_uboot
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

br28_uboot:
	$(MAKE) -C cpu/br28 -f Makefile

clean_br28_uboot:
	$(MAKE) -C cpu/br28 -f Makefile clean

wl82_uboot:
	$(MAKE) -C cpu/wl82 -f Makefile

clean_wl82_uboot:
	$(MAKE) -C cpu/wl82 -f Makefile clean

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
