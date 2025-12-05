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
/**        \file  Dcm_Svc86.c
 *         \unit  Svc86
 *        \brief  Contains the implementation of Service 0x86 unit.
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
#define DCM_SVC86_SOURCE

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
#include "Dcm_Svc86.h"
#if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_MSR_UT_OPTIONAL_UNIT */
# include "Rte_Dcm.h"
# include "Dcm_Tsk.h"
# include "Dcm_CfgWrap.h"
# include "Dcm_Debug.h"
# include "Dcm_Net.h"
# include "Dcm_NetPbCfg.h"
# include "Dcm_Repeater.h"
# include "Dcm_Svc86Schd.h"
# include "Dcm_State.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_Uti.h"
# if (DCM_VARMGR_MULTI_SVC_EXCLUSIVE_ENABLED == STD_ON)
#  include "Dcm_VarMgr.h"
# endif
# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
#  include "NvM.h"
# endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Repeater step for start processing in subfunction handler */
# define DCM_SVC86_PROGRESS_SUBFUNCTION                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))

/*! Service 0x86 0x03 sub-repeater IDs */
# define DCM_SVC86_03_SUBPROGRESS_INITIAL                            ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL + 0u)
# define DCM_SVC86_03_SUBPROGRESS_DIDLOOKUP                          ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL + 1u)
# define DCM_SVC86_03_SUBPROGRESS_DIDCHECKCONDITIONS                 ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL + 2u)
# define DCM_SVC86_03_SUBPROGRESS_DIDGETLENGTH                       ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL + 3u)

/*! Service 0x86 0x05 sub-repeater IDs */
# define DCM_SVC86_05_SUBPROGRESS_INITIAL                            ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL + 0u)
# define DCM_SVC86_05_SUBPROGRESS_UPDATE_NVM                         ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL + 1u)

/*! Service 0x86 0x07 sub-repeater IDs */
# define DCM_SVC86_07_SUBPROGRESS_INITIAL                            ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL + 0u)
# define DCM_SVC86_07_SUBPROGRESS_DIDLOOKUP                          ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL + 1u)
# define DCM_SVC86_07_SUBPROGRESS_DIDCHECKCONDITIONS                 ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL + 2u)
# define DCM_SVC86_07_SUBPROGRESS_DIDGETLENGTH                       ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL + 3u)

/*! Event window time values */
# define DCM_SVC86_EWT_INFINITE                                      0x02u

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc86Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc86SubFuncHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 SubFunction Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_09CheckEventTypeRecord()
 *********************************************************************************************************************/
/*! \brief          Check the validity of the event type parameter when processing Svc86 0x09 request.
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_Svc86_09CheckEventTypeRecord(
  uint8 svc19SubFunc,
  Dcm_MsgLenType lengthReqData
  );
# endif

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86RemovePersistedStart()
 *********************************************************************************************************************/
/*! \brief          Removes the persisted start for response on event.
 *  \details        Setting up a new event, clearing or stopping the response on event service, shall remove the 
 *                  persisted start for response of event after reset or power on of the server.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86RemovePersistedStart(
  void
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc86ReqLengthGetter()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 sub-function length getter.
 *  \details        Always returns the sub-function specific length.
 *  \param[in]      sfRef                The sub-function zero-based index.
 *  \return         Expected sub-function specific length.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc86ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  );

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_03Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 0x03 Handler.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       The operations status
 *  \param[in,out]  pMsgContext    Current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_NOT_OK    Send negative response
 *  \return         DCM_E_LOOP      Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_03Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_03DidLookUp()
 *********************************************************************************************************************/
/*! \brief          Look up for the requested DID.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       The operations status
 *  \param[in,out]  pMsgContext    Current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_NOT_OK     Send negative response
 *  \return         DCM_E_PENDING    Retry later
 *  \return         DCM_E_LOOP       Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_03DidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_03DidCheckCondition()
 *********************************************************************************************************************/
/*! \brief          Checks the preconditions of the requested DID.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       The operations status
 *  \param[in,out]  pMsgContext    Current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_NOT_OK     Send negative response
 *  \return         DCM_E_PENDING    Retry later
 *  \return         DCM_E_LOOP       Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_03DidCheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_03DidGetLength()
 *********************************************************************************************************************/
/*! \brief          Reads the length of a requested DID.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       The operations status
 *  \param[in,out]  pMsgContext    Current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_NOT_OK     Send negative response
 *  \return         DCM_E_PENDING    Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_03DidGetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_Svc86_05Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 0x05 Handler.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       The operations status
 *  \param[in,out]  pMsgContext    Current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_NOT_OK    Send negative response
 *  \return         DCM_E_LOOP      Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_05Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc86_05UpdateNvM()
 *********************************************************************************************************************/
/*! \brief          Stores the data into non-volatile memory (if requested) and finalizes service processing.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       The operations status
 *  \param[in,out]  pMsgContext    Current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_OK         Send positive response
 *  \return         DCM_E_PENDING    Retry later
 *  \return         DCM_E_NOT_OK     Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_05UpdateNvM(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_07Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 0x07 Handler.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       The operations status
 *  \param[in,out]  pMsgContext    Current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_NOT_OK    Send negative response
 *  \return         DCM_E_LOOP      Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_07Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_07DidLookUp()
 *********************************************************************************************************************/
