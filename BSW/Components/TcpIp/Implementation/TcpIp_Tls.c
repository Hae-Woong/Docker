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
/*!        \file  TcpIp_Tls.c
 *        \brief  Transport Layer Security (TLS) main API source file.
 *
 *      \details  This file is part of the TcpIp TLS submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#define TCPIP_TLS_SOURCE

#include "TcpIp_Cfg.h"
#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "TcpIp_Lcfg.h"
# include "TcpIp_Tls.h"
# include "TcpIp_TlsCoreCommon.h"
# if (TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON)
#  include "TcpIp_Tls12Core.h"
#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
#   include "TcpIp_TlsServer.h"
#  endif /* (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON) */
#  if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
#   include "TcpIp_Tls12Client.h"
#  endif /* (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON) */
# endif /* (TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON) */
# if (TCPIP_SUPPORT_ASRTLS_VERSION_1_3 == STD_ON)
#  include "TcpIp_Tls13Core.h"
# endif
# if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
#  include "TcpIp_TlsClientCommon.h"
# endif /* (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON) */
# include "TcpIp.h"
# include "TcpIp_Tcp.h"

# if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
#  include "TcpIp_TlsClientCommon.h"
# endif /* TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON */



# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# if !defined (TCPIP_LOCAL)
#  define TCPIP_LOCAL static
# endif

# if !defined (TCPIP_LOCAL_INLINE)
#  define TCPIP_LOCAL_INLINE LOCAL_INLINE
# endif

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
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define TCPIP_START_SEC_CODE
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  TcpIp_Tls_InitMemory
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tls_InitMemory( void )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 No memory section to initialize - left blank intentionally */
} /* TcpIp_Tls_InitMemory() */

/**********************************************************************************************************************
 *  TcpIp_Tls_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tls_Init( void )
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize sub-modules */
  TcpIp_TlsCore_Init();

# if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
  TcpIp_TlsServer_Init();
# endif /* TCPIP_SUPPORT_ASRTLSSERVER == STD_ON */

# if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
  TcpIp_TlsClient_Init();
# endif /* TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON */

  /* This justifications verify that TLS is not configured with more than 2^32 bytes of buffer. */
  /* CM_TCPIP_TLS_R_WRITE_INDEX_AND_BUFFER_AREA_LENGTH_CSL01 */
  /*@ assert sizeof(TcpIp_SizeOfTcpTxBufferType) <= sizeof(uint32); */
  /*@ assert sizeof(TcpIp_SizeOfTcpRxBufferType) <= sizeof(uint32); */

} /* TcpIp_Tls_Init() */

/**********************************************************************************************************************
 *  TcpIp_Tls_MainFunctionRx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tls_MainFunctionRx(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call main function for reception */
  TcpIp_TlsCoreMainFunction_Rx();
} /* TcpIp_Tls_MainFunctionRx() */

/**********************************************************************************************************************
 *  TcpIp_Tls_MainFunctionState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tls_MainFunctionState(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call main function for states */
  TcpIp_TlsCoreMainFunction_State();
} /* TcpIp_Tls_MainFunctionState() */

/**********************************************************************************************************************
 *  TcpIp_Tls_MainFunctionTx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tls_MainFunctionTx(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Call main function for transmission */
  TcpIp_TlsCoreMainFunction_Tx();
} /* TcpIp_Tls_MainFunctionTx() */

/**********************************************************************************************************************
 *  TcpIp_Tls_MainFunctionLowPrio
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tls_MainFunctionLowPrio(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check initialization of the component. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    /* module is not initialized, so do nothing here but just return */
  }
  else
  {
    /* #20 Call low prio main function */
    TcpIp_TlsCoreMainFunction_LowPrio();
  }

} /* TcpIp_Tls_MainFunctionLowPrio() */

/**********************************************************************************************************************
 *  TcpIp_Tls_RxIndicaion
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tls_RxIndication(
  TcpIp_SizeOfSocketTcpDynType SocketTcpIdx,
  TCPIP_P2V(uint8)             DataPtr,
  uint16                       DataLen)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward the call to the internal TLS lower layer RxIndication */
  TcpIp_TlsCoreLl_RxIndication(SocketTcpIdx, DataPtr, DataLen);

} /* TcpIp_Tls_RxIndicaion() */

/**********************************************************************************************************************
 *  TcpIp_Tls_ChangeParameter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_ChangeParameter(
  TcpIp_SizeOfSocketDynType    SocketIdx,
  uint8                        OptName,
  TCPIP_P2C(uint8)             OptValPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  TCPIP_ASSERT(SocketIdx < TcpIp_GetSizeOfSocketDyn());

  /* ----- Implementation ----------------------------------------------- */
  if (TCPIP_SOCKET_IDX_IS_TCP(SocketIdx))
  {
    TcpIp_SizeOfSocketTcpDynType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx);
    /* #10 Forward the change parameter request to the TLS core unit */
    switch (OptName)
    {
      case TCPIP_PARAMID_TLS_CONNECTION_ASSIGNMENT:
      case TCPIP_PARAMID_TLS_CONNECTIONBASED_SESSION_RESUMPTION:
      {
        retVal = TcpIp_TlsCore_ChangeParameter(socketTcpIdx, OptName, OptValPtr);
        break;
      }
      default:
      {
        /* ----- Development Error Report ------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
        /* #20 Report an error if necessary. */
        (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID,
          TCPIP_TLS_API_ID_TLS_CHANGE_PARAMETER, TCPIP_TLS_E_INV_ARG);
# endif /* TCPIP_DEV_ERROR_REPORT */
        break;
      }
    } /* switch() */
  }
  return retVal;
} /* TcpIp_Tls_ChangeParameter() */

