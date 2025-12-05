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
/*!        \file  SoAd_RemoteAddr.h
 *         \unit  RemoteAddr
 *        \brief  Header file of the RemoteAddr unit for AUTOSAR Socket Adaptor module.
 *      \details  Interface and type definitions of the RemoteAddr unit.
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

#if !defined (SOAD_REMOTEADDR_H)
# define SOAD_REMOTEADDR_H

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
 *  SoAd_GetRemoteAddr()
 *********************************************************************************************************************/
/*! \brief       Returns the remote address of a socket connection.
 *  \details     -
 *  \param[in]   SoConId    Socket connection identifier.
 *  \param[out]  IpAddrPtr  Pointer to remote address.
 *                          [Points to one of the following structs depending on configured IP address version of
 *                          parameter SoConId:
 *                            - SoAd_SockAddrInetType for IPv4
 *                            - SoAd_SockAddrInet6Type for IPv6]
 *  \return      E_OK       Request was accepted.
 *  \return      E_NOT_OK   Request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr);

/**********************************************************************************************************************
 *  SoAd_GetRemoteAddrState()
 *********************************************************************************************************************/
/*! \brief       Returns the remote address and remote address state of a socket connection.
 *  \details     -
 *  \param[in]   SoConId       Socket connection identifier.
 *  \param[out]  IpAddrPtr     Pointer to remote address.
 *                             [Points to one of the following structs depending on configured IP address version of
 *                             parameter SoConId:
 *                               - SoAd_SockAddrInetType for IPv4
 *                               - SoAd_SockAddrInet6Type for IPv6]
 *  \param[out]  RemAddrState  Pointer to remote address state.
 *  \return      E_OK          Request was accepted.
 *  \return      E_NOT_OK      Request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \trace       CREQ-123315
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRemoteAddrState(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr,
  P2VAR(SoAd_RemAddrStateType, AUTOMATIC, SOAD_APPL_VAR) RemAddrState);

/**********************************************************************************************************************
 *  SoAd_GetRcvRemoteAddr()
 *********************************************************************************************************************/
/*! \brief       Returns the remote address of the last received message on a socket connection.
 *  \details     -
 *  \param[in]   SoConId    Socket connection identifier.
 *  \param[out]  IpAddrPtr  Pointer to remote address.
 *                          [Points to one of the following structs depending on configured IP address version of
 *                          parameter SoConId:
 *                            - SoAd_SockAddrInetType for IPv4
 *                            - SoAd_SockAddrInet6Type for IPv6]
 *  \return      E_OK       Request was accepted.
 *  \return      E_NOT_OK   Request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRcvRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr);

/**********************************************************************************************************************
 *  SoAd_SetRemoteAddr()
 *********************************************************************************************************************/
/*! \brief       Sets the remote address of a socket connection.
 *  \details     -
 *  \param[in]   SoConId        Socket connection identifier.
 *  \param[in]   RemoteAddrPtr  Pointer to remote address.
 *                              [Points to one of the following structs depending on configured IP address version of
 *                              parameter SoConId:
 *                                - SoAd_SockAddrInetType for IPv4
 *                                - SoAd_SockAddrInet6Type for IPv6]
 *  \return      E_OK           Request was accepted.
 *  \return      E_NOT_OK       Request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection identifiers, FALSE for same socket connection identifier.
 *  \synchronous TRUE
 *  \note        The remote address cannot be set while transmission or reception is active.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr);

/**********************************************************************************************************************
 *  SoAd_SetUniqueRemoteAddr()
 *********************************************************************************************************************/
/*! \brief       Sets the remote address of a suitable socket connection in a socket connection group.
 *  \details     Considers the best match algorithm to select the socket connection.
 *  \param[in]   SoConId             Socket connection identifier (any socket connection in socket connection group).
 *  \param[in]   RemoteAddrPtr       Pointer to remote address.
 *                                   [Points to one of the following structs depending on configured IP address version
 *                                   of parameter SoConId:
 *                                     - SoAd_SockAddrInetType for IPv4
 *                                     - SoAd_SockAddrInet6Type for IPv6]
 *  \param[out]  AssignedSoConIdPtr  Pointer to assigned socket connection identifier.
 *  \return      E_OK                Request was accepted.
 *  \return      E_NOT_OK            Request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection identifiers, FALSE for same socket connection identifier.
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetUniqueRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_VAR) AssignedSoConIdPtr);

/**********************************************************************************************************************
 *  SoAd_ReleaseRemoteAddr()
 *********************************************************************************************************************/
/*! \brief       Releases the remote address (IP address and port) of the specified socket connection by setting it
 *               back to the configured remote address setting.
 *  \details     Releases the remote address of a socket connection if it is not locked due to pending transmissions/
 *               receptions or an established TCP connection. In case of a locked connection the request is stored to
 *               process it later in main function context. Due to a high load of transmissions and reception of
 *               messages it may happen that a release of the remote address is not possible.
 *  \param[in]   SoConId  Socket connection identifier.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection identifiers, FALSE for same socket connection identifier.
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_ReleaseRemoteAddr(
  SoAd_SoConIdType SoConId);

/**********************************************************************************************************************
 *  SoAd_ForceReleaseRemoteAddr()
 *********************************************************************************************************************/
/*! \brief       Forces the release of the remote address (IP address and port) of the specified socket connection by
 *               setting it back to the configured remote address setting.
 *  \details     Releases the remote address of a socket connection if it is not locked due to pending transmissions/
 *               receptions. If the connection is locked due to an open TCP connection, the remote address is released
 *               by forcing the socket connection to close. Additionally, new transmissions/receptions are rejected.
 *  \param[in]   SoConId  Socket connection identifier.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection identifiers, FALSE for same socket connection identifier.
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_ForceReleaseRemoteAddr(
  SoAd_SoConIdType SoConId);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* !defined(SOAD_REMOTEADDR_H) */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_RemoteAddr.h
 *********************************************************************************************************************/
