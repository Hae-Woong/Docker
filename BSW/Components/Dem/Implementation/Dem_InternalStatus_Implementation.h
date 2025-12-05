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
/*! \addtogroup Dem_EventInternalStatus
 *  \{
 *  \file       Dem_InternalStatus_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of EventInternalStatus subcomponent
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

#if !defined (DEM_EVENTINTERNALSTATUS_IMPLEMENTATION_H)
#define DEM_EVENTINTERNALSTATUS_IMPLEMENTATION_H

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
#include "Dem_InternalStatus_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_UDSStatus_Interface.h"
#include "Dem_Infrastructure_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_EVENTINTERNALSTATUS_IMPLEMENTATION_FILENAME "Dem_InternalStatus_Implementation.h"

#define DEM_DTCINTERNALSTATUS_STORED_STATUS                    DEM_NBIT     (2u, 0u)  /*!< Bit mask for stored status bit(s) */
#define DEM_DTCINTERNALSTATUS_STORED_STATUS_MASK               DEM_NBIT_MASK(2u, 0u, DEM_EVENT_INTERNAL_STATUS_BYTESIZE)  /*!< Negated bit mask for stored status bit(s) */


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
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_InternalStatus_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Event_InternalStatus_GetMilGroupTripCountRaw
 *****************************************************************************/
/*!
 * \brief         Get the trip count of the MIL group
 *
 * \details       Get the trip count of the MIL group. There is additional
 *                information encoded in the value, so it cannot be used
 *                directly
 *
 * \param[in]     MilGroupIndex
 *                Unique handle of the MIL group
 *
 * \return        Trip count of the MIL group (raw)
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_MIL_GROUPS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_InternalStatus_GetMilGroupTripCountRaw(
  uint16 MilGroupIndex
  );

/* ****************************************************************************
 * Dem_Event_InternalStatus_SetMilGroupTripCount
 *****************************************************************************/
