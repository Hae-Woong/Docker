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
 *  \defgroup   Dem_EventIF Interface for Events IF
 *  \{
 *  \file       Dem_EventIF_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Public interface of the EventIF subcomponent
 *  \entity     EventIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EVENTIF_INTERFACE_H)
#define DEM_EVENTIF_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_EventIF_Types.h"

/* ------------------------------------------------------------------------- */
#include "Dem_DTC_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*!
 * \defgroup Dem_Event_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Core_Event_GetMaxFdcToc
 *****************************************************************************/
/*!
 * \brief         Returns the 'Maximum FDC This Operation Cycle' of the event.
 *
 * \details       If the event supports combination on storage, the 
 *                'Maximum FDC This Operation Cycle' of the combined group is
 *                returned. Otherwise it is retrieved from the single event.
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
DEM_LOCAL FUNC(sint8, DEM_CODE)
Dem_Core_Event_GetMaxFdcToc(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Event_SetEntryIndependentMaxFdcSlc
 *****************************************************************************/
/*!
 * \brief         Set the memory independent 'MaxFDCSinceLastClear' of the
 *                given event.
 *
 * \details       Set the memory independent 'MaxFDCSinceLastClear' of the
 *                given event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     FdcValue
 *                New value of the max fault detection counter since last clear
 *                of the event.
 *
 * \pre           'Maximum FDC Since Last Clear' value is stored for all DTCs
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Core_Event_SetEntryIndependentMaxFdcSlc(
  Dem_EventIdType EventId,
  sint8 FdcValue
);

/* ****************************************************************************
 * Dem_Core_Event_IsMemIndependentMaxFdcSlcSupported
 *****************************************************************************/
 /*!
  * \brief         Returns true if data element 'Max FDC Since Last Clear' is 
  *                stored independent of an event entry.
  * \details       -
  *
  * \return        TRUE
  *                'Max FDC Since Last Clear' stored independent of an event entry.
  * \return        FALSE
  *                'Max FDC Since Last Clear' only stored in event entry.
  *
  * \pre           -
  *
  * \context       TASK|ISR2
  * \synchronous   TRUE
  * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_IsMemIndependentMaxFdcSlcSupported(
  void
);

/* ****************************************************************************
 * Dem_Core_Event_GetMaxFdcSlc
 *****************************************************************************/
/*!
 * \brief         Gets the max FDC since last clear for given event.
 *
 * \details       Gets the max FDC since last clear for given event. Depending
 *                on the configuration the memory independent or memory
                  dependent max FDC SLC is returned.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * 
 * \return        Max fault detionction counter since last clear of event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_Core_Event_GetMaxFdcSlc(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Event_UpdateMaxFdcSlc
 *****************************************************************************/
/*!
 * \brief         Update 'Maximum FDC Since Last Clear' of the event.
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
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_UpdateMaxFdcSlc(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Event_GetInternalUdsStatus
 *****************************************************************************/
/*!
 * \brief         Returns the interally used UDS status of the event.
 *
 * \details       The UDS status of the event is returned if event can be stored
 *                (Single events and combination on retrieval). For combination
 *                on storage, the UDS status of the combined group is returned.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The status byte relevant for the event.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Core_Event_GetInternalUdsStatus(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Core_Event_TryResetQualifiedConfirmedSinceLastClear
 *****************************************************************************/
/*!
 * \brief         Reset the qualified confirmed since last clear bit of extended
 *                event status.
 *
 * \details       Reset the qualified confirmed since last clear bit of extended
 *                event status, if Event is not confirmed (externally visible)
 *                currently
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 *                
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_TryResetQualifiedConfirmedSinceLastClear(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Event_GetActivationTimerIndex
 *****************************************************************************/
