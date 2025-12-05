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
/*! \addtogroup Dem_EventIF
 *  \{
 *  \file       Dem_EventIF_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of the EventIF subcomponent
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

#if !defined (DEM_EVENTIF_IMPLEMENTATION_H)
#define DEM_EVENTIF_IMPLEMENTATION_H

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
#include "Dem_EventIF_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Satellite_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_RecordReader_Interface.h"
#include "Dem_OperationCycle_Interface.h"
#include "Dem_StatusIndicator_Interface.h"
#include "Dem_DTC_Interface.h"
#include "Dem_UDSStatus_Interface.h"
#include "Dem_Esm_Interface.h"
#include "Dem_InternalStatus_Interface.h"
#include "Dem_ExtendedStatus_Interface.h"
#include "Dem_CombinedStorage_Interface.h"
#include "Dem_SingleStorage_Interface.h"
#include "Dem_Aging_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_EVENT_IMPLEMENTATION_FILENAME "Dem_Event_Implementation.h"

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
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_EventIF_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Core_Event_SetMaxFdcSlc
 *****************************************************************************/
/*!
 * \brief         Sets the max FDC since last clear for given event.
 *
 * \details       Sets the max FDC since last clear for given event. Depending
 *                on the configuration the memory independent or memory
                  dependent max FDC SLC is set.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     FdcValue
 *                Value to set the max fdc slc to.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_SetMaxFdcSlc(
  Dem_EventIdType EventId,
  sint8 FdcValue
);

/* ****************************************************************************
 * Dem_Core_Event_GetEntryIndependentMaxFdcSlc
 *****************************************************************************/
/*!
 * \brief         Get the storage independent 'MaxFDCSinceLastClear' of
 *                the respective event.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \return        Storage independent 'MaxFDCSinceLastClear'
 *
 * \pre           'MaxFDCSinceLastClear' is stored for all DTCs
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_Core_Event_GetEntryIndependentMaxFdcSlc(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_Event_SetInternalUdsStatus
 *****************************************************************************/
/*!
 * \brief         Sets the event's internal UDS status byte.
 *
 * \details       Sets the event's internal UDS status byte. If the event is
 *                part of a combined group, the UDS status of the group is
 *                updated aswell.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     InternalUdsStatus
 *                Value to which the event status byte shall be set.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_SetInternalUdsStatus(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType InternalUdsStatus
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Core_Event_SetMaxFdcSlc
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
Dem_Core_Event_SetMaxFdcSlc(
  Dem_EventIdType EventId,
  sint8 FdcValue
  )
{
  if (Dem_Core_Event_IsMemIndependentMaxFdcSlcSupported() == TRUE)
  { 
    Dem_Core_Event_SetEntryIndependentMaxFdcSlc(EventId, FdcValue);
  }
  else
  { 
    if (Dem_Event_InternalStatus_TestEventStored(EventId) == TRUE)
    {
      Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, TRUE);
      Dem_MemoryEntry_EventEntry_SetMaxFdcSlc(lEventEntryIndex, FdcValue);
      Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
}

/* ****************************************************************************
% Dem_Core_Event_GetEntryIndependentMaxFdcSlc
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
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_Core_Event_GetEntryIndependentMaxFdcSlc(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
)
{
  sint8 lMaxFDCSinceLastClear = 0;
#if (DEM_CFG_DATA_MAX_FDC_SINCE_LAST_CLEAR == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_MAX_FDC_SLC == STD_ON)
  Dem_Cfg_MaxFDCSinceLastClearIndexOfEventTableType lMaxFDCSinceLastClearIndex = Dem_Cfg_GetMaxFDCSinceLastClearIndexOfEventTable(EventId);

  if (lMaxFDCSinceLastClearIndex != DEM_CFG_MAXFDCSINCELASTCLEAR_INDEX_INVALID)
  {
    lMaxFDCSinceLastClear = Dem_Cfg_AdminData.MaxFDCSinceLastClear[lMaxFDCSinceLastClearIndex];
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
  return lMaxFDCSinceLastClear;
}

/* ****************************************************************************
 % Dem_Core_Event_SetInternalUdsStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_SetInternalUdsStatus(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType InternalUdsStatus
  )
{
  Dem_Cfg_CombinedGroupIndexType lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, InternalUdsStatus);
  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    Dem_Event_UDSStatus_CalculateAndUpdateCombinedGroupStatus(lGroupId);
  }
}

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
 * \defgroup Dem_Event_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Event_IsMultiEventTriggeringSupported
 *****************************************************************************/
 /*!
 * \brief         Tests if multi event triggering is supported.
 *
 * \details       Tests if multi event triggering is supported.
 *
 * \return        TRUE
 *                Multi event triggering is supported
 * \return        FALSE
 *                Multi event triggering is supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsMultiEventTriggeringSupported(
  void
);


/* ****************************************************************************
 * Dem_Event_SetDataCommitStarted
 *****************************************************************************/
/*!
 * \brief         Set commit started state of event data update transaction.
 *
 * \details       Set commit started state of event data update transaction.
 *
 * \param[in]     The update state of the event data.
 *
 * \return        Event data state marked as 'Commit Started'.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_SetDataCommitStarted(
  CONST(uint8, AUTOMATIC)  EventDataState
  );

/* ****************************************************************************
 * Dem_Event_SetDataCommitFinished
 *****************************************************************************/
/*!
 * \brief         Set commit finished state of event data update transaction.
 *
 * \details       Set commit finished state of event data update transaction.
 *
 * \param[in]     The update state of the event data.
 *
 * \return        Event data state marked as 'Commit Finished'.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_SetDataCommitFinished(
  CONST(uint8, AUTOMATIC)  EventDataState
  );
  
/* ****************************************************************************
 * Dem_Event_TestSimilarConditions
 *****************************************************************************/
/*!
 * \brief         Test if similar conditions are fulfilled for event.
 *
 * \details       Test if similar conditions are fulfilled for event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Similar conditions are supported, event is OBD related and
 *                the external status bit for similar conditions is set.
 * \return        FALSE
 *                Otherwise.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestSimilarConditions(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );


#if (DEM_FEATURE_NEED_IMMEDIATEHEAL == STD_ON)
/* ****************************************************************************
 * Dem_Core_Event_ProcessHealingOnPassed
 *****************************************************************************/
/*!
 * \brief         Process event healing for events with healing target 0.
 *
 * \details       Tests if an event has healed with healing target 0 and
 *                processes the actions resulting from healing. This function
 *                has no effect when called for events with healing target != 0.
 *
 * \param[in]     EventContext
 *                Event processing context
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_IMMEDIATEHEAL == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_ProcessHealingOnPassed(
  CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  );
#endif


/* ****************************************************************************
 * Dem_Event_TestConfirmed
 *****************************************************************************/
/*!
 * \brief         Tests if an event has confirmed (completed tripping)
 *
 * \details       -
 *
 * \param[in]     EventContext
 *                Event processing context
 *
 * \return        TRUE
 *                Event is confirmed
 * \return        FALSE
 *                Event is not yet confirmed
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestConfirmed(
  CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC)  EventContext
  );

/* ****************************************************************************
 * Dem_Core_Event_TestMultiTripTargetReached
 *****************************************************************************/
/*!
 * \brief         Tests whether a multi-trip event has reached its trip target
 *
 * \details       Tests whether a multi-trip event has reached its trip target
 *                or MIL group trip target, if event is part of a MIL group.
 *                The function is only relevant for multi-trip configurations.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event has reached its trip target
 * \return        FALSE
 *                The event has not reached its trip target 
 *                OR the event is not multi-trip
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_TestMultiTripTargetReached(
  Dem_EventIdType  EventId
);

/* ****************************************************************************
 * Dem_Event_OpCycleStart
 *****************************************************************************/
/*!
 * \brief         Processes the start of the event's operation cycle
 *
 * \details       Processes the start of the event's operation cycle: Reset
 *                status bits and debouncing.
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
Dem_Event_OpCycleStart(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );


/* ****************************************************************************
 * Dem_Event_OpCycleEnd_Healing
 *****************************************************************************/
/*!
 * \brief         Processes healing at the end of the event's operation cycle
 *
 * \details       Processes healing at the end of the event's operation cycle
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current event status
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Event_OpCycleEnd_Healing(
  Dem_EventIdType EventId,
  uint8 EventStatus
  );


/* ****************************************************************************
 * Dem_Event_OpCycleEnd
 *****************************************************************************/
/*!
 * \brief         Processes the end of the event's operation cycle
 *
 * \details       Processes actions performed at the end of the event's operation
 *                cycle, i.e. healing, aging start condition.
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
Dem_Event_OpCycleEnd(
  Dem_EventIdType EventId
  );


/* ****************************************************************************
 * Dem_Event_InitEventContext
 *****************************************************************************/
/*!
 * \brief         Initialize context of event's status report processing.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \param[out]    EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InitEventContext(
CONST(Dem_EventIdType, AUTOMATIC)  EventId,
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
);


/* ****************************************************************************
 * Dem_Event_CalculateStatusOnFailed
 *****************************************************************************/
/*!
 * \brief         Update of event's storage independent UDS status bits on
 *                'Failed' status report
 *
 * \details       -
 *
 * \param[in,out] EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventContext->EventId or same EventContext
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CalculateStatusOnFailed(
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
);


/* ****************************************************************************
 * Dem_Event_CalculateFailedEffects
 *****************************************************************************/
/*!
 * \brief         Calculate storage independent event triggers on 'Failed' status report.
 *
 * \details       Calculate event triggers from changed, storage independent
 *                UDS status bits on 'Failed' status report.
 *
 * \param[in,out] EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventContext->EventId or same EventContext
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CalculateFailedEffects(
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
);


/* ****************************************************************************
 * Dem_Event_CalculateStatusOnFailed_OnStorage
 *****************************************************************************/
/*!
 * \brief         Update of event's storage dependent UDS status bits on
 *                'Failed' status report
 *
 * \details       -
 *
 * \param[in,out] EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventContext->EventId or same EventContext
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CalculateStatusOnFailed_OnStorage(
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
);  


/* ****************************************************************************
 * Dem_Event_CalculateFailedEffects_OnStorage
 *****************************************************************************/
/*!
 * \brief         Calculate storage depending event triggers on 'Failed' status report.
 *
 * \details       Among others, process reaching pending state, confirmation
 *                on reaching event's or Mil group's (if configured) trip count
 *                 target and warning indicator requests.
 *
 * \param[in,out] EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CalculateFailedEffects_OnStorage(
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
);  


/* ****************************************************************************
 * Dem_Event_CalculateStatusOnPassed
 *****************************************************************************/
/*!
 * \brief         Update of event's UDS status bits on 'Passed' status report
 *
 * \details       -
 *
 * \param[in,out] EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventContext->EventId or same EventContext
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CalculateStatusOnPassed(
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
);


/* ****************************************************************************
 * Dem_Event_Passed_CalcEventEffects
 *****************************************************************************/
/*!
 * \brief         Calculate event triggers on 'Passed' status report.
 *
 * \details       -
 *
 * \param[in,out] EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventContext->EventId or same EventContext
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Passed_CalcEventEffects(
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
);


/* ****************************************************************************
 * Dem_Event_RestoreAvailabilityFromNv
 *****************************************************************************/
/*!
 * \brief         Initializes the event availability from NV Ram
 *
 * \details       Initializes the event availability from NV Ram
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           May be called only during module initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_RestoreAvailabilityFromNv(
  Dem_EventIdType EventId
  );


/* ****************************************************************************
 * Dem_Event_InitQualification
 *****************************************************************************/
/*!
 * \brief         Initializes the qualification states
 *
 * \details       Initializes the qualification states if the confirmed is 
 *                processed for all DTCs and Obd is supported in this 
 *                (PBS) variant
 *
 * \pre           May be called only during module initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InitQualification(
  void
  );


/* ****************************************************************************
 * Dem_Event_InitEventInternalStatus
 *****************************************************************************/
/*!
 * \brief         Initializes the internal event status of a given event
 *
 * \details       Initializes the internal event status of a given event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           May be called only during module initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InitEventInternalStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );


/* ****************************************************************************
 * Dem_Event_InitStatus_ResetStoredOnlyBits
 *****************************************************************************/
/*!
 * \brief         Reset the stored-only bits in the UDS status.
 *
 * \details       Reset the stored-only bits in the UDS status.
 *
 * \param[in]     EventStatus
 *                Current UDS status of the event.
 * \return        Updated value of the EventStatus
 *
 * \pre           May be called only during module initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InitStatus_ResetStoredOnlyBits(
  CONST(Dem_UDSStatus_StatusType, AUTOMATIC) EventStatus
  );


#if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
/* ****************************************************************************
 * Dem_Event_InitStatus_AgedEvent
 *****************************************************************************/
/*!
 * \brief         Initialize aged events.
 *
 * \details       For aged events, reset in the UDS status the relevant bits
 *                and perform or start healing.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current UDS status of the event.
 * \return        Updated value of the EventStatus
 *
 * \pre           May be called only during module initialization
 * \config        (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InitStatus_AgedEvent(
  CONST(Dem_EventIdType, AUTOMATIC) EventId,
  CONST(Dem_UDSStatus_StatusType, AUTOMATIC) EventStatus
  );
#endif


/* ****************************************************************************
 * Dem_Event_InitStatus_AgingEvent
 *****************************************************************************/
/*!
 * \brief         Initialize aging events.
 *
 * \details       For aging events, reset in the UDS status the relevant bits
 *                and perform or start healing.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current UDS status of the event.
 * \return        Updated value of the EventStatus
 *
 * \pre           May be called only during module initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InitStatus_AgingEvent(
  CONST(Dem_EventIdType, AUTOMATIC) EventId,
  CONST(Dem_UDSStatus_StatusType, AUTOMATIC) EventStatus
  );


#if (DEM_FEATURE_NEED_INDICATORS == STD_ON) && (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
/* ****************************************************************************
 * Dem_Event_InitStatus_UpdateMilGroups
 *****************************************************************************/
/*!
 * \brief         Update the MIL group pending event count.
 *
 * \details       Update the MIL group pending event count.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           May be called only during module initialization
 * \config        (DEM_FEATURE_NEED_INDICATORS == STD_ON)
 *                && (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InitStatus_UpdateMilGroups(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );
#endif


/* ****************************************************************************
 * Dem_Event_InitEventStatus
 *****************************************************************************/
/*!
 * \brief         Initializes the event status of a given event
 *
 * \details       Initializes the event status of a given event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           May be called only during module initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InitEventStatus(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );


#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 * Dem_Event_ProcessEventAvailable
 *****************************************************************************/
/*!
 * \brief         Process event availability changes
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 * \pre           -
 * \config        DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ProcessEventAvailable(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );
#endif


/* ****************************************************************************
 * Dem_Core_Event_UpdateTripCounterOnFailed
 *****************************************************************************/
/*!
 * \brief         Update event's trip counter on a failed report
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 * \param[in]     OldEventStatus
 *                Event UDS status of the event before the failed report
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_UpdateTripCounterOnFailed(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType OldEventStatus
  );

/* ****************************************************************************
 * Dem_Core_Event_ResetTripAndHealingCounterOnClear
 *****************************************************************************/
/*!
 * \brief         Reset trip and healing counter of the event on clear
 *
 * \details       Reset trip and healing counter of the event when the event
 *                is cleared
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 * \param[in]     PendingBitSetBeforeClear
 *                TRUE
 *                Pending bit of the event was set before the clear
 *                FALSE
 *                Pending bit of the event was not set before the clear
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_ResetTripAndHealingCounterOnClear(
  Dem_EventIdType EventId,
  boolean PendingBitSetBeforeClear
  );


/* ****************************************************************************
 * Dem_Core_Event_SetHealingCounter
 *****************************************************************************/
/*!
 * \brief         Set healing counter for the event
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the event
 * \param[in]     CounterValue
 *                New value of the shared healing counter.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_SetHealingCounter(
  Dem_EventIdType EventId,
  uint8 CounterValue
  );


/* ****************************************************************************
 * Dem_Core_Event_Healed
 *****************************************************************************/
/*!
 * \brief         Process event healed
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current UDS status of the event.
 *
 * \return        Modified UDS status of the event
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Core_Event_Healed(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
);


/* ****************************************************************************
 * Dem_Core_Event_ProcessHealingOnOpCycleEnd
 *****************************************************************************/
