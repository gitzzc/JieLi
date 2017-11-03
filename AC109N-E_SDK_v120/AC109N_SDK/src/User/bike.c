
#include "config.h"
#include "key.h"
#include "iic.h"

#include "bl55072.h"
#include "display.h"
#include "bike.h"

#define ContainOf(x) (sizeof(x)/sizeof(x[0]))

const unsigned int  BatStatus48[] AT(BIKE_TABLE_CODE) = {420,427,432,439,444,448,455,459,466,470,0xFFFF};
const unsigned int  BatStatus60[] AT(BIKE_TABLE_CODE) = {520,526,533,540,547,553,560,566,574,580,0xFFFF};

unsigned int _xdata tick_100ms=0;
unsigned int _xdata speed_buf[16];
unsigned int _xdata vol_buf[32];
unsigned int _xdata temp_buf[4];
unsigned char vol_index=0;

BIKE_STATUS _xdata bike;
__no_init BIKE_CONFIG _xdata config;


unsigned int Get_SysTick(void) AT(BIKE_CODE)
{
	unsigned int tick;
	
	EA = 0;
	tick = bike.uiTick;
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

#if 0
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
#endif

int GetVol(void) AT(BIKE_CODE)
{
	static unsigned char index = 0;
	long vol;
	unsigned char i;

	vol = (unsigned long)(AD_var.wADValue[AD_BIKE_VOL]>>6)*1033UL/1024UL;   //ADC/1024*103.3/3.3V*3.3V

	vol_buf[index++] = vol;
	if ( index >= ContainOf(vol_buf) )
		index = 0;
	for(i=0,vol=0;i<ContainOf(vol_buf);i++)
		vol += vol_buf[i];
	vol /= ContainOf(vol_buf);

	return vol;
}

void GetVolSample(void)
{
    if ( vol_index >= ContainOf(vol_buf) )
      return ;
    
    vol_buf[vol_index++] = AD_var.wADValue[AD_BIKE_VOL];
}

unsigned char GetVolStabed(unsigned int* vol) AT(BIKE_CODE)
{
	unsigned long mid;
	static int buf[32];
	static unsigned char index = 0;
	unsigned char i;
    
    EA = 0;
    if ( vol_index < ContainOf(vol_buf) ){
      EA = 1;
      return 0;
    }      
	
	for(i=0,mid=0;i<ContainOf(vol_buf);i++)
		mid += vol_buf[i];
	mid /= ContainOf(vol_buf);

	*vol = (mid>>6)*1033UL/1024UL;   //ADC/1024*103.3/3.3V*3.3V

    //deg("mid %ld\n",mid);
	for( i=0;i<32;i++){
      if ( mid > (20UL<<6) && ((mid *100 / vol_buf[i]) > 102 ||  (mid *100 / vol_buf[i]) < 98) ){
		//if ( mid > 5 && ((mid *100 / buf[i]) > 102 ||  (mid *100 / buf[i]) < 98) )
            vol_index = 0;
            EA = 1;
			return 0;
      }
	}
	
    vol_index = 0;
    EA = 1;
	return 1;
}

unsigned char GetSpeed(void) AT(BIKE_CODE)
{
	static unsigned char index = 0;
	unsigned int speed,vol;
	unsigned char i;

	vol = AD_var.wADValue[AD_BIKE_SPEED]>>6;
    //deg("wADValue %u ",AD_var.wADValue[AD_BIKE_SPEED]>>6);
 	
	speed_buf[index++] = vol;
	if ( index >= ContainOf(speed_buf) )
		index = 0;

	for(i=0,vol=0;i<ContainOf(speed_buf);i++)
		vol += speed_buf[i];
	vol /= ContainOf(speed_buf);
    vol = (unsigned long)vol*1033/1024;
    //deg("vol %u\n",vol);
	
	if ( config.uiSysVoltage	== 48 ){	
      if ( vol < 210 ){
		speed = (unsigned long)vol*182UL/1024UL;        //ADC/1024*103.3/3.3*3.3V/21V*37 KM/H
      } else if ( vol < 240 ){
		speed = (unsigned long)vol*18078UL/102400UL;    //ADC/1024*103.3/3.3*3.3V/24V*42 KM/H
      } else/* if ( vol < 270 )*/{
		speed = (unsigned long)vol*18364UL/102400UL;    //ADC/1024*103.3/3.3*3.3V/27V*48 KM/H
      }
	} else if ( config.uiSysVoltage	== 60 ) {
      if ( vol < 260 ){
		speed = (unsigned long)vol*15098UL/102400UL;   //ADC/1024*103.3/3.3*3.3V/26V*38 KM/H
      } else if ( vol < 300 ){
		speed = (unsigned long)vol*15151UL/102400UL;   //ADC/1024*103.3/3.3*3.3V/30V*44 KM/H
      } else/* if ( vol < 335 )*/{
		speed = (unsigned long)vol*15110UL/102400UL;   //ADC/1024*103.3/3.3*3.3V/33.5V*49 KM/H
      }
	}
	if ( speed > 99 )
		speed = 99;
	
  return speed;
}

#define READ_TURN_LEFT()		(P32)
#define READ_TURN_RIGHT()		(P31)

void LRFlash_Task(void)
{
	static unsigned char left_on=0	,left_off=0;
	static unsigned char right_on=0	,right_off=0;
	static unsigned char left_count=0,right_count=0;

	if ( READ_TURN_LEFT() ){	//ON
        left_off = 0;
        if ( left_on ++ > 10 ){		//200ms 滤波
            if ( left_on > 100 ){
          	    left_on = 101;
                bike.bLFlashType = 0;
            }
           	if ( left_count < 0xFF-50 ){
	            left_count++;
            }
			bike.bLeftFlash	= 1;
			bike.bTurnLeft 	= 1;
        }
	} else {					//OFF
        left_on = 0;
        if ( left_off ++ == 10 ){
        	left_count += 50;	//500ms
			bike.bLeftFlash	= 0;
        } else if ( left_off > 10 ){
	        left_off = 11;
            bike.bLFlashType = 1;
            if ( left_count == 0 ){
				bike.bTurnLeft = 0;
            } else
				left_count --;
		}
	}
	
	if ( READ_TURN_RIGHT() ){	//ON
        right_off = 0;
        if ( right_on ++ > 10 ){
            if ( right_on > 100 ){
          	    right_on = 101;
                bike.bRFlashType = 0;
            }
           	if ( right_count < 0xFF-50 ){
				right_count++;
            }
			bike.bRightFlash	= 1;
			bike.bTurnRight 	= 1;
        }
	} else {					//OFF
        right_on = 0;
        if ( right_off ++ == 10 ){
        	right_count += 50;	//500ms
			bike.bRightFlash = 0;
        } else if ( right_off > 10 ){
	        right_off = 11;
            bike.bRFlashType = 1;
            if ( right_count == 0 ){
				bike.bTurnRight = 0;
            } else
				right_count --;
		}
	}
}

void Light_Task(void) AT(BIKE_CODE)
{
	unsigned char speed_mode=0;
	
	//if ( bike.YXTERR )
    {
		P3PU 	&=~(BIT(2)|BIT(1)|BIT(0));
		P3PD 	&=~(BIT(2)|BIT(1)|BIT(0));
		P3DIE 	|= (BIT(2)|BIT(1)|BIT(0));
		P3DIR 	|= (BIT(2)|BIT(1)|BIT(0));
	
		if( P30 ) {
		  bike.bNearLight = 1;
		  P2PU  |= BIT(0);  //背光调节
		  P2HD  |= BIT(0);
		  P2DIE |= BIT(0);
		  P2DIR &=~BIT(0);
		  P20    = 0;
		} else {
		  bike.bNearLight = 0;
		  P2PU  |= BIT(0);  //背光调节
		  P2HD  |= BIT(0);
		  P2DIE |= BIT(0);
		  P2DIR &=~BIT(0);
		  P20    = 1;
		}
		//if( P31 ) bike.bTurnRight = 1; else bike.bTurnRight = 0;
		//if( P32 ) bike.bTurnLeft  = 1; else bike.bTurnLeft  = 0;

		bike.ucPHA_Speed 	= (unsigned long)GetSpeed();
		bike.ucSpeed 		= (unsigned long)bike.ucPHA_Speed*1000UL/config.uiSpeedScale;
    	//deg("ucPHA_Speed=%u,SpeedScale=%u,Speed=%u\n",bike.ucPHA_Speed,config.SpeedScale,bike.ucSpeed);
    }	
}

void HotReset(void) AT(BIKE_CODE)
{
	if (config.ucBike[0] == 'b' &&
		config.ucBike[1] == 'i' &&
		config.ucBike[2] == 'k' &&
		config.ucBike[3] == 'e' ){
		bike.bHotReset = 1;
	} else {
		bike.bHotReset = 0;
	}
}

void WriteConfig(void) AT(BIKE_CODE)
{
	unsigned char *cbuf = (unsigned char *)&config;
	unsigned char i;

	config.ucBike[0] = 'b';
	config.ucBike[1] = 'i';
	config.ucBike[2] = 'k';
	config.ucBike[3] = 'e';
	for(config.ucSum=0,i=0;i<sizeof(BIKE_CONFIG)-1;i++)
		config.ucSum += cbuf[i];
		
	for(i=0;i<sizeof(BIKE_CONFIG);i++)
		set_memory(BIKE_EEPROM_START+i,cbuf[i]);
}

void InitConfig(void) AT(BIKE_CODE)
{
	unsigned char *cbuf = (unsigned char *)&config;
	unsigned char i,sum;

	for(i=0;i<sizeof(BIKE_CONFIG);i++)
		cbuf[i] = get_memory(BIKE_EEPROM_START + i);

	for(sum=0,i=0;i<sizeof(BIKE_CONFIG)-1;i++)
		sum += cbuf[i];
		
	if (config.ucBike[0] != 'b' ||
		config.ucBike[1] != 'i' ||
		config.ucBike[2] != 'k' ||
		config.ucBike[3] != 'e' ||
		sum != config.ucSum ){
		config.uiSysVoltage 	= 60;
		config.uiVolScale  		= 1000;
		config.uiTempScale 		= 1000;
		config.uiSpeedScale		= 1000;
		config.uiYXT_SpeedScale	= 1000;
		config.ulMile			= 0;
	}

	bike.ulMile = config.ulMile;
#if ( TIME_ENABLE == 1 )
	bike.bHasTimer = 0;
#endif
	//bike.SpeedMode = SPEEDMODE_DEFAULT;
	bike.bYXTERR = 1;
	bike.bPlayFlash = 0;
    bike.uiTick = 0;
	
    P3PU    &=~(BIT(3)|BIT(4));
    P3PD    &=~(BIT(3)|BIT(4));
    P3DIE   |= (BIT(3)|BIT(4));
    P3DIR   |= (BIT(3)|BIT(4));

	if ( P33 == 0 ){
        config.uiSysVoltage = 48;
    } else {
        config.uiSysVoltage = 60;
    }
    bike.bLFlashType = P34;
    bike.bRFlashType = P34;
}

unsigned char GetBatStatus(unsigned int vol) AT(BIKE_CODE)
{
	unsigned char i;
	unsigned int const _code * BatStatus;

	switch ( config.uiSysVoltage ){
	case 48:BatStatus = BatStatus48;break;
	case 60:BatStatus = BatStatus60;break;
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
	static unsigned char count = 0;
	
    if ( TaskFlag == TASK_EXIT )
        return 0;
    
	if ( Get_ElapseTick(pre_tick) > 10000 | bike.bBraked | bike.ucSpeed )
		TaskFlag = TASK_EXIT;

	switch( TaskFlag ){
	case TASK_INIT:
		if ( Get_SysTick() < 3000 && bike.bTurnRight == 1 ){
			TaskFlag = TASK_STEP1;
			count = 0;
		}
		break;
	case TASK_STEP1:
		if ( bike.bLastNear == 0 && bike.bNearLight){
			pre_tick = Get_SysTick();
			if ( ++count >= 8 ){
				TaskFlag = TASK_STEP2;
				bike.bMileFlash = 1;
				bike.ulMile = config.ulMile;
			}
		}
		bike.bLastNear = bike.bNearLight;
		break;
	case TASK_STEP2:
		if ( bike.bTurnRight == 0 && bike.bTurnLeft == 1 ) {
			TaskFlag = TASK_EXIT;
    		bike.bMileFlash = 0;
			bike.ulFMile 	= 0;
			bike.ulMile 	= 0;
			config.ulMile 	= 0;
			WriteConfig();
		}
		break;
	case TASK_EXIT:
	default:
		bike.bMileFlash = 0;
		break;
	}
	return 0;
}

void MileTask(void) AT(BIKE_CODE)
{
	static unsigned int time = 0;
	unsigned char speed;
	
	speed = bike.ucSpeed;
	if ( speed > DISPLAY_MAX_SPEED )
		speed = DISPLAY_MAX_SPEED;
	
#ifdef SINGLE_TRIP
	time ++;
	if ( time < 20 ) {	//2s
		bike.ulMile = config.ulMile;
	} else if ( time < 50 ) { 	//5s
		if ( speed ) {
			time = 50;
			bike.ulMile = 0;
		}
	} else if ( time == 50 ){
		bike.ulMile = 0;
	} else
#endif	
	{
		time = 51;
		
		bike.ulFMile = bike.ulFMile + speed;
		if(bike.ulFMile >= 36000)
		{
			bike.ulFMile = 0;
			bike.ulMile++;
			if ( bike.ulMile > 99999 )	bike.ulMile = 0;
			config.ulMile ++;
			if ( config.ulMile > 99999 )config.ulMile = 0;
			WriteConfig();
		}
	}
}

#if ( TIME_ENABLE == 1 )
void TimeTask(void) AT(BIKE_CODE)
{
	static unsigned char time_task=0,left_on= 0,NL_on = 0;
	static unsigned int pre_tick;
	
	if (!bike.bHasTimer)
		return ;
	
	if (bike.ucSpeed > 1)
		time_task = 0xff;
	
	switch ( time_task ){
	case 0:
		if ( Get_SysTick() < 5000 && bike.bNearLight == 0 && bike.bTurnLeft == 1 ){
			pre_tick = Get_SysTick();
			time_task++;
		}
		break;
	case 1:
		if ( bike.bTurnLeft == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.bNearLight ) time_task = 0xFF;
		}
		break;
	case 2:
		if ( bike.bTurnRight == 1 ){
			if ( Get_ElapseTick(pre_tick) > 1000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000  || bike.bNearLight ) time_task = 0xFF;
		}
		break;
	case 3:
		if ( bike.bTurnRight == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.bNearLight ) time_task = 0xFF;
		}
		break;
	case 4:
		if ( bike.bTurnLeft == 1 ){
			if ( Get_ElapseTick(pre_tick) > 1000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000  || bike.bNearLight ) time_task = 0xFF;
		}
		break;
	case 5:
		if ( bike.bTurnLeft == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.bNearLight ) time_task = 0xFF;
		}
		break;
	case 6:
		if ( bike.bTurnRight == 1 ){
			if ( Get_ElapseTick(pre_tick) > 1000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000  || bike.bNearLight ) time_task = 0xFF;
		}
	case 7:
		if ( bike.bTurnRight == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.bNearLight ) time_task = 0xFF;
		}
		break;
	case 8:
		if ( bike.bTurnLeft == 1 ){
			if ( Get_ElapseTick(pre_tick) > 1000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000  || bike.bNearLight ) time_task = 0xFF;
		}
	case 9:
		if ( bike.bTurnLeft == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.bNearLight ) time_task = 0xFF;
		}
		break;
	case 10:
		if ( bike.bTurnRight == 1 ){
			if ( Get_ElapseTick(pre_tick) > 1000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000  || bike.bNearLight ) time_task = 0xFF;
		}
	case 11:
		if ( bike.bTurnRight == 0 ){
			if ( Get_ElapseTick(pre_tick) < 2000  ) time_task = 0xFF;	else { pre_tick = Get_SysTick(); time_task++; }
		} else {
			if ( Get_ElapseTick(pre_tick) > 10000 || bike.bNearLight ) time_task = 0xFF;
		}
		break;
	case 12:
		if ( bike.bTurnLeft == 1 || bike.bNearLight ){
			 time_task = 0xFF;
		} else {
			if ( Get_ElapseTick(pre_tick) > 1000 ) {
				time_task= 0;
				bike.ucTimePos = 0;
				bike.bTimeSet = 1;
				pre_tick = Get_SysTick();
			}
		}
		break;
	default:
		bike.ucTimePos = 0;
		bike.bTimeSet = 0;
		time_task = 0;
		break;
	}

	if ( bike.bTimeSet ){
		if ( bike.bTurnLeft ) { left_on = 1; }
		if ( bike.bTurnLeft == 0 ) {
			if ( left_on == 1 ){
				bike.ucTimePos ++;
				bike.ucTimePos %= 4;
				left_on = 0;
				pre_tick = Get_SysTick();
			}
		}
		if ( bike.bNearLight ) { NL_on = 1; pre_tick = Get_SysTick(); }
		if ( bike.bNearLight == 0 && NL_on == 1 ) {
			NL_on = 0;
			if ( Get_ElapseTick(pre_tick) < 5000 ){
				switch ( bike.ucTimePos ){
				case 0:
					bike.ucHour += 10;
					bike.ucHour %= 20;
					break;
				case 1:
					if ( bike.ucHour % 10 < 9 )
						bike.ucHour ++;
					else
						bike.ucHour -= 9;
					break;
				case 2:
					bike.ucMinute += 10;
					bike.ucMinute %= 60;
					break;
				case 3:
					if ( bike.ucMinute % 10 < 9 )
						bike.ucMinute ++;
					else
						bike.ucMinute -= 9;
					break;
				default:
					bike.bTimeSet = 0;
					break;
				}
			}
			RtcTime.RTC_Hours 	= bike.ucHour;
			RtcTime.RTC_Minutes = bike.ucMinute;
			//PCF8563_SetTime(PCF_Format_BIN,&RtcTime);
		}
		if ( Get_ElapseTick(pre_tick) > 30000 ){
			bike.bTimeSet = 0;
		}
	}		
	
	//PCF8563_GetTime(PCF_Format_BIN,&RtcTime);
	bike.ucHour 	= RtcTime.RTC_Hours%12;
	bike.ucMinute 	= RtcTime.RTC_Minutes;
}

