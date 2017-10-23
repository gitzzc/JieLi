/*--------------------------------------------------------------------------*/
/**@file     rda5807.c
   @brief    RDA5807收音底层驱动
   @details
   @author
   @date   2011-11-24
   @note
*/
/*----------------------------------------------------------------------------*/
#include "FM_API.h"

#ifdef	RDA5807

#include "RDA5807.h"
#include "iic.h"

//FM_RDA5807_VAR _xdata FM_rda5807_var;
_no_init u8 _idata rda5807_dat[12];
_no_init u8 _xdata read_dat[10];
_no_init u8 _xdata fm_type;

#if 0
/*--------------RDA5807SP Initial Table----------------*/
const u8 rda5807sp[] AT(RDA5807_TABLE_CODE)= {
	0xC0,0x01,	
#if defined(_FM_STEP_50K_)
    0x1b, 0x92,
    0x0C, 0x00,
#else       //Step 100K
    0x00, 0x10,
    0x04, 0x00,
#endif

    RDA5807_RSSI | 0x80, 0xBF, //05H:
    0x80, 0x00,
    0x5F, 0x1A, //07H
    0x5e, 0xc6,
    0x00, 0x00,
    0x40, 0x6e, //0AH:
    0x2d, 0x80,
    0x58, 0x03,
    0x58, 0x04,
    0x58, 0x04,
    0x58, 0x04,
    0x00, 0x47, //10H:
    0x90, 0x00,
    0xF5, 0x87,
    0x7F, 0x0B, //13H:
	0x04, 0xF1,
	0x5E, 0xc0, //15H: 0x42, 0xc0
    0x41, 0xe0,
    0x50, 0x6f,
    0x55, 0x92,
    0x00, 0x7d,
    0x10, 0xC0,//1AH
    0x07, 0x80,
    0x41, 0x1d,//1CH,
    0x40, 0x06,
    0x1f, 0x9B,
    0x4c, 0x2b,//1FH.
    0x81, 0x10, //20H:
    0x45, 0xa0,// 21H

#if defined(_FM_STEP_50K_)
    0x55, 0x3F, //22H
#else
    0x19, 0x3F, //22H
#endif

    0xaf, 0x40,
	0x06, 0x81,
    0x1b, 0x2a, //25H
    0x0D, 0x04,
	0x80, 0x9F, //0x80, 0x2F,
    0x17, 0x8A,
    0xD3, 0x49,
    0x11, 0x42,
    0xA0, 0xC4, //2BH
	0x3E, 0xBB,
    0x00, 0x00,
    0x58, 0x04,
    0x58, 0x04, //2FH
    0x58, 0x04,
    0x00, 0x74,
    0x3D, 0x00,
    0x03, 0x0C,
    0x2F, 0x68,
    0x38, 0x77, //35H
};

/*--------------RDA5807HP Initial Table----------------*/
const u8 rda5807hp[] AT(RDA5807_TABLE_CODE)= { //HP
    //0x00,0x01,
    //fm_dat[0] =0xC0; 	fm_dat[1] =0x05;
    0xc0,
    0x01,
    0x00,0x00,
    0x04,0x00,
    0x80 | RDA5807_RSSI,0xBF,//0xBF,  //05H
    0x40,0x00,
    0x46,0xC6,
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,  //0AH
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x06,  //10H  				
    0x00,0x19, //
    0x2A,0x11,
    0x00,0x2E,
    0x2A,0x30,
    0xB8,0x3C,  //15H
    0x90,0x00,
    0x2A,0x91,
    0x84,0x12,
    0x00,0xA8,
    0xC4,0x00,  //1AH
    0xE0,0x00,
    0x30,0x1D,//0x24,0x
    0x81,0x6A,
    0x46,0x08,
    0x00,0x86,  //1FH
    0x06,0x61,  //20H  // if 0x1661, 设置0xc05调整灵敏度  if 0x0661,设置0x23调整信噪比阀值
    0x00,0x00,
    0x10,0x9E,
    0x24,0xC9,//   0x23		 //0x25,0x4A   //0x23,0xC7 		 //值大台少
    0x04,0x08,//0830
    0x0d,0x18,  //0x06,
    0xE1,0x05,
    0x3B,0x6C,
    0x2B,0xEC,
    0x09,0x0F,
    0x34,0x14,  //2AH
    0x14,0x50,
    0x09,0x6D,
    0x2D,0x96,
    0x01,0xDA,
    0x2A,0x7B,
    0x08,0x21,   //30H
    0x13,0xD5,
    0x48,0x91,
    0x00,0xbc,
    0x08,0x96,   //34H
    0x15,0x3C,  //35H
    0x0B,0x80,   //36H
    0x25,0xC7,   //37H
    0x00,0x00,   //38H 	
};


