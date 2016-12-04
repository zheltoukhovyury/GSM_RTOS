#include <stdlib.h>
#include "stm32f4xx_iwdg.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

/* Scheduler include files. */
#include "GSMToSTMPort.h"
#include "FreeRTOS.h"
#include "task.h"



static portTASK_FUNCTION_PROTO( vLEDFlashTask, pvParameters );
static portTASK_FUNCTION_PROTO( testTask, pvParameters );
void StartTick(void);

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
  StartTick();
	for(;;)
	{
          i++;
          IWDG_ReloadCounter();
	}
}



          
static UINT32 sysTick = 0;
void StartTick(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  TIM_TimeBaseStructure.TIM_Period = 1000;
  TIM_TimeBaseStructure.TIM_Prescaler = 24000 - 1;   
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;   
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  
  NVIC_InitTypeDef nvicStructure;
  nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
  nvicStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicStructure);
  
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  GPIO_InitTypeDef gpio_initStruct;
  gpio_initStruct.GPIO_Pin = GPIO_Pin_13;
  gpio_initStruct.GPIO_OType = GPIO_OType_PP;
  gpio_initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  gpio_initStruct.GPIO_Mode = GPIO_Mode_OUT;
  gpio_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &gpio_initStruct);
  GPIO_SetBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_14);
}

void TIM2_IRQHandler( void )
{
  sysTick++;
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

UINT32 TickGet(void)
{
  return 32;
}

          
void StartComport(void)
{



  
  
GPIO_InitTypeDef GPIO_InitStructure;   
USART_InitTypeDef  USART_InitStructure; 
 
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4); 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4); 
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
 
  
 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);      
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;   
  USART_InitStructure.USART_Parity = USART_Parity_No;   
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  USART_Init(UART4, &USART_InitStructure); 
 
  USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
  
  NVIC_InitTypeDef nvicStructure;
  nvicStructure.NVIC_IRQChannel = UART4_IRQn;
  nvicStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicStructure);
  
  USART_Cmd(UART4, ENABLE);  
  
  
  
  USART_SendData(UART4, 0x11);
  while(1)
  {
    while(USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
    USART_SendData(UART4, 0x11);
  }
}

  
  int received = 0;
void UART4_IRQHandler( void )
{
  received++;
  UINT32 data = USART_ReceiveData(UART4);
  data = data;
  
}



