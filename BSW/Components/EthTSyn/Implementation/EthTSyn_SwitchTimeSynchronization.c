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
/*!        \file  EthTSyn_SwitchTimeSynchronization.c
 *        \brief  EthTSyn SwitchTimeSynchronization source file
 *      \details  Implementation of all units included in the logical unit SwitchTimeSynchronization
 *                (SwtTimeSyncSm and RateMeas).
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

#define ETHTSYN_SWT_TIME_SYNC_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_GeneratedCfgAccess.h"

#if (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
# include "EthTSyn.h"
# include "EthTSyn_SwtTimeSyncSm_Int.h"
# include "EthTSyn_RateMeas_Int.h"

# include "StbM.h"
# include "EthIf.h"
# include "EthTSyn_ComCtrl_Int.h"
# include "EthTSyn_Tx_Int.h"
# include "EthTSyn_SyncSendTx_Int.h"
# include "EthTSyn_Timestamp_Int.h"
# include "EthTSyn_MsgDefs_Int.h"
# include "EthTSyn_Memory_Int.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define ETHTSYN_RATE_MEAS_OUT_OF_SYNC_COUNT_MAX_ALLOWED_VALUE        (255u)

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
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 * Internal function prototypes of SwtTimeSyncSm
 *!
 * \unit SwtTimeSyncSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SWTTIMESYNCSM)
/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_Proc
 *********************************************************************************************************************/
/*! \brief        Processes a SwtTimeSyncSm instance.
 *  \details      -
 *  \param[in]    SwtTimeSyncSmIdx Index of the SwtTimeSyncSmIdx
 *                                 [range: SwtTimeSyncSmIdx < EthTSyn_GetSizeOfSwtTimeSyncSm()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SwtTimeSyncSmIdx < EthTSyn_GetSizeOfSwtTimeSyncSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_Proc(
  EthTSyn_SwtTimeSyncSmIterType SwtTimeSyncSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_SetSwitchCorrectionTime
 *********************************************************************************************************************/
/*! \brief        Applies the time corrections to the switch cascade.
 *  \details      -
 *  \param[in]    SwtTimeSyncSmIdx Index of the SwtTimeSyncSmIdx
 *                                 [range: SwtTimeSyncSmIdx < EthTSyn_GetSizeOfSwtTimeSyncSm()]
 *  \param[in]    TimeCorrReqPtr   Information whether correction of offset and/or rate ratio is required
 *  \param[in]    Offset           Value of the offset correction (if required)
 *  \param[in]    RateRatio        Value of the rate ratio correction (if required)
 *  \return       E_OK - Successfully applied corrections to switch cascade or no correction required
 *  \return       E_NOT_OK - Corrections of at least one switch of the cascade failed
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SwtTimeSyncSmIdx < EthTSyn_GetSizeOfSwtTimeSyncSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_SetSwitchCorrectionTime(
                  EthTSyn_SwtTimeSyncSmIterType SwtTimeSyncSmIdx,
  ETHTSYN_P2CONST(EthTSyn_TimeCorrRequiredType) TimeCorrReqPtr,
                  EthTSyn_TimediffType          Offset,
                  float64                       RateRatio);

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_ReportNewSyncState
 *********************************************************************************************************************/
/*! \brief        Report the synchronization state to the user via optional callback function if sync state has
 *                changed.
 *  \details      -
 *  \param[in]    SwtTimeSyncSmIdx Index of the SwtTimeSyncSmIdx
 *                                 [range: SwtTimeSyncSmIdx < EthTSyn_GetSizeOfSwtTimeSyncSm()]
 *  \param[in]    NewSyncState     New synchronization state
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SwtTimeSyncSmIdx < EthTSyn_GetSizeOfSwtTimeSyncSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_ReportNewSyncState(
  EthTSyn_SwtTimeSyncSmIterType SwtTimeSyncSmIdx,
  EthTSyn_SyncStateType         NewSyncState);

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_StoreTsInfo
 *********************************************************************************************************************/
/*! \brief        Stores timestamp information required for switch time synchronization.
 *  \details      Additionally reset sync event timeout counter.
 *  \param[in]    SwtTimeSyncSmIdx Index of the SwtTimeSyncSm
 *                                 [range: SwtTimeSyncSmIdx < EthTSyn_GetSizeOfSwtTimeSyncSm()]
 *  \param[in]    SequenceId       Sequence id
 *  \param[in]    PotPtr           Precise origin timestamp
 *  \param[in]    SwtIngTsPtr      Switch ingress timestamp
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SwtTimeSyncSmIdx < EthTSyn_GetSizeOfSwtTimeSyncSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_StoreTsInfo(
                  EthTSyn_SwtTimeSyncSm_SmIdxType SwtTimeSyncSmIdx,
                  uint16                          SequenceId,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)    PotPtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)    SwtIngTsPtr);

# endif /* ETHTSYN_NOUNIT_SWTTIMESYNCSM */

/**********************************************************************************************************************
 * Internal function prototypes of RateMeas
 *!
 * \unit RateMeas
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_RATEMEAS)
/**********************************************************************************************************************
 *  EthTSyn_RateMeas_IsOfsJumpCorrRequired
 *********************************************************************************************************************/
/*! \brief         Determines whether offset jump correction is required.
 *  \details       Checks if the current time offset exceeds the offset jump correction threshold.
 *  \param[in]     RateMeasIdx Index of the RateMeas instance
 *                             [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in]     Offset      Current time offset
 *  \return        TRUE - Offset jump correction is required
 *  \return        FALSE - Offset jump correction is not required
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_RateMeas_IsOfsJumpCorrRequired(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  EthTSyn_TimediffType     Offset);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_GetNewSyncState
 *********************************************************************************************************************/
/*! \brief         Determines the new synchronization state.
 *  \details       -
 *  \param[in]     RateMeasIdx         Index of the RateMeas instance
 *                                     [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in]     Offset              Current time offset
 *  \param[in]     OfsJumpCorrRequired Indication if offset jump correction is required
 *  \return        ETHTSYN_SYNC - New synchronization state is SYNC
 *  \return        ETHTSYN_UNSYNC - New synchronization state is UNSYNC
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SyncStateType, ETHTSYN_CODE) EthTSyn_RateMeas_GetNewSyncState(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  EthTSyn_TimediffType     Offset,
  boolean                  OfsJumpCorrRequired);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_GetCorrActions
 *********************************************************************************************************************/
/*! \brief         Determines which type of time correction is required.
 *  \details       -
 *  \param[in]     RateMeasIdx         Index of the RateMeas instance
 *                                     [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in]     NewSyncState        New synchronization state
 *                                     [range: ETHTSYN_SYNC, ETHTSYN_UNSYNC]
 *  \param[in]     OfsJumpCorrRequired Indication if offset jump correction is required
 *  \return        ETHTSYN_CORR_ACTION_OFS - only offset correction
 *  \return        ETHTSYN_CORR_ACTION_OFS_GM_RATE - offset and grand master rate ratio correction
 *  \return        ETHTSYN_CORR_ACTION_RATE_REGULATOR - grand master rate ratio and offset rate ratio correction
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_CorrActionType, ETHTSYN_CODE) EthTSyn_RateMeas_GetCorrActions(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  EthTSyn_SyncStateType    NewSyncState,
  boolean                  OfsJumpCorrRequired);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_SetNewSyncState
 *********************************************************************************************************************/
