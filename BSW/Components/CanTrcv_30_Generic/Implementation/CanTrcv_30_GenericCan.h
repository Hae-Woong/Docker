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
 |         File:  CanTrcv_30_GenericCan.h
 |      Project:  MICROSAR
 |       Module:  CAN Transceiver driver
 |    Generator:  DaVinci Configurator
 |  Description:  Header of the MICROSAR CAN Transceiver Driver.
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
 *  1.06.00   2008-10-08  Ou      -             No changes, just merge it with the high-level version 1.03
 *  1.07.00   2008-10-28  Ou      -             No changes, just update the specific DLLs
 *  1.08.00   2008-12-11  Ou      -             Use the Autosar compliant copyright
 *  1.09.00   2009-02-19  Tvi     -             Changed to match with updated DLLs
 *  1.10.00   2009-02-26  Tvi     ESCAN00033398 Compiler warnings when using different transceiver types within the same stack
 *  1.11.00   2009-07-01  Fn      -             (no changes here, just to merge with HL)
 *            2009-07-27  Tvi     ESCAN00033879 Define is written wrong and not enclosured with a switch
 *  1.11.01   2009-10-15  Tvi     -             (no changes here, just to merge with HL)
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
 *            GENERICCAN is used for definitions in upper case for defines.
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
 *                        Tvi     ESCAN00049831 [PN only] Store wakeup event and report them if Trcv is in Standby mode
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
#if !defined (CANTRCV_30_GENERICCAN_API_H)
# define CANTRCV_30_GENERICCAN_API_H

/* PRQA S 0850 EOF */ /* MD_MSR_MacroArgumentEmpty */
/* PRQA S 0777 EOF */ /* MD_MSR_Rule5.1 */

# define V_CANTRCV_30_GENERICCAN_HEADER

/* ********************************************************************************************************************
 * INCLUDES
 * *******************************************************************************************************************/
# include "CanTrcv_30_GenericCan_Cfg.h" /* SREQ00010148,SREQ00007523 */

# include "SchM_CanTrcv_30_GenericCan.h"

/* ********************************************************************************************************************
 * Module version
 * *******************************************************************************************************************/

# define CANTRCV_30_GENERICCAN_AR_RELEASE_MAJOR_VERSION      4u
# define CANTRCV_30_GENERICCAN_AR_RELEASE_MINOR_VERSION      0u
# define CANTRCV_30_GENERICCAN_AR_RELEASE_REVISION_VERSION   3u
# define CANTRCV_30_GENERICCAN_AUTOSARVERSION_4X             0x403u

/* ##V_CFG_MANAGEMENT ##CQProject : DrvTrans_GenericCandioAsr CQComponent : Implementation */
# define DRVTRANS_GENERICCANDIOASR_VERSION                     0x0402u
# define DRVTRANS_GENERICCANDIOASR_RELEASE_VERSION             0x00u


#if defined(CANTRCV_30_GENERICCAN_INCLUDE_INTERNAL) /* COV_CANTRCV_HL_GENERICCAN_VARCOV_AVOID_COREVER_REDEF */
/* ##V_CFG_MANAGEMENT ##CQProject : DrvTrans__coreCanxAsr CQComponent : Implementation */
# define DRVTRANS__CORECANXASR_VERSION                         0x0507u
# define DRVTRANS__CORECANXASR_RELEASE_VERSION                 0x00u
#endif

# define CANTRCV_30_GENERICCAN_SW_MAJOR_VERSION       ((uint8)((DRVTRANS_GENERICCANDIOASR_VERSION & 0xFF00u) >> 8))
# define CANTRCV_30_GENERICCAN_SW_MINOR_VERSION                (DRVTRANS_GENERICCANDIOASR_VERSION & 0x00FFu)
# define CANTRCV_30_GENERICCAN_SW_PATCH_VERSION                 DRVTRANS_GENERICCANDIOASR_RELEASE_VERSION


