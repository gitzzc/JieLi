
#include "config.h"
#include "key.h"
#include "iic.h"

#include "bl55072.h"
#include "display.h"
#include "bike.h"

#define ContainOf(x) (sizeof(x)/sizeof(x[0]))
typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;
typedef   signed int  int16_t;
typedef unsigned long uint32_t;


const unsigned int  BatStatus48[] AT(BIKE_TABLE_CODE) = {420,427,432,439,444,448,455,459,466,470,0xFFFF};
const unsigned int  BatStatus60[] AT(BIKE_TABLE_CODE) = {520,526,533,540,547,553,560,566,574,580,0xFFFF};	//V1.16
//const unsigned int  BatStatus60[] AT(BIKE_TABLE_CODE) = {480,496,509,522,535,548,561,574,587,600,0xFFFF};	//V1.17
const unsigned int  BatStatus72[] AT(BIKE_TABLE_CODE) = {630,641,651,661,671,681,691,701,711,720,0xFFFF};	//V1.18

unsigned int _xdata tick_100ms=0;
unsigned int _xdata speed_buf[16];
unsigned int _xdata vol_buf[32]; 
unsigned int _xdata temp_buf[4];
unsigned char vol_index=0;

BIKE_STATUS _xdata sBike;
__no_init BIKE_CONFIG _xdata sConfig;


unsigned int Get_SysTick(void) AT(BIKE_CODE)
{
	uint16_t uiTick;
	
	EA = 0;
	uiTick = sBike.uiTick;
	EA = 1;
	
	return uiTick;
}

unsigned int Get_ElapseTick(unsigned int uiPreTick) AT(BIKE_CODE)
{
	uint16_t uiTick = Get_SysTick();

	if ( uiTick >= uiPreTick )	
		return (uiTick - uiPreTick); 
	else
		return (0xFFFF - uiPreTick + uiTick);
}


void HotReset(void) AT(BIKE_CODE)
{
	if (sConfig.ucBike[0] == 'b' &&
		sConfig.ucBike[1] == 'i' &&
		sConfig.ucBike[2] == 'k' &&
		sConfig.ucBike[3] == 'e' ){
		sBike.bHotReset = 1;
	} else {
		sBike.bHotReset = 0;
	}
}

void WriteConfig(void) AT(BIKE_CODE)
{
	uint8_t *cbuf = (uint8_t *)&sConfig;
	uint8_t i;

	sConfig.ucBike[0] = 'b';
	sConfig.ucBike[1] = 'i';
	sConfig.ucBike[2] = 'k';
	sConfig.ucBike[3] = 'e';
	for(sConfig.ucSum=0,i=0;i<sizeof(BIKE_CONFIG)-1;i++)
		sConfig.ucSum += cbuf[i];
		
	for(i=0;i<sizeof(BIKE_CONFIG);i++)
		set_memory(BIKE_EEPROM_START+i,cbuf[i]);
}

void InitConfig(void) AT(BIKE_CODE)
{
	uint8_t *cbuf = (uint8_t *)&sConfig;
	uint8_t i,sum;

	for(i=0;i<sizeof(BIKE_CONFIG);i++)
		cbuf[i] = get_memory(BIKE_EEPROM_START + i);

	for(sum=0,i=0;i<sizeof(BIKE_CONFIG)-1;i++)
		sum += cbuf[i];
		
	if (sConfig.ucBike[0] != 'b' ||
		sConfig.ucBike[1] != 'i' ||
		sConfig.ucBike[2] != 'k' ||
		sConfig.ucBike[3] != 'e' ||
		sum != sConfig.ucSum ){
		sConfig.uiSysVoltage 	= 60;
		sConfig.uiVolScale  		= 1000;
		sConfig.uiTempScale 		= 1000;
		sConfig.uiSpeedScale		= 1000;
		sConfig.uiYXT_SpeedScale	= 1000;
#ifdef SINGLE_TRIP
		sConfig.uiSingleTrip	= 1;
#else
        sConfig.uiSingleTrip		= 0;
#endif
		sConfig.ulMile			= 0;
	}

	sBike.ulMile = sConfig.ulMile;
#if ( TIME_ENABLE == 1 )
	sBike.bHasTimer = 0;
#endif
	sBike.ulFMile = 0;
	sBike.ucSpeedMode = 0;
	sBike.bYXTERR = 1;
	sBike.bPlayFlash = 0;
    sBike.uiTick = 0;
    sBike.bVolFlash = 0;
    sBike.uiShowFileNO = 2;  //2s
	
    P3PU    &=~BIT(3);
    P3PD    &=~BIT(3);
    P3DIE   |= BIT(3);
    P3DIR   |= BIT(3);

	if ( P33 == 0 ){
#ifdef VOL6072
		sConfig.uiSysVoltage = 72;
#else
		sConfig.uiSysVoltage = 48;
#endif
	} else {
        sConfig.uiSysVoltage = 60;
    }
}