/**********************************************************************************************************************
 *  TcpIp_Tls_AllocateConnection
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_AllocateConnection(
  TcpIp_SizeOfSocketDynType SocketIdx,
  TcpIp_TlsConnectionDynIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the TLS connection exists and is free. */
  if ((TlsConIdx < TcpIp_GetSizeOfTlsConnection()) &&
      (TcpIp_GetSocketDynComIdxOfTlsConnectionDyn(TlsConIdx) == TCPIP_NO_SOCKETDYNCOMIDXOFTLSCONNECTIONDYN))
  {
    /* The TLS connection is not yet in use.*/
      /* #20 Check if the LocalAddrIdx of the TLS connection is suitable, and allocate it. */
    if ((TcpIp_GetLocalAddrIdxOfTlsConnection(TlsConIdx) == TCPIP_NO_LOCALADDRIDXOFTLSCONNECTION) ||
        (TcpIp_GetLocalAddrIdxOfTlsConnection(TlsConIdx) == TcpIp_GetLocalAddrBindIdxOfSocketDyn(SocketIdx)))
    {
      /* The TLS connection is not yet in use and the LocalAddrIdx matches. Thus, the connection can be assigned
         to the socket. Set the socket index of the Tls - Connection to connected socket. */
      TcpIp_SetSocketDynComIdxOfTlsConnectionDyn(TlsConIdx, SocketIdx);
      TcpIp_SetTlsConnectionIdxOfSocketTcpDyn((TcpIp_TlsConnectionIdxOfSocketTcpDynType)TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx), (TcpIp_SizeOfTlsConnectionDynType)TlsConIdx);
      TcpIp_SetConnectionStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CONNECTION_ALLOCATED);

      retVal = E_OK;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report an error if necessary. */
  if (retVal != E_OK)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID, TCPIP_TLS_API_ID_TLS_ALLOCATE_CONNECTION, TCPIP_TLS_E_RESOURCE_IN_USE);
  }
# endif /* TCPIP_DEV_ERROR_REPORT */

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_Tls_FreeAllocatedConnection
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_Tls_FreeAllocatedConnection(
  TcpIp_TlsConnectionDynIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the TLS connection exists and free it. */
  if (TlsConIdx < TcpIp_GetSizeOfTlsConnection())
  {
    TcpIp_SizeOfSocketTcpDynType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(TcpIp_GetSocketDynComIdxOfTlsConnectionDyn(TlsConIdx));
    if(socketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn())
    {
      TcpIp_SetTlsConnectionIdxOfSocketTcpDyn(socketTcpIdx, TCPIP_NO_TLSCONNECTIONIDXOFSOCKETTCPDYN);
      TcpIp_SetSocketDynComIdxOfTlsConnectionDyn(TlsConIdx, TCPIP_NO_SOCKETDYNCOMIDXOFTLSCONNECTIONDYN);
      TcpIp_SetConnectionStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CONNECTION_HS_INACTIVE);

      retVal = E_OK;
    }
  } /* else - retVal already set to E_NOT_OK */

  /* ----- Development Error Report ------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  /* #20 Report an error if necessary. */
  if (retVal != E_OK)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID, TCPIP_TLS_API_ID_TLS_DISALLOCATE_CONNECTION, TCPIP_TLS_E_NO_TLS_CONNECTION);
  }
# else
  TCPIP_DUMMY_STATEMENT(retVal);                                                                                        /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif /* TCPIP_DEV_ERROR_REPORT */
}

# if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls_BindAndListen
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_BindAndListen(
  TcpIp_SocketDynIterType SocketIdx,
  uint16                  LocalPort)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsListenerIterType nextFreeTlsListenerIdx;
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the next free TLS-listener. */
  retVal = TcpIp_TlsServerListener_GetNextFreeListenerIndex(&nextFreeTlsListenerIdx);
  if ((retVal == E_OK) && (nextFreeTlsListenerIdx < TcpIp_GetSizeOfTlsListener()))
  {
    /* #20 Save the port and socket information in the TLS-listener. */
    TcpIp_SetSocketDynIdxOfTlsListener(nextFreeTlsListenerIdx, (TcpIp_SocketDynIdxOfTlsListenerType)SocketIdx);
    TcpIp_SetPortOfTlsListener(nextFreeTlsListenerIdx, LocalPort);
  }

  /* ----- Development Error Report ------------------------------------- */
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report an error if necessary. */
  if (retVal != E_OK)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID, TCPIP_TLS_API_ID_TLS_BIND_AND_LISTEN, TCPIP_TLS_E_NO_LISTENER);
  }
#  endif /* TCPIP_DEV_ERROR_REPORT */

  return retVal;

} /* TcpIp_Tls_BindAndListen() */

# endif /* TCPIP_SUPPORT_ASRTLSSERVER */

/**********************************************************************************************************************
 *  TcpIp_Tls_TcpIpEventCallout
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tls_TcpIpEventCallout(
  TcpIp_SocketOwnerConfigIterType SocketOwnerCfgIdx,
  TcpIp_SocketDynIterType         SocketIdx,
  IpBase_TcpIpEventType           TcpIpEventType)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward the call to the TcpIp Event callout function */
  if ((SocketOwnerCfgIdx < TcpIp_GetSizeOfSocketOwnerConfig()) && (SocketIdx < TcpIp_GetSizeOfSocketDyn()))
  {
    TcpIp_TcpIpEventFunctions(SocketOwnerCfgIdx, SocketIdx, TcpIpEventType);
  }
  else
  {
    /* ----- Development Error Report ------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
    /* Report an error if necessary. */
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID, TCPIP_TLS_API_ID_TLS_TCPEVENT_CALLOUT, TCPIP_TLS_E_INV_ARG);
# endif /* TCPIP_DEV_ERROR_REPORT */
  }

} /* TcpIp_Tls_TcpIpEventCallout() */

