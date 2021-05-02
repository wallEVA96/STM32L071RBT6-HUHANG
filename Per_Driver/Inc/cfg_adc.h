/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CFG_ADC_H
#define __CFG_ADC_H

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
struct adc1_data{
uint32_t chn4;
uint32_t vref;
int32_t temp;
};
 
/* Exported Function ------------------------------------------------------------*/
void Configure_ADC1(void);
void Activate_ADC1(void);
struct adc1_data ConversionStartPoll_ADC1_GrpRegular(void);
void Configure_ADC1_TO_DMA(void);

#endif 
/************************ (C) COPYRIGHT walleva *****END OF FILE****/
