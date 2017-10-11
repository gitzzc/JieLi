/*--------------------------------------------------------------------------*/
/**@file     KT0830EG.c
   @brief    KT0830EG收音底层驱动
   @details
   @author
   @date   2011-9-8
   @note
*/
/*----------------------------------------------------------------------------*/
#include "FM_API.h"

#ifdef	KT0830EG

#include "KT0830EG.h"
#include "iic.h"
#include "device.h"
#include "play_file.h"
#include "sdmmc_api.h"

//FM_KT0830_VAR _xdata FM_kt0830_var;
u8  _idata mem_afc[3];					//Remembered afc values for previous, current and next stations
u16 _idata mem_freq[3];				    //Remembered channel frequencies for previous, current and next stations
u8  _idata mem_vol;						//Remembered volume before mute


#ifdef SEEK_WITH_SNR
u8 _idata mem_snr[3];					//Remembered SNR values for previous, current and next stations
#endif

#ifdef ULTRA_LOW_Q
int CAP_OFFSET;
#endif

/*----------------------------------------------------------------------------*/
/**@brief   标准IIC总线向一个目标ID读取几个数据
   @param   address : 目标ID
   @param   *iic_dat     :  存档读取到的数据的buffer指针
   @param   n :  需要读取的数据的个数
   @return  无
   @note    void iic_readKT(u8 chip_id,u8 iic_addr,u8 *iic_dat,u8 n)
*/
/*----------------------------------------------------------------------------*/
void iic_readKT(u8 chip_id,u8 iic_addr,u8 *iic_dat,u8 n) AT(KT0830_CODE) 
{
    iic_busy = 1;
    iic_start();                	//I2C启动
    iic_sendbyte(chip_id & 0xfe);   //写命令
    iic_sendbyte(iic_addr);         //写地址
    iic_start();                	//I2C启动
    iic_sendbyte(chip_id);     	    //写地址
    for (;n>1;n--)
    {
        *iic_dat++ = iic_revbyte(0); //读数据
    }
    *iic_dat++ = iic_revbyte(1);
    iic_stop();                 	//I2C停止时序
    iic_busy = 0;
}

void sd_iic_readKT(u8 chip_id,u8 iic_addr,u8 *iic_dat,u8 n) AT(KT0830_CODE) 
{
    u8 p_status;
    
    /*Wait SD I/O release*/
    if (device_active == DEVICE_SDMMCA)
        p_status = pause_decode();
    sdmmca_force_idle();
    sdmmcb_force_idle();
    while(check_sd_controller());   //根据SD ID等待控制器繁忙 
    IO_MC0 &= ~BIT(3);

    /*IIC Communicate*/
    iic_readKT(chip_id, iic_addr, iic_dat, n);
    
    /*Reset SD I/O*/
    IO_MC0 |= BIT(3); 
    unlock_cmd_chk();     
    if ((p_status == MAD_PLAY)&&(device_active == DEVICE_SDMMCA))
        start_decode();    
}

/*----------------------------------------------------------------------------*/
/**@brief    KT0830 写寄存器函数
   @param    Register_Address：寄存器地址   Temp：写入数据
   @return   无
   @note     void KT0830EG_Write(u8 Register_Address, u16 Temp)
*/
/*----------------------------------------------------------------------------*/
void KT0830EG_Write(u8 Register_Address, u16 Temp) AT(KT0830_CODE) 
{
    u8 pTemp[2];
    
    pTemp[0] = ((u8 *)(&Temp))[0];
    pTemp[1] = ((u8 *)(&Temp))[1];
    app_IIC_write(KTFMw_address, Register_Address, pTemp, 2);
}

