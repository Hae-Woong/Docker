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
/*!        \file  SoAd_SoCon.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component SoCon.
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

/* PRQA S 1881 EOF */ /* MD_MSR_AutosarBoolean */
/* PRQA S 4304 EOF */ /* MD_MSR_AutosarBoolean */

#define SOAD_SO_CON_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd_EventQueue.h"
#include "SoAd_TimeoutList.h"
#include "SoAd_Util.h"
#include "SoAd_SoCon.h"
#include "SoAd_Tx.h"
#include "SoAd_Rx.h"
#include "SoAd_Anomaly.h"
#include "SoAd_GenTcpIpApi.h"
#include "SoAd_State_Int.h"
#include "SoAd_LocalAddr_Int.h"
#include "SoAd_RemoteAddr_Int.h"
#include "SoAd_BestMatch_Int.h"
#include "SoAd_TxSocketManager_Int.h"
#include "SoAd_Priv.h"

#if ( (SOAD_TCP == STD_ON) || (SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON) )
# if ( SOAD_RUNTIME_ERROR_REPORT == STD_ON )
# include "Det.h"
# endif /* SOAD_RUNTIME_ERROR_REPORT == STD_ON */
#endif /* (SOAD_TCP == STD_ON) || (SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON) */
#include "IpBase.h"

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
#endif

#if !defined (SOAD_LOCAL_INLINE) /* COV_SOAD_COMPATIBILITY */
# define SOAD_LOCAL_INLINE LOCAL_INLINE
#endif

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
 *  SoAd_SoCon_UdpChangeParameter()
 *********************************************************************************************************************/
/*! \brief      Changes parameters on a UDP socket via the Socket API.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  SocketId        Socket identifier.
 *  \return     E_OK            Parameter change request was accepted.
 *  \return     E_NOT_OK        Parameter change request was not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \trace      DSGN-SoAdHandleSockets, DSGN-SoAdUdpRetry
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_UdpChangeParameter(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_TcpChangeParameterNoDelay()
 *********************************************************************************************************************/
/*! \brief      Changes no delay (Nagle algorithm) parameter on a TCP socket via the Socket API.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  SocketId        Socket identifier.
 *  \return     E_OK            Parameter change request was accepted or not required.
 *  \return     E_NOT_OK        Parameter change request was not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameterNoDelay(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId);

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpChangeParameterKeepAlive()
 *********************************************************************************************************************/
/*! \brief      Changes keep alive parameters on a TCP socket via the Socket API.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  SocketId        Socket identifier.
 *  \return     E_OK            Parameter change requests were accepted or not required.
 *  \return     E_NOT_OK        Parameter change requests were not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameterKeepAlive(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId);

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpChangeParameterTls()
 *********************************************************************************************************************/
/*! \brief      Changes TLS parameter on a TCP socket via the Socket API.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  SocketId        Socket identifier.
 *  \return     E_OK            Parameter change request was accepted or not required.
 *  \return     E_NOT_OK        Parameter change request was not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameterTls(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId);

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpChangeParameterMsl()
 *********************************************************************************************************************/
/*! \brief      Changes MSL parameter on a TCP socket via the Socket API.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  SocketId        Socket identifier.
 *  \return     E_OK            Parameter change request was accepted or not required.
 *  \return     E_NOT_OK        Parameter change request was not accepted.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameterMsl(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId);

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpAcceptedCheckAcceptance()
 *********************************************************************************************************************/
/*! \brief      Checks connection acceptance on a socket connection on connection establishment on listen socket.
 *  \details    -
 *  \param[in]  SoConIdx            Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx        Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  SocketIdConnected   Connected socket identifier.
 *  \param[in]  RemoteAddrPtr       Pointer to remote address.
 *                                  [Points to one of the following structs depending on configured IP address version
 *                                  of parameter SoConIdx:
 *                                    - SoAd_SockAddrInetType for IPv4
 *                                    - SoAd_SockAddrInet6Type for IPv6]
 *  \return     E_OK                Socket connection accepted connection establishment.
 *  \return     E_NOT_OK            Socket connection rejected connection establishment.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpAcceptedCheckAcceptance(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketIdConnected,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr);

#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_EventUdpClose()
 *********************************************************************************************************************/
/*! \brief      Handles UDP close event on socket.
 *  \details    -
 *  \param[in]  SockIdx      Module internal socket index.
 *                           [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]  PartitionIdx Partition index.
 *                           [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket indexes, FALSE for same socket index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_EventUdpClose(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_EventUdpCloseAllSoCons()
 *********************************************************************************************************************/
/*! \brief      Closes all related socket connections on UDP close event on socket.
 *  \details    -
 *  \param[in]  SockIdx      Module internal socket index.
 *                           [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]  PartitionIdx Partition index.
 *                           [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket indexes, FALSE for same socket index.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_EventUdpCloseAllSoCons(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_SetCloseMode()
 *********************************************************************************************************************/
/*! \brief        Sets close mode considering priority of requested and current close mode.
 *  \details      Has to be called in critical section.
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    CloseMode       Requested close mode.
 *                                [range: SOAD_CLOSE_SOCKET_RECONNECT .. SOAD_CLOSE_OFFLINE_RESET]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_SetCloseMode(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConCloseModeType CloseMode);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_EventTcpClose()
 *********************************************************************************************************************/
/*! \brief      Handles TCP close and reset event on socket.
 *  \details    -
 *  \param[in]  SockIdx      Module internal socket index.
 *                           [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]  PartitionIdx Partition index.
 *                           [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket indexes, FALSE for same socket index.
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_EventTcpClose(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_EventTcpFinReceived()
 *********************************************************************************************************************/
/*! \brief      Handles TCP FIN received event on socket.
 *  \details    -
 *  \param[in]  SockIdx      Module internal socket index.
 *                           [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]  PartitionIdx Partition index.
 *                           [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket indexes, FALSE for same socket index.
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_EventTcpFinReceived(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_CheckAndOpen()
 *********************************************************************************************************************/
/*! \brief      Performs open actions to open a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \trace      DSGN-SoAdAutomaticOpenAndClose, DSGN-SoAdOpenOnReception
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CheckAndOpen(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_CheckAndOpenSocketForOpenOnReception()
 *********************************************************************************************************************/
/*! \brief      Performs open actions for a UDP socket to support socket connection open on reception.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CheckAndOpenSocketForOpenOnReception(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_Open()
 *********************************************************************************************************************/
/*! \brief      Opens a socket connection and the related socket.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \trace      DSGN-SoAdHandleSockets
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_Open(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketUdp()
 *********************************************************************************************************************/
/*! \brief      Opens a socket for a UDP socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK            Socket is opened or already assigned.
 *  \return     E_NOT_OK        Socket is not opened.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketUdp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_AssignSocketUdp()
 *********************************************************************************************************************/
/*! \brief      Assigns a socket for a UDP socket connection and updates the state.
 *  \details    -
 *  \param[in]  SockIdx         Module internal socket index.
 *                              [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  LocalPort       Local port.
 *  \return     E_OK            Socket is opened or already assigned.
 *  \return     E_NOT_OK        Socket is not opened.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_AssignSocketUdp(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_PortType LocalPort);

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSoConUdp()
 *********************************************************************************************************************/
/*! \brief      Opens UDP socket connection if a socket is already assigned.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_OpenSoConUdp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpClient()
 *********************************************************************************************************************/
/*! \brief      Opens a socket for a TCP client socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK            Socket is assigned and connects to remote entity.
 *  \return     E_NOT_OK        Socket is not assigned or is already connecting.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpClient(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpClientGetAndConnect()
 *********************************************************************************************************************/
/*! \brief      Gets a socket for a TCP client socket connection and starts connecting to remote entity.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  SockIdx         Module internal socket index.
 *                              [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \return     E_OK            Socket is assigned and connects to remote entity.
 *  \return     E_NOT_OK        Socket is not assigned.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpClientGetAndConnect(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SizeOfSocketType SockIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpClientConnect()
 *********************************************************************************************************************/
/*! \brief      Starts connecting to remote entity on a socket for a TCP client socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  SockIdx         Module internal socket index.
 *                              [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]  SocketId        Socket identifier.
 *  \param[in]  LocalPort       Local port.
 *  \return     E_OK            Socket is assigned and connects to remote entity.
 *  \return     E_NOT_OK        Socket is not assigned.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpClientConnect(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SizeOfSocketType SockIdx,
  SoAd_SocketIdType SocketId,
  SoAd_PortType LocalPort);

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpServer()
 *********************************************************************************************************************/
/*! \brief      Opens a socket for a TCP server socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(),
 *                              SoAd_IsSocketTcpUsedOfSoConGrp(),
 *                              !SoAd_IsTcpInitiateOfSocketTcp()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     E_OK            Socket is assigned and listens for connection attempts from remote entity.
 *  \return     E_NOT_OK        Socket is not assigned or is already listening.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpServer(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpServerGetAndListen()
 *********************************************************************************************************************/
/*! \brief      Gets a socket for a TCP server socket connection and starts listening for connection attempts.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  ListenSockIdx   Module internal socket index of listen socket.
 *                              [range: ListenSockIdx < SoAd_GetSizeOfSocket()]
 *  \return     E_OK            Socket is assigned and listens for connection attempts from remote entity.
 *  \return     E_NOT_OK        Socket is not assigned.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpServerGetAndListen(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SizeOfSocketType ListenSockIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpServerListen()
 *********************************************************************************************************************/
/*! \brief      Starts listening for connection attempts on a socket for a TCP server.
 *  \details    -
 *  \param[in]  ListenSockIdx   Module internal socket index of listen socket.
 *                              [range: ListenSockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  SocketId        Socket identifier.
 *  \param[in]  LocalPort       Local port.
 *  \return     E_OK            Socket is assigned and listens for connection attempts from remote entity.
 *  \return     E_NOT_OK        Socket is not assigned.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpServerListen(
  SoAd_SizeOfSocketType ListenSockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId,
  SoAd_PortType LocalPort);

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSoConTcp()
 *********************************************************************************************************************/
/*! \brief      Opens TCP socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_OpenSoConTcp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_StartTcpAutoConnectTimeout()
 *********************************************************************************************************************/
/*! \brief      Starts a not yet running TCP auto connect timeout on a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(),
 *                              SoAd_IsSocketTcpUsedOfSoConGrp()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_StartTcpAutoConnectTimeout(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_ResetTcpAutoConnectTimeout()
 *********************************************************************************************************************/
/*! \brief      Resets the TCP auto connect timeout on a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(),
 *                              SoAd_IsSocketTcpUsedOfSoConGrp()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_ResetTcpAutoConnectTimeout(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_Close()
 *********************************************************************************************************************/
/*! \brief      Closes a socket connection and the related sockets.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_Close(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseSoCon()
 *********************************************************************************************************************/
/*! \brief      Closes a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  CloseMode       Requested close mode.
 *                              [range: SOAD_CLOSE_SOCKET_RECONNECT .. SOAD_CLOSE_OFFLINE_RESET]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CloseSoCon(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConCloseModeType CloseMode);

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseSocket()
 *********************************************************************************************************************/
/*! \brief      Closes socket connection related sockets.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  Abort           Flag to close socket connection immediately.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CloseSocket(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean Abort);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_CloseSocketTcp()
 *********************************************************************************************************************/
/*! \brief      Closes TCP socket connection related socket.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  Abort           Flag to close socket connection immediately.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CloseSocketTcp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean Abort);

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseSocketTcpListen()
 *********************************************************************************************************************/
/*! \brief      Closes TCP socket connection related listen socket if no other socket connection requires the socket.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  Abort           Flag to close socket connection immediately.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \config     SOAD_TCP
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CloseSocketTcpListen(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean Abort);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseSocketUdp()
 *********************************************************************************************************************/
/*! \brief      Closes UDP socket connection related socket if no other socket connection requires the socket.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  Abort           Flag to close socket connection immediately.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CloseSocketUdp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean Abort);

/**********************************************************************************************************************
 *  SoAd_SoCon_ResetCloseMode()
 *********************************************************************************************************************/
/*! \brief        Resets close mode considering socket connection and socket states.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_ResetCloseMode(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_AddSoConModeChgQueueElement()
 *********************************************************************************************************************/
/*! \brief      Adds a socket connection mode change to the queue.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  Mode            Socket connection mode.
 *                              [range: SOAD_SOCON_ONLINE, SOAD_SOCON_RECONNECT, SOAD_SOCON_OFFLINE]
 *  \param[out] ErrorIdPtr      Pointer to error identifier (set if error occurred).
 *  \return     TRUE            Mode change callback shall be immediately called.
 *  \return     FALSE           Mode change callback shall not be immediately called.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \trace      DSGN-SoAdNotifySocketConnectionStateChange
 *  \config     SOAD_MODE_CHG_QUEUE_ENABLED
 *  \note       This function must be called in a critical section.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_SoCon_AddSoConModeChgQueueElement(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConModeType Mode,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr);
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_CheckAndNotifySoConModeChg()
 *********************************************************************************************************************/
/*! \brief      Checks if notification is required immediately and notifies users about socket connection mode change.
 *  \details    The ErrorId is checked and runtime error is reported if required. Depending on value of ImmediateCbk,
 *              the mode change is reported or delayed to the main function.
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  Mode            Socket connection mode.
 *                              [range: SOAD_SOCON_ONLINE, SOAD_SOCON_RECONNECT, SOAD_SOCON_OFFLINE]
 *  \param[in]  ImmediateCbk    Identifies if mode change callback shall be called immediately.
 *                              [range: TRUE: Mode change shall be notified immediately.
 *                                      FALSE: Mode change callback shall be delayed and handled in main function.]
 *  \param[in]  ErrorId         Identifies if error occured and shall be reported.
 *                              [range: SOAD_E_NO_ERROR, SOAD_E_NO_MODE_CHG_QUEUE_ELEM]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \trace      DSGN-SoAdNotifySocketConnectionStateChange
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_SoCon_CheckAndNotifySoConModeChg(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConModeType Mode,
  boolean ImmediateCbk,
  uint8 ErrorId);

/**********************************************************************************************************************
 *  SoAd_SoCon_NotifySoConModeChg()
 *********************************************************************************************************************/
/*! \brief      Notifies users about a socket connection mode change.
 *  \details    Several mode changes may be reported in case the mode change callback queue is used.
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  Mode            Socket connection mode.
 *                              [range: SOAD_SOCON_ONLINE, SOAD_SOCON_RECONNECT, SOAD_SOCON_OFFLINE]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *  \trace      DSGN-SoAdNotifySocketConnectionStateChange
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_SoCon_NotifySoConModeChg(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConModeType Mode);

/**********************************************************************************************************************
 *  SoAd_SoCon_ResetLocalPortDyn()
 *********************************************************************************************************************/
/*! \brief        Resets local port of a socket.
 *  \details      Considers that a local port is shared by multiple sockets/socket connections.
 *  \param[in]    SockIdx       Module internal socket index.
 *                              [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_SoCon_ResetLocalPortDyn(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_ResetSoCon()
 *********************************************************************************************************************/
/*! \brief      Resets socket connection specific properties.
 *  \details    -
 *  \param[in]  SoConIdx      Socket connection index.
 *                            [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]  PartitionIdx  Partition index.
 *                            [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]  ResetRemAddr  Indicates if remote address shall be reset.
 *  \param[in]  ResetOpClSeq  Indicates if open/close sequence shall be reset.
 *  \param[in]  NormalClose   Indicates if socket connection is closed by normal close request.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_ResetSoCon(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean ResetRemAddr,
  boolean ResetOpClSeq,
  boolean NormalClose);

/**********************************************************************************************************************
 *  SoAd_SoCon_TriggerAddressResolution()
 *********************************************************************************************************************/
/*! \brief        Triggers address resolution for a remote address on a socket connection (ARP/NDP).
 *  \details      -
 *  \param[in]    SoConIdx      Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_TriggerAddressResolution(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_SetUdpAliveTimeout()
 *********************************************************************************************************************/
/*! \brief      Sets the UDP alive supervision timeout on a socket connection.
 *  \details    -
 *  \param[in]  SoConIdx        Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon(),
 *                              SoAd_IsSocketUdpUsedOfSoConGrp()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_SetUdpAliveTimeout(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_DecrementOpenSockets()
 *********************************************************************************************************************/
