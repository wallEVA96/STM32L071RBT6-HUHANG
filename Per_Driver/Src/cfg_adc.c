/**
  ******************************************************************************
  * @file    uart driver
  * @author  wallea hu
  * @brief   USART LL module driver.
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "cfg_adc.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define ADC_CALIBRATION_TIMEOUT_MS       ((uint32_t)   1)
#define ADC_ENABLE_TIMEOUT_MS            ((uint32_t)   1)
#define ADC_DISABLE_TIMEOUT_MS           ((uint32_t)   1)
#define ADC_STOP_CONVERSION_TIMEOUT_MS   ((uint32_t)   1)
#define ADC_CONVERSION_TIMEOUT_MS        ((uint32_t) 300)

/* Delay between ADC end of calibration and ADC enable.                     */
/* Delay estimation in CPU cycles: Case of ADC enable done                  */
/* immediately after ADC calibration, ADC clock setting slow                */
/* (LL_ADC_CLOCK_ASYNC_DIV32). Use a higher delay if ratio                  */
/* (CPU clock / ADC clock) is above 32.                                     */
#define ADC_DELAY_CALIB_ENABLE_CPU_CYCLES  (LL_ADC_DELAY_CALIB_ENABLE_ADC_CYCLES * 32)
#define VDDA_APPLI                       ((uint32_t)3300)
#define VAR_CONVERTED_DATA_INIT_VALUE    (__LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B) + 1)
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)   3)
__IO uint16_t aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE] = {0}; /* ADC group regular conversion data */
__IO uint8_t itDmaTransferStatus = 0; /* Variable set into DMA interruption callback */

/* Private function -----------------------------------------------------------*/
/**
  * @brief  This function configures DMA for transfer of data from ADC
  * @param  None
  * @retval None
  */
