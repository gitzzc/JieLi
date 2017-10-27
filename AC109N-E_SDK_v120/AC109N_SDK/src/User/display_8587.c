#include "config.h"
#include "fm_radio.h"
#include "hot_msg.h"
#include "music_play.h"
#include "play_file.h"
#include "get_music_file.h"
#include "main.h"

#include "stdlib.h"
#include "bike.h"
#include "bl55072.h"

unsigned char _xdata BL_Data[19];
unsigned char _xdata flashflag = 0;
unsigned char _xdata TL_Flash = 0;
unsigned char _xdata TR_Flash = 0;

const unsigned char _code SegDataTime[10] AT(BIKE_TABLE_CODE)	= {0xFA,0x60,0xD6,0xF4,0x6C,0xBC,0xBE,0xE0,0xFE,0xFC};
const unsigned char _code SegDataVoltage[10] AT(BIKE_TABLE_CODE)= {0xF5,0x60,0xD3,0xF2,0x66,0xB6,0xB7,0xE0,0xF7,0xF6};
const unsigned char _code SegDataMile[10] AT(BIKE_TABLE_CODE)	= {0xF5,0x60,0xD3,0xF2,0x66,0xB6,0xB7,0xE0,0xF7,0xF6};
const unsigned char _code SegDataSpeed[10] AT(BIKE_TABLE_CODE)  = {0xF5,0x60,0xD3,0xF2,0x66,0xB6,0xB7,0xE0,0xF7,0xF6};

#define LCD_LEFT()	{BL_Data[14] |= 0x08;}	//S1
#define LCD_RIGHT()	{BL_Data[15] |= 0x08;}	//S10
#define LCD_V()		{BL_Data[ 7] |= 0x08;}	//S9
#define LCD_KM()	{BL_Data[ 2] |= 0x08;}	//S3
#define LCD_KMH()	{BL_Data[ 0] |= 0x08;}	//S16
#define LCD_LIGHT()	{BL_Data[13] |= 0x08;}	//S2
#define LCD_FM()	{BL_Data[ 1] |= 0x08;}	//S14
#define LCD_BT()	{BL_Data[15] |= 0x02;}	//S13
#define LCD_MP3()	{BL_Data[15] |= 0x01;}	//S12
#define LCD_RP()	{BL_Data[15] |= 0x04;}	//S11
#define LCD_VDOT()	{BL_Data[ 3] |= 0x08;}	//S17
#define LCD_FDOT()	{BL_Data[16] |= 0x01;}	//P1
#define LCD_SEC()	{BL_Data[17] |= 0x01;}	//P2
#define LCD_DEGREE()		{BL_Data[6] |= 0x08;}	//S4
#define LCD_DEGREE_NIG()	{BL_Data[6] |= 0x08;}	//S4

#define MEDIA_USB   0
#define MEDIA_SD    1
#define MEDIA_FM    2
#define MEDIA_OFF   0xFF