#endif


unsigned char SpeedCaltTask(void)
{
	static unsigned int pre_tick=0;
	static unsigned char TaskFlag = TASK_INIT;
	static unsigned char lastSpeed = 0;
	static unsigned char count = 0;
    static signed char SpeedInc=0;
	
    if ( TaskFlag == TASK_EXIT )
      	return 0;
    
	if ( Get_ElapseTick(pre_tick) > 10000 || bike.bBraked )
		TaskFlag = TASK_EXIT;

	switch( TaskFlag ){
	case TASK_INIT:
		if ( Get_SysTick() < 3000 && bike.bTurnLeft == 1 ){
			TaskFlag = TASK_STEP1;
			count = 0;
		}
		break;
	case TASK_STEP1:
		if ( bike.bLastNear == 0 && bike.bNearLight == 1){
			if ( ++count >= 8 ){
				TaskFlag 	= TASK_STEP2;
				count 		= 0;
				SpeedInc 	= 0;
				bike.bSpeedFlash = 1;
			}
			pre_tick = Get_SysTick();
		}
		bike.bLastNear = bike.bNearLight;
        bike.bLastLeft = bike.bTurnLeft;
        bike.bLastRight = bike.bTurnRight;
		break;
	case TASK_STEP2:
        if ( config.uiSysVoltage == 48 )
			bike.ucSpeed = 42;
        else if ( config.uiSysVoltage == 60 )
			bike.ucSpeed = 44;

		if ( bike.bLastNear == 0 && bike.bNearLight == 1 ){
			pre_tick = Get_SysTick();
            if ( bike.bTurnLeft == 1 ) {
				count = 0;
				if ( bike.ucSpeed + SpeedInc > 1 )
					SpeedInc --;
	        } else if ( bike.bTurnRight == 1 ) {
				count = 0;
                if ( bike.ucSpeed + SpeedInc < 99 )
					SpeedInc ++;
            } else {
				if ( ++count >= 5 ){
					TaskFlag = TASK_EXIT;
					bike.bSpeedFlash = 0;
					if ( bike.ucSpeed ) {
						if ( bike.bYXTERR )
							config.uiSpeedScale 		= (unsigned long)bike.ucSpeed*1000UL/(bike.ucSpeed+SpeedInc);
						else
							config.uiYXT_SpeedScale 	= (unsigned long)bike.ucSpeed*1000UL/(bike.ucSpeed+SpeedInc);
						WriteConfig();
					}
				}
            }
		}
		bike.bLastNear = bike.bNearLight;

		if ( lastSpeed && bike.ucSpeed == 0 ){
			TaskFlag = TASK_EXIT;
		}
        
		//if ( bike.ucSpeed )
		//	pre_tick = Get_SysTick();

        bike.ucSpeed += SpeedInc;
        lastSpeed 	= bike.ucSpeed;
		break;
	case TASK_EXIT:
	default:
		bike.bSpeedFlash = 0;
		break;
	}
	return 0;
}