/*! \brief          Look up for the requested DID.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       The operations status
 *  \param[in,out]  pMsgContext    Current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_NOT_OK     Send negative response
 *  \return         DCM_E_PENDING    Retry later
 *  \return         DCM_E_LOOP       Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_07DidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_07DidCheckCondition()
 *********************************************************************************************************************/
/*! \brief          Checks the preconditions of the requested DID.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       The operations status
 *  \param[in,out]  pMsgContext    Current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_NOT_OK     Send negative response
 *  \return         DCM_E_PENDING    Retry later
 *  \return         DCM_E_LOOP       Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_07DidCheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_07DidGetLength()
 *********************************************************************************************************************/
/*! \brief          Reads the length of a requested DID.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       The operations status
 *  \param[in,out]  pMsgContext    Current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[in,out]  pRepContext    Current repeater proxy context
 *  \return         DCM_E_NOT_OK     Send negative response
 *  \return         DCM_E_PENDING    Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_07DidGetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc86Handler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                lStdResult;
  Dcm_DiagSubServiceRefOptType  lSubSvcRef;
  uint8                         lReqSf;

  DCM_IGNORE_UNREF_PARAM(opStatus);

  lReqSf = Dcm_UtiGetReqDataAsU8(pMsgContext);                                                                                                       /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

  lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                   ,Dcm_CfgSvc86SubFuncLookUpTable
                                                   ,Dcm_CfgSvc86SubFuncExecPrecondTable
                                                   ,Dcm_Svc86ReqLengthGetter
                                                   ,Dcm_DiagNoSequenceChecker
                                                   ,&lSubSvcRef
                                                   ,ErrorCode);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
  {
    pRepContext->SubSvcRef  = (Dcm_DiagSubServiceRefMemType)lSubSvcRef;                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->StoreState = (Dcm_UtiBitOpTest(uint8, lReqSf, 0x40) ? TRUE : FALSE);                                                                /* SBSW_DCM_PARAM_PTR_WRITE */

    Dcm_RepeaterNextStep(pContext, DCM_SVC86_PROGRESS_SUBFUNCTION); /* delegate job */                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;/* speed up processing */
  } /* else DCM_E_NOT_OK (ErrorCode is already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc86SubFuncHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                 lStdResult;
  Dcm_CfgSvc86SubFuncInfoPtrType pSubFuncInfo;

  lStdResult = Dcm_CfgWrapSvc86SubFuncInfoGetEntry(pRepContext->SubSvcRef, &pSubFuncInfo, ErrorCode);                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)                                                                                                                        /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = pSubFuncInfo->SubSvcFunc(pContext, opStatus, pMsgContext, ErrorCode);                                                               /* SBSW_DCM_CALL_FUNCPTR_SVC86SUBFUNC */
  }

  return lStdResult;
}

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_09CheckEventTypeRecord()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_Svc86_09CheckEventTypeRecord(
  uint8 svc19SubFunc,
  Dcm_MsgLenType lengthReqData
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean lEvtTypValid = FALSE;
  boolean lSubFuncValid = TRUE;
  uint8   lExpectedReqLength = 0u;

  /* ----- Implementation ----------------------------------------------- */
  switch (svc19SubFunc)
  {
    case 0x09u:
      lExpectedReqLength = 0u;
      break;
    case 0x04u:
    case 0x06u:
    case 0x10u:
      lExpectedReqLength = 1u;
      break;
    case 0x18u:
    case 0x19u:
      lExpectedReqLength = 2u;
      break;
    default:
      lSubFuncValid = FALSE;
      break;
  }

  if ((lSubFuncValid == TRUE) && (lExpectedReqLength == lengthReqData))
  {
    lEvtTypValid = TRUE;
  }

  return lEvtTypValid;
}
# endif

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86RemovePersistedStart()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc86RemovePersistedStart(
  void
  )
{
  if (Dcm_Svc86SchdGetRoEState() == DCM_SVC86_ROE_STATE_STARTED_PERSISTENTLY)
  {
    Dcm_Svc86SchdSetRoEState(DCM_SVC86_ROE_STATE_STARTED);
    Dcm_UtiNvMSetRamBlockStatus((uint16)DCM_SVC_86_NVRAM_BLOCKID);
  }
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc86ReqLengthGetter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc86ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  )
{
  /* Return sub-function specific length */
  return (Dcm_CfgWrapSvc86SubFuncInfo(sfRef))->ReqLength;
}

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_03Handler()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_03Handler(                                                                                        /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult;
  uint8          lEventWindowTime;
  uint8          lNumDidRecords = Dcm_Svc86SchdGetOnDidChangeNumRecords();
  uint16         lDidOfEventTypeRecord;

  DCM_IGNORE_UNREF_PARAM(opStatus);

  /* ----- Implementation ----------------------------------------------- */
  if (pMsgContext->reqDataLen >= 6u)
  {
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lEventWindowTime);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &lDidOfEventTypeRecord);                                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */

    if ( (pRepContext->StoreState == TRUE)
       ||(lNumDidRecords == DCM_SVC_86_MAX_NUM_CHANGE_OF_DID_EVENTS)
       ||(Dcm_UtiGetReqDataAsU8(pMsgContext) != 0x22u) /* Only SID 0x22 is allowed as STRT */                                                        /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
       ||(pMsgContext->reqDataLen > DCM_SVC_86_03_MAX_SUPPORTED_STRT_LENGTH) )
    {
      lStdResult = DCM_E_NOT_OK;
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      Dcm_Svc86OnDidChangeDidRecordPtrType pDidRecord;

      pDidRecord = Dcm_Svc86Schd_03GetDidRecord(lNumDidRecords);

      pRepContext->DidInfoContext.Did = lDidOfEventTypeRecord; /* DID to observe */                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

      pDidRecord->STRTLength = (uint8)pMsgContext->reqDataLen;                                                                                       /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */

      Dcm_UtiConsumeReqDataAsUN(pMsgContext
                               ,pDidRecord->STRT
                               ,DCM_SVC_86_03_MAX_SUPPORTED_STRT_LENGTH
                               ,pMsgContext->reqDataLen);                                                                                            /* SBSW_DCM_COMB_PTR_FORWARD */

      /* Prepare response data */
      Dcm_UtiProvideResDataAsU8(pMsgContext, 0x00u); /* Number of activated elements is always zero */                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU16(pMsgContext, lDidOfEventTypeRecord);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsUN(pMsgContext
                               ,pDidRecord->STRT
                               ,pDidRecord->STRTLength);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

      Dcm_RepeaterNextSubStep(pContext, DCM_SVC86_03_SUBPROGRESS_DIDLOOKUP);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP;
    }
  }
  else
  {
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_03DidLookUp()
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
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_03DidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult;

  /* ----- Implementation ----------------------------------------------- */
  lStdResult = Dcm_DidMgrGetDidInfo(pMsgContext->threadId
                                   ,opStatus
                                   ,&pRepContext->DidInfoContext
                                   ,&pRepContext->DidOpTypeContext
                                   ,DCM_DIDMGR_OP_READ);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

  switch(lStdResult)
  {
  case DCM_E_OK:
    if(Dcm_DidMgrIsOpSupported(Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DidInfoContext)), DCM_DIDMGR_OP_DEFINE))
    {
      /* No DynDID may be referenced! */
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    else
    {
      lStdResult = Dcm_StateCheckDID(Dcm_NetPbCfgGetConnHdlOfRxPduId(pMsgContext->rxPduId)                                                           /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
                                    ,pRepContext->DidInfoContext.Did
                                    ,DCM_DIDMGR_OP_READ
                                    ,Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext))->ExecCondRef
                                    ,ErrorCode);                                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */

      if(lStdResult == DCM_E_OK)
      {
        Dcm_RepeaterNextSubStep(pContext, DCM_SVC86_03_SUBPROGRESS_DIDCHECKCONDITIONS);                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdResult = DCM_E_LOOP;
      } /* else DCM_E_NOT_OK (ErrorCode already set) */
    }
    break;
  case DCM_E_PENDING:
    /* lStdResult is already set */
    break;
  default: /* DCM_E_NOT_OK */
    /* source DID is not supported */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_03DidCheckCondition()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_03DidCheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_LOOP;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);

  /* ----- Implementation ----------------------------------------------- */
  if(Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext))
                                ,DCM_DIDMGR_OPTYPE_READCHKCOND))
  {
    /* Check condition on this DID */
    Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_READCHKCOND);                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_DidMgrReadCheckCond(opStatus
                                        ,&(pRepContext->DidInfoContext)
                                        ,&(pRepContext->DidOpTypeContext)
                                        ,ErrorCode);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

    switch(lStdResult)
    {
    case DCM_E_OK:
      lStdResult = DCM_E_LOOP;
      break; /* go on with next DID */
    case DCM_E_PENDING:
      break;
    default: /* DCM_E_NOT_OK */
      break; /* finish service processing immediately */
    }
  }/* else - not supported operation - go on with getLength */

  if(lStdResult == DCM_E_LOOP)
  {
    Dcm_RepeaterNextSubStep(pContext, DCM_SVC86_03_SUBPROGRESS_DIDGETLENGTH);                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  return lStdResult; /* speed up processing */
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_03DidGetLength()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_03DidGetLength(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;
  uint8          lNumDidRecords = Dcm_Svc86SchdGetOnDidChangeNumRecords();
  DCM_IGNORE_UNREF_PARAM(pContext);
  DCM_IGNORE_UNREF_PARAM(pMsgContext);

  /* ----- Implementation ----------------------------------------------- */
  if(Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext)), DCM_DIDMGR_OPTYPE_READLENGTH))
  {
    Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_READLENGTH);                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_DidMgrReadLength(opStatus
                                     ,&(pRepContext->DidInfoContext)
                                     ,&(pRepContext->DidOpTypeContext)
                                     ,ErrorCode);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if (lStdResult == DCM_E_OK)
  {
    if (pRepContext->DidInfoContext.ReservedLength > DCM_SVC_86_MAX_SUPPORTED_DID_LENGTH)
    {
      lStdResult = DCM_E_NOT_OK;
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      Dcm_Svc86OnDidChangeDidRecordPtrType pDidRecord;

      /* Add scheduler job - set the operation now since not changed later */
      Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_READ);                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

      pDidRecord = Dcm_Svc86Schd_03GetDidRecord(lNumDidRecords);

      pDidRecord->DidInfoContext = pRepContext->DidInfoContext;                                                                                      /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
      pDidRecord->RefState       = DCM_SVC86_DID_REFERENCE_STATE_INVALID;                                                                            /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */

      Dcm_Svc86SchdIncOnDidChangeNumRecords();

#  if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
      Dcm_Svc86RemovePersistedStart();
#  endif
    }
  }

  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc86_05Handler()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_05Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  lStdResult;
  uint8           lEventWindowTime;

  DCM_IGNORE_UNREF_PARAM(opStatus);

  /* ----- Implementation ----------------------------------------------- */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lEventWindowTime);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

  if ( (lEventWindowTime != DCM_SVC86_EWT_INFINITE)
     ||( (pRepContext->StoreState == TRUE)
# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
       &&(Dcm_StateIsDefaultSessionActive() == FALSE)                                                                                                /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
# endif
       ) )
  {
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    Dcm_NetTransportObjectConstPtrType pTranspObj;

    pTranspObj = Dcm_DiagGetTranspObj(pContext->ThreadId);

    Dcm_Svc86SchdGetSessionContext(&(pRepContext->SessionContext));                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Store the source address, the latest client will receive the STRTs */
    pRepContext->SessionContext.RxPduId       = pMsgContext->rxPduId;                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    pRepContext->SessionContext.ClientSrcAddr = pTranspObj->ClientSrcAddr;                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
    if (pRepContext->StoreState == TRUE)
    {
      pRepContext->SessionContext.RoEState = DCM_SVC86_ROE_STATE_STARTED_PERSISTENTLY;                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
# endif
    {
      pRepContext->SessionContext.RoEState = DCM_SVC86_ROE_STATE_STARTED;                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    Dcm_UtiProvideResDataAsU8(pMsgContext, 0x00u);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */

    Dcm_RepeaterNextSubStep(pContext, DCM_SVC86_05_SUBPROGRESS_UPDATE_NVM);                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc86_05UpdateNvM()
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
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_05UpdateNvM(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;

  DCM_IGNORE_UNREF_PARAM(pContext);
  DCM_IGNORE_UNREF_PARAM(opStatus);
  DCM_IGNORE_UNREF_PARAM(pMsgContext);
  DCM_IGNORE_UNREF_PARAM(ErrorCode);

  /* ----- Implementation ----------------------------------------------- */
# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
  if (pRepContext->StoreState == TRUE)
  {
    lStdResult = Dcm_UtiNvMWrite(opStatus, &(pRepContext->SessionContext), DCM_SVC_86_NVRAM_BLOCKID);                                                /* SBSW_DCM_ROE_NV_ACCESS */ /* PRQA S 0315 */ /* MD_MSR_Dir1.1 */
  }
  else
  {
    Dcm_Svc86RemovePersistedStart();
  }

  if (lStdResult == DCM_E_NOT_OK)
  {
    /* NvM storage failed */
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else if (lStdResult == DCM_E_PENDING)
  {
    /* Just try later */
  }
  else /* DCM_E_OK */
# endif
  {
    Dcm_Svc86RoeStateType lOldRoEState = Dcm_Svc86SchdGetRoEState();

    /* Update the temporarily stored session context data */
    Dcm_Svc86SchdSetSessionContext(&pRepContext->SessionContext);                                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lOldRoEState == DCM_SVC86_ROE_STATE_STOPPED)
    {
      Dcm_Svc86SchdStartAll();
    }
  }

  return lStdResult;
}

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_07Handler()
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
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_07Handler(                                                                                        /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean        lSignedComparison;
  Std_ReturnType lStdResult;
  uint8          lEventWindowTime;
  uint8          lComparisonLogic;
  uint8          lHysteresisPercentage;
  uint8          lRawRefCompVal[4];
  uint8          lBitLength;
  uint8          lNumDidRecords = Dcm_Svc86SchdGetOnCompOfValNumRecords();
  uint16         lOffset;
  uint16         lDidOfEventTypeRecord;
  uint16         lLocalization;

  DCM_IGNORE_UNREF_PARAM(opStatus);

  /* ----- Implementation ----------------------------------------------- */
  if (pMsgContext->reqDataLen >= 14u)
  {
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lEventWindowTime);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &lDidOfEventTypeRecord);                                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lComparisonLogic);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsUN(pMsgContext, lRawRefCompVal, 4u, 4u);                                                                                  /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lHysteresisPercentage);                                                                                  /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &lLocalization);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

    lOffset           = lLocalization & 0x03FFu;                    /* mask the first 10 bits of the localization (Offset) */
    lBitLength        = (uint8)((lLocalization & 0x7C00u) >> 10u);  /* mask 5 bits of the bit length (bits 10-14) */
    lSignedComparison = (boolean)((lLocalization & 0x8000u) != 0u); /* mask 1 bit that represent if comparison is with sign or not  (bit 15) */

    if ((pRepContext->StoreState == TRUE)
      || (lNumDidRecords == DCM_SVC_86_MAX_NUM_COMPARISON_OF_VALUE_EVENTS)                                                                           /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
      || (Dcm_UtiGetReqDataAsU8(pMsgContext) != 0x22u) /* Only SID 0x22 is allowed as STRT */                                                        /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
      || (pMsgContext->reqDataLen > DCM_SVC_86_07_MAX_SUPPORTED_STRT_LENGTH)
      || ((lComparisonLogic < 1u) || (lComparisonLogic > 4u)) /* comparison logic should be between 1 to 4 */
      || (lHysteresisPercentage > 100u) /* hysteresis value should be between 0x00 (0%) to 0x64 (100%) */
      || ((lSignedComparison == TRUE) && (((lBitLength % 8u) != 0u) || (lBitLength == 24u))))
    {
      lStdResult = DCM_E_NOT_OK;
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord;

      pDidRecord = Dcm_Svc86Schd_07GetDidRecord(lNumDidRecords);
      pRepContext->DidInfoContext.Did = lDidOfEventTypeRecord; /* DID to observe */                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      pDidRecord->STRTLength = (uint8)pMsgContext->reqDataLen;                                                                                       /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
      pDidRecord->ComparisonLogic = lComparisonLogic;                                                                                                /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
      pDidRecord->HysteresisPercentage = lHysteresisPercentage;                                                                                      /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
      pDidRecord->TriggerActive = TRUE;                                                                                                              /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
      pDidRecord->Offset = lOffset;                                                                                                                  /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
      pDidRecord->BitLength = lBitLength;                                                                                                            /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
      pDidRecord->Signedness = lSignedComparison;                                                                                                    /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */
      pDidRecord->RawRefCompVal = Dcm_UtiMake32Bit(lRawRefCompVal[0], lRawRefCompVal[1], lRawRefCompVal[2], lRawRefCompVal[3]);                      /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */

      Dcm_UtiConsumeReqDataAsUN(pMsgContext, pDidRecord->STRT, DCM_SVC_86_07_MAX_SUPPORTED_STRT_LENGTH, pMsgContext->reqDataLen);                    /* SBSW_DCM_COMB_PTR_FORWARD */

      /* Prepare response data */
      Dcm_UtiProvideResDataAsU8(pMsgContext, 0x00u); /* Number of activated elements is always zero */                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU16(pMsgContext, lDidOfEventTypeRecord);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU8(pMsgContext, pDidRecord->ComparisonLogic);                                                                           /* SBSW_DCM_COMB_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU32(pMsgContext, pDidRecord->RawRefCompVal);                                                                            /* SBSW_DCM_COMB_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU8(pMsgContext, pDidRecord->HysteresisPercentage);                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU16(pMsgContext, lLocalization);                                                                                        /* SBSW_DCM_COMB_PTR_FORWARD */
      Dcm_UtiProvideResDataAsUN(pMsgContext, pDidRecord->STRT, pDidRecord->STRTLength);                                                              /* SBSW_DCM_COMB_PTR_FORWARD */

      Dcm_RepeaterNextSubStep(pContext, DCM_SVC86_07_SUBPROGRESS_DIDLOOKUP);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP;
    }
  }
  else
  {
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_07DidLookUp()
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
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_07DidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult;

  /* ----- Implementation ----------------------------------------------- */
  lStdResult = Dcm_DidMgrGetDidInfo(pMsgContext->threadId, opStatus, &pRepContext->DidInfoContext, &pRepContext->DidOpTypeContext, DCM_DIDMGR_OP_READ); /* SBSW_DCM_PARAM_PTR_FORWARD */

  switch (lStdResult)
  {
  case DCM_E_OK:
    if (Dcm_DidMgrIsOpSupported(Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DidInfoContext)), DCM_DIDMGR_OP_DEFINE))
    {
      /* No DynDID may be referenced! */
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    else
    {
      lStdResult = Dcm_StateCheckDID(Dcm_NetPbCfgGetConnHdlOfRxPduId(pMsgContext->rxPduId)                                                           /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
                                    ,pRepContext->DidInfoContext.Did
                                    ,DCM_DIDMGR_OP_READ
                                    ,Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext))->ExecCondRef
                                    ,ErrorCode);                                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */

      if (lStdResult == DCM_E_OK)
      {
        Dcm_RepeaterNextSubStep(pContext, DCM_SVC86_07_SUBPROGRESS_DIDCHECKCONDITIONS);                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdResult = DCM_E_LOOP;
      } /* else DCM_E_NOT_OK (ErrorCode already set) */
    }
    break;
  case DCM_E_PENDING:
    /* lStdResult is already set */
    break;
  default: /* DCM_E_NOT_OK */
    /* source DID is not supported */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_07DidCheckCondition()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_07DidCheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_LOOP;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);

  /* ----- Implementation ----------------------------------------------- */
  if (Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext))
     ,DCM_DIDMGR_OPTYPE_READCHKCOND))
  {
    /* Check condition on this DID */
    Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_READCHKCOND);                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_DidMgrReadCheckCond(opStatus
                                        ,&(pRepContext->DidInfoContext)
                                        ,&(pRepContext->DidOpTypeContext)
                                        ,ErrorCode);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

    switch (lStdResult)
    {
    case DCM_E_OK:
      lStdResult = DCM_E_LOOP;
      break; /* go on with next DID */
    case DCM_E_PENDING:
      break;
    default: /* DCM_E_NOT_OK */
      break; /* finish service processing immediately */
    }
  }/* else - not supported operation - go on with getLength */

  if (lStdResult == DCM_E_LOOP)
  {
    Dcm_RepeaterNextSubStep(pContext, DCM_SVC86_07_SUBPROGRESS_DIDGETLENGTH);                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  return lStdResult; /* speed up processing */
}
# endif

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc86_07DidGetLength()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc86_07DidGetLength(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc86RepeaterProxyContextPtrType pRepContext
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType lStdResult = DCM_E_OK;
  uint8          lNumDidRecords = Dcm_Svc86SchdGetOnCompOfValNumRecords();

  DCM_IGNORE_UNREF_PARAM(pContext);
  DCM_IGNORE_UNREF_PARAM(pMsgContext);

  /* ----- Implementation ----------------------------------------------- */
  if (Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext)), DCM_DIDMGR_OPTYPE_READLENGTH))
  {
    Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_READLENGTH);                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_DidMgrReadLength(opStatus
                                     ,&(pRepContext->DidInfoContext)
                                     ,&(pRepContext->DidOpTypeContext)
                                     ,ErrorCode);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if (lStdResult == DCM_E_OK)
  {
    Dcm_Svc86OnCompOfValDidRecordPtrType pDidRecord;

    pDidRecord = Dcm_Svc86Schd_07GetDidRecord(lNumDidRecords);

    if ( (pRepContext->DidInfoContext.ReservedLength > DCM_SVC_86_MAX_SUPPORTED_DID_LENGTH)
      || (((uint32)pDidRecord->Offset + pDidRecord->BitLength) > ((uint32)pRepContext->DidInfoContext.DidLength * 8u)) )
    {
      lStdResult = DCM_E_NOT_OK;
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      /* Add scheduler job - set the operation now since not changed later */
      Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_READ);                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

      pDidRecord->DidInfoContext = pRepContext->DidInfoContext;                                                                                      /* SBSW_DCM_POINTER_WRITE_86DIDRECORD */

      Dcm_Svc86SchdIncOnCompOfValNumRecords();

#  if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
      Dcm_Svc86RemovePersistedStart();
#  endif
    }
  }

  return lStdResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if ((DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON) || (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Dcm_DemTriggerOnDTCStatus()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DemTriggerOnDTCStatus(
  uint32 DTC,
  uint8 DTCStatusOld,
  uint8 DTCStatusNew
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;
  Std_ReturnType lReturnVal = DCM_E_OK;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(Dcm_DebugIsModuleInitialized() == FALSE))
  {
    lErrorId = DCM_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    if (Dcm_Svc86SchdGetRoEState() != DCM_SVC86_ROE_STATE_STOPPED)
    {
#  if(DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
      Dcm_Svc86Schd_01TriggerOnDTCStatus(DTCStatusOld, DTCStatusNew);
#  endif
#  if(DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
      Dcm_Svc86Schd_09TriggerOnDTCStatus(DTC, DTCStatusOld, DTCStatusNew);
#  endif
    }
  }
  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_DEMTRIGGERONDTCSTATUS, lErrorId);

  DCM_IGNORE_UNREF_PARAM(DTC);
  return lReturnVal;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc86OnChangeSession()
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
 */
FUNC(void, DCM_CODE) Dcm_Svc86OnChangeSession(
  Dcm_CfgStateGroupOptType oldSession,
  Dcm_CfgStateGroupOptType newSession
  )
{
  Dcm_Svc86SchdOnChangeSession(oldSession, newSession);
}

/**********************************************************************************************************************
 *  Dcm_TriggerOnEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_TriggerOnEvent(
  uint8 RoeEventId                                                                                                                                   /* PRQA S 1330 */ /* MD_Dcm_RteSpecific_1330 */
  )
{
  DCM_IGNORE_UNREF_PARAM(RoeEventId);

  Dcm_DebugReportError(DCM_SID_TRIGGERONEVENT, DCM_E_INVALID_CONFIG);

  return DCM_E_NOT_OK;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service86_00Processor()
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
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service86_00Processor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                            lStdResult = DCM_E_OK;
  Dcm_Svc86RepeaterProxyContextConstPtrType pRepContext;

  DCM_IGNORE_UNREF_PARAM(opStatus);

  /* ----- Implementation ----------------------------------------------- */
  pRepContext = Dcm_RepeaterGetSvc86Context(pContext->ThreadId);

  if(pRepContext->StoreState == TRUE)
  {
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    uint8 lEventWindowTime;

    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lEventWindowTime);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */

    /* Prepare response data */
    Dcm_UtiProvideResDataAsU8(pMsgContext, 0x00u);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (Dcm_Svc86SchdGetRoEState() != DCM_SVC86_ROE_STATE_STOPPED)
    {
# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
      Dcm_Svc86RemovePersistedStart();
# endif
      Dcm_Svc86SchdStopAll();
    }
  }

  return lStdResult;
}

