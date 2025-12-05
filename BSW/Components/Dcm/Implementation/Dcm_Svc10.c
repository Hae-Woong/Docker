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
/**        \file  Dcm_Svc10.c
 *         \unit  Svc10
 *        \brief  Contains the implementation of Service 0x10 unit.
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
#define DCM_SVC10_SOURCE

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
#include "Dcm_Svc10.h"
#include "Dcm_Repeater.h"
#include "Dcm_Net.h"
#include "Dcm_NetPbCfg.h"
#include "Dcm_CfgWrap.h"
#include "Dcm_Debug.h"
#include "Dcm_Uti.h"
#include "Dcm_Mode.h"
#include "Dcm_Diag.h"
#include "Dcm_State.h"
#include "Dcm.h"
#if (DCM_SVC_10_RST2BOOT_HIS_ENABLED == STD_ON)
# include "Dcm_Tmr.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON) &&\
    (DCM_SVC_10_RST2BOOT_HIS_ENABLED == STD_OFF)
# define DCM_SVC_10_RESET_AFTER_RESPONSE                             STD_ON
#else
# define DCM_SVC_10_RESET_AFTER_RESPONSE                             STD_OFF
#endif

/*! Diagnostic service 0x10 boot loader interaction types */
#define DCM_SVC10_BOOT_TYPE_NONE                                     (Dcm_Svc10SessionBootType)0 /*!< No bootloader interaction (just session state change) */
#define DCM_SVC10_BOOT_TYPE_OEM                                      (Dcm_Svc10SessionBootType)1 /*!< Performs jump to the OEM bootloader */
#define DCM_SVC10_BOOT_TYPE_SYS                                      (Dcm_Svc10SessionBootType)2 /*!< Performs jump to the system supplier bootloader */

/*! Service 0x10 processing steps */
#define DCM_SVC10_PROGRESS_CHECK_ACCESS                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
#define DCM_SVC10_PROGRESS_TRIGGER_RESET                             ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))
#define DCM_SVC10_PROGRESS_WAIT_RESET_ACK                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 2u))
#define DCM_SVC10_PROGRESS_WAIT_RCRRP_ACK                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 3u))
#define DCM_SVC10_PROGRESS_SET_PRGCOND                               ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 4u))

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_TriggerReset()
 *********************************************************************************************************************/
/*! \brief          Triggers an Ecu Reset.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_FORCE_RCRRP       Enforce a RCR-RP transmission
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_NOT_OK            Operation failed, send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_TriggerReset(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_WaitForResetAck()
 *********************************************************************************************************************/
/*! \brief          Wait for Ecu Reset acknowledgment.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_FORCE_RCRRP       Enforce a RCR-RP transmission
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_NOT_OK            Operation failed, send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_WaitForResetAck(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_WaitForRcrRpAck()
 *********************************************************************************************************************/
/*! \brief          Wait for RCR-RP acknowledgment.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \return         DCM_E_NOT_OK            Operation failed, send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_WaitForRcrRpAck(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_SetProgConditions()
 *********************************************************************************************************************/
/*! \brief          Sets programming conditions.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in]      pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_STOP_REPEATER     Trigger to stop repeater (programming conditions set)
 *  \return         DCM_E_NOT_OK            Operation failed, send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_SetProgConditions(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );
#endif

/**********************************************************************************************************************
 *  Dcm_Svc10Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x10 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send final positive response
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \return         DCM_E_NOT_OK            Operation failed
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc10CheckAccess()
 *********************************************************************************************************************/
/*! \brief          Checks whether transistion to requested session can be performed.
 *  \details        If e.g. any OBD request is currently in progress the session change request has to be delayed.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send final positive response
 *  \return         DCM_E_LOOP              Execute next repeater stage immediately
 *  \return         DCM_E_NOT_OK            Operation failed
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10CheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc10UndoPendingSession()
 *********************************************************************************************************************/
/*! \brief          Undo the pending session expectation by synchronizing it with the active session.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc10UndoPendingSession(
  void
  );
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_TriggerReset()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_TriggerReset(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_NetTransportObjectConstPtrType pTranspObj = Dcm_DiagGetTranspObj(pContext->ThreadId);

  DCM_IGNORE_UNREF_PARAM(opStatus);
  DCM_IGNORE_UNREF_PARAM(pMsgContext);

  /* Prepare programming conditions */
  pRepContext->ProgConditions.ProtocolId = Dcm_NetPbCfgGetProtocolIdOfTranspObj(pTranspObj);                                                         /* SBSW_DCM_POINTER_WRITE_TRANSPORTOBJECT */ /* SBSW_DCM_PARAM_PTR_WRITE */
