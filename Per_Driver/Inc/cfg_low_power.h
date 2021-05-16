/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CFG_LOW_POWER
#define __CFG_LOW_POWER

/* Includes ------------------------------------------------------------------*/
/* LL drivers common to all LL examples */
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_ll_gpio.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_cortex.h"
#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported Defines ------------------------------------------------------------*/

/* Exported Function ------------------------------------------------------------*/
void ReduceIOPowerConsumption(void);
void EnterSTOPMode(void);

#endif 
/************************ (C) COPYRIGHT walleva *****END OF FILE****/
