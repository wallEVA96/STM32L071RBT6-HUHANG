/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CFG_UARTX_H
#define __CFG_UARTX_H

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
#include "stdio.h" 
#include "string.h" 

/* Exported Defines ------------------------------------------------------------*/
void Configure_USARTx(USART_TypeDef *USARTx_INSTANCE);
void Buffer_Transfer_USARTx(USART_TypeDef *USARTx_INSTANCE, uint8_t *buffer, size_t buf_size);
void TR_Loop_Test_USARTx(USART_TypeDef *USARTx_INSTANCE);

/**
  * @brief  Function called for achieving TX buffer sending
  * @param  None
  * @retval None
  */

#endif /* __CFG_UARTX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
