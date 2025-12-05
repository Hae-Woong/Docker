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
/*! \addtogroup Dem_EventMemory
 *  \{
 *  \file       Dem_EventMemory_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Implementation of the Permanent Memory subcomponent which manages storage of permanent DTCs.
 *  \entity     EventMemory
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EVENTMEMORY_IMPLEMENTATION_H)
#define DEM_EVENTMEMORY_IMPLEMENTATION_H

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
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_EventMemory_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_MemoryEntry_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_MemoriesIF_Interface.h"
#include "Dem_DataStorage_Interface.h"
#include "Dem_StorageManager_Interface.h"
#include "Dem_MemoryRestoration_Interface.h"
#include "Dem_Infrastructure_Interface.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_EVENTMEMORY_IMPLEMENTATION_FILENAME "Dem_EventMemory_Implementation.h"

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_EventMemory_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_EventMemory_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Memories_EventMemory_GetTotalSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Memories_EventMemory_GetTotalSize(
  void
  )
{
  /* The readout buffers are also part of Dem_Cfg_EventEntryPtr and are always the
     last attributes after all valid event entries. They are not considered
     as 'real' event memory, so don't include them in the size. */
  return DEM_EVENTENTRY_HANDLE_READOUTBUFFER;
}


/* ****************************************************************************
 % Dem_Memories_EventMemory_IsReferredByMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_IsReferredByMemoryId(
  uint16 TranslatedMemoryId
  )
{
  boolean lEventDestination;
  lEventDestination = FALSE;
  
  if ((TranslatedMemoryId == DEM_CFG_MEMORYID_PRIMARY)
    || (TranslatedMemoryId <= Dem_Memories_EventMemory_UserDefined_GetNumberOfUserDefinedMemories()))
  {
    lEventDestination = TRUE;
  }

  return lEventDestination;
}

/* ****************************************************************************
 % Dem_Memories_EventMemory_UserDefined_IsEnabled()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_IsEnabled(
  void
  )
{
  return (boolean)(DEM_CFG_SUPPORT_USERDEFINEDMEMORY == STD_ON);
}

/* ****************************************************************************
% Dem_Memories_EventMemory_UserDefined_IsValid()
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_IsValid(
  Dem_DTCOriginType DTCOrigin
  )
{
    return (boolean)((DTCOrigin >= DEM_MEMORY_USERDEFINEDMEMORY_DTCORIGIN_LOWER_LIMIT)
                    && (DTCOrigin <= DEM_MEMORY_USERDEFINEDMEMORY_DTCORIGIN_UPPER_LIMIT));
}

/* ****************************************************************************
 % Dem_Memories_EventMemory_UserDefined_GetNumberOfUserDefinedMemories()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_GetNumberOfUserDefinedMemories(
  void
  )
{
#if (DEM_CFG_SUPPORT_USERDEFINEDMEMORY == STD_ON)
  return (uint16)DEM_CFG_MAX_NUM_USER_DEFINED_MEMORIES;
#else
  return (uint16)0x00U;
#endif
}

/* ****************************************************************************
 % Dem_Memories_EventMemory_UserDefined_TranslateDTCOrigin
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_TranslateDTCOrigin(
  Dem_DTCOriginType  DTCOrigin
  )
{
  uint16 lTranslatedMemoryId;
  lTranslatedMemoryId = DEM_CFG_MEMORYID_PRIMARY;

#if (DEM_CFG_SUPPORT_USERDEFINEDMEMORY == STD_ON)
  {
    uint16 lUserDefinedMemoryId;
    lUserDefinedMemoryId = (DTCOrigin - DEM_MEMORY_USERDEFINEDMEMORY_TRANSLATION_OFFSET);
    if (Dem_Cfg_IsMemoryInfoTableUsedOfUserDefinedOriginTable(lUserDefinedMemoryId) == TRUE)
    {
      lTranslatedMemoryId = (uint16)Dem_Cfg_GetMemoryInfoTableIdxOfUserDefinedOriginTable(lUserDefinedMemoryId);
    }
  }
#endif

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DTCOrigin)
  return lTranslatedMemoryId;
}

/* ****************************************************************************
% Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculated
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculated(
  Dem_Cfg_SRecNumerationType NumerationType
  )
{
  return (boolean)((NumerationType == DEM_CFG_MEMORY_FF_RECNUM_CALCULATED)
    || (NumerationType == DEM_CFG_MEMORY_FF_RECNUM_CALCULATED_FIFO));
}

/* ****************************************************************************
% Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculatedFiFo
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculatedFiFo(
  Dem_Cfg_SRecNumerationType NumerationType
  )
{
  return (boolean)(NumerationType == DEM_CFG_MEMORY_FF_RECNUM_CALCULATED_FIFO);
}

/* ****************************************************************************
% Dem_Memories_EventMemory_UserDefined_IsSRecNumerationConfigured
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_IsSRecNumerationConfigured(
  Dem_Cfg_SRecNumerationType NumerationType
  )
{
  return (boolean)(NumerationType == DEM_CFG_MEMORY_FF_RECNUM_CONFIGURED);
}

/* ****************************************************************************
 % Dem_Memories_EventMemory_TestDTCOriginValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
*/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestDTCOriginValid(
 Dem_DTCOriginType  DTCOrigin
 )
{
  boolean lDTCOriginValid;
  lDTCOriginValid = FALSE;

  if ((DTCOrigin != 0u) && (DTCOrigin < DEM_DTC_ORIGIN_INVALID))
  {
    lDTCOriginValid = TRUE;
  }
  else if (Dem_Memories_EventMemory_UserDefined_IsValid(DTCOrigin) == TRUE)
  {
    lDTCOriginValid = TRUE;
  }
  else
  {
    /* MISRA */
  }

  return lDTCOriginValid;
}

