/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "stm32f0xx_hal_gpio.h"
    #include "stm32f0xx_hal_def.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
unsigned int time_10ms=0;
unsigned int time_05s=0;
unsigned int time_4ms;
unsigned int TimeForPPS;
unsigned char Ltime_4msl;
unsigned char DIN_1,DIN_2,DIN_3;
unsigned char Time_50HZ_1=0,Time_50HZ_2=0,Time_50HZ_3=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
unsigned int Time_2_2KHz_1;
unsigned int Time_2_2KHz_2;
unsigned int Time_2_2KHz_3;
unsigned char DIN1_RISE=1;
unsigned char DIN2_RISE=1;
unsigned char DIN3_RISE=1;


/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DAC_HandleTypeDef hdac1;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */
unsigned char GetDIN_1()
{
	return DIN_1;
}
unsigned char GetDIN_2()
{
	return DIN_2;
}
unsigned char GetDIN_3()
{
	return DIN_3;
}
void SetDIN_1(unsigned char data)
{
	DIN_1 =data;
}
void SetDIN_2(unsigned char data)
{
	DIN_2 =data;
}
void SetDIN_3(unsigned char data)
{
	DIN_3 =data;
}
unsigned int GetTimer4ms()
{
	return time_4ms;
}
void SetTimer4ms(unsigned int data )
{
	 time_4ms = data;
}
unsigned int GetTimer4msl()
{
	return Ltime_4msl;
}
void SetTimer4msl(unsigned int data )
{
	 Ltime_4msl = data;
}
unsigned char Get0_5SecCounter()
{
	return time_05s;
}
void ResetSecCounter()
{
	time_05s =0;
}
unsigned int GetTime_2_2KHz_1()
{
  return Time_2_2KHz_1;
}
unsigned int GetTime_2_2KHz_2()
{
  return Time_2_2KHz_2;
}
unsigned int GetTime_2_2KHz_3()
{
  return Time_2_2KHz_3;
}



/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line 2 and 3 interrupts.
  */
void EXTI2_3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_3_IRQn 0 */
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET)
	  {
		  Ltime_4msl=0;
		  Inccounter_PPS();
		  SetPPS_stat();
		  if (GetB_ON())
		  {
			  Setcounter_PPS(10);
			  ResetB_ON();
		  }
		  if( Getcounter_PPS() >= GetTic_PPS())
		  {
		      Setcounter_PPS(0);
		      SetSmenaPPS();
		      SetPPS_OK();
		  };
            __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);

	  }
          
  /* USER CODE END EXTI2_3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI2_3_IRQn 1 */

  /* USER CODE END EXTI2_3_IRQn 1 */
}

/**
  * @brief This function handles EXTI line 4 to 15 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI4_15_IRQn 0 */
  /* Обработка прирывания по приходу сигнала PPS от модуля GPS
   *
   *
   */

  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET)
  {
	   if (DIN1_RISE==0)   //Если прирывание по falling edge
            {
                DIN1_RISE = 1;
                EXTI->FTSR|=GPIO_PIN_8;
                EXTI->RTSR&=~GPIO_PIN_8;                
                Time_2_2KHz_1 = 0;       //Сброс счетчика
                
                
          }              
          else
          {
              DIN1_RISE = 0;   
              EXTI->IMR &= (~GPIO_PIN_8);              
              EXTI->RTSR|=GPIO_PIN_8;
              EXTI->FTSR&=~GPIO_PIN_8;
              EXTI->IMR |= GPIO_PIN_8;
              if (Time_2_2KHz_1 >= VEdge  ) 
                DIN_1 = 1;
              if (Time_2_2KHz_1 <= VEdgel   )
                DIN_1 = 0;   
        }

          __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
  }
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12) != RESET)
  {
	  if (DIN2_RISE==0)   //Если прирывание по falling edge
          {
                DIN2_RISE = 1;
                EXTI->FTSR|=GPIO_PIN_12;
                EXTI->RTSR&=~GPIO_PIN_12;                
                Time_2_2KHz_2 = 0;       //Сброс счетчика
                
                
          }              
          else
          {
              DIN2_RISE = 0;   
              EXTI->IMR &= (~GPIO_PIN_12);              
              EXTI->RTSR|=GPIO_PIN_12;
              EXTI->FTSR&=~GPIO_PIN_12;
              EXTI->IMR |= GPIO_PIN_12;
              if (Time_2_2KHz_2 >= VEdge  ) DIN_2 = 1;
              if (Time_2_2KHz_2 <= VEdgel   ) DIN_2 = 0;   
        }
          __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
  }
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15) != RESET)
  {
	   if (DIN1_RISE==3)   //Если прирывание по falling edge
            {
                DIN3_RISE = 1;
                EXTI->FTSR|=GPIO_PIN_15;
                EXTI->RTSR&=~GPIO_PIN_15;                
                Time_2_2KHz_3 = 0;       //Сброс счетчика
                
                
          }              
          else
          {
              DIN3_RISE = 0;   
              EXTI->IMR &= (~GPIO_PIN_15);              
              EXTI->RTSR|=GPIO_PIN_15;
              EXTI->FTSR&=~GPIO_PIN_15;
              EXTI->IMR |= GPIO_PIN_15;
              if (Time_2_2KHz_3 >= VEdge  ) DIN_3 = 1;
              if (Time_2_2KHz_3 <= VEdgel   ) DIN_3 = 0;   
        }
          __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
  }
  if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET)
  {

  }


  /* USER CODE END EXTI4_15_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
  /* USER CODE BEGIN EXTI4_15_IRQn 1 */

  /* USER CODE END EXTI4_15_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
  
   time_10ms++;
   if (time_10ms == 50 )
   {
		  time_10ms=0;
		  time_05s++;
	  }
   time_4ms++;
   Ltime_4msl++;
   TimeForPPS++;   
  htim3.Instance->SR &=0xFE;
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global and DAC underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
	/*  time_10ms++;
	  if (time_10ms == 125 )
	  {
		  time_10ms=0;
		  time_05s++;
	  }
	  time_4ms++;
	  Ltime_4msl++;
	  TimeForPPS++;*/
	  Time_2_2KHz_1++;
          Time_2_2KHz_2++;
          Time_2_2KHz_3++;

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  HAL_DAC_IRQHandler(&hdac1);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles I2C1 event global interrupt / I2C1 wake-up interrupt through EXTI line 23.
  */
void I2C1_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_IRQn 0 */

  /* USER CODE END I2C1_IRQn 0 */
  if (hi2c1.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
    HAL_I2C_ER_IRQHandler(&hi2c1);
  } else {
    HAL_I2C_EV_IRQHandler(&hi2c1);
  }
  /* USER CODE BEGIN I2C1_IRQn 1 */

  /* USER CODE END I2C1_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

//	USART1Inter();
	// return;
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
