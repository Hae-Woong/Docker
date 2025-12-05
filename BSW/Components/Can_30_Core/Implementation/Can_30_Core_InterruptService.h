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
/*!       \file  Can_30_Core_InterruptService.h
 *        \brief  Can_30_Core_InterruptService header file
 *        \details  Header of the Interrupt service unit. It provides the API declaration for this unit used by other units.
 *        \unit Can_30_Core_InterruptService
 *********************************************************************************************************************/
#if !defined(CAN_30_CORE_INTERRUPTSERVICE_H)
#define CAN_30_CORE_INTERRUPTSERVICE_H

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
 *  Can_30_Core_InterruptServiceInit()
 *********************************************************************************************************************/
/*! \brief       Initializes the interrupt service unit
 *  \details     Initializes the interrupt nesting counters for all controllers.
 *  \param[in]   -
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_InterruptServiceInit(void);

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CAN_30_CORE_INTERRUPTSERVICE_H */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_InterruptService.h
 *********************************************************************************************************************/
