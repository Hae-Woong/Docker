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
/*!       \file  Can_30_Core_ModeRequest.h
 *        \brief  Can_30_Core_ModeRequest header file
 *        \details  Header of the Mode request unit. It provides the API declaration for this unit used by other units.
 *        \unit Can_30_Core_ModeRequest
 *********************************************************************************************************************/
#if !defined(CAN_30_CORE_MODEREQUEST_H)
#define CAN_30_CORE_MODEREQUEST_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core_PrivateTypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_ModeTransition()
 *********************************************************************************************************************/
/*! \brief       Request a mode transistion
 *  \details     Handle/Request transition and wait asynchronous-short time.
 *  \param[in]   Controller             The requested controller
 *  \param[in]   TransitionRequest      The requested mode
 *  \param[in]   ContinueBusOffRecovery TRUE/FALSE if the Bus off handling shall be continued
 *  \param[in]   DoRamCheck             TRUE/FALSE if the RAM check should be executed
 *  \return      CAN_NOT_OK             Transition rejected
 *  \return      CAN_OK                 Transition finished
 *  \return      CAN_REQUESTED          Transistion is requested but ongoing.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(Can_ReturnType, CAN_30_CORE_CODE) Can_30_Core_ModeTransition(
  uint8 Controller,
  uint8 TransitionRequest,
  boolean ContinueBusOffRecovery,
  boolean DoRamCheck);

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CAN_30_CORE_MODEREQUEST_H */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_ModeRequest.h
 *********************************************************************************************************************/
