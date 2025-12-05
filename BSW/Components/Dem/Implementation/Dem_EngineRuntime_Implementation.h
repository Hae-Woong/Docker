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
/*! \addtogroup Dem_EngineRuntime
 *  \{
 *  \file       Dem_EngineRuntime_Implementation.h
 *  \brief      Subcomponent to fetch engine runtime.
 *  \details    Subcomponent to fetch and provide engine runtime.
 *  \entity     EngineRuntime
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_ENGINERUNTIME_IMPLEMENTATION_H)
#define DEM_ENGINERUNTIME_IMPLEMENTATION_H

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
#include "Dem_EngineRuntime_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Client_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Pto_Interface.h"
#include "Dem_Cfg_Declarations.h"
#include "Dem_Indicator_Interface.h"
#include "Dem_ActivationMode_Interface.h"
#include "Dem_GlobalDiagnostics_Interface.h"



/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_ENGINERUNTIME_IMPLEMENTATION_FILENAME "Dem_EngineRuntime_Implementation.h"

 /* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/
#define DEM_START_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

 /*! Engine runtime value in minutes update request flag */
DEM_LOCAL VAR(boolean, DEM_VAR_NO_INIT)           Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesUpdatePending;

 /*! Engine runtime value in seconds update request flag */
DEM_LOCAL VAR(boolean, DEM_VAR_NO_INIT)           Dem_GlobalDiagnostics_EngineRuntime_Value_SecondsUpdatePending;

#define DEM_STOP_SEC_VAR_NO_INIT_8
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define DEM_START_SEC_VAR_NO_INIT_32
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

 /*! Last fetched engine runtime value in minutes */
DEM_LOCAL VAR(uint32, DEM_VAR_NO_INIT)            Dem_GlobalDiagnostics_EngineRuntime_Value_Minutes;

/*! Poll timer to update engine runtime in minutes*/
DEM_LOCAL VAR(uint32, DEM_VAR_NO_INIT)            Dem_GlobalDiagnostics_EngineRuntime_Timer_Minutes;

/*! Last fetched engine runtime value in seconds */
DEM_LOCAL VAR(uint32, DEM_VAR_NO_INIT)            Dem_GlobalDiagnostics_EngineRuntime_Value_Seconds;

/*! Poll timer to update engine runtime in seconds*/
DEM_LOCAL VAR(uint32, DEM_VAR_NO_INIT)            Dem_GlobalDiagnostics_EngineRuntime_Timer_Seconds;

#define DEM_STOP_SEC_VAR_NO_INIT_32
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_EngineRuntime_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_SetCumulativeRuntime
 *****************************************************************************/
/*!
 * \brief         Sets the cumulative engine runtime.
 *
 * \details       Sets the cumulative engine runtime in the NvM admin data block.
 *
 * \param[in]     CumulativeEngineRuntime
 *                The cumulative engine runtime to be set.
 *
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_SetCumulativeRuntime(
  uint32 CumulativeEngineRuntime
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_IsTimeSinceMilOnSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether MIL related engine runtime is supported.
 * 
 * \details       -
 *
 * \return        TRUE
 *                Engine runtime since MIL on is supported.
 * \return        FALSE
 *                Engine runtime since MIL on is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_IsSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_IsSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether clear related engine runtime is supported.
 * 
 * \details       -
 *
 * \return        TRUE
 *                Engine runtime since last clear is supported.
 * \return        FALSE
 *                Engine runtime since last clear is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_IsSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_SetTimeSinceClear
 *****************************************************************************/
/*!
 * \brief         Sets the engine runtime since the last clear operation was
 *                performed.
 *
 * \details       Sets the engine runtime since the last clear operation was
 *                performed in the NvM admin data.
 *
 * \param[in]     EngineRuntimeSinceClear
 *                The value the engine runtime is set to.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_SetTimeSinceClear(
  uint16 EngineRuntimeSinceClear
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_SetTimeSinceConfirmedDTC
 *****************************************************************************/
 /*!
 * \brief         Sets the engine runtime since the first DTC was reported
 *                confirmed.
 *
 * \details       Sets the engine runtime since the first DTC was reported
 *                confirmed in the NvM admin data.
 *
 * \param[in]     EngineRunTimeAtMalfunctionActive
 *                The value the engine runtime is set to.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_SetTimeSinceConfirmedDTC(
  uint16 EngineRunTimeAtMalfunctionActive
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_SetCumulativeRuntime
 *****************************************************************************/
