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
/*! \addtogroup Dem_CustomTriggerMemory
 *  \{
 *  \file       Dem_CustomTriggerMemory_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of the Custom Trigger Memory subcomponent which manages the dedicated memory for custom triggered
 *              snapshot data and time series snapshot data.
 *  \unit     CustomTriggerMemory
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_CUSTOMTRIGGERMEMORY_IMPLEMENTATION_H)
#define DEM_CUSTOMTRIGGERMEMORY_IMPLEMENTATION_H

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
#include "Dem_CustomTriggerMemory_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_Satellite_Interface.h"
                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_CUSTOMTRIGGERMEMORY_IMPLEMENTATION_FILENAME "Dem_CustomTriggerMemory_Implementation.h"

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
 * \defgroup Dem_CustomTriggerMemory_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_GetMemoryId
 *****************************************************************************/
/*!
 * \brief         Get the translated memory id of the custom trigger destination
 *                for the event
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The translated memory id of the event's custom trigger destination.
 *                DEM_CFG_MEMORYID_INVALID if no custom trigger destination is supported.
 *
 * \pre           -
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Memories_CustomTriggerMemory_GetMemoryId(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_EntryIterInit
 *****************************************************************************/
/*!
 * \brief         Initializes for iteration over all entries of an custom trigger
 *                memory
 *
 * \details       -
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 * \param[in]     CustomTriggerTranslatedMemoryId
 *                Translated memory Id of the custom trigger memory
 *
 * \pre           The translated memory Id must belong to a custom trigger memory
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_EntryIterInit(
  CONSTP2VAR(Dem_Cfg_ComplexIterType32Bit, AUTOMATIC, AUTOMATIC) IterPtr,
  uint16 CustomTriggerTranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_GetSRecNumIndex
 *****************************************************************************/
/*!
 * \brief         Get the index in the Freeze Frame Number table for the event's
 *                custom triggered snapshot record
 *
 * \details       An event may support upto 1 custom triggered snapshot record. This
 *                getter returns the index.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The index in the Freeze Frame Number table for the event's
 *                custom triggered snapshot record or the end index of the Freeze
 *                Frame Number table if the event has no custom triggered snapshot
 *                record.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecIndexType, DEM_CODE)
Dem_Memories_CustomTriggerMemory_GetSRecNumIndex(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_RecordMaxRawSizeGet
 *****************************************************************************/
/*!
 * \brief         Get the maximum raw size of a custom triggered snapshot record
 *
 * \details       Get the maximum size available for a single custom triggered
 *                snapshot record. This only counts the data that needs to
 *                be stored in the user-data area of a custom trigger entry.
 *
 * \return        The maximum raw size of a custom triggered snapshot record
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_CustomTriggerMemory_RecordMaxRawSizeGet(
  void
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_TestEventSupportsUpdate
 *****************************************************************************/
/*!
 * \brief         Test whether the event's custom trigger entry is
 *                update-able.
 *
 * \details       Test whether the event is stored in a custom trigger memory
 *                entry and the custom triggered data is update-able.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event is stored in a custom trigger memory entry and
 *                the custom triggered data is update-able.
 * \return        FALSE
 *                The event is not stored in a custom trigger memory entry or
 *                the custom triggered data is not update-able.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestEventSupportsUpdate(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_TestSRecUpdatable
 *****************************************************************************/
/*!
 * \brief         Test whether the event's custom triggered snapshot record is
 *                update-able.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event's custom triggered snapshot record is update-able.
 * \return        FALSE
 *                The event's custom triggered snapshot record is not update-able
 *                or the event has no custom triggered snapshot record
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestSRecUpdatable(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_TestTssrUpdatable
 *****************************************************************************/
/*!
 * \brief         Test whether the event's custom triggered TSS record
 *                is update-able.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event's custom triggered TSS record is update-able.
 * \return        FALSE
 *                The event's custom triggered TSS record is not update-able
 *                or the event has no custom triggered TSS record
 *
 * \pre           -
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestTssrUpdatable(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_GetSamplingProfileId
 *****************************************************************************/
/*!
* \brief          Returns the sampling profile index for custom triggered time series
*                 snapshots for the event
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Sampling Profile Id
 *                sampling profile index for custom triggered time series for event
 *                in Dem_Cfg_TimeSeriesSamplingProfileTable
 *                Dem_Cfg_GetSizeOfTimeSeriesSamplingProfileTable() is returned
 *                if event does not support custom triggered time series
 *
 * \pre           Event must support custom triggered time series
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_GetSamplingProfileId(
  Dem_EventIdType  EventId
  );

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/


/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_GetMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Memories_CustomTriggerMemory_GetMemoryId(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  uint16 lCustomTriggerTranslatedMemoryId = DEM_CFG_MEMORYID_INVALID;

  uint16 lEventDestinationTranslatedMemoryId = Dem_Core_Event_GetDestination(EventId);
  if (lEventDestinationTranslatedMemoryId != DEM_CFG_MEMORYID_INVALID)                                                           /* COV_DEM_ROBUSTNESS TX */
  {
    lCustomTriggerTranslatedMemoryId = Dem_Memories_EventMemory_GetReferencedCustomTriggerMemoryId(lEventDestinationTranslatedMemoryId);
  }

  return lCustomTriggerTranslatedMemoryId;
}


/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_EntryIterInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_EntryIterInit(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONSTP2VAR(Dem_Cfg_ComplexIterType32Bit, AUTOMATIC, AUTOMATIC) IterPtr,
  uint16 CustomTriggerTranslatedMemoryId
  )
{
  Dem_Cfg_ComplexIter32BitInit(IterPtr,                                                                                          /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    Dem_FaultMemory_Memories_GetFirstEntryIndex(CustomTriggerTranslatedMemoryId),
    Dem_FaultMemory_Memories_GetEndEntryIndex(CustomTriggerTranslatedMemoryId));
}


