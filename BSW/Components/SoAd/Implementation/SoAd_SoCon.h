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
/*!        \file  SoAd_SoCon.h
 *        \brief  Socket Adaptor header file
 *
 *      \details  Vector static code header file for AUTOSAR Socket Adaptor sub-component SoCon.
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

#if !defined (SOAD_SO_CON_H)
# define SOAD_SO_CON_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "SoAd.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

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

# define SOAD_START_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
  SoAd_SoCon_InitSoCon()
**********************************************************************************************************************/
/*! \brief        Initializes socket connection structs.
 *  \details      -
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_InitSoCon(
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_InitSocket()
 *********************************************************************************************************************/
/*! \brief        Initializes module internal socket index structs.
 *  \details      -
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_InitSocket(
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_GetSockIdxBySocketId()
 *********************************************************************************************************************/
/*! \brief        Returns module internal socket index of a socket identifier if mapping is available.
 *  \details      -
 *  \param[in]    SocketId        Socket identifier.
 *  \param[out]   SockIdxPtr      Pointer to the module internal partition specific socket index.
 *  \param[out]   PartitionIdxPtr Pointer to the partition index of the socket.
 *  \return       E_OK            Mapping found and SockIdxPtr and PartitionIdxPtr are valid.
 *  \return       E_NOT_OK        No mapping found.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_GetSockIdxBySocketId(
  SoAd_SocketIdType SocketId,
  P2VAR(SoAd_SizeOfSocketType, AUTOMATIC, SOAD_APPL_DATA) SockIdxPtr,
  P2VAR(SoAd_PartitionConfigIdxType, AUTOMATIC, SOAD_APPL_DATA) PartitionIdxPtr);

/**********************************************************************************************************************
 *  SoAd_SoCon_GetSoConIdxBySoConId()
 *********************************************************************************************************************/
/*! \brief        Checks the validity of the socket connection map identifier and gets the partition specific index.
 *  \details      -
 *  \pre          -
 *  \param[in]    SoConId         Socket connection identifier.
 *  \param[out]   SoConIdxPtr     Pointer to the partition specific SoConIdx.
 *  \param[out]   PartitionIdxPtr Pointer to the partition index of the SoCon.
 *  \return       E_OK            SoCon map identifier is inside valid range.
 *  \return       E_NOT_OK        SoCon map identifier is not inside valid range.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_GetSoConIdxBySoConId(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SizeOfSoConType, AUTOMATIC, SOAD_APPL_DATA) SoConIdxPtr,
  P2VAR(SoAd_PartitionConfigIdxType, AUTOMATIC, SOAD_APPL_DATA) PartitionIdxPtr);

/**********************************************************************************************************************
 *  SoAd_SoCon_GetLocalAddrIdx()
 *********************************************************************************************************************/
/*! \brief        Get local address of the current socket connection.
 *  \details      -
 *  \param[in]    SoConIdx     Socket connection index.
 *                             [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       LocalAddrIdx Index of local address.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(SoAd_SizeOfLocalAddrType, SOAD_CODE) SoAd_SoCon_GetLocalAddrIdx(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_GetSocketIdx()
 *********************************************************************************************************************/
/*! \brief        Get socketIdx of the current socket connection.
 *  \details      -
 *  \param[in]    SoConIdx     Socket connection index.
 *                             [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       SocketIdx    Index of socket.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(SoAd_SizeOfSocketType, SOAD_CODE) SoAd_SoCon_GetSocketIdx(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

# if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_TcpAccepted()
 *********************************************************************************************************************/
/*! \brief        Accepts TCP connections on a listen socket.
 *  \details      -
 *  \param[in]    SockIdx             Module internal socket index.
 *                                    [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx        Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    SocketIdConnected   Connected socket identifier.
 *  \param[in]    RemoteAddrPtr       Pointer to remote address.
 *                                    [Points to one of the following structs depending on configured IP address version
 *                                    of parameter SoConIdx:
 *                                      - SoAd_SockAddrInetType for IPv4
 *                                      - SoAd_SockAddrInet6Type for IPv6]
 *  \return       E_OK                Connection was accepted.
 *  \return       E_NOT_OK            Connection was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket indexes, FALSE for same socket index.
 *  \config       SOAD_TCP
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpAccepted(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketIdConnected,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr);

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpAcceptedGetSoConIdx()
 *********************************************************************************************************************/
