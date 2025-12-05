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
/*!        \file  EthTSyn_Timestamp_Int.h
 *         \unit  Timestamp
 *        \brief  EthTSyn internal header file of the Timestamp unit.
 *      \details  Interface and type definitions of the Timestamp unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_TIMESTAMP_INT_H)
# define ETHTSYN_TIMESTAMP_INT_H

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
 *  EthTSyn_Ts_TsToIntVltChecked
 *********************************************************************************************************************/
/*! \brief         Converts a global time stamp into a virtual local time (internal representation)
 *  \details       Checks if the global time stamp can be represented as virtual local time and optionally converts
 *                 it
 *  \param[in]     TsPtr      The global timestamp
 *  \param[out]    IntVltPtr  The passed global timestamp converted to virtual local time
 *  \return        E_OK - Conversion succeeded
 *  \return        E_NOT_OK - Conversion failed. Global time stamp is too big to be represented as virtual local time
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \spec
 *    requires TsPtr != NULL_PTR;
 *    requires IntVltPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_TsToIntVltChecked(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) TsPtr,
    ETHTSYN_P2VAR(EthTSyn_IntVltType)          IntVltPtr);

/**********************************************************************************************************************
 *  EthTSyn_Ts_TsToVltChecked
 *********************************************************************************************************************/
/*! \brief         Converts a global time stamp into a virtual local time
 *  \details       Checks if the global time stamp can be represented as virtual local time and optionally converts
 *                 it
 *  \param[in]     TsPtr      The global timestamp
 *  \param[out]    VltPtr     The passed global timestamp converted to virtual local time
 *  \return        E_OK - Conversion succeeded
 *  \return        E_NOT_OK - Conversion failed. Global time stamp is too big to be represented as virtual local time
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \spec
 *    requires TsPtr != NULL_PTR;
 *    requires VltPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_TsToVltChecked(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) TsPtr,
    ETHTSYN_P2VAR(StbM_VirtualLocalTimeType)   VltPtr);

/**********************************************************************************************************************
 *  EthTSyn_Ts_VltToIntVlt
 *********************************************************************************************************************/
/*! \brief         Converts a virtual local time into a virtual local time in internal representation.
 *  \details       -
 *  \param[in]     VltPtr  The virtual local time
 *  \return        The passed virtual local time in internal representation
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \spec
 *    requires VltPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(EthTSyn_IntVltType, ETHTSYN_CODE) EthTSyn_Ts_VltToIntVlt(
  ETHTSYN_P2CONST(StbM_VirtualLocalTimeType) VltPtr);

/**********************************************************************************************************************
 *  EthTSyn_Ts_IntVltToVlt
 *********************************************************************************************************************/
/*! \brief         Converts a virtual local time in internal representation into a virtual local time.
 *  \details       -
 *  \param[in]     IntVlt  The virtual local time in internal representation
 *  \return        The converted virtual local time
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *********************************************************************************************************************/
FUNC(StbM_VirtualLocalTimeType, ETHTSYN_CODE) EthTSyn_Ts_IntVltToVlt(
  EthTSyn_IntVltType IntVlt);

/**********************************************************************************************************************
 * EthTSyn_Ts_IntVltMgmtToTsMgmt
 *********************************************************************************************************************/
/*! \brief          Converts a virtual local time in internal representation and its validity information into a global
 *                  timestamp with validity information.
 *  \details        -
 *  \param[in]      IntVltMgmtPtr  The IntVlt with its validity information
 *  \return         The converted IntVlt and its validity information
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires IntVltMgmtPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(EthTSyn_TsMgmtType, ETHTSYN_CODE) EthTSyn_Ts_IntVltMgmtToTsMgmt(
  ETHTSYN_P2CONST(EthTSyn_IntVltMgmtType) IntVltMgmtPtr);

/**********************************************************************************************************************
 * EthTSyn_Ts_TimeDiffToTimeIntDiff
 *********************************************************************************************************************/
/*! \brief          Converts a time difference into the AUTOSAR format (TimeIntDiffType).
 *  \details        -
 *  \param[in]      TimeDiff  Time difference
 *  \return         The converted time difference in AUTOSAR format
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
FUNC(EthTSyn_TimeIntDiffType, ETHTSYN_CODE) EthTSyn_Ts_TimeDiffToTimeIntDiff(
  EthTSyn_TimediffType TimeDiff);

/**********************************************************************************************************************
 * EthTSyn_Ts_IngrTsToGlobalTsTimeSrcNeq
 *********************************************************************************************************************/
