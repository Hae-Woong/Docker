/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  SoAd_LocalAddr.h
 *         \unit  LocalAddr
 *        \brief  Header file of the LocalAddr unit for AUTOSAR Socket Adaptor module.
 *      \details  Interface and type definitions of the LocalAddr unit.
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

#if !defined (SOAD_LOCALADDR_H)
# define SOAD_LOCALADDR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "SoAd_Types.h"

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
 *  SoAd_RequestIpAddrAssignment()
 *********************************************************************************************************************/
/*! \brief       Requests IP address assignment on a local address identified by a socket connection.
 *  \details     -
 *  \param[in]   SoConId          Socket connection identifier.
 *  \param[in]   Type             IP address type.
 *  \param[in]   LocalIpAddrPtr   Pointer to IP address which shall be assigned.
 *                                [Points to one of the following structs depending on value of struct element domain:
 *                                  - SoAd_SockAddrInetType for IPv4
 *                                  - SoAd_SockAddrInet6Type for IPv6]
 *  \param[in]   Netmask          Netmask in CIDR.
 *  \param[in]   DefaultRouterPtr Pointer to default router (gateway) address.
 *                                [Type == SOAD_IPADDR_ASSIGNMENT_STATIC:
 *                                  ->  DefaultRouterPtr != NULL_PTR
 *                                      Points to one of the following structs depending on configured IP address
 *                                      version of parameter SoConIdx:
 *                                        - SoAd_SockAddrInetType for IPv4
 *                                        - SoAd_SockAddrInet6Type for IPv6
 *                                Type != SOAD_IPADDR_ASSIGNMENT_STATIC:
 *                                  ->  DefaultRouterPtr == NULL_PTR]
 *  \return      E_OK             Assignment request was accepted.
 *  \return      E_NOT_OK         Assignment request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different local IP addresses, FALSE for same local IP address.
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_RequestIpAddrAssignment(
  SoAd_SoConIdType SoConId,
  SoAd_IpAddrAssignmentType Type,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalIpAddrPtr,
  uint8 Netmask,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr);

/**********************************************************************************************************************
 *  SoAd_ReleaseIpAddrAssignment()
 *********************************************************************************************************************/
/*! \brief       Releases IP address assignment on a local address identified by a socket connection.
 *  \details     -
 *  \param[in]   SoConId          Socket connection identifier.
 *  \return      E_OK             Release request was accepted.
 *  \return      E_NOT_OK         Release request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different local IP addresses, FALSE for same local IP address.
 *  \synchronous TRUE
 *  \trace       CREQ-138021
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReleaseIpAddrAssignment(
  SoAd_SoConIdType SoConId);

/**********************************************************************************************************************
 *  SoAd_GetLocalAddr()
 *********************************************************************************************************************/
/*! \brief       Returns a local IP address identified by a socket connection.
 *  \details     -
 *  \param[in]   SoConId          Socket connection identifier.
 *  \param[out]  LocalAddrPtr     Pointer to local address (IP and Port).
 *                                [Points to one of the following structs depending on value of struct element domain:
 *                                  - SoAd_SockAddrInetType for IPv4
 *                                  - SoAd_SockAddrInet6Type for IPv6]
 *  \param[out]  NetmaskPtr       Pointer to network mask (CIDR Notation).
 *  \param[out]  DefaultRouterPtr Pointer to default router (gateway).
 *                                [Points to one of the following structs depending on configured IP address
 *                                version of parameter SoConId:
 *                                  - SoAd_SockAddrInetType for IPv4
 *                                  - SoAd_SockAddrInet6Type for IPv6]
 *  \return      E_OK             Request was accepted.
 *  \return      E_NOT_OK         Request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-123308
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetLocalAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) NetmaskPtr,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr);

/**********************************************************************************************************************
 *  SoAd_ReadDhcpHostNameOption()
 *********************************************************************************************************************/
/*! \brief          Returns the DHCP hostname option currently configured on a local interface identified by a
 *                  socket connection.
 *  \details        -
 *  \param[in]      SoConId   Socket connection identifier.
 *  \param[in,out]  length    Length of buffer for hostname (length of provided buffer, updated to length of hostname).
 *  \param[out]     data      Pointer to buffer for hostname.
 *  \return         E_OK      Request was accepted.
 *  \return         E_NOT_OK  Request was not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-123281
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReadDhcpHostNameOption(
  SoAd_SoConIdType SoConId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) length,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) data);

/**********************************************************************************************************************
 *  SoAd_WriteDhcpHostNameOption()
 *********************************************************************************************************************/
/*! \brief        Sets the DHCP hostname option on a local interface identified by a socket connection.
 *  \details      -
 *  \param[in]    SoConId   Socket connection identifier.
 *  \param[in]    length    Length of buffer for hostname.
 *  \param[in]    data      Pointer to buffer for hostname.
 *  \return       E_OK      Request was accepted.
 *  \return       E_NOT_OK  Request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \trace        CREQ-138020
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_WriteDhcpHostNameOption(
  SoAd_SoConIdType SoConId,
  uint8 length,
  P2CONST(uint8, AUTOMATIC, SOAD_APPL_VAR) data);

/**********************************************************************************************************************
 *  SoAd_ReadDhcpOption()
 *********************************************************************************************************************/
/*! \brief          Returns a DHCP option on a local address.
 *  \details        -
 *  \param[in]      IpAddrId      IP address identifier.
 *  \param[in]      Option        DHCP option code.
 *  \param[in,out]  DataLengthPtr Pointer to length of option buffer (updated to length of option on return).
 *  \param[out]     DataPtr       Pointer to option buffer.
 *  \return         E_OK          Request was accepted.
 *  \return         E_NOT_OK      Request was not accepted.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \trace          CREQ-189730
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReadDhcpOption(
  SoAd_LocalAddrIdType IpAddrId,
  uint16 Option,
  P2VAR(uint16, AUTOMATIC, SOAD_APPL_VAR) DataLengthPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) DataPtr);

/**********************************************************************************************************************
 *  SoAd_WriteDhcpOption()
 *********************************************************************************************************************/
/*! \brief       Sets a DHCP option on a local address.
 *  \details     -
 *  \param[in]   IpAddrId    IP address identifier.
 *  \param[in]   Option      DHCP option code.
 *  \param[in]   DataLength  Length of option buffer.
 *  \param[in]   DataPtr     Pointer to option buffer.
 *  \return      E_OK        Request was accepted.
 *  \return      E_NOT_OK    Request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-189731
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_WriteDhcpOption(
  SoAd_LocalAddrIdType IpAddrId,
  uint16 Option,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, SOAD_APPL_VAR) DataPtr);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* !defined(SOAD_LOCALADDR_H) */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_LocalAddr.h
 *********************************************************************************************************************/
