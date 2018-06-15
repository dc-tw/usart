/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hw_cfg.h"
#include "stdio.h"

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_TimeBase
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t NbrOfDataToTransfer;
extern uint8_t NbrOfDataToRead;
extern __IO uint8_t TxCount; 
extern __IO uint16_t RxCount; 
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f0xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f0xx.c file
  */ 

  /* NVIC configuration */
  NVIC_Config();

  /* USART configuration */
  USART_Config();

  /* Enable the EVAL_COM1 Transmoit interrupt: this interrupt is generated when the 
  EVAL_COM1 transmit data register is empty */  
//  USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);

  /* Wait until EVAL_COM1 send the TxBuffer */
/*  while(TxCount < NbrOfDataToTransfer)
  {}*/

  /* The software must wait until TC=1. The TC flag remains cleared during all data
  transfers and it is set by hardware at the last frame’s end of transmission*/
/*  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}*/

  /* Enable the EVAL_COM1 Receive interrupt: this interrupt is generated when the 
  EVAL_COM1 receive data register is not empty */
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);

  /* Wait until EVAL_COM1 receive the RxBuffer */
//  while(RxCount < NbrOfDataToRead)
//  {}

  /* Infinite loop */
  printf("start\n\r");
  while (1)
  {
  }
}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