/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_GetSRecNumIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_SRecIndexType, DEM_CODE)
Dem_Memories_CustomTriggerMemory_GetSRecNumIndex(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
  )
{
  Dem_Cfg_SRecIndexType lSRecIndex = Dem_Cfg_SRecNumEndIdx(EventId);
  Dem_Cfg_ComplexIterType lSRecIter;

  for (Dem_Cfg_ComplexIterInit(&lSRecIter, Dem_Cfg_SRecNumStartIdx(EventId), Dem_Cfg_SRecNumEndIdx(EventId));                    /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */ /* SBSW_DEM_CALL_ITERATOR_POINTER */ /* COV_DEM_ROBUSTNESS TX */
    (Dem_Cfg_ComplexIterExists(&lSRecIter) == TRUE);                                                                             /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_ComplexIterNext(&lSRecIter))                                                                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    if (Dem_Cfg_SRecTrigger(Dem_Cfg_ComplexIterGet(&lSRecIter)) == DEM_CFG_TRIGGER_CUSTOM)                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      lSRecIndex = Dem_Cfg_ComplexIterGet(&lSRecIter);                                                                           /* SBSW_DEM_CALL_ITERATOR_POINTER */
      break;
    }
  }
  return lSRecIndex;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_RecordMaxRawSizeGet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_CustomTriggerMemory_RecordMaxRawSizeGet(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return DEM_CFG_MAX_SIZE_CUSTOM_TRIGGERED_RECORD;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_TestEventSupportsUpdate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestEventSupportsUpdate(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return (  Dem_Memories_CustomTriggerMemory_TestTssrUpdatable(EventId) 
         || Dem_Memories_CustomTriggerMemory_TestSRecUpdatable(EventId));
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_TestSRecUpdatable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestSRecUpdatable(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
  )
{
  boolean lUpdate = FALSE;

#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_SREC == STD_ON)
  Dem_Cfg_SRecIndexType  lSRecIndex = Dem_Memories_CustomTriggerMemory_GetSRecNumIndex(EventId);
  if (lSRecIndex != Dem_Cfg_SRecNumEndIdx(EventId))                                                                              /* COV_DEM_ROBUSTNESS TX */
  {
    lUpdate = Dem_Cfg_SRecUpdate(lSRecIndex);
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

  return lUpdate;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_TestTssrUpdatable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestTssrUpdatable(
  Dem_EventIdType  EventId                                                                                                       /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  boolean lUpdate = FALSE;

#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
  if (Dem_Core_TestCustomTriggeredTssrConfigured(EventId) == TRUE)
  {
    lUpdate = Dem_Cfg_IsUpdateRecordOfCustomTimeSeriesFreezeFrameTable(Dem_Cfg_GetCustomTimeSeriesFreezeFrameTableIdxOfEventTable(EventId));
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return lUpdate;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_GetSamplingProfileId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_GetSamplingProfileId(                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
  )
{
  uint8 lSamplingProfileId = 0u;
#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
  if (Dem_Core_TestCustomTriggeredTssrConfigured(EventId) == TRUE)                                                               /* COV_DEM_ROBUSTNESS TX */
  {
    lSamplingProfileId = Dem_Cfg_GetTimeSeriesSamplingProfileTableIdxOfCustomTimeSeriesFreezeFrameTable(
           Dem_Cfg_GetCustomTimeSeriesFreezeFrameTableIdxOfEventTable(EventId));
  }
#else
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
#endif
  DEM_IGNORE_UNUSED_VARIABLE(EventId);                                                                                           /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return lSamplingProfileId;
}
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
 * \addtogroup Dem_CustomTriggerMemory_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_IsReferredByMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_IsReferredByMemoryId(                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16 TranslatedMemoryId
  )
{
  boolean lIsCustomTriggerMemory = FALSE;

  if ((TranslatedMemoryId != DEM_CFG_MEMORYID_INVALID)                                                                           /* COV_DEM_ROBUSTNESS TF tx tf */
    && (Dem_Memories_CustomTriggerMemory_IsEnabled() == TRUE))
  {
    uint16 lEventMemoryId;

    for (lEventMemoryId = 0;
      lEventMemoryId <= Dem_Memories_EventMemory_UserDefined_GetNumberOfUserDefinedMemories();
      lEventMemoryId++)
    {
      uint16 lCustomTriggerMemoryId = Dem_Memories_EventMemory_GetReferencedCustomTriggerMemoryId(lEventMemoryId);
      if (lCustomTriggerMemoryId == TranslatedMemoryId)
      {
        lIsCustomTriggerMemory = TRUE;
        break;
      }
    }
  }
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TranslatedMemoryId)                                                                           /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

  return lIsCustomTriggerMemory;
}


/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_TestRecordNumberIsValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestRecordNumberIsValid(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId,
  uint8  RecordNumber
  )
{
  return (boolean)(  (Dem_Memories_CustomTriggerMemory_TestRecordNumberIsSRec(EventId, RecordNumber) == TRUE)
                  || (Dem_Memories_CustomTriggerMemory_TestRecordNumberIsTSSRec(EventId,RecordNumber) == TRUE));
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_TestRecordNumberIsSRec
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestRecordNumberIsSRec(                                                                         /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId,
  uint8  RecordNumber
  )
{
  boolean lIsCustomTriggeredSRec = FALSE;

  if ( (Dem_Core_TestCustomTriggeredSrecConfigured(EventId) == TRUE)
    && (Dem_Memories_CustomTriggerMemory_GetSRecNumber(EventId) == RecordNumber))
  {
    lIsCustomTriggeredSRec = TRUE;
  }

  return lIsCustomTriggeredSRec;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_GetSRecNumber
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_GetSRecNumber(
  Dem_EventIdType EventId
  )
{
  uint8 lSRecNumber = DEM_CUSTOMTRIGGERMEMORY_SREC_NUMBER_INVALID;

#if (DEM_CFG_SUPPORT_SRECS == STD_ON)
  Dem_Cfg_SRecIndexType lSRecIndex = Dem_Memories_CustomTriggerMemory_GetSRecNumIndex(EventId);
  if (lSRecIndex != Dem_Cfg_SRecNumEndIdx(EventId))                                                                              /* COV_DEM_ROBUSTNESS TX */
  {
    lSRecNumber = Dem_Cfg_SRecId(lSRecIndex);
  }
#else
  Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
  
  return lSRecNumber;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_TestRecordNumberIsTSSRec
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestRecordNumberIsTSSRec(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId,
  uint8  RecordNumber
  )
{
  boolean lIsCustomTriggeredSRec = FALSE;
#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
  if (  (Dem_Core_TestCustomTriggeredTssrConfigured(EventId) == TRUE)
     && (RecordNumber >= Dem_Memories_CustomTriggerMemory_FirstTSSRecNum(EventId))
     && (RecordNumber <= Dem_Memories_CustomTriggerMemory_LastTSSRecNum(EventId)))
  {
    lIsCustomTriggeredSRec = TRUE;
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(RecordNumber)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

  return lIsCustomTriggeredSRec;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_GetTimeSeriesSampleIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_GetTimeSeriesSampleIndex(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId,
  uint8  RecordNumber
  )
{
  uint8 lSampleIndex = 0u;
#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
  if (Dem_Memories_CustomTriggerMemory_TestRecordNumberIsTSSRec(EventId, RecordNumber) == TRUE)                                  /* COV_DEM_ROBUSTNESS TX */
  {
    lSampleIndex = RecordNumber - Dem_Memories_CustomTriggerMemory_FirstTSSRecNum(EventId);
  }
  else
#endif
  {
    Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
  }
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(RecordNumber)                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

  return lSampleIndex;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_FirstTSSRecNum
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_FirstTSSRecNum(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  uint8 lRecordNum = DEM_CUSTOMTRIGGERMEMORY_SREC_NUMBER_INVALID;

#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
  lRecordNum = Dem_Cfg_GetFirstRecordNumberOfCustomTimeSeriesFreezeFrameTable(Dem_Cfg_GetCustomTimeSeriesFreezeFrameTableIdxOfEventTable(EventId));
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return lRecordNum;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_LastTSSRecNum
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_LastTSSRecNum(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  uint8 lRecordNum = DEM_CUSTOMTRIGGERMEMORY_SREC_NUMBER_INVALID;

#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
  lRecordNum = Dem_Cfg_GetLastRecordNumberOfCustomTimeSeriesFreezeFrameTable(Dem_Cfg_GetCustomTimeSeriesFreezeFrameTableIdxOfEventTable(EventId));
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return lRecordNum;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_NumPastTssSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_NumPastTssSamples(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  uint8 lNumPastTssSamples = 0u;

#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
  if (Dem_Core_TestCustomTriggeredTssrConfigured(EventId) == TRUE)                                                               /* COV_DEM_ROBUSTNESS TX */
  {
    uint8 lSamplingProfileId = Dem_Memories_CustomTriggerMemory_GetSamplingProfileId(EventId);
    lNumPastTssSamples = Dem_Cfg_GetPastSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId);                             /* PRQA S 2842 */ /* MD_DEM_Dir4.1_ReadOperation */
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(lSamplingProfileId)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return lNumPastTssSamples;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_NumFutureTssSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_NumFutureTssSamples(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType  EventId
  )
{
  uint8 lNumFutureTssSamples = 0u;

#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
  if (Dem_Core_TestCustomTriggeredTssrConfigured(EventId) == TRUE)                                                               /* COV_DEM_ROBUSTNESS TX */
  {
    uint8 lSamplingProfileId = Dem_Memories_CustomTriggerMemory_GetSamplingProfileId(EventId);
    lNumFutureTssSamples = Dem_Cfg_GetFutureSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId);                         /* PRQA S 2842 */ /* MD_DEM_Dir4.1_ReadOperation */
    DEM_IGNORE_UNUSED_CONST_ARGUMENT(lSamplingProfileId)                                                                         /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  return lNumFutureTssSamples;
}

 /* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_IsEnabled()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_IsEnabled(
  void
  )
{
  return (boolean)(DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON);
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_MaxTssSamplesGet
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_MaxTssSamplesGet(                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return (uint8) DEM_CFG_MAX_SAMPLES_CUSTOM_TRIGGERED_TIMESERIESSNAPSHOT;
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
 * \defgroup Dem_CustomTriggerMemory_Private Private Methods
 * \{
 */
 
/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_ProcessSatelliteQueueActions
 *****************************************************************************/
/*!
 * \brief         Processes all queued custom trigger actions.
 *
 * \details       This function processes queued custom trigger actions for all
 *                events of the given satellite.
 *
 * \param[in]     SatelliteId
 *                Unique handle of the Satellite.
 *                The SatelliteId must be in range [1..Dem_Cfg_GlobalSatelliteCount()[.
 *
 * \pre           Function must only be invoked when the feature "Custom Snapshot Storage
 *                Trigger" is supported.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_ProcessSatelliteQueueActions(
  Dem_Satellite_IdType  SatelliteId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_StoreEntry
 *****************************************************************************/
/*!
 * \brief         Store a custom trigger entry
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_StoreEntry(
  Dem_EventIdType  EventId
  );


/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_StoreFutureSamples
 *****************************************************************************/
/*!
 * \brief         Store future samples into a custom trigger entry
 *
 * \details       -
 *
 * \param[in]     CustomTriggerEntryIndex
 *                Index of the entry in Dem_Cfg_CustomTriggerEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfCustomTriggerEntryPtr()[.
 *
 * \pre           The custom trigger entry must be allocated to an event that
 *                supports custom triggered time series snapshot
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_StoreFutureSamples(
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  );


/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_AllocateEntry
 *****************************************************************************/
/*!
 * \brief         Allocates a custom trigger entry to an event
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The entry index of the allocated custom trigger entry if one
 *                could be allocated. DEM_CFG_ENTRYINDEX_INVALID is returned if
 *                no entry can be allocated.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Memories_CustomTriggerMemory_AllocateEntry(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_UpdateTimeStampAndChronology
 *****************************************************************************/
/*!
 * \brief         Moves the passed entry up to most current in the chronology list.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     CustomTriggerEntryIndex
 *                Index of the custom trigger entry in Dem_Cfg_CustomTriggerEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfCustomTriggerEntryPtr()[.
 *
 * \pre           The event's custom triggered data must be stored
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_UpdateTimeStampAndChronology(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_SortEntryIntoChronologicalList
 *****************************************************************************/
/*!
 * \brief         Sort the custom trigger entry into the chronological list.
 *
 * \details       -
 *
 * \param[in]     TranslatedMemoryId
 *                Unique Identifier for the custom trigger memory.
 * \param[in]     CustomTriggerEntryIndex
 *                Handle of the event entry to sort into the chronology.
 * \param[in]     Timestamp
 *                Current timestamp of the event entry.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_SortEntryIntoChronologicalList(
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex,
  uint32 Timestamp
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_SelectDisplaceIndex
 *****************************************************************************/
/*!
 * \brief         Selects an entry to displace in a filled custom trigger memory
 *
 * \details       -
 *
 * \param[in]     CustomTriggerTranslatedMemoryId
 *                Translated Memory Identifier of the custom trigger memory.
 * \param[in]     EventId
 *                EventId of the freeze frame that needs to be stored
 *
 * \return        The entry index of the displaced custom trigger entry if one could
 *                be identified. DEM_CFG_ENTRYINDEX_INVALID if displacement
 *                was not possible.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Memories_CustomTriggerMemory_SelectDisplaceIndex(
  uint16 CustomTriggerTranslatedMemoryId,
  Dem_EventIdType DisplacingEventId
  );


/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_ProcessSatelliteQueueActions
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
Dem_Memories_CustomTriggerMemory_ProcessSatelliteQueueActions(
  Dem_Satellite_IdType  SatelliteId
  )
{
  Dem_Satellite_MonitorIdType lMonitorId;

  for (lMonitorId = 0u;
       lMonitorId < Dem_Cfg_GetEventCountOfSatelliteInfo(SatelliteId);
       ++lMonitorId)
  {
    if (Dem_Satellite_PeekCustomTriggerActionQueue(SatelliteId, lMonitorId) != DEM_CUSTOMTRIGGERMEMORY_QUEUE_INITIAL)
    {
      Dem_Memories_CustomTriggerMemory_QueueActionType lQueueStatus;                                                             /* PRQA S 0759 */ /* MD_MSR_Union */
      Dem_EventIdType lEventId;

      lEventId = Dem_Satellite_GetEventId(SatelliteId, lMonitorId);
      lQueueStatus = Dem_Satellite_ConsumeCustomTriggerActionQueue(SatelliteId, lMonitorId);

#if (DEM_DEV_ERROR_REPORT == STD_ON)
      /*Only events configured for custom entry should be processed here*/
      Dem_Internal_AssertContinue((Dem_Core_TestCustomTriggerConfigured(lEventId) == TRUE), DEM_E_INCONSISTENT_STATE)
#endif

      if (lQueueStatus == DEM_CUSTOMTRIGGERMEMORY_QUEUE_STOREFF)
      {
        Dem_Memories_CustomTriggerMemory_StoreEntry(lEventId);
      }
    }
  }
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_StoreEntry
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
Dem_Memories_CustomTriggerMemory_StoreEntry(
  Dem_EventIdType EventId
  )
{
#if (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  Dem_EventIdType lMemoryRepresentativeEventId = Dem_Cfg_GetMemoryRepresentative(EventId);
  Dem_Cfg_EntryIndexType lCustomTriggerEntryIndex = Dem_Memories_CustomTriggerMemory_FindEntryIndex(lMemoryRepresentativeEventId);

  Dem_Event_DataUpdateStart(EventId);

  if (lCustomTriggerEntryIndex == DEM_CFG_ENTRYINDEX_INVALID)
  {
    /* No entry exists - try to create one */
    lCustomTriggerEntryIndex = Dem_Memories_CustomTriggerMemory_AllocateEntry(EventId);
  }
  else /* Entry already exists */
  {
    if (Dem_Memories_CustomTriggerMemory_TestEventSupportsUpdate(EventId) == FALSE)
    {
      /* Update not supported for custom triggered storage */
      lCustomTriggerEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
    }
    else
    {
      /* event storage or update trigger matched */
      Dem_Memories_CustomTriggerMemory_UpdateTimeStampAndChronology(EventId, lCustomTriggerEntryIndex);
    }
  }

  if (lCustomTriggerEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    Dem_Cfg_NvBlockIndexType lNvBlockIndex = Dem_MemoryEntry_CustomTriggerEntry_GetEntryIndexToNvBlockIndex(lCustomTriggerEntryIndex);

# if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_SREC == STD_ON)
    if (Dem_Core_TestCustomTriggeredSrecConfigured(EventId) == TRUE)
    {
      Dem_StorageManager_DataStorage_CollectSnapshot(EventId,                                                                    /* SBSW_DEM_POINTER_CUSTOMTRIGGER_SNAPSHOT_BUFFER */
                                        Dem_MemoryEntry_CustomTriggerEntry_GetSnapshotRecordDataPtr(lCustomTriggerEntryIndex),
                                        Dem_Memories_CustomTriggerMemory_RecordMaxRawSizeGet());
    }
# endif

# if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
    if (Dem_Core_TestCustomTriggeredTssrConfigured(EventId) == TRUE)
    {
      Dem_Data_DestinationBuffer lSnapshotDataBuffer;
      uint8 lSamplingProfileId = Dem_Memories_CustomTriggerMemory_GetSamplingProfileId(EventId);
      uint8 lPastSamples = Dem_Memories_CustomTriggerMemory_NumPastTssSamples(EventId);
      uint8 lNumberStoredSamples = Dem_GlobalDiagnostics_GetRingBufferSampleCount(lSamplingProfileId);
      uint8 lNumberNotStoredSamples = (lPastSamples - lNumberStoredSamples);
      uint8 lSourceSample;

      for (lSourceSample = 0u; lSourceSample < lNumberStoredSamples; ++lSourceSample)
      {
        uint8 lTimeSeriesSampleIndex = lSourceSample + lNumberNotStoredSamples;
        Dem_Data_InitDestinationBuffer(&lSnapshotDataBuffer,                                                                     /* SBSW_DEM_CALL_CUSTOMTRIGGER_TSSNAPSHOTDATA_DSTBUFFER_INIT */
          Dem_MemoryEntry_CustomTriggerEntry_GetTSSnapshotRecordDataPtr(lCustomTriggerEntryIndex, lTimeSeriesSampleIndex),
          Dem_Memories_CustomTriggerMemory_RecordMaxRawSizeGet());

        Dem_Memories_TimeSeriesMemory_CopyDidsFromRingBuffer(
          EventId,
          lSamplingProfileId,
          lSourceSample,
          &lSnapshotDataBuffer                                                                                                   /* SBSW_DEM_POINTER_FORWARD_STACK */
          );
      }

      Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lCustomTriggerEntryIndex)->UnusedFutureSamples =                           /* SBSW_DEM_POINTER_WRITE_CUSTOMTRIGGER_ENTRY */
        Dem_Memories_CustomTriggerMemory_NumFutureTssSamples(EventId);

      Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lCustomTriggerEntryIndex)->UnusedPastSamples =                             /* SBSW_DEM_POINTER_WRITE_CUSTOMTRIGGER_ENTRY */
        lNumberNotStoredSamples;
    }
# endif

    if ((Dem_Cfg_EventSupportImmediateNv(EventId) == TRUE)
      && ((Dem_Core_TestCustomTriggeredTssrConfigured(EventId) == FALSE)
        ||(Dem_Memories_CustomTriggerMemory_NumFutureTssSamples(EventId) == 0u)))
    {
      Dem_Nvm_SetSingleBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
    else
    {
      Dem_Nvm_SetSingleBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }

  Dem_Event_DataUpdateFinish(EventId);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_AllocateEntry
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
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Memories_CustomTriggerMemory_AllocateEntry(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
  )
{
  Dem_Cfg_EntryIndexType lCustomTriggerEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;

#if (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  Dem_EventIdType lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);
  uint16 lCustomTriggerTranslatedMemoryId = Dem_Memories_CustomTriggerMemory_GetMemoryId(EventId);

  if (Dem_FaultMemory_Memories_GetMaxSize(lCustomTriggerTranslatedMemoryId) == Dem_FaultMemory_Memories_GetCurrentSize(lCustomTriggerTranslatedMemoryId))
  { /* Memory is filled up */
    lCustomTriggerEntryIndex = Dem_Memories_CustomTriggerMemory_SelectDisplaceIndex(lCustomTriggerTranslatedMemoryId, EventId);
    if (lCustomTriggerEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
    { /* a displaced DTC was identified, now remove that DTC */
      Dem_EventIdType lDisplacedEventId = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lCustomTriggerEntryIndex)->EventId;
      Dem_Event_DataUpdateStart(lDisplacedEventId);
      Dem_FaultMemory_Memories_RemoveEntryAndChrono(lCustomTriggerTranslatedMemoryId, lCustomTriggerEntryIndex);
      Dem_Event_DataUpdateFinish(lDisplacedEventId);
    }
  }
  else
  { /* Memory is not full -> Find first free entry */
    lCustomTriggerEntryIndex = Dem_FaultMemory_Memories_FindEntryIndexOfEventGeneric(DEM_EVENT_INVALID, FALSE, lCustomTriggerTranslatedMemoryId);
  }

  if (lCustomTriggerEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
  {
    Dem_Cfg_CustomTriggerEntryPtrType lEntry = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lCustomTriggerEntryIndex);
    uint8 lMemorySize = Dem_FaultMemory_Memories_GetCurrentSize(lCustomTriggerTranslatedMemoryId);

    Dem_FaultMemory_Memories_SetChronology(lCustomTriggerTranslatedMemoryId, lMemorySize, lCustomTriggerEntryIndex);

    lEntry->EventId = lMasterEventId;                                                                                            /* SBSW_DEM_POINTER_WRITE_CUSTOMTRIGGER_ENTRY */
    lEntry->Timestamp = Dem_FaultMemory_Memories_GetCurrentTimestamp();                                                          /* SBSW_DEM_POINTER_WRITE_CUSTOMTRIGGER_ENTRY */

    Dem_FaultMemory_Memories_SetCurrentTimestamp(Dem_FaultMemory_Memories_GetCurrentTimestamp() + 1u);
    Dem_FaultMemory_Memories_SetCurrentSize(lCustomTriggerTranslatedMemoryId, (uint8)(lMemorySize + 1u));
  }
#else
   DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                     /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif

  return lCustomTriggerEntryIndex;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_StoreFutureSamples
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
Dem_Memories_CustomTriggerMemory_StoreFutureSamples(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  )
{
#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
  Dem_EventIdType lEventId = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex)->EventId;
  uint8 lUnusedFutureSamples = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex)->UnusedFutureSamples;

  if (lUnusedFutureSamples > 0u)
  {
    Dem_Data_DestinationBuffer lSnapshotDataBuffer;
    uint8 lSamplingProfileId = Dem_Memories_CustomTriggerMemory_GetSamplingProfileId(lEventId);
    uint8 lFutureSamples = Dem_Memories_CustomTriggerMemory_NumFutureTssSamples(lEventId);
    uint8 lPastSamples = Dem_Memories_CustomTriggerMemory_NumPastTssSamples(lEventId);
    uint8 lTimeSeriesSampleIndex = lPastSamples + (lFutureSamples - lUnusedFutureSamples);
    uint8 lSourceSample = Dem_GlobalDiagnostics_GetMostRecentRingBufferSampleId(lSamplingProfileId);
    Dem_Cfg_NvBlockIndexType lNvBlockIndex = Dem_MemoryEntry_CustomTriggerEntry_GetEntryIndexToNvBlockIndex(CustomTriggerEntryIndex);

    Dem_Data_InitDestinationBuffer(&lSnapshotDataBuffer,                                                                         /* SBSW_DEM_CALL_CUSTOMTRIGGER_TSSNAPSHOTDATA_DSTBUFFER_INIT */
      Dem_MemoryEntry_CustomTriggerEntry_GetTSSnapshotRecordDataPtr(CustomTriggerEntryIndex, lTimeSeriesSampleIndex),
      Dem_Memories_CustomTriggerMemory_RecordMaxRawSizeGet());

    Dem_Memories_TimeSeriesMemory_CopyDidsFromRingBuffer(
      lEventId,
      lSamplingProfileId,
      lSourceSample,
      &lSnapshotDataBuffer                                                                                                       /* SBSW_DEM_POINTER_FORWARD_STACK */
      );
    --lUnusedFutureSamples;
    Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex)->UnusedFutureSamples                                /* SBSW_DEM_POINTER_WRITE_CUSTOMTRIGGER_ENTRY */
                                                                    = lUnusedFutureSamples;

    if ((Dem_Cfg_EventSupportImmediateNv(lEventId) == TRUE)
    && (lUnusedFutureSamples == 0u))
    {
      Dem_Nvm_SetSingleBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);
    }
    else
    {
      Dem_Nvm_SetSingleBlockState(lNvBlockIndex, DEM_NVM_BLOCKSTATE_DIRTY);
    }
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CustomTriggerEntryIndex)                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_UpdateTimeStampAndChronology
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_UpdateTimeStampAndChronology(                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex
  )
{
#if (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  /* Store the global timestamp to the entry, and update it */
  Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex)->Timestamp = Dem_FaultMemory_Memories_GetCurrentTimestamp(); /* SBSW_DEM_POINTER_WRITE_CUSTOMTRIGGER_ENTRY */
  Dem_FaultMemory_Memories_SetCurrentTimestamp(Dem_FaultMemory_Memories_GetCurrentTimestamp() + 1u);

  Dem_FaultMemory_Memories_UpdateChrono(Dem_Memories_CustomTriggerMemory_GetMemoryId(EventId), CustomTriggerEntryIndex);
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CustomTriggerEntryIndex)                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
}


/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_SortEntryIntoChronologicalList
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
Dem_Memories_CustomTriggerMemory_SortEntryIntoChronologicalList(                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint16 TranslatedMemoryId,
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex,
  uint32 Timestamp
  )
{
#if (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  uint8 lSortedIndex;
  for (lSortedIndex = Dem_FaultMemory_Memories_GetCurrentSize(TranslatedMemoryId);
        (lSortedIndex > 0u)  && ((Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, (uint8)(lSortedIndex - 1u))) ->Timestamp)  > Timestamp);
       --lSortedIndex
    )
  {
    Dem_FaultMemory_Memories_SetChronology(
      TranslatedMemoryId,
      lSortedIndex,
      Dem_FaultMemory_Memories_GetChronology(TranslatedMemoryId, (uint8)(lSortedIndex - 1u)));
  }
  Dem_FaultMemory_Memories_SetChronology(TranslatedMemoryId, lSortedIndex, CustomTriggerEntryIndex);
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TranslatedMemoryId)                                                                           /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CustomTriggerEntryIndex)                                                                      /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Timestamp)                                                                                    /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

#endif
}


/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_SelectDisplaceIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Memories_CustomTriggerMemory_SelectDisplaceIndex(                                                                            /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint16 CustomTriggerTranslatedMemoryId,
  Dem_EventIdType DisplacingEventId
)
{
  Dem_Cfg_EntryIndexType lDisplaceEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;
#if (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  uint8 lChronoIndexIterator;

  /* Scan in chronological order, so the first found entry is the older one (to break iteration) */
  for (lChronoIndexIterator = 0u;                                                                                                /* PRQA S 0771 */ /* MD_DEM_15.4_opt */
    lChronoIndexIterator < Dem_FaultMemory_Memories_GetCurrentSize(CustomTriggerTranslatedMemoryId);
    ++lChronoIndexIterator)
  {
    Dem_Cfg_EntryIndexType lCustomTriggerEntryIndex = Dem_FaultMemory_Memories_GetChronology(CustomTriggerTranslatedMemoryId, lChronoIndexIterator);
    Dem_EventIdType lEventId = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lCustomTriggerEntryIndex)->EventId;
    if (Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(lEventId, FALSE) == DEM_CFG_ENTRYINDEX_INVALID)
    {
      lDisplaceEntryIndex = lCustomTriggerEntryIndex;
      break;
    }
  }

#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CustomTriggerTranslatedMemoryId)                                                              /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DisplacingEventId)                                                                            /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */

  return lDisplaceEntryIndex;
}
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
 * \addtogroup Dem_CustomTriggerMemory_Public
 * \{
 */


/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_Init(
  void
  )
{
  if(Dem_Memories_CustomTriggerMemory_IsEnabled())
  {
    /* Initialize Custom Trigger Memory */
    Dem_Scheduler_EnableTask(Dem_Scheduler_Task_CustomTrigger);
  }
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_InitNvData
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
Dem_Memories_CustomTriggerMemory_InitNvData(
  void
)
{
#if (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  uint16 lEventMemoryId;

  for (lEventMemoryId = 0;
    lEventMemoryId <= Dem_Memories_EventMemory_UserDefined_GetNumberOfUserDefinedMemories();
    lEventMemoryId++)
  {
    uint16 lCustomTriggerMemoryId = Dem_Memories_EventMemory_GetReferencedCustomTriggerMemoryId(lEventMemoryId);
    if (lCustomTriggerMemoryId != DEM_CFG_MEMORYID_INVALID)
    {
      Dem_Cfg_EntryIterType lCustomTriggerEntryIter;

      Dem_FaultMemory_Memories_SetCurrentSize(lCustomTriggerMemoryId, 0);

      for (Dem_Cfg_ComplexIter32BitInit(&lCustomTriggerEntryIter,                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_FaultMemory_Memories_GetFirstEntryIndex(lCustomTriggerMemoryId),
        Dem_FaultMemory_Memories_GetEndEntryIndex(lCustomTriggerMemoryId));
        Dem_Cfg_ComplexIter32BitExists(&lCustomTriggerEntryIter) == TRUE;                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_ComplexIter32BitNext(&lCustomTriggerEntryIter))                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        Dem_Cfg_EntryIndexType lCustomTriggerEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lCustomTriggerEntryIter);                 /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_CustomTriggerEntryPtrType lEntry = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lCustomTriggerEntryIndex);

        Dem_MemCpy((Dem_SharedDataPtrType)lEntry,                                                                                /* PRQA S 0310, 0311, 0315, 0602 */ /* MD_DEM_11.4_cpy, MD_DEM_11.8, MD_MSR_VStdLibCopy, MD_DEM_20.2 */  /* SBSW_DEM_POINTER_MEMCPY_CUSTOMTRIGGERENTRY */
          (Dem_ConstDataPtrType)(&Dem_Cfg_CustomTriggerEntryInit),
          sizeof(Dem_Cfg_CustomTriggerEntryType));
      }
    }
  }
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_RestoreOnInit
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_RestoreOnInit(
  void
)
{
#if (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  uint16 lEventMemoryId;

  for (lEventMemoryId = 0;
    lEventMemoryId <= Dem_Memories_EventMemory_UserDefined_GetNumberOfUserDefinedMemories();
    lEventMemoryId++)
  {
    uint16 lCustomTriggerMemoryId = Dem_Memories_EventMemory_GetReferencedCustomTriggerMemoryId(lEventMemoryId);

    if (lCustomTriggerMemoryId != DEM_CFG_MEMORYID_INVALID)
    {
      Dem_Cfg_EntryIterType lCustomTriggerEntryIter;
      
      Dem_FaultMemory_Memories_SetCurrentSize(lCustomTriggerMemoryId, 0);
      for (Dem_Memories_CustomTriggerMemory_EntryIterInit(&lCustomTriggerEntryIter, lCustomTriggerMemoryId);                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_ComplexIter32BitExists(&lCustomTriggerEntryIter) == TRUE;                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_ComplexIter32BitNext(&lCustomTriggerEntryIter))                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        Dem_Cfg_EntryIndexType lEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lCustomTriggerEntryIter);                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_EventIdType lEventId = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lEntryIndex)->EventId;
        if (lEventId != DEM_EVENT_INVALID)
        {
          uint32 lTimestamp = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lEntryIndex)->Timestamp;
          Dem_Cfg_EntryIndexType lRemoveEntryIndex = DEM_CFG_ENTRYINDEX_INVALID;

          if ((lEventId >= Dem_Cfg_GlobalEventCount())
            || (Dem_Core_TestCustomTriggerConfigured(lEventId) == FALSE)
            || (Dem_Memories_CustomTriggerMemory_GetMemoryId(lEventId) != lCustomTriggerMemoryId)
# if (DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON)
            || (lEventId != Dem_Cfg_GetMasterEvent(lEventId))
# endif
            )
          {
            lRemoveEntryIndex = lEntryIndex;
            /* DET: To get this inconsistency, it requires a configuration change without clearing the NV contents */
            Dem_Error_ReportError(DEM_INIT_APIID, DEM_E_INCONSISTENT_STATE);
          }
          else
          {
            /* find duplicate of time series entry */
            Dem_Cfg_EntryIndexType lOtherEntryIndex = Dem_Memories_CustomTriggerMemory_FindEntryIndex(lEventId);

            if (lOtherEntryIndex != lEntryIndex)
            {
              /* Clear the older entry */
              if (lTimestamp > Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lOtherEntryIndex)->Timestamp)
              {
                lRemoveEntryIndex = lOtherEntryIndex;
              }
              else
              {
                lRemoveEntryIndex = lEntryIndex;
              }
            }
          }

          if (lRemoveEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
          {
            Dem_Nvm_SetSingleBlockState(Dem_MemoryEntry_CustomTriggerEntry_GetEntryIndexToNvBlockIndex(lRemoveEntryIndex), DEM_NVM_BLOCKSTATE_DIRTYIMMEDIATE);

            if (lRemoveEntryIndex == lEntryIndex)
            {
              /* Current entry is not yet added in chronology list. Only free it is enough. */
              Dem_FaultMemory_Memories_FreeEntry(lCustomTriggerMemoryId, lRemoveEntryIndex);
              continue;
            }
            else
            {
              /* In this case, lOtherEntryIndex must be smaller than the lEntryIndex since it is the first matching entry. */
              /* This means, lOtherEntryIndex has already been added to chronolist. Thus, remove it from chronology list is necessary. */
              Dem_FaultMemory_Memories_RemoveEntryAndChrono(lCustomTriggerMemoryId, lRemoveEntryIndex);
            }
          }

          /* Synchronize the global timestamp with the timestamp stored in the entry */
          if (lTimestamp > Dem_FaultMemory_Memories_GetCurrentTimestamp())                                                       /* COV_DEM_ROBUSTNESS TX */
          {
            Dem_FaultMemory_Memories_SetCurrentTimestamp(lTimestamp);
          }

          Dem_Memories_CustomTriggerMemory_SortEntryIntoChronologicalList(lCustomTriggerMemoryId, lEntryIndex, lTimestamp);

          /* Update the number of occupied entries */
          Dem_FaultMemory_Memories_SetCurrentSize(lCustomTriggerMemoryId,
            (uint8)(Dem_FaultMemory_Memories_GetCurrentSize(lCustomTriggerMemoryId) + 1u));
        }

      }
      /* Update the global timestamp */
      Dem_FaultMemory_Memories_SetCurrentTimestamp(Dem_FaultMemory_Memories_GetCurrentTimestamp() + 1u);
    }
  }
