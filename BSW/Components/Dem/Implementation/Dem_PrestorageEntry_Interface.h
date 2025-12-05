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
 *  \defgroup   Dem_PrestorageEntry
 *  \{
 *  \file       Dem_PrestorageEntry_Interface.h
 *  \brief      Stores and manages single entries of 'prestored freezeframes'.
 *  \details    INterface of the Prestorage Entry unit which handles access to and from prestored freezeframe entries.
 *  \entity     PrestorageEntry
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_PRESTORAGEENTRY_INTERFACE_H)
#define DEM_PRESTORAGEENTRY_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_PrestorageEntry_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_PrestorageEntry_PublicProperties Public Properties
 * \{
 */

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_Checked
 *****************************************************************************/
/*!
 * \brief         Check position 'PrestorageEntry' for being a valid index.
 *
 * \details       If run-time checks are enabled, check index 'PrestorageEntry'. 
 *                When the check fails, return the 'invalid' prestorage entry
 *                that points to an existing temporary buffer element.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 *
 * \return        Without runtime checks: argument 'PrestorageEntry'.
 * \return        With runtime checks: 'PrestorageEntry' for valid positions,
 *                otherwise DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY (which is the
 *                position of the temporary buffer element).
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_PrestorageIndexType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_Checked(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC) PrestorageEntry
  );
#endif


#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_GetEventId
 *****************************************************************************/
/*!
 * \brief         Get the EventId of a prestored entry.
 *
 * \details       Get the EventId of a prestored entry. The requested entry
 *                must be allocated and not be part of the free list.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 *
 * \return        EventId that has set this prestored entry.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetEventId(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  );
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_SetEventId
 *****************************************************************************/
/*!
 * \brief         Set the EventId of a prestored entry.
 *
 * \details       Set EventId of a prestored entry.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 * \param[in]     EventId
 *                New EventId for the entry.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_SetEventId(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry,
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON) && (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_GetGlobalSRecDataPtr
 *****************************************************************************/
/*!
 * \brief         Get data buffer for global snapshot record data of a prestored
 *                entry.
 *
 * \details       Get data buffer for global snapshot record data of a prestored
 *                entry.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 *
 * \return        Address of the data buffer.
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
 *                && (DEM_CFG_SUPPORT_GLOBAL_SRECS == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetGlobalSRecDataPtr(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  );
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON) && (DEM_CFG_SUPPORT_SRECS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_GetSRecDataPtr
 *****************************************************************************/
/*!
 * \brief         Get data buffer for UDS snapshot record data of a prestored
 *                entry.
 *
 * \details       Get data buffer for UDS snapshot record data of a prestored
 *                entry.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 *
 * \return        Address of the data buffer.
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
 *                && (DEM_CFG_SUPPORT_SRECS == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetSRecDataPtr(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  );
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON) && (DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_GetObdFFDataPtr
 *****************************************************************************/
/*!
 * \brief         Get data buffer for OBD freeze frame data of a prestored
 *                entry.
 *
 * \details       Get data buffer for OBD freeze frame data of a prestored
 *                entry.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 *
 * \return        Address of the data buffer.
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
 *                && ((DEM_CFG_SUPPORT_OBDII_OR_OBDONUDS == STD_ON)) 
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetObdFFDataPtr(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  );
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON) && ((DEM_CFG_SUPPORT_J1939_FREEZEFRAME == STD_ON) || (DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME == STD_ON))
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_GetJ1939FFDataPtr
 *****************************************************************************/
/*!
 * \brief         Get data buffer for J1939 freeze frame data of a prestored
 *                entry.
 *
 * \details       Get data buffer for J1939 freeze frame data of a prestored
 *                entry.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 *
 * \return        Address of the data buffer.
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
 *                && ( (DEM_CFG_SUPPORT_J1939_FREEZEFRAME == STD_ON)
 *                  || (DEM_CFG_SUPPORT_J1939_EXPANDED_FREEZEFRAME == STD_ON))
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetJ1939FFDataPtr(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  );
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON) && (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_GetWwhObdFFDataPtr
 *****************************************************************************/
/*!
 * \brief         Get data buffer for WWH-OBD freeze frame data of a prestored
 *                entry.
 *
 * \details       Get data buffer for WWH-OBD freeze frame data of a prestored
 *                entry.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 *
 * \return        Address of the data buffer.
 *
 * \pre           -
 * \config        (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
 *                && (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetWwhObdFFDataPtr(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  );
#endif

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_GetState
 *****************************************************************************/
/*!
 * \brief         Get the state of a prestored entry.
 *
 * \details       Get the state of a prestored entry.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 *
 * \return        DEM_PRESTORE_STATUS_FREE
 *                Prestored entry is free.
 * \return        DEM_PRESTORAGEMEMORY_STATUS_READY
 *                Prestored entry contains valid data.
 * \return        DEM_PRESTORAGEMEMORY_STATUS_UPDATING
 *                Prestored entry is updating data.
 * \return        DEM_PRESTORE_STATUS_LOCKED
 *                Prestored entry is locked.
 * \return        DEM_PRESTORE_STATUS_DISCARDED
 *                Prestored entry needs to be freed.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetState(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  );
#endif


#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_SetState
 *****************************************************************************/
/*!
 * \brief         Set the state of a prestored entry.
 *
 * \details       Set state of a prestored entry.
 *
 * \param[in]     PrestorageEntry
 *                Position of a prestorage entry.
 * \param[in]     State
 *                New state for the entry.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_SetState(
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry,
  CONST(uint8, AUTOMATIC)  State
  );
#endif


#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_SetEntry
 *****************************************************************************/
/*!
 * \brief         Set the prestorage buffer slot, allocated to the event.
 *
 * \details       Set the prestorage buffer, that is allocated to the event.
 *
 * \param[in]     EventId
 *                Unique handle of the event
 * \param[in]     PrestorageEntry
 *                Prestorage buffer slot (currently) allocated to the event.
 *                DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY is '(currently) no buffer'.
 *
 * \pre           -
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_SetEntry(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId,
  CONST(Dem_Cfg_PrestorageIndexType, AUTOMATIC)  PrestorageEntry
  );
#endif

/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_EventHasEntryAssigned
 *****************************************************************************/
/*! 
 * \brief         Return if a pre-storage entry is assigned to the event
 *
 * \details       Return if a pre-storage entry is assigned to the event
 *
 * \param[in]     Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.

 * \return        TRUE
 *                Pre-storage entry is assigned to the event.
 *                FALSE
 *                No pre-storage entry is assigned to the event.
 *
 * \pre           The event must support pre-storage.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_EventHasEntryAssigned(
  Dem_EventIdType EventId
);

#if (DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON)
/* ****************************************************************************
 * Dem_MemoryEntry_PrestorageEntry_GetEntry
 *****************************************************************************/
/*!
 * \brief         Locates the prestored data entry of an event.
 *
 * \details       This function looks up the prestorage entry for the given
 *                event, and returns its handle (position in buffer).
 *
 * \param[in]     EventId
 *                Unique handle of the event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *                The event must have configured prestorage.
 *
 * \return        Position in Prestorage buffer.
 *
 * \pre           The event must support prestorage, always proove that before
 *                by calling Dem_Cfg_EventPrestorageIndex(EventId). If run-time
 *                checks are enabled, a violation of this precondition will
 *                return DEM_PRESTORAGEMEMORY_INVALID_BUFFER_ENTRY.
 * \config        DEM_CFG_SUPPORT_PRESTORED_FF == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_PrestorageIndexType, DEM_CODE)
Dem_MemoryEntry_PrestorageEntry_GetEntry(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );
#endif


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
 * \defgroup Dem_PrestorageEntry_Public Public Methods
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_PRESTORAGEENTRY_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_PrestorageEntry_Interface.h
 *********************************************************************************************************************/
