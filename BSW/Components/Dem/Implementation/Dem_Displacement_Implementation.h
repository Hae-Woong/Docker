/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*! \addtogroup Dem_Displacement
 *  \{
 *  \file       Dem_Displacement_Implementation.h
 *  \brief      Basic routines for event memories.
 *  \details    Handle displacement for every kind of entry, this includes:
 *              - Find next displacement candidate
 *              - Check priority of entries
 *  \entity     Displacement
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_DISPLACEMENT_IMPLEMENTATION_H)
#define DEM_DISPLACEMENT_IMPLEMENTATION_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Displacement_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_MemoryEntry_Interface.h"
#include "Dem_ObdFreezeFrameEntry_Interface.h"
#include "Dem_ObdFreezeFrameMemory_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_TimeSeriesMemory_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_DISPLACEMENT_IMPLEMENTATION_FILENAME "Dem_Displacement_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem__PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_StorageManager_Displacement_TestEventTestedForDisplacmentTOC
 *****************************************************************************/
 /*!
 * \brief         Tests whether the event is considered as tested in the 
 *                displacement process.
 *
 * \details       Tests whether the event is considered as tested in the 
 *                displacement process.
 *
 * \param[in]     EventInternalUdsStatus
 *                Internal UDS status of the event 
 *
 * \return        TRUE
 *                Event was tested failed this operation cycle or was
 *                exclusively tested passed (resp. all subevents exclusively
                  passed).
 *                FALSE
 *                Event was not tested this operation cycle (resp. not every
                  subevent was tested).
 * 
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_Displacement_TestEventTestedForDisplacmentTOC(
  Dem_UDSStatus_StatusType EventInternalUdsStatus
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_StorageManager_Displacement_TestEventTestedForDisplacmentTOC
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_StorageManager_Displacement_TestEventTestedForDisplacmentTOC(                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_UDSStatus_StatusType EventInternalUdsStatus
)
{
  return (Dem_Event_UDSStatus_TestBit(EventInternalUdsStatus, Dem_UdsStatus_TNCTOC) == FALSE)
      || (Dem_Event_UDSStatus_TestBit(EventInternalUdsStatus, Dem_UdsStatus_TFTOC) == TRUE);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Displacement_PublicProperties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Displacement_Private Private Methods
 * \{
 */
#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
 /* ****************************************************************************
 * Dem_StorageManager_Displacement_TimeSeriesGetNextCandidate
 *****************************************************************************/
 /*!
 * \brief         Get the next time series entry candidate for the displacement check.
 *
 * \details       Get the next time series entry candidate for the displacement check.
 *
 * \param[out]    CandidateInfo
 *                Buffer that receives the candidate's characteristic data.
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of TimeSeries memory.
 * \param[in]     ChronoIndexIterator
 *                Iterator on the chronology of elements to be displaced.
 *                The iterator must be in range
 *                [1..Dem_FaultMemory_Memories_GetCurrentSize(MemoryInfo)[.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_Displacement_TimeSeriesGetNextCandidate(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo,
  uint16 TranslatedMemoryId,
  CONST(uint8, AUTOMATIC)  ChronoIndexIterator
);
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 * Dem_StorageManager_Displacement_GetNextCandidate
 *****************************************************************************/
/*!
 * \brief         Get the next event entry candidate for the displacement check.
 *
 * \details       Get the next event entry candidate for the displacement check.
 *
 * \param[out]    CandidateInfo
 *                Buffer that receives the candidate's characteristic data.
 * \param[in]     TranslatedMemoryId
 *                Unique Identifier of Event memory
 * \param[in]     ChronoIndexIterator
 *                Iterator on the chronology of elements to be displaced.
 *                The iterator must be in range
 *                [1..Dem_FaultMemory_Memories_GetCurrentSize(MemoryInfo)[.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_Displacement_GetNextCandidate(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo,
  uint16 TranslatedMemoryId,
  CONST(uint8, AUTOMATIC)  ChronoIndexIterator
  );
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 * Dem_StorageManager_Displacement_UpdateBestMatch
 *****************************************************************************/
