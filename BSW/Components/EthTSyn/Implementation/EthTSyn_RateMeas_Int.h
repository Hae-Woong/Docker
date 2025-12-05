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
/*!        \file  EthTSyn_RateMeas_Int.h
 *         \unit  RateMeas
 *        \brief  EthTSyn internal header file of the RateMeas unit.
 *      \details  Interface and type definitions of the RateMeas unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_RATE_MEAS_INT_H)
# define ETHTSYN_RATE_MEAS_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
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
#  define ETHTSYN_START_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_Init
 *********************************************************************************************************************/
/*! \brief         Initializes the RateMeas unit.
 *  \details       -
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_Reset
 *********************************************************************************************************************/
/*! \brief         Resets the rate measurements of one RateMeas instance.
 *  \details       -
 *  \param[in]     RateMeasIdx Index of the RateMeas instance
 *                             [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_Reset(
  EthTSyn_RateMeas_IdxType RateMeasIdx);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_GetSyncStateAndCorrVals
 *********************************************************************************************************************/
/*! \brief         Calculates the new synchronization state. Determines whether offset and/or rate ratio time
 *                 correction shall be applied and calculates the respective values.
 *  \details       -
 *  \param[in]     RateMeasIdx  Index of the RateMeas instance
 *                              [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in]     TsInfoPtr    Timestamps and sequence id required for time synchronization
 *  \param[in,out] SyncStatePtr - In: current synchronization state
 *                              - Out: new synchronization state
 *  \param[out]    OffsetPtr    Offset time correction value (value only valid if offset correction shall be applied)
 *  \param[out]    RateRatioPtr Rate ratio time correction value (value only valid if rate ratio correction shall be
 *                              applied)
 *  \return        Information whether offset and/or rate ratio time correction shall be applied
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
FUNC(EthTSyn_TimeCorrRequiredType, ETHTSYN_CODE) EthTSyn_RateMeas_GetSyncStateAndCorrVals(
  EthTSyn_RateMeas_IdxType                                     RateMeasIdx,
  ETHTSYN_P2CONST(EthTSyn_SwtTimeSync_TimestampInfoStructType) TsInfoPtr,
    ETHTSYN_P2VAR(EthTSyn_SyncStateType)                       SyncStatePtr,
    ETHTSYN_P2VAR(EthTSyn_TimediffType)                        OffsetPtr,
    ETHTSYN_P2VAR(float64)                                     RateRatioPtr);

/**********************************************************************************************************************
 *  EthTSyn_RateMeas_TimeCorrSuccess
 *********************************************************************************************************************/
/*! \brief         Reports information about a successful time correction.
 *  \details       -
 *  \param[in]     RateMeasIdx      Index of the RateMeas instance
 *                                  [range: RateMeasIdx < EthTSyn_GetSizeOfRateMeas()]
 *  \param[in]     OffsetPtr        Offset which was used for time correction or NULL_PTR if offset was not used for
 *                                  time correction.
 *  \param[in]     RateRatioPtr     Rate ratio which was used for time correction or NULL_PTR if rate ratio was not
 *                                  used for time correction.
 *  \pre           Only call if EthTSyn_RateMeas_GetSyncStateAndCorrVals() was called before.
 *  \pre           Only call if EthIf_SwitchSetCorrectionTime() returned E_OK.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires RateMeasIdx < EthTSyn_GetSizeOfRateMeas();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_RateMeas_TimeCorrSuccess(
                  EthTSyn_RateMeas_IdxType RateMeasIdx,
  ETHTSYN_P2CONST(EthTSyn_TimediffType)    OffsetPtr,
  ETHTSYN_P2CONST(float64)                 RateRatioPtr);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) */
#endif /* ETHTSYN_RATE_MEAS_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_RateMeas_Int.h
 *********************************************************************************************************************/