/*! \brief         Sets the new synchronization state as out-parameter.
 *  \details       Resets or increments the out-of-sync counter, depending on whether the new state is SYNC or not.
 *                 The state UNSYNC is only set, if the maximum out-of-sync count is reached or exceeded.
 *  \param[in]     RateMeasIdx         Index of the RateMeas instance
 *                                     [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in]     OfsJumpCorrRequired Indication if offset jump correction is required
 *  \param[in]     NewSyncState        New synchronization state
 *                                     [range: ETHTSYN_SYNC, ETHTSYN_UNSYNC, ETHTSYN_UNCERTAIN]
 *  \param[in,out] CurrSyncStatePtr    In: synchronization state before the synchronization event
 *                                     Out: new synchronization state (if set)
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_SetNewSyncState(
                EthTSyn_RateMeas_IdxType RateMeasIdx,
                boolean                  OfsJumpCorrRequired,
                EthTSyn_SyncStateType    NewSyncState,
  ETHTSYN_P2VAR(EthTSyn_SyncStateType)   CurrSyncStatePtr);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_IsRateRatioCorrNeeded
 *********************************************************************************************************************/
/*! \brief         Checks if ratio ratio time correction is required and tries to calculate it.
 *  \details       -
 *  \param[in]     RateMeasIdx  Index of the RateMeas instance
 *                              [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in]     NewSyncState New synchronization state
 *  \param[in]     CorrAction   The correction actions which are required for the current sync event
 *  \param[in]     Offset       Current time offset
 *  \param[out]    RateRatioPtr Calculated ratio ratio time correction (value only valid if TRUE is returned)
 *  \return        TRUE - RateRatio required and calculation succeeded
 *  \return        FALSE - RateRatio not required or calculation failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_RateMeas_IsRateRatioCorrNeeded(
                EthTSyn_RateMeas_IdxType RateMeasIdx,
                EthTSyn_SyncStateType    NewSyncState,
                EthTSyn_CorrActionType   CorrAction,
                EthTSyn_TimediffType     Offset,
  ETHTSYN_P2VAR(float64)                 RateRatioPtr);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_CalculateMasterAndSwitchTimeDiff
 *********************************************************************************************************************/
/*! \brief         Tries to calculate the master and switch time differences of two consecutive sync events.
 *  \details       -
 *  \param[in]     RateMeasIdx       Index of the RateMeas instance
 *                                   [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[out]    MasterTimeDiffPtr Calculated master time difference (value only valid for return value E_OK)
 *  \param[out]    SwtTimeDiffPtr    Calculated switch time difference (value only valid for return value E_OK)
 *  \return        E_OK - Calculation of time differences succeeded
 *  \return        E_NOT_OK - Calculation of time differences failed
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_RateMeas_CalculateMasterAndSwitchTimeDiff(
                EthTSyn_RateMeas_IdxType RateMeasIdx,
  ETHTSYN_P2VAR(EthTSyn_UTimediffType)   MasterTimeDiffPtr,
  ETHTSYN_P2VAR(EthTSyn_UTimediffType)   SwtTimeDiffPtr);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_UpdateAllRateMeasurements
 *********************************************************************************************************************/
/*! \brief         Adds new calculated master and switch time difference to the stored accumulated values of every
 *                 active rate measurement and returns the index of the longest running rate measurement.
 *  \details       -
 *  \param[in]     RateMeasIdx    Index of the RateMeas instance
 *                                [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in]     MasterTimeDiff Master time difference
 *  \param[in]     SwtTimeDiff    Switch time difference
 *  \return        EthTSyn_GetSizeOfRateMeasBuf() - no valid rate measurement was found
 *  \return        other values - Index of the longest running rate measurement
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_RateMeasBufIterType, ETHTSYN_CODE) EthTSyn_RateMeas_UpdateAllRateMeasurements(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  EthTSyn_UTimediffType    MasterTimeDiff,
  EthTSyn_UTimediffType    SwtTimeDiff);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_CalculateGmRateRatio
 *********************************************************************************************************************/
/*! \brief         Calculates the grand master rate ratio.
 *  \details       -
 *  \param[in]     RateMeasBufIdx Index of the RateMeas buffer
 *                                [range: RateMeasBufIdx < EthTSyn_GetSizeOfRateMeasBuf()]
 *  \return        The calculated grand master rate ratio
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasBufIdx < EthTSyn_GetSizeOfRateMeasBuf();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(float64, ETHTSYN_CODE) EthTSyn_RateMeas_CalculateGmRateRatio(
  EthTSyn_RateMeasBufIterType RateMeasBufIdx);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_CalculateOffsetCorrRateRatio
 *********************************************************************************************************************/
/*! \brief         Calculates the offset rate ratio.
 *  \details       -
 *  \param[in]     RateMeasIdx    Index of the RateMeas instance
 *                                [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in]     RateMeasBufIdx Index of the RateMeas buffer
 *                                [range: RateMeasBufIdx < EthTSyn_GetSizeOfRateMeasBuf()]
 *  \param[in]     NewSyncState   New synchronization state
 *  \param[in]     Offset         Current time offset
 *  \return        The calculated offset rate ratio
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *    requires RateMeasBufIdx < EthTSyn_GetSizeOfRateMeasBuf();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(float64, ETHTSYN_CODE) EthTSyn_RateMeas_CalculateOffsetCorrRateRatio(
  EthTSyn_RateMeas_IdxType    RateMeasIdx,
  EthTSyn_RateMeasBufIterType RateMeasBufIdx,
  EthTSyn_SyncStateType       NewSyncState,
  EthTSyn_TimediffType        Offset);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_ApplyRateRatioBoundaries
 *********************************************************************************************************************/
/*! \brief         Correct rate ratio for the configured maximum and minimum values.
 *  \details       -
 *  \param[in]     RateMeasIdx  Index of the RateMeas instance
 *                              [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in,out] RateRatioPtr In: the calculated ratio ratio
 *                              Out: the corrected ratio ratio
 *  \pre          -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_ApplyRateRatioBoundaries(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  ETHTSYN_P2VAR(float64)   RateRatioPtr);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_CorrAllRateMeasForNewRateRatio
 *********************************************************************************************************************/
/*! \brief         Corrects the switch time difference of every active rate measurement for new rate ratio.
 *  \details       -
 *  \param[in]     RateMeasIdx Index of the RateMeas instance
 *                             [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in]     RateRatio   Calculated and corrected rate ratio
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_CorrAllRateMeasForNewRateRatio(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  float64                  RateRatio);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_StartNewRateMeasurement
 *********************************************************************************************************************/
/*! \brief         Starts a new rate measurement.
 *  \details       -
 *  \param[in]     RateMeasIdx    Index of the RateMeas instance
 *                                [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in]     RateMeasBufIdx Index of the RateMeas buffer
 *                                [range: RateMeasBufIdx < EthTSyn_GetSizeOfRateMeasBuf()]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *    requires RateMeasBufIdx < EthTSyn_GetSizeOfRateMeasBuf();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_StartNewRateMeasurement(
  EthTSyn_RateMeas_IdxType    RateMeasIdx,
  EthTSyn_RateMeasBufIterType RateMeasBufIdx);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_StartSingleRateMeasurement
 *********************************************************************************************************************/
