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
 *  \file       Dem_CombinedStorage_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of CombinedStorage unit which handles events supporting combination on storage
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

#if !defined (DEM_COMBINEDSTORAGE_IMPLEMENTATION_H_)
#define DEM_COMBINEDSTORAGE_IMPLEMENTATION_H_

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

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_CombinedStorage_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_SingleStorage_Interface.h"
#include "Dem_InternalStatus_Interface.h"
#include "Dem_UDSStatus_Interface.h"
#include "Dem_DTC_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_Infrastructure_Interface.h"

/* Parent Logical Unit will not be included after refactoring */
#include "Dem_EventIF_Interface.h"

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

#define DEM_COMBINEDSTORAGE_IMPLEMENTATION_FILENAME "Dem_CombinedStorage_Implementation.h"

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
 * \defgroup Dem_CombinedStorage_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Event_CombinedStorage_SharedCounter_TestHealingConditionsFulfilled
 *****************************************************************************/
/*!
 * \brief         Tests whether the combination group fulfills the conditions
 *                to increment the common healing counter
 *
 * \details       -
 *
 * \param[in]     GroupId
 *                Combined group identifier
 *
 * \return        TRUE
 *                Healing conditions are fulfilled for the combination group
 * \return        FALSE
 *                Healing conditions are not fulfilled for the combination group
 *
 * \pre           GroupId is valid
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_TestHealingConditionsFulfilled(
  Dem_Cfg_CombinedGroupIndexType GroupId
  );


/* ****************************************************************************
 * Dem_Event_CombinedStorage_SharedCounter_TestHealingCounterIncrementedThisCycle
 *****************************************************************************/
/*!
 * \brief         Tests whether the shared healing counter is already incremented for
 *                the combination group in this cycle
 *
 * \details       -
 *
 * \param[in]     GroupId
 *                Combined group identifier
 *
 * \return        TRUE
 *                Shared Healing counter is already incremented for the combination
 *                group in this cycle
 *
 * \return        FALSE
 *                Shared Healing counter is not yet incremented for the combination
 *                group in this cycle
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_TestHealingCounterIncrementedThisCycle(
  Dem_Cfg_CombinedGroupIndexType GroupId
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_IncrementHealingCounter
 *****************************************************************************/
/*!
 * \brief         Increments the healing counter
 *
 * \details       Increments the healing counter of an event that supports
 *                combined storage and tests whether the healing target was
 *                reached
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_IncrementHealingCounter(
  Dem_EventIdType EventId
  );


/* ****************************************************************************
 * Dem_Event_CombinedStorage_SharedCounter_IncrementHealingCounter
 *****************************************************************************/
/*!
 * \brief         Increments the healing counter of the event
 *
 * \details       Increments the healing counter of an event which supports
 *                'Shared Counter Combination' in the current (PBS) variant
 *                and tests whether the healing target is reached
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_IncrementHealingCounter(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_SharedCounter_SetHealingCounterIncrementedThisCycle
 *****************************************************************************/
/*!
 * \brief         Mark the shared healing counter as already incremented for
 *                the combination group in this cycle
 *
 * \details       -
 *
 * \param[in]     GroupId
 *                Combined group identifier
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_SetHealingCounterIncrementedThisCycle(
  Dem_Cfg_CombinedGroupIndexType GroupId
  );

/* ****************************************************************************
 * Dem_Event_CombinedStorage_TestPendingResetConditionsFulfilled
 *****************************************************************************/
