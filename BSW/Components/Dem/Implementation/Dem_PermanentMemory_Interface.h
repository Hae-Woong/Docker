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
 *  \defgroup   Dem_PermanentMemory MemPermanent
 *  \{
 *  \file       Dem_PermanentMemory_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file.
 *  \details    Implementation of the Permanent Memory subcomponent which manages storage of permanent DTCs.
 *  \entity     PermanentMemory
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_PERMANENTMEMORY_INTERFACE_H)
#define DEM_PERMANENTMEMORY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_PermanentMemory_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_PermanentMemory_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_SetEnabled
 *****************************************************************************/
/*!
 * \brief         Sets the enabled state of the permanent memory.
 *
 * \details       Sets the enabled state of the permanent memory, if supported
 *                by configuration.
 *
 * \param[in]     State
 *                The state to set the permanent memory enabled state to.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_SetEnabled(
  boolean State
  );

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_TestEnabled
 *****************************************************************************/
/*!
 * \brief         Returns the current state of the permanent memory.
 *
 * \details       Returns the current state of the permanent memory. If permanent
 *                memory is not supported, state 'disabled' is always returned.
 *
 * \return        TRUE
 *                Permanent memory has been enabled.
 * \return        FALSE
 *                Permanent memory has not been enabled yet or is not supported
 *                by configuration.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Memories_PermanentMemory_TestEnabled(
  void
  );

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_SetInitPattern
 *****************************************************************************/
/*!
 * \brief         Set the init pattern of the permanent memory.
 *
 * \details       Set the init pattern of the permanent memory.
 *
 * \param[in]     InitPattern
 *                New init pattern to be set
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_SetInitPattern(
  CONST(uint16, AUTOMATIC)  InitPattern
  );
#endif

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_GetInitPattern
 *****************************************************************************/
/*!
 * \brief         Get the init pattern of the permanent memory.
 *
 * \details       Get the init pattern of the permanent memory.
 *
 * \return        The init pattern of the permanent memory
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Memories_PermanentMemory_GetInitPattern(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_ResetOverflown
 *****************************************************************************/
/*!
 * \brief         Mark the confirmed chronology as NOT overflown.
 *
 * \details       Mark the confirmed chronology as NOT overflown.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_ResetOverflown( 
  void
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
 * \defgroup Dem_PermanentMemory_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_RestoreMemory
 *****************************************************************************/
/*!
 * \brief         Initializes Permanent memory
 *
 * \details       This function verifies the integrity of permanent entries and
 *                links them up into the runtime control structures.
 *
 * \pre           NvM must have restored NV mirrors
 * \config        Feature OBD II, OBDonUDS or WWH-OBD is enabled
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_RestoreMemory(
  void
  );

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_UpdatePermanentEntryOBD
 *****************************************************************************/
/*!
 * \brief         Processes the permanent memory state update.
 *
 * \details       This function transitions the permanent memory states at the
 *                end of a driving cycle if ObdII and permanent memory is supported.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     OldDtcStatus
 *                Dtc status before DCY restart
 * \param[in]     NewDtcStatus
 *                Dtc status after DCY restart
 *
 * \pre           The passed event must be a master event
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_UpdatePermanentEntryOBD(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  OldDtcStatus,
  CONST(uint8, AUTOMATIC)  NewDtcStatus
  );

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_UpdatePermanentEntryWWHOBD
 *****************************************************************************/
/*!
 * \brief         Processes permanent state changes regarding clearDTC.
 *
 * \details       This function transitions the permanent memory states at the
 *                end of a driving cycle required for entry removal with ClearDTC
 *                if permanent memory for WwhObd is enabled.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     OldDtcStatus
 *                Dtc status before DCY restart
 * \pre           The passed event must be a master event
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_UpdatePermanentEntryWWHOBD(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(uint8, AUTOMATIC)  OldDtcStatus
  );


/* ****************************************************************************
 * Dem_Memories_PermanentMemory_AddEvent
 *****************************************************************************/
