# STM32 LED点灯
本项目基于 STM32F103，开发板自带LED接 PC13，实现1秒周期闪烁。
外接一个小灯，实现双灯共亮
外接三个小灯，三个小灯依次闪烁
外接2个按键与小灯，分别控制
外接按键与小灯，按下按键打乱小灯原来的闪烁
本项目基于STM32F103，分为串口轮询、串口中断、串口DMA三组实验，实现电脑和单片机互相收发数据，串口控制LED亮灭。
- 1‑轮询模式：阻塞式收发，在main，while里面`HAL_UART_Transmit`、`HAL_UART_Receive`
- 
 2‑串口中断：串口接收中断，收到数据立刻进入回调函数（void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)）处理，HAL_UART_Transmit_IT 、HAL_UART_receive_IT 
  
 3‑串口DMA：DMA搬运数据，CPU不用介入搬运，效率最高，HAL_UART_Transmit_DMA

## 开发环境
- STM32CubeMX：引脚配置、生成初始化代码
- Keil MDK：代码编译
- STM32 HAL库

## 文件说明
- led_test.ioc：CubeMX工程文件，可打开查看引脚配置
- main.c：主程序，LED
- Project.uvprojx：Keil工程文件

## 使用步骤
--1
1. 用CubeMX打开 led_test.ioc，确认芯片与PC13引脚配置
2. Keil打开 Project.uvprojx
3. 配置好STM32F1 HAL库，编译工程
4. ST-Link连接开发板，下载程序
5. PC13板载LED，每1秒亮灭翻转

--3
1.重新命名小灯例如LED_BLUE
2.两种代码控制小灯闪烁：一种是set，reset，另一种是TogglePin
3.在RCC里开Crystal
--4
1.按键引脚配置为GPIO_input,重命名为KEYX
2.按键记得开上下拉，即pull up
--5
1.配置按钮引脚为EXTL_GPIO，重命名为KEYX（X是引脚名称的数字）
2.在GPIO里设置GPIOmode为Rising/Fallingedge，记得开上下拉，即pull up
3.在NVIC里设置优先级，（数字越小，越优先执行）如果在smt32f1xx.it.c里面写HAL_Delay(),则需把EXTL设置为15，System tick设置为14
--6
1、轮询模式、
打开connectivity的usart2
 USART2 → Asynchronous（异步串口）
 参数：波特率115200，8N1
2、串口中断
与1一样
NVIC 设置：勾选 USART2 global interrupt，优先级默认
3、串口DMA
其他与12一样
DMA Settings Add：USART2_RX
方向：Peripheral To Memory
Mode：Normal
## 重要提醒
仓库没有上传Drivers HAL库文件夹，下载工程后需要自行添加HAL库，否则编译报错。
形成电压差即可让小灯亮起，SET输出高电平3.3v,RESET输出低电平0v
共阳极：此时引脚为3.3v,SET是熄灭，RESET是点亮
共阴极：此时引脚为0v,SET是点亮，RESET是熄灭
（PC13板载LED特性：低电平点亮，高电平熄灭。）
1. 轮询模式：`HAL_UART_Transmit()`会卡住CPU，数据没发完程序停在这里，适合少量短数据。
2. 串口中断：收到1个字节就触发一次中断；接收完要再次调用`HAL_UART_Receive_IT`继续等待下一次数据。
3. DMA串口：DMA负责搬运字节，CPU可以干别的；空闲中断检测回车/一整串消息，支持长字符串。
> ❗DMA坑点：回调处理完成**一定要重新开启接收**，否则只收一次就不再响应。
> ❗`sizeof()`取缓冲区总大小；`strlen()`只算有效字符，DMA接收不要用strlen做接收长度，用callback的入参Size
> ❗串口硬件：TX接CH340‑RX；RX接CH340‑TX，TX‑RX必须交叉；GND必须共地，否则乱码。


## 实验现象
STM32F103 板载 PC13 LED和外接LED 周期闪烁，间隔约1秒亮灭切换。
闪烁：外接3个LED灯交替闪烁
按键控制：按下按钮，蓝灯按下熄灭，松开点亮，红灯按一下点亮，再按一下熄灭
中断：按下按钮，打破原先闪烁的蓝灯，使处在在发亮的蓝灯瞬间熄灭2秒，再继续发亮，持续原本的闪烁
串口：1. **轮询模式**：单片机循环向上位机打印字符串；发送指令不会立刻响应，要等到轮询走到接收代码。
2. **串口中断模式**：串口发送字符，立刻触发中断；发送`1`点亮LED，发送`0`熄灭LED。
3. **串口DMA+空闲中断**：支持发送一长串文本，完整接收一帧再解析；发送指令可以点灯，同时把收到全部字符回显给电脑。

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
5.按键输入GPIO-input，要记得消震Delay(20)
6.发现小灯不停闪烁，要配置上下拉（在GPIO的pull-up/pull-down中选择puii-up）
7.串口中断：中断接收会自动关闭，只会收一次！所以在回调函数末尾{在main里while上方}，必须再调用一次  HAL_UART_Receive_IT() ，重新开启接收，才能继续收下一批数据。
串口DMA：在回调函数末尾{在main里while上方}HAL_UARTEx_ReceiveToIdle_DMA (&huart2 ,receiveDate ,sizeof(receiveDate ));__HAL_DMA_DISABLE_IT (&hdma_usart2_rx ,DMA_IT_HT );
## 工程说明
- 1-pc13-single-LED：简易单点灯工程
- 2-pc13-pa0-dual-LED：CubeMX生成HAL库双LED工程
  
