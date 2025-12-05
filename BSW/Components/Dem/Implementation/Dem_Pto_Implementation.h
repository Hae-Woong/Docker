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
/*! \addtogroup Dem_Pto
 *  \{
 *  \file       Dem_Pto_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Manages Power Take-Off related Handling.
 *  \entity     Pto
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_PTO_IMPLEMENTATION_H)
#define DEM_PTO_IMPLEMENTATION_H

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
#include "Dem_Pto_Interface.h"

                                                    /* Used unit API */
/* ------------------------------------------------------------------------- */
#include "Dem_Core_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Satellite_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"
#include "Dem_FaultMemory_Interface.h"

                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_PTO_IMPLEMENTATION_FILENAME "Dem_Pto_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/
#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Maintains the current PTO activation status in Dem */
DEM_LOCAL VAR(boolean, DEM_VAR_NO_INIT)  Dem_GlobalDiagnostics_Pto_ActivationState;

#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Pto_PrivateProperties Private Properties
 * \{
 */


/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_ActivationTimer_GetTimer
 *****************************************************************************/
/*!
 * \brief         Get the PTO activation timer value for the timer index
 *
 * \details       Get the PTO activation timer value of for the timer index
 *
 * \param[in]     PtoTimerIdx
 *                Unique handle of the PTO activation timer
 *                The PtoTimerIdx must be in range [0..
 *                Dem_GlobalDiagnostics_Pto_ActivationTimer_GetSizeOfPtoTimerTable()[.
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_GetTimer(
  Dem_Pto_ActivationTimer_TimerIdType PtoTimerIdx
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_ActivationTimer_SetTimer
 *****************************************************************************/
/*!
 * \brief         Set the PTO activation timer value for the timer index
 *
 * \details       Set the PTO activation timer value for the timer index
 *
 * \param[in]     PtoTimerIdx
 *                Unique handle of the PTO activation timer
 *
 * \param[in]     TimerValue
 *                The new PTO activation timer value to set
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_SetTimer(
  Dem_Pto_ActivationTimer_TimerIdType PtoTimerIdx,
  uint16 TimerValue
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_ActivationTimer_GetSizeOfPtoTimerTable
 *****************************************************************************/
/*!
 * \brief         Returns the size of the array which stores the PTO timers
 *
 * \details       Returns the size of the array which stores the PTO timers
 *
 * \return        Size of PTO timer table
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Pto_ActivationTimer_TimerIdType, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_GetSizeOfPtoTimerTable(
  void
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/


/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_ActivationTimer_GetTimer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_GetTimer(
  Dem_Pto_ActivationTimer_TimerIdType PtoTimerIdx
  )
{
  uint16 lTimer = 0;
#if (DEM_FEATURE_NEED_PTO_TIMER == STD_ON)
  lTimer = Dem_Cfg_AdminData.EventPtoTimer[PtoTimerIdx];                                                                         /* PRQA S 2843 */ /* MD_DEM_Dir4.1_ReadOperation */
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(PtoTimerIdx)                                                                                  /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
  return lTimer;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_ActivationTimer_SetTimer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_SetTimer(
  Dem_Pto_ActivationTimer_TimerIdType PtoTimerIdx,
  uint16 TimerValue
  )
{
#if (DEM_FEATURE_NEED_PTO_TIMER == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (PtoTimerIdx >= Dem_GlobalDiagnostics_Pto_ActivationTimer_GetSizeOfPtoTimerTable())
  {
    Dem_Error_RunTimeCheckFailed(DEM_PTO_IMPLEMENTATION_FILENAME, __LINE__);                                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
# endif
  {
    Dem_Cfg_AdminData.EventPtoTimer[PtoTimerIdx] = TimerValue;                                                                   /* SBSW_DEM_ARRAY_WRITE_ADMINDATA_PTOTIMER */
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(PtoTimerIdx)                                                                                  /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(TimerValue)                                                                                   /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_ActivationTimer_GetSizeOfPtoTimerTable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Pto_ActivationTimer_TimerIdType, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_GetSizeOfPtoTimerTable(
  void
)
{
#if (DEM_FEATURE_NEED_PTO_TIMER == STD_ON)
  return (Dem_Pto_ActivationTimer_TimerIdType)DEM_CFG_MAX_NUMBER_EVENTS_WITH_PTO_TIMER;
#else
  return (Dem_Pto_ActivationTimer_TimerIdType)0;
#endif
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
 * \addtogroup Dem_Pto_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_ActivationTimer_IsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_IsSupported(
   void
  )
{
  return (boolean)(DEM_FEATURE_NEED_PTO_TIMER == STD_ON);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_ActivationTimer_ResetTimer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_ResetTimer(
  Dem_Pto_ActivationTimer_TimerIdType PtoTimerIdx
  )
{
  if (Dem_GlobalDiagnostics_Pto_ActivationTimer_IsSupported() == TRUE)
  {
    if (PtoTimerIdx != DEM_CFG_PTO_TIMER_INDEX_INVALID)
    {
      Dem_GlobalDiagnostics_Pto_ActivationTimer_SetTimer(PtoTimerIdx, 0);
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_ActivationTimer_TestTimerLatched
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_TestTimerLatched(
  Dem_Pto_ActivationTimer_TimerIdType PtoTimerIdx
)
{
  boolean lResult = FALSE;
  if (Dem_GlobalDiagnostics_Pto_IsPtoStatusSupportEnabled() == TRUE)
  { 
    if (PtoTimerIdx != DEM_CFG_PTO_TIMER_INDEX_INVALID)
    {
      lResult = (boolean)(Dem_GlobalDiagnostics_Pto_ActivationTimer_GetTimer(PtoTimerIdx) == DEM_PTO_ACTIVATIONTIMER_LATCHED);
    }
  }
  return lResult;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_SetPtoStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_Pto_SetPtoStatus(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  boolean PtoStatus
  )
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if (Dem_GlobalDiagnostics_Pto_IsPtoStatusSupportEnabled() == TRUE)
  {
    Dem_GlobalDiagnostics_Pto_ActivationState = PtoStatus;
    lReturnValue = E_OK;
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_GetPtoStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Pto_GetPtoStatus(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  void
  )
{
  boolean lReturnValue = FALSE;

  if (Dem_GlobalDiagnostics_Pto_IsPtoStatusSupportEnabled() == TRUE)
  {
    lReturnValue = Dem_GlobalDiagnostics_Pto_ActivationState;
  }

  return lReturnValue;
}


/* ****************************************************************************
% Dem_GlobalDiagnostics_Pto_Init
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_Init(
  void
  )
{
  Dem_GlobalDiagnostics_Pto_ActivationState = FALSE;
  Dem_Scheduler_EnableTask(Dem_Scheduler_Task_Pto);
}


/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_IsPtoStatusSupportEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Pto_IsPtoStatusSupportEnabled(
  void
  )
{
  return (boolean)(DEM_CFG_PTO_STATUS_SUPPORT == STD_ON);
}


/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_TestPtoActiveAndEventAffected
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Pto_TestPtoActiveAndEventAffected(
  Dem_EventIdType     EventId
  )
{
  return (boolean)( (Dem_GlobalDiagnostics_Pto_GetPtoStatus() == TRUE)
    && (Dem_Core_Event_TestEventAffectedByPto(EventId) == TRUE) );
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
 * \defgroup Dem_Pto_Private Private Methods
 * \{
 */


/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_ProcessSatelliteQueueActions
 *****************************************************************************/
/*!
 * \brief         Process queued PTO timer actions.
 *
 * \details       This function processes queued PTO timer actions for all
 *                events of the given satellite.
 *
 * \param[in]     SatelliteId
 *                Unique handle of the Satellite.
 *                The SatelliteId must be in range [1..Dem_Cfg_GlobalSatelliteCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ProcessSatelliteQueueActions(
  Dem_Satellite_IdType SatelliteId
  );

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

 
/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_ProcessSatelliteQueueActions
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ProcessSatelliteQueueActions(
  Dem_Satellite_IdType SatelliteId
  )
{
  Dem_Satellite_MonitorIdType lMonitorId = 0U;

  for (;lMonitorId < Dem_Cfg_GetEventCountOfSatelliteInfo(SatelliteId); ++lMonitorId)
  {
    if (Dem_Satellite_PeekPtoTimerActionQueue(SatelliteId, lMonitorId) != DEM_PTO_ACTIVATIONTIMER_QUEUE_INITIAL)
    {
      Dem_EventIdType lEventId = Dem_Satellite_GetEventId(SatelliteId, lMonitorId);
      Dem_Pto_ActivationTimer_TimerIdType lPtoTimerQueueStatus = Dem_Satellite_ConsumePtoTimerActionQueue(SatelliteId, lMonitorId);

      if (lPtoTimerQueueStatus == DEM_PTO_ACTIVATIONTIMER_QUEUE_RESTART)
      {
        Dem_Pto_ActivationTimer_TimerIdType lActivationTimerIndex = Dem_Core_Event_GetActivationTimerIndex(lEventId);
        if (Dem_GlobalDiagnostics_Pto_ActivationTimer_TestTimerLatched(lActivationTimerIndex) == TRUE)
        {
          Dem_GlobalDiagnostics_Readiness_RecalculateCompletionOnActivationTimerReset(lEventId);
        }
        Dem_GlobalDiagnostics_Pto_ActivationTimer_ResetTimer(lActivationTimerIndex);
      }
    }
  }
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
 * \addtogroup Dem_Pto_Public
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_ActivationTimer_InitAdminData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_InitAdminData(
  void
)
{
  if (Dem_GlobalDiagnostics_Pto_ActivationTimer_IsSupported() == TRUE)
  {    
    Dem_Pto_ActivationTimer_TimerIdType lPtoTimerIndex;
    Dem_Pto_ActivationTimer_TimerIdType lSizeOfPtoTimerTable;

    lSizeOfPtoTimerTable = Dem_GlobalDiagnostics_Pto_ActivationTimer_GetSizeOfPtoTimerTable();

    for (lPtoTimerIndex = 0u;
      lPtoTimerIndex < lSizeOfPtoTimerTable;
      lPtoTimerIndex++)
    {
      Dem_GlobalDiagnostics_Pto_ActivationTimer_SetTimer(lPtoTimerIndex, 0);
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_MainFunction
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_MainFunction(
  void
  )
{
  if ((Dem_GlobalDiagnostics_Pto_ActivationTimer_IsSupported() == TRUE)
    && (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE) )
  {
    Dem_Satellite_IdType lSatelliteId;
    for (lSatelliteId = 0u; lSatelliteId < Dem_Cfg_GlobalSatelliteCount(); ++lSatelliteId)
    {
      if (Dem_SatelliteCore_TestAndClearPtoTimerQueueUpdated(lSatelliteId) == TRUE)
      {
        Dem_GlobalDiagnostics_Pto_ProcessSatelliteQueueActions(lSatelliteId);

      }
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Pto_CalculateTimer
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_CalculateTimer(
   uint32 EngineRuntimeDelta
  )
{
  if ( (Dem_GlobalDiagnostics_Pto_ActivationTimer_IsSupported() == TRUE)
    && (Dem_GlobalDiagnostics_Pto_GetPtoStatus() == TRUE) )
  {
    Dem_EventIdType lEventId;

    for (lEventId = Dem_FaultMemory_Memories_GetFirstEvent(DEM_CFG_MEMORYID_PRIMARY);
         lEventId <= Dem_FaultMemory_Memories_GetLastEvent(DEM_CFG_MEMORYID_PRIMARY);
         ++lEventId)
    {
      Dem_Pto_ActivationTimer_TimerIdType lTimerIdx = Dem_Core_Event_GetActivationTimerIndex(lEventId);
      if ((lTimerIdx != DEM_CFG_PTO_TIMER_INDEX_INVALID)
        && (Dem_Event_TestEventAvailable(lEventId) == TRUE))
      {
        if (Dem_GlobalDiagnostics_Pto_ActivationTimer_TestTimerLatched(lTimerIdx) == FALSE)
        {
          uint32 lNewTimer = Dem_GlobalDiagnostics_Pto_ActivationTimer_GetTimer(lTimerIdx) +
            EngineRuntimeDelta;

          if (lNewTimer > DEM_PTO_ACTIVATIONTIMER_LATCHED)
          { /* latch at 750min */
            lNewTimer = DEM_PTO_ACTIVATIONTIMER_LATCHED;
          }

          Dem_GlobalDiagnostics_Pto_ActivationTimer_SetTimer(lTimerIdx, (uint16)lNewTimer);

          if (Dem_GlobalDiagnostics_Pto_ActivationTimer_TestTimerLatched(lTimerIdx) == TRUE)
          {
            Dem_Core_Event_TryResetQualifiedConfirmedSinceLastClear(lEventId);
            Dem_GlobalDiagnostics_Readiness_CompletionRecalculationOnActivationTimerLatch(lEventId);
          }
        }
      }
    }
  }

}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_PTO_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Pto_Implementation.h
 *********************************************************************************************************************/