/*--------------RDA5807MP Initial Table----------------*/
const u8 rda5807mp[] AT(RDA5807_TABLE_CODE)=
{
//#if defined(_SHARE_CRYSTAL_24MHz_)
//    0xC4,0x51, //02H:
//#elif defined(_SHARE_CRYSTAL_12MHz_)
//    0xC4,0x11, //02H:
//#elif defined(_SHARE_CRYSTAL_32KHz_)
//    0xC4,0x01, //02H:
//#else
#ifdef SHARE_RTC_OSC_TO_FM
  0xC4,0x01 , //02H:
#else
    0xC0,0x05,
#endif
//#endif
    0x00,
    0x10,
    0x04,
    0x00,
    RDA5807_RSSI | 0xC0, 
    0xBF, //05h
    0x60, 
    0x00,
    0x42, 
    0x1A,//11
    0x00, 
    0x00,
    0x00, 
    0x00,
    0x00, 
    0x00,  //0x0ah
    0x00, 
    0x00,
    0x00, 
    0x00,
    0x00, 
    0x00,
    0x00, 
    0x00,
    0x00, 
    0x00,
    0x00,
    0x00,  //0x10h
    0x00, 
    0x19,  
    0x2a, 
    0x11,
    0xB0,//0x80; 20110804
    0x42,//0x53;
    0x2a,//0x22; 20110804
    0x11,//加大音量0x31		wengyidong
    0xb8,//0xf8;		 20110804
    0x31,//0x30;  //0x15h
    0xc0, 
    0x00,
    0x2a, 
    0x91,
    0x94, 
    0x00,
    0x00, 
    0xa8,
    0xc4, 
    0x00,  //0x1ah
    0xf7,//0xe0; 20110804
    0x0f,//0x00;
    0x2a,//0x3a;20110804 
    0xdc,//0x14;  //0x1ch
    0x80,//0x81;20110804 
    0x6f,//0x6a;
    0x46, 
    0x08,
    0x00, 
    0x86,
    0x06, 
    0x61,  //0x20h
    0x00, 
    0x00,  
    0x10, 
    0x9e,
    0x23,//0x22;	20110804 
    0xc8,//0x44;
    0x04, 
    0x06,//0x08;  //0x24h 20110804
    0x0e,//0x0d;
    0x1c,//0x18;  //0x25h
};

/*----------------------------------------------------------------------------*/
/**@brief    RDA5807写寄存器函数
   @param    num 需要的数目
   @return   无
   @note     void rda5807_write(u8 num)
*/
/*----------------------------------------------------------------------------*/
void rda5807_write(u8 num) AT(RDA5807_CODE)
{
    app_IIC_write(RDA5807_WR_ADDRESS, 0xff, (u8 *)rda5807_dat, num);
}
/*----------------------------------------------------------------------------*/
/**@brief    RDA5807读寄存器函数
   @param    需要读取的数目
   @return   无
   @note     void rda5807_read(u8 num)
*/
/*----------------------------------------------------------------------------*/
void rda5807_read(u8 num) AT(RDA5807_CODE)
{
    app_IIC_readn(RDA5807_RD_ADDRESS, 0xff, read_dat, num);
}

