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
 *  \defgroup   Dem_EventInternalStatus EventInternal Status
 *  \{
 *  \file       Dem_InternalStatus_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Interface of the EventInternalStatus subcomponent
 *  \entity     InternalStatus
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EVENTINTERNALSTATUS_INTERFACE_H)
#define DEM_EVENTINTERNALSTATUS_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_InternalStatus_Types.h"
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_InternalStatus_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Event_InternalStatus_TestEventStored
 *****************************************************************************/
/*!
 * \brief         Tests whether an event is currently stored (an event entry
 *                exists)
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                An event entry exists for event
 * \return        FALSE
 *                No event entry exists for event
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_InternalStatus_TestEventStored(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_InternalStatus_GetStoredStatus
 *****************************************************************************/
/*!
 * \brief         Get the stored status of the memory representative event.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        The stored status of the memory representative event.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_InternalStatus_StoredStatusType, DEM_CODE)
Dem_Event_InternalStatus_GetStoredStatus(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_InternalStatus_SetStoredStatus
 *****************************************************************************/
/*!
 * \brief         Set the stored status of the memory representative event.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     StoredStatus
 *                New stored status to set
 *
 * \pre           Must be called within critical section DiagMonitor
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_SetStoredStatus(
  Dem_EventIdType EventId,
  Dem_InternalStatus_StoredStatusType StoredStatus
  );

/* ****************************************************************************
 * Dem_Event_InternalStatus_GetCombinedGroupStatus
 *****************************************************************************/
/*!
 * \brief         Get current internal status of the combined group
 *
 * \details       Get current internal status of the combined group
 *
 * \param[in]     GroupId
 *                Unique handle of the combined group
 *
 * \return        Current internal status of the combined group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Event_InternalStatusType, DEM_CODE)
Dem_Event_InternalStatus_GetCombinedGroupStatus(
  Dem_Cfg_CombinedGroupIndexType GroupId
  );


/* ****************************************************************************
 * Dem_Event_InternalStatus_SetCombinedGroupStatus
 *****************************************************************************/
/*!
 * \brief         Set current internal status of the combined group
 *
 * \details       Set current internal status of the combined group
 *
 * \param[in]     GroupId
 *                Unique handle of the combined group
 * \param[in]     Status
 *                New internal status the combined group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_SetCombinedGroupStatus(
  Dem_Cfg_CombinedGroupIndexType GroupId,
  Dem_Event_InternalStatusType Status
  );

/* ****************************************************************************
 * Dem_Event_InternalStatus_GetEventStatus
 *****************************************************************************/
/*!
 * \brief         Get current internal status of the event
 *
 * \details       Get current internal status of the event
 *
 * \param[in]     EventId
 *                Unique handle of the event
 *
 * \return        Current internal status of the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Event_InternalStatusType, DEM_CODE)
Dem_Event_InternalStatus_GetEventStatus(
   Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_InternalStatus_SetEventStatus
 *****************************************************************************/
/*!
 * \brief         Set current internal status value of the event
 *
 * \details       Set current internal status value of the event
 *
 * \param[in]     EventId
 *                Unique handle of the event
 * \param[in]     Status
 *                New internal status of the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_SetEventStatus(
  Dem_EventIdType EventId,
  Dem_Event_InternalStatusType Status
  );

/* ****************************************************************************
 * Dem_Event_InternalStatus_SetBit
 *****************************************************************************/
/*!
 * \brief         Sets a single bit in the event internal status.
 *
 * \details       -
 *
 * \param[in]     InternalStatus
 *                Internal status byte
 * \param[in]     BitIndex
 *                Bit index to set in the status byte
 *
 * \return        Internal status byte with the set bit value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Event_InternalStatusType, DEM_CODE)
Dem_Event_InternalStatus_SetBit(
  Dem_Event_InternalStatusType InternalStatus,
  Dem_InternalStatus_InternalStatusBitsType BitIndex
);

/* ****************************************************************************
 * Dem_Event_InternalStatus_ResetBit
 *****************************************************************************/
/*!
 * \brief         Resets a single bit in the event internal status.
 *
 * \details       -
 *
 * \param[in]     InternalStatus
 *                Internal status byte
 * \param[in]     BitIndex
 *                Bit index to set in the status byte
 *
 * \return        Internal status byte with the reset bit value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Event_InternalStatusType, DEM_CODE)
Dem_Event_InternalStatus_ResetBit(
  Dem_Event_InternalStatusType InternalStatus,
  Dem_InternalStatus_InternalStatusBitsType BitIndex
);


/* ****************************************************************************
 * Dem_Event_InternalStatus_TestBit
 *****************************************************************************/