void MenuUpdate(BIKE_STATUS* bike) AT(BIKE_CODE)
{
	unsigned char i = 0;

	flashflag++;
	flashflag %= 10;
	
	for(i=0;i<18;i++)	BL_Data[i] = 0x00;
	
    if ( bike->bLFlashType ){
		if ( bike->bLeftFlash 	)	LCD_LEFT();
    } else {
     	if ( bike->bLeftFlash 	&& flashflag >= 5 )	LCD_LEFT();
    }
    if ( bike->bRFlashType ){
		if ( bike->bRightFlash	)	LCD_RIGHT();
    } else {
     	if ( bike->bRightFlash && flashflag >= 5 )	LCD_RIGHT();
    }
	if( bike->bNearLight 	) 		LCD_LIGHT();

    /***************************Battery Area Display**********************************/
	BL_Data[15] |= 0x80;	//T0
    BL_Data[ 8] |= 0x10;	//T10
	switch ( bike->ucBatStatus ){
    case 0:
		if ( flashflag < 5 )
			BL_Data[ 8] &=~0x10;break;	//T0
    case 1: BL_Data[ 8] |= 0x30;break;
    case 2: BL_Data[ 8] |= 0x70;break;
    case 3: BL_Data[ 8] |= 0xF0;break;
    case 4: BL_Data[ 8] |= 0xF8;break;
    case 5: BL_Data[ 8] |= 0xFC;break;
    case 6: BL_Data[ 8] |= 0xFE;break;
    case 7: BL_Data[ 8] |= 0xFF;break;
    case 8: BL_Data[ 8] |= 0xFF;BL_Data[15] |= 0x10;break;
    default:BL_Data[ 8] |= 0xFF;BL_Data[15] |= 0x30;break;
	}

	/***************************Temp Area Display**********************************/
/*	BL_Data[ 7] |= SegDataVoltage[abs(bike->Temperature/100))%10];
	BL_Data[ 6] |= SegDataVoltage[abs(bike->Temperature/10 ))%10];
	BL_Data[ 4] |= SegDataVoltage[abs(bike->Temperature%10	 	] & 0xF0);
	BL_Data[ 3] |= SegDataVoltage[abs(bike->Temperature%10	 	] & 0x0F);
	LCD_DEGREE();
	if (bike->Temperature < 0)
		LCD_DEGREE_NIG();
*/	
	/*************************** Voltage Display**********************************/
	BL_Data[ 7] |= SegDataVoltage[(bike->uiVoltage/100)%10];
	BL_Data[ 6] |= SegDataVoltage[(bike->uiVoltage/10	)%10];
	BL_Data[ 4] |= SegDataVoltage[(bike->uiVoltage	)%10] & 0xF0;
	BL_Data[ 3] |= SegDataVoltage[(bike->uiVoltage	)%10] & 0x0F;
	LCD_V();LCD_VDOT();	
		
	/***************************Time Area Display**********************************/
	if ( bike->uiPlayMedia != MEDIA_OFF )
		LCD_RP();
		
	if ( bike->uiPlayMedia == MEDIA_USB ){
		LCD_MP3();
	} else if ( bike->uiPlayMedia == MEDIA_FM ){
		LCD_FM();
	}
	
	if ( bike->bShowVol ){
		BL_Data[12] |= 0x00;
		BL_Data[11] |= 0x7A;
		BL_Data[17] |= ( SegDataTime[(bike->uiValue/10)%10] );
		BL_Data[16] |= ( SegDataTime[ bike->uiValue%10	  ] );
	} else {
		if ( bike->uiPlayMedia == MEDIA_USB ){
            if ( bike->uiShowFileNO ){
				BL_Data[12] |= SegDataTime[(bike->uiFileNO/1000		)%10];
				BL_Data[11] |= SegDataTime[(bike->uiFileNO/100		)%10];
				BL_Data[17] |= SegDataTime[(bike->uiFileNO/10		)%10];
				BL_Data[16] |= SegDataTime[ bike->uiFileNO%10			];
			} else {
				BL_Data[12] |= SegDataTime[( bike->uiPlayTime/60/10	)%10];
				BL_Data[11] |= SegDataTime[( bike->uiPlayTime/60	)%10];
				BL_Data[17] |= SegDataTime[((bike->uiPlayTime%60)/10)%10];
				BL_Data[16] |= SegDataTime[  bike->uiPlayTime%10		];
				LCD_SEC();				
			}
			if ( bike->uiPlayStatus == MAD_PAUSE ){
				BL_Data[12] = 0xCE;	//'P'
				BL_Data[11] = 0xEE;	//'A'
				BL_Data[17] = 0x7A;	//'U'
				BL_Data[16] = 0xBC;	//'S'
			} else if ( bike->bShowWait ){
				BL_Data[12] = 0x00;	//' '
				BL_Data[11] = 0x7A;	//'U'
				BL_Data[17] = 0xBC;	//'S'
				BL_Data[16] = 0x3E;	//'b'
            }
		} else if ( bike->uiPlayMedia == MEDIA_FM ){
			if ( bike->uiShowFileNO ){
				BL_Data[12] |= 0x9A;  //'C'
				BL_Data[11] |= 0x6E;  //'H'
				BL_Data[17] |= SegDataTime[(bike->uiFM_Channel/10	)%10];
				BL_Data[16] |= SegDataTime[ bike->uiFM_Channel%10		];
			} else {
                LCD_FDOT();
				BL_Data[12] |= SegDataTime[(bike->uiFM_Freq/1000	)%10];
				BL_Data[11] |= SegDataTime[(bike->uiFM_Freq/100		)%10];
				BL_Data[17] |= SegDataTime[(bike->uiFM_Freq/10		)%10];
				BL_Data[16] |= SegDataTime[ bike->uiFM_Freq%10			];
			}
		} else if ( bike->uiPlayMedia == MEDIA_OFF ){
            //BL_Data[12] |= SegDataTime[];' '
            BL_Data[11] |= 0xFA;	//'O'
            BL_Data[17] |= 0x8E;	//'F'
            BL_Data[16] |= 0x8E;	//'F'
        }
    }
	if ( bike->bPlayFlash | bike->bMute ){
		if ( flashflag < 5  ) {
			BL_Data[12] &= 0x01;
			BL_Data[11] &= 0x01;
			BL_Data[17] &= 0x00;
			BL_Data[16] &= 0x00;
		}
	}

	/*
	if ( bike->HasTimer )
	{
		BL_Data[12] |= ( SegDataTime[bike->Hour/10]);
		BL_Data[11] |= ( SegDataTime[bike->Hour%10]);
		BL_Data[17] |= ( SegDataTime[bike->Minute/10] );
		BL_Data[16] |= ( SegDataTime[bike->Minute%10] );
		if ( bike->time_set ){
			switch ( bike->time_pos ){
			case 0:if ( flashflag < 5 ) BL_Data[12] &= 0x08; break;			
			case 1:if ( flashflag < 5 ) BL_Data[11] &= 0x08; break;
			case 2:if ( flashflag < 5 ) BL_Data[17] &= 0x08; break;
			case 3:if ( flashflag < 5 ) BL_Data[16] &= 0x08; break;
			default:break;		
			}
			LCD_SEC();
		} else if ( flashflag < 5 ) LCD_SEC();	//col
	}*/
	
	/*************************** Mile Display**********************************/
	LCD_KM();	
	BL_Data[ 5] |= SegDataMile [(bike->ulMile/10000)%10] & 0xF0;
	BL_Data[ 4] |= SegDataMile [(bike->ulMile/10000)%10] & 0x0F;
	BL_Data[ 0] |= SegDataMile [(bike->ulMile/1000 )%10] & 0xF0;
	BL_Data[ 5] |= SegDataMile [(bike->ulMile/1000 )%10] & 0x0F;
	BL_Data[ 1] |= SegDataMile [(bike->ulMile/100  )%10] & 0xF0;
	BL_Data[ 0] |= SegDataMile [(bike->ulMile/100  )%10] & 0x0F;
	BL_Data[ 2] |= SegDataMile [(bike->ulMile/10   )%10] & 0xF0;
	BL_Data[ 1] |= SegDataMile [(bike->ulMile/10   )%10] & 0x0F;
	BL_Data[ 3] |= SegDataMile [(bike->ulMile   	 )%10] & 0xF0;
	BL_Data[ 2] |= SegDataMile [(bike->ulMile	     )%10] & 0x0F;
	if ( bike->bMileFlash ){
		if ( flashflag < 5  ) {
			BL_Data[ 4] &= 0xF0;
			BL_Data[ 5] &= 0x08;
			BL_Data[ 0] &= 0x08;
			BL_Data[ 1] &= 0x08;
			BL_Data[ 2] &= 0x08;
			BL_Data[ 3] &= 0x0F;
		}
	}

	/*************************** Speed Display**********************************/
	LCD_KMH();
	BL_Data[14] |= (SegDataSpeed[(bike->ucSpeed/10)	%10]);
	BL_Data[13] |= (SegDataSpeed[ bike->ucSpeed		%10]);
	if ( bike->bSpeedFlash ){
		if ( flashflag < 5  ) {
			BL_Data[14] &= 0x08;
			BL_Data[13] &= 0x08;
		}
	}

	BL_Write_Data(0,18,BL_Data);
}


