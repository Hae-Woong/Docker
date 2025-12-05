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
/*!       \file  Can_30_Core_ErrorHandler.c
 *        \brief  Can_30_Core_ErrorHandler source file
 *        \details  Contains the implementation of the Error Handler unit.
 *        \unit Can_30_Core_ErrorHandler
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/
#define CAN_30_CORE_ERRORHANDLER_SOURCE
#define CAN_30_CORE_INTERNAL

/**********************************************************************************************************************
 * LOCAL MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2880, 2991, 2992, 2994, 2995, 2996 EOF */ /* MD_Can_30_Core_ConstValue */
/* PRQA  S 2741, 2742 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core.h"
#include "Can_30_Core_vCan.h"
#include "Can_30_Core_ErrorHandler.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ExclusiveAreaHandler.h"
#include "Can_30_Core_ModeHandler.h"
#include "Can_30_Core_ModeRequest.h"
#include "Can_30_Core_Initialization.h"
#include "Can_30_Core_InterruptService.h"
#include "CanIf.h"
#include "CanIf_Cbk.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_ErrorStatePassiveHandling()
 *********************************************************************************************************************/
/*! \brief       Handles the error state passive.
 *  \details     Reports an occured transition to error state passive to the CanIf, if security event reporting is
 *               enabled.
 *  \param[in]   Controller    The requested controller
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ErrorStatePassiveHandling(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_ErrorStateBusOffHandling()
 *********************************************************************************************************************/
/*! \brief       Handles the error state bus off.
 *  \details     Reports an occured transition to error state bus off to the CanIf.
 *  \param[in]   Controller    The requested controller
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ErrorStateBusOffHandling(uint8 Controller);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Can_30_Core_ErrorStatePassiveHandling()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ErrorStatePassiveHandling
  Relation_Context:
ChannelAmount, BusoffPolling, Variant, DevErrorDetect, Overrun
  Relation:
CanSecurityEventReporting
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
 *
 *
 */
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ErrorStatePassiveHandling(uint8 Controller)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 If security event reporting is enabled */
  if (Can_30_Core_IsSecurityEventReportingOfGeneralFeature(0))
  { /* #110 If transition to error state passive occurred */
    if (Can_30_Core_GetErrorStatePassiveTransitionOccurredFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller) == TRUE)  /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    {
      /* #120 Report transition with the rx/tx error counters to the CanIf */
      Can_30_Core_CallCanIf_ControllerErrorStatePassive(Controller, /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
                                                (Can_30_Core_GetGetControllerRxErrorCounterFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller)),
                                                (Can_30_Core_GetGetControllerTxErrorCounterFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller)));
    }
    /* #130 Execute error notification handling of the extension */
    Can_30_Core_GetErrorNotificationHandlingFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
  }
}

/**********************************************************************************************************************
 *  Can_30_Core_ErrorStateBusOffHandling()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ErrorStateBusOffHandling
  Relation_Context:
ChannelAmount, BusoffPolling, Variant, DevErrorDetect
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
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ErrorStateBusOffHandling(uint8 Controller)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 If new transition to error state bus off occurred */
  if ((Can_30_Core_GetBusOffOccurredFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller)) && /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
      (!Can_30_Core_IsIsBusOffOfControllerData(Controller))) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
  {
    /* #120 Secure the mode change */
    Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_6);
    /* #130 Limit the recovery attempts to 3 retries to avoid too many repeated calls from CanSm */
    Can_30_Core_SetBusOffRecoveryCounterOfControllerData(Controller, 3u); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    Can_30_Core_SetIsBusOffOfControllerData(Controller, TRUE); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    /* #140 Call mode transition to ResetBusOffStart */
    (void)Can_30_Core_ModeTransition(Controller, CAN_30_COREMODERESETBUSOFFSTART, TRUE, FALSE);
    /* #150 Controller is in STOP mode after ResetBusOffStart (SPEC-1578, SPEC-1664, SPEC-1663) */
    /* #160 Set controller mode status mode to STOP */
    Can_30_Core_SetLogStatusOfControllerData(Controller, (uint8)((Can_30_Core_GetLogStatusOfControllerData(Controller) & CAN_30_CORE_STATUS_MASK_NOMODE) | CAN_30_CORE_STATUS_STOP)); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_6);
    /* #170 Notify CanIf of the bus off occurrence (SPEC-1726, SPEC-1578) */
    CanIf_30_Core_ControllerBusOff((uint8)Can_30_Core_GetCanIfChannelId(Controller));
  }
}
#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_MainFunction_BusOff_Internal()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_MainFunction_BusOff_Internal
  Relation_Context:
  Relation:
ChannelAmount, Multipartition, VariantSel, DevErrorDetect, BusoffPolling
ChannelAmount, Multipartition, VariantSel, DevErrorDetect, IntLock
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
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_MainFunction_BusOff_Internal(Can_30_Core_OsApplicationType ApplicationId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  if (Can_30_Core_IsDriverInitialized())
  {
    uint8 controller = 0u;
    /* #20 Loop over all controllers */
    for (; controller < Can_30_Core_GetSizeOfControllerConfig(); controller++)
    {
      if (Can_30_Core_GetOsApplicationIdOfControllerConfig(controller) != ApplicationId)
      { /* #25 Skip if controller is not in partition */
        continue;
      }
      if (Can_30_Core_IsPostBuildSelectableOfGeneralFeature(0) &&
          (Can_30_Core_GetCanIfChannelId(controller) == CAN_30_CORE_NO_CANIFCHANNELID))
      { /* #27 Skip inactive controllers in variant */
        continue;
      }
      if (!Can_30_Core_IsBusoffProcessingPollingOfControllerConfig(controller))
      { /* #30 Skip if Error Polling is disabled for the controller */
        continue;
      }
      /* #40 Check if controller is initialized */
      if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(controller)))
      {
        errorId = CAN_30_CORE_E_UNINIT;
      }
      else
      {
        /* #50 Do not access hardware sleep mode */
        if (!Can_30_Core_IsControllerInSleep(controller))
        {
          /* #60 Disable CAN controller interrupt */
          Can_30_Core_DisableControllerInterrupts(controller);
          /* #70 Call Controller Error Handling */
          Can_30_Core_ErrorHandling(controller);
          /* #80 Enable CAN controller interrupt */
          Can_30_Core_EnableControllerInterrupts(controller);
        }
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_MAINFCT_BO, errorId);
}

/**********************************************************************************************************************
 *  Can_30_Core_GetStatus()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetStatus
  Relation_Context:
  Relation:
ChannelAmount, Variant, DevErrorDetect
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
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(uint8, CAN_30_CORE_CODE) Can_30_Core_GetStatus(uint8 Controller)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 canReturnCode = CAN_30_CORE_STATUS_INCONSISTENT;
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if Driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(Controller)))
  { /* #40 Check if controller is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    Can_30_Core_GetGetStatusBeginFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    /* #100 Set logical state as the return value */
    canReturnCode = (uint8)Can_30_Core_GetLogStatusOfControllerData(Controller);
    if (Can_30_Core_GetIsHwBusOffFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller)) /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    { /* #110 Check if hardware is in bus off */
      canReturnCode |= CAN_30_CORE_STATUS_BUSOFF;
    }
    if (Can_30_Core_GetIsHwPassiveFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller)) /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    { /* #120 Check if hardware is error passive */
      canReturnCode |= CAN_30_CORE_STATUS_PASSIVE;
    }
    if (Can_30_Core_GetIsHwWarningFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller)) /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    { /* #130 Check if hardware is in error warning */
      canReturnCode |= CAN_30_CORE_STATUS_WARNING;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_GET_STATUS, errorId);
  return (uint8)canReturnCode;
}

/**********************************************************************************************************************
 *  Can_30_Core_GetControllerErrorState()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetControllerErrorState
  Relation_Context:
  Relation:
ChannelAmount, DevErrorDetect
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
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_GetControllerErrorState(
  uint8 Controller, 
  Can_ErrorStatePtrType ErrorStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if Driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if Controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(Controller)))
  { /* #30 Check if controller is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(ErrorStatePtr == NULL_PTR))
  { /* #40 Check that the given pointer is not a null pointer */
    errorId = CAN_30_CORE_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Call extension get status begin */
    Can_30_Core_GetGetStatusBeginFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    if (Can_30_Core_GetIsHwBusOffFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller)) /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    { /* #110 Check if hardware is in bus off */
      *ErrorStatePtr = CAN_ERRORSTATE_BUSOFF; /* VCA_CAN_30_CORE_ERRORSTATEPTR */
    }
    else if ((Can_30_Core_GetIsHwPassiveFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller))) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */ /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    { /* #120 Check if hardware is error passive */
      *ErrorStatePtr = CAN_ERRORSTATE_PASSIVE; /* VCA_CAN_30_CORE_ERRORSTATEPTR */
    }
    else
    { /* #130 Default return is active */
      *ErrorStatePtr = CAN_ERRORSTATE_ACTIVE; /* VCA_CAN_30_CORE_ERRORSTATEPTR */
    }
    retval = E_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_GET_CTR_ERR_STATE, errorId);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_ErrorHandling()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ErrorHandling
  Relation_Context:
