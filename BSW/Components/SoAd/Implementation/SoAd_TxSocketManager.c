/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  SoAd_TxSocketManager.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component TxSocketManager.
 *
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

#define SOAD_TX_SOCKET_MANAGER_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd_TxSocketManager_Int.h"
#include "SoAd_TxSocketManager_Cbk.h"
#include "SoAd_SoCon.h"
#include "SoAd_State_Int.h"
#include "SoAd_RemoteAddr_Int.h"
#include "SoAd_TimeoutList.h"
#include "SoAd_Tx.h"
#include "SoAd_Util.h"
#include "SoAd_GenTcpIpApi.h"
#include "SoAd_Cfg.h"
#include "SoAd_Priv.h"

#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
# include "Det.h"
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if !defined (SOAD_LOCAL) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL static
#endif /* !defined (SOAD_LOCAL) */

#if !defined (SOAD_LOCAL_INLINE) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL_INLINE LOCAL_INLINE
#endif /* !defined (SOAD_LOCAL_INLINE) */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define SOAD_START_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_CopyTxData()
 *********************************************************************************************************************/
/*! \brief          Copies data to provided transmission buffer.
 *  \details        Uses "BufLengthPtr" to update length if less data is copied to provided buffer.
 *  \param[in]      SockIdx           Module internal socket index.
 *                                    [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]      PartitionIdx      Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]      BufPtr            Pointer to buffer of provided transmission buffer.
 *  \param[in,out]  BufLengthPtr      Pointer to length of provided transmission buffer.
 *  \return         BUFREQ_OK         Copy request accepted.
 *  \return         BUFREQ_E_NOT_OK   Copy request not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different socket indexes, FALSE for same socket index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxSocketManager_CopyTxData(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr);

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_CopyTxDataCheckParameter()
 *********************************************************************************************************************/
/*! \brief      Checks transmission parameters on call to SoAd_CopyTxData.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  BufLength       Length of provided transmission buffer.
 *  \return     E_OK            Parameters are valid for transmission.
 *  \return     E_NOT_OK        Parameters are not valid for transmission.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxSocketManager_CopyTxDataCheckParameter(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 BufLength);

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Confirms transmission of data.
 *  \details      -
 *  \param[in]    SockIdx         Module internal socket index.
 *                                [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    Length          Length of confirmed data.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket indexes, FALSE for same socket index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxSocketManager_TxConfirmation(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 Length);

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_TerminateTp()
 *********************************************************************************************************************/
/*! \brief       Terminates active TP sessions.
 *  \details     -
 *  \param[in]   SoConIdx           Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsTxMgtUsedOfSoCon()]
 *  \param[in]   PartitionIdx       Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different PDUs, FALSE for same PDU.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxSocketManager_TerminateTp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  SoAd_TxSocketManager_CopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_TxSocketManager_CopyTxData(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType soConIdx = SoAd_GetSoConTxActiveIdxOfSocketDyn(SockIdx, PartitionIdx);
  BufReq_ReturnType    retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if parameters are valid. */
  if ( SoAd_TxSocketManager_CopyTxDataCheckParameter(soConIdx, PartitionIdx, *BufLengthPtr) == E_OK )
  {
    /* #20 Handle transmission for TCP (if enabled). */
#if ( SOAD_TCP == STD_ON )
    SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
    if ( SoAd_IsSocketTcpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) )
    {
      retVal = SoAd_TxTcp_CopyTxData(soConIdx, PartitionIdx, BufPtr, *BufLengthPtr);                                   /* SBSW_SOAD_POINTER_FORWARD */
    }
    /* #30 Handle transmission for UDP otherwise. */
    else
