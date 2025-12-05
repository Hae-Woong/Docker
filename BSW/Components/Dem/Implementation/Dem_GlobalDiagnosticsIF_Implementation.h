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
/*! \addtogroup Dem_GlobalDiagnosticsIF
 *  \{
 *  \file       Dem_GlobalDiagnosticsIF_Implementation.h
 *  \brief      Implementation of unit GlobalDiagnosticsIF.
 *  \details    Realizes the public interfaces of its parent logical unit, i.e. both Dem external service APIs and Dem internal APIs.
 *  \entity     GlobalDiagnosticsIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_GLOBALDIAGNOSTICSIF_IMPLEMENTATION_H)
#define DEM_GLOBALDIAGNOSTICSIF_IMPLEMENTATION_H

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

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_GlobalDiagnosticsIF_Interface.h"

                                                    /* Used subcomponent API */
/* ------------------------------------------------------------------------- */
#include "Dem_Client_Interface.h"
#include "Dem_Readiness_Interface.h"
#include "Dem_ActivationMode_Interface.h"
#include "Dem_Indicator_Interface.h"
#include "Dem_Pto_Interface.h"
#include "Dem_EngineRuntime_Interface.h"
#include "Dem_OdometerData_Interface.h"
#include "Dem_EnableCondition_Interface.h"
#include "Dem_StorageCondition_Interface.h"
#include "Dem_EventMonitor_Interface.h"
#include "Dem_RingBuffer_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Sampler_Interface.h"

                                                   /* Subcomponents callbacks*/
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Client_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_GLOBALDIAGNOSTICSIF_IMPLEMENTATION_FILENAME "Dem_GlobalDiagnosticsIF_Implementation.h"

/*! Aging counter threshold for PID aging */
#define DEM_GLOBALDIAGNOSTICS_PID_AGING_COUNTER_THRESHOLD      0x28U

#define DEM_GLOBALDIAGNOSTICS_NO_OBD_REQUIREMENTS              5U

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/
#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Flag indicating if a transition from 0u to 1u or from 1u to 0u has happened in Dem_GlobalDiagnostics_NumOfConfirmedEvents */
DEM_LOCAL VAR(boolean, DEM_VAR_NO_INIT)          Dem_GlobalDiagnostics_IsNumOfConfirmedTransited;

#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"  


#define DEM_START_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Number of currently ZEVonUDS related events which are confirmed */
DEM_LOCAL VAR(uint16, DEM_VAR_NO_INIT)           Dem_GlobalDiagnostics_NumOfConfirmedEvents;

#define DEM_STOP_SEC_VAR_NO_INIT_16
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_GlobalDiagnosticsIF_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_IsWarmUpCycleCountSinceClearSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether the warum cycle count since last clear
 *                operation is supported.
 *
 * \details       -
 *
 * \return        TRUE
 *                Warmup cycle count since last clear is supported.
 * \return        FALSE
 *                Warmup cycle count since last clear is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsWarmUpCycleCountSinceClearSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_SetWarmUpCycleCountSinceClear
 *****************************************************************************/
/*!
 * \brief         Sets the number of warmup cycles since the last clear
 *                operation was performed.
 *
 * \details       Sets the number of warmup cycles since last clear operation
 *                in the NvM Admin Data block.
 *
 * \param[in]     WarmUpCylceCount
 *                The new cylce count to set the warmup cycle counter to.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_SetWarmUpCycleCountSinceClear(
  uint8 WarmUpCylceCount
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetPidAgingCounter
 *****************************************************************************/
/*!
 * \brief         Returns the PID aging counter.
 *
 * \details       Returns the PID aging counter from the NvM admin data block, 
 *                if supported by configuration.
 *
 * \return        PID aging counter.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_GetPidAgingCounter(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_SetPidAgingCounter
 *****************************************************************************/
/*!
 * \brief         Sets the PID aging counter.
 *
 * \details       Sets the PID aging counter in the NvM admin data block,
 *                if supported by configuration.
 *
 * \param[in]     PidAgingCounter
 *                The new cylce count to set the PID aging counter to.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_SetPidAgingCounter(
  uint8 PidAgingCounter
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_SetIsNumOfConfirmedTransited
 *****************************************************************************/
/*!
 * \brief         Sets Dem_GlobalDiagnostics_IsNumOfConfirmedTransited
 *                to TRUE.
 *
 * \details       Sets Dem_GlobalDiagnostics_IsNumOfConfirmedTransited
 *                to TRUE.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_SetIsNumOfConfirmedTransited(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ResetIsNumOfConfirmedTransited
 *****************************************************************************/
