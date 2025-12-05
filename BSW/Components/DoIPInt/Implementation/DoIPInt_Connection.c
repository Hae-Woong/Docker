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
/*!        \file  DoIPInt_Connection.c
 *        \brief  DoIPInt source file for sub-module DoIP_Connection.
 *      \details  Implementation for sub-module DoIP_Connection of component DoIPInt.
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

#define DOIPINT_CONNECTION_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "DoIPInt.h"
#include "DoIPInt_Connection.h"
#include "DoIPInt_Tx.h"
#include "DoIPInt_Rx.h"
#include "DoIPInt_Priv.h"
#include "SoAd.h"
#if ( DOIPINT_CHANNEL_READY_ENABLED == STD_ON )
# include "BswM_DoIPInt.h"
#endif /* DOIPINT_CHANNEL_READY_ENABLED == STD_ON */

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

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DOIPINT_START_SEC_CODE
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_Connection_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Connection_Init(
  DoIPInt_ConnectionIterType ConnectionIter)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize socket connection mode. */
  DoIPInt_SetModeOfConnectionDyn(ConnectionIter, SOAD_SOCON_OFFLINE);

  /* #20 Initialize requested connection mode. */
  DoIPInt_SetRequestModeOfConnectionDyn(ConnectionIter, DOIPINT_SO_CON_REQUEST_MODE_NONE);

  /* #30 Initialize close pending flag. */
  DoIPInt_SetClosePendingOfConnectionDyn(ConnectionIter, FALSE);
} /* DoIPInt_Connection_Init() */

/**********************************************************************************************************************
 *  DoIPInt_Connection_GetConnectionIdxBySoConId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_Connection_GetConnectionIdxBySoConId(
  SoAd_SoConIdType SoConId,
  P2VAR(DoIPInt_SizeOfConnectionType, AUTOMATIC, DOIPINT_VAR) ConnectionIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_ConnectionIterType  connectionIter;
  Std_ReturnType              retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set ouput parameter to invalid value. */
  *ConnectionIdxPtr = DoIPInt_GetSizeOfConnection();

  /* #20 Iterate over all connections. */
  for ( connectionIter = 0u; connectionIter < DoIPInt_GetSizeOfConnection(); connectionIter++ )
  {
    /* #30 Check if connection has the requested socket connection index. */
    if ( DoIPInt_GetSoConIdOfConnection(connectionIter) == SoConId )
    {
      /* #40 Set output parameter to the connection index. */
      *ConnectionIdxPtr = (DoIPInt_SizeOfConnectionType)connectionIter;
      retVal = E_OK;
      break;
    }
  }

  /*@ assert retVal != E_OK || *ConnectionIdxPtr < DoIPInt_GetSizeOfConnection(); */
  return retVal;
} /* DoIPInt_Connection_GetConnectionIdxBySoConId() */

/**********************************************************************************************************************
 *  DoIPInt_Connection_IsOpened()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, DOIPINT_CODE) DoIPInt_Connection_IsOpened(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket connection is online. */
  if ( DoIPInt_GetModeOfConnectionDyn(ConnectionIdx) == SOAD_SOCON_ONLINE )
  {
    /* #20 Check if connection is requested to be opened. */
    if ( DoIPInt_GetRequestModeOfConnectionDyn(ConnectionIdx) == DOIPINT_SO_CON_REQUEST_MODE_ONLINE )
    {
      /* #30 Indicate that connection is opened. */
      retVal = TRUE;
    }
  }

  return retVal;
} /* DoIPInt_Connection_IsOpened() */

/**********************************************************************************************************************
 *  DoIPInt_Connection_IsClosedOrRequested()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(boolean, DOIPINT_CODE) DoIPInt_Connection_IsClosedOrRequested(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket connection is online. */
  if ( DoIPInt_GetModeOfConnectionDyn(ConnectionIdx) == SOAD_SOCON_ONLINE )
  {
    /* #20 Check if connection is requested to be opened. */
    if ( DoIPInt_GetRequestModeOfConnectionDyn(ConnectionIdx) == DOIPINT_SO_CON_REQUEST_MODE_ONLINE )
    {
      /* #30 Check if close is not pending for the connection. */
      if ( !DoIPInt_IsClosePendingOfConnectionDyn(ConnectionIdx) )
      {
        /* #40 Indicate that connection is neither closed nor requested to be closed. */
        retVal = FALSE;
      }
    }
  }

  return retVal;
} /* DoIPInt_Connection_IsClosedOrRequested() */