/*! \brief      Decrements the number of open sockets.
 *  \details    -
 *  \param[in]  InstanceIdx     Instance index.
 *                              [range: InstanceIdx < SoAd_GetSizeOfInstance()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_DecrementOpenSockets(
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_SoCon_UdpChangeParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_UdpChangeParameter(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType  soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_SizeOfLocalAddrType localAddrIdx = SoAd_GetLocalAddrIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType           retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set frame priority. */
  if ( SoAd_GetFramePriorityOfSoConGrp(soConGrpIdx, PartitionIdx) != SOAD_NO_FRAMEPRIORITYOFSOCONGRP )
  {
    uint8 framePriority = SoAd_GetFramePriorityOfSoConGrp(soConGrpIdx, PartitionIdx);

    retVal = SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_FRAMEPRIO, &framePriority);                           /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  }

  /* #20 Set UDP Tx request list size. */
  if ( SoAd_IsImmedIfTxConfOfSocketUdp(socketUdpIdx, PartitionIdx) )
  {
    uint8 udpTxReqListSize = SoAd_GetImmedIfTxConfListSizeOfSocketUdp(socketUdpIdx, PartitionIdx);

    retVal |= SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_UDP_TXREQLISTSIZE, &udpTxReqListSize);               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  }

  /* #30 Set UDP Tx retry list size (number of Eth Tx buffers used in case of physical address miss). */
  if (SoAd_LocalAddr_IsRetryEnabled(localAddrIdx, PartitionIdx) )
  {
    uint8 udpTxRetryListSize = SoAd_GetRetryQueueLimitOfSocketUdp(socketUdpIdx, PartitionIdx);

    retVal |= SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_UDP_TXRETRYLISTSIZE, &udpTxRetryListSize);           /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  }

  /* #40 Enable/Disable UDP checksum if UDP checksum shall be changed for the socket. */
  if ( SoAd_IsChecksumChangeEnabledOfSocketUdp(socketUdpIdx, PartitionIdx) )
  {
    boolean checksumEnabled = (boolean)SoAd_IsChecksumEnabledOfSocketUdp(socketUdpIdx, PartitionIdx);

    retVal |= SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_UDP_CHECKSUM, SOAD_A_P2VAR(uint8)&checksumEnabled);  /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  }

  return retVal;
} /* SoAd_SoCon_UdpChangeParameter() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_TcpChangeParameterNoDelay()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameterNoDelay(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSocketTcpType  socketTcpIdx = SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType            retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  switch ( SoAd_GetTcpNoDelayOfSocketTcp(socketTcpIdx, PartitionIdx) )
  {
    /* #10 Change parameter if no delay (Nagle algorithm) is enabled. */
    case SOAD_TCP_NO_DELAY_ENABLED:
    {
      uint8 value = 0x00u;

      retVal = SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_TCP_NAGLE, &value);                                 /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      break;
    }
    /* #20 Change parameter if no delay (Nagle algorithm) is disabled. */
    case SOAD_TCP_NO_DELAY_DISABLED:
    {
      uint8 value = 0x01u;

      retVal = SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_TCP_NAGLE, &value);                                 /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      break;
    }
    /* #30 Do not change parameter if no delay (Nagle algorithm) is not configured. */
    default:
    { /* SOAD_TCP_NO_DELAY_NOT_SPECIFIED */
      /* nothing to do */
      break;
    }
  }

  return retVal;
} /* SoAd_SoCon_TcpChangeParameterNoDelay() */

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpChangeParameterKeepAlive()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameterKeepAlive(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSocketTcpType  socketTcpIdx = SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType            retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if keep alive is enabled globally. */
  if ( SoAd_IsTcpKeepAliveGloballyEnabled(0u) )
  {
    boolean tcpKeepAlive = (boolean)SoAd_IsTcpKeepAliveOfSocketTcp(socketTcpIdx, PartitionIdx);

    /* #100 Enable/Disable keep alive. */
    retVal = SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_TCP_KEEPALIVE,                                        /* SBSW_SOAD_CAST_POINTER_FORWARD */
      SOAD_A_P2VAR(uint8)&tcpKeepAlive);

    /* #101 Check if keep alive could be set. */
    if ( (retVal == E_OK) && (tcpKeepAlive == TRUE) )
    {
      uint32 tcpKeepAliveTime = SoAd_GetTcpKeepAliveTimeOfSocketTcp(socketTcpIdx, PartitionIdx);
      uint32 tcpKeepAliveInterval = SoAd_GetTcpKeepAliveIntervalOfSocketTcp(socketTcpIdx, PartitionIdx);
      uint16 tcpKeepAliveProbesMax = SoAd_GetTcpKeepAliveProbesMaxOfSocketTcp(socketTcpIdx, PartitionIdx);

      /* #1010 Set keep alive time. */
      if ( tcpKeepAliveTime != SOAD_NO_TCPKEEPALIVETIMEOFSOCKETTCP )
      {
        retVal = SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_TCP_KEEPALIVE_TIME,                               /* SBSW_SOAD_CAST_POINTER_FORWARD */
          SOAD_A_P2VAR(uint8)&tcpKeepAliveTime); /* PRQA S 310 */ /* MD_SoAd_PointerCastOnParam */
      }
      /* #1011 Set keep alive maximum probes. */
      if ( tcpKeepAliveProbesMax != SOAD_NO_TCPKEEPALIVEPROBESMAXOFSOCKETTCP )
      {
        retVal |= SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_TCP_KEEPALIVE_PROBES_MAX,                        /* SBSW_SOAD_CAST_POINTER_FORWARD */
          SOAD_A_P2VAR(uint8)&tcpKeepAliveProbesMax); /* PRQA S 310 */ /* MD_SoAd_PointerCastOnParam */
      }
      /* #1012 Set keep alive interval. */
      if ( tcpKeepAliveInterval != SOAD_NO_TCPKEEPALIVEINTERVALOFSOCKETTCP )
      {
        retVal |= SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_TCP_KEEPALIVE_INTERVAL,                          /* SBSW_SOAD_CAST_POINTER_FORWARD */
          SOAD_A_P2VAR(uint8)&tcpKeepAliveInterval); /* PRQA S 310 */ /* MD_SoAd_PointerCastOnParam */
      }
    }
  }

  return retVal;
} /* SoAd_SoCon_TcpChangeParameterKeepAlive() */

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpChangeParameterTls()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameterTls(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketTcpType  socketTcpIdx = SoAd_GetSocketTcpIdxOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx,
    PartitionIdx), PartitionIdx);
  uint16                    tcpTlsConId = SoAd_GetTcpTlsConIdOfSocketTcp(socketTcpIdx, PartitionIdx);
  Std_ReturnType            retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket connection references a TLS connection. */
  if ( tcpTlsConId != SOAD_NO_TCPTLSCONIDOFSOCKETTCP )
  {
    /* #100 Set TLS connection identifier. */
    retVal = SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_TLS_CONNECTION_ASSIGNMENT, (uint8*)&tcpTlsConId);     /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  }

  return retVal;
} /* SoAd_SoCon_TcpChangeParameterTls() */

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpChangeParameterMsl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameterMsl(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketTcpType  socketTcpIdx = SoAd_GetSocketTcpIdxOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx,
    PartitionIdx), PartitionIdx);
  uint32                    tcpMslValue = SoAd_GetTcpMslOfSocketTcp(socketTcpIdx, PartitionIdx);
  Std_ReturnType            retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket uses a specific MSL value. */
  if ( tcpMslValue != SOAD_NO_TCPMSLOFSOCKETTCP )
  {
    /* #100 Set MSL value. */
    retVal = SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_TCP_MSL, (uint8*)&tcpMslValue);                       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  }

  return retVal;
} /* SoAd_SoCon_TcpChangeParameterMsl() */

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpAcceptedCheckAcceptance()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpAcceptedCheckAcceptance(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketIdConnected,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketType   sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that socket state and socket connection mode is set in other context. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if socket connection related socket state is closed. */
  if ( SoAd_GetStateOfSocketDyn(sockIdx, PartitionIdx) == SOAD_SOCK_STATE_CLOSED )
  {
    boolean            immediateCbk = TRUE;
    SoAd_SoConModeType newSoConMode = SOAD_SOCON_ONLINE;
    uint8              errorId = SOAD_E_NO_ERROR;

    /* #200 Assign socket identifier to module internal socket index. */
    SoAd_SetSocketIdOfSocketDyn(sockIdx, SocketIdConnected, PartitionIdx);                                             /* SBSW_SOAD_CSL02_CSL05 */

    /* #201 Try to update remote address on socket connection. */
    /* Validity of RemoteAddrPtr is already checked within SoAd_TcpAccepted(). */
    (void)SoAd_RemoteAddr_Set(SoConIdx, PartitionIdx, RemoteAddrPtr, FALSE, FALSE);                                    /* SBSW_SOAD_POINTER_FORWARD */

    /* #202 Set socket state and socket connection mode. */
    SoAd_SetStateOfSocketDyn(sockIdx, SOAD_SOCK_STATE_ESTABLISHED, PartitionIdx);                                      /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_Util_FetchAddU32(SoAd_GetAddrOpenSocksOfInstanceDyn(instanceIdx, PartitionIdx), SOAD_INC_DEC_BY_ONE, TRUE);   /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    SoAd_SetModeOfSoConDyn(SoConIdx, newSoConMode, PartitionIdx);                                                      /* SBSW_SOAD_PARAMETER_IDX */

# if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
    /* #203 Add mode change to the mode change queue if enabled. */
    immediateCbk = SoAd_SoCon_AddSoConModeChgQueueElement(SoConIdx, PartitionIdx, newSoConMode, &errorId);             /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
# endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

    /* #204 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();

    /* #205 Notify user about mode change. */
    SoAd_SoCon_CheckAndNotifySoConModeChg(SoConIdx, PartitionIdx, newSoConMode, immediateCbk, errorId);

    /* #206 Accept connection establishment if socket connection has no SocketRoute. */
    if ( SoAd_GetSocketRouteStartIdxOfSoCon(SoConIdx, PartitionIdx) == SoAd_GetSocketRouteEndIdxOfSoCon(SoConIdx,
        PartitionIdx) )
    {
      retVal = E_OK;
    }
    /* #207 Accept connection establishment if TP reception can be started otherwise. */
    else if ( SoAd_Rx_TpStartOfReception(SoConIdx, PartitionIdx) == E_OK )
    {
      retVal = E_OK;
    }
    else
    {
      /* Nothing to do. */
    }
  }
  /* #21 Otherwise leave critical section. */
  else
  {
    SOAD_END_CRITICAL_SECTION();
  }

  return retVal;
} /* SoAd_SoCon_TcpAcceptedCheckAcceptance() */

#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_EventUdpClose()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_EventUdpClose(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent inconsistencies in socket and socket connection states. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Reset map of module internal socket index to socket identifier. */
  SoAd_SetSocketIdOfSocketDyn(SockIdx, SOAD_INV_SOCKET_ID, PartitionIdx);                                              /* SBSW_SOAD_PARAMETER_IDX */

  /* #30 Check if socket is not yet in closed state. */
  if ( SoAd_GetStateOfSocketDyn(SockIdx, PartitionIdx) != SOAD_SOCK_STATE_CLOSED )
  {
    /* #40 Decrement number of open sockets. */
    SoAd_SoCon_DecrementOpenSockets(instanceIdx, PartitionIdx);

    /* #50 Set socket state. */
    SoAd_SetStateOfSocketDyn(SockIdx, SOAD_SOCK_STATE_CLOSED, PartitionIdx);                                           /* SBSW_SOAD_PARAMETER_IDX */

    /* #60 Reset active transmission on related socket. */
    SoAd_TxSocketManager_ResetTxActive(soConIdx, PartitionIdx, FALSE);

    /* #70 Reset local port to the configured value. */
    SoAd_SoCon_ResetLocalPortDyn(SockIdx, PartitionIdx);

    /* #80 Close all related socket connections. */
    SoAd_SoCon_EventUdpCloseAllSoCons(SockIdx, PartitionIdx);
  }

  /* #90 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();
} /* SoAd_SoCon_EventUdpClose() */

/**********************************************************************************************************************
 *  SoAd_SoCon_EventUdpCloseAllSoCons()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_EventUdpCloseAllSoCons(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIterType      soConIter; /* Not initialized: For-loop counter. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all related socket connection to close them. */
  for ( soConIter = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);
    soConIter < SoAd_GetSoConEndIdxOfSocket(SockIdx, PartitionIdx);
    soConIter++ )
  {
    /* #20 Check if socket connection is not closed. */
    if ( SoAd_GetModeOfSoConDyn(soConIter, PartitionIdx) != SOAD_SOCON_OFFLINE )
    {
      const SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIter, PartitionIdx);

      /* #30 Set close mode to indicate that socket is closed. */
      SoAd_SoCon_SetCloseMode((SoAd_SizeOfSoConType)soConIter, PartitionIdx, SOAD_CLOSE_SOCKET);

      /* #40 Set event to handle socket connection state in main function. */
      SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, (SoAd_SizeOfSoConType)soConIter,
        SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);
    }
  }
} /* SoAd_SoCon_EventUdpCloseAllSoCons() */

/**********************************************************************************************************************
 *  SoAd_SoCon_SetCloseMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_SetCloseMode(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConCloseModeType CloseMode)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if new close mode has higher priority than current close mode. */
  if ( CloseMode > SoAd_GetCloseModeOfSoConDyn(SoConIdx, PartitionIdx) )
  {
    /* #20 Set close mode to requested close mode. */
    SoAd_SetCloseModeOfSoConDyn(SoConIdx, CloseMode, PartitionIdx);                                                    /* SBSW_SOAD_PARAMETER_IDX */
  }
} /* SoAd_SoCon_SetCloseMode() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_EventTcpClose()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_EventTcpClose(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent inconsistencies in socket and socket connection states. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Reset map of module internal socket index to socket identifier. */
  SoAd_SetSocketIdOfSocketDyn(SockIdx, SOAD_INV_SOCKET_ID, PartitionIdx);                                              /* SBSW_SOAD_PARAMETER_IDX */

  /* #30 Check if socket is not yet in closed state. */
  if ( SoAd_GetStateOfSocketDyn(SockIdx, PartitionIdx) != SOAD_SOCK_STATE_CLOSED )
  {
    /* #40 Decrement number of open sockets. */
    SoAd_SoCon_DecrementOpenSockets(instanceIdx, PartitionIdx);

    /* #50 Set socket state. */
    SoAd_SetStateOfSocketDyn(SockIdx, SOAD_SOCK_STATE_CLOSED, PartitionIdx);                                           /* SBSW_SOAD_PARAMETER_IDX */

    /* #60 Reset active transmission on related socket. */
    SoAd_TxSocketManager_ResetTxActive(soConIdx, PartitionIdx, FALSE);

    /* #70 Reset local port to the configured value. */
    SoAd_SoCon_ResetLocalPortDyn(SockIdx, PartitionIdx);

    /* #80 Check if socket is no listen socket. */
    if ( SockIdx != SoAd_GetSocketIdxOfSoConGrp(soConGrpIdx, PartitionIdx) )
    {
      /* #800 Set close mode to indicate that socket is closing and shall be reconnected in case of client socket. */
      if ( SoAd_IsTcpInitiateOfSocketTcp(SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
      {
        SoAd_SoCon_SetCloseMode(soConIdx, PartitionIdx, SOAD_CLOSE_SOCKET_RECONNECT);
      }
      /* #801 Set close mode to indicate that socket is closing otherwise. */
      else
      {
        SoAd_SoCon_SetCloseMode(soConIdx, PartitionIdx, SOAD_CLOSE_SOCKET);
      }

      /* #802 Set event to handle socket connection state in main function. */
      SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, soConIdx, instanceIdx, PartitionIdx);
    }
    /* #81 Set event to handle state in main function for all socket connections related to listen socket otherwise. */
    else
    {
      SoAd_SoConIterType soConIter;

      for ( soConIter = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);
        soConIter < SoAd_GetSoConEndIdxOfSocket(SockIdx, PartitionIdx);
        soConIter++ )
      {
        SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, (SoAd_SizeOfSoConType)soConIter, instanceIdx,
          PartitionIdx);
      }
    }
  }

  /* #90 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();
} /* SoAd_SoCon_EventTcpClose() */

/**********************************************************************************************************************
 *  SoAd_SoCon_EventTcpFinReceived()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_EventTcpFinReceived(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SocketIdType       socketId = SoAd_GetSocketIdOfSocketDyn(SockIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to check socket identifier validity and the socket state. */
  SOAD_BEGIN_CRITICAL_SECTION();

  if ( SoAd_GetStateOfSocketDyn(SockIdx, PartitionIdx) != SOAD_SOCK_STATE_CLOSED )
  {
    /* #20 Set close mode to indicate that socket is closing and shall be reconnected in case of client socket. */
    if ( SoAd_IsTcpInitiateOfSocketTcp(SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
    {
      SoAd_SoCon_SetCloseMode(soConIdx, PartitionIdx, SOAD_CLOSE_SOCKET_RECONNECT);
    }
    /* #21 Set close mode to indicate that socket is closing otherwise. */
    else
    {
      SoAd_SoCon_SetCloseMode(soConIdx, PartitionIdx, SOAD_CLOSE_SOCKET);
    }

    /* #30 Set event to handle socket connection state in main function. */
    SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, soConIdx, SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx,
        PartitionIdx), PartitionIdx);

    /* #40 Set socket to closing. */
    SoAd_SetStateOfSocketDyn(SockIdx, SOAD_SOCK_STATE_CLOSING, PartitionIdx);                                          /* SBSW_SOAD_PARAMETER_IDX */

    /* #50 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();

    /* #60 Close socket.*/
    (void)SoAd_TcpIpApiClose(socketId, FALSE);
  }
  else
  {
    /* #70 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();
  }
} /* SoAd_SoCon_EventTcpFinReceived() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_CheckAndOpen()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CheckAndOpen(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfLocalAddrType  localAddrIdx = SoAd_GetLocalAddrIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if module is initialized and not in shutdown state. */
  if ( SoAd_State_GetState() == SOAD_STATE_INIT )
  {
    /* #20 Check if socket connection has automatic setup, is in reconnect state or is requested to be opened. */
    if ( SoAd_IsSockAutoSoConSetupOfSoConGrp(soConGrpIdx, PartitionIdx) ||
      (SoAd_GetModeOfSoConDyn(SoConIdx, PartitionIdx) == SOAD_SOCON_RECONNECT) ||
      (SoAd_GetOpenCloseCounterOfSoConDyn(SoConIdx, PartitionIdx) > 0u) )
    {
      /* #200 Check if socket connection is not online yet and IP address is assigned. */
      /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
      if ( (SoAd_GetModeOfSoConDyn(SoConIdx, PartitionIdx) != SOAD_SOCON_ONLINE) &&
        (SoAd_LocalAddr_GetState(localAddrIdx, PartitionIdx) == SOAD_IPADDR_STATE_ASSIGNED) )
      {
        /* #201 Try to open socket connection. */
        SoAd_SoCon_Open(SoConIdx, PartitionIdx);
      }
    }
    /* #21 Check if socket shall be opened to enable socket connection open on reception otherwise. */
    else
    {
      SoAd_SoCon_CheckAndOpenSocketForOpenOnReception(SoConIdx, PartitionIdx);
    }
  }
} /* SoAd_SoCon_CheckAndOpen() */

