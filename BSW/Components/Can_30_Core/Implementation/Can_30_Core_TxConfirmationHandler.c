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
/*!       \file  Can_30_Core_TxConfirmationHandler.c
 *        \brief  Can_30_Core_TxConfirmationHandler source file
 *        \details  Contains the implementation of the Tx Confirmation handler unit.
 *        \unit Can_30_Core_TxConfirmationHandler
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
#define CAN_30_CORE_TXCONFIRMATIONHANDLER_SOURCE
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
#include "vstdlib.h"
#include "Can_30_Core_TxConfirmationHandler.h"
#include "Can_30_Core_TxBufferHandler.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ExclusiveAreaHandler.h"
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
 *  Can_30_Core_GenericConfirmation()
 *********************************************************************************************************************/
/*! \brief       Performs the generic confirmation
 *  \details     Calls the generic confirmatiaon APIs (API1 or API2)
 *  \param[in]   Controller    The requested controller
 *  \param[in]   PduInfoPtr    The pointer to the pdu information
 *  \return      CAN_NOT_OK    Generic return value is not ok
 *  \return      CAN_OK        Generic return value is ok
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_GenericConfirmation(uint8 Controller, Can_PduInfoPtrType PduInfoPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Can_30_Core_GenericConfirmation()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GenericConfirmation
  Relation_Context:
DevErrorDetect, TxBasicCan, TxFullCan, ChannelAmount
DevErrorDetect, TxBasicCan, TxFullCan, HwCancelTx, TransmitCancellationAPI, GenericConfirmation, MirrorMode
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
 */
CAN_30_CORE_LOCAL FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_GenericConfirmation(uint8 Controller, Can_PduInfoPtrType PduInfoPtr) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_OK;
  /* ----- Implementation ----------------------------------------------- */
  if (Can_30_Core_GetGenericConfirmationOfGeneralFeature(0) == CAN_30_CORE_GENERICCONF_API1)
  { /* #100 Call generic confirmation API1 */
    retval = Can_30_Core_CallApplGenericConfirmationApi1(PduInfoPtr->swPduHandle);
  }
  else  /* no check against API2 - this function is only called for API1 or API2 */
  { /* #110 Call generic confirmation API2 */
    retval = Can_30_Core_CallApplGenericConfirmationApi2(Controller, PduInfoPtr); /* VCA_CAN_30_CORE_CALLAPPLGENERICCONFIRMATIONAPI2 */
  }
  CAN_30_CORE_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  CAN_30_CORE_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
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
 *  Can_30_Core_MainFunction_Write_Internal()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_MainFunction_Write_Internal
  Relation_Context:
  Relation:
ChannelAmount, Multipartition, VariantSel, DevErrorDetect, Wakeup, TxPolling
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
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_MainFunction_Write_Internal(Can_30_Core_OsApplicationType ApplicationId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;

  CanHookBegin_Can_30_Core_MainFunction_Write();
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
      if (Can_30_Core_IsPostBuildSelectableOfGeneralFeature(0) &&
          (Can_30_Core_GetCanIfChannelId(controller) == CAN_30_CORE_NO_CANIFCHANNELID))
      { /* #120 Skip inactive controllers */
        continue;
      }
      if (!Can_30_Core_IsTxProcessingPollingOfControllerConfig(controller))
      { /* #125 Skip if TX polling (include vCanHardwareCancellationTaskSupport) is disabled for the controller */      
        continue;
      }
      /* #130 Check if controller is initialized */
      if (Can_30_Core_DevCheck_IsConditionTrue((!Can_30_Core_IsControllerInitialized_Internal(controller))))
      {
        errorId = CAN_30_CORE_E_UNINIT;
      }
      else
      {
        /* #140 Do not access hardware sleep mode */
        if (!Can_30_Core_IsControllerInSleep(controller))
        {
          /* #150 Call Extension Mainfunction Write */
          Can_30_Core_GetTxPollingFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(controller))(controller); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT */
        }
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_MAINFCT_WRITE, errorId);
  CanHookEnd_Can_30_Core_MainFunction_Write();
}