/**********************************************************************************************************************
 *  DoIPInt_Connection_ProcessState()
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
FUNC(void, DOIPINT_CODE) DoIPInt_Connection_ProcessState(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_ConnectionDynIterType connectionDynIterType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all connections. */
  for ( connectionDynIterType = 0u; connectionDynIterType < DoIPInt_GetSizeOfConnectionDyn(); connectionDynIterType++ )
  {
    /* #20 Check if connection is requested to be closed. */
    if ( DoIPInt_IsClosePendingOfConnectionDyn(connectionDynIterType) )
    {
      /* #30 Check if no transmission is pending for the connection. */
      if ( DoIPInt_Tx_IsTransmissionPending((DoIPInt_SizeOfConnectionType)connectionDynIterType) == FALSE )
      {
        /* #40 Call SoAd to close socket connection. */
        (void)SoAd_CloseSoCon(DoIPInt_GetSoConIdOfConnection(connectionDynIterType), FALSE);

        /* #50 Indicate that connection is now requested to be closed. */
        DoIPInt_SetRequestModeOfConnectionDyn(connectionDynIterType, DOIPINT_SO_CON_REQUEST_MODE_OFFLINE);
        DoIPInt_SetClosePendingOfConnectionDyn(connectionDynIterType, FALSE);
      }
    }

    /* #60 Check if connection is not yet requested to be opened. */
    if ( DoIPInt_GetRequestModeOfConnectionDyn(connectionDynIterType) == DOIPINT_SO_CON_REQUEST_MODE_NONE )
    {
      /* #70 Call SoAd to open socket connection. */
      (void)SoAd_OpenSoCon(DoIPInt_GetSoConIdOfConnection(connectionDynIterType));

      /* #80 Indicate that connection is now requested to be opened. */
      DoIPInt_SetRequestModeOfConnectionDyn(connectionDynIterType, DOIPINT_SO_CON_REQUEST_MODE_ONLINE);
    }
  }
} /* DoIPInt_Connection_ProcessState() */

/**********************************************************************************************************************
 *  DoIPInt_Connection_CloseConnection()
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
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Connection_CloseConnection(
  DoIPInt_ConnectionIdType ConnectionId,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfConnectionType connectionIdx = DoIPInt_GetConnectionIdxOfConnectionIdMap(ConnectionId);
  boolean                            closeRequired = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent mode modifications from different context. */
  DOIPINT_BEGIN_CRITICAL_SECTION();

  /* #20 Handle close request depending on socket connection mode. */
  switch ( DoIPInt_GetModeOfConnectionDyn(connectionIdx) )
  {
    /* #30 Handle close request if socket connection is online. */
    case SOAD_SOCON_ONLINE:
    {
      /* #300 Immediately close the connection when the abort flag is set. */
      if ( Abort == TRUE )
      {
        /* #3000 Indicate that a call to SoAd to close the socket connection is required. */
        closeRequired = TRUE;

        /* #3001 Indicate that connection is now requested to be closed. */
        DoIPInt_SetRequestModeOfConnectionDyn(connectionIdx, DOIPINT_SO_CON_REQUEST_MODE_OFFLINE);
        DoIPInt_SetClosePendingOfConnectionDyn(connectionIdx, FALSE);
      }
      /* #301 Check for pending transmission otherwise. */
      else
      {
        /* #3010 Close the connection when no transmission is pending. */
        if ( DoIPInt_Tx_IsTransmissionPending(connectionIdx) == FALSE )
        {
          /* #30100 Indicate that a call to SoAd to close the socket connection is required. */
          closeRequired = TRUE;

          /* #30101 Indicate that connection is now requested to be closed. */
          DoIPInt_SetRequestModeOfConnectionDyn(connectionIdx, DOIPINT_SO_CON_REQUEST_MODE_OFFLINE);
          DoIPInt_SetClosePendingOfConnectionDyn(connectionIdx, FALSE);
        }
        /* #3011 Set a flag to handle the close request later otherwise. */
        else
        {
          DoIPInt_SetClosePendingOfConnectionDyn(connectionIdx, TRUE);
        }
      }
      break;
    }
    /* #40 Handle close request if socket connection is offline or in reconnect state otherwise. */
    default: /* SOAD_SOCON_OFFLINE, SOAD_SOCON_RECONNECT */
    {
      /* #400 Close socket connection explicitly if not done already. */
      if ( DoIPInt_GetRequestModeOfConnectionDyn(connectionIdx) == DOIPINT_SO_CON_REQUEST_MODE_ONLINE )
      {
        /* #4000 Indicate that a call to SoAd to close the socket connection is required. */
        closeRequired = TRUE;

        /* #4001 Indicate that connection is now requested to be closed. */
        DoIPInt_SetRequestModeOfConnectionDyn(connectionIdx, DOIPINT_SO_CON_REQUEST_MODE_OFFLINE);
      }
      break;
    }
  }

  /* #50 Leave critical section. */
  DOIPINT_END_CRITICAL_SECTION();

  /* #60 Check if it was indicated that a call to SoAd to close the socket connection is required. */
  if ( closeRequired == TRUE )
  {
    /* #600 Call SoAd to close socket connection. */
    (void)SoAd_CloseSoCon(DoIPInt_GetSoConIdOfConnection(connectionIdx), Abort);
  }
} /* DoIPInt_Connection_CloseConnection() */

