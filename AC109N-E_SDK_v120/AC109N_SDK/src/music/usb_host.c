/*--------------------------------------------------------------------------*/
/**@file    usb_host.c
   @brief   USB 接口函数
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/

#include "usb_host.h"
#include "device.h"
#include "USB_Device_Mode.h"

extern USB_DETECT _xdata * _data USB_Detect_ptr;


/*----------------------------------------------------------------------------*/
/**@brief   设置USB 为I/O 模式
   @param   void
   @return  void
   @note	void set_usb_io(void)
*/
/*----------------------------------------------------------------------------*/
__root void set_usb_io(void) @ "USB_HOST_CODE"
{
    USBCTL_IE(0);
    USBSOF_IE(0);
    P3DIR |= BIT(6)|BIT(7); //DP/DM
    P3PU &= ~(BIT(7)|BIT(6));
    P3PD |= BIT(7)|BIT(6);
    USB_CON0 = 0x11;
    USB_CON2 |= BIT(1);     //比较器enable

}
/*----------------------------------------------------------------------------*/
/**@brief   USB 在线检测
   @param   void
   @return  0x1:Host offline 0x2:Device offline 0x3:Host online 0x4:Device online
   @note	u8 USB_Online(void)
*/
/*----------------------------------------------------------------------------*/
__root u8 USB_Online_app(void) @ "USB_HOST_CODE"
{
    _no_init static u8 __idata online_cnt;
    _no_init static u8 __idata USB_last_io;
    _no_init static bool __bit pcin_double_check;
    _no_init static u8 __idata delay_counter;

    u8 USB_cur_io;

    USB_Detect_ptr = (USB_DETECT *)get_usb_host_detect();
    /**/
    delay_counter++;        //2ms per delay_counter
    if (delay_counter == 10)        //20ms
    {
        delay_counter = 0;
        if (USB_Detect_ptr->time_out < 0xff)
            USB_Detect_ptr->time_out++;
    }

    if (pcin_double_check)
    {
        pcin_double_check = 0;
        //my_printf("-USB DM : %02x\n", P37);
        if ((P37 == 0)&& (USB_CON2 & BIT(0)))
        {
            return 0x4;
        }
    }
    if (USB_Detect_ptr->online == 0x0)
    {
        set_usb_io();
    }
    USB_cur_io = (P3 & (BIT(6)|BIT(7)));
    USB_cur_io |= (USB_CON2 & BIT(0));

    if (USB_cur_io != USB_last_io)
    {
        online_cnt = 0;
        USB_last_io = USB_cur_io;
    }
    else
    {
        online_cnt++;
    }

    if (online_cnt < 40)
        return 0x0;

    online_cnt = 0;
    //my_printf("0-USB I/O : %02x\n", USB_cur_io);

    if ((USB_Detect_ptr->online & BIT(0))||(USB_Detect_ptr->online & BIT(1)))
    {
        if ((USB_Detect_ptr->online & BIT(0)) && (!(USB_last_io & BIT(6))))
        {
            return 0x1;//usb_host_status(0x0);
        }
        else if ((USB_Detect_ptr->online & BIT(1)) && (USB_last_io & BIT(7)))
        {
            return 0x2;//usb_device_status(0x0);
        }
    }
    else
    {
        if ((USB_last_io == 0x41))
        {
            return 0x3;//usb_host_status(0x1);
        }
        else if ((USB_last_io == 0x0))
        {
            /**/
            P3PD &= ~BIT(7);
            P3PU |= BIT(7);
#if 1
            P3PD &= ~BIT(6);
            //my_printf("1-USB I/O : %02x\n", USB_cur_io);
            if (!P36)
            {
                pcin_double_check = 1;//return 0x4;//usb_device_status(0x1);
            }
#endif
         }
    }
    return 0x0;
}
/*----------------------------------------------------------------------------*/
/**@brief   USB OTG 检测函数
   @param   void
   @return  void
   @author  Change.tsai
   @note	void USB_online_otp(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void USB_online_otp(void) AT(COMMON_CODE)
{
    switch (USB_Online_app())
    {
    case 0x1:
        //my_puts("usb_host_status-0\n");
        usb_host_status(0x0);
        break;
    case 0x2:
        //my_puts("usb_device_status-0\n");
        usb_device_status(0x0);
        break;
    case 0x3:
        //my_puts("usb_host_status-1\n");
        usb_host_status(0x1);
        break;
    case 0x4:
        //my_puts("usb_device_status-1\n");
        usb_device_status(0x1);
        break;
    default:
        break;
    }
}










