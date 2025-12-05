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
 *  \file       Dem_TimeSeriesMemory_Types.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Types for the Time Series Memory Unit which manages storage of time series snapshots.
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

#if !defined (DEM_TIMESERIESMEMORY_TYPES_H)
#define DEM_TIMESERIESMEMORY_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

 /* Max number of samples that can be stored in a time series powertrain entry. - Used for runtime checks */


#define DEM_TIMESERIESMEMORY_INVALID_SREC_ID  (uint8)0u

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


typedef struct Dem_TimeSeriesRecordIterator_s {
  Dem_EventIdType EventId;
  Dem_TimeSeriesIdType StartId;
  Dem_TimeSeriesIdType NumberOfSeries;
  Dem_TimeSeriesIdType SeriesId;
  uint8 ChronologicalId;
  uint8 RecordNumber;
  uint8 FirstRecordNumber;
  uint8 LastRecordNumber;
} Dem_TimeSeriesRecordIterator;


typedef struct Dem_BitField256Bits_s {
  uint8 data[32u];
} Dem_BitField256Bits;

#endif /* DEM_TIMESERIESMEMORY_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_TimeSeriesMemory_Types.h
 *********************************************************************************************************************/