/*!
 * \brief         Get the PTO activation timer index for the event.
 *
 * \details       Get the PTO activation timer index for the event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The PTO activation timer index of the event.
 *                
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Pto_ActivationTimer_TimerIdType, DEM_CODE)
Dem_Core_Event_GetActivationTimerIndex(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Core_Event_HasConfiguredOrCalculatedSRecs
 *****************************************************************************/
 /*!
 * \brief         Tests if event has Snapshot Records configured.
 *
 * \details       Tests if event has Snapshot Records of enumertaion type
 *                Configured, Calculated or Calculated FIFO configured.
 *
 * \return        TRUE
 *                Event has Snapshot Records configured.
 * \return        FALSE
 *                Event does not have Snapshot Records configured.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_HasConfiguredOrCalculatedSRecs(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Event_IsSimilarConditionsEnabled
 *****************************************************************************/
/*!
 * \brief         Returns true if feature similar condition is enabled.
 * \details       -
 *
 * \return        TRUE
 *                Supported
 * \return        FALSE
 *                Not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsSimilarConditionsEnabled(
  void
);


/* ****************************************************************************
 * Dem_Event_TestValidHandle
 *****************************************************************************/
/*!
 * \brief         Test whether a handle is a valid event ID
 *
 * \details       This function tests whether a handle is in the valid range
 *                of EventIds. This function does not consider calibration or
 *                runtime-availability.
 *
 * \param[in]     EventId
 *                Handle to test against range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                EventId is valid
 * \return        FALSE
 *                EventId is not valid
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestValidHandle(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

 /* ****************************************************************************
 * Dem_Event_IsSlaveEvent
 *****************************************************************************/
 /*!
 * \brief         Check whether the event is configured as the slave event in a
 *                multievent triggering.
 * 
 * \details       Check whether the event is configured as the slave event in a
 *                multievent triggering.
 *
 * \param[in]     EventId
 *                Global Event Id
 *
 * \return        TRUE
 *                Is slave event
 * 
 * \return        FALSE
 *                Is not slave event
 *
 * \pre           -
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsSlaveEvent(
  Dem_EventIdType EventId                                                                                                        /* PRQA S 3206 */ /* MD_DEM_3206 */
);

 /* ****************************************************************************
 * Dem_Event_IsMasterEvent
 *****************************************************************************/
 /*!
 * \brief         Check whether the event is configured as the master event in a
 *                multievent triggering.
 * 
 * \details       Check whether the event is configured as the master event in a
 *                multievent triggering.
 *
 * \param[in]     EventId
 *                Global Event Id
 *
 * \return        TRUE
 *                Is master event
 * 
 * \return        FALSE
 *                Is not master event
 *
 * \pre           -
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsMasterEvent(
  Dem_EventIdType EventId                                                                                                        /* PRQA S 3206 */ /* MD_DEM_3206 */
);

/* ****************************************************************************
 * Dem_Event_TestDebounceCounterStoredInNv
 *****************************************************************************/
/*!
 * \brief         Test if the event stores the debounce counter in NV
 *
 * \details       Test if the event stores the debounce counter in NV
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event stores the debounce counter in NV
 * \return        FALSE
 *                The event does not stores the debounce counter in NV
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestDebounceCounterStoredInNv(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );


/* ****************************************************************************
 * Dem_Event_TestObdVisibilityDelayed
 *****************************************************************************/
/*!
 * \brief         Returns if the external visibility of OBD CDTC / WIR status
 *                bits (and all related changes in visibility) is delayed until
 *                DCY qualification.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                External visibility is delayed until DCY qualification.
 *                FALSE
 *                Immediate visibility.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestObdVisibilityDelayed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

#if (DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON)
/* ****************************************************************************
 * Dem_Event_GetTripCount
 *****************************************************************************/
/*!
 * \brief         Get current trip count of the event
 *
 * \details       Get current trip count of the event
 *
 * \param[in]     EventId
 *                Unique handle of the event
 *
 * \return        Current trip count of the event
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_GetTripCount(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );
#endif

/* ****************************************************************************
 * Dem_Core_Event_GetHealingCounter
 *****************************************************************************/
/*!
 * \brief         Get healing counter for the event
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the event
 *
 * \return        Current healing counter for the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Core_Event_GetHealingCounter(
  Dem_EventIdType EventId
  );

#if (DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON)
/* ****************************************************************************
 * Dem_Event_SetTripCount
 *****************************************************************************/
