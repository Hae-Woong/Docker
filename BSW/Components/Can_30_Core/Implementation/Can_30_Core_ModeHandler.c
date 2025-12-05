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
/*!       \file  Can_30_Core_ModeHandler.c
 *        \brief  Can_30_Core_ModeHandler source file
 *        \details  Contains the implementation of the Mode handler unit.
 *        \unit Can_30_Core_ModeHandler
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
#define CAN_30_CORE_MODEHANDLER_SOURCE
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
#include "Can_30_Core_ModeHandler.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ExclusiveAreaHandler.h"
#include "Can_30_Core_Initialization.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_GetControllerMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetControllerMode
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_GetControllerMode(
  uint8 Controller,
  Can_30_Core_ControllerStatePtrType ControllerModePtr)
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
  { /* #20 Check if controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(ControllerModePtr == NULL_PTR))
  { /* #30 Check if the given mode pointer is a null pointer */
    errorId = CAN_30_CORE_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Get logical controller mode from state machine */
    if (!Can_30_Core_IsControllerInitialized_Internal(Controller))
    {
      *ControllerModePtr = CAN_CS_UNINIT; /* VCA_CAN_30_CORE_CONTROLLERMODEPTR */
    }
    else if (Can_30_Core_IsControllerInStart(Controller))
    {
      *ControllerModePtr = CAN_CS_STARTED; /* VCA_CAN_30_CORE_CONTROLLERMODEPTR */
    }
    else if (Can_30_Core_IsControllerInSleep(Controller))
    {
      *ControllerModePtr = CAN_CS_SLEEP; /* VCA_CAN_30_CORE_CONTROLLERMODEPTR */
    }
    else /* Default is STOPPED */
    {
      *ControllerModePtr = CAN_CS_STOPPED; /* VCA_CAN_30_CORE_CONTROLLERMODEPTR */
    }
    retval = E_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_GET_CTR_MODE, errorId);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_GetSilentMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetSilentMode
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, SilentMode
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
FUNC(Can_30_Core_SilentModeType, CAN_30_CORE_CODE) Can_30_Core_GetSilentMode(uint8 Controller)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  Can_30_Core_SilentModeType retval = CAN_30_CORE_SILENT_INACTIVE;
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
    if (Can_30_Core_IsSilentModeOfGeneralFeature(0))
    {
      /* #100 Get the silent mode state */
      retval = Can_30_Core_GetSilentModeStateOfControllerData(Controller);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_GET_MODE_SILENT, errorId);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_SetSilentMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_SetSilentMode
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, SilentMode
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
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_SetSilentMode(uint8 Controller, Can_30_Core_SilentModeType SilentMode)
{ /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    if (Can_30_Core_IsSilentModeOfGeneralFeature(0))
    { /* #100 Set silent mode for the given controller */
      Can_30_Core_SetSilentModeStateOfControllerData(Controller, SilentMode); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_SET_MODE_SILENT, errorId);
}

/**********************************************************************************************************************
 *  Can_30_Core_GetMirrorMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetMirrorMode
  Relation_Context:
MirrorMode, GenericPreCopy, RxQueue
MirrorMode, GenericConfirmation, HwCancelTx
  Relation:
DevErrorDetect, ChannelAmount
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
FUNC(CddMirror_MirrorModeType, CAN_30_CORE_CODE) Can_30_Core_GetMirrorMode(uint8 Controller) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  CddMirror_MirrorModeType retval = CDDMIRROR_INACTIVE;
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
    /* #100 Get the mirror mode state */
    retval = Can_30_Core_GetMirrorModeStateOfControllerData(Controller);
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_GET_MODE_MIRROR, errorId);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_SetMirrorMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetMirrorMode
  Relation_Context:
MirrorMode
  Relation:
DevErrorDetect, ChannelAmount
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
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_SetMirrorMode(uint8 Controller, CddMirror_MirrorModeType State)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else if ((State != CDDMIRROR_ACTIVE) && (State != CDDMIRROR_INACTIVE))
  { /* 30 Check if the requested mirror mode is valid */
    errorId = CAN_30_CORE_E_PARAM_HANDLE;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    if (Can_30_Core_IsMirrorModeOfGeneralFeature(0))
    { /* #40 set mirror mode for given controller */
      Can_30_Core_SetMirrorModeStateOfControllerData(Controller, State); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_SET_MODE_MIRROR, errorId);
}

/**********************************************************************************************************************
 *  Can_30_Core_SetWakeupEvent()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_SetWakeupEvent
  Relation_Context:
Wakeup
  Relation:
DevErrorDetect, ChannelAmount
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
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_SetWakeupEvent(uint8 Controller, boolean State)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Development Error Checks ------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  if (Can_30_Core_IsWakeupSupportOfControllerConfig(Controller))
  {
    /* #100 Set the wakeup state */
    Can_30_Core_SetIsWakeupOfControllerData(Controller, State); /* VCA_CAN_30_CORE_CONTROLLERDATA */
  }
}

