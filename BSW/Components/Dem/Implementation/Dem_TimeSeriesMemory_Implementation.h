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
/*! \addtogroup Dem_TimeSeriesMemory
 *  \{
 *  \file       Dem_TimeSeriesMemory_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Implementation of the Time Series Memory Unit which manages storage of time series snapshots.
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

#if !defined (DEM_TIMESERIESMEMORY_IMPLEMENTATION_H)
#define DEM_TIMESERIESMEMORY_IMPLEMENTATION_H

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
#include "Dem_TimeSeriesMemory_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_Cfg_Declarations.h"


/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_TIMESERIESMEMORY_IMPLEMENTATION_FILENAME "Dem_TimeSeriesMemory_Implementation.h"

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /*MD_MSR_MemMap */
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/*! Virtual buffer deletion request flag */
DEM_LOCAL VAR(boolean, DEM_VAR_NO_INIT)           Dem_Memories_TimeSeriesMemory_RequestCleanUp;
#endif
#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /*MD_MSR_MemMap */


/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_TimeSeriesMemory_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_SetBit
 *****************************************************************************/
/*!
 * \brief         Set the specified bit in a bit field
 *
 * \details       Set the specified bit in a bit field
 *
 * \param[in]     BitField
 *                Pointer to a bit field
 *
 * \param[in]     BitPosition
 *                Bit position in bit field
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_SetBit(
  P2VAR(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) BitField,
  uint8 BitPosition
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetBit
 *****************************************************************************/
/*!
 * \brief         Get the specified bit from a bit field
 *
 * \details       Get the specified bit from a bit field
 *
 * \param[in]     BitField
 *                Pointer to a bit field
 *
 * \param[in]     BitPosition
 *                Bit position in bit field
 *
 * \return        The value of the specified bit as a boolean
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetBit(
  P2CONST(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) BitField,
  uint8 BitPosition
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetStartingSequentialDIDIdentifier
 *****************************************************************************/
/*!
 * \brief         Get the starting sequential DID identifier for time-series
 *                with the specified sampling profile.
 *
 * \details       Get the starting sequential DID identifier for time-series
 *                with the specified sampling profile.
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \return        The starting sequential DID identifier
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetStartingSequentialDIDIdentifier(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TssSetIter_GetTssSetId
 *****************************************************************************/
/*!
 * \brief         Get current element in iterator in chronological order
 *
 * \details       Get current element in iterator in chronological order
 *
 * \param[in]     IterPtr
 *                Iterator for Tss Fifo
 *
 * \return        SetId 
 *                Current iterator element in chronological order
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TssSetIter_GetTssSetId(
  CONSTP2CONST(Dem_Cfg_TssFifoEntry_IterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetStorageTrigger
 *****************************************************************************/
/*!
 * \brief         Retrieves the storage trigger of the event's TSSR.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \param[in]     SeriesId
 *                Id specifying a time series of the event stored in the specified entry
 *
 * \return        The storage trigger of the event's time series snapshot.
 *
 * \pre           -
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_StorageTriggerType, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetStorageTrigger(
  Dem_EventIdType EventId,
  Dem_TimeSeriesIdType SeriesId
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetFirstTranslatedMemoryId
 *****************************************************************************/
/*!
 * \brief         Gets the translated id of the first time series memory.
 *
 * \details       -
 *
 * \return        The translated id of the first time series memory.
 *
 * \pre           -
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetFirstTranslatedMemoryId(
  void
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetNumberOfEventMemorySetsWithTimeSeries
 *****************************************************************************/
/*!
 * \brief         Get the number of time series memories.
 *
 * \details       -
 *
 *
 * \return        The number of time series memories.
 *
 * \pre           -
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetNumberOfEventMemorySetsWithTimeSeries(
  void
  );


/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetChronologicalIdOfRecordNumber()
 *****************************************************************************/
/*!
 * \brief         Calculate the index enumerating sets from oldest to newest specifying
 *                the set that contains the specified record number
 *
 * \details       Calculate the index enumerating sets from oldest to newest specifying
 *                the set that contains the specified record number
 *
 * \param[in]     EventId
 *                Id specifying an event
 *
 * \param[in]     SeriesId
 *                Id specifying a time series of the event stored in the specified entry
 *
 * \param[in]     RecordNumber
 *                Record number
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetChronologicalIdOfRecordNumber(
  Dem_EventIdType EventId,
  Dem_TimeSeriesIdType SeriesId,
  uint8  RecordNumber
);

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetSetIdOfChronologicalId()
 *****************************************************************************/
/*!
 * \brief         Calculate the index specifying the position of a time series set
 *                in memory from the index enumerating sets from oldest to newest
 *
 * \details       Calculate the index specifying the position of a time series set
 *                in memory from the index enumerating sets from oldest to newest
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Id specifying a time series entry
 *
 * \param[in]     SeriesId
 *                Id specifying a time series of the event stored in the specified entry
 *
 * \param[in]     ChronologicalId
 *                Id specifying a time series set by order of storage
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetSetIdOfChronologicalId(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  uint8  ChronologicalId
);
#endif

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetMemoryIdOrInvalid
 *****************************************************************************/
/*!
 * \brief         Get the translated memory id for event's Time Series Snapshot
 *                Record.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The translated memory id of the event's TSSR destination.
 *                DEM_CFG_MEMORYID_INVALID if no TSSR destination is supported.
 *
 * \pre           -
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetMemoryIdOrInvalid(
  Dem_EventIdType EventId
);
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetMemoryId
 *****************************************************************************/
/*!
 * \brief         Get the translated memory id for event's Time Series Snapshot
 *                Record.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The translated memory id of the event's TSSR destination.
 *
 * \pre           TSSR destination is supported for the specified event.
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetMemoryId(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TestSnapshotEntryStorageTriggerIsFulfilled
 *****************************************************************************/
/*!
 * \brief         Tests whether a storage trigger is fulfillied for the given 
 *                TSSR entry.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     SeriesId
 *                Id specifying a time series of the event stored in the specified entry
 * \param[in]     UpdateFlags
 *                Trigger fulfilled by the event.
 *
 * \return        TRUE
 *                The storage trigger is fulfilled.
 * \return        FALSE
 *                The storage trigger was not fulfilled.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestSnapshotEntryStorageTriggerIsFulfilled(
  Dem_EventIdType  EventId,
  Dem_TimeSeriesIdType SeriesId,
  Dem_Cfg_StorageTriggerType  UpdateFlags
  );

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TriggerNotConsistentWithSeriesBeingStored
 *****************************************************************************/
/*!
 * \brief         Check if the DTC status is consistent with the time series
 *                being stored based on its storage trigger
 *
 * \details       Check if the DTC status is consistent with the time series
 *                being stored based on its storage trigger
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     SeriesId
 *                Id specifying a time series of the event stored in the specified entry
 * \param[in]     DTCStatus
 *                DTC status byte
 *
 * \return        TRUE
 *                The storage trigger is fulfilled.
 * \return        FALSE
 *                The storage trigger was not fulfilled.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TriggerNotConsistentWithSeriesBeingStored(                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId, 
  Dem_TimeSeriesIdType SeriesId,
  Dem_DTC_UDSStatusType DTCStatus
  );
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/



/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetStartingSequentialDIDIdentifier
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetStartingSequentialDIDIdentifier(                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
  DEM_IGNORE_UNUSED_ARGUMENT(SamplingProfileId);                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON)
  return (uint16)Dem_Cfg_GetSequentialDidIdStartOfTimeSeriesSamplingProfileTable(SamplingProfileId);
#else
  return (uint16)0x00U;
#endif
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetMemoryIdOrInvalid
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
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetMemoryIdOrInvalid(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
)
{
  uint16 lTimeSeriesTranslatedMemoryId = DEM_CFG_MEMORYID_INVALID;

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  uint16 lEventDestinationTranslatedMemoryId = Dem_Core_Event_GetDestination(EventId);
  if (lEventDestinationTranslatedMemoryId != DEM_CFG_MEMORYID_INVALID)
  {
    lTimeSeriesTranslatedMemoryId = Dem_Cfg_GetTimeSeriesTranslatedMemoryIdOfMemoryInfoTable(lEventDestinationTranslatedMemoryId);
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif

  return lTimeSeriesTranslatedMemoryId;
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetMemoryId(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
)
{
  uint16 lTimeSeriesTranslatedMemoryId = Dem_Memories_TimeSeriesMemory_GetMemoryIdOrInvalid(EventId);                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Internal_AssertContinue(lTimeSeriesTranslatedMemoryId != DEM_CFG_MEMORYID_INVALID, DEM_E_INCONSISTENT_STATE)
  return lTimeSeriesTranslatedMemoryId;
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TssSetIter_GetTssSetId()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TssSetIter_GetTssSetId(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONSTP2CONST(Dem_Cfg_TssFifoEntry_IterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  return IterPtr->TssSetId;
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetStorageTrigger
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_StorageTriggerType, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetStorageTrigger(
  Dem_EventIdType EventId,
  Dem_TimeSeriesIdType SeriesId
  )
{
  Dem_Cfg_StorageTriggerType lStorageTrigger = DEM_CFG_TRIGGER_NONE;

  /* The trigger for all time series configured for an event are guaranteed to be the same by the validator */
#if (DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_OFF) && ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON))
  if (Dem_Cfg_IsTimeSeriesIndirectionTableUsedOfEventTable(EventId) == TRUE)                                                     /* COV_DEM_ROBUSTNESS TX */
  {
    lStorageTrigger = Dem_Cfg_GetRecordTriggerOfTimeSeriesFreezeFrameTable(Dem_Cfg_GetTimeSeriesFreezeFrameTableIdxOfTimeSeriesIndirectionTable(SeriesId + Dem_Cfg_GetTimeSeriesIndirectionTableStartIdxOfEventTable(EventId)));
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SeriesId)                                                                                     /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

  return lStorageTrigger;
}


