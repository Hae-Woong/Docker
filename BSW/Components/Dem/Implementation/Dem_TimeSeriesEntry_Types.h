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
 *  \file       Dem_TimeSeriesEntry_Types.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Types for the Time Series Entry unit which handles access to and from time series entries.
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

#if !defined (DEM_TIMESERIESENTRY_TYPES_H)
#define DEM_TIMESERIESENTRY_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

#define DEM_MEMORY_TIMESERIES_INVALID_SREC_ID  (uint8)0u

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 /*! Pointer to a time-series entry */
typedef CONSTP2VAR(Dem_Cfg_TimeSeriesEntryBaseType, TYPEDEF, DEM_NVM_DATA) Dem_TimeSeriesEntry_ConstPtrToBaseType;
typedef P2VAR(Dem_Cfg_TimeSeriesEntryBaseType, TYPEDEF, DEM_NVM_DATA) Dem_TimeSeriesEntry_PtrToBaseType;
/*! Non-writeable Pointer to a time-series entry */
typedef P2CONST(Dem_Cfg_TimeSeriesEntryBaseType, TYPEDEF, DEM_NVM_DATA)  Dem_TimeSeriesEntry_PtrToConstBaseType;

 /*! Pointer to a time-series entry companion */
typedef P2VAR(Dem_Cfg_TimeSeriesEntryCompanionType, AUTOMATIC, AUTOMATIC) Dem_TimeSeriesEntry_CompanionPtrType;

 /*! Pointer to a const time-series entry companion */
typedef P2CONST(Dem_Cfg_TimeSeriesEntryCompanionType, AUTOMATIC, AUTOMATIC) Dem_TimeSeriesEntry_CompanionPtrToConstType;

/*! Const pointer to a time-series entry companion */
typedef CONSTP2VAR(Dem_Cfg_TimeSeriesEntryCompanionType, AUTOMATIC, AUTOMATIC) Dem_TimeSeriesEntry_ConstCompanionPtrType;

 /*! Const pointer to a const time-series entry companion */
typedef CONSTP2CONST(Dem_Cfg_TimeSeriesEntryCompanionType, AUTOMATIC, AUTOMATIC) Dem_TimeSeriesEntry_ConstCompanionPtrToConstType;

/*  TSS Fifo entry handle type  */
typedef struct Dem_Cfg_TssFifoEntry_Handle_s {
  uint16                        EventId;                                        /* Event ID */
  Dem_Cfg_EntryIndexType        TimeSeriesEntryIndex;                           /* Time series entry handle */
  uint8                         TssSetId;                                       /* Tss Fifo Set index of current iterator element */
  uint8                         TssChronoId;                                    /* Tss Fifo chrono index of current iterator element */
  Dem_TimeSeriesIdType          SeriesId;
}                                                                     Dem_Cfg_TssFifoEntry_IterType;

/*! Structure containing meta information about the TimeSeries SRec for DID data readout */
typedef struct Dem_Memory_TimeSeries_PowertrainTimeSeriesInfoType_s {
  uint16 SamplingRate;                    /*!< Sampling rate of the DID */
  uint16 StartingSequentialDIDIdentifier; /*!< Starting sequential DID identifier for report in service 0x19 04*/
  uint8  NumberOfConfiguredSamples;       /*!< Number of configured DID samples */
  uint8  NumberOfNotStoredSamples;        /*!< Number of configured DID samples which are not stored*/
} Dem_Memory_TimeSeries_PowertrainTimeSeriesInfoType;

typedef P2VAR(Dem_Cfg_TssFifoEntry_IterType, TYPEDEF, AUTOMATIC)     Dem_Cfg_TssFifoEntry_IterPtrType;                           /* PRQA S 0779 */ /* MD_MSR_Rule5.2 */

/*!
 * \defgroup  Dem_Memory_TimeSeriesEntry_ReadoutStatusType Dem_TimeSeriesEntry_ReadoutStatusType
 * \{
 */
 /*! This type is used to encode the time-series entry readout status */
typedef uint8 Dem_TimeSeriesEntry_ReadoutStatusType;


#define DEM_TIMESERIESENTRY_READOUTSTATUS_UNUSED                        0u  /*!< Initialization value, time-series entry is unused */
#define DEM_TIMESERIESENTRY_READOUTSTATUS_UNLOCKED                      1u  /*!< Time-series entry is allocated to an event. No on-going readout */
#define DEM_TIMESERIESENTRY_READOUTSTATUS_LOCKED_FOR_READOUT            2u  /*!< Time-series entry is being read out via service 19 subfunction 04 */
#define DEM_TIMESERIESENTRY_READOUTSTATUS_DELETION_CANDIDATE            3u  /*!< Time-series entry is being read out via service 19 subfunction 04 and it should be freed immediately when all ongoing readout finish */
/*!
 * \}
 */

/* Init value of time series entry used in time series powertrain - Indicate that none past sample was ever stored */
#define DEM_TIMESERIESENTRY_UNUSEDSAMPLES_INITVALUE (0xFFu) 

#endif /* DEM_TIMESERIESENTRY_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_TimeSeriesEntry_Types.h
 *********************************************************************************************************************/
