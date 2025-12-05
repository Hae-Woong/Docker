/* Kernbauer Version: 1.16 Konfiguration: DrvTransCan Erzeugungsgangnummer: 356 */



/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 |  FILE DESCRIPTION
 |  -------------------------------------------------------------------------------------------------------------------
 |         File:  CanTrcv_30_GenericCan.c
 |      Project:  MICROSAR
 |       Module:  CAN Transceiver driver
 |    Generator:  DaVinci Configurator
 |  Description:  Implementation of the MICROSAR CAN Transceiver Driver.
 **********************************************************************************************************************/

 /**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Senol Cendere                 Ces           Vector Informatik GmbH
 *  Matthias Fleischmann          Fn            Vector Informatik GmbH
 *  Mihai Olariu                  Ou            Vector Informatik GmbH
 *  Timo Vanoni                   Tvi           Vector Informatik GmbH
 *  Eugen Stripling               Seu           Vector Informatik GmbH
 *  Jan Hammer                    Jan           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  1.00.00   2007-06-12  Ces     -             First Implementation
 *  1.01.00   2008-07-11  Ou      -             Not usable version because the generic transceiver driver was addapted with the hand (according to the process HL + Ll_template)
 *  1.02.00   2008-07-14  Ou      -             Separate the template version of generic transceiver driver low-level version
 *  1.03.00   2008-07-25  Ou      ESCAN00028568 Compile error: Versions of generated files does not map to static files
 *  1.04.00   2008-08-21  Ou      ESCAN00029373 Version scanning fails for the generic transceiver driver
 *  1.05.00   2008-09-12  Ou      -             Create intermediate macros for low-level version
 *  1.06.00   2008-10-08  Ou      -             Remove the V_SUPPRESS_EXTENDED_VERSION_CHECK encapsulations, the DLLs versions are always checked
 *  1.07.00   2008-10-28  Ou      -             No changes, just update the specific DLLs
 *  1.08.00   2008-12-11  Ou      -             Use the Autosar compliant copyright
 *  1.09.00   2009-02-19  Tvi     -             Changed to match with updated DLLs
 *  1.10.00   2009-02-26  Tvi     ESCAN00033398 Compiler warnings when using different transceiver types within the same stack
 *  1.11.00   2009-07-01  Fn      -             (no changes here, just to merge with HL)
 *            2009-07-27  Tvi     ESCAN00033879 Define is written wrong and not enclosured with a switch
 *  1.11.01   2009-10-16  Tvi     -             (no changes here, just to merge with HL)
 *  1.12.00   2009-11-04  Tvi     -             (no changes here, just to merge with HL)
 *  1.13.00   2010-05-04  Tvi     -             (no changes here, just to merge with HL)
 *  1.14.00   2010-12-06  Tvi     ESCAN00047454 Improve comments for Generic Trcv
 *  1.14.01   2011-02-08  Tvi     -             (no changes here, just to merge with HL)
 *  1.14.02   2011-04-04  Tvi     -             (no changes here, just to merge with HL)
 *  2.00.00   2011-07-12  Tvi     -             (no changes here, just to merge with HL)
 *  2.01.00   2011-11-28  Tvi     ESCAN00053775 provide Generic Transceiver driver for partial networking
 *  2.01.01   2012-05-03  Tvi     -             (no changes here, just to merge with HL)
 *  3.00.00   2012-07-30  Tvi     ESCAN00060109 AR3-2406: Asynchronous transceiver handling according to AUTOASR 3.2.2
 *  3.00.01   2012-09-27  Tvi     -             (remerge with CFG5 support)
 *  3.01.00   2013-04-04  Tvi     -             Add new macro "WupIsValid"
 *  3.02.00   2014-02-05  Tvi     -             (no changes here, just to merge with HL)
 *  4.00.00   2014-10-15  Seu     ESCAN00078952 AR4-698: Post-Build Selectable (Identity Manager)
 *  4.00.01   2015-07-29  Seu     -             (no changes here, just to merge with HL)
 *  4.01.00   2015-10-21  Seu     -             Merge with new HL
 *            2015-10-22  Seu     ESCAN00086019 Harmonise the default VendorApiInfix to "GenericCan"
 *  4.01.01   2016-10-17  Seu     -             Merge with new HL
 *  4.02.00   2018-08-21  Jan     STORYC-6330   MISRA-C:2012 Compliance, Merge with new HL
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * ATTENTION: Replace the placeholders GenericCan and GENERICCAN with the according name of the used transceiver.
 *            GENERICCAN is used for definitions in upper case.
 *            GenericCan is used for variables in camel case.
 * EXAMPLE:   Replace GENERICCAN to TJA1041 and GenericCan to Tja1041.
 *********************************************************************************************************************/
/* ********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Matthias Fleischmann          Fn            Vector Informatik GmbH
 *  Mihai Olariu                  Ou            Vector Informatik GmbH
 *  Timo Vanoni                   Tvi           Vector Informatik GmbH
 *  Eugen Stripling               Seu           Vector Informatik GmbH
 *  Jan Hammer                    Jan           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2008-02-25  Fn      -             Intitial revision of the core implementation
 *  01.00.01  2008-04-17  Fn      -             Update of the include file names
 *  01.01.00  2008-06-26  Ou      ESCAN00027616 Various compile errors, problems with Organi
 *  01.02.00  2008-09-12  Ou      ESCAN00029980 Compiler failure due to wrong definition of SW version
 *  01.03.00  2008-10-07  Ou      ESCAN00028298 Several warnings due to unreferenced local variables
 *                        Ou      ESCAN00029572 DrvTrans__coreCanxAsr raise DET error if CanTrcv_SetOpMode is called with invalid TrcvWakeupMode
 *                        Ou      ESCAN00030536 Compile error at extended version check
 *  01.04.00  2008-12-11  Ou      ESCAN00031771 Adjust wakeup by bus handling for platforms which cannot be woken up with a CAN frame
 *  01.05.00  2009-02-19  Tvi     ESCAN00033258 GetBusWuReason may return an invalid value
 *  01.06.00  2009-02-26  Tvi     ESCAN00033383 Don't throw DET error in case the same mode is requested again
 *                        Tvi     ESCAN00033398 Compiler warnings when using different transceiver types within the same stack
 *                        Tvi     ESCAN00033413 Possible out of bounds access in SetOpMode if DET is enabled
 *  02.00.00  2009-06-29  Fn      ESCAN00035913 CAN Transceiver channel does not wake up
 *  02.00.01  2009-10-15  Tvi     ESCAN00038495 Compile error: unknown type definitions in dio.h
 *  02.01.00  2009-11-04  Tvi     ESCAN00036841 OP_MODE_SLEEP should be independent of GENERAL_WAKE_UP_SUPPORT
 *                        Tvi     ESCAN00038945 Use different DioPins tokens for GetOpMode and CHECK_WA
 *                        Tvi     ESCAN00038946 Rename filenames to match BSW00347
 *                        Tvi     ESCAN00038947 CanTrcv087 has to be implemented the correct way
 *                        Tvi     ESCAN00039039 Add support for timers
 *                        Tvi     ESCAN00039041 Allow transition to OP_MODE_NORMAL even if it is the current OpMode
 *                        Tvi     ESCAN00039043 Implement an initialization via OP_MODE_NORMAL
 *                        Tvi     ESCAN00039194 Disable IcuNotification when entering Normal mode in Init and SetOpMode
 *  02.02.00  2010-05-04  Tvi     ESCAN00040088 Compiler Warning: unused parameter 'TrcvWakeupMode'
 *  02.20.00  2010-07-01  Tvi     ESCAN00046512 Add support for partial network transceivers
 *            2010-11-29  Tvi     ESCAN00046781 Support Multiple Identity Configuration
 *                                ESCAN00045601 "Channel Used" parameter not well implemented
 *                                ESCAN00046813 Adapt AUTOSAR version check
 *                                ESCAN00047668 Critical areas should be Trcv specific
 *  02.20.01  2011-02-08  Tvi     ESCAN00048498 Wrong DEM Events used
 *  02.20.02  2011-03-11  Tvi     ESCAN00049830 Remove ASR Version check for SPI / DIO
 *                                ESCAN00049831 [PN only] Store wakeup event and report them if Trcv is in Standby mode
 *  03.00.00  2011-05-05  Tvi     ESCAN00051930 Add support for ASR3.2.1
 *                                ESCAN00051564 Remove the AUTOSAR Release Version Check
 *  03.01.00  2011-11-22  Tvi     ESCAN00054349 AR3-1738: Add support for asynchronous transceiver driver
 *                                ESCAN00053272 AR3-1698: Add MISRA justifications in source code for automatic generation of MISRA reports
 *                                ESCAN00053265 AR3-2069: Remove non-SchM code for critical section handling
 *  03.01.01  2012-02-12  Tvi     ESCAN00057892 [PN only] CanTrcv does not wakeup when Synchronous SPI is being used
 *                        Tvi     ESCAN00056788 Compiler error/warning: DRVTRANS__CORECANXASR_RELEASE_VERSION' redefined
 *  04.00.00  2012-05-21  Tvi     ESCAN00058641 AR4-95: Provide CanTrcv according ASR 4, Asynchronous Transceiver Mode Changes
 *                                ESCAN00059656 Add support for SetPNActivationState according to ASR4
 *                                ESCAN00060455 AR3-2406: Asynchronous transceiver handling according to AUTOASR 3.2.2
 *  04.00.01  2012-09-27  Tvi     ESCAN00061779 [CFG5/ASR4 only]: Compiler error about unkown struct member instanceID
 *  04.01.00  2012-10-08  Tvi     ESCAN00062121 Report DEM if PN is not available
 *                        Tvi     ESCAN00066283 AR4-352: Support partial networking
 *  04.02.00  2013-11-20  Tvi     ESCAN00071370 AR4-569: [CanTrcv]  Incompatible  argument  in call to EcuM_SetWakeupEvent()
 *  04.03.00  2014-01-10  Tvi     ESCAN00073861 AR4-345: Implement PF4_PostBuildSelectable
 *  05.00.00  2014-09-24  Seu     ESCAN00076622 AR4-698: Post-Build Selectable (Identity Manager)
 *  05.00.01  2014-11-13  Seu     -             MISRA deviation comment for rule 6080 added (APIs: *_ClearTrcvWufFlag() and *_CheckWakeFlag())
 *  05.01.00  2015-05-04  Seu     ESCAN00077814 Support for CAN Transceiver on System-Basis Chips (SBCs)
 *            2015-07-23  Seu     ESCAN00084154 Only since R13: Compiler error: C2059: syntax error : '{'
 *            2015-07-23  Seu     ESCAN00083583 Adapt in order to fix ESCAN00083361 / ESCAN00083362
 *            2015-07-23  Seu     ESCAN00083395 Compiler warning: parameter is never used
 *  05.01.01  2015-09-24  Seu     ESCAN00085451 Support usage of Partial Networking for SBCs.
 *  05.01.02  2015-10-06  Seu     ESCAN00085678 Initialize requestState in case of INTERFACE_SBC and PN is used
 *  05.02.00  2015-02-04  Seu     ESCAN00080310 FEAT-427: SafeBSW Step I
 *            2015-10-05  Seu     ESCAN00081795 FEAT-1275: SafeBSW Step 2
 *            2015-10-05  Seu     ESCAN00085154 FEAT-1527: SafeBSW Step 3
 *  05.03.00  2016-01-16  Seu     ESCAN00087580 SafeBSW - Silent Analysis
 *            2016-03-09  Seu     ESCAN00088810 Add Postbuild Loadable Support for CanTrcvDrv
 *  05.04.00  2016-04-05  Seu     ESCAN00087813 FEAT-1688: SafeBSW Step 4
 *            2016-05-19  Seu     ESCAN00090097 Compiler error: Syntax Error caused by undefined compiler class
 *  05.04.01  2016-08-02  Seu     ESCAN00091270 MISRA deviation: MISRA-C:2004 Rule 8.8
 *            2016-08-17  Seu     ESCAN00091505 Improve code coverage and doxygen comments
 *            2016-10-17  Seu     ESCAN00091616 Compiler error: identifier index not declared
 *  05.05.00  2016-11-23  Seu     ESCAN00092817 Add support for asynchronous SPI without polling support
 *            2016-11-29  Seu     ESCAN00093075 [Only: Tja1145 / E52013] In case of startup of ECU SysErr-wakeup is reported instead of PowerOn-wakeup
 *            2016-12-06  Seu     ESCAN00093177 Omit interrupted transition: STANDBY -> NORMAL (pending) -> SLEEP  [only async SPI]
 *  05.06.00  2017-04-05  Seu     ESCAN00094603 Add support for requirements: CanTrcv_00230, CanTrcv_00190, CanTrcv_00191
 *            2017-04-05  Seu     ESCAN00094658 Improve doxygen comments
 *  05.06.01  2017-05-30  Seu     ESCAN00095350 Add: SafeBSW support for DIO-CanTrcv, flexibility for SPI-CanTrcv
 *  05.07.00  2018-08-21  Jan     STORYC-4868   MISRA-C:2012 Compliance
 * *******************************************************************************************************************/

/* PRQA S 0715 EOF */ /* MD_MSR_1.1_715 */
/* PRQA S 0850 EOF */ /* MD_MSR_MacroArgumentEmpty */
/* PRQA S 0777 EOF */ /* MD_MSR_Rule5.1 */
/* PRQA S 3453 EOF */ /* MD_MSR_FctLikeMacro */
/* PRQA S 0779 EOF */ /* MD_MSR_Rule5.2 */

/* ********************************************************************************************************************
 *  INCLUDES
 * *******************************************************************************************************************/
#define CANTRCV_30_GENERICCAN_INCLUDE_INTERNAL
#define V_CANTRCV_30_GENERICCAN_SOURCE

/* \trace SPEC-19973, SPEC-20008, SPEC-19954, SPEC-19965 */
/* \trace SPEC-14977, SPEC-15093, SPEC-15086, SPEC-14969 */
#include "CanTrcv_30_GenericCan.h"

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
# include "Spi.h"
#endif

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
# include "Dio.h"
#endif

#include "CanIf.h"

#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
# include "Det.h"
#endif

#include "EcuM.h"




/* *********************************************************************************************************************
 *  VERSION CHECK
 * ********************************************************************************************************************/

#if (DRVTRANS__CORECANXASR_VERSION                             != 0x0507u)
# error "Module CanTrcv_30_GenericCan: Source and Header files are inconsistent!"
#endif
#if (DRVTRANS__CORECANXASR_RELEASE_VERSION                     != 0x00u)
# error "Module CanTrcv_30_GenericCan: Source and Header files are inconsistent!"
#endif


/* ** Version Check: CFG5 - Base part ** */
#define CANTRCV_30_GENERICCAN_BASE_GENDATA_COMP_VERSION     0x0105u

#if (CANTRCV_30_GENERICCAN_GENTOOL_CFG5_BASE_COMP_VERSION != CANTRCV_30_GENERICCAN_BASE_GENDATA_COMP_VERSION)
# error "Module CanTrcv_30_GenericCan: Source and Generated Header files are inconsistent! [BASE]"
#endif


#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
# if defined(CANTRCV_30_GENERICCAN_USE_ENDCHECKWAKEUP)
# else
/* \trace SPEC-46905 */
#  if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
#    define CANTRCV_30_GENERICCAN_USE_ENDCHECKWAKEUP STD_ON
#  else
#    define CANTRCV_30_GENERICCAN_USE_ENDCHECKWAKEUP STD_OFF
#  endif
# endif
#endif


#if (DRVTRANS_GENERICCANDIOASR_VERSION                        != 0x0402u)
# error "Module CanTrcv_30_GenericCan: Source and Header files are inconsistent!"
#endif
#if (DRVTRANS_GENERICCANDIOASR_RELEASE_VERSION                != 0x00u)
# error "Module CanTrcv_30_GenericCan: Source and Header files are inconsistent!"
#endif


/* ** Version Check: CFG5 - HW specific part ** */
#if (CANTRCV_30_GENERICCAN_GENTOOL_CFG5_HW_COMP_VERSION != 0x0102u)
# error "Module CanTrcv_30_GenericCan: Source and Generated Header files are inconsistent!"
#endif






/* ********************************************************************************************************************
 *  LOCAL DEFINES
 * ********************************************************************************************************************/


#if !defined (CANTRCV_30_GENERICCAN_LOCAL)
# define CANTRCV_30_GENERICCAN_LOCAL             static
#endif

#if !defined (CANTRCV_30_GENERICCAN_LOCAL_INLINE)
# define CANTRCV_30_GENERICCAN_LOCAL_INLINE      LOCAL_INLINE
#endif

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
# if defined(CANTRCV_30_GENERICCAN_AUTO_REINIT)
# else
#  define CANTRCV_30_GENERICCAN_AUTO_REINIT STD_OFF
# endif
#endif


#define CanTrcvCfg_IsChannelUsed(ch)                   CanTrcv_30_GenericCan_IsChannelUsed(ch)
#define CanTrcvCfg_IsWakeupByBusUsed(ch)               CanTrcv_30_GenericCan_IsWakeupByBusUsed(ch)
#define CanTrcvCfg_GetWakeupSource(ch)                 CanTrcv_30_GenericCan_GetWakeupSource(ch)
#define CanTrcvCfg_GetWakeupSourceSyserr(ch)           CanTrcv_30_GenericCan_GetWuSrcSyserr(ch)
#define CanTrcvCfg_GetWakeupSourcePor(ch)              CanTrcv_30_GenericCan_GetWuSrcPor(ch)
#define CanTrcvCfg_IsIcuChannelSet(ch)                 CanTrcv_30_GenericCan_IsIcuChannelSetOfChannel(ch)
#define CanTrcvCfg_GetIcuChannel(ch)                   CanTrcv_30_GenericCan_GetIcuChannelOfChannel(ch)
#define CanTrcvCfg_GetMagicNumber()                    CanTrcv_30_GenericCan_GetFinalMagicNumber()
#define CanTrcvCfg_GetGeneratorCompatibilityVersion()  CanTrcv_30_GenericCan_GetGeneratorCompatibilityVersion()

#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
# define CanTrcvCfg_IsPnEnabled(ch)               CanTrcv_30_GenericCan_IsPnEnabled(ch)
#endif


#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) 

/* --- Flags to handle any kind of requests --- */

/* 
* Please note CanTrcv_30_GenericCan_Prob_Type.requestState is a 32bit value: |4.byte|3.byte|2.byte|1.byte|
* The bits/bytes are reserved/used as follows:
* LL: |4.byte|
* HL: |3.byte| - |1.byte|
*/
# define CanTrcv_30_GenericCan_Req_None                     0x00000000u   /* No request pending                     */

# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
/* ! Operation modes requests */
#  define CanTrcv_30_GenericCan_Req_OpModeMask              0x0000000Fu   /* Mask to query operation mode requests  */
#  define CanTrcv_30_GenericCan_Req_OpModeNormal            0x00000001u   /* Normal operation mode                  */
#  define CanTrcv_30_GenericCan_Req_OpModeStandby           0x00000002u   /* Standby mode                           */
#  define CanTrcv_30_GenericCan_Req_OpModeSleep             0x00000004u   /* Sleep mode                             */

# define CanTrcv_30_GenericCan_Req_CbByBusFlag              0x00000040u   /* Request to execute CheckWakeup         */
# endif /* SPI_COMMENT */

# if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
#  define CanTrcv_30_GenericCan_Req_OpModeNormalPn          0x00000008u   /* Special mode with Pn enabled in Normal */

#  define CanTrcv_30_GenericCan_Req_ClrWufFlag              0x00000010u   /* Request to execute ClearTrcvWufFlag    */
#  define CanTrcv_30_GenericCan_Req_ChkWakFlag              0x00000020u   /* Request to execute CheckWakeFlag       */

#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#   define CanTrcv_30_GenericCan_Req_ConfirmPnAvailability  0x00001000u   /* Request to confirm Pn avaibility       */

#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
#    if (CANTRCV_30_GENERICCAN_AUTO_REINIT == STD_ON)
#     define CanTrcv_30_GenericCan_Req_ReInitPn             0x00000100u   /* Request to re-init the PN-configuration in case of asynchronous SPI is configured */
#    endif
#    define CanTrcv_30_GenericCan_Req_SyncSpiTx             0x00010000u   /* Request to execute synchronous transmission in case of asynchronous SPI is configured */
#   endif

#  endif /* SPI_COMMENT */
# endif /* PN */


/* --- Macros to set/get/clear pending requests ---                                */

/* !!! DO NOT use Set* / Clr* in HW specific parts unless stated otherwise     !!! */
/* !!! Ensure data consistency when using Set* / Clr* by using interrupt locks !!! */

/* Check if ANY request is pending. Returns TRUE if at least one of the requests above is pending  */
# define CanTrcv_30_GenericCan_IsReqFlagPnd(index) (CanTrcv_30_GenericCan_Prob[index].requestState != CanTrcv_30_GenericCan_Req_None)

