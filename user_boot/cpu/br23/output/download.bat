@echo off

@echo ********************************************************************************
@echo 			BR23 UBOOT			
@echo ********************************************************************************
@echo %date%

cd %~dp0

set OBJDUMP=C:\JL\pi32\bin\llvm-objdump.exe
set OBJCOPY=C:\JL\pi32\bin\llvm-objcopy.exe
set ELFFILE=uboot.elf

%OBJDUMP% -section-headers %ELFFILE%


%OBJCOPY% -O binary -j .text %ELFFILE% uboot.bin
%OBJCOPY% -O binary -j .version %ELFFILE% uboot.version

BankLink.exe 0x4000 uboot.bin uboot.boot 

uboot_package.exe --info uboot.version --uboot uboot.boot --output uboot.boot