/*! \brief        Returns socket connection of a socket connection group on connection establishment on listen socket.
 *  \details      Uses best match algorithm to get socket connection if required. Parameter "SoConIdx" is used to
 *                identify the socket connection group.
 *  \param[in]    SoConIdx            Socket connection index.
 *                                    [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx        Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    RemoteAddrPtr       Pointer to remote address.
 *                                    [Points to one of the following structs depending on configured IP address version
 *                                    of parameter SoConIdx:
 *                                      - SoAd_SockAddrInetType for IPv4
 *                                      - SoAd_SockAddrInet6Type for IPv6]
 *  \param[out]   SoConIdxPtr         Pointer to socket connection index.
 *  \return       E_OK                Socket connection was found.
 *  \return       E_NOT_OK            No socket connection was found.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config       SOAD_TCP
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpAcceptedGetSoConIdx(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  P2VAR(SoAd_SizeOfSoConType, AUTOMATIC, SOAD_APPL_DATA) SoConIdxPtr);

/**********************************************************************************************************************
 *  SoAd_SoCon_TcpConnected()
 *********************************************************************************************************************/
/*! \brief        Handles TCP connections which have been initiated locally and are now successfully connected.
 *  \details      -
 *  \param[in]    SockIdx         Module internal socket index.
 *                                [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket indexes, FALSE for same socket index.
 *  \config       SOAD_TCP
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_TcpConnected(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
  SoAd_SoCon_TcpChangeParameter()
**********************************************************************************************************************/
/*! \brief        Changes parameters on a TCP socket via the Socket API.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    SocketId        Socket identifier.
 *  \return       E_OK            Parameter change request was accepted.
 *  \return       E_NOT_OK        Parameter change request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \config       SOAD_TCP
 *  \synchronous  TRUE
 *  \trace        DSGN-SoAdHandleSockets
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_TcpChangeParameter(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SocketIdType SocketId);
# endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_Event()
 *********************************************************************************************************************/
/*! \brief        Handles events on socket.
 *  \details      -
 *  \param[in]    SockIdx      Module internal socket index.
 *                             [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    Event        Event type.
 *                             [range: SOAD_TCP_RESET, SOAD_TCP_CLOSED, SOAD_TCP_FIN_RECEIVED, SOAD_UDP_CLOSED,
 *                             SOAD_TLS_HANDSHAKE_SUCCEEDED]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket indexes, FALSE for same socket index.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_Event(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_EventType Event);

/**********************************************************************************************************************
 *  SoAd_SoCon_RequestOpenSoCon()
 *********************************************************************************************************************/
/*! \brief        Stores a socket connection open request.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       E_OK            Request succeeded.
 *  \return       E_NOT_OK        Request failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_RequestOpenSoCon(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_RequestCloseSoCon()
 *********************************************************************************************************************/
/*! \brief        Stores a socket connection close request.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    Abort           Flag to close socket connection immediately.
 *  \return       E_OK            Request succeeded.
 *  \return       E_NOT_OK        Request failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_RequestCloseSoCon(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean Abort);

/**********************************************************************************************************************
 *  SoAd_SoCon_RequestClose()
 *********************************************************************************************************************/
/*! \brief        Sets the close mode and adds an element to the event queue to handle the mode change in the next main
 *                function.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    InstanceIdx     Instance index.
 *                                [range: InstanceIdx < SoAd_GetSizeOfInstance()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    CloseMode       Requested close mode.
 *                                [range: SOAD_CLOSE_SOCKET_RECONNECT .. SOAD_CLOSE_OFFLINE_RESET]
 *  \param[in]    Abort           Flag to close socket connection immediately.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_RequestClose(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_SoConCloseModeType CloseMode,
  boolean Abort);

/**********************************************************************************************************************
 *  SoAd_SoCon_GetMode()
 *********************************************************************************************************************/
/*! \brief        Gets mode of the current socket connection.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       SOAD_SOCON_ONLINE
 *  \return       SOAD_SOCON_RECONNECT
 *  \return       SOAD_SOCON_OFFLINE
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(SoAd_SoConModeType, SOAD_CODE) SoAd_SoCon_GetMode(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_GetCloseMode()
 *********************************************************************************************************************/
/*! \brief        Gets close mode of the current socket connection.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       SOAD_CLOSE_NONE
 *  \return       SOAD_CLOSE_SOCKET_RECONNECT
 *  \return       SOAD_CLOSE_SOCKET
 *  \return       SOAD_CLOSE_RECONNECT
 *  \return       SOAD_CLOSE_OFFLINE
 *  \return       SOAD_CLOSE_OFFLINE_RESET
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(SoAd_SoConCloseModeType, SOAD_CODE) SoAd_SoCon_GetCloseMode(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_CheckConnectionStates()
 *********************************************************************************************************************/
