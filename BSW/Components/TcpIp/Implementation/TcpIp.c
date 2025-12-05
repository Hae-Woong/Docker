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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  TcpIp.c
 *        \brief  Implementation of TcpIp Module
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

/*lint -e451 */ /* Suppress ID451 because TcpIp_MemMap.h / MemMap.h cannot use a include guard */


/* PRQA S 0828 EOF */ /* MD_MSR_1.1 */

#define TCPIP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "TcpIp.h"
#include "TcpIp_Cbk.h"
#include "TcpIp_Priv.h"
#include "TcpIp_Lcfg.h"

#include "IpBase.h"

#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# include "TcpIp_IpV4.h"
# include "TcpIp_IpV4_Cbk.h"   /* needed to forward the RxIndication */
# include "TcpIp_Arp.h"
# include "TcpIp_Arp_Cbk.h"    /* needed to forward the RxIndication */
#endif

#if (TCPIP_SUPPORT_IPSEC == STD_ON)
# include "TcpIp_IpSec.h"
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
# include "TcpIp_IpV6.h"
# include "TcpIp_IpV6_Cbk.h"   /* needed to forward the RxIndication */
# include "TcpIp_IcmpV6.h"
# include "TcpIp_Ndp.h"
#endif

#include "TcpIp_Udp.h"

#if (TCPIP_SUPPORT_TCP == STD_ON)
# include "TcpIp_Tcp.h"
#endif

#if (TCPIP_SUPPORT_DHCPV4 == STD_ON)
# include "TcpIp_DhcpV4.h"
#endif

#if (TCPIP_SUPPORT_DHCPV6 == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
# include "TcpIp_DhcpV6.h"
#endif

#if (TCPIP_SUPPORT_DHCPV4SERVER == STD_ON)                                                                              /* COV_MSR_UNSUPPORTED */
# include "TcpIp_DhcpV4Server.h"
#endif

#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
# include "TcpIp_Tls.h"
# if(TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON)
#  include "TcpIp_Tls12Core.h"
# endif
# include "TcpIp_TlsCoreCommon.h"
# if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
#  include "TcpIp_TlsServer.h"
#  include "TcpIp_TlsPriv.h"
# endif
# if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
#  include "TcpIp_TlsClientCommon.h"
# endif
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of TcpIp.h file */
#if (  (TCPIP_SW_MAJOR_VERSION != 17u) \
    || (TCPIP_SW_MINOR_VERSION != 3u) \
    || (TCPIP_SW_PATCH_VERSION != 1u) )
# error "TcpIp.c: Source and Header file are inconsistent!"
#endif

/* Check configuration variant for building the library */
#if ( (defined V_EXTENDED_BUILD_LIB_CHECK) && (TCPIP_CONFIGURATION_VARIANT_PRECOMPILE == TCPIP_CONFIGURATION_VARIANT) )
# error "TcpIp.c: Switch configuration variant to link-time or post-build for library build!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct
{
  boolean IsIpV4;
  boolean IsIpV6;
  boolean IsUdp;
  boolean IsTcp;
  boolean IsTls;
} TcpIp_ParameterType;

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
#define TCPIP_START_SEC_VAR_NO_INIT_32
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
# if (TCPIP_SUPPORT_IPSEC == STD_ON)
STATIC VAR(uint32, TCPIP_VAR_NO_INIT) TcpIp_IpSecSaEntryPairCnt;                                                        /* PRQA S 3218 */ /* MD_TCPIP_Rule8.9_3218 */
# endif /* (TCPIP_SUPPORT_IPSEC == STD_ON) */
#endif /* (TCPIP_SUPPORT_MEASUREMEN== STD_ON) */

#define TCPIP_STOP_SEC_VAR_NO_INIT_32
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define TCPIP_START_SEC_VAR_CLEARED_8
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

VAR(TcpIp_StateType, TCPIP_VAR_CLEARED) TcpIp_State = TCPIP_STATE_UNINIT;

#define TCPIP_STOP_SEC_VAR_CLEARED_8
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  TcpIp_VSocketAccessExclusive()
 *********************************************************************************************************************/
/*! \brief          Checks if another TX request is currently processed for a socket.
 *                  If not, the socket will be locked for other TX requests.
 *  \details        -
 *  \param[in]      SocketIdx             Index of a UDP or TCP socket.
 *                                        \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \return         TRUE                  Socket has been locked and can be used for transmit.
 *  \return         FALSE                 Socket is already locked and must not be used for transmit.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different SocketIdx
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(boolean, TCPIP_CODE) TcpIp_VSocketAccessExclusive(TcpIp_SizeOfSocketDynType SocketIdx);

/**********************************************************************************************************************
 *  TcpIp_UdpTransmitInternalVerify()
 *********************************************************************************************************************/
/*! \brief          Verifies the socket state and triggers transmission of the packet if possible.
 *  \details        -
 *  \param[in]      SocketUdpIdx        Index of a UDP socket.
 *                                      \spec requires SocketUdpIdx < TcpIp_GetSizeOfSocketUdpDyn(); \endspec
 *  \param[in]      RemoteSockAddrPtr   Pointer to remote socket address (IPv4/IPv6 address and port).
 *  \param[in]      DataPtr             Pointer to payload of the UDP packet.
 *                                      May be NULL_PTR if indirect data provision is used. (see TcpIp_UdpTransmit()).
 *  \param[in]      DataLen             Length of the UDP payload. [bytes]
 *  \return         E_OK                Datagram successfully passed to lower layer for transmission.
 *  \return         E_NOT_OK            Socket in invalid state or transmit was rejected by UDP submodule.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different SocketUdpIdx
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_UdpTransmitInternalVerify(
  TcpIp_SizeOfSocketUdpDynType      SocketUdpIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RemoteSockAddrPtr,
  TCPIP_P2C(uint8)                  DataPtr,
  uint16                            DataLen);

/**********************************************************************************************************************
 *  TcpIp_UdpTransmitInternal()
 *********************************************************************************************************************/
/*! \brief          Copies the data to a UDP tx buffer and triggers trnasmission of the packet.
 *  \details        -
 *  \param[in]      SocketUdpIdx        Index of a UDP socket.
 *                                      \spec requires SocketUdpIdx < TcpIp_GetSizeOfSocketUdpDyn(); \endspec
 *  \param[in]      RemoteSockAddrPtr   Remote socket address (IPv4/IPv6 address and port).
 *  \param[in]      DataPtr             Payload of the UDP packet.
 *                                      May be NULL_PTR if indirect data provision is used. (see TcpIp_UdpTransmit()).
 *  \param[in]      DataLen             Length of the UDP payload. [bytes]
 *  \return         E_OK                Datagram successfully passed to lower layer for transmission.
 *  \return         E_NOT_OK            Transmit was rejected by UDP submodule.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different SocketUdpIdx
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_UdpTransmitInternal(
  TcpIp_SizeOfSocketUdpDynType      SocketUdpIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RemoteSockAddrPtr,
  TCPIP_P2C(uint8)                  DataPtr,
  uint16                            DataLen);

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_TcpConnectInternal()
 *********************************************************************************************************************/
/*! \brief          Initiates active open of a TCP connection to the specified remote address.
 *  \details        -
 *  \param[in]      SocketTcpIdx        Index of a TCP socket.
 *                                      \spec requires SocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn(); \endspec
 *  \param[in]      RemoteAddrPtr       Remote socket address (IPv4/IPv6 address and port).
 *  \return         E_OK                The request has been accepted.
 *  \return         E_NOT_OK            The request has not been accepted.
 *  \pre            SocketIdx was allocated by <Up>_GetSocket().
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different SocketTcpIdx
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpConnectInternal(
  TcpIp_SizeOfSocketTcpDynType      SocketTcpIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RemoteAddrPtr);
#endif

/**********************************************************************************************************************
 *  TcpIp_VBindSocketImplicit()
 *********************************************************************************************************************/
/*! \brief         Tries to bind a socket to the local address if the socket is not bound already.
 *  \details       -
 *  \param[in]     SocketIdx   Index of a UDP or TCP socket.
 *                             \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \return        E_OK        Socket successfully bound or was already bound.
 *  \return        E_NOT_OK    Socket could not be bound.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE for different SocketIdx
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VBindSocketImplicit(TcpIp_SizeOfSocketDynType SocketIdx);

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VCloseTcpSocket()
 *********************************************************************************************************************/
/*! \brief         Requests to close the given TCP socket connection and releases all related resources.
 *  \details       Performs the following actions for the socket specified by SocketTcpIdx in case it is a TCP socket:
 *                 (a) if the connection is active and
 *                 (a1) Abort = FALSE: the connection is terminated after performing a regular connection termination handshake and releasing all related resources.
 *                 (a2) Abort = TRUE: connection will immediately be terminated by sending a RST segment and releasing all related resources.
 *                 (b) if the socket is in the Listen state, the Listen state is left immediately and related resources are released.
 *  \param[in]     SocketTcpIdx  Index of a TCP socket.
 *                 \spec requires SocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn(); \endspec
 *  \param[in]     Abort    TRUE: connection will immediately be terminated by sending a RST-Segment and releasing
 *                          all related resources.
 *                          FALSE: connection will be terminated after performing a regular connection termination
 *                          handshake and releasing all related resources.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE for different SocketTcpIdx.
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_VCloseTcpSocket(
  TcpIp_SizeOfSocketTcpDynType SocketTcpIdx,
  boolean                      Abort);
#endif /* TCPIP_SUPPORT_TCP */

/**********************************************************************************************************************
 *  TcpIp_BindInternal()
 *********************************************************************************************************************/
/*! \brief          Binds a UDP or TCP socket to a local resource
 *  \details        Checks whether the socket may be bound, using the given parameters, and does the actual binding.
 *  \param[in]      SocketIdx           Index of a UDP or TCP socket.
 *                                      \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec.
 *  \param[in]      LocalAddrIdx        Refer to description of this parameter at function TcpIp_Bind().
 *                                      \spec requires LocalAddrIdx < TcpIp_GetSizeOfLocalAddr() || LocalAddrIdx == TCPIP_LOCALADDRID_ANY; \endspec.
 *  \param[in,out]  PortPtr             Refer to description of this parameter at function TcpIp_Bind().
 *  \param[out]     ErrorIdPtr          TCPIP_E_NO_ERROR    In case the binding was conducted.
 *                                      TCPIP_E_ADDRINUSE   In case a socket with the requested protocol, port and
 *                                                          IpAddrId has already been requested, earlier.
 *  \return         E_OK                The request has been accepted.
 *  \return         E_NOT_OK            The request has not been accepted (e.g. address in use).
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different SocketIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_BindInternal(
  TcpIp_SizeOfSocketDynType SocketIdx,
  TcpIp_SizeOfLocalAddrType LocalAddrIdx,
  TCPIP_P2V(uint16)         PortPtr,
  TCPIP_P2V(uint8)          ErrorIdPtr);

/**********************************************************************************************************************
 *  TcpIp_RequestIpAddrAssignmentInternal()
 *********************************************************************************************************************/
/*! \brief          Initiates the local IP address assignment for the IP address specified by LocalAddrIdx.
 *  \details        -
 *  \param[in]      LocalAddrIdx       IP address index specifying the IP address for which an
 *                                     assignment shall be initiated.
 *                                     \spec requires LocalAddrIdx < TcpIp_GetSizeOfLocalAddr(); \endspec
 *  \param[in]      Type               Type of IP address assignment which shall be initiated
 *  \param[in]      LocalIpAddrPtr     Pointer to structure containing the IP address which shall be
 *                                     assigned to the EthIf controller indirectly specified via
 *                                     LocalAddrIdx.
 *                                     May be NULL_PTR if ip address shall not be set.
 *  \param[in]      Netmask            Network mask of IPv4 address or address prefix of IPv6 address in CIDR Notation.
 *  \param[in]      DefaultRouterPtr   Pointer to struct where the IP address of the default router
 *                                     (gateway) is stored (struct member 'port' is not used and of
 *                                     arbitrary value) (IPv4 only)
 *                                     May be NULL_PTR if default router shall not be set.
 *  \return         E_OK               The request has been accepted.
 *  \return         E_NOT_OK           The request has not been accepted.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different LocalAddrIdx.
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestIpAddrAssignmentInternal(
  TcpIp_SizeOfLocalAddrType         LocalAddrIdx,
  TcpIp_IpAddrAssignmentType        Type,
  TCPIP_P2C(TcpIp_SockAddrBaseType) LocalIpAddrPtr,
  uint8                             Netmask,
  TCPIP_P2C(TcpIp_SockAddrBaseType) DefaultRouterPtr);

/**********************************************************************************************************************
 *  TcpIp_VGetSocketForOwner()
 *********************************************************************************************************************/
/*! \brief          Allocates a TCP/UDP socket for use in the specified domain (IPv4/IPv6).
 *  \details        -
 *  \param[in]      Domain             Domain / address family that will be used with the socket.
 *                                     [TCPIP_AF_INET (IPv4) or TCPIP_AF_INET6 (IPv6)]
 *  \param[in]      Protocol           Transport layer protocol that will be used with the socket.
 *                                     [TCPIP_IPPROTO_TCP or TCPIP_IPPROTO_UDP]
 *  \param[out]     SocketIdPtr        Socket identifier of the allocated socket.
 *                                     Only valid if E_OK is returned.
 *  \param[in]      SocketOwnerIdx     Index of the socket owner for which the socket shall be allocated.
 *                                     \spec requires SocketOwnerIdx < TcpIp_GetSizeOfSocketOwnerConfig(); \endspec
 *  \return         E_OK               Socket was allocated (*SocketIdPtr updated).
 *  \return         E_NOT_OK           The socket could not be allocated.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different SocketOwnerIdx.
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VGetSocketForOwner(
  TcpIp_DomainType                Domain,
  TcpIp_ProtocolType              Protocol,
  TCPIP_P2V(TcpIp_SocketIdType)   SocketIdPtr,
  TcpIp_SocketOwnerConfigIterType SocketOwnerIdx);

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VTcpListenSockStateListen()
 *********************************************************************************************************************/
/*! \brief          Handles the listen action for a socket that is in state 'listen', already.
 *  \details        -
 *  \param[in]      SocketTcpIdx       Index of a TCP socket.
 *                                     \spec requires SocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn(); \endspec
 *  \param[in]      MaxChannels        Maximum number of new parallel connections established on this listen connection.
 *  \return         E_OK               The request has been accepted.
 *  \return         E_NOT_OK           The request has not been accepted.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different SocketTcpIdx.
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VTcpListenSockStateListen(
  TcpIp_SocketTcpDynIterType SocketTcpIdx,
  uint16                     MaxChannels);
#endif

#if (TCPIP_SUPPORT_DIAG_ACCESS_EXTENSIONS == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_DiagDataReadAccessInternal()
 *********************************************************************************************************************/
/*! \brief          Reads miscellaneous TcpIp related diagnostic data.
 *  \details        The memory fragment DataPtr points to should be aligned properly regarding the expected returned
 *                  type / struct. Data is only written if RetValue is E_OK.
 *                  This is a Vector extension.
 *  \param[in]      SocketIdx      Index of a UDP or TCP socket.
 *                                 \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in]      DataID         Valid and checked data identifier.
 *  \param[out]     DataPtr        Valid and checked pointer for diagnostic data.
 *                                 \spec requires DataPtr != NULL_PTR; \endspec
 *                                 \spec requires $lengthOf(DataPtr) >= *DataLenPtr; \endspec
 *  \param[in,out]  DataLenPtr     Valid and checked pointer for maximum / actual length of diagnostic data in bytes
 *  \return         E_OK           Diagnostic data has been written to the DataPtr and DataLenPtr parameter.
 *  \return         E_NOT_OK       Invalid parameter. (data identifier not found, NULL_PTR parameter, invalid length)
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different SocketIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DiagDataReadAccessInternal(
  TcpIp_SizeOfSocketDynType SocketIdx,
  TcpIp_DiagParamsType      DataID,
  TCPIP_P2V(uint8)          DataPtr,
  TCPIP_P2V(uint16)         DataLenPtr);
#endif

/**********************************************************************************************************************
 *  TcpIp_GetParameterType()
 *********************************************************************************************************************/
/*! \brief          Get the protocol type(s) that are responsible for the given ParameterId
 *  \details        The function sets the flags IsUdp, IsTls, IsTcp, IsIpV4 and IsIpV6 of the returned object to true
 *                  depending on the ParameterId.
 *  \pre            -
 *  \param[in]      ParameterId         Id of the Parameter
 *  \return         ParameterType       Object with the flags IsUdp, IsTls, IsTcp, IsIpV4 and IsIpV6
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(TcpIp_ParameterType, TCPIP_CODE) TcpIp_GetParameterType(
  TcpIp_ParamIdType              ParameterId);

#if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VGetAndResetMeasurementData()
 *********************************************************************************************************************/
/*! \brief          Read the corresponding measurement data counter and optionally reset it.
 *  \details        -
 *  \param[in]      MeasurementIdx         Index of a measurement data.
 *  \param[in]      MeasurementResetNeeded Flag for the measurement data to reset.
 *                                         TRUE  - Reset the measurement data.
 *                                         FALSE - Do not reset.
 *  \param[out]     MeasurementDataPtr     Returns the value of the measurement data count.
 *                                         CONSTRAINT: [May be NULL_PTR in combination with
 *                                                      MeasurementResetNeeded == TRUE.]
 *  \return         E_OK                   Measurement data is read and optionally reset successfully.
 *  \return         E_NOT_OK               Read or Reset is not successful.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VGetAndResetMeasurementData(
  TcpIp_MeasurementIdxType  MeasurementIdx,
  boolean                   MeasurementResetNeeded,
  TCPIP_P2V(uint32)         MeasurementDataPtr);

/**********************************************************************************************************************
 *  TcpIp_VResetSpecificMeasurementData()
 *********************************************************************************************************************/
/*! \brief          Reset the corresponding measurement data counter.
 *  \details        -
 *  \param[in]      MeasurementIdx         Index of a measurement data.
 *  \return         E_OK                   Measurement data is reset successfully.
 *  \return         E_NOT_OK               Reset is not successful.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VResetSpecificMeasurementData(
  TcpIp_MeasurementIdxType  MeasurementIdx);

/**********************************************************************************************************************
 *  TcpIp_VResetMeasurementData()
 **********************************************************************************************************************/
/*! \brief          Reset all measurement data counters for dropped TCP/UDP/IPV4/IPV6 packets.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_VResetMeasurementData(void);
#endif

#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_MultiPartitionMainFunctionRx()
 *********************************************************************************************************************/
/*! \brief          Handles reception of data and forwards it to the socket owners.
 *  \details        -
 *  \param[in]      ApplId         Application Id of the invoking application
 *  \pre            -
 *  \context        TASK (Called by TcpIp_MultiPartitionMainFunction() which is called cyclically by schedule manager)
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MultiPartitionMainFunctionRx(TcpIp_OsApplicationType ApplId);

/**********************************************************************************************************************
 *  TcpIp_MultiPartitionMainFunctionState()
 *********************************************************************************************************************/
/*! \brief          Performs pending state changes of the TcpIp module and the TCP/UDP sockets.
 *  \details        -
 *  \param[in]      ApplId         Application Id of the invoking application
 *  \pre            -
 *  \context        TASK (Called by TcpIp_MultiPartitionMainFunction() which is called cyclically by schedule manager)
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MultiPartitionMainFunctionState(TcpIp_OsApplicationType ApplId);

/**********************************************************************************************************************
 *  TcpIp_MultiPartitionMainFunctionTx()
 *********************************************************************************************************************/
/*! \brief          Queues data from the socket owners and triggers transmission of data at the lower layer.
 *  \details        -
 *  \param[in]      ApplId         Application Id of the invoking application
 *  \pre            -
 *  \context        TASK (Called by TcpIp_MultiPartitionMainFunction() which is called cyclically by schedule manager)
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MultiPartitionMainFunctionTx(TcpIp_OsApplicationType ApplId);
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TcpIp_InitMemory()
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
FUNC(void, TCPIP_CODE) TcpIp_InitMemory( void )
{
  /* #10 Set the TcpIp initialization state to 'not initialized'. */
  TcpIp_State = TCPIP_STATE_UNINIT;

#if (TCPIP_SUPPORT_DHCPV4SERVER == STD_ON)                                                                              /* COV_MSR_UNSUPPORTED */
  /* #20 Call the InitMemory function of the submodule DhcpV4Server in case it is configured. */
  TcpIp_DhcpV4Server_InitMemory();
#endif

#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
  /* #30 Call the InitMemory function of the submodule IpV4 in case it is configured. */
  IpV4_InitMemory();
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
  /* #40 Call the InitMemory function of the submodule IpV6 in case it is configured. */
  IpV6_InitMemory();
