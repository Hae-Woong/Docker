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
/*!        \file  SoAd_RemoteAddr.c
 *        \brief  Socket Adaptor source file
 *
 *      \details  Vector static code implementation for AUTOSAR Socket Adaptor sub-component RemoteAddr.
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

#define SOAD_REMOTE_ADDR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "SoAd_RemoteAddr_Int.h"
#include "SoAd_LocalAddr_Int.h"
#include "SoAd_TxSocketManager_Int.h"
#include "SoAd_SoCon.h"
#include "SoAd_State_Int.h"
#include "SoAd_Util.h"
#include "SoAd_Rx.h"
#include "SoAd_Tx.h"
#include "SoAd_EventQueue.h"
#include "SoAd_BestMatch_Int.h"
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
 *  SoAd_RemoteAddr_GetRcvRemAddr()
 *********************************************************************************************************************/
/*! \brief       Get the received remote address of the socket connection in case it is set.
 *  \details     -
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon(), SoAd_IsRemAddrRcvUsedOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out]  IpAddrPtr     Pointer to received remote address.
 *                             [Points to one of the following structs depending on configured IP address version of
 *                             parameter SoConIdx:
 *                               - SoAd_SockAddrInetType for IPv4
 *                               - SoAd_SockAddrInet6Type for IPv6]
 *  \return      E_OK          Remote address is set and copied successfully.
 *  \return      E_NOT_OK      Remote address is not set and is not copied.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_RemoteAddr_GetRcvRemAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_SetExternally()
 *********************************************************************************************************************/
/*! \brief       Checks and sets the remote address for SoAd_SetRemoteAddr() API.
 *  \details     -
 *  \param[in]   SoConIdx       Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx   Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]   RemoteAddrPtr  Pointer to remote address.
 *                              [Points to one of the following structs depending on configured IP address version of
 *                              parameter SoConIdx:
 *                                - SoAd_SockAddrInetType for IPv4
 *                                - SoAd_SockAddrInet6Type for IPv6]
 *  \return      E_OK           Remote address was successfully set.
 *  \return      E_NOT_OK       Remote address was not set.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_RemoteAddr_SetExternally(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_CheckForWildcards
 *********************************************************************************************************************/
/*! \brief       Checks socket address for containing wildcards.
 *  \details     -
 *  \param[in]   AddrCheck    Pointer to socket address to be checked.
 *                            [Points to one of the following structs depending on the domain stored in the pointer
 *                            value:
 *                              - SoAd_SockAddrInetType for IPv4
 *                              - SoAd_SockAddrInet6Type for IPv6]
 *  \param[out]  IpAddrIsAny  Pointer to indicate if IP address is any (wildcard).
 *  \param[out]  PortIsAny    Pointer to indicate if port is any (wildcard).
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_RemoteAddr_CheckForWildcards(
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) AddrCheck,
  P2VAR(boolean, AUTOMATIC, SOAD_APPL_VAR) IpAddrIsAny,
  P2VAR(boolean, AUTOMATIC, SOAD_APPL_VAR) PortIsAny);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_IsLocked()
 *********************************************************************************************************************/
/*! \brief       Checks socket connection state to indicate if remote address is allowed to be set.
 *  \details     Checks the state of TCP socket connection and the socket connection transmission and reception states.
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return      TRUE          State on socket connection does not allow to set remote address.
 *  \return      FALSE         State on socket connection allows to set remote address.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \note        This function must be called in a critical section.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(boolean, SOAD_CODE) SoAd_RemoteAddr_IsLocked(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_Overwrite()
 *********************************************************************************************************************/
/*! \brief       Overwrites current remote address and state with new one.
 *  \details     -
 *  \param[in]   SoConIdx         Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx     Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]   RemoteAddrPtr    Pointer to remote address.
 *                                [Points to one of the following structs depending on configured IP address version
 *                                of parameter SoConIdx:
 *                                  - SoAd_SockAddrInetType for IPv4
 *                                  - SoAd_SockAddrInet6Type for IPv6]
 *  \param[in]   RemAddrStatePtr  Pointer to remote address state.
 *                                [range: SOAD_SOCON_IP_SET_PORT_SET
 *                                        SOAD_SOCON_IP_SET_PORT_ANY
 *                                        SOAD_SOCON_IP_SET_PORT_NOT
 *                                        SOAD_SOCON_IP_ANY_PORT_SET
 *                                        SOAD_SOCON_IP_ANY_PORT_ANY
 *                                        SOAD_SOCON_IP_ANY_PORT_NOT
 *                                        SOAD_SOCON_IP_NOT_PORT_SET
 *                                        SOAD_SOCON_IP_NOT_PORT_ANY
 *                                        SOAD_SOCON_IP_NOT_PORT_NOT]
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \note        This function must be called in a critical section.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RemoteAddr_Overwrite(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  SoAd_RemAddrStateType RemAddrState);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_SetUnique()
 *********************************************************************************************************************/
/*! \brief       Sets the remote address of a suitable socket connection in a socket connection group.
 *  \details     -
 *  \param[in]   SoConIdx            Socket connection index.
 *                                   [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx        Partition index.
 *                                   [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]   RemoteAddrPtr       Pointer to remote address.
 *                                   [Points to one of the following structs depending on configured IP address
 *                                   version of parameter SoConIdx:
 *                                     - SoAd_SockAddrInetType for IPv4
 *                                     - SoAd_SockAddrInet6Type for IPv6]
 *  \param[out]  AssignedSoConIdPtr  Pointer to assigned socket connection identifier.
 *  \return      E_OK                Request was accepted.
 *  \return      E_NOT_OK            Request was not accepted.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_RemoteAddr_SetUnique(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_VAR) AssignedSoConIdPtr);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_CheckRelease()
 *********************************************************************************************************************/
/*! \brief       Checks if a release of the remote address is valid as the currently set remote address is not equal
 *               to the configured remote address of a socket connection and sets the release state.
 *  \details     -
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]   ForceRelease  Flag to force release of remote address.
 *  \return      E_OK          Release of remote address is valid.
 *  \return      E_NOT_OK      Release of remote address is not valid.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_RemoteAddr_CheckRelease(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean ForceRelease);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_GetConfigured()
 *********************************************************************************************************************/
/*! \brief       Gets the configured remote address dependent on the domain type.
 *  \details     -
 *  \param[in]   SoConIdx         Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx     Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out]  RemAddrPtr       Pointer to configured remote address.
 *  \param[out]  RemAddrStatePtr  Pointer to configured remote address state.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_RemoteAddr_GetConfigured(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SockAddrInetXType, AUTOMATIC, SOAD_APPL_VAR) RemAddrPtr,
  P2VAR(SoAd_RemAddrStateType, AUTOMATIC, SOAD_APPL_VAR) RemAddrStatePtr);

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_RemoteAddr_ForceRelease()
 *********************************************************************************************************************/
