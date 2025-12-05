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
 *  \defgroup   Dem_MemoryRestoration Memory Restoration
 *  \{
 *  \file       Dem_MemoryRestoration_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Interface of the Memory Restoration subcomponent which restores the status of an event entry, 
 *              fixes event entry allocation errors etc.
 *  \entity     MemoryRestoration
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMORYRESTORATION_INTERFACE_H)
#define DEM_MEMORYRESTORATION_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_MemoryRestoration_Types.h"

/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/


/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_MemoryRestoration_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_FaultMemory_MemoryRestoration_RestoreEventStatus
 *****************************************************************************/
/*!
 * \brief         Restores the event status of an event
 *
 * \details       Restores the event status and internal status based on the 
 *                configured storage trigger.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     EventId
 *                EventId stored at the event entry index.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemoryRestoration_RestoreEventStatus(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_FaultMemory_MemoryRestoration_RestoreTripCountFromCFCC
 *****************************************************************************/
/*!
 * \brief         Restores the trip counter of an event from consecutive
 *                failed cycles.
 *
 * \details       Restores the trip counter of an event from consecutive
 *                failed cycles.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 * \param[in]     EventId
 *                EventId stored at the event entry index.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemoryRestoration_RestoreTripCountFromCFCC(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_FaultMemory_MemoryRestoration_FixCorruptEventEntry
 *****************************************************************************/
/*!
 * \brief         Check for corrupt event entry.
 *
 * \details       Test if an event entry is corrupt. If so, fix it and report
 *                DET.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier for the event memory.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        TRUE
 *                Entry was corrupt - caller shall continue with next entry.
 * \return        FALSE
 *                Entry is correct.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_MemoryRestoration_FixCorruptEventEntry(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/* ****************************************************************************
 * Dem_FaultMemory_MemoryRestoration_FixDoubleAllocation
 *****************************************************************************/
/*!
 * \brief         Check for duplicate event entry.
 *
 * \details       Test if event entry is allocated to an event that already
 *                has an event entry. If so, remove the duplicate.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier for the event memory.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \return        TRUE
 *                This event entry was the duplicate - the caller shall
 *                continue with next entry, this one was removed.
 * \return        FALSE
 *                This entry is correct.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_MemoryRestoration_FixDoubleAllocation(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMORYRESTORATION_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemoryRestoration_Interface.h
 *********************************************************************************************************************/
