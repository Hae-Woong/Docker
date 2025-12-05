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
/*!       \file  Can_30_Core_Baudrate.h
 *        \brief  Can_30_Core_Baudrate header file
 *        \details  Header of the Baudrate unit. It provides the API declaration for this unit used by other units.
 *        \unit Can_30_Core_Baudrate
 *********************************************************************************************************************/
#if !defined(CAN_30_CORE_BAUDRATE_H)
#define CAN_30_CORE_BAUDRATE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core_PrivateTypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define CAN_30_CORE_STOP_SEC_CODE
# include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif /* CAN_30_CORE_BAUDRATE_H */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_Baudrate.h
 *********************************************************************************************************************/
