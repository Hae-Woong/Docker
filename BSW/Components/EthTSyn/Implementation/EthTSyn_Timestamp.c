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
/*!        \file  EthTSyn_Timestamp.c
 *         \unit  Timestamp
 *        \brief  EthTSyn Timestamp source file
 *      \details  Implementation of the Timestamp unit.
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

#define ETHTSYN_TIMESTAMP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_Timestamp_Int.h"
#include "EthTSyn_GeneratedCfgAccess.h"

#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
# include "EthTSyn.h"
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#define ETHTSYN_TS_NS_PER_SEC                                         (1000000000u)
/* 48 bit seconds */
#define ETHTSYN_TIMESTAMP_MAX_SECONDS                                 (0xFFFFFFFFFFFFuLL)
/* 10^9 nanoseconds per second */
#define ETHTSYN_TIMESTAMP_MAX_NANOSECONDS                             (1000000000uL)
/* 63 bit max timediff in nanoseconds */
#define ETHTSYN_TIMEDIFF_MAX_NANOSECONDS                              (0x7FFFFFFFFFFFFFFFuLL)
#define ETHTSYN_TIMEDIFF_MAX_SECONDS                                  (ETHTSYN_TIMEDIFF_MAX_NANOSECONDS /\
                                                                       ETHTSYN_TS_NS_PER_SEC)
#define ETHTSYN_TS_GLOBAL_SEC_HIGH_SHIFT                              (32u)

/* Virtual local time macros */
#define ETHTSYN_TS_VLT_NS_HIGH_MASK                                   (0xFFFFFFFF00000000uLL)
#define ETHTSYN_TS_VLT_NS_HIGH_SHIFT                                  (32u)
#define ETHTSYN_TS_VLT_NS_LOW_MASK                                    (0x00000000FFFFFFFFuLL)
#define ETHTSYN_TS_VLT_NS_LOW_SHIFT                                   (0u)
#define ETHTSYN_TS_VLT_MAX_SECONDS                                    (18446744073uLL)
/* Max nanosecond value for a VLT when seconds part == ETHTSYN_TS_VLT_MAX_SECONDS */
#define ETHTSYN_TS_VLT_MAX_NANOSECONDS                                (709551615uL)

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

/**********************************************************************************************************************
 *  EthTSyn_Ts_TsToIntTsChecked
 *********************************************************************************************************************/
/*! \brief         Converts a timestamp into an internal timestamp with value range check.
 *  \details       -
 *  \param[in]     TsPtr      Timestamp
 *  \param[out]    IntTsPtr   Converted timestamp
 *  \return        E_OK - Conversion succeeded
 *  \return        E_NOT_OK - Conversion failed
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_TsToIntTsChecked(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)   TsPtr,
    ETHTSYN_P2VAR(EthTSyn_InternalTimestampType) IntTsPtr);

/**********************************************************************************************************************
 *  EthTSyn_Ts_IntTsToTs
 *********************************************************************************************************************/
/*! \brief         Converts an internal timestamp into a timestamp.
 *  \details       -
 *  \param[in]     IntTsPtr   Internal timestamp
 *  \return        The passed internal timestamp as timestamp
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_GlobalTimestampType, ETHTSYN_CODE) EthTSyn_Ts_IntTsToTs(
  ETHTSYN_P2CONST(EthTSyn_InternalTimestampType) IntTsPtr);

/**********************************************************************************************************************
 *  EthTSyn_Ts_AddTimediffToTs
 *********************************************************************************************************************/
/*! \brief         Adds a time difference to an internal timestamp.
 *  \details       -
 *  \param[in]     TimeDiffNs Time difference in nanoseconds
 *  \param[in,out] TsPtr       In: The timestamp
 *                            Out: The timestamp plus the time difference
 *  \return        E_OK - Adding the time difference to the timestamp succeeded
 *  \return        E_NOT_OK - Adding the time difference to the timestamp failed
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE for different IntTsPtr
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_AddTimediffToTs(
                EthTSyn_UTimediffType        TimeDiffNs,
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) TsPtr);

/**********************************************************************************************************************
 * EthTSyn_Ts_SubtractTimediffFromTs
 *********************************************************************************************************************/
