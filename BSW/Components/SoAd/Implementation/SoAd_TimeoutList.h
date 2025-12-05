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
/*!        \file  SoAd_TimeoutList.h
 *        \brief  Socket Adaptor header file
 *
 *      \details  Vector static code header file for AUTOSAR Socket Adaptor sub-component TimeoutList.
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

#if !defined (SOAD_TIMEOUT_LIST_H)
# define SOAD_TIMEOUT_LIST_H

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
  SoAd_TimeoutList_Init()
**********************************************************************************************************************/
/*! \brief        Initializes all timeout lists.
 *  \details      Refer to "Concept for TimeoutList" in SoAd_TimeoutList.c.
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TimeoutList_Init(
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
  SoAd_TimeoutList_SetElement()
**********************************************************************************************************************/
/*! \brief        Sets an element in a specific timeout list.
 *  \details      Adds an element if not already in list or updates element in case of SOAD_TIMEOUT_LIST_UDP_ALIVE and
 *                SOAD_TIMEOUT_LIST_N_PDU otherwise. No update is performed for SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT.
 *                Refer to "Concept for TimeoutList" in SoAd_TimeoutList.c.
 *  \param[in]    TimeoutList   Name of timeout list.
 *                              [range: SOAD_TIMEOUT_LIST_UDP_ALIVE, SOAD_TIMEOUT_LIST_N_PDU,
 *                                      SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT]
 *  \param[in]    HandleIdx     Handle index
 *                              [range: HandleIdx < SoAd_GetSizeOf<HandleType>(); HandleType: SoCon, NPduUdpTx]
 *  \param[in]    Timeout       Configured timeout value.
 *  \param[in]    InstanceIdx   Instance index.
 *                              [range: InstanceIdx < SoAd_GetSizeOfInstance()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       E_OK          Element added or updated.
 *  \return       E_NOT_OK      List size is not sufficient (occurs if limitation is enabled only) or element is
 *                              already in list (SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT).
 *  \pre          Timeout list elements identified by parameter "TimeoutList" have to exist (refer to Safe BSW
 *                assumptions for TimeoutListIdent -> TimeoutListNPduUdpTxConfig and TimeoutListIdent ->
 *                TimeoutListUdpAliveConfig and TimeoutListIdent -> TimeoutListTcpAutoConnectConfig.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_SetElement(
  SoAd_TimeoutListEnumType TimeoutList,
  uint32 HandleIdx,
  uint32 Timeout,
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
  SoAd_TimeoutList_GetCurrentTimeout()
**********************************************************************************************************************/
/*! \brief        Gets the current timeout value of an element in a specific timeout list.
 *  \details      Refer to "Concept for TimeoutList" in SoAd_TimeoutList.c.
 *  \param[in]    TimeoutList   Name of timeout list.
 *                              [range: SOAD_TIMEOUT_LIST_UDP_ALIVE, SOAD_TIMEOUT_LIST_N_PDU,
 *                                      SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT]
 *  \param[in]    HandleIdx     Handle index
 *                              [range: HandleIdx < SoAd_GetSizeOf<HandleType>(); HandleType: SoCon, NPduUdpTx]
 *  \param[out]   TimeoutPtr    Pointer to current timeout value.
 *  \param[in]    TimerCounter  Running timer counter value.
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \return       E_OK          Element is in timeout list and current timeout value is returned.
 *  \return       E_NOT_OK      Element is not in timeout list or function was called for TimeoutList
 *                              SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT (currently no getting of value supported).
 *  \pre          Timeout list elements identified by parameter "TimeoutList" have to exist (refer to Safe BSW
 *                assumptions for TimeoutListIdent -> TimeoutListNPduUdpTxConfig and TimeoutListIdent ->
 *                TimeoutListUdpAliveConfig.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_GetCurrentTimeout(
  SoAd_TimeoutListEnumType TimeoutList,
  uint32 HandleIdx,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) TimeoutPtr,
  uint32 TimerCounter,
  SoAd_PartitionConfigIdxType PartitionIdx);

/**********************************************************************************************************************
  SoAd_TimeoutList_CheckElements()
**********************************************************************************************************************/
/*! \brief        Checks for a timeout in a specific timeout list.
 *  \details      Refer to "Concept for TimeoutList" in SoAd_TimeoutList.c.
 *  \param[in]    TimeoutList   Name of timeout list.
 *                              [range: SOAD_TIMEOUT_LIST_UDP_ALIVE, SOAD_TIMEOUT_LIST_N_PDU,
 *                                      SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT]
 *  \param[out]   ElementIdxPtr Pointer to element index with timeout.
 *  \param[out]   HandleIdxPtr  Pointer to handle index with timeout.
 *                              [range: HandleIdx < SoAd_GetSizeOf<HandleType>(); HandleType: SoCon, NPduUdpTx]
 *  \param[in]    InstMapIdx    Instance Map index.
 *                              [range: InstMapIdx < SoAd_GetSizeOfInstanceMap()]
 *  \return       E_OK          Element with timeout found and out parameter are set.
 *  \return       E_NOT_OK      No element with timeout found.
 *  \pre          Timeout list elements identified by parameter "TimeoutList" have to exist (refer to Safe BSW
 *                assumptions for TimeoutListIdent -> TimeoutListNPduUdpTxConfig and TimeoutListIdent ->
 *                TimeoutListUdpAliveConfig and TimeoutListIdent -> TimeoutListTcpAutoConnectConfig.
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, SOAD_CODE) SoAd_TimeoutList_CheckElements(
  SoAd_TimeoutListEnumType TimeoutList,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) ElementIdxPtr,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_VAR) HandleIdxPtr,
  SoAd_SizeOfInstanceMapType InstMapIdx);

/**********************************************************************************************************************
  SoAd_TimeoutList_RemoveElement()
**********************************************************************************************************************/
/*! \brief        Removes an element from a specific timeout list.
 *  \details      Refer to "Concept for TimeoutList" in SoAd_TimeoutList.c.
 *  \param[in]    TimeoutList   Name of timeout list.
 *                              [range: SOAD_TIMEOUT_LIST_UDP_ALIVE, SOAD_TIMEOUT_LIST_N_PDU,
 *                                      SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT]
 *  \param[in]    HandleIdx     Handle index (e.g. socket connection index).
 *                              [range: HandleIdx < SoAd_GetSizeOf<HandleType>(); HandleType: SoCon, NPduUdpTx]
 *  \param[in]    InstanceIdx   Instance index.
 *                              [range: InstanceIdx < SoAd_GetSizeOfInstance()]
 *  \param[in]    PartitionIdx  Partition index.
 *                              [range: PartitionIdx < size of SoAd_PCPartitionConfig]
 *  \pre          Timeout list elements identified by parameter "TimeoutList" have to exist (refer to Safe BSW
 *                assumptions for TimeoutListIdent -> TimeoutListNPduUdpTxConfig and TimeoutListIdent ->
 *                TimeoutListUdpAliveConfig and TimeoutListIdent -> TimeoutListTcpAutoConnectConfig.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, SOAD_CODE) SoAd_TimeoutList_RemoveElement(
  SoAd_TimeoutListEnumType TimeoutList,
  uint32 HandleIdx,
  SoAd_SizeOfInstanceType InstanceIdx,
  SoAd_PartitionConfigIdxType PartitionIdx);

# define SOAD_STOP_SEC_CODE
# include "SoAd_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */

#endif /* SOAD_TIMEOUT_LIST_H */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_TimeoutList.h
 *********************************************************************************************************************/