/*----------------------------------------------------------------------------*/
/**@brief    RDA5807HP 初始化
   @param    无
   @return   无
   @note     void rda5807hp_init(void)
*/
/*----------------------------------------------------------------------------*/
void rda5807hp_init(void) AT(RDA5807_CODE)
{
   app_IIC_write(RDA5807_WR_ADDRESS, 0xff, (u8 *)rda5807hp, 110);
}

/*----------------------------------------------------------------------------*/
/**@brief    RDA5807写SP 初始化
   @param    num 需要的数目
   @return   无
   @note     void rda5807sp_init(void)
*/
/*----------------------------------------------------------------------------*/
void rda5807sp_init(void) AT(RDA5807_CODE)
{
   app_IIC_write(RDA5807_WR_ADDRESS, 0xff, (u8 *)rda5807sp, 104);
}

/*----------------------------------------------------------------------------*/
/**@brief    RDA5807MP 初始化
   @param    无
   @return   无
   @note     void rda5807mp_init(void)
*/
/*----------------------------------------------------------------------------*/
void rda5807mp_init(void) AT(RDA5807_CODE)
{	
   app_IIC_write(RDA5807_WR_ADDRESS, 0xff, (u8 *)rda5807mp, 72);
    
}

/*----------------------------------------------------------------------------*/
/**@brief    RDA5807 初始化
   @param    无
   @return   无
   @note     void init_RDA5807(void)
*/
/*----------------------------------------------------------------------------*/
__root void init_RDA5807(void) AT(RDA5807_CODE)
{
	u8 i;

    rda5807_dat[0] = 0x00;
    rda5807_dat[1] = 0x02;	

	rda5807_write(2);
	delay_n10ms(10);

	if(fm_type==3)//SP
	{
		for(i = 0;i<12;i++)
		{
		    rda5807_dat[i] = rda5807sp[i];
		}
		rda5807_dat[6] = RDA5807_RSSI | 0x80;
	}
	else if(fm_type==4)//HP
	{
		for(i = 0;i<12;i++)
		{
		    rda5807_dat[i] = rda5807hp[i];
		}
	}
	else if(fm_type==7)//MP
	{
		for(i = 0;i<12;i++)
		{
		    rda5807_dat[i] = rda5807mp[i];
		}
	    rda5807_dat[6] = RDA5807_RSSI | 0xC0;
	}


	rda5807_write(12);
	delay_n10ms(5);

	/*
#if 1
	rda5807_dat[7] &=~0x0F;
	rda5807_dat[7] |= 0x0f;
#endif
    */
	if(fm_type==3)//sp
	{
		 rda5807sp_init();//	110
	}
	else if(fm_type==4)//hp
	{
		rda5807hp_init();//	110
	}
	else//MP
	{
	    rda5807mp_init();  //72
	}
		
	delay_n10ms(5);
}

/*----------------------------------------------------------------------------*/
/**@brief    设置一个频点RDA5807
   @param    fre 频点  875~1080
   @return   1：当前频点有台，0：当前频点无台
   @note     bool set_fre_RDA5807(u16 fre)
*/
/*----------------------------------------------------------------------------*/
__root bool set_fre_RDA5807(u16 fre) AT(RDA5807_CODE)
{
    u16 pll;
	u8 i;

	i = 0;

    pll = (fre - 870);
    rda5807_dat[2] = pll >> 2;
    rda5807_dat[3] = ((pll & 0x0003)<<6)|0x10;

    rda5807_write(4);
    delay_n10ms(5);

	do
	{
        rda5807_read(4);
		i++;
		//delay_n10ms(1);
		if(rda5807_true())
		{
		    return 1;
		}
	}while(i<2);

    return 0;
}


