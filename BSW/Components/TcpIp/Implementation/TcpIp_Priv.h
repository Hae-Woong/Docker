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
 *         \file  TcpIp_Priv.h
 *        \brief  Implementation of TcpIp Module - Internal Functions
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#if !defined TCPIP_PRIV_H
# define TCPIP_PRIV_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "TcpIp_Priv_Types.h"

# include "TcpIp_Cfg.h"
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif
# include "TcpIp_Lcfg.h"
# include "SchM_TcpIp.h"

# include "EthIf.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define TCPIP_SOCK_CONN_NONE                          0u
# define TCPIP_SOCK_CONN_ALLOCATED                     1u
# define TCPIP_SOCK_CONN_LISTEN_ENABLED                2u
# if(TCPIP_SUPPORT_TCP == STD_ON)
#  define TCPIP_SOCK_CONN_ACTIVE                       3u  /* any TCP state other than CLOSED and LISTEN */
# endif

/* Ethernet frame types */
# define TCPIP_ETH_FRAME_TYPE_IPV4                     0x0800u
# define TCPIP_ETH_FRAME_TYPE_ARP                      0x0806u
# define TCPIP_ETH_FRAME_TYPE_IPV6                     0x86DDu

/* Private constants of the TCPIP */
# define TCPIP_TX_CONF_LEN_MAX                         0xFFFFU

# define TCPIP_CHECKSUM_UNSET                          0U

# if defined(TCPIP_VENABLE_CANOE_WRITE_STRING)                                                                          /* COV_TCPIP_CANOE_DEBUG */
/* Use user defined value. */
# else
#  define TCPIP_VENABLE_CANOE_WRITE_STRING STD_OFF
# endif

# define TCPIP_API_ID_V_INTERNAL_FUNCTION              0xFFu

# define TCPIP_DHCPV4_MULTIPLE_CONFIG_SETS             STD_ON
# define TCPIP_DHCPV4_MULTIPLE_INSTANCES               STD_ON

# define TCPIP_TCP_HDR_SOURCE_PORT_OFFSET              0u
# define TCPIP_TCP_HDR_TARGET_PORT_OFFSET              2u
# define TCPIP_UDP_HDR_SOURCE_PORT_OFFSET              0u
# define TCPIP_UDP_HDR_TARGET_PORT_OFFSET              2u

# define IP_MAX_ETHIF_FRAME_PRIO                       7u

# define TCPIP_ADDR_MATCH_EXACT                       1u /* Match via IP address value */
# define TCPIP_ADDR_MATCH_INTERFACE                   2u /* Match via Broadcast/All-Node. */
# define TCPIP_ADDR_MATCH_ANYIP                       3u /* Match via ANY-IP. */
# define TCPIP_ADDR_MATCH_NONE                        4u /* No kind of match fits. */

# define TCPIP_BITS_IN_BYTE   8u   /* A byte contains 8 bits */
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 3453 FunctionLikeMacros */ /* MD_MSR_FctLikeMacro */

# if (TCPIP_VENABLE_CANOE_WRITE_STRING == STD_ON)                                                                       /* COV_TCPIP_CANOE_DEBUG */
#  include "stdio.h"
#  include "CANoeApi.h"
#  define CANOE_WRITE_STRING(Txt)                 CANoeAPI_WriteString((Txt));
#  define CANOE_WRITE_STRING_1(Txt, P1)           \
    _snprintf(TcpIp_CanoeWriteStr, 256, Txt, P1); CANoeAPI_WriteString((TcpIp_CanoeWriteStr));
#  define CANOE_WRITE_STRING_2(Txt, P1, P2)       \
    _snprintf(TcpIp_CanoeWriteStr, 256, Txt, P1, P2); \
    CANoeAPI_WriteString((TcpIp_CanoeWriteStr));
#  define CANOE_WRITE_STRING_3(Txt, P1, P2, P3)   \
    _snprintf(TcpIp_CanoeWriteStr, 256, Txt, P1, P2, P3); \
    CANoeAPI_WriteString((TcpIp_CanoeWriteStr));
#  define CANOE_WRITE_STRING_4(Txt, P1, P2, P3, P4)   \
    _snprintf(TcpIp_CanoeWriteStr, 256, Txt, P1, P2, P3, P4); \
    CANoeAPI_WriteString((TcpIp_CanoeWriteStr));
#  define CANOE_WRITE_INCOMPLETE_FCT()  CANOE_WRITE_STRING_2(">> INCOMPLETE FUNCTION IMPLEMENTATION CALLED ON LINE %s<%d> <<\r\n", __FILE__, __LINE__); CANoeAPI_StopMeasurement();
#  define CANOE_WRITE_UNTESTED_FCT()    CANOE_WRITE_STRING_2(">> UNTESTED FUNCTION CALLED ON LINE %s<%d> <<\r\n", __FILE__, __LINE__); CANoeAPI_StopMeasurement();
# else
#  define CANOE_WRITE_STRING(txt)
#  define CANOE_WRITE_STRING_1(txt, p1)
#  define CANOE_WRITE_STRING_2(txt, p1, p2)
#  define CANOE_WRITE_STRING_3(txt, p1, p2, p3)
#  define CANOE_WRITE_STRING_4(txt, p1, p2, p3, p4)
#  define CANOE_WRITE_INCOMPLETE_FCT()
#  define CANOE_WRITE_UNTESTED_FCT()
# endif

# define TCPIP_VENTER_CRITICAL_SECTION() SchM_Enter_TcpIp_TCPIP_EXCLUSIVE_AREA_0()
# define TCPIP_VLEAVE_CRITICAL_SECTION() SchM_Exit_TcpIp_TCPIP_EXCLUSIVE_AREA_0()

# define TCPIP_VENTER_MC_ALWAYS_CRITICAL_SECTION() SchM_Enter_TcpIp_TCPIP_EXCLUSIVE_AREA_1()
# define TCPIP_VLEAVE_MC_ALWAYS_CRITICAL_SECTION() SchM_Exit_TcpIp_TCPIP_EXCLUSIVE_AREA_1()

/* Critical Section, that is only used when MC is enabled */
# if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_ON)                                                                        /* COV_TCPIP_MULTICORE_SUPPORT_NEVER_ON */
#  define TCPIP_VENTER_MC_ONLY_CRITICAL_SECTION() SchM_Enter_TcpIp_TCPIP_EXCLUSIVE_AREA_1()
#  define TCPIP_VLEAVE_MC_ONLY_CRITICAL_SECTION() SchM_Exit_TcpIp_TCPIP_EXCLUSIVE_AREA_1()
# else
#  define TCPIP_VENTER_MC_ONLY_CRITICAL_SECTION() /* Critical Section is only required in Multicore environments */
#  define TCPIP_VLEAVE_MC_ONLY_CRITICAL_SECTION() /* Critical Section is only required in Multicore environments */
# endif


# define TCPIP_COND_INIT                     ( TCPIP_STATE_INIT == TcpIp_State )  /* TCPIP_E_NOTINIT */                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */





# define TCPIP_SOCKET_ID_TO_IDX(SOCKET_ID)   ((TcpIp_SizeOfSocketDynType)(SOCKET_ID))
# define TCPIP_SOCKET_IDX_TO_ID(SOCKET_IDX)  ((TcpIp_SocketIdType)(SOCKET_IDX))

# define TCPIP_SOCKET_IDX_TO_UDP_IDX(SOCKET_IDX)   ((TcpIp_SizeOfSocketUdpDynType)(SOCKET_IDX))
# define TCPIP_UDP_IDX_TO_SOCKET_IDX(UDP_IDX)      ((TcpIp_SizeOfSocketDynType)(UDP_IDX))

# define TCPIP_SOCKET_IDX_TO_TCP_IDX(SOCKET_IDX)  ((TcpIp_SizeOfSocketTcpDynType)((SOCKET_IDX) - TcpIp_GetSizeOfSocketUdpDyn()))
# define TCPIP_TCP_IDX_TO_SOCKET_IDX(TCP_IDX)     ((TcpIp_SizeOfSocketDynType)((TCP_IDX)       + TcpIp_GetSizeOfSocketUdpDyn()))

# define TCPIP_SOCKET_IDX_IS_TCP(SOCKET_IDX)      ((SOCKET_IDX) >= TcpIp_GetSizeOfSocketUdpDyn())  /*! Checks if a valid SocketId references a TCP socket. (Use TCPIP_COND_VALID_SOCKET_ID() in order to check if the socket ID is valid at all.) */
# define TCPIP_SOCKET_IDX_IS_UDP(SOCKET_IDX)      ((SOCKET_IDX) <  TcpIp_GetSizeOfSocketUdpDyn())  /*! Checks if a valid SocketId references a UDP socket.  */

# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
#  define TCPIP_IPV4_SOCKET_TO_SOCKET(IpV4Socket_IDX)         ((TcpIp_SocketDynIterType)IpV4Socket_IDX)
#  define TCPIP_SOCKET_TO_IPV4_SOCKET(IpV4Socket_IDX)         ((TcpIp_SizeOfIpV4SocketDynType)IpV4Socket_IDX)
# endif
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
#  define TCPIP_IPV6_SOCKET_TO_SOCKET(IpV6Socket_IDX)         ((TcpIp_SocketDynIterType)IpV6Socket_IDX)
#  define TCPIP_SOCKET_TO_IPV6_SOCKET(IpV6Socket_IDX)         ((TcpIp_SizeOfIpV6SocketDynType)IpV6Socket_IDX)
# endif


# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
/* */
# else
#  define TcpIp_GetSizeOfLocalAddrV4() 0u
# endif

/*
Datatype                                Variable Name      Description
uint8                                   CtrlIdx            EthIf Controller Index (AUTOSAR)
TcpIp_SizeOfIpV4CtrlType                IpV4CtrlIdx        IpV4/IpV6 controller index
TcpIp_SizeOfIpV6CtrlType                IpV6CtrlIdx        IpV4/IpV6 controller index

TcpIp_LocalAddrIdType                   LocalAddrId        Local IP Address Id (AUTOSAR)
TcpIp_SizeOfLocalAddrType               LocalAddrIdx       All IPv4 addresses + All IPv6 addresses

TcpIp_SizeOfLocalAddrV4Type             LocalAddrV4Idx     All IPv4 addresses
TcpIp_SizeOfUnicastAddrV4Type           SrcAddrIdx         All Unicast/Source IPv4 addresses
TcpIp_SizeOfMulticastAddrV4Type         McAddrIdx          All Multicast IPv4 addresses
TcpIp_SizeOfDefaultUnicastAddrV4Type    DefaultUcAddrIdx   All Unicast IPv4 address default values
TcpIp_SizeOfDefaultMulticastAddrV4Type  DefaultMcAddrIdx   All Multicast IPv4 address default values

TcpIp_SizeOfLocalAddrV6Type             LocalAddrV6Idx     All IPv6 addresses
TcpIp_SizeOfIpV6SourceAddressType       SrcAddrIdx         All Unicast/Source IPv6 addresses
TcpIp_SizeOfIpV6MulticastAddrType       McAddrIdx          All Multicast IPv6 addresses
TcpIp_SizeOfDefaultAddrV6Type           DefAddrIdx         All IPv6 address default values

TcpIp_SocketIdType                      SocketId           Socket Id (AUTOSAR)
TcpIp_SizeOfSocketType                  SocketIdx          All UDP sockets + All TCP sockets
TcpIp_SizeOfSocketTcpType               SocketTcpIdx       All TCP sockets
TcpIp_SizeOfSocketUdpType               SocketUdpIdx       All UDP sockets


AUTOSAR:                CtrlIdx                       LocalAddrId                      SocketId
                           |                               |                               |
                           |                               |                               |
TcpIp:                     |                          LocalAddrIdx                     SocketIdx
                     ______|______                  _______|_______                 _______|______
                    |             |                |               |               |              |
IpVX/Tcp/Udp:  IpV4CtrlIdx   IpV6CtrlIdx    LocalAddrV4Idx   LocalAddrV6Idx   SocketTcpIdx   SocketUdpIdx


Name           | Valid Check                                         | Conversion to (and back)                            | Target
=====================================================================|=====================================================|==================
Controller:
----------------------------------------------------------------------------------------------------------------------------------------------
CtrlIdx        | TcpIp_IsIpV4CtrlUsedOfEthIfCtrl                     | TcpIp_GetIpV4CtrlIdxOfEthIfCtrl                     | IpV4CtrlIdx
               |                                                     | TcpIp_GetEthIfCtrlIdxOfIpV4Ctrl                     |
----------------------------------------------------------------------------------------------------------------------------------------------
CtrlIdx        | TcpIp_IsIpV6CtrlUsedOfEthIfCtrl                     | TcpIp_GetIpV6CtrlIdxOfEthIfCtrl                     | IpV6CtrlIdx
               |                                                     | TcpIp_GetEthIfCtrlIdxOfIpV6Ctrl                     |
----------------------------------------------------------------------------------------------------------------------------------------------

IP Addresses:
----------------------------------------------------------------------------------------------------------------------------------------------
LocalAddrId    | TCPIP_LOCAL_ADDR_ID_IS_VALID                        | TCPIP_LOCAL_ADDR_ID_TO_IDX                          | LocalAddrIdx
               |                                                     | TCPIP_LOCAL_ADDR_IDX_TO_ID                          |
----------------------------------------------------------------------------------------------------------------------------------------------


IPv4 Addresses:
----------------------------------------------------------------------------------------------------------------------------------------------
LocalAddrIdx   | TCPIP_LOCAL_ADDR_IDX_IS_IPV4                        | TCPIP_LOCAL_ADDR_IDX_TO_IPV4                        | LocalAddrV4Idx
               |                                                     | TCPIP_IPV4_TO_LOCAL_ADDR_IDX                        |
----------------------------------------------------------------------------------------------------------------------------------------------
LocalAddrV4Idx | TcpIp_IsUnicastAddrV4UsedOfLocalAddrV4              | TcpIp_GetUnicastAddrV4IdxOfLocalAddrV4              | SrcAddrIdx
               |                                                     | X                                                   |
----------------------------------------------------------------------------------------------------------------------------------------------
LocalAddrV4Idx | TcpIp_IsMulticastAddrV4UsedOfLocalAddrV4            | TcpIp_GetMulticastAddrV4IdxOfLocalAddrV4            | McAddrIdx
               |                                                     | TcpIp_GetLocalAddrV4IdxOfMulticastAddrV4            |
----------------------------------------------------------------------------------------------------------------------------------------------
SrcAddrIdx     | TcpIp_IsDefaultUnicastAddrV4UsedOfUnicastAddrV4     | TcpIp_GetDefaultUnicastAddrV4IdxOfUnicastAddrV4     | DefaultUcAddrIdx
               |                                                     | X                                                   |
----------------------------------------------------------------------------------------------------------------------------------------------
McAddrIdx      | TcpIp_IsDefaultMulticastAddrV4UsedOfMulticastAddrV4 | TcpIp_GetDefaultMulticastAddrV4IdxOfMulticastAddrV4 | DefaultMcAddrIdx
               |                                                     | X                                                   |
----------------------------------------------------------------------------------------------------------------------------------------------

IPv6 Addresses:
----------------------------------------------------------------------------------------------------------------------------------------------
LocalAddrIdx   | TCPIP_LOCAL_ADDR_IDX_IS_IPV6                        | TCPIP_LOCAL_ADDR_IDX_TO_IPV6                        | LocalAddrV6Idx
               |                                                     | TCPIP_IPV6_TO_LOCAL_ADDR_IDX                        |
----------------------------------------------------------------------------------------------------------------------------------------------
LocalAddrV6Idx | TcpIp_IsIpV6SourceAddressUsedOfLocalAddrV6          | TcpIp_GetIpV6SourceAddressIdxOfLocalAddrV6          | SrcAddrIdx
               |                                                     | TcpIp_GetLocalAddrV6IdxOfIpV6SourceAddress          |
----------------------------------------------------------------------------------------------------------------------------------------------
LocalAddrV6Idx | TcpIp_IsIpV6MulticastAddrUsedOfLocalAddrV6          | TcpIp_GetIpV6MulticastAddrIdxOfLocalAddrV6          | McAddrIdx
               |                                                     | TcpIp_GetLocalAddrV6IdxOfIpV6MulticastAddr          |
----------------------------------------------------------------------------------------------------------------------------------------------
SrcAddrIdx     | TcpIp_IsDefaultAddrV6UsedOfIpV6SourceAddress        | TcpIp_GetDefaultAddrV6IdxOfIpV6SourceAddress        | DefAddrIdx
               |                                                     | X                                                   |
----------------------------------------------------------------------------------------------------------------------------------------------
McAddrIdx      | TcpIp_IsDefaultAddrV6UsedOfIpV6MulticastAddr        | TcpIp_GetDefaultAddrV6IdxOfIpV6MulticastAddr        | DefAddrIdx
               |                                                     | X
----------------------------------------------------------------------------------------------------------------------------------------------

Sockets:
----------------------------------------------------------------------------------------------------------------------------------------------
SocketId       | TCPIP_SOCKET_ID_IS_VALID                            | TCPIP_SOCKET_ID_TO_IDX                              | SocketIdx
               |                                                     | TCPIP_SOCKET_IDX_TO_ID                              |
----------------------------------------------------------------------------------------------------------------------------------------------
SocketIdx      | TCPIP_SOCKET_IDX_IS_TCP                             | TCPIP_SOCKET_IDX_TO_TCP_IDX                         | SocketTcpIdx
               |                                                     | TCPIP_TCP_IDX_TO_SOCKET_IDX                         |
----------------------------------------------------------------------------------------------------------------------------------------------
SocketIdx      | TCPIP_SOCKET_IDX_IS_UDP                             | TCPIP_SOCKET_IDX_TO_UDP_IDX                         | SocketUdpIdx
               |                                                     | TCPIP_UDP_IDX_TO_SOCKET_IDX                         |
----------------------------------------------------------------------------------------------------------------------------------------------
*/

