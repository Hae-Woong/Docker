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
/*! \addtogroup Dem_Sampler
 *  \{
 *  \file       Dem_Sampler_Implementation.h
 *  \brief      Manages time series sampling timer.
 *  \details    Manages time series sampling timer.
 *  \entity     Sampler
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_SAMPLER_IMPLEMENTATION_H)
#define DEM_SAMPLER_IMPLEMENTATION_H

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
#include "Dem_Sampler_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_RingBuffer_Interface.h"
#include "Dem_DataStorage_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_DataStorage_Interface.h"
#include "Dem_Cfg_Declarations.h"
#include "Dem_TimeSeriesMemory_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_SAMPLER_IMPLEMENTATION_FILENAME "Dem_Sampler_Implementation.h"

 /* *******************************************************************************************************************
  *  SUBCOMPONENT FUNCTION MACROS
  ********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)

#define DEM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! time-series new sample collected flags by sampling profile */
DEM_LOCAL VAR(boolean, DEM_VAR_NO_INIT)           Dem_GlobalDiagnostics_Sampler_NewSampleAvailable[DEM_CFG_TIMESERIES_NUMBER_OF_SAMPLING_PROFILES];
/*! time-series sampling timer expired flags by sampling profile */
DEM_LOCAL VAR(boolean, DEM_VAR_NO_INIT)           Dem_GlobalDiagnostics_Sampler_Fetch[DEM_CFG_TIMESERIES_NUMBER_OF_SAMPLING_PROFILES];

#define DEM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DEM_START_SEC_VAR_NO_INIT_32
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! time-series snapshot record update poll timer */
DEM_LOCAL VAR(uint32, DEM_VAR_NO_INIT)            Dem_GlobalDiagnostics_Sampler_RateTimer[DEM_CFG_TIMESERIES_NUMBER_OF_SAMPLING_PROFILES];

#define DEM_STOP_SEC_VAR_NO_INIT_32
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Sampler_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Sampler_GetTimer
 *****************************************************************************/
/*!
 * \brief         Get the current value of time series timer.
 *
 * \details       Get the remaining number of main function ticks of the time 
 *                series timer for the given sample rate
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \return        Current value of sample timer for the given sample rate
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_GetTimer(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Sampler_CollectSamples
 *****************************************************************************/
/*!
 * \brief         Collect time series samples of sampling profile
 *
 * \details       Collect time series samples of sampling profile
 *
 * \pre           timer for sampling profile has expired
 * \config        DEM_FEATURE_NEED_TIME_SERIES == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_CollectSamples(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Sampler_GetTimer
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_GetTimer(                                                                                          /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
  uint32 lTimer = 0x00;
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SamplingProfileId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  if(SamplingProfileId < DEM_CFG_TIMESERIES_NUMBER_OF_SAMPLING_PROFILES)                                                         /* COV_DEM_ROBUSTNESS TX */
  {
    lTimer = Dem_GlobalDiagnostics_Sampler_RateTimer[SamplingProfileId];
  }