/*!
 * \brief         Test if the conditions for resetting the pending bit of the
 *                combined group are fulfilled
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                Pending bit should be reset
 * \return        FALSE
 *                Pending bit should not be reset
 * \pre           Shared Counter storage is supported for the event
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_TestPendingResetConditionsFulfilled(
  Dem_EventIdType EventId
  );


/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Event_CombinedStorage_SharedCounter_TestHealingConditionsFulfilled
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_TestHealingConditionsFulfilled(
  Dem_Cfg_CombinedGroupIndexType GroupId
  )
{
  boolean lConditionsFulfilled = FALSE;
  Dem_DTC_UDSStatusType lGroupUdsStatus = Dem_Event_UDSStatus_GetCombinedGroupStatus(GroupId);

  if ( (Dem_Event_UDSStatus_TestBit(lGroupUdsStatus, Dem_UdsStatus_TNCTOC) == FALSE)
    && (Dem_Event_UDSStatus_TestBit(lGroupUdsStatus, Dem_UdsStatus_TFTOC) == FALSE) )
  {
    lConditionsFulfilled = TRUE;
  }

  return lConditionsFulfilled;
}

/* ****************************************************************************
 % Dem_Event_CombinedStorage_SharedCounter_TestHealingCounterIncrementedThisCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_TestHealingCounterIncrementedThisCycle(
  Dem_Cfg_CombinedGroupIndexType GroupId
  )
{
  boolean lHealingCounterIncremented = FALSE;

#if (DEM_CFG_SUPPORT_SHAREDCOUNTER_COMBINATION == STD_ON)
  if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(Dem_Cfg_CombinedGroupGetEvent(GroupId)))               /* COV_DEM_ROBUSTNESS TX */
  {
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    /* Only valid pointer shall be forwarded to other function in safety context, */
    /* even if pointer is not used for write operation */
    if (Dem_Cfg_GetAddrSharedHealingCounterIncremented(0) == NULL_PTR)                                                           /* COV_DEM_ROBUSTNESS XF */
    {
      Dem_Error_RunTimeCheckFailed(DEM_COMBINEDSTORAGE_IMPLEMENTATION_FILENAME, __LINE__);                                       /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    }
    else
# endif
    {
      lHealingCounterIncremented = Dem_Com_TestBitInBitfield(
        Dem_Cfg_GetAddrSharedHealingCounterIncremented(0),                                                                       /* SBSW_DEM_POINTER_FORWARD_SHAREDHEALINGCOUNTERINCREMENTED */
        Dem_Cfg_GetSizeOfSharedHealingCounterIncremented(),
        (Dem_HandleType)GroupId);
    }
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId)

  return lHealingCounterIncremented;
}

/* ****************************************************************************
 % Dem_Event_CombinedStorage_IncrementHealingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_IncrementHealingCounter(
  Dem_EventIdType EventId
  )
{
  boolean lHealingTargetReached = FALSE;
  if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(EventId))
  {
    lHealingTargetReached = Dem_Event_CombinedStorage_SharedCounter_IncrementHealingCounter(EventId);
  }
  else
  {
    lHealingTargetReached = Dem_Event_SingleStorage_IncrementHealingCounter(EventId);
  }

  return lHealingTargetReached;
}

/* ****************************************************************************
 % Dem_Event_CombinedStorage_SharedCounter_IncrementHealingCounter
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
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_IncrementHealingCounter(
  Dem_EventIdType EventId
  )
{
  boolean lHealingTargetReached = FALSE;
  Dem_Cfg_CombinedGroupIndexType lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  Dem_EventIdType lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);

  if (Dem_Event_CombinedStorage_SharedCounter_TestHealingConditionsFulfilled(lGroupId) == TRUE)
  {
    if (Dem_Event_CombinedStorage_SharedCounter_TestHealingCounterIncrementedThisCycle(lGroupId) == FALSE)
    {
      lHealingTargetReached = Dem_Event_SingleStorage_IncrementHealingCounter(lMasterEventId);
      Dem_Event_CombinedStorage_SharedCounter_SetHealingCounterIncrementedThisCycle(lGroupId);
    }
    else
    {
      if (Dem_Event_CombinedStorage_SharedCounter_GetHealingCounter(lMasterEventId) >= Dem_Cfg_EventHealingTarget(EventId))
      {
        lHealingTargetReached = TRUE;
      }
    }
  }

  return lHealingTargetReached;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_CombinedStorage_SharedCounter_SetHealingCounterIncrementedThisCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_SetHealingCounterIncrementedThisCycle(
  Dem_Cfg_CombinedGroupIndexType GroupId
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId);

#if (DEM_CFG_SUPPORT_SHAREDCOUNTER_COMBINATION == STD_ON)
  if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(Dem_Cfg_CombinedGroupGetEvent(GroupId)))               /* COV_DEM_ROBUSTNESS TX */
  {
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if (Dem_Cfg_GetAddrSharedHealingCounterIncremented(0) == NULL_PTR)
    {
      Dem_Error_RunTimeCheckFailed(DEM_COMBINEDSTORAGE_IMPLEMENTATION_FILENAME, __LINE__);                                       /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    }
    else
# endif
    {
      Dem_Com_SyncSetBitInBitfield(
        Dem_Cfg_GetAddrSharedHealingCounterIncremented(0),                                                                       /* SBSW_DEM_POINTER_FORWARD_SHAREDHEALINGCOUNTERINCREMENTED */
        Dem_Cfg_GetSizeOfSharedHealingCounterIncremented(),
        (Dem_HandleType)GroupId);
    }
  }
