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
/*! \addtogroup Dem_TimeSeriesEntry
 *  \{
 *  \file       Dem_TimeSeriesEntry_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Implementation of the Time Series Entry unit which handles access to and from time series entries.
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

#if !defined (DEM_TIMESERIESENTRY_IMPLEMENTATION_H)
#define DEM_TIMESERIESENTRY_IMPLEMENTATION_H

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
#include "Dem_TimeSeriesEntry_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */

#include "Dem_Cfg_Declarations.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_Core_Interface.h"
/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_TIMESERIESENTRY_IMPLEMENTATION_FILENAME "Dem_TimeSeriesEntry_Implementation.h"

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/


/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_TimeSeriesEntry_PrivateProperties Private Properties
 * \{
 */

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState
 *****************************************************************************/
/*!
 * \brief         Get readout status of the time series entry.
 *
 * \details       Get readout status of the time series entry.
 *
 * \param[in]     Entry
 *                Pointer to the time series entry
 *
 * \return        The readout status of the time series entry.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_TimeSeriesEntry_ReadoutStatusType, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(
  Dem_TimeSeriesEntry_ConstCompanionPtrToConstType  EntryCompanion
);
#endif

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_IncrementClientCounter
 *****************************************************************************/
 /*!
 * \brief         Increment the client counter of the time series entry
 *
 * \details       Increment the client counter of the time series entry
 *
 * \param[in]     EntryCompanion
 *                Pointer to Time Series Entry Companion
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *                && DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_IncrementClientCounter(
  Dem_TimeSeriesEntry_ConstCompanionPtrType  EntryCompanion
);
#endif

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_DecrementClientCounter
 *****************************************************************************/
/*!
 * \brief         Decrement the client counter of the time series entry
 *
 * \details       Decrement the client counter of the time series entry
 *
 * \param[in]     EntryCompanion
 *                Pointer to Time Series Entry Companion
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *                && DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_DecrementClientCounter(
  Dem_TimeSeriesEntry_ConstCompanionPtrType  EntryCompanion
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_InitClientCounter
 *****************************************************************************/
/*!
 * \brief         Initialize client counter of time series entry to 0.
 *
 * \details       Initialize client counter of time series entry to 0.
 *
 * \param[in]     EntryCompanion
 *                Pointer to Time Series Entry Companion
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_InitClientCounter(
  Dem_TimeSeriesEntry_ConstCompanionPtrType  EntryCompanion
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_GetClientCounter
 *****************************************************************************/
/*!
 * \brief         Get client counter of time series entry.
 *
 * \details       Get client counter of time series entry.
 *
 * \param[in]     EntryCompanion
 *                Pointer to Time Series Entry Companion
 *
 * \return        client counter of the time series entry
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetClientCounter(
  Dem_TimeSeriesEntry_ConstCompanionPtrToConstType  EntryCompanion
);
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_TimeSeriesEntry_ComputeSampleOffsetInEntryBuffer
 *****************************************************************************/
/*!
 * \brief         Offset of the specified sample from the start of the time series entry buffer
 *
 * \details       Offset of the specified sample from the start of the time series entry buffer.
 *                See DSGN-Dem-TimeSeriesDataStructure for a description of how data is stored
 *                in the entry
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
 * \return        Offset of the specified sample from the start of the time series entry buffer
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_ComputeSampleOffsetInEntryBuffer(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId,
  uint16 IdOfSampleInItsSet
  );
#endif


/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_TimeSeriesEntry_ReadoutStatusType, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(
  Dem_TimeSeriesEntry_ConstCompanionPtrToConstType  EntryCompanion
)
{
  return (EntryCompanion->ReadoutStatus);
}
#endif

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_IncrementClientCounter
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_IncrementClientCounter(
  Dem_TimeSeriesEntry_ConstCompanionPtrType EntryCompanion
)
{
  ++(EntryCompanion->ClientCounter);                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}
#endif

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_DecrementClientCounter
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_DecrementClientCounter(
  Dem_TimeSeriesEntry_ConstCompanionPtrType EntryCompanion
)
{
  Dem_Internal_AssertReturnVoid(EntryCompanion->ClientCounter > 0x00U, DEM_E_INCONSISTENT_STATE)
  --(EntryCompanion->ClientCounter);                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_InitClientCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_InitClientCounter(
  Dem_TimeSeriesEntry_ConstCompanionPtrType EntryCompanion
)
{
  EntryCompanion->ClientCounter = 0;                                                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_GetClientCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetClientCounter(
  Dem_TimeSeriesEntry_ConstCompanionPtrToConstType EntryCompanion
)
{
  return (EntryCompanion->ClientCounter);
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_ComputeSampleOffsetInEntryBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_ComputeSampleOffsetInEntryBuffer(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId,
  uint16 IdOfSampleInItsSet
  )
{
  uint32 lSamplesBeforeCurrentSet = 0;
  Dem_TimeSeriesEntry_PtrToConstBaseType lEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);
  uint32 lNumberOfSets = Dem_Memories_TimeSeriesMemory_GetMaxNumOfTssSets(lEntry->EventId);
  Dem_TimeSeriesIdType lSeriesId;
  for(lSeriesId = 0; lSeriesId < SeriesId; lSeriesId++)
  { /* Add up the number of samples in all sets of series earlier in the buffer */
    uint32 lSamplesPerSet = Dem_Memories_TimeSeriesMemory_GetNumberOfSamplesOfSeries(lEntry->EventId, lSeriesId);
    lSamplesBeforeCurrentSet += lSamplesPerSet * lNumberOfSets;
  }

  { /* Add the number of samples from earlier (by set id) sets in the buffer from this series */
    uint32 lSamplesPerSet = Dem_Memories_TimeSeriesMemory_GetNumberOfSamplesOfSeries(lEntry->EventId, lSeriesId);
    lSamplesBeforeCurrentSet += lSamplesPerSet * (uint32)SetId;
  }

  { /* Add the number of earlier samples in the current set and multiply by the size of the samples to get the offset */
    uint32 lBytesPerSample = Dem_Cfg_GetMaxSnapShotSizeOfTimeSeriesEntryTable(TimeSeriesEntryIndex);
    return lBytesPerSample * (lSamplesBeforeCurrentSet + (uint32)IdOfSampleInItsSet);
  }
}
#endif

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
 * \addtogroup Dem_TimeSeriesEntry_PublicProperties
 * \{
 */

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimerSeriesEntry_TestDisplaceable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimerSeriesEntry_TestDisplaceable(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
)
{
  Dem_TimeSeriesEntry_ReadoutStatusType lReadoutStatus;
  lReadoutStatus =
    Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(TimeSeriesEntryIndex)); /* SBSW_DEM_POINTER_READ_TIMESERIESENTRYCOMPANION */

  return (boolean)(lReadoutStatus == DEM_TIMESERIESENTRY_READOUTSTATUS_UNLOCKED);
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
 /* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_SetUnusedFutureSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_SetUnusedFutureSamples( 
  Dem_TimeSeriesEntry_ConstPtrToBaseType BaseEntry,                                                                              /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  Dem_TimeSeriesIdType SeriesId,
  uint16 SetId,
  uint8 UnusedFutureSamples
)
{
# if (DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_OFF)
#  if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ((SeriesId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SERIES) || (SetId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS))
  {
    Dem_Error_RunTimeCheckFailed(DEM_TIMESERIESENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#  endif
  {
    BaseEntry->UnusedFutureSamples[SeriesId][SetId] = UnusedFutureSamples;                                                       /* SBSW_DEM_ARRAY_WRITE_TIMESERIESENTRY_UNUSEDFUTURESAMPLES */
  }
# endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(BaseEntry)                                                                                    /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SeriesId)                                                                                     /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SetId)                                                                                        /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(UnusedFutureSamples)                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
}
#endif


#if (DEM_CFG_SUPPORT_TIME_SERIES_FUTURE == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_GetMissingFutureSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetMissingFutureSamples(                                                                         /* PRQA S 2889 */ /* MD_DEM_15.5 */
  Dem_TimeSeriesEntry_ConstCompanionPtrToConstType EntryCompanion,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId
  )
{
  return EntryCompanion->MissingFutureSamples[SeriesId][SetId];
}
#endif

/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_GetTimeSinceLastSample
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetTimeSinceLastSample(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_TimeSeriesEntry_PtrToConstBaseType Entry
  )
{
# if (DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON)
  return Entry->Td;
# else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Entry)                                                                                        /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return (uint16) 0x00U;
# endif
}

/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_SetTimeSinceLastSample
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_SetTimeSinceLastSample(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_TimeSeriesEntry_ConstPtrToBaseType BaseEntry,                                                                              /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  uint16 TimeSinceLastSample
)
{
#if (DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON)
  BaseEntry->Td = TimeSinceLastSample;                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(BaseEntry)                                                                                    /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TimeSinceLastSample)                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
}

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_GetUnusedPastSamplesNormal
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetUnusedPastSamples(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_TimeSeriesEntry_PtrToConstBaseType Entry,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId
  )
{
  return Entry->UnusedPastSamples[SeriesId][SetId];
}
#endif


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_GetUnusedFutureSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetUnusedFutureSamples(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_TimeSeriesEntry_PtrToConstBaseType Entry,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId
)
{
  return Entry->UnusedFutureSamples[SeriesId][SetId];
}
#endif


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_SetReadoutState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_SetReadoutState(
  Dem_TimeSeriesEntry_ConstCompanionPtrType EntryCompanion,
  Dem_TimeSeriesEntry_ReadoutStatusType State
)
{
  EntryCompanion->ReadoutStatus = State;                                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
 /* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_TestEntryAvailable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_TestEntryAvailable(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
)
{
  Dem_TimeSeriesEntry_CompanionPtrToConstType lEntryCompanion;
  lEntryCompanion = Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(TimeSeriesEntryIndex);

  return (boolean)(Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(lEntryCompanion)                                              /* SBSW_DEM_POINTER_READ_TIMESERIESENTRYCOMPANION */
    != DEM_TIMESERIESENTRY_READOUTSTATUS_DELETION_CANDIDATE);
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_TestEntryNeedToBeCleaned
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_TestEntryNeedToBeCleaned(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
)
{
  uint8 lClientCounter;
  Dem_TimeSeriesEntry_CompanionPtrToConstType lTimeSeriesEntryCompanion;
  Dem_TimeSeriesEntry_ReadoutStatusType lReadoutStatus;

  lTimeSeriesEntryCompanion = Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(TimeSeriesEntryIndex);
  lReadoutStatus = Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(lTimeSeriesEntryCompanion);                                   /* SBSW_DEM_POINTER_READ_TIMESERIESENTRYCOMPANION */
  lClientCounter = Dem_MemoryEntry_TimeSeriesEntry_GetClientCounter(lTimeSeriesEntryCompanion);                                  /* SBSW_DEM_POINTER_READ_TIMESERIESENTRYCOMPANION */

  return (boolean)((lReadoutStatus == DEM_TIMESERIESENTRY_READOUTSTATUS_DELETION_CANDIDATE) && (lClientCounter == 0x00U));
}
#endif



