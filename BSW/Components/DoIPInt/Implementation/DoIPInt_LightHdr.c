/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  DoIPInt_LightHdr.c
 *        \brief  DoIPInt source file for sub-module DoIPInt_LightHdr.
 *      \details  Implementation for sub-module DoIPInt_LightHdr of component DoIPInt.
 *                Sub-module implements handling for transmission and reception context when light header is used.
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

#define DOIPINT_LIGHTHDR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "DoIPInt.h"
#include "DoIPInt_Tx.h"
#include "DoIPInt_Rx.h"
#include "DoIPInt_Util.h"
#include "DoIPInt_Priv.h"
#include "DoIPInt_LightHdr.h"
#include "DoIPInt_Event.h"
#include "PduR_DoIPInt.h"

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT )
/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#  define DOIPINT_LIGHT_HDR_OFFSET_LOG_ADDR                (0u)       /*!< Offset of logical addresses in header. */
#  define DOIPINT_LIGHT_HDR_OFFSET_TARGET_ADDR             (2u)       /*!< Offset of target address in header. */
#  define DOIPINT_LIGHT_HDR_OFFSET_LENGTH                  (4u)       /*!< Offset of payload length in header. */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# if !defined (DOIPINT_LOCAL)
#  define DOIPINT_LOCAL static
# endif /* !defined (DOIPINT_LOCAL) */

# if !defined (DOIPINT_LOCAL_INLINE)
#  define DOIPINT_LOCAL_INLINE LOCAL_INLINE
# endif /* !defined (DOIPINT_LOCAL_INLINE) */

# define DOIPINT_LIGHTHDR_RX_GET_PARSED_PAYLOAD_LENGTH(CONNECTION_IDX) \
  (IpBase_GetUint32(DoIPInt_GetAddrLightHdrParseBuf(DoIPInt_GetLightHdrParseBufStartIdxOfLightHdrCfg( \
          DoIPInt_GetLightHdrCfgIdxOfConnection((CONNECTION_IDX)))), \
      DOIPINT_LIGHT_HDR_OFFSET_LENGTH))

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Type definitions for DoIPInt_MsgHandlerRxStateType of LIGHT header unit */
/*!< Parsing not started. */
# define DOIPINT_LIGHT_HDR_RX_STATE_NONE                   (0u)
/*!< Copy up to length field. */
# define DOIPINT_LIGHT_HDR_RX_STATE_COPY_UP_TO_LENGTH      (1u)
/*!< Validate payload length. */
# define DOIPINT_LIGHT_HDR_RX_STATE_VALIDATE_LENGTH        (2u)
/*!< Copy up to the end of data that shall be buffered. */
# define DOIPINT_LIGHT_HDR_RX_STATE_COPY_UP_TO_END         (3u)
/*!< Find channel for validation of logical addresses. */
# define DOIPINT_LIGHT_HDR_RX_STATE_VALIDATE_LOG_ADDR      (4u)
/*!< Start reception of message. */
# define DOIPINT_LIGHT_HDR_RX_STATE_START_RECEPTION        (5u)
/*!< Forward the stored user data. */
# define DOIPINT_LIGHT_HDR_RX_STATE_FWD_STORED_USER_DATA   (6u)
/*!< Forward the remaining user data. */
# define DOIPINT_LIGHT_HDR_RX_STATE_FWD_USER_DATA          (7u)
/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define DOIPINT_START_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxAddBuf()
 *********************************************************************************************************************/
/*! \brief        Adds a transmission request to the transmission buffer on a channel.
 *  \details      -
 *  \param[in]    ChannelIdx              Channel index.
 *                                        [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
 *  \param[in]    PduInfoPtr              PDU information structure which contains the length and the data of the
 *                                        message.
 *                                        [range: PduInfoPtr != NULL_PTR,
 *                                                PduInfoPtr->SduLength != 0,
 *                                                PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr)]
 *  \return       E_OK                    Transmission data added to buffer.
 *  \return       E_NOT_OK                Transmission data could not be added to buffer.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \spec
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *    requires PduInfoPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxAddBuf(
  DoIPInt_SizeOfChannelType ChannelIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCopyHeader()
 *********************************************************************************************************************/
/*! \brief        Copies light message header.
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
 *  \param[out]   AvailableDataPtr      Indicates the remaining number of light header bytes that must be copied.
 *                                      [range: AvailableDataPtr != NULL_PTR]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_TxCopyHeader(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfChannelType ChannelIdx,
  uint32 UserDataLength,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCopyPayload()
 *********************************************************************************************************************/
/*! \brief        Copies diagnostic message payload and updates the offset and PDU information.
 *  \details      -
 *  \param[in]    ConnectionIdx         Connection index.
 *                                      [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    ChannelIdx            Channel index.
 *                                      [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
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
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxCopyPayload(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfChannelType ChannelIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCopyApiSpecificUserData()
 *********************************************************************************************************************/
/*! \brief        Copies diagnostic message payload according to connection API type.
 *  \details      -
 *  \param[in]    ConnectionIdx         Connection index.
 *                                      [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    ChannelIdx            Channel index.
 *                                      [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
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
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxCopyApiSpecificUserData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfChannelType ChannelIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCopyIfBuf()
 *********************************************************************************************************************/
/*! \brief        Copies the IF transmission buffer to the destination buffer.
 *  \details      -
 *  \param[in]    ChannelIdx              Channel index.
 *                                        [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
 *  \param[in]    Offset                  Offset of the already copied data.
 *  \param[in]    PduInfoPtr              Provides the destination buffer and the number of bytes to be copied.
 *                                        [range: PduInfoPtr != NULL_PTR,
 *                                                PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: AvailableDataPtr != NULL_PTR]
 *  \return       BUFREQ_OK               Data has been copied to the transmit buffer completely as requested.
 *  \return       BUFREQ_E_NOT_OK         Data has not been copied. Request failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \spec
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxCopyIfBuf(
  DoIPInt_SizeOfChannelType ChannelIdx,
  uint32 Offset,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxBufRelease()
 *********************************************************************************************************************/
/*! \brief        Releases a previously reserved transmission buffer on a channel.
 *  \details      -
 *  \param[in]    ChannelIdx              Channel index.
 *                                        [range: ChannelIdx < DoIPInt_GetSizeOfChannel()]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \spec
 *    requires ChannelIdx < DoIPInt_GetSizeOfChannel();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_TxBufRelease(
  DoIPInt_SizeOfChannelType ChannelIdx);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxParseCopyUpToLength()
 *********************************************************************************************************************/
/*! \brief         Copies header up to the header length.
 *  \details       -
 *  \param[in]     LightHdrCfgIdx              Light header configuration index.
 *                                             [range: LightHdrCfgIdx < DoIPInt_GetSizeOfLightHdrCfg()]
 *  \param[in,out] PduInfoPtr                  Provides the buffer and the number of bytes to be copied.
 *                                             [range: IN:   PduInfoPtr != NULL_PTR, PduInfoPtr->SduLength ==
 *                                                           lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                             PduInfoPtr->SduLength == 0 =>
 *                                                               PduInfoPtr->SduDataPtr == NULL_PTR
 *                                                     OUT:  PduInfoPtr->SduLength == 0 || unchanged]
 *  \param[out]    AvailableDataPtr            Indicates the remaining number of bytes that are available to be copied.
 *                                             [range: AvailableDataPtr != NULL_PTR]
 *  \return        DOIPINT_MSG_HANDLER_PENDING Copy process succeeded, message handling will be continued.
 *  \return        DOIPINT_MSG_HANDLER_FAILED  Number of bytes to be copied is invalid, message handling failed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires LightHdrCfgIdx < DoIPInt_GetSizeOfLightHdrCfg();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxParseCopyUpToLength(
  DoIPInt_SizeOfLightHdrCfgType LightHdrCfgIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxParseValidateLengthField()
 *********************************************************************************************************************/