unsigned char GetBatStatus(unsigned int uiVol) AT(BIKE_CODE)
{
	uint8_t i;
	int16_t const _code * uiBatStatus;

	switch ( sConfig.uiSysVoltage ){
	case 48:uiBatStatus = BatStatus48;break;
	case 60:uiBatStatus = BatStatus60;break;
	case 72:uiBatStatus = BatStatus72;break;
	default:uiBatStatus = BatStatus60;break;
	}

	for(i=0;i<ContainOf(BatStatus60);i++)
		if ( uiVol < uiBatStatus[i] ) break;
	return i;
}

#if 0
const int32_t NTC_B3450[29][2] = 
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
	uint8_t i,j;

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
	
	if ( sConfig.uiSysVoltage	== 48 ){
      if ( vol < 210 ){
		speed = (unsigned long)vol*182UL/1024UL;        //ADC/1024*103.3/3.3*3.3V/21V*37 KM/H
      } else if ( vol < 240 ){
		speed = (unsigned long)vol*18078UL/102400UL;    //ADC/1024*103.3/3.3*3.3V/24V*42 KM/H
      } else/* if ( vol < 270 )*/{
		speed = (unsigned long)vol*18364UL/102400UL;    //ADC/1024*103.3/3.3*3.3V/27V*48 KM/H
      }
	} else if ( sConfig.uiSysVoltage	== 60 ) {
#ifdef BIKE_48_60_FM_BANPENG
		speed = (unsigned long)vol*16528UL/102400UL;   //ADC/1024*103.3/3.3*3.3V/25V*40 KM/H
#else
	  if ( vol < 260 ){
		speed = (unsigned long)vol*15098UL/102400UL;   //ADC/1024*103.3/3.3*3.3V/26V*38 KM/H
      } else if ( vol < 300 ){
		speed = (unsigned long)vol*15151UL/102400UL;   //ADC/1024*103.3/3.3*3.3V/30V*44 KM/H
      } else/* if ( vol < 335 )*/{
		speed = (unsigned long)vol*15110UL/102400UL;   //ADC/1024*103.3/3.3*3.3V/33.5V*49 KM/H
      }
#endif
	} else if ( sConfig.uiSysVoltage	== 72 ) {
      if ( vol < 260 ){
		speed = (unsigned long)vol*12462UL/102400UL;   //ADC/1024*103.3/3.3*3.3V/31.5V*38 KM/H
      } else if ( vol < 300 ){
		speed = (unsigned long)vol*12626UL/102400UL;   //ADC/1024*103.3/3.3*3.3V/36V*44 KM/H
      } else/* if ( vol < 335 )*/{
		speed = (unsigned long)vol*12498UL/102400UL;   //ADC/1024*103.3/3.3*3.3V/40.5V*49 KM/H
      }
	}
	if ( speed > 99 )
		speed = 99;
	
  return speed;
}

#define READ_TURN_LEFT()		(P32)
#define READ_TURN_RIGHT()		(P31)