/**********************************************************************************************************************
 *  TcpIp_Tls_CloseInternal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tls_CloseInternal(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfSocketDynType socketIdx = TcpIp_GetSocketDynComIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_SizeOfSocketTcpDynType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(socketIdx);

  /* ----- Implementation ----------------------------------------------- */
  if (socketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn())
  {
    /* #10 Forward call to internal TCP */
    TcpIp_CloseTcpSocketFromTls(socketTcpIdx, FALSE);
  }
  else
  {
    /* Memory runtime check fails - report DET */
    TcpIp_Tls_CallDetReportError(TCPIP_TLS_API_ID_TLS_DATA_PROCESSING, TCPIP_TLS_E_INV_RUNTIME_CHECK);
  }
} /* TcpIp_Tls_CloseInternal() */

/**********************************************************************************************************************
 *  TcpIp_Tls_GetUserError
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
*/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_GetUserError(
  TcpIp_SocketIdType                     SocketId,
  TCPIP_P2V(TcpIp_TlsUserErrorType)      UserErrorMemoryPtr,
  TCPIP_P2V(uint32)                      NumOfUserErrorPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsConnectionIterType tlsConIdx = TcpIp_GetSizeOfTlsConnection();
  uint8                       errorId   = TCPIP_TLS_E_NO_ERROR;
  Std_ReturnType              retVal    = E_NOT_OK;

  /* Obtain the TLS connection for the given socket. */
  (void)TcpIp_SocketIdxToTlsConIdx(TCPIP_SOCKET_ID_TO_IDX(SocketId), &tlsConIdx);

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check input parameters. */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* Check parameter 'UserErrorMemoryPtr' and NumOfUserErrorPtr' */
  if ((UserErrorMemoryPtr == NULL_PTR) || (NumOfUserErrorPtr == NULL_PTR) || (*NumOfUserErrorPtr == 0u))
  {
    errorId = TCPIP_TLS_E_PARAM_POINTER;
  }
  /* Check parameter 'SocketId' */
  else if (!(TCPIP_SOCKET_ID_IS_VALID(SocketId) && TCPIP_SOCKET_IDX_IS_TCP(TCPIP_SOCKET_ID_TO_IDX(SocketId))))          /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
  {
    errorId = TCPIP_TLS_E_INV_ARG;
  }
  /* Check if there is a valid TLS connection for this socket. */
  else if (tlsConIdx >= TcpIp_GetSizeOfTlsConnection())
  {
    errorId = TCPIP_TLS_E_NO_TLS_CONNECTION;
  }
  else
# endif /* TCPIP_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward call to internal TLS function to read out the stored TLS user errors and report them to the user */
    retVal = TcpIp_TlsCoreError_GetUserError(tlsConIdx, UserErrorMemoryPtr, NumOfUserErrorPtr);
  }

  /* ----- Development Error Report ------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  /* Report an error if necessary. */
  if (errorId != TCPIP_TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID, TCPIP_TLS_API_ID_TLS_GET_USER_ERROR, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif /* TCPIP_DEV_ERROR_REPORT */

  return retVal;
} /* TcpIp_Tls_GetUserError() */


# if ((TCPIP_SUPPORT_ASRTLS_MASTER_SECRET_ACCESS == STD_ON) && (TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON))
/**********************************************************************************************************************
 *  TcpIp_Tls_GetMasterSecret
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_GetMasterSecret(
  TcpIp_SocketIdType SocketId,
  TCPIP_P2V(uint8)   MasterSecretPtr,
  TCPIP_P2V(uint8)   ClientRandomPtr,
  TCPIP_P2V(uint8)   SessionIdPtr,
  TCPIP_P2V(uint8)   SessionIdLenPtr)
{
  /* DEPRECATED: This API is deprecated since TcpIp@17.01.00. Use TcpIp_Tls_GetTlsSecrets() instead */

  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId = TCPIP_TLS_E_NO_ERROR;
  Std_ReturnType retVal  = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check input parameters. */
#  if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* Check parameter 'SocketId' */
  if (!(TCPIP_SOCKET_ID_IS_VALID(SocketId) && TCPIP_SOCKET_IDX_IS_TCP(TCPIP_SOCKET_ID_TO_IDX(SocketId))))               /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
  {
    errorId = TCPIP_TLS_E_INV_ARG;
  }
  /* Check parameter 'MasterSecretPtr' */
  else if (MasterSecretPtr == NULL_PTR)
  {
    errorId = TCPIP_TLS_E_PARAM_POINTER;
  }
  /* Call the function */
  else
#  endif /* TCPIP_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward call to internal TLS function */
    retVal = TcpIp_Tls12Core_ReadHandshakeParameter(TCPIP_SOCKET_ID_TO_IDX(SocketId), MasterSecretPtr, ClientRandomPtr, SessionIdPtr, SessionIdLenPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  /* Report an error if necessary. */
  if (errorId != TCPIP_TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID, TCPIP_TLS_API_ID_TLS_GET_MASTER_SECRET, errorId);
  }
#  else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif /* TCPIP_DEV_ERROR_REPORT */

  return retVal;
} /* TcpIp_Tls_GetMasterSecret() */
# endif /* (TCPIP_SUPPORT_ASRTLS_MASTER_SECRET_ACCESS == STD_ON) && (TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON) */

