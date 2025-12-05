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
/*!        \file  DoIPInt_Tx.c
 *        \brief  DoIPInt source file for sub-module DoIPInt_Tx.
 *      \details  Implementation for sub-module DoIPInt_Tx of component DoIPInt.
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

#define DOIPINT_TX_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "DoIPInt.h"
#include "DoIPInt_Priv.h"
#include "DoIPInt_Tx.h"
#include "DoIPInt_TxQueue.h"
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
#include "DoIPInt_GenHdr.h"
#include "DoIPInt_DiagMsg.h"
#include "DoIPInt_OemSpecific.h"
#else
#include "DoIPInt_LightHdr.h"
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
#include "DoIPInt_Connection.h"
#include "SoAd.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#if !defined (DOIPINT_LOCAL)
# define DOIPINT_LOCAL static
#endif /* !defined (DOIPINT_LOCAL) */

#if !defined (DOIPINT_LOCAL_INLINE)
# define DOIPINT_LOCAL_INLINE LOCAL_INLINE
#endif /* !defined (DOIPINT_LOCAL_INLINE) */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define DOIPINT_START_SEC_CODE
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyMessage()
 *********************************************************************************************************************/
/*! \brief        Copies message to provided buffer.
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
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyMessage(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_Tx_LimitAvailableData()
 *********************************************************************************************************************/
/*! \brief        Limits the available length of data to be copied considering the message length.
 *  \details      -
 *  \param[in]    MsgHandlerIdx           Message handler index.
 *                                        [range: MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler()]
 *  \param[in]    UserDataLength          Length of user data of the current transmission.
 *  \param[in]    Offset                  Offset of the already copied data.
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: IN:  AvailableDataPtr != NULL_PTR
 *                                                OUT: undefined]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \spec
 *    requires MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler();
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_Tx_LimitAvailableData(
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx,
  uint32 UserDataLength,
  uint32 Offset,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
/**********************************************************************************************************************
 *  DoIPInt_Tx_DeriveGenHdrInformation()
 *********************************************************************************************************************/
/*! \brief         Derives information (user data length and payload type) from the currently active transmission
 *                 information which are required for the generic header.
 *  \details       User data length and payload type are derived according to the used message handler for an active
 *                 transmission.
 *  \param[in]     ConnectionIdx      Connection index.
 *                                    [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]     HandleIdx          Handle index of transmission.
 *                                    [range: For diagnostic messages: HandleIdx < DoIPInt_GetSizeOfChannel()
 *                                            For OEM specific payload: HandleIdx < DoIPInt_GetSizeOfOemPayloadTxBuf ]
 *  \param[in]     MsgHandlerIdx      Message handler index.
 *                                    [range: MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler()]
 *  \param[out]    UserDataLengthPtr  Pointer to the length of user data of the current transmission.
 *                                    [range: UserDataLengthPtr != NULL_PTR]
 *  \param[out]    PayloadTypePtr     Pointer to the payload type of the current transmission.
 *                                    [range: PayloadTypePtr != NULL_PTR]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler();
 *    requires UserDataLengthPtr != NULL_PTR;
 *    requires PayloadTypePtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_Tx_DeriveGenHdrInformation(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 HandleIdx,
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx,
  P2VAR(uint32, AUTOMATIC, DOIPINT_VAR) UserDataLengthPtr,
  P2VAR(uint16, AUTOMATIC, DOIPINT_VAR) PayloadTypePtr);

/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyGenericHeader()
 *********************************************************************************************************************/
/*! \brief          Copies the generic header data and returns the remaining amount of available data.
 *  \details        -
 *  \param[in]      ConnectionIdx         Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in,out]  PduInfoPtr            Provides the destination buffer and the number of bytes to be copied.
 *                                        [range: PduInfoPtr != NULL_PTR,
 *                                                PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]     AvailableDataPtr      Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: AvailableDataPtr != NULL_PTR]
 *  \return         BUFREQ_OK             Data has been copied and/or query succeeded.
 *  \return         BUFREQ_E_NOT_OK       Copying data or query request failed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyGenericHeader(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyUserData()
 *********************************************************************************************************************/
/*! \brief          Copies user data and returns the remaining amount of available data.
 *  \details        -
 *  \param[in]      ConnectionIdx         Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in,out]  PduInfoPtr            Provides the destination buffer and the number of bytes to be copied. Is
 *                                        updated on return with the offset of copied data.
 *                                        [range: PduInfoPtr != NULL_PTR,
 *                                                PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]     AvailableDataPtr      Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: AvailableDataPtr != NULL_PTR]
 *  \return         BUFREQ_OK             Data has been copied to the transmit buffer completely as requested.
 *  \return         BUFREQ_E_NOT_OK       Data has not been copied. Request failed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyUserData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyPayloadTypeSpecificData()
 *********************************************************************************************************************/
