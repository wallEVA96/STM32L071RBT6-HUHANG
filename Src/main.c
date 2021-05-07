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
#include "cfg_adc.h"
#include "soft_i2c.h"
#include "apply_hmcl5883.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
unsigned int GLOBAL_LOOP_TIME_OUT_VAL = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
extern __IO uint16_t aADCxConvertedData[3]; /* ADC group regular conversion data */

int main(void)
{
  /* Configure the system clock to external crytal 8 MHz */
  SystemClock_Config();
	/* Configure LED on PD2 */
	Configure_LED_GPIO();
	/* Configure USART */
	Configure_USARTx(USART1);
	Configure_USARTx(USART2);
	Configure_USARTx(USART4);
	Configure_USARTx(USART5);
	/* Configure Hardware I2C Master */
	Configure_I2Cx_Master(I2C3);
	/* Configure Hardware I2C Slave, 
		 Slaver I2C1 do loop communication with master I2C3,
		 Test pass. 
	*/
	//Configure_I2Cx_Slave(I2C1);
	init_hmcl5883(I2C3);
	/* Configure ADC1 about Vref, Channel 4 on PA4.
		 Test on tempetature in 68'C, voltage in 3.3v.
		 Adc data include vref, temp, channel 4 are normal. */
	Configure_ADC1_TO_DMA();
	Configure_ADC1();
	Activate_ADC1();
	/* soft i2c configure.	 */
	#define IIC_SCL_GPIOx 			GPIOC
	#define SCL_PIN 						LL_GPIO_PIN_6
	#define IIC_SDA_GPIOx 			GPIOC
	#define SDA_PIN 						LL_GPIO_PIN_9
	Configure_SOFT_IIC_GPIO(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
	
  /* Add your application code here */
	printf("Hello, This is a USART2 printf debug\r\n");
	Buffer_Transfer_USARTx(USART5);
	
  /* Infinite loop */
  while (1)
  {
		/* periph i2c write hmcl5883 test.	 */
//	struct hmcl5883_data tmp_hmcl5883 = get_data_from_hmcl5883(I2C3);
//	printf("hmcl5883 data, x: %d, y: %d, z: %d \r\n", tmp_hmcl5883.x, tmp_hmcl5883.y, tmp_hmcl5883.z);	
		
		/* soft i2c write hmcl5883 test.	 */
		Common_WriteByte(0x1e, 0x00, 0x10, IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
		Common_WriteByte(0x1e, 0x02, 0x00, IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
		uint8_t tmp = 0;
		Common_ReadByte(0x1e, 0x04, &tmp, IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
		printf("hmcl id: %02x \r",  tmp);
		
		/* adc sample include vref:  chn4:  temp: */
		struct adc1_data cal_ad_data = ConversionStartPoll_ADC1_GrpRegular();
		printf("vref: %d, chn4: %d, temp: %d \r", cal_ad_data.vref, cal_ad_data.chn4, cal_ad_data.temp);
		
		LL_mDelay(LED_BLINK_SLOW);		
		TR_Loop_Test_USARTx(USART5);
		LL_GPIO_TogglePin(LED_GPIO_PORT, LED2_PIN);	
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
  * @brief  
  * @param  
  * @param  
  * @retval None
  */
char LOOP_IS_TIME_OUT_xMS(void)
{
  /* Infinite loop */
	if(LL_SYSTICK_IsActiveCounterFlag())
		GLOBAL_LOOP_TIME_OUT_VAL--;
	return (GLOBAL_LOOP_TIME_OUT_VAL==0)?1:0;
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
