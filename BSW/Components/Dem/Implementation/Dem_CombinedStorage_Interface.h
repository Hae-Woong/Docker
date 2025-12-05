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
 *  \defgroup   Dem_CombinedStorage CombinedStorage
 *  \addtogroup Dem_CombinedStorage
 *  \{
 *  \file       Dem_CombinedStorage_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Public interface of CombinedStorage unit which handles events supporting combination on storage
 *  \unit       CombinedStorage
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_COMBINEDSTORAGE_INTERFACE_H_)
#define DEM_COMBINEDSTORAGE_INTERFACE_H_

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_CombinedStorage_Types.h"
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_CombinedStorage_Properties Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Event_CombinedStorage_GetMaxFdcToc
 *****************************************************************************/
 /*!
 * \brief         Returns the max fault detection counter this operation cycle
 *                of the combined group of the event.
 * 
 * \details       Returns the max fault detection counter this operation cycle
 *                of the combined group of the event. It is calculated by taking
 *                the biggest MaxFdcToc value of any of the subevents.
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Max fault detection counter this operation of the combined group.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_Event_CombinedStorage_GetMaxFdcToc(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Event_CombinedStorage_IsSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether event combination on storage is supported.
 *
 * \details       -
 *
 * \return        TRUE
 *                Event combination on storage is supported
 * \return        FALSE
 *                Event combination on storage is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_IsSupported(
  void
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_IsSupportedByEvent
 *****************************************************************************/
/*!
 * \brief         Tests whether the event supports combined storage.
 *
 * \details       Tests whether the feature event combination on storage is
 *                supported and the given event is part of a combined group.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Event combination on storage is supported and the event belongs
 *                to a combined group.
 * \return        FALSE
 *                Event combination on storage is not supported or the event does
 *                not belong to a combined group.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_IsSupportedByEvent(
  Dem_EventIdType EventId
  );


/* ****************************************************************************
 * Dem_Event_CombinedStorage_GetHealingCounter
 *****************************************************************************/
 /*!
 * \brief         Gets the healing counter.
 * 
 * \details       Gets the healing counter of an event which supports combined
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
Dem_Event_CombinedStorage_GetHealingCounter(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_SetHealingCounter
 *****************************************************************************/
 /*!
 * \brief         Sets the healing counter.
 * 
 * \details       Sets the healing counter of an event which supports combined
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
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CombinedStorage_SetHealingCounter(
  Dem_EventIdType EventId,
  uint8 CounterValue
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_SharedCounter_IsSupportedInVariant
 *****************************************************************************/
/*!
 * \brief         Test whether Shared Counter Combination is supported 
 *                in this (PBS) variant.
 *
 * \details       -
 *
 * \return        TRUE
 *                'Shared Counter Combination' is supported in current (PBS)
 *                variant.
 * \return        FALSE
 *                'Shared Counter Combination' is not supported in current
 *                (PBS) variant.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_IsSupportedInVariant(
  void
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant
 *****************************************************************************/
/*!
 * \brief         Tests whether the event supports 'Shared Counter Combination'
 *                in this (PBS) variant.
 *
 * \details       Tests if the feature 'Shared Counter Combination' is supported
 *                in this (PBS) variant, the event supports combined storage and
 *                is OBD related.
 *
 * \return        TRUE
 *                The event supports 'Shared Counter Combination' in the current
 *                (PBS) variant.
 * \return        FALSE
 *                The feature 'Shared Counter Combination' is not supported in 
 *                the current (PBS) variant or the event does not support combined
 *                storage or the event is not OBD related.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_SharedCounter_SetHealingCounter
 *****************************************************************************/
/*!
 * \brief         Sets the shared healing counter.
 * 
 * \details       Sets the shared healing counter that belongs to the event's
 *                combination group.
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     CounterValue
 *                New value of the shared healing counter.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_SetHealingCounter(
  Dem_EventIdType EventId,
  uint8 CounterValue
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_SharedCounter_GetHealingCounter
 *****************************************************************************/
 /*!
 * \brief         Gets the shared healing counter.
 * 
 * \details       Gets the shared healing counter that belongs to the event's
 *                combination group.
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Shared healing counter value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_GetHealingCounter(
  Dem_EventIdType EventId
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
 * \defgroup Dem_CombinedStorage_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Event_CombinedStorage_ProcessHealing
 *****************************************************************************/
/*!
 * \brief         Processes healing for an event which supports combined storage.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current UDS status of the event.
 *
 * \return        Modified UDS status of the event.
 *
 * \pre           Combined storage is supported for the event
 *
 * \return        Modified UDS status of event
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_CombinedStorage_ProcessHealing(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_Healed
 *****************************************************************************/
/*!
 * \brief         Process that an event which supports combined storage healed.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current UDS status of the event.
 *
 * \return        Modified UDS status of the event.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_CombinedStorage_Healed(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_TestHealingPreventsAging
 *****************************************************************************/
/*!
 * \brief         Tests whether the healing process currently prevents the start
 *                of aging.
 *
  * \details      Tests whether the healing process currently prevents the start
  *               of aging. Depending on the configuration aging might be allowed
  *               during the healing process or not.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The event may not age
 * \return        FALSE
 *                The event may age
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_TestHealingPreventsAging(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_SharedCounter_ResetAllHealingCounterIncrementedThisCycle
 *****************************************************************************/
/*!
 * \brief         Reset all flags of the 'Shared Healing Counter Incremented'
 *                array.
 *
 * \details       -
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_ResetAllHealingCounterIncrementedThisCycle(
  void
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_ResetPending
 *****************************************************************************/
/*!
 * \brief         Processes reset of Pending bit for an event which supports
 *                combined storage.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[in]     EventStatus
 *                Current UDS status of the event.
 *
 * \return        Modified UDS status of the event.
 *
 * \pre           Combined storage is supported for the event
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_CombinedStorage_ResetPending(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif /* DEM_COMBINEDSTORAGE_INTERFACE_H_ */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_CombinedStorage_Interface.h
 *********************************************************************************************************************/
