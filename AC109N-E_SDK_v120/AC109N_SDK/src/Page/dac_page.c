/*! \page8 DAC 编程指南
 *
 * \section dac0 DAC 介绍
 * - CD002 有9 种采样率\n
 * \section dac1 数字部分控制寄存器
 * - DAC_CON0（可读写）\n
 *   Bit3~0，DAC采样率设置\n
 *   0000：44.1KHz\n
 *   0001：48KHz\n
 *   0010：32KHz\n
 *   0100：22.05KHz\n
 *   0101：24KHz\n
 *   0110：16KHz\n
 *   1000：11.025KHz\n
 *   1001：12KHz\n
 *   1010：8KHz\n
 *   others：reserved，请勿设置\n
 *   Bit4：DAC模块使能  0：关闭DAC  1：打开DAC\n
 *   Bit7~5：声道模式设置\n
 *   0xx：Lout = Lin，Rout = Rin\n
 *   100：Lout = Rout = Lin + Rin\n
 *   101：Lout = Rout = Lin - Rin\n
 *   110：Lout = Rout = Lin\n
 *   111：Lout = Rout = Rin

 * - DAC_VLML\n
 *   DAC左声道音量控制寄存器，只写，读出为不确定值。\n
 *   此寄存器物理上为15bit，允许写入数值为0-16384，0为静音，16384为最大音量。\n
 *   设置此寄存器时，需向此寄存器写入两次，先写入高7位，后写入低8位。\n
 *   如，需设置为0x1234，先写入0x12，再写入0x34

 * - DAC_VLMR\n
 *   DAC右声道音量控制寄存器，只写，读出为不确定值。\n
 *   此寄存器物理上为15bit，允许写入数值为0-16384，0为静音，16384为最大音量。\n
 *   设置此寄存器时，需向此寄存器写入两次，先写入高7位，后写入低8位。\n
 *   如，需设置为0x1234，先写入0x12，再写入0x34\n
 *
 * - KV_START\n
 *   按键音起始地址寄存器，只写，读出为不确定值。\n
 *   设置起始地址时，需向此寄存器写入两次，先写入地址的高8位，后写入地址的低8位。\n
 *   如，需设置起始地址为0x1234，先写入0x12，再写入0x34。\n
 *
 * - KV_END\n
 *   按键音结束地址寄存器，只写，读出为不确定值。\n
 *   设置结束地址时，需向此寄存器写入两次，先写入地址的高8位，后写入地址的低8位。\n
 *   如，需设置结束地址为0x1234，先写入0x12，再写入0x34。\n
 *
 * - KV_CNT:\n
 *   按键音重复次数寄存器，只写，读出为不确定值。\n
 *   此寄存器物理上共有10bit，允许写入的值为1-1023，写入0值不会产生按键音，写入100表示将从KV_START至KV_END的数据播放100次，以此类推。
 *   设置此寄存器时，需向此寄存器写入两次，先写入高2位，后写入低8位。\n
 *   如，需设置重复次数为为0x123，先写入0x1，再写入0x23。\n
 *
 * - KV_VLM:\n
 *   按键音音量，只写，读出为不确定值。\n
 *   按键音音量为一个7bit的无符号数，0为无声，127为最大声。
 *
 * \section dac2 模拟部分控制寄存器
 * - DAA_CON0\n
 *   Bit0：DAC enable,高有效\n
 *   Bit1：DACVDD电压控制位\n
 *   Bit2：LDO1enable，高有效\n
 *   Bit3：LDO2 enable，高有效\n
 *   Bit4: AMUX enable，高有效\n
 *   Bit5: AMUX gain boost控制位，高有效\n
 *   Bit6：Power up/down pop pop noise suppress控制位，高有效\n
 *   Bit7：AMUX mute控制位，高有效 \n
 *
 * - DAA_CON1\n
 *   Bit4~0：DAC L channel gain control\n
 *   Bit7~0: Reserve
 *
 * - DAA_CON2\n
 *   Bit4~0：DAC R channel gain control\n
 *   Bit5：LIN-in BUF enable，For ADC record，高有效\n
 *   Bit6：AMUX L+R>L控制位，高有效\n
 *   Bit7：AMUX L+R>R控制位，高有效\n
 *
 * - DAA_CON3\n
 *   Bit4~0：MIC gain control\n
 *   Bit5：MIC OPA enable，高有效\n
 *   Bit6：MIC > L控制位，高有效\n
 *   Bit7：MIC > R控制位，高有效\n

 * - DAA_CON4\n
 *   Bit0：LIN-in 0 L channel enable,高有效\n
 *   Bit1：LIN-in 0 R channel enable,高有效\n
 *   Bit2：LIN-in 1 L channel enable，高有效\n
 *   Bit3：LIN-in 1 R channel enable，高有效\n
 *   Bit4: VCM detect block enable，高有效\n
 *   Bit5: DAC with AMUX common mode控制位，高有效\n
 *   Bit6：VCM_OUT buffer enable控制位，高有效\n
 *   Bit7：VCM_OUT pull down 1K 电阻控制位，高有效 \n
 */