# if (DCM_NET_CONNECTION_ID_ENABLED == STD_ON)
  pRepContext->ProgConditions.ConnectionId = Dcm_NetPbCfgGetConnId(pTranspObj->ConnHdl);                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
# endif
  pRepContext->ProgConditions.TesterSourceAddr = pTranspObj->ClientSrcAddr;                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ProgConditions.Sid = 0x10u;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ProgConditions.SubFuncId = Dcm_CfgWrapSvc10SubFuncLookUpTable(pRepContext->SesStateIdx+1u);                                           /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->ProgConditions.ReprogrammingRequest = TRUE;                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
  /* This flag shall be initialized from the FBL, not the DCM!
  pRepContext->ProgConditions.ApplUpdated = TRUE; */
  /* To be set once the Dcm_setProgCond API is called in order to consider the current RCR-RP status!
    * pRepContext->ProgConditions.ResponseRequired = ...; */
  pRepContext->ResetMode = (Rte_ModeType_DcmEcuReset)( (Dcm_CfgWrapSvc10SubFuncInfo(pRepContext->SesStateIdx)->BootType == DCM_SVC10_BOOT_TYPE_OEM)
                                                      ? RTE_MODE_DcmEcuReset_JUMPTOBOOTLOADER
                                                      : RTE_MODE_DcmEcuReset_JUMPTOSYSSUPPLIERBOOTLOADER);                                           /* SBSW_DCM_PARAM_PTR_WRITE */

  if(Dcm_ModeSwitchEcuReset(pRepContext->ResetMode) == DCM_E_OK)
  {
    /* Determine entry point - with or without RCR-RP */
    Dcm_RepeaterNextStep(pContext, DCM_SVC10_PROGRESS_WAIT_RESET_ACK);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP; /* start immediately */
  }
  else
  {
    /* for some reason, no mode switch is possible - exit with NRC */
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_WaitForResetAck()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_WaitForResetAck(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pContext);
  DCM_IGNORE_UNREF_PARAM(opStatus);
  DCM_IGNORE_UNREF_PARAM(pMsgContext);
  DCM_IGNORE_UNREF_PARAM(pRepContext);

  /* Check mode switch state */
  lStdResult = Dcm_ModeSwitchAckEcuReset();
  switch(lStdResult)
  {
  case DCM_E_PENDING:                                                                                                                                /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    break;
  case DCM_E_OK:
    Dcm_RepeaterNextStep(pContext, DCM_SVC10_PROGRESS_WAIT_RCRRP_ACK);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    if(Dcm_NetPbCfgGetHasRespPendOnBoot(Dcm_DiagGetTranspObj(pContext->ThreadId)) == TRUE)                                                           /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* COV_DCM_RTM_DEPENDS_ON_UNSUPPORTED TX */ /* SBSW_DCM_PARAM_PTR_FORWARD */
    {
      lStdResult = DCM_E_FORCE_RCRRP;
    }
    else
    {
      lStdResult = DCM_E_LOOP;
    }
    break;/* go on with next step */
  default:
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }

  return lStdResult;
}
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_WaitForRcrRpAck()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_WaitForRcrRpAck(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);

  if(opStatus == DCM_FORCE_RCRRP_NOT_OK)
  {
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
# if (DCM_SVC_10_RST2BOOT_HIS_ENABLED == STD_ON)
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    Dcm_SplitTaskEnterCS();/* avoid interrupt from the Dcm_DiagTaskTx */
    if(pMsgContext->msgAddInfo.suppressPosResponse != 0u)
    {
      pRepContext->ProgConditions.ResponseRequired = FALSE;                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Stop to avoid inconsistency between P2 timer and the SetProgCond asynchronous API's "ResponseRequired" value */
      Dcm_TmrStopTimerByThread(DCM_TMR_ID_P2, pContext->ThreadId);
    }
    else
    {
      pRepContext->ProgConditions.ResponseRequired = TRUE;                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    Dcm_SplitTaskLeaveCS();/* avoid interrupt from the Dcm_DiagTaskTx */
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
# else
    pRepContext->ProgConditions.ResponseRequired = FALSE;/* no response required since DCM sends the final response */                               /* SBSW_DCM_PARAM_PTR_WRITE */
# endif

    Dcm_RepeaterNextStep(pContext, DCM_SVC10_PROGRESS_SET_PRGCOND);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }
  return lStdResult;
}
#endif

