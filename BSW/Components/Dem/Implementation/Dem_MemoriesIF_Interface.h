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
 *  \defgroup   Dem_MemoriesIF Dem MemoriesIF
 *  \{
 *  \file       Dem_MemoriesIF_Interface.h
 *  \brief      Abstraction for multiple event memories.
 *  \details    Public Interfaces of MemoriesIF subcomponent which is facade for all memory related tasks which can be
 *              abstracted.
 *  \entity     MemoriesIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMORIESIF_INTERFACE_H)
#define DEM_MEMORIESIF_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_MemoriesIF_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Memory_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_FaultMemory_Memories_SetCurrentTimestamp
 *****************************************************************************/
/*!
 * \brief         Set current value for time stamp.
 *
 * \details       Set current value for time stamp.
 *
 * \param[in]     CurrentTimeStamp
 *                Current value for time stamp.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SetCurrentTimestamp(                                                                                                
  CONST(uint32, AUTOMATIC)  CurrentTimeStamp
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_GetCurrentTimestamp
 *****************************************************************************/
/*!
 * \brief         Get current value for time stamp.
 *
 * \details       Get current value for time stamp.
 *
 * \return        Current value for time stamp.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_FaultMemory_Memories_GetCurrentTimestamp(                                                                                                
  void
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_GetChronology
 *****************************************************************************/
/*!
 * \brief         Get the entry index from the chronology of its memory.
 *
 * \details       Get the entry index from the chronology of its memory.
 *                Entry indices are stored backwards in the chronology.
 *                Chronology index 0 represents the oldest entry index.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the memory.
 * \param[in]     ChronoIndex
 *                Chronology index at which to return the entry index.
 *
 * \return        Entry index stored in the chronology at given index.
 *
 * \pre           Translated memory id refers to event or time series memory.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_GetChronology(
  uint16 TranslatedMemoryId,
  uint8  ChronoIndex
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_SetChronology
 *****************************************************************************/
/*!
 * \brief         Set the Nth oldest entry index
 *
 * \details       Set the Nth oldest entry index. 0 is the oldest index.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier for the event or time series memory.
 * \param[in]     ChronoIndex
 *                Index into chronology.
 * \param[in]     EntryIndex
 *                Index of the entry
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SetChronology(
  uint16 TranslatedMemoryId,
  uint8 ChronoIndex,
  Dem_Cfg_EntryIndexType EntryIndex
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_GetFirstEntryIndex
 *****************************************************************************/
/*!
 * \brief         Get the index of the first entry in the given memory.
 *
 * \details       Get the index of the first entry in the given memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the event memory.
 *
 * \return        Entry index of the first entry.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_GetFirstEntryIndex(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_GetEndEntryIndex
 *****************************************************************************/
/*!
 * \brief         Get the index of the last entry in the given memory.
 *
 * \details       Get the index of the last entry in the given memory. The index
 *                returned does not belong to the memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the event memory.
 *
 * \return        Entry index of the last entry.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_GetEndEntryIndex(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_GetMaxSize
 *****************************************************************************/
/*!
 * \brief         Get the size of the event memory.
 *
 * \details       Get the size of the event memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the event memory.
 *
 * \return        The maximum size of the event memory.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FaultMemory_Memories_GetMaxSize(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_GetCurrentSize
 *****************************************************************************/
/*!
 * \brief         Get the number of occupied entries in the event memory.
 *
 * \details       Get the number of occupied entries in the event memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique Identifier for the memory.
 *
 * \return        The number of occupied entries in the event memory.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FaultMemory_Memories_GetCurrentSize(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_SetCurrentSize
 *****************************************************************************/
/*!
 * \brief         Set the number of occupied entries in the event memory.
 *
 * \details       Set the number of occupied entries in the event memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique Identifier for the memory.
 * \param[in]     Size
 *                New number of occupied entries in the event memory.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SetCurrentSize(
  uint16 TranslatedMemoryId,
  uint8 Size
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_GetFirstEvent
 *****************************************************************************/
/*!
 * \brief         Get the first event mapped to the given memory.
 *
 * \details       Get the first event mapped to the given memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the memory.
 *
 * \return        Unique handle of the event.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_FaultMemory_Memories_GetFirstEvent(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_GetLastEvent
 *****************************************************************************/
/*!
 * \brief         Get the last event mapped to the given memory.
 *
 * \details       Get the last event mapped to the given memory.
 *
 * \param[in]     MemoryId
 *                Unique identifier of the memory.
 *
 * \return        Unique handle of the event.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_FaultMemory_Memories_GetLastEvent(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_SetInitAdminDataRequested
 *****************************************************************************/
