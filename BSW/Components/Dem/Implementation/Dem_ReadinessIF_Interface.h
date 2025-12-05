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
 *  \defgroup   Dem_ReadinessIF ReadinessIF
 *  \{
 *  \file       Dem_ReadinessIF_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     ReadinessIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_READINESSIF_INTERFACE_H)
#define DEM_READINESSIF_INTERFACE_H

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
#include "Dem_ReadinessIF_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ReadinessIF_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_TestGroupCompleteThisDcy
 *****************************************************************************/
/*!
 * \brief         Cacluates the completion state of a readiness group for the
 *                current DCY.
 *
 * \details       -
 *
 * \param[in]     ReadinessGroupInfo
 *                Information about the availability, tested and failed status of
 *                all events assigned to the readiness group in the current
 *                operation cycle.
 * \param[in]     DataContext
 *                Specifies the context of the readiness calculation which
 *                is needed for correct calculation and formatting inside
 *                the destination buffer.
 * 
 * \return        TRUE
 *                Group is complete.
 *                FALSE
 *                Group is not complete.
 * 
 * \pre           DataContext refers PID41 or DM26.
 * \pre           Readiness calculation is supported for the given data context.
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_TestGroupCompleteThisDcy(                                                                        /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint8 ReadinessGroupInfo,
  uint8 DataContext
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_TestGroupDisabled
 *****************************************************************************/
/*!
 * \brief         Checks if Readiness Group is disabled
 *
 * \details       Checks if Readiness Group is disabled
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \return        TRUE
 *                Readiness group is disabled
 * \return        FALSE
 *                Readiness group is enabled
 *
 * \pre           Function to be invoked only if dynamic calculation of
 *                PID41 is enabled.
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_TestGroupDisabled(
  Dem_EventOBDReadinessGroupType ReadinessGroup
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_Init
 *****************************************************************************/
/*!
 * \brief         Initializes readiness components.
 *
 * \details       -
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_Init(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId
 *****************************************************************************/
/*!
 * \brief         Get the readiness group of event.
 *
 * \details       Get the readiness group of event.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()].
 *
 * \return        The readiness group of event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventOBDReadinessGroupType, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_GetReadinessGroupId(
  CONST(Dem_EventIdType, AUTOMATIC) EventId                                                                                      /* PRQA S 3206 */ /* MD_DEM_3206 */
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_TestReadinessGroupIsHighPriority
 *****************************************************************************/
/*!
 * \brief         Tests if the passed readiness group is high priority.
 *
 * \details       Tests if the passed readiness group belongs to misfire or fuel system
 *
 * \param[in]     ReadinessGroup
 *                Readiness group to test for high priority
 *
 * \return        TRUE
 *                Major monitors are supported and readiness group is
 *                FLSYS_NONCONT, FLSYS or MISF
 * \return        FALSE
 *                Major monitors are not supported or readiness group is
 *                other than FLSYS_NONCONT, FLSYS or MISF
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_TestReadinessGroupIsHighPriority(                                                                /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  Dem_EventOBDReadinessGroupType ReadinessGroup
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_IsMajorMonitorsSupported
 *****************************************************************************/
/*!
 * \brief         Tests if major monitors are supported.
 *
 * \details       Tests if major monitors are supported.
 *
 * \return        TRUE
 *                Major monitors are supported.
 * \return        FALSE
 *                Major monitors are not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsMajorMonitorsSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported
 *****************************************************************************/
/*!
 * \brief         Tests if motor type compression is supported in configuration.
 *
 * \details       Tests if motor type compression is supported in configuration.
 *
 * \return        TRUE
 *                Motor Type Compression is configured.
 * \return        FALSE
 *                Motor Type Spark is configured.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported
 *****************************************************************************/
/*!
 * \brief         Tests if readiness calculation is supported.
 *
 * \details       Tests if readiness calculation is supported.
 *
 * \return        TRUE
 *                Readiness calculation is supported.
 * \return        FALSE
 *                Readiness calculation is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInRdyThisDcy
 *****************************************************************************/
/*!
 * \brief         Tests if comprehensive components shall be reported as
 *                complete for readiness this driving cycle.
 *
 * \details       Tests if comprehensive component monitoring shall always be
 *                reported as complete in PID41 and DM26.
 *
 * \return        TRUE
 *                Comprehensive components monitoring shall be reported as
 *                complete in PID41 and DM26.
 * \return        FALSE
 *                Comprehensive component monitor completion is dynamically
 *                calculated in PID41 and DM26.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInRdyThisDcy(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInDidF501
 *****************************************************************************/