/*  SupportedModes
    --------------
    
    Specify the modes and features supported by the underlying transceiver
    hardware by enabling the following defines.

    Enable this if the underlying transceiver hardware supports a standby mode
    
    #define CANTRCV_30_GENERICCAN_SUPPORTS_MODE_STANDBY

    
    Enable this if the underlying transceiver Hardware supports a sleep mode
    
    #define CANTRCV_30_GENERICCAN_SUPPORTS_MODE_SLEEP

    
    Enable this to init via normal mode. This is needed by the Tja1041 to clear 
    the pwon flag and allow the transceiver to enter sleep from init
    
    #define CANTRCV_30_GENERICCAN_INIT_VIA_NORMAL_MODE


    Enable this to allow the transceiver switching to normal mode again. This 
    is needed by the Tja1041 to toggle the WAKE pin.
    
    #define CANTRCV_30_GENERICCAN_ALLOW_ENTER_NORMAL_TWICE
    
    
    Example TJA1041:
    #define CANTRCV_30_GENERICCAN_SUPPORTS_MODE_STANDBY
    #define CANTRCV_30_GENERICCAN_SUPPORTS_MODE_SLEEP
    #define CANTRCV_30_GENERICCAN_INIT_VIA_NORMAL_MODE
    #define CANTRCV_30_GENERICCAN_ALLOW_ENTER_NORMAL_TWICE
*/
<Your_Trcv_Code>


/* ********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 * *******************************************************************************************************************/ 
# define CANTRCV_30_GENERICCAN_VENDOR_ID                      30u
# define CANTRCV_30_GENERICCAN_MODULE_ID                      70u

/* ! API IDs for Error Reporting */
# define CANTRCV_30_GENERICCAN_INIT_ID                        0x00u
# define CANTRCV_30_GENERICCAN_SETOPMODE_ID                   0x01u
# define CANTRCV_30_GENERICCAN_GETOPMODE_ID                   0x02u
# define CANTRCV_30_GENERICCAN_GETBUSWUREASON_ID              0x03u
# define CANTRCV_30_GENERICCAN_GETVERSIONINFO_ID              0x04u
# define CANTRCV_30_GENERICCAN_SETWAKEUPMODE_ID               0x05u
# define CANTRCV_30_GENERICCAN_MAINFUNCTION_ID                0x06u
# define CANTRCV_30_GENERICCAN_CHECKWAKEUP_ID                 0x07u 
# define CANTRCV_30_GENERICCAN_MAINFUNCTIONDIAGNOSTICS        0x08u
# define CANTRCV_30_GENERICCAN_GETTRCVSYSTEMDATA_ID           0x09u
# define CANTRCV_30_GENERICCAN_CLEARTRCVWUFFLAG_ID            0x0Au
# define CANTRCV_30_GENERICCAN_READTRCVTIMEOUTFLAG_ID         0x0Bu
# define CANTRCV_30_GENERICCAN_CLEARTRCVTIMEOUTFLAG_ID        0x0Cu
# define CANTRCV_30_GENERICCAN_READTRCVSILENCEFLAG_ID         0x0Du
# define CANTRCV_30_GENERICCAN_CHECKWAKEFLAG_ID               0x0Eu
# define CANTRCV_30_GENERICCAN_SETPNACTIVATIONSTATE_ID        0x0Fu
# define CANTRCV_30_GENERICCAN_SPIINDICATION_ID               0x10u


# define CANTRCV_30_GENERICCAN_OP_MODE_NORMAL                 CANTRCV_TRCVMODE_NORMAL
# define CANTRCV_30_GENERICCAN_OP_MODE_STANDBY                CANTRCV_TRCVMODE_STANDBY 
# define CANTRCV_30_GENERICCAN_OP_MODE_SLEEP                  CANTRCV_TRCVMODE_SLEEP

# define CANTRCV_30_GENERICCAN_WU_ENABLE                      CANTRCV_WUMODE_ENABLE
# define CANTRCV_30_GENERICCAN_WU_DISABLE                     CANTRCV_WUMODE_DISABLE
# define CANTRCV_30_GENERICCAN_WU_CLEAR                       CANTRCV_WUMODE_CLEAR

# define CANTRCV_30_GENERICCAN_WU_ERROR                       CANTRCV_WU_ERROR
# define CANTRCV_30_GENERICCAN_WU_NOT_SUPPORTED               CANTRCV_WU_NOT_SUPPORTED
# define CANTRCV_30_GENERICCAN_WU_BY_BUS                      CANTRCV_WU_BY_BUS
# define CANTRCV_30_GENERICCAN_WU_INTERNALLY                  CANTRCV_WU_INTERNALLY
# define CANTRCV_30_GENERICCAN_WU_RESET                       CANTRCV_WU_RESET
# define CANTRCV_30_GENERICCAN_WU_POWER_ON                    CANTRCV_WU_POWER_ON
# define CANTRCV_30_GENERICCAN_WU_BY_PIN                      CANTRCV_WU_BY_PIN
# define CANTRCV_30_GENERICCAN_WU_BY_SYSERR                   CANTRCV_WU_BY_SYSERR

