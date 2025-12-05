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
/*! \addtogroup Dem_EventMonitor
 *  \{
 *  \file       Dem_EventMonitor_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Manages completition status of readiness group based on disabled event monitor related Handling.
 *  \entity     EventMonitor
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EVENTMONITOR_IMPLEMENTATION_H)
#define DEM_EVENTMONITOR_IMPLEMENTATION_H

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
/* PRQA S 3415 EOF */ /* MD_DEM_13.5_cf */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own unit header */
/* ------------------------------------------------------------------------- */
#include "Dem_EventMonitor_Interface.h"

                                                    /* Used unit API */
/* ------------------------------------------------------------------------- */
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_GlobalDiagnosticsIF_Interface.h"
#include "Dem_Readiness_Interface.h"
#include "Dem_FaultMemory_Interface.h"

                                                   /* unit callbacks */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"


/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_EVENTMONITOR_IMPLEMENTATION_FILENAME "Dem_EventMonitor_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_EventMonitor_PrivateProperties Private Properties
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
 * \addtogroup Dem_EventMonitor_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EventMonitor_IsDisableSupported
 *****************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EventMonitor_IsDisableSupported(
  void
)
{
  return (boolean)(DEM_FEATURE_NEED_DISABLE_EVENT_MONITOR == STD_ON);
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
 * \defgroup Dem_EventMonitor_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EventMonitor_SetDisabled
 *****************************************************************************/
/*!
 * \brief         Set disabled event monitor this operation cycle
 *
 * \details       Sets disabled event monitor this operation cycle bit and
 *                triggers a recalculation of affected readiness group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EventMonitor_SetDisabled(
  Dem_EventIdType EventId
  );

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EventMonitor_SetDisabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EventMonitor_SetDisabled(
  Dem_EventIdType EventId
)
{
  Dem_Core_SetMonitorDisabledTOC(EventId);
  /* Trigger recalculation of readiness group */
  Dem_Readiness_Completion_TriggerCompletionRecalculation(Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId));
}


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
 * \addtogroup Dem_EventMonitor_Public
 * \{
 */

/* ****************************************************************************
  % Dem_GlobalDiagnostics_EventMonitor_ResetAll
  *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EventMonitor_ResetAll(
  void)
{
  /* Check if EventMonitorDisabled is supported */
  if (Dem_GlobalDiagnostics_EventMonitor_IsDisableSupported())
  {
    /* Iterate through events and reset Completion state when
        EventMonitorDisable is set for any of the events in readiness group */
    Dem_EventIdType lEventId;

    for (lEventId = Dem_FaultMemory_Memories_GetFirstEvent(DEM_CFG_MEMORYID_PRIMARY);
      lEventId <= Dem_FaultMemory_Memories_GetLastEvent(DEM_CFG_MEMORYID_PRIMARY);
      ++lEventId)
    {
      Dem_GlobalDiagnostics_EventMonitor_Reset(lEventId);
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EventMonitor_Reset
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EventMonitor_Reset(
  Dem_EventIdType EventId
)
{
  if (Dem_Core_TestEventExtendedStatusBit(EventId, Dem_ExtendedStatus_MonitorDisabledTOC) == TRUE)
  {
    Dem_EventOBDReadinessGroupType lReadinessGroupId = Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId);
    Dem_Core_ResetMonitorDisabledTOC(EventId);

    if (Dem_Readiness_Completion_TestGroupCalculatedCompleted(lReadinessGroupId) == TRUE)
    {
      /* Reset completion status in readiness group */
      Dem_Readiness_Completion_UpdateCompletionStatus(lReadinessGroupId, FALSE);
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EventMonitor_DisableCurrentDcy
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_EventMonitor_DisableCurrentDcy(
  Dem_EventIdType EventId
)
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if ((Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId) != DEM_CFG_READINESS_NONE)
    && (Dem_Event_TestEventUnavailable(EventId) == FALSE)                                                                        /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    )
  {
    Dem_GlobalDiagnostics_EventMonitor_SetDisabled(EventId);
    lReturnValue = E_OK;
  }

  return lReturnValue;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_EVENTMONITOR_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EventMonitor_Implementation.h
 *********************************************************************************************************************/