/*!
 * \brief         Sets the cumulative engine runtime.
 *
 * \details       Sets the cumulative engine runtime in the NvM admin data block.
 *
 * \param[in]     CumulativeEngineRuntime
 *                The cumulative engine runtime to be set.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_SetCumulativeRuntime(
  uint32 CumulativeEngineRuntime
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_SetCumulativeRuntime
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_SetCumulativeRuntime(
  uint32 CumulativeEngineRuntime
)
{
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  Dem_Cfg_AdminData.CumulativeEngineRunTime = CumulativeEngineRuntime;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(CumulativeEngineRuntime);
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_IsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_IsSupported(
  void
)
{
  return (boolean)(DEM_FEATURE_NEED_ENGINERUNTIME_MIL_ON == STD_ON);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_SetTimeSinceClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_SetTimeSinceClear(
  uint16 EngineRuntimeSinceClear
)
{
#if (DEM_FEATURE_NEED_ENGINERUNTIME_SINCE_CLEAR == STD_ON)
  Dem_Cfg_AdminData.EngineRunTimeSinceDtcClear = EngineRuntimeSinceClear;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EngineRuntimeSinceClear);
#endif
}

/* ****************************************************************************
% Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_SetTimeSinceConfirmedDTC
*****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_SetTimeSinceConfirmedDTC(
  uint16 EngineRunTimeAtMalfunctionActive
)
{
#if (DEM_FEATURE_NEED_ENGINERUNTIME_CONFIRMEDDTC == STD_ON)
  Dem_Cfg_AdminData.EngineRunTimeAtMalfunctionActive = EngineRunTimeAtMalfunctionActive;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EngineRunTimeAtMalfunctionActive);
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_IsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_IsSupported(
  void
)
{
  return (boolean)(DEM_FEATURE_NEED_ENGINERUNTIME_SINCE_CLEAR == STD_ON);
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
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_EngineRuntime_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_SetTimeSinceMilOn
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_SetTimeSinceMilOn(
  uint16 EngineRunTimeAtMalfunctionActive
)
{
#if (DEM_FEATURE_NEED_ENGINERUNTIME_MIL_ON == STD_ON)
  Dem_Cfg_AdminData.EngineRunTimeAtMalfunctionActive = EngineRunTimeAtMalfunctionActive;
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EngineRunTimeAtMalfunctionActive);
#endif
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_GetTimeSinceMilOn
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_GetTimeSinceMilOn(
  void
)
{
  uint16 lReturnValue = 0u;
#if (DEM_FEATURE_NEED_ENGINERUNTIME_MIL_ON == STD_ON)
  lReturnValue = Dem_Cfg_AdminData.EngineRunTimeAtMalfunctionActive;
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_GetTimeSinceConfirmedDTC
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_GetTimeSinceConfirmedDTC(
  void
)
{
  uint16 lReturnValue = 0u;
#if (DEM_FEATURE_NEED_ENGINERUNTIME_CONFIRMEDDTC == STD_ON)
  lReturnValue = Dem_Cfg_AdminData.EngineRunTimeAtMalfunctionActive;
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_GetTimeSinceClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_GetTimeSinceClear(
  void
)
{
  uint16 lReturnValue = 0u;
#if (DEM_FEATURE_NEED_ENGINERUNTIME_SINCE_CLEAR == STD_ON)
  lReturnValue = Dem_Cfg_AdminData.EngineRunTimeSinceDtcClear;
#endif
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_GetCumulativeRuntime
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_GetCumulativeRuntime(
  void
)
{
  uint32 lReturnValue = 0;
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  lReturnValue = Dem_Cfg_AdminData.CumulativeEngineRunTime;
#endif
  return lReturnValue;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_EngineRuntime_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_CalculateTimeSinceClear
 *****************************************************************************/
/*!
 * \brief         Calculates the time since the last clear operation happened.
 *
 * \details       -
 *
 * \param[in]     EngineRunTimeTicks
 *                Engine runtime delta
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_CalculateTimeSinceClear(
  uint32 EngineRunTimeTicks
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_CalculateTimeSinceMilOn
 *****************************************************************************/
/*!
 * \brief         Calculates the time since the MIL is on.
 *
 * \details       -
 *
 * \param[in]     EngineRunTimeTicks
 *                Engine runtime delta
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_CalculateTimeSinceMilOn(
  uint32 EngineRunTimeTicks
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_CalculateTimeSinceConfirmedDTC
 *****************************************************************************/
/*!
 * \brief         Calculates the time since the first DTC was reported confirmed.
 *
 * \details       -
 *
 * \param[in]     EngineRunTimeTicks
 *                Engine runtime delta
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_CalculateTimeSinceConfirmedDTC(
  uint32 EngineRunTimeTicks
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_IsSupported
 *****************************************************************************/
