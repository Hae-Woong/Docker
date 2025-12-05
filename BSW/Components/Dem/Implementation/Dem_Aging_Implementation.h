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
/*! \ingroup    Dem_Master | Dem_Satellite
 *  \defgroup   Dem_Aging Aging
 *  \addtogroup Dem_Aging
 *  \{
 *  \file       Dem_Aging_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    The purpose is to manage the aging of events in the Dem. It takes care of starting, continuing,
 *              ending and interrupting the aging process. Internally, the aging target cycle count is managed.
 *  \unit       Aging
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_AGING_IMPLEMENTATION_H_)
#define DEM_AGING_IMPLEMENTATION_H_

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

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_Aging_Interface.h"
/* ------------------------------------------------------------------------- */
#include "Dem_DTC_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_OperationCycle_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Event_Interface.h"


/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

#define DEM_AGING_IMPLEMENTATION_FILENAME "Dem_Aging_Implementation.h"

/*! Aging timer 200 hour threshold value in minutes */
#define DEM_AGING_AGINGTIMER_MINUTES_200h                   (12000U)

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Aging_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Core_Aging_SetAgingTargetCycle
 *****************************************************************************/
/*!
 * \brief         Sets the aging counter for the event.
 *
 * \details       Sets the aging counter for the event. Depending on the configuration,
 *                the global aging counter or the event entry specific aging counter
 *                is set.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *                Even if an event entry exists the entry independent aging is
 *                preferred if supported. (EventId always needed)
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Should be set to INVALID in case the function is called in a
 *                context in which always memory independent aging is requested.
 * \param[in]     AgingTargetCycle
 *                The cycle count in which the aging will be complete.
 *
 * \pre           If aging for all events is not enabled, the event must have
 *                an event entry.
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Core_Aging_SetAgingTargetCycle(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  uint16 AgingTargetCycle
  );


/* ****************************************************************************
 * Dem_Core_Aging_SetEntryIndependentAgingTargetCycle
 *****************************************************************************/
/*!
 * \brief         Stored the event's aging target cycle inside the dedicated aging
 *                counter array.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \param[in]     AgingTargetCycle
 *                Target aging cycle to store.
 *
 * \pre           -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_SetEntryIndependentAgingTargetCycle(
  Dem_EventIdType EventId,
  uint16 AgingTargetCycle
  );

/* ****************************************************************************
 * Dem_Core_Aging_GetEntryIndependentAgingTargetCycle
 *****************************************************************************/
/*!
 * \brief         Retrieve the event's aging target cycle stored in the dedicated
 *                aging counter array.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The aging target cycle of the event.
 *
 * \pre           -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Core_Aging_GetEntryIndependentAgingTargetCycle(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Core_Aging_InvalidateEntryIndependentAgingTargetCycle
 *****************************************************************************/
/*!
 * \brief         Tests and invalidates aging counter of the event.
 *
 * \details       Tests and invalidates aging counter of the event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Core_Aging_InvalidateEntryIndependentAgingTargetCycle(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Core_Aging_TestEntryIndependentAgingSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether aging can start withour an event entry.
 * 
 * \details       Tests whether aging can start without an event entry.
 *                (AgingCounter is not stored in event entry)
 * 
 * \return        TRUE
 *                Aging without event entry is supported.
 * \return        FALSE
 *                Aging without event entry is not supported.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestEntryIndependentAgingSupported(
  void
);

/* ****************************************************************************
 * Dem_Core_Aging_TestImmediateAgingStartOnPassedSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether aging can start immediately after a passed
 *                report.
 * 
 * \details       Tests whether aging can start immediately after a passed
 *                report. (Additional starting conditions might apply)
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * 
 * \return        TRUE
 *                Aging start after passed report is supported.
 * \return        FALSE
 *                Aging start after passed report is not supported.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestImmediateAgingStartOnPassedSupported(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Aging_TestStartNeedsFailureFreeCycle
 *****************************************************************************/
/*!
 * \brief         Tests whether a failure free cycle is requred for an event
 *                to start aging.
 *
 * \details       Tests whether a failure free cycle is requred for an event
 *                to start aging. (Additional starting conditions might apply)
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Failure free cycle is required.
 * \return        FALSE
 *                Failure free cycle is not required.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestStartNeedsFailureFreeCycle(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Aging_TestContinuationNeedsFailureFreeCycle
 *****************************************************************************/
/*!
 * \brief         Tests whether a the event must be failed at the end the OP cycle
 *                in order for the aging to continue.
 *
 * \details       Tests whether a the event must be failed at the end the OP cycle
 *                in order for the aging to continue. (Additional continuation
 *                conditions might apply)
 *
 * \return        TRUE
 *                Events must not be reported failed in the aging cycle 
 *                for the aging to continue.
 * \return        FALSE
 *                Events may be reported failed without interrupting the aging.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestContinuationNeedsFailureFreeCycle(
  void
);

/* ****************************************************************************
 * Dem_Core_Aging_TestContinuationNeedsReport
 *****************************************************************************/
