@echo off

@echo ********************************************************************************
@echo 			SDK BR30			
@echo ********************************************************************************
@echo %date%

cd %~dp0

set OBJDUMP=C:\JL\pi32\bin\llvm-objdump.exe
set OBJCOPY=C:\JL\pi32\bin\llvm-objcopy.exe
set ELFFILE=uboot.elf

%OBJDUMP% -section-headers %ELFFILE%


%OBJCOPY% -O binary -j .text %ELFFILE% uboot.bin
%OBJCOPY% -O binary -j .uboot.version %ELFFILE% uboot.version

set run_addr=0x2000
set load_addr=0x6000
set mask_addr=0x100000

uboot_lz4.exe uboot.bin uboot.lz4 %run_addr% %load_addr% rom.image %mask_addr%
BankLink.exe %run_addr% uboot.lz4 uboot.boot

uboot_package.exe --info uboot.version --uboot uboot.boot --output uboot.boot