/*----------------------------------------------------------------------------*/
/**@brief    关闭RDA5807的电源
   @param    无
   @return   无
   @note     void RDA5807_PowerDown(void)
*/
/*----------------------------------------------------------------------------*/
__root void RDA5807_PowerDown(void) AT(RDA5807_CODE)
{
	rda5807_dat[1] &= ~(1<<0);
	rda5807_write(2);
	delay_n10ms(5);
}

/*----------------------------------------------------------------------------*/
/**@brief    FM 模块静音控制
   @param    dir：1-mute 0-unmute
   @return   无
   @note     void RDA5807_mute(dir)
*/
/*----------------------------------------------------------------------------*/
__root void RDA5807_mute(u8 dir) AT(RDA5807_CODE)
{	
	if(dir)
	{
		rda5807_dat[0] &= 0xBF;// mute on
	}
	else
	{	
		rda5807_dat[0] |= 0x40;// mute off
	}
	rda5807_write(2);
}
/*----------------------------------------------------------------------------*/
/**@brief   FM模块检测，获取RDA5807 模块ID
   @param   无
   @return  检测到RDA5807模块返回1，否则返回0
   @note    bool RDA5807_Read_ID(void)
*/
/*----------------------------------------------------------------------------*/
__root bool RDA5807_Read_ID(void) AT(RDA5807_CODE)
{	
//	iic_readn(RDA5807_RD_ADDRESS,0xff,rda5807_dat,10);
//	{
//	put_str("read 5807 id : ");
//	{
//		u8 i;
//		for(i = 0;i < 10;i++)
//		{
//			put_hex(rda5807_dat[i]);
//		}
//		put_str("\n");
//	}
//	}
	///*
	rda5807_dat[0] = 0x00;
	rda5807_dat[1] = 0x02;
	rda5807_write(2);
	delay_n10ms(1);
	rda5807_read(10);
	app_IIC_readn(RDA5807_RD_ADDRESS,0xff,(u8 *)rda5807_dat,10);

	if(read_dat[8]==0x58 && read_dat[9]==0x04)	//FM_RDA5807SP
	{
        fm_type =  3;
	}
	else if(read_dat[8]==0x58 && read_dat[9]==0x01)	  //FM_RDA5807HP
	{
        fm_type = 4;	
	}
	else if(read_dat[8]==0x58 && read_dat[9]==0x08)  //5807MP
	{
        fm_type = 7;
	}
	else if(read_dat[4]==0x58 && read_dat[5]==0x03)		//FM_RDA5807SP;
	{
        fm_type = 3;	
	}
	else
	{
        fm_type = 0;
		return 0;
	}
	return 1;
}

#endif


#define rda5807_stc() (read_dat[0] & (1 << 6))       ///<锁频结束
#define rda5807_true() (read_dat[2] & (1 << 0))      ///<是否有台
#define rda5807_rssi() ((read_dat[2] >> 1))          ///<接收信号强度
#define rda5807_st()   (read_dat[0] & (1 << 2))
/*----------------------------------------------------------------------------*/
/**@brief    RDA5807写寄存器函数
   @param    num 需要的数目
   @return   无
   @note     void rda5807_write(u8 num)
*/
/*----------------------------------------------------------------------------*/
void rda5807_write(u8 num)
{
    iic_write(RDA5807_WR_ADDRESS,0xff,(u8 *)rda5807_dat,num);
}
/*----------------------------------------------------------------------------*/
/**@brief    RDA5807读寄存器函数
   @param    需要读取的数目
   @return   无
   @note     void rda5807_read(u8 num)
*/
/*----------------------------------------------------------------------------*/
void rda5807_read(u8 num)
{
    iic_readn(RDA5807_RD_ADDRESS,0xff,(u8 *)read_dat,num);
}
/*----------------------------------------------------------------------------*/
/**@brief    RDA5807 初始化
   @param    无
   @return   无
   @note     void init_rda5807(void)
*/
/*----------------------------------------------------------------------------*/