/*!
 * \brief         Set current trip count of the event
 *
 * \details       Set current trip count of the event
 *
 * \param[in]     EventId
 *                Unique handle of the event
 * \param[in]     TripCount
 *                New trip count of the event
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_SetTripCount(
  CONST(Dem_EventIdType, AUTOMATIC) EventId,
  CONST(uint8, AUTOMATIC) TripCount
  );
#endif

/* ****************************************************************************
 * Dem_Core_Event_UpdateTrigger_TestConfirmed
 *****************************************************************************/
 /*!
  * \brief         Test if bit for CDTC update is set
  *
  * \details       Test if bit for CDTC update is set
  *
  * \param[in]     EventTrigger
  *                Bitmask for event update trigger
  *
  * \pre           -
  *
  * \context       TASK
  * \synchronous   TRUE
  * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_UpdateTrigger_TestConfirmed(
  CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC) EventTrigger
);

/* ****************************************************************************
 * Dem_Core_Event_UpdateTrigger_TestWIR
 *****************************************************************************/
 /*!
  * \brief         Test if bit for WIR update is set
  *
  * \details       Test if bit for WIR update is set
  *
  * \param[in]     EventTrigger
  *                Bitmask for event update trigger
  *
  * \pre           -
  *
  * \context       TASK
  * \synchronous   TRUE
  * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_UpdateTrigger_TestWIR(
  CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC) EventTrigger
);

/* ****************************************************************************
 * Dem_Core_Event_UpdateTrigger_TestFailed
 *****************************************************************************/
 /*!
  * \brief         Test if bit for TestFailed update is set
  *
  * \details       Test if bit for TestFailed update is set
  *
  * \param[in]     EventTrigger
  *                Bitmask for event update trigger
  *
  * \pre           -
  *
  * \context       TASK
  * \synchronous   TRUE
  * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_UpdateTrigger_TestFailed(
  CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC) EventTrigger
);

/* ****************************************************************************
 * Dem_Core_Event_UpdateTrigger_TestPassed
 *****************************************************************************/
 /*!
  * \brief         Test if bit for Passed update is set
  *
  * \details       Test if bit for Passed update is set
  *
  * \param[in]     EventTrigger
  *                Bitmask for event update trigger
  *
  * \pre           -
  *
  * \context       TASK
  * \synchronous   TRUE
  * \reentrant     TRUE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_UpdateTrigger_TestPassed(
  CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC) EventTrigger
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Event_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Event_GetDataCommitState
 *****************************************************************************/
/*!
 * \brief         Get the update state for readable data of this event.
 *
 * \details       Get the update state for readable data of this event.
 *
 * \param[in]     EventId
 *                Index in update state array
 *
 * \return        The update state of event's readable data.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_GetDataCommitState(
  Dem_EventIdType EventId
);

#if (DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON)
/* ****************************************************************************
 * Dem_Event_GetAvailableMask
 *****************************************************************************/
/*!
 * \brief         Get the event available mask at position 'index'
 *
 * \details       Get the event available mask at position 'index'.
 *
 * \param[in]     Index
 *                Index of the available mask
 *
 * \return        The event available mask at position 'index'
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_GetAvailableMask(
  CONST(uint16, AUTOMATIC) Index
  );
#endif

#if (DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON)
/* ****************************************************************************
 * Dem_Event_SetAvailableMask
 *****************************************************************************/
/*!
 * \brief         Set the event available mask at position 'index'.
 *
 * \details       Set the event available mask at position 'index'.
 *
 * \param[in]     Index
 *                Index of the event available mask
 * \param[in]     Mask
 *                New event available mask at position 'index'
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_SetAvailableMask(
  CONST(uint16, AUTOMATIC) Index,
  CONST(uint8, AUTOMATIC) Mask
  );
#endif

/* ****************************************************************************
 * Dem_Event_TestDataUpdateInProgress
 *****************************************************************************/
/*!
 * \brief         Test commit started state of event data update transaction.
 *
 * \details       Test commit started state of event data update transaction.
 *
 * \param[in]     The update state of the event data.
 *
 * \return        TRUE if commit has started.
 * \return        FALSE if commit has not started.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestDataUpdateInProgress(
  CONST(uint8, AUTOMATIC)  DataState
  );


/* ****************************************************************************
 * Dem_Event_DataUpdateStart
 *****************************************************************************/
