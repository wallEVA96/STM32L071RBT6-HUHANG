/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPLY_BC_H
#define __APPLY_BC_H

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
#define BC_RES_LEN 30
void BCxx_UDP_Init(USART_TypeDef *USARTx);
char BCxx_UDP_Create_Socket(USART_TypeDef *USARTx);
char BCxx_UDP_Send(USART_TypeDef *USARTx, int32_t socket_id);

extern uint8_t global_bcxx_response[BC_RES_LEN];
extern uint8_t global_bcxx_res_index;
#endif 

/************************ (C) COPYRIGHT walleva *****END OF FILE****/