/*!
 * \brief         Process healing for the event on operation cycle end
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current UDS status of the event.
 *
 * \return        Modified UDS status of the event
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Core_Event_ProcessHealingOnOpCycleEnd(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
);

/* ****************************************************************************
 * Dem_Core_Event_ProcessPendingResetOnOpCycleEnd
 *****************************************************************************/
/*!
 * \brief         Process Reset of Pending bit for the event on operation cycle end
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current UDS status of the event.
 *
 * \return        Modified UDS status of the event
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Core_Event_ProcessPendingResetOnOpCycleEnd(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Event_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Core_Event_GetMaxFdcToc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(sint8, DEM_CODE)
Dem_Core_Event_GetMaxFdcToc(
  Dem_EventIdType EventId
  )
{
  sint8 lMaxFdcToc = 0;
  if (Dem_Event_CombinedStorage_IsSupportedByEvent(EventId) == TRUE)
  {
    lMaxFdcToc = Dem_Event_CombinedStorage_GetMaxFdcToc(EventId);
  }
  else
  {
    lMaxFdcToc = Dem_Event_SingleStorage_GetMaxFdcToc(EventId);
  }
  return lMaxFdcToc;
}

/* ****************************************************************************
 % Dem_Core_Event_SetEntryIndependentMaxFdcSlc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Core_Event_SetEntryIndependentMaxFdcSlc(
  Dem_EventIdType EventId,
  sint8 FdcValue
)
{
#if (DEM_CFG_DATA_MAX_FDC_SINCE_LAST_CLEAR == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_MAX_FDC_SLC == STD_ON)
  Dem_Cfg_MaxFDCSinceLastClearIndexOfEventTableType lMaxFDCSinceLastClearIndex = Dem_Cfg_GetMaxFDCSinceLastClearIndexOfEventTable(EventId);

  if (lMaxFDCSinceLastClearIndex != DEM_CFG_MAXFDCSINCELASTCLEAR_INDEX_INVALID)
  {
    if ( (Dem_Error_isDevRunTimeChecksEnabled() == TRUE)
      && (lMaxFDCSinceLastClearIndex >= DEM_CFG_MAX_NUMBER_MAX_FDC_SLC_DTCS) )
    {
      Dem_Error_RunTimeCheckFailed(DEM_DTCIF_IMPLEMENTATION_FILENAME, __LINE__);                                                 /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    }
    else
    {
      Dem_Cfg_AdminData.MaxFDCSinceLastClear[lMaxFDCSinceLastClearIndex] = FdcValue;                                             /* SBSW_DEM_ARRAY_WRITE_ADMINDATA_MAXFDCSINCELASTCLEAR */
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FdcValue)                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}


/* ****************************************************************************
% Dem_Core_Event_IsMemIndependentMaxFdcSlcSupported
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_IsMemIndependentMaxFdcSlcSupported(
  void
)
{
  return (DEM_CFG_SUPPORT_MEM_INDEPENDENT_MAX_FDC_SLC == STD_ON);
}

/* ****************************************************************************
 % Dem_Core_Event_GetMaxFdcSlc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_Core_Event_GetMaxFdcSlc(
  Dem_EventIdType EventId
  )
{
  sint8 lMaxFdcSlc = 0;
  if (Dem_Core_Event_IsMemIndependentMaxFdcSlcSupported() == TRUE)
  { 
    lMaxFdcSlc = Dem_Core_Event_GetEntryIndependentMaxFdcSlc(EventId);
  }
  else
  { 
    if (Dem_Event_InternalStatus_TestEventStored(EventId) == TRUE)
    {
      Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, TRUE);
      lMaxFdcSlc = Dem_MemoryEntry_EventEntry_GetMaxFdcSlc(lEventEntryIndex);
    }
  }
  return lMaxFdcSlc;
}

/* ****************************************************************************
 % Dem_Core_Event_UpdateMaxFdcSlc
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
Dem_Core_Event_UpdateMaxFdcSlc(                                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
)
{
  sint8 lOldFdc = Dem_Core_Event_GetMaxFdcSlc(EventId);
  sint8 lNewFdc = Dem_Core_Event_GetMaxFdcToc(EventId);

  if (lNewFdc > lOldFdc)
  {
    Dem_Core_Event_SetMaxFdcSlc(EventId, lNewFdc);
  }
}

/* ****************************************************************************
 % Dem_Core_Event_GetInternalUdsStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Core_Event_GetInternalUdsStatus(
  Dem_EventIdType EventId
  )
{
  Dem_DTC_UDSStatusType lStatus;
  if (Dem_Event_CombinedStorage_IsSupportedByEvent(EventId) == TRUE)
  {
    lStatus = Dem_Event_UDSStatus_GetCombinedGroupStatus(Dem_Cfg_EventCombinedGroup(EventId));

  }
  else
  {
    lStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
  }
  return lStatus;
}

/* ****************************************************************************
 % Dem_Core_Event_TryResetQualifiedConfirmedSinceLastClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_TryResetQualifiedConfirmedSinceLastClear(
  Dem_EventIdType EventId
  )
{
  Dem_UDSStatus_StatusType lExternalEventStatus =
    Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId));

  if (Dem_Event_UDSStatus_TestBit(lExternalEventStatus, Dem_UdsStatus_CDTC) == FALSE)
  {
    Dem_ExtendedStatus_StatusType lExtendedStatus = Dem_Event_ExtendedStatus_GetEventStatus(EventId);
    lExtendedStatus = Dem_Event_ExtendedStatus_ResetBit(lExtendedStatus, Dem_ExtendedStatus_QCSLC);
    Dem_Event_ExtendedStatus_SetEventStatus(EventId, lExtendedStatus);
  }
}

/* ****************************************************************************
 % Dem_Core_Event_GetActivationTimerIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Pto_ActivationTimer_TimerIdType, DEM_CODE)
Dem_Core_Event_GetActivationTimerIndex(
  Dem_EventIdType EventId
  )
{
  Dem_Pto_ActivationTimer_TimerIdType lTimer = DEM_CFG_PTO_TIMER_INDEX_INVALID;
  if (EventId != DEM_EVENT_INVALID)                                                                                              /* COV_DEM_ROBUSTNESS TX */
  {
# if (DEM_FEATURE_NEED_PTO_TIMER == STD_ON)
    lTimer = Dem_Cfg_GetEventPtoTimerIndexOfEventTable(EventId);
#endif
  }
  return lTimer;
}

/* ****************************************************************************
 % Dem_Core_Event_HasConfiguredOrCalculatedSRecs
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_HasConfiguredOrCalculatedSRecs(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
)
{
  boolean lReturnValue;

  if( (Dem_Cfg_IsSRecSupportEnabled() == TRUE)
   && (Dem_Cfg_SRecNumStartIdx(EventId) < Dem_Cfg_SRecNumEndIdx(EventId)) )
  {
    lReturnValue = TRUE;
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Event_IsSimilarConditionsEnabled
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsSimilarConditionsEnabled(
  void
)
{
  return (boolean)(DEM_CFG_SUPPORT_SIMILAR_CONDITIONS == STD_ON);
}

/* ****************************************************************************
 % Dem_Event_TestValidHandle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestValidHandle(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  return (boolean)( (EventId != DEM_EVENT_INVALID)
                 && (EventId < Dem_Cfg_GlobalEventCount()) );                                                                    
}

/* ****************************************************************************
 % Dem_Event_IsSlaveEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsSlaveEvent(
  Dem_EventIdType EventId                                                                                                        /* PRQA S 3206 */ /* MD_DEM_3206 */
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId);                                                                                     /* PRQA S 1338 */ /* MD_MSR_DummyStmt */
  return Dem_Event_IsMultiEventTriggeringSupported() && Dem_Cfg_IsMultiEventTriggeringSlaveOfEventPrecompileDataTable(EventId);
}

/* ****************************************************************************
 % Dem_Event_IsMasterEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsMasterEvent(
  Dem_EventIdType EventId                                                                                                        /* PRQA S 3206 */ /* MD_DEM_3206 */
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId);                                                                                     /* PRQA S 1338 */ /* MD_MSR_DummyStmt */
  return Dem_Event_IsMultiEventTriggeringSupported() && (Dem_Cfg_IsMultiEventTriggeringSlaveEventsTableUsedOfEventPrecompileDataTable(EventId));
}

/* ****************************************************************************
 % Dem_Event_TestDebounceCounterStoredInNv
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestDebounceCounterStoredInNv(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId                                                                                     /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_DEBOUNCE_NV == STD_ON)
  return (boolean)(Dem_Cfg_IsDebounceCounterStorageOfEventPrecompileDataTable(EventId));
#else
  return FALSE;
#endif
}

/* ****************************************************************************
 % Dem_Event_TestObdVisibilityDelayed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestObdVisibilityDelayed(                                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId                                                                                     /* PRQA S 3206 */ /* MD_DEM_3206 */
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)
  return (boolean)(Dem_Cfg_IsOBDVisibilityDelayedUntilDcyQualificationOfEventTable(EventId));
}

#if (DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON)
/* ****************************************************************************
 % Dem_Event_GetTripCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
   */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_GetTripCount(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  return Dem_Cfg_GetStatusData().TripCount[EventId];                                                                             /* PRQA S 2842, 2843 */ /* MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation */
}
#endif

/* ****************************************************************************
 % Dem_Core_Event_GetHealingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
   */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Core_Event_GetHealingCounter(                                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  uint8 lReturn = 0;

  if (Dem_Event_CombinedStorage_IsSupportedByEvent(EventId))
  {
    lReturn = Dem_Event_CombinedStorage_GetHealingCounter(EventId);
  }
  else
  {
    lReturn = Dem_Event_SingleStorage_GetHealingCounter(EventId);
  }


  return lReturn;
}

#if (DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON)
/* ****************************************************************************
 % Dem_Event_SetTripCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_SetTripCount(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  TripCount
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EventId >= Dem_Cfg_EventCountOfStatusBlock())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENT_IMPLEMENTATION_FILENAME, __LINE__);                                                   /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_GetStatusData().TripCount[EventId] = TripCount;                                                                      /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_STATUSDATA_TRIPCOUNT */
  }
}
#endif

/* ****************************************************************************
 % Dem_Core_Event_UpdateTrigger_TestConfirmed
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_UpdateTrigger_TestConfirmed(
  CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC) EventTrigger
)
{
  return ((EventTrigger & DEM_CFG_TRIGGER_CONFIRMED) != 0u);
}

/* ****************************************************************************
 % Dem_Core_Event_UpdateTrigger_TestWIR
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_UpdateTrigger_TestWIR(
  CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC) EventTrigger
)
{
  return ((EventTrigger & DEM_CFG_TRIGGER_INDICATOR) != 0u);
}

/* ****************************************************************************
 % Dem_Core_Event_UpdateTrigger_TestFailed
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_UpdateTrigger_TestFailed(
  CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC) EventTrigger
)
{
  return ((EventTrigger & DEM_CFG_TRIGGER_TESTFAILED) != 0u);
}

/* ****************************************************************************
 % Dem_Core_Event_UpdateTrigger_TestPassed
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_UpdateTrigger_TestPassed(
  CONST(Dem_Cfg_StorageTriggerType, AUTOMATIC) EventTrigger
)
{
  return ((EventTrigger & DEM_CFG_TRIGGER_PASSED) != 0u);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Event_Private Private Methods
 * \{
 */

/* ****************************************************************************
 % Dem_Event_IsMultiEventTriggeringSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsMultiEventTriggeringSupported(
  void
)
{
  return (boolean)(DEM_CFG_SUPPORT_MULTI_EVENT_TRIGGERING == STD_ON);
}

 /* ****************************************************************************
 % Dem_Event_SetDataCommitStarted
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_SetDataCommitStarted(
  CONST(uint8, AUTOMATIC)  EventDataState
)
{
  uint8 lEventDataState;
  lEventDataState = (uint8)(EventDataState | DEM_EVENT_DATA_COMMIT_STARTED);
  return lEventDataState;
}

/* ****************************************************************************
% Dem_Event_SetDataCommitFinished
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_SetDataCommitFinished(
  CONST(uint8, AUTOMATIC)  EventDataState
)
{
  uint8 lEventDataState;
  lEventDataState = (uint8)((EventDataState + 1u) & DEM_EVENT_DATA_COMMIT_FINISHED);
  return lEventDataState;
}

/* ****************************************************************************
 % Dem_Event_TestSimilarConditions
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestSimilarConditions(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lReturnValue;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

#if (DEM_CFG_SUPPORT_SIMILAR_CONDITIONS == STD_ON)
  if ((Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
    && (Dem_Core_DTC_IsObdRelated(EventId) == TRUE))
  {
    
    lReturnValue = Dem_Event_ExtendedStatus_TestBit(Dem_Event_ExtendedStatus_GetEventStatus(EventId), Dem_ExtendedStatus_SimilarConditions);
  }
  else
#endif
  {
    lReturnValue = TRUE;
  }
  return lReturnValue;
}

#if (DEM_FEATURE_NEED_IMMEDIATEHEAL == STD_ON)
/* ****************************************************************************
 % Dem_Core_Event_ProcessHealingOnPassed
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
Dem_Core_Event_ProcessHealingOnPassed(
  CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext                                                           /* PRQA S 3673 */ /* MD_DEM_8.13_Design */
  )
{
  if (Dem_Cfg_EventHealingTarget(EventContext->EventId) == 0u)
  {
# if (DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON)
    if ( (Dem_Event_UDSStatus_TestBit(EventContext->OldEventStatus, Dem_UdsStatus_PDTC) == TRUE) &&
         (Dem_Core_Event_TestHasIndicator(EventContext->EventId) == TRUE) )
    {
      Dem_Event_SetTripCount(EventContext->EventId, 0);
    }
# endif

    if (Dem_Event_UDSStatus_TestBit(EventContext->OldEventStatus, Dem_UdsStatus_WIR) == TRUE)
    {
      Dem_UDSStatus_StatusType lNewEventStatus = EventContext->NewEventStatus;
      EventContext->NewEventStatus  = Dem_Core_Event_Healed(EventContext->EventId, lNewEventStatus);                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* DEM_FEATURE_NEED_IMMEDIATEHEAL == STD_ON */

/* ****************************************************************************
 % Dem_Event_TestConfirmed
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestConfirmed(
  CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext                                                           /* PRQA S 3673 */ /* MD_DEM_8.13_Design */
  )
{
  boolean lEventConfirmed = FALSE;

  /* event is configured as single-trip */
  if (Dem_Cfg_EventTripTarget(EventContext->EventId) == 0u)
  {
    lEventConfirmed = TRUE;
  }
  else
  {
    if (Dem_Event_TestSimilarConditions(EventContext->EventId) == TRUE)
    {
#if (DEM_CFG_SUPPORT_MULTITRIP == STD_ON)
      lEventConfirmed = Dem_Core_Event_TestMultiTripTargetReached(EventContext->EventId);
#else
      if ( (Dem_Event_UDSStatus_TestBit(EventContext->OldEventStatus, Dem_UdsStatus_PDTC) == TRUE)
        && (Dem_Event_UDSStatus_TestBit(EventContext->OldEventStatus, Dem_UdsStatus_TFTOC) == FALSE))
      {
        /* event has tripped twice */
        lEventConfirmed = TRUE;
      }
#endif
    }
  }

  return lEventConfirmed;
}

/* ****************************************************************************
 % Dem_Core_Event_TestMultiTripTargetReached
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_TestMultiTripTargetReached(
  Dem_EventIdType  EventId
)
{
  boolean lReturnValue = FALSE;

  if (Dem_Cfg_EventTripTarget(EventId) != 0u)
  {
#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
    if (Dem_Cfg_EventMilGroupIndex(EventId) != DEM_CFG_MIL_GROUP_INVALID)
    {
      if (Dem_Mem_GetMilGroupTripCount(Dem_Cfg_EventMilGroupIndex(EventId)) >= Dem_Cfg_EventTripTarget(EventId))
      {
        /* MIL Group is used and MIL group tripped */
        lReturnValue = TRUE;
      }
      /* else: event has a MIL group, but the group has not tripped yet */
    }
    else
#endif
    {
#if (DEM_CFG_SUPPORT_MULTITRIP == STD_ON)
      if (Dem_Event_GetTripCount(EventId) >= Dem_Cfg_EventTripTarget(EventId))
      {
        /* multi-trip target reached */
        lReturnValue = TRUE;
      }
      /* else: event has not tripped yet */
#endif
    }
  }

  return lReturnValue;
}


