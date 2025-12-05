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
/*! \addtogroup Dem_EventEntry
 *  \{
 *  \file       Dem_EventEntry_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Implementation of the Event Entry unit which handles access to and from event entries.
 *  \entity     EventEntry
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EVENTENTRY_IMPLEMENTATION_H)
#define DEM_EVENTENTRY_IMPLEMENTATION_H

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
#include "Dem_EventEntry_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Client_Interface.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_EVENTENTRY_IMPLEMENTATION_FILENAME "Dem_EventEntry_Implementation.h"

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/
#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define DEM_START_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define DEM_STOP_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */




/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_EventEntry_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_Get
 *****************************************************************************/
/*!
 * \brief         Get the N-th event entry
 *
 * \details       Get the N-th event entry
 *
 * \param[in]     EventEntryIndex
 *                The event entry handle selects the index 'N'
 *
 * \return        Pointer to the N-th event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SharedEventEntryPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_Get(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );


/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetOccurrenceCounter
 *****************************************************************************/
/*!
 * \brief         Set the occurrence counter in the event entry
 *
 * \details       Set the occurrence counter in the event entry
 *
 * \param[in]     EventEntryIndex
 *                The event entry handle
 * \param[in]     OccurrenceCounter
 *                The occurrence counter to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(Dem_EventEntry_OccurrenceCounterType, AUTOMATIC) OccurrenceCounter
  );

/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_ResetOccurrenceCounter
 *****************************************************************************/
/*!
 * \brief         Reset the occurrence counter in the event entry
 *
 * \details       Reset the occurrence counter in the event entry
 *
 * \param[in]     EventEntryIndex
 *                The event entry handle
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_ResetOccurrenceCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex
  );


/* ****************************************************************************
 * Dem_MemoryEntry_EventEntry_SetFailedCycleCounter
 *****************************************************************************/
/*!
 * \brief         Set the 'Failed Cycle Counter' in the event entry
 *
 * \details       Set the 'Failed Cycle Counter' in the event entry
 *
 * \param[in]     EventEntryIndex
 *                The event entry handle
 * \param[in]     FailedCycleCounter
 *                The 'Failed Cycle Counter' to be set in the event entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetFailedCycleCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) FailedCycleCounter
  );

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_Get
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SharedEventEntryPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_Get(
  Dem_Cfg_EntryIndexType EventEntryIndex
)
{
  Dem_Cfg_SharedEventEntryPtrType lMemoryEntry;
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EventEntryIndex >= Dem_Cfg_GetSizeOfEventEntryPtr())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENTENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lMemoryEntry = Dem_Cfg_GetEventEntryPtr(0u);                                                                                 /* PRQA S 0310, 3305 */ /* MD_DEM_11.4_nvm, MD_DEM_3305 */
  }
  else
# endif
  {
    lMemoryEntry = Dem_Cfg_GetEventEntryPtr(EventEntryIndex);                                                                    /* PRQA S 0310, 3305, 2841, 2842, 2852 */ /* MD_DEM_11.4_nvm, MD_DEM_3305, MD_DEM_Dir4.1_EventEntryPointerAccess, MD_DEM_Dir4.1_EventEntryPointerAccess, MD_DEM_Dir1.1_EventEntryPtrTableAccess */
  }

  return lMemoryEntry;
}


