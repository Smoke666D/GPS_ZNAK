#ifndef __NMEA_H
#define __NMEA_H


#include "main.h"
#define CR_SYMBOL               13
#define LF_SYMBOL               10

#define MAX_NMEA_SIZE           80   //������������ ������ ��������� NMEA
#define MAX_FLOAT_SIZE          10   //������������ ���-�� �������� ������ � �����

#define GSA_STATUS  9


typedef enum
{
NO_MESSAGE,
GPGLL,
GPRMC,
GPVTG,
GPGGA,
GPGSA,
GPGSV,
GPZDA
} NMEA_PARSER_CODE;


typedef enum
{
LATITUDE,
LONGITUDE,

} COR_TYPE;

typedef struct 
{
  unsigned char hour;                      
  unsigned char minute;
  unsigned char second;                      
  unsigned int  milisecond;
  unsigned char YAER;
  unsigned char MOUNTH;
  unsigned char DATE;
} GPS_TIME;

typedef enum
{
VALID,
UNVALID,
RMX_COMMAND_ERROR,
} RMC_PARCE_ERROR;
typedef enum
{
NO_RES,
RES_GPS,
RES_GLONASS,
RES_ERROR,
} GSA_PARCE_ERROR;

typedef enum
{
 NO_VALID_FIX,
 GPS_FIX,
 DIFF_GPS_FIX,
 PPS_FIX,
 RTK_INT_FIX,
 RTK_FLOAT_FIX,
 NOT_DEFINE_CODE,
} GGA_PARCE_ERROR;

typedef enum
{
NMEA_PDOP,
NMEA_HDOP,
NMEA_VDOP,
} DOP_TYPE;


unsigned char NMEA_CRC(unsigned char * data, unsigned char count);
unsigned char  GetNMEAMessage(unsigned char * buffer);
NMEA_PARSER_CODE NMEA_Parser();
void CLEAR_NMEA_MASSEGE_BUFFER(void);
float ConverNEMEAStringToFloat(unsigned char *NMEAString);
GSA_PARCE_ERROR Parse_GSA_Command(unsigned char * Command,float * PDOP,float * HDOP, float * VDOP );
GGA_PARCE_ERROR Parce_GGA_Command(uint8_t * Commnad,GPS_TIME *gps_time);
double ConvertNEMEAStringToDegree(unsigned char * NEMEAString,COR_TYPE Lat_Long);
RMC_PARCE_ERROR Parse_RMC_Command(unsigned char * Command,double * Lat,double * Long, float * Speed,GPS_TIME *gps_time);
unsigned char NMEA_CHECK(unsigned char DATA);
unsigned char NMEA_READY();
unsigned char CHECK_CRC(unsigned char * data);

#endif
