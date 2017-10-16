#include "config.h"

#include "stdlib.h"
#include "bike.h"
#include "bl55072.h"

unsigned char _xdata BL_Data[19];
unsigned char _xdata flashflag = 0;
unsigned char _xdata TL_Flash = 0;
unsigned char _xdata TR_Flash = 0;

const unsigned char _code SegDataTime[10] AT(BIKE_TABLE_CODE)	= {0xF6,0x60,0xB5,0xF1,0x63,0xD3,0xD7,0x70,0xF7,0xF3};
const unsigned char _code SegDataVoltage[10] AT(BIKE_TABLE_CODE)= {0xFA,0x60,0xBC,0xF4,0x66,0xD6,0xDE,0x70,0xFE,0xF6};
const unsigned char _code SegDataMile[10] AT(BIKE_TABLE_CODE)	= {0xFA,0x60,0xBC,0xF4,0x66,0xD6,0xDE,0x70,0xFE,0xF6};
const unsigned char _code SegDataSpeed[10] AT(BIKE_TABLE_CODE)  = {0xFA,0x60,0xBC,0xF4,0x66,0xD6,0xDE,0x70,0xFE,0xF6};

#define LCD_LEFT()	{BL_Data[14] |= 0x01;}	//S1
#define LCD_RIGHT()	{BL_Data[15] |= 0x01;}	//S10
#define LCD_V()		{BL_Data[ 7] |= 0x01;}	//S9
#define LCD_KM()	{BL_Data[ 2] |= 0x01;}	//S3
#define LCD_KMH()	{BL_Data[ 0] |= 0x01;}	//S16
#define LCD_LIGHT()	{BL_Data[13] |= 0x01;}	//S2
#define LCD_FM()	{BL_Data[ 1] |= 0x01;}	//S14
#define LCD_BT()	{BL_Data[15] |= 0x04;}	//S13
#define LCD_MP3()	{BL_Data[15] |= 0x08;}	//S12
#define LCD_VDOT()	{BL_Data[ 3] |= 0x01;}	//S17
#define LCD_TDOT()	{BL_Data[16] |= 0x08;}	//P1
#define LCD_SEC()	{BL_Data[17] |= 0x08;}	//P2
#define LCD_DEGREE()	{BL_Data[6] |= 0x01;}	//S4
#define LCD_DEGREE_NIG()	{BL_Data[6] |= 0x01;}	//S4

void MenuUpdate(BIKE_STATUS* bike) AT(BIKE_CODE)
{
	unsigned char i = 0;
  
	flashflag++;
	flashflag %= 10;
	
	for(i=0;i<18;i++)	BL_Data[i] = 0x00;
	
	if( bike->TurnLeft	){
		TL_Flash++;
		TL_Flash %= 10;
		if ( TL_Flash < 5 ) LCD_LEFT();
	} else 
		TL_Flash = 0;
	
	if( bike->TurnRight	){
		TR_Flash++;
		TR_Flash %= 10;
		if ( TR_Flash < 5 ) LCD_RIGHT();
	} else 
		TR_Flash = 0;
	
	if( bike->NearLight 	) LCD_LIGHT();

	/***************************Battery Area Display**********************************/
	BL_Data[15] |= 0x10;	//T0
	switch ( bike->BatStatus ){
    case 0:
		if ( flashflag < 5 ) 
			BL_Data[15] &= 0xEF;break;	//T0
    case 1: BL_Data[ 8] |= 0x80;break;	//T10
    case 2: BL_Data[ 8] |= 0xC0;break;
    case 3: BL_Data[ 8] |= 0xE0;break;
    case 4: BL_Data[ 8] |= 0xF0;break;
    case 5: BL_Data[ 8] |= 0xF8;break;
    case 6: BL_Data[ 8] |= 0xFC;break;      
    case 7: BL_Data[ 8] |= 0xFE;break;
    case 8: BL_Data[ 8] |= 0xFF;break;          
    case 9: BL_Data[ 8] |= 0xFF;BL_Data[15] |= 0x80;break;          
    case 10:BL_Data[ 8] |= 0xFF;BL_Data[15] |= 0xC0;break;          
    default:break; 
	}

	/***************************Temp Area Display**********************************/
	BL_Data[ 6] |=  SegDataVoltage[abs(bike->Temperature/10)/10];
	BL_Data[ 4] |= (SegDataVoltage[abs(bike->Temperature/10)%10] & 0xF0);
	BL_Data[ 3] |= (SegDataVoltage[abs(bike->Temperature/10)%10] & 0x0F);       
	LCD_DEGREE();
	if (bike->Temperature < 0)
		LCD_DEGREE_NIG(); 
	
	/*************************** Voltage Display**********************************/
	BL_Data[ 7] |= SegDataVoltage[(bike->Voltage/100	)%10];
	BL_Data[ 6] |= SegDataVoltage[(bike->Voltage/10		)%10];
	BL_Data[ 4] |= SegDataVoltage[(bike->Voltage		)%10] & 0xF0;
	BL_Data[ 3] |= SegDataVoltage[(bike->Voltage		)%10] & 0x0F;       
		
	/***************************Time Area Display**********************************/
	if ( bike->HasTimer ){
		BL_Data[12] |= ( SegDataTime[bike->Hour%10]);
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
	}
	
	/*************************** Mile Display**********************************/  
	LCD_KM();	
	BL_Data[ 5] |= SegDataMile [(bike->Mile/10000)%10] & 0xF0; 
	BL_Data[ 4] |= SegDataMile [(bike->Mile/10000)%10] & 0x0F; 
	BL_Data[ 0] |= SegDataMile [(bike->Mile/1000 )%10] & 0xF0;
	BL_Data[ 5] |= SegDataMile [(bike->Mile/1000 )%10] & 0x0F;
	BL_Data[ 1] |= SegDataMile [(bike->Mile/100  )%10] & 0xF0; 
	BL_Data[ 0] |= SegDataMile [(bike->Mile/100  )%10] & 0x0F; 
	BL_Data[ 2] |= SegDataMile [(bike->Mile/10   )%10] & 0xF0;
	BL_Data[ 1] |= SegDataMile [(bike->Mile/10   )%10] & 0x0F; 
	BL_Data[ 3] |= SegDataMile [(bike->Mile   	 )%10] & 0xF0;
	BL_Data[ 2] |= SegDataMile [(bike->Mile	     )%10] & 0x0F; 
	if ( bike->MileFlash ){
		if ( flashflag < 5  ) {
			BL_Data[ 4] &= 0x01;
			BL_Data[ 5] &= 0x01;
			BL_Data[ 0] &= 0x01; 
			BL_Data[ 1] &= 0x01; 
			BL_Data[ 2] &= 0x01;
		}
	}

	/*************************** Speed Display**********************************/
	LCD_KMH();
	BL_Data[14] |= (SegDataSpeed[ bike->Speed/10 %10]);
	BL_Data[13] |= (SegDataSpeed[(bike->Speed	)%10]); 
	if ( bike->SpeedFlash ){
		if ( flashflag < 5  ) {
			BL_Data[14] &= 0x01;
			BL_Data[13] &= 0X01; 
		}
	}

	BL_Write_Data(0,18,BL_Data);
}


void Delay(unsigned long nCount) AT(BIKE_CODE)
{
  for(; nCount != 0; nCount--);
}