void Configure_ADC1_TO_DMA(void)
{
  /*## Configuration of NVIC #################################################*/
  /* Configure NVIC to enable DMA interruptions */
  NVIC_SetPriority(DMA1_Channel1_IRQn, 1);  /* DMA IRQ lower priority than ADC IRQ */
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  
  /*## Configuration of DMA ##################################################*/
  /* Enable the peripheral clock of DMA */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  
  /* Configure the DMA transfer */
  /*  - DMA transfer in circular mode to match with ADC configuration:        */
  /*    DMA unlimited requests.                                               */
  /*  - DMA transfer from ADC without address increment.                      */
  /*  - DMA transfer to memory with address increment.                        */
  /*  - DMA transfer from ADC by half-word to match with ADC configuration:   */
  /*    ADC resolution 12 bits.                                               */
  /*  - DMA transfer to memory by half-word to match with ADC conversion data */
  /*    buffer variable type: half-word.                                      */
  LL_DMA_ConfigTransfer(DMA1,
                        LL_DMA_CHANNEL_1,
                        LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
                        LL_DMA_MODE_CIRCULAR              |
                        LL_DMA_PERIPH_NOINCREMENT         |
                        LL_DMA_MEMORY_INCREMENT           |
                        LL_DMA_PDATAALIGN_HALFWORD        |
                        LL_DMA_MDATAALIGN_HALFWORD        |
                        LL_DMA_PRIORITY_HIGH               );
  
  /* Select ADC as DMA transfer request */
  LL_DMA_SetPeriphRequest(DMA1, 
                          LL_DMA_CHANNEL_1,
                          LL_DMA_REQUEST_0);
  
  /* Set DMA transfer addresses of source and destination */
  LL_DMA_ConfigAddresses(DMA1,
                         LL_DMA_CHANNEL_1,
                         LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                         (uint32_t)&aADCxConvertedData,
                         LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  
  /* Set DMA transfer size */
  LL_DMA_SetDataLength(DMA1,
                       LL_DMA_CHANNEL_1,
                       ADC_CONVERTED_DATA_BUFFER_SIZE);
  
  /* Enable DMA transfer interruption: transfer complete */
  LL_DMA_EnableIT_TC(DMA1,
                     LL_DMA_CHANNEL_1);
  
  /* Enable DMA transfer interruption: transfer error */
  LL_DMA_EnableIT_TE(DMA1,
                     LL_DMA_CHANNEL_1);
  
  /*## Activation of DMA #####################################################*/
  /* Enable the DMA transfer */
  LL_DMA_EnableChannel(DMA1,
                       LL_DMA_CHANNEL_1);
}

/* Private function ------------------------------------------------------------*/
/**
  * @brief  Configure_ADC function
  * @note   
  * @param  None
  * @retval None
  */
void Configure_ADC1(void)
{
  __IO uint32_t wait_loop_index = 0;
  
  /*## Configuration of GPIO used by ADC channels ############################*/
  
  /* Note: On this STM32 device, ADC1 channel 4 is mapped on GPIO pin PA.04 */  
  /* Enable GPIO Clock */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  
  /* Configure GPIO in analog mode to be used as ADC input */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);
  
  /*## Configuration of NVIC #################################################*/
  /* Configure NVIC to enable ADC1 interruptions */
  //NVIC_SetPriority(ADC1_COMP_IRQn, 0); /* ADC IRQ greater priority than DMA IRQ */
  //NVIC_EnableIRQ(ADC1_COMP_IRQn);
  
  /*## Configuration of ADC ##################################################*/

  /* Enable ADC clock (core clock) */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       All ADC instances of the ADC common group must be disabled.        */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if(__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
  {
    /* Note: Call of the functions below are commented because they are       */
    /*       useless in this example:                                         */
    /*       setting corresponding to default configuration from reset state. */
    
    /* Set ADC clock (conversion clock) common to several ADC instances */
    /* Note: On this STM32 serie, ADC common clock asynchonous prescaler      */
    /*       is applied to each ADC instance if ADC instance clock is         */
    /*       set to clock source asynchronous                                 */
    /*       (refer to function "LL_ADC_SetClock()" below).                   */
    // LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_ASYNC_DIV1);
    
    /* Set ADC measurement path to internal channels */
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), (LL_ADC_PATH_INTERNAL_VREFINT | LL_ADC_PATH_INTERNAL_TEMPSENSOR));

    /* Delay for ADC temperature sensor stabilization time.                   */
    /* Compute number of CPU cycles to wait for, from delay in us.            */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    /* Note: If system core clock frequency is below 200kHz, wait time        */
    /*       is only a few CPU processing cycles.                             */
    /* Note: This delay is implemented here for the purpose in this example.  */
    /*       It can be optimized if merged with other delays                  */
    /*       during ADC activation or if other actions are performed          */
    /*       in the meantime.                                                 */
    wait_loop_index = ((LL_ADC_DELAY_TEMPSENSOR_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }
  
  /*## Configuration of ADC hierarchical scope: multimode ####################*/
  
    /* Note: Feature not available on this STM32 serie */ 
    
  }
  
  
  /*## Configuration of ADC hierarchical scope: ADC instance #################*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled.                                              */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {
    /* Note: Call of the functions below are commented because they are       */
    /*       useless in this example:                                         */
    /*       setting corresponding to default configuration from reset state. */
    
    /* Set ADC clock (conversion clock) */
    LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV2);
    
    /* Set ADC data resolution */
    // LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
    
    /* Set ADC conversion data alignment */
    // LL_ADC_SetResolution(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
    
    /* Set ADC low power mode */
    LL_ADC_SetLowPowerMode(ADC1, LL_ADC_LP_AUTOWAIT_AUTOPOWEROFF);
    
    /* Set ADC channels sampling time */
    /* Note: On this STM32 serie, sampling time is common to all channels     */
    /*       of the entire ADC instance.                                      */
    /*       Therefore, sampling time is configured here under ADC instance   */
    /*       scope (not under channel scope as on some other STM32 devices    */
    /*       on which sampling time is channel wise).                         */
    /* Note: Considering interruption occurring after each ADC group          */
    /*       regular sequence conversions                                     */
    /*       (IT from DMA transfer complete),                                 */
    /*       select sampling time and ADC clock with sufficient               */
    /*       duration to not create an overhead situation in IRQHandler.      */
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_160CYCLES_5);
    
  }
  
  
  /*## Configuration of ADC hierarchical scope: ADC group regular ############*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled or enabled without conversion on going        */
  /*       on group regular.                                                  */
  if ((LL_ADC_IsEnabled(ADC1) == 0)               ||
      (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    /* Set ADC group regular trigger source */
    LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
    
    /* Set ADC group regular trigger polarity */
    // LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);
    
    /* Set ADC group regular continuous mode */
    LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
    
    /* Set ADC group regular conversion data transfer */
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
    
    /* Set ADC group regular overrun behavior */
    LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);
    
    /* Set ADC group regular sequencer */
    /* Note: On this STM32 serie, ADC group regular sequencer is              */
    /*       not fully configurable: sequencer length and each rank           */
    /*       affectation to a channel are fixed by channel HW number.         */
    /*       Refer to description of function                                 */
    /*       "LL_ADC_REG_SetSequencerChannels()".                             */
    /*       Case of STM32L0xx:                                               */
    /*       ADC Channel ADC_CHANNEL_VREFINT is on ADC channel 17,            */
    /*       there is 1 other channel enabled with lower channel number.      */
    /*       Therefore, ADC_CHANNEL_VREFINT will be converted by the          */
    /*       sequencer as the 2nd rank.                                       */
    /*       ADC Channel ADC_CHANNEL_TEMPSENSOR is on ADC channel 18,         */
    /*       there are 2 other channels enabled with lower channel number.    */
    /*       Therefore, ADC_CHANNEL_TEMPSENSOR will be converted by the       */
    /*       sequencer as the 3rd rank.                                       */
    /* Set ADC group regular sequencer discontinuous mode */
    // LL_ADC_REG_SetSequencerDiscont(ADC1, LL_ADC_REG_SEQ_DISCONT_DISABLE);
    
    /* Set ADC group regular sequence: channel on rank corresponding to       */
    /* channel number.                                                        */
    LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_4 | LL_ADC_CHANNEL_VREFINT | LL_ADC_CHANNEL_TEMPSENSOR);
	}
  
  
  /*## Configuration of ADC hierarchical scope: ADC group injected ###########*/
  
  /* Note: Feature not available on this STM32 serie */ 
  
  
  /*## Configuration of ADC hierarchical scope: channels #####################*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled or enabled without conversion on going        */
  /*       on either groups regular or injected.                              */
  if ((LL_ADC_IsEnabled(ADC1) == 0)               ||
      (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    /* Set ADC channels sampling time */
    /* Note: On this STM32 serie, sampling time is common to all channels     */
    /*       of the entire ADC instance.                                      */
    /*       See sampling time configured above, at ADC instance scope.       */
    
  }
  
  
  /*## Configuration of ADC transversal scope: analog watchdog ###############*/
  
  /* Note: On this STM32 serie, there is only 1 analog watchdog available.    */
  
  /* Set ADC analog watchdog: channels to be monitored */
  // LL_ADC_SetAnalogWDMonitChannels(ADC1, LL_ADC_AWD_DISABLE);
  
  /* Set ADC analog watchdog: thresholds */
  // LL_ADC_ConfigAnalogWDThresholds(ADC1, __LL_ADC_DIGITAL_SCALE(LL_ADC_RESOLUTION_12B), 0x000);
  
  
  /*## Configuration of ADC transversal scope: oversampling ##################*/
  
  /* Set ADC oversampling scope */
  // LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);
  
  /* Set ADC oversampling parameters */
  // LL_ADC_ConfigOverSamplingRatioShift(ADC1, LL_ADC_OVS_RATIO_2, LL_ADC_OVS_SHIFT_NONE);
  
  
  /*## Configuration of ADC interruptions ####################################*/
  /* Enable interruption ADC group regular end of sequence conversions */
  //LL_ADC_EnableIT_EOS(ADC1);
  
  /* Enable interruption ADC group regular overrun */
  //LL_ADC_EnableIT_OVR(ADC1);
  
  /* Note: in this example, ADC group regular end of conversions              */
  /*       (number of ADC conversions defined by DMA buffer size)             */
  /*       are notified by DMA transfer interruptions).                       */
  /*       ADC interruptions of end of conversion are enabled optionally,     */
  /*       as demonstration purpose in this example.                          */
  
}