/*!
 * \brief         Resets Dem_GlobalDiagnostics_IsNumOfConfirmedTransited
 *                to FALSE.
 *
 * \details       Resets Dem_GlobalDiagnostics_IsNumOfConfirmedTransited
 *                to FALSE.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ResetIsNumOfConfirmedTransited(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_IncrementConfirmedEvents
 *****************************************************************************/
/*!
 * \brief         Increments the counter which counts the number of obd-related
 *                confirmed events
 *
 * \details       Increments the counter which counts the number of obd-related
 *                confirmed events
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_IncrementConfirmedEvents(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_DecrementConfirmedEvents
 *****************************************************************************/
/*!
 * \brief         Decrements the counter which counts the number of obd-related
 *                confirmed events
 *
 * \details       Decrements the counter which counts the number of obd-related
 *                confirmed events
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_DecrementConfirmedEvents(
  void
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 * Dem_GlobalDiagnostics_IsWarmUpCycleCountSinceClearSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsWarmUpCycleCountSinceClearSupported(
  void
)
{
  return (boolean)(DEM_FEATURE_NEED_WARMUP_CYCLES_SINCE_CLEAR == STD_ON);
}

/* ****************************************************************************
 * Dem_GlobalDiagnostics_SetWarmUpCycleCountSinceClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_SetWarmUpCycleCountSinceClear(
  uint8 WarmUpCylceCount
)
{
#if DEM_FEATURE_NEED_WARMUP_CYCLES_SINCE_CLEAR == STD_ON
  Dem_Cfg_AdminData.WarmUpCountAtClear = WarmUpCylceCount;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(WarmUpCylceCount);
#endif
}

/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetPidAgingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_GetPidAgingCounter(
  void
)
{
  uint8 lPidAgingCounter = 0U;
#if DEM_FEATURE_NEED_GLOBAL_MIL_STATE == STD_ON
  lPidAgingCounter = Dem_Cfg_AdminData.PidAgingCounter;
#endif
  return lPidAgingCounter;
}

/* ****************************************************************************
 * Dem_GlobalDiagnostics_SetPidAgingCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_SetPidAgingCounter(
  uint8 PidAgingCounter
)
{
#if DEM_FEATURE_NEED_GLOBAL_MIL_STATE == STD_ON
  Dem_Cfg_AdminData.PidAgingCounter = PidAgingCounter;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(PidAgingCounter);
#endif
}


/* ****************************************************************************
 * Dem_GlobalDiagnostics_SetIsNumOfConfirmedTransited
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_SetIsNumOfConfirmedTransited(
  void
)
{
  Dem_GlobalDiagnostics_IsNumOfConfirmedTransited = TRUE;
}

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ResetIsNumOfConfirmedTransited
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ResetIsNumOfConfirmedTransited(
  void
)
{
  Dem_GlobalDiagnostics_IsNumOfConfirmedTransited = FALSE;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_IncrementConfirmedEvents
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
Dem_GlobalDiagnostics_IncrementConfirmedEvents(
  void
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_GlobalDiagnostics_NumOfConfirmedEvents >= Dem_Cfg_GlobalEventCount())
  {
    Dem_Error_RunTimeCheckFailed(DEM_GLOBALDIAGNOSTICSIF_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    ++Dem_GlobalDiagnostics_NumOfConfirmedEvents;
  }
  if (Dem_GlobalDiagnostics_NumOfConfirmedEvents == 1U)
  {
    Dem_GlobalDiagnostics_SetIsNumOfConfirmedTransited();
  }
  else
  {
    Dem_GlobalDiagnostics_ResetIsNumOfConfirmedTransited();
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_DecrementConfirmedEvents
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
Dem_GlobalDiagnostics_DecrementConfirmedEvents(
  void
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_GlobalDiagnostics_NumOfConfirmedEvents == 0U)
  {
    Dem_Error_RunTimeCheckFailed(DEM_GLOBALDIAGNOSTICSIF_IMPLEMENTATION_FILENAME, __LINE__);                                     /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    --Dem_GlobalDiagnostics_NumOfConfirmedEvents;
  }
  if (Dem_GlobalDiagnostics_NumOfConfirmedEvents == 0U)
  {
    Dem_GlobalDiagnostics_SetIsNumOfConfirmedTransited();
  }
  else
  {
    Dem_GlobalDiagnostics_ResetIsNumOfConfirmedTransited();
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_GlobalDiagnostics_ActivationMode_PublicProperties
 * \{
 */

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
/* ****************************************************************************
 % Dem_GlobalDiagnostics_ReadRingBufferSlotChronological
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_DataPtrType, DEM_CODE)
Dem_GlobalDiagnostics_ReadRingBufferSlotChronological(                                                                           /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId,
  uint8 DataIndexChrono
)
{
  return Dem_GlobalDiagnostics_RingBuffer_ReadSlotChronological(SamplingProfileId, DataIndexChrono);
}
#endif

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestNewRingBufferSampleAvailable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestNewRingBufferSampleAvailable(                                                                          /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
  return Dem_GlobalDiagnostics_Sampler_TestNewRingBufferSampleAvailable(SamplingProfileId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetRingBufferSampleCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_GetRingBufferSampleCount(                                                                                  /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
  return Dem_GlobalDiagnostics_RingBuffer_GetSampleCount(SamplingProfileId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetMostRecentRingBufferSampleId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_GetMostRecentRingBufferSampleId(                                                                           /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
  uint8 lMostRecentSampleId = 0u;
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  if (Dem_GlobalDiagnostics_RingBuffer_ArePastSamplesSupported(SamplingProfileId) == TRUE)
  {
    uint8 lSampleCount = Dem_GlobalDiagnostics_GetRingBufferSampleCount(SamplingProfileId);
    Dem_Internal_AssertContinue(lSampleCount > 0, DEM_E_INCONSISTENT_STATE)
    lMostRecentSampleId = (lSampleCount - 1u);
  }
#endif
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SamplingProfileId);
  return lMostRecentSampleId;

}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetTimeSinceLastRingBufferSample
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetTimeSinceLastRingBufferSample(                                                                          /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
  return Dem_GlobalDiagnostics_Sampler_GetTimeSinceLastRingBufferSample(SamplingProfileId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestStorageConditionValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestStorageConditionValid(
  uint8 StorageConditionId
  )
{
  return (boolean)(StorageConditionId < Dem_GlobalDiagnostics_StorageCondition_GetGlobalCount());
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestEnableConditionValid
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEnableConditionValid(
  uint8 EnableConditionId
  )
{
  return (boolean)(EnableConditionId < Dem_GlobalDiagnostics_EnableCondition_GetGlobalCount());
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_IsPtoStatusSupportEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsPtoStatusSupportEnabled(
  void
  )
{
  return Dem_GlobalDiagnostics_Pto_IsPtoStatusSupportEnabled();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetPtoStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_GetPtoStatus(                                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_LIB */
  void
  )
{
  return Dem_GlobalDiagnostics_Pto_GetPtoStatus();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_IsReadinessCalculationSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsReadinessCalculationSupported(
  void
)
{
  return Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetEngineRuntimeInSeconds
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_GetEngineRuntimeInSeconds(
  void
)
{
  return Dem_GlobalDiagnostics_EngineRuntime_GetInSeconds();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetCumulativeEngineRuntime
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_GetCumulativeEngineRuntime(
  void
)
{
  return Dem_GlobalDiagnostics_EngineRuntime_GetCumulativeRuntime();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_IsPtoActivationTimerCalculationSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsPtoActivationTimerCalculationSupported(
  void
)
{
  return Dem_GlobalDiagnostics_Pto_ActivationTimer_IsSupported();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_IsPtoActivationTimerSupportedForEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsPtoActivationTimerSupportedForEvent(
  Dem_EventIdType EventId
)
{
  return (boolean)(Dem_Core_Event_GetActivationTimerIndex(EventId) != DEM_CFG_PTO_TIMER_INDEX_INVALID);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_IsDisableEventMonitorSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsDisableEventMonitorSupported(
  void
)
{
  return Dem_GlobalDiagnostics_EventMonitor_IsDisableSupported();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetDistanceTravelledSinceMilOn
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetDistanceTravelledSinceMilOn(
  void
)
{
  return Dem_GlobalDiagnostics_OdometerData_MilRelated_GetDistanceSinceMilOn();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetDistanceTravelledSinceLastClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetDistanceTravelledSinceLastClear(
  void
)
{
  return Dem_GlobalDiagnostics_OdometerData_ClearRelated_GetDistanceSinceLastClear();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetDistanceTravelledSinceConfirmedDTC
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetDistanceTravelledSinceConfirmedDTC(                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
)
{
  return Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_GetDistanceConfirmedDTC();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetEngineRuntimeSinceMilOn
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetEngineRuntimeSinceMilOn(
  void
)
{
  return Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_GetTimeSinceMilOn();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetEngineRuntimeSinceClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetEngineRuntimeSinceClear(
  void
)
{
  return Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_GetTimeSinceClear();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestPtoActiveAndEventAffected
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestPtoActiveAndEventAffected(
  Dem_EventIdType  EventId
)
{
  return Dem_GlobalDiagnostics_Pto_TestPtoActiveAndEventAffected(EventId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestEventHasReadinessGroup
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEventHasReadinessGroup(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  return (boolean)(Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId) != DEM_CFG_READINESS_NONE);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestEventHasHighPriorityReadinessGroup
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEventHasHighPriorityReadinessGroup(                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
)
{
  return Dem_GlobalDiagnostics_Readiness_TestReadinessGroupIsHighPriority(
    Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetGlobalObdRequirements
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_GetGlobalObdRequirements(                                                                                  /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  uint8 lGlobalObdRequirements = DEM_GLOBALDIAGNOSTICS_NO_OBD_REQUIREMENTS;
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    lGlobalObdRequirements = Dem_Cfg_GetGlobalObdRequirements();
  }
  return lGlobalObdRequirements;
}


/* ****************************************************************************
 * Dem_GlobalDiagnostics_GetWarmUpCycleCountSinceClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_GetWarmUpCycleCountSinceClear(
  void
)
{
  uint8 lWarmUpCycleCountSinceClear = 0U;
#if DEM_FEATURE_NEED_WARMUP_CYCLES_SINCE_CLEAR == STD_ON
  lWarmUpCycleCountSinceClear = Dem_Cfg_AdminData.WarmUpCountAtClear;
#endif
  return lWarmUpCycleCountSinceClear;
}

/* ****************************************************************************
 * Dem_GlobalDiagnostics_TestEventHasMilAssigned
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEventHasMilAssigned(
  Dem_EventIdType  EventId
)
{
  return Dem_GlobalDiagnostics_Indicator_TestEventHasMilAssigned(EventId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestAnyObdRelevantEventConfirmed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestAnyObdRelevantEventConfirmed(
  void
)
{
  return (boolean)(Dem_GlobalDiagnostics_NumOfConfirmedEvents > 0u);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestNumOfConfirmedTransitionFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestNumOfConfirmedTransitionFlag(
  void
)
{
  return (boolean)(Dem_GlobalDiagnostics_IsNumOfConfirmedTransited == TRUE);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_IncrementConfirmedEventsIfNeeded
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
    Dem_GlobalDiagnostics_IncrementConfirmedEventsIfNeeded(
        CONST(Dem_EventIdType, AUTOMATIC) EventId
)
{
  if ((Dem_Core_DTC_IsObdRelated(EventId) == TRUE) && (Dem_Cfg_IsZeroEmissionObdSupportedInVariant() == TRUE))
  {
    Dem_GlobalDiagnostics_IncrementConfirmedEvents();
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_DecrementConfirmedEventsIfNeeded
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_DecrementConfirmedEventsIfNeeded(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
)
{
  if ((Dem_Core_DTC_IsObdRelated(EventId) == TRUE) && (Dem_Cfg_IsZeroEmissionObdSupportedInVariant() == TRUE))
  {
    Dem_GlobalDiagnostics_DecrementConfirmedEvents();
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_OdometerData_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ProcessEventCDTCEnabled
 *****************************************************************************/
/*!
 * \brief         Process the effects of a Confirmed bit transition to ON.
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
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventCDTCEnabled(
  Dem_EventIdType EventId
  );


/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/


/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessEventCDTCEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventCDTCEnabled(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_EventIdType EventId
)
{
  Dem_Readiness_Completion_UpdateCompletionStatus(
    Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId), TRUE);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_GlobalDiagnosticsIF_Public
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestAnyNewRingBufferSampleAvailable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestAnyNewRingBufferSampleAvailable(
  void
  )
{
  return Dem_GlobalDiagnostics_Sampler_TestAnyNewRingBufferSampleAvailable();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_CalculateNumberOfObdRelevantConfirmedDTCs
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_CalculateNumberOfObdRelevantConfirmedDTCs(
  void
  )
{
  uint16 lTotalConfirmedDTCCount = 0u;
  Dem_EventIdType lEventId = 0;
  for (lEventId = Dem_FaultMemory_Memories_GetFirstEvent(DEM_CFG_MEMORYID_PRIMARY);
       lEventId <= Dem_FaultMemory_Memories_GetLastEvent(DEM_CFG_MEMORYID_PRIMARY);
       lEventId++)
  {
    if ((Dem_Core_DTC_IsObdRelated(lEventId) == TRUE)
      && (lEventId == Dem_Cfg_GetMasterEvent(lEventId)))
    {
      uint8 lExternalDtcUdsStatus = Dem_DTC_GetExternalUDSStatus(lEventId);
      if (Dem_Event_UDSStatus_TestBit(lExternalDtcUdsStatus, Dem_UdsStatus_CDTC) == TRUE)
      {
        lTotalConfirmedDTCCount++;
      }
    }
  }
  return lTotalConfirmedDTCCount;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_CountConfirmedEvents
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
Dem_GlobalDiagnostics_CountConfirmedEvents(
  void
  )
{
  if (Dem_Cfg_IsZeroEmissionObdSupportedInVariant() == TRUE)
  {
    Dem_GlobalDiagnostics_NumOfConfirmedEvents = Dem_GlobalDiagnostics_CalculateNumberOfObdRelevantConfirmedDTCs();
  }
  else
  {
    Dem_GlobalDiagnostics_NumOfConfirmedEvents = 0u;
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_UpdateObdPidAging
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_UpdateObdPidAging(
  void
  )
{
  /* Process PID aging if MIL is reported as inactive */
  if (Dem_GlobalDiagnostics_Indicator_TestMilReportedActive() == FALSE)
  {
    uint8 lPidAgingCounter = Dem_GlobalDiagnostics_GetPidAgingCounter();

    if (lPidAgingCounter != 0u)
    {
      --lPidAgingCounter;
      Dem_GlobalDiagnostics_SetPidAgingCounter(lPidAgingCounter);

      if (lPidAgingCounter == 0u)
      {
        Dem_GlobalDiagnostics_OdometerData_MilRelated_Reset();
        Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_SetTimeSinceMilOn(0u);
      }
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_InitReadinessEventIter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_InitReadinessEventIter(
  Dem_EventOBDReadinessGroupType ReadinessGroupId,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
)
{
  Dem_GlobalDiagnostics_Readiness_EventIterInit(ReadinessGroupId, IterPtr);                                                      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ReadinessEventIterGetEventId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_GlobalDiagnostics_ReadinessEventIterGetEventId(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
)
{
  return Dem_GlobalDiagnostics_Readiness_EventIterGetEventId(IterPtr);                                                           /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_J1939CalculateLampStatusOfEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_J1939DcmLampStatusType, DEM_CODE)
Dem_GlobalDiagnostics_J1939CalculateLampStatusOfEvent(
  Dem_EventIdType  EventId
)
{
  return Dem_GlobalDiagnostics_Indicator_J1939_CalculateLampStatusOfEvent(EventId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_J1939CalculateNodeRelatedLampStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_J1939DcmLampStatusType, DEM_CODE)
Dem_GlobalDiagnostics_J1939CalculateNodeRelatedLampStatus(
  uint8  NodeIndex
)
{
  return Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeRelatedLampStatus(NodeIndex);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TriggerReadinessCompletionRecalculation
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_TriggerReadinessCompletionRecalculation(
  Dem_EventOBDReadinessGroupType ReadinessGroupId
)
{
  Dem_Readiness_Completion_TriggerCompletionRecalculation(ReadinessGroupId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ReadDataOfDM26
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ReadDataOfDM26(
  Dem_Data_DestinationBufferPtrType DestinationBuffer
)
{
  Dem_Readiness_Readout_ReadDataOfDM26(DestinationBuffer);                                                                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ReadDataOfDM05
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ReadDataOfDM05(
  Dem_Data_DestinationBufferPtrType DestinationBuffer
)
{
  Dem_Readiness_Readout_ReadDataOfDM05(DestinationBuffer);                                                                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
}


/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessOBDCycleRestart
 *****************************************************************************/
/*!
 * Internal comment removed. *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessOBDCycleRestart(
  void
  )
{
  Dem_GlobalDiagnostics_Readiness_SetRdyThisDcyDisabledMask(0);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessDCYEnd
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessDCYEnd(
void
)
{
  if (Dem_GlobalDiagnostics_Indicator_TestMilReportedActive() == TRUE)
  {
    Dem_GlobalDiagnostics_SetPidAgingCounter(DEM_GLOBALDIAGNOSTICS_PID_AGING_COUNTER_THRESHOLD);
  }
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessHealing();
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessHealing();
  }

  Dem_GlobalDiagnostics_EventMonitor_ResetAll();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessWUCStart
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessWUCStart(
  void
)
{
  if (Dem_GlobalDiagnostics_IsWarmUpCycleCountSinceClearSupported())
  {
    uint8 lWarmUpCyclesSinceClear = Dem_GlobalDiagnostics_GetWarmUpCycleCountSinceClear();
    if (lWarmUpCyclesSinceClear < 0xFFU)
    {
      Dem_GlobalDiagnostics_SetWarmUpCycleCountSinceClear(lWarmUpCyclesSinceClear + 1U);
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessWUCEnd
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessWUCEnd(
  void
)
{
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessAgingCounter();
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessEventCleared
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
Dem_GlobalDiagnostics_ProcessEventCleared(
  Dem_EventIdType  EventId,
  Dem_UdsStatusByteType OldEventStatus
)
{
  if (Dem_Event_UDSStatus_TestBit(OldEventStatus, Dem_UdsStatus_TF) == TRUE)
  {
    Dem_GlobalDiagnostics_ActivationMode_ProcessEventCleared(EventId);
  }
  if (Dem_Core_DTC_IsObdRelated(EventId))
  {
    Dem_GlobalDiagnostics_Pto_ActivationTimer_ResetTimer(Dem_Core_Event_GetActivationTimerIndex(EventId));
  }
  if (Dem_Event_UDSStatus_TestBit(OldEventStatus, Dem_UdsStatus_WIR) == TRUE)
  {
    Dem_GlobalDiagnostics_ProcessEventWirDisabled(EventId);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessEventDisabled
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventDisabled(
  Dem_EventIdType EventId
)
{
  Std_ReturnType lReturnValue = E_NOT_OK;
  Dem_EventOBDReadinessGroupType lReadinessGroup = Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId);
  if ( (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE)
    && (lReadinessGroup != DEM_CFG_READINESS_NONE)
    && (Dem_Event_TestEventUnavailable(EventId) == FALSE)                                                                        /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    )
  {
    Dem_GlobalDiagnostics_Readiness_SetGroupDisabled(lReadinessGroup);
    lReturnValue = E_OK;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessSynchronousEffectsOfEventAvailabilityChange
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessSynchronousEffectsOfEventAvailabilityChange(
  Dem_EventIdType  EventId,
  boolean Available
)
{
  if (Available == FALSE)                                                                                                        /* COV_DEM_ROBUSTNESS TX */
  {
    Dem_Readiness_Completion_TriggerCompletionRecalculation(Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId));
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessAsyncEffectsOfEventAvailabilityChange
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
Dem_GlobalDiagnostics_ProcessAsyncEffectsOfEventAvailabilityChange(
  Dem_EventIdType  EventId,
  boolean Available
)
{
  if (Available == TRUE)
  {
    Dem_Readiness_Completion_UpdateCompletionStatus(
      Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId), FALSE);
  }
  else
  {
    if (Dem_Core_DTC_IsObdRelated(EventId) == TRUE)
    {
      Dem_GlobalDiagnostics_Pto_ActivationTimer_ResetTimer(Dem_Core_Event_GetActivationTimerIndex(EventId));
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessEventPassed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventPassed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
)
{
  Dem_GlobalDiagnostics_ActivationMode_ProcessEventPassed(EventContext);                                                         /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessEventPDTCDisabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventPDTCDisabled(
  Dem_EventIdType  EventId
)
{
  Dem_Readiness_Completion_TriggerCompletionRecalculation(
    Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessEventTPSLCEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventTPSLCEnabled(
  Dem_EventIdType  EventId
)
{
  Dem_GlobalDiagnostics_TriggerReadinessCompletionRecalculation(
    Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventId));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_IsEventAndActivationMode4Active
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_IsEventAndActivationMode4Active(
  Dem_EventIdType  EventId
)
{
  return Dem_GlobalDiagnostics_ActivationMode_IsEventAndActivationMode4Active(EventId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessEventWirEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventWirEnabled(
  Dem_EventIdType  EventId
)
{
  Dem_GlobalDiagnostics_Indicator_ProcessEventWirEnabled(EventId);
  Dem_GlobalDiagnostics_ActivationMode_ProcessWirEnabled(EventId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessEventWirDisabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventWirDisabled(
  Dem_EventIdType  EventId
  )
{
  if ((Dem_Cfg_IsEmissionObdSupportedInVariant() == FALSE)
    || (Dem_Event_InternalStatus_QualifyStatus_TestBit(Dem_Event_InternalStatus_QualifyStatus_GetEventStatus(EventId), Dem_QualifyStatus_WIR) == TRUE))
  {
    Dem_GlobalDiagnostics_Indicator_ProcessEventWirDisabled(EventId);
    Dem_GlobalDiagnostics_ActivationMode_ProcessWirDisabled(EventId);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_PreInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_PreInit(
  void
)
{
  Dem_GlobalDiagnostics_EnableCondition_PreInit();
  Dem_GlobalDiagnostics_StorageCondition_PreInit();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Init
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Init(
  void
)
{
  Dem_GlobalDiagnostics_Readiness_Init();
  Dem_GlobalDiagnostics_EngineRuntime_Init();
  Dem_GlobalDiagnostics_OdometerData_Init();
  Dem_GlobalDiagnostics_EnableCondition_Init();
  Dem_GlobalDiagnostics_StorageCondition_Init();
  Dem_GlobalDiagnostics_Sampler_Init();
  Dem_GlobalDiagnostics_RingBuffer_Init();
  Dem_GlobalDiagnostics_Indicator_Init();
  Dem_GlobalDiagnostics_ActivationMode_Init();
  Dem_GlobalDiagnostics_ActivationMode_RestoreCounters();
  Dem_GlobalDiagnostics_Pto_Init();
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_GlobalDiagnostics_InitAdminData
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_InitAdminData(
  void
)
{
  /* Delegation to sub units */
  Dem_GlobalDiagnostics_OdometerData_InitAdminData();
  Dem_GlobalDiagnostics_EngineRuntime_InitAdminData();
  Dem_GlobalDiagnostics_ActivationMode_InitAdminData();
  Dem_GlobalDiagnostics_Pto_ActivationTimer_InitAdminData();

  /* Data handled by GlobalDiagnostics directly */
  Dem_GlobalDiagnostics_SetPidAgingCounter(0U);
  Dem_GlobalDiagnostics_SetWarmUpCycleCountSinceClear(0U);
  Dem_GlobalDiagnostics_Readiness_SetRdyThisDcyDisabledMask(0u);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_InitStatusData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_InitStatusData(
  void
)
{
  Dem_Readiness_Completion_ResetReadinessGroupCompletedStates();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessDcyOfEventQualified
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
Dem_GlobalDiagnostics_ProcessDcyOfEventQualified(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  )
{
  if (Dem_Core_Event_UpdateTrigger_TestConfirmed(EventContext->EventEffects) == TRUE)
  {
    if (Dem_GlobalDiagnostics_Pto_ActivationTimer_TestTimerLatched(
      Dem_Core_Event_GetActivationTimerIndex(EventContext->EventId)) == FALSE)
    {
      Dem_Readiness_Completion_UpdateCompletionStatus(
        Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(EventContext->EventId), TRUE);
    }

    if (Dem_Event_UDSStatus_TestBit(EventContext->NewEventStatus, Dem_UdsStatus_TF) == TRUE)
    {
      Dem_GlobalDiagnostics_ActivationMode_ProcessQualifiedActiveEvent(EventContext->EventId);
    }
  }

  if (Dem_Core_Event_UpdateTrigger_TestWIR(EventContext->EventEffects) == TRUE)
  { 
    /* Process the indicators */
    Dem_GlobalDiagnostics_ProcessEventWirEnabled(EventContext->EventId);
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessEventFailed
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
Dem_GlobalDiagnostics_ProcessEventFailed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  )
{
  if (Dem_Core_Event_UpdateTrigger_TestWIR(EventContext->EventEffects) == TRUE)
  {
    /* Suppress the Indicators if the driving cycle is not qualified yet */
    if ((Dem_Cfg_IsEmissionObdSupportedInVariant() == FALSE)
      || (Dem_Event_InternalStatus_QualifyStatus_TestBit(EventContext->QualifyStatus, Dem_QualifyStatus_WIR) == TRUE))
    {
      /* Process the indicators */
      Dem_GlobalDiagnostics_ProcessEventWirEnabled(EventContext->EventId);
    }
  }

  /* increment counter only if event initially becomes confirmed or
     if it is still confirmed and test failed changes from 0 to 1 */
  if (((Dem_Core_Event_UpdateTrigger_TestConfirmed(EventContext->EventEffects) == TRUE)
    && (Dem_Event_InternalStatus_QualifyStatus_TestBit(EventContext->QualifyStatus, Dem_QualifyStatus_CDTC) == TRUE))
    || ((Dem_Core_Event_UpdateTrigger_TestFailed(EventContext->EventEffects) == TRUE)
      && (Dem_Event_UDSStatus_TestBit(EventContext->OldExternalEventStatus, Dem_UdsStatus_CDTC) == TRUE)))
  {
    Dem_GlobalDiagnostics_ActivationMode_ProcessQualifiedActiveEvent(EventContext->EventId);
  }
  if ((Dem_Core_Event_UpdateTrigger_TestConfirmed(EventContext->EventEffects) == TRUE)
    && (Dem_Event_InternalStatus_QualifyStatus_TestBit(EventContext->QualifyStatus, Dem_QualifyStatus_CDTC) == TRUE))
  {
    Dem_GlobalDiagnostics_ProcessEventCDTCEnabled(EventContext->EventId);
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessEventOpCycleEnd
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessEventOpCycleEnd(
  Dem_EventIdType EventId
)
{
  Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessMalfunctionStatusAtOpCycleEnd(EventId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessAfterClear
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
Dem_GlobalDiagnostics_ProcessAfterClear(
  void
  )
{
  /* Delegation to sub units */
  Dem_GlobalDiagnostics_OdometerData_ProcessAfterClear();
  Dem_GlobalDiagnostics_EngineRuntime_ProcessAfterClear();
  Dem_Readiness_Completion_UpdateCompletionStatesAfterClear();

  /* Data handled by GlobalDiagnostics directly */
  Dem_GlobalDiagnostics_SetWarmUpCycleCountSinceClear(0U);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessAfterClearAll
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessAfterClearAll(
  void
  )
{
  Dem_GlobalDiagnostics_ActivationMode_ProcessAfterClearAll();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestEventEnableConditionsSatisfied
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEventEnableConditionsSatisfied(
  Dem_EventIdType  EventId
)
{
  return Dem_GlobalDiagnostics_EnableCondition_Group_TestActive(
    Dem_GlobalDiagnostics_EnableCondition_GetEnableConditionGroupId(EventId));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestEnableConditionFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEnableConditionFulfilled(                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8 EnableConditionId
)
{
  return Dem_GlobalDiagnostics_EnableCondition_TestFulfilled(EnableConditionId);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_TestEventStorageConditionsSatisfied
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_TestEventStorageConditionsSatisfied(
  Dem_EventIdType  EventId
)
{
  return Dem_GlobalDiagnostics_StorageCondition_TestStorageGroupConditionsFulfilled(
    Dem_GlobalDiagnostics_StorageCondition_GetStorageConditionGroupId(EventId));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ProcessTimers
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ProcessTimers(
  void
)
{
  Dem_GlobalDiagnostics_Sampler_ProcessTimerTask();
  Dem_GlobalDiagnostics_EngineRuntime_ProcessTimers();
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_DisableEventMonitoring
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_DisableEventMonitoring(
  Dem_EventIdType EventId
)
{
  Std_ReturnType retVal = E_NOT_OK;

  if (Dem_GlobalDiagnostics_EventMonitor_IsDisableSupported())
  {
    retVal = Dem_GlobalDiagnostics_EventMonitor_DisableCurrentDcy(EventId);
  }

  return retVal;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_ResetDisabledEventMonitor
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ResetDisabledEventMonitor(
  Dem_EventIdType  EventId
)
{
  if (Dem_GlobalDiagnostics_EventMonitor_IsDisableSupported())
  {
    Dem_GlobalDiagnostics_EventMonitor_Reset(EventId);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_GetSamplingTime
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_GetSamplingTime(                                                                                           /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
  DEM_IGNORE_UNUSED_ARGUMENT(SamplingProfileId);                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
  {
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
    float32 lSamplingTime = (float32)Dem_Cfg_GetCyclesBetweenSamplesOfTimeSeriesSamplingProfileTable(SamplingProfileId) * Dem_GlobalDiagnostics_Sampler_GetDemTaskTime();
    return Dem_Infrastructure_Utility_RoundFloatToUint16(lSamplingTime);
#else
    return 0;
#endif
  }
}
/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_GLOBALDIAGNOSTICSIF_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_GlobalDiagnosticsIF_Implementation.h
 *********************************************************************************************************************/
