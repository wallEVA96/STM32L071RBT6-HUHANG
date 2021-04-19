/**
  ******************************************************************************
  * @file    uart driver
  * @author  wallea hu
  * @brief   USART LL module driver.
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "cfg_i2c.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Timing register value is computed with the STM32CubeMX Tool,
  * Fast Mode @400kHz with I2CCLK = 32 MHz,
  * rise time = 100ns, fall time = 10ns
  * Timing Value = (uint32_t)0x00601B28
  */
#define I2C_TIMING               0x00601B28

/* Private variables ---------------------------------------------------------*/

/* Private function  -----------------------------------------------*/
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
void Configure_I2Cx_Master(I2C_TypeDef *I2Cx)
{
	/* (1) Enables GPIO clock and configures the I2C3 pins **********************/
  /*    (SCL on PC.0, SDA on PC.1)                     **********************/

  /* Enable the peripheral clock of GPIOC */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);

  /* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOC, LL_GPIO_PIN_0, LL_GPIO_AF_7);
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_0, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_0, LL_GPIO_PULL_UP);

  /* Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOC, LL_GPIO_PIN_1, LL_GPIO_AF_7);
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_1, LL_GPIO_PULL_UP);

  /* (2) Enable the I2C3 peripheral clock and I2C3 clock source ***************/

  /* Enable the peripheral clock for I2C3 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C3);

  /* Set I2C3 clock source as SYSCLK */
  LL_RCC_SetI2CClockSource(LL_RCC_I2C3_CLKSOURCE_SYSCLK);
  
  /* (3) Configure NVIC for I2C3 **********************************************/

  /* Configure Event and Error IT:
   *  - Set priority for I2C3_IRQn
   *  - Enable I2C3_IRQn
   */
//  NVIC_SetPriority(I2C3_IRQn, 0);
//  NVIC_EnableIRQ(I2C3_IRQn);

  /* (4) Configure I2C3 functional parameters *********************************/

  /* Disable I2C3 prior modifying configuration registers */
  LL_I2C_Disable(I2C3);

  /* Configure the SDA setup, hold time and the SCL high, low period */
  /* (uint32_t)0x00601B28 = I2C_TIMING*/
	uint32_t timing = __LL_I2C_CONVERT_TIMINGS(0x0, 0x6, 0x0, 0x1B, 0x28);

  LL_I2C_SetTiming(I2C3, timing);

  /* Configure the Own Address1                   */
  /* Reset Values of :
   *     - OwnAddress1 is 0x00
   *     - OwnAddrSize is LL_I2C_OWNADDRESS1_7BIT
   *     - Own Address1 is disabled
   */
  //LL_I2C_SetOwnAddress1(I2C3, 0x00, LL_I2C_OWNADDRESS1_7BIT);
  //LL_I2C_DisableOwnAddress1(I2C3);

  /* Enable Clock stretching */
  /* Reset Value is Clock stretching enabled */
  //LL_I2C_EnableClockStretching(I2C3);

  /* Configure Digital Noise Filter */
  /* Reset Value is 0x00            */
  //LL_I2C_SetDigitalFilter(I2C3, 0x00);

  /* Enable Analog Noise Filter           */
  /* Reset Value is Analog Filter enabled */
  //LL_I2C_EnableAnalogFilter(I2C3);

  /* Enable General Call                  */
  /* Reset Value is General Call disabled */
  //LL_I2C_EnableGeneralCall(I2C3);

  /* Configure the 7bits Own Address2               */
  /* Reset Values of :
   *     - OwnAddress2 is 0x00
   *     - OwnAddrMask is LL_I2C_OWNADDRESS2_NOMASK
   *     - Own Address2 is disabled
   */
  //LL_I2C_SetOwnAddress2(I2C3, 0x00, LL_I2C_OWNADDRESS2_NOMASK);
  //LL_I2C_DisableOwnAddress2(I2C3);

  /* Configure the Master to operate in 7-bit or 10-bit addressing mode */
  /* Reset Value is LL_I2C_ADDRESSING_MODE_7BIT                         */
  //LL_I2C_SetMasterAddressingMode(I2C3, LL_I2C_ADDRESSING_MODE_7BIT);

  /* Enable Peripheral in I2C mode */
  /* Reset Value is I2C mode */
  //LL_I2C_SetMode(I2C3, LL_I2C_MODE_I2C);

  /* (5) Enable I2C3 **********************************************************/
  LL_I2C_Enable(I2C3);

  /* (6) Enable I2C3 transfer complete/error interrupts:
   *  - Enable Receive Interrupt
   *  - Enable Not acknowledge received interrupt
   *  - Enable Error interrupts
   *  - Enable Stop interrupt
   */
