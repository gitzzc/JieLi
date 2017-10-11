#ifndef _USB_SLAVE_MODE_H_
#define _USB_SLAVE_MODE_H_


#ifdef __IAR_SYSTEMS_ICC__

void USB_device_mode(void);

extern u8 _data PC_volr,_data PC_voll;
/**************************************************************************/
/*
               CLASS  BITMAP
    7   |   6   |   5   |   4   |   3   |   2   |   1   |   0
                                            HID    Audio   Mass Storage
*/
/**************************************************************************/

#define MASSSTORAGE_CLASS   BIT(0)
#define AUDIO_CLASS         BIT(1)
#define HID_CLASS           BIT(2)

#define CLASS_CONFIG        (MASSSTORAGE_CLASS|AUDIO_CLASS|HID_CLASS)




/*******************************************/
/*
            Maskrom API
                --USB Slave Part
*/
/*******************************************/
/*----------------------------------------------------------------------------*/
/**@brief   设置USB 从机在线状态
   @param   void
   @return  void
   @author  Change.tsai
   @note	void usb_device_status(u8 status)
*/
/*----------------------------------------------------------------------------*/
_near_func void usb_device_status(u8 status);
/*----------------------------------------------------------------------------*/
/**@brief   USB Slave 挂载，控制器初始化
   @param   无
   @return  无
   @note    u8 usb_device_mount(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 usb_device_mount(void);
/*----------------------------------------------------------------------------*/
/**@brief   USB 控制器恢复普通I/O模式
   @param   void
   @return  卸载成功
   @note	u8 usb_device_unmount(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 usb_device_unmount(void);
/*----------------------------------------------------------------------------*/
/**@brief   USB Class 设置函数
   @param   void
   @return  void
   @note	void USB_class_setting(u8 config)
*/
/*----------------------------------------------------------------------------*/
_near_func void USB_class_setting(u8 config);
/*----------------------------------------------------------------------------*/
/**@brief   HID 按键发送函数
   @param   void
   @return  void
   @note	void usb_hid_key(u16 key)
*/
/*----------------------------------------------------------------------------*/
_near_func void usb_hid_key(u16 key);
/*----------------------------------------------------------------------------*/
/**@brief   USB UFI/SCSI Class Bulk 传输处理函数
   @param   void
   @return  void
   @note    void USB_MassStorage(void)
*/
/*----------------------------------------------------------------------------*/
_near_func bool USB_MassStorage(void);
/*----------------------------------------------------------------------------*/
/**@brief   设置USB Audio 音量
   @param   chl：左声道
   @param   chr：右声道
   @return  void
   @note    void set_Audio_volume(u8 chl, u8 chr)
*/
/*----------------------------------------------------------------------------*/
_near_func void set_Audio_volume(u8 chl, u8 chr);
/*----------------------------------------------------------------------------*/
/**@brief   获取USB Audio 音量
   @param   chl：左声道
   @param   chr：右声道
   @return  左/右声道值
   @note    u8 get_Audio_volume(u8 ch)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 get_Audio_volume(u8 ch);
_near_func void set_hook_usb_scsi(u8 (_near_func * fun)(u8));
/*----------------------------------------------------------------------------*/
/**@brief   USB 从机模式 端点初始化
   @param   无
   @return  无
   @note    void USB_Slave_ep0_init(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void USB_Slave_ep0_init(void);
/*----------------------------------------------------------------------------*/
/**@brief   USB 从机模式 寄存器器初始化
   @param   无
   @return  无
   @note    void USB_Slave_reg_init(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void USB_Slave_reg_init(void);
/*----------------------------------------------------------------------------*/
/**@brief   USB 端点0 控制传输处理函数
   @param   无
   @return  无
   @note    void Ep0_control_transfer(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void Ep0_control_transfer(void);
/*----------------------------------------------------------------------------*/
/**@brief   USB ISO 传输处理函数
   @param   void
   @return  无
   @note    void USB_iso_transfer(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void USB_iso_transfer(void);
/*----------------------------------------------------------------------------*/
/**@brief   获取USB 端点繁忙状态
   @param   void
   @return  繁忙状态
   @note    u16 get_usb_ep_busy(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u16 get_usb_ep_busy(void);
//New 12/7/8
_near_func _bool MCU_SRAMToUSB(u8 _xdata *pBuf, u16 uCount);
_near_func _bool MCU_USBToSRAM(u8 _xdata *pBuf, u16 uCount);
_near_func void STALL_INEP(u8 bEp);
_near_func void STALL_OUTEP(char bEp);
_near_func void Unknow(void);
_near_func bool RBC_CheckSDStatus(void);
_near_func void SendCSW(void);
_near_func bool GetCBW(void);
_near_func u16 get_usb_slave_var(void);
_near_func u16 get_usb_slave_bulkinfo_var(void);
_near_func u16 get_usb_slave_bulk_var(void);
_near_func u16 get_usb_slave_bulkTx_var(void);
_near_func u16 get_usb_slave_bulkRx_var(void);
_near_func void set_hook_sdmmc_read_usb(u8 (*fun)( u16 lba_num,u32 lba,u8 sd_id));
_near_func void set_hook_sdmmc_write_usb(u8 (*fun)( u16 lba_num,u32 lba,u8 sd_id));
_near_func void set_hook_set_descriptor(void (* fun)(void));
_near_func void set_hook_get_sd_capacity(u32 (* fun)(u8 sd_id));
_near_func void set_descriptor_buf(u8 *pbuf);
_near_func void set_hook_set_HID_report(void (* fun)(void));
_near_func void set_HID_report_buf(u8 *pbuf);
_near_func void set_USB_MSD_number(u8 lun);
_near_func void set_hook_MCU_SRAMToUSB(_bool (* fun)(u8 _xdata *, u16));
_near_func void set_hook_MCU_USBToSRAM(_bool (* fun)(u8 _xdata *, u16));
_near_func void set_hook_GetCBW(_bool (* fun)(void));
_near_func void set_hook_hook_SendCSW(void (* fun)(void));
_near_func void set_USB_string_set(u16 _xdata *pbuf);


#endif  /*  __IAR_SYSTEMS_ICC__ */

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


//----------------------------------
// MUSB
//----------------------------------
#define     FADDR_REG               0x00
#define     POWER_REG               0x01
#define     INTRTX1_REG             0x02
#define     INTRTX2_REG             0x03
#define     INTRRX1_REG             0x04
#define     INTRRX2_REG             0x05
#define     USBINTR_REG             0x06
#define     INTRTX1E_REG            0x07
#define     INTRTX2E_REG            0x08
#define     INTRRX1E_REG            0x09
#define     INTRRX2E_REG            0x0A
#define     USBINTEN_REG            0x0B
#define     FRAMENUM1_REG           0x0C
#define     FRAMENUM2_REG           0x0D
#define     INDEX_REG               0x0E
#define     DEVCTL_REG              0x0F
#endif  /*  _USB_SLAVE_MODE_H_   */