/*! \brief          Subtracts a time difference from a timestamp.
 *  \details        -
 *  \param[in]      TimeDiffNs  Time difference in nanoseconds
 *  \param[in,out]  TsPtr       In: The timestamp
 *                             Out: The timestamp minus the time difference
 *  \return         E_OK - Subtracting the time difference from the timestamp succeeded
 *  \return         E_OK - Subtracting the time difference from the timestamp failed
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE for different TsPtr
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_SubtractTimediffFromTs(
                EthTSyn_UTimediffType        TimeDiffNs,
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) TsPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  EthTSyn_Ts_TsToIntTsChecked
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_TsToIntTsChecked(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)   TsPtr,
    ETHTSYN_P2VAR(EthTSyn_InternalTimestampType) IntTsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize the out-parameter. */
  IntTsPtr->seconds = 0u;
  IntTsPtr->nanoseconds = 0u;

  if (EthTSyn_Ts_IsTsValid(TsPtr) == TRUE)
  {
    IntTsPtr->nanoseconds = TsPtr->nanoseconds;
    IntTsPtr->seconds     = TsPtr->seconds;
    IntTsPtr->seconds    |= ((uint64)TsPtr->secondsHi << 32u);

    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_Ts_TsToIntTsChecked() */

/**********************************************************************************************************************
 *  EthTSyn_Ts_IntTsToTs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_GlobalTimestampType, ETHTSYN_CODE) EthTSyn_Ts_IntTsToTs(
  ETHTSYN_P2CONST(EthTSyn_InternalTimestampType) IntTsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_GlobalTimestampType ts = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Convert timestamp into internal timestamp. */
  ts.nanoseconds = IntTsPtr->nanoseconds;
  ts.seconds     = (uint32)IntTsPtr->seconds;
  ts.secondsHi   = (uint16)(IntTsPtr->seconds >> 32u);

  return ts;
} /* EthTSyn_Ts_IntTsToTs() */

/**********************************************************************************************************************
 *  EthTSyn_Ts_AddTimediffToTs
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_AddTimediffToTs(
                EthTSyn_UTimediffType        TimeDiffNs,
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) TsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_InternalTimestampType intTs = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to convert the provided timestamp into internal representation. */
  if (EthTSyn_Ts_TsToIntTsChecked(TsPtr, &intTs) == E_OK)
  {
    /* #20 Calculate time difference of seconds and nanoseconds. */
    const EthTSyn_UTimediffType diffSeconds = TimeDiffNs / ETHTSYN_TIMESTAMP_MAX_NANOSECONDS;
    const EthTSyn_UTimediffType diffNanoseconds = TimeDiffNs % ETHTSYN_TIMESTAMP_MAX_NANOSECONDS;

    /* #30 Calculate sum of seconds and nanoseconds. */
    EthTSyn_UTimediffType sumSeconds = diffSeconds + intTs.seconds;
    EthTSyn_UTimediffType sumNanoseconds = diffNanoseconds + intTs.nanoseconds;

    /* #40 Handle potential nanosecond overflow. */
    sumSeconds += (sumNanoseconds / ETHTSYN_TIMESTAMP_MAX_NANOSECONDS);
    sumNanoseconds %= ETHTSYN_TIMESTAMP_MAX_NANOSECONDS;

    /* #50 Check that sum of seconds is in the valid range of a timestamp (48 bit). */
    if (sumSeconds <= ETHTSYN_TIMESTAMP_MAX_SECONDS)
    {
      /* #60 Add time difference to timestamp. */
      intTs.nanoseconds = (uint32)sumNanoseconds;
      intTs.seconds = sumSeconds;

      (*TsPtr) = EthTSyn_Ts_IntTsToTs(&intTs);

      retVal = E_OK;
    }
  }

  return retVal;
} /* EthTSyn_Ts_AddTimediffToTs() */

