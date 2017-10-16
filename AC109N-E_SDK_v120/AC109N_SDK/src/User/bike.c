
#include "config.h"
#include "key.h"

#include "bl55072.h"
#include "display.h"
#include "bike.h"

#define ContainOf(x) (sizeof(x)/sizeof(x[0]))

const unsigned int  BatStatus48[] AT(BIKE_TABLE_CODE) = {431,436,441,446,451,456,461,466,471,476,481};
const unsigned int  BatStatus60[] AT(BIKE_TABLE_CODE) = {520,531,544,556,568,570,575,580,585,590,595};
const unsigned int  BatStatus72[] AT(BIKE_TABLE_CODE) = {630,642,653,664,675,687,700,715};

unsigned int _xdata tick_100ms=0;
unsigned int _xdata speed_buf[4];
unsigned int _xdata vol_buf[4];
unsigned int _xdata temp_buf[4];

BIKE_STATUS _xdata bike;
__no_init BIKE_CONFIG _xdata config;


unsigned int Get_SysTick(void) AT(BIKE_CODE)
{
	unsigned int tick;
	
	EA = 0;
	tick = bike.Tick;
	EA = 1;
	
	return tick;
}

unsigned int Get_ElapseTick(unsigned int pre_tick) AT(BIKE_CODE)
{
	unsigned int tick = Get_SysTick();

	if ( tick >= pre_tick )	
		return (tick - pre_tick); 
	else 
		return (0xFFFF - pre_tick + tick);
}

const long NTC_B3450[29][2] = 
{
	251783,	-400,	184546,	-350,	137003,	-300,	102936,	-250,	78219,	-200,
	60072,	-150,	46601,	-100,	36495,	-50,	28837,	0,		22980,	50,
	18460,	100,	14942,	150,	12182,	200,	10000,	250,	8263,	300,
	6869,	350,	5745,	400,	4832,	450,	4085,	500,	3472,	550,
	2965,	600,	2544,	650,	2193,	700,	1898,	750,	1649,	800,
	1439,	850,	1260,	900,	1108,	950,	977,	1000
};

long NTCtoTemp(long ntc) AT(BIKE_CODE)
{
	unsigned char i,j;

	if ( ntc > NTC_B3450[0][0] ){
		return NTC_B3450[0][1];
	} else {
		for(i=0;i<sizeof(NTC_B3450)/sizeof(NTC_B3450[0][0])/2-1;i++){
			if ( ntc <= NTC_B3450[i][0] && ntc > NTC_B3450[i+1][0] )
				break;
		}
		if ( i == sizeof(NTC_B3450)/sizeof(NTC_B3450[0][0])/2-1 ){
			return NTC_B3450[28][1];
		} else {
			for(j=0;j<50;j++){
				if ( NTC_B3450[i][0] - (j*(NTC_B3450[i][0] - NTC_B3450[i+1][0])/50) <= ntc )
					return NTC_B3450[i][1] + j;
			}
			return NTC_B3450[i+1][1];
		}
	}
}

int GetTemp(void) AT(BIKE_CODE)
{
	static unsigned char index = 0;
	long temp;
	unsigned char i;

	temp = AD_var.wADValue[AD_BIKE_TEMP]>>6;
  
	temp_buf[index++] = temp;
	if ( index >= ContainOf(temp_buf) )
		index = 0;
	for(i=0,temp=0;i<ContainOf(temp_buf);i++)
		temp += temp_buf[i];
	temp /= ContainOf(temp_buf);

	temp = 10000*1024UL/(1024-temp)-10000;
	temp = NTCtoTemp(temp);
	if ( temp > 999  ) temp =  999;
	if ( temp < -999 ) temp = -999;
	
	return temp;
}