/**********************************************************************************************************************
 *  Can_30_Core_CheckWakeup()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_CheckWakeup
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, Wakeup
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
 */
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_CheckWakeup(uint8 Controller) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{ /* (SPEC-1740) */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(Controller)))
  { /* #30 Check if the controller is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    if (Can_30_Core_IsWakeupSupportOfControllerConfig(Controller))
    {
      /* #100 Check if a WAKEUP occurred */
      if (Can_30_Core_IsIsWakeupOfControllerData(Controller))
      {
        /* #110 Set the wakeup state */
        Can_30_Core_SetWakeupEvent(Controller, FALSE);
        /* #120 Call EcuM_SetWakeupEvent() to prevent missing WAKEUP notification (multiple call is OK) */
        EcuM_SetWakeupEvent(Can_30_Core_GetWakeupSourceRefOfControllerConfig(Controller));
        retval = E_OK;
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CHECK_WU, errorId);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_MainFunction_Mode_Internal()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_MainFunction_Mode_Internal
  Relation_Context:
  Relation:
ChannelAmount, Multipartition, VariantSel, DevErrorDetect
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
 */
/* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_MainFunction_Mode_Internal(Can_30_Core_OsApplicationType ApplicationId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the driver is initialized */
  if (Can_30_Core_IsDriverInitialized())
  {
    uint8 Controller = 0u;

    for (; Controller < Can_30_Core_GetSizeOfControllerConfig(); Controller++)
    {
      if (Can_30_Core_GetOsApplicationIdOfControllerConfig(Controller) != ApplicationId)
      { /* #100 Skip if controller is not in partition */
        continue;
      }
      if (Can_30_Core_IsPostBuildSelectableOfGeneralFeature(0) &&
          (Can_30_Core_GetCanIfChannelId(Controller) == CAN_30_CORE_NO_CANIFCHANNELID))
      { /* #110 Check if the controller is used */
        continue;
      }
      if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(Controller)))
      { /* #120 Check if controller is initialized */
        errorId = CAN_30_CORE_E_UNINIT;
      }
      else
      {
        uint8 transitionRequest = Can_30_Core_GetModeTransitionRequestOfControllerData(Controller);
        if (transitionRequest != CAN_30_COREMODENONE)
        {
          /* #125 Secure the mainfunction mode handling and check again the transition request to ensure data consistency */
          Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_6);
          {
            boolean continueBusOffRecovery = Can_30_Core_IsContinueBusOffRecoveryOfControllerData(Controller);
            boolean doRamCheck = Can_30_Core_IsRamCheckTransitionRequestOfControllerData(Controller);
            Can_ReturnType transitionState = CAN_NOT_OK;
            transitionRequest = Can_30_Core_GetModeTransitionRequestOfControllerData(Controller);
            
            /* #130 Handle the mode request */
            switch (transitionRequest)
            {
            case CAN_30_COREMODESLEEP:
              /* #140 Call extension mode transition to sleep */
              transitionState = Can_30_Core_GetModeTransitionFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, transitionRequest, continueBusOffRecovery, doRamCheck); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
              Can_30_Core_NotifyTransition(Controller, CAN_30_CORE_STATUS_SLEEP, CANIF_CS_SLEEP, transitionState);
              break;
            case CAN_30_COREMODESTOPREINITFAST:
            case CAN_30_COREMODESTOPREINIT:
            case CAN_30_COREMODEWAKEUPSTOPREINIT:
              /* #150 Call extension mode transition to stop */
              transitionState = Can_30_Core_GetModeTransitionFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, transitionRequest, continueBusOffRecovery, doRamCheck); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
              Can_30_Core_NotifyTransition(Controller, CAN_30_CORE_STATUS_STOP, CANIF_CS_STOPPED, transitionState);
              break;
            case CAN_30_COREMODESTARTREINIT:
            case CAN_30_COREMODESTART:
            case CAN_30_COREMODERESETBUSOFFEND:
            case CAN_30_COREMODESILENT:
              /* #160 Call extension mode transition to start */
              transitionState = Can_30_Core_GetModeTransitionFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, transitionRequest, continueBusOffRecovery, doRamCheck); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
              Can_30_Core_NotifyTransition(Controller, CAN_30_CORE_STATUS_START, CANIF_CS_STARTED, transitionState);
              if (transitionState == CAN_OK)
              {
                Can_30_Core_SetIsBusOffOfControllerData(Controller, FALSE); /* VCA_CAN_30_CORE_CONTROLLERDATA */
              }
              break;
            case CAN_30_COREMODERESETBUSOFFSTART: /* not expected, no repetition request */
            default: /* PRQA S 2016 */            /* MD_MSR_EmptyClause */
              break;
            }
          }
          Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_6);
        }
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_MAINFCT_MODE, errorId);
}

