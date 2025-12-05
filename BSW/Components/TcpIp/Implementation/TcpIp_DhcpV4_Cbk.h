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
 *         \file  TcpIp_DhcpV4_Cbk.h
 *        \brief  Implementation of Dynamic Host Configuration Protocol for IPv4 - Client (DHCPv4 Client) - Callback Header
 *
 *      \details  This file is part of the TcpIp IPv4 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv4 submodule. >> TcpIp_IpV4.h / TcpIp.h (Version 8.xx.xx and higher)
 *********************************************************************************************************************/

#if !defined (TCPIP_DHCPV4_CBK_H)
# define TCPIP_DHCPV4_CBK_H

# include "TcpIp_Cfg.h"
# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#  include "Std_Types.h"
#  include "TcpIp.h"
#  include "TcpIp_Cfg.h"
#  include "IpBase_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#  define TCPIP_START_SEC_CODE
#  include "TcpIp_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  TcpIp_DhcpV4_RxIndication
 *********************************************************************************************************************/
/*! \brief      Receive Indication Callback
 *  \details    -
 *  \param[in]  IpV4CtrlIdx          Controller index of the received message
 *                                   \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]  DataPtr              Pointer to the received data
 *  \param[in]  DataLen              Length of the received data
 *  \param[in]  SourcePtr            Source network address and port
 *  \param[in]  DestinationPtr       Destination network address and port
 *  \param[in]  ChecksumCalculated   Indicates if the UDP checksum is already checked by lower layer
 *  \return     E_OK                 UDP message was processed by DHCPv4
 *  \return     TCPIP_DHCPV4_NOTDHCP UDP message was not processed by DHCPv4
 *  \return     E_NOT_OK             DHCP message is invalid
 *  \pre        -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DhcpV4_RxIndication(
  TcpIp_IpV4CtrlIterType                                     IpV4CtrlIdx,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA)                 DataPtr,
  uint16                                                     DataLen,
  P2CONST(IpBase_SockAddrInType, AUTOMATIC, TCPIP_APPL_DATA) SourcePtr,
  P2CONST(IpBase_SockAddrInType, AUTOMATIC, TCPIP_APPL_DATA) DestinationPtr,
  boolean                                                    ChecksumCalculated);

#  define TCPIP_STOP_SEC_CODE
#  include "TcpIp_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (TCPIP_SUPPORT_IPV4 == STD_ON) */
#endif /* TCPIP_DHCPV4_CBK_H */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_DhcpV4_Cbk.h
 *********************************************************************************************************************/
