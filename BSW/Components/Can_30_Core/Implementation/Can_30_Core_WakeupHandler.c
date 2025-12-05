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
/*!       \file  Can_30_Core_WakeupHandler.c
 *        \brief  Can_30_Core_WakeupHandler source file
 *        \details  Contains the implementation of the Wakeup Handler unit.
 *        \unit Can_30_Core_WakeupHandler
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
#define CAN_30_CORE_WAKEUPHANDLER_SOURCE
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
#include "Can_30_Core_WakeupHandler.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ExclusiveAreaHandler.h"
#include "Can_30_Core_ModeHandler.h"
#include "Can_30_Core_Initialization.h"
#include "Can_30_Core_ModeRequest.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_MainFunction_Wakeup_Internal()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_MainFunction_Wakeup_Internal
  Relation_Context:
  Relation:
ChannelAmount, Multipartition, VariantSel, DevErrorDetect, Wakeup, WakeupPolling
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
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_MainFunction_Wakeup_Internal(Can_30_Core_OsApplicationType ApplicationId)
{ /* (SPEC-1770) */
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Check if the driver is initialized */
  if (Can_30_Core_IsDriverInitialized())
  {
    /* #110 Loop over all controllers */
    uint8 controller = 0u;
    for (; controller < Can_30_Core_GetSizeOfControllerConfig(); controller++)
    {
      if (Can_30_Core_GetOsApplicationIdOfControllerConfig(controller) != ApplicationId)
      { /* #115 Skip if controller is not in partition */
        continue;
      }
      if (!Can_30_Core_IsWakeupSupportOfControllerConfig(controller))
      { /* #120 Skip if Wakeup is disabled for the controller */
        continue;
      }
      if (Can_30_Core_IsPostBuildSelectableOfGeneralFeature(0) &&
          (Can_30_Core_GetCanIfChannelId(controller) == CAN_30_CORE_NO_CANIFCHANNELID))
      { /* #130 Skip inactive controllers */
        continue;
      }
      if (!Can_30_Core_IsWakeupProcessingPollingOfControllerConfig(controller))
      { /* #135 Skip if Wakuep polling is disabled for the controller */
        continue;
      }
      /* #140 Check if the controller is initialized */
      if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(controller)))
      {
        errorId = CAN_30_CORE_E_UNINIT;
      }
      else
      {
        /* #150 Check if a wakeup occurred */
        if (Can_30_Core_GetWakeUpOccurredFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(controller))(controller)) /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
        {
          /* #160 Secure the state transition - only wakeup Polling */
          Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_5);
          /* #165 check the current mode, Wakeup is allowed only from Sleep/Stop mode (SPEC-1654, SPEC-1649) */
          if (Can_30_Core_IsControllerInStop(controller) || Can_30_Core_IsControllerInSleep(controller)) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
          {
            /* #170 Call extension wakeup handling */
            Can_30_Core_GetWakeUpHandlingFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(controller))(controller); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
            /* #180 Call the Wakeup Handling */
            Can_30_Core_WakeupHandling(controller);
          }
          Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_5);
        }
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_MAINFCT_WU, errorId);
}

/**********************************************************************************************************************
 *  Can_30_Core_WakeupHandling()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_WakeupHandling
  Relation_Context:
ChannelAmount, Wakeup, WakeupPolling
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_WakeupHandling(uint8 Controller)
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
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(Controller)))
  { /* #20 Check if the controller is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Set the controller mode to WAKEUP */
    (void)Can_30_Core_SetControllerMode(Controller, CAN_T_WAKEUP);
    /* #110 Set the wakeup event */
    Can_30_Core_SetWakeupEvent(Controller, TRUE);
    /* #120 call EcuM_CheckWakeup() (SPEC-1725, SPEC-1659, SPEC-1697) */
    EcuM_CheckWakeup(Can_30_Core_GetWakeupSourceRefOfControllerConfig(Controller));
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CTR_WAKEUP_HANDLING, errorId);
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_WakeupHandler.c
 *********************************************************************************************************************/