/*! \brief       Tries to force release the remote address (IP address and port) of the specified socket connection by
 *               setting it back to the configured remote address setting.
 *  \details     -
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \config      SOAD_TCP
 *  \note        This function must be called in a critical section.
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RemoteAddr_ForceRelease(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_Reset()
 *********************************************************************************************************************/
/*! \brief       Resets remote address on a socket connection.
 *  \details     -
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \note        This function must be called in a critical section (except SoAd_RemoteAddr_Init()).
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RemoteAddr_Reset(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_ResetRcvRemAddr()
 *********************************************************************************************************************/
/*! \brief       Resets received remote address on a socket connection.
 *  \details     -
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \note        This function must be called in a critical section (except SoAd_RemoteAddr_Init()).
 *********************************************************************************************************************/
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RemoteAddr_ResetRcvRemAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_GetRcvRemAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_RemoteAddr_GetRcvRemAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const SoAd_SizeOfRemAddrRcvType rcvRemAddrIdx = SoAd_GetRemAddrRcvIdxOfSoCon(SoConIdx, PartitionIdx);
  Std_ReturnType                  retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that a reception interrupts and address becomes inconsistent. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Check if address is set and has no wildcards (i.e. received something from a remote entity). */
  /* PRQA S 310 1 */ /* MD_SoAd_PointerCastOnStruct */
  if ( SoAd_RemoteAddr_DeriveState(SOAD_A_P2CONST(SoAd_SockAddrType)SoAd_GetAddrRemAddrRcv(                            /* SBSW_SOAD_CONST_POINTER_FORWARD */
        rcvRemAddrIdx, PartitionIdx)) == SOAD_SOCON_IP_SET_PORT_SET )
  {
    /* #30 Return received remote address. */
    /* PRQA S 310 1 */ /* MD_SoAd_PointerCastOnStruct */
    SoAd_Util_CopySockAddr(IpAddrPtr, SOAD_A_P2CONST(SoAd_SockAddrType)SoAd_GetAddrRemAddrRcv(rcvRemAddrIdx,           /* SBSW_SOAD_POINTER_AND_CONST_POINTER_FORWARD */
        PartitionIdx));

    retVal = E_OK;
  }

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_RemoteAddr_GetRcvRemAddr() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_SetExternally()
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
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_RemoteAddr_SetExternally(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Compare remote address to corresponding local IP addresses to prevent setting local IP address as remote. */
  /* PRQA S 310, 3305 2 */ /* MD_SoAd_PointerCastOnStruct */
  if ( SoAd_LocalAddr_IsAddrActiveOnCtrl(SoConIdx, PartitionIdx,
      (SOAD_A_P2CONST(SoAd_SockAddrInetXType)RemoteAddrPtr)->addr) != E_OK )                                           /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_STRUCT_ELEMENT_FORWARD */
  {
    /* #20 Enter critical section to prevent that remote address is set in other context. */
    SOAD_BEGIN_CRITICAL_SECTION();

    /* #30 Check if socket connection is not locked and if no release is pending. */
    if ( (SoAd_RemoteAddr_IsLocked(SoConIdx, PartitionIdx) == FALSE) &&
      (SoAd_GetRemAddrReleaseOfSoConDyn(SoConIdx, PartitionIdx) == SOAD_RELEASE_NONE) )
    {
      boolean ipHasWildcard; /* Not initialized: Variable is initialized by another function. */
      boolean portHasWildcard; /* Not initialized: Variable is initialized by another function. */
      boolean remAddrHasWildcard = FALSE;

      /* #40 Try to set remote address. */
      (void)SoAd_RemoteAddr_Set(SoConIdx, PartitionIdx, RemoteAddrPtr, FALSE, TRUE);                                   /* SBSW_SOAD_POINTER_FORWARD */

      /* #50 Check if remote address contains wildcards. */
      SoAd_RemoteAddr_CheckForWildcards(RemoteAddrPtr, &ipHasWildcard, &portHasWildcard);                              /* SBSW_SOAD_POINTER_AND_VARIABLE_POINTER_FORWARD */

      if ( (ipHasWildcard == TRUE) || (portHasWildcard == TRUE) )
      {
        remAddrHasWildcard = TRUE;
      }

      /* #60 Leave critical section. */
      SOAD_END_CRITICAL_SECTION();

      /* #70 Check and perform mode change of the socket connection if required. */
      SoAd_SoCon_NotifyRemoteAddrChg(SoConIdx, PartitionIdx, remAddrHasWildcard);

      retVal = E_OK;
    }
    /* #31 Leave critical section otherwise. */
    else
    {
      SOAD_END_CRITICAL_SECTION();
    }
  }

  return retVal;
} /* SoAd_RemoteAddr_SetExternally() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_CheckForWildcards()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_RemoteAddr_CheckForWildcards(
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) AddrCheck,
  P2VAR(boolean, AUTOMATIC, SOAD_APPL_VAR) IpAddrIsAny,
  P2VAR(boolean, AUTOMATIC, SOAD_APPL_VAR) PortIsAny)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_IPV6 == STD_ON )
  /* #10 Check if IPv6 address is wildcard (if enabled). */
  if ( AddrCheck->domain == SOAD_AF_INET6 )
  {
    /* PRQA S 310, 3305 4 */ /* MD_SoAd_PointerCastOnStruct */
    if ( (((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))AddrCheck)->addr[0u] == SOAD_IPADDR_ANY) &&
      (((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))AddrCheck)->addr[1u] == SOAD_IPADDR_ANY) &&
      (((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))AddrCheck)->addr[2u] == SOAD_IPADDR_ANY) &&
      (((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))AddrCheck)->addr[3u] == SOAD_IPADDR_ANY) )
    {
      *IpAddrIsAny = TRUE;                                                                                             /* SBSW_SOAD_POINTER_WRITE */
    }
    else
    {
      *IpAddrIsAny = FALSE;                                                                                            /* SBSW_SOAD_POINTER_WRITE */
    }
  }
  else