/*!
 * \brief         Tests if calculation of engine runtime is supported.
 *
 * \details       Tests if calculation of engine runtime with any resolution
 *                (Minutes, Seconds) is supported.
 *
 * \return        TRUE
 *                Calculation of engine runtime is supported.
 *                FALSE
 *                Calculation of engine runtime is not supported.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_IsSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_IsResolutionSupported
 *****************************************************************************/
/*!
 * \brief         Tests if the engine runtime calculation for the given
 *                resolution is supported.
 *
 * \details       Tests if the engine runtime calculation for the given
 *                resolution is supported.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \return        TRUE
 *                Calculation of engine runtime with the given resolution
 *                is supported.
 *                FALSE
 *                Calculation of engine runtime with the given resolution
 *                is not supported.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_IsResolutionSupported(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_InitForResolution
 *****************************************************************************/
/*!
 * \brief         Initialize the engine runtime with the given resolution.
 *
 * \details       For the engine runtime with the given resolution initialize
 *                the engine runtime value and timer to periodically fetch it.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_InitForResolution(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_EngineRuntime_Value_Init
 *****************************************************************************/
/*!
 * \brief         Initialize the engine runtime value.
 *
 * \details       Initialize the engine runtime value with the given resoltion.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_Init(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesInitToInvalid
 *****************************************************************************/
 /*!
 * \brief         Initialize the engine runtime value with a resolution of
 *                1 minute to invalid.
 *
 * \details       Initialize the engine runtime value with a resolution of
 *                one minute to invalid to indicate that no value value has
 *                been fetched so far.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesInitToInvalid(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesTestInvalid
 *****************************************************************************/
 /*!
 * \brief         Test if the engine runtime in minutes value is invalid.
 *
 * \details       Test if the engine runtime in minutes value has not been
 *                fetched so far.
 *
 * \return        TRUE
 *                Engine runtime value is invalid.
 *                FALSE
 *                Engine runtime value is valid.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesTestInvalid(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Timer_GetPollRate
 *****************************************************************************/
/*!
 * \brief         Get the poll rate for the engine runtime.
 *
 * \details       Get the timer ticks for the periodic polling of the engine
 *                runtime with the given resolution.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \return        PollRate
 *                The poll rate for the engine runtime with the given
 *                resolution.
 *                Returns zero if the engine runtime with the given resolution
 *                is not supported.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Timer_GetPollRate(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Value_GetCallback
 *****************************************************************************/
/*!
 * \brief         Get callback to retrieve the engine runtime value.
 *
 * \details       Get callback to retrieve the engine runtime with the given
 *                resolution.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \return        Handle to a data element descriptor.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DataIndexType, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_GetCallback(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Value_SetUpdatePending
 *****************************************************************************/
/*!
 * \brief         Sets the engine runtime update pending flag.
 *
 * \details       Sets the flag to indicate that a new engine runtime value
 *                with the given resolution shall be fetched.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_SetUpdatePending(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Value_ResetUpdatePending
 *****************************************************************************/
/*!
 * \brief         Resets the engine runtime update pending flag.
 *
 * \details       Resets the flag to indicate that a new engine runtime value
 *                with the given resolution shall be fetched.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_ResetUpdatePending(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Value_TestUpdatePending
 *****************************************************************************/
/*!
 * \brief         Tests if engine runtime update pending flag is set.
 *
 * \details       Tests if the flag to indicate that a new engine runtime
 *                value with the given resolution shall be fetched is set.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \return        TRUE
 *                Update pending flag is set.
 *                FALSE
 *                Update pending flag is not set.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_TestUpdatePending(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Timer_Init
 *****************************************************************************/
/*!
 * \brief         Initialize timer to fetch the engine runtime.
 *
 * \details       Initialize timer to fetch engine runtime value with the
 *                the given resolution.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Timer_Init(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ProcessTimerForResolution
 *****************************************************************************/
/*!
 * \brief         Process engine runtime timer with given resolution.
 *
 * \details       Process engine runtime timer and schedule fetching of engine
 *                runtime value with given resolution.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ProcessTimerForResolution(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Timer_Decrease
 *****************************************************************************/
/*!
 * \brief         Decrease the timer to fetch the engine runtime.
 *
 * \details       Count down the timer to periodically fetch engine runtime
 *                value with the the given resolution.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Timer_Decrease(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Timer_HasElapsed
 *****************************************************************************/
