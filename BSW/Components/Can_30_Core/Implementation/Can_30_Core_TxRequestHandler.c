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
/*!       \file  Can_30_Core_TxRequestHandler.c
 *        \brief  Can_30_Core_TxRequestHandler source file
 *        \details  Contains the implementation of the Tx Request Handler unit.
 *        \unit Can_30_Core_TxRequestHandler
 *********************************************************************************************************************/
#define CAN_30_CORE_TXREQUESTHANDLER_SOURCE
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
#include "Can_30_Core_TxRequestHandler.h"
#include "Can_30_Core_TxBufferHandler.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ExclusiveAreaHandler.h"
#include "Can_30_Core_ModeHandler.h"
#include "Can_30_Core_Initialization.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Can_30_Core_TxPrepare()
**********************************************************************************************************************/
/*! \brief      Prepares the Transmit
 *  \details    The prepare buffer function calls the generic pretransmit and stores data for the buffer if needed
 *  \param[in]  hth               Handle of the mailbox intended to send the message
 *  \param[in]  mailboxElement    Handle to the mailbox element
 *  \param[in]  PduInfo           Pointer to the Pdu Information (ID, dataLength, data)
 *  \return     CAN_OK            success
 *  \return     CAN_NOT_OK        error
 *  \context    ANY
 *  \reentrant  TRUE
 *  \pre        -
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_INLINE FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_TxPrepare(
  Can_HwHandleType hth,
  Can_HwHandleType mailboxElement,
  Can_PduInfoPtrType PduInfo);

/**********************************************************************************************************************
 *  Can_30_Core_IsIllegalBitsInCanId()
 *********************************************************************************************************************/
