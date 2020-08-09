//****************************************************

#include "ht16k33.h"
#include "delay.h"


//****************************************************
static IIC_hardware_init  g_iicHardWareOption;
uint8_t g_keyDataBuff[KEY_DATA_RAM_LENGTH] ={0};
uint8_t g_displayWriteBuff[DISPLAY_DATA_RAM_LENGTH]={0};
uint8_t g_numberArry[12]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x39,0x71};
void Init_Ht16K33_Peripheral(void)
{
    g_iicHardWareOption.sda_l = ClearSDALow;
    g_iicHardWareOption.sda_h = SetSDAHigh;
    g_iicHardWareOption.scl_l = ClearSCLLow;
    g_iicHardWareOption.scl_h = SetSCLHigh;
    g_iicHardWareOption.readSda = ReadSDA;
    g_iicHardWareOption.delayus = DelayUs;
    g_iicHardWareOption.delayuscount = 5;
    g_iicHardWareOption.sda_Output = SetSDAOutput;
    g_iicHardWareOption.sda_Input = SetSDAIntput;
    g_iicHardWareOption.bI2C_Write_ACK_OK = 0;
}

//****************************************************
void HT16_Start()
{
	g_iicHardWareOption.scl_h();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.sda_h();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.sda_l();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.scl_l();
}

//****************************************************
void HT16_Stop()
{
	g_iicHardWareOption.scl_h();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.sda_l();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.sda_h();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.scl_l();
}

//****************************************************
void Write_ACK_Check()
{
	uint8_t i;
	g_iicHardWareOption.sda_Input();
	g_iicHardWareOption.scl_h();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.bI2C_Write_ACK_OK=0;
	for(i=0;i<5;i++)
	{
		if(g_iicHardWareOption.readSda()==0)
		{
			g_iicHardWareOption.bI2C_Write_ACK_OK=1;
			i=5;
		}
	}
	g_iicHardWareOption.scl_l();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.sda_Output();
	g_iicHardWareOption.sda_l();
}

//****************************************************
void Read_ACK()
{
	g_iicHardWareOption.sda_l();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.scl_h();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.scl_l();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.sda_h();
}

//****************************************************
void Read_NACK()
{
	g_iicHardWareOption.sda_h();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.scl_h();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.scl_l();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	g_iicHardWareOption.sda_l();
}

//****************************************************
//after called , According to g_iicHardWareOption.bI2C_Write_ACK_OK to determine whether to rewrite
//****************************************************
void Byte_Write(unsigned char WriteByte)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		g_iicHardWareOption.scl_l();
		if(WriteByte&0x80)
		{
			g_iicHardWareOption.sda_h();
		}
		else
		{
			g_iicHardWareOption.sda_l();
		}
		g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
		g_iicHardWareOption.scl_h();
		WriteByte<<=1;
	}
	g_iicHardWareOption.scl_l();
	g_iicHardWareOption.sda_Input();
	g_iicHardWareOption.scl_h();
	g_iicHardWareOption.delayus(g_iicHardWareOption.delayuscount);
	for(i=0;i<5;i++)
	{
		if(g_iicHardWareOption.readSda()==0)
		{
			g_iicHardWareOption.bI2C_Write_ACK_OK=1;
			i=5;
		}
		else
		{
			g_iicHardWareOption.bI2C_Write_ACK_OK=0;
		}
	}
	g_iicHardWareOption.scl_l();
	g_iicHardWareOption.sda_Output();
	g_iicHardWareOption.sda_l();
}

//****************************************************
unsigned char Byte_Read()
{
	uint8_t i;
	uint8_t ReadByte;
	g_iicHardWareOption.sda_Input();
	ReadByte=0x00;
	for(i=0;i<8;i++)
	{
		g_iicHardWareOption.scl_h();
		ReadByte<<=1;
		if(g_iicHardWareOption.readSda())
		{
			ReadByte=ReadByte&0xfe;
			ReadByte=ReadByte|0x01;
		}
		g_iicHardWareOption.scl_l();
	}
	g_iicHardWareOption.sda_Output();
	return ReadByte;
}

//****************************************************
void HT16_Command_Write_Operation(unsigned char CommandCode)
{
    uint8_t errCount =0;
	do
	{
		HT16_Start();
		Byte_Write(WRITE_COMMAND);
        errCount++;
		if(g_iicHardWareOption.bI2C_Write_ACK_OK)
		{
			Byte_Write(CommandCode);
		}
	}while((g_iicHardWareOption.bI2C_Write_ACK_OK==0)&&(errCount < 4)); // 4次没有通讯上就跳过该指令
	HT16_Stop();
}

//****************************************************
void HT16_Byte_Write_Operation(unsigned char Waddr,unsigned char Wdata)
{
    uint8_t errCount =0;
	do
	{
		HT16_Start();
		Byte_Write(WRITE_COMMAND);
        errCount++;
		if(g_iicHardWareOption.bI2C_Write_ACK_OK)
		{
			Byte_Write(Waddr);
			if(g_iicHardWareOption.bI2C_Write_ACK_OK)
			{
				Byte_Write(Wdata);
			}
		}
	}while((g_iicHardWareOption.bI2C_Write_ACK_OK==0)&&(errCount < 4));// 4次没有通讯上就跳过该指令
	HT16_Stop();
}