/**********************************************************************************************************************
 *  SoAd_SoCon_CheckAndOpenSocketForOpenOnReception()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CheckAndOpenSocketForOpenOnReception(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketType     sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfLocalAddrType  localAddrIdx = SoAd_GetLocalAddrIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if IP is assigned, socket is not yet opened and protocol type is UDP. */
  if ( (SoAd_LocalAddr_GetState(localAddrIdx, PartitionIdx) == SOAD_IPADDR_STATE_ASSIGNED) &&
    (SoAd_GetStateOfSocketDyn(sockIdx, PartitionIdx) == SOAD_SOCK_STATE_CLOSED) &&
    SoAd_IsSocketUdpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) )
  {
    /* #20 Check if message acceptance filter is enabled and UDP listen only is disabled. */
    if ( SoAd_IsMsgAcceptFilterEnabledOfSoConGrp(soConGrpIdx, PartitionIdx) &&
      !SoAd_IsUdpListenOnlyOfSocketUdp(SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
    {
      /* #30 Check if configured remote address contains wildcards. */
      if ( SoAd_RemoteAddr_HasConfigWildcard(SoConIdx, PartitionIdx, TRUE) == TRUE )
      {
        /* #40 Request to open the UDP socket. */
        (void)SoAd_SoCon_OpenSocketUdp(SoConIdx, PartitionIdx);
      }
    }
  }
} /* SoAd_SoCon_CheckAndOpenSocketForOpenOnReception() */

/**********************************************************************************************************************
 *  SoAd_SoCon_Open()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_Open(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
#endif /* SOAD_TCP == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  /* #10 Handle socket connection open sequence for TCP (if enabled). */
  if ( SoAd_IsSocketTcpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) )
  {
    /* #20 Handle socket connection open sequence for TCP client. */
    if ( SoAd_IsTcpInitiateOfSocketTcp(SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
    {
      /* #200 Assert that no timeout has previously occurred. */
      if ( !SoAd_IsTimeoutOccurredOfSoConDyn(SoConIdx, PartitionIdx) )
      {
        /* #2000 Try to open TCP client socket. */
        if ( SoAd_SoCon_OpenSocketTcpClient(SoConIdx, PartitionIdx) == E_OK )
        {
          /* #20000 Handle TCP socket connection state for TCP client. */
          SoAd_SoCon_OpenSoConTcp(SoConIdx, PartitionIdx);
        }
      }
    }
    /* #30 Handle socket connection open sequence for TCP server. */
    else
    {
      /* #300 Open TCP server (i.e. listen) socket. */
      if ( SoAd_SoCon_OpenSocketTcpServer(SoConIdx, PartitionIdx) == E_OK )
      {
        /* #3000 Handle TCP socket connection state for TCP server. */
        SoAd_SoCon_OpenSoConTcp(SoConIdx, PartitionIdx);
      }
    }
  }
  else
#endif /* SOAD_TCP == STD_ON */
  /* #40 Handle socket connection open sequence for UDP (if enabled). */
  {
    /* #400 Open UDP socket. */
    if ( SoAd_SoCon_OpenSocketUdp(SoConIdx, PartitionIdx) == E_OK )
    {
      /* #4000 Open UDP socket connection. */
      SoAd_SoCon_OpenSoConUdp(SoConIdx, PartitionIdx);
    }
  }
} /* SoAd_SoCon_Open() */

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketUdp()
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketUdp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSocketType     sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfLocalAddrType  localAddrIdx = SoAd_GetLocalAddrIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_SocketIdType         socketId = SOAD_INV_SOCKET_ID;
  Std_ReturnType            retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if no socket is assigned. */
  if ( SoAd_GetStateOfSocketDyn(sockIdx, PartitionIdx) == SOAD_SOCK_STATE_CLOSED )
  {
    /* #20 Get socket. */
    if ( SoAd_TcpIpApiSoAdGetSocket(SoAd_LocalAddr_GetDomain(localAddrIdx, PartitionIdx), SOAD_IPPROTO_UDP, &socketId) /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      == E_OK )
    {
      SoAd_PortType localPort = (SoAd_PortType)IPBASE_HTON16(SoAd_SoCon_GetLocalPortDyn(sockIdx, PartitionIdx));

      /* #30 Set socket identifier before it may be reset due to interrupts. */
      SoAd_SetSocketIdOfSocketDyn(sockIdx, socketId, PartitionIdx);                                                    /* SBSW_SOAD_PARAMETER_IDX */

      /* #40 Bind socket. */
      if ( SoAd_TcpIpApiBind(socketId,
          SoAd_LocalAddr_GetTcpIpLocalAddrId(localAddrIdx, PartitionIdx), &localPort) == E_OK )                        /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      {
        /* #50 Change parameter on socket. */
        /* Socket is bound before changing the socket parameter to enable the TcpIp to reject changing parameter
         * depending on controller specific configuration. */
        if ( SoAd_SoCon_UdpChangeParameter(SoConIdx, PartitionIdx, socketId) == E_OK )
        {
          /* #60 Assign socket and update state. */
          retVal = SoAd_SoCon_AssignSocketUdp(sockIdx, PartitionIdx, localPort);
        }
      }

      /* #70 Close socket if any error occurred. */
      if ( retVal != E_OK )
      {
        (void)SoAd_TcpIpApiClose(socketId, FALSE);
      }
    }
  }
  else
  {
    retVal = E_OK;
  }

  /* #80 If no socket is assigned set event to handle socket connection state in main function again. */
  if ( retVal != E_OK )
  {
    SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, SoConIdx,
      SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);
  }

  return retVal;
} /* SoAd_SoCon_OpenSocketUdp() */

/**********************************************************************************************************************
 *  SoAd_SoCon_AssignSocketUdp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_AssignSocketUdp(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_PortType LocalPort)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that socket state is overwritten by any function call. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Assign socket and update local port and state in case of a valid socket identifier. */
  if ( SoAd_GetSocketIdOfSocketDyn(SockIdx, PartitionIdx) != SOAD_INV_SOCKET_ID )
  {
    SoAd_SoCon_SetLocalPortDyn(SockIdx, PartitionIdx, (SoAd_PortType)IPBASE_HTON16(LocalPort));
    SoAd_SetStateOfSocketDyn(SockIdx, SOAD_SOCK_STATE_ESTABLISHED, PartitionIdx);                                      /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_Util_FetchAddU32(SoAd_GetAddrOpenSocksOfInstanceDyn(instanceIdx, PartitionIdx), SOAD_INC_DEC_BY_ONE, TRUE);   /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

    retVal = E_OK;
  }

  /* #30 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_SoCon_AssignSocketUdp() */

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSoConUdp()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_OpenSoConUdp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_RemAddrStateType   remAddrState = SoAd_RemoteAddr_GetState(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that socket connection mode is set in other context. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if remote address is set or UDP listen only is configured. */
  if ( (remAddrState == SOAD_SOCON_IP_SET_PORT_SET) ||
    SoAd_IsUdpListenOnlyOfSocketUdp(SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
  {
    boolean            immediateCbk = TRUE;
    SoAd_SoConModeType newSoConMode = SOAD_SOCON_ONLINE;
    uint8              errorId = SOAD_E_NO_ERROR;

    /* #200 Set socket connection to online. */
    SoAd_SetModeOfSoConDyn(SoConIdx, newSoConMode, PartitionIdx);                                                      /* SBSW_SOAD_PARAMETER_IDX */

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
    /* #201 Add mode change to the mode change queue if enabled. */
    immediateCbk = SoAd_SoCon_AddSoConModeChgQueueElement(SoConIdx, PartitionIdx, newSoConMode, &errorId);             /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

    /* #202 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();

    /* #203 Trigger address resolution if remote address is set. */
    if ( remAddrState == SOAD_SOCON_IP_SET_PORT_SET )
    {
      SoAd_SoCon_TriggerAddressResolution(SoConIdx, PartitionIdx);
    }

    /* #204 Notify user about mode change. */
    SoAd_SoCon_CheckAndNotifySoConModeChg(SoConIdx, PartitionIdx, newSoConMode, immediateCbk, errorId);

    /* #205 Start TP sessions by calling StartOfReception if required. */
    (void)SoAd_Rx_TpStartOfReception(SoConIdx, PartitionIdx);
  }
  /* #21 Set socket connection mode to reconnect (if not done before). */
  else if ( SoAd_GetModeOfSoConDyn(SoConIdx, PartitionIdx) != SOAD_SOCON_RECONNECT )
  {
    boolean            immediateCbk = TRUE;
    SoAd_SoConModeType newSoConMode = SOAD_SOCON_RECONNECT;
    uint8              errorId = SOAD_E_NO_ERROR;

    /* #210 Set socket connection to reconnect. */
    SoAd_SetModeOfSoConDyn(SoConIdx, newSoConMode, PartitionIdx);                                                      /* SBSW_SOAD_PARAMETER_IDX */

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
    /* #211 Add mode change to the mode change queue if enabled. */
    immediateCbk = SoAd_SoCon_AddSoConModeChgQueueElement(SoConIdx, PartitionIdx, newSoConMode, &errorId);             /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

    /* #212 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();

    /* #213 Notify user about mode change. */
    SoAd_SoCon_CheckAndNotifySoConModeChg(SoConIdx, PartitionIdx, newSoConMode, immediateCbk, errorId);
  }
  /* #22 Leave critical section otherwise. */
  else
  {
    SOAD_END_CRITICAL_SECTION();
  }
} /* SoAd_SoCon_OpenSoConUdp() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpClient()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpClient(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketType   sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if no socket is assigned and remote address is set. */
  /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
  if ( (SoAd_GetStateOfSocketDyn(sockIdx, PartitionIdx) == SOAD_SOCK_STATE_CLOSED) &&
    (SoAd_RemoteAddr_GetState(SoConIdx, PartitionIdx) == SOAD_SOCON_IP_SET_PORT_SET) )
  {
    /* #20 Try to get and connect socket. */
    retVal = SoAd_SoCon_OpenSocketTcpClientGetAndConnect(SoConIdx, PartitionIdx, sockIdx);

    /* #30 If no socket is assigned set event to handle socket connection state in main function again. */
    if ( retVal != E_OK )
    {
      SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, SoConIdx,
        SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);
    }

    /* #40 Start TCP auto connect timeout handling. */
    SoAd_SoCon_StartTcpAutoConnectTimeout(SoConIdx, PartitionIdx);
  }

  return retVal;
} /* SoAd_SoCon_OpenSocketTcpClient() */

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpClientGetAndConnect()
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpClientGetAndConnect(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SizeOfSocketType SockIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SocketIdType         socketId = SOAD_INV_SOCKET_ID;
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfLocalAddrType  localAddrIdx = SoAd_GetLocalAddrIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType            retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get socket. */
  if ( SoAd_TcpIpApiSoAdGetSocket(SoAd_LocalAddr_GetDomain(localAddrIdx, PartitionIdx), SOAD_IPPROTO_TCP, &socketId)   /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    == E_OK )
  {
    /* #20 Change parameter on socket. */
    if ( SoAd_SoCon_TcpChangeParameter(SoConIdx, PartitionIdx, socketId) == E_OK )
    {
      SoAd_PortType localPort = (SoAd_PortType)IPBASE_HTON16(SoAd_SoCon_GetLocalPortDyn(SockIdx, PartitionIdx));

      /* #30 Set socket identifier before it may be reset due to interrupts. */
      SoAd_SetSocketIdOfSocketDyn(SockIdx, socketId, PartitionIdx);                                                    /* SBSW_SOAD_PARAMETER_IDX */

      /* #40 Bind socket. */
      if ( SoAd_TcpIpApiBind(socketId, SoAd_LocalAddr_GetTcpIpLocalAddrId(
        localAddrIdx, PartitionIdx), &localPort) == E_OK )                                                             /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      {
        /* #50 Start connect and assign socket. */
        retVal = SoAd_SoCon_OpenSocketTcpClientConnect(SoConIdx, PartitionIdx, SockIdx, socketId, localPort);
      }
    }

    /* #60 Close socket if any error occurred. */
    if ( retVal != E_OK )
    {
      (void)SoAd_TcpIpApiClose(socketId, FALSE);
    }
  }

  return retVal;
} /* SoAd_SoCon_OpenSocketTcpClientGetAndConnect() */

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpClientConnect()
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
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpClientConnect(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SizeOfSocketType SockIdx,
  SoAd_SocketIdType SocketId,
  SoAd_PortType LocalPort)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_SockAddrInet6Type  remAddr = { 0u, 0u, { 0u, 0u, 0u, 0u } };
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* PRQA S 310 5 */ /* MD_SoAd_PointerCastOnStruct */
  /* #10 Get remote address. */
  SoAd_RemoteAddr_Get(SoConIdx, PartitionIdx, SOAD_A_P2VAR(SoAd_SockAddrType)&remAddr);                                /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

  /* #20 Start connect. */
  if ( SoAd_TcpIpApiTcpConnect(SocketId, SOAD_A_P2VAR(SoAd_SockAddrType)&remAddr) == E_OK )                            /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* #30 Enter critical section to prevent that socket state is overwritten by any function call. */
    SOAD_BEGIN_CRITICAL_SECTION();

    /* #40 Assign socket and update local port and state in case of a valid socket identifier. */
    if ( SoAd_GetSocketIdOfSocketDyn(SockIdx, PartitionIdx) != SOAD_INV_SOCKET_ID )
    {
      SoAd_SoCon_SetLocalPortDyn(SockIdx, PartitionIdx, (SoAd_PortType)IPBASE_HTON16(LocalPort));
      SoAd_SetStateOfSocketDyn(SockIdx, SOAD_SOCK_STATE_CONNECT, PartitionIdx);                                        /* SBSW_SOAD_PARAMETER_IDX */
      SoAd_Util_FetchAddU32(SoAd_GetAddrOpenSocksOfInstanceDyn(instanceIdx, PartitionIdx), SOAD_INC_DEC_BY_ONE, TRUE); /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

      retVal = E_OK;
    }

    /* #50 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();
  }

  return retVal;
} /* SoAd_SoCon_OpenSocketTcpClientConnect() */

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpServer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpServer(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  /* Optional indirection: Refer to Safe BSW assumption SoConGrp -> Socket. */
  SoAd_SizeOfSocketType   listenSockIdx = SoAd_GetSocketIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType          retVal; /* Not initialized: Improved readability. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if no listen socket is assigned. */
  if ( SoAd_GetStateOfSocketDyn(listenSockIdx, PartitionIdx) == SOAD_SOCK_STATE_CLOSED )
  {
    /* #20 Try to get socket and start listening. */
    retVal = SoAd_SoCon_OpenSocketTcpServerGetAndListen(SoConIdx, PartitionIdx, listenSockIdx);

    /* #30 If no socket is assigned set event to handle socket connection state in main function again. */
    if ( retVal != E_OK )
    {
      SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, SoConIdx,
        SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);
    }
  }
  else
  {
    retVal = E_OK;
  }

  return retVal;
} /* SoAd_SoCon_OpenSocketTcpServer() */

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpServerGetAndListen()
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpServerGetAndListen(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SizeOfSocketType ListenSockIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SocketIdType         socketId = SOAD_INV_SOCKET_ID;
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfLocalAddrType  localAddrIdx = SoAd_GetLocalAddrIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType            retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get socket. */
  if ( SoAd_TcpIpApiSoAdGetSocket(SoAd_LocalAddr_GetDomain(localAddrIdx, PartitionIdx), SOAD_IPPROTO_TCP, &socketId)   /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    == E_OK )
  {
    /* #20 Change parameter on socket. */
    if ( SoAd_SoCon_TcpChangeParameter(SoConIdx, PartitionIdx, socketId) == E_OK )
    {
      SoAd_PortType localPort = (SoAd_PortType)IPBASE_HTON16(SoAd_SoCon_GetLocalPortDyn(ListenSockIdx, PartitionIdx));

      /* #30 Set socket identifier before it may be reset due to interrupts. */
      SoAd_SetSocketIdOfSocketDyn(ListenSockIdx, socketId, PartitionIdx);                                              /* SBSW_SOAD_PARAMETER_IDX */

      /* #40 Bind socket. */
      if ( SoAd_TcpIpApiBind(socketId, SoAd_LocalAddr_GetTcpIpLocalAddrId(
        localAddrIdx, PartitionIdx), &localPort) == E_OK )                                                             /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
      {
        /* #50 Start listen and assign socket. */
        retVal = SoAd_SoCon_OpenSocketTcpServerListen(ListenSockIdx, PartitionIdx, socketId, localPort);
      }
    }

    /* #60 Close socket if any error occurred. */
    if ( retVal != E_OK )
    {
      (void)SoAd_TcpIpApiClose(socketId, FALSE);
    }
  }

  return retVal;
} /* SoAd_SoCon_OpenSocketTcpServerGetAndListen() */

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSocketTcpServerListen()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSocketTcpServerListen(
  SoAd_SizeOfSocketType ListenSockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId,
  SoAd_PortType LocalPort)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType    soConIdx = SoAd_GetSoConStartIdxOfSocket(ListenSockIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Start listen. */
  /* PRQA S 4391 2 */ /* MD_SoAd_GenericWiderTypeCast */
  if ( SoAd_TcpIpApiTcpListen(SocketId,
      (uint16)(SoAd_GetSoConEndIdxOfSocket(ListenSockIdx, PartitionIdx) - SoAd_GetSoConStartIdxOfSocket(ListenSockIdx,
        PartitionIdx))) == E_OK )
  {
    /* #20 Enter critical section to prevent that socket state is overwritten by any function call. */
    SOAD_BEGIN_CRITICAL_SECTION();

    /* #30 Assign socket and update local port and state in case of a valid socket identifier. */
    if ( SoAd_GetSocketIdOfSocketDyn(ListenSockIdx, PartitionIdx) != SOAD_INV_SOCKET_ID )
    {
      SoAd_SoCon_SetLocalPortDyn(ListenSockIdx, PartitionIdx, (SoAd_PortType)IPBASE_HTON16(LocalPort));
      SoAd_SetStateOfSocketDyn(ListenSockIdx, SOAD_SOCK_STATE_LISTEN, PartitionIdx);                                   /* SBSW_SOAD_PARAMETER_IDX */
      SoAd_Util_FetchAddU32(SoAd_GetAddrOpenSocksOfInstanceDyn(instanceIdx, PartitionIdx), SOAD_INC_DEC_BY_ONE, TRUE); /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

      retVal = E_OK;
    }

    /* #40 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();
  }

  return retVal;
} /* SoAd_SoCon_OpenSocketTcpServerListen() */

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSoConTcp()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_OpenSoConTcp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that socket connection mode is set in other context. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if socket connection is already set to reconnect. */
  if ( SoAd_GetModeOfSoConDyn(SoConIdx, PartitionIdx) != SOAD_SOCON_RECONNECT )
  {
    boolean            immediateCbk = TRUE;
    SoAd_SoConModeType newSoConMode = SOAD_SOCON_RECONNECT;
    uint8              errorId = SOAD_E_NO_ERROR;

    /* #200 Set socket connection to reconnect. */
    SoAd_SetModeOfSoConDyn(SoConIdx, newSoConMode, PartitionIdx);                                                      /* SBSW_SOAD_PARAMETER_IDX */

# if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
    /* #201 Add mode change to the mode change queue if enabled. */
    immediateCbk = SoAd_SoCon_AddSoConModeChgQueueElement(SoConIdx, PartitionIdx, newSoConMode, &errorId);             /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
# endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

    /* #202 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();

    /* #203 Notify user about socket connection mode change. */
    SoAd_SoCon_CheckAndNotifySoConModeChg(SoConIdx, PartitionIdx, newSoConMode, immediateCbk, errorId);
  }
  /* #21 Leave critical section otherwise. */
  else
  {
    SOAD_END_CRITICAL_SECTION();
  }
} /* SoAd_SoCon_OpenSoConTcp() */

/**********************************************************************************************************************
 *  SoAd_SoCon_StartTcpAutoConnectTimeout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_StartTcpAutoConnectTimeout(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType  soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSocketTcpType socketTcpIdx = SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if TCP auto connect timeout is configured. */
  if ( SoAd_GetTcpAutoConnectTimeoutOfSocketTcp(socketTcpIdx, PartitionIdx) !=
    SOAD_NO_TCPAUTOCONNECTTIMEOUTOFSOCKETTCP )
  {
    SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

    /* #20 Set timeout if it is not yet running. */
    (void)SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT, SoConIdx,
      SoAd_GetTcpAutoConnectTimeoutOfSocketTcp(socketTcpIdx, PartitionIdx), instanceIdx, PartitionIdx);
  }
} /* SoAd_SoCon_StartTcpAutoConnectTimeout() */

