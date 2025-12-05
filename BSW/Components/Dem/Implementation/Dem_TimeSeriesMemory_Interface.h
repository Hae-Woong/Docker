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
 *  \defgroup   Dem_TimeSeriesMemory MemPermanent
 *  \{
 *  \file       Dem_TimeSeriesMemory_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Interface of the Time Series Memory Unit which manages storage of time series snapshots.
 *  \entity     TimeSeriesMemory
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_TIMESERIESMEMORY_INTERFACE_H)
#define DEM_TIMESERIESMEMORY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_TimeSeriesMemory_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_TimeSeriesMemory_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_SetAllBits
 *****************************************************************************/
/*!
 * \brief         Set all bits in a bit field
 *
 * \details       Set all bits in a bit field
 *
 * \param[in]     BitField
 *                Pointer to a bit field
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_SetAllBits(
  P2VAR(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) BitField
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_SetBitsForTriggeredSeries
 *****************************************************************************/
/*!
 * \brief         Set the bits in a bit field if the time series with series
 *                id equal to the bit position has its storage trigger
 *                condition satisfied, return indicating if at least one has
 *                been triggered.
 *
 * \details       Set the bits in a bit field if the time series with series
 *                id equal to the bit position has its storage trigger
 *                condition satisfied, return indicating if at least one has
 *                been triggered.
 *
 * \param[in]     BitField
 *                Pointer to a bit field
 *
 * \param[in]     DTCContext
 *                DTC processing context
 *
 * \return        TRUE if storage of at least one series has been triggered FALSE
 *                otherwise
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_SetBitsForTriggeredSeries(
  P2VAR(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) TriggerBitField,
  P2CONST(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetGlobalEntryCount
 *****************************************************************************/
/*!
 * \brief         Get the total number of all time series entries.
 *
 * \details       -
 *
 * \return        The total number of all time series entries.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetGlobalEntryCount(
  void
  );

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetSizeOfTimeSeriesEntryTable
 *****************************************************************************/
/*!
 * \brief         Returns the total size of the time series entry table.
 *
 * \details       Returns the total size of the time series entry table.
 *
 * \return        Total size the time series entry table
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_TimeSeriesEntryTableIterType, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetSizeOfTimeSeriesEntryTable(
  void
  );
#endif


/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_IsPowertrainEnabled
 *****************************************************************************/
/*!
 * \brief         Returns true if feature time series powertrain is enabled.
 *
 * \details       -
 *
 * \return        TRUE
 *                Time series powertrain is enabled
 * \return        FALSE
 *                Time series powertrain is not enabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_IsPowertrainEnabled(
  void
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetPrimaryTranslatedMemoryId
 *****************************************************************************/
/*!
 * \brief         Get the translated memory id of the time series 
 *                memory for primary memory.
 *
 * \details       Get the translated memory id of the time series
 *                memory for primary memory.
 *
 * \return        The translated memory id of the time series
 *                memory for primary memory. 
 *                DEM_CFG_MEMORYID_INVALID if primary memory has no
 *                time series memory.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetPrimaryTranslatedMemoryId(
  void
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_IsReferredByMemoryId
 *****************************************************************************/
/*!
 * \brief         Test if the given translated memory Id belongs to a time
 *                series memory
 *
 * \details       Test if the given translated memory Id belongs to a time
 *                series memory
 *
 * \param[in]     TranslatedMemoryId
 *                Translated Memory Identifier of the memory.
 *
 * \return        TRUE
 *                The translated memory id belongs to a time series memory
 * \return        FALSE
 *                The translated memory id does not belongs to a time series
 *                memory
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_IsReferredByMemoryId(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TssSetIter_Init
 *****************************************************************************/
/*!
 * \brief         Initializes the iterator for Tss Fifo
 *
 * \details       Initializes the iterator for Tss Fifo
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \param[in]     SeriesId
 *                Id specifying a time series of the event stored in the specified entry
 *
 * \param[in]     IterPtr
 *                Iterator for Tss Fifo
 *
 *
 * \pre           Function must be invoked only when time series snapshot records
 *                are enabled.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TssSetIter_Init(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  CONSTP2VAR(Dem_Cfg_TssFifoEntry_IterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TssSetIter_Exists
 *****************************************************************************/
/*!
 * \brief         Test if the Tss Fifo entry iterator points to a valid element
 *
 * \details       Test if the Tss Fifo entry iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Iterator for Tss Fifo
 *
 * \return        TRUE if iterator points to valid element
 * \return        FALSE if iterator does not point to valid element
 *
 * \pre           Function must be invoked only when time series snapshot records
 *                are enabled.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TssSetIter_Exists(
  CONSTP2CONST(Dem_Cfg_TssFifoEntry_IterType, AUTOMATIC, AUTOMATIC) IterPtr
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TssSetIter_Next
 *****************************************************************************/
