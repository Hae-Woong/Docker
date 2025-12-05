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
/*! \addtogroup Dem_FreezeFrameIterator
 *  \{
 *  \file       Dem_FreezeFrameIterator_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) Implementation file
 *  \entity     FreezeFrameIterator
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_FREEZEFRAMEITERATOR_IMPLEMENTATION_H)
#define DEM_FREEZEFRAMEITERATOR_IMPLEMENTATION_H

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
#include "Dem_FreezeFrameIterator_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_FaultMemory_Interface.h"
#include "Dem_RecordReader_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_FilterData_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_DTCReporting_Interface.h"
#include "Dem_ClientAccess_Interface.h"


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_FREEZEFRAMEITERATOR_IMPLEMENTATION_FILENAME "Dem_FreezeFrameIterator_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_FreezeFrameIterator_Private Private Methods
 * \{
 */

# if (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF)
/* ****************************************************************************
 * Dem_FreezeFrameIterator_TimeSeriesSRecCount
 *****************************************************************************/
/*!
 * \brief         Counts the number of snapshot records stored in a time series
 *                entry
 *
 * \details       Counts the number of snapshot records stored in a time series
 *                entry
 *
 * \param[in]     TimeSeriesEntryIndex
 *                Index of the time series entry in table Dem_Cfg_TimeSeriesEntryTable.
 *                Must be in range [0..Dem_Cfg_GetSizeOfTimeSeriesEntryTable()[.
 *
 * \return        Number of stored time series snapshot records
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON &&
 *                DEM_FEATURE_NEED_TIME_SERIES == STD_ON &&
 *                DEM_CFG_SUPPORT_SRECS == STD_ON &&
 *                DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_TimeSeriesSRecCount(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
  );
#endif

 /* ****************************************************************************
 * Dem_FreezeFrameIterator_SetTimeSeriesPowertrainReportedState
 *****************************************************************************/
/*!
 * \brief         Set the time series powertrain reported state of a
 *                snapshot iterator.
 *
 * \details       Set the time series powertrain reported state of a
 *                snapshot iterator.
 *
 * \param[in,out] FreezeFrameFilterPtr
 *                Pointer to the current freeze frame filter
 * \param[in]     ReportedState
 *                New time series powertrain reported state
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_SetTimeSeriesPowertrainReportedState(
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr,
  boolean ReportedState
);

/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetTimeSeriesPowertrainReportedState
 *****************************************************************************/
/*!
 * \brief         Get the time series powertrain reported state of a
 *                snapshot iterator.
 *
 * \details       Get the time series powertrain reported state of a
 *                snapshot iterator.
 *
 * \param[in]     FreezeFrameFilterPtr
 *                Pointer to the current freeze frame filter
 *
 * \return        TRUE
 *                Time series powertrain was reported
 * \return        False
 *                Time series powertrain was not yet reported
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_GetTimeSeriesPowertrainReportedState(
  CONSTP2CONST(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
);

/* ****************************************************************************
 * Dem_FreezeFrameIterator_SetOBDonUDSFF0x00ReportedState
 *****************************************************************************/
/*!
 * \brief         Set the OBD on UDS FF 0x00 reported state of a
 *                FF filter.
 *
 * \details       Set the OBD on UDS FF 0x00 reported state of a
 *                FF filter.
 *
 * \param[in,out] FreezeFrameFilterPtr
 *                Pointer to the current freeze frame filter
 * \param[in]     ReportedState
 *                New OBD on UDS FF 0x00 reported state
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_SetOBDonUDSFF0x00ReportedState(
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr,
  boolean ReportedState
);

/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetOBDonUDSFF0x00ReportedState
 *****************************************************************************/
 /*!
 * \brief         Get the OBD on UDS FF 0x00 reported state of a
 *                FF filter.
 *
 * \details       Get the OBD on UDS FF 0x00 reported state of a
 *                FF filter.
 *
 * \param[in]     FreezeFrameFilterPtr
 *                Pointer to the current freeze frame filter
 *
 * \return        TRUE
 *                OBD on UDS FF 0x00 was reported
 * \return        False
 *                OBD on UDS FF 0x00 was not yet reported
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_GetOBDonUDSFF0x00ReportedState(
  CONSTP2CONST(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
);


/* ****************************************************************************
 * Dem_FreezeFrameIterator_SetObdFFMemoryIndex
 *****************************************************************************/
/*!
 * \brief         Set the OBD on UDS FF index of a
 *                FF filter.
 *
 * \details       Set the OBD on UDS FF index of a
 *                FF filter.
 *
 * \param[in,out] FreezeFrameFilterPtr
 *                Pointer to the current freeze frame filter
 * \param[in]     ObdFFMemoryIndex
 *                New OBD on UDS FF index
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_SetObdFFMemoryIndex(
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr,
  uint8 ObdFFMemoryIndex
);

/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetObdFFMemoryIndex
 *****************************************************************************/
/*!
 * \brief         Get the OBD on UDS FF index of a
 *                FF filter.
 *
 * \details       Get the OBD on UDS FF index of a
 *                FF filter.
 *
 * \param[in]     FreezeFrameFilterPtr
 *                Pointer to the current freeze frame filter
 *
 * \return        OBD FF memory index, or invalid value
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetObdFFMemoryIndex(
  CONSTP2CONST(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
);


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetDTCFormat
 *****************************************************************************/
/*!
 * \brief         Get the DTCFormat of the FreezeFrameIterator.
 *
 * \details       Get the DTCFormat of the FreezeFrameIterator.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 *
 * \return        DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT
 *                Initialization value, filter/iterator is unused.
 *
 *                DEM_FREEZEFRAMEITERATOR_DTCFORMAT_UDS
 *                The filter selects the 3-byte UDS DTC format
 *                (refer to configuration parameter DemUdsDTC).
 *
 *                DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD
 *                The filter selects the 2-byte OBD DTC format
 *                (refer to configuration parameter DemObdDTC).
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_FreezeFrameIterator_DTCFormatType, DEM_CODE)
Dem_FreezeFrameIterator_GetDTCFormat(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  );
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetEventEntryIndex
 *****************************************************************************/
/*!
 * \brief         Get the event entry index of the FreezeFrameIterator.
 *
 * \details       Get the event entry index of the FreezeFrameIterator.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 *
 * \return        The event entry iterator position.
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FreezeFrameIterator_GetEventEntryIndex(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  );
#endif


#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetNodeIndex
 *****************************************************************************/
/*!
 * \brief         Get the node Index of the FreezeFrameIterator.
 *
 * \details       Get the node Index of the FreezeFrameIterator.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 *
 * \return        The node Index of the FreezeFrameIterator.
 *
 * \config        DEM_CFG_SUPPORT_J1939 == STD_ON
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetNodeIndex(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  );
#endif

/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetFilterState
 *****************************************************************************/
/*!
 * \brief         Get the filter state of the FreezeFrameIterator.
 *
 * \details       Get the filter state of the FreezeFrameIterator.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 *
 * \return        The filter state of the FreezeFrameIterator.
 *
 * \config        -
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetFilterState(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  );

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)))
/* ****************************************************************************
 * Dem_FreezeFrameIterator_NextEvent
 *****************************************************************************/
/*!
 * \brief         Proceed FreezeFrameIterator to next event.
 *
 * \details       Proceed FreezeFrameIterator to next event entry and
 *                reset snapshot specific flags and iterators.
 *
 * \param[in,out] FreezeFrameFilterPtr
 *                Pointer to the current freeze frame filter
 * \param[in,out] MemEntryIter
 *                Pointer to processed and next event entry
 *
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON and
 *                (DEM_CFG_SUPPORT_SRECS == STD_ON or
 *                DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different FreezeFrameIterator Ids
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_NextEvent(
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr,
  CONSTP2VAR(Dem_Cfg_EntryIterType, AUTOMATIC, AUTOMATIC) EventEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetNextFilteredGlobalSRecNumber
 *****************************************************************************/
/*!
 * \brief         Filter for global snapshot
 *
 * \details       Filter for global snapshot
 *
 * \param[in,out] FreezeFrameFilterPtr
 *                Pointer to the freeze frame filter iterator
 *
 * \return        SRecNumber
 *                SRecNumber identifying the global SRec
 *
 * \pre           SnapshotEntryIter has been initialized
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON ||
 *                DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetNextFilteredGlobalSRecNumber(
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
  );
#endif


#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)))
/* ****************************************************************************
 * Dem_FreezeFrameIterator_FilterSRec
 *****************************************************************************/
/*!
 * \brief         Filters the snapshot records of events stored in primary
 *                memory.
 * \details       Filters the snapshot records of events stored in primary
 *                memory.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 * \param[out]    DTC
 *                Pointer to receive the UDS DTC number
 * \param[out]    RecordNumber
 *                Pointer to receive the snapshot record number
 *
 * \return        TRUE
 *                Next record was found
 *                FALSE
 *                No more record found
 *
 * \pre           The record filter has been initialized by
 *                Dem_SetFreezeFrameRecordFilter
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *                && ((DEM_CFG_SUPPORT_SRECS == STD_ON) 
 *                   || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_FilterSRec(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  RecordNumber
  );
# endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 * Dem_Client_FreezeFrameIterator_GetNextFilteredObdOnUdsFFMemoryEntry
 *****************************************************************************/
