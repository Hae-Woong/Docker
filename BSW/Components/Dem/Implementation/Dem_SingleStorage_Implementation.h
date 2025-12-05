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
 *  \file       Dem_SingleStorage_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of SingleStorage unit which handles non-combined events or events supporting combination
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

#if !defined (DEM_SINGLESTORAGE_IMPLEMENTATION_H_)
#define DEM_SINGLESTORAGE_IMPLEMENTATION_H_

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
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_SingleStorage_Interface.h"
/* ------------------------------------------------------------------------- */
#include "Dem_CombinedStorage_Interface.h"
#include "Dem_UDSStatus_Interface.h"
#include "Dem_InternalStatus_Interface.h"
#include "Dem_DTC_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Satellite_Interface.h"
#include "Dem_Aging_Interface.h"

/* Parent Logical Unit will not be included after refactoring */
#include "Dem_EventIF_Interface.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

#define DEM_SINGLESTORAGE_IMPLEMENTATION_FILENAME "Dem_SingleStorage_Implementation.h"

/* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_SingleStorage_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Event_SingleStorage_TestWirStoredOnlySupported
 *****************************************************************************/
/*!
 * \brief         Tests whether the WIR bit can only be set for stored events.
 * 
 * \details       -
 * 
 * \return        TRUE
 *                WIR bit can only be set for stored events.
 * \return        FALSE
 *                WIR bit can be set for all confirmed events irrespective of
 *                the stored status.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_SingleStorage_TestWirStoredOnlySupported(
  void
);

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ****************************************************************************
 % Dem_Event_SingleStorage_TestWirStoredOnlySupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_SingleStorage_TestWirStoredOnlySupported(
  void
)
{
  return (DEM_CFG_PROCESS_WIR_STOREDONLY == STD_ON);
}


/*!
 * \}
 */

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_SingleStorage_Properties
 * \{
 */

/* ****************************************************************************
 % Dem_Event_SingleStorage_GetMaxFdcToc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_Event_SingleStorage_GetMaxFdcToc(
  Dem_EventIdType EventId
)
{
  return Dem_Satellite_GetMaxFaultDetectionCounter(
    Dem_Cfg_EventSatelliteId(EventId),
    Dem_Cfg_EventSatelliteEventId(EventId));
}

/* ****************************************************************************
 % Dem_Event_SingleStorage_GetHealingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_SingleStorage_GetHealingCounter(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  uint8 lHealingCounter = 0;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_HEALINGCOUNTER == STD_ON)
  lHealingCounter = Dem_Event_GetTripCount(EventId);
#endif

  return lHealingCounter;
}

/* ****************************************************************************
 % Dem_Event_SingleStorage_SetHealingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_SingleStorage_SetHealingCounter(                                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId,
  uint8 CounterValue
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CounterValue)                                                                                 /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_HEALINGCOUNTER == STD_ON)
    Dem_Event_SetTripCount(EventId, CounterValue);
#endif
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
 * \defgroup Dem_SingleStorage_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Event_SingleStorage_TestAgingAfterHealingRequired
 *****************************************************************************/
/*!
 * \brief         Tests whether aging after healing is required for an event.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Aging after healing is required for the event.
 * \return        FALSE
 *                Aging after healing is not required for the event.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_SingleStorage_TestAgingAfterHealingRequired(
  Dem_EventIdType EventId
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_SingleStorage_Private
 * \{
 */

/* ****************************************************************************
 % Dem_Event_SingleStorage_TestHealingPreventsAging
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
 *
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_SingleStorage_TestAgingAfterHealingRequired(
  Dem_EventIdType EventId
)
{
  boolean lAgingAfterHealing = FALSE;
  Dem_UDSStatus_StatusType lUdsStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);

  if (Dem_Event_UDSStatus_TestBit(lUdsStatus, Dem_UdsStatus_WIR) == TRUE)
  {
    lAgingAfterHealing = (((Dem_Core_DTC_IsObdRelated(EventId) == TRUE) && (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE))
      || (Dem_Core_Aging_SingleStorage_TestAgingAfterHealingAllEventsSupported() == TRUE)
      || (Dem_Core_Aging_SingleStorage_TestAgingAfterHealingSupported() == TRUE));
  }
  else if ((Dem_Event_UDSStatus_TestBit(lUdsStatus, Dem_UdsStatus_CDTC) == TRUE)
    && ((Dem_Core_Event_TestHasIndicator(EventId) == FALSE)
      || ((Dem_Event_SingleStorage_TestWirStoredOnlySupported() == TRUE)
        && (Dem_Event_InternalStatus_TestEventStored(EventId) == FALSE))))                                                       /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
  {
    lAgingAfterHealing = (((Dem_Core_DTC_IsObdRelated(EventId) == TRUE) && (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE))
      || (Dem_Core_Aging_SingleStorage_TestAgingAfterHealingAllEventsSupported() == TRUE));
  }
  else
  { /* MISRA case */
  }
  return lAgingAfterHealing;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

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
 * \addtogroup Dem_SingleStorage_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Event_SingleStorage_IncrementHealingCounter
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
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_SingleStorage_IncrementHealingCounter(
  Dem_EventIdType EventId
  )
{ 
  boolean lHealingTargetReached = FALSE;
#if (DEM_FEATURE_NEED_CUSTOMIZED_MEMORY_PROCESSING == STD_ON)
  if ((uint16)Dem_DebounceBaseSat_GetDebounceValueMax(EventId) < Dem_Cfg_EventPreconfirmedTarget(EventId))
#endif
  {
#if (DEM_FEATURE_NEED_HEALINGCOUNTER == STD_ON)
    if (Dem_Event_SingleStorage_GetHealingCounter(EventId) < 0xffU)
    {
      Dem_Event_SingleStorage_SetHealingCounter(EventId, (uint8)(Dem_Event_SingleStorage_GetHealingCounter(EventId) + 1u));
    }
    if (Dem_Event_SingleStorage_GetHealingCounter(EventId) >= Dem_Cfg_EventHealingTarget(EventId))
    {
      lHealingTargetReached = TRUE;
    }
#else
    if (Dem_Event_UDSStatus_TestBit(Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId), Dem_UdsStatus_WIR) == TRUE)
    { /* Healing target reached -> heal all related indicators */
      lHealingTargetReached = TRUE;
    }
#endif
  }
  return lHealingTargetReached;
}