#endif
}                                                                                                                                /* PRQA S 6030, 6050, 6080 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_StoreCustomTriggeredFreezeFrame
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_StoreCustomTriggeredFreezeFrame(                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  Dem_EventIdType  EventId
  )
{
  Dem_SatelliteSat_SetCustomTriggerActionQueue(Dem_Cfg_EventSatelliteId(EventId), Dem_Cfg_EventSatelliteEventId(EventId));
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_MainFunction
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
Dem_Memories_CustomTriggerMemory_MainFunction(
  void
  )
{
  Dem_Satellite_IdType lSatelliteId;
  for (lSatelliteId = 0u; lSatelliteId < Dem_Cfg_GlobalSatelliteCount(); ++lSatelliteId)
  {
    if (Dem_SatelliteCore_TestAndClearCustomTriggerQueueUpdated(lSatelliteId) == TRUE)
    {
      Dem_Memories_CustomTriggerMemory_ProcessSatelliteQueueActions(lSatelliteId);
    }
  }

  if (Dem_GlobalDiagnostics_TestAnyNewRingBufferSampleAvailable() == TRUE)
  {
    Dem_Memories_CustomTriggerMemory_ProcessNewFutureSampleAvailable();
  }
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_FindEntryIndex
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Memories_CustomTriggerMemory_FindEntryIndex(
  Dem_EventIdType EventId
  )
{
  uint16 lCustomTriggerTranslatedMemoryId = Dem_Memories_CustomTriggerMemory_GetMemoryId(EventId);
  return Dem_FaultMemory_Memories_FindEntryIndexOfEventGeneric(EventId, FALSE, lCustomTriggerTranslatedMemoryId);
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_ClearEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_ClearEvent(
  Dem_EventIdType EventId,
  uint8  BlockStatus
  )
{
  if (Dem_Core_TestCustomTriggerConfigured(EventId) == TRUE)
  {
    Dem_Cfg_EntryIndexType lCustomTriggerEntryIndex = Dem_Memories_CustomTriggerMemory_FindEntryIndex(EventId);
    if (lCustomTriggerEntryIndex != DEM_CFG_ENTRYINDEX_INVALID)
    {
      Dem_Cfg_NvBlockIndexType lNvBlockIndex = Dem_MemoryEntry_CustomTriggerEntry_GetEntryIndexToNvBlockIndex(lCustomTriggerEntryIndex);
      uint16 lTranslatedMemoryId = Dem_Memories_CustomTriggerMemory_GetMemoryId(EventId);
      
      Dem_FaultMemory_Memories_RemoveEntryAndChrono(lTranslatedMemoryId, lCustomTriggerEntryIndex);

      Dem_Nvm_SetSingleBlockState(lNvBlockIndex, BlockStatus);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_TestRecordIsStored
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
Dem_Memories_CustomTriggerMemory_TestRecordIsStored(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex,
  uint8  RecordNumber
  )
{
  boolean lIsStored = FALSE;

#if (DEM_FEATURE_NEED_CUSTOMTRIGGERSTORAGE == STD_ON)
  Dem_EventIdType lEventId = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex)->EventId;

  if (lEventId != DEM_EVENT_INVALID)                                                                                             /* COV_DEM_ROBUSTNESS TX */
  {
    if (Dem_Memories_CustomTriggerMemory_TestRecordNumberIsSRec(lEventId, RecordNumber) == TRUE)
    {
      lIsStored = TRUE;
    }
# if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
    else if (Dem_Memories_CustomTriggerMemory_TestRecordNumberIsTSSRec(lEventId, RecordNumber) == TRUE)                          /* COV_DEM_ROBUSTNESS TX */
    {
      uint8 lTimeSeriesSampleIndex = Dem_Memories_CustomTriggerMemory_GetTimeSeriesSampleIndex(lEventId, RecordNumber);
      uint8 lPastSamples = Dem_Memories_CustomTriggerMemory_NumPastTssSamples(lEventId);
      uint8 lFutureSamples = Dem_Memories_CustomTriggerMemory_NumFutureTssSamples(lEventId);

      if (lTimeSeriesSampleIndex >= lPastSamples)
      {
        /* Future Sample */
        lTimeSeriesSampleIndex -= lPastSamples;
        lIsStored = (boolean)(lTimeSeriesSampleIndex < (lFutureSamples - Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex)->UnusedFutureSamples));
      }
      else
      {
        /* Past Sample */
        lIsStored = (boolean)(lTimeSeriesSampleIndex >= Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(CustomTriggerEntryIndex)->UnusedPastSamples);
      }
    }