/*!
 * \brief         Filters the snapshot records of events stored in OBD FF memory.
 * \details       Filters the snapshot records of events stored in OBD FF memory.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 * \param[out]    DTC
 *                Pointer to receive the UDS DTC number
 * \param[out]    RecordNumber
 *                Pointer to receive the snapshot record number
 *
 * \return        TRUE
 *                Next record was found
 *                FALSE
 *                No more record found
 *
 * \pre           The record filter has been initialized by
 *                Dem_SetFreezeFrameRecordFilter
 * \config        DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_FreezeFrameIterator_GetNextFilteredObdOnUdsFFMemoryEntry(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  RecordNumber
);
# endif


#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)))
/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetNextFilteredSRecNumber
 *****************************************************************************/
 /*!
 * \brief         Get next stored Snapshot Record Number for an event.
 *
 * \details       Get next stored Snapshot Record Number for the provided event,
 *                based on the provided freeze frame filter data.
 *
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     FreezeFrameFilterPtr
 *                Pointer to the current freeze frame filter
 *
 * \return        RecordNumber
 *                Unique identifier of the next SRec
 *
 * \pre           The record filter has been initialized by
 *                Dem_SetFreezeFrameRecordFilter
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *                && ((DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                   || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetNextFilteredSRecNumber(                                                                               /* PRQA S 2889 */ /* MD_DEM_15.5 */
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
);
#endif

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && (DEM_CFG_SUPPORT_SRECS == STD_ON) )
/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetNextFilteredEventSpecificSRecNumber
 *****************************************************************************/
 /*!
 * \brief         Get next stored Snapshot Record Number specific to the event.
 *
 * \details       Get next stored Snapshot Record Number specific to the event
 *                (which excludes Global Snapshot Records).
 *
 * \param[in]     FreezeFrameFilterPtr
 *                Pointer to the current freeze frame filter
 *
 * \return        RecordNumber
 *                Unique identifier of the next SRec
 *
 * \pre           The record filter has been initialized by
 *                Dem_SetFreezeFrameRecordFilter
 * \config        (   DEM_CFG_SUPPORT_DCM   == STD_ON)
 *                && (DEM_CFG_SUPPORT_SRECS == STD_ON) )
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetNextFilteredEventSpecificSRecNumber(                                                                  /* PRQA S 2889 */ /* MD_DEM_15.5 */
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
);
#endif

# if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF))
/* ****************************************************************************
 * Dem_FreezeFrameIterator_FilterTimeSeriesSRec
 *****************************************************************************/
/*!
 * \brief         Filters the time series snapshot records
 *
 * \details       Filters the time series snapshot records
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 * \param[out]    DTC
 *                Pointer to receive the UDS DTC number
 * \param[out]    RecordNumber
 *                Pointer to receive the snapshot record number
 *
 * \return        TRUE
 *                Next record was found
 *                FALSE
 *                No more record found
 *
 * \pre           The record filter has been initialized by
 *                Dem_SetFreezeFrameRecordFilter
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *                && (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
 *                && (DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_FilterTimeSeriesSRec( 
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  RecordNumber
  );
# endif

/* ****************************************************************************
 * Dem_Client_FreezeFrameIterator_FilterCustomTriggerRecord
 *****************************************************************************/
/*!
 * \brief         Returns the next stored custom triggered record according to
 *                set filter
 *
 * \details       Returns the filtered custom triggered snapshot records
 *                (both snapshot record and time series snapshot record)
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 * \param[out]    DTC
 *                Pointer to receive the UDS DTC number
 * \param[out]    RecordNumber
 *                Pointer to receive the snapshot record number
 *
 * \return        TRUE
 *                Next record was found
 *                FALSE
 *                No more record found
 *
 * \pre           The record filter has been initialized by
 *                Dem_SetFreezeFrameRecordFilter
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_FreezeFrameIterator_FilterCustomTriggerRecord(
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  RecordNumber
  );

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)))
/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetNumberOfSRec
 *****************************************************************************/
/*!
 * \brief         Gets the number of stored snapshot records
 *
 * \details       Gets the number of stored snapshot records
 *
 * \return        Number of stored records
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *                && ((DEM_CFG_SUPPORT_SRECS == STD_ON) 
 *                   || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FreezeFrameIterator_GetNumberOfSRec(
  void
  );
# endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
* Dem_Client_FreezeFrameIterator_GetNumberOfObdOnUdsSRec
*****************************************************************************/
/*!
* \brief         Gets the number of stored OBD on UDS snapshot records
*
* \details       Gets the number of stored OBD on UDS snapshot records 
*                within the OBD FF memory.
*
* \return        Number of stored OBD on UDS FF records
*
* \pre           -
* \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     FALSE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_FreezeFrameIterator_GetNumberOfObdOnUdsSRec(
  void
  );
# endif



# if (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF)
/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetNumberOfTimeSeriesSRec
 *****************************************************************************/
/*!
 * \brief         Gets the number of stored time series snapshot records
 *
 * \details       Gets the number of stored time series snapshot records
 *
 * \return        number of stored records
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_DCM == STD_ON)
 *                && (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *                && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
 *                && (DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FreezeFrameIterator_GetNumberOfTimeSeriesSRec(
  void
  );
# endif

/* ****************************************************************************
 * Dem_Client_FreezeFrameIterator_GetNumberOfCustomTriggerRecords
 *****************************************************************************/
/*!
 * \brief         Gets the number of stored custom triggered records
 *
 * \details       Gets the number of stored custom triggered snapshot records
 *
 * \return        number of stored records
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_FreezeFrameIterator_GetNumberOfCustomTriggerRecords(
  void
  );

/* ****************************************************************************
 * Dem_FreezeFrameIterator_J1939SpnIter_GetSpnDataLength
 *****************************************************************************/
/*!
 * \brief         Get the SPN Data Length pointed to by the iterator.
 *
 * \details       Get the SPN Data Length pointed to by the iterator.
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        SpnDataLength
 *                The SPN Data Length pointed to by the iterator
 *
 * \pre           IterPtr has been initialized.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_J1939SpnIter_GetSpnDataLength(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_FreezeFrameIterator_J1939SpnIter_GetSpnId
 *****************************************************************************/
/*!
 * \brief         Get the SPN Id pointed to by the iterator.
 *
 * \details       Get the SPN Id pointed to by the iterator.
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        SpnId
 *                The SPN Id pointed to by the iterator
 *
 * \pre           IterPtr has been initialized.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_FreezeFrameIterator_J1939SpnIter_GetSpnId(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );


/* ****************************************************************************
 * Dem_FreezeFrameIterator_SetJ1939SpnIter
 *****************************************************************************/
/*!
 * \brief         Assign SPN iterator to the SPN iterator of 
 *                provided Freeze frame iterator handle.
 *
 * \details       Assign SPN iterator to the SPN iterator of 
 *                provided Freeze frame iterator handle.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 * \param[in]     SpnIter
 *                SPN iterator to assign
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_SetJ1939SpnIter(
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId,
  Dem_Cfg_ComplexIterType  SpnIter
  );

/* ****************************************************************************
 * Dem_FreezeFrameIterator_GetJ1939SpnIter
 *****************************************************************************/
/*!
 * \brief         Get the SPN iterator of the current iteration handle.
 *
 * \details       Get the SPN iterator of the current iteration handle.
 *
 * \param[in]     FreezeFrameIteratorId
 *                Index in FreezeFrameIteratorTable, derived from the ClientId.
 *
 * \return        SpnIter
 *                SPN iterator for the current freeze frame iteration handle
 *
 * \pre           Freeze Frame handle is valid and SPN Iter is assigned
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_ComplexIterType, DEM_CODE)
Dem_FreezeFrameIterator_GetJ1939SpnIter(
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId
  );


/* ****************************************************************************
 * Dem_FreezeFrameIterator_J1939SpnIter_Init()
 *****************************************************************************/
/*!
 * \brief         Init the iterator.
 *
 * \details       Initializes the iterator to iterate over SPN Ids
 *                of the J1939 expanded freeze frame
 *
 * \param[in]     IterPtr
 *                Pointer to the Iterator.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_J1939SpnIter_Init(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
  );

/* ****************************************************************************
 * Dem_Client_FreezeFrameIterator_CustomTriggerTssrIterInit()
 *****************************************************************************/
/*!
 * \brief         Init the iterator for custom trigger time series snapshot data.
 *
 * \details       Initializes the iterator over configured time series samples
 *                in the custom trigger time series record for the event
 *
 * \param[in]     IterPtr
 *                Pointer to the Iterator.
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Event should support custom triggered time series snapshot
 *                records
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Client_FreezeFrameIterator_CustomTriggerTssrIterInit(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr,
  Dem_EventIdType EventId
  );

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

# if (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF)
/* ****************************************************************************
 % Dem_FreezeFrameIterator_TimeSeriesSRecCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_TimeSeriesSRecCount(
  Dem_Cfg_EntryIndexType TimeSeriesEntryIndex
)
{
  uint8 lReturnValue = 0u;
  Dem_Cfg_TssFifoEntry_IterType lIter;
  Dem_TimeSeriesEntry_PtrToConstBaseType lTimeSeriesEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(TimeSeriesEntryIndex);
  Dem_EventIdType lEventId = lTimeSeriesEntry->EventId;

  Dem_TimeSeriesIdType lSeriesId;
  Dem_TimeSeriesIdType lNumberOfSeries = Dem_Memories_TimeSeriesMemory_GetNumberOfSeriesOfEvent(lEventId);
  for(lSeriesId = 0; lSeriesId < lNumberOfSeries; lSeriesId++)
  {
    Dem_Cfg_SamplingProfileIterType lSamplingProfileId = Dem_Memories_TimeSeriesMemory_GetSamplingProfileId(lEventId, lSeriesId);
    uint8 lSamplesPerSet = (Dem_Cfg_GetFutureSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId) + Dem_Cfg_GetPastSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId));

    DEM_IGNORE_UNUSED_VARIABLE(lSamplingProfileId);                                                                              /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
    for (Dem_Memories_TimeSeriesMemory_TssSetIter_Init(TimeSeriesEntryIndex, lSeriesId, &lIter);                                 /* SBSW_DEM_POINTER_FORWARD_STACK */
        Dem_Memories_TimeSeriesMemory_TssSetIter_Exists(&lIter);                                                                 /* SBSW_DEM_POINTER_FORWARD_STACK */
        Dem_Memories_TimeSeriesMemory_TssSetIter_Next(&lIter))                                                                   /* SBSW_DEM_POINTER_FORWARD_STACK */
    {

      lReturnValue += lSamplesPerSet
        - Dem_MemoryEntry_TimeSeriesEntry_GetUnusedPastSamples(lTimeSeriesEntry, lSeriesId, lIter.TssSetId)                      /* SBSW_DEM_POINTER_READ_TIMESERIES_BASEENTRY */
        - Dem_MemoryEntry_TimeSeriesEntry_GetUnusedFutureSamples(lTimeSeriesEntry, lSeriesId, lIter.TssSetId);                   /* SBSW_DEM_POINTER_READ_TIMESERIES_BASEENTRY */
    }
  }

  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