/*!
 * \brief         Update the best match info by the candidate info.
 *
 * \details       Update the best match info by the candidate info.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: The best match updated with the relevant candidate info.
 * \param[in]     CandidateInfo
 *                The candidate info.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_Displacement_UpdateBestMatch(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 * Dem_StorageManager_Displacement_CheckInvalidEntry
 *****************************************************************************/
/*!
 * \brief         Check if candidate is an invalid entry.
 *
 * \details       Check if candidate is the invalid entry. If so, it is the
 *                best match.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_StorageManager_Displace_NeedMoreTests
 *                Candidate may be a match, but this check can't decide that it
 *                is the 'best match', so this candidate shall undergo further
 *                checks. 'BestMatchInfo' is unmodified.
 * \return        Dem_StorageManager_Displace_FoundBestMatch
 *                Candidate is the best match for sure. 'BestMatchInfo'
 *                is updated.
 *                No further scanning for better matches is required.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckInvalidEntry(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON))
/* ****************************************************************************
 * Dem_StorageManager_Displacement_CheckAgedCandidate
 *****************************************************************************/
/*!
 * \brief         Check if candidate is aged.
 *
 * \details       Check if candidate is aged, so can be displaced immediately.
 *                If so, it is the best match.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_StorageManager_Displace_NeedMoreTests
 *                Candidate may be a match, but this check can't decide that it
 *                is the 'best match', so this candidate shall undergo further
 *                checks. 'BestMatchInfo' is unmodified.
 * \return        Dem_StorageManager_Displace_FoundBestMatch
 *                Candidate is the best match for sure. 'BestMatchInfo'
 *                is updated.
 *                No further scanning for better matches is required.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *                && (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckAgedCandidate(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON))
/* ****************************************************************************
 * Dem_StorageManager_Displacement_CheckHasAgingCounter
 *****************************************************************************/
/*!
 * \brief         Check if candidate has an exclusive aging counter.
 *
 * \details       Check if candidate has an exclusive aging counter, so can be
 *                displaced immediately. If so, it is the best match.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_StorageManager_Displace_NeedMoreTests
 *                Candidate may be a match, but this check can't decide that it
 *                is the 'best match', so this candidate shall undergo further
 *                checks. 'BestMatchInfo' is unmodified.
 * \return        Dem_StorageManager_Displace_FoundBestMatch
 *                Candidate is the best match for sure. 'BestMatchInfo'
 *                is updated.
 *                No further scanning for better matches is required.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *                && (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckHasAgingCounter(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 * Dem_StorageManager_Displacement_CheckPriority
 *****************************************************************************/
/*!
 * \brief         Check if candidate's priority qualifies the displacement.
 *
 * \details       Check if candidate has an event priority, that qualifies it
 *                or disqualifies it for the displacement.
 *                Otherwise it needs further tests.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_StorageManager_Displace_NeedMoreTests
 *                This check can't decide if candidate is a match, so this
 *                candidate shall undergo further checks.
 *                'BestMatchInfo' is unmodified.
 * \return        Dem_StorageManager_Displace_RatingDone
 *                Rating complete - continue the scan with next candidate.
 *                If candidate is no match, 'BestMatchInfo' is unmodified.
 *                If candidate is a match, 'BestMatchInfo' is updated.
 * \return        Dem_StorageManager_Displace_FoundBestMatch
 *                Candidate is the best match for sure.
 *                'BestMatchInfo' is updated.
 *                No further scanning for better matches is required.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckPriority(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) )
/* ****************************************************************************
 * Dem_StorageManager_Displacement_CheckObd
 *****************************************************************************/
/*!
 * \brief         Check if candidate's status qualifies the displacement.
 *
 * \details       Check if candidate is passive, and this qualifies it
 *                for the displacement.
 *                Otherwise it needs further tests.
 *                BestMatchInfo's OldestDisplaceableEntryIndex value may be set 
 *                if it is currently undefined.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: OldestDisplaceableEntryIndex value may be set if it is
 *                      currently undefined. Other values are unmodified.
 * \param[in]     CandidateInfo
 *                The candidate to be checked.
 *
 * \return        Dem_StorageManager_Displace_NeedMoreTests
 *                This check can't decide if candidate is a match, so this
 *                candidate shall undergo further checks.
 * \return        Dem_StorageManager_Displace_RatingDone
 *                Rating complete - continue the scan with next candidate.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF &&
 *                DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckObd(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif

#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) )
/* ****************************************************************************
 * Dem_StorageManager_Displacement_CheckTimeSeries
 *****************************************************************************/
