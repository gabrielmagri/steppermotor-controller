#ifndef __LED_H
#define __LED_H

#include "stm32f0xx.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define LED_GREEN	 0x01
#define LED_BLUE    0x02

#define LED_ON   0x01
#define LED_OFF  0x00

void    LED_Init(void);
void    LED_Toogle(uint8_t Led);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H */