void LRFlashTask(void)
{
	static uint8_t ucLeftOn=0	,ucLeftOff=0;
	static uint8_t ucRightOn=0	,ucRightOff=0;
	static uint8_t ucLeftCount=0,ucRightCount=0;

	if ( READ_TURN_LEFT() ){	//ON
        ucLeftOff = 0;
        if ( ucLeftOn ++ > 10 ){		//200ms 滤波
            if ( ucLeftOn > 100 ){
          	    ucLeftOn = 101;
                sBike.bLFlashType = 0;
            }
           	if ( ucLeftCount < 0xFF-50 ){
	            ucLeftCount++;
            }
			sBike.bLeftFlash= 1;
			sBike.bTurnLeft 	= 1;
        }
	} else {					//OFF
        ucLeftOn = 0;
        if ( ucLeftOff ++ == 10 ){
        	ucLeftCount += 50;	//500ms
			sBike.bLeftFlash	= 0;
        } else if ( ucLeftOff > 10 ){
	        ucLeftOff = 11;
            sBike.bLFlashType = 1;
            if ( ucLeftCount == 0 ){
				sBike.bTurnLeft = 0;
            } else
				ucLeftCount --;
		}
	}
	
	if ( READ_TURN_RIGHT() ){	//ON
        ucRightOff = 0;
        if ( ucRightOn ++ > 10 ){
            if ( ucRightOn > 100 ){
          	    ucRightOn = 101;
                sBike.bRFlashType = 0;
            }
           	if ( ucRightCount < 0xFF-50 ){
				ucRightCount++;
            }
			sBike.bRightFlash= 1;
			sBike.bTurnRight 	= 1;
        }
	} else {					//OFF
        ucRightOn = 0;
        if ( ucRightOff ++ == 10 ){
        	ucRightCount += 50;	//500ms
			sBike.bRightFlash = 0;
        } else if ( ucRightOff > 10 ){
	        ucRightOff = 11;
            sBike.bRFlashType = 1;
            if ( ucRightCount == 0 ){
				sBike.bTurnRight = 0;
            } else
				ucRightCount --;
		}
	}
}

#define TASK_INIT	0
#define TASK_STEP1	1
#define TASK_STEP2	2
#define TASK_STEP3	3
#define TASK_STEP4	4
#define TASK_EXIT	5

unsigned char MileResetTask(void) AT(BIKE_CODE)
{
	static uint16_t uiPreTick=0;
	static uint8_t TaskFlag = TASK_INIT;
	static uint8_t ucCount = 0;
	uint8_t ret = 1;
	
    if ( TaskFlag == TASK_EXIT )
        return 0;
    
	if ( Get_ElapseTick(uiPreTick) > 10000 | sBike.bBraked | sBike.ucSpeed )
		TaskFlag = TASK_EXIT;

	switch( TaskFlag ){
	case TASK_INIT:
		if ( Get_SysTick() < 3000 && sBike.bTurnRight == 1 ){
			TaskFlag = TASK_STEP1;
			ucCount = 0;
		}
		break;
	case TASK_STEP1:
		if ( sBike.bLastNear == 0 && sBike.bNearLight ){
			uiPreTick = Get_SysTick();
			if ( ++ucCount >= 8 ){
				TaskFlag = TASK_STEP2;
				ucCount = 0;
				sBike.bMileFlash = 1;
				sBike.ulMile = sConfig.ulMile;
			}
		}
		sBike.bLastNear = sBike.bNearLight;
		break;
	case TASK_STEP2:
		if ( sBike.bTurnRight == 0 && sBike.bTurnLeft == 1 ) {
			ucCount = 0;
			TaskFlag = TASK_EXIT;
			sBike.bMileFlash = 0;
			sBike.ulFMile 	= 0;
			sBike.ulMile 	= 0;
			sConfig.ulMile 	= 0;
			WriteConfig();
		} else if ( sBike.bTurnRight == 0 && sBike.bTurnLeft == 0 ) {
			if ( sBike.bLastNear == 0 && sBike.bNearLight){
				uiPreTick = Get_SysTick();
				if ( ++ucCount >= 4 ){
					TaskFlag = TASK_STEP3;
					if ( sConfig.uiSingleTrip ){
						sConfig.uiSingleTrip = 0;
						sBike.ulMile = 99999UL;
					} else {
						sConfig.uiSingleTrip = 1;
						sBike.ulMile = 0;
					}
					WriteConfig();
				}
			}
		}
		sBike.bLastNear = sBike.bNearLight;
		break;
	case TASK_STEP3:
		if ( Get_ElapseTick(uiPreTick) > 3000 ) {
			TaskFlag = TASK_EXIT;
			if ( sConfig.uiSingleTrip )
				sBike.ulMile = 0;
			else
				sBike.ulMile = sConfig.ulMile;
			sBike.bMileFlash = 0;
		}
		break;
	case TASK_EXIT:
	default:
		sBike.bMileFlash = 0;
		ret = 0;
		break;
	}
	return ret;
}

