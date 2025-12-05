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
/*! \addtogroup Dem_CustomTriggerEntry
 *  \{
 *  \file       Dem_CustomTriggerEntry_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    The purpose of the unit is to manage the entries in Custom Trigger Memory. It manages allocating,
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

#if !defined (DEM_CUSTOMTRIGGERENTRY_IMPLEMENTATION_H)
#define DEM_CUSTOMTRIGGERENTRY_IMPLEMENTATION_H

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
#include "Dem_CustomTriggerEntry_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Memories_Interface.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_CUSTOMTRIGGERENTRY_IMPLEMENTATION_FILENAME "Dem_CustomTriggerEntry_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_CustomTriggerEntry_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_CustomTriggerEntry_PublicProperties
 * \{
 */

#if (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_CustomTriggerEntryPtrType, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  )
{
  Dem_Cfg_CustomTriggerEntryPtrType lMemoryEntry;
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (CustomTriggerEntryIndex >= Dem_Cfg_GetSizeOfCustomTriggerEntryPtr())
  {
    Dem_Error_RunTimeCheckFailed(DEM_CUSTOMTRIGGERENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                      /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lMemoryEntry = Dem_Cfg_GetCustomTriggerEntryPtr(0u);
    /* PRQA S 0310, 3305 */ /* MD_DEM_11.4_nvm, MD_DEM_3305 */
  }
  else
# endif
  {
    lMemoryEntry = Dem_Cfg_GetCustomTriggerEntryPtr(CustomTriggerEntryIndex);
    /* PRQA S 0310, 3305, 2841, 2842, 2852 */ /* MD_DEM_11.4_nvm, MD_DEM_3305, MD_DEM_Dir4.1_EventEntryPointerAccess, MD_DEM_Dir4.1_EventEntryPointerAccess, MD_DEM_Dir1.1_EventEntryPtrTableAccess */
  }

  return lMemoryEntry;
}
#endif


#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_SREC == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_CustomTriggerEntry_GetSnapshotRecordDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_GetSnapshotRecordDataPtr(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  )
{
  return (Dem_SharedDataPtrType)(&(Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex)->SnapshotData[0]));
}
#endif


#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_CustomTriggerEntry_GetTSSnapshotRecordDataPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_SharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_GetTSSnapshotRecordDataPtr(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex,
  uint8 TimeSeriesSampleIndex
  )
{
  Dem_SharedDataPtrType lTSSnapshotDataPtr;

# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (TimeSeriesSampleIndex >= Dem_Memories_CustomTriggerMemory_MaxTssSamplesGet())
  {
    Dem_Error_RunTimeCheckFailed(DEM_CUSTOMTRIGGERENTRY_IMPLEMENTATION_FILENAME, __LINE__);                                      /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    lTSSnapshotDataPtr = (Dem_SharedDataPtrType)(&(Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex)->TSSnapshotData[0][0]));
  }
  else
# endif
  {
    lTSSnapshotDataPtr = (Dem_SharedDataPtrType)(&(Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex)->TSSnapshotData[TimeSeriesSampleIndex][0]));
  }
  return lTSSnapshotDataPtr;
}
#endif

/* ****************************************************************************
 % Dem_MemoryEntry_CustomTriggerEntry_GetEntryIndexToNvBlockIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_NvBlockIndexType, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_GetEntryIndexToNvBlockIndex(                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  )
{
  Dem_Cfg_NvBlockIndexType lNvBlockIndex = DEM_CFG_INVALID_NVBLOCK_INDEX;
#if(DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  Dem_Internal_AssertContinue((CustomTriggerEntryIndex < Dem_Cfg_GetSizeOfCustomTriggerEntryIndexToNvBlockIndex()), \
    DEM_E_INCONSISTENT_STATE)
  lNvBlockIndex = Dem_Cfg_GetCustomTriggerEntryIndexToNvBlockIndex(CustomTriggerEntryIndex);                                     /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
#else
   DEM_IGNORE_UNUSED_CONST_ARGUMENT(CustomTriggerEntryIndex)                                                                     /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
  return lNvBlockIndex;
}

/* ****************************************************************************
 % Dem_MemoryEntry_CustomTriggerEntry_TestEntryForEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_TestEntryForEvent(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex, 
  Dem_EventIdType EventId
  )
{
  boolean lReturn = FALSE;
#if(DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  Dem_EventIdType lEventId;
  lEventId = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex)->EventId;
  lReturn = (boolean)(lEventId == EventId);
#else
   DEM_IGNORE_UNUSED_CONST_ARGUMENT(CustomTriggerEntryIndex)                                                                     /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
   DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                     /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
  
  return lReturn;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_CustomTriggerEntry_Private Private Methods
 * \{
 */


/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_CustomTriggerEntry_Public
 * \{
 */

/* ****************************************************************************
 % Dem_MemoryEntry_CustomTriggerEntry_Free
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_CustomTriggerEntry_Free(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  )
{
#if(DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  Dem_Cfg_CustomTriggerEntryPtrType lEntryPtr = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex);

  lEntryPtr->EventId = DEM_EVENT_INVALID;                                                                                        /* SBSW_DEM_POINTER_WRITE_CUSTOMTRIGGER_ENTRY */
  lEntryPtr->Timestamp = 0;                                                                                                      /* SBSW_DEM_POINTER_WRITE_CUSTOMTRIGGER_ENTRY */
#else
   DEM_IGNORE_UNUSED_CONST_ARGUMENT(CustomTriggerEntryIndex)                                                                     /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_CUSTOMTRIGGERENTRY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_CustomTriggerEntry_Implementation.h
 *********************************************************************************************************************/
