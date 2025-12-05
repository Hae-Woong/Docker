/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file  Can_30_Core_ModeHandler.h
 *        \brief  Can_30_Core_ModeHandler header file
 *        \details  Header of the Mode handler unit. It provides the API declaration for this unit used by other units.
 *        \unit Can_30_Core_ModeHandler
 *********************************************************************************************************************/
#if !defined(CAN_30_CORE_MODEHANDLER_H)
#define CAN_30_CORE_MODEHANDLER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core_PrivateTypes.h"
#include "CanIf.h"
#include "CanIf_Cbk.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Precondition for mode transition */
#define CAN_30_CORE_SUPRESSRAMCHECK FALSE
#define CAN_30_CORE_EXECUTERAMCHECK TRUE

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_SetWakeupEvent()
 *********************************************************************************************************************/
/*! \brief       Set the wakeup event
 *  \details     Sets the wakeup status for the requested controller
 *  \param[in]   Controller    The requested controller
 *  \param[in]   State         The requested state
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_SetWakeupEvent(uint8 Controller, boolean State);

/**********************************************************************************************************************
 *  Can_30_Core_NotifyTransition()
 *********************************************************************************************************************/
/*! \brief       Mode transistion notification
 *  \details     Notifies the mode transition to the upper layer (CanIf) and sets the controller log status.
 *  \param[in]   Controller        The requested controller
 *  \param[in]   CanState           The requested state (CAN)
 *  \param[in]   CanIfState         The requested state (CANIF)
 *  \param[in]   TransitionState    The state of the mode transition
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_NotifyTransition(
  uint8 Controller,
  uint8 CanState,
  CanIf_ControllerModeType CanIfState,
  Can_ReturnType TransitionState);

/**********************************************************************************************************************
 *  Can_30_Core_IsControllerInStart()
 *********************************************************************************************************************/
/*! \brief       Start mode observation
 *  \details     Checks if the requested controller is in start mode
 *  \param[in]   Controller    The requested controller
 *  \return      FALSE         The controller is not in start mode
 *  \return      TRUE          The controller is in start mode
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsControllerInStart(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_IsControllerInStop()
 *********************************************************************************************************************/
/*! \brief       Stop mode observation
 *  \details     Checks if the requested controller is in start mode
 *  \param[in]   Controller    The requested controller
 *  \return      FALSE         The controller is not in stop mode
 *  \return      TRUE          The controller is in stop mode
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsControllerInStop(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_IsControllerInSleep()
 *********************************************************************************************************************/
/*! \brief       Sleep mode observation
 *  \details     Checks if the requested controller is in sleep mode
 *  \param[in]   Controller    The requested controller
 *  \return      FALSE         The controller is not in sleep mode
 *  \return      TRUE          The controller is in sleep mode
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsControllerInSleep(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_IsControllerDeactivated()
 *********************************************************************************************************************/
/*! \brief       Controller activation observation
 *  \details     Checks if the requested controller is activated
 *  \param[in]   Controller    The requested controller
 *  \return      FALSE         The controller is not deactivated
 *  \return      TRUE          The controller is deactivated
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(boolean, CAN_30_CORE_CODE) Can_30_Core_IsControllerDeactivated(uint8 Controller);

/**********************************************************************************************************************
 *  Can_30_Core_ModeHandlerInit()
 *********************************************************************************************************************/
/*! \brief       Initialize the mode handler
 *  \details     Initializes the mode handler to default init modes
 *  \param[in]   -
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_ModeHandlerInit(void);

/**********************************************************************************************************************
 *  Can_30_Core_AppendLogStatus()
 *********************************************************************************************************************/
/*! \brief       Append log status
 *  \details     Appends a status to the log status
 *  \param[in]   Controller    The requested controller
 *  \param[in]   Status        The state to be appended
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_AppendLogStatus(uint8 Controller, uint8 Status);

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CAN_30_CORE_MODEHANDLER_H */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_ModeHandler.h
 *********************************************************************************************************************/
