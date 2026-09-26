#include "OLED.h"
#define OLED_ADDRESS 0X78
#include"string.h" 
#include"main.h" 

void OLED_SendCmd(uint8_t cmd)
{
uint8_t sendBuffer[2];
	sendBuffer[0]=0x00;
	sendBuffer[1]=cmd;
	HAL_I2C_Master_Transmit(&hi2c1,OLED_ADDRESS,sendBuffer ,2,HAL_MAX_DELAY);
}
void OLED_Init()
{
	HAL_Delay (30);
	OLED_SendCmd (0xAE);
	HAL_Delay (10);
	OLED_SendCmd(0xAE); /*关闭显示 display off*/

	OLED_SendCmd(0x02); /*设置列起始地址 set lower column address*/
	OLED_SendCmd(0x10); /*设置列结束地址 set higher column address*/

	OLED_SendCmd(0x40); /*设置起始行 set display start line*/

	OLED_SendCmd(0xB0); /*设置页地址 set page address*/

	OLED_SendCmd(0x81); /*设置对比度 contract control*/
	OLED_SendCmd(0xCF); /*128*/

	OLED_SendCmd(0xA1); /*设置分段重映射 从右到左 set segment remap*/

	OLED_SendCmd(0xA6); /*正向显示 normal / reverse*/

	OLED_SendCmd(0xA8); /*多路复用率 multiplex ratio*/
	OLED_SendCmd(0x3F); /*duty = 1/64*/

	OLED_SendCmd(0xAD); /*设置启动电荷泵 set charge pump enable*/
	OLED_SendCmd(0x8B); /*启动DC‑DC */

	OLED_SendCmd(0x33); /*设置泵系电压 set VPP 10V */

	OLED_SendCmd(0xC8); /*设置输出扫描方向 COM[N‑1]到COM[0] Com scan direct*/

	OLED_SendCmd(0xD3); /*设置显示偏移 set display offset*/
	OLED_SendCmd(0x00); /* 0x00 */

	OLED_SendCmd(0xD5); /*设置内部时钟频率 set osc frequency*/
	OLED_SendCmd(0xC0);

	OLED_SendCmd(0xD9); /*设置放电/预充电时间 set pre‑charge period*/
	OLED_SendCmd(0xF1); /*0x22*/

	OLED_SendCmd(0xDA); /*设置引脚布局 set COM pins*/
	OLED_SendCmd(0x12);

	OLED_SendCmd(0xDB); /*设置电平 set vcomh*/
	OLED_SendCmd(0x40);

	OLED_SendCmd(0xAF); /*开启显示 display ON*/
}


uint8_t GRAM[8][128];
void OLED_NewFrame()
{
//for(uint8_t i=0;i<8;i++)
//{
//for(uint8_t j=0;j<128;j++)
	//{
//	GRAM[i][j]=0;
	memset(GRAM ,0,sizeof (GRAM ));
	
	
}
	

void OLED_ShowFrame()
{
uint8_t sendBuffer[129];
	sendBuffer [0]=0x40;
	for(uint8_t i=0;i<8;i++)
	{for(uint8_t j=0;j<128;j++)
		{
		sendBuffer [j+1]=GRAM [i][j];
		}
		OLED_SendCmd(0xB0+i);
	  OLED_SendCmd(0x00);
	  OLED_SendCmd(0x10);
		HAL_I2C_Master_Transmit(&hi2c1,OLED_ADDRESS,sendBuffer,sizeof(sendBuffer),HAL_MAX_DELAY);
	}
}	
void OLED_SetPixel(uint8_t x,uint8_t y)
{
if(x>=128||y>=64)return ;
	GRAM[y/8][x]|=0x01<<(y%8);
}

void OLED_Test()
{
  OLED_SendCmd(0xB0);
	OLED_SendCmd(0x00);
	OLED_SendCmd(0x10);
	
	uint8_t sendBuffer[]={0x40,0xAA};
	HAL_I2C_Master_Transmit(&hi2c1,OLED_ADDRESS,sendBuffer,sizeof(sendBuffer),HAL_MAX_DELAY);
}