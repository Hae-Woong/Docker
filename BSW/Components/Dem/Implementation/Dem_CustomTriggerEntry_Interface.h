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
 *  \defgroup   Dem_CustomTriggerEntry CustomTriggerEntry
 *  \{
 *  \file       Dem_CustomTriggerEntry_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    The purpose of the unit is to manage the entries in Custom Trigger Memory. It takes care of allocating,
 *              freeing, accessing entry in Custom Trigger Memory.
 *  \unit       CustomTriggerEntry
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_CUSTOMTRIGGERENTRY_INTERFACE_H)
#define DEM_CUSTOMTRIGGERENTRY_INTERFACE_H

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
/* PRQA S 3415 EOF */ /* MD_DEM_13.5_cf */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own unit header */
/* ------------------------------------------------------------------------- */
#include "Dem_CustomTriggerEntry_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_CustomTriggerEntry_PublicProperties Public Properties
 * \{
 */

#if (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe
 *****************************************************************************/
/*!
 * \brief         Get the custom trigger entry
 *
 * \details       -
 *
 * \param[in]     EventEntryIndex
 *                Index of the entry in Dem_Cfg_CustomTriggerEntryPtr
 *
 * \return        Pointer to the custom trigger entry
 *
 * \pre           -
 *
 * \config        DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_CustomTriggerEntryPtrType, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  );
#endif

#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_SREC == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_CustomTriggerEntry_GetSnapshotRecordDataPtr
 *****************************************************************************/
/*!
 * \brief         Get the snapshot data buffer of the custom trigger entry
 *
 * \details       -
 *
 * \param[in]     CustomTriggerEntryIndex
 *                Index of the entry in Dem_Cfg_CustomTriggerEntryPtr.
 *
 * \return        Pointer to the snapshot data buffer of the custom trigger entry
 *
 * \pre           -
 *
 * \config        DEM_CFG_SUPPORT_CUSTOMTRIGGER_SREC == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_GetSnapshotRecordDataPtr(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  );
#endif

#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_CustomTriggerEntry_GetTSSnapshotRecordDataPtr
 *****************************************************************************/
/*!
 * \brief         Get time series snapshot data sample buffer of 
 *                the custom trigger entry
 *
 * \details       -
 *
 * \param[in]     CustomTriggerEntryIndex
 *                Index of the entry in Dem_Cfg_CustomTriggerEntryPtr.
 * \param[in]     TimeSeriesSampleIndex
 *                Index of the Time series sample in the time series snapshot data.
 *
 * \return        Pointer to the custom triggered time series snapshot sample 
 *                buffer of the custom trigger entry
 *
 * \pre           -
 *
 * \config        DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_GetTSSnapshotRecordDataPtr(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex,
  uint8 TimeSeriesSampleIndex
  );
#endif

/* ****************************************************************************
 * Dem_MemoryEntry_CustomTriggerEntry_GetEntryIndexToNvBlockIndex
 *****************************************************************************/
/*!
 * \brief         Returns the Nv Block Index referencing the custom trigger entry.
 *
 * \details       -
 *
 * \param[in]     CustomTriggerEntryIndex
 *                Index of the entry in Dem_Cfg_CustomTriggerEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfCustomTriggerEntryPtr()[.
 *
 * \return        NV Block Id
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_NvBlockIndexType, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_GetEntryIndexToNvBlockIndex(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  );

/* ****************************************************************************
 * Dem_MemoryEntry_CustomTriggerEntry_TestEntryForEvent
 *****************************************************************************/
/*!
 * \brief         Test if given event occupies custom trigger entry linked to given index
 *
 * \details       -
 *
 * \param[in]     CustomTriggerEntryIndex
 *                Index of the entry in Dem_Cfg_CustomTriggerEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfCustomTriggerEntryPtr()[.
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \return        TRUE
 *                Given event occupies given entry
 * \return        FALSE
 *                Given event does not occupy given entry
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_TestEntryForEvent(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex,
  Dem_EventIdType EventId
  );
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_CustomTriggerEntry_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_MemoryEntry_CustomTriggerEntry_Free
 *****************************************************************************/
/*!
 * \brief         Free custom trigger entry linked to given index
 *
 * \details       -
 *
 * \param[in]     CustomTriggerEntryIndex
 *                Index of the entry in Dem_Cfg_CustomTriggerEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfCustomTriggerEntryPtr()[.
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_Free(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_CUSTOMTRIGGERENTRY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_CustomTriggerEntry_Interface.h
 *********************************************************************************************************************/
