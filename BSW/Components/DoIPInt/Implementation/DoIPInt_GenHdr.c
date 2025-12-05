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
/*!        \file  DoIPInt_GenHdr.c
 *        \brief  DoIPInt source file for sub-module DoIPInt_GenHdr.
 *      \details  Implementation for sub-module DoIPInt_GenHdr of component DoIPInt.
 *                Sub-module implements handling of generic header for transmission and reception context.
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

#define DOIPINT_GENHDR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "DoIPInt.h"
#include "DoIPInt_Priv.h"
#include "DoIPInt_GenHdr.h"
#include "DoIPInt_Util.h"
#include "DoIPInt_Event.h"

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*!< Offset of protocol in header. */
#  define DOIPINT_GEN_HDR_OFFSET_PROTOCOL               (0u)
/*!< Offset of inverse protocol in header. */
#  define DOIPINT_GEN_HDR_OFFSET_INV_PROTOCOL           (1u)
/*!< Offset of payload type in header. */
#  define DOIPINT_GEN_HDR_OFFSET_TYPE                   (2u)

/*!< Offset of payload length in header. */
#  define DOIPINT_GEN_HDR_OFFSET_LENGTH                 (4u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# if !defined (DOIPINT_LOCAL)
#  define DOIPINT_LOCAL static
# endif /* !defined (DOIPINT_LOCAL) */

# if !defined (DOIPINT_LOCAL_INLINE)
#  define DOIPINT_LOCAL_INLINE LOCAL_INLINE
# endif /* !defined (DOIPINT_LOCAL_INLINE) */

# define DOIPINT_GENHDR_RX_GET_PARSED_PAYLOAD_LENGTH(CONNECTION_IDX) \
  (IpBase_GetUint32(DoIPInt_GetAddrGenHdrParseBuf( \
        DoIPInt_GetGenHdrParseBufStartIdxOfGenHdrCfg(DoIPInt_GetGenHdrCfgIdxOfConnection((CONNECTION_IDX)))), \
      DOIPINT_GEN_HDR_OFFSET_LENGTH))

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Type definitions for DoIPInt_MsgHandlerRxStateType of generic header unit */
# define DOIPINT_GEN_HDR_RX_STATE_NONE                  (0u)  /*!< Parsing not started. */
# define DOIPINT_GEN_HDR_RX_STATE_COPY_UP_TO_LENGTH     (1u)  /*!< Copy up to length field. */
# define DOIPINT_GEN_HDR_RX_STATE_VALIDATE_HDR          (2u)  /*!< Validate generic header structure. */
# define DOIPINT_GEN_HDR_RX_STATE_VALIDATE_LENGTH       (3u)  /*!< Validate payload length. */

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
 *  DoIPInt_GenHdr_RxParseCopyUpToLength()
 *********************************************************************************************************************/
/*! \brief          Copies header up to the length field.
 *  \details        -
 *  \param[in]      GenHdrCfgIdx                Generic header configuration index.
 *                                              [range: GenHdrCfgIdx < DoIPInt_GetSizeOfGenHdrCfg()]
 *  \param[in,out]  PduInfoPtr                  Provides the buffer and the number of bytes to be copied.
 *                                              [range: IN: PduInfoPtr != NULL_PTR,
 *                                                          PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr)
 *                                                            || PduInfoPtr->SduLength == 0 =>
 *                                                               PduInfoPtr->SduDataPtr == NULL_PTR
 *                                                      OUT: PduInfoPtr->SduLength == 0 || unchanged]
 *  \param[out]     AvailableDataPtr            Indicates the remaining number of bytes that are available to be copied.
 *                                              [range: AvailableDataPtr != NULL_PTR]
 *  \return         DOIPINT_MSG_HANDLER_PENDING Copy process succeeded, message handling can be continued.
 *  \return         DOIPINT_MSG_HANDLER_FAILED  Number of bytes to be copied is invalid.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires GenHdrCfgIdx < DoIPInt_GetSizeOfGenHdrCfg();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_GenHdr_RxParseCopyUpToLength(
  DoIPInt_SizeOfGenHdrCfgType GenHdrCfgIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxParseValidateUpToLengthField()
 *********************************************************************************************************************/
