/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file  Can_30_Core.h
 *        \brief  Can_30_Core header file
 *        \details  Public header of the component. Upper layer external modules include this file.
 *        \unit Can_30_Core
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  00.05.00  2021-02-22  vishum     PS-748        Initial implementation
 *  01.00.00  2021-06-07  vishum     PS-747        Beta implementation
 *            2021-07-05  visgaz     CANCORE-487   [DrvCan_CoreGen3] Perform Static analysis for Beta version with reworks
 *  01.00.01  2021-08-31  vishum     CANCORE-545   Extend Can_30_Core_CancelTx() to handle Tx Full CANs 
 *  02.00.00  2021-10-04  visbir     CANCORE-569   Rework include structure, remove unused InitControllerMailboxActivate()
 *                                                 Add new header file: Extension API (Can_30_Core_vCan.h) like Cbk files
 *            2021-11-04  visgaz     CANCORE-770   Interface of OverrunOccurred in Core&Ext (return value is not used in the Core)
 *                                   CANCORE-86    Implement RTM hooks in static code
 *            2021-11-16  virbks     CANCORE-780   Add missing CDD for application callouts
 *            2021-11-19  virstl     CANCORE-539   Change of BusOffRecovery type
 *            2021-11-17  visgaz     CANCORE-787   Rework current implementation (HW/SW cancel)
 *            2021-11-23  visgaz     CANCORE-898   Add annotation (\units, \entities); Adapt inline functions for GoogleTest unit tests
 *                                   CANCORE-876   Use GetAddr() macros; Added missing unit includes
 *            2021-12-16             -             Reworked Can_30_Core_Write DET checks for FD
 *            2021-12-21             -             Reworked call of Can_30_Core_TxPrepare in Can_30_Core_Write
 *            2022-01-14             -             Reworked DET checks for controller parameter
 *            2022-01-17  virbks     -             Reworked CDD of Can_30_Core_Indication and Can_30_Core_CheckDLCLength
 *            2022-02-08  visbir     CANCORE-580   Remove Can_30_Core_InitPowerOn() and call to Extension-InitMemory
 *            2022-02-14  vishum     CANCORE-1018  Reworked Can_30_Core_IsFdTxBrs() and added Can_30_Core_IsFdBrs()
 *            2022-02-16  visbir     CANCORE-638   Exclusive Area 3 restricts Can_write() during TxConfirmation
 *                                                 set CAN_INTERNAL to support TickType for other components in generated data as uint32 but OS type for Core
 *                                                 Change 'PostBuildVariantOfGeneralFeature' to 'PostBuildSelectableOfGeneralFeature'
 *            2022-03-02  visgaz     CANCORE-1098  [DrvCan_CoreGen3] Implementation : New publics functions for setting FD and EXT flag in CanId
 *            2022-03-04  visbir     -             Do not include CanIf.h in can_cfg.h but in needed units
 *            2022-03-07  visbir     -             Skip Init in case of postbuild loadable issue (DET + BSW_ErrorHook)
 *            2022-03-10  visbir     CANCORE-743   Support Multipartition
 *            2022-03-21  visbir     CANCORE-1081  Remove GeneralFeature switch checks: ChangeBaudrateApi, SetBaudrateApi, VersionInfoApi and GetStatus (always true)
 *            2022-03-31  visbir                   Minor issue fix: Can_30_Core_GlobalInitState - without compiler abstraction and within wrong memory section
 *            2022-04-12  visbir                   Minor issue fix: Update DataModel
 *            2022-04-27  visgaz     CANCORE-1157  [DrvCan_CoreGen3] CanIf_30_CoreErrorNotification is called unconditionally
 *  02.01.00  2022-06-07  virepm     CANCORE-1234  Removed GetBusOffRecoveryCounter and SetBusOffRecoveryCounter functions
 *            2022-06-08  vispip     CANCORE-1296  Add new Error ID for generic hardware issues
 *            2022-06-08  vispip     CANCORE-1297  Add new Service ID for interrupt request
 *            2022-07-29  virbks     -             Minor issue fix: Update DataModel
 *            2022-08-03  virbks     -             Minor issue fix: Use Can_30_Core_IsCanIdLowerPrio instead of Can_30_Core_IsCanIdHigherPrio
 *            2022-08-26  virbks     CANCORE-1399  Verify traceability
 *            2022-09-01  visjan     CANCORE-1425  Table misalignment with types in different units due to inconsistent usage of Os Types
 *            2022-09-02  visbir                   Trivial change: add external definition Can_30_Core_Disable/EnableControllerInterrupts() in Can_30_Core_vCan.h - may used in vCan's
 *            2022-09-13  visbir     CANCORE-1453  Trivial change: QAC comments and casts
 *  02.02.00  2022-10-06  virbks     -             Trivial change: prepare next version
 *            2022-10-11  destler    CANCORE-1503  Improvements for static code
 *            2022-10-19  destler    CANCORE-1059  Reworked all DET checks
 *            2022-11-11  destler    CANCORE-1479  Traces updated according to changes in CADS
 *            2022-11-14  vispip     CANCORE-1511  Add COV comment for wakeup in init and precondition for Can_30_Core_Indication()
 *            2022-11-10  visbir     CANCORE-548   Support Silent
 *            2022-11-23  visbir     -             Minor reworks in TxRequestHandler (inline and rename of local functions)
 *  02.02.01  2023-03-31  visbir     CANCORE-1886  RAM check issue fix notification
 *                                   CANCORE-1888  RAM check issue fix MB
 *                                   CANCORE-1935  SafeBsw check fix
 *                                                 ESCAN00114715 CancelTx with too short critical section
 *  02.03.00  2023-05-26  visjan     CANCORE-1602  Finite Execution Time Analysis
 *                                   CANCORE-1998  Fix ESCAN00113285
 *                                   CANCORE-929   MemMap Generation
 *                                   CANCORE-1979  CSL ConstantDataToDefine Optimization: Add MISRA justifications
 *            2023-06-16  hjenhani   CANCORE-1938  Implement InitPowerOn callout to the extension
 *                                   CANCORE-1470  Add Runtime Measurement hooks for Rx
 *            2023-07-04  visbir     CANCORE-1997  Baudrate start-stop index as CSL03
 *                                   CANCORE-2043  Fix ESCAN00115020 / EcuM call abstraction
 *            2023-07-18  hjenhani   CANCORE-1785  Improve Exclusive Area handling in MainFunctionMode
 *                                   CANCORE-90    Merge Exclusive Area 2 and Exclusive Area 3
 *            2023-08-04  visbir     CANCORE-2080  call vCan polling handler only if polling is active
 *                                   ESCAN00115231 store LastInitObject before set new for recovery
 *  02.04.00  2023-11-24  visbir     CANCORE-2308  Fix ESCAN00116122 Wrong brs bit check in Can_Write()
 *            2023-12-05  hjenhani   CANCORE-2303  Check controller mode before to allow wakeup event
 *            2023-12-05  hjenhani   CANCORE-2294  Add new Core/extension interface Can_30_Core_BswErrorHook
 *  02.05.00  2024-01-10  hjenhani   CANCORE-2403  Remove Extended RAMCheck feature
 *  main-1    2024-02-26  visgaz     CANCORE-2371  Change history is maintained in the global ChangeHistory.txt file starting with this release
 *********************************************************************************************************************/

