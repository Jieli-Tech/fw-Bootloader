# uboot 升级协议

---

| 版本号 | 备注     |
| ------ | -------- |
| v1.1.2 | 初始版本 |

## 一、uboot工程选择
根据使用的芯片型号，选择对应的uboot工程。以AC632N为例，需要打开的工程为fw-Bootloader-main\user_boot\cpu\bd19\bd19_uboot.cbp。如图所示：

<br/>
<div align="center">
    <img src="./attch\uboot_select_project.png">
</div>
<br/>

## 二、升级模式选择
uboot升级有 “串口升级模式” 和 “USB_HID升级模式” 两种。根据需求进行选择。
### 1.串口升级模式配置
打开 Project build options 设置，选择 Compiler settings，#defines设置，添加 USB_MODE=0 即可将 uboot 工程配置成串口升级模式。如图所示：

<br/>
<div align="center">
    <img src="./attch\serial_update_mode_config.png">
</div>
<br/>

app\src\user.c文件，ut_device_mode(tx, rx, bud)函数设置串口TX脚，RX脚，波特率。如图所示：

<br/>
<div align="center">
    <img src="./attch\serial_config.png">
</div>
<br/>

### 2.USB_HID升级模式配置
打开 Project build options 设置，选择 Compiler settings，#defines设置，添加 USB_MODE=1 即可将 uboot 工程配置成USB_HID升级模式。如图所示：

<br/>
<div align="center">
    <img src="./attch\usb_hid_update_mode_config.png">
</div>
<br/>

选择 usb_hid 升级模式时，uboot 工程的 usb_vid，usb_pid 与 usb_hid 上位机的 usb_vid，usb_pid 需要保持一致 。

uboot 工程修改 usb_vid 和 usb_pid 方法如图所示：
<br/>
<div align="center">
    <img src="./attch\uboot_usb_vid_pid.png">
</div>
<br/>

usb_hid 上位机修改 usb_vid 和 usb_pid 方法如图所示：（打开pc_demo\usb_hid\main.cpp查看）
<br/>
<div align="center">
    <img src="./attch\usb_hid_usb_vid_pid.png">
</div>
<br/>

## 三、调试功能配置
### 1.使能调试打印功能
打开 Project build options 设置，选择 Compiler settings，#defines设置，添加 __DEBUG 即可使能调试打印功能。如图所示：

<br/>
<div align="center">
    <img src="./attch\debug_enable.png">
</div>
<br/>

### 2.调试打印脚和波特率配置
方法一：使用 isd_config.ini 配置文件中的引脚和波特率配置。如图所示：

<br/>
<div align="center">
    <img src="./attch\isd_ini_config.png">
</div>
<br/>

在 main 函数中调用 uart_init(uttx, ut_buad) 函数，如图所示：

<br/>
<div align="center">
    <img src="./attch\isd_ini_config_code.png">
</div>
<br/>

方法二：代码中直接设置打印脚和波特率，以PA5，1000000波特率为例。如图所示：

<br/>
<div align="center">
    <img src="./attch\set_tx_and_baud.png">
</div>
<br/>

## 四、升级触发方式配置
升级触发方式有 I/O 口检测触发和 sdk 软件复位触发。
### 1. I/O 口触发
进入uboot，main函数中，通过检测一个I/O的电平状态，选择是否跳转到升级流程。如图所示：

<br/>
<div align="center">
    <img src="./attch\io_detect.png">
</div>
<br/>

### 2.SDK软件复位触发
user.h文件中，使能 USE_UPGRADE_MAGIC 宏。如图所示：

<br/>
<div align="center">
    <img src="./attch\sdk_softreset.png">
</div>
<br/>

在 sdk 工程中，添加以下代码实现 SDK软件复位触发。（任意位置）

    extern u32 nvram_list[];
    #define NV_RAM_LIST_ADDR nvram_list
    static u8 uboot_uart_upgrade_mode_magic[8] = {'u', 'b', 'o', 'o', 't', 0x5a, 's', 't', };
    static u8 uboot_uart_upgrade_succ_magic[8] = {'u', 'b', 'o', 'o', 't', 0xa5, 'o', 'k', };
    void check_uboot_uart_upgrade() //检查 uboot 升级是否成功，须在 memory_init();前检测该标志
    {
        if (memcmp((char *)NV_RAM_LIST_ADDR, uboot_uart_upgrade_succ_magic, sizeof(uboot_uart_upgrade_succ_magic)) == 0) {
            memset((char *)NV_RAM_LIST_ADDR, 0, sizeof(uboot_uart_upgrade_succ_magic));
            log_info("uboot uart upgrade succ\n");
        }
    }
    void hw_mmu_disable(void);
    void chip_reboot_entry_uboot_uart_upgrade_mode()    // uboot 升级跳转函数
    {
        hw_mmu_disable();
        memcpy((char *)NV_RAM_LIST_ADDR, uboot_uart_upgrade_mode_magic, sizeof(uboot_uart_upgrade_mode_magic));
        cpu_reset();
    }