ChannelAmount, BusoffPolling, Variant, DevErrorDetect, Overrun
  Relation:
Overrun
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
 *
 *
 *
 *
 *
 *
 */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_ErrorHandling(uint8 Controller)
{ /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #5 Check if driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #10 Check if controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Execute begin error handling in extension */
    Can_30_Core_GetErrorHandlingBeginFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */

    /* #110 Execute error state passive handling */
    Can_30_Core_ErrorStatePassiveHandling(Controller);

    /* #120 Execute error state bus off handling */
    Can_30_Core_ErrorStateBusOffHandling(Controller);

    /* #130 If overrun notifcation is enabled */
    if (Can_30_Core_GetOverrunNotificationOfGeneralFeature(0) != CAN_30_CORE_OVERRUN_NONE)
    {
      /* #140 Excecute overrun detection in extension */
      Can_30_Core_GetOverrunOccurredFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    }

    /* #150 Execute post error handling in extension */
    Can_30_Core_GetErrorHandlingEndFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CTR_ERR_HANDLING, errorId);
}

/**********************************************************************************************************************
 *  Can_30_Core_ErrorNotification()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ErrorNotification
  Relation_Context:
  Relation:
ChannelAmount, DevErrorDetect
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
 *
 */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_ErrorNotification(uint8 Controller, Can_ErrorType CanError) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #5 Check if driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #10 Check if the controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Notify the error to CanIf */
    Can_30_Core_CallCanIf_ErrorNotification(Controller, CanError);
  }
  CAN_30_CORE_DUMMY_STATEMENT(CanError); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CTR_ERR_NOTIFICATION, errorId);
}

/**********************************************************************************************************************
 *  Can_30_Core_GetControllerRxErrorCounter()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetControllerRxErrorCounter
  Relation_Context:
  Relation:
ChannelAmount, DevErrorDetect
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
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_GetControllerRxErrorCounter(
  uint8 Controller,
  Can_30_Core_ErrorCounterPtrType RxErrorCounterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if the controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(Controller)))
  { /* #30 Check if the controller is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(RxErrorCounterPtr == NULL_PTR))
  { /* #40 Check if the pointer parameter is a null pointer */
    errorId = CAN_30_CORE_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Get the controller RX error counter */
    uint16 localErrorCounter = (Can_30_Core_GetGetControllerRxErrorCounterFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller)); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    /* #110 Reduce the counter to 255 in case the hardware supports a larger counter */
    if (localErrorCounter > 255u)
    {
      localErrorCounter = 255u;
    }
    *RxErrorCounterPtr = (uint8)localErrorCounter; /* VCA_CAN_30_CORE_RXERRORCOUNTERPTR */
    retval = E_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_GET_CTR_RX_ERR_CNT, errorId);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_GetControllerTxErrorCounter()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetControllerTxErrorCounter
  Relation_Context:
  Relation:
ChannelAmount, DevErrorDetect
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
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_GetControllerTxErrorCounter(
  uint8 Controller,
  Can_30_Core_ErrorCounterPtrType TxErrorCounterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if the controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(Controller)))
  { /* #30 Check if the controller is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(TxErrorCounterPtr == NULL_PTR))
  { /* #40 Check if the pointer parameter is a null pointer */
    errorId = CAN_30_CORE_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Get the controller TX error counter */
    uint16 localErrorCounter = (Can_30_Core_GetGetControllerTxErrorCounterFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller)); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    /* #110 Reduce the counter to 255 in case the hardware supports a larger counter */
    if (localErrorCounter > 255u)
    {
      localErrorCounter = 255u;
    }
    *TxErrorCounterPtr = (uint8)localErrorCounter; /* VCA_CAN_30_CORE_TXERRORCOUNTERPTR */
    retval = E_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_GET_CTR_TX_ERR_CNT, errorId);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_ErrorHandlerInit()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ErrorHandlerInit
  Relation_Context:
  Relation:
ChannelAmount
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
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_ErrorHandlerInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 Controller = 0u;
  /* ----- Implementation ----------------------------------------------- */
  for (; Controller < Can_30_Core_GetSizeOfControllerConfig(); Controller++) /* FETA_CAN_30_CORE_1 */
  { /* #100 Set the bus off recovery counter to 0 */
    Can_30_Core_SetBusOffRecoveryCounterOfControllerData(Controller, 0u); /* VCA_CAN_30_CORE_CONTROLLERDATA */
  }
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_ErrorHandler.c
 *********************************************************************************************************************/
