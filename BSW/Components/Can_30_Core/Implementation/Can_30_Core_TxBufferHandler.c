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
/*!       \file  Can_30_Core_TxBufferHandler.c
 *        \brief  Can_30_Core_TxBufferHandler source file
 *        \details  Contains the implementation of the Tx Buffer Handler unit.
 *        \unit Can_30_Core_TxBufferHandler
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
#define CAN_30_CORE_TXBUFFERHANDLER_SOURCE
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
#include "Can_30_Core_TxBufferHandler.h"
#include "Can_30_Core_DiagnosticHandler.h"
#include "Can_30_Core_ExclusiveAreaHandler.h"
#include "Can_30_Core_Initialization.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Can_30_Core_SetBufferStatus()
**********************************************************************************************************************/
/*! \brief       Set the buffer status
 *  \details     Sets the status of the buffer and stores the PduId
 *  \param[in]   hth              Handle to mailbox object
 *  \param[in]   mailboxElement   Handle to the mailbox element
 *  \param[in]   pduid            ID of the transmit pdu
 *  \param[in]   status           Buffer status (Can_30_Core_TxBufferStatusType)
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetBufferStatus(
  Can_HwHandleType hth,
  Can_HwHandleType mailboxElement,
  PduIdType pduid,
  Can_30_Core_TxBufferStatusType status);

/**********************************************************************************************************************
 *  Can_30_Core_PutMessageTxFifo()
 *********************************************************************************************************************/
/*! \brief       Push mesage to Tx FIFO
 *  \details     Pushes a message to the Tx FIFO
 *  \param[in]   hth                 Hardware transmit handle of the message
 *  \return      Can_HwHandleType    The mailbox element in the fifo if the FIFO is free, otherwise invalid element
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(Can_HwHandleType, CAN_30_CORE_STATIC_CODE) Can_30_Core_PutMessageTxFifo(Can_HwHandleType hth);

/**********************************************************************************************************************
 *  Can_30_Core_ConfirmMessageTxFifo()
 *********************************************************************************************************************/
/*! \brief       Pops a message from the Tx FIFO
 *  \details     Confirms the transmission of a message in the Tx FIFO
 *  \param[in]   hth  Hardware transmit handle of the message 
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ConfirmMessageTxFifo(Can_HwHandleType hth);

/**********************************************************************************************************************
 *  Can_30_Core_GetTxFifoFillLevel()
 *********************************************************************************************************************/
/*! \brief       Tx FIFO fill level
 *  \details     Returns the fill level of the tx FIFO
 *  \param[in]   txFifoIndex    Index of the Tx FIFO
 *  \return      uint8          The Tx FIFO fill level
 *  \pre         The Tx FIFO is initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(uint8, CAN_30_CORE_STATIC_CODE) Can_30_Core_GetTxFifoFillLevel(Can_HwHandleType txFifoIndex);

/**********************************************************************************************************************
 *  Can_30_Core_UpdateTxFifoReadIndex()
 *********************************************************************************************************************/
/*! \brief       Updates the Tx FIFO read index.
 *  \details     Sets the read index to the next message in the FIFO and updates the fill level.
 *  \param[in]   hth            Hardware transmit handle of the message
 *  \param[in]   txFifoIndex    Index of the Tx FIFO
 *  \pre         The Tx FIFO is initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_UpdateTxFifoReadIndex(Can_HwHandleType hth, Can_HwHandleType txFifoIndex);

/**********************************************************************************************************************
 *  Can_30_Core_UpdateTxFifoWriteIndex()
 *********************************************************************************************************************/
/*! \brief       Updates the Tx FIFO write index.
 *  \details     Sets the write index to the next free space in the FIFO and updates the fill level.
 *  \param[in]   hth            Hardware transmit handle of the message
 *  \param[in]   txFifoIndex    Index of the Tx FIFO
 *  \pre         The Tx FIFO is initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_UpdateTxFifoWriteIndex(Can_HwHandleType hth, Can_HwHandleType txFifoIndex);

/**********************************************************************************************************************
 *  Can_30_Core_IsTxFifoFull()
 *********************************************************************************************************************/
/*! \brief       Returns whether the FIFO is full or not.
 *  \details     -
 *  \param[in]   hth            Hardware transmit handle of the message
 *  \param[in]   txFifoIndex    Index of the Tx FIFO
 *  \return      FALSE          FIFO is not full.
 *  \return      TRUE           FIFO is full.
 *  \pre         The Tx FIFO is initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsTxFifoFull(Can_HwHandleType hth, Can_HwHandleType txFifoIndex);

/**********************************************************************************************************************
 *  Can_30_Core_ClearTxFifo()
 *********************************************************************************************************************/