/* ****************************************************************************
 % Dem_FreezeFrameIterator_SetTimeSeriesPowertrainReportedState
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_SetTimeSeriesPowertrainReportedState(                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr,                                /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  boolean ReportedState
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FreezeFrameFilterPtr)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_ARGUMENT(ReportedState)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

#if DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON
    FreezeFrameFilterPtr->TimeSeriesSnapshotReported = ReportedState;                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#endif
}

/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetTimeSeriesPowertrainReportedState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_GetTimeSeriesPowertrainReportedState(                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2CONST(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FreezeFrameFilterPtr)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

#if DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON
    return FreezeFrameFilterPtr->TimeSeriesSnapshotReported;                                                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
#else
    return TRUE;
#endif
}

/* ****************************************************************************
 % Dem_FreezeFrameIterator_SetOBDonUDSFF0x00ReportedState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_SetOBDonUDSFF0x00ReportedState(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr,                                /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  boolean ReportedState
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FreezeFrameFilterPtr)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_ARGUMENT(ReportedState)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

#if DEM_CFG_SUPPORT_OBDONUDS == STD_ON
    FreezeFrameFilterPtr->ObdOnUdsImmutableReported = ReportedState;                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#endif
}

/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetOBDonUDSFF0x00ReportedState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_GetOBDonUDSFF0x00ReportedState(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2CONST(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FreezeFrameFilterPtr)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

#if DEM_CFG_SUPPORT_OBDONUDS == STD_ON
    return FreezeFrameFilterPtr->ObdOnUdsImmutableReported;
#else
    return TRUE;
#endif
}


/* ****************************************************************************
 % Dem_FreezeFrameIterator_SetObdFFMemoryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_SetObdFFMemoryIndex(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr,                                /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  uint8 ObdFFMemoryIndex
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FreezeFrameFilterPtr)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_ARGUMENT(ObdFFMemoryIndex)                                                                                   /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

#if DEM_CFG_SUPPORT_OBDONUDS == STD_ON
    FreezeFrameFilterPtr->ObdFFMemoryIndex = ObdFFMemoryIndex;                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#endif
}

/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetObdFFMemoryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetObdFFMemoryIndex(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2CONST(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(FreezeFrameFilterPtr)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

#if DEM_CFG_SUPPORT_OBDONUDS == STD_ON
    return FreezeFrameFilterPtr->ObdFFMemoryIndex;
#else
    return DEM_CFG_MAX_SIZE_OBDFREEZEFRAME;
#endif
}

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetDTCFormat
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_FreezeFrameIterator_DTCFormatType, DEM_CODE)
Dem_FreezeFrameIterator_GetDTCFormat(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  )
{
  return Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId).FilterDTCFormat;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetEventEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FreezeFrameIterator_GetEventEntryIndex(                                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  )
{
  return Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId).EventEntryIndex;
}
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetNodeIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetNodeIndex(                                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  )
{
  return Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId).NodeIndex;
}
#endif

/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetFilterState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetFilterState(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  )
{
  return Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId).FilterState;
}

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)))
/* ****************************************************************************
 % Dem_FreezeFrameIterator_NextEvent
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_NextEvent(
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr,
  CONSTP2VAR(Dem_Cfg_EntryIterType, AUTOMATIC, AUTOMATIC) EventEntryIter
  )
{
  Dem_Cfg_ComplexIter32BitNext(EventEntryIter);                                                                                  /* PRQA S 2469 */ /* MD_DEM_14.2_LoopControlHandledInternally */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
  if(Dem_Cfg_ComplexIter32BitExists(EventEntryIter) == TRUE)                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    FreezeFrameFilterPtr->EventEntryIndex = Dem_Cfg_ComplexIter32BitGet(EventEntryIter);                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_MemoryEntry_SnapshotEntry_IteratorInvalidate(&FreezeFrameFilterPtr->SnapshotEntryIter);                                  /* SBSW_DEM_POINTER_FORWARD_STACK */

# if(DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
    FreezeFrameFilterPtr->GlobalSnapshotReported = FALSE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# endif
# if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON)
    FreezeFrameFilterPtr->TimeSeriesSnapshotReported = FALSE;                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
# endif
  }
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetNextFilteredGlobalSRecNumber
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetNextFilteredGlobalSRecNumber(
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
  )
{
  uint8 lRecordNumber;
  lRecordNumber = DEM_FREEZEFRAMEITERATOR_INVALID_RECORDNUMBER;

  if (FreezeFrameFilterPtr->GlobalSnapshotReported == FALSE)
  {
    /* Global snapshot not yet reported for this event entry */
    if (Dem_MemoryEntry_EventEntry_TestGlobalSnapshotRecordStored(FreezeFrameFilterPtr->EventEntryIndex) == TRUE)
    {
      lRecordNumber = Dem_Cfg_GlobalSRecId();
      FreezeFrameFilterPtr->GlobalSnapshotReported = TRUE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
  return lRecordNumber;
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON) */

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)))
/* ****************************************************************************
 % Dem_FreezeFrameIterator_FilterSRec
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
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_FilterSRec(                                                                                              /* PRQA S 2889 */ /* MD_DEM_15.5 */
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,                                                                              /* PRQA S 3673 */ /* MD_DEM_8.13 */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  RecordNumber
  )
{
  Dem_Cfg_FreezeFrameIterator_FilterType lFreezeFrameFilterData;
  Dem_Cfg_EntryIterType lPrimaryEventEntryIter;
    
  lFreezeFrameFilterData = Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId);

  for (Dem_Cfg_ComplexIter32BitInit(&lPrimaryEventEntryIter, lFreezeFrameFilterData.EventEntryIndex, Dem_FaultMemory_Memories_GetEndEntryIndex(DEM_CFG_MEMORYID_PRIMARY)); /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ComplexIter32BitExists(&lPrimaryEventEntryIter) == TRUE;)                                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
  { 
    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lPrimaryEventEntryIter);                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
    lFreezeFrameFilterData.EventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);

    if ( (Dem_Event_TestValidHandle(lFreezeFrameFilterData.EventId) == TRUE)
      && (Dem_DTC_TestSuppressionOnDtcRetrieval(lFreezeFrameFilterData.EventId) == FALSE) )                                      /* PRQA S 3415 */ /* MD_DEM_13.5_cf */                                                            
    {
      /* Skip Events which do not have an OBD DTC but filter is set for OBD format */
      if ( (Dem_Core_DTC_IsObdRelated(lFreezeFrameFilterData.EventId) == TRUE)
        || (lFreezeFrameFilterData.FilterDTCFormat != DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD) )
      {
        /* Special case - skip Dependent OBD DTCs */
        if ((Dem_DTCReporting_TestObdHideOccurrences() == FALSE)
          || (Dem_Cfg_EventSignificance(lFreezeFrameFilterData.EventId) != DEM_CFG_EVENT_SIGNIFICANCE_OCCURRENCE)
          || (Dem_Core_DTC_IsObdRelated(lFreezeFrameFilterData.EventId) == FALSE))
        {
          uint8 lRecordNumber;
          /* Get next filtered record number */
          lRecordNumber = Dem_FreezeFrameIterator_GetNextFilteredSRecNumber(&lFreezeFrameFilterData);                            /* SBSW_DEM_POINTER_FORWARD_STACK */
          if (lRecordNumber != DEM_FREEZEFRAMEITERATOR_INVALID_RECORDNUMBER)
          {
            *RecordNumber = lRecordNumber;                                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

            /* Assign DTC in requested format */
            if (lFreezeFrameFilterData.FilterDTCFormat == DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD)
            {
              *DTC = (uint32)(((uint32)Dem_Cfg_EventObdDtc(lFreezeFrameFilterData.EventId)) << 8u);                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
            }
            else
            { /* Only EventIds with a valid DTC number are processed here */
              /* -> DTC mapping will always return a valid DTC number */
              *DTC = Dem_Cfg_EventUdsDtc(lFreezeFrameFilterData.EventId);                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
            }

            Dem_FreezeFrameIterator_SetFreezeFrameFilterData(FreezeFrameIteratorId, lFreezeFrameFilterData);
            return TRUE;
          }
        }
      }
    }
    Dem_FreezeFrameIterator_NextEvent(&lFreezeFrameFilterData, &lPrimaryEventEntryIter);                                         /* PRQA S 2469 */ /* MD_DEM_14.2_LoopControlHandledInternally */ /* SBSW_DEM_POINTER_FORWARD_STACK */ /* SBSW_DEM_CALL_ITERATOR_POINTER */

# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
    Dem_DTC_SetCombinedGroupProcessed(Dem_Cfg_EventCombinedGroup(lFreezeFrameFilterData.EventId), lFreezeFrameFilterData.ProcessedCombinedDTCGroups); /* SBSW_DEM_POINTER_FORWARD_STACK */