# define CANTRCV_30_GENERICCAN_E_NO_ERROR                     0x00u
# define CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK          0x01u
/* ! SREQ00010156: Keep the invalid CAN Network switch for compatibility reason. */
# define CANTRCV_30_GENERICCAN_E_INVALID_TRANSCEIVER          CANTRCV_30_GENERICCAN_E_INVALID_CAN_NETWORK
# define CANTRCV_30_GENERICCAN_E_PARAM_POINTER                0x02u
# define CANTRCV_30_GENERICCAN_E_UNINIT                       0x11u
# define CANTRCV_30_GENERICCAN_E_TRCV_NOT_STANDBY             0x21u
# define CANTRCV_30_GENERICCAN_E_TRCV_NOT_NORMAL              0x22u
# define CANTRCV_30_GENERICCAN_E_PARAM_TRCV_WAKEUP_MODE       0x23u
# define CANTRCV_30_GENERICCAN_E_PARAM_TRCV_OP_MODE           0x24u
# define CANTRCV_30_GENERICCAN_E_BAUDRATE_NOT_SUPPORTED       0x25u
# define CANTRCV_30_GENERICCAN_E_CLR_TRCV_WUF_NOT_CALLED      0x30u
# define CANTRCV_30_GENERICCAN_E_NO_TRCV_CONTROL              0x40u /* SREQ00010156: Actual ID is unspecified in SWS! */


# define CANTRCV_30_GENERICCAN_WAKEUP_NOT_SUPPORTED          0u
# define CANTRCV_30_GENERICCAN_WAKEUP_BY_POLLING             1u
# define CANTRCV_30_GENERICCAN_WAKEUP_BY_ISR                 2u
/* ! Compatibility defines... */
# define CANTRCV_30_GENERICCAN_WAKEUP_NONE                   CANTRCV_30_GENERICCAN_WAKEUP_NOT_SUPPORTED
# define CANTRCV_30_GENERICCAN_WAKEUP_POLLING                CANTRCV_30_GENERICCAN_WAKEUP_BY_POLLING
# define CANTRCV_30_GENERICCAN_WAKEUP_ISR                    CANTRCV_30_GENERICCAN_WAKEUP_BY_ISR

# define CanTrcv_30_GenericCan_TrcvModeType                  CanTrcv_TrcvModeType
# define CanTrcv_30_GenericCan_TrcvWakeupModeType            CanTrcv_TrcvWakeupModeType
# define CanTrcv_30_GenericCan_TrcvWakeupReasonType          CanTrcv_TrcvWakeupReasonType


/* ! Timer configuration */
/* TimerConfiguration
   ------------------

   Activate this to enable the usage of hardware timers. For the Tja1041
   these timers are needed to wait some time after some mode transitions.
   
   Activating this will enable the callback:
      Appl_CanTrcv_30_GenericCan_Wait(uint8 TimerIndex)

   Example TJA1041:
   # define CANTRCV_30_GENERICCAN_USE_TIMERS                    STD_ON
*/
<Your_Trcv_Code>

/* ! Predefined timer indexes (0x01 - 0x0f)                                   */
# define kCanTrcv_30_GenericCan_LoopInit                          0x01

/*  AdditionalTimer
    ---------------
    
    If needed, you may specify additional timer indexes here. These must have
    the following format and can be used within Appl_CanTrcv_30_GenericCan_Wait

    # define kCanTrcv_30_GenericCan_<identifier>                 <0x10 - 0xff>
*/
<Your_Trcv_Code>



/* ********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 * *******************************************************************************************************************/

/* ********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 * *******************************************************************************************************************/
/* SREQ00007522 */
/* ! Defines the state of any requested FLAG-action: *SET or *CLEARED. */
typedef enum
{
  CANTRCV_30_GENERICCAN_FLAG_SET,
  CANTRCV_30_GENERICCAN_FLAG_CLEARED
}
CanTrcv_30_GenericCan_TrcvFlagStateType;