/*!
 * \brief         Tests if the timer to fetch the engine runtime has elasped.
 *
 * \details       Tests if the timer to periodically fetch engine runtime
 *                value with the the given resolution has elapsed.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \return        TRUE
 *                Timer with the given resolution has elasped.
 *                FALSE
 *                Timer with the given resolution has not elasped.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Timer_HasElapsed(
  uint8 Resolution
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ProcessForResolution
 *****************************************************************************/
/*!
 * \brief         Process engine runtime with given resolution.
 *
 * \details       Fetch and update engine runtime value with the given
 *                resolution.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ProcessForResolution(
  uint8 Resolution
);


/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_UpdateCumulative
 *****************************************************************************/
/*!
 * \brief         Increases the cumulative engine runtime by the given runtime
 *                ticks.
 *
 * \details       Increases the cumulative engine runtime by the given runtime
 *                ticks if WWHOBD is supported.
 *
 * \param[in]     EngineRunTimeTicks
 *                The amount of engine runtime ticks since the last update.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_UpdateCumulative(
  uint32 EngineRuntimeTicks
);


/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Value_Fetch
 *****************************************************************************/
/*!
 * \brief         Fetch the engine runtime value.
 *
 * \details       Fetch the engine runtime value with the given resolution
 *                from the application.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 * \param[out]    NewEngineRuntimeValue
 *                Pointer to receive the fetched engine runtime value.
 *
 * \return        E_OK
 *                Fetching engine runtime was successful.
 *                E_NOT_OK
 *                Fetching engine runtime was not successful.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_Fetch(
  uint8 Resolution,
  CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) NewEngineRuntimeValue
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Value_Update
 *****************************************************************************/
/*!
 * \brief         Update the engine runtime value with the newly fetched
 *                value.
 *
 * \details       Update the engine runtime value with the given resolution 
 *                with the newly fetched value.
 *
 * \param[in]     Resolution
 *                Engine runtime resolution (Minutes, Seconds) for which the
 *                function is invoked.
 * \param[in]     NewEngineRuntimeValue
 *                The new newly fetched engine runtime value.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_Update(
  uint8 Resolution,
  uint32 NewEngineRuntimeValue
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesCalculateDelta
 *****************************************************************************/
/*!
 * \brief         Calculate the passed engine runtime in minutes.
 *
 * \details       Calculate the engine runtime in minutes that has passed
 *                since the new fetch and the last time the engine runtime
 *                value was fetched.
 *
 * \param[in]     NewEngineRuntimeValue
 *                The new newly fetched engine runtime value.
 * \param[out]    EngineRuntimeDelta
 *                The pointer to receive the passed engine runtime in minutes.
 *
 * \return        E_OK
 *                Calculating the passed engine runtime was succesful.
 *                E_NOT_OK
 *                The passed engine runtime could not be calculated (e.g.,
 *                the engine runtme value has only been fetched once).
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesCalculateDelta(
  uint32 NewEngineRuntimeValue,
  CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) EngineRuntimeDelta
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesTriggerUpdate
 *****************************************************************************/