/**********************************************************************************************************************
 * EthTSyn_Ts_SubtractTimediffFromTs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_SubtractTimediffFromTs(
                EthTSyn_UTimediffType        TimeDiffNs,
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) TsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_InternalTimestampType intTs = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to convert the provided timestamp into internal representation. */
  if (EthTSyn_Ts_TsToIntTsChecked(TsPtr, &intTs) == E_OK)
  {
    /* #20 Calculate time difference of seconds and nanoseconds. */
    const EthTSyn_UTimediffType diffSeconds = TimeDiffNs / ETHTSYN_TIMESTAMP_MAX_NANOSECONDS;
    const EthTSyn_UTimediffType diffNanoseconds = TimeDiffNs % ETHTSYN_TIMESTAMP_MAX_NANOSECONDS;

    /* #30 Subtract seconds if possible. */
    if (intTs.seconds >= diffSeconds)
    {
      intTs.seconds -= diffSeconds;

      /* #40 Subtract nanoseconds if possible. */
      if (intTs.nanoseconds >= diffNanoseconds)
      {
        intTs.nanoseconds -= (uint32)diffNanoseconds;

        (*TsPtr) = EthTSyn_Ts_IntTsToTs(&intTs);
        retVal = E_OK;
      }
      /* In case of nanoseconds underflow, seconds need to be decremented. */
      else if (intTs.seconds >= 1u)
      {
        intTs.seconds--;
        intTs.nanoseconds += (ETHTSYN_TS_NS_PER_SEC - (uint32)diffNanoseconds);

        (*TsPtr) = EthTSyn_Ts_IntTsToTs(&intTs);
        retVal = E_OK;
      }
      else
      {
        /* Calculation not possible. */
        retVal = E_NOT_OK;
      }
    }
  }

  return retVal;
} /* EthTSyn_Ts_SubtractTimediffFromTs() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/* ----- Public interfaces of Timestamp --------------------------------- */
/**********************************************************************************************************************
 *  EthTSyn_Ts_TsToIntVltChecked
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_TsToIntVltChecked(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) TsPtr,
    ETHTSYN_P2VAR(EthTSyn_IntVltType)          IntVltPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint64 tsSeconds = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize the out parameter. */
  (*IntVltPtr) = 0u;

  /* #10 Get total seconds included in the global timestamp. */
  tsSeconds = (uint64)TsPtr->secondsHi << ETHTSYN_TS_GLOBAL_SEC_HIGH_SHIFT;
  tsSeconds |= (uint64)TsPtr->seconds;

  /* #20 Check if timestamp fits in the VLT. */
  if (((tsSeconds < ETHTSYN_TS_VLT_MAX_SECONDS) && (TsPtr->nanoseconds < ETHTSYN_TS_NS_PER_SEC)) ||
      ((tsSeconds == ETHTSYN_TS_VLT_MAX_SECONDS) && (TsPtr->nanoseconds <= ETHTSYN_TS_VLT_MAX_NANOSECONDS)))
  {
    /* #30 Convert the timestamp into VLT. */
    (*IntVltPtr) = (tsSeconds * ETHTSYN_TS_NS_PER_SEC) + TsPtr->nanoseconds;
    retVal = E_OK;
  }
#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #21 Otherwise, report an error to default error tracer (DET) (if error reporting is enabled). */
  else
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_TS_TS_TO_INT_VLT_CHECKED,
      ETHTSYN_E_INV_TS_FORMAT);
  }
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

  return retVal;
} /* EthTSyn_Ts_TsToIntVltChecked() */

/**********************************************************************************************************************
 *  EthTSyn_Ts_TsToVltChecked
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_TsToVltChecked(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) TsPtr,
    ETHTSYN_P2VAR(StbM_VirtualLocalTimeType)   VltPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_IntVltType intVlt = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize the out parameter. */
  VltPtr->nanosecondsHi = 0u;
  VltPtr->nanosecondsLo = 0u;

  /* #10 Try to convert timestamp into internal virtual local time. */
  if (EthTSyn_Ts_TsToIntVltChecked(TsPtr, &intVlt) == E_OK)
  {
    /* #20 Convert internal virtual local time into virtual local time. */
    (*VltPtr) = EthTSyn_Ts_IntVltToVlt(intVlt);

    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_Ts_TsToVltChecked() */

/**********************************************************************************************************************
 *  EthTSyn_Ts_VltToIntVlt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(EthTSyn_IntVltType, ETHTSYN_CODE) EthTSyn_Ts_VltToIntVlt(
  ETHTSYN_P2CONST(StbM_VirtualLocalTimeType) VltPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_IntVltType intVlt = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Convert virtual local time into internal virtual local time. */
  intVlt = (EthTSyn_IntVltType)((EthTSyn_IntVltType)VltPtr->nanosecondsHi << ETHTSYN_TS_VLT_NS_HIGH_SHIFT);
  intVlt |= (EthTSyn_IntVltType)VltPtr->nanosecondsLo;

  return intVlt;
} /* EthTSyn_Ts_VltToIntVlt() */

