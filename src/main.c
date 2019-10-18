/* Common includes */
#include "stm32f0xx.h"
#include "string.h"
#include "stdlib.h"
#include "float.h"
#include <stdbool.h>

/* FreeRTOS includes */
#include "FreeRTOS/inc/FreeRTOS.h"
#include "FreeRTOS/inc/task.h"
#include "FreeRTOS/inc/queue.h"
#include "FreeRTOS/inc/timers.h"
#include "FreeRTOS/FreeRTOSConfig.h"

/* drv includes */
#include "drv/LED/LED.h"
#include "drv/KEY/Keyboard.h"

xTaskHandle xHandle_LEDTask 	  = NULL;
xTaskHandle xHandle_KeyboardTask  = NULL;

static void vLEDTask(void *pvParameters);
static void vKeyboardTask(void *pvParameters);

int main(void)
{
	/* Drivers Initializations */
	LED_Init();
	Keyboard_Init();

    xTaskCreate(vLEDTask,
    			"LEDTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				tskIDLE_PRIORITY,
				xHandle_LEDTask);

    xTaskCreate(vKeyboardTask,
				"KeyboardTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				tskIDLE_PRIORITY,
				xHandle_KeyboardTask);

    vTaskStartScheduler();

    return 0;
}

static void vLEDTask (void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 10; // 10ms
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		//LED_Toogle(0);
	}

	vTaskDelete(NULL);
}



static void vKeyboardTask (void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 10; // 10ms
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		unsigned long in = Keyboard_Continuous_In();
		if(in!=0) LED_Toogle(0);
	}

	vTaskDelete(NULL);
}

/**
  * @brief  Idle Hook (Idle Task)
  *
  * vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
  *	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
  *	task.  It is essential that code added to this hook function never attempts
  *	to block in any way (for example, call xQueueReceive() with a block time
  *	specified, or call vTaskDelay()).  If the application makes use of the
  *	vTaskDelete() API function (as this demo application does) then it is also
  *	important that vApplicationIdleHook() is permitted to return to its calling
  *	function, because it is the responsibility of the idle task to clean up
  *	memory allocated by the kernel to any task that has since been deleted.
  *
  * @param  None
  *
  * @retval None
  */
void vApplicationIdleHook(void)
{

}

/**
  * @brief  Stack overflow
  *
  * Run time stack overflow checking is performed if
  *	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook
  *	function is called if a stack overflow is detected.
  *
  * @param  None
  *
  * @retval None
  */
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
	(void) pcTaskName;
	(void) pxTask;

	for(;;)
	{

	}
}

/**
  * @brief  Tick Hook
  *
  * This function will be called by each tick interrupt if
  *	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
  *	added here, but the tick hook is called from an interrupt context, so
  *	code must not attempt to block, and only the interrupt safe FreeRTOS API
  *	functions can be used (those that end in FromISR()).
  *
  * @param  None
  *
  * @retval None
  */
void vApplicationTickHook(void)
{

}

