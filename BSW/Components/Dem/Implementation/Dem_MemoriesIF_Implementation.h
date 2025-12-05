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
/*! \addtogroup Dem_MemoriesIF
 *  \{
 *  \file       Dem_MemoriesIF_Implementation.h
 *  \brief      Abstraction for multiple event memories.
 *  \details    Implementation of MemoriesIF subcomponent which is facade for all memory related tasks which can be
 *              abstracted.
 *  \entity     MemoriesIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_MEMORIESIF_IMPLEMENTATION_H)
#define DEM_MEMORIESIF_IMPLEMENTATION_H

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
#include "Dem_MemoriesIF_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/ - units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_MemoryEntry_Interface.h"
#include "Dem_StorageManager_Interface.h"
#include "Dem_PrestorageMemory_Interface.h"
#include "Dem_CustomTriggerMemory_Interface.h"
#include "Dem_TimeSeriesMemory_Interface.h"
#include "Dem_PermanentMemory_Interface.h"
#include "Dem_ObdFreezeFrameMemory_Interface.h"
#include "Dem_Satellite_Interface.h"
#include "Dem_MemoryEntry_Interface.h"
#include "Dem_EventMemory_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_MemoryRestoration_Interface.h"


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_MEMORIESIF_IMPLEMENTATION_FILENAME "Dem_MemoriesIF_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! NvM requested initialization of admin data */
DEM_LOCAL VAR(boolean, DEM_VAR_NO_INIT) Dem_FaultMemory_Memories_InitAdminDataRequested;

#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DEM_START_SEC_VAR_NO_INIT_32
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Current time stamp, used to establish a chronological ordering */
DEM_LOCAL VAR(uint32, DEM_VAR_NO_INIT)         Dem_FaultMemory_Memories_CurrentTimestamp;

#define DEM_STOP_SEC_VAR_NO_INIT_32
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */     

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
/*!
 * \defgroup Dem_Memory_PrivateProperties Private Properties
 * \{
 */
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"
/* ****************************************************************************
 * Dem_FaultMemory_Memories_GetInitAdminDataRequested
 *****************************************************************************/