#endif /* SOAD_IPV6 == STD_ON */
  /* #11 Check if IPv4 address is wildcard otherwise. */
  {
    /* PRQA S 310, 3305 1 */ /* MD_SoAd_PointerCastOnStruct */
    if ( ((P2CONST(SoAd_SockAddrInetType, AUTOMATIC, AUTOMATIC))AddrCheck)->addr[0u] == SOAD_IPADDR_ANY )
    {
      *IpAddrIsAny = TRUE;                                                                                             /* SBSW_SOAD_POINTER_WRITE */
    }
    else
    {
      *IpAddrIsAny = FALSE;                                                                                            /* SBSW_SOAD_POINTER_WRITE */
    }
  }

  /* #20 Check if port is wildcard. */
  /* PRQA S 310, 3305 1 */ /* MD_SoAd_PointerCastOnStruct */
  if ( ((P2CONST(SoAd_SockAddrInetXType, AUTOMATIC, AUTOMATIC))AddrCheck)->port == SOAD_PORT_ANY )
  {
    *PortIsAny = TRUE;                                                                                                 /* SBSW_SOAD_POINTER_WRITE */
  }
  else
  {
    *PortIsAny = FALSE;                                                                                                /* SBSW_SOAD_POINTER_WRITE */
  }
} /* SoAd_RemoteAddr_CheckForWildcards() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_IsLocked()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(boolean, SOAD_CODE) SoAd_RemoteAddr_IsLocked(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_TCP == STD_ON )
  /* #10 Check if socket connection is not offline in case of TCP (if enabled). */
  if ( (SoAd_SoCon_GetMode(SoConIdx, PartitionIdx) != SOAD_SOCON_OFFLINE) &&
    SoAd_IsSocketTcpUsedOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx) )
  {
    retVal = TRUE;
  }
  else
#endif /* SOAD_TCP == STD_ON */
  /* #20 Check if any reception or transmission is active or pending otherwise. */
  {
    /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
    if ( (SoAd_Rx_IsActive(SoConIdx, PartitionIdx) == TRUE) ||
      (SoAd_TxSocketManager_IsTxActive(SoConIdx, PartitionIdx, FALSE) == TRUE) )
    {
      retVal = TRUE;
    }
  }

  return retVal;
} /* SoAd_RemoteAddr_IsLocked() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_Overwrite()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RemoteAddr_Overwrite(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  SoAd_RemAddrStateType RemAddrState)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy remote address to local struct. */
  /* PRQA S 310 1 */ /* MD_SoAd_PointerCastOnStruct */
  SoAd_Util_CopySockAddr(SOAD_A_P2VAR(SoAd_SockAddrType)SoAd_GetAddrRemAddrDynOfSoConDyn(SoConIdx, PartitionIdx),      /* SBSW_SOAD_VARIABLE_POINTER_CSL02_AND_CONST_POINTER_FORWARD */
    RemoteAddrPtr);

  /* #20 Set remote address state. */
  SoAd_SetRemAddrStateDynOfSoConDyn(SoConIdx, RemAddrState, PartitionIdx);                                             /* SBSW_SOAD_CSL02_CSL05 */
} /* SoAd_RemoteAddr_Overwrite() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_SetUnique()
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_RemoteAddr_SetUnique(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_VAR) AssignedSoConIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const SoAd_RemAddrStateType remAddrState = SoAd_RemoteAddr_DeriveState(RemoteAddrPtr);                               /* SBSW_SOAD_POINTER_FORWARD */
  Std_ReturnType              retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent that remote address is set in other context. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Validate remote address to be set. */
  if ( remAddrState == SOAD_SOCON_IP_SET_PORT_SET )
  {
    const SoAd_SizeOfSoConGrpType       soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);
    const SoAd_SizeOfInstanceType       instanceIdx = SoAd_GetInstanceIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
    SoAd_SizeOfSoConType                assignedSoConIdx; /* Not initialized: Variable is initialized by another
                                                           * function. */
    SoAd_BestMatch_SoConStartEndIdxType soConStartEndIdxStruct; /* Not initialized. Struct is initialized member
                                                                 * wise. */

    soConStartEndIdxStruct.InstanceIdx = instanceIdx;
    soConStartEndIdxStruct.PartitionIdx = PartitionIdx;
    soConStartEndIdxStruct.SoConStartIdx = SoAd_GetSoConStartIdxOfSoConGrp(soConGrpIdx, PartitionIdx);
    soConStartEndIdxStruct.SoConEndIdx = SoAd_GetSoConEndIdxOfSoConGrp(soConGrpIdx, PartitionIdx);

    /* #30 Derive socket connection list using best match algorithm. */
    SoAd_BestMatch_DeriveList(soConStartEndIdxStruct, FALSE, RemoteAddrPtr);                                           /* SBSW_SOAD_CONST_POINTER_FORWARD */

    /* #40 Get first socket connection from best match algorithm. */
    if ( SoAd_BestMatch_GetNextSoCon(instanceIdx, PartitionIdx, 0u, &assignedSoConIdx) == E_OK )                       /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
    {
      /* #50 Check if requested remote address is already set on the socket connection. */
      if ( SoAd_GetRemAddrStateDynOfSoConDyn(assignedSoConIdx, PartitionIdx) == SOAD_SOCON_IP_SET_PORT_SET )
      {
        /* #500 Remove alive supervision timeout from timeout list if configured and running. */
#if ( SOAD_TCP == STD_ON )
        if ( SoAd_IsSocketUdpUsedOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(assignedSoConIdx, PartitionIdx), PartitionIdx) )
#endif /* SOAD_TCP == STD_ON */
        {
          SoAd_SoCon_ResetUdpAliveTimeout(assignedSoConIdx, PartitionIdx);
        }

        retVal = E_OK;
      }
      else
      {
        /* #51 Compare remote address to corresponding local IP addresses to prevent setting local IP address as
         *     remote otherwise. */
        /* PRQA S 310, 3305 2 */ /* MD_SoAd_PointerCastOnStruct */
        if ( SoAd_LocalAddr_IsAddrActiveOnCtrl(assignedSoConIdx, PartitionIdx,
            (SOAD_A_P2CONST(SoAd_SockAddrInetXType)RemoteAddrPtr)->addr) != E_OK )                                     /* SBSW_SOAD_SOCK_ADDR_CAST_POINTER_STRUCT_ELEMENT_FORWARD */
        {
          /* #510 Set remote address and state. */
          SoAd_RemoteAddr_Overwrite(assignedSoConIdx, PartitionIdx, RemoteAddrPtr, remAddrState);                      /* SBSW_SOAD_CONST_POINTER_FORWARD */

          /* #511 Notify socket connection unit to handle socket connection state change in next main function. */
          SoAd_SoCon_NotifyRemoteAddrChg(assignedSoConIdx, PartitionIdx, FALSE);

          retVal = E_OK;
        }
      }

      /* #60 Return socket connection if found by best match algorithm and it is valid to set. */
      if ( retVal == E_OK )
      {
        *AssignedSoConIdPtr = SoAd_GetSoConIdOfSoCon(assignedSoConIdx, PartitionIdx);                                  /* SBSW_SOAD_POINTER_WRITE */
      }
    }
  }

  /* #70 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_RemoteAddr_SetUnique() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_CheckRelease()
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
SOAD_LOCAL FUNC(Std_ReturnType, SOAD_CODE) SoAd_RemoteAddr_CheckRelease(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean ForceRelease)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockAddrInetXType               cfgRemAddr; /* Not initialized: Struct is initialized by another function. */
  SoAd_RemAddrStateType                cfgRemAddrState; /* Not initialized: Variable is initialized by another
                                                         * function. */
  /* PRQA S 310, 3305 1 */ /* MD_SoAd_PointerCastOnStruct */
  SOAD_P2CONST(SoAd_SockAddrInetXType) currentRemAddrPtr = SoAd_GetAddrRemAddrDynOfSoConDyn(SoConIdx, PartitionIdx);
  Std_ReturnType                       retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the configured remote address. */
  SoAd_RemoteAddr_GetConfigured(SoConIdx, PartitionIdx, &cfgRemAddr, &cfgRemAddrState);                                /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

  /* #20 Enter critical section to prevent that remote address release state is set in other context. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #30 Check if the currently set remote address (IP address and port) is equal to the configured one. */
  if ( SoAd_Util_CompareIpAddr(cfgRemAddr.domain, cfgRemAddr.addr, currentRemAddrPtr->addr) == E_OK )                  /* SBSW_SOAD_CONST_POINTER_FORWARD */
  {
    if ( cfgRemAddr.port == currentRemAddrPtr->port )
    {
      retVal = E_NOT_OK;
    }
  }

  /* #40 Check if release is necessary. */
  if ( retVal == E_OK )
  {
    /* #50 Set force remote address release state in case the release is forced or was forced already. */
    if ( (ForceRelease == TRUE) || (SoAd_GetRemAddrReleaseOfSoConDyn(SoConIdx, PartitionIdx) ==
        SOAD_RELEASE_REM_ADDR_FORCE) )
    {
      SoAd_SetRemAddrReleaseOfSoConDyn(SoConIdx, SOAD_RELEASE_REM_ADDR_FORCE, PartitionIdx);                           /* SBSW_SOAD_PARAMETER_IDX */
    }
    /* #60 Set remote address release state otherwise. */
    else
    {
      SoAd_SetRemAddrReleaseOfSoConDyn(SoConIdx, SOAD_RELEASE_REM_ADDR, PartitionIdx);                                 /* SBSW_SOAD_PARAMETER_IDX */
    }
  }

  /* #70 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();

  return retVal;
} /* SoAd_RemoteAddr_CheckRelease() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_GetConfigured()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL FUNC(void, SOAD_CODE) SoAd_RemoteAddr_GetConfigured(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SockAddrInetXType, AUTOMATIC, SOAD_APPL_VAR) RemAddrPtr,
  P2VAR(SoAd_RemAddrStateType, AUTOMATIC, SOAD_APPL_VAR) RemAddrStatePtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store the configured remote address to the pointer value. */
