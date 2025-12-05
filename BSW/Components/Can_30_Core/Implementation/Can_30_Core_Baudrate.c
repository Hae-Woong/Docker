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
/*!       \file  Can_30_Core_Baudrate.c
 *        \brief  Can_30_Core_Baudrate source file
 *        \details  Contains the implementation of the Baudrate unit.
 *        \unit Can_30_Core_Baudrate
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
#define CAN_30_CORE_BAUDRATE_SOURCE
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
#include "Can_30_Core_Baudrate.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ExclusiveAreaHandler.h"
#include "Can_30_Core_ModeHandler.h"
#include "Can_30_Core_ModeRequest.h"
#include "Can_30_Core_Initialization.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_ChangeBaudrate()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ChangeBaudrate
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, ChangeBaudrate
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
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_ChangeBaudrate(uint8 Controller, uint16 Baudrate)
{ /* (SPEC-20314) */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if parameter controller is valid and state is INIT and STOP (SPEC-20338) */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if Controller parameter is valid (SPEC-20331) */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  /* #30 Check if controller is not in STOP mode */
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInStop(Controller)))
  { /* (SPEC-1655, SPEC-20312) */
    errorId = CAN_30_CORE_E_TRANSITION;
  }
  else
  { /* ----- Implementation ----------------------------------------------- */
    uint8_least baudrateIndex = (uint8)Can_30_Core_GetInitObjectBaudrateStartIdxOfControllerConfig(Controller);
    for (; baudrateIndex < Can_30_Core_GetInitObjectBaudrateEndIdxOfControllerConfig(Controller); baudrateIndex++) /* FETA_CAN_30_CORE_1 */
    { /* #100 search for baud rate configuration */
      if (Can_30_Core_GetInitObjectBaudrate(baudrateIndex) == Baudrate)
      { 
        Can_30_Core_InitHandle currentBaudrateIndex;
        Can_ReturnType transitionState;
        /* #110 Secure the mode change */
        Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_6);
        /* #115 store temporary to redo in case transition fail */
        currentBaudrateIndex = Can_30_Core_GetLastInitObjectOfControllerData(Controller);
        /* #120 Set baud rate and reinitialize controller to activate baud rate (SPEC-1669) */
        Can_30_Core_SetLastInitObjectOfControllerData(Controller, (uint8)baudrateIndex); /* VCA_CAN_30_CORE_CONTROLLERDATA */
        /* #130 Perform mode transistion (Reinit) */
        transitionState = Can_30_Core_ModeTransition(Controller, CAN_30_COREMODESTOPREINIT, FALSE, FALSE);
        /* #140 check transition STOP (Reinit) is successful */
        if (transitionState == CAN_NOT_OK)
        { /* #150 transition fail: revert last init object (no new baudrate active) */
          Can_30_Core_SetLastInitObjectOfControllerData(Controller, (uint8)currentBaudrateIndex); /* VCA_CAN_30_CORE_CONTROLLERDATA */
        }
        else
        { /* #160 transition pass: store new logical mode in case CAN_OK or CAN_REQUESTED */
          retval = E_OK;
          Can_30_Core_AppendLogStatus(Controller, CAN_30_CORE_STATUS_STOP); /* STOP mode reached */
        }
        Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_6);
        break;
      }
    }
    /* #170 Check if baudrate configuration is found */
    if (retval != E_OK)
    {
      errorId = CAN_30_CORE_E_PARAM_BAUDRATE;
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CHANGE_BR, errorId);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_CheckBaudrate()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_CheckBaudrate
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, ChangeBaudrate
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
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_CheckBaudrate(uint8 Controller, uint16 Baudrate)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR; /* (SPEC-20317) */
  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if parameter controller is valid and state is INIT (SPEC-20328, SPEC-20318) */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  {
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if the controller parameter is valid (SPEC-20335) */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8_least baudrateIndex = (uint8)Can_30_Core_GetInitObjectBaudrateStartIdxOfControllerConfig(Controller);
    for (; baudrateIndex < Can_30_Core_GetInitObjectBaudrateEndIdxOfControllerConfig(Controller); baudrateIndex++) /* FETA_CAN_30_CORE_1 */
    { /* #100 Search for baudrate configuration */
      if (Can_30_Core_GetInitObjectBaudrate(baudrateIndex) == Baudrate)
      { /* #110 Requested baudrate is supported - return OK */
        retval = E_OK;
        break;
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CHECK_BR, errorId);
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_SetBaudrate()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_SetBaudrate
  Relation_Context:
  Relation:
DevErrorDetect, ChannelAmount, ChangeBaudrate
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
/* PRQA S 6050, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CAN_30_CORE_CODE) Can_30_Core_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if parameter controller is valid and state is INIT and STOP (SPEC-50595) */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  {
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* (SPEC-50587) */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInStop(Controller)))
  { /* (SPEC-50584) */
    errorId = CAN_30_CORE_E_TRANSITION;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(BaudRateConfigID >= ((uint16)Can_30_Core_GetInitObjectBaudrateLengthOfControllerConfig(Controller))))
  { /* (SPEC-50625, SPEC-50563) */
    errorId = CAN_30_CORE_E_PARAM_BAUDRATE;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Secure the mode change */
    Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_6);
    {
      /* #105 store temporary to redo in case transition fail */
      Can_30_Core_InitHandle currentBaudrateIndex = Can_30_Core_GetLastInitObjectOfControllerData(Controller);
      /* #110 Set new baudrate */
      Can_30_Core_SetLastInitObjectOfControllerData(Controller, (uint8)(Can_30_Core_GetInitObjectBaudrateStartIdxOfControllerConfig(Controller) + BaudRateConfigID)); /* PRQA S 2986 */ /* MD_Can_30_Core_ConstValue */ /* VCA_CAN_30_CORE_CONTROLLERDATA */
      /* #120 Re-Initialize controller to activate the baud rate */
      {
        Can_ReturnType transitionState = Can_30_Core_ModeTransition(Controller, CAN_30_COREMODESTOPREINIT, FALSE, FALSE);
        /* #130 check transition STOP (Reinit) is successful */
        if (transitionState == CAN_NOT_OK)
        { /* #140 transition fail: revert last init object (no new baudrate active) */
          Can_30_Core_SetLastInitObjectOfControllerData(Controller, (uint8)currentBaudrateIndex); /* VCA_CAN_30_CORE_CONTROLLERDATA */
        }
        else
        { /* #150 transition pass: store new logical mode in case CAN_OK or CAN_REQUESTED */
          retval = E_OK;
          Can_30_Core_AppendLogStatus(Controller, CAN_30_CORE_STATUS_STOP); /* STOP mode reached */
        }
      }
    }
    Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_6);
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_SET_BR, errorId);
  return retval;
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_Baudrate.c
 *********************************************************************************************************************/
