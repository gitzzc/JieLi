/*;-----------------------------------------------------------------------------
;
;	File:		iar_cfi.h
;
;	Description:	Call Frame Information (CFI) macros for runtime library.
;
;       REVISON INFORMATION
;
;       $Revision: 1.23 $
;
;       Log information is available at the end of this file
;-----------------------------------------------------------------------------*/


#if (defined(__EXTENDED_DPTR__))
 #define CFA_SPP        CFA_PSP24
 #define SPP            PSP24
 #define CFA_SPX        CFA_XSP24
 #define SPX            XSP24
#else
 #define CFA_SPP        CFA_PSP16
 #define SPP            PSP16
 #define CFA_SPX        CFA_XSP16
 #define SPX            XSP16
#endif
#if (defined(__EXTENDED_STACK__))
 #if (defined(__EXTENDED_DPTR__))
  #define CFA_MACHINE   CFA_ESP24
  #define MACHINE_SP    ESP24
  #define MACHINE_MEM   XDATA
 #else
  #define CFA_MACHINE   CFA_ESP16
  #define MACHINE_SP    ESP16
  #define MACHINE_MEM   XDATA
 #endif ; defined(__EXTENDED_DPTR__)
#else
 #define CFA_MACHINE    CFA_SP
 #define MACHINE_SP     SP
 #define MACHINE_MEM    DATA
#endif ; defined(__EXTENDED_STACK__)

#if (__CODE_MODEL__ == __CM_FAR__)
 #define __SIZE_OF_RETADDR__ 3
#else
 #define __SIZE_OF_RETADDR__ 2
#endif

/*;--------------------------------------------------------------
;
; CFI Names macros
;
;--------------------------------------------------------------

; register resources
;--------------------------------------------------------------*/
CFI_NAMES_DECLARE_REGISTER_RESOURCES MACRO
        cfi Resource A:8, B:8, PSW:8
#if (__NUMBER_OF_DPTRS__ > 1)
        cfi Resource DPSEL:8
#endif
        cfi Resource DPTR0:16
#if (__NUMBER_OF_DPTRS__ > 1)
        cfi Resource DPTR1:16
#endif
#if (__NUMBER_OF_DPTRS__ > 2)
        cfi Resource DPTR2:16
#endif
#if (__NUMBER_OF_DPTRS__ > 3)
        cfi Resource DPTR3:16
#endif
#if (__NUMBER_OF_DPTRS__ > 4)
        cfi Resource DPTR4:16
#endif
#if (__NUMBER_OF_DPTRS__ > 5)
        cfi Resource DPTR5:16
#endif
#if (__NUMBER_OF_DPTRS__ > 6)
        cfi Resource DPTR6:16
#endif
#if (__NUMBER_OF_DPTRS__ > 7)
        cfi Resource DPTR7:16
#endif
        cfi Resource DPL0:8
        cfi Resource DPH0:8
#ifdef __EXTENDED_DPTR__
        cfi Resource DPX0:8
#endif
#if (__NUMBER_OF_DPTRS__ > 1)
        cfi Resource DPL1:8, DPH1:8
#ifdef __EXTENDED_DPTR__
        cfi Resource DPX1:8
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 2)
        cfi Resource DPL2:8, DPH2:8
#ifdef __EXTENDED_DPTR__
        cfi Resource DPX2:8
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 3)
        cfi Resource DPL3:8, DPH3:8
#ifdef __EXTENDED_DPTR__
        cfi Resource DPX3:8
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 4)
        cfi Resource DPL4:8, DPH4:8
#ifdef __EXTENDED_DPTR__
        cfi Resource DPX4:8
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 5)
        cfi Resource DPL5:8, DPH5:8
#ifdef __EXTENDED_DPTR__
        cfi Resource DPX5:8
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 6)
        cfi Resource DPL6:8, DPH6:8
#ifdef __EXTENDED_DPTR__
        cfi Resource DPX6:8
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 7)
        cfi Resource DPL7:8, DPH7:8
#ifdef __EXTENDED_DPTR__
        cfi Resource DPX7:8