/*! \brief        Copies user data according to connection API type.
 *  \details      -
 *  \param[in]    ConnectionIdx         Connection index.
 *                                      [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
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
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyPayloadTypeSpecificData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT )
/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyLightHeader()
 *********************************************************************************************************************/
/*! \brief          Copies light header data and returns the remaining amount of available data.
 *  \details        -
 *  \param[in]      ConnectionIdx         Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in,out]  PduInfoPtr            Provides the destination buffer and the number of bytes to be copied. Is
 *                                        updated on return with the offset of copied data.
 *                                        [range: PduInfoPtr != NULL_PTR,
 *                                                PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]     AvailableDataPtr      Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: AvailableDataPtr != NULL_PTR]
 *  \return         BUFREQ_OK             Data has been copied to the transmit buffer completely as requested.
 *  \return         BUFREQ_E_NOT_OK       Data has not been copied. Request failed.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyLightHeader(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT */

/**********************************************************************************************************************
 *  DoIPInt_Tx_ForwardConfirmation()
 *********************************************************************************************************************/
/*! \brief        Forwards confirmation for transmitted PDU.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    HandleIdx               Handle index.
 *                                        [range: Diagnostic messages: HandleIdx < DoIPInt_GetSizeOfChannel()
 *                                                OEM specific messages: HandleIdx < DoIPInt_GetSizeOfOemPayloadTxBuf]
 *  \param[in]    MsgHandlerIdx           Message handler index.
 *                                        [range: MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler()]
 *  \param[in]    Result                  Result of the transmission.
 *                                        [range: E_OK, E_NOT_OK]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_Tx_ForwardConfirmation(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 HandleIdx,
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx,
  Std_ReturnType Result);

/**********************************************************************************************************************
 *  DoIPInt_Tx_GetMsgHandlerSpecificHdrLen()
 *********************************************************************************************************************/