/*----------------------------------------------------------------------------*/
/**@brief    KT0830 写寄存器函数
   @param    Register_Address：寄存器地址
   @return   无
   @note     u16 KT0830EG_Read(u8 Register_Address)
*/
/*----------------------------------------------------------------------------*/
u16 KT0830EG_Read(u8 Register_Address) AT(KT0830_CODE) 
{
    u8 pTemp[2];
    u16 Temp;
    
    KT_IIC_readn(KTFMr_address, Register_Address, pTemp, 2);
    ((u8 *)(&Temp))[0] = pTemp[0];
    ((u8 *)(&Temp))[1] = pTemp[1];
    return Temp;
}



/***************************************************************************
@Function: KT0830EG initialization
@Parameter: NON
@Return: 0->Fail,1->Success
@Note: u8 KT_FMInit(void)
****************************************************************************/
u8 KT_FMInit(void) AT(KT0830_CODE) 
{
    u16 regx;
    u8 i;

    for (i=0;i<INIT_FAIL_TH;i++)
    {
        delay_n10ms(50);
        regx = KT0830EG_Read(0x12);						//Read power-up indicator
        if ((regx & 0x8800) != 0x8800)
            continue;
        break;
    }

    if (INIT_FAIL_TH == i)
        return(0);

    KT0830EG_Write(0x0a, 0x0000);						//Turn On AFC function
    KT0830EG_Write(0x02, 0x2207);						//Write Frequency Space to 100KHz

    if ((KT0830EG_Read(0x1D) & 0x0080) == 0x0080)		//if bit7==1, KT0830EG run patch	 /////problem:12 pin reset
    {
        // RFAGC patch (V2.2 and V3.4)
        regx = KT0830EG_Read(0x22);
        KT0830EG_Write(0x22, regx | 0x1000);			//Configure RFAGC parameter of agc_filter_dis=1

        regx = KT0830EG_Read(0x21);
        KT0830EG_Write(0x21, (regx & 0xfff8)| 0x0003);	//Configure RFAGC parameter of peaklowth=30mV

        regx = KT0830EG_Read(0x09);
        KT0830EG_Write(0x09, (regx & 0x1fff)| 0x2000);	//Configure RFAGC parameter of peakhighth=90mV

        // LOLDO patch (V2.8)
        regx = KT0830EG_Read(0x0A);
        KT0830EG_Write(0x0A, regx | 0x0400);			//loldo_ctrl=highest

        // SDMCODE patch (V3.7)
        while ((KT0830EG_Read(0x10)&0x4000) != 0)		//Wait cali-done signal
        {
            delay_n10ms(5);
        }
        KT0830EG_Write(0x10,0x5200);					//Recali sdmcode
        delay_n10ms(1);
        regx = KT0830EG_Read(0x11);
        if ((regx&0x000f)>0x0A)
        {
            KT0830EG_Write(0x11,(regx&0xfff0)|0x000A);	//sdm_code patch
        }
    }

#ifdef AUDIO_LEVEL_BOOST
    regx=KT0830EG_Read(0x23);
    KT0830EG_Write(0x23, regx | 0x0008);				// AU_GAIN=1
#endif

    KT_FMAntTuneOn();

    return(1);
}

/************************************************************************
@Function: The chip will tune the antenna automatically
		   whenever TUNE function is performed
@Parameter:	NON
@Return: NON
@Note:  void KT_FMAntTuneOn(void)
*************************************************************************/
void KT_FMAntTuneOn(void) AT(KT0830_CODE) 
{
    u16 regx;
    regx = KT0830EG_Read(0x23);
    KT0830EG_Write(0x23, regx | 0x0004);	//antenna tuning enable
    regx = KT0830EG_Read(0x1D);
    KT0830EG_Write(0x1D, (regx & 0xff7f)|((3-INDUCTOR_Q)<<5));	//cali loop disable =0
}