# endif
    else
    {
      /* MISRA else unreachable code */
      Dem_Internal_AssertAlways(DEM_E_INCONSISTENT_STATE);
    }
  }
#endif

  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CustomTriggerEntryIndex)                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(RecordNumber)                                                                                 /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return lIsStored;
}

/* ****************************************************************************
 % Dem_Memories_CustomTriggerMemory_ProcessNewFutureSampleAvailable
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
Dem_Memories_CustomTriggerMemory_ProcessNewFutureSampleAvailable(                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void                                                                                                                           /* PRQA S 2843 */ /* MD_DEM_Dir4.1_ReadOperation */
)
{
#if (DEM_CFG_SUPPORT_CUSTOMTRIGGER_TSSR == STD_ON)
  uint16 lEventMemoryId;

  for (lEventMemoryId = 0;
    lEventMemoryId <= Dem_Memories_EventMemory_UserDefined_GetNumberOfUserDefinedMemories();
    lEventMemoryId++)
  {
    uint16 lCustomTriggerMemoryId = Dem_Memories_EventMemory_GetReferencedCustomTriggerMemoryId(lEventMemoryId);

    if (lCustomTriggerMemoryId != DEM_CFG_MEMORYID_INVALID)
    {
      Dem_Cfg_EntryIterType lCustomTriggerEntryIter;

      for (Dem_Memories_CustomTriggerMemory_EntryIterInit(&lCustomTriggerEntryIter, lCustomTriggerMemoryId);                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_ComplexIter32BitExists(&lCustomTriggerEntryIter) == TRUE;                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_Cfg_ComplexIter32BitNext(&lCustomTriggerEntryIter))                                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        Dem_Cfg_EntryIndexType lCustomTriggerEntryIndex = Dem_Cfg_ComplexIter32BitGet(&lCustomTriggerEntryIter);                 /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_EventIdType lEventId = Dem_MemoryEntry_CustomTriggerEntry_GetEntrySafe(lCustomTriggerEntryIndex)->EventId;
        if ((lEventId != DEM_EVENT_INVALID) && Dem_Core_TestCustomTriggeredTssrConfigured(lEventId))
        {
          Dem_Cfg_SamplingProfileIterType lSamplingProfileId = Dem_Memories_CustomTriggerMemory_GetSamplingProfileId(lEventId);
          if(Dem_GlobalDiagnostics_TestNewRingBufferSampleAvailable(lSamplingProfileId))
          {
            Dem_Memories_CustomTriggerMemory_StoreFutureSamples(lCustomTriggerEntryIndex);
          }
        }
      }
    }
  }
#endif
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_CUSTOMTRIGGERMEMORY_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_CustomTriggerMemory_Implementation.h
 *********************************************************************************************************************/