#endif
#endif
        cfi Resource R0:8, R1:8, R2:8, R3:8, R4:8, R5:8, R6:8, R7:8
        cfi Resource VB:8
        cfi Resource V0:8,  V1:8,  V2:8,  V3:8,  V4:8,  V5:8,  V6:8,  V7:8
        cfi Resource V8:8,  V9:8,  V10:8, V11:8, V12:8, V13:8, V14:8, V15:8
        cfi Resource V16:8, V17:8, V18:8, V19:8, V20:8, V21:8, V22:8, V23:8
        cfi Resource V24:8, V25:8, V26:8, V27:8, V28:8, V29:8, V30:8, V31:8
        cfi Resource ?V_START:8
        ENDM 
        
//; return resources
//;--------------------------------------------------------------
CFI_NAMES_DECLARE_RETURN_RESOURCE MACRO
#if   (__CODE_MODEL__ == __CM_FAR__)
        cfi VirtualResource     ?RET:24, ?RET_LOW:8, ?RET_HIGH:8, ?RET_EXT:8
        cfi ResourceParts       ?RET ?RET_EXT, ?RET_HIGH, ?RET_LOW
#else
        cfi VirtualResource     ?RET:16, ?RET_LOW:8, ?RET_HIGH:8
        cfi ResourceParts       ?RET ?RET_HIGH, ?RET_LOW
#endif
        ENDM 

//; stack resources
//;--------------------------------------------------------------
CFI_NAMES_DECLARE_STACKS MACRO
        cfi StackFrame CFA_MACHINE MACHINE_SP MACHINE_MEM
#if (defined(__EXTENDED_DPTR__))
        cfi StackFrame    CFA_PSP24 PSP24 XDATA
        cfi StackFrame    CFA_XSP24 XSP24 XDATA
        cfi Resource      PSP24:24, PSPX:8, PSPH:8, PSPL:8
        cfi Resource      XSP24:24, XSPX:8, XSPH:8, XSPL:8
        cfi ResourceParts PSP24 PSPX, PSPH, PSPL
        cfi ResourceParts XSP24 XSPX, XSPH, XSPL
#if (defined(__EXTENDED_STACK__))
        cfi Resource      MACHINE_SP:24
        cfi Resource      ESPX:8, ESPH:8, ESPL:8
        cfi ResourceParts MACHINE_SP ESPX, ESPH, ESPL
#else
        cfi Resource MACHINE_SP:8
#endif
        
#else
        cfi StackFrame    CFA_PSP16 PSP16 XDATA
        cfi StackFrame    CFA_XSP16 XSP16 XDATA
        cfi Resource      PSP16:16, PSPH:8, PSPL:8
        cfi Resource      XSP16:16, XSPH:8, XSPL:8
        cfi ResourceParts PSP16 PSPH, PSPL
        cfi ResourceParts XSP16 XSPH, XSPL
#if (defined(__EXTENDED_STACK__))
        cfi Resource      MACHINE_SP:16
        cfi Resource      ESPH:8, ESPL:8
        cfi ResourceParts MACHINE_SP ESPH, ESPL
#else
        cfi Resource MACHINE_SP:8
#endif

#endif ; defined(__EXTENDED_DPTR__)

        ENDM

//; This names block is used by all runtime library routines.
//;--------------------------------------------------------------
CFI_nam: MACRO
        cfi NAMES cfi_names
        CFI_NAMES_DECLARE_REGISTER_RESOURCES()
        CFI_NAMES_DECLARE_STACKS()
        CFI_NAMES_DECLARE_RETURN_RESOURCE()
        cfi ENDNAMES cfi_names
        ENDM

CFI_banked_nam: MACRO
        CFI NAMES cfi_banked_names
        CFI_NAMES_DECLARE_REGISTER_RESOURCES()
        CFI_NAMES_DECLARE_STACKS()
        cfi VirtualResource     ?RET:24, ?RET_LOW:8, ?RET_HIGH:8, ?RET_EXT:8
        cfi ResourceParts       ?RET ?RET_EXT, ?RET_HIGH, ?RET_LOW
        cfi ENDNAMES cfi_banked_names
        ENDM

CFI_pdata_enter_nam: MACRO
        CFI NAMES cfi_pdata_enter_names EXTENDS cfi_names
        CFI BASEADDRESS Base XDATA
        CFI FRAMECELL RETH_CELL  Base(1):8
        CFI FRAMECELL RETL_CELL  Base(2):8
        CFI ENDNAMES cfi_pdata_enter_names
        ENDM