/*! \brief       Check for illegal bits
 *  \details     Checks the CAN ID for illegal bits
 *  \param[in]   hth      message handle
 *  \param[in]   CanId    CAN ID
 *  \return      FALSE    No illegal bits
 *  \return      TRUE     illegal bits detected
 *  \pre         Hth must be a valid index in the mailboxtable
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsIllegalBitsInCanId(Can_HwHandleType hth, Can_IdType CanId);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  Can_30_Core_TxPrepare()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_TxPrepare
  Relation_Context:
DevErrorDetect, TxBasicCan, TxFullCan, ChannelAmount
  Relation:
GenericPreTransmit
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
CAN_30_CORE_LOCAL_INLINE FUNC(Can_ReturnType, CAN_30_CORE_STATIC_CODE) Can_30_Core_TxPrepare(
  Can_HwHandleType hth,
  Can_HwHandleType mailboxElement,
  Can_PduInfoPtrType PduInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK;
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Check if tx buffer is allocated for transmit */
  if (Can_30_Core_GetBufferStatus(hth, mailboxElement) == CAN_30_CORE_BUFFER_SEND)
  {
    /* #110 Store the Pdu Info */
    Can_30_Core_StoreData(hth, mailboxElement, PduInfo);
    /* #120 Call Generic PreTransmit with a pointer to the stored data */
    if (Can_30_Core_IsGenericPreTransmitOfGeneralFeature(0))
    {
      Can_30_Core_CallApplGenericPreTransmit(Can_30_Core_GetControllerConfigIdxOfMailbox(hth), Can_30_Core_GetStoredData(hth, mailboxElement)); /* VCA_CAN_30_CORE_CALLAPPLGENERICPRETRANSMIT */
    }
    retval = CAN_OK;
  }
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_IsIllegalBitsInCanId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CAN_30_CORE_LOCAL_INLINE FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsIllegalBitsInCanId(Can_HwHandleType hth, Can_IdType CanId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retval = FALSE;
  /* ----- Development Error Checks ------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Check if CanId contains illegal bits */
  if ((CanId & (Can_IdType)CAN_30_CORE_ID_UNUSED_MASK) != (Can_IdType)0UL)
  {
    retval = TRUE;
  }
  else if (Can_30_Core_IsSupportMixedIdOfGeneralFeature(0) &&
           (!Can_30_Core_IsExtendedIdMessage(CanId)) && /* PRQA S 2992,3415 */ /* MD_Can_30_Core_ConstValue,MD_Can_30_Core_SideEffects */
           ((CanId & (Can_IdType)CAN_30_CORE_ID_MASK) > (Can_IdType)CAN_30_CORE_ID_MASK_STD)) /* PRQA S 2996 */ /* MD_Can_30_Core_ConstValue */
  { /* #110 Check parameter PduInfo->id for STD ID is in STD range (EXT is always in range) */
    retval = TRUE;
  }
  else if (!Can_30_Core_IsSupportExtIdOfGeneralFeature(0) && (Can_30_Core_IsExtendedIdMessage(CanId))) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
  { /* #120 Check if CanId contains extended bit when it is not expected */
    retval = TRUE;
  }
  else if ((Can_30_Core_GetFdSupportOfControllerConfig(Can_30_Core_GetControllerConfigIdxOfMailbox(hth)) == CAN_30_CORE_FD_NONE) && /* PRQA S 2004 */ /* MD_Can_30_Core_NoElseAfterIf */
           Can_30_Core_IsFdMessage(CanId)) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
  { /* #130 Check if the CanId belongs to a CanFd message when it is not configured */
    retval = TRUE;
  }
  CAN_30_CORE_DUMMY_STATEMENT(hth); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
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
 *  Can_30_Core_Write()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_Write
  Relation_Context:
DevErrorDetect, TxBasicCan, TxFullCan, ChannelAmount
DevErrorDetect, TxBasicCan, TxHwFifo, MultiplexedTx
DevErrorDetect, TxBasicCan, TxFullCan, CanFdSupport
DevErrorDetect, TxBasicCan, TxFullCan, RamCheck
DevErrorDetect, TxBasicCan, TxFullCan, GenericPreTransmit
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
FUNC(Can_ReturnType, CAN_30_CORE_CODE) Can_30_Core_Write(Can_HwHandleType hth, Can_PduInfoPtrType PduInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_ReturnType retval = CAN_NOT_OK;
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;

  CanHookBegin_Can_30_Core_Write();

  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if controller is valid and state is INIT (SPEC-1759) */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(hth >= Can_30_Core_GetSizeOfMailbox()))
  { /* #20 Check if the handle is inside the expected range */
    errorId = CAN_30_CORE_E_PARAM_HANDLE;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue((Can_30_Core_GetMailboxTypeOfMailbox(hth) != CAN_30_CORE_TX_BASICCAN_TYPE) &&
                                                (Can_30_Core_GetMailboxTypeOfMailbox(hth) != CAN_30_CORE_TX_BASICCAN_MUX_TYPE) &&
                                                (Can_30_Core_GetMailboxTypeOfMailbox(hth) != CAN_30_CORE_TX_BASICCAN_FIFO_TYPE) &&
                                                (Can_30_Core_GetMailboxTypeOfMailbox(hth) != CAN_30_CORE_TX_FULLCAN_TYPE)))
  { /* #30 Check if the handle belongs to a tx message (SPEC-1763) */
    errorId = CAN_30_CORE_E_PARAM_HANDLE;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(PduInfo == NULL_PTR))
  { /* #40 Check if the pdu info pointer is valid (SPEC-1761) */
    errorId = CAN_30_CORE_E_PARAM_POINTER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue((PduInfo->sdu == NULL_PTR) && (PduInfo->length != 0u)))
  { /* #50 Check if the sdu pointer is valid  */
    errorId = CAN_30_CORE_E_PARAM_POINTER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue((Can_30_Core_GetFdSupportOfControllerConfig(Can_30_Core_GetControllerConfigIdxOfMailbox(hth)) == CAN_30_CORE_FD_FULL) &&
                                                (PduInfo->length > 8u) &&
                                                (!Can_30_Core_IsFdBrs(Can_30_Core_GetControllerConfigIdxOfMailbox(hth))))) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
  { /* #60 CAN_FD: Check parameter PduInfo->length is smaller than 9 for none FD configuration (SPEC-1758, SPEC-60433) */
    errorId = CAN_30_CORE_E_PARAM_DATA_LENGTH;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(PduInfo->length > Can_30_Core_GetMaxDataLenOfMailbox(hth)))
  { /* #70 Check parameter PduInfo->length against maximum buffer size (SPEC-1758) */
    errorId = CAN_30_CORE_E_PARAM_DATA_LENGTH;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(Can_30_Core_IsIllegalBitsInCanId(hth, PduInfo->id)))
  { /* #80 Check for illegal bits in the CanId */
    errorId = CAN_30_CORE_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    Can_PduType localPduInfo;
    Can_PduInfoPtrType localPduInfoPtr;
    Can_HwHandleType mailboxElement;
    uint8 Controller = Can_30_Core_GetControllerConfigIdxOfMailbox(hth);

    /* #100 Check if FD is enabled, the DLC is not greater than 8, baud rate switching is deactivated and the message is FD */
    if ((Can_30_Core_GetFdSupportOfControllerConfig(Controller) != CAN_30_CORE_FD_NONE) &&
        (PduInfo->length <= 8u) &&
        (!Can_30_Core_IsFdBrs(Controller)) && /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
        (Can_30_Core_IsFdMessage(PduInfo->id))) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
    {
      /* #101 Store the PduInfo in a local copy (needed for FD) */
      localPduInfo.id = PduInfo->id;
      localPduInfo.length = PduInfo->length;
      localPduInfo.sdu = PduInfo->sdu;
      localPduInfo.swPduHandle = PduInfo->swPduHandle;
      /* #102 Remove FD flag from the Can ID */
      localPduInfo.id = (localPduInfo.id & (~CAN_30_CORE_ID_FD_MASK));
      /* #103 Point the local pduInfo to the local copy of the PduInfo */
      localPduInfoPtr = &localPduInfo;
    }
    else
    {
      /* #110 Point the local pduInfo to the PduInfo Parameter */
      localPduInfoPtr = PduInfo;
    }
    /* #120 Secure the transmission */
    Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_2);
    if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Controller)))
    { /* #130 Check if the controller is valid */
      errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
    }
    else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(Controller)))
    { /* #140 Check if the controller is initialized */
      errorId = CAN_30_CORE_E_UNINIT;
    }

    /* An explicit DET check, whether the controller is in Start mode is excluded by the CAN Driver AUTOSAR specification (shall be ensured by the caller) */

    else if ((Can_30_Core_GetRamCheckOfControllerConfig(Controller) != CAN_30_CORE_RAMCHECK_NONE) &&
        (Can_30_Core_GetMailboxState(hth) == CAN_NOT_OK))
    { /* #150 Reject deactivated mailbox (by RamCheck) */
      retval = CAN_NOT_OK;
    }
    else if (Can_30_Core_IsIsBusOffOfControllerData(Controller))
    { /* #160 Check Hardware is in BUSOFF recovery state and is not able to handle a request (SPEC-1764) */
      retval = CAN_NOT_OK;
    }
    else if ((Can_30_Core_GetRamCheckOfControllerConfig(Controller) != CAN_30_CORE_RAMCHECK_NONE) &&
             (Can_30_Core_IsControllerDeactivated(Controller))) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
    { /* #170 Check if the controller is activated (no RAM check issue) */
      retval = CAN_NOT_OK;
    }
    else if (Can_30_Core_Allocate(hth, &mailboxElement, localPduInfoPtr) == CAN_30_CORE_BUFFER_FREE)
    { /* #180 Allocate a tx buffer to the transmit handle */

      if (Can_30_Core_TxPrepare(hth, mailboxElement, localPduInfoPtr) == CAN_OK)
      { /* #190 Prepare the tx buffer for transmit */

        if (Can_30_Core_IsGenericPreTransmitOfGeneralFeature(0))
        { /* #200 Call Extension TxStart() with a pointer to the stored data */
          retval = Can_30_Core_GetTxStartFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, hth, mailboxElement, Can_30_Core_GetStoredData(hth, mailboxElement)); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT_HTRH */
        }
        else
        { /* #210 Call Extension TxStart() */
          retval = Can_30_Core_GetTxStartFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Controller))(Controller, hth, mailboxElement, localPduInfoPtr); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT_HTRH */
        }

        if (retval != CAN_OK)
        {
          /* #220 Release the tx buffer if TxStart() is not OK */
          Can_30_Core_RevokeTxRequest(hth, mailboxElement);
        }
      }
      else
      {
        /* #230 Preparation of the tx buffer failed (keep the current tx buffer status) */
        retval = CAN_NOT_OK;
      }
    }
    else
    { /* #240 No buffer available (already pending message) */
      retval = CAN_BUSY;
    }
    Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_2);
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_WRITE, errorId);
  CanHookEnd_Can_30_Core_Write();
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_CancelTx()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_CancelTx
  Relation_Context:
DevErrorDetect, TxBasicCan, TxFullCan, ChannelAmount
DevErrorDetect, TxBasicCan, TxHwFifo, HwCancelTx
DevErrorDetect, TxBasicCan, TxFullCan, TransmitCancellationAPI, TxHwFifo, HwCancelTx
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
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_CancelTx(Can_HwHandleType Hth, PduIdType PduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsDriverInitialized()))
  { /* #10 Check if the driver is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(Hth >= Can_30_Core_GetSizeOfMailbox()))
  { /* #20 Check if the handle is valid */
    errorId = CAN_30_CORE_E_PARAM_HANDLE;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(Can_30_Core_IsHardwareCancellationTxOfGeneralFeature(0) &&
           (Can_30_Core_GetMailboxTypeOfMailbox(Hth) == CAN_30_CORE_TX_BASICCAN_FIFO_TYPE)))
  { /* #30 Check if the type of the mailbox is valid for cancellation in hardware */
    errorId = CAN_30_CORE_E_GENDATA;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerValid(Can_30_Core_GetControllerConfigIdxOfMailbox(Hth))))
  { /* #40 Check if the controller parameter is valid */
    errorId = CAN_30_CORE_E_PARAM_CONTROLLER;
  }
  else if (Can_30_Core_DevCheck_IsConditionTrue(!Can_30_Core_IsControllerInitialized_Internal(Can_30_Core_GetControllerConfigIdxOfMailbox(Hth))))
  { /* #50 Check if the controller is initialized */
    errorId = CAN_30_CORE_E_UNINIT;
  }
  /* #60 Do CancelSw only when configured, otherwise CanIf will not get any TX notification */
  else if (Can_30_Core_IsTxCancelInSwOfGeneralFeature(0)) /* PRQA S 2004 */ /* MD_Can_30_Core_NoElseAfterIf */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Loop over all mailbox elements */
    uint8 mailboxElement = 0;
    for (; mailboxElement < Can_30_Core_GetMailboxSizeOfMailbox(Hth); mailboxElement++) /* FETA_CAN_30_CORE_1 */
    {
      /* #105 Secure the cancellation */
      Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_2);
      /* #110 Check if a message is pending in the buffer (marked as send or for Hw cancel) */
      if ((Can_30_Core_GetBufferStatus(Hth, mailboxElement) == CAN_30_CORE_BUFFER_SEND) ||
          (Can_30_Core_GetBufferStatus(Hth, mailboxElement) == CAN_30_CORE_BUFFER_CANCEL)) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
      {
        /* #120 Check if the requested PduId is pending in the buffer */
        if (Can_30_Core_GetPduId(Hth, mailboxElement) == PduId)
        {
          if (Can_30_Core_IsHardwareCancellationTxOfGeneralFeature(0) && (Can_30_Core_GetMailboxTypeOfMailbox(Hth) != CAN_30_CORE_TX_BASICCAN_FIFO_TYPE))
          {            
            /* #130 Call Extension TxCancelInHw */
            Can_30_Core_GetTxCancelInHwFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(Can_30_Core_GetControllerConfigIdxOfMailbox(Hth)))(Can_30_Core_GetControllerConfigIdxOfMailbox(Hth), Hth, mailboxElement); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT_HTRH */
          }
          /* #140 Set buffer status to cancel */
          Can_30_Core_CancelBuffer(Hth, mailboxElement);
          
        }
      }
      Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_2);
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CANCEL_MSG_TX, errorId);
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_TxRequestHandler.c
 *********************************************************************************************************************/