/**********************************************************************************************************************
 *  Can_30_Core_Confirmation()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_Confirmation
  Relation_Context:
DevErrorDetect, VariantSel, ChannelAmount, Wakeup, TxPolling
  Relation:
DevErrorDetect, TxBasicCan, MultiplexedTx, TxHwFifo
DevErrorDetect, TxBasicCan, TxFullCan, ChannelAmount
DevErrorDetect, TxBasicCan, TxFullCan, HwCancelTx, TransmitCancellationAPI, GenericConfirmation, MirrorMode
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
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030,6050,6080 1 */ /* MD_MSR_STCYC,MD_MSR_STCAL,MD_MSR_STMIF */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_Confirmation(Can_HwHandleType hth, Can_HwHandleType mailboxElement)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  Can_ReturnType generic_retval;  
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #5 Check if driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }

  /* An explicit DET check in callback functions, whether the controller is in Start mode is excluded by the CAN Driver AUTOSAR specification */

  else if (Can_30_Core_DevCheck_IsConditionTrue(hth >= Can_30_Core_GetSizeOfMailbox()))
  { /* #10 Check that the handle is inside the expected range */
    errorId = CAN_30_CORE_E_PARAM_HANDLE;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue((Can_30_Core_GetMailboxTypeOfMailbox(hth) != CAN_30_CORE_TX_BASICCAN_TYPE) &&
                                                (Can_30_Core_GetMailboxTypeOfMailbox(hth) != CAN_30_CORE_TX_BASICCAN_MUX_TYPE) &&
                                                (Can_30_Core_GetMailboxTypeOfMailbox(hth) != CAN_30_CORE_TX_BASICCAN_FIFO_TYPE) &&
                                                (Can_30_Core_GetMailboxTypeOfMailbox(hth) != CAN_30_CORE_TX_FULLCAN_TYPE)))
  { /* #20 Check that the handle belongs to a Tx mailbox */
    errorId = CAN_30_CORE_E_PARAM_HANDLE;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(mailboxElement > Can_30_Core_GetMailboxSizeOfMailbox(hth)))
  { /* #30 Check that the mailbox element is inside the expected range */
    errorId = CAN_30_CORE_E_PARAM_HANDLE;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */  
    /* #100 Check if the message belongs in the Tx FIFO */
    if (Can_30_Core_GetMailboxTypeOfMailbox(hth) == CAN_30_CORE_TX_BASICCAN_FIFO_TYPE)
    {
      mailboxElement = Can_30_Core_GetTxFifoReadIndex(hth); /* PRQA S 1338 */ /* MD_Can_30_Core_ParamValueMod */
    }
    {
      /* #110 Get the status of the buffer */
      Can_30_Core_TxBufferStatusType bufferstatus = Can_30_Core_GetBufferStatus(hth, mailboxElement);
      if (bufferstatus == CAN_30_CORE_BUFFER_FREE)
      { /* #120 Confirmation should not be called for a free buffer */
        errorId = CAN_30_CORE_E_PARAM_HANDLE;
      }
      else 
      {
        uint8 controller = Can_30_Core_GetControllerConfigIdxOfMailbox(hth);
        boolean isMsgTransmitted = TRUE;
        /* #140 Get the PduId stored in the buffer */
        PduIdType tmp_pdu = Can_30_Core_GetPduId(hth, mailboxElement);
        if ((Can_30_Core_IsHardwareCancellationTxOfGeneralFeature(0)) && (bufferstatus != CAN_30_CORE_BUFFER_SEND))
        { /* #150 Call Extension to determine if the canceled CAN frame was successfully transmitted (only if HardwareCancellationTx is enabled) */
          isMsgTransmitted = Can_30_Core_GetTxConfIsMsgTransmittedFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(controller))(controller, hth, mailboxElement); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT_HTRH */
        }
        /* #160 If the CAN frame was transmitted on the bus */
        if (isMsgTransmitted)
        {
          /* #180 If Generic Confirmation is active */
          if (Can_30_Core_GetGenericConfirmationOfGeneralFeature(0) != CAN_30_CORE_GENERICCONF_NONE)
          {
            /* #190 If Mirror Mode is enabled but inactive */
            if ((Can_30_Core_IsMirrorModeOfGeneralFeature(0)) &&
                (Can_30_Core_GetMirrorMode(controller) == CDDMIRROR_INACTIVE)) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
            { /* #191 Set the generic return value to CAN_OK */
              /* #192 Release the transmit buffer when no generic confirmation is used */
              Can_30_Core_Release(hth, mailboxElement);
              generic_retval = CAN_OK;
            }
            else
            {
              /* #195 Release the transmit buffer when generic confirmation is used after storing data */
              Can_PduType tmpPdu;
              uint8 canData[64]; /* reserve max size (FD) */
              const Can_PduType* pduPtr = Can_30_Core_GetStoredData(hth, mailboxElement);
              tmpPdu.sdu = (Can_SduPtrType)canData;
              tmpPdu.id = pduPtr->id;
              tmpPdu.swPduHandle = pduPtr->swPduHandle;
              tmpPdu.length = pduPtr->length;
              VStdMemCpy(tmpPdu.sdu, pduPtr->sdu, pduPtr->length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_CAN_30_CORE_CALLVSTDMEMCPYCONFIRMATION */
              Can_30_Core_Release(hth, mailboxElement);
              /* #200 Call the generic confirmation */
              generic_retval = Can_30_Core_GenericConfirmation(controller, &tmpPdu);
            }
          }
          else
          {
            /* #205 Release the transmit buffer when no generic confirmation is used */
            Can_30_Core_Release(hth, mailboxElement);
            generic_retval = CAN_OK;
          }
          /* #210 If generic return value is ok */
          if (generic_retval == CAN_OK) /* PRQA S 2991, 2995 */ /* MD_Can_30_Core_ConstValue */
          {
            /* #220 If buffer was cancelled by software cancellation request */
            if (bufferstatus == CAN_30_CORE_BUFFER_CANCEL_SW)
            {
              /* #221 Call CanIf Cancel Tx Notification */
              Can_30_Core_CallCanIf_CancelTxNotification(tmp_pdu, FALSE);
            }
            else /* (bufferstatus == CAN_30_CORE_BUFFER_SEND) || (bufferstatus == CAN_30_CORE_BUFFER_CANCEL) (cancel in HW was no longer possible) */
            {
              /* #230 Call CanIf Tx Confirmation */
              CanIf_30_Core_TxConfirmation(tmp_pdu);
            }
          }
          /* #240 Generic Retval is not CAN_OK */
          else
          {
            if (Can_30_Core_IsTxCancelInSwOfGeneralFeature(0) && Can_30_Core_IsTransmitBufferOfGeneralFeature(0))
            {
              /* #250 Call CanIf Cancel Tx Notification */
              Can_30_Core_CallCanIf_CancelTxNotification(tmp_pdu, FALSE);
            }
          }
        }
        /* #260 Mailbox is cancelled in hardware */
        else
        {
          /* #270 If buffer was cancelled by software cancellation request */
          if (bufferstatus == CAN_30_CORE_BUFFER_CANCEL_SW)
          {
            /* #280 Release the buffer before CanIf Cancel Tx Notification */
            Can_30_Core_Release(hth, mailboxElement);
            /* #290 Call CanIf Cancel Tx Notification */
            Can_30_Core_CallCanIf_CancelTxNotification(tmp_pdu, TRUE);
          }
          /* #300 If buffer was cancelled by internal PIA or IdenticalIdCancellation */
          else /* bufferstatus == CAN_30_CORE_BUFFER_CANCEL */
          {
            /* EXCLUSIVE_AREA_2: needed to avoid another Can_Write() while CAN Interface send out of Tx buffer (first send out of Tx buffer avoids inversion) -> no call to upper layer here */
            Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_2);
            /* #310 Release the buffer */
            Can_30_Core_Release(hth, mailboxElement);
            if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsActiveSendDataOfGeneralFeature(0)))
            {
              errorId = CAN_30_CORE_E_GENDATA;
            }
            else
            {
              /* #320 Call Cancel Tx Confirmation to re trigger queue (SPEC-1726, SPEC-1725, SPEC-1684, SPEC-1689) */
              Can_30_Core_CallCanIf_CancelTxConfirmation(Can_30_Core_GetStoredData(hth, mailboxElement)->swPduHandle, Can_30_Core_GetStoredData(hth, mailboxElement)); /* VCA_CAN_30_CORE_CALLCANIFCANCELTXCONFIRMATION */
            }
            Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_2);
          }
        }
        CAN_30_CORE_DUMMY_STATEMENT(controller); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_TX_CONFIRMATION, errorId);
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_TxConfirmationHandler.c
 *********************************************************************************************************************/