# define TCPIP_LOCAL_ADDR_ID_TO_IDX(LOCAL_ADDR_ID)           (LOCAL_ADDR_ID)
# define TCPIP_LOCAL_ADDR_IDX_TO_ID(LOCAL_ADDR_IDX)          ((TcpIp_SizeOfLocalAddrType)(LOCAL_ADDR_IDX))
# define TCPIP_LOCAL_ADDR_ID_IS_VALID(LOCAL_ADDR_ID)         ((LOCAL_ADDR_ID) < TCPIP_LOCAL_ADDR_IDX_COUNT)
# define TCPIP_LOCAL_ADDR_ID_IS_VALID_OR_ANY(LOCAL_ADDR_ID)  (TCPIP_LOCAL_ADDR_ID_IS_VALID(LOCAL_ADDR_ID) || ((LOCAL_ADDR_ID) == TCPIP_LOCALADDRID_ANY))

# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
#  if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
/* IPv4 and IPv6 */
#   define TCPIP_LOCAL_ADDR_IDX_COUNT                         ((TcpIp_SizeOfLocalAddrType)((TcpIp_GetSizeOfLocalAddrV4() + TcpIp_GetSizeOfLocalAddrV6())))
#   define TCPIP_LOCAL_ADDR_IDX_IS_IPV4(LOCAL_ADDR_IDX)       ((LOCAL_ADDR_IDX) <  TcpIp_GetSizeOfLocalAddrV4())        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define TCPIP_LOCAL_ADDR_IDX_IS_IPV6(LOCAL_ADDR_IDX)       (((LOCAL_ADDR_IDX) >= TcpIp_GetSizeOfLocalAddrV4()) && ((LOCAL_ADDR_IDX) < (TcpIp_LocalAddrIterType)(TcpIp_GetSizeOfLocalAddrV4() + TcpIp_GetSizeOfLocalAddrV6())))
#   define TCPIP_LOCAL_ADDR_IDX_TO_IPVX(LOCAL_ADDR_IDX)       ((TcpIp_SizeOfLocalAddrType)((TCPIP_LOCAL_ADDR_IDX_IS_IPV4(LOCAL_ADDR_IDX) ? (LOCAL_ADDR_IDX) : ((LOCAL_ADDR_IDX) - TcpIp_GetSizeOfLocalAddrV4()))))
#   define TCPIP_LOCAL_ADDR_IDX_TO_IPV4(LOCAL_ADDR_IDX)       (LOCAL_ADDR_IDX)
#   define TCPIP_LOCAL_ADDR_IDX_TO_IPV6(LOCAL_ADDR_IDX)       ((TcpIp_SizeOfLocalAddrV6Type)((LOCAL_ADDR_IDX) - TcpIp_GetSizeOfLocalAddrV4()))
#   define TCPIP_IPV4_TO_LOCAL_ADDR_IDX(IP_ADDR_IDX)          ((TcpIp_SizeOfLocalAddrType)(IP_ADDR_IDX))
#   define TCPIP_IPV6_TO_LOCAL_ADDR_IDX(IP_ADDR_IDX)          ((TcpIp_SizeOfLocalAddrType)((IP_ADDR_IDX) + TcpIp_GetSizeOfLocalAddrV4()))
#   define TCPIP_IS_ADDR_FAMILY_SUPPORTED(DOMAIN_TYPE) (((DOMAIN_TYPE) == TCPIP_AF_INET) || ((DOMAIN_TYPE) == TCPIP_AF_INET6))
#  else
/* IPv4 only */
#   define TCPIP_LOCAL_ADDR_IDX_COUNT                         TcpIp_GetSizeOfLocalAddrV4()
#   define TCPIP_LOCAL_ADDR_IDX_IS_IPV4(LOCAL_ADDR_IDX)       ((LOCAL_ADDR_IDX) <  TcpIp_GetSizeOfLocalAddrV4())        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#   define TCPIP_LOCAL_ADDR_IDX_IS_IPV6(LOCAL_ADDR_IDX)       (TRUE  == FALSE)
#   define TCPIP_LOCAL_ADDR_IDX_TO_IPVX(LOCAL_ADDR_IDX)       (LOCAL_ADDR_IDX)
#   define TCPIP_LOCAL_ADDR_IDX_TO_IPV4(LOCAL_ADDR_IDX)       (LOCAL_ADDR_IDX)
#   define TCPIP_IPV4_TO_LOCAL_ADDR_IDX(IP_ADDR_IDX)          ((TcpIp_SizeOfLocalAddrType)(IP_ADDR_IDX))
#   define TCPIP_IS_ADDR_FAMILY_SUPPORTED(DOMAIN_TYPE) ((DOMAIN_TYPE) == TCPIP_AF_INET)
#  endif
# else
/* IPv6 only */
#  define TCPIP_LOCAL_ADDR_IDX_COUNT                         TcpIp_GetSizeOfLocalAddrV6()
#  define TCPIP_LOCAL_ADDR_IDX_IS_IPV4(LOCAL_ADDR_IDX)       (TRUE == FALSE)
#  define TCPIP_LOCAL_ADDR_IDX_IS_IPV6(LOCAL_ADDR_IDX)       ((TCPIP_LOCAL_ADDR_ID_IS_VALID(TCPIP_LOCAL_ADDR_IDX_TO_ID(LOCAL_ADDR_IDX))))
#  define TCPIP_LOCAL_ADDR_IDX_TO_IPVX(LOCAL_ADDR_IDX)       (LOCAL_ADDR_IDX)
#  define TCPIP_LOCAL_ADDR_IDX_TO_IPV6(LOCAL_ADDR_IDX)       (LOCAL_ADDR_IDX)
#  define TCPIP_IPV6_TO_LOCAL_ADDR_IDX(IP_ADDR_IDX)          ((TcpIp_SizeOfLocalAddrV6Type)(IP_ADDR_IDX))
#  define TCPIP_IS_ADDR_FAMILY_SUPPORTED(DOMAIN_TYPE) ((DOMAIN_TYPE) == TCPIP_AF_INET6)
# endif

# define TCPIP_SOCKET_IDX_INV                                   TcpIp_GetSizeOfSocketDyn()
# define TCPIP_SOCKET_ID_IS_VALID(SOCKET_ID)                    ((SOCKET_ID) < TcpIp_GetSizeOfSocketDyn())
# define TCPIP_SOCKET_VX_IDX_IS_VALID_SOCKET_IDX(SOCKET_VX_IDX) ((SOCKET_VX_IDX) < TcpIp_GetSizeOfSocketDyn())

# define TCPIP_PORT_NOT_SET                               0x0000u
# define TCPIP_DYNAMIC_PORT_COUNT                         0x3FFFu
# define TCPIP_DYNAMIC_PORT_START                         0xC000u


# define TCPIP_V_IP_TRANSMIT_FLAG_TX_CONFIRMATION         1u
# define TCPIP_V_IP_TRANSMIT_FLAG_RETRY_ON_PHYS_ADDR_MISS 2u

/* TcpIp TxBuf */

# define TCPIP_DLIST_GET_NEXT(NodeIdx, DList) ((DList).DescPtr->NodePtr[(NodeIdx)].NextIdx)                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define TCPIP_DLIST_GET_PREV(NodeIdx, DList) ((DList).DescPtr->NodePtr[(NodeIdx)].PrevIdx)                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# define TCPIP_DLIST_IS_EMPTY(DList)           (TCPIP_DLIST_END_IDX == (DList).FirstIdx)                                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# define TCPIP_DLIST_END_IDX 0xFFu

/* TCP */

/* RAM array access macros: */

/* RxBuffers */
# define TcpIp_Tcp_GetRxBuffersElemPtr(SocketTcpIdx, IndexInsideRxBuffer)  (TcpIp_GetAddrTcpRxBuffer(  \
                    TcpIp_GetTcpRxBufferStartIdxOfTcpRxBufferDesc(TcpIp_GetTcpRxBufferDescIdxOfSocketTcpDyn(SocketTcpIdx)) +  \
                    (IndexInsideRxBuffer)))                                                                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Development Error Tracer */
# if ( TCPIP_DEV_ERROR_REPORT == STD_ON )
#  define TcpIp_CheckDetErrorReturnVoid(CONDITION, API_ID, ERROR_CODE)           { if(!(CONDITION)) {    \
    (void)Det_ReportError( TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, (API_ID), (ERROR_CODE)); return; } }
#  define TcpIp_CheckDetErrorReturnValue(CONDITION, API_ID, ERROR_CODE, RET_VAL)  { if(!(CONDITION)) {    \
    (void)Det_ReportError( TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, (API_ID), (ERROR_CODE)); return (RET_VAL); } }
#  define TcpIp_CheckDetErrorContinue(CONDITION, API_ID, ERROR_CODE)           { if(!(CONDITION)) {    \
    (void)Det_ReportError( TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, (API_ID), (ERROR_CODE)); } }
#  define TcpIp_CallDetReportError(API_ID, ERROR_CODE)  (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, (API_ID), (ERROR_CODE))
# else
#  define TcpIp_CheckDetErrorReturnVoid(  CONDITION, API_ID, ERROR_CODE )
#  define TcpIp_CheckDetErrorReturnValue( CONDITION, API_ID, ERROR_CODE, RET_VAL )
#  define TcpIp_CheckDetErrorContinue(    CONDITION, API_ID, ERROR_CODE )
#  define TcpIp_CallDetReportError(                  API_ID, ERROR_CODE )
# endif /* TCPIP_DEV_ERROR_REPORT */

/* CHECK CONDITIONS to be used in ASSERT and DET macros: */

# define TCPIP_IPADDR_ASSIGNMENT_MAX_VALUE  TCPIP_IPADDR_ASSIGNMENT_IPV6_ROUTER

/* implementation limit: max message length is 64kB */
# define TCPIP_COND_LEN_MAX_UINT16(Len)      ( (uint32)(Len) <= (uint32)0xFFFFu )        /* TCPIP_E_INV_ARG */          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# define TCPIP_COND_VALID_DOMAIN(Domain)      ( (IPBASE_AF_INET == (Domain)) || \
                                               (IPBASE_AF_INET6 == (Domain)))          /* TCPIP_E_INV_ARG */            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# if (TCPIP_SUPPORT_TCP == STD_ON)
#  define TCPIP_COND_VALID_PROT(Prot)          ( (TCPIP_SOCK_PROT_UDP == (Prot)) || \
                                               (TCPIP_SOCK_PROT_TCP == (Prot)) )       /* TCPIP_E_INV_ARG */            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define TCPIP_COND_VALID_PROT(Prot)          ( TCPIP_SOCK_PROT_UDP == (Prot) )         /* TCPIP_E_INV_ARG */          /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif
# define TCPIP_TCP_COND_VALID_TCPIDX(SocketTcpIdx)   (TcpIp_GetSizeOfSocketTcpDyn() > (SocketTcpIdx))  /* TCPIP_E_INV_SOCK_ID */ /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# define TCPIP_ASSERT(CONDITION)                TCPIP_ASSERT_MSG(CONDITION, NULL_PTR)
# define TCPIP_ASSERT_UNREACHABLE()             TCPIP_ASSERT_UNREACHABLE_MSG("This statement must not be reached!")
# if defined(TCPIP_ASSERT_HANDLER)                                                                                      /* COV_TCPIP_CANOE_DEBUG */
#  define TCPIP_ASSERT_MSG(CONDITION, MSG)                (void)TCPIP_ASSERT_HANDLER(CONDITION, MSG)
#  define TCPIP_ASSERT_UNREACHABLE_MSG(MSG)               (void)TCPIP_ASSERT_HANDLER(FALSE, MSG)
# else
#  define TCPIP_ASSERT_MSG(CONDITION, MSG)
#  define TCPIP_ASSERT_UNREACHABLE_MSG(MSG)
# endif

# define TCPIP_CYCLES_PER_SECOND             (1000u / TCPIP_MAIN_FCT_PERIOD_MSEC)
# define TCPIP_MAX_SEC_IN_UINT32_CYCLE_VAR   (0xFFFFFFFFu / TCPIP_CYCLES_PER_SECOND)

/* UDP */
/* Configuration access (depending on configuration variant) */

/* The following macros write unsigned integers into a linear buffer. (host byte order -> network byte order). */

# define TCPIP_PUT_UINT8(Buffer, Offset, Value)          ((Buffer)[(Offset)] = (Value))
# define TCPIP_PUT_UINT16(Buffer, Offset, Value)         TcpIp_WriteUInt16((Buffer), (Offset), (Value))
# define TCPIP_PUT_UINT32(Buffer, Offset, Value)         TcpIp_WriteUInt32((Buffer), (Offset), (Value))

/* The following macros write unsigned integers into a linear buffer without modifying the byte order. */

# if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)                                                                                /* COV_TCPIP_BYTE_ORDER */
#  define TCPIP_PUT_UINT16_RAW(Buffer, Offset, Value)     TcpIp_WriteUInt16((Buffer), (Offset), (Value))
#  define TCPIP_PUT_UINT32_RAW(Buffer, Offset, Value)     TcpIp_WriteUInt32((Buffer), (Offset), (Value))
# else
#  define TCPIP_PUT_UINT16_RAW(Buffer, Offset, Value)     TcpIp_WriteUInt16Le((Buffer), (Offset), (Value))
#  define TCPIP_PUT_UINT32_RAW(Buffer, Offset, Value)     TcpIp_WriteUInt32Le((Buffer), (Offset), (Value))
# endif

/* The following macros read unsigned integers from a linear (buffer network byte order -> host byte order). */

# define TCPIP_GET_UINT8(Buffer, Offset)                                       ((uint8)(Buffer)[(Offset)])

