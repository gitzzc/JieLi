/*************************************************************/
/** @file:usb_host.h
    @brief:
    @details:
    @author:Juntham
    @date: 2012-07-25,8:59
    @note:
*/
/*************************************************************/

#ifndef _USB_HOST_H_
#define _USB_HOST_H_

#include "config.h"


#ifdef __IAR_SYSTEMS_ICC__

typedef struct _USB_DETECT
{
    u8 online;     		//<USB 在线标志	[0]:USB Host [1]:USB Slave
    u8 online_counter;  //<USB 在线计数器
    u8 last_data;       //<USB 控制器已检测的值
    u8 current_data;    //<USB 控制器当前检测的值
    volatile u8 time_out;		//<USB 操作超时计数器
}USB_DETECT;


_near_func void USB_online_otp(void);
bool write_udisk(void);
u8 usb_host_mount_app(u8 retry, u8 timeout, u8 reset_delay, u8 test_delay);
void force_usb_suspend(void);
__monitor void USB_Host_init(void);
u8 USB_Online_app(void);

/////
#ifdef UDISK_IDLE_EN
#define udisk_force_idle() usb_suspend()
#else
#define udisk_force_idle()
#endif
//////



/*******************************************/
/*
            Maskrom API
                --USB Host Part
*/
/*******************************************/
/*----------------------------------------------------------------------------*/
/**@brief   USB 在线检测
   @param   void
   @return  0x1:Host offline 0x2:Device offline 0x3:Host online 0x4:Device online
   @author  Change.tsai
   @note	u8 USB_Online(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 USB_Online(void);
/*----------------------------------------------------------------------------*/
/**@brief   设置USB Host在线状态
   @param   void
   @return  void
   @author  Change.tsai
   @note	void usb_host_status(u8 status)
*/
/*----------------------------------------------------------------------------*/
_near_func void usb_host_status(u8 status);
/*----------------------------------------------------------------------------*/
/**@brief   USB 控制器进入挂起模式
   @param   void
   @return  void
   @author  Change.tsai
   @note	void usb_suspend(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void usb_suspend(void);
/*----------------------------------------------------------------------------*/
/**@brief   USB Host 挂载
   @param   void
   @return  0：挂载失败；1：挂载成功
   @author  Change.tsai
   @note	u8 usb_host_mount(u8 retry, u8 timeout, u8 init_delay, u8 reset_delay, u8 test_delay)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 usb_host_mount(u8 retry, u8 timeout, u8 reset_delay, u8 test_delay);
/*----------------------------------------------------------------------------*/
/**@brief   USB 控制器恢复普通I/O模式
   @param   void
   @return  卸载成功
   @author  Change.tsai
   @note	u8 usb_host_unmount(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 usb_host_unmount(void);
/*----------------------------------------------------------------------------*/
/**@brief   USB Host 读接口
   @param   buf：数据缓冲区；lba：物理地址
   @return  0：成功；other：失败
   @author  Change.tsai
   @note	u8 usb_host_read(u8 _xdata *buf, u32 lba)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 usb_host_read(u8 _xdata *buf, u32 lba);
/*----------------------------------------------------------------------------*/
/**@brief   USB Host 写接口
   @param   buf：数据缓冲区；lba：物理地址
   @return  0：成功；other：失败
   @author  Change.tsai
   @note	u8 usb_host_write(u8 _xdata *buf, u32 lba)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 usb_host_write(u8 _xdata *buf, u32 lba);
/*----------------------------------------------------------------------------*/
/**@brief   USB 超时控制器
   @param   void
   @return  0：超时；other：设备在线状态
   @author  Change.tsai
   @note	bool USB_Host_timeout(void)
*/
/*----------------------------------------------------------------------------*/
_near_func _bool USB_Host_timeout();
/*----------------------------------------------------------------------------*/
/**@brief   USB 读取剩余数据接口函数
   @param   void
   @return  传输成功标志
   @author  Change.tsai
   @note	_bool USB_read_remain(void)
*/
/*----------------------------------------------------------------------------*/
_near_func _bool USB_read_remain(void);