#endif /* SOAD_TCP == STD_ON */
    {
      retVal = SoAd_TxUdp_CopyTxData(soConIdx, PartitionIdx, BufPtr, BufLengthPtr);                                    /* SBSW_SOAD_POINTER_FORWARD */
    }
  }
  return retVal;
} /* SoAd_TxSocketManager_CopyTxData() */

 /*********************************************************************************************************************
 *  SoAd_TxSocketManager_CopyTxDataCheckParameter()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxSocketManager_CopyTxDataCheckParameter(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 BufLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if no socket connection is active on requested socket. */
  if ( SoConIdx == SoAd_GetSizeOfSoCon(PartitionIdx) )
  {
    retVal = E_NOT_OK;
  }
  /* #11 Check socket connection states otherwise. */
  /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
  else if ( (SoAd_SoCon_GetMode(SoConIdx, PartitionIdx) != SOAD_SOCON_ONLINE) ||
    (SoAd_SoCon_GetCloseMode(SoConIdx, PartitionIdx) != SOAD_CLOSE_NONE) )
  {
    retVal = E_NOT_OK;
  }
  /* #12 Check for PDU route otherwise. */
  else if ( !SoAd_IsTxMgtUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    retVal = E_NOT_OK;
  }
  /* #13 Check buffer length otherwise. */
  else if ( BufLength > SoAd_GetTxBytesPendingOfTxMgt(SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
  {
    retVal = E_NOT_OK;
  }
  /* #14 Indicate that parameters are correct. */
  else
  {
    retVal = E_OK;
  }

  return retVal;
} /* SoAd_TxSocketManager_CopyTxDataCheckParameter() */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_TxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 3206 3 */ /* MD_SoAd_UnusedParameter */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxSocketManager_TxConfirmation(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType soConIdx = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the connection states and that the remote address is not forced to be released. */
  /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
  if ( (SoAd_SoCon_CheckConnectionStates(soConIdx, PartitionIdx) == E_OK) &&
    (SoAd_RemoteAddr_IsForceRelease(soConIdx, PartitionIdx) == FALSE) )
  {
#if ( SOAD_TCP == STD_ON )
    /* #20 Handle TxConfirmation for TCP socket connection (if enabled). */
    if ( SoAd_IsSocketTcpUsedOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx), PartitionIdx) )
    {
      SoAd_TxTcpQueue_TxConfirmation(soConIdx, PartitionIdx, (PduLengthType)Length);
    }
    else
#else
  SOAD_DUMMY_STATEMENT(Length); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_TCP == STD_ON */
    /* #21 Handle TxConfirmation for UDP socket connection with immediate TxConfirmation otherwise. */
    {
      SoAd_Tx_UdpImmediateIfTxConfirmation(soConIdx, PartitionIdx);
    }
  }
} /* SoAd_TxSocketManager_TxConfirmation() */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_TerminateTp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_TxSocketManager_TerminateTp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Optional indirection: Refer to parameter value range in function description. */
  SoAd_SizeOfTxMgtType        txMgtIdx = SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfPduRouteDestType pduRouteDestIdx = SoAd_GetPduRouteDestIdxOfTxMgt(txMgtIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if transmission is active on socket connection. */
  if ( pduRouteDestIdx != SoAd_GetSizeOfPduRouteDest(PartitionIdx) )
  {
    SoAd_SizeOfPduRouteType pduRouteIdx = SoAd_GetPduRouteIdxOfPduRouteDest(pduRouteDestIdx, PartitionIdx);

    /* #20 Check if transmission is of type TP. */
    if ( SoAd_GetUpperLayerApiOfPduRoute(pduRouteIdx, PartitionIdx) == SOAD_UL_API_TP )
    {
      /* #30 Handle TP termination. */
      SoAd_TxTp_Terminate(txMgtIdx, pduRouteIdx, PartitionIdx);

      /* #40 Reset transmission struct on socket connection. */
      SoAd_SetTxBytesLenOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                           /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      SoAd_SetTxBytesPendingOfTxMgt(txMgtIdx, 0u, PartitionIdx);                                                       /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      SoAd_SetPduRouteDestIdxOfTxMgt(txMgtIdx, SoAd_GetSizeOfPduRouteDest(PartitionIdx), PartitionIdx);                /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
      SoAd_SetCancelRequestedOfTxMgt(txMgtIdx, FALSE, PartitionIdx);                                                   /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    }
  }
} /* SoAd_TxSocketManager_TerminateTp() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/* ----- Service functions --------------------------------------------- */


