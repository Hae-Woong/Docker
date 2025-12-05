/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file  FrTSyn.h
 *        \unit   Common
 *        \unit   Utility
 *        \unit   LU_Transmit
 *        \unit   Transmit
 *        \unit   LU_ProtocolTx
 *        \unit   ProtocolTx
 *        \unit   SyncTx
 *        \unit   CurrentTime
 *        \unit   AssembleMsg
 *        \unit   Validation
 *        \unit   TxPeriodTime
 *        \unit   TxSequenceCounter
 *        \unit   CyclicMsgResumeTime
 *        \unit   TriggerTransmitTimeout
 *        \unit   DebounceTime
 *        \unit   PduIdLock
 *        \unit   FrController
 *        \unit   LU_Receive
 *        \unit   Receive
 *        \unit   LU_ProtocolRx
 *        \unit   ProtocolRx
 *        \unit   SyncRx
 *        \unit   DisassembleMsg
 *        \unit   ValidationMsg
 *        \brief  Vector AUTOSAR FrTSyn header file
 *
 *      \details  This is the main header file of the Vector AUTOSAR module FrTSyn.

 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author   Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-12-04  visssf   ESCAN00077543 FEAT-971: Provide module FrTSyn for global time synchronization over FlexRay
 *  01.00.01  2015-03-20  visssf   ESCAN00081941 Wrong calculation of current local time of Time Slave
 *  02.00.00  2015-09-22  visssf   -             Updated Generator Version to 2.0 (Java 8 and R14 CFG5 Breaking Changes)
 *  02.01.00  2016-02-26  visssf   ESCAN00088578 FEAT-1694: Support multiple masters per Time Domain
 *            2016-03-01  visssf   ESCAN00087839 FEAT-1631: Trigger Transmit API with SduLength In/Out according to ASR4.2.2
 *            2016-03-15  visssf   ESCAN00088704 Wrong calculation and validation of Sequence Counter
 *            2016-03-22  visssf   ESCAN00087328 MISRA deviation: MISRA-C:2004 Rules 1.1, 5.6, 14.3, 14.1, 19.7, 10.1 and HIS Metrics
 *  03.00.00  2017-04-13  vistra   STORYC-252    Update BSWMD Files to AR 4.3
 *  03.01.00  2017-05-15  visssf   STORYC-129    Tx state machine shall support Multiple Time Domains
 *  03.02.00  2017-06-19  visssf   ESCAN00090441 Wrong CRC calculation and validation
 *            2017-06-20  vismno   STORYC-115    Debounce Time introduction
 *            2017-06-20  vismno   ESCAN00095192 Wrong validation of Sequence Counter after overflow
 *            2017-06-22  visssf   STORYC-1210   Global Time Precision Measurement Support
 *            2017-06-23  vistra   ESCAN00095253 Missing initialization of FrTSyn_LastTransmitFrCycle
 *            2017-06-26  vistra   ESCAN00095418 Time slave not correctly synchronized to the time master
 *  03.03.00  2017-08-03  vistra   STORYC-112    Immediate Time Synchronization
 *            2017-08-23  visssf   ESCAN00090314 OS API called with disabled interrupts
 *  03.04.00  2017-09-20  viscpi   STORYC-118    STORY-130: AR4.3 extensions not covered by separate story
 *            2017-10-17  vistra   STORYC-1736   STORY-1727: Support AR4.3 message types and for backward compatibility also AR 4.2 messages types
 *  03.05.00  2017-12-21  viscpi   STORYC-2520   Adaptions with ComStackLib Macros
 *            2018-03-01  visssf   ESCAN00098538 Transmit trigger not accepted due to wrong master state
 *            2018-04-05  visssf   ESCAN00098993 StbM API called with disabled interrupts
 *  03.06.00  2018-04-24  vissi    STORYC-5074   STORY-4927: Support Measurement (MC Data)
 *            2018-05-02  visssf   ESCAN00099201 Wrong evaluation of FlexRay controller properties for time masters
 *            2018-05-15  vissi    STORYC-5070   STORY-4926: Improve accuracy of the global time by rework of exclusive areas around OS API calls
 *            2018-08-21  visssf   ESCAN00099009 Wrong time calculation by master or slave
 *            2018-08-27  vistra   ESCAN00100606 Compiler warning: several GCC specific warnings in FrTSyn.c
 *            2018-08-29  visssf   ESCAN00100556 Wrong evaluation of user data by the time slave
 *  03.07.00  2018-09-19  vissi    STORYC-5090   STORY-4944: MISRA-C2012: SysService_AsrTSynFr
 *            2018-12-06  vistra   ESCAN00101480 SGW bit is never set in SYNC messages
 *            2019-02-07  vissi    ESCAN00102043 Wrong error codes for Det errors FRTSYN_E_PARAM and FRTSYN_E_INV_CTRL_IDX
 *            2019-02-07  vistra   ESCAN00101896 SC Jump Width checking shall be disabled after a timeout
 *            2019-02-13  vissi    ESCAN00100975 Linker error: unresolved external symbol FrTSyn_GetTxDebounceCounterIdxOfMasterConfig
 *  04.00.00  2019-09-26  visbfk   MSR4-213      Support time synchronization as safety requirement (FrTsyn)
 *            2019-10-14  visssf   ESCAN00104280 Immediate Transmission might be lost if message processing fails
 *            2019-10-16  visbfk   ESCAN00103223 Usage of invalid debouncing index for master without debouncing configured among masters with debouncing configured
 *            2019-10-21  visssf   ESCAN00104412 Restart of cyclic transmission after Immediate Transmit might be delayed.
 *            2019-10-23  visssf   ESCAN00104518 Tx interval might get longer for delayed TriggerTransmit()
 *            2019-10-24  visssf   ESCAN00104560 FrTSyn transmits at most one SYNC or OFS message in one FlexRay cycle
 *            2019-11-06  visssf   ESCAN00104565 Debounce time not considered if several time masters use the same PDU
 *  05.00.00  2019-12-04  visgig   ESCAN00105100 Timeout not evaluated during jump width validation for offset time domain
 *            2020-01-31  visbfk   MSR4-193      Enhance precision of Global Time (AR RfC 79959)
 *            2020-02-26  visjwe   MSR4-28910    Clarify to enter cyclic operation right after timebase startup (AR-2920)
 *  05.01.00  2020-06-15  visjwe   TSYNC-1515    Rework QAC justifications for Helix QAC 2019-2
 *  05.02.00  2021-01-07  visbfk   TSYNC-1556    FrTSyn: Usage of Vector Code Analyzer
 *            2021-01-11  visssf   TSYNC-1924    Harmonization CanTSyn / FrTSyn
 *            2021-08-12  visssf   TSYNC-2644    Optimize initialization of local variables
 *  06.00.00  2021-11-16  visjwe   MSR4-31533    ASR memmap include structure ALL SLP
 *            2022-02-23  vissi    TSYNC-3052    FrTSyn: Remove unnecessary VCA Justifications
 *  06.01.00  2022-08-18  visssf   ESCAN00112274 Wrong re-synchronization after FR bus inactivity
 *            2022-12-09  vissi    TSYNC-4035    FrTSyn: QAC 2022.2 support
 *  06.02.00  2023-02-19  vissi    TSYNC-4438    Add MISRA justification for MsgID 0883 (Dir4.10)
 *            2023-08-07  fmommer  ESCAN00115227 FrTSynMasterTriggerTransmitTimeout smaller than main function period leads to maximum timeout value being used
 *            2023-08-09  visjwe   TSYNC-4315    Migrate FrTSyn to Test2020
 *            2023-08-16  fmommer  ESCAN00115242 Cyclic transmission is not started in the earliest possible FrTSyn_MainFunction after FrTSynGlobalTimeTxPeriod is reached
 *            2023-08-23  visjwe   TSYNC-4484    Add unit annotations for header files and remove NO_UNIT switches for prototypes
 *  06.03.00  2023-09-14  fmommer  TSYNC-4531    Harmonize design of FrTSyn and CanTSyn
 *            2023-10-27  fmommer  MSR4-35758    [FrTSyn] Unit Test Migration Test2020
 *            2023-10-27  fmommer  TSYNC-4568    FrTSyn: Improve variable scopes
 *            2023-11-16  fmommer  TSYNC-4617    Rework calculation functions for FrTSyn/CanTSyn
 *            2024-02-19  fmommer  ESCAN00116666 TriggerTransmitTimeout is not taken into account after sending an immediate SYNC message
 *********************************************************************************************************************/