/*!
 * \brief         Check if the readout status of the time series entry candidate
 *                qualifies the displacement.
 *
 * \details       Check if candidate has ongoing readout.
 *                If not, it can be tested further.
 *                BestMatchInfo's OldestDisplaceableEntryIndex value may be set 
 *                if it is currently undefined.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: OldestDisplaceableEntryIndex value may be set if it is
 *                      currently undefined. Other values are unmodified.
 * \param[in]     CandidateInfo
 *                The candidate to be checked.
 *
 * \return        Dem_StorageManager_Displace_NeedMoreTests
 *                This check can't decide if candidate is a match, so this
 *                candidate shall undergo further checks.
 * \return        Dem_StorageManager_Displace_RatingDone
 *                Rating complete - continue the scan with next candidate.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF &&
 *                DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON &&
 *                DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckTimeSeries(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_DISPLACEMENT_PREFER_PASSIVE == STD_ON))
/* ****************************************************************************
 * Dem_StorageManager_Displacement_CheckPassiveState
 *****************************************************************************/
/*!
 * \brief         Check if candidate's status (TestFailed bit) qualifies the
 *                displacement.
 *
 * \details       Check if candidate is passive, and this qualifies it
 *                for the displacement.
 *                Otherwise it needs further tests.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_StorageManager_Displace_NeedMoreTests
 *                This check can't decide if candidate is a match, so this
 *                candidate shall undergo further checks.
 *                'BestMatchInfo' is unmodified.
 * \return        Dem_StorageManager_Displace_RatingDone
 *                Rating complete - continue the scan with next candidate.
 *                If candidate is no match, 'BestMatchInfo' is unmodified.
 *                If candidate is a match, 'BestMatchInfo' is updated.
 * \return        Dem_StorageManager_Displace_FoundBestMatch
 *                Candidate is the best match for sure.
 *                'BestMatchInfo' is updated.
 *                No further scanning for better matches is required.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *                && (DEM_CFG_DISPLACEMENT_PREFER_PASSIVE == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckPassiveState(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif


#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_DISPLACEMENT_TNCTOC_IS_PASSIVE == STD_ON))
/* ****************************************************************************
 * Dem_StorageManager_Displacement_CheckReadinessState
 *****************************************************************************/