void LCD_show_volume(void) AT(BIKE_CODE)
{
	bike.bLcdFresh = 0;
	bike.bShowVol = 1;
	bike.uiValue  = main_vol_L;
	if ( bike.uiValue > 30 )
		bike.uiValue = 30;
	bike.bLcdFresh = 1;
}
/*
void LCD_show_dev(void) AT(BIKE_CODE)
{
    //Music Device type
    if (device_active == DEVICE_UDISK)
		bike.uiPlayMedia = MEDIA_USB;
#ifdef SDMMCB_EN
    else if ((device_active == DEVICE_SDMMCA)||(device_active == DEVICE_SDMMCB))
#elif defined SDMMCA_EN
    if (device_active == DEVICE_SDMMCA)
#endif
		bike.uiPlayMedia = MEDIA_SD;
}*/

void LCD_show_music_wait(void) AT(BIKE_CODE)
{
	bike.bLcdFresh = 0;
	bike.uiPlayMedia = MEDIA_USB;
 	bike.bShowWait = 1;
	bike.bLcdFresh = 1;
}

void LCD_show_music_main(void) AT(BIKE_CODE)
{
    u16 play_time;

	bike.bLcdFresh = 0;
    /*Music Play time info*/
    bike.uiPlayTime = get_music_play_time();
	bike.bShowVol = 0;
 	bike.bShowWait = 0;
    if( bike.uiShowFileNO ) bike.uiShowFileNO--;
	bike.uiPlayMedia = MEDIA_USB;

    //LCD_show_dev();
    bike.uiPlayStatus = Music_Play_var.bPlayStatus;
	if ( bike.uiPlayStatus == MAD_PAUSE ){
		bike.bPlayFlash = 1;
	} else if ( bike.uiPlayStatus == MAD_PLAY ){
		bike.bPlayFlash = 0;
	}
	bike.bLcdFresh = 1;
}