#if !defined (FRTSYN_H)
# define FRTSYN_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "FrTSyn_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define FRTSYN_VENDOR_ID                        (30u)
# define FRTSYN_MODULE_ID                        (0xA3u)

/* AUTOSAR Software specification version information */
# define FRTSYN_AR_RELEASE_MAJOR_VERSION         (0x04u)
# define FRTSYN_AR_RELEASE_MINOR_VERSION         (0x04u)
# define FRTSYN_AR_RELEASE_REVISION_VERSION      (0x00u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define FRTSYN_SW_MAJOR_VERSION                 (6u)
# define FRTSYN_SW_MINOR_VERSION                 (3u)
# define FRTSYN_SW_PATCH_VERSION                 (0u)

# define FRTSYN_INSTANCE_ID_DET                  (0x00u)

/* ----- API service IDs ----- */
# define FRTSYN_SID_INIT                         (0x01u) /*!< Service ID: FrTSyn_Init() */
# define FRTSYN_SID_GET_VERSION_INFO             (0x02u) /*!< Service ID: FrTSyn_GetVersionInfo() */
# define FRTSYN_SID_SET_TRANSMISSION_MODE        (0x03u) /*!< Service ID: FrTSyn_SetTransmissionMode() */
# define FRTSYN_SID_MAIN_FUNCTION                (0x04u) /*!< Service ID: FrTSyn_MainFunction() */
# define FRTSYN_SID_TRIGGER_TRANSMIT             (0x41u) /*!< Service ID: FrTSyn_TriggerTransmit() */
# define FRTSYN_SID_RX_INDICATION                (0x42u) /*!< Service ID: FrTSyn_RxIndication() */