/*!
 * \brief         Mark update started for the event.
 *
 * \details       Mark update started for the event.
 *
 * \param[in]     EventId
 *                Index in update state array
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_DataUpdateStart(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_DataUpdateFinish
 *****************************************************************************/
/*!
 * \brief         Mark update finished for the event.
 *
 * \details       Mark update finished for the event.
 *
 * \param[in]     EventId
 *                Index in update state array
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_DataUpdateFinish(
  Dem_EventIdType EventId
  );

#if ((DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) \
    && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF))
/* ****************************************************************************
 * Dem_Event_UpdateQualificationStatus
 *****************************************************************************/
/*!
 * \brief         Persist the newest qualification status of event to memory.
 *
 * \details       Persist the newest qualification status of event to memory.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           -
 *
 * \config        (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
 *                 && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Event_UpdateQualificationStatus(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif

/* ****************************************************************************
 * Dem_Core_Event_TestHealingPreventsAging
 *****************************************************************************/
/*!
 * \brief         Tests whether the healing process currently prevents the start
 *                of aging.
 *
 * \details       Tests whether the healing process currently prevents the start
 *                of aging. Depending on the configuration aging might be allowed
 *                during the healing process or not.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event may not age
 * \return        FALSE
 *                The event may age
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Core_Event_TestHealingPreventsAging(
  Dem_EventIdType EventId
  );


#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 * Dem_Event_Disconnect_Normal
 *****************************************************************************/
/*!
 * \brief         Marks a normal event unavailable
 *
 * \details       This function sets the event unavailable if it is not stored.
 *
 *                Do not call directly, use Dem_Event_Disconnect instead.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Must be called in critical section DiagMonitor
 * \config        DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Disconnect_Normal(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 * Dem_Event_Disconnect
 *****************************************************************************/
/*!
 * \brief         Marks an event unavailable
 *
 * \details       This function sets an event unavailable, if its state allows
 *                to do so.
 *
 *                In case the event is set unavailable, its event status byte
 *                is changed to 0x00 and the FiM is informed.
 *
 *                Otherwise, this function returns E_NOT_OK.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        E_OK
 *                The event was set to unavailable
 * \return        E_NOT_OK
 *                The event was not set to unavailable
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Event_Disconnect(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 * Dem_Event_Reconnect_Normal
 *****************************************************************************/
/*!
 * \brief         Marks a normal event available
 *
 * \details       This function sets the event available.
 *
 *                Do not call directly, use Dem_Event_Reconnect instead.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Reconnect_Normal(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

/* ****************************************************************************
 * Dem_Event_TestEventAvailable
 *****************************************************************************/
/*!
 * \brief         Tests if an Event is available based on configuration variant
 *                and runtime calibration
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event is available
 * \return        FALSE
 *                The event is not available
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestEventAvailable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Event_TestEventRuntimeAvailable
 *****************************************************************************/
/*!
 * \brief         Tests if an Event is available based on runtime calibration
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event is available
 * \return        FALSE
 *                The event is not available
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestEventRuntimeAvailable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Event_TestSuppressionOnEventRetrieval
 *****************************************************************************/
/*!
 * \brief         Tests if an event can be considered for reporting API
 *
 * \details       Tests all options due to which an event has to be excluded
 *                from reporting API, based on an individual event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event is currently suppressed
 * \return        FALSE
 *                The event is currently not suppressed
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestSuppressionOnEventRetrieval(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );


/* ****************************************************************************
 * Dem_Event_TestEventUnavailable
 *****************************************************************************/
/*!
 * \brief         Tests if an event is unavailable
 *
 * \details       Tests all options due to which an event can be set
 *                unavailable, based on an individual event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event is currently unavailable
 * \return        FALSE
 *                The event is currently available
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestEventUnavailable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

#if (DEM_FEATURE_NEED_EVENTAVAILABLE_DEFAULT == STD_ON) \
 || (DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON)
/* ****************************************************************************
 * Dem_Event_SetDisconnectedBit
 *****************************************************************************/
