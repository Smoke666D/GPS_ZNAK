//#include "utils.h"
#include "USART.H"
#include "KL3333.H"
#include "main.h"
//#include "NMEA.H"
#include "gpio.h"
#include "cmsis_os.h"
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
static unsigned char PMTR_ACK[]="$PMTK001";
static const char* hex_digits="0123456789ABCDEF";



static uint8_t GPS_FSM =RESET_STATE;
static unsigned char  message_buffer[200];
//double CurLat, CurLong = 0;
//float  GroundSpeed = 100;
GPS_TIME system_time;

void GPS_Task(void const * argument)
{
	while(1)
	{
    ResetWDT();
	osDelay(100);
	switch (GPS_FSM)
	{
		case RESET_STATE:
			RESET_GPS_ON;
			osDelay(1000);
			RESET_GPS_OFF;
			ClearRXBuffer();
			GPS_FSM = INIT_RATE_STATE;
			break;
		case INIT_RATE_STATE:
			if (GetNMEAMessage(message_buffer))
			{
				ClearRXBuffer();
			    if ((message_buffer[0] == '$') && (message_buffer[1]=='G') )
			    {
				   PMTK_PARAMETR_COMMAND(PMTK_SET_NMEA_BAUDRATE,(uint8_t *)"115200");
				   GPS_FSM = INIT_OUT_STATE;
				}
			}
			break;
		case INIT_OUT_STATE:
			if (GetNMEAMessage(message_buffer))
			{
				ClearRXBuffer();
			    if ((message_buffer[0] == '$') && (message_buffer[1]=='G') )
			    {
			    	PMTK_PARAMETR_COMMAND(PMTK_DT_NMEA_OUTPUT,NMEA_OUTPUT_GGA_ONE_FIX_POSITION );
			    	PMTK_PARAMETR_COMMAND(PMTK_SET_PPS_CONFIG_CMD, NMEA_PPS_CONFGI);
			    	PMTK_PARAMETR_COMMAND( PMTK_API_SET_GNSS_SEARCH_MODE,NMEA_GPS_GLOANSS);
				    GPS_FSM = GET_DATA_STATE;
				    ClearRXBuffer();
			    }
			}
			break;
		case GET_DATA_STATE:
			if (GetNMEAMessage(message_buffer) && GetPPS_OK())
			   if (!CHECK_CRC(message_buffer)  &&  (message_buffer[3]=='G')   )
			   {

			             if (Parce_GGA_Command(message_buffer,&system_time)!=NO_VALID_FIX)
			             {
			            	 if (system_time.second ==0)
			            	 {
			            		 osDelay(100);
			            		 SetB_ON();
			            	 }
			             }
			   }
			break;
		default:
			break;
	}
	}
}


 unsigned int temp;
 ACK_CODE PMTK_ACK(unsigned char * Command)
 {
   unsigned char i;
   ACK_CODE error_code = ACK_TIME_OUT;
   unsigned char DATA[100];
  
      
   
   //��� �������������� ������ �������, �������� ������������ ������ �� ���������, ��������� �� ������ 
   //�������� ������ �������� ��� �������� ������ ���������, ����� ����� ������ ����� ���� ��� ���������
   // NMEA ���������, ������� ���� ������, PMTK
   
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
   //������� ���������, ���� ����� ��������, ����� ������������ ������������ ���������� ������
   //�� ��������, �� ��� ����������� ����������������� ��������� ���������� ������ �� XOR c ��������� �������
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




ACK_CODE PMTK_PARAMETR_COMMAND( uint8_t *Command, uint8_t *Parametr)
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
                  
                  
                  
                  
                  