/*!
 * \brief         Returns if admin data was initialized or not
 *
 * \details       Returns if admin data was initialized or not
 *
 * \return        TRUE: Admin data was initialized by NvM
 * \return        FALSE: Otherwise
 *
 * \pre           NvM must have restored NV mirrors.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_Memories_GetInitAdminDataRequested(
  void
  );

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/


/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetInitAdminDataRequested
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_Memories_GetInitAdminDataRequested(
  void
  )
{
  return Dem_FaultMemory_Memories_InitAdminDataRequested;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Memory_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetSRecNumerationType
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecNumerationType, DEM_CODE)
Dem_FaultMemory_Memories_GetSRecNumerationType(
  uint16 TranslatedMemoryId
)
{
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
    return Dem_Cfg_GetSRecNumTypeOfMemoryInfoTable(TranslatedMemoryId);
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_SetCurrentTimestamp
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SetCurrentTimestamp(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint32, AUTOMATIC)  CurrentTimeStamp
  )
{
  Dem_FaultMemory_Memories_CurrentTimestamp = CurrentTimeStamp;
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetCurrentTimestamp
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_FaultMemory_Memories_GetCurrentTimestamp(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return Dem_FaultMemory_Memories_CurrentTimestamp;
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetChronology
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_GetChronology(
  uint16 TranslatedMemoryId,
  uint8  ChronoIndex
  )
{
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(ChronoIndex)                                                                                  /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return (Dem_Cfg_GetChronoPtrOfMemoryInfoTable(TranslatedMemoryId))[ChronoIndex];
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_SetChronology
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SetChronology(
  uint16 TranslatedMemoryId,
  uint8 ChronoIndex,
  Dem_Cfg_EntryIndexType EntryIndex
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ( (TranslatedMemoryId >= Dem_Cfg_GetSizeOfMemoryInfoTable())
    || (ChronoIndex >= Dem_FaultMemory_Memories_GetMaxSize(TranslatedMemoryId))
    || (Dem_Cfg_GetChronoPtrOfMemoryInfoTable(TranslatedMemoryId) == NULL_PTR))
  {
    Dem_Error_RunTimeCheckFailed(DEM_MEMORIESIF_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    (Dem_Cfg_GetChronoPtrOfMemoryInfoTable(TranslatedMemoryId))[ChronoIndex] = EntryIndex;                                       /* SBSW_DEM_ARRAY_WRITE_MEMORYCHRONOLOGY */
    DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                               /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  }
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetFirstEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_GetFirstEntryIndex(
  uint16 TranslatedMemoryId
  )
{
  Dem_Cfg_EntryIndexType lResult = DEM_CFG_ENTRYINDEX_INVALID;
  if (TranslatedMemoryId < Dem_Cfg_GetSizeOfMemoryInfoTable())
  {
    DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                               /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
    lResult = Dem_Cfg_GetFirstEntryIndexOfMemoryInfoTable(TranslatedMemoryId);
  }
  return lResult;

}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetEndEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_GetEndEntryIndex(
  uint16 TranslatedMemoryId
)
{
#if(DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  Dem_Internal_AssertReturnValue(TranslatedMemoryId != DEM_CFG_MEMORYID_PERMANENT, DEM_E_INCONSISTENT_STATE, Dem_FaultMemory_Memories_GetFirstEntryIndex(TranslatedMemoryId));
#endif
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return (Dem_Cfg_EntryIndexType)(Dem_FaultMemory_Memories_GetFirstEntryIndex(TranslatedMemoryId)
                                   + Dem_FaultMemory_Memories_GetMaxSize(TranslatedMemoryId));
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetMaxSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FaultMemory_Memories_GetMaxSize(
  uint16 TranslatedMemoryId
  )
{
  uint8 lResult = 0u;
  if (TranslatedMemoryId < Dem_Cfg_GetSizeOfMemoryInfoTable())
  {
    lResult = Dem_Cfg_GetMaxSizeOfMemoryInfoTable(TranslatedMemoryId);
  }
  return lResult;
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetCurrentSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FaultMemory_Memories_GetCurrentSize(
  uint16 TranslatedMemoryId
  )
{
  uint8 lResult = 0u;
  if (TranslatedMemoryId < Dem_Cfg_GetSizeOfMemoryInfoTable())
  {
    lResult = Dem_Cfg_GetMemoryCurrentCount(TranslatedMemoryId);                                                                 /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
  }
  return lResult;
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_SetCurrentSize
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SetCurrentSize(
  uint16 TranslatedMemoryId,
  uint8 Size
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (TranslatedMemoryId >= Dem_Cfg_GetSizeOfMemoryCurrentCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_MEMORIESIF_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
  if (Size > Dem_FaultMemory_Memories_GetMaxSize(TranslatedMemoryId))
  {
    Dem_Error_RunTimeCheckFailed(DEM_MEMORIESIF_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    Dem_Cfg_SetMemoryCurrentCount(TranslatedMemoryId, Dem_FaultMemory_Memories_GetMaxSize(TranslatedMemoryId));                  /* SBSW_DEM_ARRAY_WRITE_MEMORYCURRENTCOUNT */
  }
  else
#endif
  {
    Dem_Cfg_SetMemoryCurrentCount(TranslatedMemoryId, Size);                                                                     /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_MEMORYCURRENTCOUNT */
  }
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetFirstEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_FaultMemory_Memories_GetFirstEvent(
  uint16 TranslatedMemoryId
  )
{
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return Dem_Cfg_GetFirstEventOfMemoryInfoTable(TranslatedMemoryId);                                                             /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetLastEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_FaultMemory_Memories_GetLastEvent(
  uint16 TranslatedMemoryId
  )
{
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return (Dem_EventIdType)Dem_Cfg_GetLastEventOfMemoryInfoTable(TranslatedMemoryId);                                             /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
}


/* ****************************************************************************
 % Dem_FaultMemory_Memories_SetInitAdminDataRequested
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SetInitAdminDataRequested(
  CONST(boolean, AUTOMATIC)  InitRequested
  )
{
  Dem_FaultMemory_Memories_InitAdminDataRequested = InitRequested;
}

#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"

/*!
 * \}
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Memory_Private Private Methods
 * \{
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_FaultMemory_Memories_Private Private Methods
 * \{
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_FaultMemory_Memories_Public
 * \{
 */

/* ****************************************************************************
 % Dem_FaultMemory_Memories_AllocateEventEntryAging
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
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_AllocateEventEntryAging(
  uint16 TranslatedMemoryId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_Cfg_EntryIndexType lEventEntryIndex;
  Dem_EventIdType lMemoryRepresentativeEvent;

  lMemoryRepresentativeEvent = Dem_Cfg_GetMemoryRepresentative(EventId);

  /* Find empty slot if possible, otherwise remove aged slot */
  if (Dem_FaultMemory_Memories_GetMaxSize(TranslatedMemoryId) == Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId))
  { /* Memory is filled up -
     * Aged environmental data is not displaced, the remaining slots (if any)
     * are still used -> no chance to allocate an aging counter */
    lEventEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
  }
  else
  { /* Memory is not full -> Find first free entry */
    lEventEntryIndex = Dem_FaultMemory_Memories_FindEntryIndexOfEventGeneric(DEM_EVENT_INVALID, FALSE, TranslatedMemoryId);
  }

  if (lEventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    uint8 lMemorySize = Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId);

    /* At this point, the event memory is not completely filled */
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if (lMemorySize >= Dem_FaultMemory_Memories_GetMaxSize(TranslatedMemoryId))
    {
      Dem_Error_RunTimeCheckFailed(DEM_MEMORIESIF_IMPLEMENTATION_FILENAME, __LINE__);                                            /* SBSW_DEM_POINTER_RUNTIME_CHECK */
      lEventEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
    }
    else
# endif
    {
      /* fill the chrono record with event id and lock the referenced PriMem slot */
      Dem_MemoryEntry_EventEntry_SetTimestamp(lEventEntryIndex, Dem_FaultMemory_Memories_GetCurrentTimestamp());
      Dem_FaultMemory_Memories_SetChronology(TranslatedMemoryId, lMemorySize, lEventEntryIndex);

      Dem_MemoryEntry_EventEntry_SetEventId(lEventEntryIndex, lMemoryRepresentativeEvent);

      /* Data */
      Dem_MemoryEntry_EventEntry_InitializeEntry(lEventEntryIndex);
      Dem_MemoryEntry_EventEntry_SetState(lEventEntryIndex, DEM_EVENTENTRY_SET_AGING_ONLY(0u));

      Dem_FaultMemory_Memories_SetCurrentTimestamp(Dem_FaultMemory_Memories_GetCurrentTimestamp() + 1u);
      Dem_FaultMemory_Memories_SetCurrentSize(TranslatedMemoryId, (uint8)(lMemorySize + 1u));
    }
  }

  return lEventEntryIndex;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_FaultMemory_Memories_UpdateChrono
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_UpdateChrono(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType EntryIndex
  )
{
  /* move downwards the chronology moving all entries, until the updated element is overwritten */
  uint8 lChronoIterator = (uint8)(Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId) - 1u);
  Dem_Cfg_EntryIndexType lTempEntryIndex = Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, lChronoIterator);

  while (lTempEntryIndex != EntryIndex)
  {
    Dem_Cfg_EntryIndexType lSwap = lTempEntryIndex;
    --lChronoIterator;
    lTempEntryIndex = Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, lChronoIterator);
    Dem_FaultMemory_Memories_SetChronology(TranslatedMemoryId, lChronoIterator, lSwap);

    if (lChronoIterator == 0u)
    {
      /* If the last index still not match, chronology is corrupted */
      Dem_Internal_AssertContinue((lTempEntryIndex == EntryIndex), DEM_E_INCONSISTENT_STATE)
      break;
    }
  }

  Dem_FaultMemory_Memories_SetChronology(TranslatedMemoryId, (uint8)(Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId) - 1u), EntryIndex);
}


/* ****************************************************************************
 % Dem_FaultMemory_Memories_RemoveEntryAndChrono
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_RemoveEntryAndChrono(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType EntryIndex
  )
{
  Dem_Cfg_EntryIndexType lTempChronoIndex;
  uint8 lChronoIterator;

  /* reduce count of used elements in the chrono stack */

  Dem_Internal_AssertReturnVoid(EntryIndex != DEM_CFG_ENTRYINDEX_INVALID, DEM_E_INCONSISTENT_STATE)

  lChronoIterator = Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId);
  --lChronoIterator;

  Dem_FaultMemory_Memories_SetCurrentSize(TranslatedMemoryId, lChronoIterator);
  Dem_FaultMemory_Memories_FreeEntry(TranslatedMemoryId, EntryIndex);

  /* go backwards through the chronological list and move down all entries, until the deleted element is overwritten */
  lTempChronoIndex = Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, lChronoIterator);
  while (lTempChronoIndex != EntryIndex)
  {
    Dem_Cfg_EntryIndexType lSwap = lTempChronoIndex;

    --lChronoIterator;
    lTempChronoIndex = Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, lChronoIterator);
    Dem_FaultMemory_Memories_SetChronology(TranslatedMemoryId, lChronoIterator, lSwap);

    if (lChronoIterator == 0u)
    {
      /* If the last index still not match, chronology is corrupted */
      Dem_Internal_AssertContinue((lTempChronoIndex == EntryIndex), DEM_E_INCONSISTENT_STATE)
      break;
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_FaultMemory_Memories_TranslateDTCOrigin
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
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_FaultMemory_Memories_TranslateDTCOrigin(
  Dem_DTCOriginType  DTCOrigin,
  CONSTP2VAR(uint16, AUTOMATIC, AUTOMATIC)  TranslatedMemoryId
  )
{
  Std_ReturnType lTranslationStatus;
  lTranslationStatus = E_NOT_OK;

  switch (DTCOrigin)
  {
  case DEM_DTC_ORIGIN_PRIMARY_MEMORY:
    *TranslatedMemoryId = DEM_CFG_MEMORYID_PRIMARY;                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    lTranslationStatus = E_OK;
    break;

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
  case DEM_DTC_ORIGIN_PERMANENT_MEMORY:
    if (Dem_Cfg_IsPermanentSupportedInVariant() == TRUE)
    {
      *TranslatedMemoryId = DEM_CFG_MEMORYID_PERMANENT;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      lTranslationStatus = E_OK;
    }
    break;
#endif

  default:
    if ((Dem_Memories_EventMemory_UserDefined_IsEnabled() == TRUE)
        && (Dem_Memories_EventMemory_UserDefined_IsValid(DTCOrigin) == TRUE))
    {
      uint16 lTranslatedUserDefinedMemoryId;
      lTranslatedUserDefinedMemoryId = Dem_Memories_EventMemory_UserDefined_TranslateDTCOrigin(DTCOrigin);
      if (lTranslatedUserDefinedMemoryId != DEM_CFG_MEMORYID_PRIMARY)
      {
        *TranslatedMemoryId = lTranslatedUserDefinedMemoryId;                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        lTranslationStatus = E_OK;
      }
    }
    break;
  }

    return lTranslationStatus;
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_TestEntryforEventGeneric
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_Memories_TestEntryforEventGeneric(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType EntryIndex,
  Dem_EventIdType EventId
  )
{
  boolean lRetVal = FALSE;

  if (Dem_Memories_EventMemory_IsReferredByMemoryId(TranslatedMemoryId) == TRUE)
  {
    lRetVal = Dem_MemoryEntry_EventEntry_TestEntryForEvent(EntryIndex, EventId);
  }
  else 
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) 
  if (Dem_Memories_TimeSeriesMemory_IsReferredByMemoryId(TranslatedMemoryId))
  {
    lRetVal = Dem_MemoryEntry_TimerSeriesEntry_TestEntryForEvent(EntryIndex, EventId);
  }
  else
#endif
  if (Dem_Memories_CustomTriggerMemory_IsReferredByMemoryId(TranslatedMemoryId))
  {
    lRetVal = Dem_MemoryEntry_CustomTriggerEntry_TestEntryForEvent(EntryIndex, EventId);
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
  
  return lRetVal;
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_FindEntryIndexOfEventGeneric
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_FindEntryIndexOfEventGeneric(
  Dem_EventIdType EventId,
  boolean VerifyEventStored,
  uint16 ExpectedTranslatedMemoryId
)
{
  Dem_Cfg_EntryIndexType lEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
  Dem_EventIdType lMemoryRepresentativeEvent = Dem_Cfg_GetMemoryRepresentative(EventId);
  Dem_Cfg_EntryIterType lEventIter;

  for(Dem_Cfg_ComplexIter32BitInit(&lEventIter,                                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_FaultMemory_Memories_GetFirstEntryIndex(ExpectedTranslatedMemoryId),
        Dem_FaultMemory_Memories_GetEndEntryIndex(ExpectedTranslatedMemoryId));
      Dem_Cfg_ComplexIter32BitExists(&lEventIter) == TRUE;                                                                       /* PRQA S 2990 */ /* MD_DEM_2990 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIter32BitNext(&lEventIter))                                                                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_EntryIndexType lCurrentEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lEventIter);                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
    if (Dem_FaultMemory_Memories_TestEntryforEventGeneric(ExpectedTranslatedMemoryId, lCurrentEntryIndex, lMemoryRepresentativeEvent) == TRUE)
    {
      lEntryIndex = lCurrentEntryIndex;
      break;
    }
  }
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  /* Cannot happen except after the Dem state becomes inconsistent */
  if ((VerifyEventStored == TRUE) && (lEntryIndex == DEM_CFG_ENTRYINDEX_INVALID))
  {
    Dem_Error_RunTimeCheckFailed(DEM_MEMORIESIF_IMPLEMENTATION_FILENAME, __LINE__);                                              /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(VerifyEventStored);
#endif
  return lEntryIndex;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_FaultMemory_Memories_FreeEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_FreeEntry(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType EntryIndex
  )
{
  if (Dem_Memories_EventMemory_IsReferredByMemoryId(TranslatedMemoryId) == TRUE)
  {
    Dem_MemoryEntry_EventEntry_Free(EntryIndex);
  }
  else
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON) 
  if (Dem_Memories_TimeSeriesMemory_IsReferredByMemoryId(TranslatedMemoryId))
  {
    Dem_MemoryEntry_TimeSeriesEntry_Free(EntryIndex);
  }
  else
#endif
  if (Dem_Memories_CustomTriggerMemory_IsReferredByMemoryId(TranslatedMemoryId))
  {
    Dem_MemoryEntry_CustomTriggerEntry_Free(EntryIndex);
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_AllocateEventEntryIndexAging
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_AllocateEventEntryIndexAging(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  return Dem_FaultMemory_Memories_AllocateEventEntryAging(Dem_Core_Event_GetDestination(EventId), EventId);
}

#if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetOverflowOfDTCOrigin
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_FaultMemory_Memories_GetOverflowOfDTCOrigin(
  CONST(Dem_DTCOriginType, AUTOMATIC)  DTCOrigin,
  P2VAR(boolean, AUTOMATIC, DEM_APPL_DATA)  OverflowIndication
  )
{
  Std_ReturnType lReturnValue;
  uint16 lTranslatedMemoryId;

  if (Dem_FaultMemory_Memories_TranslateDTCOrigin(DTCOrigin, &lTranslatedMemoryId) == E_OK)                                      /* SBSW_DEM_POINTER_FORWARD_STACK */
  {
    *OverflowIndication = Dem_FaultMemory_Memories_GetOverflow(lTranslatedMemoryId);                                             /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    lReturnValue = E_OK;
  }
  else
  {
    lReturnValue = E_NOT_OK;
  }

  return lReturnValue;
}
#endif

/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetNumAllocatedMemoryEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_FaultMemory_Memories_GetNumAllocatedMemoryEntry(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16  TranslatedMemoryId
  )
{
  return Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId);
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_ResetOverflow
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_ResetOverflow(
  uint16 TranslatedMemoryId
  )
{
#if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
  Dem_Com_ResetBitInBitfield(&(Dem_Cfg_AdminData.MemoryOverflow[0]), DEM_CFG_MAX_SIZE_OVERFLOWINDICATOR, TranslatedMemoryId);    /* SBSW_DEM_POINTER_FORWARD_ADMIN_DATA_MEMORY_OVERFLOW */
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TranslatedMemoryId)
}

#if (DEM_FEATURE_NEED_CYCLE_COUNTERS == STD_ON)
/* ****************************************************************************
 % Dem_FaultMemory_Memories_LatchCycleCounters
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_LatchCycleCounters(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  uint16 CycleCounter
)
{
  boolean lDirty;
  lDirty = Dem_FaultMemory_StorageManager_TestCycleCounterLatched(EventEntryIndex, CycleCounter);
  if (lDirty == TRUE)
  {
    Dem_Nvm_SetEventEntryNvBlockState(EventEntryIndex, DEM_NVM_BLOCKSTATE_DIRTY);
  }
}
#endif

/* ****************************************************************************
 % Dem_FaultMemory_Memories_Init
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
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_Init(
  void
  )
{
  boolean lResetStoredData;
  Dem_EventIdType lEventId;
  uint16 lDebounceNvIndex = 0u;

  for (lEventId = DEM_EVENT_FIRST; lEventId < Dem_Cfg_GlobalEventCount(); ++lEventId)
  { /* Only reset stored status, since debouncing can have changed since pre-initialization */
    Dem_Event_InternalStatus_SetStoredStatus(lEventId, Dem_InternalStatus_StoredStatus_None);

    /* If NV storage of debounce values is configured, and the event uses that feature */
    /* It's safe to do it here since supporting events are not allowed to report before Dem_Init() */
    if (Dem_Event_TestDebounceCounterStoredInNv(lEventId) == TRUE)
    {
      boolean lFailedThresholdReached = Dem_Event_UDSStatus_TestBit(Dem_Event_UDSStatus_GetInternalEventUdsStatus(lEventId), Dem_UdsStatus_TF);

      Dem_SatelliteCore_RestoreDebounceValue(
        Dem_Cfg_EventSatelliteId(lEventId),
        Dem_Cfg_EventSatelliteEventId(lEventId),
        lDebounceNvIndex,
        lFailedThresholdReached);

      ++lDebounceNvIndex;
    }
  }

#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  {
    Dem_Cfg_CombinedGroupIndexType lGroupId;
    for (lGroupId = 0u; lGroupId < Dem_Cfg_GlobalCombinedGroupCount(); ++lGroupId)
    {
      Dem_Event_InternalStatusType lInternalStatus;
      Dem_Cfg_CombinedGroupIterType lSubEventIter;

      /* Initialize the combination group internal status */
      lInternalStatus = 0x00u;

      /* For all combination groups, test if at least one sub-events is available by configuration.
         Initialization in PreInit and Init can have different settings for
         event availability, so repeat the calculation for combined DTCs */
      for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lSubEventIter);                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
           Dem_Cfg_CombinedGroupIterExists(&lSubEventIter) == TRUE;                                                              /* PRQA S 2990 */ /* MD_DEM_2990 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
           Dem_Cfg_CombinedGroupIterNext(&lSubEventIter))                                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        if (Dem_Cfg_EventAvailableByVariant(Dem_Cfg_CombinedGroupIterGet(&lSubEventIter)) == TRUE)                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
        { /* If so, set the availability in the combination status. */
          lInternalStatus = Dem_Event_InternalStatus_SetBit(lInternalStatus, Dem_InternalStatus_AvailableInVariant);
          break;
        }
      }
      Dem_Event_InternalStatus_SetCombinedGroupStatus(lGroupId, lInternalStatus);
    }
  }
#endif

  Dem_Memories_TimeSeriesMemory_Init();

  lResetStoredData = (boolean)( (Dem_Cfg_AdminData.ImplementationVersion != Dem_Make16Bit(DEM_SW_MAJOR_VERSION, DEM_SW_MINOR_VERSION))
                           || (Dem_Cfg_AdminData.ConfigurationId != Dem_Cfg_GlobalConfigurationId())                             
                           || (Dem_FaultMemory_Memories_GetInitAdminDataRequested() == TRUE));                                                 

  if (lResetStoredData == TRUE)
  {
    /* If the NV data is not compatible to the configuration or could not be restored by NvM */
    /* Re-initialize all NV data content and reset the global time stamp */

    Dem_Nvm_SetAllBlocksDirty();

    /* Do not reinitialize an already initialized Admin data block */
    if (Dem_FaultMemory_Memories_GetInitAdminDataRequested() == FALSE)
    {
      (void)Dem_NvM_InitAdminData();
    }

    /* Reset flag to prevent re-initialization on Shutdown-MasterInit*/
    Dem_FaultMemory_Memories_SetInitAdminDataRequested(FALSE);

    (void)Dem_NvM_InitStatusData();

    if (Dem_Nvm_IsDebounceCounterStorageSupported() == TRUE)
    {
      (void)Dem_NvM_InitDebounceData();
    }

    if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
    {
      (void)Dem_NvM_InitObdFreezeFrameData();
    }

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
    if ( (Dem_Cfg_IsPermanentSupportedInVariant() == TRUE)
      && (Dem_Memories_PermanentMemory_GetInitPattern() != DEM_PERMANENTMEMORY_INIT_PATTERN) )
    { /* only initialize permanent memory if it was not initialized before */
      (void)Dem_NvM_InitObdPermanentData();
    }
#endif
    if (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
    {
      Dem_Iumpr_InitNv();

#if (DEM_CFG_SUPPORT_DTR == STD_ON)
      (void)Dem_NvM_InitDtrData();
#endif
    }

#if (DEM_FEATURE_NEED_AGING_DATA == STD_ON)
    (void)Dem_NvM_InitAgingData();
#endif
    Dem_DTC_InitMemIndependentCycleCounterDataNv();

#if (DEM_CFG_SUPPORT_EVENTAVAILABLE_NV == STD_ON)
    (void)Dem_NvM_InitEventAvailableData();
#endif

    Dem_FaultMemory_Memories_SetCurrentTimestamp(1U);

    {
      /* Translated memory index for user defined memories range from 0x01 - Number of configured memories 
         Memory Index 0x00 is reserved for Primary */
      uint16 lTranslatedMemoryId;
   
      for (lTranslatedMemoryId = 0;
        lTranslatedMemoryId <= Dem_Memories_EventMemory_UserDefined_GetNumberOfUserDefinedMemories();
        lTranslatedMemoryId++)
      {
        Dem_FaultMemory_Memories_SetCurrentSize((lTranslatedMemoryId), 0);
      }
    }

    if (Dem_Cfg_IsPermanentSupportedInVariant() == TRUE)
    {
      Dem_FaultMemory_Memories_SetCurrentSize(DEM_CFG_MEMORYID_PERMANENT, 0);
    }

    {
      Dem_Cfg_EntryIterType lAllEventsIter;
      for (Dem_Cfg_ComplexIter32BitInit(&lAllEventsIter, 0u, Dem_Memories_EventMemory_GetTotalSize());                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
           Dem_Cfg_ComplexIter32BitExists(&lAllEventsIter) == TRUE;                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
           Dem_Cfg_ComplexIter32BitNext(&lAllEventsIter))                                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lAllEventsIter);                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_MemoryEntry_EventEntry_Init(lEventEntryIndex);
      }
    }
    Dem_Memories_TimeSeriesMemory_InitNvData();
    Dem_Memories_CustomTriggerMemory_InitNvData();
  }
  /* Otherwise */
  else
  {
    uint16 lTranslatedMemoryIndex = 0u;
    Dem_FaultMemory_Memories_SetCurrentTimestamp(0U);
    /* Translated memory index for user defined memories range from 0x01 - Number of configured memories
       Memory Index 0x00 is reserved for Primary */


    for (;lTranslatedMemoryIndex <= Dem_Memories_EventMemory_UserDefined_GetNumberOfUserDefinedMemories();
      lTranslatedMemoryIndex++)
    {
      Dem_Memories_EventMemory_Restore(lTranslatedMemoryIndex);
    }
  }