/* ****************************************************************************
 % Dem_Event_OpCycleStart
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_OpCycleStart(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  uint8 lOldEventStatus;
  uint8 lNewEventStatus;

  lOldEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);

  lNewEventStatus = lOldEventStatus;
#if (DEM_CFG_SUPPORT_CLEAR_TESTFAILED_ON_OPCYCLE == STD_ON)
  lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_TF);
#endif
  lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_TFTOC);
  lNewEventStatus = Dem_Event_UDSStatus_SetBit(lNewEventStatus, Dem_UdsStatus_TNCTOC);

  Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, lNewEventStatus);

#if (DEM_CFG_SUPPORT_RESTART_CYCLE == STD_ON)
  if (Dem_InitState_TestMasterInState(DEM_INITSTATE_INITIALIZED) == TRUE)
#elif (DEM_FEATURE_NEED_CYCLEAUTOMATICEND == STD_ON)
  if ( (Dem_Cfg_IsEmissionObdSupportedInVariant() == FALSE)
    || (Dem_InitState_TestMasterInState(DEM_INITSTATE_INITIALIZED) == TRUE))
#endif
  {
    Dem_Event_ResetEventReporting(EventId);
  }

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  if (Dem_Event_InternalStatus_TestEventStored(EventId) == TRUE)
  {
    boolean lIsModified = FALSE;
    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, TRUE);
    uint8 lOldPreconfirmedStatus;
    uint8 lNewPreconfirmedStatus;

#if (DEM_CFG_SUPPORT_RESTART_CYCLE == STD_ON)
    if ( (Dem_InitState_TestMasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
      || ((uint16)Dem_DebounceBaseSat_GetDebounceValueMax(EventId) < Dem_Cfg_EventPreconfirmedTarget(EventId)) )                 /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
#endif
    {
      lOldPreconfirmedStatus = Dem_MemoryEntry_EventEntry_GetPreConfirmedStatus(lEventEntryIndex);
      lNewPreconfirmedStatus = DEM_DTC_RESET_PC_STATUS_UDTCTOC(lOldPreconfirmedStatus);
      Dem_MemoryEntry_EventEntry_SetPreConfirmedStatus(lEventEntryIndex, lNewPreconfirmedStatus);

      if (lOldPreconfirmedStatus != lNewPreconfirmedStatus)
      {
        lIsModified = TRUE;
      }
    }

    if (Dem_MemoryEntry_EventEntry_GetOccurrenceCounter1(lEventEntryIndex) < 0xffU)
    {
      Dem_MemoryEntry_EventEntry_SetOccurrenceCounter1(lEventEntryIndex,
        Dem_MemoryEntry_EventEntry_GetOccurrenceCounter1(lEventEntryIndex) + 1u);
      lIsModified = TRUE;
    }

    if (Dem_MemoryEntry_EventEntry_GetOccurrenceCounter3(lEventEntryIndex) < 0xffU)
    {
      Dem_MemoryEntry_EventEntry_SetOccurrenceCounter3(lEventEntryIndex,
        Dem_MemoryEntry_EventEntry_GetOccurrenceCounter3(lEventEntryIndex) + 1u);
      lIsModified = TRUE;
    }

    if (lIsModified == TRUE)
    {
      Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
#endif /* DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON */
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_OpCycleEnd_Healing
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
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Event_OpCycleEnd_Healing(
  Dem_EventIdType EventId,
  uint8 EventStatus
  )
{
  uint8 lNewEventStatus = EventStatus;
  boolean lClearObdFreezeFrame = FALSE;

  if (Dem_Event_UDSStatus_TestBit(lNewEventStatus, Dem_UdsStatus_TNCTOC) == FALSE)
  {
    if (Dem_Event_UDSStatus_TestBit(lNewEventStatus, Dem_UdsStatus_TFTOC) == FALSE)
    { /* Event tested this operation cycle and was not failed, reset pending state */
      if (Dem_Event_UDSStatus_TestBit(lNewEventStatus, Dem_UdsStatus_PDTC) == TRUE)
      {
        lNewEventStatus = Dem_Core_Event_ProcessPendingResetOnOpCycleEnd(EventId, lNewEventStatus);
        lClearObdFreezeFrame = TRUE;
      }
      lNewEventStatus = Dem_Core_Event_ProcessHealingOnOpCycleEnd(EventId, lNewEventStatus);

      Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, lNewEventStatus);

      if ( (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE) && (lClearObdFreezeFrame == TRUE) )
      {
#if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
        Dem_Cfg_CombinedGroupIndexType lGroupId;
        lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
        if ((lGroupId == DEM_CFG_COMBINED_GROUP_INVALID) || (Dem_Event_UDSStatus_TestBit(Dem_DTC_CalculateCombinedGroupUdsStatus(lGroupId), Dem_UdsStatus_PDTC) == FALSE))
#endif
        {
          if (Dem_Event_UDSStatus_TestBit(Dem_Core_Event_GetInternalUdsStatus(EventId), Dem_UdsStatus_CDTC) == FALSE)
          {
            /* DTC did not confirm before starting to age - release the OBD freeze
                frame if held by the event */
            Dem_Memories_ObdFreezeFrameMemory_ClearOBDFreezeFrame(EventId);
          }
        }
      }
    }
    else
    {
      if (Dem_Event_UDSStatus_TestBit(lNewEventStatus, Dem_UdsStatus_PDTC) == TRUE)
      {
#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
        /* test failed; check if MIL group exists */
        if (Dem_Cfg_EventMilGroupIndex(EventId) != DEM_CFG_MIL_GROUP_INVALID)
        {
          /* increment MIL group trip counter once per DCY if trip target has not yet been reached */
          if (Dem_Mem_GetMilGroupTripCount(Dem_Cfg_EventMilGroupIndex(EventId)) < Dem_Cfg_EventTripTarget(EventId))
          {
            if (Dem_Mem_GetMilGroupIncremented(Dem_Cfg_EventMilGroupIndex(EventId)) == 0x00u)
            {
              Dem_Mem_IncrementMilGroupTripCount(Dem_Cfg_EventMilGroupIndex(EventId));
              Dem_Mem_SetMilGroupIncremented(Dem_Cfg_EventMilGroupIndex(EventId));
            }
          }
        }
        else
#endif
#if (DEM_CFG_SUPPORT_MULTITRIP == STD_ON)
        {
          /* test failed: increment trip counter if trip target has not yet been reached */
          if (Dem_Event_GetTripCount(EventId) < Dem_Cfg_EventTripTarget(EventId))
          {
            Dem_Event_SetTripCount(EventId, (uint8)(Dem_Event_GetTripCount(EventId) + 1u));
          }
        }
#endif
      }
    }
  }
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Event_OpCycleEnd
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
Dem_Event_OpCycleEnd(
  Dem_EventIdType EventId
)
{
  uint8 lEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);

# if (DEM_FEATURE_NEED_CYCLE_COUNTERS == STD_ON)
  if ((Dem_Event_InternalStatus_TestEventStored(EventId) == TRUE) && (EventId == Dem_Cfg_GetMemoryRepresentative(EventId)))
  {
    uint16 lCycleCounter = Dem_OperationCycle_GetCycleCounter(Dem_Cfg_EventOperationCycle(EventId));
    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, TRUE);
    Dem_FaultMemory_Memories_LatchCycleCounters(lEventEntryIndex, lCycleCounter);
  }
# endif

  if (Dem_Event_TestSimilarConditions(EventId) == TRUE)
  {
    Dem_Event_OpCycleEnd_Healing(EventId, lEventStatus);
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
    Dem_Core_Aging_ProcessStartOnOPCycleEnd(EventId);
#endif
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_InitEventContext
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InitEventContext(
CONST(Dem_EventIdType, AUTOMATIC)  EventId,
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
)
{
  EventContext->EventId = EventId;                                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->GroupIndex = Dem_Cfg_EventCombinedGroup(EventId);                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->OldEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->OldExternalEventStatus = Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, EventContext->OldEventStatus);        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->NewEventStatus = EventContext->OldEventStatus;                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->QualifyStatus = Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId);                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->EventEffects = DEM_CFG_TRIGGER_NONE;                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->DataModified = FALSE;                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->QualifyEvent = FALSE;                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Event_CalculateStatusOnFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CalculateStatusOnFailed(
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
)
{
    /* Process the deferred 'asynchronous' status bit transitions for the event */
  EventContext->NewEventStatus = Dem_Event_UDSStatus_SetBit(EventContext->NewEventStatus, Dem_UdsStatus_TFTOC);                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->NewEventStatus = Dem_Event_UDSStatus_SetBit(EventContext->NewEventStatus, Dem_UdsStatus_TF);                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#if (DEM_CFG_PROCESS_TFSLC_STOREDONLY == STD_OFF)
  EventContext->NewEventStatus = Dem_Event_UDSStatus_SetBit(EventContext->NewEventStatus, Dem_UdsStatus_TFSLC);                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#endif
  EventContext->NewEventStatus = Dem_Event_UDSStatus_ResetBit(EventContext->NewEventStatus, Dem_UdsStatus_TNCSLC);               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->NewEventStatus = Dem_Event_UDSStatus_ResetBit(EventContext->NewEventStatus, Dem_UdsStatus_TNCTOC);               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Event_CalculateFailedEffects
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CalculateFailedEffects(
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
)
{
#if (DEM_CFG_SUPPORT_RETRY_STORAGE == STD_ON)
  /* If event is not stored already */
  if (Dem_Event_InternalStatus_TestEventStored(EventContext->EventId) == FALSE)
  {
    EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_TESTFAILED);          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_TESTFAILEDTHISCYCLE); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_FIRSTTHISCYCLE);      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  else
#endif
  {
    /* Derive the triggers for the event from the changes of the status bits */
    if (Dem_Event_UDSStatus_TestBit(EventContext->OldEventStatus, Dem_UdsStatus_TF) == FALSE)
    {
      EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_TESTFAILED);        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    if (Dem_Event_UDSStatus_TestBit(EventContext->OldEventStatus, Dem_UdsStatus_TFTOC) == FALSE)
    {
      EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_TESTFAILEDTHISCYCLE); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_FIRSTTHISCYCLE);    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
}

/* ****************************************************************************
 % Dem_Event_CalculateStatusOnFailed_OnStorage
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CalculateStatusOnFailed_OnStorage(
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
)
{
#if (DEM_CFG_PROCESS_TFSLC_STOREDONLY == STD_ON)
  if (Dem_Event_InternalStatus_TestEventStored(EventContext->EventId) == TRUE)
  {
    EventContext->NewEventStatus = Dem_Event_UDSStatus_SetBit(EventContext->NewEventStatus, Dem_UdsStatus_TFSLC);                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
#endif

  if ((EventContext->EventEffects & DEM_CFG_TRIGGER_PENDING) != 0u)
  {
    EventContext->NewEventStatus = Dem_Event_UDSStatus_SetBit(EventContext->NewEventStatus, Dem_UdsStatus_PDTC);                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  if ((EventContext->EventEffects & DEM_CFG_TRIGGER_CONFIRMED) != 0u)
  {
    EventContext->NewEventStatus = Dem_Event_UDSStatus_SetBit(EventContext->NewEventStatus, Dem_UdsStatus_CDTC);                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  if ((EventContext->EventEffects & DEM_CFG_TRIGGER_INDICATOR) != 0u)
  {
    EventContext->NewEventStatus = Dem_Event_UDSStatus_SetBit(EventContext->NewEventStatus, Dem_UdsStatus_WIR);                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Event_CalculateFailedEffects_OnStorage
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
Dem_Event_CalculateFailedEffects_OnStorage(
  CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  )
{
  {
    /* Calculate effects for 'Pending' bit */
    if (Dem_Event_UDSStatus_TestBit(EventContext->OldEventStatus, Dem_UdsStatus_PDTC) == FALSE)
    {
      /* Not yet pending, set pending independently of TFTOC */
      EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_PENDING);           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
      /* increment number of MIL group events that are pending (at most 127 are possible) */
      if (Dem_Cfg_EventMilGroupIndex(EventContext->EventId) != DEM_CFG_MIL_GROUP_INVALID)
      {
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
        if (Dem_Event_InternalStatus_GetMilGroupPendingCount(Dem_Cfg_EventMilGroupIndex(EventContext->EventId)) >= 0x7Fu)
        {
          Dem_Error_RunTimeCheckFailed(DEM_EVENT_IMPLEMENTATION_FILENAME, __LINE__);                                             /* SBSW_DEM_POINTER_RUNTIME_CHECK */
        }
# endif
        Dem_Mem_IncrementMilGroupPendingCount(Dem_Cfg_EventMilGroupIndex(EventContext->EventId));
      }
#endif
    }
#if (DEM_CFG_SUPPORT_RETRY_STORAGE == STD_ON) && (DEM_CFG_PROCESS_PDTC_ALL_DTC == STD_ON)
    else if (Dem_Event_InternalStatus_TestEventStored(EventContext->EventId) == FALSE)
    {
      EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_PENDING);           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
#endif
    else
    {
      /* Nothing to do */
    }
  }

  {
    /* Calculate effects for 'Confirmed' and 'WIR' bit */
    boolean lEventConfirmed;
    lEventConfirmed = Dem_Event_TestConfirmed(EventContext);                                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    if (lEventConfirmed == TRUE)
    { /* Event has confirmed */
#if (DEM_FEATURE_NEED_INDICATORS == STD_ON)
      if (Dem_Event_UDSStatus_TestBit(EventContext->OldEventStatus, Dem_UdsStatus_WIR) == FALSE)
      {
        if (Dem_Core_Event_TestHasIndicator(EventContext->EventId) == TRUE)
        {
          EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_INDICATOR);     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        }
      }
#endif

      if (Dem_Event_UDSStatus_TestBit(EventContext->OldEventStatus, Dem_UdsStatus_CDTC) == FALSE)
      {
        EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_CONFIRMED);       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      } /* else: No transition for confirmed dtc, so no update */
#if (DEM_CFG_SUPPORT_RETRY_STORAGE == STD_ON) && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
      else if (Dem_Event_InternalStatus_TestEventStored(EventContext->EventId) == FALSE)
      {
        EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_CONFIRMED);       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
#endif
      else
      {
        /* Nothing to do */
      }
    } /* else: not yet confirmed, no further action */
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Event_CalculateStatusOnPassed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CalculateStatusOnPassed(
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
)
{
  EventContext->NewEventStatus = Dem_Event_UDSStatus_ResetBit(EventContext->NewEventStatus, Dem_UdsStatus_TF);                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->NewEventStatus = Dem_Event_UDSStatus_ResetBit(EventContext->NewEventStatus, Dem_UdsStatus_TNCSLC);               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  EventContext->NewEventStatus = Dem_Event_UDSStatus_ResetBit(EventContext->NewEventStatus, Dem_UdsStatus_TNCTOC);               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Event_Passed_CalcEventEffects
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Passed_CalcEventEffects(
CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
)
{
  if (Dem_Event_UDSStatus_TestBit(EventContext->OldEventStatus, Dem_UdsStatus_TF) == TRUE)
  {
    EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_PASSED);              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
  if (Dem_Event_UDSStatus_TestBit(EventContext->OldEventStatus, Dem_UdsStatus_TNCTOC) == TRUE)
  {
    EventContext->EventEffects = (Dem_Cfg_StorageTriggerType)(EventContext->EventEffects | DEM_CFG_TRIGGER_FIRSTTHISCYCLE);      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */


/* ****************************************************************************
 % Dem_Event_RestoreAvailabilityFromNv
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_RestoreAvailabilityFromNv(
  Dem_EventIdType EventId
  )
{
#if (DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON)
  uint8 lIndexInAvailabilityArray;
  uint8 lBitIndexInAvailabilityMask;
  uint8 lAvailabilityMask;
  boolean lStoredAvailability;

  lIndexInAvailabilityArray = (uint8)(EventId / 8u);
  lBitIndexInAvailabilityMask = (uint8)(EventId % 8u);
  lAvailabilityMask = Dem_Event_GetAvailableMask(lIndexInAvailabilityArray);
  lStoredAvailability = (boolean)(((lAvailabilityMask >> lBitIndexInAvailabilityMask) & 1u) == DEM_EVENT_AVAILABILITYDATA_AVAILABLE);

  if (lStoredAvailability == FALSE)
  {
    Dem_Event_SetDisconnectedBit(EventId, Dem_Event_MayDisconnect(EventId));
  }
  else
  {
    Dem_Event_SetDisconnectedBit(EventId, FALSE);
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId);
#endif
}

/* ****************************************************************************
 % Dem_Event_InitQualification
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InitQualification(
  void
  )
{
#if (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF)
  if(Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    Dem_EventIdType lEventId;

    for (lEventId = DEM_EVENT_FIRST; lEventId < Dem_Cfg_GlobalEventCount(); ++lEventId)
    { /* Initialize the qualified status for all events */
      uint8 lEventQualifyStatus;

      lEventQualifyStatus = 0xFF;
      if (Dem_Core_DTC_IsObdRelated(lEventId) == TRUE)
      { /* only OBD related events supports the qualification of Confirmed and WIR bit */
        lEventQualifyStatus = Dem_Event_InternalStatus_QualifyStatus_ResetBit(lEventQualifyStatus, Dem_QualifyStatus_CDTC);
        lEventQualifyStatus = Dem_Event_InternalStatus_QualifyStatus_ResetBit(lEventQualifyStatus, Dem_QualifyStatus_WIR);
      }
      Dem_DTC_SetQualifyStatus(lEventId, lEventQualifyStatus);
    }
  }
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_InitEventInternalStatus
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
Dem_Event_InitEventInternalStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId                                                                                     /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_EVENTAVAILABLE_DEFAULT == STD_ON)
  if (Dem_Cfg_EventAvailableByDefault(EventId) == FALSE)
  {
    Dem_Event_SetDisconnectedBit(EventId, Dem_Event_MayDisconnect(EventId));
    Dem_Dtr_EventAvailabilityChanged(EventId);
  }
  else
  {
    Dem_Event_SetDisconnectedBit(EventId, FALSE);
  }