/*!
 * \brief         Move the Tss Fifo entry iterator to the next element
 *
 * \details       Move the Tss Fifo entry iterator to the next element
 *
 * \param[in]     IterPtr
 *                Iterator for Tss Fifo
 *
 * \pre           Function must be invoked only when time series snapshot records
 *                are enabled.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TssSetIter_Next(
  CONSTP2VAR(Dem_Cfg_TssFifoEntry_IterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_EventDidIter_Exists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_Memories_TimeSeriesMemory_EventDidIter_Get() will return
 *                a valid element on this iterator
 * \return        FALSE
 *                Dem_Memories_TimeSeriesMemory_EventDidIter_Get() is undefined
 *                on this iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_EventDidIter_Exists(
  CONSTP2CONST(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_EventDidIter_Get
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to DID descriptor table
 *
 * \pre           IterPtr has been initialized
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_Memories_TimeSeriesMemory_EventDidIter_Get(
  CONSTP2CONST(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_EventDidIter_Init
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate DIDs configured for an
 *                event (i.e. Snapshot content)
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_EventDidIter_Init(
  Dem_EventIdType EventId,
  CONSTP2VAR(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_EventDidIter_Next
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_EventDidIter_Next(
  CONSTP2VAR(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_IsSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether time series feature is supported.
 *
 * \details       Tests whether time series feature is supported by
 *                configuration.
 *
 * \return        TRUE
 *                Time series feature is supported.
 *                FALSE
 *                Time series feature is not supported.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_IsSupported(
  void
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TestSamplingProfileExists
 *****************************************************************************/
/*!
 * \brief         Tests whether any time series sampling profile is configured.
 *
 * \details       -
 *
 * \return        TRUE
 *                Any time series sampling profile is configured.
 *                FALSE
 *                No time series sampling profile is configured.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestSamplingProfileExists(
  void
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries
 *****************************************************************************/
/*!
 * \brief         Test if the event stores a time series snapshot record
 *
 * \details       Test if the event stores a time series snapshot record
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event stores a time series snapshot record
 * \return        FALSE
 *                The events does not store a time series snapshot record
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries(
  Dem_EventIdType  EventId
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TestRecordNumberIsInSeries()
 *****************************************************************************/
/*!
 * \brief         Test if the provided record number belongs to the specified event and time series
 *
 * \details       Test if the provided record number belongs to the specified event and time series
 *
 * \param[in]     EventId
 *                Id specifying an event
 *
 * \param[in]     SeriesId
 *                Id specifying a time series for the event
 *
 * \param[in]     RecordNumber
 *                record number to be tested
 *
 * \return        TRUE if record number is in series FALSE otherwise
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestRecordNumberIsInSeries(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType  EventId,
  Dem_TimeSeriesIdType SeriesId,
  uint8 RecordNumber
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetLastRecordNumberOfSeries()
 *****************************************************************************/
/*!
 * \brief         Get the last record number of the specified time series
 *
 * \details       Get the last record number of the specified time series
 *
 * \param[in]     EventId
 *                Id specifying an event
 *
 * \param[in]     SeriesId
 *                Id specifying a time series for the event
 *
 * \return        The last record number of the series
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetLastRecordNumberOfSeries(
  Dem_EventIdType  EventId,
  Dem_TimeSeriesIdType SeriesId
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetSnapshotOffsetFromFirstOneInSet()
 *****************************************************************************/
/*!
 * \brief         Get the position of a snapshot in the specified time series relative
 *                to the first one in the time serie set it belongs to
 *
 * \details       Get the position of a snapshot in the specified time series relative
 *                to the first one in the time serie set it belongs to
 *
 * \param[in]     EventId
 *                Id specifying an event
 *
 * \param[in]     SeriesId
 *                Id specifying a time series for the event
 *
 * \param[in]     RecordNumber
 *                record number to be tested
 *
 * \return        Offset of the specified samplen from the first one in the same set
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetSnapshotOffsetFromFirstOneInSet(                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType  EventId,
  Dem_TimeSeriesIdType SeriesId,
  uint8 RecordNumber
);
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetNumberOfSamplesOfSeries()
 *****************************************************************************/
/*!
 * \brief         Get the total number of samples used for the time series.
 *
 * \details       Get the total number of samples used for the time series.
 *
 * \param[in]     EventId
 *                Id specifying an event
 *
 * \param[in]     SeriesId
 *                Id specifying a time series for the event
 *
 * \return        Number of samples in the series
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetNumberOfSamplesOfSeries(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType  EventId,
  Dem_TimeSeriesIdType SeriesId
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetNumberOfStoredTimeSeriesSets()
 *****************************************************************************/
