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
#include "adc.h"
#include "dac.h"
#include "i2c.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "iwdg.h"
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
unsigned char smenaPPS=0,
    Temp_bit=0,
    Mig_ON_nOFF=0,
    B_ON=0,
    PPS_OK=0,
    PPS_stat = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
unsigned char GetTic_PPS()
{
	return Tic_PPS;
}
unsigned char GetB_ON()
{
	return B_ON;
}
void ResetB_ON()
{
	B_ON=0;
}
void SetSmenaPPS()
{
	smenaPPS=1;
}
void Inccounter_PPS()
{
	counter_PPS++;
}
unsigned char Getcounter_PPS()
{
	return counter_PPS;
}
void Setcounter_PPS(unsigned char data)
{
	counter_PPS = data;
}
void SetPPS_OK()
{
	PPS_OK=1;
}
void SetPPS_stat()
{
	PPS_stat = 1;
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
	 unsigned char _jarcostj1_mas[14]={0,0,0,0,0,0,0,0,0,0,0,0,0, 0};   // REG_1
	  unsigned char _jarcostj2_mas[14]={0,0,0,0,0,0,0,0,0,0,0,0,0, 0};   // REG_2
	  unsigned char _jarcostjB_mas[14]= {0,0,0,0,0,0,0,    B_br,B_br,B_br,B_br,B_br,B_br,B_br};   // REG_3
	  unsigned char _jarcostjW_mas[14]= {W_br,W_br,W_br,W_br,W_br,W_br,W_br,          W_br0,W_br0,W_br0,W_br0,W_br0,W_br0,W_br0};   // REG_6
	  unsigned char  message_buffer[200];
	  unsigned char temp_mcusr,step;
	  unsigned int Temp_T1,Temp_T2;
	  unsigned char Time_ON=0;
	  float  GroundSpeed = 100;
	  GPS_TIME system_time;
	  double CurLat, CurLong = 0;
          unsigned char GPS_STATE=2;
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
  MX_ADC_Init();
  MX_DAC1_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_TIM2_Init();
  MX_IWDG_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
SetPWM4(0);
SetPWM3(0);
SetPWM2(0);
SetPWM1(0);

   RestWDT();
  GPS_STATE= InitKL3333();
  Tic_PPS=2;                         // Период срабатывания семены PPS
  Temp_T2=(unsigned int)120*Tic_PPS; // Определяет период мигания,по умолчанию, после появления сигнала PPS период
                                     // мигания привязан к кол-ву PPS задаваемому Tic_PPS
  Temp_T1=(unsigned int)25*Tic_PPS;  // Определяет время горения
  Mig_ON_nOFF=1;                     // Устанавливаем флаг смены интенсивности излучения
  Temp_bit=1;
  // Проходим массивы интенсивности излучения слево на право
  counter_PPS=0;                     // Сброс счетчика импульсов PPS
  smenaPPS=0;                        // Сброс флага интервала времени по PPS
  PPS_OK=0;                          // Сброс флага успешности измерения интервала времени по PPS
  B_ON=0;                            // Сброс флага фазовой синхронизации



  //while(1);
  if (Tic_PPS==1)                    // Выбираем значение скорости изменения интенсивности излучения
  {
   temp_mcusr=1;           // temp_mcusr - определяет с какой задержкой(скоростью) меняется яркость свечения
                           // при загорании и тушении светодиодов
  }
  else{temp_mcusr=2;};     // 2-3 гламурненько Но. Для 1 Тик ппс слишком медленно. Больше 3х врядли имеет смысл.
  Time_ON=1;               // ???
  step=0;                  // Устанавливаем использование 0-ого члена массива
  SetTimer4ms(0);              // Сбрасываем время с начала периода
  SetTimer4msl(0);            // Сбрасываем время с события последненго перемещения по массиву интенсивностей

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
     RestWDT();
    if (GPS_STATE==2)
    {
      if(smenaPPS)          // Прошло TIC_PPS импульсов PPS
      {
        Mig_ON_nOFF=0;      // Сброс флага на переход к анализу этапа горения
        smenaPPS=0;         // Сброс флага
        SetTimer4ms(Temp_T2+1); // Метка времени переведена на окончание периода
      }
      //*********************************************************************
      else
      {
      }
      //*********************************************************************

      if (Mig_ON_nOFF==0 && Temp_T1 == GetTimer4ms() )// && Temp_T2 >= time_4ms  // Событие окончание горение
      {
        Mig_ON_nOFF=1;   // Необходимо изменение интенсивности излучения
        Temp_bit=0;      // Проходим массивы интенсивности излучения справо на лево
        Time_ON=1;       // ???
        SetTimer4msl(0);   // Сброс прошедшего времени
      };

      if (Mig_ON_nOFF==0 && Temp_T2 < GetTimer4ms() ) // Событие окончание периода
      {
        Mig_ON_nOFF=1;   // Необходимо изменение интенсивности излучения
        Temp_bit=1;      // Проходим массивы интенсивности излучения слево на право
        Time_ON=1;       // ???
        SetTimer4msl(0);    // Сброс прошедшего времени
        SetTimer4ms(0);     // Сброс отсчетв времени на новый период
        if(!PPS_stat) PPS_OK = 0;
        PPS_stat = 0;
      };   //!!!putchar0(Temp_Ltime);


      if(Mig_ON_nOFF) // Необходимо изменение интенсивности излучения
      {
        if(Temp_bit)  // Проходим массивы интенсивности излучения слево на право
        {
          if(GetTimer4msl(0)>=temp_mcusr) // Прошедшее время превысило интервал изменения интенсивности
          {
        	SetTimer4msl(0);      // Сброс прошедшего времени
            if(Time_ON)        // ???      (Надо добавить не тупо инкремент а +ЯЯ и в ифе меньше ЯЯ)
            {
              if(step <= 13)   // Рабочий цикл в процессе
              {
                  SetPWM4(_jarcostjB_mas[step]);  // Изменеие интенсивности                 
                  SetPWM3(_jarcostjW_mas[step]);  // Изменеие интенсивности
              //    SetPWM2(_jarcostj1_mas[step]);
                //  SetPWM1(_jarcostj2_mas[step]);

                step++;
              }
              else             // Рабочий цикл закончен
              {
                step=0;        // Сброс в начало массива
                Time_ON=0;     // ???
                Mig_ON_nOFF=0; // Сброс необходимости изменения интенсивности свечения
              };


            };
          };
        }
        else  // Проходим массивы интенсивности излучения справо на лево
        {
          if(GetTimer4msl(0)>=temp_mcusr)
          {
        	SetTimer4msl(0);      // Сброс прошедшего времени
            if(Time_ON)        // ???
            {
              if(step <= 13)   // Рабочий цикл в процессе
              {
            	SetPWM4(_jarcostjB_mas[13-step]);  // Изменеие интенсивности            	
            	SetPWM3(_jarcostjW_mas[13-step]);  // Изменеие интенсивности
            //	SetPWM2(_jarcostj1_mas[13-step]);
            //	SetPWM1(_jarcostj2_mas[13-step]);

                step++;
              }
              else             // Рабочий цикл закончен
              {
                step=0;        // Сброс в начало массива
                Time_ON=0;     // ???
                Mig_ON_nOFF=0; // Сброс необходимости изменения интенсивности свечения
              };
            };
          };
        };
      };
      RestWDT();
      if (GetNMEAMessage(message_buffer))
               //Провеяем
               if (!CHECK_CRC(message_buffer))
               {
                   switch(message_buffer[3])
                   {
                        case 'R':
                        if (Parse_RMC_Command(message_buffer,&CurLat,&CurLong,&GroundSpeed,&system_time) == VALID)
                        {
                           if (system_time.second ==0)
                        	   B_ON = 1;        // Установка флага фазовой синхронизации
                                 // if (ValidStat) SinchStat = 1;
                                 // else           SinchStat = 0;


                        }
                        break;

                   }
                 }

    }
    else
    {
      RestWDT();
        if ((GetDIN_1() ==1) && (GetDIN_2() ==0) && (GetDIN_3()==0))  //Красный цвет, зажигаем только есть есть сигнал зеленог, а остальные пустые
       {    
         if (STATE!=1)
         {  
                SetPWM3(210);   // Изменеие интенсивности
                SetPWM4(0);  
                STATE=1;
          }
       }
       if (GetDIN_2() ==1)        //Желтый, зажигаем есть есть сигнал на желтом, на остальные не смотрим
       {       
        if (STATE!=2)
         { 
                SetPWM3(170);   // Изменеие интенсивности
                SetPWM4(100);
                STATE=2;
        }
       }
       if ((GetDIN_3()==1)  && (GetDIN_2() ==0) && (GetDIN_1()==0)) //Зеленый, зажигаем только есть есть сигнал красного, а остльные пустые
       {    
       if (STATE!=3)
         { 
        SetPWM3(0);   // Изменеие интенсивности
        SetPWM4(100);
         STATE=3;
        }
       }
       if ((GetDIN_1()==0) && (GetDIN_2()==0) && (GetDIN_3()==0))
       {                
              if (STATE!=0)    
              {
               SetPWM3(0);   // Изменеие интенсивности
               SetPWM4(0); 
               STATE=0; 
               }
       }
        if (GetDIN_1() == 1)
        {
          if (GetTime_2_2KHz_1() > 3*VEdge) SetDIN_1(0);
        }   

        if (GetDIN_2() == 1)
        {
          if (GetTime_2_2KHz_2() > 3*VEdge) SetDIN_2(0);
        }  
        if (GetDIN_3() == 1)
        {
          if (GetTime_2_2KHz_3() > 3*VEdge) SetDIN_3(0);
        } 
          
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14
                              |RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
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
  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
  /* EXTI4_15_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
  /* TIM3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* I2C1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(I2C1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_IRQn);
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