/* ! Defines the state of PN (= Partial Networking): *ENABLED or *DISABLED. */
typedef enum
{
  CANTRCV_30_GENERICCAN_PN_ENABLED,
  CANTRCV_30_GENERICCAN_PN_DISABLED
} CanTrcv_30_GenericCan_PNActivationType;


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


/* ! The following type includes CAN transceiver channel specific settings. */
#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
typedef volatile struct
#else /* _DIO, _SBC */
typedef struct
#endif
{
  /* ---- Common attributes ---- */

  /* ! Last detected wakeup reason */
  CanTrcv_30_GenericCan_TrcvWakeupReasonType wakeUpReason;

  /* ! Specifies whether or not to report wakeups */
  CanTrcv_30_GenericCan_TrcvWakeupModeType wakeUpReport;

  /* ! Holds the last read statusFlags.. only valid if statusFlagsRdy == TRUE */
  CanTrcv_30_GenericCan_StatusFlagsType statusFlags;

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI) 
  /* ! Holds the current request state */
  uint32 requestState;
#endif /* SPI_COMMENT / (SBC_COMMENT + PN) */

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
  /* ! Holds the current operation mode */
  CanTrcv_30_GenericCan_TrcvModeType curOpMode;

# if (CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF) 
  /* ! Holds the next request that is triggered automatically
   * after the worker was cancelled
   */
  uint32 nextRequestState;

#  if (CANTRCV_30_GENERICCAN_WAKEUP_BY_BUS_USED == STD_ON)
  /* ! Indicates whether a wakeup was detected during a mode transition */
  boolean wakeUpInTransitionDetected;
#  endif

  /* ! Indicates that statusFlags are ready.. This must be evaluated with locked
   * interrupts before reading statusFlags
   */
  boolean statusFlagsRdy;

  /* ! Indicates that new statusFlags are available for the MainFunction.
   * will be reset as soon as read
   */
  boolean statusFlagsNew;
# endif
#endif /* CANTRCV_30_GENERICCAN_INTERFACE_SPI */

#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
  /* ! Indicates if pn is currently available in HW */
  boolean pnAvailable;

  /* ! Indicates if pn is enabled in software */
  boolean isPnEnabled;
#endif

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
  /* ! Specifies whether the transceiver is initialized, this member is not needed in case of DIO because IsUsed == IsInit */
  uint8 isInit;
#endif

  /* ! HW specific attributes */
  /*  AdditionalProbType
      ------------------
      
      Add channel specific data here. The members will be part of CanTrcv_30_GenericCan_Prob[index].
      
      Example:
        uint8   currentTransceiverState;
  */    
  
  /*  
      <Your_Trcv_Code>
  */

} CanTrcv_30_GenericCan_Prob_Type;

/* ********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 * *******************************************************************************************************************/

/* ********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 * *******************************************************************************************************************/
/* for callback function prototypes see CanTrcv_Cbk.h */
# define CANTRCV_30_GENERICCAN_START_SEC_CODE
# include "MemMap.h"


/************************************************************************************************************
 | NAME:  CanTrcv_30_GenericCan_Init()
 ***********************************************************************************************************/
/*! \brief         Initializes the CAN transceiver module.
 *  \details       Initializes all active channels of the CAN transceiver driver which are configured in the configuration tool.
 *                 This function has the service id 0x00.
 *  \param[in]     ConfigPtr - Pointer to the CanTrcv_30_GenericCan_Config structure.
 *                 If multiple configurations are available, the active configuration can be selected by using the related CanTrcv_30_GenericCan_Config_<IdentityName> structure.
 *  \pre           The function CanTrcv_30_GenericCan_InitMemory must be called before the function CanTrcv_30_GenericCan_Init can be called.
 *                 This function must be called before any other service functionality of the transceiver driver
 *                 The SPI/DIO driver must be initialized
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        -
***********************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_Init( P2CONST(CanTrcv_30_GenericCan_ConfigType, AUTOMATIC, CANTRCV_30_GENERICCAN_CONST) ConfigPtr );


/***********************************************************************************************************************
 | NAME:  CanTrcv_30_GenericCan_InitMemory
 **********************************************************************************************************************/
/*! \brief         Initializes the memory of the CAN transceiver module.
 *  \details       -
 *  \param[in]     -
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        -
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_InitMemory(void);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_SetOpMode
 **********************************************************************************************************************/
