/**
  ******************************************************************************
  * @file    uart driver
  * @author  wallea hu
  * @brief   USART LL module driver.
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "cfg_uartx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function  -----------------------------------------------*/
/**
  * @brief  overwrite fputs for printf.
  * @note   default to use USART2.
  * @param  
  * @retval 
  */
#define PRINTF_USART USART1
_ARMABI int fputc(int c, FILE * stream){
		while (!LL_USART_IsActiveFlag_TXE(PRINTF_USART)){}
    LL_USART_TransmitData8(PRINTF_USART, (uint8_t) c);
		return c;
}

/**
  * @brief  This function configures USARTx Instance.
  * @note   This function is used to :
  *         -1- Enable GPIO clock and configures the USART pins.
  *         -2- Enable the USART peripheral clock and clock source.
  *         -3- Configure USART functional parameters.
  *         -4- Enable USART.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_USARTx(USART_TypeDef *USARTx_INSTANCE)
{
  /* Enable the peripheral clock of GPIO Port */
	if(USARTx_INSTANCE == USART2){
		/* USART2 instance is used. (TX on PA.02, RX on PA.03)*/
		LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
		
		/* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
		LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_4);
		LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_HIGH);
		LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_UP);
		
		/* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
		LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_4);
		LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);
		LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_3, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);
		
		/* Enable USART peripheral clock and clock source ***********************/
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
		LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
		
		/* TX/RX direction */
		LL_USART_SetTransferDirection(USARTx_INSTANCE, LL_USART_DIRECTION_TX_RX);
		LL_USART_ConfigCharacter(USARTx_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
		LL_USART_SetBaudRate(USARTx_INSTANCE, HSE_VALUE, LL_USART_OVERSAMPLING_16, 9600); 
		LL_USART_Enable(USARTx_INSTANCE);
		
		/* Polling USART initialisation */
		while((!(LL_USART_IsActiveFlag_TEACK(USARTx_INSTANCE))) || (!(LL_USART_IsActiveFlag_REACK(USARTx_INSTANCE)))){}
		
		/* NVIC Configuration for USART interrupts */
		/*  - Set priority for USARTx_IRQn */
		/*  - Enable USARTx_IRQn */
		NVIC_SetPriority(USART2_IRQn, 0);  
		NVIC_EnableIRQ(USART2_IRQn);
		/* Enable RXNE and Error interrupts */
		LL_USART_EnableIT_RXNE(USARTx_INSTANCE);
		LL_USART_EnableIT_ERROR(USARTx_INSTANCE);
	}
	else if(USARTx_INSTANCE == USART1){
		/* USART2 instance is used. (TX on PA.09, RX on PA.10)*/
		LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
		
		/* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
		LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_9, LL_GPIO_AF_4);
		LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
		LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);
		
		/* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
		LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_10, LL_GPIO_AF_4);
		LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_HIGH);
		LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_10, LL_GPIO_PULL_UP);
		
		/* Enable USART peripheral clock and clock source ***********************/
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
		LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2);
		
		/* TX/RX direction */
		LL_USART_SetTransferDirection(USARTx_INSTANCE, LL_USART_DIRECTION_TX_RX);
		LL_USART_ConfigCharacter(USARTx_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
		LL_USART_SetBaudRate(USARTx_INSTANCE, HSE_VALUE, LL_USART_OVERSAMPLING_16, 9600); 
		LL_USART_Enable(USARTx_INSTANCE);
		
		/* Polling USART initialisation */
		while((!(LL_USART_IsActiveFlag_TEACK(USARTx_INSTANCE))) || (!(LL_USART_IsActiveFlag_REACK(USARTx_INSTANCE)))){}

		/* NVIC Configuration for USART interrupts */
		/*  - Set priority for USARTx_IRQn */
		/*  - Enable USARTx_IRQn */
		NVIC_SetPriority(USART1_IRQn, 1);  
		NVIC_EnableIRQ(USART1_IRQn);
		/* Enable RXNE and Error interrupts */
		LL_USART_EnableIT_RXNE(USARTx_INSTANCE);
		LL_USART_EnableIT_ERROR(USARTx_INSTANCE);
	}
	else if(USARTx_INSTANCE == USART4)
	{
		/* USART4 instance is used. (TX on PC.10, RX on PC.11)*/
		LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
		
		/* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
		LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_10, LL_GPIO_AF_6);
		LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_HIGH);
		LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_10, LL_GPIO_PULL_UP);
		
		/* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
		LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_11, LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_11, LL_GPIO_AF_6);
		LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_HIGH);
		LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_11, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_11, LL_GPIO_PULL_UP);
		
		/* Enable USART peripheral clock and clock source ***********************/
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART4);
		/* USART4 clock is derived from system clock */
		
		/* TX/RX direction */
		LL_USART_SetTransferDirection(USARTx_INSTANCE, LL_USART_DIRECTION_TX_RX);
		LL_USART_ConfigCharacter(USARTx_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
		LL_USART_SetBaudRate(USARTx_INSTANCE, HSE_VALUE, LL_USART_OVERSAMPLING_16, 9600); 
		LL_USART_Enable(USARTx_INSTANCE);
		
		/* Polling USART initialisation */
		while((!(LL_USART_IsActiveFlag_TEACK(USARTx_INSTANCE))) || (!(LL_USART_IsActiveFlag_REACK(USARTx_INSTANCE)))){}

	}
	else if(USARTx_INSTANCE == USART5)
	{
		/* USART5 instance is used. (TX on PB.3, RX on PB.4)*/
		LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
		
		/* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
		LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_3, LL_GPIO_AF_6);
		LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);
		LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_3, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);
		
		/* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
		LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4, LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_4, LL_GPIO_AF_6);
		LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_4, LL_GPIO_SPEED_FREQ_HIGH);
		LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_4, LL_GPIO_OUTPUT_PUSHPULL);
		LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_4, LL_GPIO_PULL_UP);
		
		/* Enable USART peripheral clock and clock source ***********************/
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART5);
		/* USART4 clock is derived from system clock */
		
		/* If disenable relavent interrupt, disable Overrun Detect is a better choice */
		/* This bit will cause receive error to avoid data lost. */
		LL_USART_DisableOverrunDetect(USARTx_INSTANCE);
		
		/* TX/RX direction */
		LL_USART_SetTransferDirection(USARTx_INSTANCE, LL_USART_DIRECTION_TX_RX);
		LL_USART_ConfigCharacter(USARTx_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
		LL_USART_SetBaudRate(USARTx_INSTANCE, HSE_VALUE, LL_USART_OVERSAMPLING_16, 9600); 
		LL_USART_Enable(USARTx_INSTANCE);
		
		/* Polling USART initialisation */
		while((!(LL_USART_IsActiveFlag_TEACK(USARTx_INSTANCE))) || (!(LL_USART_IsActiveFlag_REACK(USARTx_INSTANCE)))){}

	}
	else{
		// #warning "Please set a legal usart device."
	}
}

