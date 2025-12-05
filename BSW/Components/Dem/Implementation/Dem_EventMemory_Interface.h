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
 *  \defgroup   Dem_EventMemory MemPermanent
 *  \{
 *  \file       Dem_EventMemory_Interface.h
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

#if !defined (DEM_EVENTMEMORY_INTERFACE_H)
#define DEM_EVENTMEMORY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_EventMemory_Types.h"

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_EventMemory_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Memories_EventMemory_GetTotalSize
 *****************************************************************************/
/*!
 * \brief         Get the number of all event entries (primary and user defined)
 * 
 * \details       Get the number of all event entries (primary and user defined)
 *
 * \return        Total number of entries in all event memories 
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Memories_EventMemory_GetTotalSize(
  void
);

/* ****************************************************************************
* Dem_Memories_EventMemory_IsReferredByMemoryId
*****************************************************************************/
/*!
* \brief         Identifies if the input MemoryId is a potential event destination.
*
* \details       Identifies if the input MemoryId is a potential event destination.
*
* \param[in]     TranslatedMemoryId
*                Translated Memory Identifier of the event memory.
*
* \return        TRUE
*                MemoryId is a potential event destination
* \return        FALSE
*                MemoryId is not a potential event destination
*
* \pre           -
*
* \context       TASK/ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_IsReferredByMemoryId(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_Memories_EventMemory_UserDefined_IsEnabled
 *****************************************************************************/
/*!
 * \brief         Gets the enablement status of user defined memory feature.
 *
 * \details       Gets the enablement status of user defined memory feature.
 *
 * \return        TRUE
 *                User Defined Memory is enabled.
 * \return        FALSE
 *                User Defined Memory is disabled.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_IsEnabled(
  void
  );

/* ****************************************************************************
 * Dem_Memories_EventMemory_UserDefined_IsValid
 *****************************************************************************/
/*!
 * \brief         Returns if the input user defined memory origin is valid or not.
 *
 * \details       Returns if the input user defined memory origin is valid or not.
 *
 * \param[in]     DTCOrigin
 *                The event memory of the requested DTC or group of DTC.
 * 
 * \return        TRUE
 *                User Defined Memory Origin is valid.
 * \return        FALSE
 *                User Defined Memory Origin is invalid.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_IsValid(
  Dem_DTCOriginType DTCOrigin
  );

/* ****************************************************************************
* Dem_Memories_EventMemory_UserDefined_GetNumberOfUserDefinedMemories
*****************************************************************************/
/*!
* \brief         Return the number of configured user defined memories.
*
* \details       Return the number of configured user defined memories.
*
* \return        Number of configured user defined memories.
*
* \pre           -
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_GetNumberOfUserDefinedMemories(
  void
  );

/* **************************************************************************** 
 * Dem_Memories_EventMemory_UserDefined_TranslateDTCOrigin
 *****************************************************************************/
/*!
 * \brief         Return index of the User Defined Memory Id to the Memory Info table
 *
 * \details       Return index of the User Defined Memory Id to the Memory Info table
 *
 * \param[in]     DTCOrigin
 *                The event memory of the requested DTC or group of DTC.
 *
 * \return        Index to the Memory Info table.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_TranslateDTCOrigin(
  Dem_DTCOriginType  DTCOrigin
  );


/* ****************************************************************************
* Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculated
*****************************************************************************/
/*!
* \brief         Check if a Snapshot Enumeration type is either Calculated or Calculated FIFO
*
* \details       Check if a Snapshot Enumeration type is either Calculated or Calculated FIFO
*
* \param[in]     NumerationType
*                The Snapshot numeration type
*
* \return        TRUE
*                The enumeration type indicates either Calculated or Calculated FIFO Snapshot Records
* \return        FALSE
*                The enumeration type does not indicate Calculated Snapshot records
*
* \pre           -
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculated(
  Dem_Cfg_SRecNumerationType NumerationType
  );

/* ****************************************************************************
* Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculatedFiFo
*****************************************************************************/
/*!
* \brief         Check if a Snapshot Enumeration type is Calculated FIFO
*
* \details       Check if a Snapshot Enumeration type is Calculated FIFO
*
* \param[in]     NumerationType
*                The Snapshot numeration type
*
* \return        TRUE
*                The enumeration type indicates Calculated FIFO Snapshot Records
* \return        FALSE
*                The enumeration type does not indicate Calculated FIFO Snapshot Records
*
* \pre           -
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_IsSRecNumerationCalculatedFiFo(
  Dem_Cfg_SRecNumerationType NumerationType
  );

/* ****************************************************************************
* Dem_Memories_EventMemory_UserDefined_IsSRecNumerationConfigured
*****************************************************************************/
/*!
* \brief         Check if a Snapshot Enumeration type is Configured
*
* \details       Check if a Snapshot Enumeration type is Configured
*
* \param[in]     NumerationType
*                The Snapshot numeration type
*
* \return        TRUE
*                The enumeration type indicates Configured Snapshot Records
* \return        FALSE
*                The enumeration type does not indicate Configured Snapshot Records
*
* \pre           -
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_UserDefined_IsSRecNumerationConfigured(
  Dem_Cfg_SRecNumerationType NumerationType
  );

/* ****************************************************************************
 * Dem_Memories_EventMemory_TestDTCOriginValid
 *****************************************************************************/
/*!
 * \brief         Tests whether the input DTCOrigin is valid.
 *
 * \details       Tests whether the input DTCOrigin is valid.
 *
 * \param[in]     DTCOrigin
 *                Identifier of the event memory concerned.
 *
 * \return        TRUE
 *                DTCOrigin is valid
 * \return        FALSE
 *                DTCOrigin is not valid
 *
 * \pre           -
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestDTCOriginValid(
 Dem_DTCOriginType  DTCOrigin
 );

/* ****************************************************************************
* Dem_Memories_EventMemory_TestStorageTriggerIsOnConfirmed
*****************************************************************************/
/*!
* \brief         Check if the storage trigger for the memory is On Confirmed
*
* \details       Check if the storage trigger for the memory is On Confirmed
*
* \param[in]     EventId
*                Unique handle of the Event.
*                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()].
*
* \return        TRUE if storage trigger is on Confirmed
*                FALSE if storage trigger is NOT on Confirmed
* 
* \pre           Event must have a DTC configured
*
* \context       TASK
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestStorageTriggerIsOnConfirmed(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
* Dem_Memories_EventMemory_TestStorageTriggerIsOnFailed
*****************************************************************************/
/*!
* \brief         Check if the storage trigger for the memory is On Failed
*
* \details       Check if the storage trigger for the memory is On Failed
*
* \param[in]     EventId
*                Unique handle of the Event.
*                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()].
*
* \return        TRUE if storage trigger is on Failed
*                FALSE if storage trigger is NOT on Failed
*
* \pre           Event must have a DTC configured
*
* \context       TASK
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestStorageTriggerIsOnFailed(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
* Dem_Memories_EventMemory_TestStorageTriggerIsOnFdc
*****************************************************************************/
/*!
* \brief         Check if the storage trigger for the memory is On FDC
*
* \details       Check if the storage trigger for the memory is On FDC
*
* \param[in]     EventId
*                Unique handle of the Event.
*                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()].
*
* \return        TRUE if storage trigger is on FDC 
*                FALSE if storage trigger is NOT on FDC
*
* \pre           Event must have a DTC configured
*
* \context       TASK
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestStorageTriggerIsOnFdc(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
* Dem_Memories_EventMemory_TestStorageTriggerIsOnPending
*****************************************************************************/
/*!
* \brief         Check if the storage trigger for the memory is On Pending
*
* \details       Check if the storage trigger for the memory is On Pending
*
* \param[in]     EventId
*                Unique handle of the Event.
*                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
*
* \return        TRUE if storage trigger is on Pending
*                FALSE if storage trigger is NOT on Pending
*
* \pre           Event must have a DTC configured
*
* \context       TASK
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestStorageTriggerIsOnPending(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
* Dem_Memories_EventMemory_TestStorageTriggerIsFulfilled
*****************************************************************************/
/*!
* \brief         Check if the storage trigger for the event memory is fulfilled
*
* \details       Check if the storage trigger for the event memory is fulfilled
*
* \param[in]     EventId
*                Unique handle of the Event.
*                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()].
* \param[in]     DtcEffects
*                Status change trigger
*
* \return        TRUE if storage trigger for event matches status trigger
*                FALSE if storage trigger for event does NOT match status trigger
*
* \pre           Event must have a DTC configured
*
* \context       TASK
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_EventMemory_TestStorageTriggerIsFulfilled(
  Dem_EventIdType  EventId,
  Dem_Cfg_StorageTriggerType DtcEffects
);

/* ****************************************************************************
 * Dem_Memories_EventMemory_GetReferencedCustomTriggerMemoryId
 *****************************************************************************/
/*!
 * \brief         Get the translated memory Id of the event memory's associated
 *                custom memory
 *
 * \details       -
 * 
 * \param[in]     TranslatedMemoryId
 *                Id of the event memory's associated custom memory
 *                If the event memory supports no custom trigger memoty, an
 *                invalid Id is returned
 *
 * \return        The translated memory Id of the event memory's associated
 *                custom memory
 *
 * \pre           The specified TranslatedMemoryId must refer to primary or an user defined
 *                event memory.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_EventMemory_GetReferencedCustomTriggerMemoryId(
  uint16 TranslatedMemoryId
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_EventMemory_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination
 *****************************************************************************/
/*!
 * \brief         Finds the event entry index for the given event in the memory
 *                the event belongs to.
 *
 * \details       Finds the event entry index for the given event, if it exists.
 *                A DET error migh be thrown if event does not exist, based
 *                on the passed parameters.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     VerifyEventStored
 *                If set to true, a DET error will be thrown in case no entry
 *                was found.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_EntryIndexType, DEM_CODE)
Dem_Memories_EventMemory_FindEntryOfEventAtEventDestination(
  Dem_EventIdType EventId,
  boolean VerifyEventStored
);


#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/* ****************************************************************************
 * Dem_Memories_EventMemory_StoreWwhObdFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Stores a WWHOBD Freeze Frame.
 *
 * \details       Stores a WWHOBD freeze frame.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           The event must be a WWHOBD relevant event.
 * \config        DEM_CFG_SUPPORT_WWHOBD == STD_ON
 *
 * \return        DEM_DATA_USERDATA_CHANGED
 *                User provided data was updated.
 *
 * \context       TASK/ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Memories_EventMemory_StoreWwhObdFreezeFrame(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  );
#endif

/* ****************************************************************************
 * Dem_Memories_EventMemory_FreeEntryOfEvent
 *****************************************************************************/
/*!
 * \brief         Frees an event entry
 *
 * \details       This function frees an event entry.
 *
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
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
Dem_Memories_EventMemory_FreeEntryOfEvent(
  Dem_Cfg_EntryIndexType EventEntryIndex,
  Dem_EventIdType EventId
  );


/* ****************************************************************************
 * Dem_Memories_EventMemory_Restore
 *****************************************************************************/
/*!
 * \brief         Test and restore corrupted primary/user defined memory.
 *
 * \details       This function verifies the integrity of event entries and
 *                links them up into the runtime control structures.
 * 
 * \param[in]     TranslatedMemoryId
 *                Id of the memory for which to restore the event entries
 *
 * \pre           NvM must have restored NV mirrors.
 * \pre           TranslatedMemoryId must refer to primary or an user defined
 *                event memory.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_EventMemory_Restore(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_Memories_EventMemory_UpdateChronology
 *****************************************************************************/
/*!
 * \brief         Moves the passed entry up to most current in the chronology list.
 *
 * \details       Moves the passed entry up to most current in the chronology list.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventEntryIndex
 *                Index of the event entry in Dem_Cfg_EventEntryPtr.
 *                Must be in range [0..Dem_Cfg_GetSizeOfEventEntryPtr()[.
 *
 * \pre           The event must be stored
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_EventMemory_UpdateChronology(
  Dem_EventIdType EventId,
  Dem_Cfg_EntryIndexType EventEntryIndex
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_EVENTMEMORY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EventMemory_Interface.h
 *********************************************************************************************************************/