/*! \brief        Gets the header length depending on the used message handler and header type.
 *  \details      -
 *  \param[in]    MsgHandlerIdx   Message handler index.
 *                                [range: MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler()]
 *  \return       HeaderLen       Header length used for current transmission on the connection.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \spec
 *    requires MsgHandlerIdx < DoIPInt_GetSizeOfMsgHandler();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(uint32, DOIPINT_CODE) DoIPInt_Tx_GetMsgHandlerSpecificHdrLen(
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyMessage()
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
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyMessage(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType         pduInfo;
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
  DoIPInt_TxStateType lastStateTx;
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
  BufReq_ReturnType   retVal = BUFREQ_OK;

  /* ----- Implementation ----------------------------------------------- */
  pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
  pduInfo.SduLength = PduInfoPtr->SduLength;

  /* #10 Process state machine until no further state transition occurs. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
  do
  {
    lastStateTx = DoIPInt_GetTxStateOfConnectionDyn(ConnectionIdx);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

    /* #20 Handle copy process depending on transmission state. */
    switch ( DoIPInt_GetTxStateOfConnectionDyn(ConnectionIdx) )
    {
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      /* #200 Copy generic header. */
      case DOIPINT_TX_STATE_GEN_HDR:
      {
        retVal = DoIPInt_Tx_CopyGenericHeader(ConnectionIdx, &pduInfo, AvailableDataPtr);
        break;
      }
      /* #201 Copy user data. */
      case DOIPINT_TX_STATE_USER_DATA:
      {
        retVal = DoIPInt_Tx_CopyUserData(ConnectionIdx, &pduInfo, AvailableDataPtr);
        break;
      }
#else
      /* #202 Copy light header data. */
      case DOIPINT_TX_STATE_LIGHT_HDR:
      {
        retVal = DoIPInt_Tx_CopyLightHeader(ConnectionIdx, &pduInfo, AvailableDataPtr);
        break;
      }
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
      /* #203 Copy nothing if no transmission is active and indicate error. */
      default: /* DOIPINT_TX_STATE_NONE */
      {
        retVal = BUFREQ_E_NOT_OK;
        break;
      }
    }
  /* #30 Stop processing if state machine does not change anymore. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
  } while ( lastStateTx != DoIPInt_GetTxStateOfConnectionDyn(ConnectionIdx) );
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

  return retVal;
} /* DoIPInt_Tx_CopyMessage() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_LimitAvailableData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_Tx_LimitAvailableData(
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx,
  uint32 UserDataLength,
  uint32 Offset,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint32 hdrLen = DoIPInt_Tx_GetMsgHandlerSpecificHdrLen(MsgHandlerIdx);
  const uint32 msgLength = UserDataLength + hdrLen;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if message is copied completely. */
  if ( Offset == msgLength )
  {
    /* #100 Indicate that no data can be copied. */
    *AvailableDataPtr = 0u;
  }
  /* #20 Set available data considering the provided size but at maximum the remaining message length otherwise. */
  else
  {
    uint32 pendingMsgLength = msgLength - Offset;

    /* #200 Set available data to remaining message length if the current value exceeds the remaining data. */
    if ( *AvailableDataPtr > pendingMsgLength )
    {
      *AvailableDataPtr = (PduLengthType)pendingMsgLength;
    }
  }
} /* DoIPInt_Tx_LimitAvailableData() */

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
/**********************************************************************************************************************
 *  DoIPInt_Tx_DeriveGenHdrInformation()
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
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_Tx_DeriveGenHdrInformation(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 HandleIdx,
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx,
  P2VAR(uint32, AUTOMATIC, DOIPINT_VAR) UserDataLengthPtr,
  P2VAR(uint16, AUTOMATIC, DOIPINT_VAR) PayloadTypePtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the diagnostic message handler is used for transmission. */
  if ( MsgHandlerIdx == DoIPInt_GetMsgHandlerIdxOfDiagMsgHandler(0u) )
  {
    /* #100 Increase the to be transmitted user data length by the diagnostic header length. */
    *UserDataLengthPtr = DoIPInt_GetTxUserDataLengthOfConnectionDyn(ConnectionIdx) + DOIPINT_HDR_LOG_ADDR_LEN_DIAG;

    /* #101 Set the IF payload type in case the IF-API is used. */
    if ( DoIPInt_GetApiTypeOfChannel((DoIPInt_SizeOfChannelType)HandleIdx) == DOIPINT_API_TYPE_IF )
    {
      *PayloadTypePtr = DOIPINT_PAYLOAD_TYPE_IF;
    }
    /* #102 Set the diagnostic message payload type otherwise. */
    else
    {
      *PayloadTypePtr = DOIPINT_PAYLOAD_TYPE_DIAG_MSG;
    }
  }
  /* #11 OEM specific message handler is used otherwise. */
  else
  {
    /* #110 Update the user data length and payload type. */
    *UserDataLengthPtr = DoIPInt_GetTxUserDataLengthOfConnectionDyn(ConnectionIdx);
    /* VCA NL SLC-11 : VCA_DOIPINT_CODE_NOT_REACHED_WHEN_NULL_PTR */
    *PayloadTypePtr = DoIPInt_GetPayloadTypeOfOemPayloadTxBufConfigDyn(
      (DoIPInt_SizeOfOemPayloadTxBufConfigType)HandleIdx);
  }
} /* DoIPInt_Tx_DeriveGenHdrInformation() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyGenericHeader()
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
 */
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyGenericHeader(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const PduLengthType                initialLen = PduInfoPtr->SduLength;
  const uint32                       handleIdx = DoIPInt_GetTxHandleIdxOfConnectionDyn(ConnectionIdx);
  const DoIPInt_SizeOfMsgHandlerType msgHandlerIdx = DoIPInt_GetMsgHandlerTxIdxOfConnectionDyn(ConnectionIdx);
  uint32                             offset = DoIPInt_GetTxOffsetOfConnectionDyn(ConnectionIdx);
  uint32                             userDataLength = 0u;
  PduLengthType                      copiedLen;
  uint16                             payloadType = 0u;
  DoIPInt_MsgHndl_RetType            msgHndlRetVal;
  PduLengthType                      availableDataGenHdr = 0u;
  BufReq_ReturnType                  retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Derive user data length and payload type from the currently active transmission information. */
  /*@ assert msgHandlerIdx < DoIPInt_GetSizeOfMsgHandler(); */                                                         /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
  DoIPInt_Tx_DeriveGenHdrInformation(ConnectionIdx, handleIdx, msgHandlerIdx, &userDataLength, &payloadType);

  /* #20 Copy generic header. */
  msgHndlRetVal = DoIPInt_GenHdr_TxCopy(ConnectionIdx, payloadType, userDataLength, PduInfoPtr, &availableDataGenHdr);

  /* #30 Update the offset. */
  copiedLen = initialLen - PduInfoPtr->SduLength;
  offset += copiedLen;
  DoIPInt_SetTxOffsetOfConnectionDyn(ConnectionIdx, offset);

  /* #40 Update the state in case message handler has finished. */
  if ( msgHndlRetVal == DOIPINT_MSG_HANDLER_FINISHED )
  {
    DoIPInt_SetTxStateOfConnectionDyn(ConnectionIdx, DOIPINT_TX_STATE_USER_DATA);
    *AvailableDataPtr = 0u;
    retVal = BUFREQ_OK;
  }
  /* #41 Query the available data for the next call otherwise. */
  else
  {
    PduLengthType availableDataPayloadTypeSpecific = 0u;
    PduInfoType   pduInfo = { NULL_PTR, 0u };

    /* #410 Query the payload type specific available data. */
    retVal = DoIPInt_Tx_CopyPayloadTypeSpecificData(ConnectionIdx, &pduInfo, &availableDataPayloadTypeSpecific);

    /* #411 Calculate the sum of available data considering the generic header and the payload specific part. */
    *AvailableDataPtr = availableDataGenHdr + availableDataPayloadTypeSpecific;

    /* #412 Limit the available data considering the message length. */
    DoIPInt_Tx_LimitAvailableData(msgHandlerIdx, userDataLength, offset, AvailableDataPtr);
  }

  return retVal;
} /* DoIPInt_Tx_CopyGenericHeader() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyUserData()
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
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyUserData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const PduLengthType initialLen = PduInfoPtr->SduLength;
  BufReq_ReturnType   retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy data according to the payload type and check if copying succeeds. */
  if ( DoIPInt_Tx_CopyPayloadTypeSpecificData(ConnectionIdx, PduInfoPtr, AvailableDataPtr) == BUFREQ_OK )
  {
    const DoIPInt_SizeOfMsgHandlerType msgHandlerIdx = DoIPInt_GetMsgHandlerTxIdxOfConnectionDyn(ConnectionIdx);
    const uint32                       userDataLength = DoIPInt_GetTxUserDataLengthOfConnectionDyn(ConnectionIdx);
    const uint32                       offset = DoIPInt_GetTxOffsetOfConnectionDyn(ConnectionIdx) + initialLen;

    /* #20 Update the offset. */
    DoIPInt_SetTxOffsetOfConnectionDyn(ConnectionIdx, offset);

    /* #30 Update provided buffer struct. */
    PduInfoPtr->SduDataPtr = &PduInfoPtr->SduDataPtr[PduInfoPtr->SduLength];
    PduInfoPtr->SduLength = 0u;

    /* #40 Limit the available data considering the message length. */
    /*@ assert msgHandlerIdx < DoIPInt_GetSizeOfMsgHandler(); */                                                       /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
    DoIPInt_Tx_LimitAvailableData(msgHandlerIdx, userDataLength, offset, AvailableDataPtr);

    retVal = BUFREQ_OK;
  }
  /* #11 Indicate that no data can be copied and return an error otherwise. */
  else
  {
    *AvailableDataPtr = 0u;
    retVal = BUFREQ_E_NOT_OK;
  }

  return retVal;
} /* DoIPInt_Tx_CopyUserData() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyPayloadTypeSpecificData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyPayloadTypeSpecificData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfMsgHandlerType msgHandlerIdx = DoIPInt_GetMsgHandlerTxIdxOfConnectionDyn(ConnectionIdx);
  const uint32                       handleIdx = DoIPInt_GetTxHandleIdxOfConnectionDyn(ConnectionIdx);
  BufReq_ReturnType                  retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if OEM specific message handler is used for the connection. */
  if ( msgHandlerIdx == DoIPInt_GetMsgHandlerIdxOfOemSpecificMsgHandler(0u) )
  {
    /* #100 Copy data from internal OEM specific payload buffer. */
    /*@ assert handleIdx < DoIPInt_GetSizeOfOemPayloadTxBufConfig(); */                                                /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
    retVal = DoIPInt_OemSpecific_TxCopy(ConnectionIdx, (DoIPInt_SizeOfOemPayloadTxBufConfigType)handleIdx, PduInfoPtr,
      AvailableDataPtr);
  }
  /* #11 Assume diagnostic message handler is used otherwise. */
  else
  {
    /* #110 Copy diagnostic message user data. */
    /*@ assert handleIdx < DoIPInt_GetSizeOfChannel(); */                                                              /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
    retVal = DoIPInt_DiagMsg_TxCopy(ConnectionIdx, (DoIPInt_SizeOfChannelType)handleIdx, PduInfoPtr, AvailableDataPtr);
  }

  return retVal;
} /* DoIPInt_Tx_CopyPayloadTypeSpecificData() */
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT )
/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyLightHeader()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyLightHeader(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const PduLengthType             initialLen = PduInfoPtr->SduLength;
  const uint32                    userDataLength = DoIPInt_GetTxUserDataLengthOfConnectionDyn(ConnectionIdx);
  const DoIPInt_SizeOfChannelType handleIdx = (DoIPInt_SizeOfChannelType)DoIPInt_GetTxHandleIdxOfConnectionDyn(
    ConnectionIdx);
  BufReq_ReturnType               retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy light header data and check if copying succeeds. */
  /*@ assert handleIdx < DoIPInt_GetSizeOfChannel(); */                                                                /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
  if ( DoIPInt_LightHdr_TxCopy(ConnectionIdx, handleIdx, userDataLength, PduInfoPtr, AvailableDataPtr) == BUFREQ_OK )
  {
    const DoIPInt_SizeOfMsgHandlerType msgHandlerIdx = DoIPInt_GetMsgHandlerTxIdxOfConnectionDyn(ConnectionIdx);
    const PduLengthType                copiedLen = initialLen - PduInfoPtr->SduLength;
    const uint32                       offset = DoIPInt_GetTxOffsetOfConnectionDyn(ConnectionIdx) + copiedLen;

    /* #20 Update the offset. */
    DoIPInt_SetTxOffsetOfConnectionDyn(ConnectionIdx, offset);

    /* #30 Limit the available data considering the message length. */
    /*@ assert msgHandlerIdx < DoIPInt_GetSizeOfMsgHandler(); */                                                       /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
    DoIPInt_Tx_LimitAvailableData(msgHandlerIdx, userDataLength, offset, AvailableDataPtr);

    retVal = BUFREQ_OK;
  }
  /* #11 Indicate that no data can be copied and return an error otherwise. */
  else
  {
    *AvailableDataPtr = 0u;
    retVal = BUFREQ_E_NOT_OK;
  }

  return retVal;
} /* DoIPInt_Tx_CopyLightHeader() */
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT */

/**********************************************************************************************************************
 *  DoIPInt_Tx_ForwardConfirmation()
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
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_Tx_ForwardConfirmation(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 HandleIdx,
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx,
  Std_ReturnType Result)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
  /* #10 Forward Tx confirmation to generic header unit (if ISO header is used). */
  DoIPInt_GenHdr_TxConfirmation(ConnectionIdx);

  /* #20 Check if OEM specific message handler is used for the connection and forward Tx confirmation to OEM specific
   *     unit. */
  if ( MsgHandlerIdx == DoIPInt_GetMsgHandlerIdxOfOemSpecificMsgHandler(0u) )
  {
    /*@ assert HandleIdx < DoIPInt_GetSizeOfOemPayloadTxBufConfig(); */                                                /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
    DoIPInt_OemSpecific_TxConfirmation(ConnectionIdx, (DoIPInt_SizeOfOemPayloadTxBufConfigType)HandleIdx);
  }
  /* #21 Forward Tx confirmation to diagnostic message unit otherwise. */
  else
  {
    /*@ assert HandleIdx < DoIPInt_GetSizeOfChannel(); */                                                              /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
    DoIPInt_DiagMsg_TxConfirmation(ConnectionIdx, (DoIPInt_SizeOfChannelType)HandleIdx, Result);
  }