/*!
 * \brief         Set an event's disconnected bit
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     IsDisconnected
 *                TRUE: Set disconnected bit to TRUE
 *                FALSE: Set disconnected bit to FALSE
 *
 * \pre           Must be called within critical section DiagMonitor
 * \config        (DEM_FEATURE_NEED_EVENTAVAILABLE_DEFAULT == STD_ON)
 *                || (DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON)
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_SetDisconnectedBit(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)  IsDisconnected
  );
#endif

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 * Dem_Event_MayDisconnect
 *****************************************************************************/
/*!
 * \brief         Tests precondition for event disconnect
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \return        TRUE
 *                The event may be disconnected
 * \return        FALSE
 *                The event may not be disconnected
 * \pre           Must be called within critical section DiagMonitor
 * \config        DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_MayDisconnect(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 * Dem_Event_Reconnect
 *****************************************************************************/
/*!
 * \brief         Marks an event available
 *
 * \details       This function sets an event available.
 *
 *                In case the event is set available, its event status byte is
 *                changed to 0x50 and the FiM is informed.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Reconnect(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
/* ****************************************************************************
 * Dem_Event_ProcessFdc
 *****************************************************************************/
/*!
 * \brief         Process the event's FDC trip actions
 *
 * \details       This function processes the stored triggers from debounce
 *                values passing the FDC trip threshold.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     FdcThresholdExceeded
 *                FDC Storage threshold was exceeded
 * \param[in]     FdcMaximumExceeded
 *                FDC Maximum (this cycle) was exceeded
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ProcessFdc(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)  FdcThresholdExceeded,
  CONST(boolean, AUTOMATIC)  FdcMaximumExceeded
  );
#endif

/* ****************************************************************************
 * Dem_Event_Passed
 *****************************************************************************/
/*!
 * \brief         Process the event's queued qualified passed result
 *
 * \details       This function processes a passed result stored on the
 *                event queue, for events using the QUEUED mechanism. The
 *                triggers resulting from the status bit changes are calculated
 *                within this function, based on the current event status.
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Event_Passed(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_Failed
 *****************************************************************************/
/*!
 * \brief         Process the event's queued qualified failed result
 *
 * \details       This function processes a failed result stored on the
 *                event queue.
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Event_Failed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Event_OpCycle
 *****************************************************************************/
/*!
 * \brief         Updates an event's state due to operation cycle change
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in,out] ArgPtr
 *                Arguments: CycleStatus - encoding of cycle modification
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Event_OpCycle(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  );

/* ****************************************************************************
 * Dem_Event_ResetTestFailed
 *****************************************************************************/
/*!
 * \brief         Resets the 'TestFailed' bit
 *
 * \details       Resets the 'TestFailed' bit
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different events
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ResetTestFailed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Event_Aged
 *****************************************************************************/
/*!
 * \brief         Updates an event due to aging
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in,out] ArgPtr
 *                Arguments: none
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Aged(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  );

/* ****************************************************************************
 * Dem_Event_ProcessCleared
 *****************************************************************************/
/*!
 * \brief         Clears an event.
 *
 * \details       Invokes the 'Clear allowed' callbacks, clears an event,
 *                and invokes the 'init monitor for event' notifications.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Event has been cleared.
 * \return        FALSE
 *                Event is not allowed to be cleared.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_ProcessCleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Event_Cleared
 *****************************************************************************/
/*!
 * \brief         Clears an event's state in volatile RAM
 *
 * \details       Clears an event's state in volatile RAM
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in,out] ArgPtr
 *                Arguments: none
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Cleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  );

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) \
 && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
/* ****************************************************************************
 * Dem_Event_Displaced
 *****************************************************************************/
/*!
 * \brief         Updates an event's status due to displacement
 *
 * \details       Do not call this function directly, it is part of
 *                Dem_DTC_Displaced
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in,out] ArgPtr
 *                Arguments: none
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF
 *             && DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Displaced(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  );
#endif

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/* ****************************************************************************
 * Dem_Event_CustomizedDisplacement
 *****************************************************************************/