#endif

  return lTimer;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Sampler_CollectSamples
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_CollectSamples(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Data_DestinationBuffer lDestinationBuffer;
  if (Dem_Memories_TimeSeriesMemory_TestSamplingProfileExists() == TRUE)                                                         /* COV_DEM_ROBUSTNESS TX */
  {
    /* Collect all DIDs used in time series snapshot records with the specicied sampling profile and store them into ring buffer */
    Dem_Data_InitDestinationBuffer(&lDestinationBuffer, 
      Dem_GlobalDiagnostics_RingBuffer_GetRingBufferSlot(SamplingProfileId, Dem_GlobalDiagnostics_RingBuffer_GetCurrentIndex(SamplingProfileId)), 
      Dem_Cfg_GetSampleSizeOfTimeSeriesSamplingProfileTable(SamplingProfileId));                                                 /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT_RINGBUFFER */

    Dem_StorageManager_DataStorage_CollectTimeSeriesSamples(SamplingProfileId, &lDestinationBuffer);                             /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */

    Dem_GlobalDiagnostics_RingBuffer_Rotate(SamplingProfileId);
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(SamplingProfileId)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
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
 * \addtogroup Dem_Sampler_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Sampler_TestAnyNewRingBufferSampleAvailable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_TestAnyNewRingBufferSampleAvailable(
  void
  )
{
  boolean lAtLeastOneSampleAvailable = FALSE;

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Cfg_SamplingProfileIterType lSamplingProfileId;
  for (lSamplingProfileId = 0; lSamplingProfileId < Dem_Cfg_GetSizeOfTimeSeriesSamplingProfileTable(); lSamplingProfileId++)
  {
    if (Dem_GlobalDiagnostics_Sampler_TestNewRingBufferSampleAvailable(lSamplingProfileId) == TRUE)
    {
      lAtLeastOneSampleAvailable = TRUE;
      break;
    }
  }
#endif
  return lAtLeastOneSampleAvailable;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Sampler_Private Private Methods
 * \{
 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
                                                                                                                     /* PRQA S 6050 */ /* MD_MSR_STCAL */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Sampler_Public
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Sampler_Init
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
Dem_GlobalDiagnostics_Sampler_Init(
  void
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Cfg_SamplingProfileIterType lSamplingProfileId;
  for (lSamplingProfileId = 0;
       lSamplingProfileId < DEM_CFG_TIMESERIES_NUMBER_OF_SAMPLING_PROFILES;
       lSamplingProfileId++)
  {
    Dem_GlobalDiagnostics_Sampler_NewSampleAvailable[lSamplingProfileId] = FALSE;                                                /* SBSW_DEM_ARRAY_WRITE_SAMPLERDATA */
    Dem_GlobalDiagnostics_Sampler_Fetch[lSamplingProfileId] = FALSE;                                                             /* SBSW_DEM_ARRAY_WRITE_SAMPLERDATA */
    Dem_GlobalDiagnostics_Sampler_RateTimer[lSamplingProfileId] = Dem_Cfg_GetCyclesBetweenSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId); /* SBSW_DEM_ARRAY_WRITE_SAMPLERDATA */
  }
#endif
  if (Dem_Memories_TimeSeriesMemory_IsSupported() == TRUE)
  {
    Dem_Scheduler_EnableTask(Dem_Scheduler_Task_SamplerCollectData);
    Dem_Scheduler_EnableTask(Dem_Scheduler_Task_SamplerReset);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Sampler_ProcessTimerTask
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
Dem_GlobalDiagnostics_Sampler_ProcessTimerTask(
  void
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Cfg_SamplingProfileIterType lSamplingProfileId;
  for (lSamplingProfileId = 0;
       lSamplingProfileId < DEM_CFG_TIMESERIES_NUMBER_OF_SAMPLING_PROFILES;
       lSamplingProfileId++)
  {
    Dem_Internal_AssertContinue(!!Dem_GlobalDiagnostics_Sampler_RateTimer[lSamplingProfileId], DEM_E_INCONSISTENT_STATE)
    --Dem_GlobalDiagnostics_Sampler_RateTimer[lSamplingProfileId];                                                               /* SBSW_DEM_ARRAY_WRITE_SAMPLERDATA */
    if (Dem_GlobalDiagnostics_Sampler_RateTimer[lSamplingProfileId] == 0u)
    {
      Dem_GlobalDiagnostics_Sampler_Fetch[lSamplingProfileId] = TRUE;                                                            /* SBSW_DEM_ARRAY_WRITE_SAMPLERDATA */
      Dem_GlobalDiagnostics_Sampler_RateTimer[lSamplingProfileId] = Dem_Cfg_GetCyclesBetweenSamplesOfTimeSeriesSamplingProfileTable(lSamplingProfileId); /* PRQA S 2841 */ /* MD_DEM_Dir4.1_ArrayWriteOperation */ /* SBSW_DEM_ARRAY_WRITE_SAMPLERDATA */
    }
  }
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Sampler_CollectData
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
Dem_GlobalDiagnostics_Sampler_CollectData(
  void
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Cfg_SamplingProfileIterType lSamplingProfileId;
  for (lSamplingProfileId = 0;
       lSamplingProfileId < DEM_CFG_TIMESERIES_NUMBER_OF_SAMPLING_PROFILES;
       lSamplingProfileId++)
  {
    if (Dem_GlobalDiagnostics_Sampler_Fetch[lSamplingProfileId] == TRUE)
    {
      Dem_GlobalDiagnostics_Sampler_Fetch[lSamplingProfileId] = FALSE;                                                           /* SBSW_DEM_ARRAY_WRITE_SAMPLERDATA */
      Dem_GlobalDiagnostics_Sampler_NewSampleAvailable[lSamplingProfileId] = TRUE;                                               /* SBSW_DEM_ARRAY_WRITE_SAMPLERDATA */ 
      Dem_GlobalDiagnostics_Sampler_CollectSamples(lSamplingProfileId);
    }
  }
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Sampler_ResetNewSampleAvailability
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_ResetNewSampleAvailability(
  void
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Cfg_SamplingProfileIterType lSamplingProfileId;

  for(lSamplingProfileId = 0; lSamplingProfileId < Dem_Cfg_GetSizeOfTimeSeriesSamplingProfileTable(); lSamplingProfileId++)
  {
    if(Dem_GlobalDiagnostics_Sampler_TestNewRingBufferSampleAvailable(lSamplingProfileId))
    {
      Dem_GlobalDiagnostics_Sampler_ProcessRingBufferFetched(lSamplingProfileId);
    }
  }
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Sampler_GetTimeSinceLastRingBufferSample
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_GetTimeSinceLastRingBufferSample(                                                                  /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
  uint16 lTimeSinceLastSample = 0u;
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  if (Dem_Memories_TimeSeriesMemory_IsPowertrainEnabled() == TRUE)
  {
    uint32 lCurrentTimer = Dem_GlobalDiagnostics_Sampler_GetTimer(SamplingProfileId);
    uint32 lSamplingInterval = Dem_Cfg_GetCyclesBetweenSamplesOfTimeSeriesSamplingProfileTable(SamplingProfileId);

    lTimeSinceLastSample = Dem_Infrastructure_Utility_RoundFloatToUint16((float32)(lSamplingInterval - lCurrentTimer)            /* PRQA S 4394 */ /* MD_DEM_10.8_TdIntermediateIntegerToFloat */
      * Dem_GlobalDiagnostics_Sampler_GetDemTaskTime());
  }
#else 
  DEM_IGNORE_UNUSED_ARGUMENT(SamplingProfileId);                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
  return lTimeSinceLastSample;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Sampler_TestNewRingBufferSampleAvailable
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_TestNewRingBufferSampleAvailable(                                                                  /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
  boolean lNewSampleAvailable = FALSE;

#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  if(SamplingProfileId < DEM_CFG_TIMESERIES_NUMBER_OF_SAMPLING_PROFILES)                                                         /* COV_DEM_ROBUSTNESS TX */
  {
    lNewSampleAvailable = (boolean)(Dem_GlobalDiagnostics_Sampler_NewSampleAvailable[SamplingProfileId] == TRUE);
  }
#else
  DEM_IGNORE_UNUSED_ARGUMENT(SamplingProfileId);                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
  return lNewSampleAvailable;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Sampler_ProcessRingBufferFetched
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_ProcessRingBufferFetched(                                                                          /* PRQA S 3219 */ /*  MD_DEM_14.1_CONFIGURATION_SWITCH */
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
)
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  if(SamplingProfileId < DEM_CFG_TIMESERIES_NUMBER_OF_SAMPLING_PROFILES)                                                         /* COV_DEM_ROBUSTNESS TX */
  {
    Dem_GlobalDiagnostics_Sampler_NewSampleAvailable[SamplingProfileId] = FALSE;                                                 /* SBSW_DEM_ARRAY_WRITE_SAMPLERDATA */
  }
#else
  DEM_IGNORE_UNUSED_ARGUMENT(SamplingProfileId);                                                                                 /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */ 
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_Sampler_GetDemTaskTime
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(float32, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_GetDemTaskTime(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  void
)
{
  return (float32)DEM_CFG_TASK_TIME;
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_SAMPLER_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Sampler_Implementation.h
 *********************************************************************************************************************/