/******************************************************************
@Function: 	Mute the chip and Tune to Frequency
@Paramteer: Frequency:87.5MHz-->Frequency=8750;
@Return:
@Note: void KT_FMTune(u16 Frequency)
*******************************************************************/
void KT_FMTune(u16 Frequency) AT(KT0830_CODE) 
{
    u16 channel;

    channel = (Frequency-6400)/5;
//	KT_FMMute(1);
    KT0830EG_Write(0x03, 0x8800 | channel);		//set tune bit to 1
    delay_n10ms(5);
}

/*****************************************************************************
@Function: Read RSSI From KT0830EG
@Parameter: RSSI:range from -100 to -6, unit is dbm
@Return: non
@Note: void KT_FMReadRSSI(char *RSSI)
*****************************************************************************/
void KT_FMReadRSSI(char *RSSI) AT(KT0830_CODE) 
{
    u16 regx;
    regx = KT0830EG_Read(0x12);
    *RSSI = -(100 - ((( regx >> 3 ) & 0x001F ) * 3 ));
}

#ifdef SEEK_WITH_SNR
/*****************************************************************************
@Function: Read SRN From KT0830EG
@Parameter: NON
@Return:
@Note: u8 KT_FMGetSNR(void)
*****************************************************************************/
u8 KT_FMGetSNR(void) AT(KT0830_CODE) 
{
    u16 regx;
    regx = KT0830EG_Read(0x1F);
    return (regx & 0x00FF);
}
#endif

/*****************************************************************************
@Function: returned value's range is from -127 to 127, unit is KHz
@Parameter: NON
@Return: non
@Note: void KT_FMReadRSSI(char *RSSI)
*****************************************************************************/
u8 KT_FMGetAFC(void) AT(KT0830_CODE) 
{
    u8 afc_delta;
    u16 reg14;
    reg14 = KT0830EG_Read(0x14);								// read AFC_DELTAF
    afc_delta = reg14 & 0x003f;
    if (afc_delta & 0x20)
    {
        afc_delta = afc_delta-32;
    }
    else
    {
        afc_delta = afc_delta+32;
    }
    return (afc_delta);
}

/*----------------------------------------------------------------------------*/
/**@brief    设置一个频点KT0830
   @param    fre 频点  875~1080
   @return   1：当前频点有台，0：当前频点无台
   @note     bool set_fre_KT0830(u16 Frequen)
*/
/*----------------------------------------------------------------------------*/
__root bool set_fre_KT0830(u16 Frequen) AT(KT0830_CODE) 
{
//	KT_FMTune(Frequency);
    u16 Frequency;
    u16 nextfreq;
    u8  afc[3];								//AFC value for previous, current and next channels
    u16 freq[3];							//frequency values for previous, current and next channels

#ifdef SEEK_WITH_SNR
    u8 snr[3];								//SNR for previous, current and next channels
    u8 snr2,snr3;
#endif

    char i,j;

    Frequency = Frequen * 10;

    afc[0]=0;
    afc[1]=0;
    afc[2]=0;				//initialize
    freq[0]=0;
    freq[1]=0;
    freq[2]=0;			//initialize

#ifdef SEEK_WITH_SNR
    snr[0]=0;
    snr[1]=0;
    snr[2]=0;				//initialize
#endif

    nextfreq=Frequency - CHANNEL_STEP;

    //Get AFC values of previous, current and next channels
    for (i=0;i<3;i++)
    {
        //get AFC values for the last station's information
        for (j=0;j<3;j++)
        {
            if (mem_freq[j] == nextfreq)
            {
                afc[i] = mem_afc[j];
                freq[i] = mem_freq[j];

#ifdef SEEK_WITH_SNR
                snr[i] = mem_snr[j];
#endif
            }
        }

        //get new AFC value if the station information is not investigated in the last run
        if (!freq[i])
        {
            KT_FMTune(nextfreq);
            afc[i] = KT_FMGetAFC();
#ifdef SEEK_WITH_SNR
            snr[i] = KT_FMGetSNR();
#endif
            freq[i] = nextfreq;
        }
        nextfreq += CHANNEL_STEP;
    }

    //Record AFC values
    mem_afc[0]=afc[0];
    mem_afc[1]=afc[1];
    mem_afc[2]=afc[2];
    mem_freq[0]=freq[0];
    mem_freq[1]=freq[1];
    mem_freq[2]=freq[2];

#ifdef SEEK_WITH_SNR
    mem_snr[0]=snr[0];
    mem_snr[1]=snr[1];
    mem_snr[2]=snr[2];
#endif

    //Determine the validation of current station
    if ((afc[0]<afc[1]) && (afc[1]<afc[2]) && (afc[0]<(32-AFCTH_PREV))
            && (afc[1]>(32-AFCTH)) && (afc[1]<(AFCTH+32)) && (afc[2]>(AFCTH_NEXT+32)))
    {

#ifdef SEEK_WITH_SNR
        KT_FMTune(Frequency);
        snr2 = KT_FMGetSNR();
        if ((snr[1] >= SNR_TH) && (snr2 >= SNR_TH))
        {
            return(1);
        }
        if ((snr[1] < SNR_TH) && (snr2 < SNR_TH))
        {
            return(0);
        }
        delay_n10ms(5);
        snr3 = KT_FMGetSNR();
        if (snr3 >= SNR_TH)
        {
            return(1);
        }
        else
        {
            return(0);
        }

#endif

    }
    return(0);
}