/* Generic macros. DO NOT use. Use the explicit macros instead. Behavior may change in future      */
# define CanTrcv_30_GenericCan_SetFlagReq(index, flag) CanTrcv_30_GenericCan_Prob[index].requestState   |= (flag)
# define CanTrcv_30_GenericCan_ClrFlagReq(index, flag) CanTrcv_30_GenericCan_Prob[index].requestState   &= ~((uint32)(flag))
# define CanTrcv_30_GenericCan_GetFlagReq(index, flag) ( CanTrcv_30_GenericCan_Prob[index].requestState &  (flag) )

# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
/* Set/Get/Clear the currently requested operation mode */
#  define CanTrcv_30_GenericCan_ClrOpModeReq(index) ( CanTrcv_30_GenericCan_Prob[index].requestState &=  ~((uint32)CanTrcv_30_GenericCan_Req_OpModeMask) )
#  define CanTrcv_30_GenericCan_GetOpModeReq(index) ( CanTrcv_30_GenericCan_Prob[index].requestState & CanTrcv_30_GenericCan_Req_OpModeMask)
#  define CanTrcv_30_GenericCan_SetOpModeReq(index, mode) \
   CanTrcv_30_GenericCan_Prob[index].requestState &=  (~((uint32)CanTrcv_30_GenericCan_Req_OpModeMask)); \
   CanTrcv_30_GenericCan_Prob[index].requestState |=  (mode)

/* Set/Get/Clear a request to process a call to CanTrcv_30_GenericCan_CheckWakeup */
#  define CanTrcv_30_GenericCan_SetCbByBusFlagReq(index) CanTrcv_30_GenericCan_SetFlagReq(index, CanTrcv_30_GenericCan_Req_CbByBusFlag)
#  define CanTrcv_30_GenericCan_ClrCbByBusFlagReq(index) CanTrcv_30_GenericCan_ClrFlagReq(index, CanTrcv_30_GenericCan_Req_CbByBusFlag)
#  define CanTrcv_30_GenericCan_GetCbByBusFlagReq(index) CanTrcv_30_GenericCan_GetFlagReq(index, CanTrcv_30_GenericCan_Req_CbByBusFlag)
# endif /* SPI_COMMENT */

# if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
/* Set/Get/Clear a request to process a call to CanTrcv_30_GenericCan_ClearTrcvWufFlag */
#  define CanTrcv_30_GenericCan_SetClrWufFlagReq(index) CanTrcv_30_GenericCan_SetFlagReq(index, CanTrcv_30_GenericCan_Req_ClrWufFlag)
#  define CanTrcv_30_GenericCan_ClrClrWufFlagReq(index) CanTrcv_30_GenericCan_ClrFlagReq(index, CanTrcv_30_GenericCan_Req_ClrWufFlag)
#  define CanTrcv_30_GenericCan_GetClrWufFlagReq(index) CanTrcv_30_GenericCan_GetFlagReq(index, CanTrcv_30_GenericCan_Req_ClrWufFlag)

/* Set/Get/Clear a request to process a call to CanTrcv_30_GenericCan_CheckWakeFlag */
#  define CanTrcv_30_GenericCan_SetChkWakFlagReq(index) CanTrcv_30_GenericCan_SetFlagReq(index, CanTrcv_30_GenericCan_Req_ChkWakFlag)
#  define CanTrcv_30_GenericCan_ClrChkWakFlagReq(index) CanTrcv_30_GenericCan_ClrFlagReq(index, CanTrcv_30_GenericCan_Req_ChkWakFlag)
#  define CanTrcv_30_GenericCan_GetChkWakFlagReq(index) CanTrcv_30_GenericCan_GetFlagReq(index, CanTrcv_30_GenericCan_Req_ChkWakFlag)

#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
/* Set/Get/Clear a request to call CanIf_CanTrcv_ConfirmPnAvaibility from CanTrcv_30_GenericCan_GetStatusFlagsIndication */
#   define CanTrcv_30_GenericCan_SetConfirmPnAvailabilityReq(index) CanTrcv_30_GenericCan_SetFlagReq(index, CanTrcv_30_GenericCan_Req_ConfirmPnAvailability)
#   define CanTrcv_30_GenericCan_ClrConfirmPnAvailabilityReq(index) CanTrcv_30_GenericCan_ClrFlagReq(index, CanTrcv_30_GenericCan_Req_ConfirmPnAvailability)
#   define CanTrcv_30_GenericCan_GetConfirmPnAvailabilityReq(index) CanTrcv_30_GenericCan_GetFlagReq(index, CanTrcv_30_GenericCan_Req_ConfirmPnAvailability)

#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
#    if (CANTRCV_30_GENERICCAN_AUTO_REINIT == STD_ON)
#     define CanTrcv_30_GenericCan_SetReInitPnReq(index) CanTrcv_30_GenericCan_SetFlagReq(index, CanTrcv_30_GenericCan_Req_ReInitPn)
#     define CanTrcv_30_GenericCan_ClrReInitPnReq(index) CanTrcv_30_GenericCan_ClrFlagReq(index, CanTrcv_30_GenericCan_Req_ReInitPn)
#     define CanTrcv_30_GenericCan_GetReInitPnReq(index) CanTrcv_30_GenericCan_GetFlagReq(index, CanTrcv_30_GenericCan_Req_ReInitPn)
#    endif
#    define CanTrcv_30_GenericCan_SetSyncSpiTxReq(index) CanTrcv_30_GenericCan_SetFlagReq(index, CanTrcv_30_GenericCan_Req_SyncSpiTx)
#    define CanTrcv_30_GenericCan_ClrSyncSpiTxReq(index) CanTrcv_30_GenericCan_ClrFlagReq(index, CanTrcv_30_GenericCan_Req_SyncSpiTx)
#    define CanTrcv_30_GenericCan_GetSyncSpiTxReq(index) CanTrcv_30_GenericCan_GetFlagReq(index, CanTrcv_30_GenericCan_Req_SyncSpiTx)
#   endif

#  endif /* SPI_COMMENT */
# endif /* PN */
#endif /* SPI_COMMENT / SBC_COMMENT */


/* ********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 * ********************************************************************************************************************/
#define CANTRCV_30_GENERICCAN_IS_INIT                         1u
#define CANTRCV_30_GENERICCAN_IS_NOT_INIT                     0u

/* ********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 * ********************************************************************************************************************/
#define CanTrcv_EnterCritical() SchM_Enter_CanTrcv_30_GenericCan_CANTRCV_30_GENERICCAN_EXCLUSIVE_AREA_0()
#define CanTrcv_LeaveCritical() SchM_Exit_CanTrcv_30_GenericCan_CANTRCV_30_GENERICCAN_EXCLUSIVE_AREA_0()

#if (CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON)
# if (CANTRCV_30_GENERICCAN_PROD_ERROR_DETECT == STD_ON)
#  define CanTrcv_DemReportError(eventId)        ((void)Dem_ReportErrorStatus((eventId), (DEM_EVENT_STATUS_FAILED)))
# endif
#endif


#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
# define CanTrcv_DetReportError(apiId, errorId)  ((void)Det_ReportError((CANTRCV_30_GENERICCAN_MODULE_ID), (CANTRCV_30_GENERICCAN_INSTANCE_ID), (apiId), (errorId)))
#else
# define CanTrcv_DetReportError(apiId, errorId)
#endif


#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
#  define CanTrcv_ReportErrorNoTrcvControl(apiId) CanTrcv_DetReportError( (apiId), CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL)
#endif


/* If no errors should be reported */
#if defined(CanTrcv_ReportErrorNoTrcvControl)
#else
# define CanTrcv_ReportErrorNoTrcvControl(apiId)
#endif



# define CANTRCV_30_GENERICCAN_SF_WUP_BUS  0x0001u
# define CANTRCV_30_GENERICCAN_SF_WUP_PIN  0x0002u
# define CANTRCV_30_GENERICCAN_SF_WUP_POR  0x0004u
# define CANTRCV_30_GENERICCAN_SF_ERR_BUS  0x0100u
# define CANTRCV_30_GENERICCAN_SF_ERR_SYS  0x0200u
# define CANTRCV_30_GENERICCAN_SF_ERR_CFG  0x0400u

# define CANTRCV_30_GENERICCAN_GET_SF_INIT (CANTRCV_30_GENERICCAN_SF_WUP_BUS|CANTRCV_30_GENERICCAN_SF_WUP_PIN|CANTRCV_30_GENERICCAN_SF_WUP_POR|CANTRCV_30_GENERICCAN_SF_ERR_SYS|CANTRCV_30_GENERICCAN_SF_ERR_CFG)
# define CANTRCV_30_GENERICCAN_GET_SF_SETOPMODE (CANTRCV_30_GENERICCAN_SF_ERR_SYS|CANTRCV_30_GENERICCAN_SF_ERR_CFG)
# define CANTRCV_30_GENERICCAN_GET_SF_CBWAKEUPBYBUS (CANTRCV_30_GENERICCAN_SF_WUP_BUS|CANTRCV_30_GENERICCAN_SF_WUP_PIN|CANTRCV_30_GENERICCAN_SF_ERR_SYS)
# define CANTRCV_30_GENERICCAN_GET_SF_MAINFUNCTIONDIAGNOSTICS (CANTRCV_30_GENERICCAN_SF_ERR_BUS)
# define CANTRCV_30_GENERICCAN_GET_SF_MAINFUNCTION (CANTRCV_30_GENERICCAN_SF_WUP_PIN|CANTRCV_30_GENERICCAN_SF_WUP_BUS)

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) || defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
/* The macro CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON returns true if the
   given wakeup reason indicates an external wakeup event */
# if !defined(CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON)
#  if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
#   define CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON(wuReason)               \
                        ((wuReason) == CANTRCV_30_GENERICCAN_WU_BY_BUS)    || \
                        ((wuReason) == CANTRCV_30_GENERICCAN_WU_BY_PIN)    || \
                        ((wuReason) == CANTRCV_30_GENERICCAN_WU_BY_SYSERR)

#  else
#   define CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON(wuReason)               \
                        ((wuReason) == CANTRCV_30_GENERICCAN_WU_BY_PIN)    || \
                        ((wuReason) == CANTRCV_30_GENERICCAN_WU_BY_BUS)
#  endif
# endif /* !defined(CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON) */
#endif /* (CANTRCV_30_GENERICCAN_INTERFACE_DIO) || defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) */

#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
# define CANTRCV_30_GENERICCAN_IS_INITIALIZED(channel) \
                  ( (CanTrcv_30_GenericCan_IsInitialized == CANTRCV_30_GENERICCAN_IS_INIT) \
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
                    && (CanTrcv_30_GenericCan_Prob[channel].isInit == CANTRCV_30_GENERICCAN_IS_INIT) \
# endif
                  )
#endif


/*  CanTrcv_30_GenericCan_State...
    -------------------------------
    
    These macros shall return true if the specified state of the underlying CAN Transceiver hardware is detected.
    
    Parameters:
      CanTrcv_30_GenericCan_StatusFlagsType statusFlags - Contains the current statusFlags
      
    Return:
      Return true if the state is detected in statusFlags.
      
    The macros shall check the following:
   
    CanTrcv_30_GenericCan_State...
    ...WupIsBus                       Check if a wakeup by bus is detected (pattern OR frame)
    ...WupIsPin                       Check if a wakeup by pin is detected
    ...WupIsPor                       Check if a wakeup by power-on-reset is detected
    ...WupIsValid                     Check if a valid wakeup is detected (usually WUP+PIN)
    
    ...ErrIsBus                       Check if a bus error (e.g. dominant state) is detected
    ...ErrIsSys                       Check if SYSERR is detected
    ...ErrIsCfg                       Check if a configuration error occurred
    
    If the underlying transceiver hardware does not support one of these flags the corresponding
    macro shall always return false.
*/
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
/*
    Example:
      When using DIO interface:
      =========================
        #define CanTrcv_30_GenericCan_StateWupIsBus(statusFlags)  ( (statusFlags.PinSTB == STD_LOW) && (statusFlags.PinERR == STD_LOW) )
*/
#endif
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
/*    When using SPI interface:
      =========================
       #define CanTrcv_30_GenericCan_StateWupIsBus(statusFlags)  ( (statusFlags.DIAG & CANTRCV_30_GENERICCAN_WUP_BUS ) != 0x0 )
*/
#endif

# define CanTrcv_30_GenericCan_StateWupIsBus(statusFlags)   ( <Your_Trcv_Code> )
# define CanTrcv_30_GenericCan_StateWupIsPin(statusFlags)   ( <Your_Trcv_Code> )
# define CanTrcv_30_GenericCan_StateWupIsPor(statusFlags)   ( <Your_Trcv_Code> )
# define CanTrcv_30_GenericCan_StateWupIsValid(statusFlags) ( CanTrcv_30_GenericCan_StateWupIsBus(statusFlags) || \
                                                               CanTrcv_30_GenericCan_StateWupIsPin(statusFlags) )


# define CanTrcv_30_GenericCan_StateErrIsBus(statusFlags)   ( <Your_Trcv_Code> )
# define CanTrcv_30_GenericCan_StateErrIsSys(statusFlags)   ( <Your_Trcv_Code> )
# define CanTrcv_30_GenericCan_StateErrIsCfg(statusFlags)   ( <Your_Trcv_Code> )


#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON)
/*  CANTRCV_30_GENERICCAN_CHECK_BAUDRATE(index)
    --------------------------------------------
    
    This macro shall return true if the configured baudrate is correct for the underlying
    transceiver hardware on channel >>index<<. 
    
    Parameters:
      uint8 index     Index of the transceiver hardware to be checked
      
    Return:
      Return true if the baudrate is correct
      
    Example:
      #define CANTRCV_30_GENERICCAN_CHECK_BAUDRATE(index)    ( (CanTrcv_30_GenericCan_GetBaudrateOfPnCfg(index) == 0x1) )
*/
# define CANTRCV_30_GENERICCAN_CHECK_BAUDRATE(index)       ( <Your_Trcv_Code> )
#endif /* CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON */


/* ********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 * ********************************************************************************************************************/


/*  CanTrcv_30_GenericCan_StatusFlagsType
    --------------------------------------
    
    Specify the type of the statusFlags. This type is used for CanTrcv_30_GenericCan_Prob[index].statusFlags.
    The type specified will be used with the "CanTrcv_30_GenericCan_State.." macros to evalute the current
    state of the underlying transceiver hardware.
       
    Example:
      typedef struct {
        uint8 DIAG1;
        uint8 DIAG2;
      } CanTrcv_30_GenericCan_StatusFlagsType;
      
      OR
      
      typedef uint8 CanTrcv_30_GenericCan_StatusFlagsType;
*/    
typedef <Your_Trcv_Code> CanTrcv_30_GenericCan_StatusFlagsType;


/* ********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 * ********************************************************************************************************************/

#define CANTRCV_30_GENERICCAN_START_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
 /*
 * Specifies whether the CanTrcv module is initialized.
 */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
CANTRCV_30_GENERICCAN_LOCAL VAR(uint8, CANTRCV_30_GENERICCAN_VAR_INIT) CanTrcv_30_GenericCan_IsInitialized = CANTRCV_30_GENERICCAN_IS_NOT_INIT;
# endif

#define CANTRCV_30_GENERICCAN_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */



#define CANTRCV_30_GENERICCAN_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*
 * ! In following variable CAN transceiver channel specific settings are stored e.g. the wake-up reason or the current operating mode. 
 * For more information see the description of type: CanTrcv_30_GenericCan_Prob_Type.
 */
VAR(CanTrcv_30_GenericCan_Prob_Type, CANTRCV_30_GENERICCAN_VAR_NOINIT) CanTrcv_30_GenericCan_Prob[CANTRCV_30_GENERICCAN_MAX_CHANNEL]; /* PRQA S 3408 */ /* MD_CanTrcv_30_GenericCan_3408 */

/*  LocalData 
    ---------
    Place additional local (static) data here
*/
/* 
  <Your_Trcv_Code>
*/

#if (CANTRCV_30_GENERICCAN_USE_INIT_POINTER == STD_ON)
/* ! In following variable the configuration is stored during the initialization of the CAN transceiver driver in case of a Post-build loadable / selectable configuration. */
CANTRCV_30_GENERICCAN_LOCAL P2CONST(CanTrcv_30_GenericCan_ConfigType, CANTRCV_30_GENERICCAN_VAR_NOINIT, CANTRCV_30_GENERICCAN_INIT_DATA) CanTrcv_30_GenericCan_ConfigDataPtr;
#endif

#define CANTRCV_30_GENERICCAN_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define CANTRCV_30_GENERICCAN_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* ********************************************************************************************************************
 *  LOCAL FUNCTIONS
 * ********************************************************************************************************************/
/* Local function declarations */
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) 

# if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && (CANTRCV_30_GENERICCAN_VERIFY_DATA == STD_ON)
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_VerifyData
 **********************************************************************************************************************/
/*! \brief       Verifies if all PN-specific settings were written correctly to CAN transceiver hardware during the initialization.
 *  \details     This function has no service id. Called by transceiver driver within: CanTrcv_30_GenericCan_Init().
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \return      E_OK: Read PN-specific settings from CAN transceiver hardware correspond to written ones.
 *               E_NOT_OK: Read PN-specific settings from CAN transceiver hardware differ from written ones.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) and (CANTRCV_30_GENERICCAN_VERIFY_DATA == STD_ON)
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_VerifyData(uint8 CanTrcvIndex);
#  endif /* SPI_COMMENT */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CheckWakeFlagIndication
 **********************************************************************************************************************/
/*! \brief       Indication to be called by SeqEndNotification of the SPI-driver.
 *  \details     This function has no service id. Called by transceiver driver within: CanTrcv_30_GenericCan_CheckWakeFlag().
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver to which API call has to be applied.
 *  \pre         The LL must ensure that this API is called with valid <CanTrcvIndex>. 
 *               The value of <CanTrcvIndex> is valid if < CANTRCV_30_GENERICCAN_MAX_CHANNEL.
 *  \context     TASK|ISR
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CheckWakeFlagIndication(uint8 CanTrcvIndex);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ClearTrcvWufFlagIndication
 **********************************************************************************************************************/
/*! \brief       Indication to be called by SeqEndNotification of the SPI-driver.
 *  \details     This function has no service id. Called by transceiver driver within: LL-specific.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver to which API call has to be applied.
 *  \pre         The LL must ensure that this API is called with valid <CanTrcvIndex>. 
 *               The value of <CanTrcvIndex> is valid if < CANTRCV_30_GENERICCAN_MAX_CHANNEL.
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ClearTrcvWufFlagIndication(uint8 CanTrcvIndex);
# endif /* PN */


# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#  if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON)
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CbWakeupByBusIndication
 **********************************************************************************************************************/
/*! \brief       Has to be called if the request to CbWakeupByBus has completed.
 *  \details     This function has no service id. Called by transceiver driver within: CanTrcv_30_GenericCan_CheckWakeup() and LL-specific.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \pre         The LL must ensure that this API is called with valid <CanTrcvIndex>. 
 *               The value of <CanTrcvIndex> is valid if < CANTRCV_30_GENERICCAN_MAX_CHANNEL.
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CbWakeupByBusIndication(uint8 CanTrcvIndex);
#  endif


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetStatusFlagsIndication
 **********************************************************************************************************************/
/*! \brief       Handles the storage and usage of status flags.
 *  \details     This function has no service id. Called by SPI driver / transceiver driver within: LL-specific.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \pre         The LL must ensure that this API is called with valid <CanTrcvIndex>. 
 *               The value of <CanTrcvIndex> is valid if < CANTRCV_30_GENERICCAN_MAX_CHANNEL.
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetStatusFlagsIndication(uint8 CanTrcvIndex);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_TrcvModeIndication
 **********************************************************************************************************************/
/*! \brief       Indication to be called by SeqEndNotification of the SPI-driver.
 *  \details     Updates the internal mode state variable of transceiver channel (CanTrcvIndex) and notifies the CanIf.
 *               This function has no service id. Called by transceiver driver within: CanTrcv_30_GenericCan_SetOpMode() and LL-specific.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[in]   modeSet: TRUE The requested mode was set, FALSE the requested mode was NOT set.
 *  \pre         The LL must ensure that this API is called with valid <CanTrcvIndex>. 
 *               The value of <CanTrcvIndex> is valid if < CANTRCV_30_GENERICCAN_MAX_CHANNEL.
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_TrcvModeIndication(uint8 CanTrcvIndex, boolean modeSet);


#  if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
#   if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetSyncSpiTxSem
 **********************************************************************************************************************/