//****************************************************
unsigned char HT16_Byte_Read_Operation(unsigned char Raddr)
{
    uint8_t errCount =0;
	uint8_t Rdata;
	do
	{
		HT16_Start();
		Byte_Write(WRITE_COMMAND);
        errCount++;
		if(g_iicHardWareOption.bI2C_Write_ACK_OK)
		{
			Byte_Write(Raddr);
            errCount =0;
		}
	}while((g_iicHardWareOption.bI2C_Write_ACK_OK==0)&&(errCount < 4));// 4次没有通讯上就跳过该指令

    
	if(errCount > 4)
    {
        return 0xFF; //没有完成通讯链接，返回错误数据
    }
    
	HT16_Stop();
	do
	{
		HT16_Start();
		Byte_Write(READ_COMMAND);
        errCount++;
	}while((g_iicHardWareOption.bI2C_Write_ACK_OK==0)&&(errCount < 4));// 4次没有通讯上就跳过该指令
	Rdata=Byte_Read();
	Read_NACK();
	HT16_Stop();
	return Rdata;
}

//****************************************************
//		Rlength should be <= sizeof(RdataArray)
//****************************************************
void HT16_Page_Read_Operation(unsigned char Rstartaddr,unsigned char Rlength,unsigned char * RdataArray)
{
	uint8_t i;
    uint8_t errCount =0;
	do
	{
		HT16_Start();
		Byte_Write(WRITE_COMMAND);
        errCount++;
		if(g_iicHardWareOption.bI2C_Write_ACK_OK)
		{
			Byte_Write(Rstartaddr);
            errCount =0;
		}
	}while((g_iicHardWareOption.bI2C_Write_ACK_OK==0)&&(errCount < 4));// 4次没有通讯上就跳过该指令
	HT16_Stop();

    if(errCount > 4)
    {
        return ; //没有完成通讯链接，返回错误数据
    }
    
	do
	{
		HT16_Start();
		Byte_Write(READ_COMMAND);
        errCount++;
	}while((g_iicHardWareOption.bI2C_Write_ACK_OK==0)&&(errCount < 4));// 4次没有通讯上就跳过该指令

    if(errCount > 4)
    {
        return ; //没有完成通讯链接，返回错误数据
    }
    
	for(i=0;i<Rlength-1;i++)
	{
		RdataArray[i]=Byte_Read();
		Read_ACK();
	}
	RdataArray[Rlength-1]=Byte_Read();
	Read_NACK();
	HT16_Stop();
}

//****************************************************
//		Wlength should be <= sizeof(WdataArray)
//****************************************************
void HT16_Page_Write_Operation(unsigned char Waddr,unsigned char Wlength,unsigned char * WdataArray)
{
	uint8_t j,temp;
    uint8_t errCount =0;
	do
	{
		HT16_Start();
		Byte_Write(WRITE_COMMAND);
        errCount++;
		if(g_iicHardWareOption.bI2C_Write_ACK_OK)
		{
			Byte_Write(Waddr);
            errCount = 0;
			for(j=0;j<Wlength;j++)
			{	
				temp=WdataArray[j];
				if(g_iicHardWareOption.bI2C_Write_ACK_OK)
				{	
					Byte_Write(temp);
				}
				else
				{
					j=Wlength;
				}
			}
		}
	}while((g_iicHardWareOption.bI2C_Write_ACK_OK==0)&&(errCount < 4));// 4次没有通讯上就跳过该指令
	HT16_Stop();
}

void  Ht16k33Init(void)
{
    uint8_t i=0;

    Init_Ht16K33_Peripheral();
    HT16_Command_Write_Operation(SYSTEM_ON);
    //Ht16CommandWriteOperation(SET_INT_OUTPUT_LOW);SET_ROW_OUTPUT
    HT16_Command_Write_Operation(SET_ROW_OUTPUT);
    HT16_Command_Write_Operation(DISPLAY_OFF);
    HT16_Command_Write_Operation(DIMMING_SET_DEFAULT);
    for(i=0;i<DISPLAY_DATA_RAM_LENGTH;i++)
    {
        HT16_Byte_Write_Operation(i,0x00);
    }
    //It is strongly recommended that the key data RAM of address 0x40H~0x45H
    //should be read continuously and in one operation	
    HT16_Page_Read_Operation(KEY_DATA_RAM_START,KEY_DATA_RAM_LENGTH,g_keyDataBuff);
    HT16_Command_Write_Operation(SYSTEM_OFF);
}

uint16_t GetKeyScan(void)
{
    uint16_t res =0;
    uint8_t i =0;
    uint8_t j =0;
    
    HT16_Page_Read_Operation(KEY_DATA_RAM_START,KEY_DATA_RAM_LENGTH,g_keyDataBuff);
    for(i =0 ;i<2;i++)
    {
        for(j= 0;j<3;j++)
        {
            if(0x00 !=(g_keyDataBuff[2*i] &(0x01 << j)))
            {
               res |=  0x0001 << (j+3*i);
            }

        }
    }

    for(j= 0;j<4;j++)
    {
        if(0x00 !=(g_keyDataBuff[4] &(0x01 << j)))
        {
           res |=  0x0001 << (j+6);
        }
    }
    return res;
}
	
