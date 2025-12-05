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
/*!        \file  DoIPInt_Event.c
 *        \brief  DoIPInt source file for sub-module DoIPInt_Event.
 *      \details  Implementation for sub-module DoIPInt_Event of component DoIPInt.
 *                Sub-module implements handling of events in reception context.
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

#define DOIPINT_EVENT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "DoIPInt.h"
#include "DoIPInt_Event.h"
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
# include "DoIPInt_GenHdr.h"
# include "DoIPInt_DiagMsg.h"
# include "DoIPInt_OemSpecific.h"
#else
# include "DoIPInt_LightHdr.h"
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# if !defined (DOIPINT_LOCAL)
#  define DOIPINT_LOCAL static
# endif /* !defined (DOIPINT_LOCAL) */

# if !defined (DOIPINT_LOCAL_INLINE)
#  define DOIPINT_LOCAL_INLINE LOCAL_INLINE
# endif /* !defined (DOIPINT_LOCAL_INLINE) */

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
# define DOIPINT_START_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
/**********************************************************************************************************************
 *  DoIPInt_Event_HandleGenHdrInvPatternEvent()
 *********************************************************************************************************************/
/*! \brief        Handles event reporting of an invalid pattern.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       DOIPINT_MSG_HANDLER_FINISHED Message handling shall be finished.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message handling shall be skipped.
 *  \return       DOIPINT_MSG_HANDLER_FAILED   Message handling failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleGenHdrInvPatternEvent(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleGenHdrSkipEvents()
 *********************************************************************************************************************/
/*! \brief        Handles all diagnostic message events which require a skipping of the message.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    Event                        Type of event.
 *                                             [range: DOIPINT_EVENT_MSG_TOO_LARGE, DOIPINT_EVENT_MSG_INV_LENGTH]
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message handling shall be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleGenHdrSkipEvents(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event);

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleOemMsgReceivedUpToLenEvent()
 *********************************************************************************************************************/
/*! \brief        Handles the event when an OEM message has been received up to the configured length.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection(),
 *                                                     DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx)]
 *  \return       DOIPINT_MSG_HANDLER_PENDING  Message handling shall be continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message handling shall be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx);
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleOemMsgReceivedUpToLenEvent(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleDiagMsgReceivedUpToLenEvent()
 *********************************************************************************************************************/
/*! \brief        Handles the event when a diagnostic message has been received up to the configured length.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       DOIPINT_MSG_HANDLER_PENDING  Message handling shall be continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message handling shall be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleDiagMsgReceivedUpToLenEvent(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleOemMsgDiscardedEvent()
 *********************************************************************************************************************/
/*! \brief        Handles the event when an OEM specific message has been discarded.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message handling shall be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleOemMsgDiscardedEvent(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleDiagMsgSkipEvents()
 *********************************************************************************************************************/
/*! \brief        Handles all diagnostic message events which require a skipping of the message.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    Event                        Type of event.
 *                                             [range: DOIPINT_EVENT_DIAG_MSG_INV_REM_ADDR,
                                                       DOIPINT_EVENT_DIAG_MSG_INV_LOCAL_ADDR,
                                                       DOIPINT_EVENT_DIAG_MSG_DISCARDED]
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message handling shall be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleDiagMsgSkipEvents(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event);

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleDiagMsgForwarded()
 *********************************************************************************************************************/
/*! \brief        Handles the event when a diagnostic message has been forwarded.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       DOIPINT_MSG_HANDLER_FINISHED Message handling shall be finished.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleDiagMsgForwarded(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Event_ForwardGenHdrData()
 *********************************************************************************************************************/
/*! \brief        Gets the generic header data and forwards it via the configured callback.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    Event                        Type of event.
 *                                             [range: DOIPINT_EVENT_MSG_INV_PATTERN, DOIPINT_EVENT_MSG_TOO_LARGE,
 *                                                     DOIPINT_EVENT_MSG_INV_LENGTH, DOIPINT_EVENT_MSG_DISCARDED]
 *  \return       E_OK                         Return value from callback function. Interpretation depends on event.
 *  \return       E_NOT_OK                     Return value from callback function. Interpretation depends on event.
 *  \pre          DoIPInt_HasEventCbk()
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires DoIPInt_HasEventCbk();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Event_ForwardGenHdrData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event);

/**********************************************************************************************************************
 *  DoIPInt_Event_ForwardOemSpecificData()
 *********************************************************************************************************************/