# if (TCPIP_SUPPORT_ASRTLS_SECRET_ACCESS == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls_GetTlsSecrets
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_GetTlsSecrets(
  TcpIp_SocketIdType SocketId,
  TCPIP_P2C(TcpIp_Tls12_Secret_Type) Tls12_SecretPtr,
  TCPIP_P2C(TcpIp_Tls13_Secret_Type) Tls13_SecretPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId = TCPIP_TLS_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check input parameters. */
#  if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* Check parameter 'SocketId' */
  if (!(TCPIP_SOCKET_ID_IS_VALID(SocketId) && TCPIP_SOCKET_IDX_IS_TCP(TCPIP_SOCKET_ID_TO_IDX(SocketId))))               /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
  {
    errorId = TCPIP_TLS_E_INV_ARG;
  }
  /* Check parameter 'Tls12_SecretPtr' and 'Tls13_SecretPtr' */
  else if ((Tls12_SecretPtr == NULL_PTR) && (Tls13_SecretPtr == NULL_PTR))
  {
    errorId = TCPIP_TLS_E_PARAM_POINTER;
  }
  else if (   (Tls12_SecretPtr != NULL_PTR)
           && (Tls12_SecretPtr->MasterSecretPtr == NULL_PTR))
  {
    errorId = TCPIP_TLS_E_PARAM_POINTER;
  }
  else if (   (Tls13_SecretPtr != NULL_PTR)
           && (   (Tls13_SecretPtr->ClientSecretPtr == NULL_PTR)
               || (Tls13_SecretPtr->ServerSecretPtr == NULL_PTR)))
  {
    errorId = TCPIP_TLS_E_PARAM_POINTER;
  }
  else
#  endif /* TCPIP_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward call to internal TLS function */
    retVal = TcpIp_TlsCore_ReadTlsSecrets(
      TCPIP_SOCKET_ID_TO_IDX(SocketId),
      Tls12_SecretPtr,
      Tls13_SecretPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  /* Report an error if necessary. */
  if (errorId != TCPIP_TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID, TCPIP_TLS_API_ID_TLS_GET_TLS_SECRETS, errorId);
  }
#  else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif /* TCPIP_DEV_ERROR_REPORT */

  return retVal;
} /* TcpIp_Tls_GetTlsSecrets() */
# endif /* TCPIP_SUPPORT_ASRTLS_SECRET_ACCESS */

# if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls_ServiceChainCertificate
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_ServiceChainCertificate(
  TcpIp_TlsConnectionIterType               TlsConIdx,
  TCPIP_P2C(uint8)                          CertNamePtr,
  uint32                                    CertNameLength,
  TCPIP_P2C(TcpIp_CertDataType)             CertDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId = TCPIP_TLS_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check input parameters */
#  if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if (   (TlsConIdx >= TcpIp_GetSizeOfTlsConnection())                                                                  /* PRQA S 3415 4 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
      || (CertNameLength == 0u))
  {
    errorId = TCPIP_TLS_E_INV_ARG;
  }
  /* Check input certificate parameters */
  else if (   (CertNamePtr == NULL_PTR)
           || (CertDataPtr == NULL_PTR))
  {
    errorId = TCPIP_TLS_E_PARAM_POINTER;
  }
  else
#  endif /* TCPIP_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
#  if (TCPIP_SUPPORT_ASRTLS == STD_ON)
    TcpIp_CertificateIdType certId;

    /* #20 Check if given certificate name is configured, and write certificate data into the found KeyM key */
    if (KeyM_Cert_SearchCert(CertNamePtr, CertNameLength, &certId) == TRUE)
    {
      retVal = KeyM_SetCertificate(certId, (const KeyM_CertDataType *)CertDataPtr);                                     /* PRQA S 0310 */ /* MD_TCPIP_Rule11.3_0310_3305_TcpIp_CertDataType */
    }
    else
    {
      retVal = E_NOT_OK;
    }
#  else
    TCPIP_DUMMY_STATEMENT_CONST(CertNamePtr);                                                                           /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
    TCPIP_DUMMY_STATEMENT_CONST(CertNameLength);                                                                        /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
    TCPIP_DUMMY_STATEMENT_CONST(CertDataPtr);                                                                           /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#  endif
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  /* Report an error if necessary. */
  if (errorId != TCPIP_TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID, TCPIP_TLS_API_ID_TLS_SERVICE_CERTIFIATE, errorId);
  }
#  else
  TCPIP_DUMMY_STATEMENT_CONST(errorId);                                                                                 /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  TCPIP_DUMMY_STATEMENT_CONST(TlsConIdx);                                                                               /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#  endif /* TCPIP_DEV_ERROR_REPORT */

  return retVal;
} /* TcpIp_Tls_ServiceChainCertificate */

/**********************************************************************************************************************
 *  TcpIp_Tls_ServiceRootCertificate
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_ServiceRootCertificate(
  TcpIp_TlsConnectionIterType               TlsConIdx,
  TCPIP_P2C(uint8)                          CertNamePtr,
  uint32                                    CertNameLength,
  TCPIP_P2C(TcpIp_CertDataType)             CertDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8          errorId = TCPIP_TLS_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check input parameters */
#  if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  if (   (TlsConIdx >= TcpIp_GetSizeOfTlsConnection())                                                                  /* PRQA S 3415 4 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
      || (CertNameLength == 0u))
  {
    errorId = TCPIP_TLS_E_INV_ARG;
  }
  /* Check input certificate parameters */
  else if (   (CertNamePtr == NULL_PTR)
           || (CertDataPtr == NULL_PTR))
  {
    errorId = TCPIP_TLS_E_PARAM_POINTER;
  }
  else
#  endif /* TCPIP_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_CertificateIdType certId;

    /* #20 Check if this certificate can be loaded and load it into the KeyM */
    if (KeyM_Cert_SearchCert(CertNamePtr, CertNameLength, &certId) == TRUE)
    {
      retVal = KeyM_SetCertificate(certId, (const KeyM_CertDataType *)CertDataPtr);                                     /* PRQA S 0310 */ /* MD_TCPIP_Rule11.3_0310_3305_TcpIp_CertDataType */
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  /* Report an error if necessary. */
  if (errorId != TCPIP_TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID, TCPIP_TLS_API_ID_TLS_ROOT_CERTIFIATE, errorId);
  }
#  else
  TCPIP_DUMMY_STATEMENT_CONST(errorId);                                                                                 /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  TCPIP_DUMMY_STATEMENT_CONST(TlsConIdx);                                                                               /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#  endif /* TCPIP_DEV_ERROR_REPORT */

  return retVal;
} /* TcpIp_Tls_ServiceRootCertificate */


# endif /* TCPIP_SUPPORT_ASRTLS_ECC */

