/**
  ******************************************************************************
  * @file    main.c 
  * @author  walleva hu
  * @brief   Main program body through the LL API
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "math.h"
#include "cfg_led.h"
#include "cfg_uartx.h"
#include "cfg_i2c.h"
#include "apply_hmcl5883.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Configure the system clock to 8 MHz */
  SystemClock_Config();
	Configure_LED_GPIO();
	Configure_USARTx(USART1);
	Configure_USARTx(USART2);
	Configure_USARTx(USART4);
	Configure_USARTx(USART5);
	Configure_I2Cx_Master(I2C3);
	init_hmcl5883(I2C3);
	//Configure_I2Cx_Slave(I2C1); // loop communication with I2C3 test pass.

  /* Add your application code here */
	printf("Hello, This is a USART2 printf debug\r\n");
	//Buffer_Transfer_USARTx(USART5);
	
  /* Infinite loop */
  while (1)
  {
		struct hmcl5883_data tmp_hmcl5883 = get_data_from_hmcl5883(I2C3);
		printf("hmcl5883 data, x: %d, y: %d, z: %d \r\n", tmp_hmcl5883.x, tmp_hmcl5883.y, tmp_hmcl5883.z);
		
		TR_Loop_Test_USARTx(USART5);
		LL_GPIO_TogglePin(LED_GPIO_PORT, LED2_PIN);
		LL_mDelay(LED_BLINK_SLOW);	
	}
	return 0;
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = MSI
  *            SYSCLK(Hz)                     = 2097000
  *            HCLK(Hz)                       = 2097000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @retval None
  */
void SystemClock_Config(void)
{
  /* HSE configuration and activation */
  LL_RCC_PLL_Disable();
  /* Set new latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1) 
  {
  };

  /* Sysclk activation  */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE) 
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 2MHz */
	/* @attention: Disable PLL and DIV */
  LL_Init1msTick(HSE_VALUE); 

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(HSE_VALUE);  

  /* Enable Power Control clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency.
		 To update the voltage scaling value regarding system frequency refer to product datasheet.  */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
}
/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Loop time out mechaism.
	* @note It has a bug, When enter the loop, 
					but after tick_loop_time reduce, it smaller than out_loop_time. 
					It can't be clear.
  * @param  None
  * @retval None
  */
char Loop_Is_Timeout_Xms(int out_loop_time)
{
	#define LOOP_TIME_LIMIT 100
	static float tick_loop_time = LOOP_TIME_LIMIT;
	if(tick_loop_time >= LOOP_TIME_LIMIT)
		tick_loop_time = out_loop_time;
	
	if(LL_SYSTICK_IsActiveCounterFlag())
	{
		tick_loop_time--;
		if(tick_loop_time == 0)
		{
			tick_loop_time = LOOP_TIME_LIMIT;
			//printf("loop time out break");
			return 1;
		}
	}
	return 0;
}

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