#endif

#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
  /* #50 Call the InitMemory function of the submodule Tls in case it is configured. */
  TcpIp_Tls_InitMemory();
#endif

  /* #60 Reset configuration pointer. */
  TcpIp_ConfigDataPtr = NULL_PTR;
}

/**********************************************************************************************************************
 *  TcpIp_Init()
 *********************************************************************************************************************/
/*! Internal comment removed.
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
 * */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_Init(
  TCPIP_P2C(TcpIp_ConfigType) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = TCPIP_E_NO_ERROR;
  TcpIp_EthIfCtrlDynIterType ethIfCtrlIdx;

  /* ----- Development Error Checks ------------------------------------- */
#if(TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                   /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check plausibility of input parameters. */
  if(ConfigPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Initialize configuration pointer. */
    TcpIp_ConfigDataPtr = ConfigPtr;

    /* #30 Initialize the submodules (IPv4, IPv6, TCP, UDP), if configured. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
    IpV4_Init();
#endif

#if (TCPIP_SUPPORT_IPSEC == STD_ON)
    TcpIp_IpSec_Init();
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
    IpV6_Init();
#endif

#if (TCPIP_SUPPORT_TCP == STD_ON)
    TcpIp_Tcp_Init();
#endif

#if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
    TcpIp_VResetMeasurementData();
#endif

    TcpIp_Udp_Init();

    /* #40 Initialize socket-related data structures. */
    TcpIp_VInitSockets();

    /* #50 Initialize address-related data structures. */
    {
      TcpIp_LocalAddrIterType localAddrIdx;

      for(localAddrIdx = 0; localAddrIdx < TcpIp_GetSizeOfLocalAddr(); localAddrIdx++)
      {
        TcpIp_SetAssignmentStateOfLocalAddr(localAddrIdx, TCPIP_IPADDR_STATE_UNASSIGNED);
      }
    }

    /* #60 Initialize EthIfCtrl data structures. */
    for (ethIfCtrlIdx = 0; ethIfCtrlIdx < TcpIp_GetSizeOfEthIfCtrlDyn(); ethIfCtrlIdx++)
    {
      TcpIp_SetStateOfEthIfCtrlDyn(ethIfCtrlIdx, TCPIP_STATE_OFFLINE);
    }

    /* #70 Initialize the DHCPv4 server submodule, if configured. */
#if (TCPIP_SUPPORT_DHCPV4SERVER == STD_ON)                                                                              /* COV_MSR_UNSUPPORTED */
    TcpIp_DhcpV4Server_Init();
#endif

#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
    /* #80 Initialize the TLS submodule, if configured. */
    TcpIp_Tls_Init();
#endif

    /* #90 Set module state to INIT. */
    TcpIp_State = TCPIP_STATE_INIT;
  }

  /* ----- Development Error Report --------------------------------------- */
#if(TCPIP_DEV_ERROR_REPORT == STD_ON)
  /* #100 Report default errors if any occurred. */
  if(errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_INIT, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
} /* TcpIp_Init() */


/**********************************************************************************************************************
 *  TcpIp_RxIndication()
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
/* PRQA S 6060, 6080 1 */ /* MD_TcpIp_AutosarApi, MD_MSR_STMIF */
FUNC(void, TCPIP_CODE) TcpIp_RxIndication(
  uint8            CtrlIdx,
  Eth_FrameType    FrameType,
  boolean          IsBroadcast,
  TCPIP_P2V(uint8) PhysAddrPtr,
  TCPIP_P2V(uint8) DataPtr,
  uint16           LenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'PhysAddrPtr' */
  else if (PhysAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'LenByte' */
  else if (LenByte == 0u)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check of parameter 'CtrlIdx' is done by the RxIndication functions of the sub-modules! */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Call sub-module-specific RxIndication functions depending on the FrameType. */
    switch(FrameType)
    {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
    /* #30 In case IpV4 is configured, call the IpV4-specific RxIndication function for FrameType IpV4. */
    case TCPIP_ETH_FRAME_TYPE_IPV4:
    {
      IpV4_Ip_RxIndication(CtrlIdx, FrameType, IsBroadcast, PhysAddrPtr, DataPtr, LenByte);
      break;
    }

    /* #40 In case IpV4 is configured, call the IpV4/ARP-specific RxIndication function for FrameType IpV4/ARP. */
    case TCPIP_ETH_FRAME_TYPE_ARP:
    {
      IpV4_Arp_RxIndication(CtrlIdx, FrameType, IsBroadcast, PhysAddrPtr, DataPtr, LenByte);
      break;
    }
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
    /* #50 In case IpV6 is configured, call the IpV6-specific RxIndication function for FrameType IpV6. */
    case TCPIP_ETH_FRAME_TYPE_IPV6:
    {
      IpV6_RxIndication(CtrlIdx, FrameType, IsBroadcast, PhysAddrPtr, DataPtr, LenByte);
      break;
    }
#endif
    default:
      /* TcpIp called for unsupported FrameType. -> ignore this message */
      errorId = TCPIP_E_INV_ARG;
      break;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_RXINDICATION, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
} /* TcpIp_RxIndication() */

/**********************************************************************************************************************
 *  TcpIp_GetSocketForUser()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetSocketForUser(
  TcpIp_DomainType                Domain,
  TcpIp_ProtocolType              Protocol,
  TCPIP_P2V(TcpIp_SocketIdType)   SocketIdPtr,
  TcpIp_SocketOwnerConfigIterType SocketOwnerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'Domain' */
  else if (!TCPIP_COND_VALID_DOMAIN(Domain))
  {
    errorId = TCPIP_E_AFNOSUPPORT;
  }
  /* Parameter 'Protocol' is checked below. */
  /* Check parameter 'SocketIdPtr' */
  else if (SocketIdPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'SocketOwnerIdx' */
  else if (SocketOwnerIdx >= TcpIp_GetSizeOfSocketOwnerConfig())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
  /* ----- Implementation ----------------------------------------------- */
#endif
  /* #20 Check the protocol to be supported. */
  if(    (TCPIP_IPPROTO_UDP == Protocol)
#if (TCPIP_SUPPORT_TCP == STD_ON)
      || (TCPIP_IPPROTO_TCP == Protocol)
#endif
    )
  {
    /* #30 Forward the request to the internal helper function. */
    retVal = TcpIp_VGetSocketForOwner(Domain, Protocol, SocketIdPtr, SocketOwnerIdx);
  }
  else
  {
    /* MISRA: The protocol is not supported - leave retVal at value E_NOT_OK */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void) Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_SOCKET, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_GetSocketForUser() */

/**********************************************************************************************************************
 *  TcpIp_VGetSocketForOwner()
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
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VGetSocketForOwner(
  TcpIp_DomainType                Domain,
  TcpIp_ProtocolType              Protocol,
  TCPIP_P2V(TcpIp_SocketIdType)   SocketIdPtr,
  TcpIp_SocketOwnerConfigIterType SocketOwnerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  TcpIp_SizeOfSocketDynType socketIdx;
  TcpIp_SizeOfSocketDynType socketIdxStart;
  TcpIp_SizeOfSocketDynType socketIdxEnd;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Determine relevant sockets based on 'Protocol'. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  if (Protocol == TCPIP_IPPROTO_TCP)
  {
    socketIdxStart = TcpIp_GetSizeOfSocketUdpDyn();
    socketIdxEnd   = TcpIp_GetSizeOfSocketDyn();
  }
  else
#else
  TCPIP_DUMMY_STATEMENT_CONST(Protocol);                                                                                /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif
  {
    socketIdxStart = 0;
    socketIdxEnd   = TcpIp_GetSizeOfSocketUdpDyn();
  }


  /* #20 Enter exclusive area. */
  TCPIP_VENTER_MC_ALWAYS_CRITICAL_SECTION();  /* lock cross core interrupts */

  /* #30 Iterate over relevant sockets looking for a 'free' socket which is currently not assigned to a user. */
  for(socketIdx = socketIdxStart; socketIdx < socketIdxEnd; socketIdx++)
  {
    if (TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(socketIdx) == TCPIP_NO_SOCKETOWNERCONFIGIDXOFSOCKETDYN)
    {
      /* #40 Found a free socket, assign it to the SocketOwner. */
      TcpIp_SetSocketOwnerConfigIdxOfSocketDyn(socketIdx, (TcpIp_SizeOfSocketOwnerConfigType)SocketOwnerIdx);
      break;
    }
  }

  /* #50 Exit exclusive area. */
  TCPIP_VLEAVE_MC_ALWAYS_CRITICAL_SECTION();  /* unlock cross core interrupts */

  /* #60 In case a free socket has been found, allocate the socket for this user. */
  if (socketIdx < socketIdxEnd)
  {
    /*@ assert socketIdx < TcpIp_GetSizeOfSocketDyn(); */                                                               /* VCA_TCPIP_SOCKIDX */
#if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_ON)                                                                         /* COV_TCPIP_MULTICORE_SUPPORT_NEVER_ON */
    /* Assign the socket to current application */
    TcpIp_SetApplIdOfSocketDyn(socketIdx, GetApplicationID());
#endif
    TcpIp_VPreconfigSocket(socketIdx);
    TcpIp_SetListenActiveConnStatOfSocketDyn(socketIdx, TCPIP_SOCK_CONN_ALLOCATED);

    TcpIp_GetAddrLocSockOfSocketDyn(socketIdx)->sa_family = Domain;

    /* Initialize TCP socket parameters, if socket is a TCP socket. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
    if (Protocol == TCPIP_IPPROTO_TCP)
    {
      TCPIP_ASSERT(TCPIP_SOCKET_IDX_IS_TCP(socketIdx));
      /*@ assert socketIdx >= TcpIp_GetSizeOfSocketUdpDyn() && socketIdx < TcpIp_GetSizeOfSocketDyn(); */               /* VCA_TCPIP_SOCKIDX */
      TcpIp_Tcp_InitSock(TCPIP_SOCKET_IDX_TO_TCP_IDX(socketIdx));
    }
    else
#endif
    {
      /* Initialize UDP socket parameters, if socket is a UDP socket. */
      TCPIP_ASSERT(TCPIP_SOCKET_IDX_IS_UDP(socketIdx));
      /*@ assert socketIdx < TcpIp_GetSizeOfSocketUdpDyn(); */                                                          /* VCA_TCPIP_SOCKIDX */
      TcpIp_Udp_InitSock(TCPIP_SOCKET_IDX_TO_UDP_IDX(socketIdx));
    }
    *SocketIdPtr = TCPIP_SOCKET_IDX_TO_ID(socketIdx);

    retVal = E_OK;
  }
  /* else: No free socket was found */

  return retVal;
} /* TcpIp_VGetSocketForUser() */

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_TcpListen()
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
/* PRQA S 6050, 6080, 6030 1 */ /* MD_TcpIp_STCAL_CslAccess, MD_MSR_STMIF, MD_MSR_STCYC */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpListen(
  TcpIp_SocketIdType SocketId,
  uint16             MaxChannels)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!(TCPIP_SOCKET_ID_IS_VALID(SocketId) && TCPIP_SOCKET_IDX_IS_TCP(TCPIP_SOCKET_ID_TO_IDX(SocketId))))          /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'MaxChannels' */
  else if (MaxChannels >= TcpIp_GetSizeOfSocketTcpDyn())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SocketDynIterType    socketIdx    = TCPIP_SOCKET_ID_TO_IDX(SocketId);
    TcpIp_SocketTcpDynIterType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(socketIdx);
    /*@ assert socketTcpIdx <= TcpIp_GetSizeOfSocketTcpDyn(); */                                                        /* VCA_TCPIP_TCP_IDX_FROM_CHECKED_SOCKET_IDX */

    /* #20 In case the socket is in listen state, already: Handle the request in that state. */
    if (TcpIp_GetListenActiveConnStatOfSocketDyn(socketIdx) == TCPIP_SOCK_CONN_LISTEN_ENABLED)
    {
      retVal = TcpIp_VTcpListenSockStateListen(socketTcpIdx, MaxChannels);
    }
    else
    {
      /* #30 In case the socket is not yet in listen state: Bring the socket into that state. */
      if (TcpIp_GetListenActiveConnStatOfSocketDyn(socketIdx) == TCPIP_SOCK_CONN_ALLOCATED)
      {
        /* #40 Verify that no other TCP listener with equal local endpoint is available. */
        if (   TcpIp_SearchTcpListenSocket(TcpIp_GetAddrLocSockOfSocketDyn(socketIdx), TcpIp_GetLocalAddrBindIdxOfSocketDyn(socketIdx), FALSE)
            == TcpIp_GetSizeOfSocketDyn())
        {
          /* TCP: Passive open: (TcpIp_VTcpListenSockStateAllocated) */
          TcpIp_SockAddrBaseType SockAddrRem;                                                                           /* PRQA S 0759 */ /* MD_MSR_Union */

          /* #50 Set remote address to 'undefined'. */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
          (void)IpBase_DelSockAddr(&SockAddrRem.SockAddr, IPBASE_AF_INET6);
# else
          (void)IpBase_DelSockAddr(&SockAddrRem.SockAddr, IPBASE_AF_INET);                                              /* VCA_DEL_SOCK_ADDR_CORRECT_TYPE */
# endif

          /* #60 Try to open the TCP socket in passive mode and inform the caller about the result. */
          if (TcpIp_Tcp_Open(socketTcpIdx, &SockAddrRem, TCPIP_TCP_PASSIVE_OPEN) == E_OK)
          {
            TcpIp_SetListenActiveConnStatOfSocketDyn(socketIdx, TCPIP_SOCK_CONN_LISTEN_ENABLED);
            TcpIp_SetMaxNumListenSocketsOfSocketTcpDyn(socketTcpIdx, MaxChannels);  /* set limit */

# if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON )                                                                            /* COV_MSR_UNSUPPORTED */
            retVal = TcpIp_TlsServerListener_ConfigTcpSocket(socketIdx);
# else
            retVal = E_OK;
# endif
          }
          /* else: Open failed: Leave retVal at value E_NOT_OK */
        }
        else
        {
          /* The address combination is already in use. */
          errorId = TCPIP_E_ADDRINUSE;
        }
      }
      /* else: Socket is in an invalid state - leave retVal at E_NOT_OK. */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TCP_LISTEN, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* TcpIp_Listen() */

/**********************************************************************************************************************
 *  TcpIp_VTcpListenSockStateListen()
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
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VTcpListenSockStateListen(
  TcpIp_SocketTcpDynIterType SocketTcpIdx,
  uint16                     MaxChannels)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn());

  /* #10 In case the given MaxChannels value has changed, check if more than MaxChannels sockets have been derived
         from the current socket and set the TcpIp-internal limit to the new MaxChannels value */
  if (MaxChannels != TcpIp_GetMaxNumListenSocketsOfSocketTcpDyn(SocketTcpIdx))
  {
    /* #20 In case MaxChannels is smaller than the TcpIp-internal limit iterate all sockets and count those that are related to the given SocketId */
    if(MaxChannels < TcpIp_GetMaxNumListenSocketsOfSocketTcpDyn(SocketTcpIdx))
    {
      /* Check if more than MaxChannels sockets have been derived from the current socket */
      TcpIp_SocketIdType SockCnt = 0;
      TcpIp_SocketTcpDynIterType iterSocketTcpIdx;
      for (iterSocketTcpIdx = 0; iterSocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn(); iterSocketTcpIdx++)
      {
        if(TcpIp_GetSocketTcpDynMasterListenSocketIdxOfSocketTcpDyn(iterSocketTcpIdx) == SocketTcpIdx)
        {
          SockCnt++;
        }
      }
      /* #30 In case the number of allocated sockets related to the given SocketId has increased: Set the TcpIp-internal limit to the new value */
      if(MaxChannels >= SockCnt)
      {
        TcpIp_SetMaxNumListenSocketsOfSocketTcpDyn(SocketTcpIdx, MaxChannels);
      }
      /* #40 Indicated an error in case the given MaxChannels value is larger than the counted number of related sockets */
      else
      {
        retVal = E_NOT_OK;
      }
    }
    else
    {
      /* #50 In case MaxChannels is bigger than the currently configured value, simply set the TcpIp-internal limit to the new value */
      TcpIp_SetMaxNumListenSocketsOfSocketTcpDyn(SocketTcpIdx, MaxChannels);
    }
  }
  /* else: Leave retVal at value E_OK */
  return retVal;
} /* TcpIp_VTcpListenSockStateListen() */

/**********************************************************************************************************************
 *  TcpIp_TcpConnect()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpConnect(
  TcpIp_SocketIdType            SocketId,
  TCPIP_P2C(TcpIp_SockAddrType) RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_SOCKET_ID_IS_VALID(SocketId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'RemoteAddrPtr' */
  else if (RemoteAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else if (TcpIp_VNetAddrIsUnicast((TCPIP_P2C(TcpIp_SockAddrBaseType))RemoteAddrPtr) == FALSE)                          /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfSocketDynType socketIdx = TCPIP_SOCKET_ID_TO_IDX(SocketId);

    /* Verify that socket is a TCP socket that is bound to an unicast address. */
    if (TCPIP_SOCKET_IDX_IS_TCP(socketIdx) && TcpIp_VLocalAddrIdxIsUnicast(TcpIp_GetLocalAddrBindIdxOfSocketDyn(socketIdx)))/* PRQA S 3415 1 */ /* MD_TCPIP_Rule13.5_3415_ReadOnly */
    {
      /* #20 Forward call to internal function. */
      retVal = TcpIp_TcpConnectInternal(TCPIP_SOCKET_IDX_TO_TCP_IDX(socketIdx), (TCPIP_P2C(TcpIp_SockAddrBaseType))RemoteAddrPtr); /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
    }
    else
    {
      errorId = TCPIP_E_INV_ARG;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TCP_CONNECT, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_TcpConnectInternal()
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
/* PRQA S 6050, 6010 1 */ /* MD_TcpIp_STCAL_CslAccess, MD_MSR_STPTH */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpConnectInternal(
  TcpIp_SizeOfSocketTcpDynType      SocketTcpIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType            retVal    = E_NOT_OK;
  TcpIp_SizeOfSocketDynType socketIdx = TCPIP_TCP_IDX_TO_SOCKET_IDX(SocketTcpIdx);

  /* ----- Implementation ----------------------------------------------- */

  TcpIp_SizeOfSocketOwnerConfigType socketOwnerConfigIdx = TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(socketIdx);

  /* #10 Check if a valid socket owner is available and if a 'CONNECTED' callout is configured for that user. */

  /* In the calling function it is already assured that we have a valid TCP socket index. */

  /* Check if a valid socket owner config is available */
  if (socketOwnerConfigIdx < TcpIp_GetSizeOfSocketOwnerConfig())
  {
    /* Check if a 'CONNECTED' callout is configured for that user, otherwise reject the connection request. */
    if (TcpIp_GetTcpConnectedFuncPtrOfSocketOwnerConfig(socketOwnerConfigIdx) != NULL_PTR)
    {
      /* A CONNECTED callout is configured, so proceed with the connection request. */
      retVal = E_OK;
    }
    else
    {
      /* Error, callout is not configured but is mandatory. */
      TcpIp_CallDetReportError(TCPIP_API_ID_TCP_CONNECT, TCPIP_E_CBK_REQUIRED);
    }
  }
  else
  {
    /* Error, no valid socket owner config available. TcpIp_TcpConnect has been called for a socket that has not been
    requested before. */
    TcpIp_CallDetReportError(TCPIP_API_ID_TCP_CONNECT, TCPIP_E_SOCKOWNER_INV);
  }

  /* In the further handshake processing, it can be assumed that the callout is available. */

  /* #20 Connect the socket, in case the socket is properly bound. */
  if ((retVal == E_OK) && (TcpIp_GetListenActiveConnStatOfSocketDyn(socketIdx) == TCPIP_SOCK_CONN_ALLOCATED))
  {
    /* #30 Assign a random port number in case the socket (identified by SocketId) has not yet been assigned a valid port number. */
    if (TcpIp_GetAddrLocSockOfSocketDyn(socketIdx)->DomainAndPort.port == TCPIP_PORT_NOT_SET)
    {
      TcpIp_GetAddrLocSockOfSocketDyn(socketIdx)->DomainAndPort.port = TcpIp_VGetNextDynamicPort(TCPIP_PROTOCOL_TCP);
    }

    /* #40 Verify that no TCP connection with equal local and remote endpoint is in use. */
    if (   TcpIp_SearchTcpComSocket(TcpIp_GetLocalAddrBindIdxOfSocketDyn(socketIdx),
                                       TcpIp_GetAddrLocSockOfSocketDyn(socketIdx), RemoteAddrPtr)
        == TcpIp_GetSizeOfSocketUdpDyn())
    {
      /* The IP and Port combination is already in use. */
      retVal = E_NOT_OK;
      TcpIp_CallDetReportError(TCPIP_API_ID_TCP_CONNECT, TCPIP_E_ADDRINUSE);
    }
    else
    {
# if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)                                                                             /* COV_MSR_UNSUPPORTED */
      retVal = TcpIp_TlsClient_ConfigTcpSocket(SocketTcpIdx, RemoteAddrPtr);
# endif
    }

    if (retVal == E_OK)
    {
      /* #50 Actually open the TCP connection using the given TCP socket index. */
      if (TcpIp_Tcp_Open(SocketTcpIdx, RemoteAddrPtr, TCPIP_TCP_ACTIVE_OPEN) == E_OK)
      {
        TcpIp_SetListenActiveConnStatOfSocketDyn(socketIdx, TCPIP_SOCK_CONN_ACTIVE);
        /* retVal is already set to E_OK */
      }
      else
      {
        /* Open failed. */
        retVal = E_NOT_OK;
      }
    }
  }
  else
  {
    /* The socket is not properly bound. */
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_TcpTransmit()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpTransmit(
  TcpIp_SocketIdType SocketId,
  TCPIP_P2V(uint8)   DataPtr,                                                                                           /* PRQA S 3673 */ /* MD_TcpIp_AutosarApi */
  uint32             AvailableLength,
  boolean            ForceRetrieve)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_SOCKET_ID_IS_VALID(SocketId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'AvailableLength' */
  else if (AvailableLength > 0xFFFFu)
  {
    /* This implementation is limited to 64k per transmit. */
    errorId = TCPIP_E_INV_ARG;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check for state OnHold. */
    TcpIp_SizeOfSocketDynType socketIdx = TCPIP_SOCKET_ID_TO_IDX(SocketId);
    /*@ assert socketIdx < TcpIp_GetSizeOfSocketDyn(); */                                                               /* VCA_TCPIP_SOCKETIDX_FROM_CHECKED_SOCKETID */

    if (TcpIp_SocketIsOnHold(SocketId) == FALSE)
    {
      /* #30 Check if there is any data to transmit. */
      if (AvailableLength > 0u)
      {
        if (TCPIP_SOCKET_IDX_IS_TCP(socketIdx))
        {
# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
          TcpIp_SizeOfSocketTcpDynType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(socketIdx);

          /* #40 Forward call to Tls, if Tls is used for the socket. */
          if (TcpIp_IsUseTlsOfSocketTcpDyn(socketTcpIdx) == TRUE)
          {
            retVal = TcpIp_TlsCoreUl_Transmit(socketIdx, DataPtr, AvailableLength, ForceRetrieve);
          }
          else
# endif
            /* #50 Otherwise: Forward call to internal function. */
          {
            retVal = TcpIp_TcpTransmitInternal(socketIdx, DataPtr, AvailableLength, ForceRetrieve);
          }
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TCP_TRANSMIT, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
}
#endif /* (TCPIP_SUPPORT_TCP == STD_ON) */

/**********************************************************************************************************************
 *  TcpIp_VBindSocketImplicit()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VBindSocketImplicit(TcpIp_SizeOfSocketDynType SocketIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if socket is already bound. (local port is set to 0 for unbound sockets) */
  if (TcpIp_GetAddrLocSockOfSocketDyn(SocketIdx)->DomainAndPort.port != TCPIP_PORT_NOT_SET)
  {
    retVal = E_OK;
  }
  /* #20 In case the socket is unbound: Try to bind it implicitly to a dynamic port. */
  /* \trace SPEC-50982 */
  else
  {
    uint8 errorId;
    TcpIp_SizeOfLocalAddrType localAddrBindIdx = TcpIp_GetLocalAddrBindIdxOfSocketDyn(SocketIdx);
    /*@ assert localAddrBindIdx < TcpIp_GetSizeOfLocalAddr() || localAddrBindIdx == TCPIP_LOCALADDRID_ANY; */           /* VCA_TCPIP_BIND_IMPLICIT */
    retVal = TcpIp_BindInternal(SocketIdx, localAddrBindIdx,
      &TcpIp_GetAddrLocSockOfSocketDyn(SocketIdx)->DomainAndPort.port, &errorId);
  }

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_UdpTransmit()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_UdpTransmit(
  TcpIp_SocketIdType            SocketId,
  TCPIP_P2V(uint8)              DataPtr,                                                                                /* PRQA S 3673 */ /* MD_TcpIp_AutosarApi */
  TCPIP_P2V(TcpIp_SockAddrType) RemoteAddrPtr,                                                                          /* PRQA S 3673 */ /* MD_TcpIp_AutosarApi */
  uint16                        TotalLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* #20 Check parameter 'SocketId' */
  else if (TCPIP_SOCKET_ID_TO_IDX(SocketId) >= TcpIp_GetSizeOfSocketUdpDyn())
  {
    /* Passed socket ID does not represent a UDP socket. */
    errorId = TCPIP_E_INV_ARG;
  }
  /* #30 Check parameter 'RemoteAddrPtr' */
  else if (RemoteAddrPtr == NULL_PTR)
  {
    /* RemoteAddrPtr not set. */
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else if (TCPIP_IS_ADDR_FAMILY_SUPPORTED(RemoteAddrPtr->domain) == FALSE)                                              /* PRQA S 1881 */ /* MD_MSR_AutosarBoolean */
  {
    /* RemoteAddrPtr has invalid address family. */
    errorId = TCPIP_E_INV_ARG;
  }
  /* #40 When all DET checks are ok */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfSocketDynType    socketIdx = TCPIP_SOCKET_ID_TO_IDX(SocketId);

    /* #50 Check for state OnHold. */
    if (TcpIp_SocketIsOnHold(SocketId) == FALSE)
    {
      TcpIp_SizeOfSocketUdpDynType socketUdpIdx = TCPIP_SOCKET_IDX_TO_UDP_IDX(socketIdx);
      /*@ assert socketUdpIdx < TcpIp_GetSizeOfSocketUdpDyn(); */                                                       /* VCA_TCPIP_UDP_IDX_FROM_CHECKED_SOCKET_IDX */

      /* #60 Forward call to TcpIp_UdpTransmitInternalVerify() */
      retVal = TcpIp_UdpTransmitInternalVerify(socketUdpIdx, (TCPIP_P2C(TcpIp_SockAddrBaseType))RemoteAddrPtr, DataPtr, TotalLength); /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_UDP_TRANSMIT, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_UdpTransmit() */

/**********************************************************************************************************************
 *  TcpIp_UdpTransmitInternalVerify()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_UdpTransmitInternalVerify(
  TcpIp_SizeOfSocketUdpDynType      SocketUdpIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RemoteSockAddrPtr,
  TCPIP_P2C(uint8)                  DataPtr,
  uint16                            DataLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  TcpIp_SizeOfSocketDynType socketIdx = TCPIP_UDP_IDX_TO_SOCKET_IDX(SocketUdpIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that socket is allocated. */
  if (TcpIp_GetListenActiveConnStatOfSocketDyn(socketIdx) != TCPIP_SOCK_CONN_NONE)
  {
    /* #20 Try to bind to socket implicitly, if socket is unbound. */
    if (TcpIp_VBindSocketImplicit(socketIdx) == E_OK)
    {
      /* #30 Acquire semaphore for the socket in order to avoid simultaneous transmits over the same socket. */
      if (TcpIp_VSocketAccessExclusive(socketIdx) == TRUE)
      {
        /* #40 Try to send requested UDP datagram. */
        retVal = TcpIp_UdpTransmitInternal(SocketUdpIdx, RemoteSockAddrPtr, DataPtr, DataLen);

        /* #50 Release semaphore for the socket. */
        TcpIp_SetTxBufRequestedOfSocketDyn(socketIdx, FALSE);
      }
    }
  }

  return retVal;
} /* TcpIp_UdpTransmitInternalVerify() */

/**********************************************************************************************************************
 *  TcpIp_UdpTransmitInternal()
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
/* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_TcpIp_STCAL_CslAccess, MD_MSR_STMIF */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_UdpTransmitInternal(
  TcpIp_SizeOfSocketUdpDynType      SocketUdpIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RemoteSockAddrPtr,
  TCPIP_P2C(uint8)                  DataPtr,
  uint16                            DataLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType    retVal = E_NOT_OK;
  uint16            dataLenRemain = DataLen;
  uint16            dataLenLocal  = DataLen;
  uint16            udpPayloadBufferLen = dataLenRemain;
  BufReq_ReturnType udpBufReqRetVal;
  TCPIP_P2V(uint8)  udpPayloadPtr;
  TCPIP_P2V(TcpIp_IpTxRequestDescriptorType) txRequestDescPtr = TcpIp_GetAddrIpTxRequestDescriptorOfSocketUdpDyn(
                                                                 SocketUdpIdx);
  TCPIP_ASSERT(SocketUdpIdx < TcpIp_GetSizeOfSocketUdpDyn());

  /* ----- Implementation ----------------------------------------------- */
  /* Store if for this sockets the UDP checksum can be ignored */
  txRequestDescPtr->SkipUdpChecksum = (TcpIp_IsUdpChecksumOfSocketUdpDyn(SocketUdpIdx) == FALSE);

  /* #10 Request TX buffer for UDP packet from lower layer.
    *     Multiple buffer segments may be provided by the IP if fragmentation is required. */
    /* If DataLen exceeds the payload that fits into one IP packet TcpIp_Udp_RequestTxBuffer() may only provide a buffer for the first payload fragment.
    * Additional buffers for the other payload fragments are obtained by TcpIp_Udp_FragmentRequestTxBuffer() later.
    */
  udpBufReqRetVal = TcpIp_Udp_RequestTxBuffer(SocketUdpIdx, RemoteSockAddrPtr, &udpPayloadPtr, &udpPayloadBufferLen);

  if (udpBufReqRetVal == BUFREQ_OK)
  {
    /*@ assert $external(udpPayloadPtr) && ($lengthOf(udpPayloadPtr) >= udpPayloadBufferLen); */                        /* VCA_TCPIP_IP_BUFFER_PTR_FOR_UDP */
    boolean copyDataSuccessful = TRUE;
    uint16  dataPos = 0;

    /* a buffer could be provided successfully */
    /* #20 Iterate over all provided UDP payload buffer fragments and copy upper layer data. */
    while (dataLenRemain > 0u)
    {
      /*@ assert $external(udpPayloadPtr) && ($lengthOf(udpPayloadPtr) >= udpPayloadBufferLen); */                      /* VCA_TCPIP_IP_BUFFER_PTR_FOR_UDP */
      /* Provided buffer may be larger than required. */
      if (dataLenRemain < udpPayloadBufferLen)
      {
        udpPayloadBufferLen = dataLenRemain;
      }

      /* #30 Copy data from upper layer into TX buffer, if data was provided. */
      copyDataSuccessful = TcpIp_UdpTransmitCopyData(txRequestDescPtr, DataPtr, &dataLenRemain, udpPayloadPtr,
                                                      udpPayloadBufferLen, &dataPos, &dataLenLocal);

      /* #40 Request next TX buffer segment from lower layer if required. */
      if (dataLenRemain > 0u)
      {
        /* Request next UDP payload buffer fragment */
        if (E_OK == TcpIp_Udp_FragmentRequestTxBuffer(SocketUdpIdx, RemoteSockAddrPtr->sa_family, &udpPayloadPtr,
                                                      &udpPayloadBufferLen))
        {
          /*@ assert $external(udpPayloadPtr) && ($lengthOf(udpPayloadPtr) >= udpPayloadBufferLen); */                  /* VCA_TCPIP_IP_BUFFER_PTR_FOR_UDP */
        }
        else
        {
          /* ERROR: This should not happen. If TcpIp_Udp_RequestTxBuffer() returns BUFREQ_OK there should be sufficient buffer space */
          copyDataSuccessful = FALSE;
          dataLenRemain = 0;
        }
      }
    }

    /* #50 Transmit UDP datagram if payload was successfully copied into TX buffer.
     *     Otherwise: Abort transmission. */
    if (copyDataSuccessful == TRUE)
    {
      if (TcpIp_Udp_Transmit(SocketUdpIdx, dataLenLocal, RemoteSockAddrPtr) == E_OK)
      {
        retVal = E_OK;
      }
    }
    else
    {
      TcpIp_Udp_TransmitCancel(SocketUdpIdx);
    }
  }
  else
  {
    /* No Ethernet buffer available or link-layer address resolution pending and no free element in UDP retry queue. */
    /* retVal remains E_NOT_OK */
  }

  return retVal;
} /* TcpIp_UdpTransmitInternal() */

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 *  TcpIp_IcmpV6Transmit()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6060, 6080 1 */ /* MD_TcpIp_AutosarApi, MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IcmpV6Transmit(
  TcpIp_LocalAddrIdType          LocalIpAddrId,
  TCPIP_P2C(TcpIp_SockAddrType)  RemoteAddrPtr,
  uint8                          HopLimit,
  uint8                          Type,
  uint8                          Code,
  uint16                         DataLength,
  TCPIP_P2C(uint8)               DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal  = E_NOT_OK;
  uint8           errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else if (!(TCPIP_LOCAL_ADDR_IDX_IS_IPV6(TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalIpAddrId))))                                  /* PRQA S 2742, 3415 */ /* MD_TCPIP_Rule14.3_2741_2742_2991_2992_2995_2996, MD_TCPIP_Rule13.5_3415_CslReadOnly */ /*lint !e506 */
  {
    /* LocalIpAddrId invalid. */
    errorId = TCPIP_E_INV_ARG;                                                                                          /* PRQA S 2880 */ /* MD_TCPIP_Rule2.1_2880 */
  }
  else if (RemoteAddrPtr == NULL_PTR)
  {
    /* RemoteAddrPtr not set. */
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else if (RemoteAddrPtr->domain != TCPIP_AF_INET6)
  {
    /* RemoteAddr address family invalid. */
    errorId = TCPIP_E_INV_ARG;
  }
  else if (DataPtr == NULL_PTR)
  {
    /* DataPtr not set. */
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_LocalAddrIterType localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalIpAddrId);

    /* #20 Check for state OnHold. */
    if (TcpIp_LocalAddrIdxIsOnHold(localAddrIdx) == FALSE)
    {
      TcpIp_LocalAddrV6IterType ipV6AddrIdx = TCPIP_LOCAL_ADDR_IDX_TO_IPV6(localAddrIdx);

      TCPIP_P2C(TcpIp_SockAddrBaseType) remoteAddrPtr = (TCPIP_P2C(TcpIp_SockAddrBaseType))RemoteAddrPtr;               /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */

      /* #30 Forward call to ICMPv6 submodule. */
      if (IpV6_Icmp_TxRawMessage(&ipV6AddrIdx, &remoteAddrPtr->SockAddrIn6.sin6_addr, HopLimit, Type, Code, DataLength, DataPtr) == BUFREQ_OK)
      {
        retVal = E_OK;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_ICMPV6_TRANSMIT, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* TcpIp_IcmpV6Transmit() */
#endif

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_TcpReceived()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpReceived(
  TcpIp_SocketIdType SocketId,
  uint32             Length)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_SOCKET_ID_IS_VALID(SocketId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'Length' */
  else if (!TCPIP_COND_LEN_MAX_UINT16(Length))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfSocketDynType socketIdx = TCPIP_SOCKET_ID_TO_IDX(SocketId);

    /* #20 Check this TCP socket to be properly requested by a socket user. */
    if(TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(socketIdx) != TCPIP_NO_SOCKETOWNERCONFIGIDXOFSOCKETDYN)
    {
      /* #30 Dispatch the reception to TCP or TLS according to the sockets configuration. */
      if (TCPIP_SOCKET_IDX_IS_TCP(socketIdx))
      {
        TcpIp_SizeOfSocketTcpDynType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(socketIdx);
        /*@ assert socketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn(); */                                                     /* VCA_TCPIP_SOCKETIDX_FROM_CHECKED_SOCKETID */

# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
        if (TcpIp_IsUseTlsOfSocketTcpDyn(socketTcpIdx) == TRUE)
        {
          TcpIp_TlsCoreUl_Received(socketIdx, Length, &errorId);
        }
        else
# endif
        {
          /* TCP received */
          TcpIp_Tcp_Received(socketTcpIdx, (uint16)Length);
        }
        retVal = E_OK;
      }
      /* else: Invalid socket id: Leave retVal at value E_NOT_OK. */
    }
    /* else: socket is not requested by a user: Leave retVal at value E_NOT_OK. */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TCP_RECEIVED, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* TcpIp_TcpReceived() */
#endif

/**********************************************************************************************************************
 *  TcpIp_Close()
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
/* PRQA S 6050, 6080 1 */ /* MD_TcpIp_STCAL_CslAccess, MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Close(
  TcpIp_SocketIdType SocketId,
  boolean            Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;
#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
  TcpIp_TlsConnectionIterType tlsConIdx;
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else if (!TCPIP_SOCKET_ID_IS_VALID(SocketId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfSocketDynType socketIdx = TCPIP_SOCKET_ID_TO_IDX(SocketId);

    /* #20 Check if the socket state is valid. */
    if (TcpIp_GetListenActiveConnStatOfSocketDyn(socketIdx) != TCPIP_SOCK_CONN_NONE)
    {
      /* #30 Check if socket is a UDP or TCP socket. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
      if (TCPIP_SOCKET_IDX_IS_UDP(socketIdx))
#endif
      {
        /* #40 UDP: Close the socket and inform socket owner. */
        /* Temporarily store the index of the socket owner because the socket variables will be reset below before the
           socket owner is notified. */
        uint8 tmpSocketOwnerIdx = TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(socketIdx);
        /*@ assert tmpSocketOwnerIdx < TcpIp_GetSizeOfSocketOwnerConfig(); */                                           /* VCA_TCPIP_API_SOCKETOWNERCONFIG_IDX */

        TcpIp_SetSocketOwnerConfigIdxOfSocketDyn(socketIdx, TCPIP_NO_SOCKETOWNERCONFIGIDXOFSOCKETDYN);
        TcpIp_SetListenActiveConnStatOfSocketDyn(socketIdx, TCPIP_SOCK_CONN_NONE);

        TcpIp_VDelSockAddrMapping(socketIdx);
        TcpIp_Udp_CloseSocket(TCPIP_SOCKET_IDX_TO_UDP_IDX(socketIdx));

        TcpIp_TcpIpEventFunctions(tmpSocketOwnerIdx, socketIdx, TCPIP_UDP_CLOSED);  /* trigger 'CLOSED' event */

      }
#if (TCPIP_SUPPORT_TCP == STD_ON)
      else
      {
        TcpIp_SizeOfSocketTcpDynType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(socketIdx);

        /* #50 TCP: Forward call to internal function that closes the TCP socket depending on parameter 'Abort'. */
# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
        /* Check if the socket uses TLS and the TLS connections is in state active */
        if (   (TcpIp_IsUseTlsOfSocketTcpDyn(socketTcpIdx) == TRUE)                                                     /* PRQA S 3415 3 */ /* MD_TCPIP_Rule13.5_3415_ReadOnly */
            && (TcpIp_SocketIdxToTlsConIdx(socketIdx, &tlsConIdx) == E_OK)
            && (TcpIp_GetConnectionStateOfTlsConnectionDyn(tlsConIdx) != TCPIP_TLS_STATE_CONNECTION_HS_INACTIVE))
        {
          /* TLS connection active - Let the TLS process the close. */
          TcpIp_TlsCoreEvent_TcpIpEventClose(socketIdx, Abort);
        }
#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
        else if (   (TcpIp_IsUseTlsOfSocketTcpDyn(socketTcpIdx) == TRUE)                                                /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_ReadOnly */
                 && (TcpIp_GetSockStateOfSocketTcpDyn(socketTcpIdx) == TCPIP_TCP_SOCK_STATE_LISTEN))
        {
          /* TLS Listen Socket - Let the TLS process the close */
          TcpIp_TlsServerListener_Close(socketIdx, Abort);
        }
#  endif /* (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON) */
        else
          /* TLS is not used on this socket, let TCP handle the close */
# endif
        {
          TcpIp_VCloseTcpSocket(socketTcpIdx, Abort);
        }
      }
#else
      TCPIP_DUMMY_STATEMENT_CONST(Abort);                                                                               /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif
      retVal = E_OK;
    }
    /* else: Socket (already) closed. Leave retVal = E_NOT_OK. */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_CLOSE, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_Close() */

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VCloseTcpSocket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_VCloseTcpSocket(
  TcpIp_SizeOfSocketTcpDynType SocketTcpIdx,
  boolean                      Abort)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfSocketDynType         socketIdx;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn());

  socketIdx = TCPIP_TCP_IDX_TO_SOCKET_IDX(SocketTcpIdx);
  TCPIP_VENTER_MC_ALWAYS_CRITICAL_SECTION();  /* lock cross core interrupts */


  /* #10 Trigger the closing of the socket (via FIN) at the TCP, if 'Abort' is not requested. */
  if (Abort == FALSE)
  {
    TcpIp_Tcp_Close(SocketTcpIdx);
  }
  /* #20 Otherwise: Trigger the fast closing (abort) of the socket at the TCP and update the admin data to actually delete the socket. */
  else
  {
    TcpIp_Tcp_Abort(SocketTcpIdx);

    /* delete socket */
    TcpIp_SetSocketOwnerConfigIdxOfSocketDyn(socketIdx, TCPIP_NO_SOCKETOWNERCONFIGIDXOFSOCKETDYN);
    TcpIp_SetListenActiveConnStatOfSocketDyn(socketIdx, TCPIP_SOCK_CONN_NONE);
  }

  /* #30 Check, whether to inform the upper layers about the closing of the socket in case of slow closing but non-connected socket. */
  if ((Abort == FALSE) && (TcpIp_GetListenActiveConnStatOfSocketDyn(socketIdx) != TCPIP_SOCK_CONN_ACTIVE) )             /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
  {
    /* delete socket */
    TcpIp_SetSocketOwnerConfigIdxOfSocketDyn(socketIdx, TCPIP_NO_SOCKETOWNERCONFIGIDXOFSOCKETDYN);
    TcpIp_SetListenActiveConnStatOfSocketDyn(socketIdx, TCPIP_SOCK_CONN_NONE);
  }
  TCPIP_VLEAVE_MC_ALWAYS_CRITICAL_SECTION();  /* unlock cross core interrupts */

  /* #40 Actually inform upper layers about the closing of the socket, if required. */
  if (Abort == TRUE)
  {


  }
} /* TcpIp_VCloseTcpSocket() */

# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 *  TcpIp_CloseTcpSocketFromTls()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_CloseTcpSocketFromTls(
  TcpIp_SizeOfSocketTcpDynType SocketTcpIdx,
  boolean                      Abort)
{
  /* #10 Forward the Close call to the internal TCP related close function */
  TcpIp_VCloseTcpSocket(SocketTcpIdx, Abort);
} /* TcpIp_CloseTcpSocketFromTls() */
# endif /* (TCPIP_SUPPORT_ASRTLS == STD_ON) */

#endif /* TCPIP_SUPPORT_TCP */

/**********************************************************************************************************************
 *  TcpIp_Bind()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Bind(
  TcpIp_SocketIdType    SocketId,
  TcpIp_LocalAddrIdType LocalAddrId,
  TCPIP_P2V(uint16)     PortPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_SOCKET_ID_IS_VALID(SocketId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'LocalAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID_OR_ANY(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'PortPtr' */
  else if (PortPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check state of the address that is affected by the bind-request: */
  else if ((LocalAddrId != TCPIP_LOCALADDRID_ANY) && (TcpIp_GetAssignmentStateOfLocalAddr(TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId)) == TCPIP_IPADDR_STATE_UNASSIGNED))/* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
  {
    /* reject the bind request if no valid address is available for the LocalAddrId */
    errorId = TCPIP_E_ADDRNOTAVAIL;
  }
  /* #20 Forward call to internal function. */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfLocalAddrType localAddrIdx = (LocalAddrId != TCPIP_LOCALADDRID_ANY) ? TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId) : TCPIP_LOCALADDRID_ANY;
    retVal = TcpIp_BindInternal(TCPIP_SOCKET_ID_TO_IDX(SocketId), localAddrIdx, PortPtr, &errorId);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_BIND, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_Bind() */

/**********************************************************************************************************************
 *  TcpIp_BindInternal()
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
/* PRQA S 6010, 6030, 6050 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_BindInternal(
  TcpIp_SizeOfSocketDynType SocketIdx,
  TcpIp_SizeOfLocalAddrType LocalAddrIdx,
  TCPIP_P2V(uint16)         PortPtr,
  TCPIP_P2V(uint8)          ErrorIdPtr)
{
  /* [SWS_TCPIP_00146]:  TcpIp_Bind() shall check if there is another socket already bound to the same port, protocol
      and local address and if that is the case refuse the request and return E_NOT_OK. If development error detection
      is enabled, the service TcpIp_Bind() shall also raise the development error code TCPIP_E_ADDRINUSE. () \trace SPEC-66326 */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType          retVal = E_OK;
  TcpIp_SocketDynIterType socketStartIdx;
  TcpIp_SocketDynIterType socketEndIdx;
  uint8                   protocol;

  /* ----- Implementation ----------------------------------------------- */
  *ErrorIdPtr = TCPIP_E_NO_ERROR;
  /* #10 Select range of all TCP or all UDP sockets that need to be checked below. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  if (TCPIP_SOCKET_IDX_IS_UDP(SocketIdx))
#endif
  {
    protocol = TCPIP_PROTOCOL_UDP;

    socketStartIdx = 0;
    socketEndIdx   = TcpIp_GetSizeOfSocketUdpDyn();
  }
#if (TCPIP_SUPPORT_TCP == STD_ON)
  else
  {
    protocol = TCPIP_PROTOCOL_TCP;

    socketStartIdx = TcpIp_GetSizeOfSocketUdpDyn();
    socketEndIdx   = TcpIp_GetSizeOfSocketDyn();
  }
#endif

  /* #20 Check if the socketOwnerConfig is invalid. */
  if (TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(SocketIdx) == TCPIP_NO_SOCKETOWNERCONFIGIDXOFSOCKETDYN)
  {
    /* ERROR: Invalid socketOwnerIndex, TcpIp_Bind is called without TcpIp_GetSocketForUser() */
    *ErrorIdPtr = TCPIP_E_INV_ARG;
    retVal      = E_NOT_OK;
  }
  else
  {
    /* #30 Otherwise: Select an unused port from the dynamic port range if caller did not specify a port. */
    if (*PortPtr == TCPIP_PORT_NOT_SET)
    {
      *PortPtr = TcpIp_VFindUnusedDynamicPort(protocol, LocalAddrIdx, socketStartIdx, socketEndIdx, SocketIdx);

      if (*PortPtr == TCPIP_PORT_NOT_SET)
      {
        /* ERROR: Did not find a dynamic port that is not in use by another socket. */
        *ErrorIdPtr = TCPIP_E_ADDRINUSE;
        retVal = E_NOT_OK;
      }
    }
    /* #40 Otherwise: If protocol is TCP, port will be checked later. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
    else if(protocol == TCPIP_PROTOCOL_TCP)                                                                             /* PRQA S 2991, 2995 */ /* MD_TCPIP_Rule14.3_2741_2742_2991_2992_2995_2996 */
    {
      /* TCP socket will be checked for uniqueness while connect or set to listen. */
    }
#endif
    /* #50 Otherwise: If protocol is UDP, check if specified port is not already in use by another socket. */
    else
    {
      if (TcpIp_VIsPortAndAddrUsedBySocket(*PortPtr, LocalAddrIdx, socketStartIdx, socketEndIdx, SocketIdx) == TRUE)
      {
        /* ERROR: A socket with the requested protocol, port and LocalAddrId is already bound. */
        *ErrorIdPtr = TCPIP_E_ADDRINUSE;
        retVal = E_NOT_OK;
      }
    }

    /* #60 Bind the socket to the IP address and port, if no other socket uses the same port and address. */
    if (retVal == E_OK)
    {
      TcpIp_SizeOfLocalAddrType ipVxAddrIdx;

      if (LocalAddrIdx < TcpIp_GetSizeOfLocalAddr())
      {
        ipVxAddrIdx = TCPIP_LOCAL_ADDR_IDX_TO_IPVX(LocalAddrIdx);
      }
      else
      {
        ipVxAddrIdx = TCPIP_IP_ADDR_ID_ANY;
      }

      TcpIp_GetAddrLocSockOfSocketDyn(SocketIdx)->DomainAndPort.port = TCPIP_HTONS(*PortPtr);
      TcpIp_SetTxIpAddrIdxOfSocketDyn(SocketIdx, ipVxAddrIdx);
      TcpIp_SetLocalAddrBindIdxOfSocketDyn(SocketIdx, LocalAddrIdx);

      /* No special handling needed for sockets using TLS, this is done later. */

#if (TCPIP_SUPPORT_TCP == STD_ON)
      if (TCPIP_SOCKET_IDX_IS_UDP(SocketIdx))
#endif
      {
        /* Bound UDP socket is always handled as listening socket. */
        TcpIp_SetListenActiveConnStatOfSocketDyn(SocketIdx, TCPIP_SOCK_CONN_LISTEN_ENABLED);
      }
    }
  }
  return retVal;
} /* TcpIp_BindInternal() */

/**********************************************************************************************************************
 *  TcpIp_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestIpAddrAssignment(
  TcpIp_LocalAddrIdType         LocalAddrId,
  TcpIp_IpAddrAssignmentType    Type,
  TCPIP_P2V(TcpIp_SockAddrType) LocalIpAddrPtr,                                                                         /* PRQA S 3673 */ /* MD_TcpIp_AutosarApi */
  uint8                         Netmask,
  TCPIP_P2V(TcpIp_SockAddrType) DefaultRouterPtr)                                                                       /* PRQA S 3673 */ /* MD_TcpIp_AutosarApi */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check input parameters. */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else if ((Type > TCPIP_IPADDR_ASSIGNMENT_MAX_VALUE) && (Type != TCPIP_IPADDR_ASSIGNMENT_ANY))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else if ((Type == TCPIP_IPADDR_ASSIGNMENT_STATIC) && (LocalIpAddrPtr == NULL_PTR))
  {
    /* ERROR: LocalIpAddrPtr must not be NULL_PTR if assignment type is STATIC. */
    errorId = TCPIP_E_INV_ARG;
  }
  /* #20 Forward call to internal function. */
  /* Values of 'LocalIpAddrPtr', 'Netmask' and 'DefaultRouterPtr' are checked below. */
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = TcpIp_RequestIpAddrAssignmentInternal(TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId),
      Type,
      (TCPIP_P2C(TcpIp_SockAddrBaseType))LocalIpAddrPtr,                                                                /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
      Netmask,
      (TCPIP_P2C(TcpIp_SockAddrBaseType))DefaultRouterPtr);                                                             /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_REQ_IP_ADDR_ASSIGN, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
}  /* TcpIp_RequestIpAddrAssignment() */

/**********************************************************************************************************************
 *  TcpIp_RequestIpAddrAssignmentInternal()
 **********************************************************************************************************************/
/*! Internal comment removed.
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
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestIpAddrAssignmentInternal(
  TcpIp_SizeOfLocalAddrType         LocalAddrIdx,
  TcpIp_IpAddrAssignmentType        Type,
  TCPIP_P2C(TcpIp_SockAddrBaseType) LocalIpAddrPtr,
  uint8                             Netmask,
  TCPIP_P2C(TcpIp_SockAddrBaseType) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  /* #10 Dispatch the request to the affected IP subcomponent depending on the address type being IpV4 or IpV6. */
  if (TCPIP_LOCAL_ADDR_IDX_IS_IPV4(LocalAddrIdx))
# endif
  {
    /* #20 IPv4: Check if parameter 'Netmask' is valid IPv4 netmask. */
    if (Netmask <= 32u)
    {
      TcpIp_SizeOfLocalAddrV4Type ipV4AddrIdx       = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(LocalAddrIdx);
      IpBase_AddrInType           localIpAddr       = IPV4_ADDR_ANY;
      IpBase_AddrInType           defaultRouterAddr = IPV4_ADDR_ANY;

      retVal = E_OK;

      /* #30 Check if optional parameter 'LocalIpAddrPtr' is valid. */
      if (LocalIpAddrPtr != NULL_PTR)
      {
        if (LocalIpAddrPtr->sa_family == TCPIP_AF_INET)
        {
          localIpAddr = LocalIpAddrPtr->SockAddrIn.sin_addr;
        }
        else
        {
          retVal = E_NOT_OK;
          TcpIp_CallDetReportError(TCPIP_API_ID_REQ_IP_ADDR_ASSIGN, TCPIP_E_INV_ARG);
        }
      }
      /* else: Use IPV4_ADDR_ANY to indicate that localIpAddr was not specified. */

      /* #40 Check if optional parameter 'DefaultRouterPtr' is valid. */
      if (DefaultRouterPtr != NULL_PTR)
      {
        if (DefaultRouterPtr->sa_family == TCPIP_AF_INET)
        {
          defaultRouterAddr = DefaultRouterPtr->SockAddrIn.sin_addr;
        }
        else
        {
          retVal = E_NOT_OK;
          TcpIp_CallDetReportError(TCPIP_API_ID_REQ_IP_ADDR_ASSIGN, TCPIP_E_INV_ARG);
        }
      }
      /* else: Use IPV4_ADDR_ANY to indicate that defaultRouterAddr was not specified. */

      /* #50 Forward call to IpV4 submodule, if not error occurred. */
      if (retVal == E_OK)
      {
        retVal = IpV4_Ip_RequestIpAddrAssignment(ipV4AddrIdx, Type, localIpAddr, Netmask, defaultRouterAddr);
      }
    }
    else
    {
      /* Netmask is invalid. */
      TcpIp_CallDetReportError(TCPIP_API_ID_REQ_IP_ADDR_ASSIGN, TCPIP_E_INV_ARG);
    }
  }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
  else
# endif
  {
    /* #60 IPv6: Check if parameter 'Netmask' is valid IPv6 netmask. */
    if (Netmask <= 128u)
    {
      TcpIp_SizeOfLocalAddrV6Type   ipV6AddrIdx          = TCPIP_LOCAL_ADDR_IDX_TO_IPV6(LocalAddrIdx);
      TCPIP_P2C(IpBase_AddrIn6Type) localIpAddrPtr       = NULL_PTR;
      TCPIP_P2C(IpBase_AddrIn6Type) defaultRouterAddrPtr = NULL_PTR;

      retVal = E_OK;

      /* #70 Check if optional parameter 'LocalIpAddrPtr' is valid. */
      if (LocalIpAddrPtr != NULL_PTR)
      {
        if (LocalIpAddrPtr->sa_family == TCPIP_AF_INET6)
        {
          localIpAddrPtr = &LocalIpAddrPtr->SockAddrIn6.sin6_addr;
        }
        else
        {
          retVal = E_NOT_OK;
          TcpIp_CallDetReportError(TCPIP_API_ID_REQ_IP_ADDR_ASSIGN, TCPIP_E_INV_ARG);
        }
      }
      /* else: Use NULL_PTR to indicate that localIpAddr was not specified. */

      /* #80 Check if optional parameter 'DefaultRouterPtr' is valid. */
      if (DefaultRouterPtr != NULL_PTR)
      {
        if (DefaultRouterPtr->sa_family == TCPIP_AF_INET6)
        {
          defaultRouterAddrPtr = &DefaultRouterPtr->SockAddrIn6.sin6_addr;
        }
        else
        {
          retVal = E_NOT_OK;
          TcpIp_CallDetReportError(TCPIP_API_ID_REQ_IP_ADDR_ASSIGN, TCPIP_E_INV_ARG);
        }
      }
      /* else: Use NULL_PTR to indicate that defaultRouterAddr was not specified. */

      /* #90 Forward call to IpV6 submodule, if not error occurred. */
      if (retVal == E_OK)
      {
        retVal = IpV6_Ip_RequestIpAddrAssignment(ipV6AddrIdx, Type, localIpAddrPtr, Netmask, defaultRouterAddrPtr);
      }
    }
    else
    {
      /* Netmask is invalid. */
      TcpIp_CallDetReportError(TCPIP_API_ID_REQ_IP_ADDR_ASSIGN, TCPIP_E_INV_ARG);
    }
  }
#endif

  return retVal;
} /* TcpIp_RequestIpAddrAssignmentInternal() */

/**********************************************************************************************************************
 *  TcpIp_ReleaseIpAddrAssignment()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ReleaseIpAddrAssignment(
  TcpIp_LocalAddrIdType LocalAddrId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Call TcpIp_ReleaseSpecificIpAddrAssignment with all assignment methods. */
    retVal = TcpIp_ReleaseSpecificIpAddrAssignment(LocalAddrId, TCPIP_IPADDR_ASSIGNMENT_ANY);
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_REL_IP_ADDR_ASSIGN, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
}  /* TcpIp_ReleaseIpAddrAssignment() */

/**********************************************************************************************************************
 *  TcpIp_ReleaseSpecificIpAddrAssignment()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ReleaseSpecificIpAddrAssignment (
  TcpIp_LocalAddrIdType      LocalAddrId,
  TcpIp_IpAddrAssignmentType AssignmentType)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'AssignmentType' */
  else if ((AssignmentType > TCPIP_IPADDR_ASSIGNMENT_MAX_VALUE) && (AssignmentType != TCPIP_IPADDR_ASSIGNMENT_ANY))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfLocalAddrType localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId);

    /* #20 Forward the request to the related subcomponent, depending on the protocol of the given local address id. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
    if (TCPIP_LOCAL_ADDR_IDX_IS_IPV4(localAddrIdx))
# endif
    {
      /* LocalAddrId is an IpV4 address id */
      retVal = IpV4_Ip_ReleaseIpAddrAssignment(TCPIP_LOCAL_ADDR_IDX_TO_IPV4(localAddrIdx), AssignmentType);
    }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
    else
# endif
    {
      /* LocalAddrId is an IpV6 address id */
      uint32  preferredLifetime = 0;
      uint32  validLifetime     = 0;
      boolean noDad             = FALSE;

      retVal = IpV6_SetAddress(TCPIP_LOCAL_ADDR_IDX_TO_IPV6(localAddrIdx), NULL_PTR, preferredLifetime, validLifetime, noDad);
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_REL_IP_ADDR_ASSIGN, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_ReleaseSpecificIpAddrAssignment() */

/**********************************************************************************************************************
 *  TcpIp_GetPhysAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetPhysAddr(
  TcpIp_LocalAddrIdType LocalAddrId,
  TCPIP_P2V(uint8)      PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'PhysAddrPtr' */
  else if (PhysAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'LocalAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfLocalAddrType localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId);

    /* #20 Depending on the address-type of the given LocalAddrId, forward the request to the IpV4 or IpV6 subcomponent. */
    /* Forward request to IP */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
    if (TCPIP_LOCAL_ADDR_IDX_IS_IPV4(localAddrIdx))
# endif
    {
      /* IpV4 */
      TcpIp_LocalAddrV4IterType localAddrV4Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(localAddrIdx);
      IpV4_Ip_GetPhysAddr(localAddrV4Idx, PhysAddrPtr);
    }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
    else
# endif
    {
      /* IpV6 */
      TcpIp_LocalAddrV6IterType localAddrV6Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV6(localAddrIdx);
      IpV6_GetPhysAddrByIpAddrId(localAddrV6Idx, PhysAddrPtr);
    }
#endif
    retVal = E_OK;
  }
  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_PHYS_ADDR, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_GetPhysAddr() */

/**********************************************************************************************************************
 *  TcpIp_GetRemotePhysAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_GetRemotePhysAddr(
  uint8                         CtrlIdx,
  TCPIP_P2V(TcpIp_SockAddrType) IpAddrPtr,                                                                              /* PRQA S 3673 */ /* MD_TcpIp_AutosarApi */
  TCPIP_P2V(uint8)              PhysAddrPtr,
  boolean                       InitRes)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'IpAddrPtr' */
  else if (IpAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'PhysAddrPtr' */
  else if (PhysAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else if (CtrlIdx >= TcpIp_GetSizeOfEthIfCtrl())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TCPIP_P2C(TcpIp_SockAddrBaseType) sockAddrBaseTypePtr = (TCPIP_P2C(TcpIp_SockAddrBaseType))IpAddrPtr;               /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */

    /* #20 Forward the request to the IP subcomponent related to the domain of the given IpAddrPtr. */
    switch(IpAddrPtr->domain)
    {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
    case TCPIP_AF_INET:
      {
        /* IpV4 */
        retVal = IpV4_Ip_GetRemotePhysAddr(CtrlIdx, sockAddrBaseTypePtr->SockAddrIn.sin_addr, PhysAddrPtr, InitRes);
        break;
      }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
    case TCPIP_AF_INET6:
      {
        /* IpV6 */
        retVal = IpV6_GetRemotePhysAddr(CtrlIdx, &(sockAddrBaseTypePtr->SockAddrIn6.sin6_addr), PhysAddrPtr, InitRes);
        break;
      }
#endif
    default:
      {
        /* error, invalid domain type */
        retVal = TCPIP_E_NOT_OK;
        break;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_REMOTE_PHYS_ADDR, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_GetRemotePhysAddr() */

/**********************************************************************************************************************
 *  TcpIp_GetArpCacheEntries()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetArpCacheEntries(
  uint8                              CtrlIdx,
  TCPIP_P2V(uint32)                  NumberOfElementsPtr,                                                               /* PRQA S 3673 */ /* MD_TCPIP_Rule8.13_CfgSpecific */
  TCPIP_P2V(TcpIp_ArpCacheEntryType) EntryListPtr)                                                                      /* PRQA S 3673 */ /* MD_TCPIP_Rule8.13_CfgSpecific */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'NumberOfElementsPtr' */
  else if (NumberOfElementsPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'EntryListPtr' dependent on parameter 'NumberOfElementsPtr' */
  else if ((*NumberOfElementsPtr > 0u) && (EntryListPtr == NULL_PTR))
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else if (CtrlIdx >= TcpIp_GetSizeOfEthIfCtrl())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Forward call to IpV4 submodule. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
    retVal = IpV4_Arp_GetCacheEntries(CtrlIdx, NumberOfElementsPtr, EntryListPtr);
#else
    TCPIP_DUMMY_STATEMENT_CONST(CtrlIdx);                                                                               /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_ARP_CACHE_ENTRIES, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
  return retVal;
} /* TcpIp_GetArpCacheEntries() */

/**********************************************************************************************************************
 *  TcpIp_SendGratuitousArpReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_SendGratuitousArpReq(
  TcpIp_LocalAddrIdType LocalAddrId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;                                                                                     /* PRQA S 2981 */ /* MD_TCPIP_Rule2.2_2981_2983 */

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
  /* Check parameter 'LocalAddrId' */
  else if (LocalAddrId >= TcpIp_GetSizeOfLocalAddrV4())
  {
    errorId = TCPIP_E_INV_ARG;
  }
# endif
  else
#endif
  {
    TcpIp_LocalAddrIterType  localAddrIdx;
    /* ----- Implementation ----------------------------------------------- */
    localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId);
    /* #20 Check for state OnHold. */
    if (TcpIp_LocalAddrIdxIsOnHold(localAddrIdx) == FALSE)
    {
      /* #30 Forward call to IpV4 submodule. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
      retVal = IpV4_Arp_SendGratuitousArpReq(LocalAddrId);
#else
      TCPIP_DUMMY_STATEMENT_CONST(LocalAddrId);                                                                         /* PRQA S 2880, 3112, 2983 */ /* MD_TCPIP_Rule2.1_2880, MD_MSR_DummyStmt */
#endif
    }
  }
  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)                                                                                      /* PRQA S 2991, 2995 */ /* MD_TCPIP_Rule14.3_2741_2742_2991_2992_2995_2996 */
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_SEND_GRATUITOUS_ARP_REQ, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_SendGratuitousArpReq() */

/**********************************************************************************************************************
 *  TcpIp_AddArpCacheEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_AddArpCacheEntry(
  TcpIp_SizeOfEthIfCtrlDynType       EthIfCtrlIdx,
  TCPIP_P2C(TcpIp_SockAddrType)      RemoteAddrPtr,
  TCPIP_P2C(uint8)                   PhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;                                                                                     /* PRQA S 2981 */ /* MD_TCPIP_Rule2.2_2981_2983 */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else if (PhysAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else if (RemoteAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else if (RemoteAddrPtr->domain != TCPIP_AF_INET)
  {
    /* This feature is currently only supported for IPv4 */
    errorId = TCPIP_E_NOT_SUPPORTED;
  }
  else if (EthIfCtrlIdx >= TcpIp_GetSizeOfEthIfCtrl())
  {
    errorId = TCPIP_E_INV_ARG;
  }
#  if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
  else if (TcpIp_IsIpV4CtrlUsedOfEthIfCtrl(EthIfCtrlIdx) == FALSE)
  {
    /* This feature is currently only supported for IPv4 */
    errorId = TCPIP_E_NOT_SUPPORTED;
  }
#  endif
  else
# endif
  {
    /* #20 Call IPv4 delete function */
    IpBase_AddrInType ipAddr = ((TCPIP_P2C(TcpIp_SockAddrBaseType))RemoteAddrPtr)->SockAddrIn.sin_addr;                 /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
    retVal = IpV4_AddArpCacheEntry(TcpIp_GetIpV4CtrlIdxOfEthIfCtrl(EthIfCtrlIdx), ipAddr, PhysAddrPtr);
  }
#else
  /* This feature is currently only supported for IPv4 */
  errorId = TCPIP_E_NOT_SUPPORTED;
  TCPIP_DUMMY_STATEMENT_CONST(EthIfCtrlIdx);                                                                            /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
  TCPIP_DUMMY_STATEMENT_CONST(RemoteAddrPtr);                                                                           /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
  TCPIP_DUMMY_STATEMENT_CONST(PhysAddrPtr);                                                                             /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)                                                                                      /* PRQA S 2991, 2995 */ /* MD_TCPIP_Rule14.3_2741_2742_2991_2992_2995_2996 */
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_ADD_ARP_CACHE_ENTRIES, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_AddArpCacheEntry() */

/**********************************************************************************************************************
 *  TcpIp_RemoveArpCacheEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_RemoveArpCacheEntry(
  TcpIp_SizeOfEthIfCtrlDynType       EthIfCtrlIdx,
  TCPIP_P2C(TcpIp_SockAddrType)      RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;                                                                                     /* PRQA S 2981 */ /* MD_TCPIP_Rule2.2_2981_2983 */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else if (EthIfCtrlIdx >= TcpIp_GetSizeOfEthIfCtrl())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else if (RemoteAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else if (RemoteAddrPtr->domain != TCPIP_AF_INET)
  {
    /* This feature is currently only supported for IPv4 */
    errorId = TCPIP_E_NOT_SUPPORTED;
  }
#  if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
  else if (TcpIp_IsIpV4CtrlUsedOfEthIfCtrl(EthIfCtrlIdx) == FALSE)
  {
    /* This feature is currently only supported for IPv4 */
    errorId = TCPIP_E_NOT_SUPPORTED;
  }
#  endif
  else
# endif
  {
    /* #20 Call IPv4 delete function */
    IpBase_AddrInType ipAddr = ((TCPIP_P2C(TcpIp_SockAddrBaseType))RemoteAddrPtr)->SockAddrIn.sin_addr;                 /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
    retVal = IpV4_DelArpCacheEntry(TcpIp_GetIpV4CtrlIdxOfEthIfCtrl(EthIfCtrlIdx), ipAddr);
  }
#else
  /* This feature is currently only supported for IPv4 */
  errorId = TCPIP_E_NOT_SUPPORTED;
  TCPIP_DUMMY_STATEMENT_CONST(EthIfCtrlIdx);                                                                            /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
  TCPIP_DUMMY_STATEMENT_CONST(RemoteAddrPtr);                                                                           /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)                                                                                      /* PRQA S 2991, 2995 */ /* MD_TCPIP_Rule14.3_2741_2742_2991_2992_2995_2996 */
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_DEL_ARP_CACHE_ENTRIES, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_RemoveArpCacheEntry() */

/**********************************************************************************************************************
 *  TcpIp_GetCtrlIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetCtrlIdx(
  TcpIp_LocalAddrIdType LocalAddrId,
  TCPIP_P2V(uint8)      CtrlIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'CtrlIdxPtr' */
  else if (CtrlIdxPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'LocalAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfLocalAddrType localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId);

    /* #20 Depending on the protocol of the local address id pass the request to the affected submodule IpV4 or IpV6. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
    if(TCPIP_LOCAL_ADDR_IDX_IS_IPV4(localAddrIdx))
# endif
    {
      /* IpV4 */
      TcpIp_LocalAddrV4IterType localAddrV4Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(localAddrIdx);
      TcpIp_IpV4CtrlIdxOfLocalAddrV4Type ipV4CtrlIdx = TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(localAddrV4Idx);
      (*CtrlIdxPtr) = TcpIp_GetEthIfCtrlIdxOfIpV4Ctrl(ipV4CtrlIdx);
      retVal = E_OK;
    }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
    else
# endif
    {
      /* IpV6 */
      TcpIp_LocalAddrV6IterType localAddrV6Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV6(localAddrIdx);
      TcpIp_IpV6CtrlIdxOfLocalAddrV6Type ipV6CtrlIdx = TcpIp_GetIpV6CtrlIdxOfLocalAddrV6(localAddrV6Idx);
      (*CtrlIdxPtr) = TcpIp_GetEthIfCtrlIdxOfIpV6Ctrl(ipV6CtrlIdx);
      retVal = E_OK;
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_CTRL_IDX, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_GetCtrlIdx() */

/**********************************************************************************************************************
 *  TcpIp_MultiPartitionMainFunctionRx()
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
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_ON)
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MultiPartitionMainFunctionRx(
  TcpIp_OsApplicationType ApplId)
#else
FUNC(void, TCPIP_CODE) TcpIp_MultiPartitionMainFunctionRx(
  TcpIp_OsApplicationType ApplId)
#endif
{
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_OFF)
  /* #10 Check initialization of the component. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    /* module is not initialized, so do nothing here but just return */
  }
  else
#endif
  {
    /* IpV4 submodule does not have RX MainFunction. */

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
    /* #20 Call RX MainFunction of IpV6 submodule. */
    IpV6_MainFunctionRx();
#endif

    /* #30 Call RX MainFunction of UDP submodule. */
    TcpIp_Udp_MainFunctionRx(ApplId);

#if (TCPIP_SUPPORT_TCP == STD_ON)
    /* #40 Call RX MainFunction of TCP submodule. */
    TcpIp_Tcp_MainFunctionRx(ApplId);
#endif

#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
    /* #50 Call RX MainFunction of TLS submodule. */
    TcpIp_Tls_MainFunctionRx();
#endif
  }
} /* TcpIp_MultiPartitionMainFunctionRx() */

/**********************************************************************************************************************
 *  TcpIp_MultiPartitionMainFunctionState()
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
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_ON)
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MultiPartitionMainFunctionState(
  TcpIp_OsApplicationType ApplId)                                                                                       /* PRQA S 3206 */ /* MD_TCPIP_Rule2.7_3206 */
#else
FUNC(void, TCPIP_CODE) TcpIp_MultiPartitionMainFunctionState(
  TcpIp_OsApplicationType ApplId)                                                                                       /* PRQA S 3206 */ /* MD_TCPIP_Rule2.7_3206 */
#endif
{
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_OFF)
  /* #10 Check initialization of the component. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    /* module is not initialized, so do nothing here but just return */
  }
  else
#endif
  {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
    /* #20 Call MainFunction of IpV4 submodule. */
    IpV4_MainFunction(ApplId);
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
    /* #30 Call State MainFunction of IpV6 submodule. */
    IpV6_MainFunctionState();
#endif

#if (TCPIP_SUPPORT_DHCPV4SERVER == STD_ON)                                                                              /* COV_MSR_UNSUPPORTED */
    /* #40 Call MainFunction of DHCPv4 Server submodule. */
    TcpIp_DhcpV4Server_MainFunction();
#endif

    /* UDP submodule only has RX and TX MainFunctions. */

#if (TCPIP_SUPPORT_TCP == STD_ON)
    /* #50 Call State MainFunction of TCP submodule. */
    TcpIp_Tcp_MainFunctionState(ApplId);
#endif

#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
    /* #60 Call State MainFunction of TLS submodule. */
    TcpIp_Tls_MainFunctionState();
#endif
  }
#if (TCPIP_SUPPORT_TCP == STD_OFF) && (TCPIP_SUPPORT_IPV4 == STD_OFF)                                                   /* COV_TCPIP_NEVER_DEACTIVATED */
  TCPIP_DUMMY_STATEMENT_CONST(ApplId);
#endif
} /* TcpIp_MultiPartitionMainFunctionState() */