/*! \brief        Validates the length field stored in the connection specific header buffer.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[out]   AvailableDataPtr             Indicates the remaining number of bytes that are available to be copied.
 *                                             [range: AvailableDataPtr != NULL_PTR]
 *  \return       DOIPINT_MSG_HANDLER_PENDING  Length is valid, message handling will be continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Length is invalid, message will be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxParseValidateLengthField(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxParseCopyUpToEnd()
 *********************************************************************************************************************/
/*! \brief         Copies user data up to the configured length.
 *  \details       -
 *  \param[in]     ConnectionIdx               Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in,out] PduInfoPtr                  Provides the buffer and the number of bytes to be copied.
 *                                             [range: IN:   PduInfoPtr != NULL_PTR, PduInfoPtr->SduLength ==
 *                                                           lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                             PduInfoPtr->SduLength == 0 =>
 *                                                               PduInfoPtr->SduDataPtr == NULL_PTR
 *                                                     OUT:  PduInfoPtr->SduLength == 0 || unchanged]
 *  \param[out]    AvailableDataPtr            Indicates the remaining number of bytes that are available to be copied.
 *                                             [range: AvailableDataPtr != NULL_PTR]
 *  \return        DOIPINT_MSG_HANDLER_PENDING Copy process succeeded, message handling will be continued.
 *  \return        DOIPINT_MSG_HANDLER_FAILED  Number of bytes to be copied is invalid, message handling failed.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxParseCopyUpToEnd(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxCopyStoredPayloadData()
 *********************************************************************************************************************/
/*! \brief        Copies the stored payload data.
 *  \details      -
 *  \param[in]    ConnectionIdx               Connection index.
 *                                            [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    PduInfoPtr                  Provides the buffer and the number of bytes to be copied.
 *                                            [range: PduInfoPtr != NULL_PTR,
 *                                                    PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                    PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]   AvailableDataPtr            Indicates the remaining number of bytes that are available to be copied.
 *                                            [range: AvailableDataPtr != NULL_PTR]
 *  \return       DOIPINT_MSG_HANDLER_PENDING Data has been copied completely as requested, message handling will be
 *                                            continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP    Data has not been copied, message will be skipped.
 *  \return       DOIPINT_MSG_HANDLER_FAILED  Number of bytes to be copied is invalid, message handling failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxCopyStoredPayloadData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxCopyStoredPayloadToPduR()
 *********************************************************************************************************************/
/*! \brief        Copies the stored payload data to the PduR.
 *  \details      -
 *  \param[in]    ConnectionIdx               Connection index.
 *                                            [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[out]   AvailableDataPtr            Indicates the remaining number of bytes that are available to be copied.
 *                                            [range: AvailableDataPtr != NULL_PTR]
 *  \return       DOIPINT_MSG_HANDLER_PENDING Copying data succeeded, message handling will be continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP    Copying data did not succeed, message will be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxCopyStoredPayloadToPduR(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxCopyPayloadData()
 *********************************************************************************************************************/
/*! \brief        Copies the received payload data.
 *  \details      -
 *  \param[in]    ConnectionIdx               Connection index.
 *                                            [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    PduInfoPtr                  Provides the buffer and the number of bytes to be copied.
 *                                            [range: PduInfoPtr != NULL_PTR,
 *                                                    PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                    PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]   AvailableDataPtr            Indicates the remaining number of bytes that are available to be copied.
 *                                            [range: AvailableDataPtr != NULL_PTR]
 *  \return       DOIPINT_MSG_HANDLER_PENDING Data has been copied completely as requested, message handling will be
 *                                            continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP    Data has not been copied, message will be skipped.
 *  \return       DOIPINT_MSG_HANDLER_FAILED  Number of bytes to be copied is invalid, message handling failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxCopyPayloadData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxStartReception()
 *********************************************************************************************************************/
/*! \brief        Starts reception to PduR.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    PduInfoPtr                   Buffer that is provided from SoAd or main function context.
 *                                             [range: PduInfoPtr != NULL_PTR]
 *  \param[out]   AvailableDataPtr             Depending on the return value, the AvailableDataPtr has different
 *                                             meanings.
 *                                             DOIPINT_MSG_HANDLER_PENDING: In case start of reception succeeded, the
 *                                               available data is set to zero to assert the available data is queried
 *                                               in the next step. In case start of reception did not succeed and the
 *                                               retry mechanism is still running, the available data is set to zero as
 *                                               well to assert that no data is copied from the lower layer.
 *                                             DOIPINT_MSG_HANDLER_SKIP: The available data is set to the user data
 *                                               length for skipping the data.
 *                                             DOIPINT_MSG_HANDLER_FAILED: The available data is not set and is ignored
 *                                               by lower layer.
 *                                             [range: AvailableDataPtr != NULL_PTR]
 *  \return       DOIPINT_MSG_HANDLER_PENDING  Message handling will be continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message will be skipped.
 *  \return       DOIPINT_MSG_HANDLER_FAILED   Message handling failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxStartReception(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxRetryStartReception()
 *********************************************************************************************************************/
/*! \brief        Retry handling for start of reception.
 *  \details      Retry timeout is started and checked and in case a timeout occurred an event is reported.
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    UserDataLength               User data length.
 *  \param[out]   AvailableDataPtr             Indicates the remaining number of bytes that are available to be copied.
 *                                             [range: AvailableDataPtr != NULL_PTR]
 *  \return       DOIPINT_MSG_HANDLER_PENDING  Message handling will be continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message will be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxRetryStartReception(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  PduLengthType UserDataLength,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxValidateLogAddrAndFindChannel()
 *********************************************************************************************************************/