# define TCPIP_GET_UINT16(Buffer, Offset)                                     ((uint16)(((uint16)((Buffer)[(Offset)]) << 8) | \
                                                                                        ((uint16)((Buffer)[(Offset) + 1u]))))

# define TCPIP_GET_UINT32(Buffer, Offset)                                     ((uint32)(((uint32)((Buffer)[(Offset)]) << 24) | \
                                                                                        ((uint32)((Buffer)[(Offset) + 1u]) << 16) | \
                                                                                        ((uint32)((Buffer)[(Offset) + 2u]) <<  8) | \
                                                                                        ((uint32)((Buffer)[(Offset) + 3u]))))

/* The following macros read unsigned integers from a linear buffer without modifying the byte order. */

# if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)                                                                                /* COV_TCPIP_BYTE_ORDER */
#  define TCPIP_GET_UINT16_RAW(Buffer, Offset)    TCPIP_GET_UINT16(Buffer, Offset)
#  define TCPIP_GET_UINT32_RAW(Buffer, Offset)    TCPIP_GET_UINT32(Buffer, Offset)
# else
#  define TCPIP_GET_UINT16_RAW(Buffer, Offset)                                 ((uint16)(((uint16)((Buffer)[(Offset) + 1u]) << 8) | \
                                                                                        ((uint16)((Buffer)[(Offset)]))) )

#  define TCPIP_GET_UINT32_RAW(Buffer, Offset)                                 ((uint32)(((uint32)((Buffer)[(Offset) + 3u]) << 24) | \
                                                                                        ((uint32)((Buffer)[(Offset) + 2u]) << 16) | \
                                                                                        ((uint32)((Buffer)[(Offset) + 1u]) <<  8) | \
                                                                                        ((uint32)((Buffer)[(Offset)]))))
# endif

# define TCPIP_NTOHS(UINT16_VALUE) IPBASE_NTOH16(UINT16_VALUE)
# define TCPIP_HTONS(UINT16_VALUE) IPBASE_HTON16(UINT16_VALUE)
# define TCPIP_NTOHL(UINT32_VALUE) IPBASE_NTOH32(UINT32_VALUE)
# define TCPIP_HTONL(UINT32_VALUE) IPBASE_HTON32(UINT32_VALUE)

# define TCPIP_MIN(VALUE1, VALUE2)                                   (((VALUE1) < (VALUE2)) ? (VALUE1) : (VALUE2))

/* The following macros are used to read and set crypto operation modes. */
# define TCPIP_IS_FLAG_SET(Map, Flag) (((Map) & (Flag)) == (Flag))
# define TCPIP_SET_FLAG(Map, Flag) ((Map) |= (Flag))
/* If the flag to remove is not set, TCPIP_REMOVE_FLAG will do nothing which is the desired behavior. */
# define TCPIP_REMOVE_FLAG(Map, Flag) ((Map) &= ~(Flag))

# define TCPIP_IN_RANGE(Value, StartValue, EndValue) ((Value >= StartValue) && (Value < EndValue))

/* PRQA L:FunctionLikeMacros */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

# define TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

extern VAR(TcpIp_TcpIpErrorType, TCPIP_VAR_NO_INIT) TcpIp_LastSockError;

# define TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ----- */
# define TCPIP_START_SEC_VAR_NO_INIT_8
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


# if (TCPIP_VENABLE_CANOE_WRITE_STRING == STD_ON)                                                                       /* COV_TCPIP_CANOE_DEBUG */
extern VAR(sint8, TCPIP_VAR_NO_INIT)                TcpIp_CanoeWriteStr[];
# endif

# define TCPIP_STOP_SEC_VAR_NO_INIT_8
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ----- */
# define TCPIP_START_SEC_VAR_NO_INIT_16
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (TCPIP_SUPPORT_TCP == STD_ON)
extern VAR(uint16, TCPIP_VAR_NO_INIT)               TcpIp_Tcp_DynamicPortCount;
# endif
extern VAR(uint16, TCPIP_VAR_NO_INIT)               TcpIp_Udp_DynamicPortCount;

# define TCPIP_STOP_SEC_VAR_NO_INIT_16
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ----- */
# define TCPIP_START_SEC_CONST_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
# include "TcpIp_MemMap.h"

# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
extern CONST(IpBase_AddrIn6Type, TCPIP_CONST) TcpIp_IpV6AddrUnspecified;                                                /* PRQA S 0759 */ /* MD_MSR_Union */
# endif

# define TCPIP_STOP_SEC_CONST_UNSPECIFIED
/* PRQA S 5087 1 */ /* MD_MSR_19.1 */
# include "TcpIp_MemMap.h"

# define TCPIP_START_SEC_CODE
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if !defined (STATIC)                                                                                                  /* COV_TCPIP_COMPATIBILITY */
#  define STATIC static
# endif

# if !defined (TCPIP_LOCAL)                                                                                             /* COV_TCPIP_COMPATIBILITY */
#  define TCPIP_LOCAL static
# endif

# if !defined (TCPIP_LOCAL_INLINE)                                                                                      /* COV_TCPIP_COMPATIBILITY */
#  define TCPIP_LOCAL_INLINE LOCAL_INLINE
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * TcpIp_WriteUInt16()
 *********************************************************************************************************************/
/*! \brief          Writes a 16 bit unsigned integer at the given position into a byte buffer.
 *  \details        The value is written in network byte order.
 *  \param[out]     BufferPtr   Target buffer.
 *                              \spec requires BufferPtr != NULL_PTR; \endspec
 *                              \spec requires $lengthOf(BufferPtr) >= (BufferPos + 2); \endspec
 *  \param[in]      BufferPos   Position in target buffer. (bytes)
 *                              CONSTRAINT: [0...sizeof(Buffer)-2]
 *  \param[in]      Value       Value that shall be written into the buffer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_WriteUInt16(TCPIP_P2V(uint8) BufferPtr, uint16_least BufferPos, uint16 Value);

/**********************************************************************************************************************
 * TcpIp_WriteUInt32()
 *********************************************************************************************************************/
/*! \brief          Writes a 32 bit unsigned integer at the given position into a byte buffer.
 *  \details        The value is written in network byte order.
 *  \param[out]     BufferPtr   Target buffer.
 *                              \spec requires BufferPtr != NULL_PTR; \endspec
 *                              \spec requires $lengthOf(BufferPtr) >= (BufferPos + 4); \endspec
 *  \param[in]      BufferPos   Position in target buffer. (bytes)
 *                              CONSTRAINT: [0...sizeof(Buffer)-4]
 *  \param[in]      Value       Value that shall be written into the buffer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_WriteUInt32(TCPIP_P2V(uint8) BufferPtr, uint16_least BufferPos, uint32 Value);

# if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)                                                                                /* COV_TCPIP_BYTE_ORDER */
/* No conversion of data types required if platform uses network byte order. */
# else
/**********************************************************************************************************************
 * TcpIp_WriteUInt16Le()
 *********************************************************************************************************************/
/*! \brief          Writes a 16 bit unsigned integer at the given position into a byte buffer.
 *  \details        The value is written in little endian byte order.
 *  \param[out]     BufferPtr   Target buffer.
 *                              \spec requires BufferPtr != NULL_PTR; \endspec
 *  \param[in]      BufferPos   Position in target buffer. (bytes)
 *                              \spec requires $lengthOf(BufferPtr) >= (BufferPos + 2); \endspec
 *  \param[in]      Value       Value that shall be written into the buffer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_WriteUInt16Le(TCPIP_P2V(uint8) BufferPtr, uint16_least BufferPos, uint16 Value);

/**********************************************************************************************************************
 * TcpIp_WriteUInt32Le()
 *********************************************************************************************************************/
/*! \brief          Writes a 32 bit unsigned integer at the given position into a byte buffer.
 *  \details        The value is written in little endian byte order.
 *  \param[out]     BufferPtr   Target buffer.
 *                              \spec requires BufferPtr != NULL_PTR; \endspec
 *  \param[in]      BufferPos   Position in target buffer. (bytes)
 *                              \spec requires $lengthOf(BufferPtr) >= (BufferPos + 4); \endspec
 *  \param[in]      Value       Value that shall be written into the buffer.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_WriteUInt32Le(TCPIP_P2V(uint8) BufferPtr, uint16_least BufferPos, uint32 Value);
# endif

/**********************************************************************************************************************
 * TcpIp_VCopyPhysAddr()
 *********************************************************************************************************************/
/*! \brief          Copies a 6 byte long physical (link layer) address from source to target.
 *  \details        -
 *  \param[out]     DstAddrPtr   Destination buffer into which the address is copied.
 *                               \spec requires $lengthOf(DstAddrPtr) >= IPBASE_ETH_PHYS_ADDR_LEN_BYTE; \endspec
 *  \param[in]      SrcAddrPtr   Source buffer from which the address is copied.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_VCopyPhysAddr(TCPIP_P2V(uint8) DstAddrPtr, TCPIP_P2C(uint8) SrcAddrPtr);

/**********************************************************************************************************************
 * TcpIp_VClearPhysAddr()
 *********************************************************************************************************************/
/*! \brief          Set all 6 bytes of the physical address to 0u
 *  \details        -
 *  \param[out]     DstAddrPtr   Destination buffer into which the address is set to 0u.
 *                               \spec requires $lengthOf(DstAddrPtr) >= IPBASE_ETH_PHYS_ADDR_LEN_BYTE; \endspec
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_VClearPhysAddr(TCPIP_P2V(uint8) DstAddrPtr);
/* PRQA S 3219 InlineFunction */ /* MD_MSR_8.10 */

# if !defined(NO_INLINE_FUNCTION_DEFINITIONS)                                                                           /* COV_TCPIP_UNIT_TEST */

/**********************************************************************************************************************
 * TcpIp_WriteUInt16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_WriteUInt16(TCPIP_P2V(uint8) BufferPtr, uint16_least BufferPos, uint16 Value)
{
  /* #10 Copy the two bytes of the 16 bit value into the buffer in network byte order. */
  BufferPtr[BufferPos] = (uint8)(Value >> 8);                                                                           /* VCA_TCPIP_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION */
  BufferPtr[BufferPos + 1u] = (uint8)(Value);                                                                           /* VCA_TCPIP_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION */
}

/**********************************************************************************************************************
 * TcpIp_WriteUInt32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_WriteUInt32(TCPIP_P2V(uint8) BufferPtr, uint16_least BufferPos, uint32 Value)
{
  /* #10  Copy the four bytes of the 32 bit value into the buffer in network byte order. */
  BufferPtr[BufferPos] = (uint8)(Value >> 24);                                                                          /* VCA_TCPIP_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION */
  BufferPtr[BufferPos + 1u] = (uint8)(Value >> 16);                                                                     /* VCA_TCPIP_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION */
  BufferPtr[BufferPos + 2u] = (uint8)(Value >>  8);                                                                     /* VCA_TCPIP_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION */
  BufferPtr[BufferPos + 3u] = (uint8)(Value);                                                                           /* VCA_TCPIP_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION */
}

#  if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)                                                                               /* COV_TCPIP_BYTE_ORDER */
/* No conversion of datatypes required if platform uses network byte order. */
#  else
/**********************************************************************************************************************
 * TcpIp_WriteUInt16Le()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_WriteUInt16Le(TCPIP_P2V(uint8) BufferPtr, uint16_least BufferPos, uint16 Value)
{
  /* #10 Copy the two bytes of the 16 bit value into the buffer in little endian byte order. */
  BufferPtr[BufferPos + 1u] = (uint8)(Value >> 8);
  BufferPtr[BufferPos] = (uint8)(Value);
}

/**********************************************************************************************************************
 * TcpIp_WriteUInt32Le()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_WriteUInt32Le(TCPIP_P2V(uint8) BufferPtr, uint16_least BufferPos, uint32 Value)
{
  /* #10 Copy the four bytes of the 32 bit value into the buffer in little endian byte order. */
  BufferPtr[BufferPos + 3u] = (uint8)(Value >> 24);
  BufferPtr[BufferPos + 2u] = (uint8)(Value >> 16);
  BufferPtr[BufferPos + 1u] = (uint8)(Value >>  8);
  BufferPtr[BufferPos] = (uint8)(Value);
}
#  endif

/**********************************************************************************************************************
 *  TcpIp_VCopyPhysAddr
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_VCopyPhysAddr(TCPIP_P2V(uint8) DstAddrPtr, TCPIP_P2C(uint8) SrcAddrPtr)
{
  TCPIP_ASSERT(SrcAddrPtr != NULL_PTR);
  TCPIP_ASSERT(DstAddrPtr != NULL_PTR);
  TCPIP_VENTER_CRITICAL_SECTION();

  /* #10 Do a byte-wise copy of the 6 physical address bytes. */
  DstAddrPtr[0] = SrcAddrPtr[0];
  DstAddrPtr[1] = SrcAddrPtr[1];
  DstAddrPtr[2] = SrcAddrPtr[2];
  DstAddrPtr[3] = SrcAddrPtr[3];
  DstAddrPtr[4] = SrcAddrPtr[4];
  DstAddrPtr[5] = SrcAddrPtr[5];

  TCPIP_VLEAVE_CRITICAL_SECTION();
} /* TcpIp_VCopyPhysAddr() */

/**********************************************************************************************************************
 *  TcpIp_VClearPhysAddr
 **********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) TcpIp_VClearPhysAddr(TCPIP_P2V(uint8) DstAddrPtr)
{
  const uint8 emptyAddr[6u] = { 0u };
  /* #10 Copy empty address to buffer */
  TcpIp_VCopyPhysAddr(DstAddrPtr, &emptyAddr[0u]);
} /* TcpIp_VClearPhysAddr() */

/* PRQA L:InlineFunction */

# endif

/**********************************************************************************************************************
 * MISCELLANEOUS SERVICE FUNCTIONS - START
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * TcpIp_MemCpySafe()
 *********************************************************************************************************************/
/*! \brief          Copies data from the given source pointer to the target pointer.
 *  \details        This function is a wrapper for VStdMemCpy with additional checks of the input parameter.
 *  \param[out]     TargetPtr    Pointer to the target buffer.
 *                               \spec requires $lengthOf(TargetPtr) >= TargetSize; \endspec
 *                               \spec requires TargetPtr != NULL_PTR; \endspec
 *  \param[in]      TargetPos    Buffer position where the data should be written to.
 *  \param[in]      TargetSize   Size of the target buffer.
 *  \param[in]      SourcePtr    Pointer to the source buffer.
 *  \param[in]      Count        Amount of data which should be written.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different TargetPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_MemCpySafe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  TCPIP_P2C(uint8) SourcePtr,
  uint32           Count);

/**********************************************************************************************************************
 * TcpIp_MemCpyFromConstSegmentSafe()
 *********************************************************************************************************************/
/*! \brief          Copies data from the given source segment to the target pointer.
 *  \details        -
 *  \param[out]     TargetPtr    Pointer to the target buffer.
 *                               \spec requires $lengthOf(TargetPtr) >= TargetSize; \endspec
 *                               \spec requires TargetPtr != NULL_PTR; \endspec
 *  \param[in]      TargetPos    Buffer position where the data should be written to.
 *  \param[in]      TargetSize   Size of the target buffer.
 *  \param[in]      SourceSegPtr Pointer to the source buffer segment.
 *  \param[in]      Count        Amount of data which should be written.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different TargetPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_MemCpyFromConstSegmentSafe(
  TCPIP_P2V(uint8)          TargetPtr,
  uint32                    TargetPos,
  uint32                    TargetSize,
  TcpIp_ConstSegmentPtrType SourceSegPtr,
  uint32                    Count);


/**********************************************************************************************************************
 * TcpIp_GetUint8FromConstSegment()
 *********************************************************************************************************************/