/*!
 * \brief         Tests whether a report is needed for aging to continue.
 *
 * \details       Tests whether a report is needed for aging to continue.
 *                (Additional continuation conditions might apply)
 *
 * \return        TRUE
 *                Events must be reported for the aging to continue.
 * \return        FALSE
 *                Events need not be reported for the aging to continue.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestContinuationNeedsReport(
  void
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Core_Aging_SetAgingTargetCycle
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Core_Aging_SetAgingTargetCycle(                                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  uint16 AgingTargetCycle
  )
{
  if (Dem_Core_Aging_TestEntryIndependentAgingSupported() == TRUE)
  {
    if ((Dem_Cfg_EventUdsDtc(EventId) != DEM_CFG_DTC_UDS_INVALID)
     && (Dem_Core_Aging_GetEntryIndependentAgingTargetCycle(EventId) != AgingTargetCycle))
    {
      Dem_Core_Aging_SetEntryIndependentAgingTargetCycle(EventId, AgingTargetCycle);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_AGINGDATA), DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
  else 
  {
    Dem_Internal_AssertContinue(EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID, DEM_E_INCONSISTENT_STATE);
    Dem_MemoryEntry_EventEntry_SetAgingTargetCycle(EventEntryIndex, AgingTargetCycle);
    Dem_Nvm_SetEventEntryNvBlockState(EventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Core_Aging_SetEntryIndependentAgingTargetCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_SetEntryIndependentAgingTargetCycle(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId,
  uint16 AgingTargetCycle
  )
{
#if (DEM_CFG_SUPPORT_AGING_FOR_ALL_DTCS == STD_ON)
  {
    uint16 lAgingCounterIndex = Dem_Cfg_GetAgingCounterIndexOfEventTable(EventId);

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if (lAgingCounterIndex >= DEM_CFG_MAX_NUMBER_AGING_DTCS)
    {
      /* DEM_CFG_MAX_NUMBER_AGING_DTCS is always lesser than DEM_CFG_AGINGCOUNTER_INDEX_INVALID */
      Dem_Error_RunTimeCheckFailed(DEM_EVENT_IMPLEMENTATION_FILENAME, __LINE__);                                                 /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    }
    else
# endif
    {
      Dem_Cfg_GetAgingData().AgingTargetCycle[lAgingCounterIndex] = AgingTargetCycle;                                            /* SBSW_DEM_ARRAY_WRITE_AGINGDATA_AGINGTARGETCYCLE */
    }
  }
#else
  {
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(AgingTargetCycle)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
}

/* ****************************************************************************
 % Dem_Core_Aging_GetEntryIndependentAgingTargetCycle
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
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Core_Aging_GetEntryIndependentAgingTargetCycle(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  uint16 lAgingTargetCycle = DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID;

#if (DEM_CFG_SUPPORT_AGING_FOR_ALL_DTCS == STD_ON)
  {
    uint16 lAgingCounterIndex = Dem_Cfg_GetAgingCounterIndexOfEventTable(EventId);
    if (lAgingCounterIndex < DEM_CFG_MAX_NUMBER_AGING_DTCS)
    {
      /* If indexing is inside the array, read out the value, else return default */
      lAgingTargetCycle = Dem_Cfg_GetAgingData().AgingTargetCycle[lAgingCounterIndex];
    }
  }
#else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
#endif

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return lAgingTargetCycle;
}

/* ****************************************************************************
 % Dem_Core_Aging_InvalidateEntryIndependentAgingTargetCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Core_Aging_InvalidateEntryIndependentAgingTargetCycle(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  if ((Dem_Cfg_EventUdsDtc(EventId) != DEM_CFG_DTC_UDS_INVALID)
      && (Dem_Core_Aging_GetEntryIndependentAgingTargetCycle(EventId) != DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID))
  {
    Dem_Core_Aging_SetEntryIndependentAgingTargetCycle(EventId, DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID);
    Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_AGINGDATA),
                                DEM_NVM_BLOCKSTATE_DIRTY);
  }
}

/* ****************************************************************************
 % Dem_Core_Aging_TestEntryIndependentAgingSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestEntryIndependentAgingSupported(
  void
  )
{
  return (DEM_CFG_SUPPORT_AGING_FOR_ALL_DTCS == STD_ON);
}

/* ****************************************************************************
 % Dem_Core_Aging_TestImmediateAgingStartOnPassedSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestImmediateAgingStartOnPassedSupported(
  Dem_EventIdType EventId
  )
{
  uint8 lAgingTarget = Dem_Core_Aging_GetAgingTarget(EventId);
  return (((lAgingTarget == 0u) && (DEM_CFG_AGING_00_START_TRIGGER_PASSED == STD_ON))
       || ((lAgingTarget != 0u) && (DEM_CFG_AGING_START_TRIGGER_PASSED == STD_ON)));
}


/* ****************************************************************************
 % Dem_Core_Aging_TestStartNeedsFailureFreeCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestStartNeedsFailureFreeCycle(
  Dem_EventIdType EventId
)
{
  uint8 lAgingTarget = Dem_Core_Aging_GetAgingTarget(EventId);
  return (((lAgingTarget == 0u) && (DEM_CFG_AGING_00_START_TESTS_TFTOC == STD_ON))
       || ((lAgingTarget != 0u) && (DEM_CFG_AGING_START_TESTS_TFTOC == STD_ON)));
}

/* ****************************************************************************
 % Dem_Core_Aging_TestContinuationNeedsFailureFreeCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestContinuationNeedsFailureFreeCycle(
  void
)
{
  return (DEM_CFG_AGING_NEXT_TESTS_TFTOC == STD_ON);
}

/* ****************************************************************************
 % Dem_Core_Aging_TestContinuationNeedsFailureFreeCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestContinuationNeedsReport(
  void
)
{
  return (DEM_CFG_AGING_NEXT_TESTS_TNCTOC == STD_ON);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Aging_Properties
 * \{
 */

