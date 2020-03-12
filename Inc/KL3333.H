#ifndef __kl3333_H
#define __kl3333_H


#define PMTK_API_SET_NMEA_OUTPUT  '314' 

#define PMTK_TEST                "$PMTK000*32"
#define PMTK_CMD_HOT_START       "$PMTK101*32" 
#define PMTK_CMD_WARM_START      "$PMTK102*31" 
#define PMTK_CMD_COLD_START      "$PMTK103*30" 
#define PMTK_CMD_FULL_COLD_START "$PMTK104*37" 
#define PMTK_SET_NMEA_BAUDRATE   (uint8_t *)"$PMTK251"
#define  PMTK_DT_NMEA_OUTPUT     (uint8_t *)"$PMTK314"
#define PMTK_SET_PPS_CONFIG_CMD  (uint8_t *)"$PMTK285"
#define PMTK_SET_SYNC_PPS_NMEA   "$PMTK255"
#define PMTK_API_SET_GNSS_SEARCH_MODE (uint8_t *)"$PMTK353"

#define NMEA_PPS_CONFGI		(uint8_t *)"1,310"
#define NMEA_GPS_GLOANSS    (uint8_t *)"1,1,0,0,0"
#define NMEA_OUTPUT_RMC_ONE_FIX_POSITION "0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
#define NMEA_OUTPUT_GGA_ONE_FIX_POSITION (uint8_t *)("0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0")
#define NMEA_OUTPUT_RMC_GSA_ONE_FIX_POSITION "0,1,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0"                                        

#define RESET_GPS_ON HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET)
#define RESET_GPS_OFF HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET)

#define RESET_STATE     0
#define INIT_RATE_STATE 1
#define INIT_OUT_STATE  2
#define GET_DATA_STATE  3


typedef enum
{
    API_SET_NMEA_OUTPUT,
    API_SET_DATUM,
    
} PMTK_COMMAND;

typedef enum
{
    ACK_INVALID_COMMAND,
    ACK_UNSUPPORTED_COMMAND,
    ACK_ACTION_FAILED,
    ACK_ACTION_SUCCEDED,    
    ACK_ERROR_COMMAND,
    ACK_ERROR_CRC,
    ACK_ERROR_FLAG,
    ACK_UNEXPECTED_DATA,
    ACK_TIME_OUT,
} ACK_CODE;


void GPS_Task(void const * argument);
ACK_CODE PMTK_ACK(unsigned char * Command);
ACK_CODE PMTK_NO_PARAMETR_COMMAND( unsigned char *DATA);
ACK_CODE PMTK_PARAMETR_COMMAND( uint8_t *Command,uint8_t *Parametr);
ACK_CODE SetNMEAOutput(void);
void SetNmeaCommandFreq(unsigned char NMEA_SEN, unsigned char Setting);

#endif
