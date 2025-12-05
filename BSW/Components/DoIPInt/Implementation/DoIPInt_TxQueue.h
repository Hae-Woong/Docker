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
/*!        \file  DoIPInt_TxQueue.h
 *        \brief  Declarations for sub-module DoIPInt_TxQueue of component DoIPInt.
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

#if !defined (DOIPINT_TXQUEUE_H)
# define DOIPINT_TXQUEUE_H

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
 *  DoIPInt_TxQueue_Init()
 *********************************************************************************************************************/
/*! \brief        Initializes connection related variables used in DoIPInt_TxQueue sub-module.
 *  \details      -
 *  \param[in]    ConnectionIter  Connection iterator.
 *                                [range: ConnectionIter < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-DoIPIntInitialization
 *  \spec
 *    requires ConnectionIter < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_TxQueue_Init(
  DoIPInt_ConnectionIterType ConnectionIter);

/**********************************************************************************************************************
 *  DoIPInt_TxQueue_Add()
 *********************************************************************************************************************/
/*! \brief        Adds a transmission request to the transmission queue on a connection.
 *  \details      -
 *  \param[in]    ConnectionIdx   Connection index.
 *                                [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    HandleIdx       Handle index.
 *                                [range: Diagnostic messages: HandleIdx < DoIPInt_GetSizeOfChannel()
 *                                        OEM specific messages: HandleIdx < DoIPInt_GetSizeOfOemPayloadTxBuf]
 *  \param[in]    Length          Length of data to be transmitted.
 *  \param[in]    MsgHandlerIdx   Message handler index.
 *                                [range: MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler()]
 *  \return       E_OK            Transmit request added to queue.
 *  \return       E_NOT_OK        Transmit request could not be added to queue.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TxQueue_Add(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 HandleIdx,
  PduLengthType Length,
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx);

/**********************************************************************************************************************
 *  DoIPInt_TxQueue_GetNext()
 *********************************************************************************************************************/
/*! \brief        Returns the first queue element of the transmission queue on a connection.
 *  \details      -
 *  \param[in]    ConnectionIdx    Connection index.
 *                                 [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[out]   HandleIdxPtr     Pointer to handle index.
 *                                 [range: IN:  HandleIdxPtr != NULL_PTR
 *                                         OUT: Diagnostic messages: HandleIdxPtr < DoIPInt_GetSizeOfChannel()
 *                                              OEM specific messages: HandleIdxPtr < DoIPInt_GetSizeOfOemPayloadTxBuf]
 *  \param[out]   LengthPtr        Pointer to length of data to be transmitted.
 *                                 [range: IN:  LengthPtr != NULL_PTR
 *                                         OUT: undefined]
 *  \param[out]   MsgHandlerIdxPtr Pointer to message handler index.
 *                                 [range: IN:  MsgHandlerIdxPtr != NULL_PTR
 *                                         OUT: MsgHandlerIdxPtr < DoIPInt_GetSizeOfMsgHandler()]
 *  \return       E_OK             Queue contains an element and output parameters are set.
 *  \return       E_NOT_OK         Request failed, output parameters have not been changed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires HandleIdxPtr != NULL_PTR;
 *    requires LengthPtr != NULL_PTR;
 *    requires MsgHandlerIdxPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TxQueue_GetNext(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(uint32, AUTOMATIC, DOIPINT_VAR) HandleIdxPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) LengthPtr,
  P2VAR(DoIPInt_SizeOfMsgHandlerType, AUTOMATIC, DOIPINT_VAR) MsgHandlerIdxPtr);

/**********************************************************************************************************************
 *  DoIPInt_TxQueue_Remove()
 *********************************************************************************************************************/
/*! \brief        Removes an element from the transmission queue on a connection.
 *  \details      -
 *  \param[in]    ConnectionIdx   Connection index.
 *                                [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    HandleIdx       Handle index.
 *                                [range: Diagnostic messages: HandleIdx < DoIPInt_GetSizeOfChannel()
 *                                        OEM specific messages: HandleIdx < DoIPInt_GetSizeOfOemPayloadTxBuf]
 *  \param[in]    MsgHandlerIdx   Message handler index.
 *                                [range: MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler()]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_TxQueue_Remove(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 HandleIdx,
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx);

/**********************************************************************************************************************
 *  DoIPInt_TxQueue_IsChannelInQueue()
 *********************************************************************************************************************/
/*! \brief        Determines if a tx request is queued for the given channel.
 *  \details      -
 *  \param[in]    ChannelIdx      Channel index.
 *                                [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
 *  \return       FALSE           Queue does not contain a tx request for the channel.
 *  \return       TRUE            Queue contains a tx request for the channel.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \spec
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, DOIPINT_CODE) DoIPInt_TxQueue_IsChannelInQueue(
  DoIPInt_SizeOfChannelType ChannelIdx);

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DOIPINT_TXQUEUE_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_TxQueue.h
 *********************************************************************************************************************/