/*!
 * \brief         Stores an event to permanent memory
 *
 * \details       If a free slot is available in the permanent memory, this
 *                function stores a permanent entry for the given event.
 *
 *                This function maps combined events to their master event, so
 *                any event can be passed.
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_AddEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ClearEvent
 *****************************************************************************/
/*!
 * \brief         Processes clearing a permanent DTC
 *
 * \details       Processes clearing a permanent DTC
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     ProcessClearDTC
 *                TRUE  Function is called from ClearDTC processing context
 *                FALSE Function is not called from ClearDTC processing context
 *
 * \pre           The passed event must be a master event
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ClearEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(boolean, AUTOMATIC)   ProcessClearDTC
  );

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_FillUp
 *****************************************************************************/
/*!
 * \brief         Fills up the permanent memory after healing some entries
 *
 * \details       This function checks for OBD relevant DTCs that currently
 *                trigger the MIL but are not stored in permanent memory. Up to
 *                all empty permanent slots are filled with such events.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_FillUp(
  void
  );

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_EnableStorage
 *****************************************************************************/
/*!
 * \brief         Activates permanent storage
 *
 * \details       For production purposes the permanent storage can be delayed
 *                for some time. This method is used to activate the permanent
 *                storage.
 *
 *                If the permanent storage is not enabled actively, the Dem
 *                will enable it when receiving a configured odometer value.
 *
 * \return        E_OK
 *                Request processed successfully
 * \return        E_NOT_OK
 *                Configuration does not support OBDII or OBDonUDS
 *
 * \pre           -
 *
 * \context       TASK|ISR2, from the master partition only
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(Std_ReturnType, DEM_CODE)
Dem_Memories_PermanentMemory_EnableStorage(
  void
  );

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ProcessActivationMode4Enabled
 *****************************************************************************/
/*!
 * \brief         Ensure existing permanent entries of B1 DTCs are in state 'Active'
 *
 * \details       Search for existing permanent entries of B1 DTCs. If 'Passive'
 *                entries are found, set them to 'Active'.
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_PERMANENT_FOR_WWHOBD == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ProcessActivationMode4Enabled(
  void
  );


#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_Init
 *****************************************************************************/
/*!
 * \brief         Initialize complete confirmed chronology with 'invalid event id'
 *
 * \details       Initialize complete confirmed chronology with 'invalid event id'
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_Init(
  void
  );
#endif

#if (DEM_CFG_SUPPORT_PERMANENT == STD_ON)
/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_Restore
 *****************************************************************************/
/*!
 * \brief         Initialize and validates confirmed chronology of permanent memory
 *
 * \details       Initialize current size of chronology and removes entries in 
 *                chronology belonging to events that do not belong to primary 
 *                memory anymore or do not have the 'Confirmed Bit' set.
 *
 * \param[in]     -
 *
 * \pre           Initialization and restoration of event status at 
 *                Dem initialization is done.
 *
 * \config        DEM_CFG_SUPPORT_PERMANENT == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_Restore(
  void
  );
#endif

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_FreeEvent
 *****************************************************************************/
/*!
 * \brief         Frees the entry in confirmed chronology for the event
 *
 * \details       Move all entries that are newer than the entry to delete 
 *                one step forward in the chronology array until the entry
 *                with the forwarded event is overwritten.
 *
 * \param[in]     EventId
 *                Unique handle of the event.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_FreeEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_Memories_PermanentMemory_ConfirmedChrono_AddEvent
 *****************************************************************************/
/*!
 * \brief         Add an event to confirmed chronology
 *
 * \details       Add an event to confirmed chronology.
 *
 * \param[in]     EventId
 *                Unique handle of the event.
 * \param[in]     SearchExistingEvent
 *                TRUE  Add the event only if there is no existing entry
 *                FALSE Add the event unconditionally
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Memories_PermanentMemory_ConfirmedChrono_AddEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  ConfirmedEvent,
  CONST(boolean, AUTOMATIC) SearchExistingEvent
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_PERMANENTMEMORY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_PermanentMemory_Interface.h
 *********************************************************************************************************************/