/**********************************************************************************************************************
 *  TcpIp_Tls_GetRootCertificateId
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls_GetRootCertificateId(
  TcpIp_SocketIdType            SocketId,
  TCPIP_P2V(uint16)             CertIdPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId                         = TCPIP_TLS_E_NO_ERROR;
  Std_ReturnType retVal                 = E_NOT_OK;
  TcpIp_TlsConnectionIterType tlsConIdx = TcpIp_GetSizeOfTlsConnection();

  /* Obtain the TLS connection for the given socket. */
  (void)TcpIp_SocketIdxToTlsConIdx(TCPIP_SOCKET_ID_TO_IDX(SocketId), &tlsConIdx);

  /* ----- Development Error Checks ------------------------------------- */
/* #10 Check input parameters */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* Check parameter 'SocketId' */
  if (!(TCPIP_SOCKET_ID_IS_VALID(SocketId) && TCPIP_SOCKET_IDX_IS_TCP(TCPIP_SOCKET_ID_TO_IDX(SocketId))))               /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
  {
    errorId = TCPIP_TLS_E_INV_ARG;
  }
  /* Check parameter 'CertIdPtr' */
  else if (CertIdPtr == NULL_PTR)
  {
    errorId = TCPIP_TLS_E_PARAM_POINTER;
  }
  /* Check if there is a valid TLS connection for this socket. */
  else if (tlsConIdx >= TcpIp_GetSizeOfTlsConnection())
  {
    errorId = TCPIP_TLS_E_NO_TLS_CONNECTION;
  }
  else
# endif /* TCPIP_DEV_ERROR_DETECT */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward call to internal TLS function */
    retVal = TcpIp_TlsCoreUtil_CheckAndGetRootCertificateId(tlsConIdx, CertIdPtr);
  }
  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  /* Report an error if necessary. */
  if (errorId != TCPIP_TLS_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID, TCPIP_TLS_API_ID_TLS_GET_ROOT_CERTIFIATE_ID, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT_CONST(errorId);                                                                                 /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  TCPIP_DUMMY_STATEMENT_CONST(SocketId);                                                                                /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif /* TCPIP_DEV_ERROR_REPORT */

  return retVal;
}

# define TCPIP_STOP_SEC_CODE
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 *********************************************************************************************************************/