#else
  /* #30 Forward Tx confirmation to light header unit otherwise (if LIGHT header is used). */
  /*@ assert HandleIdx < DoIPInt_GetSizeOfChannel(); */                                                                /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
  DoIPInt_LightHdr_TxConfirmation(ConnectionIdx, (DoIPInt_SizeOfChannelType)HandleIdx, Result);

  DOIPINT_DUMMY_STATEMENT(MsgHandlerIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
} /* DoIPInt_Tx_ForwardConfirmation() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_GetMsgHandlerSpecificHdrLen()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(uint32, DOIPINT_CODE) DoIPInt_Tx_GetMsgHandlerSpecificHdrLen(
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 hdrLen;

  /* ----- Implementation ----------------------------------------------- */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
  /* #10 Get the diagnostic header length in case of a diagnostic message (and if ISO header is used). */
  if ( MsgHandlerIdx == DoIPInt_GetMsgHandlerIdxOfDiagMsgHandler(0u) )
  {
    hdrLen = DOIPINT_DIAG_HDR_LEN;
  }
  /* #11 Get the generic header length in case of an OEM specific message otherwise. */
  else
  {
    hdrLen = DOIPINT_GEN_HDR_LEN;
  }
#else
  /* #20 Get the light header length (in case LIGHT header is used). */
  hdrLen = DOIPINT_LIGHT_HDR_LEN;

  DOIPINT_DUMMY_STATEMENT(MsgHandlerIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

  return hdrLen;
} /* DoIPInt_Tx_GetMsgHandlerSpecificHdrLen() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DoIPInt_Tx_Init()
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
FUNC(void, DOIPINT_CODE) DoIPInt_Tx_Init(
  DoIPInt_ConnectionIterType ConnectionIter)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize transmission state. */
  DoIPInt_SetTxStateOfConnectionDyn(ConnectionIter, DOIPINT_TX_STATE_NONE);

  /* #20 Initialize transmission offset. */
  DoIPInt_SetTxOffsetOfConnectionDyn(ConnectionIter, 0u);

  /* #30 Initialize transmission user data length. */
  DoIPInt_SetTxUserDataLengthOfConnectionDyn(ConnectionIter, 0u);

  /* #40 Initialize transmission message handler. */
  DoIPInt_SetMsgHandlerTxIdxOfConnectionDyn(ConnectionIter, DoIPInt_GetSizeOfMsgHandler());

  /* #50 Initialize transmission handle index. */
  DoIPInt_SetTxHandleIdxOfConnectionDyn(ConnectionIter, DOIPINT_MAX_UINT_32);

  /* #60 Initialize connection in sub-module DoIPInt_TxQueue. */
  DoIPInt_TxQueue_Init(ConnectionIter);
} /* DoIPInt_Tx_Init() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_PrepareTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Tx_PrepareTransmit(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 HandleIdx,
  PduLengthType Length,
  DoIPInt_SizeOfMsgHandlerType MsgHandlerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if connection is opened and not requested to be closed. */
  if ( DoIPInt_Connection_IsClosedOrRequested(ConnectionIdx) == FALSE )
  {
    /* #20 Try to add the transmission request to the transmission queue. */
    if ( DoIPInt_TxQueue_Add(ConnectionIdx, HandleIdx, Length, MsgHandlerIdx) == E_OK )
    {
      retVal = E_OK;
    }
  }

  return retVal;
} /* DoIPInt_Tx_PrepareTransmit() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_TransmitFromQueue()
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
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Tx_TransmitFromQueue(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType                  pduInfo;
  PduLengthType                length = 0u;
  uint32                       handleIdx = 0u;
  DoIPInt_SizeOfMsgHandlerType msgHandlerIdx = 0u;
  Std_ReturnType               retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent queue modifications from different context. */
  DOIPINT_BEGIN_CRITICAL_SECTION();

  /* #20 Try to get the first queue element. */
  if ( DoIPInt_TxQueue_GetNext(ConnectionIdx, &handleIdx, &length, &msgHandlerIdx) == E_OK )
  {
    /*@ assert msgHandlerIdx < DoIPInt_GetSizeOfMsgHandler(); */                                                       /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
    const uint32 hdrLen = DoIPInt_Tx_GetMsgHandlerSpecificHdrLen(msgHandlerIdx);
    pduInfo.SduDataPtr = NULL_PTR;
    pduInfo.SduLength = (PduLengthType)hdrLen + length;

    /* #30 Check if no transmission is pending. */
    if ( DoIPInt_GetTxStateOfConnectionDyn(ConnectionIdx) == DOIPINT_TX_STATE_NONE )
    {
      /* #40 Set active message handler, length and handle index on connection. */
      DoIPInt_SetMsgHandlerTxIdxOfConnectionDyn(ConnectionIdx, msgHandlerIdx);
      DoIPInt_SetTxUserDataLengthOfConnectionDyn(ConnectionIdx, length);
      DoIPInt_SetTxHandleIdxOfConnectionDyn(ConnectionIdx, handleIdx);

      /* #50 Switch to state to copy header when ISO header is used or to copy user data otherwise and reset
       *     transmission offset. */
      DoIPInt_SetTxOffsetOfConnectionDyn(ConnectionIdx, 0u);
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      DoIPInt_SetTxStateOfConnectionDyn(ConnectionIdx, DOIPINT_TX_STATE_GEN_HDR);
#else
      DoIPInt_SetTxStateOfConnectionDyn(ConnectionIdx, DOIPINT_TX_STATE_LIGHT_HDR);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

      /* #60 Leave critical section. */
      DOIPINT_END_CRITICAL_SECTION();

      /* #70 Call SoAd to transmit data and check if call succeeds. */
      if ( SoAd_TpTransmit(DoIPInt_GetSoAdTxPduIdOfConnection(ConnectionIdx), &pduInfo) == E_OK )                      /* VCA_DOIPINT_CALL_EXTERNAL_FUNCTION_CONST_POINTER_ARGUMENT */
      {
        /* #700 Indicate transmission request as successful. */
        retVal = E_OK;
      }
      /* #71 Indicate that transmission has failed otherwise. */
      else
      {
        /* #710 Switch to state to indicate that no transmission is active. */
        DoIPInt_SetTxStateOfConnectionDyn(ConnectionIdx, DOIPINT_TX_STATE_NONE);
      }
    }
    /* #31 Leave critical section otherwise. */
    else
    {
      DOIPINT_END_CRITICAL_SECTION();
    }
  }
  /* #21 Leave critical section otherwise. */
  else
  {
    DOIPINT_END_CRITICAL_SECTION();
  }

  return retVal;
} /* DoIPInt_Tx_TransmitFromQueue() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_ProcessTransmission()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Tx_ProcessTransmission(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_SizeOfConnectionType  connectionIdx;
  uint8_least                   txCnt;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all connections. */
  for ( connectionIdx = 0u; connectionIdx < DoIPInt_GetSizeOfConnection(); connectionIdx++ )
  {
    /* #20 Check if connection is opened. */
    if ( DoIPInt_Connection_IsOpened(connectionIdx) == TRUE )
    {
      /* #30 Try to process the transmission requests in queue up to the configured process limit. */
      for ( txCnt = 0u; txCnt < DoIPInt_GetTxProcessLimit(); txCnt++ )
      {
        /* #40 Stop the transmission sequence if transmission has failed, is pending or queue is empty. */
        if ( DoIPInt_Tx_TransmitFromQueue(connectionIdx) == E_NOT_OK )
        {
          break;
        }
      }
    }
  }
} /* DoIPInt_Tx_ProcessTransmission() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_Terminate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Tx_Terminate(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduLengthType                length = 0u;
  uint32                       handleIdx = 0u;
  DoIPInt_SizeOfMsgHandlerType msgHandlerIdx = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset transmission state (but not offset to prevent inconsistency if termination interrupts copy routine). */
  DoIPInt_SetTxStateOfConnectionDyn(ConnectionIdx, DOIPINT_TX_STATE_NONE);

  /* #20 Iterate over all elements in transmission queue. */
  while ( DoIPInt_TxQueue_GetNext(ConnectionIdx, &handleIdx, &length, &msgHandlerIdx) == E_OK )
  {
    /* #200 Remove element from transmission queue. */
    /*@ assert msgHandlerIdx < DoIPInt_GetSizeOfMsgHandler(); */                                                       /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
    DoIPInt_TxQueue_Remove(ConnectionIdx, handleIdx, msgHandlerIdx);

    /* #201 Finish transmission. */
    DoIPInt_Tx_ForwardConfirmation(ConnectionIdx, handleIdx, msgHandlerIdx, E_NOT_OK);
  }
} /* DoIPInt_Tx_Terminate() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_CopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Tx_CopyTxData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if connection is opened. */
  if ( DoIPInt_Connection_IsOpened(ConnectionIdx) == TRUE )
  {
    /* #20 Try to copy message. */
    retVal = DoIPInt_Tx_CopyMessage(ConnectionIdx, PduInfoPtr, AvailableDataPtr);
  }

  return retVal;
} /* DoIPInt_Tx_CopyTxData() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_Confirmation()
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
FUNC(void, DOIPINT_CODE) DoIPInt_Tx_Confirmation(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  Std_ReturnType Result)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle confirmation depending on transmission state. */
  switch ( DoIPInt_GetTxStateOfConnectionDyn(ConnectionIdx) )
  {
    /* #20 Confirm transmission while transmission is pending. */
    case DOIPINT_TX_STATE_GEN_HDR:
    case DOIPINT_TX_STATE_LIGHT_HDR:
    case DOIPINT_TX_STATE_USER_DATA:
    {
      const uint32                       handleIdx = DoIPInt_GetTxHandleIdxOfConnectionDyn(ConnectionIdx);
      const DoIPInt_SizeOfMsgHandlerType msgHandlerIdx = DoIPInt_GetMsgHandlerTxIdxOfConnectionDyn(ConnectionIdx);

      /* #30 Set transmission state to indicate that no transmission is pending. */
      /* Reset offset. */
      DoIPInt_SetTxOffsetOfConnectionDyn(ConnectionIdx, 0u);
      DoIPInt_SetTxStateOfConnectionDyn(ConnectionIdx, DOIPINT_TX_STATE_NONE);

      /* #40 Remove element from transmission queue. */
      /*@ assert msgHandlerIdx < DoIPInt_GetSizeOfMsgHandler(); */                                                     /* VCA_DOIPINT_VALID_HANDLE_IDX_MSG_HANDLER_IDX */
      DoIPInt_TxQueue_Remove(ConnectionIdx, handleIdx, msgHandlerIdx);

      /* #50 Forward TxConfirmation. */
      DoIPInt_Tx_ForwardConfirmation(ConnectionIdx, handleIdx, msgHandlerIdx, Result);
      break;
    }
    default: /* DOIPINT_TX_STATE_NONE */
    {
      /* Nothing to do. */
      break;
    }
  }
} /* DoIPInt_Tx_Confirmation() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_IsTransmissionPending()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DOIPINT_CODE) DoIPInt_Tx_IsTransmissionPending(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32                       handleIdx = 0u;
  PduLengthType                pduLength = 0u;
  DoIPInt_SizeOfMsgHandlerType msgHandlerIdx = 0u;
  boolean                      retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Request if connection has elements in transmission queue. */
  if ( DoIPInt_TxQueue_GetNext(ConnectionIdx, &handleIdx, &pduLength, &msgHandlerIdx) == E_OK )
  {
    retVal = TRUE;
  }

  return retVal;
} /* DoIPInt_Tx_IsTransmissionPending() */