/*! \brief       Clears the Tx FIFO.
 *  \details     Sets the read index, write index and fill level to zero.
 *  \param[in]   txFifoIndex    Index of the Tx FIFO
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ClearTxFifo(Can_HwHandleType txFifoIndex);

/**********************************************************************************************************************
 *  Can_30_Core_GetCanId4Comparison()
 *********************************************************************************************************************/
/*! \brief       Gets the CAN ID for comparison.
 *  \details     Returns a modification of the passed CAN-Id that can be used for CAN-Id based comparison.
 *  \param[in]   CanId    The CAN ID
 *  \return      CAN-Id that can be used for CAN-Id based comparison.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(uint32, CAN_30_CORE_STATIC_CODE) Can_30_Core_GetCanId4Comparison(Can_IdType CanId);

/**********************************************************************************************************************
 *  Can_30_Core_IsCanIdLowerPrio()
 *********************************************************************************************************************/
/*! \brief       Check the message priority
 *  \details     Check if CanId1 has a lower priority than CanId2
 *  \param[in]   CanId1    CAN ID that should be checked
 *  \param[in]   CanId2    CAN ID that should be compared
 *  \return      FALSE     CanId1 has a higher or equal priority compared to CanId2
 *  \return      TRUE      CanId1 has a lower priority than CanId2
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsCanIdLowerPrio(Can_IdType CanId1, Can_IdType CanId2);

/**********************************************************************************************************************
 *  Can_30_Core_GetCanId()
 *********************************************************************************************************************/
/*! \brief       Gets the CAN ID
 *  \details     Returns the CAN ID without metadata
 *  \param[in]   CanId    The CAN ID what should be checked
 *  \return      CanId    The real CAN ID as transmitted on the bus
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL FUNC(Can_IdType, CAN_30_CORE_STATIC_CODE) Can_30_Core_GetCanId(Can_IdType CanId);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Can_30_Core_UpdateTxFifoReadIndex()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_UpdateTxFifoReadIndex
  Relation_Context:
DevErrorDetect, TxBasicCan, HwCancelTx, TxHwFifo
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
 */
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_UpdateTxFifoReadIndex(Can_HwHandleType hth, Can_HwHandleType txFifoIndex) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Secure the Tx FIFO handling */
  Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_7);
  /* #110 Decrement fill level */
  Can_30_Core_DecFillCountOfTxHwFifo(txFifoIndex); /* VCA_CAN_30_CORE_TXHWFIFO */
  /* #120 Increment read index */
  Can_30_Core_IncReadIndexOfTxHwFifo(txFifoIndex); /* VCA_CAN_30_CORE_TXHWFIFO */
  /* #130 Update read index if it has reached the size of buffer */
  Can_30_Core_SetReadIndexOfTxHwFifo(txFifoIndex, Can_30_Core_GetReadIndexOfTxHwFifo(txFifoIndex) % Can_30_Core_GetMailboxSizeOfMailbox(hth)); /* VCA_CAN_30_CORE_TXHWFIFO */
  Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_7);
  CAN_30_CORE_DUMMY_STATEMENT(hth); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
}

/**********************************************************************************************************************
 *  Can_30_Core_UpdateTxFifoWriteIndex()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_UpdateTxFifoWriteIndex
  Relation_Context:
DevErrorDetect, TxBasicCan, HwCancelTx, TxHwFifo
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
 */
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_UpdateTxFifoWriteIndex(Can_HwHandleType hth, Can_HwHandleType txFifoIndex) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Secure the Tx FIFO handling */
  Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_7);
  /* #110 Increment fill level */
  Can_30_Core_IncFillCountOfTxHwFifo(txFifoIndex); /* VCA_CAN_30_CORE_TXHWFIFO */
  /* #120 Increment write index */
  Can_30_Core_IncWriteIndexOfTxHwFifo(txFifoIndex); /* VCA_CAN_30_CORE_TXHWFIFO */
  /* #130 Update write index if it has reached the size of buffer */
  Can_30_Core_SetWriteIndexOfTxHwFifo(txFifoIndex, Can_30_Core_GetWriteIndexOfTxHwFifo(txFifoIndex) % Can_30_Core_GetMailboxSizeOfMailbox(hth)); /* VCA_CAN_30_CORE_TXHWFIFO */
  Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_7);
  CAN_30_CORE_DUMMY_STATEMENT(hth); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
}