/* ----- Functions ----------------------------------------------------- */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TxSocketManager_Init(
  SoAd_SocketIterType SocketIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize Tx struct elements of SocketDyn. */
  SoAd_SetSoConTxActiveIdxOfSocketDyn(SocketIdx, SoAd_GetSizeOfSoCon(PartitionIdx), PartitionIdx);                     /* SBSW_SOAD_CSL02_CSL05 */
} /* SoAd_TxSocketManager_Init() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_TxSocketManager_TcpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxSocketManager_TcpTransmit(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint32 AvailableLength,
  boolean ForceRetrieve)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketType sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SocketIdType     socketId = SoAd_SoCon_GetSocketId(sockIdx, PartitionIdx);
  Std_ReturnType        retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket identifier is valid. */
  if ( socketId != SOAD_INV_SOCKET_ID )
  {
    /* #20 Try to transmit PDU over TCP. */
    retVal = SoAd_TcpIpApiTcpTransmit(socketId, NULL_PTR, AvailableLength, ForceRetrieve);                             /* SBSW_SOAD_NULL_POINTER_FORWARD */
  }

  return retVal;
} /* SoAd_TxSocketManager_TcpTransmit() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_UdpTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxSocketManager_UdpTransmit(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 TotalLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketType sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SocketIdType     socketId = SoAd_SoCon_GetSocketId(sockIdx, PartitionIdx);
  Std_ReturnType        retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket identifier is valid. */
  if ( socketId != SOAD_INV_SOCKET_ID )
  {
    SoAd_SockAddrInet6Type remAddrPtr = { 0u, 0u, { 0u, 0u, 0u, 0u } };

    /* #20 Get remote address. */
    /* PRQA S 310 1 */ /* MD_SoAd_PointerCastOnStruct */
    SoAd_RemoteAddr_Get(SoConIdx, PartitionIdx, SOAD_A_P2VAR(SoAd_SockAddrType)&remAddrPtr);                           /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

    /* #30 Try to transmit PDU over UDP. */
    /* PRQA S 310 2 */ /* MD_SoAd_PointerCastOnStruct */
    retVal = SoAd_TcpIpApiUdpTransmit(socketId, NULL_PTR, SOAD_A_P2VAR(SoAd_SockAddrType)&remAddrPtr, TotalLength);    /* SBSW_SOAD_VARIABLE_POINTER_AND_NULL_POINTER_FORWARD */
  }

  return retVal;
} /* SoAd_TxSocketManager_UdpTransmit() */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_Terminate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TxSocketManager_Terminate(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Terminate active TP session. */
  SoAd_TxSocketManager_TerminateTp(SoConIdx, PartitionIdx);

#if ( SOAD_TCP == STD_ON )
  if ( SoAd_IsSocketTcpUsedOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
  {
    /* #20 Terminate TCP transmission if used. */
    SoAd_TxTcp_Terminate(SoConIdx, PartitionIdx);
  }
  else
#endif /* SOAD_TCP == STD_ON */
  {
    /* #30 Terminate UDP transmission otherwise. */
    SoAd_TxUdp_Terminate(SoConIdx, PartitionIdx);
  }
} /* SoAd_TxSocketManager_Terminate() */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_CheckAndSetTxActive()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxSocketManager_CheckAndSetTxActive(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean SetTxActiveOnSoCon)
  {
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketType sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType        retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent read/write issues for active transmission on socket. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if socket is established and no transmission is active on socket. */
  if ( (SoAd_SoCon_GetSocketState(sockIdx, PartitionIdx) == SOAD_SOCK_STATE_ESTABLISHED) &&
    (SoAd_GetSoConTxActiveIdxOfSocketDyn(sockIdx, PartitionIdx) == SoAd_GetSizeOfSoCon(PartitionIdx)) )
  {
    /* #30 Set transmission active on socket. */
    SoAd_SetSoConTxActiveIdxOfSocketDyn(sockIdx, SoConIdx, PartitionIdx);                                              /* SBSW_SOAD_CSL02_CSL05 */

    /* #40 Set transmission active on socket connection if required. */
    if ( SetTxActiveOnSoCon == TRUE )
    {
      SoAd_SetTxActiveOfSoConDyn(SoConIdx, TRUE, PartitionIdx);                                                        /* SBSW_SOAD_CSL02_CSL05 */
    }

    retVal = E_OK;
  }

  /* #50 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_TxSocketManager_CheckAndSetTxActive() */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_ResetTxActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_TxSocketManager_ResetTxActive(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean ResetOnSoConRequired)
  {
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketType sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset active transmission on socket. */
  SoAd_SetSoConTxActiveIdxOfSocketDyn(sockIdx, SoAd_GetSizeOfSoCon(PartitionIdx), PartitionIdx);                       /* SBSW_SOAD_CSL02_CSL05 */

  /* #20 Reset active transmission on socket connection if required. */
  if ( ResetOnSoConRequired == TRUE )
  {
    SoAd_SetTxActiveOfSoConDyn(SoConIdx, FALSE, PartitionIdx);                                                         /* SBSW_SOAD_CSL02_CSL05 */
  }
} /* SoAd_TxSocketManager_ResetTxActive() */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_IsTxActive()
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
FUNC(boolean, SOAD_CODE) SoAd_TxSocketManager_IsTxActive(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean TransmitImmediately)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isTxActive; /* Not initialized: Avoid multiple declaration for same variable. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that indicators for active transmission are accessed in other context. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check for pending IF/TP transmission. */
  if ( SoAd_IsTxMgtUsedOfSoCon(SoConIdx, PartitionIdx) &&
    (SoAd_GetPduRouteDestIdxOfTxMgt(SoAd_GetTxMgtIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) !=
      SoAd_GetSizeOfPduRouteDest(PartitionIdx)) )
  {
    isTxActive = TRUE;
  }
  /* #30 Check for pending nPdu transmission. */
  else if ( SoAd_TxNPduUdp_IsTxPending(SoConIdx, PartitionIdx, TransmitImmediately) )
  {
    isTxActive = TRUE;
  }
  /* #40 Check for pending routing group transmission. */
  else if ( SoAd_Tx_IsRouteGrpActive(SoConIdx, PartitionIdx) == TRUE )
  {
    isTxActive = TRUE;
  }
  /* #50 Indicate that no transmission is active otherwise. */
  else
  {
    isTxActive = FALSE;
  }
  /* #60 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return isTxActive;
} /* SoAd_TxSocketManager_IsTxActive() */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_IsTxActiveOnSocket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, SOAD_CODE) SoAd_TxSocketManager_IsTxActiveOnSocket(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isTxActive = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if transmission is active on socket. */
  if ( SoAd_GetSoConTxActiveIdxOfSocketDyn(SockIdx, PartitionIdx) == SoAd_GetSizeOfSoCon(PartitionIdx) )
  {
    isTxActive = FALSE;
  }

  return isTxActive;
} /* SoAd_TxSocketManager_IsTxActiveOnSocket() */

/**********************************************************************************************************************
 *  SoAd_TxSocketManager_CheckAndAddPduHdrSize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TxSocketManager_CheckAndAddPduHdrSize(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_VAR) DataLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType          retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if PDU header is enabled. */
  if ( SoAd_IsPduHdrEnabledOfSoConGrp(soConGrpIdx, PartitionIdx) )
  {
    /* #20 Get max supported length. */
#if ( SOAD_TCP == STD_ON )
    boolean isUdp = SoAd_IsSocketUdpUsedOfSoConGrp(soConGrpIdx, PartitionIdx);
    uint32 maxSupportedLen = isUdp ? (uint32)SOAD_MAX_UDP_PDU_LEN : SOAD_MAX_PDU_LEN;
#else
    uint32 maxSupportedLen = (uint32)SOAD_MAX_UDP_PDU_LEN;
#endif /* SOAD_TCP == STD_ON */

    /* #30 Assert that adding the PDU header size causes no overflow. */
    if ( *DataLength <= (maxSupportedLen - SOAD_PDU_HDR_SIZE) )
    {
      /* #40 Add PDU header size to data. */
      *DataLength += SOAD_PDU_HDR_SIZE;                                                                                /* SBSW_SOAD_POINTER_WRITE */
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
} /* SoAd_TxSocketManager_CheckAndAddPduHdrSize() */

/* ----- Callbacks ----------------------------------------------------- */

/**********************************************************************************************************************
 *  SoAd_CopyTxData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 2 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, SOAD_CODE) SoAd_CopyTxData(
  SoAd_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
#if ( SOAD_TX_DYN_LEN == STD_ON )
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) BufLengthPtr
#else
  uint16 BufLength
#endif /* SOAD_TX_DYN_LEN == STD_ON */
  )
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_TX_DYN_LEN == STD_OFF )
  uint16                      localBufLength = BufLength;