/*! \brief        Gets the generic header and OEM specific data and forwards it via the configured callback.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection(),
 *                                                     DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx)]
 *  \param[in]    Event                        Type of event.
 *                                             [range: DOIPINT_EVENT_MSG_RECEIVED_UP_TO_LENGTH]
 *  \return       E_OK                         Return value from callback function. Interpretation depends on event.
 *  \return       E_NOT_OK                     Return value from callback function. Interpretation depends on event.
 *  \pre          DoIPInt_HasEventCbk()
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx);
 *    requires DoIPInt_HasEventCbk();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Event_ForwardOemSpecificData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event);

/**********************************************************************************************************************
 *  DoIPInt_Event_ForwardDiagMsgData()
 *********************************************************************************************************************/
/*! \brief        Gets the generic header and diagnostic message data and forwards it via the configured callback.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    Event                        Type of event.
 *                                             [range: DOIPINT_EVENT_MSG_RECEIVED_UP_TO_LENGTH,
 *                                                     DOIPINT_EVENT_DIAG_MSG_INV_REM_ADDR,
 *                                                     DOIPINT_EVENT_DIAG_MSG_INV_LOCAL_ADDR,
 *                                                     DOIPINT_EVENT_DIAG_MSG_DISCARDED,
 *                                                     DOIPINT_EVENT_DIAG_MSG_FORWARDED]
 *  \return       E_OK                         Return value from callback function. Interpretation depends on event.
 *  \return       E_NOT_OK                     Return value from callback function. Interpretation depends on event.
 *  \pre          DoIPInt_HasEventCbk()
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires DoIPInt_HasEventCbk();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Event_ForwardDiagMsgData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT )
/**********************************************************************************************************************
 *  DoIPInt_Event_HandleLightMsgReceivedUpToLenEvent()
 *********************************************************************************************************************/
/*! \brief        Handles the event when a light message has been received up to the configured length.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       DOIPINT_MSG_HANDLER_PENDING  Message handling shall be continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message handling shall be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleLightMsgReceivedUpToLenEvent(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleLightMsgSkipEvents()
 *********************************************************************************************************************/
/*! \brief        Handles all light message events which require a skipping of the message.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    Event                        Type of event.
 *                                             [range: DOIPINT_EVENT_MSG_TOO_LARGE,
 *                                                     DOIPINT_EVENT_DIAG_MSG_INV_REM_ADDR,
 *                                                     DOIPINT_EVENT_DIAG_MSG_INV_LOCAL_ADDR,
 *                                                     DOIPINT_EVENT_DIAG_MSG_DISCARDED]
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message handling shall be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleLightMsgSkipEvents(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event);

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleLightMsgForwarded()
 *********************************************************************************************************************/
/*! \brief        Handles the event when a light message has been forwarded.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       DOIPINT_MSG_HANDLER_FINISHED Message handling shall be finished.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleLightMsgForwarded(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_Event_ForwardLightMsgData()
 *********************************************************************************************************************/