/*! \brief          Reads an uint8 value from the given source segment and returns the value through a pointer.
 *  \details        -
 *  \param[in]      SourceSegPtr         Pointer to the source buffer segment.
 *  \param[out]     ReadUint8ValuePtr    Pointer to the read uint8 value.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_GetUint8FromConstSegment(
  TcpIp_ConstSegmentPtrType SourceSegPtr,
  TCPIP_P2V(uint8)          ReadUint8ValuePtr);

/**********************************************************************************************************************
 * TcpIp_MemCpyFromEndOfConstSegmentSafe()
 *********************************************************************************************************************/
/*! \brief          Copies data from the end of the given source segment to the target pointer.
 *  \details        The usage of the source buffer segment will not be increased instead its size is decreased.
 *  \param[out]     TargetPtr    Pointer to the target buffer.
 *                               \spec requires $lengthOf(TargetPtr) >= TargetSize; \endspec
 *                               \spec requires TargetPtr != NULL_PTR; \endspec
 *  \param[in]      TargetPos    Buffer position where the data should be written to.
 *  \param[in]      TargetSize   Size of the target buffer.
 *  \param[in]      SourceSegPtr Pointer to the source buffer segment.
 *  \param[in]      Count        Amount of data which should be written.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different TargetPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_MemCpyFromEndOfConstSegmentSafe(
  TCPIP_P2V(uint8)          TargetPtr,
  uint32                    TargetPos,
  uint32                    TargetSize,
  TcpIp_ConstSegmentPtrType SourceSegPtr,
  uint32                    Count);

/**********************************************************************************************************************
 * TcpIp_MemSetSafe()
 *********************************************************************************************************************/
/*! \brief          Sets data at the target pointer to the passed Pattern value.
 *  \details        This function is a wrapper for VStdMemSet with additional checks of the input parameter.
 *  \param[out]     TargetPtr    Pointer to the target buffer.
 *                               \spec requires $lengthOf(TargetPtr) >= TargetSize; \endspec
 *                               \spec requires TargetPtr != NULL_PTR; \endspec
 *  \param[in]      TargetPos    Buffer position where the data should be written to.
 *  \param[in]      TargetSize   Size of the target buffer.
 *  \param[in]      Pattern      Value which shall be set.
 *  \param[in]      Count        Amount of data which should be written.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different TargetPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_MemSetSafe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  uint8            Pattern,
  uint32           Count);

/**********************************************************************************************************************
 * TcpIp_PutUint64Safe()
 *********************************************************************************************************************/
/*! \brief          Writes an eight-byte unsigned integer into the buffer at the target pointer.
 *  \details        This function is a wrapper for IpBase_PutUint64 with additional checks of the input parameter.
 *  \param[out]     TargetPtr    Pointer to the target buffer.
 *                               \spec requires $lengthOf(TargetPtr) >= TargetSize; \endspec
 *                               \spec requires TargetPtr != NULL_PTR; \endspec
 *  \param[in]      TargetPos    Buffer position where the data should be written to.
 *  \param[in]      TargetSize   Size of the target buffer.
 *  \param[in]      Value        Value which shall be written.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different TargetPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_PutUint64Safe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  uint64           Value);

/**********************************************************************************************************************
 * TcpIp_PutUint32Safe()
 *********************************************************************************************************************/
/*! \brief          Writes an four-byte unsigned integer into the buffer at the target pointer.
 *  \details        This function is a wrapper for IpBase_PutUint32 with additional checks of the input parameter.
 *  \param[out]     TargetPtr    Pointer to the target buffer.
 *                               \spec requires $lengthOf(TargetPtr) >= TargetSize; \endspec
 *                               \spec requires TargetPtr != NULL_PTR; \endspec
 *  \param[in]      TargetPos    Buffer position where the data should be written to.
 *  \param[in]      TargetSize   Size of the target buffer.
 *  \param[in]      Value        Value which shall be written.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different TargetPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_PutUint32Safe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  uint32           Value);

/**********************************************************************************************************************
 * TcpIp_PutUint16Safe()
 *********************************************************************************************************************/
/*! \brief          Writes an two-byte unsigned integer into the buffer at the target pointer.
 *  \details        This function is a wrapper for IpBase_PutUint16 with additional checks of the input parameter.
 *  \param[out]     TargetPtr    Pointer to the target buffer.
 *                               \spec requires $lengthOf(TargetPtr) >= TargetSize; \endspec
 *                               \spec requires TargetPtr != NULL_PTR; \endspec
 *  \param[in]      TargetPos    Buffer position where the data should be written to.
 *  \param[in]      TargetSize   Size of the target buffer.
 *  \param[in]      Value        Value which shall be written.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different TargetPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_PutUint16Safe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  uint16           Value);

/**********************************************************************************************************************
 * TcpIp_PutUint8Safe()
 *********************************************************************************************************************/
/*! \brief          Writes an unsigned byte into the buffer at the target pointer.
 *  \details        This function is a wrapper for IpBase_PutUint8 with additional checks of the input parameter.
 *  \param[out]     TargetPtr    Pointer to the target buffer.
 *                               \spec requires $lengthOf(TargetPtr) >= TargetSize; \endspec
 *                               \spec requires TargetPtr != NULL_PTR; \endspec
 *  \param[in]      TargetPos    Buffer position where the data should be written to.
 *  \param[in]      TargetSize   Size of the target buffer.
 *  \param[in]      Value        Value which shall be written.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different TargetPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_PutUint8Safe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  uint8            Value);

/**********************************************************************************************************************
 * TcpIp_RingBufferCpySafe()
 *********************************************************************************************************************/
/*! \brief          Copies data from the given source pointer to the target pointer with an automatic wraparound.
 *  \details        This function is a wrapper for VStdMemCpy with additional checks of the input parameter.
 *  \param[out]     TargetPtr    Pointer to the target buffer.
 *                               \spec requires $lengthOf(TargetPtr) >= TargetSize; \endspec
 *  \param[in]      TargetPos    Buffer position where the data should be written to.
 *  \param[in]      TargetSize   Size of the target buffer.
 *  \param[in]      SourcePtr    Pointer to the source buffer.
 *  \param[in]      Count        Amount of data which should be written.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different TargetPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_RingBufferCpySafe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  TCPIP_P2C(uint8) SourcePtr,
  uint32           Count);

/**********************************************************************************************************************
 * TcpIp_VerifyStartAndEndIndex()
 *********************************************************************************************************************/
/*! \brief          Verifies that the indices are in correct relation and within the borders of the buffer size.
 *  \details        In case of invalid indices a runtime check error will be triggered.
 *  \param[in]      StartIdx     Start index within the buffer.
 *  \param[in]      EndIdx       End index within the buffer.
 *  \param[in]      BufferSize   Size of the target buffer.
 *  \return         E_OK         The indices are valid.
 *  \return         E_NOT_OK     The indices are not valid.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VerifyStartAndEndIndex(
  uint32_least StartIdx,
  uint32_least EndIdx,
  uint32_least BufferSize);

/**********************************************************************************************************************
 * TcpIp_GetDecrementedRingBufferIndex()
 *********************************************************************************************************************/
/*! \brief          Gets the buffer index if a given index is decremented by a given length considering a wraparound
 *  \details        -
 *  \param[in]      CurrBufferIdx            Current buffer index from which DecrementLen shall be decremented,
 *                                           BufferStartIdx <= CurrBufferIdx < BufferEndIdx must be true
 *  \param[in]      DecrementLen             The length for which the index shall be decremented,
 *                                           DecrementLen <= BufferEndIdx - BufferStartIdx must be true
 *  \param[in]      BufferStartIdx           Start index of the buffer, BufferStartIdx < BufferEndIdx must be true
 *  \param[in]      BufferEndIdx             End index of the buffer
 *  \param[out]     DecrementedBufferIdxPtr  Pointer to the buffer index when CurrBufferIdx would be decremented
 *                                           by DecrementLen considering a wraparound
 *  \result         E_OK                     Indices and length are valid, decremented buffer index was calculated
 *  \result         E_NOT_OK                 Indices or length not valid
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetDecrementedRingBufferIndex(
  uint32                CurrBufferIdx,
  uint32                DecrementLen,
  uint32                BufferStartIdx,
  uint32                BufferEndIdx,
  TCPIP_P2V(uint32)     DecrementedBufferIdxPtr);

/**********************************************************************************************************************
 * TcpIp_GetIncrementedRingBufferIndex()
 *********************************************************************************************************************/
/*! \brief          Gets the buffer index if a given index is incremented by a given length considering a wraparound
 *  \details        -
 *  \param[in]      CurrBufferIdx            Current buffer index from which IncrementLen shall be incremented,
 *                                           BufferStartIdx <= CurrBufferIdx < BufferEndIdx must be true
 *  \param[in]      IncrementLen             The length for which the index shall be incremented,
 *                                           IncrementLen <= BufferEndIdx - BufferStartIdx must be true
 *  \param[in]      BufferStartIdx           Start index of the buffer, BufferStartIdx < BufferEndIdx must be true
 *  \param[in]      BufferEndIdx             End index of the buffer
 *  \param[out]     IncrementedBufferIdxPtr  Pointer to the buffer index when CurrBufferIdx would be incremented
 *                                           by IncrementLen considering a wraparound
 *  \result         E_OK                     Indices and length are valid, decremented buffer index was calculated
 *  \result         E_NOT_OK                 Indices or length not valid
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetIncrementedRingBufferIndex(
  uint32                CurrBufferIdx,
  uint32                IncrementLen,
  uint32                BufferStartIdx,
  uint32                BufferEndIdx,
  TCPIP_P2V(uint32)     IncrementedBufferIdxPtr);

/**********************************************************************************************************************
 * TcpIp_GetSplitLengthInRingBuffer()
 *********************************************************************************************************************/
/*! \brief          After data was written in a buffer, this function checks how the data was split
 *  \details        -
 *  \param[in]      CurrBufferIdx         Current buffer write index,
 *                                        BufferStartIdx <= CurrBufferIdx < BufferEndIdx must be true
 *  \param[in]      PrevBufferIdx         The previous buffer write index before data was written into the buffer
 *                                        BufferStartIdx <= PrevBufferIdx < BufferEndIdx must be true
 *  \param[in]      BufferStartIdx        Start index of the buffer, BufferStartIdx < BufferEndIdx must be true
 *  \param[in]      BufferEndIdx          End index of the buffer
 *  \param[out]     LinearLengthPtr       Pointer to the length which is written linearly in the buffer (can be zero)
 *  \param[out]     EndOfBufferLengthPtr  Pointer to the length which is written at the end of the buffer (can be zero)
 *  \return         E_OK                  Indices are valid, linear and end of buffer lengths were calculated
 *  \return         E_NOT_OK              Indices are invalid
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetSplitLengthInRingBuffer(
  uint32                CurrBufferIdx,
  uint32                PrevBufferIdx,
  uint32                BufferStartIdx,
  uint32                BufferEndIdx,
  TCPIP_P2V(uint32)     LinearLengthPtr,
  TCPIP_P2V(uint32)     EndOfBufferLengthPtr);

# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 * TcpIp_SegmentInitWithTcpTxBufferInRangeCheck()
 *********************************************************************************************************************/
/*! \brief          Initializes a managed buffer segment if the Tcp Tx buffer is in range.
 *  \details        The initialized buffer segment is only allowed to be modified by buffer segment helper functions
 *                  to ensure a proper buffer management and thus freedom of interference.
 *  \param[out]     SegmentPtr   Pointer to the buffer segment to be initialized.
 *                               \spec requires SegmentPtr != NULL_PTR; \endspec
 *  \param[in,out]  BufferPtr    The buffer to be managed. If the buffer check fails this parameter will be overwritten
 *                               with a null pointer.
 *  \param[in]      BufferSize   The size of the buffer to be managed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentInitWithTcpTxBufferInRangeCheck(
  TcpIp_SegmentPtrType          SegmentPtr,
  TCPIP_P2V(TcpIp_Uint8P2V)     BufferPtr,
  uint32                        BufferSize);
# endif

/**********************************************************************************************************************
 * TcpIp_SegmentInit()
 *********************************************************************************************************************/
/*! \brief          Initializes a managed buffer segment.
 *  \details        The initialized buffer segment is only allowed to be modified by buffer segment helper functions
 *                  to ensure a proper buffer management and thus freedom of interference.
 *  \param[out]     SegmentPtr   Pointer to the buffer segment to be initialized.
 *                               \spec requires SegmentPtr != NULL_PTR; \endspec
 *  \param[in]      Buffer       The buffer to be managed.
 *                               \spec requires ((Buffer != NULL_PTR) && ($lengthOf(Buffer) >= BufferSize)); \endspec
 *  \param[in]      BufferSize   The size of the buffer to be managed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentInit(
  TcpIp_SegmentPtrType SegmentPtr,
  TCPIP_P2V(uint8)     Buffer,
  uint32               BufferSize);

/**********************************************************************************************************************
 * TcpIp_ConstSegmentInitWithUsedSegment()
 *********************************************************************************************************************/
/*! \brief          Initializes a managed const buffer segment using a not const buffer segment.
 *  \details        The maximum amount which can be read from the buffer segment of SegmentToInitPtr
 *                  shall be the amount that was written into the buffer segment of UsedSegmentPtr.
 *  \param[out]     SegmentToInitPtr  Pointer to the buffer segment to be initialized.
 *                                    \spec requires SegmentToInitPtr != NULL_PTR; \endspec
 *  \param[in]      UsedSegmentPtr    Pointer to the buffer segment which shall be used to initialize the other segment
 *                                    \spec requires UsedSegmentPtr != NULL_PTR; \endspec
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentToInitPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_ConstSegmentInitWithUsedSegment(
  TcpIp_ConstSegmentPtrType         SegmentToInitPtr,
  TCPIP_P2C(TcpIp_SegmentType)      UsedSegmentPtr);

/**********************************************************************************************************************
 * TcpIp_ConstSegmentInit()
 *********************************************************************************************************************/
/*! \brief          Initializes a managed const buffer segment. Empty buffer segments are allowed.
 *  \details        The initialized buffer segment should only be accessed by const buffer segment helper functions
 *                  to ensure a proper buffer management and to prevent an invalid read access.
 *  \param[out]     SegmentPtr   Pointer to the buffer segment to be initialized.
 *                               \spec requires SegmentPtr != NULL_PTR; \endspec
 *  \param[in]      Buffer       The buffer to be managed.
 *                               \spec requires (   ((Buffer != NULL_PTR) && ($lengthOf(Buffer) >= BufferSize))
 *                                               || ((Buffer == NULL_PTR) && (BufferSize == 0u)) ); \endspec
 *  \param[in]      BufferSize   The size of the buffer to be managed.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_ConstSegmentInit(
  TcpIp_ConstSegmentPtrType SegmentPtr,
  TCPIP_P2C(uint8)          Buffer,
  uint32                    BufferSize);

/**********************************************************************************************************************
 * TcpIp_SegmentCommit()
 *********************************************************************************************************************/
/*! \brief          Commits data written into the managed buffer by external functions.
 *  \details        Updates the 'Usage' and 'AvailLen' settings of the buffer segment.
 *  \param[in,out]  SegmentPtr   Pointer to the buffer segment.
 *  \param[in]      Count        Number of bytes which were written into the managed buffer.
 *  \pre            TcpIp_SegmentInit() must have been called at least one time for SegmentPtr
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentCommit(
  TcpIp_SegmentPtrType SegmentPtr,
  uint32               Count);

/**********************************************************************************************************************
 * TcpIp_ConstSegmentCommit()
 *********************************************************************************************************************/
