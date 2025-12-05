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
/*! \addtogroup Dem_PrestorageMemory
 *  \{
 *  \file       Dem_PrestorageMemory_Implementation.h
 *  \brief      Stores and manages 'prestored freezeframes'.
 *  \details    Pre-store snapshot data for an event, in order to freeze that data set when the event is stored later.
 *  \entity     PrestorageMemory
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_PRESTORAGEMEMORY_IMPLEMENTATION_H)
#define DEM_PRESTORAGEMEMORY_IMPLEMENTATION_H

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
#include "Dem_PrestorageMemory_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Satellite_Interface.h"
#include "Dem_DataStorage_Interface.h"
#include "Dem_MemoryEntry_Interface.h"
#include "Dem_Cfg_Declarations.h"
#include "Dem_StorageManager_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_PRESTORAGEMEMORY_IMPLEMENTATION_FILENAME "Dem_PrestorageMemory_Implementation.h"

/*!
 * \defgroup  Dem_PrestorageMemoryd_PrestoredStatus  Macros encoding prestored status states
 * Macros encoding prestored status states
 * \{
 */
#define DEM_PRESTORAGEMEMORY_STATUS_FREE                                               (0x00U)  /*!< Prestored entry is free */
#define DEM_PRESTORAGEMEMORY_STATUS_READY                                              (0x01U)  /*!< Prestored entry contains valid data */
#define DEM_PRESTORAGEMEMORY_STATUS_UPDATING                                           (0x02U)  /*!< Prestored entry is updating data */
#define DEM_PRESTORAGEMEMORY_STATUS_LOCKED                                             (0x03U)  /*!< Prestored entry is locked */
#define DEM_PRESTORAGEMEMORY_STATUS_DISCARDED                                          (0x04U)  /*!< Prestored entry is locked and needs to be freed */
/*!
 * \}
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/*! Start of prestorage entry free list - index of first free buffer (linked list) or DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY with empty free list */
DEM_LOCAL VAR(Dem_Cfg_PrestorageIndexType, DEM_VAR_NO_INIT)  Dem_PrestorageMemory_FreeListHeadIndex;
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/*! Prestorage entry locked for status processing */
DEM_LOCAL VAR(Dem_Cfg_PrestorageIndexType, DEM_VAR_NO_INIT)  Dem_PrestorageMemory_LockedIndex;
#endif