/*!
 * \brief         Updates an event's status due to displacement
 *
 * \details       Do not call this function directly, it is part of
 *                Dem_DTC_CustomizedDisplacement
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in,out] ArgPtr
 *                Arguments: SI30 - Status Indicator byte
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CustomizedDisplacement(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  );
#endif

/* ****************************************************************************
 * Dem_Event_GetEventUDSStatus
 *****************************************************************************/
/*!
 * \brief         Gets the current event status
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \param[out]    EventUDSStatus
 *                Pointer receiving the event Uds status
 *
 * \return        E_OK
 *                EventUDSStatus now contains the event status
 *
 * \return        E_NOT_OK
 *                The request was rejected due to variant coding
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Event_GetEventUDSStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONSTP2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA)  EventUDSStatus
  );

/* ****************************************************************************
 * Dem_Event_GetEventFailed
 *****************************************************************************/
/*!
 * \brief         Gets Bit0 (TestFailed) of the current event status
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \param[out]    EventFailed
 *                Pointer receiving Bit0
 *
 * \return        E_OK
 *                EventFailed now contains Bit0
 *
 * \return        E_NOT_OK
 *                The request was rejected due to variant coding
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Event_GetEventFailed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONSTP2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  EventFailed
  );

/* ****************************************************************************
 * Dem_Event_GetEventTested
 *****************************************************************************/
/*!
 * \brief         Gets Bit6 (TestNotCompletedThisOperationCycle) of the current event status
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event
 *
 * \param[out]    EventTested
 *                Pointer receiving Bit6
 *
 * \return        E_OK
 *                EventTested now contains Bit6
 *
 * \return        E_NOT_OK
 *                The request was rejected due to variant coding
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Event_GetEventTested(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONSTP2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  EventTested
  );

#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
/* ****************************************************************************
 * Dem_Event_CycleQualifyDcy
 *****************************************************************************/
/*!
 * \brief         Updates an event's state due to dcy qualification
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in,out] ArgPtr
 *                Arguments: none
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_EMISSIONOBD == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Event_CycleQualifyDcy(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  );
#endif

#if ((DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) \
    && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF) \
    && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Event_ProcessDCYQualificationCombinedDTC_StoredOnly
 *****************************************************************************/
/*!
 * \brief         Function recalculates the qualification state of WIR and
 *                Confirmed bits for a combined event in a configuration with
 *                CDTC processing set as 'STORED_ONLY'.
 *
 * \details       Function recalculates the qualification state of WIR and
 *                Confirmed bits for a combined event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     ArgPtr
 *                Arguments passed to the operation.
 *
 * \pre           -
 *
 * \config        (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
 *                && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ProcessDCYQualificationCombinedDTC_StoredOnly(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  );
#endif

#if ((DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) \
    && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF) \
    && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 * Dem_Event_ProcessDCYQualificationNonCombinedDTC_StoredOnly
 *****************************************************************************/
/*!
 * \brief         Function recalculates the qualification state of WIR and
 *                Confirmed bits for a normal event in a
 *                configuration with CDTC processing set as 'STORED_ONLY'.
 *
 * \details       Function recalculates the qualification state of WIR and
 *                Confirmed bits for a normal event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     ArgPtr
 *                Arguments passed to the operation.
 *
 * \pre           -
 *
 * \config        (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
 *                && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF)
 *                && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
 *
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ProcessDCYQualificationNonCombinedDTC_StoredOnly(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  Dem_DTC_ForEachCombinedArgPtrType ArgPtr
  );
#endif

/* ****************************************************************************
 * Dem_Event_TestLatchedTestFailed
 *****************************************************************************/
/*!
 * \brief         Tests whether an event's latched failed restriction applies
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event is latched at TestFailed
 * \return        FALSE
 *                The event is not latched, process it normally
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestLatchedTestFailed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Event_InitNvData
 *****************************************************************************/
/*!
 * \brief         Initializes the event status processing sub-component
 *
 * \details       Initializes the event status processing sub-component
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           May be called only during module initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InitNvData(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Event_Init
 *****************************************************************************/
