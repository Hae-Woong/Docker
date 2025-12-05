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
 *  \defgroup   Dem_CoreIF CoreIF
 *  \addtogroup Dem_CoreIF
 *  \{
 *  \file       Dem_CoreIF_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Public interface implementation of the logical unit Core
 *  \unit       CoreIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_COREIF_IMPLEMENTATION_H_)
#define DEM_COREIF_IMPLEMENTATION_H_

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
#include "Dem_CoreIF_Interface.h"
/* ------------------------------------------------------------------------- */
#include "Dem_Event_Interface.h"
#include "Dem_DTC_Interface.h"


/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

#define DEM_COREIF_IMPLEMENTATION_FILENAME "Dem_CoreIF_Implementation.h"

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
 * \defgroup Dem_CoreIF_PrivateProperties Private Properties
 * \{
 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

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
 * \addtogroup Dem_CoreIF_Properties
 * \{
 */

/* ****************************************************************************
 % Dem_Core_GetMaxFdcToc
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_Core_GetMaxFdcToc(                                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
  )
{
  return Dem_Core_Event_GetMaxFdcToc(EventId);
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
 * \defgroup Dem_CoreIF_Private Private Methods
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
 * \addtogroup Dem_CoreIF_Private
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
 * \addtogroup Dem_CoreIF_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Core_SetMonitorDisabledTOC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_SetMonitorDisabledTOC(                                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
)
{
  Dem_ExtendedStatus_StatusType lStatus = Dem_Event_ExtendedStatus_GetEventStatus(EventId);
  lStatus = Dem_Event_ExtendedStatus_SetBit(lStatus, Dem_ExtendedStatus_MonitorDisabledTOC);
  Dem_Event_ExtendedStatus_SetEventStatus(EventId, lStatus);
}

/* ****************************************************************************
 % Dem_Core_ResetMonitorDisabledTOC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_ResetMonitorDisabledTOC(
  Dem_EventIdType EventId
)
{
  Dem_ExtendedStatus_StatusType lStatus = Dem_Event_ExtendedStatus_GetEventStatus(EventId);
  lStatus = Dem_Event_ExtendedStatus_ResetBit(lStatus, Dem_ExtendedStatus_MonitorDisabledTOC);
  Dem_Event_ExtendedStatus_SetEventStatus(EventId, lStatus);
}

/* ****************************************************************************
 % Dem_Core_TestEventExtendedStatusBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestEventExtendedStatusBit(
  Dem_EventIdType EventId,
  Dem_ExtendedStatus_ExtendedStatusBitsType StatusBit
)
{
  Dem_UDSStatus_StatusType lStatus = Dem_Event_ExtendedStatus_GetEventStatus(EventId);
  return Dem_Event_ExtendedStatus_TestBit(lStatus, StatusBit);
}

/* ****************************************************************************
 % Dem_Core_TestInternalEventUdsStatusBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestInternalEventUdsStatusBit(
  Dem_EventIdType EventId,
  Dem_UDSStatus_UDSStatusBitsType StatusBit
)
{
  Dem_UDSStatus_StatusType lStatus = Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId);
  return Dem_Event_UDSStatus_TestBit(lStatus, StatusBit);
}

/* ****************************************************************************
 % Dem_Core_TestExternalEventUdsStatusBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestExternalEventUdsStatusBit(
  Dem_EventIdType EventId,
  Dem_UDSStatus_UDSStatusBitsType StatusBit
)
{
  Dem_UDSStatus_StatusType lStatus = Dem_Event_UDSStatus_GetExternalUDSStatus(EventId, Dem_Event_UDSStatus_GetInternalEventUdsStatus(EventId));
  return Dem_Event_UDSStatus_TestBit(lStatus, StatusBit);
}

/* ****************************************************************************
 % Dem_Core_TestInternalDTCUdsStatusBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestInternalDTCUdsStatusBit(
  Dem_EventIdType EventId,
  Dem_UDSStatus_UDSStatusBitsType StatusBit
)
{
  Dem_DTC_UDSStatusType lInternalDtcUdsStatus = Dem_DTC_GetInternaUDSStatus(EventId);
  return Dem_Event_UDSStatus_TestBit(lInternalDtcUdsStatus, StatusBit);
}

/* ****************************************************************************
 % Dem_Core_TestExternalDTCUdsStatusBit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestExternalDTCUdsStatusBit(
  Dem_EventIdType EventId,
  Dem_UDSStatus_UDSStatusBitsType StatusBit
)
{
  Dem_DTC_UDSStatusType lExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(EventId);
  return Dem_Event_UDSStatus_TestBit(lExternalDtcUdsStatus, StatusBit);
}

/* ****************************************************************************
 % Dem_Core_TestCustomTriggeredSrecConfigured
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestCustomTriggeredSrecConfigured(
  Dem_EventIdType EventId
)
{
  return Dem_Core_Event_IsCustomTriggeredSrecConfigured(EventId);
}

/* ****************************************************************************
 % Dem_Core_TestCustomTriggeredTssrConfigured
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestCustomTriggeredTssrConfigured(
  Dem_EventIdType EventId
)
{
  return Dem_Core_Event_IsCustomTriggeredTssrConfigured(EventId);
}

/* ****************************************************************************
 % Dem_Core_TestCustomTriggerConfigured
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestCustomTriggerConfigured(
  Dem_EventIdType EventId
)
{
  return Dem_Core_TestCustomTriggeredSrecConfigured(EventId) || Dem_Core_TestCustomTriggeredTssrConfigured(EventId);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif /* DEM_COREIF_IMPLEMENTATION_H_ */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_CoreIF_Implementation.h
 *********************************************************************************************************************/
