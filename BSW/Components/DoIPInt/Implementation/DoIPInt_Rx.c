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
/*!        \file  DoIPInt_Rx.c
 *        \brief  DoIPInt source file for sub-module DoIP_Rx.
 *      \details  Implementation for sub-module DoIP_Rx of component DoIPInt.
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

#define DOIPINT_RX_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "DoIPInt.h"
#include "DoIPInt_Priv.h"
#include "DoIPInt_Rx.h"
#include "DoIPInt_Connection.h"
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
#include "DoIPInt_GenHdr.h"
#include "DoIPInt_DiagMsg.h"
#include "DoIPInt_OemSpecific.h"
#else
#include "DoIPInt_LightHdr.h"
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
#include "DoIPInt_Util.h"
#include "SoAd.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DOIPINT_GEN_LIGHT_HDR_LEN                     (8u)       /*!< Length of the generic and light header. */

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

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
/**********************************************************************************************************************
 *  DoIPInt_Rx_CopyUserData()
 *********************************************************************************************************************/
/*! \brief       Copies user data and validates it in case connection close is not requested.
 *  \details     -
 *  \param[in]   ConnectionIdx                Connection index.
 *                                            [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]   PduInfoPtr                   Provides the buffer and the number of bytes to be copied.
 *                                            [range: PduInfoPtr != NULL_PTR,
 *                                                    PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                    PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]  AvailableDataPtr             Indicates the remaining number of bytes that are available to be copied.
 *                                            [range: AvailableDataPtr != NULL_PTR]
 *  \return      DOIPINT_MSG_HANDLER_PENDING  The message handler has not yet finished processing the message.
 *  \return      DOIPINT_MSG_HANDLER_FINISHED The message handler has finished processing the message.
 *  \return      DOIPINT_MSG_HANDLER_SKIP     The message shall be skipped.
 *  \return      DOIPINT_MSG_HANDLER_FAILED   Handling the message failed.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE for same ConnectionIdx, TRUE otherwise
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Rx_CopyUserData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_Rx_ProcessStateRxGenHdr()
 *********************************************************************************************************************/
/*! \brief        Processes the reception state while being in state DOIPINT_RX_STATE_GEN_HDR.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    MsgHandlerRetVal        Return value of the previously called message handler.
 *                                        [range: DOIPINT_MSG_HANDLER_PENDING, DOIPINT_MSG_HANDLER_FINISHED,
 *                                                DOIPINT_MSG_HANDLER_SKIP, DOIPINT_MSG_HANDLER_FAILED]
 *  \return       BUFREQ_OK               Receiving the message may be continued.
 *  \return       BUFREQ_E_NOT_OK         Receiving the message failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_ProcessStateRxGenHdr(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_MsgHndl_RetType MsgHandlerRetVal);

/**********************************************************************************************************************
 *  DoIPInt_Rx_ProcessStateRxUserData()
 *********************************************************************************************************************/
/*! \brief        Processes the reception state while being in state DOIPINT_RX_STATE_USER_DATA.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    MsgHandlerRetVal        Return value of the previously called message handler.
 *                                        [range: DOIPINT_MSG_HANDLER_PENDING, DOIPINT_MSG_HANDLER_FINISHED,
 *                                                DOIPINT_MSG_HANDLER_SKIP, DOIPINT_MSG_HANDLER_FAILED]
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: AvailableDataPtr != NULL_PTR]
 *  \return       BUFREQ_OK               Receiving the message may be continued.
 *  \return       BUFREQ_E_NOT_OK         Receiving the message failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_ProcessStateRxUserData(
    DoIPInt_SizeOfConnectionType ConnectionIdx,
    DoIPInt_MsgHndl_RetType MsgHandlerRetVal,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT )
/**********************************************************************************************************************
 *  DoIPInt_Rx_ProcessStateRxLightHdr()
 *********************************************************************************************************************/
/*! \brief        Processes the reception state while being in state DOIPINT_RX_STATE_LIGHT_HDR.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    MsgHandlerRetVal        Return value of the previously called message handler.
 *                                        [range: DOIPINT_MSG_HANDLER_PENDING, DOIPINT_MSG_HANDLER_FINISHED,
 *                                                DOIPINT_MSG_HANDLER_SKIP, DOIPINT_MSG_HANDLER_FAILED]
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: AvailableDataPtr != NULL_PTR]
 *  \return       BUFREQ_OK               Receiving the message may be continued.
 *  \return       BUFREQ_E_NOT_OK         Receiving the message failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_ProcessStateRxLightHdr(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_MsgHndl_RetType MsgHandlerRetVal,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT */

/**********************************************************************************************************************
 *  DoIPInt_Rx_Skip()
 *********************************************************************************************************************/
/*! \brief        Skips reception of data until DoIP message is completely received.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: AvailableDataPtr != NULL_PTR]
 *  \return       BUFREQ_OK               Data has been copied (skipped) completely as requested.
 *  \return       BUFREQ_E_NOT_OK         Data has not been copied (skipped). Request failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_Skip(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_Rx_StartParse()
 *********************************************************************************************************************/
/*! \brief        Starts parsing on the specified connection by stopping a current active parsing.
 *  \details      -
 *  \param[in]    ConnectionIdx         Connection index.
 *                                      [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[out]   AvailableDataPtr      Indicates the remaining number of bytes that are available to be copied.
 *                                      [range: AvailableDataPtr != NULL_PTR]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_Rx_StartParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
/**********************************************************************************************************************
 *  DoIPInt_Rx_CopyUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Rx_CopyUserData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const PduLengthType     payloadLength = (PduLengthType)DoIPInt_GenHdr_RxGetParsedPayloadLength(ConnectionIdx);
  const uint16            payloadType = DoIPInt_GenHdr_RxGetParsedPayloadType(ConnectionIdx);
  DoIPInt_MsgHndl_RetType retVal = DOIPINT_MSG_HANDLER_FAILED;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy diagnostic user data when a diagnostic message is received. */
  if ( payloadType == DOIPINT_PAYLOAD_TYPE_DIAG_MSG )
  {
    retVal = DoIPInt_DiagMsg_RxCopy(ConnectionIdx, payloadLength, PduInfoPtr, AvailableDataPtr);
  }
  /* #11 Copy OEM specific user data otherwise. */
  else
  {
    retVal = DoIPInt_OemSpecific_RxCopy(ConnectionIdx, payloadLength, payloadType, PduInfoPtr, AvailableDataPtr);
  }

  return retVal;
} /* DoIPInt_Rx_CopyUserData() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_ProcessStateRxGenHdr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_ProcessStateRxGenHdr(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_MsgHndl_RetType MsgHandlerRetVal)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle return value and state depending on the message handler return value. */
  switch ( MsgHandlerRetVal )
  {
    /* #20 Return BUFREQ_OK when message handling is pending. */
    case DOIPINT_MSG_HANDLER_PENDING:
    {
      retVal = BUFREQ_OK;
      break;
    }
    /* #30 Return BUFREQ_OK when message handling has finished and switch to next state to copy user data. */
    case DOIPINT_MSG_HANDLER_FINISHED:
    {
      DoIPInt_SetRxStateOfConnectionDyn(ConnectionIdx, DOIPINT_RX_STATE_USER_DATA);
      retVal = BUFREQ_OK;
      break;
    }
    /* #40 Switch to next state to skip message if required. */
    case DOIPINT_MSG_HANDLER_SKIP:
    {
      DoIPInt_SetRxStateOfConnectionDyn(ConnectionIdx, DOIPINT_RX_STATE_SKIP);
      retVal = BUFREQ_OK;
      break;
    }
    default: /* DOIPINT_MSG_HANDLER_FAILED */
    {
      retVal = BUFREQ_E_NOT_OK;
      break;
    }
  }

  return retVal;
} /* DoIPInt_Rx_ProcessStateRxGenHdr() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_ProcessStateRxUserData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_ProcessStateRxUserData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_MsgHndl_RetType MsgHandlerRetVal,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle return value and state depending on the message handler return value. */
  switch ( MsgHandlerRetVal )
  {
    /* #20 Return BUFREQ_OK when message handling is pending. */
    case DOIPINT_MSG_HANDLER_PENDING:
    {
      retVal = BUFREQ_OK;
      break;
    }
    /* #30 Return BUFREQ_OK when message handling has finished and start parsing for next header. */
    case DOIPINT_MSG_HANDLER_FINISHED:
    {
      DoIPInt_Rx_StartParse(ConnectionIdx, AvailableDataPtr);

      DoIPInt_SetRxStateOfConnectionDyn(ConnectionIdx, DOIPINT_RX_STATE_GEN_HDR);

      retVal = BUFREQ_OK;
      break;
    }
    /* #40 Switch to next state to skip message if required. */
    case DOIPINT_MSG_HANDLER_SKIP:
    {
      DoIPInt_SetRxStateOfConnectionDyn(ConnectionIdx, DOIPINT_RX_STATE_SKIP);
      retVal = BUFREQ_OK;
      break;
    }
    default: /* DOIPINT_MSG_HANDLER_FAILED */
    {
      retVal = BUFREQ_E_NOT_OK;
      break;
    }
  }

  return retVal;
} /* DoIPInt_Rx_ProcessStateRxUserData() */
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT )
/**********************************************************************************************************************
 *  DoIPInt_Rx_ProcessStateRxLightHdr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_ProcessStateRxLightHdr(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_MsgHndl_RetType MsgHandlerRetVal,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle return value and state depending on the message handler return value. */
  switch ( MsgHandlerRetVal )
  {
    /* #20 Return BUFREQ_OK when message handling is pending. */
    case DOIPINT_MSG_HANDLER_PENDING:
    {
      retVal = BUFREQ_OK;
      break;
    }
    /* #30 Return BUFREQ_OK when message handling has finished and start parsing for next header. */
    case DOIPINT_MSG_HANDLER_FINISHED:
    {
      DoIPInt_Rx_StartParse(ConnectionIdx, AvailableDataPtr);

      /* No switching of state required in case of light header */
      retVal = BUFREQ_OK;
      break;
    }
    /* #40 Switch to next state to skip message if required. */
    case DOIPINT_MSG_HANDLER_SKIP:
    {
      DoIPInt_SetRxStateOfConnectionDyn(ConnectionIdx, DOIPINT_RX_STATE_SKIP);
      retVal = BUFREQ_OK;
      break;
    }
    default: /* DOIPINT_MSG_HANDLER_FAILED */
    {
      retVal = BUFREQ_E_NOT_OK;
      break;
    }
  }

  return retVal;
} /* DoIPInt_Rx_ProcessStateRxLightHdr() */
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT */

/**********************************************************************************************************************
 *  DoIPInt_Rx_Skip()
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
DOIPINT_LOCAL_INLINE FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_Skip(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
  uint64            msgLength = (uint64)DoIPInt_GenHdr_RxGetParsedPayloadLength(ConnectionIdx) +
    DOIPINT_GEN_LIGHT_HDR_LEN;
#else
  uint64            msgLength = (uint64)DoIPInt_LightHdr_RxGetParsedPayloadLength(ConnectionIdx) +
    DOIPINT_GEN_LIGHT_HDR_LEN;
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
  uint64            offset = DoIPInt_GetRxOffsetOfConnectionDyn(ConnectionIdx);
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if data to be copied does not exceed the pending data length. */
  if ( offset <= msgLength )
  {
    /* #20 Check if data to be skipped is received completely. */
    if ( offset == msgLength )
    {
      /* #200 Start parsing for next header. */
      DoIPInt_Rx_StartParse(ConnectionIdx, AvailableDataPtr);

      /* #201 Set reception state according to configured header. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      DoIPInt_SetRxStateOfConnectionDyn(ConnectionIdx, DOIPINT_RX_STATE_GEN_HDR);
#else
      DoIPInt_SetRxStateOfConnectionDyn(ConnectionIdx, DOIPINT_RX_STATE_LIGHT_HDR);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
    }
    /* #21 Prepare to continue reception otherwise. */
    else
    {
      /* #210 Set available buffer size to the remaining data length or to the supported PDU length. */
      *AvailableDataPtr = DoIPInt_Util_GetSupportedPduLength((uint32)(msgLength - offset));
    }

    retVal = BUFREQ_OK;
  }

  return retVal;
} /* DoIPInt_Rx_Skip() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_StartParse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_Rx_StartParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset reception offset on connection. */
  DoIPInt_SetRxOffsetOfConnectionDyn(ConnectionIdx, 0u);

  /* #20 Start parsing for the different units. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
  DoIPInt_GenHdr_RxStartParse(ConnectionIdx, AvailableDataPtr);
  DoIPInt_DiagMsg_RxStartParse(ConnectionIdx);
  DoIPInt_OemSpecific_RxStartParse(ConnectionIdx);
#else
  DoIPInt_LightHdr_RxStartParse(ConnectionIdx, AvailableDataPtr);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
} /* DoIPInt_Rx_StartParse() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DoIPInt_Rx_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_Init(
  DoIPInt_ConnectionIterType ConnectionIter)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize reception state. */
  DoIPInt_SetRxStateOfConnectionDyn(ConnectionIter, DOIPINT_RX_STATE_NONE);

  /* #20 Initialize retry timeout. */
  DoIPInt_SetRxRetryTimerOfConnectionDyn(ConnectionIter, 0uL);

  /* #30 Initialize reception offset and Rx pending flag. */
  DoIPInt_SetRxOffsetOfConnectionDyn(ConnectionIter, 0u);
  DoIPInt_SetRxPendingOfConnectionDyn(ConnectionIter, FALSE);
} /* DoIPInt_Rx_Init() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_ProcessRetryTimeout()
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
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_ProcessRetryTimeout(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_ConnectionIterType connectionIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all connections. */
  for ( connectionIter = 0u; connectionIter < DoIPInt_GetSizeOfConnection(); connectionIter++ )
  {
    /* #20 Enter critical section to prevent inconsistencies of the retry timeout. */
    DOIPINT_BEGIN_CRITICAL_SECTION();

    /* #30 Check if reception retry is pending. */
    if ( DoIPInt_GetRxRetryTimerOfConnectionDyn(connectionIter) > 1uL )
    {
      /* #40 Decrement retry timeout. */
      DoIPInt_DecRxRetryTimerOfConnectionDyn(connectionIter);
    }

    /* #50 Leave critical section. */
    DOIPINT_END_CRITICAL_SECTION();
  }
} /* DoIPInt_Rx_ProcessRetryTimeout() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_Terminate()
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
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_Terminate(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle termination depending on reception state. */
  switch ( DoIPInt_GetRxStateOfConnectionDyn(ConnectionIdx) )
  {
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    /* #100 Terminate reception while generic header reception. */
    case DOIPINT_RX_STATE_GEN_HDR:
    {
      /* #1000 Stop parsing generic header. */
      DoIPInt_GenHdr_RxStopParse(ConnectionIdx);
      break;
    }
    /* #101 Terminate reception while user data are copied. */
    case DOIPINT_RX_STATE_USER_DATA:
    {
      /* #1010 Stop parsing generic header first. */
      DoIPInt_GenHdr_RxStopParse(ConnectionIdx);

      /* #1011 Terminate reception for diagnostic message. */
      if ( DoIPInt_GenHdr_RxGetParsedPayloadType(ConnectionIdx) == DOIPINT_PAYLOAD_TYPE_DIAG_MSG )
      {
        DoIPInt_DiagMsg_RxStopParse(ConnectionIdx, TRUE);
      }
      /* #1012 Terminate reception for OEM specific message otherwise. */
      else
      {
        DoIPInt_OemSpecific_RxStopParse(ConnectionIdx);
      }
      break;
    }
#else
    /* #102 Terminate reception when light header data is copied. */
    case DOIPINT_RX_STATE_LIGHT_HDR:
    {
      DoIPInt_LightHdr_RxStopParse(ConnectionIdx, TRUE);
      break;
    }
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
    /* #103 Terminate reception while skipping message or no reception is active. */
    case DOIPINT_RX_STATE_SKIP:
    default: /* DOIPINT_RX_STATE_NONE */
    {
      /* Nothing to do. */
      break;
    }
  }

  /* #20 Set reception state to indicate that no reception is pending. */
  DoIPInt_SetRxStateOfConnectionDyn(ConnectionIdx, DOIPINT_RX_STATE_NONE);

} /* DoIPInt_Rx_Terminate() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_StartOfReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_StartOfReception(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if no reception is ongoing. */
  if ( DoIPInt_GetRxStateOfConnectionDyn(ConnectionIdx) == DOIPINT_RX_STATE_NONE )
  {
    /* #20 Check if connection is opened and not requested to be closed. */
    if ( DoIPInt_Connection_IsClosedOrRequested(ConnectionIdx) == FALSE )
    {
      /* #30 Start parsing of header. */
      DoIPInt_Rx_StartParse(ConnectionIdx, AvailableDataPtr);

      /* #40 Set reception state according to configured header. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      DoIPInt_SetRxStateOfConnectionDyn(ConnectionIdx, DOIPINT_RX_STATE_GEN_HDR);
#else
      DoIPInt_SetRxStateOfConnectionDyn(ConnectionIdx, DOIPINT_RX_STATE_LIGHT_HDR);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
      retVal = BUFREQ_OK;
    }
  }

  return retVal;
} /* DoIPInt_Rx_StartOfReception() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_CheckOrStartRetryTimeout()
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
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Rx_CheckOrStartRetryTimeout(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent inconsistencies of the retry timeout. */
  DOIPINT_BEGIN_CRITICAL_SECTION();

  /* #20 Stop timeout monitoring if timeout has expired. */
  if ( DoIPInt_GetRxRetryTimerOfConnectionDyn(ConnectionIdx) == 1uL )
  {
    DoIPInt_SetRxRetryTimerOfConnectionDyn(ConnectionIdx, 0uL);
    retVal = E_NOT_OK;
  }
  /* #30 Start timeout if it has not been started yet otherwise. */
  else if ( DoIPInt_GetRxRetryTimerOfConnectionDyn(ConnectionIdx) == 0uL )
  {
    /* #40 Assert that the connection is still open before starting the timeout. */
    if ( DoIPInt_Connection_IsOpened(ConnectionIdx) == TRUE )
    {
      DoIPInt_SetRxRetryTimerOfConnectionDyn(ConnectionIdx, (DoIPInt_GetRxRetryTimeout() + 1uL));
    }
    else
    {
      /* The retry timer is not started in case the connection is no longer open or requested to be closed and E_OK is
       * returned. The closing of the connection is handled in a different context and therefore no error handling in
       * the current context is required. */
    }
  }
  else
  {
    /* Nothing to do */
  }

  /* #50 Leave critical section. */
  DOIPINT_END_CRITICAL_SECTION();

  return retVal;
} /* DoIPInt_Rx_CheckOrStartRetryTimeout() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_ResetRetryTimeout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_ResetRetryTimeout(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset the retry timeout. */
  DoIPInt_SetRxRetryTimerOfConnectionDyn(ConnectionIdx, 0uL);
} /* DoIPInt_Rx_ResetRetryTimeout() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_SetPending()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_SetPending(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent inconsistencies of the pending reception and the socket connection state. */
  DOIPINT_BEGIN_CRITICAL_SECTION();

  /* #20 Assert that the connection has not been closed in the meantime. */
  if ( DoIPInt_Connection_IsOpened(ConnectionIdx) == TRUE )
  {
    /* #30 Set reception to pending for the connection. */
    DoIPInt_SetRxPendingOfConnectionDyn(ConnectionIdx, TRUE);
  }

  /* #40 Leave critical section. */
  DOIPINT_END_CRITICAL_SECTION();
} /* DoIPInt_Rx_SetPending() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_ResetPending()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_ResetPending(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset reception pending flag of the connection. */
  DoIPInt_SetRxPendingOfConnectionDyn(ConnectionIdx, FALSE);
} /* DoIPInt_Rx_ResetPending() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_CopyRxData()
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
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_Rx_CopyRxData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_RxStateType     lastRxState;
  PduInfoType             pduInfo;
  BufReq_ReturnType       retVal = BUFREQ_E_NOT_OK;
  DoIPInt_MsgHndl_RetType msgHandlerRetVal;

  /* ----- Implementation ----------------------------------------------- */
  pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
  pduInfo.SduLength = PduInfoPtr->SduLength;

  /* #10 Check if connection is opened. */
  if ( DoIPInt_Connection_IsOpened(ConnectionIdx) == TRUE )
  {
    /* #20 Increase the reception offset. */
    DoIPInt_SetRxOffsetOfConnectionDyn(ConnectionIdx, DoIPInt_GetRxOffsetOfConnectionDyn(ConnectionIdx) +
      pduInfo.SduLength);

    /* #30 Process reception state machine until the state does not change or the last state was DOIPINT_RX_STATE_SKIP
     *     or DOIPINT_RX_STATE_USER_DATA. */
    do
    {
      lastRxState = DoIPInt_GetRxStateOfConnectionDyn(ConnectionIdx);

      /* #40 Handle received data depending on reception state. */
      switch ( lastRxState )
      {
        /* #400 Copy header (ISO header enabled). */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
        case DOIPINT_RX_STATE_GEN_HDR:
        {
          /* #4000 Check that close is not requested for the connection. */
          if ( DoIPInt_Connection_IsClosedOrRequested(ConnectionIdx) == FALSE )
          {
            /* #40000 Call generic header message handler. */
            msgHandlerRetVal = DoIPInt_GenHdr_RxCopy(ConnectionIdx, &pduInfo, AvailableDataPtr);

            /* #40001 Perform state transition dependent on generic header message handler feedback. */
            retVal = DoIPInt_Rx_ProcessStateRxGenHdr(ConnectionIdx, msgHandlerRetVal);
          }
          /* #4001 Return BUFREQ_OK without copying data and set the available data to zero otherwise. */
          else
          {
            *AvailableDataPtr = 0u;
            retVal = BUFREQ_OK;
          }
          break;
        }
        /* #401 Copy user data (ISO header enabled). */
        case DOIPINT_RX_STATE_USER_DATA:
        {
          /* #4010 Check that close is not requested for the connection. */
          if ( DoIPInt_Connection_IsClosedOrRequested(ConnectionIdx) == FALSE )
          {
            /* #40100 Call user data message handler. */
            msgHandlerRetVal = DoIPInt_Rx_CopyUserData(ConnectionIdx, &pduInfo, AvailableDataPtr);

            /* #40101 Perform state transition dependent on user data message handler feedback. */
            retVal = DoIPInt_Rx_ProcessStateRxUserData(ConnectionIdx, msgHandlerRetVal, AvailableDataPtr);
          }
          /* #4011 Return BUFREQ_OK without copying data and set the available data to zero otherwise. */
          else
          {
            *AvailableDataPtr = 0u;
            retVal = BUFREQ_OK;
          }
          break;
        }
#else
        /* #402 Copy message (LIGHT header enabled). */
        case DOIPINT_RX_STATE_LIGHT_HDR:
        {
          /* #4020 Check that close is not requested for the connection. */
          if ( DoIPInt_Connection_IsClosedOrRequested(ConnectionIdx) == FALSE )
          {
            /* #40200 Call light header message handler. */
            msgHandlerRetVal = DoIPInt_LightHdr_RxCopy(ConnectionIdx, &pduInfo, AvailableDataPtr);

            /* #40201 Perform state transition dependent on light header message handler feedback. */
            retVal = DoIPInt_Rx_ProcessStateRxLightHdr(ConnectionIdx, msgHandlerRetVal, AvailableDataPtr);
          }
          /* #4021 Return BUFREQ_OK without copying data and set the available data to zero otherwise. */
          else
          {
            *AvailableDataPtr = 0u;
            retVal = BUFREQ_OK;
          }
          break;
        }
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
        /* #403 Skip data. */
        case DOIPINT_RX_STATE_SKIP:
        {
          retVal = DoIPInt_Rx_Skip(ConnectionIdx, AvailableDataPtr);
          break;
        }
        /* #404 Reject if no reception is active. */
        default: /* DOIPINT_RX_STATE_NONE */
        {
          break;
        }
      }
    } while ( (DoIPInt_GetRxStateOfConnectionDyn(ConnectionIdx) != lastRxState) &&
      (lastRxState != DOIPINT_RX_STATE_SKIP)
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      && (lastRxState != DOIPINT_RX_STATE_USER_DATA)
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
      );
  }

  return retVal;
} /* DoIPInt_Rx_CopyRxData() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_ContinueCopyRxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_ContinueCopyRxData(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_ConnectionIterType connectionIter;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all connections. */
  for ( connectionIter = 0u; connectionIter < DoIPInt_GetSizeOfConnection(); connectionIter++ )
  {
    DoIPInt_SizeOfConnectionType connectionIdx = (DoIPInt_SizeOfConnectionType)connectionIter;

    /* #20 Check if any reception is pending on the connection. */
    if ( DoIPInt_IsRxPendingOfConnectionDyn(connectionIdx) )
    {
      PduInfoType   pduInfo;
      PduLengthType availableData = 0u;

      pduInfo.SduLength = 0u;
      pduInfo.SduDataPtr = NULL_PTR;

      /* #30 Copy the buffered data. */
      (void)DoIPInt_Rx_CopyRxData(connectionIdx, &pduInfo, &availableData);
    }
  }
} /* DoIPInt_Rx_ContinueCopyRxData() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_Indication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Rx_Indication(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Ignore call since it has the same meaning like DoIPInt_Rx_Terminate on socket connection mode change. */
  DOIPINT_DUMMY_STATEMENT(ConnectionIdx); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
} /* DoIPInt_Rx_Indication() */

/**********************************************************************************************************************
 *  DoIPInt_Rx_Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Rx_Cancel(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward reception cancellation request to SoAd. */
  retVal = SoAd_TpCancelReceive(DoIPInt_GetSoAdRxPduIdOfConnection(ConnectionIdx));

  return retVal;
} /* DoIPInt_Rx_Cancel() */

#define DOIPINT_STOP_SEC_CODE
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Rx.c
 *********************************************************************************************************************/