/*;--------------------------------------------------------------
;
; CFI Common Macros
;
;--------------------------------------------------------------*/
CFI_COMMON_DECLARE_CORE_COMMON MACRO
        cfi CODEALIGN 1
        cfi DATAALIGN -1
        cfi ?V_START SameValue
#if (defined(__EXTENDED_DPTR__))
        cfi CFA_PSP24 PSP24
        cfi CFA_XSP24 XSP24
        cfi PSPX Undefined
        cfi PSPL Undefined
        cfi PSPH Undefined
        cfi XSPL Undefined
        cfi XSPH Undefined
        cfi XSPX Undefined
#else
        cfi CFA_PSP16 PSP16
        cfi CFA_XSP16 XSP16
        cfi PSPL Undefined
        cfi PSPH Undefined
        cfi XSPL Undefined
        cfi XSPH Undefined
#endif ; defined(__EXTENDED_DPTR__)

#if (defined(__EXTENDED_STACK__))
        cfi ESPL Undefined
        cfi ESPH Undefined
#if (defined(__EXTENDED_DPTR__))
        cfi ESPX Undefined
#endif
#endif
        cfi ?RET CONCAT
        ENDM

CFI_COMMON_DECLARE_SAME_VALUE_FOR_ALL_REGISTERS MACRO
        cfi A    undefined
        cfi B    SameValue
        cfi PSW  samevalue
#if (__NUMBER_OF_DPTRS__ > 1)
        cfi DPSEL SameValue
#endif
        cfi DPTR0 SameValue
#if (__NUMBER_OF_DPTRS__ > 1)
        cfi DPTR1 SameValue
#endif
#if (__NUMBER_OF_DPTRS__ > 2)
        cfi DPTR2 SameValue
#endif
#if (__NUMBER_OF_DPTRS__ > 3)
        cfi DPTR3 SameValue
#endif
#if (__NUMBER_OF_DPTRS__ > 4)
        cfi DPTR4 SameValue
#endif
#if (__NUMBER_OF_DPTRS__ > 5)
        cfi DPTR5 SameValue
#endif
#if (__NUMBER_OF_DPTRS__ > 6)
        cfi DPTR6 SameValue
#endif
#if (__NUMBER_OF_DPTRS__ > 7)
        cfi DPTR7 SameValue
#endif
        cfi DPL0 SameValue
        cfi DPH0 SameValue
#ifdef __EXTENDED_DPTR__
        cfi DPX0 SameValue
#endif
#if (__NUMBER_OF_DPTRS__ > 1)
        cfi DPL1 SameValue
        cfi DPH1 SameValue
#ifdef __EXTENDED_DPTR__
        cfi DPX1 SameValue
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 2)
        cfi DPL2 SameValue
        cfi DPH2 SameValue
#ifdef __EXTENDED_DPTR__
        cfi DPX2 SameValue
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 3)
        cfi DPL3 SameValue
        cfi DPH3 SameValue
#ifdef __EXTENDED_DPTR__
        cfi DPX3 SameValue
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 4)
        cfi DPL4 SameValue
        cfi DPH4 SameValue
#ifdef __EXTENDED_DPTR__
        cfi DPX4 SameValue
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 5)
        cfi DPL5 SameValue
        cfi DPH5 SameValue
#ifdef __EXTENDED_DPTR__
        cfi DPX5 SameValue
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 6)
        cfi DPL6 SameValue
        cfi DPH6 SameValue
#ifdef __EXTENDED_DPTR__
        cfi DPX6 SameValue
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 7)
        cfi DPL7 SameValue
        cfi DPH7 SameValue
#ifdef __EXTENDED_DPTR__
        cfi DPX7 SameValue
#endif
#endif
        cfi R0   SameValue
        cfi R1   SameValue
        cfi R2   SameValue
        cfi R3   SameValue
        cfi R4   SameValue
        cfi R5   SameValue
        cfi R6   SameValue
        cfi R7   SameValue
        cfi VB   SameValue
        cfi V0   SameValue
        cfi V1   SameValue
        cfi V2   SameValue
        cfi V3   SameValue
        cfi V4   SameValue
        cfi V5   SameValue
        cfi V6   SameValue
        cfi V7   SameValue
        cfi V8   SameValue
        cfi V9   SameValue
        cfi V10  SameValue
        cfi V11  SameValue
        cfi V12  SameValue
        cfi V13  SameValue
        cfi V14  SameValue
        cfi V15  SameValue
        cfi V16  SameValue
        cfi V17  SameValue
        cfi V18  SameValue
        cfi V19  SameValue
        cfi V20  SameValue
        cfi V21  SameValue
        cfi V22  SameValue
        cfi V23  SameValue
        cfi V24  SameValue
        cfi V25  SameValue
        cfi V26  SameValue
        cfi V27  SameValue
        cfi V28  SameValue
        cfi V29  SameValue
        cfi V30  SameValue
        cfi V31  SameValue
        ENDM