/*! \brief          Commits data read from the managed const buffer segment by external functions.
 *  \details        Updates the 'Usage' and 'AvailLen' settings of the const buffer segment.
 *  \param[in,out]  SegmentPtr   Pointer to the const buffer segment.
 *  \param[in]      Count        Number of bytes which were read from the managed buffer segment.
 *  \pre            TcpIp_SegmentConstInit() must have been called at least one time for SegmentPtr
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_ConstSegmentCommit(
  TcpIp_ConstSegmentPtrType SegmentPtr,
  uint32                    Count);

/**********************************************************************************************************************
 * TcpIp_SegmentClear()
 *********************************************************************************************************************/
/*! \brief          Clears the committed data by moving the buffer pointer to the next free byte position.
 *  \details        This effectively reduces the manageable portion of the buffer to the portion after the first free
 *                  byte. It corresponds to a reinitialization of the buffer segment, but with the difference that
 *                  afterwards only a portion of the original buffer is manageable.
 *  \param[in,out]  SegmentPtr   Pointer to the buffer segment.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentClear(
  TcpIp_SegmentPtrType SegmentPtr);

/**********************************************************************************************************************
 * TcpIp_ConstSegmentClear()
 *********************************************************************************************************************/
/*! \brief          Clears the committed data by moving the buffer pointer to the next free byte position.
 *  \details        This effectively reduces the manageable portion of the buffer to the portion after the first free
 *                  byte. It corresponds to a reinitialization of the buffer segment, but with the difference that
 *                  afterwards only a portion of the original buffer is manageable.
 *  \param[in,out]  SegmentPtr    Pointer to the const buffer segment.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_ConstSegmentClear(
  TcpIp_ConstSegmentPtrType SegmentPtr);

/**********************************************************************************************************************
 * TcpIp_SegmentMemCpy()
 *********************************************************************************************************************/
/*! \brief          Copies data from the given source pointer to the given buffer segment.
 *  \details        Runtime-checks ensure, that no buffer will be overwritten.
 *  \param[in,out]  SegmentPtr   Pointer to the buffer segment.
 *  \param[in]      SourcePtr    Pointer to the source buffer.
 *  \param[in]      Count        Amount of data which should be written.
 *  \pre            The free memory of the segment must be large enough to store 'Count' bytes.
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentMemCpy(
  TcpIp_SegmentPtrType SegmentPtr,
  TCPIP_P2C(uint8)     SourcePtr,
  uint32               Count);

/**********************************************************************************************************************
 * TcpIp_SegmentMemCpyFromConstSegment()
 *********************************************************************************************************************/
/*! \brief          Copies data from the given const source buffer segment to the given target buffer segment.
 *  \details        Runtime-checks ensure that no buffer will be overwritten and that no invalid read access occurs.
 *  \param[in,out]  TargetSegmentPtr   Pointer to the target buffer segment.
 *  \param[in]      SourceSegmentPtr   Pointer to the source buffer segment.
 *  \param[in]      Count              Amount of data which should be written.
 *  \pre            The free memory of the segment must be large enough to store 'Count' bytes.
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different TargetSegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentMemCpyFromConstSegment(
  TcpIp_SegmentPtrType        TargetSegmentPtr,
  TcpIp_ConstSegmentPtrType   SourceSegmentPtr,
  uint32                      Count);

/**********************************************************************************************************************
 * TcpIp_SegmentMemCpyFromEndOfConstSegment()
 *********************************************************************************************************************/
/*! \brief          Copies data from the end of the given const source buffer to given target buffer segment
 *  \details        Runtime-checks ensure that no buffer will be overwritten and that no invalid read access occurs.
 *                  The usage of the source buffer segment will not be increased instead its size is decreased.
 *  \param[in,out]  TargetSegmentPtr   Pointer to the target buffer segment.
 *  \param[in]      SourceSegmentPtr   Pointer to the source buffer segment.
 *  \param[in]      Count              Amount of data which should be written.
 *  \pre            The free memory of the segment must be large enough to store 'Count' bytes.
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different TargetSegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentMemCpyFromEndOfConstSegment(
  TcpIp_SegmentPtrType        TargetSegmentPtr,
  TcpIp_ConstSegmentPtrType   SourceSegmentPtr,
  uint32                      Count);

/**********************************************************************************************************************
 * TcpIp_SegmentMemSet()
 *********************************************************************************************************************/
/*! \brief          Sets data in the given buffer segment to the passed Pattern value.
 *  \details        Runtime-checks ensure, that no buffer will be overwritten.
 *  \param[in,out]  SegmentPtr   Pointer to the buffer segment.
 *  \param[in]      Pattern      Value which shall be written into the buffer.
 *  \param[in]      Count        Amount of data which should be written.
 *  \pre            The free memory of the segment must be large enough to store 'Count' bytes.
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentMemSet(
  TcpIp_SegmentPtrType SegmentPtr,
  uint8                Pattern,
  uint32               Count);

/**********************************************************************************************************************
 * TcpIp_SegmentPutUint64()
 *********************************************************************************************************************/
/*! \brief          Puts the value of the passed uint64 variable in the given buffer segment.
 *  \details        Runtime-checks ensure, that no buffer will be overwritten.
 *  \param[in,out]  SegmentPtr   Pointer to the buffer segment.
 *  \param[in]      Value        Value which shall be written into the buffer.
 *  \pre            The free memory of the segment must be large enough to store the Value variable.
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentPutUint64(
  TcpIp_SegmentPtrType SegmentPtr,
  uint64               Value);

/**********************************************************************************************************************
 * TcpIp_SegmentPutUint32()
 *********************************************************************************************************************/
/*! \brief          Puts the value of the passed uint32 variable in the given buffer segment.
 *  \details        Runtime-checks ensure, that no buffer will be overwritten.
 *  \param[in,out]  SegmentPtr   Pointer to the buffer segment.
 *  \param[in]      Value        Value which shall be written into the buffer.
 *  \pre            The free memory of the segment must be large enough to store the Value variable.
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentPutUint32(
  TcpIp_SegmentPtrType SegmentPtr,
  uint32               Value);

/**********************************************************************************************************************
 * TcpIp_SegmentPutUint16()
 *********************************************************************************************************************/
/*! \brief          Puts the value of the passed uint16 variable in the given buffer segment.
 *  \details        Runtime-checks ensure, that no buffer will be overwritten.
 *  \param[in,out]  SegmentPtr   Pointer to the buffer segment.
 *  \param[in]      Value        Value which shall be written into the buffer.
 *  \pre            The free memory of the segment must be large enough to store the Value variable.
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentPutUint16(
  TcpIp_SegmentPtrType SegmentPtr,
  uint16               Value);

/**********************************************************************************************************************
 * TcpIp_SegmentPutUint8()
 *********************************************************************************************************************/
/*! \brief          Puts the value of the passed uint8 variable in the given buffer segment.
 *  \details        Runtime-checks ensure, that no buffer will be overwritten.
 *  \param[in,out]  SegmentPtr   Pointer to the buffer segment.
 *  \param[in]      Value        Value which shall be written into the buffer.
 *  \pre            The free memory of the segment must be large enough to store the Value variable.
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different SegmentPtr
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentPutUint8(
  TcpIp_SegmentPtrType SegmentPtr,
  uint8                Value);

/**********************************************************************************************************************
 * TcpIp_SegmentMultiTargetMemCpy()
 *********************************************************************************************************************/
/*! \brief          Copies data from the given source pointer to the given buffer segments.
 *  \details        In case that the available buffer space of the first buffer segment is not large enough, the
 *                  remaining data will be copied to the second buffer segment.
 *                  Runtime-checks ensure, that no buffer will be overwritten.
 *  \param[in,out]  Segment1Ptr   Pointer to the first buffer segment.
 *  \param[in,out]  Segment2Ptr   Pointer to the second buffer segment.
 *  \param[in]      SourcePtr     Pointer to the source buffer.
 *  \param[in]      Count         Amount of data which should be written.
 *  \pre            The free memory of the two segments together must be large enough to store 'Count' bytes.
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different buffer segments
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SegmentMultiTargetMemCpy(
  TcpIp_SegmentPtrType Segment1Ptr,
  TcpIp_SegmentPtrType Segment2Ptr,
  TCPIP_P2C(uint8)     SourcePtr,
  uint32               Count);

/**********************************************************************************************************************
 * TcpIp_SegmentMemCpyFromMultiConstSegment()
 *********************************************************************************************************************/
/*! \brief          Copies data from the given const segments to the given write buffer segment.
 *  \details        In case that the available buffer space of the first const buffer segment is not large enough, the
 *                  remaining data will be copied from the second const buffer segment.
 *                  Runtime-checks ensure, that no buffer will be overwritten and no invalid read access occurs.
 *  \param[in]      SourceSegment1Ptr   Pointer to the first const buffer segment.
 *  \param[in]      SourceSegment2Ptr   Pointer to the second const buffer segment.
 *  \param[in,out]  WriteSegmentPtr     Pointer to the buffer segment where the data shall be copied to.
 *  \param[in]      CopyLen             Amount of data which should be copied.
 *  \return         E_OK                The two const segments have enough space to copy the data
 *  \return         E_NOT_OK            The two const segments don't have enough space, data can't be copied
 *  \pre            The free memory of the write buffer segment must be large enough to store 'CopyLen' bytes.
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different buffer segments
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_SegmentMemCpyFromMultiConstSegment(
  TcpIp_ConstSegmentPtrType SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType SourceSegment2Ptr,
  TcpIp_SegmentPtrType      WriteSegmentPtr,
  uint32                    CopyLen);

/**********************************************************************************************************************
 * TcpIp_MultiConstSegmentInitWithSkippedDataInMultiConstSegment()
 *********************************************************************************************************************/
/*! \brief          Initializes two const segments with the data it skips in two other given source const segments.
 *  \details        Checks if there is enough space in the two source segments for the passed length.
 *                  Instead of reading the data from the source segments, the other segments are initialized with it.
 *                  How the data is split between the two source segments is exactly how it will be split in the
 *                  initialized segments. Afterwards the data can be read from these segments. The data in the
 *                  source segments is skipped.
 *  \param[in,out]  Segment1ToInitPtr   Pointer to the first const buffer segment which is to be initialized.
 *  \param[in,out]  Segment2ToInitPtr   Pointer to the second const buffer segment which is to be initialized.
 *  \param[in,out]  SourceSegment1Ptr   Pointer to the first const buffer segment where data is skipped.
 *  \param[in,out]  SourceSegment2Ptr   Pointer to the second const buffer segment where data is skipped.
 *  \param[in]      SkipLen             Amount of data which should be skipped.
 *  \return         E_OK                Segments have enough space, data was skipped, segments were initialized
 *  \return         E_NOT_OK            Segments don't have enough space, data was NOT skipped, NO initialization
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different buffer segments
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_MultiConstSegmentInitWithSkippedDataInMultiConstSegment(
  TcpIp_ConstSegmentPtrType Segment1ToInitPtr,
  TcpIp_ConstSegmentPtrType Segment2ToInitPtr,
  TcpIp_ConstSegmentPtrType SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType SourceSegment2Ptr,
  uint32                    SkipLen);

/**********************************************************************************************************************
 * TcpIp_SkipDataInMultiConstSegment()
 *********************************************************************************************************************/
/*! \brief          Skips data in the given const segments by invoking TcpIp_ConstSegmentCommit().
 *  \details        The length of the commits is calculated according to the available lengths. If the two
 *                  const segment's available buffer is not large enough for the skip length, nothing is done.
 *  \param[in,out]  SourceSegment1Ptr   Pointer to the first const buffer segment.
 *  \param[in,out]  SourceSegment2Ptr   Pointer to the second const buffer segment.
 *  \param[in]      SkipLen             Amount of data which should be skipped.
 *  \return         E_OK                Segments have enough space, the passed length was committed
 *  \return         E_NOT_OK            Segments don't have enough space, no commit was done on either const segment
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different buffer segments
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_SkipDataInMultiConstSegment(
  TcpIp_ConstSegmentPtrType SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType SourceSegment2Ptr,
  uint32                    SkipLen);

/**********************************************************************************************************************
 * TcpIp_GetUint8FromMultiConstSegment()
 *********************************************************************************************************************/
/*! \brief          Reads an uint8 value from the given const segments and returns it through a pointer.
 *  \details        In case that the available buffer space of the first const buffer segment is not large enough,
 *                  the uint8 value will be read from the second const buffer segment.
 *                  Runtime-checks ensure, that no invalid read access occurs.
 *  \param[in]      SourceSegment1Ptr   Pointer to the first const buffer segment.
 *  \param[in]      SourceSegment2Ptr   Pointer to the second const buffer segment.
 *  \param[in,out]  ReadUint8ValuePtr   Pointer where the read uint8 value shall be saved.
 *  \return         E_OK                Segments have enough space, the uint8 value has been read
 *  \return         E_NOT_OK            Segments don't have enough space, uint8 value can't be read
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different buffer segments
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetUint8FromMultiConstSegment(
  TcpIp_ConstSegmentPtrType           SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType           SourceSegment2Ptr,
  TCPIP_P2V(uint8)                    ReadUint8ValuePtr);

/**********************************************************************************************************************
 * TcpIp_GetUint16FromMultiConstSegment()
 *********************************************************************************************************************/
/*! \brief          Reads an uint16 value from the given const segments and returns it through a pointer.
 *  \details        In case that the available buffer space of the first const buffer segment is not large enough, the
 *                  remaining data will be read from the second const buffer segment.
 *                  Runtime-checks ensure, that no invalid read access occurs.
 *  \param[in]      SourceSegment1Ptr   Pointer to the first const buffer segment.
 *  \param[in]      SourceSegment2Ptr   Pointer to the second const buffer segment.
 *  \param[in,out]  ReadUint16ValuePtr  Pointer where the read uint16 value shall be saved.
 *  \return         E_OK                Segments have enough space, the uint16 value has been read
 *  \return         E_NOT_OK            Segments don't have enough space, uint16 value can't be read
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different buffer segments
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetUint16FromMultiConstSegment(
  TcpIp_ConstSegmentPtrType           SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType           SourceSegment2Ptr,
  TCPIP_P2V(uint16)                   ReadUint16ValuePtr);

/**********************************************************************************************************************
 * TcpIp_GetUint24FromMultiConstSegment()
 *********************************************************************************************************************/
/*! \brief          Reads an uint24 value from the given const segments and returns it through a pointer.
 *  \details        In case that the available buffer space of the first const buffer segment is not large enough, the
 *                  remaining data will be read from the second const buffer segment.
 *                  Runtime-checks ensure, that no invalid read access occurs.
 *  \param[in]      SourceSegment1Ptr   Pointer to the first const buffer segment.
 *  \param[in]      SourceSegment2Ptr   Pointer to the second const buffer segment.
 *  \param[in,out]  ReadUint24ValuePtr  Pointer to an uint32 variable where the read uint24 value shall be saved.
 *  \return         E_OK                Segments have enough space, the uint24 value has been read
 *  \return         E_NOT_OK            Segments don't have enough space, uint24 value can't be read
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different buffer segments
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetUint24FromMultiConstSegment(
  TcpIp_ConstSegmentPtrType           SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType           SourceSegment2Ptr,
  TCPIP_P2V(uint32)                   ReadUint24ValuePtr);

/**********************************************************************************************************************
 * TcpIp_GetUint32FromMultiConstSegment()
 *********************************************************************************************************************/
/*! \brief          Reads an uint32 value from the given const segments and returns it through a pointer.
 *  \details        In case that the available buffer space of the first const buffer segment is not large enough, the
 *                  remaining data will be read from the second const buffer segment.
 *                  Runtime-checks ensure, that no invalid read access occurs.
 *  \param[in]      SourceSegment1Ptr   Pointer to the first const buffer segment.
 *  \param[in]      SourceSegment2Ptr   Pointer to the second const buffer segment.
 *  \param[in,out]  ReadUint32ValuePtr  Pointer where the read uint24 value shall be saved.
 *  \return         E_OK                Segments have enough space, the uint32 value has been read
 *  \return         E_NOT_OK            Segments don't have enough space, uint32 value can't be read
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE for different buffer segments
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetUint32FromMultiConstSegment(
  TcpIp_ConstSegmentPtrType           SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType           SourceSegment2Ptr,
  TCPIP_P2V(uint32)                   ReadUint32ValuePtr);

/**********************************************************************************************************************
 * TcpIp_GetSplitLengthInMultiConstSegment()
 *********************************************************************************************************************/
