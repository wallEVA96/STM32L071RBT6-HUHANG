/**
  ******************************************************************************
  * @file    init hmcl5883 and read value.
  * @author  wallea hu
  * @brief   USART LL module driver.
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "cfg_uartx.h"
#include "apply_bc.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define CGPADDR "AT+CGPADDR\r"
#define NSOCR "AT+NSOCR=DGRAM,17,6969,1\r"
#define NSOCL "AT+NSOCL=1\r"

/* Private variables ---------------------------------------------------------*/
uint8_t global_bcxx_response[BC_RES_LEN] = {0};
uint8_t global_bcxx_res_index = 0;

/* Private function  -----------------------------------------------*/
/**
  * @brief  clear response about usart.
  * @param  None
  * @retval None
  */
void Clear_Response_After_Use(void)
{
	global_bcxx_res_index = 0;
	memset(global_bcxx_response, 0, sizeof(global_bcxx_response));
}
/**
  * @brief  init bc rf
  * @param  None
  * @retval None
  */
void BCxx_UDP_Init(USART_TypeDef *USARTx)
{
	uint8_t at_cmd[15] = CGPADDR;
	while(1)
	{
		/*make sure that network is steady.*/
		Buffer_Transfer_USARTx(USARTx, at_cmd, sizeof(at_cmd));
		while(!LL_USART_IsActiveFlag_RXNE(USARTx)) {}
		LL_mDelay(500);
		printf("BCXX Local Ip:%s\r\n", global_bcxx_response);
		if(strstr((const char*)global_bcxx_response, "ADDR") != NULL ){
			Clear_Response_After_Use();
			break;
		}
		else
			Clear_Response_After_Use();
	}
}
/**
  * @brief  create udp socket 
  * @param  None
  * @retval None
  */
char BCxx_UDP_Create_Socket(USART_TypeDef *USARTx)
{
	uint8_t at_cmd[30] = NSOCR;
	do {
		Buffer_Transfer_USARTx(USARTx, (uint8_t *)NSOCL, sizeof(NSOCL));
		while(!LL_USART_IsActiveFlag_RXNE(USARTx)) {}
		LL_mDelay(500);
		printf("BCXX Close Socket: %s\r\n", global_bcxx_response);  
		Clear_Response_After_Use();

		Buffer_Transfer_USARTx(USARTx, at_cmd, sizeof(at_cmd));
		while(!LL_USART_IsActiveFlag_RXNE(USARTx)) {}
		LL_mDelay(500);
		printf("BCXX Create Socket: %s\r\n", global_bcxx_response);  
		Clear_Response_After_Use();
	} while(0);
}

/**
  * @brief  create udp socket 
  * @param  None
  * @retval None
  */
char BCxx_UDP_Send(USART_TypeDef *USARTx, int32_t tempe)
{
	char at_cmd[40] = {0};
	sprintf(at_cmd, "AT+NSOST=1,121.36.65.9,9696,1,%2X,1\r\n", (uint8_t)tempe);
	Buffer_Transfer_USARTx(USARTx, (uint8_t *)at_cmd, sizeof(at_cmd));
	while(!LL_USART_IsActiveFlag_RXNE(USARTx)) {
			/*often happen receive nothing */
	}
	LL_mDelay(500);
	printf("BCXX Socket Send: %s\r\n", global_bcxx_response);
	Clear_Response_After_Use();
}

/************************ (C) COPYRIGHT wallea Hu *****END OF FILE****/