/*!
 * \brief         Initializes the event status processing sub-component
 *
 * \details       Initializes the event status processing sub-component
 *
 * \pre           May be called only during module initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Init(
  void
  );

/* ****************************************************************************
 * Dem_Event_PreInit
 *****************************************************************************/
/*!
 * \brief         Pre-Initializes the Dem events
 *
 * \details       Pre-Initializes the Dem events
 *
 * \pre           May be called only during module pre-initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_PreInit(
  void
  );

/* ****************************************************************************
 * Dem_Event_MainFunction
 *****************************************************************************/
/*!
 * \brief         Process queued event availability changes
 *
 * \details       Process queued event availability changes
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_MainFunction(
  void
  );

/* ****************************************************************************
 * Dem_Event_GetRatioIdx
 *****************************************************************************/
/*!
 * \brief         Get the ratio index of an event
 *
 * \details       Get the ratio index of an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Handle to the ratio configuration for an event
 *
 * \pre           The event must have a ratio configured. Feature IUMPR is supported.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_RatioIdIndexType, DEM_CODE)
Dem_Event_GetRatioIdx(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

/* ****************************************************************************
 * Dem_Event_GetManIdx
 *****************************************************************************/
/*!
 * \brief         Get the MAR index of an event
 *
 * \details       Get the MAR index of an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Index to the MAN of the event.
 *
 * \pre           The event must have a ratio configured.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, DEM_CODE)
Dem_Event_GetManIdx(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

/* ****************************************************************************
 * Dem_Event_GetSmarIdx
 *****************************************************************************/
/*!
 * \brief         Get the SMAR index of an event
 *
 * \details       Get the SMAR index of an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Index to the SMAR of the events DTC.
 *
 * \pre           The event must have a ratio configured.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType, DEM_CODE)
Dem_Event_GetSmarIdx(
  CONST(Dem_EventIdType, AUTOMATIC) EventId                                                                                      /* PRQA S 3206 */ /* MD_DEM_3206 */
);

/* ****************************************************************************
 * Dem_Event_ResetEventReporting
 *****************************************************************************/
/*!
 * \brief         Reset debouncing and queue status
 *
 * \details       Reset debouncing and queue status to enable 
 *                correct event reporting/ debouncing for events. This function shall
 *                be invoked during the start of the event's operation cycle/ during 
 *                shutdown.
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
Dem_Event_ResetEventReporting(
  Dem_EventIdType  EventId
);

/* ****************************************************************************
 * Dem_Event_InitUdsFromStatusIndicator
 *****************************************************************************/
/*!
 * \brief         Initializes event status using the status indicator
 *
 * \details       Initializes event status using the status indicator
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()].
 *
 * \pre           May be called only during module initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InitUdsFromStatusIndicator(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Event_QualifyStatus
 *****************************************************************************/
/*!
 * \brief           Process qualification status of an event.
 *
 * \details         In case the DCY is not already qualified, this masks the
 *                  confirmed and WIR bits of the event status. These are
 *                  restored once the DCY qualifies later.
 *
 * \param[in,out]   EventContext
 *                  Event processing context
 *
 * \pre             Feature OBD or WWH-OBD is enabled
 *
 * \context         TASK
 * \synchronous     TRUE
 * \reentrant       TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_QualifyStatus(
  CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  );

/* ****************************************************************************
 * Dem_Core_Event_GetDestination
 *****************************************************************************/
/*!
 * \brief         Get the memory an event belongs to
 *
 * \details       Get the memory an event belongs to
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Unique identifier of the event memory
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Core_Event_GetDestination(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Event_GetObdOnUds3ByteDtc
 *****************************************************************************/
/*!
 * \brief         Get the OBDonUDS 3-Byte DTC of the event.
 *
 * \details       Get the OBDonUDS 3-Byte DTC of the event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The OBDonUDS 3-Byte DTC of the event
 *                DEM_CFG_DTC_OBD_3BYTE_INVALID if the event does not have a OBDonUDS 3-Byte DTC
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Core_Event_GetObdOnUds3ByteDtc(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Event_IndicatorIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate indicator descriptors
 *                configured for an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_IndicatorIterInit(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Core_Event_TestHasIndicator
 *****************************************************************************/
