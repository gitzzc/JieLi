/**
  ******************************************************************************
  * @file    
  * @author  ahzzc@sina.com
  * @version V2.xx
  * @date    
  * @brief   
  ******************************************************************************
  * @Changlog
  * V2.00
	* 功能：
	* 1.系统电压通过MODE1,MODE2自动检测；
	* 2.时钟功能自动检测，无芯片不显示；
	* 3.通过左右转向进行时间调整模式；
	* 4.通过串口可进行参数标定,	短接低速、SWIM信号进行参数标定；
	* 5.参数保存于EEPROM;
	* 6.一线通功能；
	* 7.档位信息优先读取一线通数据，实现四档信息判断，一线通中断后采用档把数据；
	* 8.开启开门狗功能；
	* 9.通过PCB_VER定义不同的硬件版本，支持0011、0012、0022、0041；
	*10.通过YXT_XX定义不同的控制器版本；
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
	

    
#ifndef PCB_VER
	#define PCB_VER	0100
#endif

#ifndef TIME_ENABLE
	#define TIME_ENABLE 	0				
#endif

#ifndef YXT_ENABLE
	#define YXT_ENABLE      0				
#endif

#define BIKE_TEMP_ADC	0
#define BIKE_VOL_ADC	0
#define BIKE_SPEED_ADC	0


/******************************************************************************/
#define VOL_CALIBRATIOIN	600UL	//60.0V
#define TEMP_CALIBRATIOIN	250UL	//25.0C
#define SPEED_CALIBRATIOIN	30UL	//30km/h

#define PON_ALLON_TIME		2000UL	//1000ms

#define DISPLAY_MAX_SPEED	45UL	//40km/h
#define SPEEDMODE_DEFAULT	1		//1档

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
	unsigned char VOL		:1;	
	
	unsigned char TurnLeft;
	unsigned char TurnRight;
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
	unsigned int  PlayTime;
	
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

/******************************************************************************/

#endif
