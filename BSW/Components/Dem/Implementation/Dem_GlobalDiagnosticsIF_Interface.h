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
/*!
 *  \defgroup   Dem_GlobalDiagnosticsIF GlobalDiagnosticsIF
 *  \{
 *  \file       Dem_GlobalDiagnosticsIF_Interface.h
 *  \brief      Implementation of unit GlobalDiagnosticsIF.
 *  \details    Realizes the public interfaces of its parent logical unit, i.e. both Dem external service APIs and Dem internal APIs.
 *  \entity     GlobalDiagnosticsIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_GLOBALDIAGNOSTICSIF_INTERFACE_H)
#define DEM_GLOBALDIAGNOSTICSIF_INTERFACE_H

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
/* PRQA S 3415 EOF */ /* MD_DEM_13.5_cf */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_GlobalDiagnosticsIF_PublicProperties Public Properties
 * \{
 */

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_GlobalDiagnostics_ReadRingBufferSlotChronological
 *****************************************************************************/
/*!
 * \brief         Returns ring buffer entry at the requested index for the
 *                specified sampling rate.
 *
 * \details       Returns ring buffer entry at the requested index. The index is
 *                in chronological order ranging from [0..number of stored samples[.
 *                Thus, index 0 referes to the oldest element currently saved in
 *                the ring buffer for the given sampling rate.
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 * \param[in]     DataIndexChrono
 *                Index of the entry to read. The index is in chronological order
 *                with 0 being the oldest element. The index must be less than the
 *                number of past samples used by the specified sampling profile.
 *
 * \return        Pointer to the buffer
 *
 * \pre           Time series feature must be enabled for a return value other
 *                than NULL_PTR.
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_GlobalDiagnostics_ReadRingBufferSlotChronological(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  uint8 DataIndexChrono
);
#endif

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestNewRingBufferSampleAvailable
 *****************************************************************************/
/*!
 * \brief         Test whether a new sample has been stored in the ring buffer.
 *
 * \details       Test whether a new sample has been stored in the ring buffer
 *                since the value was last fetched.
 *
 * \return        TRUE
 *                A new sample is available.
 * \return        FALSE
 *                No new sample is available.
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestNewRingBufferSampleAvailable(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetRingBufferSampleCount
 *****************************************************************************/
/*!
 * \brief         Get the number of samples currently stored in the ring
 *                buffer.
 *
 * \details       Get the number of samples currently stored in the ring
 *                buffer for the specified sampling profile
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 * 
 * \return        Sample count in the respective buffer.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_GetRingBufferSampleCount(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetMostRecentRingBufferSampleId
 *****************************************************************************/
/*!
 * \brief         Return the position of the most recent sample in the ring buffer.
 *
 * \details       Return the position of the most recent sample in the ring buffer.
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 * 
 * \return        Position of the most recent sample in the ring buffer.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_GetMostRecentRingBufferSampleId(                                                                           /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetTimeSinceLastRingBufferSample
 *****************************************************************************/
/*!
 * \brief         Returns the time since the last sample was collected to the
 *                ring buffer with the given sample rate.
 *
 * \details       Returns the time since the last sample was collected to the
 *                ring buffer with the given sample rate, if time series 
                  power train feature is supported.
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 * 
 * \return        Time since the last sample was collected.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetTimeSinceLastRingBufferSample(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestStorageConditionValid
 *****************************************************************************/
/*!
 * \brief         Tests whether the passed storage condition is valid.
 *
 * \details       Tests whether the passed storage condition is valid by
 *                checking that the id does not exceed the global storage
 *                condition count.
 *
 * \param[in]     StorageConditionId
 *                Storage condition id to validate.
 * 
 * \return        TRUE
 *                The storage condition is valid.
 * \reutrn        FALSE
 *                The storage condition is not valid.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestStorageConditionValid(
  uint8 StorageConditionId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestEnableConditionValid
 *****************************************************************************/
/*!
 * \brief         Tests whether the passed enable condition is valid.
 *
 * \details       Tests whether the passed enable condition is valid by
 *                checking that the id does not exceed the global enable
 *                condition count.
 *
 * \return        TRUE
 *                The enable condition is valid.
 * \reutrn        FALSE
 *                The enable condition is not valid.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEnableConditionValid(
  uint8 EnableConditionId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_IsPtoStatusSupportEnabled
 *****************************************************************************/
/*!
 * \brief         Test if PTO status support is enabled
 * \details       Test if PTO status support is enabled
 *
 * \return        TRUE
 *                PTO status support is enabled
 * \return        FALSE
 *                PTO status support is NOT enabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsPtoStatusSupportEnabled(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetPtoStatus
 *****************************************************************************/
/*!
 * \brief         Get the Dem PTO status
 *
 * \details       Get the Dem PTO status
 *
 * \return        TRUE
 *                PTO status is active
 * \return        FALSE
 *                PTO status is not supported or PTO status is inactive
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_GetPtoStatus(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_IsReadinessCalculationSupported
 *****************************************************************************/
/*!
 * \brief         Tests if readiness calculation is supported.
 *
 * \details       Tests if readiness calculation is supported.
 *
 * \return        TRUE
 *                Readiness calculation is supported.
 * \return        FALSE
 *                Readiness calculation is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsReadinessCalculationSupported(
  void
  );


/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetEngineRuntimeInSeconds
 *****************************************************************************/
/*!
 * \brief         Provides the aging timer for access from outside of Global
 *                Diagnostics.
 *
 * \details       -
 * 
 * \return        Aging timer in minutes
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_GetEngineRuntimeInSeconds(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetCumulativeEngineRuntime
 *****************************************************************************/
/*!
 * \brief         Returns the cumulative engine runtime used by other components.
 *
 * \details       -
 * 
 * \return        Cumulative engine runtime in minutes.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_GetCumulativeEngineRuntime(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_IsPtoActivationTimerCalculationSupported
 *****************************************************************************/
/*!
 * \brief         Tests if calculation of PTO activation timer is supported for
 *                the configuration.
 *
 * \details       Tests if calculation of PTO activation timer is supported for
 *                the configuration.
 *
 * \return        TRUE
 *                Calculation of PTO activation timer is supported by configuration.
 * \return        FALSE
 *                Calculation of PTO activation timer is not supported by configuration.
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsPtoActivationTimerCalculationSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_IsPtoActivationTimerSupportedForEvent
 *****************************************************************************/
/*!
 * \brief         Tests if calculation of PTO activation timer is supported for
 *                the event.
 *
 * \details       Tests if calculation of PTO activation timer is supported for
 *                the event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * 
 * \return        TRUE
 *                PTO activation timer is supported for event.
 * \return        FALSE
 *                PTO activation timer is not supported for event.
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsPtoActivationTimerSupportedForEvent(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetDistanceTravelledSinceMilOn
 *****************************************************************************/
/*!
 * \brief         Provides the distance travelled since the MIL turned on.
 *
 * \details       -
 *
 * \return        The distance travelled since the MIL turned on.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetDistanceTravelledSinceMilOn(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetDistanceTravelledSinceLastClear
 *****************************************************************************/
/*!
 * \brief         Provides the distance travelled since the last clear operation
 *                occurred.
 *
 * \details       -
 *
 * \return        The distance travelled since the last clear operation.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetDistanceTravelledSinceLastClear(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetDistanceTravelledSinceConfirmedDTC
 *****************************************************************************/
/*!
 * \brief         Provides the distance travelled since any DTC was confirmed.
 *
 * \details       -
 *
 * \return        The distance travelled since any DTC was confirmed.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetDistanceTravelledSinceConfirmedDTC(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetEngineRuntimeSinceMilOn
 *****************************************************************************/
/*!
 * \brief         Returns the engine runtime since the MIL turned on.
 *
 * \details       Returns the engine runtime since the MIL turned on
 *                stored in the NvM admin data block.
 *
 * \return        Engine runtime since MIL turned on.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetEngineRuntimeSinceMilOn(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetEngineRuntimeSinceClear
 *****************************************************************************/
/*!
 * \brief         Returns the engine runtime since the last clear operation.
 *
 * \details       Returns the engine runtime since the last clear operation
 *                stored in the NvM admin data block.
 *
 * \return        Engine runtime since the last clear operation.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetEngineRuntimeSinceClear(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestPtoActiveAndEventAffected
 *****************************************************************************/
/*!
 * \brief         Tests whether PTO is active and the event is affected by
 *                PTO
 *
 * \details       Tests whether PTO is active and the event is also affected by
 *                PTO
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                PTO is active and the event is affected by PTO
 * \return        FALSE
 *                PTO status is not supported, PTO is inactive or the event
 *                is not affected by PTO
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestPtoActiveAndEventAffected(
  Dem_EventIdType  EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestEventHasReadinessGroup
 *****************************************************************************/
/*!
 * \brief         Tests whether the event has a readiness group.
 *
 * \details       Tests whether the event has a readiness group.
 *
 * \param[in]     EventId
 *                Unique handle of the event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Event has readiness group.
 * \return        FALSE
 *                Event does not have readiness group.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEventHasReadinessGroup(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestEventHasHighPriorityReadinessGroup
 *****************************************************************************/
/*!
 * \brief         Tests whether the events readiness group is high priority.
 *
 * \details       Tests whether the events readiness group is high priority.
 *
 * \param[in]     EventId
 *                Unique handle of the event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Events readiness group is high priority.
 * \return        FALSE
 *                Events readiness group is not high priority.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEventHasHighPriorityReadinessGroup(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetGlobalObdRequirements
 *****************************************************************************/
/*!
 * \brief         Get the supported OBD requirement byte
 *
 * \details       Get the identifier encoding the supported OBD requirements,
 *                as used in PID 1C and DM5
 *
 * \return        The supported OBD requirement byte
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_GetGlobalObdRequirements(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetWarmUpCycleCountSinceClear
 *****************************************************************************/
/*!
 * \brief         Returns the number of warm up cycles since the last clear
 *                operation was performed.
 *
 * \details       -
 *
 * \return        Number of warmup cycles since last clear
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_GetWarmUpCycleCountSinceClear(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestEventHasMilAssigned
 *****************************************************************************/
/*!
 * \brief         Returns if the event has the Mil indicator assigned.
 *
 * \details       Returns if the event has the Mil indicator assigned.
 *
 * \param[in]     EventId
 *                Unique handle of the event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The MIL is supported.
 *                FALSE
 *                The MIL is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEventHasMilAssigned(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestAnyObdRelevantEventConfirmed
 *****************************************************************************/
/*!
 * \brief         Checks if any OBD relevant event is confirmed.
 *
 * \details       Checks if any OBD relevant event is confirmed.
 *
 * \return        TRUE:  at least one event is confirmed
 *                FALSE: no event is confirmed
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestAnyObdRelevantEventConfirmed(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestNumOfConfirmedTransitionFlag
 *****************************************************************************/
/*!
 * \brief         Checks for a set or reset transition of the confirmed 
 *                event counter.
 *
 * \details       Checks if the confirmed event counter has transitioned from 
 *                either 0->1 or 1->0.
 *
 * \return        TRUE:  transition flag is set
 *                FALSE: transition flag is not set
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestNumOfConfirmedTransitionFlag(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_IncrementConfirmedEventsIfNeeded
 *****************************************************************************/
/*!
 * \brief         Increments the confirmed event counter.
 *
 * \details       Increments the confirmed counter if the given event is OBD 
 *                related in ZEVonUDS variant.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_IncrementConfirmedEventsIfNeeded(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_DecrementConfirmedEventsIfNeeded
 *****************************************************************************/
/*!
 * \brief         Decrements the confirmed event counter
 *
 * \details       Decrements the confirmed counter if the given event is OBD 
 *                related in ZEVonUDS variant.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_DecrementConfirmedEventsIfNeeded(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_IsDisableEventMonitorSupported
 *****************************************************************************/
/*!
 * \brief         Tests if disable event monitoring in current dcy is supported for
 *                the configuration.
 *
 * \details       Tests if disable event monitoring in current dcy is supported for
 *                the configuration.
 *
 * \return        TRUE
 *                Disable event monitoring in current dcy is supported.
 * \return        FALSE
 *                Disable event monitoring in current dcy is not supported.
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsDisableEventMonitorSupported(
  void
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
#include "Dem_GlobalDiagnostics_Types.h"
/*!
 * \defgroup Dem_GlobalDiagnosticsIF_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestAnyNewRingBufferSampleAvailable
 *****************************************************************************/
/*!
 * \brief         Checks whether atleast one new sample has been collected for any of the profile
 *
 * \details       -
 *
 * \return        TRUE, if at least one new sample has been collected in ring buffer(s)
 *                FALSE, no new sample available in ring buffer(s) since last refresh
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestAnyNewRingBufferSampleAvailable(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_CalculateNumberOfObdRelevantConfirmedDTCs
 *****************************************************************************/
/*!
 * \brief         Calculates the number of OBD relevant confirmed DTCs.
 *
 * \details       Calculates the number of OBD relevant confirmed DTCs by
 *                iterating over all events of primary memory.
 *
 * \return        Number of OBD relevant confirmed DTCs
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_CalculateNumberOfObdRelevantConfirmedDTCs(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_CountConfirmedEvents
 *****************************************************************************/
/*!
 * \brief         Counts how many ZEVonUDS-related events are confirmed
 *
 * \details       Counts how many ZEVonUDS-related events are confirmed
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_CountConfirmedEvents(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_InitReadinessEventIter
 *****************************************************************************/
/*!
 * \brief         Initialize the readiness event iterator.
 *
 * \details       Initialize the readiness event iterator used to iterate
 *                over all events which belong to the specified readiness group.
 *
 * \param[in]     ReadinessGroupId
 *                Unique identifier of the readiness group.
 * \param[out]    IterPtr
 *                Iterator for all events of a readiness group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_InitReadinessEventIter(
  Dem_EventOBDReadinessGroupType ReadinessGroupId,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ReadinessEventIterGetEventId
 *****************************************************************************/
/*!
 * \brief         Returns the Event Id from the current readiness event
 *                iterator.
 *
 * \details       -
 *
 * \param[in]     IterPtr
 *                Iterator for all events of a readiness group
 *
 * \return        EventId at the current index in the readiness event iterator.
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_GlobalDiagnostics_ReadinessEventIterGetEventId(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_J1939CalculateLampStatusOfEvent
 *****************************************************************************/
/*!
 * \brief         Calculate the event specific J1939 lamp status.
 *
 * \details       Calculate the event specific J1939 lamp status.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        J1939 lamp status for the requested event id.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_J1939DcmLampStatusType, DEM_CODE)
Dem_GlobalDiagnostics_J1939CalculateLampStatusOfEvent(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_J1939CalculateNodeRelatedLampStatus
 *****************************************************************************/
/*!
 * \brief         Calculate the node specific J1939 lamp status and return it.
 *
 * \details       Calculate the node specific J1939 lamp status based on red stop lamp,
 *                amber warning lamp, malfunction indicator lamp and the protect lamp.
 *
 * \param[in]     NodeIndex
 *                The node Id.
 * \return        The node specific J1939 lamp status.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_J1939DcmLampStatusType, DEM_CODE)
Dem_GlobalDiagnostics_J1939CalculateNodeRelatedLampStatus(
  uint8  NodeIndex
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TriggerReadinessCompletionRecalculation
 *****************************************************************************/
/*!
 * \brief         Trigger recalculation of completion status of the
 *                readiness group.
 *
 * \details       Trigger recalculation of completion status of the
 *                readiness group. Recalculation is deferred to main function.
 *
 * \param[in]     ReadinessGroupId
 *                Id of the readiness group.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_TriggerReadinessCompletionRecalculation(
  Dem_EventOBDReadinessGroupType ReadinessGroupId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ReadDataOfDM26
 *****************************************************************************/
/*!
 * \brief         Provides the readiness for diagnostic readiness 3
 *
 * \details       Provides the readiness for diagnostic readiness 3
 *
 * \param[out]    DestinationBuffer
 *                Destination buffer containing the readiness values for
 *                diagnostic readiness 3 bytes 4 to 8
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ReadDataOfDM26(
  Dem_Data_DestinationBufferPtrType DestinationBuffer
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ReadDataOfDM05
 *****************************************************************************/
/*!
 * \brief         Provides the diagnostic readiness
 *
 * \details       Provides the diagnostic readiness
 *
 * \param[out]    DestinationBuffer
 *                Destination buffer containing the diagnostic readiness values
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ReadDataOfDM05(
  Dem_Data_DestinationBufferPtrType  DestinationBuffer
  );


/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessOBDCycleRestart
 *****************************************************************************/
/*!
 * \brief         Process operations on OBD Driving Cycle Restart.
 *
 * \details       Process operations on OBD Driving Cycle Restart.
 *
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessOBDCycleRestart(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_UpdateObdPidAging
 *****************************************************************************/
/*!
 * \brief         Processes aging of OBD global statistics
 *
 * \details       Processes aging of OBD global statistics
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_UpdateObdPidAging(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessDCYEnd
 *****************************************************************************/
/*!
 * \brief         Processes a driving cycle stop trigger
 *
 * \details       Updates WWH OBD statistics on a driving cycle stop trigger
 *
 * \pre           Event specific trigger on Operation cycle end are already 
 *                processed
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessDCYEnd(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessWUCStart
 *****************************************************************************/
/*!
 * \brief         Processes a warm up cycle start trigger.
 *
 * \details       Updates the number of warmup cycles since last clear operation.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessWUCStart(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessWUCEnd
 *****************************************************************************/
/*!
 * \brief         Processes a warm up cycle stop trigger
 *
 * \details       Updates WWH OBD statistics on a warm up cycle stop trigger
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessWUCEnd(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessEventCleared
 *****************************************************************************/
/*!
 * \brief         Notifies GlobalDiagnostics about the reset of an Event status.
 *
 * \details       Notifies GlobalDiagnostics about the reset of an Event status.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     OldEventStatus
 *                Old Event status before clear
 * 
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventCleared(
  Dem_EventIdType  EventId,
  Dem_UdsStatusByteType OldEventStatus
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessEventDisabled
 *****************************************************************************/
/*!
 * \brief         Sets an event disabled for readiness group calculation.
 *
 * \details       Sets an events readiness group to disabled.
 *
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        E_OK
 *                Disabling the event was successful.
 * \return        E_NOT_OK
 *                Disabling the event was not successful.
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventDisabled(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessSynchronousEffectsOfEventAvailabilityChange
 *****************************************************************************/
/*!
 * \brief         Process synchronous event effects on event availability change.
 *
 * \details       Process synchronous event effects on event availability change.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     Available
 *                Availability of the event.
 * 
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessSynchronousEffectsOfEventAvailabilityChange(
  Dem_EventIdType  EventId,
  boolean Available
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessAsyncEffectsOfEventAvailabilityChange
 *****************************************************************************/
/*!
 * \brief         Process asynchronous event effects on event availability change.
 *
 * \details       Process asynchronous event effects on event availability change.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     Available
 *                Availability of the event.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessAsyncEffectsOfEventAvailabilityChange(
  Dem_EventIdType  EventId,
  boolean Available
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessEventPassed
 *****************************************************************************/
/*!
 * \brief         Notifies GlobalDiagnostics about a passed event report.
 *
 * \details       Notifies GlobalDiagnostics about a passed event report. Internally,
 *                the activation mode is notified about the passed report
 *                in order to further process the effects.
 *
 * \param[in]     EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventPassed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessEventPDTCDisabled
 *****************************************************************************/
/*!
 * \brief         Process the effects of a Pending bit transition to OFF.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventPDTCDisabled(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessEventTPSLCEnabled
 *****************************************************************************/
/*!
 * \brief         Process the effects of a TPSLC bit transition to ON.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * 
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventTPSLCEnabled(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_IsEventAndActivationMode4Active
 *****************************************************************************/
/*!
 * \brief          Tests if given event has active state and activation mode 4 
 *                 is active also.
 *
 * \details       Tests if given event is active (failed and qualified confirmed)
 *                and the activation mode 4 is active.
 *
 * \param[in]     EventId
 *                Unique handle of the event
 *
 * \return        TRUE: The given event is active and either this leads to 
 *                      activation mode 4 being active or activation mode 4 
 *                      is active because of the B1Counter exceeding 200h
 *                FALSE: - Event is not active or 
 *                       - Event is active but neither is of Severity Class A 
 *                         nor is the B1 counter exceeding 200h
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsEventAndActivationMode4Active(
  Dem_EventIdType  EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessEventWirEnabled
 *****************************************************************************/
/*!
 * \brief        Process the effects of a WIR bit transition to ON.
 *
 * \details      -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventWirEnabled(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessEventWirDisabled
 *****************************************************************************/
/*!
 * \brief         Process the effects of a WIR bit transition to OFF.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.        
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventWirDisabled(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_PreInit
 *****************************************************************************/
/*!
 * \brief         Performs preinitalization for the Global Diagnostic 
 *                logical unit.
 *
 * \details       Performs preinitalization for the Global Diagnostic 
 *                logical unit.
 *
 * \pre           Can only be called during Dem preinitialization.
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_PreInit(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Init
 *****************************************************************************/
/*!
 * \brief         Performs initialization for the Global Diagnostic logical unit.
 *
 * \details       Performs initialization for the Global Diagnostic logical unit.
 *
 * \pre           Can only be called during Dem initialization.
 *                For an overview of which other units must be initialized before,
 *                see DSGN-Dem-GlobalDiagnosticsInitDependencies
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Init(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_InitAdminData
 *****************************************************************************/
/*!
 * \brief         Performs the initialization of admin data.
 *
 * \details       Performs the initialization of the admin data managed by the
 *                GlobalDiagnostics unit.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_InitAdminData(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_InitStatusData
 *****************************************************************************/
/*!
 * \brief         Performs the initialization of status data.
 *
 * \details       Performs the initialization of status data managed by the
 *                GlobalDiagnostics unit.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_InitStatusData(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessDcyOfEventQualified
 *****************************************************************************/
/*!
 * \brief         Process OBD DCY qualification
 *
 * \details       Process OBD DCY qualification for single event
 *
 * \param[in]     EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessDcyOfEventQualified(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessEventFailed
 *****************************************************************************/
/*!
 * \brief         Process Event Failed report
 *
 * \details       Process triggers resulting of an Event Failed report
 *
 * \param[in]     EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventFailed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessEventOpCycleEnd
 *****************************************************************************/
/*!
 * \brief         Process Event Operation Cycle End
 *
 * \details       Process updates resulting from an Event Operation Cycle End
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventOpCycleEnd(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessAfterClear
 *****************************************************************************/
/*!
 * \brief         Reset global diagnostics after clear
 *
 * \details       Reset global diagnostics after clear
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessAfterClear(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessAfterClearAll
 *****************************************************************************/
/*!
 * \brief         Reset global diagnostics after clear all
 *
 * \details       Reset global diagnostics after clear all
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessAfterClearAll(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestEventEnableConditionsSatisfied
 *****************************************************************************/
/*!
 * \brief         Tests whether the enable conditions of an event are satisfied.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The enable conditions are satisfied
 * \return        FALSE
 *                The enable conditions are not satisfied
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEventEnableConditionsSatisfied(
  Dem_EventIdType  EventId
);

/* ***************************************************************************
 * Dem_GlobalDiagnostics_TestEnableConditionFulfilled
 *****************************************************************************/
/*!
 * \brief         Retrieves activation state of an enable condition.
 *
 * \details       Retrieves activation state of an enable condition.
 *
 * \param[in]     EnableConditionId
 *                Unique handle of the enable condition.
 *                The EnableCondition must be in range [0..Dem_GlobalDiagnostics_EnableCondition_GetGlobalCount()[.
 *
 * \return        TRUE
 *                The enable condition is fulfilled
 *                FALSE
 *                The enable condition is not fulfilled
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEnableConditionFulfilled(
  uint8 EnableConditionId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestEventStorageConditionsSatisfied
 *****************************************************************************/
/*!
 * \brief         Tests whether the storage conditions of an event are satisfied
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The storage conditions are satisfied
 * \return        FALSE
 *                The storage conditions are not satisfied
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEventStorageConditionsSatisfied(
  Dem_EventIdType  EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessTimers
 *****************************************************************************/
/*!
 * \brief         Processes timer calculations
 *
 * \details       This function processes global timers like engine run
 *                time. Must be called cyclically according to the configured
 *                cycle time.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessTimers(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_DisableEventMonitoring
 *****************************************************************************/
/*!
 * \brief         Enable disabled event monitor functionality
 *
 * \details       Enable disabled event monitor functionality
 *
 * \return        TRUE
 *
 * \return        FALSE
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_DisableEventMonitoring(
  Dem_EventIdType EventId
);


/* ****************************************************************************
 * Dem_GlobalDiagnostics_ResetDisabledEventMonitor
 *****************************************************************************/
/*!
 * \brief         Resets the disabled event monitor
 *
 * \details       Function shall be used to reset disabled event monitor when 
 *                changes occur to the event which directly affects the usage
 *                of the disabled event monitor feature.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ResetDisabledEventMonitor(
  Dem_EventIdType  EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetSamplingTime
 *****************************************************************************/
/*!
 * \brief         Get sampling time in ms
 *
 * \details       Get sampling time in ms for given sampling rate
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \return        Sampling time in ms
 * \config        -
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetSamplingTime(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
);
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_GLOBALDIAGNOSTICSIF_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_GlobalDiagnosticsIF_Interface.h
 *********************************************************************************************************************/