/* ****************************************************************************
 % Dem_Core_Aging_IsAgingForAllEventsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_IsAgingForAllEventsSupported(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void)
{
  return (DEM_CFG_SUPPORT_AGING_FOR_ALL_DTCS == STD_ON);
}

/* ****************************************************************************
 % Dem_Core_Aging_SingleStorage_TestAgingAfterHealingAllEventsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_SingleStorage_TestAgingAfterHealingAllEventsSupported(
  void
)
{
  return (DEM_CFG_AGING_AFTER_HEALING_ALL_DTC == STD_ON);
}

/* ****************************************************************************
 % Dem_Core_Aging_SingleStorage_TestAgingAfterHealingSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_SingleStorage_TestAgingAfterHealingSupported(
  void
)
{
  return (DEM_CFG_AGING_AFTER_HEALING == STD_ON);
}

/* ****************************************************************************
 % Dem_Core_Aging_GetAgingTargetCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Core_Aging_GetAgingTargetCycle(                                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint16 lTargetCycle = DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID;
  if (Dem_Core_Aging_TestEntryIndependentAgingSupported() == TRUE)
  {
    lTargetCycle = Dem_Core_Aging_GetEntryIndependentAgingTargetCycle(EventId);
  }
  else
  {
    if (EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
    {
      lTargetCycle = Dem_MemoryEntry_EventEntry_GetAgingTargetCycle(EventEntryIndex);
    }
  }
  return lTargetCycle;
}

/* ****************************************************************************
 % Dem_Core_Aging_IsAgingEntryAllocationSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_IsAgingEntryAllocationSupported(
  void
  )
{
  return (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON);
}

/* ****************************************************************************
 % Dem_Core_Aging_IsEntryRetentionAfterAgingSuppoted
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_IsEntryRetentionAfterAgingSuppoted(
  void
  )
{
  return (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON);
}

/* ****************************************************************************
 % Dem_Core_Aging_GetAgingTarget
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Core_Aging_GetAgingTarget(
  Dem_EventIdType EventId
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetAgingCycleCounterThresholdOfEventTable(EventId);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Aging_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Core_Aging_Continue
 *****************************************************************************/
/*!
 * \brief         Handles the aging process at the end of the aging cycle.
 *
 * \details       Handles the aging process at the end of the aging cycle.
 *                Aging might be delayed or finished depending on aging conditions
 *                and the aging target cycle counter.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Aging must currently be active.
 * \pre           Function is called when aging cycle is ended and aging cycle counter
 *                has already been incremented.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_Continue(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Core_Aging_IncrementAgingTargetCycle
 *****************************************************************************/
/*!
 * \brief         Increments the aging target cycle by one.
 *
 * \details       Increments the aging target cycle by one. Thus, aging will 
 *                complete one cycle later.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           Aging must currently be active.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_IncrementAgingTargetCycle(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
);

/* ****************************************************************************
* Dem_Core_Aging_Finished
*****************************************************************************/
/*!
 * \brief         Handles the end of aging.
 *
 * \details       Informs other units about the end of the aging process and handles
 *                the aging target cycle count.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                An event entry always exists when aging ends with the only exception of
 *                Zero Target events which may age immediately on a passed report even
 *                without having an event entry.
 * \param[in]     SuppressNotifications
 *                Suppress DTC and Event Status Change Notifications
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_Finished(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  boolean SuppressNotifications
);

/* ****************************************************************************
* Dem_Core_Aging_RestoreAgingTargetCycleOfEntry
*****************************************************************************/
/*!
 * \brief          Restores the aging counter target of an event.
 *
 * \details        Restores the aging counter target of an event.
 *                 Based on current aging cycle value and aging threshold.
 *                 If aging would take longer than aging threshold defines.
 *
 * \param[in]      EventEntryIndex
 *                 Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                 Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre            -
 * 
 * \context        TASK
 * \synchronous    TRUE
 * \reentrant      TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_RestoreAgingTargetCycleOfEntry(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_Core_Aging_RestoreStoredStatusOfEntry
 *****************************************************************************/
/*!
 * \brief         Restores the aging status of an event and sets the correcponding
 *                event storage state.
 *
 * \details       -
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_RestoreStoredStatusOfEntry(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_Core_Aging_StartWWHOBDAgingTimer
 *****************************************************************************/
