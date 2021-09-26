
# make 编译并下载
# make VERBOSE=1 显示编译详细过程
# make clean 清除编译临时文件

# 工具路径设置
ifeq ($(OS), Windows_NT)
# Windows 下工具链位置
TOOL_DIR := C:/JL/pi32/bin
CC    := clang.exe
CXX   := clang.exe
LD    := lto-wrapper.exe
AR    := lto-ar.exe
MKDIR := mkdir_win -p
RM    := rm -rf

SYS_LIB_DIR := C:/JL/pi32/libc
SYS_INC_DIR := C:/JL/pi32/include/libc

## 后处理脚本
POST_SCRIPT     := ..\..\cpu\sh55\output\download.bat
RUN_POST_SCRIPT := $(POST_SCRIPT)
else
# Linux 下工具链位置
TOOL_DIR := /opt/jieli/pi32/bin
CC    := clang
CXX   := clang++
LD    := lto-wrapper
AR    := lto-ar
MKDIR := mkdir -p
RM    := rm -rf

SYS_LIB_DIR := $(TOOL_DIR)/../lib
SYS_INC_DIR := $(TOOL_DIR)/../include

## 后处理脚本
POST_SCRIPT     := ../../cpu/sh55/output/download.sh
RUN_POST_SCRIPT := bash $(POST_SCRIPT)
endif

CC  := $(TOOL_DIR)/$(CC)
CXX := $(TOOL_DIR)/$(CXX)
LD  := $(TOOL_DIR)/$(LD)
AR  := $(TOOL_DIR)/$(AR)
# 输出文件设置
OUT_ELF   := ../../cpu/sh55/output/uboot.elf
OBJ_FILE  := $(OUT_ELF).objs.txt
# 编译路径设置
BUILD_DIR := objs
# 工程路径前缀
ROOT_PREFIX := ../..

# 编译参数设置
CFLAGS := \
	-target pi32 \
	-integrated-as \
	-fno-builtin \
	-mllvm -pi32-memreg-opt \
	-mllvm -pi32-mem-offset-adj-opt \
	-mllvm -pi32-const-spill \
	-mllvm -pi32-enable-jz \
	-mllvm -pi32-tailcall-opt \
	-mllvm -inline-threshold=5 \
	-mllvm -pi32-enable-itblock=1 \
	-Oz \
	-flto \
	-integrated-as \
	-fallow-pointer-null \
	-fno-common \
	-flto \
	-Oz \
	-Wno-format \
	-Wno-unused-value \
	-Wno-unused-function \
	-Wno-unused-comparison \
	-Wno-parentheses \
	-Wno-tautological-compare \
	-Wno-incompatible-library-redeclaration \
	-Wno-invalid-noreturn \
	-Wno-visibility \
	-Wno-integer-overflow \


# 宏定义
DEFINES := \
	-D__CPU_sh55 \
	-DENTRY_ADDR=0 \
	-D__USE_MASK_API__ \
	-D__HID_UART_MODE_SEL=1 \


# 头文件搜索路径
INCLUDES := \
	-I../../app \
	-I../../app/inc \
	-I../../include_lib \
	-I../../include_lib/common \
	-I../../include_lib/drivers \
	-I../../include_lib/cpu/sh55 \
	-I../../include_lib/drivers/norflash \
	-I../../include_lib/fs/jlfs \
	-I../../include_lib/upgrade \
	-I$(SYS_INC_DIR) \


# 需要编译的 .c 文件
c_SRC_FILES := \
	../../app/src/descriptor.c \
	../../app/src/hid.c \
	../../app/src/main.c \
	../../app/src/user.c \
	../../cpu/sh55/delay.c \
	../../cpu/sh55/uart.c \


# 需要编译的 .S 文件
S_SRC_FILES :=


# 需要编译的 .s 文件
s_SRC_FILES :=


# 需要编译的 .cpp 文件
cpp_SRC_FILES :=


# 链接参数
LFLAGS := \
	--gc-sections \
	--plugin-opt=-dont-used-symbol-list=malloc,free,sprintf,printf,puts,putchar,perror,vprintf,printi,fopen,fread,fwrite,fseek,strlen,memset,memcpy,strcpy,memmove,strcmp \
	-M=../../cpu/sh55/output/map.txt \
	-T../../cpu/sh55/output/ram.ld \
	--start-group \
	--whole-archive \
	../../include_lib/liba/sh55/uboot.a \
	--no-whole-archive \
	--end-group \
	--plugin-opt=-pi32-memreg-opt \
	--plugin-opt=-pi32-mem-offset-adj-opt \
	--plugin-opt=-pi32-const-spill \
	--plugin-opt=-pi32-enable-jz \
	--plugin-opt=-pi32-tailcall-opt \
	--plugin-opt=-inline-threshold=5 \
	--plugin-opt=-pi32-enable-itblock=1 \


LIBPATHS := \
	-L$(SYS_LIB_DIR) \


LIBS := \
	C:/JL/pi32/lib/libm.a \
	C:/JL/pi32/lib/libc.a \
	C:/JL/pi32/lib/libm.a \
	C:/JL/pi32/lib/libcompiler-rt.a \



c_OBJS    := $(c_SRC_FILES:%.c=%.c.o)
S_OBJS    := $(S_SRC_FILES:%.S=%.S.o)
s_OBJS    := $(s_SRC_FILES:%.s=%.s.o)
cpp_OBJS  := $(cpp_SRC_FILES:%.cpp=%.cpp.o)

OBJS      := $(c_OBJS) $(S_OBJS) $(s_OBJS) $(cpp_OBJS)
DEP_FILES := $(OBJS:%.o=%.d)


OBJS      := $(addprefix $(BUILD_DIR)/, $(OBJS:$(ROOT_PREFIX)/%=%))
DEP_FILES := $(addprefix $(BUILD_DIR)/, $(DEP_FILES:$(ROOT_PREFIX)/%=%))


VERBOSE ?= 0
ifeq ($(VERBOSE), 1)
QUITE :=
else
QUITE := @
endif

.PHONY: all clean pre_build

all: pre_build $(OUT_ELF)
	@echo +POST-BUILD
	$(QUITE) $(RUN_POST_SCRIPT) uboot

pre_build:
	@echo +PRE-BUILD
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -D__LD__ -E -P ../../cpu/sh55/output/ram_ld.c -o ../../cpu/sh55/output/ram.ld

clean:
	$(QUITE) $(RM) $(OUT_ELF)
	$(QUITE) $(RM) $(OBJS)
	$(QUITE) $(RM) $(DEP_FILES)

$(OUT_ELF): $(OBJS)
	@echo +LINK $@
	$(file >$(OBJ_FILE), $(OBJS))
	$(QUITE) $(LD) -o $(OUT_ELF) @$(OBJ_FILE) $(LFLAGS) $(LIBPATHS) $(LIBS)

$(BUILD_DIR)/%.c.o : $(ROOT_PREFIX)/%.c
	@echo +CC $<
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.S.o : $(ROOT_PREFIX)/%.S
	@echo +AS $<
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.s.o : $(ROOT_PREFIX)/%.s
	@echo +AS $<
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.cpp.o : $(ROOT_PREFIX)/%.cpp
	@echo +CXX $<
	@$(MKDIR) $(@D)
	@$(CXX) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT "$@" $< -o $(@:.o=.d)
	$(QUITE) $(CXX) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

-include $(DEP_FILES)
