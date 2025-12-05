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
/*!        \file  SoAd_Cbk.h
 *        \brief  Socket Adaptor callback header file
 *
 *      \details  Vector static callback header file for AUTOSAR Socket Adaptor module. This header file contains
 *                the callback declarations of the module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/


#if !defined(SOAD_CBK_H)
# define SOAD_CBK_H


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "SoAd_Types.h"
# include "SoAd_LocalAddr_Cbk.h"
# include "SoAd_TxSocketManager_Cbk.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define SOAD_START_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  SoAd_RxIndication()
 *********************************************************************************************************************/
/*! \brief        Receives data from sockets.
 *  \details      -
 *  \param[in]    SocketId        Socket identifier.
 *  \param[in]    RemoteAddrPtr   Pointer to remote address.
 *                                [Points to one of the following structs depending on value of struct element domain:
 *                                  - SoAd_SockAddrInetType for IPv4
 *                                  - SoAd_SockAddrInet6Type for IPv6]
 *  \param[in]    BufPtr          Pointer to buffer of received data.
 *  \param[in]    Length          Length of received data.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket identifier, FALSE for same socket identifier.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_RxIndication(
  SoAd_SocketIdType SocketId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) BufPtr,
  uint16 Length);

/**********************************************************************************************************************
 *  SoAd_TcpAccepted()
 *********************************************************************************************************************/
/*! \brief        Accepts TCP connections on a listen socket.
 *  \details      -
 *  \param[in]    SocketId            Listen socket identifier.
 *  \param[in]    SocketIdConnected   Connected socket identifier.
 *  \param[in]    RemoteAddrPtr       Pointer to remote addres.
 *                                    [Points to one of the following structs depending on value of struct element
 *                                    domain:
 *                                      - SoAd_SockAddrInetType for IPv4
 *                                      - SoAd_SockAddrInet6Type for IPv6]
 *  \return       E_OK                Connection was accepted.
 *  \return       E_NOT_OK            Connection was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket identifier, FALSE for same socket identifier.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TcpAccepted(
  SoAd_SocketIdType SocketId,
  SoAd_SocketIdType SocketIdConnected,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr);

/**********************************************************************************************************************
 *  SoAd_TcpConnected()
 *********************************************************************************************************************/
/*! \brief        Handles TCP connections which have been initiated locally and are now successfully connected.
 *  \details      -
 *  \param[in]    SocketId    Socket identifier.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket identifier, FALSE for same socket identifier.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TcpConnected(
  SoAd_SocketIdType SocketId);

/**********************************************************************************************************************
 *  SoAd_TcpIpEvent()
 *********************************************************************************************************************/
/*! \brief        Handles events on sockets.
 *  \details      -
 *  \param[in]    SocketId    Socket identifier.
 *  \param[in]    Event       Event type. [SOAD_TCP_RESET, SOAD_TCP_CLOSED, SOAD_TCP_FIN_RECEIVED, SOAD_UDP_CLOSED,
 *                                        SOAD_TLS_HANDSHAKE_SUCCEEDED, SOAD_TLS_EVENT_CLOSENOTIFY_RECEIVED]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different socket identifier, FALSE for same socket identifier.
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TcpIpEvent(
  SoAd_SocketIdType SocketId,
  SoAd_EventType Event);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* !defined(SOAD_CBK_H) */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Cbk.h
 *********************************************************************************************************************/