/**********************************************************************************************************************
 *  TcpIp_MultiPartitionMainFunctionTx()
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
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_ON)
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_MultiPartitionMainFunctionTx(
  TcpIp_OsApplicationType ApplId)
#else
FUNC(void, TCPIP_CODE) TcpIp_MultiPartitionMainFunctionTx(
  TcpIp_OsApplicationType ApplId)
#endif
{
#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_OFF)
  /* #10 Check initialization of the component. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    /* module is not initialized, so do nothing here but just return */
  }
  else
#endif
  {
#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
    /* #20 Call TX MainFunction of TLS submodule. */
    TcpIp_Tls_MainFunctionTx();
#endif

    /* #30 Call TX MainFunction of UDP submodule. */
    TcpIp_Udp_MainFunctionTx(ApplId);

#if (TCPIP_SUPPORT_TCP == STD_ON)
    /* #40 Call TX MainFunction of TCP submodule. */
    TcpIp_Tcp_MainFunctionTx(ApplId);
#endif

    /* IpV4 submodule does not have a TX MainFunction. */

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
    /* #50 Call TX MainFunction of IpV6 submodule. */
    IpV6_MainFunctionTx();
#endif
  }
} /* TcpIp_MultiPartitionMainFunctionTx() */

#if (TCPIP_SINGLE_MAIN_FUNCTION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_MultiPartitionMainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_MultiPartitionMainFunction(
  TcpIp_OsApplicationType ApplId)
{
  /* #10 Check initialization of the component. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    /* module is not initialized, so do nothing here but just return */
  }
  else
  {
    /* #20 Call RX MainFunction. */
    TcpIp_MultiPartitionMainFunctionRx(ApplId);

    /* #30 Call State MainFunction. */
    TcpIp_MultiPartitionMainFunctionState(ApplId);

    /* #40 Call TX MainFunction. */
    TcpIp_MultiPartitionMainFunctionTx(ApplId);
  }
} /* TcpIp_MultiPartitionMainFunction() */
#endif