/* ****************************************************************************
 % Dem_Memories_EventMemory_TestStorageTriggerIsOnConfirmed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestStorageTriggerIsOnConfirmed(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  boolean lStorageTriggerFulfilled;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lStorageTriggerFulfilled = (boolean)(Dem_Cfg_GetEventMemoryEntryStorageTriggerOfMemoryInfoTable(
                                        Dem_Core_Event_GetDestination(EventId)) == DEM_MEMORY_STORAGE_TRIGGER_CONFIRMED);
  return lStorageTriggerFulfilled;
}

/* ****************************************************************************
 % Dem_Memories_EventMemory_TestStorageTriggerIsOnFailed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestStorageTriggerIsOnFailed(                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  boolean lStorageTriggerFulfilled;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lStorageTriggerFulfilled = (boolean)(Dem_Cfg_GetEventMemoryEntryStorageTriggerOfMemoryInfoTable(
                               Dem_Core_Event_GetDestination(EventId)) == DEM_MEMORY_STORAGE_TRIGGER_FAILED);
  return lStorageTriggerFulfilled;
}

/* ****************************************************************************
 % Dem_Memories_EventMemory_TestStorageTriggerIsOnPending
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestStorageTriggerIsOnPending(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  boolean lStorageTriggerFulfilled;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lStorageTriggerFulfilled = (boolean)(Dem_Cfg_GetEventMemoryEntryStorageTriggerOfMemoryInfoTable(
                               Dem_Core_Event_GetDestination(EventId)) == DEM_MEMORY_STORAGE_TRIGGER_PENDING);
  return lStorageTriggerFulfilled;
}

/* ****************************************************************************
 % Dem_Memories_EventMemory_TestStorageTriggerIsOnFdc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestStorageTriggerIsOnFdc(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  boolean lStorageTriggerFulfilled;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lStorageTriggerFulfilled = (boolean)(Dem_Cfg_GetEventMemoryEntryStorageTriggerOfMemoryInfoTable(
                               Dem_Core_Event_GetDestination(EventId)) == DEM_MEMORY_STORAGE_TRIGGER_FDC);
  return lStorageTriggerFulfilled;
}

/* ****************************************************************************
 % Dem_Memories_EventMemory_TestStorageTriggerIsFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestStorageTriggerIsFulfilled(                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId,
  Dem_Cfg_StorageTriggerType DtcEffects
)
{
  boolean lStorageTriggerFulfilled;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */

  lStorageTriggerFulfilled = (boolean)((DtcEffects & Dem_Cfg_GetEventMemoryEntryStorageTriggerOfMemoryInfoTable(
                                        Dem_Core_Event_GetDestination(EventId))) != 0u);
  return lStorageTriggerFulfilled;
}

/* ****************************************************************************
 % Dem_Memories_EventMemory_GetReferencedCustomTriggerMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_EventMemory_GetReferencedCustomTriggerMemoryId(                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint16 TranslatedMemoryId
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TranslatedMemoryId)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return (uint16)Dem_Cfg_GetCustomTriggerTranslatedMemoryIdOfMemoryInfoTable(TranslatedMemoryId);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_EventMemory_Private Private Methods
 * \{
 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_EventMemory_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(
  Dem_EventIdType EventId,
  boolean VerifyEventStored
)
{
  return Dem_FaultMemory_Memories_FindEntryIndexOfEventGeneric(EventId, VerifyEventStored, Dem_Core_Event_GetDestination(EventId));
}

#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/* ****************************************************************************
 % Dem_Memories_EventMemory_StoreWwhObdFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_EventMemory_StoreWwhObdFreezeFrame(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lReturnValue;

  #if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
  if (Dem_StorageManager_DataStorage_PrestorageApplyWwhObdFF(Dem_MemoryEntry_EventEntry_GetWwhObdFreezeFrameDataPtr(EventEntryIndex), /* SBSW_DEM_POINTER_WWHOBDFREEZEFRAME_BUFFER */
                                      Dem_Cfg_GlobalWwhobdFreezeFrameDataSize()) == FALSE)
  #endif
  {
    Dem_StorageManager_DataStorage_CollectEventSpecificOBDFreezeFrame(EventId,
                                      Dem_MemoryEntry_EventEntry_GetWwhObdFreezeFrameDataPtr(EventEntryIndex),                   /* SBSW_DEM_POINTER_WWHOBDFREEZEFRAME_BUFFER */
                                      Dem_Cfg_GlobalWwhobdFreezeFrameDataSize());
  }
  lReturnValue = DEM_DATA_USERDATA_CHANGED;

  return lReturnValue;
}
#endif