/*!
 * \brief         Trigger updates based on passed engine runtime.
 *
 * \details       Trigger updates that require the passed engine runtime in
 *                minutes, such as the calculation of Pid 4D and Pid 4E or the
 *                processing of B1 counter and continuous Mi counter.
 *
 * \param[in]     EngineRuntimeDelta
 *                The passed engine runtime in minutes.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesTriggerUpdate(
  uint32 EngineRuntimeDelta
);

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_CalculateTimeSinceMilOn
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
Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_CalculateTimeSinceMilOn(
  uint32 EngineRunTimeTicks
)
{
  if (Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_IsSupported())
  {
    if (Dem_GlobalDiagnostics_Indicator_TestMilReportedActive() == TRUE)
    { /* Always increment if MIL is on and latch/ don't count if MIL is off */
      /* intentionally uint32 to avoid an overflow */
      uint32 lTimeSinceMilOn = Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_GetTimeSinceMilOn();

      if (lTimeSinceMilOn < 0xffffU)
      {
        lTimeSinceMilOn += EngineRunTimeTicks;
        if (lTimeSinceMilOn > 0xffffU)
        {
          lTimeSinceMilOn = 0xffffU;
        }
        Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_SetTimeSinceMilOn((uint16)lTimeSinceMilOn);
      }
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_CalculateTimeSinceConfirmedDTC
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
Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_CalculateTimeSinceConfirmedDTC(
  uint32 EngineRunTimeTicks
)
{
  if (Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_IsSupported())
  {
    if (Dem_GlobalDiagnostics_TestAnyObdRelevantEventConfirmed() == TRUE)
    { /* Always increment if ConfirmedDTC is on and latch/ don't count if ConfirmedDTC is off */
      /* intentionally uint32 to avoid an overflow */
      uint32 lTimeSinceConfirmedDTC = Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_GetTimeSinceConfirmedDTC();

      if (lTimeSinceConfirmedDTC < 0xffffU)
      {
        lTimeSinceConfirmedDTC += EngineRunTimeTicks;
        if (lTimeSinceConfirmedDTC > 0xffffU)
        {
          lTimeSinceConfirmedDTC = 0xffffU;
        }
        Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_SetTimeSinceConfirmedDTC((uint16)lTimeSinceConfirmedDTC);
      }
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_CalculateTimeSinceClear
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
Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_CalculateTimeSinceClear(
  uint32 EngineRunTimeTicks
)
{
  if (Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_IsSupported())
  {
    /* intentionally uint32 to avoid an overflow */
    uint32 lTimeSinceClear = Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_GetTimeSinceClear();

    if (lTimeSinceClear < 0xffffU)
    {
      lTimeSinceClear += EngineRunTimeTicks;
      if (lTimeSinceClear > 0xffffU)
      {
        lTimeSinceClear = 0xffffU;
      }
      Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_SetTimeSinceClear((uint16)lTimeSinceClear);
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_IsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_IsSupported(
  void
)
{
  boolean lReturnValue = FALSE;

  if ( (Dem_GlobalDiagnostics_EngineRuntime_IsResolutionSupported(
    DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES) == TRUE)
    || (Dem_GlobalDiagnostics_EngineRuntime_IsResolutionSupported(
      DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS) == TRUE) )
  {
    lReturnValue = TRUE;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_IsResolutionSupported
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
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_IsResolutionSupported(
  uint8 Resolution
)
{
  boolean lReturnValue = FALSE;

  switch (Resolution)
  {
  case DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES:
    if ( ( ( (Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_IsSupported() == TRUE)
          || (Dem_GlobalDiagnostics_Pto_ActivationTimer_IsSupported() == TRUE) )
        && (Dem_Cfg_IsEmissionObdSupportedInVariant() == TRUE) )
      || ((Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_IsSupported() == TRUE)
        && (Dem_Cfg_IsObdSupportedInVariant() == TRUE) )
      || (Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_IsSupported() == TRUE)
      || (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE) )
    {
      lReturnValue = TRUE;
    }
    break;
  case DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS:
    if ( (Dem_Client_ClientAccess_IsJ1939Readiness3Supported() == TRUE)
      && (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE) )
    {
      lReturnValue = TRUE;
    }
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
  return lReturnValue;
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Timer_GetPollRate
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Timer_GetPollRate(
  uint8 Resolution
)
{
  uint32 lPollRate = 0u;

  switch (Resolution)
  {
  case DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES:
#if (DEM_FEATURE_NEED_ENGINE_RUN_TIME_MINUTES == STD_ON)
    lPollRate = DEM_CFG_TIMER_ENGINE_RUN_TIME_POLL_RATE;
#else
    lPollRate = 0u;
#endif
    break;
  case DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS:
#if (DEM_FEATURE_NEED_ENGINE_RUN_TIME_SECONDS == STD_ON)
    lPollRate = DEM_CFG_TIMER_ENGINE_RUN_TIME_SECONDS_POLL_RATE;
#else
    lPollRate = 0u;
#endif
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
  
  return lPollRate;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Value_GetCallback
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_Cfg_DataIndexType, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_GetCallback(
  uint8 Resolution
)
{
  Dem_Cfg_DataIndexType lIndexInDataElementTable = 0u;

  switch (Resolution)
  {
  case DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES:
#if (DEM_FEATURE_NEED_ENGINE_RUN_TIME_MINUTES == STD_ON)
    lIndexInDataElementTable
      = Dem_Cfg_GetDataElementTableIdxOfGeneralObdInput(
        DEM_CFG_OBDENGINERUNTIME_INDEX);
#else
    lIndexInDataElementTable = 0u;
#endif
    break;
  case DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS:
#if (DEM_FEATURE_NEED_ENGINE_RUN_TIME_SECONDS == STD_ON)
    lIndexInDataElementTable
      = Dem_Cfg_GetDataElementTableIdxOfGeneralObdInput(
        DEM_CFG_OBDENGINERUNTIMESECONDS_INDEX);
#else
    lIndexInDataElementTable = 0u;
#endif
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }

  return lIndexInDataElementTable;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_InitForResolution
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
Dem_GlobalDiagnostics_EngineRuntime_InitForResolution(
  uint8 Resolution
)
{
  if (Dem_GlobalDiagnostics_EngineRuntime_IsResolutionSupported(Resolution) == TRUE)
  {
    Dem_GlobalDiagnostics_EngineRuntime_Value_Init(Resolution);
    Dem_GlobalDiagnostics_EngineRuntime_Value_ResetUpdatePending(Resolution);
    Dem_GlobalDiagnostics_EngineRuntime_Timer_Init(Resolution);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Value_Init
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_Init(
  uint8 Resolution
)
{
  switch (Resolution)
  {
  case DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES:
    Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesInitToInvalid();
    break;
  case DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS:
    Dem_GlobalDiagnostics_EngineRuntime_Value_Seconds = 0u;
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesInitToInvalid();
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesInitToInvalid(
  void
)
{
  Dem_GlobalDiagnostics_EngineRuntime_Value_Minutes
    = ((uint32)((Dem_GlobalDiagnostics_EngineRuntime_Value_Minutes) | 0x80000000UL));
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesTestInvalid();
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesTestInvalid(
  void
)
{
  return ((Dem_GetHiHiByte(Dem_GlobalDiagnostics_EngineRuntime_Value_Minutes) & 0x80U) != 0u);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Timer_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Timer_Init(
  uint8 Resolution
)
{
  switch (Resolution)
  {
  case DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES:
    Dem_GlobalDiagnostics_EngineRuntime_Timer_Minutes =
      Dem_GlobalDiagnostics_EngineRuntime_Timer_GetPollRate(Resolution);
    break;
  case DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS:
    Dem_GlobalDiagnostics_EngineRuntime_Timer_Seconds =
      Dem_GlobalDiagnostics_EngineRuntime_Timer_GetPollRate(Resolution);
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Value_SetUpdatePending
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_SetUpdatePending(
  uint8 Resolution
)
{
  switch (Resolution)
  {
  case DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES:
    Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesUpdatePending = TRUE;
    break;
  case DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS:
    Dem_GlobalDiagnostics_EngineRuntime_Value_SecondsUpdatePending = TRUE;
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Value_ResetUpdatePending
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_ResetUpdatePending(
  uint8 Resolution
)
{
  switch (Resolution)
  {
  case DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES:
    Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesUpdatePending = FALSE;
    break;
  case DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS:
    Dem_GlobalDiagnostics_EngineRuntime_Value_SecondsUpdatePending = FALSE;
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Value_TestUpdatePending
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_TestUpdatePending(
  uint8 Resolution
)
{
  boolean lReturnValue = FALSE;

  switch (Resolution)
  {
  case DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES:
    lReturnValue = Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesUpdatePending;
    break;
  case DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS:
    lReturnValue = Dem_GlobalDiagnostics_EngineRuntime_Value_SecondsUpdatePending;
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ProcessTimerForResolution
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ProcessTimerForResolution(
  uint8 Resolution
)
{
  if (Dem_GlobalDiagnostics_EngineRuntime_IsResolutionSupported(Resolution) == TRUE)
  {
    Dem_GlobalDiagnostics_EngineRuntime_Timer_Decrease(Resolution);
    if (Dem_GlobalDiagnostics_EngineRuntime_Timer_HasElapsed(Resolution) == TRUE)
    {
      Dem_GlobalDiagnostics_EngineRuntime_Value_SetUpdatePending(Resolution);
      Dem_Scheduler_EnableTaskOnce(Dem_Scheduler_Task_EngineRuntime);
      Dem_GlobalDiagnostics_EngineRuntime_Timer_Init(Resolution);
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Timer_Decrease
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Timer_Decrease(
  uint8 Resolution
)
{
  switch (Resolution)
  {
  case DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES:
    --Dem_GlobalDiagnostics_EngineRuntime_Timer_Minutes;
    break;
  case DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS:
    --Dem_GlobalDiagnostics_EngineRuntime_Timer_Seconds;
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Timer_HasElapsed
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Timer_HasElapsed(
  uint8 Resolution
)
{
  boolean lReturnValue = FALSE;
  
  switch (Resolution)
  {
  case DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES:
    if (Dem_GlobalDiagnostics_EngineRuntime_Timer_Minutes == 0u)
    {
      lReturnValue = TRUE;
    }
    break;
  case DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS:
    if (Dem_GlobalDiagnostics_EngineRuntime_Timer_Seconds == 0u)
    {
      lReturnValue = TRUE;
    }
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ProcessForResolution
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
Dem_GlobalDiagnostics_EngineRuntime_ProcessForResolution(
  uint8 Resolution
)
{
  if (Dem_GlobalDiagnostics_EngineRuntime_IsResolutionSupported(Resolution) == TRUE)
  {
    if (Dem_GlobalDiagnostics_EngineRuntime_Value_TestUpdatePending(Resolution) == TRUE)
    {
      uint32 lNewEngineRuntimeValue = 0u;
      if (Dem_GlobalDiagnostics_EngineRuntime_Value_Fetch(Resolution, &lNewEngineRuntimeValue) == E_OK)                          /* SBSW_DEM_POINTER_FORWARD_STACK */
      {
        Dem_GlobalDiagnostics_EngineRuntime_Value_Update(Resolution, lNewEngineRuntimeValue);
        Dem_GlobalDiagnostics_EngineRuntime_Value_ResetUpdatePending(Resolution);
      }
      else
      {
        Dem_Scheduler_EnableTaskOnce(Dem_Scheduler_Task_EngineRuntime);
      }
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_UpdateCumulative
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_UpdateCumulative(
  uint32 EngineRuntimeTicks
)
{
  uint32 lCurrentCumulativeEngineRuntime = Dem_GlobalDiagnostics_EngineRuntime_GetCumulativeRuntime();
  Dem_GlobalDiagnostics_EngineRuntime_SetCumulativeRuntime(lCurrentCumulativeEngineRuntime + EngineRuntimeTicks);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Value_Fetch
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
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_Fetch(
  uint8 Resolution,
  CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) NewEngineRuntimeValue
)
{
  Dem_Data_DestinationBuffer lDestinationBuffer;
  Std_ReturnType lReturnValue = E_NOT_OK;
  Dem_Cfg_DataIndexType lDataIndex = Dem_GlobalDiagnostics_EngineRuntime_Value_GetCallback(Resolution);
  uint8 lEngineRuntimeValueBuffer[4] = { 0 };

  Dem_Data_InitDestinationBuffer(&lDestinationBuffer, &(lEngineRuntimeValueBuffer[0]), 0x04);                                    /* SBSW_DEM_CALL_DESTINATIONBUFFER_INIT */

  lReturnValue = Dem_FaultMemory_ExtDataElement_CollectOBDRelevantUserData(                                                      /* SBSW_DEM_POINTER_LOCAL_DESTINATIONBUFFER */
    &lDestinationBuffer,
    lDataIndex);

  if (lReturnValue == E_OK)
  {
    *NewEngineRuntimeValue = Dem_Infrastructure_Utility_ExtractValueUint32(                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      &lEngineRuntimeValueBuffer[0]);                                                                                            /* SBSW_DEM_POINTER_FORWARD_STACK */
  }

  return lReturnValue;
}


/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Value_Update
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
Dem_GlobalDiagnostics_EngineRuntime_Value_Update(
  uint8 Resolution,
  uint32 NewEngineRuntimeValue
)
{
  switch (Resolution)
  {
  case DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES:
    {
      uint32 lEngineRuntimeDelta = 0u;
      if (Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesCalculateDelta(NewEngineRuntimeValue,                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
        &lEngineRuntimeDelta) == E_OK)
      {
        Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesTriggerUpdate(lEngineRuntimeDelta);
      }
    }
    Dem_GlobalDiagnostics_EngineRuntime_Value_Minutes = NewEngineRuntimeValue;
    break;
  case DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS:
    Dem_GlobalDiagnostics_EngineRuntime_Value_Seconds = NewEngineRuntimeValue;
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesCalculateDelta
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
 */
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesCalculateDelta(
  uint32 NewEngineRuntimeValue,
  CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) EngineRuntimeDelta
)
{
  Std_ReturnType lReturnValue = E_NOT_OK;

  if (Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesTestInvalid() == FALSE)
  {
    if (NewEngineRuntimeValue > Dem_GlobalDiagnostics_EngineRuntime_Value_Minutes)
    {
      /* Same engine run, delta is [Dem_GlobalDiagnostics_EngineRuntime_Value_Minutes, NewEngineRuntimeValue]. */
      *EngineRuntimeDelta = (NewEngineRuntimeValue - Dem_GlobalDiagnostics_EngineRuntime_Value_Minutes);                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      lReturnValue = E_OK;
    }
    else if (NewEngineRuntimeValue < Dem_GlobalDiagnostics_EngineRuntime_Value_Minutes)
    {
      /* New engine run, delta is [0, NewEngineRuntimeValue]. */
      *EngineRuntimeDelta = NewEngineRuntimeValue;                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      lReturnValue = E_OK;
    }
    else
    {
      /* New engine runtime value equals stored engine runtime value. */
      /* No delta available. */
    }
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesTriggerUpdate
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
Dem_GlobalDiagnostics_EngineRuntime_Value_MinutesTriggerUpdate(
  uint32 EngineRuntimeDelta
)
{
  Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_CalculateTimeSinceMilOn(EngineRuntimeDelta);
  Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_CalculateTimeSinceConfirmedDTC(EngineRuntimeDelta);
  Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_CalculateTimeSinceClear(EngineRuntimeDelta);

  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
  {
    Dem_GlobalDiagnostics_EngineRuntime_UpdateCumulative(EngineRuntimeDelta);
    Dem_Core_Aging_ProcessAgingTimer();
  }

  Dem_GlobalDiagnostics_ActivationMode_ProcessEngineRunTime(EngineRuntimeDelta);

  Dem_GlobalDiagnostics_Pto_CalculateTimer(EngineRuntimeDelta);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_EngineRuntime_Public
 * \{
 */

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Init(
  void
)
{
  if (Dem_GlobalDiagnostics_EngineRuntime_IsSupported() == TRUE)
  {
    Dem_GlobalDiagnostics_EngineRuntime_InitForResolution(
      DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES);
    Dem_GlobalDiagnostics_EngineRuntime_InitForResolution
    (DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_InitAdminData
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_InitAdminData(
  void
)
{
  Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_SetTimeSinceMilOn(0u);
  Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_SetTimeSinceConfirmedDTC(0u);
  Dem_GlobalDiagnostics_EngineRuntime_SetCumulativeRuntime(0u);
  Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_SetTimeSinceClear(0u);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ProcessAfterClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ProcessAfterClear(
  void
)
{
  if (Dem_GlobalDiagnostics_EngineRuntime_IsSupported() == TRUE)
  {
    Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_SetTimeSinceClear(0u);
    if (Dem_GlobalDiagnostics_Indicator_IsGlobalMilStateSupported())
    {
      if (Dem_GlobalDiagnostics_Indicator_TestMilReportedActive() == FALSE)
      {
        Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_SetTimeSinceMilOn(0u);
      }
    }
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ProcessTimers
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
*/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ProcessTimers(
  void
)
{
  if (Dem_GlobalDiagnostics_EngineRuntime_IsSupported() == TRUE)
  {
    Dem_GlobalDiagnostics_EngineRuntime_ProcessTimerForResolution(
      DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES);
    Dem_GlobalDiagnostics_EngineRuntime_ProcessTimerForResolution(
      DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ProcessOnMainFunction
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ProcessOnMainFunction(
  void
)
{
  if (Dem_GlobalDiagnostics_EngineRuntime_IsSupported() == TRUE)                                                                 /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_GlobalDiagnostics_EngineRuntime_ProcessForResolution(
      DEM_ENGINE_RUNTIME_RESOLUTION_MINUTES);
    Dem_GlobalDiagnostics_EngineRuntime_ProcessForResolution(
      DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS);
  }
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_GetInSeconds
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_GetInSeconds(
  void
)
{
  uint32 lEngineRuntime = 0u;

  if (Dem_GlobalDiagnostics_EngineRuntime_IsResolutionSupported(DEM_ENGINE_RUNTIME_RESOLUTION_SECONDS) == TRUE)                  /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    lEngineRuntime = Dem_GlobalDiagnostics_EngineRuntime_Value_Seconds;
  }
  return lEngineRuntime;
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ProcessMilStateChangedEnabled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ProcessMilStateChangedEnabled(
  void
)
{
  Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_SetTimeSinceMilOn(0u);
}

/* ****************************************************************************
 % Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_ProcessConfirmedDTCChanged
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
Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_ProcessConfirmedDTCChanged(
  void
)
{
  if (Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_IsSupported() == TRUE)
  {
    if (Dem_GlobalDiagnostics_TestNumOfConfirmedTransitionFlag() == TRUE)
    {
      if (Dem_GlobalDiagnostics_TestAnyObdRelevantEventConfirmed() == FALSE)
      {
        Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_SetTimeSinceConfirmedDTC(0u);
      }
    }
  }
}

/* ****************************************************************************
% Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_IsSupported
*****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_IsSupported(
  void
)
{
  return (boolean)((DEM_FEATURE_NEED_ENGINERUNTIME_CONFIRMEDDTC == STD_ON)
    && (Dem_Cfg_IsZeroEmissionObdSupportedInVariant() == TRUE));
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_ENGINERUNTIME_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EngineRuntime_Implementation.h
 *********************************************************************************************************************/