/**********************************************************************************************************************
 *  TcpIp_ChangeParameter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050, 6080 1 */ /* MD_TcpIp_STCAL_CslAccess, MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ChangeParameter(
  TcpIp_SocketIdType SocketId,
  TcpIp_ParamIdType  ParameterId,
  TCPIP_P2V(uint8)   ParameterValue)                                                                                    /* PRQA S 3673 */ /* MD_TcpIp_AutosarApi */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId =         TCPIP_E_NO_ERROR;


  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_SOCKET_ID_IS_VALID(SocketId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'ParameterValue' */
  else if ((ParameterId != TCPIP_PARAMID_V_PHYS_DEST_ADDR) && (ParameterValue == NULL_PTR))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* ParameterId is not checked for DET because they are verified by the handlers for the specific parameters. */
  else
#endif
  {
    /* #20 Get related protocol of the ParameterId */
    /* ----- Local Variables ---------------------------------------------- */
    TcpIp_SizeOfSocketDynType socketIdx = TCPIP_SOCKET_ID_TO_IDX(SocketId);
    TcpIp_ParameterType parameterType;

    /* ----- Implementation ----------------------------------------------- */
    parameterType = TcpIp_GetParameterType(ParameterId);

    /* #30 Pass the value and ParameterId to the function of the related protocol */
    if (parameterType.IsIpV4 == TRUE)
    {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
      retVal &= TcpIp_IpV4_ChangeParameter(socketIdx, ParameterId, ParameterValue);
#endif
    }

    if (parameterType.IsIpV6 == TRUE)
    {
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
      retVal &= TcpIp_IpV6_ChangeParameter(socketIdx, ParameterId, ParameterValue);
#endif
    }

    if (parameterType.IsTcp == TRUE)
    {
#if (TCPIP_SUPPORT_TCP == STD_ON)
      retVal &= TcpIp_Tcp_ChangeParameter(socketIdx, ParameterId, ParameterValue);
#endif
    }

    if (parameterType.IsTls == TRUE)
    {
#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
        retVal &= TcpIp_Tls_ChangeParameter(socketIdx, ParameterId, ParameterValue);
#endif
    }

    if (parameterType.IsUdp == TRUE)
    {
      retVal &= TcpIp_Udp_ChangeParameter(socketIdx, ParameterId, ParameterValue);
    }

  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_CHANGE_PARAM, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_ChangeParameter() */

/**********************************************************************************************************************
 *  TcpIp_GetParameterType()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(TcpIp_ParameterType, TCPIP_CODE) TcpIp_GetParameterType(
  TcpIp_ParamIdType  ParameterId
)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ParameterType parameterType;

  /* ----- Implementation ----------------------------------------------- */
  parameterType.IsIpV4 = FALSE;
  parameterType.IsIpV6 = FALSE;
  parameterType.IsUdp =  FALSE;
  parameterType.IsTls =  FALSE;
  parameterType.IsTcp =  FALSE;

  /* #10 Set related protocol flags to true. */
  switch (ParameterId)
  {
    /* IPV4/6 Parameter */
  case TCPIP_PARAMID_FRAMEPRIO:
  case TCPIP_PARAMID_TTL:
    parameterType.IsIpV4 = TRUE;
    parameterType.IsIpV6 = TRUE;
    break;

    /* IPv4 Parameter */
  case TCPIP_PARAMID_V_PHYS_DEST_ADDR:
  case TCPIP_PARAMID_V_IPV4_TYPE_OF_SERVICE:
    parameterType.IsIpV4 = TRUE;
    break;

    /* IPv6 Parameter */
  case TCPIP_PARAMID_V_IPV6_FLOW_LABEL:
  case TCPIP_PARAMID_V_IPV6_TRAFFIC_CLASS:
    parameterType.IsIpV6 = TRUE;
    break;

    /* UDP Parameter */
  case TCPIP_PARAMID_V_UDP_TXREQLISTSIZE:
  case TCPIP_PARAMID_V_UDP_TXRETRYLISTSIZE:
  case TCPIP_PARAMID_UDP_CHECKSUM:
    parameterType.IsUdp = TRUE;
    break;

    /* TCP Parameter */
  case TCPIP_PARAMID_TCP_RXWND_MAX:
  case TCPIP_PARAMID_V_TCP_TXBUFSIZE:
  case TCPIP_PARAMID_TCP_NAGLE:
  case TCPIP_PARAMID_TCP_KEEPALIVE:
  case TCPIP_PARAMID_TCP_KEEPALIVE_INTERVAL:
  case TCPIP_PARAMID_TCP_KEEPALIVE_PROBES_MAX:
  case TCPIP_PARAMID_TCP_KEEPALIVE_TIME:
  case TCPIP_PARAMID_V_TCP_UTOUPDATE:
  case TCPIP_PARAMID_V_TCP_UTOENABLED:
  case TCPIP_PARAMID_V_TCP_UTOCHANGEABLE:
  case TCPIP_PARAMID_TCP_MSL:
  case TCPIP_PARAMID_TCP_RETRANSMIT_TIMEOUT:
    parameterType.IsTcp = TRUE;
    break;

    /* TLS Parameter */
  case TCPIP_PARAMID_TLS_CONNECTION_ASSIGNMENT:
  case TCPIP_PARAMID_TLS_CONNECTIONBASED_SESSION_RESUMPTION:
    parameterType.IsTls = TRUE;
    break;

  default:
    /* It is an unknown Parameter */
    break;
  }

  return parameterType;
} /* TcpIp_GetParameterType() */

#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
/**********************************************************************************************************************
 *  TcpIp_GetLocNetAddr()                                                                                  [DEPRECATED]
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetLocNetAddr(
  TcpIp_LocalAddrIdType        LocalAddrId,
  TCPIP_P2V(TcpIp_NetAddrType) NetAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'NetAddrPtr' */
  else if (NetAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'LocalAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check that parameter 'LocalAddrId' references an IPv4 address. */
#  if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
  else if (!TCPIP_LOCAL_ADDR_IDX_IS_IPV4(TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId)))
  {
    errorId = TCPIP_E_INV_ARG;
  }
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfLocalAddrType localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId);

    /* #20 Forward the request to the related function of the IpV4 submodule. */
    retVal = IpV4_Ip_GetLocNetAddr(TCPIP_LOCAL_ADDR_IDX_TO_IPV4(localAddrIdx), NetAddrPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_LOC_NET_ADDR, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* TcpIp_GetLocNetAddr() */

/**********************************************************************************************************************
 *  TcpIp_GetLocNetMask()                                                                                  [DEPRECATED]
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetLocNetMask(
  TcpIp_LocalAddrIdType        LocalAddrId,
  TCPIP_P2V(TcpIp_NetAddrType) NetMaskPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'NetMaskPtr' */
  else if (NetMaskPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check first criterion related to parameter 'LocalAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
#  if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
  /* Check that parameter 'LocalAddrId' references an IPv4 address. */
  else if (!TCPIP_LOCAL_ADDR_IDX_IS_IPV4(TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId)))
  {
    errorId = TCPIP_E_INV_ARG;
  }
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfLocalAddrType localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId);

    /* #20 Forward the request to the related function of the IpV4 submodule. */
    retVal = IpV4_Ip_GetLocNetMask(TCPIP_LOCAL_ADDR_IDX_TO_IPV4(localAddrIdx), NetMaskPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_LOC_NET_ADDR, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
}  /* TcpIp_GetLocNetMask() */
#endif