#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc10_SetProgConditions()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10_SetProgConditions(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);
  DCM_IGNORE_UNREF_PARAM(opStatus);

  lStdResult = Dcm_SetProgConditions(
# if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_450)
                                     opStatus,
# endif
                                     &pRepContext->ProgConditions);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

  switch(lStdResult)
  {
  case DCM_E_OK:
# if (DCM_SVC_10_RST2BOOT_HIS_ENABLED == STD_ON)
    lStdResult = Dcm_ModeSwitchEcuReset(RTE_MODE_DcmEcuReset_EXECUTE);
    if(lStdResult == DCM_E_OK)/* fire and forget */
    {
      /* just wait for reset */
      lStdResult = DCM_E_STOP_REPEATER;
    }
    else
    {
      /* This code shall not be reached, since the programming conditions are already set! */
      *ErrorCode = DCM_E_PANIC_NRC; /* Use panic NRC since the first mode switch has succeeded, and this one shall too */                            /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2880 */ /* MD_MSR_Unreachable */
      lStdResult = DCM_E_NOT_OK;
    }
# endif
    break; /* send final response and if applicable - do reset */
  case DCM_E_PENDING:
    /* already set */
    break;
  case DCM_E_NOT_OK:
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default:
    Dcm_DebugReportError(DCM_SID_SETPROGCONDITIONS, DCM_E_INTERFACE_RETURN_VALUE);
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }
  return lStdResult;
}
#endif

