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
 *  \defgroup   Dem_MemoryEntryIF Dem MemoryIF
 *  \{
 *  \file       Dem_MemoryEntryIF_Interface.h
 *  \brief      Abstraction for multiple event memories.
 *  \details    Public Interfaces of MemoryIF subcomponent which is facade for all memory related tasks.
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

#if !defined (DEM_MEMORYENTRYIF_INTERFACE_H)
#define DEM_MEMORYENTRYIF_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_MemoryEntryIF_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Memory_PublicProperties Public Properties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Memory_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_FaultMemory_MemoryEntry_ChronoIter_Exists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        TRUE
 *                Dem_FaultMemory_MemoryEntry_ChronoIter_GetEntryIndex() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_FaultMemory_MemoryEntry_ChronoIter_GetEntryIndex() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_Exists(
  CONSTP2CONST(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_FaultMemory_MemoryEntry_ChronoIter_GetEntryIndex
 *****************************************************************************/
/*!
 * \brief         Get the event entry index derived from the iterator in 
 *                chronological order starting with the most recent event entry.
 *
 * \details       Get the event entry index derived from the iterator in 
 *                chronological order starting with the most recent event entry.
 *                When iterating over all chrono slots, index 0 will actually
 *                refer to the last chrono entry in the chrono array 
                  (which is the most recent entry).
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Entry index derived from the iterator.
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_GetEntryIndex(
  CONSTP2CONST(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_FaultMemory_MemoryEntry_ChronoIter_GetCurrentIndex
 *****************************************************************************/
/*!
 * \brief         Get the current iterator index in the chrono stack.
 *
 * \details       Get the current iterator index in the chrono stack. This index
 *                is used for iterating over all chrono slots available
 *                for the respective memory.
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Chrono index for the current position of the iterator.
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_GetCurrentIndex(
  CONSTP2CONST(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_FaultMemory_MemoryEntry_ChronoIter_Init
 *****************************************************************************/
/*!
 * \brief         Initializes for iteration over allocated entries.
 *
 * \details       Initializes for iteration over allocated entries.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the event memory.
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           Memory has a chronology.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_Init(
  CONST(uint16, AUTOMATIC)  TranslatedMemoryId,
  CONSTP2VAR(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_FaultMemory_MemoryEntry_ChronoIter_Resume
 *****************************************************************************/
/*!
 * \brief         Initializes for continued iteration over allocated 
 *                entries.
 *
 * \details       Initializes for continued iteration over allocated 
 *                entries. The previous iterator position is provided 
 *                by calling function.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the event memory.
 * \param[in]     StoredChronoIter
 *                Stored chrono iterator that shall be continued with.
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           Memory has a chronology.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_Resume(
  CONST(uint16, AUTOMATIC)  TranslatedMemoryId,
  CONST(Dem_Cfg_EntryIndexType, AUTOMATIC)  StoredChronoIter,
  CONSTP2VAR(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );


/* ****************************************************************************
 * Dem_FaultMemory_MemoryEntry_ChronoIter_Next
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next (older) element
 *
 * \details       Move the iterator to the next (older) element
 *
 * \param[in,out] IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_MemoryEntry_ChronoIter_Next(
  CONSTP2VAR(Dem_MemoryEntry_AllocMemEntriesIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMORYENTRYIF_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemoryEntryIF_Interface.h
 *********************************************************************************************************************/
