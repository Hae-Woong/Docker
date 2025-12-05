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
/**        \file  Dcm_Repeater.c
 *         \unit  Repeater
 *        \brief  Contains the implementation of Repeater unit.
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
#define DCM_REPEATER_SOURCE

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
#include "Dcm_Repeater.h"
#include "Dcm_Uti.h"
#include "Dcm_CfgWrap.h"
#include "Dcm_Debug.h"
#include "Dcm_RsrcMgr.h"
#include "Dcm_Diag.h"
#include "Dcm_Net.h"
#include "Dcm_NetPbCfg.h"

#include "Dcm_Svc01.h"
#include "Dcm_Svc02.h"
#include "Dcm_Svc03.h"
#include "Dcm_Svc04.h"
#include "Dcm_Svc06.h"
#include "Dcm_Svc07.h"
#include "Dcm_Svc08.h"
#include "Dcm_Svc09.h"
#include "Dcm_Svc0A.h"
#include "Dcm_Svc10.h"
#include "Dcm_Svc11.h"
#include "Dcm_Svc14.h"
#include "Dcm_Svc19.h"
#include "Dcm_Svc22.h"
#include "Dcm_Svc23.h"
#include "Dcm_Svc24.h"
#include "Dcm_Svc27.h"
#include "Dcm_Svc28.h"
#include "Dcm_Svc29.h"
#include "Dcm_Svc2A.h"
#include "Dcm_Svc2C.h"
#include "Dcm_Svc2E.h"
#include "Dcm_Svc2F.h"
#include "Dcm_Svc31.h"
#include "Dcm_Svc34.h"
#include "Dcm_Svc35.h"
#include "Dcm_Svc36.h"
#include "Dcm_Svc37.h"
#include "Dcm_Svc3D.h"
#include "Dcm_Svc85.h"
#include "Dcm_Svc86.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#define Dcm_RepeaterResetOpStatus(pRepeaterContext)                  (pRepeaterContext->OpStatus = DCM_INITIAL)                                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Context information of service processor repeater of each service processor */
union DCM_REPEATERPROXYSVCCONTEXTTYPE_TAG
{                                                                                                                                                    /* PRQA S 0750 */ /* MD_Dcm_Rule19.2 */
#if (DCM_SVC_01_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc01RepeaterProxyContextType Svc01;  /*!< Service 0x01 temporary processing context */
#endif
#if (DCM_SVC_02_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc02RepeaterProxyContextType Svc02;  /*!< Service 0x02 temporary processing context */
#endif
#if (DCM_SVC_03_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc03RepeaterProxyContextType Svc03;  /*!< Service 0x03 temporary processing context */
#endif
#if (DCM_SVC_04_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc04RepeaterProxyContextType Svc04;  /*!< Service 0x04 temporary processing context */
#endif
#if (DCM_SVC_06_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc06RepeaterProxyContextType Svc06;  /*!< Service 0x06 temporary processing context */
#endif
#if (DCM_SVC_07_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc07RepeaterProxyContextType Svc07;  /*!< Service 0x07 temporary processing context */
#endif
#if (DCM_SVC_08_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc08RepeaterProxyContextType Svc08;  /*!< Service 0x08 temporary processing context */
#endif
#if (DCM_SVC_09_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc09RepeaterProxyContextType Svc09;  /*!< Service 0x09 temporary processing context */
#endif
#if (DCM_SVC_0A_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc0ARepeaterProxyContextType Svc0A;  /*!< Service 0x0A temporary processing context */
#endif
  Dcm_Svc10RepeaterProxyContextType Svc10;  /*!< Service 0x10 temporary processing context */
#if (DCM_SVC_11_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc11RepeaterProxyContextType Svc11;  /*!< Service 0x11 temporary processing context */
#endif
#if (DCM_SVC_14_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc14RepeaterProxyContextType Svc14;  /*!< Service 0x14 temporary processing context */
#endif
#if (DCM_SVC_19_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc19RepeaterProxyContextType Svc19;  /*!< Service 0x19 temporary processing context */
#endif
#if (DCM_SVC_22_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc22RepeaterProxyContextType Svc22;  /*!< Service 0x22 temporary processing context */
#endif
#if (DCM_SVC_23_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc23RepeaterProxyContextType Svc23;  /*!< Service 0x23 temporary processing context */
#endif
#if (DCM_SVC_24_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc24RepeaterProxyContextType Svc24;  /*!< Service 0x24 temporary processing context */
#endif
#if (DCM_SVC_27_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc27RepeaterProxyContextType Svc27;  /*!< Service 0x27 temporary processing context */
#endif
#if (DCM_SVC_28_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc28RepeaterProxyContextType Svc28;  /*!< Service 0x28 temporary processing context */
#endif
#if (DCM_SVC_29_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc29RepeaterProxyContextType Svc29;  /*!< Service 0x29 temporary processing context */
#endif
#if (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc2ARepeaterProxyContextType Svc2A;  /*!< Service 0x2A temporary processing context */
#endif
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc2CRepeaterProxyContextType Svc2C;  /*!< Service 0x2C temporary processing context */
#endif
#if (DCM_SVC_2E_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc2ERepeaterProxyContextType Svc2E;  /*!< Service 0x2E temporary processing context */
#endif
#if (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc2FRepeaterProxyContextType Svc2F;  /*!< Service 0x2F temporary processing context */
#endif
#if (DCM_SVC_31_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc31RepeaterProxyContextType Svc31;  /*!< Service 0x31 temporary processing context */
#endif
#if (DCM_SVC_34_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc34RepeaterProxyContextType Svc34;  /*!< Service 0x34 temporary processing context */
#endif
#if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc35RepeaterProxyContextType Svc35;  /*!< Service 0x35 temporary processing context */
#endif
#if (DCM_SVC_36_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc36RepeaterProxyContextType Svc36;  /*!< Service 0x36 temporary processing context */
#endif
#if (DCM_SVC_37_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc37RepeaterProxyContextType Svc37;  /*!< Service 0x37 temporary processing context */
#endif
#if (DCM_SVC_3D_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc3DRepeaterProxyContextType Svc3D;  /*!< Service 0x3D temporary processing context */
#endif
#if (DCM_SVC_85_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc85RepeaterProxyContextType Svc85;  /*!< Service 0x85 temporary processing context */
#endif
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86RepeaterProxyContextType Svc86;  /*!< Service 0x86 temporary processing context */
#endif
};
typedef union DCM_REPEATERPROXYSVCCONTEXTTYPE_TAG Dcm_RepeaterProxySvcContextType;

