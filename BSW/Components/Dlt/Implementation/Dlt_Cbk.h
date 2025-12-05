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
 *  -------------------------------------------------------------------------------------------------------------------
 *        \file   Dlt_Cbk.h
 *        \brief  Dlt Callback header file
 *
 *      \details  Types header file of the AUTOSAR Diagnostic Log and Trace, according to:
 *                AUTOSAR Diagnostic Log and Trace, AUTOSAR Release 4.0
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/


#if !defined (DLT_CBK_H)
# define DLT_CBK_H


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Dlt_Types.h"

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Dlt_MessageControlInfoType */
#  define DLT_CONTROL_REQUEST             0x1u
#  define DLT_CONTROL_RESPONSE            0x2u

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#  define DLT_START_SEC_CODE
#  include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Dlt_TpRxIndication()
 **********************************************************************************************************************/
/*! \brief       Notifies DLT that complete control message has been received.
 *  \details     Sets flag Dlt_RxInfo.RxIndicationOccured if control message has been successfully received.
 *  \param[in]   RxPduId          ID of DLT I-PDU that has been received. Identifies the data that has been received.
 *  \param[in]   Result           Result of the N-PDU reception:
 *                                 - E_OK      the complete N-PDU has been received.
 *                                 - E_NOT_OK  an error occurred during reception, used to enable unlocking of the
 *                                             receive buffer.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_TpRxIndication(PduIdType RxPduId, Std_ReturnType Result);

/**********************************************************************************************************************
 *  Dlt_TpTxConfirmation()
 **********************************************************************************************************************/
/*! \brief       Notifies DLT that message has been successfully transmitted.
 *  \details     Sets flag Dlt_MessageTransmitted if message has been successfully transmitted.
 *  \param[in]   TxPduId          ID of Dlt I-PDU that has been transmitted.
 *  \param[in]   Result           Result of the N-PDU transmission:
 *                                 - E_OK      the complete N-PDU has been transmitted.
 *                                 - E_NOT_OK  an error occurred during transmission, used to enable unlocking of the
 *                                             transmit buffer.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
FUNC(void, DLT_CODE) Dlt_TpTxConfirmation(PduIdType TxPduId, Std_ReturnType Result);

/**********************************************************************************************************************
 *  Dlt_StartOfReception()
 **********************************************************************************************************************/
/*! \brief       Notifies DLT about incoming message.
 *  \details     Gets the length of received buffer and checks if message fits in receive buffer.
 *  \param[in]   id               The PDU id.
 *  \param[in]   info             Pointer to a PduInfoType structure containing the payload data (without protocol
 *                                information) and payload length of the first frame or single frame of a transport
 *                                protocol I-PDU reception.
 *  \param[in]   TpSduLength      Total length of the N-SDU to be received.
 *  \param[out]  bufferSizePtr    Available receive buffer in the receiving module. This parameter will be used to
 *                                compute the Block Size (BS) in the transport protocol module.
 *  \return      BUFREQ_OK        Connection has been accepted. bufferSizePtr indicates the available receive buffer.
 *  \return      BUFREQ_E_NOT_OK  Connection has been rejected; reception is aborted. bufferSizePtr remains unchanged.
 *  \return      BUFREQ_E_OVFL    No buffer of the required length can be provided; reception is aborted. bufferSizePtr
 *                                remains unchanged.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *  \spec        requires info != NULL_PTR;
 *               requires bufferSizePtr != NULL_PTR;
 *               requires $external(bufferSizePtr);
 *  \endspec
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_StartOfReception(PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DLT_APPL_VAR) info,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) bufferSizePtr);

/**********************************************************************************************************************
 *  Dlt_CopyRxData()
 **********************************************************************************************************************/
/*! \brief       Provides the received data of an I-PDU segment (N-PDU) to the upper layer.
 *  \details     Copies received data to receive buffer and requests next bytes of message until complete message is
 *               received.
 *  \param[in]   id               The PDU id.
 *  \param[in]   info             Provides the source buffer (SduDataPtr) and the number of bytes to be copied
 *                                (SduLength).
 *  \param[out]  bufferSizePtr    Available receive buffer after data has been copied.
 *  \return      BUFREQ_OK        Data copied successfully.
 *  \return      BUFREQ_E_NOT_OK  Data was not copied because an error occurred.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires info != NULL_PTR;
 *               requires bufferSizePtr != NULL_PTR;
 *               requires $external(bufferSizePtr);
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_CopyRxData(PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DLT_APPL_VAR) info,
  P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) bufferSizePtr);

/**********************************************************************************************************************
 *  Dlt_CopyTxData()
 **********************************************************************************************************************/
/*! \brief       Acquires the transmit data of an I-PDU segment (N-PDU).
 *  \details     Copies as much Tx data to lower layer as requested. Fails if more data is requested then available.
 *               received.
 *  \param[in]   id               The PDU id.
 *  \param[in]   info             Provides the destination buffer (SduDataPtr) and the number of bytes to be copied
 *                                (SduLength).
 *  \param[in]   retry            This parameter is used to acknowledge transmitted data or to retransmit data after
 *                                transmission problems.
 *  \param[out]  availableDataPtr Indicates the remaining number of bytes that are available in the upper layer
 *                                module's Tx buffer.
 *  \return      BUFREQ_OK        Data has been copied to the transmit buffer completely as requested.
 *  \return      BUFREQ_E_BUSY    Request could not be fulfilled, because the required amount of Tx data is not
 *                                available.
 *  \return      BUFREQ_E_NOT_OK  Data has not been copied. Request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires info != NULL_PTR;
 *               requires $external(info->SduDataPtr);
 *               requires availableDataPtr != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_CopyTxData(PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DLT_APPL_VAR) info,
  P2VAR(RetryInfoType, AUTOMATIC, DLT_APPL_VAR) retry,
  P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) availableDataPtr);

#  define DLT_STOP_SEC_CODE
#  include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

#endif /* DLT_CBK_H */