#if (DEM_CFG_SUPPORT_OBDII == STD_ON)
  if (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE)
  {
    /* In OBD configurations, initialize the visible OBD freeze frame */
    Dem_ObdFreezeFrameMemory_FreezeFrameIndexMode02 = Dem_Memories_ObdFreezeFrameMemory_SelectHighestPriorFreezeFrame();
  }
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
  /* If prestore freeze frame is enabled, initialize freeze frame prestorage */
  Dem_Memories_PrestorageMemory_Init();
#endif

  /* If custom triggered freeze frame is enabled, initialize custom triggered freeze frame storage */
  Dem_Memories_CustomTriggerMemory_Init();

  Dem_Core_StatusIndicator_Init();
}                                                                                                                                /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 % Dem_FaultMemory_Memories_InitConfirmedChronology
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_InitConfirmedChronology(
  void
  )
{
  Dem_Memories_PermanentMemory_ConfirmedChrono_Init();
}
#endif

/* ****************************************************************************
 % Dem_FaultMemory_Memories_InitOverflow
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_InitOverflow(
  void
  )
{
#if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
  Dem_MemSet((Dem_DataPtrType) &(Dem_Cfg_AdminData.MemoryOverflow[0]),                                                           /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */ /* SBSW_DEM_MEMSET_ADMIN_DATA_MEMORY_OVERFLOW */
    0x00,
    DEM_CFG_MAX_SIZE_OVERFLOWINDICATOR * sizeof(Dem_Cfg_AdminData.MemoryOverflow[0]));
