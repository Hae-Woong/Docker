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
/**        \file  Dcm.c
 *         \unit  NoUnit
 *        \brief  Implementation of DCM controller functions
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
#define DCM_SOURCE

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
#include "Dcm.h"
#include "Dcm_Tmr.h"
#include "Dcm_Tsk.h"
#include "Dcm_Net.h"
#include "Dcm_RsrcMgr.h"
#include "Dcm_State.h"
#include "Dcm_Diag.h"
#include "Dcm_Debug.h"
#include "Dcm_CfgWrap.h"
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
# include "Dcm_PagedBuffer.h"
#endif
#if (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_DidMgr.h"
#endif
#if (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_RidMgr.h"
#endif
#if ((DCM_MODE_COMMCTRL_ENABLED == STD_ON) || (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON))
# include "Dcm_Mode.h"
#endif
#if (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_AuthMgr.h"
#endif
#if (DCM_FBLMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_FblMgr.h"
#endif
#if (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_VarMgr.h"
#endif
#if (DCM_MEMMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_MemMgr.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if ((DCM_SW_MAJOR_VERSION != 21u) || \
    (DCM_SW_MINOR_VERSION !=  5u) || \
    (DCM_SW_PATCH_VERSION !=  0u))
# error "Vendor specific version numbers of Dcm.c and Dcm.h are inconsistent"
#endif
#if ((DCM_CFG_MAJOR_VERSION != 21u) || \
    (DCM_CFG_MINOR_VERSION !=  5u) || \
    (DCM_CFG_PATCH_VERSION !=  0u))
# error "Vendor specific version numbers of Dcm.c and Dcm_Cfg.h are inconsistent"
#endif
#if ((DCM_LCFG_MAJOR_VERSION != 21u) || \
    (DCM_LCFG_MINOR_VERSION !=  5u) || \
    (DCM_LCFG_PATCH_VERSION !=  0u))
# error "Vendor specific version numbers of Dcm.c and Dcm_Lcfg.h are inconsistent"
#endif
#if ((DCM_PBCFG_MAJOR_VERSION != 21u) || \
    (DCM_PBCFG_MINOR_VERSION !=  5u) || \
    (DCM_PBCFG_PATCH_VERSION !=  0u))
# error "Vendor specific version numbers of Dcm.c and Dcm_PBcfg.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Sub-components per-thread memory */
DCM_LOCAL VAR(Dcm_ContextType, DCM_VAR_NOINIT) Dcm_ThreadContext[DCM_NUM_THREADS];                                                                   /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
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
#if (DCM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Dcm_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DCM_APPL_DATA) versioninfo
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(versioninfo == NULL_PTR))
  {
    lErrorId = DCM_E_PARAM_POINTER;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    versioninfo->vendorID = (uint16)DCM_VENDOR_ID;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    versioninfo->moduleID = (uint16)DCM_MODULE_ID;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    versioninfo->sw_major_version = (uint8)DCM_SW_MAJOR_VERSION;                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    versioninfo->sw_minor_version = (uint8)DCM_SW_MINOR_VERSION;                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    versioninfo->sw_patch_version = (uint8)DCM_SW_PATCH_VERSION;                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_GETVERSIONINFO, lErrorId);
}
#endif
/**********************************************************************************************************************
 *  Dcm_InitMemory()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_InitMemory(
  void
  )
{
  Dcm_DebugDeInit();
}

/**********************************************************************************************************************
 *  Dcm_Init()
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
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Init(                                                                                                                       /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ConfigPtrType configPtr
  )
{
  DCM_IGNORE_UNREF_PARAM(configPtr);

#if (DCM_VARMGR_SUPPORT_ENABLED == STD_ON)
  if(Dcm_VarMgrInit(configPtr) == TRUE)                                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
#endif
  {
    /* Initialize configuration abstraction */
    Dcm_CfgWrapInit(configPtr);                                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Initialize run-time kernel sub-system */
    Dcm_TmrInit();
    Dcm_TskInit();

    /* Initialize communication sub-systems */
    Dcm_NetInit(configPtr);                                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
    Dcm_PagedBufferInit();
#endif

    /* Initialize diagnostic service helpers sub-systems */
#if (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON)
    Dcm_DidMgrInit();                                                                                                                                /* PRQA S 2987 */ /* MD_Dcm_2987 */
#endif
#if (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON)
    Dcm_RidMgrInit();
#endif
    Dcm_RsrcMgrInit();
#if (DCM_FBLMGR_SUPPORT_ENABLED == STD_ON)
    Dcm_FblMgrReset();
#endif

    /* Initialize diagnostic state management sub-systems */
#if (DCM_MODE_COMMCTRL_ENABLED == STD_ON) || \
    (DCM_MODE_CTRLDTCSETTING_ENABLED == STD_ON)
    Dcm_ModeInit();
#endif
    Dcm_StateInit();
#if (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON)
    Dcm_AuthMgrInit();                                                                                                                               /* PRQA S 2987 */ /* MD_Dcm_2987 */
#endif

    /* Initialize diagnostic service processing kernel sub-system */
    Dcm_DiagInit(configPtr);                                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Initialize memory manager after diag unit due to dependency to Dcm_PbCfgDiagVariantFilter */
#if (DCM_MEMMGR_SUPPORT_ENABLED == STD_ON)
    Dcm_MemMgrInit();
#endif

    /* Initialize debug sub-component as last to mark DCM as completely initialized */
    Dcm_DebugInit();/* from now on the DCM is ready to work */
  }
}

#if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_MainFunctionTimer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_MainFunctionTimer(
  void
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  if(!Dcm_DebugIsModuleInitialized())
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    Dcm_TskScheduler(DCM_TSK_PRIO_HIGH);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_MAINFUNCTION, lErrorId);
}
#endif

#if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_MainFunctionWorker()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_MainFunctionWorker(
  void
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  if(!Dcm_DebugIsModuleInitialized())
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    Dcm_TskScheduler(DCM_TSK_PRIO_LOW);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_MAINFUNCTION, lErrorId);
}
#endif

#if (DCM_SPLIT_TASKS_ENABLED == STD_OFF)
/**********************************************************************************************************************
 *  Dcm_MainFunction()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_MainFunction(
  void
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  if(!Dcm_DebugIsModuleInitialized())
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    Dcm_TskScheduler(DCM_TSK_PRIO_NONE);
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_MAINFUNCTION, lErrorId);
}
#endif

/**********************************************************************************************************************
 *  Dcm_GetThreadContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Dcm_ContextPtrType, DCM_CODE) Dcm_GetThreadContext(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_ContextPtrType pContext;

  if (Dcm_DebugDetectRuntimeError(threadId >= DCM_NUM_THREADS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pContext = &Dcm_ThreadContext[0];
  }
  else
  {
    pContext = &Dcm_ThreadContext[threadId];
  }

  return pContext;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  COMPONENT DETAILED DESIGN OF GENERATED FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  Dcm_Cfg<Unit>Get<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/**********************************************************************************************************************
 *  Dcm_ModeRule<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_ModeCondGet_<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_ModeOnComControlModeChange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/**********************************************************************************************************************
 *  Dcm_DidMgr<XXX>ReadData()
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

/**********************************************************************************************************************
 *  Dcm_DidMgr<DynDID>Read<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr<XXX>WriteData()
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
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr<OBD_AID>ReadData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr<MID>ReadData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<Data>_Write_IOControlRequest_ControlState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_DeadEnd_Write_IOControlRequest_ControlState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<Data>_Write_IOControlRequest_IOOperationRequest()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<Data>_Read_IOControlRequest_UnderControl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<Data>_Write_IOControlRequest_UnderControl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<DID>_IoControlOperation()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<DID>_ReturnControlToECU()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<SupportedID>_ReadData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr_<SupportedID>_ReadDataLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr<NvMDID>Read()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr<NvMDID>Write()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_DidMgr<Internal>ReadData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/**********************************************************************************************************************
 *  Dcm_RidMgr<XXX>()
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
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_RidMgr_<XXX>_OBD_AID()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_RidMgr_<XXX>_OBD_TID()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/**********************************************************************************************************************
 *  Dcm_SvcWrapper_<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_SubSvcWrapper_<XXX>()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_ServiceNoPostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_ServiceNoUpdater()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_ServiceNoCancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/**********************************************************************************************************************
 *  Dcm_Svc27_Dummy_GetSecurityAttemptCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */

/**********************************************************************************************************************
 *  Dcm_Svc27_Dummy_SetSecurityAttemptCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/**********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* Following markers are only used in generated or extension files, thus "unused marker" warnings can be ignored.*/
                                                                                                                                                     /* PRQA S 0310 */ /* MD_Dcm_DifferentPointerType_0310 */
                                                                                                                                                     /* PRQA S 2982 */ /* MD_Dcm_Redundant_2982 */
                                                                                                                                                     /* PRQA S 1330 */ /* MD_Dcm_RteSpecific_1330 */
                                                                                                                                                     /* PRQA S 1514 */ /* MD_Dcm_ObjectOnlyAccessedOnce */
                                                                                                                                                     /* PRQA S 0612 */ /* MD_Dcm_0612 */

                                                                                                                                                     /* module specific MISRA deviations:

   MD_Dcm_DifferentPointerType_0310:
     Description: Rule 11.3
                  A cast shall not be performed between a pointer to object type and a pointer to a different object type.
     Reason:      To optimize access without the need of local variable, pointers to data types of the same size (uint8, sint8)
                  will be casted to the corresponding target pointer type.
     Risk:        Compile error/warnings.
     Prevention:  Compiler compatibility is tested during integration.

   MD_Dcm_0313:
     Description: Rule 11.1
                  Conversions shall not be performed between a pointer to a function and any other type.
     Reason:      No efficient dynamic-cast operation available in ANSI-C.
     Risk:        Data corruption due to miscast.
     Prevention:  Covered by descriptive pointer types and code review for correct usage.

   MD_Dcm_Rule19.2:
     Description: Rule 19.2
                  The union keyword should not be used.
     Reason:      Data without shared lifetime is stored in union for efficiency.
     Risk:        None due to no shared lifetime.
     Prevention:  None.

   MD_Dcm_Optimize_0771:
     Description: Rule 15.4
                  There should be no more than one break or goto statement used to terminate any iteration statement.
     Reason:      The loop needs multiple exit points since error conditions cannot be verified prior to the loop.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_Rule1.2_1039:
    Description: Rule 1.2
                 Treating array of length one as potentially flexible member.
    Reason:      In a special configurations, a struct type has only one memeber of an array type with a size of one.
    Risk:        Some compilers would consider the array as a flexible memeber.
    Prevention:  Testing the code under several compilers.

   MD_Dcm_RteSpecific_1330:
     Description: Rule 8.3
                  All declarations of an object or function shall use the same names and type qualifiers.
     Reason:      The function declaration generated by the RTE are RTE-implementation specific and unknown at
                  the DCM development time.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_ObjectOnlyAccessedOnce:
    Description: Rule 8.9
                 An object should be defined at block scope if its identifier only appears in a single function.
    Reason:      Autosar rules or OEM requirements mandate this implementation.
    Risk:        None.
    Prevention : None.

   MD_Dcm_Optimize_2003:
     Description: Rule 16.3
                  An unconditional break statement shall terminate every switch-clause Enforcement.
     Reason:      To use optimal design small switch-case dispatchers use fall-through cases.
     Risk:        None, since documented as an explicit fall-through case.
     Prevention:  None.

   MD_Dcm_Redundant_2982:
    Description: Rule 2.2
                 This assignment is redundant. The value of this object is never used before being modified.
    Reason:      Initializing global variables.
    Risk:        None.
    Prevention:  None.

   MD_Dcm_Redundant_2983:
    Description: Rule 2.2
                 This assignment is redundant. The value of this object is never subsequently used.
    Reason:      In the last iteration of an iterative structure, a statement might be executed but not used.
    Risk:        None.
    Prevention:  None.

   MD_Dcm_Redundant_2985:
    Description: Rule 2.2
                 This operation is redundant. The value of the result is always that of the left-hand operand.
    Reason:      Apparent redundant-like operations are needed for better code readability.
    Risk:        None.
    Prevention:  None.

   MD_Dcm_Redundant_2986:
    Description: Rule 2.2
                 This operation is redundant. The value of the result is always that of the right-hand operand.
    Reason:      Apparent redundant-like operations are needed for better code readability.
    Risk:        None.
    Prevention:  None.

   MD_Dcm_Optimize_2889:
     Description: Rule 15.5
                  A function should have a single point of exit at the end.
     Reason:      For code run-time/ROM usage reason this function terminates immediately.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_ConstExpr:
    Description: Rule 2.2
                 Controlling expressions shall not be invariant.
    Reason:      A configuration dependent situation could lead to a constant logical expression.
    Risk:        None.
    Prevention:  None.

   MD_Dcm_CodingRule_3218:
     Description: Rule 8.9
                  An object should be defined at block scope if its identifier only appears in a single function.
     Reason:      Vector style guide prevents usage of static variables/constant objects in function scope.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_Rule13.5:
     Description: Rule 13.5
                  The right hand operand of a logical && or || operator shall not contain persistent side effects.
     Reason:      The function called on the right hand side has no side effects.
     Risk:        The function may introduce side effects at a later time.
     Prevention:  None.

   MD_Dcm_UnknownSize_3684:
     Description: Rule 8.11
                  When an array with external linkage is declared, its size should be explicitly specified.
     Reason:      Array declared with unknown size to reduce number of variants and therefore complexity.
                  The user of that array does not need the size of the array.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_APIStd_3673:
     Description: Rule 8.13
                  A pointer should point to a const-qualified type whenever possible.
     Reason:      The API is defined by the AUTOSAR standardization.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_Design_3673:
     Description: Rule 8.13
                  A pointer should point to a const-qualified type whenever possible.
     Reason:      The API has to be compatible to a common prototype, defined by module's design to serve generic purposes.
     Risk:        None.
     Prevention:  None.

   MD_Dcm_BitNegation_4399:
     Description: Rule 10.8
                  An expression which is the result of a ~ or << operation has been cast to a wider type.
     Reason:      Using the ~ operator avoids errors in manually typed bit masks.
     Risk:        May generate compiler warnings although explicit casts are used.
     Prevention:  None.

   MD_Dcm_2987:
     Description: Rule 2.2
                  This function call produces no side effects and is redundant.
     Reason:      The functions produces side effects, but only in specific configurations.
     Risk:        None.
     Prevention:  None.

  MD_Dcm_RetVal:
     Description: Rule 2.2
                  This initialization is redundant. The value of this object is never used before being modified.
     Reason:      Without this initialization some not so smart compilers will erroneously complain about use of uninitialized variable.
     Risk:        The redundant initialization code might be confusing.
     Prevention:  None.

  MD_Dcm_0612:
     Description: Rule 1.1
                  Size of object exceeds 32767 bytes - program does not conform strictly to ISO:C90.
     Reason:      A DCM buffer was configured with a size greater than 32767 bytes.
     Risk:        Decreased code portability.
     Prevention:  None.

  MD_Dcm_DerefInvalidPointer:
     Description: Rule 18.1
                  Dereference of an invalid pointer value.
     Reason:      This is a false positive and a known PRQA issue.
     Risk:        None.
     Prevention:  None.

  MD_Dcm_ComputingInvalidPointer:
     Description: Rule 18.1
                  Computing an invalid pointer value.
     Reason:      This is a false positive and a known PRQA issue.
     Risk:        None.
     Prevention:  None.

  MD_Dcm_8.5_Rte:
     Description: Rule 8.5
                  Multiple declarations of external object or function.
     Reason:      MICROSAR Rte generates prototypes for callbacks. Some callbacks are also contained in a replacement
                  header in case no Rte is used. This leads to duplicate definitions.
     Risk:        None.
     Prevention:  None.

  MD_Dcm_OptimizedInlineCode_3387:
     Description: Rule 13.3
                  A full expression containing an increment (++) or decrement (--) operator should have no other
                  potential side effects other than that caused by the increment or decrement operator
     Reason:      This kind of operator is used only in case of function-like macros where also a return value is expected.
                  Needed for maximum inlining efficiency and less other MISRA deviations.
     Risk:        The risks are at minimum since the affected locations are simple expressions.
     Prevention:  These code sections are thoroughly tested by dedicated unit tests.

  MD_Dcm_Design_3679:
     Description: Rule 8.13
                  The object referenced is not modified through it, so the object could be declared with type const*.
     Reason:      The API has to be compatible to a common prototype, defined by module's design to serve generic purposes.
     Risk:        None.
     Prevention:  None.
     
  MD_Dcm_2962:
     Description: Rule 9.1
                  The value of an object with automatic storage duration shall not be read before it has been set.
     Reason:      The variable is initialized in a function call before the variable is used. It is ensured that the variable
                  is initialized in the function call, because a check whether the DID exists is already done earlier. 
     Risk:        None.
     Prevention:  None.

  MD_Dcm_2822:
     Description: Rule 21.1
                  Arithmetic operation on NULL pointer.
     Reason:      This is a false positive.
     Risk:        None.
     Prevention:  None.
     
*/
/**********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 *********************************************************************************************************************/
                                                                                                                                                     /* COV_JUSTIFICATION_BEGIN

\ID COV_DCM_MACRO_OVERRIDE
  \ACCEPT TX
  \ACCEPT XF
  \REASON The value of a macro may be overridden by user config for test purposes. Since this is not officially
          supported, it is not covered by the test suite.

\ID COV_DCM_UNSUPPORTED
  \REASON [COV_MSR_UNSUPPORTED]

\ID COV_DCM_SUPPORT_ALWAYS
  \REASON This configuration switch is always on, but the code is enclosed in a pre-processor precondition for consistency reasons and safe future changes.

\ID COV_DCM_SUPPORT_RESTRICTED
  \REASON This configuration switch is always on/off, due to feature range limitation.

\ID COV_DCM_SUPPORT_XOR
  \REASON Disjunction of configuration switches that are mutually exclusive and one of them is always true.

\ID COV_DCM_SUPPORT_WITH_MSR_DEM
  \REASON This configuration switch is always on as long as MSR4 DEM is used (required for SafeBSW context).

\ID COV_DCM_REQUIRED_BY_SAFEBSW
  \REASON This configuration switch is always on as required by SafeBSW context.

\ID COV_DCM_DEPENDS_ON_UNSUPPORTED
  \REASON This configuration switch is always on/off due to a justified expression it is involved in.

\ID COV_DCM_DEV_DEBUG
  \REASON The justified object is uncovered, since it is only used during component development to ease the detection of errors!

\ID COV_DCM_UNSUPPORTED_ERROR_DETECTION
  \REASON The justified object is uncovered, since it is only used to ease the detection of configuration errors!

\ID COV_DCM_PRQA_UNREACHABLE
  \ACCEPT XF
  \REASON The justified object is only relevant for internal code analysis and cannot be covered in component tests.

\ID COV_DCM_NOUNIT
  \REASON The justified object is always undefined, since it is only used during component unit testing.

\ID COV_DCM_RTM_NO_RCR_RP_TX
  \ACCEPT XX
  \REASON The justified object is unreachable, since the configuration does not support any feature that could use it.
          A complete encapsulation of all use-cases where no RCR-RP will be sent would be too complex and risky.

\ID COV_DCM_RTM_DEV_DEBUG
  \REASON [COV_DCM_DEV_DEBUG]

\ID COV_DCM_RTM_UNREACHABLE
  \REASON The justified object is uncovered, since it is not intended to be reached! It just exists because of other reasons like MISRA (default case in a switch)
          or because of added debug assert monitoring for development purposes (see COV_DCM_RTM_DEV_DEBUG).

\ID COV_DCM_RTM_UNREACHABLE_COMPLEXITY
  \REASON The justified object is uncovered, since it can be reached only in very rare situations (i.e. interrupt, multiple events in a specific sequence, etc.)!
          But at the same time it is part of the (sub-)component design and cannot be removed.

\ID COV_DCM_RTM_DESIGN_LOWER_LAYER
  \REASON The justified object is uncovered, since it is a basic (lower layer) functionality and is designed to behave in different situations.
          These situations are not necessarily supported by all configurations. Due to the high complexity of dependent functionalities
          the concrete usage of the uncovered functionality is left to the upper layers in DCM!

\ID COV_DCM_RTM_RUNTIME_CHECK
  \REASON This condition depends on a runtime check for an uninitialized pointer or remaining buffer size check.

\ID COV_DCM_RTM_DEPENDS_ON_UNSUPPORTED
  \REASON The justified object is uncovered, since it can vary only if a specific (non supported in SafeBSW context) configuration is activated.

\ID COV_DCM_RTM_DEPENDS_ON_CFG
  \ACCEPT TX
  \ACCEPT XF
  \REASON The justified expression evaluates always depending on the configuration either to true or false.

\ID COV_DCM_RTM_UNIT_TEST
  \REASON This condition is covered on unit test level.

COV_JUSTIFICATION_END */
/**********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 *********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_DCM_PARAM_PTR_READ_ONlY
  \DESCRIPTION    A pointer to a constant configuration table without function pointer members, is passed to a function
                  expecting a valid pointer.
  \COUNTERMEASURE \N The Compiler ensures the validity of the pointers because the pointer parameter is a constant pointer.

\ID SBSW_DCM_PARAM_PTR_WRITE
  \DESCRIPTION    In a function, data is written through a pointer passed as function argument.
  \COUNTERMEASURE \N [CM_DCM_N_PASS_PARAM_PTR]

\ID SBSW_DCM_LOCAL_ARRAY_WRITE
  \DESCRIPTION    In a function, data written to an array.
  \COUNTERMEASURE \N [CM_DCM_N_ARRAY_INDEX_WRITE]

\ID SBSW_DCM_GEN_PARAM_PTR_WRITE
  \DESCRIPTION    In a generated function, called only by DCM static code, data is written through a
                  pointer passed as function argument.
  \COUNTERMEASURE \S [CM_DCM_S_GEN_PARAM_PTR_WRITE]

\ID SBSW_DCM_PARAM_PTR_FORWARD
  \DESCRIPTION    A pointer received in a function is passed to a function expecting a valid pointer.
  \COUNTERMEASURE \N [CM_DCM_N_PASS_PARAM_PTR]

\ID SBSW_DCM_GEN_PARAM_PTR_FORWARD
  \DESCRIPTION    A pointer received in a generated function, called only by DCM static code, is passed to
                  a function expecting a valid pointer.
  \COUNTERMEASURE \S [CM_DCM_S_GEN_PARAM_PTR_FORWARD]

\ID SBSW_DCM_PARAM_FUNCPTR_CALL
  \DESCRIPTION    In a function, a function pointer passed as function argument is called.
  \COUNTERMEASURE \N [CM_DCM_N_CALL_PARAM_FUNCPTR]

\ID SBSW_DCM_GLOBAL_PTR_WRITE
  \DESCRIPTION    In a function, data is written through a pointer taken from a global variable.
  \COUNTERMEASURE \N The pointer is taken from a global variable, as such it is always valid.

\ID SBSW_DCM_POINTER_FORWARD_STACK
  \DESCRIPTION    A pointer is passed to a function expecting a valid pointer. The called function does not store its
                  pointer parameter(s).
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

\ID SBSW_DCM_GEN_POINTER_FORWARD_STACK
  \DESCRIPTION    A pointer is passed to a function expecting a valid pointer (i.e. to return data through it).
                  The called function does not store its pointer parameter(s) for deferred writing.
  \COUNTERMEASURE \S [CM_DCM_S_GEN_POINTER_FORWARD_STACK]

\ID SBSW_DCM_POINTER_FORWARD_GLOBAL
  \DESCRIPTION    A pointer is passed to a function expecting a valid pointer. The called function does not store its
                  pointer parameters.
  \COUNTERMEASURE \N The pointer is taken from a global variable, as such it is always valid.

\ID SBSW_DCM_COMB_PTR_FORWARD
  \DESCRIPTION    A combination of pointers is passed to a function expecting valid pointers. The called function does
                  not store its pointer parameters.The pointer is taken from one of the following locations:
                     - global variable;
                     - local (stack) variable;
                     - is an argument of the caller function (refer also to CM_DCM_N_PASS_PARAM_PTR);
                     - is a pointer initialized with one of the following associated initialization function that
                       returns always a valid Pointer: Dcm_UtiGetDataContextBuffer(),
                                                       Dcm_UtiGetResData(),
                                                       Dcm_UtiGetResDataAt(),
                                                       Dcm_UtiInitDataContext(),
                                                       Dcm_DiagInitMsgContextBufferInfo(),
                                                       Dcm_DiagInitiateServiceProcessing(),
                                                       Dcm_NetPbCfgGetBufferInfo(),
                                                       Dcm_NetPbCfgGetTransportObject(),
                                                       Dcm_NetPbCfgGetPeriodicTxObject(),
                                                       Dcm_Svc22GetDidInfoContext(),
                                                       Dcm_Svc2ASchdGetEntry(),
                                                       Dcm_Svc2CDefMgrGetSrcItem(),
                                                       Dcm_GetThreadContext(),
                                                       Dcm_Svc31RedirectMsgContext(),
                                                       Dcm_Svc86Schd_03GetDidRecord(),
                                                       Dcm_Svc86Schd_07GetDidRecord(),
                                                       Dcm_DidMgrSecureCodingLockBuffer(),
                                                       Dcm_PagedBufferGetContext(),
                                                       Dcm_DiagGetThreadContext
                     as such it is always valid.
\COUNTERMEASURE \M [CM_DCM_M_SIZEOF_BUFFERINFO]
                \R [CM_DCM_R_GET_BUFFERINFO]
                \R [CM_DCM_R_SET_DATA_CONTEXT_USAGE]

\ID SBSW_DCM_GEN_COMB_PARAM_PTR_FORWARD
  \DESCRIPTION    A combination of pointers is passed to a function expecting valid pointers. The called function does
                  not store its pointer parameters.The pointer is taken from one of the following locations:
                     - local (stack) variable;
                     - is an argument of the caller function (refer also to CM_DCM_S_GEN_PARAM_PTR_FORWARD);
                     - is a pointer initialized with one of the following associated initialization function that
                       returns always a valid Pointer: Dcm_UtiGetResDataAt()
                     as such it is always valid.
\COUNTERMEASURE \R [CM_DCM_R_SET_MESSAGE_CONTEXT_REQBUFFER]
                \R [CM_DCM_R_SET_MESSAGE_CONTEXT_RESBUFFER]
                \S [CM_DCM_S_ROUTINE_OP_FUNC_SIGNATURE]
                \S [CM_DCM_S_ROUTINE_OP_FUNC_WRITE_DATA_LENGTH]
                \S [CM_DCM_S_ROUTINE_WRAPPER_WRITE_DATA_LENGTH]

\ID SBSW_DCM_GEN_RID_WRAPPER
  \DESCRIPTION    In a generated function, a pointer to a uint8/sint8 array routine signal is passed to a function
                  expecting a valid pointer.
  \COUNTERMEASURE \S [CM_DCM_S_ROUTINE_OP_FUNC_SIGNATURE]
                  \S [CM_DCM_S_ROUTINE_OP_FUNC_WRITE_DATA_LENGTH]
                  \S [CM_DCM_S_ROUTINE_WRAPPER_WRITE_DATA_LENGTH]

\ID SBSW_DCM_GEN_DID_WRAPPER
  \DESCRIPTION    In a generated function, a pointer to a uint8 array DID signal is passed to a function
                  expecting a valid pointer.
  \COUNTERMEASURE \S [CM_DCM_S_DID_WRAPPER_WRITE_DATA_LENGTH]
                  \S [CM_DCM_S_DID_WRAPPER_NV_BLOCK_ID_READ]
                  \S [CM_DCM_S_DID_WRAPPER_NV_BLOCK_ID_WRITE]

\ID SBSW_DCM_ROE_NV_ACCESS
  \DESCRIPTION    In a function, a pointer to a NvM data structure is passed to a function expecting valid pointers.
  \COUNTERMEASURE \S [CM_DCM_S_ROE_NV_BLOCK_ID_ACCESS]

\ID SBSW_DCM_SVC29_NV_ACCESS
  \DESCRIPTION    In a function, a pointer to a NvM data structure is passed to a function expecting valid pointers.
  \COUNTERMEASURE \S [CM_DCM_S_SVC29_NV_BLOCK_ID_ACCESS]

\ID SBSW_DCM_POINTER_WRITE_BUFFERINFO
  \DESCRIPTION    In an internal function, data is written through a pointer to a buffer info element, or the
                  pointer is passed to a function expecting a valid reference to a buffer info element.
                  The memory entry pointer is initialized by Dcm_NetPbCfgGetBufferInfo() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_CONSISTENCY_BUFFERINFO]
                  \M [CM_DCM_M_SIZEOF_BUFFERINFO]
                  \R [CM_DCM_R_GET_BUFFERINFO]

\ID SBSW_DCM_POINTER_WRITE_COMMCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a ComM context element, or the
                  pointer is passed to a function expecting a valid reference to a ComM context element.
                  The memory entry pointer is initialized by Dcm_NetPbCfgGetComMContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_COMMCONTEXT]
                  \R [CM_DCM_R_GET_COMMCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_BUFFERCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a buffer context element, or the
                  pointer is passed to a function expecting a valid reference to a buffer context element.
                  The memory entry pointer is initialized by Dcm_NetPbCfgGetBufferContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_BUFFERCONTEXT]
                  \R [CM_DCM_R_GET_BUFFERCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT
  \DESCRIPTION    In an internal function, data is written through a pointer to a transport object, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_NetPbCfgGetTransportObject() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_TRANSPORTOBJECT]

\ID SBSW_DCM_POINTER_WRITE_TIMERCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a timer context, or the
                  pointer is passed to a function expecting a valid reference to a timer context.
                  The memory entry pointer is initialized by Dcm_TmrGetTimerContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_TIMERCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_THREADCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a thread context, or the
                  pointer is passed to a function expecting a valid reference to a thread context.
                  The memory entry pointer is initialized by Dcm_GetThreadContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_THREADCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_DIAGTHREADCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a diag unit thread context, or the
                  pointer is passed to a function expecting a valid reference to a thread context.
                  The memory entry pointer is initialized by Dcm_DiagGetThreadContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_DIAGTHREADCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_DEBUGTHREADCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a debug thread context, or the
                  pointer is passed to a function expecting a valid reference to a debug thread context.
                  The memory entry pointer is initialized by Dcm_DebugGetThreadContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_DEBUGTHREADCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_SVC31THREADCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a Svc31 thread context, or the
                  pointer is passed to a function expecting a valid reference to a Svc31 thread context.
                  The memory entry pointer is initialized by Dcm_SVC31GetThreadContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_SVC31THREADCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a paged-buffer context, or the
                  pointer is passed to a function expecting a valid reference to a paged-buffer thread context.
                  The memory entry pointer is initialized by Dcm_PagedBufferGetContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_PAGEDBUFFERCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_QUEUEDTOBJ
  \DESCRIPTION    In an internal function, data is written through a pointer to a transport object, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_DiagGetQueuedTranspObj() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_TRANSPORTOBJECT]

\ID SBSW_DCM_POINTER_WRITE_TOBJ_OF_CONNECTION
  \DESCRIPTION    In an internal function, data is written through a pointer to a transport object, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_NetGetTranspObjOfConnection().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_WRITE_ALLOCATEDTOBJ
  \DESCRIPTION    In an internal function, data is written through a pointer to a transport object, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_NetAllocateOrGetTranspObject().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_WRITE_RESERVEDTOBJ
  \DESCRIPTION    In an internal function, data is written through a pointer to a transport object, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_NetLockConnection().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_TRANSPORTOBJECT]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_WRITE_PERTXOBJ
  \DESCRIPTION    In an internal function, data is written through a pointer to a periodic transport ojbect, or the
                  pointer is passed to a function expecting a valid reference to a transport object.
                  The memory entry pointer is initialized by Dcm_NetPbCfgGetPeriodicTxObject() which will always
                  return a pointer to a valid memory entry.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_PERTXOBJ]
                  \R [CM_DCM_R_SET_PERTXOBJ]

\ID SBSW_DCM_POINTER_WRITE_TASKCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a task context object.
                  The memory entry pointer is initialized by Dcm_TskGetTaskContext().
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_WRITE_SCHEDULERCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a scheduler context object.
                  The memory entry pointer is initialized by Dcm_TskGetSchedulerContext().
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_WRITE_BITSET
  \DESCRIPTION    In an internal function, data is written to an array representing a bitset. The index within the
                  array is calculated by Dcm_UtiBitSetGetRowIdxSafe() which will always return a valid index within
                  the given bitset.
  \COUNTERMEASURE \R [CM_DCM_R_GET_BITSETROWINDEX]
                  \R [CM_DCM_R_BITSET_BASETYPE]

\ID SBSW_DCM_POINTER_WRITE_2ASCHEDULERENTRY
  \DESCRIPTION    In an internal function, data is written through a pointer to a service 0x2A scheduler table entry, or
                  the pointer is passed to a function expecting a valid reference to a service 0x2A scheduler table
                  entry. The memory entry pointer is initialized by Dcm_Svc2ASchdGetEntry() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_2ASCHEDULERENTRY]

\ID SBSW_DCM_POINTER_WRITE_2CSRCITEM
  \DESCRIPTION    In an internal function, data is written through a pointer to a service 0x2C source item, or the
                  pointer is passed to a function expecting a valid reference to a service 0x2C source item. The memory
                  entry pointer is initialized by Dcm_Svc2CDefMgrGetSrcItem() which will always return a pointer to a valid
                  memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_2CSRCITEM]

\ID SBSW_DCM_POINTER_WRITE_2CITEM
  \DESCRIPTION    In an internal function, data is written through a pointer to a service 0x2C item, or the pointer is
                  passed to a function expecting a valid reference to a service 0x2C item. The memory entry pointer is
                  initialized by Dcm_Svc2CDefMgrGetItem() which will always return a pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_2CITEM]

\ID SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a process context of a DID, or the
                  pointer is passed to a function expecting a valid reference to a process context. The memory entry
                  pointer is initialized by Dcm_Svc2CDefMgrGetDDDidProcessContext() which will always return a pointer to a
                  valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_2CPROCESSCONTEXT]

\ID SBSW_DCM_POINTER_WRITE_2FACTIVEIODIDS
  \DESCRIPTION    In an internal function, data is written to array Dcm_Svc2FSingletonContext.ActiveIoDids[] using a
                  loop iterator as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_2FACTIVEIODIDS]

\ID SBSW_DCM_POINTER_READ_86DTCRECORD
  \DESCRIPTION    In a function, a pointer to array Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcRecord.STRT[], or 
                  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.STRT[], and its size descriptor
                  Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcRecord.STRTLength, or 
                  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.STRTLength respectively, is passed
                  to a function expecting a valid pointer to an array and a size descriptor which does not exceed 
                  the size of the passed array.
  \COUNTERMEASURE \N The pointer is only used for reading and taken from a global variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_86RINGBUFFER
  \DESCRIPTION    In an internal function, data is written to the array Dcm_Svc86Schd_09DtcRingBuffer.DtcQueue[] using 
                  a variable Dcm_Svc86Schd_09DtcRingBuffer.WriteIndex as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_86RINGBUFFER]

\ID SBSW_DCM_POINTER_WRITE_86DTCRECORDSTRT
  \DESCRIPTION    In an internal function, data is written to the array 
                  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.STRT[] using a predefined index.
  \COUNTERMEASURE \N The predefined indices are less than the size of the array, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_86DTCINFORECORDPTR
  \DESCRIPTION    In an internal function, data is written through a pointer to a service 0x86 DTC record
                  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo. The memory entry pointer is 
                  initialized by Dcm_Svc86Schd_09GetDtcRecordInfo() which will 
                  always return a pointer to a valid memory entry.
  \COUNTERMEASURE \N The pointer is taken from a function which return a pointer to a global variable which is always valid.

\ID SBSW_DCM_POINTER_WRITE_86DTCRECORDONDTCCHANGE
  \DESCRIPTION    In an internal function, data is written through a pointer to a service 0x86 DTC event record
                  Dcm_Svc86CurrentSession.EventRecord.OnDtcChange. The memory entry pointer is
                  initialized by Dcm_Svc86SchdGetOnDtcChangeEventRecord() which will
                  always return a pointer to a valid memory entry.
  \COUNTERMEASURE \N The pointer is taken from a function which return a pointer to a global variable which is always valid.

\ID SBSW_DCM_POINTER_WRITE_86INTMRSPMSG
  \DESCRIPTION    In an internal function, data is written to the array 
                  Dcm_Svc86CurrentSession.EventRecord.DtcRecordInfo.DtcRecord.IntmRspMsg[] using a predefined index.
  \COUNTERMEASURE \N The predefined indices are less than the size of the array, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_86DIDRECORD
  \DESCRIPTION    In an internal function, data is written through a pointer to a service 0x86 DID record, or the
                  pointer is passed to a function expecting a valid reference to a service 0x86 DID record. The memory
                  entry pointer is initialized by Dcm_Svc86Schd_03GetDidRecord() or Dcm_Svc86Schd_07GetDidRecord() which will 
                  always return a pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_86DIDRECORD]

\ID SBSW_DCM_POINTER_WRITE_VSGACTIVECFGVARIANTS
  \DESCRIPTION    In an internal function, data is written to the array Dcm_VarMgrSingletonContext.ActiveCfgVariants[]
                  using an iterator.
  \COUNTERMEASURE \R [CM_DCM_R_GET_VSGACTIVECFGVARIANTS]

\ID SBSW_DCM_POINTER_WRITE_PERIODICTXBUFFER
  \DESCRIPTION    In an internal function, data is written through a pointer to the tx buffer of a periodic message, or
                  the pointer is passed to a function expecting a valid reference to the tx buffer of a periodic
                  message. The memory entry pointer is initialized by Dcm_NetPeriodicMsgGetTxBuffer() which will always
                  return a pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_PERIODICTXBUFFER]

\ID SBSW_DCM_POINTER_WRITE_RXPDUINFO
  \DESCRIPTION    In an internal function, data is written through a pointer to a Rx-PduInfo, or the
                  pointer is passed to a function expecting a valid reference to a Rx-PduInfo element.
                  The memory entry pointer is initialized by Dcm_NetProvideRxBuffer() which will always return a
                  pointer to a valid memory entry if the return values is BUFREQ_OK.
  \COUNTERMEASURE \R [CM_DCM_R_GET_RXPDUINFO]

\ID SBSW_DCM_POINTER_WRITE_DIDINFOCONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a Did info context, or the
                  pointer is passed to a function expecting a valid reference to a Did info context.
                  The memory entry pointer is initialized by Dcm_Svc22GetDidInfoContext() which will always return a
                  pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_DIDINFOCONTEXT]

\ID SBSW_DCM_POINTER_INIT_MESSAGE_CONTEXT
  \DESCRIPTION    The global message context variable is passed to Dcm_DiagInitMsgContextBufferInfo() for
                  initialization. That initalization function ensures that all pointer members are pointing to valid
                  memory entry pointer and the corresponding buffer size members are matching to the pointed memory
                  entry sizes.
  \COUNTERMEASURE \N The pointer is taken from a global variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_MESSAGE_CONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a message context, or the
                  pointer is passed to a function expecting a valid reference to a message context.
                  The memory entry pointer is initialized by Dcm_DiagGetMsgContext() which will always return a
                  pointer to a valid memory entry. The message context members are initialized by Dcm_DiagInit() via
                  Dcm_DiagInitMsgContextBufferInfo().
  \COUNTERMEASURE \R [CM_DCM_R_INIT_MESSAGE_CONTEXT]

\ID SBSW_DCM_POINTER_WRITE_MESSAGE_CONTEXT_REQBUFFER
  \DESCRIPTION    In an internal function, data is written through a pointer to a message context request buffer,
                  or the pointer is passed to a function expecting a valid reference to a message context
                  request buffer. The caller ensures that the reqData member points to a valid memory entry pointer
                  and the reqBufSize member matches to the pointed memory entry size.
  \COUNTERMEASURE \R [CM_DCM_R_SET_MESSAGE_CONTEXT_REQBUFFER]

\ID SBSW_DCM_POINTER_WRITE_MESSAGE_CONTEXT_RESBUFFER
  \DESCRIPTION    In an internal function, data is written through a pointer to a message context response buffer,
                  or the pointer is passed to a function expecting a valid reference to a message context
                  response buffer. The caller ensures that the resData member points to a valid memory entry pointer
                  and the resBufSize member matches to the pointed memory entry size.
  \COUNTERMEASURE \R [CM_DCM_R_SET_MESSAGE_CONTEXT_RESBUFFER]

\ID SBSW_DCM_POINTER_WRITE_MESSAGE_CONTEXT_RESBUFFER_ZERO_SEED
  \DESCRIPTION    In an internal function, data is written through a pointer to a message context response buffer,
                  or the pointer is passed to a function expecting a valid reference to a message context
                  response buffer. The caller ensures that the resData member points to a valid memory entry pointer
                  and the resBufSize member matches to the pointed memory entry size.
                  The caller ensures that the response buffer can hold the maximum zero seed size.
  \COUNTERMEASURE \R [CM_DCM_R_SET_MESSAGE_CONTEXT_RESBUFFER]
                  \M [CM_DCM_M_MAX_ZERO_SEED_LENGTH]

\ID SBSW_DCM_POINTER_WRITE_IO_CTRL_CEMR_SESSION_MASK
  \DESCRIPTION    In an internal function, data is written through a pointer to an IO control CEMR session mask, or the
                  pointer is passed to a function expecting a valid reference to an IO control CEMR session mask.
  \COUNTERMEASURE \R [CM_DCM_R_WRITE_IO_CTRL_CEMR_SESSION_MASK]

\ID SBSW_DCM_POINTER_WRITE_RECOVERYINFOCOMMCHANNELSTATE
  \DESCRIPTION    In an internal function, data is written through a pointer to a ComM channel state of the recovery
                  info, or the pointer is passed to a function expecting a valid reference to a ComM channel state of
                  the recovery info. The memory entry pointer is initialized by
                  Dcm_DiagGetRecoveryInfoComMChannelState() which will always return a pointer to a valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_RECOVERYINFOCOMMCHANNELSTATE]

\ID SBSW_DCM_POINTER_WRITE_RECOVERYINFOCOMMCONTROLSTATE
  \DESCRIPTION    In an internal function, data is written through a pointer to the communication state of a ComM
                  channel of the recovery info, or the pointer is passed to a function expecting a valid reference to
                  the communication state of a ComM channel of the recovery info. The memory entry pointer is
                  initialized by Dcm_DiagGetRecoveryInfoCommControlState() which will always return a pointer to a
                  valid memory entry.
  \COUNTERMEASURE \R [CM_DCM_R_GET_RECOVERYINFOCOMMCONTROLSTATE]

\ID SBSW_DCM_POINTER_INIT_DATA_CONTEXT
  \DESCRIPTION    The data context pointer is locally allocated and passed to Dcm_UtiInitDataContext() or 
                  Dcm_DidMgrSecureCodingLockBuffer() for initialization.
                  The caller ensures that the passed buffer pointer points to a valid memory entry
                  pointer and the passed buffer size matches to the pointed memory entry size.
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_DATA_CONTEXT
  \DESCRIPTION    In an internal function, data is written through a pointer to a data context, or the
                  pointer is passed to a function expecting a valid reference to a data context.
                  The pointer is initialized by Dcm_UtiInitDataContext(), which will always initialize it to point
                  to a valid memory entry. A data context ensures that the sum of Usage and AvailLen member is always
                  equal to or smaller than the referenced buffer size.
  \COUNTERMEASURE \R [CM_DCM_R_SET_DATA_CONTEXT_BUFFER]
                  \R [CM_DCM_R_SET_DATA_CONTEXT_USAGE]

\ID SBSW_DCM_POINTER_MEMCPY_BUFFER
  \DESCRIPTION    In an internal function, data is copied into a buffer through a pointer using memcpy, or the
                  buffer pointer with the appropriate buffer size are passed to a function expecting a valid pair
                  of buffer pointer and buffer size.
  \COUNTERMEASURE \R [CM_DCM_R_MEMCPY_PASS_BUFFER]
                  \R [CM_DCM_R_MEMCPY_WRITE]

\ID SBSW_DCM_CALL_FUNCPTR_TASKINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_TskGetTaskInfo().
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_TIMERINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_TmrGetTimerInfo() which will always return a valid function pointer.
  \COUNTERMEASURE \R [CM_DCM_R_GET_TIMERINFO]

\ID SBSW_DCM_CALL_FUNCPTR_SVCPOSTPROCESSORS
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapDiagGetPostProcessorFunc() which will always return a valid function pointer.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVCPOSTPROCESSORS]
                  \R [CM_DCM_R_GET_SVCPOSTPROCESSORS]

\ID SBSW_DCM_CALL_FUNCPTR_SVCUPDATERS
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapDiagGetUpdateFunc() which will always return a valid function pointer.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVCUPDATERS]
                  \R [CM_DCM_R_GET_SVCUPDATERS]

\ID SBSW_DCM_CALL_FUNCPTR_SVCCANCELLERS
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapDiagGetCancelFunc() which will always return a valid function pointer.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVCCANCELLERS]
                  \R [CM_DCM_R_GET_SVCCANCELLERS]

\ID SBSW_DCM_CALL_FUNCPTR_SVCPREHANDLER
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapDiagGetPreHandlerFunc() which will always return a valid function pointer.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVCPREHANDLER]
                  \R [CM_DCM_R_GET_SVCPREHANDLER]

\ID SBSW_DCM_CALL_FUNCPTR_SVCPOSTHANDLER
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapDiagGetPostHandlerFunc() which will always return a valid function pointer.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVCPOSTHANDLER]
                  \R [CM_DCM_R_GET_SVCPOSTHANDLER]

\ID SBSW_DCM_CALL_FUNCPTR_MODEMGRRULES
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapModeMgrRulesGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_MODEMGRRULES]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_PASSEDTABLETOFUNC
  \DESCRIPTION    In an internal function, iteration over all elements of a passed table to a function is done.
                  Each element is a function pointer or a structure containing only function pointer members.
                  The concrete table size is passed along the corresponding table to that function.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_OEMNOTIFICATIONINFO]
                  \M [CM_DCM_M_SIZEOF_SVCINITS]
                  \M [CM_DCM_M_SIZEOF_PROTOCOLNOTIFICATIONINFO]
                  \M [CM_DCM_M_SIZEOF_SYSNOTIFICATIONINFO]
                  \M [CM_DCM_M_SIZEOF_SESSIONNOTIFICATIONINFO]
                  \M [CM_DCM_M_SIZEOF_SECURITYNOTIFICATIONINFO]
                  \R [CM_DCM_R_LOOP]

\ID SBSW_DCM_CALL_FUNCPTR_SVCINITS
  \DESCRIPTION    In an internal function, iteration over all elements of Dcm_CfgDiagSvcInitializers is done.
                  Each element is a function pointer or a structure containing only function pointer members.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVCINITS]
                  \R [CM_DCM_R_LOOP]

\ID SBSW_DCM_LOOP
  \DESCRIPTION    In an internal function, iteration over all elements of a given context member is done.
  \COUNTERMEASURE \R [CM_DCM_R_LOOP]

\ID SBSW_DCM_POINTER_WRITE_TOBJ_ADDBUFFER
  \DESCRIPTION    In an internal function, data is written to array AddBuffer[] of a specific transport object using a
                  function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_TOBJ_ADDBUFFER]

\ID SBSW_DCM_POINTER_WRITE_COMCONTROLCHANNEL
  \DESCRIPTION    In an internal function, data is written to array Dcm_PbRamNetComCtrlChannels[] using a
                  function argument as index.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_COMCONTROLCHANNEL]
                  \R [CM_DCM_R_SET_COMCONTROLCHANNEL]

\ID SBSW_DCM_POINTER_WRITE_AUTHNVMDATA
  \DESCRIPTION    In an internal function, data is written to array Dcm_AuthMgrNvMData[] using a
                  function argument as index.
                  Alternatively, data is written to that array through a pointer.
                  In the latter case the memory entry pointer is initialized by Dcm_AuthMgrGetWLContext().
  \COUNTERMEASURE \R [CM_DCM_R_SET_AUTHNVMDATA]

\ID SBSW_DCM_ARRAY_WRITE_PENDINGAUTHDATA
  \DESCRIPTION    In an external API, data is written to array Dcm_AuthMgrSingletonContext.PendingData.PendingConnectionData[]..
  \COUNTERMEASURE \R [CM_DCM_R_SET_PENDINGAUTHDATA]

\ID SBSW_DCM_POINTER_WRITE_SERVICE_WL_ELEMENT
  \DESCRIPTION    In an internal function, data is written through a pointer to array WhiteListServiceContext.WhiteListServiceElement[]
                  of a connection white list context, or the pointer is passed to a function expecting a valid reference
                  to a connection white list context.
                  The memory entry pointer is from global variable repeater context, and a loop iterator is used as index.
                  The caller ensures that the element length matches to the pointed memory entry size.
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]
                  \R [CM_DCM_R_SET_WHITELISTSERVICEELEMENT]

\ID SBSW_DCM_POINTER_WRITE_DID_WL_ELEMENT
  \DESCRIPTION    In an internal function, data is written through a pointer to array WhiteListDidContext.WhiteListDidElement[]
                  of a connection white list context.
                  The memory entry pointer is taken from global variable repeater context, and a loop iterator is used as index.
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]
                  \R [CM_DCM_R_SET_WHITELISTDIDELEMENT]

\ID SBSW_DCM_POINTER_WRITE_RID_WL_ELEMENT
  \DESCRIPTION    In an internal function, data is written through a pointer to array WhiteListRidContext.WhiteListRidElement[]
                  of a connection white list context.
                  The memory entry pointer is taken from global variable repeater context, and a loop iterator is used as index.
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]
                  \R [CM_DCM_R_SET_WHITELISTRIDELEMENT]

\ID SBSW_DCM_POINTER_WRITE_MEM_WL_ELEMENT
  \DESCRIPTION    In an internal function, data is written through a pointer to array WhiteListMemContext.WhiteListMemElement[]
                  of a connection white list context.
                  The memory entry pointer is taken from global variable repeater context, and a loop iterator is used as index.
  \COUNTERMEASURE \R [CM_DCM_R_GET_VALID_POINTER]
                  \R [CM_DCM_R_SET_WHITELISTMEMELEMENT]

\ID SBSW_DCM_POINTER_WRITE_WL_ELEMENT
  \DESCRIPTION    The white list element pointer is passed to a function expecting a valid reference to a white list
                  element buffer. The caller ensures that the white list element points to a valid memory pointer and
                  the white list element length matches to the pointed memory entry size.
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_CERTIFICATE_DATA
  \DESCRIPTION    In an internal function, data is written through a pointer to a certificate data buffer,
                  or the certificate data pointer is passed to a function expecting a valid reference to a certificate
                  data buffer. The caller ensures that the certData member points to a valid memory entry pointer and
                  the certDataLength member matches to the pointed memory entry size.
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_CHALLENGE_SWAP_BUFFER
  \DESCRIPTION    In an internal function, data is written through a pointer to the challenge swap buffer,
                  or the challenge swap buffer pointer is passed to a function expecting a valid reference to
                  the challenge swap buffer. The caller ensures that the challenge swap buffer points to a valid memory
                  pointer and buffer length matches to the pointed memory size.
  \COUNTERMEASURE \N The pointer is taken from a global variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_CERTIFICATE_ROLE
  \DESCRIPTION    The certificate role pointer is passed to a function expecting a valid reference to a certificate
                  role buffer. The caller ensures that the role element points to a valid memory pointer and
                  the role element length matches to the pointed memory entry size.
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

\ID SBSW_DCM_POINTER_WRITE_TOBJHANDLE
  \DESCRIPTION    In an internal function, data is written to array Dcm_PbRamNetConnHdl2TObjMap[] using a
                  function argument as index.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_TOBJHANDLE]
                  \R [CM_DCM_R_SET_TOBJHANDLE]

\ID SBSW_DCM_POINTER_WRITE_DELAYTIMER
  \DESCRIPTION    In an internal function, data is written to array
                  Dcm_SingletonContext.Network.PeriodicTxContext.DelayTimer[] using a function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_DELAYTIMER]

\ID SBSW_DCM_POINTER_WRITE_TIMER
  \DESCRIPTION    In an internal function, data is written to array Dcm_TmrSingletonContext.Timer[] using a
                  function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_TIMER]

\ID SBSW_DCM_POINTER_WRITE_START_UP_FBL_RES_BUFFER
  \DESCRIPTION    In an internal function, data is written to array Dcm_SingletonContext.Diag.StartUpFbl.ResBuffer[]
                  using a function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_START_UP_FBL_RES_BUFFER]

\ID SBSW_DCM_POINTER_WRITE_ATTEMPTCOUNT
  \DESCRIPTION    In an internal function, data is written to array
                  Dcm_Svc27SingletonContext.AttemptCount[] using a function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_ATTEMPTCOUNT]

\ID SBSW_DCM_POINTER_WRITE_DELAYTIME_SEC
  \DESCRIPTION    In an internal function, data is written to array Dcm_Svc27SingletonContext.DelayTime[]
                  using a function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_DELAYTIME_SEC]

\ID SBSW_DCM_POINTER_WRITE_DELAYTIME_AUTH
  \DESCRIPTION    In an internal function, data is written to array Dcm_AuthMgrSingletonContext.DelayTime[]
                  using a function argument as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_DELAYTIME_AUTH]

\ID SBSW_DCM_POINTER_WRITE_DIDLOCKLIST
  \DESCRIPTION    In an internal function, data is written to array Dcm_RsrcMgrSingletonContext.DidLockList[] using a
                  loop iterator as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_DIDLOCKLIST]

\ID SBSW_DCM_POINTER_WRITE_RIDLOCKLIST
  \DESCRIPTION    In an internal function, data is written to array Dcm_RsrcMgrSingletonContext.RidLockList[] using a
                  loop iterator as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_RIDLOCKLIST]

\ID SBSW_DCM_POINTER_WRITE_SERVICELOCKLIST
  \DESCRIPTION    In an internal function, data is written to array Dcm_RsrcMgrSingletonContext.ServiceLockList[] using a
                  loop iterator as index.
  \COUNTERMEASURE \R [CM_DCM_R_SET_SERVICELOCKLIST]

\ID SBSW_DCM_CALL_FUNCPTR_SVC11SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc11SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC11SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC19SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc19SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC19SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC
  \DESCRIPTION    In an internal function, a function pointer is called.
                  The pointer is initialized by Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_DIDMGROPFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_DIDMGR_OP_FUNC_SIGNATURE]
                  \S [CM_DCM_S_DIDMGR_OP_FUNC_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapDidMgrIoControlSenderReceiverHandlersGetEntry() which will always return a valid function
                  pointer.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_IOCTRLSRHANDLERS]

\ID SBSW_DCM_CALL_FUNCPTR_SVC27SECLEVEL
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc27SecLevelInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC27SECLEVEL]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC27GETSEED
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc27SecLevelInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC27SECLEVEL]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_GET_SEED_FUNC_SIGNATURE]
                  \S [CM_DCM_S_GET_SEED_FUNC_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC31OPFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapRidMgrOpInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC31OPFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_ROUTINE_OP_FUNC_SIGNATURE]
                  \S [CM_DCM_S_ROUTINE_OP_FUNC_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC28SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc28SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC28SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC29SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc29SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC29SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC2CSUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc2CSubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC2CSUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC85SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc85SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC85SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_SVC86SUBFUNC
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc86SubFuncInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC86SUBFUNC]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_FUNCPTR_ROEMODESWITCH
  \DESCRIPTION    In an internal function, a function pointer is called. The index to the called function pointer
                  is a function argument.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_ROEEVENTMODESWITCHINFO]
                  \R [CM_DCM_R_GET_ROEEVENTMODESWITCHINFO]

\ID SBSW_DCM_CALL_FUNCPTR_AUTHMODESWITCH
  \DESCRIPTION    In an internal function, a function pointer is called. The index to the called function pointer
                  is a function argument.
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_CFGAUTHMGRNETCONNINFO]
                  \R [CM_DCM_R_GET_CFGAUTHMGRNETCONNINFO]

\ID SBSW_DCM_CALL_FUNCPTR_SERVICEINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapDiagServiceInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SERVICEINFO]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_CALL_READMEMORY
  \DESCRIPTION    The callout Dcm_ReadMemory uses a write-able pointer of the DCM buffer, which is always valid. The
                  application always writes as many data as the caller expects.
  \COUNTERMEASURE \S [CM_DCM_S_READMEMORY_FUNC_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC01SERVICEINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc01ServiceInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC01SERVICEINFO]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_GET_SVC01SERVICEINFO_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC08SERVICEINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc08ServiceInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC08SERVICEINFO]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_GET_SVC08SERVICEINFO_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC09SERVICEINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc09ServiceInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC09SERVICEINFO]
                  \R [CM_DCM_R_GET_VALID_POINTER]
                  \S [CM_DCM_S_GET_SVC09SERVICEINFO_WRITE_DATA_LENGTH]

\ID SBSW_DCM_CALL_FUNCPTR_SVC06MIDTIDINFO
  \DESCRIPTION    In an internal function, a function pointer is called. The pointer is initialized by
                  Dcm_CfgWrapSvc06MidTidInfoGetEntry().
  \COUNTERMEASURE \M [CM_DCM_M_SIZEOF_SVC06MIDTIDINFO]
                  \R [CM_DCM_R_GET_VALID_POINTER]

\ID SBSW_DCM_POINTER_CONVERTTOARRAY
  \DESCRIPTION    In an internal function, data is copied into a buffer through a pointer, or the
                  buffer pointer with the appropriate buffer size are passed to a function expecting a valid pair
                  of buffer pointer and buffer size.
  \COUNTERMEASURE \R [CM_DCM_R_CONVERTTOARRAY_PASS_BUFFER]
                  \R [CM_DCM_R_CONVERTTOARRAY_WRITE]
                  \S [CM_DCM_S_GEN_POINTER_FORWARD_STACK]

\ID SBSW_DCM_CALL_PROCESSREQUESTTRANSFEREXIT
  \DESCRIPTION    The callout Dcm_ProcessRequestTransferExit uses a write-able pointer of the DCM buffer, which 
                  is always valid. The amount of data which can be written is indicated by an function argument 
                  to the application.
  \COUNTERMEASURE \S [CM_DCM_S_PROCESSREQUESTTRANSFEREXIT_FUNC_WRITE_DATA_LENGTH]

\ID SBSW_DCM_POINTER_SVC31_REDIRECT_MESSAGE_CONTEXT
  \DESCRIPTION    A local message context variable is passed to Dcm_Svc31RedirectMsgContext() for
                  initialization. That initalization function ensures that all pointer members are pointing to valid
                  memory entry pointer and the corresponding buffer size members are matching to the pointed memory
                  entry sizes.
  \COUNTERMEASURE \N The pointer is taken from a local variable, as such it is always valid.

SBSW_JUSTIFICATION_END */