/*! \brief          Converts an ingress timestamp into a global timestamp.
 *  \details        Checks input values for plausibility (i.e. conversion of ingress timestamp is possible) and
 *                  optionally converts the timestamp into a global timestamp.
 *  \param[in,out]  IngrTsPtr          In: The ingress timestamp which shall be converted
 *                                    Out: The converted ingress timestamp
 *  \param[in]      RefLocalTimePtr   Local reference time fetched via StbM_BusGetCurrentTime()
 *  \param[in]      RefGlobalTimePtr  Global reference time fetched via StbM_BusGetCurrentTime()
 *  \param[in]      RefStbMVlt        Virtual local time fetched via StbM_GetCurrentVirtualLocalTime()
 *  \param[in]      RefHwTime         HW time fetched via EthIf_GetCurrentTime()
 *  \return         E_OK - Conversion of the ingress timestamp succeeded
 *  \return         E_NOT_OK - Conversion of the ingress timestamp failed
 *  \pre            StbM uses a different time source than the EthTSyn
 *  \pre            Reference times where fetched after frame reception
 *  \pre            RefStbMVlt and RefHwTime where taken 'at the same time' just from different sources
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires IngrTsPtr != NULL_PTR;
 *    requires RefLocalTimePtr != NULL_PTR;
 *    requires RefGlobalTimePtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_IngrTsToGlobalTsTimeSrcNeq(
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) IngrTsPtr,
  ETHTSYN_P2CONST(StbM_VirtualLocalTimeType)   RefLocalTimePtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) RefGlobalTimePtr,
                  EthTSyn_IntVltType           RefStbMVlt,
                  EthTSyn_IntVltType           RefHwTime);

/**********************************************************************************************************************
 * EthTSyn_Ts_IngrTsToGlobalTsTimeSrcEq
 *********************************************************************************************************************/
/*! \brief          Converts an ingress timestamp into a global timestamp.
 *  \details        Checks input values for plausibility (i.e. conversion of ingress timestamp is possible) and
 *                  optionally converts the timestamp into a global timestamp.
 *  \param[in,out]  IngrTsPtr          In: The ingress timestamp which shall be converted
 *                                    Out: The converted ingress timestamp
 *  \param[in]      RefLocalTimePtr   Local reference time fetched via StbM_BusGetCurrentTime()
 *  \param[in]      RefGlobalTimePtr  Global reference time fetched via StbM_BusGetCurrentTime()
 *  \return         E_OK - Conversion of the ingress timestamp succeeded
 *  \return         E_NOT_OK - Conversion of the ingress timestamp failed
 *  \pre            StbM uses the same time source as the EthTSyn
 *  \pre            Reference times where fetched after frame reception
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires IngrTsPtr != NULL_PTR;
 *    requires RefLocalTimePtr != NULL_PTR;
 *    requires RefGlobalTimePtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_IngrTsToGlobalTsTimeSrcEq(
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) IngrTsPtr,
  ETHTSYN_P2CONST(StbM_VirtualLocalTimeType)   RefLocalTimePtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) RefGlobalTimePtr);

/**********************************************************************************************************************
 * EthTSyn_Ts_EgrTsToGlobalTsTimeSrcNeq
 *********************************************************************************************************************/
/*! \brief          Converts an egress timestamp into a global timestamp.
 *  \details        Checks input values for plausibility (i.e. conversion of egress timestamp is possible) and
 *                  optionally converts the timestamp into a global timestamp.
 *  \param[in,out]  EgrTsPtr           In: The egress timestamp which shall be converted
 *                                    Out: The converted egress timestamp
 *  \param[in]      RefLocalTimePtr   Local reference time fetched via StbM_BusGetCurrentTime()
 *  \param[in]      RefGlobalTimePtr  Global reference time fetched via StbM_BusGetCurrentTime()
 *  \param[in]      RefStbMVlt        Virtual local time fetched via StbM_GetCurrentVirtualLocalTime()
 *  \param[in]      RefHwTime         HW time fetched via EthIf_GetCurrentTime()
 *  \return         E_OK - Conversion of the egress timestamp succeeded
 *  \return         E_NOT_OK - Conversion of the egress timestamp failed
 *  \pre            StbM uses a different time source than the EthTSyn
 *  \pre            Reference times where fetched before frame transmission
 *  \pre            RefStbMVlt and RefHwTime where taken 'at the same time' just from different sources
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires EgrTsPtr != NULL_PTR;
 *    requires RefLocalTimePtr != NULL_PTR;
 *    requires RefGlobalTimePtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_EgrTsToGlobalTsTimeSrcNeq(
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) EgrTsPtr,
  ETHTSYN_P2CONST(StbM_VirtualLocalTimeType)   RefLocalTimePtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) RefGlobalTimePtr,
                  EthTSyn_IntVltType           RefStbMVlt,
                  EthTSyn_IntVltType           RefHwTime);

/**********************************************************************************************************************
 * EthTSyn_Ts_EgrTsToGlobalTsTimeSrcEq
 *********************************************************************************************************************/