/*!
 * \brief         Check if candidate's status (TNCTOC bit) qualifies the
 *                displacement.
 *
 * \details       Check if candidate's readiness (TNCTOC bit) is a better match
 *                than the current 'best match' for the displacement.
 *                Otherwise it needs further tests.
 *
 * \param[in,out] BestMatchInfo
 *                IN: The current best match for displacement.
 *                OUT: Either the candidate (if better match) or the previous
 *                value.
 * \param[in]     CandidateInfo
 *                The candidate to be checked against the current best match.
 *
 * \return        Dem_StorageManager_Displace_NeedMoreTests
 *                This check can't decide if candidate is a match, so this
 *                candidate shall undergo further checks.
 *                'BestMatchInfo' is unmodified.
 * \return        Dem_StorageManager_Displace_RatingDone
 *                Rating complete - continue the scan with next candidate.
 *                If candidate is no match, 'BestMatchInfo' is unmodified.
 *                If candidate is a match, 'BestMatchInfo' is updated.
 *
 * \pre           -
 * \config        (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
 *                && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
 *                && (DEM_CFG_DISPLACEMENT_TNCTOC_IS_PASSIVE == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckReadinessState(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  );
#endif

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
% Dem_StorageManager_Displacement_TimeSeriesGetNextCandidate
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_Displacement_TimeSeriesGetNextCandidate(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo,
  uint16 TranslatedMemoryId,
  CONST(uint8, AUTOMATIC)  ChronoIndexIterator
)
{
  CandidateInfo->EntryIndex = Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, ChronoIndexIterator);                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

  CandidateInfo->EventId = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(CandidateInfo->EntryIndex)->EventId;              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON) || (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  {
    CandidateInfo->EntryIndexAging = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(CandidateInfo->EventId, FALSE); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
# endif
  CandidateInfo->EventInternalUdsStatus = Dem_Core_Event_GetInternalUdsStatus(CandidateInfo->EventId);                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
  {
    CandidateInfo->OldestDisplaceableEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
# endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 % Dem_StorageManager_Displacement_GetNextCandidate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_Displacement_GetNextCandidate(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo,
  uint16 TranslatedMemoryId,
  CONST(uint8, AUTOMATIC)  ChronoIndexIterator
  )
{
  CandidateInfo->EntryIndex = Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, ChronoIndexIterator);                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON) || (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  CandidateInfo->EntryIndexAging = CandidateInfo->EntryIndex;                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# endif
  CandidateInfo->EventId = Dem_MemoryEntry_EventEntry_GetEventId(CandidateInfo->EntryIndex);                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  CandidateInfo->EventInternalUdsStatus = Dem_Core_Event_GetInternalUdsStatus(CandidateInfo->EventId);                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  CandidateInfo->OldestDisplaceableEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 % Dem_StorageManager_Displacement_UpdateBestMatch
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_StorageManager_Displacement_UpdateBestMatch(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
)
{
# if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
  {
    Dem_Cfg_EntryIndexType lOldestDisplaceableEntryIndex;
    lOldestDisplaceableEntryIndex = BestMatchInfo->OldestDisplaceableEntryIndex;
    *BestMatchInfo = *CandidateInfo;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    BestMatchInfo->OldestDisplaceableEntryIndex = lOldestDisplaceableEntryIndex;                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  else
# endif
  {
    *BestMatchInfo = *CandidateInfo;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 % Dem_StorageManager_Displacement_CheckInvalidEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckInvalidEntry(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_StorageManager_Displace_CheckResultType lCheckResult;
  lCheckResult = Dem_StorageManager_Displace_NeedMoreTests;

  if (Dem_Event_TestValidHandle(CandidateInfo->EventId) == FALSE)
  {
    /* always replace an invalid entry */
    Dem_StorageManager_Displacement_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    lCheckResult = Dem_StorageManager_Displace_FoundBestMatch;
  }

  return lCheckResult;
}
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON))
/* ****************************************************************************
 % Dem_StorageManager_Displacement_CheckAgedCandidate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckAgedCandidate(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_StorageManager_Displace_CheckResultType lCheckResult;
  lCheckResult = Dem_StorageManager_Displace_NeedMoreTests;

  if (
# if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
      (CandidateInfo->EntryIndexAging != DEM_CFG_ENTRYINDEX_INVALID) &&
# endif
      (Dem_Core_Aging_GetAgingStatus(CandidateInfo->EventId, CandidateInfo->EntryIndexAging) == Dem_AgingStatus_Aged))           /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
  { /* aged event can be displaced immediately */
    Dem_StorageManager_Displacement_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    lCheckResult = Dem_StorageManager_Displace_FoundBestMatch;
  }

  return lCheckResult;
}
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON))
/* ****************************************************************************
 % Dem_StorageManager_Displacement_CheckHasAgingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckHasAgingCounter(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_StorageManager_Displace_CheckResultType lCheckResult;
  lCheckResult = Dem_StorageManager_Displace_NeedMoreTests;

  if (
#  if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
      (CandidateInfo->EntryIndexAging != DEM_CFG_ENTRYINDEX_INVALID) &&
#  endif
      (DEM_EVENTENTRY_TEST_AGING_ONLY(                                                                                           /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
         Dem_MemoryEntry_EventEntry_GetState(
           CandidateInfo->EntryIndexAging
      )) == TRUE))
  { /* aging counter can be displaced immediately */
    Dem_StorageManager_Displacement_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    lCheckResult = Dem_StorageManager_Displace_FoundBestMatch;
  }

  return lCheckResult;
}
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON))
/* ****************************************************************************
 % Dem_StorageManager_Displacement_CheckPriority
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckPriority(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_StorageManager_Displace_CheckResultType lCheckResult;

  if (Dem_Cfg_EventPriority(CandidateInfo->EventId) < Dem_Cfg_EventPriority(BestMatchInfo->EventId))
  {
    /* Never displace events with higher priority */
    lCheckResult = Dem_StorageManager_Displace_RatingDone;
  }
  else
  if (Dem_Cfg_EventPriority(CandidateInfo->EventId) > Dem_Cfg_EventPriority(BestMatchInfo->EventId))
  {
    /* always prefer an event with lowest priority */
    Dem_StorageManager_Displacement_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
# if (DEM_FEATURE_NEED_DISPLACEMENT_FIRST_MATCH == STD_ON)
    /* If configured always use first match */
    lCheckResult = Dem_StorageManager_Displace_FoundBestMatch;
# else
    lCheckResult = Dem_StorageManager_Displace_RatingDone;
# endif
  }
  else
  {
    /* No candidate identified */
    lCheckResult = Dem_StorageManager_Displace_NeedMoreTests;
  }

  return lCheckResult;
}
#endif