/*!
 * \brief         Starts the WWHOBD aging timer.
 *
 * \details       Starts the WWHOBD aging timer when aging starts.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_StartWWHOBDAgingTimer(
  Dem_Cfg_EntryIndexType EventEntryIndex
);

/* ****************************************************************************
 * Dem_Core_Aging_TryAllocateAgingOnlyEntry
 *****************************************************************************/
/*!
 * \brief         If supported by configuration, allocate an aging only entry.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Event entry index to be used for aging. Invalid allocatable
 *                or event is already stored.
 * 
 * \pre           Event is not stored.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Core_Aging_TryAllocateAgingOnlyEntry(
  Dem_EventIdType  EventId
);

/* ****************************************************************************
 * Dem_Core_Aging_UpdateEntryOnAgingStart
 *****************************************************************************/
/*!
 * \brief         Updates event entry related data once aging starts.
 *
 * \details       Updates event entry related data once aging starts, such as
 *                the stored status and WWHOBD aging timer.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_UpdateEntryOnAgingStart(
  Dem_Cfg_EntryIndexType EventEntryIndex
);

/* ****************************************************************************
 * Dem_Core_Aging_StartMultiTarget
 *****************************************************************************/
/*!
 * \brief         Starts aging of an event.
 *
 * \details       This function sets up an event for aging. In case the event
 *                already ages (or does not support aging), this function has
 *                no effect.
 *
 * \param[in]     AgingEventContext
 *                Contains information about the event to be aged.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_StartMultiTarget(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Core_Aging_StartZeroTarget
 *****************************************************************************/
 /*!
 * \brief         Starts aging of an event with aging target 0
 *
 * \details       This function processes aging for events with aging target
 *                0. I.e. the event will age immediately. This function has
 *                no effect if the event in question is already aged or does
 *                not support aging.
 *
 * \param[in]     AgingEventContext
 *                Contains information about the event to be aged.
 * \param[in]     SuppressNotifications
 *                Suppress DTC and Event Status Change Notifications
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_StartZeroTarget(
  Dem_EventIdType EventId,
  boolean SuppressNotifications
  );

/* ****************************************************************************
 * Dem_Core_Aging_Start
 *****************************************************************************/
/*!
 * \brief         Starts aging of an event.
 *
 * \details       This function sets up an event for aging for both zero target
                  and multi target events.
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     SuppressNotifications
 *                Suppress DTC and Event Status Change Notifications
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Core_Aging_Start(
  Dem_EventIdType EventId,
  boolean SuppressNofitications
);

/* ****************************************************************************
 * Dem_Core_Aging_TestStartConditionsFulfilled
 *****************************************************************************/
/*!
 * \brief         Checks whether the conditions are fulfilled to start aging.
 *
 * \details       Checks whether the conditions are fulfilled to start aging
 *                for all configurable aging types. The output of this function
 *                is only valid, if aging is not currently prevented by healing.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     DuringActiveOperationCycle
 *                Inidcates whether aging start conditions are requested
 *                for an active operation cycle or at the end of a cycle.
 *
 * \pre           Aging is not prevented by healing.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestStartConditionsFulfilled(
  Dem_EventIdType EventId,
  boolean DuringActiveOperationCycle
);

/* ****************************************************************************
 * Dem_Core_Aging_TestPausedThisCycle
 *****************************************************************************/
/*!
 * \brief         Tests whether the conditions to continue aging are currently
 *                fulfilled.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The aging conditions are fulfilled for this cycle.
 * \return        FALSE
 *                the aging conditions are not fulfilled for this cycle.
 *
 * \pre           Aging is currently active and advancing, so TF of the Event is
 *                false.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestPausedThisCycle(
  Dem_EventIdType EventId
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Aging_Private
 * \{
 */