void MileTask(void) AT(BIKE_CODE)
{
	static uint16_t uiTime = 0;
	uint8_t uiSpeed;
	
	if ( MileResetTask() )
		return ;

	uiSpeed = sBike.ucSpeed;
	if ( uiSpeed > DISPLAY_MAX_SPEED )
		uiSpeed = DISPLAY_MAX_SPEED;
	
//#ifdef SINGLE_TRIP
	uiTime ++;
	if ( uiTime < 20 ) {	//2s
		if ( sConfig.uiSingleTrip == 0 )
			uiTime = 51;
		sBike.ulMile = sConfig.ulMile;
	} else if ( uiTime < 50 ) { 	//5s
		if ( uiSpeed ) {
			uiTime = 50;
			sBike.ulMile = 0;
		}
	} else if ( uiTime == 50 ){
		sBike.ulMile = 0;
	} else
//#endif	
	{
		uiTime = 51;
		
		sBike.ulFMile = sBike.ulFMile + uiSpeed;
		if(sBike.ulFMile >= 36000)
		{
			sBike.ulFMile = 0;
			sBike.ulMile++;
			if ( sBike.ulMile > 99999 )	sBike.ulMile = 0;
			sConfig.ulMile ++;
			if ( sConfig.ulMile > 99999 )	sConfig.ulMile = 0;
			WriteConfig();
		}
	}
}

uint8_t SpeedCaltTask(void)
{
	static uint16_t uiPreTick=0;
	static uint8_t TaskFlag = TASK_INIT;
	static uint8_t ucLastSpeed = 0;
	static uint8_t ucCount = 0;
    static signed char cSpeedInc=0;
	static uint8_t yxterr=0;
	
    if ( TaskFlag == TASK_EXIT )
      	return 0;
    
	if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bBraked )
		TaskFlag = TASK_EXIT;

	switch( TaskFlag ){
	case TASK_INIT:
		if ( Get_SysTick() < 3000 && sBike.bTurnLeft == 1 ){
			TaskFlag = TASK_STEP1;
			ucCount = 0;
		}
		break;
	case TASK_STEP1:
		if ( sBike.bLastNear == 0 && sBike.bNearLight){
			if ( ++ucCount >= 8 ){
				TaskFlag 	= TASK_STEP2;
				ucCount 	= 0;
				cSpeedInc 	= 0;
				sBike.bSpeedFlash = 1;
				yxterr = sBike.bYXTERR;
				if ( yxterr )
					sConfig.uiSpeedScale 	 = 1000;
				else
					sConfig.uiYXT_SpeedScale = 1000;
			}
			uiPreTick = Get_SysTick();
		}
		sBike.bLastNear = sBike.bNearLight;
		break;
	case TASK_STEP2:
        if ( sConfig.uiSysVoltage == 48 )
			sBike.ucSpeed = 42;
        else if ( sConfig.uiSysVoltage == 60 )
			sBike.ucSpeed = 44;
        else if ( sConfig.uiSysVoltage == 72 )
			sBike.ucSpeed = 44;

		if ( sBike.bLastNear == 0 && sBike.bNearLight == 1 ){
			uiPreTick = Get_SysTick();
            if ( sBike.bTurnLeft == 1 ) {
				ucCount = 0;
				if ( sBike.ucSpeed + cSpeedInc > 1 )
					cSpeedInc --;
	        } else if ( sBike.bTurnRight == 1 ) {
				ucCount = 0;
                if ( sBike.ucSpeed + cSpeedInc < 99 )
					cSpeedInc ++;
            } else {
				if ( ++ucCount >= 5 ){
					TaskFlag = TASK_EXIT;
					sBike.bSpeedFlash = 0;
					if ( sBike.ucSpeed ) {
						if ( yxterr )
							sConfig.uiSpeedScale 	 = (uint32_t)sBike.ucSpeed*1000UL/(sBike.ucSpeed+cSpeedInc);
						else
							sConfig.uiYXT_SpeedScale = (uint32_t)sBike.ucSpeed*1000UL/(sBike.ucSpeed+cSpeedInc);
						WriteConfig();
					}
				}
            }
		}
		sBike.bLastNear = sBike.bNearLight;

		if ( ucLastSpeed && sBike.ucSpeed == 0 ){
			TaskFlag = TASK_EXIT;
		}
        
		//if ( sBike.ucSpeed )
		//	uiPreTick = Get_SysTick();

        sBike.ucSpeed += cSpeedInc;
		ucLastSpeed = sBike.ucSpeed;
		break;
	case TASK_EXIT:
	default:
		sBike.bSpeedFlash = 0;
		break;
	}
	return 0;
}