//  LL_I2C_EnableIT_RX(I2C3);
//  LL_I2C_EnableIT_NACK(I2C3);
//  LL_I2C_EnableIT_ERR(I2C3);
//  LL_I2C_EnableIT_STOP(I2C3);	
}

void Configure_I2Cx_Slave(I2C_TypeDef *I2Cx)
{
  uint32_t timing = 0;

  /* (1) Enables GPIO clock and configures the I2C1 pins **********************/
  /*    (SCL on PB.8, SDA on PB.9)                     **********************/

  /* Enable the peripheral clock of GPIOB */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

  /* Configure SCL Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_8, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_8, LL_GPIO_PULL_UP);

  /* Configure SDA Pin as : Alternate function, High Speed, Open drain, Pull up */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_9, LL_GPIO_AF_4);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_OPENDRAIN);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);

  /* (2) Enable the I2C1 peripheral clock and I2C1 clock source ***************/

  /* Enable the peripheral clock for I2C1 */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* Set I2C1 clock source as SYSCLK */
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_SYSCLK);
  
  /* (3) Configure NVIC for I2C1 **********************************************/

  /* Configure Event and Error IT:
   *  - Set priority for I2C1_IRQn
   *  - Enable I2C1_IRQn
   */
  NVIC_SetPriority(I2C1_IRQn, 0);
  NVIC_EnableIRQ(I2C1_IRQn);

  /* (4) Configure I2C1 functional parameters *********************************/

  /* Disable I2C1 prior modifying configuration registers */
  LL_I2C_Disable(I2C1);

  /* Configure the SDA setup, hold time and the SCL high, low period */
  /* Timing register value is computed with the STM32CubeMX Tool,
    * Fast Mode @400kHz with I2CCLK = 32 MHz,
    * rise time = 100ns, fall time = 10ns
    * Timing Value = (uint32_t)0x00601B28
    */
  timing = __LL_I2C_CONVERT_TIMINGS(0x0, 0x6, 0x0, 0x1B, 0x28);
  LL_I2C_SetTiming(I2C1, timing);

  /* Configure the Own Address1 :
   *  - OwnAddress1 is SLAVE_OWN_ADDRESS
   *  - OwnAddrSize is LL_I2C_OWNADDRESS1_7BIT
   *  - Own Address1 is enabled
   */
  LL_I2C_SetOwnAddress1(I2C1, SLAVE_OWN_ADDRESS, LL_I2C_OWNADDRESS1_7BIT);
  LL_I2C_EnableOwnAddress1(I2C1);

  /* Enable Clock stretching */
  /* Reset Value is Clock stretching enabled */
  //LL_I2C_EnableClockStretching(I2C1);

  /* Configure Digital Noise Filter */
  /* Reset Value is 0x00            */
  //LL_I2C_SetDigitalFilter(I2C1, 0x00);

  /* Enable Analog Noise Filter           */
  /* Reset Value is Analog Filter enabled */
  //LL_I2C_EnableAnalogFilter(I2C1);

  /* Enable General Call                  */
  /* Reset Value is General Call disabled */
  //LL_I2C_EnableGeneralCall(I2C1);

  /* Configure the 7bits Own Address2               */
  /* Reset Values of :
   *     - OwnAddress2 is 0x00
   *     - OwnAddrMask is LL_I2C_OWNADDRESS2_NOMASK
   *     - Own Address2 is disabled
   */
  //LL_I2C_SetOwnAddress2(I2C1, 0x00, LL_I2C_OWNADDRESS2_NOMASK);
  //LL_I2C_DisableOwnAddress2(I2C1);

  /* Enable Peripheral in I2C mode */
  /* Reset Value is I2C mode */
  //LL_I2C_SetMode(I2C1, LL_I2C_MODE_I2C);

  /* (5) Enable I2C1 **********************************************************/
  LL_I2C_Enable(I2C1);

  /* (6) Enable I2C1 address match/error interrupts:
   *  - Enable Address Match Interrupt
   *  - Enable Not acknowledge received interrupt
   *  - Enable Error interrupts
   *  - Enable Stop interrupt
   */
  LL_I2C_EnableIT_ADDR(I2C1);
  LL_I2C_EnableIT_NACK(I2C1);
  LL_I2C_EnableIT_ERR(I2C1);
  LL_I2C_EnableIT_STOP(I2C1);		
}