CFI_COMMON_DECLARE_UNDEFINED_FOR_ALL_DPTR_REGISTERS MACRO
        cfi DPTR0 Undefined
#if (__NUMBER_OF_DPTRS__ > 1)
        cfi DPTR1 Undefined
#endif
#if (__NUMBER_OF_DPTRS__ > 2)
        cfi DPTR2 Undefined
#endif
#if (__NUMBER_OF_DPTRS__ > 3)
        cfi DPTR3 Undefined
#endif
#if (__NUMBER_OF_DPTRS__ > 4)
        cfi DPTR4 Undefined
#endif
#if (__NUMBER_OF_DPTRS__ > 5)
        cfi DPTR5 Undefined
#endif
#if (__NUMBER_OF_DPTRS__ > 6)
        cfi DPTR6 Undefined
#endif
#if (__NUMBER_OF_DPTRS__ > 7)
        cfi DPTR7 Undefined
#endif
        cfi DPL0 Undefined
        cfi DPH0 Undefined
#ifdef __EXTENDED_DPTR__
        cfi DPX0 Undefined
#endif
#if (__NUMBER_OF_DPTRS__ > 1)
        cfi DPL1 Undefined
        cfi DPH1 Undefined
#ifdef __EXTENDED_DPTR__
        cfi DPX1 Undefined
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 2)
        cfi DPL2 Undefined
        cfi DPH2 Undefined
#ifdef __EXTENDED_DPTR__
        cfi DPX2 Undefined
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 3)
        cfi DPL3 Undefined
        cfi DPH3 Undefined
#ifdef __EXTENDED_DPTR__
        cfi DPX3 Undefined
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 4)
        cfi DPL4 Undefined
        cfi DPH4 Undefined
#ifdef __EXTENDED_DPTR__
        cfi DPX4 Undefined
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 5)
        cfi DPL5 Undefined
        cfi DPH5 Undefined
#ifdef __EXTENDED_DPTR__
        cfi DPX5 Undefined
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 6)
        cfi DPL6 Undefined
        cfi DPH6 Undefined
#ifdef __EXTENDED_DPTR__
        cfi DPX6 Undefined
#endif
#endif
#if (__NUMBER_OF_DPTRS__ > 7)
        cfi DPL7 Undefined
        cfi DPH7 Undefined
#ifdef __EXTENDED_DPTR__
        cfi DPX7 Undefined
#endif
#endif
        ENDM

/*;--------------------------------------------------------------
;
; CFI macro: COMMON block
;
; This common block is used by all runtime library routines.
;
;--------------------------------------------------------------*/
CFI_com: MACRO
        cfi COMMON cfi_common USING cfi_names
        CFI_COMMON_DECLARE_CORE_COMMON()
        cfi ?RET_LOW  Frame(CFA_MACHINE, 1)
        cfi ?RET_HIGH Frame(CFA_MACHINE, 2)
#if   (__CODE_MODEL__ == __CM_FAR__)
        cfi ?RET_EXT  Frame(CFA_MACHINE, 3)
#endif
        cfi CFA_MACHINE MACHINE_SP - __SIZE_OF_RETADDR__
        cfi RETURNADDRESS ?RET CODE
        CFI_COMMON_DECLARE_SAME_VALUE_FOR_ALL_REGISTERS()
        cfi ENDCOMMON cfi_common
        ENDM

CFI_banked_com: MACRO
        cfi COMMON cfi_banked_common USING cfi_banked_names
        CFI_COMMON_DECLARE_CORE_COMMON()
        cfi ?RET_LOW  Frame(CFA_MACHINE, 1)
        cfi ?RET_HIGH Frame(CFA_MACHINE, 2)
        cfi ?RET_EXT  Frame(CFA_MACHINE, 3)