/* ****************************************************************************
 % Dem_Core_Aging_Continue
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_Continue(                                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
)
{
  Dem_UDSStatus_StatusType lEventUdsStatus = Dem_Core_Event_GetInternalUdsStatus(EventId);
  if (Dem_Event_UDSStatus_TestBit(lEventUdsStatus, Dem_UdsStatus_TF) == FALSE)
  {
    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, FALSE);
    if (Dem_Core_Aging_TestPausedThisCycle(EventId) == TRUE)
    {
      Dem_Event_DataUpdateStart(EventId);
      Dem_Core_Aging_IncrementAgingTargetCycle(EventId, lEventEntryIndex);
      Dem_Event_DataUpdateFinish(EventId);
    }
    else if (Dem_OperationCycle_GetCurrentAgingCycleCount(EventId) == Dem_Core_Aging_GetAgingTargetCycle(EventId, lEventEntryIndex))
    {
      Dem_Event_DataUpdateStart(EventId);
      Dem_Core_Aging_Finished(EventId, lEventEntryIndex, FALSE);
      Dem_Event_DataUpdateFinish(EventId);
    }
    else
    { /* MISRA */ }
  }
  
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STMIF, MD_MSR_STCAL */


 /* ****************************************************************************
 % Dem_Core_Aging_IncrementAgingTargetCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_IncrementAgingTargetCycle(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
)
{
  uint16 lAgingTargetCycle = Dem_Core_Aging_GetAgingTargetCycle(EventId, EventEntryIndex);
  if (lAgingTargetCycle <= DEM_OPERATIONCYCLE_CYCLECOUNT_MAX)
  {
    lAgingTargetCycle = Dem_OperationCycle_AddCycleCount(lAgingTargetCycle, 1);
    Dem_Core_Aging_SetAgingTargetCycle(EventId, EventEntryIndex, lAgingTargetCycle);
  }
}


/* ****************************************************************************
% Dem_Core_Aging_IncrementAgingTargetCycle
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_Finished(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  boolean SuppressNotifications
)
{
  if (Dem_Cfg_EventSupportAging(EventId) == TRUE)
  {
    Dem_DTC_ProcessAgingFinished(EventId, EventEntryIndex, SuppressNotifications);
  }
  if ((Dem_Core_Aging_IsAgingForAllEventsSupported() == TRUE) || (EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID))
  {
    if (Dem_Cfg_EventSupportAging(EventId) == TRUE)
    {
      Dem_Core_Aging_SetAgingTargetCycle(EventId, EventEntryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_AGED);
      Dem_Memories_ObdFreezeFrameMemory_ProcessAgingFinished(EventId);
    }
    else
    {
      Dem_Core_Aging_SetAgingTargetCycle(EventId, EventEntryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_LATCHED);
    }
  }
}

/* ****************************************************************************
% Dem_Core_Aging_RestoreAgingTargetCycleOfEntry
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_RestoreAgingTargetCycleOfEntry(
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_EventIdType lEventId = Dem_MemoryEntry_EventEntry_GetEventId(EventEntryIndex);
  uint16 lAgingTargetCycle = Dem_Core_Aging_GetAgingTargetCycle(lEventId, EventEntryIndex);

  if ((Dem_Core_Aging_TestStartNeedsFailureFreeCycle(lEventId) == TRUE)
   && (Dem_Core_Aging_TestImmediateAgingStartOnPassedSupported(lEventId) == FALSE)
   && (Dem_Core_Aging_IsAgingForAllEventsSupported() == TRUE))
  {
    if (Dem_MemoryEntry_EventEntry_GetConsecutiveFailedCycleCounter(EventEntryIndex) > 0u)
    {
      Dem_Core_Aging_InvalidateEntryIndependentAgingTargetCycle(lEventId);
    }
  }

  if ((lAgingTargetCycle <= DEM_OPERATIONCYCLE_CYCLECOUNT_MAX) && (Dem_Cfg_EventSupportAging(lEventId) == TRUE))
  {
    uint16 lCurrentAgingCycleCount = Dem_OperationCycle_GetCurrentAgingCycleCount(lEventId);
    uint8 lAgingTarget = Dem_Core_Aging_GetAgingTarget(lEventId);

    if (Dem_OperationCycle_CycleCountDistance(lCurrentAgingCycleCount, lAgingTargetCycle) > lAgingTarget)
    {
      lAgingTargetCycle = Dem_OperationCycle_AddCycleCount(lCurrentAgingCycleCount, lAgingTarget);
      Dem_Core_Aging_SetAgingTargetCycle(lEventId, EventEntryIndex, lAgingTargetCycle);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Core_Aging_RestoreStoredStatusOfEntry
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_RestoreStoredStatusOfEntry(
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_InternalStatus_StoredStatusType lStoredStatus = Dem_InternalStatus_StoredStatus_None;
  Dem_EventIdType lEventId = Dem_MemoryEntry_EventEntry_GetEventId(EventEntryIndex);
  Dem_Aging_StatusType lAgingStatus = Dem_Core_Aging_GetAgingStatus(lEventId, EventEntryIndex);

  if ((lAgingStatus == Dem_AgingStatus_Aging) || (lAgingStatus == Dem_AgingStatus_Latched))
  {
    lStoredStatus = Dem_InternalStatus_StoredStatus_Aging;
  }
  else if ((Dem_Core_Aging_IsEntryRetentionAfterAgingSuppoted() == TRUE) && (lAgingStatus == Dem_AgingStatus_Aged))
  {
    lStoredStatus = Dem_InternalStatus_StoredStatus_Aged;
  }
  else
  {
    lStoredStatus = Dem_InternalStatus_StoredStatus_Active;
  }
  Dem_Event_InternalStatus_SetStoredStatus(lEventId, lStoredStatus);
}

/* ****************************************************************************
% Dem_Core_Aging_StartWWHOBDAgingTimer
*****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_StartWWHOBDAgingTimer(
  Dem_Cfg_EntryIndexType EventEntryIndex
)
{
  /* For WWH-OBD, also start the event's aging timer */
  if (Dem_Cfg_EventSupportAging(Dem_MemoryEntry_EventEntry_GetEventId(EventEntryIndex)) == TRUE)
  {
    uint32 lAgingTimer = Dem_GlobalDiagnostics_GetCumulativeEngineRuntime();
    Dem_MemoryEntry_EventEntry_SetAgingTimer(EventEntryIndex, lAgingTimer + DEM_AGING_AGINGTIMER_MINUTES_200h);
    Dem_Nvm_SetEventEntryNvBlockState(EventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
  }
}

 /* ****************************************************************************
 % Dem_Core_Aging_TryAllocateAgingOnlyEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Core_Aging_TryAllocateAgingOnlyEntry(
  Dem_EventIdType  EventId
)
{
  Dem_Cfg_EntryIndexType lEventEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
  if ((Dem_Core_Aging_IsAgingEntryAllocationSupported() == TRUE) && (Dem_Event_InternalStatus_TestEventStored(EventId) == FALSE)) /* COV_DEM_ROBUSTNESS TX tx tx */ /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
  {
    if (Dem_Event_UDSStatus_TestBit(Dem_Core_Event_GetInternalUdsStatus(EventId), Dem_UdsStatus_CDTC) == TRUE)
    {
      lEventEntryIndex = Dem_FaultMemory_Memories_AllocateEventEntryIndexAging(EventId);
    }
  }
  return lEventEntryIndex;
}

