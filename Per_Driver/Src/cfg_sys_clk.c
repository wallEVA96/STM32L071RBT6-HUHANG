/**
  ******************************************************************************
  * @file    led configure
  * @author  wallea hu
  * @brief   
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
 #include "cfg_sys_clk.h"

 /* Define  ------------------------------------------------------------------*/
 #define RTC_ERROR_NONE    0
#define RTC_ERROR_TIMEOUT 1
/* ck_apre=LSEFreq/(ASYNC prediv + 1) = 256Hz with LSEFreq=32768Hz */
#define RTC_ASYNCH_PREDIV          ((uint32_t)0x7F)
/* ck_spre=ck_apre/(SYNC prediv + 1) = 1 Hz */
#define RTC_SYNCH_PREDIV           ((uint32_t)0x00FF)
#define RTC_BKP_DATE_TIME_UPDTATED ((uint32_t)0x32F2)


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = HSE
  *            SYSCLK(Hz)                     = 8Mhz
  *            HCLK(Hz)                       = 8Mhz
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 0
  *            Main regulator output voltage  = Scale3 mode
  * @retval None
  */
void SystemClock_Config(void)
{
  /* HSE configuration and activation */
  LL_RCC_PLL_Disable();
  /* Set new latency */
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  LL_RCC_HSE_Enable();
  while(LL_RCC_HSE_IsReady() != 1) 
  {
  };

  /* Sysclk activation  */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE) 
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 2MHz */
	/* @attention: Disable PLL and DIV */
  LL_Init1msTick(HSE_VALUE); 

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(HSE_VALUE);  

  /* Enable Power Control clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency.
		 To update the voltage scaling value regarding system frequency refer to product datasheet.  */
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
  /* Ensure that HSE is wake-up system clock */ 
	LL_RCC_SetClkAfterWakeFromStop(LL_RCC_STOP_WAKEUPCLOCK_MSI);
}

/**
  * @brief  USE FOR AVOID TO TIMEOUT IN LOOP FUNCTION.
  * @param  
  * @param  
  * @retval None
  */
unsigned int GLOBAL_LOOP_TIME_OUT_VAL = 2; //GLOBAL_LOOP_TIME_OUT_VAL should be set more than 2.
char LOOP_IS_TIME_OUT_xMS(void)
{
  /* Infinite loop */
	if(LL_SYSTICK_IsActiveCounterFlag())
		GLOBAL_LOOP_TIME_OUT_VAL--;
	return (GLOBAL_LOOP_TIME_OUT_VAL==0)?1:0;
}

/**
  * @brief  Wait until the RTC Time and Date registers (RTC_TR and RTC_DR) are
  *         synchronized with RTC APB clock.
  * @param  None
  * @retval RTC_ERROR_NONE if no error (RTC_ERROR_TIMEOUT will occur if RTC is 
  *         not synchronized)
  */
uint32_t WaitForSynchro_RTC(void)
{
  /* Clear RSF flag */
  LL_RTC_ClearFlag_RS(RTC);

  /* Wait the registers to be synchronised */
  while(LL_RTC_IsActiveFlag_RS(RTC) != 1)
  {

  }
  return RTC_ERROR_NONE;
}


/**
  * @brief  Enter in initialization mode
  * @note In this mode, the calendar counter is stopped and its value can be updated
  * @param  None
  * @retval RTC_ERROR_NONE if no error
  */
uint32_t Enter_RTC_InitMode(void)
{
  /* Set Initialization mode */
  LL_RTC_EnableInitMode(RTC);

  /* Check if the Initialization mode is set */
  while (LL_RTC_IsActiveFlag_INIT(RTC) != 1)
  {
		
  } 
  return RTC_ERROR_NONE;
}

/**
  * @brief  Exit Initialization mode 
  * @param  None
  * @retval RTC_ERROR_NONE if no error
  */