/*! \brief        Checks state of the socket connection and its corresponding socket.
 *  \details      Checks if the socket connection is open and not requested to be closed and if the corresponding
 *                socket is established.
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       E_OK            Socket connection is open and not requested to be closed and socket is established.
 *  \return       E_NOT_OK        Either socket connection state or socket state is not as expected.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_CheckConnectionStates(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_HandleSoConStates()
 *********************************************************************************************************************/
/*! \brief        Handles socket connection open and close actions in main function context.
 *  \details      -
 *  \param[in]    InstMapIdx   Instance Map index.
 *                             [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_HandleSoConStates(
  SoAd_SizeOfInstanceMapType InstMapIdx);

#if ( SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_HandleSoConModeChgCbkQueue()
 *********************************************************************************************************************/
/*! \brief        Handles socket connection mode change reporting in main function context.
 *  \details      -
 *  \param[in]    InstMapIdx   Instance Map index.
 *                             [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_HandleSoConModeChgCbkQueue(
  SoAd_SizeOfInstanceMapType InstMapIdx);
#endif /* SOAD_MODE_CHG_QUEUE_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_Reconnect()
 *********************************************************************************************************************/
/*! \brief        Reconnects a socket connection by resetting the corresponding structs and notifying the user.
 *  \details      Checks if automatic setup is enabled and remote address has wildcards configured before reconnecting.
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_Reconnect(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_RequestReconnect()
 *********************************************************************************************************************/
/*! \brief        Requests reconnection of a socket connection in main function.
 *  \details      Used to reconnect socket connection with auto setup and wildcards after transmission.
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_RequestReconnect(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_NotifyRemoteAddrChg()
 *********************************************************************************************************************/
/*! \brief        Notifies about a remote address change.
 *  \details      Checks if socket connection is online and shall be set to reconnect since the newly set remote
 *                address has wildcards. The mode is set accordingly and the user is notified about the socket
 *                connection mode change.
 *  \param[in]    SoConIdx             Socket connection index.
 *                                     [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx         Partition index.
 *                                     [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    ReconnectOnWildcard  Flag to indicate if reconnect mode should be set if remote address contains
 *                                     wildcards.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_NotifyRemoteAddrChg(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean ReconnectOnWildcard);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_IsTcpSocketConnecting()
 *********************************************************************************************************************/
/*! \brief        Checks if the current TCP socket is connecting.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config       SOAD_TCP
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(boolean, SOAD_CODE) SoAd_SoCon_IsTcpSocketConnecting(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseAllSockets()
 *********************************************************************************************************************/
/*! \brief        Closes all configured sockets and implicitly the corresponding socket connections.
 *  \details      Used to close all socket connections in case of module shutdown.
 *  \param[in]    InstMapIdx   Instance Map index.
 *                             [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_CloseAllSockets(
  SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_CloseAllSoCons()
 *********************************************************************************************************************/
/*! \brief        Closes all configured socket connections considering if a transmission is pending.
 *  \details      Used to close all socket connections in case of module shutdown.
 *  \param[in]    InstMapIdx      Instance Map index.
 *                                [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \return       TRUE            All socket connections are offline.
 *  \return       FALSE           At least one socket connection is not in state offline.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(boolean, SOAD_CODE) SoAd_SoCon_CloseAllSoCons(
  SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_OpenSoConOnReception()
 *********************************************************************************************************************/
/*! \brief        Tries to open a socket connection on reception.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    RemoteAddrPtr   Pointer to remote address.
 *                                [Points to one of the following structs depending on configured IP address version of
 *                                parameter SoConIdx:
 *                                  - SoAd_SockAddrInetType for IPv4
 *                                  - SoAd_SockAddrInet6Type for IPv6]
 *  \return       E_OK            Socket connection open succeeded.
 *  \return       E_NOT_OK        Socket connection open failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SoCon_OpenSoConOnReception(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr);

/**********************************************************************************************************************
 *  SoAd_SoCon_HandleUdpAliveTimeout()
 *********************************************************************************************************************/
/*! \brief        Handles UDP alive supervision timeout on a socket connection in main function.
 *  \details      -
 *  \param[in]    InstMapIdx   Instance Map index.
 *                             [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \trace        DSGN-SoAdCloseByTimeout
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_HandleUdpAliveTimeout(
  SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_UpdateUdpAliveTimeout()
 *********************************************************************************************************************/
/*! \brief        Updates an already running UDP Alive Supervision Timeout on a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *  \trace        DSGN-SoAdCloseByTimeout
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_UpdateUdpAliveTimeout(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_ResetUdpAliveTimeout()
 *********************************************************************************************************************/
/*! \brief        Resets the UDP alive supervision timeout on a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon(),
 *                                SoAd_IsSocketUdpUsedOfSoConGrp()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_ResetUdpAliveTimeout(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_SoCon_HandleTcpAutoConnectTimeout()
 *********************************************************************************************************************/
