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
/*!       \file  Can_30_Core_InterruptService.c
 *        \brief  Can_30_Core_InterruptService source file
 *        \details  Contains the implementation of the Interrupt service unit.
 *        \unit Can_30_Core_InterruptService
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
#define CAN_30_CORE_INTERRUPTSERVICE_SOURCE
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
#include "Can_30_Core_InterruptService.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ExclusiveAreaHandler.h"
#include "Can_30_Core_Initialization.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_DisableControllerInterrupts()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_DisableControllerInterrupts
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, IntLock
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
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_DisableControllerInterrupts(uint8 Controller)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if the driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if the controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(Can_30_Core_GetCanInterruptNestingCounterOfControllerData(Controller) >= 255u))
  { /* #30 Check if the counter will overflow */
    errorId = CAN_30_CORE_E_DATALOST;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Secure the interrupt handling */
    Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_1);
    /* #110 Check interrupt nesting counter */
    if (Can_30_Core_GetCanInterruptNestingCounterOfControllerData(Controller) == 0u)
    {
      if ((Can_30_Core_GetInterruptLockOfGeneralFeature(0) == CAN_30_CORE_ILOCK_DRIVER) ||
          (Can_30_Core_GetInterruptLockOfGeneralFeature(0) == CAN_30_CORE_ILOCK_BOTH))
      {
        /* #120 Call disable controller interrupts in extension */
        Can_30_Core_GetDisableControllerInterruptsFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
      }
      if ((Can_30_Core_GetInterruptLockOfGeneralFeature(0) == CAN_30_CORE_ILOCK_APPL) ||
          (Can_30_Core_GetInterruptLockOfGeneralFeature(0) == CAN_30_CORE_ILOCK_BOTH))
      { /* #130 Let application disable the CAN interrupts */
        Can_30_Core_CallCanInterruptDisable(Controller);
      }
    }
    /* #140 Increment interrupt nesting counter */
    Can_30_Core_IncCanInterruptNestingCounterOfControllerData(Controller); /* VCA_CAN_30_CORE_CONTROLLERDATA */
    Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_1);
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CTR_INT_DISABLE, errorId);
}

/**********************************************************************************************************************
 *  Can_30_Core_EnableControllerInterrupts()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_EnableControllerInterrupts
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, IntLock
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
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_EnableControllerInterrupts(uint8 Controller)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if the driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if the controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Secure the interrupt handling */
    Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_1);
    /* #110 Check the interrupt nesting level */
    if (Can_30_Core_GetCanInterruptNestingCounterOfControllerData(Controller) != 0u)
    {
      /* #120 Decrement the nesting counter */
      Can_30_Core_DecCanInterruptNestingCounterOfControllerData(Controller); /* VCA_CAN_30_CORE_CONTROLLERDATA */
      /* #130 Check if the nesting counter is equal to 0 */
      if (Can_30_Core_GetCanInterruptNestingCounterOfControllerData(Controller) == 0u)
      {
        if ((Can_30_Core_GetInterruptLockOfGeneralFeature(0) == CAN_30_CORE_ILOCK_DRIVER) ||
            (Can_30_Core_GetInterruptLockOfGeneralFeature(0) == CAN_30_CORE_ILOCK_BOTH))
        { /* #140 Call enable controller interrupts in extension */
          Can_30_Core_GetRestoreControllerInterruptsFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
        }
        if ((Can_30_Core_GetInterruptLockOfGeneralFeature(0) == CAN_30_CORE_ILOCK_APPL) ||
            (Can_30_Core_GetInterruptLockOfGeneralFeature(0) == CAN_30_CORE_ILOCK_BOTH))
        { /* #150 Let application restore the CAN interrupts */
          Can_30_Core_CallCanInterruptRestore(Controller);
        }
      }
    }
    Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_1);
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CTR_INT_ENABLE, errorId);
}

/**********************************************************************************************************************
 *  Can_30_Core_InterruptServiceInit()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_InterruptServiceInit
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
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_InterruptServiceInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 Controller = 0u;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Loop over all controllers and reset nesting counters */
  for (; Controller < Can_30_Core_GetSizeOfControllerConfig(); Controller++)
  {
    Can_30_Core_SetCanInterruptNestingCounterOfControllerData(Controller, 0u); /* VCA_CAN_30_CORE_CONTROLLERDATA */
  }
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_InterruptService.c
 *********************************************************************************************************************/