/*----------------------------------------------------------------------------*/
/**@brief    FM 模块静音控制
   @param    dir：1-mute 0-unmute
   @return   无
   @note     void KT0830_mute(u8 flag)
*/
/*----------------------------------------------------------------------------*/
__root void KT0830_mute(u8 flag) AT(KT0830_CODE) 
{
    u16 regx;
    
    regx = KT0830EG_Read(0x04);
    if (flag)
    {
        KT0830EG_Write(0x04, regx & 0xFFF0 );		//Write volume to 0
    }
    else
    {
        KT0830EG_Write(0x04, regx & 0xfff0 | 0x000e);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief    KT0830 初始化
   @param    无
   @return   无
   @note     void init_KT0830(void)
*/
/*----------------------------------------------------------------------------*/
__root void init_KT0830(void) AT(KT0830_CODE) 
{
    u16 regx;
    /*对于32.768KHz的共晶振与独立晶振，在应用IDE调试时，须加上，在SOCKY座上不用*/

    regx=KT0830EG_Read(0x0f);
    KT0830EG_Write(0x0F, regx | 0x4000);
    delay_n10ms(10);

    KT0830EG_Write(0x0F, 0x8A00);				//Write Standby bit to 0

    KT_FMInit();
    KT0830_mute(0);
}

/*----------------------------------------------------------------------------*/
/**@brief    关闭 KT0830的电源
   @param    无
   @return   无
   @note     void KT0830_PowerDown(void)
*/
/*----------------------------------------------------------------------------*/
__root void KT0830_PowerDown(void) AT(KT0830_CODE) 
{
    KT0830EG_Write(0x0F, 0x9A00);      			//Write Standby bit to 1
    delay_n10ms(2);
}

/*----------------------------------------------------------------------------*/
/**@brief   FM模块检测，获取KT0830 模块ID
   @param   无
   @return  检测到KT0830模块返回1，否则返回0
   @note    bool KT0830_Read_ID(void)   
*/
/*----------------------------------------------------------------------------*/
__root bool KT0830_Read_ID(void) AT(KT0830_CODE) 
{
    u16 regx;
    u8  i;
    
    for (i = 0; i < INIT_FAIL_TH; i++)
    {
        delay_n10ms(1);
        regx = KT0830EG_Read(0x00);			//Read Manufactory ID
        if (regx != 0xb002)
            continue;
        break;
    }
    if (i == INIT_FAIL_TH)
    {
        return(0);
    }
    if (0xb002 == regx)
    {
        return TRUE;
    }
    return FALSE;
}
#endif