/* ****************************************************************************
 % Dem_Core_Aging_UpdateEntryOnAgingStart
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_UpdateEntryOnAgingStart(
  Dem_Cfg_EntryIndexType EventEntryIndex
)
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    Dem_Core_Aging_StartWWHOBDAgingTimer(EventEntryIndex);
  }
  /* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
  Dem_EnterCritical_DiagMonitor();
  Dem_Event_InternalStatus_SetStoredStatus(Dem_MemoryEntry_EventEntry_GetEventId(EventEntryIndex), Dem_InternalStatus_StoredStatus_Aging);
  /* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
  Dem_LeaveCritical_DiagMonitor();
}

/* ****************************************************************************
 % Dem_Core_Aging_StartMultiTarget
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_StartMultiTarget(
  Dem_EventIdType EventId
)
{
  Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, FALSE);
  if ((Dem_Core_Aging_IsAgingEntryAllocationSupported() == TRUE) && (lEventEntryIndex == DEM_CFG_ENTRYINDEX_INVALID))
  {
    lEventEntryIndex = Dem_Core_Aging_TryAllocateAgingOnlyEntry(EventId);
  }
  
  if ((Dem_Core_Aging_IsAgingForAllEventsSupported() == TRUE) || (lEventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID))
  {
    if ((Dem_Event_InternalStatus_GetStoredStatus(EventId) == Dem_InternalStatus_StoredStatus_Active) 
     || (Dem_Event_UDSStatus_TestBit(Dem_Core_Event_GetInternalUdsStatus(EventId), Dem_UdsStatus_CDTC) == TRUE))
    {
      uint16 lAgingCounter = Dem_OperationCycle_GetCurrentAgingCycleCount(EventId);
      lAgingCounter = Dem_OperationCycle_AddCycleCount(lAgingCounter, Dem_Core_Aging_GetAgingTarget(EventId));
      Dem_Core_Aging_SetAgingTargetCycle(EventId, lEventEntryIndex, lAgingCounter);

      if (lEventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
      {
        Dem_Core_Aging_UpdateEntryOnAgingStart(lEventEntryIndex);
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Core_Aging_StartZeroTarget
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_StartZeroTarget(
  Dem_EventIdType EventId,
  boolean SuppressNotifications
)
{
  if (Dem_Event_InternalStatus_GetStoredStatus(EventId) < Dem_InternalStatus_StoredStatus_Aging)                                 /* COV_DEM_ROBUSTNESS TX */
  {
    Dem_Core_Aging_Finished(EventId, Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, FALSE), SuppressNotifications);
  }
}