/*! \brief          This function checks how data with a given length is split between two passed const segments.
 *  \details        It also checks that the segments have enough space left for the data length to potentially be read.
 *  \param[in]      SourceSegment1Ptr     Pointer to the first const segment
 *  \param[in]      SourceSegment2Ptr     Pointer to the second const segment
 *  \param[in]      Length                Length of the data
 *  \param[out]     LengthInSegment1Ptr   Pointer to the length which can be read from the first segment (can be zero)
 *  \param[out]     LengthInSegment2Ptr   Pointer to the length which can be read from the second segment (can be zero)
 *  \return         E_OK                  Segments have enough space, their corresponding lengths were calculated
 *  \return         E_NOT_OK              Segments don't have enough space, data with the passed length can't be read
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetSplitLengthInMultiConstSegment(
  TCPIP_P2C(TcpIp_ConstSegmentType) SourceSegment1Ptr,
  TCPIP_P2C(TcpIp_ConstSegmentType) SourceSegment2Ptr,
  uint32                            Length,
  TCPIP_P2V(uint32)                 LengthInSegment1Ptr,
  TCPIP_P2V(uint32)                 LengthInSegment2Ptr);

/**********************************************************************************************************************
 * TcpIp_SegmentAssertMinLen()
 *********************************************************************************************************************/
/*! \brief          Checks whether the given buffer segment has free memory for at least 'Count' bytes.
 *  \details        This function can be used to ensure a minimum amount of free memory before the managed buffer is
 *                  passed as a pointer to an external function for write operations.
 *  \param[in]      SegmentPtr   Pointer to the buffer segment.
 *  \param[in]      Count        Amount of data which should be written.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_SegmentAssertMinLen(
  TCPIP_P2C(TcpIp_SegmentType) SegmentPtr,
  uint32                       Count);

/**********************************************************************************************************************
 *  TcpIp_VPreconfigSocket()
 *********************************************************************************************************************/
/*! \brief          Pre-configures a (TCP or UDP) socket
 *  \details        -
 *  \param[in]      SocketIdx   Index of a UDP or TCP socket.
 *                              \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different SocketIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VPreconfigSocket(TcpIp_SocketDynIterType SocketIdx);

/**********************************************************************************************************************
 *  TcpIp_VRxUdpSockIdxIdent()
 *********************************************************************************************************************/
/*! \brief          Identifies an incoming UDP socket.
 *  \details        -
 *  \param[in]      RxSockLocAddrPtr    Local address of the received packet.
 *  \param[in]      LocalAddrIdx        Index of a local address.
 *                                      \spec requires LocalAddrIdx < TcpIp_GetSizeOfLocalAddr(); \endspec
 *  \return         Socket index of the given socket address in case the UDP socket was identified.
 *  \return         TCPIP_SOCKET_IDX_INV in case the UDP socket could NOT be identified.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(TcpIp_SocketDynIterType, TCPIP_CODE) TcpIp_VRxUdpSockIdxIdent(
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TcpIp_LocalAddrIterType           LocalAddrIdx);

# if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VRxTcpSockIdxIdent()
 *********************************************************************************************************************/
/*! \brief          Identifies an incoming TCP socket.
 *  \details        -
 *  \param[in]      RxSockRemAddrPtr    Remote address of the received packet.
 *  \param[in]      RxSockLocAddrPtr    Local address of the received packet.
 *  \param[in]      LocalAddrIdx        Index of a local address.
 *                                      \spec requires LocalAddrIdx < TcpIp_GetSizeOfLocalAddr(); \endspec
 *  \return         Socket index of the given socket address in case the TCP socket was identified.
 *  \return         TCPIP_SOCKET_IDX_INV in case the TCP socket could NOT be identified.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(TcpIp_SocketDynIterType, TCPIP_CODE) TcpIp_VRxTcpSockIdxIdent(
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockRemAddrPtr,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TcpIp_LocalAddrIterType           LocalAddrIdx);
# endif

/**********************************************************************************************************************
 *  TcpIp_SetLastSockError()
 *********************************************************************************************************************/
/*! \brief         Sets the last socket error.
 *  \details       -
 *  \param[in]     Error               Error code.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_SetLastSockError(TcpIp_TcpIpErrorType Error);

/**********************************************************************************************************************
 *  TcpIp_VInitSockets()
 *********************************************************************************************************************/
/*! \brief         Initializes all sockets.
 *  \details       -
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VInitSockets(void);

/**********************************************************************************************************************
 *  TcpIp_VInitSocket()
 *********************************************************************************************************************/
/*! \brief         Initializes the admin data of one single socket.
 *  \details       Called in the context of the initialization of the TcpIp.
 *  \param[in]     SocketIdx   Index of a UDP or TCP socket.
 *                 \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VInitSocket(TcpIp_SocketDynIterType SocketIdx);

/**********************************************************************************************************************
 *  TcpIp_VDelSockAddrMapping()
 *********************************************************************************************************************/
/*! \brief         Deletes a socket address mapping.
 *  \details       Called in the context of the initialization of the TcpIp.
 *  \param[in]     SocketIdx   Index of a UDP or TCP socket.
 *                 \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VDelSockAddrMapping(TcpIp_SocketDynIterType SocketIdx);

/**********************************************************************************************************************
 *  TcpIp_VSockIpAddrIsEqual()
 *********************************************************************************************************************/
/*! \brief         Checks if the IP addresses of two given sockets are equal.
 *  \details       -
 *  \param[in]     SockAPtr             Pointer to first socket address.
 *  \param[in]     SockBPtr             Pointer to second socket address.
 *  \return        TRUE                 IP address family and address values are equal.
 *  \return        FALSE                IP address family does not match or address values differ.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_VSockIpAddrIsEqual(
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockAPtr,
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockBPtr);

# if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VSockAddrIpAndPortIsEqual()
 *********************************************************************************************************************/
/*! \brief         Checks if IP addresses and ports of two given sockets are equal.
 *  \details       This function does the same as TcpIp_VSockIpAddrIsEqual() plus a comparison of the port value.
 *  \param[in]     SockAPtr             Pointer to first socket address.
 *  \param[in]     SockBPtr             Pointer to second socket address.
 *  \return        TRUE                 IP address family, address values and port values are equal.
 *  \return        FALSE                IP address family, address values or ports values differ.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_VSockAddrIpAndPortIsEqual(
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockAPtr,
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockBPtr);

/**********************************************************************************************************************
 *  TcpIp_VNetAddrIsUnicast()
 *********************************************************************************************************************/
/*! \brief         Checks if the given SockAddr is a unicast address.
 *  \details       -
 *  \param[in]     SockAddrPtr      Socket address containing the IP address
 *  \return        TRUE             The socket address contains a unicast IP address
 *  \return        FALSE            The socket address does not contain a unicast IP address
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_VNetAddrIsUnicast(
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockAddrPtr);

/**********************************************************************************************************************
 *  TcpIp_VLocalAddrIdxIsUnicast()
 *********************************************************************************************************************/
/*! \brief         Checks if the given Ip address Idx belongs to an unicast address.
 *  \details       -
 *  \param[in]     LocalAddrIdx     Local address index
 *  \return        TRUE             The local address Idx belongs to a local unicast IP address
 *  \return        FALSE            The local address Idx does not belong to a local unicast IP address
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_VLocalAddrIdxIsUnicast(
  TcpIp_LocalAddrIterType LocalAddrIdx);

# endif

# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
/**********************************************************************************************************************
 *  TcpIp_VCalcIpV4PseudoHdrChecksum()
 ***********************************************************************************************************************/
/*! \brief         Calculates the pseudo header checksum for IPv4.
 *  \details       The returened checksum value is not finalized (wrapped to 16 bit) in order to allow adding payload
 *                 bytes to the checksum.
 *                 IpBase_TcpIpChecksumAdd(..., TRUE) must be called to finalize the checksum.
 *  \param[in]     RemAddr               Remote IPv4 address.
 *  \param[in]     LocAddr               Local IPv4 address.
 *  \param[in]     Protocol              IP Protocol (TCP, UDP, ICMP, ...)
 *  \param[in]     ProtocolPayloadLen    Payload length following the IP header.
 *  \return                              Partial uint32 checksum value.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(uint32, TCPIP_CODE) TcpIp_VCalcIpV4PseudoHdrChecksum(
  IpBase_AddrInType RemAddr,
  IpBase_AddrInType LocAddr,
  uint8             Protocol,
  uint16            ProtocolPayloadLen);
# endif

# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 *  TcpIp_VCalcIpV6PseudoHdrChecksum()
 *********************************************************************************************************************/
/*! \brief         Calculates the pseudo header checksum for IPv6.
 *  \details       The returned checksum value is not finalized (wrapped to 16 bit) in order to allow adding payload
 *                 bytes to the checksum.
 *                 IpBase_TcpIpChecksumAdd(..., TRUE) must be called to finalize the checksum.
 *  \param[in]     RemAddrPtr            Remote IPv6 address.
 *  \param[in]     LocAddrPtr            Local IPv6 address.
 *  \param[in]     Protocol              IP Protocol (TCP, UDP, ICMP, ...)
 *  \param[in]     ProtocolPayloadLen    Payload length following the IP header.
 *  \return                              Partial uint32 checksum value.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(uint32, TCPIP_CODE) TcpIp_VCalcIpV6PseudoHdrChecksum(
  TCPIP_P2C(IpBase_AddrIn6Type) RemAddrPtr,
  TCPIP_P2C(IpBase_AddrIn6Type) LocAddrPtr,
  uint8                         Protocol,
  uint16                        ProtocolPayloadLen);
# endif

/**********************************************************************************************************************
 *  TcpIp_VCalcPseudoHdrAndChecksum()
 *********************************************************************************************************************/
/*! \brief         Calculates the pseudo header checksum depending on the IP version used in the socket addresses.
 *  \details       -
 *  \param[in]     RxSockRemAddrPtr      pointer to remote address value
 *  \param[in]     RxSockLocAddrPtr      pointer to local address value
 *  \param[in]     DataPtr               pointer to payload data
 *  \param[in]     DataLen               length payload data in bytes
 *  \param[in]     Protocol              protocol identifier
 *  \return                              Checksum value finalized to uint16 range.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(uint16, TCPIP_CODE) TcpIp_VCalcPseudoHdrAndChecksum(
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockRemAddrPtr,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TCPIP_P2C(uint8)                  DataPtr,
  uint16                            DataLen,
  uint8                             Protocol);

/**********************************************************************************************************************
 * TcpIp_CmpLLAddr()
 *********************************************************************************************************************/
/*! \brief          This method compares two Link Layer addresses (Ethernet physical addresses).
 *  \details        -
 *  \param[in]      LLAddr1Ptr      pointer to first physical layer address to compare.
 *  \param[in]      LLAddr2Ptr      pointer to second physical layer address to compare.
 *  \return         TRUE in case the addresses are equal, FALSE otherwise.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_CmpLLAddr(
  TCPIP_P2C(uint8) LLAddr1Ptr,
  TCPIP_P2C(uint8) LLAddr2Ptr);

/**********************************************************************************************************************
 *  TcpIp_VIpTransmit()
 *********************************************************************************************************************/
/*! \brief          Wraps the call to transmit function of IP layer. (IPV4/IPv6)
 *  \details        -
 *  \param[in]      IpTxReqDescPtr          Pointer to TX request descriptor that was used at the
 *                                          "provide tx buffer call" of the related IPv4 or IPv6 protocol, i.e.:
 *                                          IPv4: The IpV4_Ip_ProvideTxBuffer() call.
 *  \param[in]      IpPayloadLen            Number of bytes to transmit.
 *                                          This implementations allows two special cases:
 *                                          1: The value of ipPayloadLen may be larger than requested by IpV4_Ip_ProvideTxBuffer() if
 *                                             there was enough space left in the last provided buffer fragment.
 *                                          2: The value of ipPayloadLen may be less than requested by IpV4_ProvideTxBuffer().
 *                                             Even if there were multiple buffers provided, only the required number of fragments will be sent.
 *                                             Previously provided but unused buffers will be released by this function.
 *  \param[in]      IpProtocol              IP protocol value. (TCP, UDP, ICMP)
 *  \param[in]      UlTxReqIdx              Upper layer TX request identifier.
 *  \param[in]      Flags                   Any combination of the following flags:
 *                                          - TCPIP_V_IP_TRANSMIT_FLAG_TX_CONFIRMATION:
 *                                            Request TX confirmation from EthIf.
 *                                          - TCPIP_V_IP_TRANSMIT_FLAG_RETRY_ON_PHYS_ADDR_MISS:
 *                                            Do not cancel transmit if link layer address of destination is not (yet)
 *                                            in link layer address resolution cache.
 *  \return         TCPIP_OK                Transmission triggered.
 *  \return         TCPIP_E_NOT_OK          Transmit failed.
 *  \return         TCPIP_E_PHYS_ADDR_MISS  Transmit failed due to ARP cache miss.
 *  \pre            A previous call of IpVx_ProvideTxBuffer() must have been successful.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_VIpTransmit(
  TCPIP_P2C(TcpIp_IpTxRequestDescriptorType) IpTxReqDescPtr,
  uint16                                     IpPayloadLen,
  uint8                                      IpProtocol,
  uint8                                      UlTxReqIdx,
  uint8                                      Flags);

/**********************************************************************************************************************
 *  TcpIp_VIpTransmitCancel()
 *********************************************************************************************************************/
/*! \brief          Cancels the transmit after a preceding call to ProvideTxBuffer.
 *  \details        -
 *  \param[in,out]  IpTxReqDescrPtr       IP TX request descriptor.
 *  \param[in]      UlTxReqTabIdx         TX request index.
 *  \pre            A previous call of IpVx_ProvideTxBuffer() must have been successful.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VIpTransmitCancel(
  TCPIP_P2C(TcpIp_IpTxRequestDescriptorType) IpTxReqDescrPtr,
  uint8                                      UlTxReqTabIdx);

/**********************************************************************************************************************
 *  TcpIp_VRstCtrl()
 *********************************************************************************************************************/
/*! \brief         Resets some variables for one controller
 *  \details       Called in the context of transceiver link loss, or IP address expiration.
 *  \param[in]     LocalAddrIdx     Index of the local address that cannot be used by sockets anymore.
 *                                  \spec requires LocalAddrIdx < TcpIp_GetSizeOfLocalAddr(); \endspec
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VRstCtrl(TcpIp_LocalAddrIterType LocalAddrIdx);

# if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VRstCtrl_TcpListen
 *********************************************************************************************************************/
/*! \brief         Reset the listen socket, depending on the TLS usage.
 *  \details       -
 *  \param[in]     SocketIdx   Index of a TCP socket.
 *                 CONSTRAINT: [0...TcpIp_GetSizeOfSocketDyn()-1]
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_VRstCtrl_TcpListen(
  TcpIp_SocketDynIterType SocketIdx);
# endif

# if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VIncMeasurementDataCounter()
 *********************************************************************************************************************/
/*! \brief         Increments the corresponding measurement data counter.
 *  \details       The function ensures that the data counter does not overflow.
 *  \param[in]     MeasurementIdx         Index of measurement data.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VIncMeasurementDataCounter(
  TcpIp_MeasurementIdxType  MeasurementIdx);
# endif


/**********************************************************************************************************************
 *  TcpIp_ReportSecurityEvent()
 *********************************************************************************************************************/
