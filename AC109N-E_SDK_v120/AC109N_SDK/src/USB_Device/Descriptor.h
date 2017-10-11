#ifndef _DESCRIPTOR_H_
#define _DESCRIPTOR_H_

#ifdef __IAR_SYSTEMS_ICC__

#define USB_SLAVE_INF		(MASSSTORAGE_INF)
#define USB_SLAVE_CFG		(MASSSTORAGE_CFG + 9)

///////////MassStorage Class
#define	    BULK_EP_OUT		     		1
#define	    BULK_EP_IN		     		1

#define	    MAXP_SIZE_BULKOUT				64
#define	    MAXP_SIZE_BULKIN				64

#define 	MASSSTORAGE_CFG           (23)
#define 	MASSSTORAGE_INF              1


/////////////Audio Class
#define     ISO_EP_OUT           		2

#define     SPK_AUDIO_RATE      48
#define		SPK_AUDIO_RES		16

#define		SPK_CHANNEL			2
#define		SPK_FRAME_LEN		(SPK_AUDIO_RATE * SPK_AUDIO_RES / 8 * SPK_CHANNEL)

#define		SPK_PCM_Type		(SPK_AUDIO_RES >> 4)				// 0=8 ,1=16
#define		SPK_AUDIO_TYPE		(0x02 - SPK_PCM_Type)			// TYPE1_PCM16


#define 	AUDIO_CFG       (0x65)
#define 	AUDIO_INF         2


////////////HID Class
#define     HID_EP_IN            		2
//#define     HID_EP_OUT           		2
#define     HID_CFG                     (25)
#define     HID_INF                     1
#define     HidReportDescriptors_len 	51

/*Main Items*/
#define INPUT(x)              (0x80 | 0x1), x
#define OUTPUT(x)             (0x90 | 0x1), x
#define COLLECTION(x)         (0xA0 | 0x1), x
#define FEATURE(x)            (0xB0 | 0x1), x
#define END_COLLECTION        0xC0

/*Golbal Items*/
#define USAGE_PAGE(x)         (0x04 | 0x1), x
#define LOGICAL_MIN(x)        (0x14 | 0x1), x
#define LOGICAL_MAX(x)        (0x24 | 0x1), x
#define PHYSICAL_MIN(x)       (0x34 | 0x1), x
#define PHYSICAL_MAX(x)       (0x44 | 0x1), x
#define UNIT_EXPONENT(x)      (0x54 | 0x1), x
#define UNIT(x)               (0x64 | 0x1), x
#define REPORT_SIZE(x)        (0x74 | 0x1), x
#define REPORT_ID(x)          (0x84 | 0x1), x
#define REPORT_COUNT(x)       (0x94 | 0x1), x
#define PUSH(x)               (0xA4 | 0x1), x
#define POP(x)                (0xB4 | 0x1), x

/*Local Items*/
#define USAGE(x)              (0x08 | 0x1), x
#define USAGE2(x)             (0x08 | 0x2), x
#define USAGE_MIN(x)          (0x18 | 0x1), x
#define USAGE_MAX(x)          (0x28 | 0x1), x
#define DESIGNATOR_INDEX(x)   (0x38 | 0x1), x
#define DESIGNATOR_MIN(x)     (0x48 | 0x1), x
#define DESIGNATOR_MAX(x)     (0x58 | 0x1), x
#define STRING_INDEX(x)       (0x78 | 0x1), x
#define STRING_MIN(x)         (0x88 | 0x1), x
#define STRING_MAX(x)         (0x98 | 0x1), x
#define DELIMITER(x)          (0xA8 | 0x1), x


/*Consumer Page*/
#define CONSUMER_PAGE           0x0C
#define CONSUMER_CONTROL        0x01

//Collection
#define PHYSICAL                0x00
#define APPLICATION             0x01
#define LOGICAL                 0x02
#define REPORT                  0x03

#define PLAY                    0xB0
#define PAUSE                   0xB1
#define RECORD                  0xB2
#define FAST_FORWARD            0xB3
#define REWIND                  0xB4
#define SCAN_NEXT_TRACK         0xB5
#define SCAN_PREV_TRACK         0xB6
#define STOP                    0xB7

#define FRAME_FORWARD           0xC0
#define FRAME_BACK              0xC1
#define TRACKING_INC            0xCA
#define TRACKING_DEC            0xCB
#define STOP_EJECT              0xCC
#define PLAY_PAUSE              0xCD
#define PLAY_SKIP               0xCE