unsigned char GetVolStabed(unsigned int* vol) AT(BIKE_CODE)
{
	unsigned long mid;
	static int buf[32];
	static unsigned char index = 0;
	unsigned char i;
	
	buf[index++] = AD_var.wADValue[AD_BIKE_VOL]>>6;
	if ( index >= 32 )	index = 0;
	
	*vol = (unsigned long)(AD_var.wADValue[AD_BIKE_VOL]>>6)*693UL/1024UL+400;

    //deg("GetVolStabed %u\n",AD_var.wADValue[AD_BIKE_VOL]>>6);

	for(i=0,mid=0;i<32;i++)	mid += buf[i];
	mid /= 32;
	for( i=0;i<32;i++){
		//if ( mid > 5 && ((mid *100 / buf[i]) > 101 ||  (mid *100 / buf[i]) < 99) )
		if ( mid > 5 && ((mid *100 / buf[i]) > 105 ||  (mid *100 / buf[i]) < 95) )
			return 0;
	}
	
	return 1;
}

unsigned char GetSpeed(void) AT(BIKE_CODE)
{
	static unsigned char index = 0;
	unsigned int speed;
	unsigned char i;

	speed = AD_var.wADValue[AD_BIKE_SPEED]>>6;
    //deg("GetSpeed %u\n",AD_var.wADValue[AD_BIKE_SPEED]>>6);
 	
	speed_buf[index++] = speed;
	if ( index >= ContainOf(speed_buf) )
		index = 0;

	for(i=0,speed=0;i<ContainOf(speed_buf);i++)
		speed += speed_buf[i];
	speed /= ContainOf(speed_buf);
	
	if ( config.SysVoltage	== 48 ){	// speed*5V*21/1024/24V*45 KM/H
			speed = (unsigned long)speed*1925UL/8192UL;		//24V->55KM/H
	} else if ( config.SysVoltage	== 60 ) {	// speed*5V*21/1024/30V*45 KM/H
			speed = (unsigned long)speed*385/2048;			//30V->55KM/H
	} else if ( config.SysVoltage	== 72 )	{// speed*5V*21/1024/36V*45 KM/H
			speed = (unsigned long)speed*1925UL/12288UL;	//36V->55KM/H
	}
	if ( speed > 99 )
		speed = 99;
	
  return speed;
}
/*
unsigned char GPIO_Read(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin) AT(BIKE_CODE)
{
	//GPIO_Init(GPIOx, GPIO_Pin, GPIO_MODE_IN_FL_NO_IT);
	//return GPIO_ReadInputPin(GPIOx, GPIO_Pin);
}*/

void Light_Task(void) AT(BIKE_CODE)
{
	unsigned char speed_mode=0;
	
	P3HD 	&=~(BIT(2)|BIT(1)|BIT(0));
    P3DIR 	|= (BIT(2)|BIT(1)|BIT(0));

	if( P30 ) bike.NearLight = 1; else bike.NearLight = 0;
	if( P31 ) bike.TurnRight = 1; else bike.TurnRight = 0;
	if( P32 ) bike.TurnLeft  = 1; else bike.TurnLeft  = 0;
	bike.PHA_Speed = (unsigned long)GetSpeed();
	bike.Speed = (unsigned long)bike.PHA_Speed*1000UL/config.SpeedScale + bike.Speed_dec;
}

void HotReset(void) AT(BIKE_CODE)
{
	if (config.bike[0] == 'b' &&
		config.bike[1] == 'i' && 
		config.bike[2] == 'k' && 
		config.bike[3] == 'e' ){
		bike.HotReset = 1;
	} else {
		bike.HotReset = 0;
	}
}

void WriteConfig(void) AT(BIKE_CODE)
{
	unsigned char *cbuf = (unsigned char *)&config;
	unsigned char i;

	//FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
	//FLASH_Unlock(FLASH_MEMTYPE_DATA);  
	Delay(5000);

	config.bike[0] = 'b';
	config.bike[1] = 'i';
	config.bike[2] = 'k';
	config.bike[3] = 'e';
	for(config.Sum=0,i=0;i<sizeof(BIKE_CONFIG)-1;i++)
		config.Sum += cbuf[i];
		
	//for(i=0;i<sizeof(BIKE_CONFIG);i++)
	//	FLASH_ProgramByte(0x4000+i, cbuf[i]);

	Delay(5000);
	//FLASH_Lock(FLASH_MEMTYPE_DATA);
}