/*! \brief         Sets all variables of a rate measurement buffer to default values and IsActive to TRUE.
 *  \details       -
 *  \param[in]     RateMeasBufIdx Index of the RateMeas buffer
 *                                [range: RateMeasBufIdx < EthTSyn_GetSizeOfRateMeasBuf()]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasBufIdx < EthTSyn_GetSizeOfRateMeasBuf();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_StartSingleRateMeasurement(
  EthTSyn_RateMeasBufIterType RateMeasBufIdx);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_ResetSingleRateMeasurement
 *********************************************************************************************************************/
/*! \brief         Initializes all variables of a rate measurement buffer.
 *  \details       -
 *  \param[in]     RateMeasBufIdx Index of the RateMeas buffer
 *                                [range: RateMeasBufIdx < EthTSyn_GetSizeOfRateMeasBuf()]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasBufIdx < EthTSyn_GetSizeOfRateMeasBuf();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_ResetSingleRateMeasurement(
  EthTSyn_RateMeasBufIterType RateMeasBufIdx);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_ResetAllRateMeasurements
 *********************************************************************************************************************/
/*! \brief         Resets all rate measurements.
 *  \details       Initializes variable data of all rate measurement buffers, resets the sync cycle counter and sets
 *                 the first rate measurement as active.
 *  \param[in]     RateMeasIdx Index of the RateMeas instance
 *                             [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_ResetAllRateMeasurements(
  EthTSyn_RateMeas_IdxType RateMeasIdx);

# endif /* ETHTSYN_NOUNIT_RATEMEAS */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Internal functions of SwtTimeSyncSm
 *!
 * \unit SwtTimeSyncSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SWTTIMESYNCSM)
/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_Proc
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
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_Proc(
  EthTSyn_SwtTimeSyncSmIterType SwtTimeSyncSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if sync event timeout is not expired yet.
   *       Decrement sync event timeout counter.
   *       Check if sync event timeout expired this main function.
   *         Set sync state to UNCERTAIN and report to user if it has changed.
   *         Reset instance of RateMeas unit. */
  if (EthTSyn_GetSyncEventTimeoutCntOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx) > 0u)
  {
    EthTSyn_DecSyncEventTimeoutCntOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx);

    if (EthTSyn_GetSyncEventTimeoutCntOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx) == 0u)
    {
      EthTSyn_SwtTimeSyncSm_ReportNewSyncState(SwtTimeSyncSmIdx, (EthTSyn_SyncStateType)ETHTSYN_UNCERTAIN);
      EthTSyn_RateMeas_Reset(EthTSyn_GetRateMeasIdxOfSwtTimeSyncSm(SwtTimeSyncSmIdx));
    }
  }

  /* #20 Check if state of SwtTimeSyncSm is TS_READY.
   *       Get new sync state and switch time correction values.
   *       Try to apply switch time correction to switch cascade.
   *       Otherwise, new sync sate is UNCERTAIN.
   *       Report sync state to user if it has changed.
   *       Set state of SwtTimeSyncSm to WAIT_TS. */
  if (EthTSyn_GetStateOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx) == ETHTSYN_TS_READY_STATEOFSWTTIMESYNCSMDYN)
  {
    EthTSyn_SyncStateType syncState = EthTSyn_GetSwtSyncStateOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx);
    EthTSyn_TimediffType offset = 0;
    float64 rateRatio = 0.0;
    EthTSyn_TimeCorrRequiredType timeCorrReq = {0};

    timeCorrReq = EthTSyn_RateMeas_GetSyncStateAndCorrVals(EthTSyn_GetRateMeasIdxOfSwtTimeSyncSm(SwtTimeSyncSmIdx),
      EthTSyn_GetAddrTimeStampInfoOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx), &syncState, &offset, &rateRatio);

    if (EthTSyn_SwtTimeSyncSm_SetSwitchCorrectionTime(SwtTimeSyncSmIdx, &timeCorrReq, offset,
          rateRatio) != E_OK)
    {
      syncState = ETHTSYN_UNCERTAIN;
    }

    EthTSyn_SwtTimeSyncSm_ReportNewSyncState(SwtTimeSyncSmIdx, syncState);

    EthTSyn_SetStateOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx, ETHTSYN_WAIT_TS_STATEOFSWTTIMESYNCSMDYN);
  }
} /* EthTSyn_SwtTimeSyncSm_Proc */

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_SetSwitchCorrectionTime
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_SetSwitchCorrectionTime(
                  EthTSyn_SwtTimeSyncSmIterType SwtTimeSyncSmIdx,
  ETHTSYN_P2CONST(EthTSyn_TimeCorrRequiredType) TimeCorrReqPtr,
                  EthTSyn_TimediffType          Offset,
                  float64                       RateRatio)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  if ((TimeCorrReqPtr->IsOfsCorrNeeded == TRUE) || (TimeCorrReqPtr->IsRateRatioCorrNeeded == TRUE))
  {
    ETHTSYN_P2CONST(EthTSyn_TimeIntDiffType) offsetPtr = NULL_PTR;
    ETHTSYN_P2CONST(float64) rateRatioPtr = NULL_PTR;
    ETHTSYN_P2CONST(float64) slaveSwtRateRatioPtr = NULL_PTR;
    const float64 localRateRatio = RateRatio;
    EthTSyn_TimeIntDiffType localOffset = {0};
    localOffset = EthTSyn_Ts_TimeDiffToTimeIntDiff(Offset);

    if (TimeCorrReqPtr->IsOfsCorrNeeded == TRUE)
    {
      offsetPtr = &localOffset;
    }
    if (TimeCorrReqPtr->IsRateRatioCorrNeeded == TRUE)
    {
      rateRatioPtr = &localRateRatio;
    }

    /* #10 Try to apply switch time correction to master switch of the cascade.
     *       Report success of time correction to RateMeas unit.
     *       Check if rate ratio must be applied to slave switches separately.
     *       Apply switch time correction to all slave switches of the cascade if required. */
    retVal = EthIf_SwitchSetCorrectionTime(EthTSyn_GetEthIfSwitchIdxOfSwtTimeSyncSm(SwtTimeSyncSmIdx), offsetPtr,
               rateRatioPtr);

    if (retVal == E_OK)
    {
      ETHTSYN_P2CONST(EthTSyn_TimediffType) correctedOffsetPtr = NULL_PTR;
      const EthTSyn_TimediffType correctedOffset = Offset;
      if (TimeCorrReqPtr->IsOfsCorrNeeded == TRUE)
      {
        correctedOffsetPtr = &correctedOffset;
      }
      EthTSyn_RateMeas_TimeCorrSuccess(EthTSyn_GetRateMeasIdxOfSwtTimeSyncSm(SwtTimeSyncSmIdx),
        correctedOffsetPtr, rateRatioPtr);

      if (EthTSyn_IsApplyRateRatioToSlaveSwitchesOfSwtTimeSyncSm(SwtTimeSyncSmIdx))
      {
        slaveSwtRateRatioPtr = rateRatioPtr;
      }

      if ((offsetPtr != NULL_PTR) || (slaveSwtRateRatioPtr != NULL_PTR))
      {
        EthTSyn_SlaveSwitchIdxListIterType slaveSwitchIter = 0u;
        for(slaveSwitchIter = EthTSyn_GetSlaveSwitchIdxListStartIdxOfSwtTimeSyncSm(SwtTimeSyncSmIdx);
            slaveSwitchIter < EthTSyn_GetSlaveSwitchIdxListEndIdxOfSwtTimeSyncSm(SwtTimeSyncSmIdx);
            slaveSwitchIter++)
        {
          /*@ assert slaveSwitchIter < EthTSyn_GetSizeOfSlaveSwitchIdxList(); */                                       /* VCA_ETHTSYN_0_N_IND_CSL03 */
          const uint8 slaveSwitchIdx = EthTSyn_GetSlaveSwitchIdxList(slaveSwitchIter);
          retVal |= EthIf_SwitchSetCorrectionTime(slaveSwitchIdx, offsetPtr, slaveSwtRateRatioPtr);
        }
      }
    }
  }

  return retVal;
} /* EthTSyn_SwtTimeSyncSm_SetSwitchCorrectionTime */

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_ReportNewSyncState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_ReportNewSyncState(
  EthTSyn_SwtTimeSyncSmIterType SwtTimeSyncSmIdx,
  EthTSyn_SyncStateType         NewSyncState)
{
  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_GetSwtSyncStateOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx) != NewSyncState)
  {
    EthTSyn_SwtTimeSync_SwtSyncStateChgFctPtrType swtSyncStateChgFctPtr =
      EthTSyn_GetSwtSyncStateChgFctPtrOfSwtTimeSyncSm(SwtTimeSyncSmIdx);
    if (swtSyncStateChgFctPtr != NULL_PTR)
    {
      swtSyncStateChgFctPtr((uint8)SwtTimeSyncSmIdx, NewSyncState);
    }

    EthTSyn_SetSwtSyncStateOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx, NewSyncState);
  }
} /* EthTSyn_SwtTimeSyncSm_ReportNewSyncState */

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_StoreTsInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_StoreTsInfo(
                  EthTSyn_SwtTimeSyncSm_SmIdxType SwtTimeSyncSmIdx,
                  uint16                          SequenceId,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)    PotPtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)    SwtIngTsPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_GetStateOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx) == ETHTSYN_WAIT_TS_STATEOFSWTTIMESYNCSMDYN)
  {
    EthTSyn_SwtTimeSync_TimestampInfoStructType tsInfo = {0};
    tsInfo.SequenceId = SequenceId;
    tsInfo.PreciseOriginTimestamp = (*PotPtr);
    tsInfo.SyncIngressTimestamp = (*SwtIngTsPtr);
    EthTSyn_SetTimeStampInfoOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx, tsInfo);

    EthTSyn_SetSyncEventTimeoutCntOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx,
      EthTSyn_GetSyncEventTimeoutOfSwtTimeSyncSm(SwtTimeSyncSmIdx));
    EthTSyn_SetStateOfSwtTimeSyncSmDyn(SwtTimeSyncSmIdx, ETHTSYN_TS_READY_STATEOFSWTTIMESYNCSMDYN);
  }
} /* EthTSyn_SwtTimeSyncSm_StoreTsInfo */