#define VOLUME                  0xE0
#define BALANCE                 0xE1
#define MUTE                    0xE2
#define BASS                    0xE3
#define VOLUME_INC              0xE9
#define VOLUME_DEC              0xEA

#define BALANCE_LEFT            0x50, 0x01
#define BALANCE_RIGHT           0x51, 0x01
#define CHANNEL_LEFT            0x61, 0x01
#define CHANNEL_RIGHT           0x62, 0x01
//----------------------------------
// HID key for audio
//----------------------------------
#define USB_AUDIO_STOP          0x8000
#define USB_AUDIO_FASTFORWARD   0x4000
#define USB_AUDIO_PREFILE       0x2000
#define USB_AUDIO_NEXTFILE      0x1000
#define USB_AUDIO_PP            0x0800
#define USB_AUDIO_MUTE          0x0400
#define USB_AUDIO_VOLDOWN       0x0200
#define USB_AUDIO_VOLUP         0x0100
#define USB_AUDIO_NONE          0x0000

#define USB_AUDIO_PAUSE         0x0080
#define USB_AUDIO_PLAY          0x0040
#define USB_AUDIO_BALANCE_LEFT  0x0020
#define USB_AUDIO_BALANCE_RIGHT 0x0010
#define USB_AUDIO_VOLUME        0x0008
#define USB_AUDIO_STOP_EJECT    0x0004
#define USB_AUDIO_TRACKING_DEC  0x0002
#define USB_AUDIO_TRACKING_INC  0x0001
#define USB_AUDIO_NONE          0x0000


#define DEV_LEN     0x12

//---------------------------------------------------------------------------------------------------------------------
// DEVICE DESCRIPTOR
//---------------------------------------------------------------------------------------------------------------------
const u8 sDeviceDescriptor[] =  //<Device Descriptor
    {
        DEV_LEN,      // bLength: Size of descriptor
        0x01,       // bDescriptorType: Device
        0x10,0x01,      // bcdUSB: USB 1.1
        0x00,       // bDeviceClass: none
        0x00,       // bDeviceSubClass: none
        0x00,       // bDeviceProtocol: none
        8,//EP0_LEN,      // bMaxPacketSize0: 8/64 bytes
        0xB8,0xE2,      // idVendor: 0x04B4 - Cypress
        0x11,0x08,      // idProduct: 0x1111 - SL811S
        0x00,0x01,      // bcdDevice: version 1.0
        0x01,       // iManufacturer: Cypress
        0x02,       // iProduct: Cypress SL811S - HID Consumer Control
        0x01,       // iSerialNumber: none
        0x01        // bNumConfigurations: 1
    };

const u8 sConfigDescriptor[] = 	//<Config Descriptor
{
//ConfiguraTIon
    0x09,    //Length
    0x02,    //DescriptorType : ConfigDescriptor
    USB_SLAVE_CFG/*0x20*/,
    0 >> 8, //TotalLength
    USB_SLAVE_INF/*1*/,      //NumInteRFaces:2
    0x01,    //ConfigurationValue
    0x00,    //Configuration String
    0x80,    //Attributes:Bus Power
    200,     //MaxPower = 200*2ma
};

/*********************************************************/
/*
			   Mass Storage Class
*/
/*********************************************************/
const u8 sMassDescriptor[] = 	//<Interface & Endpoint Descriptor
{
///Interface
    0x09,   //Length
    0x04,   //DescriptorType:Inerface
    0x00,   //InterfaceNum:0  !!
    0x00,   //AlternateSetting:0
    0x02,   //NumEndpoint:0
    0x08,   //InterfaceClass:MSD
    0x06,   //InterfaceSubClass
    0x50,   //InterfaceProtocol
    0x00,   //Interface String
///Endpoint
    0x07,0x05,0x80 | BULK_EP_IN,0x02,MAXP_SIZE_BULKIN & 0xFF,MAXP_SIZE_BULKIN >> 0x8,0x01,
    0x07,0x05,BULK_EP_OUT,0x02,MAXP_SIZE_BULKOUT & 0xFF,MAXP_SIZE_BULKOUT >> 0x8,0x01,
};


