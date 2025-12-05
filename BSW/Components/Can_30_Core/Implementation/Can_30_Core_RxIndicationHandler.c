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
/*!       \file  Can_30_Core_RxIndicationHandler.c
 *        \brief  Can_30_Core_RxIndicationHandler source file
 *        \details  Contains the implementation of the Rx Indication Handler unit.
 *        \unit Can_30_Core_RxIndicationHandler
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
#define CAN_30_CORE_RXINDICATIONHANDLER_SOURCE
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
#include "Can_30_Core_RxIndicationHandler.h"
#include "Can_30_Core_RxQueueHandler.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ModeHandler.h"
#include "Can_30_Core_Initialization.h"
#include "CanIf.h"
#include "CanIf_Cbk.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_CheckDLCLength()
 *********************************************************************************************************************/
/*! \brief       Check the DLC length
 *  \details     Checks if the DLC length of the message is valid
 *  \param[in]   hrh        The hardware receive handle
 *  \param[in]   PduInfo    Pointer to the pdu information
 *  \return      FALSE      The DLC is not valid
 *  \return      TRUE       The DLC is valid
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_CheckDLCLength(Can_HwHandleType hrh, Can_PduInfoPtrType PduInfo);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Can_30_Core_CheckDLCLength()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_CheckDLCLength
  Relation_Context:
DevErrorDetect, RxFullCan
DevErrorDetect, RxBasicCan
  Relation:
CanFdSupport
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
CAN_30_CORE_LOCAL FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_CheckDLCLength(Can_HwHandleType hrh, Can_PduInfoPtrType PduInfo)
{ /* ----- Local Variables ---------------------------------------------- */
  boolean retval = FALSE;
  /* ----- Implementation ----------------------------------------------- */
  if (PduInfo->length > Can_30_Core_GetMaxDataLenOfMailbox(hrh))
  { /* #100 DLC is bigger than expected length from mailbox (MAX over all PDU for this mailbox) */
    retval = TRUE;
  }
  else if ((Can_30_Core_GetFdSupportOfControllerConfig(Can_30_Core_GetControllerConfigIdxOfMailbox(hrh)) != CAN_30_CORE_FD_NONE) &&
           (!Can_30_Core_IsFdMessage(PduInfo->id)) && /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
           (PduInfo->length > 8u))
  { /* #110 DLC is bigger than expected length (8) for CLASSIC CAN msg */
    retval = TRUE;
  }
  else if ((Can_30_Core_GetFdSupportOfControllerConfig(Can_30_Core_GetControllerConfigIdxOfMailbox(hrh)) != CAN_30_CORE_FD_FULL) && /* PRQA S 2004 */ /* MD_Can_30_Core_NoElseAfterIf */
           (PduInfo->length > 8u))
  { /* #120 Check if the DLC is bigger than 8 for classic CAN message */
    retval = TRUE;
  }
  CAN_30_CORE_DUMMY_STATEMENT(hrh); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
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
 *  Can_30_Core_Indication()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_Indication
  Relation_Context:
ChannelAmount, VariantSel, DevErrorDetect, RxPolling, RxBasicCan, RxFullCan, IDType, CanFdSupport
  Relation:
DevErrorDetect, RxQueue, GenericPreCopy, MirrorMode
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
 /* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_Indication(Can_HwHandleType hrh, Can_PduInfoPtrType PduInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #5 Check if driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }

  /* An explicit DET check in callback functions, whether the controller is in Start mode is excluded by the CAN Driver AUTOSAR specification */

  else if (Can_30_Core_DevCheck_IsConditionTrue(hrh >= Can_30_Core_GetSizeOfMailbox()))
  { /* #10 Check if the hardware handle is in the expected range */
    errorId = CAN_30_CORE_E_PARAM_HANDLE;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue((Can_30_Core_GetMailboxTypeOfMailbox(hrh) != CAN_30_CORE_RX_BASICCAN_TYPE) && 
                                                (Can_30_Core_GetMailboxTypeOfMailbox(hrh) != CAN_30_CORE_RX_FULLCAN_TYPE)))
  { /* #20 Check if the hardware handle belongs to a Rx object */
    errorId = CAN_30_CORE_E_PARAM_HANDLE;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(PduInfo == NULL_PTR))
  { /* #30 Check that the given PduInfo pointer is not a null pointer */
    errorId = CAN_30_CORE_E_PARAM_POINTER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(PduInfo->sdu == NULL_PTR))
  { /* #35 Check that the given SDU pointer is not a null pointer */
    errorId = CAN_30_CORE_E_PARAM_POINTER;
  }
  else if (Can_30_Core_CheckDLCLength(hrh, PduInfo))
  { /* #40 Check that DLC is not bigger than expected length for the mailbox - always active */
    errorId = CAN_30_CORE_E_PARAM_DATA_LENGTH;
  }
  else if ((Can_30_Core_GetFdSupportOfControllerConfig(Can_30_Core_GetControllerConfigIdxOfMailbox(hrh)) == CAN_30_CORE_FD_NONE) &&
           Can_30_Core_IsFdMessage(PduInfo->id)) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
  { /* #50 Check that the message is not FD when it is not expected */
    errorId = CAN_30_CORE_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    if (Can_30_Core_IsRxQueueOfGeneralFeature(0))
    {
      /* #100 Store data in the RxQueue and store the returned errorId */
      errorId = Can_30_Core_PutMessage(hrh, PduInfo); 
    }
    else
    {
      Can_ReturnType genericRetval = CAN_NOT_OK;
      if (Can_30_Core_IsGenericPrecopyOfGeneralFeature(0))
      {
        uint8 Controller = Can_30_Core_GetControllerConfigIdxOfMailbox(hrh);
        if (Can_30_Core_IsMirrorModeOfGeneralFeature(0) &&
            (Can_30_Core_GetMirrorMode(Controller) == CDDMIRROR_INACTIVE)) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
        { /* #110 Do not call Can_30_Core_CallApplGenericPrecopy() in case mirror mode feature is enabled but mirror mode was deactivated */
          genericRetval = CAN_OK;
        }
        else
        {
          /* #120 Call Can_30_Core_CallApplGenericPrecopy() */
          genericRetval = Can_30_Core_CallApplGenericPrecopy((uint8)Controller, PduInfo->id, PduInfo->length, (Can_DataPtrType)PduInfo->sdu); /* VCA_CAN_30_CORE_CALLAPPLGENERICPRECOPY */
        }
      }
      if ((!Can_30_Core_IsGenericPrecopyOfGeneralFeature(0)) || (genericRetval == CAN_OK)) /* (SPEC-15119) */
      {
        /* #130 Call CanIf_RxIndication() if mirror mode is not active or Can_30_Core_CallApplGenericPrecopy() returns CAN_OK (SPEC-1726, SPEC-1688, SPEC-1687, SPEC-1687) */
        CanIf_30_Core_RxIndication(hrh, PduInfo->id, PduInfo->length, PduInfo->sdu);
      }
      CAN_30_CORE_DUMMY_STATEMENT(genericRetval); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_RX_INDICATION, errorId);
}
/**********************************************************************************************************************
 *  Can_30_Core_MainFunction_Read_Internal()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_MainFunction_Read_Internal
  Relation_Context:
  Relation:
ChannelAmount, Multipartition, VariantSel, DevErrorDetect, Wakeup, RxPolling
ChannelAmount, RxQueue
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
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_MainFunction_Read_Internal(Can_30_Core_OsApplicationType ApplicationId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId= CAN_30_CORE_E_NO_ERROR;

  CanHookBegin_Can_30_Core_MainFunction_Read();
  /* ----- Implementation ----------------------------------------------- */
  if (Can_30_Core_IsDriverInitialized())
  {
    /* #100 Loop over all Controllers */
    uint8 Controller = 0u;
    for (; Controller < Can_30_Core_GetSizeOfControllerConfig(); Controller++)
    {
      if (Can_30_Core_GetOsApplicationIdOfControllerConfig(Controller) != ApplicationId)
      { /* #105 Skip if controller is not in partition */
        continue;
      }
      if (Can_30_Core_IsPostBuildSelectableOfGeneralFeature(0) &&
          (Can_30_Core_GetCanIfChannelId(Controller) == CAN_30_CORE_NO_CANIFCHANNELID))
      { /* #110 Skip inactive controllers */ 
        continue;
      }
      if (!Can_30_Core_IsRxProcessingPollingOfControllerConfig(Controller))
      { /* #115 Skip if RX polling is disabled for the controller */
        continue;
      }
      /* #120 Check if controller is initialized */
      if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(Controller)))
      {
        errorId = CAN_30_CORE_E_UNINIT;
      }
      else
      {
        /* #130 Do not access hardware sleep mode */
        if (!Can_30_Core_IsControllerInSleep(Controller))
        {
          /* #140 Call Extension Mainfunction read */
          Can_30_Core_GetRxPollingFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
        }
      }
    }
    if (errorId == CAN_30_CORE_E_NO_ERROR)
    {
      if (Can_30_Core_IsRxQueueOfGeneralFeature(0))
      { /* #150 Handle the Rx Queue */
        Can_30_Core_RxQueueExecution();
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_MAINFCT_READ, errorId);
  CanHookEnd_Can_30_Core_MainFunction_Read();
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_RxIndicationHandler.c
 *********************************************************************************************************************/