/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetOccurrenceCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(Dem_EventEntry_OccurrenceCounterType, AUTOMATIC) OccurrenceCounter
  )
{
#if (DEM_FEATURE_NEED_OCC_COUNTER == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OccurrenceCounter = OccurrenceCounter;                                        /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OccurrenceCounter)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}


/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_ResetOccurrenceCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_ResetOccurrenceCounter(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
#if (DEM_FEATURE_NEED_OCC_COUNTER == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OccurrenceCounter = DEM_DATA_OCCURRENCE_INIT;                                 /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}


/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetFailedCycleCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetFailedCycleCounter(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) FailedCycleCounter
  )
{
#if (DEM_CFG_DATA_FAILED_CYCLES == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_OFF)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->FailedCycleCounter = FailedCycleCounter;                                      /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FailedCycleCounter)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_EventEntry_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetEventId(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  return (Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->EventId);
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetEventId(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  )
{
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->EventId = EventId;                                                            /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
}


/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_Free
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_Free(
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_MemoryEntry_EventEntry_SetEventId(EventEntryIndex, DEM_EVENT_INVALID);
  Dem_MemoryEntry_EventEntry_SetTimestamp(EventEntryIndex, 0);
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetTimestamp
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetTimestamp(                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  return (Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->Timestamp);
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetTimestamp
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetTimestamp(                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint32, AUTOMATIC) Timestamp
  )
{
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->Timestamp = Timestamp;                                                        /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetMaxFdcSlc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetMaxFdcSlc(                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  sint8 lMaxFDCSinceLastClear = 0;
#if (DEM_CFG_DATA_MAX_FDC_SINCE_LAST_CLEAR == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_MAX_FDC_SLC == STD_OFF)
  lMaxFDCSinceLastClear = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->MaxFDCSinceLastClear;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
  return lMaxFDCSinceLastClear;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetMaxFdcSlc
 *****************************************************************************/
/*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetMaxFdcSlc(                                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  sint8 FdcValue
  )
{
#if (DEM_CFG_DATA_MAX_FDC_SINCE_LAST_CLEAR == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_MAX_FDC_SLC == STD_OFF)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->MaxFDCSinceLastClear = FdcValue;                                              /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FdcValue)                                                                                     /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetState(                                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lState;
#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) || (DEM_CFG_SUPPORT_WWHOBD == STD_ON) || (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
  lState = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->State;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lState = 0u;
#endif
  return lState;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetState(                                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) State
  )
{
#if (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON) || (DEM_CFG_SUPPORT_WWHOBD == STD_ON) || (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->State = State;                                                                /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(State)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetAgingTargetCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetAgingTargetCycle(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint16 lTargetCycle;
#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_AGING_FOR_ALL_DTCS == STD_OFF))
  lTargetCycle = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->AgingTargetCycle;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lTargetCycle = 0u;
#endif
  return lTargetCycle;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetAgingTargetCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetAgingTargetCycle(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  uint16 TargetCycle
  )
{
#if ((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF) && (DEM_CFG_SUPPORT_AGING_FOR_ALL_DTCS == STD_OFF))
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->AgingTargetCycle = TargetCycle;                                               /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TargetCycle)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetAgingTimer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetAgingTimer(                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint32 lAgingTimer;
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  lAgingTimer = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->AgingTimer;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lAgingTimer = 0u;
#endif
  return lAgingTimer;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetAgingTimer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetAgingTimer(                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint32, AUTOMATIC) AgingTimer
  )
{
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->AgingTimer = AgingTimer;                                                      /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(AgingTimer)                                                                                   /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetFailedCycleCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetFailedCycleCounter(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lFailedCycleCounter;
#if (DEM_CFG_DATA_FAILED_CYCLES == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_OFF)
  lFailedCycleCounter = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->FailedCycleCounter;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lFailedCycleCounter = 0u;
#endif
  return lFailedCycleCounter;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetConsecutiveFailedCycleCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetConsecutiveFailedCycleCounter(                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lConsecutiveFailedCycleCounter = 0u;
#if ( (DEM_CFG_DATA_CONSECUTIVE_FAILED_CYCLES == STD_ON) \
   || ((DEM_CFG_DATA_FAULT_PENDING_COUNTER == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_OFF)))
  lConsecutiveFailedCycleCounter = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->ConsecutiveFailedCycleCounter;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
  return lConsecutiveFailedCycleCounter;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetConsecutiveFailedCycleCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetConsecutiveFailedCycleCounter(                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
 Dem_Cfg_EntryIndexType EventEntryIndex,
 CONST(uint8, AUTOMATIC) ConsecutiveFailedCycleCounter
 )
{
#if ( (DEM_CFG_DATA_CONSECUTIVE_FAILED_CYCLES == STD_ON) \
  || ((DEM_CFG_DATA_FAULT_PENDING_COUNTER == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_OFF)))
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->ConsecutiveFailedCycleCounter = ConsecutiveFailedCycleCounter;                /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ConsecutiveFailedCycleCounter)                                                                /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetCyclesSinceFirstFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetCyclesSinceFirstFailed(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint16 lCyclesSinceFirstFailed;
#if (DEM_CFG_DATA_CYCLES_SINCE_FIRST_FAILED == STD_ON)
  lCyclesSinceFirstFailed = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->CyclesSinceFirstFailed;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lCyclesSinceFirstFailed = 0u;
#endif
  return lCyclesSinceFirstFailed;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetCyclesSinceFirstFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetCyclesSinceFirstFailed(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint16, AUTOMATIC) CyclesSinceFirstFailed
  )
{
#if (DEM_CFG_DATA_CYCLES_SINCE_FIRST_FAILED == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->CyclesSinceFirstFailed = CyclesSinceFirstFailed;                              /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CyclesSinceFirstFailed)                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetTestedCyclesSinceFirstFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetTestedCyclesSinceFirstFailed(                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lTestedCyclesSinceFirstFailed;
#if (DEM_CFG_DATA_CYCLES_TESTED_SINCE_FIRST_FAILED == STD_ON)
  lTestedCyclesSinceFirstFailed = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->TestedCyclesSinceFirstFailed;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lTestedCyclesSinceFirstFailed = 0u;
#endif
  return lTestedCyclesSinceFirstFailed;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetTestedCyclesSinceFirstFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetTestedCyclesSinceFirstFailed(                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) TestedCyclesSinceFirstFailed
  )
{
#if (DEM_CFG_DATA_CYCLES_TESTED_SINCE_FIRST_FAILED == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->TestedCyclesSinceFirstFailed = TestedCyclesSinceFirstFailed;                  /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TestedCyclesSinceFirstFailed)                                                                 /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetCyclesSinceLastFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetCyclesSinceLastFailed(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint16 lCyclesSinceLastFailed;
#if (DEM_CFG_DATA_CYCLES_SINCE_LAST_FAILED == STD_ON)
  lCyclesSinceLastFailed = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->CyclesSinceLastFailed;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lCyclesSinceLastFailed = 0u;
#endif
  return lCyclesSinceLastFailed;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetCyclesSinceLastFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetCyclesSinceLastFailed(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint16, AUTOMATIC) CyclesSinceLastFailed
  )
{
#if (DEM_CFG_DATA_CYCLES_SINCE_LAST_FAILED == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->CyclesSinceLastFailed = CyclesSinceLastFailed;                                /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CyclesSinceLastFailed)                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
% Dem_MemoryEntry_EventEntry_GetWarmUpCyclesSinceLastFailed
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetWarmUpCyclesSinceLastFailed(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint16 lWarmUpCyclesSinceLastFailed;
#if (DEM_CFG_DATA_WUC_SINCE_LAST_FAILED == STD_ON)
  lWarmUpCyclesSinceLastFailed = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->WUCSinceLastFailed;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lWarmUpCyclesSinceLastFailed = 0u;
#endif
  return lWarmUpCyclesSinceLastFailed;
}

/* ****************************************************************************
% Dem_MemoryEntry_EventEntry_SetWarmUpCyclesSinceLastFailed
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetWarmUpCyclesSinceLastFailed(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint16, AUTOMATIC) WarmUpCyclesSinceLastFailed
  )
{
#if (DEM_CFG_DATA_WUC_SINCE_LAST_FAILED == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->WUCSinceLastFailed = WarmUpCyclesSinceLastFailed;                             /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(WarmUpCyclesSinceLastFailed)                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetPreConfirmedStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetPreConfirmedStatus(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lPreconfirmedStatus;
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  lPreconfirmedStatus = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->StatusIndicator;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lPreconfirmedStatus = 0u;
#endif
  return lPreconfirmedStatus;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetPreConfirmedStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetPreConfirmedStatus(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) StatusIndicator
  )
{
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->StatusIndicator = StatusIndicator;                                            /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(StatusIndicator)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetOccurrenceCounter1
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter1(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lOccurrenceCounter1;
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  lOccurrenceCounter1 = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OCC1;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lOccurrenceCounter1 = 0u;
#endif
  return lOccurrenceCounter1;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetOccurrenceCounter1
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter1(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) OccurrenceCounter1
  )
{
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OCC1 = OccurrenceCounter1;                                                    /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OccurrenceCounter1)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetOccurrenceCounter2
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter2(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lOccurrenceCounter2;
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  lOccurrenceCounter2 = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OCC2;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lOccurrenceCounter2 = 0u;
#endif
  return lOccurrenceCounter2;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetOccurrenceCounter2
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter2(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) OccurrenceCounter2
  )
{
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OCC2 = OccurrenceCounter2;                                                    /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OccurrenceCounter2)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetOccurrenceCounter3
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter3(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lOccurrenceCounter3;
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  lOccurrenceCounter3 = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OCC3;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lOccurrenceCounter3 = 0u;
#endif
  return lOccurrenceCounter3;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetOccurrenceCounter3
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter3(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) OccurrenceCounter3
  )
{
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OCC3 = OccurrenceCounter3;                                                    /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OccurrenceCounter3)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetOccurrenceCounter4
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter4(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lOccurrenceCounter4;
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  lOccurrenceCounter4 = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OCC4;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lOccurrenceCounter4 = 0u;
#endif
  return lOccurrenceCounter4;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetOccurrenceCounter4
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter4(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) OccurrenceCounter4
  )
{
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OCC4 = OccurrenceCounter4;                                                    /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OccurrenceCounter4)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetOccurrenceCounter7
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter7(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lOccurrenceCounter7;
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  lOccurrenceCounter7 = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OCC7;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lOccurrenceCounter7 = 0u;
#endif
  return lOccurrenceCounter7;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetOccurrenceCounter7
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetOccurrenceCounter7(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) OccurrenceCounter7
  )
{
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OCC7 = OccurrenceCounter7;                                                    /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OccurrenceCounter7)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetStdEnvData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetStdEnvData(                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC)  DataIndex
  )
{
  uint8 lDataElement;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DataIndex)                                                                                    /* PRQA S 3112 */ /* MD_DEM_14.2 */
# if (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
  if (DataIndex < DEM_EVENTENTRY_EXT_ENV_DATA_SIZE)
  {
    lDataElement = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->StdEnvData[DataIndex];
  }
  else
# elif (DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON)
  if (DataIndex < DEM_EVENTENTRY_STD_ENV_DATA_SIZE)
  {
    lDataElement = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->StdEnvData[DataIndex];
  }
  else
# endif
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
    lDataElement = 0u;
  }
  return lDataElement;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetStdEnvData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetStdEnvData(                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC)  DataIndex,
  CONST(uint8, AUTOMATIC)  DataValue
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DataIndex)                                                                                    /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DataValue)                                                                                    /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON) || (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (DataIndex >= DEM_EVENTENTRY_ENV_DATA_SIZE)
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENTENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->StdEnvData[DataIndex] = DataValue;                                          /* SBSW_DEM_WRITE_EVENTENTRY_STDENVDATA */
  }
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetSnapshotHeader
 *****************************************************************************/
/*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetSnapshotHeader(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lSnapshotHeader;
#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  lSnapshotHeader = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->SnapshotHeader;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lSnapshotHeader = 0u;
#endif
  return lSnapshotHeader;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetSnapshotHeader
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetSnapshotHeader(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) SnapshotHeader
  )
{
#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->SnapshotHeader = SnapshotHeader;                                              /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SnapshotHeader)                                                                               /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetSnapshotHeaderFifo
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetSnapshotHeaderFifo(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType  EventEntryIndex
)
{
  uint8 lSnapshotHeaderFifo;
#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  lSnapshotHeaderFifo = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->SnapshotHeaderFifo;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lSnapshotHeaderFifo = 0u;
#endif
  return lSnapshotHeaderFifo;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetSnapshotHeaderFifo
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetSnapshotHeaderFifo(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_EntryIndexType  EventEntryIndex,
  uint8  SnapshotHeaderFifo
)
{
#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->SnapshotHeaderFifo = SnapshotHeaderFifo;                                      /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SnapshotHeaderFifo)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}


/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetCombinedDTCSnapshotHeader
 *****************************************************************************/
/*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetCombinedDTCSnapshotHeader(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint32 lCombinedDTCSnapshotHeader;
#if ((DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)\
     && (DEM_CFG_SUPPORT_SRECS == STD_ON))
  lCombinedDTCSnapshotHeader = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->CombinedDTCSnapshotHeader;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lCombinedDTCSnapshotHeader = 0x00U;
#endif
  return lCombinedDTCSnapshotHeader;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetCombinedDTCSnapshotHeader
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetCombinedDTCSnapshotHeader(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint32, AUTOMATIC) CombinedDTCSnapshotHeader
  )
{
#if ((DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)\
     && (DEM_CFG_SUPPORT_SRECS == STD_ON))
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->CombinedDTCSnapshotHeader = CombinedDTCSnapshotHeader;                        /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CombinedDTCSnapshotHeader)                                                                    /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetCombinedDTCExtendedHeader
 *****************************************************************************/
/*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetCombinedDTCExtendedHeader(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint32 lCombinedDTCExtendedHeader;
#if ((DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) && (DEM_CFG_SUPPORT_USER_ERECS == STD_ON))
  lCombinedDTCExtendedHeader = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->CombinedDTCExtendedHeader;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lCombinedDTCExtendedHeader = 0u;
#endif
  return lCombinedDTCExtendedHeader;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetCombinedDTCExtendedHeader
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetCombinedDTCExtendedHeader(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint32, AUTOMATIC) CombinedDTCExtendedHeader
  )
{
#if ((DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) && (DEM_CFG_SUPPORT_USER_ERECS == STD_ON))
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->CombinedDTCExtendedHeader = CombinedDTCExtendedHeader;                        /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CombinedDTCExtendedHeader)                                                                    /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetExtendedHeader
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetExtendedHeader(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lExtendedHeader;
#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
  lExtendedHeader = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->ExtendedHeader;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lExtendedHeader = 0u;
#endif
  return lExtendedHeader;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetExtendedHeader
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetExtendedHeader(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC) ExtendedHeader
  )
{
#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->ExtendedHeader = ExtendedHeader;                                              /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ExtendedHeader)                                                                               /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetSnapshotRecordDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetSnapshotRecordDataPtr(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC)  SRecIndex
  )
{
  Dem_SharedDataPtrType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (SRecIndex >= Dem_Cfg_GlobalSRecMaxCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENTENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &(Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->SnapshotData[0][0]);
  }
  else
# endif
  {
    lReturnValue = &(Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->SnapshotData[SRecIndex][0]);
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetSnapshotRecordDataConstPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_ConstSharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetSnapshotRecordDataConstPtr(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(Dem_Cfg_SRecIndexType, AUTOMATIC)  SRecIndex
  )
{
  Dem_ConstSharedDataPtrType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (SRecIndex >= Dem_Cfg_GlobalSRecMaxCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENTENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &(Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->SnapshotData[0][0]);
  }
  else
# endif
  {
    lReturnValue = &(Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->SnapshotData[SRecIndex][0]);
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetExtendedRecordDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetExtendedRecordDataPtr(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC)  ERecIndex
  )
{
  Dem_SharedDataPtrType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (ERecIndex >= Dem_Cfg_GlobalERecMaxCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENTENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &(Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->ExtendedData[0][0]);
  }
  else
# endif
  {
    lReturnValue = &(Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->ExtendedData[ERecIndex][0]);
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetExtendedRecordDataConstPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_ConstSharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetExtendedRecordDataConstPtr(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(uint8, AUTOMATIC)  ERecIndex
  )
{
  Dem_ConstSharedDataPtrType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (ERecIndex >= Dem_Cfg_GlobalERecMaxCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENTENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &(Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->ExtendedData[0][0]);
  }
  else
# endif
  {
    lReturnValue = &(Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->ExtendedData[ERecIndex][0]);
  }
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetWwhObdFreezeFrameDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetWwhObdFreezeFrameDataPtr(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_SharedDataPtrType lReturnValue;

  lReturnValue = &(Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->ObdFreezeFrameData[0]);
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_J1939_FREEZEFRAME == STD_ON) || (DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetJ1939FreezeFramePtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetJ1939FreezeFramePtr(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_SharedDataPtrType lReturnValue;
  lReturnValue = &(Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->J1939FreezeFrame[0]);
  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetGlobalSnapshotRecordDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetGlobalSnapshotRecordDataPtr(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_SharedDataPtrType lReturnValue;
  lReturnValue = &(Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->GlobalSnapshotData[0]);
  return lReturnValue;
}
#endif

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_TestGlobalSnapshotRecordStored
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_TestGlobalSnapshotRecordStored(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  boolean lGlobalSnapshotRecordStored;

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
  lGlobalSnapshotRecordStored = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->GlobalSnapshotRecordStored;
#else
  {
    lGlobalSnapshotRecordStored = FALSE;
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
#endif

  return lGlobalSnapshotRecordStored;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetGlobalSnapshotRecordStored
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetGlobalSnapshotRecordStored(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONST(boolean, AUTOMATIC) StoredState
  )
{
#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->GlobalSnapshotRecordStored = StoredState;                                     /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(StoredState)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
}


/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_TestJ1939FFStored
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_TestJ1939FFStored(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  boolean lJ1939FFStored = FALSE;

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
  lJ1939FFStored = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->J1939FFStored;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif

  return lJ1939FFStored;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_SetJ1939FFStoredStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_SetJ1939FFStoredStatus(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  boolean StoredState
  )
{
#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
  Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->J1939FFStored = StoredState;                                                  /* SBSW_DEM_POINTER_WRITE_EVENTENTRY */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(StoredState)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_EventEntry_Private Private Methods
 * \{
 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_EventEntry_Public
 * \{
 */


/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_TestEntryForEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_TestEntryForEvent(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  Dem_EventIdType EventId
  )
{
  return (boolean)(Dem_MemoryEntry_EventEntry_GetEventId(EventEntryIndex) == EventId);
}


#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_TestGlobalSnapshotStored
 *****************************************************************************/
/*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_TestGlobalSnapshotStored(
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  return Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->GlobalSnapshotRecordStored;
}
#endif

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetOccurrenceCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventEntry_OccurrenceCounterType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetOccurrenceCounter(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_EventEntry_OccurrenceCounterType lOccurrenceCounter;
#if (DEM_FEATURE_NEED_OCC_COUNTER == STD_ON)
  lOccurrenceCounter = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex)->OccurrenceCounter;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  lOccurrenceCounter = 0u;
#endif
  return lOccurrenceCounter;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_Init(
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_Cfg_SharedEventEntryPtrType lEntry;
  lEntry = Dem_MemoryEntry_EventEntry_Get(EventEntryIndex);
  Dem_MemCpy((Dem_SharedDataPtrType)lEntry,                                                                                      /* PRQA S 0310, 0311, 0315, 0602 */ /* MD_DEM_11.4_cpy, MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_MEMCPY_PRIMARYENTRY */
    (Dem_ConstDataPtrType)(&Dem_Cfg_MemoryEntryInit),
    sizeof(Dem_Cfg_PrimaryEntryType));
}

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_CopyEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_CopyEntry(
  Dem_Cfg_EntryIndexType DestinationEntryIndex,
  Dem_Cfg_EntryIndexType SourceEntryIndex
  )
{
  Dem_Cfg_ConstSharedEventEntryPtrType lSourceEntry;
  Dem_Cfg_SharedEventEntryPtrType lDestinationEntry;
  lSourceEntry = Dem_MemoryEntry_EventEntry_Get(SourceEntryIndex);
  lDestinationEntry = Dem_MemoryEntry_EventEntry_Get(DestinationEntryIndex);
  Dem_MemCpy((Dem_SharedDataPtrType)lDestinationEntry,                                                                           /* PRQA S 0310, 0311, 0315, 0602 */ /* MD_DEM_11.4_cpy, MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_MEMCPY_PRIMARYENTRY */
             (Dem_ConstSharedDataPtrType)lSourceEntry,
             sizeof(Dem_Cfg_PrimaryEntryType));
}
#endif

#if (DEM_FEATURE_NEED_OCC_COUNTER == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_IncrementOccurrenceCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_IncrementOccurrenceCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  boolean lSuccessfulIncrement;
  Dem_EventEntry_OccurrenceCounterType lOccurrenceCounter;
  lSuccessfulIncrement = FALSE;
  lOccurrenceCounter = Dem_MemoryEntry_EventEntry_GetOccurrenceCounter(EventEntryIndex);

  if (lOccurrenceCounter < DEM_DATA_OCCURRENCE_MAX)
  {
    lOccurrenceCounter = (Dem_EventEntry_OccurrenceCounterType)(lOccurrenceCounter + 1u);
    Dem_MemoryEntry_EventEntry_SetOccurrenceCounter(EventEntryIndex, lOccurrenceCounter);
    lSuccessfulIncrement = TRUE;
  }
  return lSuccessfulIncrement;
}
#endif

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_IncrementFailedCycleCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_IncrementFailedCycleCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  boolean lSuccessfulIncrement;
  lSuccessfulIncrement = FALSE;