/*********************************************************/
/*
			   Audio Class
*/
/*********************************************************/
const u8 sAduioDescriptor[] =
{
//standard interface AC descriptor(Interface 0, Alternate Setting 0):
    0x09,       //Length
    0x04,       //DescriptorType:Inerface
    0x01,       //InterfaceNum:0  !!
    0x00,       //AlternateSetting:0
    0x00,       //NumEndpoint:0
    0x01,       //InterfaceClass:audio
    0x01,       //InterfaceSubClass:audio ctl
    0x00,       //InterfaceProtocol
    0x00,       //Interface String

//class-specific AC interface descriptor
    0x09,    //Length
    0x24,      //DescriptorType:audio interface descriptor
    0x01,     //DescriptorSubType:audio control header
    0x00,0x01,  //bcdADC:audio Device Class v1.00
    0x28,0x00,  //TotalLength of class-specific AC interface descriptors
    0x01,      //InCollection:1 AudioStreaming interface
    0x02,      //InterfaceNr(1) - AS #2 id AudioStreaming interface 2 belongs to this AudioControl interface

//USB USB Streaming IT
    0x0c,       //Length
    0x24,       //DescriptorType:audio interface descriptor
    0x02,       //DescriptorSubType:Input Terminal
    0x01,       //TerminalID:0x01
    0x01,0x01,  //TerminalType:USB Streaming
    0x00,       //AssocTerminal
    0x02,       //NrChannels:2 channel
    0x03,0x00,  //ChannelConfig:Left Front,Right Front,
    0x00,       //ChannelName String
    0x00,       //Terminal String

//Audio Feature Unit Descriptor
//SourceId 0x04,ControlSize 0x01,Mute,Volume,
    0x0a,    //Length
    0x24,      //DescriptorType:audio interface descriptor
    0x06,       //DescriptorSubType:Audio Feature Unit
    0x09,       //UnitID:0x09
    0x01,      //SourceID:1 #USB Streaming IT
    0x01,      //ControlSize:1 byte
    0x01,      //Controls:Mute,
    0x02,       //Controls(0):Left Volume
    0x02,      //Controls(1):Right Volume
    0x00,       //Feature String

//USB Speaker OT
    0x09,      //Length
    0x24,      //DescriptorType:audio interface descriptor
    0x03,      //DescriptorSubTYpe:Output Terminal
    0x03,      //TerminalID:0x03
    0x01,0x03,  //TerminalType:Speaker
    0x00,       //AssocTerminal:
    0x09,      //SourceID:9 #Feature UNIT
    0x00,      //Terminal String

//-------------------Speaker  interface---------------------//
//standard interface AS descriptor(Interface 1, Alternate Setting 0):
    0x09,      //Length
    0x04,      //DescriptorType:Interface
    0x02,       //InterfaceNum:1    !!
    0x00,       //AlternateSetting:0
    0x00,      //NumEndpoint:0
    0x01,       //InterfaceClass:audio
    0x02,      //InterfaceSubClass:audio streaming
    0x00,      //InterfaceProtocol
    0x00,       //Interface String

//standard interface AS descriptor(Interface 2, Alternate Setting 1):
    0x09,      //Length
    0x04,      //DescriptorType:Interface
    0x02,       //InterfaceNum:1    !!
    0x01,       //AlternateSetting:1
    0x01,       //NumEndpoint:1
    0x01,      //InterfaceClass:audio
    0x02,       //InterfaceSubClass:audio streaming
    0x00,       //InterfaceProtocol
    0x00,      //Interface String

//Audio Streaming Interface Descriptor:AS_GENERAL(0x01),
    0x07,      //Length
    0x24,       //DescriptorType:audio interface descriptor
    0x01,       //DescriptorSubType:AS_GENERAL
    0x01,      //TerminalLink:#1 USB Streaming IT
    0x01,      //Delay:1
    (SPK_AUDIO_TYPE & 0xFF), (SPK_AUDIO_TYPE >> 8),  //FormatTag:PCM

//Type 1 Format type descriptor
    0x0b,     //Length
    0x24,       //DescriptorType:audio interface descriptor
    0x02,      //DescriptorSubType:Format_type
    0x01,       //FormatType:Format type 1
    SPK_CHANNEL,       //NumberOfChannel:2
    0x02,       //SubframeSize:2byte
    SPK_AUDIO_RES,      //BitsResolution:16bit
    0x01,      //SampleFreqType:One sampling frequency.
    (((u16)SPK_AUDIO_RATE*1000) & 0xFF), (((u16)SPK_AUDIO_RATE*1000) >> 8), 0x00,   //48K(0x00BB80)

//Endpoint 1 - Standard Descriptor:Output Endpoint1
//Isochronous,Synchronization Type(Asynchronous)
    0x09,      //Length
    0x05,       //DescriptorType:endpoint descriptor
    ISO_EP_OUT,       //EndpointAddress:Output endpoint 1
    0x09,
    SPK_FRAME_LEN, //MaxPacketSize
    0x00,      //Asynchronous
    0x01,
    0x0,      //Interval
    0x0,

//Endpoint - Audio Streaming Descriptor
//Audio Endpoint descriptor,General
    0x07,       //Length
    0x25,       //DescriptorType:audio endpoint descriptor
    0x01,       //DescriptorSubType:audio endpiont general
    0x00,      //Attributes:0x00.............
    0x00,       //LockDelayUnits
    0x00,0x00,  //LockDelay
};