#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) )
/* ****************************************************************************
 % Dem_StorageManager_Displacement_CheckObd
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckObd(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,                                                     /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_StorageManager_Displace_CheckResultType lCheckResult;

  /* Can be unused depending on configuration */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(BestMatchInfo)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CandidateInfo)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */

  /* Skip OBD displacement algorithms*/
  lCheckResult = Dem_StorageManager_Displace_NeedMoreTests;

# if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
  if ((Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE))
  {
    /* Ignore stored events which are emission related
       AND (having stored the OBD FreezeFrame  OR  are pending  OR  requesting the MIL) */
#  if (DEM_CFG_SUPPORT_OBDII == STD_ON)
    if ((Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
      && (Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 != Dem_Memories_ObdFreezeFrameMemory_GetSize())
      && (Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02) == CandidateInfo->EventId)) /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      /* stored event id is visible in Mode02 -> skip */
      lCheckResult = Dem_StorageManager_Displace_RatingDone;
    }
    else
#  endif
    if ((Dem_Core_DTC_IsObdRelated(CandidateInfo->EventId) == TRUE)
      && (Dem_Event_UDSStatus_TestBit(CandidateInfo->EventInternalUdsStatus, Dem_UdsStatus_PDTC) == TRUE))
    {
      lCheckResult = Dem_StorageManager_Displace_RatingDone;
    }
    else
    if ((Dem_GlobalDiagnostics_TestEventHasMilAssigned(CandidateInfo->EventId) == TRUE)
      && (Dem_Event_UDSStatus_TestBit(CandidateInfo->EventInternalUdsStatus, Dem_UdsStatus_WIR) == TRUE))
    {
      lCheckResult = Dem_StorageManager_Displace_RatingDone;
    }
#  if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
    else
    if (BestMatchInfo->OldestDisplaceableEntryIndex == DEM_CFG_ENTRYINDEX_INVALID)
    {
      /* no oldest displaceable index identified yet */
      BestMatchInfo->OldestDisplaceableEntryIndex = CandidateInfo->EntryIndex;                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      lCheckResult = Dem_StorageManager_Displace_NeedMoreTests;
    }
#  endif
    else
    {
      /* MISRA case: lCheckResult is already initialized */
    }
  }
# endif

  return lCheckResult;
}                                                                                                                                /* PRQA S 6080, 6050 */ /* MD_MSR_STMIF, MD_MSR_STCAL */
#endif

#if ( (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON))
/* ****************************************************************************
 % Dem_StorageManager_Displacement_CheckTimeSeries
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckTimeSeries(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,                                                     /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_StorageManager_Displace_CheckResultType lCheckResult;

  /* Can be unused depending on configuration */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(BestMatchInfo)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CandidateInfo)                                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */

  /* Default: Candidate is not classified yet and further test needed on this candidate */
  lCheckResult = Dem_StorageManager_Displace_NeedMoreTests;

  /* Check: Candidate time series entry can not be displaced due to ongoing readout */
  if (Dem_MemoryEntry_TimerSeriesEntry_TestDisplaceable(CandidateInfo->EntryIndex) == FALSE)
  {
    lCheckResult = Dem_StorageManager_Displace_RatingDone;
  }