void InitConfig(void) AT(BIKE_CODE)
{
	unsigned char *cbuf = (unsigned char *)&config;
	unsigned char i,sum;

	//for(i=0;i<sizeof(BIKE_CONFIG);i++)
	//	cbuf[i] = FLASH_ReadByte(0x4000 + i);

	for(sum=0,i=0;i<sizeof(BIKE_CONFIG)-1;i++)
		sum += cbuf[i];
		
	if (config.bike[0] != 'b' || 
		config.bike[1] != 'i' || 
		config.bike[2] != 'k' || 
		config.bike[3] != 'e' || 
		sum != config.Sum ){
		config.SysVoltage 	= 60;
		config.VolScale  	= 1000;
		config.TempScale 	= 1000;
		config.SpeedScale	= 1000;
		config.YXT_SpeedScale= 1000;
		config.Mile			= 0;
	}

	bike.Mile = config.Mile;
#if ( TIME_ENABLE == 1 )
	bike.HasTimer = 0;
#endif
	//bike.SpeedMode = SPEEDMODE_DEFAULT;
	bike.YXTERR = 1;
	bike.Speed_dec = 0;
	bike.bPlayFlash = 0;
	
	/*GPIO_Init(VMODE1_PORT, VMODE1_PIN, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(VMODE2_PORT, VMODE2_PIN, GPIO_MODE_IN_PU_NO_IT);
	if ( GPIO_ReadInputPin(VMODE1_PORT, VMODE1_PIN) == RESET ){
		config.SysVoltage = 72;
	} else {
		if ( GPIO_ReadInputPin(VMODE2_PORT, VMODE2_PIN) == RESET ){
			config.SysVoltage = 48;
		} else {
			config.SysVoltage = 60;
		}
	}*/

}

unsigned char GetBatStatus(unsigned int vol) AT(BIKE_CODE)
{
	unsigned char i;
	unsigned int const _code * BatStatus;

	switch ( config.SysVoltage ){
	case 48:BatStatus = BatStatus48;break;
	case 60:BatStatus = BatStatus60;break;
	case 72:BatStatus = BatStatus72;break;
	default:BatStatus = BatStatus60;break;
	}

	for(i=0;i<ContainOf(BatStatus60);i++)
		if ( vol < BatStatus[i] ) break;
	return i;
}

#define TASK_INIT	0
#define TASK_STEP1	1
#define TASK_STEP2	2
#define TASK_STEP3	3
#define TASK_STEP4	4
#define TASK_EXIT	5

unsigned char MileResetTask(void) AT(BIKE_CODE)
{
	static unsigned int pre_tick=0;
	static unsigned char TaskFlag = TASK_INIT;
	static unsigned char lastLight = 0;
	static unsigned char count = 0;
	
	if ( Get_ElapseTick(pre_tick) > 10000 | bike.Braked | bike.Speed )
		TaskFlag = TASK_EXIT;

	switch( TaskFlag ){
	case TASK_INIT:
		if ( Get_SysTick() < 3000 && bike.TurnRight == 1 ){
			TaskFlag = TASK_STEP1;
			count = 0;
		}
		break;
	case TASK_STEP1:
		if ( lastLight == 0 && bike.NearLight){
			count ++;
			if ( count >= 8 ){
				bike.MileFlash = 1;
				pre_tick = Get_SysTick();
				bike.Mile = config.Mile;
				TaskFlag = TASK_STEP2;
			}
		}
		lastLight = bike.NearLight;
		break;
	case TASK_STEP2:
		if ( bike.TurnRight == 0 && bike.TurnLeft == 1 ) {
			TaskFlag = TASK_EXIT;
			bike.MileFlash 	= 0;
			bike.FMile 		= 0;
			bike.Mile 		= 0;
			config.Mile 	= 0;
			WriteConfig();
		}
		break;
	case TASK_EXIT:
	default:
		bike.MileFlash = 0;
		break;
	}
	return 0;
}