# endif /* ETHTSYN_NOUNIT_SWTTIMESYNCSM */

/**********************************************************************************************************************
 * Internal functions of RateMeas
 *!
 * \unit RateMeas
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_RATEMEAS)
/**********************************************************************************************************************
 *  EthTSyn_RateMeas_IsOfsJumpCorrRequired
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_RateMeas_IsOfsJumpCorrRequired(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  EthTSyn_TimediffType     Offset)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean ofsJumpCorrRequired = FALSE;
  const EthTSyn_UTimediffType unsignedOffset =
    ((Offset < 0) ? ((EthTSyn_UTimediffType)(-Offset)) : ((EthTSyn_UTimediffType)Offset));
  const EthTSyn_UTimediffType ofsJumpCorrThreshold =
    ((EthTSyn_UTimediffType)EthTSyn_GetOfsJumpCorrThresholdNsOfRateMeas(RateMeasIdx));

  /* ----- Implementation ----------------------------------------------- */
  if ((ofsJumpCorrThreshold != 0u) && (unsignedOffset >= ofsJumpCorrThreshold))
  {
    ofsJumpCorrRequired = TRUE;
  }

  return ofsJumpCorrRequired;
} /* EthTSyn_RateMeas_IsOfsJumpCorrRequired */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_GetNewSyncState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SyncStateType, ETHTSYN_CODE) EthTSyn_RateMeas_GetNewSyncState(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  EthTSyn_TimediffType     Offset,
  boolean                  OfsJumpCorrRequired)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SyncStateType newSyncState = ETHTSYN_UNCERTAIN;
  const EthTSyn_UTimediffType unsignedOffset =
    ((Offset < 0) ? ((EthTSyn_UTimediffType)(-Offset)) : ((EthTSyn_UTimediffType)Offset));

  /* ----- Implementation ----------------------------------------------- */
  if (OfsJumpCorrRequired == TRUE)
  {
    newSyncState = ETHTSYN_UNSYNC;
  }
  else if (unsignedOffset >= EthTSyn_GetSyncPrecisionLimitNsOfRateMeas(RateMeasIdx))
  {
    newSyncState = ETHTSYN_UNSYNC;
  }
  else
  {
    newSyncState = ETHTSYN_SYNC;
    EthTSyn_SetWasSyncReachedOfRateMeasDyn(RateMeasIdx, TRUE);
  }

  return newSyncState;
} /* EthTSyn_RateMeas_GetNewSyncState */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_GetCorrActions
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
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_CorrActionType, ETHTSYN_CODE) EthTSyn_RateMeas_GetCorrActions(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  EthTSyn_SyncStateType    NewSyncState,
  boolean                  OfsJumpCorrRequired)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_CorrActionType corrAction = ETHTSYN_CORR_ACTION_NONE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If sync state is ETHTSYN_UNSYNC.
   *       If sync state was never reached yet and initial offset correction is configured, rate ratio and offset.
   *       correction is used.
   *       If offset jump correction is required, only offset correction is used.
   *       Otherwise rate regulator is used. */
  if (NewSyncState == ETHTSYN_UNSYNC)
  {
    if ((EthTSyn_IsUseInitialOffsetCorrectionOfRateMeas(RateMeasIdx)) &&
       (!EthTSyn_IsWasSyncReachedOfRateMeasDyn(RateMeasIdx)))
    {
      corrAction = ETHTSYN_CORR_ACTION_OFS_GM_RATE;
    }
    else if (OfsJumpCorrRequired == TRUE)
    {
      corrAction = ETHTSYN_CORR_ACTION_OFS;
      /* If a large time leap occurs in the GM clock, the master time diff changes drastically in this sync cycle and
         the computed GM rate ratio is therefore very high or low. The offset will now be corrected by a time leap of
         the switch clock. If GM RateRatio correction would be used in addition, it would induce a new offset in the
         next sync cycle. */
    }
    else
    {
      corrAction = ETHTSYN_CORR_ACTION_RATE_REGULATOR;
    }
  }
  /* #20 Otherwise, sync state is ETHTSYN_SYNC and rate regulator is used. */
  else
  {
    corrAction = ETHTSYN_CORR_ACTION_RATE_REGULATOR;
  }

  return corrAction;
} /* EthTSyn_RateMeas_GetCorrActions */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_SetNewSyncState
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
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_SetNewSyncState(
                EthTSyn_RateMeas_IdxType RateMeasIdx,
                boolean                  OfsJumpCorrRequired,
                EthTSyn_SyncStateType    NewSyncState,
  ETHTSYN_P2VAR(EthTSyn_SyncStateType)   CurrSyncStatePtr)
{
  /* ----- Implementation ----------------------------------------------- */
  if (NewSyncState == ETHTSYN_SYNC)
  {
    EthTSyn_SetOutOfSyncCntOfRateMeasDyn(RateMeasIdx, 0u);
    (*CurrSyncStatePtr) = ETHTSYN_SYNC;
  }
  else
  {
    if (EthTSyn_GetOutOfSyncCntOfRateMeasDyn(RateMeasIdx) <
      ETHTSYN_RATE_MEAS_OUT_OF_SYNC_COUNT_MAX_ALLOWED_VALUE)
    {
      EthTSyn_IncOutOfSyncCntOfRateMeasDyn(RateMeasIdx);
    }

    if (NewSyncState == ETHTSYN_UNSYNC)
    {
      if ((EthTSyn_GetOutOfSyncCntOfRateMeasDyn(RateMeasIdx) >= EthTSyn_GetMaxOutOfSyncCntOfRateMeas(RateMeasIdx)) ||
         (OfsJumpCorrRequired == TRUE))
      {
        (*CurrSyncStatePtr) = ETHTSYN_UNSYNC;
      }
    }
    else /* NewSyncState == ETHTSYN_UNCERTAIN */
    {
      (*CurrSyncStatePtr) = ETHTSYN_UNCERTAIN;
    }
  }
} /* EthTSyn_RateMeas_SetNewSyncState */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_IsRateRatioCorrNeeded
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
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_RateMeas_IsRateRatioCorrNeeded(
                EthTSyn_RateMeas_IdxType RateMeasIdx,
                EthTSyn_SyncStateType    NewSyncState,
                EthTSyn_CorrActionType   CorrAction,
                EthTSyn_TimediffType     Offset,
  ETHTSYN_P2VAR(float64)                 RateRatioPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isRateRatioCorrNeeded = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize out-parameter */
  (*RateRatioPtr) = 0.0;

  if ((CorrAction == ETHTSYN_CORR_ACTION_OFS_GM_RATE) || (CorrAction == ETHTSYN_CORR_ACTION_RATE_REGULATOR))
  {
    EthTSyn_UTimediffType masterTimeDiff = 0;
    EthTSyn_UTimediffType swtTimeDiff = 0;

    if (EthTSyn_RateMeas_CalculateMasterAndSwitchTimeDiff(RateMeasIdx, &masterTimeDiff, &swtTimeDiff) == E_OK)
    {
      EthTSyn_RateMeasBufIterType longestMeasBufIdx = EthTSyn_RateMeas_UpdateAllRateMeasurements(RateMeasIdx,
                                                    masterTimeDiff, swtTimeDiff);
      if (longestMeasBufIdx < EthTSyn_GetSizeOfRateMeasBuf()) /* COV_ETHTSYN_MEASBUF_ACTIVE */
      {
        (*RateRatioPtr) = EthTSyn_RateMeas_CalculateGmRateRatio(longestMeasBufIdx);

        if (CorrAction == ETHTSYN_CORR_ACTION_RATE_REGULATOR)
        {
          (*RateRatioPtr) += EthTSyn_RateMeas_CalculateOffsetCorrRateRatio(RateMeasIdx, longestMeasBufIdx,
                              NewSyncState, Offset);
        }

        EthTSyn_RateMeas_ApplyRateRatioBoundaries(RateMeasIdx, RateRatioPtr);
        EthTSyn_RateMeas_StartNewRateMeasurement(RateMeasIdx, longestMeasBufIdx);

        isRateRatioCorrNeeded = TRUE;
      }
    }
  }

  if (isRateRatioCorrNeeded == FALSE)
  {
    EthTSyn_RateMeas_ResetAllRateMeasurements(RateMeasIdx);
  }

return isRateRatioCorrNeeded;
} /* EthTSyn_RateMeas_IsRateRatioCorrNeeded */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_CalculateMasterAndSwitchTimeDiff
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_RateMeas_CalculateMasterAndSwitchTimeDiff(
                EthTSyn_RateMeas_IdxType RateMeasIdx,
  ETHTSYN_P2VAR(EthTSyn_UTimediffType)   MasterTimeDiffPtr,
  ETHTSYN_P2VAR(EthTSyn_UTimediffType)   SwtTimeDiffPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_SwtTimeSync_TimestampInfoStructType currTsInfo = {0};
  EthTSyn_SwtTimeSync_TimestampInfoStructType prevTsInfo = {0};

  /* ----- Implementation ----------------------------------------------- */
  currTsInfo = EthTSyn_GetCurrTsInfoOfRateMeasDyn(RateMeasIdx);
  prevTsInfo = EthTSyn_GetPrevTsInfoOfRateMeasDyn(RateMeasIdx);

  /* Initialize out-parameters */
  (*MasterTimeDiffPtr) = 0u;
  (*SwtTimeDiffPtr) = 0u;

  if (EthTSyn_GetStateOfRateMeasDyn(RateMeasIdx) == ETHTSYN_TS_READY_STATEOFRATEMEASDYN)
  {
    if (currTsInfo.SequenceId == (prevTsInfo.SequenceId + 1u))
    {
      EthTSyn_TimediffType masterTimeDiff = 0;
      EthTSyn_TimediffType swtTimeDiff = 0;

      retVal = EthTSyn_Ts_TsMinusTs(&currTsInfo.PreciseOriginTimestamp, &prevTsInfo.PreciseOriginTimestamp,
                &masterTimeDiff);
      retVal |= EthTSyn_Ts_TsMinusTs(&currTsInfo.SyncIngressTimestamp, &prevTsInfo.SyncIngressTimestamp, &swtTimeDiff);

      if (retVal == E_OK)
      {
        swtTimeDiff -= EthTSyn_GetLastCorrectedOffsetNsOfRateMeasDyn(RateMeasIdx);
        EthTSyn_SetLastCorrectedOffsetNsOfRateMeasDyn(RateMeasIdx, 0);

        if ((masterTimeDiff > 0) && (swtTimeDiff > 0))
        {
          (*MasterTimeDiffPtr) = (EthTSyn_UTimediffType)masterTimeDiff;
          (*SwtTimeDiffPtr) = (EthTSyn_UTimediffType)swtTimeDiff;
        }
        else
        {
          retVal = E_NOT_OK;
        }
      }
    }
  }

return retVal;
} /* EthTSyn_RateMeas_CalculateMasterAndSwitchTimeDiff */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_UpdateAllRateMeasurements
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
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_RateMeasBufIterType, ETHTSYN_CODE) EthTSyn_RateMeas_UpdateAllRateMeasurements(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  EthTSyn_UTimediffType    MasterTimeDiff,
  EthTSyn_UTimediffType    SwtTimeDiff)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_RateMeasBufIterType longestMeasBufIdx = EthTSyn_GetSizeOfRateMeasBuf();
  uint64 nrOfSyncCyclesOfLongestMeas = 0;
  EthTSyn_RateMeasBufIterType rateMeasBufIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for (rateMeasBufIter = EthTSyn_GetRateMeasBufStartIdxOfRateMeas(RateMeasIdx);
       rateMeasBufIter < EthTSyn_GetRateMeasBufEndIdxOfRateMeas(RateMeasIdx);
       rateMeasBufIter++)
  {
    if (EthTSyn_IsActiveOfRateMeasBuf(rateMeasBufIter) == TRUE)
    {
      EthTSyn_RateCorrectedTimediffType accumulatedMasterTimeDiff =
        EthTSyn_GetAccumulatedMasterTimeDiffOfRateMeasBuf(rateMeasBufIter);
      EthTSyn_RateCorrectedTimediffType accumulatedSwtTimeDiff =
        EthTSyn_GetAccumulatedSwtTimeDiffOfRateMeasBuf(rateMeasBufIter);
      uint8 activeNrOfSyncCycles = EthTSyn_GetActiveNrOfSyncCyclesOfRateMeasBuf(rateMeasBufIter);

      EthTSyn_SetAccumulatedMasterTimeDiffOfRateMeasBuf(
        rateMeasBufIter, accumulatedMasterTimeDiff + (EthTSyn_RateCorrectedTimediffType)MasterTimeDiff);
      EthTSyn_SetAccumulatedSwtTimeDiffOfRateMeasBuf(
        rateMeasBufIter, accumulatedSwtTimeDiff + (EthTSyn_RateCorrectedTimediffType)SwtTimeDiff);
      activeNrOfSyncCycles++;
      EthTSyn_SetActiveNrOfSyncCyclesOfRateMeasBuf(rateMeasBufIter, activeNrOfSyncCycles);

      if (activeNrOfSyncCycles > nrOfSyncCyclesOfLongestMeas)
      {
        nrOfSyncCyclesOfLongestMeas = activeNrOfSyncCycles;
        longestMeasBufIdx = rateMeasBufIter;
      }
    }
  }