/*! \brief          Converts an egress timestamp into a global timestamp.
 *  \details        Checks input values for plausibility (i.e. conversion of egress timestamp is possible) and
 *                  optionally converts the timestamp into a global timestamp.
 *  \param[in,out]  EgrTsPtr           In: The egress timestamp which shall be converted
 *                                    Out: The converted egress timestamp
 *  \param[in]      RefLocalTimePtr   Local reference time fetched via StbM_BusGetCurrentTime()
 *  \param[in]      RefGlobalTimePtr  Global reference time fetched via StbM_BusGetCurrentTime()
 *  \return         E_OK - Conversion of the egress timestamp succeeded
 *  \return         E_NOT_OK - Conversion of the egress timestamp failed
 *  \pre            StbM uses the same time source as the EthTSyn
 *  \pre            Reference times where fetched before frame transmission
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires EgrTsPtr != NULL_PTR;
 *    requires RefLocalTimePtr != NULL_PTR;
 *    requires RefGlobalTimePtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_EgrTsToGlobalTsTimeSrcEq(
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) EgrTsPtr,
  ETHTSYN_P2CONST(StbM_VirtualLocalTimeType)   RefLocalTimePtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) RefGlobalTimePtr);

/**********************************************************************************************************************
 *  EthTSyn_Ts_TsMinusTs
 *********************************************************************************************************************/
/*! \brief         Calculates the time difference of two timestamps (Ts1 - Ts2).
 *  \details       -
 *  \param[in]     Ts1Ptr      First timestamp
 *  \param[in]     Ts2Ptr      Second timestamp
 *  \param[out]    TimeDiffPtr Calculated time difference in nanoseconds
 *  \return        E_OK - Calculation was successful
 *  \return        E_NOT_OK - Calculation failed
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \note           Ts1Ptr corresponds to minuend
 *                  Ts2Ptr corresponds to subtrahend
 *  \spec
 *    requires Ts1Ptr != NULL_PTR;
 *    requires Ts2Ptr != NULL_PTR;
 *    requires TimeDiffPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_TsMinusTs(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) Ts1Ptr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) Ts2Ptr,
    ETHTSYN_P2VAR(EthTSyn_TimediffType)        TimeDiffPtr);

/**********************************************************************************************************************
 *  EthTSyn_Ts_TsPlusUTimediff
 *********************************************************************************************************************/
/*! \brief         Adds a positive time difference to a timestamps.
 *  \details       -
 *  \param[in,out] TsPtr       Timestamp
 *  \param[in]     TimeDiff    Time difference
 *  \return        E_OK - Calculation was successful
 *  \return        E_NOT_OK - Calculation failed
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE for different TsPtr
 *  \spec
 *    requires TsPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_TsPlusUTimediff(
  ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) TsPtr,
                EthTSyn_UTimediffType        TimeDiff);

/**********************************************************************************************************************
 * EthTSyn_Ts_SumUTimediffsChecked
 *********************************************************************************************************************/
/*! \brief          Check if sum of all time differences fits in the EthTSyn_UTimediffType and calculates the sum.
 *  \details        -
 *  \param[in]      TimediffValuesPtr  Pointer to all time difference values (array)
 *  \param[in]      TimediffCount      Amount of time difference values in TimediffValuesPtr
 *  \param[out]     TotalTimediffPtr   Sum of all time difference values
 *  \return         E_OK - Successfully calculated the sum of all time difference
 *  \return         E_NOT_OK - Failed to calculate the sum of all time difference
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires TimediffValuesPtr != NULL_PTR;
 *    requires $lengthOf(TimediffValuesPtr) >= TimediffCount;
 *    requires TotalTimediffPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Ts_SumUTimediffsChecked(
  ETHTSYN_P2CONST(EthTSyn_UTimediffType) TimediffValuesPtr,
                  uint8                  TimediffCount,
    ETHTSYN_P2VAR(EthTSyn_UTimediffType) TotalTimediffPtr);

/**********************************************************************************************************************
 * EthTSyn_Ts_ResetTsMgmt
 *********************************************************************************************************************/
/*! \brief          Resets the timestamp management structure
 *  \details        Resets all timestamp value fields to zero and the validity flag to FALSE
 *  \param[out]     TsMgmtPtr  Pointer to the timestamp management struture
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires TsMgmtPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_Ts_ResetTsMgmt(
  ETHTSYN_P2VAR(EthTSyn_TsMgmtType) TsMgmtPtr);

/**********************************************************************************************************************
 * EthTSyn_Ts_IsTsValid
 *********************************************************************************************************************/
/*! \brief          Checks if a timestamp is valid with respect to the allowed value range
 *  \details        -
 *  \param[in]      TsPtr  The timestamp to check
 *  \return         TRUE - Timestamp is valid
 *  \return         FALSE - Timestamp is invalid
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires TsPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_Ts_IsTsValid(
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) TsPtr);

/**********************************************************************************************************************
 * EthTSyn_Ts_IsStbmTsValid
 *********************************************************************************************************************/
/*! \brief          Checks if an StbM timestamp is valid with respect to the allowed value range
 *  \details        -
 *  \param[in]      StbmTsPtr  The timestamp to check
 *  \return         TRUE - Timestamp is valid
 *  \return         FALSE - Timestamp is invalid
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *  \spec
 *    requires StbmTsPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(boolean, ETHTSYN_CODE) EthTSyn_Ts_IsStbmTsValid(
  ETHTSYN_P2CONST(StbM_TimeStampType) StbmTsPtr);

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ETHTSYN_TIMESTAMP_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Timestamp_Int.h
 *********************************************************************************************************************/