uint32_t Exit_RTC_InitMode(void)
{
  LL_RTC_DisableInitMode(RTC);
  
  /* Wait for synchro */
  /* Note: Needed only if Shadow registers is enabled           */
  /*       LL_RTC_IsShadowRegBypassEnabled function can be used */
  return (WaitForSynchro_RTC());
}

/**
  * @brief  Configure RTC
  * @param  void
  * @retval None
  */
void Configure_RTC(int wu_sec)
{
  /*##-1- Enables the PWR Clock and Enables access to the backup domain #######*/
  /* To change the source clock of the RTC feature (LSE, LSI), you have to:
     - Enable the power clock
     - Enable write access to configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain
     - Configure the needed RTC clock source */
  LL_PWR_EnableBkUpAccess();
  
  /*##-2- Configure LSE as RTC clock source ###############################*/
  /* Enable LSE only if disabled.*/
 if (LL_RCC_LSE_IsReady() == 0)
  {
		LL_RCC_ForceBackupDomainReset();
		LL_RCC_ReleaseBackupDomainReset();
		LL_RCC_LSE_Enable();

		while (LL_RCC_LSE_IsReady() != 1)
		{

		}
	}
	/* Choose LSE to */
	LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
	
	/*##-3- Enable RTC peripheral Clocks #######################################*/
	/* Enable RTC Clock */ 
	LL_RCC_EnableRTC();

  /*##-4- Disable RTC registers write protection ##############################*/
  LL_RTC_DisableWriteProtection(RTC);

  /*##-5- Enter in initialization mode #######################################*/
  if (Enter_RTC_InitMode() != RTC_ERROR_NONE)   
  {
			printf("RTC enter init error\r");
  }

  /*##-6- Configure RTC ######################################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* Set Hour Format */
  LL_RTC_SetHourFormat(RTC, LL_RTC_HOURFORMAT_24HOUR);
  /* Set Asynch Prediv (value according to source clock) */
  LL_RTC_SetAsynchPrescaler(RTC, RTC_ASYNCH_PREDIV);
  /* Set Synch Prediv (value according to source clock) */
  LL_RTC_SetSynchPrescaler(RTC, RTC_SYNCH_PREDIV);
  /* Set OutPut */
  /* Reset value is LL_RTC_ALARMOUT_DISABLE */
  //LL_RTC_SetAlarmOutEvent(RTC, LL_RTC_ALARMOUT_DISABLE);
  /* Set OutPutPolarity */
  /* Reset value is LL_RTC_OUTPUTPOLARITY_PIN_HIGH */
  //LL_RTC_SetOutputPolarity(RTC, LL_RTC_OUTPUTPOLARITY_PIN_HIGH);
  /* Set OutPutType */
  /* Reset value is LL_RTC_ALARM_OUTPUTTYPE_OPENDRAIN */
  //LL_RTC_SetAlarmOutputType(RTC, LL_RTC_ALARM_OUTPUTTYPE_OPENDRAIN);

  /*##-7- Exit of initialization mode #######################################*/
  Exit_RTC_InitMode();
  
	/* Disable wake up timer to modify it */
  LL_RTC_WAKEUP_Disable(RTC);
  
  /* Wait until it is allow to modify wake up reload value */
  while (LL_RTC_IsActiveFlag_WUTW(RTC) != 1)
  {
		
  }
  /* Setting the Wakeup time to 1 s
       If LL_RTC_WAKEUPCLOCK_CKSPRE is selected, the frequency is 1Hz, 
       this allows to get a wakeup time equal to 1 s if the counter is 0x0 */
  LL_RTC_WAKEUP_SetAutoReload(RTC, wu_sec);
  LL_RTC_WAKEUP_SetClock(RTC, LL_RTC_WAKEUPCLOCK_CKSPRE);
  
  /* Enable wake up counter and wake up interrupt */
  LL_RTC_WAKEUP_Enable(RTC);
	LL_RTC_EnableIT_WUT(RTC);
	
  /*##-8- Enable RTC registers write protection #############################*/
  LL_RTC_EnableWriteProtection(RTC);  
  
  /* Configure exti and nvic for RTC IT */
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_20);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_20);
  NVIC_SetPriority(RTC_IRQn, 0);
  NVIC_EnableIRQ(RTC_IRQn);
}

