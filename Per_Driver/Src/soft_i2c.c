/**
  ******************************************************************************
  * @file    led configure
  * @author  wallea hu
  * @brief   
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "soft_i2c.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define IIC_SCL_GPIO_CLK		LL_IOP_GRP1_PERIPH_GPIOC
//#define IIC_SDA_GPIO_CLK		LL_IOP_GRP1_PERIPH_GPIOC
//#define IIC_SCL_GPIOx 			GPIOC
//#define SCL_PIN 						LL_GPIO_PIN_6
//#define IIC_SDA_GPIOx 			GPIOC
//#define SDA_PIN 						LL_GPIO_PIN_9
#define SCL_H(IIC_SCL_GPIOx, SCL_PIN)         LL_GPIO_SetOutputPin(IIC_SCL_GPIOx,   SCL_PIN)
#define SCL_L(IIC_SCL_GPIOx, SCL_PIN)         LL_GPIO_ResetOutputPin(IIC_SCL_GPIOx, SCL_PIN)
#define SDA_H(IIC_SDA_GPIOx, SDA_PIN)         LL_GPIO_SetOutputPin(IIC_SDA_GPIOx,   SDA_PIN)
#define SDA_L(IIC_SDA_GPIOx, SDA_PIN)         LL_GPIO_ResetOutputPin(IIC_SDA_GPIOx, SDA_PIN)
#define SCL_READ(IIC_SCL_GPIOx, SCL_PIN)      LL_GPIO_IsInputPinSet(IIC_SCL_GPIOx,  SCL_PIN)
#define SDA_READ(IIC_SDA_GPIOx, SDA_PIN)   		LL_GPIO_IsInputPinSet(IIC_SDA_GPIOx,  SDA_PIN)

/* Private variables ---------------------------------------------------------*/
/* Private function  -----------------------------------------------*/
/**
  * @brief  This function configures IIC Delay for multi-NOP
  * @param  None
  * @retval None
  */
#define I2C_DELAY_COUNT  20    // ~= 19.41khz
__STATIC_INLINE void I2C_DELAY(void){
		int tmp_i2c_elay_count = I2C_DELAY_COUNT;
		do{
			__NOP;  
			__NOP;
		}while(tmp_i2c_elay_count--);
}
											
/**
  * @brief  This function configures IIC GPIO init
  * @param  None
  * @retval None
  */

