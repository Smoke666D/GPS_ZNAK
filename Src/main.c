/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
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
#include "cmsis_os.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "iwdg.h"
#include "KL3333.H"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 #define B_br  68 //60
 #define B_br0  0
 #define W_br  33 //60
 #define W_br0 33 //15
     

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned char counter_PPS=0,
              Tic_PPS=2;
static unsigned char smenaPPS=0,
  Temp_bit=0,
   Mig_ON_nOFF=0;
   B_ON=0,
   PPS_OK=0,
   PPS_stat = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void vmainPPSSet()
{

	counter_PPS++;
	PPS_stat = 1;
	if (GetB_ON())
	{
	   counter_PPS=10;
	   B_ON=0;
    }
	if( counter_PPS >= Tic_PPS)
	{
	   counter_PPS=0;
	   SetSmenaPPS();
	   PPS_OK=1;
    }
}

unsigned char GetTic_PPS()
{
	return Tic_PPS;
}
unsigned char GetB_ON()
{
	return B_ON;
}

void SetB_ON()
{
	B_ON=1;

}
void SetSmenaPPS()
{
	smenaPPS=1;
}




void vManiTsk()
{
	SetPWM4(100);
    SetPWM3(0);
	osDelay(1000);
	SetPWM4(0);
	SetPWM3(100);
	osDelay(1000);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    unsigned char STATE=0;
	  unsigned char _jarcostjB_mas[14]= {0,0,0,0,0,0,0,    B_br,B_br,B_br,B_br,B_br,B_br,B_br};   // REG_3
	  unsigned char _jarcostjW_mas[14]= {W_br,W_br,W_br,W_br,W_br,W_br,W_br,          W_br0,W_br0,W_br0,W_br0,W_br0,W_br0,W_br0};   // REG_6
	  unsigned char temp_mcusr,step;
	  unsigned int Temp_T1,Temp_T2;
	  unsigned char Time_ON=0;

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  SetPWM4(0);
  SetPWM3(0);

   //RestWDT();

  Tic_PPS=2;                         // ������ ������������ ������ PPS
  Temp_T2=(unsigned int)120*Tic_PPS; // ���������� ������ �������,�� ���������, ����� ��������� ������� PPS ������
                                     // ������� �������� � ���-�� PPS ����������� Tic_PPS
  Temp_T1=(unsigned int)25*Tic_PPS;  // ���������� ����� �������
  Mig_ON_nOFF=1;                     // ������������� ���� ����� ������������� ���������
  Temp_bit=1;
  // �������� ������� ������������� ��������� ����� �� �����
  counter_PPS=0;                     // ����� �������� ��������� PPS
  smenaPPS=0;                        // ����� ����� ��������� ������� �� PPS
  PPS_OK=0;                          // ����� ����� ���������� ��������� ��������� ������� �� PPS
  B_ON=0;                            // ����� ����� ������� �������������



  //while(1);
  if (Tic_PPS==1)                    // �������� �������� �������� ��������� ������������� ���������
  {
   temp_mcusr=1;           // temp_mcusr - ���������� � ����� ���������(���������) �������� ������� ��������
                           // ��� ��������� � ������� �����������
  }
  else{temp_mcusr=2;};     // 2-3 ������������ ��. ��� 1 ��� ��� ������� ��������. ������ 3� ������ ����� �����.
  Time_ON=1;               // ???
  step=0;                  // ������������� ������������� 0-��� ����� �������
  SetTimer4ms(0);              // ���������� ����� � ������ �������
  SetTimer4msl(0);            // ���������� ����� � ������� ����������� ����������� �� ������� ��������������

  /* USER CODE END 2 */
 
  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init(); 
 
  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  //  RestWDT();
    if(smenaPPS)          // ������ TIC_PPS ��������� PPS
      {
        Mig_ON_nOFF=0;      // ����� ����� �� ������� � ������� ����� �������
        smenaPPS=0;         // ����� �����
        SetTimer4ms(Temp_T2+1); // ����� ������� ���������� �� ��������� �������
      }
      //*********************************************************************
      else
      {
      }
      //*********************************************************************

      if (Mig_ON_nOFF==0 && Temp_T1 == GetTimer4ms() )// && Temp_T2 >= time_4ms  // ������� ��������� �������
      {
        Mig_ON_nOFF=1;   // ���������� ��������� ������������� ���������
        Temp_bit=0;      // �������� ������� ������������� ��������� ������ �� ����
        Time_ON=1;       // ???
        SetTimer4msl(0);   // ����� ���������� �������
      };

      if (Mig_ON_nOFF==0 && Temp_T2 < GetTimer4ms() ) // ������� ��������� �������
      {
        Mig_ON_nOFF=1;   // ���������� ��������� ������������� ���������
        Temp_bit=1;      // �������� ������� ������������� ��������� ����� �� �����
        Time_ON=1;       // ???
        SetTimer4msl(0);    // ����� ���������� �������
        SetTimer4ms(0);     // ����� ������� ������� �� ����� ������
        if(!PPS_stat) PPS_OK = 0;
        PPS_stat = 0;
      };   //!!!putchar0(Temp_Ltime);


      if(Mig_ON_nOFF) // ���������� ��������� ������������� ���������
      {
        if(Temp_bit)  // �������� ������� ������������� ��������� ����� �� �����
        {
          if(GetTimer4msl(0)>=temp_mcusr) // ��������� ����� ��������� �������� ��������� �������������
          {
        	SetTimer4msl(0);      // ����� ���������� �������
            if(Time_ON)        // ???      (���� �������� �� ���� ��������� � +�� � � ��� ������ ��)
            {
              if(step <= 13)   // ������� ���� � ��������
              {
                  SetPWM4(_jarcostjB_mas[step]);  // �������� �������������                 
                  SetPWM3(_jarcostjW_mas[step]);  // �������� �������������             
                  step++;
              }
              else             // ������� ���� ��������
              {
                step=0;        // ����� � ������ �������
                Time_ON=0;     // ???
                Mig_ON_nOFF=0; // ����� ������������� ��������� ������������� ��������
              };


            };
          };
        }
        else  // �������� ������� ������������� ��������� ������ �� ����
        {
          if(GetTimer4msl(0)>=temp_mcusr)
          {
        	SetTimer4msl(0);      // ����� ���������� �������
            if(Time_ON)        // ???
            {
              if(step <= 13)   // ������� ���� � ��������
              {
            	SetPWM4(_jarcostjB_mas[13-step]);  // �������� �������������            	
            	SetPWM3(_jarcostjW_mas[13-step]);  // �������� �������������          
                step++;
              }
              else             // ������� ���� ��������
              {
                step=0;        // ����� � ������ �������
                Time_ON=0;     // ???
                Mig_ON_nOFF=0; // ����� ������������� ��������� ������������� ��������
              };
            };
          };
        };
      };

          
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
 // }*/
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* EXTI2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
  /* TIM3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM3_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
