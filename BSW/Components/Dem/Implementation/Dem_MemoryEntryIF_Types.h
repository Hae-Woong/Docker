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
/*! \addtogroup Dem_MemoryEntryIF
 *  \{
 *  \file       Dem_MemoryEntryIF_Types.h
 *  \brief      Abstraction for multiple event memories.
 *  \details    Type definitions of MemoryIF subcomponent which is facade for all memory related tasks.
 *  \entity     MemoryEntryIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMORYENTRYIF_TYPES_H)
#define DEM_MEMORYENTRYIF_TYPES_H

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
struct Dem_MemoryEntry_AllocMemEntriesIterType_s
{
  Dem_Cfg_EntryIndexType mIdx;    /*!< Current iterator position */
  Dem_Cfg_EntryIndexType mEndIdx; /*!< Iterator end position */
  uint16 TranslatedMemoryId;                 /*!< Unique identifier of event memory */
};

/*! Iterator for iterations over all event entries in chronological order*/
typedef struct Dem_MemoryEntry_AllocMemEntriesIterType_s Dem_MemoryEntry_AllocMemEntriesIterType;

#endif /* DEM_MEMORYENTRYIF_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemoryEntryIF_Types.h
 *********************************************************************************************************************/
