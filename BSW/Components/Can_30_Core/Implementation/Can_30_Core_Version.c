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
/*!       \file  Can_30_Core_Version.c
 *        \brief  Can_30_Core_Version source file
 *        \details  Contains the implementation of the Version unit.
 *        \unit Can_30_Core_Version
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
#define CAN_30_CORE_VERSION_SOURCE
#define CAN_30_CORE_INTERNAL

/**********************************************************************************************************************
 * LOCAL MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2880, 2991, 2992, 2994, 2995, 2996 EOF */ /* MD_Can_30_Core_ConstValue */
/* PRQA  S 2741, 2742 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Can_30_Core.h"
#include "Can_30_Core_vCan.h"
#include "Can_30_Core_Version.h"
#include "Can_30_Core_DiagnosticHandler.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define CAN_30_CORE_START_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Can_30_Core_GetVersionInfo()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Can_30_Core_GetVersionInfo
  Relation_Context:
DevErrorDetect
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
 *
 */
FUNC(void, CAN_30_CORE_CODE)
Can_30_Core_GetVersionInfo(Can_30_Core_VersionInfoPtrType VersionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_30_Core_ErrorId errorId = CAN_30_CORE_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if VersionInfo is a null pointer */
  if (Can_30_Core_DevCheck_IsConditionTrue(VersionInfo == NULL_PTR))
  {
    errorId = CAN_30_CORE_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Copy version info to given pointer parameter */
    VersionInfo->vendorID = CAN_30_CORE_VENDOR_ID; /* VCA_CAN_30_CORE_VERSIONINFO */
    VersionInfo->moduleID = CAN_30_CORE_MODULE_ID; /* VCA_CAN_30_CORE_VERSIONINFO */
    VersionInfo->sw_major_version = (uint8)CAN_30_CORE_SW_MAJOR_VERSION; /* VCA_CAN_30_CORE_VERSIONINFO */
    VersionInfo->sw_minor_version = (uint8)CAN_30_CORE_SW_MINOR_VERSION; /* VCA_CAN_30_CORE_VERSIONINFO */
    VersionInfo->sw_patch_version = (uint8)CAN_30_CORE_SW_PATCH_VERSION; /* VCA_CAN_30_CORE_VERSIONINFO */
  }
  /* ----- Development Error Report --------------------------------------- */
  Can_30_Core_ReportDevelopmentError(CAN_30_CORE_SID_VERSION, errorId);
}
#define CAN_30_CORE_STOP_SEC_CODE
#include "Can_30_Core_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Can_30_Core_Version.c
 *********************************************************************************************************************/