return longestMeasBufIdx;
} /* EthTSyn_RateMeas_UpdateAllRateMeasurements */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_CalculateGmRateRatio
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(float64, ETHTSYN_CODE) EthTSyn_RateMeas_CalculateGmRateRatio(
  EthTSyn_RateMeasBufIterType RateMeasBufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_RateCorrectedTimediffType accumulatedMasterTimeDiff =
    EthTSyn_GetAccumulatedMasterTimeDiffOfRateMeasBuf(RateMeasBufIdx);
  EthTSyn_RateCorrectedTimediffType accumulatedSwtTimeDiff =
    EthTSyn_GetAccumulatedSwtTimeDiffOfRateMeasBuf(RateMeasBufIdx);

  /* ----- Implementation ----------------------------------------------- */
  return accumulatedMasterTimeDiff / accumulatedSwtTimeDiff;
} /* EthTSyn_RateMeas_CalculateGmRateRatio */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_CalculateOffsetCorrRateRatio
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
ETHTSYN_LOCAL_INLINE FUNC(float64, ETHTSYN_CODE) EthTSyn_RateMeas_CalculateOffsetCorrRateRatio(
  EthTSyn_RateMeas_IdxType    RateMeasIdx,
  EthTSyn_RateMeasBufIterType RateMeasBufIdx,
  EthTSyn_SyncStateType       NewSyncState,
  EthTSyn_TimediffType        Offset)
{
  /* ----- Local Variables ---------------------------------------------- */
  float64 rateRatio = 0.0;
  EthTSyn_RateCorrectedTimediffType accumulatedSwtTimeDiff =
    EthTSyn_GetAccumulatedSwtTimeDiffOfRateMeasBuf(RateMeasBufIdx);
  uint8 syncCycleCounterForOfsCorr = EthTSyn_GetSyncCycleCounterOfRateMeasDyn(RateMeasIdx);

  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_IsSyncCycleCounterRestartedOfRateMeasDyn(RateMeasIdx))
  {
    if (NewSyncState == ETHTSYN_SYNC)
    {
      EthTSyn_SetSyncCycleCounterRestartedOfRateMeasDyn(RateMeasIdx, FALSE);
    }
  }
  else
  {
    if (NewSyncState == ETHTSYN_UNSYNC)
    {
      syncCycleCounterForOfsCorr = 0;
      EthTSyn_SetSyncCycleCounterRestartedOfRateMeasDyn(RateMeasIdx, TRUE);
    }
  }

  rateRatio = (float64)Offset / (float64)accumulatedSwtTimeDiff;
  rateRatio *=
    (float64)EthTSyn_GetActiveNrOfSyncCyclesOfRateMeasBuf(RateMeasBufIdx);
  rateRatio /=
    ((float64)EthTSyn_GetNrSyncCyclesForOfsCorrOfRateMeas(RateMeasIdx) -
    (float64)syncCycleCounterForOfsCorr);

  syncCycleCounterForOfsCorr++;

  if (syncCycleCounterForOfsCorr >= EthTSyn_GetNrSyncCyclesForOfsCorrOfRateMeas(RateMeasIdx))
  {
    EthTSyn_SetSyncCycleCounterOfRateMeasDyn(RateMeasIdx, 0u);
  }
  else
  {
    EthTSyn_SetSyncCycleCounterOfRateMeasDyn(RateMeasIdx, syncCycleCounterForOfsCorr);
  }

  return rateRatio;
} /* EthTSyn_RateMeas_CalculateOffsetCorrRateRatio */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_ApplyRateRatioBoundaries
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_ApplyRateRatioBoundaries(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  ETHTSYN_P2VAR(float64)   RateRatioPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  if ((*RateRatioPtr) > EthTSyn_GetRateRatioMaxOfRateMeas(RateMeasIdx))
  {
    (*RateRatioPtr) = EthTSyn_GetRateRatioMaxOfRateMeas(RateMeasIdx);
  }
  else if ((*RateRatioPtr) < EthTSyn_GetRateRatioMinOfRateMeas(RateMeasIdx))
  {
    (*RateRatioPtr) = EthTSyn_GetRateRatioMinOfRateMeas(RateMeasIdx);
  }
  else
  {
    /* RateRatio is within limits. Nothing to do. */
  }
} /* EthTSyn_RateMeas_ApplyRateRatioBoundaries */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_CorrAllRateMeasForNewRateRatio
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_CorrAllRateMeasForNewRateRatio(
  EthTSyn_RateMeas_IdxType RateMeasIdx,
  float64                  RateRatio)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_RateMeasBufIterType rateMeasBufIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for (rateMeasBufIter = EthTSyn_GetRateMeasBufStartIdxOfRateMeas(RateMeasIdx);
       rateMeasBufIter < EthTSyn_GetRateMeasBufEndIdxOfRateMeas(RateMeasIdx);
       rateMeasBufIter++)
  {
    if (EthTSyn_IsActiveOfRateMeasBuf(rateMeasBufIter) == TRUE)
    {
      EthTSyn_RateCorrectedTimediffType accumulatedSwtTimeDiff =
        EthTSyn_GetAccumulatedSwtTimeDiffOfRateMeasBuf(rateMeasBufIter);
      EthTSyn_SetAccumulatedSwtTimeDiffOfRateMeasBuf(rateMeasBufIter, accumulatedSwtTimeDiff * RateRatio);
    }
  }
} /* EthTSyn_RateMeas_CorrAllRateMeasForNewRateRatio */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_StartNewRateMeasurement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_StartNewRateMeasurement(
  EthTSyn_RateMeas_IdxType    RateMeasIdx,
  EthTSyn_RateMeasBufIterType RateMeasBufIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_GetActiveNrOfSyncCyclesOfRateMeasBuf(RateMeasBufIdx) >=
      EthTSyn_GetRateMeasBufLengthOfRateMeas(RateMeasIdx))
  {
    EthTSyn_RateMeas_StartSingleRateMeasurement(RateMeasBufIdx);
  }
  else
  {
    EthTSyn_RateMeasBufIterType rateMeasBufIter = 0u;
    for (rateMeasBufIter = EthTSyn_GetRateMeasBufStartIdxOfRateMeas(RateMeasIdx);
         rateMeasBufIter < EthTSyn_GetRateMeasBufEndIdxOfRateMeas(RateMeasIdx);
         rateMeasBufIter++) /* COV_ETHTSYN_MEASBUF_INACTIVE */
    {
      /*@ assert rateMeasBufIter < EthTSyn_GetSizeOfRateMeasBuf(); */
      if (EthTSyn_IsActiveOfRateMeasBuf(rateMeasBufIter) == FALSE)
      {
        EthTSyn_RateMeas_StartSingleRateMeasurement(rateMeasBufIter);
        break;
      }
    }
  }
} /* EthTSyn_RateMeas_StartNewRateMeasurement */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_StartSingleRateMeasurement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_StartSingleRateMeasurement(
  EthTSyn_RateMeasBufIterType RateMeasBufIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_RateMeas_ResetSingleRateMeasurement(RateMeasBufIdx);

  EthTSyn_SetActiveOfRateMeasBuf(RateMeasBufIdx, TRUE);
} /* EthTSyn_RateMeas_StartSingleRateMeasurement */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_ResetSingleRateMeasurement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_ResetSingleRateMeasurement(
  EthTSyn_RateMeasBufIterType RateMeasBufIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_SetAccumulatedSwtTimeDiffOfRateMeasBuf(RateMeasBufIdx, 0.0);
  EthTSyn_SetAccumulatedMasterTimeDiffOfRateMeasBuf(RateMeasBufIdx, 0.0);
  EthTSyn_SetActiveNrOfSyncCyclesOfRateMeasBuf(RateMeasBufIdx, 0u);
  EthTSyn_SetActiveOfRateMeasBuf(RateMeasBufIdx, FALSE);
} /* EthTSyn_RateMeas_ResetSingleRateMeasurement */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_ResetAllRateMeasurements
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_ResetAllRateMeasurements(
  EthTSyn_RateMeas_IdxType RateMeasIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_RateMeasBufIterType rateMeasBufIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for (rateMeasBufIter = EthTSyn_GetRateMeasBufStartIdxOfRateMeas(RateMeasIdx);
       rateMeasBufIter < EthTSyn_GetRateMeasBufEndIdxOfRateMeas(RateMeasIdx);
       rateMeasBufIter++)
  {
    EthTSyn_RateMeas_ResetSingleRateMeasurement(rateMeasBufIter);
  }
  EthTSyn_SetActiveOfRateMeasBuf(EthTSyn_GetRateMeasBufStartIdxOfRateMeas(RateMeasIdx), TRUE);

  EthTSyn_SetSyncCycleCounterOfRateMeasDyn(RateMeasIdx, 0u);
  EthTSyn_SetSyncCycleCounterRestartedOfRateMeasDyn(RateMeasIdx, FALSE);
} /* EthTSyn_RateMeas_ResetAllRateMeasurements */