/**********************************************************************************************************************
 *  SoAd_SoCon_ResetTcpAutoConnectTimeout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_ResetTcpAutoConnectTimeout(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType  soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSocketTcpType socketTcpIdx = SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if TCP auto connect timeout is configured. */
  if ( SoAd_GetTcpAutoConnectTimeoutOfSocketTcp(socketTcpIdx, PartitionIdx) !=
    SOAD_NO_TCPAUTOCONNECTTIMEOUTOFSOCKETTCP )
  {
    SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

    /* #20 Remove TCP auto connect timeout from timeout list. */
    SoAd_TimeoutList_RemoveElement(SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT, SoConIdx, instanceIdx, PartitionIdx);
  }
} /* SoAd_SoCon_ResetTcpAutoConnectTimeout() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_Close()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_Close(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SoConModeType      newSoConMode; /* Not initialized: Improved readability. */
  boolean                 abortFlag = (boolean)SoAd_IsAbortOfSoConDyn(SoConIdx, PartitionIdx);
  SoAd_SoConCloseModeType closeMode = SoAd_GetCloseModeOfSoConDyn(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that no transmission or reception is active for the socket connection. */
  if ( !SoAd_IsTxActiveOfSoConDyn(SoConIdx, PartitionIdx) && !SoAd_IsRxActiveOfSoConDyn(SoConIdx, PartitionIdx) )
  {
    /* #20 Get expected socket connection state after closing. */
    switch ( closeMode )
    {
      case SOAD_CLOSE_OFFLINE_RESET:
      case SOAD_CLOSE_OFFLINE:
      case SOAD_CLOSE_SOCKET:
      {
        newSoConMode = SOAD_SOCON_OFFLINE;
        break;
      }
      default: /* SOAD_CLOSE_RECONNECT || SOAD_CLOSE_SOCKET_RECONNECT */
      {
        newSoConMode = SOAD_SOCON_RECONNECT;
        break;
      }
    }

    /* #30 Check if expected socket connection state after closing is different to the current state. */
    if ( SoAd_GetModeOfSoConDyn(SoConIdx, PartitionIdx) != newSoConMode )
    {
      boolean immediateCbk = TRUE;
      uint8   errorId = SOAD_E_NO_ERROR;

      /* #40 Enter critical section to prevent that socket connection mode is set in other context. */
      SOAD_BEGIN_CRITICAL_SECTION();

      /* #50 Set new socket connection mode. */
      SoAd_SetModeOfSoConDyn(SoConIdx, newSoConMode, PartitionIdx);                                                    /* SBSW_SOAD_PARAMETER_IDX */

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
      /* #60 Add mode change to the mode change queue if enabled. */
      immediateCbk = SoAd_SoCon_AddSoConModeChgQueueElement(SoConIdx, PartitionIdx, newSoConMode, &errorId);           /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

      /* #70 Leave critical section. */
      SOAD_END_CRITICAL_SECTION();

      /* #80 Close socket connection. */
      SoAd_SoCon_CloseSoCon(SoConIdx, PartitionIdx, closeMode);

      /* #90 Notify user about socket connection mode change. */
      SoAd_SoCon_CheckAndNotifySoConModeChg(SoConIdx, PartitionIdx, newSoConMode, immediateCbk, errorId);
    }

    /* #100 Close related sockets. */
    SoAd_SoCon_CloseSocket(SoConIdx, PartitionIdx, abortFlag);

    /* #110 Reset socket connection close mode. */
    SoAd_SoCon_ResetCloseMode(SoConIdx, PartitionIdx);
  }

  /* #120 Set event to handle socket connection state in main function. */
  SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, SoConIdx,
    SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);
} /* SoAd_SoCon_Close() */

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseSoCon()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CloseSoCon(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConCloseModeType CloseMode)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle socket connection close dependent on close mode. */
  switch ( CloseMode )
  {
    /* #100 Check if socket connection shall be closed by normal close request. */
    case SOAD_CLOSE_OFFLINE:
    {
      /* #1000 Reset structs, remote address and terminate active TP sessions. */
      SoAd_SoCon_ResetSoCon(SoConIdx, PartitionIdx, TRUE, FALSE, TRUE);
      break;
    }
    /* #101 Check if socket connection shall be closed by special request (shutdown, API call failed, config issue). */
    case SOAD_CLOSE_OFFLINE_RESET:
    {
      /* #1010 Reset structs, remote address, open/close sequence and terminate active TP sessions negatively. */
      SoAd_SoCon_ResetSoCon(SoConIdx, PartitionIdx, TRUE, TRUE, FALSE);
      break;
    }
#if ( SOAD_TCP == STD_ON )
    /* #102 Check if socket connection shall be closed by socket close and TcpInitiate is true (if TCP is enabled). */
    case SOAD_CLOSE_SOCKET_RECONNECT:
    {
      /* #1020 Reset structs and terminate active TP sessions negatively. */
      SoAd_SoCon_ResetSoCon(SoConIdx, PartitionIdx, FALSE, FALSE, FALSE);
      break;
    }
#endif /* SOAD_TCP == STD_ON */
    /* #103 Close socket connection if socket is closed or requested to be reconnected otherwise. */
    default: /* SOAD_CLOSE_SOCKET || SOAD_CLOSE_RECONNECT */
    {
      /* #1030 Reset structs, remote address and terminate active TP sessions negatively. */
      SoAd_SoCon_ResetSoCon(SoConIdx, PartitionIdx, TRUE, FALSE, FALSE);
      break;
    }
  }
} /* SoAd_SoCon_CloseSoCon() */

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseSocket()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CloseSocket(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean Abort)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  /* #10 Check if socket is of type TCP. */
  if ( SoAd_IsSocketTcpUsedOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
  {
    /* #100 Close socket. */
    SoAd_SoCon_CloseSocketTcp(SoConIdx, PartitionIdx, Abort);

    /* #101 Handle listen socket close. */
    SoAd_SoCon_CloseSocketTcpListen(SoConIdx, PartitionIdx, Abort);
  }
  else
#endif /* SOAD_TCP == STD_ON */
  /* #20 Handle close if socket is of type UDP. */
  {
    SoAd_SoCon_CloseSocketUdp(SoConIdx, PartitionIdx, Abort);
  }

  /* #30 Reset number of data to be skipped on reception. */
  SoAd_SetSkipBytesOfSocketDyn(SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx), 0u, PartitionIdx);                    /* SBSW_SOAD_PARAMETER_IDX */

} /* SoAd_SoCon_CloseSocket() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_CloseSocketTcp()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CloseSocketTcp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketType sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SocketIdType     socketId = SoAd_GetSocketIdOfSocketDyn(sockIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that socket state is overwritten by any function call. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Ignore close if socket is already closed. */
  if ( SoAd_GetStateOfSocketDyn(sockIdx, PartitionIdx) == SOAD_SOCK_STATE_CLOSED )
  {
    /* #200 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();
  }
  /* #21 Wait for socket close if socket is closing. */
  else if ( SoAd_GetStateOfSocketDyn(sockIdx, PartitionIdx) == SOAD_SOCK_STATE_CLOSING )
  {
    /* #210 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();
  }
  /* #22 Set socket to closing and call close service otherwise. */
  else
  {
    SoAd_SetStateOfSocketDyn(sockIdx, SOAD_SOCK_STATE_CLOSING, PartitionIdx);                                          /* SBSW_SOAD_CSL02_CSL05 */

    /* #220 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();

    /* #221 Close socket. */
    (void)SoAd_TcpIpApiClose(socketId, Abort);
  }
} /* SoAd_SoCon_CloseSocketTcp() */

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseSocketTcpListen()
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
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CloseSocketTcpListen(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket is a server socket. */
  if ( !SoAd_IsTcpInitiateOfSocketTcp(SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
  {
    /* #20 Get listen socket. */
    SoAd_SizeOfSocketType sockIdx = SoAd_GetSocketIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
    SoAd_SocketIdType     socketId = SoAd_GetSocketIdOfSocketDyn(sockIdx, PartitionIdx);

    /* #30 Enter critical section to prevent that socket state is overwritten by any function call. */
    SOAD_BEGIN_CRITICAL_SECTION();

    /* #40 Check if listen socket is in close or closing state. */
    if ( (SoAd_GetStateOfSocketDyn(sockIdx, PartitionIdx) != SOAD_SOCK_STATE_CLOSED) &&
      (SoAd_GetStateOfSocketDyn(sockIdx, PartitionIdx) != SOAD_SOCK_STATE_CLOSING) )
    {
      /* #50 Iterate over all socket connections of socket connection group. */
      SoAd_SoConIterType  soConIter;
      boolean             listenSocketRequired = FALSE;

      for ( soConIter = SoAd_GetSoConStartIdxOfSocket(sockIdx, PartitionIdx);
        soConIter < SoAd_GetSoConEndIdxOfSocket(sockIdx, PartitionIdx);
        soConIter++ )
      {
        /* #60 Check if socket connection still requires a listen socket. */
        if ( SoAd_GetModeOfSoConDyn(soConIter, PartitionIdx) != SOAD_SOCON_OFFLINE )
        {
          listenSocketRequired = TRUE;
          break;
        }
      }

      /* #70 Handle closing of the socket if no socket connection requires a listen socket. */
      if ( (listenSocketRequired == FALSE) )
      {
        SoAd_SetStateOfSocketDyn(sockIdx, SOAD_SOCK_STATE_CLOSING, PartitionIdx);                                      /* SBSW_SOAD_CSL02_CSL05 */
        
        /* #700 Leave critical section and call close service. */
        SOAD_END_CRITICAL_SECTION();

        /* #701 Close socket. */
        (void)SoAd_TcpIpApiClose(socketId, Abort);
      }
      /* #71 Otherwise leave critical section. */
      else
      {
        SOAD_END_CRITICAL_SECTION();
      }
    }
    /* #41 Otherwise leave critical section. */
    else
    {
      SOAD_END_CRITICAL_SECTION();
    }
  }
} /* SoAd_SoCon_CloseSocketTcpListen() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseSocketUdp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_CloseSocketUdp(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketType sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SoConIterType    soConIter; /* Not initialized: For-loop counter. */
  SoAd_SocketIdType     socketId = SoAd_GetSocketIdOfSocketDyn(sockIdx, PartitionIdx);
  boolean               socketRequired = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket is not yet closed. */
  if ( SoAd_GetStateOfSocketDyn(sockIdx, PartitionIdx) != SOAD_SOCK_STATE_CLOSED )
  {
    /* #20 Iterate over all socket connections of socket connection group. */
    for ( soConIter = SoAd_GetSoConStartIdxOfSocket(sockIdx, PartitionIdx);
      soConIter < SoAd_GetSoConEndIdxOfSocket(sockIdx, PartitionIdx);
      soConIter++ )
    {
      /* #30 Check if socket connection still requires the shared socket. */
      if ( SoAd_GetModeOfSoConDyn(soConIter, PartitionIdx) != SOAD_SOCON_OFFLINE )
      {
        socketRequired = TRUE;
        break;
      }
    }

    /* #40 Close socket if shared socket is not required anymore and socket identifier is valid. */
    if ( (socketRequired == FALSE) && (socketId != SOAD_INV_SOCKET_ID) )
    {
      (void)SoAd_TcpIpApiClose(socketId, Abort);
    }
  }
} /* SoAd_SoCon_CloseSocketUdp() */

/**********************************************************************************************************************
 *  SoAd_SoCon_ResetCloseMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_ResetCloseMode(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  SoAd_SizeOfSocketType   sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
#endif /* SOAD_TCP == STD_ON */
  SoAd_SoConModeType      newSoConMode; /* Not initialized: Improved readability. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that close mode is overwritten by any function call. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Get expected socket connection mode after closing. */
  switch ( SoAd_GetCloseModeOfSoConDyn(SoConIdx, PartitionIdx) )
  {
    case SOAD_CLOSE_OFFLINE_RESET:
    case SOAD_CLOSE_OFFLINE:
    case SOAD_CLOSE_SOCKET:
    {
      newSoConMode = SOAD_SOCON_OFFLINE;
      break;
    }
    default: /* SOAD_CLOSE_RECONNECT || SOAD_CLOSE_SOCKET_RECONNECT */
    {
      newSoConMode = SOAD_SOCON_RECONNECT;
      break;
    }
  }

  /* #30 Reset close mode if socket connection is in expected mode and socket is closed in case of TCP. */
  if ( (SoAd_GetModeOfSoConDyn(SoConIdx, PartitionIdx) == newSoConMode)
#if ( SOAD_TCP == STD_ON )
    && (SoAd_IsSocketUdpUsedOfSoConGrp(soConGrpIdx, PartitionIdx)
      || (SoAd_GetStateOfSocketDyn(sockIdx, PartitionIdx) == SOAD_SOCK_STATE_CLOSED))
#endif /* SOAD_TCP == STD_ON */
    )
  {
    SoAd_SetCloseModeOfSoConDyn(SoConIdx, SOAD_CLOSE_NONE, PartitionIdx);                                              /* SBSW_SOAD_PARAMETER_IDX */
  }

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();
} /* SoAd_SoCon_ResetCloseMode() */

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_AddSoConModeChgQueueElement()
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
SOAD_LOCAL FUNC(boolean, SOAD_CODE) SoAd_SoCon_AddSoConModeChgQueueElement(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConModeType Mode,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfModeChgQueueType        soConModeChgQueueSize =
    SoAd_GetModeChgQueueEndIdxOfSoCon(SoConIdx, PartitionIdx) -
    SoAd_GetModeChgQueueStartIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_ModeChgQueueLvlOfSoConDynType soConModeChgQueueLvl = SoAd_GetModeChgQueueLvlOfSoConDyn(SoConIdx, PartitionIdx);
  SoAd_ModeChgQueueIdxOfSoConDynType firstModeChgQueueIdx = SoAd_GetModeChgQueueIdxOfSoConDyn(SoConIdx, PartitionIdx);
  boolean                            immediateCbk = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the mode change queue has any free elements left. */
  if ( soConModeChgQueueSize > soConModeChgQueueLvl )
  {
    /* #100 Calculate next queue index considering wrap around. */
    SoAd_SizeOfModeChgQueueType nextQueueIdx = (SoAd_SizeOfModeChgQueueType)SoAd_Util_GetNextArrayElement(
      firstModeChgQueueIdx, soConModeChgQueueLvl, SoAd_GetModeChgQueueStartIdxOfSoCon(SoConIdx, PartitionIdx),
      SoAd_GetModeChgQueueEndIdxOfSoCon(SoConIdx, PartitionIdx));

    /* #101 Write mode to queue and increase the level of elements. */
    SoAd_SetModeOfModeChgQueue(nextQueueIdx, Mode, PartitionIdx);                                                      /* SBSW_SOAD_CALCULATED_CSL03_CSL05_ModeChgQueue */
    SoAd_IncModeChgQueueLvlOfSoConDyn(SoConIdx, PartitionIdx);                                                         /* SBSW_SOAD_PARAMETER_IDX */
  }
  /* #11 Anyway handle the mode change if no queue elements are left otherwise. */
  else
  {
    /* #110 Calculate last queue index considering wrap around. */
    /* PRQA S 4391 2 */ /* MD_SoAd_GenericWiderTypeCast */
    SoAd_SizeOfModeChgQueueType lastQueueIdx = (SoAd_SizeOfModeChgQueueType)SoAd_Util_GetNextArrayElement(
      firstModeChgQueueIdx, (SoAd_SizeOfModeChgQueueType)(soConModeChgQueueLvl - 1u),
      SoAd_GetModeChgQueueStartIdxOfSoCon(SoConIdx, PartitionIdx),
      SoAd_GetModeChgQueueEndIdxOfSoCon(SoConIdx, PartitionIdx));

    /* #111 Overwrite last element in queue with the new mode so that the last reported mode is at least correct. */
    SoAd_SetModeOfModeChgQueue(lastQueueIdx, Mode, PartitionIdx);                                                      /* SBSW_SOAD_CALCULATED_CSL03_CSL05_ModeChgQueue */

    /* #112 Set the error ID to report a runtime error. */
    *ErrorIdPtr = SOAD_E_NO_MODE_CHG_QUEUE_ELEM;                                                                       /* SBSW_SOAD_POINTER_WRITE */
  }

  /* #20 Check if mode change queue elements can be directly handled and set process flag accordingly. */
  if ( !SoAd_IsModeChgQueueProcessFlagOfSoConDyn(SoConIdx, PartitionIdx) )
  {
    SoAd_SetModeChgQueueProcessFlagOfSoConDyn(SoConIdx, TRUE, PartitionIdx);                                           /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03_CSL05 */
    immediateCbk = TRUE;
  }

  return immediateCbk;
} /* SoAd_SoCon_AddSoConModeChgQueueElement() */
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_CheckAndNotifySoConModeChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_SoCon_CheckAndNotifySoConModeChg(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConModeType Mode,
  boolean ImmediateCbk,
  uint8 ErrorId)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
  /* #10 Report runtime error if any error occurred. */
# if ( SOAD_RUNTIME_ERROR_REPORT == STD_ON )
  if ( ErrorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportRuntimeError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_SO_CON_MODE_CHG, ErrorId);
  }