/*----------------------------------------------------------------------------*/
/**@brief   获取USB 设备sector大小
   @param   void
   @return  0：超时；other：设备在线状态
   @note	u8 get_usb_pkt_size(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 get_usb_pkt_size(void);
/*----------------------------------------------------------------------------*/
/**@brief   获取USB 在线状态
   @param   void
   @return  void
   @author  Change.tsai
   @note	u8 get_usb_online_status(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 get_usb_online_status(void);
/*----------------------------------------------------------------------------*/
/**@brief   读取USB 寄存器
   @param   addr：寄存器地址
   @return  寄存器数值
   @author  Change.tsai
   @note	u8 read_USB_reg(u8 addr)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 read_USB_reg(u8 addr);
/*----------------------------------------------------------------------------*/
/**@brief   写入USB 寄存器
   @param   addr：写入地址
   @return  value：写入数据
   @author  Change.tsai
   @note	void write_USB_reg(u8 addr , u8 value)
*/
/*----------------------------------------------------------------------------*/
_near_func void write_USB_reg(u8 addr , u8 value);
/*----------------------------------------------------------------------------*/
/**@brief   USB 控制传输事务处理函数
   @param   void
   @return  传输成功标志
   @author  Change.tsai
   @note	_bool USB_ctlXfer(u8 _xdata *buf)
*/
/*----------------------------------------------------------------------------*/
_near_func _bool USB_ctlXfer(u8 _xdata *buf);
/*----------------------------------------------------------------------------*/
/**@brief   USB 控制传输处理函数
   @param   void
   @return  传输成功标志
   @author  Change.tsai
   @note	_bool USB_Control_transfers(void)
*/
/*----------------------------------------------------------------------------*/
_near_func _bool USB_Control_transfers(void);
/*----------------------------------------------------------------------------*/
/**@brief   USB Host Device枚举
   @param   void
   @return  设备枚举成功标志
   @author  Change.tsai
   @note	_bool USB_Host_dev_enum(u8 _xdata *buf, u16 delay_max)
*/
/*----------------------------------------------------------------------------*/
_near_func _bool USB_Host_dev_enum(u8 _xdata *buf, u8 delay_max);
/*----------------------------------------------------------------------------*/
/**@brief   USB Host Mass Storage枚举
   @param   void
   @return  Mass Storage枚举成功标志
   @author  Change.tsai
   @note	_bool USB_Host_Mass_Storage_enum(u8 _xdata *buf, u8 delay)
*/
/*----------------------------------------------------------------------------*/
_near_func _bool USB_Host_Mass_Storage_enum(u8 _xdata *buf, u8 delay);
/*----------------------------------------------------------------------------*/
/**@brief   USB Finite State Machine
   @param   buf：接收数据缓冲区（64Bytes）
   @return  0：MSD 枚举成功，1：状态机正常；2：传输错误枚举失败 3：端点0枚举没有发现MSD 端点 4：MSD 设备掉线
   @author  Change.tsai
   @note	u8 USB_FSM(u8 __xdata *buf)
*/
/*----------------------------------------------------------------------------*/
_near_func u8 USB_FSM(u8 _xdata *buf);
/*----------------------------------------------------------------------------*/
/**@brief   UFI Command Block-Inquiry
   @param   void
   @return  传输成功标志
   @author  Change.tsai
   @note	_bool USB_UFI_Inquiry(void)
*/
/*----------------------------------------------------------------------------*/
_near_func _bool USB_UFI_Inquiry(void);
/*----------------------------------------------------------------------------*/
/**@brief   UFI Command Block-Test_unit_ready
   @param   void
   @return  传输成功标志
   @author  Change.tsai
   @note	_bool USB_UFI_Test_unit_ready(void)
*/
/*----------------------------------------------------------------------------*/
_near_func _bool USB_UFI_Test_unit_ready(void);
/*----------------------------------------------------------------------------*/
/**@brief   UFI Command Block-Request_sense
   @param   void
   @return  传输成功标志
   @author  Change.tsai
   @note	_bool USB_UFI_Request_sense(void)
*/
/*----------------------------------------------------------------------------*/
_near_func _bool USB_UFI_Request_sense(void);
/*----------------------------------------------------------------------------*/
/**@brief   UFI Command Block-Request_sense
   @param   void
   @return  传输成功标志
   @author  Change.tsai
   @note	_bool USB_UFI_Request_sense(void)
*/
/*----------------------------------------------------------------------------*/
_near_func _bool USB_UFI_Ready_capacity(void);
/*----------------------------------------------------------------------------*/
/**@brief   获取USB Host 变量
   @param   void
   @return  变量地址
   @author  Change.tsai
   @note	u16 get_usb_host_var(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u16 get_usb_host_var(void);
/*----------------------------------------------------------------------------*/
/**@brief   获取USB Host 变量
   @param   void
   @return  变量地址
   @author  Change.tsai
   @note	u16 get_usb_host_var(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u16 get_usb_host_detect(void);
/*----------------------------------------------------------------------------*/
/**@brief   获取USB 控制传输命令变量
   @param   void
   @return  变量地址
   @author  Change.tsai
   @note	u16 get_usb_control_var(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u16 get_usb_host_control_var(void);
/*----------------------------------------------------------------------------*/
/**@brief   获取USB 控制传输过程变量
   @param   void
   @return  变量地址
   @author  Change.tsai
   @note	u16 get_usb_ctlXfer_var(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u16 get_usb_host_ctlXfer_var(void);
/*----------------------------------------------------------------------------*/
/**@brief   获取USB Bulk-Only传输过程变量
   @param   void
   @return  变量地址
   @author  Change.tsai
   @note	u16 get_usb_bulkXfer_var(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u16 get_usb_host_bulkXfer_var(void);
/*----------------------------------------------------------------------------*/
/**@brief   获取USB Bulk-Only传输命令变量
   @param   void
   @return  变量地址
   @author  Change.tsai
   @note	u16 get_usb_bulk_var(void)
*/
/*----------------------------------------------------------------------------*/
_near_func u16 get_usb_host_bulk_var(void);

void get_usb_host_maskrom_ptr(void);
_bool USB_UFI_specify_command(u8 cmd);

_bool judge_usb_host_virkey(void);

#endif


#endif