#if ( TIME_ENABLE == 1 )
void TimeTask(void) AT(BIKE_CODE)
{
	static uint8_t ucTask=0,ucLeftOn= 0,NL_on = 0;
	static uint16_t uiPreTick;
	
	if (!sBike.bHasTimer)
		return ;
	
	if (sBike.ucSpeed > 1)
		ucTask = 0xff;
	
	switch ( ucTask ){
	case 0:
		if ( Get_SysTick() < 5000 && sBike.bNearLight == 0 && sBike.bTurnLeft == 1 ){
			uiPreTick = Get_SysTick();
			ucTask++;
		}
		break;
	case 1:
		if ( sBike.bTurnLeft == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 2:
		if ( sBike.bTurnRight == 1 ){
			if ( Get_ElapseTick(uiPreTick) > 1000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000  || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 3:
		if ( sBike.bTurnRight == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 4:
		if ( sBike.bTurnLeft == 1 ){
			if ( Get_ElapseTick(uiPreTick) > 1000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000  || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 5:
		if ( sBike.bTurnLeft == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 6:
		if ( sBike.bTurnRight == 1 ){
			if ( Get_ElapseTick(uiPreTick) > 1000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000  || sBike.bNearLight ) ucTask = 0xFF;
		}
	case 7:
		if ( sBike.bTurnRight == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 8:
		if ( sBike.bTurnLeft == 1 ){
			if ( Get_ElapseTick(uiPreTick) > 1000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000  || sBike.bNearLight ) ucTask = 0xFF;
		}
	case 9:
		if ( sBike.bTurnLeft == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 10:
		if ( sBike.bTurnRight == 1 ){
			if ( Get_ElapseTick(uiPreTick) > 1000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000  || sBike.bNearLight ) ucTask = 0xFF;
		}
	case 11:
		if ( sBike.bTurnRight == 0 ){
			if ( Get_ElapseTick(uiPreTick) < 2000  ) ucTask = 0xFF;	else { uiPreTick = Get_SysTick(); ucTask++; }
		} else {
			if ( Get_ElapseTick(uiPreTick) > 10000 || sBike.bNearLight ) ucTask = 0xFF;
		}
		break;
	case 12:
		if ( sBike.bTurnLeft == 1 || sBike.bNearLight ){
			 ucTask = 0xFF;
		} else {
			if ( Get_ElapseTick(uiPreTick) > 1000 ) {
				ucTask= 0;
				sBike.ucTimePos = 0;
				sBike.bTimeSet = 1;
				uiPreTick = Get_SysTick();
			}
		}
		break;
	default:
		sBike.ucTimePos = 0;
		sBike.bTimeSet = 0;
		ucTask = 0;
		break;
	}

	if ( sBike.bTimeSet ){
		if ( sBike.bTurnLeft ) { ucLeftOn = 1; }
		if ( sBike.bTurnLeft == 0 ) {
			if ( ucLeftOn == 1 ){
				sBike.ucTimePos ++;
				sBike.ucTimePos %= 4;
				ucLeftOn = 0;
				uiPreTick = Get_SysTick();
			}
		}
		if ( sBike.bNearLight ) { NL_on = 1; uiPreTick = Get_SysTick(); }
		if ( sBike.bNearLight == 0 && NL_on == 1 ) {
			NL_on = 0;
			if ( Get_ElapseTick(uiPreTick) < 5000 ){
				switch ( sBike.ucTimePos ){
				case 0:
					sBike.ucHour += 10;
					sBike.ucHour %= 20;
					break;
				case 1:
					if ( sBike.ucHour % 10 < 9 )
						sBike.ucHour ++;
					else
						sBike.ucHour -= 9;
					break;
				case 2:
					sBike.ucMinute += 10;
					sBike.ucMinute %= 60;
					break;
				case 3:
					if ( sBike.ucMinute % 10 < 9 )
						sBike.ucMinute ++;
					else
						sBike.ucMinute -= 9;
					break;
				default:
					sBike.bTimeSet = 0;
					break;
				}
			}
			RtcTime.RTC_Hours 	= sBike.ucHour;
			RtcTime.RTC_Minutes = sBike.ucMinute;
			//PCF8563_SetTime(PCF_Format_BIN,&RtcTime);
		}
		if ( Get_ElapseTick(uiPreTick) > 30000 ){
			sBike.bTimeSet = 0;
		}
	}		
	
	//PCF8563_GetTime(PCF_Format_BIN,&RtcTime);
	sBike.ucHour 	= RtcTime.RTC_Hours%12;
	sBike.ucMinute 	= RtcTime.RTC_Minutes;
}
#endif


void BikeCalibration(void) AT(BIKE_CODE)
{
	unsigned char i;
	unsigned int vol;
	
    if(  AD_var.wADValue[AD_CH_KEY] < (100UL<<6) && P36 == 1 && P37 == 1 ){
		for(i=0;i<32;i++){
			if(  AD_var.wADValue[AD_CH_KEY] > (100UL<<6) || P36 == 0 || P37 == 0 )
                break;
			delay_n10ms(2);
            WDT_CLEAR();
		}
		if ( i == 32 ){
			for(i=0;i<0xFF;i++){
	            delay_n10ms(10);
                if ( GetVolStabed(&vol) /*&& (400 < vol) && (vol < 600)*/ ){
					sBike.uiVoltage			= vol;
		//			sConfig.uiVolScale		= 1000;
					sConfig.uiVolScale		= (unsigned long)vol*1000UL/VOL_CALIBRATIOIN;
					sConfig.uiSysVoltage 	= 60;
					sConfig.uiTempScale 		= 1000;
					sConfig.uiSpeedScale		= 1000;
					sConfig.uiYXT_SpeedScale	= 1000;
                    sConfig.uiSingleTrip		= 0;
					sConfig.ulMile			= 0;
					WriteConfig();
                    sBike.bVolFlash 			= 1;
                    break;
                }
                WDT_CLEAR();
			}
		}
    }
}


void Light_Task(void) AT(BIKE_CODE)
{
	unsigned char speed_mode=0;
	
	//if ( sBike.YXTERR )
    {
		P3PU 	&=~(BIT(2)|BIT(1)|BIT(0));
		P3PD 	&=~(BIT(2)|BIT(1)|BIT(0));
		P3DIE 	|= (BIT(2)|BIT(1)|BIT(0));
		P3DIR 	|= (BIT(2)|BIT(1)|BIT(0));
	
		if( P30 ) {
		  sBike.bNearLight = 1;
		  P2PU  |= BIT(0);  //背光调节
		  P2HD  |= BIT(0);
		  P2DIE |= BIT(0);
		  P2DIR &=~BIT(0);
		  P20    = 0;
		} else {
		  sBike.bNearLight = 0;
		  P2PU  |= BIT(0);  //背光调节
		  P2HD  |= BIT(0);
		  P2DIE |= BIT(0);
		  P2DIR &=~BIT(0);
		  P20    = 1;
		}
		//if( P31 ) sBike.bTurnRight = 1; else sBike.bTurnRight = 0;
		//if( P32 ) sBike.bTurnLeft  = 1; else sBike.bTurnLeft  = 0;

		sBike.ucPHA_Speed 	= (unsigned long)GetSpeed();
		sBike.ucSpeed 		= (unsigned long)sBike.ucPHA_Speed*1000UL/sConfig.uiSpeedScale;
    	//deg("ucPHA_Speed=%u,SpeedScale=%u,Speed=%u\n",sBike.ucPHA_Speed,sConfig.SpeedScale,sBike.ucSpeed);
    }	
}


void BikePowerUp(void)
{
	HotReset();
	if ( sBike.bHotReset == 0 ){
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
	static unsigned int uiCount=0;
	unsigned int uiVol=0;
	static unsigned int task=BIKE_INIT;	
	
	sBike.uiTick += 100;
	
	switch(task){
	case BIKE_INIT:

		for(i=0;i<32;i++){	GetVolSample(); }
	//	for(i=0;i<16;i++){	GetSpeed();	/*IWDG_ReloadCounter(); */ }
	//	for(i=0;i<4;i++) {	GetTemp();	/*IWDG_ReloadCounter(); */ }

		InitConfig();
		BikeCalibration();

	#if ( TIME_ENABLE == 1 )	
		//sBike.HasTimer = !PCF8563_Check();
		//sBike.HasTimer = PCF8563_GetTime(PCF_Format_BIN,&RtcTime);
	#endif
  	
		GetVolStabed(&uiVol);
		sBike.uiVoltage 	= (unsigned long)uiVol*1000UL/sConfig.uiVolScale;
		sBike.ucBatStatus= GetBatStatus(sBike.uiVoltage);

		if ( sBike.bHotReset == 0 ) {
			task = BIKE_RESET_WAIT;
		} else {
			task = BIKE_SETUP;
		}
		break;
	case BIKE_RESET_WAIT:
        while( 1 )
        {
			if ( Get_SysTick() > PON_ALLON_TIME ){
				BL55072_Config(0);
				task = BIKE_SETUP;
                break;
			}
            delay_n10ms(10);
            sBike.uiTick += 100;
            WDT_CLEAR();
        }
		break;
    case BIKE_SETUP:
        task = BIKE_RUN;
	case BIKE_RUN:
		//uiTick = Get_SysTick();
		
		//if ( (uiTick >= tick_100ms && (uiTick - tick_100ms) >= 100 ) || \
		//	 (uiTick <  tick_100ms && (0xFFFF - tick_100ms + uiTick) >= 100 ) ) {
		//	tick_100ms = uiTick;
			uiCount ++;

            if ( (uiCount % 5) == 0 ) {
                if ( GetVolStabed(&uiVol) ){
					sBike.uiVoltage = (uint32_t)uiVol*1000UL/sConfig.uiVolScale;
					sBike.ucBatStatus= GetBatStatus(sBike.uiVoltage);
                }
            }
#if 0
			if ( (uiCount % 10) == 0 ){
				if ( sBike.bUart == 0 ){
				//	sBike.siTemperature= (long)GetTemp()	*1000UL/sConfig.TempScale;
					sBike.siTemperature= GetTemp();
				}
			}
#endif
		#ifdef LCD8794GCT
			//sBike.ucEnergy 	= GetBatEnergy(sBike.uiVoltage);
		#endif
		
			Light_Task();
			MileTask();
			
		#if ( YXT_ENABLE == 1 )
			YXT_Task(&sBike,&sConfig);  
		#endif
			
			SpeedCaltTask();
					
		#if ( TIME_ENABLE == 1 )	
			TimeTask();
		#endif

		#ifdef LCD_SEG_TEST
			if ( uiCount >= 100 ) uiCount = 0;
			sBike.ucVoltage 	= uiCount/10 + uiCount/10*10UL + uiCount/10*100UL + uiCount/10*1000UL;
			sBike.siTemperature	= uiCount/10 + uiCount/10*10UL + uiCount/10*100UL;
			sBike.ucSpeed		= uiCount/10 + uiCount/10*10;
			sBike.ulMile		= uiCount/10 + uiCount/10*10UL + uiCount/10*100UL + uiCount/10*1000UL + uiCount/10*10000UL;
			sBike.ucHour       	= uiCount/10 + uiCount/10*10;
			sBike.ucMinute     	= uiCount/10 + uiCount/10*10;
		#endif

            MenuUpdate(&sBike);
	
		//}
		break;
	default:
		task = BIKE_INIT;
		break;
	}
}