/*!
 * \brief         Tests if comprehensive components shall be reported as complete in PID F501.
 *
 * \details       Tests if comprehensive component monitoring shall always be
 *                 reported as complete in PID F501.
 *
 * \return        TRUE
 *                Comprehensive components monitoring shall be reported as complete in PID F501.
 * \return        FALSE
 *                Comprehensive component monitor completion is dynamically calculated in PID F501.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInDidF501(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_SetRdyThisDcyDisabledMask
 *****************************************************************************/
/*!
 * \brief        Set the value of the readiness this driving cycle disabled
 *               mask state
 *
 * \details      Set the readiness this driving cycle disabled mask state to
 *               the given value
 *
 * \param[in]    ReadinessThisDcyDisabledMaskValue
 *               Readiness this driving cycle disabled mask state value
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_SetRdyThisDcyDisabledMask(
  uint32 ReadinessThisDcyDisabledMaskValue
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
 * \defgroup Dem_ReadinessIF_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_SetGroupDisabled
 *****************************************************************************/
/*!
 * \brief         Disable OBD Readiness Group
 *
 * \details       Sets bit corresponding to readiness group in
 *                ReadinessThisDcyDisabledMask
 *                (0 = enabled, 1= disabled)
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_SetGroupDisabled(
  Dem_EventOBDReadinessGroupType ReadinessGroup
);


/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_CompletionRecalculationOnActivationTimerLatch
 *****************************************************************************/
/*!
 * \brief         Recalculate completion on event Pto timer latched
 *
 * \details       Iterate over all events in readiness group and recalculate
 *                completion
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_CompletionRecalculationOnActivationTimerLatch(
  Dem_EventIdType  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_RecalculateCompletionOnActivationTimerReset
 *****************************************************************************/
/*!
 * \brief         Recalculate completion on event Pto timer reset from latched
 *
 * \details       Recalculate completion based on event status and completion state
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_RecalculateCompletionOnActivationTimerReset(
  Dem_EventIdType  EventId
  );


/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_CalculateCurrentEventInfo
 *****************************************************************************/
/*!
 * \brief         Provides the event related information this driving cycle.
 *
 * \details       Provides the event related information this driving cycle,
 *                such as 'AnyEventFailed', 'AnyEventUntested', 'AnyEventAvailable'.
 *                These values are used for readiness calculation.
 *                All values are calculated by one function
 *                to increase the performance.
 *
 * \param[in]     ReadinessGroup
 *                Identifier of the readiness group
 * 
 * \return        Bit-coded readiness group information this DCY.
 * 
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_CalculateCurrentEventInfo(
  Dem_EventOBDReadinessGroupType ReadinessGroup
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_GroupIterInit
 *****************************************************************************/
/*!
 * \brief         Initialize the iterator to iterate over all readiness groups.
 *
 * \details       -
 *
 * \param[out]    IterPtr
 *                Iterator used for iteration over all readiness groups.
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
Dem_GlobalDiagnostics_Readiness_GroupIterInit(
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_GroupIterGetGroupId
 *****************************************************************************/
/*!
 * \brief         Returns the readiness group Id the iterator is pointing to.
 *
 * \details       -
 *
 * \param[in]     IterPtr
 *                Iterator used for iteration over all readiness groups.
 *
 * \return        Readiness group Id the iterator is pointing to.
 *
 * \pre           IterPtr has been initialized
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventOBDReadinessGroupType, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_GroupIterGetGroupId(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);


/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_EventIterGetEventId
 *****************************************************************************/
/*!
 * \brief         Returns the current event id the iterator is pointing to.
 *
 * \details       Returns the current event id the iterator is pointing to.
 *
 * \param[in]     IterPtr
 *                Iterator for all events of a readiness group
 *
 * \return        Current event id the iterator is pointing to
 *
 * \pre           IterPtr has been initialized
 *
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE)
Dem_GlobalDiagnostics_Readiness_EventIterGetEventId(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Readiness_EventIterInit
 *****************************************************************************/
/*!
 * \brief         Initialize the readiness event iterator.
 *
 * \details       Initialize the readiness event iterator used to iterate
 *                over all events which belong to the specified readiness group.
 *
 * \param[in]     ReadinessGroupId
 *                Unique identifier of the readiness group.
 * \param[out]    IterPtr
 *                Iterator for all events of a readiness group
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
Dem_GlobalDiagnostics_Readiness_EventIterInit(
  Dem_EventOBDReadinessGroupType ReadinessGroupId,
  CONSTP2VAR(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_READINESSIF_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ReadinessIF_Interface.h
 *********************************************************************************************************************/