/**********************************************************************************************************************
 *  Can_30_Core_IsTxFifoFull()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_IsTxFifoFull
  Relation_Context:
DevErrorDetect, TxBasicCan, HwCancelTx, TxHwFifo
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
CAN_30_CORE_LOCAL FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsTxFifoFull(Can_HwHandleType hth, Can_HwHandleType txFifoIndex)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Check if the fill level has reached the buffer size */
  CAN_30_CORE_DUMMY_STATEMENT(hth); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  return Can_30_Core_GetFillCountOfTxHwFifo(txFifoIndex) >= Can_30_Core_GetMailboxSizeOfMailbox(hth);
}

/**********************************************************************************************************************
 *  Can_30_Core_ClearTxFifo()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ClearTxFifo
  Relation_Context:
DevErrorDetect, ChannelAmount, TxBasicCan, TxHwFifo
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
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ClearTxFifo(Can_HwHandleType txFifoIndex) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Reset the Fill Level */
  Can_30_Core_SetFillCountOfTxHwFifo(txFifoIndex, 0u); /* VCA_CAN_30_CORE_TXHWFIFO */
  /* #110 Reset the Read Index */
  Can_30_Core_SetReadIndexOfTxHwFifo(txFifoIndex, 0u); /* VCA_CAN_30_CORE_TXHWFIFO */
  /* #120 Reset the Read Write Index */
  Can_30_Core_SetWriteIndexOfTxHwFifo(txFifoIndex, 0u); /* VCA_CAN_30_CORE_TXHWFIFO */
}

/**********************************************************************************************************************
 *  Can_30_Core_SetBufferStatus()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_SetBufferStatus
  Relation_Context:
DevErrorDetect, TxBasicCan, TxHwFifo, IdenticalIdCancel, HwCancelTx
DevErrorDetect, TxFullCan, TxHwFifo, IdenticalIdCancel, HwCancelTx
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
 */
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_SetBufferStatus(
  Can_HwHandleType hth,
  Can_HwHandleType mailboxElement,
  PduIdType pduid,
  Can_30_Core_TxBufferStatusType status)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Get the active send object */
  Can_HwHandleType activeObject = (Can_HwHandleType)Can_30_Core_GetActiveSendObjectIdxOfMailbox(hth) + mailboxElement; /* PRQA S 2986 */ /* MD_Can_30_Core_ConstValue */
  /* #105 Check ActiveSendObject out of bounce write access */
  if(Can_30_Core_SafeBsw_IsConditionTrue(Can_30_Core_GetSizeOfActiveSendObject() <= activeObject))
  {
    errorId = CAN_30_CORE_E_GENDATA;
  }
  else
  {
    /* #110 Set the buffer status */
    Can_30_Core_SetStateOfActiveSendObject(activeObject, status); /* VCA_CAN_30_CORE_ACTIVESENDOBJECT */
    /* #120 Store the PduId in the buffer */
    Can_30_Core_SetPduOfActiveSendObject(activeObject, pduid); /* VCA_CAN_30_CORE_ACTIVESENDOBJECT */
  }  
    CAN_30_CORE_DUMMY_STATEMENT(hth); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_WRITE, errorId);
}

/**********************************************************************************************************************
 *  Can_30_Core_PutMessageTxFifo()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_PutMessageTxFifo
  Relation_Context:
DevErrorDetect, TxBasicCan, TxHwFifo
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
 */
CAN_30_CORE_LOCAL FUNC(Can_HwHandleType, CAN_30_CORE_STATIC_CODE) Can_30_Core_PutMessageTxFifo(Can_HwHandleType hth) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_HwHandleType mailboxElementFound;
  /* ----- Implementation ----------------------------------------------- */
  Can_HwHandleType txFifoIndex = (Can_HwHandleType)Can_30_Core_GetTxHwFifoIdxOfMailbox(hth);
  /* #100 Check that the Tx FIFO is not full */
  if (!Can_30_Core_IsTxFifoFull(hth, txFifoIndex))
  {
    /* #110 Get the write element of the Fifo (next available buffer) */
    mailboxElementFound = Can_30_Core_GetWriteIndexOfTxHwFifo(txFifoIndex);
    /* #120 Update TxHwFifo Write Index */
    Can_30_Core_UpdateTxFifoWriteIndex(hth, txFifoIndex);
  }
  else
  {
    mailboxElementFound = Can_30_Core_GetMailboxSizeOfMailbox(hth);
  }
  return mailboxElementFound;
}

/**********************************************************************************************************************
 *  Can_30_Core_ConfirmMessageTxFifo()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ConfirmMessageTxFifo
  Relation_Context:
DevErrorDetect, TxBasicCan, TxHwFifo
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
CAN_30_CORE_LOCAL FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_ConfirmMessageTxFifo(Can_HwHandleType hth) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_HwHandleType txFifoIndex = (Can_HwHandleType)Can_30_Core_GetTxHwFifoIdxOfMailbox(hth);

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Check if there is a message in the Tx FIFO */
  if (Can_30_Core_GetTxFifoFillLevel(txFifoIndex) > 0u)
  {
    /* #110 Update the Tx Fifo read index */
    Can_30_Core_UpdateTxFifoReadIndex(hth, txFifoIndex);
  }
}