/*! \brief        Gets the light message data and forwards the data via the configured callback.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    Event                        Type of event.
 *                                             [range: DOIPINT_EVENT_MSG_RECEIVED_UP_TO_LENGTH,
 *                                                     DOIPINT_EVENT_DIAG_MSG_INV_REM_ADDR,
 *                                                     DOIPINT_EVENT_DIAG_MSG_INV_LOCAL_ADDR,
 *                                                     DOIPINT_EVENT_DIAG_MSG_DISCARDED,
 *                                                     DOIPINT_EVENT_DIAG_MSG_FORWARDED]
 *  \return       E_OK                         Return value from callback function. Interpretation depends on event.
 *  \return       E_NOT_OK                     Return value from callback function. Interpretation depends on event.
 *  \pre          DoIPInt_HasEventCbk()
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires DoIPInt_HasEventCbk();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Event_ForwardLightMsgData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
/**********************************************************************************************************************
 *  DoIPInt_Event_HandleGenHdrInvPatternEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleGenHdrInvPatternEvent(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal; /* Not initialized: Improved readability. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward event in case an event callback is configured. */
  if ( DoIPInt_HasEventCbk() )
  {
    if ( DoIPInt_Event_ForwardGenHdrData(ConnectionIdx, DOIPINT_EVENT_MSG_INV_PATTERN) == E_OK )
    {
      retVal = DOIPINT_MSG_HANDLER_FINISHED;
    }
    else
    {
      retVal = DOIPINT_MSG_HANDLER_SKIP;
    }
  }
  /* #11 Return that message handling failed otherwise (default behavior). */
  else
  {
    retVal = DOIPINT_MSG_HANDLER_FAILED;
  }

  return retVal;
} /* DoIPInt_Event_HandleGenHdrInvPatternEvent() */

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleGenHdrSkipEvents()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleGenHdrSkipEvents(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal = DOIPINT_MSG_HANDLER_SKIP;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward event in case an event callback is configured. */
  if ( DoIPInt_HasEventCbk() )
  {
    (void)DoIPInt_Event_ForwardGenHdrData(ConnectionIdx, Event);
  }

  return retVal;
} /* DoIPInt_Event_HandleGenHdrSkipEvents() */

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleOemMsgReceivedUpToLenEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* VCA NL SPC-03 : VCA_DOIPINT_NO_ENTRY_FUNC */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleOemMsgReceivedUpToLenEvent(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal; /* Not initialized: Improved readability. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward event in case an event callback is configured. */
  if ( DoIPInt_HasEventCbk() )
  {
    if ( DoIPInt_Event_ForwardOemSpecificData(ConnectionIdx, DOIPINT_EVENT_MSG_RECEIVED_UP_TO_LENGTH) == E_OK )
    {
      retVal = DOIPINT_MSG_HANDLER_PENDING;
    }
    else
    {
      retVal = DOIPINT_MSG_HANDLER_SKIP;
    }
  }
  /* #11 Return that message handling shall be continued otherwise (default behavior). */
  else
  {
    retVal = DOIPINT_MSG_HANDLER_PENDING;
  }

  return retVal;
} /* DoIPInt_Event_HandleOemMsgReceivedUpToLenEvent() */

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleDiagMsgReceivedUpToLenEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleDiagMsgReceivedUpToLenEvent(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal; /* Not initialized: Improved readability. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward event in case an event callback is configured. */
  if ( DoIPInt_HasEventCbk() )
  {
    if ( DoIPInt_Event_ForwardDiagMsgData(ConnectionIdx, DOIPINT_EVENT_MSG_RECEIVED_UP_TO_LENGTH) == E_OK )
    {
      retVal = DOIPINT_MSG_HANDLER_PENDING;
    }
    else
    {
      retVal = DOIPINT_MSG_HANDLER_SKIP;
    }
  }
  /* #11 Return that message handling shall be continued otherwise (default behavior). */
  else
  {
    retVal = DOIPINT_MSG_HANDLER_PENDING;
  }

  return retVal;
} /* DoIPInt_Event_HandleDiagMsgReceivedUpToLenEvent() */

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleOemMsgDiscardedEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleOemMsgDiscardedEvent(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal = DOIPINT_MSG_HANDLER_SKIP;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward event in case an event callback is configured. */
  if ( DoIPInt_HasEventCbk() )
  {
    (void)DoIPInt_Event_ForwardGenHdrData(ConnectionIdx, DOIPINT_EVENT_MSG_DISCARDED);
  }

  return retVal;
} /* DoIPInt_Event_HandleOemMsgDiscardedEvent() */

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleDiagMsgSkipEvents()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleDiagMsgSkipEvents(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal = DOIPINT_MSG_HANDLER_SKIP;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward event in case an event callback is configured. */
  if ( DoIPInt_HasEventCbk() )
  {
    (void)DoIPInt_Event_ForwardDiagMsgData(ConnectionIdx, Event);
  }

  return retVal;
} /* DoIPInt_Event_HandleDiagMsgSkipEvents() */

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleDiagMsgForwarded()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleDiagMsgForwarded(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal = DOIPINT_MSG_HANDLER_FINISHED;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward event in case an event callback is configured. */
  if ( DoIPInt_HasEventCbk() )
  {
    (void)DoIPInt_Event_ForwardDiagMsgData(ConnectionIdx, DOIPINT_EVENT_DIAG_MSG_FORWARDED);
  }

  return retVal;
} /* DoIPInt_Event_HandleDiagMsgForwarded() */