/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetFirstTranslatedMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetFirstTranslatedMemoryId(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
)
{
  uint16 lTimeSeriesMemoryID = (uint16)DEM_CFG_MEMORYID_INVALID;
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  lTimeSeriesMemoryID = (uint16)DEM_CFG_FIRST_MEMORYID_TIMESERIES;
#endif
  return lTimeSeriesMemoryID;
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetNumberOfEventMemorySetsWithTimeSeries
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetNumberOfEventMemorySetsWithTimeSeries(                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
)
{
  uint16 lNumTimeSeriesMemories = 0u;
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  lNumTimeSeriesMemories = (uint16)(DEM_CFG_MAX_NUM_TIMESERIES_MEMORIES);
#endif
  return lNumTimeSeriesMemories;
}

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetChronologicalIdOfRecordNumber
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetChronologicalIdOfRecordNumber(
  Dem_EventIdType EventId,
  Dem_TimeSeriesIdType SeriesId,
  uint8  RecordNumber
)
{
  uint8 lChronologicalId = 0;
#if (DEM_CFG_SUPPORT_TIME_SERIES_FIFO == STD_ON)
  uint8 lTimeSeriesTableId = Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(EventId, SeriesId);
  uint8 lFirstRecordNumber = Dem_Cfg_GetFirstRecordNumberOfTimeSeriesFreezeFrameTable(lTimeSeriesTableId);
  DEM_IGNORE_UNUSED_VARIABLE(lTimeSeriesTableId);                                                                                /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 

  if(lFirstRecordNumber <= RecordNumber)
  {
    uint8 lRecNumFromFirst = RecordNumber - lFirstRecordNumber;
    uint16 lTranslatedMemoryId = Dem_Memories_TimeSeriesMemory_GetMemoryId(EventId);
    uint8 lRecNumOffset = Dem_Cfg_GetTimeSeriesSetsRecNumOffsetOfMemoryInfoTable(lTranslatedMemoryId);
    if(lRecNumOffset == 0u)                                                                                                      /* COV_DEM_ROBUSTNESS XF */
    { /* Should not happen if the generator works correctly. */
      Dem_Internal_AssertContinue(lRecNumOffset != 0u, DEM_E_INCONSISTENT_STATE);
      lRecNumOffset = 1u;
    }
    lChronologicalId = lRecNumFromFirst / lRecNumOffset;
  }
#else
  DEM_IGNORE_UNUSED_ARGUMENT(EventId);                                                                                           /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(SeriesId);                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(RecordNumber);                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
  return lChronologicalId;
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetSetIdOfChronologicalId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetSetIdOfChronologicalId(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  uint8  ChronologicalId
)
{
  uint8 lSetId = 0;
#if (DEM_CFG_SUPPORT_TIME_SERIES_FIFO == STD_ON)
  Dem_TimeSeriesEntry_PtrToConstBaseType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);
  uint16 lEventId = lEntry->EventId;
  if(1u < Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(lEventId))
  {
    /* Start from the index of the oldest set */
    lSetId = lEntry->AllSetsStored[SeriesId] ? lEntry->NextSetId[SeriesId] : 0u;

    /* Move forwards lChronologicalId steps */
    lSetId += ChronologicalId;

    /* Wrap if needed */
    lSetId %= Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(lEventId);
  }
#else
  DEM_IGNORE_UNUSED_ARGUMENT(SeriesId);                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(ChronologicalId);                                                                                   /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(TimeSeriesEntryIndex);                                                                              /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
  return lSetId;
}
#endif

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_SetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_SetBit(                                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  P2VAR(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) BitField,                                                                     /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  uint8 BitPosition
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  BitField->data[BitPosition / 8u] |= (1u << (BitPosition % 8u));                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#else
  DEM_IGNORE_UNUSED_ARGUMENT(BitField);                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(BitPosition);                                                                                       /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetBit(                                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  P2CONST(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) BitField,
  uint8 BitPosition
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  return (boolean)(0u != (BitField->data[BitPosition / 8u] & (1u << (BitPosition % 8u))));
#else
  DEM_IGNORE_UNUSED_ARGUMENT(BitField);                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(BitPosition);                                                                                       /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  return FALSE;
#endif
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TestSnapshotEntryStorageTriggerIsFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestSnapshotEntryStorageTriggerIsFulfilled(                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId,
  Dem_TimeSeriesIdType SeriesId,
  Dem_Cfg_StorageTriggerType UpdateFlags
  )
{
  Dem_Cfg_StorageTriggerType StorageTrigger = Dem_Memories_TimeSeriesMemory_GetStorageTrigger(EventId, SeriesId);
  return (boolean)((StorageTrigger & UpdateFlags) == StorageTrigger);
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TriggerNotConsistentWithSeriesBeingStored
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
Dem_Memories_TimeSeriesMemory_TriggerNotConsistentWithSeriesBeingStored(                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId, 
  Dem_TimeSeriesIdType SeriesId,
  Dem_DTC_UDSStatusType DTCStatus
)
{
  boolean lReturnValue = FALSE;
  if (Dem_Memories_TimeSeriesMemory_GetStorageTrigger(EventId, SeriesId) == DEM_CFG_TRIGGER_CONFIRMED)
  {
    lReturnValue = (boolean)(Dem_Event_UDSStatus_TestBit(DTCStatus, Dem_UdsStatus_CDTC) == FALSE);
  }
  else if (Dem_Memories_TimeSeriesMemory_GetStorageTrigger(EventId, SeriesId) == DEM_CFG_TRIGGER_TESTFAILED_FIRST_IN_CYCLE)
  {
# if (DEM_CFG_PROCESS_TFSLC_ALL_DTC == STD_ON) || (DEM_CFG_PROCESS_TFSLC_CLEARONLY == STD_ON)
    lReturnValue = (Dem_Event_UDSStatus_TestBit(DTCStatus, Dem_UdsStatus_TFSLC) == FALSE);
# endif
  }
else if (Dem_Memories_TimeSeriesMemory_GetStorageTrigger(EventId, SeriesId) == DEM_CFG_TRIGGER_TESTFAILED)                       /* COV_DEM_ROBUSTNESS TX */
  {
# if (DEM_CFG_PROCESS_TFSLC_ALL_DTC == STD_ON) || (DEM_CFG_PROCESS_TFSLC_CLEARONLY == STD_ON)
    lReturnValue = (Dem_Event_UDSStatus_TestBit(DTCStatus, Dem_UdsStatus_TFSLC) == FALSE);
# endif
  }
  else
  {
    /* Unexpected trigger, unable to determine whether entry was expected or not */
  }
  return lReturnValue;
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
 * \addtogroup Dem_TimeSeriesMemory_PublicProperties
 * \{
 */

/*****************************************************************************
 % Dem_Memories_TimeSeriesMemory_SetAllBits
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_SetAllBits(                                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  P2VAR(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) BitField                                                                      /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  uint8_least i;
  for(i = 0u; i < 8u; i++) {
    BitField->data[i] = 255u;                                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
#else
  DEM_IGNORE_UNUSED_ARGUMENT(BitField);                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
}

/*****************************************************************************
 % Dem_Memories_TimeSeriesMemory_SetBitsForTriggeredSeries
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_SetBitsForTriggeredSeries(
  P2VAR(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) TriggerBitField,                                                              /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  P2CONST(Dem_DTC_ContextType, AUTOMATIC, AUTOMATIC) DTCContext
  )
{
  boolean lStorageTriggered = FALSE;
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_TimeSeriesIdType lSeriesId;
  Dem_BitField256Bits lC90StructLiteralSubstitute = { 0 };
  *TriggerBitField = lC90StructLiteralSubstitute;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

  for(lSeriesId = 0; lSeriesId < Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(DTCContext->EventId); lSeriesId++) {
    if(Dem_Memories_TimeSeriesMemory_TestSnapshotEntryStorageTriggerIsFulfilled(DTCContext->EventId, (Dem_TimeSeriesIdType)lSeriesId, DTCContext->DtcEffects) == TRUE) {
      Dem_Memories_TimeSeriesMemory_SetBit(TriggerBitField, (Dem_TimeSeriesIdType)lSeriesId);                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      lStorageTriggered = TRUE;
    }
  }
#else
  DEM_IGNORE_UNUSED_ARGUMENT(TriggerBitField);                                                                                   /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(DTCContext);                                                                                        /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
  return lStorageTriggered;
}

/*****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetGlobalEntryCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetGlobalEntryCount(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
)
{
  uint16 lGlobalTimeSeriesEntries = 0u;
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  lGlobalTimeSeriesEntries = (uint16)(DEM_CFG_GLOBAL_TIMESERIES_SNAPSHOTS_SIZE);
#endif
  return lGlobalTimeSeriesEntries;
}

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetSizeOfTimeSeriesEntryTable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_TimeSeriesEntryTableIterType, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetSizeOfTimeSeriesEntryTable(
  void
  )
{
  return Dem_Cfg_GetSizeOfTimeSeriesEntryTable();
}
#endif


/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_IsPowertrainEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_IsPowertrainEnabled(
  void
)
{
  return (boolean)(DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON);
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetPrimaryTranslatedMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetPrimaryTranslatedMemoryId(                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return (uint16)(Dem_Cfg_GetTimeSeriesTranslatedMemoryIdOfMemoryInfoTable(DEM_CFG_MEMORYID_PRIMARY));
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_IsReferredByMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_IsReferredByMemoryId(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16 TranslatedMemoryId
  )
{
  boolean lIsTimeSeriesMemory = FALSE;
  if ((TranslatedMemoryId >= Dem_Memories_TimeSeriesMemory_GetFirstTranslatedMemoryId())
    && (TranslatedMemoryId < (Dem_Memories_TimeSeriesMemory_GetFirstTranslatedMemoryId()
                              + Dem_Memories_TimeSeriesMemory_GetNumberOfEventMemorySetsWithTimeSeries())))
  {
    lIsTimeSeriesMemory = TRUE;
  }
  return lIsTimeSeriesMemory;
}


/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TssSetIter_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TssSetIter_Init(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  CONSTP2VAR(Dem_Cfg_TssFifoEntry_IterType, AUTOMATIC, AUTOMATIC) IterPtr
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  if (TimeSeriesEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)                                                                        /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_TimeSeriesEntry_PtrToConstBaseType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);

    IterPtr->EventId = lEntry->EventId;                                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    IterPtr->TimeSeriesEntryIndex = TimeSeriesEntryIndex;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    IterPtr->TssChronoId = 0u;                                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    IterPtr->SeriesId = SeriesId;                                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# if (DEM_CFG_SUPPORT_TIME_SERIES_FIFO == STD_ON)
    if (lEntry->AllSetsStored[SeriesId] == TRUE)
    {
      /* Wraparound has happened, the header is the oldest set */
      IterPtr->TssSetId = lEntry->NextSetId[SeriesId];                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
# endif
    {
      /* Wraparound has not yet happened. The first set is the oldest one */
      IterPtr->TssSetId = 0u;                                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
  else
#else
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(TimeSeriesEntryIndex)                                                                       /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(SeriesId)                                                                                   /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
  {
    IterPtr->EventId = DEM_EVENT_INVALID;                                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    IterPtr->TimeSeriesEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    IterPtr->SeriesId = 0u;                                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    IterPtr->TssChronoId = 0u;                                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    IterPtr->TssSetId = 0u;                                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TssSetIter_Exists
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TssSetIter_Exists(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONSTP2CONST(Dem_Cfg_TssFifoEntry_IterType, AUTOMATIC, AUTOMATIC) IterPtr
)
{
  boolean lReturnValue = FALSE;
  if(IterPtr->TimeSeriesEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)                                                                /* COV_DEM_ROBUSTNESS TX */
  {
    lReturnValue = (IterPtr->TssChronoId < Dem_Memories_TimeSeriesMemory_GetNumberOfStoredTimeSeriesSets(IterPtr->TimeSeriesEntryIndex, IterPtr->SeriesId));
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TssSetIter_Next
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TssSetIter_Next(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONSTP2VAR(Dem_Cfg_TssFifoEntry_IterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  IterPtr->TssChronoId++;                                                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->TssSetId = (IterPtr->TssSetId + 1u) % Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(IterPtr->EventId);             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}


/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_EventDidIter_Exists
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_EventDidIter_Exists(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
CONSTP2CONST(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
    return (boolean)(IterPtr->mIdx < IterPtr->mEndIdx);
#else
    return FALSE;
#endif
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_EventDidIter_Get
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DidIndexType, DEM_CODE)
Dem_Memories_TimeSeriesMemory_EventDidIter_Get(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
CONSTP2CONST(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                            /* PRQA S 3206 */ /* MD_DEM_3206 */
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
    return Dem_Cfg_GetDataCollectionTableFfm2CollInd(IterPtr->mIdx);
#else
    return (Dem_Cfg_DidIndexType)0x00;
#endif
}


/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_EventDidIter_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_EventDidIter_Init(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId,                                                                                                       /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONSTP2VAR(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                            /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
# if(DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON)
  IterPtr->mIdx = Dem_Cfg_GetDataCollectionTableFfm2CollIndStartIdxOfFreezeFrameTable(Dem_Cfg_GetFreezeFrameTableDevFFBIdxOfEventTable(EventId)); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Cfg_GetDataCollectionTableFfm2CollIndEndIdxOfFreezeFrameTable(Dem_Cfg_GetFreezeFrameTableDevFFBIdxOfEventTable(EventId)); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  IterPtr->mIdx = Dem_Cfg_GetDataCollectionTableFfm2CollIndStartIdxOfFreezeFrameTable(Dem_Cfg_GetFreezeFrameTableStdFFIdxOfEventTable(EventId)); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Cfg_GetDataCollectionTableFfm2CollIndEndIdxOfFreezeFrameTable(Dem_Cfg_GetFreezeFrameTableStdFFIdxOfEventTable(EventId)); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# endif
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  IterPtr->mIdx = 0x00u;                                                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = 0x00u;                                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#endif
}


/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_EventDidIter_Next
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_EventDidIter_Next(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
CONSTP2VAR(Dem_Cfg_EventDidIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                                              /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  IterPtr->mIdx += 1u;                                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#else
  IterPtr->mIdx = 0x00u;                                                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#endif
}


/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_IsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_IsSupported(
  void
)
{
  return (boolean)(DEM_FEATURE_NEED_TIME_SERIES == STD_ON);
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TestSamplingProfileExists
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestSamplingProfileExists(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  return (boolean)(Dem_Cfg_GetSizeOfTimeSeriesSamplingProfileTable() > 0U);
#else
  return FALSE;
#endif
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries(
  Dem_EventIdType  EventId
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  return (Dem_Cfg_GetTimeSeriesIndirectionTableStartIdxOfEventTable(EventId) != DEM_CFG_NO_TIMESERIESINDIRECTIONTABLESTARTIDXOFEVENTTABLE);
#else
  DEM_IGNORE_UNUSED_ARGUMENT(EventId);                                                                                           /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  return FALSE;
#endif
}

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetLastRecordNumberOfSeries
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetLastRecordNumberOfSeries(
  Dem_EventIdType  EventId,
  Dem_TimeSeriesIdType SeriesId
)
{
  uint8 lReturn = 0;
  uint16 lTimeSeriesTranslatedMemoryId = Dem_Memories_TimeSeriesMemory_GetMemoryIdOrInvalid(EventId);
  if (DEM_CFG_MEMORYID_INVALID != lTimeSeriesTranslatedMemoryId)
  {
    uint8 lRecNumOffset = Dem_Cfg_GetTimeSeriesSetsRecNumOffsetOfMemoryInfoTable(lTimeSeriesTranslatedMemoryId);
    uint8 lLastRecordNumber = Dem_Cfg_GetLastRecordNumberOfTimeSeriesFreezeFrameTable(Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(EventId, SeriesId));
    uint8 lFinalSetId = Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(EventId) - 1u;
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(SeriesId)
    lReturn = lLastRecordNumber + (lFinalSetId * lRecNumOffset);
  }

  return lReturn;
}
#endif

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TestRecordNumberIsInSeries
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
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestRecordNumberIsInSeries(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType  EventId,
  Dem_TimeSeriesIdType SeriesId,
  uint8 RecordNumber
)
{
  uint8 lFirstRecordNumber = Dem_Cfg_GetFirstRecordNumberOfTimeSeriesFreezeFrameTable(Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(EventId, SeriesId));
  return (lFirstRecordNumber <= RecordNumber) &&
    (Dem_Memories_TimeSeriesMemory_GetChronologicalIdOfRecordNumber(EventId, SeriesId, RecordNumber) < Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(EventId)) && 
    (Dem_Memories_TimeSeriesMemory_GetSnapshotOffsetFromFirstOneInSet(EventId, SeriesId, RecordNumber) < Dem_Memories_TimeSeriesMemory_GetNumberOfSamplesOfSeries(EventId, SeriesId));
}
#endif


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetSnapshotOffsetFromFirstOneInSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetSnapshotOffsetFromFirstOneInSet(                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType  EventId,
  Dem_TimeSeriesIdType SeriesId,
  uint8 RecordNumber
)
{
  uint8 lReturn = 0;
  uint16 lTimeSeriesTranslatedMemoryId = Dem_Memories_TimeSeriesMemory_GetMemoryIdOrInvalid(EventId);
  if (DEM_CFG_MEMORYID_INVALID != lTimeSeriesTranslatedMemoryId)                                                                 /* COV_DEM_ROBUSTNESS TX */
  {
    uint8 lRecNumOffset = Dem_Cfg_GetTimeSeriesSetsRecNumOffsetOfMemoryInfoTable(lTimeSeriesTranslatedMemoryId);
    uint8 lChronologicalId = Dem_Memories_TimeSeriesMemory_GetChronologicalIdOfRecordNumber(EventId, SeriesId, RecordNumber);
    uint8 lFirstRecordNumberInSet = Dem_Cfg_GetFirstRecordNumberOfTimeSeriesFreezeFrameTable(Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(EventId, SeriesId))
      + (lChronologicalId * lRecNumOffset);
    lReturn = RecordNumber - lFirstRecordNumberInSet;

  }
  
  return lReturn;
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetNumberOfSamplesOfSeries()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetNumberOfSamplesOfSeries(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType  EventId,
  Dem_TimeSeriesIdType SeriesId
)
{
  Dem_Cfg_SamplingProfileIterType lSamplingProfileId = Dem_Memories_TimeSeriesMemory_GetSamplingProfileId(EventId, SeriesId);
  DEM_IGNORE_UNUSED_VARIABLE(lSamplingProfileId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

  return Dem_Cfg_GetPastSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId) + Dem_Cfg_GetFutureSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId);
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TestRecordNumberIsValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestRecordNumberIsValid(                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId,
  uint8  RecordNumber
  )
{
  boolean lReturnValue = FALSE;
  if(Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries(EventId))
  {
    Dem_TimeSeriesIdType lSeriesId = Dem_Memories_TimeSeriesMemory_GetNextOrLastSeriesByRecordNumber(EventId, RecordNumber);
    lReturnValue = Dem_Memories_TimeSeriesMemory_TestRecordNumberIsInSeries(EventId, lSeriesId, RecordNumber);
  }

  return lReturnValue;
}
#endif





/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  /* Always at least 1 set */
  uint8 lMaxTssSets = 1u;

#if (DEM_CFG_SUPPORT_TIME_SERIES_FIFO == STD_ON)
  {
    uint16 lTranslatedMemoryId = Dem_Memories_TimeSeriesMemory_GetMemoryId(EventId);
    lMaxTssSets = Dem_Cfg_GetMaxNumberTimeSeriesSetsOfMemoryInfoTable(lTranslatedMemoryId);
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
  return lMaxTssSets;
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetNumberOfStoredTimeSeriesSets
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetNumberOfStoredTimeSeriesSets(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_TimeSeriesEntry_PtrToConstBaseType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);
  return (uint8)(lEntry->AllSetsStored[SeriesId]
    ? Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(lEntry->EventId)
    : lEntry->NextSetId[SeriesId]);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TimeSeriesEntryIndex)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SeriesId)                                                                                     /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return 0u;
#endif
}


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetSetIdOfRecordNumber
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetSetIdOfRecordNumber(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  uint8  RecordNumber
)
{
  uint8 lSetId = 0;
#if (DEM_CFG_SUPPORT_TIME_SERIES_FIFO == STD_ON)
  Dem_TimeSeriesEntry_PtrToConstBaseType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);

  if (Dem_Memories_TimeSeriesMemory_TestRecordNumberIsInSeries(lEntry->EventId, SeriesId, RecordNumber))                         /* COV_DEM_ROBUSTNESS TX */
  { /* Calculate chronological index */
    uint8 lChronologicalId = Dem_Memories_TimeSeriesMemory_GetChronologicalIdOfRecordNumber(lEntry->EventId, SeriesId, RecordNumber);
    lSetId = Dem_Memories_TimeSeriesMemory_GetSetIdOfChronologicalId(TimeSeriesEntryIndex,  SeriesId, lChronologicalId);
  }
  else
  {
    lSetId = Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(lEntry->EventId);
  }
#else
  DEM_IGNORE_UNUSED_ARGUMENT(TimeSeriesEntryIndex);                                                                              /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(SeriesId);                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(RecordNumber);                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif

  return lSetId;
}
#endif


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
 * \defgroup Dem_TimeSeriesMemory_Private Private Methods
 * \{
 */

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_AllocateEntry
 *****************************************************************************/
/*!
 * \brief         Selects a time series entry for an event
 *
 * \details       This function manages the time series snapshot data storage.
 *                If a free slot is available, it is returned directly. In case
 *                no free slot is available, the configured displacement
 *                algorithm is applied to free a slot.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The time series entry index of the allocated time series entry if one
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
Dem_Memories_TimeSeriesMemory_AllocateEntry(
  Dem_EventIdType EventId
  );
#endif


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_ProcessCleanUp
 *****************************************************************************/
/*!
 * \brief         Processes asynchronous tasks to clean up the time series entries
 *
 * \details       Remove time series entries, which were requested to be deleted and
 *                are not used as virtual readout buffer any more.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_ProcessCleanUp(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_StorePastSamples
 *****************************************************************************/
/*!
 * \brief         Store past samples for given event
 *
 * \details       Store past samples for given event
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \param[in]     StorageTriggerMask
 *                Pointer to a bit field indicating for which time series the storage
 *                trigger has occured
 * 
 *
 * \pre           Time series entry allocated for Event
 *                Event is configured to Store/Update Normal Rate Past TSSR
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_StorePastSamples(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  P2CONST(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) StorageTriggerMask
);

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_GetAndUpdateEntryHeaderSetId
 *****************************************************************************/
/*!
 * \brief          Retrieve and update the next TSS set Id in the TSSR entry for storage
 *                 
 * \details        Retrieve and update the next TSS set Id in the TSSR entry for storage
 *                 and update the Tssr Fifo indices
 *
 * \param[in]      TimeSeriesEntryIndex
 *                 Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                 Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 * \param[in]      SeriesId
 *                 Id identifying one of the time series configured for the stored event.
 *
 * \return         Next TSS set Id for storage in Time series entry
 *                 If TSS Fifo is not enabled, 0u is returned always
 *
 * \pre            Function must be invoked only when time series snapshot records
 *                 are enabled.
 * \config         -
 *
 * \context        TASK|ISR2
 * \synchronous    TRUE
 * \reentrant      TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetAndUpdateEntryHeaderSetId(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId
  );
#endif


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_SeriesHasStoredSet
 *****************************************************************************/
/*!
 * \brief          Check if any set belonging to this time series has been stored
 *                 
 * \details        Check if any set belonging to this time series has been stored
 *
 * \param[in]      TimeSeriesEntryIndex
 *                 Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                 Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 * \param[in]      SeriesId
 *                 Id identifying one of the time series configured for the stored event.
 *
 * \return         TRUE if a set is stored, False otherwise
 *
 * \pre            Function must be invoked only when time series snapshot records
 *                 are enabled.
 * \config         -
 *
 * \context        TASK|ISR2
 * \synchronous    TRUE
 * \reentrant      TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_SeriesHasStoredSet(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId
  );
#endif

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_InitCompanionEntries
 *****************************************************************************/
/*!
 * \brief         Initializes all time series companion entries to unused
 *
 * \details       Initializes all time series companion entries to unused
 *
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
Dem_Memories_TimeSeriesMemory_InitCompanionEntries(
  void
  );

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_HasNoValidDataAfterResettingInconsistentSeries
 *****************************************************************************/
/*!
 * \brief         Check whether the time series entry is unexpected for the 
 *                given event.
 *
 * \details       Depending on the configured storage trigger, check whether 
 *                the event is missing an expected primary entry or has 
 *                inconsistent UDS status bits.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Event cannot store a TSSR entry.
 * \return        FALSE
 *                Event can store a TSSR entry.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_HasNoValidDataAfterResettingInconsistentSeries(
  Dem_EventIdType EventId
  );
#endif

#if (DEM_CFG_SUPPORT_TIME_SERIES_FUTURE == STD_ON)
/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TriggerNvMStorageOnFutureSampleUpdate
 *****************************************************************************/
/*!
 * \brief         Mark the time series entry changed for NvM persistence.
 *
 * \details       Mark the time series entry changed for NvM persistence.
 *                Depending on configuration either 'dirty' or 'dirty immediate'
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 * \param[in]     EntryCompanion
 *                Pointer to Time Series Entry Companion
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \config        DEM_CFG_SUPPORT_TIME_SERIES_FUTURE == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TriggerNvMStorageOnFutureSampleUpdate(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesEntry_ConstCompanionPtrToConstType EntryCompanion,
  Dem_EventIdType  EventId
  );
#endif


/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_ProcessNewFutureSampleAvailable
 *****************************************************************************/
/*!
 * \brief         Store time series record future samples for all entries
 *
 * \details       Store time series record future samples for all entries
 *
 * \pre           -
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_ProcessNewFutureSampleAvailable(
  void
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_StoreFutureSamples
 *****************************************************************************/
/*!
 * \brief         Store future samples for given entry and series
 *
 * \details       Store future samples for given entry and series
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 * \param[in]     SeriesId
 *                Id identifying one of the time series configured for the stored event.
 *
 * \return        TRUE
 *                Entry has changed
 * \return        FALSE
 *                Entry has not changed
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_StoreFutureSamples(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId
);

/* ****************************************************************************
 * Dem_Memories_TimeSeriesMemory_TimeSeriesMemoriesIterInit
 *****************************************************************************/
/*!
 * \brief         Initializes for iteration over all time series memories.
 *
 * \details       Initializes for iteration over all time series memories.
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TimeSeriesMemoriesIterInit(
  CONSTP2VAR(Dem_Cfg_TSSRTranslatedMemoryIdIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_AllocateEntry
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
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Memories_TimeSeriesMemory_AllocateEntry(
  Dem_EventIdType EventId
  )
{
  Dem_EventIdType lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);
  uint16 lTimeSeriesTranslatedMemoryId = Dem_Memories_TimeSeriesMemory_GetMemoryIdOrInvalid(EventId);
  Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;

  if (Dem_FaultMemory_Memories_GetMaxSize(lTimeSeriesTranslatedMemoryId) == Dem_FaultMemory_Memories_GetCurrentSize(lTimeSeriesTranslatedMemoryId))
  { /* Memory is filled up */
    lTimeSeriesEntryIndex = Dem_StorageManager_Displacement_SelectDisplacedIndex(lTimeSeriesTranslatedMemoryId, EventId);
    if (lTimeSeriesEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
    { /* a displaced DTC was identified, now remove that DTC */
      Dem_EventIdType lDisplacedEventId = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(lTimeSeriesEntryIndex)->EventId;
      Dem_Event_DataUpdateStart(lDisplacedEventId);
      Dem_FaultMemory_Memories_RemoveEntryAndChrono(lTimeSeriesTranslatedMemoryId, lTimeSeriesEntryIndex);
      Dem_Event_DataUpdateFinish(lDisplacedEventId);
    }
  }
  else
  { /* Memory is not full -> Find first free entry */
    lTimeSeriesEntryIndex = Dem_FaultMemory_Memories_FindEntryIndexOfEventGeneric(DEM_EVENT_INVALID, FALSE, lTimeSeriesTranslatedMemoryId);
    /* if not found : lTimeSeriesEntryIndex == DEM_ENRYINDEX_INVALID */
  }

  if (lTimeSeriesEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    Dem_Cfg_TimeSeriesEntryBasePtrType lEntry;
    Dem_TimeSeriesEntry_CompanionPtrType lEntryCompanion;
    uint8 lMemorySize;

    lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(lTimeSeriesEntryIndex);
    lEntryCompanion = Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(lTimeSeriesEntryIndex);
    lMemorySize = Dem_FaultMemory_Memories_GetCurrentSize(lTimeSeriesTranslatedMemoryId);

    /* fill the chrono record with event id and lock the referenced time series slot */
    Dem_FaultMemory_Memories_SetChronology(lTimeSeriesTranslatedMemoryId, lMemorySize, lTimeSeriesEntryIndex);

    lEntry->EventId = lMasterEventId;                                                                                            /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
    lEntry->Timestamp = Dem_FaultMemory_Memories_GetCurrentTimestamp();                                                          /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */

    Dem_MemoryEntry_TimeSeriesEntry_Init(lEntry, EventId);                                                                       /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
    Dem_MemoryEntry_TimeSeriesEntry_EntryCompanionInit(lEntryCompanion,                                                          /* SBSW_DEM_POINTER_WRITE_TIMESERIESENTRYCOMPANION */
      DEM_TIMESERIESENTRY_READOUTSTATUS_UNLOCKED); 

    Dem_FaultMemory_Memories_SetCurrentTimestamp(Dem_FaultMemory_Memories_GetCurrentTimestamp() + 1u);
    Dem_FaultMemory_Memories_SetCurrentSize(lTimeSeriesTranslatedMemoryId, (uint8)(lMemorySize + 1u));
  }
  return lTimeSeriesEntryIndex;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_ProcessCleanUp
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
Dem_Memories_TimeSeriesMemory_ProcessCleanUp(
  void
  )
{
  Dem_Cfg_EntryIterType lTimeSeriesEntryIter;

  for (Dem_Cfg_ComplexIter32BitInit(&lTimeSeriesEntryIter, 0u, Dem_Memories_TimeSeriesMemory_GetSizeOfTimeSeriesEntryTable());   /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_ComplexIter32BitExists(&lTimeSeriesEntryIter) == TRUE;                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_ComplexIter32BitNext(&lTimeSeriesEntryIter))                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lTimeSeriesEntryIter);                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_EventIdType lEventId = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(lTimeSeriesEntryIndex)->EventId;
    if (Dem_MemoryEntry_TimeSeriesEntry_TestEntryNeedToBeCleaned(lTimeSeriesEntryIndex))
    {
      Dem_Cfg_NvBlockIndexType lNvBlockIndex = Dem_MemoryEntry_TimeSeriesEntry_GetEntryIndexToNvBlockIndex(lTimeSeriesEntryIndex);
      Dem_FaultMemory_Memories_RemoveEntryAndChrono(Dem_Memories_TimeSeriesMemory_GetMemoryId(lEventId), lTimeSeriesEntryIndex);
      Dem_Nvm_SetTimeSeriesMemoryBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_StorePastSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_StorePastSamples(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  P2CONST(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) StorageTriggerMask
  )
{
  uint8 lSourceSample;
  Dem_Cfg_TimeSeriesEntryBasePtrType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);

  Dem_TimeSeriesIdType lSeriesId;
  Dem_TimeSeriesIdType lNumberOfSeries = Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(lEntry->EventId);
  for(lSeriesId = 0; lSeriesId < lNumberOfSeries; lSeriesId++)
  {
    if(Dem_Memories_TimeSeriesMemory_GetBit(StorageTriggerMask, lSeriesId)) {                                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      boolean lIsUpdate = Dem_Memories_TimeSeriesMemory_SeriesHasStoredSet(TimeSeriesEntryIndex, lSeriesId);
      uint8 lSetId = Dem_Memories_TimeSeriesMemory_GetAndUpdateEntryHeaderSetId(TimeSeriesEntryIndex, lSeriesId);
      uint8 lTimeSeriesTableId = Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(lEntry->EventId, lSeriesId);
      Dem_Cfg_SamplingProfileIterType lSamplingProfileId = Dem_Cfg_GetTimeSeriesSamplingProfileTableIdxOfTimeSeriesFreezeFrameTable(lTimeSeriesTableId);
      uint16 lTimeSinceLastSample = Dem_GlobalDiagnostics_GetTimeSinceLastRingBufferSample(lSamplingProfileId);

      uint8 lPastSamples = Dem_Cfg_GetPastSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId);
      uint8 lNumberStoredSamples = Dem_GlobalDiagnostics_GetRingBufferSampleCount(lSamplingProfileId);
      uint8 lNumberNotStoredSamples = (lPastSamples - lNumberStoredSamples);
      DEM_IGNORE_UNUSED_VARIABLE(lSamplingProfileId)
      DEM_IGNORE_UNUSED_VARIABLE(lTimeSeriesTableId)
      Dem_Internal_AssertContinue(lSamplingProfileId < Dem_Cfg_GetSizeOfTimeSeriesSamplingProfileTable(), DEM_E_INCONSISTENT_STATE);

      if((lIsUpdate == FALSE) || (Dem_Cfg_IsUpdateRecordOfTimeSeriesFreezeFrameTable(lTimeSeriesTableId)))
      {
        for (lSourceSample = 0u; lSourceSample < lNumberStoredSamples; ++lSourceSample)
        {
          Dem_Data_DestinationBuffer lSnapshotDataBuffer;

          Dem_MemoryEntry_TimeSeriesEntry_InitSnapshotDataBuffer(                                                                /* SBSW_DEM_CALL_TIMESERIES_SNAPSHOTBUFFER_INIT */
              TimeSeriesEntryIndex, 
              lSeriesId,
              lSetId,
              lSourceSample + lNumberNotStoredSamples, 
              &lSnapshotDataBuffer);                                                                                             /* SBSW_DEM_POINTER_FORWARD_STACK */

          Dem_Memories_TimeSeriesMemory_CopyDidsFromRingBuffer(
            lEntry->EventId,
            lSamplingProfileId,
            lSourceSample,
            &lSnapshotDataBuffer                                                                                                 /* SBSW_DEM_POINTER_FORWARD_STACK */
            );
        }

        {
# if (DEM_CFG_SUPPORT_TIME_SERIES_FUTURE == STD_ON)
          uint8 lFutureSamples = Dem_Cfg_GetFutureSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId);
          Dem_TimeSeriesEntry_CompanionPtrType lEntryCompanion = Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(TimeSeriesEntryIndex);
          Dem_MemoryEntry_TimeSeriesEntry_SetMissingFutureSamples(lEntryCompanion, lSeriesId, lSetId, lFutureSamples);           /* SBSW_DEM_POINTER_WRITE_TIMESERIESENTRYCOMPANION */
          Dem_MemoryEntry_TimeSeriesEntry_SetUnusedFutureSamples(lEntry, lSeriesId, lSetId, lFutureSamples);                     /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
# endif
          Dem_MemoryEntry_TimeSeriesEntry_SetUnusedPastSamples(lEntry, lSeriesId, lSetId, lNumberNotStoredSamples);              /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
        }
      }

      Dem_MemoryEntry_TimeSeriesEntry_SetTimeSinceLastSample(lEntry, lTimeSinceLastSample);                                      /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
    }
  }


}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetAndUpdateEntryHeaderSetId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetAndUpdateEntryHeaderSetId(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId
  )
{
  uint8 lNextTssSetId = 0u;
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (SeriesId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SERIES)
  {
    Dem_Error_RunTimeCheckFailed(DEM_TIMESERIESMEMORY_IMPLEMENTATION_FILENAME, __LINE__);                                        /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_TimeSeriesEntryBasePtrType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);
    uint8 lMaxNumSets = Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(lEntry->EventId);
    uint8 lTssFifoSetIndex = lEntry->NextSetId[SeriesId];

    /* If Tss fifo is full, set wraparound flag to true */
    if (lTssFifoSetIndex == (lMaxNumSets - 1u))
    {
      /* Indicate all sets are stored and wraparound will happen */
      lEntry->AllSetsStored[SeriesId] = TRUE;                                                                                    /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
    }

    /* The current TssFifoSetIndex shall be returned */
    lNextTssSetId = lTssFifoSetIndex;

    if (lMaxNumSets > 1u)
    {
      /* Increment FIfo index to point to next index */
      lTssFifoSetIndex = (lTssFifoSetIndex + 1u) % lMaxNumSets;
    }

    /* Update the TssFifoSetIndex to point to the next free index */
    lEntry->NextSetId[SeriesId] = lTssFifoSetIndex;                                                                              /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
  }

  return lNextTssSetId;
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_SeriesHasStoredSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_SeriesHasStoredSet(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId
  )
{
  Dem_TimeSeriesEntry_PtrToConstBaseType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);
  return (boolean)(lEntry->AllSetsStored[SeriesId] == TRUE) || (lEntry->NextSetId[SeriesId] != 0u);
}
#endif



/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_InitCompanionEntries
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_InitCompanionEntries(
  void
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) 
  uint8 lIndex;
  for (lIndex = 0; lIndex < Dem_Cfg_GetSizeOfTimeSeriesEntryCompanion(); lIndex++)
  {
    Dem_MemoryEntry_TimeSeriesEntry_EntryCompanionInit(Dem_Cfg_GetAddrTimeSeriesEntryCompanion(lIndex),                          /* SBSW_DEM_ARRAY_WRITE_TIMESERIESENTRYCOMPANION */
      DEM_TIMESERIESENTRY_READOUTSTATUS_UNUSED);
  }
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_HasNoValidDataAfterResettingInconsistentSeries
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
Dem_Memories_TimeSeriesMemory_HasNoValidDataAfterResettingInconsistentSeries(
  Dem_EventIdType EventId
  )
{
  boolean lHasConsistentSeries = FALSE;
  Dem_DTC_UDSStatusType lDTCStatus = Dem_Core_Event_GetInternalUdsStatus(EventId);
  if ( Dem_Memories_TimeSeriesMemory_IsPowertrainEnabled())
  {
    Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(EventId, FALSE);
    lHasConsistentSeries = (boolean)(lTimeSeriesEntryIndex != DEM_CFG_ENTRYINDEX_INVALID);
  }
  else
  {
    Dem_TimeSeriesIdType lSeriesId;
    Dem_TimeSeriesIdType lNumberOfSeries = Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(EventId);
    Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = Dem_Memories_TimeSeriesMemory_FindEntryIndex(EventId);
    if(lTimeSeriesEntryIndex < Dem_Memories_TimeSeriesMemory_GetSizeOfTimeSeriesEntryTable())                                    /* COV_DEM_ROBUSTNESS TX */
    {
      Dem_TimeSeriesEntry_PtrToBaseType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(lTimeSeriesEntryIndex);
      for(lSeriesId = 0; lSeriesId < lNumberOfSeries; lSeriesId++)
      {
        if((Dem_Memories_TimeSeriesMemory_TriggerNotConsistentWithSeriesBeingStored(EventId, lSeriesId, lDTCStatus)))
        {
          Dem_MemoryEntry_TimeSeriesEntry_InitializeSeries(lEntry, EventId, lSeriesId);                                          /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
        else 
        {
          lHasConsistentSeries = TRUE;
        }
      }
    }
    else
    {
      /* If there is no valid entry lHasConsistentSeries will be FALSE */
    }
  }

  return lHasConsistentSeries == FALSE;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_TIME_SERIES_FUTURE == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TriggerNvMStorageOnFutureSampleUpdate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TriggerNvMStorageOnFutureSampleUpdate(
 Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesEntry_ConstCompanionPtrToConstType  EntryCompanion,
  Dem_EventIdType  EventId
  )
{
  uint8 lSetId;
  uint16 lMissingSamples = 0u;

  Dem_TimeSeriesIdType lSeriesId;
  Dem_TimeSeriesIdType lNumberOfSeries = Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(EventId);
  for (lSeriesId = 0; lSeriesId < lNumberOfSeries; lSeriesId++)
  {
    for (lSetId = 0u; lSetId < Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(EventId); lSetId++)
    {
      lMissingSamples += Dem_MemoryEntry_TimeSeriesEntry_GetMissingFutureSamples(EntryCompanion, lSeriesId, lSetId);             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }

  {
    Dem_Cfg_NvBlockIndexType lNvBlockIndex = Dem_MemoryEntry_TimeSeriesEntry_GetEntryIndexToNvBlockIndex(TimeSeriesEntryIndex);
    if ( (lMissingSamples == 0u)
      && (Dem_Cfg_EventSupportImmediateNv(EventId) == TRUE) )
    {
      Dem_Nvm_SetTimeSeriesMemoryBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
    else
    {
      Dem_Nvm_SetTimeSeriesMemoryBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_ProcessNewFutureSampleAvailable(
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
Dem_Memories_TimeSeriesMemory_ProcessNewFutureSampleAvailable(
  void
)
{
  Dem_Cfg_EntryIterType lTimerSeriesEntryIter;

  for (Dem_Cfg_ComplexIter32BitInit(&lTimerSeriesEntryIter, 0u, Dem_Memories_TimeSeriesMemory_GetSizeOfTimeSeriesEntryTable());  /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ComplexIter32BitExists(&lTimerSeriesEntryIter) == TRUE;                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ComplexIter32BitNext(&lTimerSeriesEntryIter))                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lTimerSeriesEntryIter);                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
    if (Dem_MemoryEntry_TimeSeriesEntry_TestEntryAvailable(lTimeSeriesEntryIndex) == TRUE)
    {
      Dem_TimeSeriesEntry_PtrToConstBaseType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(lTimeSeriesEntryIndex);
      if(lEntry->EventId != DEM_EVENT_INVALID)
      {
        Dem_TimeSeriesIdType lSeriesId;
        Dem_TimeSeriesIdType lNumberOfSeries = Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(lEntry->EventId);
        for(lSeriesId = 0; lSeriesId < lNumberOfSeries; lSeriesId++)
        {
          Dem_Cfg_SamplingProfileIterType lSamplingProfileId = Dem_Memories_TimeSeriesMemory_GetSamplingProfileId(lEntry->EventId, lSeriesId);
          if(Dem_GlobalDiagnostics_TestNewRingBufferSampleAvailable(lSamplingProfileId))
          {
            Dem_Memories_TimeSeriesMemory_StoreFutureSamples(lTimeSeriesEntryIndex, lSeriesId);
          }
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_StoreFutureSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_StoreFutureSamples(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId
)
{
#if (DEM_CFG_SUPPORT_TIME_SERIES_FUTURE == STD_ON)
  Dem_Cfg_TssFifoEntry_IterType lIter;
  boolean lTriggerNvmStorage = FALSE;
  
  Dem_Cfg_TimeSeriesEntryBasePtrType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);
  Dem_TimeSeriesEntry_CompanionPtrType lEntryCompanion = Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(TimeSeriesEntryIndex);
  Dem_Cfg_SamplingProfileIterType lSamplingProfileId = Dem_Memories_TimeSeriesMemory_GetSamplingProfileId(lEntry->EventId, SeriesId);
  
  for (Dem_Memories_TimeSeriesMemory_TssSetIter_Init(TimeSeriesEntryIndex, SeriesId, &lIter);                                    /* SBSW_DEM_POINTER_FORWARD_STACK */
      Dem_Memories_TimeSeriesMemory_TssSetIter_Exists(&lIter);                                                                   /* SBSW_DEM_POINTER_FORWARD_STACK */
      Dem_Memories_TimeSeriesMemory_TssSetIter_Next(&lIter))                                                                     /* SBSW_DEM_POINTER_FORWARD_STACK */
  {
    uint8 lSetId = Dem_Memories_TimeSeriesMemory_TssSetIter_GetTssSetId(&lIter);                                                 /* SBSW_DEM_POINTER_FORWARD_STACK */
    uint8 lMissingSamples = Dem_MemoryEntry_TimeSeriesEntry_GetMissingFutureSamples(lEntryCompanion, SeriesId, lSetId);          /* SBSW_DEM_POINTER_READ_TIMESERIESENTRYCOMPANION */

    if (lMissingSamples > 0u)
    {
      uint8 lSourceSample;
      uint8 lTargetSample;
      Dem_Data_DestinationBuffer lSnapshotDataBuffer;

      lTargetSample = Dem_Memories_TimeSeriesMemory_GetNumberOfSamplesOfSeries(lEntry->EventId, SeriesId) - lMissingSamples;
      lSourceSample = Dem_GlobalDiagnostics_GetMostRecentRingBufferSampleId(lSamplingProfileId);

      Dem_MemoryEntry_TimeSeriesEntry_InitSnapshotDataBuffer(
        TimeSeriesEntryIndex,
        SeriesId,
        lSetId,
        lTargetSample,
        &lSnapshotDataBuffer);                                                                                                   /* SBSW_DEM_POINTER_FORWARD_STACK */

      Dem_Memories_TimeSeriesMemory_CopyDidsFromRingBuffer(                                                                      /* SBSW_DEM_POINTER_FORWARD_STACK */
        lEntry->EventId,
        lSamplingProfileId,
        lSourceSample,
        &lSnapshotDataBuffer
          );
      --lMissingSamples;
      Dem_MemoryEntry_TimeSeriesEntry_SetUnusedFutureSamples(lEntry, SeriesId, lSetId, lMissingSamples);                         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      Dem_MemoryEntry_TimeSeriesEntry_SetMissingFutureSamples(lEntryCompanion, SeriesId, lSetId, lMissingSamples);               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

      if (lTriggerNvmStorage == FALSE)
      {
        lTriggerNvmStorage = TRUE;
      }
    }
  }

  if (lTriggerNvmStorage == TRUE)
  {
    Dem_Memories_TimeSeriesMemory_TriggerNvMStorageOnFutureSampleUpdate(TimeSeriesEntryIndex, lEntryCompanion, lEntry->EventId); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TimeSeriesEntryIndex)
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SeriesId)
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TimeSeriesMemoriesIterInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TimeSeriesMemoriesIterInit(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_Cfg_TSSRTranslatedMemoryIdIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  IterPtr->mIdx = Dem_Memories_TimeSeriesMemory_GetFirstTranslatedMemoryId();                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Memories_TimeSeriesMemory_GetFirstTranslatedMemoryId()                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
                     + Dem_Memories_TimeSeriesMemory_GetNumberOfEventMemorySetsWithTimeSeries();  
}

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
 * \addtogroup Dem_TimeSeriesMemory_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_InvalidateEntryOfEvent
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_InvalidateEntryOfEvent(
  Dem_EventIdType EventId,
  uint8  BlockStatus
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  {
    Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = Dem_Memories_TimeSeriesMemory_FindEntryIndex(EventId);
    if (lTimeSeriesEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
    {
      Dem_TimeSeriesEntry_CompanionPtrType lEntryCompanion 
        = Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(lTimeSeriesEntryIndex);

      /* request to remove the time series entry */
      if (Dem_MemoryEntry_TimeSeriesEntry_RequestDeletion(lEntryCompanion) == TRUE)                                              /* SBSW_DEM_POINTER_WRITE_TIMESERIESENTRYCOMPANION */
      {
        Dem_Cfg_NvBlockIndexType lNvBlockIndex = Dem_MemoryEntry_TimeSeriesEntry_GetEntryIndexToNvBlockIndex(lTimeSeriesEntryIndex);
        /* remove the time series entry immediately */
        Dem_FaultMemory_Memories_RemoveEntryAndChrono(Dem_Memories_TimeSeriesMemory_GetMemoryId(EventId), lTimeSeriesEntryIndex);
        Dem_Nvm_SetTimeSeriesMemoryBlockState(lNvBlockIndex, BlockStatus);
      }
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_ARGUMENT(BlockStatus)                                                                                        /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_Init(
  void
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Scheduler_EnableTask(Dem_Scheduler_Task_TimeSeries);
  Dem_Memories_TimeSeriesMemory_RequestCleanUp = FALSE;
#endif
  Dem_Memories_TimeSeriesMemory_InitCompanionEntries();
}




/* ****************************************************************************
% Dem_Memories_TimeSeriesMemory_IsTimeSeriesPowertrainStoredByEvent
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
Dem_Memories_TimeSeriesMemory_IsTimeSeriesPowertrainStoredByEvent(                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
)
{
  boolean lReturn = FALSE;

  if ((Dem_Memories_TimeSeriesMemory_IsPowertrainEnabled() == TRUE)
    && (Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries(EventId) == TRUE)
    && (Dem_Memories_TimeSeriesMemory_FindEntryIndex(EventId) != DEM_CFG_ENTRYINDEX_INVALID))                                    /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
  {
    lReturn = TRUE;
  }

  return lReturn;
}


/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_InitNvData
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
Dem_Memories_TimeSeriesMemory_InitNvData(
  void
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Cfg_TSSRTranslatedMemoryIdIterType lTimeSeriesMemoryIter;

    for (Dem_Memories_TimeSeriesMemory_TimeSeriesMemoriesIterInit(&lTimeSeriesMemoryIter);                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_ComplexIterExists(&lTimeSeriesMemoryIter) == TRUE;                                                              /* PRQA S 2994 */ /* MD_DEM_2994 */  /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_ComplexIterNext(&lTimeSeriesMemoryIter))                                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_Cfg_EntryIterType lTimeSeriesEntryIter;
      uint16 lTranslatedMemoryId = Dem_Cfg_ComplexIterGet(&lTimeSeriesMemoryIter);                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */

      Dem_FaultMemory_Memories_SetCurrentSize(lTranslatedMemoryId, 0);

      for (Dem_Cfg_ComplexIter32BitInit(&lTimeSeriesEntryIter,
                                        Dem_FaultMemory_Memories_GetFirstEntryIndex(lTranslatedMemoryId),
                                        Dem_FaultMemory_Memories_GetEndEntryIndex(lTranslatedMemoryId));                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
           Dem_Cfg_ComplexIter32BitExists(&lTimeSeriesEntryIter) == TRUE;                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
           Dem_Cfg_ComplexIter32BitNext(&lTimeSeriesEntryIter))                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lTimeSeriesEntryIter);                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_TimeSeriesEntryBasePtrType lBaseEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(lTimeSeriesEntryIndex);
        Dem_MemSet((Dem_SharedDataPtrType)lBaseEntry, 0u, sizeof(*lBaseEntry));                                                  /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_DEM_POINTER_MEMSET_TIMESERIES_BASEENTRY */
      }
    }
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_RestoreOnInit
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
Dem_Memories_TimeSeriesMemory_RestoreOnInit(
  void
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Cfg_TSSRTranslatedMemoryIdIterType lTimeSeriesMemoryIter;

  for (Dem_Memories_TimeSeriesMemory_TimeSeriesMemoriesIterInit(&lTimeSeriesMemoryIter);                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ComplexIterExists(&lTimeSeriesMemoryIter) == TRUE;                                                                /* PRQA S 2994 */ /* MD_DEM_2994 */  /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ComplexIterNext(&lTimeSeriesMemoryIter))                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_EntryIterType lTimeSeriesEntryIter;
    uint16 lTranslatedMemoryId = Dem_Cfg_ComplexIterGet(&lTimeSeriesMemoryIter);                                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */

    Dem_FaultMemory_Memories_SetCurrentSize(lTranslatedMemoryId, 0);

    for (Dem_Cfg_ComplexIter32BitInit(&lTimeSeriesEntryIter,
                                      Dem_FaultMemory_Memories_GetFirstEntryIndex(lTranslatedMemoryId),
                                      Dem_FaultMemory_Memories_GetEndEntryIndex(lTranslatedMemoryId));                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_ComplexIter32BitExists(&lTimeSeriesEntryIter) == TRUE;                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_ComplexIter32BitNext(&lTimeSeriesEntryIter))                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lTimeSeriesEntryIter);                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_EventIdType lEventId;

      lEventId = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(lTimeSeriesEntryIndex)->EventId;
      if (lEventId != DEM_EVENT_INVALID)
      {
        uint32 lTimestamp;
        uint8 lSortedIndex;
        Dem_Cfg_EntryIndexType lOtherEntryIndex;
        Dem_Cfg_NvBlockIndexType lOtherNvBlockIndex;
        Dem_Cfg_NvBlockIndexType lNvBlockIndex = Dem_MemoryEntry_TimeSeriesEntry_GetEntryIndexToNvBlockIndex(lTimeSeriesEntryIndex);

        if ((lEventId >= Dem_Cfg_GlobalEventCount())
          || (Dem_Memories_TimeSeriesMemory_GetMemoryIdOrInvalid(lEventId) != lTranslatedMemoryId)
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
          || (lEventId != Dem_Cfg_GetMasterEvent(lEventId))
# endif
          )
        {
          /* The time series entry is not allocated to the master event. Remove it. */
          Dem_FaultMemory_Memories_FreeEntry(lTranslatedMemoryId, lTimeSeriesEntryIndex);
          Dem_Nvm_SetTimeSeriesMemoryBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);

          /* DET: To get this inconsistency, it requires a configuration change without clearing the NV contents */
          Dem_Error_ReportError(DEM_INIT_APIID, DEM_E_INCONSISTENT_STATE);
          continue;
        }

        /* find duplicate of time series entry */
        lOtherEntryIndex = Dem_Memories_TimeSeriesMemory_FindEntryIndex(lEventId);
        lOtherNvBlockIndex = Dem_MemoryEntry_TimeSeriesEntry_GetEntryIndexToNvBlockIndex(lOtherEntryIndex);
        lTimestamp = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(lTimeSeriesEntryIndex)->Timestamp;

        if (lOtherEntryIndex != lTimeSeriesEntryIndex)
        {
          /* Clear the older entry */
          if (lTimestamp > Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(lOtherEntryIndex)->Timestamp)
          {
            Dem_FaultMemory_Memories_RemoveEntryAndChrono(lTranslatedMemoryId, lOtherEntryIndex);
            Dem_Nvm_SetTimeSeriesMemoryBlockState(lOtherNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
          }
          else
          {
            Dem_FaultMemory_Memories_FreeEntry(lTranslatedMemoryId, lTimeSeriesEntryIndex);
            Dem_Nvm_SetTimeSeriesMemoryBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
            continue;
          }
        }

        /* Check whether the entry is missing a primary entry or has inconsistent UDS status bits */
        if (Dem_Memories_TimeSeriesMemory_HasNoValidDataAfterResettingInconsistentSeries(lEventId) == TRUE)
        {
          Dem_FaultMemory_Memories_FreeEntry(lTranslatedMemoryId, lTimeSeriesEntryIndex);
          Dem_Nvm_SetTimeSeriesMemoryBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
          continue;
        }

        {
          /* reset the readout status and client counter for virtual buffer */
          Dem_TimeSeriesEntry_CompanionPtrType lEntryCompanion = Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(lTimeSeriesEntryIndex);
          Dem_MemoryEntry_TimeSeriesEntry_SetReadoutState(lEntryCompanion,
            DEM_TIMESERIESENTRY_READOUTSTATUS_UNLOCKED);                                                                         /* SBSW_DEM_POINTER_WRITE_TIMESERIESENTRYCOMPANION */
          }

        /* Synchronize the global timestamp with the timestamp stored in the time series entry */
        if (lTimestamp > Dem_FaultMemory_Memories_GetCurrentTimestamp())
        {
          Dem_FaultMemory_Memories_SetCurrentTimestamp(lTimestamp);
        }

        /* Sort the time series entry into the chronological list */
        lSortedIndex = Dem_FaultMemory_Memories_GetCurrentSize(lTranslatedMemoryId);
        while ((lSortedIndex > 0u)
          && (Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(Dem_FaultMemory_Memories_GetChronology(lTranslatedMemoryId, lSortedIndex - 1u))->Timestamp > lTimestamp)) /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        {
          Dem_FaultMemory_Memories_SetChronology(lTranslatedMemoryId,
            lSortedIndex,
            Dem_FaultMemory_Memories_GetChronology(lTranslatedMemoryId, lSortedIndex - 1u));
          --lSortedIndex;
        }

        Dem_FaultMemory_Memories_SetChronology(lTranslatedMemoryId, lSortedIndex, lTimeSeriesEntryIndex);

        /* Update the number of occupied entries */
        Dem_FaultMemory_Memories_SetCurrentSize(lTranslatedMemoryId,
          (uint8)(Dem_FaultMemory_Memories_GetCurrentSize(lTranslatedMemoryId) + 1u));
      }
    }
    /* Update the global timestamp */
    Dem_FaultMemory_Memories_SetCurrentTimestamp(Dem_FaultMemory_Memories_GetCurrentTimestamp() + 1u);
  }
#endif
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */



/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_StoreEntry
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
Dem_Memories_TimeSeriesMemory_StoreEntry(
  Dem_EventIdType EventId,
  P2CONST(Dem_BitField256Bits, AUTOMATIC, AUTOMATIC) SeriesTriggeredFlags
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  /* Check if there is already a time series entry for this DTC */
  Dem_EventIdType lMemoryRepresentativeEventId = Dem_Cfg_GetMemoryRepresentative(EventId);
  Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = Dem_Memories_TimeSeriesMemory_FindEntryIndex(lMemoryRepresentativeEventId);

  if (lTimeSeriesEntryIndex == DEM_CFG_ENTRYINDEX_INVALID)
  {
    /* No entry exists - try to create one */
    lTimeSeriesEntryIndex = Dem_Memories_TimeSeriesMemory_AllocateEntry(EventId);
  }
  else if (Dem_MemoryEntry_TimeSeriesEntry_TestEntryUpdatable(EventId, lTimeSeriesEntryIndex))
  {
    /* Entry already exists and updates are supported - determine if we shall update samples */
  }
  else
  {
    /* Entry already exists and updates not supported */
    lTimeSeriesEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
  }

  /* Proceed storing samples if event entry exists */
  if (lTimeSeriesEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    Dem_Cfg_NvBlockIndexType lNvBlockIndex = Dem_MemoryEntry_TimeSeriesEntry_GetEntryIndexToNvBlockIndex(lTimeSeriesEntryIndex);
    Dem_Memories_TimeSeriesMemory_StorePastSamples(lTimeSeriesEntryIndex, SeriesTriggeredFlags);                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

    /* Update NV block state */
#if (DEM_CFG_SUPPORT_TIME_SERIES_FUTURE == STD_OFF)
    if (Dem_Cfg_EventSupportImmediateNv(EventId) == TRUE)
    {
      Dem_Nvm_SetTimeSeriesMemoryBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
    else
#endif
    {
      Dem_Nvm_SetTimeSeriesMemoryBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
#else
  DEM_IGNORE_UNUSED_ARGUMENT(EventId);                                                                                           /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(SeriesTriggeredFlags);                                                                              /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
}                                                                                                                                /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_ReleaseEntry
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_ReleaseEntry(
Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
)
{
  Dem_TimeSeriesEntry_CompanionPtrType lEntryCompanion
    = Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(TimeSeriesEntryIndex);

  if (Dem_MemoryEntry_TimeSeriesEntry_ReleaseLock(lEntryCompanion) == TRUE)                                                      /* SBSW_DEM_POINTER_WRITE_TIMESERIESENTRYCOMPANION */
  {
    Dem_Memories_TimeSeriesMemory_RequestCleanUp = TRUE;
  }
}
#endif


/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_MainFunction
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
Dem_Memories_TimeSeriesMemory_MainFunction(
  void
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  if (Dem_GlobalDiagnostics_TestAnyNewRingBufferSampleAvailable() == TRUE)
  {
    Dem_Memories_TimeSeriesMemory_ProcessNewFutureSampleAvailable();
  }

  if (Dem_Memories_TimeSeriesMemory_RequestCleanUp == TRUE)
  {
    Dem_Memories_TimeSeriesMemory_RequestCleanUp = FALSE;
    Dem_Memories_TimeSeriesMemory_ProcessCleanUp();
  }
#endif
}

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TestSRecIsStored
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
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TestSRecIsStored(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  CONSTP2CONST(Dem_TimeSeriesRecordIterator, AUTOMATIC, AUTOMATIC) Iter
  )
{
  boolean lIsStored = FALSE;

  if((Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorIsInvalid(Iter) == FALSE) && (TimeSeriesEntryIndex < Dem_Cfg_GetSizeOfTimeSeriesEntryTable())) /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_TimeSeriesEntry_PtrToConstBaseType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);
    uint8 lSetId = Dem_Memories_TimeSeriesMemory_GetSetIdOfChronologicalId(TimeSeriesEntryIndex, Iter->SeriesId, Iter->ChronologicalId);
    uint8 lSnapshotPositionInSet = Dem_Memories_TimeSeriesMemory_GetSnapshotOffsetFromFirstOneInSet(Iter->EventId, Iter->SeriesId, Iter->RecordNumber);

    uint8 lFirstStoredSampleInSet = Dem_MemoryEntry_TimeSeriesEntry_GetUnusedPastSamples(lEntry, Iter->SeriesId, lSetId);        /* SBSW_DEM_POINTER_READ_TIMESERIES_BASEENTRY */
    uint8 lLastStoredSampleInSet  = Dem_Memories_TimeSeriesMemory_GetNumberOfSamplesOfSeries(Iter->EventId, Iter->SeriesId)
     - Dem_MemoryEntry_TimeSeriesEntry_GetUnusedFutureSamples(lEntry, Iter->SeriesId, lSetId);                                   /* SBSW_DEM_POINTER_READ_TIMESERIES_BASEENTRY */

    lIsStored = (boolean)((lFirstStoredSampleInSet <= lSnapshotPositionInSet) && (lSnapshotPositionInSet < lLastStoredSampleInSet));
  }

  return lIsStored;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_FindEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Memories_TimeSeriesMemory_FindEntryIndex(
  Dem_EventIdType EventId
  )
{
  uint16 lTimeSeriesTranslatedMemoryId = Dem_Memories_TimeSeriesMemory_GetMemoryIdOrInvalid(EventId);
  return Dem_FaultMemory_Memories_FindEntryIndexOfEventGeneric(EventId, FALSE, lTimeSeriesTranslatedMemoryId);
}

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetTimeSeriesPowertrainSequentialInfo
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
Dem_Memories_TimeSeriesMemory_GetTimeSeriesPowertrainSequentialInfo(
  Dem_Cfg_EntryIndexType  TimeSeriesEntryIndex,
  Dem_EventIdType EventId,
  CONSTP2VAR(Dem_Memory_TimeSeries_PowertrainTimeSeriesInfoType, AUTOMATIC, AUTOMATIC) TimeSeriesInfo
)
{

  Dem_TimeSeriesEntry_PtrToConstBaseType lSourceEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);
  Dem_Cfg_SamplingProfileIterType lSamplingProfileId = Dem_Memories_TimeSeriesMemory_GetSamplingProfileId(EventId, 0);
  TimeSeriesInfo->NumberOfConfiguredSamples = Dem_Cfg_GetPastSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId);        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  TimeSeriesInfo->NumberOfNotStoredSamples = Dem_MemoryEntry_TimeSeriesEntry_GetUnusedPastSamples(lSourceEntry, 0, 0u);          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */ /* SBSW_DEM_POINTER_READ_TIMESERIES_BASEENTRY */

  /* Disable requirement to support float when time series powertrain is disabled */
  TimeSeriesInfo->SamplingRate = Dem_GlobalDiagnostics_GetSamplingTime(lSamplingProfileId);                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  TimeSeriesInfo->StartingSequentialDIDIdentifier = 
  Dem_Memories_TimeSeriesMemory_GetStartingSequentialDIDIdentifier(lSamplingProfileId);                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif


/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetNextOrLastSeriesByRecordNumber
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_TimeSeriesIdType, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetNextOrLastSeriesByRecordNumber(                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId,
  uint8 RecordNumber
)
{ /* Get series of the index of the series with the lowest first record number that the iterator has not already passed by. */
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  boolean lMatchFound = FALSE;
  Dem_TimeSeriesIdType lSeriesId = 0;
  Dem_TimeSeriesIdType lSeriesIdMax = 0;
  uint8 lFirstRecordNumber = 255;
  uint8 lMaxLastRecordNumber = 0;

  Dem_TimeSeriesIdType lSeriesIdIter;
  Dem_TimeSeriesIdType lNumberOfSeries = Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(EventId);
  for(lSeriesIdIter = 0; lSeriesIdIter < lNumberOfSeries; lSeriesIdIter++)
  {
    uint8 lFirstRecordNumberTmp = Dem_Cfg_GetFirstRecordNumberOfTimeSeriesFreezeFrameTable(Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(EventId, lSeriesIdIter));
    uint8 lLastRecordNumberOfLastSet = Dem_Memories_TimeSeriesMemory_GetLastRecordNumberOfSeries(EventId, lSeriesIdIter);
    if((lFirstRecordNumber >= lFirstRecordNumberTmp) && (RecordNumber <= lLastRecordNumberOfLastSet))
    {
      lFirstRecordNumber = lFirstRecordNumberTmp;
      lSeriesId = lSeriesIdIter;
      lMatchFound = TRUE;
    }
    if(lMaxLastRecordNumber < lLastRecordNumberOfLastSet)
    {
      lMaxLastRecordNumber = lLastRecordNumberOfLastSet;
      lSeriesIdMax = lSeriesIdIter;
    }
  }
  if(lMatchFound == FALSE)
  { /* In case no matching series was found, return id of series with highest record number. */
    lSeriesId = lSeriesIdMax;
  }
  return lSeriesId;
#else
  DEM_IGNORE_UNUSED_ARGUMENT(EventId);                                                                                           /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(RecordNumber);                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  
  return 0;
#endif
}



/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_InitializeTimeSeriesRecordIteratorBySeriesId()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_TimeSeriesRecordIterator, DEM_CODE)
Dem_Memories_TimeSeriesMemory_InitializeTimeSeriesRecordIteratorBySeriesId(                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId,
  Dem_TimeSeriesIdType SeriesId
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_TimeSeriesRecordIterator lIter = { 0 };
  if(Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries(EventId))
  {
    Dem_TimeSeriesIdType lStartId = Dem_Cfg_GetTimeSeriesIndirectionTableStartIdxOfEventTable(EventId);
    uint8 lTimeSeriesTableId = Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(EventId, SeriesId);
    DEM_IGNORE_UNUSED_VARIABLE(lTimeSeriesTableId);                                                                              /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 

    lIter.EventId = EventId;
    lIter.StartId = lStartId;
    lIter.NumberOfSeries = Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(EventId);
    lIter.SeriesId = SeriesId;
    lIter.ChronologicalId = 0;
    lIter.RecordNumber = Dem_Cfg_GetFirstRecordNumberOfTimeSeriesFreezeFrameTable(lTimeSeriesTableId);
    lIter.FirstRecordNumber = Dem_Cfg_GetFirstRecordNumberOfTimeSeriesFreezeFrameTable(lTimeSeriesTableId);
    lIter.LastRecordNumber = Dem_Cfg_GetLastRecordNumberOfTimeSeriesFreezeFrameTable(lTimeSeriesTableId);
  }
#else
  const Dem_TimeSeriesRecordIterator lIter = { 0 };
  DEM_IGNORE_UNUSED_ARGUMENT(EventId);                                                                                           /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(SeriesId);                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
  return lIter;
}
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_InitializeTimeSeriesRecordIteratorByRecordNumber()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_TimeSeriesRecordIterator, DEM_CODE)
Dem_Memories_TimeSeriesMemory_InitializeTimeSeriesRecordIteratorByRecordNumber(Dem_EventIdType EventId, uint8 RecordNumber)      /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
{
  Dem_TimeSeriesRecordIterator lIter = { 0 };
  lIter.FirstRecordNumber = 1; /* Range empty if not supported */
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  if(Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries(EventId))
  {
    Dem_TimeSeriesIdType lStartId = Dem_Cfg_GetTimeSeriesIndirectionTableStartIdxOfEventTable(EventId);
    Dem_TimeSeriesIdType lSeriesId = Dem_Memories_TimeSeriesMemory_GetNextOrLastSeriesByRecordNumber(EventId, RecordNumber);
    Dem_TimeSeriesIdType lNumberOfSeries = Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(EventId);
    uint8 lTimeSeriesTableId = Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(EventId, lSeriesId);

    uint16 lTranslatedMemoryId = Dem_Memories_TimeSeriesMemory_GetMemoryId(EventId);
    uint8 lRecNumOffset = Dem_Cfg_GetTimeSeriesSetsRecNumOffsetOfMemoryInfoTable(lTranslatedMemoryId);
    uint8 lChronologicalId = Dem_Memories_TimeSeriesMemory_GetChronologicalIdOfRecordNumber(EventId, lSeriesId, RecordNumber);
    DEM_IGNORE_UNUSED_VARIABLE(lTimeSeriesTableId);                                                                              /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 

    lIter.EventId = EventId;
    lIter.StartId = lStartId;
    lIter.NumberOfSeries = lNumberOfSeries;
    lIter.SeriesId = lSeriesId;
    lIter.ChronologicalId = lChronologicalId;
    lIter.RecordNumber = RecordNumber;
    lIter.FirstRecordNumber = Dem_Cfg_GetFirstRecordNumberOfTimeSeriesFreezeFrameTable(lTimeSeriesTableId) + (lChronologicalId * lRecNumOffset);
    lIter.LastRecordNumber = Dem_Cfg_GetLastRecordNumberOfTimeSeriesFreezeFrameTable(lTimeSeriesTableId) + (lChronologicalId * lRecNumOffset);
  }
#else
  DEM_IGNORE_UNUSED_ARGUMENT(EventId);                                                                                           /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_ARGUMENT(RecordNumber);                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
  return lIter;
}

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextSeriesByRecordNumber()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextSeriesByRecordNumber(                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_TimeSeriesRecordIterator, AUTOMATIC, AUTOMATIC) Iter                                                            /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  if(!Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorIsInvalid(Iter))                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    uint8 lRecordNumberAfterCurrentSeries = Dem_Memories_TimeSeriesMemory_GetLastRecordNumberOfSeries(Iter->EventId, Iter->SeriesId) + 1u;
    Dem_TimeSeriesIdType lNextSeriesId = Dem_Memories_TimeSeriesMemory_GetNextOrLastSeriesByRecordNumber(Iter->EventId, lRecordNumberAfterCurrentSeries);
    if(lNextSeriesId < Iter->NumberOfSeries)
    {
      uint8 lTimeSeriesTableId = Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(Iter->EventId, lNextSeriesId);
      uint8 lNextSeriesLastRecordNumber = Dem_Cfg_GetLastRecordNumberOfTimeSeriesFreezeFrameTable(lTimeSeriesTableId);
      DEM_IGNORE_UNUSED_VARIABLE(lTimeSeriesTableId);                                                                            /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 

      if(lNextSeriesLastRecordNumber > Iter->LastRecordNumber) 
      {
        Iter->SeriesId = lNextSeriesId;                                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        Iter->ChronologicalId = 0;                                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        Iter->RecordNumber = Dem_Cfg_GetFirstRecordNumberOfTimeSeriesFreezeFrameTable(lTimeSeriesTableId);                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        Iter->FirstRecordNumber = Dem_Cfg_GetFirstRecordNumberOfTimeSeriesFreezeFrameTable(lTimeSeriesTableId);                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        Iter->LastRecordNumber = lNextSeriesLastRecordNumber;                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Iter);                                                                                        /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
}

# if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextSet()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextSet(
  CONSTP2VAR(Dem_TimeSeriesRecordIterator, AUTOMATIC, AUTOMATIC) Iter
)
{
  if((Iter->ChronologicalId + 1u) < Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(Iter->EventId))
  {
    uint16 lTranslatedMemoryId = Dem_Memories_TimeSeriesMemory_GetMemoryId(Iter->EventId);
    uint8 lRecNumOffset = Dem_Cfg_GetTimeSeriesSetsRecNumOffsetOfMemoryInfoTable(lTranslatedMemoryId);

    Iter->RecordNumber = Iter->FirstRecordNumber + lRecNumOffset;                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    Iter->FirstRecordNumber += lRecNumOffset;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    Iter->LastRecordNumber += lRecNumOffset;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    Iter->ChronologicalId++;                                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}
#endif

# if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextRecordNumberInAnySeriesOfEvent()
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
Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextRecordNumberInAnySeriesOfEvent(                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_TimeSeriesRecordIterator, AUTOMATIC, AUTOMATIC) Iter
)
{
  if((Iter->RecordNumber + 1u) <= Iter->LastRecordNumber)
  {
    if(Iter->RecordNumber < Iter->FirstRecordNumber)
    {
      Iter->RecordNumber = Iter->FirstRecordNumber;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else 
    {
      Iter->RecordNumber++;                                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
  else if((Iter->ChronologicalId + 1u) < Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(Iter->EventId))
  {
    Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextSet(Iter);                                                         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  else
  {
    Dem_TimeSeriesIdType lSeriesId = Iter->SeriesId;
    Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextSeriesByRecordNumber(Iter);                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    if(lSeriesId == Iter->SeriesId)
    { /* Set values to indicate that we are past the end of the valid elements. */
      Iter->RecordNumber = Iter->LastRecordNumber;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      Iter->ChronologicalId = Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(Iter->EventId);                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      Iter->SeriesId = Iter->NumberOfSeries;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
}
#endif

/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorIsInvalid()
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE) Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorIsInvalid(                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2CONST(Dem_TimeSeriesRecordIterator, AUTOMATIC, AUTOMATIC) Iter
)
{
  boolean lIsInvalid = ((Iter->EventId == 0u) || (Iter->SeriesId >= Iter->NumberOfSeries));
#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON))
  if((lIsInvalid == FALSE) && (Dem_Memories_TimeSeriesMemory_TestRecordNumberIsInSeries(Iter->EventId, Iter->SeriesId, Iter->RecordNumber) == FALSE))
  {
    lIsInvalid = (Iter->ChronologicalId >= Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(Iter->EventId));
    if(lIsInvalid)
    { /* Are there later record numbers belonging to time series? */
      uint8 lRecordNumberAfterCurrentSeries = Dem_Memories_TimeSeriesMemory_GetLastRecordNumberOfSeries(Iter->EventId, Iter->SeriesId) + 1u;
      Dem_TimeSeriesIdType lNextSeriesId = Dem_Memories_TimeSeriesMemory_GetNextOrLastSeriesByRecordNumber(Iter->EventId, lRecordNumberAfterCurrentSeries);
      lIsInvalid = (lNextSeriesId == Iter->SeriesId);
    }
  }
#endif

  return lIsInvalid;
}

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetSamplingProfileId()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetSamplingProfileId(Dem_EventIdType EventId, Dem_TimeSeriesIdType SeriesId)
{
  Dem_TimeSeriesIdType lTableId = Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(EventId, SeriesId);

  uint8 lSamplingProfileId = Dem_Cfg_GetTimeSeriesSamplingProfileTableIdxOfTimeSeriesFreezeFrameTable(lTableId);
  DEM_IGNORE_UNUSED_VARIABLE(lTableId);                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  Dem_Internal_AssertContinue(lSamplingProfileId < Dem_Cfg_GetSizeOfTimeSeriesSamplingProfileTable(), DEM_E_INCONSISTENT_STATE);
  return lSamplingProfileId;
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId(Dem_EventIdType EventId, Dem_TimeSeriesIdType SeriesId)
{
  Dem_TimeSeriesIdType lStartId = Dem_Cfg_GetTimeSeriesIndirectionTableStartIdxOfEventTable(EventId);
  DEM_IGNORE_UNUSED_VARIABLE(EventId);                                                                                           /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_VARIABLE(SeriesId);                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_VARIABLE(lStartId);                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 

  Dem_Internal_AssertContinue(SeriesId < Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(EventId), DEM_E_INCONSISTENT_STATE)
  return Dem_Cfg_GetTimeSeriesFreezeFrameTableIdxOfTimeSeriesIndirectionTable(lStartId + SeriesId);
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_GetTimeSeriesTableId()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(Dem_EventIdType EventId)
{
  return Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries(EventId) ?
    (Dem_Cfg_GetTimeSeriesIndirectionTableEndIdxOfEventTable(EventId) - Dem_Cfg_GetTimeSeriesIndirectionTableStartIdxOfEventTable(EventId)) : 0u;
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_Memories_TimeSeriesMemory_CopyDidsFromRingBuffer
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
Dem_Memories_TimeSeriesMemory_CopyDidsFromRingBuffer(
  Dem_EventIdType EventId,
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  uint8 SourceIndex,
  Dem_Data_DestinationBufferPtrType  DestinationBuffer
  )
{
  if (Dem_Memories_TimeSeriesMemory_TestSamplingProfileExists() == TRUE)                                                         /* COV_DEM_ROBUSTNESS TX */
  {
    Dem_Cfg_EventDidIterType lDidIter;
    Dem_ConstSharedDataPtrType lSourceBuffer;

    lSourceBuffer = Dem_GlobalDiagnostics_ReadRingBufferSlotChronological(SamplingProfileId, SourceIndex);
    for (Dem_Memories_TimeSeriesMemory_EventDidIter_Init(EventId, &lDidIter);                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Memories_TimeSeriesMemory_EventDidIter_Exists(&lDidIter) == TRUE;                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Memories_TimeSeriesMemory_EventDidIter_Next(&lDidIter))                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      uint16 lDidSize;
      Dem_Cfg_DidIndexType lDidIndex = Dem_Memories_TimeSeriesMemory_EventDidIter_Get(&lDidIter);                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      uint16 SamplingProfileDidInfoTableId = Dem_StorageManager_DataStorage_GetSamplingProfileDidInfoTableId(SamplingProfileId, lDidIndex);

      lDidSize = Dem_StorageManager_DataStorage_GetDidNVSize(lDidIndex);

      if (Dem_Data_CheckAvailableSizeDestinationBuffer(DestinationBuffer, lDidSize) == E_OK)                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        Dem_SharedDataPtrType lDestinationBufferWritePtr = Dem_Data_GetPtrToWriteIndexOfDestinationBuffer(DestinationBuffer);    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

        Dem_MemCpy(lDestinationBufferWritePtr,                                                                                   /* PRQA S 0311, 0315, 0602, 2843, 2842 */ /* MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2, MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation */  /* SBSW_DEM_WRITE_DESTINATIONBUFFER */
          (Dem_DataPtrType)&lSourceBuffer[Dem_Cfg_GetDidRingBufferOffsetOfSamplingProfileDidInfoTable(SamplingProfileDidInfoTableId)], 
          lDidSize);
        Dem_Data_IncrementWriteIndexDestinationBuffer(DestinationBuffer, lDidSize);                                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_TIMESERIESMEMORY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_TimeSeriesMemory_Implementation.h
 *********************************************************************************************************************/
