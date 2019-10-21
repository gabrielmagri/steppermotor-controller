#include "LED.h"

#include "stm32f0xx_conf.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* Configure PA5 as output pushpull mode */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
}

void LED_Toogle(uint8_t Led)
{
	static uint8_t status = 0;
	status ^= 0x01;

	GPIO_WriteBit(GPIOA,GPIO_Pin_5, status);

}
