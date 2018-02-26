/**
  ******************************************************************************
  * @file
  * @author  ahzzc@sina.com
  * @version V1.xx
  * @date
  * @brief
  ******************************************************************************
  * @Changlog
  * V1.17　-　20180224
  * 修改60V系统电量显示；
  *
  * V1.16　-　20171103
  * 修改FM芯片损坏后固定有OFF模式的问题、OFF模式关机后，插入U盘启动切入MP3模式、U盘内歌曲比较多时从FM切换到MP3模式长时间等待的问题；
  *
  * V1.15　-　20171103
  * 减少开机全显时间，减少电压标定用时；
  *
  * V1.14　-　20171103
  * 增加电压标定功能，同时清除配置信息；
  *
  * V1.13　-　20171103
  * 增加单次里各参数设置功能；
  *
  * V1.12　-　20171103
  * 更改速度调整方式，静音的相关问题，
  *
  * V1.11　-　20171031
  * 修改MP3播放暂停时拔出Ｕ盘后状态不同步的问题，暂停时不闪烁；
  *
  * V1.10　-　20171031
  * 修改按键音的问题,RADIO&PLYAER图标始终显示
  *
  * V1.09　-　20171029
  * FM按键操作提前进行静音、改正FM静音时插入U盘状态未更新的问题，增加了FM播放时功放使用AB类模式、CLASSIC音效；
  *
  * V1.08　-　20171029
  * 改正电压显示，速度调整，清里程的问题
  *
  * V1.04
  * 增加速度调整功能
  *
  * V1.00
  * 左右转信号开关和闪光器自动识别，电压两级滤波。
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
#define VOL_CALIBRATIOIN	500UL	//50.0V
#define TEMP_CALIBRATIOIN	250UL	//25.0C
#define SPEED_CALIBRATIOIN	30UL	//30km/h

#define PON_ALLON_TIME		1500UL	//2000ms

#define DISPLAY_MAX_SPEED	99UL	//40km/h
#define SPEEDMODE_DEFAULT	1		//1妗?

/******************************************************************************/
typedef struct {
    unsigned char bLFlashType	:1;
    unsigned char bRFlashType	:1;
    unsigned char bLeftFlash	:1;	
    unsigned char bRightFlash	:1;	
    unsigned char bTurnLeft		:1;
    unsigned char bTurnRight	:1;

	unsigned char bHotReset		:1;	
	unsigned char bNearLight	:1;
	unsigned char bLastNear		:1;
	unsigned char bBraked		:1;
	unsigned char bCruise		:1;
	unsigned char bMileFlash	:1;	
	unsigned char bSpeedFlash	:1;	
	unsigned char bVolFlash		:1;	

	unsigned char bECUERR		:1;
	unsigned char bPhaseERR		:1;
	unsigned char bHallERR		:1;
	unsigned char bWheelERR		:1;
	unsigned char bYXTERR		:1;
	unsigned char bHasTimer		:1;
	unsigned char bTimeSet		:1;
	unsigned char bUart			:1;	

	unsigned char bBT			:1;	
	unsigned char bMP3			:1;	
	unsigned char bFM			:1;	
	unsigned char bShowVol		:1;	
    unsigned char bShowWait		:1;
    unsigned char bPlayFlash	:1;
    unsigned char bMute			:1;

	unsigned char ucSpeedMode;
	signed int    siTemperature;
	unsigned int  uiVoltage;
	unsigned char ucBatStatus;
	unsigned char ucEnergy;
	unsigned char ucSpeed;
	unsigned char ucPHA_Speed;
	unsigned char ucYXT_Speed;
	unsigned long ulMile;
	unsigned long ulFMile;
	unsigned int  uiTick;
	unsigned int  uiPlayTime;
	unsigned int  uiShowFileNO;
	unsigned int  uiShowChannel;
	unsigned int  uiPlayMedia;
	unsigned int  uiValue;
	unsigned int  uiFileNO;
	unsigned int  uiFM_Freq;
	unsigned int  uiFM_Channel;
	unsigned int  uiPlayStatus;
	
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned char ucTimePos;
	
} BIKE_STATUS,*pBIKE_STATUS;
	
typedef struct {
	unsigned char ucBike[4];
	unsigned int  uiSysVoltage;
	unsigned int  uiVolScale;
	unsigned int  uiTempScale;
	unsigned int  uiSpeedScale;
	unsigned int  uiYXT_SpeedScale;
	unsigned int  uiSingleTrip;
	unsigned long ulMile;
	unsigned char ucSum;
} BIKE_CONFIG,*pBIKE_CONFIG;
	
extern BIKE_STATUS sBike;
extern BIKE_CONFIG sConfig;

unsigned int Get_SysTick(void);
unsigned int Get_ElapseTick(unsigned int pre_tick);
void bike_task(void);
void LRFlashTask(void);
void BikePowerUp(void);
void GetVolSample(void);
void BikeCalibration(void);

/******************************************************************************/

#endif