# endif
  }

  Dem_FreezeFrameIterator_SetFreezeFrameFilterData(FreezeFrameIteratorId, lFreezeFrameFilterData);
  return FALSE;
}                                                                                                                                /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_Client_FreezeFrameIterator_GetNextFilteredObdOnUdsFFMemoryEntry
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_FreezeFrameIterator_GetNextFilteredObdOnUdsFFMemoryEntry(                                                             /* PRQA S 2889 */ /* MD_DEM_15.5 */
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,                                                                              /* PRQA S 3673 */ /* MD_DEM_8.13 */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  RecordNumber
  )
{
  boolean lResult;

  lResult = FALSE;

  if (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE)
  {
    Dem_Cfg_FreezeFrameIterator_FilterType lFreezeFrameFilterData;
    uint8 lFreezeFrameIndex;

    lFreezeFrameFilterData = Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId);
    lFreezeFrameIndex = Dem_FreezeFrameIterator_GetObdFFMemoryIndex(&lFreezeFrameFilterData);                                    /* SBSW_DEM_POINTER_FORWARD_STACK */

    while (lFreezeFrameIndex < Dem_Memories_ObdFreezeFrameMemory_GetSize())                                                      /* PRQA S 2994 */ /* MD_DEM_2994 */
    {
      Dem_EventIdType lEventId;

      lEventId = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex);

      /* Special case - skip Dependent OBD DTCs */
      if ((Dem_Event_TestValidHandle(lEventId) == TRUE)
        && (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == FALSE)                                                            /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        && ((Dem_DTCReporting_TestObdHideOccurrences() == FALSE)                                                                 /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
          || (Dem_Cfg_EventSignificance(lEventId) != DEM_CFG_EVENT_SIGNIFICANCE_OCCURRENCE)))                                    /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      {
        lResult = TRUE;
        
        if (Dem_FreezeFrameIterator_GetOBDonUDSFF0x00ReportedState(&lFreezeFrameFilterData) == FALSE)                            /* SBSW_DEM_POINTER_FORWARD_STACK */
        {
          *RecordNumber = Dem_Memories_ObdFreezeFrameMemory_GetObdOnUdsSRecNumberByFreezeFrameType(Dem_RecordReader_0x1904_OBDonUDSFF_0x00); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
          Dem_FreezeFrameIterator_SetOBDonUDSFF0x00ReportedState(&lFreezeFrameFilterData, TRUE);                                 /* SBSW_DEM_POINTER_FORWARD_STACK */
        }
        else
        {
          *RecordNumber = Dem_Memories_ObdFreezeFrameMemory_GetObdOnUdsSRecNumberByFreezeFrameType(Dem_RecordReader_0x1904_OBDonUDSFF_0xF0); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
          Dem_FreezeFrameIterator_SetOBDonUDSFF0x00ReportedState(&lFreezeFrameFilterData, FALSE);                                /* SBSW_DEM_POINTER_FORWARD_STACK */
          lFreezeFrameIndex++;
        }

        /* EventIds are tested for a valid DTC number */
        /* -> DTC mapping will always return a valid DTC number */
        *DTC = Dem_Cfg_EventUdsDtc(lEventId);                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

        break;
      }
      
      lFreezeFrameIndex++;
    }

    Dem_FreezeFrameIterator_SetObdFFMemoryIndex(&lFreezeFrameFilterData, lFreezeFrameIndex);                                     /* SBSW_DEM_POINTER_FORWARD_STACK */
    Dem_FreezeFrameIterator_SetFreezeFrameFilterData(FreezeFrameIteratorId, lFreezeFrameFilterData);
    
  }

  return lResult;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
  && ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)))
/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetNextFilteredSRecNumber
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetNextFilteredSRecNumber(                                                                               /* PRQA S 2889 */ /* MD_DEM_15.5 */
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
  )
{
  uint8 lRecordNumber = DEM_FREEZEFRAMEITERATOR_INVALID_RECORDNUMBER;

  # if(DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
  Dem_Cfg_SRecNumerationType lSrecNumerationType = Dem_FaultMemory_Memories_GetSRecNumerationType(DEM_CFG_MEMORYID_PRIMARY);

  if (Dem_Memories_EventMemory_UserDefined_IsSRecNumerationConfigured(lSrecNumerationType) == TRUE)
  {
    /* Global snapshots for configured snapshots */
    lRecordNumber = Dem_FreezeFrameIterator_GetNextFilteredGlobalSRecNumber(FreezeFrameFilterPtr);                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  # endif

  # if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  if (lRecordNumber == DEM_FREEZEFRAMEITERATOR_INVALID_RECORDNUMBER)
  {
    lRecordNumber = Dem_FreezeFrameIterator_GetNextFilteredEventSpecificSRecNumber(FreezeFrameFilterPtr);                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  # endif

  # if(DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
  if ((Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculated(lSrecNumerationType) == TRUE)
    && (lRecordNumber == DEM_FREEZEFRAMEITERATOR_INVALID_RECORDNUMBER))
  {
    /* Global snapshots for calculated snapshots */
    lRecordNumber = Dem_FreezeFrameIterator_GetNextFilteredGlobalSRecNumber(FreezeFrameFilterPtr);                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  }
  # endif

  return lRecordNumber;
}
#endif


#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && (DEM_CFG_SUPPORT_SRECS == STD_ON) )
/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetNextFilteredEventSpecificSRecNumber
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_GetNextFilteredEventSpecificSRecNumber(                                                                  /* PRQA S 2889 */ /* MD_DEM_15.5 */
  CONSTP2VAR(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC, AUTOMATIC)  FreezeFrameFilterPtr
)
{
  uint8 lRecordNumber;
  lRecordNumber = DEM_FREEZEFRAMEITERATOR_INVALID_RECORDNUMBER;

  #  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  if (Dem_DTC_TestCombinedGroupProcessed(Dem_Cfg_EventCombinedGroup(FreezeFrameFilterPtr->EventId),                              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    FreezeFrameFilterPtr->ProcessedCombinedDTCGroups) == FALSE)
  #  endif
  {
    if (Dem_MemoryEntry_SnapshotEntry_SearchStoredEntry(FreezeFrameFilterPtr->EventId, 
                   FreezeFrameFilterPtr->EventEntryIndex, &FreezeFrameFilterPtr->SnapshotEntryIter) == TRUE)                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    {
      lRecordNumber = Dem_MemoryEntry_SnapshotEntry_IteratorGetStoredRecordNumber(&FreezeFrameFilterPtr->SnapshotEntryIter);     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

      if ((lRecordNumber > DEM_FREEZEFRANEITERATOR_TIMESERIES_POWERTRAIN_RECORDNUMBER)
        && (Dem_Memories_TimeSeriesMemory_IsTimeSeriesPowertrainStoredByEvent(FreezeFrameFilterPtr->EventId) == TRUE)            /* PRQA S 3415 */ /* MD_DEM_13.5_cf */ 
        && (Dem_FreezeFrameIterator_GetTimeSeriesPowertrainReportedState(FreezeFrameFilterPtr) == FALSE)                         /* PRQA S 3415 */ /* MD_DEM_13.5_cf */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        )
      {
        lRecordNumber = DEM_FREEZEFRANEITERATOR_TIMESERIES_POWERTRAIN_RECORDNUMBER;
        Dem_FreezeFrameIterator_SetTimeSeriesPowertrainReportedState(FreezeFrameFilterPtr, TRUE);                                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      }
      else
      {
        /* Increment in global state, the next iteration will continue with the next record */
#  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
        Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorNext(&FreezeFrameFilterPtr->SnapshotEntryIter);              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#  else
        Dem_MemoryEntry_SnapshotEntry_IteratorNext(&FreezeFrameFilterPtr->SnapshotEntryIter);                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#  endif
      }
    }
    else if ((Dem_Memories_TimeSeriesMemory_IsTimeSeriesPowertrainStoredByEvent(FreezeFrameFilterPtr->EventId) == TRUE)
          && (Dem_FreezeFrameIterator_GetTimeSeriesPowertrainReportedState(FreezeFrameFilterPtr) == FALSE)                       /* PRQA S 3415 */ /* MD_DEM_13.5_cf */ /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      )
    {
      lRecordNumber = DEM_FREEZEFRANEITERATOR_TIMESERIES_POWERTRAIN_RECORDNUMBER;
      Dem_FreezeFrameIterator_SetTimeSeriesPowertrainReportedState(FreezeFrameFilterPtr, TRUE);                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      /* MISRA Case */
    }
  }

  return lRecordNumber;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

# if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF))
/* ****************************************************************************
 % Dem_FreezeFrameIterator_FilterTimeSeriesSRec
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_FilterTimeSeriesSRec(                                                                                    /* PRQA S 2889 */ /* MD_DEM_15.5 */
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,                                                                              /* PRQA S 3673 */ /* MD_DEM_8.13 */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  RecordNumber
)
{
  boolean lStoredRecordFound = FALSE; 
  uint16 lTranslatedMemoryId = Dem_Memories_TimeSeriesMemory_GetPrimaryTranslatedMemoryId();
  if (lTranslatedMemoryId != DEM_CFG_MEMORYID_INVALID)
  {
    Dem_Cfg_FreezeFrameIterator_FilterType lFreezeFrameFilterData;
    Dem_Cfg_EntryIterType lEntryIter;
 
    lFreezeFrameFilterData = Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId);

    for (Dem_Cfg_ComplexIter32BitInit(&lEntryIter, lFreezeFrameFilterData.TimeSeriesEntryIndex, Dem_FaultMemory_Memories_GetEndEntryIndex(lTranslatedMemoryId)); /* SBSW_DEM_CALL_ITERATOR_POINTER */
         (Dem_Cfg_ComplexIter32BitExists(&lEntryIter) == TRUE);                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_ComplexIter32BitNext(&lEntryIter))                                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_TimeSeriesEntry_PtrToConstBaseType lTimeSeriesEntry = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(Dem_Cfg_ComplexIter32BitGet(&lEntryIter)); /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_EventIdType lEventId = lTimeSeriesEntry->EventId;
      lFreezeFrameFilterData.TimeSeriesEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lEntryIter);                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if ((Dem_Event_TestValidHandle(lEventId) == TRUE)
        && (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == FALSE))                                                           /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      {
        /* suppress record for OBD related events if the confirmed bit is not qualified yet.
        UDS only events are not taken into account or OBD related events which have a qualified DCY */

        /* Skip Events which do not have an OBD DTC but filter is set for OBD format */
        if ((Dem_Core_DTC_IsObdRelated(lEventId) == TRUE)
          || (lFreezeFrameFilterData.FilterDTCFormat != DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD))
        {
          /* Special case - skip Dependent OBD DTCs */
          if ((Dem_DTCReporting_TestObdHideOccurrences() == FALSE)
            || (Dem_Cfg_EventSignificance(lEventId) != DEM_CFG_EVENT_SIGNIFICANCE_OCCURRENCE)
            || (Dem_Core_DTC_IsObdRelated(lEventId) == FALSE))
          {
            Dem_TimeSeriesRecordIterator lIter;
            for(lIter = Dem_Memories_TimeSeriesMemory_InitializeTimeSeriesRecordIteratorByRecordNumber(lEventId, lFreezeFrameFilterData.TimeSeriesRecordNumber);
                (Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorIsInvalid(&lIter) == FALSE);                              /* SBSW_DEM_POINTER_FORWARD_STACK */
                Dem_Memories_TimeSeriesMemory_TimeSeriesRecordIteratorNextRecordNumberInAnySeriesOfEvent(&lIter))                /* PRQA S 2469 */ /* MD_DEM_14.2_LoopControlHandledInternally */ /* SBSW_DEM_POINTER_FORWARD_STACK */
            {
              if(Dem_Memories_TimeSeriesMemory_TestSRecIsStored(lFreezeFrameFilterData.TimeSeriesEntryIndex, &lIter))            /* SBSW_DEM_POINTER_FORWARD_STACK */
              {
                lStoredRecordFound = TRUE;
                break;
              }
            }

            if (lStoredRecordFound == TRUE)
            { /* If we found a stored snapshot */

              /* Record number is stored and in this TSS set, return values */
              if (lFreezeFrameFilterData.FilterDTCFormat == DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD)
              {
                *DTC = (uint32)(((uint32)Dem_Cfg_EventObdDtc(lEventId)) << 8u);                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
              }
              else
              { /* Only EventIds with a valid DTC number are processed here */
                /* -> DTC mapping will always return a valid DTC number */
                *DTC = Dem_Cfg_EventUdsDtc(lEventId);                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
              }

              *RecordNumber = lIter.RecordNumber;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

              /* Increment in global state, the next iteration will continue with the next record */
              lFreezeFrameFilterData.TimeSeriesRecordNumber = lIter.RecordNumber + 1u;

              break;
            }
          }
        }
      }
      lFreezeFrameFilterData.TimeSeriesRecordNumber = 0x00;
    }

    if(lStoredRecordFound == FALSE)
    {
      Dem_Cfg_ComplexIter32BitNext(&lEntryIter);                                                                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
      lFreezeFrameFilterData.TimeSeriesEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lEntryIter);                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
    }
    Dem_FreezeFrameIterator_SetFreezeFrameFilterData(FreezeFrameIteratorId, lFreezeFrameFilterData);
  }

  return lStoredRecordFound == TRUE;
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
# endif

