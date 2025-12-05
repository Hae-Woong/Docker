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
/*!       \file  Can_30_Core_vCan.h
 *        \brief  Can_30_Core_vCan header file
 *        \details  Public header of the component. Lower layer extension modules include this file.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *********************************************************************************************************************/

#if !defined(CAN_30_CORE_VCAN_H)
# define CAN_30_CORE_VCAN_H

/**********************************************************************************************************************
 * LOCAL MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2880, 2991, 2992, 2994, 2995, 2996 EOF */ /* MD_Can_30_Core_ConstValue */
/* PRQA  S 2741, 2742 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Can_30_Core_Cfg.h"
# include "Can_30_Core_Types.h"
# include "Can_30_Core_PrivateTypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define CAN_30_CORE_START_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*********  General Unit  ***********************************************************************************/
/* \unit Can_30_Core_General */

/**********************************************************************************************************************
 *  Can_30_Core_IsFdMessage()
 *********************************************************************************************************************/
/*! \brief       Checks if message is FD
 *  \details     Checks the message is an CAN-FD message
 *  \param[in]   CanId    The CAN ID
 *  \return      FALSE    Message is not CAN-FD
 *  \return      TRUE     Message is CAN-FD
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsFdMessage(Can_IdType CanId);

/**********************************************************************************************************************
 *  Can_30_Core_IsExtendedIdMessage()
 *********************************************************************************************************************/
/*! \brief       Extended ID check
 *  \details     Checks if the given CAN-ID is extended
 *  \param[in]   CanId    The CAN ID
 *  \return      FALSE    The CAN ID is not extended
 *  \return      TRUE     The CAN ID is extended
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsExtendedIdMessage(Can_IdType CanId);

/**********************************************************************************************************************
 *  Can_30_Core_IsFdBrs()
 *********************************************************************************************************************/
/*! \brief       Gets the BRS state
 *  \details     Returns the current state of the baud rate switch for the active baud rate 
 *  \param[in]   Controller    The requested controller
 *  \return      FALSE         BRS is inactive for the current baudrate
 *  \return      TRUE          BRS is active for the current baudrate
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsFdBrs(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_IsFdTxBrs()
 *********************************************************************************************************************/
/*! \brief       Gets the Tx BRS state
 *  \details     Returns the current state of the transmission baud rate switch for the active baud rate
 *  \param[in]   Controller    The requested controller
 *  \return      FALSE         Tx BRS is inactive for the current baudrate
 *  \return      TRUE          Tx BRS is active for the current baudrate
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsFdTxBrs(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_AppendCanIdFdBit()
 *********************************************************************************************************************/
/*! \brief       Appends the FD-bit to the passed CAN-Id and returns the CAN-Id.
 *  \details     Appends the FD-bit also in none FD configurations.
 *  \param[in]   CanId    CanId to which the FD-bit is to be appended
 *  \return      CanId    CanId with set FD-bit
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Can_IdType, CAN_30_CORE_CODE) Can_30_Core_AppendCanIdFdBit(Can_IdType CanId);

/**********************************************************************************************************************
 *  Can_30_Core_AppendCanIdExtBit()
 *********************************************************************************************************************/
/*! \brief       Appends the EXT-bit to the passed CAN-Id and returns the CAN-Id.
 *  \details     In Standard-Id only configurations the unused bit 15 is set as EXT-bit.
 *  \param[in]   CanId    CanId to which the EXT-bit is to be appended
 *  \return      CanId    CanId with set EXT-bit
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Can_IdType, CAN_30_CORE_CODE) Can_30_Core_AppendCanIdExtBit(Can_IdType CanId);

/*********  TxRequestHandler Unit  ***********************************************************************************/


/*********  TxConfirmationHandler Unit  ******************************************************************************/
/* \unit Can_30_Core_TxConfirmationHandler */


/**********************************************************************************************************************
 *  Can_30_Core_Confirmation()
 *********************************************************************************************************************/
/*! \brief       Tx Confirmation.
 *  \details     Signalizes a successful CAN frame tranmission to the Core.
 *  \param[in]   hth              Handle to CanHardwareObject.
 *  \param[in]   mailboxElement   Handle to the mailbox element.
 *  \pre         Controller must be in Start mode.
 *  \pre         The CanHardwareObject (its mailxbox element) has an Tx event.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE)  Can_30_Core_Confirmation(Can_HwHandleType hth, Can_HwHandleType mailboxElement);


/*********  TxBufferHandler Unit  ************************************************************************************/
/* \unit Can_30_Core_TxBufferHandler */