void Configure_SOFT_IIC_GPIO(GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
														 GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN)
{
  /* Enable the GPIOC Clock */
	if(IIC_SDA_GPIOx == GPIOA || IIC_SCL_GPIOx == GPIOA)
		LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
	if(IIC_SDA_GPIOx == GPIOB || IIC_SCL_GPIOx == GPIOB)
		LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
	if(IIC_SDA_GPIOx == GPIOC || IIC_SCL_GPIOx == GPIOC)
		LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
	if(IIC_SDA_GPIOx == GPIOD || IIC_SCL_GPIOx == GPIOD)
		LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  /* Configure SCL Pin as : High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(IIC_SCL_GPIOx, SCL_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed(IIC_SCL_GPIOx, SCL_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(IIC_SCL_GPIOx, SCL_PIN, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(IIC_SCL_GPIOx, SCL_PIN, LL_GPIO_PULL_UP);
	/* Configure SDA Pin as : High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(IIC_SDA_GPIOx, SDA_PIN, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed(IIC_SDA_GPIOx, SDA_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(IIC_SDA_GPIOx, SDA_PIN, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(IIC_SDA_GPIOx, SDA_PIN, LL_GPIO_PULL_UP);
	SCL_H(IIC_SCL_GPIOx, SCL_PIN);
	SDA_H(IIC_SDA_GPIOx, SDA_PIN);
	I2C_DELAY();
}

/**
  * @brief  This function configures i2c pin for input or output.
  * @param  None
  * @retval None
  */
#define I2C_SET_SDA_IN(IIC_SDA_GPIOx, SDA_PIN)  LL_GPIO_SetPinMode(IIC_SDA_GPIOx, SDA_PIN, LL_GPIO_MODE_INPUT)
#define I2C_SET_SDA_OUT(IIC_SDA_GPIOx,SDA_PIN)  LL_GPIO_SetPinMode(IIC_SDA_GPIOx, SDA_PIN, LL_GPIO_MODE_OUTPUT)

/**
  * @brief  This function configures IIC Start
  * @param  None
  * @retval None
  */
static int I2C_Start(GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
										 GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN)
{
	I2C_SET_SDA_OUT(IIC_SDA_GPIOx, SDA_PIN);
	SDA_H(IIC_SDA_GPIOx, SDA_PIN);
	SCL_H(IIC_SCL_GPIOx, SCL_PIN);
	I2C_DELAY();
	if(!SDA_READ(IIC_SDA_GPIOx, SDA_PIN))
	{
		return 0;
	}
	SDA_L(IIC_SDA_GPIOx, SDA_PIN);
	I2C_DELAY();
	if(SDA_READ(IIC_SDA_GPIOx, SDA_PIN))
	{
		return 0;
	}
	SCL_L(IIC_SCL_GPIOx, SCL_PIN);
	return 1;
}

/**
  * @brief  This function configures IIC Stop
  * @param  None
  * @retval None
  */
static void I2C_Stop(GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
										 GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN)
{
	I2C_SET_SDA_OUT(IIC_SDA_GPIOx,SDA_PIN);
	//SCL_L;
	SDA_L(IIC_SDA_GPIOx, SDA_PIN);
	SCL_H(IIC_SCL_GPIOx, SCL_PIN);
	I2C_DELAY();

	SDA_H(IIC_SDA_GPIOx, SDA_PIN);
	I2C_DELAY();	
}

/**
  * @brief  This function configures IIC Ack
  * @param  None
  * @retval None
  */
static void I2C_Ack(GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
										GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN)
{
	SCL_L(IIC_SCL_GPIOx, SCL_PIN);
	I2C_SET_SDA_OUT(IIC_SDA_GPIOx,SDA_PIN);
	SDA_L(IIC_SDA_GPIOx, SDA_PIN);
	I2C_DELAY();
	SCL_H(IIC_SCL_GPIOx, SCL_PIN);
	I2C_DELAY();
	SCL_L(IIC_SCL_GPIOx, SCL_PIN);
}

/**
  * @brief  This function configures IIC NoAck
  * @param  None
  * @retval None
  */
static void I2C_NoAck(GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
										  GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN)
{
	SCL_L(IIC_SCL_GPIOx, SCL_PIN);
	I2C_SET_SDA_OUT(IIC_SDA_GPIOx,SDA_PIN);
	
	I2C_DELAY();
	SDA_H(IIC_SDA_GPIOx, SDA_PIN);
	I2C_DELAY();
	SCL_H(IIC_SCL_GPIOx, SCL_PIN);
	I2C_DELAY();
	SCL_L(IIC_SCL_GPIOx, SCL_PIN);
}

/**
  * @brief  This function configures IIC getAck
  * @param  None
  * @retval None
  */
static uint8_t I2C_GetAck(GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
									 GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN)
{
  uint8_t times = 0;
	
	I2C_SET_SDA_IN(IIC_SDA_GPIOx, SDA_PIN);
	SDA_H(IIC_SDA_GPIOx, SDA_PIN);
	SCL_H(IIC_SCL_GPIOx, SCL_PIN);
	I2C_DELAY();
	while(SDA_READ(IIC_SDA_GPIOx, SDA_PIN))
	{
		times++;
		if(times> 250)
		{			
			SCL_L(IIC_SCL_GPIOx, SCL_PIN);
			return 0;
		}
	}
	SCL_L(IIC_SCL_GPIOx, SCL_PIN);
	return 1;
}
/**
  * @brief  This function configures IIC write data.
  * @param  None
  * @retval None
  */
static void I2C_WriteByte(uint8_t Data,
									 GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
									 GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN)
{
  uint8_t cnt = 0;
	
  I2C_SET_SDA_OUT(IIC_SDA_GPIOx,SDA_PIN);
  for(cnt=0; cnt<8; cnt++)
  {
    SCL_L(IIC_SCL_GPIOx, SCL_PIN);
    I2C_DELAY();
 
    if(Data & 0x80)
    {
      SDA_H(IIC_SDA_GPIOx, SDA_PIN);
    }
    else
    {
      SDA_L(IIC_SDA_GPIOx, SDA_PIN);
    }
    Data <<= 1;
    SCL_H(IIC_SCL_GPIOx, SCL_PIN);
		I2C_DELAY();
  }
  SCL_L(IIC_SCL_GPIOx, SCL_PIN);
  I2C_DELAY();
}

/**
  * @brief  This function configures IIC read byte
  * @param  None
  * @retval None
  */
static uint8_t I2C_ReadByte(uint8_t ack,
										 GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
									   GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN)
{
  uint8_t cnt = 0;
  static uint8_t data = 0;
  
	data = 0;
	I2C_SET_SDA_IN(IIC_SDA_GPIOx, SDA_PIN);

  for(cnt=0; cnt<8; cnt++)
  {
    SCL_L(IIC_SCL_GPIOx, SCL_PIN);                 
    I2C_DELAY();
		
    SCL_H(IIC_SCL_GPIOx, SCL_PIN);                             
    data <<= 1;
    if(SDA_READ(IIC_SDA_GPIOx, SDA_PIN))
    {
      data |= 0x01;                              
    }
    I2C_DELAY();
  }
	
  if(ack == 1)
  {
     I2C_NoAck(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
  }
  else
  {
		 I2C_Ack(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
  }
  return data;                                  
}

/**
  * @brief  a common method for writing device .
  * @param  None
  * @retval None
  */
int Common_WriteByte(uint16_t device, uint16_t addr, uint8_t data, 
										 GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
										 GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN)
{
	I2C_Start(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
	I2C_WriteByte(device << 1 | 0x00, IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
  if(!I2C_GetAck(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN))
  {
		I2C_Stop(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
		return 0;
  }
	
  I2C_WriteByte((uint8_t)(addr&0x00FF), IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);  
	// Must get ack to avoid slaver happen bus error.
  I2C_GetAck(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
  I2C_WriteByte(data, IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
	I2C_GetAck(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
  I2C_Stop(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
	return 1;
}

/**
  * @brief  a common method for reading device.
  * @param  None
  * @retval None
  */
int Common_ReadByte(uint16_t device, uint16_t addr, uint8_t *data,
										GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
										GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN)
{
	I2C_Start(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
	I2C_WriteByte(device << 1 | 0x00, IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
  if(!I2C_GetAck(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN))
  {
		I2C_Stop(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
		return 0;
  } 
	I2C_WriteByte((uint8_t)(addr & 0x00FF), IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
	I2C_GetAck(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
	
  I2C_Start(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
	I2C_WriteByte(device << 1 | 0x01, IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);	
	if(!I2C_GetAck(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN))
	{
		I2C_Stop(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
		return 0;
  }
	
  *data = I2C_ReadByte(1, IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
	I2C_Stop(IIC_SDA_GPIOx, SDA_PIN, IIC_SCL_GPIOx, SCL_PIN);
	return 1;
}

/************************ (C) COPYRIGHT wallea Hu *****END OF FILE****/