/* ****************************************************************************
 % Dem_Client_FreezeFrameIterator_FilterCustomTriggerRecord
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_FreezeFrameIterator_FilterCustomTriggerRecord(                                                                        /* PRQA S 2889, 3219 */ /* MD_DEM_15.5, MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)  DTC,                                                                              /* PRQA S 3673 */ /* MD_DEM_8.13 */
  CONSTP2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)  RecordNumber                                                                       /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
)
{
  boolean lReturn = FALSE;
#if(DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  uint16 lTranslatedMemoryId = Dem_Memories_EventMemory_GetReferencedCustomTriggerMemoryId(DEM_CFG_MEMORYID_PRIMARY);
  if (lTranslatedMemoryId != DEM_CFG_MEMORYID_INVALID)
  {
    Dem_Cfg_FreezeFrameIterator_FilterType lFreezeFrameFilterData;
    Dem_Cfg_EntryIterType lEntryIter;
 
    lFreezeFrameFilterData = Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId);

    /* Iterate over all custom trigger entries assigned to primary memory */
    for (Dem_Cfg_ComplexIter32BitInit(&lEntryIter, lFreezeFrameFilterData.CustomTriggerEntryIndex, Dem_FaultMemory_Memories_GetEndEntryIndex(lTranslatedMemoryId)); /* PRQA S 0771 */ /* MD_DEM_15.4_opt */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_ComplexIter32BitExists(&lEntryIter) == TRUE;)                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_Cfg_EntryIndexType lCustomTriggerEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lEntryIter);                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_EventIdType lEventId = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lCustomTriggerEntryIndex)->EventId;

      if ((Dem_Event_TestValidHandle(lEventId) == TRUE)
        && (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == FALSE))                                                           /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      {
        /* Skip Events which do not have an OBD DTC but filter is set for OBD format */
        if ((Dem_Core_DTC_IsObdRelated(lEventId) == TRUE)
          || (lFreezeFrameFilterData.FilterDTCFormat != DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD))
        {
          /* Special case - skip Dependent OBD DTCs */
          if ((Dem_DTCReporting_TestObdHideOccurrences() == FALSE)
            || (Dem_Cfg_EventSignificance(lEventId) != DEM_CFG_EVENT_SIGNIFICANCE_OCCURRENCE)
            || (Dem_Core_DTC_IsObdRelated(lEventId) == FALSE))
          {
            uint8 lRecordNumber = DEM_CUSTOMTRIGGERMEMORY_SREC_NUMBER_INVALID;
            if ((Dem_Core_TestCustomTriggeredTssrConfigured(lEventId) == TRUE) && (lFreezeFrameFilterData.CustomTriggerEntryReadStatus == FALSE))
            {
              Dem_Client_FreezeFrameIterator_CustomTriggerTssrIterInit(&lFreezeFrameFilterData.CustomTriggerTssrIter, lEventId); /* SBSW_DEM_POINTER_FORWARD_STACK */
            }

            if (   (Dem_Core_TestCustomTriggeredSrecConfigured(lEventId) == TRUE)
                && (lFreezeFrameFilterData.CustomTriggerEntryReadStatus == FALSE))
            {
              lRecordNumber = Dem_Memories_CustomTriggerMemory_GetSRecNumber(lEventId);                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
              lReturn = TRUE;
            }
            else if (Dem_Cfg_ComplexIterExists(&lFreezeFrameFilterData.CustomTriggerTssrIter) == TRUE)                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
            {
              /* Next valid record is custom Tssr */
              lRecordNumber = (uint8)Dem_Cfg_ComplexIterGet(&lFreezeFrameFilterData.CustomTriggerTssrIter);                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
              if (Dem_Memories_CustomTriggerMemory_TestRecordIsStored(lFreezeFrameFilterData.CustomTriggerEntryIndex, lRecordNumber))
              {
                lReturn = TRUE;
              }
              /* Increment custom Time series iterator */
              Dem_Cfg_ComplexIterNext(&lFreezeFrameFilterData.CustomTriggerTssrIter);                                            /* PRQA S 2469 */ /* MD_DEM_14.2_LoopControlHandledInternally */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
            }
            else
            {
              /* All valid custom records for current custom entry has been reported */
            }

            /* Set reporting is in progress flag for current entry index */
            lFreezeFrameFilterData.CustomTriggerEntryReadStatus = TRUE;

            if (lReturn == TRUE)
            {
              /* Copy DTC and record number and return TRUE */
              if (lFreezeFrameFilterData.FilterDTCFormat == DEM_FREEZEFRAMEITERATOR_DTCFORMAT_OBD)
              {
                *DTC = (uint32)(((uint32)Dem_Cfg_EventObdDtc(lEventId)) << 8u);                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
              }
              else
              {
                *DTC = Dem_Cfg_EventUdsDtc(lEventId);                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
              }

              *RecordNumber = lRecordNumber;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
              break;
            }

            if (Dem_Cfg_ComplexIterExists(&lFreezeFrameFilterData.CustomTriggerTssrIter) == TRUE)                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
            {
              /* Custom Trigger Time series iterator is in progress do not increment entry iterator */
              continue;
            }
          }
        }
      }

      /* Once current entry has been fully reported, increment entry iterator and reset custom time series iterator, reporting is in progress flag */
      Dem_Cfg_ComplexIter32BitNext(&lEntryIter);                                                                                 /* PRQA S 2469 */ /* MD_DEM_14.2_LoopControlHandledInternally */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterInit(&lFreezeFrameFilterData.CustomTriggerTssrIter, 0u, 0u);                                            /* SBSW_DEM_POINTER_FORWARD_STACK */
      lFreezeFrameFilterData.CustomTriggerEntryIndex = (Dem_Cfg_EntryIndexType)(Dem_Cfg_ComplexIter32BitGet(&lEntryIter));       /* SBSW_DEM_CALL_ITERATOR_POINTER */
      lFreezeFrameFilterData.CustomTriggerEntryReadStatus = FALSE;
    }
    Dem_FreezeFrameIterator_SetFreezeFrameFilterData(FreezeFrameIteratorId, lFreezeFrameFilterData);
  }
