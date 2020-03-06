#ifndef __kl3333_H
#define __kl3333_H


#define PMTK_API_SET_NMEA_OUTPUT  '314' 

#define PMTK_TEST                "$PMTK000*32"
#define PMTK_CMD_HOT_START       "$PMTK101*32" 
#define PMTK_CMD_WARM_START      "$PMTK102*31" 
#define PMTK_CMD_COLD_START      "$PMTK103*30" 
#define PMTK_CMD_FULL_COLD_START "$PMTK104*37" 
#define PMTK_SET_NMEA_BAUDRATE   "$PMTK251"
#define  PMTK_DT_NMEA_OUTPUT     "$PMTK314"
#define PMTK_SET_PPS_CONFIG_CMD  "$PMTK285"
#define PMTK_SET_SYNC_PPS_NMEA   "$PMTK255"
#define NMEA_OUTPUT_RMC_ONE_FIX_POSITION "0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
#define NMEA_OUTPUT_RMC_GSA_ONE_FIX_POSITION "0,1,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0"                                        


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


ACK_CODE PMTK_ACK(unsigned char * Command);
ACK_CODE PMTK_NO_PARAMETR_COMMAND( unsigned char *DATA);
ACK_CODE PMTK_PARAMETR_COMMAND( unsigned char *Command, unsigned char *Parametr);
ACK_CODE SetNMEAOutput(void);
void SetNmeaCommandFreq(unsigned char NMEA_SEN, unsigned char Setting);
unsigned char InitKL3333();

#endif
