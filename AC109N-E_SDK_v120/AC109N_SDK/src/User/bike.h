/**
  ******************************************************************************
  * @file
  * @author  ahzzc@sina.com
  * @version V2.xx
  * @date
  * @brief
  ******************************************************************************
  * @Changlog
  * V1.00
  *
  ******************************************************************************
  */
/******************************************************************************/

#ifndef __BIKE_H__
#define __BIKE_H__

/******************************************************************************/

//#define SINGLE_TRIP	
//#define LCD_SEG_TEST
#define PCB_VER		0100
#define LCD9040
#define TIME_ENABLE 0
#define YXT_ENABLE  0				
#define RESET_MILE_ENABLE

#define BIKE_EEPROM_START   0x80

#ifndef PCB_VER
	#define PCB_VER	0100
#endif

#ifndef TIME_ENABLE
	#define TIME_ENABLE 	0				
#endif

#ifndef YXT_ENABLE
	#define YXT_ENABLE      0				
#endif

/******************************************************************************/
#define VOL_CALIBRATIOIN	600UL	//60.0V
#define TEMP_CALIBRATIOIN	250UL	//25.0C
#define SPEED_CALIBRATIOIN	30UL	//30km/h

#define PON_ALLON_TIME		2000UL	//1000ms

#define DISPLAY_MAX_SPEED	45UL	//40km/h
#define SPEEDMODE_DEFAULT	1		//1æ¡?

/******************************************************************************/
typedef struct {
	unsigned char NearLight	:1;
	unsigned char CRZLight	:1;
	unsigned char Cruise	:1;
	unsigned char ECUERR	:1;
	unsigned char Braked	:1;
	unsigned char PhaseERR	:1;
	unsigned char HallERR	:1;
	unsigned char WheelERR	:1;
	unsigned char YXTERR	:1;
	unsigned char HasTimer	:1;
	unsigned char time_set	:1;
	unsigned char uart		:1;	
	unsigned char HotReset	:1;	
	unsigned char MileFlash	:1;	
	unsigned char SpeedFlash:1;	
	unsigned char BT		:1;	
	unsigned char MP3		:1;	
	unsigned char FM		:1;	
	unsigned char bShowVol	:1;	
    unsigned char bShowWait	:1;
    unsigned char bPlayFlash:1;
    unsigned char bMute		:1;
    unsigned char bLRFlashType	:1;
    unsigned char bLeftFlash	:1;	
    unsigned char bRightFlash	:1;	
    unsigned char bTurnLeft		:1;
    unsigned char bTurnRight	:1;

	unsigned char SpeedMode;
			 int  Temperature;
	unsigned int  Voltage;
	unsigned char BatStatus;
	unsigned char Energy;
	unsigned char Speed;
	unsigned char PHA_Speed;
	unsigned char YXT_Speed;
			 char Speed_dec;
	unsigned char SpeedAdj;
	unsigned long Mile;
	unsigned long FMile;
	unsigned int  Tick;
	unsigned int  uiPlayTime;
	unsigned int  uiShowFileNO;
	unsigned int  uiPlayMedia;
	unsigned int  uiValue;
	unsigned int  uiFileNO;
	unsigned int  uiFM_Freq;
	unsigned int  uiFM_Channel;
	unsigned int  uiPlayStatus;
	
	unsigned char Hour;
	unsigned char Minute;
	unsigned char Second;
	unsigned char time_pos;
	
} BIKE_STATUS,*pBIKE_STATUS;
	
typedef struct {
	unsigned char bike[4];
	unsigned int  SysVoltage;
	unsigned int  VolScale	;
	unsigned int  TempScale	;
	unsigned int  SpeedScale;
	unsigned int  YXT_SpeedScale;
	unsigned long Mile;
	unsigned char Sum;
} BIKE_CONFIG,*pBIKE_CONFIG;
	
extern BIKE_STATUS bike;
extern BIKE_CONFIG config;

unsigned int Get_SysTick(void);
unsigned int Get_ElapseTick(unsigned int pre_tick);
void bike_task(void);
void LRFlash_Task(void);

/******************************************************************************/

#endif
