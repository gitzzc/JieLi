/*--------------------------------------------------------------------------*/
/**@file    iic.c
   @brief   IIC 接口函数
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/#include "iic.h"
#include "RTC_API.h"
#include "sdmmc_api.h"
#include "play_file.h"
#include "device.h"
#include "IRTC.h"

bool iic_busy;

__code const u16 iic_io_tab[8] AT(TABLE_CODE)=
{
    (u16)iic_data_out,
    (u16)iic_data_in,
    (u16)iic_data_r,
    (u16)iic_data,
    (u16)iic_clk_out,
    (u16)iic_clk,
};


/*----------------------------------------------------------------------------*/
/** @brief: IIC dat I/O 设置输出函数
    @param: void
    @return:void
    @author:Juntham
    @note:  void iic_data_out(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void iic_data_out(void) AT(COMMON_CODE)
{
#ifdef IIC_DAT_USE_P02
    P0DIR &= ~(1<<2);
#else  
    P0DIR &= ~(1<<1);
#endif
}


/*----------------------------------------------------------------------------*/
/** @brief: IIC dat I/O 设置输入函数
    @param: void
    @return:void
    @author:Juntham
    @note:  void iic_data_in(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void iic_data_in(void) AT(COMMON_CODE)
{
#ifdef IIC_DAT_USE_P02
    P0DIR |= (1<<2);
    P0PU |= (1<<2);
#else  
    P0DIR |= (1<<1);
    P0PU |= (1<<1);
#endif
}


/*----------------------------------------------------------------------------*/
/** @brief: IIC dat I/O 读取函数
    @param: void
    @return:读取数值
    @author:Juntham
    @note:  bool iic_data_r(void)
*/
/*----------------------------------------------------------------------------*/
_near_func bool iic_data_r(void) AT(COMMON_CODE)
{
#ifdef IIC_DAT_USE_P02
    return P02;
#else  
    return P01;
#endif
}

/*----------------------------------------------------------------------------*/
/** @brief: IIC dat I/O 输出值设置函数
    @param: flag：输出电平
    @return:void
    @author:Juntham
    @note:  void iic_data(bool flag)
*/
/*----------------------------------------------------------------------------*/
_near_func void iic_data(bool flag) AT(COMMON_CODE)
{
#ifdef IIC_DAT_USE_P02
    P02 = flag;
#else  
    P01 = flag;
#endif
}

/*----------------------------------------------------------------------------*/
/** @brief: IIC dat I/O 输出值设置函数
    @param: flag：输出电平
    @return:void
    @author:Juntham
    @note:  void iic_clk_out(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void iic_clk_out(void) AT(COMMON_CODE)
{
    P0DIR &= ~(1<<0);
}

/*----------------------------------------------------------------------------*/
/** @brief: IIC dat I/O 输出值设置函数
    @param: flag：输出电平
    @return:void
    @author:Juntham
    @note:  void iic_clk(bool flag)
*/
/*----------------------------------------------------------------------------*/
_near_func void iic_clk(bool flag) AT(COMMON_CODE)
{
    P00 = flag;
}



/*----------------------------------------------------------------------------*/
/** @brief: IIC 模块初始化函数
    @param: void
    @return:void
    @author:Juntham
    @note:  void iic_init(void)
*/
/*----------------------------------------------------------------------------*/
void iic_init(void) AT(IIC_CODE)
{
    set_iic_io((u16 __code *)iic_io_tab);       ///<配置接口函数IO
}

void eeprom_verify(void) AT(IIC_CODE)
{
    unsigned char i;

#ifdef CHECK_EEPROM_ON_POWER_ON
	if ((get_memory(0) != 0x55)
        ||(get_memory(1) != 0xAA))
	{
        //deg_puts("reset EEPROM\n");
        set_memory(0, 0x55);
        set_memory(1, 0xAA);
        for(i=2;i<MEM_CHANNL;i++)
          set_memory(i, 0);
    }

    if ((get_memory(0) != 0x55)
        ||(get_memory(1) != 0xAA))
	{
        //外接eeprom无效
        //deg_puts("EEPROM error\n");
    }
    else
    {
        //deg_puts("EEPROM OK\n");
        //有外接eeprom
        //my_puts("find eeprom\n");
    }
#endif

}
/*----------------------------------------------------------------------------*/
/** @brief: 记忆信息到存储器（EEPROM）
    @param: void
    @return:void
    @author:Juntham
    @note:  void set_memory(u8 addr, u8 dat)
*/
/*----------------------------------------------------------------------------*/
_near_func void set_memory(u8 addr, u8 dat) AT(COMMON_CODE)
{
#ifdef USE_EEPROM_MEMORY
    write_eerom(addr, dat);
#endif
#ifdef USE_IRTC_MEMORY
    write_IRTC_RAM(addr, dat);
#endif
}
/*----------------------------------------------------------------------------*/
/** @brief: 获取记忆信息（EEPROM）
    @param: void
    @return:void
    @author:Juntham
    @note:  u8 get_memory(u8 addr)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 get_memory(u8 addr) AT(COMMON_CODE)
{
#ifdef USE_EEPROM_MEMORY
    return read_eerom(addr);
#endif
#ifdef USE_IRTC_MEMORY
    return read_IRTC_RAM(addr);
#endif
}