/*! \brief       Changes the transceiver operating mode to the requested mode.
 *  \details     This function has the service id 0x01.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[in]   OpMode: Operating mode which shall be set.
 *  \return      E_OK: Change of transceiver operating mode was successful.
 *  \return      E_NOT_OK: Change of transceiver operating mode failed or passed parameter(s) is/are invalid.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON), FALSE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
 *  \config      -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_SetOpMode(uint8 CanTrcvIndex, CanTrcv_TrcvModeType OpMode);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetOpMode
 **********************************************************************************************************************/
/*! \brief       Returns the current transceiver operating mode.
 *  \details     DIO transceiver: The mode is determined from status of PINs of the transceiver hardware.
 *               SPI transceiver: The mode is read from state variable. No access to transceiver hardware is performed.
 *               This function has the service id 0x02.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[out]  OpMode: Pointer to buffer where the current operating mode is stored.
 *  \return      E_OK: Determination of current transceiver operating mode was successful.
 *  \return      E_NOT_OK: Determination of transceiver operating mode failed or passed parameter(s) is/are invalid.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetOpMode(uint8 CanTrcvIndex, P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) OpMode);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetBusWuReason
 **********************************************************************************************************************/
/*! \brief       Returns the bus wake-up reason.
 *  \details     The wake-up reason is read from state variable. No access to transceiver hardware is performed.
 *               This function has the service id 0x03.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[out]  Reason: Pointer to buffer where the bus wake-up reason is stored.
 *  \return      E_OK: Determination of wake-up reason was successfull.
 *  \return      E_NOT_OK: Determination of wake-up reason failed or passed parameter(s) is/are invalid.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetBusWuReason(uint8 CanTrcvIndex, P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) Reason);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_SetWakeupMode
 **********************************************************************************************************************/
/*! \brief       Changes the wake-up mode to the requested mode.
 *  \details     For more information about possible wake-up modes please see the description of type <CanTrcv_TrcvWakeupModeType>.
 *               This function has the service id 0x05.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[in]   TrcvWakeupMode: Requested wake-up mode for the transceiver channel (CanTrcvIndex).
 *  \return      E_OK: Change of wake-up mode was successful.
 *  \return      E_NOT_OK: Change of wake-up mode has failed or passed parameter(s) is/are invalid.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_SetWakeupMode(uint8 CanTrcvIndex, CanTrcv_TrcvWakeupModeType TrcvWakeupMode);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CheckWakeup
 **********************************************************************************************************************/
/*! \brief       Checks if a wake-up event was detected.
 *  \details     This function has the service id 0x07.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \return      E_OK: Wake-up was detected / the request to check for wake-up was accepted (only asynchronous SPI-interface).
 *  \return      E_NOT_OK: No wake-up was detected / the request to check for wake-up was rejected (only asynchronous SPI-interface).
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON), FALSE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
 *  \config      -
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CheckWakeup(uint8 CanTrcvIndex);


#if (CANTRCV_30_GENERICCAN_GET_VERSION_INFO == STD_ON)
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetVersionInfo
 **********************************************************************************************************************/
/*! \brief       Returns the version info of the CAN transceiver module.
 *  \details     The versions are stored into the structure referenced by parameter: VersionInfo.
 *               This function has the service id 0x04.
 *  \param[out]  VersionInfo: Pointer to structure for storing of version info.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_GET_VERSION_INFO == STD_ON
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) VersionInfo);
#endif /* (CANTRCV_30_GENERICCAN_GET_VERSION_INFO == STD_ON) */

/* SREQ00007670 */
#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) 
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_GetTrcvSystemData
 **********************************************************************************************************************/
/*! \brief       Reads the transceiver configuration / status data and returns it via the parameter TrcvSysData.
 *  \details     This function has the service id 0x09.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[out]  TrcvSysData: Configuration / status data of the transceiver.
 *  \return      E_OK: Status was read successfully.
                 E_NOT_OK: Status data is not available or DET/DEM occurred.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_GetTrcvSystemData(uint8 CanTrcvIndex, P2VAR(uint32, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) TrcvSysData );
#endif /* (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) */

#if (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) /* SREQ00007670 */
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ClearTrcvWufFlag
 **********************************************************************************************************************/
