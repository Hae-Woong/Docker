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
/*!        \file  EthTSyn_PortParamStats.c
 *         \unit  PortParamStats
 *        \brief  EthTSyn PortParamStats source file
 *      \details  Implementation of the PortParamStats unit.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define ETHTSYN_PORT_PARAM_STATS_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_GeneratedCfgAccess.h"
#include "EthTSyn.h"
#include "EthTSyn_Init_Int.h"

#include "EthTSyn_PortParamStats.h"
#include "EthTSyn_PortParamStats_Int.h"

#include "SchM_EthTSyn.h"
#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define ETHTSYN_PORT_PARAM_STATS_MAX_CNT_VALUE                        0xFFFFFFFFu

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define ETHTSYN_START_SEC_CODE
#include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IntInit
 *********************************************************************************************************************/
/*! \brief          Initializes all port parameter counters of an EthTSyn port.
 *  \details        -
 *  \param[in]      PortParamStatsIdx Index of port parameter statistics for an EthTSyn port.
 *                                    [range: PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn()]
 *  \pre            Must be called within critical section that protects against reception interrupts, unless function
 *                  is called during initialization.
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IntInit(
  EthTSyn_PortParamStatsDynIterType PortParamStatsIdx);

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncCounter
 *********************************************************************************************************************/
/*! \brief          Increments a port parameter counter of an EthTSyn port.
 *  \details        Checks if counter has reached its maximum possible value in order to prevent overflow.
 *  \param[in]      CounterPtr  Port parameter statistics counter which shall be incremented.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncCounter(
  ETHTSYN_P2VAR(uint32) CounterPtr);
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IntInit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IntInit(
  EthTSyn_PortParamStatsDynIterType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_SetRxSyncCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetRxFupCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetRxPdReqCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetRxPdRespCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetRxPdRespFupCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetRxAnnounceCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetRxDiscardCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetPdLostRespExceededCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetTxSyncCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetTxFupCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetTxPdReqCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetTxPdRespCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetTxPdRespFupCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
  EthTSyn_SetTxAnnounceCountOfPortParamStatsDyn(PortParamStatsIdx, 0u);
} /* EthTSyn_PortParamStats_IntInit() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncCounter(
  ETHTSYN_P2VAR(uint32) CounterPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  if ((*CounterPtr) < ETHTSYN_PORT_PARAM_STATS_MAX_CNT_VALUE)
  {
    (*CounterPtr)++;
  }
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
} /* EthTSyn_PortParamStats_IncCounter() */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/* ----- Public interfaces of PortParamStats ----------------------------------- */
/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_Init(void)
{
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortParamStatsDynIterType portParamStatsIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for (portParamStatsIter = 0u; portParamStatsIter < EthTSyn_GetSizeOfPortParamStatsDyn(); portParamStatsIter++)
  {
    EthTSyn_PortParamStats_IntInit(portParamStatsIter);
  }
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_Init() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxSyncCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxSyncCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrRxSyncCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncRxSyncCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxFupCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxFupCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrRxFupCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncRxFupCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxPdReqCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxPdReqCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrRxPdReqCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncRxPdReqCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxPdRespCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxPdRespCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrRxPdRespCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncRxPdRespCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxPdRespFupCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxPdRespFupCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrRxPdRespFupCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncRxPdRespFupCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxAnnounceCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxAnnounceCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrRxAnnounceCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncRxAnnounceCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncRxDiscardCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncRxDiscardCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrRxDiscardCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncRxDiscardCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncPdLostRespExceededCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncPdLostRespExceededCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrPdLostRespExceededCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncPdLostRespExceededCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxSyncCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxSyncCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrTxSyncCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncTxSyncCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxFupCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxFupCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrTxFupCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncTxFupCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxPdReqCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxPdReqCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrTxPdReqCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncTxPdReqCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxPdRespCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxPdRespCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrTxPdRespCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncTxPdRespCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxPdRespFupCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxPdRespFupCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrTxPdRespFupCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncTxPdRespFupCnt() */

/**********************************************************************************************************************
 *  EthTSyn_PortParamStats_IncTxAnnounceCnt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortParamStats_IncTxAnnounceCnt(
  EthTSyn_PortParamStats_IdxType PortParamStatsIdx)
{
  /* ----- Implementation ----------------------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
  if (PortParamStatsIdx < EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    EthTSyn_PortParamStats_IncCounter(EthTSyn_GetAddrTxAnnounceCountOfPortParamStatsDyn(PortParamStatsIdx));
  }
#else
  ETHTSYN_DUMMY_STATEMENT(PortParamStatsIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */
} /* EthTSyn_PortParamStats_IncTxAnnounceCnt() */