/**********************************************************************************************************************
 *  Can_30_Core_GetTxFifoFillLevel()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetTxFifoFillLevel
  Relation_Context:
DevErrorDetect, TxBasicCan, TxHwFifo
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
CAN_30_CORE_LOCAL FUNC(uint8, CAN_30_CORE_STATIC_CODE) Can_30_Core_GetTxFifoFillLevel(Can_HwHandleType txFifoIndex) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Return the fill level of the TxFifo */
  return (uint8)Can_30_Core_GetFillCountOfTxHwFifo(txFifoIndex);
}

/**********************************************************************************************************************
 *  Can_30_Core_GetCanId4Comparison()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetCanId4Comparison
  Relation_Context:
DevErrorDetect, TxBasicCan, TxHwFifo, IdenticalIdCancel, HwCancelTx
  Relation:
IDType
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
CAN_30_CORE_LOCAL FUNC(uint32, CAN_30_CORE_STATIC_CODE) Can_30_Core_GetCanId4Comparison(Can_IdType CanId) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* #10 Get CAN-Id without extended bit */
  uint32 canId_temp = (uint32)Can_30_Core_GetCanId(CanId);

  /* # 20 Bit-shift standard message 18 bits, if mixed id support is enabled */
  if (Can_30_Core_IsSupportMixedIdOfGeneralFeature(0) && (!Can_30_Core_IsExtendedIdMessage(CanId))) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
  {
    canId_temp <<= 18u;
  }
  return canId_temp;
}

/**********************************************************************************************************************
 *  Can_30_Core_IsCanIdLowerPrio()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_IsCanIdLowerPrio
  Relation_Context:
DevErrorDetect, TxBasicCan, TxHwFifo, IdenticalIdCancel, HwCancelTx
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
CAN_30_CORE_LOCAL FUNC(boolean, CAN_30_CORE_STATIC_CODE) Can_30_Core_IsCanIdLowerPrio(Can_IdType CanId1, Can_IdType CanId2) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* #10 Get the CAN-Ids for comparison */
  uint32 canId1_temp = Can_30_Core_GetCanId4Comparison(CanId1);
  uint32 canId2_temp = Can_30_Core_GetCanId4Comparison(CanId2);
  boolean retval = FALSE; /* Assume that CanId1 has higher or equal prio compared to CanId2 */

  if (canId1_temp > canId2_temp)
  { /* #20 Check if CanId1 has a lower priority than CanId2 */
    retval = TRUE;
  }
  return retval;
}

/***********************************************************************************************************************
 * Can_30_Core_GetCanId()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL FUNC(Can_IdType, CAN_30_CORE_STATIC_CODE) Can_30_Core_GetCanId(Can_IdType CanId) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* #100 Return the real CanId without the metadata */
  return (Can_IdType)(CanId & CAN_30_CORE_ID_MASK);
}
#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_Allocate()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_Allocate
  Relation_Context:
DevErrorDetect, TxBasicCan
DevErrorDetect, TxFullCan
  Relation:
TxHwFifo, IdenticalIdCancel, HwCancelTx
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
/* PRQA S 6010,6030,6080 1 */ /* MD_MSR_STPTH,MD_MSR_STCYC,MD_MSR_STMIF */
CAN_30_CORE_LOCAL_API FUNC(Can_30_Core_TxBufferStatusType, CAN_30_CORE_CODE) Can_30_Core_Allocate(
  Can_HwHandleType hth,
  Can_HwHandleType *mailboxElement,
  Can_PduInfoPtrType PduInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_TxBufferStatusType retval = CAN_30_CORE_BUFFER_SEND;
  /* ----- Implementation ----------------------------------------------- */
  Can_HwHandleType mailboxElementFound = Can_30_Core_GetMailboxSizeOfMailbox(hth);
  /* #100 If the handle belongs to a Tx FIFO message */
  if (Can_30_Core_GetMailboxTypeOfMailbox(hth) == CAN_30_CORE_TX_BASICCAN_FIFO_TYPE)
  { /* #110 Put the message in the Tx FIFO */
    mailboxElementFound = Can_30_Core_PutMessageTxFifo(hth);
  }
  else
  {
    {
      Can_HwHandleType localMailboxElement = 0;
      /* #120 Loop over all mailboxelements in the mailbox  */
      for (; localMailboxElement < Can_30_Core_GetMailboxSizeOfMailbox(hth); localMailboxElement++) /* PRQA S 0771 */ /* MD_Can_30_Core_MultipleBreak */ /* FETA_CAN_30_CORE_1 */
      {
        /* #130 Cancel message with Identical ID in hardware for non-fullcan objects */
        if (Can_30_Core_IsIdenticalIdCancellationOfGeneralFeature(0) &&
            Can_30_Core_IsHardwareCancellationTxOfGeneralFeature(0) &&
            (Can_30_Core_GetMailboxTypeOfMailbox(hth) != CAN_30_CORE_TX_FULLCAN_TYPE))
        {
          Can_HwHandleType activeObject = (Can_HwHandleType)Can_30_Core_GetActiveSendObjectIdxOfMailbox(hth) + localMailboxElement; /* PRQA S 2986 */ /* MD_Can_30_Core_ConstValue */
          Can_IdType storedPduInfoId = Can_30_Core_GetPduInfoOfActiveSendObject(activeObject).id;
          if ((Can_30_Core_GetBufferStatus(hth, localMailboxElement) != CAN_30_CORE_BUFFER_FREE) &&
              (storedPduInfoId == PduInfo->id))
          {
            /* Cancel Identical Tx Message */
            mailboxElementFound = localMailboxElement;
            break;
          }
          else if (Can_30_Core_GetBufferStatus(hth, localMailboxElement) == CAN_30_CORE_BUFFER_FREE) /* PRQA S 2004 */ /* MD_Can_30_Core_NoElseAfterIf */
          { /* Do not break, the next mailboxelement might be identical ID */
            mailboxElementFound = localMailboxElement;
          }
        }
        else
        {
          /* #140 Check if the mailboxelement is available */
          if (Can_30_Core_GetBufferStatus(hth, localMailboxElement) == CAN_30_CORE_BUFFER_FREE)
          {
            mailboxElementFound = localMailboxElement;
            break;
          }
        }
      }
    }
    /* #150 No free mailbox found and no mailbox with identical ID was cancelled */
    if ((Can_30_Core_IsHardwareCancellationTxOfGeneralFeature(0)) &&
        (mailboxElementFound == Can_30_Core_GetMailboxSizeOfMailbox(hth)) &&
        (Can_30_Core_GetMailboxTypeOfMailbox(hth) != CAN_30_CORE_TX_FULLCAN_TYPE))
    {          
      Can_IdType lowestPrioId = PduInfo->id; /* has to be initialized outside the for-loop */
      Can_HwHandleType localMailboxElement = 0;
      for (; localMailboxElement < Can_30_Core_GetMailboxSizeOfMailbox(hth); localMailboxElement++) /* FETA_CAN_30_CORE_1 */
      {
        if (Can_30_Core_GetBufferStatus(hth, localMailboxElement) == CAN_30_CORE_BUFFER_SEND)
        { 
          P2CONST(Can_PduType, AUTOMATIC, CAN_30_CORE_VAR_NO_INIT) canPdu = Can_30_Core_GetStoredData(hth, localMailboxElement);
          /* #160 Cancel message if the stored message has lower priority */
          if ((Can_30_Core_IsCanIdLowerPrio(canPdu->id, lowestPrioId))) /* PRQA S 3415 */ /* MD_Can_30_Core_SideEffects */
          {
            lowestPrioId = canPdu->id;
            mailboxElementFound = localMailboxElement;
          }
        }
      }
    }
  }

  /* #170 Check if a "free" mailboxelement is found */
  if (mailboxElementFound < Can_30_Core_GetMailboxSizeOfMailbox(hth))
  {
    /* #180 Store the current bufferstatus */
    retval = Can_30_Core_GetBufferStatus(hth, mailboxElementFound);

    if ((Can_30_Core_IsHardwareCancellationTxOfGeneralFeature(0)) && (retval == CAN_30_CORE_BUFFER_SEND))
    {
      uint8 controller = Can_30_Core_GetControllerConfigIdxOfMailbox(hth);
      /* #190 Cancel the buffer transmission */
      Can_30_Core_GetTxCancelInHwFctOfExtensionConfig(Can_30_Core_GetExtensionConfigIdxOfControllerConfig(controller))(controller, hth, mailboxElementFound); /* VCA_CAN_30_CORE_FCTPOINTERTOEXT_HTRH */
      Can_30_Core_SetBufferStatus(hth, mailboxElementFound, PduInfo->swPduHandle, CAN_30_CORE_BUFFER_CANCEL);
    }
    else
    {
      /* #200 Allocate the buffer for transmission */
      Can_30_Core_SetBufferStatus(hth, mailboxElementFound, PduInfo->swPduHandle, CAN_30_CORE_BUFFER_SEND);
    }
    /* #210 Update the mailboxelement of the buffer */
    *mailboxElement = mailboxElementFound;
  }
  else
  { /* Send default buffer just in case */
    *mailboxElement = 0u;
  }

  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_GetBufferStatus()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetBufferStatus
  Relation_Context:
DevErrorDetect, TxBasicCan, TxFullCan, HwCancelTx, TransmitCancellationAPI
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
CAN_30_CORE_LOCAL_API FUNC(Can_30_Core_TxBufferStatusType, CAN_30_CORE_CODE) Can_30_Core_GetBufferStatus(
  Can_HwHandleType hth,
  Can_HwHandleType mailboxElement)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Get the active send object */
  Can_HwHandleType activeObject = (Can_HwHandleType)Can_30_Core_GetActiveSendObjectIdxOfMailbox(hth) + mailboxElement; /* PRQA S 2986 */ /* MD_Can_30_Core_ConstValue */
  /* #110 Get the status of the buffer from the active send object */
  Can_30_Core_TxBufferStatusType retval = Can_30_Core_GetStateOfActiveSendObject(activeObject);
  CAN_30_CORE_DUMMY_STATEMENT(hth); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_GetPduId()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetPduId
  Relation_Context:
DevErrorDetect, TxBasicCan, TxFullCan
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
CAN_30_CORE_LOCAL_API FUNC(PduIdType, CAN_30_CORE_CODE) Can_30_Core_GetPduId(Can_HwHandleType hth, Can_HwHandleType mailboxElement)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Get the active send object */
  Can_HwHandleType activeObject = (Can_HwHandleType)Can_30_Core_GetActiveSendObjectIdxOfMailbox(hth) + mailboxElement; /* PRQA S 2986 */ /* MD_Can_30_Core_ConstValue */
  /* #110 Get the PduId of the active send object */
  PduIdType retval = Can_30_Core_GetPduOfActiveSendObject(activeObject);
  CAN_30_CORE_DUMMY_STATEMENT(hth); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  return retval;
}