#if (DEM_CFG_DATA_FAILED_CYCLES == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_OFF)
  {
    uint8 lCycleCounter;

    lCycleCounter = Dem_MemoryEntry_EventEntry_GetFailedCycleCounter(EventEntryIndex);
    if (lCycleCounter < 0xffU)
    {
      lCycleCounter = (uint8)(lCycleCounter + 1u);
      Dem_MemoryEntry_EventEntry_SetFailedCycleCounter(EventEntryIndex, lCycleCounter);
      lSuccessfulIncrement = TRUE;
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
  return lSuccessfulIncrement;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_IncrementConsecutiveFailedCycleCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_IncrementConsecutiveFailedCycleCounter(
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  boolean lSuccessfulIncrement;
  lSuccessfulIncrement = FALSE;
#if ( (DEM_CFG_DATA_CONSECUTIVE_FAILED_CYCLES == STD_ON) \
   || ((DEM_CFG_DATA_FAULT_PENDING_COUNTER == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_OFF)))
  {
    uint8 lCycleCounter;

    lCycleCounter = Dem_MemoryEntry_EventEntry_GetConsecutiveFailedCycleCounter(EventEntryIndex);
    if (lCycleCounter < 0xffU)
    {
      lCycleCounter = (uint8)(lCycleCounter + 1u);
      Dem_MemoryEntry_EventEntry_SetConsecutiveFailedCycleCounter(EventEntryIndex, lCycleCounter);
      lSuccessfulIncrement = TRUE;
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
  return lSuccessfulIncrement;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_InitializeEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_MemoryEntry_EventEntry_InitializeEntry(
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */

#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
  Dem_MemoryEntry_EventEntry_SetGlobalSnapshotRecordStored(EventEntryIndex, FALSE);
#endif
#if (DEM_FEATURE_NEED_OCC_COUNTER == STD_ON)
  Dem_MemoryEntry_EventEntry_ResetOccurrenceCounter(EventEntryIndex);
#endif
#if (((DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)) && (DEM_CFG_SUPPORT_AGING_FOR_ALL_DTCS == STD_OFF))
  Dem_MemoryEntry_EventEntry_SetAgingTargetCycle(EventEntryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID);
#endif
#if (DEM_CFG_SUPPORT_USER_ERECS == STD_ON)
  Dem_MemoryEntry_EventEntry_SetExtendedHeader(EventEntryIndex, 0U);
#endif
#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  Dem_MemoryEntry_EventEntry_SetSnapshotHeader(EventEntryIndex, 0U);
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  Dem_MemoryEntry_EventEntry_SetCombinedDTCSnapshotHeader(EventEntryIndex, 0U);
# endif
  Dem_MemoryEntry_EventEntry_SetSnapshotHeaderFifo(EventEntryIndex, 0U);
#endif
#if ((DEM_CFG_SUPPORT_USER_ERECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
  Dem_MemoryEntry_EventEntry_SetCombinedDTCExtendedHeader(EventEntryIndex, 0U);
#endif
#if (DEM_CFG_DATA_MAX_FDC_SINCE_LAST_CLEAR == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_MAX_FDC_SLC == STD_OFF)
  Dem_MemoryEntry_EventEntry_SetMaxFdcSlc(EventEntryIndex, 0);
#endif
#if ((DEM_CFG_DATA_FAILED_CYCLES == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_OFF))
  Dem_MemoryEntry_EventEntry_SetFailedCycleCounter(EventEntryIndex, 0U);
#endif
#if ( (DEM_CFG_DATA_CONSECUTIVE_FAILED_CYCLES == STD_ON) \
   || ((DEM_CFG_DATA_FAULT_PENDING_COUNTER == STD_ON) && (DEM_CFG_SUPPORT_MEM_INDEPENDENT_CYCLE_COUNTERS == STD_OFF)))
  Dem_MemoryEntry_EventEntry_SetConsecutiveFailedCycleCounter(EventEntryIndex, 0U);
#endif
#if (DEM_CFG_DATA_CYCLES_SINCE_FIRST_FAILED == STD_ON)
  Dem_MemoryEntry_EventEntry_SetCyclesSinceFirstFailed(EventEntryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID);
#endif
#if (DEM_CFG_DATA_CYCLES_SINCE_LAST_FAILED == STD_ON)
  Dem_MemoryEntry_EventEntry_SetCyclesSinceLastFailed(EventEntryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID);
#endif
#if (DEM_CFG_DATA_CYCLES_TESTED_SINCE_FIRST_FAILED == STD_ON)
  Dem_MemoryEntry_EventEntry_SetTestedCyclesSinceFirstFailed(EventEntryIndex, 0U);
#endif
#if (DEM_CFG_DATA_WUC_SINCE_LAST_FAILED == STD_ON)
  Dem_MemoryEntry_EventEntry_SetWarmUpCyclesSinceLastFailed(EventEntryIndex, DEM_OPERATIONCYCLE_CYCLECOUNT_INVALID);
#endif
#if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON) || (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
  Dem_MemoryEntry_EventEntry_SetState(EventEntryIndex, 0U);
#endif
#if (DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON) || (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
  {
    uint8 lDataIndex;
    for (lDataIndex = 0u; lDataIndex < DEM_EVENTENTRY_STD_ENV_DATA_SIZE; lDataIndex++)
    {
      Dem_MemoryEntry_EventEntry_SetStdEnvData(EventEntryIndex, lDataIndex, 0U);
    }
  }
#endif
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  Dem_MemoryEntry_EventEntry_SetPreConfirmedStatus(EventEntryIndex, DEM_EVENTENTRY_PC_STATUS_INITIALIZE);
  Dem_MemoryEntry_EventEntry_SetOccurrenceCounter1(EventEntryIndex, 0U);
  Dem_MemoryEntry_EventEntry_SetOccurrenceCounter2(EventEntryIndex, 0U);
  Dem_MemoryEntry_EventEntry_SetOccurrenceCounter3(EventEntryIndex, 0U);
  Dem_MemoryEntry_EventEntry_SetOccurrenceCounter4(EventEntryIndex, 0U);
  Dem_MemoryEntry_EventEntry_SetOccurrenceCounter7(EventEntryIndex, 0U);
#endif
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    Dem_MemoryEntry_EventEntry_SetAgingTimer(EventEntryIndex, DEM_EVENTENTRY_AGINGTIME_INVALID);
  }
#endif
  Dem_MemoryEntry_EventEntry_SetJ1939FFStoredStatus(EventEntryIndex, FALSE);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_GetEntryIndexToNvBlockIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(Dem_Cfg_NvBlockIndexType, DEM_CODE)
Dem_MemoryEntry_EventEntry_GetEntryIndexToNvBlockIndex(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  Dem_Internal_AssertReturnValue((EventEntryIndex < Dem_Cfg_GetSizeOfEventEntryIndexToNvBlockIndex()), \
    DEM_E_INCONSISTENT_STATE, DEM_CFG_INVALID_NVBLOCK_INDEX)

  return Dem_Cfg_GetEventEntryIndexToNvBlockIndex(EventEntryIndex);                                                              /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_TestAgingOnlyEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_EventEntry_TestAgingOnlyEntry(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  boolean lAgingOnlyEntry;

#if (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  if (DEM_EVENTENTRY_TEST_AGING_ONLY(Dem_MemoryEntry_EventEntry_GetState(EventEntryIndex)) == TRUE)
  {
    lAgingOnlyEntry = TRUE;
  }
  else
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventEntryIndex)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
  {
    lAgingOnlyEntry = FALSE;
  }

  return lAgingOnlyEntry;
}

/* ****************************************************************************
 % Dem_MemoryEntry_EventEntry_TestUpdatable
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
Dem_MemoryEntry_EventEntry_TestUpdatable(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  boolean  lUpdateable;

  if (
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
# if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON)
     (Dem_Event_InternalStatus_GetStoredStatus(EventId) == Dem_InternalStatus_StoredStatus_Aged)
      ||
# endif
#endif
      (Dem_MemoryEntry_EventEntry_TestAgingOnlyEntry(EventEntryIndex) == TRUE)                                                   /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
     )
  {
    lUpdateable = FALSE;
  }
  else
  {
    lUpdateable = TRUE;
  }
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return lUpdateable;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_EVENTENTRY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EventEntry_Implementation.h
 *********************************************************************************************************************/
