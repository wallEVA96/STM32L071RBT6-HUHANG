/**
  ******************************************************************************
  * @file    led configure
  * @author  wallea hu
  * @brief   
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "cfg_low_power.h"
 #include "cfg_sys_clk.h"

/**
  * @brief  Function to exit stop mode.
  * @param  None
  * @retval None
  */
void ReadyForNormalMode(void){
		/* Choose External OSC */
		SystemClock_Config();
		/* Wait for USART is ready */
		//while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1)))){}
}

/**
  * @brief  Function to reduce IO pin power consumption.
  * @param  None
  * @retval None
  */
void ReduceIOPowerConsumption(void){
	
 LL_GPIO_InitTypeDef gpio_initstruct = {LL_GPIO_PIN_ALL, LL_GPIO_MODE_ANALOG, 
                                         LL_GPIO_SPEED_FREQ_HIGH, LL_GPIO_OUTPUT_PUSHPULL, 
                                         LL_GPIO_PULL_NO, LL_GPIO_AF_0};

  /* Set all GPIO in analog state to reduce power consumption,                */
  /* Note: Debug using ST-Link is not possible during the execution of this   */
  /*       example because communication between ST-link and the device       */
  /*       under test is done through UART. All GPIO pins are disabled (set   */
  /*       to analog input mode) including  UART I/O pins.                    */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA |
                           LL_IOP_GRP1_PERIPH_GPIOB |
                           LL_IOP_GRP1_PERIPH_GPIOC);

  LL_GPIO_Init(GPIOA, &gpio_initstruct);
  LL_GPIO_Init(GPIOB, &gpio_initstruct);
  LL_GPIO_Init(GPIOC, &gpio_initstruct);


  LL_IOP_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOA |
                           LL_IOP_GRP1_PERIPH_GPIOB |
                           LL_IOP_GRP1_PERIPH_GPIOC );
}

/**
  * @brief  Function to configure and enter in STOP Mode.
  * @param  None
  * @retval None
  */
void EnterSTOPMode(void)
{ 
	//ReduceIOPowerConsumption();
  /** Request to enter STOP mode
    * Following procedure describe in STM32L0xx Reference Manual
    * See PWR part, section Low-power modes, STOP mode
    */
  /* Enable ultra low power mode */
  LL_PWR_EnableUltraLowPower();

  /** Set the regulator to low power before setting MODE_STOP.   
    * If the regulator remains in "main mode",   
    * it consumes more power without providing any additional feature. */    
  LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);

  /* Set STOP mode when CPU enters deepsleep */
  LL_PWR_SetPowerMode(LL_PWR_MODE_STOP);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  LL_LPM_EnableDeepSleep();  

  /* Request Wait For Interrupt */
  __WFI();
}