#if ( SOAD_IPV6 == STD_ON )
  if ( SoAd_IsRemAddrIpV6UsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    const SoAd_RemAddrIpV6IdxOfSoConType ipV6AddrIdx = SoAd_GetRemAddrIpV6IdxOfSoCon(SoConIdx, PartitionIdx);
    /* PRQA S 759 1 */ /* MD_MSR_Union */
    SoAd_IpAddrInet6Type                 remAddrIpV6; /* Not initialized: Avoid QAC warning 1031. */

    remAddrIpV6 = SoAd_GetAddrOfRemAddrIpV6(ipV6AddrIdx, PartitionIdx);

    /* PRQA S 2983 2 */ /* MD_SoAd_UsageNotDetected */
    RemAddrPtr->domain   = SOAD_AF_INET6;                                                                              /* SBSW_SOAD_POINTER_WRITE */
    RemAddrPtr->port     = SoAd_GetPortOfRemAddrIpV6(ipV6AddrIdx, PartitionIdx);                                       /* SBSW_SOAD_POINTER_WRITE */
    RemAddrPtr->addr[0u] = remAddrIpV6.addr32[0u];                                                                     /* SBSW_SOAD_POINTER_WRITE */
    RemAddrPtr->addr[1u] = remAddrIpV6.addr32[1u];                                                                     /* SBSW_SOAD_POINTER_WRITE */
    RemAddrPtr->addr[2u] = remAddrIpV6.addr32[2u];                                                                     /* SBSW_SOAD_POINTER_WRITE */
    RemAddrPtr->addr[3u] = remAddrIpV6.addr32[3u];                                                                     /* SBSW_SOAD_POINTER_WRITE */
  }
  else
#endif /* SOAD_IPV6 == STD_ON */
  {
    const SoAd_RemAddrIpV4IdxOfSoConType ipV4AddrIdx = SoAd_GetRemAddrIpV4IdxOfSoCon(SoConIdx, PartitionIdx);
 
    /* PRQA S 2983 2 */ /* MD_SoAd_UsageNotDetected */
    RemAddrPtr->domain   = SOAD_AF_INET;                                                                               /* SBSW_SOAD_POINTER_WRITE */
    RemAddrPtr->port     = SoAd_GetPortOfRemAddrIpV4(ipV4AddrIdx, PartitionIdx);                                       /* SBSW_SOAD_POINTER_WRITE */
    RemAddrPtr->addr[0u] = SoAd_GetAddrOfRemAddrIpV4(ipV4AddrIdx, PartitionIdx);                                       /* SBSW_SOAD_POINTER_WRITE */
  }
  /* #20 Return configured remote address state. */
  *RemAddrStatePtr = SoAd_GetRemAddrCfgStateOfSoCon(SoConIdx, PartitionIdx);                                           /* SBSW_SOAD_POINTER_WRITE */
} /* SoAd_RemoteAddr_GetConfigured() */