#endif
}

/* ****************************************************************************
 % Dem_Event_InitStatus_ResetStoredOnlyBits
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InitStatus_ResetStoredOnlyBits(
  CONST(Dem_UDSStatus_StatusType, AUTOMATIC)  EventStatus
)
{
  Dem_UDSStatus_StatusType lReturnValue;
  lReturnValue = EventStatus;

  /* Note: this relies on the compiler to optimize the different bit operations into a single mask operation.
  * The intention is to find a dedicated access for each Bit modification for review purposes. */
# if (DEM_CFG_PROCESS_PDTC_STOREDONLY == STD_ON)
  lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_PDTC);
# endif
# if (DEM_CFG_PROCESS_CDTC_STOREDONLY == STD_ON)
  lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_CDTC);
# endif
# if (DEM_CFG_PROCESS_TFSLC_STOREDONLY == STD_ON)
  lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_TFSLC);
# endif
  return lReturnValue;
}

#if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
/* ****************************************************************************
 % Dem_Event_InitStatus_AgedEvent
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
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InitStatus_AgedEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONST(Dem_UDSStatus_StatusType, AUTOMATIC)  EventStatus
)
{
  Dem_UDSStatus_StatusType lReturnValue;

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  lReturnValue = EventStatus;

  if ((Dem_Core_Aging_IsAgingForAllEventsSupported() == TRUE)
   || (Dem_Memories_EventMemory_TestStorageTriggerIsOnConfirmed(EventId) == FALSE))
  {
    lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_TF);
  }

  if (Dem_Memories_EventMemory_TestStorageTriggerIsOnConfirmed(EventId) == FALSE)
  {
    if ( (Dem_Event_UDSStatus_TestBit(EventStatus, Dem_UdsStatus_PDTC) == TRUE)
# if (DEM_CFG_AGING_START_TRIGGER_PASSED == STD_ON)
      && (Dem_Core_Aging_GetAgingTarget(EventId) == 0u)                                                                                
# endif
      )
    {
      lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_PDTC);
      if (Dem_Cfg_EventHealingTarget(EventId) == 0u)
      {
        if (Dem_Event_UDSStatus_TestBit(EventStatus, Dem_UdsStatus_WIR) == TRUE)
        {
          lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_WIR);
          if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
          {
            Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId,
              Dem_Event_InternalStatus_QualifyStatus_ResetBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_WIR));
          }
        }
      }
      Dem_Core_Event_SetHealingCounter(EventId, 0);
    }

# if (DEM_CFG_PROCESS_TFSLC_ALL_DTC == STD_ON)
    lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_TFSLC);
# endif
  }

  lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_CDTC);
  if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
  {
    Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId,
      Dem_Event_InternalStatus_QualifyStatus_ResetBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_CDTC));
  }

# if (DEM_FEATURE_NEED_AGING_AFTER_HEALING == STD_ON)
#  if (DEM_FEATURE_NEED_AGING_AFTER_HEALING_MIL_ONLY == STD_ON)
  if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
#  endif
  {
    lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_WIR);
    if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
    {
      Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId,
        Dem_Event_InternalStatus_QualifyStatus_ResetBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_WIR));
    }

  }
# endif

  return lReturnValue;
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif

/* ****************************************************************************
 % Dem_Event_InitStatus_AgingEvent
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
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InitStatus_AgingEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_UDSStatus_StatusType, AUTOMATIC)  EventStatus
  )
{
  Dem_UDSStatus_StatusType lReturnValue;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  lReturnValue = EventStatus;

  {
    /* Reconstruct the TF, TFTOC, TNCSLC bits of an aging event */
    lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_TF);
#if (DEM_CFG_AGING_START_TESTS_TFTOC == STD_ON)
    lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_TFTOC);
#endif
    lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_TNCSLC);
  }

#if (DEM_CFG_AGING_START_TRIGGER_PASSED == STD_OFF)
  if (Dem_Event_UDSStatus_TestBit(EventStatus, Dem_UdsStatus_PDTC) == TRUE)
  {
    lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_PDTC);
    if (Dem_Cfg_EventHealingTarget(EventId) == 0u)
    {
      if (Dem_Event_UDSStatus_TestBit(EventStatus, Dem_UdsStatus_WIR) == TRUE)
      {
        lReturnValue = Dem_Event_UDSStatus_ResetBit(lReturnValue, Dem_UdsStatus_WIR);
        if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
        {
          Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId,
            Dem_Event_InternalStatus_QualifyStatus_ResetBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_WIR));
        }
      }
    }
    Dem_Core_Event_SetHealingCounter(EventId, 0);
  }
#endif

  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_FEATURE_NEED_INDICATORS == STD_ON) && (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
/* ****************************************************************************
 % Dem_Event_InitStatus_UpdateMilGroups
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
Dem_Event_InitStatus_UpdateMilGroups(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  if (Dem_Cfg_EventMilGroupIndex(EventId) != DEM_CFG_MIL_GROUP_INVALID)
  {
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if (Dem_Event_InternalStatus_GetMilGroupPendingCount(Dem_Cfg_EventMilGroupIndex(EventId)) >= 0x7FU)
    {
      Dem_Error_RunTimeCheckFailed(DEM_EVENT_IMPLEMENTATION_FILENAME, __LINE__);                                                 /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    }
# endif
    Dem_Mem_IncrementMilGroupPendingCount(Dem_Cfg_EventMilGroupIndex(EventId));
  }
}
#endif

/* ****************************************************************************
 % Dem_Event_InitEventStatus
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InitEventStatus(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_UDSStatus_StatusType lEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
  if (Dem_Cfg_EventAvailableByVariant(EventId) == FALSE)
  {
    lEventStatus = 0x00;
    Dem_Dtr_EventAvailabilityChanged(EventId);
  }
  else
  {
    Dem_InternalStatus_StoredStatusType lAgedState = Dem_Event_InternalStatus_GetStoredStatus(EventId);

    switch (lAgedState)
    {
    case Dem_InternalStatus_StoredStatus_None:
      /* Initialize event status of aging and non aging events without event entry */
      lEventStatus = Dem_Event_InitStatus_ResetStoredOnlyBits(lEventStatus);
#if (DEM_CFG_SUPPORT_AGING_FOR_ALL_DTCS == STD_ON)
      /* Initialize event status of aging events without event entry */
      if (Dem_Core_Aging_GetAgingStatus(EventId, DEM_CFG_ENTRYINDEX_INVALID) == Dem_AgingStatus_Aging)
      {
        lEventStatus = Dem_Event_InitStatus_AgingEvent(EventId, lEventStatus);
      }
#endif
      break;

#if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
      /* Initialize event status of aged events with event entry */
    case Dem_InternalStatus_StoredStatus_Aged:
      lEventStatus = Dem_Event_InitStatus_AgedEvent(EventId, lEventStatus);
      break;
#endif

      /* Initialize event status of aging events with event entry */
    case Dem_InternalStatus_StoredStatus_Aging:
      lEventStatus = Dem_Event_InitStatus_AgingEvent(EventId, lEventStatus);
      break;

    default:                                                                                                                     /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
      /* Do nothing */
      break;
    }
  }

#if (DEM_CFG_SUPPORT_TESTFAILED_STORAGE != STD_ON)
  lEventStatus = Dem_Event_UDSStatus_ResetBit(lEventStatus, Dem_UdsStatus_TF);
#endif

  if ((lEventStatus & (DEM_UDS_STATUS_TFTOC | DEM_UDS_STATUS_TNCTOC)) == (DEM_UDS_STATUS_TFTOC | DEM_UDS_STATUS_TNCTOC))
  { /* Test not completed, but failed this cycle is not a consistent state.
    *  Reset TFTOC to restore consistency. This also makes sure a new test result is processed */
    lEventStatus = Dem_Event_UDSStatus_ResetBit(lEventStatus, Dem_UdsStatus_TFTOC);
  }

  if ((lEventStatus & (DEM_UDS_STATUS_TFSLC | DEM_UDS_STATUS_TNCSLC)) == (DEM_UDS_STATUS_TFSLC | DEM_UDS_STATUS_TNCSLC))
  { /* Test not completed, but failed since last clear is not a consistent state.
    *  Reset TNCSLC to restore consistency. Keep TFSLC since it is sometimes set by correction code */
    lEventStatus = Dem_Event_UDSStatus_ResetBit(lEventStatus, Dem_UdsStatus_TNCSLC);
  }

  if (Dem_Core_Event_TestHasIndicator(EventId) == FALSE)
  {
    lEventStatus = Dem_Event_UDSStatus_ResetBit(lEventStatus, Dem_UdsStatus_WIR);
    if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
    {
      Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId,
        Dem_Event_InternalStatus_QualifyStatus_ResetBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_WIR));
    }
  }

  Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, lEventStatus);

#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
  if (Dem_Event_UDSStatus_TestBit(lEventStatus, Dem_UdsStatus_PDTC) == TRUE)
  {
    Dem_Event_InitStatus_UpdateMilGroups(EventId);
  }
#endif
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 % Dem_Event_ProcessEventAvailable
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
Dem_Event_ProcessEventAvailable(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  )
{
  if (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(EventId), Dem_InternalStatus_AvailabilityChanged) == TRUE)
  {
    Dem_Event_InternalStatusType lInternalStatus;
    boolean lEventWasMadeAvailable;

/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();

    lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
    lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_AvailabilityChanged);
    Dem_Event_InternalStatus_SetEventStatus(EventId, lInternalStatus);

    lEventWasMadeAvailable = (boolean)(Dem_Event_InternalStatus_TestBit(lInternalStatus, Dem_InternalStatus_Disconnected) == FALSE);
    if (lEventWasMadeAvailable == TRUE)
    {
      /* Event was reconnected */
      Dem_DTC_Reconnect(EventId);
      Dem_Iumpr_SetRatioAvailability(EventId, Dem_Iumpr_ReconnectRatio);
    }
    else
    {
      /* Dtc was requested to disconnect */
      Dem_DTC_Disconnect(EventId);

      if (Dem_Cfg_EventPrestorageIndex(EventId) != DEM_PRESTORAGEMEMORY_NOT_CONFIGURED_FOR_EVENT)
      {
        (void)Dem_Memories_PrestorageMemory_EventPrestorageClear(EventId);
      }

      Dem_Iumpr_SetRatioAvailability(EventId, Dem_Iumpr_DisconnectRatio);
    }

    Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

    Dem_DiagnosticObserver_ProcessAsyncEffectsOfEventAvailabilityChange(EventId, lEventWasMadeAvailable);
    Dem_Notifications_EventAvailabilityChanged(EventId, lEventWasMadeAvailable);
  }
}                                                                                                                                /* PRQA S 6080, 6050 */ /* MD_MSR_STMIF, MD_MSR_STCAL */
#endif


/* ****************************************************************************
 % Dem_Core_Event_UpdateTripCounterOnFailed
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
Dem_Core_Event_UpdateTripCounterOnFailed(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType OldEventStatus
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OldEventStatus)                                                                               /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON)
  if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(EventId))
  {
    Dem_Event_CombinedStorage_SharedCounter_SetHealingCounter(EventId, 0u);
  }
  else
  {
    if (Dem_Event_UDSStatus_TestBit(OldEventStatus, Dem_UdsStatus_PDTC) == FALSE)
    {
      Dem_Event_SetTripCount(EventId, 0);
    }
  }
#endif
}


/* ****************************************************************************
 % Dem_Core_Event_ResetTripAndHealingCounterOnClear
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
Dem_Core_Event_ResetTripAndHealingCounterOnClear(
  Dem_EventIdType EventId,
  boolean PendingBitSetBeforeClear
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(PendingBitSetBeforeClear)                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
  {
    uint16 lMilGroupIndex;
    lMilGroupIndex = Dem_Cfg_EventMilGroupIndex(EventId);

    if (lMilGroupIndex != DEM_CFG_MIL_GROUP_INVALID)
    {
      Dem_Mem_InitMilGroupTripCount(lMilGroupIndex);

      if (PendingBitSetBeforeClear == TRUE)
      {
        /* decrement MIL group pending count if event pending state has changed from 1 to 0 */
        Dem_Mem_DecrementMilGroupPendingCount(lMilGroupIndex);
      }
    }
  }
#endif

#if (DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON)
  if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(EventId))
  {
    Dem_Event_CombinedStorage_SharedCounter_SetHealingCounter(EventId, 0u);
  }
  else
  {
    Dem_Event_SetTripCount(EventId, 0);
  }
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Core_Event_SetHealingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
   */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_SetHealingCounter(                                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId,
  uint8 CounterValue
  )
{
  if (Dem_Event_CombinedStorage_IsSupportedByEvent(EventId) == TRUE)
  {
    Dem_Event_CombinedStorage_SetHealingCounter(EventId, CounterValue);
  }
  else
  {
    Dem_Event_SingleStorage_SetHealingCounter(EventId, CounterValue);
  }
}

/* ****************************************************************************
 % Dem_Core_Event_Healed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Core_Event_Healed(                                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  )
{
  Dem_UDSStatus_StatusType lNewEventStatus = EventStatus;
  if (Dem_Event_CombinedStorage_IsSupportedByEvent(EventId) == TRUE)
  {
    lNewEventStatus = Dem_Event_CombinedStorage_Healed(EventId, EventStatus);
  }
  else
  {
    lNewEventStatus = Dem_Event_SingleStorage_Healed(EventId, EventStatus);
  }

  return lNewEventStatus;
}

/* ****************************************************************************
 % Dem_Core_Event_ProcessHealingOnOpCycleEnd
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Core_Event_ProcessHealingOnOpCycleEnd(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  )
{
  Dem_UDSStatus_StatusType lNewEventStatus = EventStatus;
  if (Dem_Event_CombinedStorage_IsSupportedByEvent(EventId) == TRUE)
  {
    lNewEventStatus = Dem_Event_CombinedStorage_ProcessHealing(EventId, EventStatus);
  }
  else
  {
    lNewEventStatus = Dem_Event_SingleStorage_ProcessHealing(EventId, EventStatus);
  }

  return lNewEventStatus;
}

/* ****************************************************************************
 % Dem_Core_Event_ProcessPendingResetOnOpCycleEnd
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Core_Event_ProcessPendingResetOnOpCycleEnd(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  )
{
  Dem_UDSStatus_StatusType lNewEventStatus = EventStatus;
  if (Dem_Event_CombinedStorage_IsSupportedByEvent(EventId) == TRUE)
  {
    lNewEventStatus = Dem_Event_CombinedStorage_ResetPending(EventId, EventStatus);
  }
  else
  {
    lNewEventStatus = Dem_Event_SingleStorage_ResetPending(EventId, EventStatus);
  }

  return lNewEventStatus;
}


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
 * \addtogroup Dem_Event_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Event_GetDataCommitState
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_GetDataCommitState(                                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
)
{
  return Dem_Cfg_GetEventDataCommitNumber(Dem_Cfg_GetMemoryRepresentative(EventId));
}

#if (DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON)
/* ****************************************************************************
 % Dem_Event_GetAvailableMask
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_GetAvailableMask(
  CONST(uint16, AUTOMATIC)  Index                                                                                                /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  return Dem_Cfg_EventAvailableData[Index];
}
#endif

#if (DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON)
/* ****************************************************************************
 % Dem_Event_SetAvailableMask
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_SetAvailableMask(
  CONST(uint16, AUTOMATIC)  Index,
  CONST(uint8, AUTOMATIC)  Mask
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Index >= Dem_Cfg_GlobalEventAvailableDataSize())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENT_IMPLEMENTATION_FILENAME, __LINE__);                                                   /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_EventAvailableData[Index] = Mask;                                                                                    /* SBSW_DEM_ARRAY_WRITE_EVENTAVAILABLEDATA */
  }
}
#endif

 /* ****************************************************************************
 % Dem_Event_TestDataUpdateInProgress
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestDataUpdateInProgress(                                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint8, AUTOMATIC)  DataState
)
{
  boolean lCommitStarted;
  if ((DataState & DEM_EVENT_DATA_COMMIT_STARTED) != 0x00u)
  {
    lCommitStarted = TRUE;
  }
  else
  {
    lCommitStarted = FALSE;
  }
  return lCommitStarted;
}

/* ****************************************************************************
 % Dem_Event_DataUpdateStart
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
Dem_Event_DataUpdateStart(
  Dem_EventIdType  EventId
  )
{
  Dem_EventIdType  lEventId;
  lEventId = Dem_Cfg_GetMemoryRepresentative(EventId);
  
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (lEventId >= Dem_Cfg_GetSizeOfEventDataCommitNumber())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENT_IMPLEMENTATION_FILENAME, __LINE__);                                                   /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    Dem_Cfg_SetEventDataCommitNumber(lEventId,                                                                                   /* SBSW_DEM_ARRAY_WRITE_EVENTDATACOMMITNUMBER */
                                  Dem_Event_SetDataCommitStarted(
                                  Dem_Cfg_GetEventDataCommitNumber(lEventId)));
  }

}