/*! \brief        Validates the ISO header stored in the connection specific header buffer up to the length field
 *                (excluding length field).
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       DOIPINT_MSG_HANDLER_PENDING  Header up to length field is valid, message handling can be continued.
 *  \return       DOIPINT_MSG_HANDLER_FINISHED Header up to length field is invalid, message handling can be continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Header up to length field is invalid, message shall be skipped.
 *  \return       DOIPINT_MSG_HANDLER_FAILED   Header up to length field is invalid, message handling failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_GenHdr_RxParseValidateUpToLengthField(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxParseValidateLengthField()
 *********************************************************************************************************************/
/*! \brief        Validates the length field stored in the connection specific header buffer.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[out]   AvailableDataPtr             Indicates the remaining number of bytes that are available to be copied.
 *                                             [range: AvailableDataPtr != NULL_PTR]
 *  \return       DOIPINT_MSG_HANDLER_FINISHED Length is valid, message handler has finished.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Length is invalid, message shall be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_GenHdr_RxParseValidateLengthField(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxParseCopyUpToLength()
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
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_GenHdr_RxParseCopyUpToLength(
  DoIPInt_SizeOfGenHdrCfgType GenHdrCfgIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8                            offset = DoIPInt_GetRxOffsetParseOfGenHdrCfgDyn(GenHdrCfgIdx);
  DoIPInt_SizeOfGenHdrParseBufType parseBufStartIdx = DoIPInt_GetGenHdrParseBufStartIdxOfGenHdrCfg(GenHdrCfgIdx);
  DoIPInt_SizeOfGenHdrParseBufType parseBufLength = DoIPInt_GetGenHdrParseBufEndIdxOfGenHdrCfg(GenHdrCfgIdx) -
    parseBufStartIdx;
  DoIPInt_MsgHndl_RetType          retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if data length to be parsed is valid. */
  if ( DoIPInt_Util_IsLengthInRange(PduInfoPtr->SduLength, DOIPINT_GEN_HDR_LEN, parseBufLength, offset) == E_OK )
  {
    /* #20 Copy data to local buffer and switch to next state if data up to length field has been copied. */
    P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) hdrBufPtr = DoIPInt_GetAddrGenHdrParseBuf(parseBufStartIdx + offset);

    if ( DoIPInt_Util_CopyUpToLevel(DOIPINT_GEN_HDR_LEN, &offset, hdrBufPtr, PduInfoPtr, AvailableDataPtr) == E_OK )
    {
      DoIPInt_SetRxStateOfGenHdrCfgDyn(GenHdrCfgIdx, DOIPINT_GEN_HDR_RX_STATE_VALIDATE_HDR);
    }

    /* #30 Indicate that data has been copied. */
    PduInfoPtr->SduLength = 0u;

    /* #40 Update offset for parsed header. */
    DoIPInt_SetRxOffsetParseOfGenHdrCfgDyn(GenHdrCfgIdx, offset);

    /* #50 Indicate that parsing is pending. */
    retVal = DOIPINT_MSG_HANDLER_PENDING;
  }
  /* #11 Indicate that parsing has failed otherwise. */
  else
  {
    retVal = DOIPINT_MSG_HANDLER_FAILED;
  }

  return retVal;
} /* DoIPInt_GenHdr_RxParseCopyUpToLength() */

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxParseValidateUpToLengthField()
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
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_GenHdr_RxParseValidateUpToLengthField(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(uint8, AUTOMATIC, DOIPINT_VAR) genHdrBufPtr = DoIPInt_GetAddrGenHdrParseBuf(
    DoIPInt_GetGenHdrParseBufStartIdxOfGenHdrCfg(DoIPInt_GetGenHdrCfgIdxOfConnection(ConnectionIdx)));
  DoIPInt_MsgHndl_RetType                retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the fields from header. */
  const uint8  protocolVersion = IpBase_GetUint8(genHdrBufPtr, DOIPINT_GEN_HDR_OFFSET_PROTOCOL);
  const uint8  invProtocolVersion = IpBase_GetUint8(genHdrBufPtr, DOIPINT_GEN_HDR_OFFSET_INV_PROTOCOL);

  /* #20 Check if protocol version is invalid. */
  if ( protocolVersion != DOIPINT_PROTOCOL_VERSION )
  {
    /* #200 Report event in case of an invalid protocol version. */
    retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_MSG_INV_PATTERN);
  }
  /* #21 Check if inverse protocol version is invalid otherwise. */
  else if ( invProtocolVersion != DOIPINT_INV_PROTOCOL_VERSION )
  {
    /* #210 Report event in case of an invalid inverse protocol version. */
    retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_MSG_INV_PATTERN);
  }
  /* #22 Assume header is valid and switch to next state otherwise. */
  else
  {
    DoIPInt_SetRxStateOfGenHdrCfgDyn(ConnectionIdx, DOIPINT_GEN_HDR_RX_STATE_VALIDATE_LENGTH);
    retVal = DOIPINT_MSG_HANDLER_PENDING;
  }

  return retVal;
} /* DoIPInt_GenHdr_RxParseValidateUpToLengthField() */

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxParseValidateLengthField()
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
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_GenHdr_RxParseValidateLengthField(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the length field from header. */
  const uint32 payloadLength = DOIPINT_GENHDR_RX_GET_PARSED_PAYLOAD_LENGTH(ConnectionIdx);

  /* #20 Check if length field exceeds the maximum length otherwise. */
  if ( payloadLength > DoIPInt_Util_GetSupportedPduLength(payloadLength) )
  {
    /* #200 Set the available buffer size to the invalid payload length to skip the message. */
    *AvailableDataPtr = DoIPInt_Util_GetSupportedPduLength(payloadLength);

    /* #210 Report event that message is too large. */
    retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_ISO);
  }
  /* #21 Assume length field is valid and switch to next state otherwise. */
  else
  {
    retVal = DOIPINT_MSG_HANDLER_FINISHED;
  }

  return retVal;
} /* DoIPInt_GenHdr_RxParseValidateLengthField() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DoIPInt_GenHdr_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_GenHdr_Init(
  DoIPInt_ConnectionIterType ConnectionIter)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfGenHdrCfgType genHdrCfgIdx = DoIPInt_GetGenHdrCfgIdxOfConnection(ConnectionIter);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the transmission offset. */
  DoIPInt_SetTxOffsetOfGenHdrCfgDyn(genHdrCfgIdx, 0u);

  /* #20 Initialize the generic header parse state and offset. */
  DoIPInt_GenHdr_RxStopParse((DoIPInt_SizeOfConnectionType)ConnectionIter);
} /* DoIPInt_GenHdr_Init() */

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_TxCopy()
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
FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_GenHdr_TxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint16 PayloadType,
  uint32 UserDataLength,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfGenHdrCfgType genHdrCfgIdx = DoIPInt_GetGenHdrCfgIdxOfConnection(ConnectionIdx);
  uint8                             offset = DoIPInt_GetTxOffsetOfGenHdrCfgDyn(genHdrCfgIdx);
  uint8                             dataToBeCopied;
  DoIPInt_MsgHndl_RetType           retVal = DOIPINT_MSG_HANDLER_PENDING;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate data to be copied depending on remaining header length. */
  dataToBeCopied = DOIPINT_GEN_HDR_LEN - offset;

  /* #20 Limit data to be copied to the requested length. */
  if ( PduInfoPtr->SduLength < dataToBeCopied )
  {
    dataToBeCopied = (uint8)PduInfoPtr->SduLength;
  }

  /* #30 Check if data shall be copied. */
  if ( dataToBeCopied > 0u )
  {
    uint8 hdrBuf[DOIPINT_GEN_HDR_LEN];

    /* #40 Copy ISO header to local temporary buffer. */
    hdrBuf[0u] = DOIPINT_PROTOCOL_VERSION;
    hdrBuf[1u] = DOIPINT_INV_PROTOCOL_VERSION;

    hdrBuf[2u] = (uint8)(PayloadType >> 8);
    hdrBuf[3u] = (uint8)(PayloadType);

    hdrBuf[4u] = (uint8)(UserDataLength >> 24);
    hdrBuf[5u] = (uint8)(UserDataLength >> 16);
    hdrBuf[6u] = (uint8)(UserDataLength >> 8);
    hdrBuf[7u] = (uint8)(UserDataLength);

    /* #50 Copy local buffer to provided buffer with specified offset. */
    /*@ assert $external(PduInfoPtr->SduDataPtr); */                                                                   /* VCA_DOIPINT_MEM_CPY_EXT_POINTER_LENGTH_VALID */
    /* PRQA S 314 1 */ /* MD_MSR_VStdLibCopy */
    VStdLib_MemCpy((void*)PduInfoPtr->SduDataPtr, (void*)&hdrBuf[offset], dataToBeCopied);                             /* VCA_DOIPINT_MEM_CPY_EXT_POINTER_LENGTH_VALID */

    /* #60 Update transmission offset. */
    offset += dataToBeCopied;
    DoIPInt_SetTxOffsetOfGenHdrCfgDyn(genHdrCfgIdx, offset);

    /* #70 Update provided buffer struct. */
    PduInfoPtr->SduLength -= (PduLengthType)dataToBeCopied;
    PduInfoPtr->SduDataPtr = &PduInfoPtr->SduDataPtr[dataToBeCopied];
  }

  /* #80 Indicate to switch to next state to copy user data if generic header has been copied completely. */
  if ( offset >= DOIPINT_GEN_HDR_LEN )
  {
    retVal = DOIPINT_MSG_HANDLER_FINISHED;
  }

  /* #90 Set the available data pointer to the remaining length of the generic header that needs to be copied. */
  *AvailableDataPtr = DOIPINT_GEN_HDR_LEN - (PduLengthType)offset;

  return retVal;
} /* DoIPInt_GenHdr_TxCopy() */

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_TxConfirmation()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_GenHdr_TxConfirmation(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfGenHdrCfgType genHdrCfgIdx = DoIPInt_GetGenHdrCfgIdxOfConnection(ConnectionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset offset. */
  DoIPInt_SetTxOffsetOfGenHdrCfgDyn(genHdrCfgIdx, 0u);
} /* DoIPInt_GenHdr_TxConfirmation() */

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxCopy()
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
FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_GenHdr_RxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHandlerRxStateType     lastMsgHandlerRxState;
  const DoIPInt_SizeOfGenHdrCfgType genHdrCfgIdx = DoIPInt_GetGenHdrCfgIdxOfConnection(ConnectionIdx);
  DoIPInt_MsgHndl_RetType           retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Process state machine until segment to be parsed is handled completely (i.e. no state transition anymore). */
  do
  {
    lastMsgHandlerRxState = DoIPInt_GetRxStateOfGenHdrCfgDyn(genHdrCfgIdx);

    /* #20 Handle parse process depending on state. */
    switch ( lastMsgHandlerRxState )
    {
      /* #30 Copy the header to local buffer up to length field. */
      case DOIPINT_GEN_HDR_RX_STATE_COPY_UP_TO_LENGTH:
      {
        retVal = DoIPInt_GenHdr_RxParseCopyUpToLength(genHdrCfgIdx, PduInfoPtr, AvailableDataPtr);
        break;
      }
      /* #40 Validate header up to length field. */
      case DOIPINT_GEN_HDR_RX_STATE_VALIDATE_HDR:
      {
        retVal = DoIPInt_GenHdr_RxParseValidateUpToLengthField(ConnectionIdx);
        break;
      }
      /* #50 Validate length field. */
      case DOIPINT_GEN_HDR_RX_STATE_VALIDATE_LENGTH:
      {
        retVal = DoIPInt_GenHdr_RxParseValidateLengthField(ConnectionIdx, AvailableDataPtr);
        break;
      }
      /* #60 Return error if parsing is not active. */
      default: /* DOIPINT_GEN_HDR_RX_STATE_NONE */
      {
        retVal = DOIPINT_MSG_HANDLER_FAILED;
        break;
      }
    }
  } while ( DoIPInt_GetRxStateOfGenHdrCfgDyn(genHdrCfgIdx) != lastMsgHandlerRxState );

  return retVal;
} /* DoIPInt_GenHdr_RxCopy() */

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxStartParse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_GenHdr_RxStartParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Stop parsing. */
  DoIPInt_GenHdr_RxStopParse(ConnectionIdx);

  /* #20 Set state to indicate that parsing has been started. */
  DoIPInt_SetRxStateOfGenHdrCfgDyn(DoIPInt_GetGenHdrCfgIdxOfConnection(ConnectionIdx),
    DOIPINT_GEN_HDR_RX_STATE_COPY_UP_TO_LENGTH);

  /* #30 Set the available buffer size to the size of the header. */
  *AvailableDataPtr = DOIPINT_GEN_HDR_LEN;
} /* DoIPInt_GenHdr_RxStartParse() */

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxStopParse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_GenHdr_RxStopParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset offset for parsing. */
  DoIPInt_SetRxOffsetParseOfGenHdrCfgDyn(DoIPInt_GetGenHdrCfgIdxOfConnection(ConnectionIdx), 0u);

  /* #20 Reset state. */
  DoIPInt_SetRxStateOfGenHdrCfgDyn(DoIPInt_GetGenHdrCfgIdxOfConnection(ConnectionIdx), DOIPINT_GEN_HDR_RX_STATE_NONE);
} /* DoIPInt_GenHdr_RxStopParse() */

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxGetParsedPayloadLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint32, DOIPINT_CODE) DoIPInt_GenHdr_RxGetParsedPayloadLength(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get parsed payload length and return it. */
  const uint32 payloadLength = DOIPINT_GENHDR_RX_GET_PARSED_PAYLOAD_LENGTH(ConnectionIdx);

  return payloadLength;
} /* DoIPInt_GenHdr_RxGetParsedPayloadLength() */

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxGetParsedPayloadType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint16, DOIPINT_CODE) DoIPInt_GenHdr_RxGetParsedPayloadType(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get parsed payload type and return it. */
  const uint16 payloadType = IpBase_GetUint16(DoIPInt_GetAddrGenHdrParseBuf(
      DoIPInt_GetGenHdrParseBufStartIdxOfGenHdrCfg(DoIPInt_GetGenHdrCfgIdxOfConnection(ConnectionIdx))),
    DOIPINT_GEN_HDR_OFFSET_TYPE);

  return payloadType;
} /* DoIPInt_GenHdr_RxGetParsedPayloadType() */

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxGetParsedHdr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_GenHdr_RxGetParsedHdr(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  PduLengthType DestBufLength,
  P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) DestBufPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) MsgLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfGenHdrCfgType genHdrCfgIdx = DoIPInt_GetGenHdrCfgIdxOfConnection(ConnectionIdx);
  const uint8                       parseOffset = DoIPInt_GetRxOffsetParseOfGenHdrCfgDyn(genHdrCfgIdx);
  PduLengthType                     lengthToCopy = 0u;

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

  /* #20 Get the parsed header. */
  /* PRQA S 314 2 */ /* MD_MSR_VStdLibCopy */
  VStdLib_MemCpy((void*)DestBufPtr,                                                                                    /* VCA_DOIPINT_MEM_CPY_POINTER_LENGTH_CHECKED */
    DOIPINT_A_P2VAR(void)DoIPInt_GetAddrGenHdrParseBuf(DoIPInt_GetGenHdrParseBufStartIdxOfGenHdrCfg(genHdrCfgIdx)),
    lengthToCopy);

  *MsgLengthPtr = lengthToCopy;
} /* DoIPInt_GenHdr_RxGetParsedHdr() */

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_GenHdr.c
 *********************************************************************************************************************/