/*
   MICROSAR global MISRA deviations:
   - see file 'WI_MISRAC2004_PES.pdf'

   Justification for module-specific MISRA deviations:

   module global deviations -> see file TcpIp.c

   TLS-specific deviations:

   MD_TcpIp_TlsStatemachine_STCAL_6050: Number of distinct function calls too high (HIS: STCAL = 0..7)
     Reason:     Number of distinct function calls is too high due to the number of different states which are needed
                 to process the TLS message/state. Splitting the state machine into multiple state machines to reduce
                 the number of function calls does affect the readability and understanding of the code.
                 The individual sub-states of the state machine delegate the processing to separate functions, which
                 ensures that the state machines are implemented in a clear and readable manner.
     Risk:       Using too many distinct functions does affect readability and complexity of the code.
     Prevention: Covered by code review and state machine documented in design.

   MD_TcpIp_TlsStatemachine_STMIF_6080: Number of maximum nesting of control structures defined by HIS shall be in
                 range 0..4
     Reason:     Number of maximum nesting control structures is too high due to the parsing of many different elements
                 within one context of the processing of an TLS message/state. Within a message processing chain, many
                 different events have to be considered, which leads to an increased number of logically related nesting
                 control structures. Splitting the nesting control structures would make the logical processing and
                 parsing of the TLS messages/states unnecessarily complicated.
     Risk:       Using too many nesting control structures does affect readability and complexity of the code.
     Prevention: Covered by code review and well documented implementation.

   MD_TcpIp_TlsRuntimecheck_STMIF_6080: Number of maximum nesting control structures defined by HIS shall be in
                 range 0..4
     Reason:     Number of maximum nesting control structures is too high due to many runtime checks, that are required
                 for the safe process. This check(s) verify any index/indices at runtime.
     Risk:       Using too many nesting control structures does affect readability and complexity of the code.
     Prevention: Covered by code review and well documented implementation.

   MD_TcpIp_TlsRxLengthChecks_STMIF: Number of maximum nesting control structures defined by HIS shall be in range 0..4
     Reason:     Number of maximum nesting control structures is too high due to many different length checks that have
                 to be done for received messages.
                 Often length checks even need two steps, first check that the length field is available in the message,
                 and second read the length an check that this length is available in the message.
                 Splitting the nesting control structures would make the logical processing and parsing of the TLS
                 messages unnecessarily complicated and less readable.
     Risk:       Using too many nesting control structures does affect readability and complexity of the code.
     Prevention: Covered by code review and well documented implementation.

   MD_TcpIp_TLS_HARDWARE_DEPENDING: The first operand of this conditional operator is always constant 'true'.
     Reason:     The result of the ternary operator depends on the hardware platform.
     Risk:       None.
     Prevention: Covered by code review.

   MD_TCPIP_Rule2.2_RedundantAssignment: This initialization is redundant. The value of this object is never used
                 before being modified.
     Reason:     Uninitialized assignment may cause compiler warnings.
     Risk:       None.
     Prevention: Covered by code review.

   MD_TCPIP_Rule11.3_0310_3305_TcpIp_CertDataType: Casting to different object pointer type.
     Reason:     Casting from TcpIp_CertDataType to KeyM_CertDataType type to support PSK without KeyM integration.
     Risk:       None.
     Prevention: Covered by code review.

   MD_TCPIP_Rule11.3_0310_3305_SignatureAlgorithm_Type: Casting to different object pointer type.
     Reason:     Casting from pointer to uint8 (part of received message) to pointer to uint16 (length of a signature
                 and hash algorithm tuple).
     Risk:       None.
     Prevention: Covered by code review.

   MD_TcpIp_Tls_2995: The result of this logical operation is always 'true'.
     Reason:     Using macro for range check of received length information, where the third argument is always checked
                 against the TcpIp_GetSizeOfTlsBufferRx(), and therefore the evaluation of the term is redundant. This
                 causes MISRA warnings when checking multiple length information in a row.
     Risk:       None
     Prevention: Covered by code review.

   MD_TcpIp_Tls_2991_2995: The value/result of this expression/logical operation is always 'true'.
     Reason:     Some API patterns have already been applied, although the functionality will only be
     implemented in a future version. This increases the readability of the code in the appropriate places
     Risk:       None
     Prevention: Covered by code review.

   MD_TcpIp_TlsStreaming_STCAL_6050: Number of distinct function calls too high (HIS: STCAL = 0..7)
     Reason:     Number of distinct function calls is too high
                 due to the number of different buffer situations which can arise when decrypting.
                 Splitting the selection into different functions to reduce the number of function calls
                 in one function does affect the readability and understanding of the code.
                 The processing of the different buffer situations is delegated to separate functions,
                 which ensures that the function with too many function calls is implemented in a clear and readable
                 manner.
     Risk:       Using too many distinct functions does affect readability and complexity of the code.
     Prevention: Covered by code review.

   MD_TcpIp_TlsStreaming_STMIF_6080: Number of maximum nesting control structures defined by HIS shall be in range 0..4
     Reason:     Number of maximum nesting control structures is too high
                 due to the number of different buffer situations which can arise when decrypting.
                 Those nested control structures are logically related and splitting them into different functions
                 would affect the readability and understanding of the code.
     Risk:       Using too many nesting control structures does affect readability and complexity of the code.
     Prevention: Covered by code review and well documented implementation.

   MD_TcpIp_TlsStreaming_STPTH_6010: Estimated static path count defined by HIS shall be in range 1..80
     Reason:     Number of static path is too high
                 due to the distinction between the different buffer situations which can arise when performing the
                 streaming operations.
                 Splitting the distinction for the different buffer situations would affect the readability and
                 understanding of the code.
     Risk:       Using too many static path does affect readability and complexity of the code.
     Prevention: Covered by code review.

   MD_TcpIp_TlsStreaming_STCYC_6030: Cyclomatic complexity defined by HIS shall be in range 1..10
     Reason:     Cyclomatic complexity is too high due to the distinction between the different buffer
                 situations which can arise when performing the streaming operations.
                 Splitting the distinction for the different buffer situations would affect the readability and
                 understanding of the code.
     Risk:       Too high cyclomatic complexity does affect readability and complexity of the code.
     Prevention: Covered by code review and state machine documented in design.

   MD_TcpIp_TlsResetConnection_STCAL_6050: Number of distinct function calls too high (HIS: STCAL = 0..7)
     Reason:     Number of distinct function calls is too high because when reseting a connection
                 all submodules need to be initialized which is executed through a function call.
                 Splitting the selection into different functions to reduce the number of function calls
                 does not make sense logically and affects the readability and understanding of the code.
                 The readability and understanding is ensured by the fact
                 that no other possibly complex code is part of the function besides the function calls.
     Risk:       Using too many distinct functions does affect readability and complexity of the code.
     Prevention: Covered by code review.

   MD_TcpIp_TlsStatemachine_STCYC_6030: Cyclomatic complexity defined by HIS shall be in range 1..10
     Reason:     Cyclomatic complexity is too high due to the distinction between the states of a state machine.
                 This function is reentrant and delegates the tasks in the different states
                 to limit the complexity of the function.
                 Splitting the distinction between the states of a state machine
                 would make the logical processing and parsing of the states unnecessarily complicated.
     Risk:       Too high cyclomatic complexity does affect readability and complexity of the code.
     Prevention: Covered by code review and state machine documented in design.

   MD_TcpIp_TlsPointerAndLength_STPAR_6060: Number of function parameters defined by HIS shall be in range 0..5
     Reason:     Number of function parameters is too high due to the fact that pointer
                 and their length are passed in separate parameters.
                 Combining them into a data type would add additional code
                 to combine and access the respective parameters and make the code unnecessarily complicated.
     Risk:       Using too many function parameters does affect readability and complexity of the code.
     Prevention: Covered by code review.

   MD_TcpIp_Tls13KeyDerivation_STPAR_6060: Number of function parameters defined by HIS shall be in range 0..5
     Reason:     Number of function parameters is too high due to the fact that the key derivation in TLS 1.3 is used
                 multiple times for slightly different purposes (secret derivation / key derivation) and further
                 requires additional parameters for CSM jobs.
                 Combining them into a data type would add additional code
                 to combine and access the respective parameters and make the code unnecessarily complicated.
     Risk:       Using too many function parameters does affect readability and complexity of the code.
     Prevention: Covered by code review.

   MD_TcpIp_WrapperFunction_STPAR_6060: Number of function parameters defined by HIS shall be in range 0..5
     Reason:     Number of function parameters is too high due to the fact that this is a wrapper function defined by
                 AUTOSAR. The original API also has a number of function parameter that is too high.
     Risk:       Using too many function parameters does affect readability and complexity of the code.
     Prevention: Covered by code review.

   MD_TcpIp_TlsStatemachine_STPTH_6010: Estimated static path count defined by HIS shall be in range 1..80
     Reason:     Number of static path is too high
                 due to the distinction between the states of a state machine.
                 This function is reentrant and delegates the tasks in the different states
                 to limit the complexity of the function.
                 Splitting the distinction between the states of a state machine
                 would make the logical processing and parsing of the states unnecessarily complicated.
     Risk:       Using too many static path does affect readability and complexity of the code.
     Prevention: Covered by code review.

   MD_TcpIp_TlsCore_10.5_4342_a:
     Reason:     An expression of essential type has been cast to an enum type.
     Risk:       The essential type is not in range of enum type.
     Prevention: The essential type is checked by SW before it will be used.

   MD_TcpIp_TLS_BUILD_CHECK_STRUCT:
     Reason:      This struct only exists to verify at compile time that our pointer size assumption holds on the
                  current platform. So it is asserted that it is never used.
     Risk:        None.
     Prevention:  Covered by code review.

   MD_TcpIp_TLS_PTR_ADDR_CAST:
     Reason:      For later address calculation the address must be converted to integer values. It is ensured at
                  compile time with struct VerifyPtrStructNotForUsage, that the used unsigned integer is big enough
                  to store the address value.
     Risk:        Address calculation makes the code complex.
     Prevention:  Covered by code review.

*/

