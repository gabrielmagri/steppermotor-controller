/*
 * SPI.c
 *
 *  Created on: Oct 20, 2019
 *      Author: Gabriel Henrique Magri
 */

#include "stm32f0xx_spi.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "Nokia5110.h"

enum typeOfWrite{
  COMMAND, // the transmission is an LCD command
  DATA     // the transmission is data
};


void static dataSend(enum typeOfWrite type, uint8_t data)
{
	/* First configure the type of write that will be send to display */
	(COMMAND==type) ? GPIO_ResetBits(GPIOA, NOKIA_Pin_DC) : GPIO_SetBits(GPIOA, NOKIA_Pin_DC);

	/* Enable slave (CE low), send data, wait it and Disable slave */
	GPIO_ResetBits(GPIOA, NOKIA_Pin_CE);
	SPI_SendData8(SPI1, data);
	for (int var = 0; var < 300; ++var);
	GPIO_SetBits(GPIOA, NOKIA_Pin_CE);
}

void Nokia5110_Init(void)
{
	volatile unsigned long delay;

	/* Enable the clock for GPIOB where SPI1 is located on STM32f030 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* Configure pins  CLK and MOSI pins as SPI (alternate function)*/
	GPIO_InitTypeDef GPIOB_Init;
	GPIOB_Init.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_5;
	GPIOB_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOB_Init.GPIO_Mode  = GPIO_Mode_AF;
	GPIOB_Init.GPIO_OType = GPIO_OType_PP;
	GPIOB_Init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIOB_Init);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_0);

	/* Configure Generic purpose pins to act as SPI RST, DC and CE */
	/* These pins are going to be managed by software (this driver) */
	GPIO_InitTypeDef GPIOA_Init;
	GPIOA_Init.GPIO_Pin   = NOKIA_Pin_RST | NOKIA_Pin_DC | NOKIA_Pin_CE;
	GPIOA_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOA_Init.GPIO_Mode  = GPIO_Mode_OUT;
	GPIOA_Init.GPIO_OType = GPIO_OType_PP;
	GPIOA_Init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIOA_Init);
	GPIO_SetBits(GPIOA, NOKIA_Pin_RST);
	GPIO_SetBits(GPIOA, NOKIA_Pin_DC);
	GPIO_SetBits(GPIOA, NOKIA_Pin_CE);

	/* Enable the clock for SPI functionality */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* Configure the SPI structure with all SPI related configuration on working mode */
	SPI_InitTypeDef SPI1_InitConfig;
	SPI1_InitConfig.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI1_InitConfig.SPI_CPHA              = SPI_CPHA_1Edge;
	SPI1_InitConfig.SPI_CPOL              = SPI_CPOL_Low;
	SPI1_InitConfig.SPI_CRCPolynomial     = 0x01;
	SPI1_InitConfig.SPI_DataSize          = SPI_DataSize_8b;
	SPI1_InitConfig.SPI_Direction         = SPI_Direction_1Line_Tx;
	SPI1_InitConfig.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI1_InitConfig.SPI_Mode              = SPI_Mode_Master;
	SPI1_InitConfig.SPI_NSS               = SPI_NSS_Soft;

	SPI_Init(SPI1, &SPI1_InitConfig);
	SPI_Cmd(SPI1, ENABLE); /* Enable the SPI */

	/* Execute a reset on NOKIA5110 for at least 10 ns */
	GPIO_ResetBits(GPIOA, NOKIA_Pin_RST);
	for(delay=0; delay<10; delay=delay+1);
	GPIO_SetBits(GPIOA, NOKIA_Pin_RST);

	/* Send initial necessary configuration commands */
	/* Without these commands in the correct order the display will not work */
	dataSend(COMMAND, 0x21); /* Chip active, horizontal addressing mode (V = 0); use extended instruction set (H = 1) */
	dataSend(COMMAND, CONTRAST); /* Contrast is impacted by: display type, input voltage (3v3 or 5v), etc */
	dataSend(COMMAND, 0x04); /* Set temperature coefficient */
	dataSend(COMMAND, 0x14); /* LCD bias mode 1:48: try 0x13 or 0x14 */
	dataSend(COMMAND, 0x20); /* We must send 0x20 before modifying the display control mode */
	dataSend(COMMAND, 0x0C); /* Set display control to normal mode: 0x0D for inverse */

}

void Nokia5110_OutChar(unsigned char data){
	int i;
	dataSend(DATA, 0x00);                 // blank vertical line padding
	for(i=0; i<5; i=i+1){
		dataSend(DATA, ASCII[data - 0x20][i]);
	}
	dataSend(DATA, 0x00);                 // blank vertical line padding
}

void Nokia5110_OutString(char *ptr){
	while(*ptr){
		Nokia5110_OutChar((unsigned char)*ptr);
		ptr = ptr + 1;
	}
}

void Nokia5110_OutUDec(unsigned short n){
	if(n < 10){
		Nokia5110_OutString("    ");
		Nokia5110_OutChar(n+'0'); /* n is between 0 and 9 */
	} else if(n<100){
		Nokia5110_OutString("   ");
		Nokia5110_OutChar(n/10+'0'); /* tens digit */
		Nokia5110_OutChar(n%10+'0'); /* ones digit */
	} else if(n<1000){
		Nokia5110_OutString("  ");
		Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
		n = n%100;
		Nokia5110_OutChar(n/10+'0'); /* tens digit */
		Nokia5110_OutChar(n%10+'0'); /* ones digit */
	}
	else if(n<10000){
		Nokia5110_OutChar(' ');
		Nokia5110_OutChar(n/1000+'0'); /* thousands digit */
		n = n%1000;
		Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
		n = n%100;
		Nokia5110_OutChar(n/10+'0'); /* tens digit */
		Nokia5110_OutChar(n%10+'0'); /* ones digit */
	}
	else {
		Nokia5110_OutChar(n/10000+'0'); /* ten-thousands digit */
		n = n%10000;
		Nokia5110_OutChar(n/1000+'0'); /* thousands digit */
		n = n%1000;
		Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
		n = n%100;
		Nokia5110_OutChar(n/10+'0'); /* tens digit */
		Nokia5110_OutChar(n%10+'0'); /* ones digit */
	}
}


