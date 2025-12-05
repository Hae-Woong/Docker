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
 *  \file       Dem_EventEntry_Types.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Types for the Event Entry unit which handles access to and from event entries.
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

#if !defined (DEM_EVENTENTRY_TYPES_H)
#define DEM_EVENTENTRY_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/*!
 * \defgroup  Dem_MemoryEntry_Handles  Macros encoding handles for specific event entries.
 * \{
 */
#define DEM_EVENTENTRY_HANDLE_PRIMARY                  0U                                                                 /*!< Value for the first primary event entry handle */
#define DEM_EVENTENTRY_HANDLE_SECONDARY                DEM_CFG_GLOBAL_PRIMARY_SIZE                                        /*!< Value for the first secondary event entry handle */
#define DEM_EVENTENTRY_HANDLE_READOUTBUFFER            ((Dem_Cfg_EntryIndexType)DEM_CFG_MEMORYENTRY_HANDLE_READOUTBUFFER) /*!< Value for the first readout buffer event entry handle */

/*!
 * \}
 */

/*!
 * \defgroup  Dem_MemoryEntry_StandardEnvironmentalDataBytes  Macros encoding the bytes of the standard environmental data.
 * \{
 */
#define DEM_EVENTENTRY_STDENVDATA_STATUS               0x00U  /*!< Status */
#define DEM_EVENTENTRY_STDENVDATA_FIRSTODOMETER        0x01U  /*!< Original Odometer */
#define DEM_EVENTENTRY_STDENVDATA_CURRENTODOMETER      0x03U  /*!< Most Recent Odometer */
#define DEM_EVENTENTRY_STDENVDATA_TIME                 0x05U  /*!< Operating Time */
#define DEM_EVENTENTRY_STDENVDATA_TIMESTATUS           0x09U  /*!< Operating Time Status */
/*!
 * \}
 */

#define DEM_EVENTENTRY_STD_ENV_DATA_SIZE               5U     /* Size of Standard Environmental data stored in an event entry*/
#define DEM_EVENTENTRY_EXT_ENV_DATA_SIZE               10U    /* Size of Extended Environmental data stored in an event entry*/

#if (DEM_CFG_DATA_EXTENDED_ENV_DATA == STD_ON)
# define DEM_EVENTENTRY_ENV_DATA_SIZE                  DEM_EVENTENTRY_EXT_ENV_DATA_SIZE
#elif (DEM_CFG_DATA_STANDARD_ENV_DATA == STD_ON)
# define DEM_EVENTENTRY_ENV_DATA_SIZE                  DEM_EVENTENTRY_STD_ENV_DATA_SIZE
#endif

/*! Aging timer invalid (stopped) value */
#define DEM_EVENTENTRY_AGINGTIME_INVALID               0x00000000UL

/*! Preconfirmed DTC status after re-initialization */
#define DEM_EVENTENTRY_PC_STATUS_INITIALIZE            0x00U

/*! Test aging only state of event entry status byte */
#define DEM_EVENTENTRY_TEST_AGING_ONLY(State)          ((uint8)((State) & 0x01U) == 0x01U)
/*! Set aging only state of event entry status byte */
#define DEM_EVENTENTRY_SET_AGING_ONLY(State)           ((uint8)((State) | 0x01U))
/*! Reset aging only state of event entry status byte */
#define DEM_EVENTENTRY_RESET_AGING_ONLY(State)         ((uint8)((State) & 0xfeU))

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if (DEM_CFG_DATA_OCCCTR_2BYTE == STD_ON)
/*! Type for 2 byte DTC occurrence counter */
typedef uint16 Dem_EventEntry_OccurrenceCounterType;
#else
/*! Type for 1 byte DTC occurrence counter */
typedef uint8 Dem_EventEntry_OccurrenceCounterType;
#endif


#endif /* DEM_EVENTENTRY_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EventEntry_Types.h
 *********************************************************************************************************************/