#if !defined(CAN_30_CORE_H)
#define CAN_30_CORE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core_Cfg.h"
#include "Can_30_Core_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define CAN_30_CORE_SW_MAJOR_VERSION                    (4u)
# define CAN_30_CORE_SW_MINOR_VERSION                    (1u)
# define CAN_30_CORE_SW_PATCH_VERSION                    (0u)

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_VAR_INIT_UNSPECIFIED
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to the configuration */
extern P2CONST(Can_30_Core_ConfigType, CAN_30_CORE_VAR_CLEARED, CAN_30_CORE_PBCFG) Can_30_Core_ConfigDataPtr; /* PRQA S 3449,3451 */ /* MD_Can_30_Core_ExternalScope */

#define CAN_30_CORE_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/*********  TxRequestHandler Unit  ***********************************************************************************/
/* \unit Can_30_Core_TxRequestHandler */


/**********************************************************************************************************************
 *  Can_30_Core_Write()
 *********************************************************************************************************************/
/*! \brief         Requests a transmission of a PDU as CAN frame.
 *  \details       Forwards the transmit requests to the Extensions.
 *  \param[in]     hth           Handle of the CanHardwareObject intended to transmit the PDU.
 *  \param[in]     PduInfo       Pointer to the PDU information.
 *                               Caller must ensure the consistency of the PDU information.
 *  \return        CAN_NOT_OK    Transmit request rejected.
 *  \return        CAN_OK        Transmit request successful.
 *  \return        CAN_BUSY      Transmit request could not be accomplished due to the CanHardwareObject is busy.
 *  \pre           CAN interrupts has to be disabled/locked (Refer to critical section).
 *                 Controller must be in Start mode.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Can_ReturnType, CAN_30_CORE_CODE) Can_30_Core_Write(Can_HwHandleType hth, Can_PduInfoPtrType PduInfo);

/**********************************************************************************************************************
 *  Can_30_Core_CancelTx()
 *********************************************************************************************************************/
/*! \brief         Cancel TX message
 *  \details       Cancel the TX message in the hardware buffer (if possible) or mark the message as not to be confirmed
 *                 in case of the cancellation is unsuccessful.
 *  \param[in]     Hth     Handle of the mailbox intended to be cancelled.
 *  \param[in]     PduId   PDU identifier
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Do not call this API within TX confirmation context. Otherwise confirmation will occur for already
 *                 cancelled messages. Only one message will be cancelled, if there are multiple requests also multiple
 *                 cancellations expected.
 *                 None AUTOSAR API
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_CancelTx(Can_HwHandleType Hth, PduIdType PduId);


/*********  TxConfirmationHandler Unit  ******************************************************************************/
/* \unit Can_30_Core_TxConfirmationHandler */


