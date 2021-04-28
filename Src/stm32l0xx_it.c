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
  * Brief   This function handles I2C3 (Master) interrupt request.
  * Param   None
  * Retval  None
  */
void I2C3_IRQHandler(void)
{	
//  /* Check RXNE flag value in ISR register */
//  if(LL_I2C_IsActiveFlag_RXNE(I2C3))
//  {
//    /* Call function Master Reception Callback */
//    printf("%d\r", LL_I2C_ReceiveData8(I2C3));
//  }
//  /* Check STOP flag value in ISR register */
//  else if(LL_I2C_IsActiveFlag_STOP(I2C3))
//  {
//    /* End of Transfer */
//    LL_I2C_ClearFlag_STOP(I2C3);

//    /* Call function Master Complete Callback */
//		/* Read Received character.
//		RXNE flag is cleared by reading of RXDR register */
//  }
//  else
//  {
//    /* Call Error function */
//    /* Disable I2C1_IRQn and I2C3_IRQn */
//		NVIC_DisableIRQ(I2C3_IRQn);
//  }
}

void I2C1_IRQHandler(void)
{
  /* Check ADDR flag value in ISR register */
  if(LL_I2C_IsActiveFlag_ADDR(I2C1))
  {
    /* Verify the Address Match with the OWN Slave address */
    if(LL_I2C_GetAddressMatchCode(I2C1) == SLAVE_OWN_ADDRESS)
    {
      /* Verify the transfer direction, a read direction, Slave enters transmitter mode */
      if(LL_I2C_GetTransferDirection(I2C1) == LL_I2C_DIRECTION_READ)
      {
        /* Clear ADDR flag value in ISR register */
        LL_I2C_ClearFlag_ADDR(I2C1);

        /* Enable Transmit Interrupt */
        LL_I2C_EnableIT_TX(I2C1);
      }
      else
      {
        /* Clear ADDR flag value in ISR register */
        LL_I2C_ClearFlag_ADDR(I2C1);

        /* Call Error function */
				NVIC_DisableIRQ(I2C1_IRQn);
      }
    }
    else
    {
      /* Clear ADDR flag value in ISR register */
      LL_I2C_ClearFlag_ADDR(I2C1);
        
      /* Call Error function */
      NVIC_DisableIRQ(I2C1_IRQn);
    }
  }
  /* Check NACK flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_NACK(I2C1))
  {
    /* End of Transfer */
    LL_I2C_ClearFlag_NACK(I2C1);
  }
  /* Check TXIS flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_TXIS(I2C1))
  {
    /* Call function Slave Ready to Transmit Callback */
		LL_I2C_TransmitData8(I2C1, 0x96);
  }
  /* Check STOP flag value in ISR register */
  else if(LL_I2C_IsActiveFlag_STOP(I2C1))
  {
    /* Clear STOP flag value in ISR register */
    LL_I2C_ClearFlag_STOP(I2C1);
  }
  /* Check TXE flag value in ISR register */
  else if(!LL_I2C_IsActiveFlag_TXE(I2C1))
  {
    /* Do nothing */
    /* This Flag will be set by hardware when the TXDR register is empty */
    /* If needed, use LL_I2C_ClearFlag_TXE() interface to flush the TXDR register  */
  }
  else
  {
    /* Call Error function */
		NVIC_DisableIRQ(I2C3_IRQn);
  }
}

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