/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void Configure_RTC_Calendar(void)
{
  /*##-1- Disable RTC registers write protection ############################*/
  LL_RTC_DisableWriteProtection(RTC);

  /*##-2- Enter in initialization mode ######################################*/
  if (Enter_RTC_InitMode() != RTC_ERROR_NONE)   
  {
		printf("RTC enter init error\r");
  }

  /*##-3- Configure the Date ################################################*/
  /* Set Date: Sunday May 16th 2021 */
  LL_RTC_DATE_Config(RTC, LL_RTC_WEEKDAY_SUNDAY, 0x16, LL_RTC_MONTH_MAY, 0x21);
  //LL_RTC_DATE_Config(RTC, LL_RTC_WEEKDAY_MONDAY, 0x31, LL_RTC_MONTH_MARCH, 0x15);

  /*##-4- Configure the Time ################################################*/
	/* Set Time: 20:22:00 PM*/
  LL_RTC_TIME_Config(RTC, LL_RTC_TIME_FORMAT_AM_OR_24, 0x21, 0x17, 0x00);
  
  /*##-5- Exit of initialization mode #######################################*/
  if (Exit_RTC_InitMode() != RTC_ERROR_NONE)   
  {
		printf("RTC exit init error\r");
  }
   
  /*##-6- Enable RTC registers write protection #############################*/
  LL_RTC_EnableWriteProtection(RTC);

  /*##-8- Writes a data in a RTC Backup data Register1 #######################*/
  LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR1, RTC_BKP_DATE_TIME_UPDTATED);
}

/**
  * @brief  This function configures IWDG
  * @param  None
  * @retval None
  */
void Configure_IWDG(int ww_sec)
{
  /* Enable the peripheral clock of DBG register (uncomment for debug purpose) */
  /* ------------------------------------------------------------------------- */
  /*  LL_DBGMCU_APB1_GRP1_FreezePeriph(LL_DBGMCU_APB1_GRP1_IWDG_STOP); */
  
  /* Enable the peripheral clock IWDG */
  /* -------------------------------- */
  LL_RCC_LSI_Enable();
  while (LL_RCC_LSI_IsReady() != 1)
  {
  }

  /* Configure the IWDG with window option disabled */
  /* ------------------------------------------------------- */
  /* (1) Enable the IWDG by writing 0x0000 CCCC in the IWDG_KR register */
  /* (2) Enable register access by writing 0x0000 5555 in the IWDG_KR register */
  /* (3) Write the IWDG prescaler by programming IWDG_PR from 0 to 7 - LL_IWDG_PRESCALER_4 (0) is lowest divider*/
  /* (4) Write the reload register (IWDG_RLR) */
  /* (5) Wait for the registers to be updated (IWDG_SR = 0x0000 0000) */
  /* (6) Refresh the counter value with IWDG_RLR (IWDG_KR = 0x0000 AAAA) */
  LL_IWDG_Enable(IWDG);                             /* (1) */
  LL_IWDG_EnableWriteAccess(IWDG);                  /* (2) */
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_256);  /* (3) */
  LL_IWDG_SetReloadCounter(IWDG, ww_sec*0x7D);            /* (4) */
  while (LL_IWDG_IsReady(IWDG) != 1)                /* (5) */
  {
  }
  LL_IWDG_ReloadCounter(IWDG);                      /* (6) */
}

/**
  * @brief  This function check if the system has resumed from IWDG reset
  * @param  None
  * @retval None
  */
void Check_IWDG_Reset(void)
{
  if (LL_RCC_IsActiveFlag_IWDGRST())
  {
    /* clear IWDG reset flag */
    LL_RCC_ClearResetFlags();

    /* do something here */
  	printf("ERROR: IWDG happen reset.\r\n");
  }
}
