/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file  Can_30_Core_TxBufferHandler.h
 *        \brief  Can_30_Core_TxBufferHandler header file
 *        \details  Header of the Tx Buffer Handler unit. It provides the API declaration for this unit used by other units.
 *        \unit Can_30_Core_TxBufferHandler
 *********************************************************************************************************************/
#if !defined(CAN_30_CORE_TXBUFFERHANDLER_H)
#define CAN_30_CORE_TXBUFFERHANDLER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core_PrivateTypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_Allocate()
 *********************************************************************************************************************/
/*! \brief       Transmit buffer allocation
 *  \details     Allocates a transmit buffer and mailboxelement for the requested hth and returns the previous buffer
 *               status
 *  \param[in]   hth              Handle to mailbox object
 *  \param[in]   mailboxElement   Pointer to mailbox element
 *  \param[in]   PduInfo            Pointer to the pdu information
 *  \return      CAN_30_CORE_Bufferfree      Buffer is free
 *  \return      CAN_30_CORE_BufferCancel    Buffer is allocated and marked with cancel (PIA)
 *  \return      CAN_30_CORE_BufferCancelSw  Buffer is allocated and marked with cancelSw
 *  \return      CAN_30_CORE_BufferSend      Buffer is allocated
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(Can_30_Core_TxBufferStatusType, CAN_30_CORE_CODE)  Can_30_Core_Allocate(
  Can_HwHandleType hth,
  Can_HwHandleType *mailboxElement,
  Can_PduInfoPtrType PduInfo);

/**********************************************************************************************************************
 *  Can_30_Core_GetBufferStatus()
 *********************************************************************************************************************/
/*! \brief       Get the buffer status
 *  \details     Returns the status of the hth
 *  \param[in]   hth              Handle to mailbox object
 *  \param[in]   mailboxElement   Handle to the mailbox element
 *  \return      CAN_30_CORE_Bufferfree      Buffer is free
 *  \return      CAN_30_CORE_BufferCancel    Buffer is allocated and marked with cancel (PIA)
 *  \return      CAN_30_CORE_BufferCancelSw  Buffer is allocated and marked with cancelSw
 *  \return      CAN_30_CORE_BufferSend      Buffer is allocated
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(Can_30_Core_TxBufferStatusType, CAN_30_CORE_CODE) Can_30_Core_GetBufferStatus(
  Can_HwHandleType hth,
  Can_HwHandleType mailboxElement);

/**********************************************************************************************************************
 *  Can_30_Core_GetPduId()
 *********************************************************************************************************************/
/*! \brief       Get the PduId stored in the buffer
 *  \details     Returns the PduId stored in the buffer
 *  \param[in]   hth              Handle to mailbox object
 *  \param[in]   mailboxElement   Handle to the mailbox element
 *  \return      PduId            ID of the transmit Pdu stored in the buffer
 *  \pre         Buffer is previsously allocated
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(PduIdType, CAN_30_CORE_CODE) Can_30_Core_GetPduId(Can_HwHandleType hth, Can_HwHandleType mailboxElement);

/**********************************************************************************************************************
 *  Can_30_Core_Release()
 *********************************************************************************************************************/
/*! \brief       Releases the transmit buffer
 *  \details     Sets the buffer status to free
 *  \param[in]   hth              Handle to mailbox object
 *  \param[in]   mailboxElement   Handle to the mailbox element
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_Release(Can_HwHandleType hth, Can_HwHandleType mailboxElement);

/**********************************************************************************************************************
 *  Can_30_Core_RevokeTxRequest()
 *********************************************************************************************************************/
/*! \brief       Revoke Tx request.
 *  \details     Sets the buffer status back to free and for a Tx FIFO, also removes one message from the Tx FIFO by 
 *               decreasing its write index and fillcount.
 *  \param[in]   hth                 Hardware transmit handle of the message
 *  \param[in]   mailboxElement      Handle to the mailbox element
 *  \pre         Tx Fifo must not be empty
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_STATIC_CODE) Can_30_Core_RevokeTxRequest(Can_HwHandleType hth, Can_HwHandleType mailboxElement);

/**********************************************************************************************************************
 *  Can_30_Core_CancelBuffer()
 *********************************************************************************************************************/
/*! \brief       Cancels the transmit buffer
 *  \details     Sets the buffer status to Cancel
 *  \param[in]   hth              Handle to mailbox object
 *  \param[in]   mailboxElement   Handle to the mailbox element
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_CancelBuffer(Can_HwHandleType hth, Can_HwHandleType mailboxElement);

/**********************************************************************************************************************
 *  Can_30_Core_StoreData()
 *********************************************************************************************************************/
/*! \brief       Store PduInfo and data before transmission
 *  \details     Multiple features (GenericConfApi2, FIFO and TxHwCancellation) needs to access the transmitted data
 *               after transmission, either in confirmation or precopy.
 *  \param[in]   hth              Handle to mailbox object
 *  \param[in]   mailboxElement   Handle to mailbox element
 *  \param[in]   PduInfo          Pointer to local PduInfo struct
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_StoreData(
  Can_HwHandleType hth,
  Can_HwHandleType mailboxElement,
  Can_PduInfoPtrType PduInfo);

/**********************************************************************************************************************
 *  Can_30_Core_GetStoredData()
 *********************************************************************************************************************/
/*! \brief       Returns the Stored PduInfo and data
 *  \details     Updates the sdu and returns a pointer to the active send data
 *  \param[in]   hth              Handle to mailbox object
 *  \param[in]   mailboxElement   Handle to mailbox element
 *  \return      Can_PduType      Pointer to active send data
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC_P2VAR(Can_PduType, AUTOMATIC, CAN_30_CORE_CODE) Can_30_Core_GetStoredData(
  Can_HwHandleType hth,
  Can_HwHandleType mailboxElement);

/**********************************************************************************************************************
 *  Can_30_Core_GetTxFifoReadIndex()
 *********************************************************************************************************************/
/*! \brief       Gets the Tx FIFO read index
 *  \details     Returns the mailbox handle at the Tx FIFO read index
 *  \param[in]   hth                 Handle to the mailbox that should be checked
 *  \return      Can_HwHandleType    Mailbox element at the Tx FIFO read index
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(Can_HwHandleType, CAN_30_CORE_CODE) Can_30_Core_GetTxFifoReadIndex(Can_HwHandleType hth);

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CAN_30_CORE_TXBUFFERHANDLER_H */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_TxBufferHandler.h
 *********************************************************************************************************************/