/**********************************************************************************************************************
 *  Can_30_Core_MainFunction_Write_Internal()
 *********************************************************************************************************************/
/*! \brief       MainFunction Write
 *  \details     Calls the polling tasks of the extension and handles Tx Confirmation.
 *  \param[in]   ApplicationId     Handle of the OS application calling this main function.
 *  \pre         Must not be called nested and must not be interrupted by other CAN main function calls.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_MainFunction_Write_Internal(Can_30_Core_OsApplicationType ApplicationId);
/* Wrapper function with OsApplication name instead of _'Internal' and without parameter will be generated */

/*********  TxBufferHandler Unit  ************************************************************************************/


/*********  RxIndicationHandler Unit  ********************************************************************************/
/* \unit Can_30_Core_RxIndicationHandler */


/**********************************************************************************************************************
 *  Can_30_Core_MainFunction_Read_Internal()
 *********************************************************************************************************************/
/*! \brief       Mainfunction Read
 *  \details     Calls the polling tasks of the extensions and handles the RxQueue.
 *  \param[in]   ApplicationId     Handle of the OS application or partition calling this main function.
 *  \pre         Must not be called nested and must not be interrupted by other CAN main function calls. (SPEC-1569)
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_MainFunction_Read_Internal(Can_30_Core_OsApplicationType ApplicationId);
/* Wrapper function with OsApplication name instead of _'Internal' and without parameter will be generated */


/*********  RxQueueHandler Unit  *************************************************************************************/


/*********  InterruptService Unit  ***********************************************************************************/
/* \unit Can_30_Core_InterruptService */


/**********************************************************************************************************************
 *  Can_30_Core_DisableControllerInterrupts()
 *********************************************************************************************************************/
/*! \brief       Disables the CAN controller interrupts.
 *  \details     -
 *  \param[in]   Controller  The requested CAN controller
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_DisableControllerInterrupts(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_EnableControllerInterrupts()
 *********************************************************************************************************************/
/*! \brief       Enables the CAN controller interrupts.
 *  \details     -
 *  \param[in]   Controller  The requested CAN controller
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_EnableControllerInterrupts(uint8 Controller);


/*********  DiagnosticHandler Unit  **********************************************************************************/


/*********  Version Unit  ********************************************************************************************/
/* \unit Can_30_Core_Version */


/**********************************************************************************************************************
 *  Can_30_Core_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information
 *  \details     Returns version information, vendor ID and AUTOSAR module ID.
 *  \param[in]   VersionInfo    Pointer to the version information.
                                Parameter must not be NULL and available for write access.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_GetVersionInfo(Can_30_Core_VersionInfoPtrType VersionInfo);


/*********  ExclusiveAreaHandler Unit  *******************************************************************************/


/*********  TimeoutHandler Unit  *************************************************************************************/


/*********  ModeHandler Unit  ****************************************************************************************/
/* \unit Can_30_Core_ModeHandler */


/**********************************************************************************************************************
 *  Can_30_Core_GetControllerMode()
 *********************************************************************************************************************/
/*! \brief       Get the controller mode
 *  \details     Returns the mode of the given controller
 *  \param[in]   Controller           The requested controller.
 *  \param[in]   ControllerModePtr    Pointer to variable in which the driver will store the controller mode.
                                      Parameter must not be NULL and available for write access.
 *  \return      E_NOT_OK             Controller mode request was not accepted
 *  \return      E_OK                 Controller mode request was accepted
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       DSGN-CanCore-HwMode_StateMachineModeChange
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_GetControllerMode(
  uint8 Controller,
  Can_30_Core_ControllerStatePtrType ControllerModePtr);

/**********************************************************************************************************************
 *  Can_30_Core_GetMirrorMode()
 *********************************************************************************************************************/
/*! \brief       Get the Mirror mode state
 *  \details     Returns the mirror mode state of the requested controller
 *  \param[in]   Controller            The requested controller
 *  \return      CDDMIRROR_INACTIVE    Mirror mode is inactive
 *  \return      CDDMIRROR_ACTIVE      Mirror mode is active
 *  \pre         MirrorMode Feature is globally enabled
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(CddMirror_MirrorModeType, CAN_30_CORE_CODE) Can_30_Core_GetMirrorMode(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_SetMirrorMode()
 *********************************************************************************************************************/
/*! \brief       Set the Mirror mode state
 *  \details     Sets the mirror mode state of the requested controller
 *  \param[in]   Controller    The requested controller
 *  \param[in]   State         The requested state
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_SetMirrorMode(uint8 Controller, CddMirror_MirrorModeType State);

/**********************************************************************************************************************
 *  Can_30_Core_GetSilentMode()
 *********************************************************************************************************************/
