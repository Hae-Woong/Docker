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
/*!        \file  DoIPInt_LightHdr.h
 *        \brief  Declarations for sub-module DoIPInt_LightHdr of component DoIPInt.
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

#if !defined (DOIPINT_LIGHTHDR_H)
# define DOIPINT_LIGHTHDR_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "DoIPInt_Types.h"
# include "PduR_DoIPInt.h"

# if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT )
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  define DOIPINT_LIGHT_HDR_LEN                            (8u)       /*!< Length of header. */

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
#  define DOIPINT_START_SEC_CODE
#  include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_Init()
 *********************************************************************************************************************/
/*! \brief        Initializes connection related variables used in DoIPInt_LightHdr sub-module.
 *  \details      -
 *  \param[in]    ConnectionIter          Connection iterator.
 *                                        [range: ConnectionIter < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \trace        DSGN-DoIPIntInitialization
 *  \spec
 *    requires ConnectionIter < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_Init(
  DoIPInt_ConnectionIterType ConnectionIter);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxTpTransmit()
 *********************************************************************************************************************/
/*! \brief        Requests transmission of a TP diagnostic message on a specific channel.
 *  \details      -
 *  \param[in]    ChannelIdx              Channel index.
 *                                        [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
 *  \param[in]    Length                  Length of data to be transmitted.
 *  \return       E_OK                    Transmit request was accepted.
 *  \return       E_NOT_OK                Transmit request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same ChannelIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \spec
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxTpTransmit(
  DoIPInt_SizeOfChannelType ChannelIdx,
  PduLengthType Length);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCancel()
 *********************************************************************************************************************/
/*! \brief        Requests transmission cancellation on a specific channel.
 *  \details      -
 *  \param[in]    ChannelIdx              Channel index.
 *                                        [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
 *  \return       E_OK                    Request was accepted.
 *  \return       E_NOT_OK                Request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same ChannelIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \spec
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxCancel(
  DoIPInt_SizeOfChannelType ChannelIdx);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxCancel()
 *********************************************************************************************************************/
/*! \brief        Requests reception cancellation of a diagnostic message on a specific channel.
 *  \details      -
 *  \param[in]    ChannelIdx              Channel index.
 *                                        [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
 *  \return       E_OK                    Request was accepted.
 *  \return       E_NOT_OK                Request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same ChannelIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \spec
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_RxCancel(
  DoIPInt_SizeOfChannelType ChannelIdx);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxIfTransmit()
 *********************************************************************************************************************/
/*! \brief        Requests transmission of an IF diagnostic message on a specific channel.
 *  \details      -
 *  \param[in]    ChannelIdx              Channel index.
 *                                        [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
 *  \param[in]    PduInfoPtr              PDU information structure which contains the length and the data of the
 *                                        message.
 *  \return       E_OK                    Transmit request was accepted.
 *  \return       E_NOT_OK                Transmit request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same ChannelIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \spec
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *    requires PduInfoPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxIfTransmit(
  DoIPInt_SizeOfChannelType ChannelIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCopy()
 *********************************************************************************************************************/
/*! \brief        Copies diagnostic message with light header to provided buffer.
 *  \details      -
 *  \param[in]    ConnectionIdx         Connection index.
 *                                      [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    ChannelIdx            Channel index.
 *                                      [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
 *  \param[in]    UserDataLength        Length of user data of the current transmission.
 *  \param[in]    PduInfoPtr            Provides the destination buffer and the number of bytes to be copied. Is
 *                                      updated on return with the offset of copied data.
 *                                      [range: PduInfoPtr != NULL_PTR,
 *                                              PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                              PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]   AvailableDataPtr      Indicates the remaining number of bytes that are available to be copied.
 *                                      [range: AvailableDataPtr != NULL_PTR]
 *  \return       BUFREQ_OK             Data has been copied to the transmit buffer completely as requested.
 *  \return       BUFREQ_E_NOT_OK       Data has not been copied. Request failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfChannelType ChannelIdx,
  uint32 UserDataLength,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Forwards confirmation for transmitted PDU.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    ChannelIdx              Channel index.
 *                                        [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
 *  \param[in]    Result                  Result of the transmission.
 *                                        [range: E_OK, E_NOT_OK]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_TxConfirmation(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfChannelType ChannelIdx,
  Std_ReturnType Result);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxCopy()
 *********************************************************************************************************************/
/*! \brief        Copies received header and user data and validates it.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    PduInfoPtr                   Provides the buffer and the number of bytes to be copied.
 *                                             [range: PduInfoPtr != NULL_PTR,
 *                                                     PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                     PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]   AvailableDataPtr             Indicates the remaining number of bytes that are available to be copied.
 *                                             [range: AvailableDataPtr != NULL_PTR]
 *  \return       DOIPINT_MSG_HANDLER_PENDING  The message handler has not yet finished processing the message.
 *  \return       DOIPINT_MSG_HANDLER_FINISHED The message handler has finished processing the message.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Handling the message failed and the message shall be skipped.
 *  \return       DOIPINT_MSG_HANDLER_FAILED   Handling the message failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxStartParse()
 *********************************************************************************************************************/
/*! \brief        Starts parsing on the specified connection by stopping a current active parsing.
 *  \details      -
 *  \param[in]    ConnectionIdx         Connection index.
 *                                      [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[out]   AvailableDataPtr      Indicates the number of bytes that can be copied in the first call of
 *                                      DoIPInt_SoAdTpCopyRxData on reception of the next PDU.
 *                                      [range: AvailableDataPtr != NULL_PTR]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_RxStartParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxStopParse()
 *********************************************************************************************************************/
/*! \brief        Stops and initializes parsing on the specified connection.
 *  \details      -
 *  \param[in]    ConnectionIdx         Connection index.
 *                                      [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    RxTerminate           Indicates if termination is being handled.
 *                                      [range: TRUE, FALSE]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_RxStopParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  boolean RxTerminate);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxGetParsedPayloadLength()
 *********************************************************************************************************************/
/*! \brief        Gets the parsed payload length of the light header buffer of the connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *  \return       Parsed payload length.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint32, DOIPINT_CODE) DoIPInt_LightHdr_RxGetParsedPayloadLength(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxGetParsedData()
 *********************************************************************************************************************/
/*! \brief        Gets the parsed data of the connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    DestBufLength           Length of the DestBufPtr.
 *  \param[out]   DestBufPtr              Pointer to the buffer that shall be written.
 *                                        [range: DestBufPtr != NULL_PTR
 *                                                lengthOf(DestBufPtr) >= DOIPINT_LIGHT_HDR_LEN]
 *  \param[out]   MsgLengthPtr            Pointer to the length that shall be updated.
 *                                        [range: MsgLengthPtr != NULL_PTR]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires DestBufPtr != NULL_PTR;
 *    requires MsgLengthPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_RxGetParsedData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  PduLengthType DestBufLength,
  P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) DestBufPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) MsgLengthPtr);

#  define DOIPINT_STOP_SEC_CODE
#  include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT */
#endif /* DOIPINT_LIGHTHDR_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_LightHdr.h
 *********************************************************************************************************************/
