//#include "utils.h"
#include "USART.H"
#include "KL3333.H"
//#include "NMEA.H"
#include "gpio.h"
//#include "stm32f0xx_it.h"
#include "main.h"
#include "iwdg.h"
//#include "HMI.H"


#define NMEA_COMMAND_SUPPORTED_COUNT    19

#define NMEA_SEN_GLL            0
#define NMEA_SEN_RMC            1
#define NMEA_SEN_VTG            2
#define NMEA_SEN_GGA            3
#define NMEA_SEN_GSA            4
#define NMEA_SEN_GSV            5
#define NMEA_SEN_ZDA            17
#define DISABLED                0
#define EVERY_ONE_POSITION      1
#define EVERY_TWO_POSITION      2
#define EVERY_THREE_POSITION    3
#define EVERY_FOUR_POSITION     4
#define EVERY_FIVE_POSITION     5

#define INVALID_COMMAND         '0'
#define UNSUPPORTED_COMMAND     '1'
#define ACTION_FAILED           '2'
#define ACTION_SUCCEDED         '3'
#define ACK_TIMEOUT             1000
const unsigned char * PMTR_ACK="$PMTK001";
static const char* hex_digits="0123456789ABCDEF";

unsigned char InitKL3333()
{
  static unsigned char  message_buffer[200];
  unsigned char STATE= 0;
  ACK_CODE ACK;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
  ResetSecCounter();
  
  HAL_UART_Receive_IT(&huart1, (uint8_t*)&message_buffer[0], 1);
  while (Get0_5SecCounter() < 2)
  {
     RestWDT();
  }
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); 
  ClearRXBuffer();
   ResetSecCounter();
  //÷икл на 10 секунд дл€ инициализации модул€ GPS
  while (Get0_5SecCounter() <= 10)
  {      
     RestWDT();
    //„ерез 2 секудны после начала работы переинециализируетм канал св€зи с GPS модулем на штатную скорость работы
    if (((Get0_5SecCounter() == 5) && (STATE ==0)) || (STATE==3))
     {                                        
            STATE = 1;           
            ClearRXBuffer();
     }     
     //¬ыходим из цикла, в случаее если прин€т пакет на штатной скорости рабоы, в противном случаее выходим из цикла через 4 секудны, со значением EREOR_STATE   
     if  (STATE == 2)
     {
        
        ClearRXBuffer();
        PMTK_PARAMETR_COMMAND(PMTK_DT_NMEA_OUTPUT,NMEA_OUTPUT_RMC_ONE_FIX_POSITION );
       
      //  SetNmeaCommandFreq(17,1);
       // ACK = SetNMEAOutput();
       // ACK = PMTK_PARAMETR_COMMAND( PMTK_SET_PPS_CONFIG_CMD ,"3,10"); //”станавливаем парамерты PPS сигнала, 2D/3D fix, 10 мс длительность PPS
        //ACK = PMTK_PARAMETR_COMMAND( PMTK_SET_SYNC_PPS_NMEA,"1")       //¬ключаем PPS
       return STATE;                 
     }
     /*
      ѕринимаем пакет от GPS модул€, если переменна€ STATE в начальном значении INIT_9600_STATE, значит пакет прин€т на скорости 9600 ,
      тогда отправл€ем команду на изменение скорости, если переменна€ STATE имеет значение ERROR_STATE, значит канал уже перенасторен на
      штаную скорость и можно переходить к инициализации GPS модул€
     */
     if (GetNMEAMessage(message_buffer))
     {
        ClearRXBuffer();
       if ((message_buffer[0] == '$') && (message_buffer[1]=='G') )       
                switch (STATE)
                {
                    case 1:
                        STATE = 2;                      
                        break;
                    case 0:   
         
                        PMTK_PARAMETR_COMMAND( PMTK_SET_NMEA_BAUDRATE,"115200");                                                
                        STATE=3;
                        break;
                }                                                                              

      }
  }
 return STATE;
}
 unsigned int temp;
 ACK_CODE PMTK_ACK(unsigned char * Command)
 {
   unsigned char i;
   ACK_CODE error_code = ACK_TIME_OUT;
   unsigned char DATA[100];
  
      
   
   //ƒл€ гаранитрованой работы системы, провер€м корректность ответа от приемника, поскольку на момент 
   //передачи данных приемник мог выводить другие сообщени€, ответ может прейти через одно или несколько
   // NMEA сообщений, поэтому ждем пакета, PMTK
   
   for (i=0;i<100;i++)     
   {
     if (GetNMEAMessage(DATA))       
               if (DATA[1] =='P') 
               {
                 error_code =ACK_UNEXPECTED_DATA;
                 break;                         
               }
   }
     
   if (error_code != ACK_TIME_OUT)
   {
   //ѕровера преамбулы, если очень хочетьс€, можно использовать стандрартную библиотеку работы
   //со строками, но дл€ оптимизации прозиводительнсти суммируем полученные данные по XOR c ожидаемым ответом
   if (!(DATA[0]^PMTR_ACK[0]^DATA[1]^PMTR_ACK[1]^DATA[2]^PMTR_ACK[2]^DATA[3]^PMTR_ACK[3]^DATA[4]^PMTR_ACK[4]^DATA[5]^PMTR_ACK[5]^DATA[6]^PMTR_ACK[6]^DATA[7]^PMTR_ACK[7]))
   {
     temp = DATA[9]^Command[5]^DATA[10]^Command[6]^DATA[11]^Command[7];
      if (!temp)
      {
            i = NMEA_CRC(&DATA[1],13);
            if (!( hex_digits[ i>>4 ]^DATA[16] ^ hex_digits[ i&0x0F]^DATA[15]))
            {
               switch (DATA[13])
               {
                  case '0':
                        error_code = ACK_INVALID_COMMAND;
                        break;
                  case '1':
                        error_code = ACK_UNSUPPORTED_COMMAND;
                        break;
                  case '2':
                        error_code = ACK_ACTION_FAILED;
                        break;
                  case '3':
                        error_code = ACK_ACTION_SUCCEDED;
                        break;
               }
            }
            else 
              error_code =   ACK_ERROR_CRC;
      }
      else
        error_code = ACK_ERROR_COMMAND;
     
   }
   }
   return error_code;
 }

