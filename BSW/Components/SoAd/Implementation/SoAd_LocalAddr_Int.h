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
/*!        \file  SoAd_LocalAddr_Int.h
 *         \unit  LocalAddr
 *        \brief  Internal header file of the LocalAddr unit for AUTOSAR Socket Adaptor module.
 *      \details  Interface and type definitions of the LocalAddr unit for module internal usage.
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

#if !defined (SOAD_LOCALADDR_INT_H)
# define SOAD_LOCALADDR_INT_H

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
  SoAd_LocalAddr_Init()
 *********************************************************************************************************************/
/*! \brief        Initializes local address structs.
 *  \details      -
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_LocalAddr_Init(
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_LocalAddr_GetDomain()
 *********************************************************************************************************************/
/*! \brief      Gets domain of the current local address.
 *  \details    -
 *  \param[in]  LocalAddrIdx    Local address index.
 *                              [range: LocalAddrIdx < SoAd_GetSizeOfLocalAddr()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     SOAD_AF_INET
 *  \return     SOAD_AF_INET6
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(SoAd_DomainType, SOAD_CODE) SoAd_LocalAddr_GetDomain(
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_LocalAddr_GetIpAddr()
 *********************************************************************************************************************/
/*! \brief        Returns the local IP address on a local address identified by a socket connection.
 *  \details      -
 *  \param[in]    SoConIdx          Socket connection index.
 *                                  [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx      Partition index.
 *                                  [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[out]   LocalAddrPtr      Pointer to local address (IP and Port).
 *                                  [Points to one of the following structs depending on configured IP address version
 *                                  of parameter SoConIdx:
 *                                    - SoAd_SockAddrInetType for IPv4
 *                                    - SoAd_SockAddrInet6Type for IPv6]
 *  \param[out]   NetmaskPtr        Pointer to network mask (CIDR Notation).
 *  \param[out]   DefaultRouterPtr  Pointer to default router (gateway).
 *                                  [Points to one of the following structs depending on configured IP address
 *                                  version of parameter SoConIdx:
 *                                    - SoAd_SockAddrInetType for IPv4
 *                                    - SoAd_SockAddrInet6Type for IPv6]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_LocalAddr_GetIpAddr(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) LocalAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_VAR) NetmaskPtr,
  P2VAR(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_VAR) DefaultRouterPtr);

/**********************************************************************************************************************
 *  SoAd_LocalAddr_GetState()
 *********************************************************************************************************************/
/*! \brief      Gets IP address state of the current local address.
 *  \details    -
 *  \param[in]  LocalAddrIdx    Local address index.
 *                              [range: LocalAddrIdx < SoAd_GetSizeOfLocalAddr()]
 *  \param[in]  PartitionIdx    Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     SOAD_IPADDR_STATE_ASSIGNED
 *  \return     SOAD_IPADDR_STATE_ONHOLD
 *  \return     SOAD_IPADDR_STATE_UNASSIGNED
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(SoAd_IpAddrStateType, SOAD_CODE) SoAd_LocalAddr_GetState(
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_LocalAddr_GetTcpIpLocalAddrId()
 *********************************************************************************************************************/
/*! \brief      Get TcpIpLocalAddrId of the current local address.
 *  \details    -
 *  \param[in]  LocalAddrIdx         Local address index.
 *                                   [range: LocalAddrIdx < SoAd_GetSizeOfLocalAddr()]
 *  \param[in]  PartitionIdx         Partition index.
 *                                   [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return     TcpIpLocalAddrId     Id of local address.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(SoAd_LocalAddrIdType, SOAD_CODE) SoAd_LocalAddr_GetTcpIpLocalAddrId(
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
 *  SoAd_LocalAddr_IsAddrActiveOnCtrl
 *********************************************************************************************************************/
/*! \brief        Compares an IP address to all local IP addresses on an IP controller/interface.
 *  \details      -
 *  \param[in]    SoConIdx        Socket connection index.
 *                                [range: SoConIdx < SoAd_GetSizeOfSoCon()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \param[in]    IpAddrPtr       Pointer to IP address.
 *  \return       E_OK            IP address matches one of the local IP addresses.
 *  \return       E_NOT_OK        IP address does not match one of the local IP addresses.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different local IP addresses, FALSE for same local IP address.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_LocalAddr_IsAddrActiveOnCtrl(
  SoAd_SizeOfSoConType SoConIdx,
  SoAd_PartitionConfigIdxType PartitionIdx,
  SoAd_IpAddrConstPtrType IpAddrPtr);

/**********************************************************************************************************************
 * SoAd_LocalAddr_IsRetryEnabled()
 *********************************************************************************************************************/
 /*!
 * \brief         Returns true if feature transmission retry is available on TcpIp controller.
 * \details       -
 *  \param[in]    LocalAddrIdx    Local address index.
 *                                [range: LocalAddrIdx < SoAd_GetSizeOfLocalAddr()]
 *  \param[in]    PartitionIdx    Partition index.
 *                                [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 * \return        TRUE            Support for 'retry storage'
 * \return        FALSE           No support for 'retry storage'
 * \pre           -
 * \context       TASK|ISR2
 * \reentrant     TRUE
 *********************************************************************************************************************/
FUNC(boolean, SOAD_CODE) SoAd_LocalAddr_IsRetryEnabled(
  SoAd_SizeOfLocalAddrType LocalAddrIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* SOAD_LOCALADDR_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_LocalAddr_Int.h
 *********************************************************************************************************************/
