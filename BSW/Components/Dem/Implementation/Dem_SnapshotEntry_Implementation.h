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
/*! \addtogroup Dem_SnapshotEntry
 *  \{
 *  \file       Dem_SnapshotEntry_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Encapsulates the list of snapshot record buffers stored in the event entry
 *  \entity     SnapshotEntry
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_SNAPSHOTENTRY_IMPLEMENTATION_H)
#define DEM_SNAPSHOTENTRY_IMPLEMENTATION_H

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

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_SnapshotEntry_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Core_Interface.h"
#include "Dem_RecordReader_Interface.h"
#include "Dem_Memories_Interface.h"
#include "Dem_MemoryEntryIF_Interface.h"
#include "Dem_EventEntry_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_SNAPSHOTENTRY_IMPLEMENTATION_FILENAME "Dem_SnapshotEntry_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/


/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_SnapshotEntry_Private Private Methods
 * \{
 */
#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorIsInvalidated
 *****************************************************************************/
/*!
 * \brief         Test if the snapshot entry is in state 'invalidated'
 *
 * \details       Test if the snapshot entry is in state 'invalidated'
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON &&
 *                DEM_CFG_SUPPORT_DCM == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorIsInvalidated(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  );
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored
 *****************************************************************************/
/*!
 * \brief         Tests whether a combined snapshot record is currently stored.
 *
 * \details       Tests whether a combined snapshot record pointed to by the iterator
 *                is currently stored.
 *
 * \param[in]     SnapshotEntryIter
 *                Pointer to the snapshot entry iterator
 *
 * \return        TRUE
 *                The snapshot record is stored
 * \return        FALSE
 *                The snapshot record is not stored
 *
 * \pre           SnapshotEntryIter->EventEntryIndex must be inside valid range
 * \config        DEM_CFG_SUPPORT_SRECS == STD_ON
 *                && DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored(
CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
);
#endif

/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_GetFirstSRecNumIdxExcludingCustomTrigger
 *****************************************************************************/
/*!
 * \brief         Get the event's first idx in Freeze Frame Record Table with
 *                trigger other than custom
 *
 * \details       Jump over custom triggered SRec since they are not stored
 *                into event entry snapshot buffer. Instead they are stored
 *                into a separate memory entry.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The first referenced idx in Freeze Frame Record Table with
 *                trigger other than custom
 *                If the event references no SRec or only custom triggered SRec,
 *                Dem_Cfg_GetFreezeFrameNumTableEndIdxOfEventTable() is returned
 *                
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecIndexType, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_GetFirstSRecNumIdxExcludingCustomTrigger(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_MemoryEntry_SnapshotEntry_GetNextSRecNumIdxExcludingCustomTrigger
 *****************************************************************************/
/*!
 * \brief         Get the event's next idx in Freeze Frame Record Table with
 *                trigger other than custom.
 *
 * \details       Jump over custom triggered SRec since they are not stored
 *                into event entry snapshot buffer. Instead they are stored
 *                into a separate memory entry.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     CurrentSRecIdx
 *                Current Idx in Freeze Frame Record Table
 *
  * \return       The first referenced idx in Freeze Frame Record Table with
 *                trigger other than custom.
 *                If no martching SRec idx can be found as the next Idx,
 *                Dem_Cfg_GetFreezeFrameNumTableEndIdxOfEventTable() is returned
 *
 * \pre           The CurrentSRecIdx must be referenced by the event 
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecIndexType, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_GetNextSRecNumIdxExcludingCustomTrigger(
  Dem_EventIdType EventId,
  Dem_Cfg_SRecIndexType CurrentSRecIndex
  );


/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorIsInvalidated
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorIsInvalidated(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  return (boolean)(SnapshotEntryIter->EventEntryIndex == DEM_CFG_ENTRYINDEX_INVALID);
}
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored
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
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored(
CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  boolean lSnapshotRecordIsStored;
  Dem_EventIdType lEventId;
  Dem_Cfg_EntryIterType lPrimaryEventEntryIter;

  lSnapshotRecordIsStored = FALSE;

  if (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)                                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_SRecNumerationType lSrecNumerationType = Dem_FaultMemory_Memories_GetSRecNumerationType(DEM_CFG_MEMORYID_PRIMARY);

    for (Dem_Cfg_ComplexIter32BitInit(&lPrimaryEventEntryIter, SnapshotEntryIter->EventEntryIndex, Dem_FaultMemory_Memories_GetEndEntryIndex(DEM_CFG_MEMORYID_PRIMARY)); /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIter32BitExists(&lPrimaryEventEntryIter) == TRUE;                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIter32BitNext(&lPrimaryEventEntryIter))                                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lPrimaryEventEntryIter);                            /* SBSW_DEM_CALL_ITERATOR_POINTER */

      lEventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);
      if (Dem_Cfg_EventCombinedGroup(lEventId) == SnapshotEntryIter->CombinedDTCIndex)
      {
        if (Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculated(lSrecNumerationType))
        {
          if (Dem_MemoryEntry_EventEntry_GetSnapshotHeader(lEventEntryIndex) > SnapshotEntryIter->SnapshotEntryIndex)
          {
            lSnapshotRecordIsStored = TRUE;
          }
        }
        else /* Uses configured records */
        {
          uint32 lMask;

          lMask = (uint32)1 << SnapshotEntryIter->CombinedDTCSnapshotEntryIndex;

          if ((Dem_MemoryEntry_EventEntry_GetCombinedDTCSnapshotHeader(lEventEntryIndex) & lMask) != 0u)
          {
            lSnapshotRecordIsStored = TRUE;
          }
        }

        if (lSnapshotRecordIsStored == TRUE)
        {
          break;
        }
      }
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
  return lSnapshotRecordIsStored;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) */

