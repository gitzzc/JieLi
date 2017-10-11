/*******************************************************************************************
File Name:  Log.c

Version:    1.00

Discription: APP开发日志信息

Author:     Caibingquan

Email:      change.tsai@gmail.com

Date:       2012.07.24

Copyright:(c) 2012 , All Rights Reserved.
*******************************************************************************************/
////*更新时间：2012/08/23*/
//1.工程部分代码编译成Lib，DAC 部分整合Jutham
//2.USB RST 3ms 后IDLE
//3.RTC 时间溢出校正
//4.RTC 闹钟打断FM 搜索
//5.USB 驱动移至LIB


////*更新时间：2012/08/22*/
//已完成
//1.RTC/AUX/FM 段整理，Page切换实现
//遗漏细节
//1.RTC 闹钟打断FM 搜索
//2.RTC 时间溢出校正
//3.USB RST 3ms 后IDLE


////*更新时间：2012/08/20*/
//已完成
//1.DAC,PLL 功能完成
//2.ADC 改为16通道中断方式采集

////*更新时间：2012/08/17*/
//已完成
//1.LCD 并口驱动添加完成

//注意
//1.LCD 段码屏Port SFR 需要添加影子SFR 不能直接读

//Remain work for analog part
//1.ADC Driver
//2.AD IR I/O 复用
//3.DAC Driver

////*更新时间：2012/08/16*/
//1.get_u16_random 由于Optimize Level-High 导致作inline 优化

////*更新时间：2012/08/13*/
//已完成
//1.SD IIC I/O 复用实现可行，已验证EEPROM 与 FM 模块工作正常

//遗漏问题
//1.查找get_u16_random 函数在.map无法定位原因

////*更新时间：2012/8/08*/
//已完成
//1.USB 从机重复测试，系统需要运行48M

////*更新时间：2012/8/07*/
//已完成
//1.FM LCD/LED 界面完善，FPGA RDA5807MP 收音控制流程

//注意
//1.按键音Table 需要word 对齐

////*更新时间：2012/8/06*/
//已完成
//1.RTC 操作界面完成，工作日闹钟完成

////*更新时间：2012/8/01*/
//已完成
//1.LCD 驱动/界面功能完善

////*更新时间：2012/7/31*/
//已完成
//1.FM 模式功能/界面完善
//2.RTC 增加周闹钟功能

////*更新时间：2012/7/27*/
///已完成
//1.完成LED 驱动，界面功能属性-主界面返回/红外输入选中，完成，UI 独立分开LCD点阵/LED& LCD SEG
//2.界面属性-渐亮/渐滅

//存在问题
//1.USB 兼容性，4/16号设备 MSD 枚举失败


////*更新时间：2012/7/24*/
///已完成
//1.添加AR1019 驱动，整合都FM 标准接口
//2.宏开关整理，FM/PC/RTC 模式开关


////*更新时间：2012/7/23*/
///已完成
//1.添加RDA5807SP/HP/MP，BK1080，KT0830，QN8035驱动，编译通过
//2.FM 标准API
//bool init_fm_rev(void)
//bool set_fre(u8 mode)
//void fm_rev_powerdown(void)
//void fm_module_mute(u8 flag)
//void fm_set_ch(u8 db)

///未完成
//1.QN8075，AR1010/1019驱动 