#if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_GetAndResetMeasurementData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetAndResetMeasurementData(
  TcpIp_MeasurementIdxType  MeasurementIdx,
  boolean                   MeasurementResetNeeded,
  TCPIP_P2V(uint32)         MeasurementDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Check Measurement Index. */
    if (   (MeasurementIdx != TCPIP_MEAS_DROP_TCP)
        && (MeasurementIdx != TCPIP_MEAS_DROP_UDP)
        && (MeasurementIdx != TCPIP_MEAS_DROP_IPV4)
        && (MeasurementIdx != TCPIP_MEAS_DROP_IPV6)
        && (MeasurementIdx != TCPIP_MEAS_VENDOR_SPECIFIC_DROP_LAYER3_IPV6)
        && (MeasurementIdx != TCPIP_MEAS_VENDOR_SPECIFIC_DROP_LAYER4)
        && (MeasurementIdx != TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_TCP_TX_BUFFER)
        && (MeasurementIdx != TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_TCP_RX_BUFFER)
        && (MeasurementIdx != TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_IPV4_FRAGMENT_RX_BUFFER)
# if (TCPIP_SUPPORT_IPSEC == STD_ON)
        && (MeasurementIdx != TCPIP_MEAS_VENDOR_SPECIFIC_NR_SA_PAIRS)
        && (MeasurementIdx != TCPIP_MEAS_VENDOR_SPECIFIC_SA_PAIRS)
# endif /* (TCPIP_SUPPORT_IPSEC == STD_ON) */
        && (MeasurementIdx != TCPIP_MEAS_ALL))
    {
      /* Unsupported measurement index. */
      retVal = E_NOT_OK;
    }
    else if (   (MeasurementIdx == TCPIP_MEAS_ALL)
             && (MeasurementDataPtr != NULL_PTR))
    {
      /* Unsupported combination of TCPIP_MEAS_ALL and DataPtr. */
      retVal = E_NOT_OK;
    }
    /* #30 Forward call to Internal function. */
    else
    {
      retVal = TcpIp_VGetAndResetMeasurementData(MeasurementIdx, MeasurementResetNeeded, MeasurementDataPtr);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_AND_RESET_MEAS_DATA, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* TcpIp_GetAndResetMeasurementData() */

/**********************************************************************************************************************
 *  TcpIp_VGetAndResetMeasurementData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6010, 6030, 6050 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VGetAndResetMeasurementData(
  TcpIp_MeasurementIdxType  MeasurementIdx,
  boolean                   MeasurementResetNeeded,
  TCPIP_P2V(uint32)         MeasurementDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_VENTER_MC_ALWAYS_CRITICAL_SECTION();  /* lock cross core interrupts */
  /* #10 Copy the counter value of the dropped packets to the given measurement data pointer, if MeasurementDataPtr is not NULL_PTR. */
  if (MeasurementDataPtr != NULL_PTR)
  {
    switch (MeasurementIdx)
    {
# if (TCPIP_SUPPORT_TCP == STD_ON)
    case TCPIP_MEAS_DROP_TCP:
      *MeasurementDataPtr = TcpIp_GetDroppedTcpPacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
      retVal = E_OK;
      break;

    case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_TCP_TX_BUFFER:
      *MeasurementDataPtr = TcpIp_GetDroppedTcpPacketsInsuffTxBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
      retVal = E_OK;
      break;

    case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_TCP_RX_BUFFER:
      *MeasurementDataPtr = TcpIp_GetDroppedTcpPacketsInsuffRxBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
      retVal = E_OK;
      break;
# endif

    case TCPIP_MEAS_DROP_UDP:
      *MeasurementDataPtr = TcpIp_GetDroppedUdpPacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
      retVal = E_OK;
      break;

    case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_LAYER4:
      *MeasurementDataPtr = TcpIp_GetDroppedVndrLayer4PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
      retVal = E_OK;
      break;

# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
    case TCPIP_MEAS_DROP_IPV4:
      *MeasurementDataPtr = TcpIp_GetDroppedIpV4PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
      retVal = E_OK;
      break;

    case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_IPV4_FRAGMENT_RX_BUFFER:
      *MeasurementDataPtr = TcpIp_GetDroppedIpV4PacketsInsuffReassemblyBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
      retVal = E_OK;
      break;
# endif

# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
    case TCPIP_MEAS_DROP_IPV6:
      *MeasurementDataPtr = TcpIp_GetDroppedIpV6PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
      retVal = E_OK;
      break;

    case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_LAYER3_IPV6:
      *MeasurementDataPtr = TcpIp_GetDroppedVndrIpV6PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
      retVal = E_OK;
      break;

# endif

# if (TCPIP_SUPPORT_IPSEC == STD_ON)
    case TCPIP_MEAS_VENDOR_SPECIFIC_NR_SA_PAIRS:
      /* Store the current number of valid Sa entry pairs in current context */
      TcpIp_IpSecSaEntryPairCnt = TcpIp_IpSec_GetNumSaEntryPairs();
      *MeasurementDataPtr = TcpIp_IpSecSaEntryPairCnt;
      retVal = E_OK;
      break;

    case TCPIP_MEAS_VENDOR_SPECIFIC_SA_PAIRS:
      /* Retrieve the IPsec status */
      /* PRQA S 0310 2 */ /* MD_TCPIP_Rule11.3_0310_Uint32ToStruct */
      retVal = TcpIp_IpSec_GetSaEntryPairsStatus((TcpIp_IpSecSaStatusType *)MeasurementDataPtr, TcpIp_IpSecSaEntryPairCnt);
      break;
# endif /* (TCPIP_SUPPORT_IPSEC == STD_ON) */

    default:
      /* Unsupported measurement index. Nothing to do. */
      break;
    }
  }

  /* #20 Reset measurement data counter of dropped packets, if MeasurementResetNeeded is TRUE. */
  if(MeasurementResetNeeded == TRUE)
  {
    retVal = TcpIp_VResetSpecificMeasurementData(MeasurementIdx);
  }
  TCPIP_VLEAVE_MC_ALWAYS_CRITICAL_SECTION();
  return retVal;
} /* TcpIp_VGetAndResetMeasurementData() */