void init_RDA5807(void)
{

    read_dat[0] = 0;
    read_dat[1] = 0;
    read_dat[2] = 0;
    read_dat[3] = 0;
    rda5807_dat[0] = 0;
    rda5807_dat[1] = 2;
    rda5807_dat[2] = 0;
    rda5807_dat[3] = 0x10;
    rda5807_dat[4] = 0x04;
    rda5807_dat[5] = 0x0;
//    rda5807_dat[6] = 0x80 | 0x6;
    rda5807_dat[6] = 0x80 | 0x0;
    rda5807_dat[7] = 0xbf;
    rda5807_dat[8] = 0x0;
    rda5807_dat[9] = 0x0;
    rda5807_dat[10] = 0x7e;
    rda5807_dat[11] = 0xc6;
    rda5807_write(2);
    delay_n10ms(40);
    rda5807_dat[0] = 0xc4;
    rda5807_dat[1] = 0x01;
    rda5807_dat[7] &=~0x0F;
    rda5807_dat[7] |= 0x07;
    rda5807_dat[0] = 0xC0;
    rda5807_write(12);
    delay_n10ms(10);
}
/*----------------------------------------------------------------------------*/
/**@brief    设置一个频点RDA5807
   @param    fre 频点  875~1080
   @return   1：当前频点有台，0：当前频点无台
   @note     bool set_fre_rda5807(u16 fre, u8 mode)
*/
/*----------------------------------------------------------------------------*/
bool set_fre_RDA5807(u16 fre)
{
    u16 pll;
    u16 i;

    pll = (fre - 870);
    rda5807_dat[2] = pll >> 2;
    rda5807_dat[3] = ((pll & 0x0003)<<6)|0x10;

    rda5807_write(4);
    delay_n10ms(5);
    i = 0;
    do
    {
        rda5807_read(4);
        i++;
        delay_n10ms(1);
        if (rda5807_true())
        {
            return 1;
        }
    }
    while (i<3);

    return 0;
}

/*----------------------------------------------------------------------------*/
/**@brief    关闭RDA5807的电源
   @param    无
   @return   无
   @note     void rda5807_poweroff(void)
*/
/*----------------------------------------------------------------------------*/
void RDA5807_PowerDown(void)
{
    rda5807_dat[1] &= ~(1<<0);
    rda5807_write(2);
    delay_n10ms(5);
}
/*----------------------------------------------------------------------------*/
/**@brief    获取RDA5807 芯片ID号
   @param    无
   @return   获取成功标志位
   @note     bool rda5807_read_id(void)
*/
/*----------------------------------------------------------------------------*/
bool RDA5807_Read_ID(void)
{
	rda5807_dat[0] = 0x00;
	rda5807_dat[1] = 0x02;
	rda5807_write(2);
	delay_n10ms(1);
	rda5807_read(10);

    if ((0x58 == read_dat[4]) || (0x58 == read_dat[6]) || (0x58 == read_dat[8]))
    {
        return TRUE;
    }
    return FALSE;
}

/*----------------------------------------------------------------------------*/
/**@brief    RDA5807 Mute函数
   @param    flag：Mute使能位
   @return   无
   @note     void rda5807_mute(u8 flag)
*/
/*----------------------------------------------------------------------------*/
void RDA5807_mute(u8 flag)
{
    if (flag)
        rda5807_dat[0] &= ~BIT(6); 	//mute
    else
        rda5807_dat[0] |= BIT(6);

    rda5807_write(2);
}
/*----------------------------------------------------------------------------*/
/**@brief    RDA5807 Mute函数
   @param    flag：Mute使能位
   @return   无
   @note     void rda5807_setch(u8 db)
*/
/*----------------------------------------------------------------------------*/
void rda5807_setch(u8 db)
{
    db=db;
}
#endif

