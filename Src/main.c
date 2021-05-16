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
#include "cfg_sys_clk.h"
#include "cfg_led.h"
#include "cfg_uartx.h"
#include "cfg_i2c.h"
#include "cfg_adc.h"
#include "soft_i2c.h"
#include "apply_hmcl5883.h"
#include "cfg_low_power.h"

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
char GLOBAL_MCU_UID[25]= {0}; /* UID : 96bit--> 24 bytes + 0*/


/* Private define ------------------------------------------------------------*/
#define Get_Mcu_UID() do{snprintf(GLOBAL_MCU_UID, sizeof(GLOBAL_MCU_UID), "%08X%08X%08X", \
												 LL_GetUID_Word0(), LL_GetUID_Word1(), LL_GetUID_Word2());} while(0)

/* Private macro -------------------------------------------------------------*/
#define IIC_SCL_GPIOx 			GPIOC
#define SCL_PIN 						LL_GPIO_PIN_6
#define IIC_SDA_GPIOx 			GPIOC
#define SDA_PIN 						LL_GPIO_PIN_9

												 
/**
  * @brief  Display the current time and date.
  * @param  None
  * @retval None
  */
/* Buffers used for displaying Time and Date */
uint8_t aShowTime[50] = {0};
uint8_t aShowDate[50] = {0};
void Show_RTC_Calendar(void)
{
  /* Note: need to convert in decimal value in using __LL_RTC_CONVERT_BCD2BIN helper macro */
  /* Display time Format : hh:mm:ss */
  sprintf((char*)aShowTime,"%.2d:%.2d:%.2d", __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC)), 
          __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC)), 
          __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC)));
  /* Display date Format : mm-dd-yy */
  sprintf((char*)aShowDate,"%.2d-%.2d-%.2d", __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC)), 
          __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC)), 
          2000 + __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC)));
}
												 
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
												 
int main(void)
{
	/* Get MCU UID To GLOBAL_MCU_UID Array */
	Get_Mcu_UID();
	/* St-link is invalid when use following function. */
	//ReduceIOPowerConsumption();
  /* Configure the system clock to external crytal 8 MHz */
  SystemClock_Config();
	/* Configure RTC for calender and wakeup by LSE */
	Configure_RTC();
	//Configure_RTC_Calendar();
	Show_RTC_Calendar();

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
	Configure_SOFT_IIC_GPIO(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
	
  /* Add your application code here */
	printf("Manufacture Date: %s \r\n", aShowDate);
	printf("Manufacture Time: %s \r\n", aShowTime);
	printf("MCU Device ID : 0x%08X\r", LL_DBGMCU_GetDeviceID());
	printf("MCU Revision ID : 0x%08X\r", LL_DBGMCU_GetRevisionID());
	printf("MCU Device UID : 0x%s \r", GLOBAL_MCU_UID);
	Buffer_Transfer_USARTx(USART5);

	EnterSTOPMode();
	/* Infinite loop */
  while (1)
  {	
		/* soft i2c write hmcl5883 test.	 */
		Common_WriteByte(0x1e, 0x00, 0x10, IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
		Common_WriteByte(0x1e, 0x02, 0x00, IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
		uint8_t tmp = 0;
		Common_ReadByte(0x1e, 0x04, &tmp, IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
		printf("hmcl id: %02x \r",  tmp);
		
		/* adc sample comprise vref:  chn4:  temp: */
		struct adc1_data cal_ad_data = ConversionStartPoll_ADC1_GrpRegular();
		printf("vref: %d, chn4: %d, temp: %d \r", cal_ad_data.vref, cal_ad_data.chn4, cal_ad_data.temp);
		
		TR_Loop_Test_USARTx(USART5);
		LL_mDelay(500);
	}
	return 0;
}

/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */
/* Private functions ---------------------------------------------------------*/
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
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
