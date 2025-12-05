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
 *  \defgroup   Dem_Displacement Displacement
 *  \{
 *  \file       Dem_Displacement_Interface.h
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

#if !defined (DEM_MEMACCESS_INTERFACE_H)
#define DEM_MEMACCESS_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Displacement_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Displacement_PublicProperties Public Properties
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
 * \defgroup Dem_Displacement_Public Public Methods
 * \{
 */

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF)
/* ****************************************************************************
 * Dem_StorageManager_Displacement_SelectDisplacedIndex
 *****************************************************************************/
/*!
 * \brief         Selects an event to displace in a filled memory
 *
 * \details       Selects an event to displace in a filled memory
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of Event memory.
 * \param[in]     DisplacingEventId
 *                Unique handle of the Event to be entered into the memory
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The event entry index of the displaced event entry if one could
 *                be identified. DEM_CFG_ENTRYINDEX_INVALID if displacement
 *                was not possible.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_OFF
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_StorageManager_Displacement_SelectDisplacedIndex(
  uint16 TranslatedMemoryId,
  Dem_EventIdType DisplacingEventId
  );
#endif

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
/* ****************************************************************************
 * Dem_StorageManager_Displacement_SelectCustomizedDisplacedIndex
 *****************************************************************************/
/*!
 * \brief         Selects an event to displace in a filled memory
 *
 * \details       Selects an event to displace in a filled memory
 *
 * \param[in]     TranslatedMemoryId
 *                Unique Identifier of Event memory.
 * \param[in]     DisplacingEventId
 *                Unique handle of the Event to be entered into the memory
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The entry index of the displaced entry if one could
 *                be identified. DEM_ENTRYINDEX_INVALID if displacement
 *                was not possible.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_StorageManager_Displacement_SelectCustomizedDisplacedIndex(
  uint16 TranslatedMemoryId,
  Dem_EventIdType DisplacingEventId
  );
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMACCESS_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Displacement_Interface.h
 *********************************************************************************************************************/