/**********************************************************************************************************************
 *  Can_30_Core_Release()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_Release
  Relation_Context:
DevErrorDetect, TxBasicCan, TxFullCan, HwCancelTx, TransmitCancellationAPI
  Relation:
TxHwFifo
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
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_Release(Can_HwHandleType hth, Can_HwHandleType mailboxElement)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 If the handle belongs to a Tx FIFO message */
  if (Can_30_Core_GetMailboxTypeOfMailbox(hth) == CAN_30_CORE_TX_BASICCAN_FIFO_TYPE)
  { /* #110 Ackknowledge the message transmission to the tx fifo */
    Can_30_Core_ConfirmMessageTxFifo(hth);
  }
  /* #120 Free the buffer */
  Can_30_Core_SetBufferStatus(hth, mailboxElement, 0x0, CAN_30_CORE_BUFFER_FREE);
}

/**********************************************************************************************************************
 *  Can_30_Core_RevokeTxRequest()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_RevokeTxRequest
  Relation_Context:
TxBasicCan, TxHwFifo
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
 */
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_RevokeTxRequest(Can_HwHandleType hth, Can_HwHandleType mailboxElement)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #100 If the handle belongs to a Tx FIFO message */
  if (Can_30_Core_GetMailboxTypeOfMailbox(hth) == CAN_30_CORE_TX_BASICCAN_FIFO_TYPE)
  {
    Can_HwHandleType txFifoIndex = (Can_HwHandleType)Can_30_Core_GetTxHwFifoIdxOfMailbox(hth);

    /* #110 Secure the Tx FIFO handling */
    Can_30_Core_EnterExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_7);

    /* #120 Decrement write index */
    Can_30_Core_SetWriteIndexOfTxHwFifo(txFifoIndex, /* PRQA S 2985, 2986 */ /* MD_Can_30_Core_ConstValue */ /* VCA_CAN_30_CORE_TXHWFIFO */
      (Can_30_Core_GetWriteIndexOfTxHwFifo(txFifoIndex) + (Can_30_Core_MailboxSizeOfMailboxType)(Can_30_Core_GetMailboxSizeOfMailbox(hth) - (uint8)1))
      % Can_30_Core_GetMailboxSizeOfMailbox(hth)); /* Index 0 must go back to last index due to ring buffer behavior */

    /* #130 Decrement fill level */
    Can_30_Core_DecFillCountOfTxHwFifo(txFifoIndex); /* VCA_CAN_30_CORE_TXHWFIFO */
    Can_30_Core_ExitExclusiveArea(CAN_30_CORE_EXCLUSIVE_AREA_7);
  }

  /* #140 Free the buffer */
  Can_30_Core_SetBufferStatus(hth, mailboxElement, 0x0, CAN_30_CORE_BUFFER_FREE);
}