/**********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 *********************************************************************************************************************/

/* VCA_JUSTIFICATION_START

  \ID VCA_TCPIP_TLS_COPY_TX_DATA_FUNCTIONS
    \DESCRIPTION       Function TcpIp_CopyTxDataFunctions() is called with valid 'BufferPtr' and 'BufferLenBytePtr'
                       values.
    \COUNTERMEASURE \N The forwarded parameters are restricted by spec requires statements of the calling function
                       TcpIp_Tls12CoreUl_CopyTxDataIndirectWithWraparound().
                       It is ensured by review that:
                        - The given pointer is not modified nor an offset is added.
                        - The buffer length is not modified.

  \ID VCA_TCPIP_TLS_BUFFER_SEGMENT
    \DESCRIPTION       A pointer to a buffer, managed by a buffer segment object (TcpIp_SegmentType), is used for write
                       operations or passed to an external function for write operations.
    \COUNTERMEASURE \N The buffer segment object ensures the consistency of all the buffer segment properties.
                       The 'Size' setting specifies the size of the buffer behind 'BasePtr', whereas the 'AvailLen'
                       setting describes the remaining size of the buffer behind 'AvailPtr'.
                       A buffer segment object is not allowed to be modified in any way except by dedicated and secured
                       buffer segment utility functions.
                        - TcpIp_SegmentInit         (TCASE-TCPIP_000713)
                        - TcpIp_SegmentCommit       (TCASE-TCPIP_000714)
                        - TcpIp_SegmentMemCpy       (TCASE-TCPIP_000724)
                        - TcpIp_SegmentMultiTargetMemCpy  (TCASE-TCPIP_000723)
                        - TcpIp_SegmentClear        (TCASE-TCPIP_000722)
                       The proper usage of a buffer segment is ensured by review.

  \ID VCA_TCPIP_TLS_CSM_RANDOM_GENERATE
    \DESCRIPTION        Function TCPIP_CSM_RandomGenerate() is called with valid 'resultPtr' and 'resultLengthPtr'
                        values.
    \COUNTERMEASURE \R  The forwarded parameters have their origin in the well defined TcpIp_SegmentType. (See
                        VCA_TCPIP_TLS_BUFFER_SEGMENT)
                        The used 'resultLengthPtr' is checked to be within the valid range by function
                        TcpIp_SegmentAssertMinLen().
                        The usage of function TcpIp_SegmentAssertMinLen() is ensured by the traced TCASEs, the
                        correctness of the function itself is ensured by the following tests:
                         - TCASE-TCPIP_000715

  \ID VCA_TCPIP_TLS_VERIFIED_START_END_IDX
    \DESCRIPTION        A function is called with valid Buffer and BufferLength values.
    \COUNTERMEASURE \R  It is ensured via function TcpIp_VerifyStartAndEndIndex(), that the used start and end indices
                        are valid indices within the buffer.
                        The usage of function TcpIp_VerifyStartAndEndIndex() is ensured by the traced TCASEs, the
                        correctness of the function itself is ensured by the following tests:
                         - TCASE-TCPIP_000718
                         - TCASE-TCPIP_000719

  \ID VCA_TCPIP_TLS_VERIFIED_TCP_TX_BUFFER
    \DESCRIPTION        The buffer and the length used to initialize a buffer segment object via TcpIp_SegmentInit()
                        is within a valid range of the TcpIp_TcpTxBuffer[] buffer.
    \COUNTERMEASURE \R  It is ensured via function TcpIp_Tls12CoreRl_CheckTcpTxBufferInRange(), that the used segment
                        pointer and the segment length are valid within the buffer.
                        The usage of function TcpIp_Tls12CoreRl_CheckTcpTxBufferInRange() is ensured by the traced
                        TCASEs, the correctness of the function itself by the following tests:
                         - TCASE-TCPIP_000699
                         - TCASE-TCPIP_000700
                         - TCASE-TCPIP_000701
                         - TCASE-TCPIP_000702
                         - TCASE-TCPIP_000712
                         - TCASE-TCPIP_000720
                         - TCASE-TCPIP_000721
                        The checked buffer pointer is translated to a VCA understandable pointer by a call to function
                        TcpIp_Tls12CoreRl_FixGetTcpTxBuffer(). The usage of the function is ensured by the traced
                        TCASEs, the correctness of the function itself by the following tests:
                         - TCASE-TCPIP_000697
                         - TCASE-TCPIP_000698

  \ID VCA_TCPIP_TLS_CSM_DECRYPT_CSL01
    \DESCRIPTION        Function TCPIP_CSM_Decrypt() is invoked. The function parameter 'resultPtr' points to a
                        buffer with a size of at least 'ResultLengthPtr' bytes.
    \COUNTERMEASURE \R  [CM_TCPIP_TLS_R_WRITE_INDEX_AND_BUFFER_AREA_LENGTH_CSL01]
    \COUNTERMEASURE \T  TCASE-TCPIP_000728

  \ID VCA_TCPIP_TLS_CSM_AEAD_DECRYPT_CSL01
    \DESCRIPTION        Function TCPIP_CSM_AEADDecrypt() is invoked. The function parameter 'resultPtr' points to a
                        buffer with a size of at least 'ResultLengthPtr' bytes.
    \COUNTERMEASURE \R  [CM_TCPIP_TLS_R_WRITE_INDEX_AND_BUFFER_AREA_LENGTH_CSL01]
    \COUNTERMEASURE \T  TCASE-TCPIP_000730

  \ID VCA_TCPIP_TLS_CSM_KEY_EXCHANGE_CSL01
    \DESCRIPTION        Function TCPIP_CSM_KeyExchangeCalcPubVal() is invoked. The function parameter 'resultPtr' points
                        to a buffer with a size of at least 'ResultLengthPtr' bytes.
    \COUNTERMEASURE \R  [CM_TCPIP_TLS_R_WRITE_INDEX_AND_BUFFER_AREA_LENGTH_CSL01]
    \COUNTERMEASURE \T  TCASE-TCPIP_000732

  \ID VCA_TCPIP_TLS_CSM_MAC_GENERATE_CSL01
    \DESCRIPTION        Function TCPIP_CSM_MacGenerate() is invoked. The function parameter 'resultPtr' points to
                        a buffer with a size of at least 'ResultLengthPtr' bytes.
    \COUNTERMEASURE \R  [CM_TCPIP_TLS_R_WRITE_INDEX_AND_BUFFER_AREA_LENGTH_CSL01]
    \COUNTERMEASURE \T  TCASE-TCPIP_000734

  \ID VCA_TCPIP_TLS_COPY_TX_DATA_FIRST_PAIR
    \DESCRIPTION        Parameter tlsSeg1Ptr points to a buffer with at least tlsSeg1Len bytes of data.
    \COUNTERMEASURE \R  The pointer points to the TcpIp_TlsBufferTx[] buffer at index 'txBufferWriteIdx'.
                        The length parameter is 'SourceDataLen'.
                        The 'txBufferEndIdx' of the buffer is derived via CSL03 compliant use-case.
                        It is ensured by runtime check ((txBufferWriteIdx + SourceDataLen) <= txBufferEndIdx) that
                        the combination of tlsSeg1Ptr and tlsSeg1Len is valid.
                        Qualified use-case CSL03 of ComStackLib.

  \ID VCA_TCPIP_TLS_COPY_TX_DATA_SECOND_PAIR_NULL_PTR
    \DESCRIPTION        The buffer pointer is explicitly set to NULL_PTR and the length to 0.
    \COUNTERMEASURE \R  The second buffer is not required and hence, set to NULL_PTR.

  \ID VCA_TCPIP_TLS_COPY_TX_DATA_FIRST_PAIR_WRAPAROUND
    \DESCRIPTION        Parameter tlsSeg1Ptr points to a buffer with at least tlsSeg1Len bytes of data.
    \COUNTERMEASURE \R  The pointer points to the TcpIp_TlsBufferTx[] buffer at index 'txBufferWriteIdx'.
                        A runtime check ensures that the 'txBufferWriteIdx' is always smaller as the CLS03 compliant
                        'txBufferEndIdx' of the buffer.
                        The length parameter is set to 'txBufferEndIdx - txBufferWriteIdx'.
                        This ensures, that the combination of tlsSeg1Ptr and tlsSeg1Len is valid.
                        Qualified use-case CSL03 of ComStackLib.

  \ID VCA_TCPIP_TLS_COPY_TX_DATA_SECOND_PAIR_WRAPAROUND
    \DESCRIPTION        Parameter tlsSeg2Ptr points to a buffer with at least tlsSeg2Len bytes of data.
    \COUNTERMEASURE \R  The pointer points to the TcpIp_TlsBufferTx[] buffer at its CSL03 compliant start index
                        'txBufferStartIdx'.
                        The length parameter is 'SourceDataLen' - 'tlsSeg1Len'.
                        It is ensured by runtime check (SourceDataLen <= (txBufferEndIdx - txBufferStartIdx)) that the
                        'SourceDataLen' is smaller or equal as the entire buffer and bigger as 'tlsSeg1Len'.
                        Hence, the combination of tlsSeg2Ptr and tlsSeg2Len is valid.
                        Qualified use-case CSL03 of ComStackLib.

  \ID VCA_TCPIP_TLS_COPY_TX_DATA
    \DESCRIPTION        Function is called with valid combinations of Buffer and BufferLength.
    \COUNTERMEASURE \N  The function TcpIp_Tls12CoreUl_CopyTxDataIndirectWithWraparound() takes two pairs of
                        buffer-pointer and buffer-length:
                        - DstChunk1Ptr, DstChunk1LenPtr
                        - DstChunk2Ptr, DstChunk2LenPtr
                        It is ensured by additional asserts at the parameter assignment that the pairs are within the
                        requested range. See the corresponding justifications:
                        - VCA_TCPIP_TLS_COPY_TX_DATA_FIRST_PAIR
                        - VCA_TCPIP_TLS_COPY_TX_DATA_SECOND_PAIR_NULL_PTR
                        - VCA_TCPIP_TLS_COPY_TX_DATA_FIRST_PAIR_WRAPAROUND
                        - VCA_TCPIP_TLS_COPY_TX_DATA_SECOND_PAIR_WRAPAROUND

 VCA_JUSTIFICATION_END */

/*
  \CM CM_TCPIP_TLS_R_WRITE_INDEX_AND_BUFFER_AREA_LENGTH_CSL01
    Ensure that the sum of the untrusted write index and the untrusted buffer area length does not exceed the trusted
    total buffer size. Ensure also, that no uint32 overflow can occur during the sum calculation by using
      - a uint64 cast for at least one of the summands to enforce a proper integer conversion.
      - a VCA assertion for generated TcpIp_SizeOfTcpRxBufferType and TcpIp_SizeOfTcpTxBufferType, to enforce a data
        type length of 32 bits or less.
      - a compile time check for generated TcpIp_SizeOfTcpRxBufferType and TcpIp_SizeOfTcpTxBufferType, to enforce a
        data type length of 32 bits or less.
    Qualified use case CSL01 of ComStackLib, since the validity of the check is based solely on the total buffer size
    provided by the ComStackLib.
*/

#endif /* (TCPIP_SUPPORT_ASRTLS == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Tls.c
 *********************************************************************************************************************/