#else
   DEM_IGNORE_UNUSED_CONST_ARGUMENT(FreezeFrameIteratorId)                                                                       /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
   DEM_IGNORE_UNUSED_CONST_ARGUMENT(DTC)                                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
   DEM_IGNORE_UNUSED_CONST_ARGUMENT(RecordNumber)                                                                                /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
  return lReturn;
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */


#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) \
   && ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)))
/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetNumberOfSRec
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
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FreezeFrameIterator_GetNumberOfSRec(
  void
)
{
  uint16 lRecordCount = 0;
  Dem_Cfg_EntryIterType lPrimaryEventEntryIter;
  /* Count stored snapshot records in primary memory to prevent concurrent modification
     of the chrono index to have an effect on the iteration */

# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  Dem_Cfg_ProcessedCombinedDTCGroupType lProcessedCombinedDTCGroups;
  Dem_DTC_InitCombinedGroupProcessed(lProcessedCombinedDTCGroups);                                                               /* SBSW_DEM_POINTER_FORWARD_STACK */
# endif

  for (Dem_Cfg_ComplexIter32BitInit(&lPrimaryEventEntryIter,                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_FaultMemory_Memories_GetFirstEntryIndex(DEM_CFG_MEMORYID_PRIMARY),
         Dem_FaultMemory_Memories_GetEndEntryIndex(DEM_CFG_MEMORYID_PRIMARY));    Dem_Cfg_ComplexIter32BitExists(&lPrimaryEventEntryIter) == TRUE; /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIter32BitNext(&lPrimaryEventEntryIter))                                                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lPrimaryEventEntryIter);                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_EventIdType lEventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);
    
    if ((Dem_Event_TestValidHandle(lEventId) == TRUE)
      && (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == FALSE))                                                             /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      if ((Dem_DTCReporting_TestObdHideOccurrences() == FALSE)
        || (Dem_Cfg_EventSignificance(lEventId) != DEM_CFG_EVENT_SIGNIFICANCE_OCCURRENCE)
        || (Dem_Core_DTC_IsObdRelated(lEventId) == FALSE))
      {
        /* Occupied Entry */
# if (DEM_CFG_SUPPORT_SRECS == STD_ON)
#  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
        if (Dem_Cfg_EventIsCombined(lEventId) == TRUE)
        {
          Dem_Cfg_CombinedGroupIndexType lGroupId;
          lGroupId = Dem_Cfg_EventCombinedGroup(lEventId);

          if (Dem_DTC_TestCombinedGroupProcessed(lGroupId, lProcessedCombinedDTCGroups) == FALSE)                                /* SBSW_DEM_POINTER_FORWARD_STACK */
          {
            Dem_DTC_SetCombinedGroupProcessed(lGroupId, lProcessedCombinedDTCGroups);                                            /* SBSW_DEM_POINTER_FORWARD_STACK */
            lRecordCount = (uint16)(lRecordCount +                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
              Dem_MemoryEntry_SnapshotEntry_GetNumberOfStoredCombinedSnapshotRecords(lPrimaryEventEntryIter, lGroupId));
          }
        }
        else
#  endif
        {
          lRecordCount = (uint16)(lRecordCount + Dem_MemoryEntry_SnapshotEntry_GetNumberOfStoredSnapshotRecords(lEventEntryIndex));
        }
# endif

        if (Dem_Memories_TimeSeriesMemory_IsPowertrainEnabled() == TRUE)
        {
          if ((Dem_Memories_TimeSeriesMemory_TestEventSupportTimeSeries(lEventId) == TRUE)                                       /* PRQA S 3415 */ /* MD_DEM_13.5_cf */ 
            && (Dem_Memories_TimeSeriesMemory_FindEntryIndex(lEventId) != DEM_CFG_ENTRYINDEX_INVALID))                           /* PRQA S 3415 */ /* MD_DEM_13.5_cf */ 
          {
            lRecordCount = lRecordCount + 1u;
          }
        }

# if (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
        if (Dem_MemoryEntry_EventEntry_TestGlobalSnapshotRecordStored(lEventEntryIndex) == TRUE)
        {
          lRecordCount++;
        }
# endif
      }
    }

  }
  return lRecordCount;
}                                                                                                                                 /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
% Dem_Client_FreezeFrameIterator_GetNumberOfObdOnUdsSRec
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
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_FreezeFrameIterator_GetNumberOfObdOnUdsSRec(
  void
)
{
  uint16 lRecordCount;
  lRecordCount = 0;

  if (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE)
  {
    uint8 lFreezeFrameIndex;
    Dem_EventIdType lEventId;

    lFreezeFrameIndex = Dem_Memories_ObdFreezeFrameMemory_GetSize();
    while (lFreezeFrameIndex != 0U)                                                                                              /* PRQA S 2994 */ /* MD_DEM_2994 */
    {
      lFreezeFrameIndex--;
      lEventId = Dem_MemoryEntry_ObdFreezeFrameEntry_GetEventId(lFreezeFrameIndex);
      
      if ((Dem_Event_TestValidHandle(lEventId) == TRUE)
        && (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == FALSE)                                                            /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        && ((Dem_DTCReporting_TestObdHideOccurrences() == FALSE)                                                                 /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
          || (Dem_Cfg_EventSignificance(lEventId) != DEM_CFG_EVENT_SIGNIFICANCE_OCCURRENCE)))                                    /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      {
        lRecordCount = lRecordCount + 2u; /* An OBD on UDS FF entry always contains two FFs */
      }
    }
  }

  return lRecordCount;
}                                                                                                                               
#endif

# if (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF)
/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetNumberOfTimeSeriesSRec
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FreezeFrameIterator_GetNumberOfTimeSeriesSRec(
  void
  )
{
  uint16 lRecordCount = 0u;
  uint16 lTranslatedMemoryId = Dem_Memories_TimeSeriesMemory_GetPrimaryTranslatedMemoryId();

  if (lTranslatedMemoryId != DEM_CFG_MEMORYID_INVALID)
  {
    Dem_Cfg_EntryIterType lPrimaryTimeSeriesEntryIter;
    for (Dem_Cfg_ComplexIter32BitInit(&lPrimaryTimeSeriesEntryIter,
                                   Dem_FaultMemory_Memories_GetFirstEntryIndex(lTranslatedMemoryId),
                                   Dem_FaultMemory_Memories_GetEndEntryIndex(lTranslatedMemoryId));                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIter32BitExists(&lPrimaryTimeSeriesEntryIter) == TRUE;                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIter32BitNext(&lPrimaryTimeSeriesEntryIter))                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_Cfg_EntryIndexType lTimeSeriesEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lPrimaryTimeSeriesEntryIter);                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_EventIdType lEventId = Dem_MemoryEntry_TimeSeriesEntry_GetBaseEntryPtrSafe(lTimeSeriesEntryIndex)->EventId;

      if ((Dem_Event_TestValidHandle(lEventId) == TRUE)
        && (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == FALSE))                                                           /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      {
        if ((Dem_DTCReporting_TestObdHideOccurrences() == FALSE)
          || (Dem_Cfg_EventSignificance(lEventId) != DEM_CFG_EVENT_SIGNIFICANCE_OCCURRENCE)                                      /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
          || (Dem_Core_DTC_IsObdRelated(lEventId) == FALSE))                                                                     /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        {
          /* Occupied entry */
          lRecordCount = (uint16)(lRecordCount + Dem_FreezeFrameIterator_TimeSeriesSRecCount(lTimeSeriesEntryIndex));
        }
      }
    }
  }

  return lRecordCount;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
# endif

