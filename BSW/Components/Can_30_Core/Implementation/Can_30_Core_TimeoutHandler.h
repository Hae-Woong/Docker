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
/*!       \file  Can_30_Core_TimeoutHandler.h
 *        \brief  Can_30_Core_TimeoutHandler header file
 *        \details  Header of the Timeout Handler unit. It provides the API declaration for this unit used by other units.
 *        \unit Can_30_Core_TimeoutHandler
 *********************************************************************************************************************/
#if !defined(CAN_30_CORE_TIMEOUTHANDLER_H)
#define CAN_30_CORE_TIMEOUTHANDLER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Os.h"
#include "Can_30_Core_PrivateTypes.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Timer type */
typedef enum
{
  CAN_30_CORE_TIMER_START,
  CAN_30_CORE_TIMER_END
} Can_30_Core_TimerUpdateType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif /* CAN_30_CORE_TIMEOUTHANDLER_H */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_TimeoutHandler.h
 *********************************************************************************************************************/
