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
 *  \defgroup   Dem_EngineRuntime EngineRuntime
 *  \{
 *  \file       Dem_EngineRuntime_Interface.h
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

#if !defined (DEM_ENGINERUNTIME_INTERFACE_H)
#define DEM_ENGINERUNTIME_INTERFACE_H

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
#include "Dem_EngineRuntime_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_EngineRuntime_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_SetTimeSinceMilOn
 *****************************************************************************/
/*!
 * \brief         Sets the engine runtime since the MIL is on.
 *
 * \details       Sets the engine runtime since the MIL is on in the NvM admin data.
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
Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_SetTimeSinceMilOn(
  uint16 EngineRunTimeAtMalfunctionActive
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_GetTimeSinceMilOn
 *****************************************************************************/
/*!
 * \brief         Returns the engine runtime since the MIL turned on.
 *
 * \details       Returns the engine runtime since the MIL turned on
 *                stored in the NvM admin data block.
 *
 * \return        Engine runtimesince MIL turned on.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_MilRelatedData_GetTimeSinceMilOn(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_GetTimeSinceClear
 *****************************************************************************/
/*!
 * \brief         Returns the engine runtime since the MIL turned on.
 *
 * \details       Returns the engine runtime since the MIL turned on
 *                stored in the NvM admin data block.
 *
 * \return        Engine runtime since MIL turned on.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ClearRelatedData_GetTimeSinceClear(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_GetTimeSinceConfirmedDTC
 *****************************************************************************/
 /*!
 * \brief         Returns the engine runtime since confirmed DTC.
 *
 * \details       Returns the engine runtime since the first DTC was reported
 *                confirmed, stored in the NvM admin data block.
 *
 * \return        Engine runtime since ConfirmedDTC.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_GetTimeSinceConfirmedDTC(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_GetCumulativeRuntime
 *****************************************************************************/
/*!
 * \brief         Returns the cumulative engine runtime.
 *
 * \details       Returns the cumulative engine runtime from the NvM admin
 *                data block.
 *
 * \return        Cumulative Engine Runtime.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_GetCumulativeRuntime(
  void
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_EngineRuntime_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_Init
 *****************************************************************************/
/*!
 * \brief         Initialize engine runtime.
 *
 * \details       Initialize engine runtime values and timers to periodically
 *                fetch them.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_Init(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_InitAdminData
 *****************************************************************************/
/*!
 * \brief         Initialize admin data related to engine runtime.
 *
 * \details       Initialize admin data values related to the engine runtime.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_InitAdminData(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ProcessAfterClear
 *****************************************************************************/
/*!
 * \brief         Reset engine runtime values affected by clear operation.
 *
 * \details       Reset engine runtime values affected by clear operation.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ProcessAfterClear(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ProcessTimers
 *****************************************************************************/
/*!
 * \brief         Process engine runtime timers.
 *
 * \details       Process engine runtime timers and schedule fetching of engine
 *                runtime values if a timer has elapsed.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ProcessTimers(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ProcessOnMainFunction
 *****************************************************************************/
 /*!
 * \brief         Process engine runtime.
 *
 * \details       Fetch and update engine runtime values.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ProcessOnMainFunction(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_GetInSeconds
 *****************************************************************************/
/*!
 * \brief         Get the engine runtime value in seconds.
 *
 * \details       Get the stored engine runtime value in seconds.
 *
 * \return        The engine runtime in seconds.
 *                Zero if calculation of engine runtime in seconds is not
 *                supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_GetInSeconds(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ProcessMilStateChangedEnabled
 *****************************************************************************/
/*!
 * \brief         Hook function called when the MIL is activated
 *
 * \details       If the Mil status has changed from Off to On the current the
 *                Engine Run Time is set to 0
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ProcessMilStateChangedEnabled(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_ProcessConfirmedDTCChanged
 *****************************************************************************/
 /*!
 * \brief         Hook function called when the last DTC becomes unconfirmed
 *
 * \details       -
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_ProcessConfirmedDTCChanged(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_IsSupported
 *****************************************************************************/
 /*!
 * \brief         Tests whether ConfirmedDTCRelated engine runtime is supported.
 *
 * \details       -
 *
 * \return        TRUE
 *                Engine runtime since ConfirmedDTC is supported.
 * \return        FALSE
 *                Engine runtime since ConfirmedDTC is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EngineRuntime_ConfirmedDTCRelatedData_IsSupported(
  void
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_ENGINERUNTIME_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EngineRuntime_Interface.h
 *********************************************************************************************************************/