# endif /* ETHTSYN_NOUNIT_RATEMEAS */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Public interfaces of SwtTimeSyncSm
 *!
 * \unit SwtTimeSyncSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SWTTIMESYNCSM)
/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtTimeSyncSmIterType swtTimeSyncSmIter = 0u;
  const EthTSyn_SwtTimeSync_TimestampInfoStructType intTsInfo = {0u};

  /* ----- Implementation ----------------------------------------------- */
  for (swtTimeSyncSmIter = 0; swtTimeSyncSmIter < EthTSyn_GetSizeOfSwtTimeSyncSm(); swtTimeSyncSmIter++)
  {
    EthTSyn_SetStateOfSwtTimeSyncSmDyn(swtTimeSyncSmIter, ETHTSYN_WAIT_TS_STATEOFSWTTIMESYNCSMDYN);
    EthTSyn_SetTimeStampInfoOfSwtTimeSyncSmDyn(swtTimeSyncSmIter, intTsInfo);
    EthTSyn_SetSwtSyncStateOfSwtTimeSyncSmDyn(swtTimeSyncSmIter, ETHTSYN_NEVERSYNC);
    EthTSyn_SetSyncEventTimeoutCntOfSwtTimeSyncSmDyn(swtTimeSyncSmIter, 0u);
  }
} /* EthTSyn_SwtTimeSyncSm_Init() */

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SwtTimeSyncSmIterType swtTimeSyncSmIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for (swtTimeSyncSmIter = 0; swtTimeSyncSmIter < EthTSyn_GetSizeOfSwtTimeSyncSm(); swtTimeSyncSmIter++)
  {
    EthTSyn_SwtTimeSyncSm_Proc(swtTimeSyncSmIter);
  }
} /* EthTSyn_SwtTimeSyncSm_MainFunction() */

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncSendSm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncSendSm(
                  EthTSyn_SwtTimeSyncSm_SmIdxType SwtTimeSyncSmIdx,
                  uint16                          SequenceId,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)    PotPtr,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)             SyncEgrTsInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  if (SwtTimeSyncSmIdx < EthTSyn_GetSizeOfSwtTimeSyncSm())
  {
    if (SyncEgrTsInfoPtr->SwtIngrTsMgmt.TsValid == TRUE)
    {
      EthTSyn_SwtTimeSyncSm_StoreTsInfo(SwtTimeSyncSmIdx, SequenceId, PotPtr, &SyncEgrTsInfoPtr->SwtIngrTsMgmt.Ts);
    }
  }
} /* EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncSendSm() */

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncRcvSm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncRcvSm(
                  EthTSyn_SwtTimeSyncSm_SmIdxType SwtTimeSyncSmIdx,
                  EthTSyn_PdelayType              Pdelay,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)             PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                          FupPayloadPtr,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)             SyncSwtIngrTsMgmtPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  if (SwtTimeSyncSmIdx < EthTSyn_GetSizeOfSwtTimeSyncSm())
  {
    if (SyncSwtIngrTsMgmtPtr->TsValid == TRUE)
    {
      EthTSyn_GlobalTimestampType pot = {0};
      EthTSyn_UTimediffType potCorr = (EthTSyn_UTimediffType)PtpHdrPtr->CorrectionFieldNs;
      potCorr += (EthTSyn_UTimediffType)Pdelay;

      pot.secondsHi = EthTSyn_Mem_GetUint16(FupPayloadPtr, ETHTSYN_MSG_FUP_POT_SEC_HI_PL_REL_OFS);
      pot.seconds = EthTSyn_Mem_GetUint32(FupPayloadPtr, ETHTSYN_MSG_FUP_POT_SEC_LOW_PL_REL_OFS);
      pot.nanoseconds = EthTSyn_Mem_GetUint32(FupPayloadPtr, ETHTSYN_MSG_FUP_POT_NSEC_PL_REL_OFS);

      if (EthTSyn_Ts_TsPlusUTimediff(&pot, potCorr) == E_OK)
      {
        EthTSyn_SwtTimeSyncSm_StoreTsInfo(SwtTimeSyncSmIdx, PtpHdrPtr->SequenceId, &pot,
          &SyncSwtIngrTsMgmtPtr->Ts);
      }
    }
  }
} /* EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncRcvSm() */
# endif /* ETHTSYN_NOUNIT_SWTTIMESYNCSM */

