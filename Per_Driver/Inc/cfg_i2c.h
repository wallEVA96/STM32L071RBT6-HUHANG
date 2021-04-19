/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CFG_I2C_H
#define __CFG_I2C_H

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
#include "stdio.h"
#include "main.h"

#endif /* USE_FULL_ASSERT */

/* Exported Defines ------------------------------------------------------------*/
#define SLAVE_OWN_ADDRESS 0x5A 
void Configure_I2Cx_Master(I2C_TypeDef *I2Cx);
void Configure_I2Cx_Slave(I2C_TypeDef *I2Cx);
void I2C_Write(I2C_TypeDef *I2Cx, unsigned char slave_addr, unsigned char reg_addr, unsigned char reg_data);
unsigned char I2C_Read(I2C_TypeDef *I2Cx, unsigned char slave_addr, unsigned char reg_addr);

#endif /* __CFG_I2C_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