/**********************************************************************************************************************
 *  EthTSyn_Ts_IntVltToVlt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(StbM_VirtualLocalTimeType, ETHTSYN_CODE) EthTSyn_Ts_IntVltToVlt(
  EthTSyn_IntVltType IntVlt)
{
  /* ----- Local Variables ---------------------------------------------- */
  StbM_VirtualLocalTimeType vlt = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Convert internal virtual local time into virtual local time. */
  vlt.nanosecondsHi = (uint32)(IntVlt >> ETHTSYN_TS_VLT_NS_HIGH_SHIFT);
  vlt.nanosecondsLo = (uint32)IntVlt;

  return vlt;
} /* EthTSyn_Ts_IntVltToVlt() */

/**********************************************************************************************************************
 * EthTSyn_Ts_IntVltMgmtToTsMgmt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(EthTSyn_TsMgmtType, ETHTSYN_CODE) EthTSyn_Ts_IntVltMgmtToTsMgmt(
  ETHTSYN_P2CONST(EthTSyn_IntVltMgmtType) IntVltMgmtPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TsMgmtType tsMgmt = {0};
  const uint64 intVltSeconds = IntVltMgmtPtr->IntVlt / ETHTSYN_TS_NS_PER_SEC;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Convert the internal virtual local time with validity information into a global timestamp with validity
   *     info. */
  tsMgmt.TsValid = IntVltMgmtPtr->IntVltValid;

  tsMgmt.Ts.secondsHi = (uint16)(intVltSeconds >> ETHTSYN_TS_GLOBAL_SEC_HIGH_SHIFT);
  tsMgmt.Ts.seconds = (uint32)intVltSeconds;
  tsMgmt.Ts.nanoseconds = (uint32)(IntVltMgmtPtr->IntVlt % ETHTSYN_TS_NS_PER_SEC);

  return tsMgmt;
} /* EthTSyn_Ts_IntVltMgmtToTsMgmt() */

/**********************************************************************************************************************
 * EthTSyn_Ts_TimeDiffToTimeIntDiff
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(EthTSyn_TimeIntDiffType, ETHTSYN_CODE) EthTSyn_Ts_TimeDiffToTimeIntDiff(
  EthTSyn_TimediffType TimeDiff)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeIntDiffType timeIntDiff = {0};
  EthTSyn_UTimediffType uTimeDiff = 0u;
  uint64 timeDiffSeconds = 0u;
  uint32 timeDiffNanoseconds = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get sign and absolute value of time diff. */
  if (TimeDiff < 0)
  {
    timeIntDiff.sign = FALSE;
    uTimeDiff = (EthTSyn_UTimediffType)(-TimeDiff);
  }
  else
  {
    timeIntDiff.sign = TRUE;
    uTimeDiff = (EthTSyn_UTimediffType)TimeDiff;
  }

  /* #20 Split absolute value of time diff into seconds and nanoseconds. */
  timeDiffSeconds = uTimeDiff / ETHTSYN_TS_NS_PER_SEC;
  timeDiffNanoseconds = (uint32)(uTimeDiff % ETHTSYN_TS_NS_PER_SEC);

  /* #30 Transform seconds and nanoseconds part into timestamp format. */
  timeIntDiff.diff.secondsHi = (uint16)(timeDiffSeconds >> 32u);
  timeIntDiff.diff.seconds = (uint32)timeDiffSeconds;
  timeIntDiff.diff.nanoseconds = timeDiffNanoseconds;

  return timeIntDiff;
} /* EthTSyn_Ts_TimeDiffToTimeIntDiff() */

