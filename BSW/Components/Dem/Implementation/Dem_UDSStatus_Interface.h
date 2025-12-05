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
 *  \defgroup   Dem_UDSStatus UDSStatus
 *  \{
 *  \file       Dem_UDSStatus_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Public interface of UDSStatus subcomponent
 *  \entity     UDSStatus
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_UDSSTATUS_INTERFACE_H)
#define DEM_UDSSTATUS_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_UDSStatus_Types.h"
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_UDSStatus_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Event_UDSStatus_SetBit
 *****************************************************************************/
/*!
 * \brief         Sets a single bit in the UDS status.
 *
 * \details       -
 *
 * \param[in]     UDSStatus
 *                UDSStatus status byte
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
Dem_Event_UDSStatus_SetBit(
  Dem_UDSStatus_StatusType UDSStatus,
  Dem_UDSStatus_UDSStatusBitsType BitIndex
  );


/* ****************************************************************************
 * Dem_Event_UDSStatus_ResetBit
 *****************************************************************************/
/*!
 * \brief         Resets a single bit in the UDS status.
 *
 * \details       -
 *
 * \param[in]     UDSStatus
 *                UDSStatus status byte
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
Dem_Event_UDSStatus_ResetBit(
  Dem_UDSStatus_StatusType UDSStatus,
  Dem_UDSStatus_UDSStatusBitsType BitIndex
  );

/* ****************************************************************************
 * Dem_Event_UDSStatus_TestBit
 *****************************************************************************/
/*!
 * \brief         Tests whether a single bit in the UDS status is set.
 *
 * \details       -
 *
 * \param[in]     UDSStatus
 *                UDS status byte
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
Dem_Event_UDSStatus_TestBit(
  Dem_UDSStatus_StatusType UDSStatus,
  Dem_UDSStatus_UDSStatusBitsType BitIndex
  );


/* ****************************************************************************
 * Dem_Event_UDSStatus_GetInternalEventUdsStatus
 *****************************************************************************/
/*!
 * \brief         Get current internal UDS status of the event
 *
 * \details       Get current internal UDS status of the event
 *
 * \param[in]     EventId
 *                Unique handle of the event
 *
 * \return        Current internal UDS status of the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_UDSStatus_GetInternalEventUdsStatus(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_UDSStatus_SetInternalEventUdsStatus
 *****************************************************************************/
/*!
 * \brief         Set current UDS status value of the event
 *
 * \details       Set current UDS status value of the event
 *
 * \param[in]     EventId
 *                Unique handle of the event
 * \param[in]     Status
 *                New UDS status of the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_UDSStatus_SetInternalEventUdsStatus(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType Status
  );

/* ****************************************************************************
 * Dem_Event_UDSStatus_GetCombinedGroupStatus
 *****************************************************************************/
/*!
 * \brief         Get current UDS status of the combined group
 *
 * \details       -
 *
 * \param[in]     GroupId
 *                Unique handle of the combined group
 *
 * \return        Current UDS status of the combined group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_UDSStatus_GetCombinedGroupStatus(
  Dem_Cfg_CombinedGroupIndexType GroupId
  );


/* ****************************************************************************
 * Dem_Event_UDSStatus_CalculateAndUpdateCombinedGroupStatus
 *****************************************************************************/
/*!
 * \brief         Calculates and sets the current UDS status of the combined
 *                group
 *
 * \details       -
 *
 * \param[in]     GroupId
 *                Unique handle of the combined group
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_UDSStatus_CalculateAndUpdateCombinedGroupStatus(
  Dem_Cfg_CombinedGroupIndexType GroupId
  );

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
 * \defgroup Dem_UDSStatus_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Event_UDSStatus_GetExternalUDSStatus
 *****************************************************************************/
/*!
 * \brief         Get the current external UDS status of the event
 *
 * \details       Get the current external UDS status of the event.
 *                In case of OBD the non-qualified status bits are masked out.
 *
 * \param[in]     EventId
 *                Unique handle of the event
 * \param[in]     EventStatus
 *                The current internal event UDS status of the event
 *
 * \return        Current external UDS status of the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_UDSStatus_GetExternalUDSStatus(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  );

/* ****************************************************************************
 * Dem_Event_UDSStatus_IsNvRelevantStatusChange
 *****************************************************************************/
/*!
 * \brief         Check, if the status change is relevant to store the NV
 *                event-status memory block
 *
 * \details       Check, if the status change is relevant to store the NV
 *                event-status memory block
 *
 * \param[in]     OldEventStatus
 *                Old event status
 * \param[in]     NewEventStatus
 *                New event status
 *
 * \return        TRUE
 *                Relevant status bit change
 * \return        FALSE
 *                No or not relevant status bit change
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_UDSStatus_IsNvRelevantStatusChange(
  Dem_UDSStatus_StatusType  OldEventStatus,
  Dem_UDSStatus_StatusType  NewEventStatus
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_UDSSTATUS_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_UDSStatus_Interface.h
 *********************************************************************************************************************/