/*! \brief         Report security event to IdsM module.
 *  \details       The function checks whether the given security event is configured and only forwards configured
 *                 events to the IdsM.
 *  \param[in]     SecurityEventId        Internal TcpIp security event ID
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_ReportSecurityEvent(
  TcpIp_SecurityEventIdType SecurityEventId);

/**********************************************************************************************************************
 * MISCELLANEOUS SERVICE FUNCTIONS - STOP
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * FUNCTIONS CONDUCTING CALLOUTS TO UPPER LAYERS - START
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TcpIp_VPhysAddrTableChg()
 *********************************************************************************************************************/
/*! \brief         Called by the IpV4 or IpV6 in case of a change in the ARP/NDP table related to the
 *                 controller specified by CtrlIdx.
 *  \details       -
 *  \param[in]     CtrlIdx     EthIf controller index of the related ARP/NDP table.
 *                             \spec requires CtrlIdx < TcpIp_GetSizeOfEthIfCtrl(); \endspec
 *  \param[in]     IpAddrPtr   specifies the IP address of the changed ARP/NDP table entry
 *  \param[in]     PhysAddrPtr specifies the physical address of the changed ARP/NDP table entry
 *                             \spec requires $lengthOf(PhysAddrPtr) >= TCPIP_PHYS_ADDR_LEN_BYTE; \endspec
 *  \param[in]     Valid       specifies if the ARP/NDP table entry is added or changed (TRUE) or has been removed (FALSE)
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VPhysAddrTableChg(
  uint8                             CtrlIdx,
  TCPIP_P2V(TcpIp_SockAddrBaseType) IpAddrPtr,
  TCPIP_P2V(uint8)                  PhysAddrPtr,
  boolean                           Valid);

# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 *  TcpIp_VDuplicateAddrDetected()
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about an IPv6 address conflict.
 *  \details       Does NOT trigger the named DEM event.
 *  \param[in]     LocalAddrId        IP address Identifier, representing an IP address specified in the TcpIp module
 *                                    configuration.
 *  \param[in]     SrcAddrPtr         Pointer to the memory where the affected source IP address is stored in
 *                                    network byte order.
 *  \param[in]     RemotePhysAddrPtr  Pointer to the memory where the remote physical address (MAC address) related
 *                                    to the specified IP address is stored in network byte order.
 *  \pre           -
 *  \context       TASK|ISR
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \trace         DSGN-TcpIpDuplicateAddrDetection
 *********************************************************************************************************************/
extern FUNC(void, TCPIP_CODE) TcpIp_VDuplicateAddrDetected(
  TcpIp_LocalAddrIdType LocalAddrId,
  TCPIP_P2C(uint8)      SrcAddrPtr,
  TCPIP_P2C(uint8)      RemotePhysAddrPtr);
# endif

/**********************************************************************************************************************
 *  TcpIp_RxIndicationFunctions()
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about the finalization of a reception of data.
 *  \details       -
 *  \param[in]     SocketIdx         Index of a UDP or TCP socket.
 *                                   \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in]     RemoteSockAddrPtr Address info of the socket connected newly.
 *  \param[in]     DataPtr           Payload of the data received.
 *  \param[in]     DataLenByte       Payload length of the data received.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_RxIndicationFunctions(
  TcpIp_SocketDynIterType           SocketIdx,
  TCPIP_P2V(TcpIp_SockAddrBaseType) RemoteSockAddrPtr,
  TCPIP_P2V(uint8)                  DataPtr,
  uint16                            DataLenByte);

/**********************************************************************************************************************
 *  TcpIp_TxConfirmationFunctions()
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about a transmitted packet.
 *  \details       -
 *  \param[in]     SocketIdx         Index of a UDP or TCP socket.
 *                                   \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in]     DataLenByte       Length of data that was transmitted. [bytes]
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_TxConfirmationFunctions(
  TcpIp_SocketDynIterType SocketIdx,
  uint16                  DataLenByte);

# if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_TcpAcceptedFunctions()
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about a new socket connection on a listen socket and asks them for acceptance.
 *  \details       -
 *  \param[in]     SocketOwnerConfigIdx  Index of a socket owner.
 *                                       \spec requires SocketOwnerConfigIdx < TcpIp_GetSizeOfSocketOwnerConfig(); \endspec
 *  \param[in]     SocketTcpIdx          Index of a TCP socket that is in state listen.
 *                                       \spec requires SocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn(); \endspec
 *  \param[in]     ConnectedSocketTcpIdx Index of a newly allocated TCP socket that represents the accepted connection.
 *                                       \spec requires ConnectedSocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn(); \endspec
 *  \param[in]     RemoteSockAddrPtr     Pointer to remote socket address of the accepted connection.
 *  \return        E_OK                  Upper layer has accepted the new connection.
 *  \return        E_NOT_OK              Upper layer has rejected the new connection.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpAcceptedFunctions(
  TcpIp_SocketOwnerConfigIterType SocketOwnerConfigIdx,
  TcpIp_SocketTcpDynIterType      SocketTcpIdx,
  TcpIp_SocketTcpDynIterType      ConnectedSocketTcpIdx,
  TCPIP_P2V(TcpIp_SockAddrType)   RemoteSockAddrPtr);

/**********************************************************************************************************************
 *  TcpIp_TcpConnectedFunctions()
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about the "connect" event on a socket.
 *  \details       -
 *  \param[in]     SocketTcpIdx      Index of a TCP socket.
 *                                   \spec requires SocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn(); \endspec
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_TcpConnectedFunctions(
  TcpIp_SocketTcpDynIterType SocketTcpIdx);
# endif

/**********************************************************************************************************************
 *  TcpIp_TcpIpEventFunctions()
 *********************************************************************************************************************/
/*! \brief         Informs upper layers about TcpIp events of active sockets.
 *  \details       -
 *  \param[in]     SocketOwnerConfigIdx Index of a socket owner.
 *                                      \spec requires SocketOwnerConfigIdx < TcpIp_GetSizeOfSocketOwnerConfig(); \endspec
 *  \param[in]     SocketIdx            Index of the TCP/UDP socket.
 *                                      \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in]     EventType            Status event that occurred in the connection related to the given socket.
 *  \pre           -
 *  \note          Use TcpIp_TcpIpClosedEventFunctions() for the events TCPIP_TCP_CLOSED and TCPIP_UDP_CLOSED.
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_TcpIpEventFunctions(
  TcpIp_SocketOwnerConfigIterType SocketOwnerConfigIdx,
  TcpIp_SocketDynIterType         SocketIdx,
  IpBase_TcpIpEventType           EventType);

/**********************************************************************************************************************
 *  TcpIp_CopyTxDataFunctions()
 *********************************************************************************************************************/
/*! \brief         Calls the configured CopyTxData function of the socket owner.
 *  \details       -
 *  \param[in]     SocketIdx         Index of a UDP or TCP socket.
 *                                   \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[out]    BufferPtr         Buffer into which the socket owner shall copy the data.
 *  \param[in,out] BufferLenBytePtr  Length of the TCP buffer [byte].
 *                                   Out: Updated value if the socket owner has written less bytes.
 *                                   \spec requires $lengthOf(BufferPtr) >= *BufferLenBytePtr; \endspec
 *  \param[in]     UseTlsCopyTxData  Indicates which CopxTxData function should be used:
 *                                   TRUE:  TLS CopyTxData function with record layer transformation is used.
 *                                   FALSE: Configured CopyTxData callback of socket owner is used.
 *  \return        BUFREQ_OK         Data was copied.
 *  \return        BUFREQ_E_NOT_OK   No data copied. Transmit shall be aborted.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_CopyTxDataFunctions(
  TcpIp_SizeOfSocketDynType SocketIdx,
  TCPIP_P2V(uint8)          BufferPtr,
  TCPIP_P2V(uint16)         BufferLenBytePtr,
  boolean                   UseTlsCopyTxData);

/**********************************************************************************************************************
 *  TcpIp_LocalIpAssignmentChgFunctions()
 *********************************************************************************************************************/
/*! \brief         Calls the configured IP address assignment change callbacks of the socket owners.
 *  \details       -
 *  \param[in]     LocalAddrIdx     Index of a local IPv4 or IPv6 address.
 *                                  \spec requires LocalAddrIdx < TcpIp_GetSizeOfLocalAddr(); \endspec
 *  \param[in]     AddrState        Assignment state of the IP address. (ASSIGNED, UNASSIGNED)
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_LocalIpAssignmentChgFunctions(
  TcpIp_LocalAddrIterType LocalAddrIdx,
  TcpIp_IpAddrStateType   AddrState);

# if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VSockTcpStateChg()
 *********************************************************************************************************************/
/*! \brief         Conducts the "TCP socket state change" event callback to upper layers.
 *  \details       The callback is conducted for the given socket index and socket user.
 *  \param[in]     SocketIdx               Index of a TCP socket.
 *                                         \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in]     FormerSocketOwnerCfgIdx Index of the owner that is (or was) assigned to the socket before closing.
 *                                         \spec requires FormerSocketOwnerCfgIdx < TcpIp_GetSizeOfSocketOwnerConfig(); \endspec
 *  \param[in]     State                   New socket state.
 *  \param[in]     SocketUsesTls           Indicates whether the socket uses TLS.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VSockTcpStateChg(
  TcpIp_SocketDynIterType         SocketIdx,
  TcpIp_SocketOwnerConfigIterType FormerSocketOwnerCfgIdx,
  TcpIp_StateType                 State,
  boolean                         SocketUsesTls);
# endif

/**********************************************************************************************************************
 *  TcpIp_VSockTxConfirmation()
 *********************************************************************************************************************/
/*! \brief         Conducts the transmit confirmation callback to upper layers.
 *  \details       -
 *  \param[in]     SocketIdx         Index of a UDP or TCP socket.
 *                                   \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in]     DataLenByte       Length of transmitted data. [bytes]
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VSockTxConfirmation(
  TcpIp_SocketDynIterType SocketIdx,
  uint32                  DataLenByte);

/**********************************************************************************************************************
 *  TcpIp_Cbk_VLocalIpAssignmentChg()
 *********************************************************************************************************************/
/*! \brief         Conducts the IP state change event callback to upper layers.
 *  \details       The callback is conducted to for the given local address index
 *  \param[in]     LocalAddrIdx          Index of a local address.
 *                                       \spec requires LocalAddrIdx < TcpIp_GetSizeOfLocalAddr(); \endspec
 *  \param[in]     State                 New state of address (ASSIGNED, UNASSIGNED, ONHOLD).
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VLocalIpAssignmentChg(
  TcpIp_LocalAddrIterType LocalAddrIdx,
  TcpIp_IpAddrStateType   State);

/**********************************************************************************************************************
 *  TcpIp_Cbk_VPathMtuChg()
 *********************************************************************************************************************/
/*! \brief          Conducts the "path MTU has changed" event callout to upper layers.
 *  \details        The callback is conducted for the given ethernet ctrl index and destination address.
 *  \param[in]      CtrlIdx               controller index
 *                                        \spec requires CtrlIdx < TcpIp_GetSizeOfEthIfCtrl(); \endspec
 *  \param[in]      SockAddrPtr           net address that changed its state
 *  \param[in]      PathMtuSize           new size of the Path-MTU (transport layer part)
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VPathMtuChg(
  uint8                             CtrlIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockAddrPtr,
  uint16                            PathMtuSize);

/**********************************************************************************************************************
 *  TcpIp_Cbk_VAddrResTimeout()
 *********************************************************************************************************************/
/*! \brief          Notifies the TcpIp that the link layer address resolution for the specified destination timed out.
 *  \details        -
 *  \param[in]      DestAddrPtr    Pointer to remote IP address for which the link layer address could not be resolved.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VAddrResTimeout(
  TCPIP_P2C(TcpIp_SockAddrBaseType) DestAddrPtr);

# if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VSockTcpStateChgAccepted()
 *********************************************************************************************************************/
/*! \brief          Conducts the "TCP state change accepted" event callback to upper layers.
 *  \details        This function is derived from TcpIp_VSockTcpStateChg but implements a the following special handling:
 *                  1. It provides a return value from the called upper layer function.
 *                  2. It checks the given SoUserIdx.
 *                  3. It looks up the ListenSockIdx using the runtime-data-array TcpIp_SocketList at index SockIdx.
 *  \param[in]      SocketTcpIdx          Index of a TCP socket.
 *                                        \spec requires SocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn(); \endspec
 *  \return         E_OK                  The user accepted the accepted connection.
 *                  E_NOT_OK              The user refused the accepted connection.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VSockTcpStateChgAccepted(
  TcpIp_SocketTcpDynIterType SocketTcpIdx);
# endif
/* (STD_ON == TCPIP_SUPPORT_TCP) */

/**********************************************************************************************************************
 * FUNCTIONS CONDUCTING CALLOUTS TO UPPER LAYERS - STOP
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * PUBLIC FUNCTIONS OF THE DYNAMIC LIST FUNCTIONALITY ("DList") - START
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TcpIp_DListInitDesc()
 *********************************************************************************************************************/
/*! \brief          Initializes a Dlist descriptor. All nodes from the node storage array will be linked and inserted
 *                  into the free list. This descriptor can be used by multiple DList instances. See TcpIp_DListInit().
 *  \details        -
 *  \param[in]      DescPtr       Descriptor to be initialized.
 *  \param[in]      NodePtr       Node storage array
 *  \param[in]      NodeCount     Number of nodes in node storage array
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DListInitDesc(
  TCPIP_P2V(TcpIp_DListDescType) DescPtr,
  TCPIP_P2V(TcpIp_DListNodeType) NodePtr,
  uint8                          NodeCount);

/**********************************************************************************************************************
 *  TcpIp_DListInit()
 *********************************************************************************************************************/
/*! \brief          Initializes a DList instance. After initialization the list is empty.
 *                  New elements can be inserted by TcpIp_DListAddElement().
 *  \details        -
 *  \param[in]      ListPtr       DList instance
 *  \param[in]      DescPtr       DList descriptor that will by used by this instance.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DListInit(
  TCPIP_P2V(TcpIp_DListType)     ListPtr,
  TCPIP_P2V(TcpIp_DListDescType) DescPtr);

/**********************************************************************************************************************
 *  TcpIp_DListAddElement()
 *********************************************************************************************************************/
/*! \brief          Adds an element to a DList instance by moving it from the free list.
 *  \details        -
 *  \param[in]      DListPtr   Handle to the list into that the new element should be inserted.
 *                             \spec requires DListPtr != NULL_PTR; \endspec
 *  \param[in]      AfterIdx   Index of a list item that shall be the predecessor of the new item.
 *                             If the index is TCPIP_DLIST_END_IDX the new item will be inserted at the beginning of the list.
 *  \param[out]     NewIdxPtr  Index of the inserted item. Only valid if result is E_OK.
 *                             \spec requires NewIdxPtr != NULL_PTR; \endspec
 *  \return         E_OK       success
 *  \return         E_NOT_OK   No elements in free list. (NewIdxPtr not modified)
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DListAddElement(
  TCPIP_P2V(TcpIp_DListType)    DListPtr,
  TcpIp_DListIdxType            AfterIdx,
  TCPIP_P2V(TcpIp_DListIdxType) NewIdxPtr);

/**********************************************************************************************************************
 *  TcpIp_DListRemoveElement()
 *********************************************************************************************************************/
/*! \brief          Removes a single element from a DList instance and adds it to the free list of the descriptor
 *  \details        -
 *  \param[in]      DListPtr  Handle to the DList instance containing the element to be removed.
 *  \param[in]      RemIdx    Index of the element that shall be removed.
 *                            CONSTRAINT: [0...<#Elements at DListPtr>-1]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DListRemoveElement(
  TCPIP_P2V(TcpIp_DListType) DListPtr,
  TcpIp_DListIdxType         RemIdx);

/**********************************************************************************************************************
 *  TcpIp_DListRemoveElementRange()
 *********************************************************************************************************************/