void MileTask(void) AT(BIKE_CODE)
{
	static unsigned int time = 0;
	unsigned char speed;
	
	speed = bike.Speed;
	if ( speed > DISPLAY_MAX_SPEED )
		speed = DISPLAY_MAX_SPEED;
	
#ifdef SINGLE_TRIP
	time ++;
	if ( time < 20 ) {	//2s
		bike.Mile = config.Mile;
	} else if ( time < 50 ) { 	//5s
		if ( speed ) {
			time = 50;
			bike.Mile = 0;
		}
	} else if ( time == 50 ){
		bike.Mile = 0;
	} else 
#endif	
	{
		time = 51;
		
		bike.FMile = bike.FMile + speed;
		if(bike.FMile >= 36000)
		{
			bike.FMile = 0;
			bike.Mile++;
			if ( bike.Mile > 99999 )	bike.Mile = 0;
			config.Mile ++;
			if ( config.Mile > 99999 )	config.Mile = 0;
			WriteConfig();
		}  
	}
}

#if ( TIME_ENABLE == 1 )
void TimeTask(void) AT(BIKE_CODE)
{
	static unsigned char time_task=0,left_on= 0,NL_on = 0;
	static unsigned int pre_tick;
	
	if (!bike.HasTimer)
		return ;
	
	if (bike.Speed > 1)
		time_task = 0xff;
	
	switch ( time_task ){
	case 0:
		if ( Get_SysTick() < 5000 && bike.NearLight == 0 && bike.TurnLeft == 1 ){
			pre_tick = Get_SysTick();
			time_task++;
		}
		break;
	case 1:
		if ( bike.TurnLeft == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.NearLight ) time_task = 0xFF;
		}
		break;
	case 2:
		if ( bike.TurnRight == 1 ){
			if ( Get_ElapseTick(pre_tick) > 1000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000  || bike.NearLight ) time_task = 0xFF;
		}
		break;
	case 3:
		if ( bike.TurnRight == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.NearLight ) time_task = 0xFF;
		}
		break;
	case 4:
		if ( bike.TurnLeft == 1 ){
			if ( Get_ElapseTick(pre_tick) > 1000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000  || bike.NearLight ) time_task = 0xFF;
		}
		break;
	case 5:
		if ( bike.TurnLeft == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.NearLight ) time_task = 0xFF;
		}
		break;
	case 6:
		if ( bike.TurnRight == 1 ){
			if ( Get_ElapseTick(pre_tick) > 1000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000  || bike.NearLight ) time_task = 0xFF;
		}
	case 7:
		if ( bike.TurnRight == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.NearLight ) time_task = 0xFF;
		}
		break;
	case 8:
		if ( bike.TurnLeft == 1 ){
			if ( Get_ElapseTick(pre_tick) > 1000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000  || bike.NearLight ) time_task = 0xFF;
		}
	case 9:
		if ( bike.TurnLeft == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.NearLight ) time_task = 0xFF;
		}
		break;
	case 10:
		if ( bike.TurnRight == 1 ){
			if ( Get_ElapseTick(pre_tick) > 1000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000  || bike.NearLight ) time_task = 0xFF;
		}
	case 11:
		if ( bike.TurnRight == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.NearLight ) time_task = 0xFF;
		}
		break;
	case 12:
		if ( bike.TurnLeft == 1 || bike.NearLight ){
			 time_task = 0xFF;
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000 ) {
				time_task= 0;
				bike.time_pos = 0;
				bike.time_set = 1; 
				pre_tick = Get_SysTick();
			}
		}
		break;
	default:
		bike.time_pos = 0;
		bike.time_set = 0; 
		time_task = 0;
		break;
	}

	if ( bike.time_set ){
		if ( bike.TurnLeft ) { left_on = 1; }
		if ( bike.TurnLeft == 0 ) {
			if ( left_on == 1 ){
				bike.time_pos ++;
				bike.time_pos %= 4;
				left_on = 0;
				pre_tick = Get_SysTick();
			}
		}
		if ( bike.NearLight ) { NL_on = 1; pre_tick = Get_SysTick(); }
		if ( bike.NearLight == 0 && NL_on == 1 ) {
			NL_on = 0;
			if ( Get_ElapseTick(pre_tick) < 5000 ){
				switch ( bike.time_pos ){
				case 0:
					bike.Hour += 10;
					bike.Hour %= 20;
					break;
				case 1:
					if ( bike.Hour % 10 < 9 )
						bike.Hour ++;
					else 
						bike.Hour -= 9;
					break;
				case 2:
					bike.Minute += 10;
					bike.Minute %= 60;
					break;
				case 3:
					if ( bike.Minute % 10 < 9 )
						bike.Minute ++;
					else 
						bike.Minute -= 9;
					break;
				default:
					bike.time_set = 0;
					break;
				}
			}
			RtcTime.RTC_Hours 	= bike.Hour;
			RtcTime.RTC_Minutes = bike.Minute;
			//PCF8563_SetTime(PCF_Format_BIN,&RtcTime);
		}
		if ( Get_ElapseTick(pre_tick) > 30000 ){
			bike.time_set = 0;
		}
	}		
	
	//PCF8563_GetTime(PCF_Format_BIN,&RtcTime);
	bike.Hour 		= RtcTime.RTC_Hours%12;
	bike.Minute 	= RtcTime.RTC_Minutes;
}