/* ****************************************************************************
 % Dem_Client_FreezeFrameIterator_GetNumberOfCustomTriggerRecords
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
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_FreezeFrameIterator_GetNumberOfCustomTriggerRecords(                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  uint16 lRecordCount = 0u;
#if(DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  uint16 lTranslatedMemoryId = Dem_Memories_EventMemory_GetReferencedCustomTriggerMemoryId(DEM_CFG_MEMORYID_PRIMARY);

  if (lTranslatedMemoryId != DEM_CFG_MEMORYID_INVALID)
  {
    Dem_Cfg_EntryIterType lPrimaryCustomTriggerEntryIter;
    for (Dem_Cfg_ComplexIter32BitInit(&lPrimaryCustomTriggerEntryIter,
                                   Dem_FaultMemory_Memories_GetFirstEntryIndex(lTranslatedMemoryId),
                                   Dem_FaultMemory_Memories_GetEndEntryIndex(lTranslatedMemoryId));                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIter32BitExists(&lPrimaryCustomTriggerEntryIter) == TRUE;                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIter32BitNext(&lPrimaryCustomTriggerEntryIter))                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_Cfg_EntryIndexType lCustomTriggerEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lPrimaryCustomTriggerEntryIter);            /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_EventIdType lEventId = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lCustomTriggerEntryIndex)->EventId;

      if ((Dem_Event_TestValidHandle(lEventId) == TRUE)
        && (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == FALSE))                                                           /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
      {
        if ((Dem_DTCReporting_TestObdHideOccurrences() == FALSE)
          || (Dem_Cfg_EventSignificance(lEventId) != DEM_CFG_EVENT_SIGNIFICANCE_OCCURRENCE)                                      /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
          || (Dem_Core_DTC_IsObdRelated(lEventId) == FALSE))                                                                     /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
        {
          if (Dem_Core_TestCustomTriggeredSrecConfigured(lEventId) == TRUE)
          {
            lRecordCount ++;
          }
# if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
          if (Dem_Core_TestCustomTriggeredTssrConfigured(lEventId) == TRUE)
          {
            uint8 lStoredPastSamples = Dem_Memories_CustomTriggerMemory_NumPastTssSamples(lEventId)
              - Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lCustomTriggerEntryIndex)->UnusedPastSamples;
            uint8 lStoredFutureSamples = Dem_Memories_CustomTriggerMemory_NumFutureTssSamples(lEventId)
              - Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lCustomTriggerEntryIndex)->UnusedFutureSamples;
            lRecordCount += (uint16)lStoredPastSamples + (uint16)lStoredFutureSamples;
          }
# endif
        }
      }
    }
  }
#endif

  return lRecordCount;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetJ1939SpnIter()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_ComplexIterType, DEM_CODE)
Dem_FreezeFrameIterator_GetJ1939SpnIter(
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId
  )
{
  return Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId).SpnIter;
}

/* ****************************************************************************
 % Dem_FreezeFrameIterator_SetJ1939SpnIter()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_SetJ1939SpnIter(
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId,
  Dem_Cfg_ComplexIterType SpnIter
  )
{
  Dem_Cfg_FreezeFrameIterator_FilterType lFreezeFrameFilterData;
  lFreezeFrameFilterData = Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId);
  lFreezeFrameFilterData.SpnIter = SpnIter;

  Dem_FreezeFrameIterator_SetFreezeFrameFilterData(FreezeFrameIteratorId, lFreezeFrameFilterData);
}

/* ****************************************************************************
 % Dem_FreezeFrameIterator_J1939SpnIter_Init()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_J1939SpnIter_Init(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
)
{
  IterPtr->mIdx = 0u;                                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Cfg_GetSizeOfJ1939SpnsInExpandedFreezeFrameTable();                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Client_FreezeFrameIterator_CustomTriggerTssrIterInit()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Client_FreezeFrameIterator_CustomTriggerTssrIterInit(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr,
  Dem_EventIdType EventId
)
{
#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
  IterPtr->mIdx = Dem_Memories_CustomTriggerMemory_FirstTSSRecNum(EventId);                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = (uint16_least)Dem_Memories_CustomTriggerMemory_LastTSSRecNum(EventId) + 1u;                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#else
  IterPtr->mIdx = 0u;                                                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = 0u;                                                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
}

/* ****************************************************************************
 % Dem_FreezeFrameIterator_J1939SpnIter_GetSpnDataLength()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FreezeFrameIterator_J1939SpnIter_GetSpnDataLength(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)
  return (uint8)Dem_Cfg_GetSpnSizeOfJ1939SpnsInExpandedFreezeFrameTable(IterPtr->mIdx);
}

/* ****************************************************************************
 % Dem_FreezeFrameIterator_J1939SpnIter_GetSpnId()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_FreezeFrameIterator_J1939SpnIter_GetSpnId(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC) IterPtr
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)
  return (uint32)Dem_Cfg_GetSpnIdOfJ1939SpnsInExpandedFreezeFrameTable(IterPtr->mIdx);
}

/*!
 * \}
 */
# define DEM_STOP_SEC_CODE
# include "Dem_MemMap.h"                                                                                                         /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
# define DEM_START_SEC_CODE
# include "Dem_MemMap.h"                                                                                                         /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_FreezeFrameIterator_Public
 * \{
 */

#if ( (DEM_CFG_SUPPORT_DCM == STD_ON) || (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON) )
/* ****************************************************************************
 % Dem_FreezeFrameIterator_InitDefault
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_InitDefault(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  )
{
  Dem_Cfg_FreezeFrameIterator_FilterType lFreezeFrameFilterData;
  lFreezeFrameFilterData = Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId);

# if (DEM_CFG_SUPPORT_DCM == STD_ON)
  lFreezeFrameFilterData.FilterDTCFormat = DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT;
# endif
# if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
  lFreezeFrameFilterData.FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_INIT;
# endif

  Dem_FreezeFrameIterator_SetFreezeFrameFilterData(FreezeFrameIteratorId, lFreezeFrameFilterData);
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_FreezeFrameIterator_TestFilterSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_TestFilterSet(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  )
{
  return (boolean)(Dem_FreezeFrameIterator_GetDTCFormat(FreezeFrameIteratorId) != DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT);
}
#endif

#if (DEM_CFG_SUPPORT_J1939 == STD_ON)
/* ****************************************************************************
 % Dem_FreezeFrameIterator_TestJ1939FreezeFrameFilterSet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_TestJ1939FreezeFrameFilterSet(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  )
{
  return (boolean)(Dem_FreezeFrameIterator_GetFilterState(FreezeFrameIteratorId) != DEM_FREEZEFRAMEITERATOR_J1939STATE_INIT);
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 % Dem_FreezeFrameIterator_StartIterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_StartIterator(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  CONST(Dem_FreezeFrameIterator_DTCFormatType, AUTOMATIC)  FilterDTCFormat
  )
{
  Dem_Cfg_FreezeFrameIterator_FilterType lFreezeFrameFilterData;

  lFreezeFrameFilterData = Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId);
  Dem_MemoryEntry_SnapshotEntry_IteratorInvalidate(&lFreezeFrameFilterData.SnapshotEntryIter);                                   /* SBSW_DEM_POINTER_FORWARD_STACK */
  lFreezeFrameFilterData.EventEntryIndex = 0u;
  lFreezeFrameFilterData.TimeSeriesEntryIndex = 0u;
  lFreezeFrameFilterData.CustomTriggerEntryIndex = 0u;
  lFreezeFrameFilterData.FilterDTCFormat = FilterDTCFormat;

# if(DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
  lFreezeFrameFilterData.GlobalSnapshotReported = FALSE;
# endif
# if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
#  if DEM_CFG_SUPPORT_TIME_SERIES_POWERTRAIN == STD_ON
   lFreezeFrameFilterData.TimeSeriesSnapshotReported = FALSE;
#  endif
  lFreezeFrameFilterData.TimeSeriesRecordNumber = 0u;
# endif

# if(DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  lFreezeFrameFilterData.CustomTriggerEntryReadStatus = FALSE;
  Dem_Cfg_ComplexIterInit(&lFreezeFrameFilterData.CustomTriggerTssrIter, 0u, 0u);                                                /* SBSW_DEM_POINTER_FORWARD_STACK */
# endif

# if (DEM_CFG_SUPPORT_J1939 == STD_ON)
  lFreezeFrameFilterData.FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_INIT;
# endif
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
  Dem_DTC_InitCombinedGroupProcessed(lFreezeFrameFilterData.ProcessedCombinedDTCGroups);                                         /* SBSW_DEM_POINTER_FORWARD_STACK */
# endif
# if (DEM_CFG_SUPPORT_OBDONUDS == STD_ON)
  {
    lFreezeFrameFilterData.ObdOnUdsImmutableReported  = FALSE;
    lFreezeFrameFilterData.ObdFFMemoryIndex = 0u;
  }
# endif
  Dem_FreezeFrameIterator_SetFreezeFrameFilterData(FreezeFrameIteratorId, lFreezeFrameFilterData);
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON)
/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetNext
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FreezeFrameIterator_GetNext(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)    FreezeFrameIteratorId,
  P2VAR(uint32, AUTOMATIC, DEM_DCM_DATA)                  DTC,                                                                   /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13 */
  P2VAR(uint8, AUTOMATIC, DEM_DCM_DATA)                   RecordNumber                                                           /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13 */
  )
{
  boolean lReturnValue;
  lReturnValue = FALSE;

# if ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON))
  if (Dem_FreezeFrameIterator_FilterSRec(FreezeFrameIteratorId, DTC, RecordNumber) == TRUE)                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    lReturnValue = TRUE;
  }
  else
# endif
  if (Dem_Client_FreezeFrameIterator_GetNextFilteredObdOnUdsFFMemoryEntry(FreezeFrameIteratorId, DTC, RecordNumber) == TRUE)     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    lReturnValue = TRUE;
  }
  else
# if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF))
  if (Dem_FreezeFrameIterator_FilterTimeSeriesSRec(FreezeFrameIteratorId, DTC, RecordNumber) == TRUE)                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    lReturnValue = TRUE;
  }
  else
# endif
  if (Dem_Client_FreezeFrameIterator_FilterCustomTriggerRecord(FreezeFrameIteratorId, DTC, RecordNumber) == TRUE)                /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    lReturnValue = TRUE;
  }
  else
  {
    /* MISRA CASE */
  }

  return lReturnValue;
}
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetNumberOfFreezeFrameRecords
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_FreezeFrameIterator_GetNumberOfFreezeFrameRecords(
  void
  )
{
  uint16 lRecordCount;
  lRecordCount = 0;
  
  /* Count stored snapshot records */
#if ((DEM_CFG_SUPPORT_SRECS == STD_ON) || (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON))
  lRecordCount = (uint16)(lRecordCount + Dem_FreezeFrameIterator_GetNumberOfSRec());
#endif
  lRecordCount = (uint16)(lRecordCount + Dem_Client_FreezeFrameIterator_GetNumberOfObdOnUdsSRec());
# if ((DEM_FEATURE_NEED_TIME_SERIES == STD_ON) && (DEM_CFG_SUPPORT_POWERTRAIN == STD_OFF))
  lRecordCount = (uint16)(lRecordCount + Dem_FreezeFrameIterator_GetNumberOfTimeSeriesSRec());
# endif
  lRecordCount = (uint16)(lRecordCount + Dem_Client_FreezeFrameIterator_GetNumberOfCustomTriggerRecords());
  return lRecordCount;
}
#endif