/**********************************************************************************************************************
 *  DoIPInt_Tx_Cancel()
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
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Tx_Cancel(
  DoIPInt_SizeOfChannelType ChannelIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Only proceed if a transmission request for the given channel exists. */
  if ( DoIPInt_TxQueue_IsChannelInQueue(ChannelIdx) == TRUE )
  {
    const DoIPInt_SizeOfConnectionType connectionIdx = DoIPInt_GetConnectionIdxOfChannel(ChannelIdx);
    const uint32                       handleIdx = DoIPInt_GetTxHandleIdxOfConnectionDyn(connectionIdx);
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    const DoIPInt_SizeOfMsgHandlerType msgHandlerIdx = DoIPInt_GetMsgHandlerIdxOfDiagMsgHandler(0u);
#else
    const DoIPInt_SizeOfMsgHandlerType msgHandlerIdx = DoIPInt_GetMsgHandlerIdxOfLightMsgHandler(0u);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

    /* #20 Check if transmission is already in progress for the requested channel. */
    if ( (handleIdx == ChannelIdx)
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      && (DoIPInt_GetMsgHandlerTxIdxOfConnectionDyn(connectionIdx) == msgHandlerIdx)
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
       )
    {
      /* #30 Check if transmission is pending on the connection of the requested channel. */
      if ( DoIPInt_GetTxStateOfConnectionDyn(connectionIdx) != DOIPINT_TX_STATE_NONE )
      {
        /* #300 Forward cancellation to SoAd. */
        retVal = SoAd_TpCancelTransmit(DoIPInt_GetSoAdTxPduIdOfConnection(connectionIdx));
      }
      /* #31 Remove tx request from queue silently otherwise. */
      else
      {
        DoIPInt_TxQueue_Remove(connectionIdx, ChannelIdx, msgHandlerIdx);

        /* #310 Forward negative Tx confirmation. */
        DoIPInt_Tx_ForwardConfirmation(connectionIdx, ChannelIdx, msgHandlerIdx, E_NOT_OK);

        retVal = E_OK;
      }
    }
    /* #21 Remove tx request from queue silently otherwise. */
    else
    {
      DoIPInt_TxQueue_Remove(connectionIdx, ChannelIdx, msgHandlerIdx);

      /* #210 Forward negative Tx confirmation. */
      DoIPInt_Tx_ForwardConfirmation(connectionIdx, ChannelIdx, msgHandlerIdx, E_NOT_OK);

      retVal = E_OK;
    }
  }

  return retVal;
} /* DoIPInt_Tx_Cancel() */

#define DOIPINT_STOP_SEC_CODE
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Tx.c
 *********************************************************************************************************************/