void Activate_ADC1(void)
{
  __IO uint32_t wait_loop_index = 0;
  __IO uint32_t backup_setting_adc_dma_transfer = 0;
  #if (USE_TIMEOUT == 1)
  uint32_t Timeout = 0; /* Variable used for timeout management */
  #endif /* USE_TIMEOUT */
  
  /*## Operation on ADC hierarchical scope: ADC instance #####################*/
  
  /* Note: Hardware constraint (refer to description of the functions         */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of these features is conditioned to   */
  /*       ADC state:                                                         */
  /*       ADC must be disabled.                                              */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  if (LL_ADC_IsEnabled(ADC1) == 0)
  {   
		/* Disable ADC DMA transfer request during calibration */
    /* Note: Specificity of this STM32 serie: Calibration factor is           */
    /*       available in data register and also transfered by DMA.           */
    /*       To not insert ADC calibration factor among ADC conversion data   */
    /*       in array variable, DMA transfer must be disabled during          */
    /*       calibration.                                                     */
    backup_setting_adc_dma_transfer = LL_ADC_REG_GetDMATransfer(ADC1);
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_NONE);
    /* Run ADC self calibration */
    LL_ADC_StartCalibration(ADC1);
    
    /* Poll for ADC effectively calibrated */
    #if (USE_TIMEOUT == 1)
    Timeout = ADC_CALIBRATION_TIMEOUT_MS;
    #endif /* USE_TIMEOUT */
    
    while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
    {
    #if (USE_TIMEOUT == 1)
      /* Check Systick counter flag to decrement the time-out value */
      if (LL_SYSTICK_IsActiveCounterFlag())
      {
        if(Timeout-- == 0)
        {
        /* Time-out occurred. Set LED to blinking mode */
        LED_Blinking(LED_BLINK_ERROR);
        }
      }
    #endif /* USE_TIMEOUT */
    }
    
    /* Delay between ADC end of calibration and ADC enable.                   */
    /* Note: Variable divided by 2 to compensate partially                    */
    /*       CPU processing cycles (depends on compilation optimization).     */
    wait_loop_index = (ADC_DELAY_CALIB_ENABLE_CPU_CYCLES >> 1);
    while(wait_loop_index != 0)
    {
      wait_loop_index--;
    }
		/* Restore ADC DMA transfer request after calibration */
		LL_ADC_REG_SetDMATransfer(ADC1, backup_setting_adc_dma_transfer);

    /* Note: With ADC low power mode "LL_ADC_LP_AUTOWAIT_AUTOPOWEROFF" enabled, operation of   */
    /*       ADC enable is managed automatically by hardware.                 */    
//    /* Enable ADC */
//    LL_ADC_Enable(ADC1);
//		
//    /* Poll for ADC ready to convert */
//    #if (USE_TIMEOUT == 1)
//    Timeout = ADC_ENABLE_TIMEOUT_MS;
//    #endif /* USE_TIMEOUT */
//    
//    while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0)
//    {
//    #if (USE_TIMEOUT == 1)
//      /* Check Systick counter flag to decrement the time-out value */
//      if (LL_SYSTICK_IsActiveCounterFlag())
//      {
//        if(Timeout-- == 0)
//        {
//        /* Time-out occurred. Set LED to blinking mode */
//        LED_Blinking(LED_BLINK_ERROR);
//        }
//      }
//    #endif /* USE_TIMEOUT */
//    }
  }
}