/**********************************************************************************************************************
 *  Can_30_Core_CancelBuffer()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_CancelBuffer
  Relation_Context:
DevErrorDetect, TxBasicCan, TxFullCan, HwCancelTx, TransmitCancellationAPI
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
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_CancelBuffer(Can_HwHandleType hth, Can_HwHandleType mailboxElement) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Get the active send object */
  Can_HwHandleType activeObject = (Can_HwHandleType)Can_30_Core_GetActiveSendObjectIdxOfMailbox(hth) + mailboxElement; /* PRQA S 2986 */ /* MD_Can_30_Core_ConstValue */
  /* #110 Cancel the buffer */
  Can_30_Core_SetBufferStatus(hth, mailboxElement, Can_30_Core_GetPduOfActiveSendObject(activeObject), CAN_30_CORE_BUFFER_CANCEL_SW);
}

/**********************************************************************************************************************
 *  Can_30_Core_StoreData()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_StoreData
  Relation_Context:
DevErrorDetect, TxBasicCan, TxFullCan, CanFdSupport
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
 */
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_StoreData(
  Can_HwHandleType hth,
  Can_HwHandleType mailboxElement,
  Can_PduInfoPtrType PduInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Get the active send object */
  Can_HwHandleType activeObject = (Can_HwHandleType)Can_30_Core_GetActiveSendObjectIdxOfMailbox(hth) + mailboxElement; /* PRQA S 2986 */ /* MD_Can_30_Core_ConstValue */
  /* #105 Check ActiveSendObject out of bounce write access */
  if(Can_30_Core_SafeBsw_IsConditionTrue(Can_30_Core_GetSizeOfActiveSendObject() <= activeObject))
  {
    errorId = CAN_30_CORE_E_GENDATA;
  }
  else
  {
    /* #110 Store PduInfo in active send object table */
    Can_30_Core_SetPduInfoOfActiveSendObject(activeObject, *PduInfo); /* VCA_CAN_30_CORE_ACTIVESENDOBJECT */
  }
  if (Can_30_Core_IsActiveSendDataOfGeneralFeature(0))
  {
    /* #120 Calculate the index of the active send data table */
    uint32 activeSendData = (Can_30_Core_GetActiveSendDataIdxOfMailbox(hth) + ((uint32)mailboxElement * Can_30_Core_GetMaxDataLenOfMailbox(hth))); /* PRQA S 2986 */ /* MD_Can_30_Core_ConstValue */
    /* #125 Check ActiveSendData out of bounce write access */
    if(Can_30_Core_SafeBsw_IsConditionTrue(Can_30_Core_GetSizeOfActiveSendData() < (activeSendData + PduInfo->length)))
    {
      errorId = CAN_30_CORE_E_GENDATA;
    }
    else
    {
      /* #130 Store data (sdu) */
      VStdMemCpy(Can_30_Core_GetAddrActiveSendData(activeSendData), PduInfo->sdu, PduInfo->length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_CAN_30_CORE_CALLVSTDMEMCPYTXREQUEST */
    }
  }
  CAN_30_CORE_DUMMY_STATEMENT(hth); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_WRITE, errorId);
}

/**********************************************************************************************************************
 *  Can_30_Core_GetStoredData()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetStoredData
  Relation_Context:
DevErrorDetect, TxBasicCan, TxFullCan, GenericConfirmation, GenericPreTransmit, HwCancelTx
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
 */
FUNC_P2VAR(Can_PduType, AUTOMATIC, CAN_30_CORE_CODE) Can_30_Core_GetStoredData(
  Can_HwHandleType hth,
  Can_HwHandleType mailboxElement)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Get the active send object */
  Can_HwHandleType activeObject = (Can_HwHandleType)Can_30_Core_GetActiveSendObjectIdxOfMailbox(hth) + mailboxElement; /* PRQA S 2986 */ /* MD_Can_30_Core_ConstValue */
  /* 110 Check if the active send data is available */
  if (Can_30_Core_IsActiveSendDataOfGeneralFeature(0))
  {
    /* #120 Calculate the index of the active send data table */
    uint32 activeSendData = (Can_30_Core_GetActiveSendDataIdxOfMailbox(hth) + ((uint32)mailboxElement * Can_30_Core_GetMaxDataLenOfMailbox(hth))); /* PRQA S 2986 */ /* MD_Can_30_Core_ConstValue */
    /* #125 Check ActiveSendObject out of bounce write access */
    if(Can_30_Core_SafeBsw_IsConditionTrue(Can_30_Core_GetSizeOfActiveSendObject() <= activeObject))
    {
      errorId = CAN_30_CORE_E_GENDATA;
    }
    else
    {
      /* #130 Point the sdu of the active send object pdu to the send data */
      Can_30_Core_GetAddrPduInfoOfActiveSendObject(activeObject)->sdu = Can_30_Core_GetAddrActiveSendData(activeSendData); /* VCA_CAN_30_CORE_ACTIVESENDOBJECT */
    }
  }
  CAN_30_CORE_DUMMY_STATEMENT(hth); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_WRITE, errorId);
  /* #140 Return the pointer to the Active send object Pdu */
  return Can_30_Core_GetAddrPduInfoOfActiveSendObject(activeObject); 
}