/**********************************************************************************************************************
 *  TcpIp_VResetSpecificMeasurementData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
 /* PRQA S 6010, 6030 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VResetSpecificMeasurementData(
  TcpIp_MeasurementIdxType  MeasurementIdx)
{
  Std_ReturnType retVal = E_NOT_OK;
  /* #10 Search and reset measurements */
  switch (MeasurementIdx)
  {
# if (TCPIP_SUPPORT_TCP == STD_ON)
  case TCPIP_MEAS_DROP_TCP:
    TcpIp_SetDroppedTcpPacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
    retVal = E_OK;
    break;

  case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_TCP_TX_BUFFER:
    TcpIp_SetDroppedTcpPacketsInsuffTxBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
    retVal = E_OK;
    break;

  case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_TCP_RX_BUFFER:
    TcpIp_SetDroppedTcpPacketsInsuffRxBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
    retVal = E_OK;
    break;
# endif

  case TCPIP_MEAS_DROP_UDP:
    TcpIp_SetDroppedUdpPacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
    retVal = E_OK;
    break;

  case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_LAYER4:
    TcpIp_SetDroppedVndrLayer4PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
    retVal = E_OK;
    break;

# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
  case TCPIP_MEAS_DROP_IPV4:
    TcpIp_SetDroppedIpV4PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
    retVal = E_OK;
    break;

  case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_IPV4_FRAGMENT_RX_BUFFER:
    TcpIp_SetDroppedIpV4PacketsInsuffReassemblyBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
    retVal = E_OK;
    break;
# endif

# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  case TCPIP_MEAS_DROP_IPV6:
    TcpIp_SetDroppedIpV6PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
    retVal = E_OK;
    break;

  case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_LAYER3_IPV6:
    TcpIp_SetDroppedVndrIpV6PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
    retVal = E_OK;
    break;
# endif

  case TCPIP_MEAS_ALL:
    /* #20 Reset all measurement data counter in case of TCPIP_MEAS_ALL. */
    TcpIp_VResetMeasurementData();
    retVal = E_OK;
    break;

  default:
    /* Unsupported measurement index. Nothing to do. */
    break;
  }
  return retVal;
} /* TcpIp_VResetSpecificMeasurementData */

/**********************************************************************************************************************
 *  TcpIp_VResetMeasurementData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_VResetMeasurementData(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset measurement data counter. */

# if (TCPIP_SUPPORT_TCP == STD_ON)
  TcpIp_SetDroppedTcpPacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
  TcpIp_SetDroppedTcpPacketsInsuffTxBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
  TcpIp_SetDroppedTcpPacketsInsuffRxBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
# endif

  TcpIp_SetDroppedUdpPacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
  TcpIp_SetDroppedVndrLayer4PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);

# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
  TcpIp_SetDroppedIpV4PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
  TcpIp_SetDroppedIpV4PacketsInsuffReassemblyBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
# endif

# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  TcpIp_SetDroppedIpV6PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
  TcpIp_SetDroppedVndrIpV6PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX, 0);
# endif
} /* TcpIp_VResetMeasurementData() */
#endif

/**********************************************************************************************************************
 *  TcpIp_GetIpAddr()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
/* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_TcpIp_STCAL_CslAccess, MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetIpAddr(
  TcpIp_LocalAddrIdType         LocalAddrId,
  TCPIP_P2V(TcpIp_SockAddrType) IpAddrPtr,
  TCPIP_P2V(uint8)              NetmaskPtr,
  TCPIP_P2V(TcpIp_SockAddrType) DefaultRouterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal  = E_NOT_OK;
  uint8          errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else
#endif
  /* Check parameter 'LocalAddrId' */
  if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId))
  {
    /* invalid or any address id: Leave retVal at value E_NOT_OK */
    errorId = TCPIP_E_INV_ARG;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfLocalAddrType localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId);

    /* PRQA S 0310, 3305 2 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
    TCPIP_P2V(TcpIp_SockAddrBaseType) ipAddrPtr            = (TCPIP_P2V(TcpIp_SockAddrBaseType))IpAddrPtr;
    TCPIP_P2V(TcpIp_SockAddrBaseType) defaultRouterAddrPtr = (TCPIP_P2V(TcpIp_SockAddrBaseType))DefaultRouterPtr;

    /* #20 Check if address family is AF_INET (IPv4). */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
    if(TCPIP_LOCAL_ADDR_IDX_IS_IPV4(localAddrIdx))
# endif
    {
      TcpIp_SizeOfLocalAddrV4Type ipV4AddrIdx = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(localAddrIdx);

      /* #30 Write IpV4 address if requested. */
      if (ipAddrPtr != NULL_PTR)
      {
        ipAddrPtr->sa_family = IPBASE_AF_INET;
        retVal = IpV4_Ip_GetLocNetAddr(ipV4AddrIdx, &ipAddrPtr->SockAddrIn.sin_addr);
      }
      else
      {
        retVal = E_OK;
      }

      if (retVal == E_OK)
      {
        /* #40 Write IpV4 netmask if requested. */
        if (NetmaskPtr != NULL_PTR)
        {
          IpBase_AddrInType netmask;

          if (IpV4_Ip_GetLocNetMask(ipV4AddrIdx, &netmask) == E_OK)
          {
            *NetmaskPtr = TcpIp_VIpV4NetmaskToCidr(netmask);
          }
          else
          {
            *NetmaskPtr = 0;
          }
        }

        /* #50 Write IpV4 default router address if requested. */
        if (defaultRouterAddrPtr != NULL_PTR)
        {
          defaultRouterAddrPtr->sa_family = IPBASE_AF_INET;
          /* Ignore return value of IpV4_Ip_GetDefaultRouterNetAddr() because TcpIp_GetIpAddr() should not return an error if there is no default router. */
          IpV4_Ip_GetDefaultRouterNetAddr(ipV4AddrIdx, &defaultRouterAddrPtr->SockAddrIn.sin_addr);
        }
      }
    }
#endif
     /* #60 Check if address family is AF_INET6 (IPv6). */
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
    else
# endif
    {
      TcpIp_SizeOfLocalAddrV6Type ipV6AddrIdx = TCPIP_LOCAL_ADDR_IDX_TO_IPV6(localAddrIdx);

      /* #70 Write IpV6 address and netmask if requested. */
      if (ipAddrPtr != NULL_PTR)
      {
        ipAddrPtr->sa_family = IPBASE_AF_INET6;
        retVal = IpV6_GetLocalAddress(ipV6AddrIdx, &ipAddrPtr->SockAddrIn6.sin6_addr, NetmaskPtr);
      }
      else
      {
        retVal = E_OK;
      }

      /* #80 Write IpV6 default router address if requested. */
      if ((retVal == E_OK) && (defaultRouterAddrPtr != NULL_PTR))
      {
        boolean routerIsReachable;
        defaultRouterAddrPtr->sa_family = IPBASE_AF_INET6;
        /* Ignore return value of IpV6_GetDefaultRouterAddress() because TcpIp_GetIpAddr() should not return an error if there is no default router. */
        (void)IpV6_GetDefaultRouterAddress(ipV6AddrIdx, &defaultRouterAddrPtr->SockAddrIn6.sin6_addr, &routerIsReachable);
      }
      /* else: No further action requested: Leave retVal on its current value */
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_IP_ADDR, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_GetIpAddr() */

/**********************************************************************************************************************
 *  TcpIp_GetRemNetAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetRemNetAddr(
  TcpIp_SocketIdType               SocketId,
  TCPIP_P2V(TcpIp_SockAddrTypeP2V) SockAddrPtr)                                                                         /* PRQA S 3673 */ /* MD_TCPIP_Rule8.13_CfgSpecific */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check Parameter 'SocketId'. */
  else if (!TCPIP_SOCKET_ID_IS_VALID(SocketId))
  {
    errorId = TCPIP_E_INV_SOCK_ID;
  }
  /* Check parameter 'SockAddrPtr' */
  else if (SockAddrPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (TCPIP_SUPPORT_TCP == STD_ON)
    TcpIp_SizeOfSocketDynType socketIdx = TCPIP_SOCKET_ID_TO_IDX(SocketId);

    /* #20 In case it is a TCP socket AND the socket is connected: Store the remote address. */
    if (TCPIP_SOCKET_IDX_IS_TCP(socketIdx))
    {
      if (TcpIp_Tcp_GetSockIsConnected(TCPIP_SOCKET_IDX_TO_TCP_IDX(socketIdx)) == TRUE)
      {
        *SockAddrPtr = &TcpIp_GetAddrRemSockOfSocketDyn(socketIdx)->TcpIpSockAddr;
        retVal = E_OK;
      }
      /* else: TCP socket is not connected: Leave retVal at value E_NOT_OK. */
    }
    /* else: No remote socket address is stored for UDP sockets: Leave retVal at value E_NOT_OK. */
#else
    TCPIP_DUMMY_STATEMENT_CONST(SocketId);                                                                              /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
    TCPIP_DUMMY_STATEMENT_CONST(SockAddrPtr);                                                                           /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */

    /* #30 Reject the request in case only UDP is enabled because in this case no remote addresses are stored. */
    retVal = E_NOT_OK;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_REM_NET_ADDR, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_GetRemNetAddr() */

#if (TCPIP_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_GetVersionInfo(
  TCPIP_P2V(Std_VersionInfoType) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'versioninfo' */
  else if (versioninfo == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Write the version info into the struct pointed to by the given pointer. */
    versioninfo->vendorID         = TCPIP_VENDOR_ID;
    versioninfo->moduleID         = TCPIP_MODULE_ID;
    versioninfo->sw_major_version = TCPIP_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = TCPIP_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = TCPIP_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_VERSION_INFO, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
} /* TcpIp_GetVersionInfo() */
#endif
  /* TCPIP_VERSION_INFO_API */

#if TCPIP_SUPPORT_DHCPV4 == STD_ON
/**********************************************************************************************************************
 *  TcpIp_DhcpReadOption()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpReadOption(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  uint8                 Option,
  TCPIP_P2V(uint8)      DataLength,
  TCPIP_P2V(uint8)      DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalIpAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalIpAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
#  if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
  else if (!TCPIP_LOCAL_ADDR_IDX_IS_IPV4(TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalIpAddrId)))
  {
    errorId = TCPIP_E_INV_ARG;
  }
#  endif
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_LocalAddrIterType   localAddrIdx   = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalIpAddrId);
    TcpIp_LocalAddrV4IterType localAddrV4Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(localAddrIdx);
    /*@ assert DataLength != NULL_PTR && $lengthOf(DataPtr) >= *DataLength; */                                          /* VCA_TCPIP_PTR_AND_DATALENGTH */

    /* #20 Forward the request to the DHCPv4 subcomponent. */
    retVal = TcpIp_DhcpV4_VGetTxOption(localAddrV4Idx, Option, DataLength, DataPtr);                                    /* VCA_TCPIP_PTR_AND_DATALENGTH */
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_SET_DHCP_READ_OPT, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* TcpIp_DhcpReadOption() */

/**********************************************************************************************************************
 *  TcpIp_DhcpWriteOption()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpWriteOption(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  uint8                 Option,
  uint8                 DataLength,
  TCPIP_P2C(uint8)      DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalIpAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalIpAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
#  if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
  else if (!TCPIP_LOCAL_ADDR_IDX_IS_IPV4(TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalIpAddrId)))
  {
    errorId = TCPIP_E_INV_ARG;
  }
#  endif
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_LocalAddrIterType   localAddrIdx   = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalIpAddrId);
    TcpIp_LocalAddrV4IterType localAddrV4Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(localAddrIdx);

    /* #20 Forward the request to the DHCPv4 subcomponent. */
    retVal = TcpIp_DhcpV4_VSetTxOption(localAddrV4Idx, Option, DataLength, DataPtr, TCPIP_DHCPV4_MSG_FLAG_ALWAYS);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_SET_DHCP_WRITE_OPT, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* TcpIp_DhcpWriteOption() */
#endif /* TCPIP_SUPPORT_DHCPV4 == STD_ON */

#if (TCPIP_SUPPORT_DHCPV6 == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
# if (TCPIP_SUPPORT_DHCPV6_OPTIONS == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_DhcpV6ReadOption()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6ReadOption(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  uint16                Option,
  TCPIP_P2V(uint16)     DataLength,
  TCPIP_P2V(uint8)      DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalIpAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalIpAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
#   if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                   /* COV_TCPIP_IPV4_ALWAYS_ON */
  else if (!TCPIP_LOCAL_ADDR_IDX_IS_IPV6(TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalIpAddrId)))                                    /* PRQA S 2742, 2995, 3415 */ /* MD_TCPIP_Rule14.3_2741_2742_2991_2992_2995_2996, MD_TCPIP_Rule13.5_3415_CslReadOnly */ /*lint !e506 */
  {
    errorId = TCPIP_E_INV_ARG;                                                                                          /* PRQA S 2880 */ /* MD_TCPIP_Rule2.1_2880 */
  }
#   endif
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfLocalAddrType   localAddrIdx   = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalIpAddrId);
    TcpIp_SizeOfLocalAddrV6Type localAddrV6Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV6(localAddrIdx);

    /* Socket has Unicast or AutoSelect binding. This means use concrete or any IP on concrete controller for transmission */
    /* #20 Forward the request to the DHCPv6 subcomponent. */
    retVal = TcpIp_DhcpV6_VGetTxOption(localAddrV6Idx, Option, DataLength, DataPtr);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_SET_DHCP_V6_READ_OPT, errorId);
  }
#  else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif

  return retVal;
} /* TcpIp_DhcpV6ReadOption() */

