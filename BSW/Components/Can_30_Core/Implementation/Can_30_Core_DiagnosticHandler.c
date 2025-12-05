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
/*!       \file  Can_30_Core_DiagnosticHandler.c
 *        \brief  Can_30_Core_DiagnosticHandler source file
 *        \details  Contains the implementation of the Diagnostic handler unit.
 *        \unit Can_30_Core_DiagnosticHandler
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
#define CAN_30_CORE_DIAGNOSTICHANDLER_SOURCE
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
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_Initialization.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_Overrun()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_Overrun
  Relation_Context:
DevErrorDetect, ChannelAmount, RxFullCan, RxBasicCan, Overrun
  Relation:
RxBasicCan, RxFullCan, ChannelAmount  
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
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_Overrun(uint8 Controller, Can_HwHandleType Hrh) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
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
  else if (Can_30_Core_DevCheck_IsConditionTrue((Can_30_Core_GetMailboxTypeOfMailbox(Hrh) != CAN_30_CORE_RX_BASICCAN_TYPE) &&
           (Can_30_Core_GetMailboxTypeOfMailbox(Hrh) != CAN_30_CORE_RX_FULLCAN_TYPE)))
  { /* #20 Check if the handle belongs to an Rx mailbox */
    errorId = CAN_30_CORE_E_PARAM_HANDLE;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Check if overrun notification  is enabled */
    if (Can_30_Core_GetOverrunNotificationOfGeneralFeature(0) != CAN_30_CORE_OVERRUN_NONE)
    {
      /* #110 Check if the overrun should be notified to the DET */
      if (Can_30_Core_GetOverrunNotificationOfGeneralFeature(0) == CAN_30_CORE_OVERRUN_DET)
      {
        /* #111 Notify the overrun */
        errorId = CAN_30_CORE_E_DATALOST;
      }
      else
      { /* #120 Overrun notified to the application */
        if (Can_30_Core_GetMailboxTypeOfMailbox(Hrh) == CAN_30_CORE_RX_BASICCAN_TYPE)
        {
          /* #121 Notify BasicCan Overrun */
          Can_30_Core_CallApplCanOverrun(Controller);
        }
        else
        {
          /* #122 Notify FullCan Overrun */
          Can_30_Core_CallApplCanFullCanOverrun(Controller);
        }
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_RX_OVERRUN, errorId);
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_DiagnosticHandler.c
 *********************************************************************************************************************/