/**********************************************************************************************************************
 * Public interfaces of RateMeas
 *!
 * \unit RateMeas
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_RATEMEAS)
/**********************************************************************************************************************
 *  EthTSyn_RateMeas_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_RateMeasIterType rateMeasIter = 0u;
  EthTSyn_SwtTimeSync_TimestampInfoStructType initSyncTsInfo = {0u};

  /* ----- Implementation ----------------------------------------------- */
  for (rateMeasIter = 0; rateMeasIter < EthTSyn_GetSizeOfRateMeas(); rateMeasIter++)
  {
    EthTSyn_SetStateOfRateMeasDyn(rateMeasIter, ETHTSYN_WAIT_FIRST_TS_STATEOFRATEMEASDYN);
    EthTSyn_SetCurrTsInfoOfRateMeasDyn(rateMeasIter, initSyncTsInfo);
    EthTSyn_SetPrevTsInfoOfRateMeasDyn(rateMeasIter, initSyncTsInfo);

    EthTSyn_SetLastCorrectedOffsetNsOfRateMeasDyn(rateMeasIter, 0);
    EthTSyn_SetOutOfSyncCntOfRateMeasDyn(rateMeasIter, 0u);
    EthTSyn_SetWasSyncReachedOfRateMeasDyn(rateMeasIter, FALSE);

    /* SyncCycleCounter and SyncCycleCounterRestarted are initialized in EthTSyn_RateMeas_ResetAllRateMeasurements() */
    EthTSyn_RateMeas_ResetAllRateMeasurements((EthTSyn_RateMeas_IdxType)rateMeasIter);
  }
} /* EthTSyn_RateMeas_Init() */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_Reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_Reset(
  EthTSyn_RateMeas_IdxType RateMeasIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_RateMeas_ResetAllRateMeasurements(RateMeasIdx);
} /* EthTSyn_RateMeas_Reset() */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_GetSyncStateAndCorrVals
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
 */
