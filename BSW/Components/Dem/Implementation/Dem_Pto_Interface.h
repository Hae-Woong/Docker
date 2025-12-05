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
 *  \defgroup   Dem_Pto Pto
 *  \{
 *  \file       Dem_Pto_Interface.h
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

#if !defined (DEM_PTO_INTERFACE_H)
#define DEM_PTO_INTERFACE_H

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
#include "Dem_Pto_Types.h"

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Pto_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_ActivationTimer_IsSupported
 *****************************************************************************/
/*!
 * \brief         Tests if calculation of PTO activation timer is supported for
 *                the configuration.
 *
 * \details       Tests if calculation of PTO activation timer is supported for
 *                the configuration.
 *
 * \return        TRUE
 *                Calculation of PTO activation timer is supported.
 * \return        FALSE
 *                Calculation of PTO activation timer is not supported.
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_IsSupported(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_ActivationTimer_ResetTimer
 *****************************************************************************/
/*!
 * \brief         Reset the PTO activation timer with the given index.
 *
 * \details       Reset the PTO activation timer with the given index.
 *
 * \param[in]     PtoTimerIdx
 *                Unique handle of the PTO activation timer
 *                The PtoTimerIdx must be in range [0..
 *                Dem_GlobalDiagnostics_Pto_ActivationTimer_GetSizeOfPtoTimerTable()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_ResetTimer(
  Dem_Pto_ActivationTimer_TimerIdType PtoTimerIdx
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_ActivationTimer_TestTimerLatched
 *****************************************************************************/
/*!
 * \brief         Test whether the PTO activation timer is latched
 *
 * \details       Test whether the PTO activation timer is latched
 *
 * \param[in]     PtoTimerIdx
 *                Unique handle of the PTO activation timer
 *                The PtoTimerIdx must be in range [0..
 *                Dem_GlobalDiagnostics_Pto_ActivationTimer_GetSizeOfPtoTimerTable()[.
 *
 * \return        TRUE
 *                PTO activation timer is latched
 * \return        FALSE
 *                PTO activation timer is not latched
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_TestTimerLatched(
  Dem_Pto_ActivationTimer_TimerIdType PtoTimerIdx
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_SetPtoStatus
 *****************************************************************************/
/*!
 * \brief         Set the Dem PTO status
 *
 * \details       Set the Dem PTO status
 *
 * \param[in]     PtoStatus
 *                Sets the status of the PTO (TRUE==active; FALSE==inactive)
 *
 * \return        E_OK
 *                new PTO-status has been adopted by Dem
 * \return        E_NOT_OK
 *                all other cases
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_GlobalDiagnostics_Pto_SetPtoStatus(
  boolean PtoStatus
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_GetPtoStatus
 *****************************************************************************/
/*!
 * \brief         Get the Dem PTO status
 *
 * \details       Get the Dem PTO status
 *
 * \return        TRUE
 *                PTO status is active
 * \return        FALSE
 *                PTO status is not supported or PTO status is inactive
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Pto_GetPtoStatus(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_Init
 *****************************************************************************/
/*!
 * \brief         Initialize the PTO to inactive.
 *
 * \details       Initialize the PTO to inactive.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_Init(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_IsPtoStatusSupportEnabled
 *****************************************************************************/
/*!
 * \brief         Test if PTO status support is enabled
 * \details       Test if PTO status support is enabled
 *
 * \return        TRUE
 *                PTO status support is enabled
 * \return        FALSE
 *                PTO status support is NOT enabled
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Pto_IsPtoStatusSupportEnabled(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_TestPtoActiveAndEventAffected
 *****************************************************************************/
/*!
 * \brief         Tests whether PTO is active and the event is affected by
 *                PTO
 *
 * \details       Tests whether PTO is active and the event is also affected by
 *                PTO
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                PTO is active and the event is affected by PTO
 * \return        FALSE
 *                PTO status is not supported, PTO is inactive or the event
 *                is not affected by PTO
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Pto_TestPtoActiveAndEventAffected(
  Dem_EventIdType EventId
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
 * \defgroup Dem_Pto_Public Public Methods
 * \{
 */


/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_ActivationTimer_InitAdminData
 *****************************************************************************/
/*!
 * \brief         Initializes PTO activation timers stored in AdminData block
 *
 * \details       Initialize all PTO activation timers to 0.
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_ActivationTimer_InitAdminData(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_MainFunction
 *****************************************************************************/
/*!
 * \brief         Process queued PTO activation timer changes
 *
 * \details       Process queued PTO activation timer changes
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_MainFunction(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Pto_CalculateTimer
 *****************************************************************************/
/*!
 * \brief         Calculates PTO activation timers
 *
 * \details       Calculates PTO activation timers
 *
 * \param[in]     EngineRunTimeTicks
 *                Engine runtime delta
 *                
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Pto_CalculateTimer(
  uint32 EngineRuntimeDelta
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_PTO_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Pto_Interface.h
 *********************************************************************************************************************/