/* ****************************************************************************
 % Dem_Event_DataUpdateFinish
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_DataUpdateFinish(
  Dem_EventIdType  EventId
  )
{
  Dem_EventIdType  lEventId;
  lEventId = Dem_Cfg_GetMemoryRepresentative(EventId);

#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (lEventId >= Dem_Cfg_GetSizeOfEventDataCommitNumber())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENT_IMPLEMENTATION_FILENAME, __LINE__);                                                   /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    Dem_Cfg_SetEventDataCommitNumber(lEventId,                                                                                   /* SBSW_DEM_ARRAY_WRITE_EVENTDATACOMMITNUMBER */
                                  Dem_Event_SetDataCommitFinished(
                                  Dem_Cfg_GetEventDataCommitNumber(lEventId)));
  }
}

#if ((DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) \
    && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF))
/* ****************************************************************************
 % Dem_Event_UpdateQualificationStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Event_UpdateQualificationStatus(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lEventQualifyStatus;
  uint8 lEntryQualifyStatus;

  lEventQualifyStatus = Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId);
  lEntryQualifyStatus = DEM_MEM_SET_NON_QUALIFIED_STATUS_BITS(Dem_MemoryEntry_EventEntry_GetState(EventEntryIndex));

  /* Synchronize the events qualification status */
  if ((lEntryQualifyStatus ^ lEventQualifyStatus) != 0u)
  {
    Dem_MemoryEntry_EventEntry_SetState(EventEntryIndex,
      DEM_MEM_APPLY_QUALIFIED_STATUS(Dem_MemoryEntry_EventEntry_GetState(EventEntryIndex),lEventQualifyStatus));
    Dem_Nvm_SetEventEntryNvBlockState(EventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
  }
}
#endif

/* ****************************************************************************
 % Dem_Core_Event_TestHealingPreventsAging
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Core_Event_TestHealingPreventsAging(                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
)
{
  boolean lAgingPrevented;
  if (Dem_Event_CombinedStorage_IsSupportedByEvent(EventId) == TRUE)
  {
    lAgingPrevented = Dem_Event_CombinedStorage_TestHealingPreventsAging(EventId);
  }
  else
  {
    lAgingPrevented = Dem_Event_SingleStorage_TestHealingPreventsAging(EventId);
  }
  return lAgingPrevented;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 % Dem_Event_Disconnect_Normal
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
Dem_Event_Disconnect_Normal(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Event_InternalStatusType lInternalStatus;

  lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
  lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_FDCTOC);
  Dem_Event_InternalStatus_SetEventStatus(EventId, lInternalStatus);

  Dem_SatelliteCore_ClearEvent(
      Dem_Cfg_EventSatelliteId(EventId),
      Dem_Cfg_EventSatelliteEventId(EventId));


  if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
  {
    Dem_Event_ExtendedStatus_SetEventStatus(EventId,
      Dem_Event_ExtendedStatus_ResetBit(Dem_Event_ExtendedStatus_GetEventStatus(EventId), Dem_ExtendedStatus_SimilarConditions)
    );
  }

  Dem_Mar_DecrementPendingEventsIfNeeded(EventId);

  Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, 0x00U);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif


#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 % Dem_Event_Reconnect_Normal
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Reconnect_Normal(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, 0x50U);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_Event_TestEventAvailable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestEventAvailable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId                                                                                     /* PRQA S 3206 */ /* MD_DEM_3206 */
)
{
  boolean lReturnValue;

  if (Dem_Cfg_EventAvailableByVariant(EventId) == TRUE)
  {
#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
    lReturnValue = (boolean)(Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(EventId), Dem_InternalStatus_Disconnected) == FALSE);
#else
    lReturnValue = TRUE;
#endif
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Event_TestEventRuntimeAvailable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestEventRuntimeAvailable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId                                                                                     /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  boolean lReturnValue;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
  lReturnValue = (boolean)(Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(EventId), Dem_InternalStatus_Disconnected) == FALSE);
#else
  lReturnValue = TRUE;
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Event_TestSuppressionOnEventRetrieval
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestSuppressionOnEventRetrieval(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lReturnValue;
  lReturnValue = (boolean)
     ( (Dem_DTC_TestDtcSuppression(EventId) == TRUE)
    || (Dem_Cfg_EventAvailableByVariant(EventId) == FALSE)                                                                       
    || ( (Dem_Event_TestEventRuntimeAvailable(EventId) == FALSE)                                                                 /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      &&  (Dem_Event_InternalStatus_TestEventStored(EventId) == FALSE))                                                          /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      );
  return lReturnValue;
}


/* ****************************************************************************
 % Dem_Event_TestEventUnavailable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestEventUnavailable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lReturnValue;
  lReturnValue = (boolean)
     ( (Dem_Cfg_EventAvailableByVariant(EventId) == FALSE)
    || ( (Dem_Event_TestEventRuntimeAvailable(EventId) == FALSE)                                                                 /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      && (Dem_Event_InternalStatus_TestEventStored(EventId)  == FALSE) )                                                         /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
     );
  return lReturnValue;
}


#if (DEM_FEATURE_NEED_EVENTAVAILABLE_DEFAULT == STD_ON) || (DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON)
/* ****************************************************************************
 % Dem_Event_SetDisconnectedBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_SetDisconnectedBit(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)  IsDisconnected
  )
{
  Dem_Event_InternalStatusType lInternalStatus;
  lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);

  if (IsDisconnected == TRUE)
  {
    lInternalStatus = Dem_Event_InternalStatus_SetBit(lInternalStatus, Dem_InternalStatus_Disconnected);
    Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, 0x00);
  }
  else
  {
    lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_Disconnected);
  }

  Dem_Event_InternalStatus_SetEventStatus(EventId, lInternalStatus);
  Dem_Dtr_EventAvailabilityChanged(EventId);
}
#endif

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 % Dem_Event_MayDisconnect
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
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_MayDisconnect(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  boolean lReturnValue;
  uint8 lEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);

  if ( (Dem_Event_UDSStatus_TestBit(lEventStatus, Dem_UdsStatus_TF) == FALSE)
    && (Dem_Event_UDSStatus_TestBit(lEventStatus, Dem_UdsStatus_WIR) == FALSE)
    && (Dem_Event_UDSStatus_TestBit(lEventStatus, Dem_UdsStatus_PDTC) == FALSE)
    && (Dem_Event_UDSStatus_TestBit(lEventStatus, Dem_UdsStatus_CDTC) == FALSE)
    )
  {
    if (Dem_Event_InternalStatus_TestEventStored(EventId) == FALSE)
    {
      lReturnValue = TRUE;
    }
    else
    {
      lReturnValue = Dem_DTC_TestRemainingEventforDtc(EventId);
    }
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 % Dem_Event_Disconnect
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
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Event_Disconnect(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Std_ReturnType lReturnValue;
  Dem_Event_InternalStatusType lEventInternalStatus;

  lEventInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
  if (Dem_Event_InternalStatus_TestBit(lEventInternalStatus, Dem_InternalStatus_Disconnected) == FALSE)
  {
    if ( ((Dem_InitState_TestMasterInState(DEM_INITSTATE_PREINITIALIZED) == TRUE) || (Dem_Event_MayDisconnect(EventId) == TRUE)) /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      && (Dem_Event_InternalStatus_TestBit(lEventInternalStatus, Dem_InternalStatus_AvailabilityChanged) == FALSE) )
    {
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
      Dem_EnterCritical_DiagMonitor();
      lEventInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
      lEventInternalStatus = Dem_Event_InternalStatus_SetBit(lEventInternalStatus, Dem_InternalStatus_Disconnected);
      lEventInternalStatus = Dem_Event_InternalStatus_SetBit(lEventInternalStatus, Dem_InternalStatus_AvailabilityChanged);
      Dem_Event_InternalStatus_SetEventStatus(EventId, lEventInternalStatus);
      Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

      Dem_Scheduler_EnableTaskOnce(Dem_Scheduler_Task_Event);

# if (DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON)
      {
        uint8 lNvAvailableMask;
        lNvAvailableMask = Dem_Event_GetAvailableMask(EventId / 8u);
        lNvAvailableMask = (uint8)(lNvAvailableMask | DEM_BIT(EventId % 8u));
        Dem_Event_SetAvailableMask(EventId / 8u, lNvAvailableMask);
        Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_AVAILABILITY),
                                    DEM_NVM_BLOCKSTATE_DIRTY);
      }
# endif

      Dem_GlobalDiagnostics_ResetDisabledEventMonitor(EventId);

      Dem_DiagnosticObserver_ProcessSynchronousEffectsOfEventAvailabilityChange(EventId, FALSE);
      Dem_Dtr_EventAvailabilityChanged(EventId);

      lReturnValue = E_OK;
    }
    else
    {
      lReturnValue = E_NOT_OK;
    }
  }
  else
  {
    lReturnValue = E_OK;
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON) */

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
/* ****************************************************************************
 % Dem_Event_Reconnect
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Reconnect(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Event_InternalStatusType lEventInternalStatus;

  lEventInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
  if ( (Dem_Event_InternalStatus_TestBit(lEventInternalStatus, Dem_InternalStatus_Disconnected) == TRUE)
    && (Dem_Event_InternalStatus_TestBit(lEventInternalStatus, Dem_InternalStatus_AvailabilityChanged) == FALSE) )                                        
  {
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();
    lEventInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
    lEventInternalStatus = Dem_Event_InternalStatus_ResetBit(lEventInternalStatus, Dem_InternalStatus_Disconnected);
    lEventInternalStatus = Dem_Event_InternalStatus_SetBit(lEventInternalStatus, Dem_InternalStatus_AvailabilityChanged);
    Dem_Event_InternalStatus_SetEventStatus(EventId, lEventInternalStatus);
    Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

    Dem_Scheduler_EnableTaskOnce(Dem_Scheduler_Task_Event);

# if (DEM_FEATURE_NEED_EVENTAVAILABLE_NV == STD_ON)
    {
      uint8 lNvAvailableMask;
      lNvAvailableMask = Dem_Event_GetAvailableMask(EventId / 8u);
      lNvAvailableMask = (uint8)(lNvAvailableMask & DEM_BIT_MASK(EventId % 8u, DEM_1BYTE));
      Dem_Event_SetAvailableMask(EventId / 8u, lNvAvailableMask);
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_AVAILABILITY),
                                  DEM_NVM_BLOCKSTATE_DIRTY);
    }
# endif
    Dem_Dtr_EventAvailabilityChanged(EventId);
    Dem_Dtr_ResetDtrsOfEvent(EventId);
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON) */

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
/* ****************************************************************************
 % Dem_Event_ProcessFdc
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
Dem_Event_ProcessFdc(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)  FdcThresholdExceeded,
  CONST(boolean, AUTOMATIC)  FdcMaximumExceeded
  )
{
#if (DEM_FEATURE_NEED_FDC_TRIP == STD_ON)
  /* no events without a DTC attached, disabled storage conditions, dtc/event suppressed... */
  if ((Dem_Cfg_EventUdsDtc(EventId) != DEM_CFG_DTC_UDS_INVALID)
        /* Don't store with disabled storage conditions */
    && (Dem_GlobalDiagnostics_TestEventStorageConditionsSatisfied(EventId) == TRUE))
  {
# if (DEM_CFG_SUPPORT_TRIGGER_FDC_FIRST_IN_CYCLE == STD_ON)  || (DEM_CFG_SUPPORT_TRIGGER_FDC == STD_ON) || (DEM_CFG_STORAGE_AT_FDC == STD_ON)
    if (FdcThresholdExceeded == TRUE)
    {
      Dem_DTC_FdcTrip(EventId);
    }
# endif
    if (FdcMaximumExceeded == TRUE)
    {
      Dem_Core_Event_UpdateMaxFdcSlc(EventId);
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FdcMaximumExceeded)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FdcThresholdExceeded)                                                                         /* PRQA S 3112 */ /* MD_DEM_14.2 */
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_Event_Passed
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
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Event_Passed(
  Dem_EventIdType EventId
  )
{
  if (Dem_Event_TestLatchedTestFailed(EventId) == FALSE)
  {
    Dem_Event_ContextType lEventContext;
    Dem_DTC_ContextType lDTCContext;

    Dem_Event_InitEventContext(EventId, &lEventContext);                                                                         /* SBSW_DEM_POINTER_FORWARD_STACK */
    Dem_DTC_InitDTCContext(EventId, &lDTCContext);                                                                               /* SBSW_DEM_POINTER_FORWARD_STACK */

    Dem_Event_CalculateStatusOnPassed(&lEventContext);                                                                           /* SBSW_DEM_POINTER_FORWARD_STACK */
    Dem_Event_Passed_CalcEventEffects(&lEventContext);                                                                           /* SBSW_DEM_POINTER_FORWARD_STACK */

#if (DEM_CFG_SUPPORT_SIMILAR_CONDITIONS == STD_OFF) && (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
    if (lEventContext.OldEventStatus != lEventContext.NewEventStatus)
#endif
    {
      Dem_Event_DataUpdateStart(EventId);
#if (DEM_FEATURE_NEED_IMMEDIATEHEAL == STD_ON)
      if (Dem_Event_TestSimilarConditions(EventId) == TRUE)
      {
        Dem_Core_Event_ProcessHealingOnPassed(&lEventContext);                                                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }
#endif
      Dem_Core_Event_SetInternalUdsStatus(EventId, lEventContext.NewEventStatus);

      Dem_DiagnosticObserver_ProcessEventPassed(&lEventContext);                                                                 /* SBSW_DEM_POINTER_FORWARD_STACK */

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON || DEM_CFG_SUPPORT_TRIGGER_PASSED == STD_ON)
      Dem_DTC_Passed_CalcDTCEffects(&lDTCContext, lEventContext.EventEffects);                                                   /* SBSW_DEM_POINTER_FORWARD_STACK */
#endif
      Dem_Notifications_EventStatusChanged(EventId,
        lEventContext.OldExternalEventStatus,
        Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, lEventContext.NewEventStatus));
      Dem_Notifications_DtcStatusChanged(EventId,
        lDTCContext.OldExternalDtcStatus,
        Dem_DTC_GetExternalUDSStatus(EventId),
        DEM_NOTIFICATIONS_DTC_UPDATED);

      Dem_DTC_Passed_ProcessDTCEffects(&lEventContext, &lDTCContext);                                                            /* SBSW_DEM_POINTER_FORWARD_STACK */
      /* Aging needs to be processed after event entry update at passed (Dem_DTC_Passed_ProcessDTCEffects) due
         to feature retain in memory */
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
      Dem_DTC_CustomizedAgingStart(&lDTCContext);                                                                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
#else
      Dem_Core_Aging_ProcessStartOnPassed(EventId);
#endif

      Dem_Event_DataUpdateFinish(EventId);
      if (lEventContext.DataModified == TRUE)
      {
        Dem_Notifications_EventDataChanged(lEventContext.EventId);
      }
    }

    if (Dem_Event_UDSStatus_IsNvRelevantStatusChange(lEventContext.OldEventStatus, lEventContext.NewEventStatus) == TRUE)
    {
      Dem_Nvm_SetStatusBlockChanged();
    }

    Dem_Mar_MarkNumeratorToBeIncremented(EventId);
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_Failed
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Event_Failed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Event_ContextType lEventContext;
  Dem_DTC_ContextType lDTCContext;

  Dem_Event_InitEventContext(EventId, &lEventContext);                                                                           /* SBSW_DEM_POINTER_FORWARD_STACK */
  Dem_DTC_InitDTCContext(EventId, &lDTCContext);                                                                                 /* SBSW_DEM_POINTER_FORWARD_STACK */

  Dem_Event_CalculateStatusOnFailed(&lEventContext);                                                                             /* SBSW_DEM_POINTER_FORWARD_STACK */
  Dem_Core_Event_SetInternalUdsStatus(EventId, lEventContext.NewEventStatus);

  Dem_Event_CalculateFailedEffects(&lEventContext);                                                                              /* SBSW_DEM_POINTER_FORWARD_STACK */
  Dem_DTC_CalculateFailedEffects(&lDTCContext, lEventContext.EventEffects);                                                      /* SBSW_DEM_POINTER_FORWARD_STACK */

  if ( (lEventContext.OldEventStatus != lEventContext.NewEventStatus)
#if (DEM_CFG_SUPPORT_RETRY_STORAGE == STD_ON)
    || (Dem_Event_InternalStatus_TestEventStored(EventId) == FALSE)                                                              /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
#endif
    || ( (Dem_Event_IsSimilarConditionsEnabled() == TRUE)                                                                        /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      && (Dem_Event_TestSimilarConditions(lEventContext.EventId) == TRUE)
      && (Dem_Core_Event_TestMultiTripTargetReached(lEventContext.EventId) == TRUE)
      && (Dem_Event_UDSStatus_TestBit(lEventContext.OldEventStatus, Dem_UdsStatus_CDTC) == FALSE)
       )
     )
  {
    if (Dem_Event_UDSStatus_IsNvRelevantStatusChange(lEventContext.OldEventStatus, lEventContext.NewEventStatus) == TRUE)
    {
      Dem_Nvm_SetStatusBlockChanged();
    }

    Dem_Event_DataUpdateStart(EventId);

    /* Reset aging of the event on a test failed trigger */
    if ((lDTCContext.DtcEffects & DEM_CFG_TRIGGER_TESTFAILED) != 0u)
    {
# if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
      Dem_Core_Aging_ProcessOnFailed(EventId);
#endif

      if (Dem_Core_StatusIndicator_isStatusIndicatorEnabled() == TRUE)
      {
        /*Rest the AgedDTC bit on the SI30 byte*/
        Dem_StatusIndicator_StatusType lStatusIndicatorByte = Dem_Core_StatusIndicator_Get(EventId);
        Dem_Core_StatusIndicator_Set(EventId, Dem_Core_StatusIndicator_ResetBit(lStatusIndicatorByte, Dem_StatusIndicator_ADTC));
        Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS), DEM_NVM_BLOCKSTATE_DIRTY);
      }
    }

    Dem_Core_Event_UpdateTripCounterOnFailed(EventId, lEventContext.OldEventStatus);

    if ( (Dem_Cfg_EventUdsDtc(EventId) != DEM_CFG_DTC_UDS_INVALID)
      && (Dem_GlobalDiagnostics_TestEventStorageConditionsSatisfied(EventId) == TRUE))
    {

      Dem_DTC_UpdateMemIndependentCycleCounter(EventId, lDTCContext.DtcEffects);
      if ((lDTCContext.DtcEffects & DEM_CFG_TRIGGER_TESTFAILEDTHISCYCLE) != 0u)
      {
        Dem_Core_Event_SetEntryIndependentMaxFdcSlc(EventId, 127);
      }

      Dem_Event_CalculateFailedEffects_OnStorage(&lEventContext);                                                                /* SBSW_DEM_POINTER_FORWARD_STACK */
      Dem_DTC_CalculateFailedEffects_OnStorage(&lDTCContext, lEventContext.EventEffects);                                        /* SBSW_DEM_POINTER_FORWARD_STACK */

      Dem_DTC_ProcessStorage(&lEventContext, &lDTCContext);                                                                      /* SBSW_DEM_POINTER_FORWARD_STACK */

      if ((lEventContext.EventEffects & DEM_CFG_TRIGGER_PENDING) != 0u)
      {
          Dem_Mar_IncrementPendingEventsIfNeeded(lEventContext.EventId);
      }

      Dem_Event_CalculateStatusOnFailed_OnStorage(&lEventContext);                                                               /* SBSW_DEM_POINTER_FORWARD_STACK */
      Dem_Core_Event_SetInternalUdsStatus(EventId, lEventContext.NewEventStatus);

      if ((Dem_Event_UDSStatus_TestBit(lEventContext.OldEventStatus, Dem_UdsStatus_CDTC) == FALSE)
        && (Dem_Event_UDSStatus_TestBit(lEventContext.NewEventStatus, Dem_UdsStatus_CDTC) == TRUE))
      {
        Dem_GlobalDiagnostics_IncrementConfirmedEventsIfNeeded(lEventContext.EventId);
        Dem_GlobalDiagnostics_OdometerData_ProcessConfirmedDTCChanged();
      }

      if (lDTCContext.DtcEffects != DEM_CFG_TRIGGER_NONE)
      {
        Dem_DTC_ProcessFailedEffects(&lDTCContext);                                                                              /* SBSW_DEM_POINTER_FORWARD_STACK */

        if (Dem_DTC_TestEventStoresPermanentDTC(EventId) == TRUE)
        {
          if ((Dem_Event_UDSStatus_TestBit(lEventContext.OldEventStatus, Dem_UdsStatus_CDTC) == FALSE)
            && (Dem_Event_UDSStatus_TestBit(lEventContext.NewEventStatus, Dem_UdsStatus_CDTC) == TRUE))
          {
            Dem_EventIdType lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);
            Dem_Memories_PermanentMemory_ConfirmedChrono_AddEvent(lMasterEventId, FALSE);
          }
        }
      }

      if (lEventContext.EventEffects != DEM_CFG_TRIGGER_NONE)
      {
        if (Dem_Core_DTC_IsObdRelated(lEventContext.EventId) == TRUE)
        {
          Dem_Event_QualifyStatus(&lEventContext);                                                                               /* SBSW_DEM_POINTER_FORWARD_STACK */
        }
      }
      Dem_DiagnosticObserver_ProcessEventFailed(&lEventContext);                                                                 /* SBSW_DEM_POINTER_FORWARD_STACK */

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
      Dem_Memories_PrestorageMemory_StateSetDiscard();