/**********************************************************************************************************************
 *  DoIPInt_Event_ForwardGenHdrData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Event_ForwardGenHdrData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType    pduInfo; /* Not initialized: Improved readability. */
  uint8          genHdrData[DOIPINT_GEN_HDR_LEN]; /* Not initialized: Array is initialized by another function. */
  PduLengthType  genHdrLength = 0u;
  Std_ReturnType retVal; /* Not initialized: Improved readability. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the parsed generic header data. */
  DoIPInt_GenHdr_RxGetParsedHdr(ConnectionIdx, DOIPINT_GEN_HDR_LEN, &genHdrData[0u], &genHdrLength);

  /* #20 Initialize the PDU information that is forwarded to the event callback. */
  pduInfo.SduLength = genHdrLength;
  pduInfo.SduDataPtr = &genHdrData[0u];

  /* #30 Call the event callback. */
  retVal = DoIPInt_GetEventCbk(0u)(DoIPInt_GetConnectionIdOfConnection(ConnectionIdx), Event, &pduInfo);               /* VCA_DOIPINT_OPTIONAL_FUNCTION_PTR */ /* VCA_DOIPINT_CALL_EXTERNAL_FUNCTION_CONST_POINTER_ARGUMENT */

  return retVal;
} /* DoIPInt_Event_ForwardGenHdrData() */

/**********************************************************************************************************************
 *  DoIPInt_Event_ForwardOemSpecificData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* VCA NL SPC-03 : VCA_DOIPINT_NO_ENTRY_FUNC */
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Event_ForwardOemSpecificData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType    pduInfo; /* Not initialized: Improved readability. */
  uint8          msgData[DOIPINT_GEN_HDR_LEN + DOIPINT_EVENT_MAX_USER_DATA_LEN]; /* Not initialized: Array is initialized by another function. */
  PduLengthType  msgDataLength = DOIPINT_GEN_HDR_LEN + DOIPINT_EVENT_MAX_USER_DATA_LEN;
  PduLengthType  copiedMsgLength = 0u;
  Std_ReturnType retVal; /* Not initialized: Improved readability. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the parsed generic header and OEM specific data. */
  DoIPInt_GenHdr_RxGetParsedHdr(ConnectionIdx, msgDataLength, &msgData[0u], &copiedMsgLength);
  DoIPInt_OemSpecific_RxGetParsedData(ConnectionIdx, (msgDataLength- copiedMsgLength), &msgData[copiedMsgLength],
    &copiedMsgLength);

  /* #20 Initialize the PDU information that is forwarded to the event callback. */
  pduInfo.SduLength = copiedMsgLength;
  pduInfo.SduDataPtr = &msgData[0u];

  /* #30 Call the event callback. */
  retVal = DoIPInt_GetEventCbk(0u)(DoIPInt_GetConnectionIdOfConnection(ConnectionIdx), Event, &pduInfo);               /* VCA_DOIPINT_OPTIONAL_FUNCTION_PTR */ /* VCA_DOIPINT_CALL_EXTERNAL_FUNCTION_CONST_POINTER_ARGUMENT */

  return retVal;
} /* DoIPInt_Event_ForwardOemSpecificData() */

/**********************************************************************************************************************
 *  DoIPInt_Event_ForwardDiagMsgData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Event_ForwardDiagMsgData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType    pduInfo; /* Not initialized: Improved readability. */
  uint8          msgData[DOIPINT_DIAG_HDR_LEN + DOIPINT_EVENT_MAX_USER_DATA_LEN]; /* Not initialized: Array is initialized by another function. */
  PduLengthType  msgDataLength = DOIPINT_DIAG_HDR_LEN + DOIPINT_EVENT_MAX_USER_DATA_LEN;
  PduLengthType  copiedMsgLength = 0u;
  Std_ReturnType retVal; /* Not initialized: Improved readability. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the parsed generic header and diagnostic message data. */
  DoIPInt_GenHdr_RxGetParsedHdr(ConnectionIdx, msgDataLength, &msgData[0u], &copiedMsgLength);
  DoIPInt_DiagMsg_RxGetParsedData(ConnectionIdx, (msgDataLength- copiedMsgLength), &msgData[copiedMsgLength],
    &copiedMsgLength);

  /* #20 Initialize the PDU information that is forwarded to the event callback. */
  pduInfo.SduLength = copiedMsgLength;
  pduInfo.SduDataPtr = &msgData[0u];

  /* #30 Call the event callback. */
  retVal = DoIPInt_GetEventCbk(0u)(DoIPInt_GetConnectionIdOfConnection(ConnectionIdx), Event, &pduInfo);               /* VCA_DOIPINT_OPTIONAL_FUNCTION_PTR */ /* VCA_DOIPINT_CALL_EXTERNAL_FUNCTION_CONST_POINTER_ARGUMENT */

  return retVal;
} /* DoIPInt_Event_ForwardDiagMsgData() */
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT )
/**********************************************************************************************************************
 *  DoIPInt_Event_HandleLightMsgReceivedUpToLenEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleLightMsgReceivedUpToLenEvent(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal; /* Not initialized: Improved readability. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward event in case an event callback is configured. */
  if ( DoIPInt_HasEventCbk() )
  {
    if ( DoIPInt_Event_ForwardLightMsgData(ConnectionIdx, DOIPINT_EVENT_MSG_RECEIVED_UP_TO_LENGTH) == E_OK )
    {
      retVal = DOIPINT_MSG_HANDLER_PENDING;
    }
    else
    {
      retVal = DOIPINT_MSG_HANDLER_SKIP;
    }
  /* #11 Return that message handling shall be continued otherwise (default behavior). */
  }
  else
  {
    retVal = DOIPINT_MSG_HANDLER_PENDING;
  }

  return retVal;
} /* DoIPInt_Event_HandleLightMsgReceivedUpToLenEvent() */

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleLightMsgSkipEvents()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleLightMsgSkipEvents(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal = DOIPINT_MSG_HANDLER_SKIP;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward event in case an event callback is configured. */
  if ( DoIPInt_HasEventCbk() )
  {
    (void)DoIPInt_Event_ForwardLightMsgData(ConnectionIdx, Event);
  }

  return retVal;
} /* DoIPInt_Event_HandleLightMsgSkipEvents() */