/*! \brief       Get the Silent mode state
 *  \details     Returns the Silent mode state of the requested controller.
 *  \param[in]   Controller                     The requested controller
 *  \return      CAN_30_CORE_SILENT_INACTIVE    Silent mode is inactive
 *  \return      CAN_30_CORE_SILENT_ACTIVE      Silent mode is active
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Can_30_Core_SilentModeType, CAN_30_CORE_CODE) Can_30_Core_GetSilentMode(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_SetSilentMode()
 *********************************************************************************************************************/
/*! \brief       Set the silent mode state
 *  \details     Sets the silent mode state of the requested controller
 *  \param[in]   Controller    The requested controller
 *  \param[in]   SilentMode    The requested silent mode state
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_SetSilentMode(uint8 Controller, Can_30_Core_SilentModeType SilentMode);

/**********************************************************************************************************************
 *  Can_30_Core_CheckWakeup()
 *********************************************************************************************************************/
/*! \brief       Check wakeup occurrance
 *  \details     Check the occurrence of WAKEUP events for the given controller \n
 *               (used as WAKEUP callback for higher layers).
 *  \param[in]   Controller           The requested controller
 *  \return      E_OK/CAN_OK          The given controller caused a WAKEUP before.
 *  \return      E_NOT_OK/CAN_NOT_OK  The given controller caused no WAKEUP before.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       DSGN-CanCore-HwMode_WakeupHandling
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_CheckWakeup(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_MainFunction_Mode_Internal()
 *********************************************************************************************************************/
/*! \brief       Mode transition observation
 *  \details     Polls the mode changes for all the controllers asynchronous if not accomplished in Can_30_Core_SetControllerMode()
 *  \param[in]   ApplicationId     Handle of the OS application or partition calling this mainfunction.
 *  \pre         Must not be called nested and must not be interrupted by other CAN main function calls.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_MainFunction_Mode_Internal(Can_30_Core_OsApplicationType ApplicationId);
/* Wrapper function with OsApplication name instead of _'Internal' and without parameter will be generated */


/*********  ModeRequest Unit  ****************************************************************************************/
/* \unit Can_30_Core_ModeRequest */


/**********************************************************************************************************************
 *  Can_30_Core_SetControllerMode()
 *********************************************************************************************************************/
/*! \brief       Change the controller mode
 *  \details     Requests a mode transition that will be performed synchronously within a short time \n
 *               The mode change might be finished asynchronously within the Can_30_Core_MainFunction_Mode() if not here.
 *  \param[in]   Controller    The requested controller
 *  \param[in]   Transition    The requested transition mode (CAN_T_START, CAN_T_STOP, CAN_T_SLEEP, CAN_T_WAKEUP)
 *  \return      CAN_NOT_OK    Transition request rejected
 *  \return      CAN_OK        Transition request accepted
 *  \pre         Must not be called within CAN driver context (Rx, Tx or Bus off callouts)
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \trace       DSGN-CanCore-HwMode_ModeChange
 *********************************************************************************************************************/
FUNC(Can_ReturnType, CAN_30_CORE_CODE) Can_30_Core_SetControllerMode(uint8 Controller, Can_StateTransitionType Transition);

/*********  Initialization Unit  *************************************************************************************/
/* \unit Can_30_Core_Initialization */


/**********************************************************************************************************************
 *  Can_30_Core_Init()
 *********************************************************************************************************************/
/*! \brief         Initializes component
 *  \details       Initializes all component variables and sets the component state to initialized.
 *  \param[in]     ConfigPtr          Component configuration structure
 *  \pre           Interrupts are disabled.
 *  \pre           Module is uninitialized.
 *  \pre           Can_30_Core_InitMemory() has been called unless CAN Module is initialized by start up code.
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Parameter "ConfigPtr" will be taken into account only for "Multiple ECU Configuration" and in Post-Build variant
 *  \trace         DSGN-CanCore-Init_Initialization
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_Init(Can_30_Core_ConfigPtrType ConfigPtr);

/**********************************************************************************************************************
 *  Can_30_Core_InitMemory()
 *********************************************************************************************************************/
/*! \brief         Power-up memory initialization
 *  \details       Initializes component variables in *_INIT_* sections at power up. \n
 *                 Use this to re-run the system without performing a new start from power on. \n
 *                 (E.g.: used to support an ongoing debug session without a complete re-initialization.) \n
 *  \param[in]     -
 *  \pre           Module is not initialized or in STOP mode.
 *  \note          Must be followed by a call to "Can_30_Core_Init()".
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         DSGN-CanCore-Init_InitMemory
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_InitMemory(void);

/*********  ErrorHandler Unit  ***************************************************************************************/
/* \unit Can_30_Core_ErrorHandler */


/**********************************************************************************************************************
 *  Can_30_Core_MainFunction_BusOff_Internal()
 *********************************************************************************************************************/
