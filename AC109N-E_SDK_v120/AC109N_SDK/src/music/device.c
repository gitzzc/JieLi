/*--------------------------------------------------------------------------*/
/**@file    device.c
   @brief   �豸�����ܽӿ�
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/
#include "device.h"
#include "play_file.h"
#include "get_music_file.h"
#include "music_play.h"
#include "sdmmc_api.h"
#include "break_point.h"
#include "clock.h"
#include "usb_host.h"
#include "iic.h"

_no_init u8 _data device_online;			    ///<�豸���߱��
_no_init u8 _data device_active;                ///<��ǰ���߻�豸
_no_init bool _bit find_break_point_file_flag;
#ifdef USB_VIR_KEY_EN
_no_init bool _bit have_vir_key;
#endif

#ifndef NDEBUG
u8 const _code file_type[] = "WAV";  ///<�����ļ���ʽ
#else
u8 const _code file_type[] = "WAVMP?";  ///<�����ļ���ʽ
#endif

u8 (_near_func *const disk_read_table[MAX_DEVICE + 1])(u8 _xdata * buf, u32 lba) =
{
#ifdef USB_DISK_EN
    usb_host_read,
#endif

#ifdef SDMMCA_EN
    sdmmcA_read,
#endif

#ifdef SDMMCB_EN
    sdmmcB_read,
#endif
};

#ifdef USB_VIR_KEY_EN
_bool send_vir_key(u8 key)AT(GET_DEVICE)
{
    if(have_vir_key)
    {
        USB_UFI_specify_command(key);
        return TRUE;
    }
    return FALSE;
}
#endif

/*----------------------------------------------------------------------------*/
/**@brief   ��⵱ǰ����豸
   @param   void
   @return  0����ǰ����豸�����ߣ���������ǰ����豸
   @author: Juntham
   @note    u8 device_check(void)
*/
/*----------------------------------------------------------------------------*/
u8 device_check(void) AT(GET_DEVICE)
{
#ifdef SDMMCA_EN
    if (get_sdmmc_online_status(SDMMCA_ID) == 0)
    {
        device_online &= ~BIT(DEVICE_SDMMCA);
    }
    else
        device_online |= BIT(DEVICE_SDMMCA);
#endif

#ifdef SDMMCB_EN
    if (get_sdmmc_online_status(SDMMCB_ID) == 0)
    {
        device_online &= ~BIT(DEVICE_SDMMCB);
    }
    else
        device_online |= BIT(DEVICE_SDMMCB);
#endif

#ifdef USB_DISK_EN
    if (get_usb_online_status() & BIT(0))   //Host online
        device_online |= BIT(DEVICE_UDISK);
    else
        device_online &= ~BIT(DEVICE_UDISK);
#endif

    return (BIT(device_active)) & device_online;
}


/*----------------------------------------------------------------------------*/
/**@brief   �ļ�ɨ��
   @param   void
   @return  0��ɨ��ɹ���������ʧ��
   @author: Juntham
   @note    u8 fs_get_total_files(void)
*/
/*----------------------------------------------------------------------------*/
u8 fs_get_total_files(void) AT(GET_DEVICE)
{
    /*Maskrom File by number*/
    //ff_api.fs_msg.fileNumber = 0x0101;
    //fs_getfile_bynumber(&ff_api,ff_api.fs_msg.fileNumber);

    my_memset((u8 _xdata *)&ff_api, 0, sizeof(FF_APIS));

    find_break_point_file_flag = 0;

#ifdef LAST_MEM_FILE_PLAY_EN
    load_music_break_point(device_active);
#endif

    if (music_device_info[device_active].wfileTotal == 0)
    {
#ifdef LAST_MEM_FILE_PLAY_EN
        ff_api.fs_msg.sclust_id = music_break_info.wSclustID;
        ff_api.fs_msg.fsize_id = music_break_info.wFsizeID;
#endif
        //system_clk(48/48);      //Ϊ�˼ӿ������ٶȣ�CPUȫ������
        if (fs_scan_disk(&ff_api))
        {
            find_break_point_file_flag = 1;
        }

        ff_api.fs_msg.dirCounter = 0;

        ff_api.fs_msg.fileTotalOutDir = 0;
        if (ff_api.fs_msg.fileCounter == 0)
        {
            return 0xfe;
        }
        else
        {
            music_device_info[device_active].wfileTotal = ff_api.fs_msg.fileCounter;
        }
    }

    return 0;
}