#if 0
void Calibration(void) AT(BIKE_CODE)
{
	unsigned char i;
	unsigned int vol;
	
	CFG->GCR = CFG_GCR_SWD;
	//短接低速、SWIM信号
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
		//bike.ucSpeed		= GetSpeed();

		config.VolScale		= (unsigned long)bike.Voltage*1000UL/VOL_CALIBRATIOIN;					//60.00V
		//config.TempScale	= (long)bike.Temperature*1000UL/TEMP_CALIBRATIOIN;	//25.0C
		//config.SpeedScale = (unsigned long)bike.ucSpeed*1000UL/SPEED_CALIBRATIOIN;				//30km/h
		//config.ulMile = 0;
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

void bike_PowerUp(void)
{
	HotReset();
	if ( bike.bHotReset == 0 ){
		BL55072_Config(1);
	} else {
		BL55072_Config(0);
	}
}

#define BIKE_INIT 		0
#define BIKE_RESET_WAIT 1
#define BIKE_SETUP		2
#define BIKE_RUN		3

void bike_task(void) AT(BIKE_CODE)
{
	unsigned char i;
	unsigned int tick;
	static unsigned int count=0;
	unsigned int vol=0;
	static unsigned int task=BIKE_INIT;	
	
	bike.uiTick += 100;
	
	switch(task){
	case BIKE_INIT:

		for(i=0;i<32;i++){	GetVolSample(); }
	//	for(i=0;i<16;i++){	GetSpeed();	/*IWDG_ReloadCounter(); */ }
	//	for(i=0;i<4;i++) {	GetTemp();	/*IWDG_ReloadCounter(); */ }

		InitConfig();
		//Calibration();

	#if ( TIME_ENABLE == 1 )	
		//bike.HasTimer = !PCF8563_Check();
		//bike.HasTimer = PCF8563_GetTime(PCF_Format_BIN,&RtcTime);
	#endif
  	
		GetVolStabed(&vol);
		bike.uiVoltage 	= (unsigned long)vol*1000UL/config.uiVolScale;
		bike.ucBatStatus= GetBatStatus(bike.uiVoltage);

		if ( bike.bHotReset == 0 ) {
			task = BIKE_RESET_WAIT;
		} else {
			task = BIKE_SETUP;
		}
		break;
	case BIKE_RESET_WAIT:
		if ( Get_SysTick() > PON_ALLON_TIME ){
			BL55072_Config(0);
			task = BIKE_SETUP;
		}
		break;
    case BIKE_SETUP:
        task = BIKE_RUN;
	case BIKE_RUN:
		//tick = Get_SysTick();
		
		//if ( (tick >= tick_100ms && (tick - tick_100ms) >= 100 ) || \
		//	 (tick <  tick_100ms && (0xFFFF - tick_100ms + tick) >= 100 ) ) {
		//	tick_100ms = tick;
			count ++;

            if ( (count % 5) == 0 )
            {
				if ( GetVolStabed(&vol) )
					bike.uiVoltage = (unsigned long)vol*1000UL/config.uiVolScale;
				bike.ucBatStatus 	= GetBatStatus(bike.uiVoltage);
            }
		
			Light_Task();
			MileTask();
			
					
		#if ( TIME_ENABLE == 1 )	
			TimeTask();
		#endif

		#ifdef RESET_MILE_ENABLE	
			MileResetTask();
		#endif	

            SpeedCaltTask();

		#ifdef LCD_SEG_TEST
			if ( count >= 100 ) count = 0;
			bike.ucVoltage 		= count/10 + count/10*10UL + count/10*100UL + count/10*1000UL;
			bike.siTemperature	= count/10 + count/10*10UL + count/10*100UL;
			bike.ucSpeed		= count/10 + count/10*10;
			bike.ulMile			= count/10 + count/10*10UL + count/10*100UL + count/10*1000UL + count/10*10000UL;
			bike.ucHour       	= count/10 + count/10*10;
			bike.ucMinute     	= count/10 + count/10*10;
		#endif

            MenuUpdate(&bike);
	
		//}
		break;
	default:
		task = BIKE_INIT;
		break;
	}
}

