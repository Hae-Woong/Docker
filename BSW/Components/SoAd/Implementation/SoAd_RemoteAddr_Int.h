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
/*!        \file  SoAd_RemoteAddr_Int.h
 *         \unit  RemoteAddr
 *        \brief  Internal header file of the RemoteAddr unit for AUTOSAR Socket Adaptor module.
 *      \details  Interface and type definitions of the RemoteAddr unit for module internal usage.
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

#if !defined (SOAD_REMOTEADDR_INT_H)
# define SOAD_REMOTEADDR_INT_H

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
  SoAd_RemoteAddr_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes socket connection structs related to the remote address.
 *  \details     -
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_RemoteAddr_Init(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_ResetRemAndRcvRemAddr()
 *********************************************************************************************************************/
/*! \brief       Resets remote address and received remote address on a socket connection.
 *  \details     -
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_RemoteAddr_ResetRemAndRcvRemAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_HandleRelease()
 *********************************************************************************************************************/
/*! \brief       Releases the remote address on previous request.
 *  \details     Tries to release the remote address (IP address and port) of the specified socket connection by
 *               setting it back to the configured remote address setting.
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_RemoteAddr_HandleRelease(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_UpdateRcvRemAddr()
 *********************************************************************************************************************/
/*! \brief       Updates received remote address on a socket connection.
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
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_RemoteAddr_UpdateRcvRemAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_GetState()
 *********************************************************************************************************************/
/*! \brief       Returns remote adress state of a socket connection.
 *  \details     -
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return      SOAD_SOCON_IP_SET_PORT_SET
 *  \return      SOAD_SOCON_IP_SET_PORT_ANY
 *  \return      SOAD_SOCON_IP_SET_PORT_NOT
 *  \return      SOAD_SOCON_IP_ANY_PORT_SET
 *  \return      SOAD_SOCON_IP_ANY_PORT_ANY
 *  \return      SOAD_SOCON_IP_ANY_PORT_NOT
 *  \return      SOAD_SOCON_IP_NOT_PORT_SET
 *  \return      SOAD_SOCON_IP_NOT_PORT_ANY
 *  \return      SOAD_SOCON_IP_NOT_PORT_NOT
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   FALSE
 *********************************************************************************************************************/
FUNC(SoAd_RemAddrStateType, SOAD_CODE) SoAd_RemoteAddr_GetState(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_IsForceRelease()
 *********************************************************************************************************************/
/*! \brief       Checks if force release is requested for a specific socket connection.
 *  \details     -
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return      TRUE          Release state is not force release.
 *  \return      FALSE         Release state is force release.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
FUNC(boolean, SOAD_CODE) SoAd_RemoteAddr_IsForceRelease(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_Get()
 *********************************************************************************************************************/
/*! \brief       Returns remote address (domain, remote IP address and port) of a socket connection.
 *  \details     -
 *  \param[in]   SoConIdx      Socket connection index.
 *                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx  Partition index.
 *                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out]  IpAddrPtr     Pointer to remote address.
 *                             [Points to one of the following structs depending on configured IP address version of
 *                             parameter SoConIdx:
 *                               - SoAd_SockAddrInetType for IPv4
 *                               - SoAd_SockAddrInet6Type for IPv6]
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_RemoteAddr_Get(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) IpAddrPtr);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_Set()
 *********************************************************************************************************************/
/*! \brief       Sets remote address on a socket connection.
 *  \details     Function fails if SetOnlyIfCurrentHasWildcards is true but current remote address has no wildcards 
 *               or if AllowSettingWildcards is false but the new state is not SOAD_SOCON_IP_SET_PORT_SET.
 *  \param[in]   SoConIdx                      Socket connection index.
 *                                             [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx                  Partition index.
 *                                             [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]   RemoteAddrPtr                 Pointer to remote address.
 *                                             [Points to one of the following structs depending on configured
 *                                             IP address version of parameter SoConIdx:
 *                                               - SoAd_SockAddrInetType for IPv4
 *                                               - SoAd_SockAddrInet6Type for IPv6]
 *  \param[in]   SetOnlyIfCurrentHasWildcards  Indicator if current remote address has to be checked for
 *                                             wildcards in the context of open on reception.
 *  \param[in]   AllowSettingWildcards         Indicator if it is allowed to set wildcards within
 *                                             Soad_SetRemoteAddr().
 *  \return      E_OK                          Remote address is valid and set.
 *  \return      E_NOT_OK                      Remote address is not valid.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *  \note        This function must be called in a critical section.
 *********************************************************************************************************************/
 FUNC(Std_ReturnType, SOAD_CODE) SoAd_RemoteAddr_Set(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr,
  boolean SetOnlyIfCurrentHasWildcards,
  boolean AllowSettingWildcards);
 
/**********************************************************************************************************************
 *  SoAd_RemoteAddr_DeriveState()
 *********************************************************************************************************************/
/*! \brief       Derives state of remote address by comparing with wildcard mask.
 *  \details     -
 *  \param[in]   RemoteAddrPtr  Pointer to remote address.
 *                              [Points to one of the following structs depending on the domain stored in the pointer
 *                              value:
 *                                - SoAd_SockAddrInetType for IPv4
 *                                - SoAd_SockAddrInet6Type for IPv6]
 *  \return      SOAD_SOCON_IP_SET_PORT_SET
 *  \return      SOAD_SOCON_IP_SET_PORT_ANY
 *  \return      SOAD_SOCON_IP_ANY_PORT_SET
 *  \return      SOAD_SOCON_IP_ANY_PORT_ANY
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE
 *********************************************************************************************************************/
FUNC(SoAd_RemAddrStateType, SOAD_CODE) SoAd_RemoteAddr_DeriveState(
  P2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) RemoteAddrPtr);

/**********************************************************************************************************************
 *  SoAd_RemoteAddr_HasConfigWildcard()
 *********************************************************************************************************************/
/*! \brief       Checks if the configured remote address contains wildcards.
 *  \details     Not set values can be excluded as well.
 *  \param[in]   SoConIdx       Socket connection index.
 *                              [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]   PartitionIdx   Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]   IsSetRequired  Indicator if not set values are excluded.
 *  \return      TRUE           Configured remote address contains wildcards.
 *  \return      FALSE          Configured remote address contains no wildcards.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE for different socket connection indexes, FALSE for same socket connection index.
 *********************************************************************************************************************/
FUNC(boolean, SOAD_CODE) SoAd_RemoteAddr_HasConfigWildcard(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  boolean IsSetRequired);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* SOAD_REMOTEADDR_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_RemoteAddr_Int.h
 *********************************************************************************************************************/