void LCD_show_fm_main(void) AT(BIKE_CODE)
{
	bike.bLcdFresh = 0;
    /*FM - Frequency*/
 	bike.bShowVol = 0;
    if( bike.uiShowFileNO ) bike.uiShowFileNO--;
 	bike.uiPlayMedia = MEDIA_FM;
    bike.uiFM_Freq = fm_mode_var.wFreq;
	bike.uiFM_Channel = fm_mode_var.bFreChannel;
    //deg("LCD_show_fm_main %u %u\n",bike.uiPlayMedia,bike.uiFM_Freq);
	bike.bLcdFresh = 1;
}

void LCD_show_fm_station(void) AT(BIKE_CODE)
{
	bike.bLcdFresh = 0;
    /*FM - Station*/
	bike.uiFM_Channel = fm_mode_var.bFreChannel;
 	bike.bShowVol = 0;
    bike.uiShowFileNO = 2;  //2s
	bike.bLcdFresh = 1;
}

void LCD_show_file_number(void) AT(BIKE_CODE)
{
	bike.bLcdFresh = 0;
    /*Music File Number info*/
	bike.uiPlayTime = 0;
	bike.bShowVol = 0;
    bike.uiShowFileNO = 2;  //2s
    bike.uiFileNO = playfile.given_file_number;
	bike.bLcdFresh = 1;
}

void LCD_mute(void)  AT(BIKE_CODE)
{
  bike.bMute = 1;
}

void LCD_unmute(void)  AT(BIKE_CODE)
{
  bike.bMute = 0;
}

void LCD_stop(void) AT(BIKE_CODE)
{
	bike.bLcdFresh = 0;
 	bike.uiPlayMedia = MEDIA_OFF;
    bike.bMute = 0;
    bike.bPlayFlash = 0;
	bike.bLcdFresh = 1;
}
