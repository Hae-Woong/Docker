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
/*! \addtogroup Dem_UDSStatus
 *  \{
 *  \file       Dem_UDSStatus_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of UDSStatus subcomponent
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

#if !defined (DEM_UDSSTATUS_IMPLEMENTATION_H)
#define DEM_UDSSTATUS_IMPLEMENTATION_H

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
#include "Dem_UDSStatus_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_InternalStatus_Interface.h"
#include "Dem_DTC_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_UDSSTATUS_IMPLEMENTATION_FILENAME "Dem_UDSStatus_Implementation.h"

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
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_UDSStatus_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Event_UDSStatus_SetCombinedGroupStatus
 *****************************************************************************/
/*!
 * \brief         Set current UDS status of the combined group
 *
 * \details       -
 *
 * \param[in]     GroupId
 *                Unique handle of the combined group
 * \param[in]     Status
 *                New UDS status of the combined group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_UDSStatus_SetCombinedGroupStatus(
  Dem_Cfg_CombinedGroupIndexType GroupId,
  Dem_UDSStatus_StatusType Status
  );

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Event_UDSStatus_SetCombinedGroupStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_UDSStatus_SetCombinedGroupStatus(                                                                                      /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH  */
  Dem_Cfg_CombinedGroupIndexType GroupId,
  Dem_UDSStatus_StatusType Status
  )
{
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (GroupId >= Dem_Cfg_GetSizeOfCombinedGroupStatus())
  {
    Dem_Error_RunTimeCheckFailed(DEM_UDSSTATUS_IMPLEMENTATION_FILENAME, __LINE__);                                               /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_SetCombinedGroupStatus(GroupId, Status);                                                                             /* SBSW_DEM_ARRAY_WRITE_COMBINEDGROUPSTATUS */
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId);
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(Status);
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
 * \addtogroup Dem_UDSStatus_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_Event_UDSStatus_SetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_UDSStatus_SetBit(
  Dem_UDSStatus_StatusType UDSStatus,
  Dem_UDSStatus_UDSStatusBitsType BitIndex
  )
{
  return (uint8)Dem_Infrastructure_Utility_SetBitAtIndex(UDSStatus, (uint8)BitIndex, TRUE);
}

/* ****************************************************************************
 % Dem_Event_UDSStatus_ResetBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_UDSStatus_ResetBit(
  Dem_UDSStatus_StatusType UDSStatus,
  Dem_UDSStatus_UDSStatusBitsType BitIndex
  )
{
  return (Dem_UDSStatus_StatusType)Dem_Infrastructure_Utility_SetBitAtIndex(UDSStatus, (uint8)BitIndex, FALSE);
}

/* ****************************************************************************
 % Dem_Event_UDSStatus_TestBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_UDSStatus_TestBit(
  Dem_UDSStatus_StatusType UDSStatus,
  Dem_UDSStatus_UDSStatusBitsType BitIndex
  )
{
  return Dem_Infrastructure_Utility_TestBitAtIndex(UDSStatus, (uint8)BitIndex);
}

/* ****************************************************************************
 % Dem_Event_UDSStatus_GetInternalEventUdsStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_UDSStatus_GetInternalEventUdsStatus(
  Dem_EventIdType EventId
  )
{
  return Dem_Cfg_GetStatusData().EventStatus[EventId];                                                                           /* PRQA S 2842, 2843 */ /* MD_DEM_Dir4.1_ReadOperation, MD_DEM_Dir4.1_ReadOperation */
}

/* ****************************************************************************
 % Dem_Event_UDSStatus_SetInternalEventUdsStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_UDSStatus_SetInternalEventUdsStatus(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType Status
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EventId >= Dem_Cfg_EventCountOfStatusBlock())
  {
    Dem_Error_RunTimeCheckFailed(DEM_UDSSTATUS_IMPLEMENTATION_FILENAME, __LINE__);                                               /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    Dem_Cfg_GetStatusData().EventStatus[EventId] = Status;                                                                       /* PRQA S 2842 */ /* MD_DEM_Dir4.1_RunTimeCheckWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_STATUSDATA_EVENTSTATUS */
  }
}


/* ****************************************************************************
 % Dem_Event_UDSStatus_GetCombinedGroupStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_UDSStatus_GetCombinedGroupStatus(                                                                                      /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH  */
  Dem_Cfg_CombinedGroupIndexType GroupId
  )
{
  Dem_UDSStatus_StatusType lStatus = 0u;
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  lStatus = Dem_Cfg_GetCombinedGroupStatus(GroupId);                                                                             /* PRQA S 2842 */ /* MD_DEM_Dir4.1_ReadOperation */
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId);
  return lStatus;
}


/* ****************************************************************************
 % Dem_Event_UDSStatus_CalculateAndUpdateCombinedGroupStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_UDSStatus_CalculateAndUpdateCombinedGroupStatus(                                                                       /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH  */
  Dem_Cfg_CombinedGroupIndexType GroupId
  )
{
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
  Dem_DTC_UDSStatusType lGroupUdsStatus = Dem_DTC_CalculateCombinedGroupUdsStatus(GroupId);
  Dem_Event_UDSStatus_SetCombinedGroupStatus(GroupId, lGroupUdsStatus);
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(GroupId);
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
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_UDSStatus_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Event_UDSStatus_GetExternalUDSStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_UDSStatus_StatusType, DEM_CODE)
Dem_Event_UDSStatus_GetExternalUDSStatus(
  Dem_EventIdType EventId,
  Dem_UDSStatus_StatusType EventStatus
  )
{
  Dem_UDSStatus_StatusType lMaskedEventStatus;

  if(Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
  {
    /* suppress not qualified status bits */
    lMaskedEventStatus = (Dem_UDSStatus_StatusType)(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId) & EventStatus);
  }
  else
  {
    lMaskedEventStatus = EventStatus;
  }
  return lMaskedEventStatus;
}


/* ****************************************************************************
 % Dem_Event_UDSStatus_IsNvRelevantStatusChange
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_UDSStatus_IsNvRelevantStatusChange(
  Dem_UDSStatus_StatusType OldEventStatus,
  Dem_UDSStatus_StatusType NewEventStatus
  )
{
  boolean returnValue;

#if (DEM_CFG_USE_NVM == STD_OFF)
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(OldEventStatus)                                                                               /* PRQA S 3112 */ /* MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(NewEventStatus)                                                                               /* PRQA S 3112 */ /* MD_DEM_14.2 */

  returnValue = FALSE;
#elif (DEM_CFG_SUPPORT_TESTFAILED_STORAGE == STD_ON)
  returnValue = OldEventStatus != NewEventStatus;
#else
   /* True, if: Any bit is different (TF is ignored) */
   returnValue = Dem_Event_UDSStatus_ResetBit((OldEventStatus ^ NewEventStatus), Dem_UdsStatus_TF) != 0u;
#endif

  return returnValue;
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* DEM_UDSSTATUS_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_UDSStatus_Implementation.h
 *********************************************************************************************************************/