/* ****************************************************************************
 % Dem_Event_SingleStorage_Healed
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
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_SingleStorage_Healed(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  )
{
  Dem_UDSStatus_StatusType lNewEventStatus = EventStatus;
  if (Dem_Event_UDSStatus_TestBit(EventStatus, Dem_UdsStatus_WIR) == TRUE)
  {
    lNewEventStatus = Dem_Event_UDSStatus_ResetBit(EventStatus, Dem_UdsStatus_WIR);
    Dem_DiagnosticObserver_ProcessEventHealed(EventId);
    if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
    { 
      /* only OBD related events supports the qualification of Confirmed and WIR bit */
      Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(EventId,
          Dem_Event_InternalStatus_QualifyStatus_ResetBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_WIR));
    }
  }

  return lNewEventStatus;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */


/* ****************************************************************************
 % Dem_Event_SingleStorage_ProcessHealing
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_SingleStorage_ProcessHealing(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  )
{
  Dem_UDSStatus_StatusType lNewEventStatus = EventStatus;
  boolean lHealingTargetReached = Dem_Event_SingleStorage_IncrementHealingCounter(EventId);
  if (lHealingTargetReached == TRUE)
  {
    lNewEventStatus = Dem_Event_SingleStorage_Healed(EventId, EventStatus);
  }

  return lNewEventStatus;
}

/* ****************************************************************************
 % Dem_Event_SingleStorage_TestHealingPreventsAging
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
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_SingleStorage_TestHealingPreventsAging(
  Dem_EventIdType EventId
  )
{
  boolean lAgingPrevented = FALSE;
  if ((Dem_Event_SingleStorage_TestAgingAfterHealingRequired(EventId) == TRUE) && (Dem_Cfg_EventHealingTarget(EventId) > 0u))
  {
    Dem_UDSStatus_StatusType lUdsStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
#  if (DEM_FEATURE_NEED_HEALINGCOUNTER == STD_ON)
    lAgingPrevented = (boolean)((Dem_Core_Event_GetHealingCounter(EventId) < Dem_Cfg_EventHealingTarget(EventId))
      || (Dem_Event_UDSStatus_TestBit(lUdsStatus, Dem_UdsStatus_PDTC) == TRUE));
#  else
    lAgingPrevented = (Dem_Event_UDSStatus_TestBit(lUdsStatus, Dem_UdsStatus_PDTC) == TRUE);
#  endif
  }

  return lAgingPrevented;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_SingleStorage_ProcessPendingReset
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
Dem_Event_SingleStorage_ProcessPendingReset(
  Dem_EventIdType EventId
  )
{
#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
  /* pending bit was set before and is now reset: reduce number of pending events in MIL group */
  if (Dem_Cfg_EventMilGroupIndex(EventId) != DEM_CFG_MIL_GROUP_INVALID)
  {
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if (Dem_Event_InternalStatus_GetMilGroupPendingCount(Dem_Cfg_EventMilGroupIndex(EventId)) <= 0u)
    {
      Dem_Error_RunTimeCheckFailed(DEM_EVENT_IMPLEMENTATION_FILENAME, __LINE__);                                                 /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    }
#endif
    Dem_Mem_DecrementMilGroupPendingCount(Dem_Cfg_EventMilGroupIndex(EventId));
  }
#endif
  Dem_DiagnosticObserver_ProcessEventPDTCDisabled(EventId);
  Dem_Mar_DecrementPendingEventsIfNeeded(EventId);
}

/* ****************************************************************************
 % Dem_Event_SingleStorage_ResetPending
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_SingleStorage_ResetPending(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  )
{
  Dem_UDSStatus_StatusType lNewEventStatus = EventStatus;

#if (DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON)
  Dem_Event_SetTripCount(EventId, 0);
#endif
  Dem_Event_SingleStorage_ProcessPendingReset(EventId);
  lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_PDTC);

  return lNewEventStatus;
}   

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_SINGLESTORAGE_IMPLEMENTATION_H_ */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_SingleStorage_Implementation.h
 *********************************************************************************************************************/
