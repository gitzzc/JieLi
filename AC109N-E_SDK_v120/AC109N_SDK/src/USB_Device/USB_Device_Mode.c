/*--------------------------------------------------------------------------*/
/**@file    USB_Device_Mode.c
   @brief   USB 从机模式主循环
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/
#include "config.h"
#ifdef USB_DEVICE_EN
#include "USB_Device_Mode.h"
#include "msg.h"
#include "hot_msg.h"
#include "key.h"
#include "main.h"
#include "device.h"
#include "iic.h"
#include "DAC.h"
#include "Descriptor.h"
#include "sdmmc_api.h"
#include "UI_API.h"
#include "clock.h"
#include "usb_host.h"
#include "RTC_API.h"

_no_init u8 _data temp_volr,_data temp_voll;
_no_init u8 _data PC_volr,_data PC_voll;


/*----------------------------------------------------------------------------*/
/**@brief   USB 端点中断服务程序
   @param   void
   @return  void
   @note    void USB_isr(void)
*/
/*----------------------------------------------------------------------------*/
__root
_near_func void USB_isr(void) AT(COMMON_CODE)
{
    u8 index, reg;
//
    index = read_USB_reg(INDEX_REG);

    write_USB_reg(INDEX_REG, 0);

//Reset Signal
    reg = read_USB_reg(USBINTR_REG);    //Clear Pending

    if (reg & BIT(1))					//[1]:Resume
    {
        //USB_Slave_ctl.bDevConfig = 0;
    }
    if (reg & BIT(2))   				//[2]:Reset
    {
        USB_Slave_ep0_init();
        USB_Slave_reg_init();

    }
//Ep0 Interrupt
    reg = read_USB_reg(INTRTX1_REG);

    if (reg & BIT(0))					//Control Transfer
    {
        Ep0_control_transfer();
    }

//Ep2 Interrupt
    reg = read_USB_reg(INTRRX1_REG);	//ISO Transfer
    if (reg & BIT(2))   //RxPktRdy
    {
        USB_iso_transfer();
    }
    //printf("Slave Init USB_CON0: %02bx\n", USB_CON0);
    write_USB_reg(INDEX_REG, index);
}
/*----------------------------------------------------------------------------*/
/**@brief   usb hid 命令接口函数
   @param   key ：  hid 命令
   @return  无
   @note    void usb_hid_control(u16 key)
*/
/*----------------------------------------------------------------------------*/
void usb_hid_control(u16 key) AT(USB_DEVICE_CODE)
{
    usb_hid_key(key);
    usb_hid_key(0);
}

/*----------------------------------------------------------------------------*/
/**@brief   设置USB Audio 音量
   @param   无
   @return  无
   @note    void set_Audio_vol(void)
*/
/*----------------------------------------------------------------------------*/
void set_Audio_vol(void) AT(USB_DEVICE_CODE)
{
    if (mute)
      return;
    main_vol_L = get_Audio_volume(0x0);
    if (main_vol_L)
    {
        main_vol_L >>= 3;
        main_vol_L = (main_vol_L == 0) ? 1 : main_vol_L;
        main_vol_L = (main_vol_L > 31) ? 31 : main_vol_L;
    }

    main_vol_R = get_Audio_volume(0x1);
    if (main_vol_R)
    {
        main_vol_R >>= 3;
        main_vol_R = (main_vol_R == 0) ? 1 : main_vol_R;
        main_vol_R = (main_vol_R > 31) ? 31 : main_vol_R;
    }
    set_main_vol(main_vol_L, main_vol_R);
    set_memory(MEM_VOL_PCL, main_vol_L);
    set_memory(MEM_VOL_PCR, main_vol_R);
}