/**********************************************************************************************************************
 *  Can_30_Core_TxBufferHandlerClear()
 *********************************************************************************************************************/
/*! \brief       Clears the CanHardwareObjects (and their mailbox elements) for the requested controller in the CAN Core.
 *  \details     -
 *  \param[in]   Controller    The requested controller
 *  \pre         Controller must NOT be in Start mode.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_TxBufferHandlerClear(uint8 Controller);


/*********  RxIndicationHandler Unit  *************************************************************************************/
/* \unit Can_30_Core_RxIndicationHandler */


/**********************************************************************************************************************
 *  Can_30_Core_Indication()
 *********************************************************************************************************************/
/*! \brief       Rx Indication.
 *  \details     Signalizes a CAN frame reception to the Core.
 *  \param[in]   hrh              Handle to CanHardwareObject.
 *  \param[in]   PduInfo          Pointer to local PduInfo struct, must not be NULL.
 *                                PduInfo->sdu must not be NULL.
 *                                Caller must ensure the consistency of the PduInfo struct.
 *  \pre         Controller must be in Start mode.
 *  \pre         The CanHardwareObject has an Rx event.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_Indication(Can_HwHandleType hrh, Can_PduInfoPtrType PduInfo);


/*********  RxQueueHandler Unit  *************************************************************************************/


/*********  InterruptService Unit  ***********************************************************************************/


/*********  DiagnosticHandler Unit  **********************************************************************************/
/* \unit Can_30_Core_DiagnosticHandler */


# define CAN_30_CORE_STOP_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Even Functions intended to be inlined shall be declared and defined within a MEMMAP Section */
# define CAN_30_CORE_START_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_BswErrorHook()
 *********************************************************************************************************************/
/*! \brief       Call the generated EcuM_BswErrorHook.
 *  \details     Wrapper to let the possibility to the vCan to call the generated EcuM_BswErrorHook
 *  \param[in]   ModuleId    module identification
 *  \param[in]   ErrorId     EcuM Error Id
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_INLINE FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_BswErrorHook(uint16 ModuleId, uint8 ErrorId);

/***********************************************************************************************************************
 * Can_30_Core_DevCheck_IsConditionTrue()
 ***********************************************************************************************************************/
/*! \brief        Generic Condition Check; modified by Development error Detection switch
 *  \details      Any given conditon becomes FALSE, if development error Detection is DISABLED;
 *  \param[in]    condition    The condition, i.e. the result of a boolean expression
 *  \return       FALSE        Condition is FALSE or Development Error Detection is DISABLED
 *  \return       TRUE         Condition is TRUE AND Development Error Detection is ENABLED
 *  \pre          -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_DevCheck_IsConditionTrue(boolean condition);

/***********************************************************************************************************************
 * Can_30_Core_ReportDevelopmentError()
 ***********************************************************************************************************************/
/*! \brief        Report given error to DET, or drop it (if Development Error Reporting is disabled)
 *  \details      Does nothing if error is CAN_30_CORE_E_NO_ERROR
 *  \param[in]    ApiId      ApiId of calling service
 *  \param[in]    ErrorId    Error Code
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
CAN_30_CORE_LOCAL_INLINE FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ReportDevelopmentError(
  Can_30_Core_ServiceId ApiId,
  Can_30_Core_ErrorId ErrorId);

/***********************************************************************************************************************
 * Can_30_Core_SafeBsw_IsConditionTrue()
 ***********************************************************************************************************************/
/*! \brief        Generic Condition Check; modified by safe bsw switch
 *  \details      Any given conditon becomes FALSE, if safe bsw is DISABLED;
 *  \param[in]    condition    The condition, i.e. the result of a boolean expression
 *  \return       FALSE        Condition is FALSE or safe bsw is DISABLED
 *  \return       TRUE         Condition is TRUE AND safe bsw is ENABLED
 *  \pre          -
 *  \context      ANY
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_SafeBsw_IsConditionTrue(boolean condition);

# define CAN_30_CORE_STOP_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CAN_30_CORE_START_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  Can_30_Core_Overrun()
 *********************************************************************************************************************/
