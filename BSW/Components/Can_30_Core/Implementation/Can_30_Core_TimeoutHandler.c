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
/*!       \file  Can_30_Core_TimeoutHandler.c
 *        \brief  Can_30_Core_TimeoutHandler source file
 *        \details  Contains the implementation of the Timeout Handler unit.
 *        \unit Can_30_Core_TimeoutHandler
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
#define CAN_30_CORE_TIMEOUTHANDLER
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
#include "Can_30_Core_TimeoutHandler.h"
#include "Can_30_Core_DiagnosticHandler.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_TimerUpdate()
 *********************************************************************************************************************/
/*! \brief       Executes the timer update
 *  \details     Function to update a start or end timer
 *  \param[in]   Controller    The requested controller on which the hardware observation takes place.
 *  \param[in]   source        Source for the hardware observation
 *  \param[in]   timer         Timer that should be updated
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_TimerUpdate(uint8 Controller, uint8 source, Can_30_Core_TimerUpdateType timer);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Can_30_Core_TimerUpdate()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_TimerUpdate
  Relation_Context:
DevErrorDetect, ChannelAmount, HardwareLoopCheck
DevErrorDetect, ChannelAmount, RamCheck
  Relation:
HardwareCancelByAppl
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_TimerUpdate(uint8 Controller, uint8 source, Can_30_Core_TimerUpdateType timer)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(source >= Can_30_Core_GetLoopTimeoutLengthOfControllerConfig(Controller))) 
  { /* #10 Check if source parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_HANDLE;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
  { /* #20 Check if controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Check if hardware loop timer should be used */
    if (Can_30_Core_IsHardwareLoopCheckOfGeneralFeature(0) || (source == CAN_30_CORE_LOOP_MODE_CHANGE))
    {
      /* #110 Check if hardware loops should be handled by application */
      if (Can_30_Core_IsHardwareCancelByApplOfGeneralFeature(0))
      {
        if (timer == CAN_30_CORE_TIMER_START)
        {
          Can_30_Core_CallApplTimerStart(Controller, source);
        }
        else
        {
          Can_30_Core_CallApplTimerEnd(Controller, source);
        }
      }
      else
      { /* Internal handling of hardware loops */
        if (timer == CAN_30_CORE_TIMER_START)
        { /* #120 Update the timer value at the requested source */
          Can_30_Core_LoopTimeoutIterType loopTimeoutIndex = source + (Can_30_Core_LoopTimeoutIterType) Can_30_Core_GetLoopTimeoutStartIdxOfControllerConfig(Controller); /* PRQA S 2985, 2986 */ /* MD_Can_30_Core_ConstValue */
          if(Can_30_Core_SafeBsw_IsConditionTrue(Can_30_Core_GetSizeOfLoopTimeout() <= loopTimeoutIndex))
          {
            errorId = CAN_30_CORE_E_GENDATA;
          }
          else
          {
            Can_30_Core_CallGetCounterValue(Can_30_Core_GetOsCounterIdOfControllerConfig(Controller), Can_30_Core_GetAddrLoopTimeout(loopTimeoutIndex)); /* VCA_CAN_30_CORE_CALLGETCOUNTERVALUE */
          }
        }
      }
    }
  }
  /* ----- Development Error Reporting --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_TIMER_UPDATE, errorId);
}
#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_TimerLoop()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_TimerLoop
  Relation_Context:
DevErrorDetect, ChannelAmount, HardwareLoopCheck
DevErrorDetect, ChannelAmount, RamCheck
Multipartition
  Relation:
HardwareCancelByAppl
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
FUNC(Can_ReturnType, CAN_30_CORE_CODE) Can_30_Core_TimerLoop(uint8 Controller, uint8 source)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK;
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  if (Can_30_Core_IsHardwareLoopCheckOfGeneralFeature(0) || (source == CAN_30_CORE_LOOP_MODE_CHANGE))
  {
    /* Internal handling of hardware loops */
    /* ----- Development Error Checks ------------------------------------- */
    if (Can_30_Core_DevCheck_IsConditionTrue(source >= Can_30_Core_GetLoopTimeoutLengthOfControllerConfig(Controller)))
    { /* #10 Check if source parameter is valid */
      errorId = CAN_30_CORE_E_PARAM_HANDLE;
    }
    else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
    { /* #20 Check if controller parameter is valid */
      errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
    }
    else
    {
      /* ----- Implementation ----------------------------------------------- */
      if (Can_30_Core_IsHardwareCancelByApplOfGeneralFeature(0))
      {
        retval = Can_30_Core_CallApplTimerLoop(Controller, source);
      }
      else
      {
        Can_30_Core_LoopTimeoutIterType loopTimeoutIndex = source + (Can_30_Core_LoopTimeoutIterType) Can_30_Core_GetLoopTimeoutStartIdxOfControllerConfig(Controller); /* PRQA S 2985, 2986 */ /* MD_Can_30_Core_ConstValue */
        Can_30_Core_ExternalTickType elapsedTime = 0;
        /* #100 Get the start time for the source */
        Can_30_Core_ExternalTickType startTime = Can_30_Core_GetLoopTimeout(loopTimeoutIndex);
        /* #110 Calculate the elapsed time since the start */
        if (Can_30_Core_CallGetElapsedValue(Can_30_Core_GetOsCounterIdOfControllerConfig(Controller), &startTime, &elapsedTime) != (StatusType)E_OK) /* PRQA S 2992, 2996 */ /* MD_Can_30_Core_ConstValue */ /* VCA_CAN_30_CORE_CALLGETELAPSEDVALUE */
        {
          errorId = CAN_30_CORE_E_TIMEOUT_DET; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
        }
        else
        {
          /* #120 Check if a timeout occurs for hardware loop check */
          if (elapsedTime < Can_30_Core_GetTimeoutDurationOfControllerConfig(Controller))
          { /* No timeout occurred */
            retval = CAN_OK;
          }
          else
          {
            /* #130 Do not throw a DET for synchronous mode change */
            if (source != CAN_30_CORE_LOOP_MODE_CHANGE)
            {
              errorId = CAN_30_CORE_E_TIMEOUT_DET;
            }
          }
        }
      }
    }
    /* ----- Development Error Reporting --------------------------------------- */
    Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_TIMER_LOOP, errorId);
  }
  else
  {
    /* #300 Feature not active, return CAN_OK */
    retval = CAN_OK;
  }
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_TimerStart()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_TimerStart
  Relation_Context:
DevErrorDetect, ChannelAmount, HardwareLoopCheck
DevErrorDetect, ChannelAmount, RamCheck
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
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_TimerStart(uint8 Controller, uint8 source)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Development Error Checks ------------------------------------- */
  /* DevChecks implemented in TimerUpdate to avoid code duplication */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call update timer with CAN_30_CORE_TIMER_START */
  Can_30_Core_TimerUpdate(Controller, source, CAN_30_CORE_TIMER_START);
}

/**********************************************************************************************************************
 *  Can_30_Core_TimerEnd()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_TimerEnd
  Relation_Context:
DevErrorDetect, ChannelAmount, HardwareLoopCheck
DevErrorDetect, ChannelAmount, RamCheck
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
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_TimerEnd(uint8 Controller, uint8 source)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Development Error Checks ------------------------------------- */
  /* DevChecks implemented in TimerUpdate to avoid code duplication */
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call update timer with CAN_30_CORE_TIMER_END */
  Can_30_Core_TimerUpdate(Controller, source, CAN_30_CORE_TIMER_END);
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_TimeoutHandler.c
 *********************************************************************************************************************/