#endif
}

/* ****************************************************************************
% Dem_Event_CombinedStorage_TestPendingResetConditionsFulfilled
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
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
    Dem_Event_CombinedStorage_TestPendingResetConditionsFulfilled(
        Dem_EventIdType EventId)
{
  boolean lConditionsFulfilled = FALSE;
  Dem_Cfg_CombinedGroupIndexType lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
  if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)                                                                                /* COV_DEM_ROBUSTNESS TX */
  {
    if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(EventId))
    {
      Dem_DTC_UDSStatusType lGroupUdsStatus = Dem_Event_UDSStatus_GetCombinedGroupStatus(lGroupId);
      
      if ( (Dem_Event_UDSStatus_TestBit(lGroupUdsStatus, Dem_UdsStatus_TNCTOC) == FALSE)
        && (Dem_Event_UDSStatus_TestBit(lGroupUdsStatus, Dem_UdsStatus_TFTOC) == FALSE) )
      {
        lConditionsFulfilled = TRUE;
      }
    }
    else
    {
      lConditionsFulfilled = TRUE;
    }
  }
  return lConditionsFulfilled;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_CombinedStorage_Properties
 * \{
 */

/* ****************************************************************************
 % Dem_Event_CombinedStorage_GetMaxFdcToc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_Event_CombinedStorage_GetMaxFdcToc(
  Dem_EventIdType EventId
)
{
  sint8 lTotalMaxFdcToc = 0;
  Dem_Cfg_CombinedGroupIterType lCombinedIter;

  for (Dem_Cfg_CombinedGroupIterInit(Dem_Cfg_EventCombinedGroup(EventId), &lCombinedIter);                                       /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                                     /* PRQA S 2994 */ /* MD_DEM_2994 */ /* SBSW_DEM_CALL_ITERATOR_POINTER */
    Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_EventIdType lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                  /* SBSW_DEM_CALL_ITERATOR_POINTER */
    if ((Dem_Cfg_EventAvailableByVariant(lSubEventId) == TRUE)
      && (Dem_Event_InternalStatus_TestBit(Dem_Event_InternalStatus_GetEventStatus(lSubEventId), Dem_InternalStatus_Disconnected) == FALSE))
    {
      sint8 lCurrentMaxFdcToc = Dem_Event_SingleStorage_GetMaxFdcToc(lSubEventId);
      if (lCurrentMaxFdcToc > lTotalMaxFdcToc)
      {
        lTotalMaxFdcToc = lCurrentMaxFdcToc;
      }
    }
  }
  return lTotalMaxFdcToc;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Event_CombinedStorage_IsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_IsSupported(
  void
  )
{
  return (boolean)(DEM_CFG_SUPPORT_TYPE1_COMBINATION == STD_ON);
}

