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
 *  \defgroup   Dem_TimeSeriesEntry MemPermanent
 *  \{
 *  \file       Dem_TimeSeriesEntry_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Interface of the Time Series Entry unit which handles access to and from time series entries.
 *  \entity     TimeSeriesEntry
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_TIMESERIESENTRY_INTERFACE_H)
#define DEM_TIMESERIESENTRY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_TimeSeriesEntry_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_TimeSeriesEntry_PublicProperties Public Properties
 * \{
 */


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimerSeriesEntry_TestDisplaceable
 *****************************************************************************/
/*!
 * \brief         Test whether the time series entry can be displaced
 *
 * \details       -
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \return        TRUE
 *                The entry can be displaced
 * \return        FALSE
 *                The entry can not be displaced
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimerSeriesEntry_TestDisplaceable(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_SetUnusedFutureSamples
 *****************************************************************************/
/*!
 * \brief         Sets number of unused future samples in time series entry
 *
 * \details       Sets number of unused future samples in time series entry if available
 *
 * \param[in]     BaseEntry
 *                Pointer to the time series entry
 * \param[in]     SeriesId
 *                Specifies a time series
 * \param[in]     SetId
 *                Index of TSS set in time series entry
 *                [0 to DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS - 1]
 * \param[in]     UnusedFutureSamples
 *                Number of unused future samples
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_SetUnusedFutureSamples( 
  Dem_TimeSeriesEntry_ConstPtrToBaseType BaseEntry,                                                                              /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  Dem_TimeSeriesIdType SeriesId,
  uint16 SetId,
  uint8 UnusedFutureSamples
);
#endif

/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_GetMissingFutureSamples
 *****************************************************************************/
/*!
 * \brief         Get the missing future samples of the indexed time series
 *                snapshot record.
 *
 * \details       Get the missing future samples of the indexed time series
 *                snapshot record.
 *
 * \param[in]     EntryCompanion
 *                Pointer to the time series entry companion
 * \param[in]     SeriesId
 *                Specifies a time series
 * \param[in]     SetId
 *                Index of TSS set in time series entry
 *                [0 to DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS - 1]
 *
 * \return        Number of missing future samples
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetMissingFutureSamples(
  Dem_TimeSeriesEntry_ConstCompanionPtrToConstType EntryCompanion,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId
);

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_SetUnusedPastSamples
 *****************************************************************************/
/*!
 * \brief         Sets number of unused past samples in time series entry
 *
 * \details       Sets number of unused past samples in time series entry if available
 *
 * \param[in]     BaseEntry
 *                Pointer to the time series entry
 * \param[in]     SeriesId
 *                Specifies a time series
 * \param[in]     SetId
 *                Index of TSS set in time series entry
 *                [0 to DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS - 1]
 * \param[in]     UnusedPastSamples
 *                Number of unused past samples
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_SetUnusedPastSamples( 
  Dem_TimeSeriesEntry_ConstPtrToBaseType BaseEntry,                                                                              /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  Dem_TimeSeriesIdType SeriesId,
  uint16 SetId,
  uint8 UnusedPastSamples
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_SetMissingFutureSamples
 *****************************************************************************/
/*!
 * \brief         Set number of missing past samples of time series entry companion
 *
 * \details       Set number of missing past samples of time series entry companion
 *
 * \param[in]     EntryCompanion
 *                Pointer to the time series entry companion
 * \param[in]     SeriesId
 *                Specifies a time series
 * \param[in]     SetId
 *                Index of TSS set in time series entry
 *                [0 to DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS - 1]
 * \param[in]     UnusedFutureSamples
 *                Number of unused past samples
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_SetMissingFutureSamples( 
  Dem_TimeSeriesEntry_ConstCompanionPtrType  EntryCompanion,
  Dem_TimeSeriesIdType SeriesId,
  uint16 SetId,
  uint8  UnusedFutureSamples
);
#endif

/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_GetTimeSinceLastSample
 *****************************************************************************/
/*!
 * \brief         Return time since last sample collection of entry.
 *
 * \details       Return time since last sample collection of entry.
 *
 * \param[in]     Entry
 *                Pointer to the time series entry.
 *
 * \return        Time since last sample collection.
 *
 * \pre           -
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetTimeSinceLastSample(
  Dem_TimeSeriesEntry_PtrToConstBaseType Entry
  );

/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_SetTimeSinceLastSample
 *****************************************************************************/
/*!
 * \brief         Sets time since the last sample in time series entry
 *                was collected
 *
 * \details       Sets time since the last sample in time series entry 
 *                was collected. Only sets the value, if time series powertrain 
 *                is enabled
 *
 * \param[in]     BaseEntry
 *                Pointer to the time series Base entry
 * \param[in]     TimeSinceLastSample
 *                Time since the last sample was collected
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different entries.
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_SetTimeSinceLastSample(
  Dem_TimeSeriesEntry_ConstPtrToBaseType BaseEntry,
  uint16 TimeSinceLastSample
  );

/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_GetUnusedPastSamples
 *****************************************************************************/
/*!
 * \brief         Return number of unused past samples of time series entry
 *
 * \details       Return number of unused past samples of time series entry if available
 *
 * \param[in]     Entry
 *                Pointer to the time series entry
 * \param[in]     SeriesId
 *                Specifies a time series
 * \param[in]     SetId
 *                Index of TSS set in time series entry
 *                [0 to DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS - 1]
 *
 * \return        Number of unused past samples of time series entry.
 *
 * \pre           -
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetUnusedPastSamples(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_TimeSeriesEntry_PtrToConstBaseType Entry,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId
  );

/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_GetUnusedFutureSamples
 *****************************************************************************/
/*!
 * \brief         Return number of unused future samples of time series entry
 *
 * \details       Return number of unused future samples of time series entry
 *                if available and otherwise 0.
 *
 * \param[in]     Entry
 *                Pointer to the time series entry
 * \param[in]     SeriesId
 *                Specifies a time series
 * \param[in]     SetId
 *                Index of TSS set in time series entry
 *                [0 to DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS - 1]
 *
 * \return        Number of unused future samples of time series entry.
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetUnusedFutureSamples(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_TimeSeriesEntry_PtrToConstBaseType Entry,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId
);



#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_SetReadoutState
 *****************************************************************************/
/*!
 * \brief         Set readout state of time series entry.
 *
 * \details       Set readout state of time series entry.
 *
 * \param[in]     Entry
 *                Pointer to Time Series Entry
 *
 * \param[in]     State
 *                New readout state to be set
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_SetReadoutState(
  Dem_TimeSeriesEntry_ConstCompanionPtrType  EntryCompanion,
  Dem_TimeSeriesEntry_ReadoutStatusType  State
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
 /* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_TestEntryAvailable
 *****************************************************************************/
/*!
 * \brief         Test whether the entry is available based on whether it is in 
 *                readout status DELETION CANDIDATE
 *
 * \details       Test whether the entry is available based on whether it is in 
 *                readout status DELETION CANDIDATE
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \pre           -
 *
 * \return        TRUE
 *                The time series entry is available.
 * \return        FALSE
 *                The time series entry is not available.
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_TestEntryAvailable(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_TestEntryNeedToBeCleaned
 *****************************************************************************/
/*!
 * \brief         Test whether the time series entry with readout state of DELETION CANDIDATE
 *                can be deleted due to finished readout
 *
 * \details       -
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \return        TRUE
 *                The entry needs to be deleted with the main function
 * \return        FALSE
 *                The entry does not need to be deleted with the main function
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_TestEntryNeedToBeCleaned(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_GetSnapshotDataPtr
 *****************************************************************************/
/*!
 * \brief         Pointer to a snapshot in a time series entry
 *
 * \details       Pointer to a snapshot in a time series entry
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in Dem_Cfg_TimeSeriesEntryTable
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 * \param[in]     SeriesId
 *                Specifies a time series
 * \param[in]     SetId
 *                Specifies a set of the time series
 * \param[in]     IdOfSampleInItsSet
 *                Position of snapshot in set
 *
 * \return        Pointer to the constant indexed time series sample snapshotdata
 *                buffer
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetSnapshotDataPtr(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId,
  uint16 IdOfSampleInItsSet
  );
#endif




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
 * \defgroup Dem_TimeSeriesEntry_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_GetEntryIndexToNvBlockIndex
 *****************************************************************************/
/*!
 * \brief         Returns the Nv Block Index referencing the time series entry.
 *
 * \details       -
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time seres entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \return        NV Block Id
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_NvBlockIndexType, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetEntryIndexToNvBlockIndex(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
  );

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimerSeriesEntry_TestEntryForEvent
 *****************************************************************************/
/*!
 * \brief         Test if given event occupies time series entry linked to given index
 *
 * \details       Test if given event occupies time series entry linked to given index
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \return        TRUE
 *                Given event allocates given entry
 * \return        FALSE
 *                Given event does not allocate given entry
 * \pre           -
 *
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimerSeriesEntry_TestEntryForEvent(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_EventIdType EventId
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) 
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_EntryCompanionInit
 *****************************************************************************/
/*!
 * \brief         Initializes a time series entry companion
 *
 * \details       Initializes a time series entry companion
 *
 * \param[in]     TimeSeriesEntryCompanion
 *                Pointer to the time series entry companion
 * \param[in]     State
 *                New readout state to be set
 *
 * \pre           -
 *
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_EntryCompanionInit(
  Dem_TimeSeriesEntry_ConstCompanionPtrType TimeSeriesEntryCompanion,
  Dem_TimeSeriesEntry_ReadoutStatusType State
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_Free
 *****************************************************************************/
/*!
 * \brief         Free time series entry linked to given index
 *
 * \details       Free time series entry linked to given index
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 * \pre           -
 *
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_Free(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe
 *****************************************************************************/
/*!
 * \brief         Get the indexed time series Base entry
 *
 * \details       Get the indexed time series Base entry
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \return        Pointer to the indexed time series base entry
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_TimeSeriesEntryBasePtrType, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe
 *****************************************************************************/
/*!
 * \brief         Get the indexed time series entry companion
 *
 * \details       Get the indexed time series entry companion
 *
 * \param[in]     EntryCompanionIndex
 *                Index of the time series entry companion in Dem_Cfg_TimeSeriesEntryCompanion.
 *                Must be in range [0..Dem_Memories_TimeSeriesMemory_GetGlobalEntryCount()[.
 *
 * \return        Pointer to the indexed time series entry companion
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_TimeSeriesEntry_CompanionPtrType, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(
  Dem_Cfg_EntryIndexType EntryCompanionIndex
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_Init
 *****************************************************************************/
/*!
 * \brief         Initializes a time series entry
 *
 * \details       Initializes a time series entry
 *
 * \param[in]     TimeSeriesBaseEntry
 *                Pointer to the time series Base entry
 * \param[in]     EventId
 *                Unique handle of the Event.
 *
 * \pre           -
 *
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_Init(
  Dem_TimeSeriesEntry_ConstPtrToBaseType  TimeSeriesBaseEntry,
  uint16 EventId
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_InitializeSeries()
 *****************************************************************************/
/*!
 * \brief         Set the time series entry header data for the series to indicate that it is not yet stored
 *
 * \details       Set the time series entry header data for the series to indicate that it is not yet stored
 *
 * \param[in]     TimeSeriesBaseEntry
 *                Pointer to the time series Base entry
 *
 * \param[in]     EventId
 *                Id specifying the event
 *
 * \param[in]     SeriesId
 *                Specifies a time series
 *
 * \return        Number of time series for the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_InitializeSeries(
  Dem_TimeSeriesEntry_ConstPtrToBaseType  TimeSeriesBaseEntry,
  uint16 EventId,
  Dem_TimeSeriesIdType SeriesId
  );
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_TestEntryUpdatable
 *****************************************************************************/
/*!
 * \brief         Test whether TSSR associated with the event is updatable or not.
 *
 * \details       This function determines if TSSR associated with the event
 *                can be updated based on the configuration and presence of
 *                an ongoing 0x19-04 readout.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \return        TRUE
 *                Entry may be updated.
 * \return        FALSE
 *                Entry cannot be updated.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_TestEntryUpdatable(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
  );
#endif

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_Lock
 *****************************************************************************/
/*!
 * \brief         Update the entry readout status and increment client counter
 *
 * \details       Update the entry readout status and increment client counter
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 * \pre           EntryCompanion's state is UNLOCKED or LOCKED
 *
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *                && DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_Lock(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
);
#endif

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_ReleaseLock
 *****************************************************************************/
/*!
 * \brief         Decrements the entry client counter and update the readout status
 *
 * \details       Decrements the entry client counter and update the readout status
 *
 * \param[in]     EntryCompanion
 *                Pointer to Time Series Entry Companion
 *
 * \return        TRUE
 *                Entry should be freed
 * \return        FALSE
 *                Entry should not be freed
 *
 * \pre           -
 *
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *                && DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_ReleaseLock(
  Dem_TimeSeriesEntry_ConstCompanionPtrType EntryCompanion
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_RequestDeletion
 *****************************************************************************/
/*!
 * \brief         Request entry deletion and return whether the entry should be freed
 *
 * \details       Request entry deletion and return whether the entry should be freed
 *
 * \param[in]     EntryCompanion
 *                Pointer to Time Series Entry Companion
 *
 * \return        TRUE
 *                Entry should be freed
 * \return        FALSE
 *                Entry should not be freed
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_RequestDeletion(
  Dem_TimeSeriesEntry_ConstCompanionPtrType EntryCompanion
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
* Dem_MemoryEntry_TimeSeriesEntry_InitSnapshotDataBuffer
*****************************************************************************/
/*!
* \brief         Initialize the snapshot buffer to write data
*
* \details       Initialize the snapshot buffer for writing time series samples
*
* \param[in]     TimeSeriesEntryIndex
*                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
*                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
* \param[in]     SeriesId
*                Specifies a time series
* \param[in]     SetId
*                Specifies a set in the time series
* \param[in]     SampleOffsetInSet
*                Index of the time series snapshot record whose data shall be
*                be stored.
* \param[out]    SnapshotDataPtr
*                Pointer Destination Buffer for snapshot data
*
* \pre           -
* \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_InitSnapshotDataBuffer(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId,
  uint8 SampleOffsetInSet,
  Dem_Data_DestinationBufferPtrType SnapshotDataBuffer
  );
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_TIMESERIESENTRY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_TimeSeriesEntry_Interface.h
 *********************************************************************************************************************/
