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
 *  \defgroup   Dem_ExtendedStatus ExtendedStatus
 *  \{
 *  \file       Dem_ExtendedStatus_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *  \entity     ExtendedStatus
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_EXTENDEDSTATUS_INTERFACE_H)
#define DEM_EXTENDEDSTATUS_INTERFACE_H

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
#include "Dem_ExtendedStatus_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_ExtendedStatus_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Event_ExtendedStatus_GetEventStatus
 *****************************************************************************/
/*!
 * \brief         Get current extended status of the event
 *
 * \details       Get current extended status of the event
 *
 * \param[in]     EventId
 *                Unique handle of the event
 *
 * \return        Current extended status of the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_ExtendedStatus_StatusType, DEM_CODE)
Dem_Event_ExtendedStatus_GetEventStatus(
  Dem_EventIdType EventId
);

/* ****************************************************************************
 * Dem_Event_ExtendedStatus_SetEventStatus
 *****************************************************************************/
/*!
 * \brief         Set current extended status value of the event
 *
 * \details       Set current extended status value of the event
 *
 * \param[in]     EventId
 *                Unique handle of the event
 * \param[in]     Status
 *                New extended status of the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ExtendedStatus_SetEventStatus(
  Dem_EventIdType EventId,
  Dem_ExtendedStatus_StatusType Status
);

/* ****************************************************************************
 * Dem_Event_ExtendedStatus_SetBit
 *****************************************************************************/
/*!
 * \brief         Sets a single bit in the event extended status.
 *
 * \details       -
 *
 * \param[in]     ExtendedStatus
 *                Extended status byte
 * \param[in]     BitIndex
 *                Bit index to set in the status byte
 *
 * \return        Internal status byte with the set bit value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_ExtendedStatus_StatusType, DEM_CODE)
Dem_Event_ExtendedStatus_SetBit(
  Dem_ExtendedStatus_StatusType ExtendedStatus,
  Dem_ExtendedStatus_ExtendedStatusBitsType BitIndex
);

/* ****************************************************************************
 * Dem_Event_ExtendedStatus_ResetBit
 *****************************************************************************/
/*!
 * \brief         Resets a single bit in the event extended status.
 *
 * \details       -
 *
 * \param[in]     ExtendedStatus
 *                Extended status byte
 * \param[in]     BitIndex
 *                Bit index to set in the status byte
 *
 * \return        Internal status byte with the reset bit value
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_ExtendedStatus_StatusType, DEM_CODE)
Dem_Event_ExtendedStatus_ResetBit(
  Dem_ExtendedStatus_StatusType ExtendedStatus,
  Dem_ExtendedStatus_ExtendedStatusBitsType BitIndex
);

/* ****************************************************************************
 * Dem_Event_ExtendedStatus_TestBit
 *****************************************************************************/
/*!
 * \brief         Tests whether a single bit in the event extended status is set.
 *
 * \details       -
 *
 * \param[in]     ExtendedStatus
 *                Extended status byte
 * \param[in]     BitIndex
 *                Bit index to test in the status byte
 *
 * \return        TRUE
 *                Bit is set
 * \return        FALSE
 *                Bit is not set
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Event_ExtendedStatus_TestBit(
  Dem_ExtendedStatus_StatusType ExtendedStatus,
  Dem_ExtendedStatus_ExtendedStatusBitsType BitIndex
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
 * \defgroup Dem_ExtendedStatus_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Event_ExtendedStatus_ProcessPassedEffectsTPSLC
 *****************************************************************************/
 /*!
 * \brief         Processes DTCs that are relevant for Readiness Completion calculation
 *
 * \details       Extended Status of DTC is set to 'passed since last clear'.
 *                For DTC with combined group status it is only set if all
 *                subevents are tested this cycle and no subevent has status
 *                failed.
 *
 * \param[in]     EventContext
 *                Event processing context
 *
 * \pre           -
 * \config        Feature Major Monitors and PID 01 is enabled
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE for different DTCContext
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Event_ExtendedStatus_ProcessPassedEffectsTPSLC(
  CONSTP2CONST(Dem_Event_ContextType, AUTOMATIC, AUTOMATIC) EventContext
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "Dem_MemMap.h"                                                                                                          /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_EXTENDEDSTATUS_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ExtendedStatus_Interface.h
 *********************************************************************************************************************/
