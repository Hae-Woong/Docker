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
 *  \defgroup   Dem_Indicator Indicator
 *  \{
 *  \file       Dem_Indicator_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Manages the User and Special indicator states
 *  \entity     Indicator
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_INDICATOR_INTERFACE_H)
#define DEM_INDICATOR_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Indicator_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Indicator_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId
 *****************************************************************************/
/*!
 * \brief         Get the unique handle of indicator 'Malfunction Indicator Lamp'
 *
 * \details       Get the unique handle of indicator 'Malfunction Indicator Lamp'
 *
 * \return        Unique handle of the indicator.
 *                DEM_CFG_GLOBAL_INDICATOR_COUNT if no such indicator is configured
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetGlobalMilIndicatorId(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_IsGlobalMilStateSupported
 *****************************************************************************/
/*!
 * \brief         Checks whether global Mil state is supported.
 *
 * \details       Checks whether global Mil state is supported.
 *
 * \return        TRUE
 *                Global Mil state is supported.
 *                FALSE
 *                Global Mil state is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IsGlobalMilStateSupported(
  void
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_IsIndicatorIdValid
 *****************************************************************************/
/*!
 * \brief         Test if the Indicator Id is valid
 *
 * \details       Test if the Indicator Id is in range [0,DEM_CFG_GLOBAL_INDICATOR_COUNT[
 *
 * \return        TRUE: Indicator Id is valid
 *                FALSE: Indicator Id is invalid
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_IsIndicatorIdValid(
  uint8 IndicatorId
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
 * \defgroup Dem_Indicator_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_J1939_CalculateLampStatusOfEvent
 *****************************************************************************/
/*!
 * \brief         Calculate the event specific J1939 lamp status.
 *
 * \details       Calculate the event specific J1939 lamp status.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        Global J1939 lamp status for the requested event id.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_J1939DcmLampStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_CalculateLampStatusOfEvent(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeRelatedLampStatus
 *****************************************************************************/
/*!
 * \brief         Calculate the node specific J1939 lamp status and return it.
 *
 * \details       Calculate the node specific J1939 lamp status based on red stop lamp,
 *                amber warning lamp, malfunction indicator lamp and the protect lamp.
 *
 * \param[in]     NodeIndex
 *                The node Id.
 * \return        The node specific J1939 lamp status.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_J1939DcmLampStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_J1939_CalculateNodeRelatedLampStatus(
  uint8 NodeIndex
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_ProcessEventWirEnabled
 *****************************************************************************/
/*!
 * \brief         Updates indicator states on an event WIR bit ON trigger
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique identifier of the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_ProcessEventWirEnabled(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_ProcessEventWirDisabled
 *****************************************************************************/
/*!
 * \brief         Updates indicator states on an event WIR bit OFF trigger
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
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_ProcessEventWirDisabled(
  Dem_EventIdType EventId 
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_GetIndicatorState
 *****************************************************************************/
/*!
 * \brief         Gets the indicator state for a given J1939 Node or all Nodes
 *
 * \details       -
 *
 * \param[in]     IndicatorId
 *                Unique handle of the indicator
 *
 * \param[in]     NodeIndex
 *                Index of the J1939 Node
 *
 * \param[in]     NodeIndependent
 *                TRUE: NodeIndex is not evaluated
 *                FALSE: NodeIndex is evaluated
 *
 * \return        The indicator state
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_GetIndicatorState(
  uint8 IndicatorId,
  uint8 NodeIndex,
  boolean NodeIndependent
);

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_TestEventHasMilAssigned
 *****************************************************************************/
/*!
 * \brief         Returns if the event has the Mil indicator assigned.
 *
 * \details       Returns if the event has the Mil indicator assigned.
 *
 * \param[in]     EventId
 *                Unique handle of the event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        TRUE
 *                The MIL is supported.
 *                FALSE
 *                The MIL is not supported.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_TestEventHasMilAssigned(
  Dem_EventIdType EventId
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_Init
 *****************************************************************************/
/*!
 * \brief         Initialize indicator state based on event state.
 *
 * \details       If indicators are configured, initialize all indicator state
 *                counters based on the status of all events available in this
 *                variant.
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_Init(
  void
  );

/* ****************************************************************************
 * Dem_GlobalDiagnostics_Indicator_TestMilReportedActive
 *****************************************************************************/
/*!
 * \brief         Returns if the MIL is considered as active.
 *
 * \details       The MIL is always considered as active (for reporting in 
 *                PID 0x01, PID 0x21 and PID 0x4D), if it is continuous (or 
 *                continuous blinking). 
 *                In configurations with 'Report Blinking Mil As Active' 
 *                enabled, the Mil is also considered as active if it is only
 *                blinking or flashing. 
 *
 * \return        TRUE
 *                The MIL is considered as active.
 * \return        FALSE
 *                The MIL is not considered as active
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalDiagnostics_Indicator_TestMilReportedActive(
  void
  );


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_INDICATOR_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Indicator_Interface.h
 *********************************************************************************************************************/