/**********************************************************************************************************************
 *  DoIPInt_Connection_ModeChg()
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
 */
FUNC(void, DOIPINT_CODE) DoIPInt_Connection_ModeChg(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  SoAd_SoConModeType Mode)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store new mode on connection. */
  DoIPInt_SetModeOfConnectionDyn(ConnectionIdx, Mode);

  switch ( Mode )
  {
    /* #20 Check if socket connection is online. */
    case SOAD_SOCON_ONLINE:
    {
      /* #200 Check if no explicit open request has been performed. */
      if ( DoIPInt_GetRequestModeOfConnectionDyn(ConnectionIdx) != DOIPINT_SO_CON_REQUEST_MODE_ONLINE )
      {
        /* #2000 Call SoAd to close socket connection. */
        (void)SoAd_CloseSoCon(DoIPInt_GetSoConIdOfConnection(ConnectionIdx), TRUE);

        /* #2001 Indicate that connection is now requested to be closed. */
        DoIPInt_SetRequestModeOfConnectionDyn(ConnectionIdx, DOIPINT_SO_CON_REQUEST_MODE_OFFLINE);
      }
#if ( DOIPINT_CHANNEL_READY_ENABLED == STD_ON )
      /* #201 Assume a valid socket conection mode transition otherwise. */
      else
      {
        /* #2010 Check if connection uses channel ready notification. */
        if ( DoIPInt_IsChannelReadyUsedOfConnection(ConnectionIdx) == TRUE )
        {
          /* #20100 Notify user that ComM channel is ready to transmit diagnostic messages. */
          BswM_DoIPInt_SetChannelReady(DoIPInt_GetComMChannelIdOfConnection(ConnectionIdx));
        }
      }
#endif /* DOIPINT_CHANNEL_READY_ENABLED == STD_ON */
      break;
    }
    /* #30 Check if socket connection is offline otherwise. */
    case SOAD_SOCON_OFFLINE:
    {
      /* #300 Close socket connection explicitly if not done already. */
      if ( DoIPInt_GetRequestModeOfConnectionDyn(ConnectionIdx) == DOIPINT_SO_CON_REQUEST_MODE_ONLINE )
      {
        (void)SoAd_CloseSoCon(DoIPInt_GetSoConIdOfConnection(ConnectionIdx), TRUE);
      }

      /* #301 Terminate pending transmission. */
      DoIPInt_Tx_Terminate(ConnectionIdx);

      /* #302 Terminate pending reception. */
      DoIPInt_Rx_Terminate(ConnectionIdx);

      /* #303 Reset requested mode of connection. */
      DoIPInt_SetRequestModeOfConnectionDyn(ConnectionIdx, DOIPINT_SO_CON_REQUEST_MODE_NONE);
      break;
    }
    /* #40 Assume socket connection is in reconnect mode otherwise. */
    default: /* SOAD_SOCON_RECONNECT */
    {
      /* #400 Terminate pending transmission. */
      DoIPInt_Tx_Terminate(ConnectionIdx);

      /* #401 Terminate pending reception. */
      DoIPInt_Rx_Terminate(ConnectionIdx);
      break;
    }
  }
} /* DoIPInt_Connection_ModeChg() */

#define DOIPINT_STOP_SEC_CODE
#include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Connection.c
 *********************************************************************************************************************/
