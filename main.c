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
__IO uint32_t TimingDelay;
__IO uint32_t counting = 0;
//__IO uint8_t scan_step = 0;
uint8_t scan_flag = 0;

__IO uint8_t pressed = 0;
__IO uint8_t no_exti = 0;
uint8_t tmp = 0;
__IO uint8_t exti_flag = 0;//record which pin has interrupt


uint8_t keys[12] = {0};
uint8_t i;

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
static void scan8(void);
static void scan6(void);
static void scan5(void);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

	/* USART configuration */
  USART_Config();
	while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}
    
	if (SysTick_Config(SystemCoreClock / 1000))
  {
    /* Capture error */ 
    while (1);
  }
	    /* Enable GPIO */ 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIOA->BSRR = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_12;

  EXTI4_15_Config();
  TIM_Config();
  TIM_Cmd(TIM3, DISABLE);

	for(i = 0; i < 12; i ++){
    keys[i] = 0;
  }

/*------------------------------------------------------------------------------------------*/  
	while (1)
  {
    switch(exti_flag)
		{
      case 8:
        no_exti = 1;
        scan8();
        no_exti = 0;

        if(keys[1] + keys[4] +keys[7] + keys[10] > 1){
          if(scan_flag == 10){
            printf("star");
        }
          else{
            printf("%d",scan_flag);
          }
        }  

        while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) != RESET){}
        exti_flag = 0;
        for(i = 0; i < 12; i ++){
          keys[i] = 0;
        }

      break;

      case 6:
        no_exti = 1;
        scan6();
        no_exti = 0;

        if(keys[2] + keys[5] +keys[8] + keys[0] > 1){
          printf("%d",scan_flag);
        }  

        while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) != RESET){}
        exti_flag = 0;
        for(i = 0; i < 12; i ++){
          keys[i] = 0;
        }
      break;

      case 5:
        no_exti = 1;
        scan5();
        no_exti = 0;

        if(keys[3] + keys[6] +keys[9] + keys[11] > 1){
          if(scan_flag == 11){
            printf("number");
        }
          else{
            printf("%d",scan_flag);
          }
        }  

        while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != RESET){}
        exti_flag = 0;
        for(i = 0; i < 12; i ++){
          keys[i] = 0;
        }
      default:
      break;
	}
}

}

static void scan8(void)
{ 
  TIM_Cmd(TIM3, ENABLE);

  tmp = pressed;
  GPIOA->BRR = GPIO_Pin_6 | GPIO_Pin_7;
  GPIOC->BRR = GPIO_Pin_12;
  GPIOC->BSRR = GPIO_Pin_10;
  while(pressed < tmp + 5){
    GPIOA->BSRR = GPIO_Pin_7;
    GPIOC->BSRR = GPIO_Pin_10;

    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) != RESET){
      keys[1] ++;
    }
    GPIOC->BRR = GPIO_Pin_10;
    GPIOC->BSRR = GPIO_Pin_12;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) != RESET){
      keys[4] ++;
    }
    GPIOA->BSRR = GPIO_Pin_6;
    GPIOC->BRR = GPIO_Pin_12;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) != RESET){
      keys[7] ++;
    }
    GPIOA->BRR = GPIO_Pin_6;
    GPIOA->BSRR = GPIO_Pin_7;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) != RESET){
      keys[10] ++;
    }
  }

  scan_flag = keys[1] > keys[4] ? (keys[1] > keys[7]?(keys[1] > keys[10] ? 1:10):(keys[7] > keys[10] ? 7:10)):(keys[4] > keys[7]?(keys[4] > keys[10] ? 4:10):(keys[7] > keys[10] ? 7:10));
  TIM_Cmd(TIM3, DISABLE);

  GPIOA->BSRR = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_12;
}

static void scan6(void)
{   
  TIM_Cmd(TIM3, ENABLE);

  tmp = pressed;
  GPIOA->BRR = GPIO_Pin_6 | GPIO_Pin_7;
  GPIOC->BRR = GPIO_Pin_12;
  GPIOC->BSRR = GPIO_Pin_10;
  while(pressed < tmp + 5){
    GPIOA->BSRR = GPIO_Pin_7;
    GPIOC->BSRR = GPIO_Pin_10;

    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) != RESET){
      keys[2] ++;
    }
    GPIOC->BRR = GPIO_Pin_10;
    GPIOC->BSRR = GPIO_Pin_12;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) != RESET){
      keys[5] ++;
    }
    GPIOA->BSRR = GPIO_Pin_6;
    GPIOC->BRR = GPIO_Pin_12;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) != RESET){
      keys[8] ++;
    }
    GPIOA->BRR = GPIO_Pin_6;
    GPIOA->BSRR = GPIO_Pin_7;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) != RESET){
      keys[0] ++;
    }
  }

  scan_flag = keys[2] > keys[5] ? (keys[2] > keys[8]?(keys[2] > keys[0] ? 2:0):(keys[8] > keys[0] ? 8:0)):(keys[5] > keys[8]?(keys[5] > keys[0] ? 5:0):(keys[8] > keys[0] ? 8:0));
  
  GPIOA->BSRR = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_12;

}

static void scan5(void)
{ 
  TIM_Cmd(TIM3, ENABLE);

  tmp = pressed;
  GPIOA->BRR = GPIO_Pin_6 | GPIO_Pin_7;
  GPIOC->BRR = GPIO_Pin_12;
  GPIOC->BSRR = GPIO_Pin_10;
  while(pressed < tmp + 5){
    GPIOA->BSRR = GPIO_Pin_7;
    GPIOC->BSRR = GPIO_Pin_10;

    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != RESET){
      keys[3] ++;
    }
    GPIOC->BRR = GPIO_Pin_10;
    GPIOC->BSRR = GPIO_Pin_12;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != RESET){
      keys[6] ++;
    }
    GPIOA->BSRR = GPIO_Pin_6;
    GPIOC->BRR = GPIO_Pin_12;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != RESET){
      keys[9] ++;
    }
    GPIOA->BRR = GPIO_Pin_6;
    GPIOA->BSRR = GPIO_Pin_7;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) != RESET){
      keys[11] ++;
    }
  }

  scan_flag = keys[3] > keys[6] ? (keys[3] > keys[9]?(keys[3] > keys[11] ? 3:11):(keys[9] > keys[11] ? 9:11)):(keys[6] > keys[9]?(keys[6] > keys[11] ? 6:11):(keys[9] > keys[11] ? 9:11));

  GPIOA->BSRR = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_12;

}


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