/**
  * @brief  This Function handle Master events to perform a transmission process
  * @note  This function is composed in different steps :
  *        -1- Initiate a Start condition to the Slave device
  *        -2- Loop until end of transfer received (STOP flag raised)
  *             -2.1- Transmit data (TXIS flag raised)
  *        -3- Clear pending flags, Data consistency are checking into Slave process
  * @param  None
  * @retval None
  */
void I2C_Write(I2C_TypeDef *I2Cx, unsigned char slave_addr, unsigned char reg_addr, unsigned char reg_data)
{
	unsigned char data_size = 2;
  /* Master Generate Start condition for a write request :              */
  /*    - to the Slave with a 7-Bit SLAVE_OWN_ADDRESS                   */
  /*    - with a auto stop condition generation when transmit all bytes */
  LL_I2C_HandleTransfer(I2Cx, slave_addr, LL_I2C_ADDRSLAVE_7BIT, data_size, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

  /* Loop until STOP flag is raised  */
  while(!LL_I2C_IsActiveFlag_STOP(I2Cx))
  {
    /* Check TXIS flag value in ISR register */
    if(LL_I2C_IsActiveFlag_TXE(I2Cx) && data_size == 2)
    {
      /* Write data in Transmit Data register.
      TXIS flag is cleared by writing data in TXDR register */
      LL_I2C_TransmitData8(I2Cx, reg_addr);
			data_size--;
    }
		/* Check TXIS flag value in ISR register */
    if(LL_I2C_IsActiveFlag_TXE(I2Cx) && data_size == 1)
    {
      /* Write data in Transmit Data register.
      TXIS flag is cleared by writing data in TXDR register */
      LL_I2C_TransmitData8(I2Cx, reg_data);
			data_size--;
    }
		if(Loop_Is_Timeout_Xms(5))
			break;
  }
  /* (3) Clear pending flags, Data consistency are checking into Slave process */

  /* End of I2C_SlaveReceiver_MasterTransmitter Process */
  LL_I2C_ClearFlag_STOP(I2Cx);
}

/**
  * @brief  This Function handle Master events to perform a transmission process
  * @note  This function is composed in different steps :
  *        -1- Initiate a Start condition to the Slave device
  *        -2- Loop until end of transfer received (STOP flag raised)
  *             -2.1- Transmit data (TXIS flag raised)
  *        -3- Clear pending flags, Data consistency are checking into Slave process
  * @param  None
  * @retval None
  */
unsigned char I2C_Read(I2C_TypeDef *I2Cx, unsigned char slave_addr, unsigned char reg_addr)
{
	unsigned char read_byte = 0;
	unsigned char data_size = 1;
  /* Master Generate Start condition for a write request :              */
  /*    - to the Slave with a 7-Bit SLAVE_OWN_ADDRESS                   */
  /*    - with a auto stop condition generation when transmit all bytes */
  LL_I2C_HandleTransfer(I2Cx, slave_addr, LL_I2C_ADDRSLAVE_7BIT, data_size, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

  /* Loop until STOP flag is raised  */
	/* This loop is dangerous when power support is terrrible. */
  while(!LL_I2C_IsActiveFlag_TC(I2Cx))
	{
    /* Check TXIS flag value in ISR register */
    if(LL_I2C_IsActiveFlag_TXE(I2Cx) && data_size == 1)
    {
      /* Write data in Transmit Data register.
      TXIS flag is cleared by writing data in TXDR register */
      LL_I2C_TransmitData8(I2Cx, reg_addr);
			data_size--;
    }
		if(Loop_Is_Timeout_Xms(3))
			break;
  }
	
	data_size = 1;
	LL_I2C_HandleTransfer(I2Cx, slave_addr, LL_I2C_ADDRSLAVE_7BIT, data_size, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
  /* Loop until STOP flag is raised  */
	while(!LL_I2C_IsActiveFlag_STOP(I2Cx))
	{
		if(Loop_Is_Timeout_Xms(4))
			break;
	}
	if(LL_I2C_IsActiveFlag_RXNE(I2Cx))
			read_byte = LL_I2C_ReceiveData8(I2Cx);
  /* (3) Clear pending flags, Data consistency are checking into Slave process */
  LL_I2C_ClearFlag_STOP(I2Cx);

	return read_byte;
}
/************************ (C) COPYRIGHT wallea Hu *****END OF FILE****/
