/**
  ******************************************************************************
  * @file    init hmcl5883 and read value.
  * @author  wallea hu
  * @brief   USART LL module driver.
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "cfg_i2c.h"
#include "apply_hmcl5883.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function  -----------------------------------------------*/
/**
  * @brief  Init HMCL5883 By Hardware I2C
  * @param  None
  * @retval None
  */
void init_hmcl5883(I2C_TypeDef *I2Cx)
{	
	/* Init HMC5883L By Hardware I2C */
	#define	HMC5883L_Addr (0x1E<<1)	// 7-bit address.
	I2C_Write(I2Cx, HMC5883L_Addr, 0x00, 0x10);
	I2C_Write(I2Cx, HMC5883L_Addr, 0x02, 0x00);
}
/**
  * @brief  get HMCL5883 data.
  * @param  None
  * @retval None
	* @note 	periph i2c write hmcl5883 test.	 
						struct hmcl5883_data tmp_hmcl5883 = get_data_from_hmcl5883(I2C3);
						printf("hmcl5883 data, x: %d, y: %d, z: %d \r\n", tmp_hmcl5883.x, tmp_hmcl5883.y, tmp_hmcl5883.z);	
  */
struct hmcl5883_data get_data_from_hmcl5883(I2C_TypeDef *I2Cx)
{
		struct hmcl5883_data tmp_hmcl5883 = {0};
		char hmcl_buf[2] = {0};
		hmcl_buf[0] = I2C_Read(I2C3, HMC5883L_Addr, 0x03);
		hmcl_buf[1] = I2C_Read(I2C3, HMC5883L_Addr, 0x04);
		tmp_hmcl5883.x = ((hmcl_buf[0] << 8) | hmcl_buf[1]);
		
		hmcl_buf[0] = I2C_Read(I2C3, HMC5883L_Addr, 0x05);
		hmcl_buf[1] = I2C_Read(I2C3, HMC5883L_Addr, 0x06);
		tmp_hmcl5883.z = ((hmcl_buf[0] << 8) | hmcl_buf[1]);
		
		hmcl_buf[0] = I2C_Read(I2C3, HMC5883L_Addr, 0x07);
		hmcl_buf[1] = I2C_Read(I2C3, HMC5883L_Addr, 0x08);
		tmp_hmcl5883.y = ((hmcl_buf[0] << 8) | hmcl_buf[1]);
		return tmp_hmcl5883;
}

/************************ (C) COPYRIGHT wallea Hu *****END OF FILE****/