# if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  else if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == FALSE)
  {
    if (BestMatchInfo->OldestDisplaceableEntryIndex == DEM_CFG_ENTRYINDEX_INVALID)
    {
      /* no oldest displaceable index identified yet */
      BestMatchInfo->OldestDisplaceableEntryIndex = CandidateInfo->EntryIndex;                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
# endif
  else
  {
    /* MISRA case: lCheckResult is already initialized */
  }

  return lCheckResult;
}                                                                                                                                /* PRQA S 6080, 6050 */ /* MD_MSR_STMIF, MD_MSR_STCAL */
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_DISPLACEMENT_PREFER_PASSIVE == STD_ON))
/* ****************************************************************************
 % Dem_StorageManager_Displacement_CheckPassiveState
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckPassiveState(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  Dem_StorageManager_Displace_CheckResultType lCheckResult;

# if (DEM_FEATURE_NEED_DISPLACEMENT_FIRST_MATCH == STD_OFF)
  if (Dem_Event_UDSStatus_TestBit(BestMatchInfo->EventInternalUdsStatus, Dem_UdsStatus_TF) == FALSE)
  {
    /* If current best candidate is passive, it is a better match as it is older */
    lCheckResult = Dem_StorageManager_Displace_RatingDone;
  }
  else
# endif
  if (Dem_Event_UDSStatus_TestBit(CandidateInfo->EventInternalUdsStatus, Dem_UdsStatus_TF) == FALSE)
  {
    /* did not find a passive event before, so candidate is a better match */
    Dem_StorageManager_Displacement_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
# if (DEM_FEATURE_NEED_DISPLACEMENT_FIRST_MATCH == STD_ON)
    /* If configured always use first match */
    lCheckResult = Dem_StorageManager_Displace_FoundBestMatch;
# else
    lCheckResult = Dem_StorageManager_Displace_RatingDone;
# endif
  }
  else
  {
    /* no candidate identified */
    lCheckResult = Dem_StorageManager_Displace_NeedMoreTests;
  }

  return lCheckResult;
}
#endif

#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON) \
  && (DEM_CFG_DISPLACEMENT_TNCTOC_IS_PASSIVE == STD_ON))
/* ****************************************************************************
 % Dem_StorageManager_Displacement_CheckReadinessState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_StorageManager_Displace_CheckResultType, DEM_CODE)
Dem_StorageManager_Displacement_CheckReadinessState(
  CONSTP2VAR(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) BestMatchInfo,
  CONSTP2CONST(Dem_Displacement_InfoType, AUTOMATIC, AUTOMATIC) CandidateInfo
  )
{
  /* No candidate identified */
  Dem_StorageManager_Displace_CheckResultType lCheckResult = Dem_StorageManager_Displace_NeedMoreTests;

  if ( (Dem_StorageManager_Displacement_TestEventTestedForDisplacmentTOC(BestMatchInfo->EventInternalUdsStatus) == TRUE)
    && (Dem_StorageManager_Displacement_TestEventTestedForDisplacmentTOC(CandidateInfo->EventInternalUdsStatus) == FALSE))
  {
    Dem_StorageManager_Displacement_UpdateBestMatch(BestMatchInfo, CandidateInfo);                                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
# if (DEM_FEATURE_NEED_DISPLACEMENT_FIRST_MATCH == STD_ON)
# error "Not supported, if first match displacement is enabled according DPRS"
# else
    lCheckResult = Dem_StorageManager_Displace_RatingDone;
# endif
  }
  return lCheckResult;
}
#endif


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Displacement_Public
 * \{
 */

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
/* ****************************************************************************
 % Dem_StorageManager_Displacement_SelectDisplacedIndex
 *****************************************************************************/
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
DEM_LOCAL FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_StorageManager_Displacement_SelectDisplacedIndex(
  uint16 TranslatedMemoryId,
  Dem_EventIdType DisplacingEventId
)
{
# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
  Dem_Displacement_InfoType lBestMatchInfo;
  Dem_Displacement_InfoType lCandidateInfo;
  Dem_StorageManager_Displace_CheckResultType lCheckResult;
  uint8 lChronoIndexIterator;

  /* Initialize Best Candidate to 'displacement not possible' */
  lBestMatchInfo.EventId = DisplacingEventId;
  lBestMatchInfo.EntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
  lBestMatchInfo.EventInternalUdsStatus = DEM_UDS_STATUS_TF;
#  if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  lBestMatchInfo.OldestDisplaceableEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
#  endif
#  if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON) || (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  lBestMatchInfo.EntryIndexAging = DEM_CFG_ENTRYINDEX_INVALID;
#  endif

  /* find lowest priority DTC in memory. scan in chronological order, so the
   * first found entry is the older one (to break ties) */
  for (lChronoIndexIterator = 0u;                                                                                                /* PRQA S 0771 */ /* MD_DEM_15.4_opt */
    lChronoIndexIterator < Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId);
    ++lChronoIndexIterator)
  {
    /* Get next Candidate to be checked */
#  if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
    if (Dem_Memories_TimeSeriesMemory_IsReferredByMemoryId(TranslatedMemoryId))
    {
      Dem_StorageManager_Displacement_TimeSeriesGetNextCandidate(&lCandidateInfo, TranslatedMemoryId, lChronoIndexIterator);     /* SBSW_DEM_POINTER_FORWARD_STACK */
      /* Check: Candidate time series entry can not be displaced due to ongoing readout */
      if (Dem_StorageManager_Displacement_CheckTimeSeries(&lBestMatchInfo, &lCandidateInfo) == Dem_StorageManager_Displace_RatingDone) /* SBSW_DEM_POINTER_FORWARD_STACK */
      {
        continue;
      }
    }
    else
#  endif
    {
       Dem_StorageManager_Displacement_GetNextCandidate(&lCandidateInfo, TranslatedMemoryId, lChronoIndexIterator);              /* SBSW_DEM_POINTER_FORWARD_STACK */
    }
    /* Check: Candidate is invalid entry */
    if (Dem_StorageManager_Displacement_CheckInvalidEntry(&lBestMatchInfo, &lCandidateInfo) == Dem_StorageManager_Displace_FoundBestMatch) /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      /* always replace an invalid entry */
      break;
    }

    /* Check: Candidate is aged */
#  if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
    if (Dem_StorageManager_Displacement_CheckAgedCandidate(&lBestMatchInfo, &lCandidateInfo) == Dem_StorageManager_Displace_FoundBestMatch) /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      /* aged event can be displaced immediately */
      break;
    }
