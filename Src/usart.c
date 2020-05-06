/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "KL3333.H"
#include "main.h"
volatile char sim;
volatile char sim2;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_8;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_DMADISABLEONERROR_INIT|UART_ADVFEATURE_AUTOBAUDRATE_INIT;
  huart1.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  huart1.AdvancedInit.AutoBaudRateEnable = UART_ADVFEATURE_AUTOBAUDRATE_ENABLE;
  huart1.AdvancedInit.AutoBaudRateMode = UART_ADVFEATURE_AUTOBAUDRATE_ONFALLINGEDGE;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */
 //   READ_REG(huart1.Instance->RDR);
  //  SET_BIT(huart1.Instance->CR1, USART_CR1_RXNEIE);
  //  SET_BIT(huart1.Instance->CR3, USART_CR3_EIE);


  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
// USART Receiver buffer
#define RX_BUFFER_SIZE 300
static  uint8_t rx_buffer[RX_BUFFER_SIZE];

static uint16_t rx_wr_index=0,rx_rd_index=0;
static uint16_t rx_counter=0;

#define TX_BUFFER_SIZE 100
static  uint8_t tx_buffer[TX_BUFFER_SIZE];

static uint16_t tx_wr_index=0,tx_rd_index=0;
static uint16_t tx_counter=0;

void USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_8;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_DMADISABLEONERROR_INIT;
  huart1.AdvancedInit.DMADisableonRxError = UART_ADVFEATURE_DMA_DISABLEONRXERROR;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void USART1_115200_ReInit(void)
{

 HAL_UART_DeInit(&huart1);
// HAL_UART_DeInit(&huart2);
 HAL_UART_Init(&huart1);
 MX_USART1_UART_Init();
// MX_USART2_UART_Init();
 HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
 HAL_NVIC_EnableIRQ(USART1_IRQn);
// HAL_UART_Receive_IT(&huart1, (uint8_t*)&sim, 1);
 ClearRXBuffer();
 StartRecieve();
}


void put_char(unsigned char c)
{
    
   while(HAL_UART_Transmit_IT(&huart1, &c, 1)==HAL_BUSY);
 //  while(HAL_UART_Transmit_IT(&huart2, &c, 1)==HAL_BUSY);
   
}

uint8_t get_char(uint8_t * data)
{
if (rx_counter)
{
	__disable_irq ();
	*data=rx_buffer[rx_rd_index];
	if (++rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
    --rx_counter;
    __enable_irq ();
    return 1;
}
else
return 0;
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  if (huart->Instance== USART1)
  {	
		rx_counter++;
		rx_buffer[rx_wr_index]=sim;
		if (tx_counter)
		{
			tx_counter++;
			tx_buffer[tx_wr_index]=sim;
			if (++tx_wr_index == TX_BUFFER_SIZE) tx_wr_index=0;
		}
		else
			HAL_UART_Transmit_IT(&huart2, (uint8_t*)&sim,1);

		NMEA_CHECK(rx_buffer[rx_wr_index]);     //put_char1(rx_buffer[rx_wr_index]);
		if (++rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;

		HAL_UART_Receive_IT(&huart1, (uint8_t*)&sim, 1);


  }      
  if (huart->Instance== USART2)
   {

 		HAL_UART_Transmit_IT(&huart2, (uint8_t*)&sim2,1);
 		HAL_UART_Receive_IT(&huart2, (uint8_t*)&sim2, 1);


   }

}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    
      

	if (huart->ErrorCode ==HAL_UART_ERROR_ORE)
	{
		READ_REG(huart->Instance->RDR);
		HAL_UART_Receive_IT(&huart1, (uint8_t*)&sim, 1);

	}

}


 void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

  if (huart->Instance== USART2)
  {
	  	  if (tx_counter)
	  	  {
	  		HAL_UART_Transmit_IT(&huart2, &tx_buffer[tx_rd_index],1);
	  		if (++tx_rd_index == TX_BUFFER_SIZE) tx_rd_index=0;
	  		tx_counter--;
	  	  }


  }
}


void ClearRXBuffer()
{
  rx_counter=0;
  rx_wr_index=0;
  rx_rd_index=0;
  tx_counter=0;
  tx_wr_index=0;
  tx_rd_index=0;
  CLEAR_NMEA_MASSEGE_BUFFER();
}


void StartRecieve()
{
	  HAL_UART_Receive_IT(&huart1, (uint8_t*)&sim, 1);
	//  HAL_UART_Receive_IT(&huart2, (uint8_t*)&sim2, 1);
}

void put_str_pak(unsigned char *s, unsigned int c)
{    

	while(HAL_UART_Transmit_IT(&huart1,s,c)==HAL_BUSY);
	//while(HAL_UART_Transmit_IT(&huart2,s,c)==HAL_BUSY);
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