/* ****************************************************************************
 % Dem_Core_Aging_TestStartConditionsFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Core_Aging_Start(
  Dem_EventIdType EventId,
  boolean SuppressNofitications
)
{
  if (Dem_Core_Aging_GetAgingTarget(EventId) == 0u)
  {
    Dem_Core_Aging_StartZeroTarget(EventId, SuppressNofitications);
  }
  else
  {
    Dem_Core_Aging_StartMultiTarget(EventId);
  }
}

/* ****************************************************************************
 % Dem_Core_Aging_TestStartConditionsFulfilled
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
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestStartConditionsFulfilled(
  Dem_EventIdType EventId,
  boolean DuringActiveOperationCycle
  )
{

  Dem_UDSStatus_StatusType lUdsStatus = Dem_Core_Event_GetInternalUdsStatus(EventId);
  boolean lConditionsFulfilled = (boolean)((Dem_Event_UDSStatus_TestBit(lUdsStatus, Dem_UdsStatus_TF) == FALSE)
                                        && (Dem_Event_UDSStatus_TestBit(lUdsStatus, Dem_UdsStatus_TNCTOC) == FALSE)
                                        && (Dem_Core_Aging_GetAgingStatus(EventId,                                               /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
                                              Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, FALSE)) == Dem_AgingStatus_None));

  if ((Dem_Core_Aging_TestStartNeedsFailureFreeCycle(EventId) == TRUE)
   && (Dem_Event_UDSStatus_TestBit(lUdsStatus, Dem_UdsStatus_TFTOC) == TRUE))
  {
    lConditionsFulfilled = FALSE;
  }
  if ((DuringActiveOperationCycle == TRUE) && (Dem_Core_Aging_TestImmediateAgingStartOnPassedSupported(EventId) == FALSE))
  {
    lConditionsFulfilled = FALSE;
  }
  return lConditionsFulfilled;
}

/* ****************************************************************************
 % Dem_Core_Aging_TestPausedThisCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_TestPausedThisCycle(
  Dem_EventIdType EventId
  )
{
  Dem_UDSStatus_StatusType lUdsStatus = Dem_Core_Event_GetInternalUdsStatus(EventId);
  boolean lPauseAging = FALSE;
  if ((Dem_Core_Aging_TestContinuationNeedsFailureFreeCycle() == TRUE)
   && (Dem_Event_UDSStatus_TestBit(lUdsStatus, Dem_UdsStatus_TFTOC) == TRUE))
  {
    lPauseAging = TRUE;
  }
  if ((Dem_Core_Aging_TestContinuationNeedsReport() == TRUE)
   && (Dem_Event_UDSStatus_TestBit(lUdsStatus, Dem_UdsStatus_TNCTOC) == TRUE))
  {
    lPauseAging = TRUE;
  }
  return lPauseAging;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Aging_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Core_Aging_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_Init(
  void
  )
{
  if (Dem_Core_Aging_IsAgingForAllEventsSupported())
  {
    Dem_EventIdType lEventId;
    for (lEventId = DEM_EVENT_FIRST; lEventId < Dem_Cfg_GlobalEventCount(); ++lEventId)
    {
      /* Only process events which can be stored */
      if (lEventId != Dem_Cfg_GetMemoryRepresentative(lEventId))
      {
        continue;
      }
      /* Invalidate the aging counter of events which are not stored and confirmed */
      if ((Dem_Event_InternalStatus_TestEventStored(lEventId) == FALSE)
        && (Dem_Event_UDSStatus_TestBit(Dem_Core_Event_GetInternalUdsStatus(lEventId), Dem_UdsStatus_CDTC) == FALSE))
      {
        Dem_Core_Aging_InvalidateEntryIndependentAgingTargetCycle(lEventId);
      }
    }
  }
}

/* ****************************************************************************
 % Dem_Core_Aging_ProcessAgingCycleEnd
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessAgingCycleEnd(
  Dem_EventIdType EventId
)
{
  /* Events which do not have a DTC configured need not be handled */
  /* Only process master events because combined events have the same DTC */
  if ((Dem_Cfg_EventUdsDtc(EventId) != DEM_CFG_DTC_UDS_INVALID) && (EventId == Dem_Cfg_GetMemoryRepresentative(EventId)))
  { 
# if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
    /* Event is currently active (normal case) or aging with an event entry */
    if ((Dem_Core_Aging_GetAgingStatus(EventId, Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, FALSE)) == Dem_AgingStatus_Aging)
      || (Dem_Event_InternalStatus_GetStoredStatus(EventId) == Dem_InternalStatus_StoredStatus_Active))                          /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      Dem_Core_Aging_Continue(EventId);
    }
# endif
  }
}

