#ifndef _HT16K33_H_
#define _HT16K33_H_
#include "gpio.h"
#include "common.h"

#define		SLAVE_ADDRESS			0xEE	//slave address= 0xE0 NOTE :当前例程的接法是 A0 A1 A2的取值是 1 1 1；
#define		WRITE_COMMAND			0xEE	
#define		READ_COMMAND			0xEF
#define		SYSTEM_ON				0x21
#define		SYSTEM_OFF				0x20
#define		SET_ROW_OUTPUT			0xA0
#define		SET_INT_OUTPUT_LOW		0xA1
#define		SET_INT_OUTPUT_HIGH		0xA3
#define		DISPLAY_OFF				0x80
#define		DISPLAY_ON_OFF			0x81
#define		DISPLAY_ON_2HZ			0x83
#define		DISPLAY_ON_1HZ			0x85
#define		DISPLAY_ON_05HZ			0x87
#define		DIMMING_SET_DEFAULT		0xEF
#define		DIMMING_SET_ddds		0xE3

#define		KEY_DATA_RAM_START		0x40
#define		KEY_DATA_RAM_LENGTH		0x06

#define		DISPLAY_DATA_RAM_START	0x00
#define		DISPLAY_DATA_RAM_LENGTH	16


typedef void(*VOID_FPTR)(void);
typedef uint8 (*U8_FPTR)(void);
typedef void(*FPTR_U16)(uint16);


typedef struct 
{
    VOID_FPTR sda_l;
    VOID_FPTR sda_h;
    VOID_FPTR scl_l;
    VOID_FPTR scl_h;
    VOID_FPTR sda_Output;
    VOID_FPTR sda_Input;
    U8_FPTR readSda;
    FPTR_U16 delayus;
    uint8 delayuscount;
    uint8 bI2C_Write_ACK_OK;
}IIC_hardware_init;



typedef  struct
{
	uint8_t keyScanNo;
	uint8_t keyNo;
}KeyPositionStruct;

//****************************************************

void HT16_Command_Write_Operation(unsigned char CommandCode);
void HT16_Page_Read_Operation(unsigned char Rstartaddr,unsigned char Rlength,unsigned char * RdataArray);
void HT16_Page_Write_Operation(unsigned char Waddr,unsigned char Wlength,unsigned char * WdataArray);
void HT16_Byte_Write_Operation(unsigned char Waddr,unsigned char Wdata);
unsigned char HT16_Byte_Read_Operation(unsigned char Raddr);



//****************************************************

void  Ht16k33Init(void);
uint16_t GetKeyScan(void);

#endif