/*!
 * \brief         Test, if the event has configured any indicator.
 *
 * \details       Test, if the event has configured any indicator.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \return        TRUE
 *                The event has configured a user- or a special indicator.
 * \return        FALSE
 *                The event has no indicators configured.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_TestHasIndicator(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Core_Event_MultiEventIterInit
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate slave events in a 
 *                multi event.
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_MultiEventIterInit(
  Dem_EventIdType EventId,                                                                                                       /* PRQA S 3206 */ /* MD_DEM_3206 */
  Dem_Cfg_ComplexIterType * const IterPtr                                                                                        /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  );

/* ****************************************************************************
 % Dem_Core_Event_MultiEventIterGet
 *****************************************************************************/
/*!
 * \brief         Get the slave event pointed to by the iterator
 *
 * \details       Get the slave event pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to a data element descriptor
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Core_Event_MultiEventIterGet(                                                                                                /* PRQA S 3206 */ /* MD_DEM_3206 */
  Dem_Cfg_ComplexIterType * const IterPtr                                                                                        /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  );

/* ****************************************************************************
 * Dem_Event_IsCalculatedSRecsUsed
 *****************************************************************************/
/*!
 * \brief         Test whether the memory associated with the event supports Calculated Snapshot Record Enumeration.
 *
 * \details       Test whether the memory associated with the event supports Calculated Snapshot Record Enumeration.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        True if event memory supports Calculated Snapshot Record Enumeration, otherwise false.
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsCalculatedSRecsUsed(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_IsCalculatedFifoSRecsUsed
 *****************************************************************************/
/*!
 * \brief         Test whether the memory associated with the event supports Calculated FIFO Snapshot Record Enumeration.
 *
 * \details       Test whether the memory associated with the event supports Calculated FIFO Snapshot Record Enumeration.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        True if event memory supports Calculated FIFO Snapshot Record Enumeration, otherwise false.
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsCalculatedFifoSRecsUsed(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_IsConfiguredSRecsUsed
 *****************************************************************************/
/*!
 * \brief         Test whether the memory associated with the event supports Configured Snapshot Record Enumeration.
 *
 * \details       Test whether the memory associated with the event supports Configured Snapshot Record Enumeration.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        True if event memory supports Configured Snapshot Record Enumeration, otherwise false.
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsConfiguredSRecsUsed(
  Dem_EventIdType EventId
  );


/* ****************************************************************************
 * Dem_Core_Event_TestEventAffectedByPto
 *****************************************************************************/
/*!
 * \brief         Tests if an Event is affected by PTO
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event is affected by PTO
 * \return        FALSE
 *                The event is not affected by PTO/ PTO is not enabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_TestEventAffectedByPto(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_Core_Event_ProcessOBDCycleEnd
 *****************************************************************************/
/*!
 * \brief         Process end of Obd relevant operation cycles
 *
 * \details       Process end of driving cycle
 *
 * \param[in]     CycleId
 *                Unique Id of the cycle
 *
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_ProcessOBDCycleEnd(
  uint8 CycleId
  );

/* ****************************************************************************
 * Dem_Core_Event_IsCustomTriggeredSrecConfigured
 *****************************************************************************/
/*!
 * \brief         Test whether event has custom triggered snapshot record configured.
 *
 * \details       Test whether event has custom triggered snapshot record configured.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        True if custom triggered snapshot recrod is configured for event
 *                False, otherwise.
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_IsCustomTriggeredSrecConfigured(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Core_Event_IsCustomTriggeredTssrConfigured
 *****************************************************************************/
/*!
 * \brief         Test whether event has custom trigger time series record configured.
 *
 * \details       Test whether event has custom trigger time series record configured.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        True if custom triggered snapshot TSS is configured for event
 *                False, otherwise.
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_IsCustomTriggeredTssrConfigured(
  Dem_EventIdType EventId
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_EVENTIF_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EventIF_Interface.h
 *********************************************************************************************************************/
