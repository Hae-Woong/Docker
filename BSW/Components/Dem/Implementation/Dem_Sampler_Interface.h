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
/*!
 *  \defgroup   Dem_Sampler Sampler
 *  \{
 *  \file       Dem_Sampler_Interface.h
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

#if !defined (DEM_SAMPLER_INTERFACE_H)
#define DEM_SAMPLER_INTERFACE_H

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
#include "Dem_Sampler_Types.h"

/* Includes for Other types used by the Implementation */
/* ------------------------------------------------------------------------- */
#include "Dem_Client_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Sampler_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Sampler_TestAnyNewRingBufferSampleAvailable
 *****************************************************************************/
/*!
 * \brief         Checks the ring buffers and returns if new sample has been 
 *                collected
 *
 * \details       New sample is collected for each sampling profile when the sampling timer
 *                expires. This function returns true if at least 1 new sample is collected
 *                for any of the configured sampling profiles.
 *                The return value of this function is only refreshed once per main function
 *
 * \return        TRUE, if at least one new sample has been collected in ring buffer(s)
 *                FALSE, no new sample available in ring buffer(s) since last refresh
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_TestAnyNewRingBufferSampleAvailable(
  void
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Sampler_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Sampler_Init
 *****************************************************************************/
/*!
 * \brief         Initializes time series sampler
 *
 * \details       Initializes time series sampler
 *
 * \pre           NvM must have restored NV mirrors
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_Init(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Sampler_ProcessTimerTask
 *****************************************************************************/
/*!
 * \brief         Processes sampling timer of time series snapshot records
 *
 * \details       Processes sampling timer of time series snapshot records
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_ProcessTimerTask(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Sampler_CollectData
 *****************************************************************************/
/*!
 * \brief         Processes asynchronously the collection of new time series
 *                samples into ring buffer for all sampling profiles
 *
 * \details       Each time the related timers have expired time series snapshot
 *                data is collected and stored into ring buffers.
 *                Furthermore the storage of future samples is triggered for
 *                configured sampling profiles. This function sets the NewSampleAvailable
 *                if a new sample has been collected in the ring buffer
 *
 * \pre           This function should be executed before any other main function(s)
 *                that fetch the new sample from the ring buffer.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_CollectData(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Sampler_ResetNewSampleAvailability
 *****************************************************************************/
/*!
 * \brief         Reset the new sample availability flags.
 *
 * \details       For each sampling in the ring buffer, reset the
 *                NewSampleAvailable flag.
 *
 * \pre           This function should only be executed after other main functions
 *                have finished fetching the new sample from the ring buffer.
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_ResetNewSampleAvailability(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Sampler_GetTimeSinceLastRingBufferSample
 *****************************************************************************/
/*!
 * \brief         Returns the time since the last sample was collected to the
 *                ring buffer with the given sample rate.
 *
 * \details       Returns the time since the last sample was collected to the
 *                ring buffer with the given sample rate, if time series
 *                power train feature is supported.
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \return        Time since the last sample was collected.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_GetTimeSinceLastRingBufferSample(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Sampler_TestNewRingBufferSampleAvailable
 *****************************************************************************/
/*!
 * \brief         Test whether a new sample has been stored in the ring buffer.
 *
 * \details       Test whether a new sample has been stored in the ring buffer
 *                since the value was last fetched.
 *
 * \return        TRUE
 *                A new sample is available.
 * \return        FALSE
 *                No new sample is available.
 *
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_TestNewRingBufferSampleAvailable(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Sampler_ProcessRingBufferFetched
 *****************************************************************************/
/*!
 * \brief         Sets the flag indicating whether a new sample
 *                is available.
 *
 * \details       -
 *
 * \param[in]     AvailabilityStatus
 *                The value to set the new sample flag to.
 * \param[in]     SamplingProfileId
 *                Specifies a sampling profile
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_ProcessRingBufferFetched(
  Dem_Cfg_SamplingProfileIterType SamplingProfileId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Sampler_GetDemTaskTime
 *****************************************************************************/
/*!
 * \brief         Get the configured DEM Task time in milliseconds
 *
 * \details       Get the configured DEM Task time in milliseconds
 *
 * \return        DEM Task time in milliseconds
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(float32, DEM_CODE)
Dem_GlobalDiagnostics_Sampler_GetDemTaskTime(
  void
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_SAMPLER_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Sampler_Interface.h
 *********************************************************************************************************************/