# endif /* SOAD_RUNTIME_ERROR_REPORT == STD_ON */

  /* #20 Add event to handle mode change callback in main function context if it shall not be immediately handled. */
  if ( ImmediateCbk == FALSE )
  {
    SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_MODE_CHG, SoConIdx, SoAd_GetInstanceIdxOfSoConGrp(
        SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx), PartitionIdx);
  }
  else
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */
  /* #21 Notify mode change otherwise. */
  {
    SoAd_SoCon_NotifySoConModeChg(SoConIdx, PartitionIdx, Mode);
  }

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_OFF )
  SOAD_DUMMY_STATEMENT(ImmediateCbk); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_OFF */

#if ( (SOAD_MODE_CHG_QUEUE_ENABLED == STD_OFF) || (SOAD_RUNTIME_ERROR_REPORT == STD_OFF) )
  SOAD_DUMMY_STATEMENT(ErrorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (SOAD_MODE_CHG_QUEUE_ENABLED == STD_OFF) || (SOAD_RUNTIME_ERROR_REPORT == STD_OFF) */
} /* SoAd_SoCon_CheckAndNotifySoConModeChg() */

/**********************************************************************************************************************
 *  SoAd_SoCon_NotifySoConModeChg()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_SoCon_NotifySoConModeChg(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConModeType Mode)
{
#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_ModeChgQueueLvlOfSoConDynType modeChgQueueLvlIter; /* Not initialized: For-loop counter. */
  SoAd_SizeOfModeChgQueueType        currModeChgQueueIdx = SoAd_GetModeChgQueueIdxOfSoConDyn(SoConIdx, PartitionIdx);
  SoAd_ModeChgQueueLvlOfSoConDynType soConModeChgQueueLvl = SoAd_GetModeChgQueueLvlOfSoConDyn(SoConIdx, PartitionIdx);
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
  /* #10 Iterate over mode change queue elements (if applicable). */
  for ( modeChgQueueLvlIter = 0u; modeChgQueueLvlIter < soConModeChgQueueLvl; modeChgQueueLvlIter++ )
  {
    SoAd_SizeOfModeChgQueueType     nextFirstModeChgQueueIdx; /* Not initialized: Variable is initialized by another
                                                               * function. */
    SoAd_SoConModeType              currentMode; /* Not initialized: Improve readability. */
    SoAd_SoConModeChgCbkIndIterType soConModeChgCbkIndIter; /* Not initialized: For-loop counter. */

    /* #20 Calculate next first index considering wrap around. */
    nextFirstModeChgQueueIdx = (SoAd_SizeOfModeChgQueueType)SoAd_Util_GetNextArrayElement(
      currModeChgQueueIdx, 1u, SoAd_GetModeChgQueueStartIdxOfSoCon(SoConIdx, PartitionIdx),
      SoAd_GetModeChgQueueEndIdxOfSoCon(SoConIdx, PartitionIdx));

    /* #30 Store the current mode and set the mode change queue index for the next iteration. */
    currentMode = SoAd_GetModeOfModeChgQueue(currModeChgQueueIdx, PartitionIdx);
    currModeChgQueueIdx = nextFirstModeChgQueueIdx;

    /* #40 Update first queue index and and decrease the level of elements protected inside a critical section. */
    SOAD_BEGIN_CRITICAL_SECTION();
    SoAd_SetModeChgQueueIdxOfSoConDyn(SoConIdx, nextFirstModeChgQueueIdx, PartitionIdx);                               /* SBSW_SOAD_PARAMETER_IDX */
    SoAd_DecModeChgQueueLvlOfSoConDyn(SoConIdx, PartitionIdx);                                                         /* SBSW_SOAD_PARAMETER_IDX */
    SOAD_END_CRITICAL_SECTION();
#else
  {
    SoAd_SoConModeChgCbkIndIterType soConModeChgCbkIndIter; /* Not initialized: For-loop counter. */
    SoAd_SoConModeType              currentMode = Mode;
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

    /* #50 Call all socket connection related socket connection mode change notifications. */
    for ( soConModeChgCbkIndIter = SoAd_GetSoConModeChgCbkIndStartIdxOfSoCon(SoConIdx, PartitionIdx);
      soConModeChgCbkIndIter < SoAd_GetSoConModeChgCbkIndEndIdxOfSoCon(SoConIdx, PartitionIdx);
      soConModeChgCbkIndIter++ )
    {
      if ( SoAd_GetSoConModeChgCbk(SoAd_GetSoConModeChgCbkInd(soConModeChgCbkIndIter, PartitionIdx)) != NULL_PTR )
      {
        SoAd_GetSoConModeChgCbk(SoAd_GetSoConModeChgCbkInd(soConModeChgCbkIndIter, PartitionIdx))(                     /* SBSW_SOAD_FUNCTION_PTR */
          SoAd_GetSoConIdOfSoCon(SoConIdx, PartitionIdx), currentMode);
      }
    }
  }

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
  /* #60 Reset mode change queue process flag. */
  SoAd_SetModeChgQueueProcessFlagOfSoConDyn(SoConIdx, FALSE, PartitionIdx);                                            /* SBSW_SOAD_PARAMETER_IDX */

  SOAD_DUMMY_STATEMENT(Mode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */
} /* SoAd_SoCon_NotifySoConModeChg() */

/**********************************************************************************************************************
 *  SoAd_SoCon_ResetLocalPortDyn()
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
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_SoCon_ResetLocalPortDyn(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType    soConStartIdx = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConStartIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  /* #10 Reset local port on listen socket in case of TCP server socket (if enabled). */
  if ( SoAd_IsSocketTcpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) &&
    !SoAd_IsTcpInitiateOfSocketTcp(SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
  {
    /* Optional indirection: Refer to Safe BSW assumption SoConGrp -> Socket. */
    SoAd_SizeOfSocketType grpSockIdx = SoAd_GetSocketIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
    SoAd_SoConIterType    soConIter;
    SoAd_ReturnType       retVal = E_OK;

    /* #100 Check if listen socket is closed. */
    if ( SoAd_GetStateOfSocketDyn(grpSockIdx, PartitionIdx) == SOAD_SOCK_STATE_CLOSED )
    {
      /* #1000 Check if all data sockets are closed and indicate that local port cannot be reset if not. */
      for ( soConIter = SoAd_GetSoConStartIdxOfSocket(grpSockIdx, PartitionIdx);
        soConIter < SoAd_GetSoConEndIdxOfSocket(grpSockIdx, PartitionIdx);
        soConIter++ )
      {
        if ( SoAd_GetStateOfSocketDyn(SoAd_GetSocketIdxOfSoCon(soConIter, PartitionIdx), PartitionIdx) !=
          SOAD_SOCK_STATE_CLOSED )
        {
          retVal = E_NOT_OK;
          break;
        }
      }
    }
    /* #101 Indicate that local port cannot be reset otherwise. */
    else
    {
      retVal = E_NOT_OK;
    }

    /* #102 Reset local port on listen socket to configured value if all sockets are closed. */
    if ( retVal == E_OK )
    {
      SoAd_SetLocalPortOfSocketDyn(grpSockIdx, SoAd_GetLocalPortOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);  /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
    }
  }
  else
#endif /* SOAD_TCP == STD_ON */
  /* #20 Reset local port on data socket to configured value otherwise. */
  {
    SoAd_SetLocalPortOfSocketDyn(SockIdx, SoAd_GetLocalPortOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);       /* SBSW_SOAD_PARAMETER_IDX */
  }
} /* SoAd_SoCon_ResetLocalPortDyn() */

/**********************************************************************************************************************
 *  SoAd_SoCon_ResetSoCon()
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
/* PRQA S 3206 5 */ /* MD_SoAd_UnusedParameter */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_ResetSoCon(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean ResetRemAddr,
  boolean ResetOpClSeq,
  boolean NormalClose)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset remote address if required. */
#if ( SOAD_TCP == STD_ON )
  if ( ResetRemAddr == TRUE )
#else
  SOAD_DUMMY_STATEMENT(ResetRemAddr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_TCP == STD_ON */
  {
    SoAd_RemoteAddr_ResetRemAndRcvRemAddr(SoConIdx, PartitionIdx);
  }

  /* #20 Reset open/close sequence if required. */
  if ( ResetOpClSeq == TRUE )
  {
    SoAd_SetOpenCloseCounterOfSoConDyn(SoConIdx, 0u, PartitionIdx);                                                    /* SBSW_SOAD_PARAMETER_IDX */
  }

  /* #30 Reset abort flag. */
  SoAd_SetAbortOfSoConDyn(SoConIdx, FALSE, PartitionIdx);                                                              /* SBSW_SOAD_PARAMETER_IDX */

  /* #40 Terminate active transmissions. */
  if ( SoAd_IsTxMgtUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    SoAd_TxSocketManager_Terminate(SoConIdx, PartitionIdx);
  }

  /* #50 Terminate active receptions. */
  if ( SoAd_IsRxMgtUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    SoAd_Rx_TerminateReceive(SoConIdx, PartitionIdx, NormalClose);
  }

  /* #60 Reset alive supervision timeout. */
#if ( SOAD_TCP == STD_ON )
  if ( SoAd_IsSocketUdpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) )
#endif /* SOAD_TCP == STD_ON */
  {
    if ( SoAd_GetAliveTimeoutMaxCntOfSocketUdp(SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx)
        > 0u )
    {
      SoAd_TimeoutList_RemoveElement(SOAD_TIMEOUT_LIST_UDP_ALIVE, SoConIdx, instanceIdx, PartitionIdx);
    }
  }
} /* SoAd_SoCon_ResetSoCon() */

/**********************************************************************************************************************
 *  SoAd_SoCon_TriggerAddressResolution()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
  */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_TriggerAddressResolution(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if any Tx PDU is configured for the socket connection. */
  if ( SoAd_GetPduRouteDestBySoConIndStartIdxOfSoCon(SoConIdx, PartitionIdx) !=
    SoAd_GetPduRouteDestBySoConIndEndIdxOfSoCon(SoConIdx, PartitionIdx))
  {
    uint8 ctrlIdx = 0u;
    uint8 physAddr[6u] = { 0u, 0u, 0u, 0u, 0u, 0u };

    /* #20 Get the corresponding Ethernet interface controller index. */
    if ( SoAd_TcpIpApiGetCtrlIdx(SoAd_LocalAddr_GetTcpIpLocalAddrId(
          SoAd_GetLocalAddrIdxOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx),
          PartitionIdx), &ctrlIdx) == E_OK )                                                                           /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      /* PRQA S 310 */ /* MD_SoAd_PointerCastOnStruct */
      SoAd_SockAddrInet6Type remAddr = { 0u, 0u, { 0u, 0u, 0u, 0u } };

      /* #30 Get remote address. */
      /* PRQA S 310 1 */ /* MD_SoAd_PointerCastOnStruct */
      SoAd_RemoteAddr_Get(SoConIdx, PartitionIdx, SOAD_A_P2VAR(SoAd_SockAddrType)&remAddr);                            /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

      /* #40 Trigger address resolution by getting remote physical address for the Ethernet interface controller. */
      /* PRQA S 310 2 */ /* MD_SoAd_PointerCastOnStruct */
      (void)SoAd_TcpIpApiGetRemotePhysAddr(ctrlIdx,                                                                    /* SBSW_SOAD_POINTER_AND_CONST_POINTER_FORWARD_CSL02 */
        SOAD_A_P2VAR(SoAd_SockAddrType)&remAddr, &physAddr[0], TRUE);
    }
  }
} /* SoAd_SoCon_TriggerAddressResolution() */

/**********************************************************************************************************************
 *  SoAd_SoCon_SetUdpAliveTimeout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_SetUdpAliveTimeout(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType  soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType  instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if alive supervision timeout is configured. */
  if ( SoAd_GetAliveTimeoutMaxCntOfSocketUdp(socketUdpIdx, PartitionIdx) != 0u )
  {
    /* #20 Set alive supervision timeout. */
    (void)SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_UDP_ALIVE, SoConIdx,
      SoAd_GetAliveTimeoutMaxCntOfSocketUdp(socketUdpIdx, PartitionIdx), instanceIdx, PartitionIdx);
  }
} /* SoAd_SoCon_SetUdpAliveTimeout() */

/**********************************************************************************************************************
 *  SoAd_SoCon_DecrementOpenSockets()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_SoCon_DecrementOpenSockets(
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if at least one socket is opened. */
  if ( SoAd_Util_LoadU32(SoAd_GetAddrOpenSocksOfInstanceDyn(InstanceIdx, PartitionIdx), TRUE) > 0u )                   /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* #20 Decrement number of open sockets. */
    SoAd_Util_FetchSubU32(SoAd_GetAddrOpenSocksOfInstanceDyn(InstanceIdx, PartitionIdx), SOAD_INC_DEC_BY_ONE);         /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  }
} /* SoAd_SoCon_DecrementOpenSockets() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_SoCon_InitSoCon()
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
FUNC(void, SOAD_CODE) SoAd_SoCon_InitSoCon(
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIterType soConIter; /* Not initialized: For-loop counter. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all socket connection indexes. */
  for ( soConIter = 0u; soConIter < SoAd_GetSizeOfSoCon(PartitionIdx); soConIter++ )
  {
    /* #20 Initialize general struct elements. */
    SoAd_SetModeOfSoConDyn(soConIter, SOAD_SOCON_OFFLINE, PartitionIdx);                                               /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_SetTxActiveOfSoConDyn(soConIter, FALSE, PartitionIdx);                                                        /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_SetRxActiveOfSoConDyn(soConIter, FALSE, PartitionIdx);                                                        /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_SetAbortOfSoConDyn(soConIter, FALSE, PartitionIdx);                                                           /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_SetCloseModeOfSoConDyn(soConIter, SOAD_CLOSE_NONE, PartitionIdx);                                             /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_SetOpenCloseCounterOfSoConDyn(soConIter, 0u, PartitionIdx);                                                   /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_SetTimeoutOccurredOfSoConDyn(soConIter, FALSE, PartitionIdx);                                                 /* SBSW_SOAD_CSL02_CSL05 */
#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
    SoAd_SetModeChgQueueIdxOfSoConDyn(soConIter, SoAd_GetModeChgQueueStartIdxOfSoCon(soConIter, PartitionIdx),         /* SBSW_SOAD_CSL02_CSL05 */
      PartitionIdx);
    SoAd_SetModeChgQueueLvlOfSoConDyn(soConIter, 0u, PartitionIdx);                                                    /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_SetModeChgQueueProcessFlagOfSoConDyn(soConIter, FALSE, PartitionIdx);                                         /* SBSW_SOAD_CSL02_CSL05 */
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

    /* #30 Initialize remote address struct elements. */
    SoAd_RemoteAddr_Init((SoAd_SizeOfSoConType)soConIter, PartitionIdx);

    /* #40 Initialize tx struct elements. */
    SoAd_Tx_InitSoCon((SoAd_SizeOfSoConType)soConIter, PartitionIdx);

    /* #50 Initialize rx struct elements. */
    SoAd_Rx_InitSoCon((SoAd_SizeOfSoConType)soConIter, PartitionIdx);
  }
} /* SoAd_SoCon_InitSoCon() */