#endif
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS),
                                  DEM_NVM_BLOCKSTATE_DIRTY);
    }
    Dem_Event_DataUpdateFinish(EventId);
  }

  Dem_Notifications_EventStatusChanged(lEventContext.EventId,
                                       lEventContext.OldExternalEventStatus,
                                       Dem_Event_UDSStatus_GetExternalUDSStatus(lEventContext.EventId, lEventContext.NewEventStatus));
  if (lEventContext.DataModified == TRUE)
  {
    Dem_Notifications_EventDataChanged(lEventContext.EventId);
  }
  Dem_Notifications_DtcStatusChanged(lDTCContext.EventId,
                                     lDTCContext.OldExternalDtcStatus,
                                     Dem_DTC_GetExternalUDSStatus(EventId),
                                     DEM_NOTIFICATIONS_DTC_UPDATED);
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STPTH */

/* ****************************************************************************
 % Dem_Event_OpCycle
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
Dem_Event_OpCycle(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr                                                                    /* PRQA S 3673 */ /* MD_DEM_8.13_Internal */
)
{
  Dem_UdsStatusByteType lOldEventStatus;
  Dem_UdsStatusByteType lOldExternalEventStatus;
  Dem_UdsStatusByteType lNewEventStatus;

  lOldEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
  lOldExternalEventStatus = Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, lOldEventStatus);

  if ((ArgPtr->Specific.EventOpCycle_Internal.CycleStatus & DEM_OPERATIONCYCLE_CYCLE_STOPPED) != 0U)
  {
    Dem_DiagnosticObserver_ProcessEventOpCycleEnd(EventId);
    Dem_Event_OpCycleEnd(EventId);
  }
  if ((ArgPtr->Specific.EventOpCycle_Internal.CycleStatus & DEM_OPERATIONCYCLE_CYCLE_STARTED) != 0U)
  {
    Dem_Event_OpCycleStart(EventId);
  }
  lNewEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);

  if (Dem_Event_UDSStatus_IsNvRelevantStatusChange(lOldEventStatus, lNewEventStatus) == TRUE)
  {
    Dem_Nvm_SetStatusBlockChanged();
  }

  if (Dem_InitState_TestMasterInState(DEM_INITSTATE_INITIALIZED) == TRUE)
  {
    Dem_Notifications_EventStatusChanged(EventId,
                                         lOldExternalEventStatus,
                                         Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, lNewEventStatus));
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_ResetTestFailed
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ResetTestFailed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Event_ContextType lEventContext;
  Dem_DTC_ContextType lDTCContext;

  Dem_Event_InitEventContext(EventId, &lEventContext);                                                                           /* SBSW_DEM_POINTER_FORWARD_STACK */
  Dem_DTC_InitDTCContext(EventId, &lDTCContext);                                                                                 /* SBSW_DEM_POINTER_FORWARD_STACK */

  lEventContext.NewEventStatus = Dem_Event_UDSStatus_ResetBit(lEventContext.OldEventStatus, Dem_UdsStatus_TF);
  Dem_Core_Event_SetInternalUdsStatus(EventId, lEventContext.NewEventStatus);

  Dem_DTC_ResetFdcToc(EventId);                                                                                                  /* SBSW_DEM_POINTER_FORWARD_STACK */

  if (Dem_Event_UDSStatus_IsNvRelevantStatusChange(lEventContext.OldEventStatus, lEventContext.NewEventStatus) == TRUE)
  {
    Dem_Nvm_SetStatusBlockChanged();
  }

  Dem_Notifications_EventStatusChanged(lEventContext.EventId,
                                       lEventContext.OldExternalEventStatus,
                                       Dem_Event_UDSStatus_GetExternalUDSStatus(lEventContext.EventId, lEventContext.NewEventStatus));
  if (lEventContext.DataModified == TRUE)
  {
    Dem_Notifications_EventDataChanged(lEventContext.EventId);
  }
  Dem_Notifications_DtcStatusChanged(lDTCContext.EventId,
                                     lDTCContext.OldExternalDtcStatus,
                                     Dem_DTC_GetExternalUDSStatus(EventId),
                                     DEM_NOTIFICATIONS_DTC_UPDATED);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_Aged
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Aged(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr                                                                    /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_Internal */
  )
{
  Dem_UdsStatusByteType lOldEventStatus;
  Dem_UdsStatusByteType lOldExternalEventStatus;
  Dem_UdsStatusByteType lNewEventStatus;

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ArgPtr)                                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lOldEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
  lOldExternalEventStatus = Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, lOldEventStatus);
  lNewEventStatus = lOldEventStatus;

  /* remove the aged event from the primary memory */
  lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_CDTC);

  if (Dem_Core_Aging_GetAgingTarget(EventId) == 0u)
  {
# if (DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON)
    if (Dem_Event_UDSStatus_TestBit(lOldEventStatus, Dem_UdsStatus_PDTC) == TRUE)
    {
      Dem_Event_SetTripCount(EventId, 0);
    }
# endif

    Dem_Mar_DecrementPendingEventsIfNeeded(EventId);

    lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_PDTC);
    lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_TFTOC);
  }
# if (DEM_CFG_PROCESS_TFSLC_CLEARONLY != STD_ON)
  lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_TFSLC);
# endif
  Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, lNewEventStatus);

/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
  Dem_EnterCritical_DiagMonitor();


# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
  if (Dem_Event_InternalStatus_TestEventStored(EventId) == TRUE)
  {
    if (ArgPtr->Specific.Event_Aged.IsAgingOnlyEntry == FALSE)
    {
      Dem_Event_InternalStatus_SetStoredStatus(EventId, Dem_InternalStatus_StoredStatus_Aged);
    }
    else
    {
       /* 'aging only' entry is always removed */
      Dem_Event_InternalStatus_SetStoredStatus(EventId, Dem_InternalStatus_StoredStatus_None);
    }
  }
# else
   Dem_Event_InternalStatus_SetStoredStatus(EventId, Dem_InternalStatus_StoredStatus_None);
# endif

  {
    Dem_Event_InternalStatusType lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
    lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_FDCTOC);
    Dem_Event_InternalStatus_SetEventStatus(EventId, lInternalStatus);
  }

  Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

  if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
  { /* only OBD related events supports the qualification of Confirmed and WIR bit */
    Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId,
      Dem_Event_InternalStatus_QualifyStatus_ResetBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_CDTC));
  }

  if (Dem_Event_UDSStatus_IsNvRelevantStatusChange(lOldEventStatus, lNewEventStatus) == TRUE)
  {
    Dem_Nvm_SetStatusBlockChanged();
  }

  if ((Dem_InitState_TestMasterInState(DEM_INITSTATE_INITIALIZED) == TRUE)
    && (ArgPtr->Specific.Event_Aged.SuppressNotifications == FALSE))
  {
    Dem_Notifications_EventStatusChanged(EventId,
                                         lOldExternalEventStatus,
                                         Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, lNewEventStatus));
  }
}                                                                                                                                /* PRQA S 6010, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_ProcessCleared
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_ProcessCleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lEventCleared;

  lEventCleared = FALSE;
#if (DEM_CFG_NOTIFY_CLEAR_ALLOWED == STD_ON)
  if (Dem_ClearTask_ClearEventAllowed(EventId) == TRUE)
#endif
  {
    Dem_DTC_Cleared_Internal(EventId);
    lEventCleared = TRUE;
  }
  Dem_Notifications_InitMonitorForEvent(EventId, DEM_INIT_MONITOR_CLEAR);

  return lEventCleared;
}

/* ****************************************************************************
 % Dem_Event_Cleared
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Cleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr                                                                    /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_Internal */
  )
{
  Dem_Event_InternalStatusType lInternalStatus;
  Dem_UdsStatusByteType lOldEventStatus;
  Dem_UdsStatusByteType lOldExternalEventStatus;

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ArgPtr)                                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */

    Dem_SatelliteCore_ClearEvent(
      Dem_Cfg_EventSatelliteId(EventId),
      Dem_Cfg_EventSatelliteEventId(EventId));

  /* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
  Dem_EnterCritical_DiagMonitor();
  Dem_Event_InternalStatus_SetStoredStatus(EventId, Dem_InternalStatus_StoredStatus_None);

  lOldEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
  lOldExternalEventStatus = Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, lOldEventStatus);

  lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
  lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_FDCTOC);

  if (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    Dem_ExtendedStatus_StatusType lNewEventStatus = DEM_EXT_STATUS_INITIALIZE;

    if (Dem_GlobalDiagnostics_IsDisableEventMonitorSupported() == TRUE)
    {
      Dem_ExtendedStatus_StatusType lOldExtendedEventStatus = Dem_Event_ExtendedStatus_GetEventStatus(EventId);

      /* Do not reset disabled event monitor bit. */
      if (Dem_Event_ExtendedStatus_TestBit(lOldExtendedEventStatus, Dem_ExtendedStatus_MonitorDisabledTOC) == TRUE)
      {
        lNewEventStatus = Dem_Event_ExtendedStatus_SetBit(lNewEventStatus, Dem_ExtendedStatus_MonitorDisabledTOC);
      }
    }

    /* Reset extended status */
    Dem_Event_ExtendedStatus_SetEventStatus(EventId, lNewEventStatus);
  }

  Dem_Event_InternalStatus_SetEventStatus(EventId, lInternalStatus);

  Dem_Mar_DecrementPendingEventsIfNeeded(EventId);

  /* Reset status byte */
  Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, DEM_UDS_STATUS_INITIALIZE);
  /* EventStatus is marked modified by the caller to reduce runtime overhead */

  Dem_LeaveCritical_DiagMonitor();
  /* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

  Dem_DiagnosticObserver_ProcessEventCleared(EventId, lOldEventStatus);

  if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
  { /* only OBD related events supports the qualification of Confirmed and WIR bit */
    Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId, DEM_QUALIFICATION_STATUS_ALL_UNQUALIFIED);
  }

  if (Dem_Cfg_EventPrestorageIndex(EventId) != DEM_PRESTORAGEMEMORY_NOT_CONFIGURED_FOR_EVENT)
  {
    (void)Dem_Memories_PrestorageMemory_EventPrestorageClear(EventId);
  }

