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
## 实验现象
STM32F103 板载 PC13 LED 周期闪烁，间隔约1秒亮灭切换。

## ST-LINK 接线说明（SWD下载）
ST-LINK <----> STM32开发板
- SWDIO  ↔ PA13棕色线
- SWCLK  ↔ PA14白色线
- GND    ↔ GND黑色线
- 3.3v--红色线

## 本次踩坑记录
1.发现cubemx里的PA0忘记改成输入模式，即GPIO-output
2.改完后要generate，弹出的界面不要选openfolder，直接关闭，到keil再选yes
3.外接LED时，低电平长引脚接正极，短引脚接PA0，高电平长引脚则是接PA0，段引脚接负极
4.想要让板子自带的小灯亮，一定要在cubemx上打开引脚PC13;

## 工程说明
- 1-pc13-single-LED：简易单点灯工程
- 2-pc13-pa0-dual-LED：CubeMX生成HAL库双LED工程
