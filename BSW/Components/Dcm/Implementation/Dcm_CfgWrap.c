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
/**        \file  Dcm_CfgWrap.c
 *         \unit  CfgWrap
 *        \brief  Contains the implementation of Configuration Wrapper unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#define DCM_CFGWRAP_SOURCE

#ifdef __PRQA__                                                                                                                                      /* COV_DCM_PRQA_UNREACHABLE XF */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetReqData" 2985 /* MD_Dcm_Redundant_2985 */
# pragma PRQA_MACRO_MESSAGES_OFF "Dcm_UtiGetResData" 2985 /* MD_Dcm_Redundant_2985 */
                                                                                                                                                     /* PRQA S 2991 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2992 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2995 EOF */ /* MD_Dcm_ConstExpr */
                                                                                                                                                     /* PRQA S 2996 EOF */ /* MD_Dcm_ConstExpr */
#endif
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_CfgWrap.h"
/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#if (DCM_SVC_27_PB_ENABLED == STD_ON)
# define Dcm_CfgWrapSvc27Indirection(secLev)                         (Dcm_CfgWrapSingletonContext.DiagVariant->Svc27IndirectionTable[(secLev)])
#else
# define Dcm_CfgWrapSvc27Indirection(secLev)                         (secLev)
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if (DCM_SVC_27_PB_ENABLED == STD_ON)
/*! Singleton context of CfgWrap unit */
struct DCM_CFGWRAPCONTEXTTYPE_TAG
{
  Dcm_CfgVarMgrDiagVariantType DiagVariant;
};
typedef struct DCM_CFGWRAPCONTEXTTYPE_TAG Dcm_CfgWrapSingletonContextType;
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_SVC_27_PB_ENABLED == STD_ON)
/*! Singleton context of Net unit */
DCM_LOCAL VAR(Dcm_CfgWrapSingletonContextType, DCM_VAR_NOINIT) Dcm_CfgWrapSingletonContext;
#endif
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Dcm_CfgWrapInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_CfgWrapInit(
  Dcm_ConfigPtrType configPtr
  )
{
  DCM_IGNORE_UNREF_PARAM(configPtr);

#if (DCM_SVC_27_PB_ENABLED == STD_ON)
  Dcm_CfgWrapSingletonContext.DiagVariant = &(configPtr->DiagVariant);
#endif
}

#if (DCM_SVC_27_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_CfgWrapSvc27SecLevelInfoGetEntry()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_CfgWrapSvc27SecLevelInfoGetEntry(                                                                                 /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  uint8_least secLev,
  CONSTP2VAR(Dcm_CfgSvc27SecLevelInfoPtrType, AUTOMATIC, AUTOMATIC) pSecLevelInfo,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lResult;
  uint8_least    lIndex = Dcm_CfgWrapSvc27Indirection(secLev);

  if (Dcm_DebugDetectRuntimeError(lIndex >= DCM_CFGSVC27SECLEVELINFO_SIZE))                                                                          /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);

    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_NOT_OK;
  }
  else
  {
    *pSecLevelInfo = &Dcm_CfgSvc27SecLevelInfo[lIndex];                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_OK;
  }

  return lResult;
}
#endif

#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_CfgWrap.c
 *********************************************************************************************************************/
