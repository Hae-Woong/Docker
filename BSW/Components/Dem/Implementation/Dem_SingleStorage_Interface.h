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
/*! \ingroup    Dem_Master | Dem_Satellite
 *  \defgroup   Dem_SingleStorage SingleStorage
 *  \addtogroup Dem_SingleStorage
 *  \{
 *  \file       Dem_SingleStorage_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Public interface of SingleStorage unit which handles non-combined events or events supporting combination
 *              on retrieval
 *  \unit       SingleStorage
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_SINGLESTORAGE_INTERFACE_H_)
#define DEM_SINGLESTORAGE_INTERFACE_H_

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_SingleStorage_Types.h"
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_SingleStorage_Properties Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Event_SingleStorage_GetMaxFdcToc
 *****************************************************************************/
 /*!
 * \brief         Gets the event's max fault detection counter this operation
 *                cycle.
 * 
 * \details       -
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Max fault detection counter this operation cycle of the event.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_Event_SingleStorage_GetMaxFdcToc(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Event_SingleStorage_GetHealingCounter
 *****************************************************************************/
 /*!
 * \brief         Gets the event's healing counter.
 * 
 * \details       Gets the healing counter of an event which supports single
 *                storage.
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Healing counter value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_SingleStorage_GetHealingCounter(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_SingleStorage_SetHealingCounter
 *****************************************************************************/
 /*!
 * \brief         Sets the event's healing counter.
 * 
 * \details       Sets the healing counter of an event which supports single
 *                storage.
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     CounterValue
 *                New value of the healing counter.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_SingleStorage_SetHealingCounter(
  Dem_EventIdType EventId,
  uint8 CounterValue
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
 * \defgroup Dem_SingleStorage_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Event_SingleStorage_IncrementHealingCounter
 *****************************************************************************/
/*!
 * \brief         Increments the event's healing counter
 *
 * \details       Increments the event's healing counter and
 *                tests whether the healing target is reached
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Healing target is reached
 * \return        FALSE
 *                Healing target is not reached
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_SingleStorage_IncrementHealingCounter(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_SingleStorage_Healed
 *****************************************************************************/
/*!
 * \brief         Process event healed
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current UDS status of the event.
 *
 * \return        Modified UDS status of the event
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_SingleStorage_Healed(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  );

/* ****************************************************************************
 * Dem_Event_SingleStorage_ProcessHealing
 *****************************************************************************/
/*!
 * \brief         Processes the healing counter of an event and calculates the
 *                new event status
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current UDS status of the event
 *
 * \return        Modified UDS status of the event
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_SingleStorage_ProcessHealing(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  );

/* ****************************************************************************
 * Dem_Event_SingleStorage_TestHealingPreventsAging
 *****************************************************************************/
/*!
 * \brief         Tests whether the healing process currently prevents the start
 *                of aging
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Aging is prevented. Event must heal first.
 * \return        FALSE
 *                The event may age during the healing process.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_SingleStorage_TestHealingPreventsAging(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_SingleStorage_ProcessPendingReset
 *****************************************************************************/
/*!
 * \brief         Processes the effects of resetting the event's Pending bit
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
Dem_Event_SingleStorage_ProcessPendingReset(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_SingleStorage_ResetPending
 *****************************************************************************/
/*!
 * \brief         Resets the Pending bit of an event and calculates the
 *                new event status
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current UDS status of the event
 *
 * \return        Modified UDS status of the event
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_SingleStorage_ResetPending(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif /* DEM_SINGLESTORAGE_INTERFACE_H_ */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_SingleStorage_Interface.h
 *********************************************************************************************************************/
