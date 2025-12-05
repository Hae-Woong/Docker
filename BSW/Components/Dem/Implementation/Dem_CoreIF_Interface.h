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
/*! \ingroup    Dem_Master | Dem_Satellite
 *  \defgroup   Dem_CoreIF CoreIF
 *  \addtogroup Dem_CoreIF
 *  \{
 *  \file       Dem_CoreIF_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Public interface of the logical unit Core
 *  \unit       CoreIF
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_COREIF_INTERFACE_H_)
#define DEM_COREIF_INTERFACE_H_

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_CoreIF_Types.h"
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_CoreIF_Properties Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Core_GetMaxFdcToc
 *****************************************************************************/
/*!
 * \brief         Returns the 'Maximum FDC This Operation Cycle' of the event.
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * 
 * \pre           -
 *
 * \context       ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(sint8, DEM_CODE)
Dem_Core_GetMaxFdcToc(
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
 * \defgroup Dem_CoreIF_Public Public Methods
 * \{
 */
 
/* ****************************************************************************
 * Dem_Core_SetMonitorDisabledTOC
 *****************************************************************************/
/*!
 * \brief         Disables a monitor for the current operation cycle.
 *
 * \details       Disables a monitor for the current operation cycle by setting
 *                MonitorDisabledTOC in the event extended status.
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_SetMonitorDisabledTOC(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_ResetMonitorDisabledTOC
 *****************************************************************************/
/*!
 * \brief         Resets disabled flag of a monitor for the current operation cycle.
 *
 * \details       Resets disabled flag of a monitor for the current operation cycle by
 *                resetting MonitorDisabledTOC in the event extended status.
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Core_ResetMonitorDisabledTOC(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_TestEventExtendedStatusBit
 *****************************************************************************/
/*!
 * \brief         Tests whether a single bit is set in the extended status
 *                of an event.
 *
 * \details       -
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 * 
 * \return        TRUE
 *                Bit is set in the extended status byte.
 * \return        FALSE
 *                Bit is not set in the extended status byte.
 *
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestEventExtendedStatusBit(
  Dem_EventIdType EventId,
  Dem_ExtendedStatus_ExtendedStatusBitsType StatusBit
);

/* ****************************************************************************
 * Dem_Core_TestInternalEventUdsStatusBit
 *****************************************************************************/
/*!
 * \brief         Tests whether a single bit is set in the internal UDS status
 *                of an event.
 *
 * \details       -
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 * 
 * \return        TRUE
 *                Bit is set in the internal UDS status byte.
 * \return        FALSE
 *                Bit is not set in the internal UDS status byte.
 *  
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestInternalEventUdsStatusBit(
  Dem_EventIdType EventId,
  Dem_UDSStatus_UDSStatusBitsType StatusBit
);

/* ****************************************************************************
 * Dem_Core_TestExternalEventUdsStatusBit
 *****************************************************************************/
/*!
 * \brief         Tests whether a single bit is set in the external UDS status
 *                of an event.
 *
 * \details       -
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 * 
 * \return        TRUE
 *                Bit is set in the external UDS status byte.
 * \return        FALSE
 *                Bit is not set in the external UDS status byte.
 *  
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestExternalEventUdsStatusBit(
  Dem_EventIdType EventId,
  Dem_UDSStatus_UDSStatusBitsType StatusBit
);

/* ****************************************************************************
 * Dem_Core_TestInternalDTCUdsStatusBit
 *****************************************************************************/
/*!
 * \brief         Tests whether a single bit is set in the internal UDS status
 *                of a DTC.
 *
 * \details       -
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 * 
 * \return        TRUE
 *                Bit is set in the internal UDS status byte.
 * \return        FALSE
 *                Bit is not set in the internal UDS status byte.
 *  
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestInternalDTCUdsStatusBit(
  Dem_EventIdType EventId,
  Dem_UDSStatus_UDSStatusBitsType StatusBit
);

/* ****************************************************************************
 * Dem_Core_TestExternalDTCUdsStatusBit
 *****************************************************************************/
/*!
 * \brief         Tests whether a single bit is set in the external UDS status
 *                of a DTC.
 *
 * \details       -
 * 
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           -
 * 
 * \return        TRUE
 *                Bit is set in the external DTC UDS status byte.
 * \return        FALSE
 *                Bit is not set in the external DTC UDS status byte.
 *  
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestExternalDTCUdsStatusBit(
  Dem_EventIdType EventId,
  Dem_UDSStatus_UDSStatusBitsType StatusBit
);

/* ****************************************************************************
 * Dem_Core_TestCustomTriggeredSrecConfigured
 *****************************************************************************/
/*!
 * \brief         Test whether event has custom trigger snapshot record configured.
 *
 * \details       Test whether event has custom trigger snapshot record  configured.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        True if custom snapshot recrod is configured for event
 *                False, otherwise.
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestCustomTriggeredSrecConfigured(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_TestCustomTriggeredTssrConfigured
 *****************************************************************************/
/*!
 * \brief         Test whether event has custom TSS record configured.
 *
 * \details       Test whether event has custom TSS record configured.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        True if custom snapshot recrod is configured for event
 *                False, otherwise.
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestCustomTriggeredTssrConfigured(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Core_TestCustomTriggerConfigured
 *****************************************************************************/
/*!
 * \brief         Test whether event has custom trigger snapshot data configured.
 *
 * \details       Test whether event has custom trigger snapshot data configured.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \return        True if any custom trigger snapshot data is configured for event
 *                False, otherwise.
 * 
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Core_TestCustomTriggerConfigured(
  Dem_EventIdType EventId
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */


#endif /* DEM_COREIF_INTERFACE_H_ */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_CoreIF_Interface.h
 *********************************************************************************************************************/