/* ****************************************************************************
 % Dem_FreezeFrameIterator_StartJ1939Iterator
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FreezeFrameIterator_StartJ1939Iterator(                                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  CONST(Dem_J1939DcmSetFreezeFrameFilterType, AUTOMATIC)  FreezeFrameKind,
  CONST(uint8, AUTOMATIC)  NodeIndex
  )
{
  Dem_Cfg_FreezeFrameIterator_FilterType lFreezeFrameFilterData;
  lFreezeFrameFilterData = Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId);

  lFreezeFrameFilterData.FreezeFrameKind = FreezeFrameKind;
  lFreezeFrameFilterData.EventId = Dem_FaultMemory_Memories_GetFirstEvent(DEM_CFG_MEMORYID_PRIMARY);
  lFreezeFrameFilterData.NodeIndex = NodeIndex;
  lFreezeFrameFilterData.FilterState = DEM_FREEZEFRAMEITERATOR_J1939STATE_USED;
  lFreezeFrameFilterData.EventEntryIndex = 0u;
  lFreezeFrameFilterData.TimeSeriesEntryIndex = 0u;
# if (DEM_CFG_SUPPORT_DCM == STD_ON)
  lFreezeFrameFilterData.FilterDTCFormat = DEM_FREEZEFRAMEITERATOR_DTCFORMAT_INIT;
# endif

  if (FreezeFrameKind == DEM_J1939DCM_SPNS_IN_EXPANDED_FREEZEFRAME)
  {
    /* Initialize the J1939 SPN iterator */
    Dem_FreezeFrameIterator_J1939SpnIter_Init(&(lFreezeFrameFilterData.SpnIter));                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
  }

  Dem_FreezeFrameIterator_SetFreezeFrameFilterData(FreezeFrameIteratorId, lFreezeFrameFilterData);
}

#if (DEM_FEATURE_NEED_J1939_FREEZEFRAME == STD_ON)
/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetNextJ1939FreezeFrame
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FreezeFrameIterator_GetNextJ1939FreezeFrame(
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  J1939DTC,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  OccurrenceCounter,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  DestBuffer,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  BufSize 
  )
{
  Dem_Cfg_FreezeFrameIterator_FilterType lFreezeFrameFilterData;
  Dem_Cfg_EntryIterType lPrimaryEventsIter;

  Std_ReturnType lReturnValue = DEM_NO_SUCH_ELEMENT;
  
  lFreezeFrameFilterData = Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId);
  
  for (Dem_Cfg_ComplexIter32BitInit(&lPrimaryEventsIter, lFreezeFrameFilterData.EventEntryIndex, Dem_FaultMemory_Memories_GetEndEntryIndex(DEM_CFG_MEMORYID_PRIMARY)); /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ComplexIter32BitExists(&lPrimaryEventsIter) == TRUE;                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Cfg_ComplexIter32BitNext(&lPrimaryEventsIter))                                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lPrimaryEventsIter);                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_EventIdType lEventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);

    if ((Dem_Event_TestValidHandle(lEventId) == FALSE)
        || (Dem_DTC_TestSuppressionOnDtcRetrieval(lEventId) == TRUE))                                                            /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      continue;
    }

    if (((lFreezeFrameFilterData.FreezeFrameKind == DEM_J1939DCM_FREEZEFRAME) && (Dem_Cfg_EventJ1939FFExists(lEventId) == FALSE))
      || ((lFreezeFrameFilterData.FreezeFrameKind == DEM_J1939DCM_EXPANDED_FREEZEFRAME) && (Dem_Cfg_EventJ1939ExpFFExists(lEventId) == FALSE)))
    {
      continue;
    }

    if (Dem_Client_ClientAccess_IsEventAssignedToNode(lEventId, 
          Dem_FreezeFrameIterator_GetNodeIndex(FreezeFrameIteratorId)) == FALSE)
    {
      continue;
    }

    if (Dem_MemoryEntry_EventEntry_TestJ1939FFStored(lEventEntryIndex) == FALSE)
    {
      continue;
    }

    /* Found a candidate, copy data */
    {
      boolean lModificationDetected = FALSE;
      uint8 lUpdateState = Dem_Event_GetDataCommitState(lEventId);

      /* check for concurrent access to the event entry */
      if (Dem_Event_TestDataUpdateInProgress(lUpdateState) == FALSE)
      {
        uint8  lSize = 0u;
        uint16 lSourceOffset = 0u;
        Dem_ConstSharedDataPtrType lFreezeFramePtr = Dem_MemoryEntry_EventEntry_GetJ1939FreezeFramePtr(lEventEntryIndex);

        /* set the source buffer offset and the size to copy */
        if (lFreezeFrameFilterData.FreezeFrameKind == DEM_J1939DCM_FREEZEFRAME)
        {
          lSize = (uint8)Dem_Cfg_GlobalJ1939FFRawSize();
        }
        else
        {
# if (DEM_CFG_SUPPORT_J1939_FREEZEFRAME == STD_ON) && (DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME == STD_ON)
          lSourceOffset = Dem_Cfg_GlobalJ1939FFRawSize();
# endif
          lSize = (uint8)Dem_Cfg_GlobalJ1939ExpFFRawSize();
        }

        if (*BufSize >= lSize)
        {
          Dem_MemCpy(DestBuffer, &(lFreezeFramePtr[lSourceOffset]), lSize);                                                      /* PRQA S 0311, 0315, 0602 */ /* MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_POINTER_WRITE_ARGUMENT_BUFFER */

          *BufSize = lSize;                                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
          *J1939DTC = Dem_Cfg_EventJ1939Dtc(lEventId);                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
          *OccurrenceCounter = Dem_Client_FilterData_GetJ1939OccurrenceCounter(lEventId);                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

          if (lUpdateState != Dem_Event_GetDataCommitState(lEventId))
          { /* concurrent access to event entry during copy process */
            lModificationDetected = TRUE;
          }
        }
        else
        {
          lReturnValue = DEM_BUFFER_TOO_SMALL;
        }
      }
      else
      { /* concurrent access detected */
        lModificationDetected = TRUE;
      }

      if (lModificationDetected == TRUE)
      { /* retry next call of this interface */
        lReturnValue = DEM_PENDING;
      }
      else if (lReturnValue != DEM_BUFFER_TOO_SMALL)
      { /* copy finished */
        lReturnValue = E_OK;
      }
      else
      {
        /* Return DEM_BUFFER_TOO_SMALL */
      }
    }
    break;
  }
  if (lReturnValue == E_OK)
  {
    Dem_Cfg_ComplexIter32BitNext(&lPrimaryEventsIter);                                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
  }
  lFreezeFrameFilterData.EventEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lPrimaryEventsIter);                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
  Dem_FreezeFrameIterator_SetFreezeFrameFilterData(FreezeFrameIteratorId, lFreezeFrameFilterData);

  return lReturnValue;
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif

/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetNextSPNInFreezeFrame
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FreezeFrameIterator_GetNextSPNInFreezeFrame(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId,
  CONSTP2VAR(uint32, AUTOMATIC, DEM_J1939DCM_DATA)  SPN,
  CONSTP2VAR(uint8, AUTOMATIC, DEM_J1939DCM_DATA)  SPNDataLength
  )
{
  Dem_Cfg_ComplexIterType lSpnIter;
  Std_ReturnType lReturnValue = DEM_NO_SUCH_ELEMENT;

  lSpnIter = Dem_FreezeFrameIterator_GetJ1939SpnIter(FreezeFrameIteratorId);

  if (Dem_Cfg_ComplexIterExists(&lSpnIter) == TRUE)                                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    *SPN = Dem_FreezeFrameIterator_J1939SpnIter_GetSpnId(&lSpnIter);                                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    *SPNDataLength = Dem_FreezeFrameIterator_J1939SpnIter_GetSpnDataLength(&lSpnIter);                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&lSpnIter);                                                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */

    Dem_FreezeFrameIterator_SetJ1939SpnIter(FreezeFrameIteratorId, lSpnIter);
    lReturnValue = E_OK;
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetFreezeFrameKind
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_J1939DcmSetFreezeFrameFilterType, DEM_CODE)
Dem_FreezeFrameIterator_GetFreezeFrameKind(                                                                                      /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_FreezeFrameIterator_HandleType  FreezeFrameIteratorId
  )
{
  return Dem_FreezeFrameIterator_GetFreezeFrameFilterData(FreezeFrameIteratorId).FreezeFrameKind;
}

/* ****************************************************************************
 % Dem_FreezeFrameIterator_SetFreezeFrameFilterData()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_FreezeFrameIterator_SetFreezeFrameFilterData(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId,
  CONST(Dem_Cfg_FreezeFrameIterator_FilterType, AUTOMATIC)  FreezeFrameFilterData
  )
{
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (FreezeFrameIteratorId >= Dem_Cfg_GetSizeOfFreezeFrameIteratorTable())
  {
    Dem_Error_RunTimeCheckFailed(DEM_FREEZEFRAMEITERATOR_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_SetFreezeFrameIteratorTable(FreezeFrameIteratorId, FreezeFrameFilterData);                                           /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_FREEZEFRAMEITERATORTABLE */ 
  }
}

/* ****************************************************************************
 % Dem_FreezeFrameIterator_GetFreezeFrameFilterData()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_FreezeFrameIterator_FilterType, DEM_CODE) 
Dem_FreezeFrameIterator_GetFreezeFrameFilterData(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_FreezeFrameIterator_HandleType, AUTOMATIC)  FreezeFrameIteratorId
  )
{
  return Dem_Cfg_GetFreezeFrameIteratorTable(FreezeFrameIteratorId);
}


/*!
 * \}
 */
# define DEM_STOP_SEC_CODE
# include "Dem_MemMap.h"                                                                                                         /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_FREEZEFRAMEITERATOR_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_FreezeFrameIterator_Implementation.h
 *********************************************************************************************************************/