/*! Diagnostic service processor repeater context */
struct DCM_REPEATERTHREADCONTEXTTYPE_TAG
{
  Dcm_RepeaterProxySvcContextType Context;     /*!< Service specific repeater contexts */
  uint8                           CalleeIdx;   /*!< Index of the next function to call */
  Dcm_OpStatusType                OpStatus;    /*!< Current operation status */
  Dcm_RepeaterProgressType        Progress;    /*!< Progress of the repeater */
  Dcm_RepeaterProgressType        SubProgress; /*!< Progress of a sub-repeater */
};
typedef struct DCM_REPEATERTHREADCONTEXTTYPE_TAG Dcm_RepeaterThreadContextType;
typedef P2VAR(Dcm_RepeaterThreadContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_RepeaterThreadContextPtrType;
typedef P2CONST(Dcm_RepeaterThreadContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_RepeaterThreadContextConstPtrType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetThreadContext()
 *********************************************************************************************************************/
/*! \brief          Returns a repeater unit thread context element of the given index.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid repeater unit thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_RepeaterThreadContextPtrType, DCM_CODE) Dcm_RepeaterGetThreadContext(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_RepeaterProcessSvcPreHandler()
 *********************************************************************************************************************/
/*! \brief         Process service pre handler function.
 *  \details       -
 *  \param[in]     pContext         Pointer to the context
 *  \param[in]     opStatus         The operation status
 *  \param[in]     pServiceInfo     Pointer to the service info
 *  \param[out]    ErrorCode        The NRC
 *  \return        DCM_E_LOOP       Repeat Repeater Proxy immediately
 *  \return        DCM_E_PENDING    Repeat Repeater Proxy next cylce
 *  \return        DCM_E_NOT_OK     Send negative response
 *  \context       TASK
 *  \reentrant     FALSE
 *  \pre           -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterProcessSvcPreHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_CfgDiagServiceInfoPtrType pServiceInfo,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_RepeaterProcessSvcPostHandler()
 *********************************************************************************************************************/
/*! \brief         Process service post handler function.
 *  \details       -
 *  \param[in]     pContext         Pointer to the context
 *  \param[in]     opStatus         The operation status
 *  \param[in]     pServiceInfo     Pointer to the service info
 *  \param[out]    ErrorCode        The NRC
 *  \return        DCM_E_OK         Send positive response
 *  \return        DCM_E_PENDING    Repeat Repeater Proxy next cylce
 *  \return        DCM_E_NOT_OK     Send negative response
 *  \context       TASK
 *  \reentrant     FALSE
 *  \pre           -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterProcessSvcPostHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_CfgDiagServiceInfoPtrType pServiceInfo,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_RepeaterProcessSvcProcessor()
 *********************************************************************************************************************/
/*! \brief         Process service processor.
 *  \details       -
 *  \param[in]     pContext               Pointer to the context
 *  \param[in]     opStatus               The operation status
 *  \param[in]     pServiceInfo           Pointer to the service info
 *  \param[out]    ErrorCode              The NRC
 *  \return        DCM_E_LOOP             Repeat Repeater Proxy immediately
 *  \return        DCM_E_PENDING          Repeat Repeater Proxy next cylce
 *  \return        DCM_E_FORCE_RCRRP      Repeat Repeater Proxy next cylce and send RCRRP response
 *  \return        DCM_E_STOP_REPEATER    Leave Repeater Proxy
 *  \return        DCM_E_NOT_OK           Send negative response
 *  \context       TASK
 *  \reentrant     FALSE
 *  \pre           -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterProcessSvcProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_CfgDiagServiceInfoPtrType pServiceInfo,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
DCM_LOCAL VAR(Dcm_RepeaterThreadContextType, DCM_VAR_NOINIT) Dcm_RepeaterThreadContext[DCM_NUM_THREADS];                                             /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterGetThreadContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_RepeaterThreadContextPtrType, DCM_CODE) Dcm_RepeaterGetThreadContext(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_RepeaterThreadContextPtrType pRepeaterContext;

  if (Dcm_DebugDetectRuntimeError(threadId >= DCM_NUM_THREADS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pRepeaterContext = &Dcm_RepeaterThreadContext[0];
  }
  else
  {
    pRepeaterContext = &Dcm_RepeaterThreadContext[threadId];
  }

  return pRepeaterContext;
}

/**********************************************************************************************************************
 *  Dcm_RepeaterProcessSvcPreHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterProcessSvcPreHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_CfgDiagServiceInfoPtrType pServiceInfo,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_DiagSvcPreHandlerFuncType      lPreHandlerFunc;
  Dcm_ReadOnlyMsgContextPtrType      pMsgContext = Dcm_DiagGetMsgContext(pContext->ThreadId);
  Dcm_NetTransportObjectConstPtrType pTranspObj = Dcm_DiagGetTranspObj(pContext->ThreadId);

  lPreHandlerFunc = Dcm_CfgWrapDiagGetPreHandlerFunc(pServiceInfo->PreHandlerFuncRef);

  lStdResult = lPreHandlerFunc(opStatus
                              ,Dcm_DiagGetSid(pContext->ThreadId)
                              ,Dcm_UtiGetReqDataAt(pMsgContext,0)
                              ,pMsgContext->reqDataLen
                              ,Dcm_NetPbCfgGetProtocolIdOfTranspObj(pTranspObj)                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
                              ,pTranspObj->ClientSrcAddr
                              ,Dcm_NetPbCfgGetConnId(pTranspObj->ConnHdl)
                              ,(uint8)pMsgContext->msgAddInfo.reqType
                              ,ErrorCode);                                                                                                           /* SBSW_DCM_CALL_FUNCPTR_SVCPREHANDLER */

  switch (lStdResult)
  {
    case E_OK:
      Dcm_RepeaterNextStep(pContext, DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL);                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP; /* SpeedUp Processing */
      break;
    case DCM_E_PENDING:
      /* Fall through */
    case E_NOT_OK:
      Dcm_UtiHandleApplNrc(lStdResult, ErrorCode, DCM_E_CONDITIONSNOTCORRECT);                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;
    default:
      lStdResult = DCM_E_NOT_OK;
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_DebugReportError(DCM_SID_PREHANDLERFUNC, DCM_E_INTERFACE_RETURN_VALUE);
      break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_RepeaterProcessSvcPostHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterProcessSvcPostHandler(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_CfgDiagServiceInfoPtrType pServiceInfo,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_DiagSvcPostHandlerFuncType     lPostHandlerFunc;
  Dcm_ReadOnlyMsgContextPtrType      pMsgContext = Dcm_DiagGetMsgContext(pContext->ThreadId);
  Dcm_NetTransportObjectConstPtrType pTranspObj = Dcm_DiagGetTranspObj(pContext->ThreadId);

  lPostHandlerFunc = Dcm_CfgWrapDiagGetPostHandlerFunc(pServiceInfo->PostHandlerFuncRef);

  lStdResult = lPostHandlerFunc(opStatus
                               ,Dcm_DiagGetSid(pContext->ThreadId)
                               ,Dcm_UtiGetResDataAt(pMsgContext,0)                                                                                   /* SBSW_DCM_POINTER_WRITE_MESSAGE_CONTEXT_REQBUFFER */
                               ,pMsgContext->resDataLen
                               ,Dcm_NetPbCfgGetProtocolIdOfTranspObj(pTranspObj)                                                                     /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */
                               ,pTranspObj->ClientSrcAddr
                               ,Dcm_NetPbCfgGetConnId(pTranspObj->ConnHdl)
                               ,(uint8)pMsgContext->msgAddInfo.reqType
                               ,ErrorCode);                                                                                                          /* SBSW_DCM_CALL_FUNCPTR_SVCPOSTHANDLER */

  switch (lStdResult)
  {
    case E_OK:
      /* Fall through */
    case DCM_E_PENDING:
      /* Fall through */
    case E_NOT_OK:
      Dcm_UtiHandleApplNrc(lStdResult, ErrorCode, DCM_E_CONDITIONSNOTCORRECT);                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;
    default:
      lStdResult = DCM_E_NOT_OK;
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_DebugReportError(DCM_SID_POSTHANDLERFUNC, DCM_E_INTERFACE_RETURN_VALUE);
      break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_RepeaterProcessSvcProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterProcessSvcProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_CfgDiagServiceInfoPtrType pServiceInfo,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

#if (DCM_DIAG_SERVICE_DISPATCHER_ENABLED == STD_ON)
  if(Dcm_DiagIsExternallyHandled(pContext->ThreadId))
  {
    lStdResult = pServiceInfo->AltProcFunc(pContext
                                          ,opStatus
                                          ,Dcm_DiagGetMsgContext(pContext->ThreadId)
                                          ,ErrorCode);                                                                                               /* SBSW_DCM_CALL_FUNCPTR_SERVICEINFO */
  }
  else
#endif
  {
    lStdResult = pServiceInfo->ProcFunc(pContext
                                        ,opStatus
                                        ,Dcm_DiagGetMsgContext(pContext->ThreadId)
                                        ,ErrorCode);                                                                                                 /* SBSW_DCM_CALL_FUNCPTR_SERVICEINFO */
  }

  switch (lStdResult)
  {
    case E_OK:
      /* Fall through */
    case DCM_E_PROCESSINGDONE:
      Dcm_RepeaterNextStep(pContext, DCM_REPEATER_PROGRESS_POST_HANDLER);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP; /* Speed up processing */
      break;
    default:
      /* DCM_E_PENDING: */
      /* DCM_E_LOOP: */
      /* DCM_E_STOP_REPEATER: */
      /* DCM_E_FORCE_RCRRP: */
      /* E_NOT_OK: */
      break;
  }

  if(opStatus == DCM_CANCEL)
  {
    Dcm_DiagSvcCancelFuncType lCancelFunc = Dcm_CfgWrapDiagGetCancelFunc(pServiceInfo->CancelFuncRef);                                               /* PRQA S 2962 */ /* MD_Dcm_2962 */

    lCancelFunc(pContext);                                                                                                                           /* SBSW_DCM_CALL_FUNCPTR_SVCCANCELLERS */
  }

  return lStdResult;
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_SVC_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc01Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc01RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc01Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc01;
}
#endif

#if (DCM_SVC_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc02Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc02RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc02Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc02;
}
#endif

#if (DCM_SVC_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc03Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc03RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc03Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc03;
}
#endif

#if (DCM_SVC_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc04Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc04RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc04Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc04;
}
#endif

#if (DCM_SVC_06_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc06Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc06RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc06Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc06;
}
#endif

