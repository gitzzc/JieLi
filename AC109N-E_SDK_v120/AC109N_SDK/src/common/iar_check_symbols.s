/*;-----------------------------------------------------------------------------
;
;	File:		iar_check_symbols.h
;
;	Description:	Consistens check for runtime library building symbols.
;
;
;       REVISON INFORMATION                                 
;                                                           
;       $Revision: 1.10 $
;                                                           
;       Log information is available at the end of this file
;
;-----------------------------------------------------------------------------*/

#ifndef __CORE__
#error "You must define __CORE__"
#endif
#define __CORE_TINY__      0
#define __CORE_PLAIN__     1
#define __CORE_EXTENDED1__ 2

#ifndef __CODE_MODEL__
#error "You must define 'Code model' with: -D__CODE_MODEL__"
#endif
#define __CM_TINY__   0
#define __CM_NEAR__   1
#define __CM_BANKED__ 2
#define __CM_FAR__    3

#ifndef __DATA_MODEL__
#error "You muste define 'data model' with: -D__DATA_MODEL__"
#endif
#define __DM_TINY__      0
#define __DM_SMALL__     1
#define __DM_LARGE__     2
#define __DM_GENERIC__   3
#define __DM_FAR__       4

#ifndef __CALLING_CONVENTION__
#error "You muste define 'calling convention' with: -D__CALLING_CONVENTION__"
#endif
#define __CC_DO__   0
#define __CC_IO__   1
#define __CC_IR__   2
#define __CC_PR__   3
#define __CC_XR__   4
#define __CC_ER__   5

#ifndef __NUMBER_OF_DPTRS__
#error "You must define 'Nr of data pointers' (X) with: -D__NUMBER_OF_DPTRS__"
#endif

#if (__NUMBER_OF_DPTRS__ > 1)
  #if !( defined(__XOR_DPSEL_SELECT__) || defined(__INC_DPSEL_SELECT__) )
    #error "You must define 'DPTR Select method' with: __XOR_DPSEL_SELECT__ or __INC_DPSEL_SELECT__"
  #endif

  #if !( defined(__DPTR_SHADOWED__) || defined(__DPTR_SEPARATE__) )
    #error "You must define 'Visibility for DPTR SFRs' with: __DPTR_SHADOWED__ or __DPTR_SEPARATE__"
  #endif
#endif ; __NUMBER_OF_DPTRS__ > 1


#if (__CORE__ == __CORE_EXTENDED1__)
#undef __CODE_MODEL__
#undef __EXTENDED_DPTR__
#define __CODE_MODEL__ 3
#define __EXTENDED_DPTR__ 1
#endif


/*;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  $Id: iar_check_symbols.h 1.10 2005/02/17 13:29:26Z ola ADx_patch larssu $
;  $Log: iar_check_symbols.h $
;  Revision 1.10  2005/02/17 13:29:26Z  ola
;  Revision 1.9  2005/01/25 09:16:44Z  ola
;  Revision 1.8  2005/01/17 08:07:59Z  ola
;  Revision 1.7  2003/09/24 08:46:05Z  ola
;  Revision 1.6  2003/09/23 15:20:16Z  ola
;  Revision 1.4  2003/06/26 11:23:53Z  owi
;  Revision 1.3  2003/06/13 07:53:42Z  owi
;  Revision 1.2  2003/06/03 16:29:21Z  owi
;  Revision 1.1  2003/05/26 14:03:48Z  owi
;  Initial revision
*/