/*! \brief       Requests to clear the WUF flag of a transceiver channel (CanTrcvIndex).
 *  \details     This function has the service id 0x0A.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \return      E_OK: Request to clear the WUF was accepted.
                 E_NOT_OK: Request to clear the WUF was rejected.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON), FALSE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
 *  \config      CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ClearTrcvWufFlag(uint8 CanTrcvIndex);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ReadTrcvTimeoutFlag
 **********************************************************************************************************************/
/*! \brief       Reads the status of the timeout flag from the transceiver hardware.
 *  \details     This function has the service id 0x0B.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[out]  FlagState: State of the timeout flag.
 *  \return      E_OK: Status was read successfully.
                 E_NOT_OK: Status could not be read or DET occurred.
 *  \pre         -
 *  \context     ANY
  * \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ReadTrcvTimeoutFlag(uint8 CanTrcvIndex, P2VAR(CanTrcv_30_GenericCan_TrcvFlagStateType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) FlagState);


/***********************************************************************************************************************
 | NAME:  CanTrcv_30_GenericCan_ClearTrcvTimeoutFlag
 **********************************************************************************************************************/
/*! \brief       Clears the status of the timeout flag in the transceiver hardware.
 *  \details     This function has the service id 0x0C.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \return      E_OK: Timeout flag cleared successfully.
                 E_NOT_OK: Timeout flag could not be cleared or DET occurred.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ClearTrcvTimeoutFlag(uint8 CanTrcvIndex);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_ReadTrcvSilenceFlag
 **********************************************************************************************************************/
/*! \brief       Reads the status of the silence flag from the transceiver hardware.
 *  \details     This function has the service id 0x0D.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \param[out]  FlagState: State of the silence flag.
 *  \return      E_OK: Status was read successfully.
                 E_NOT_OK: Status could not be read or DET occurred.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_ReadTrcvSilenceFlag(uint8 CanTrcvIndex, P2VAR(CanTrcv_30_GenericCan_TrcvFlagStateType, AUTOMATIC, CANTRCV_30_GENERICCAN_APPL_VAR) FlagState);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_CheckWakeFlag
 **********************************************************************************************************************/
/*! \brief       Requests to check the status of the wake-up flag from the transceiver hardware.
 *  \details     This function has the service id 0x0E.
 *  \param[in]   CanTrcvIndex: Index of the selected transceiver channel to which API call has to be applied.
 *  \return      E_OK: Request was accepted.
                 E_NOT_OK: Request was not accepted or DET occurred.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different transceiver channels (CanTrcvIndex)
 *  \synchronous TRUE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON), FALSE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
 *  \config      CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_CheckWakeFlag(uint8 CanTrcvIndex);


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_SetPNActivationState
 **********************************************************************************************************************/
/*! \brief       Enables / disables selective wake-up for all transceiver channels.
 *  \details     This function has the service id 0x0F.
 *  \param[in]   ActivationState: State of PN-mode to be applied for all transceiver channels.
 *               <ENABLE>: Selective wake-up for all channels where PN is configured shall be enabled.
 *               <DISABLE>: Disable the selective wake-up functionality on all transceiver channels.
 *  \return      E_OK: PN-mode was changed successfully.
 *               E_NOT_OK: PN-mode change was not accepted or DET occurred.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON
 **********************************************************************************************************************/
