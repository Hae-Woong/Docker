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
 *  \defgroup   Dem_EventMonitor EventMonitor
 *  \{
 *  \file       Dem_EventMonitor_Interface.h
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

#if !defined (DEM_EVENTMONITOR_INTERFACE_H)
#define DEM_EVENTMONITOR_INTERFACE_H

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
#include "Dem_EventMonitor_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_EventMonitor_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EventMonitor_IsDisableSupported
 *****************************************************************************/
/*!
 * \brief         Tests if disable event monitoring in current dcy is supported for
 *                the configuration.
 *
 * \details       Tests if disable event monitoring in current dcy is supported for
 *                the configuration.
 *
 * \return        TRUE
 *                Disable event monitoring in current dcy is supported.
 * \return        FALSE
 *                Disable event monitoring in current dcy is not supported.
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_EventMonitor_IsDisableSupported(
  void
);


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
 * \defgroup Dem_EventMonitor_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EventMonitor_ResetAll
 *****************************************************************************/
 /*!
  * \brief         Reset the disabled event monitor during end of driving cycle.
  *
  * \details       Reset the disabled event monitor during end of driving cycle.
  *
  * \pre           -
  *
  * \context       TASK
  * \synchronous   TRUE
  * \reentrant     FALSE
  *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EventMonitor_ResetAll(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EventMonitor_Reset
 *****************************************************************************/
/*!
 * \brief         Reset disabled event monitor functionality
 *
 * \details       Reset disabled event monitor functionality
 *
 * \pre           EventMonitorDisabled is supported
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_EventMonitor_Reset(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_EventMonitor_DisableCurrentDcy
 *****************************************************************************/
/*!
 * \brief         Enable disabled event monitor functionality
 *
 * \details       Enable disabled event monitor functionality
 *
 * \return        E_OK
 *                Event monitor disabled is enabled
 * \return        E_NOT_OK
 *                Event monitor disabled is not enabled
 *                
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_EventMonitor_DisableCurrentDcy(
  Dem_EventIdType EventId
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_EVENTMONITOR_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_PtoIF_Interface.h
 *********************************************************************************************************************/