#endif /* SOAD_TX_DYN_LEN == STD_OFF */
  SoAd_SizeOfSocketType       sockIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  BufReq_ReturnType           retVal = BUFREQ_E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( SoAd_SoCon_GetSockIdxBySocketId(SocketId, &sockIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  else if ( BufPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
# if ( SOAD_TX_DYN_LEN == STD_ON )
  else if ( BufLengthPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( *BufLengthPtr == 0u )
  {
    errorId = SOAD_E_INV_ARG;
  }
# else
  else if ( localBufLength == 0u )
  {
    errorId = SOAD_E_INV_ARG;
  }
# endif /* SOAD_TX_DYN_LEN == STD_ON */
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped instance index. */
  else if ( SoAd_Util_CheckApplicationIdValidity(partitionIdx) != E_OK )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
  else
#else
  if ( SoAd_SoCon_GetSockIdxBySocketId(SocketId, &sockIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Handle copy request for transmission data. */
#if ( SOAD_TX_DYN_LEN == STD_ON )
    retVal = SoAd_TxSocketManager_CopyTxData(sockIdx, partitionIdx, BufPtr, BufLengthPtr);                             /* SBSW_SOAD_POINTER_FORWARD */
#else
    retVal = SoAd_TxSocketManager_CopyTxData(sockIdx, partitionIdx, BufPtr, &localBufLength);                          /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_TX_DYN_LEN == STD_ON */
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_COPY_TX_DATA, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_CopyTxData() */

/**********************************************************************************************************************
 *  SoAd_TxConfirmation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, SOAD_CODE) SoAd_TxConfirmation(
  SoAd_SocketIdType SocketId,
  uint16 Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketType       sockIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( Length == 0u )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( SoAd_SoCon_GetSockIdxBySocketId(SocketId, &sockIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_SOCKETID;
  }
  else if ( !SoAd_IsTxMgtUsedOfSoCon(SoAd_GetSoConStartIdxOfSocket(sockIdx, partitionIdx), partitionIdx) )
  {
    errorId = SOAD_E_INV_ARG;
  }
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped instance index. */
  else if ( SoAd_Util_CheckApplicationIdValidity(partitionIdx) != E_OK )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
  else
#else
  if ( SoAd_SoCon_GetSockIdxBySocketId(SocketId, &sockIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */

    /* #30 Handle TxConfirmation. */
    SoAd_TxSocketManager_TxConfirmation(sockIdx, partitionIdx, Length);
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #40 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_TX_CONFIRMATION, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
} /* SoAd_TxConfirmation() */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_TxSocketManager.c
 *********************************************************************************************************************/
