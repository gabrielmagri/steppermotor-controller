/* Prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_H
#define __KEY_H

#include "stm32f0xx.h"

#ifdef __cplusplus
 extern "C" {
#endif

void    KEY_Init(void);
uint8_t KEY_Read();

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */
