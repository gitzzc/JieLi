/**
  ******************************************************************************
  * @file
  * @author  ahzzc@sina.com
  * @version V1.xx
  * @date
  * @brief
  ******************************************************************************
  * @Changlog
  *
  * V1.21 - 20191118
  * ����BIKE_48_60_FM_BANPENG���Ͷ��壬60Vϵͳ��������ѹ25V����Ӧ���ٶ���40km/h��
  *
  * V1.20 - 20191016
  * ����STARTUP_OFF_MODE���壬ѡ�񿪻����Ƿ�������ģʽ��
  *
  * V1.19 - 20191014
  * �޸�ICSET�Ĵ������ã�����BL55072AоƬ��
  *
  * V1.18��-��20180928
  * ����60V��72V��ѹ�汾��ͨ��VOL6072����ѡ��
  *
  * V1.17��-��20180224
  * �޸�60Vϵͳ������ʾ��
  *
  * V1.16��-��20171103
  * �޸�FMоƬ�𻵺�̶���OFFģʽ�����⡢OFFģʽ�ػ��󣬲���U����������MP3ģʽ��U���ڸ����Ƚ϶�ʱ��FM�л���MP3ģʽ��ʱ��ȴ������⣻
  *
  * V1.15��-��20171103
  * ���ٿ���ȫ��ʱ�䣬���ٵ�ѹ�궨��ʱ��
  *
  * V1.14��-��20171103
  * ���ӵ�ѹ�궨���ܣ�ͬʱ���������Ϣ��
  *
  * V1.13��-��20171103
  * ���ӵ�������������ù��ܣ�
  *
  * V1.12��-��20171103
  * �����ٶȵ�����ʽ��������������⣬
  *
  * V1.11��-��20171031
  * �޸�MP3������ͣʱ�γ����̺�״̬��ͬ�������⣬��ͣʱ����˸��
  *
  * V1.10��-��20171031
  * �޸İ�����������,RADIO&PLYAERͼ��ʼ����ʾ
  *
  * V1.09��-��20171029
  * FM����������ǰ���о���������FM����ʱ����U��״̬δ���µ����⣬������FM����ʱ����ʹ��AB��ģʽ��CLASSIC��Ч��
  *
  * V1.08��-��20171029
  * ������ѹ��ʾ���ٶȵ���������̵�����
  *
  * V1.04
  * �����ٶȵ�������
  *
  * V1.00
  * ����ת�źſ��غ��������Զ�ʶ�𣬵�ѹ�����˲���
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

//#define VOL6072
//#define STARTUP_OFF_MODE

/******************************************************************************/
#define BIKE_48_60_FM_BANPENG	//48/60Fm����
/******************************************************************************/

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
#define SPEEDMODE_DEFAULT	1		//1�?

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