#endif
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_SetOverflow
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SetOverflow(
  uint16 TranslatedMemoryId
  )
{
#if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
  Dem_Com_SetBitInBitfield(&(Dem_Cfg_AdminData.MemoryOverflow[0]), DEM_CFG_MAX_SIZE_OVERFLOWINDICATOR, TranslatedMemoryId);      /* SBSW_DEM_POINTER_FORWARD_ADMIN_DATA_MEMORY_OVERFLOW */
#endif
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryId)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_GetOverflow
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_FaultMemory_Memories_GetOverflow(                                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16 TranslatedMemoryHandle
  )
{
#if (DEM_FEATURE_NEED_OVFLIND == STD_ON)
  return Dem_Com_TestBitInBitfield(&(Dem_Cfg_AdminData.MemoryOverflow[0]), DEM_CFG_MAX_SIZE_OVERFLOWINDICATOR,                   /* SBSW_DEM_POINTER_FORWARD_ADMIN_DATA_MEMORY_OVERFLOW */ 
                                    TranslatedMemoryHandle); 
#else
  DEM_IGNORE_UNUSED_ARGUMENT(TranslatedMemoryHandle)                                                                             /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return FALSE;
#endif
}


/* ****************************************************************************
 % Dem_FaultMemory_Memories_SortEventEntryIntoChronologicalList
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_FaultMemory_Memories_SortEventEntryIntoChronologicalList(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  uint32 Timestamp
  )
{
  uint8 lSortedIndex;
  for (lSortedIndex = Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId);
        (lSortedIndex > 0u)  && (Dem_MemoryEntry_EventEntry_GetTimestamp(                                                        /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
                                Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, (uint8)(lSortedIndex - 1u)))  > Timestamp);
       --lSortedIndex
    )
  {
    Dem_FaultMemory_Memories_SetChronology(
      TranslatedMemoryId,
      lSortedIndex,
      Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, (uint8)(lSortedIndex - 1u)));
  }
  Dem_FaultMemory_Memories_SetChronology(TranslatedMemoryId, lSortedIndex, EventEntryIndex);
}

/* ****************************************************************************
 % Dem_FaultMemory_Memories_AllocateEventEntry
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
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_FaultMemory_Memories_AllocateEventEntry(
  uint16 TranslatedMemoryId,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  Dem_EventIdType lMemoryRepresentativeEvent = Dem_Cfg_GetMemoryRepresentative(EventId);
  Dem_Cfg_EntryIndexType lEventEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;

  if (Dem_FaultMemory_Memories_GetMaxSize(TranslatedMemoryId) == Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId))
  { /* Memory is filled up */
    Dem_FaultMemory_Memories_SetOverflow(Dem_Core_Event_GetDestination(EventId));

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING != STD_ON)
    lEventEntryIndex = Dem_StorageManager_Displacement_SelectDisplacedIndex(TranslatedMemoryId, EventId);