/*! \brief       Bus off observation
 *  \details     Polling of bus off events to accomplish the bus off handling for all CAN controllers.
 *  \param[in]   ApplicationId     Handle of the OS application or partition calling this main function.
 *  \pre         Must not be called nested and must not be interrupted by other CAN main function calls.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_MainFunction_BusOff_Internal(Can_30_Core_OsApplicationType ApplicationId);
/* Wrapper function with OsApplication name instead of _'Internal' and without parameter will be generated */

/**********************************************************************************************************************
 *  Can_30_Core_GetStatus()
 *********************************************************************************************************************/
/*! \brief       Get status/mode of the controller
 *  \details     Delivers the status of the hardware
 *  \param[in]   Controller       CAN controller requested for status information.
 *  \return      CAN_30_CORE_STATUS_START            START mode
 *  \return      CAN_30_CORE_STATUS_STOP             STOP mode
 *  \return      CAN_30_CORE_STATUS_INIT             Initialized controller
 *  \return      CAN_30_CORE_STATUS_INCONSISTENT     STOP or SLEEP are inconsistent over common CAN controllers
 *  \return      CAN_30_CORE_DEACTIVATE_CONTROLLER   RAM check failed CAN controller is deactivated
 *  \return      CAN_30_CORE_STATUS_WARNING          WARNING state
 *  \return      CAN_30_CORE_STATUS_PASSIVE          PASSIVE state
 *  \return      CAN_30_CORE_STATUS_BUSOFF           BUSOFF mode
 *  \return      CAN_30_CORE_STATUS_SLEEP            SLEEP mode
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        None AUTOSAR API
 *  \trace       DSGN-CanCore-HwMode_GetStatus
 *********************************************************************************************************************/