/**********************************************************************************************************************
 *  Dcm_Svc10Handler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10Handler(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext
  )
{
  Dcm_DiagSubServiceRefOptType  subSvcRef;
  Std_ReturnType                lStdResult;

  DCM_IGNORE_UNREF_PARAM(pContext);
  DCM_IGNORE_UNREF_PARAM(opStatus);

  lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                    ,Dcm_CfgSvc10SubFuncLookUpTable
                                                    ,Dcm_CfgSvc10SubFuncExecPrecondTable
                                                    ,Dcm_DiagSvcWithOnlySubFuncReqLengthGetter
                                                    ,Dcm_DiagNoSequenceChecker
                                                    ,&subSvcRef
                                                    ,ErrorCode);                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
  if(lStdResult == DCM_E_OK)
  {
    /* Compose positive response */
    Dcm_UtiProvideResDataAsU16(pMsgContext, Dcm_CfgWrapSvc10SubFuncInfo(subSvcRef)->ResponseTime.P2);                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU16(pMsgContext, Dcm_CfgWrapSvc10SubFuncInfo(subSvcRef)->ResponseTime.P2Star);                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

    pRepContext->SesStateIdx = (uint8)subSvcRef;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */

    Dcm_RepeaterNextStep(pContext, DCM_SVC10_PROGRESS_CHECK_ACCESS);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP; /* start immediately */
  } /* else DCM_E_NOT_OK (ErrorCode is already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc10CheckAccess()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc10CheckAccess(                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc10RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_NetTransportObjectConstPtrType pTranspObj = Dcm_DiagGetTranspObj(pContext->ThreadId);

  DCM_IGNORE_UNREF_PARAM(pContext);
  DCM_IGNORE_UNREF_PARAM(pMsgContext);
  DCM_IGNORE_UNREF_PARAM(ErrorCode);

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
  if( ( ((DCM_STATE_VALUE_SESSION_INIT & Dcm_StateGetPendingSession()) == 0u) /* a non-default session is/will be active */
      &&( (Dcm_NetGetSessionConnection() != pTranspObj->ConnHdl)                                                                                     /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
        ||(Dcm_NetGetSessionClientSrcAddr() != pTranspObj->ClientSrcAddr) ) /* not the owner of the (non-default) session */                         /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
      &&(Dcm_NetPbCfgGetProtocolPriorityByConnHdl(pTranspObj->ConnHdl)                                                                               /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
        >= Dcm_NetPbCfgGetProtocolPriorityByConnHdl(Dcm_NetGetSessionConnection())) /* priority not enough */                                        /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
      )
    )
  {
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
#endif
  {
    if(opStatus == DCM_INITIAL)
    {
      Dcm_UtiEnterCriticalSection();
      /*=================================*
        BEGIN CRITICAL SECTION
       *=================================*/
      Dcm_StateSetPendingSession(Dcm_UtiGetBitFromIndex(Dcm_CfgStateGroupMemType, pRepContext->SesStateIdx));

      /*
       * Set always a potential tester owner: In case of failed positive response confirmation or negative response,
       * the session will remain in its old state (default) -> the set value does not matter!
       * The situation that the session was a non-default one and another client has requested SID 0x10 is not possible:
       * - Each protocol switch invokes session transition to the default one!
       * - As long as a non-default session is active, no other client with the same priority can access the ECU!
       */
      Dcm_NetSetSessionConnection(pTranspObj->ConnHdl);
      Dcm_NetSetSessionClientSrcAddr(pTranspObj->ClientSrcAddr);
      /*=================================*
        END CRITICAL SECTION
       *=================================*/
      Dcm_UtiLeaveCriticalSection();
    }

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
    lStdResult = Dcm_DiagIsSessionChangeAllowed(pContext->ThreadId);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

    if(lStdResult == DCM_E_NOT_OK)
    {
      lStdResult = DCM_E_PENDING;
    }
    else
#endif
    {
#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
      pRepContext->ResetMode = RTE_MODE_DcmEcuReset_NONE;                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
      if(Dcm_CfgWrapSvc10SubFuncInfo(pRepContext->SesStateIdx)->BootType != DCM_SVC10_BOOT_TYPE_NONE)
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC10_PROGRESS_TRIGGER_RESET);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdResult = DCM_E_LOOP;
      }
      else
#endif
      {
        /* Just send the final response */
        lStdResult = DCM_E_OK;
      }
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc10UndoPendingSession()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc10UndoPendingSession(
  void
  )
{
    /* Undo pending session expectation */
#if (DCM_NET_PROTECT_SESSION_OF_CLIENT_ENABLED == STD_ON)
    Dcm_UtiEnterCriticalSection();/* avoid interrupt from the Dcm_NetTaskRx || Dcm_NetStartOfReception() */
#else
    Dcm_SplitTaskEnterCS();/* avoid interrupt from the Dcm_NetTaskRx */
#endif
    /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
    Dcm_StateSyncPendingSession();/* synchronize with the currently active session */
    /*=================================*
      END CRITICAL SECTION
      *=================================*/
#if (DCM_NET_PROTECT_SESSION_OF_CLIENT_ENABLED == STD_ON)
    Dcm_UtiLeaveCriticalSection();
#else
    Dcm_SplitTaskLeaveCS();
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CALLOUT_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service10Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service10Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc10RepeaterProxyContextPtrType pRepContext = Dcm_RepeaterGetSvc10Context(pContext->ThreadId);

  switch(Dcm_RepeaterGetProgress(pContext))                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
  case DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL:
    lStdResult = Dcm_Svc10Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC10_PROGRESS_CHECK_ACCESS:
    lStdResult = Dcm_Svc10CheckAccess(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
#if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
  case DCM_SVC10_PROGRESS_TRIGGER_RESET:
    lStdResult = Dcm_Svc10_TriggerReset(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                    /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC10_PROGRESS_WAIT_RESET_ACK:
    lStdResult = Dcm_Svc10_WaitForResetAck(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC10_PROGRESS_WAIT_RCRRP_ACK:
    lStdResult = Dcm_Svc10_WaitForRcrRpAck(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC10_PROGRESS_SET_PRGCOND:
    lStdResult = Dcm_Svc10_SetProgConditions(opStatus, pMsgContext, ErrorCode, pRepContext);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
#endif
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Service10FastPostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service10FastPostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  if(status == DCM_RES_POS_OK)
  {
    Dcm_Svc10RepeaterProxyContextConstPtrType pRepContext = Dcm_RepeaterGetSvc10Context(pContext->ThreadId);

    Dcm_DiagSetP2Timings(pRepContext->SesStateIdx
                        ,Dcm_NetGetProtIdOfActiveProtocol()
                        ,pContext->ThreadId);

#if (DCM_SVC_10_RESET_AFTER_RESPONSE == STD_ON)
    if(pRepContext->ResetMode != RTE_MODE_DcmEcuReset_NONE)
    {
      Dcm_NetSetRxAllowed(FALSE); /* lock any further communication while the ECU is getting reset */
    }
#endif
  }
}

/**********************************************************************************************************************
 *  Dcm_Service10PostProcessor()
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
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service10PostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  if(status == DCM_RES_POS_OK)
  {
    Dcm_Svc10RepeaterProxyContextConstPtrType pRepContext = Dcm_RepeaterGetSvc10Context(pContext->ThreadId);

    /* execute transition */
    Dcm_StateSetSession(pRepContext->SesStateIdx);
#if (DCM_SVC_10_RESET_AFTER_RESPONSE == STD_ON)
    if(pRepContext->ResetMode != RTE_MODE_DcmEcuReset_NONE)
    {
      /* fire and forget - if failed - no chance to send any NRC from here */
      (void)Dcm_ModeSwitchEcuReset(RTE_MODE_DcmEcuReset_EXECUTE);
    }
#endif
  }
  else
  {
#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
    Dcm_NetTransportObjectConstPtrType pTranspObj = Dcm_DiagGetTranspObj(pContext->ThreadId);
#endif

    /* on the way of changing a session */
    if ( (Dcm_StateGetSession() != Dcm_StateGetPendingSession())
#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
       &&(Dcm_NetGetSessionConnection() == pTranspObj->ConnHdl) /* and the owner of the pending session ... */                                       /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#endif
       )
    {
      Dcm_Svc10UndoPendingSession();
    }/* else - nothing to undo */
  }
}
#define DCM_STOP_SEC_CALLOUT_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc10.c
 *********************************************************************************************************************/