#if (DCM_SVC_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc07Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc07RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc07Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc07;
}
#endif

#if (DCM_SVC_08_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc08Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc08RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc08Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc08;
}
#endif

#if (DCM_SVC_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc09Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc09RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc09Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc09;
}
#endif

#if (DCM_SVC_0A_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc0AContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc0ARepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc0AContext(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc0A;
}
#endif

/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc10Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc10RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc10Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc10;
}

#if (DCM_SVC_11_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc11Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc11RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc11Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc11;
}
#endif

#if (DCM_SVC_14_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc14Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc14RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc14Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc14;
}
#endif

#if (DCM_SVC_19_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc19Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc19RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc19Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc19;
}
#endif

#if (DCM_SVC_22_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc22Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc22RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc22Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc22;
}
#endif

#if (DCM_SVC_23_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc23Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc23RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc23Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc23;
}
#endif

#if (DCM_SVC_24_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc24Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc24RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc24Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc24;
}
#endif

#if (DCM_SVC_27_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc27Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc27RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc27Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc27;
}
#endif

#if (DCM_SVC_28_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc28Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc28RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc28Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc28;
}
#endif

#if (DCM_SVC_29_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc29Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc29RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc29Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc29;
}
#endif

#if (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc2AContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc2ARepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc2AContext(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc2A;
}
#endif

#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc2CContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc2CRepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc2CContext(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc2C;
}
#endif