#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
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
 * \addtogroup Dem_Memories_PrestorageMemory_PublicProperties
 * \{
 */

 /* ****************************************************************************
 % Dem_Memories_PrestorageMemory_isPrestoreFFEnabled()
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_PrestorageMemory_isPrestoreFFEnabled(
  void
  )
{
  return (boolean)(DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON);
}

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_GetLockedIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_PrestorageIndexType, DEM_CODE)
Dem_Memories_PrestorageMemory_GetLockedIndex(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return Dem_PrestorageMemory_LockedIndex;
}
#endif


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
 * \defgroup Dem_PrestorageMemory_Private Private Methods
 * \{
 */

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_FreeListGetNext
 *****************************************************************************/
/*!
 * \brief         Get the next prestorage entry from the list of free buffers.
 *
 * \details       Get the immediate following entry from the linked list of
 *                free buffers.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 *
 * \return        Next prestored entry.
 * \return        DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY
 *                No next prestorage entry.
 *
 * \pre           'PrestorageEntry' is part of the free list.
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_PrestorageIndexType, DEM_CODE)
Dem_Memories_PrestorageMemory_FreeListGetNext(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  );
#endif


#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_FreeListSetNext
 *****************************************************************************/
/*!
 * \brief         Set the 'next' prestorage entry (for list of free buffers).
 *
 * \details       Set the 'PrestorageEntry' to link to the
 *                'NextPrestorageEntry'. This inserts this entry in the list of
 *                free buffers.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 * \param[in]     NextPrestorageEntry
 *                Next free prestorage entry after 'PrestorageEntry'. Use
 *                DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY for no-next-element.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_FreeListSetNext(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry,
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  NextPrestorageEntry
  );
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_AllocateEntry
 *****************************************************************************/
/*!
 * \brief         Allocates a new prestored entry for an event
 *
 * \details       This function consumes the next free prestorage entry (if
 *                available) and allocates it to the given event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Index into Prestorage area, if one was found.
 *                DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY otherwise.
 *
 * \pre           The event must support prestorage
 * \pre           The event must not have a prestored entry
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_PrestorageIndexType, DEM_CODE)
Dem_Memories_PrestorageMemory_AllocateEntry(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_ClearEntry
 *****************************************************************************/
/*!
 * \brief         Free a prestored entry
 *
 * \details       This function removes the ownership of the prestored entry
 *                buffer and returns it to the free list.
 *
 * \param[in]     PrestorageEntry
 *                Prestored entry to free
 *
 * \pre           Must be called within critical section 'DiagMonitor'
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_ClearEntry(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  );
#endif


/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_UpdateForEvent
 *****************************************************************************/
/*!
 * \brief         Store prestored data for an event
 *
 * \details       This function tries to find a prestorage entry for the given
 *                event, allocating a new one if needed.
 *                If a prestorage entry is found, it is updated with sample
 *                data.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        E_OK
 *                Prestored data was successfully collected
 * \return        E_NOT_OK
 *                No data was collected
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Memories_PrestorageMemory_UpdateForEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );


/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_ProcessSatelliteQueueActions
 *****************************************************************************/
/*!
 * \brief         Processes all queued prestore actions.
 *
 * \details       This function processes queued prestore actions for all
 *                events of the given satellite.
 *
 * \param[in]     SatelliteId
 *                Unique handle of the Satellite.
 *                The SatelliteId must be in range [1..Dem_Cfg_GlobalSatelliteCount()[.
 *
 * \pre           Function must only be invoked when the features "Prestorage of
 *                freeze frames" and "Multiple Satellites" are supported.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_ProcessSatelliteQueueActions(
  CONST(Dem_Satellite_IdType, AUTOMATIC)  SatelliteId
  );

/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_LockedForEvent
 *****************************************************************************/
/*!
 * \brief         Returns if a pre-storage entry is locked for the event
 *                for status processing.
 *
 * \details       Returns if a pre-storage entry is locked for the event
 *                for status processing.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Pre-storage entry is locked for the event.
 *                FALSE
 *                No Pre-storage entry is locked or the locked pre-storage
 *                entry does not belong to the event.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_PrestorageMemory_LockedForEvent(
  Dem_EventIdType EventId
);

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_FreeListGetNext
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_PrestorageIndexType, DEM_CODE)
Dem_Memories_PrestorageMemory_FreeListGetNext(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  )
{
  return (Dem_Cfg_PrestorageIndexType) Dem_Cfg_GetPrestorage()[PrestorageEntry].EventId;
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_FreeListSetNext
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_FreeListSetNext(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry,
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  NextPrestorageEntry
  )
{
  Dem_Cfg_GetPrestorage()[Dem_MemoryEntry_PrestorageEntry_Checked(PrestorageEntry)].EventId = (Dem_EventIdType)NextPrestorageEntry; /* SBSW_DEM_ARRAY_WRITE_PRESTORAGE */
}
#endif