/* ----- Service interfaces of PortParamStats ---------------------------------- */
#if (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_GetAndResetPortParameterStatistics
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_GetAndResetPortParameterStatistics(
                uint8                       PortIdx,
                boolean                     StatisticsResetNeeded,
  ETHTSYN_P2VAR(EthTSyn_PortParamStatsType) PortParamStatsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = ETHTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check plausibility of input parameters. */
  if (EthTSyn_Init_IsInitialized() == FALSE)
  {
    errorId = ETHTSYN_E_NOT_INITIALIZED;
  }
  else if (PortIdx >= EthTSyn_GetSizeOfPortParamStatsDyn())
  {
    errorId = ETHTSYN_E_PORT_IDX;
  }
  /* PRQA S 2992,2996,2880 4 */ /* MD_EthTSyn_BooleanValueRangeCheck */
  else if ((StatisticsResetNeeded != TRUE) && (StatisticsResetNeeded != FALSE))
  {
    errorId = ETHTSYN_E_PARAM;
  }
  else if (PortParamStatsPtr == NULL_PTR)
  {
    errorId = ETHTSYN_E_PARAM_POINTER;
  }
  else
# endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation --------------------------------------------- */
    /* #20 Retrieve all port parameter counters of passed port. */
    EthTSyn_PortParamStatsType portParams = {0u};

    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
    portParams.rxSyncCount = EthTSyn_GetRxSyncCountOfPortParamStatsDyn(PortIdx);
    portParams.rxFollowUpCount = EthTSyn_GetRxFupCountOfPortParamStatsDyn(PortIdx);
    portParams.rxPdelayRequestCount = EthTSyn_GetRxPdReqCountOfPortParamStatsDyn(PortIdx);
    portParams.rxPdelayResponseCount = EthTSyn_GetRxPdRespCountOfPortParamStatsDyn(PortIdx);
    portParams.rxPdelayResponseFollowUpCount = EthTSyn_GetRxPdRespFupCountOfPortParamStatsDyn(PortIdx);
    portParams.rxAnnounceCount = EthTSyn_GetRxAnnounceCountOfPortParamStatsDyn(PortIdx);
    portParams.rxPTPPacketDiscardCount = EthTSyn_GetRxDiscardCountOfPortParamStatsDyn(PortIdx);
    portParams.pdelayAllowedLostResponsesExceededCount =
      EthTSyn_GetPdLostRespExceededCountOfPortParamStatsDyn(PortIdx);
    portParams.txSyncCount = EthTSyn_GetTxSyncCountOfPortParamStatsDyn(PortIdx);
    portParams.txFollowUpCount = EthTSyn_GetTxFupCountOfPortParamStatsDyn(PortIdx);
    portParams.txPdelayRequestCount = EthTSyn_GetTxPdReqCountOfPortParamStatsDyn(PortIdx);
    portParams.txPdelayResponseCount = EthTSyn_GetTxPdRespCountOfPortParamStatsDyn(PortIdx);
    portParams.txPdelayResponseFollowUpCount = EthTSyn_GetTxPdRespFupCountOfPortParamStatsDyn(PortIdx);
    portParams.txAnnounceCount = EthTSyn_GetTxAnnounceCountOfPortParamStatsDyn(PortIdx);

    (*PortParamStatsPtr) = portParams;

    /* #30 Reset port parameter counters if requested. */
    if (StatisticsResetNeeded == TRUE)
    {
      EthTSyn_PortParamStats_IntInit(PortIdx);
    }
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

    retVal = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report error to default error tracer (DET) if any occurred. */
  if (errorId != ETHTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_GET_AND_RESET_PORT_PARAM_STATS, errorId);
  }
# else
  ETHTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
# endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
} /* EthTSyn_GetAndResetPortParameterStatistics() */
#endif /* (ETHTSYN_PORT_PARAM_STATS_SUPPORT == STD_ON) */

#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_PortParamStats.c
 *********************************************************************************************************************/
