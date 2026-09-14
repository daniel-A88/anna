# STM32 PC13 板载LED点灯实验
本项目基于 STM32F103，开发板自带LED接 PC13，实现1秒周期闪烁。

## 开发环境
- STM32CubeMX：引脚配置、生成初始化代码
- Keil MDK：代码编译
- STM32 HAL库

## 文件说明
- led_test.ioc：CubeMX工程文件，可打开查看引脚配置
- main.c：主程序，LED闪烁逻辑
- Project.uvprojx：Keil工程文件

## 使用步骤
1. 用CubeMX打开 led_test.ioc，确认芯片与PC13引脚配置
2. Keil打开 Project.uvprojx
3. 配置好STM32F1 HAL库，编译工程
4. ST-Link连接开发板，下载程序
5. PC13板载LED，每1秒亮灭翻转

## 重要提醒
仓库没有上传Drivers HAL库文件夹，下载工程后需要自行添加HAL库，否则编译报错。
PC13板载LED特性：低电平点亮，高电平熄灭。