/*!
 * \brief         Set the trip count of the MIL group
 *
 * \details       Set the trip count of the MIL group.
 *
 * \param[in]     MilGroupIndex
 *                Unique handle of the MIL group
 * \param[in]     TripCount
 *                New trip count
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_MIL_GROUPS == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_SetMilGroupTripCount(
  uint16 MilGroupIndex,
  uint8 TripCount
  );

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Event_InternalStatus_GetMilGroupTripCountRaw
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Event_InternalStatus_GetMilGroupTripCountRaw(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint16 MilGroupIndex
  )
{
  uint8 lTripCount = 0u;
#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
  lTripCount = Dem_Cfg_StatusData.MilGroupTripCount[MilGroupIndex];                                                              /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(MilGroupIndex);
  return lTripCount;
}


/* ****************************************************************************
 % Dem_Event_InternalStatus_SetMilGroupTripCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_SetMilGroupTripCount(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint16 MilGroupIndex,
  uint8 TripCount
  )
{
#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (MilGroupIndex >= Dem_Cfg_GlobalMilGroupCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENTINTERNALSTATUS_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_StatusData.MilGroupTripCount[MilGroupIndex] = TripCount;                                                             /* SBSW_DEM_ARRAY_WRITE_STATUSDATA_MILGROUPTRIPCOUNT */
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(MilGroupIndex);
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TripCount);
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_InternalStatus_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Event_InternalStatus_TestEventStored
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_InternalStatus_TestEventStored(
  Dem_EventIdType EventId
)
{
  return (Dem_Event_InternalStatus_GetStoredStatus(EventId) != Dem_InternalStatus_StoredStatus_None);
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_GetStoredStatus
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
 */
DEM_LOCAL_INLINE FUNC(Dem_InternalStatus_StoredStatusType, DEM_CODE)
Dem_Event_InternalStatus_GetStoredStatus(
  Dem_EventIdType EventId
  )
{
  Dem_EventIdType lMemoryRepresentativeId = Dem_Cfg_GetMemoryRepresentative(EventId);
  uint8 lStoredStatus = (Dem_Event_InternalStatus_GetEventStatus(lMemoryRepresentativeId) & DEM_DTCINTERNALSTATUS_STORED_STATUS);
  return (Dem_InternalStatus_StoredStatusType)(lStoredStatus);                                                                   /* PRQA S 4342 */ /* MD_DEM_Rule10.5_Enum */
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_SetStoredStatus
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_SetStoredStatus(
  Dem_EventIdType EventId,
  Dem_InternalStatus_StoredStatusType StoredStatus
  )
{
  Dem_EventIdType lMemoryRepresentativeId = Dem_Cfg_GetMemoryRepresentative(EventId);
  Dem_Event_InternalStatusType lInternalStatus = Dem_Event_InternalStatus_GetEventStatus(lMemoryRepresentativeId);
  lInternalStatus = (lInternalStatus & DEM_DTCINTERNALSTATUS_STORED_STATUS_MASK) | ((Dem_Event_InternalStatusType)StoredStatus & DEM_DTCINTERNALSTATUS_STORED_STATUS);
  Dem_Event_InternalStatus_SetEventStatus(lMemoryRepresentativeId, lInternalStatus);
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_GetCombinedGroupStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Event_InternalStatusType, DEM_CODE)
Dem_Event_InternalStatus_GetCombinedGroupStatus(
  Dem_Cfg_CombinedGroupIndexType GroupId
)
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  return Dem_Cfg_GetCombinedGroupInternalStatus(GroupId);                                                                        /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ReadOperation */
#else
  return 0;
#endif
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_SetCombinedGroupStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_SetCombinedGroupStatus(                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_CombinedGroupIndexType GroupId,
  Dem_Event_InternalStatusType Status
  )
{
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (GroupId >= Dem_Cfg_GetSizeOfCombinedGroupInternalStatus())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENTINTERNALSTATUS_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_SetCombinedGroupInternalStatus(GroupId, Status);                                                                     /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_COMBINEDGROUPINTERNALSTATUS */
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId);
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Status);
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_GetEventStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Event_InternalStatusType, DEM_CODE)
Dem_Event_InternalStatus_GetEventStatus(
  Dem_EventIdType EventId
  )
{
  return Dem_Cfg_GetEventInternalStatus(EventId);                                                                                /* PRQA S 2842, 2843 */ /* MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation */
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_SetEventStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_SetEventStatus(
   Dem_EventIdType EventId,
   Dem_Event_InternalStatusType Status
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EventId >= Dem_Cfg_GetSizeOfEventInternalStatus())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENTINTERNALSTATUS_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    Dem_Cfg_SetEventInternalStatus(EventId, Status);                                                                             /* PRQA S 2841, 2842 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation, MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_EVENTINTERNALSTATUS */
  }
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_SetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Event_InternalStatusType, DEM_CODE)
Dem_Event_InternalStatus_SetBit(
  Dem_Event_InternalStatusType InternalStatus,
  Dem_InternalStatus_InternalStatusBitsType BitIndex
  )
{
  return (Dem_Event_InternalStatusType)Dem_Infrastructure_Utility_SetBitAtIndex(InternalStatus, (uint8)BitIndex, TRUE);
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_ResetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Event_InternalStatusType, DEM_CODE)
Dem_Event_InternalStatus_ResetBit(
  Dem_Event_InternalStatusType InternalStatus,
  Dem_InternalStatus_InternalStatusBitsType BitIndex
  )
{
  return (Dem_Event_InternalStatusType)Dem_Infrastructure_Utility_SetBitAtIndex(InternalStatus, (uint8)BitIndex, FALSE);
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_TestBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_InternalStatus_TestBit(
  Dem_Event_InternalStatusType InternalStatus,
  Dem_InternalStatus_InternalStatusBitsType BitIndex
  )
{
  return Dem_Infrastructure_Utility_TestBitAtIndex(InternalStatus, (uint8)BitIndex);
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_QualifyStatus_GetEventStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(
  Dem_EventIdType  EventId
  )
{
#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
# if (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
  return Dem_Cfg_GetStatusData().State[EventId];                                                                                 /* PRQA S 2842, 2843 */ /* MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation */
# else
  return Dem_Cfg_GetEventCycleStatus(EventId);                                                                                   /* PRQA S 2842, 2843 */ /* MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation */
# endif
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return 0u;
#endif
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_QualifyStatus_SetEventStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_QualifyStatus_SetEventStatus(
  Dem_EventIdType  EventId,
  Dem_UDSStatus_StatusType  Status
  )
{
#if (DEM_FEATURE_NEED_EMISSIONOBD == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EventId >= Dem_Cfg_EventCycleStatusCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENTINTERNALSTATUS_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
# if (DEM_CFG_PROCESS_CDTC_ALL_DTC == STD_ON)
    Dem_Cfg_GetStatusData().State[EventId] = Status;                                                                             /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_STATUSDATA_STATE */
# else
    Dem_Cfg_SetEventCycleStatus(EventId, Status);                                                                                /* SBSW_DEM_ARRAY_WRITE_EVENTCYCLESTATUS */
# endif
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EventId)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Status)                                                                                       /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_QualifyStatus_SetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InternalStatus_QualifyStatus_SetBit(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_UDSStatus_StatusType QualifyStatus,
  Dem_InternalStatus_QualifyStatusBitsType BitIndex
  )
{
  return (Dem_UDSStatus_StatusType)Dem_Infrastructure_Utility_SetBitAtIndex(QualifyStatus, (uint8)BitIndex, TRUE);
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_QualifyStatus_ResetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_InternalStatus_QualifyStatus_ResetBit(
  Dem_UDSStatus_StatusType QualifyStatus,
  Dem_InternalStatus_QualifyStatusBitsType BitIndex
  )
{
  return (Dem_UDSStatus_StatusType)Dem_Infrastructure_Utility_SetBitAtIndex(QualifyStatus, (uint8)BitIndex, FALSE);
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_QualifyStatus_TestBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_InternalStatus_QualifyStatus_TestBit(
  Dem_UDSStatus_StatusType QualifyStatus,
  Dem_InternalStatus_QualifyStatusBitsType BitIndex
  )
{
  return Dem_Infrastructure_Utility_TestBitAtIndex(QualifyStatus, (uint8)BitIndex);
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_GetMilGroupPendingCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Event_InternalStatus_GetMilGroupPendingCount(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint16 MilGroupIndex
  )
{
  uint16 lPendingCount = 0u;
#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
  lPendingCount = Dem_Cfg_StatusData.MilGroupActiveEvents[MilGroupIndex];
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(MilGroupIndex);
  return lPendingCount;
}

/* ****************************************************************************
 % Dem_Event_InternalStatus_SetMilGroupPendingCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_InternalStatus_SetMilGroupPendingCount(                                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint16 MilGroupIndex,
  uint16 PendingCount
  )
{
#if (DEM_FEATURE_NEED_MIL_GROUPS == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if ( (MilGroupIndex >= Dem_Cfg_GlobalMilGroupCount())
    || (PendingCount > 0xffU) )
  {
    Dem_Error_RunTimeCheckFailed(DEM_EVENTINTERNALSTATUS_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_StatusData.MilGroupActiveEvents[MilGroupIndex] = (uint8)PendingCount;                                                /* PRQA S 2841 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */  /* SBSW_DEM_ARRAY_WRITE_STATUSDATA_MILGROUPACTIVEEVENTS */
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(MilGroupIndex);
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(PendingCount);
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/

#endif /* DEM_EVENTINTERNALSTATUS_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_InternalStatus_Implementation.h
 *********************************************************************************************************************/