/* ****************************************************************************
 % Dem_Event_CombinedStorage_IsSupportedByEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_IsSupportedByEvent(
  Dem_EventIdType EventId
  )
{
  return ( (Dem_Event_CombinedStorage_IsSupported() == TRUE)
        && (Dem_Cfg_EventCombinedGroup(EventId) != DEM_CFG_COMBINED_GROUP_INVALID) );
}


/* ****************************************************************************
 % Dem_Event_CombinedStorage_GetHealingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_CombinedStorage_GetHealingCounter(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventIdType EventId
  )
{
  uint8 lHealingCounter = 0;
  if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(EventId) == TRUE)
  {
    lHealingCounter = Dem_Event_CombinedStorage_SharedCounter_GetHealingCounter(EventId);
  }
  else
  {
    lHealingCounter = Dem_Event_SingleStorage_GetHealingCounter(EventId);
  }

  return lHealingCounter;
}

/* ****************************************************************************
 % Dem_Event_CombinedStorage_SetHealingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CombinedStorage_SetHealingCounter(                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId,
  uint8 CounterValue
  )
{
  if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(EventId) == TRUE)
  {
    Dem_Event_CombinedStorage_SharedCounter_SetHealingCounter(EventId, CounterValue);
  }
  else
  {
    Dem_Event_SingleStorage_SetHealingCounter(EventId, CounterValue);
  }
}


/* ****************************************************************************
 % Dem_Event_CombinedStorage_SharedCounter_IsSupportedInVariant
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_IsSupportedInVariant(
  void
  )
{
  return (boolean)( (DEM_CFG_SUPPORT_SHAREDCOUNTER_COMBINATION == STD_ON)
                 && (Dem_Cfg_IsObdOnUdsSupportedInVariant() == TRUE) );
}

/* ****************************************************************************
 % Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(
  Dem_EventIdType EventId
  )
{
  return ( Dem_Event_CombinedStorage_SharedCounter_IsSupportedInVariant()
        && Dem_Event_CombinedStorage_IsSupportedByEvent(EventId)
        && Dem_Core_DTC_IsObdRelated(EventId) );
}

/* ****************************************************************************
 % Dem_Event_CombinedStorage_SharedCounter_SetHealingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_SetHealingCounter(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId,
  uint8 CounterValue
  )
{
  if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(EventId) == TRUE)                                      /* COV_DEM_ROBUSTNESS TX */
  {
    Dem_EventIdType lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);
    Dem_Event_SingleStorage_SetHealingCounter(lMasterEventId, CounterValue);
  }
}


/* ****************************************************************************
 % Dem_Event_CombinedStorage_SharedCounter_GetHealingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_GetHealingCounter(                                                                       /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
  )
{
  uint8 lHealingCounter = 0;

  if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(EventId) == TRUE)                                      /* COV_DEM_ROBUSTNESS TX */
  {
    Dem_EventIdType lMasterEventId = Dem_Cfg_GetMasterEvent(EventId);
    lHealingCounter = Dem_Event_SingleStorage_GetHealingCounter(lMasterEventId);
  }

  return lHealingCounter;
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
 * \defgroup Dem_CombinedStorage_Private Private Methods
 * \{
 */

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
 * \addtogroup Dem_CombinedStorage_Private
 * \{
 */

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
 * \addtogroup Dem_CombinedStorage_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Event_CombinedStorage_ProcessHealing
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
Dem_Event_CombinedStorage_ProcessHealing(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  )
{
  boolean lHealingTargetReached = FALSE;
  Dem_UDSStatus_StatusType lNewEventStatus = EventStatus;

  lHealingTargetReached = Dem_Event_CombinedStorage_IncrementHealingCounter(EventId);

  if (lHealingTargetReached == TRUE)
  {
    lNewEventStatus = Dem_Event_CombinedStorage_Healed(EventId, EventStatus);
  }

  return lNewEventStatus;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Event_CombinedStorage_Healed
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
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_CombinedStorage_Healed(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  )
{
  Dem_UDSStatus_StatusType lNewEventStatus = EventStatus;

  if (Dem_Event_UDSStatus_TestBit(EventStatus, Dem_UdsStatus_WIR) == TRUE)
  {
    lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_WIR);
    Dem_DiagnosticObserver_ProcessEventHealed(EventId);

    if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
    { /* only OBD related events supports the qualification of Confirmed and WIR bit */
      Dem_Cfg_CombinedGroupIndexType lGroupId = Dem_Cfg_EventCombinedGroup(EventId);

      if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)                                                                            /* COV_DEM_ROBUSTNESS TX */
      {
        Dem_UDSStatus_StatusType lDtcStatus = lNewEventStatus;
        Dem_Cfg_CombinedGroupIterType lCombinedIter;
        Dem_EventIdType lSubEventId;
        for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lCombinedIter);                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_Cfg_CombinedGroupIterExists(&lCombinedIter) == TRUE;                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_Cfg_CombinedGroupIterNext(&lCombinedIter))                                                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
        {
          lSubEventId = Dem_Cfg_CombinedGroupIterGet(&lCombinedIter);                                                            /* SBSW_DEM_CALL_ITERATOR_POINTER */

          if ((lSubEventId != EventId)
            && (Dem_Event_TestEventAvailable(lSubEventId) == TRUE))                                                              /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
          {
            lDtcStatus = (Dem_DTC_UDSStatusType)(lDtcStatus | Dem_Event_UDSStatus_GetInternalEventUdsStatus(lSubEventId));
          }
        }

        if (Dem_Event_UDSStatus_TestBit(lDtcStatus, Dem_UdsStatus_WIR) == FALSE)
        {
          Dem_DTC_SetQualifyStatus(EventId,
            Dem_Event_InternalStatus_QualifyStatus_ResetBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_WIR));
        }
      }
    }
  }

  return lNewEventStatus;
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */


/* ****************************************************************************
 % Dem_Event_CombinedStorage_TestHealingPreventsAging
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Event_CombinedStorage_TestHealingPreventsAging(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
  )
{
  boolean lAgingPrevented = FALSE;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_NEED_HEALING == STD_ON) && (DEM_FEATURE_NEED_AGING_AFTER_HEALING == STD_ON)
  if (Dem_Event_CombinedStorage_IsSupportedByEvent(EventId) == TRUE)                                                             /* COV_DEM_ROBUSTNESS TX */
  {
    Dem_Cfg_CombinedGroupIndexType lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
    Dem_Cfg_CombinedGroupIterType lGroupIter;
    for (Dem_Cfg_CombinedGroupIterInit(lGroupId, &lGroupIter);                                                                   /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterExists(&lGroupIter) == TRUE;                                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
      Dem_Cfg_CombinedGroupIterNext(&lGroupIter))                                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
    {
      Dem_EventIdType lSubEvent = Dem_Cfg_CombinedGroupIterGet(&lGroupIter);                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
      if (Dem_Event_SingleStorage_TestHealingPreventsAging(lSubEvent) == TRUE)
      {
        lAgingPrevented = TRUE;
        break;
      }
    }
  }
#endif
  return lAgingPrevented;
}

/* ****************************************************************************
 % Dem_Event_CombinedStorage_SharedCounter_ResetAllHealingCounterIncrementedThisCycle
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_CombinedStorage_SharedCounter_ResetAllHealingCounterIncrementedThisCycle(
  void
  )
{
#if (DEM_CFG_SUPPORT_SHAREDCOUNTER_COMBINATION == STD_ON)
  if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedInVariant())
  {
    Dem_Cfg_SharedHealingCounterIncrementedIterType idx;
    for (idx = 0u; idx < Dem_Cfg_GetSizeOfSharedHealingCounterIncremented(); idx++) {
      Dem_Cfg_SetSharedHealingCounterIncremented(idx, 0);                                                                        /* SBSW_DEM_ARRAY_WRITE_SHAREDHEALINGCOUNTERINCREMENTED */
    }
  }
#endif
}

/* ****************************************************************************
 % Dem_Event_CombinedStorage_ResetPending
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
Dem_Event_CombinedStorage_ResetPending(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  )
{
  Dem_UDSStatus_StatusType lNewEventStatus = EventStatus;

  if (Dem_Event_CombinedStorage_TestPendingResetConditionsFulfilled(EventId) == TRUE)
  {
    if (Dem_Event_CombinedStorage_SharedCounter_IsSupportedByEventInVariant(EventId) == TRUE)
    {
      Dem_Cfg_CombinedGroupIndexType lGroupId = Dem_Cfg_EventCombinedGroup(EventId);
      if (lGroupId != DEM_CFG_COMBINED_GROUP_INVALID)                                                                            /* COV_DEM_ROBUSTNESS TX */
      {
        if (Dem_Event_CombinedStorage_SharedCounter_TestHealingCounterIncrementedThisCycle(lGroupId) == FALSE)
        {
          Dem_Event_CombinedStorage_SharedCounter_SetHealingCounter(EventId, 0u);
        }
      }
    }
#if (DEM_FEATURE_NEED_TRIPCOUNTER == STD_ON)
    else
    {
      Dem_Event_SetTripCount(EventId, 0);
    }
#endif
    Dem_Event_SingleStorage_ProcessPendingReset(EventId);
    lNewEventStatus = Dem_Event_UDSStatus_ResetBit(lNewEventStatus, Dem_UdsStatus_PDTC);
  }

  return lNewEventStatus;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_COMBINEDSTORAGE_IMPLEMENTATION_H_ */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_CombinedStorage_Implementation.h
 *********************************************************************************************************************/