/* ****************************************************************************
 % Dem_Core_Aging_ProcessAgingTimer
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessAgingTimer(
  void
)
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)                                                                              /* COV_DEM_ROBUSTNESS TX */
  {
    Dem_Cfg_EntryIterType lPrimaryEventEntryIter;

    /* Check for all OBD events in primary memory if they are currently in aging process and if aging time has exceeded */

    for (Dem_Cfg_ComplexIter32BitInit(&lPrimaryEventEntryIter,                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
           Dem_FaultMemory_Memories_GetFirstEntryIndex(DEM_CFG_MEMORYID_PRIMARY),
           Dem_FaultMemory_Memories_GetEndEntryIndex(DEM_CFG_MEMORYID_PRIMARY));
         Dem_Cfg_ComplexIter32BitExists(&lPrimaryEventEntryIter) == TRUE;                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_ComplexIter32BitNext(&lPrimaryEventEntryIter))                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lPrimaryEventEntryIter);                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_EventIdType lEventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);
      uint32 lMemoryEntryAgingTimer = Dem_MemoryEntry_EventEntry_GetAgingTimer(lEventEntryIndex);

      if (Dem_Event_TestValidHandle(lEventId) == TRUE)
      {
        if ((Dem_Core_DTC_IsObdRelated(lEventId) == TRUE)
          && (Dem_Cfg_EventSupportAging(lEventId) == TRUE))
        {
          if (Dem_Event_InternalStatus_GetStoredStatus(lEventId) == Dem_InternalStatus_StoredStatus_Aging)
          { /* Event is currently aging */
            if (Dem_GlobalDiagnostics_GetCumulativeEngineRuntime() >= lMemoryEntryAgingTimer)
            { /* Aging time reached, so this event has aged */
              Dem_Event_DataUpdateStart(lEventId);
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
              Dem_Core_Aging_Finished(lEventId, lEventEntryIndex, FALSE);
#endif
              Dem_Event_DataUpdateFinish(lEventId);
            }
          }
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */


/* ****************************************************************************
 % Dem_Core_Aging_ProcessOnDisplaced
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessOnDisplaced(                                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType MemoryRepresentativeEventId
)
{
  if ((Dem_Core_Aging_IsAgingForAllEventsSupported() == TRUE) && (Dem_Event_UDSStatus_TestBit(Dem_Core_Event_GetInternalUdsStatus(MemoryRepresentativeEventId), Dem_UdsStatus_CDTC) == FALSE))
  {
    Dem_Core_Aging_InvalidateEntryIndependentAgingTargetCycle(MemoryRepresentativeEventId);
  }
}

/* ****************************************************************************
 % Dem_Core_Aging_ProcessOnClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessOnClear(
  Dem_EventIdType EventId
  )
{
  if (Dem_Core_Aging_IsAgingForAllEventsSupported() == TRUE)
  {
    Dem_Core_Aging_InvalidateEntryIndependentAgingTargetCycle(EventId);
  }
}

/* ****************************************************************************
 % Dem_Core_Aging_ProcessOnFailed
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessOnFailed(                                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  if ((Dem_Core_Aging_IsAgingForAllEventsSupported() == TRUE)
   || (Dem_Event_InternalStatus_GetStoredStatus(EventId) == Dem_InternalStatus_StoredStatus_Aging))                              /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
  {
    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, FALSE);
    if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
    {
      Dem_MemoryEntry_EventEntry_SetAgingTimer(lEventEntryIndex, DEM_EVENTENTRY_AGINGTIME_INVALID);
    }
    Dem_Core_Aging_SetAgingTargetCycle(EventId, lEventEntryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID);
  }
}

/* ****************************************************************************
 % Dem_Core_Aging_ProcessOnFdcTrip
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessOnFdcTrip(                                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
  )
{
  if ((Dem_Core_Aging_IsAgingForAllEventsSupported() == TRUE) && (Dem_Core_Aging_GetEntryIndependentAgingTargetCycle(EventId) == DEM_OPERATIONCYCLE_CYCLECOUNT_AGED))
  {
    Dem_Core_Aging_InvalidateEntryIndependentAgingTargetCycle(EventId);
  }
}

/* ****************************************************************************
 % Dem_Core_Aging_RestoreAgingDataOfEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_RestoreAgingDataOfEntry(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_Core_Aging_RestoreAgingTargetCycleOfEntry(EventEntryIndex);
  Dem_Core_Aging_RestoreStoredStatusOfEntry(EventEntryIndex);
}

/* ****************************************************************************
 % Dem_Core_Aging_GetAgingStatus
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
DEM_LOCAL FUNC(Dem_Aging_StatusType, DEM_CODE)
Dem_Core_Aging_GetAgingStatus(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_Aging_StatusType lAgingStatus = Dem_AgingStatus_None;
  uint16 lAgingTargetCycle;

  lAgingTargetCycle = Dem_Core_Aging_GetAgingTargetCycle(EventId, EventEntryIndex);

  if (lAgingTargetCycle <= DEM_OPERATIONCYCLE_CYCLECOUNT_MAX)
  {
    lAgingStatus = Dem_AgingStatus_Aging;
  }
  else if (lAgingTargetCycle == DEM_OPERATIONCYCLE_CYCLECOUNT_AGED)
  {
    lAgingStatus = Dem_AgingStatus_Aged;
  }
  else if (lAgingTargetCycle == DEM_OPERATIONCYCLE_CYCLECOUNT_LATCHED)
  {
    lAgingStatus = Dem_AgingStatus_Latched;
  }
  else
  {
    /* Invaid operation cycle count - return aging status 'none'. */
  }
  return lAgingStatus;
}

/* ****************************************************************************
 % Dem_Core_Aging_ProcessStartOnPassed
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessStartOnPassed(                                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
)
{
  if (Dem_Core_Event_TestHealingPreventsAging(EventId) == FALSE)
  {
    if (Dem_Core_Aging_TestStartConditionsFulfilled(EventId, TRUE) == TRUE)
    {
      Dem_Core_Aging_Start(EventId, FALSE);
    }
  }
}

/* ****************************************************************************
 % Dem_Core_Aging_ProcessStartOnOPCycleEnd
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessStartOnOPCycleEnd(                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
)
{
  if (Dem_Core_Event_TestHealingPreventsAging(EventId) == FALSE)
  {
    if (Dem_Core_Aging_TestStartConditionsFulfilled(EventId, FALSE) == TRUE)
    {
      Dem_Core_Aging_Start(EventId, TRUE);
    }
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif /* DEM_AGING_IMPLEMENTATION_H_ */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Aging_Implementation.h
 *********************************************************************************************************************/