/**********************************************************************************************************************
 * EthTSyn_Ts_IngrTsToGlobalTsTimeSrcNeq
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_IngrTsToGlobalTsTimeSrcNeq(
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) IngrTsPtr,
  ETHTSYN_P2CONST(StbM_VirtualLocalTimeType)   RefLocalTimePtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) RefGlobalTimePtr,
                  EthTSyn_IntVltType           RefStbMVlt,
                  EthTSyn_IntVltType           RefHwTime)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_GlobalTimestampType ingrTsGlobal = {0};
  EthTSyn_IntVltType ingrTsAsIntVlt = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* Details for the timestamp conversion:
   *
   *     | <-------------hwTimeDiff--------------> |
   *     |                                         |
   *     | <---corrTime----> | <---swTimeDiff----> |
   *     v                   v                     v
   * t----------------------------------------------------->
   *     ^                   ^                     ^
   *     |                   |                     |
   * ingrTs      RefLocalTime;RefGlobalTime  RefStbMVlt;RefHwTime
   *
   * -->
   * - ingrTsGlobal = RefGlobalTime - corrTime
   * - corrTime = hwTimeDiff - swTimeDiff
   * - hwTimeDiff = RefHwTime - ingrTs
   * - swTimeDiff = RefStbMVlt - RefLocalTime
   */

  /* #10 Try to convert the ingress timestamp into a virtual local time for later calculation and checks. */
  if (EthTSyn_Ts_TsToIntVltChecked(IngrTsPtr, &ingrTsAsIntVlt) == E_OK)
  {
    const EthTSyn_IntVltType refLocalTimeAsIntVlt = EthTSyn_Ts_VltToIntVlt(RefLocalTimePtr);

    /* #20 Check plausibility of the input timestamps. */
    if ((RefHwTime >= ingrTsAsIntVlt) && (RefStbMVlt >= refLocalTimeAsIntVlt))
    {
      /* #30 Calculate HW and SW time differences and verify their plausibility. */
      const EthTSyn_UTimediffType hwTimeDiff = RefHwTime - ingrTsAsIntVlt;
      const EthTSyn_UTimediffType swTimeDiff = RefStbMVlt - refLocalTimeAsIntVlt;

      if (hwTimeDiff >= swTimeDiff)
      {
        const EthTSyn_UTimediffType corrTime = hwTimeDiff - swTimeDiff;

        /* #40 Calculate the global ingress timestamp by subtracting the 'corrTime' from 'RefGlobalTime'. */
        ingrTsGlobal = (*RefGlobalTimePtr);
        retVal = EthTSyn_Ts_SubtractTimediffFromTs(corrTime, &ingrTsGlobal);
      }
    }
  }

  /* #50 Only update the passed timestamp when conversion succeeded. */
  if (retVal == E_OK)
  {
    (*IngrTsPtr) = ingrTsGlobal;
  }

  return retVal;
} /* EthTSyn_Ts_IngrTsToGlobalTsTimeSrcNeq() */