/*!
 * \brief         Tests whether a single bit in the event internal status is set.
 *
 * \details       -
 *
 * \param[in]     InternalStatus
 *                Internal status byte
 * \param[in]     BitIndex
 *                Bit index to test in the status byte
 *
 * \return        TRUE
 *                Bit is set
 * \return        FALSE
 *                Bit is not set
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
  DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
  Dem_Event_InternalStatus_TestBit(
    Dem_Event_InternalStatusType InternalStatus,
    Dem_InternalStatus_InternalStatusBitsType BitIndex
  );

/* ****************************************************************************
 * Dem_Event_InternalStatus_QualifyStatus_GetEventStatus
 *****************************************************************************/
/*!
 * \brief         Get current qualification status of the event
 *
 * \details       Get current qualification status of the event
 *
 * \param[in]     EventId
 *                Unique handle of the event
 *
 * \return        Current qualification status of the event
 *
 * \pre           Obd is enabled
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_InternalStatus_QualifyStatus_SetEventStatus
 *****************************************************************************/
/*!
 * \brief         Set current qualification status of the event
 *
 * \details       Set current qualification status of the event
 *
 * \param[in]     EventId
 *                Unique handle of the event
 * \param[in]     Status
 *                New qualification status of the event
 *
 * \pre           Event is OBD related
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType Status
  );

/* ****************************************************************************
 * Dem_Event_InternalStatus_QualifyStatus_SetBit
 *****************************************************************************/
/*!
 * \brief         Sets a single bit in the event qualify status.
 *
 * \details       -
 *
 * \param[in]     QualifyStatus
 *                Qualify status byte
 * \param[in]     BitIndex
 *                Bit index to set in the status byte
 *
 * \return        Internal status byte with the set bit value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InternalStatus_QualifyStatus_SetBit(
  Dem_UDSStatus_StatusType QualifyStatus,
  Dem_InternalStatus_QualifyStatusBitsType BitIndex
);

/* ****************************************************************************
 * Dem_Event_InternalStatus_QualifyStatus_ResetBit
 *****************************************************************************/
/*!
 * \brief         Resets a single bit in the qualify status.
 *
 * \details       -
 *
 * \param[in]     QualifyStatus
 *                Qualify status byte
 * \param[in]     BitIndex
 *                Bit index to set in the status byte
 *
 * \return        Internal status byte with the reset bit value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InternalStatus_QualifyStatus_ResetBit(
  Dem_UDSStatus_StatusType QualifyStatus,
  Dem_InternalStatus_QualifyStatusBitsType BitIndex
);

/* ****************************************************************************
 * Dem_Event_InternalStatus_QualifyStatus_TestBit
 *****************************************************************************/
/*!
 * \brief         Tests whether a single bit in the event internal status is set.
 *
 * \details       -
 *
 * \param[in]     QualifyStatus
 *                Qualify status byte
 * \param[in]     BitIndex
 *                Bit index to test in the status byte
 *
 * \return        TRUE
 *                Bit is set
 * \return        FALSE
 *                Bit is not set
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
  DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
  Dem_Event_InternalStatus_QualifyStatus_TestBit(
    Dem_UDSStatus_StatusType QualifyStatus,
    Dem_InternalStatus_QualifyStatusBitsType BitIndex
  );

/* ****************************************************************************
 * Dem_Event_InternalStatus_GetMilGroupPendingCount
 *****************************************************************************/
/*!
 * \brief         Get the number of pending events of the MIL group
 *
 * \details       Get the number of pending events of the MIL group
 *
 * \param[in]     MilGroupIndex
 *                Unique handle of the MIL group
 *
 * \return        The number of pending events of the MIL group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Event_InternalStatus_GetMilGroupPendingCount(
  uint16 MilGroupIndex
  );


/* ****************************************************************************
 * Dem_Event_InternalStatus_SetMilGroupPendingCount
 *****************************************************************************/
/*!
 * \brief         Set the number of pending events of the MIL group
 *
 * \details       Set the number of pending events of the MIL group
 *
 * \param[in]     MilGroupIndex
 *                Unique handle of the MIL group
 * \param[in]     PendingCount
 *                New number of pending events of the MIL group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_SetMilGroupPendingCount(
  uint16 MilGroupIndex,
  uint16 PendingCount
  );

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
 * \defgroup Dem_InternalStatus_Private Private Methods
 * \{
 */

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
 * \defgroup Dem_InternalStatus_Public Public Methods
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_EVENTINTERNALSTATUS_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_InternalStatus_Interface.h
 *********************************************************************************************************************/
