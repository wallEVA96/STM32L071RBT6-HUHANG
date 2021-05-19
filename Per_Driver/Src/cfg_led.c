/**
  ******************************************************************************
  * @file    led configure
  * @author  wallea hu
  * @brief   
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "cfg_led.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function  -----------------------------------------------*/
/**
  * @brief  This function configures GPIO
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
	
void Configure_LED_GPIO(void)
{
  /* Enable the LED5 Clock */
	LED_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED2 */
  LL_GPIO_SetPinMode(LED_GPIO_PORT, LED2_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  LL_GPIO_SetPinOutputType(LED_GPIO_PORT, LED2_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  LL_GPIO_SetPinSpeed(LED_GPIO_PORT, LED2_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  LL_GPIO_SetPinPull(LED_GPIO_PORT, LED2_PIN, LL_GPIO_PULL_UP);
	LL_GPIO_SetOutputPin(LED_GPIO_PORT, LED2_PIN);
}

/************************ (C) COPYRIGHT wallea Hu *****END OF FILE****/