#  endif /* (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON) */

    /* Check: Candidate only holds an exclusive aging counter */
#  if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
    if (Dem_StorageManager_Displacement_CheckHasAgingCounter(&lBestMatchInfo, &lCandidateInfo) == Dem_StorageManager_Displace_FoundBestMatch) /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      /* aging counter can be displaced immediately */
      break;
    }
#  endif /* (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON) */

    /* Check: event priorities */
    lCheckResult = Dem_StorageManager_Displacement_CheckPriority(&lBestMatchInfo, &lCandidateInfo);                              /* SBSW_DEM_POINTER_FORWARD_STACK */
    if (lCheckResult == Dem_StorageManager_Displace_RatingDone)
    {
#  if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
      (void)Dem_StorageManager_Displacement_CheckObd(&lBestMatchInfo, &lCandidateInfo);                                          /* SBSW_DEM_POINTER_FORWARD_STACK */
#  endif
      continue;                                                                                                                  
    }
    else if (lCheckResult == Dem_StorageManager_Displace_FoundBestMatch)
    {
      break;
    }
    else
    {
      /* check next category */
    }

    /* Check: OBD enabled */
    if (Dem_StorageManager_Displacement_CheckObd(&lBestMatchInfo, &lCandidateInfo) == Dem_StorageManager_Displace_RatingDone)    /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      continue;                                                                                                                  
    }

    /* Check: consider passive for displacement */
#  if (DEM_CFG_DISPLACEMENT_PREFER_PASSIVE == STD_ON)
    lCheckResult = Dem_StorageManager_Displacement_CheckPassiveState(&lBestMatchInfo, &lCandidateInfo);                          /* SBSW_DEM_POINTER_FORWARD_STACK */
    if (lCheckResult == Dem_StorageManager_Displace_RatingDone)
    {
      continue;
    }
    else if (lCheckResult == Dem_StorageManager_Displace_FoundBestMatch)
    {
      break;
    }
    else
    {
      /* check next category */
    }
#  endif /* DEM_CFG_DISPLACEMENT_PREFER_PASSIVE == STD_ON */

    /* Check: consider readiness for displacement */