#if ( SOAD_TCP == STD_ON )
/**********************************************************************************************************************
 *  SoAd_RemoteAddr_ForceRelease()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RemoteAddr_ForceRelease(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset the remote address if the connection is not online and the socket is not yet connecting. */
  /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
  if ( (SoAd_SoCon_GetMode(SoConIdx, PartitionIdx) != SOAD_SOCON_ONLINE) &&
    (SoAd_SoCon_IsTcpSocketConnecting(SoConIdx, PartitionIdx) == FALSE) )
  {
    SoAd_RemoteAddr_Reset(SoConIdx, PartitionIdx);

    /* #100 Notify socket connection unit to handle the state change (and release) in the next main function. */
    SoAd_SoCon_NotifyRemoteAddrChg(SoConIdx, PartitionIdx, FALSE);
  }
  /* #20 Handle the release of the remote address in the next main funtion cycle otherwise. */
  else
  {
    /* #200 Set close mode reconnect. */
# if ( SOAD_RST_FOR_FORCE_RELEASE_ENABLED == STD_ON )
    SoAd_SoCon_RequestClose(SoConIdx, SoAd_GetInstanceIdxOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx),
        PartitionIdx), PartitionIdx, SOAD_CLOSE_RECONNECT, TRUE);
# else
    SoAd_SoCon_RequestClose(SoConIdx, SoAd_GetInstanceIdxOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx),
        PartitionIdx), PartitionIdx, SOAD_CLOSE_RECONNECT, FALSE);
# endif /* SOAD_RST_FOR_FORCE_RELEASE_ENABLED == STD_ON */
  }
} /* SoAd_RemoteAddr_ForceRelease() */
#endif /* SOAD_TCP == STD_ON */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_Reset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RemoteAddr_Reset(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SockAddrInetXType cfgRemAddr; /* Not initialized: Struct is initialized by another function. */
  SoAd_RemAddrStateType  cfgRemAddrState; /* Not initialized: Struct is initialized by another function. */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the configured remote address and state. */
  SoAd_RemoteAddr_GetConfigured(SoConIdx, PartitionIdx, &cfgRemAddr, &cfgRemAddrState);                                /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */

  /* #20 Reset remote address and state to configured one. */
  /* PRQA S 310 1 */ /* MD_SoAd_PointerCastOnStruct */
  SoAd_RemoteAddr_Overwrite(SoConIdx, PartitionIdx, SOAD_A_P2CONST(SoAd_SockAddrType)&cfgRemAddr, cfgRemAddrState);    /* SBSW_SOAD_CONST_POINTER_FORWARD */

  /* #30 Reset remote address release flag. */
  SoAd_SetRemAddrReleaseOfSoConDyn(SoConIdx, SOAD_RELEASE_NONE, PartitionIdx);                                         /* SBSW_SOAD_PARAMETER_IDX */
} /* SoAd_RemoteAddr_Reset() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_ResetRcvRemAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOAD_LOCAL_INLINE FUNC(void, SOAD_CODE) SoAd_RemoteAddr_ResetRcvRemAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if received remote address is configured. */
  if ( SoAd_IsRemAddrRcvUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    SoAd_SockAddrInetXType rcvRemAddr; /* Not initialized: Non-compile-time-const expression. */

    /* #20 Reset received remote address. */
#if ( SOAD_IPV6 == STD_ON )
    if ( SoAd_IsRemAddrIpV6UsedOfSoCon(SoConIdx, PartitionIdx) )
    {
      rcvRemAddr.domain = SOAD_AF_INET6;
    }
    else
#endif /* SOAD_IPV6 == STD_ON */
    {
      rcvRemAddr.domain = SOAD_AF_INET;
    }

    rcvRemAddr.port = SOAD_PORT_ANY;

    rcvRemAddr.addr[0u] = SOAD_IPADDR_ANY;                                                                             /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
#if ( SOAD_IPV6 == STD_ON )
    rcvRemAddr.addr[1u] = SOAD_IPADDR_ANY;                                                                             /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    rcvRemAddr.addr[2u] = SOAD_IPADDR_ANY;                                                                             /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
    rcvRemAddr.addr[3u] = SOAD_IPADDR_ANY;                                                                             /* SBSW_SOAD_EXPECTED_SIZE_WRITE */
#endif /* SOAD_IPV6 == STD_ON */

    SoAd_SetRemAddrRcv(SoAd_GetRemAddrRcvIdxOfSoCon(SoConIdx, PartitionIdx), rcvRemAddr, PartitionIdx);                /* SBSW_SOAD_OPTIONAL_CHECKED_CSL03_CSL05 */
  }
} /* SoAd_RemoteAddr_ResetRcvRemAddr() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/* ----- Service functions --------------------------------------------- */

