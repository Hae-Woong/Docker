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
 *  \defgroup   Dem_ActivationMode ActivationMode
 *  \{
 *  \file       Dem_ActivationMode_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Manages the User and Special indicator states
 *  \entity     ActivationMode
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_ACTIVATIONMODE_INTERFACE_H)
#define DEM_ACTIVATIONMODE_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Dem_Int.h"
                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_ActivationMode_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ActivationMode_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValueInHours
 *****************************************************************************/
/*!
 * \brief          Get the WWH OBD B1 counter value in hours
 *
 * \details        -
 *
 * \return         B1 counter value in hours
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_GetValueInHours(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetValueInHours
 *****************************************************************************/
/*!
 * \brief        Get the WWH OBD continuous Mi counter value in hours
 *
 * \details      -
 *
 * \return       Continuous Mi counter value in hours
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_GetValueInHours(
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
 * \defgroup Dem_ActivationMode_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_Init
 *****************************************************************************/
/*!
 * \brief         Initializes Unit ActivationMode 
 *
 * \details       Initialize the activation mode of the ECU based on the status
 *                of all events available in this configuration variant.
 *
 *
 * \pre           Event status has been restored before
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_Init(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_InitAdminData
 *****************************************************************************/
/*!
 * \brief         Initializes activation mode counter stored in AdminData block
 *
 * \details       Initialize the B1Counter and ContinuousMI counter and their
 *                respective Healing and Aging counters
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_InitAdminData(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitCounters
 *****************************************************************************/
/*!
 * \brief         Initializes continuous Mi counters
 *
 * \details       Initializes continuous Mi counter related counters
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_InitCounters(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessAgingCounter
 *****************************************************************************/
/*!
 * \brief         Process aging of Continuous Mi counter
 *
 * \details       Process aging of Continuous Mi counter after WUC cycle end
 *
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessAgingCounter(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessHealing
 *****************************************************************************/
/*!
 * \brief         Process healing of Continuous Mi counter
 *
 * \details       Process healing of Continuous Mi counter after DCY cycle end
 *
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ContinuousMiCounter_ProcessHealing(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ProcessEngineRunTime
 *****************************************************************************/
/*!
 * \brief         Process EngineRunTime update
 *
 * \details       If WwhObd is enabled, update B1 counter and continuous Mi 
 *                counter and process activation mode change
 *
 * \param[in]     EngineRunTimeTicks
 *                Current engine run time since engine start
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessEngineRunTime(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_IsEventAndActivationMode4Active
 *****************************************************************************/
/*!
 * \brief         Tests if with currently processed event activation mode 4 
 *                gets active
 *
 * \details       Tests if given event has active state and activation mode 4 
 *                is active
 *
 * \param[in]    EventId
 *               Unique handle of the event
 *
 * \return        TRUE: The given event is active and either this leads to 
 *                      activation mode 4 being active or activation mode 4 
 *                      is active because of the B1Counter exceeding 200h
 *                FALSE: - Event is not active or 
 *                       - Event is active but neither is of Severity Class A 
 *                         nor is the B1 counter exceeding 200h
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_IsEventAndActivationMode4Active(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_GetMILStatus
 *****************************************************************************/
/*!
 * \brief         Gets the current indicator status.
 *
 * \details       Gets the indicator status resulting from current Activation 
 *                Mode.
 *
 * \return        The current indicator status
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_GetMILStatus(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_RestoreCounters
 *****************************************************************************/
/*!
 * \brief         Restore WWHOBD counters
 *
 * \details       Restore WWHOBD counters during initialization
 *
 * \pre           May be called only during module initialization
 *                Must be called after Dem_GlobalDiagnostics_ActivationMode_Init
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_RestoreCounters(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ProcessWirEnabled
 *****************************************************************************/
/*!
 * \brief         Updates WWH OBD statistics on an event WIR bit ON trigger
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique identifier of the event   
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessWirEnabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ProcessWirDisabled
 *****************************************************************************/
/*!
 * \brief         Updates WWH OBD statistics on an event WIR bit OFF trigger
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique identifier of the event   
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessWirDisabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ProcessQualifiedActiveEvent
 *****************************************************************************/
/*!
 * \brief         Processes the increment of the qualified active event count
 *
 * \details       Increments of the qualified active event count and processes 
 *                a resulting transition to activation mode 4
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different EventId
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessQualifiedActiveEvent(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ProcessEventPassed
 *****************************************************************************/
/*!
 * \brief         Processes reset of TF bit
 *
 * \details       Updates qualified active event counts and processes 
 *                a resulting activation mode change
 *
 * \param[in]     EventContext
 *                Event processing context
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessEventPassed(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ProcessEventCleared
 *****************************************************************************/
/*!
 * \brief         Processes reset of Event status
 *
 * \details       Updates status related event counts and processes a 
 *                resulting activation mode change
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessEventCleared(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessHealing
 *****************************************************************************/
/*!
 * \brief         Process healing of B1 counter
 *
 * \details       -
 *
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessHealing(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessMalfunctionStatusAtOpCycleEnd
 *****************************************************************************/
/*!
 * \brief         Processes the status of a Class B1 malfunction at operation
 *                cycle end
 *
 * \details       Process the status of a WWH-OBD Class B1 malfunction at the
 *                end of the operation cycle to determine if
 *                - any Class B1 malfunction failed this DCY
 *                - all Class B1 malfunctions were tested this DCY.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_B1Counter_ProcessMalfunctionStatusAtOpCycleEnd(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_ActivationMode_ProcessAfterClearAll
 *****************************************************************************/
/*!
 * \brief         Reset activation mode data after clear all
 *
 * \details       Reset activation mode data after clear all
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_ActivationMode_ProcessAfterClearAll(
  void
);


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_ACTIVATIONMODE_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ActivationMode_Interface.h
 *********************************************************************************************************************/