#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
  if ((Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
    && (Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 == Dem_Memories_ObdFreezeFrameMemory_GetSize()))
  { /* No Freeze Frame visible in Mode02, scan for one */
    Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 = Dem_Memories_ObdFreezeFrameMemory_SelectHighestPriorFreezeFrame();
  }
#endif

  Dem_Core_Event_ResetTripAndHealingCounterOnClear(EventId,
    (Dem_Event_UDSStatus_TestBit(lOldEventStatus, Dem_UdsStatus_PDTC) == TRUE));

  Dem_Notifications_EventStatusChanged(EventId,
    lOldExternalEventStatus,
    DEM_UDS_STATUS_INITIALIZE);
  }                                                                                                                              /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
/* ****************************************************************************
 % Dem_Event_Displaced
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Displaced(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr                                                                    /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_Internal */
  )
{
  Dem_Event_InternalStatusType lInternalStatus;
# if (DEM_FEATURE_NEED_STOREDONLY_BITS == STD_ON)
  Dem_UdsStatusByteType lOldEventStatus;
  Dem_UdsStatusByteType lOldExternalEventStatus;
  Dem_UdsStatusByteType lNewEventStatus;
# endif

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ArgPtr)                                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */

/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
  Dem_EnterCritical_DiagMonitor();

  Dem_Event_InternalStatus_SetStoredStatus(EventId, Dem_InternalStatus_StoredStatus_None);
  
  lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(EventId);
  lInternalStatus = Dem_Event_InternalStatus_ResetBit(lInternalStatus, Dem_InternalStatus_FDCTOC);
  Dem_Event_InternalStatus_SetEventStatus(EventId, lInternalStatus);

  Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

/* Reset the SI30 byte */
  Dem_Core_StatusIndicator_ResetSI30byte(EventId);

# if (DEM_FEATURE_NEED_STOREDONLY_BITS == STD_ON)
  lOldEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
  lOldExternalEventStatus = Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, lOldEventStatus);
  lNewEventStatus = lOldEventStatus;
# endif
  /* Update the status bit */
# if (DEM_CFG_PROCESS_PDTC_STOREDONLY == STD_ON)
  
  Dem_Mar_DecrementPendingEventsIfNeeded(EventId);

  lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_PDTC);
# endif
# if (DEM_CFG_PROCESS_CDTC_STOREDONLY == STD_ON)
  lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_CDTC);
  if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
  {
    Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId,
      Dem_Event_InternalStatus_QualifyStatus_ResetBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_CDTC));
  }
# endif
# if (DEM_CFG_PROCESS_TFSLC_STOREDONLY == STD_ON)
  lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_TFSLC);
# endif
# if (DEM_FEATURE_NEED_STOREDONLY_BITS == STD_ON)
  Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, lNewEventStatus);
# endif
  /* Displacement follows an event status change. No need to mark Nv modification again. */

# if (DEM_FEATURE_NEED_STOREDONLY_BITS == STD_ON)
  Dem_Notifications_EventStatusChanged(EventId,
                                       lOldExternalEventStatus,
                                       lNewEventStatus);
# endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/* ****************************************************************************
 % Dem_Event_CustomizedDisplacement
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
Dem_Event_CustomizedDisplacement(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr                                                                    /* PRQA S 3673 */ /* MD_DEM_8.13_Internal */
  )
{
  Dem_UdsStatusByteType lOldEventStatus;
  Dem_UdsStatusByteType lOldExternalEventStatus;
  Dem_UdsStatusByteType lNewEventStatus;

/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
  Dem_EnterCritical_DiagMonitor();
  Dem_Event_InternalStatus_SetStoredStatus(EventId, Dem_InternalStatus_StoredStatus_None);
  Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

  lOldEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
  lOldExternalEventStatus = Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, lOldEventStatus);
  lNewEventStatus = lOldEventStatus;

  if (DEM_DTC_TEST_PC_STATUS_ADTC(ArgPtr->Specific.Event_CustomizedDisplacement_Internal.SI30) == FALSE)
  { /* Event not aged - modify status byte */
    /* Update the status bit */
    lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_CDTC);
    Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, lNewEventStatus);
    /* Displacement already precludes an event status change. No need to mark Nv modification again. */
  }

  Dem_Notifications_EventStatusChanged(EventId,
                                       lOldExternalEventStatus,
                                       lNewEventStatus);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#endif

/* ****************************************************************************
 % Dem_Event_GetEventUDSStatus
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Event_GetEventUDSStatus(
    CONST(Dem_EventIdType, AUTOMATIC)  EventId,
    CONSTP2VAR(Dem_UdsStatusByteType, AUTOMATIC, DEM_APPL_DATA)  EventUDSStatus
  )
{
  Std_ReturnType lReturnValue;
  lReturnValue = E_NOT_OK;
  if ( (Dem_Event_TestEventAvailable(EventId) == TRUE) )
  {
    *EventUDSStatus = Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId)); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    lReturnValue = E_OK;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Event_GetEventFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Event_GetEventFailed(
    CONST(Dem_EventIdType, AUTOMATIC)  EventId,
    CONSTP2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  EventFailed
    )
{
  Std_ReturnType lReturnValue;
  lReturnValue = E_NOT_OK;
  if ( (Dem_Event_TestEventAvailable(EventId) == TRUE) )
  {
    if (Dem_Event_UDSStatus_TestBit(Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId), Dem_UdsStatus_TF) == TRUE)
    {
      *EventFailed = TRUE;                                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      *EventFailed = FALSE;                                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    lReturnValue = E_OK;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Event_GetEventTested
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Event_GetEventTested(
    CONST(Dem_EventIdType, AUTOMATIC)  EventId,
    CONSTP2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  EventTested
  )
{
  Std_ReturnType lReturnValue;
  lReturnValue = E_NOT_OK;
  if ( (Dem_OperationCycle_GetCurrentCycleStatus(Dem_Cfg_EventOperationCycle(EventId)) == DEM_OPERATIONCYCLE_CYCLE_STARTED)
    && (Dem_Event_TestEventAvailable(EventId) == TRUE)                                                                           /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    )
  {
    if (Dem_Event_UDSStatus_TestBit(Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId), Dem_UdsStatus_TNCTOC) == FALSE)
    {
      *EventTested = TRUE;                                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      *EventTested = FALSE;                                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    lReturnValue = E_OK;
  }
  /* else: with stopped cycle the bit value is undecided */
  return lReturnValue;
}

#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
/* ****************************************************************************
 % Dem_Event_CycleQualifyDcy
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Event_CycleQualifyDcy(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr                                                                    /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_Internal */
)
{
  Dem_Event_ContextType lEventContext;

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ArgPtr)                                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Event_InitEventContext(EventId, &lEventContext);                                                                           /* SBSW_DEM_POINTER_FORWARD_STACK */


  if ( (Dem_Event_InternalStatus_QualifyStatus_TestBit(lEventContext.QualifyStatus, Dem_QualifyStatus_CDTC) == FALSE)
    && (Dem_Event_UDSStatus_TestBit(lEventContext.OldEventStatus, Dem_UdsStatus_CDTC) == TRUE)
    )
  { /* set confirmed status to qualified */
    lEventContext.QualifyStatus = Dem_Event_InternalStatus_QualifyStatus_SetBit(lEventContext.QualifyStatus, Dem_QualifyStatus_CDTC);
    Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId, lEventContext.QualifyStatus);
    lEventContext.EventEffects = (lEventContext.EventEffects | DEM_CFG_TRIGGER_CONFIRMED);

# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
    {
      uint8 lEventQualificationStatus = ArgPtr->Specific.CycleQualifyDcy_Internal.EventQualificationStatus;
      ArgPtr->Specific.CycleQualifyDcy_Internal.EventQualificationStatus = (lEventQualificationStatus | DEM_UDS_STATUS_CDTC);    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
# endif
  }

  if ( (Dem_Event_InternalStatus_QualifyStatus_TestBit(lEventContext.QualifyStatus, Dem_QualifyStatus_WIR) == FALSE)
    && (Dem_Event_UDSStatus_TestBit(lEventContext.OldEventStatus, Dem_UdsStatus_WIR) == TRUE))
  { /* set warningIndicatorRequested status to qualified */
    lEventContext.QualifyStatus = Dem_Event_InternalStatus_QualifyStatus_SetBit(lEventContext.QualifyStatus, Dem_QualifyStatus_WIR);
    Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId, lEventContext.QualifyStatus);
    lEventContext.EventEffects = (lEventContext.EventEffects | DEM_CFG_TRIGGER_INDICATOR);
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
    {
      uint8 lEventQualificationStatus = ArgPtr->Specific.CycleQualifyDcy_Internal.EventQualificationStatus;
      ArgPtr->Specific.CycleQualifyDcy_Internal.EventQualificationStatus = (lEventQualificationStatus | DEM_UDS_STATUS_WIR);     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
# endif
  }

  if (Dem_Core_Event_UpdateTrigger_TestConfirmed(lEventContext.EventEffects))
  {
    Dem_ExtendedStatus_StatusType lExtendedStatus = Dem_Event_ExtendedStatus_GetEventStatus(lEventContext.EventId);
    lExtendedStatus = Dem_Event_ExtendedStatus_SetBit(lExtendedStatus, Dem_ExtendedStatus_QCSLC);
    Dem_Event_ExtendedStatus_SetEventStatus(lEventContext.EventId, lExtendedStatus);
  }

  Dem_DiagnosticObserver_ProcessDcyOfEventQualified(&lEventContext);                                                             /* SBSW_DEM_POINTER_FORWARD_STACK */

  Dem_Notifications_EventStatusChanged(EventId,
                                       lEventContext.OldExternalEventStatus,
                                       Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, lEventContext.OldEventStatus));
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) */

#if ((DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) \
    && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF) \
    && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_Event_ProcessDCYQualificationCombinedDTC_StoredOnly
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
Dem_Event_ProcessDCYQualificationCombinedDTC_StoredOnly(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_DTC_ForEachCombinedArgPtrType, AUTOMATIC)  ArgPtr
  )
{
  Dem_Cfg_CombinedGroupIndexType lCurrentlyProcessingGroupId;

  lCurrentlyProcessingGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  if (lCurrentlyProcessingGroupId != DEM_CFG_COMBINED_GROUP_INVALID)
  {
    /* DTC has combined group */
    Dem_MemoryEntry_AllocMemEntriesIterType lStoredEventsIter;

    for (Dem_FaultMemory_MemoryEntry_ChronoIter_Init(DEM_CFG_MEMORYID_PRIMARY, &lStoredEventsIter);                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_FaultMemory_MemoryEntry_ChronoIter_Exists(&lStoredEventsIter) == TRUE;                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_FaultMemory_MemoryEntry_ChronoIter_Next(&lStoredEventsIter))                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_Cfg_EntryIndexType lEventEntryIndex;
      Dem_EventIdType lSubEventId;
      Dem_Cfg_CombinedGroupIndexType lSubEventIdCombinedGroup;

      lEventEntryIndex = Dem_FaultMemory_MemoryEntry_ChronoIter_GetEntryIndex(&lStoredEventsIter);                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
      lSubEventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);

# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
      /* Skip aged Events */
      if (Dem_Event_InternalStatus_GetStoredStatus(lSubEventId) == Dem_InternalStatus_StoredStatus_Aged)
      {
        continue;                                                                                                                
      }
#endif

      if (Dem_Event_TestValidHandle(lSubEventId) == FALSE)
      {
        Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE)
        continue;                                                                                                                
      }

      lSubEventIdCombinedGroup = Dem_Cfg_EventCombinedGroup(lSubEventId);
      if (lSubEventIdCombinedGroup == lCurrentlyProcessingGroupId)
      {
        if ( (Dem_Event_TestEventAvailable(lSubEventId) == TRUE) )
        {
          Dem_Event_CycleQualifyDcy(lSubEventId, ArgPtr);                                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
          Dem_Event_UpdateQualificationStatus(lSubEventId, lEventEntryIndex);
        }
      }
    }
    Dem_DTC_SetQualificationProcessedCombinedDTC(lCurrentlyProcessingGroupId);
    Dem_Event_UDSStatus_CalculateAndUpdateCombinedGroupStatus(lCurrentlyProcessingGroupId);
    ArgPtr->Common.NewDtcStatus = Dem_Event_UDSStatus_GetCombinedGroupStatus(lCurrentlyProcessingGroupId);                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif


#if ((DEM_FEATURE_NEED_EMISSIONOBD == STD_ON) \
    && (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF) \
    && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_Event_ProcessDCYQualificationNonCombinedDTC_StoredOnly
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ProcessDCYQualificationNonCombinedDTC_StoredOnly(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  Dem_DTC_ForEachCombinedArgPtrType ArgPtr
  )
{
  if (EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    Dem_Event_CycleQualifyDcy(EventId, ArgPtr);                                                                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Event_UpdateQualificationStatus(EventId, EventEntryIndex);
  }
  ArgPtr->Common.NewDtcStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif


/* ****************************************************************************
 % Dem_Event_TestLatchedTestFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_TestLatchedTestFailed(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lReturnValue;

  if (Dem_Cfg_EventLatchTestFailed(EventId) == TRUE)
  {
    Dem_UDSStatus_StatusType lStatusMask;
    lStatusMask = DEM_UDS_STATUS_TFTOC;

#if (DEM_CFG_LATCH_TF_FOR_CONFIRMED_UNTIL_CLEAR == STD_ON)
    lStatusMask |= DEM_UDS_STATUS_CDTC;
#endif

    lReturnValue =
      (boolean)(((lStatusMask)& Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId)) != 0u);
  }
  else
  {
    lReturnValue = FALSE;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Event_InitNvData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InitNvData(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, DEM_UDS_STATUS_INITIALIZE);

#if (DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON)
  Dem_Event_SetTripCount(EventId, 0);
#endif
  Dem_Event_ExtendedStatus_SetEventStatus(EventId, DEM_EXT_STATUS_INITIALIZE);
#if (DEM_CFG_SUPPORT_SI30_STATUS == STD_ON)
  Dem_Core_StatusIndicator_Set(EventId, DEM_STATUSINDICATOR_QUEUE_INITIAL);
  Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS), DEM_NVM_BLOCKSTATE_DIRTY);
#endif
#if (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
  if (Dem_Core_DTC_IsObdRelated(EventId) == FALSE)
  {
    Dem_DTC_SetQualifyStatus(EventId, DEM_QUALIFICATION_STATUS_ALL_QUALIFIED);
  }
  else
  {
    Dem_DTC_SetQualifyStatus(EventId, DEM_QUALIFICATION_STATUS_ALL_UNQUALIFIED);
  }
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_Init
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_Init(
  void
  )
{
  Dem_EventIdType lEventId;

  Dem_Event_InitQualification();
  Dem_Event_CombinedStorage_SharedCounter_ResetAllHealingCounterIncrementedThisCycle();
  Dem_DTC_InitQualification();

#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
  /* ----- MIL group handling at DCY restart ----- */
  if (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    uint16 lMilGroupIndex;
    lMilGroupIndex = Dem_Cfg_GlobalMilGroupCount();
    while (lMilGroupIndex != DEM_CFG_MIL_GROUP_INVALID)
    {
      lMilGroupIndex--;
      Dem_Event_InternalStatus_SetMilGroupPendingCount(lMilGroupIndex, 0);
    }
  }
#endif

  for (lEventId = DEM_EVENT_FIRST; lEventId < Dem_Cfg_GlobalEventCount(); ++lEventId)
  {
    Dem_Event_InitEventInternalStatus(lEventId);
    Dem_Event_InitEventStatus(lEventId);
    Dem_Event_RestoreAvailabilityFromNv(lEventId);

#if (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
    /* Set qualification state for all UDS DTCs to qualified */
    if (Dem_Core_DTC_IsObdRelated(lEventId) == FALSE)
    {
      Dem_DTC_SetQualifyStatus(lEventId, DEM_QUALIFICATION_STATUS_ALL_QUALIFIED);
    }
#endif
    if (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lEventId), Dem_InternalStatus_Disconnected) == TRUE)
    {
      Dem_Iumpr_SetRatioAvailability(lEventId, Dem_Iumpr_DisconnectRatio);
    }
    else
    {
      Dem_Iumpr_SetRatioAvailability(lEventId, Dem_Iumpr_ReconnectRatio);
    }
  } /* for each event */

    /* Initialize transactional update commit numbers */
  Dem_MemSet((Dem_DataPtrType)&Dem_Cfg_GetEventDataCommitNumber(0), DEM_EVENT_INIT_DATA_COMMIT_NUMBER, Dem_Cfg_GetSizeOfEventDataCommitNumber() * sizeof(Dem_Cfg_GetEventDataCommitNumber(0))); /* PRQA S 0312, 0315, 0602 */ /* MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_MEMSET_EVENTDATACOMMITNUMBER */

