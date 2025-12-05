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
/**        \file  Dcm_VarMgr.c
 *         \unit  VarMgr
 *        \brief  Contains the implementation of Variant Manager unit.
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
#define DCM_VARMGR_SOURCE

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
#include "Dcm_VarMgr.h"
#include "Dcm_Debug.h"
#include "Dcm_Uti.h"

#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Variant bitmap access */
#define Dcm_VarMgrBitSetCalcSize(bits)                               (Dcm_UtiBitSetCalcSize(Dcm_CfgVarMgrBitSetBaseType, (bits)))                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_VarMgrBitSetTestBit(bitSet, bit)                         (Dcm_UtiBitSetTestBit(Dcm_CfgVarMgrBitSetBaseType, (bitSet), (bit)))            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_VarMgrBitSetSetBit(bitSet, bit)                          (Dcm_UtiBitSetSetBit(Dcm_CfgVarMgrBitSetBaseType, (bitSet), (bit)))             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define Dcm_VarMgrBitSetClrBit(bitSet, bit)                          (Dcm_UtiBitSetClrBit(Dcm_CfgVarMgrBitSetBaseType, (bitSet), (bit)))             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/*! EcuM notification for run-time variant initialization errors */
#if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
# define Dcm_EcuMErrorNotification(errorCode)                        (EcuM_BswErrorHook((uint16) DCM_MODULE_ID, (uint8) (errorCode)))                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
# define Dcm_EcuMErrorNotification(errorCode)                        /* not used */
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
/*! Package with current communication and diagnostic variant selection (cached variant references) */
struct DCM_VARMGRSINGLETONCONTEXTTYPE_TAG
{
  Dcm_CfgVarMgrBitSetBaseType        ActiveCfgVariants[Dcm_VarMgrBitSetCalcSize(DCM_VARMGR_NUM_CFG_VARIANTS)];  /*!< Contains all active CFG-criteria IDs */ /* PRQA S 1039 */ /* MD_Dcm_Rule1.2_1039 */
# if (DCM_VARMGR_MULTI_SVC_EXCLUSIVE_ENABLED == STD_ON)
  Dcm_CfgVarMgrDiagVariantIdMemType  ActiveCfgVariantId;                                                        /*!< Contains the active CFG-criteria ID */
# endif
};
typedef struct DCM_VARMGRSINGLETONCONTEXTTYPE_TAG Dcm_VarMgrSingletonContextType;
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VarMgrVsgSetSingle()
 *********************************************************************************************************************/
/*! \brief          Sets the status of a single VSG.
 *  \details        -
 *  \param[in]      VsgId         Unique handle of the VSG.
 *  \param[in]      State         DCM_VSG_ENABLED:  Set VSG to active
 *                                DCM_VSG_DISABLED: Set VSG to inactive
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_VarMgrVsgSetSingle(
  Dcm_VsgIdentifierType VsgId,
  Dcm_VsgStateType State
  );
#endif

#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VarMgrVsgIsActive()
 *********************************************************************************************************************/
/*! \brief          Returns status of a single VSG.
 *  \details        -
 *  \param[in]      VsgId         Unique handle of the VSG.
 *  \param[out]     State         DCM_VSG_ENABLED:  VSG is active
 *                                DCM_VSG_DISABLED: VSG is inactive
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
*********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_VarMgrVsgIsActive(
  Dcm_VsgIdentifierType VsgId,
  P2VAR(Dcm_VsgStateType, AUTOMATIC, DCM_APPL_DATA) State
  );
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
/*! Singleton context of VarMgr unit */
DCM_LOCAL VAR(Dcm_VarMgrSingletonContextType, DCM_VAR_NOINIT) Dcm_VarMgrSingletonContext;
#endif
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VarMgrVsgSetSingle()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_VarMgrVsgSetSingle(
  Dcm_VsgIdentifierType VsgId,
  Dcm_VsgStateType State
  )
{
  if(State == DCM_VSG_ENABLED)
  {
    Dcm_VarMgrBitSetSetBit(Dcm_VarMgrSingletonContext.ActiveCfgVariants, VsgId);                                                                     /* SBSW_DCM_POINTER_WRITE_BITSET */
  } /* else State == DCM_VSG_DISABLED */
  else
  {
    Dcm_VarMgrBitSetClrBit(Dcm_VarMgrSingletonContext.ActiveCfgVariants, VsgId);                                                                     /* SBSW_DCM_POINTER_WRITE_BITSET */
  }
}
#endif