#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_AllocateEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_PrestorageIndexType, DEM_CODE)
Dem_Memories_PrestorageMemory_AllocateEntry(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_PrestorageIndexType lPrestorageEntryFirstUnused;
  Dem_Internal_AssertReturnValue((DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY <= Dem_MemoryEntry_PrestorageEntry_GetEntry(EventId)), DEM_E_PARAM_DATA, DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)

  lPrestorageEntryFirstUnused = Dem_PrestorageMemory_FreeListHeadIndex;
  if (lPrestorageEntryFirstUnused != DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
  {
    /* shorten the free list by removing its first element */
    Dem_PrestorageMemory_FreeListHeadIndex = Dem_Memories_PrestorageMemory_FreeListGetNext(lPrestorageEntryFirstUnused);
    /* connect the event with the buffer slot */
    Dem_MemoryEntry_PrestorageEntry_SetEntry(EventId, lPrestorageEntryFirstUnused);
    /* fill the buffer slot with the EventId */
    Dem_MemoryEntry_PrestorageEntry_SetEventId(lPrestorageEntryFirstUnused, EventId);
  }

  return lPrestorageEntryFirstUnused;
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_ClearEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_ClearEntry(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  )
{
  Dem_Internal_AssertReturnVoid(DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY != PrestorageEntry, DEM_E_PARAM_DATA)

  /* un-connect the event from the buffer slot */
  Dem_MemoryEntry_PrestorageEntry_SetEntry(Dem_MemoryEntry_PrestorageEntry_GetEventId(PrestorageEntry), DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY);

  /* enqueue the buffer slot as first element in the free list */
  Dem_Memories_PrestorageMemory_FreeListSetNext(PrestorageEntry, Dem_PrestorageMemory_FreeListHeadIndex);
  Dem_PrestorageMemory_FreeListHeadIndex = PrestorageEntry;
}
#endif

/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_UpdateForEvent
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Memories_PrestorageMemory_UpdateForEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Std_ReturnType lReturnValue;

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
  Dem_Cfg_PrestorageIndexType lPrestorageEntry;

/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
  Dem_EnterCritical_DiagMonitor();

  lPrestorageEntry = Dem_MemoryEntry_PrestorageEntry_GetEntry(EventId);
  if (lPrestorageEntry < DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
  { /* there already is an entry allocated to this event */
    Dem_MemoryEntry_PrestorageEntry_SetState(lPrestorageEntry, DEM_PRESTORAGEMEMORY_STATUS_UPDATING);
  }
  else
  { /* try to find a free entry for this event */
    lPrestorageEntry = Dem_Memories_PrestorageMemory_AllocateEntry(EventId);
    if (lPrestorageEntry != DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
    {
      Dem_MemoryEntry_PrestorageEntry_SetState(lPrestorageEntry, DEM_PRESTORAGEMEMORY_STATUS_UPDATING);
    }
  }

  Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

  if (lPrestorageEntry < DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
  {
    Dem_Memories_PrestorageMemory_UpdateEntryOfEvent(EventId, lPrestorageEntry);
    Dem_MemoryEntry_PrestorageEntry_SetState(lPrestorageEntry, DEM_PRESTORAGEMEMORY_STATUS_READY);
    lReturnValue = E_OK;
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }

#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  lReturnValue = E_NOT_OK;
#endif

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_ProcessSatelliteQueueActions
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
Dem_Memories_PrestorageMemory_ProcessSatelliteQueueActions(
  CONST(Dem_Satellite_IdType, AUTOMATIC)  SatelliteId
  )
{
  Dem_Satellite_MonitorIdType lMonitorId;

  for (lMonitorId = 0u;
       lMonitorId < Dem_Cfg_GetEventCountOfSatelliteInfo(SatelliteId);
       ++lMonitorId)
  {
    if (Dem_Satellite_PeekPrestoreActionQueue(SatelliteId, lMonitorId) != DEM_PRESTORAGEMEMORY_QUEUE_INITIAL)
    {
      Dem_Memories_PrestorageMemory_QueueActionType lPrestoreQueueStatus;                                                        /* PRQA S 0759 */ /* MD_MSR_Union */
      Dem_EventIdType lEventId;

      lEventId = Dem_Satellite_GetEventId(SatelliteId, lMonitorId);
      lPrestoreQueueStatus = Dem_Satellite_ConsumePrestoreActionQueue(SatelliteId, lMonitorId);

# if (DEM_DEV_ERROR_REPORT == STD_ON)
      /*Only events configured for prestorage should be processed here*/
      Dem_Internal_AssertContinue((Dem_Cfg_EventPrestorageIndex(lEventId) != DEM_PRESTORAGEMEMORY_NOT_CONFIGURED_FOR_EVENT), DEM_E_INCONSISTENT_STATE)
      /*Only events not assigned to the master partition should be processed here*/
      Dem_Internal_AssertContinue((Dem_Satellite_CheckEventApplicationId(lEventId) == FALSE), DEM_E_INCONSISTENT_STATE)
# endif

      if (lPrestoreQueueStatus == DEM_PRESTORAGEMEMORY_QUEUE_PRESTOREFF)
      {
        (void)Dem_Memories_PrestorageMemory_UpdateForEvent(lEventId);
      }
      else
      {
        (void)Dem_Memories_PrestorageMemory_EventPrestorageClear(lEventId);
      }
      
    }
  }
}

/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_LockedForEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_PrestorageMemory_LockedForEvent(
  Dem_EventIdType EventId
)
{
  boolean lReturnValue = FALSE;

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
  if ( (Dem_PrestorageMemory_LockedIndex != DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
    && (Dem_MemoryEntry_PrestorageEntry_GetEventId(Dem_PrestorageMemory_LockedIndex) == EventId) )
  {
    lReturnValue = TRUE;
  }
#endif

  return lReturnValue;
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
 * \addtogroup Dem_Memories_PrestorageMemory_Public
 * \{
 */

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_Init
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
Dem_Memories_PrestorageMemory_Init(
  void
  )
{
  Dem_Cfg_PrestorageIndexType lPrestorageEntry;
  uint16_least lPrestorageIndex;

  /* here: initialize the temporary element too, so DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY is the last valid index */
  for (lPrestorageEntry = 0u; lPrestorageEntry < DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY; ++lPrestorageEntry)
  { /* Chain the blocks */
    Dem_MemoryEntry_PrestorageEntry_SetState(lPrestorageEntry, DEM_PRESTORAGEMEMORY_STATUS_FREE);
    Dem_Memories_PrestorageMemory_FreeListSetNext(lPrestorageEntry, (Dem_Cfg_PrestorageIndexType)(lPrestorageEntry + 1u));
  }
  /* Last block points to end, head points to first block */
  Dem_MemoryEntry_PrestorageEntry_SetState(DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY, DEM_PRESTORAGEMEMORY_STATUS_FREE);
  Dem_Memories_PrestorageMemory_FreeListSetNext(DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY, DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY);
  Dem_PrestorageMemory_FreeListHeadIndex = 0u;

  /* no event has currently allocated a buffer */
  for (lPrestorageIndex = 0u; lPrestorageIndex < Dem_Cfg_GetSizeOfPrestorageIndex(); ++lPrestorageIndex)
  {
    Dem_Cfg_SetPrestorageIndex(lPrestorageIndex, DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY);                                     /* SBSW_DEM_ARRAY_WRITE_PRESTORAGEINDEX */
  }

  /* Initialize the prestorage lock state */
  Dem_PrestorageMemory_LockedIndex = DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY;

  /* Set prestorage tasks on scheduler to enabled*/
  Dem_Scheduler_EnableTask(Dem_Scheduler_Task_Prestorage);
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_StateSetDiscard
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_StateSetDiscard(
  void
  )
{
  if (Dem_PrestorageMemory_LockedIndex != DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
  {
    Dem_MemoryEntry_PrestorageEntry_SetState(Dem_PrestorageMemory_LockedIndex, DEM_PRESTORAGEMEMORY_STATUS_DISCARDED);
  }
}
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_EventPrestorageLock
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
Dem_Memories_PrestorageMemory_EventPrestorageLock(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_PrestorageIndexType lPrestoredEntry;

  if (Dem_Cfg_EventPrestorageIndex(EventId) != DEM_PRESTORAGEMEMORY_NOT_CONFIGURED_FOR_EVENT)
  {
    lPrestoredEntry = Dem_MemoryEntry_PrestorageEntry_GetEntry(EventId);
    if (lPrestoredEntry < DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
    {
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
      Dem_EnterCritical_DiagMonitor();

      if (Dem_MemoryEntry_PrestorageEntry_GetState(lPrestoredEntry) == DEM_PRESTORAGEMEMORY_STATUS_READY)
      {
        Dem_PrestorageMemory_LockedIndex = lPrestoredEntry;
        Dem_MemoryEntry_PrestorageEntry_SetState(lPrestoredEntry, DEM_PRESTORAGEMEMORY_STATUS_LOCKED);
        Dem_MemoryEntry_PrestorageEntry_SetEntry(EventId, DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY);
      }

      Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_EventPrestorageRelease
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
Dem_Memories_PrestorageMemory_EventPrestorageRelease(
  void
  )
{
  if (Dem_PrestorageMemory_LockedIndex != DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
  {
    Dem_Cfg_PrestorageIndexType lPrestorageEntry;
    Dem_EventIdType lEventId;

    lPrestorageEntry = Dem_PrestorageMemory_LockedIndex;
    lEventId = Dem_MemoryEntry_PrestorageEntry_GetEventId(lPrestorageEntry);

/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();

    if (Dem_MemoryEntry_PrestorageEntry_GetState(lPrestorageEntry) == DEM_PRESTORAGEMEMORY_STATUS_LOCKED)
    { /* Entry unused, return it to the event */
      Dem_Cfg_PrestorageIndexType lPrestorageIndex;
      lPrestorageIndex = Dem_MemoryEntry_PrestorageEntry_GetEntry(lEventId);
      if (lPrestorageIndex < DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
      { /* Event has created a new prestored slot since locking the old, so free the locked entry */
        Dem_MemoryEntry_PrestorageEntry_SetState(lPrestorageEntry, DEM_PRESTORAGEMEMORY_STATUS_FREE);
        Dem_Memories_PrestorageMemory_ClearEntry(lPrestorageEntry);
        /* FreeEntry removes the prestored block from the event, repair the link */
        Dem_MemoryEntry_PrestorageEntry_SetEntry(lEventId, lPrestorageIndex);
      }
      else
      { /* Return the unused prestorage entry to the event*/
        Dem_MemoryEntry_PrestorageEntry_SetState(lPrestorageEntry, DEM_PRESTORAGEMEMORY_STATUS_READY);
        Dem_MemoryEntry_PrestorageEntry_SetEntry(lEventId, lPrestorageEntry);
      }
    }
    else
    {
      /* Prestorage entry has been discarded */
      Dem_MemoryEntry_PrestorageEntry_SetState(lPrestorageEntry, DEM_PRESTORAGEMEMORY_STATUS_FREE);
      Dem_Memories_PrestorageMemory_ClearEntry(lPrestorageEntry);
    }

    Dem_PrestorageMemory_LockedIndex = DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY;
    Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */

  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_EventPrestorageClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_Memories_PrestorageMemory_EventPrestorageClear(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Std_ReturnType lReturnValue;
#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
  Dem_Cfg_PrestorageIndexType lPrestorageEntry;
#endif
  
  lReturnValue = E_NOT_OK;

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
  lPrestorageEntry = Dem_MemoryEntry_PrestorageEntry_GetEntry(EventId);

  if (lPrestorageEntry < DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY)
  {
    /* there already is an entry allocated to this event */
/* >>>> -------------------------------- Enter Critical Section: DiagMonitor */
    Dem_EnterCritical_DiagMonitor();

    if (Dem_MemoryEntry_PrestorageEntry_GetState(lPrestorageEntry) != DEM_PRESTORAGEMEMORY_STATUS_LOCKED)
    {
      Dem_MemoryEntry_PrestorageEntry_SetState(lPrestorageEntry, DEM_PRESTORAGEMEMORY_STATUS_FREE);
      Dem_Memories_PrestorageMemory_ClearEntry(lPrestorageEntry);
      lReturnValue = E_OK;
    }

    Dem_LeaveCritical_DiagMonitor();
/* <<<< -------------------------------- Leave Critical Section: DiagMonitor */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_EventPrestoreFreezeFrame
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
Dem_Memories_PrestorageMemory_EventPrestoreFreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)


  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_SatelliteInState(Dem_Cfg_EventSatelliteId(EventId), DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_EventApplicationId(EventId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  /* ----- Global Precondition Checks For EventId ------------------------ */
  if (Dem_Cfg_EventUdsDtc(EventId) == DEM_CFG_DTC_UDS_INVALID)
  {
    lReturnValue = E_NOT_OK;
  }
  else
  if (Dem_Cfg_EventPrestorageIndex(EventId) == DEM_PRESTORAGEMEMORY_NOT_CONFIGURED_FOR_EVENT)
  {
    lReturnValue = E_NOT_OK;
  }
  else
  if (Dem_Event_TestEventAvailable(EventId) == FALSE)
  {
    lReturnValue = E_NOT_OK;
  }
  else
  {
    /* Only pre-store for externally visible Events that support prestorage */
    if (Dem_ExtCom_TestCurrentPartitionIsMaster() == FALSE)
    {
      Dem_SatelliteSat_UpdatePrestoreActionQueue(Dem_Cfg_EventSatelliteId(EventId), Dem_Cfg_EventSatelliteEventId(EventId), DEM_PRESTORAGEMEMORY_QUEUE_PRESTOREFF);
      lReturnValue = E_OK;
    }
    else
    {
      lReturnValue = Dem_Memories_PrestorageMemory_UpdateForEvent(EventId);
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_PRESTOREFREEZEFRAME_APIID, Dem_Det_ErrorRegisterGet());
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_MultiEvent_PrestoreFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_MultiEvent_PrestoreFreezeFrame(
  Dem_EventIdType EventId
  )
{
  if (Dem_Event_IsMasterEvent(EventId))
  {
    Dem_Cfg_ComplexIterType iter;
    for (Dem_Core_Event_MultiEventIterInit(EventId, &iter);                                                                      /* SBSW_DEM_CALL_DEBOUNCEINFO_INIT */
          (Dem_Cfg_ComplexIterExists(&iter) == TRUE);                                                                            /* SBSW_DEM_CALL_DEBOUNCEINFO_POINTER */
          Dem_Cfg_ComplexIterNext(&iter))                                                                                        /* SBSW_DEM_CALL_DEBOUNCEINFO_POINTER */
    {
      (void)Dem_Memories_PrestorageMemory_EventPrestoreFreezeFrame(Dem_Core_Event_MultiEventIterGet(&iter));                     /* PRQA S 2469 */ /* MD_DEM_14.2_LoopControlHandledInternally */  /* SBSW_DEM_CALL_DEBOUNCEINFO_POINTER */
    }
  }

  return;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_EventClearPrestoredFreezeFrame
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Memories_PrestorageMemory_EventClearPrestoredFreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Std_ReturnType lReturnValue;
  Dem_Det_ErrorRegisterDefine()

  lReturnValue = E_NOT_OK;

  Dem_Det_ErrorRegisterSet(DEM_E_NO_ERROR)

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dem_APIChecks_MasterInState(DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_SatelliteInState(Dem_Cfg_EventSatelliteId(EventId), DEM_INITSTATE_INITIALIZED) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_UNINIT)
  }
  else
  if (Dem_APIChecks_EventApplicationId(EventId) == FALSE)
  {
    Dem_Det_ErrorRegisterSet(DEM_E_PARAM_DATA)
  }
  else
  /* ----- Global Precondition Checks -------------------------------------- */
  if (Dem_Cfg_EventUdsDtc(EventId) == DEM_CFG_DTC_UDS_INVALID)
  {
    lReturnValue = E_NOT_OK;
  }
  else
  if (Dem_Cfg_EventPrestorageIndex(EventId) == DEM_PRESTORAGEMEMORY_NOT_CONFIGURED_FOR_EVENT)
  {
    lReturnValue = E_NOT_OK;
  }
  else
  if (Dem_Event_TestEventAvailable(EventId) == FALSE)
  {
    lReturnValue = E_NOT_OK;
  }
  else
  {
    if (Dem_ExtCom_TestCurrentPartitionIsMaster() == FALSE)
    {
      Dem_SatelliteSat_UpdatePrestoreActionQueue(Dem_Cfg_EventSatelliteId(EventId), Dem_Cfg_EventSatelliteEventId(EventId), DEM_PRESTORAGEMEMORY_QUEUE_CLEAR);
      lReturnValue = E_OK;
    }
    else
    {
      lReturnValue = Dem_Memories_PrestorageMemory_EventPrestorageClear(EventId);
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  if (Dem_Det_ErrorRegisterGet() != DEM_E_NO_ERROR)
  {
    Dem_Error_ReportError(DEM_CLEARPRESTOREDFREEZEFRAME_APIID, Dem_Det_ErrorRegisterGet());
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_MultiEvent_ClearPrestoredFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_MultiEvent_ClearPrestoredFreezeFrame(
  Dem_EventIdType EventId
  )
{
  if (Dem_Event_IsMasterEvent(EventId))
  {
    Dem_Cfg_ComplexIterType iter;
    for (Dem_Core_Event_MultiEventIterInit(EventId, &iter);                                                                      /* SBSW_DEM_CALL_DEBOUNCEINFO_INIT */
          (Dem_Cfg_ComplexIterExists(&iter) == TRUE);                                                                            /* SBSW_DEM_CALL_DEBOUNCEINFO_POINTER */
          Dem_Cfg_ComplexIterNext(&iter))                                                                                        /* SBSW_DEM_CALL_DEBOUNCEINFO_POINTER */
    {
      (void)Dem_Memories_PrestorageMemory_EventClearPrestoredFreezeFrame(Dem_Core_Event_MultiEventIterGet(&iter));               /* PRQA S 2469 */ /* MD_DEM_14.2_LoopControlHandledInternally */  /* SBSW_DEM_CALL_DEBOUNCEINFO_POINTER */
    }
  }

  return;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_TestEventFreezeFramePrestored
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
Dem_Memories_PrestorageMemory_TestEventFreezeFramePrestored(
  Dem_EventIdType  EventId
)
{
  boolean lFreezeFramePrestored;

  if ( (Dem_Memories_PrestorageMemory_LockedForEvent(EventId) == TRUE)
    || (Dem_MemoryEntry_PrestorageEntry_EventHasEntryAssigned(EventId) == TRUE) )
  {
    lFreezeFramePrestored = TRUE;
  }
  else
  {
    lFreezeFramePrestored = FALSE;
  }

  return lFreezeFramePrestored;
}

/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_MainFunction
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_MainFunction(
  void
  )
{
  Dem_Satellite_IdType lSatelliteId;
  for (lSatelliteId = 0u; lSatelliteId < Dem_Cfg_GlobalSatelliteCount(); ++lSatelliteId)
  {
    if (Dem_SatelliteCore_TestAndClearPrestoreQueueUpdated(lSatelliteId) == TRUE)
    {
      Dem_Memories_PrestorageMemory_ProcessSatelliteQueueActions(lSatelliteId);
    }
  }
}

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 % Dem_Memories_PrestorageMemory_UpdateEntryOfEvent
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
Dem_Memories_PrestorageMemory_UpdateEntryOfEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry                                                                 /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(PrestorageEntry)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
  /* Collect a global snapshot record into the prestorage buffer */
  Dem_StorageManager_DataStorage_CollectGlobalSnapshot(EventId,                                                                  /* SBSW_DEM_POINTER_PRESTORAGE_GLOBAL_SNAPSHOT_BUFFER */
                                          Dem_MemoryEntry_PrestorageEntry_GetGlobalSRecDataPtr(PrestorageEntry),
                                          Dem_Cfg_GlobalSRecMaxRawSize());
#endif
# if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  /* Sample a UDS snapshot into the prestorage buffer */
  Dem_StorageManager_DataStorage_CollectSnapshot(EventId,                                                                                     
                                    Dem_MemoryEntry_PrestorageEntry_GetSRecDataPtr(PrestorageEntry),                             /* SBSW_DEM_POINTER_PRESTORAGE_SNAPSHOT_BUFFER */
                                    Dem_Cfg_EventSRecMaxRawSize());
# endif

  if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
  {
    /* If the event is OBDII relevant, sample an OBDII freeze frame into the prestorage buffer */
#  if (DEM_CFG_SUPPORT_OBDII == STD_ON)
    if (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
    {
      Dem_StorageManager_DataStorage_CollectObdIIFreezeFrame(EventId,
        Dem_MemoryEntry_PrestorageEntry_GetObdFFDataPtr(PrestorageEntry),                                                        /* SBSW_DEM_POINTER_PRESTORAGE_OBD_FREEZEFRAME_BUFFER */
        Dem_Cfg_GlobalObdFreezeFrameMaxSize());
    }
#  endif
   /* If the event is OBDonUDS relevant, sample a OBDonUDS freeze frame into the prestorage buffer */
#  if (DEM_CFG_SUPPORT_OBDONUDS == STD_ON)
    if (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE)
    {
      Dem_StorageManager_DataStorage_CollectEventSpecificOBDFreezeFrame(EventId,
        Dem_MemoryEntry_PrestorageEntry_GetObdFFDataPtr(PrestorageEntry),                                                        /* SBSW_DEM_POINTER_PRESTORAGE_OBD_FREEZEFRAME_BUFFER */
        Dem_Cfg_GlobalObdFreezeFrameMaxSize());
    }
#  endif
    /* If the event is WWH-OBD relevant, sample a WWH-OBD freeze frame into the prestorage buffer */
#  if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
    if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
    {
      Dem_StorageManager_DataStorage_CollectEventSpecificOBDFreezeFrame(EventId,                                                 /* SBSW_DEM_POINTER_PRESTORAGE_WWHOBDFREEZEFRAME_BUFFER */
        Dem_MemoryEntry_PrestorageEntry_GetWwhObdFFDataPtr(PrestorageEntry),
        Dem_Cfg_GlobalPrestoredWwhobdFreezeFrameDataSize());
    }
#  endif
  }

# if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
  /* If the event has a J1939 DTC and J1939 OBDII is not supported in variant, sample the SPNs into the prestorage buffer */
  if (   (Dem_Core_DTC_HasJ1939DTC(EventId) == TRUE)
      && (Dem_Cfg_IsEmissionObdSupportedInVariant() == FALSE))
  {
    Dem_StorageManager_DataStorage_CollectJ1939FreezeFrame(EventId,                                                              /* SBSW_DEM_POINTER_PRESTORAGE_J1939FREEZEFRAME_BUFFER */
                                              Dem_MemoryEntry_PrestorageEntry_GetJ1939FFDataPtr(PrestorageEntry),
                                              Dem_Cfg_GlobalJ1939FFTotalRawSize());
  }
# endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_PRESTORE_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_PrestorageMemory_Implementation.h
 *********************************************************************************************************************/