/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_GetFirstSRecNumIdxExcludingCustomTrigger
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
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecIndexType, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_GetFirstSRecNumIdxExcludingCustomTrigger(                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
  )
{
  Dem_Cfg_SRecIndexType lSartSRecNumIdx = Dem_Cfg_SRecNumStartIdx(EventId);

#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_SREC == STD_ON)
  if (Dem_Core_TestCustomTriggerConfigured(EventId) == TRUE)
  {
    Dem_Cfg_ComplexIterType lSRecIter;

    for (Dem_Cfg_ComplexIterInit(&lSRecIter, lSartSRecNumIdx, Dem_Cfg_SRecNumEndIdx(EventId));                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      (Dem_Cfg_ComplexIterExists(&lSRecIter) == TRUE);                                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterNext(&lSRecIter))                                                                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      if (Dem_Cfg_SRecTrigger(Dem_Cfg_ComplexIterGet(&lSRecIter)) != DEM_CFG_TRIGGER_CUSTOM)                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        break;
      }
    }

    lSartSRecNumIdx = (Dem_Cfg_SRecIndexType)(Dem_Cfg_ComplexIterGet(&lSRecIter));                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  }
#endif

  return lSartSRecNumIdx;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_GetNextSRecNumIdxExcludingCustomTrigger
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
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecIndexType, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_GetNextSRecNumIdxExcludingCustomTrigger(                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId,
  Dem_Cfg_SRecIndexType CurrentSRecIndex
  )
{
  Dem_Cfg_SRecIndexType lNextSRecNumIdx = (Dem_Cfg_SRecIndexType)(CurrentSRecIndex + 1u);

#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_SREC == STD_ON)
  if (Dem_Core_TestCustomTriggerConfigured(EventId) == TRUE)
  {
    Dem_Cfg_ComplexIterType lSRecIter;

    for (Dem_Cfg_ComplexIterInit(&lSRecIter, lNextSRecNumIdx, Dem_Cfg_SRecNumEndIdx(EventId));                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
      (Dem_Cfg_ComplexIterExists(&lSRecIter) == TRUE);                                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_ComplexIterNext(&lSRecIter))                                                                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      if (Dem_Cfg_SRecTrigger(Dem_Cfg_ComplexIterGet(&lSRecIter)) != DEM_CFG_TRIGGER_CUSTOM)                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        break;
      }
    }

    lNextSRecNumIdx = (Dem_Cfg_SRecIndexType)(Dem_Cfg_ComplexIterGet(&lSRecIter));                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif

  return lNextSRecNumIdx;
}


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
 * \addtogroup Dem_MemoryEntry_SnapshotEntry_Public
 * \{
 */

#if ((DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)\
    && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_SRecIsSupportedByAvailableCombinedEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_SRecIsSupportedByAvailableCombinedEvent(
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  CombinedDTCGroup,
  CONST(uint8, AUTOMATIC)  SRecId
  )
{
  boolean lMatch;
  lMatch = FALSE;

  if ( (Dem_RecordReader_0x1904_IsObdIIFreezeFrameInService19EnabledInVariant() == TRUE)
    && (SRecId == DEM_RECORDREADER_SNAPSHOTDATARECORD_OBD) )
  /* global OBD II freeze frame is always supported */
  {
    lMatch = TRUE;
  }
  else
  {
    Dem_Cfg_FreezeFrameRecordEventIterType lFreezeFrameRecordEventIter;

    for (Dem_Cfg_FreezeFrameRecordEventIterInit(&lFreezeFrameRecordEventIter, CombinedDTCGroup, SRecId);                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_FreezeFrameRecordEventIterExists(&lFreezeFrameRecordEventIter) == TRUE;                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_FreezeFrameRecordEventIterNext(&lFreezeFrameRecordEventIter))                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lEventId;
      lEventId = Dem_Cfg_FreezeFrameRecordEventIterGet(&lFreezeFrameRecordEventIter);                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */

      if ((Dem_Cfg_EventAvailableByVariant(lEventId) == TRUE)
        && (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lEventId), Dem_InternalStatus_Disconnected) == FALSE))
      {
        lMatch = TRUE;
        break;
      }
    }
  }

  return lMatch;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (DEM_CFG_SUPPORT_DCM == STD_ON) 
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorInvalidate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorInvalidate(
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter                                            /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  SnapshotEntryIter->EventEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SnapshotEntryIter)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorInit
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
Dem_MemoryEntry_SnapshotEntry_IteratorInit(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  SnapshotEntryIter->EventId = EventId;                                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  SnapshotEntryIter->EventEntryIndex = EventEntryIndex;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  SnapshotEntryIter->SnapshotEntryIndex = 0u;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  SnapshotEntryIter->SnapshotFifoEntryIndex = 0u;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

  if (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    /* Jump over custom triggered SRec since they are not stored into event memory entry but in separate memory */
    SnapshotEntryIter->SnapshotIteratorIndex = Dem_MemoryEntry_SnapshotEntry_GetFirstSRecNumIdxExcludingCustomTrigger(EventId);  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

    if (Dem_Event_IsCalculatedFifoSRecsUsed(EventId) == TRUE)
    {
      if (Dem_MemoryEntry_EventEntry_GetSnapshotHeader(SnapshotEntryIter->EventEntryIndex) !=
          Dem_MemoryEntry_EventEntry_GetSnapshotHeaderFifo(SnapshotEntryIter->EventEntryIndex))
      {
        SnapshotEntryIter->SnapshotEntryIndex =
          Dem_MemoryEntry_EventEntry_GetSnapshotHeaderFifo(SnapshotEntryIter->EventEntryIndex);                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
    }
  }
  else
  {
    SnapshotEntryIter->SnapshotIteratorIndex = 0u;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorNext
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
Dem_MemoryEntry_SnapshotEntry_IteratorNext(
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  if (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    Dem_EventIdType lEventId = SnapshotEntryIter->EventId;
    /* Jump over custom triggered SRec since they are not stored into event memory entry but in separate memory */
    SnapshotEntryIter->SnapshotIteratorIndex = Dem_MemoryEntry_SnapshotEntry_GetNextSRecNumIdxExcludingCustomTrigger(lEventId, SnapshotEntryIter->SnapshotIteratorIndex); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    SnapshotEntryIter->SnapshotEntryIndex++;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    SnapshotEntryIter->SnapshotFifoEntryIndex++;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

    if (Dem_Event_IsCalculatedFifoSRecsUsed(lEventId) == TRUE)
    {
      if (Dem_MemoryEntry_EventEntry_GetSnapshotHeader(SnapshotEntryIter->EventEntryIndex) !=
        Dem_MemoryEntry_EventEntry_GetSnapshotHeaderFifo(SnapshotEntryIter->EventEntryIndex))
      {
        if (SnapshotEntryIter->SnapshotEntryIndex >= Dem_MemoryEntry_EventEntry_GetSnapshotHeader(SnapshotEntryIter->EventEntryIndex))
        {
          SnapshotEntryIter->SnapshotEntryIndex = 0u;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        }
      }
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorExists
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorExists(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  boolean lIteratorExists;
  if (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    lIteratorExists = (boolean)(SnapshotEntryIter->SnapshotIteratorIndex <
      Dem_Cfg_SRecNumEndIdx(SnapshotEntryIter->EventId));
  }
  else
  {
    lIteratorExists = FALSE;
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
  return lIteratorExists;
}
#endif


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorInit
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorInit(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  SnapshotEntryIter->EventId = EventId;                                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  SnapshotEntryIter->EventEntryIndex = EventEntryIndex;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  SnapshotEntryIter->SnapshotEntryIndex = 0u;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  if (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    SnapshotEntryIter->SnapshotIteratorIndex =
      Dem_Cfg_GetFreezeFrameNumTableStartIdxOfEventTable(EventId);                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */

    SnapshotEntryIter->CombinedDTCIndex = Dem_Cfg_EventCombinedGroup(EventId);                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      SnapshotEntryIter->CombinedDTCSnapshotEntryIndex = 0u;                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      Dem_Cfg_DTCFreezeFrameRecordIterInit(
        &(SnapshotEntryIter->CombinedDTCSnapShotIterator)                                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
        , SnapshotEntryIter->CombinedDTCIndex);
    }
  }
  else
  {
    SnapshotEntryIter->SnapshotIteratorIndex = 0u;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorNext
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
Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorNext(
CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  Dem_Cfg_SRecNumerationType lSrecNumerationType = Dem_FaultMemory_Memories_GetSRecNumerationType(DEM_CFG_MEMORYID_PRIMARY);

  if (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    {
      if (Dem_Memories_EventMemory_UserDefined_IsSRecNumerationConfigured(lSrecNumerationType) == TRUE)
      {
        Dem_Cfg_DTCFreezeFrameRecordIterNext(&SnapshotEntryIter->CombinedDTCSnapShotIterator);                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }
      else
      {
        SnapshotEntryIter->SnapshotEntryIndex++;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      }

      SnapshotEntryIter->CombinedDTCSnapshotEntryIndex++;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      SnapshotEntryIter->SnapshotIteratorIndex++;                                                                                /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      SnapshotEntryIter->SnapshotEntryIndex++;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorExists
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
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorExists(
CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  boolean lIteratorExists;
  if (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) /* Configured */
    if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      lIteratorExists = Dem_Cfg_DTCFreezeFrameRecordIterExists(&SnapshotEntryIter->CombinedDTCSnapShotIterator);                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
    }
    else
# endif
    {
      lIteratorExists = (boolean)(SnapshotEntryIter->SnapshotIteratorIndex <
        Dem_Cfg_SRecNumEndIdx(SnapshotEntryIter->EventId));
    }
  }
  else
  {
    lIteratorExists = FALSE;
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
  return lIteratorExists;
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotRecordIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecIndexType, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotRecordIndex(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  Dem_Cfg_SRecIndexType lIndex;
  if (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    lIndex = SnapshotEntryIter->SnapshotIteratorIndex;
  }
  else
  {
    lIndex = 0u;
  }
  return lIndex;
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotEntryIndex(                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  uint8 lIndex;
  if (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    lIndex = SnapshotEntryIter->SnapshotEntryIndex;
  }
  else
  {
    lIndex = 0u;
  }
  return lIndex;
}
#endif 

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
% Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex(                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  return SnapshotEntryIter->SnapshotFifoEntryIndex;
}
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotRecordDataConstPtr
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_ConstSharedDataPtrType, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotRecordDataConstPtr(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  if (SnapshotEntryIter->EventEntryIndex == DEM_CFG_ENTRYINDEX_INVALID)
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
  return Dem_MemoryEntry_EventEntry_GetSnapshotRecordDataConstPtr(SnapshotEntryIter->EventEntryIndex, SnapshotEntryIter->SnapshotEntryIndex);
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorIsSnapshotRecordStored
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
Dem_MemoryEntry_SnapshotEntry_IteratorIsSnapshotRecordStored(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  boolean lSnapshotRecordIsStored;

  lSnapshotRecordIsStored = FALSE;

  if (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    if (Dem_Event_IsCalculatedFifoSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
    {
      if (Dem_MemoryEntry_EventEntry_GetSnapshotHeader(SnapshotEntryIter->EventEntryIndex) >
          Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex(SnapshotEntryIter))                                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        lSnapshotRecordIsStored = TRUE;
      }
    }
    else if (Dem_Event_IsCalculatedSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
    {
      if (Dem_MemoryEntry_EventEntry_GetSnapshotHeader(SnapshotEntryIter->EventEntryIndex) > SnapshotEntryIter->SnapshotEntryIndex)
      {
        lSnapshotRecordIsStored = TRUE;
      }
    }
    else /* Uses configured records */
    {
      uint8 lMask;

      lMask = (uint8)1 << SnapshotEntryIter->SnapshotEntryIndex;
      if ((Dem_MemoryEntry_EventEntry_GetSnapshotHeader(SnapshotEntryIter->EventEntryIndex) & lMask) != 0u)
      {
        lSnapshotRecordIsStored = TRUE;
      }
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }

  return lSnapshotRecordIsStored;
}
#endif /* DEM_CFG_SUPPORT_SRECS == STD_ON */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorTestSnapshotRecordFifoStorageRequired
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorTestSnapshotRecordFifoStorageRequired(                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  boolean lNeedToStoreSnapshotRecord;
  uint8 lNextRecordPos;

  lNeedToStoreSnapshotRecord = FALSE;
  lNextRecordPos = Dem_MemoryEntry_EventEntry_GetSnapshotHeaderFifo(SnapshotEntryIter->EventEntryIndex);

  if (lNextRecordPos == SnapshotEntryIter->SnapshotEntryIndex)
  {
    lNeedToStoreSnapshotRecord = TRUE;
  }

  return lNeedToStoreSnapshotRecord;
}
#endif /* DEM_CFG_SUPPORT_SRECS == STD_ON */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorUpdateSnapshotEntry
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorUpdateSnapshotEntry(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  if (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    if (Dem_Event_IsCalculatedSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
    {
      if (Dem_MemoryEntry_EventEntry_GetSnapshotHeader(SnapshotEntryIter->EventEntryIndex) == SnapshotEntryIter->SnapshotEntryIndex)
      { /* Only increment if a new snapshot is added, updates keep the current ID */
        Dem_MemoryEntry_EventEntry_SetSnapshotHeader(SnapshotEntryIter->EventEntryIndex,
          (uint8)(Dem_MemoryEntry_EventEntry_GetSnapshotHeader(SnapshotEntryIter->EventEntryIndex) + 1u));
      }

      /* For calculated fifo, also update the snapshot header fifo with wrapping */
      if (Dem_Event_IsCalculatedFifoSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
      {
        uint8 lSnapshotHeaderFifo;
        lSnapshotHeaderFifo = (Dem_MemoryEntry_EventEntry_GetSnapshotHeaderFifo(SnapshotEntryIter->EventEntryIndex) + 1u) %
          Dem_Cfg_SRecNumConfigured(SnapshotEntryIter->EventId);

        Dem_MemoryEntry_EventEntry_SetSnapshotHeaderFifo(SnapshotEntryIter->EventEntryIndex, lSnapshotHeaderFifo);
      }
    }
    else /* Uses configured records */
    {
      uint8 lMask;

      lMask = (uint8)1 << SnapshotEntryIter->SnapshotEntryIndex;
      Dem_MemoryEntry_EventEntry_SetSnapshotHeader(
        SnapshotEntryIter->EventEntryIndex, (uint8)(Dem_MemoryEntry_EventEntry_GetSnapshotHeader(SnapshotEntryIter->EventEntryIndex) | lMask));
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

# if ((DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)\
      && (DEM_CFG_SUPPORT_SRECS == STD_ON))
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_SetSRecStorageInDTCHeader
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_SetSRecStorageInDTCHeader(
  CONST(uint8, AUTOMATIC)  SRecId,
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  CombinedDTCGroup,    
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  if (EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    uint32 lMask;
    uint8 lMaskIndex;
    Dem_Cfg_SRecNumerationType lSRecNumerationType;
    Dem_EventIdType lEventId;

    lMask = 0x00U;

    lEventId = Dem_MemoryEntry_EventEntry_GetEventId(EventEntryIndex);
    lSRecNumerationType = Dem_FaultMemory_Memories_GetSRecNumerationType(Dem_Core_Event_GetDestination(lEventId));
    lMaskIndex = Dem_MemoryEntry_SnapshotEntry_FindSRecIndexInCombinedDTCSRecList(SRecId, lSRecNumerationType, CombinedDTCGroup);

    if (lMaskIndex < DEM_SNAPSHOTENTRY_CONFIGURED_SREC_INDEX_INVALID)
    {
      lMask = (uint32)1u << lMaskIndex;
      Dem_MemoryEntry_EventEntry_SetCombinedDTCSnapshotHeader(EventEntryIndex,
        (Dem_MemoryEntry_EventEntry_GetCombinedDTCSnapshotHeader(EventEntryIndex) | lMask));
    }
    else
    {
      /* Stored an SRec which was not present in the DTC SRec List */
      Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
    }
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}
#endif 

#if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_GetSRecStorageStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_GetSRecStorageStatus(
  CONST(uint8, AUTOMATIC)  SRecId,
  CONST(uint8, AUTOMATIC)  SnapshotHeaderBitMaskIndex,
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  boolean lSnapshotRecordIsStored;
  Dem_EventIdType lEventId;

  lSnapshotRecordIsStored = FALSE;
  lEventId = Dem_MemoryEntry_EventEntry_GetEventId(EventEntryIndex);

  if (Dem_Event_IsCalculatedSRecsUsed(lEventId) == TRUE)
  {
    if (Dem_MemoryEntry_EventEntry_GetSnapshotHeader(EventEntryIndex) >= SRecId)
    {
      lSnapshotRecordIsStored = TRUE;
    }
  }
  else /* Uses configured records */
  {
    uint32 lMask;

    lMask = 0x00U;
    if (SnapshotHeaderBitMaskIndex < DEM_SNAPSHOTENTRY_CONFIGURED_SREC_INDEX_INVALID)
    {
      lMask = (uint32)1u << SnapshotHeaderBitMaskIndex;
      if ((Dem_MemoryEntry_EventEntry_GetCombinedDTCSnapshotHeader(EventEntryIndex) & lMask) != 0u)
      {
        lSnapshotRecordIsStored = TRUE;
      }
    }
    else
    {
      Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
    }
  }

  return lSnapshotRecordIsStored;
}
#endif 

# if ((DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON))
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_FindSRecIndexInCombinedDTCSRecList
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
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_FindSRecIndexInCombinedDTCSRecList(
  CONST(uint8, AUTOMATIC)  SRecId,
  CONST(Dem_Cfg_SRecNumerationType, AUTOMATIC)  SRecNumerationType,
  CONST(Dem_Cfg_CombinedGroupIndexType, AUTOMATIC)  CombinedDTCGroup
  )
{
  uint8 lMaskIndex;
  boolean lRecordFound;
  Dem_Cfg_DTCFreezeFrameIterType lDTCFreezeFrameIter;

  lRecordFound = FALSE;
  lMaskIndex = 0x00U;

  if (Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculated(SRecNumerationType) == TRUE)
  {
    /* If the DTC at least has one freeze frame configured */
    Dem_Cfg_DTCFreezeFrameRecordIterInit(&lDTCFreezeFrameIter, CombinedDTCGroup);                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
    if (Dem_Cfg_DTCFreezeFrameRecordIterExists(&lDTCFreezeFrameIter) == TRUE)                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      /* Initialize to end of list. If the last SRec Id is greater than requested SRec Id,
         the SRec exists in list since the record numbers are sequential */
      if (Dem_Cfg_GetMaxFreezeFrameIdCombinedEvent(CombinedDTCGroup) >= SRecId)
      {
        lRecordFound = TRUE; 
      }
    }
  }
  else /* Uses configured records */
  {
    for (Dem_Cfg_DTCFreezeFrameRecordIterInit(&lDTCFreezeFrameIter, CombinedDTCGroup);                                           /* PRQA S 0771 */ /* MD_DEM_15.4_opt */  /* SBSW_DEM_CALL_ITERATOR_POINTER */
         (Dem_Cfg_DTCFreezeFrameRecordIterExists(&lDTCFreezeFrameIter) == TRUE);                                                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
         Dem_Cfg_DTCFreezeFrameRecordIterNext(&lDTCFreezeFrameIter))                                                             /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      if (Dem_Cfg_DTCFreezeFrameRecordIterGet(&lDTCFreezeFrameIter) == SRecId)                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        lRecordFound = TRUE; 
        break;
      }

      if (Dem_Cfg_DTCFreezeFrameRecordIterGet(&lDTCFreezeFrameIter) > SRecId)                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
      { /* In an ordered list, no point to search for entries greater the SRec Id. 
           This means the combined DTC does not have the freeze frame configured */
        break;
      }

      lMaskIndex++;
    }
  }

  /* Return invalid index if record is not found */
  if (lRecordFound == FALSE)
  {
    lMaskIndex = DEM_SNAPSHOTENTRY_CONFIGURED_SREC_INDEX_INVALID;
  }

  return lMaskIndex;
}
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorCopySnapshotEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorCopySnapshotEntry(
  CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter,
  uint8 SnapshotRecordIndex
  )
{
  if ( (SnapshotEntryIter->EventEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
    && (SnapshotRecordIndex < Dem_Cfg_GlobalSRecMaxCount()) )
  {
    Dem_MemCpy((Dem_MemoryEntry_EventEntry_GetSnapshotRecordDataPtr(SnapshotEntryIter->EventEntryIndex, SnapshotEntryIter->SnapshotEntryIndex)), /* PRQA S 0315, 0602 */ /* MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_SNAPSHOT_BUFFER */
               (Dem_MemoryEntry_EventEntry_GetSnapshotRecordDataPtr(SnapshotEntryIter->EventEntryIndex, SnapshotRecordIndex)),
                Dem_Cfg_EventSRecMaxRawSize());
  }
  else
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorFindLastStoredEntry
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorFindLastStoredEntry(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  for (Dem_MemoryEntry_SnapshotEntry_IteratorInit(EventId, EventEntryIndex, SnapshotEntryIter);                                  /* PRQA S 2467, 2476, 2477 */ /* MD_DEM_14.2_LoopControl, MD_DEM_14.2_LoopInitialisation, MD_DEM_14.2_LoopControl */  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
       Dem_MemoryEntry_SnapshotEntry_IteratorExists(SnapshotEntryIter) == TRUE;                                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
       Dem_MemoryEntry_SnapshotEntry_IteratorNext(SnapshotEntryIter))                                                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    if ( (Dem_MemoryEntry_EventEntry_GetSnapshotHeader(EventEntryIndex) > 0u)
      && (Dem_MemoryEntry_EventEntry_GetSnapshotHeader(EventEntryIndex) <= Dem_Cfg_GlobalSRecMaxCount()) )                       /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    {
      boolean lFound = FALSE;

      if (Dem_Event_IsCalculatedFifoSRecsUsed(EventId) == TRUE)
      {
        /* Calculated fifo snapshot records */
        if ((Dem_MemoryEntry_EventEntry_GetSnapshotHeader(EventEntryIndex) - 1u) ==
            Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex(SnapshotEntryIter))                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        {
          lFound = TRUE;
        }
      }
      else
      {
        /* Calculated snapshot records */
        if ((Dem_MemoryEntry_EventEntry_GetSnapshotHeader(EventEntryIndex) - 1u) == SnapshotEntryIter->SnapshotEntryIndex)
        {
          lFound = TRUE;
        }
      }

      if (lFound == TRUE)
      {
        break;
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorFindEntryWithRecordNumber
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorFindEntryWithRecordNumber(
  Dem_EventIdType EventId,
  uint8 RecordNumber,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
  )
{
  for (Dem_MemoryEntry_SnapshotEntry_IteratorInit(EventId, EventEntryIndex, SnapshotEntryIter);                                  /* PRQA S 2467, 2476, 2477 */ /* MD_DEM_14.2_LoopControl, MD_DEM_14.2_LoopInitialisation, MD_DEM_14.2_LoopControl */  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
       Dem_MemoryEntry_SnapshotEntry_IteratorExists(SnapshotEntryIter) == TRUE;                                                  /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
       Dem_MemoryEntry_SnapshotEntry_IteratorNext(SnapshotEntryIter))                                                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
    Dem_Cfg_SRecIndexType lCfgSRecIndex;

    lCfgSRecIndex = Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotRecordIndex(SnapshotEntryIter);                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

    if (RecordNumber == Dem_Cfg_SRecId(lCfgSRecIndex))
    {
      if (Dem_MemoryEntry_SnapshotEntry_IteratorIsSnapshotRecordStored(SnapshotEntryIter) == TRUE)                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        break;
      }
    }
  }
}
#endif /* (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_SearchStoredEntry
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_SearchStoredEntry(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex,
  CONSTP2VAR(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter                                            /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
  )
{
  boolean lFoundEntry;
  lFoundEntry = FALSE;

  if(Dem_MemoryEntry_SnapshotEntry_IteratorIsInvalidated(SnapshotEntryIter) == TRUE)                                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
  {
#  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
    Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorInit(EventId, EventEntryIndex, SnapshotEntryIter);               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
#  else
    Dem_MemoryEntry_SnapshotEntry_IteratorInit(EventId, EventEntryIndex, SnapshotEntryIter);                                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
#  endif
  }

  if (Dem_Event_IsConfiguredSRecsUsed(EventId) == TRUE)
  {
    while ((Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorExists(SnapshotEntryIter) == TRUE) && (lFoundEntry == FALSE)) /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    {
  #  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
      if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)
      {
        if (Dem_MemoryEntry_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored(SnapshotEntryIter) == TRUE)                     /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        {
          lFoundEntry = TRUE;
        }
        else
        {
          Dem_MemoryEntry_SnapshotEntry_CombinationOnRetrival_IteratorNext(SnapshotEntryIter);                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
      }
      else
  #  endif
      {
        if (Dem_MemoryEntry_SnapshotEntry_IteratorIsSnapshotRecordStored(SnapshotEntryIter) == TRUE)                             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        {
          lFoundEntry = TRUE;
        }
        else
        {
          Dem_MemoryEntry_SnapshotEntry_IteratorNext(SnapshotEntryIter);                                                         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        }
      }
    }
  }
  else /* Uses calculated snapshot records */
  {
#  if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
    if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      if (Dem_MemoryEntry_SnapshotEntry_IteratorIsCombinedSnapshotRecordStored(SnapshotEntryIter) == TRUE)                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        lFoundEntry = TRUE;
      }
    }
    else
#  endif
    {
      if (Dem_MemoryEntry_SnapshotEntry_IteratorIsSnapshotRecordStored(SnapshotEntryIter) == TRUE)                               /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        lFoundEntry = TRUE;
      }
    }
  }

  return lFoundEntry;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) */


#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_IteratorGetStoredRecordNumber
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_IteratorGetStoredRecordNumber(
CONSTP2CONST(Dem_Cfg_SnapshotEntry_IterType, AUTOMATIC, AUTOMATIC)  SnapshotEntryIter
)
{
  uint8 lRecordNumber; 

  if (Dem_Event_IsCalculatedSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
  {
    if (Dem_Event_IsCalculatedFifoSRecsUsed(SnapshotEntryIter->EventId) == TRUE)
    {
      lRecordNumber = (uint8)(Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotFifoEntryIndex(SnapshotEntryIter) + 1u);          /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      lRecordNumber = (uint8)(Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotEntryIndex(SnapshotEntryIter) + 1u);              /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
  else /* Uses configured records */
  {
# if (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
    if (SnapshotEntryIter->CombinedDTCIndex != DEM_CFG_COMBINED_GROUP_INVALID)
    {
      lRecordNumber = Dem_Cfg_DTCFreezeFrameRecordIterGet(&(SnapshotEntryIter->CombinedDTCSnapShotIterator));                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
# endif
    {
      lRecordNumber = Dem_Cfg_SRecId(Dem_MemoryEntry_SnapshotEntry_IteratorGetSnapshotRecordIndex(SnapshotEntryIter));           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }

  return lRecordNumber;
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_GetNumberOfStoredSnapshotRecords
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_GetNumberOfStoredSnapshotRecords(
  Dem_Cfg_EntryIndexType EventEntryIndex
  )
{
  uint8 lNumberOfRecords;

  if (Dem_Event_IsCalculatedSRecsUsed(Dem_MemoryEntry_EventEntry_GetEventId(EventEntryIndex)) == TRUE)
  {
    lNumberOfRecords = Dem_MemoryEntry_EventEntry_GetSnapshotHeader(EventEntryIndex);
  }
  else
  {
    uint8 lSnapshotHeader;
    lNumberOfRecords = 0u;
    lSnapshotHeader = Dem_MemoryEntry_EventEntry_GetSnapshotHeader(EventEntryIndex);
    while (lSnapshotHeader != 0u)
    {
      lNumberOfRecords = (uint8)(lNumberOfRecords + (lSnapshotHeader & 0x01U));
      lSnapshotHeader = (uint8)(lSnapshotHeader >> 0x01U);
    }
  }

  return lNumberOfRecords;
}
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) */

#if (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON)
/* ****************************************************************************
 % Dem_MemoryEntry_SnapshotEntry_GetNumberOfStoredCombinedSnapshotRecords
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
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_SnapshotEntry_GetNumberOfStoredCombinedSnapshotRecords(
  Dem_Cfg_EntryIterType EventEntryIter,
  Dem_Cfg_CombinedGroupIndexType GroupId
)
{
  
  Dem_EventIdType lEventId;
  uint8 lCrntSnapshotHeader;
  uint8 lNumberOfRecords = 0u;
  uint32 lCombinedSnapshotHeader = 0u;
  Dem_Cfg_SRecNumerationType lSrecNumerationType = Dem_FaultMemory_Memories_GetSRecNumerationType(DEM_CFG_MEMORYID_PRIMARY);
  Dem_Cfg_EntryIterType lEventEntryIter;

  for (lEventEntryIter = EventEntryIter;
    Dem_Cfg_ComplexIter32BitExists(&lEventEntryIter) == TRUE;                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIter32BitNext(&lEventEntryIter))                                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {

    Dem_Cfg_EntryIndexType lEventEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lEventEntryIter);                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
    lEventId = Dem_MemoryEntry_EventEntry_GetEventId(lEventEntryIndex);
   
    if ((Dem_Event_TestValidHandle(lEventId) == TRUE)
      && (Dem_Cfg_EventCombinedGroup(lEventId) == GroupId))                                                                      
    {
      if (Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculated(lSrecNumerationType))
      {
        lCrntSnapshotHeader = Dem_MemoryEntry_EventEntry_GetSnapshotHeader(lEventEntryIndex);
        if (lNumberOfRecords < lCrntSnapshotHeader)
        {
          lNumberOfRecords = lCrntSnapshotHeader;
        }
      }
      else /* Uses configured records */
      {
        lCombinedSnapshotHeader |= Dem_MemoryEntry_EventEntry_GetCombinedDTCSnapshotHeader(lEventEntryIndex);
      }
    }
  }

  /* Count configured records */
  while (lCombinedSnapshotHeader != 0u)                                                                                          /* PRQA S 2994 */ /* MD_DEM_2994 */
  {
    lNumberOfRecords = (uint8)(lNumberOfRecords + (lCombinedSnapshotHeader & 0x01U));
    lCombinedSnapshotHeader = (uint32)(lCombinedSnapshotHeader >> 0x01U);
  }

  return lNumberOfRecords;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif /* (DEM_CFG_SUPPORT_DCM == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON) && (DEM_CFG_SUPPORT_TYPE2_COMBINATION == STD_ON) */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_SNAPSHOTENTRY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_SnapshotEntry_Implementation.h
 *********************************************************************************************************************/