unsigned char NMEA_COMMAND_FREQ[NMEA_COMMAND_SUPPORTED_COUNT] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void SetNmeaCommandFreq(unsigned char NMEA_SEN, unsigned char Setting)
{
     NMEA_COMMAND_FREQ[NMEA_SEN] = Setting;
}  

ACK_CODE SetNMEAOutput(void){
   unsigned char CRC1;
   ACK_CODE return_code;
   unsigned char Command[45]={'P','M','T','K','3','1','4',',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0,',',0};
   for (unsigned char i=0;i < NMEA_COMMAND_SUPPORTED_COUNT;i++)
   {
     Command[8+i*2] =  NMEA_COMMAND_FREQ[i];     
   }
   put_char('$');
   put_str_pak(Command,45);
   put_char('*');
   CRC1 =NMEA_CRC(Command,45);
   put_char( hex_digits[CRC1>>4]);
   put_char( hex_digits[CRC1 & 0x0F]);
   put_char(CR_SYMBOL);
   put_char(LF_SYMBOL);   
   
   return_code = PMTK_ACK( &Command[4]);
   return return_code;
  
}




ACK_CODE PMTK_PARAMETR_COMMAND( unsigned char *Command, unsigned char *Parametr)
{
    unsigned char CRC1,i;
    
    put_str_pak(Command,8);
    put_char(',');
    //put_str_pak(Parametr,strlen(Parametr));
    for (i=0;i<250;i++)
    {
        if (!Parametr[i]) break;
        put_char(Parametr[i]);
    }
    put_char('*');
    CRC1 = NMEA_CRC(&Command[1],7) ^ NMEA_CRC(Parametr,i)^ ',';
    put_char( hex_digits[CRC1>>4]);
    put_char( hex_digits[CRC1 & 0x0F]);
    ClearRXBuffer();
    put_char(CR_SYMBOL);
    put_char(LF_SYMBOL);
    ClearRXBuffer();
    return PMTK_ACK(Command);
}

//PMTK_TEST

/*void SEND_COMMAND(PMTK_COMMAND command,unsigned char * parametr, unsigned char parametr len)
{
  put_str_pak("$PMTK",5);
    switch (command)
    {
        case API_SET_NMEA_OUTPUT:
          put_srt_pak(PMTK_API_SET_NMEA_OUTPUT,3);
          put_str_pak(parametr);
          put_str("
          break;
      
      
    }
    
  
}

  */                
                  
                  
                  
                  
                  