FUNC(EthTSyn_TimeCorrRequiredType, ETHTSYN_CODE) EthTSyn_RateMeas_GetSyncStateAndCorrVals(
  EthTSyn_RateMeas_IdxType                                     RateMeasIdx,
  ETHTSYN_P2CONST(EthTSyn_SwtTimeSync_TimestampInfoStructType) TsInfoPtr,
    ETHTSYN_P2VAR(EthTSyn_SyncStateType)                       SyncStatePtr,
    ETHTSYN_P2VAR(EthTSyn_TimediffType)                        OffsetPtr,
    ETHTSYN_P2VAR(float64)                                     RateRatioPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimediffType offset = 0;
  float64 rateRatio = 0.0;
  EthTSyn_TimeCorrRequiredType timeCorrReq = {0u};
  boolean ofsJumpCorrRequired = FALSE;
  EthTSyn_SyncStateType newSyncState = ETHTSYN_UNCERTAIN;

  /* ----- Implementation ----------------------------------------------- */
  timeCorrReq.IsOfsCorrNeeded = FALSE;
  timeCorrReq.IsRateRatioCorrNeeded = FALSE;

  /* #10 Try to calculate offset.
   *       Store timestamp information.
   *       If state is WAIT_FIRST_TS, set state to WAIT_SECOND_TS.
   *       Otherwise set state to TS_READY.
   *       Check if offset jump correction is required.
   *       Determine new synchronization state.
   *       Determine required time correction actions.
   *       Calculate rate ratio time correction if required. */
  if (EthTSyn_Ts_TsMinusTs(&TsInfoPtr->PreciseOriginTimestamp, &TsInfoPtr->SyncIngressTimestamp, &offset) == E_OK)
  {
    EthTSyn_CorrActionType corrAction = ETHTSYN_CORR_ACTION_NONE;

    EthTSyn_SetPrevTsInfoOfRateMeasDyn(RateMeasIdx, EthTSyn_GetCurrTsInfoOfRateMeasDyn(RateMeasIdx));
    EthTSyn_SetCurrTsInfoOfRateMeasDyn(RateMeasIdx, (*TsInfoPtr));

    if (EthTSyn_GetStateOfRateMeasDyn(RateMeasIdx) == ETHTSYN_WAIT_FIRST_TS_STATEOFRATEMEASDYN)
    {
      EthTSyn_SetStateOfRateMeasDyn(RateMeasIdx, ETHTSYN_WAIT_SECOND_TS_STATEOFRATEMEASDYN);
    }
    else /* ETHTSYN_WAIT_SECOND_TS_STATEOFRATEMEASDYN or ETHTSYN_TS_READY_STATEOFRATEMEASDYN */
    {
      EthTSyn_SetStateOfRateMeasDyn(RateMeasIdx, ETHTSYN_TS_READY_STATEOFRATEMEASDYN);
    }

    ofsJumpCorrRequired = EthTSyn_RateMeas_IsOfsJumpCorrRequired(RateMeasIdx, offset);
    newSyncState = EthTSyn_RateMeas_GetNewSyncState(RateMeasIdx, offset, ofsJumpCorrRequired);
    corrAction = EthTSyn_RateMeas_GetCorrActions(RateMeasIdx, newSyncState,
      ofsJumpCorrRequired);

    if ((corrAction == ETHTSYN_CORR_ACTION_OFS) || (corrAction == ETHTSYN_CORR_ACTION_OFS_GM_RATE))
    {
      timeCorrReq.IsOfsCorrNeeded = TRUE;
    }

    timeCorrReq.IsRateRatioCorrNeeded = EthTSyn_RateMeas_IsRateRatioCorrNeeded(RateMeasIdx, newSyncState, corrAction,
                                          offset, &rateRatio);
  }

  /* #20 Set new synchronization state. */
  EthTSyn_RateMeas_SetNewSyncState(RateMeasIdx, ofsJumpCorrRequired, newSyncState, SyncStatePtr);

  (*OffsetPtr) = offset;
  (*RateRatioPtr) = rateRatio;

  return timeCorrReq;
} /* EthTSyn_RateMeas_GetSyncStateAndCorrVals() */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_TimeCorrSuccess
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_TimeCorrSuccess(
                  EthTSyn_RateMeas_IdxType RateMeasIdx,
  ETHTSYN_P2CONST(EthTSyn_TimediffType)    OffsetPtr,
  ETHTSYN_P2CONST(float64)                 RateRatioPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  if (OffsetPtr != NULL_PTR)
  {
    EthTSyn_SetLastCorrectedOffsetNsOfRateMeasDyn(RateMeasIdx, (*OffsetPtr));
  }

  if (RateRatioPtr != NULL_PTR)
  {
    EthTSyn_RateMeas_CorrAllRateMeasForNewRateRatio(RateMeasIdx, (*RateRatioPtr));
  }
} /* EthTSyn_RateMeas_TimeCorrSuccess() */

# endif /* ETHTSYN_NOUNIT_RATEMEAS */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SwitchTimeSynchronization.c
 *********************************************************************************************************************/