在 maskron_stubs.ld 文件，添加以下代码， 路径为 fw-Bootloader-main\user_boot\cpu\bd19\output 。如图所示：

    nvram_list = ABSOLUTE(0x800);

<br/>
<div align="center">
    <img src="./attch\sdk_softreset.png">
</div>
<br/>

不同的芯片型号，对应不同的参数：

    BR23: nvram_list = ABSOLUTE(0x10800);
    BR25: nvram_list = ABSOLUTE(0x10880);
    BR34: nvram_list = ABSOLUTE(0x28800);
    BD19: nvram_list = ABSOLUTE(0x800);

在需要执行 uboot 升级的位置，调用 chip_reboot_entry_uboot_uart_upgrade_mode() 函数即可。

升级完成后，sdk 调用 check_uboot_uart_upgrade() 函数检测升级是否成功（需要放在memory_init( ) 前）。

## 五、上位机使用
上位机使用QT编写，开放源码，用户可自行修改（版本号5.9以上，6.0以下）。

串口升级上位机界面说如下：

    1.选择对应的串口
    2.设置波特率
    3.升级 uboot（一般不勾选此项） 
    4.通信加密秘钥（uboot默认秘钥为 12345678（10进制），秘钥可在 user.c 文件，communication_key 变量修改）
    5.刷新串口
    6.升级文件选择
    7.开始升级
    注意：选择串口升级时，包长自定义，默认长度是4K。

<br/>
<div align="center">
    <img src="./attch\serial_upper_host_machine.png">
</div>
<br/>

USB_HID 上位机说明如下：（暂时没有图形界面）

    1. 打开 fw-Bootloader-main\pc_demo\usb_hid\build-out-bin 文件夹 ；
    2. 将生成的 jl_isd.bin 文件复制到文件夹 ；
    3. 打开 Powershell 窗口 ；
    4. 输入 .\UbootHid.exe ， 回车执行 ；
    5. 升级完成，复位；

<br/>
<div align="center">
    <img src="./attch\usb_hid_build_out_bin.png">
</div>
<br/>

<br/>
<div align="center">
    <img src="./attch\open_powershell_window.png">
</div>
<br/>

<br/>
<div align="center">
    <img src="./attch\UbootHid_exe.png">
</div>
<br/>

<br/>
<div align="center">
    <img src="./attch\update_ok.png">
</div>
<br/>

## 六、测试流程
    1. build uboot 工程，生成新的 uboot.boot 文件，路径为 cpu\bd19\output ；
    2. 复制 uboot.boot 文件到 sdk 的下载目录，即 \cpu\bd19\tools 文件夹 ；
    3. 编译下载 sdk 到小机，此时生成 jl_isd.bin 文件为程序A，先备份一下程序A ；
    4. 修改 sdk（如修改一些打印），再编译下载到小机， 此时生成 jl_isd.bin 文件为程序B ；
    5. 通过上面步骤，我们得到正在小机上运行的程序B，和待升级的程序A ；
    6. PC 与小机通过串口连接，小机上电，触发升级后进入升级模式等待升级（串口升级或 USB_HID 升级） ；
    7. PC 上位机设置好相应的参数（com口，波特率，秘钥，升级文件）；
    8. 点击 Start Update 即可开始升级；

## 七、注意事项
    1. 小机运行中的程序所使用的 uboot.boot 和待升级文件使用的 uboot.boot 需要一致 ；
    2. 建议将程序强制设置成 4K 对齐，在 isd_config.ini 文件添加以下代码；
    SPECIAL_OPT=0; 
    FORCE_4K_ALIGN=YES;
    3. 若无法使用4k对齐(代码空间不够)，请确保升级用的bin文件，是在用强制升级工具连接样机下载代码时生成的；
    4. 如果 isd_config.ini 文件有 EOFFSET=1；的配置，则需要在 isd_config.ini 文件里加上 GENERATE_TWO_BIN = YES ;用以生成 0K/4K 文件；
        然后根据 upgrade_eoffset（uboot 代码里有），等于 4k 就用 jl_isd_4K.bin,否则用 jl_isd_0K.bin。如果没有 EOFFSET=1 ;直接使用 jl_isd.bin 文件即可；
    5. 选择 usb_hid 升级时，由于 hid 传输包长最大为 64Byte，写 flash 的命令还会占用一些 Byte，所以实际写到 flash 的数据长度 = （64 - 写 flash 命令长度）；如图所示：（打开pc_demo\usb_hid\main.cpp查看）
<br/>
<div align="center">
    <img src="./attch\write_flash_max_len.png">
</div>
<br/>