/**********************************************************************************************************************
 *  Can_30_Core_GetTxFifoReadIndex()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetTxFifoReadIndex
  Relation_Context:
DevErrorDetect, TxBasicCan, TxHwFifo
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
CAN_30_CORE_LOCAL_API FUNC(Can_HwHandleType, CAN_30_CORE_CODE) Can_30_Core_GetTxFifoReadIndex(Can_HwHandleType hth) /* COV_CAN_30_CORE_FCT_CALL_NOT_IN_ALL_CONFIG */
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Get the Tx FIFO index */
  Can_HwHandleType txFifoIndex = (Can_HwHandleType)Can_30_Core_GetTxHwFifoIdxOfMailbox(hth);
  CAN_30_CORE_DUMMY_STATEMENT(hth); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  /* #110 Get the mailbox element at the read index */
  return Can_30_Core_GetReadIndexOfTxHwFifo(txFifoIndex);
}

/**********************************************************************************************************************
 *  Can_30_Core_TxBufferHandlerClear()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_TxBufferHandlerClear
  Relation_Context:
DevErrorDetect, ChannelAmount
  Relation:
TxBasicCan, TxHwFifo
TxFullCan
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
FUNC(void, CAN_30_CORE_CODE) Can_30_Core_TxBufferHandlerClear(uint8 Controller)
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
  else
  {  
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Loop over all the basiccan transmit objects */
    {
      Can_HwHandleType hth = (Can_HwHandleType)Can_30_Core_GetMailboxTxBasicStartIdxOfControllerConfig(Controller);
      for (; hth < (Can_HwHandleType)Can_30_Core_GetMailboxTxBasicEndIdxOfControllerConfig(Controller); hth++) /* FETA_CAN_30_CORE_1 */
      {
        /* #120 Loop over all the mailbox elements */
        Can_HwHandleType mailboxElement = 0u;
        for (; mailboxElement < Can_30_Core_GetMailboxSizeOfMailbox(hth); mailboxElement++) /* FETA_CAN_30_CORE_1 */
        {
          /* #130 Initialize the transmit buffer */
          Can_30_Core_SetBufferStatus(hth, mailboxElement, 0x00, CAN_30_CORE_BUFFER_FREE);
        }
        if (Can_30_Core_GetMailboxTypeOfMailbox(hth) == CAN_30_CORE_TX_BASICCAN_FIFO_TYPE)
        {
          /* #140 Clear the Tx Hw Fifo */
          Can_30_Core_ClearTxFifo((Can_HwHandleType)Can_30_Core_GetTxHwFifoIdxOfMailbox(hth));
        }
      }
    }
    
    /* #200 Loop over all the fullcan transmit objects */
    {
      Can_HwHandleType hth = (Can_HwHandleType)Can_30_Core_GetMailboxTxFullStartIdxOfControllerConfig(Controller);
      for (; hth < (Can_HwHandleType)Can_30_Core_GetMailboxTxFullEndIdxOfControllerConfig(Controller); hth++) /* PRQA S 2994, 2996 */ /* MD_Can_30_Core_ConstValue */ /* FETA_CAN_30_CORE_1 */
      {
        /* #210 Loop over all the mailbox elements */
        Can_HwHandleType mailboxElement = 0u;
        for (; mailboxElement < Can_30_Core_GetMailboxSizeOfMailbox(hth); mailboxElement++) /* PRQA S 2880 */ /* MD_MSR_Unreachable */ /* FETA_CAN_30_CORE_1 */
        {
          /* #220 Initialize the transmit buffer */
          Can_30_Core_SetBufferStatus(hth, mailboxElement, 0x00, CAN_30_CORE_BUFFER_FREE);
        }
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_CLEAR_TX_BUFFERS, errorId);
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_TxBufferHandler.c
 *********************************************************************************************************************/