# if (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service86_01Processor()
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
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service86_01Processor(                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                            lStdResult;
  uint8                                     lEventWindowTime;
  uint8                                     lDtcStatusMask;
  Dcm_Svc86RepeaterProxyContextConstPtrType pRepContext;

  DCM_IGNORE_UNREF_PARAM(opStatus);

  /* ----- Implementation ----------------------------------------------- */
  pRepContext = Dcm_RepeaterGetSvc86Context(pContext->ThreadId);

  if (pMsgContext->reqDataLen >= 4u)
  {
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lEventWindowTime);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lDtcStatusMask);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

    if ( (pRepContext->StoreState == TRUE)
       ||(Dcm_UtiGetReqDataAsU8(pMsgContext) != 0x19u) /* Only SID 0x19 is allowed as STRT */                                                        /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
       ||((Dcm_UtiGetReqDataAsU8Rel(pMsgContext, 1u) & 0x80u) != 0u) /* SPRMIB is not allowed for STRT */
       ||(pMsgContext->reqDataLen > DCM_SVC_86_01_MAX_SUPPORTED_STRT_LENGTH) )
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    else
    {
      Dcm_Svc86OnDtcChangeEventRecordPtrType pDTCEventRecord = Dcm_Svc86SchdGetOnDtcChangeEventRecord();

      Dcm_TskClrEvent(DCM_TSK_ID_ROE, DCM_TSK_EV_ROE_86_01_DTC_STATUS_CHANGE_DETECTED
                                    | DCM_TSK_EV_ROE_86_01_DTC_STATUS_CHANGE_SEND_STRT);

      pDTCEventRecord->DtcRecord.STRTLength = (uint8)pMsgContext->reqDataLen;                                                                        /* SBSW_DCM_POINTER_WRITE_86DTCRECORDONDTCCHANGE */

      Dcm_UtiConsumeReqDataAsUN(pMsgContext
                              , pDTCEventRecord->DtcRecord.STRT
                              , DCM_SVC_86_01_MAX_SUPPORTED_STRT_LENGTH
                              , pMsgContext->reqDataLen);                                                                                            /* SBSW_DCM_COMB_PTR_FORWARD */

      Dcm_UtiEnterCriticalSection();
      /*=================================*
        BEGIN CRITICAL SECTION
       *=================================*/
      pDTCEventRecord->DtcRecord.DTCStatusMask = lDtcStatusMask;                                                                                     /* SBSW_DCM_POINTER_WRITE_86DTCRECORDONDTCCHANGE */
      pDTCEventRecord->DtcEventState = DCM_SVC86_EVENT_STATE_ACTIVE;                                                                                 /* SBSW_DCM_POINTER_WRITE_86DTCRECORDONDTCCHANGE */
      /*=================================*
        END CRITICAL SECTION
       *=================================*/
      Dcm_UtiLeaveCriticalSection();

      /* Report response data */
      Dcm_UtiProvideResDataAsU8(pMsgContext, 0x00u);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU8(pMsgContext, lDtcStatusMask);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsUN(pMsgContext
                               ,pDTCEventRecord->DtcRecord.STRT
                               ,pDTCEventRecord->DtcRecord.STRTLength);                                                                              /* SBSW_DCM_COMB_PTR_FORWARD */

#  if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
      Dcm_Svc86RemovePersistedStart();
#  endif

      lStdResult = DCM_E_OK;
    }
  }
  else
  {
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_03_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service86_03Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service86_03Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc86RepeaterProxyContextPtrType pRepContext = Dcm_RepeaterGetSvc86Context(pContext->ThreadId);

  switch (Dcm_RepeaterGetSubProgress(pContext))                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
  case DCM_SVC86_03_SUBPROGRESS_INITIAL:
    lStdResult = Dcm_Svc86_03Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC86_03_SUBPROGRESS_DIDLOOKUP:
    lStdResult = Dcm_Svc86_03DidLookUp(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC86_03_SUBPROGRESS_DIDCHECKCONDITIONS:
    lStdResult = Dcm_Svc86_03DidCheckCondition(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                             /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC86_03_SUBPROGRESS_DIDGETLENGTH:
    lStdResult = Dcm_Svc86_03DidGetLength(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                  /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Service86_05Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service86_05Processor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc86RepeaterProxyContextPtrType pRepContext = Dcm_RepeaterGetSvc86Context(pContext->ThreadId);

  switch (Dcm_RepeaterGetSubProgress(pContext))                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
  case DCM_SVC86_05_SUBPROGRESS_INITIAL:
    lStdResult = Dcm_Svc86_05Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC86_05_SUBPROGRESS_UPDATE_NVM:
    lStdResult = Dcm_Svc86_05UpdateNvM(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Service86_06Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service86_06Processor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                            lStdResult = DCM_E_OK;
  Dcm_Svc86RepeaterProxyContextConstPtrType pRepContext;

  DCM_IGNORE_UNREF_PARAM(pContext);
  DCM_IGNORE_UNREF_PARAM(opStatus);

  /* ----- Implementation ----------------------------------------------- */
  pRepContext = Dcm_RepeaterGetSvc86Context(pContext->ThreadId);

  if (pRepContext->StoreState == TRUE)
  {
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    uint8 lEventWindowTime;

    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lEventWindowTime);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */

# if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
    Dcm_Svc86RemovePersistedStart();
# endif
    Dcm_Svc86SchdClearAll();

    Dcm_UtiProvideResDataAsU8(pMsgContext, 0x00u);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  return lStdResult;
}

# if (DCM_SVC_86_07_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service86_07Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service86_07Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc86RepeaterProxyContextPtrType pRepContext = Dcm_RepeaterGetSvc86Context(pContext->ThreadId);

  switch (Dcm_RepeaterGetSubProgress(pContext))                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
  case DCM_SVC86_07_SUBPROGRESS_INITIAL:
    lStdResult = Dcm_Svc86_07Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC86_07_SUBPROGRESS_DIDLOOKUP:
    lStdResult = Dcm_Svc86_07DidLookUp(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC86_07_SUBPROGRESS_DIDCHECKCONDITIONS:
    lStdResult = Dcm_Svc86_07DidCheckCondition(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                             /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC86_07_SUBPROGRESS_DIDGETLENGTH:
    lStdResult = Dcm_Svc86_07DidGetLength(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                  /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service86_09Processor()
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
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service86_09Processor(                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
/* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                            lStdResult;
  uint8                                     lEventWindowTime;
  uint8                                     lDtcStatusMask;
  uint8                                     lSvc19Subfunc;
  Dcm_Svc86RepeaterProxyContextConstPtrType pRepContext;

  DCM_IGNORE_UNREF_PARAM(opStatus);

  /* ----- Implementation ----------------------------------------------- */
  pRepContext = Dcm_RepeaterGetSvc86Context(pContext->ThreadId);

  if (pMsgContext->reqDataLen >= 3u)
  {
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lEventWindowTime);                                                                                       /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lDtcStatusMask);                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lSvc19Subfunc);                                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */

    if ( (pRepContext->StoreState == TRUE)
       ||(Dcm_Svc86_09CheckEventTypeRecord(lSvc19Subfunc, pMsgContext->reqDataLen) == FALSE)) /* Check if event type subfunction and length are correct */
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    else
    {
      Dcm_Svc86DtcRecordInfoEventRecordPtrType pDTCRecordInfo = Dcm_Svc86Schd_09GetDtcRecordInfo();
      Dcm_MsgLenType lSTRTLen = pMsgContext->reqDataLen;

      pDTCRecordInfo->DtcRecord.STRT[1u] = lSvc19Subfunc;                                                                                            /* SBSW_DCM_POINTER_WRITE_86DTCINFORECORDPTR */ /* SBSW_DCM_POINTER_WRITE_86DTCRECORDSTRT */
      pDTCRecordInfo->DtcRecord.STRTLength = (DCM_SVC_86_09_MIN_SUPPORTED_STRT_LENGTH + (uint8)lSTRTLen);                                            /* SBSW_DCM_POINTER_WRITE_86DTCINFORECORDPTR */

      /* Data is copied to index 5 to reserve gap to fit DTC later */
      Dcm_UtiMemCopySafe(Dcm_UtiGetReqData(pMsgContext),
                         pDTCRecordInfo->DtcRecord.STRT,                                                                                             /* SBSW_DCM_POINTER_WRITE_86DTCINFORECORDPTR */
                         DCM_SVC_86_09_MIN_SUPPORTED_STRT_LENGTH,
                         DCM_SVC_86_09_MAX_SUPPORTED_STRT_LENGTH,
                         lSTRTLen);                                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */ /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

      Dcm_UtiEnterCriticalSection();
      /*=================================*
        BEGIN CRITICAL SECTION
       *=================================*/
      Dcm_TskClrEvent(DCM_TSK_ID_ROE, DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_DETECTED
                                    | DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_STRT
                                    | DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_BUFFER_FULL
                                    | DCM_TSK_EV_ROE_86_09_DTC_STATUS_CHANGE_SEND_INTM_RESP);

      Dcm_Svc86Schd_09RingBufferInit();

      pDTCRecordInfo->DtcRecord.DTCStatusMask = lDtcStatusMask;                                                                                      /* SBSW_DCM_POINTER_WRITE_86DTCINFORECORDPTR */
      pDTCRecordInfo->DtcEventState = DCM_SVC86_EVENT_STATE_ACTIVE;                                                                                  /* SBSW_DCM_POINTER_WRITE_86DTCINFORECORDPTR */
      /*=================================*
        END CRITICAL SECTION
       *=================================*/
      Dcm_UtiLeaveCriticalSection();

      Dcm_UtiProvideResDataAsU8(pMsgContext, 0x00u);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU8(pMsgContext, lEventWindowTime);                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU8(pMsgContext, lDtcStatusMask);                                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiProvideResDataAsU8(pMsgContext, lSvc19Subfunc);                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

      /* Provide event type record to response message */
      Dcm_UtiProvideResDataAsUN(pMsgContext,
                               &pDTCRecordInfo->DtcRecord.STRT[DCM_SVC_86_09_MIN_SUPPORTED_STRT_LENGTH],
                               lSTRTLen);                                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

      pDTCRecordInfo->DtcRecord.IntmRspMsgLength = (uint8)pMsgContext->resDataLen + 2u;                                                              /* SBSW_DCM_POINTER_WRITE_86DTCINFORECORDPTR */

      /* The first byte of response data is not copied as the no. of identified event must be 0xFF as per ISO 14229 */
      Dcm_UtiMemCopySafe(Dcm_UtiGetResDataAt(pMsgContext, 1u),
                         pDTCRecordInfo->DtcRecord.IntmRspMsg,                                                                                       /* SBSW_DCM_POINTER_WRITE_86DTCINFORECORDPTR */
                         3u,
                         DCM_SVC_86_09_MAX_SUPPORTED_INTERMEDIATE_RESPONSE_LENGTH,
                         pMsgContext->resDataLen - 1u);                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */ /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

#  if (DCM_SVC_86_NVM_SUPPORT_ENABLED == STD_ON)
      Dcm_Svc86RemovePersistedStart();
#  endif

      lStdResult = DCM_E_OK;
    }
  }
  else
  {
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Service86Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service86Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc86RepeaterProxyContextPtrType pRepContext = Dcm_RepeaterGetSvc86Context(pContext->ThreadId);

  switch (Dcm_RepeaterGetProgress(pContext))                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
  case DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL:
    lStdResult = Dcm_Svc86Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC86_PROGRESS_SUBFUNCTION:
    lStdResult = Dcm_Svc86SubFuncHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}
# define DCM_STOP_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc86.c
 *********************************************************************************************************************/