FUNC(uint8, CAN_30_CORE_CODE) Can_30_Core_GetStatus(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_GetControllerErrorState()
 *********************************************************************************************************************/
/*! \brief       Get status of the given controller
 *  \details     Delivers the controllers error state.
 *  \param[in]   Controller       CAN controller requested for status information
 *  \param[in]   ErrorStatePtr    Pointer to variable to store CAN controllers error state.
                                  Parameter must not be NULL and available for write access.
 *  \return      E_NOT_OK         Controller state request has not been accepted
 *  \return      E_OK             Controller state request has been accepted
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       DSGN-CanCore-HwMode_ErrorStates
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_GetControllerErrorState(uint8 Controller, Can_ErrorStatePtrType ErrorStatePtr);

/**********************************************************************************************************************
 *  Can_30_Core_GetControllerRxErrorCounter()
 *********************************************************************************************************************/
/*! \brief       Gets the Rx Counter
 *  \details     Delivers the RX counter for the requested controller
 *  \param[in]   Controller           Requested CAN controller
 *  \param[in]   RxErrorCounterPtr    Pointer to variable to store CAN controllers RX error counter. 
                                      Parameter must not be NULL and available for write access.
 *  \return      E_NOT_OK             Controller RX error counter not available
 *  \return      E_OK                 Controller RX error counter available
 *  \pre         RxErrorCounterPtr must not be NULL.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       DSGN-CanCore-HwMode_ErrorStates
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_GetControllerRxErrorCounter(
  uint8 Controller, 
  Can_30_Core_ErrorCounterPtrType RxErrorCounterPtr);

/**********************************************************************************************************************
 *  Can_30_Core_GetControllerTxErrorCounter()
 *********************************************************************************************************************/
/*! \brief       Gets the TX Counter
 *  \details     Delivers the TX counter for the requested controller
 *  \param[in]   Controller           Requested CAN controller
 *  \param[in]   TxErrorCounterPtr    Pointer to variable to store CAN controllers TX error counter. 
                                      Parameter must not be NULL and available for write access.
 *  \return      E_NOT_OK             Controller TX error counter not available
 *  \return      E_OK                 Controller TX error counter available
 *  \pre         TxErrorCounterPtr must not be NULL.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       DSGN-CanCore-HwMode_ErrorStates
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_GetControllerTxErrorCounter(
  uint8 Controller, 
  Can_30_Core_ErrorCounterPtrType TxErrorCounterPtr);


/*********  WakeupHandler Unit  **************************************************************************************/
/* \unit Can_30_Core_WakeupHandler */


/**********************************************************************************************************************
 *  Can_30_Core_MainFunction_Wakeup_Internal()
 *********************************************************************************************************************/
/*! \brief       Wake up observation
 *  \details     Polling WAKEUP events for all controllers to accomplish the WAKEUP handling
 *  \param[in]   ApplicationId     Handle of the OS application or partition calling this mainfunction.
 *  \pre         Must not be called nested and must not be interrupted by other CAN main function calls. \n
 *               CAN driver has to be initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_MainFunction_Wakeup_Internal(Can_30_Core_OsApplicationType ApplicationId);
/* Wrapper function with OsApplication name instead of _'Internal' and without parameter will be generated */


/*********  Baudrate Unit  *******************************************************************************************/
/* \unit Can_30_Core_Baudrate */


/**********************************************************************************************************************
 *  Can_30_Core_ChangeBaudrate()
 *********************************************************************************************************************/
/*! \brief       Change baud rate
 *  \details     This service shall change the baud rate and re-initialize the CAN controller.
 *  \param[in]   Controller    CAN Controller to be changed
 *  \param[in]   Baudrate      Baud rate to be set
 *  \return      E_NOT_OK      Baud rate was not set
 *  \return      E_OK          Baud rate was set
 *  \pre         The CAN controller must be in "STOP" mode
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       DSGN-CanCore-Init_ChangeBaudrate
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_ChangeBaudrate(uint8 Controller, uint16 Baudrate);

/**********************************************************************************************************************
 *  Can_30_Core_CheckBaudrate()
 *********************************************************************************************************************/
/*! \brief       Checks the baud rate
 *  \details     This service shall check if a certain CAN controller supports a requested baudrate.
 *  \param[in]   Controller    CAN controller to be checked
 *  \param[in]   Baudrate      Baud rate to be checked
 *  \return      E_NOT_OK      Baud rate is not available
 *  \return      E_OK          Baud rate is available
 *  \pre         The CAN controller must be initialized
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace      DSGN-CanCore-Init_ChangeBaudrate
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_CheckBaudrate(uint8 Controller, uint16 Baudrate);

/**********************************************************************************************************************
 *  Can_30_Core_SetBaudrate()
 *********************************************************************************************************************/
/*! \brief       Set baud rate
 *  \details     The service shall change the baud rate and re-initialize the CAN controller.
 *  \param[in]   Controller          Controller to be set
 *  \param[in]   BaudRateConfigID    Identity of the configured baud rate (available as Symbolic Name)
 *  \return      E_NOT_OK            Baud rate was not set
 *  \return      E_OK                Baud rate was set
 *  \pre         The CAN controller must be in "STOP" mode.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \trace       DSGN-CanCore-Init_ChangeBaudrate
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID);


/**********************************************************************************************************************
 *  MODULE LOCAL HELPER FUNCTION
 *********************************************************************************************************************/

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  API DESCRIPTION FOR APPLICATION CALLOUTS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_CoreCanTimerStart()
 *********************************************************************************************************************/
/*! \fn Appl_30_CoreCanTimerStart
 *  \brief         Start time out monitoring.
 *  \details       Service function to start an observation timer.
 *  \param[in]     Controller  CAN controller on which the hardware observation takes place. \n
 *  \param[in]     source      Source for the hardware observation.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_CoreCanTimerEnd()
 *********************************************************************************************************************/
/*! \fn Appl_30_CoreCanTimerEnd
 *  \brief         Stop time out monitoring.
 *  \details       Service function to to end an observation timer.
 *  \param[in]     Controller  CAN controller on which the hardware observation takes place. \n
 *  \param[in]     source      Source for the hardware observation.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_CoreCanTimerLoop()
 *********************************************************************************************************************/
/*! \fn Appl_30_CoreCanTimerLoop
 *  \brief         Time out monitoring.
 *  \details       Service function to check (against generated maximum loop value) whether a hardware loop shall be continued or broken.
 *  \param[in]     Controller  CAN controller on which the hardware observation takes place. \n
 *  \param[in]     source      Source for the hardware observation.
 *  \return        CAN_NOT_OK  When loop shall be broken (observation stops). \n
 *                             CAN_NOT_OK should only be used in case of a time out occurs due to a hardware issue. \n
 *                             After this an appropriate error handling is needed (see chapter Hardware Loop Check / Time out Monitoring).
 *  \return        CAN_OK      When loop shall be continued (observation continues).
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_Core_GenericConfirmation() API1
 *********************************************************************************************************************/
/*! \fn Appl_30_Core_GenericConfirmation
 *  \brief         Common TX notification callback that will be called before message specific callback will be called.
 *  \details       Application callback function which informs about TX messages being sent to the CAN bus.
 *                 It can be used to block confirmation or route the information to other layer as well.
 *  \param[in]     PduId       Handle of the PDU specifying the message.
 *  \return        CAN_OK      Higher layer confirmation will be called afterwards (CanIf_TxConfirmation()).
 *  \return        CAN_NOT_OK  Higher layer confirmation will not be called afterwards.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note         "PduId" is read only and must not be accessed for further write operations. \n
 *                 None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_Core_GenericConfirmation() API2
 *********************************************************************************************************************/
/*! \fn Appl_30_Core_GenericConfirmation
 *  \brief         Common TX notification callback that will be called before message specific callback will be called.
 *  \details       Application callback function which informs about TX messages being sent to the CAN bus.
 *                 It can be used to block confirmation or route the information to other layer as well.
 *  \param[in]     Controller  CAN controller which send the message.
 *  \param[in]     DataPtr     Pointer to a Can_PduType structure including CAN-Id (containing IDE,FD bit), DataLength, PDU and data pointer (read only).
 *  \return        CAN_OK      Higher layer confirmation will be called afterwards (CanIf_TxConfirmation()).
 *  \return        CAN_NOT_OK  Higher layer confirmation will not be called afterwards.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          A new transmission within this call out will corrupt the DataPtr context, if the CanHardwareObject is a FIFO object and the FIFO is full. \n
 *                 None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_Core_GenericPrecopy()
 *********************************************************************************************************************/
/*! \fn Appl_30_Core_GenericPrecopy
 *  \brief         Common RX indication callback that will be called before message specific callback will be called.
 *  \details       Application callback function which informs about all incoming RX messages including the contained data. \n
 *                 It can be used to block notification to upper layer. E.g. to filter incoming messages or route it for special handling.
 *  \param[in]     Controller   CAN controller which received the message.
 *  \param[in]     ID           CAN-Id of the received message (including IDE,FD). \n
 *                              In case of extended or mixed CAN-Id systems the highest bit (bit 31) is set to mark an extended CAN-Id. \n
 *                              FD-bit (bit 30) can be masked out with a define in the user config file.
 *  \param[in]     DataLength   Data length of the received message.
 *  \param[in]     DataPtr      Pointer to the data of the received message (read only).
 *  \return        CAN_OK       Higher layer indication will be called afterwards (CanIf_RxIndication()).
 *  \return        CAN_NOT_OK   Higher layer indication will not be called afterwards.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          "DataPtr" is read only and must not be accessed for further write operations (explicitly not const because of backward compatibility).\n
 *                 The CAN protocol allows the usage of data length values greater than eight (CAN-FD).\n
 *                 Depending on the implementation of this callback it may be necessary to consider this special case (e.g. if the data length is used as index value in a buffer write access).
 *                 None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_Core_GenericPreTransmit()
 *********************************************************************************************************************/
/*! \fn Appl_30_Core_GenericPreTransmit
 *  \brief         Common transmit callback.
 *  \details       Application callback function allowing the modification of the data to be transmitted (e.g.: add CRC).
 *  \param[in]     Controller  CAN controller on which the message will be send.
 *  \param[in]     PduPtr      Pointer to a Can_PduType structure including CAN-Id (containing IDE,FD bit), DataLength, PDU and data pointer.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Do not extend PduPtr->length beyond the maximum length for the given PduPtr->swPduHandle (see related CanHardwareObject\CanMaxDataLen).
 *                 Do not write beyond the PduPtr->length to the PduPtr->sdu.
 *                 None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_Core_CanCorruptMailbox()
 *********************************************************************************************************************/
/*! \fn Appl_30_Core_CanCorruptMailbox
 *  \brief         Mailbox notification in case of RAM check failure.
 *  \details       Will notify the application (during re-initialization) about a defect mailbox within the CAN cell.
 *  \param[in]     Controller   CAN controller for which the check failed.
 *  \param[in]     mailboxHandle  Hardware handle of the defect mailbox.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_Core_CanRamCheckFailed()
 *********************************************************************************************************************/
/*! \fn Appl_30_Core_CanRamCheckFailed
 *  \brief         CAN controller notification in case of RAM check failure.
 *  \details       Will notify the application (during re-initialization) about a defect CAN controller \n
 *                 due to a previous failed mailbox check. The return value decides how to proceed with the initialization.
 *  \param[in]     Controller   CAN controller for which the check failed.
 *  \return        CAN_DEACTIVATE_CONTROLLER   Deactivate the controller.
 *  \return        CAN_ACTIVATE_CONTROLLER     Activate the controller.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_CoreCanInterruptDisable()
 *********************************************************************************************************************/
/*! \fn Appl_30_CoreCanInterruptDisable
 *  \brief         CAN interrupt disabling by application.
 *  \details       Disabling of CAN Interrupts by the application. \n
 *                 E.g.: The CAN driver itself should not access the common Interrupt Controller due to application \n
 *                 specific restrictions (like security level). Or the application likes to be informed because of \n
 *                 an CAN interrupt lock. Called by Can_DisableControllerInterrupts().
 *  \param[in]     Controller   CAN controller for the CAN interrupt lock.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_CoreCanInterruptRestore()
 *********************************************************************************************************************/
/*! \fn Appl_30_CoreCanInterruptRestore
 *  \brief         CAN interrupt restore by application.
 *  \details       Re-enabling of CAN Interrupts by the application. \n
                   E.g.: The CAN driver itself should not access the common Interrupt Controller due to application \n
                   specific restrictions (like security level). Or the application likes to be informed because of \n
                   an CAN interrupt lock. Called by Can_EnableControllerInterrupts().
 *  \param[in]     Controller   CAN controller for the CAN interrupt unlock.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_Core_CanOverrun()
 *********************************************************************************************************************/
/*! \fn Appl_30_Core_CanOverrun
 *  \brief         Overrun detection.
 *  \details       Called when an overrun is detected for a BasicCAN mailbox. \n
 *                 Alternatively a DET call can be selected instead of ("CanOverrunNotification" is set to "DET").
 *  \param[in]     Controller   CAN controller for which the overrun was detected.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          None AUTOSAR API
**********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_30_Core_CanFullCanOverrun()
 *********************************************************************************************************************/
/*! \fn Appl_30_Core_CanFullCanOverrun
 *  \brief         Overrun detection.
 *  \details       Called when an overrun is detected for a FullCAN mailbox. \n
 *                 Alternatively a DET call can be selected instead of ("CanOverrunNotification" is set to "DET").
 *  \param[in]     Controller   CAN controller for which the overrun was detected.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          None AUTOSAR API
**********************************************************************************************************************/

/*!
 * \exclusivearea CAN_30_CORE_EXCLUSIVE_AREA_1
 * Ensures consistency while modifying the interrupt counter and CAN interrupt lock registers.
 * \protects The interrupt handling against concurrent access to the interrupt status and enable registers, as well as nesting counters.
 * \usedin Can_30_Core_DisableControllerInterrupts, Can_30_Core_EnableControllerInterrupts
 * \exclude All functions provided by Can.
 * \length SHORT The interrupt registers and a counter variable are modified.
 * \endexclusivearea
 *
 * \exclusivearea  CAN_30_CORE_EXCLUSIVE_AREA_2
 * Ensures consistency while modifying the TX mailbox buffer states and data.
 * Ensures send order (PIA) while CanIf TX buffer reload. CanIf_CancelTxConfirmation interrupted by Can_30_Core_Write may lead to send priority inversion.
 * \protects TX buffer states and CanHardwareObject registers.
 * \usedin Can_30_Core_Write, Can_30_Core_CancelTx, Can_30_Core_MainFunction_Write, Can TX Interrupt
 * \exclude Can_30_Core_Write, Can_30_Core_MainFunction_Write, Can TX Interrupt handling
 * \length MEDIUM Multiplexed TX and cancellation handling. Cancellation handling by CAN Interface triggered. CAN TX mailbox registers and semaphore for locked mailbox modified.
 * \externalcall CanIf_CancelTxConfirmation
 * \endexclusivearea
 *
 * \exclusivearea  CAN_30_CORE_EXCLUSIVE_AREA_4
 * Ensures consistency of RX Queue.
 * \protects RX Queue counter and data.
 * \usedin Can_30_Core_PutMessage, Can_30_Core_RxQueueExecution
 * \exclude Can_30_Core_MainFunction_Read, Can RX Interrupt
 * \length SHORT Modify queue counter and copy data to queue.
 * \externalcall VStdMemCpy
 * \endexclusivearea
 *
 * \exclusivearea  CAN_30_CORE_EXCLUSIVE_AREA_5
 * Ensures atomic wakeup mode transition. Do not use CAN interrupt locks because CAN interrupt status may be changed within.
 * Inside this exclusive Area, it is ensured that the call to the extension WakeupHandler and the call of Can_30_Core_WakeupHandling (which will call Can_30_Core_SetControllerMode and EcuM_CheckWakeup) will be atomic
 * resulting a protection against any additional mode transition request that could happen in between from an interrupt context.
 * \protects wakeup status and mode information.
 * \usedin Can_30_Core_MainFunction_Wakeup
 * \exclude Can_30_Core_SetControllerMode, Can BusOff Interrupt, Can Wakeup, Can_30_Core_MainFunction_Wakeup.
 * \length MEDIUM Wakeup mode transition will be performed.
 * \externalcall EcuM_CheckWakeup
 * \endexclusivearea
 *
 * \exclusivearea  CAN_30_CORE_EXCLUSIVE_AREA_6
 * Ensures atomic mode transition.
 * \protects CAN status and mode information.
 * \usedin Can_30_Core_MainFunction_Mode, Can_30_Core_SetControllerMode, Can_30_Core_MainFunction_BusOff, Can BusOff Interrupt, Can_30_Core_ChangeBaudrate, Can_30_Core_SetBaudrate
 * \exclude Can_30_Core_MainFunction_Mode, Can_30_Core_SetControllerMode, Can_30_Core_MainFunction_BusOff, Can BusOff Interrupt, Can_30_Core_MainFunction_Wakeup, Can_30_Core_ChangeBaudrate, Can_30_Core_SetBaudrate
 * \length MEDIUM Mode transition will be performed.
 * \externalcall CanIf_30_Core_ControllerModeIndication
 * \endexclusivearea
 *
 * \exclusivearea  CAN_30_CORE_EXCLUSIVE_AREA_7
 * Ensures consistency of TX HW FIFO.
 * \protects TX FIFO counter and data
 * \usedin Can_30_Core_UpdateTxFifoReadIndex, Can_30_Core_UpdateTxFifoWriteIndex
 * \exclude Can_30_Core_MainFunction_Write, Can TX Interrupt
 * \length SHORT Modify queue counter and copy data to queue.
 * \endexclusivearea
 */

#endif /* CAN_30_CORE_H */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core.h
 *********************************************************************************************************************/