void Nokia5110_OutDouble(double n){
	int tempInt = 0;
	if(n < 10){
		Nokia5110_OutString(" ");

		tempInt = (int)n;
		n = n - tempInt;
		Nokia5110_OutChar(tempInt+'0'); /* n is between 0 and 9 */
		Nokia5110_OutChar('.');
		n=n*10.0;
		tempInt = (int)n;
		n = n - tempInt;
		Nokia5110_OutChar(tempInt+'0'); /* n is between 0 and 9 */
		n=n*10.0;
		tempInt = (int)n;
		n = n - tempInt;
		Nokia5110_OutChar(tempInt+'0'); /* n is between 0 and 9 */
		n=n*10.0;
		tempInt = (int)n;
		n = n - tempInt;
		Nokia5110_OutChar(tempInt+'0'); /* n is between 0 and 9 */

	} else if(n < 100){

		tempInt = (int)n;
		n = n - tempInt;
		Nokia5110_OutChar(tempInt/10+'0'); /* tens digit */
		Nokia5110_OutChar(tempInt%10+'0'); /* ones digit */
		Nokia5110_OutChar('.');
		n=n*10.0;
		tempInt = (int)n;
		n = n - tempInt;
		Nokia5110_OutChar(tempInt+'0'); /* n is between 0 and 9 */
		n=n*10.0;
		tempInt = (int)n;
		n = n - tempInt;
		Nokia5110_OutChar(tempInt+'0'); /* n is between 0 and 9 */
		n=n*10.0;
		tempInt = (int)n;
		n = n - tempInt;
		Nokia5110_OutChar(tempInt+'0'); /* n is between 0 and 9 */

	}
}

void Nokia5110_Clear(void){
	int i;
	Nokia5110_SetCursor(0, 0);
	for(i=0; i<(MAX_X*MAX_Y/8); i=i+1){
		dataSend(DATA, 0x00);
	}
	Nokia5110_SetCursor(0, 0);
}

void Nokia5110_SetCursor(unsigned char newX, unsigned char newY){
	if((newX > 11) || (newY > 5)){        // bad input
		return;                           // do nothing
	}
	GPIO_ResetBits(GPIOA, NOKIA_Pin_DC); /* Set DC to command mode */

	// multiply newX by 7 because each character is 7 columns wide
	dataSend(COMMAND, 0x80|(newX*7));     // setting bit 7 updates X-position
	dataSend(COMMAND, 0x40|newY);         // setting bit 6 updates Y-position

	GPIO_SetBits(GPIOA, NOKIA_Pin_DC); /* Set DC back to data mode */
}

void Nokia5110_DrawFullImage(const char *ptr){
	int i;
	Nokia5110_SetCursor(0, 0);
	for(i=0; i<(MAX_X*MAX_Y/8); i=i+1){
		dataSend(DATA, ptr[i]);
	}
}

char Screen[SCREENW*SCREENH/8]; // buffer stores the next image to be printed on the screen

void Nokia5110_PrintBMP(unsigned char xpos, unsigned char ypos, const unsigned char *ptr, unsigned char threshold){
	long width = ptr[18], height = ptr[22], i, j;
	unsigned short screenx, screeny;
	unsigned char mask;
	// check for clipping
	if((height <= 0) ||              // bitmap is unexpectedly encoded in top-to-bottom pixel order
		((width%2) != 0) ||           // must be even number of columns
		((xpos + width) > SCREENW) || // right side cut off
		(ypos < (height - 1)) ||      // top cut off
		(ypos > SCREENH))
	{ // bottom cut off
		return;
	}
	if(threshold > 14)
	{
		threshold = 14;             // only full 'on' turns pixel on
	}
	// bitmaps are encoded backwards, so start at the bottom left corner of the image
	screeny = ypos/8;
	screenx = xpos + SCREENW*screeny;
	mask = ypos%8;                // row 0 to 7
	mask = 0x01<<mask;            // now stores a mask 0x01 to 0x80
	j = ptr[10];                  // byte 10 contains the offset where image data can be found
	for(i=1; i<=(width*height/2); i=i+1)
	{
		// the left pixel is in the upper 4 bits
		if(((ptr[j]>>4)&0xF) > threshold)
		{
			Screen[screenx] |= mask;
		} else
		{
			Screen[screenx] &= ~mask;
		}
		screenx = screenx + 1;
		// the right pixel is in the lower 4 bits
		if((ptr[j]&0xF) > threshold)
		{
			Screen[screenx] |= mask;
		} else
		{
			Screen[screenx] &= ~mask;
		}
		screenx = screenx + 1;
		j = j + 1;
		if((i%(width/2)) == 0)// at the end of a row
		{
			if(mask > 0x01)
			{
				mask = mask>>1;
			} else
			{
				mask = 0x80;
				screeny = screeny - 1;
			}
			screenx = xpos + SCREENW*screeny;
			// bitmaps are 32-bit word aligned
			switch((width/2)%4)// skip any padding
			{
				case 0: j = j + 0; break;
				case 1: j = j + 3; break;
				case 2: j = j + 2; break;
				case 3: j = j + 1; break;
			}
		}
	}
}

void Nokia5110_DisplayBuffer(void){
	Nokia5110_DrawFullImage(Screen);
}