/*!
 * \brief         Get the number of time series sets that have been stored for in 
 *                the specified time series entry (per series)
 *
 * \details       Get the number of time series sets that have been stored for in 
 *                the specified time series entry (per series)
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Id specifying a time series entry
 *
 * \param[in]     SeriesId
 *                Id specifying a time series of the event stored in the specified entry
 *
 * \return        Number of stored time series sets
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetNumberOfStoredTimeSeriesSets(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId
);


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetSetIdOfRecordNumber()
 *****************************************************************************/
/*!
 * \brief         Calculate the index specifying the position of a time series set
 *                in memory that contains the specified record number
 *
 * \details       Calculate the index specifying the position of a time series set
 *                in memory that contains the specified record number
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Id specifying a time series entry
 *
 * \param[in]     SeriesId
 *                Id specifying a time series of the event stored in the specified entry
 *
 * \param[in]     RecordNumber
 *                Record number
 *
 * \return        Set id of record numbers
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetSetIdOfRecordNumber(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  uint8  RecordNumber
);
#endif


/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TestRecordNumberIsValid
 *****************************************************************************/
/*!
 * \brief         Tests whether a time series snapshot record number is valid
 *
 * \details       Tests whether a time series snapshot record number is valid
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     RecordNumber
 *                Snapshot record number
 *
 * \return        TRUE
 *                The record number is valid
 * \return        FALSE
 *                The record number is invalid
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestRecordNumberIsValid(
  Dem_EventIdType  EventId,
  uint8 RecordNumber
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets
 *****************************************************************************/
/*!
 * \brief         Calculates the maximum number of Tss sets per
 *                Time series entry for the event
 *
 * \details       Calculates the maximum number of Tss sets per 
 *                Time series entry for the event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()].
 *
 * \return        Number of TSS sets supported 
 *                If the event supports time series and the event memory
 *                supports Tssr Fifo
 * \return        1u
 *                If the event does not support Time series Fifo
 *
 * \pre           Function must be invoked only when event supports time series
 *                snapshot records.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(
  Dem_EventIdType EventId
);



/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_TimeSeriesMemory_Public Public Methods
 * \{
 */



/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_InvalidateEntryOfEvent
 *****************************************************************************/
/*!
 * \brief         Remove a time series snapshot.
 *
 * \details       This function clears a time series snapshot stored for the
 *                passed Event and marks the block for nv-storage.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     BlockStatus
 *                New status of the memory block.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_InvalidateEntryOfEvent(
  Dem_EventIdType EventId,
  uint8  BlockStatus
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_Init
 *****************************************************************************/
/*!
 * \brief         Initializes time series functionality
 *
 * \details       Initializes time series sampler, ringbuffer and 
 *                time series entry companion
 *
 * \pre           NvM must have restored NV mirrors
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_Init(
  void
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_IsTimeSeriesPowertrainStoredByEvent
 *****************************************************************************/
/*!
 * \brief         Test whether TimeSeries Powertrain Snapshot is ready to be reported.
 *
 * \details       Test whether TimeSeries Powertrain Snapshot is ready to be reported.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \return        TRUE
 *                TimeSeries Powertrain Snapshot ready to be reported 
 * \return        FALSE
 *                No TimeSeries Powertrain Snapshot ready or was already reported
 * \pre           -
 *
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_IsTimeSeriesPowertrainStoredByEvent(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_InitNvData
 *****************************************************************************/
/*!
 * \brief         Initializes time series entries
 *
 * \details       This function initializes time series entries and
 *                time series memory control structures.
 *
 * \pre           NvM must have restored NV mirrors
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_InitNvData(
  void
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_RestoreOnInit
 *****************************************************************************/
/*!
 * \brief         Validate and recover time series entries
 *
 * \details       This function verifies the integrity of time series entries
 *                and links them up into the runtime control structures.
 *
 * \pre           NvM must have restored NV mirrors
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_RestoreOnInit(
  void
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_StoreEntry
 *****************************************************************************/
/*!
 * \brief         Store a time series entry
 *
 * \details       This function tries to update an existing entry for the given event
 *                if updates are supported. 
 *                In case no entry exists, a new entry will be 
 *                allocated if possible. This can displace existing time series 
 *                entries if all time series entries are occupied.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \param[in]     SeriesTriggeredFlags
 *                Pointer to a bit field indicating for which time series the storage
 *                trigger has occured
 * 
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_StoreEntry(
  Dem_EventIdType  EventId,
  P2CONST(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) SeriesTriggeredFlags
);

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_ReleaseEntry
 *****************************************************************************/