/**********************************************************************************************************************
 *  Can_30_Core_NotifyTransition()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_NotifyTransition
  Relation_Context:
Wakeup
SilentMode
  Relation:
DevErrorDetect, ChannelAmount
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
 */
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_NotifyTransition(
  uint8 Controller,
  uint8 CanState,
  CanIf_ControllerModeType CanIfState,
  Can_ReturnType TransitionState)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Check if mode was changed successfully */
  if (TransitionState == CAN_OK)
  {
    /* #110 Change internal state */
    Can_30_Core_SetLogStatusOfControllerData(Controller, (uint8)((Can_30_Core_GetLogStatusOfControllerData(Controller) & CAN_30_CORE_STATUS_MASK_NOMODE) | CanState)); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    /* #120 Remove transistion request */
    Can_30_Core_SetModeTransitionRequestOfControllerData(Controller, CAN_30_COREMODENONE); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    /* #130 Call CanIf mode Indication (SPEC-1726, SPEC-1644, SPEC-60446) */
    CanIf_30_Core_ControllerModeIndication(Can_30_Core_GetCanIfChannelId(Controller), CanIfState);
  }
}

/**********************************************************************************************************************
 *  Can_30_Core_IsControllerInStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL_API FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsControllerInStart(uint8 Controller)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Return the state of the start mode */
  return ((Can_30_Core_GetLogStatusOfControllerData(Controller) & CAN_30_CORE_STATUS_START) == CAN_30_CORE_STATUS_START);
}

/**********************************************************************************************************************
 *  Can_30_Core_IsControllerInStop()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL_API FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsControllerInStop(uint8 Controller)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Return the state of the stop mode */
  return ((Can_30_Core_GetLogStatusOfControllerData(Controller) & CAN_30_CORE_STATUS_STOP) == CAN_30_CORE_STATUS_STOP);
}

/**********************************************************************************************************************
 *  Can_30_Core_IsControllerInSleep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL_API FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsControllerInSleep(uint8 Controller)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Return the state of the sleep mode */
  return ((Can_30_Core_GetLogStatusOfControllerData(Controller) & CAN_30_CORE_STATUS_SLEEP) == CAN_30_CORE_STATUS_SLEEP);
}

/**********************************************************************************************************************
 *  Can_30_Core_IsControllerDeactivated()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL_API FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsControllerDeactivated(uint8 Controller)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Return the state of the controller activation */
  return ((Can_30_Core_GetLogStatusOfControllerData(Controller) & CAN_30_CORE_DEACTIVATE_CONTROLLER) == CAN_30_CORE_DEACTIVATE_CONTROLLER);
}

/**********************************************************************************************************************
 *  Can_30_Core_ModeHandlerInit()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ModeHandlerInit
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
 *
 *
 *
 *
 *
 *
 *
 *
 */
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_ModeHandlerInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 Controller = 0u;
  /* ----- Development Error Checks ------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Loop over all Controller */
  for (; Controller < Can_30_Core_GetSizeOfControllerConfig(); Controller++)
  {
    if(!Can_30_Core_IsControllerValid(Controller))
    { /* #105 Skip dummy controllers for better performance */
      continue; 
    }
    else 
    {
      /* #110 Initialize the Controller mode request */
      Can_30_Core_SetModeTransitionRequestOfControllerData(Controller, CAN_30_COREMODENONE); /* VCA_CAN_30_CORE_CONTROLLERDATA */
      /* #120 Initialize the Controller busOff recovery request */
      Can_30_Core_SetContinueBusOffRecoveryOfControllerData(Controller, FALSE); /* VCA_CAN_30_CORE_CONTROLLERDATA */
      /* #130 Initialize the Controller ram check execution request */
      Can_30_Core_SetRamCheckTransitionRequestOfControllerData(Controller, FALSE); /* VCA_CAN_30_CORE_CONTROLLERDATA */
      /* #140 Initialize busOff status -> no busOff at startup */
      Can_30_Core_SetIsBusOffOfControllerData(Controller, FALSE); /* VCA_CAN_30_CORE_CONTROLLERDATA */
      /* #150 Initialize the Mirror Mode state */
      Can_30_Core_SetMirrorMode(Controller, CDDMIRROR_INACTIVE);
      /* #160 Initialize the Silent Mode state */
      Can_30_Core_SetSilentMode(Controller, CAN_30_CORE_SILENT_INACTIVE);
      /* #170 Initialize the wakeup event */
      Can_30_Core_SetWakeupEvent(Controller, FALSE);
    }
  }
}

/**********************************************************************************************************************
 *  Can_30_Core_AppendLogStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_AppendLogStatus(uint8 Controller, uint8 Status)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Append the status to the controller log status */
  Can_30_Core_SetLogStatusOfControllerData(Controller, Can_30_Core_GetLogStatusOfControllerData(Controller) | Status); /* VCA_CAN_30_CORE_CONTROLLERDATA */
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_ModeHandler.c
 *********************************************************************************************************************/