FUNC(Std_ReturnType, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_SetPNActivationState(CanTrcv_30_GenericCan_PNActivationType ActivationState);
#endif /* (CANTRCV_30_GENERICCAN_HW_PN_SUPPORT == STD_ON) */


/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_MainFunction
 **********************************************************************************************************************/
/*! \brief       Service to scan all buses for wake up events and perform these event.
 *  \details     This function has the service id 0x06.
 *  \param[in]   -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON), FALSE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
 *  \config      -
 **********************************************************************************************************************/
/* FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_MainFunction(void); */ /* ESCAN00091270 */

#  if (CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON) /* SREQ00007558 */
/***********************************************************************************************************************
| NAME:  CanTrcv_30_GenericCan_MainFunctionDiagnostics
 **********************************************************************************************************************/
/*! \brief       Reads the transceiver diagnostic status periodically and sets product/development error accordingly.
 *  \details     This function has the service id 0x08.
 *  \param[in]   -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_ON), FALSE (if CANTRCV_30_GENERICCAN_SPI_ACCESS_SYNCHRONOUS == STD_OFF)
 *  \config      CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON
 **********************************************************************************************************************/
FUNC(void, CANTRCV_30_GENERICCAN_CODE) CanTrcv_30_GenericCan_MainFunctionDiagnostics(void);
#  endif /* CANTRCV_30_GENERICCAN_BUS_ERR_FLAG == STD_ON */


# define CANTRCV_30_GENERICCAN_STOP_SEC_CODE
# include "MemMap.h"

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_DIO)
/*! 
* \exclusivearea CANTRCV_GENERICCAN_EXCLUSIVE_AREA_0
* Ensures consistent handling of CanTrcv-hardware via the DIO-interface.
* \protects Atomic / consistent reading of DIO-pins and setting of them.
* \usedin CanTrcv_30_GenericCan_Init(), CanTrcv_30_GenericCan_SetOpMode(), CanTrcv_30_GenericCan_GetOpMode(), CanTrcv_30_GenericCan_CheckWakeup() and CanTrcv_30_GenericCan_MainFunction().
* \exclude call of: CanTrcv-driver APIs: CanTrcv_30_GenericCan_Init(), CanTrcv_30_GenericCan_SetOpMode(), CanTrcv_30_GenericCan_GetOpMode(), CanTrcv_30_GenericCan_CheckWakeup() and CanTrcv_30_GenericCan_MainFunction() 
*                   AND in fact the instructions within the exclusive area must be executed atomic without any interruption / delay in between.
* \length SHORT Call to DIO module.
* \endexclusivearea
*/
#endif

#if defined (CANTRCV_30_GENERICCAN_INTERFACE_SPI)
/*! 
* \exclusivearea CANTRCV_GENERICCAN_EXCLUSIVE_AREA_0
* Ensures consistent handling of CanTrcv-hardware via the SPI-interface.
* \protects Atomic / consistent usage of SPI-interface.
* \usedin CanTrcv_30_GenericCan_Init(), CanTrcv_30_GenericCan_SetOpMode(), CanTrcv_30_GenericCan_GetOpMode(), CanTrcv_30_GenericCan_CheckWakeup(), CanTrcv_30_GenericCan_CbWakeupByBusIndication
*         CanTrcv_30_GenericCan_TrcvModeIndication(), CanTrcv_30_GenericCan_GetTrcvSystemData(), CanTrcv_30_GenericCan_ClearTrcvWufFlag(), CanTrcv_30_GenericCan_ClearTrcvWufFlagIndication()
*         CanTrcv_30_GenericCan_ReadTrcvTimeoutFlag(), CanTrcv_30_GenericCan_ClearTrcvTimeoutFlag(),  CanTrcv_30_GenericCan_ReadTrcvSilenceFlag(), CanTrcv_30_GenericCan_CheckWakeFlag() 
*         CanTrcv_30_GenericCan_CheckWakeFlagIndication(), CanTrcv_30_GenericCan_GetStatusFlagsIndication(), CanTrcv_30_GenericCan_StartTask(), CanTrcv_30_GenericCan_ReSchedule() 
*         CanTrcv_30_GenericCan_Schedule() and CanTrcv_30_GenericCan_MainFunction().
* \exclude call of: CanTrcv-driver APIs: CanTrcv_30_GenericCan_Init(), CanTrcv_30_GenericCan_SetOpMode(), CanTrcv_30_GenericCan_GetOpMode(), CanTrcv_30_GenericCan_CheckWakeup(), CanTrcv_30_GenericCan_MainFunction(), 
*                   CanTrcv_30_GenericCan_GetTrcvSystemData(), CanTrcv_30_GenericCan_ClearTrcvWufFlag(), CanTrcv_30_GenericCan_ReadTrcvTimeoutFlag(), CanTrcv_30_GenericCan_ClearTrcvTimeoutFlag(),
*                   CanTrcv_30_GenericCan_ReadTrcvSilenceFlag(), CanTrcv_30_GenericCan_CheckWakeFlag(), CanTrcv_30_GenericCan_MainFunction() and CanTrcv_30_GenericCan_SpiIndication().
* \length LONG Call to SPI module, update of internal states.
* \endexclusivearea
*/
#endif


#endif /* CANTRCV_30_GENERICCAN_API_H */

/* ********************************************************************************************************************
 *  END OF FILE
 * *******************************************************************************************************************/