//        cfi ?RET_EXT  load(1, SFR, 0x90)
        cfi CFA_MACHINE MACHINE_SP - 3
        cfi RETURNADDRESS ?RET CODE
        CFI_COMMON_DECLARE_SAME_VALUE_FOR_ALL_REGISTERS()
        cfi ENDCOMMON cfi_banked_common
        ENDM

CFI_pdata_enter_com: MACRO
        cfi COMMON cfi_pdata_enter_common USING cfi_pdata_enter_names
        CFI_COMMON_DECLARE_CORE_COMMON()
        cfi CFA_MACHINE sub(sub(MACHINE_SP,__SIZE_OF_RETADDR__),__SIZE_OF_RETADDR__)
        cfi Base SPP
        cfi RETL_CELL Frame(CFA_MACHINE, 1, 1)
        cfi RETH_CELL Frame(CFA_MACHINE, 2, 1)
#if (__CORE__ == __CORE_EXTENDED1__)
        CFI ?RET_LOW  Frame(CFA_MACHINE, 3, 1)
        CFI ?RET_HIGH Frame(CFA_MACHINE, 4, 1)
        CFI ?RET_EXT  Frame(CFA_MACHINE, 5, 1)
#else
        CFI ?RET_LOW  Frame(CFA_MACHINE, 3, 1)
        CFI ?RET_HIGH Frame(CFA_MACHINE, 4, 1)
#endif
        cfi RETURNADDRESS ?RET CODE
        CFI_COMMON_DECLARE_SAME_VALUE_FOR_ALL_REGISTERS()
        cfi ENDCOMMON cfi_pdata_enter_common
        ENDM

////////////////////////////////////////////////////////////////////////////////////////
// EXPORT above macros
////////////////////////////////////////////////////////////////////////////////////////
         CFI_nam
         CFI_banked_nam
         CFI_pdata_enter_nam
         CFI_com
         CFI_banked_com
         CFI_pdata_enter_com
////////////////////////////////////////////////////////////////////////////////////////

/*
 * CFI Macro: cfi_DATA_BLOCK_start name
 *            cfi_DATA_BLOCK_end name
 *
 * Parameter: name      Name of data block
 *
 * Description: Call Frame Information Data Block
 */

cfi_DATA_BLOCK_start    MACRO name
                        cfi block \1 using cfi_common
                        cfi nofunction
cfi_OFFSET              SET      0
                        ENDM

cfi_DATA_BLOCK_end      MACRO name
                        IF cfi_OFFSET
                          #error "Non-zero offset on cfi data block end"
                        ELSE
                          cfi endblock \1
                        ENDIF
                        ENDM

/*
 * CFI Macro: cfi_STACK_ALLOC bytes
 *
 * Parameter: bytes      Number of bytes to allocate
 *
 * Description: Allocate word on stack
 */

cfi_STACK_ALLOC:        MACRO   bytes
cfi_OFFSET              SET     cfi_OFFSET + bytes
                        cfi     CFA_MACHINE MACHINE_SP - (__SIZE_OF_RETADDR__ + cfi_OFFSET)
                        ENDM

/*
 * CFI Macro: cfi_STACK_DEALL bytes
 *
 * Parameter: bytes      Number of bytes to deallocate
 *
 * Description: Deallocate word from stack
 */

cfi_STACK_DEALL:        MACRO   bytes
cfi_OFFSET              SET     cfi_OFFSET - bytes
                        IF (cfi_OFFSET + __SIZE_OF_RETADDR__) < 0
#ifndef __IAR_SYSTEMS_ICC__ /* Let it accept preprocessing by compiler */
                            #error "Negative size on cfi deallocate"
#endif
                        ELSEIF cfi_OFFSET
                          cfi     CFA_MACHINE MACHINE_SP - (__SIZE_OF_RETADDR__ + cfi_OFFSET)
                        ELSE
                          cfi     CFA_MACHINE MACHINE_SP - __SIZE_OF_RETADDR__
                        ENDIF
                        ENDM

/*
 * CFI Macro: cfi_PUSH_REG reg offset
 *
 * Parameter: reg       Register to push
 *            offset    Stack offset of pushed register
 *
 * Description: Move reg to stack
 */

