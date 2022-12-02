# fw-Bootloader
Customized bootloader for JL series(AC63/...)

JL 系列user boot固件程序

本仓库包含user boot release 版本代码，线下线上支持同步发布，支持用户进行自定义串口升级和 usb_hid 升级。

本工程提供的例子，需要结合对应命名规则的库文件(lib.a) 和对应的子仓库进行编译.

快速开始
------------

欢迎使用杰理开源项目，在开始进入项目之前，请详细阅读介绍文档

工具链
------------

使用的工具链与标准SDK一致。

介绍文档
------------

uboot 升级使用说明：[uboot升级使用说明](doc/uboot%E5%8D%87%E7%BA%A7%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8Ev1.1.2.md) 

uboot 升级协议流程：[uboot升级协议流程](doc/uboot%E5%8D%87%E7%BA%A7%E5%8D%8F%E8%AE%AE%E6%B5%81%E7%A8%8Bv1.1.2.md) 


SDK与BootLoader系列对应说明
------------
| SDK型号    | BootLoader对应  |
|  ----      | ----        |
| AC693N/AC693X  | bd19 |
| AC635N/AC695X/AC695N  | br23 |
| AC636N/AC696X/AC696N  | br25 |
| AC697N/AC897N  | br30 |
| AC638N/AD698N  | br34 |
| AD14N/AD104N   | sh54 |
| AD15N/AD105N   | sh55 |
| AC791N         | wl82 |
| AC701N         | br28 |

例如: SDK型号使用的是AC701N, 使用user_boot\cpu\br28\br28_uboot.cbp作为工程入口。

编译工程
-------------

SDK 同时支持Codeblock 和 Make 编译环境，请确保编译前已经搭建好编译环境，

* Codeblock 编译 : 进入对应的工程目录并找到后缀为 `.cbp` 的文件, 双击打开便可进行编译.
  

硬件环境
-------------

与标准SDK一致，生成的uboot.boot要添加到原SDK下载目录调试，流程见 [uboot升级使用说明](doc/uboot%E5%8D%87%E7%BA%A7%E4%BD%BF%E7%94%A8%E8%AF%B4%E6%98%8Ev1.1.2.md)

免责声明
------------

user boot 支持多系列芯片开发，鉴于boot的特殊性，请务必进行充分测试.
