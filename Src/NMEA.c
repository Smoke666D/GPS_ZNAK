//#include "NMEA.H"
#include "math.h"
#include "usart.h"
#include "main.h"

unsigned char NMEA_CRC(unsigned char * data, unsigned char count)
{
  unsigned char crc=0;
  for (unsigned char i=0;i<count;i++)
  {
    crc = crc ^ data[i];
  }
  return crc;
  
}
  
/* 
������� �������� ����������� ����� ��������� ������
��������� ������ ������, ���� ��� ������ ������ ������, �� ���� � ������ ������
����������� ����� '*', �������� ��� ���� ��������������� ���� ����������� �����.
����� ����� ���������� ������������ ��������� � ����������� ������ �� ������
� �������� ���������� ������� ����������:
0 - ���� CRC �� ������
������ ������ - ���� CRC ���������.������ ������ ������� ��� �������, � ���
����� CRC
*/
unsigned char CHECK_CRC(unsigned char * data)
{
    unsigned char i=0;
    unsigned char crc=0;
    if (data[0] == '$')
      
    for (i = 1; i<250;i++)
    {
      if (data[i] != '*')
      {
         crc = crc ^ data[i];
      }
      else 
        break;
    }
    if (crc != ((data[i+1]-48)*10 + (data[i+2]-48)))
      return 0;
    else 
      return (i+2);
    
}


static unsigned char MessageBuffer[100];
//static unsigned char MessageCount;
/*
������� ������������� ������� ������ NMEA, ����������� ������� ������ �� �������� �� �������������, 
��� ��������� ������ � ����������� ����, � ����� ���������� ���������� ������� �� ���������������������
��������������� � ����������� ������� ������
*/


/*
������� �������������� ����� � ��������� ������ �� NMEA ������. 
*NMEAString - ��������� �� ������ ���� ����� � ������
*/
float ConverNEMEAStringToFloat(unsigned char *NMEAString)
{
  float FloatData=0.0;
  unsigned char t,i,point_position=0;
  
  //���� ����� � ���������, ��� �� ���������� ����� ������, ����� ���� 0.00, 00.0, 0000
  for (i=0;i<MAX_FLOAT_SIZE;i++)
  {
    if (NMEAString[i]== '.') point_position=i;
    if ((NMEAString[i]== ',') || (NMEAString[i]== 0) || (NMEAString[i]== '*'))  break;
  }
  for (t=0;t<i;t++)
  {
    if (t<(point_position-1))
     FloatData =FloatData+ (NMEAString[t]-48)*(10.0*(point_position-t-1));
    if (t>point_position)
      FloatData =FloatData+(NMEAString[t]-48)/(10.0*(t-point_position));
    if (t==(point_position-1))
      FloatData =FloatData+ (NMEAString[t]-48);
  }
 return FloatData;  
}


//������� ����������� ���������� GPS � �������, ��� ���������� ����������� ����������.
double ConvertNEMEAStringToDegree(unsigned char * NEMEAString,COR_TYPE Lat_Long)
{
double  Degree,Minute, Second = 0;
char i =0;
switch (Lat_Long)
{
  case LATITUDE:   i=0;
    break;
  case LONGITUDE:  i=1;
    Degree = (NEMEAString[0]-48)*100;
    break;
}
Degree = (NEMEAString[0+i]-48)*10+(NEMEAString[1+i]-48);
Minute = (NEMEAString[2+i]-48)*10+(NEMEAString[3+i]-48)
        + (NEMEAString[5+i]-48)/10.0+(NEMEAString[6+i]-48)/100.0
        +(NEMEAString[7+i]-48)/1000.0+(NEMEAString[8+i]-48)/10000.0
          +(NEMEAString[9+i]-48)/100000.0+(NEMEAString[10+i]-48)/1000000.0;
Degree = Degree + Minute/60.0;
return Degree;
}

float GetNEMEA_DOP(unsigned char * NMEAString,DOP_TYPE DOP)
{
  unsigned char i,c=0;
  float ReturnValue = 20;
  unsigned char *NMEABuffer;
  NMEABuffer = &NMEAString[GSA_STATUS+2];
  for (i=0;i<MAX_NMEA_SIZE;i++)
  {
    if (NMEABuffer[i]==',') c++;
    if (c == 12) {i++;break;    }
  }
  switch (DOP)
  {
    case NMEA_PDOP:
      ReturnValue = ConverNEMEAStringToFloat(&NMEABuffer[i]);
    break;  
    case NMEA_HDOP:
      ReturnValue = ConverNEMEAStringToFloat(&NMEABuffer[i+5]);
    break;
    case NMEA_VDOP:
      ReturnValue = ConverNEMEAStringToFloat(&NMEABuffer[i+10]);
    break;
  }
  return  ReturnValue;
}

// &GPRMC , HH MM   SS  ,   STAT , LAT   , N/S ,   ,LON    ,   E/W   , SPEED
//   012345 6 78 910 1112 13   14  5 16-22 23 24  25  26-33 34    35  36  37
#define RMC_STATUS  18
#define RMC_LATITUDE 20
#define RMC_LONGITUDE 34
#define RMC_GROUND_SPEED 49
#define RMC_DATE  61
#define RMC_MONTH 63
#define RMC_YEAR  65
//$GNRMC,103547.991,A,5952.880211,N,03030.159098,E,2.95,291.45,261016,,,A*77
//$GNRMC,054521.300,A,5952.871026,N,03030.130130,E,0.00,114.29,271016,,,A*79

