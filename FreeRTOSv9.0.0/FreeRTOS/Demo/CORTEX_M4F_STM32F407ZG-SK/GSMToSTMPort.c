#include <stdlib.h>
#include "stm32f4xx_iwdg.h"
#include "stm32f4xx_gpio.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"



static portTASK_FUNCTION_PROTO( vLEDFlashTask, pvParameters );
static portTASK_FUNCTION_PROTO( testTask, pvParameters );

/*-----------------------------------------------------------*/

void vStartLEDFlashTasks ( UBaseType_t uxPriority )
{
  xTaskCreate( vLEDFlashTask, "LEDx", 150, NULL, uxPriority, ( TaskHandle_t * ) NULL );
  xTaskCreate( testTask, "test", 150, NULL, uxPriority, ( TaskHandle_t * ) NULL );
  
}
/*-----------------------------------------------------------*/

static portTASK_FUNCTION( vLEDFlashTask, pvParameters )
{
  int i;
	for(;;)
	{
          i++;
          IWDG_ReloadCounter();
	}
}



static portTASK_FUNCTION( testTask, pvParameters )
{
  int i;
	for(;;)
	{
          i++;
          IWDG_ReloadCounter();
	}
}