/*! \brief       Overrun notification
 *  \details     Should be called to notify an overrun on the given receive handle
 *  \param[in]   Controller    The controller which detected the overrun
 *  \param[in]   Hrh           The Rx handle on which the overrun was detected
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_Overrun(uint8 Controller, Can_HwHandleType Hrh);


/*********  Version Unit  ********************************************************************************************/


/*********  ExclusiveAreaHandler Unit  *******************************************************************************/


/*********  TimeoutHandler Unit  *************************************************************************************/
/* \unit Can_30_Core_TimeoutHandler */


/**********************************************************************************************************************
 *  Can_30_Core_TimerLoop()
 *********************************************************************************************************************/
/*! \brief       Executes the timeout monitoring.
 *  \details     Service function to check (against generated maximum loop value) whether a hardware loop shall be
 *               continued or broken.
 *  \param[in]   Controller    The requested controller on which the hardware observation takes place.
 *  \param[in]   source        Source for the hardware observation.
 *                             The calling component may only use its own hardware loop check source.
 *  \return      CAN_OK        Returned when the loop should be continued (observation continues).
 *  \return      CAN_NOT_OK    Returned when the loop should be discontinued (observation stops).
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Can_ReturnType, CAN_30_CORE_CODE) Can_30_Core_TimerLoop(uint8 Controller, uint8 source);

/**********************************************************************************************************************
 *  Can_30_Core_TimerStart()
 *********************************************************************************************************************/
/*! \brief       Starts the timeout monitoring.
 *  \details     Service function to start an observation timer.
 *  \param[in]   Controller    The requested controller on which the hardware observation takes place.
 *  \param[in]   source        Source for the hardware observation.
 *                             The calling component may only use its own hardware loop check source.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_TimerStart(uint8 Controller, uint8 source);

/**********************************************************************************************************************
 *  Can_30_Core_TimerEnd()
 *********************************************************************************************************************/
/*! \brief       Ends the timeout monitoring.
 *  \details     Service function to end an observation timer.
 *  \param[in]   Controller    The requested controller on which the hardware observation takes place.
 *  \param[in]   source        Source for the hardware observation.
 *                             The calling component may only use its own hardware loop check source.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_TimerEnd(uint8 Controller, uint8 source);


/*********  ModeHandler Unit  ****************************************************************************************/


/*********  ModeRequest Unit  ****************************************************************************************/


/*********  Initialization Unit  *************************************************************************************/
/* \unit Can_30_Core_Initialization */

/**********************************************************************************************************************
 *  Can_30_Core_IsControllerInitialized()
 *********************************************************************************************************************/
/*! \brief       Checks if the controller is initialized
 *  \details     Checks if the controller is initialized using the controller log status
 *  \param[in]   Controller    The requested controller
 *  \return      FALSE         The controller is not initialized
 *  \return      TRUE          The controller is initialized
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsControllerInitialized(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_InitController()
 *********************************************************************************************************************/
/*! \brief       Initializes the controller and its CanHardwareObjects.
 *  \details     It also execute the optional RAM check.
 *  \param[in]   Controller    The controller that should be initialized.
 *  \param[in]   doRamCheck    TRUE/FALSE if the RAM check should be performed.
 *  \return      CAN_NOT_OK    Controller initialization failed.
 *  \return      CAN_OK        Controller initialization was successful.
 *  \pre         CAN Core is initialized.
 *  \pre         Controller must NOT be in Start mode.
 *  \pre         Controller does NOT perform a controller mode change.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Can_ReturnType, CAN_30_CORE_CODE) Can_30_Core_InitController(uint8 Controller, boolean doRamCheck);


/*********  ErrorHandler Unit  ***************************************************************************************/
/* \unit Can_30_Core_ErrorHandler */


/**********************************************************************************************************************
 *  Can_30_Core_ErrorHandling()
 *********************************************************************************************************************/
/*! \brief       Controller error handling
 *  \details     Handles the controller errors (e.g. bus off)
 *  \param[in]   Controller    The requested controller
 *  \pre         Controller must NOT be in Sleep mode.
 *  \pre         The controller has an error (only in case the function is called from interrupt context).
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_ErrorHandling(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_ErrorNotification()
 *********************************************************************************************************************/
