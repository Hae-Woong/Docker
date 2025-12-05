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
/*!       \file  Can_30_Core_ExclusiveAreaHandler.h
 *        \brief  Can_30_Core_ExclusiveAreaHandler header file
 *        \details  Header of the Exclusive area handler unit. It provides the API declaration for this unit used by other units.
 *        \unit Can_30_Core_ExclusiveAreaHandler
 *********************************************************************************************************************/
#if !defined(CAN_30_CORE_EXCLUSIVEAREAHANDLER_H)
#define CAN_30_CORE_EXCLUSIVEAREAHANDLER_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core_PrivateTypes.h"
#include "SchM_Can_30_Core.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ----- Exclusive Areas ----- */
#define CAN_30_CORE_EXCLUSIVE_AREA_1 (0x01)
#define CAN_30_CORE_EXCLUSIVE_AREA_2 (0x02)
#define CAN_30_CORE_EXCLUSIVE_AREA_4 (0x04)
#define CAN_30_CORE_EXCLUSIVE_AREA_5 (0x05)
#define CAN_30_CORE_EXCLUSIVE_AREA_6 (0x06)
#define CAN_30_CORE_EXCLUSIVE_AREA_7 (0x07)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_EnterExclusiveArea()
 *********************************************************************************************************************/
/*! \brief       Enter Can exclusive area
 *  \details     Enters the requested exclusive area
 *  \param[in]   area    Requested exclusive  area
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_EnterExclusiveArea(uint8 area);

/**********************************************************************************************************************
 *  Can_30_Core_ExitExclusiveArea()
 *********************************************************************************************************************/
/*! \brief       Exit Can exclusive area
 *  \details     Exits the requested exclusive area
 *  \param[in]   area    Requested exclusive  area
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_ExitExclusiveArea(uint8 area);

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* CAN_30_CORE_EXCLUSIVEAREAHANDLER_H */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_ExclusiveAreaHandler.h
 *********************************************************************************************************************/