/*! \brief        Validates the received logical addresses by finding a corresponding channel on a connection.
 *  \details      -
 *  \param[in]    ConnectionIdx               Connection index.
 *                                            [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[out]   AvailableDataPtr            Indicates the remaining number of bytes that are available to be copied.
 *                                            [range: AvailableDataPtr != NULL_PTR]
 *  \return       DOIPINT_MSG_HANDLER_PENDING Channel found, message handling will be continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP    No corresponding channel found, message will be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxValidateLogAddrAndFindChannel(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxAddBuf()
 *********************************************************************************************************************/
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
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxAddBuf(
  DoIPInt_SizeOfChannelType ChannelIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent buffer modifications from different context. */
  DOIPINT_BEGIN_CRITICAL_SECTION();

  /* #20 Ensure that the requested length does not exceed the buffer length. */
  if ( PduInfoPtr->SduLength <=
    ((PduLengthType)DoIPInt_GetIfTxBufEndIdxOfChannel(ChannelIdx) -
    (PduLengthType)DoIPInt_GetIfTxBufStartIdxOfChannel(ChannelIdx)) )
  {
    /* #30 Ensure that no transmission is yet pending for this channel. */
    if ( DoIPInt_GetTxMsgLenOfChannelDyn(ChannelIdx) == 0u )
    {
      /* #40 Set the message length to signalize a pending transmission. */
      DoIPInt_SetTxMsgLenOfChannelDyn(ChannelIdx, PduInfoPtr->SduLength);

      /* #50 Copy data to IF Tx buffer. */
      /* PRQA S 314 2 */ /* MD_MSR_VStdLibCopy */
      VStdLib_MemCpy(DOIPINT_A_P2VAR(void)DoIPInt_GetAddrIfTxBuf(DoIPInt_GetIfTxBufStartIdxOfChannel(ChannelIdx)),     /* VCA_DOIPINT_MEM_CPY_POINTER_LENGTH_CHECKED */
        DOIPINT_A_P2CONST(void)PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

      retVal = E_OK;
    }
  }

  /* #60 Leave critical section. */
  DOIPINT_END_CRITICAL_SECTION();

  return retVal;
} /* DoIPInt_LightHdr_TxAddBuf() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCopyHeader()
 *********************************************************************************************************************/
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
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_TxCopyHeader(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfChannelType ChannelIdx,
  uint32 UserDataLength,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
  uint32                        offset = DoIPInt_GetTxOffsetOfLightHdrCfgDyn(lightHdrCfgIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the offset is valid. */
  if ( offset < DOIPINT_LIGHT_HDR_LEN )
  {
    /* #20 Check if data shall be copied. */
    if ( PduInfoPtr->SduLength > 0u )
    {
      const uint32 logicalRemoteAddress = DoIPInt_GetLogicalRemoteAddressOfChannelDyn(ChannelIdx);
      const uint32 logicalLocalAddress = DoIPInt_GetLogicalLocalAddressOfChannelDyn(ChannelIdx);
      uint32       dataToBeCopied = DOIPINT_LIGHT_HDR_LEN - offset;
      uint8        hdrBuf[DOIPINT_LIGHT_HDR_LEN];

      /* #30 Calculate length to be copied depending on requested length and pending header segment. */
      if ( PduInfoPtr->SduLength < dataToBeCopied )
      {
        dataToBeCopied = PduInfoPtr->SduLength;
      }

      /* #40 Copy ISO header to local temporary buffer. */
      hdrBuf[0u] = (uint8)(logicalLocalAddress >> 8);
      hdrBuf[1u] = (uint8)(logicalLocalAddress);
      hdrBuf[2u] = (uint8)(logicalRemoteAddress >> 8);
      hdrBuf[3u] = (uint8)(logicalRemoteAddress);

      hdrBuf[4u] = (uint8)(UserDataLength >> 24);
      hdrBuf[5u] = (uint8)(UserDataLength >> 16);
      hdrBuf[6u] = (uint8)(UserDataLength >> 8);
      hdrBuf[7u] = (uint8)(UserDataLength);

      /* #50 Copy local buffer to provided buffer with specified offset. */
      /*@ assert $external(PduInfoPtr->SduDataPtr); */                                                                 /* VCA_DOIPINT_MEM_CPY_EXT_POINTER_LENGTH_VALID */
      /* PRQA S 314 1 */ /* MD_MSR_VStdLibCopy */
      VStdLib_MemCpy((void*)PduInfoPtr->SduDataPtr, (void*)&hdrBuf[offset], dataToBeCopied);                           /* VCA_DOIPINT_MEM_CPY_EXT_POINTER_LENGTH_VALID */

      /* #60 Update transmission offset. */
      offset += dataToBeCopied;
      DoIPInt_SetTxOffsetOfLightHdrCfgDyn(lightHdrCfgIdx, offset);

      /* #70 Update provided buffer struct. */
      PduInfoPtr->SduLength -= (PduLengthType)dataToBeCopied;
      PduInfoPtr->SduDataPtr = &PduInfoPtr->SduDataPtr[dataToBeCopied];
    }

    /* #80 Set the available data pointer to the remaining length of the light header that needs to be copied. */
    *AvailableDataPtr = (PduLengthType)(DOIPINT_LIGHT_HDR_LEN - offset);
  }
  /* #11 Return that no more header data is available to be copied otherwise. */
  else
  {
    *AvailableDataPtr = 0u;
  }
} /* DoIPInt_LightHdr_TxCopyHeader() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCopyPayload()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxCopyPayload(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfChannelType ChannelIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy data according to the API type and check if copying succeeds. */
  if ( DoIPInt_LightHdr_TxCopyApiSpecificUserData(ConnectionIdx, ChannelIdx, PduInfoPtr, AvailableDataPtr) ==
    BUFREQ_OK )
  {
    const DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
    const uint32                        offset = DoIPInt_GetTxOffsetOfLightHdrCfgDyn(lightHdrCfgIdx);

    /* #20 Update the offset. */
    DoIPInt_SetTxOffsetOfLightHdrCfgDyn(lightHdrCfgIdx, offset + PduInfoPtr->SduLength);

    /* #30 Update provided buffer struct. */
    PduInfoPtr->SduDataPtr = &PduInfoPtr->SduDataPtr[PduInfoPtr->SduLength];
    PduInfoPtr->SduLength = 0u;

    /* Ignore returned available buffer size (availableData) since it is requested later again. */

    retVal = BUFREQ_OK;
  }

  return retVal;
} /* DoIPInt_LightHdr_TxCopyPayload() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCopyApiSpecificUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxCopyApiSpecificUserData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfChannelType ChannelIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
  BufReq_ReturnType                   retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call PduR to copy data and check if call succeeds in case of TP-API type of channel. */
  if ( DoIPInt_GetApiTypeOfChannel(ChannelIdx) == DOIPINT_API_TYPE_TP )
  {
    retVal = PduR_DoIPIntTpCopyTxData(
      DoIPInt_GetPduRTxPduIdOfChannel(ChannelIdx),
      PduInfoPtr,
      NULL_PTR,
      AvailableDataPtr);
  }
  /* #11 Copy data from internal IF buffer otherwise. */
  else
  {
    const uint32 offset = DoIPInt_GetTxOffsetOfLightHdrCfgDyn(lightHdrCfgIdx);
    const uint32 userDataOffset = (offset >= DOIPINT_LIGHT_HDR_LEN) ? (offset - DOIPINT_LIGHT_HDR_LEN) : 0u;

    retVal = DoIPInt_LightHdr_TxCopyIfBuf(ChannelIdx, userDataOffset, PduInfoPtr, AvailableDataPtr);
  }

  return retVal;
} /* DoIPInt_LightHdr_TxCopyApiSpecificUserData() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCopyIfBuf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxCopyIfBuf(
  DoIPInt_SizeOfChannelType ChannelIdx,
  uint32 Offset,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const PduLengthType txMsgLen = DoIPInt_GetTxMsgLenOfChannelDyn(ChannelIdx);
  BufReq_ReturnType   retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Ensure that the user data shall be copied inside a valid range. */
  if ( (Offset + PduInfoPtr->SduLength) <= txMsgLen )
  {
    /* #20 Check if data shall be copied. */
    if ( PduInfoPtr->SduLength != 0u )
    {
      const PduLengthType ifBufIdx = (PduLengthType)(DoIPInt_GetIfTxBufStartIdxOfChannel(ChannelIdx) + Offset);

      /* #30 Copy data from internal buffer to target buffer. */
      /*@ assert $external(PduInfoPtr->SduDataPtr); */                                                                 /* VCA_DOIPINT_MEM_CPY_EXT_POINTER_LENGTH_VALID */
      /* PRQA S 314 2 */ /* MD_MSR_VStdLibCopy */
      VStdLib_MemCpy(DOIPINT_A_P2VAR(void)PduInfoPtr->SduDataPtr,                                                      /* VCA_DOIPINT_MEM_CPY_EXT_POINTER_LENGTH_VALID */
        DOIPINT_A_P2CONST(void)(DoIPInt_GetAddrIfTxBuf(ifBufIdx)),
        (uint32)PduInfoPtr->SduLength);
    }

    /* #40 Set available data length. */
    *AvailableDataPtr = (PduLengthType)(txMsgLen - (Offset + PduInfoPtr->SduLength));

    retVal = BUFREQ_OK;
  }

  return retVal;
} /* DoIPInt_LightHdr_TxCopyIfBuf() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxBufRelease()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_TxBufRelease(
  DoIPInt_SizeOfChannelType ChannelIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset the message length to signalize that no transmission is pending. */
  DoIPInt_SetTxMsgLenOfChannelDyn(ChannelIdx, 0u);
} /* DoIPInt_LightHdr_TxBufRelease() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxParseCopyUpToLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxParseCopyUpToLength(
  DoIPInt_SizeOfLightHdrCfgType LightHdrCfgIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                              offset = DoIPInt_GetRxOffsetParseOfLightHdrCfgDyn(LightHdrCfgIdx);
  DoIPInt_SizeOfLightHdrParseBufType parseBufStartIdx = DoIPInt_GetLightHdrParseBufStartIdxOfLightHdrCfg(
    LightHdrCfgIdx);
  DoIPInt_SizeOfLightHdrParseBufType parseBufLength = DoIPInt_GetLightHdrParseBufEndIdxOfLightHdrCfg(LightHdrCfgIdx) -
    parseBufStartIdx;
  DoIPInt_MsgHndl_RetType            retVal = DOIPINT_MSG_HANDLER_PENDING;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if data length to be parsed is valid. */
  if ( DoIPInt_Util_IsLengthInRange(PduInfoPtr->SduLength, DOIPINT_LIGHT_HDR_LEN, parseBufLength, offset) == E_OK )
  {
    P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) hdrBufPtr = DoIPInt_GetAddrLightHdrParseBuf(parseBufStartIdx + offset);

    /* #20 Copy data to local buffer and switch to next state if data up to length field has been copied. */
    if ( DoIPInt_Util_CopyUpToLevel(DOIPINT_LIGHT_HDR_LEN, &offset, hdrBufPtr, PduInfoPtr, AvailableDataPtr) == E_OK )
    {
      DoIPInt_SetRxStateOfLightHdrCfgDyn(LightHdrCfgIdx, DOIPINT_LIGHT_HDR_RX_STATE_VALIDATE_LENGTH);
    }

    /* #30 Indicate that data has been copied. */
    PduInfoPtr->SduLength = 0u;

    /* #40 Update offset for parsed header. */
    DoIPInt_SetRxOffsetParseOfLightHdrCfgDyn(LightHdrCfgIdx, offset);
  }
  /* #11 Indicate that parsing has failed otherwise. */
  else
  {
    retVal = DOIPINT_MSG_HANDLER_FAILED;
  }

  return retVal;
} /* DoIPInt_LightHdr_RxParseCopyUpToLength() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxParseValidateLengthField()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxParseValidateLengthField(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
  DoIPInt_MsgHndl_RetType       retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the length field from header. */
  const uint32 payloadLength = DOIPINT_LIGHTHDR_RX_GET_PARSED_PAYLOAD_LENGTH(ConnectionIdx);
  const uint32 supportedPduLength = DoIPInt_Util_GetSupportedPduLength(payloadLength);

  /* #20 Check if length field exceeds the maximum length. */
  if ( payloadLength > supportedPduLength )
  {
    /* #200 Set the available buffer size to the invalid payload length to skip the message. */
    *AvailableDataPtr = (PduLengthType)supportedPduLength;

    /* #210 Report event that message is too large. */
    retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_LIGHT);
  }
  /* #21 Assume length field is valid and switch to next state otherwise. */
  else
  {
    DoIPInt_SetRxStateOfLightHdrCfgDyn(lightHdrCfgIdx, DOIPINT_LIGHT_HDR_RX_STATE_COPY_UP_TO_END);
    retVal = DOIPINT_MSG_HANDLER_PENDING;
  }

  return retVal;
} /* DoIPInt_LightHdr_RxParseValidateLengthField() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxParseCopyUpToEnd()
 *********************************************************************************************************************/
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
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxParseCopyUpToEnd(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
  const uint32                  payloadLength = DOIPINT_LIGHTHDR_RX_GET_PARSED_PAYLOAD_LENGTH(ConnectionIdx);
  const uint32                  parseLength = DOIPINT_MIN(DoIPInt_GetEventMaxUserDataLength(), payloadLength);
  DoIPInt_MsgHndl_RetType       retVal = DOIPINT_MSG_HANDLER_PENDING;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if any user data needs to be buffered. */
  if ( parseLength > 0u )
  {
    uint8                              offset = DoIPInt_GetRxOffsetParseOfLightHdrCfgDyn(lightHdrCfgIdx);
    DoIPInt_SizeOfLightHdrParseBufType parseBufStartIdx = DoIPInt_GetLightHdrParseBufStartIdxOfLightHdrCfg(
      lightHdrCfgIdx);
    DoIPInt_SizeOfLightHdrParseBufType parseBufLength =
      DoIPInt_GetLightHdrParseBufEndIdxOfLightHdrCfg(lightHdrCfgIdx) - parseBufStartIdx;

    /* #20 Check if data length to be parsed is valid. */
    if ( DoIPInt_Util_IsLengthInRange(PduInfoPtr->SduLength, parseLength + DOIPINT_LIGHT_HDR_LEN, parseBufLength,
      offset) == E_OK )
    {
      P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) hdrBufPtr = DoIPInt_GetAddrLightHdrParseBuf(parseBufStartIdx + offset);

      /* #30 Copy data to local buffer and switch to next state if data up to configured length has been copied. */
      if ( DoIPInt_Util_CopyUpToLevel(parseLength + DOIPINT_LIGHT_HDR_LEN, &offset, hdrBufPtr,
          DOIPINT_A_P2CONST(PduInfoType)PduInfoPtr, AvailableDataPtr) == E_OK )
      {
        /* #300 Update offset for parsed data. */
        DoIPInt_SetRxOffsetParseOfLightHdrCfgDyn(lightHdrCfgIdx, offset);

        /* #301 Report event that data has been received completely up to the configured length. */
        retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_LIGHT);

        /* #302 Go to next state if message shall not be skipped. */
        if ( retVal != DOIPINT_MSG_HANDLER_SKIP )
        {
          DoIPInt_SetRxStateOfLightHdrCfgDyn(lightHdrCfgIdx, DOIPINT_LIGHT_HDR_RX_STATE_VALIDATE_LOG_ADDR);
        }
      }
      /* #31 Only update offset for parsed data otherwise. */
      else
      {
        DoIPInt_SetRxOffsetParseOfLightHdrCfgDyn(lightHdrCfgIdx, offset);
      }

      /* #40 Indicate that data has been copied. */
      PduInfoPtr->SduLength = 0u;
    }
    /* #21 Indicate that parsing has failed otherwise. */
    else
    {
      retVal = DOIPINT_MSG_HANDLER_FAILED;
    }
  }
  /* #11 Proceed with next state otherwise. */
  else
  {
    DoIPInt_SetRxStateOfLightHdrCfgDyn(lightHdrCfgIdx, DOIPINT_LIGHT_HDR_RX_STATE_VALIDATE_LOG_ADDR);

    /* #110 Report event that data has been received completely up to the configured length. */
    retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_LIGHT);
  }

  return retVal;
} /* DoIPInt_LightHdr_RxParseCopyUpToEnd() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxCopyStoredPayloadData()
 *********************************************************************************************************************/
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
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxCopyStoredPayloadData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
  DoIPInt_MsgHndl_RetType             retVal = DOIPINT_MSG_HANDLER_FAILED;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if not yet all buffered data has been copied. */
  if ( DoIPInt_GetRxOffsetForwardOfLightHdrCfgDyn(lightHdrCfgIdx) <
    DoIPInt_GetRxOffsetParseOfLightHdrCfgDyn(lightHdrCfgIdx) )
  {
    /* #20 Assert that no data from the lower layer remains to be copied since only buffered data is forwarded. */
    if ( PduInfoPtr->SduLength == 0u )
    {
      /* #30 Try to copy as much data to PduR as possible. */
      retVal = DoIPInt_LightHdr_RxCopyStoredPayloadToPduR(ConnectionIdx, AvailableDataPtr);

      /* #40 Check if copying data to PduR succeeded. */
      if ( retVal == DOIPINT_MSG_HANDLER_PENDING)
      {
        /* #400 Check if all buffered data has already been copied. */
        if ( DoIPInt_GetRxOffsetForwardOfLightHdrCfgDyn(lightHdrCfgIdx) >=
          DoIPInt_GetRxOffsetParseOfLightHdrCfgDyn(lightHdrCfgIdx) )
        {
          /* #4000 Deactivate main function handling of pending reception. */
          DoIPInt_Rx_ResetPending(ConnectionIdx);

          /* #4001 Set state to forward the remaining user data in the next step. */
          DoIPInt_SetRxStateOfLightHdrCfgDyn(lightHdrCfgIdx, DOIPINT_LIGHT_HDR_RX_STATE_FWD_USER_DATA);
        }
        /* #401 Assert that copying the buffered data is retried in main function context otherwise. */
        else
        {
          DoIPInt_Rx_SetPending(ConnectionIdx);
        }
      }
      /* #41 Terminate reception to PduR and skip message otherwise. */
      else
      {
        const DoIPInt_SizeOfChannelType channelIdx = DoIPInt_GetChannelRxIdxOfLightHdrCfgDyn(lightHdrCfgIdx);
        const uint64                    msgLength = (uint64)DOIPINT_LIGHTHDR_RX_GET_PARSED_PAYLOAD_LENGTH(
          ConnectionIdx) + DOIPINT_LIGHT_HDR_LEN;

        /* #410 Call reception indication of PduR and indicate error. */
        PduR_DoIPIntTpRxIndication(DoIPInt_GetPduRRxPduIdOfChannel(channelIdx), E_NOT_OK);

        /* #411 Set available buffer size to the remaining user data length. */
        *AvailableDataPtr = (PduLengthType)(msgLength - DoIPInt_GetRxOffsetForwardOfLightHdrCfgDyn(lightHdrCfgIdx));
      }
    }
  }
  /* #11 Set state to forward the remaining user data in the next step otherwise. */
  else
  {
    DoIPInt_SetRxStateOfLightHdrCfgDyn(lightHdrCfgIdx, DOIPINT_LIGHT_HDR_RX_STATE_FWD_USER_DATA);
    retVal = DOIPINT_MSG_HANDLER_PENDING;
  }

  return retVal;
} /* DoIPInt_LightHdr_RxCopyStoredPayloadData() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxCopyStoredPayloadToPduR()
 *********************************************************************************************************************/
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
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxCopyStoredPayloadToPduR(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
  const DoIPInt_SizeOfChannelType     channelIdx = DoIPInt_GetChannelRxIdxOfLightHdrCfgDyn(lightHdrCfgIdx);
  PduInfoType                         pduInfo = { NULL_PTR, 0u };
  uint64                              fwdOffset = DoIPInt_GetRxOffsetForwardOfLightHdrCfgDyn(lightHdrCfgIdx);
  const uint32                        parseOffset = DoIPInt_GetRxOffsetParseOfLightHdrCfgDyn(lightHdrCfgIdx);
  DoIPInt_MsgHndl_RetType             retVal = DOIPINT_MSG_HANDLER_PENDING;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy data to PduR until all buffered data is copied, no more data can be retrieved or copying data fails. */
  do
  {
    /* #20 Call PduR and check if copying user data succeeds. */
    if ( PduR_DoIPIntTpCopyRxData(
      DoIPInt_GetPduRRxPduIdOfChannel(channelIdx),
      &pduInfo, AvailableDataPtr) == BUFREQ_OK )
    {
      /* #200 Update offset. */
      fwdOffset += pduInfo.SduLength;

      /* #201 Continue copying the buffered data in case any data is remaining. */
      if ( fwdOffset < parseOffset )
      {
        /* #2010 Calculate the amount of buffered data that shall be copied in the next step. */
        if ( *AvailableDataPtr > (parseOffset - fwdOffset) )
        {
          pduInfo.SduLength = (PduLengthType)(parseOffset - fwdOffset);
        }
        else
        {
          pduInfo.SduLength = *AvailableDataPtr;
        }

        /* #2011 Set the pointer to the correct offset in the buffered data. */
        pduInfo.SduDataPtr = DoIPInt_GetAddrLightHdrParseBuf(DoIPInt_GetLightHdrParseBufStartIdxOfLightHdrCfg(
          lightHdrCfgIdx) + fwdOffset);
      }
      /* #202 Set available buffer size to value of PduR but at maximum to the remaining payload length otherwise. */
      else
      {
        const uint64 msgLength = (uint64)DOIPINT_LIGHTHDR_RX_GET_PARSED_PAYLOAD_LENGTH(ConnectionIdx) +
          DOIPINT_LIGHT_HDR_LEN;

        if ( *AvailableDataPtr > (PduLengthType)(msgLength - fwdOffset) )
        {
          *AvailableDataPtr = (PduLengthType)(msgLength - fwdOffset);
        }
      }
    }
    /* #21 Indicate to skip message otherwise. */
    else
    {
      retVal = DOIPINT_MSG_HANDLER_SKIP;
    }
  } while ( (retVal == DOIPINT_MSG_HANDLER_PENDING) && (fwdOffset < parseOffset) && (*AvailableDataPtr > 0u) );

  /* #30 Update the forwarded offset. */
  DoIPInt_SetRxOffsetForwardOfLightHdrCfgDyn(lightHdrCfgIdx, fwdOffset);

  return retVal;
} /* DoIPInt_LightHdr_RxCopyStoredPayloadToPduR() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxCopyPayloadData()
 *********************************************************************************************************************/
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
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxCopyPayloadData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
  uint64                              fwdOffset = DoIPInt_GetRxOffsetForwardOfLightHdrCfgDyn(lightHdrCfgIdx);
  const uint64                        msgLength = (uint64)DOIPINT_LIGHTHDR_RX_GET_PARSED_PAYLOAD_LENGTH(
    ConnectionIdx) + DOIPINT_LIGHT_HDR_LEN;
  DoIPInt_MsgHndl_RetType             retVal = DOIPINT_MSG_HANDLER_FAILED;
  BufReq_ReturnType                   localRetVal = BUFREQ_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if data to be copied does not exceed the pending user data length. */
  if ( (fwdOffset + PduInfoPtr->SduLength) <= msgLength )
  {
    const DoIPInt_SizeOfChannelType channelIdx = DoIPInt_GetChannelRxIdxOfLightHdrCfgDyn(lightHdrCfgIdx);

    /* #20 Call PduR to copy user data in case data is still pending and available length has not yet been queried. */
    if ( (fwdOffset != msgLength) && (*AvailableDataPtr == 0u) )
    {
      localRetVal = PduR_DoIPIntTpCopyRxData(DoIPInt_GetPduRRxPduIdOfChannel(channelIdx),
        PduInfoPtr,
        AvailableDataPtr);
    }

    /* #30 Check if call to PduR succeeded. */
    if ( localRetVal == BUFREQ_OK )
    {
      /* #300 Update offset. */
      fwdOffset += PduInfoPtr->SduLength;
      DoIPInt_SetRxOffsetForwardOfLightHdrCfgDyn(lightHdrCfgIdx, fwdOffset);

      /* #301 Finish reception if all data has been copied. */
      if ( fwdOffset == msgLength )
      {
        /* #3010 Call PduR to finish reception. */
        PduR_DoIPIntTpRxIndication(DoIPInt_GetPduRRxPduIdOfChannel(channelIdx), E_OK);

        /* #3011 Report event that message has been successfully forwarded to the user. */
        retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_DIAG_MSG_FORWARDED_LIGHT);
      }
      /* #302 Prepare to continue reception otherwise. */
      else
      {
        /* #3020 Set available buffer size to the value of PduR but at maximum to the remaining payload length. */
        if ( *AvailableDataPtr > (PduLengthType)(msgLength - fwdOffset) )
        {
          *AvailableDataPtr = (PduLengthType)(msgLength - fwdOffset);
        }

        /* #3021 Indicate that message handling is still pending. */
        retVal = DOIPINT_MSG_HANDLER_PENDING;
      }
    }
    /* #31 Terminate reception to PduR and skip message otherwise. */
    else
    {
      /* #310 Call reception indication of PduR and indicate error. */
      PduR_DoIPIntTpRxIndication(DoIPInt_GetPduRRxPduIdOfChannel(channelIdx), E_NOT_OK);

      /* #311 Set available buffer size to the remaining user data length. */
      *AvailableDataPtr = (PduLengthType)(msgLength - fwdOffset);

      /* #312 Indicate that message shall be skipped. */
      retVal = DOIPINT_MSG_HANDLER_SKIP;
    }
  }

  return retVal;
} /* DoIPInt_LightHdr_RxCopyPayloadData() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxStartReception()
 *********************************************************************************************************************/
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
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxStartReception(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal = DOIPINT_MSG_HANDLER_FAILED;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Assert that no data from the lower layer remains to be copied since only buffered data is forwarded. */
  if ( PduInfoPtr->SduLength == 0u )
  {
    const DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
    const DoIPInt_SizeOfChannelType     channelIdx = DoIPInt_GetChannelRxIdxOfLightHdrCfgDyn(lightHdrCfgIdx);
    const uint32                        userDataLength = DOIPINT_LIGHTHDR_RX_GET_PARSED_PAYLOAD_LENGTH(ConnectionIdx);
    PduLengthType                       availableData = 0u;

    /* #100 Call PduR and check if it accepts reception. */
    if ( PduR_DoIPIntTpStartOfReception(DoIPInt_GetPduRRxPduIdOfChannel(channelIdx),                                   /* VCA_DOIPINT_CALL_EXTERNAL_FUNCTION_VAR_POINTER_ARGUMENT */
      NULL_PTR,
      userDataLength,
      &availableData) == BUFREQ_OK )
    {
      /* #1000 Reset the retry timeout. */
      DoIPInt_Rx_ResetRetryTimeout(ConnectionIdx);

      /* #1001 Switch to next state to signalize that payload data can now be forwarded. */
      DoIPInt_SetRxStateOfLightHdrCfgDyn(lightHdrCfgIdx, DOIPINT_LIGHT_HDR_RX_STATE_FWD_STORED_USER_DATA);

      /* #1002 Set available buffer size to zero to assert that the available data is queried in the next step. */
      *AvailableDataPtr = 0u;

      retVal = DOIPINT_MSG_HANDLER_PENDING;
    }
    /* #101 Handle retry if reception has been rejected by PduR. */
    else
    {
      retVal = DoIPInt_LightHdr_RxRetryStartReception(ConnectionIdx, userDataLength, AvailableDataPtr);
    }
  }

  return retVal;
} /* DoIPInt_LightHdr_RxStartReception() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxRetryStartReception()
 *********************************************************************************************************************/
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
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxRetryStartReception(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  PduLengthType UserDataLength,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal = DOIPINT_MSG_HANDLER_PENDING;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check timeout monitoring and skip message if timeout has expired. */
  if ( DoIPInt_Rx_CheckOrStartRetryTimeout(ConnectionIdx) == E_NOT_OK )
  {
    /* #100 Report event that message is discarded since retry limit has exceeded. */
    retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_DIAG_MSG_DISCARDED_LIGHT);

    /* #101 Deactivate main function handling of pending reception. */
    DoIPInt_Rx_ResetPending(ConnectionIdx);

    /* #102 Set available buffer size to the user data length. */
    *AvailableDataPtr = UserDataLength;
  }
  /* #11 Continue to retry start of reception otherwise. */
  else
  {
    /* #110 Assert that starting reception is retried in main function context. */
    DoIPInt_Rx_SetPending(ConnectionIdx);

    /* #111 Set available buffer size to indicate that no more data can be received at the moment. */
    *AvailableDataPtr = 0u;
  }

  return retVal;
} /* DoIPInt_LightHdr_RxRetryStartReception() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxValidateLogAddrAndFindChannel()
 *********************************************************************************************************************/
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
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxValidateLogAddrAndFindChannel(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_ChannelIndIterType             channelIndIter;
  DoIPInt_SizeOfLightHdrCfgType          lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
  P2CONST(uint8, AUTOMATIC, DOIPINT_VAR) hdrBufPtr = DoIPInt_GetAddrLightHdrParseBuf(
    DoIPInt_GetLightHdrParseBufStartIdxOfLightHdrCfg(lightHdrCfgIdx));
  DoIpInt_LogAddrFindingLevelType        findingLevel = DOIPINT_FIND_LVL_NONE;
  DoIPInt_MsgHndl_RetType                retVal = DOIPINT_MSG_HANDLER_SKIP;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the logical addresses from the received buffer. */
  const uint16 logicalRemoteAddress = IpBase_GetUint16(hdrBufPtr, DOIPINT_LIGHT_HDR_OFFSET_LOG_ADDR);
  const uint16 logicalLocalAddress = IpBase_GetUint16(hdrBufPtr, DOIPINT_LIGHT_HDR_OFFSET_TARGET_ADDR);

  /* #20 Read the user data length from the received buffer. */
  const uint32 userDataLength = IpBase_GetUint32(hdrBufPtr, DOIPINT_LIGHT_HDR_OFFSET_LENGTH);

  /* #30 Iterate over all channels related to the connection. */
  for ( channelIndIter = DoIPInt_GetChannelIndStartIdxOfConnection(ConnectionIdx);
    channelIndIter < DoIPInt_GetChannelIndEndIdxOfConnection(ConnectionIdx);
    channelIndIter++ )
  {
    const DoIPInt_SizeOfChannelType channelIdx = DoIPInt_GetChannelInd(channelIndIter);

    /* #40 Check if an Rx PDU is configured on the channel. */
    if ( DoIPInt_GetPduRRxPduIdOfChannel(channelIdx) != DOIPINT_NO_PDURRXPDUIDOFCHANNEL )
    {
      /* #50 Check if the channel's logical remote address matches the received logical remote address. */
      if ( DoIPInt_GetLogicalRemoteAddressOfChannelDyn(channelIdx) == logicalRemoteAddress )
      {
        findingLevel = DOIPINT_FIND_LVL_REMOTE;

        /* #60 Check if the channel's logical local address matches the received logical local address. */
        if ( DoIPInt_GetLogicalLocalAddressOfChannelDyn(channelIdx) == logicalLocalAddress )
        {
          /* #70 Set the channel on the light header message configuration. */
          DoIPInt_SetChannelRxIdxOfLightHdrCfgDyn(lightHdrCfgIdx, channelIdx);
          DoIPInt_SetRxStateOfLightHdrCfgDyn(lightHdrCfgIdx, DOIPINT_LIGHT_HDR_RX_STATE_START_RECEPTION);
          retVal = DOIPINT_MSG_HANDLER_PENDING;
          findingLevel = DOIPINT_FIND_LVL_REMOTE_AND_LOCAL;
          break;
        }
      }
    }
  }

  /* #80 Check if no valid combination of remote and local address was found. */
  if ( findingLevel != DOIPINT_FIND_LVL_REMOTE_AND_LOCAL )
  {
    /* #90 Set available buffer size to the remaining user data length. */
    *AvailableDataPtr = (PduLengthType)(userDataLength + DOIPINT_LIGHT_HDR_LEN -
      DoIPInt_GetRxOffsetParseOfLightHdrCfgDyn(lightHdrCfgIdx));

    /* #100 Report event of invalid remote address if no matching remote address was found. */
    if ( findingLevel == DOIPINT_FIND_LVL_NONE )
    {
      retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_REM_ADDR_LIGHT);
    }
    /* #110 Report event of invalid local address otherwise. */
    else
    {
      retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_LOCAL_ADDR_LIGHT);
    }
  }

  return retVal;
} /* DoIPInt_LightHdr_RxValidateLogAddrAndFindChannel() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DoIPInt_LightHdr_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_Init(
  DoIPInt_ConnectionIterType ConnectionIter)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_ChannelIndIterType          channelIndIter;
  const DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIter);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all channels related to the connection. */
  for ( channelIndIter = DoIPInt_GetChannelIndStartIdxOfConnection(ConnectionIter);
    channelIndIter < DoIPInt_GetChannelIndEndIdxOfConnection(ConnectionIter);
    channelIndIter++ )
  {
    const DoIPInt_SizeOfChannelType channelIdx = DoIPInt_GetChannelInd(channelIndIter);

    /* #20 Reset the message length to signalize that no transmission is pending. */
    /*@ assert channelIdx < DoIPInt_GetSizeOfChannel(); */                                                             /* VCA_DOIPINT_1_N_IND_CSL03 */
    DoIPInt_SetTxMsgLenOfChannelDyn(channelIdx, 0u);
  }

  /* #30 Initialize the light header transmission offset. */
  DoIPInt_SetTxOffsetOfLightHdrCfgDyn(lightHdrCfgIdx, 0u);

  /* #40 Initialize the active channel Rx index. */
  DoIPInt_SetChannelRxIdxOfLightHdrCfgDyn(lightHdrCfgIdx, DoIPInt_GetSizeOfChannel());

  /* #50 Initialize the light header parse state and offset. */
  DoIPInt_LightHdr_RxStopParse((DoIPInt_SizeOfConnectionType)ConnectionIter, FALSE);
} /* DoIPInt_LightHdr_Init() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxTpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxTpTransmit(
  DoIPInt_SizeOfChannelType ChannelIdx,
  PduLengthType Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfConnectionType connectionIdx = DoIPInt_GetConnectionIdxOfChannel(ChannelIdx);
  Std_ReturnType                     retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if transmission can be prepared (connection is open and queue element is available). */
  if ( DoIPInt_Tx_PrepareTransmit(connectionIdx, ChannelIdx, Length, DoIPInt_GetMsgHandlerIdxOfLightMsgHandler(0u)) ==
    E_OK )
  {
    /* #20 Try to transmit from queue. */
    (void)DoIPInt_Tx_TransmitFromQueue(connectionIdx);
    retVal = E_OK;
  }

  return retVal;
} /* DoIPInt_LightHdr_TxTpTransmit() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxCancel(
  DoIPInt_SizeOfChannelType ChannelIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward cancellation to transmission unit. */
  retVal = DoIPInt_Tx_Cancel(ChannelIdx);

  return retVal;
} /* DoIPInt_LightHdr_TxCancel() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxCancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_RxCancel(
  DoIPInt_SizeOfChannelType ChannelIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfConnectionType connectionIdx = DoIPInt_GetConnectionIdxOfChannel(ChannelIdx);
  Std_ReturnType                     retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if reception is pending on the connection of the requested channel. */
  if ( (DoIPInt_SizeOfChannelType)DoIPInt_GetRxStateOfConnectionDyn(connectionIdx) == DOIPINT_RX_STATE_LIGHT_HDR )
  {
    /* #20 Check if reception is pending for the requested channel. */
    if ( DoIPInt_GetChannelRxIdxOfLightHdrCfgDyn(DoIPInt_GetLightHdrCfgIdxOfConnection(connectionIdx)) == ChannelIdx )
    {
      /* #30 Forward cancellation to SoAd. */
      retVal = DoIPInt_Rx_Cancel(connectionIdx);
    }
  }

  return retVal;
} /* DoIPInt_LightHdr_RxCancel() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxIfTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxIfTransmit(
  DoIPInt_SizeOfChannelType ChannelIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to add the transmission request to the transmission buffer. */
  if ( DoIPInt_LightHdr_TxAddBuf(ChannelIdx, PduInfoPtr) == E_OK )
  {
    const DoIPInt_SizeOfConnectionType connectionIdx = DoIPInt_GetConnectionIdxOfChannel(ChannelIdx);

    /* #20 Check if transmission can be prepared (connection is open and queue element is available). */
    if ( DoIPInt_Tx_PrepareTransmit(connectionIdx, ChannelIdx, PduInfoPtr->SduLength,
        DoIPInt_GetMsgHandlerIdxOfLightMsgHandler(0u)) == E_OK )
    {
      /* #30 Try to transmit from queue in case adding the transmission request to the queue was successful. */
      (void)DoIPInt_Tx_TransmitFromQueue(connectionIdx);
      retVal = E_OK;
    }
    else
    {
      /* #31 Release the buffer otherwise. */
      DoIPInt_LightHdr_TxBufRelease(ChannelIdx);
    }
  }

  return retVal;
} /* DoIPInt_LightHdr_TxIfTransmit() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_LightHdr_TxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfChannelType ChannelIdx,
  uint32 UserDataLength,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType     availableDataQueried = 0u;
  PduLengthType     availableDataLightHeader = 0u;
  BufReq_ReturnType retVal = BUFREQ_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy the light header to the buffer. */
  DoIPInt_LightHdr_TxCopyHeader(ConnectionIdx, ChannelIdx, UserDataLength, PduInfoPtr, &availableDataLightHeader);

  /* #20 Forward call to copy the payload data or to query the available data. */
  retVal = DoIPInt_LightHdr_TxCopyPayload(ConnectionIdx, ChannelIdx, PduInfoPtr, &availableDataQueried);

  /* #30 Update the available data considering the queried length and the remaining light message header. */
  *AvailableDataPtr = availableDataLightHeader + availableDataQueried;

  return retVal;
} /* DoIPInt_LightHdr_TxCopy() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_TxConfirmation()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_TxConfirmation(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfChannelType ChannelIdx,
  Std_ReturnType Result)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
  const PduIdType                     txConfPduId = DoIPInt_GetPduRTxPduIdOfChannel(ChannelIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if IF-API is used for the channel. */
  if ( DoIPInt_GetApiTypeOfChannel(ChannelIdx) == DOIPINT_API_TYPE_IF )
  {
    /* #100 Release the transmission buffer. */
    DoIPInt_LightHdr_TxBufRelease(ChannelIdx);

#if ( DOIPINT_PDUR_IF_TXCONF_ENABLED == STD_ON )
    /* #101 Forward TxConfirmation to PduR in case it is enabled for the IF channel. */
    if ( txConfPduId != DOIPINT_NO_PDURTXPDUIDOFCHANNEL )
    {
      PduR_DoIPIntIfTxConfirmation(txConfPduId);
    }
#endif /* DOIPINT_PDUR_IF_TXCONF_ENABLED == STD_ON */
  }
  /* #11 Forward TxConfirmation to PduR otherwise. */
  else
  {
    PduR_DoIPIntTpTxConfirmation(txConfPduId, Result);
  }

  /* #20 Initialize the light header transmission offset. */
  DoIPInt_SetTxOffsetOfLightHdrCfgDyn(lightHdrCfgIdx, 0u);
} /* DoIPInt_LightHdr_TxConfirmation() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxCopy()
 *********************************************************************************************************************/
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
 */
FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_LightHdr_RxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHandlerRxStateType lastMsgHandlerRxState;
  DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
  DoIPInt_MsgHndl_RetType       retVal = DOIPINT_MSG_HANDLER_FAILED;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Process state machine until segment to be parsed is handled completely (i.e. no state transition anymore). */
  do
  {
    lastMsgHandlerRxState = DoIPInt_GetRxStateOfLightHdrCfgDyn(lightHdrCfgIdx);

    /* #20 Handle parse process depending on state. */
    switch ( lastMsgHandlerRxState )
    {
      /* #30 Copy the header to local buffer up to the length field. */
      case DOIPINT_LIGHT_HDR_RX_STATE_COPY_UP_TO_LENGTH:
      {
        retVal = DoIPInt_LightHdr_RxParseCopyUpToLength(lightHdrCfgIdx, PduInfoPtr, AvailableDataPtr);
        break;
      }
      /* #40 Validate length field. */
      case DOIPINT_LIGHT_HDR_RX_STATE_VALIDATE_LENGTH:
      {
        retVal = DoIPInt_LightHdr_RxParseValidateLengthField(ConnectionIdx, AvailableDataPtr);
        break;
      }
      /* #50 Copy up to the configured length of data that shall be buffered. */
      case DOIPINT_LIGHT_HDR_RX_STATE_COPY_UP_TO_END:
      {
        retVal = DoIPInt_LightHdr_RxParseCopyUpToEnd(ConnectionIdx, PduInfoPtr, AvailableDataPtr);
        break;
      }
      /* #60 Validate the logical addresses from header by finding a suiting channel. */
      case DOIPINT_LIGHT_HDR_RX_STATE_VALIDATE_LOG_ADDR:
      {
        retVal = DoIPInt_LightHdr_RxValidateLogAddrAndFindChannel(ConnectionIdx, AvailableDataPtr);
        break;
      }
      /* #70 Start reception to PduR. */
      case DOIPINT_LIGHT_HDR_RX_STATE_START_RECEPTION:
      {
        retVal = DoIPInt_LightHdr_RxStartReception(ConnectionIdx, PduInfoPtr, AvailableDataPtr);
        break;
      }
      /* #80 Forward the stored payload data. */
      case DOIPINT_LIGHT_HDR_RX_STATE_FWD_STORED_USER_DATA:
      {
        retVal = DoIPInt_LightHdr_RxCopyStoredPayloadData(ConnectionIdx, PduInfoPtr, AvailableDataPtr);
        break;
      }
      /* #90 Forward the remaining payload data. */
      case DOIPINT_LIGHT_HDR_RX_STATE_FWD_USER_DATA:
      {
        retVal = DoIPInt_LightHdr_RxCopyPayloadData(ConnectionIdx, PduInfoPtr, AvailableDataPtr);
        break;
      }
      /* #100 Return error if parsing is not active. */
      default: /* lastMsgRxState == DOIPINT_LIGHT_HDR_RX_STATE_NONE */
      {
        retVal = DOIPINT_MSG_HANDLER_FAILED;
        break;
      }
    }
  } while ( DoIPInt_GetRxStateOfLightHdrCfgDyn(lightHdrCfgIdx) != lastMsgHandlerRxState );

  return retVal;
} /* DoIPInt_LightHdr_RxCopy() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxStartParse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_RxStartParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Stop parsing. */
  DoIPInt_LightHdr_RxStopParse(ConnectionIdx, FALSE);

  /* #20 Set state to indicate that parsing has been started. */
  DoIPInt_SetRxStateOfLightHdrCfgDyn(DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx),
    DOIPINT_LIGHT_HDR_RX_STATE_COPY_UP_TO_LENGTH);

  /* #30 Set the available buffer size to the size of the header. */
  *AvailableDataPtr = DOIPINT_LIGHT_HDR_LEN;
} /* DoIPInt_LightHdr_RxStartParse() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxStopParse()
 *********************************************************************************************************************/
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
 */
FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_RxStopParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  boolean RxTerminate)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the current state of reception for termination if required. */
  if ( RxTerminate == TRUE )
  {
    switch ( DoIPInt_GetRxStateOfLightHdrCfgDyn(lightHdrCfgIdx) )
    {
      /* #100 Call PduR to finish reception in case of an ongoing reception. */
      case DOIPINT_LIGHT_HDR_RX_STATE_FWD_STORED_USER_DATA:
      case DOIPINT_LIGHT_HDR_RX_STATE_FWD_USER_DATA:
      {
        const DoIPInt_SizeOfChannelType channelIdx = DoIPInt_GetChannelRxIdxOfLightHdrCfgDyn(lightHdrCfgIdx);

        PduR_DoIPIntTpRxIndication(DoIPInt_GetPduRRxPduIdOfChannel(channelIdx), E_NOT_OK);
        break;
      }
      default: /* DOIPINT_LIGHT_HDR_RX_STATE_COPY_UP_TO_LENGTH, DOIPINT_LIGHT_HDR_RX_STATE_VALIDATE_LENGTH,
                  DOIPINT_LIGHT_HDR_RX_STATE_COPY_UP_TO_END, DOIPINT_LIGHT_HDR_RX_STATE_VALIDATE_LOG_ADDR,
                  DOIPINT_LIGHT_HDR_RX_STATE_START_RECEPTION, DOIPINT_LIGHT_HDR_RX_STATE_NONE */
      {
        /* Nothing to do. */
        break;
      }
    }
  }
  /* #20 Reset offset for parsing. */
  DoIPInt_SetRxOffsetParseOfLightHdrCfgDyn(lightHdrCfgIdx, 0u);

  /* #30 Reset offset for data forwarding. */
  DoIPInt_SetRxOffsetForwardOfLightHdrCfgDyn(lightHdrCfgIdx, DOIPINT_LIGHT_HDR_LEN);

  /* #40 Reset state. */
  DoIPInt_SetRxStateOfLightHdrCfgDyn(DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx),
    DOIPINT_LIGHT_HDR_RX_STATE_NONE);

  /* #50 Reset the retry timeout. */
  DoIPInt_Rx_ResetRetryTimeout(ConnectionIdx);

  /* #60 Indicate that no reception is pending. */
  DoIPInt_Rx_ResetPending(ConnectionIdx);
} /* DoIPInt_LightHdr_RxStopParse() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxGetParsedPayloadLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint32, DOIPINT_CODE) DoIPInt_LightHdr_RxGetParsedPayloadLength(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get parsed payload length and return it. */
  const uint32 payloadLength = DOIPINT_LIGHTHDR_RX_GET_PARSED_PAYLOAD_LENGTH(ConnectionIdx);

  return payloadLength;
} /* DoIPInt_LightHdr_RxGetParsedPayloadLength() */