/**********************************************************************************************************************
 *  SoAd_SoCon_InitSocket()
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
FUNC(void, SOAD_CODE) SoAd_SoCon_InitSocket(
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SocketIterType      sockIter; /* Not initialized: For-loop counter. */
  SoAd_InstanceDynIterType instDynIter; /* Not initialized: For-loop counter. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all module internal socket indexes. */
  for ( sockIter = 0u; sockIter < SoAd_GetSizeOfSocket(PartitionIdx); sockIter++ )
  {
    /* #20 Initialize general struct elements. */
    SoAd_SetStateOfSocketDyn(sockIter, SOAD_SOCK_STATE_CLOSED, PartitionIdx);                                          /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_SetSocketIdOfSocketDyn(sockIter, SOAD_INV_SOCKET_ID, PartitionIdx);                                           /* SBSW_SOAD_CSL02_CSL05 */
    SoAd_SoCon_ResetLocalPortDyn((SoAd_SizeOfSocketType)sockIter, PartitionIdx);

    /* #30 Initialize tx struct elements. */
    SoAd_TxSocketManager_Init(sockIter, PartitionIdx);

    /* #40 Initialize rx struct elements. */
    SoAd_SetSkipBytesOfSocketDyn(sockIter, 0u, PartitionIdx);                                                          /* SBSW_SOAD_CSL02_CSL05 */
  }

  /* #50 Initialize the number of open sockets. */
  for ( instDynIter = 0; instDynIter < SoAd_GetSizeOfInstanceDyn(PartitionIdx); instDynIter++ )
  {
    SoAd_Util_StoreU32(SoAd_GetAddrOpenSocksOfInstanceDyn(instDynIter, PartitionIdx), 0u, FALSE);                      /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  }
} /* SoAd_SoCon_InitSocket() */

/**********************************************************************************************************************
 *  SoAd_SoCon_GetSockIdxBySocketId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_GetSockIdxBySocketId(
  SoAd_SocketIdType SocketId,
  P2VAR(SoAd_SizeOfSocketType, AUTOMATIC, SOAD_APPL_DATA) SockIdxPtr,
  P2VAR(SoAd_PartitionConfigIdxType, AUTOMATIC, SOAD_APPL_DATA) PartitionIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SocketIterType               sockIter; /* Not initialized: For-loop counter. */
  SoAd_PartitionIdentifiersIterType partitionIdentifierIter; /* Not initialized: For-loop counter. */
  Std_ReturnType                    retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  for ( partitionIdentifierIter = 0u; partitionIdentifierIter < SoAd_GetSizeOfPartitionIdentifiers();
    partitionIdentifierIter++ )
  {
    SoAd_PartitionConfigIdxType partitionIdx = SoAd_GetPCPartitionConfigIdxOfPartitionIdentifiers(
        partitionIdentifierIter);
#if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
    ApplicationType             osApplicationId = (ApplicationType)GetApplicationID();

    if ( SoAd_GetOsApplicationIdOfPartition(0u, partitionIdx) == osApplicationId )
#endif /* SOAD_MULTI_PARTITION == STD_ON */
    {
      /* #10 Iterate over all socket indexes. */
      for ( sockIter = 0u; sockIter < SoAd_GetSizeOfSocket(partitionIdx); sockIter++ )
      {
        /* #20 Check if specified socket identifier is related to the socket index. */
        if ( SocketId == SoAd_GetSocketIdOfSocketDyn(sockIter, partitionIdx) )
        {
          /* #30 Return socket index and stop iteration. */
          *SockIdxPtr = (SoAd_SizeOfSocketType)sockIter;                                                               /* SBSW_SOAD_POINTER_WRITE */
          *PartitionIdxPtr = partitionIdx;                                                                             /* SBSW_SOAD_POINTER_WRITE */
          retVal = E_OK;
          break;
        }
      }
      if ( retVal == E_OK )
      {
        break;
      }
    }
  }

  return retVal;
} /* SoAd_SoCon_GetSockIdxBySocketId() */

/**********************************************************************************************************************
 *  SoAd_SoCon_GetSoConIdxBySoConId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_GetSoConIdxBySoConId(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SizeOfSoConType, AUTOMATIC, SOAD_APPL_DATA) SoConIdxPtr,
  P2VAR(SoAd_PartitionConfigIdxType, AUTOMATIC, SOAD_APPL_DATA) PartitionIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the socket connection identifier is inside the valid range. */
  if ( (SoConId < SoAd_GetSizeOfSoConMap())
# if ( SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE )
    && !SoAd_IsInvalidHndOfSoConMap(SoConId)
# endif /* SOAD_CONFIGURATION_VARIANT == SOAD_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE */
    )
  {
    /* #20 Get the partition specific socket connection index and the corresponding partition index. */
    *SoConIdxPtr = SoAd_GetPartitionSpecificSoConIdxOfSoConMap(SoConId);                                               /* SBSW_SOAD_POINTER_WRITE */
    *PartitionIdxPtr = SoAd_GetPartitionSpecificSoConPartitionIdxOfSoConMap(SoConId);                                  /* SBSW_SOAD_POINTER_WRITE */
    retVal = E_OK;
  }

  return retVal;
} /* SoAd_SoCon_GetSoConIdxBySoConId() */

/**********************************************************************************************************************
 *  SoAd_SoCon_GetLocalAddrIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(SoAd_SizeOfLocalAddrType, SOAD_CODE) SoAd_SoCon_GetLocalAddrIdx(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return local address index. */
  const SoAd_SizeOfSoConGrpType  soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  const SoAd_SizeOfLocalAddrType localAddrIdx = SoAd_GetLocalAddrIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  return localAddrIdx;
} /* SoAd_SoCon_GetLocalAddrIdx() */

/**********************************************************************************************************************
 *  SoAd_SoCon_GetSocketIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(SoAd_SizeOfSocketType, SOAD_CODE) SoAd_SoCon_GetSocketIdx(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return socket index. */
  const SoAd_SizeOfSocketType socketIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);

  return socketIdx;
} /* SoAd_SoCon_GetSocketIdx() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_TcpAccepted()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpAccepted(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketIdConnected,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType  soConIdx = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);
  SoAd_SizeOfSoConType  soConIdxAccepted = SoAd_GetSizeOfSoCon(PartitionIdx);
  Std_ReturnType        retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check listen socket state. */
  if ( SoAd_GetStateOfSocketDyn(SockIdx, PartitionIdx) == SOAD_SOCK_STATE_LISTEN )
  {
    /* #20 Check if a suitable socket connection can be found. */
    if ( SoAd_SoCon_TcpAcceptedGetSoConIdx(soConIdx, PartitionIdx, RemoteAddrPtr, &soConIdxAccepted) == E_OK )         /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */
    {
      /* #30 Check if socket connection accepts connection. */
      retVal = SoAd_SoCon_TcpAcceptedCheckAcceptance(soConIdxAccepted, PartitionIdx, SocketIdConnected, RemoteAddrPtr);/* SBSW_SOAD_POINTER_FORWARD */
    }
    /* #21 Increment dropped TCP connection counter and report the corresponding security event (as configured)
     *     otherwise. */
    else
    {
# if ( (SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON) || (SOAD_SECURITY_EVENT_REPORTING == STD_ON) )
      SoAd_Anomaly_Report(SOAD_MEAS_DROP_TCP_CONNECTION, PartitionIdx);
# endif /* (SOAD_GET_RESET_MEASUREMENT_DATA_API == STD_ON) || (SOAD_SECURITY_EVENT_REPORTING == STD_ON) */
    }
  }

  return retVal;
} /* SoAd_SoCon_TcpAccepted() */

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpAcceptedGetSoConIdx()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpAcceptedGetSoConIdx(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  P2VAR(SoAd_SizeOfSoConType, AUTOMATIC, SOAD_APPL_DATA) SoConIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if message acceptance filter is enabled. */
  if ( SoAd_IsMsgAcceptFilterEnabledOfSoConGrp(soConGrpIdx, PartitionIdx) )
  {
    SoAd_SizeOfSoConType                assignedSoConIdx; /* Not initialized: Variable is initialized by another
                                                           * function. */
    SoAd_BestMatch_SoConStartEndIdxType soConStartEndIdxStruct; /* Not initialized. Struct is initialized member
                                                                 * wise. */

    soConStartEndIdxStruct.InstanceIdx = instanceIdx;
    soConStartEndIdxStruct.PartitionIdx = PartitionIdx;
    soConStartEndIdxStruct.SoConStartIdx = SoAd_GetSoConStartIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
    soConStartEndIdxStruct.SoConEndIdx = SoAd_GetSoConEndIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

    /* #100 Enter critical section to prevent that socket connections of group change states or remote addresses. */
    SOAD_BEGIN_CRITICAL_SECTION();

    /* #101 Derive socket connection list using best match algorithm. */
    SoAd_BestMatch_DeriveList(soConStartEndIdxStruct, TRUE, RemoteAddrPtr);                                            /* SBSW_SOAD_CONST_POINTER_FORWARD */

    /* #102 Check if best match algorithm found a suitable socket connection. */
    if ( SoAd_BestMatch_GetNextSoCon(instanceIdx, PartitionIdx, 0u, &assignedSoConIdx) == E_OK )                       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      /* #103 Return first matching socket connection. */
      *SoConIdxPtr = assignedSoConIdx;                                                                                 /* SBSW_SOAD_POINTER_WRITE */

      retVal = E_OK;
    }

    /* #104 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();
  }
  /* #11 Check acceptance on single socket connection otherwise. */
  else
  {
    /* #110 Check socket connection state. */
    if ( SoAd_GetModeOfSoConDyn(SoConIdx, PartitionIdx) == SOAD_SOCON_RECONNECT )
    {
      *SoConIdxPtr = SoConIdx;                                                                                         /* SBSW_SOAD_POINTER_WRITE */

      retVal = E_OK;
    }
  }

  return retVal;
} /* SoAd_SoCon_TcpAcceptedGetSoConIdx() */

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpConnected()
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
FUNC(void, SOAD_CODE) SoAd_SoCon_TcpConnected(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType  soConIdx = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if socket is in connected state. */
  if ( SoAd_GetStateOfSocketDyn(SockIdx, PartitionIdx) == SOAD_SOCK_STATE_CONNECT )
  {
    /* #200 Check if socket connection is still requested to be opened and no timeout occurred. */
    if ( ((SoAd_GetOpenCloseCounterOfSoConDyn(soConIdx, PartitionIdx) > 0u) ||
      SoAd_IsSockAutoSoConSetupOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(soConIdx, PartitionIdx), PartitionIdx)) &&
      (!SoAd_IsTimeoutOccurredOfSoConDyn(soConIdx, PartitionIdx)) )
    {
      /* #2000 Check if socket connection mode is not online. */
      if ( SoAd_GetModeOfSoConDyn(soConIdx, PartitionIdx) != SOAD_SOCON_ONLINE )
      {
        boolean               immediateCbk = TRUE;
        SoAd_SoConModeType    newSoConMode = SOAD_SOCON_ONLINE;
        uint8                 errorId = SOAD_E_NO_ERROR;

        /* #20000 Change socket state and socket connection mode. */
        SoAd_SizeOfSocketType sockIdx = SoAd_GetSocketIdxOfSoCon(soConIdx, PartitionIdx);
        SoAd_SetStateOfSocketDyn(sockIdx, SOAD_SOCK_STATE_ESTABLISHED, PartitionIdx);                                  /* SBSW_SOAD_CSL02_CSL05 */
        SoAd_SetModeOfSoConDyn(soConIdx, newSoConMode, PartitionIdx);                                                  /* SBSW_SOAD_PARAMETER_IDX */

        /* #20001 Remove TCP auto connect timeout if configured. */
        SoAd_SoCon_ResetTcpAutoConnectTimeout(soConIdx, PartitionIdx);

# if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
        /* #20002 Add mode change to the mode change queue if enabled. */
        immediateCbk = SoAd_SoCon_AddSoConModeChgQueueElement(soConIdx, PartitionIdx, newSoConMode, &errorId);         /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
# endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

        /* #20003 Leave critical section. */
        SOAD_END_CRITICAL_SECTION();

        /* #20004 Notify user about mode change. */
        SoAd_SoCon_CheckAndNotifySoConModeChg(soConIdx, PartitionIdx, newSoConMode, immediateCbk, errorId);

        /* #20005 Try to start TP reception if required. */
        (void)SoAd_Rx_TpStartOfReception(soConIdx, PartitionIdx);
      }
      /* #2001 Otherwise leave critical section. */
      else
      {
        SOAD_END_CRITICAL_SECTION();
      }
    }
    /* #210 Otherwise leave critical section. */
    else
    {
      SOAD_END_CRITICAL_SECTION();
    }
  }
  /* #21 Otherwise leave critical section. */
  else
  {
    SOAD_END_CRITICAL_SECTION();
  }
} /* SoAd_SoCon_TcpConnected() */

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpChangeParameter()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameter(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType   soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSocketTcpType  socketTcpIdx = SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  uint32                    tcpTxBufMin = SoAd_GetTcpTxBufMinOfSocketTcp(socketTcpIdx, PartitionIdx);
  uint32                    tcpRxBufMin = SoAd_GetTcpRxBufMinOfSocketTcp(socketTcpIdx, PartitionIdx);
  Std_ReturnType            retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set frame priority. */
  if ( SoAd_GetFramePriorityOfSoConGrp(soConGrpIdx, PartitionIdx) != SOAD_NO_FRAMEPRIORITYOFSOCONGRP )
  {
    uint8 framePriority = SoAd_GetFramePriorityOfSoConGrp(soConGrpIdx, PartitionIdx);

    retVal = SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_FRAMEPRIO, &framePriority);                           /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  }

  /* #20 Set rx buffer size. */
  /* PRQA S 310 1 */ /* MD_SoAd_PointerCastOnParam */
  retVal |= SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_TCP_RXWND_MAX, (uint8*)&tcpRxBufMin);                  /* SBSW_SOAD_CAST_POINTER_FORWARD */

  /* #30 Set tx buffer size. */
  /* PRQA S 310 1 */ /* MD_SoAd_PointerCastOnParam */
  retVal |= SoAd_TcpIpApiChangeParameter(SocketId, SOAD_PARAMID_TCP_TXWND_MAX, (uint8*)&tcpTxBufMin);                  /* SBSW_SOAD_CAST_POINTER_FORWARD */

  /* #40 Set no delay (Nagle algorithm). */
  retVal |= SoAd_SoCon_TcpChangeParameterNoDelay(SoConIdx, PartitionIdx, SocketId);

  /* #50 Set keep alive. */
  retVal |= SoAd_SoCon_TcpChangeParameterKeepAlive(SoConIdx, PartitionIdx, SocketId);

  /* #60 Set TLS. */
  retVal |= SoAd_SoCon_TcpChangeParameterTls(SoConIdx, PartitionIdx, SocketId);

  /* #70 Set MSL. */
  retVal |= SoAd_SoCon_TcpChangeParameterMsl(SoConIdx, PartitionIdx, SocketId);

  return retVal;
} /* SoAd_SoCon_TcpChangeParameter() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_Event()
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
/* PRQA S 3206 3 */ /* MD_SoAd_UnusedParameter */
FUNC(void, SOAD_CODE) SoAd_SoCon_Event(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_EventType Event)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  /* #10 Handle socket dependent of Event type (if TCP is enabled). */
  switch ( Event )
  {
    /* #100 Handle event of UDP close. */
    case SOAD_UDP_CLOSED:
    {
      SoAd_SoCon_EventUdpClose(SockIdx, PartitionIdx);
      break;
    }
    /* #101 Handle event of TCP close or reset. */
    case SOAD_TCP_RESET:
    case SOAD_TCP_CLOSED:
    {
      SoAd_SoCon_EventTcpClose(SockIdx, PartitionIdx);
      break;
    }
    /* #102 Handle event of TCP FIN received. */
    case SOAD_TCP_FIN_RECEIVED:
    {
      SoAd_SoCon_EventTcpFinReceived(SockIdx, PartitionIdx);
      break;
    }
    /* #103 Ignore event of TLS handshake succeeded and TLS close notify received. */
    default:
    { /* SOAD_TLS_HANDSHAKE_SUCCEEDED */
      /* SOAD_TLS_EVENT_CLOSENOTIFY_RECEIVED */
      break;
    }
  }
#else
  /* #11 Handle event of UDP close (otherwise). */
  SoAd_SoCon_EventUdpClose(SockIdx, PartitionIdx);

  SOAD_DUMMY_STATEMENT(Event); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_TCP == STD_ON */
} /* SoAd_SoCon_Event() */

/**********************************************************************************************************************
 *  SoAd_SoCon_RequestOpenSoCon()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_RequestOpenSoCon(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that call is interrupted and open close counter wraps around. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if open close counter can handle new request. */
  if ( SoAd_GetOpenCloseCounterOfSoConDyn(SoConIdx, PartitionIdx) < SOAD_INV_BUF_IDX_32 )
  {
    /* #30 Increment counter. */
    SoAd_IncOpenCloseCounterOfSoConDyn(SoConIdx, PartitionIdx);                                                        /* SBSW_SOAD_PARAMETER_IDX */

    /* #40 Set event to handle socket connection state in main function. */
    SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, SoConIdx,
      SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);

    retVal = E_OK;
  }

  /* #50 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_SoCon_RequestOpenSoCon() */