/*! \brief          Removes a chain of linked elements from the list and adds them to the free list of the descriptor.
 *  \details        -
 *  \param[in]      DListPtr    Handle to the DList instnace containing the elements to be removed.
 *                              \spec requires DListPtr != NULL_PTR; \endspec
 *  \param[in]      RemStartIdx Index of the first item of the element chain that shall be removed.
 *                              CONSTRAINT: [0...<#Elements at DListPtr>-1]
 *                              \spec requires RemStartIdx < $lengthOf(DListPtr); \endspec
 *  \param[in]      RemEndIdx   Index of the lest item of the element chain that shall be removed.
 *                              CONSTRAINT: [0...<#Elements at DListPtr>-1]
 *                              \spec requires RemEndIdx < $lengthOf(DListPtr); \endspec
 *  \note           RemStartIdx and RemEndIdx may be equal but for performance reasons TcpIp_DListRemoveElement() should be
 *                  used for removing single elements.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DListRemoveElementRange(
  TCPIP_P2V(TcpIp_DListType) DListPtr,
  TcpIp_DListIdxType         RemStartIdx,
  TcpIp_DListIdxType         RemEndIdx);

/**********************************************************************************************************************
 *  TcpIp_DListClear()
 *********************************************************************************************************************/
/*! \brief          Clears a DList instance by moving all elements to the free list of the descriptor.
 *  \details        -
 *  \param[in]      DListPtr  Handle to the DList instance.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_DListClear(
  TCPIP_P2V(TcpIp_DListType) DListPtr);

/**********************************************************************************************************************
 * PUBLIC FUNCTIONS OF THE DYNAMIC LIST FUNCTIONALITY ("DList") - STOP
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TcpIp_VIsPortAndAddrUsedBySocket()
 *********************************************************************************************************************/
/*! \brief          Checks whether any active socket in the specified range uses the given IP address and port.
 *  \details        -
 *  \param[in]      Port                Port number (host byte order)
 *  \param[in]      LocalAddrIdx        Index of a local address.
 *                                      \spec requires LocalAddrIdx < TcpIp_GetSizeOfLocalAddr() || LocalAddrIdx == TCPIP_LOCALADDRID_ANY; \endspec.
 *  \param[in]      SocketStartIdx      Index of the first a UDP/TCP socket.
 *                                      \spec requires SocketStartIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in]      SocketEndIdx        Index of the last a UDP/TCP socket + 1.
 *                                      CONSTRAINT: [SocketStartIdx...TcpIp_GetSizeOfSocketDyn()]
 *                                      If SocketStartIdx <= SocketEndIdx the function returns FALSE.
 *  \param[in]      SocketIgnoreIdx     Index of a UDP/TCP socket that shall be ignored.
 *                                      If the value is in range [SocketStartIdx...SocketEndIdx-1] this socket
 *                                      will be ignored.
 *  \return         TRUE                A matching socket was found.
 *  \return         FALSE               No matching socket was found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_VIsPortAndAddrUsedBySocket(
  uint16                    Port,
  TcpIp_LocalAddrIterType   LocalAddrIdx,
  TcpIp_SocketDynIterType   SocketStartIdx,
  TcpIp_SocketDynIterType   SocketEndIdx,
  TcpIp_SocketDynIterType   SocketIgnoreIdx);

/**********************************************************************************************************************
 *  TcpIp_VFindUnusedDynamicPort()
 *********************************************************************************************************************/
/*! \brief          Selects a port number from the dynamic port range that is not currently in use by another socket.
 *  \details        -
 *  \param[in]      Protocol            IP Protocol.
 *                                      CONSTRAINT: [TCPIP_PROTOCOL_UDP, TCPIP_PROTOCOL_TCP]
 *  \param[in]      LocalAddrIdx        Index of a local address.
 *                                      \spec requires LocalAddrIdx < TcpIp_GetSizeOfLocalAddr() || LocalAddrIdx == TCPIP_LOCALADDRID_ANY; \endspec.
 *  \param[in]      SocketStartIdx      Index of the first a UDP/TCP socket.
 *                                      \spec requires SocketStartIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in]      SocketEndIdx        Index of the last a UDP/TCP socket + 1.
 *                                      CONSTRAINT: [SocketStartIdx...TcpIp_GetSizeOfSocketDyn()]
 *                                      If SocketStartIdx <= SocketEndIdx the function returns FALSE.
 *  \param[in]      SocketIgnoreIdx     Index of a UDP/TCP socket that shall be ignored.
 *                                      If the value is in range [SocketStartIdx...SocketEndIdx-1] this socket
 *                                      will be ignored.
 *  \return         > 0                 Port number that is not in used by any other socket.
 *  \return         TCPIP_PORT_NOT_SET  No unused port was found.
 *  \note                               The current implementation will never return TCPIP_PORT_NOT_SET since it just
 *                                      iterates sequentially over the dynamic port range and will find an unused port
 *                                      after a maximum of (SocketEndIdx - SocketStartIdx) iterations.
 *                                      But future implementations may not guarantee this behavior and therefore the
 *                                      caller must be able to handle the raturn value TCPIP_PORT_NOT_SET.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(uint16, TCPIP_CODE) TcpIp_VFindUnusedDynamicPort(
  uint8                     Protocol,
  TcpIp_LocalAddrIterType   LocalAddrIdx,
  TcpIp_SocketDynIterType   SocketStartIdx,
  TcpIp_SocketDynIterType   SocketEndIdx,
  TcpIp_SocketDynIterType   SocketIgnoreIdx);

/**********************************************************************************************************************
 *  TcpIp_VGetNextDynamicPort()
 *********************************************************************************************************************/
/*! \brief          Returns the next number from the dynamic port range (49152-65535) defined by tha IANA.
 *  \details        This function uses a counter and will return the next port within the dynamic port range considering
 *                  the overflow.
 *  \param[in]      Protocol            IP Protocol identifier.
 *                                      CONSTRAINT: [TCPIP_PROTOCOL_UDP, TCPIP_PROTOCOL_TCP]
 *  \return                             Dynamic port number.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(uint16, TCPIP_CODE) TcpIp_VGetNextDynamicPort(uint8 Protocol);

/**********************************************************************************************************************
 *  TcpIp_VSetSocketSpecificFramePriority()
 *********************************************************************************************************************/
/*! \brief          Sets the ethernet frame priority value for all outgoing packets of the given socket.
 *  \details        -
 *  \param[in]      SocketIdx           Index of a UDP or TCP socket.
 *                                      \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in]      Value               EthIf frame priority.
 *                                      CONSTRAINT: [0..7]
 *  \return         E_OK                The value has been updated in the configured IP submodules.
 *  \return         E_NOT_OK            IP submodule returned negative result. (Value not in range)
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different socket IDs.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VSetSocketSpecificFramePriority(
  TcpIp_SocketDynIterType SocketIdx,
  uint8                   Value);

# if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VGetSocketSpecificFramePriority()
 *********************************************************************************************************************/
/*! \brief          Reads the ethernet frame priority value from listen socket,set through TcpIp_ChangeParameter.
 *  \details        -
 *  \param[in]      SocketIdx           Index of a UDP or TCP socket.
 *                                      \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in,out]  Priority            Pointer to EthIf frame priority.
 *                                      \spec requires Priority != NULL_PTR; \endspec
 *  \return         E_OK                The value has been read from the configured IP submodules.
 *  \return         E_NOT_OK            IP submodule returned negative result. (Value not in range)
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different socket IDs.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VGetSocketSpecificFramePriority(
  TcpIp_SocketDynIterType           SocketIdx,
  TCPIP_P2V(uint8)                  Priority);

/**********************************************************************************************************************
 *  TcpIp_VCopySocketSpecificFramePriority()
 *********************************************************************************************************************/
/*! \brief         Copy the ethernet frame priority value from listen socket to new(duplicate) created Tcp socket.
 *  \details        -
 *  \param[in]      SocketIdx           Index of new created duplicate socket.
 *                                      \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in]      ListenSocketIdx     Index of a Listen socket.
 *                                      \spec requires ListenSocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different socket IDs.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VCopySocketSpecificFramePriority(
  TcpIp_SocketDynIterType      SocketIdx,
  TcpIp_SocketDynIterType      ListenSocketIdx);

/**********************************************************************************************************************
 *  TcpIp_SearchTcpListenSocket()
 *********************************************************************************************************************/
/*! \brief          Search for TCP listen sockets that fits for the given IP address.
 *  \details        When AcceptAnyIp is TRUE sockets bound to ANY-IP will be accepted when no better macht is available.
 *  \param[in]      RxSockLocAddrPtr    Local address of the received packet.
 *                                      CONSTRAINT: [Must be non null pointer]
 *  \param[in]      LocalAddrIdx        Index of the local address.
 *                                      CONSTRAINT: [0...TcpIp_GetSizeOfLocalAddr()-1]
 *  \param[in]      AcceptAnyIp         Socket with ANY-IP are acceptable
 *  \return                             Index of the found socket. TcpIp_GetSizeOfSocketDyn() when no socket is found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(TcpIp_SocketDynIterType, TCPIP_CODE) TcpIp_SearchTcpListenSocket(
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TcpIp_LocalAddrIterType           LocalAddrIdx,
  boolean                           AcceptAnyIp
);

/**********************************************************************************************************************
 *  TcpIp_SearchTcpComSocket()
 *********************************************************************************************************************/
/*! \brief          Search a TCP sockets that fits to the local and remote port and IP address.
 *  \details        -
 *  \param[in]      LocalAddrIdx        Index of the local addres.
 *  \param[in]      RxSockLocAddrPtr    Local address of the received packet.
 *                                      CONSTRAINT: [Must be non null pointer]
 *  \param[in]      RxSockRemAddrPtr    Local address of the received packet.
 *                                      CONSTRAINT: [Must be non null pointer]
 *  \return                             Index of the found socket. TcpIp_GetSizeOfSocketDyn() when no socket is found.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(TcpIp_SocketDynIterType, TCPIP_CODE) TcpIp_SearchTcpComSocket(
  TcpIp_LocalAddrIterType           LocalAddrIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockRemAddrPtr
);
# endif

/**********************************************************************************************************************
 *  TcpIp_VIpV4NetmaskToCidr()
 **********************************************************************************************************************/
/*! \brief          Converts an IPv4 netmask to the CIDR notation.
 *  \details        The function counts the number of set most significant bits.
 *  \param[in]      IpV4Netmask         IPv4 netmask. (i.e.: 255.255.0.0 = 0xFFFF0000)
 *  \return                             CIDR notation of the netmask. (i.e.: 16 for IpV4Netmask = 0xFFFF0000)
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(uint8, TCPIP_CODE) TcpIp_VIpV4NetmaskToCidr(IpBase_AddrInType IpV4Netmask);

/**********************************************************************************************************************
 *  TcpIp_VRequestComModeInternal()
 *********************************************************************************************************************/
/*! \brief         Forwards the communication mode state change request to internal function.
 *  \details        Forwards the requested ComMode state to the transceiver link state change of the IP layer.
 *  \param[in]      CtrlIdx             Index of a Eth controller.
 *                                      \spec requires CtrlIdx < TcpIp_GetSizeOfEthIfCtrl(); \endspec
 *  \param[in]      CtrlState           Requested state of the Eth controller.
 *                                      CONSTRAINT: [TCPIP_STATE_OFFLINE...TCPIP_STATE_ONLINE]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE for different CtrlIdx.
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_VRequestComModeInternal(
  uint8                                     CtrlIdx,
  CONST(TcpIp_StateType, TCPIP_CONST)       CtrlState);

# if ((TCPIP_SUPPORT_DHCPV4 == STD_ON) || (TCPIP_SUPPORT_IPV6 == STD_ON))                                               /* COV_TCPIP_DHCPV4_OR_IPV6_SUPPORT_ALWAYS_OFF */
/**********************************************************************************************************************
 * TcpIp_VGetRandomValue()
 *********************************************************************************************************************/
/*! \brief        Generate random value within given range
 *  \details      -
 *  \param[in]    MinValue      Lower range
 *  \param[in]    MaxValue      Upper range
 *  \return       RndValue      Random value generated
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
*********************************************************************************************************************/
FUNC(uint32, TCPIP_CODE) TcpIp_VGetRandomValue(
  uint32 MinValue,
  uint32 MaxValue);
# endif

/**********************************************************************************************************************
 *  TcpIp_LocalAddrIdxIsOnHold()
 **********************************************************************************************************************/
/*! \brief          Verify if the controller related to this local addr is in state OnHold
 *  \details        For IPv4 addresses the current state of the related IPv6 ctrl is checked.
 *                  For IPv6 addresses the current state of the related IPv4 ctrl is checked.
 *  \param[in]      LocalAddrIdx  Idx of the address
 *  \return         TRUE          OnHold is active
 *  \return         FALSE         OnHold is deactivate
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_LocalAddrIdxIsOnHold(
  TcpIp_LocalAddrIterType LocalAddrIdx);

/**********************************************************************************************************************
 *  TcpIp_SocketIsOnHold()
 **********************************************************************************************************************/
/*! \brief          Verify if the controller related to this socket is in state OnHold
 *  \details        -
 *  \param[in]      SocketId    Id of the socket
 *  \return         TRUE        OnHold is active
 *  \return         FALSE       OnHold is deactivate
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_SocketIsOnHold(
  TcpIp_SocketIdType SocketId);

/**********************************************************************************************************************
 *  TcpIp_ChangeCtrlStateNotification()
 **********************************************************************************************************************/
/*! \brief          Notify the EthSm about a controller state change.
 *  \details        For dual stack controllers, the EthSm is only notified if the overall state changes.
 *  \param[in]      EthIfCtrlIdx  Idx of the EthCtrl
 *  \param[in]      TcpIpState    New state
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_ChangeCtrlStateNotification(
  uint8 EthIfCtrlIdx,
  TcpIp_StateType TcpIpState);

/**********************************************************************************************************************
 *  TcpIp_VerifyAddrIdAcceptable()
 **********************************************************************************************************************/
 /*! \brief          Verify if the combination of BindLocalAddrIdx and RecvLocalAddrIdx can be used for receiving of messages.
  *  \details        -
  *  \param[in]      BindLocalAddrIdx  Idx of the address that is bound to the socket.
  *  \param[in]      RecvLocalAddrIdx  Idx of the address that receive the message.
  *  \param[out]     MatchType         Indicate how good the addresses fits.
  *                                    CONSTRAINT: [Must be non null pointer]
  *  \return         TRUE              Receiving in this combination is possible.
  *  \return         FALSE             Receiving is this combination not possible.
  *  \pre            -
  *  \context        TASK|ISR2
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *********************************************************************************************************************/
FUNC(boolean, TCPIP_CODE) TcpIp_VerifyAddrIdAcceptable(
  TcpIp_LocalAddrIterType   BindLocalAddrIdx,
  TcpIp_LocalAddrIterType   RecvLocalAddrIdx,
  TCPIP_P2V(uint8)          MatchType);

/**********************************************************************************************************************
 *  TcpIp_AddPartialChecksums()
 **********************************************************************************************************************/
/*! \brief           Add two partial not finalized checksum
  *  \details        -
  *  \param[in]      Crc1              Partial checksum 1
  *  \param[in]      Crc2              Partial checksum 2
  *  \return                           The result of the checksums
  *  \pre            -
  *  \context        TASK|ISR2
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *********************************************************************************************************************/
FUNC(uint32, TCPIP_CODE) TcpIp_AddPartialChecksums(
  uint32 Crc1,
  uint32 Crc2
);

# define TCPIP_STOP_SEC_CODE
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif
  /* TCPIP_PRIV_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Priv.h
 *********************************************************************************************************************/