/**********************************************************************************************************************
 * EthTSyn_Ts_IngrTsToGlobalTsTimeSrcEq
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_IngrTsToGlobalTsTimeSrcEq(
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) IngrTsPtr,
  ETHTSYN_P2CONST(StbM_VirtualLocalTimeType)   RefLocalTimePtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) RefGlobalTimePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_GlobalTimestampType ingrTsGlobal = {0};
  EthTSyn_IntVltType ingrTsAsIntVlt = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* Details for the timestamp conversion:
   *
   *     | <---corrTime----> |
   *     v                   v
   * t---------------------------------->
   *     ^                   ^
   *     |                   |
   * ingrTs      RefLocalTime;RefGlobalTime
   *
   * -->
   * - ingrTsGlobal = RefGlobalTime - corrTime
   * - corrTime = RefLocalTime - ingrTs
   */

  /* #10 Try to convert the ingress timestamp into a virtual local time for later calculation and checks. */
  if (EthTSyn_Ts_TsToIntVltChecked(IngrTsPtr, &ingrTsAsIntVlt) == E_OK)
  {
    const EthTSyn_IntVltType refLocalTimeAsIntVlt = EthTSyn_Ts_VltToIntVlt(RefLocalTimePtr);

    /* #20 Check plausibility of the input timestamps. */
    if (refLocalTimeAsIntVlt >= ingrTsAsIntVlt)
    {
      /* #30 Calculate the correction time and the global ingress timestamp by subtracting the 'corrTime' from
       *     'RefGlobalTime. */
      const EthTSyn_UTimediffType corrTime = refLocalTimeAsIntVlt - ingrTsAsIntVlt;

      ingrTsGlobal = (*RefGlobalTimePtr);
      retVal = EthTSyn_Ts_SubtractTimediffFromTs(corrTime, &ingrTsGlobal);
    }
  }

  /* #40 Only update the passed timestamp when conversion succeeded. */
  if (retVal == E_OK)
  {
    (*IngrTsPtr) = ingrTsGlobal;
  }

  return retVal;
} /* EthTSyn_Ts_IngrTsToGlobalTsTimeSrcEq() */

/**********************************************************************************************************************
 * EthTSyn_Ts_EgrTsToGlobalTsTimeSrcNeq
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_EgrTsToGlobalTsTimeSrcNeq(
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) EgrTsPtr,
  ETHTSYN_P2CONST(StbM_VirtualLocalTimeType)   RefLocalTimePtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) RefGlobalTimePtr,
                  EthTSyn_IntVltType           RefStbMVlt,
                  EthTSyn_IntVltType           RefHwTime)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_GlobalTimestampType egrTsGlobal = {0};
  EthTSyn_IntVltType egrTsAsIntVlt = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* Details for the timestamp conversion:
   *
   *      | <--------------corrTime---------------> |
   *      |                                         |
   *      | <--swTimeDiff---> | <---hwTimeDiff----> |
   *      v                   v                     v
   *  t----------------------------------------------------->
   *      ^                   ^                     ^
   *      |                   |                     |
   * RefLocalTime;     RefStbMVlt;RefHwTime       egrTs
   * RefGlobalTime
   *
   * -->
   * - egrTsGlobal = RefGlobalTime + corrTime
   * - corrTime = swTimeDiff + hwTimeDiff
   * - swTimeDiff = RefStbMVlt - RefLocalTime
   * - hwTimeDiff = egrTs - RefHwTime
   */

  /* #10 Try to convert the egress timestamp into a virtual local time for later calculation and checks. */
  if (EthTSyn_Ts_TsToIntVltChecked(EgrTsPtr, &egrTsAsIntVlt) == E_OK)
  {
    const EthTSyn_IntVltType refLocalTimeAsIntVlt = EthTSyn_Ts_VltToIntVlt(RefLocalTimePtr);

    /* #20 Check plausibility of the input timestamps. */
    if ((RefStbMVlt >= refLocalTimeAsIntVlt) && (egrTsAsIntVlt >= RefHwTime))
    {
      /* #30 Calculate 'corrTime' and verify plausibility. */
      const EthTSyn_UTimediffType swTimeDiff = RefStbMVlt - refLocalTimeAsIntVlt;
      const EthTSyn_UTimediffType hwTimeDiff = egrTsAsIntVlt - RefHwTime;
      const EthTSyn_UTimediffType corrTime = swTimeDiff + hwTimeDiff;

      /* Ensure that no overflow occurred in calculation of 'corrTime'. */
      if (corrTime >= swTimeDiff)
      {
        /* #40 Calculate the global egress timestamp by adding the 'corrTime' to the 'RefGlobalTime'. */
        egrTsGlobal = (*RefGlobalTimePtr);
        retVal = EthTSyn_Ts_AddTimediffToTs(corrTime, &egrTsGlobal);
      }
    }
  }

  /* #50 Only update the passed timestamp when conversion succeeded. */
  if (retVal == E_OK)
  {
    (*EgrTsPtr) = egrTsGlobal;
  }

  return retVal;
} /* EthTSyn_Ts_EgrTsToGlobalTsTimeSrcNeq() */

