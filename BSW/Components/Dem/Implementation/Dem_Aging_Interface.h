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
 *  \file       Dem_Aging_Interface.h
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

#if !defined (DEM_AGING_INTERFACE_H_)
#define DEM_AGING_INTERFACE_H_

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_Aging_Types.h"
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Aging_Properties Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Core_Aging_IsAgingForAllEventsSupported
 *****************************************************************************/
/*!
 * \brief         Check if aging is supported for all events
 *
 * \details       Check if aging is supported for all events
 *
 * \return        TRUE if every event can age (has an aging target cycle)
 * \return        FALSE if only stored events can age
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_IsAgingForAllEventsSupported(
  void
);

/* ****************************************************************************
 * Dem_Core_Aging_SingleStorage_TestAgingAfterHealingAllEventsSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether aging after healing for all events is supported.
 * 
 * \details       Tests whether aging after healing for all events is supported.
 *                This includes all events, event if no indicator is configured.
 * 
 * \return        TRUE
 *                Aging after healing for all DTCs is supported.
 * \return        FALSE
 *                Aging aftre healing for all DTCs is not suported.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_SingleStorage_TestAgingAfterHealingAllEventsSupported(
  void
);

/* ****************************************************************************
 * Dem_Core_Aging_SingleStorage_TestAgingAfterHealingSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether aging after healing non-OBD events is supported.
 * 
 * \details       Tests whether aging after healing non-OBD events is supported.
 *                This includes all events which have an indicator configured.
 * 
 * \return        TRUE
 *                Aging after healing for non-OBD events is supported.
 * \return        FALSE
 *                Aging aftre healing for non-OBD events is not suported.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_SingleStorage_TestAgingAfterHealingSupported(
  void
);

/* ****************************************************************************
 * Dem_Core_Aging_GetAgingTargetCycle
 *****************************************************************************/
/*!
 * \brief         Gets the cycle counter value at which the event will be aged.
 *
 * \details       Gets the aging target of the event. Depending on the configuration,
 *                the global aging counter or the event entry specific aging counter
 *                is returned.
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
 *
 * \return        Aging target cycle of the event.
 * \return        Invalid aging target cycle, if aging has not started yet or event is not
 *                stored and entry independent aging counters are not aupported.
 *
 * \pre           -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Core_Aging_GetAgingTargetCycle(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  );


/* ****************************************************************************
 * Dem_Core_Aging_IsAgingEntryAllocationSupported
 *****************************************************************************/
/*!
 * \brief         Check whether entry allocation for aging is supported.
 *
 * \details       -
 *
 * \return        TRUE
 *                Allocation of an event entry for aging is supported.
 * \return        FALSE
 *                Allocation of an event entry for aging is not supported.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_IsAgingEntryAllocationSupported(
  void
);

/* ****************************************************************************
 * Dem_Core_Aging_IsEntryRetentionAfterAgingSuppoted
 *****************************************************************************/
/*!
 * \brief         Check whether retention of event entries after aging is supported
 *                or not.
 *
 * \details       -
 *
 * \return        TRUE
 *                Event entries will be retained in memory after aging.
 * \return        FALSE
 *                Event entries will not be retained in memory after aging.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Aging_IsEntryRetentionAfterAgingSuppoted(
  void
);

/* ****************************************************************************
 * Dem_Core_Aging_GetAgingTarget
 *****************************************************************************/
/*!
 * \brief         Get the aging target configured for an event.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Number of cycles required to age the event.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Core_Aging_GetAgingTarget(
  Dem_EventIdType EventId
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Aging_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Core_Aging_Init
 *****************************************************************************/
/*!
 * \brief         Initializes the aging unit.
 *
 * \details       Initializes the aging unit. This function is only needed for
 *                initialization of the entry independent aging target cycle
 *                counters.
 *
 * \pre           Function must be called during init sequence before
 *                Dem_OperationCycle_Init because aging is handled at operation
 *                cycle end and requires correct aging target cycle counters.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_Init(
  void
);

/* ****************************************************************************
 * Dem_Core_Aging_ProcessAgingCycleEnd
 *****************************************************************************/
/*!
 * \brief         Function recalculates the aging counter threshold
 *
 * \details       This function recalculates the aging counter threshold for
 *                all master events which are either confirmed or stored in
 *                memory based on the status. If the aging
 *                threshold for the event is reached, it is aged.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * 
 * \param[in]     CycleId
 *                Unique Id of the cycle
 * \param[in]     CycleCounter
 *                Current cycle count
 *
 * \pre           Cycle counter has already been incremented for the cycle.
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessAgingCycleEnd(
  Dem_EventIdType EventId
  );


/* ****************************************************************************
 * Dem_Core_Aging_ProcessAgingTimer
 *****************************************************************************/
/*!
 * \brief         Calculates the 200h aging timer
 *
 * \details       This function processes aging of WWH-OBD DTCs after 200h of
 *                engine run time if WwhObd is enabled.
 *
 * \pre           -
 *
 * \context       ISR
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessAgingTimer(
  void
);

/* ****************************************************************************
 * Dem_Core_Aging_ProcessOnDisplaced
 *****************************************************************************/
/*!
 * \brief         Reacts to a displacement on an event.
 *
 * \details       -
 *
 * \param[in]     MemoryRepresentativeEventId
 *                Unique handle of the Event in memory.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessOnDisplaced(
  Dem_EventIdType MemoryRepresentativeEventId
);

/* ****************************************************************************
 * Dem_Core_Aging_ProcessOnDisplaced
 *****************************************************************************/
/*!
 * \brief         Reacts to a clear operation on an event.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event in memory.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessOnClear(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Aging_ProcessOnFailed
 *****************************************************************************/
/*!
 * \brief         Reacts to a failed report on an event.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event in memory.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessOnFailed(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Aging_ProcessOnFdcTrip
 *****************************************************************************/
/*!
 * \brief         Reacts to a FDC Trip of an event.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event in memory.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessOnFdcTrip(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Aging_RestoreAgingDataOfEntry
 *****************************************************************************/
/*!
 * \brief         Restores aging related data.
 *
 * \details       Restores aging related data, such as stored status and 
 *                aging completion cycle count.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           Must be called during memory restoration.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_RestoreAgingDataOfEntry(
  Dem_Cfg_EntryIndexType EventEntryIndex
);

/* ****************************************************************************
 * Dem_Core_Aging_GetAgingStatus
 *****************************************************************************/
/*!
 * \brief         Function returns the current aging status of an event.
 *
 * \details       Function returns the current aging status of an event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     AgingTatgetCycle
 *                The cycle count in which the aging will be complete.
 *
 * \return        Current aging status of the event.
 *
 * \pre           -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Aging_StatusType, DEM_CODE)
Dem_Core_Aging_GetAgingStatus(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_Core_Aging_ProcessStartOnPassed
 *****************************************************************************/
/*!
 * \brief         Processes aging after a passed report
 *
 * \details       Depending on the configuration and aging starting conditions
 *                the aging process is started.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Healing is processed before calling this function.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessStartOnPassed(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Core_Aging_ProcessStartOnOPCycleEnd
 *****************************************************************************/
/*!
 * \brief         Processes aging after on operation cycle end.
 *
 * \details       Depending on the configuration and aging starting conditions
 *                the aging process is started.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Healing is processed before calling this function.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Aging_ProcessStartOnOPCycleEnd(
  Dem_EventIdType EventId
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif /* DEM_AGING_INTERFACE_H_ */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Aging_Interface.h
 *********************************************************************************************************************/