/**
  * @brief  Function called for achieving TX buffer sending
  * @param  None
  * @retval None
  */
const uint8_t aStringToSend[] = "HUHANG-STM32L071RBT6\r\n";
void Buffer_Transfer_USARTx(USART_TypeDef *USARTx_INSTANCE)
{
	uint8_t ubSend = 0;
  /* Send characters one per one, until last char to be sent */
  while (ubSend < sizeof(aStringToSend))
  {
    /* Wait for TXE flag to be raised */
    while (!LL_USART_IsActiveFlag_TXE(USARTx_INSTANCE)){}

    /* If last char to be sent, clear TC flag */
    if (ubSend == (sizeof(aStringToSend) - 1))
    {
      LL_USART_ClearFlag_TC(USARTx_INSTANCE); 
    }

    /* Write character in Transmit Data register.
       TXE flag is cleared by writing data in TDR register */
    LL_USART_TransmitData8(USARTx_INSTANCE, aStringToSend[ubSend++]);
	}

  /* Wait for TC flag to be raised for last char */
  while (!LL_USART_IsActiveFlag_TC(USARTx_INSTANCE)){}
}

/**
  * @brief  Transmiss the Data that Receive.
	* @note  It have a bug: when receive more than a byte data, it will can't transmit receive data.
		
  * @param  None
  * @retval None
  */
void TR_Loop_Test_USARTx(USART_TypeDef *USARTx_INSTANCE)
{
		if(LL_USART_IsActiveFlag_RXNE(USARTx_INSTANCE)){
			
			while (!LL_USART_IsActiveFlag_TXE(USARTx_INSTANCE)){}				
			LL_USART_TransmitData8(USARTx_INSTANCE, LL_USART_ReceiveData8(USARTx_INSTANCE));		
		}
}

/************************ (C) COPYRIGHT wallea Hu *****END OF FILE****/
