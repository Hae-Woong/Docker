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
/*!       \file  Can_30_Core_ModeRequest.c
 *        \brief  Can_30_Core_ModeRequest source file
 *        \details  Contains the implementation of the Mode request unit.
 *        \unit Can_30_Core_ModeRequest
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
#define CAN_30_CORE_MODEREQUEST_SOURCE
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
#include "Can_30_Core_ModeRequest.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ExclusiveAreaHandler.h"
#include "Can_30_Core_ModeHandler.h"
#include "Can_30_Core_Initialization.h"
#include "Can_30_Core_TimeoutHandler.h"
#include "CanIf.h"
#include "CanIf_Cbk.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_CheckTransition()
 *********************************************************************************************************************/
/*! \brief       Verify the mode transition
 *  \details     Checks if the mode transition is allowed
 *  \param[in]   Controller    The requested controller
 *  \param[in]   Transition    The requested transition
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_CheckTransition(uint8 Controller, Can_StateTransitionType Transition);

/**********************************************************************************************************************
 *  Can_30_Core_SetControllerModeStart()
 *********************************************************************************************************************/
/*! \brief       Change the controller mode to start.
 *  \details     -
 *  \param[in]   Controller    The requested controller
 *  \return      CAN_NOT_OK    Transition request rejected
 *  \return      CAN_OK        Transition request accepted
 *  \pre         Must be called in context of entered CAN_30_CORE_EXCLUSIVE_AREA_6
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetControllerModeStart(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_SetControllerModeStop()
 *********************************************************************************************************************/
/*! \brief       Change the controller mode to stop.
 *  \details     -
 *  \param[in]   Controller    The requested controller
 *  \return      CAN_NOT_OK    Transition request rejected
 *  \return      CAN_OK        Transition request accepted
 *  \pre         Must be called in context of entered CAN_30_CORE_EXCLUSIVE_AREA_6
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetControllerModeStop(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_SetControllerModeWakeup()
 *********************************************************************************************************************/
/*! \brief       Change the controller mode to wakeup.
 *  \details     -
 *  \param[in]   Controller    The requested controller
 *  \return      CAN_NOT_OK    Transition request rejected
 *  \return      CAN_OK        Transition request accepted
 *  \pre         Must be called in context of entered CAN_30_CORE_EXCLUSIVE_AREA_6
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetControllerModeWakeup(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_SetControllerModeSleep()
 *********************************************************************************************************************/
/*! \brief       Change the controller mode to sleep.
 *  \details     -
 *  \param[in]   Controller    The requested controller
 *  \return      CAN_NOT_OK    Transition request rejected
 *  \return      CAN_OK        Transition request accepted
 *  \pre         Must be called in context of entered CAN_30_CORE_EXCLUSIVE_AREA_6
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetControllerModeSleep(uint8 Controller);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Can_30_Core_CheckTransition()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_CheckTransition
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
 */
CAN_30_CORE_LOCAL FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_CheckTransition(uint8 Controller, Can_StateTransitionType Transition)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retval = FALSE;
  /* ----- Implementation ----------------------------------------------- */
  if ((!Can_30_Core_IsControllerInStop(Controller)) &&
      (!Can_30_Core_IsControllerInSleep(Controller)) && /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
      ((Transition == CAN_T_WAKEUP) || (Transition == CAN_T_SLEEP)))
  { /* #100 Check  No STOP Nor SLEEP -> SLEEP-Req or No SLEEP Nor STOP --> WAKEUP-Req is invalid (SPEC-1654, SPEC-1649) */
    retval = TRUE;
  }
  else if ((!Can_30_Core_IsControllerInStop(Controller)) &&
           (!Can_30_Core_IsControllerInStart(Controller)) && /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
           (Transition == CAN_T_STOP))
  { /* #110 Check  No START Nor STOP -> STOP-Req is invalid (SPEC-1652) */
    retval = TRUE;
  }
  else if ((!Can_30_Core_IsControllerInStop(Controller)) && (Transition == CAN_T_START)) /* PRQA S 2004 */ /* MD_Can_30_Core_NoElseAfterIf */
  { /* #120 Check No-STOP -> START-Req is invalid (SPEC-1637) */
    retval = TRUE;
  }
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_SetControllerModeStart()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_SetControllerModeStart
  Relation_Context:
DevErrorDetect, ChannelAmount, RamCheck, Variant
  Relation:
SilentMode
ReInitStart
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
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetControllerModeStart(uint8 Controller)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK;
  uint8 transitionRequest = CAN_30_COREMODENONE;
  /* ----- Implementation ----------------------------------------------- */
  if (Can_30_Core_IsIsBusOffOfControllerData(Controller))
  { /* #100 BUSOFF occurred, finish BUSOFF handling and perform START transition */
    transitionRequest = CAN_30_COREMODERESETBUSOFFEND;
  }
  else if (Can_30_Core_GetSilentMode(Controller) == CAN_30_CORE_SILENT_ACTIVE)
  { /* #110 Silent mode active, set transition request to silent mode */
    transitionRequest = CAN_30_COREMODESILENT;
  }
  else
  { /* #120 Not bus off or silent, set transistion request to start mode */
    transitionRequest = (Can_30_Core_IsReinitStartOfGeneralFeature(0)) ? CAN_30_COREMODESTARTREINIT : CAN_30_COREMODESTART; /* PRQA S 3493, 3494 */ /* MD_Can_30_Core_ConstValue */
  }
  /* #130 Request mode transistion */
  retval = Can_30_Core_ModeTransition(Controller, transitionRequest, TRUE, FALSE);

  if (retval == CAN_OK)
  {
    Can_30_Core_SetIsBusOffOfControllerData(Controller, FALSE); /* VCA_CAN_30_CORE_CONTROLLERDATA */
  }

  /* #140 Notify mode transistion */
  Can_30_Core_NotifyTransition(Controller, CAN_30_CORE_STATUS_START, CANIF_CS_STARTED, retval);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_SetControllerModeStop()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_SetControllerModeStop
  Relation_Context:
DevErrorDetect, ChannelAmount, RamCheck, Variant
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
 */
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetControllerModeStop(uint8 Controller)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK;
  boolean busOffRecovery = FALSE;
  /* ----- Implementation ----------------------------------------------- */
  if (Can_30_Core_GetBusOffRecoveryCounterOfControllerData(Controller) != 0u)
  {  /* #100 Continue bus off recovery */
    Can_30_Core_DecBusOffRecoveryCounterOfControllerData(Controller); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    busOffRecovery = TRUE;
  }
  
  /* #110 Request mode transistion */
  retval = Can_30_Core_ModeTransition(Controller, CAN_30_COREMODESTOPREINITFAST, busOffRecovery, FALSE);

  /* #120 Notify mode transistion */
  Can_30_Core_NotifyTransition(Controller, CAN_30_CORE_STATUS_STOP, CANIF_CS_STOPPED, retval);

  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_SetControllerModeWakeup()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_SetControllerModeWakeup
  Relation_Context:
DevErrorDetect, ChannelAmount, RamCheck, Variant
  Relation:
Wakeup
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
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetControllerModeWakeup(uint8 Controller)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK;
  uint8 transitionRequest = CAN_30_COREMODESTOPREINITFAST;
  boolean busOffRecovery = FALSE;
  boolean doRamCheck = FALSE;
  /* ----- Implementation ----------------------------------------------- */
  if (((Can_30_Core_GetRamCheckOfControllerConfig(Controller) == CAN_30_CORE_RAMCHECK_ACTIVE) ||
       (Can_30_Core_GetRamCheckOfControllerConfig(Controller) == CAN_30_CORE_RAMCHECK_MAILBOX)) &&
      (!Can_30_Core_IsControllerInStop(Controller))) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
  { /* #100 Perform standard RAM check if controller is not in STOP mode */
    transitionRequest = CAN_30_COREMODESTOPREINIT; /* PRQA S 2982 */ /* MD_Can_30_Core_ConstValue */
    doRamCheck = TRUE;
  }

  if ((Can_30_Core_IsWakeupSupportOfControllerConfig(Controller)) &&
      (Can_30_Core_GetWakeupSourceRefOfControllerConfig(Controller) != 0u)) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
  { /* #110 Hardware is only in SLEEP if WAKEUP over bus is active and supported and wakeup on this controller is activated -> otherwise emulated SLEEP */
    transitionRequest = CAN_30_COREMODEWAKEUPSTOPREINIT;
  }

  /* #120 Request mode transition */
  retval = Can_30_Core_ModeTransition(Controller, transitionRequest, busOffRecovery, doRamCheck);

  /* #130 Notify mode transition */
  Can_30_Core_NotifyTransition(Controller, CAN_30_CORE_STATUS_STOP, CANIF_CS_STOPPED, retval);

  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_SetControllerModeSleep()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_SetControllerModeSleep
  Relation_Context:
DevErrorDetect, ChannelAmount, RamCheck, Variant
  Relation:
Wakeup
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
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetControllerModeSleep(uint8 Controller)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ((Can_30_Core_IsWakeupSupportOfControllerConfig(Controller)) &&
      (Can_30_Core_GetWakeupSourceRefOfControllerConfig(Controller) != 0u)) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
  { /* #100 Transition only when WAKEUP by bus is configured, otherwise use emulated SLEEP (SPEC-1633) */
    retval = Can_30_Core_ModeTransition(Controller, CAN_30_COREMODESLEEP, FALSE, FALSE);
  }
  else
  { /* #110 no WAKEUP-source-ref: do emulated SLEEP mode (SPEC-1629, SPEC-1641, SPEC-1645) */
    retval = CAN_OK;
  }
  /* #120 Notify mode transistion */
  Can_30_Core_NotifyTransition(Controller, CAN_30_CORE_STATUS_SLEEP, CANIF_CS_SLEEP, retval);

  return retval;
}
#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_SetControllerMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_SetControllerMode
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, RamCheck, Variant
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
 */
 /* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Can_ReturnType, CAN_30_CORE_CODE) Can_30_Core_SetControllerMode(uint8 Controller, Can_StateTransitionType Transition)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  Can_ReturnType retval = CAN_NOT_OK;

  CanHookBegin_Can_30_Core_SetControllerMode();

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
  { /* #30 Check if controller is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(Can_30_Core_CheckTransition(Controller, Transition)))
  { /* #40 Check if the mode transition is allowed */
    errorId = CAN_30_CORE_E_TRANSITION;
  }
  else
  {
    /* #50 If the Controller is deactivated do nothing */
    if ((Can_30_Core_GetRamCheckOfControllerConfig(Controller) != CAN_30_CORE_RAMCHECK_NONE) &&
        (Can_30_Core_IsControllerDeactivated(Controller))) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
    {
      /* Do nothing */
    }
    else
    {
      /* ----- Implementation ----------------------------------------------- */
      /* EXCLUSIVE_AREA_6 avoid nesting mode changes (e.g. same API, initialization or BUSOFF) */
      Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_6);
      /* #100 Begin mode transition */
      switch (Transition)
      {
      case CAN_T_START:
        /* #110 Handle START transition */
        retval = Can_30_Core_SetControllerModeStart(Controller);
        break;

      case CAN_T_STOP:
        /* #120 Handle STOP transition */
        retval = Can_30_Core_SetControllerModeStop(Controller);
        break;

      case CAN_T_WAKEUP:
        /* #130 Handle WAKEUP transition */
        if ((Can_30_Core_IsWakeupSupportOfControllerConfig(Controller)) &&
            ((Can_30_Core_GetModeTransitionRequestOfControllerData(Controller) & CAN_30_COREMODESLEEP) == CAN_30_COREMODESLEEP)) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
        {
          errorId = CAN_30_CORE_E_TRANSITION;
        }
        else
        {
          retval = Can_30_Core_SetControllerModeWakeup(Controller);
        }
        break;

      case CAN_T_SLEEP: /* (SPEC-1639) */
        /* #140 Handle SLEEP transition */
        retval = Can_30_Core_SetControllerModeSleep(Controller);
        break;

      default:
        errorId = CAN_30_CORE_E_TRANSITION; /* (SPEC-1733, SPEC-1407) */
        break;

      } /* switch */
      Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_6);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_SET_CTR_MODE, errorId);
  CanHookEnd_Can_30_Core_SetControllerMode();
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_ModeTransition()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ModeTransition
  Relation_Context:
DevErrorDetect, ChannelAmount, RamCheck, Wakeup
DevErrorDetect, ChannelAmount, VariantSel
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
CAN_30_CORE_LOCAL_API FUNC(Can_ReturnType, CAN_30_CORE_CODE) Can_30_Core_ModeTransition(
  uint8 Controller,
  uint8 TransitionRequest,
  boolean ContinueBusOffRecovery,
  boolean DoRamCheck)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType transitionState = CAN_NOT_OK;
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Request Extension Mode Transistion */
  transitionState = Can_30_Core_GetModeTransitionFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, TransitionRequest, ContinueBusOffRecovery, DoRamCheck); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
  if (transitionState == CAN_REQUESTED)
  { /* repeat only short time synchronous when there is the possibility of success - CAN_REQUESTED (NOT_OK means it is not possible now or never) */
    Can_30_Core_TimerStart(Controller, CAN_30_CORE_LOOP_MODE_CHANGE);
    /* #110 Synchronous mode transistion in case first transition fails */
    do /* FETA_CAN_30_CORE_2 */
    {
      /* #120 Check if the requested mode is reached */
      transitionState = Can_30_Core_GetModeTransitionFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, TransitionRequest, ContinueBusOffRecovery, DoRamCheck); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
    } while ((transitionState == CAN_REQUESTED) && (Can_30_Core_TimerLoop(Controller, CAN_30_CORE_LOOP_MODE_CHANGE) == CAN_OK)); /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
    Can_30_Core_TimerEnd(Controller, CAN_30_CORE_LOOP_MODE_CHANGE);
  }
  if (transitionState == CAN_REQUESTED)
  {
    /* #130 Store the requested transition for asynchronous repetition of the mode transition */
    Can_30_Core_SetModeTransitionRequestOfControllerData(Controller, TransitionRequest); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    Can_30_Core_SetContinueBusOffRecoveryOfControllerData(Controller, ContinueBusOffRecovery); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    Can_30_Core_SetRamCheckTransitionRequestOfControllerData(Controller, DoRamCheck); /* VCA_CAN_30_CORE_CONTROLLERDATA */
  }
  return transitionState;
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_ModeRequest.c
 *********************************************************************************************************************/