/**********************************************************************************************************************
 * EthTSyn_Ts_EgrTsToGlobalTsTimeSrcEq
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_EgrTsToGlobalTsTimeSrcEq(
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) EgrTsPtr,
  ETHTSYN_P2CONST(StbM_VirtualLocalTimeType)   RefLocalTimePtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) RefGlobalTimePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_GlobalTimestampType egrTsGlobal = {0};
  EthTSyn_IntVltType egrTsAsIntVlt = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* Details for the timestamp conversion:
   *
   *      | <-----corrTime----> |
   *      v                     v
   *  t--------------------------->
   *      ^                     ^
   *      |                     |
   * RefLocalTime;            egrTs
   * RefGlobalTime
   *
   * -->
   * - egrTsGlobal = RefGlobalTime + corrTime
   * - corrTime = egrTs - RefLocalTime
   */

  /* #10 Try to convert the egress timestamp into a virtual local time for later calculation and checks. */
  if (EthTSyn_Ts_TsToIntVltChecked(EgrTsPtr, &egrTsAsIntVlt) == E_OK)
  {
    const EthTSyn_IntVltType refLocalTimeAsIntVlt = EthTSyn_Ts_VltToIntVlt(RefLocalTimePtr);

    /* #20 Check plausibility of the input timestamps. */
    if (egrTsAsIntVlt >= refLocalTimeAsIntVlt)
    {
      /* #30 Calculate the correction time and the global egress timestamp by adding the 'corrTime' to
       *     'RefGlobalTime. */
      const EthTSyn_UTimediffType corrTime = egrTsAsIntVlt - refLocalTimeAsIntVlt;

      egrTsGlobal = (*RefGlobalTimePtr);
      retVal = EthTSyn_Ts_AddTimediffToTs(corrTime, &egrTsGlobal);
    }
  }

  /* #40 Only update the passed timestamp when conversion succeeded. */
  if (retVal == E_OK)
  {
    (*EgrTsPtr) = egrTsGlobal;
  }

  return retVal;
} /* EthTSyn_Ts_EgrTsToGlobalTsTimeSrcEq() */

/**********************************************************************************************************************
 *  EthTSyn_Ts_TsMinusTs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_TsMinusTs(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) Ts1Ptr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) Ts2Ptr,
    ETHTSYN_P2VAR(EthTSyn_TimediffType)        TimeDiffPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_TimediffType diffSeconds = 0;
  EthTSyn_TimediffType diffNanoseconds = 0;
  EthTSyn_UTimediffType diffSecondsU64 = 0u;
  EthTSyn_InternalTimestampType intTs1 = {0};
  EthTSyn_InternalTimestampType intTs2 = {0};

  /* ----- Implementation ----------------------------------------------- */
  (*TimeDiffPtr) = 0;

  /* #10 Try to convert the provided timestamps into internal representation. */
  retVal = EthTSyn_Ts_TsToIntTsChecked(Ts1Ptr, &intTs1);
  retVal |= EthTSyn_Ts_TsToIntTsChecked(Ts2Ptr, &intTs2);

  if (retVal == E_OK)
  {
    /* #20 Check if time difference can be expressed as sint64 nanoseconds value. */
    /* No overflow is expected due Seconds field of EthTSyn_TimestampType should not exceed 2^48. */
    diffSeconds = (EthTSyn_TimediffType)intTs1.seconds - (EthTSyn_TimediffType)intTs2.seconds;
    diffNanoseconds = (EthTSyn_TimediffType)intTs1.nanoseconds - (EthTSyn_TimediffType)intTs2.nanoseconds;

    if (diffSeconds < 0)
    {
      diffSecondsU64 = (EthTSyn_UTimediffType)(-diffSeconds);
    }
    else
    {
      diffSecondsU64 = (EthTSyn_UTimediffType)diffSeconds;
    }

    if (diffSecondsU64 < (EthTSyn_UTimediffType)ETHTSYN_TIMEDIFF_MAX_SECONDS)
    {
      /* #30 Calculate time difference. */
      /* set seconds value */
      (*TimeDiffPtr) = diffSeconds * ((EthTSyn_TimediffType)ETHTSYN_TIMESTAMP_MAX_NANOSECONDS);

      /* add nanoseconds value */
      (*TimeDiffPtr) += diffNanoseconds;

      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
} /* EthTSyn_Ts_TsMinusTs() */

/**********************************************************************************************************************
 *  EthTSyn_Ts_TsPlusUTimediff
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_TsPlusUTimediff(
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) TsPtr,
                EthTSyn_UTimediffType        TimeDiff)
{
  /* ----- Implementation ----------------------------------------------- */
  return EthTSyn_Ts_AddTimediffToTs(TimeDiff, TsPtr);
} /* EthTSyn_Ts_TsPlusUTimediff() */