/*! \brief       Security Event Reporting of errors
 *  \details     Forwards the error type to CanIf_ErrorNotification()
 *  \param[in]   Controller    CAN controller (only if not using "Optimize for one controller")
 *  \param[in]   CanError      Reported CAN error
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_ErrorNotification(uint8 Controller, Can_ErrorType CanError);


/*********  WakeupHandler Unit  **************************************************************************************/
/* \unit Can_30_Core_WakeupHandler */


/**********************************************************************************************************************
 *  Can_30_Core_WakeupHandling()
 *********************************************************************************************************************/
/*! \brief       Performs the wakeup handling
 *  \details     Handles the Wake up event for the requested controller
 *  \param[in]   Controller    The requested controller
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_WakeupHandling(uint8 Controller);


/*********  Baudrate Unit  *******************************************************************************************/


# define CAN_30_CORE_STOP_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 * IMPLEMENTATION
 *********************************************************************************************************************/
/* Even Functions intended to be inlined shall be declared and defined within a MEMMAP Section */
# define CAN_30_CORE_START_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/*********  DiagnosticHandler Unit  **********************************************************************************/
/* \unit Can_30_Core_DiagnosticHandler */


# ifndef CAN_NOUNIT_CAN_30_CORE_DIAGNOSTICHANDLER     /* COV_CAN_30_CORE_UNUSED_PREPROCESSOR */
/**********************************************************************************************************************
 * Can_30_Core_DevCheck_IsConditionTrue()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_DevCheck_IsConditionTrue
Relation_Context:
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_DevCheck_IsConditionTrue(  /* PRQA S 3219 */ /* MD_Can_30_Core_NotUsedInUnit */
  boolean condition)
{
  return (Can_30_Core_IsDevErrorDetectionOfGeneralFeature(0) && condition);
}

/**********************************************************************************************************************
 * Can_30_Core_ReportDevelopmentError()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ReportDevelopmentError
Relation_Context:
Relation:
DevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
*/
CAN_30_CORE_LOCAL_INLINE FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ReportDevelopmentError( /* PRQA S 3219 */ /* MD_Can_30_Core_NotUsedInUnit */
  Can_30_Core_ServiceId ApiId,
  Can_30_Core_ErrorId ErrorId)
{
  /* #100 Check if DET reporting is active */
  if (Can_30_Core_IsDevErrorReportOfGeneralFeature(0) && (ErrorId != CAN_30_CORE_E_NO_ERROR))
  {
    /* #110 Call DET Error Reporting */
    Can_30_Core_CallDetErrorReporting(CAN_30_CORE_MODULE_ID, CAN_30_CORE_INSTANCE_ID, (uint8)ApiId, (uint8)ErrorId);
  }
  else
  {
    CAN_30_CORE_DUMMY_STATEMENT(ApiId); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  }
}

/**********************************************************************************************************************
 * Can_30_Core_SafeBsw_IsConditionTrue()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_SafeBsw_IsConditionTrue
Relation_Context:
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
*/
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_SafeBsw_IsConditionTrue(  /* PRQA S 3219 */ /* MD_Can_30_Core_NotUsedInUnit */
  boolean condition)
{
  return (Can_30_Core_IsSafeBswChecksOfGeneralFeature(0) && condition);
}

/**********************************************************************************************************************
 *  Can_30_Core_BswErrorHook()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_BswErrorHook
Relation_Context:
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL_INLINE FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_BswErrorHook(uint16 ModuleId, uint8 ErrorId)  /* PRQA S 3219 */ /* MD_Can_30_Core_NotUsedInUnit */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Call generated EcuM_BswErrorHook */
  Can_30_Core_CallEcuM_BswErrorHook(ModuleId, ErrorId);

  CAN_30_CORE_DUMMY_STATEMENT(ModuleId); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  CAN_30_CORE_DUMMY_STATEMENT(ErrorId); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
}

# endif /* CAN_NOUNIT_CAN_30_CORE_DIAGNOSTICHANDLER */


# define CAN_30_CORE_STOP_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* CAN_30_CORE_VCAN_H */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_vCan.h
 *********************************************************************************************************************/