#if (DCM_SVC_2E_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc2EContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc2ERepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc2EContext(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc2E;
}
#endif

#if (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc2FContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc2FRepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc2FContext(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc2F;
}
#endif

#if (DCM_SVC_31_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc31Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc31RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc31Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc31;
}
#endif

#if (DCM_SVC_34_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc34Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc34RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc34Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc34;
}
#endif

#if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc35Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc35RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc35Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc35;
}
#endif

#if (DCM_SVC_36_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc36Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc36RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc36Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc36;
}
#endif

#if (DCM_SVC_37_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc37Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc37RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc37Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc37;
}
#endif

#if (DCM_SVC_3D_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc3DContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc3DRepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc3DContext(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc3D;
}
#endif

#if (DCM_SVC_85_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc85Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc85RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc85Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc85;
}
#endif

#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterGetSvc86Context()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_Svc86RepeaterProxyContextPtrType, DCM_CODE) Dcm_RepeaterGetSvc86Context(
  Dcm_ThreadIdOptType threadId
  )
{
  return &Dcm_RepeaterGetThreadContext(threadId)->Context.Svc86;
}
#endif

/**********************************************************************************************************************
 *  Dcm_RepeaterGetProgress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_RepeaterProgressType, DCM_CODE) Dcm_RepeaterGetProgress(                                                                                    /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_RepeaterThreadContextConstPtrType pRepeaterContext = Dcm_RepeaterGetThreadContext(pContext->ThreadId);

  return pRepeaterContext->Progress;
}

/**********************************************************************************************************************
 *  Dcm_RepeaterGetSubProgress()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_RepeaterProgressType, DCM_CODE) Dcm_RepeaterGetSubProgress(                                                                                 /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_RepeaterThreadContextConstPtrType pRepeaterContext = Dcm_RepeaterGetThreadContext(pContext->ThreadId);

  return pRepeaterContext->SubProgress;
}

/**********************************************************************************************************************
 *  Dcm_RepeaterNextSubStep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RepeaterNextSubStep(                                                                                                        /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_RepeaterProgressType nextSubStep
  )
{
  Dcm_RepeaterThreadContextPtrType pRepeaterContext = Dcm_RepeaterGetThreadContext(pContext->ThreadId);

  pRepeaterContext->SubProgress = nextSubStep;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_RepeaterResetOpStatus(pRepeaterContext);                                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_RepeaterNextStep()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RepeaterNextStep(                                                                                                           /* PRQA S 3219 */ /* MD_MSR_Unreachable */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_RepeaterProgressType nextStep
  )
{
  Dcm_RepeaterThreadContextPtrType pRepeaterContext = Dcm_RepeaterGetThreadContext(pContext->ThreadId);

  pRepeaterContext->Progress = nextStep;                                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_RepeaterNextSubStep(pContext, DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL);                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_RepeaterSetCallee()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RepeaterSetCallee(
  Dcm_ContextPtrType pContext,
  uint8 serviceProcessorIdx
  )
{
  Dcm_RepeaterThreadContextPtrType pRepeaterContext = Dcm_RepeaterGetThreadContext(pContext->ThreadId);

  pRepeaterContext->CalleeIdx = serviceProcessorIdx;                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_RepeaterNextStep(pContext, DCM_REPEATER_PROGRESS_PRE_HANDLER);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
}

/**********************************************************************************************************************
 *  Dcm_RepeaterSetOpStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RepeaterSetOpStatus(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_OpStatusType opStatus
  )
{
  Dcm_RepeaterThreadContextPtrType pRepeaterContext = Dcm_RepeaterGetThreadContext(pContext->ThreadId);
  pRepeaterContext->OpStatus = opStatus;                                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
}

/**********************************************************************************************************************
 *  Dcm_RepeaterExecute()
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
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterExecute(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  Dcm_CfgDiagServiceInfoPtrType pServiceInfo;
  Dcm_RepeaterThreadContextPtrType pRepeaterContext = Dcm_RepeaterGetThreadContext(pContext->ThreadId);

  /* Make a backup in case the processor uses the Dcm_RepeaterSetCallee API */
  Dcm_OpStatusType opStatusBckp = pRepeaterContext->OpStatus;

  Dcm_UtiResetRuntimeLimiter();

  lStdResult = Dcm_CfgWrapDiagServiceInfoGetEntry(pRepeaterContext->CalleeIdx, &pServiceInfo, ErrorCode);                                            /* SBSW_DCM_COMB_PTR_FORWARD */

  DCM_UTI_LOOP_FOR_EVER                                                                                                                              /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
  {
    if ( (lStdResult == DCM_E_OK) || (lStdResult == DCM_E_LOOP) )                                                                                    /* COV_DCM_RTM_RUNTIME_CHECK TX tf tf */
    {
      switch (pRepeaterContext->Progress)
      {
      case DCM_REPEATER_PROGRESS_PRE_HANDLER:
        lStdResult = Dcm_RepeaterProcessSvcPreHandler(pContext, pRepeaterContext->OpStatus, pServiceInfo, ErrorCode);                                /* SBSW_DCM_COMB_PTR_FORWARD */ /* PRQA S 2962 */ /* MD_Dcm_2962 */
        break;
      case DCM_REPEATER_PROGRESS_POST_HANDLER:
        lStdResult = Dcm_RepeaterProcessSvcPostHandler(pContext, pRepeaterContext->OpStatus, pServiceInfo, ErrorCode);                               /* SBSW_DCM_COMB_PTR_FORWARD */
        break;
      default:
        lStdResult = Dcm_RepeaterProcessSvcProcessor(pContext, pRepeaterContext->OpStatus, pServiceInfo, ErrorCode);                                 /* SBSW_DCM_COMB_PTR_FORWARD */
        break;
      }
    }

    if(opStatusBckp == DCM_CANCEL)
    {
      *ErrorCode = DCM_DIAG_CANCEL_OP_NRC;/* always return the cancellation NRC! */                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;/* enforce finalization */
    }

    if(lStdResult != DCM_E_LOOP)
    {
      break;
    }/* else - just loop again */

    Dcm_RepeaterResetOpStatus(pRepeaterContext);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (Dcm_UtiRuntimeLimitReached() == TRUE)
    {
      lStdResult = DCM_E_PENDING_LIMITER;
      break;
    }
  }

  return lStdResult;
}

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RepeaterCheckAndGetAccess()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_RepeaterCheckAndGetAccess(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;                                                                                                              /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  Dcm_RepeaterThreadContextPtrType pRepeaterContext = Dcm_RepeaterGetThreadContext(pContext->ThreadId);

  if ((Dcm_DiagGetServiceInfo(pContext->ThreadId)->Props & DCM_DIAG_SVC_CFG_PROP_NEEDS_LOCK) != 0u)
  {
    if (!Dcm_RsrcMgrHasServiceLock(pContext->ThreadId))
    {
      lStdResult = Dcm_RsrcMgrGetServiceLock(pRepeaterContext->OpStatus, Dcm_DiagGetSid(pContext->ThreadId), pContext->ThreadId);

      switch (lStdResult)
      {
      case DCM_E_OK:
        /* set opStatus as initial, because now the Dcm_RepeaterExecute will be called the first time */
        pRepeaterContext->OpStatus = DCM_INITIAL;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
        break;
      case DCM_E_NOT_OK:
        *ErrorCode = DCM_DIAG_CANCEL_OP_NRC; /* always return the cancellation NRC! */                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
        break;
      default:
        /* DCM_E_PENDING do nothing */
        break;
      }
    }
  }
  return lStdResult;
}
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CALLOUT_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RepeaterDeadEnd()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_RepeaterDeadEnd(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);
  DCM_IGNORE_UNREF_PARAM(opStatus);
  DCM_IGNORE_UNREF_PARAM(pMsgContext);
  DCM_IGNORE_UNREF_PARAM(ErrorCode);

  return DCM_E_STOP_REPEATER;
}
#define DCM_STOP_SEC_CALLOUT_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Repeater.c
 *********************************************************************************************************************/