/*!
 * \brief         Sets if admin data was initialized or not
 *
 * \details       Sets if admin data was initialized or not
 *
 * \param[in]     InitRequested
 *                TRUE: Admin data was initialized by NvM
 *                FALSE: Otherwise
 *
 * \pre           NvM must have restored NV mirrors.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SetInitAdminDataRequested(
  CONST(boolean, AUTOMATIC)  InitRequested
  );


/* ****************************************************************************
* Dem_FaultMemory_Memories_GetSRecNumerationType
*****************************************************************************/
/*!
* \brief         Get the Snapshot Record type used for the event memory.
*
* \details       Get the Snapshot Record type used for the event memory.
*
* \param[in]     TranslatedMemoryId
*                Unique Identifier for the memory.
*
* \return        Snapshot Record type of the event memory.
*
* \pre           -
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecNumerationType, DEM_CODE)
Dem_FaultMemory_Memories_GetSRecNumerationType(
  uint16 TranslatedMemoryId
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
 * \defgroup Dem_Memory_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_FaultMemory_Memories_AllocateEventEntryAging
 *****************************************************************************/
/*!
 * \brief         Selects an event entry for an event's aging counter
 *
 * \details       If a free memory slot is available, it is returned directly.
 *                There is no fallback in case of a filled up event memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique Identifier of the Event memory
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The entry index of the allocated event entry if one
 *                could be identified. DEM_CFG_ENTRYINDEX_INVALID if no entry
 *                was allocated.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_AllocateEventEntryAging(
  uint16 TranslatedMemoryId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_UpdateChrono
 *****************************************************************************/
/*!
 * \brief         Moves the passed entry up to most recent in the chronology list.
 *
 * \details       -
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of Event memory.
 * \param[in]     EntryIndex
 *                Index of the entry to free
 *
 * \pre           The EntryIndex must belong to the TranslatedMemoryId
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_UpdateChrono(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType EntryIndex
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_RemoveEntryAndChrono
 *****************************************************************************/
/*!
 * \brief         Frees the given entry.
 *
 * \details       Frees the given entry in the event or time series memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of Event memory.
 * \param[in]     EntryIndex
 *                Index of the entry to free
 *
 * \pre           The EntryIndex must belong to the TranslatedMemoryId
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_RemoveEntryAndChrono(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType EntryIndex
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_TranslateDTCOrigin
 *****************************************************************************/
/*!
 * \brief         Translates the DTC Origin to the DEM Internal Memory handle.
 *
 * \details       Translates the DTC Origin to the DEM Internal Memory handle.
 *
 * \param[in]     DTCOrigin
 *                Identifier of the event memory concerned.
 * \param[out]    TranslatedMemoryId
 *                Translated Memory Identifier of the event memory.
 *
 * \return        TRUE
 *                Translation successful.
 * \return        FALSE
 *                Translation unsuccessful.
 *
 * \pre           -
 * 
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_FaultMemory_Memories_TranslateDTCOrigin(
  Dem_DTCOriginType  DTCOrigin,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC)  TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_TestEntryforEventGeneric
 *****************************************************************************/
/*!
 * \brief         Tests whether the entry belonging to the Memory stores the
 *                input event.
 *
 * \details       Tests whether the entry belonging to the Memory stores the
 *                input event.
 *
 * \param[in]     TranslatedMemoryId
 *                Translated Memory Identifier of the event memory.
 * \param[in]     EntryIndex
 *                Handle of the entry
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Event is stored.
 * \return        FALSE
 *                Event is not stored.
 *
 * \pre           Function must only be invoked to test entries in Primary,
 *                UserDefined and Time Series Memory banks.
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_Memories_TestEntryforEventGeneric(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType EntryIndex,
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_FindEntryIndexOfEventGeneric
 *****************************************************************************/
/*!
 * \brief         Finds the entry index for the given event.
 *
 * \details       Finds the entry index for the given event in the specified
 *                memory, if it exists.
 *                A DET error might be trown if no entry is found for the event,
 *                based on the passed parameters.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     VerifyEventStored
 *                If set to true, a runtime error will be thrown in case no entry
 *                was found.
 * \param[in]     ExpectedTranslatedMemoryId
 *                Memory Id in which the event is expected to be stored
 *
 * \return        Index of the entry for the event or DEM_CFG_ENTRYINDEX_INVALID
 *                in case no entry was found.
 * 
 * \pre           This function may only be invoked to search for event entries
 *                in primary, user defined or time series memory.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_FindEntryIndexOfEventGeneric(
  Dem_EventIdType EventId,
  boolean VerifyEventStored,
  uint16 ExpectedTranslatedMemoryId
);

/* ****************************************************************************
 * Dem_FaultMemory_Memories_FreeEntry
 *****************************************************************************/
/*!
 * \brief         Frees an entry in the specified memory.
 *
 * \details       Frees an entry in the specified memory. This can be any event
 *                or time series memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Translated Memory Identifier of the event memory.
 * \param[in]     EntryIndex
 *                Index of the entry to free.
 *
 * \pre           Function must only be invoked to clear entries in Primary,
 *                UserDefined and Time Series Memory banks. The entry index must
 *                be valid.
 * 
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_FreeEntry(
 uint16 TranslatedMemoryId,
 Dem_Cfg_EntryIndexType EntryIndex
 );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_AllocateEventEntryIndexAging
 *****************************************************************************/
/*!
 * \brief         Selects an event entry for an event's aging counter
 *
 * \details       If a free memory slot is available, it is returned directly.
 *                There is no fallback in case of a filled up event memory.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The event entry index of the allocated event entry if one
 *                could be identified. DEM_CFG_ENTRYINDEX_INVALID if no entry
 *                was allocated.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_AllocateEventEntryIndexAging(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_GetOverflowOfDTCOrigin
 *****************************************************************************/
