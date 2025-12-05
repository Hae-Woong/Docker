/**********************************************************************************************************************
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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!       \file  Can_30_Core_ExclusiveAreaHandler.c
 *        \brief  Can_30_Core_ExclusiveAreaHandler source file
 *        \details  Contains the implementation of the Exclusive area handler unit.
 *        \unit Can_30_Core_ExclusiveAreaHandler
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/
#define CAN_30_CORE_EXCLUSIVEAREAHANDLER_SOURCE
#define CAN_30_CORE_INTERNAL

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core.h"
#include "Can_30_Core_ExclusiveAreaHandler.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_EnterExclusiveArea()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_EnterExclusiveArea
  Relation_Context:
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_EnterExclusiveArea(uint8 area)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter the requested exclusive area */
  switch (area)
  {
  case CAN_30_CORE_EXCLUSIVE_AREA_1:
    SchM_Enter_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_1();
    break;
  case CAN_30_CORE_EXCLUSIVE_AREA_2:
    SchM_Enter_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_2();
    break;
  case CAN_30_CORE_EXCLUSIVE_AREA_4:
    SchM_Enter_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_4();
    break;
  case CAN_30_CORE_EXCLUSIVE_AREA_5:
    SchM_Enter_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_5();
    break;
  case CAN_30_CORE_EXCLUSIVE_AREA_6:
    SchM_Enter_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_6();
    break;
  case CAN_30_CORE_EXCLUSIVE_AREA_7:
    SchM_Enter_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_7();
    break;
  default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
    break;
  }
}

/**********************************************************************************************************************
 *  Can_30_Core_ExitExclusiveArea()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_ExitExclusiveArea
  Relation_Context:
  Relation:
  Parameter_PreCompile:
  Parameter_Data:
  Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 */
CAN_30_CORE_LOCAL_API FUNC(void, CAN_30_CORE_CODE) Can_30_Core_ExitExclusiveArea(uint8 area)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Exit the requested exclusive area */
  switch (area)
  {
  case CAN_30_CORE_EXCLUSIVE_AREA_1:
    SchM_Exit_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_1();
    break;
  case CAN_30_CORE_EXCLUSIVE_AREA_2:
    SchM_Exit_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_2();
    break;
  case CAN_30_CORE_EXCLUSIVE_AREA_4:
    SchM_Exit_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_4();
    break;
  case CAN_30_CORE_EXCLUSIVE_AREA_5:
    SchM_Exit_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_5();
    break;
  case CAN_30_CORE_EXCLUSIVE_AREA_6:
    SchM_Exit_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_6();
    break;
  case CAN_30_CORE_EXCLUSIVE_AREA_7:
    SchM_Exit_Can_30_Core_CAN_30_CORE_EXCLUSIVE_AREA_7();
    break;
  default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
    break;
  }
}

#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_ExclusiveAreaHandler.c
 *********************************************************************************************************************/
