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
 *  \defgroup   Dem_PrestorageMemory Prestore
 *  \{
 *  \file       Dem_PrestorageMemory_Interface.h
 *  \brief      Stores and manages 'prestored freezeframes'.
 *  \details    Pre-store snapshot data for an event, in order to freeze that data set when the event is stored later.
 *  \entity     PrestorageMemory
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_PRESTORAGEMEMORY_INTERFACE_H)
#define DEM_PRESTORAGEMEMORY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_PrestorageMemory_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_PrestorageMemory_PublicProperties Public Properties
 * \{
 */

 /* ****************************************************************************
 * Dem_Memories_PrestorageMemory_isPrestoreFFEnabled
 *****************************************************************************/
 /*!
 * \brief         Return if prestorage of freeze frames is supported.
 * \details       -
 *
 * \return        TRUE
 *                Supported
 * \return        FALSE
 *                Not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_PrestorageMemory_isPrestoreFFEnabled(
  void
  );

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_GetLockedIndex
 *****************************************************************************/
/*!
 * \brief         Get the LockedIndex value.
 *
 * \details       Get the LockedIndex value.
 *
 * \return        LockedIndex value.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_PrestorageIndexType, DEM_CODE)
Dem_Memories_PrestorageMemory_GetLockedIndex(
  void
  );
#endif

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
 * \defgroup Dem_PrestorageMemory_Public Public Methods
 * \{
 */

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_Init
 *****************************************************************************/
/*!
 * \brief         Initializes the prestorage free list.
 *
 * \details       Initializes the prestorage free list.
 *
 * \pre           Can only be called during Dem initialization
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_Init(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_StateSetDiscard
 *****************************************************************************/
/*!
 * \brief         Marks a prestored entry as used up
 *
 * \details       Prestored data (if available) is valid throughout the whole
 *                event processing step. This function marks the current data
 *                to be discarded after event processing completes.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_StateSetDiscard(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_EventPrestorageLock
 *****************************************************************************/
/*!
 * \brief         Locks a ready prestored entry to use its data
 *
 * \details       This function prepares the prestored data of the given event
 *                (if available) for consumption during the event processing
 *                step.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Must be called within critical section 'DiagMonitor'
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_EventPrestorageLock(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_EventPrestorageRelease
 *****************************************************************************/
/*!
 * \brief         Unlock the previously locked prestored entry
 *
 * \details       This function releases the prestored data locked by
 *                Dem_Memories_PrestorageMemory_EventPrestorageLock, if such data exists.
 *
 *                If the data was consumed during the event processing step,
 *                the data is discarded.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_EventPrestorageRelease(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_EventPrestorageClear
 *****************************************************************************/
/*!
 * \brief         Clears the prestorage entry of an event
 *
 * \details       Clears the prestorage entry of an event
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        E_OK
 *                The data was cleared successfully
 * \return        E_NOT_OK
 *                The data was not cleared, e.g. no data was available
 *
 * \pre           Must be called within critical section 'DiagMonitor'
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different events
 *****************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_Memories_PrestorageMemory_EventPrestorageClear(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_EventPrestoreFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Trigger prestoring freeze frame data for an event
 *
 * \details       Trigger prestoring freeze frame data for an event if
 *                preconditions are fulfilled.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        E_OK
 *                The data was collected successfully (if processed synchronously).
 *                Prestore freeze frame request accepted (if processed asynchronously).
 *
 * \return        E_NOT_OK
 *                No data was collected, e.g. due to variant coding (see
 *                Dem_SetEventAvailable() )
 *
 * \pre           Function should only be invoked when feature "Prestorage of 
 *                freeze frames" is supported
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Memories_PrestorageMemory_EventPrestoreFreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_MultiEvent_PrestoreFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Trigger prestoring of freeze frame data for each slave event of the specified event.
 *
 * \details       Trigger prestoring of freeze frame data for each slave event of the specified event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_MultiEvent_PrestoreFreezeFrame(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_EventClearPrestoredFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Trigger clearing the prestored freeze frame data for an event
 *
 * \details       Trigger clearing the prestored freeze frame data for an event
 *                if preconditions are fulfilled.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        E_OK
 *                Prestore data cleared (if processed synchronously).
 *                Clear prestore data request accepted (if processed asynchronously).
 *
 * \return        E_NOT_OK
 *                No data was cleared, e.g. due to variant coding (see
 *                Dem_SetEventAvailable() )
 *
 * \pre           Function should only be invoked when the feature "Support 
 *                prestorage of freeze frames" is supported.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_Memories_PrestorageMemory_EventClearPrestoredFreezeFrame(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_MultiEvent_ClearPrestoredFreezeFrame
 *****************************************************************************/
/*!
 * \brief         Trigger clearing of freeze frame data for each slave event of the specified event.
 *
 * \details       Trigger clearing of freeze frame data for each slave event of the specified event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_MultiEvent_ClearPrestoredFreezeFrame(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_TestEventFreezeFramePrestored
 *****************************************************************************/
/*!
 * \brief         Get if freeze frame data is pre-stored for the event
 *
 * \details       Get if freeze frame data is pre-stored for the event 
 *                waiting to be processed or locked for event status
 *                processing.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Freeze frame data is pre-stored for the event.
 *                FALSE
 *                No freeze frame data is pre-stored for the event.
 *
 * \pre           Function should only be invoked when the feature "Support
 *                prestorage of freeze frames" is supported.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_PrestorageMemory_TestEventFreezeFramePrestored(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_MainFunction
 *****************************************************************************/
/*!
 * \brief         Process queued prestorage operations
 *
 * \details       Process queued prestorage operations
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_MainFunction(
  void
  );

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PrestorageMemory_UpdateEntryOfEvent
 *****************************************************************************/
/*!
 * \brief         Samples all event related data into a prestorage entry
 *
 * \details       Samples all event related data into a prestorage entry
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     PrestorageEntry
 *                Position of the prestorage entry.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PrestorageMemory_UpdateEntryOfEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  );
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_PRESTORAGEMEMORY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_PrestorageMemory_Interface.h
 *********************************************************************************************************************/