#endif 

#if 0
void Calibration(void) AT(BIKE_CODE)
{
	unsigned char i;
	unsigned int vol;
	
	CFG->GCR = CFG_GCR_SWD;
	//¶Ì½ÓµÍËÙ¡¢SWIMÐÅºÅ
	GPIO_Init(GPIOD, GPIO_PIN_1, GPIO_MODE_OUT_OD_HIZ_SLOW);

	for(i=0;i<32;i++){
		GPIO_WriteLow (GPIOD,GPIO_PIN_1);
		Delay(1000);
		if( GPIO_Read(SPMODE1_PORT	, SPMODE1_PIN) ) break;
		GPIO_WriteHigh (GPIOD,GPIO_PIN_1);
		Delay(1000);
		if( GPIO_Read(SPMODE1_PORT	, SPMODE1_PIN)  == RESET ) break;
	}
	if ( i == 32 ){
		for(i=0;i<0xFF;i++){
			if ( GetVolStabed(&vol) && (vol > 120) ) break;
			//IWDG_ReloadCounter();  
		}
		bike.Voltage		= vol;
		//bike.Temperature	= GetTemp();
		//bike.Speed		= GetSpeed();

		config.VolScale		= (unsigned long)bike.Voltage*1000UL/VOL_CALIBRATIOIN;					//60.00V
		//config.TempScale	= (long)bike.Temperature*1000UL/TEMP_CALIBRATIOIN;	//25.0C
		//config.SpeedScale = (unsigned long)bike.Speed*1000UL/SPEED_CALIBRATIOIN;				//30km/h
		//config.Mile = 0;
		WriteConfig();
	}
	
	for(i=0;i<32;i++){
		GPIO_WriteLow (GPIOD,GPIO_PIN_1);
		Delay(1000);
		if( GPIO_Read(SPMODE2_PORT	, SPMODE2_PIN) ) break;
		GPIO_WriteHigh (GPIOD,GPIO_PIN_1);
		Delay(1000);
		if( GPIO_Read(SPMODE2_PORT	, SPMODE2_PIN)  == RESET ) break;
	}
	if ( i == 32 ){
		bike.uart = 1;
	} else
		bike.uart = 0;

	CFG->GCR &= ~CFG_GCR_SWD;
}
#endif

