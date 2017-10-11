/*************************************************************
File: typedef.h
Author:Juntham
Discriptor:
    数据类型重定义
Version:
Date：
*************************************************************/
#ifndef _typedef_h_
#define _typedef_h_


#ifdef WIN32
typedef unsigned char	u8, uint8;
typedef signed char		s8, int8;
typedef unsigned short	u16, uint16;
typedef signed short	s16, int16;
typedef unsigned int	u32, uint32;
typedef signed int		s32, int32;
typedef unsigned long	u64, uint64;
typedef signed long		s64, int64;
#endif

#ifdef __SMART_CPU__
typedef unsigned char	u8, uint8;
typedef signed char		s8, int8;
typedef unsigned int	u16, uint16, func;
typedef signed int		s16, int16;
typedef unsigned long	u32, uint32;
typedef signed long		s32, int32;
#endif

#define FALSE	0
#define TRUE    1

#define false	0
#define true    1

#ifdef WIN32
#define	LD_WORD(ptr)		(u16)(*(u16*)(u8*)(ptr))
#define	LD_DWORD(ptr)		(u32)(*(u32*)(u8*)(ptr))
#define	ST_WORD(ptr,val)	*(u16*)(u8*)(ptr)=(u16)(val)
#define	ST_DWORD(ptr,val)	*(u32*)(u8*)(ptr)=(u32)(val)
#endif

#ifdef __KEIL__
#define LD_WORD(ptr)	int2(ptr)
#define LD_DWORD(ptr)	int4(ptr)
#endif


#ifdef __IAR_SYSTEMS_ICC__
#define	LD_WORD(ptr)		(u16)(*(u16*)(u8*)(ptr))
#define	LD_DWORD(ptr)		(u32)(*(u32*)(u8*)(ptr))
#define	ST_WORD(ptr,val)	*(u16*)(u8*)(ptr)=(u16)(val)
#define	ST_DWORD(ptr,val)	*(u32*)(u8*)(ptr)=(u32)(val)
#endif


#ifdef __KEIL__
#define bool		bit
#define _xdata		xdata
#define _data		data
#define _idata		idata
#define _pdata		pdata
#define _bdata		bdata
#define _bit		bit
#define _sbit		sbit
#define _code		code
#define _small		small
#define _large      large
#define _root
#define _no_init
//extern void my_memcpy(u8 _xdata *s1, u8 _xdata *s2, u16 len);
//extern void my_memset(u8 _xdata *s1, u8 volume, u16 len);
#define my_memset	memset
#define my_memcpy   memcpy
#define AT(segment)
void delay_n10ms(u8);
#endif

#ifdef __IAR_SYSTEMS_ICC__
#include <stdbool.h>
#define _bool       bool
#define _xdata		__xdata
#define _data		__data
#define _idata		__idata
#define _pdata		__pdata
#define _bdata		__bdata
#define _bit		__bit
#define _code		__code
#define _root       __root
#define _no_init    __no_init
#define _monitor    __monitor

#define AT(segment) @ #segment
//#define my_memset	memset
//#define my_memcpy   memcpy

#endif

#ifdef WIN32
#define _xdata
#define _data
#define _idata
#define _pdata
#define _bdata
#define _code
#define _bit		bool
#define _sbit		bool
#define _root
#define _no_init
#define my_memset	memset
#define my_memcpy   memcpy
#define my_imemset	memset
#define my_imemcpy  memcpy
#define AT(segment)
#endif

#define BIT(n)		(1L << (n))

#endif