/* ----- Error codes ----- */
# define FRTSYN_E_NO_ERROR                       (0x00u) /*!< used to check if no error occurred */
# define FRTSYN_E_INVALID_PDUID                  (0x01u) /*!< Error code: API service called with wrong PDU or SDU  */
# define FRTSYN_E_UNINIT                         (0x20u) /*!< Error code: API service used in un-initialized state */
# define FRTSYN_E_NOT_INITIALIZED                (0x20u) /*!< Error code: API service used in un-initialized state */ /* for compatibility until AR 4.3.0 */
# define FRTSYN_E_NULL_POINTER                   (0x21u) /*!< Error code: API service called with NULL_PTR as parameter  */
# define FRTSYN_E_INIT_FAILED                    (0x22u) /*!< Error code: API service called and initialization failed  */
# define FRTSYN_E_PARAM                          (0x23u) /*!< Error code: API service called with invalid parameter  */
# define FRTSYN_E_INV_CTRL_IDX                   (0x24u) /*!< Error code: API service called with invalid controller index  */

/* ----- Modes ----- */
# define FRTSYN_UNINIT                           (0x00u)
# define FRTSYN_INIT                             (0x01u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define FRTSYN_START_SEC_CODE
# include "FrTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  FrTSyn_InitMemory()
 *********************************************************************************************************************/
/*! \brief       Function for *_INIT_*-variable initialization
 *  \details     Service to initialize module global variables at power up. This function can be used to initialize the
 *               variables in *_INIT_* sections in case they are not initialized by the startup code.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \unit        Common
 **********************************************************************************************************************/
FUNC(void, FRTSYN_CODE) FrTSyn_InitMemory(void);

/**********************************************************************************************************************
 *  FrTSyn_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization function
 *  \details     This function initializes the module FrTSyn. It initializes all variables and sets the module state to
 *               initialized.
 *  \param[in]   configPtr               Configuration structure for initializing the module.
 *  \pre         Interrupts are disabled and FrTSyn_InitMemory has been called if required.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \unit        Common
 *  \note        Specification of module initialization
 *********************************************************************************************************************/
FUNC(void, FRTSYN_CODE) FrTSyn_Init(P2CONST(FrTSyn_ConfigType, AUTOMATIC, FRTSYN_APPL_DATA) configPtr);

# if (FRTSYN_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     FrTSyn_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \unit        Common
 *  \config      FRTSYN_VERSION_INFO_API
 *********************************************************************************************************************/
FUNC(void, FRTSYN_CODE) FrTSyn_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FRTSYN_APPL_VAR) versioninfo);
# endif

/**********************************************************************************************************************
 *  FrTSyn_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Main function
 *  \details     This function processes cyclic tasks of the FrTSyn module.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \unit        Common
 *********************************************************************************************************************/
/* The FrTSyn_MainFunction definition is localized in FrTSyn.c. The declaration is realized by SchM */

# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_SetTransmissionMode()
 *********************************************************************************************************************/
/*! \brief       Set transmission mode
 *  \details     This function turns the TX capabilities of the FrTSyn on and off.
 *  \param[in]   CtrlIdx                 Index of the FlexRay channel.
 *  \param[in]   Mode                    Transmission on/off.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \unit        Common
 *  \config      FRTSYN_MASTERCONFIG
 *********************************************************************************************************************/
FUNC(void, FRTSYN_CODE) FrTSyn_SetTransmissionMode(uint8 CtrlIdx, FrTSyn_TransmissionModeType Mode);
# endif

# define FRTSYN_STOP_SEC_CODE
# include "FrTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*!
 *
 * \exclusivearea FRTSYN_EXCLUSIVE_AREA_0
 * Ensures consistency of global RAM variables.
 * \protects All affected global RAM variables.
 * \usedin FrTSyn_Tx_TransmitSyncRequest, FrTSyn_Tx_GetTimeSync,
 *         FrTSyn_Rx_OnReceiveSyncMsg, FrTSyn_Rx_OnReceiveOfsMsg
 * \exclude All functions provided by FrTSyn.
 * \length SHORT depending on the API and the call graph.
 * \endexclusivearea
 *
 */

#endif  /* FRTSYN_H */

/**********************************************************************************************************************
 *  END OF FILE: FrTSyn.h
 *********************************************************************************************************************/