/**********************************************************************************************************************
 *  DoIPInt_Event_HandleLightMsgForwarded()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_HandleLightMsgForwarded(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal = DOIPINT_MSG_HANDLER_FINISHED;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward event in case an event callback is configured. */
  if ( DoIPInt_HasEventCbk() )
  {
    (void)DoIPInt_Event_ForwardLightMsgData(ConnectionIdx, DOIPINT_EVENT_DIAG_MSG_FORWARDED);
  }

  return retVal;
} /* DoIPInt_Event_HandleLightMsgForwarded() */

/**********************************************************************************************************************
 *  DoIPInt_Event_ForwardLightMsgData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Event_ForwardLightMsgData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_EventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType    pduInfo; /* Not initialized: Improved readability. */
  uint8          msgData[DOIPINT_LIGHT_HDR_LEN + DOIPINT_EVENT_MAX_USER_DATA_LEN]; /* Not initialized: Array is initialized by another function. */
  PduLengthType  msgLength = 0u;
  Std_ReturnType retVal; /* Not initialized: Improved readability. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the parsed light header data. */
  DoIPInt_LightHdr_RxGetParsedData(ConnectionIdx, (DOIPINT_LIGHT_HDR_LEN + DOIPINT_EVENT_MAX_USER_DATA_LEN),
    &msgData[0u], &msgLength);

  /* #20 Initialize the PDU information that is forwarded to the event callback. */
  pduInfo.SduLength = msgLength;
  pduInfo.SduDataPtr = &msgData[0u];

  /* #30 Call the event callback. */
  retVal = DoIPInt_GetEventCbk(0u)(DoIPInt_GetConnectionIdOfConnection(ConnectionIdx), Event, &pduInfo);               /* VCA_DOIPINT_OPTIONAL_FUNCTION_PTR */ /* VCA_DOIPINT_CALL_EXTERNAL_FUNCTION_CONST_POINTER_ARGUMENT */

  return retVal;
} /* DoIPInt_Event_ForwardLightMsgData() */
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_LIGHT */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DoIPInt_Event_Report()
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
 */
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_Report(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_InternalEventType Event)
{
  /* ----- Local Variables ---------------------------------------------- */
   DoIPInt_MsgHndl_RetType retVal = DOIPINT_MSG_HANDLER_FAILED;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Report event depending on the internal event type. */
  switch ( Event )
  {
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    /* #20 Report event of invalid pattern (ISO header). */
    case DOIPINT_INTERNAL_EVENT_MSG_INV_PATTERN:
    {
      retVal = DoIPInt_Event_HandleGenHdrInvPatternEvent(ConnectionIdx);
      break;
    }
    /* #30 Report event that message is too large (ISO header and OEM payload). */
    case DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_OEM:
    case DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_ISO:
    {
      retVal = DoIPInt_Event_HandleGenHdrSkipEvents(ConnectionIdx, DOIPINT_EVENT_MSG_TOO_LARGE);
      break;
    }
    /* #40 Report event of invalid length (ISO header). */
    case DOIPINT_INTERNAL_EVENT_MSG_INV_LENGTH:
    {
      retVal = DoIPInt_Event_HandleGenHdrSkipEvents(ConnectionIdx, DOIPINT_EVENT_MSG_INV_LENGTH);
      break;
    }
    /* #50 Report event that message has been received up to length (OEM payload). */
    case DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_OEM:
    {
      retVal = DoIPInt_Event_HandleOemMsgReceivedUpToLenEvent(ConnectionIdx);
      break;
    }
    /* #60 Report event that message has been received up to length (ISO header). */
    case DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_ISO:
    {
      retVal = DoIPInt_Event_HandleDiagMsgReceivedUpToLenEvent(ConnectionIdx);
      break;
    }
    /* #70 Report event that message has been discarded (OEM payload). */
    case DOIPINT_INTERNAL_EVENT_MSG_DISCARDED:
    {
      retVal = DoIPInt_Event_HandleOemMsgDiscardedEvent(ConnectionIdx);
      break;
    }
    /* #80 Report event of invalid remote address (ISO header). */
    case DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_REM_ADDR_ISO:
    {
      retVal = DoIPInt_Event_HandleDiagMsgSkipEvents(ConnectionIdx, DOIPINT_EVENT_DIAG_MSG_INV_REM_ADDR);
      break;
    }
    /* #90 Report event of invalid local address (ISO header). */
    case DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_LOCAL_ADDR_ISO:
    {
      retVal = DoIPInt_Event_HandleDiagMsgSkipEvents(ConnectionIdx, DOIPINT_EVENT_DIAG_MSG_INV_LOCAL_ADDR);
      break;
    }
    /* #100 Report event that message has been discarded (ISO header). */
    case DOIPINT_INTERNAL_EVENT_DIAG_MSG_DISCARDED_ISO:
    {
      retVal = DoIPInt_Event_HandleDiagMsgSkipEvents(ConnectionIdx, DOIPINT_EVENT_DIAG_MSG_DISCARDED);
      break;
    }
    /* #110 Report event that message has been forwarded (ISO header). */
    case DOIPINT_INTERNAL_EVENT_DIAG_MSG_FORWARDED_ISO:
    {
      retVal = DoIPInt_Event_HandleDiagMsgForwarded(ConnectionIdx);
      break;
    }
#else
    /* #120 Report event that message is too large (LIGHT header). */
    case DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_LIGHT:
    {
      retVal = DoIPInt_Event_HandleLightMsgSkipEvents(ConnectionIdx, DOIPINT_EVENT_MSG_TOO_LARGE);
      break;
    }
    /* #130 Report event that message has been received up to length (LIGHT header). */
    case DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_LIGHT:
    {
      retVal = DoIPInt_Event_HandleLightMsgReceivedUpToLenEvent(ConnectionIdx);
      break;
    }
    /* #140 Report event of invalid remote address (LIGHT header). */
    case DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_REM_ADDR_LIGHT:
    {
      retVal = DoIPInt_Event_HandleLightMsgSkipEvents(ConnectionIdx, DOIPINT_EVENT_DIAG_MSG_INV_REM_ADDR);
      break;
    }
    /* #150 Report event of invalid local address (LIGHT header). */
    case DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_LOCAL_ADDR_LIGHT:
    {
      retVal = DoIPInt_Event_HandleLightMsgSkipEvents(ConnectionIdx, DOIPINT_EVENT_DIAG_MSG_INV_LOCAL_ADDR);
      break;
    }
    /* #160 Report event that message has been discarded (LIGHT header). */
    case DOIPINT_INTERNAL_EVENT_DIAG_MSG_DISCARDED_LIGHT:
    {
      retVal = DoIPInt_Event_HandleLightMsgSkipEvents(ConnectionIdx, DOIPINT_EVENT_DIAG_MSG_DISCARDED);
      break;
    }
    /* #170 Report event that message has been forwarded (LIGHT header). */
    case DOIPINT_INTERNAL_EVENT_DIAG_MSG_FORWARDED_LIGHT:
    {
      retVal = DoIPInt_Event_HandleLightMsgForwarded(ConnectionIdx);
    break;
    }
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
    default:
    {
      /* Nothing to do. */
      break;
    }
  }

  return retVal;
} /* DoIPInt_Event_Report() */

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Event.c
 *********************************************************************************************************************/