/*!
 * \brief         Release lock of the time series entry
 *
 * \details       Release lock of the time series entry
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 * \pre           -
 *
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *                && DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_ReleaseEntry(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
);
#endif


/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_MainFunction
 *****************************************************************************/
/*!
 * \brief         Processes tasks related to Time Series snapshot records
 *
 * \details       This function stores the future samples and processes the
 *                asynchronous tasks related to clearing of time series snapshot
 *                records
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_MainFunction(
  void
);

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF))
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TestSRecIsStored
 *****************************************************************************/
/*!
 * \brief         Tests whether a time series snapshot record is currently stored
 *
 * \details       Tests whether a time series snapshot record is currently stored
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \param[in] Iter
 *                pointer to an iterator
 *
 * \return        TRUE
 *                The record is stored
 * \return        FALSE
 *                The record is not stored
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON && 
 *                DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestSRecIsStored(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  CONSTP2CONST(Dem_TimeSeriesRecordIterator, AUTOMATIC, AUTOMATIC) Iter
);
#endif

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_FindEntryIndex
 *****************************************************************************/
/*!
 * \brief         Finds the entry index of a time series entry for event if allocated.
 *
 * \details       Finds the entry index of a time series entry. If the event
 *                does not have a valid time series entry, an invalid index is
 *                returned instead.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The entry index used by the event.
 *                In error cases, an invalid index is returned.
 *
 * \pre           -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Memories_TimeSeriesMemory_FindEntryIndex(
  Dem_EventIdType  EventId
);

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON)
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetTimeSeriesPowertrainSequentialInfo
 *****************************************************************************/
/*!
 * \brief         Copies sequential DID into a buffer.
 *
 * \details       Copies a sequential DID into the provided buffer. In case
 *                required number of samples are not present, padding is also
 *                done.
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 * \param[in]     EventId
 *                Unique handle of the Event
 * \param[out]    TimeSeriesInfo
 *                Pointer to struct containing meta information about the TimeSeries SRec
 *
 *
 * \pre           A time series powertrain snapshot entry must have been
 *                locked (see Dem_DcmDisableDTCRecordUpdate)
 * \pre           Existence of the record must be verified before calling this
 *                function, otherwise random data will be copied.
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON &&
 *                DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetTimeSeriesPowertrainSequentialInfo(
  Dem_Cfg_EntryIndexType  TimeSeriesEntryIndex,
  Dem_EventIdType EventId,
  CONSTP2VAR(Dem_Memory_TimeSeries_PowertrainTimeSeriesInfoType, AUTOMATIC, AUTOMATIC) TimeSeriesInfo
);

#endif


/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetNextOrLastSeriesByRecordNumber
 *****************************************************************************/
/*!
 * \brief         If any record number larger than the provided one is contained in a
 *                time series, return the SeriesId of the time series containing the lowest
 *                such record number, otherwise return the id of the time series with the
 *                highest record number.
 *
 * \details       If any record number larger than the provided one is contained in a
 *                time series, return the SeriesId of the time series containing the lowest
 *                such record number, otherwise return the id of the time series with the
 *                highest record number.
 *
 * \param[in]     EventId
 *                Id specifying the event
 * \param[in]     RecordNumber
 *                Specified record number
 *
 * \return        Time series id
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_TimeSeriesIdType, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetNextOrLastSeriesByRecordNumber(
  Dem_EventIdType EventId,
  uint8 RecordNumber
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_InitializeTimeSeriesRecordIteratorBySeriesId
 *****************************************************************************/
/*!
 * \brief         Initialize the iterator to start from the time series specified by its arguments
 *
 * \details       Initialize the iterator to start from the time series specified by its arguments.
 *
 * \param[in]     EventId
 *                Id specifying the event
 * \param[in]     SeriesId
 *                Id specifying a time series used by the event
 *
 * \return        The newly initialized iterator struct
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_TimeSeriesRecordIterator, DEM_CODE)
Dem_Memories_TimeSeriesMemory_InitializeTimeSeriesRecordIteratorBySeriesId(
  Dem_EventIdType EventId,
  Dem_TimeSeriesIdType SeriesId
);


/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_InitializeTimeSeriesRecordIteratorByRecordNumber
 *****************************************************************************/
