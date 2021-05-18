/**
  ******************************************************************************
  * @file    Templates_LL/Src/stm32l0xx.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_it.h"
#include "cfg_uartx.h"
#include "cfg_i2c.h"
#include "cfg_led.h"
#include "cfg_sys_clk.h"

/** @addtogroup STM32L0xx_LL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32L0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @Brief   This function handles USARTx Instance interrupt request.
  * @Param   None
  * @Retval  None
  */
void USART2_IRQHandler(void)
{
  /* Check RXNE flag value in ISR register */
  if(LL_USART_IsActiveFlag_RXNE(USART2) && LL_USART_IsEnabledIT_RXNE(USART2))
  {
    /* RXNE flag will be cleared by reading of RDR register (done in call) */
    /* Call function in charge of handling Character reception */
		LL_USART_TransmitData8(USART2, LL_USART_ReceiveData8(USART2));
  }
  else
  {
    /* Call Error function */
  }
}

/**
  * @Brief   This function handles USARTx Instance interrupt request.
  * @Param   None
  * @Retval  None
  */
void USART1_IRQHandler(void)
{
  /* Check RXNE flag value in ISR register */
  if(LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
  {
    /* RXNE flag will be cleared by reading of RDR register (done in call) */
    /* Call function in charge of handling Character reception */
		LL_USART_TransmitData8(USART1, LL_USART_ReceiveData8(USART1));
  }
  else
  {
    /* Call Error function */
  }
}

/**
  * @brief  This function handles DMA1 interrupt request.
  * @param  None
  * @retval None
  */
extern __IO uint8_t itDmaTransferStatus; /* Variable set into DMA interruption callback */
void DMA1_Channel1_IRQHandler(void)
{
	  /* Check whether DMA transfer complete caused the DMA interruption */
  if(LL_DMA_IsActiveFlag_TC1(DMA1) == 1)
  {
    /* Clear flag DMA global interrupt */
    /* (global interrupt flag: half transfer and transfer complete flags) */
    LL_DMA_ClearFlag_GI1(DMA1);
    
    /* Call interruption treatment function */
     itDmaTransferStatus = 1;
  }
  /* Check whether DMA transfer error caused the DMA interruption */
  if(LL_DMA_IsActiveFlag_TE1(DMA1) == 1)
  {
    /* Clear flag DMA transfer error */
    LL_DMA_ClearFlag_TE1(DMA1);
    
    /* Call interruption treatment function */
  }
}

/**
  * Brief   This function handles RTC interrupt request.
  * Param   None
  * Retval  None
  */
void RTC_IRQHandler(void)
{
  /* Check WUT flag */
  if(LL_RTC_IsActiveFlag_WUT(RTC) == 1)
  {
    /* Reset Wake up flag */
    LL_RTC_ClearFlag_WUT(RTC); 
    /* clear exti line 20 flag */
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_20);	
  }
}
/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
