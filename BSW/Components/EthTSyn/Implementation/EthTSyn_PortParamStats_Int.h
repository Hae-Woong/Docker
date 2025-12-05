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
/*!        \file  EthTSyn_PortParamStats_Int.h
 *         \unit  PortParamStats
 *        \brief  EthTSyn internal header file of the PortParamStats unit.
 *      \details  Interface and type definitions of the PortParamStats unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_PORT_PARAM_STATS_INT_H)
# define ETHTSYN_PORT_PARAM_STATS_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

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
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_Init
 *********************************************************************************************************************/
/*! \brief          Initializes the PortParamStats unit.
 *  \details        -
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxSyncCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "rxSyncCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxSyncCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxFupCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "rxFollowUpCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxFupCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxPdReqCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "rxPdelayRequestCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxPdReqCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxPdRespCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "rxPdelayResponseCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxPdRespCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxPdRespFupCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "rxPdelayResponseFollowUpCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxPdRespFupCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxAnnounceCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "rxAnnounceCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxAnnounceCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxDiscardCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "rxPTPPacketDiscardCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxDiscardCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncPdLostRespExceededCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "pdelayAllowedLostResponsesExceededCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            Function call must be protected against Rx interrupts
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncPdLostRespExceededCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxSyncCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "txSyncCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxSyncCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxFupCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "txFollowUpCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxFupCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxPdReqCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "txPdelayRequestCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxPdReqCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxPdRespCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "txPdelayResponseCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxPdRespCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxPdRespFupCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "txPdelayResponseFollowUpCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxPdRespFupCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxAnnounceCnt
 *********************************************************************************************************************/
/*! \brief          Increments the port parameter "txAnnounceCount" of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxAnnounceCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx);

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHTSYN_PORT_PARAM_STATS_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_PortParamStats_Int.h
 *********************************************************************************************************************/