#  if (DEM_CFG_DISPLACEMENT_TNCTOC_IS_PASSIVE == STD_ON)
    if (Dem_StorageManager_Displacement_CheckReadinessState(&lBestMatchInfo, &lCandidateInfo) == Dem_StorageManager_Displace_RatingDone) /* SBSW_DEM_POINTER_FORWARD_STACK */
    {
      continue;                                                                                                                  
    }
#  endif /* (DEM_CFG_DISPLACEMENT_TNCTOC_IS_PASSIVE == STD_ON) */
  }

  /* When no match found - use fallback if configured */
#  if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  if (lBestMatchInfo.EventId == DisplacingEventId)
  {
    /* The oldest event can be always displaced (could be DEM_CFG_ENTRYINDEX_INVALID!) */
    if ((Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
#   if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
      || Dem_Memories_TimeSeriesMemory_IsReferredByMemoryId(TranslatedMemoryId)
#   endif
      )
    {
      lBestMatchInfo.EntryIndex = lBestMatchInfo.OldestDisplaceableEntryIndex;
    }
    else
    {
      lBestMatchInfo.EntryIndex = Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, 0);
    }
  }
#  endif /* (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON) */

  return lBestMatchInfo.EntryIndex;
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TranslatedMemoryId)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DisplacingEventId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return DEM_CFG_ENTRYINDEX_INVALID; /* Always failed */
# endif

}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080  */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)*/

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/* ****************************************************************************
 % Dem_StorageManager_Displacement_SelectCustomizedDisplacedIndex
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_StorageManager_Displacement_SelectCustomizedDisplacedIndex(
  uint16 TranslatedMemoryId,
  Dem_EventIdType DisplacingEventId                                                                                              /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  Dem_Cfg_EntryIndexType lDisplacedIndex;
  uint8 lChronoIndexIterator;

# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
  Dem_Cfg_EntryIndexType lLowestPrioIndex;
  uint8 lLowestPrio;
# endif

  /* Can be unused depending on configuration */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DisplacingEventId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */

# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
  lLowestPrio = Dem_Cfg_EventPriority(DisplacingEventId);
  lLowestPrioIndex = DEM_CFG_ENTRYINDEX_INVALID;
# endif

  lDisplacedIndex = DEM_CFG_ENTRYINDEX_INVALID;

  /*
   * find oldest DTC in Memory (by scanning elements in chronological order, starting with oldest),
   * that has a lower priority (=higher number) than the new DTC
   */

  /* Start with index of the oldest element (0) of the stack */
  for (lChronoIndexIterator = 0u;
       lChronoIndexIterator < Dem_FaultMemory_Memories_GetMaxSize(TranslatedMemoryId);
       ++lChronoIndexIterator)
  {
    Dem_Cfg_EntryIndexType lEventEntryIndex;
# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
    uint8 lStoredPriority;
# endif

    lEventEntryIndex = Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, lChronoIndexIterator);
# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
    lStoredPriority = Dem_Cfg_EventPriority(Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex));
# endif
    /* Event cannot still be aged and reach here since an event would cross FDC threshold before failing */
    if (DEM_DTC_TEST_PC_STATUS_ADTC(Dem_MemoryEntry_EventEntry_GetPreConfirmedStatus(lEventEntryIndex)) == TRUE)
    {
      /* aged event can be displaced immediately */
      lDisplacedIndex = lEventEntryIndex;
      break;
    }
# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
    else
    if (lStoredPriority > lLowestPrio)
    {
      /* Found oldest event entry with lower priority - remember in case there still is an aged event */
      lLowestPrioIndex = lEventEntryIndex;
      lLowestPrio = lStoredPriority;
    }
# endif
    else
    {
      /* Not a candidate for displacement */
    }
  }

# if (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
  if (lDisplacedIndex == DEM_CFG_ENTRYINDEX_INVALID)
  { /* no aged event detected */
    if (lLowestPrioIndex != DEM_CFG_ENTRYINDEX_INVALID)
    { /* No aged event, but an old, lower prioritized event was found */
      lDisplacedIndex = lLowestPrioIndex;
    }
  }
# endif

  return lDisplacedIndex;
}
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMACCESS_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Displacement_Implementation.h
 *********************************************************************************************************************/