/**********************************************************************************************************************
 *  SoAd_GetRemoteAddr()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  Std_ReturnType              retVal = E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( IpAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
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
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
      /* #40 Check if remote address has no "not set" values. */
      if ( (SoAd_GetRemAddrStateDynOfSoConDyn(soConIdx, partitionIdx) & SOAD_SOCON_MASK_NOT) == 0u )
      {
        /* #50 Return remote address. */
        SoAd_RemoteAddr_Get(soConIdx, partitionIdx, IpAddrPtr);                                                        /* SBSW_SOAD_POINTER_FORWARD */
        retVal = E_OK;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_GET_REMOTE_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_GetRemoteAddr() */

/**********************************************************************************************************************
 *  SoAd_GetRemoteAddrState()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRemoteAddrState(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr,
  P2VAR(SoAd_RemAddrStateType, AUTOMATIC, SOAD_APPL_VAR) RemAddrState)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  Std_ReturnType              retVal = E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( IpAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( RemAddrState == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
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
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
      /* #40 Return remote address. */
      SoAd_RemoteAddr_Get(soConIdx, partitionIdx, IpAddrPtr);                                                          /* SBSW_SOAD_POINTER_FORWARD */

      /* #50 Return remote address state. */
      *RemAddrState = SoAd_GetRemAddrStateDynOfSoConDyn(soConIdx, partitionIdx);                                       /* SBSW_SOAD_POINTER_WRITE */
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_GET_REMOTE_ADDR_STATE, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_GetRemoteAddrState() */

/**********************************************************************************************************************
 *  SoAd_GetRcvRemoteAddr()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRcvRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  Std_ReturnType              retVal = E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( !SoAd_IsRemAddrRcvUsedOfSoCon(soConIdx, partitionIdx) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( IpAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
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
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
      /* #40 Try to return received remote address. */
      retVal = SoAd_RemoteAddr_GetRcvRemAddr(soConIdx, partitionIdx, IpAddrPtr);                                       /* SBSW_SOAD_POINTER_FORWARD */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_GET_RCV_REMOTE_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_GetRcvRemoteAddr() */

/**********************************************************************************************************************
 *  SoAd_SetRemoteAddr()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  uint8                       errorId = SOAD_E_NO_ERROR;
  Std_ReturnType              retVal = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  /* SPEC-2743546 */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( RemoteAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
  /* SPEC-2743547 */
  else if ( SoAd_IsSockAutoSoConSetupOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(soConIdx, partitionIdx), partitionIdx) )
  {
    errorId = SOAD_E_INV_ARG;
  }
  /* API must only be called when domain of local address and remote address match. */
  /* SPEC-2743551 */
  else if ( SoAd_LocalAddr_GetDomain(SoAd_SoCon_GetLocalAddrIdx(soConIdx, partitionIdx), partitionIdx) !=
    RemoteAddrPtr->domain )
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
  {
#else
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* API must only be called when domain of local address and remote address match. */
    /* SPEC-2743551 */
    if ( SoAd_LocalAddr_GetDomain(SoAd_SoCon_GetLocalAddrIdx(soConIdx, partitionIdx), partitionIdx) ==
      RemoteAddrPtr->domain )
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #30 Check module state. */
      if ( SoAd_State_GetState() == SOAD_STATE_INIT )
      {
        /* #40 Try to set remote address. */
        retVal = SoAd_RemoteAddr_SetExternally(soConIdx, partitionIdx, RemoteAddrPtr);                                 /* SBSW_SOAD_CONST_POINTER_FORWARD */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_SET_REMOTE_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_SetRemoteAddr() */

/**********************************************************************************************************************
 *  SoAd_SetUniqueRemoteAddr()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetUniqueRemoteAddr(
  SoAd_SoConIdType SoConId,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_VAR) AssignedSoConIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
  SoAd_PartitionConfigIdxType partitionIdx; /* Not initialized: Variable is initialized by another function. */
  Std_ReturnType              retVal = E_NOT_OK;
  uint8                       errorId = SOAD_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ( SOAD_DEV_ERROR_DETECT == STD_ON )
  /* #10 Check initialization state. */
  /* SPEC-2743553 */
  if ( SoAd_State_GetState() < SOAD_STATE_INIT )
  {
    errorId = SOAD_E_NOTINIT;
  }
  /* #20 Check plausibility of all input parameters. */
  else if ( RemoteAddrPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
  /* SPEC-2743554 */
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
  /* API must only be called when domain of local address and remote address match. */
  /* SPEC-2743558 */
  else if ( SoAd_LocalAddr_GetDomain(SoAd_SoCon_GetLocalAddrIdx(soConIdx, partitionIdx), partitionIdx) !=
    RemoteAddrPtr->domain )
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( AssignedSoConIdPtr == NULL_PTR )
  {
    errorId = SOAD_E_PARAM_POINTER;
  }
# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
  /* Check validity of application identifier and the mapped instance index. */
  else if ( SoAd_Util_CheckApplicationIdValidity(partitionIdx) != E_OK )
  {
    errorId = SOAD_E_INV_APPLICATION_ID;
  }
# endif /* SOAD_MULTI_PARTITION == STD_ON */
  else
  {
#else
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    /* API must only be called when domain of local address and remote address match. */
    /* SPEC-2743558 */
    if ( SoAd_LocalAddr_GetDomain(SoAd_SoCon_GetLocalAddrIdx(soConIdx, partitionIdx), partitionIdx) ==
      RemoteAddrPtr->domain )
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #30 Check module state. */
      if ( SoAd_State_GetState() == SOAD_STATE_INIT )
      {
        /* #40 Try to set remote address and return the assigned socket connection. */
        retVal = SoAd_RemoteAddr_SetUnique(soConIdx, partitionIdx, RemoteAddrPtr, AssignedSoConIdPtr);                 /* SBSW_SOAD_POINTER_AND_CONST_POINTER_FORWARD */
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #50 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_SET_UNI_REMOTE_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */

  return retVal;
} /* SoAd_SetUniqueRemoteAddr() */

/**********************************************************************************************************************
 *  SoAd_ReleaseRemoteAddr()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, SOAD_CODE) SoAd_ReleaseRemoteAddr(
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
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
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( SoAd_IsSockAutoSoConSetupOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(soConIdx, partitionIdx), partitionIdx) )
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
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
      /* #40 Check if the currently set remote address is unequal to the configured one. */
      if ( SoAd_RemoteAddr_CheckRelease(soConIdx, partitionIdx, FALSE) == E_OK )
      {
        /* #50 Try to release the remote address of the socket connection. */
        SoAd_RemoteAddr_HandleRelease(soConIdx, partitionIdx);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_RELEASE_REMOTE_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
} /* SoAd_ReleaseRemoteAddr() */

/**********************************************************************************************************************
 *  SoAd_ForceReleaseRemoteAddr()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, SOAD_CODE) SoAd_ForceReleaseRemoteAddr(
  SoAd_SoConIdType SoConId)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_SizeOfSoConType        soConIdx; /* Not initialized: Variable is initialized by another function. */
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
  else if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) != E_OK )                               /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
  {
    errorId = SOAD_E_INV_ARG;
  }
  else if ( SoAd_IsSockAutoSoConSetupOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(soConIdx, partitionIdx), partitionIdx) )
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
  if ( SoAd_SoCon_GetSoConIdxBySoConId(SoConId, &soConIdx, &partitionIdx) == E_OK )                                    /* SBSW_SOAD_VARIABLE_POINTER_FORWARD */
#endif /* SOAD_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Check module state. */
    if ( SoAd_State_GetState() == SOAD_STATE_INIT )
    {
      /* #40 Check if the currently set remote address is unequal to the configured one. */
      if ( SoAd_RemoteAddr_CheckRelease(soConIdx, partitionIdx, TRUE) == E_OK )
      {
        /* #50 Try to release the remote address of the socket connection. */
        SoAd_RemoteAddr_HandleRelease(soConIdx, partitionIdx);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if ( SOAD_DEV_ERROR_REPORT == STD_ON )
  /* #60 Report default error if any occurred. */
  if ( errorId != SOAD_E_NO_ERROR )
  {
    (void)Det_ReportError(SOAD_MODULE_ID, SOAD_INSTANCE_ID_DET, SOAD_SID_FORCE_RELEASE_REMOTE_ADDR, errorId);
  }
#else
  SOAD_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* SOAD_DEV_ERROR_REPORT == STD_ON */
} /* SoAd_ForceReleaseRemoteAddr() */

/* ----- Functions ----------------------------------------------------- */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_RemoteAddr_Init(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset remote address. */
  SoAd_RemoteAddr_Reset(SoConIdx, PartitionIdx);

  /* #20 Reset received remote address (if enabled). */
  SoAd_RemoteAddr_ResetRcvRemAddr(SoConIdx, PartitionIdx);
} /* SoAd_RemoteAddr_Init() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_ResetRemAndRcvRemAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_RemoteAddr_ResetRemAndRcvRemAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Reset remote address. */
  SoAd_RemoteAddr_Reset(SoConIdx, PartitionIdx);

  /* #30 Reset received remote address (if enabled). */
  SoAd_RemoteAddr_ResetRcvRemAddr(SoConIdx, PartitionIdx);

  /* #40 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();
} /* SoAd_RemoteAddr_ResetRemAndRcvRemAddr() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_HandleRelease()
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(void, SOAD_CODE) SoAd_RemoteAddr_HandleRelease(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RemAddrReleaseStateType remAddrReleaseState; /* Not initialized: Is initialized within critical section. */
  boolean                      releaseTriggered = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section. */
  SOAD_BEGIN_CRITICAL_SECTION();

  remAddrReleaseState = SoAd_GetRemAddrReleaseOfSoConDyn(SoConIdx, PartitionIdx);

  /* #20 Check if release is requested for the current socket connection. */
  if ( remAddrReleaseState != SOAD_RELEASE_NONE )
  {
    /* #30 Check if socket connection transmission and reception states allow to release remote address. */
    /* PRQA S 3415 2 */ /* MD_SoAd_RightHandOperandWithoutSideEffects */
    if ( (SoAd_Rx_IsActive(SoConIdx, PartitionIdx) == FALSE) &&
      (SoAd_TxSocketManager_IsTxActive(SoConIdx, PartitionIdx, FALSE) == FALSE) )
    {
#if ( SOAD_TCP == STD_ON )
      const SoAd_SizeOfSoConGrpType soConGrpIdx = SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx);

      /* #300 Check if socket connection is not offline in case of TCP (if enabled). */
      if ( (SoAd_SoCon_GetMode(SoConIdx, PartitionIdx) != SOAD_SOCON_OFFLINE) &&
        SoAd_IsSocketTcpUsedOfSoConGrp(soConGrpIdx, PartitionIdx) )
      {
        /* #3000 Force release if requested. */
        if ( remAddrReleaseState == SOAD_RELEASE_REM_ADDR_FORCE )
        {
          SoAd_RemoteAddr_ForceRelease(SoConIdx, PartitionIdx);
          releaseTriggered = TRUE;

          /* #30000 Leave critical section. */
          SOAD_END_CRITICAL_SECTION();
        }
      }
      /* #301 Release the remote address otherwise. */
      else
#endif /* SOAD_TCP == STD_ON */
      {
        const boolean hasCfgWildcard = SoAd_RemoteAddr_HasConfigWildcard(SoConIdx, PartitionIdx, FALSE);

        /* #3010 Reset the remote address. */
        SoAd_RemoteAddr_Reset(SoConIdx, PartitionIdx);

        /* #3011 Leave critical section. */
        SOAD_END_CRITICAL_SECTION();

        /* #3012 Check and perform mode change of the socket connection if required. */
        SoAd_SoCon_NotifyRemoteAddrChg(SoConIdx, PartitionIdx, hasCfgWildcard);

        releaseTriggered = TRUE;
      }
    }

    /* #40 Check if release was not yet triggered. */
    if ( releaseTriggered == FALSE )
    {
      /* #400 Leave critical section. */
      SOAD_END_CRITICAL_SECTION();

      /* #410 Add an event to the queue to handle the release in the main function. */
      SoAd_EventQueue_AddElement(SOAD_EVENT_QUEUE_SO_CON_STATE, SoConIdx,
        SoAd_GetInstanceIdxOfSoConGrp(SoAd_GetSoConGrpIdxOfSoCon(SoConIdx, PartitionIdx), PartitionIdx), PartitionIdx);
    }
  }
  /* #21 Leave critical section otherwise. */
  else
  {
    SOAD_END_CRITICAL_SECTION();
  }
} /* SoAd_RemoteAddr_HandleRelease() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_UpdateRcvRemAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_RemoteAddr_UpdateRcvRemAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the remote address shall be stored on reception for the current socket connection. */
  if ( SoAd_IsRemAddrRcvUsedOfSoCon(SoConIdx, PartitionIdx) )
  {
    const SoAd_SizeOfRemAddrRcvType rcvRemAddrIdx = SoAd_GetRemAddrRcvIdxOfSoCon(SoConIdx, PartitionIdx);

    /* #20 Enter critical section to prevent that received remote address buffer is read while modification. */
    SOAD_BEGIN_CRITICAL_SECTION();

    /* #30 Store the received remote address. */
    /* PRQA S 310 1 */ /* MD_SoAd_PointerCastOnStruct */
    SoAd_Util_CopySockAddr(SOAD_A_P2VAR(SoAd_SockAddrType)SoAd_GetAddrRemAddrRcv(rcvRemAddrIdx, PartitionIdx),         /* SBSW_SOAD_VARIABLE_POINTER_CSL03_CSL05_AND_CONST_POINTER_FORWARD */
      RemoteAddrPtr);

    /* #40 Leave critical section. */
    SOAD_END_CRITICAL_SECTION();
  }
} /* SoAd_RemoteAddr_UpdateRcvRemAddr() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_GetState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(SoAd_RemAddrStateType, SOAD_CODE) SoAd_RemoteAddr_GetState(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return remote address state. */
  return SoAd_GetRemAddrStateDynOfSoConDyn(SoConIdx, PartitionIdx);
} /* SoAd_RemoteAddr_GetState() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_IsForceRelease()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, SOAD_CODE) SoAd_RemoteAddr_IsForceRelease(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isForceRelease = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if force release is requested for the current socket connection. */
  if ( SoAd_GetRemAddrReleaseOfSoConDyn(SoConIdx, PartitionIdx) == SOAD_RELEASE_REM_ADDR_FORCE )
  {
    isForceRelease = TRUE;
  }

  return isForceRelease;
} /* SoAd_RemoteAddr_IsForceRelease() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_Get()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, SOAD_CODE) SoAd_RemoteAddr_Get(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section. */
  SOAD_BEGIN_CRITICAL_SECTION();

  /* #20 Copy remote address to provided struct. */
  /* PRQA S 310 1 */ /* MD_SoAd_PointerCastOnStruct */
  SoAd_Util_CopySockAddr(IpAddrPtr, SOAD_A_P2CONST(SoAd_SockAddrType)SoAd_GetAddrRemAddrDynOfSoConDyn(SoConIdx,        /* SBSW_SOAD_POINTER_AND_CONST_POINTER_FORWARD_CSL02 */
    PartitionIdx));

  /* #30 Leave critical section. */
  SOAD_END_CRITICAL_SECTION();
} /* SoAd_RemoteAddr_Get() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_Set()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, SOAD_CODE) SoAd_RemoteAddr_Set(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  boolean SetOnlyIfCurrentHasWildcards,
  boolean AllowSettingWildcards)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if new remote address may be set considering wildcards (if required) in the current remote address. */
  if ( (SetOnlyIfCurrentHasWildcards == FALSE) ||
    ((SoAd_GetRemAddrStateDynOfSoConDyn(SoConIdx, PartitionIdx) <= SOAD_SOCON_IP_ANY_PORT_ANY) &&
      ((SoAd_GetRemAddrStateDynOfSoConDyn(SoConIdx, PartitionIdx) & SOAD_SOCON_MASK_ANY) != 0u)) )
  {
    /* #20 Derive remote address state. */
    const SoAd_RemAddrStateType remAddrState = SoAd_RemoteAddr_DeriveState(RemoteAddrPtr);                             /* SBSW_SOAD_CONST_POINTER_FORWARD */

    /* #30 Check if remote address is allowed to contain wildcards or the ip address and port is set. */
    if ( (AllowSettingWildcards == TRUE) || (remAddrState == SOAD_SOCON_IP_SET_PORT_SET) )
    {
      /* #40 Set remote address and state. */
      SoAd_RemoteAddr_Overwrite(SoConIdx, PartitionIdx, RemoteAddrPtr, remAddrState);                                  /* SBSW_SOAD_CONST_POINTER_FORWARD */ 

      retVal = E_OK;
    }
  }

  return retVal;
} /* SoAd_RemoteAddr_Set() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_DeriveState()
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
FUNC(SoAd_RemAddrStateType, SOAD_CODE) SoAd_RemoteAddr_DeriveState(
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  SoAd_RemAddrStateType remAddrState; /* Not initialized: Improve readability. */

  /* ----- Implementation ----------------------------------------------- */