/**********************************************************************************************************************
 * EthTSyn_Ts_SumUTimediffsChecked
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_SumUTimediffsChecked(
  ETHTSYN_P2CONST(EthTSyn_UTimediffType) TimediffValuesPtr,
                  uint8                  TimediffCount,
    ETHTSYN_P2VAR(EthTSyn_UTimediffType) TotalTimediffPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK; /* Initialize with E_OK because its easier to detect an error later. */
  uint8_least timediffIter = 0u;
  EthTSyn_UTimediffType unsignedTimediffSum = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all time difference values. */
  for (timediffIter = 0u; timediffIter < TimediffCount; timediffIter++)
  {
    /* #20 Add the current time difference value to the sum. */
    unsignedTimediffSum += TimediffValuesPtr[timediffIter];

    /* #30 Check if an overflow occurred. */
    if (unsignedTimediffSum < TimediffValuesPtr[timediffIter])
    {
      /* #40 Cancel iteration. */
      retVal = E_NOT_OK;
      break;
    }
  }

  /* #50 Set the out parameter TotalTimediffPtr to the sum of all time difference. */
  (*TotalTimediffPtr) = unsignedTimediffSum;

  return retVal;
} /* EthTSyn_Ts_SumUTimediffsChecked() */

/**********************************************************************************************************************
 * EthTSyn_Ts_ResetTsMgmt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_Ts_ResetTsMgmt(
  ETHTSYN_P2VAR(EthTSyn_TsMgmtType) TsMgmtPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  TsMgmtPtr->Ts.secondsHi   = 0;
  TsMgmtPtr->Ts.seconds     = 0;
  TsMgmtPtr->Ts.nanoseconds = 0;

  TsMgmtPtr->TsValid        = FALSE;
} /* EthTSyn_Ts_ResetTsMgmt() */

/**********************************************************************************************************************
 * EthTSyn_Ts_IsTsValid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, ETHTSYN_CODE) EthTSyn_Ts_IsTsValid(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) TsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean tsValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if (TsPtr->nanoseconds < ETHTSYN_TS_NS_PER_SEC)
  {
    tsValid = TRUE;
  }
#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  else
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_TS_IS_TS_VALID,
      ETHTSYN_E_INV_TS_FORMAT);
  }
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

  return tsValid;
} /* EthTSyn_Ts_IsTsValid() */

/**********************************************************************************************************************
 * EthTSyn_Ts_IsStbmTsValid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, ETHTSYN_CODE) EthTSyn_Ts_IsStbmTsValid(
  ETHTSYN_P2CONST(StbM_TimeStampType) StbmTsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean tsValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if (StbmTsPtr->nanoseconds < ETHTSYN_TS_NS_PER_SEC)
  {
    tsValid = TRUE;
  }
#if (ETHTSYN_DEV_ERROR_REPORT == STD_ON)
  else
  {
    (void)Det_ReportError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_TS_IS_STBM_TS_VALID,
      ETHTSYN_E_INV_TS_FORMAT);
  }
#endif /* (ETHTSYN_DEV_ERROR_REPORT == STD_ON) */

  return tsValid;
} /* EthTSyn_Ts_IsStbmTsValid() */

#define ETHTSYN_STOP_SEC_CODE
#include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Timestamp.c
 *********************************************************************************************************************/