/*
\CM CM_DCM_N_PASS_PARAM_PTR
  The caller ensures the pointers passed to this function are valid.

\CM CM_DCM_N_ARRAY_INDEX_WRITE
  The function insures calculating the correct array size and writing in an array index within the array index limit.

\CM CM_DCM_S_GEN_PARAM_PTR_WRITE
  The user of MICROSAR Safe shall verify that none of the generated functions in Dcm_Lcfg.c modifies the pointer
  passed as an argument, but only writes a new value to it.
  SMI-37386

\CM CM_DCM_S_GEN_PARAM_PTR_FORWARD
  The user of MICROSAR Safe shall verify that none of the generated functions in Dcm_Lcfg.c modifies the pointer
  passed as an argument, but only forwards it to another function.
  SMI-37387

\CM CM_DCM_S_GEN_POINTER_FORWARD_STACK
  The user of MICROSAR Safe shall verify that none of the generated functions in Dcm_Lcfg.c modifies the pointer taken
  from a local variable, and that the function receiving those pointer parameter(s) neither stores the addresses after
  return nor writes more data to the addesses than specified by the byte size of the local variable (especially in
  case of local arrays).
  SMI-37388

\CM CM_DCM_S_DIDMGR_OP_FUNC_SIGNATURE
  The user of MICROSAR Safe shall verify that all generated OpFunc functions in table Dcm_CfgDidMgrSignalOpClassInfo
  located in Dcm_Lcfg.c match the signature indicated by the FuncClass setting.
  SMI-106112, SMI-117436, SMI-126353, SMI-163630

\CM CM_DCM_S_GET_SEED_FUNC_SIGNATURE
  The user of MICROSAR Safe shall verify that all generated GetSeedFunc functions in table Dcm_CfgSvc27SecLevelInfo
  located in Dcm_Lcfg.c match the signature indicated by the GetSeedFuncClass setting.
  SMI-40607

\CM CM_DCM_S_GET_SEED_FUNC_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated GetSeedFunc functions in table Dcm_CfgSvc27SecLevelInfo
  located in Dcm_Lcfg.c no more data will be written than specified by DCM_SAFEBSW_SVC27_MAX_SEED_LENGTH setting located
  in Dcm_Cfg.h.
  SMI-49478

\CM CM_DCM_S_ROUTINE_OP_FUNC_SIGNATURE
  The user of MICROSAR Safe shall verify that all generated OpFunc functions in table Dcm_CfgRidMgrOpInfo located
  in Dcm_Lcfg.c match the signature indicated by the OpType setting.
  SMI-64952

\CM CM_DCM_S_ROUTINE_OP_FUNC_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table Dcm_CfgRidMgrOpInfo located
  in Dcm_Lcfg.c no more data will be written than specified by the corresponding ResMaxLength setting.
  SMI-64953

\CM CM_DCM_S_ROUTINE_WRAPPER_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify all generated Dcm_RidMgr_<RID>_<Start|Stop|RequestResults> functions
  implemented in Dcm_Lcfg.c that for each Dcm_UtiGetResDataRel(pMsgContext, *<offset>*) usage on out signals the
  application does not write more data than (ResMaxLength - *<offset>*), specified by the corresponding ResMaxLength
  setting in table Dcm_CfgRidMgrOpInfo located in _Dcm_Lcfg.c.
  SMI-65597

\CM CM_DCM_S_DID_WRAPPER_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table
  Dcm_CfgDidMgrSignalOpClassInfo located in Dcm_Lcfg.c with a FuncClass setting of 0x00XX or 0x30XX, that are entirely
  or partially (e.g. through a wrapper function) implemented by the application, no more data will be written than
  specified by the corresponding MaxLength setting.
  SMI-105069

\CM CM_DCM_S_DID_WRAPPER_NV_BLOCK_ID_READ
  The user of MICROSAR Safe shall verify that in the table Dcm_CfgDidMgrSignalOpClassInfo, if OpFunc requires
  Nv Block ID, the Nv Block ID passed to the function Dcm_DidMgrNvMReadSignal() is intended to be used exclusively
  by Dcm.

  This SMI is required to fulfill SMI-22 by NvM.

  The user of MICROSAR Safe shall verify that in the table Dcm_CfgDidMgrSignalOpClassInfo, if OpFunc requires
  Nv Block ID, the Nv Block ID passed to the function Dcm_DidMgrNvMReadSignal() has a size equals to MaxLength.

  This SMI is required to fulfill SMI-23 by NvM.

  OpFunc functions that require Nv Block ID are implemented in Dcm_Lcfg.c both of:
  - with a generic name Dcm_DidMgr_<SignalName>_Read,
  - and the corresponding FuncClass equals to 0x0002.

  The table Dcm_CfgDidMgrSignalOpClassInfo is located in Dcm_Lcfg.c.
  SMI-106815, SMI-106816

\CM CM_DCM_S_DID_WRAPPER_NV_BLOCK_ID_WRITE
  The user of MICROSAR Safe shall verify that in the table Dcm_CfgDidMgrSignalOpClassInfo, if OpFunc requires
  Nv Block ID, the Nv Block ID passed to the function Dcm_DidMgrNvMWriteSignal() is intended to be used exclusively
  by Dcm.

  This SMI is required to fulfill SMI-22 by NvM.

  The user of MICROSAR Safe shall verify that in the table Dcm_CfgDidMgrSignalOpClassInfo, if OpFunc requires
  Nv Block ID, the Nv Block ID passed to the function Dcm_DidMgrNvMWriteSignal() has a size equals to MaxLength.

  This SMI is required to fulfill SMI-23 by NvM.

  OpFunc functions that require Nv Block ID are implemented in Dcm_Lcfg.c both of:
  - with a generic name Dcm_DidMgr_<SignalName>_Write,
  - and the corresponding FuncClass equals to 0x1002.

  The table Dcm_CfgDidMgrSignalOpClassInfo is located in Dcm_Lcfg.c.
  SMI-117437, SMI-117438

\CM CM_DCM_S_ROE_NV_BLOCK_ID_ACCESS
  The user of MICROSAR Safe shall verify that the Nv Block with the ID defined by DCM_SVC_86_NVRAM_BLOCKID located
  in Dcm_Cfg.h is intended to be used exclusively by Dcm.

  This SMI is required to fulfill SMI-22 by NvM.

  The user of MICROSAR Safe shall verify that the Nv Block with the ID defined by DCM_SVC_86_NVRAM_BLOCKID located
  in Dcm_Cfg.h has a size equals to the size of the structure data type Dcm_ExtSvc86NvMDataType.

  This SMI is required to fulfill SMI-23 by NvM.
  SMI-163518, SMI-163519

\CM CM_DCM_S_SVC29_NV_BLOCK_ID_ACCESS
  The user of MICROSAR Safe shall verify that each Nv Block, with the ID defined by NvMBlockId setting of table
  Dcm_CfgAuthMgrNetConnInfo located in Dcm_Lcfg.c, is intended to be used exclusively by Dcm.

  This SMI is required to fulfill SMI-22 by NvM.

  The user of MICROSAR Safe shall verify that each Nv Block, with the ID defined by NvMBlockId setting of table
  Dcm_CfgAuthMgrNetConnInfo located in Dcm_Lcfg.c, has a size of the structure data type Dcm_AuthMgrNvMDataType.

  This SMI is required to fulfill SMI-23 by NvM.
  SMI-440519, SMI-440520

\CM CM_DCM_S_READMEMORY_FUNC_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that the implementation of the call-out function Dcm_ReadMemory() will not
  write more data than specified by the MemorySize parameter.
  SMI-74833

\CM CM_DCM_S_DIDMGR_OP_FUNC_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table
  Dcm_CfgDidMgrSignalOpClassInfo located in Dcm_Lcfg.c with a FuncClass setting of 0x00XX or 0x30XX, that are entirely
  or partially (e.g. through a wrapper function) implemented by the application, no more data will be written than
  specified by the corresponding MaxLength setting.

  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table
  Dcm_CfgDidMgrSignalOpClassInfo located in Dcm_Lcfg.c with a FuncClass setting of 0x0003 no more data will be written
  than specified by DCM_DIDMGR_READ_PAGE_SIZE setting located in Dcm_Cfg.h.
  SMI-105069, SMI-105312

\CM CM_DCM_S_GET_SVC01SERVICEINFO_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table Dcm_CfgSvc01ServiceInfoTable
  located in Dcm_Lcfg.c no more data will be written than specified by the corresponding ResLength setting.
  SMI-147940

\CM CM_DCM_S_GET_SVC08SERVICEINFO_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table Dcm_CfgSvc08ServiceInfoTable
  located in Dcm_Lcfg.c no more data will be written than specified by the corresponding ResLength setting.
  SMI-143395

\CM CM_DCM_S_GET_SVC09SERVICEINFO_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that for all generated OpFunc functions in table Dcm_CfgSvc09ServiceInfoTable
  located in Dcm_Lcfg.c no more data will be written than specified by the corresponding ResLength setting.
  SMI-147938

\CM CM_DCM_S_PROCESSREQUESTTRANSFEREXIT_FUNC_WRITE_DATA_LENGTH
  The user of MICROSAR Safe shall verify that the implementation of the call-out function
  Dcm_ProcessRequestTransferExit() will not write more data than specified by the transferResponseParameterRecordSize
  parameter.
  SMI-579281

\CM CM_DCM_N_CALL_PARAM_FUNCPTR
  The caller ensures the pointers passed to this function are valid.

\CM CM_DCM_M_CONSISTENCY_BUFFERINFO
  Verify for each element in Dcm_CfgNetBufferInfo[] that the member 'Size' matches to the size of the array the
  member 'BufferPtr' is pointing to.

\CM CM_DCM_M_SIZEOF_BUFFERINFO
  Verify Dcm_PbCfgNetBufferInfo[] is defined for each id in range [0..Dcm_NetPbCfgNumBuffers[.

\CM CM_DCM_M_SIZEOF_COMMCONTEXT
  Verify Dcm_PbRamNetComMContext[] is defined for each id in range [0..Dcm_NetPbCfgNumComMChannels[.

\CM CM_DCM_M_SIZEOF_BUFFERCONTEXT
  Verify Dcm_PbRamNetBufferContext[] is defined for each id in range [0..Dcm_NetPbCfgNumBuffers[.

\CM CM_DCM_M_SIZEOF_TRANSPORTOBJECT
  Verify Dcm_PbRamNetTransportObject[] is defined for each id in range [0..Dcm_NetPbCfgNumTransportObjects[.

\CM CM_DCM_M_SIZEOF_COMCONTROLCHANNEL
  Verify Dcm_PbRamNetComCtrlChannels[] is defined for each id in range [0..Dcm_NetPbCfgNumComMChannels[.

\CM CM_DCM_M_SIZEOF_TOBJHANDLE
  Verify Dcm_PbRamNetConnHdl2TObjMap[] is defined for each id in range [0..Dcm_NetPbCfgNumConnections[.

\CM CM_DCM_M_SIZEOF_PERTXOBJ
  Verify Dcm_PbRamNetPeriodicTxObject[] is defined for each id in range [0..Dcm_NetPbCfgNumPerTxObjects[.

\CM CM_DCM_M_SIZEOF_SVCPOSTPROCESSORS
  Verify Dcm_CfgDiagSvcPostProcessors[] is defined for each id in range [0..DCM_CFGDIAGSVCPOSTPROCESSORS_SIZE[.

\CM CM_DCM_M_SIZEOF_SVCUPDATERS
  Verify Dcm_CfgDiagSvcUpdaters[] is defined for each id in range [0..DCM_CFGDIAGSVCUPDATERS_SIZE[.

\CM CM_DCM_M_SIZEOF_SVCCANCELLERS
  Verify Dcm_CfgDiagSvcCancellers[] is defined for each id in range [0..DCM_CFGDIAGSVCCANCELLERS_SIZE[.

\CM CM_DCM_M_SIZEOF_SVCPREHANDLER
  Verify Dcm_CfgDiagSvcPreHandlers[] is defined for each id in range [0..DCM_CFGDIAGSVCPREHANDLERS_SIZE[.

\CM CM_DCM_M_SIZEOF_SVCPOSTHANDLER
  Verify Dcm_CfgDiagSvcPostHandlers[] is defined for each id in range [0..DCM_CFGDIAGSVCPOSTHANDLERS_SIZE[.

\CM CM_DCM_M_SIZEOF_MODEMGRRULES
  Verify Dcm_CfgModeMgrRules[] is defined for each id in range [0..DCM_CFGMODEMGRRULES_SIZE[.

\CM CM_DCM_M_SIZEOF_SVCINITS
  Verify Dcm_CfgDiagSvcInitializers[] is defined for each id in range [0..DCM_CFGDIAGSVCINITIALIZERS_SIZE[.

\CM CM_DCM_M_SIZEOF_PROTOCOLNOTIFICATIONINFO
  Verify Dcm_CfgNetProtocolNotificationInfo[] is defined for each id in range [0..DCM_CFGNETPROTOCOLNOTIFICATIONINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SYSNOTIFICATIONINFO
  Verify Dcm_CfgDiagSysNotificationInfo[] is defined for each id in range [0..DCM_CFGDIAGSYSNOTIFICATIONINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_OEMNOTIFICATIONINFO
  Verify Dcm_CfgDiagOemNotificationInfo[] is defined for each id in range [0..DCM_CFGDIAGOEMNOTIFICATIONINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SESSIONNOTIFICATIONINFO
  Verify Dcm_CfgStateSessionNotificationInfo[] is defined for each id in range [0..DCM_CFGSTATESESSIONNOTIFICATIONINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SECURITYNOTIFICATIONINFO
  Verify Dcm_CfgStateSecurityNotificationInfo[] is defined for each id in range [0..DCM_CFGSTATESECURITYNOTIFICATIONINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC11SUBFUNC
  Verify Dcm_CfgSvc11SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC11SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC19SUBFUNC
  Verify Dcm_CfgSvc19SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC19SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_DIDMGROPFUNC
  Verify Dcm_CfgDidMgrSignalOpClassInfo[] is defined for each id in range [0..DCM_CFGDIDMGRSIGNALOPCLASSINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_IOCTRLSRHANDLERS
  Verify Dcm_CfgDidMgr_IOControlSenderReceiverHandlers[] is defined for each id in range
  [0..DCM_CFGDIDMGR_IOCONTROLSENDERRECEIVERHANDLERS_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC27SECLEVEL
  Verify Dcm_CfgSvc27SecLevelInfo[] is defined for each id in range [0..DCM_CFGSVC27SECLEVELINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC31OPFUNC
  Verify Dcm_CfgRidMgrOpInfo[] is defined for each id in range [0..DCM_CFGRIDMGROPINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC28SUBFUNC
  Verify Dcm_CfgSvc28SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC28SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC29SUBFUNC
  Verify Dcm_CfgSvc29SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC29SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC2CSUBFUNC
  Verify Dcm_CfgSvc2CSubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC2CSUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC85SUBFUNC
  Verify Dcm_CfgSvc85SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC85SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC86SUBFUNC
  Verify Dcm_CfgSvc86SubFuncInfo[] is defined for each id in range [0..DCM_CFGSVC86SUBFUNCINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_ROEEVENTMODESWITCHINFO
  Verify Dcm_CfgModeRoEEventModeSwitchInfoTable[] is defined for each id in range [0..DCM_CFGMODEROEEVENTMODESWITCHINFOTABLE_SIZE[.

\CM CM_DCM_M_SIZEOF_CFGAUTHMGRNETCONNINFO
  Verify Dcm_CfgAuthMgrNetConnInfo[] is defined for each id in range [0..DCM_CFGAUTHMGRNETCONNINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SERVICEINFO
  Verify Dcm_CfgDiagServiceInfo[] is defined for each id in range [0..DCM_CFGDIAGSERVICEINFO_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC01SERVICEINFO
  Verify Dcm_CfgSvc01ServiceInfoTable[] is defined for each id in range [0..DCM_CFGSVC01SERVICEINFOTABLE_SIZE[.

 \CM CM_DCM_M_SIZEOF_SVC06MIDTIDINFO
  Verify Dcm_CfgSvc06MidTidInfoTable[] is defined for each id in range [0..DCM_CFGSVC06MIDTIDINFOTABLE_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC08SERVICEINFO
  Verify Dcm_CfgSvc08ServiceInfoTable[] is defined for each id in range [0..DCM_CFGSVC08SERVICEINFOTABLE_SIZE[.

\CM CM_DCM_M_SIZEOF_SVC09SERVICEINFO
  Verify Dcm_CfgSvc09ServiceInfoTable[] is defined for each id in range [0..DCM_CFGSVC09SERVICEINFOTABLE_SIZE[.

\CM CM_DCM_M_MAX_ZERO_SEED_LENGTH
  Verify for each element in Dcm_CfgSvc27SecLevelInfo[] that the member 'SeedLength' is smaller than
  or equal to DCM_SAFEBSW_SVC27_MAX_SEED_LENGTH.

\CM CM_DCM_R_CHECK_REMAINING_BUFFER
  Before writing data to a buffer, check that the buffer is large enough for the data written to it.

\CM CM_DCM_R_GET_BUFFERINFO
  Dcm_NetPbCfgGetBufferInfo() will always return a valid buffer info element even when the passed memory index is
  not a handle to a buffer info element.

\CM CM_DCM_R_GET_COMMCONTEXT
  Dcm_NetPbCfgGetComMContext() will always return a valid ComM context element even when the passed memory index is
  not a handle to a ComM context element.

\CM CM_DCM_R_GET_BUFFERCONTEXT
  Dcm_NetPbCfgGetBufferContext() will always return a valid buffer context element even when the passed memory index is
  not a handle to a buffer context element.

\CM CM_DCM_R_GET_TRANSPORTOBJECT
  Dcm_NetPbCfgGetTransportObject() will always return a valid transport object even when the passed memory index is
  not a handle to a transport object.

\CM CM_DCM_R_GET_TIMERCONTEXT
  Dcm_TmrGetTimerContext() will always return a valid transport object even when the passed memory index is
  not a handle to a timer context.

\CM CM_DCM_R_GET_BITSETROWINDEX
  Dcm_UtiBitSetGetRowIdxSafe() will return a index within the given bitset array, even when the passed bit position is
  not within the bitset.

\CM CM_DCM_R_BITSET_BASETYPE
  Verify that the base type passed to the bitset manipulation macro matches the base type of the respective bitset
  array.

\CM CM_DCM_R_GET_2ASCHEDULERENTRY
  Dcm_Svc2ASchdGetEntry() will return a valid entry of the 0x2A scheduler table, even when the passed memory index is
  not a handle to a scheduler table entry.

\CM CM_DCM_R_GET_2CSRCITEM
  Dcm_Svc2CDefMgrGetSrcItem() will return a valid entry of the 0x2C source item table, even when the passed memory index is
  not a handle to a source item table entry.

\CM CM_DCM_R_GET_2CITEM
  Dcm_Svc2CDefMgrGetItem() will return a valid entry of the 0x2C item table, even when the passed memory index is not a handle
  to an item table entry.

\CM CM_DCM_R_GET_2CPROCESSCONTEXT
  Dcm_Svc2CDefMgrGetDDDidProcessContext() will return a valid process context of a DID, even when the passed memory index is
  not a handle to a process context.

\CM CM_DCM_R_SET_2FACTIVEIODIDS
  Verify that the array index is in range before writing to Dcm_Svc2FSingletonContext.ActiveIoDids[].

\CM CM_DCM_R_SET_86RINGBUFFER
  Verify that the array index is in range before writing to Dcm_Svc86Schd_09DtcRingBuffer.DtcQueue[].

\CM CM_DCM_R_GET_86DIDRECORD
  Dcm_Svc86Schd_03GetDidRecord() or Dcm_Svc86Schd_07GetDidRecord() will return a valid entry of the DID record table, even when
  the passed memory index is not a handle to a DID record table entry.

\CM CM_DCM_R_GET_VSGACTIVECFGVARIANTS
  Verify that the array index is in range before writing to Dcm_VarMgrSingletonContext.ActiveCfgVariants[].

\CM CM_DCM_R_GET_PERIODICTXBUFFER
  Dcm_NetPeriodicMsgGetTxBuffer() will return a valid tx buffer of a periodic message, even when the passed memory index
  is not a handle to a periodic tx object.

\CM CM_DCM_R_GET_RXPDUINFO
  Verify that the local pointer initialized as a NULL_PTR is set after calling Dcm_NetProvideRxBuffer().

\CM CM_DCM_R_GET_THREADCONTEXT
  Dcm_GetThreadContext() will always return a valid thread info pointer even when the passed memory index is
  not a handle to a thread info.

\CM CM_DCM_R_GET_DIAGTHREADCONTEXT
  Dcm_GetDiagThreadContext() will always return a valid thread info pointer even when the passed memory index is
  not a handle to a thread info.

\CM CM_DCM_R_GET_DEBUGTHREADCONTEXT
  Dcm_DebugGetThreadContext() will always return a valid thread info pointer even when the passed memory index is
  not a handle to a thread info.

\CM CM_DCM_R_GET_SVC31THREADCONTEXT
  Dcm_Svc31GetThreadContext() will always return a valid thread info pointer even when the passed memory index is
  not a handle to a thread info.

\CM CM_DCM_R_GET_PAGEDBUFFERCONTEXT
  Dcm_PagedBufferContext() will always return a valid pointer even when the passed memory index is
  not a handle to a paged-buffer context.

\CM CM_DCM_R_GET_TIMERINFO
  Dcm_TmrGetTimerInfo() will always return a valid timer info even when the passed memory index is
  not a handle to a timer info.

\CM CM_DCM_R_GET_SVCPOSTPROCESSORS
  Dcm_CfgWrapDiagGetPostProcessorFunc() will always return a valid function pointer even when the passed memory index is
  not a handle to a post-processor function.

\CM CM_DCM_R_GET_SVCUPDATERS
  Dcm_CfgWrapDiagGetUpdateFunc() will always return a valid function pointer even when the passed memory index is
  not a handle to an update function.

\CM CM_DCM_R_GET_SVCCANCELLERS
  Dcm_CfgWrapDiagGetCancelFunc() will always return a valid function pointer even when the passed memory index is
  not a handle to a cancellation function.

\CM CM_DCM_R_GET_SVCPREHANDLER
  Dcm_CfgWrapDiagGetPreHandlerFunc() will always return a valid function pointer even when the passed memory index is
  not a handle to a pre handler function.

\CM CM_DCM_R_GET_SVCPOSTHANDLER
  Dcm_CfgWrapDiagGetPostHandlerFunc() will always return a valid function pointer even when the passed memory index is
  not a handle to a post handler function.

\CM CM_DCM_R_TERMINATED_LIST
  Verify that iteration over all elements of a given table is only done until NULL_PTR element.

\CM CM_DCM_R_LOOP
  The for loop or while loop with constrains ensures that the index never exceeds the size of the given table.

\CM CM_DCM_R_SET_TOBJ_ADDBUFFER
  Verify that the array index is in range before writing to AddBfufer[] of a specific transport object.

\CM CM_DCM_R_SET_COMCONTROLCHANNEL
  Verify that the array index is in range before writing to Dcm_PbRamNetComCtrlChannels[].

\CM CM_DCM_R_SET_AUTHNVMDATA
  Verify that the array index is in range before writing to Dcm_AuthMgrNvMData[].

\CM CM_DCM_R_SET_PENDINGAUTHDATA
  Verify that the array index is in range before writing to Dcm_AuthMgrSingletonContext.PendingData.Role[],
  Dcm_AuthMgrSingletonContext.PendingData.State[] or Dcm_AuthMgrSingletonContext.PendingData.WhiteLists[].

\CM CM_DCM_R_SET_WHITELISTSERVICEELEMENT
  Verify that the array index is in range before writing to pWhiteListServiceContext->WhiteListServiceElement[].

\CM CM_DCM_R_SET_WHITELISTDIDELEMENT
  Verify that the array index is in range before writing to pWhiteListDidContext->WhiteListDidElement[].

\CM CM_DCM_R_SET_WHITELISTRIDELEMENT
  Verify that the array index is in range before writing to pWhiteListRidContext->WhiteListRidElement[].

\CM CM_DCM_R_SET_WHITELISTMEMELEMENT
  Verify that the array index is in range before writing to pWhiteListMemContext->WhiteListMemElement[].

\CM CM_DCM_R_SET_TOBJHANDLE
  Verify that the array index is in range before writing to Dcm_PbRamNetConnHdl2TObjMap[].

\CM CM_DCM_R_SET_TIMER
  Verify that the array index is in range before writing to Dcm_TmrSingletonContext.Timer[].

\CM CM_DCM_R_SET_START_UP_FBL_RES_BUFFER
  Verify that the array index is in range before writing to Dcm_SingletonContext.Diag.StartUpFbl.ResBuffer[].

\CM CM_DCM_R_SET_ATTEMPTCOUNT
  Verify that the array index is in range before writing to Dcm_Svc27SingletonContext.AttemptCount[].

\CM CM_DCM_R_SET_DELAYTIME_SEC
  Verify that the array index is in range before writing to Dcm_Svc27SingletonContext.DelayTime[].

\CM CM_DCM_R_SET_DELAYTIME_AUTH
  Verify that the array index is in range before writing to Dcm_AuthMgrSingletonContext.DelayTime[].

\CM CM_DCM_R_SET_DIDLOCKLIST
  Verify that the array index is in range before writing to Dcm_RsrcMgrSingletonContext.DidLockList[].

\CM CM_DCM_R_SET_RIDLOCKLIST
  Verify that the array index is in range before writing to Dcm_RsrcMgrSingletonContext.RidLockList[].

\CM CM_DCM_R_SET_SERVICELOCKLIST
  Verify that the array index is in range before writing to Dcm_RsrcMgrSingletonContext.ServiceLockList[].

\CM CM_DCM_R_SET_PERTXOBJ
  Verify that the array index is in range before writing to Dcm_PbRamNetPeriodicTxObject[].

\CM CM_DCM_R_SET_DELAYTIMER
  Verify that the array index is in range before writing to Dcm_SingletonContext.Network.PeriodicTxContext.DelayTimer[].

\CM CM_DCM_R_SET_DATA_CONTEXT_BUFFER
  Verify that the array index is in range [0..pDataContext->Size[ before writing to pDataContext->Buffer.

\CM CM_DCM_R_SET_MESSAGE_CONTEXT_RESBUFFER
  Verify that the array index is in range [0..pMsgContext->resBufSize[ before writing to pMsgContext->resData.

\CM CM_DCM_R_WRITE_IO_CTRL_CEMR_SESSION_MASK
  Verify that the write access to the bitmask Dcm_DidMgrIoControlCemrSessionMask does not exceed its
  size (DCM_DIDMGR_IODID_MAX_EXT_CEMR_SIZE).
  
\CM CM_DCM_R_INIT_MESSAGE_CONTEXT
  Ensure that the message context is initialized via Dcm_DiagInitMsgContextBufferInfo() before using it.

\CM CM_DCM_R_SET_MESSAGE_CONTEXT_REQBUFFER
  Verify that the array index is in range [0..pMsgContext->reqBufSize[ before writing to pMsgContext->reqData.

\CM CM_DCM_R_SET_DATA_CONTEXT_USAGE
  Verify that the sum of pDataContext->Usage and pDataContext->AvailLen is always equal to or smaller as
  pDataContext->Size.

\CM CM_DCM_R_MEMCPY_PASS_BUFFER
  Verify that the passed pair of buffer pointer and buffer size matches to referenced memory entry and memory size.

\CM CM_DCM_R_MEMCPY_WRITE
  Verify that the array index is in range between 0 and the passed buffer size before writing to the passed
  buffer pointer.

\CM CM_DCM_R_GET_DIDINFOCONTEXT
  Dcm_Svc22GetDidInfoContext() will always return a valid Did info context even when the passed memory index is
  not a handle to a Did info context.

\CM CM_DCM_R_GET_CFGAUTHMGRNETCONNINFO
  Verify that the array index is in range before getting the mode switch function from Dcm_CfgAuthMgrNetConnInfo[].

\CM CM_DCM_R_GET_ROEEVENTMODESWITCHINFO
  Verify that the array index is in range before getting the mode switch function from Dcm_CfgModeRoEEventModeSwitchInfoTable[].

\CM CM_DCM_R_GET_RECOVERYINFOCOMMCHANNELSTATE
  Dcm_DiagGetRecoveryInfoComMChannelState() will return a valid ComM channel state of the recovery info, even when the
  passed memory index is not a handle to a ComM channel state of the recovery info.

\CM CM_DCM_R_GET_RECOVERYINFOCOMMCONTROLSTATE
  Dcm_DiagGetRecoveryInfoCommControlState() will return a valid communication state of a ComM channel of the recovery
  info, even when the passed memory index is not a handle to a communication state of a ComM channel of the recovery
  info.

\CM CM_DCM_R_CONVERTTOARRAY_PASS_BUFFER
  Verify that the passed pair of buffer pointer and buffer size matches to referenced memory entry and memory size.

\CM CM_DCM_R_CONVERTTOARRAY_WRITE
  Verify that the array index is in range between 0 and the passed buffer size before writing to the passed
  buffer pointer.

\CM CM_DCM_R_GET_VALID_POINTER
  The pointer concerned is initialized with a getter-function by memory index.
  The caller must explicitly verify that the initialization was successful before forwarding or writing to the pointer,
  unless when it can be guaranteed that the passed memory index is valid.

*/
/**********************************************************************************************************************
 *  END OF FILE: Dcm.c
 *********************************************************************************************************************/