#if ( SOAD_IPV6 == STD_ON )
  /* #10 Check for IPv6 address (if enabled). */
  if ( RemoteAddrPtr->domain == SOAD_AF_INET6 )
  {
    /* #100 Assume IP address state "ANY" if IP address is wildcard. */
    /* PRQA S 310, 3305 4 */ /* MD_SoAd_PointerCastOnStruct */
    if ( (((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))RemoteAddrPtr)->addr[0u] == SOAD_IPADDR_ANY) &&
      (((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))RemoteAddrPtr)->addr[1u] == SOAD_IPADDR_ANY) &&
      (((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))RemoteAddrPtr)->addr[2u] == SOAD_IPADDR_ANY) &&
      (((P2CONST(SoAd_SockAddrInet6Type, AUTOMATIC, AUTOMATIC))RemoteAddrPtr)->addr[3u] == SOAD_IPADDR_ANY) )
    {
      remAddrState = (SoAd_RemAddrStateType)(SOAD_SOCON_IP_ANY_PORT_ANY & SOAD_SOCON_MASK_IPADDR);
    }
    /* #101 Assume IP address state "SET" otherwise. */
    else
    {
      remAddrState = (SoAd_RemAddrStateType)(SOAD_SOCON_IP_SET_PORT_ANY & SOAD_SOCON_MASK_IPADDR);
    }
  }
  else