/**********************************************************************************************************************
 *  TcpIp_DhcpV6WriteOption()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV6WriteOption(
  TcpIp_LocalAddrIdType LocalIpAddrId,
  uint16                Option,
  uint16                DataLength,
  TCPIP_P2C(uint8)      DataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalIpAddrId' */
  else if (!TCPIP_LOCAL_ADDR_IDX_IS_IPV6(TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalIpAddrId)))                                    /* PRQA S 2742, 2995, 3415 */ /* MD_TCPIP_Rule14.3_2741_2742_2991_2992_2995_2996, MD_TCPIP_Rule13.5_3415_CslReadOnly */ /*lint !e506 */
  {
    errorId = TCPIP_E_INV_ARG;                                                                                          /* PRQA S 2880 */ /* MD_TCPIP_Rule2.1_2880 */
  }
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfLocalAddrType   localAddrIdx   = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalIpAddrId);
    TcpIp_SizeOfLocalAddrV6Type localAddrV6Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV6(localAddrIdx);

    /* #20 Forward the request to the DHCPv6 subcomponent. */
    retVal = TcpIp_DhcpV6_VSetTxOption(localAddrV6Idx, Option, DataLength, DataPtr, TCPIP_DHCPV6_MSG_FLAG_ALLWAYS);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_SET_DHCP_V6_WRITE_OPT, errorId);
  }
#  else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#  endif

  return retVal;
} /* TcpIp_DhcpV6WriteOption() */
# endif
#endif /* TCPIP_SUPPORT_DHCPV6 == STD_ON */

/**********************************************************************************************************************
 *  TcpIp_GetDhcpTimeoutInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_GetDhcpTimeoutInfo(
  uint8 EthIfCtrlIdx,
  uint8 IpVer)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal  = FALSE;
  uint8   errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'CtrlIdx' */
  else if (EthIfCtrlIdx >= TcpIp_GetSizeOfEthIfCtrl())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (TCPIP_SUPPORT_DHCPV4 == STD_ON)
    /* #20 Check the given IP version to be IpV4. */
    if ((IpVer == TCPIP_IP_PROT_VER_4) && (TcpIp_IsIpV4CtrlUsedOfEthIfCtrl(EthIfCtrlIdx) == TRUE))                      /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
    {
      /* #30 Call the internal service function of the DHCPv4 submodule in order to retrieve the timeout status. */
      TcpIp_IpV4CtrlIterType ipV4CtrlIdx = TcpIp_GetIpV4CtrlIdxOfEthIfCtrl(EthIfCtrlIdx);
      /*@ assert ipV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); */                                                            /* VCA_TCPIP_CSL03_IS_USEDOF */
      /* The following function returns the timeout status, directly, i.e.: FALSE: No DHCP timeout occurred, TRUE: A DHCP timeout occurred. */
      retVal = TcpIp_DhcpV4_GetDhcpTimeoutInfo(ipV4CtrlIdx);
    }
    /* else: This case is currently not supported: Leave retVal at value FALSE */
#else
    /* DHCP is not activated, so no timeout can occur: Leave retVal at value FALSE. */
    TCPIP_DUMMY_STATEMENT_CONST(IpVer);                                                                                 /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
    TCPIP_DUMMY_STATEMENT_CONST(EthIfCtrlIdx);                                                                          /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_DHCP_TIMEOUT_INFO, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_GetDhcpTimeoutInfo() */

/**********************************************************************************************************************
 *  TcpIp_GetIpAddrCfgSrc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetIpAddrCfgSrc(
  TcpIp_LocalAddrIdType  LocalAddrId,
  TCPIP_P2V(uint8)       CfgSrcPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'LocalAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'CfgSrcPtr' */
  else if (CfgSrcPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfLocalAddrType localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId);

    /* #20 Check address family. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
    if (TCPIP_LOCAL_ADDR_IDX_IS_IPV4(localAddrIdx))
# endif
    {
      /* #30 Map IpV4 address configuration source to TcpIp configuration source. */
      (*CfgSrcPtr) = IpV4_Ip_GetActiveAddrAssignmentMethod(TCPIP_LOCAL_ADDR_IDX_TO_IPV4(localAddrIdx));
      retVal = E_OK;
    }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
    else
# endif
    {
       /* #40 Map IpV6 address configuration source to TcpIp configuration source. */
      (*CfgSrcPtr) = IpV6_GetLocalAddressCfgSrc(TCPIP_LOCAL_ADDR_IDX_TO_IPV6(localAddrIdx));
      retVal = E_OK;
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_IP_ADDR_CFG_SRC, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_GetIpAddrCfgSrc() */

/**********************************************************************************************************************
 *  TcpIp_RequestComMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_RequestComMode(
  uint8            CtrlIdx,
  TcpIp_StateType  State)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'CtrlIdx' */
  else if (CtrlIdx >= TcpIp_GetSizeOfEthIfCtrl())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'State' */
  else if (State > TCPIP_STATE_ONLINE)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set requested com mode */
    TcpIp_VRequestComModeInternal(CtrlIdx, State);

    /* Set return value for API compatibility */
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_TCP_REQUEST_COM_MODE, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_RequestComMode() */

/**********************************************************************************************************************
 *  TcpIp_GetLocSockAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetLocSockAddr(
  TcpIp_SocketIdType            SocketId,
  TCPIP_P2V(TcpIp_SockAddrType) SockPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_SOCKET_ID_IS_VALID(SocketId))
  {
    errorId = TCPIP_E_INV_SOCK_ID;
  }
  /* Check parameter 'SockPtr' */
  else if (SockPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfSocketDynType socketIdx = TCPIP_SOCKET_ID_TO_IDX(SocketId);
    TCPIP_P2V(TcpIp_SockAddrBaseType) sockAddrPtr = (TCPIP_P2V(TcpIp_SockAddrBaseType))SockPtr;                         /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */

    /* #20 In case all parameters are valid, copy the desired ones. */
    retVal = IpBase_CopySockAddr(&sockAddrPtr->SockAddr, &TcpIp_GetAddrLocSockOfSocketDyn(socketIdx)->SockAddr);        /* VCA_TCPIP_SOCKET_ADDRESS_COPY */
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_LOCAL_SOCK_ADDR, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_GetLocSockAddr() */

#if (TCPIP_SUPPORT_DIAG_ACCESS_EXTENSIONS == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_DiagDataReadAccess()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DiagDataReadAccess(
  TcpIp_SocketIdType   SocketId,
  TcpIp_DiagParamsType DataID,
  TCPIP_P2V(uint8)     DataPtr,
  TCPIP_P2V(uint16)    DataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_SOCKET_ID_IS_VALID(SocketId))
  {
    errorId = TCPIP_E_INV_SOCK_ID;
  }
  /* Check parameter 'DataID' */
  else if (DataID >= TCPIP_DP_INVALID)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'DataPtr' */
  else if (DataPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'DataLenPtr' */
  else if (DataLenPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_SizeOfSocketDynType socketIdx = TCPIP_SOCKET_ID_TO_IDX(SocketId);
    /* #20 In case all parameters are valid, call the internal service function that conducts the desired action. */
    retVal = TcpIp_DiagDataReadAccessInternal(socketIdx, DataID, DataPtr, DataLenPtr);                                  /* VCA_TCPIP_PTR_AND_DATALENGTH */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_DIAG_DATA_READ_ACCESS, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* TcpIp_DiagDataReadAccess() */

/**********************************************************************************************************************
 *  TcpIp_DiagDataReadAccessInternal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DiagDataReadAccessInternal(
  TcpIp_SizeOfSocketDynType SocketIdx,
  TcpIp_DiagParamsType      DataID,
  TCPIP_P2V(uint8)          DataPtr,
  TCPIP_P2V(uint16)         DataLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketIdx < TcpIp_GetSizeOfSocketDyn());

  /* #10 Evaluate the DataID the caller desires to read. */
  switch(DataID)
  {
  case TCPIP_DP_SOCKET_LOCAL_PORT:
    /* ----------------------------------------------------------------------------------------- */
    /* #20 Read the current local port number for a given socket. */
    if(*DataLenPtr >= 2u)
    {
      /*@ assert $lengthOf(DataPtr) >= 2; */                                                                            /* VCA_TCPIP_PTR_AND_DATALENGTH */
      TCPIP_PUT_UINT16(DataPtr, 0, TcpIp_GetAddrLocSockOfSocketDyn(SocketIdx)->DomainAndPort.port);
      *DataLenPtr = 2;
      retVal = E_OK;
    }
    break;
# if ((TCPIP_SUPPORT_TCP == STD_ON) && (TCPIP_TCP_DIAG_READ_ACK_SEQ_NUM_ENABLED == STD_ON))
  case TCPIP_DP_SOCKET_TCP_CURRENT_ACK_NUM:
    /* ----------------------------------------------------------------------------------------- */
    /* #30 Read the TCP acknowledgment number that would be used for the next segment for sending. */
    if (TCPIP_SOCKET_IDX_IS_TCP(SocketIdx) && (*DataLenPtr >= 4u))
    {
      /*@ assert $lengthOf(DataPtr) >= 4; */                                                                            /* VCA_TCPIP_PTR_AND_DATALENGTH */
      TcpIp_SizeOfSocketTcpDynType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx);
      TCPIP_PUT_UINT32_RAW(DataPtr, 0, TcpIp_GetRcvNxtOfSocketTcpDyn(socketTcpIdx));  /* direct access to a TCP variable */
      *DataLenPtr = 4;
      retVal = E_OK;
    }
    break;

  case TCPIP_DP_SOCKET_TCP_NEXT_SEQ_NUM:
    /* ----------------------------------------------------------------------------------------- */
    /* #40 Read the TCP sequence number that would be used for the next data segment for sending. */
    if (TCPIP_SOCKET_IDX_IS_TCP(SocketIdx) && (*DataLenPtr >= 4u))
    {
      /*@ assert $lengthOf(DataPtr) >= 4; */                                                                            /* VCA_TCPIP_PTR_AND_DATALENGTH */
      TcpIp_SizeOfSocketTcpDynType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx);
      TCPIP_PUT_UINT32_RAW(DataPtr, 0, TcpIp_GetTxNextSendSeqNoOfSocketTcpDyn(socketTcpIdx));  /* direct access to a TCP variable */
      *DataLenPtr = 4;
      retVal = E_OK;
    }
    break;
# endif
  default:
    /* do nothing */
    break;
  }
  return retVal;
}
#endif /* (TCPIP_SUPPORT_DIAG_ACCESS_EXTENSIONS == STD_ON) */

/**********************************************************************************************************************
 *  TcpIp_ClearARCache()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_ClearARCache(
  TcpIp_LocalAddrIdType LocalAddrId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                  /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_LocalAddrIterType localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId);

    /* #20 Forward call to IPv4 or IPv6 sub module depending on the address type. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
    if (TCPIP_LOCAL_ADDR_IDX_IS_IPV4(localAddrIdx))
# endif
    {
      /* IpV4 */
      TcpIp_LocalAddrV4IterType localAddrV4Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(localAddrIdx);
      IpV4_Arp_ClearCache(localAddrV4Idx);
      retVal = E_OK;
    }
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
# if (TCPIP_SUPPORT_IPV4 == STD_ON)
    else
# endif
    {
      /* IpV6 */
      TcpIp_LocalAddrV6IterType localAddrV6Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV6(localAddrIdx);
      retVal = IpV6_Ndp_ClearCache(localAddrV6Idx);
    }
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_CLEAR_AR_CACHE, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_ClearARCache() */

/**********************************************************************************************************************
 *  TcpIp_GetNdpCacheEntries()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetNdpCacheEntries(
  uint8                              CtrlIdx,
  TCPIP_P2V(uint32)                  NumberOfElementsPtr,                                                               /* PRQA S 3673 */ /* MD_TcpIp_AutosarApi */
  TCPIP_P2V(TcpIp_NdpCacheEntryType) EntryListPtr)                                                                      /* PRQA S 3673 */ /* MD_TcpIp_AutosarApi */
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
  TcpIp_EthIfCtrlIterType ethIfCtrlIdx = CtrlIdx;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  /* Check parameter 'TcpIp_State' */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'ethIfCtrlIdx' */
  else if (ethIfCtrlIdx >= TcpIp_GetSizeOfEthIfCtrl())
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else if (TcpIp_IsIpV6CtrlUsedOfEthIfCtrl(ethIfCtrlIdx) == FALSE)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  /* Check parameter 'NumberOfElementsPtr' */
  else if (NumberOfElementsPtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'EntryListPtr' dependent on parameter 'NumberOfElementsPtr' */
  else if ((*NumberOfElementsPtr > 0u) && (EntryListPtr == NULL_PTR))
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Return cache entries if IpV6 is supported. */
    TcpIp_IpV6CtrlIterType ipV6CtrlIdx = TcpIp_GetIpV6CtrlIdxOfEthIfCtrl(ethIfCtrlIdx);

    IpV6_Ndp_GetCacheEntries(ipV6CtrlIdx, NumberOfElementsPtr, EntryListPtr);
    retVal = E_OK;
  }
#else
  uint8 errorId = TCPIP_E_NOT_SUPPORTED;
  TCPIP_DUMMY_STATEMENT_CONST(CtrlIdx);                                                                                 /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
  TCPIP_DUMMY_STATEMENT_CONST(NumberOfElementsPtr);                                                                     /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
  TCPIP_DUMMY_STATEMENT_CONST(EntryListPtr);                                                                            /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif /* (TCPIP_SUPPORT_IPV6 == STD_ON) */

  /* ----- Development Error Report --------------------------------------- */
#if (TCPIP_DEV_ERROR_REPORT == STD_ON)
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  if (errorId != TCPIP_E_NO_ERROR)
# endif
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_NDP_CACHE_ENTRIES, errorId);
  }
#else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif

  return retVal;
} /* TcpIp_GetNdpCacheEntries() */

#if (TCPIP_SUPPORT_IPSEC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_IpSecSaEntryPairAdd()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecSaEntryPairAdd(
  TcpIp_LocalAddrIdType             LocalAddrId,
  TCPIP_P2C(TcpIp_IpSecSaInfoType)  SaInfoPtr,
  boolean                           IsInitiator)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  /* Check parameter 'TcpIp_State' */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SaSelPtr' */
  else if (   (SaInfoPtr == NULL_PTR)
           || (SaInfoPtr->KeyMaterial.InboundKeyMatPtr == NULL_PTR)
           || (SaInfoPtr->KeyMaterial.OutboundKeyMatPtr == NULL_PTR)
           || (SaInfoPtr->TrafficSelectors == NULL_PTR)
           || (SaInfoPtr->InboundSpi == 0u))
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* Check parameter 'LocalAddrId' */
  else if (!TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId))
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    TcpIp_LocalAddrIterType   localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId);

    /* #20 Add a pair of SA entries using the parameters passed */
    retVal = TcpIp_IpSec_AddSaEntryPair(localAddrIdx, SaInfoPtr, IsInitiator);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_IPSEC_ADD_SAENTRY_PAIR, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_IpSecSaEntryPairDelete()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecSaEntryPairDelete(
  uint32                       InboundSpi)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  /* Check parameter 'TcpIp_State' */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else if (InboundSpi == 0u)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Delete a pair of SA entries referenced by the Spi */
    retVal = TcpIp_IpSec_DelSaEntryPair(InboundSpi);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if(errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_IPSEC_DEL_SAENTRY_PAIR, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_IpSecSaEntryPreparePairDelete()
 *********************************************************************************************************************/
 /*! Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecSaEntryPairPrepareDelete(
  uint32                       InboundSpi)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = TCPIP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check component initialization state and input parameters. */
  /* Check parameter 'TcpIp_State' */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  else if (InboundSpi == 0u)
  {
    errorId = TCPIP_E_INV_ARG;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set the SA entry to prepare for deletion */
    retVal = TcpIp_IpSec_PrepareDelSaEntryPair(InboundSpi);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_IPSEC_PREPARE_DEL_SAENTRY_PAIR, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
}
#endif


#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_GetAvailableTxBufferSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetAvailableTxBufferSize(
  TcpIp_SocketIdType  SocketId,
  TCPIP_P2V(uint32)   AvailableTxBufferSizePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = TCPIP_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
# if (TCPIP_DEV_ERROR_DETECT == STD_ON)                                                                                 /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check component initialization state and input parameters. */
  if (TcpIp_State == TCPIP_STATE_UNINIT)
  {
    errorId = TCPIP_E_NOTINIT;
  }
  /* Check parameter 'SocketId' */
  else if (!TCPIP_SOCKET_ID_IS_VALID(SocketId))
  {
    errorId = TCPIP_E_INV_SOCK_ID;
  }
  else if (!TCPIP_SOCKET_IDX_IS_TCP(TCPIP_SOCKET_ID_TO_IDX(SocketId)))
  {
    /* Socket is not a TCP socket */
    errorId = TCPIP_E_INV_SOCK_ID;
  }
  /* Check parameter 'RemTxBufferSizePtr' */
  else if (AvailableTxBufferSizePtr == NULL_PTR)
  {
    errorId = TCPIP_E_PARAM_POINTER;
  }
  /* #20 Forward call to internal function. */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    retVal = TcpIp_Tcp_GetAvailableTxBufferSize(TCPIP_SOCKET_ID_TO_IDX(SocketId), AvailableTxBufferSizePtr);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_GET_REM_TXBUFFER_SIZE, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
}
#endif /* (TCPIP_SUPPORT_TCP == STD_ON) */

/**********************************************************************************************************************
 *  INTERNAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TcpIp_VPreconfigSocket()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VPreconfigSocket(TcpIp_SocketDynIterType SocketIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 addrFamily;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketIdx < TcpIp_GetSizeOfSocketDyn());

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
  TCPIP_ASSERT(sizeof(TcpIp_SockAddrBaseType) == sizeof(TcpIp_SockAddrInet6Type));
  addrFamily = IPBASE_AF_INET6;
#else
  TCPIP_ASSERT(sizeof(TcpIp_SockAddrBaseType) >= sizeof(TcpIp_SockAddrInetType));
  addrFamily = IPBASE_AF_INET;
#endif

  /* #10 Initialize the given socket's admin data for both IP protocols. */
  (void)IpBase_DelSockAddr(&TcpIp_GetAddrRemSockOfSocketDyn(SocketIdx)->SockAddr, addrFamily);                          /* VCA_DEL_SOCK_ADDR_CORRECT_TYPE */
  (void)IpBase_DelSockAddr(&TcpIp_GetAddrLocSockOfSocketDyn(SocketIdx)->SockAddr, addrFamily);                          /* VCA_DEL_SOCK_ADDR_CORRECT_TYPE */


  /* #20 Initialize the given socket connection status. */
  TcpIp_SetListenActiveConnStatOfSocketDyn(SocketIdx, TCPIP_SOCK_CONN_NONE);
} /* TcpIp_VPreconfigSocket() */

/**********************************************************************************************************************
 *  TcpIp_VSocketAccessExclusive()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(boolean, TCPIP_CODE) TcpIp_VSocketAccessExclusive(TcpIp_SizeOfSocketDynType SocketIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketIdx < TcpIp_GetSizeOfSocketDyn());

  /* #10 Enter exclusive area. */
  TCPIP_VENTER_CRITICAL_SECTION();  /* lock interrupts */

  /* #20 Try to obtain semaphore (lock variable) and return result via retVal. */
  if (TcpIp_IsTxBufRequestedOfSocketDyn(SocketIdx) == TRUE)
  {
    retVal = FALSE;
  }
  else
  {
    TcpIp_SetTxBufRequestedOfSocketDyn(SocketIdx, TRUE);
    retVal = TRUE;
  }

  /* #30 Exit exclusive area. */
  TCPIP_VLEAVE_CRITICAL_SECTION();  /* unlock interrupts */

  return retVal;
} /* TcpIp_VSocketAccessExclusive() */