/*!
 * \brief         Get the 'overflowed' state for an event memory.
 *
 * \details       Get the 'overflowed' state for an event memory.
 *
 * \param[in]     DTCOrigin
 *                Unique identifier of the memory.
 * \param[out]    OverflowIndication
 *                Retrieved overflow indication.
 *
 * \return        E_OK
 *                Overflow indication is written to output parameter.
 * \return        E_NOT_OK
 *                Overflow indication is not written to output parameter.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FaultMemory_Memories_GetOverflowOfDTCOrigin(
  CONST(Dem_DTCOriginType, AUTOMATIC)  DTCOrigin,
  P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  OverflowIndication
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_GetNumAllocatedMemoryEntry
 *****************************************************************************/
/*!
 * \brief         Facade, fetch the number of allocated entries of the respective memory.
 *
 * \details       Facade, fetch the number of allocated entries of the respective memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the memory.
 *
 * \return        Number of allocated entries.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FaultMemory_Memories_GetNumAllocatedMemoryEntry(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16  TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_ResetOverflow
 *****************************************************************************/
/*!
 * \brief         Clear the 'overflowed' state for the given memory.
 *
 * \details       Clear the 'overflowed' state for the given memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the memory.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_ResetOverflow(
  uint16 TranslatedMemoryId
  );

#if (DEM_FEATURE_NEED_CYCLE_COUNTERS == STD_ON)
/* ****************************************************************************
 * Dem_FaultMemory_Memories_LatchCycleCounters
 *****************************************************************************/
/*!
 * \brief         Test if cycle counters need to be latched. Latch the counters 
 *                if it is required.
 *
 * \details       Test if cycle counters need to be latched. Latch the counters 
 *                if it is required.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     CycleCounter
 *                Current cycle count
 *
 * \pre           Cycle counters have already been incremented for the cycle
   \config        DEM_FEATURE_NEED_CYCLE_COUNTERS == STD_ON.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_LatchCycleCounters(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  uint16 CycleCounter
);
#endif

/* ****************************************************************************
 * Dem_FaultMemory_Memories_Init
 *****************************************************************************/
/*!
 * \brief         Initializes backing memory
 *
 * \details       This function controls initialization of the memory
 *                subcomponent. After the function completes, event memories
 *                are available.
 *
 * \pre           May only be called during initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_Init(
  void
  );

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_FaultMemory_Memories_InitConfirmedChronology
 *****************************************************************************/
/*!
 * \brief         Initialize confirmed chronology
 *
 * \details       Initialize confirmed chronology
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_InitConfirmedChronology(
  void
  );
#endif

/* ****************************************************************************
 * Dem_FaultMemory_Memories_InitOverflow
 *****************************************************************************/
/*!
 * \brief         Set the 'overflowed' state for an event memory.
 *
 * \details       Set the 'overflowed' state for an event memory.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_InitOverflow(
  void
  );

/* ****************************************************************************
 * Dem_FaultMemory_Memories_SetOverflow
 *****************************************************************************/
/*!
 * \brief         Set the 'overflowed' state for an event memory.
 *
 * \details       Set the 'overflowed' state for an event memory.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the event memory.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SetOverflow(
  uint16  TranslatedMemoryId
  );

/* ****************************************************************************
* Dem_FaultMemory_Memories_GetOverflow
*****************************************************************************/
/*!
* \brief         Get the memory overflow state.
*
* \details       Get the memory overflow state.
*
* \param[in]     TranslatedMemoryHandle
*                DEM Internal memory handle.
*
* \return        The memory overflow state.
*
* \pre           -
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_Memories_GetOverflow(
  uint16 TranslatedMemoryHandle
  );


/* ****************************************************************************
 * Dem_FaultMemory_Memories_SortEventEntryIntoChronologicalList
 *****************************************************************************/
/*!
 * \brief         Sort the event entry into the chronological list.
 *
 * \details       Sort the event entry into the chronological list.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique Identifier for the event memory.
 * \param[in]     EventEntryIndex
 *                Handle of the event entry to sort into the chronology.
 * \param[in]     Timestamp
 *                Current timestamp of the event entry.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SortEventEntryIntoChronologicalList(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  uint32 Timestamp
  );


/* ****************************************************************************
 * Dem_FaultMemory_Memories_AllocateEventEntry
 *****************************************************************************/
/*!
 * \brief         Selects an event entry for an event
 *
 * \details       This function manages the event memory storage. If a free
 *                slot is available, it is returned directly. In case no free
 *                slot is available, the configured displacement algorithm is
 *                applied to free a slot.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique Identifier of Event memory.
 *                References primary or user defined.
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The event entry index of the allocated event entry if one
 *                could be identified. DEM_CFG_ENTRYINDEX_INVALID if no entry
 *                was allocated.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_AllocateEventEntry(
  uint16 TranslatedMemoryId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMORIESIF_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemoriesIF_Interface.h
 *********************************************************************************************************************/