/*!
 * \brief         Initialize the iterator to start from the time series snapshot record of the 
 *                specified event that has the specified record number.
 *
 * \details       Initialize the iterator to start from the time series snapshot record of the 
 *                specified event that has the specified record number.
 *
 * \param[in]     EventId
 *                Id specifying the event
 * \param[in]     RecordNumber
 *                Specified record number
 *
 * \return        The newly initialized iterator struct
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_TimeSeriesRecordIterator, DEM_CODE)
Dem_Memories_TimeSeriesMemory_InitializeTimeSeriesRecordIteratorByRecordNumber(
  Dem_EventIdType EventId,
  uint8 RecordNumber
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextSeriesByRecordNumber
 *****************************************************************************/
/*!
 * \brief         Advances the iterator to the lowest record number of any time series
 *                with record numbers greater than the current one
 *
 * \details       Advances the iterator to the lowest record number of any time series
 *                with record numbers greater than the current one
 *
 * \param[in,out] Iter
 *                pointer to an iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextSeriesByRecordNumber(
  CONSTP2VAR(Dem_TimeSeriesRecordIterator, AUTOMATIC, AUTOMATIC) Iter
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextSet()
 *****************************************************************************/
/*!
 * \brief         Advances the iterator to the next set of the current series if one exists
 *
 * \details       Advances the iterator to the next set of the current series if one exists
 *
 * \param[in,out] Iter
 *                pointer to an iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextSet(
  CONSTP2VAR(Dem_TimeSeriesRecordIterator, AUTOMATIC, AUTOMATIC) Iter
  );


/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextRecordNumberInAnySeriesOfEvent()
 *****************************************************************************/
/*!
 * \brief         Advances the iterator to the next record number belonging to any time series
 *                of the event if one exists
 *
 * \details       Advances the iterator to the next record number belonging to any time series
 *                of the event if one exists
 *
 * \param[in,out] Iter
 *                pointer to an iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
 DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextRecordNumberInAnySeriesOfEvent(
  CONSTP2VAR(Dem_TimeSeriesRecordIterator, AUTOMATIC, AUTOMATIC) Iter
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorIsInvalid()
 *****************************************************************************/
/*!
 * \brief         Check if the iterator is in an invalid state, indicating the end of the iteration
 *
 * \details       Check if the iterator is in an invalid state, indicating the end of the iteration
 *
 * \param[in,out] Iter
 *                pointer to an iterator
 *
 * \return        TRUE if the iterator is invalid FALSE otherwise
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorIsInvalid(
  CONSTP2CONST(Dem_TimeSeriesRecordIterator, AUTOMATIC, AUTOMATIC) Iter
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetSamplingProfileId()
 *****************************************************************************/
/*!
 * \brief         Get the id into the sampling profile table of the specified time series of an event
 *
 * \details       Get the id into the sampling profile table of the specified time series of an event
 *
 * \param[in]     EventId
 *                Id specifying the event
 * \param[in]     SeriesId
 *                Id specifying a time series used by the event
 *
 * \return        Sampling profile id
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetSamplingProfileId(
  Dem_EventIdType EventId,
  Dem_TimeSeriesIdType SeriesId
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId()
 *****************************************************************************/
/*!
 * \brief         Get the id into the time series freezeframe table of the specified time series of an event
 *
 * \details       Get the id into the time series freezeframe table of the specified time series of an event
 *
 * \param[in]     EventId
 *                Id specifying the event
 * \param[in]     SeriesId
 *                Id specifying a time series used by the event
 *
 * \return        Id of the series into the time series freeze frame table
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(
  Dem_EventIdType EventId,
  Dem_TimeSeriesIdType SeriesId
);

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent()
 *****************************************************************************/
/*!
 * \brief         Get the id into the time series freezeframe table of the specified time series of an event
 *
 * \details       Get the id into the time series freezeframe table of the specified time series of an event
 *
 * \param[in]     EventId
 *                Id specifying the event
 *
 * \return        Number of time series for the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(
  Dem_EventIdType EventId
);
#endif

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_CopyDidsFromRingBuffer
 *****************************************************************************/
/*!
 * \brief         Copy latest time series samples from the ringbuffer to a
 *                desitination buffer
 *
 * \details       Copy time series samples from the ringbuffer. This function
 *                is used by both Time series memory and Custom Trigger memory
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 * \param[in]     SourceIndex
 *                RingBuffer Index to copy. 0: Oldest Sample
 * \param[out]    DestinationBuffer
 *                Pointer to the buffer to store the data
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_CopyDidsFromRingBuffer(
  Dem_EventIdType EventId,
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  uint8 SourceIndex,
  Dem_Data_DestinationBufferPtrType  DestinationBuffer
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_TIMESERIESMEMORY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_TimeSeriesMemory_Interface.h
 *********************************************************************************************************************/