/**
  * @brief  Perform ADC group regular conversion start, poll for conversion
  *         completion.
  *         (ADC instance: ADC1).
  * @note   This function does not perform ADC group regular conversion stop:
  *         intended to be used with ADC in single mode, trigger SW start
  *         (only 1 ADC conversion done at each trigger, no conversion stop
  *         needed).
  *         In case of continuous mode or conversion trigger set to 
  *         external trigger, ADC group regular conversion stop must be added.
  * @param  None
  * @retval None
  */
struct adc1_data ConversionStartPoll_ADC1_GrpRegular(void)
{
   #if (USE_TIMEOUT == 1)
  uint32_t Timeout = 0; /* Variable used for timeout management */
  #endif /* USE_TIMEOUT */
  
  /* Start ADC group regular conversion */
  /* Note: Hardware constraint (refer to description of the function          */
  /*       below):                                                            */
  /*       On this STM32 serie, setting of this feature is conditioned to     */
  /*       ADC state:                                                         */
  /*       ADC must be enabled without conversion on going on group regular,  */
  /*       without ADC disable command on going.                              */
  /* Note: In this example, all these checks are not necessary but are        */
  /*       implemented anyway to show the best practice usages                */
  /*       corresponding to reference manual procedure.                       */
  /*       Software can be optimized by removing some of these checks, if     */
  /*       they are not relevant considering previous settings and actions    */
  /*       in user application.                                               */
  /* Note: With ADC low power mode "auto power off" enabled, operation of     */
  /*       ADC enable is managed automatically by hardware.                   */
  /*       Check of ADC enable state using function "LL_ADC_IsEnabled()"      */
  /*       before performing ADC conversion start is not relevant.            */
  if ((LL_ADC_IsDisableOngoing(ADC1) == 0)        &&
      (LL_ADC_REG_IsConversionOngoing(ADC1) == 0)   )
  {
    LL_ADC_REG_StartConversion(ADC1);
  }
  else
  {
    /* Error: ADC conversion start could not be performed */
		
	}
  
/* 	When use DMA transfer data, following procedure is useless. */
//  #if (USE_TIMEOUT == 1)
//  Timeout = ADC_UNITARY_CONVERSION_TIMEOUT_MS;
//  #endif /* USE_TIMEOUT */
//  
//  while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0)
//  {
//  #if (USE_TIMEOUT == 1)
//    /* Check Systick counter flag to decrement the time-out value */
//    if (LL_SYSTICK_IsActiveCounterFlag())
//    {
//      if(Timeout-- == 0)
//      {
//      /* Time-out occurred. Set LED to blinking mode */
//      LED_Blinking(LED_BLINK_SLOW);
//      }
//    }
//  #endif /* USE_TIMEOUT */
//  }
//  LL_ADC_ClearFlag_EOC(ADC1);

	/* Read relavent data from DMA */
	struct adc1_data calc_ad_data = {0};
	while (itDmaTransferStatus == 0){
  #if (USE_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (LL_SYSTICK_IsActiveCounterFlag())
    {
      if(Timeout-- == 0)
      {
      /* Time-out occurred. Set LED to blinking mode */
		
      }
    }
  #endif /* USE_TIMEOUT */
  }
	
	if(itDmaTransferStatus == 1){	 
		calc_ad_data.vref = __LL_ADC_CALC_VREFANALOG_VOLTAGE(aADCxConvertedData[1], LL_ADC_RESOLUTION_12B);
		calc_ad_data.chn4 = __LL_ADC_CALC_DATA_TO_VOLTAGE(calc_ad_data.vref, aADCxConvertedData[0], LL_ADC_RESOLUTION_12B);
		calc_ad_data.temp = __LL_ADC_CALC_TEMPERATURE(calc_ad_data.vref, aADCxConvertedData[2], LL_ADC_RESOLUTION_12B);  	
		itDmaTransferStatus=0;
	}

	return calc_ad_data;
}