RMC_PARCE_ERROR Parse_RMC_Command(unsigned char * Command,double * Lat,double * Long, float * Speed,GPS_TIME *gps_time)
{
       unsigned char K=0;
      gps_time->hour = (Command[7]-48)*10+(Command[8]-48);
      gps_time->minute =(Command[9]-48)*10+(Command[10]-48);
      gps_time->second =(Command[11]-48)*10+(Command[12]-48);
      gps_time->milisecond = (Command[14]-48)*100+(Command[15]-48)*10+(Command[16]-48);
   if (Command[RMC_STATUS] == 'A') // ��������� ���������� ������
   {
      *Lat =  ConvertNEMEAStringToDegree(&Command[RMC_LATITUDE],LATITUDE);
      *Long = ConvertNEMEAStringToDegree(&Command[RMC_LONGITUDE],LONGITUDE);
      *Speed = ConverNEMEAStringToFloat(&Command[RMC_GROUND_SPEED ])*1.852; 
      for ( K=RMC_GROUND_SPEED+5; K<200;K++)
        if (Command[K] ==',') break;
      K++;
      gps_time->DATE = (Command[K ]-48)*10+(Command[K+1]-48);
      K=K+2;
      gps_time->MOUNTH =(Command[K ]-48)*10+(Command[K+1]-48);
      K=K+2;
      gps_time->YAER =(Command[K]-48)*10+(Command[K+1]-48);
                      
    }
    else
      return UNVALID;  
  return VALID;
}


//$GPGSA,A,3,28,32,24,27,22,14,08,,,,,,2.13,0.99,1.89*07
//$GLGSA,A,3,,,,,,,,,,,,,2.13,0.99,1.89*1E
GSA_PARCE_ERROR Parse_GSA_Command(unsigned char * Command,float * PDOP,float * HDOP, float * VDOP )
{
    
   if (Command[GSA_STATUS] == '3') // ��������� ���������� ������
   {
      //��������� ������ DOP, ��� ��������� �������� ��������� �� ������ PRN � NMEA ������
      *PDOP =   GetNEMEA_DOP(Command,NMEA_PDOP);
      *HDOP =   GetNEMEA_DOP(Command,NMEA_HDOP);
      *VDOP =   GetNEMEA_DOP(Command,NMEA_VDOP);
    }
    else    
      return NO_RES; 
  if (Command[2]=='P')
    return RES_GPS;
  else
    return RES_GLONASS;
}

//$$$GNGGA,235955.800,,,,,0,0,,,M,,M,,*53\r\n",
GGA_PARCE_ERROR Parce_GGA_Command(uint8_t * Command,GPS_TIME *gps_time)
{
  uint8_t i,counter =0;
  GGA_PARCE_ERROR ErrorCode=NOT_DEFINE_CODE;
  gps_time->hour = (Command[7]-48)*10+(Command[8]-48);
  gps_time->minute =(Command[9]-48)*10+(Command[10]-48);
  gps_time->second =(Command[11]-48)*10+(Command[12]-48);
  gps_time->milisecond = (Command[14]-48)*100+(Command[15]-48)*10+(Command[16]-48);
  for (i=0;i<0xFF;i++)
  {
	  if (Command[i]==',')
		 counter++;
	  if (counter>=5)
	  {
		switch(Command[i+1]-48)
		{
			  case 0:ErrorCode=NO_VALID_FIX;break;
			  case 1:ErrorCode=GPS_FIX;break;
			  case 2:ErrorCode=DIFF_GPS_FIX;break;
			  case 3:ErrorCode=PPS_FIX;break;
			  case 4:ErrorCode=RTK_INT_FIX;break;
			  case 5:ErrorCode=RTK_FLOAT_FIX;break;
		}
		break;
	  }
  }
  return ErrorCode;
}


static unsigned char NMEA_MESSAGE_READY = 0;
unsigned char NMEA_READY()
{
  if (NMEA_MESSAGE_READY)      
    return 1;  
  else 
    return 0;
    
}

void CLEAR_NMEA_MASSEGE_BUFFER(void)
{
  NMEA_MESSAGE_READY = 0;
}

unsigned char NMEA_CHECK(unsigned char DATA)
{
  if (DATA == LF_SYMBOL)
  {

    NMEA_MESSAGE_READY++;  

    return 1;
  }
  else
    return 0;
}


unsigned char GetNMEAMessage(unsigned char * buffer)
{
  unsigned char DATA;
  unsigned char DataCount=0;
  
  
   if (NMEA_MESSAGE_READY)      
   {  

     for (unsigned char i= 0;i <200;i++)
     {  
      while(!get_char(&DATA));
      if (!DataCount)
      {
        if (DATA == '$')
        {
          buffer[DataCount] = DATA;      
          DataCount++;          
        }
      }
      else
      {
        buffer[DataCount] = DATA;      
        DataCount++;
        if(DATA == LF_SYMBOL)
        {
        //*count = MessageCount;
           buffer = &MessageBuffer[i-DataCount];
           NMEA_MESSAGE_READY--;
           return 1;
        }
      }
     }
   }
  return 0;
  
}