cfi_PUSH_REG:           MACRO reg
                        cfi_STACK_ALLOC 1
                        cfi reg frame(CFA_MACHINE, __SIZE_OF_RETADDR__ + cfi_OFFSET)
                        ENDM


/*
 * CFI Macro: cfi_SET_REG reg_from, reg_to
 *
 * Parameter: reg_from  Register to move from
 *            reg_to    Register to move to or 'samevalue'
 *
 * Description: Move from REG to REG (samevalue)
 */

cfi_SET_REG:            MACRO reg_from, reg_to
                        cfi reg_from reg_to
                        ENDM

/*
 * CFI Macro: cfi_REMEMBER_STATE
 *
 * Parameter: no        State number to remember
 *
 * Description: Remember state
 */

cfi_REMEMBER_STATE:     MACRO no
                        cfi rememberstate
cfi_OFFSET_\1           SET      cfi_OFFSET
                        ENDM


/*
 * CFI Macro: cfi_RESTORE_STATE
 *
 * Parameter: no        State number to restore
 *
 * Description: Restore state
 */

cfi_RESTORE_STATE:      MACRO no
                        cfi restorestate
cfi_OFFSET              SET      cfi_OFFSET_\1
                        ENDM



cfi_PUSH_ISTACK_RET_DEFS_CONST_DEPEND: MACRO constant
#if (defined(__EXTENDED_STACK__))
    cfi ?RET_LOW  load(1, XDATA, sub(CFA_MACHINE, sub(constant,1)))
    cfi ?RET_HIGH load(1, XDATA, sub(CFA_MACHINE, sub(constant,2)))
#else
    cfi ?RET_LOW  load(1, IDATA, sub(CFA_MACHINE, sub(constant,1)))
    cfi ?RET_HIGH load(1, IDATA, sub(CFA_MACHINE, sub(constant,2)))
#endif
#if (__CORE__ == __CORE_EXTENDED1__)
  #if (defined(__EXTENDED_STACK__))
    cfi ?RET_EXT  load(1, XDATA, sub(CFA_MACHINE, sub(constant,3)))
  #else
    cfi ?RET_EXT  load(1, IDATA, sub(CFA_MACHINE, sub(constant,3)))
  #endif
#endif
                        ENDM

cfi_PUSH_ISTACK_RET_DEFS_REG_DEPEND: MACRO reg
#if (defined(__EXTENDED_STACK__))
    cfi ?RET_LOW  load(1, XDATA, sub(CFA_MACHINE, sub(reg,1)))
    cfi ?RET_HIGH load(1, XDATA, sub(CFA_MACHINE, sub(reg,2)))
#else
    cfi ?RET_LOW  load(1, IDATA, sub(CFA_MACHINE, sub(reg,1)))
    cfi ?RET_HIGH load(1, IDATA, sub(CFA_MACHINE, sub(reg,2)))
#endif
#if (__CORE__ == __CORE_EXTENDED1__)
  #if (defined(__EXTENDED_STACK__))
    cfi ?RET_EXT  load(1, XDATA, sub(CFA_MACHINE, sub(reg,3)))
  #else
    cfi ?RET_EXT  load(1, IDATA, sub(CFA_MACHINE, sub(reg,3)))
  #endif
#endif
                        ENDM

/*;
; Revision control system
;       $Id: iar_cfi.h 1.23 2005/03/04 10:29:57Z owi ADx_patch $
;       $Log: iar_cfi.h $
;       Revision 1.23  2005/03/04 10:29:57Z  owi
;       Revision 1.22  2004/11/22 15:15:12Z  owi
;       Revision 1.21  2004/11/18 13:55:17Z  owi
;       Revision 1.20  2004/11/16 16:41:06Z  owi
;       Revision 1.19  2004/11/10 16:31:02Z  owi
;       Revision 1.18  2004/11/10 08:51:14Z  owi
;       Revision 1.17  2004/09/16 08:02:35Z  owi
;       Revision 1.16  2004/03/15 16:45:32Z  owi
;       Revision 1.15  2004/03/15 13:15:28Z  owi
;       Revision 1.14  2004/03/12 15:46:22Z  owi
;       Revision 1.13  2004/01/27 15:19:11Z  owi
;       Revision 1.12  2004/01/27 13:57:41Z  owi
;       Revision 1.11  2003/10/23 11:17:37Z  owi
;       Revision 1.10  2003/10/08 08:51:25Z  owi
*/