#define BIKE_INIT 		0
#define BIKE_RESET_WAIT 1
#define BIKE_RUN		2

void bike_task(void) AT(BIKE_CODE)
{
	unsigned char i;
	unsigned int tick;
	static unsigned int count=0;
	unsigned int vol=0;
	static unsigned int task=BIKE_INIT;	
	
	bike.Tick += 100;
	
	switch(task){
	case BIKE_INIT:
		deg_puts("BIKE_INIT\n");
		HotReset();
		if ( bike.HotReset == 0 ){
		    deg_puts("BL55072_Config(1)\n");
			BL55072_Config(1);
		} else {
		    deg_puts("BL55072_Config(0)\n");
			BL55072_Config(0);
		}

	//	for(i=0;i<32;i++){	GetVol();	/*IWDG_ReloadCounter(); */ }
	//	for(i=0;i<16;i++){	GetSpeed();	/*IWDG_ReloadCounter(); */ }
		for(i=0;i<4;i++) {	GetTemp();	/*IWDG_ReloadCounter(); */ }

		InitConfig();
		//Calibration();

	#if ( TIME_ENABLE == 1 )	
		//bike.HasTimer = !PCF8563_Check();
		//bike.HasTimer = PCF8563_GetTime(PCF_Format_BIN,&RtcTime);
	#endif
  	
		GetVolStabed(&vol);
		bike.Voltage = (unsigned long)vol*1000UL/config.VolScale;
		bike.Temperature = GetTemp();

		if ( bike.HotReset == 0 ) {
			task = BIKE_RESET_WAIT;
			deg_puts("BIKE_RESET_WAIT\n");
		} else {
			task = BIKE_RUN;
			deg_puts("BIKE_RUN\n");
		}
		break;
	case BIKE_RESET_WAIT:
		if ( Get_SysTick() > PON_ALLON_TIME ){
			BL55072_Config(0);
			task = BIKE_RUN;
		}
		break;
	case BIKE_RUN:
		tick = Get_SysTick();
		
		if ( (tick >= tick_100ms && (tick - tick_100ms) > 100 ) || \
			 (tick <  tick_100ms && (0xFFFF - tick_100ms + tick) > 100 ) ) {
			tick_100ms = tick;
			count ++;
			

			if ( (count % 4 ) == 0 ){
				if ( GetVolStabed(&vol) ){}
				bike.Voltage = (unsigned long)vol*1000UL/config.VolScale;
			}
			if ( (count % 10) == 0 ){
			//	bike.Temperature= (long)GetTemp()	*1000UL/config.TempScale;
				bike.Temperature= GetTemp();
			}
			bike.BatStatus 	= GetBatStatus(bike.Voltage);
		
			Light_Task();
			MileTask(); 
			
		#ifdef RESET_MILE_ENABLE	
			MileResetTask();
		#endif	
					
		#if ( TIME_ENABLE == 1 )	
			TimeTask();   
		#endif
      
		#ifdef LCD_SEG_TEST
			if ( count >= 100 ) count = 0;
			bike.Voltage 	= count/10 + count/10*10UL + count/10*100UL + count/10*1000UL;
			bike.Temperature= count/10 + count/10*10UL + count/10*100UL;
			bike.Speed		= count/10 + count/10*10;
			bike.Mile		= count/10 + count/10*10UL + count/10*100UL + count/10*1000UL + count/10*10000UL;
			bike.Hour       = count/10 + count/10*10;
			bike.Minute     = count/10 + count/10*10;
		#endif
	
			MenuUpdate(&bike);
		}
		break;
	default:
		task = BIKE_INIT;
		break;
	}
}

