# HT16K33_drive

根据当前的结构体中的指针函数的功能定义匹配自己的工程；
//写入HT16K33的功能指令 ，其中的slave_adress请根据参考手册中的说明设定（因为不同的接法和封装引脚对应不同的slave_adress)
void HT16_Command_Write_Operation(unsigned char CommandCode);

//读按键的缓存数据，一般是一次性全部读出来，一次性读6个数据，
void HT16_Page_Read_Operation(unsigned char Rstartaddr,unsigned char Rlength,unsigned char * RdataArray);

//写HT16K33显示缓存的指令（建议是一次性写16个字节）
void HT16_Page_Write_Operation(unsigned char Waddr,unsigned char Wlength,unsigned char * WdataArray);

//写单独的HT16K33的显示缓存
void HT16_Byte_Write_Operation(unsigned char Waddr,unsigned char Wdata);

//
unsigned char HT16_Byte_Read_Operation(unsigned char Raddr);


//建议采用官方给定的显示和按键demo示例，至于想知道显示和按键输入的缓存对应哪些字节可以采用发送指令显示，减少开发时间；
//类似于像下面的方式能够很快知道哪些位对应哪个断码管显示，然后就自行标记输出了
data = UartReceive[0];
HT16_Byte_Write_Operation(00，data); //data =0x01;0x01;0x04;0x08;