/*********************************************************/
/*
			   HID Class
*/
/*********************************************************/
u8 const sHIDDescriptor[] =
{
//HID
    //InterfaceDescriptor:
    0x09, 0x04    , // Length, Type
    0x03,
    0x00,0x01 , // No alternate setting
    0x03    , // Class = Human Interface Device
    0x00, 0x00  , // Subclass and Protocol
    0x00    , // Interface Name

    //HIDDescriptor:
    0x09, 0x21  , // Length, Type
    0x01, 0x02  , // HID Class Specification compliance
    0x00    , // Country localization (=none)
    0x01    , // Number of descriptors to follow
    0x22    , // And it's a Report descriptor
    51, 0x00  , //LOW(ReportLength), HIGH(ReportLength) , //Wheel

    //EndpointDescriptor:
    0x07, 0x05  , // Length, Type
    0x80|HID_EP_IN    , // Address = IN 2
    0x03    , // Interrupt
    0x08, 0x00  , // Maximum packet size
    0x01    , // Poll every 10msec seconds
};

u8 const sHIDReport[] =
{
    USAGE_PAGE(CONSUMER_PAGE),
    USAGE(CONSUMER_CONTROL),
    COLLECTION(APPLICATION),

    LOGICAL_MIN(0x00),
    LOGICAL_MAX(0x01),

    USAGE(VOLUME_INC),
    USAGE(VOLUME_DEC),
    USAGE(MUTE),
    USAGE(PLAY_PAUSE),
    USAGE(SCAN_NEXT_TRACK),
    USAGE(SCAN_PREV_TRACK),
    USAGE(FAST_FORWARD),
    USAGE(STOP),

    USAGE(TRACKING_INC),
    USAGE(TRACKING_DEC),
    USAGE(STOP_EJECT),
    USAGE(VOLUME),
    USAGE2(BALANCE_LEFT),
    USAGE2(BALANCE_RIGHT),
    USAGE(PLAY),
    USAGE(PAUSE),

    REPORT_SIZE(0x01),
    REPORT_COUNT(0x10),
    INPUT(0x42),
    END_COLLECTION
};

const u8 LANGUAGE_STR[] =
{
    0x04, 0x03, 0x09, 0x04
};

const u8 CHIPSERIAL_NUM[] =
{
    14, 0x03, 'C',0,'D',0,'0',0,'0',0, '2', 0, 0, 0
};

const u8 MANUFACTURE_STR[] =
{
    14, 0x03, 'C',0,'D',0,'0',0,'0',0, '2', 0, 0, 0
};

/*0-ÅÌ·ûÃèÊö·û*/
const u8 SCSIInquiryData[] =
{
    0x00, 0x80, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,
    'J','I','E',' ','L','I',' ',' ',
    'C', 'D', '0', '0', '2', '-', '1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    0x31, 0x2e, 0x30, 0x30			//version
};

/*1-ÅÌ·ûÃèÊö·û*/
const u8 SCSIInquiryData1[]	=
{
    0x00, 0x80, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,
    'H','o','H','o','H','o','n','g',
    'S', 'T', 'A', 'R', '-', '2', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    0x31, 0x2e, 0x30, 0x30			//version
};


const u8 bAudioVol[] =
{
    //0x00,0x00,
    0x00,0x00,              //cur
    //0x00,0x80,
    0x80, 0x00,             //min
    //0x00,0x00,
    0x80, 0xFF,             //max
    //0x00,0x04,
    0x00, 0x01,              //res
};
#endif  /*  __IAR_SYSTEMS_ICC__ */

#endif  /*  _DESCRIPTOR_H_  */