#if (DEM_CFG_SUPPORT_TIME_SERIES_FUTURE == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_SetMissingFutureSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_SetMissingFutureSamples(
  Dem_TimeSeriesEntry_ConstCompanionPtrType  EntryCompanion,
  Dem_TimeSeriesIdType SeriesId,
  uint16 SetId,
  uint8  UnusedFutureSamples
  )
{
#  if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ((SeriesId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SERIES) || (SetId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS))
  {
    Dem_Error_RunTimeCheckFailed(DEM_TIMESERIESENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#  endif
  {
    EntryCompanion->MissingFutureSamples[SeriesId][SetId] = UnusedFutureSamples;                                                 /* SBSW_DEM_ARRAY_WRITE_ENTRYCOMPANION_MISSINGFUTURESAMPLES */
  }
}
#endif


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_GetSnapshotDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetSnapshotDataPtr(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId,
  uint16 IdOfSampleInItsSet
  )
{
  Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = TimeSeriesEntryIndex;
  uint32 lTotalOffset = 0;
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (TimeSeriesEntryIndex >= Dem_Cfg_GetSizeOfTimeSeriesEntryTable())
  {
    Dem_Error_RunTimeCheckFailed(DEM_TIMESERIESENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lTimeSeriesEntryIndex = 0;
  }
  else
  if ((SeriesId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SERIES) || (SetId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS))
  {
    Dem_Error_RunTimeCheckFailed(DEM_TIMESERIESENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    lTotalOffset = Dem_MemoryEntry_TimeSeriesEntry_ComputeSampleOffsetInEntryBuffer(TimeSeriesEntryIndex, SeriesId, SetId, IdOfSampleInItsSet);

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    {
      uint32 lBufferLimit = lTotalOffset + Dem_Cfg_GetMaxSnapShotSizeOfTimeSeriesEntryTable(TimeSeriesEntryIndex);
      if((lBufferLimit <= Dem_Cfg_GetBufferSizeOfTimeSeriesEntryTable(TimeSeriesEntryIndex)) && 
         (lTotalOffset < lBufferLimit))
      {
        /* Current snapshot is completely contained in the buffer. */
      }
      else
      { 
        Dem_Error_RunTimeCheckFailed(DEM_TIMESERIESENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
        lTotalOffset = 0; 
      }
    }
# endif
  }

  return &Dem_Cfg_GetEntryDataOfTimeSeriesEntryTable(lTimeSeriesEntryIndex)[lTotalOffset];
}
#endif

#if(DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_SetUnusedPastSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_SetUnusedPastSamples(
  Dem_TimeSeriesEntry_ConstPtrToBaseType BaseEntry,
  Dem_TimeSeriesIdType SeriesId,
  uint16 SetId,
  uint8 UnusedPastSamples
)
{
#  if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ((SeriesId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SERIES) || (SetId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS))
  {
    Dem_Error_RunTimeCheckFailed(DEM_TIMESERIESENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#  endif
  {
    BaseEntry->UnusedPastSamples[SeriesId][SetId] = UnusedPastSamples;                                                           /* SBSW_DEM_ARRAY_WRITE_TIMESERIESENTRY_UNUSEDPASTSAMPLES */
  }
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
 * \defgroup Dem_TimeSeriesEntry_Private Private Methods
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
 * \addtogroup Dem_TimeSeriesEntry_Public
 * \{
 */

/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_GetEntryIndexToNvBlockIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(Dem_Cfg_NvBlockIndexType, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetEntryIndexToNvBlockIndex(                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Internal_AssertReturnValue((TimeSeriesEntryIndex < Dem_Cfg_GetSizeOfTimeSeriesEntryIndexToNvBlockIndex()), \
    DEM_E_INCONSISTENT_STATE, DEM_CFG_INVALID_NVBLOCK_INDEX)

  return Dem_Cfg_GetTimeSeriesEntryIndexToNvBlockIndex(TimeSeriesEntryIndex);                                                    /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TimeSeriesEntryIndex);                                                                        /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return DEM_CFG_INVALID_NVBLOCK_INDEX;
#endif
}


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimerSeriesEntry_TestEntryForEvent
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
Dem_MemoryEntry_TimerSeriesEntry_TestEntryForEvent(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex, 
  Dem_EventIdType EventId
  )
{
  Dem_EventIdType lEventId;
  boolean lEntryAvailable;

  lEventId = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex)->EventId;
  lEntryAvailable = Dem_MemoryEntry_TimeSeriesEntry_TestEntryAvailable(TimeSeriesEntryIndex);
  
  return (boolean)((lEventId == EventId) && (lEntryAvailable == TRUE));
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) 
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_EntryCompanionInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_EntryCompanionInit(
  Dem_TimeSeriesEntry_ConstCompanionPtrType TimeSeriesEntryCompanion,
  Dem_TimeSeriesEntry_ReadoutStatusType State
)
{
  Dem_MemoryEntry_TimeSeriesEntry_SetReadoutState(TimeSeriesEntryCompanion,                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    State);
  Dem_MemoryEntry_TimeSeriesEntry_InitClientCounter(TimeSeriesEntryCompanion);                                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

# if (DEM_CFG_SUPPORT_TIME_SERIES_FUTURE == STD_ON)
  {
    Dem_TimeSeriesIdType lSeriesId;
    for (lSeriesId = 0; lSeriesId < DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SERIES; lSeriesId++)
    {
      uint8 lSetId;
      for (lSetId = 0u; lSetId < DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS; lSetId++)
      {
        Dem_MemoryEntry_TimeSeriesEntry_SetMissingFutureSamples(TimeSeriesEntryCompanion, lSeriesId, lSetId, 0u);                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TimeSeriesEntryCompanion)
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(State)
# endif
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_Free
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
Dem_MemoryEntry_TimeSeriesEntry_Free(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
  )
{
  Dem_TimeSeriesEntry_CompanionPtrType lTimeSeriesEntryCompanionPtr;
  Dem_Cfg_TimeSeriesEntryBasePtrType lTimeSeriesEntryPtr;

  lTimeSeriesEntryPtr = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);
  lTimeSeriesEntryCompanionPtr = Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(TimeSeriesEntryIndex);

  lTimeSeriesEntryPtr->EventId = DEM_EVENT_INVALID;                                                                              /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
  lTimeSeriesEntryPtr->Timestamp = 0;                                                                                            /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */

  {
    Dem_TimeSeriesIdType lSeriesId;
    for(lSeriesId = 0; lSeriesId < DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SERIES; lSeriesId++)
    {
      lTimeSeriesEntryPtr->NextSetId[lSeriesId] = 0u;                                                                            /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
      lTimeSeriesEntryPtr->AllSetsStored[lSeriesId] = FALSE;                                                                     /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
    }
  }

/* No critical section needed for setting readout state and client counter since this entry-to-free function is unreachable for external APIs (Dem_EnableDtcRecordupdate). */
  Dem_MemoryEntry_TimeSeriesEntry_SetReadoutState(lTimeSeriesEntryCompanionPtr, 
    DEM_TIMESERIESENTRY_READOUTSTATUS_UNUSED);                                                                                   /* SBSW_DEM_POINTER_WRITE_TIMESERIESENTRYCOMPANION */
  Dem_MemoryEntry_TimeSeriesEntry_InitClientCounter(lTimeSeriesEntryCompanionPtr);                                               /* SBSW_DEM_POINTER_WRITE_TIMESERIESENTRYCOMPANION */
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_TimeSeriesEntryBasePtrType, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
)
{
  Dem_Cfg_TimeSeriesEntryBasePtrType lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (TimeSeriesEntryIndex >= Dem_Memories_TimeSeriesMemory_GetSizeOfTimeSeriesEntryTable())
  {
    lReturnValue = Dem_Cfg_GetBaseEntryOfTimeSeriesEntryTable(0u);
    Dem_Error_RunTimeCheckFailed(DEM_TIMESERIESENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    lReturnValue = Dem_Cfg_GetBaseEntryOfTimeSeriesEntryTable(TimeSeriesEntryIndex);
  }
  return lReturnValue;
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
*/
DEM_LOCAL FUNC(Dem_TimeSeriesEntry_CompanionPtrType, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(
  Dem_Cfg_EntryIndexType EntryCompanionIndex
)
{
  Dem_Cfg_TimeSeriesEntryCompanionType* lReturnValue;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EntryCompanionIndex >= Dem_Memories_TimeSeriesMemory_GetGlobalEntryCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_TIMESERIESENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lReturnValue = &Dem_Cfg_TimeSeriesEntryCompanion[0u];                                                                        /* PRQA S 0310, 3305 */ /* MD_DEM_11.4_nvm, MD_DEM_3305 */
  }
  else
# endif
  {
    lReturnValue = &Dem_Cfg_TimeSeriesEntryCompanion[EntryCompanionIndex];                                                       /* PRQA S 0310, 3305 */ /* MD_DEM_11.4_nvm, MD_DEM_3305 */
  }
  return lReturnValue;
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_Init(
  Dem_TimeSeriesEntry_ConstPtrToBaseType  TimeSeriesBaseEntry,
  uint16 EventId
  )
{
  Dem_TimeSeriesIdType lSeriesId;
  for(lSeriesId = 0u; lSeriesId < DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SERIES; lSeriesId++)
  {
    Dem_MemoryEntry_TimeSeriesEntry_InitializeSeries(TimeSeriesBaseEntry, EventId, lSeriesId);                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_InitializeSeries
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_InitializeSeries(
  Dem_TimeSeriesEntry_ConstPtrToBaseType  TimeSeriesBaseEntry,
  uint16 EventId,
  Dem_TimeSeriesIdType SeriesId
  )
{
  if(SeriesId >= DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SERIES)
  {
    Dem_Error_RunTimeCheckFailed(DEM_TIMESERIESENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
  {
    Dem_TimeSeriesIdType lNumberOfSeries = Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(EventId);
    if(SeriesId < lNumberOfSeries)
    {
      Dem_Cfg_SamplingProfileIterType lSamplingProfileId = Dem_Memories_TimeSeriesMemory_GetSamplingProfileId(EventId, SeriesId);
      uint8 lSetId;
      for (lSetId = 0u; lSetId < DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS; lSetId++)
      {
        Dem_MemoryEntry_TimeSeriesEntry_SetUnusedPastSamples(TimeSeriesBaseEntry, SeriesId, lSetId, Dem_Cfg_GetPastSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId)); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        Dem_MemoryEntry_TimeSeriesEntry_SetUnusedFutureSamples(TimeSeriesBaseEntry, SeriesId, lSetId, Dem_Cfg_GetFutureSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId)); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }
      DEM_IGNORE_UNUSED_VARIABLE(lSamplingProfileId);                                                                            /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 

      TimeSeriesBaseEntry->NextSetId[SeriesId] = 0u;                                                                             /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
      TimeSeriesBaseEntry->AllSetsStored[SeriesId] = FALSE;                                                                      /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
    }
    else 
    {
      uint8 lSetId;
      for (lSetId = 0u; lSetId < DEM_CFG_TIMESERIES_MAX_NUMBER_OF_SETS; lSetId++)
      {
        Dem_MemoryEntry_TimeSeriesEntry_SetUnusedPastSamples(TimeSeriesBaseEntry, SeriesId, lSetId, 255u);                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        Dem_MemoryEntry_TimeSeriesEntry_SetUnusedFutureSamples(TimeSeriesBaseEntry, SeriesId, lSetId, 255u);                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }

      TimeSeriesBaseEntry->NextSetId[SeriesId] = 0u;                                                                             /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
      TimeSeriesBaseEntry->AllSetsStored[SeriesId] = FALSE;                                                                      /* SBSW_DEM_POINTER_WRITE_TIMESERIES_BASEENTRY */
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_TestEntryUpdatable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_TestEntryUpdatable(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
  )
{
  boolean lEntryUpdatable = FALSE;

  if (Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(TimeSeriesEntryIndex)) /* SBSW_DEM_POINTER_READ_TIMESERIESENTRYCOMPANION */
        == DEM_TIMESERIESENTRY_READOUTSTATUS_UNLOCKED)
  {
    lEntryUpdatable = TRUE;
  }

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TimeSeriesEntryIndex)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

  return lEntryUpdatable;
}
#endif

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_Lock
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
Dem_MemoryEntry_TimeSeriesEntry_Lock(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
)
{
/* >>>> -------------------------------- Enter Critical Section: DcmApi */
/* This function is involved in main function. To prevent that the readout status and client counter are
 * modified due to enable DTC update, a critical section is needed */
  Dem_Cfg_TimeSeriesEntryCompanionPtrType lEntryCompanion = Dem_MemoryEntry_TimeSeriesEntry_GetEntryCompanionSafe(TimeSeriesEntryIndex);
  Dem_EnterCritical_DcmApi();
  if (Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(lEntryCompanion) == DEM_TIMESERIESENTRY_READOUTSTATUS_UNLOCKED)            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_MemoryEntry_TimeSeriesEntry_SetReadoutState(lEntryCompanion,
      DEM_TIMESERIESENTRY_READOUTSTATUS_LOCKED_FOR_READOUT);                                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }

  Dem_Internal_AssertReturnVoid(Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(lEntryCompanion)                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      == DEM_TIMESERIESENTRY_READOUTSTATUS_LOCKED_FOR_READOUT, DEM_E_INCONSISTENT_STATE);

  Dem_MemoryEntry_TimeSeriesEntry_IncrementClientCounter(lEntryCompanion);                                                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */


  Dem_LeaveCritical_DcmApi();
/* <<<< -------------------------------- Leave Critical Section: DcmApi */
}
#endif

#if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON))
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_ReleaseLock
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_ReleaseLock(
  Dem_TimeSeriesEntry_ConstCompanionPtrType  EntryCompanion
)
{
  boolean lReturn;
  lReturn = FALSE;

/* >>>> -------------------------------- Enter Critical Section: DcmApi */
/* this function is involved when a client enables the DTC update. A critical section is needed to prevent the status being modified from main function */
  Dem_EnterCritical_DcmApi();
  Dem_MemoryEntry_TimeSeriesEntry_DecrementClientCounter(EntryCompanion);                                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  if (Dem_MemoryEntry_TimeSeriesEntry_GetClientCounter(EntryCompanion) == 0x00U)                                                 /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */                                               
  {
    if (Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(EntryCompanion)
      == DEM_TIMESERIESENTRY_READOUTSTATUS_LOCKED_FOR_READOUT )                                                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    {
      Dem_MemoryEntry_TimeSeriesEntry_SetReadoutState(EntryCompanion,                                                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        DEM_TIMESERIESENTRY_READOUTSTATUS_UNLOCKED);
    }

    if (Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(EntryCompanion)
      == DEM_TIMESERIESENTRY_READOUTSTATUS_DELETION_CANDIDATE)                                                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    {
      lReturn = TRUE;
    }
  }
  Dem_LeaveCritical_DcmApi();
/* <<<< -------------------------------- Leave Critical Section: DcmApi */

  return lReturn;
}
#endif

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_RequestDeletion
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
Dem_MemoryEntry_TimeSeriesEntry_RequestDeletion(
  Dem_TimeSeriesEntry_ConstCompanionPtrType EntryCompanion
)
{
  boolean lReturn;
  lReturn = FALSE;
/* >>>> -------------------------------- Enter Critical Section: DcmApi */
/* This function is involved in main function. To prevent that the readout status is modified due to enable DTC update, a critical section is needed */
  Dem_EnterCritical_DcmApi();
  if (Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(EntryCompanion)
    == DEM_TIMESERIESENTRY_READOUTSTATUS_LOCKED_FOR_READOUT)                                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_MemoryEntry_TimeSeriesEntry_SetReadoutState(EntryCompanion, 
      DEM_TIMESERIESENTRY_READOUTSTATUS_DELETION_CANDIDATE);                                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  Dem_LeaveCritical_DcmApi();
/* <<<< -------------------------------- Leave Critical Section: DcmApi */
                                                                                                                                 
  if (Dem_MemoryEntry_TimeSeriesEntry_GetReadoutState(EntryCompanion)                                                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    == DEM_TIMESERIESENTRY_READOUTSTATUS_UNLOCKED)  
  {/* The client counter and readout status of an entry with readout status of UNLOCKED can only be changed within main function 
      So no critical section is needed */
    lReturn = TRUE;
  }

  return lReturn;
}
#endif


#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_TimeSeriesEntry_InitSnapshotDataBuffer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_TimeSeriesEntry_InitSnapshotDataBuffer(
  Dem_Cfg_EntryIndexType  TimeSeriesEntryIndex,
  Dem_TimeSeriesIdType SeriesId,
  uint8 SetId,
  uint8  SampleOffsetInSet,
  Dem_Data_DestinationBufferPtrType SnapshotDataBuffer
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if(TimeSeriesEntryIndex >= Dem_Cfg_GetSizeOfTimeSeriesEntryTable())                                                            /* PRQA S 2842 */ /* MD_DEM_Dir4.1_ReadOperation */
  {
    Dem_Error_RunTimeCheckFailed(DEM_TIMESERIESENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    Dem_Data_InitDestinationBuffer(SnapshotDataBuffer,                                                                           /* SBSW_DEM_CALL_TIMESERIES_SNAPSHOTDATA_DSTBUFFER_INIT */
                                  Dem_MemoryEntry_TimeSeriesEntry_GetSnapshotDataPtr(0u, SeriesId, SetId, 0u),
                                  Dem_Cfg_GetMaxSnapShotSizeOfTimeSeriesEntryTable(0u));
  }
  else
# endif
  {
    Dem_Data_InitDestinationBuffer(SnapshotDataBuffer,                                                                           /* SBSW_DEM_CALL_TIMESERIES_SNAPSHOTDATA_DSTBUFFER_INIT */
                                  Dem_MemoryEntry_TimeSeriesEntry_GetSnapshotDataPtr(TimeSeriesEntryIndex, SeriesId, SetId, SampleOffsetInSet),
                                  Dem_Cfg_GetMaxSnapShotSizeOfTimeSeriesEntryTable(TimeSeriesEntryIndex));
  }
}
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_TIMESERIESENTRY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_TimeSeriesEntry_Implementation.h
 *********************************************************************************************************************/
