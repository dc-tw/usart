#include "hw_cfg.h"
#include "stdio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay;

EXTI_InitTypeDef   EXTI_InitStructure;
GPIO_InitTypeDef   GPIO_InitStructure;
NVIC_InitTypeDef   NVIC_InitStructure;

__IO uint16_t CCR1_Val = 40961;
__IO uint16_t CCR2_Val = 27309;
__IO uint16_t CCR3_Val = 13654;
__IO uint16_t CCR4_Val = 6826;
uint16_t PrescalerValue = 0;

/* Private function prototypes -----------------------------------------------*/
void USART_Config(void);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	
void Delay(__IO uint32_t nTime);
void EXTI4_15_Config(void);
void NVIC_Config(void);
  
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
	
	    /* Enable GPIO */ 
/*
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);*/

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */

void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/*
if (SysTick_Config(SystemCoreClock / 1000))
  {
    while (1);//capture error
  }*/

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None  
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{	
	while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

void EXTI4_15_Config(void)
{
  
	 /* Enable GPIOC clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure PC8 and PC13 pins as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
   /* Connect EXTI8 Line to PC8 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource8);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6);
  
  /* Configure EXTI8 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line8|EXTI_Line5|EXTI_Line6;  
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//DISABLE
  EXTI_Init(&EXTI_InitStructure);
  
  /* Enable and set EXTI4_15 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/* USARTx configured as follow:
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - Stop Bit = 1 Stop Bit
  - Parity = No Parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
/*
void USART_Config(void)
{ 
  USART_InitTypeDef USART_InitStructure;
  
  
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHBPeriphClockCmd(EVAL_COM1_RX_GPIO_CLK, ENABLE);

  RCC_APB1PeriphClockCmd(EVAL_COM1_CLK, ENABLE); 

  GPIO_PinAFConfig(EVAL_COM1_TX_GPIO_PORT, EVAL_COM1_TX_SOURCE, EVAL_COM1_TX_AF);

  GPIO_PinAFConfig(EVAL_COM1_RX_GPIO_PORT, EVAL_COM1_RX_SOURCE, EVAL_COM1_RX_AF);

  GPIO_InitStructure.GPIO_Pin = EVAL_COM1_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(EVAL_COM1_TX_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = EVAL_COM1_RX_PIN;
  GPIO_Init(EVAL_COM1_TX_GPIO_PORT, &GPIO_InitStructure);

  USART_Init(EVAL_COM1, &USART_InitStructure);

  USART_Cmd(EVAL_COM1, ENABLE);
	
}*/

void TIM_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  
    /* -----------------------------------------------------------------------
    TIM3 Configuration: Output Compare Timing Mode:
    
    In this example TIM3 input clock (TIM3CLK) is set to APB1 clock (PCLK1),  
      => TIM3CLK = PCLK1 = SystemCoreClock = 48 MHz
          
    To get TIM3 counter clock at 6 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = (PCLK1 /6 MHz) - 1
                                              
    CC1 update rate = TIM3 counter clock / CCR1_Val = 146.48 Hz
    ==> Toggling frequency = 73.24 Hz
    
    C2 update rate = TIM3 counter clock / CCR2_Val = 219.7 Hz
    ==> Toggling frequency = 109.8 Hz
    
    CC3 update rate = TIM3 counter clock / CCR3_Val = 439.4 Hz
    ==> Toggling frequency = 219.7 Hz
    
    CC4 update rate = TIM3 counter clock / CCR4_Val = 878.9 Hz
    ==> Toggling frequency = 439.4 Hz

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  ----------------------------------------------------------------------- */   


  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock  / 6000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);

  /* Output Compare Timing Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Output Compare Timing Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;

  TIM_OC4Init(TIM3, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable);
   
  /* TIM Interrupts enable */
  TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET)
  {}

  return ch;
}

void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief Configure the USART Device
  * @param  None
  * @retval None
  */
void USART_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
    
/* USARTx configured as follow:
  - BaudRate = 9600 baud  
  - Word Length = 8 Bits
  - Two Stop Bit
  - Odd parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;//origin 9600
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  //STM_EVAL_COMInit(COM1, &USART_InitStructure);
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(EVAL_COM1_RX_GPIO_CLK, ENABLE);

  /* Enable USART clock */
  RCC_APB1PeriphClockCmd(EVAL_COM1_CLK, ENABLE); 

  /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(EVAL_COM1_TX_GPIO_PORT, EVAL_COM1_TX_SOURCE, EVAL_COM1_TX_AF);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(EVAL_COM1_RX_GPIO_PORT, EVAL_COM1_RX_SOURCE, EVAL_COM1_RX_AF);
  
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = EVAL_COM1_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(EVAL_COM1_TX_GPIO_PORT, &GPIO_InitStructure);
    
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = EVAL_COM1_RX_PIN;
  GPIO_Init(EVAL_COM1_TX_GPIO_PORT, &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(EVAL_COM1, &USART_InitStructure);
    
  /* Enable USART */
  USART_Cmd(EVAL_COM1, ENABLE);
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