#endif /* SOAD_IPV6 == STD_ON */
  /* #11 Check for IPv4 address otherwise. */
  {
    /* #110 Assume IP address state "ANY" if IP address is wildcard. */
    /* PRQA S 310, 3305 1 */ /* MD_SoAd_PointerCastOnStruct */
    if ( ((P2CONST(SoAd_SockAddrInetType, AUTOMATIC, AUTOMATIC))RemoteAddrPtr)->addr[0u] == SOAD_IPADDR_ANY )
    {
      remAddrState = (SoAd_RemAddrStateType)(SOAD_SOCON_IP_ANY_PORT_ANY & SOAD_SOCON_MASK_IPADDR);
    }
    /* #111 Assume IP address state "SET" otherwise. */
    else
    {
      remAddrState = (SoAd_RemAddrStateType)(SOAD_SOCON_IP_SET_PORT_ANY & SOAD_SOCON_MASK_IPADDR);
    }
  }

  /* #20 Assume port state "ANY" if port is wildcard. */
  /* PRQA S 310, 3305 1 */ /* MD_SoAd_PointerCastOnStruct */
  if ( ((P2CONST(SoAd_SockAddrInetXType, AUTOMATIC, AUTOMATIC))RemoteAddrPtr)->port == SOAD_PORT_ANY )
  {
    remAddrState |= (SOAD_SOCON_IP_ANY_PORT_ANY & SOAD_SOCON_MASK_PORT);
  }
  /* #21 Assume port state "SET" otherwise. */
  else
  {
    remAddrState |= (SOAD_SOCON_IP_SET_PORT_SET & SOAD_SOCON_MASK_PORT);
  }

  return remAddrState;
} /* SoAd_RemoteAddr_DeriveState() */

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_HasConfigWildcard()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, SOAD_CODE) SoAd_RemoteAddr_HasConfigWildcard(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean IsSetRequired)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean hasWildcards = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if configured remote address requires set values. */
  if ( IsSetRequired == TRUE )
  {
    /* #20 Check if configured remote address is set but contains wildcards. */
    if ( (SoAd_GetRemAddrCfgStateOfSoCon(SoConIdx, PartitionIdx) <= SOAD_SOCON_IP_ANY_PORT_ANY) &&
      ((SoAd_GetRemAddrCfgStateOfSoCon(SoConIdx, PartitionIdx) & SOAD_SOCON_MASK_ANY) != 0u) )
      {
        hasWildcards = TRUE;
      }
  }
  /* #30 Check only for wildcards otherwise. */
  else
  {
    if ( (SoAd_GetRemAddrCfgStateOfSoCon(SoConIdx, PartitionIdx) & SOAD_SOCON_MASK_ANY) != 0u )
      {
        hasWildcards = TRUE;
      }
  }

  return hasWildcards;
} /* SoAd_RemoteAddr_HasConfigWildcard() */

#define SOAD_STOP_SEC_CODE
#include "SoAd_MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 * see file SoAd.c
 */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_RemoteAddr.c
 *********************************************************************************************************************/