#else
    lEventEntryIndex = Dem_StorageManager_Displacement_SelectCustomizedDisplacedIndex(TranslatedMemoryId, EventId);
#endif

#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON) || (DEM_CFG_SUPPORT_DISPLACEMENT == STD_ON)
    if (lEventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
    {
      /* a displaced DTC was identified, now remove that DTC */
      Dem_EventIdType lDisplacedEventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);

      if (Dem_Event_TestValidHandle(lDisplacedEventId) == TRUE)
      {
        Dem_Event_DataUpdateStart(lDisplacedEventId);
# if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
        Dem_DTC_CustomizedDisplacement(lDisplacedEventId, Dem_MemoryEntry_EventEntry_GetPreConfirmedStatus(lEventEntryIndex));
# else
        Dem_DTC_Displaced(lDisplacedEventId);
# endif

        Dem_Memories_TimeSeriesMemory_InvalidateEntryOfEvent(lDisplacedEventId, DEM_NVM_BLOCKSTATE_DIRTY);
        Dem_Memories_CustomTriggerMemory_ClearEvent(lDisplacedEventId, DEM_NVM_BLOCKSTATE_DIRTY);

        if (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
        {
          Dem_Memories_ObdFreezeFrameMemory_EventDisplaced(lDisplacedEventId);
        }
        Dem_Event_DataUpdateFinish(lDisplacedEventId);
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_OFF)
        if (Dem_DTC_TestEventStoresPermanentDTC(EventId) == TRUE)
        {
          Dem_Memories_PermanentMemory_ClearEvent(lDisplacedEventId, FALSE);
        }
# endif
      }
      Dem_FaultMemory_Memories_RemoveEntryAndChrono(TranslatedMemoryId, lEventEntryIndex);
    }
    /* else: no adequate DTC found, the new entry cannot be added - return an invalid index */
