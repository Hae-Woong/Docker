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
 *  \defgroup   Dem_Completion Completion
 *  \{
 *  \file       Dem_Completion_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     Completion
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_COMPLETION_INTERFACE_H)
#define DEM_COMPLETION_INTERFACE_H

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
#include "Dem_Completion_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Completion_PublicProperties Public Properties
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
 * \defgroup Dem_Completion_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Readiness_Completion_TestGroupSupportsStaticCompletion
 *****************************************************************************/
/*!
 * \brief         Function returns if the completeness for readiness group is
 *                statically calculated
 *
 * \details       Function returns if the completeness for readiness group is
 *                statically calculated
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, PID41, DID F501, DM05, DM26.
 *
 * \return        TRUE
 *                Completeness for readiness group is statically calculated
 * \return        FALSE
 *                Completeness for readiness group is dynamically calculated
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_TestGroupSupportsStaticCompletion(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  uint8  DataContext
);

 /* ****************************************************************************
 *  Dem_Readiness_Completion_ProcessUpdateFlag
 *****************************************************************************/
 /*!
 * \brief         Calculates Readiness Completion.
 *
 * \details       Calculates Readiness Completion.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
 Dem_Readiness_Completion_ProcessUpdateFlag(
  void
);

/* ****************************************************************************
 * Dem_Readiness_Completion_ResetReadinessGroupCompletedStates
 *****************************************************************************/
/*!
 * \brief        Resets globally the stored completion values of all readiness
 *               groups.
 *
 * \details      Resets globally the stored completion values of all readiness
 *               groups.
 *
 *
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_ResetReadinessGroupCompletedStates(
  void
);


/* ****************************************************************************
 * Dem_Readiness_Completion_UpdateCompletionStatesAfterClear
 *****************************************************************************/
/*!
 * \brief         Update completion states after clear request.
 *
 * \details       Update completion states after clear request.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_UpdateCompletionStatesAfterClear(
  void
);

/* ****************************************************************************
 * Dem_Readiness_Completion_UpdateCompletionStatus
 *****************************************************************************/
/*!
 * \brief         Update completion status of the readiness group.
 *
 * \details       Update completion status of the readiness group.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[in]     Completed
 *                TRUE    Readiness group shall be set to completed.
 *                FALSE   Readiness group shall be set to not completed.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_UpdateCompletionStatus(
  Dem_EventOBDReadinessGroupType ReadinessGroup,
  boolean Complete
);

/* ****************************************************************************
 * Dem_Readiness_Completion_TriggerCompletionRecalculation
 *****************************************************************************/
/*!
 * \brief         Trigger recalculation of completion status of the
 *                readiness group.
 *
 * \details       Trigger recalculation of completion status of the
 *                readiness group. Recalculation is deferred to main function.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_TriggerCompletionRecalculation(
  Dem_EventOBDReadinessGroupType ReadinessGroup
);

/* ****************************************************************************
 * Dem_Readiness_Completion_TriggerCompletionRecalculationAll
 *****************************************************************************/
/*!
 * \brief         Trigger recalculation of completion states of all readiness
 *                groups.
 *
 * \details       Trigger recalculation of completion states of all readiness
 *                groups.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_TriggerCompletionRecalculationAll(
  void
);


/* ****************************************************************************
 * Dem_Readiness_Completion_TestGroupCalculatedCompleted
 *****************************************************************************/
/*!
 * \brief         Checks if readiness group is completed.
 *
 * \details       Checks the stored completion value of the given readiness
 *                group.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \return        TRUE
 *                Readiness group is completed
 * \return        FALSE
 *                Readiness group is not completed
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_TestGroupCalculatedCompleted(
  Dem_EventOBDReadinessGroupType ReadinessGroup
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_COMPLETION_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Completion_Interface.h
 *********************************************************************************************************************/