#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VarMgrVsgIsActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_VarMgrVsgIsActive(
  Dcm_VsgIdentifierType VsgId,
  P2VAR(Dcm_VsgStateType, AUTOMATIC, DCM_APPL_DATA) State
  )
{
  /* If Bit assigned to VsgId is set */
  if ((boolean)Dcm_VarMgrBitSetTestBit(Dcm_VarMgrSingletonContext.ActiveCfgVariants, VsgId) == TRUE)                                                 /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  {
    /* Set out parameter to enabled */
    *State = DCM_VSG_ENABLED;                                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  /* If Bit assigned to VsgId is cleared */
  else
  {
    /* Set out parameter to disabled */
    *State = DCM_VSG_DISABLED;                                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VarMgrInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_VarMgrInit(
  Dcm_ConfigPtrType configPtr
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;
  boolean lResult;

# if (DCM_VARMGR_MODE_POSTBUILD_ANY_ENABLED == STD_ON)
  /* Check plausibility of the configuration pointer parameter */
  if (configPtr == NULL_PTR)                                                                                                                         /* COV_DCM_RTM_UNIT_TEST XF */
  {
    Dcm_EcuMErrorNotification(ECUM_BSWERROR_NULLPTR);
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
#  if (DCM_VARMGR_MODE_POSTBUILD_LOAD_ENABLED == STD_ON)
  if (configPtr->FinalMagicNumber != DCM_CFG_FINAL_MAGIC_NUMBER)                                                                                     /* COV_DCM_RTM_UNIT_TEST XF */
  {
    Dcm_EcuMErrorNotification(ECUM_BSWERROR_MAGICNUMBER);
    lErrorId = DCM_E_INVALID_CONFIG;
  }
  else if (configPtr->GenOutputType != DCM_CFG_GEN_COMPATIBILITY_VERSION)                                                                            /* COV_DCM_RTM_UNIT_TEST XF */
  {
    Dcm_EcuMErrorNotification(ECUM_BSWERROR_COMPATIBILITYVERSION);
    lErrorId = DCM_E_INVALID_CONFIG;
  }
  else
#  endif
# endif
  {
# if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
    Dcm_CfgVarMgrDiagVariantIdOptType lVariantIter;

    /* Deactivate all Variants in ActiveCfgVariant Bitmap */
    for (lVariantIter = 0; lVariantIter < Dcm_VarMgrBitSetCalcSize(DCM_VARMGR_NUM_CFG_VARIANTS); ++lVariantIter)
    {
      Dcm_VarMgrSingletonContext.ActiveCfgVariants[lVariantIter] = 0u;                                                                               /* SBSW_DCM_POINTER_WRITE_VSGACTIVECFGVARIANTS */
    }

#  if (DCM_VARMGR_MULTI_SVC_EXCLUSIVE_ENABLED  == STD_ON)
    Dcm_VarMgrSingletonContext.ActiveCfgVariantId = configPtr->DiagVariant.Id;
    Dcm_VarMgrBitSetSetBit(Dcm_VarMgrSingletonContext.ActiveCfgVariants, configPtr->DiagVariant.Id);                                                 /* SBSW_DCM_POINTER_WRITE_BITSET */
#  else
    Dcm_VarMgrBitSetSetBit(Dcm_VarMgrSingletonContext.ActiveCfgVariants, 0u);/* activate base variant */                                             /* SBSW_DCM_POINTER_WRITE_BITSET */
#  endif
# endif
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_INIT, lErrorId);

# if (DCM_VARMGR_MODE_POSTBUILD_ANY_ENABLED == STD_ON)
  lResult = (boolean) (lErrorId == DCM_E_NO_ERROR);                                                                                                  /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
# else
  lResult = TRUE;
# endif

  DCM_IGNORE_UNREF_PARAM(configPtr);
  DCM_IGNORE_UNREF_PARAM(lErrorId);

  return lResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_VarMgrGetActiveCfgVariantId()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_CfgVarMgrDiagVariantIdMemType, DCM_CODE) Dcm_VarMgrGetActiveCfgVariantId(
  void
  )
{
#if (DCM_VARMGR_MULTI_SVC_EXCLUSIVE_ENABLED == STD_ON)
  return Dcm_VarMgrSingletonContext.ActiveCfgVariantId;
#else
  return 0;
#endif
}

#if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VarMgrIsEnabledInActiveVariants()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_VarMgrIsEnabledInActiveVariants(
  P2CONST(Dcm_CfgVarMgrBitSetBaseType, TYPEDEF, DCM_CONST) varInfoTable
  )
{
  boolean lIsEnabled = FALSE;
  Dcm_CfgVarMgrDiagVariantIdOptType varRowIter = Dcm_VarMgrBitSetCalcSize(DCM_VARMGR_NUM_CFG_VARIANTS);

  /* scan all variants (at least 8 way scan) for at least one match */
  while(varRowIter > 0u)
  {
    --varRowIter;
    if((varInfoTable[varRowIter] & Dcm_VarMgrSingletonContext.ActiveCfgVariants[varRowIter]) != 0u)
    {
      lIsEnabled = TRUE;/* at least one matched variant */
      break;
    }/* else - continue searching */
  }

  return lIsEnabled;
}
#endif

#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgSetSingle()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgSetSingle(
  Dcm_VsgIdentifierType VsgId,
  Dcm_VsgStateType State
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint8          lErrorId   = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError( (VsgId >= DCM_VARMGR_NUM_CFG_VARIANTS)
                               ||(VsgId == 0u) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if (Dcm_DebugDetectError( (State != DCM_VSG_ENABLED)
                               &&(State != DCM_VSG_DISABLED) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lStdResult = DCM_E_OK;

    Dcm_VarMgrVsgSetSingle(VsgId, State);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_VSGSETSINGLE, lErrorId);

  return lStdResult;
}
#endif

#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgIsActive()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgIsActive(
  Dcm_VsgIdentifierType VsgId,
  P2VAR(Dcm_VsgStateType, AUTOMATIC, DCM_APPL_DATA) State
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint8          lErrorId   = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError( (VsgId >= DCM_VARMGR_NUM_CFG_VARIANTS)
                               ||(VsgId == 0u) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if (Dcm_DebugDetectError(State == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    lStdResult = DCM_E_OK;

    Dcm_VarMgrVsgIsActive(VsgId, State);                                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_VSGISACTIVE, lErrorId);

  return lStdResult;
}
#endif

#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgSetMultiple()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgSetMultiple(                                                                                                   /* PRQA S 6080 */ /* MD_MSR_STMIF */
  P2CONST(Dcm_VsgIdentifierType, AUTOMATIC, DCM_APPL_DATA) VsgIdList,
  uint16 VsgListSize,
  Dcm_VsgStateType State
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint8          lErrorId   = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError(VsgIdList == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if (Dcm_DebugDetectError( (VsgListSize > DCM_VARMGR_NUM_CFG_VARIANTS)
                               ||(VsgListSize == 0u) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_VsgIdentifierType lVsgIter;

    /* Iterate over all passed VSGs */
    for (lVsgIter = 0; lVsgIter < VsgListSize; ++lVsgIter)
    {
      lStdResult = Dcm_VsgSetSingle(VsgIdList[lVsgIter], State);

      if(lStdResult == DCM_E_NOT_OK)
      {
        break;
      }
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_VSGSETMULTIPLE, lErrorId);

  return lStdResult;
}
#endif

#if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgIsActiveAnyOf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgIsActiveAnyOf(                                                                                                 /* PRQA S 6080 */ /* MD_MSR_STMIF */
  P2CONST(Dcm_VsgIdentifierType, AUTOMATIC, DCM_APPL_DATA) VsgIdList,
  uint16 VsgListSize,
  P2VAR(Dcm_VsgStateType, AUTOMATIC, DCM_APPL_DATA) State
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  uint8          lErrorId   = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if (Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if (Dcm_DebugDetectError(VsgIdList == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else if (Dcm_DebugDetectError( (VsgListSize > DCM_VARMGR_NUM_CFG_VARIANTS)
                               ||(VsgListSize == 0u) ))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if (Dcm_DebugDetectError(State == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_VsgIdentifierType lVsgIter;

    /* Iterate over all passed VSGs */
    for (lVsgIter = 0; lVsgIter < VsgListSize; ++lVsgIter)
    {
      lStdResult = Dcm_VsgIsActive(VsgIdList[lVsgIter], State);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

      if( (lStdResult == DCM_E_NOT_OK)
        ||(*State == DCM_VSG_ENABLED) )
      {
        break;
      }
    }
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_VSGISACTIVEANY, lErrorId);

  return lStdResult;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_VarMgr.c
 *********************************************************************************************************************/
