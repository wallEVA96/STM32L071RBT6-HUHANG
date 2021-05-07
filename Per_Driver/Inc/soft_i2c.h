/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H

/* Includes ------------------------------------------------------------------*/
/* LL drivers common to all LL examples */
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_gpio.h"
/* LL drivers specific to LL examples IPs */
#include "stm32l0xx_ll_adc.h"
#include "stm32l0xx_ll_comp.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_crc.h"
#include "stm32l0xx_ll_crs.h"
#include "stm32l0xx_ll_dac.h"
#include "stm32l0xx_ll_dma.h"
#include "stm32l0xx_ll_i2c.h"
#include "stm32l0xx_ll_iwdg.h"
#include "stm32l0xx_ll_lptim.h"
#include "stm32l0xx_ll_lpuart.h"
#include "stm32l0xx_ll_rng.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_spi.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_wwdg.h"
#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported Defines ------------------------------------------------------------*/
void Configure_SOFT_IIC_GPIO(GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
														 GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN);
int Common_WriteByte(uint16_t device, uint16_t addr, uint8_t data, 
										 GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
										 GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN);
int Common_ReadByte(uint16_t device, uint16_t addr, uint8_t *data,
										GPIO_TypeDef *IIC_SDA_GPIOx, uint32_t SDA_PIN,
										GPIO_TypeDef *IIC_SCL_GPIOx, uint32_t SCL_PIN);

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
