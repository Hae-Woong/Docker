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
 *  \defgroup   Dem_CustomTriggerMemory CustomTriggerMemory
 *  \{
 *  \file       Dem_CustomTriggerMemory_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Interface of the Custom Trigger Memory subcomponent which manages the dedicated memory for custom triggered
 *              snapshot data and time series snapshot data.
 *  \entity     CustomTriggerMemory
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_CUSTOMTRIGGERMEMORY_INTERFACE_H)
#define DEM_CUSTOMTRIGGERMEMORY_INTERFACE_H

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
#include "Dem_CustomTriggerMemory_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_CustomTriggerMemory_PublicProperties Public Properties
 * \{
 */


/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_IsReferredByMemoryId
 *****************************************************************************/
/*!
 * \brief         Test if the given translated memory Id belongs to a ccustom
 *                trigger memory
 *
 * \details       -
 *
 * \param[in]     TranslatedMemoryId
 *                Translated Memory Identifier of the memory.
 *
 * \return        TRUE
 *                The translated memory Id belongs to a custom trigger memory
 * \return        FALSE
 *                The translated memory Id does not belong to a custom trigger
 *                memory
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_IsReferredByMemoryId(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_TestRecordNumberIsValid
 *****************************************************************************/
/*!
 * \brief         Tests whether the record number belongs to custom triggered
 *                snaphsot record or time series snaphshot record
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     RecordNumber
 *                Snapshot record number
 *
 * \return        TRUE
 *                The record number belongs to custom triggered snaphsot record
 *                or time series snaphshot record
 * \return        FALSE
 *                The record number does not belong to custom triggered snaphsot
 *                record or time series snaphshot record
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestRecordNumberIsValid(
  Dem_EventIdType  EventId,
  uint8 RecordNumber
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_TestRecordNumberIsSRec
 *****************************************************************************/
/*!
 * \brief         Tests whether the record number belongs to custom triggered
 *                snaphsot record.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     RecordNumber
 *                Snapshot record number
 *
 * \return        TRUE
 *                The record number belongs to custom triggered snaphsot record
 * \return        FALSE
 *                The record number does not belong to custom triggered snaphsot
 *                record
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestRecordNumberIsSRec(
  Dem_EventIdType  EventId,
  uint8 RecordNumber
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_GetSRecNumber
 *****************************************************************************/
/*!
 * \brief         Returns the snapshot record number of the event's custom
 *                triggered snapshot record.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The snapshot record number of the event's custom triggered
 *                snapshot record or 0xFF in case that the event does not support
 *                any custom triggered snapshot record
 *
 * \pre           Event must support custom triggered snapshot record
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_GetSRecNumber(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_TestRecordNumberIsTSSRec
 *****************************************************************************/
/*!
 * \brief         Tests whether the requested SRec number belongs to the event's
 *                custom triggered time series snapshots
 *
 * \details       If event supports custom triggered time series snapshot data,
 *                the record number is valid if it is in the range of first and last 
 *                valid custom triggered time series snapshot record number
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     RecordNumber
 *                Snapshot record number
 *
 * \return        TRUE
 *                The record number belongs to custom triggered TSS record
 * \return        FALSE
 *                The record number does not belong to custom triggered TSS
 *                record or custom triggered TSS is not supported for event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestRecordNumberIsTSSRec(
  Dem_EventIdType  EventId,
  uint8 RecordNumber
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_GetTimeSeriesSampleIndex
 *****************************************************************************/
/*!
 * \brief         Returns the sample index of the requested record number
 *                in the Custom triggered time series snapshot data array
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     RecordNumber
 *                Record number of custom triggered time series record
 *
 * \return        TimeSeriesSampleIndex
 *                Index at which the record is stored in the Time series snapshot array
 *
 * \pre           Record Number is valid
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_GetTimeSeriesSampleIndex(
  Dem_EventIdType  EventId,
  uint8 RecordNumber
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_FirstTSSRecNum
 *****************************************************************************/
/*!
 * \brief         Returns the first custom triggered time series record number
 *                for the event
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        RecordNumber
 *                First custom triggered time series record number for event
 *                DEM_CUSTOMTRIGGERMEMORY_SREC_NUMBER_INVALID
 *                if event doesnot support custom triggered time series
 *
 * \pre           Event should support custom triggered time series snapshot
 *                records
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_FirstTSSRecNum(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_LastTSSRecNum
 *****************************************************************************/
/*!
* \brief         Returns the last custom triggered time series record number for the
*                event
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        RecordNumber
 *                Last custom triggered time series record number for event
 *                DEM_CUSTOMTRIGGERMEMORY_SREC_NUMBER_INVALID
 *                if event doesnot support custom triggered time series
 *
 * \pre           Event should support custom triggered time series snapshot 
 *                records
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_LastTSSRecNum(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_NumPastTssSamples
 *****************************************************************************/
/*!
* \brief          Returns the configured number of past samples for the custom
*                 triggered time series of the event
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        NumPastSamples
 *                Number of the configured number of past samples for the custom
 *                triggered time series of the event
 *                0 is returned if event supports no custom triggered time series
 *
 * \pre           -
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_NumPastTssSamples(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_NumFutureTssSamples
 *****************************************************************************/
/*!
* \brief          Returns the configured number of future samples for the custom
*                 triggered time series of the event
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        NumFutureSamples
 *                Number of the configured number of future samples for the custom
 *                triggered time series of the event
 *                0 is returned if event supports no custom triggered time series
 *
 * \pre           -
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_NumFutureTssSamples(
  Dem_EventIdType  EventId
  );

 /* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_IsEnabled
 *****************************************************************************/
 /*!
 * \brief         Return if  custom triggered storage is enabled in the configuration.
 * \details       -
 *
 * \return        TRUE
 *                Custom triggered storage is supported
 * \return        FALSE
 *                Custom triggered storage is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_IsEnabled(
  void
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_MaxTssSamplesGet
 *****************************************************************************/
/*!
 * \brief         Get the maximum number of samples that may be stored in 
 *                the custom triggered entry
 *
 * \details       -
 *
 * \return        The maximum number of custom time series samples that may be stored
 *                0 if  custom triggered storage is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_CustomTriggerMemory_MaxTssSamplesGet(
  void
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
 * \defgroup Dem_CustomTriggerMemory_Public Public Methods
 * \{
 */


/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_Init
 *****************************************************************************/
/*!
 * \brief         Initializes the custom trigger storage.
 *
 * \details       -
 *
 * \pre           Can only be called during Dem initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_Init(
  void
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_InitNvData
 *****************************************************************************/
/*!
 * \brief         Re-initialize all NV Data for custom trigger storage
 *
 * \details       -
 *
 * \pre           NvM must have restored NV mirrors
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_InitNvData(
  void
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_RestoreOnInit
 *****************************************************************************/
/*!
 * \brief         Validate and recover custom trigger entries
 *
 * \details       -
 *
 * \pre           NvM must have restored NV mirrors
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_RestoreOnInit(
  void
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_StoreCustomTriggeredFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Queue custom storage trigger for event
 *
 * \details       Queue custom storage trigger for event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventIds.
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_StoreCustomTriggeredFreezeFrame(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_MainFunction
 *****************************************************************************/
/*!
 * \brief         Process queued custom trigger operations
 *
 * \details       Process queued custom trigger operations
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_MainFunction(
  void
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_FindEntryIndex
 *****************************************************************************/
/*!
 * \brief         Finds the entry index of the allocated custom trigger entry
 *                for event.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The custom trigger entry index allocated to the event or
 *                invalid entry index in case that no allocated custom trigger
 *                entry is found.
 *
 * \pre           -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Memories_CustomTriggerMemory_FindEntryIndex(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_ClearEvent
 *****************************************************************************/
/*!
 * \brief         Remove the custom trigger entry for the event
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     BlockStatus
 *                New status of the memory block.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_ClearEvent(
  Dem_EventIdType EventId,
  uint8 BlockStatus
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_ProcessNewFutureSampleAvailable
 *****************************************************************************/
/*!
 * \brief         Store future samples for all custom triggered time series entries
 *
 * \details       -
 *
 * \pre           Custom triggered time series is supported
 *
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_CustomTriggerMemory_ProcessNewFutureSampleAvailable(
  void
  );

/* ****************************************************************************
 * Dem_Memories_CustomTriggerMemory_TestRecordIsStored
 *****************************************************************************/
/*!
 * \brief         Check if requested record number is stored in custom trigger entry
 *
 * \details       -
 *
 * \param[in]     CustomTriggerEntryIndex
 *                Index of the entry in Dem_Cfg_CustomTriggerEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfCustomTriggerEntryPtr()[.
 * \param[in]     RecordNumber
 *                Custom trigger record number
 *
 * \return        TRUE
 *                The record corresponding to the requested record number is
 *                stored in custom trigger entry
 * \return        FALSE
 *                The record corresponding to the requested record number is not
 *                stored in custom trigger entry
 *
 * \pre           -
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_CustomTriggerMemory_TestRecordIsStored(                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_EntryIndexType CustomTriggerEntryIndex,
  uint8  RecordNumber
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_CUSTOMTRIGGERMEMORY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_CustomTriggerMemory_Interface.h
 *********************************************************************************************************************/