/*! \brief       Locks the SPI for synchronous transmission in case of asynchronous SPI is configured.
 *  \details     This function has no service id. In case of the API returns TRUE the SPI interface for synchronous transmission 
 *               can be used otherwise NOT. After usage of SPI for synchronous transmission the SPI must be released by usage 
 *               of API CanTrcv_30_GenericCan_ReleaseSyncSpiTxSem().
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \return      TRUE: Locking of SPI for synchronous transmission succeeded.
 *               FALSE: Locking of SPI for synchronous transmission failed.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) && (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON)
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(boolean, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetSyncSpiTxSem(uint8 CanTrcvIndex);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ReleaseSyncSpiTxSem
 **********************************************************************************************************************/
/*! \brief       To be used to release the lock of SPI for synchronous transmission in case of asynchronous 
 *               SPI is configured.
 *  \details     This function has no service id.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) && (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON)
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ReleaseSyncSpiTxSem(uint8 CanTrcvIndex);
#   endif /* CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ProcessNextOpModeReq
 **********************************************************************************************************************/
/*! \brief       Verifies if another mode change was requested after the last one.
 *  \details     This function has no service id.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \return      E_OK: Another mode change was requested after the last one. Process the new one.
 *               E_NOT_OK: No other mode change was requested after the last one.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ProcessNextOpModeReq(uint8 CanTrcvIndex);
#  endif /* CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF */
# endif /* SPI_COMMENT */
#endif /* SPI_COMMENT / SBC_COMMENT */

#if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) || \
    (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON)
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ReportWakeup
 **********************************************************************************************************************/
/*! \brief       Reports the wake-up reason to the upper layer.
 *  \details     This function has no service id. Called by transceiver driver within: LL-specific.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \pre         The LL must ensure that this API is called with valid <CanTrcvIndex>. 
 *               The value of <CanTrcvIndex> is valid if < CANTRCV_30_GENERICCAN_MAX_CHANNEL.
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) || (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON)
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ReportWakeup(uint8 CanTrcvIndex);
#endif

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_LL_InitMemoryChannelBegin
 **********************************************************************************************************************/
/*! \brief       LL: Initializes the LL-specific channel data of the CAN transceiver hardware.
 *  \details     This function is called for ALL channels and has no service id.
 *               The callee (HL) ensures the validity of <CanTrcvIndex>. 
 *               Called by transceiver driver within: CanTrcv_30_GenericCan_Init().
 *               This is ensured if <CanTrcvIndex> < CANTRCV_30_GENERICCAN_MAX_CHANNEL.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE only for different transceiver channels (index)
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_LL_InitMemoryChannelBegin(uint8 CanTrcvIndex);

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_LL_InitChannel
 **********************************************************************************************************************/
/*! \brief       LL: Initializes the LL-specific channel data of the CAN transceiver hardware.
 *  \details     This function is called only for "used" (== activated) channels and has no service id.
 *               The callee (HL) ensures the validity of <CanTrcvIndex>. 
 *               This is ensured if <CanTrcvIndex> < CANTRCV_30_GENERICCAN_MAX_CHANNEL.
 *               Called by transceiver driver within: CanTrcv_30_GenericCan_Init().
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE only for different transceiver channels (index)
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_LL_InitChannel(uint8 CanTrcvIndex);

#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_InitSubInitPn
 **********************************************************************************************************************/
/*! \brief       Initializes the PN-data of the CAN transceiver hardware.
 *  \details     This function is called only for "used" (== activated) channels and has no service id.
 *               The callee (HL) ensures the validity of <CanTrcvIndex>. 
 *               This is ensured if <CanTrcvIndex> < CANTRCV_30_GENERICCAN_MAX_CHANNEL.
 *               Called by transceiver driver within: CanTrcv_30_GenericCan_Init().
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[in]   getStatusFlagRetVal: Status of determination of CAN transceiver HW status flags.
 *  \param[in]   pnEnabled: Status of partial networking within the configuration. TRUE: enabled, FALSE: disabled.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE only for different transceiver channels (index)
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_InitSubInitPn(uint8 CanTrcvIndex, Std_ReturnType getStatusFlagRetVal, boolean pnEnabled);

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CheckPnAvailability
 **********************************************************************************************************************/
/*! \brief       Checks whether PN-functionality is available by HW or not and adapts the internal state 
*                variable accordingly.
 *  \details     The callee (HL) ensures the validity of <CanTrcvIndex>. 
 *               This is ensured if <CanTrcvIndex> < CANTRCV_30_GENERICCAN_MAX_CHANNEL.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (index)
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CheckPnAvailability(uint8 CanTrcvIndex);
#endif /* CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON */




/*  LocalFunctionDecl 
    -----------------
    Add declarations of local (static) functions here.
*/
/* 
  <Your_Trcv_Code>
*/


/* ********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 * ********************************************************************************************************************/

/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_InitMemory
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_InitMemory(void)
{
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #100 Mark driver as uninitialized. */
  CanTrcv_30_GenericCan_IsInitialized = CANTRCV_30_GENERICCAN_IS_NOT_INIT;
#endif
}


/**********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_LL_InitMemoryChannelBegin
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_LL_InitMemoryChannelBegin(uint8 CanTrcvIndex)
{
  /* #100 Initialize the CAN transceiver channel-specific settings (independent active or inactive one) (LL-specific). */
  /*  InitMemory_Channel_Begin 
      ------------------------
      Init additional channel-specific memory here
  */
  /* 
    <Your_Trcv_Code>
  */ 
}


/**********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_LL_InitChannel
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_LL_InitChannel(uint8 CanTrcvIndex)
{
  /*  Init_Channel_Begin 
      ------------------
      Add declaration of local variables here.
      
      Restrictions:
      - DO NOT access the hardware
      
      Example:
        uint8 localVariable1;
  */
  /* 
    <Your_Trcv_Code>
  */ 

  /* #100 Initialize the CAN transceiver channel-specific settings (only active one) (LL-specific). */
  /*  Init_Channel
      ------------
      Add some channel specific initialization here.
      
      Restrictions:
      - DO NOT modify the operation mode
      - DO NOT initialize ID/MASK/DATA/DLC/CODE (for CanTrcv with selective wakeup support)
      - DO NOT use any asynchronous DIO/SPI API to access the hardware
  */
  /* 
    <Your_Trcv_Code>
  */ 
}


#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
/**********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_LL_InitChannelPnData
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_InitSubInitPn(uint8 CanTrcvIndex, Std_ReturnType getStatusFlagRetVal, boolean pnEnabled)
{
  /* #100 Initialize the PN-configuration only if PN is enabled within the configuration and determination of status flags from CAN transceiver HW succeeded. */
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
  if ((pnEnabled == TRUE) && (getStatusFlagRetVal == E_OK) && (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlagsRdy == TRUE))
#  else
  if ((pnEnabled == TRUE) && (getStatusFlagRetVal == E_OK))