#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*
   MICROSAR global MISRA deviations:
   - see file 'WI_MISRA_Compliance_MSR.pdf'

   module specific MISRA deviations:

   MD_TCPIP_Rule13.5_3415_CslReadOnly: The right hand operand of a logical && or || operator shall not contain side effects.
     Reason:     No side effects possible because the (inline) function only wraps a read-only access to a variable/constant.
                 Used to justify read-only access to constants/variables via inline function generated by the ComStackLib (CSL). (e.g.: TcpIp_Get*()
                 TcpIp_Is*(),TcpIp_Has*())
     Risk:       None.
     Prevention: Covered by code review.

   MD_TCPIP_Rule13.5_3415_ReadOnly: The right hand operand of a logical && or || operator shall not contain side effects.
     Reason:     Moving function call to extra statement would increase complexity of the code.
                 It is intended that this function is only called if the first check failed (||) / was true (&&).
                 The function does not modify any global variables and therefore has no side effects.
     Risk:       Behavior of the function may be changed in future and the above condition could be not valid anymore.
     Prevention: Covered by code review.

   MD_TcpIp_STCAL_CslAccess: Number of distinct function calls too high (HIS: STCAL = 0..7)
     Reason:     Number of distinct function calls is only too high due to trivial inline functions for accessing the ComStackLib data.
                 Complexity of the function does not increase compared to ComStackLib access via macros.
     Risk:       None. Using inline functions instead of macros for accessing the configuration data does not affect complexity of the code.
     Prevention: n/a

   MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType: Casting to different object pointer type. / Pointer cast to stricter alignment.
     Reason:     TcpIp_SockAddrType is casted to union type TcpIp_SockAddrBaseType in order to avoid further casts to TcpIp_SockAddrInetType and TcpIp_SockAddrInet6Type.
     Risk:       Unaligned memory access may occur if the source buffer is not aligned correctly.
     Prevention: Covered by code review.

   MD_TcpIp_AutosarApi:
     Reason:     This API signature is specified by AUTOSAR and therefore not changed.
     Risk:       None
     Prevention: n/a

   MD_TcpIp_0612_BufferSize: [L] Size of object '-identifier-' exceeds 32767 bytes - program is non-conforming.
     Reason:     Large buffers of this size are only used for testing purposes.
     Risk:       None.
     Prevention: n/a

   MD_TCPIP_Rule11.3_0310_3305_AddrList: Casting to different object pointer type.
     Reason:     Casting from uint8 to IpV6_AddrType type referring to an array of IpV6 addresses of variable size.
     Risk:       Unaligned memory access may occur if the source buffer is not aligned correctly.
     Prevention: Covered by code review.

   MD_TCPIP_Rule11.3_0310_Uint8ToStruct: Casting to different object pointer type.
     Reason:     Casting from uint8 to Struct type.
     Risk:       Unaligned memory access may occur if the data is not aligned correctly.
     Prevention: Covered by code review.

   MD_TCPIP_Rule11.3_0310_CastToUInt8: Casting pointer type of higher alignment to uint8 pointer
     Reason:     The data shall be processed byte wise.
                 1) The EthIf_ProvideTxBuffer provides the buffer as type Eth_DataType which may be uint16 or uint32.
                 2) Pointer is passed as IpV4_Ip_HdrType but following payload is accessed byte wise.
     Risk:       None. The Data is stored in network byte order regardless of the types Eth_DataType and IpV4_Ip_HdrType.
     Prevention: Covered by code review.

   MD_TCPIP_Rule8.13_CfgSpecific: The object addressed by the pointer parameter '...' is not modified and so the pointer could be of type 'pointer to const'.
     Reason:     The parameter is only modified in some preprocessor variants.
     Risk:       The parameter could also be modified in variants where a modification is not intended.
     Prevention: Covered by code review.

   MD_TCPIP_Rule16.3_2003: An unconditional break statement shall terminate every switch-clause
     Reason:     The fall-through is utilized to execute statements which are common to all the switch cases
     Risk:       Cases in the beginning might execute unnecessary statements.
     Prevention: Covered by code review.

   MD_TCPIP_Rule1.2_1039: Treating array of length one as potentially flexible member.
     Reason:     The struct is defined so to allow interoperability between IPv4 and IPv6
     Risk:       None.
     Prevention: Covered by code review.

   MD_TCPIP_Rule11.3_0310_Uint32ToStruct: Casting to different object pointer type.
     Reason:     Casting from uint32 to Struct type to reuse existing public API
                 for special case
     Risk:       Unaligned memory access may occur if the data is not aligned correctly.
     Prevention: Covered by code review.

   MD_TCPIP_Rule8.9_3218: File scope static, '%s', only accessed in one function.
     Reason:      Vector style guide prevents usage of static variables in function scope
     Risk:        None.
     Prevention:  None.

   MD_TCPIP_Rule2.2_2986: This operation is redundant. The value of the result is always that of the right-hand operand.
     Reason:      The redundant operation is a side effect of a better readability of the code.
     Risk:        None.
     Prevention:  n/a

   MD_TCPIP_Rule14.3_2741_2742_2991_2992_2995_2996: The value of the controlling expression in 'if' statement and logical operation is always true/false.
     Reason:      In specific configurations (e.g. IPv4-only or IPv6-only), the expression evaluates to a constant value.
     Risk:        In the affected variants, the always true/false code might be confusing.
     Prevention:  None. Explicit handling of precompile variants will decrease readability of code.

   MD_TCPIP_Rule2.1_2880: This code is unreachable.
     Reason:      In specific configurations, the comparison evaluates to a constant expression.
     Risk:        Compiler warnings or compiler errors depending on configuration.
                  Unreachable code cannot be tested in specific configurations.
     Prevention:  None.

   MD_TCPIP_Rule2.2_2981_2983: This initialization is redundant. The value of this object is never used before being modified or never subsequently used.
     Reason:      The initialization is only redundant in specific configurations.
     Risk:        The redundant initialization code might be confusing.
     Prevention:  None. Explicit handling of precompile variants will decrease readability of code.

   MD_TCPIP_Rule2.7_3206: The parameter '%s' is not used in this function.
     Reason:      This parameter is used for configuration specific code and might only be active in some configurations.
     Risk:        None.
     Prevention:  None. Explicit handling of precompile variants will decrease readability of code.

   MD_TCPIP_Rule3.1_3108: Nested comments are not recognized in the ISO standard.
     Reason:      This is not a nested comment, but a double slash as part of a URL.
     Risk:        None.
     Prevention:  None. Covered by code review.

   MD_TCPIP_2843:
     Reason:      A Local parameter prevEleIdx is used to access an array element. This variable is always assigned a valid index by the
                  preceding while loop. It is ensured that this code is only reached if there is at least one element in the retry queue.
     Risk:        There is no risk.
     Prevention:  Covered by code review.

   MD_TCPIP_2897:
     Reason:      The function parameter LocalAddrIdx is used for TCPIP_LOCAL_ADDR_IDX_TO_IPV6().
                  This macro is used only if the check((TcpIp_GetAddrLocSockOfSocketDyn(newSocketIdx)->sa_family == IPBASE_AF_INET6))around it is true.
                  Caller of this function make sure sa_family is set to IPBASE_AF_INET6. That means the index is IpV6 Index.
                  That's why this macro will never return negative value.
     Risk:        There is no risk.
     Prevention:  Covered by code review.

   MD_TCPIP_2896: Negative value is not cast to unsigned type socketIdx
     Reason:      The Local parameter socketIdx is passed into macro TCPIP_SOCKET_IDX_TO_TCP_IDX to convert socket index to TCP socket index.
                  This line of code is only executed if socket is in state TCPIP_SOCK_CONN_ACTIVE, which is only true for TCP but not for UDP sockets.
                  Hence, there is no chance that the calculated TCP socket index gets negative.
     Risk:        There is no risk.
     Prevention:  Covered by code review.

   MD_TCPIP_Rule20.13_3115: Unrecognized preprocessing directive has been ignored because of conditional inclusion directives.
     Reason:      The preprocessing directive "warning" is not known by QAC framework.
                  The directive is only used during Vector Code Analyzer execution, to ensure consistency of the analysis.
     Risk:        There is no risk.
     Prevention:  Covered by code review.

   MD_TCPIP_FalsePossible_Rule2983: This assignment is redundant. The value of this object is never subsequently used.
     Reason:      This is a false possible, this assigned value is later used.
     Risk:        There is no risk.
     Prevention:  Covered by code review.

*/

/* COV_JUSTIFICATION_BEGIN
  \ID COV_TCPIP_IPV4_ALWAYS_ON
  \ACCEPT TX
  \REASON IPv4 is always active in SafeBSW configurations.

  \ID COV_TCPIP_MULTICORE_SUPPORT_ALWAYS_OFF
  \ACCEPT TX
  \REASON Multicore is always inactive in SafeBSW configurations.

  \ID COV_TCPIP_MULTICORE_SUPPORT_NEVER_ON
  \ACCEPT XF
  \REASON Multicore is always inactive in SafeBSW configurations.

  \ID COV_TCPIP_MULTIPLE_UNSAFE_FEATURES
  \ACCEPT XF
  \ACCEPT XF xf xf
  \REASON All related features are not available in SafeBSW configurations.

  \ID COV_TCPIP_IPV6_SUPPORT_ALWAYS_OFF
  \ACCEPT XF xf tf
  \REASON IPv6 is always inactive in SafeBSW configurations.

  \ID COV_TCPIP_DHCPV4_OR_IPV6_SUPPORT_ALWAYS_OFF
  \ACCEPT TF tf xf
  \REASON DHCPv4 is switched on and off, but IPv6 is always inactive in SafeBSW configurations.

  \ID COV_TCPIP_ICMPV4_ALWAYS_ON
  \ACCEPT TX
  \REASON ICMPv4 is always active in SafeBSW configurations.

  \ID COV_TCPIP_IPV6_ONLY
  \ACCEPT XF xf xf
  \REASON IPv6 is always inactive in SafeBSW configurations.

  \ID COV_TCPIP_DHCPv6_SUPPORT_ALWAYS_OFF
  \ACCEPT TF tf xf
  \REASON DHCPv6 is always inactive in SafeBSW configurations.

  \ID COV_TCPIP_DEV_ERROR_DETECT
  \ACCEPT TX
  \REASON TCPIP_DEV_ERROR_DETECT must always be enabled in SafeBSW.

  \ID COV_TCPIP_CANOE_DEBUG
  \ACCEPT XF
  \ACCEPT XF xf xf
  \REASON This code is only active in debug environments.

  \ID COV_TCPIP_COMPATIBILITY
  \ACCEPT TX
  \ACCEPT XF
  \REASON [COV_MSR_COMPATIBILITY]

  \ID COV_TCPIP_C_COMMENT_VECTOR
  \ACCEPT XF
  \REASON Commented section.

  \ID COV_TCPIP_BYTE_ORDER
  \ACCEPT XF
  \REASON Only little endian byte-order is tested.

  \ID COV_TCPIP_UNIT_TEST
  \ACCEPT TX
  \REASON Code is only inactive in unit test environment.

  \ID COV_TCPIP_RUNTIME_TEST
  \ACCEPT XF xf xx
  \REASON Code in only active in runtime test environment.

  \ID COV_TCPIP_NEVER_DEACTIVATED
  \ACCEPT XF tf xf
  \REASON IPv4 is never deactivated in SafeBSW.

  \ID COV_TCPIP_VCA_EXCLUDE
  \ACCEPT XF
  \REASON Code produces problems during VCA analysis and is therefore excluded from analysis.

  \ID COV_TCPIP_HARDWARE_DEPENDING
  \ACCEPT XF
  \ACCEPT TX
  \REASON Code is active/inactive for some hardware platforms.

COV_JUSTIFICATION_END */

/* VCA_JUSTIFICATION_START

  \ID VCA_TCPIP_CSL01
    \DESCRIPTION       CSL index validated based on CSL functions. (Qualified use-case CSL01 of ComStackLib.)
    \COUNTERMEASURE \R The function ensures that the index is within a valid range by using the runtime check
                       of TcpIp_GetSizeOf...() function.

  \ID VCA_TCPIP_CSL03_START_END
    \DESCRIPTION       Valid index derived by CSL indirection based on valid index.
                       (Qualified use-case CSL03 of ComStackLib.)
    \COUNTERMEASURE \R The function ensures that the index is within a valid range by using the
                       TcpIp_...EndIdxOf...(...Idx_A) function.
                       The validity of the Idx_A used as function parameter within the TcpIp_...EndIdxOf...(...Idx_A)
                       call is ensured by either:
                       - An explicit check.
                       - A VCA assertion.
                       - A VCA requires statement.

  \ID VCA_TCPIP_CSL03_IS_USEDOF
    \DESCRIPTION       Valid index derived by CSL indirection based on valid index.
                       (Qualified use-case CSL03 of ComStackLib.)
    \COUNTERMEASURE \R It is ensured by usage of TcpIp_Is...UsedOf...() or a check against the corresponding
                       TCPIP_NO_... define, that the derived index is valid.
                       The index which is used to check TcpIp_Is...UsedOf...() is ensured to be within
                       a valid range by either:
                       - An explicit check.
                       - A VCA assertion.
                       - A VCA requires statement.
                       TcpIp_Is...UsedOf...() is only needed for 0:n relation. For 1:1 relations,
                       TcpIp_Get..Of..() is used.

  \ID VCA_TCPIP_CONFIG_PTR_RESTORE
    \DESCRIPTION       The config ptr is restored to a valid value.
    \COUNTERMEASURE \N The config ptr TcpIp_ConfigDataPtr is set during initialization and not touchted after that.
                       It is ensured by review that it is always set to a valid value.

  \ID VCA_TCPIP_PTR_AND_DATALENGTH
    \DESCRIPTION       Memory write access to pointer which is passed via public API TcpIp_DiagDataReadAccess() or
                       TcpIp_DhcpReadOption().
    \COUNTERMEASURE \S It is ensured via SMI-16 that the public API is invoked with a data pointer, that points to
                       an array with at least the specified length.

  \ID VCA_TCPIP_MEMCPY_WITH_CHECKED_PTR_AND_LEN
    \DESCRIPTION       Memcpy function 'VStdLib_MemCpy' or IpBase_TcpIpChecksumCopyAdd is called with valid pointer
                       and a checked length.
    \COUNTERMEASURE \R Runtime check and/or "requires" constraint present which ensures that:
                        - the given pointer points to a valid memory region != NULL_PTR
                        - the calculated length used for copying data to the buffer fits to the used buffer.
                        - if an offset is used, the offset must be subtracted of the available length.

  \ID VCA_TCPIP_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION
    \DESCRIPTION        Write access to given pointer cannot be ensured to be within valid range.
    \COUNTERMEASURE \N  Combination of pointer as well as length of the pointer is satisfied by formal specification.

  \ID VCA_TCPIP_RUNTIME_CHECK_PRESENT_IN_SAME_FUNCTION
    \DESCRIPTION       Index which is calculated/checked within the function.
    \COUNTERMEASURE \R Index is checked inside the function using runtime check. Qualified UseCase of CSL01.

  \ID VCA_TCPIP_SOCKIDX
    \DESCRIPTION       Socket index within range [0..TcpIp_GetSizeOfSocketUdpDyn()-1] or [TcpIp_GetSizeOfSocketUdpDyn()..TcpIp_GetSizeOfSocketDyn()-1].
    \COUNTERMEASURE \N The array SocketDyn[] contains all UDP sockets followed by all TCP sockets:
                       TcpIp_GetSizeOfSocketDyn() = TcpIp_GetSizeOfSocketUdpDyn() + TcpIp_GetSizeOfSocketTcpDyn().
                       Therefore TcpIp_GetSizeOfSocketUdpDyn() is never bigger than TcpIp_GetSizeOfSocketDyn().
                       TcpIp_GetSizeOfSocketDyn() is generated by the CSL: Qualified use-case CSL01 of ComStackLib.

  \ID VCA_TCPIP_TCP_IDX_FROM_CHECKED_SOCKET_IDX
    \DESCRIPTION       TCP socket index which is derived from generic socket index using the macro:
                       TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx).
                       -> SocketTcpIdx == SocketIdx - TcpIp_GetSizeOfSocketUdpDyn().
    \COUNTERMEASURE \N Socket index is ensured to be within valid range by:
                       1: SocketIdx is checked for validity: SocketIdx < TcpIp_GetSizeOfSocketDyn()
                          TcpIp_GetSizeOfSocketDyn() = TcpIp_GetSizeOfSocketUdpDyn() + TcpIp_GetSizeOfSocketTcpDyn()
                       2: SocketIdx is checked to be a TCP socket.
                          (e.g. via TCPIP_SOCKET_IDX_IS_TCP(SocketIdx) or socket state is only valid for TCP sockets.)

  \ID VCA_TCPIP_UDP_IDX_FROM_CHECKED_SOCKET_IDX
    \DESCRIPTION       UDP socket index which is derived from generic socket index using the
                       macro TCPIP_SOCKET_IDX_TO_UDP_IDX(SocketIdx). -> SocketUdpIdx == SocketIdx.
    \COUNTERMEASURE \N Socket index is ensured to be a valid UDP socket. (e.g. via TCPIP_SOCKET_IDX_IS_UDP(SocketIdx)).

  \ID VCA_TCPIP_API_SOCKETOWNERCONFIG_IDX
    \DESCRIPTION       The returned socketOwnerConfigIdx of TcpIp_GetSocketOwnerConfigIdxOfSocketDyn() is valid
                       for socketIdx of assigned sockets because TcpIp_VGetSocketForOwner() sets
                       ListenActiveConnStatOfSocketDyn and SocketOwnerConfigIdxOfSocketDyn.
    \COUNTERMEASURE \R It is checked with:
                       TcpIp_GetListenActiveConnStatOfSocketDyn(SocketIdx) != TCPIP_SOCK_CONN_NONE
                       that the socket is in a assigned state.
                       Additionally VCA asserts are used to make sure the socked index is within a valid range.

  \ID VCA_TCPIP_SOCKETIDX_FROM_CHECKED_SOCKETID
    \DESCRIPTION       Valid index socketIdx or socketTcpIdx derived based on valid socketId.
    \COUNTERMEASURE \N The socketIdx is resolved via macro TCPIP_SOCKET_ID_TO_IDX(SocketId).
                       The socketTcpIdx might be additionally resolved via macro TCPIP_SOCKET_IDX_TO_TCP_IDX(socketIdx).
                       TCPIP_SOCKET_ID_TO_IDX() is a direct mapping: socketIdx == SocketId.
                       TCPIP_SOCKET_IDX_TO_TCP_IDX() performs a calculation of the socket index under consideration
                       of the configured UDP sockets.
                       The parameter SocketId is checked by a SafeBSW check via the macro
                       TCPIP_SOCKET_ID_IS_VALID(SocketId) = (SOCKET_ID) < TcpIp_GetSizeOfSocketDyn().

  \ID VCA_TCPIP_LISTEN_SOCKET_TCP_IDX_FROM_SOCKETTCPIDX
    \DESCRIPTION       listenSocketTcpIdx is derived from TCP socket index.
    \COUNTERMEASURE \N listenSocketTcpIdx is within a valid range because the indirection index is valid.
                       TCP socket index is a function parameter and its validity is ensured by the caller.

  \ID VCA_TCPIP_BIND_IMPLICIT
    \DESCRIPTION       Local address is within a valid range.
    \COUNTERMEASURE \N This function is only called in context of a UDP packet transmission via TcpIp_UdpTransmit().
                       The LocalAddrBindIdxOfSocketDyn is within a valid range, because this value as well as the
                       ListenActiveConnStatOfSocketDyn are initialized if the socket is requested via a call to
                       TcpIp_GetSocketForUser(). During packet transmission, it is ensured that the
                       ListenActiveConnStatOfSocketDyn is set properly and hence, that the LocalAddrBindIdxOfSocketDyn
                       is also within a valid range.

  \ID VCA_TCPIP_SOCKET_ADDRESS_COPY
    \DESCRIPTION       An IP address is copied to another of the same type and address family.
    \COUNTERMEASURE \N It is ensured that the target address has enough space to store the address.
                       - For IPv4 addresses the raw address is 4 Bytes long.
                       - For IPv6 addresses the raw address is 16 Bytes long.
                       The address family is checked by the source address.
                       Runtime check and/or "requires" constraint present which ensures that:
                        - the given pointer points to a valid memory region != NULL_PTR.
                        - the length of the target address buffer is sufficient.

  \ID VCA_DEL_SOCK_ADDR_CORRECT_TYPE
    \DESCRIPTION       The API IpBase_DelSockAddr is called with correct datatype
    \COUNTERMEASURE \N It is ensured that the the API is called with IpBase_SockAddrType:
                       - For IPv4 addresses the ip address is 4 Bytes long.
                       - For IPv6 addresses the ip address is 16 Bytes long.
                       This is ensured by using the TcpIp_SockAddrBaseType union, that use the IPv6 type, so it is
                       always big enough to contain IPv4 and IPv6 addresses, if IPv6 is active. Alternatively, the union
                       also use the IPv4 type. For Safe only IPv4 is acceptable.

  VCA_JUSTIFICATION_END */

/* VCR_JUSTIFICATION_START

  \ID VCR_TCPIP_E_NOT_OK
    \DESCRIPTION       If-statement with condition that checks equality to "E_NOT_OK".
    \COUNTERMEASURE \N Covered by code review.

  VCR_JUSTIFICATION_END */

/* TCPIP_CSL_START

  \ID TCPIP_CSL_INDEX_ACCEPTED
    \DESCRIPTION       The CLS function is called with an index name that do not fit to the CSL array name
    \COUNTERMEASURE \N Covered by code review.

  \ID TCPIP_CSL_MISSDETECTION
    \DESCRIPTION       This CLS finding is a false positive finding by the CLS check tool
    \COUNTERMEASURE \N Covered by code review.

  \ID TCPIP_CSL_NAME_LENGTH
    \DESCRIPTION       A valid name would be to long
    \COUNTERMEASURE \N Covered by code review.

  TCPIP_CSL_END */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp.c
 *********************************************************************************************************************/