/*! \brief        Handles TCP auto connect timeout on a socket connection in main function.
 *  \details      -
 *  \param[in]    InstMapIdx   Instance Map index.
 *                             [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \config       SOAD_TCP
 *  \synchronous  TRUE
 *  \trace        DSGN-SoAdManualOpenAndClose
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_HandleTcpAutoConnectTimeout(
  SoAd_SizeOfInstanceMapType InstMapIdx);
#endif /* SOAD_TCP == STD_ON */

#if ( (SOAD_TCP == STD_ON) && (SOAD_VERIFYRXPDUCBK == STD_ON) )
/**********************************************************************************************************************
 *  SoAd_SoCon_GetLocalSockAddr()
 *********************************************************************************************************************/
/*! \brief        Returns local socket address (local IP address and port) of a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx            Socket connection index.
 *                                    [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx        Partition index.
 *                                    [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out]   LocalSockAddrPtr    Pointer to local socket address.
 *                                    [Points to one of the following structs depending on configured IP address
 *                                    version of parameter SoConIdx:
 *                                      - SoAd_SockAddrInetType for IPv4
 *                                      - SoAd_SockAddrInet6Type for IPv6]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \config       SOAD_TCP & SOAD_VERIFYRXPDUCBK
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_GetLocalSockAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) LocalSockAddrPtr);
#endif /* (SOAD_TCP == STD_ON) && (SOAD_VERIFYRXPDUCBK == STD_ON) */

/**********************************************************************************************************************
 *  SoAd_SoCon_UpdateLocalPort()
 *********************************************************************************************************************/
/*! \brief        Update current port of local address.
 *  \details      Context is a successful request of Ip address Assignment and the assignment type is
 *                SOAD_IPADDR_ASSIGNMENT_STATIC.
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    Port            Port of IP address which shall be assigned.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different local IP addresses, FALSE for same local IP address.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_UpdateLocalPort(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  uint16 Port);

/**********************************************************************************************************************
 *  SoAd_SoCon_GetLocalPortDyn()
 *********************************************************************************************************************/
/*! \brief        Returns local port of a socket.
 *  \details      Considers that a local port is shared by multiple sockets/socket connections.
 *  \param[in]    SockIdx       Module internal socket index.
 *                              [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       LocalPort     Local port.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(SoAd_PortType, SOAD_CODE) SoAd_SoCon_GetLocalPortDyn(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_SetLocalPortDyn()
 *********************************************************************************************************************/
/*! \brief        Sets local port of a socket.
 *  \details      Considers that a local port is shared by multiple sockets/socket connections.
 *  \param[in]    SockIdx       Module internal socket index.
 *                              [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    LocalPort     Local port.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_SetLocalPortDyn(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_PortType LocalPort);

/**********************************************************************************************************************
 *  SoAd_SoCon_GetSocketId()
 *********************************************************************************************************************/
/*! \brief        Returns socket identifier.
 *  \details      -
 *  \param[in]    SockIdx       Module internal socket index.
 *                              [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       SocketId      Socket identifier.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(SoAd_SocketIdType, SOAD_CODE) SoAd_SoCon_GetSocketId(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_GetSocketState()
 *********************************************************************************************************************/
/*! \brief        Returns state of a socket.
 *  \details      -
 *  \param[in]    SockIdx       Module internal socket index.
 *                              [range: SockIdx < SoAd_GetSizeOfSocket()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       SOAD_SOCK_STATE_ESTABLISHED
 *  \return       SOAD_SOCK_STATE_CONNECT
 *  \return       SOAD_SOCK_STATE_LISTEN
 *  \return       SOAD_SOCK_STATE_CLOSING
 *  \return       SOAD_SOCK_STATE_CLOSED
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(SoAd_SockStateType, SOAD_CODE) SoAd_SoCon_GetSocketState(
  SoAd_SizeOfSocketType SockIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_SoCon_NotifyLocalIpAddrAssignmentChg()
 *********************************************************************************************************************/
/*! \brief        Notifies users about a socket connection local IP address assignment change.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    State           State of IP address assignment.
 *                                [range: SOAD_IPADDR_STATE_ASSIGNED, SOAD_IPADDR_STATE_ONHOLD,
 *                                SOAD_IPADDR_STATE_UNASSIGNED]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        DSGN-SoAdNotifyLocalIpAddressAssignmentStateChange
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_SoCon_NotifyLocalIpAddrAssignmentChg(
  SoAd_SizeOfSoConGrpType SoConGrpIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_IpAddrStateType State);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* SOAD_SO_CON_H */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_SoCon.h
 *********************************************************************************************************************/