#endif
  }
  else
  { /* primary memory is not full */
    /* Memory is not full -> Find first free entry */
    lEventEntryIndex = Dem_FaultMemory_Memories_FindEntryIndexOfEventGeneric(DEM_EVENT_INVALID, FALSE, TranslatedMemoryId);
    /* if not found : lEventEntryIndex == DEM_CFG_ENTRYINDEX_INVALID */
  }

  if (lEventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    uint8 lMemorySize = Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId);

    /* At this point, the event memory is not completely filled */
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if (lMemorySize >= Dem_FaultMemory_Memories_GetMaxSize(TranslatedMemoryId))
    {
      Dem_Error_RunTimeCheckFailed(DEM_MEMORIESIF_IMPLEMENTATION_FILENAME, __LINE__);                                            /* SBSW_DEM_POINTER_RUNTIME_CHECK */
      lEventEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
    }
    else
#endif
    {
      /* fill the chrono record with event id and lock the referenced PriMem slot */
      Dem_FaultMemory_Memories_SetChronology(TranslatedMemoryId, lMemorySize, lEventEntryIndex);

      Dem_MemoryEntry_EventEntry_SetEventId(lEventEntryIndex, lMemoryRepresentativeEvent);
      Dem_MemoryEntry_EventEntry_SetTimestamp(lEventEntryIndex, Dem_FaultMemory_Memories_GetCurrentTimestamp());

      Dem_MemoryEntry_EventEntry_InitializeEntry(lEventEntryIndex);
      Dem_FaultMemory_Memories_SetCurrentTimestamp(Dem_FaultMemory_Memories_GetCurrentTimestamp() + 1u);
      Dem_FaultMemory_Memories_SetCurrentSize(TranslatedMemoryId, (uint8)(lMemorySize + 1u));
    }
  }
  return lEventEntryIndex;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_MEMORIESIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_MemoriesIF_Implementation.h
 *********************************************************************************************************************/
