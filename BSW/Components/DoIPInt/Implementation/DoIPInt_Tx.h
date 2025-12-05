/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  DoIPInt_Tx.h
 *        \brief  Declarations for sub-module DoIPInt_Tx of component DoIPInt.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's main source file.
 *********************************************************************************************************************/

#if !defined (DOIPINT_TX_H)
# define DOIPINT_TX_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "DoIPInt_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define DOIPINT_START_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_Tx_Init()
 *********************************************************************************************************************/
/*! \brief        Initializes connection related variables used in DoIPInt_Tx sub-module.
 *  \details      -
 *  \param[in]    ConnectionIter          Connection iterator.
 *                                        [range: ConnectionIter < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-DoIPIntInitialization
 *  \spec
 *    requires ConnectionIter < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Tx_Init(
  DoIPInt_ConnectionIterType ConnectionIter);

/**********************************************************************************************************************
 *  DoIPInt_Tx_PrepareTransmit()
 *********************************************************************************************************************/
/*! \brief        Prepares a transmission by checking if the connection is active and by adding the transmission
 *                request to the transmission queue.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    HandleIdx               Handle index.
 *                                        [range: Diagnostic messages: HandleIdx < DoIPInt_GetSizeOfChannel()
 *                                                OEM specific messages: HandleIdx < DoIPInt_GetSizeOfOemPayloadTxBuf]
 *  \param[in]    Length                  Length of data to be transmitted.
 *  \param[in]    MsgHandlerIdx           Message handler index.
 *                                        [range: MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler()]
 *  \return       E_OK                    Connection is opened and transmit request added to queue.
 *  \return       E_NOT_OK                Connection is not opened or transmit request could not be added to queue.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Tx_PrepareTransmit(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 HandleIdx,
  PduLengthType Length,
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx);

/**********************************************************************************************************************
 *  DoIPInt_Tx_TransmitFromQueue()
 *********************************************************************************************************************/
/*! \brief        Gets the first transmission queue element and tries to request transmission at SoAd.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       E_OK                    Transmission request successful.
 *  \return       E_NOT_OK                Transmission request failed, already pending or queue is empty.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Tx_TransmitFromQueue(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Tx_ProcessTransmission()
 *********************************************************************************************************************/
/*! \brief        Processes the transmission queues of the connections.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Tx_ProcessTransmission(void);

/**********************************************************************************************************************
 *  DoIPInt_Tx_Terminate()
 *********************************************************************************************************************/
/*! \brief        Terminates pending transmissions on a connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Tx_Terminate(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyTxData()
 *********************************************************************************************************************/
/*! \brief        Copies data for transmission to provided buffer.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    PduInfoPtr              Provides the destination buffer and the number of bytes to be copied.
 *                                        [range: PduInfoPtr != NULL_PTR,
 *                                                PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: IN:  AvailableDataPtr != NULL_PTR
 *                                                OUT: undefined]
 *  \return       BUFREQ_OK               Data has been copied to the transmit buffer completely as requested.
 *  \return       BUFREQ_E_NOT_OK         Data has not been copied. Request failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyTxData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_Tx_Confirmation()
 *********************************************************************************************************************/
/*! \brief        Receives confirmation for transmitted PDU.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    Result                  Result of the transmission.
 *                                        [range: E_OK, E_NOT_OK]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Tx_Confirmation(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  Std_ReturnType Result);

/**********************************************************************************************************************
 *  DoIPInt_Tx_IsTransmissionPending()
 *********************************************************************************************************************/
/*! \brief        Forwards request to Tx Queue unit if a transmission is queued for the given connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       FALSE                   Queue does not contain a Tx request for the connection.
 *  \return       TRUE                    Queue contains a Tx request for the connection.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, DOIPINT_CODE) DoIPInt_Tx_IsTransmissionPending(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Tx_Cancel()
 *********************************************************************************************************************/
/*! \brief        Forwards transmission cancellation request to the SoAd.
 *  \details      -
 *  \param[in]    ChannelIdx              Channel index.
 *                                        [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
 *  \return       E_OK                    Request was accepted.
 *  \return       E_NOT_OK                Request was not accepted.
 *  \pre          -
 *  \context      TASK/ISR2
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \spec
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Tx_Cancel(
  DoIPInt_SizeOfChannelType ChannelIdx);

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DOIPINT_TX_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Tx.h
 *********************************************************************************************************************/
