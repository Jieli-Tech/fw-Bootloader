# fw-AC63_Bootloader
Customized bootloader for AC63 series

A63 系列user boot固件程序

本仓库包含user boot release 版本代码，线下线上支持同步发布，支持用户进行自定义串口升级.

本工程提供的例子，需要结合对应命名规则的库文件(lib.a) 和对应的子仓库进行编译.

快速开始
------------

欢迎使用杰理开源项目，在开始进入项目之前，请详细阅读SDK 介绍文档 [说明文档](doc/user_uboot_demo使用说明.pdf)

工具链
------------

使用的工具链与标准SDK一致。

介绍文档
------------

SDK 简介 :见 [说明文档](doc/user_uboot_demo使用说明.pdf) 


编译工程
-------------

SDK 同时支持Codeblock 和 Make 编译环境，请确保编译前已经搭建好编译环境，

* Codeblock 编译 : 进入对应的工程目录并找到后缀为 `.cbp` 的文件, 双击打开便可进行编译.
  

硬件环境
-------------

与标准SDK一致，生成的uboot.boot要添加到原SDK下载目录调试，流程见 [说明文档](doc/user_uboot_demo使用说明.pdf) 


免责声明
------------

user boot 支持多系列芯片开发，鉴于boot的特殊性，请务必进行充分测试.