/*----------------------------------------------------------------------------*/
/**@brief   �豸��ʼ��
   @param   ��
   @return  0���ҵ���Ч�豸��������û���ҵ���Ч�豸
   @author: Juntham
   @note    u8 device_init(void)
*/
/*----------------------------------------------------------------------------*/
u8 device_init(u8 dev) AT(GET_DEVICE)
{
    //if (device_check())  ///<��ǰ��Ҫ����豸����
  {
        u8 pkt_size = 1;

        fs_ext_setting(file_type);

        if (dev == DEVICE_UDISK)
        {
            u8 retry, res;
#ifdef USB_DISK_EN
	#ifdef USB_VIR_KEY_EN
            get_usb_host_maskrom_ptr();
	#endif
            delay_n10ms(10);
            /*retry-3/time_out-254/reset_delay-40*/
            for(retry = 0; retry < 3; retry++)
            {
                //my_puts("retry\n");
                //USB_CON0 = 0x0;
                res = usb_host_mount(1, 254, 4, 100);
                if (!res)
                  break;
            }
            if (res)     ///<USB MASS STORAGE ö��
            {
                //my_printf("0-usb fail : %02x\n", res);
                force_usb_suspend();
                return 0xff;             ///<ö��ʧ��
            }
            //my_printf("1-usb ok : %02x\n", res);
            pkt_size = get_usb_pkt_size();
#else
            return 0xff;
#endif
        }
        else
        {
            //u8 retry, res;
            //my_puts("SD init...");
#if (defined (SDMMCA_EN) || defined (SDMMCB_EN))
            for(retry = 0; retry < 3; retry++)
            {
                //my_puts("retry ");
                res = identif_sdmmc(dev);
                if ((!res)||(get_sdmmc_online_status(SDMMCA_ID) == 0))                  ///<sd ������
                    break;
            }
            if (res)
            {
                return 0xff;                       ///<����ʧ��
            }
#else
            return 0xff;
#endif

            //my_puts("sd ok\n");
            //SD_CON1 |= BIT(6);
        }

        f_init(disk_read_table[dev], pkt_size);

        if (!f_mount(0))                ///<�ļ�ϵͳ��ʼ��
        {
	#ifdef USB_VIR_KEY_EN
            have_vir_key = 0;
            if (dev == DEVICE_UDISK)
            {
                have_vir_key = judge_usb_host_virkey();
            }
	#endif
            //my_puts("FAT ok\n");
            return 0;
        }
        else
        {
            //my_puts("FAT fail\n");
            return 0xff;
        }
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   ����ָ�����豸���Զ�������һ����Ч�豸
   @param   select : 1������SD�豸��2������USB mass storage�豸; 0x81:������һ���豸��0x82:������һ���豸;��������Ч
   @return  0���ɹ����ҵ�һ���豸��1:ָ�����豸û���ҵ���2��ֻ��һ���豸��������һ���豸����һ���豸��Ч;3:�ҵ��豸������ʼ�����ɹ�;4���������豸
   @author: Juntham
   @note    u8 find_device(u8 select)
*/
/*----------------------------------------------------------------------------*/
u8 find_device(ENUM_DEVICE_SELECT_MODE select) AT(GET_DEVICE)
{
    u8 i;

    /*Maskrom File by number*/
    //fs_getfile_bynumber(&ff_api,ff_api.fs_msg.fileNumber);

    device_check();

    i = get_one_count(device_online & 0x07);

    if (i == 0)				//���豸
    {
        device_active = NO_DEVICE;
        return NO_DEV_ONLINE;
    }

    if (select == DEVICE_AUTO_PREV)		//������һ���豸
    {
        for (i = 0;i <= MAX_DEVICE;i++)
        {
            device_active--;

            if (device_active > MAX_DEVICE)
            {
            #ifdef SDMMCB_EN
                device_active = DEVICE_SDMMCB;
            #elif defined (SDMMCA_EN)
                device_active = DEVICE_SDMMCA;
            #else
                device_active = DEVICE_UDISK;
            #endif
            }
            if (device_check() == 0)
                continue;

            if (device_init(device_active))             //�ҵ���Ч�豸
                continue;

            if (!fs_get_total_files())
                return FIND_DEV_OK;
        }
        device_active = NO_DEVICE;
        return NO_EFFECTIVE_DEV;				//����Ч����ʹ�õ��豸
    }
    else if (select == DEVICE_AUTO_NEXT)    //������һ���豸
    {
        for (i = 0;i <= MAX_DEVICE;i++)
        {
            device_active++;
            if (device_active > MAX_DEVICE)
                device_active = DEVICE_UDISK;

            if (device_check() == 0)
                continue;

            if (device_init(device_active))             //�ҵ���Ч�豸
                continue;

            if (!fs_get_total_files())
                return FIND_DEV_OK;
        }

        device_active = NO_DEVICE;
        return NO_EFFECTIVE_DEV;							//����Ч����ʹ�õ��豸
    }
    else                //ѡ��ָ�����豸
    {
        device_active = select;
        if (BIT(select) & device_online)
        {
            if (!device_init(device_active))
            {
                if (!fs_get_total_files())
                {
                    return FIND_DEV_OK;
                }
            }
            {
                return DEV_INIT_ERR;
            }
        }
        else
            return NO_DEFINE_DEV;
    }
}