#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
  if(Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    uint16 lMilGroupIndex;
    lMilGroupIndex = Dem_Cfg_GlobalMilGroupCount();
    while (lMilGroupIndex != DEM_CFG_MIL_GROUP_INVALID)
    {
      lMilGroupIndex--;
      if (Dem_Event_InternalStatus_GetMilGroupPendingCount(lMilGroupIndex) == 0u)
      {
        Dem_Mem_ResetMilGroupTripCount(lMilGroupIndex);
      }
    }
  }
#endif
}                                                                                                                                /* PRQA S  6010, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_Event_PreInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_PreInit(
  void
  )
{
  Dem_EventIdType lEventId;
  for (lEventId = DEM_EVENT_FIRST; lEventId < Dem_Cfg_GlobalEventCount(); ++lEventId)
  {
    if (Dem_Cfg_EventAvailableByDefault(lEventId) == TRUE)
    {
      Dem_Event_InternalStatus_SetEventStatus(lEventId, 0);
    }
    else
    {
      Dem_Event_InternalStatus_SetEventStatus(lEventId, Dem_Event_InternalStatus_SetBit(0u, Dem_InternalStatus_Disconnected));
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_MainFunction
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_MainFunction(
  void
  )
{
#if (DEM_CFG_SUPPORT_EVENTAVAILABLE == STD_ON)
  Dem_EventIdType lEventId;
  for (lEventId = DEM_EVENT_FIRST; lEventId < Dem_Cfg_GlobalEventCount(); ++lEventId)
  {
    Dem_Event_ProcessEventAvailable(lEventId);
  }
#endif
}

/* ****************************************************************************
 % Dem_Event_GetRatioIdx
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_RatioIdIndexType, DEM_CODE)
Dem_Event_GetRatioIdx(
CONST(Dem_EventIdType, AUTOMATIC) EventId                                                                                        /* PRQA S 3206 */ /* MD_DEM_3206 */
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_IUMPR == STD_ON)
  return ((Dem_RatioIdIndexType)(Dem_Cfg_GetRatioIdIndexOfEventTable(EventId)));
#else
  return DEM_CFG_RATIOINDEX_INVALID;
#endif
}


/* ****************************************************************************
 % Dem_Event_GetManIdx
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_MARIndexOfMonitorActivityRatioTableType, DEM_CODE)
Dem_Event_GetManIdx(
  CONST(Dem_EventIdType, AUTOMATIC) EventId                                                                                      /* PRQA S 3206 */ /* MD_DEM_3206 */
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
    return ((Dem_Cfg_MARIndexOfMonitorActivityRatioTableType)(Dem_Cfg_GetMARIndexOfMonitorActivityRatioTable(
      Dem_Cfg_GetMonitorActivityRatioTableIdxOfEventTable(EventId)))
      );
#else
    return  (Dem_Cfg_MARIndexOfMonitorActivityRatioTableType)DEM_CFG_MONITOR_ACTIVITY_RATIO_INDEX_INVALID;
#endif
}

/* ****************************************************************************
 % Dem_Event_GetSmarIdx
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType, DEM_CODE)
Dem_Event_GetSmarIdx(                                                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC) EventId                                                                                      /* PRQA S 3206 */ /* MD_DEM_3206 */
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_DATA_MONITOR_ACTIVITY_DATA == STD_ON)
    return ((Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType)(Dem_Cfg_GetSMARIndexOfMonitorActivityRatioTable(
      Dem_Cfg_GetMonitorActivityRatioTableIdxOfEventTable(EventId)))
      );
#else
    return  (Dem_Cfg_SMARIndexOfMonitorActivityRatioTableType)DEM_CFG_MONITOR_ACTIVITY_RATIO_INDEX_INVALID;
#endif
}

/* ****************************************************************************
 % Dem_Event_ResetEventReporting
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ResetEventReporting(
  Dem_EventIdType EventId
)
{
  Dem_SatelliteCore_RestartEvent(
    Dem_Cfg_EventSatelliteId(EventId),
    Dem_Cfg_EventSatelliteEventId(EventId));

  if ((Dem_InitState_TestMasterInState(DEM_INITSTATE_INITIALIZED) == TRUE)
      && (Dem_Core_DTC_IsObdRelated(EventId) == TRUE))
  {
    Dem_Event_ExtendedStatus_SetEventStatus(EventId,
      Dem_Event_ExtendedStatus_ResetBit(Dem_Event_ExtendedStatus_GetEventStatus(EventId), Dem_ExtendedStatus_SimilarConditions)
    );
  }
  Dem_DTC_ResetFdcToc(EventId);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_InitUdsFromStatusIndicator
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
Dem_Event_InitUdsFromStatusIndicator(
  Dem_EventIdType EventId
)
{
  if (Dem_Core_StatusIndicator_isStatusIndicatorEnabled() == TRUE)
  {
    Dem_StatusIndicator_StatusType lStatusIndicatorByte;
    Dem_UdsStatusByteType lEventStatus;
    Dem_UdsStatusByteType lOrigEventStatus;
    lEventStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
    lOrigEventStatus = lEventStatus;

    lStatusIndicatorByte = Dem_Core_StatusIndicator_Get(EventId);

    if ((Dem_Core_Event_TestHasIndicator(EventId) == TRUE)
      && (Dem_Event_UDSStatus_TestBit(lEventStatus, Dem_UdsStatus_WIR) == TRUE)
      && (Dem_Core_StatusIndicator_TestBit(lStatusIndicatorByte, Dem_StatusIndicator_WIRSLC) == FALSE))
    {
      lEventStatus = Dem_Event_UDSStatus_ResetBit(lEventStatus, Dem_UdsStatus_WIR);
    }

    if ((Dem_Event_UDSStatus_TestBit(lEventStatus, Dem_UdsStatus_TFSLC) == TRUE)
      && (Dem_Core_StatusIndicator_TestBit(lStatusIndicatorByte, Dem_StatusIndicator_TFSLC) == FALSE))
    {
      lEventStatus = Dem_Event_UDSStatus_ResetBit(lEventStatus, Dem_UdsStatus_TFSLC);
    }
    if ((Dem_Event_UDSStatus_TestBit(lEventStatus, Dem_UdsStatus_TFSLC) == FALSE)
      && (Dem_Core_StatusIndicator_TestBit(lStatusIndicatorByte, Dem_StatusIndicator_TFSLC) == TRUE))
    {
      lEventStatus = Dem_Event_UDSStatus_SetBit(lEventStatus, Dem_UdsStatus_TFSLC);
    }

    if (lOrigEventStatus != lEventStatus)
    {
      Dem_Event_UDSStatus_SetInternalEventUdsStatus(EventId, lEventStatus);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Core_Event_GetDestination
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Core_Event_GetDestination(
  Dem_EventIdType EventId                                                                                                        /* PRQA S 3206 */ /* MD_DEM_3206 */
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return (uint16)(Dem_Cfg_GetEventDestinationOfEventTable(EventId));
}

/* ****************************************************************************
 % Dem_Core_Event_GetObdOnUds3ByteDtc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Core_Event_GetObdOnUds3ByteDtc(
  Dem_EventIdType EventId
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_VARIANT_POSTBUILD_LOADABLE == STD_ON)
  return Dem_Cfg_GetObdDtc3ByteOfDtcNumberTable(Dem_Cfg_GetDtcNumberTableIdxOfEventTable(EventId));
#else
  return Dem_Cfg_GetObdDtc3ByteOfDtcTable(Dem_Cfg_GetDtcTableIdxOfEventTable(EventId));                                          /* PRQA S 2843 */ /* MD_DEM_Dir4.1_ReadOperation */
#endif
}

/* ****************************************************************************
 % Dem_Core_Event_IndicatorIterInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_IndicatorIterInit(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                             /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Cfg_ComplexIterInit(IterPtr, Dem_Cfg_GetIndicatorTableIndStartIdxOfEventTable(EventId),                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      Dem_Cfg_GetIndicatorTableIndEndIdxOfEventTable(EventId));
}


/* ****************************************************************************
 % Dem_Core_Event_TestHasIndicator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_TestHasIndicator(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean lReturnValue;

  Dem_Cfg_ComplexIterType lIndicatorIter;
  Dem_Core_Event_IndicatorIterInit(EventId, &lIndicatorIter);                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
  if (Dem_Cfg_ComplexIterExists(&lIndicatorIter) == TRUE)                                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    lReturnValue = TRUE;
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Core_Event_MultiEventIterInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_MultiEventIterInit(
  Dem_EventIdType EventId,                                                                                                       /* PRQA S 3206 */ /* MD_DEM_3206 */
  Dem_Cfg_ComplexIterType * const IterPtr                                                                                        /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  )
{
  Dem_Cfg_ComplexIterInit(IterPtr,                                                                                               /* PRQA S 3112 */ /* MD_DEM_14.2 */
    Dem_Cfg_GetMultiEventTriggeringSlaveEventsTableStartIdxOfEventPrecompileDataTable(EventId),                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_Cfg_GetMultiEventTriggeringSlaveEventsTableEndIdxOfEventPrecompileDataTable(EventId));

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId);                                                                                     /* PRQA S 1338 */ /* MD_MSR_DummyStmt */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr);
}

/* ****************************************************************************
 % Dem_Core_Event_MultiEventIterGet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_Core_Event_MultiEventIterGet(                                                                                                /* PRQA S 3206 */ /* MD_DEM_3206 */
  Dem_Cfg_ComplexIterType * const IterPtr                                                                                        /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr);
  return Dem_Cfg_GetEventPrecompileDataTableIdxOfMultiEventTriggeringSlaveEventsTable(Dem_Cfg_ComplexIterGet(IterPtr));          /* SBSW_DEM_CALL_DEBOUNCEINFO_POINTER */
}

/* ****************************************************************************
 % Dem_Event_QualifyStatus
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_QualifyStatus(
  CONSTP2VAR(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext                                                           /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
# if (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_OFF)
  Dem_InternalStatus_StoredStatusType lStoredStatus = Dem_Event_InternalStatus_GetStoredStatus(EventContext->EventId);

  /* Event is currently active (normal case) or aging (happens with 0-Trip events) */
  if ( (lStoredStatus == Dem_InternalStatus_StoredStatus_Active)
    || (lStoredStatus == Dem_InternalStatus_StoredStatus_Aging) )
# endif
  {
    uint8 lEventQualifyStatus = EventContext->QualifyStatus;

    if ((DEM_OPERATIONCYCLE_TEST_DCY_QUALIFIED(Dem_OperationCycle_GetObdCycleStates()) == TRUE)
      || (Dem_Event_TestObdVisibilityDelayed(EventContext->EventId) == FALSE)
      || ((Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE) && (EventContext->QualifyEvent == TRUE)))
    {
      /* The event has confirmed */
      if ((EventContext->EventEffects & DEM_CFG_TRIGGER_CONFIRMED) != 0u)
      {
        Dem_ExtendedStatus_StatusType lExtendedStatus;
        lEventQualifyStatus = Dem_Event_InternalStatus_QualifyStatus_SetBit(lEventQualifyStatus, Dem_QualifyStatus_CDTC);

        lExtendedStatus = Dem_Event_ExtendedStatus_GetEventStatus(EventContext->EventId);
        lExtendedStatus = Dem_Event_ExtendedStatus_SetBit(lExtendedStatus, Dem_ExtendedStatus_QCSLC);
        Dem_Event_ExtendedStatus_SetEventStatus(EventContext->EventId, lExtendedStatus);
      }
      /* The event has requested the MIL */
      if ((EventContext->EventEffects & DEM_CFG_TRIGGER_INDICATOR) != 0u)
      {
        lEventQualifyStatus = Dem_Event_InternalStatus_QualifyStatus_SetBit(lEventQualifyStatus, Dem_QualifyStatus_WIR);
      }
      EventContext->QualifyStatus = lEventQualifyStatus;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      Dem_DTC_SetQualifyStatus(EventContext->EventId, lEventQualifyStatus);
    }


# if (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
    /* If configured that DTC status is independent from stored status, always set
     * NV status data block to DIRTY */
    Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS),
                                DEM_NVM_BLOCKSTATE_DIRTY);
# else
    {
      Dem_Cfg_EntryIndexType lEventEntryIndex
        = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventContext->EventId, TRUE);

      /* synchronize the qualification state with the primary event entry */
      if (DEM_MEM_SET_NON_QUALIFIED_STATUS_BITS(Dem_MemoryEntry_EventEntry_GetState(lEventEntryIndex)) != lEventQualifyStatus)
      {
        Dem_MemoryEntry_EventEntry_SetState(lEventEntryIndex,
          DEM_MEM_APPLY_QUALIFIED_STATUS(Dem_MemoryEntry_EventEntry_GetState(lEventEntryIndex), lEventQualifyStatus));
        Dem_Nvm_SetEventEntryNvBlockState(lEventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
      }
    }
# endif
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventContext)                                                                                 /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Event_IsCalculatedSRecsUsed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsCalculatedSRecsUsed(                                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  uint16 lTranslatedMemoryId = Dem_Core_Event_GetDestination(EventId);
  return (boolean)Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculated(Dem_FaultMemory_Memories_GetSRecNumerationType(lTranslatedMemoryId));
}

/* ****************************************************************************
 % Dem_Event_IsCalculatedFifoSRecsUsed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsCalculatedFifoSRecsUsed(                                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  uint16 lTranslatedMemoryId = Dem_Core_Event_GetDestination(EventId);
  return (boolean)Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculatedFiFo(Dem_FaultMemory_Memories_GetSRecNumerationType(lTranslatedMemoryId));
}

/* ****************************************************************************
 % Dem_Event_IsConfiguredSRecsUsed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_IsConfiguredSRecsUsed(                                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  uint16 lTranslatedMemoryId = Dem_Core_Event_GetDestination(EventId);
  return (boolean)Dem_Memories_EventMemory_UserDefined_IsSRecNumerationConfigured(Dem_FaultMemory_Memories_GetSRecNumerationType(lTranslatedMemoryId));
}


/* ****************************************************************************
 % Dem_Core_Event_TestEventAffectedByPto
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_TestEventAffectedByPto(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType  EventId
  )
{
  boolean lReturnValue = FALSE;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  if (Dem_GlobalDiagnostics_IsPtoStatusSupportEnabled() == TRUE)
  {
    lReturnValue = Dem_Cfg_IsEventConsiderPtoStatusOfEventPrecompileDataTable(EventId);
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Core_Event_ProcessOBDCycleEnd
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_Event_ProcessOBDCycleEnd(
  uint8 CycleId
  )
{
  if (CycleId == Dem_Cfg_GlobalObdDrivingCycleId())
  {
    Dem_Event_CombinedStorage_SharedCounter_ResetAllHealingCounterIncrementedThisCycle();
  }
}

/* ****************************************************************************
 % Dem_Core_Event_IsCustomTriggeredSrecConfigured
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_IsCustomTriggeredSrecConfigured(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType  EventId                                                                                                       /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return Dem_Cfg_IsCustomTriggeredSnapshotConfiguredOfEventTable(EventId);
}

/* ****************************************************************************
 % Dem_Core_Event_IsCustomTriggeredTssrConfigured
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_Event_IsCustomTriggeredTssrConfigured(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType  EventId                                                                                                       /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return Dem_Cfg_IsCustomTriggeredTimeSeriesSnapshotConfiguredOfEventTable(EventId);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* DEM_EVENTIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EventIF_Implementation.h
 *********************************************************************************************************************/
