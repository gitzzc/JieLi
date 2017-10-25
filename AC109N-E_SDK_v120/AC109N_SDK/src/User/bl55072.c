
#include "config.h"
#include "iic.h"

#include "bl55072.h"
#include "bike.h"

#define BL55072_WRITE          	BL_ADDR
#define BL55072_READ           	BL_ADDR+1

unsigned char BL55072_WriteBuf(u8 CW, u8* buf, u8 len) AT(BIKE_CODE)
{
  unsigned char i;
	
	if ( iic_busy ) return 0;

	iic_busy = 1;
    iic_start();
	iic_sendbyte(BL55072_WRITE);
	for(i=0;i<len;i++)
		iic_sendbyte(buf[i]);
	iic_stop();
	iic_busy = 0;

  return 1;
}

/**
  * @brief Initialize the UART mode according to the specified
  *         parameters in the UART_InitTypeDef and initialize the associated handle.
  * @param huart: UART handle.
  * @retval HAL status
  */
volatile unsigned int  BLTimeout = BL_TIMEOUT;

void BL55072_Config(unsigned char allon) AT(BIKE_CODE)
{
	unsigned char BL_Reg[6];

	BL_Reg[0] = 0xEE;	//ICSET InnerOsc,softrst
	BL_Reg[1] = 0xF0;	//BLKCTL BlinkingOff
	BL_Reg[2] = 0xFD;	//DISCTL 50Hz,Line inversion mode,high power mode
	BL_Reg[3] = 0xEC;	//ICSET InnerOsc
	BL_Reg[4] = 0xC8;	//MODESET Display on,1/3 bias
	if ( allon )
		BL_Reg[5] = 0xFE;	//APCTL Apon
	else
		BL_Reg[5] = 0xFC;	//APCTL normal
	
	BL55072_WriteBuf(BL_ADDR,BL_Reg,6);
}

unsigned char BL_Write_Data(unsigned char ADSTART,unsigned char LEN, unsigned char * p) AT(BIKE_CODE)
{
	unsigned char reg_buf[19];
	unsigned char i;
	
	if ( ADSTART + LEN > 19 ) return 0;
	
	reg_buf[0] = 0;
	for(i=0;i<LEN;i++)
		reg_buf[i+1] = p[i];
	
	return BL55072_WriteBuf(BL_ADDR,reg_buf,LEN+1);	
}