/**********************************************************************************************************************
 *  DoIPInt_LightHdr_RxGetParsedData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_LightHdr_RxGetParsedData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  PduLengthType DestBufLength,
  P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) DestBufPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) MsgLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfLightHdrCfgType lightHdrCfgIdx = DoIPInt_GetLightHdrCfgIdxOfConnection(ConnectionIdx);
  const uint8                         parseOffset = DoIPInt_GetRxOffsetParseOfLightHdrCfgDyn(lightHdrCfgIdx);
  PduLengthType                       lengthToCopy = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Limit the length of the data to be copied to the length of the destination buffer. */
  if ( parseOffset > DestBufLength )
  {
    lengthToCopy = DestBufLength;
  }
  else
  {
    lengthToCopy = parseOffset;
  }

  /* #20 Get the parsed data. */
  /* PRQA S 314 2 */ /* MD_MSR_VStdLibCopy */
  VStdLib_MemCpy((void*)DestBufPtr,                                                                                    /* VCA_DOIPINT_MEM_CPY_POINTER_LENGTH_CHECKED */
    DOIPINT_A_P2VAR(void)DoIPInt_GetAddrLightHdrParseBuf(DoIPInt_GetLightHdrParseBufStartIdxOfLightHdrCfg(
        lightHdrCfgIdx)),
    lengthToCopy);

  *MsgLengthPtr = lengthToCopy;
} /* DoIPInt_LightHdr_RxGetParsedData() */

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_LightHdr.c
 *********************************************************************************************************************/