/**********************************************************************************************************************
 *  SoAd_SoCon_RequestCloseSoCon()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_RequestCloseSoCon(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that call is interrupted and open close counter wraps around. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if open close counter can handle new request. */
  if ( SoAd_GetOpenCloseCounterOfSoConDyn(SoConIdx, PartitionIdx) > 0u )
  {
    /* #200 Decrement counter. */
    SoAd_DecOpenCloseCounterOfSoConDyn(SoConIdx, PartitionIdx);                                                        /* SBSW_SOAD_PARAMETER_IDX */

    retVal = E_OK;
  }

  /* #30 Check if open close counter is 0 or 'Abort' flag is set. */
  if ( (SoAd_GetOpenCloseCounterOfSoConDyn(SoConIdx, PartitionIdx) == 0u) || (Abort == TRUE) )
  {
    /* #40 Check if socket connection is not yet offline. */
    if ( SoAd_GetModeOfSoConDyn(SoConIdx, PartitionIdx) != SOAD_SOCON_OFFLINE )
    {
      /* #50 Set flags to close socket connection in main function. */
      if ( Abort == TRUE )
      {
        SoAd_SetAbortOfSoConDyn(SoConIdx, TRUE, PartitionIdx);                                                         /* SBSW_SOAD_PARAMETER_IDX */
      }
      SoAd_SoCon_SetCloseMode(SoConIdx, PartitionIdx, SOAD_CLOSE_OFFLINE);

      /* #60 Set event to handle socket connection state in main function. */
      SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, SoConIdx,
        SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);
    }

    /* #70 Reset open close counter to 0. */
    SoAd_SetOpenCloseCounterOfSoConDyn(SoConIdx, 0u, PartitionIdx);                                                    /* SBSW_SOAD_PARAMETER_IDX */

    retVal = E_OK;
  }

  /* #80 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_SoCon_RequestCloseSoCon() */

/**********************************************************************************************************************
 *  SoAd_SoCon_RequestClose()
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
FUNC(void, SOAD_CODE) SoAd_SoCon_RequestClose(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConCloseModeType CloseMode,
  boolean Abort)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that close mode is read and written inconsistently. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Set the close mode as requested. */
  SoAd_SoCon_SetCloseMode(SoConIdx, PartitionIdx, CloseMode);

#if ( (SOAD_TCP == STD_ON) && (SOAD_RST_FOR_FORCE_RELEASE_ENABLED == STD_ON) )
  /* #30 Set Abort flag if required. */
  if ( Abort == TRUE )
  {
    SoAd_SetAbortOfSoConDyn(SoConIdx, TRUE, PartitionIdx);                                                             /* SBSW_SOAD_PARAMETER_IDX */
  }
#else
  SOAD_DUMMY_STATEMENT(Abort); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (SOAD_TCP == STD_ON) && (SOAD_RST_FOR_FORCE_RELEASE_ENABLED == STD_ON) */

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  /* #50 Add an event to close the socket connection in the next main function. */
  SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, SoConIdx, InstanceIdx, PartitionIdx);
} /* SoAd_SoCon_RequestClose() */

/**********************************************************************************************************************
 *  SoAd_SoCon_GetMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(SoAd_SoConModeType, SOAD_CODE) SoAd_SoCon_GetMode(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return mode of socket connection. */
  return SoAd_GetModeOfSoConDyn(SoConIdx, PartitionIdx);
} /* SoAd_SoCon_GetMode() */

/**********************************************************************************************************************
 *  SoAd_SoCon_GetCloseMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(SoAd_SoConCloseModeType, SOAD_CODE) SoAd_SoCon_GetCloseMode(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return close mode of socket connection. */
  return SoAd_GetCloseModeOfSoConDyn(SoConIdx, PartitionIdx);
} /* SoAd_SoCon_GetCloseMode() */

/**********************************************************************************************************************
 *  SoAd_SoCon_CheckConnectionStates()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_CheckConnectionStates(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSocketType sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType        retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket is open. */
  if ( SoAd_GetStateOfSocketDyn(sockIdx, PartitionIdx) == SOAD_SOCK_STATE_ESTABLISHED )
  {
    /* #20 Check if socket connection is online. */
    if ( SoAd_GetModeOfSoConDyn(SoConIdx, PartitionIdx) == SOAD_SOCON_ONLINE )
    {
      /* #30 Check if socket connection is not requested to be closed. */
      if ( SoAd_GetCloseModeOfSoConDyn(SoConIdx, PartitionIdx) == SOAD_CLOSE_NONE )
      {
        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* SoAd_SoCon_CheckConnectionStates() */

/**********************************************************************************************************************
 *  SoAd_SoCon_HandleSoConStates()
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
FUNC(void, SOAD_CODE) SoAd_SoCon_HandleSoConStates(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_EventQueueStateSoConIterType eventQueueStateSoConIter;
  uint32                            handleIdx = 0u;
  uint32                            elementNumToHandle = SoAd_EventQueue_GetElementNumToHandle(
    SOAD_EVENT_QUEUE_SO_CON_STATE, InstMapIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all queue elements which have to be handled in this context considering configured limit. */
  for ( eventQueueStateSoConIter = 0u;
    eventQueueStateSoConIter < elementNumToHandle;
    eventQueueStateSoConIter++ )
  {
    /* #20 Get next element. */
    if ( SoAd_EventQueue_GetNextElement(SOAD_EVENT_QUEUE_SO_CON_STATE, InstMapIdx, &handleIdx) == E_OK )               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      SoAd_SizeOfSoConType        soConIdx = (SoAd_SizeOfSoConType)handleIdx;
      SoAd_PartitionConfigIdxType partitionIdx =
        SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);

      /* #30 Check for a request for the release of the remote address and handle it. */
      SoAd_RemoteAddr_HandleRelease(soConIdx, partitionIdx);

      /* #40 Check and try to open socket connection if no socket connection close is requested. */
      if ( SoAd_GetCloseModeOfSoConDyn(soConIdx, partitionIdx) == SOAD_CLOSE_NONE )
      {
        SoAd_SoCon_CheckAndOpen(soConIdx, partitionIdx);
      }
      /* #41 Close socket connection otherwise. */
      else
      {
        SoAd_SoCon_Close(soConIdx, partitionIdx);
      }
    }
    /* #21 Stop iteration if no element is left. */
    else
    {
      break;
    }
  }
} /* SoAd_SoCon_HandleSoConStates() */

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_HandleSoConModeChgCbkQueue()
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
FUNC(void, SOAD_CODE) SoAd_SoCon_HandleSoConModeChgCbkQueue(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_EventQueueStateSoConIterType eventQueueStateSoConIter; /* Not initialized: For-loop counter. */
  uint32                            handleIdx = 0u;
  uint32                            elementNumToHandle = SoAd_EventQueue_GetElementNumToHandle(
    SOAD_EVENT_QUEUE_SO_CON_MODE_CHG, InstMapIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all queue elements which have to be handled in this context considering configured limit. */
  for ( eventQueueStateSoConIter = 0u;
    eventQueueStateSoConIter < elementNumToHandle;
    eventQueueStateSoConIter++ )
  {
    /* #20 Get next element. */
    if ( SoAd_EventQueue_GetNextElement(SOAD_EVENT_QUEUE_SO_CON_MODE_CHG, InstMapIdx, &handleIdx) == E_OK )            /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      SoAd_SizeOfSoConType        soConIdx = (SoAd_SizeOfSoConType)handleIdx;
      SoAd_PartitionConfigIdxType partitionIdx =
        SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);

      /* #30 Enter critical section to protect access of mode change callback process flag. */
      SOAD_BEGIN_CRITICAL_SECTION();

      /* #40 Assert that the mode change callback is currently not being handled. */
      if ( !SoAd_IsModeChgQueueProcessFlagOfSoConDyn(soConIdx, partitionIdx) )
      {
        /* #400 Set the mode change callback process flag. */
        SoAd_SetModeChgQueueProcessFlagOfSoConDyn(soConIdx, TRUE, partitionIdx);                                       /* SBSW_SOAD_INDEX_BY_FUNCTION */

        /* #401 Leave critical section. */
        SOAD_END_CRITICAL_SECTION();

        /* #402 Notify the socket connection mode change. */
        SoAd_SoCon_NotifySoConModeChg(soConIdx, partitionIdx, 0u);
      }
      /* #41 Otherwise, delay the mode change reporting again. */
      else
      {
        /* #410 Leave critical section. */
        SOAD_END_CRITICAL_SECTION();

        /* #411 Add element to event queue. */
        SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_MODE_CHG, handleIdx, SoAd_GetInstanceIdxOfSoConGrp(
            SoAd_GetSoConGrpIdxOfSoCon(soConIdx, partitionIdx), partitionIdx), partitionIdx);
      }
    }
    /* #21 Stop iteration if no element is left. */
    else
    {
      break;
    }
  }
} /* SoAd_SoCon_HandleSoConModeChgCbkQueue() */
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_Reconnect()
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
FUNC(void, SOAD_CODE) SoAd_SoCon_Reconnect(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if automatic setup is enabled. */
  if ( SoAd_IsSockAutoSoConSetupOfSoConGrp(soConGrpIdx, PartitionIdx) )
  {
    /* #20 Check if keep online is not enabled. */
    if ( !SoAd_IsSockAutoSoConSetupKeepOnlineOfSoConGrp(soConGrpIdx, PartitionIdx) )
    {
      /* #30 Check if configured remote address contains wildcards. */
      if ( SoAd_RemoteAddr_HasConfigWildcard(SoConIdx, PartitionIdx, FALSE) == TRUE )
      {
        boolean            immediateCbk = TRUE;
        SoAd_SoConModeType newSoConMode = SOAD_SOCON_RECONNECT;
        uint8              errorId = SOAD_E_NO_ERROR;

        /* #40 Reset remote address and received remote address. */
        SoAd_RemoteAddr_ResetRemAndRcvRemAddr(SoConIdx, PartitionIdx);

        /* #50 Enter critical section to prevent that socket connection mode is set in other context. */
        SOAD_BEGIN_CRITICAL_SECTION();

        /* #60 Set socket connection to reconnect. */
        SoAd_SetModeOfSoConDyn(SoConIdx, newSoConMode, PartitionIdx);                                                  /* SBSW_SOAD_PARAMETER_IDX */

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
        /* #70 Add mode change to the mode change queue if enabled. */
        immediateCbk = SoAd_SoCon_AddSoConModeChgQueueElement(SoConIdx, PartitionIdx, newSoConMode, &errorId);         /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

        /* #80 Leave critical section. */
        SOAD_END_CRITICAL_SECTION();

        /* #90 Set event to handle socket connection mode in main function. */
        SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, SoConIdx,
          SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);

        /* #100 Notify user about socket connection mode change. */
        SoAd_SoCon_CheckAndNotifySoConModeChg(SoConIdx, PartitionIdx, newSoConMode, immediateCbk, errorId);
      }
    }
  }
} /* SoAd_SoCon_Reconnect() */

/**********************************************************************************************************************
 *  SoAd_SoCon_RequestReconnect()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_SoCon_RequestReconnect(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket connection has automatic setup enabled. */
  if ( SoAd_IsSockAutoSoConSetupOfSoConGrp(soConGrpIdx, PartitionIdx) )
  {
    /* #20 Check if keep online is not enabled. */
    if ( !SoAd_IsSockAutoSoConSetupKeepOnlineOfSoConGrp(soConGrpIdx, PartitionIdx) )
    {
      /* #30 Check if no not set values and wildcards are configured for remote address. */
      if ( SoAd_RemoteAddr_HasConfigWildcard(SoConIdx, PartitionIdx, TRUE) == TRUE )
      {
        /* #40 Request to reconnect the socket connection in main function context. */
        SoAd_SoCon_RequestClose(SoConIdx, SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx,
          SOAD_CLOSE_RECONNECT, FALSE);
      }
    }
  }
} /* SoAd_SoCon_RequestReconnect() */

/**********************************************************************************************************************
 *  SoAd_SoCon_NotifyRemoteAddrChg()
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
FUNC(void, SOAD_CODE) SoAd_SoCon_NotifyRemoteAddrChg(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean ReconnectOnWildcard)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  boolean                 hasSoConModeChanged = FALSE;
  boolean                 immediateCbk = TRUE;
  SoAd_SoConModeType      newSoConMode = SOAD_SOCON_RECONNECT;
  uint8                   errorId = SOAD_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that socket connection mode is set in other context. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if socket connection is online and shall be set to reconnect since remote address has wildcards. */
  if ( (SoAd_GetModeOfSoConDyn(SoConIdx, PartitionIdx) == SOAD_SOCON_ONLINE) && (ReconnectOnWildcard == TRUE) )
  {
    SoAd_SetModeOfSoConDyn(SoConIdx, newSoConMode, PartitionIdx);                                                      /* SBSW_SOAD_PARAMETER_IDX */
    hasSoConModeChanged = TRUE;

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
    /* #30 Add mode change to the mode change queue if enabled. */
    immediateCbk = SoAd_SoCon_AddSoConModeChgQueueElement(SoConIdx, PartitionIdx, newSoConMode, &errorId);             /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */
  }

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  /* #50 Notify about changed socket connection mode if required. */
  if ( hasSoConModeChanged == TRUE )
  {
    SoAd_SoCon_CheckAndNotifySoConModeChg(SoConIdx, PartitionIdx, newSoConMode, immediateCbk, errorId);
  }
  /* #51 Otherwise, set event to handle socket connection in main function if requested and remote address is set. */
  else if ( ReconnectOnWildcard == FALSE )
  {
    SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, SoConIdx,
      SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);
  }
  else
  {
    /* Nothing to do. */
  }
} /* SoAd_SoCon_NotifyRemoteAddrChg() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_IsTcpSocketConnecting()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, SOAD_CODE) SoAd_SoCon_IsTcpSocketConnecting(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isConnecting = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if TCP socket is connecting. */
  if ( SoAd_GetStateOfSocketDyn(SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) ==
    SOAD_SOCK_STATE_CONNECT )
  {
    isConnecting = TRUE;
  }

  return isConnecting;
} /* SoAd_SoCon_IsTcpSocketConnecting() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseAllSockets()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_SoCon_CloseAllSockets(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_PartitionConfigIdxType partitionIdx = SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);
  SoAd_SocketIterType         sockIter; /* Not initialized: For-loop counter. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all sockets of the current partition. */
  for ( sockIter = 0u; sockIter < SoAd_GetSizeOfSocket(partitionIdx); sockIter++ )
  {
    /* #20 Check if socket belongs to the current instance. */
#if ( SOAD_MULTI_INSTANCE == STD_ON )
    SoAd_SizeOfInstanceType instanceIdx = SoAd_GetPartitionSpecificInstanceIdxOfInstanceMap(InstMapIdx);
    SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(
      SoAd_GetSoConStartIdxOfSocket(sockIter, partitionIdx), partitionIdx);

    if ( SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, partitionIdx) == instanceIdx )
#endif /* SOAD_MULTI_INSTANCE == STD_ON */
    {
      SoAd_SocketIdType socketId = SoAd_GetSocketIdOfSocketDyn(sockIter, partitionIdx);

      /* #30 Check if socket is not closed. */
      if ( SoAd_GetStateOfSocketDyn(sockIter, partitionIdx) != SOAD_SOCK_STATE_CLOSED )
      {
        /* #40 Close socket. */
        (void)SoAd_TcpIpApiClose(socketId, TRUE);
      }
    }
  }
} /* SoAd_SoCon_CloseAllSockets() */

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseAllSoCons()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(boolean, SOAD_CODE) SoAd_SoCon_CloseAllSoCons(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_PartitionConfigIdxType partitionIdx = SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);
  SoAd_SoConIterType          soConIter; /* Not initialized: For-loop counter. */
  boolean                     allSoConsClosed = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all socket connections. */
  for ( soConIter = 0u; soConIter < SoAd_GetSizeOfSoCon(partitionIdx); soConIter++ )
  {
    SoAd_SizeOfSoConType     soConIdx = (SoAd_SizeOfSoConType)soConIter;
    SoAd_SizeOfSoConGrpType  soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConIdx, partitionIdx);

    /* #20 Check if socket connection belongs to the current instance. */
#if ( SOAD_MULTI_INSTANCE == STD_ON )
    SoAd_SizeOfInstanceType  instanceIdx = SoAd_GetPartitionSpecificInstanceIdxOfInstanceMap(InstMapIdx);

    if ( SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, partitionIdx) == instanceIdx )
#endif /* SOAD_MULTI_INSTANCE == STD_ON */
    {
      /* #30 Check if socket connection is not offline or data socket is not closed (i.e. close mode is not reset). */
      if ( (SoAd_GetModeOfSoConDyn(soConIdx, partitionIdx) != SOAD_SOCON_OFFLINE) ||
        (SoAd_GetCloseModeOfSoConDyn(soConIdx, partitionIdx) != SOAD_CLOSE_NONE) )
      {
        /* #40 Indicate that not all socket connections are closed. */
        allSoConsClosed = FALSE;

        /* #50 Check if socket connection is not requested to be closed. */
        if ( SoAd_GetCloseModeOfSoConDyn(soConIdx, partitionIdx) == SOAD_CLOSE_NONE )
        {
          /* #60 Check if neither transmission nor reception is active or pending on socket connection. */
          /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
          if ( (SoAd_Rx_IsActive(soConIdx, partitionIdx) == FALSE) &&
           (SoAd_TxSocketManager_IsTxActive(soConIdx, partitionIdx, TRUE) == FALSE) )
          {
            /* #70 Request to close the socket connection in main function context. */
            SoAd_SoCon_RequestClose(soConIdx, SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, partitionIdx), partitionIdx,
              SOAD_CLOSE_OFFLINE_RESET, FALSE);
          }
        }
      }
    }
  }

  return allSoConsClosed;
} /* SoAd_SoCon_CloseAllSoCons() */

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSoConOnReception()
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
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSoConOnReception(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  /* #10 Check if socket connection is of type UDP (if TCP is enabled). */
  if ( SoAd_IsSocketUdpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) )
