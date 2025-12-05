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
/*! \addtogroup Dem_Displacement
 *  \{
 *  \file       Dem_Displacement_Types.h
 *  \brief      Basic routines for event memories.
 *  \details    Handle displacement for every kind of entry, this includes:
 *              - Find next displacement candidate
 *              - Check priority of entries
 *  \entity     Displacement
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMACCESS_TYPES_H)
#define DEM_MEMACCESS_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! Displacement info data type for selecting the "best" element for displacement */
struct Dem_Displacement_InfoType_s
{
  uint16                   EventId;                      /*!< EventId of current element */
  Dem_Cfg_EntryIndexType   EntryIndex;                   /*!< Entry index of current element; DEM_CFG_ENTRYINDEX_INVALID marks invalid structure */
#if (DEM_CFG_AGING_RETAIN_MEMORY_ENTRY == STD_ON) || (DEM_FEATURE_NEED_AGING_REALLOCATION == STD_ON)
  Dem_Cfg_EntryIndexType   EntryIndexAging;              /*!< Entry index of current element for considering aging */
#endif
#if (DEM_CFG_DISPLACEMENT_FALLBACK_OLDEST == STD_ON)
  Dem_Cfg_EntryIndexType   OldestDisplaceableEntryIndex; /*!< Entry index of oldest event can be always displaced - used as fallback */
#endif
  uint8                    EventInternalUdsStatus;       /*!< Event status considered for displacement */
};

/*! Displacement info data type for selecting the "best" element for displacement */
typedef struct Dem_Displacement_InfoType_s Dem_Displacement_InfoType;


/*! Result of a displacement check */
typedef enum
{
  Dem_StorageManager_Displace_NeedMoreTests,   /*!< Candidate is not classified yet, apply further tests on this candidate */
  Dem_StorageManager_Displace_RatingDone,      /*!< Candidate is classified, continue the scan with next candidate */
  Dem_StorageManager_Displace_FoundBestMatch   /*!< Candidate is the best match, abort scanning for better matches */
} Dem_StorageManager_Displace_CheckResultType;


#endif /* DEM_MEMACCESS_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Displacement_Types.h
 *********************************************************************************************************************/