/* ****************************************************************************
 % Dem_Memories_EventMemory_FreeEntryOfEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_EventMemory_FreeEntryOfEvent(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  Dem_EventIdType EventId
  )
{
  Dem_FaultMemory_Memories_RemoveEntryAndChrono(Dem_Core_Event_GetDestination(EventId), EventEntryIndex);
}

/* ****************************************************************************
 % Dem_Memories_EventMemory_Restore
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_EventMemory_Restore(
  uint16 TranslatedMemoryId
  )
{
  Dem_Cfg_EntryIterType lAllEventsIter;
 
  Dem_FaultMemory_Memories_SetCurrentSize(TranslatedMemoryId, 0);

  for (Dem_Cfg_ComplexIter32BitInit(&lAllEventsIter,
         Dem_FaultMemory_Memories_GetFirstEntryIndex(TranslatedMemoryId),
         Dem_FaultMemory_Memories_GetEndEntryIndex(TranslatedMemoryId)
       );                                                                                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIter32BitExists(&lAllEventsIter) == TRUE;                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIter32BitNext(&lAllEventsIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lAllEventsIter);                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_EventIdType lEventId;

    /* Check and fix problems with internal corruption of the event entry record: */
    if (Dem_FaultMemory_MemoryRestoration_FixCorruptEventEntry(TranslatedMemoryId, lEventEntryIndex) == TRUE)
    { /* Continue with next event entry */
      continue;                                                                                                                  
    }

    /* If the event entry is allocated to an event that already has an event entry: */
    if (Dem_FaultMemory_MemoryRestoration_FixDoubleAllocation(TranslatedMemoryId, lEventEntryIndex) == TRUE)
    { /* Continue with next event entry */
      continue;                                                                                                                  
    }

    lEventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);
    if (lEventId != DEM_EVENT_INVALID)
    {
      uint32 lTimestamp;
      lTimestamp = Dem_MemoryEntry_EventEntry_GetTimestamp(lEventEntryIndex);

      Dem_Core_StatusIndicator_UpdateOnMemoryInitRestoreMemory(lEventId);

      Dem_FaultMemory_MemoryRestoration_RestoreEventStatus(lEventEntryIndex, lEventId);
      Dem_FaultMemory_MemoryRestoration_RestoreTripCountFromCFCC(lEventEntryIndex, lEventId);

      Dem_Event_InitUdsFromStatusIndicator(lEventId);

      /* Synchronize the global timestamp with the timestamp stored in the event entry */
      if (lTimestamp > Dem_FaultMemory_Memories_GetCurrentTimestamp())
      {
        Dem_FaultMemory_Memories_SetCurrentTimestamp(lTimestamp);
      }

      /* Sort the event entry into the chronological list */
      Dem_FaultMemory_Memories_SortEventEntryIntoChronologicalList(TranslatedMemoryId, lEventEntryIndex, lTimestamp);

      /* Update the number of occupied event entries */
      Dem_FaultMemory_Memories_SetCurrentSize(TranslatedMemoryId,
        (uint8)(Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId) + 1u));
    }
  }

  /* Update the global timestamp */
  Dem_FaultMemory_Memories_SetCurrentTimestamp(Dem_FaultMemory_Memories_GetCurrentTimestamp() + 1u);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Memories_EventMemory_UpdateChronology
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_EventMemory_UpdateChronology(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  /* Store the global timestamp to the entry, and update it */
  Dem_MemoryEntry_EventEntry_SetTimestamp(EventEntryIndex, Dem_FaultMemory_Memories_GetCurrentTimestamp());
  Dem_FaultMemory_Memories_SetCurrentTimestamp(Dem_FaultMemory_Memories_GetCurrentTimestamp() + 1u);

  Dem_FaultMemory_Memories_UpdateChrono(Dem_Core_Event_GetDestination(EventId), EventEntryIndex);
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_EVENTMEMORY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EventMemory_Implementation.h
 *********************************************************************************************************************/