#endif /* SOAD_TCP == STD_ON */
  {
    /* #20 Check if message acceptance filter is enabled. */
    if ( SoAd_IsMsgAcceptFilterEnabledOfSoConGrp(soConGrpIdx, PartitionIdx) )
    {
      /* #30 Check if UDP listen only is disabled. */
      if ( !SoAd_IsUdpListenOnlyOfSocketUdp(SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
      {
        /* #40 Enter critical section to prevent that remote address, mode or alive timeout is set in other context. */
        SOAD_BEGIN_CRITICAL_SECTION();

        /* #50 Set remote address if allowed. */
        if ( SoAd_RemoteAddr_Set(SoConIdx, PartitionIdx, RemoteAddrPtr, TRUE, FALSE) == E_OK )                         /* SBSW_SOAD_POINTER_FORWARD */
        {
          boolean            immediateCbk = TRUE;
          SoAd_SoConModeType newSoConMode = SOAD_SOCON_ONLINE;
          uint8              errorId = SOAD_E_NO_ERROR;

          /* #60 Set socket connection to online. */
          SoAd_SetModeOfSoConDyn(SoConIdx, newSoConMode, PartitionIdx);                                                /* SBSW_SOAD_PARAMETER_IDX */

          /* #70 Set alive supervision timeout. */
          SoAd_SoCon_SetUdpAliveTimeout(SoConIdx, PartitionIdx);

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
          /* #80 Add mode change to the mode change queue if enabled. */
          immediateCbk = SoAd_SoCon_AddSoConModeChgQueueElement(SoConIdx, PartitionIdx, newSoConMode, &errorId);       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

          /* #90 Leave critical section. */
          SOAD_END_CRITICAL_SECTION();

          /* #100 Notify user about mode change. */
          SoAd_SoCon_CheckAndNotifySoConModeChg(SoConIdx, PartitionIdx, newSoConMode, immediateCbk, errorId);

          /* #110 Start TP reception if required and succeeds. */
          retVal = SoAd_Rx_TpStartOfReception(SoConIdx, PartitionIdx);
        }
        /* #120 Leave critical section otherwise. */
        else
        {
          SOAD_END_CRITICAL_SECTION();
        }
      }
    }
  }

  return retVal;
} /* SoAd_SoCon_OpenSoConOnReception() */

/**********************************************************************************************************************
 *  SoAd_SoCon_HandleUdpAliveTimeout()
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
FUNC(void, SOAD_CODE) SoAd_SoCon_HandleUdpAliveTimeout(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32                      elementIdx = 0u;
  uint32                      handleIdx = SOAD_INV_BUF_IDX_16;
  SoAd_PartitionConfigIdxType partitionIdx = SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);
  SoAd_SizeOfInstanceType     instanceIdx = SoAd_GetPartitionSpecificInstanceIdxOfInstanceMap(InstMapIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all timeouts in this main function cycle. */
  while ( SoAd_TimeoutList_CheckElements(SOAD_TIMEOUT_LIST_UDP_ALIVE, &elementIdx, &handleIdx, InstMapIdx) == E_OK )   /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
     SoAd_SizeOfSoConType soConIdx = (SoAd_SizeOfSoConType)handleIdx;

    /* #20 Enter critical section to prevent that close mode is overwritten by any function call. */
    SOAD_BEGIN_CRITICAL_SECTION();

    /* #30 Check if socket connection is online and socket connection is not requested to be closed. */
    if ( (SoAd_GetModeOfSoConDyn(soConIdx, partitionIdx) == SOAD_SOCON_ONLINE) &&
      (SoAd_GetCloseModeOfSoConDyn(soConIdx, partitionIdx) == SOAD_CLOSE_NONE) )
    {
      /* #300 Request socket connection to be reconnected. */
      SoAd_SoCon_SetCloseMode(soConIdx, partitionIdx, SOAD_CLOSE_RECONNECT);

      /* #301 Leave critical section. */
      SOAD_END_CRITICAL_SECTION();

      /* #302 Set an event to handle socket connection reconnect in main function. */
      SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, soConIdx, instanceIdx, partitionIdx);
    }
    /* #31 Leave critical section otherwise. */
    else
    {
      SOAD_END_CRITICAL_SECTION();
    }

    /* #40 Remove timeout from list. */
    SoAd_TimeoutList_RemoveElement(SOAD_TIMEOUT_LIST_UDP_ALIVE, soConIdx, instanceIdx, partitionIdx);
  }
} /* SoAd_SoCon_HandleUdpAliveTimeout() */

/**********************************************************************************************************************
 *  SoAd_SoCon_UpdateUdpAliveTimeout()
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
FUNC(void, SOAD_CODE) SoAd_SoCon_UpdateUdpAliveTimeout(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32                   tmpCnt = 0u;
  SoAd_SizeOfSoConGrpType  soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfInstanceType  instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  /* #10 Check if socket connection is of type UDP (if TCP is enabled). */
  if ( SoAd_IsSocketUdpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) )
#endif /* SOAD_TCP == STD_ON */
  {
    /* #20 Check if timeout is configured. */
    if ( SoAd_GetAliveTimeoutMaxCntOfSocketUdp(socketUdpIdx, PartitionIdx) != 0u )
    {
      /* #30 Enter critical section to prevent that timeout update process is interrupted. */
      SOAD_BEGIN_CRITICAL_SECTION();

      /* #40 Check if timeout is already running. */
      if ( SoAd_TimeoutList_GetCurrentTimeout(SOAD_TIMEOUT_LIST_UDP_ALIVE, SoConIdx, &tmpCnt,                          /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
           SoAd_GetCounterOfInstanceDyn(instanceIdx, PartitionIdx), PartitionIdx) == E_OK )
      {
        /* #50 Update timeout. */
        (void)SoAd_TimeoutList_SetElement(SOAD_TIMEOUT_LIST_UDP_ALIVE, SoConIdx, SoAd_GetAliveTimeoutMaxCntOfSocketUdp(
            socketUdpIdx, PartitionIdx), instanceIdx, PartitionIdx);
      }

      /* #60 Leave critical section. */
      SOAD_END_CRITICAL_SECTION();
    }
  }
} /* SoAd_SoCon_UpdateUdpAliveTimeout() */

/**********************************************************************************************************************
 *  SoAd_SoCon_ResetUdpAliveTimeout()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_SoCon_ResetUdpAliveTimeout(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConGrpType  soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
  SoAd_SizeOfSocketUdpType socketUdpIdx = SoAd_GetSocketUdpIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if alive supervision timeout is configured. */
  if ( SoAd_GetAliveTimeoutMaxCntOfSocketUdp(socketUdpIdx, PartitionIdx) != 0u )
  {
    SoAd_SizeOfInstanceType instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

    /* #20 Remove alive supervision timeout from timeout list. */
    SoAd_TimeoutList_RemoveElement(SOAD_TIMEOUT_LIST_UDP_ALIVE, SoConIdx, instanceIdx, PartitionIdx);
  }
} /* SoAd_SoCon_ResetUdpAliveTimeout() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_HandleTcpAutoConnectTimeout()
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
FUNC(void, SOAD_CODE) SoAd_SoCon_HandleTcpAutoConnectTimeout(
  SoAd_SizeOfInstanceMapType InstMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 elementIdx = 0u;
  uint32 handleIdx = SOAD_INV_BUF_IDX_16;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all timeouts in this main function cycle. */
  while ( SoAd_TimeoutList_CheckElements(SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT, &elementIdx, &handleIdx, InstMapIdx) ==   /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    E_OK )
  {
    SoAd_SizeOfSoConType        soConIdx = (SoAd_SizeOfSoConType)handleIdx;
    SoAd_PartitionConfigIdxType partitionIdx = SoAd_GetPartitionSpecificInstancePartitionIdxOfInstanceMap(InstMapIdx);
    SoAd_SizeOfInstanceType     instanceIdx = SoAd_GetPartitionSpecificInstanceIdxOfInstanceMap(InstMapIdx);

    /* #20 Report runtime error. */
# if ( SOAD_RUNTIME_ERROR_REPORT == STD_ON )
    (void)Det_ReportRuntimeError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_MAIN_FUNCTION_STATE,
      SOAD_E_TCP_AUTOCONNECT_FAILED);
# endif /* SOAD_RUNTIME_ERROR_REPORT == STD_ON */

    /* #30 Enter critical section to prevent that socket connection related parameter are set inconsistent. */
    SOAD_BEGIN_CRITICAL_SECTION();

    /* #40 Set close mode to indicate that socket is closing. */
    SoAd_SoCon_SetCloseMode(soConIdx, partitionIdx, SOAD_CLOSE_OFFLINE);

    /* #50 Set event to handle socket connection state in main function. */
    SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, soConIdx, instanceIdx, partitionIdx);

    /* #60 Assert that socket connection will not be tried to be opened again until reinitialization of the SoAd. */
    SoAd_SetTimeoutOccurredOfSoConDyn(soConIdx, TRUE, partitionIdx);                                                   /* SBSW_SOAD_INDEX_BY_FUNCTION */

    /* #70 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();

    /* #80 Remove timeout from list. */
    SoAd_TimeoutList_RemoveElement(SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT, soConIdx, instanceIdx, partitionIdx);
  }
} /* SoAd_SoCon_HandleTcpAutoConnectTimeout() */
#endif /* SOAD_TCP == STD_ON */

#if ( (SOAD_TCP == STD_ON) && (SOAD_VERIFYRXPDUCBK == STD_ON) )
/**********************************************************************************************************************
 *  SoAd_SoCon_GetLocalSockAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_SoCon_GetLocalSockAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) LocalSockAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockAddrInetXType  dummyDefaultRouter = { 0 };
  uint8                   dummyNetmask = 0u;
  SoAd_SizeOfSocketType   sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward request to TcpIp to get local IP address from TcpIp module. */
  /* PRQA S 310 2 */ /* MD_SoAd_PointerCastOnStruct */
  SoAd_LocalAddr_GetIpAddr(SoConIdx, PartitionIdx, LocalSockAddrPtr, &dummyNetmask,
    SOAD_A_P2VAR(SoAd_SockAddrType)&dummyDefaultRouter);                                                               /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */

  /* #20 Get port from local data structure. */
  /* PRQA S 310, 3305 1 */ /* MD_SoAd_PointerCastOnStruct */
  ((SoAd_SockAddrInetXType*)LocalSockAddrPtr)->port = SoAd_SoCon_GetLocalPortDyn(sockIdx, PartitionIdx);               /* SBSW_SOAD_POINTER_WRITE */
} /* SoAd_SoCon_GetLocalSockAddr() */
#endif /* (SOAD_TCP == STD_ON) && (SOAD_VERIFYRXPDUCBK == STD_ON) */

/**********************************************************************************************************************
 *  SoAd_SoCon_UpdateLocalPort()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_SoCon_UpdateLocalPort(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 Port)
{
  /* ----- Local Variables ---------------------------------------------- */
  const SoAd_SizeOfSocketType sockIdx = SoAd_GetSocketIdxOfSoCon(SoConIdx, PartitionIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Local Port is any. */
  if ( SoAd_SoCon_GetLocalPortDyn(sockIdx, PartitionIdx) == SOAD_PORT_ANY )
  {
    /* #20 Set port of current local address. */
    SoAd_SoCon_SetLocalPortDyn(sockIdx, PartitionIdx, Port);
  }
} /* SoAd_SoCon_UpdateLocalPort() */

/**********************************************************************************************************************
 *  SoAd_SoCon_GetLocalPortDyn()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(SoAd_PortType, SOAD_CODE) SoAd_SoCon_GetLocalPortDyn(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_PortType           localPort; /* Not initialized: Improved readability. */
#if ( SOAD_TCP == STD_ON )
  SoAd_SizeOfSoConType    soConStartIdx = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConStartIdx, PartitionIdx);
#endif /* SOAD_TCP == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  /* #10 Return local port of listen socket in case of TCP server socket (if enabled). */
  if ( SoAd_IsSocketTcpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) &&
    !SoAd_IsTcpInitiateOfSocketTcp(SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
  {
    localPort = SoAd_GetLocalPortOfSocketDyn(SoAd_GetSocketIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx);
  }
  else
#endif /* SOAD_TCP == STD_ON */
  /* #20 Return local port of data socket otherwise. */
  {
    localPort = SoAd_GetLocalPortOfSocketDyn(SockIdx, PartitionIdx);
  }

  return localPort;
} /* SoAd_SoCon_GetLocalPortDyn() */

/**********************************************************************************************************************
 *  SoAd_SoCon_SetLocalPortDyn()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_SoCon_SetLocalPortDyn(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_PortType LocalPort)
{
  /* ----- Local Variables ---------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  SoAd_SizeOfSoConType    soConStartIdx = SoAd_GetSoConStartIdxOfSocket(SockIdx, PartitionIdx);
  SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(soConStartIdx, PartitionIdx);
#endif /* SOAD_TCP == STD_ON */

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  /* #10 Set local port on listen socket in case of TCP server socket (if enabled). */
  if ( SoAd_IsSocketTcpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) &&
    !SoAd_IsTcpInitiateOfSocketTcp(SoAd_GetSocketTcpIdxOfSoConGrp(soConGrpIdx, PartitionIdx), PartitionIdx) )
  {
    /* Optional indirection: Refer to Safe BSW assumption SoConGrp -> Socket. */
    SoAd_SetLocalPortOfSocketDyn(SoAd_GetSocketIdxOfSoConGrp(soConGrpIdx, PartitionIdx), LocalPort, PartitionIdx);     /* SBSW_SOAD_OPTIONAL_CSL03_CSL05 */
  }
  else
#endif /* SOAD_TCP == STD_ON */
  /* #20 Set local port on data socket otherwise. */
  {
    SoAd_SetLocalPortOfSocketDyn(SockIdx, LocalPort, PartitionIdx);                                                    /* SBSW_SOAD_CSL02_CSL05 */
  }
} /* SoAd_SoCon_SetLocalPortDyn() */

/**********************************************************************************************************************
 *  SoAd_SoCon_GetSocketId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(SoAd_SocketIdType, SOAD_CODE) SoAd_SoCon_GetSocketId(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return socket identifier. */
  return SoAd_GetSocketIdOfSocketDyn(SockIdx, PartitionIdx);
} /* SoAd_SoCon_GetSocketId() */

/**********************************************************************************************************************
 *  SoAd_SoCon_GetSocketState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(SoAd_SockStateType, SOAD_CODE) SoAd_SoCon_GetSocketState(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return current state of SocketDyn. */
  return SoAd_GetStateOfSocketDyn(SockIdx, PartitionIdx);
} /* SoAd_SoCon_GetSocketState() */

/**********************************************************************************************************************
 *  SoAd_SoCon_NotifyLocalIpAddrAssignmentChg()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_SoCon_NotifyLocalIpAddrAssignmentChg(
  SoAd_SizeOfSoConGrpType SoConGrpIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_IpAddrStateType State)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SoConIterType soConIter; /* Not initialized: For-loop counter. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all socket connections which are related to the socket connection group. */
  for ( soConIter = SoAd_GetSoConStartIdxOfSoConGrp(SoConGrpIdx, PartitionIdx);
    soConIter < SoAd_GetSoConEndIdxOfSoConGrp(SoConGrpIdx, PartitionIdx); soConIter++ )
  {
    SoAd_SizeOfSoConType                        soConIdx = (SoAd_SizeOfSoConType)soConIter;
    SoAd_LocalIpAddrAssignmentChgCbkIndIterType localIpAddrAssignmentChgCbkIndIter; /* Not initialized: For-loop
                                                                                     * counter. */

    /* #20 Call all socket connection related socket connection local IP address assignment change notifications. */
    for ( localIpAddrAssignmentChgCbkIndIter = SoAd_GetLocalIpAddrAssignmentChgCbkIndStartIdxOfSoCon(soConIdx,
        PartitionIdx);
      localIpAddrAssignmentChgCbkIndIter < SoAd_GetLocalIpAddrAssignmentChgCbkIndEndIdxOfSoCon(soConIdx, PartitionIdx);
      localIpAddrAssignmentChgCbkIndIter++ )
    {
      if ( SoAd_GetLocalIpAddrAssignmentChgCbk(SoAd_GetLocalIpAddrAssignmentChgCbkInd(
          localIpAddrAssignmentChgCbkIndIter, PartitionIdx)) != NULL_PTR )
      {
        SoAd_GetLocalIpAddrAssignmentChgCbk(SoAd_GetLocalIpAddrAssignmentChgCbkInd(
          localIpAddrAssignmentChgCbkIndIter, PartitionIdx))( SoAd_GetSoConIdOfSoCon(soConIdx, PartitionIdx), State);  /* SBSW_SOAD_FUNCTION_PTR */
      }
    }
    /* #30 Set event to handle socket connection state in main function. */
    SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, soConIdx,
      SoAd_GetInstanceIdxOfSoConGrp(SoConGrpIdx, PartitionIdx), PartitionIdx);

#if ( SOAD_TCP == STD_ON )
    /* #40 Overwrite close mode to prevent that socket connection is opened automatically on IP reassignment. */
    if ( SoAd_GetCloseModeOfSoConDyn(soConIdx, PartitionIdx) == SOAD_CLOSE_SOCKET_RECONNECT )
    {
      SOAD_BEGIN_CRITICAL_SECTION();

      SoAd_SoCon_SetCloseMode(soConIdx, PartitionIdx, SOAD_CLOSE_SOCKET);

      SOAD_END_CRITICAL_SECTION();
    }
#endif /* SOAD_TCP == STD_ON */
  }
} /* SoAd_SoCon_NotifyLocalIpAddrAssignmentChg() */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_SoCon.c
 *********************************************************************************************************************/