/*----------------------------------------------------------------------------*/
/**@brief   USB 从机模式主循环
   @param   无
   @return  无
   @note    void set_Audio_vol(void)
*/
/*----------------------------------------------------------------------------*/
void USB_device_mode_loop(void) AT(USB_DEVICE_CODE)
{
    u8 msg;

    while (1)
    {
        msg = app_get_msg();

#if (CLASS_CONFIG & MASSSTORAGE_CLASS)
        USB_MassStorage();
#endif
        switch (msg)
        {
        case MSG_CHANGE_WORK_MODE:
            return;

        case MSG_USB_PC_OUT:
            LED_FADE_ON();
            work_mode = MUSIC_MODE;
            usb_device_unmount();
            return;

#if (CLASS_CONFIG & HID_CLASS)
        case MSG_MUSIC_PREV_FILE:
            usb_hid_control(USB_AUDIO_PREFILE);
            break;

        case MSG_MUSIC_NEXT_FILE:
            usb_hid_control(USB_AUDIO_NEXTFILE);
            break;

        case MSG_MUSIC_PP:
            usb_hid_control(USB_AUDIO_PP);
            break;

        case MSG_VOL_DOWN:
            usb_hid_control(USB_AUDIO_VOLDOWN);
            UI_menu(MENU_PC_VOL_DOWN);
            break;

        case MSG_VOL_UP:
            usb_hid_control(USB_AUDIO_VOLUP);
            UI_menu(MENU_PC_VOL_UP);
            break;

        case MSG_MUTE:
            usb_hid_control(USB_AUDIO_MUTE);
            break;
#else
        case MSG_VOL_DOWN:
            break;

        case MSG_VOL_UP:
            break;

        case MSG_MUTE:
            break;
#endif

#if (CLASS_CONFIG & AUDIO_CLASS)
        case MSG_PC_MUTE:
            dac_mute(1);
            break;

        case MSG_PC_UNMUTE:
            dac_mute(0);
            put_msg_fifo(MSG_PC_AUDIO_VOL);
            break;

        case MSG_PC_AUDIO_VOL:
            set_Audio_vol();
            break;
#endif
        case MSG_HALF_SECOND:
            LED_FADE_OFF();
            UI_menu(MENU_MAIN);
            UI_menu(MENU_HALF_SEC_REFRESH);
            break;

        default:
            ap_handle_hotkey(msg);
            break;
        }
    }

}
/*----------------------------------------------------------------------------*/
/** @brief:
    @param:
    @return:
    @author:Juntham
    @note:
*/
/*----------------------------------------------------------------------------*/
void usb_device_info_init(void) AT(USB_DEVICE_CODE)
{
    /*------------System configuration-----------*/
    Sys_IRInput = 0;
    Sys_Volume = 1;

    int_enter_pro[USBCTL_INT] = USB_isr;
    USB_class_setting(CLASS_CONFIG);
    set_hook_usb_scsi(dummy_func8);
#ifdef SDMMCB_EN
    set_USB_MSD_number(1);
#else
    set_USB_MSD_number(0);
#endif

    /*Recovery Main vol*/
    temp_volr = main_vol_R;
    temp_voll = main_vol_L;

    /*PC 独立音量*/
    main_vol_L = get_memory(MEM_VOL_PCL);
    main_vol_R = get_memory(MEM_VOL_PCR);
    main_vol_L = (main_vol_L >= 31) ? 31 : main_vol_L;
    main_vol_R = (main_vol_R >= 31) ? 31 : main_vol_R;
    set_Audio_volume(main_vol_L, main_vol_R);
    SET_UI_MAIN(MENU_PC_MAIN);
    UI_menu(MENU_PC_MAIN);

    /*------Key Table*/
    key_table_sel(0);
}

/*----------------------------------------------------------------------------*/
/** @brief:
    @param:
    @return:
    @author:Juntham
    @note:
*/
/*----------------------------------------------------------------------------*/
void USB_device_mode(void) AT(USB_DEVICE_CODE)
{
    if ((get_usb_online_status() & BIT(1)) == 0x0)
    {
        work_mode++;
        return;
    }

#ifdef SDMMCA_EN
        music_device_info[DEVICE_SDMMCA].wBreak_point_filenum = 0;
        music_device_info[DEVICE_SDMMCA].wfileTotal = 0;
#endif
#ifdef SDMMCB_EN
        music_device_info[DEVICE_SDMMCB].wBreak_point_filenum = 0;
        music_device_info[DEVICE_SDMMCB].wfileTotal = 0;
#endif

#ifdef SDMMCA_DET_CLK
    set_sd_clk = 1;
#endif
    /*------System Clock*/
    system_clk_div(CLK_48M);

    usb_device_info_init();
    usb_device_mount();
    dac_channel_sel(DAC_DECODER);
#ifdef CLK_USE_HTC
    if (!check_htc_sof(3))
    {
#ifdef SDMMCA_DET_CLK
    set_sd_clk = 0;
#endif
        work_mode = MUSIC_MODE;
        main_vol_R = temp_volr;
        main_vol_L = temp_voll;
        usb_device_unmount();
        return;
    }
#endif
    USB_device_mode_loop();

    P3PU &= ~BIT(6);
    sdmmca_force_idle();
    sdmmcb_force_idle();

    /*Recovery Main vol*/
    main_vol_R = temp_volr;
    main_vol_L = temp_voll;

#ifdef SDMMCA_DET_CLK
    set_sd_clk = 0;
#endif
}
#endif
