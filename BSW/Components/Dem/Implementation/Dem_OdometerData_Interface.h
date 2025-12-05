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
 *  \defgroup   Dem_Odometer Odometer
 *  \{
 *  \file       Dem_OdometerData_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details
 *  \entity     OdometerData
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_ODOMETERDATA_INTERFACE_H)
#define DEM_ODOMETERDATA_INTERFACE_H

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
#include "Dem_OdometerData_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_OdometerData_PublicProperties Public Properties
 * \{
 */

/*!
* \}
*/
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
*  UNIT API FUNCTION DECLARATIONS
*********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
* \defgroup Dem_OdometerData_Public Public Methods
* \{
*/

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_MilRelated_GetDistanceSinceMilOn
 *****************************************************************************/
/*!
 * \brief         Provides the distance travelled since the MIL turned on.
 *
 * \details       -
 *
 * \return        The distance travelled since the MIL turned on.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_GetDistanceSinceMilOn(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_MilRelated_Reset
 *****************************************************************************/
/*!
 * \brief         Reset odometer data related to the MIL.
 *
 * \details       Reset odometer data related to the MIL and set the update
 *                required to false.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_MilRelated_Reset(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ClearRelated_GetDistanceSinceLastClear
 *****************************************************************************/
/*!
 * \brief         Provides the distance travelled since the last clear operation
 *                occurred.
 *
 * \details       -
 *
 * \return        The distance travelled since the last clear operation.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ClearRelated_GetDistanceSinceLastClear(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_IsSupported
 *****************************************************************************/
/*!
 * \brief         Tests whether ConfirmedDTC related odometer data is supported.
 *
 * \details       Tests whether ConfirmedDTC related odometer data is supported.
 *
 * \return        TRUE
 *                ConfirmedDTC related odometer data is supported.
 * \return        FALSE
 *                ConfirmedDTC related odometer data is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_IsSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_GetDistanceConfirmedDTC
 *****************************************************************************/
/*!
 * \brief         Provides the distance travelled since any DTC was confirmed.
 *
 * \details       -
 *
 * \return        The distance travelled since any DTC was confirmed.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ConfirmedDTCRelated_GetDistanceConfirmedDTC(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_Init
 *****************************************************************************/
/*!
 * \brief         Initializes the odometer.
 *
 * \details       -
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_Init(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_InitAdminData
 *****************************************************************************/
/*!
 * \brief         Performs the initialization of odometer related admin data.
 *
 * \details       Performs the initialization of the odometer related admin data
 *                managed by the Global Diagnostics unit.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_InitAdminData(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ProcessAfterClear
 *****************************************************************************/
 /*!
 * \brief         Reset odometer values affected by clear operation.
 *
 * \details       -
 *
 * \pre           -
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ProcessAfterClear(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ProcessOnMainfunction
 *****************************************************************************/
/*!
 * \brief         Processes all tasks related to odometer updates
 *
 * \details       This function fetches the current odometer data and updates
 *                odometer related statistic data, e.g. PIDs or the permanent
 *                memory activation.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ProcessOnMainfunction(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ProcessMilStateChangedEnabled
 *****************************************************************************/
/*!
 * \brief         Hook function called when the MIL is activated
 *
 * \details       If the Mil status has changed from Off to On the current
 *                Odometer value (absolute) is set as base value.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ProcessMilStateChangedEnabled(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ProcessMilStateChangedDisabled
 *****************************************************************************/
/*!
 * \brief         Hook function called when the MIL is deactivated
 *
 * \details       If the Mil status has changed from On to Off the current
 *                Odometer value (absolute) is set invalid.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ProcessMilStateChangedDisabled(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_OdometerData_ProcessConfirmedDTCChanged
 *****************************************************************************/
/*!
 * \brief         Hook function called when the an event becomes confirmed
 *
 * \details       If the Mil status has changed from On to Off the current
 *                Odometer value (absolute) is set invalid.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_OdometerData_ProcessConfirmedDTCChanged(
  void
);

/*!
* \}
*/
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_ODOMETERDATA_INTERFACE_H */

/*!
* \}
*/
/* ********************************************************************************************************************
*  END OF FILE: Dem_OdometerData_Interface.h
*********************************************************************************************************************/