#  endif
  {
    /* #110 Determine and store (overwrite the wake-up reason) whether POR or SYSERR is set. If one of them is set the PN configuration must be re-loaded. */
    if (
        (CanTrcv_30_GenericCan_StateWupIsPor(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)) ||
        (CanTrcv_30_GenericCan_StateErrIsSys(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)) ||
        (CanTrcv_30_GenericCan_StateErrIsCfg(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)) )  /* ASR extension: CFGERR has to be checked too! */ /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_POWER_ON */
    {
      Std_ReturnType retVal = E_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

      /* NOTE: Do NOT change the order between POWER_ON and SYSERR detection. Reason in case of POWER_ON is set SYSERR is set as well -> but in this case POWER_ON 
       * is the relevant one [see: ESCAN00093075]. */
      if (CanTrcv_30_GenericCan_StateWupIsPor(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_POWER_ON */
      {
        /* Store / overwrite the the wake-up reason: POWER_ON */
        CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_POWER_ON; /* \trace SPEC-14972 */ /* SBSW_CANTRCV_HL_GENERICCAN_1 */
      }
      else
      if (CanTrcv_30_GenericCan_StateErrIsSys(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_POWER_ON */
      {
        /* Store / overwrite the wake-up reason: BY_SYSERR */
        CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_SYSERR; /* \trace SPEC-15005 */ /* SBSW_CANTRCV_HL_GENERICCAN_1 */
      }
      else
      {
        /* Nothing. */
      }

      {
        /* \trace SPEC-15068 */
        /*  Init_Channel_PnData
            -------------------
            Fill only if CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON. Only SPI interface is supported here.
            
            Write ID/MASK/DATA/DLC/CODE to the underlying CanTrcv hardware.
            Store E_OK to retVal if data has been successfully initialized.
            
            Restrictions:
            - DO NOT modify the operation mode
            - DO NOT use any asynchronous DIO/SPI API to access the hardware
            
            Example:
              CanTrcv_SpiCmdBuffer[CanTrcvIndex] = CANTRCV_30_GENERICCAN_WR_MASK_ID;
            
              CanTrcv_SpiOutBuffer[CanTrcvIndex][0] = (uint8)(CanTrcv_30_GenericCan_GetCanIdMaskOfPnCfg(CanTrcvIndex)  >> 32);
              CanTrcv_SpiOutBuffer[CanTrcvIndex][1] = (uint8)(CanTrcv_30_GenericCan_GetCanIdMaskOfPnCfg(CanTrcvIndex)  >> 16);
              CanTrcv_SpiOutBuffer[CanTrcvIndex][2] = (uint8)(CanTrcv_30_GenericCan_GetCanIdMaskOfPnCfg(CanTrcvIndex)  >>  8);
              CanTrcv_SpiOutBuffer[CanTrcvIndex][3] = (uint8)(CanTrcv_30_GenericCan_GetCanIdMaskOfPnCfg(CanTrcvIndex)  >>  0);
              
              CanTrcv_SpiOutBuffer[CanTrcvIndex][4] = (uint8)(CanTrcv_30_GenericCan_GetCanIdOfPnCfg(CanTrcvIndex) >> 32);
              CanTrcv_SpiOutBuffer[CanTrcvIndex][5] = (uint8)(CanTrcv_30_GenericCan_GetCanIdOfPnCfg(CanTrcvIndex) >> 16);
              CanTrcv_SpiOutBuffer[CanTrcvIndex][6] = (uint8)(CanTrcv_30_GenericCan_GetCanIdOfPnCfg(CanTrcvIndex) >>  8);
              CanTrcv_SpiOutBuffer[CanTrcvIndex][7] = (uint8)(CanTrcv_30_GenericCan_GetCanIdOfPnCfg(CanTrcvIndex) >>  0);
              
              retVal = Spi_SetupEB(
                CanTrcv_30_GenericCan_SpiChannels[CanTrcvIndex].DATA,
                &(CanTrcv_SpiOutBuffer[CanTrcvIndex][0]),
                NULL_PTR
              );
              
              if (retVal == E_OK)
              {
                retVal = Spi_SyncTransmit(CanTrcv_30_GenericCan_SpiSequences[CanTrcvIndex]);
              }
        */
        <Your_Trcv_Code>
      }

      /* #120 Verify if channel specific initialization of PN was successful. If fails report error and disable PN functionality in SW. */
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) 
# if (CANTRCV_30_GENERICCAN_VERIFY_DATA == STD_ON)
      if (retVal == E_OK)
      {
        retVal = CanTrcv_30_GenericCan_VerifyData((uint8)CanTrcvIndex);
      }
# endif
#endif
      if (retVal != E_OK)
      {
        /* \trace SPEC-20013 */
        CanTrcv_ReportErrorNoTrcvControl(CANTRCV_30_GENERICCAN_INIT_ID);

        CanTrcv_30_GenericCan_Prob[CanTrcvIndex].pnAvailable = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
      }

      /* Dummy statement to prevent compiler warnings */
      CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(retVal); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */

    } /* porFlag == CANTRCV_30_GENERICCAN_FLAG_SET || sysErrFlag == CANTRCV_30_GENERICCAN_FLAG_SET */
  }
  else /* pnEnabled == TRUE  */
  {
    /* if pn is disabled, data is considered as always valid */
  }
}


/**********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CheckPnAvailability
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CheckPnAvailability(uint8 CanTrcvIndex)
{
  if ((CanTrcv_30_GenericCan_Prob[CanTrcvIndex].isPnEnabled == TRUE) &&
      (!(CanTrcv_30_GenericCan_StateErrIsSys(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags))) &&
      (!(CanTrcv_30_GenericCan_StateErrIsCfg(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)))) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_HW_SPECIFIC */
  {
    /* #100 If PN functionality is available in HW: Inform the CanIf about this and set internally that PN is available in HW. */

    /* \trace SPEC-15087, SPEC-19956 */
    CanIf_30_GenericCan_ConfirmPnAvailability(CanTrcvIndex);

    /* Report that PN is available again */
    CanTrcv_30_GenericCan_Prob[CanTrcvIndex].pnAvailable = TRUE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
  }
  else
  {
    /* ESCAN00083583 */
    if ((CanTrcv_30_GenericCan_StateErrIsSys(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)) ||
        (CanTrcv_30_GenericCan_StateErrIsCfg(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags))) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_HW_SPECIFIC */
    {
      /* PN does NOT work due to CFGERR or SYSERR occurred */

      /* #110 If PN-functionality is NOT available in HW: set internally that PN is NOT available in HW. */
      CanTrcv_30_GenericCan_Prob[CanTrcvIndex].pnAvailable = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
    }
  }
}
#endif /* CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON */


/**********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_Init
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_Init( P2CONST(CanTrcv_30_GenericCan_ConfigType, AUTOMATIC, CANTRCV_30_GENERICCAN_INIT_DATA) ConfigPtr )
{
  /* \trace SPEC-14971 */

  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* ESCAN00076622 */
#if (CANTRCV_30_GENERICCAN_USE_INIT_POINTER == STD_ON)
  /* #100 Store the configuration pointer. (Required before any check especially for PBL-configurations due to access to GeneratorCompatibilityVersion and MagicNumber via the ConfigPtr.) */
  CanTrcv_30_GenericCan_ConfigDataPtr = ConfigPtr;

# if ((CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON) || (CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT == CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)) /* COV_CANTRCV_HL_LL_GENERICCAN_VARCOV_SW_FEAT_NOT_SUPPORTED */
  /* #10 Check if parameter ConfigPtr is valid. */
  if (ConfigPtr == NULL_PTR)
  {
#  if (CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT == CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE) /* COV_CANTRCV_HL_LL_GENERICCAN_VARCOV_SW_FEAT_NOT_SUPPORTED */
    EcuM_BswErrorHook((uint16) CANTRCV_30_GENERICCAN_MODULE_ID, (uint8) ECUM_BSWERROR_NULLPTR);
#  endif /* (CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT == CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE) */

#  if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON) /* COV_CANTRCV_HL_LL_GENERICCAN_VARCOV_SW_FEAT_NOT_SUPPORTED */
    /* \trace SPEC-14948 */
    errorId = CANTRCV_30_GENERICCAN_E_PARAM_POINTER;
#  endif /* (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON) */
  }
  else
# endif /* (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON) || (CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT == CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE) */
# if (CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT == CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE) /* COV_CANTRCV_HL_LL_GENERICCAN_VARCOV_SW_FEAT_NOT_SUPPORTED */
  /* #20 Check the generator compatibility version */
  if (CanTrcvCfg_GetGeneratorCompatibilityVersion() != (uint32)((CANTRCV_30_GENERICCAN_BASE_GENDATA_COMP_VERSION << 16u) | CANTRCV_30_GENERICCAN_HW_GENDATA_COMP_VERSION)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_INVALID_GENDATA */
  {
    EcuM_BswErrorHook((uint16) CANTRCV_30_GENERICCAN_MODULE_ID, (uint8) ECUM_BSWERROR_COMPATIBILITYVERSION);
  }
  /* #30 Check the magic number */
  else if (CanTrcvCfg_GetMagicNumber() != CANTRCV_30_GENERICCAN_FINAL_MAGIC_NUMBER) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_INVALID_GENDATA */
  {
    EcuM_BswErrorHook((uint16) CANTRCV_30_GENERICCAN_MODULE_ID, (uint8) ECUM_BSWERROR_MAGICNUMBER);
  }
  else
# endif /* (CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT == CANTRCV_30_GENERICCAN_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE) */
#endif /* (CANTRCV_30_GENERICCAN_USE_INIT_POINTER == STD_ON) */
  {
    uint8_least index;



    /*  Init_Begin 
        ----------------
        
        Global initialization tasks, that are not specific for a channel. You may add additional local variables here.
    */
    /* 
      <Your_Trcv_Code>
    */ 


#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
    /* #110 Set transceiver driver as initialized at all. */
    CanTrcv_30_GenericCan_IsInitialized = CANTRCV_30_GENERICCAN_IS_INIT;
#endif

    /* #120 Run through all transceiver channels and initialize them.*/
    for(index = 0; index < CANTRCV_30_GENERICCAN_MAX_CHANNEL; ++index)
    {

      /* Initialize variables */

      /* Initialize LL */
      CanTrcv_30_GenericCan_LL_InitMemoryChannelBegin((uint8)index);

      /* #130 Set the channel states concerning: initialization, wake-up report and wake-up reason to default values. */
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
      CanTrcv_30_GenericCan_Prob[index].isInit = CANTRCV_30_GENERICCAN_IS_NOT_INIT; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#endif
      CanTrcv_30_GenericCan_Prob[index].wakeUpReport = CANTRCV_30_GENERICCAN_WU_ENABLE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
      CanTrcv_30_GenericCan_Prob[index].wakeUpReason = CANTRCV_30_GENERICCAN_WU_ERROR; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

      /* #140 Set the channel state concerning: request state to default values (only SPI-interface or SBC-interface with PN). */
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) 
      CanTrcv_30_GenericCan_Prob[index].requestState = CanTrcv_30_GenericCan_Req_None; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#endif

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
      /* #150 Set the channel state concerning: current operating mode to default values (only SPI-interface). */
      CanTrcv_30_GenericCan_Prob[index].curOpMode = CANTRCV_30_GENERICCAN_OP_MODE_NORMAL; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
# if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 

#  if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON)
      /* #160 Set the channel states concerning: wake-up during state transition detection, status flags to default values (only asynchronous SPI-interface). */
      CanTrcv_30_GenericCan_Prob[index].wakeUpInTransitionDetected = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#  endif

      CanTrcv_30_GenericCan_Prob[index].statusFlagsRdy = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
      CanTrcv_30_GenericCan_Prob[index].statusFlagsNew = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

      CanTrcv_30_GenericCan_Prob[index].nextRequestState = CanTrcv_30_GenericCan_Req_None; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
# endif
#endif

#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
      /* #170 Set the channel states concerning: Partial networking to default values (only PN). */
      CanTrcv_30_GenericCan_Prob[index].pnAvailable = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
      CanTrcv_30_GenericCan_Prob[index].isPnEnabled = FALSE; /* is configured in _Init() */ /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#endif


#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && \
    (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) && (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON) 
      /* #180 Check the configured baudrate (only PN). */
      if (CANTRCV_30_GENERICCAN_CHECK_BAUDRATE(index) != E_OK) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_INVALID_GENDATA */
      {
        /* \trace SPEC-15101 */
        errorId = CANTRCV_30_GENERICCAN_E_BAUDRATE_NOT_SUPPORTED;
      }
      else
#endif
      /* ----- Implementation ----------------------------------------------- */

      /* #190 Initialize next parameters only for active transceiver channels. */
      if(CanTrcvCfg_IsChannelUsed(index))
      {
#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
        uint8 pnEnabled;
#endif

# if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) || \
    ( (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) ) 
        Std_ReturnType getStatusFlagRetVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
# endif

#if defined(CANTRCV_30_GENERICCAN_INTERFACE_SPI) 
        Std_ReturnType modeSet = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
#endif

        /* #200 If no wake-up event occurred during initialization the default reason is RESET. */
        CanTrcv_30_GenericCan_Prob[index].wakeUpReason = CANTRCV_30_GENERICCAN_WU_RESET; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
        pnEnabled = CanTrcvCfg_IsPnEnabled(index); /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */

        /* #210 Set the channel states concerning: partial networking to configured values (only PN). */
        CanTrcv_30_GenericCan_Prob[index].pnAvailable = pnEnabled; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
        CanTrcv_30_GenericCan_Prob[index].isPnEnabled = pnEnabled; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#endif

        /* #220 Do additional checks: wake-up event, HW status, PN configuration. (critical section is used). */
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
        CanTrcv_EnterCritical();
#endif /* DIO / SBC */

#if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) || \
    ( (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) ) 
        /* \trace SPEC-14953 */


          /* #230 Check general status flags. If fails report error. */
        /*  GetStatusFlags
            --------------
            Request all status flags (SYSERR, POR, WUP, WUF, LWUP, ...) from the underlying transceiver hardware. 
            Set getStatusFlagRetVal = E_OK if the request was successful. The flags are evaluated later with the CanTrcv_30_GenericCan_State*
            macros.
            
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*  When using DIO interface:
            =========================
              Store the current level of the DIO pins to statusFlags. 
              
              Example TJA1041:
                statusFlags.PinERR = Dio_ReadChannel(CanTrcv_30_GenericCan_GetERROfDioConfiguration(index));
                statusFlags.PinSTB = Dio_ReadChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(index));
                getStatusFlagRetVal = E_OK;
        */
        #endif
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        /*  When using a SPI interface:
            ===========================
              This MUST be done synchronously.
            
              Restrictions:
                - If the driver is not initialized yet the access MUST be done synchronously (e.g. in context
                  of CanTrcv_30_GenericCan_Init())
                - When using asynchronous access the request to read status flags may be interrupted
                  by any other API / request. The driver MUST handle this.
                - DO NOT loose any request
                - ALWAYS call CanTrcv_30_GenericCan_GetStatusFlagsIndication as soon as status flags
                  have been read. 
                - If the same flags are polled in MainFunction ensure that the polled flags are
                  processed even if a new request (e.g. from interrupt context) is pending
                - ALWAYS call the pending indications for an active request
                    - GetCbByBusFlagReq == TRUE -> CanTrcv_30_GenericCan_CbWakeupByBusIndication
            
              Example:
                // This function will try to request the status flags. If the driver is currently busy with e.g. a mode change
                // the request will be held pending and processed as soon as the driver is ready again.
            
                // As soon as the flags are ready, the function GetStatusFlagsIndication is beeing called.
                getStatusFlagRetVal = CanTrcv_30_GenericCan_RequestStatusFlags(index, TRUE);
        */
        #endif
        <Your_Trcv_Code>
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
        if ((getStatusFlagRetVal == E_NOT_OK) || (CanTrcv_30_GenericCan_Prob[index].statusFlagsRdy == FALSE))
#  else
        if ((getStatusFlagRetVal == E_NOT_OK))
#  endif
        {
          /* \trace SPEC-20013 */
          CanTrcv_ReportErrorNoTrcvControl(CANTRCV_30_GENERICCAN_INIT_ID);
        }
#endif /* CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON || CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON */

        /* #240 Determine whether a wake-up event (BY_BUS, BY_PIN) occurred or NOT. If occurred store it. */
#if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON)
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
        if ((getStatusFlagRetVal == E_OK) && (CanTrcv_30_GenericCan_Prob[index].statusFlagsRdy == TRUE))
#  else
        if ((getStatusFlagRetVal == E_OK))
#  endif
        {
          if (CanTrcv_30_GenericCan_StateWupIsBus(CanTrcv_30_GenericCan_Prob[index].statusFlags))
          {
            /* A wakeup occurred. The reason is: BY_BUS */
            CanTrcv_30_GenericCan_Prob[index].wakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_BUS; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
          }
          else if (CanTrcv_30_GenericCan_StateWupIsPin(CanTrcv_30_GenericCan_Prob[index].statusFlags)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_BY_PIN */
          {
            /* A wakeup occurred. The reason is: BY_PIN */
            CanTrcv_30_GenericCan_Prob[index].wakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_PIN; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
          }
          else
          {
            /* Nothing. */
          }
        }
#endif /* CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON */

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
        CanTrcv_LeaveCritical();
#endif /* DIO / SBC */

        /* Do some generic initialization of LL */
        CanTrcv_30_GenericCan_LL_InitChannel((uint8)index);

#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
        /* #250 Initialize the CAN transceiver HW according partial networking configuration. */
        CanTrcv_30_GenericCan_InitSubInitPn((uint8)index, getStatusFlagRetVal, pnEnabled);
#endif /* CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON */

#if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) || \
    (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON)
        /* #260 Report the detected wake-up event to the upper layer. */
        CanTrcv_30_GenericCan_ReportWakeup((uint8)index);
#endif

        /* #270 Set the operating mode to NORMAL. In case of SPI-interface always synchronous. (critical section used) */
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
        CanTrcv_EnterCritical();
#endif /* DIO / SBC */

#if (CANTRCV_30_GENERICCAN_USE_ICU == STD_ON)
        /* #280 If ICU is configured, disable ICU. */
        if (CanTrcvCfg_IsIcuChannelSet(index)) /* \trace SPEC-14950 */
        {
          Icu_DisableNotification( CanTrcvCfg_GetIcuChannel(index) ); /* \trace SPEC-14973*/
        }
#endif

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        /* This is always synchronous */
        CanTrcv_30_GenericCan_SetOpModeReq(index, CanTrcv_30_GenericCan_Req_OpModeNormal); /* SBSW_CANTRCV_HL_GENERICCAN_1 */ /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#endif
        /*  SetOpModeNormal
            ---------------
            
            Set the underlying transceiver hardware to normal operation mode by changing
            the Dio Levels.
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*  When using DIO interface:
            ========================
              Example for TJA1041:
                Dio_WriteChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(index),STD_HIGH);
                Dio_WriteChannel(CanTrcv_30_GenericCan_GetENOfDioConfiguration(index),STD_HIGH);
                modeSet = E_OK;
        */
        #endif
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        /*  When using SPI interface:
            =========================
              While in context of CanTrcv_30_GenericCan_Init() this mode change MUST be done synchronously.
              
              When in context of CanTrcv_30_GenericCan_SetOpMode() this mode change should be done asynchronously, 
              if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF
              
              Set modeSet = E_OK if the mode has been written and verified (synchronous) / the request has been
              acknowledged (asynchronous).
           
              Recommendations:
              - Verify that the written data has been acknowledged by the underlying transceiver (e.g. by reading back the register)
              - Use CanTrcv_30_GenericCan_Get*Req to check which requests are currently pending
              - Implement a state machine that is handled in MainFunctionRequestHandler
              
              Restrictions:
              - When in CanTrcv_30_GenericCan_Init():
                - DO NOT access the hardware asynchronously
                - DO NOT call CanTrcv_30_GenericCan_ConfirmPnAvaibility()
                
                
              - When in CanTrcv_30_GenericCan_SetOpMode():
                - If the request has been acknowledged by the underlying SPI, the API CanTrcv_30_GenericCan_ModeIndication() 
                  MUST be called as soon as the mode change is completed and verified.
                - Any asynchronous mode change can be interrupted by any other API. 
                    a. The driver MUST NOT loose any new request (ClearWufFlag, CheckWakeFlag, SetOpMode, ...)
                    b. The driver MUST only confirm the latest mode request. All other pending mode requests
                       MUST be cancelled without confirmation.
                    c. The driver MUST confirm the requests in the same order as they were called.
                - When NORMAL mode is requested, the driver MUST call CanTrcv_30_GenericCan_SetConfirmPnAvaibilityReq(),
                  request the StatusFlags and call CanTrcv_30_GenericCan_GetStatusFlagsIndication() as soon as the 
                  StatusFlags are ready. This MUST be done BEFORE changing operation mode to NORMAL.
        
              Example:
                // This example assumes that there is a function CanTrcv_30_GenericCan_ModeChangeHandler that
                // can be called from MainFunction context, SPI Indication context and SetOpMode Context and
                // also considers any other pending request. 
                //
                // When this function is called with EVENT_CANCEL, the previous request is cancelled as soon
                // as possible (e.g. after all SPI requests are finished). After cancellation the function
                // process the nextRequestMode.
                //
                // When this function is called with EVENT_START, the function tries to reach the mode requested 
                // with SetOpModeReq
                //
                // If the driver is already initialized and the mode change has completed, the function calls
                // CanTrcv_30_GenericCan_ModeIndication()
                //
                // If CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF, the function has a synchronous behavior.
              
              
                modeSet = CanTrcv_30_GenericCan_ModeChangeHandler(index, EVENT_START);
            
        */
        #endif
        <Your_Trcv_Code>
        
        modeSet = E_OK;

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
        CanTrcv_LeaveCritical();
#endif /* DIO / SBC */



#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) 
        /* #300 Check if setting of operating mode was successful. */
        if (modeSet == E_OK)
        {
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
          /* #310 Mode change was successful: Store the current operating mode. (only SPI-interface) */
          CanTrcv_30_GenericCan_Prob[index].curOpMode = CANTRCV_30_GENERICCAN_OP_MODE_NORMAL; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

          /* #320 Mode change was successful: Clear all pending requests. (only SPI-interface) */
          CanTrcv_30_GenericCan_Prob[index].requestState = CanTrcv_30_GenericCan_Req_None; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
# endif
          /* #330 Mode change was successful: Set current transceiver channel to initialized. */
          CanTrcv_30_GenericCan_Prob[index].isInit = CANTRCV_30_GENERICCAN_IS_INIT; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
        }
        else
        {
          /* \trace SPEC-20013 */
          /* #340 Mode change was NOT successful: Report error. */
          errorId = CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL;
        }
#endif
      } /* CanTrcvChannelUsed == TRUE */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON) && (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
      else
      {
        /* Just to prevent MISRA violations */
      }
#endif
    } /* for (...) */
  } /* if (ConfigPtr == NULL_PTR) .. else */

#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR) /* PRQA S 2992,2996 */ /* MD_CanTrcv_30_GenericCan_14.3 */ /* COV_CANTRCV_HL_GENERICCAN_CODECOV_NOT_EACH_CONFIG */
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_INIT_ID, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
#endif
  
#if (CANTRCV_30_GENERICCAN_USE_INIT_POINTER == STD_OFF)
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(ConfigPtr); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
#endif
} /* PRQA S 6030,6050,6080 */ /* MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_SetOpMode
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_SetOpMode(uint8 CanTrcvIndex, CanTrcv_TrcvModeType OpMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) 
  Std_ReturnType modeSet = E_NOT_OK;
#endif

  CanTrcv_30_GenericCan_TrcvModeType currentOpMode;

  /* ----- Development Error Checks ------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* \trace SPEC-19976 */  
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active. */
  else if(!CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
  {
    /* \trace SPEC-19976 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex))
  {
    /* \trace SPEC-15064 */
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute transition of operating mode only if transceiver channel (CanTrcvIndex) is active. */
    if(CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
#endif
    {
      /* #110 Check if the current operating mode equals the requested one. */
      (void)CanTrcv_30_GenericCan_GetOpMode(CanTrcvIndex, &currentOpMode); /* SBSW_CANTRCV_HL_GENERICCAN_3 */

#if defined (CANTRCV_30_GENERICCAN_ALLOW_ENTER_NORMAL_TWICE)
      /* If OpMode is the same as the currentOpMode (except for NORMAL MODE), return E_OK */
      if ((currentOpMode == OpMode) && (OpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL))
#else
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)      
      if ((currentOpMode == OpMode) && (CanTrcv_30_GenericCan_GetOpModeReq(CanTrcvIndex) == CanTrcv_30_GenericCan_Req_None)) /* \trace SPEC-15090 */ /* COV_CANTRCV_HL_GENERICCAN_CODECOV_OP_MODE_REQ_NONE */
# else
      if (currentOpMode == OpMode) /* \trace SPEC-15090 */
# endif
#endif
      {
        /* \trace SPEC-19955 */
        returnVal = E_OK;
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) 
        /* The mode was verified within GetOpMode, so the mode was set */
        modeSet = E_OK;
#endif

#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
# if !defined (CANTRCV_30_GENERICCAN_ALLOW_ENTER_NORMAL_TWICE)
        /* #130 The requested one equals the current one. Do NOT execute the mode change request. */
        if ((currentOpMode == CANTRCV_30_GENERICCAN_OP_MODE_NORMAL) &&
            (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].pnAvailable == TRUE) && (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].isPnEnabled == TRUE))
        {
          /* #140 The requested one equals the current one. If PN is available and enabled confirm the PN availability. (only PN) */
          CanIf_30_GenericCan_ConfirmPnAvailability(CanTrcvIndex); /* \trace SPEC-15087, SPEC-19956 */
        }
# endif
#endif

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        /* #150 The requested one equals the current one. Inform the CanIf about the successful mode change. (only SPI-interface) */
        CanIf_30_GenericCan_TrcvModeIndication(CanTrcvIndex, currentOpMode); /* \trace SPEC-20006 */
#endif
      }
      else
      {
        /* #160 The requested one does NOT equal the current one. Execute the requested mode change. (critical section is used) */
        /* \trace SPEC-19962 */
        switch(OpMode)
        {
          /* #200 Mode change request to NORMAL. */
          case CANTRCV_30_GENERICCAN_OP_MODE_NORMAL:

            /* #210 Check whether it is an internal wake-up. If yes store the wake-up reason. */         
            if(
# if defined (CANTRCV_30_GENERICCAN_ALLOW_ENTER_NORMAL_TWICE)
               (currentOpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL) && 
# endif
               (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason == CANTRCV_30_GENERICCAN_WU_ERROR))
            {
              /* Transceiver is switched from STANDBY/SLEEP -> NORMAL and has not detected any wakeup.
                 This is considered as internal wakeup. */
              CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_INTERNALLY; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
            }
            
            CanTrcv_EnterCritical();

# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#  if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
#   if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON)
            CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpInTransitionDetected = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#   endif
#  endif
# endif
            /* #220 If ICU is configured, disable ICU. */
# if (CANTRCV_30_GENERICCAN_USE_ICU == STD_ON)
            if (CanTrcvCfg_IsIcuChannelSet(CanTrcvIndex)) /* \trace SPEC-14950 */
            {
              Icu_DisableNotification( CanTrcvCfg_GetIcuChannel(CanTrcvIndex) ); /* \trace SPEC-14973 */
            }
# endif
            
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && \
             (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
            if (CanTrcv_30_GenericCan_GetOpModeReq(CanTrcvIndex) != 0x0u)
            {
              /* #230 If any mode request is currently pending, queue this request and cancel previous worker (only asynchronous SPI-interface) */
              CanTrcv_30_GenericCan_Prob[CanTrcvIndex].nextRequestState = CanTrcv_30_GenericCan_Req_OpModeNormal; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
              /*  CancelOpModeReq
                  --------------
              */
              #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
              /*  When using DIO interface:
                  =========================
                    Leave empty.
              */
              #endif
              #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
              /*  When using SPI interface:
                  =========================
                    Cancel all previously pending mode changes. Call 
                    CanTrcv_30_GenericCan_ProcessNextOpModeReq(CanTrcvIndex, processNewRequest);
                    as soon as the cancel request has completed. If "processNewRequest" contains E_OK,
                    a new request is pending and should be perfomed.
                  
                    Restrictions:
                    - DO NOT report ModeIndications for the cancelled job
                  
                    Example:
                      modeSet = CanTrcv_30_GenericCan_ModeChangeHandler(CanTrcvIndex, CANTRCV_30_E52013_WORKER_EV_CANCEL);
              */
              #endif
              <Your_Trcv_Code>
            }
            else
# endif
            {
              /* #240 No mode request is currently pending. Store the current mode request. (only SPI-interface) */
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
              CanTrcv_30_GenericCan_SetOpModeReq(CanTrcvIndex, CanTrcv_30_GenericCan_Req_OpModeNormal); /* SBSW_CANTRCV_HL_GENERICCAN_1 */ /* SBSW_CANTRCV_HL_GENERICCAN_1 */
# endif
              /* #250 Execute operating mode change request to NORMAL. */
              /*  SetOpModeNormal
                  ---------------
                  
                  Set the underlying transceiver hardware to normal operation mode by changing
                  the Dio Levels.
              */
              #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
              /*  When using DIO interface:
                  ========================
                    Example for TJA1041:
                      Dio_WriteChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(CanTrcvIndex),STD_HIGH);
                      Dio_WriteChannel(CanTrcv_30_GenericCan_GetENOfDioConfiguration(CanTrcvIndex),STD_HIGH);
                      modeSet = E_OK;
              */
              #endif
              #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
              /*  When using SPI interface:
                  =========================
                    While in context of CanTrcv_30_GenericCan_Init() this mode change MUST be done synchronously.
                    
                    When in context of CanTrcv_30_GenericCan_SetOpMode() this mode change should be done asynchronously, 
                    if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF
                    
                    Set modeSet = E_OK if the mode has been written and verified (synchronous) / the request has been
                    acknowledged (asynchronous).
                 
                    Recommendations:
                    - Verify that the written data has been acknowledged by the underlying transceiver (e.g. by reading back the register)
                    - Use CanTrcv_30_GenericCan_Get*Req to check which requests are currently pending
                    - Implement a state machine that is handled in MainFunctionRequestHandler
                    
                    Restrictions:
                    - When in CanTrcv_30_GenericCan_Init():
                      - DO NOT access the hardware asynchronously
                      - DO NOT call CanTrcv_30_GenericCan_ConfirmPnAvaibility()
                      
                      
                    - When in CanTrcv_30_GenericCan_SetOpMode():
                      - If the request has been acknowledged by the underlying SPI, the API CanTrcv_30_GenericCan_ModeIndication() 
                        MUST be called as soon as the mode change is completed and verified.
                      - Any asynchronous mode change can be interrupted by any other API. 
                          a. The driver MUST NOT loose any new request (ClearWufFlag, CheckWakeFlag, SetOpMode, ...)
                          b. The driver MUST only confirm the latest mode request. All other pending mode requests
                             MUST be cancelled without confirmation.
                          c. The driver MUST confirm the requests in the same order as they were called.
                      - When NORMAL mode is requested, the driver MUST call CanTrcv_30_GenericCan_SetConfirmPnAvaibilityReq(),
                        request the StatusFlags and call CanTrcv_30_GenericCan_GetStatusFlagsIndication() as soon as the 
                        StatusFlags are ready. This MUST be done BEFORE changing operation mode to NORMAL.
              
                    Example:
                      // This example assumes that there is a function CanTrcv_30_GenericCan_ModeChangeHandler that
                      // can be called from MainFunction context, SPI Indication context and SetOpMode Context and
                      // also considers any other pending request. 
                      //
                      // When this function is called with EVENT_CANCEL, the previous request is cancelled as soon
                      // as possible (e.g. after all SPI requests are finished). After cancellation the function
                      // process the nextRequestMode.
                      //
                      // When this function is called with EVENT_START, the function tries to reach the mode requested 
                      // with SetOpModeReq
                      //
                      // If the driver is already initialized and the mode change has completed, the function calls
                      // CanTrcv_30_GenericCan_ModeIndication()
                      //
                      // If CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF, the function has a synchronous behavior.
                    
                    
                      modeSet = CanTrcv_30_GenericCan_ModeChangeHandler(CanTrcvIndex, EVENT_START);
                  
              */
              #endif
              <Your_Trcv_Code>
              
              modeSet = E_OK;

            }

            CanTrcv_LeaveCritical();
            returnVal = E_OK;
            break;

          case CANTRCV_30_GENERICCAN_OP_MODE_STANDBY:
             /* #300 Mode change request to STANDBY. (only if supported by HW) */
            /* #310 Mode transition to STANDBY is only possible from NORMAL. Otherwise DET error is reported. */
            if(currentOpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL) /* \trace SPEC-19955 */
            {
#  if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
              /* \trace SPEC-19985 */
              errorId = CANTRCV_30_GENERICCAN_E_TRCV_NOT_NORMAL;
#  endif
            }
            else
            {
              /* #320 Current mode is NORMAL: Execute mode request. */
              CanTrcv_EnterCritical();

              /* #330 If ICU is configured, enable ICU. */
# if (CANTRCV_30_GENERICCAN_USE_ICU == STD_ON)
              if (CanTrcvCfg_IsIcuChannelSet(CanTrcvIndex)) /* \trace SPEC-14950 */
              {
                Icu_EnableNotification( CanTrcvCfg_GetIcuChannel(CanTrcvIndex) ); /* \trace SPEC-15004 */
              }
# endif
              /* #340 Reset wake-up reason. */
              CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_ERROR; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && \
             (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
              if (CanTrcv_30_GenericCan_GetOpModeReq(CanTrcvIndex) != 0x0u)
              {
                /* #350 If any mode request is currently pending queue this request and cancel previous worker. (only asynchronous SPI-interface) */
                CanTrcv_30_GenericCan_Prob[CanTrcvIndex].nextRequestState = CanTrcv_30_GenericCan_Req_OpModeStandby; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
                /*  CancelOpModeReq
                    --------------
                */
                #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
                /*  When using DIO interface:
                    =========================
                      Leave empty.
                */
                #endif
                #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
                /*  When using SPI interface:
                    =========================
                      Cancel all previously pending mode changes. Call 
                      CanTrcv_30_GenericCan_ProcessNextOpModeReq(CanTrcvIndex, processNewRequest);
                      as soon as the cancel request has completed. If "processNewRequest" contains E_OK,
                      a new request is pending and should be perfomed.
                    
                      Restrictions:
                      - DO NOT report ModeIndications for the cancelled job
                    
                      Example:
                        modeSet = CanTrcv_30_GenericCan_ModeChangeHandler(CanTrcvIndex, CANTRCV_30_E52013_WORKER_EV_CANCEL);
                */
                #endif
                <Your_Trcv_Code>
              }
              else
# endif
              {
                /* #360 No mode request is currently pending. Store the mode request. (only SPI-interface) */
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
                CanTrcv_30_GenericCan_SetOpModeReq(CanTrcvIndex, CanTrcv_30_GenericCan_Req_OpModeStandby); /* SBSW_CANTRCV_HL_GENERICCAN_1 */ /* SBSW_CANTRCV_HL_GENERICCAN_1 */
# endif
                /* #370 Execute operating mode change request to STANDBY. */
                /*  SetOpModeStandby
                    ----------------
                    Set the underlying transceiver hardware to operation mode standby.
                */
                #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
                /*  When using DIO interface:
                    =========================
                      Example for TJA1041:
                        Dio_WriteChannel(CanTrcv_30_GenericCan_GetENOfDioConfiguration(CanTrcvIndex),STD_LOW);
                        Dio_WriteChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(CanTrcvIndex),STD_LOW);
                        modeSet = E_OK;
                */
                #endif
                #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
                /*  When using SPI interface:
                    =========================
                      While in context of CanTrcv_30_GenericCan_Init() this mode change MUST be done synchronously.
                      
                      When in context of CanTrcv_30_GenericCan_SetOpMode() this mode change should be done
                      asynchronously, if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF
                      
                      Set modeSet = E_OK if the mode has been written and verified (synchronous) / the request has been
                      acknowledged (asynchronous).
                      
                      See SetOpModeNormal for details.
                    
                */
                #endif
                <Your_Trcv_Code>
                
                modeSet = E_OK;
              }

              CanTrcv_LeaveCritical();
              returnVal = E_OK;
            }
            break;

          case CANTRCV_30_GENERICCAN_OP_MODE_SLEEP:
            /* #400 Mode change request to SLEEP. (only if supported by HW) */

            /* #410 Mode transition to SLEEP is only possible from STANDBY. Otherwise DET error is reported. */
            if(currentOpMode != CANTRCV_30_GENERICCAN_OP_MODE_STANDBY)
            {
#  if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
              /* \trace SPEC-19964 */
              errorId = CANTRCV_30_GENERICCAN_E_TRCV_NOT_STANDBY;
#  endif
            }
            else
            {
              CanTrcv_EnterCritical();

# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && \
             (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
              if (CanTrcv_30_GenericCan_GetOpModeReq(CanTrcvIndex) != 0x0u)
              {
                if (CanTrcv_30_GenericCan_GetOpModeReq(CanTrcvIndex) == CanTrcv_30_GenericCan_Req_OpModeNormal)
                {
                  /* Invalid transition */
                  returnVal = E_NOT_OK;
                }
                else
                {
                  /* #420 If any mode request is currently pending queue this request and cancel previous worker. (only asynchronous SPI-interface) */
                  CanTrcv_30_GenericCan_Prob[CanTrcvIndex].nextRequestState = CanTrcv_30_GenericCan_Req_OpModeSleep; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
                  /*  CancelOpModeReq
                      --------------
                  */
                  #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
                  /*  When using DIO interface:
                      =========================
                        Leave empty.
                  */
                  #endif
                  #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
                  /*  When using SPI interface:
                      =========================
                        Cancel all previously pending mode changes. Call 
                        CanTrcv_30_GenericCan_ProcessNextOpModeReq(CanTrcvIndex, processNewRequest);
                        as soon as the cancel request has completed. If "processNewRequest" contains E_OK,
                        a new request is pending and should be perfomed.
                      
                        Restrictions:
                        - DO NOT report ModeIndications for the cancelled job
                      
                        Example:
                          modeSet = CanTrcv_30_GenericCan_ModeChangeHandler(CanTrcvIndex, CANTRCV_30_E52013_WORKER_EV_CANCEL);
                  */
                  #endif
                  <Your_Trcv_Code>

                  returnVal = E_OK;
                }
              }
              else
# endif
              {
                /* #430 No mode request is currently pending. Store the mode request. (only SPI-interface) */
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
                CanTrcv_30_GenericCan_SetOpModeReq(CanTrcvIndex, CanTrcv_30_GenericCan_Req_OpModeSleep); /* SBSW_CANTRCV_HL_GENERICCAN_1 */ /* SBSW_CANTRCV_HL_GENERICCAN_1 */
# endif
                /* #440 Execute operating mode change request to SLEEP. */
                /*  SetOpModeSleep
                    ----------------
                    Set the underlying transceiver hardware to operation mode sleep.
                    
                */  
                #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)  
                /*  When using DIO interface:
                    =========================
                      Example for TJA1041:
                        Dio_WriteChannel(CanTrcv_30_GenericCan_GetENOfDioConfiguration(CanTrcvIndex),STD_LOW);
                        Dio_WriteChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(CanTrcvIndex),STD_LOW);
                */
                #endif
                #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
                /*  When using SPI interface:
                    =========================
                      While in context of CanTrcv_30_GenericCan_Init() this mode change MUST be done synchronously.
                      
                      When in context of CanTrcv_30_GenericCan_SetOpMode() this mode change should be done
                      asynchronously, if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF
                      
                      Set modeSet = E_OK if the mode has been written and verified (synchronous) / the request has been
                      acknowledged (asynchronous).
                      
                      See SetOpModeNormal for details.
                    
                */
                #endif
                <Your_Trcv_Code>
                
                modeSet = E_OK;

                returnVal = E_OK;
              }

              CanTrcv_LeaveCritical();
            }
            break;

          default:
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
            /* \trace SPEC-14963 */
            /* #40 Check if parameter OpMode is valid. */
            errorId = CANTRCV_30_GENERICCAN_E_PARAM_TRCV_OP_MODE;
#endif
            break;
          } /* switch() */
      } /* currentOpMode == OpMode */

      /* #600 If mode request executed successfully then inform the CanIf else report error. */
      if (returnVal == E_OK)
      {
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        /* For SPI transceivers, the operating mode is verified directly after setting */
        if (modeSet != E_OK) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_HW_SPECIFIC */
# else
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        (void)CanTrcv_30_GenericCan_GetOpMode(CanTrcvIndex, &currentOpMode); /* SBSW_CANTRCV_HL_GENERICCAN_3 */

        if(currentOpMode == OpMode) /* COV_CANTRCV_LL_GENERICCAN_CODECOV_HW_FAILURE */
#  endif
        {
          /* #620 Inform the CanIf about successful operating mode transition. */
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
          CanIf_30_GenericCan_TrcvModeIndication(CanTrcvIndex, currentOpMode); /* \trace SPEC-20006 */
#  endif
        }
        else
# endif
        {
          /* \trace SPEC-20011 */
          errorId = CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL;

          returnVal = E_NOT_OK;
        } /* currentOpMode != OpMode */
      } /* returnVal == E_OK */
    } /* channelUsed == TRUE */
  }

#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_SETOPMODE_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
#endif
  return returnVal;
} /* PRQA S 6010,6030,6050,6080 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetOpMode
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetOpMode(uint8 CanTrcvIndex, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) OpMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;


#if defined(CANTRCV_30_GENERICCAN_INTERFACE_DIO)
  /*  DioPins_GetOpMode
      -----------------
  
      Specify the needed Dio pins for reading the transceivers current operation 
      mode. The pins have to be specified with Dio_LevelType type.
      
      Example for TJA1041:
      Dio_LevelType CanTrcvSTB;
      Dio_LevelType CanTrcvEN;
  */
  <Your_Trcv_Code>
# endif

  /* ----- Development Error Checks ------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter OpMode is valid. */
  if(OpMode == NULL_PTR)
  {
    /* \trace SPEC-14958 */
    errorId = CANTRCV_30_GENERICCAN_E_PARAM_POINTER;
  }
  /* #20 Check if parameter CanTrcvIndex is valid. */
  else if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* \trace SPEC-20009 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is active. */
  else if(!CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
  {
    /* \trace SPEC-20009 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #40 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex))
  {
    /* \trace SPEC-15042 */
    errorId =  CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. */
    if(CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
#endif
    {
      /* #200 DIO-interface: Read the mode from status of PINs of underlying HW. (critical section used) */
      /* #201 SPI-interface: Read the mode from state variable. */
      /* #202 SBC-interface: Read the mode via SBC module API. (critical section used) */
#if defined(CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
      CanTrcv_EnterCritical();

      /*  GetPins_GetOpMode
          -----------------
      */  
      #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)    
      /*  When using DIO interface:
          =========================
            Read the DioChannel Levels for the Trcv pins needed to obtain the current
            operation mode. You may use CanTrcv_30_GenericCan_Get<NameOfPin>OfDioConfiguration(CanTrcvIndex)
            to get the configured Dio pins.
            Store the values to the local Dio_LevelType variables specified in 
            DioPins_GetOpMode. 
            
            Example for TJA1041 
              CanTrcvSTB = Dio_ReadChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(CanTrcvIndex));
              CanTrcvEN  = Dio_ReadChannel(CanTrcv_30_GenericCan_GetENOfDioConfiguration(CanTrcvIndex));
      */
      #endif
      #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)  
      /*  When using SPI interface:
          =========================
            Leave empty.
      */
      #endif
      <Your_Trcv_Code>
      
      returnVal = E_OK;

      CanTrcv_LeaveCritical();

#endif
      {
        /* \trace SPEC-19974 */

        /*  CheckOpmodeNormal
            -----------------
            Check if normal mode is reached.
            
            This code must have the following format
            
              if(<condition for normal mode>)
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*  When using DIO interface
            ========================
              Use the local Dio_LevelType variables specified in DioPins_GetOpMode and
              read in GetPins_GetOpMode to determine the current operation mode.
            
              Example for TJA1041
                if((CanTrcvSTB == STD_HIGH) && (CanTrcvEN == STD_HIGH))
        */ 
        #endif
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        /*  When using SPI interface
            ========================
              Return the current state of the underlying transceiver hardware.
              It is recommended NOT to query the hardware but using curOpMode instead.
            
              Example:
              if ( (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].curOpMode == CANTRCV_30_GENERICCAN_OP_MODE_NORMAL) )
        */
        #endif
        <Your_Trcv_Code>
        {
          /* CAN transceiver is in normal operation mode */
          *OpMode = CANTRCV_30_GENERICCAN_OP_MODE_NORMAL; /* SBSW_CANTRCV_HL_GENERICCAN_2 */
          returnVal = E_OK;
        }

        /*  CheckOpmodeSleep
            -----------------
            Check if sleep mode is reached.
            
            This code must have the following format
            
              if(<condition for normal mode>)
              
            If sleep mode is not supported, you should place if(0) here and remove
            the define from SupportedModes      
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*  When using DIO interface
            ========================
              Use the local Dio_LevelType variables specified in DioPins_GetOpMode and
              read in GetPins_GetOpMode to determine the current operation mode.
            
              Example for TJA1041
                if((CanTrcvSTB == STD_LOW) && (CanTrcvEN == STD_HIGH))
        */
        #endif
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        /*  When using SPI interface
            ========================
              Return the current state of the underlying transceiver hardware.
              It is recommended NOT to query the hardware but using curOpMode instead.
            
              Example:
              if ( (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].curOpMode == CANTRCV_30_GENERICCAN_OP_MODE_SLEEP) )
        */
        #endif
        <Your_Trcv_Code>
        {
          /* CAN transceiver is in sleep mode */
          *OpMode = CANTRCV_30_GENERICCAN_OP_MODE_SLEEP; /* SBSW_CANTRCV_HL_GENERICCAN_2 */
          returnVal = E_OK;
        }


        /*  CheckOpmodeStandby
            -----------------
            Check if standby mode is reached.
            
            This code must have the following format
            
              if(<condition for normal mode>)
              
            If standby mode is not supported, you should place if(0) here and remove
            the define from SupportedModes      
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*
            When using DIO interface
            ========================
              Use the local Dio_LevelType variables specified in DioPins_GetOpMode and
              read in GetPins_GetOpMode to determine the current operation mode.
            
              Example for TJA1041
                if((CanTrcvSTB == STD_LOW) && (CanTrcvEN == STD_LOW))
        */
        #endif
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        /* When using SPI interface
            ========================
              Return the current state of the underlying transceiver hardware.
              It is recommended NOT to query the hardware but using curOpMode instead.
            
              Example:
              if ( (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].curOpMode == CANTRCV_30_GENERICCAN_OP_MODE_STANDBY) )
        */
        #endif
        <Your_Trcv_Code>
        {
          /* CAN transceiver is in standby mode */
          *OpMode = CANTRCV_30_GENERICCAN_OP_MODE_STANDBY; /* SBSW_CANTRCV_HL_GENERICCAN_2 */
          returnVal = E_OK;
        }
      }

      /* #210 If reading of operating mode NOT succeeded report error. */
      if (returnVal == E_NOT_OK) /* PRQA S 2992,2996 */ /* MD_CanTrcv_30_GenericCan_14.3 */ /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_HW_SPECIFIC */
      {
        /* \trace SPEC-19940 */
        errorId = CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
      }
    } /* Channel is used */
  } /* DET checks passed */
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_GETOPMODE_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
#endif

 return returnVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetBusWuReason
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetBusWuReason(uint8 CanTrcvIndex, P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) Reason)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;

#if (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT != CANTRCV_30_GENERICCAN_WAKEUP_NONE) 
  CanTrcv_30_GenericCan_TrcvModeType currentOpMode;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if the return-parameter Reason is valid. */
  if(Reason == NULL_PTR)
  {
    /* \trace SPEC-15063 */
    errorId = CANTRCV_30_GENERICCAN_E_PARAM_POINTER;
  }
  /* #20 Check if parameter CanTrcvIndex is valid. */
  else if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* \trace SPEC-19993 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is active. */
  else if(!CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
  {
    /* \trace SPEC-19993 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #40 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex))
  {
    /* \trace SPEC-15059 */
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. */
    if(CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
#endif
    {
      /* UREQ00015010 */
      /* #200 Check the wake-up reason for validity. */
#if (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT != CANTRCV_30_GENERICCAN_WAKEUP_NONE) 
      if(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason == CANTRCV_30_GENERICCAN_WU_ERROR)
      {
        /* #300 If wake-up reason is invalid return WU_ERROR. */
        *Reason = CANTRCV_30_GENERICCAN_WU_ERROR; /* SBSW_CANTRCV_HL_GENERICCAN_2 */

        /* #310 Wake-up reason is invalid (WU_ERROR). This is OK only in case of transceiver channel is in NORMAL mode. */
        (void)CanTrcv_30_GenericCan_GetOpMode(CanTrcvIndex, &currentOpMode); /* SBSW_CANTRCV_HL_GENERICCAN_3 */

        if(currentOpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL)
        {
          /* \trace SPEC-19980 */
          /* #320 Being in any mode except NORMAL with WU_ERROR is an error hence report it. */
          errorId = CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL;
        }
      }
      else
      {
        /* #400 Wake-up reason is valid (neither WU_ERROR nor WU_NOT_SUPPORTED). Return the current wake-up reason.*/
        *Reason = CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason; /* SBSW_CANTRCV_HL_GENERICCAN_2 */
        returnVal = E_OK;
      }
#else /* (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT == CANTRCV_30_GENERICCAN_WAKEUP_NONE) */
      *Reason = CANTRCV_30_GENERICCAN_WU_NOT_SUPPORTED; /* SBSW_CANTRCV_HL_GENERICCAN_2 */
#endif /* (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT != CANTRCV_30_GENERICCAN_WAKEUP_NONE) */
    } /* IsChannelUsed */
  }
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_GETBUSWUREASON_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
#endif
  
  return returnVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_SetWakeupMode
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_SetWakeupMode(uint8 CanTrcvIndex, CanTrcv_TrcvWakeupModeType TrcvWakeupMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* \trace SPEC-19998 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active. */
  else if(!CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
  {
    /* \trace SPEC-19998 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex))
  {
    /* \trace SPEC-15077 */
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT != CANTRCV_30_GENERICCAN_WAKEUP_NONE) 
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. */
    if(CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
# endif
    {
      /* #200 Set the requested wake-up mode. */
      switch(TrcvWakeupMode)
      {
        case CANTRCV_30_GENERICCAN_WU_ENABLE:
             /* \trace SPEC-19990 */
             CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReport = CANTRCV_30_GENERICCAN_WU_ENABLE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

             /* #300 In addition: If requested wake-up mode is WU_ENABLE report if wake-up event(s) is/are pending. */
# if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) || \
     (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON)
             CanTrcv_30_GenericCan_ReportWakeup(CanTrcvIndex);
# endif

             returnVal = E_OK;
             break;

        case CANTRCV_30_GENERICCAN_WU_DISABLE:
             /* \trace SPEC-20012 */
             CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReport = CANTRCV_30_GENERICCAN_WU_DISABLE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

             returnVal = E_OK;
             break;

        case CANTRCV_30_GENERICCAN_WU_CLEAR:
             /* \trace SPEC-19961 */
             /* #500 In addition: If requested wake-up mode is WU_CLEAR clear the wake-up reason and all detected wake-up events. */
             CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_ERROR; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
             returnVal = E_OK;

             break;
        default:
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
             /* #40 Check if parameter TrcvWakeupMode is valid. */
             errorId = CANTRCV_30_GENERICCAN_E_PARAM_TRCV_WAKEUP_MODE; /* \trace SPEC-15036 */
# endif
             break;
      }
    }
#endif /* CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT != CANTRCV_30_GENERICCAN_WAKEUP_NONE */
  }
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_SETWAKEUPMODE_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
#endif  
  
#if (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT == CANTRCV_30_GENERICCAN_WAKEUP_NONE) 
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(TrcvWakeupMode); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
# if(CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(CanTrcvIndex); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
# endif
#endif
  /* \trace SPEC-19978: Not implemented as no need to query HW at this place. */

  return returnVal;
}


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CheckWakeup
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CheckWakeup(uint8 CanTrcvIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType returnVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;



  /* ----- Development Error Checks ------------------------------------- */
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* \trace SPEC-20003 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active. */
  else if(!CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
  {
    /* \trace SPEC-20003 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex))
  {
    /* \trace SPEC-19994 */
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON)
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active and wake-up detection is enabled. (critical section used) */
    if(CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
# endif
    {
      if(CanTrcvCfg_IsWakeupByBusUsed(CanTrcvIndex) &&
         (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReport == CANTRCV_30_GENERICCAN_WU_ENABLE)) /* COV_CANTRCV_HL_GENERICCAN_CODECOV_IS_WU_BY_BUS_USED */
      {
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
        CanTrcv_30_GenericCan_TrcvModeType currentOpMode;
# endif
        CanTrcv_EnterCritical();

        /* #110 Is the transceiver channel in NORMAL? (only DIO-/SBC-interface) */
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
        (void)CanTrcv_30_GenericCan_GetOpMode(CanTrcvIndex, &currentOpMode); /* SBSW_CANTRCV_HL_GENERICCAN_3 */

        if (currentOpMode == CANTRCV_30_GENERICCAN_OP_MODE_NORMAL)
        {
          /* #120 Operating mode NORMAL: No valid wake-up event detected. (only DIO-/SBC-interface)*/
          returnVal = E_NOT_OK;
        }
        else
# endif
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) || defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        /* #200 Check if there are any wake-up events pending in HW. */
        if (CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_BY_PIN */
        {
          /* #300 Any wake-up event is pending: Check if currently an operating mode change is requested. (only asynchronous SPI-interface) */
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
          if (CanTrcv_30_GenericCan_GetOpModeReq(CanTrcvIndex) != 0u)
          {
            /* #310 Currently operating mode change is requested: Store the detected wake-up event. (only asynchronous SPI-interface) */
            CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpInTransitionDetected = TRUE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

            returnVal = E_OK; /* Request acknowledged */
          }
          else
#   endif /* (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) */
          /* #320 In which operating mode is the transceiver channel? (only SPI-interface) */
          if (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].curOpMode == CANTRCV_30_GENERICCAN_OP_MODE_NORMAL)
          {
#   if (CANTRCV_30_GENERICCAN_USE_ENDCHECKWAKEUP == STD_ON)
            /* #330 Operating mode NORMAL: No valid wake-up event detected. Inform EcuM that the check-wake-up-procedure has completed. (only SPI-interface) */
            EcuM_EndCheckWakeup( CanTrcvCfg_GetWakeupSource(CanTrcvIndex) ); /* \trace SPEC-40535 */
#   endif
            returnVal = E_NOT_OK; /* No wakeup detected */
          }
          else
#  endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */
          {
            /* #340 Operating mode STANDBY / SLEEP: Report the detected wake-up event to the EcuM. */
            CanTrcv_30_GenericCan_ReportWakeup(CanTrcvIndex);

#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
            /* #350 Clear any wake-up event that occurred during mode transition (only asynchronous SPI-interface) */
            CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpInTransitionDetected = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#   endif /* (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) */
#  endif /* (CANTRCV_30_GENERICCAN_INTERFACE_SPI) */
            returnVal = E_OK; /* wake-up detected  */
          }
        }
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
        else if ((CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlagsNew == TRUE) &&
                 ((CanTrcv_30_GenericCan_StateWupIsBus(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags))
                 || (CanTrcv_30_GenericCan_StateWupIsPin(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags))
#    if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
                 || ((CanTrcv_30_GenericCan_Prob[CanTrcvIndex].isPnEnabled == TRUE) &&
                     (CanTrcv_30_GenericCan_StateErrIsSys(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags))
                    )
#    endif /* (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) */
                  )
                ) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_BY_PIN */
        {
          /* #360 If there are already valid wake-up status flags. Do NOT request new flags but use the valid ones instead. (only asynchronous SPI-interface) */

          /* #370 Mark the used wake-up flags as used. (only asynchronous SPI-interface) */
          CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlagsNew = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

          /* #380 Evaluate the wake-up flags. */
          CanTrcv_30_GenericCan_CbWakeupByBusIndication(CanTrcvIndex);

          returnVal = E_OK; /* Request acknowledged */ /* \trace SPEC-20014  */
        }
#   endif /* (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) */
#  endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */
        else
# endif /* CANTRCV_30_GENERICCAN_INTERFACE_DIO || CANTRCV_30_GENERICCAN_INTERFACE_SPI */
        {
          /* #400 No pending wake-up flags. Request the underlying HW for wake-up flags. */

#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
          CanTrcv_30_GenericCan_SetCbByBusFlagReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#  endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */

          /*  GetStatusFlags
              --------------
              Request all status flags (SYSERR, POR, WUP, WUF, LWUP, ...) from the underlying transceiver hardware. 
              Set returnVal = E_OK if the request was successful. The flags are evaluated later with the CanTrcv_30_GenericCan_State*
              macros.
              
          */
          #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
          /*  When using DIO interface:
              =========================
                Store the current level of the DIO pins to statusFlags. 
                
                Example TJA1041:
                  statusFlags.PinERR = Dio_ReadChannel(CanTrcv_30_GenericCan_GetERROfDioConfiguration(CanTrcvIndex));
                  statusFlags.PinSTB = Dio_ReadChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(CanTrcvIndex));
                  returnVal = E_OK;
          */
          #endif
          #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
          /*  When using a SPI interface:
              ===========================
                Depending on CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS, this should be either done asynchronously or synchronously.
              
                Restrictions:
                  - If the driver is not initialized yet the access MUST be done synchronously (e.g. in context
                    of CanTrcv_30_GenericCan_Init())
                  - When using asynchronous access the request to read status flags may be interrupted
                    by any other API / request. The driver MUST handle this.
                  - DO NOT loose any request
                  - ALWAYS call CanTrcv_30_GenericCan_GetStatusFlagsIndication as soon as status flags
                    have been read. 
                  - If the same flags are polled in MainFunction ensure that the polled flags are
                    processed even if a new request (e.g. from interrupt context) is pending
                  - ALWAYS call the pending indications for an active request
                      - GetCbByBusFlagReq == TRUE -> CanTrcv_30_GenericCan_CbWakeupByBusIndication
              
                Example:
                  // This function will try to request the status flags. If the driver is currently busy with e.g. a mode change
                  // the request will be held pending and processed as soon as the driver is ready again.
              
                  // As soon as the flags are ready, the function GetStatusFlagsIndication is beeing called.
                  returnVal = CanTrcv_30_GenericCan_RequestStatusFlags(index, FALSE);
          */
          #endif
          <Your_Trcv_Code>
          /* --> Flags will be processed in GetStatusFlagsIndication + CbWakeupByBusIndication */

#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
          if (returnVal == E_NOT_OK)
          {
            /* Remove request as it failed */
            CanTrcv_30_GenericCan_ClrCbByBusFlagReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */

#   if (CANTRCV_30_GENERICCAN_USE_ENDCHECKWAKEUP == STD_ON)
            /* #410 If NO valid wake-up events detected. Inform EcuM that the check-wake-up-procedure has completed. (only SPI-interface) */
            EcuM_EndCheckWakeup( CanTrcvCfg_GetWakeupSource(CanTrcvIndex) ); /* \trace SPEC-40535 */
#   endif /* CANTRCV_30_GENERICCAN_USE_ENDCHECKWAKEUP == STD_ON */
          }
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON) 
          /* For synchronous SPI an external wake-up reason indicates that a wake-up occurred. */
          else if (CANTRCV_30_GENERICCAN_IS_EXTERNAL_WU_REASON(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_BY_PIN */
          {
            /* Wake-up detected - Return E_OK */
            returnVal = E_OK;
          }
          else
          {
            returnVal = E_NOT_OK;  /* No wake-up detected */
          }
#   endif /* CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON */
#  endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */

          /* #500 If any wake-up event detected determine the wake-up reason (e.g BY_BUS, BY_PIN). (only DIO-interface or SBC-interface) */
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
          if (returnVal == E_OK)
          {
            if (CanTrcv_30_GenericCan_StateWupIsBus(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags))
            {
              CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_BUS; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
              CanTrcv_30_GenericCan_ReportWakeup(CanTrcvIndex);

              returnVal = E_OK; /* Wakeup detected */
            }
            else if (CanTrcv_30_GenericCan_StateWupIsPin(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_BY_PIN */
            {
              CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_PIN; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
              CanTrcv_30_GenericCan_ReportWakeup(CanTrcvIndex);

              returnVal = E_OK; /* Wakeup detected */
            }
            else
            {
              returnVal = E_NOT_OK; /* No Wakeup detected */
            }
          }
#  endif /* CANTRCV_30_GENERICCAN_INTERFACE_DIO || CANTRCV_30_GENERICCAN_INTERFACE_SBC_COMMENT*/

        }

        CanTrcv_LeaveCritical();

      }
    }
#else
    returnVal = E_NOT_OK;
#endif /* CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED */
  }
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_CHECKWAKEUP_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
#endif  
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF) && (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_OFF)
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(CanTrcvIndex); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
#endif
  return returnVal;
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */


#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#  if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON)
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CbWakeupByBusIndication
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CbWakeupByBusIndication(uint8 CanTrcvIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  CanTrcv_30_GenericCan_TrcvWakeupReasonType localWakeUpReason = CANTRCV_30_GENERICCAN_WU_ERROR;

# if (CANTRCV_30_GENERICCAN_USE_ENDCHECKWAKEUP == STD_ON)
  boolean wakeupNotified = FALSE;
# endif

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Determine the wake-up reason. (critical section used) */
  CanTrcv_EnterCritical();

# if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
  if ((CanTrcv_30_GenericCan_Prob[CanTrcvIndex].pnAvailable == TRUE) && (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].isPnEnabled == TRUE) &&
      (CanTrcv_30_GenericCan_StateErrIsSys(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)) )
  {
    localWakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_SYSERR;
  }
  else
# endif
  if ((CanTrcv_30_GenericCan_StateWupIsBus(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)))
  {
    localWakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_BUS;
  }
  else if ((CanTrcv_30_GenericCan_StateWupIsPin(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags))) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_BY_PIN */
  {
    localWakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_PIN;
  }
  else
  {
    /* Nothing. */
  }

  if (localWakeUpReason != CANTRCV_30_GENERICCAN_WU_ERROR)
  {
    /* #200 If a valid wake-up reason was detected, set the wake-up reason. */
    CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = localWakeUpReason; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

# if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
    if (CanTrcv_30_GenericCan_GetOpModeReq(CanTrcvIndex) != 0x0u)
    {
      /* #210 In addition: If the wake-up event was detected during mode transition store this. (only asynchronous SPI-interface) */
      CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpInTransitionDetected = TRUE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#  if (CANTRCV_30_GENERICCAN_USE_ENDCHECKWAKEUP == STD_ON)
      wakeupNotified = TRUE;
#  endif
    }
    else
# endif /* (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) */
    if (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].curOpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL) /* COV_CANTRCV_HL_GENERICCAN_CODECOV_DEFENSIVE_CODING_STYLE */
    {
      /* #220 If transceiver channel is currently in power-down mode (STANDBY / SLEEP) inform EcuM about detected wake-up event. */
      CanTrcv_30_GenericCan_ReportWakeup(CanTrcvIndex);
# if (CANTRCV_30_GENERICCAN_USE_ENDCHECKWAKEUP == STD_ON)
      wakeupNotified = TRUE;
# endif
    }
    else
    {
      /* No report.. */
    }
  } /* (localWakeUpReason != CANTRCV_30_GENERICCAN_WU_ERROR) */

  CanTrcv_30_GenericCan_ClrCbByBusFlagReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */

  CanTrcv_LeaveCritical();

# if (CANTRCV_30_GENERICCAN_USE_ENDCHECKWAKEUP == STD_ON)
  if (wakeupNotified == FALSE)
  {
    /* #300 If NO valid wake-up reason detected, inform EcuM that the check-wake-up-procedure has completed. */
    EcuM_EndCheckWakeup( CanTrcvCfg_GetWakeupSource(CanTrcvIndex) ); /* \trace SPEC-40535 */
  }
# endif
}
#  endif /* (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) */
#endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */


#if (CANTRCV_30_GENERICCAN_GET_VERSION_INFO == STD_ON) /* \trace SPEC-15105 */
/* \trace SPEC-19953 */
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetVersionInfo
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) VersionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter VersionInfo is valid. */
  if(VersionInfo == NULL_PTR)
  {
    errorId = CANTRCV_30_GENERICCAN_E_PARAM_POINTER;
  }
  else
# endif /* CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* \trace SPEC-14986, SPEC-15032 */
    /* #100 Retrieve the version information: VENDOR_ID, MODULE_ID, SW_MAJOR-, SW_MINOR- and SW_PATCH-version. */
    VersionInfo->vendorID = CANTRCV_30_GENERICCAN_VENDOR_ID; /* SBSW_CANTRCV_HL_GENERICCAN_2 */
    VersionInfo->moduleID = CANTRCV_30_GENERICCAN_MODULE_ID; /* SBSW_CANTRCV_HL_GENERICCAN_2 */

    VersionInfo->sw_major_version = CANTRCV_30_GENERICCAN_SW_MAJOR_VERSION; /* SBSW_CANTRCV_HL_GENERICCAN_2 */
    VersionInfo->sw_minor_version = CANTRCV_30_GENERICCAN_SW_MINOR_VERSION; /* SBSW_CANTRCV_HL_GENERICCAN_2 */
    VersionInfo->sw_patch_version = CANTRCV_30_GENERICCAN_SW_PATCH_VERSION; /* SBSW_CANTRCV_HL_GENERICCAN_2 */
  }
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_GETVERSIONINFO_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
#endif 
}
#endif /* CANTRCV_30_GENERICCAN_GET_VERSION_INFO == STD_ON */


#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_TrcvModeIndication
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_TrcvModeIndication(uint8 CanTrcvIndex, boolean modeSet)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 requestedMode = 0; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ----------------------------------------------- */
  if (modeSet == TRUE)
  {
    /* #100 If mode execution succeeded, adapt the internal mode state variable. */
    /* Get the current requested mode */
    requestedMode = CanTrcv_30_GenericCan_GetOpModeReq(CanTrcvIndex);

    switch (requestedMode)
    {
    case CanTrcv_30_GenericCan_Req_OpModeNormal:
      CanTrcv_30_GenericCan_Prob[CanTrcvIndex].curOpMode = CANTRCV_30_GENERICCAN_OP_MODE_NORMAL; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
      break;

    case CanTrcv_30_GenericCan_Req_OpModeStandby:
      CanTrcv_30_GenericCan_Prob[CanTrcvIndex].curOpMode = CANTRCV_30_GENERICCAN_OP_MODE_STANDBY; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
      break;

    case CanTrcv_30_GenericCan_Req_OpModeSleep:
      CanTrcv_30_GenericCan_Prob[CanTrcvIndex].curOpMode = CANTRCV_30_GENERICCAN_OP_MODE_SLEEP; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
      break;

    default: /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_DEFAULT */
      requestedMode = 0xffffffffu; /* PRQA S 2983 */ /* MD_MSR_RetVal */ /* Just suppress the wakeup notification */
      /* Indication without any request... old mode should be still valid */
      break;
    }
  }

  /* #110 Release mode request pending bit. After this new requests are accepted. (critical section is used) */
  CanTrcv_EnterCritical();
  CanTrcv_30_GenericCan_ClrOpModeReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */
  CanTrcv_LeaveCritical();


  if (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].isInit == CANTRCV_30_GENERICCAN_IS_INIT)
  {
    /* #120 If transceiver channel is initialized, inform the CanIf about successful mode change. */
    CanIf_30_GenericCan_TrcvModeIndication(CanTrcvIndex, CanTrcv_30_GenericCan_Prob[CanTrcvIndex].curOpMode); /* \trace SPEC-20006 */

# if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
#  if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON)
    if ((requestedMode != 0xffffffffu) &&
        (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpInTransitionDetected == TRUE)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_DEFAULT */
    {
      /* #130 If in addition a wake-up event occurred during mode transition report the detected wake-up event. (only asynchronous SPI-interface) */
      CanTrcv_30_GenericCan_ReportWakeup(CanTrcvIndex);
    }

    /* #140 Clear any wake-up event that occurred during mode transition. (only asynchronous SPI-interface) */
    CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpInTransitionDetected = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#  endif /* CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON */
# endif /* CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF */
  }
}
#endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */


#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
/* \trace SPEC-19989 */
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetTrcvSystemData
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetTrcvSystemData(uint8 CanTrcvIndex, P2VAR(uint32, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) TrcvSysData )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* \trace SPEC-19942 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active */
  else if(!CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
  {
    /* \trace SPEC-19942 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex))
  {
    /* \trace SPEC-15025 */
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  /* #40 Check if parameter TrcvSysData is valid. */
  else if(TrcvSysData == NULL_PTR)
  {
    /* \trace SPEC-15098*/
    errorId = CANTRCV_30_GENERICCAN_E_PARAM_POINTER;
  }
  else
# endif /* CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. */
    if(CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
# endif
    {
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#  if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
      if (CanTrcv_30_GenericCan_IsReqFlagPnd(CanTrcvIndex)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_SPI_BUSY */
#  else
      if (CanTrcv_30_GenericCan_GetSyncSpiTxSem(CanTrcvIndex) == FALSE)
#  endif
      {
        /* #200 SPI is busy: Do NOT execute current request. (only SPI-interface) */
      }
      else
# endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */
      {
        uint32          regVal;



        /* #300 SPI is NOT busy: Read the value of the diagnostic register. (critical section used) */
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
        CanTrcv_EnterCritical();
# else
#  if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
        CanTrcv_EnterCritical();
#  endif /* CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON */
# endif

        /* \trace SPEC-15026 */
        /*  ReadDiagRegister
            -----------------
            Fill only if CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON.
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*
            When using DIO interface
            ========================
              Not supported. Leave empty.
        */
        #endif
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        /*  When using SPI interface
            ========================
              Save the current state of the diag registers of the transceiver hardware to regVal. This must be done synchronously.
              
              Set retVal = E_OK if the read data are valid.
              
              Restrictions:
              - DO NOT loose any pending flag
              - DO NOT access asynchronously
            
              Example:
              retVal = CanTrcv_30_GenericCan_RequestDiagSet(CanTrcvIndex, TRUE); // Synchronous request for statusFlags
              
              regVal = ( (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags.DIAG2 << 8) | 
                         (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags.DIAG1)
                       );
                        
              
        */
        #endif
        <Your_Trcv_Code>

# if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
        CanTrcv_LeaveCritical();
# else
#  if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
        CanTrcv_LeaveCritical();
#  else
        CanTrcv_30_GenericCan_ReleaseSyncSpiTxSem(CanTrcvIndex);
#  endif /* CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON */
# endif

        if (retVal == E_OK)
        {
          *TrcvSysData = regVal; /* SBSW_CANTRCV_HL_GENERICCAN_2 */
        }
        else
        {
          /* retVal is already E_NOT_OK, no need to overwrite it here */
          /* \trace SPEC-19997 */
          /* #310 If reading of diagnostic register failed, report error. */
          errorId = CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL;
        }
      }
    } /* IsChannelUsed */
  }
  
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_GETTRCVSYSTEMDATA_ID, errorId);
  }
# else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
# endif  
  
  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif /* PN or SBC */


#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ClearTrcvWufFlag
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ClearTrcvWufFlag(uint8 CanTrcvIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* \trace SPEC-19970 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active. */
  else if(!CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
  {
    /* \trace SPEC-19970 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex))
  {
    /* \trace SPEC-15018 */
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */

# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. */
    if( CanTrcvCfg_IsChannelUsed(CanTrcvIndex) )
# endif
    {

      CanTrcv_EnterCritical();

      /* #110 Clear wake-up reason and all wake-up events detected during initialization. */
      CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = CANTRCV_30_GENERICCAN_WU_ERROR; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

      /* #120 Store the current request. It will be cleared within indication. */
      CanTrcv_30_GenericCan_SetClrWufFlagReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */

      /* \trace SPEC-14997 */
      #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
      /*  ClearTrcvWufFlag
          ----------------
          Fill only if CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON.
          
          Set retVal = E_OK if the request was successfully executed.
      */
      /*  When using DIO interface:
          =========================
            Not supported. Leave empty.
      */
      /*  When using SPI interface:
          =========================
            Request the underlying CanTrcv to clear the Wakeup flags.  Set retval = E_OK if the request was successfully acknowledged.
          
            Depending on the CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS this request has to be done
            either synchronously or asynchronously.
          
            Restrictions:
            - When using asynchronous access the request to read status flags may be interrupted
              by any other API / request. The driver MUST handle this.
            - DO NOT loose any request
            - ALWAYS call CanTrcv_30_GenericCan_ClearTrcvWufFlagIndication as soon as status flags
              have been read. 
      
      */
      #endif
      <Your_Trcv_Code>

      if (retVal == E_NOT_OK)
      {
        /* #130 If unable to clear the wake-up flag, clear this request. */
        CanTrcv_30_GenericCan_ClrClrWufFlagReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */
      }

      CanTrcv_LeaveCritical();

      /* The indication is called from the SPI-callback */

      if (retVal == E_NOT_OK)
      {
        /* retVal is already E_NOT_OK, no need to overwrite it here */
        /* \trace SPEC-19943 */
        /* #140 If unable to clear the wake-up flag, report error. */
        errorId = CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL;
      }
    } /* IsChannelUsed */
  }
  
#if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_CLEARTRCVWUFFLAG_ID, errorId);
  }
#else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
#endif  
  
  return retVal;
}


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ClearTrcvWufFlagIndication
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ClearTrcvWufFlagIndication(uint8 CanTrcvIndex)
{
  /* #100 Clear the request to clear the wake-up flag in HW. */
  CanTrcv_EnterCritical();
  CanTrcv_30_GenericCan_ClrClrWufFlagReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */
  CanTrcv_LeaveCritical();

  /* #110 Inform CanIf about successful completion of request. */
  CanIf_30_GenericCan_ClearTrcvWufFlagIndication(CanTrcvIndex); /* \trace SPEC-19983 */
}


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ReadTrcvTimeoutFlag
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ReadTrcvTimeoutFlag(uint8 CanTrcvIndex, P2VAR(CanTrcv_30_GenericCan_TrcvFlagStateType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) FlagState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* \trace SPEC-19984 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active. */
  else if(!CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
  {
    /* \trace SPEC-19984 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK; 
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex))
  {
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  /* #40 Check if parameter FlagState is valid. */
  else if(FlagState == NULL_PTR)
  {
    /* \trace SPEC-14943 */
    errorId = CANTRCV_30_GENERICCAN_E_PARAM_POINTER;
  }
  else
# endif /* (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON) */
  /* ----- Implementation ----------------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
  /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. */
  if(CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
# endif
  {
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
    if (CanTrcv_30_GenericCan_IsReqFlagPnd(CanTrcvIndex)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_SPI_BUSY */
#   else
    if (CanTrcv_30_GenericCan_GetSyncSpiTxSem(CanTrcvIndex) == FALSE)
#   endif
    {
      /* #200 SPI is busy: Do NOT execute current request. (only SPI-interface) */
    }
    else
#  endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */
    {
      /* #300 SPI is NOT busy: Read the value of the time-out flag. (critical section is used) */

#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
      CanTrcv_EnterCritical();
#  else
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
      CanTrcv_EnterCritical();
#   endif
#  endif

      /* \trace SPEC-20010 */
      /*  ReadTrcvTimeoutFlag
          -------------------
          Fill only if CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON.
          
          Set retVal = E_OK if the request was successfully executed.
      */
      #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
      /*  When using DIO interface:
          =========================
            Not supported. Leave empty.
      */
      #endif
      #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
      /*  When using SPI interface:
          =========================
            Read the timeout flag from the underlying hardware. Set retVal = E_OK if the request has completed.
          
            This request must be done always synchronously.
          
            Restrictions:
            - DO NOT loose any request
      */
      #endif
      <Your_Trcv_Code>
      
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
      CanTrcv_LeaveCritical();
#  else
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
      CanTrcv_LeaveCritical();
#   else
      CanTrcv_30_GenericCan_ReleaseSyncSpiTxSem(CanTrcvIndex);
#   endif /* CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON */
#  endif

      if (retVal == E_NOT_OK) /* PRQA S 2991,2995 */ /* MD_CanTrcv_30_GenericCan_14.3 */ /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_HW_SPECIFIC */
      {
        /* #310 If reading of time-out flag failed, report error. */
        errorId = CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL;
      }
    }
  } /* IsChannelUsed */

# if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR) /* PRQA S 2991,2995 */ /* MD_CanTrcv_30_GenericCan_14.3 */ /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_HW_SPECIFIC */
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_READTRCVTIMEOUTFLAG_ID, errorId);
  }
# else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
# endif  

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ClearTrcvTimeoutFlag
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ClearTrcvTimeoutFlag(uint8 CanTrcvIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* \trace SPEC-19948 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active. */
  else if(!CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
  {
    /* \trace SPEC-19948 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex))
  {
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
# endif
  /* ----- Implementation ----------------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
  /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. */
  if(CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
# endif
  {
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
    if (CanTrcv_30_GenericCan_IsReqFlagPnd(CanTrcvIndex)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_SPI_BUSY */
#   else
    if (CanTrcv_30_GenericCan_GetSyncSpiTxSem(CanTrcvIndex) == FALSE)
#   endif
    {
      /* #200 SPI is busy: Do NOT execute current request. (only SPI-interface) */
    }
    else
#  endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */
    {
      /* #300 SPI is NOT busy: Clear the value of the time-out flag. (critical section is used) */
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
      CanTrcv_EnterCritical();
#  else
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
      CanTrcv_EnterCritical();
#   endif /* CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON */
#  endif

      /* \trace SPEC-19996 */
      /*  ReadTrcvTimeoutFlag
          -------------------
          Fill only if CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON.
          
          Set retVal = E_OK if the request was successfully executed.
      */
      #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
      /*  When using DIO interface:
          =========================
            Not supported. Leave empty.
      */
      #endif
      #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
      /*  When using SPI interface:
          =========================
            Clear the timeout flag from the underlying hardware. Set retVal = E_OK if the request has completed.
          
            This request must be done always synchronously.
          
            Restrictions:
            - DO NOT loose any request
      */
      #endif
      <Your_Trcv_Code>

#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
      CanTrcv_LeaveCritical();
#  else
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
      CanTrcv_LeaveCritical();
#   else
      CanTrcv_30_GenericCan_ReleaseSyncSpiTxSem(CanTrcvIndex);
#   endif /* CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON */
#  endif

      if (retVal == E_NOT_OK) /* PRQA S 2991,2995 */ /* MD_CanTrcv_30_GenericCan_14.3 */ /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_HW_SPECIFIC */
      {
        /* #310 If clearing of the time-out flag failed, report error. */
        errorId = CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL;
      }
    }
  } /* IsChannelUsed */

# if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR) /* PRQA S 2991,2995 */ /* MD_CanTrcv_30_GenericCan_14.3 */ /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_HW_SPECIFIC */
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_CLEARTRCVTIMEOUTFLAG_ID, errorId);
  }
# else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
# endif  

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ReadTrcvSilenceFlag
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ReadTrcvSilenceFlag(uint8 CanTrcvIndex, P2VAR(CanTrcv_30_GenericCan_TrcvFlagStateType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) FlagState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* \trace SPEC-20000 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active. */
  else if(!CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
  {
    /* \trace SPEC-20000 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex))
  {
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  /* #40 Check if parameter FlagState is valid. */
  else if(FlagState == NULL_PTR)
  {
    /* \trace SPEC-15062*/
    errorId = CANTRCV_30_GENERICCAN_E_PARAM_POINTER;
  }
  else
# endif
  /* ----- Implementation ----------------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
  /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. */
  if(CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
# endif
  {
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
    if (CanTrcv_30_GenericCan_IsReqFlagPnd(CanTrcvIndex)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_SPI_BUSY */
#   else
    if (CanTrcv_30_GenericCan_GetSyncSpiTxSem(CanTrcvIndex) == FALSE)
#   endif
    {
      /* #200 SPI is busy: Do NOT execute current request. (only SPI-interface) */
    }
    else
#  endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */
    {
      /* #300 SPI is NOT busy: Read the value of the silence flag. (critical section used) */
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
      CanTrcv_EnterCritical();
#  else
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
      CanTrcv_EnterCritical();
#   endif /* CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON */
#  endif

      /* \trace SPEC-19999 */
      /*  ReadTrcvTimeoutFlag
          -------------------
          Fill only if CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON.
      */
      #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
      /*  When using DIO interface:
          =========================
            Not supported. Leave empty.
      */
      #endif
      #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
      /*  When using SPI interface:
          =========================
            Read the silence flag from the underlying hardware. Set retVal = E_OK if the request has completed.
          
            This request must be done always synchronously.
          
            Restrictions:
            - DO NOT loose any request
      */
      #endif
      <Your_Trcv_Code>


#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
      CanTrcv_LeaveCritical();
#  else
#   if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON)
      CanTrcv_LeaveCritical();
#   else
      CanTrcv_30_GenericCan_ReleaseSyncSpiTxSem(CanTrcvIndex);
#   endif /* CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON */
#  endif

      if (retVal == E_NOT_OK) /* PRQA S 2991,2995 */ /* MD_CanTrcv_30_GenericCan_14.3 */ /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_HW_SPECIFIC */
      {
        /* #310 If reading of the silence flag failed, report error. */
        errorId = CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL;
      }
    }
  } /* IsChannelUsed */

# if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR) /* PRQA S 2991,2995 */ /* MD_CanTrcv_30_GenericCan_14.3 */ /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_HW_SPECIFIC */
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_READTRCVSILENCEFLAG_ID, errorId);
  }
# else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
# endif  

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CheckWakeFlag
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CheckWakeFlag(uint8 CanTrcvIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = CANTRCV_30_GENERICCAN_E_NO_ERROR;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter CanTrcvIndex is valid. */
  if(CanTrcvIndex >= CANTRCV_30_GENERICCAN_MAX_CHANNEL)
  {
    /* \trace SPEC-20005 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #20 Check if transceiver channel (CanTrcvIndex) is active. */
  else if(!CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
  {
    /* \trace SPEC-20005 */
    errorId = CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK;
  }
  /* #30 Check if transceiver channel (CanTrcvIndex) is initialized. */
  else if(!CANTRCV_30_GENERICCAN_IS_INITIALIZED(CanTrcvIndex))
  {
    errorId = CANTRCV_30_GENERICCAN_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do execute the request only if transceiver channel (CanTrcvIndex) is active. (critical section used) */
    if(CanTrcvCfg_IsChannelUsed(CanTrcvIndex))
# endif
    {
      CanTrcv_EnterCritical();

      /* \trace SPEC-20015 */
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#  if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
      if ((CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlagsNew == TRUE) &&
          (CanTrcv_30_GenericCan_StateWupIsValid(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags))) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_BY_PIN */
      {
        /* #200 If most current status flags are available, evaluate them. (only asynchronous SPI-interface) */

        /* There are still unread status flags. As a request is now pending,
         * Main will handle these flags any more. Thus the flags have to be
         * considered as "valid" and CheckWakeFlag indication is called
         * directly from this context.
         */
        
        /* Clear the statusFlagNew bit */
        CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlagsNew = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

        /* Call the indication */
        CanTrcv_30_GenericCan_CheckWakeFlagIndication(CanTrcvIndex);

        /* This was a valid operation... */
        retVal = E_OK;
      }
      else
#  endif
# endif
      {
        /* #300 Get and evaluate status flags. (synchronous SPI-interface) Request most current status flags. (asynchronous SPI-interface) */

        /* Store the request. it will be reset on indication */
        CanTrcv_30_GenericCan_SetChkWakFlagReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */

        /* This code should read and clear the wake flags within one step */
        /*  CheckWakeFlag
            -------------
            Fill only if CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON.
            
            Issue a request to read the statusFlags. See also GetStatusFlags. 
            
            Set retVal = E_OK if request has been acknowledged.
        */
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
        /*  When using DIO interface:
            =========================
              Not supported. Leave empty.
        */
        #endif
        #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        /*  When using SPI interface:
            =========================    
              Restrictions:
              - When using asynchronous access the request to read status flags may be interrupted
                by any other API / request. The driver MUST handle this.
              - DO NOT loose any request
              - ALWAYS call the following indications in this order as soon as the statusFlags have been read:
                1. CanTrcv_30_GenericCan_GetStatusFlagsIndication
                2. CanTrcv_30_GenericCan_CheckWakeFlagIndication
              - If the same flags are polled in MainFunction ensure that the polled flags are
                processed even if a new request (e.g. from interrupt context) is pending
              
              Example:
              // This function will try to request the status flags. If the driver is currently busy with e.g. a mode change
              // the request will be held pending and processed as soon as the driver is ready again.
              
              // As soon as the flags are ready GetStatusFlagsIndication and CheckWakeFlagIndication are being called
              retval = CanTrcv_30_GenericCan_RequestWakeFlags(CanTrcvIndex);
        */
        #endif
        <Your_Trcv_Code>

        if (retVal == E_NOT_OK)
        {
          /* No indication will occur. Clear request flag */
          CanTrcv_30_GenericCan_ClrChkWakFlagReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */
        }
      }
      CanTrcv_LeaveCritical();

      if (retVal == E_NOT_OK)
      {
        /* #310 If getting / requesting of status flags fails, report error. */
        errorId = CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL;
      }
      /* --> CheckWakeFlagIndication will be called from the SPI callback */
    } /* IsChannelUsed */
  } /* DET checks passed */

# if (CANTRCV_30_GENERICCAN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANTRCV_30_GENERICCAN_E_NO_ERROR)
  {
    CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_CHECKWAKEFLAG_ID, errorId);
  }
# else
  CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(errorId); /* PRQA S 1338,2983,3112 */ /* MD_MSR_DummyStmt */
# endif  
  
  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CheckWakeFlagIndication
 **********************************************************************************************************************/
/*! Internal comment removed. *
 *
 *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CheckWakeFlagIndication(uint8 CanTrcvIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  CanTrcv_30_GenericCan_TrcvWakeupReasonType localWakeUpReason = CANTRCV_30_GENERICCAN_WU_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  CanTrcv_EnterCritical();


  /* #100 Determine the wake-up reason: BY_SYSERR (only PN), BY_BUS or BY_PIN. (critical section used) */
  if ((CanTrcv_30_GenericCan_Prob[CanTrcvIndex].pnAvailable == TRUE) && (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].isPnEnabled == TRUE) &&
      (CanTrcv_30_GenericCan_StateErrIsSys(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)))
  {
    localWakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_SYSERR;
  }
  else if ((CanTrcv_30_GenericCan_StateWupIsBus(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags)))
  {
    localWakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_BUS;
  }
  else if ((CanTrcv_30_GenericCan_StateWupIsPin(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags))) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_BY_PIN */
  {
    localWakeUpReason = CANTRCV_30_GENERICCAN_WU_BY_PIN;
  }
  else
  {
    /* Nothing. */
  }

  if (localWakeUpReason != CANTRCV_30_GENERICCAN_WU_ERROR)
  {
    /* #110 If any wake-up event was detected, store the wake-up reason and inform the EcuM. */
    CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason = localWakeUpReason; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
    CanTrcv_30_GenericCan_ReportWakeup(CanTrcvIndex);
  }

  /* Release request */
  CanTrcv_30_GenericCan_ClrChkWakFlagReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */

  CanTrcv_LeaveCritical();

  /* \trace SPEC-19979 */ 
  /* #120 Inform CanIf about successful execution of request to check for wake-up event(s). */
  CanIf_30_GenericCan_CheckTrcvWakeFlagIndication(CanTrcvIndex);
}
# endif /* CANTRCV_30_GENERICCAN_HW_PN_SUPPORT */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_MainFunction
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least index;
  
  /* ----- Implementation ----------------------------------------------- */
  /* \trace SPEC-15067, SPEC-15085 */
  /* #100 Run through all transceiver channels. */
  for(index = 0; index < CANTRCV_30_GENERICCAN_MAX_CHANNEL; ++index)
  {
    if (CanTrcvCfg_IsChannelUsed(index))
    {
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
      if(CanTrcv_30_GenericCan_Prob[index].isInit == CANTRCV_30_GENERICCAN_IS_INIT)
#endif
      {
        /* #200 Execute only if transceiver channel is active and initialized. */
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO) 
        CanTrcv_30_GenericCan_TrcvModeType currentOpMode;

        (void)CanTrcv_30_GenericCan_GetOpMode((uint8)index, &currentOpMode); /* SBSW_CANTRCV_HL_GENERICCAN_3 */

        /* #300 Execute only if transceiver channel is in power-down mode (STANDBY / SLEEP). (only DIO- or SBC-interface) */
        if (currentOpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL)
#endif
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#  if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
        if (CanTrcv_30_GenericCan_IsReqFlagPnd(index))
        {
          /* #400 If any request flag is pending execute the pending request(s). (only asynchronous SPI-interface) */
          /*  MainFunctionRequestHandler
              --------------------------
          */
          #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
          /*  When using DIO interface:
              =========================
                Not supported. Leave empty.
          */
          #endif
          #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
          /*  When using SPI interface:
              =========================    
                Process any pending request for OpMode or StatusFlags here. The driver should use 
                CanTrcv_30_GenericCan_Get*Req to identify what kind of request is currently pending and call the dedicated indications
                as soon as the request has completed.
          */
          #endif
          <Your_Trcv_Code>
        }
        else
#  endif /* (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) */
        /* #410 Execute only if transceiver channel is in power-down mode (STANDBY / SLEEP). (only asynchronous SPI-interface) */
        if (CanTrcv_30_GenericCan_Prob[index].curOpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL)
#endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */
        {
#if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) && (CANTRCV_30_GENERICCAN_GENERAL_WAKE_UP_SUPPORT == CANTRCV_30_GENERICCAN_WAKEUP_POLLING) 
          if (CanTrcvCfg_IsWakeupByBusUsed(index)) /* COV_CANTRCV_HL_GENERICCAN_CODECOV_IS_WU_BY_BUS_USED */
          {
# if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
            /* #420 Do NOT do anything if any request is still pending. (only SPI-interface) */
            if (!CanTrcv_30_GenericCan_IsReqFlagPnd(index)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_SPI_BUSY */
# endif /* (CANTRCV_30_GENERICCAN_INTERFACE_SPI) */
            {
              /* #210 Transceiver channel is in power-down mode (STANDBY / SLEEP) and has wake-up support enabled: Check if a wake-up event occurred. (critical section used) */
              Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

              CanTrcv_TrcvWakeupReasonType wakeupReason = CANTRCV_30_GENERICCAN_WU_ERROR;

              CanTrcv_EnterCritical();
              /* #430 Determine whether a wake-up event occurred or NOT from status flags read via SPI. (only SPI- / SBC-interface) */
              /* Check for status bits */
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
              if (CanTrcv_30_GenericCan_Prob[index].statusFlagsNew == FALSE)
#  endif
              {
                 /* #440 If NO most current status flags are available, request them via SPI. (only SPI-interface) */

                 /* No request pending.. its safe to request diagnostic registers here..
                 * unread data will be marked as dirty. This request may be
                 * interrupted by any other request..
                 */
                /*  GetStatusFlags
                    --------------
                    Request all status flags (SYSERR, POR, WUP, WUF, LWUP, ...) from the underlying transceiver hardware. 
                    Set retval = E_OK if the request was successful. The flags are evaluated later with the CanTrcv_30_GenericCan_State*
                    macros.
                    
                */
                #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
                /*  When using DIO interface:
                    =========================
                      Store the current level of the DIO pins to statusFlags. 
                      
                      Example TJA1041:
                        statusFlags.PinERR = Dio_ReadChannel(CanTrcv_30_GenericCan_GetERROfDioConfiguration(index));
                        statusFlags.PinSTB = Dio_ReadChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(index));
                        retval = E_OK;
                */
                #endif
                #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
                /*  When using a SPI interface:
                    ===========================
                      Depending on CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS, this should be either done asynchronously or synchronously.
                    
                      Restrictions:
                        - If the driver is not initialized yet the access MUST be done synchronously (e.g. in context
                          of CanTrcv_30_GenericCan_Init())
                        - When using asynchronous access the request to read status flags may be interrupted
                          by any other API / request. The driver MUST handle this.
                        - DO NOT loose any request
                        - ALWAYS call CanTrcv_30_GenericCan_GetStatusFlagsIndication as soon as status flags
                          have been read. 
                        - If the same flags are polled in MainFunction ensure that the polled flags are
                          processed even if a new request (e.g. from interrupt context) is pending
                        - ALWAYS call the pending indications for an active request
                            - GetCbByBusFlagReq == TRUE -> CanTrcv_30_GenericCan_CbWakeupByBusIndication
                    
                      Example:
                        // This function will try to request the status flags. If the driver is currently busy with e.g. a mode change
                        // the request will be held pending and processed as soon as the driver is ready again.
                    
                        // As soon as the flags are ready, the function GetStatusFlagsIndication is beeing called.
                        retval = CanTrcv_30_GenericCan_RequestStatusFlags(index, FALSE);
                */
                #endif
                <Your_Trcv_Code>
              }
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
              else
#  else
              if (retval == E_OK)
#  endif
              {
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
                /* #450 If most current status flags are available mark them as already used. (only asynchronous SPI-interface) */
                CanTrcv_30_GenericCan_Prob[index].statusFlagsNew = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
#  endif

                /* #460 If most current flags are available process them and if any wake-up event occurred determine the wake-up reason (BY_SYSERR (only PN), BY_BUS or BY_PIN). (only SPI- or SBC-interface) */
#  if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
                if ((CanTrcv_30_GenericCan_Prob[index].pnAvailable == TRUE) && (CanTrcv_30_GenericCan_Prob[index].isPnEnabled == TRUE) &&
                    (CanTrcv_30_GenericCan_StateErrIsSys(CanTrcv_30_GenericCan_Prob[index].statusFlags)))
                {
                  wakeupReason = CANTRCV_30_GENERICCAN_WU_BY_SYSERR;
                }
                else
#  endif
                if (CanTrcv_30_GenericCan_StateWupIsBus(CanTrcv_30_GenericCan_Prob[index].statusFlags))
                {
                  wakeupReason = CANTRCV_30_GENERICCAN_WU_BY_BUS;
                }
                else if (CanTrcv_30_GenericCan_StateWupIsPin(CanTrcv_30_GenericCan_Prob[index].statusFlags)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_BY_PIN */
                {
                  wakeupReason = CANTRCV_30_GENERICCAN_WU_BY_PIN;
                }
                else
                {
                  /* Nothing */
                }

                retval = E_OK;
              }

              if (retval == E_NOT_OK)
              {
                /* #470 If the check for wake-up events fails, report error. (only SPI- / SBC-interface) */
                CanTrcv_ReportErrorNoTrcvControl(CANTRCV_30_GENERICCAN_MAINFUNCTION_ID);
              }

              if (wakeupReason != CANTRCV_30_GENERICCAN_WU_ERROR)
              {
                /* #500 If any wake-up event detected, store the wake-up event. */
                CanTrcv_30_GenericCan_Prob[index].wakeUpReason = wakeupReason; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

                if (CanTrcv_30_GenericCan_Prob[index].wakeUpReport == CANTRCV_30_GENERICCAN_WU_ENABLE)
                {
                  /* #510 If any wake-up event detected and wake-up detection is enabled, inform the EcuM to check for wake-up event(s). */
                  EcuM_CheckWakeup( CanTrcvCfg_GetWakeupSource(index) );
                }
              }

#  if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_OFF)
              CANTRCV_30_GENERICCAN_DUMMY_STATEMENT(retval); /* PRQA S 1338,2982,2983,3112 */ /* MD_MSR_DummyStmt */
#  endif
              CanTrcv_LeaveCritical();
            } /* (!CanTrcv_30_GenericCan_IsReqFlagPnd(index)) */
          }
# endif /* (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) */
        } /* CanTrcv_30_GenericCan_Prob[index].curOpMode != CANTRCV_30_GENERICCAN_OP_MODE_NORMAL */
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
#  if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
        else
        {
          /* Required to avoid MISRA violation */
        }
#  endif /* (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) */
#endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */
      } /* CanTrcv_30_GenericCan_Prob[index].isInit != CANTRCV_30_GENERICCAN_IS_INIT */
    } /*  CanTrcvCfg_IsChannelUsed(index)  */
  } /* for ... */
} /* PRQA S 6030,6080 */ /* MD_MSR_STCYC,MD_MSR_STMIF */


# if (CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON)
/* SPEC-2890644 */
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_MainFunctionDiagnostics
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_MainFunctionDiagnostics(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least index; /* ESCAN00091616 */
  
  /* ----- Implementation ----------------------------------------------- */
  /* \trace SPEC-15088, SPEC-15073 */
  /* #100 Run through all transceiver channels. */
  for(index = 0; index < CANTRCV_30_GENERICCAN_MAX_CHANNEL; ++index)
  {
    if (CanTrcvCfg_IsChannelUsed(index))
    {
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
      if (CanTrcv_30_GenericCan_Prob[index].isInit == CANTRCV_30_GENERICCAN_IS_INIT)
#  endif
      {
        /* #200 Do execute only if transceiver channel is active and initialized. */
#  if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
        if (!CanTrcv_30_GenericCan_IsReqFlagPnd(index)) /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_SPI_BUSY */
#  endif
        {
          /* #210 Execute only if SPI is NOT busy. (only SPI-interface) */
          Std_ReturnType retval = E_NOT_OK;
#  if (CANTRCV_30_GENERICCAN_PROD_ERROR_DETECT == STD_ON)
          CanTrcv_EnterCritical();

          /* #220 Get diagnostics flags from HW. (critical section used) */
#   if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
          /* #230 Check if most current diagnostic flags are available. (only asynchronous SPI-interface) */
          if (CanTrcv_30_GenericCan_Prob[index].statusFlagsRdy == FALSE)
#   endif
          {
            /* UREQ00014985 */
            /*  GetStatusFlags
                --------------
                Request all status flags (SYSERR, POR, WUP, WUF, LWUP, ...) from the underlying transceiver hardware. 
                Set retval = E_OK if the request was successful. The flags are evaluated later with the CanTrcv_30_GenericCan_State*
                macros.
                
            */
            #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
            /*  When using DIO interface:
                =========================
                  Store the current level of the DIO pins to statusFlags. 
                  
                  Example TJA1041:
                    statusFlags.PinERR = Dio_ReadChannel(CanTrcv_30_GenericCan_GetERROfDioConfiguration(index));
                    statusFlags.PinSTB = Dio_ReadChannel(CanTrcv_30_GenericCan_GetSTBOfDioConfiguration(index));
                    retval = E_OK;
            */
            #endif
            #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
            /*  When using a SPI interface:
                ===========================
                  Depending on CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS, this should be either done asynchronously or synchronously.
                
                  Restrictions:
                    - If the driver is not initialized yet the access MUST be done synchronously (e.g. in context
                      of CanTrcv_30_GenericCan_Init())
                    - When using asynchronous access the request to read status flags may be interrupted
                      by any other API / request. The driver MUST handle this.
                    - DO NOT loose any request
                    - ALWAYS call CanTrcv_30_GenericCan_GetStatusFlagsIndication as soon as status flags
                      have been read. 
                    - If the same flags are polled in MainFunction ensure that the polled flags are
                      processed even if a new request (e.g. from interrupt context) is pending
                    - ALWAYS call the pending indications for an active request
                        - GetCbByBusFlagReq == TRUE -> CanTrcv_30_GenericCan_CbWakeupByBusIndication
                
                  Example:
                    // This function will try to request the status flags. If the driver is currently busy with e.g. a mode change
                    // the request will be held pending and processed as soon as the driver is ready again.
                
                    // As soon as the flags are ready, the function GetStatusFlagsIndication is beeing called.
                    retval = CanTrcv_30_GenericCan_RequestStatusFlags(index, FALSE);
            */
            #endif
            <Your_Trcv_Code>

            if (retval == E_NOT_OK)
            {
              /* #240 If getting of diagnostic flags fails, report error. */
              CanTrcv_ReportErrorNoTrcvControl(CANTRCV_30_GENERICCAN_MAINFUNCTIONDIAGNOSTICS);
            }
          }
#   if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) && (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
          else
#   endif
          {
            /* #250 If diagnostic flags are available, check if bus error occurred. */
            if (CanTrcv_30_GenericCan_StateErrIsBus(CanTrcv_30_GenericCan_Prob[index].statusFlags))
            {
              /* #260 If bus error occurred, report error to DEM. */
              CanTrcv_DemReportError(CANTRCV_30_GENERICCAN_E_BUS_ERROR); /* \trace SPEC-15088, SPEC-2890643 */
            }
          }

          CanTrcv_LeaveCritical();
#  endif /* (CANTRCV_30_GENERICCAN_PROD_ERROR_DETECT == STD_ON) */
        }
      }
    } /* IsChannelUsed */
  } /* FOR-loop: run through all CanTrcv channels */
}
# endif /* CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON */


#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_SetPNActivationState
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_SetPNActivationState(CanTrcv_30_GenericCan_PNActivationType ActivationState)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_OK;
  uint8_least index, numberOfConfigurationChanges;

  numberOfConfigurationChanges = 0;

  /* ----- Implementation ----------------------------------------------- */
  
  /* #100 Run through all transceiver channels. */
  for(index=0; index < CANTRCV_30_GENERICCAN_MAX_CHANNEL; index++)
  {
    if (CanTrcvCfg_IsChannelUsed(index))
    {
      /* #200 Do execute only if transceiver channel is active. */
# if (CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON)
      /* #10 Check if transceiver channel is initialized. */
      if (CanTrcv_30_GenericCan_Prob[index].isInit != CANTRCV_30_GENERICCAN_IS_INIT)
      {
        /* \trace SPEC-20019 */
        CanTrcv_DetReportError(CANTRCV_30_GENERICCAN_SETPNACTIVATIONSTATE_ID, CANTRCV_30_GENERICCAN_E_UNINIT);
        retval = E_NOT_OK;
      }
# endif
      if (CanTrcvCfg_IsPnEnabled(index))
      {
        /* #210 Transceiver channel has enabled PN functionality: Adapt the setting of PN functionality according to ActivationState. */
        if (ActivationState == CANTRCV_30_GENERICCAN_PN_ENABLED)
        {
          CanTrcv_30_GenericCan_Prob[index].isPnEnabled = TRUE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
        }
        else
        {
          CanTrcv_30_GenericCan_Prob[index].isPnEnabled = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
        }

        ++numberOfConfigurationChanges;
      }
      else
      {
        /* #220 Transceiver channel has DISABLED PN functionality: Set PN functionality to DISABLED. */
        CanTrcv_30_GenericCan_Prob[index].isPnEnabled = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
      }
    } /* IsChannelUsed */
  } /* for ... */

  if (numberOfConfigurationChanges == 0u)
  {
    /* #230 If no PN functionality was adapted according to ActivationState at all return E_NOT_OK. */
    retval = E_NOT_OK;
  }

  return retval;
}
#endif /* CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON */


#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
# if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
#  if (CANTRCV_30_GENERICCAN_VERIFY_DATA == STD_ON)
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_VerifyData
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_VerifyData(uint8 CanTrcvIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify the PN configuration written into HW. */
  /*  VerifyData
      ----------
      Only used for CanTrcv with selective wakeup functionality.
      
      Verify that all data written at Init_Channel_PnData are correct. This can be done by reading back the values.
      
      Set retval = E_OK if data is valid.
      
      Restrictions:
      - DO NOT modify the operation mode
      - DO NOT perform any write access
      - DO NOT use any asynchronous DIO/SPI API to access the hardware
      
      Example:
        uint8 i;
  
        retval = Spi_SetupEB(
          CanTrcv_30_GenericCan_SpiChannels[CanTrcvIndex].DATA,
          NULL_PTR,
          &(CanTrcv_SpiInBuffer[CanTrcvIndex][0])
        );
        
        if (retval == E_OK)
        {
          retval = Spi_SyncTransmit(CanTrcv_30_GenericCan_SpiSequences[CanTrcvIndex];
          if (retval == E_OK)
          {
            for (i = 0; i < 8; ++i)
            {
              if (CanTrcv_SpiInBuffer[CanTrcvIndex][i] != CanTrcv_SpiOutBuffer[CanTrcvIndex][i])
              {
                retval = E_NOT_OK;
                break;
              }
            }
          }
        }
        
        if (retval == E_OK)
        {
          retval = Spi_SyncTransmit(CanTrcv_30_GenericCan_SpiSequences[CanTrcvIndex]);
        }
  */
  <Your_Trcv_Code>

  /* #110 If verification was successful return E_OK, else E_NOT_OK. */
  return retval;
}
#  endif /* (CANTRCV_30_GENERICCAN_VERIFY_DATA == STD_ON) */
# endif /* (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) */
#endif


#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetStatusFlagsIndication
 **********************************************************************************************************************/
/*! Internal comment removed. *
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetStatusFlagsIndication(uint8 CanTrcvIndex)
{

  /* ----- Implementation ----------------------------------------------- */
  CanTrcv_EnterCritical();
  /* -- LL -- */
  /* #100 Get status flags. (critical section used) */
  /*  GetStatusFlags_Copy
      -------------------
  */
  #if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
  /*  When using DIO interface:
      =========================
        Leave empty. Copy MUST be done in GetStatusFlags!
  */
  #endif
  #if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
  /*  When using a SPI interface:
      ===========================
        Copy the statusflags from the SPI in buffer to  CanTrcv_30_GenericCan_Prob[CanTrcvIndex].CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags
        
        This function is beeing called only after CanTrcv_30_GenericCan_GetStatusFlags() was called.
       
        Example:
          CanTrcv_30_GenericCan_Prob[CanTrcvIndex].CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags.R0 = CanTrcv_SpiInBuffer[CanTrcvIndex][0];
          CanTrcv_30_GenericCan_Prob[CanTrcvIndex].CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlags.R1 = CanTrcv_SpiInBuffer[CanTrcvIndex][4];
  */
  #endif
  <Your_Trcv_Code>
  /* -- LL -- */

#  if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
  /* #110 Mark the got status flags as available. */
  CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlagsRdy = TRUE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */

  /* #120 Determine whether the status flags shall be processed within the *_MainFunction() or NOT. */
  if (CANTRCV_30_GENERICCAN_IS_MAIN_FCT_REQ(CanTrcvIndex))
  {
    /* Request was set in context of MainFunction -> inform the MainFunction */
    CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlagsNew = TRUE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
  }
  else
  {
    /* Request was not set in context of MainFunction -> Don't process this event in the MainFunction */
    CanTrcv_30_GenericCan_Prob[CanTrcvIndex].statusFlagsNew = FALSE; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
  }
#  endif

#  if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
  /* #130 If requested, check the availability of PN functionality in HW. */
  if ((CanTrcv_30_GenericCan_GetConfirmPnAvailabilityReq(CanTrcvIndex) != CanTrcv_30_GenericCan_Req_None))
  {
    /* Ignore this request if no normal mode is requested (may happen due to interrupted mode changes */
    if (CanTrcv_30_GenericCan_GetOpModeReq(CanTrcvIndex) == CanTrcv_30_GenericCan_Req_OpModeNormal) /* COV_CANTRCV_HL_GENERICCAN_CODECOV_DEFENSIVE_CODING_STYLE */
    {
      CanTrcv_30_GenericCan_CheckPnAvailability(CanTrcvIndex);
    }
    /* #140 Clear the request for checking of availability of PN functionality in HW. */
    CanTrcv_30_GenericCan_ClrConfirmPnAvailabilityReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */
  }
#  endif /* CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON */

  CanTrcv_LeaveCritical();
}
#endif /* SPI-interface */


#if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON) || \
    (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON)
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ReportWakeup
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ReportWakeup(uint8 CanTrcvIndex)
{
  /* ----- Implementation ----------------------------------------------- */
  
  /* #100 If any wake-up event occurred, report it to EcuM. */
  /* #110 Depending on configuration report to EcuM the wake-up source which belongs to occurred wake-up reason (POWER_ON, BY_SYSERR (only PN), BY_BUS, BY_PIN). */
  switch(CanTrcv_30_GenericCan_Prob[CanTrcvIndex].wakeUpReason)
  {
  case CANTRCV_30_GENERICCAN_WU_POWER_ON: /* COV_CANTRCV_HL_LL_GENERICCAN_CODECOV_WU_POWER_ON */
# if (CANTRCV_30_GENERICCAN_USE_EXTENDED_WU_SRC == STD_ON) 
    /* \trace SPEC-14972 */
    EcuM_SetWakeupEvent( CanTrcvCfg_GetWakeupSourcePor(CanTrcvIndex) );
    break;
# endif
#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
  case CANTRCV_30_GENERICCAN_WU_BY_SYSERR:
# if (CANTRCV_30_GENERICCAN_USE_EXTENDED_WU_SRC == STD_ON) 
    /* \trace SPEC-15081, SPEC-46898 */
    EcuM_SetWakeupEvent( CanTrcvCfg_GetWakeupSourceSyserr(CanTrcvIndex) );
    break;
# endif
#endif
  case CANTRCV_30_GENERICCAN_WU_BY_BUS:
  case CANTRCV_30_GENERICCAN_WU_BY_PIN:
    /* \trace SPEC-14953 */
    EcuM_SetWakeupEvent( CanTrcvCfg_GetWakeupSource(CanTrcvIndex) );
    break;

  default:
    /* No valid or an internal wakeup. Do not report this as wakeup event */
    break;
  }
}
#endif


#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
# if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
#  if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetSyncSpiTxSem
 **********************************************************************************************************************/
/*! Internal comment removed. *
 *
 *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(boolean, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetSyncSpiTxSem(uint8 CanTrcvIndex)
{
  boolean spiLocked;

  CanTrcv_EnterCritical();

  /* #100 Is SPI free?  (critical section is used)*/
  if (CANTRCV_30_GENERICCAN_IS_SPI_FREE(CanTrcvIndex)) /* COV_CANTRCV_HL_GENERICCAN_CODECOV_SYNC_INT_BY_SYNC */
  {
    /* #200 Yes: Lock SPI for synchronous transmission. */
    CanTrcv_30_GenericCan_SetSyncSpiTxReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */
    spiLocked = TRUE;
  }
  else
  {
    /* #300 No: Reject locking of SPI for synchronous transmission. */
    spiLocked = FALSE;
  }

  CanTrcv_LeaveCritical();

  return spiLocked;
}

/***********************************************************************************************************************
| NAME:  CanTrcv_GenericCan_ReleaseSyncSpiTxSem
 **********************************************************************************************************************/
/*! Internal comment removed. *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ReleaseSyncSpiTxSem(uint8 CanTrcvIndex)
{
  CanTrcv_EnterCritical();
  /* #100 Release semaphore for synchronous SPI-transmission. (critical section is used) */
  CanTrcv_30_GenericCan_ClrSyncSpiTxReq(CanTrcvIndex); /* SBSW_CANTRCV_HL_GENERICCAN_1 */
  CanTrcv_LeaveCritical();
}
#  endif /* CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ProcessNextOpModeReq
 **********************************************************************************************************************/
/*! Internal comment removed. *
 *
 *
 *
 *
 **********************************************************************************************************************/
CANTRCV_30_GENERICCAN_LOCAL_INLINE FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ProcessNextOpModeReq(uint8 CanTrcvIndex)
{
  Std_ReturnType retVal;

  /* #100 Was another mode change requested inbetween? */
  if (CanTrcv_30_GenericCan_Prob[CanTrcvIndex].nextRequestState != 0x0u)
  {
    /* #110 YES: Set the mode change request to new one. Return OK. */
    CanTrcv_30_GenericCan_SetOpModeReq(CanTrcvIndex, CanTrcv_30_GenericCan_Prob[CanTrcvIndex].nextRequestState); /* SBSW_CANTRCV_HL_GENERICCAN_1 */ /* SBSW_CANTRCV_HL_GENERICCAN_1 */
    CanTrcv_30_GenericCan_Prob[CanTrcvIndex].nextRequestState = 0x0u; /* SBSW_CANTRCV_HL_GENERICCAN_1 */
    retVal = E_OK;
  }
  else
  {
    /* #120 NO: Return NOT_OK. */
    retVal = E_NOT_OK;
  }

  return retVal;
}
# endif /* CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF */
#endif /* SPI_COMMENT */




/*  LocalFunctionImpl
    -----------------
    Add implementation of local (static) functions here.
*/
/* 
  <Your_Trcv_Code>
*/


#define CANTRCV_30_GENERICCAN_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* **************************************************************************/
/*  MISRA deviations                                                        */
/* **************************************************************************/
/* Justification for module-specific MISRA deviations:

MD_CanTrcv_30_GenericCan_14.3:
  Reason: Result or value is always true or false depend on configuration aspect and used HW specific implementation
  Risk: Incorrect value return will be ignored.
  Prevention: Code inspection and test of the different variants in the component test.
  
MD_CanTrcv_30_GenericCan_3408:
  Reason: Some local variables are declared without static declaration to allow better debugging.
  Risk: Name conflict.
  Prevention: All external variables are prefixed with <Msn>

MD_CanTrcv_30_GenericCan_8.9:
  Reason: Definition of CanTrcv_30_GenericCan_TaskInfoTable in global section because of better over-view.
  Risk: Name conflict.
  Prevention: Check for name conflicts.

*/


/* **************************************************************************/
/*  SILENT justifications                                                   */
/* **************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_CANTRCV_HL_GENERICCAN_1
\DESCRIPTION The variable "CanTrcv_30_GenericCan_Prob[]" must be accessed with index which is < CANTRCV_30_GENERICCAN_MAX_CHANNEL. 
\COUNTERMEASURE \N In case of an internal access initiated by the CanTrcv-driver itself this is ensured by the driver itself. The internal
access index is generated by a FOR-loop-count-index which runs from "0" until "CANTRCV_30_GENERICCAN_MAX_CHANNEL-1".
In case of an external access initiated by an upper layer e.g. CanIf this is ensured if CANTRCV_30_GENERICCAN_DEV_ERROR_DETECT == STD_ON.


\ID SBSW_CANTRCV_HL_GENERICCAN_2
\DESCRIPTION Write access to a pointer passed by an upper layer e.g. CanIf. Source data is of the same type as the destination data.
\COUNTERMEASURE \N The caller ensures that a valid pointer is passed. By the way compiler ensures that no memory violation occurs.


\ID SBSW_CANTRCV_HL_GENERICCAN_3
\DESCRIPTION Function call with passed pointer to a stack variable as parameter.
\COUNTERMEASURE \N Initiated and ensured by the CanTrcv-driver itself that a valid pointer is passed. By the way the compiler ensures that no memory violation occurs.


\ID SBSW_CANTRCV_HL_GENERICCAN_4
\DESCRIPTION Function pointer call.
\COUNTERMEASURE \N Initiated and ensured by the CanTrcv-driver itself that a valid function pointer is used. Used function pointer is retrieved from a constant table which is NOT generated. 
                   By the way the compiler ensures that no memory violation occurs.



SBSW_JUSTIFICATION_END */


/* **************************************************************************/
/*  COVERAGE justifications                                                 */
/* **************************************************************************/
/* START_COVERAGE_JUSTIFICATION


\ID COV_CANTRCV_HL_GENERICCAN_VARCOV_AVOID_COREVER_REDEF
  \ACCEPT TX
  \REASON Avoids the re-definition of CORE-version in case of multiple CanTrcv-driver are used. Hence decision "F" can NOT be fulfilled. Correct functionality is ensured by code-review.


\ID COV_CANTRCV_HL_GENERICCAN_CODECOV_NOT_EACH_CONFIG
  \ACCEPT XF
  \REASON The stimulation is not possible in each configuration. It will be ensured by inspection that both decisions are covered
          in at least one configuration. In addition correct functionality is verified by code review.


\ID COV_CANTRCV_HL_GENERICCAN_CODECOV_DEFENSIVE_CODING_STYLE
  \ACCEPT TX
  \REASON The code is required due to HL-LL-concept and in order to prevent malfunctions in case of programming of LL. 
          Hence the stimulation is not possible in case of correct programming of LL. The correct functionality is verified by code review.


\ID COV_CANTRCV_HL_GENERICCAN_CODECOV_IS_WU_BY_BUS_USED
  \ACCEPT TX
  \ACCEPT TF tx tf
  \REASON Condition / decision not covered by automatic tests. Affected code has low complexity because no else-path is available. 
          The code is verified by code inspection.


\ID COV_CANTRCV_HL_GENERICCAN_CODECOV_OP_MODE_REQ_NONE
  \ACCEPT TF tf tx
  \REASON Condition not covered by automatic tests but both decisions are covered. Affected code has low complexity. 
          The code is verified by code inspection.


\ID COV_CANTRCV_HL_GENERICCAN_CODECOV_SYNC_INT_BY_SYNC
  \ACCEPT TF tf tx
  \REASON The condition is tested manually by TCASE: TCASE-481987. In addition correctness of code is ensured by code inspection.




END_COVERAGE_JUSTIFICATION */ 

/* ********************************************************************************************************************
 *  END OF FILE
 * *******************************************************************************************************************/
