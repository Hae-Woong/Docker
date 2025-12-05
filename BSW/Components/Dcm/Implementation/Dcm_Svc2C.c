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
/**        \file  Dcm_Svc2C.c
 *         \unit  Svc2C
 *        \brief  Contains the implementation of Service 0x2C unit.
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
#define DCM_SVC2C_SOURCE

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
#include "Dcm_Svc2C.h"
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_MSR_UT_OPTIONAL_UNIT */
# include "Dcm_Svc2CDefMgr.h"
# include "Dcm_CfgWrap.h"
# include "Dcm_Debug.h"
# include "Dcm_Diag.h"
# include "Dcm_Uti.h"
# include "Dcm_Repeater.h"
# include "Dcm_State.h"
# include "Dcm_Net.h"
# include "Dcm_NetPbCfg.h"
# include "Dcm_MemMgr.h"
# include "Dcm_Svc2A.h"
# include "Dcm_Svc2ASchd.h"
# if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
#  include "NvM.h"
# endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DCM_SVC2C_PROGRESS_SUBFUNCTION                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC2C_PROGRESS_01SRC_DIDLOOKUP                          ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))
# define DCM_SVC2C_PROGRESS_01SRC_DIDCHECKCONDITIONS                 ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 2u))
# define DCM_SVC2C_PROGRESS_01SRC_DIDGETLENGTH                       ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 3u))
# define DCM_SVC2C_PROGRESS_03_DIDLOOKUP                             ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 4u))

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# define Dcm_Svc2CGetNetBufferSize(pContext)                         ((Dcm_NetPbCfgGetBufferInfo(Dcm_DiagGetTranspObj(pContext->ThreadId)->BufferHdl)->Size)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2CHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x2C Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CSubFuncHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x2C SubFunction Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CSubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidLookUp()
 *********************************************************************************************************************/
/*! \brief          Processes the source items of service 0x2C 0x01.
 *  \details        Performs a lookup for a specific source DID.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidCheckCondition()
 *********************************************************************************************************************/
/*! \brief          Processes the source items of service 0x2C 0x01.
 *  \details        Checks the condition of a specific source DID.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidCheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidGetLength()
 *********************************************************************************************************************/
/*! \brief          Processes the source items of service 0x2C 0x01.
 *  \details        Calculates and stores the length of a specific source DID.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidGetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C02SrcItemsGetLength()
 *********************************************************************************************************************/
/*! \brief          Processes the source items of service 0x2C 0x02.
 *  \details        Calculates the total length of all source items
 *  \param[in]      alfid                   The ALFID of the request
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Calculation succeeded
 *  \return         DCM_E_NOT_OK            Error during calculation
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C02SrcItemsGetLength(
  uint8 alfid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_Svc2C03DidLookUp()
 *********************************************************************************************************************/
/*! \brief          Service 0x2C 0x03 dedicated DID look up client.
 *  \details        -
 *  \param[in]      opStatus       Current operation status
 *  \param[in]      pMsgContext    Pointer to current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[out]     pRepContext    Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C03DidLookUp(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CSetMaxAllowedDynDIDLength()
 *********************************************************************************************************************/
/*! \brief          Set the maximum allowed length for the currently processed DynDID.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      pRepContext    Current repeater proxy context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2CSetMaxAllowedDynDIDLength(
  Dcm_ContextPtrType pContext,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2CReqLengthGetter()
 *********************************************************************************************************************/
/*! \brief          Service 0x2C sub-function length getter.
 *  \details        Always returns length of 0 byte to show that the requests have dynamic length and the concrete
 *                  length will be checked later.
 *  \param[in]      sfRef                The sub-function zero-based index.
 *  \return         Expected sub-function specific length.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc2CReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CParseAndCheckDynDid()
 *********************************************************************************************************************/
/*! \brief          Parses and validates the DynDID to be defined.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       Current operation status
 *  \param[in]      pMsgContext    Pointer to current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[out]     pRepContext    Current repeater proxy context
 *  \return         DCM_E_OK       Send response
 *  \return         DCM_E_NOT_OK   Send neagative response
 *  \return         DCM_E_PENDING  Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CParseAndCheckDynDid(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CCheckAndSetNumOfItems()
 *********************************************************************************************************************/
/*! \brief          Check DynDID has enough space to keep all requested definitions.
 *  \details        If the check succeeds also stores the number of requested items for further processing.
 *  \param[in]      numRequestedItems  Number of requested Items
 *  \param[out]     ErrorCode          The NRC
 *  \param[in,out]  pRepContext        Current repeater proxy context
 *  \return         DCM_E_OK        Check succeeded
 *  \return         DCM_E_NOT_OK    Check failed (NRC is stored in ErrorCode)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CCheckAndSetNumOfItems(
  Dcm_MsgLenType numRequestedItems,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  );
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
 *  Dcm_Svc2CHandler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_DiagSubServiceRefOptType lSubSvcRef;
  DCM_IGNORE_UNREF_PARAM(opStatus);

  lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                   ,Dcm_CfgSvc2CSubFuncLookUpTable
                                                   ,Dcm_CfgSvc2CSubFuncExecPrecondTable
                                                   ,Dcm_Svc2CReqLengthGetter
                                                   ,Dcm_DiagNoSequenceChecker
                                                   ,&lSubSvcRef
                                                   ,ErrorCode);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
  {
    pRepContext->SubSvcRef = (Dcm_DiagSubServiceRefMemType)lSubSvcRef;                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Perform SID specific checks */
    if (Dcm_CfgWrapSvc2CSubFuncInfo(pRepContext->SubSvcRef)->MinReqLength <= pMsgContext->reqDataLen) /* min length = (DDID, (MEMdef|DIDdef))? */
    {
      Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_SUBFUNCTION); /* delegate job */                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP; /* speed up processing */
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  } /* else DCM_E_NOT_OK (ErrorCode is already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CSubFuncHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CSubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                 lStdResult;
  Dcm_CfgSvc2CSubFuncInfoPtrType pSubFuncInfo;

  lStdResult = Dcm_CfgWrapSvc2CSubFuncInfoGetEntry(pRepContext->SubSvcRef, &pSubFuncInfo, ErrorCode);                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)                                                                                                                        /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = pSubFuncInfo->SubSvcFunc(pContext, opStatus, pMsgContext, ErrorCode);                                                               /* SBSW_DCM_CALL_FUNCPTR_SVC2CSUBFUNC */
  }

  return lStdResult;
}

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidLookUp()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidLookUp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;

  if(opStatus == DCM_INITIAL)
  {
     /* Extract source DID */
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &(pRepContext->SrcItemDidInfoContext.Did));                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  lStdReturn = Dcm_DidMgrGetDidInfo(pMsgContext->threadId
                                   ,opStatus
                                   ,&(pRepContext->SrcItemDidInfoContext)
                                   ,&(pRepContext->DynDidOpTypeContext) /* Use OpTypeContext of DynDID since no more needed at this time! */
                                   ,DCM_DIDMGR_OP_READ);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

  switch(lStdReturn)
  {
  case DCM_E_OK:
    if(Dcm_DidMgrIsOpSupported(Dcm_CfgDidMgrGetDidInfo(&(pRepContext->SrcItemDidInfoContext)), DCM_DIDMGR_OP_DEFINE))                                /* SBSW_DCM_PARAM_PTR_WRITE */
    {
      /* else - no DynDID may be referenced! */
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
    }
    else
    {
      lStdReturn = Dcm_StateCheckDID(Dcm_NetPbCfgGetConnHdlOfRxPduId(pMsgContext->rxPduId)                                                           /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
                                    ,pRepContext->SrcItemDidInfoContext.Did
                                    ,DCM_DIDMGR_OP_READ
                                    ,Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->SrcItemDidInfoContext))->ExecCondRef
                                    ,ErrorCode);                                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
      if(lStdReturn == DCM_E_OK)
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_01SRC_DIDCHECKCONDITIONS); /* next job */                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdReturn = DCM_E_LOOP;
      } /* else DCM_E_NOT_OK (ErrorCode already set) */
    }
    break;
  case DCM_E_PENDING:
    /* lStdReturn is already set */
    break;
  default: /* DCM_E_NOT_OK */
    /* source DID is not supported */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
    break;
  }

  return lStdReturn;
}
# endif

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidCheckCondition()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidCheckCondition(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  if(Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->SrcItemDidInfoContext)), DCM_DIDMGR_OPTYPE_READCHKCOND))                   /* SBSW_DCM_PARAM_PTR_WRITE */
  {
    DCM_IGNORE_UNREF_PARAM(pMsgContext);

    /* check condition on this DID */
    Dcm_DidMgrInitOpClassInfo(&(pRepContext->SrcItemDidInfoContext), DCM_DIDMGR_OPTYPE_READCHKCOND);                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = Dcm_DidMgrReadCheckCond(opStatus
                                        ,&(pRepContext->SrcItemDidInfoContext)
                                        ,&(pRepContext->DynDidOpTypeContext) /* Use OpTypeContext of DynDID since no more needed at this time! */
                                        ,ErrorCode);                                                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */

  } /* else - not supported operation - go on with getLength */

  if(lStdResult == DCM_E_OK)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_01SRC_DIDGETLENGTH); /* next job */                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP; /* speed up processing */
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01SrcDidGetLength()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01SrcDidGetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Dcm_Svc2CDynDidSrcItemPtrType pSrcItem;
  Std_ReturnType lStdReturn = DCM_E_OK;

  pSrcItem = Dcm_Svc2CDefMgrGetSrcItem(pRepContext->SrcItemIndex);

  DCM_IGNORE_UNREF_PARAM(opStatus);

  if(Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->SrcItemDidInfoContext)), DCM_DIDMGR_OPTYPE_READLENGTH))                    /* SBSW_DCM_PARAM_PTR_WRITE */
  {
    Dcm_DidMgrInitOpClassInfo(&(pRepContext->SrcItemDidInfoContext), DCM_DIDMGR_OPTYPE_READLENGTH);                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Calculate DID data length if applicable */
    lStdReturn = Dcm_DidMgrReadLength(opStatus
                                     ,&(pRepContext->SrcItemDidInfoContext)
                                     ,&(pRepContext->DynDidOpTypeContext) /* Use OpTypeContext of DynDID since no more needed at this time! */
                                     ,ErrorCode);                                                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  if(lStdReturn == DCM_E_OK)
  {
    uint8 srcDidPos;
    uint8 srcDidSize;

    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &srcDidPos);                                                                                              /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &srcDidSize);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

    if ((srcDidPos == 0u)
      ||(srcDidSize == 0u)
      ||(((Dcm_DidMgrDidLengthType)srcDidPos + (Dcm_DidMgrDidLengthType)srcDidSize - 1u) > pRepContext->SrcItemDidInfoContext.DidLength) )
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /* source DID referenced data out of boundary or memory size of 0 */                                     /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK; /* finish service processing immediately */
    } /* check for possible read buffer or response overflow */
    else if ((!Dcm_UtiIsAdditionSafe(Dcm_DidMgrDidLengthType, pRepContext->DynDidLength, srcDidSize))
      || (Dcm_UtiPromotedSum(Dcm_DidMgrDidLengthType, pRepContext->DynDidLength, srcDidSize) > pRepContext->MaxAllowedLength))
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /* too much data referenced */                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK; /* finish service processing immediately */
    }
    else /* Add this DID to the definition */
    {
      /* update total length of the DynDID for commitment */
      pRepContext->DynDidLength += srcDidSize;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

      /* store referenced data */
      pSrcItem->DidDescriptor.DidInfoIdx = pRepContext->SrcItemDidInfoContext.Descriptor.DidInfoIdx;                                                 /* SBSW_DCM_POINTER_WRITE_2CSRCITEM */
      pSrcItem->DidDescriptor.Offset = (uint8)(srcDidPos - 1u);                                                                                      /* SBSW_DCM_POINTER_WRITE_2CSRCITEM */
      pSrcItem->DidDescriptor.Size = srcDidSize;                                                                                                     /* SBSW_DCM_POINTER_WRITE_2CSRCITEM */
      Dcm_Svc2CDefMgrSetDidSrcItem(pRepContext->SrcItemIndex); /* set the type of the source item to DID */                                          /* SBSW_DCM_POINTER_WRITE_BITSET */

      ++(pRepContext->SrcItemIndex); /* next item to be configured */                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
      ++(pRepContext->DynDidItemCount); /* update number of items for commitment */                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

      --(pRepContext->ReqNumOfItems); /* commit processed item */                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
      if (pRepContext->ReqNumOfItems > 0u) /* any item left? */
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_01SRC_DIDLOOKUP); /* next job */                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdReturn = DCM_E_LOOP; /* speed up processing */
      }
      else
      {
        lStdReturn = DCM_E_OK; /* all source items finished service processing immediately */
      }
    }
  }
  return lStdReturn;
}
# endif

# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C02SrcItemsGetLength()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C02SrcItemsGetLength(
  uint8 alfid,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn = DCM_E_NOT_OK;
  Dcm_MemMgrMemBlockType lMemBlock;

  while(pRepContext->ReqNumOfItems > 0u)
  {
    Dcm_MemMgrParseMemStream(alfid, pMsgContext, &lMemBlock);                                                                                        /* SBSW_DCM_COMB_PTR_FORWARD */
    lStdReturn = Dcm_MemMgrCheckMemBlock(&lMemBlock, DCM_MEMMGR_OP_READ, ErrorCode);                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lStdReturn == DCM_E_NOT_OK)
    {
      /* stop immediately and report the error */
      pRepContext->ReqNumOfItems = 0u; /* break */                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    } /* else - check for definition length overflow */
    else if( (!Dcm_UtiIsAdditionSafe(uint32, pRepContext->DynDidLength, lMemBlock.Length))
           ||(Dcm_UtiPromotedSum(uint32, pRepContext->DynDidLength, lMemBlock.Length) > pRepContext->MaxAllowedLength) )
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE; /* too much data referenced */                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
      /* finish service processing immediately */
      pRepContext->ReqNumOfItems = 0u; /* break */                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      /* else - add this address to the DynDID definition */
      Dcm_Svc2CDynDidSrcItemPtrType pSrcItem = Dcm_Svc2CDefMgrGetSrcItem(pRepContext->SrcItemIndex);

      /* update total length of the DynDID for commitment */
      pRepContext->DynDidLength += (Dcm_DidMgrDidLengthType)(lMemBlock.Length);                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */

      pSrcItem->MemDescriptor.MemBlock = lMemBlock;                                                                                                  /* SBSW_DCM_POINTER_WRITE_2CSRCITEM */

      Dcm_Svc2CDefMgrSetMemSrcItem(pRepContext->SrcItemIndex); /* set the type of the source item to MEMORY */                                       /* SBSW_DCM_POINTER_WRITE_BITSET */
      ++(pRepContext->DynDidItemCount); /* update number of items for commitment */                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      ++(pRepContext->SrcItemIndex);    /* next item to be configured */                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
      --(pRepContext->ReqNumOfItems);   /* commit processed item */                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  return lStdReturn;
}
# endif

# if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_Svc2C03DidLookUp()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C03DidLookUp(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;

  if(opStatus == DCM_INITIAL)
  {
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &(pRepContext->DynDidInfoContext.Did));                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_UtiProvideResDataAsU16(pMsgContext, pRepContext->DynDidInfoContext.Did); /* positive response contains the DynDID (2Byte) */                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  lStdReturn = Dcm_DidMgrGetDidInfo(pMsgContext->threadId
                                   ,opStatus
                                   ,&pRepContext->DynDidInfoContext
                                   ,&pRepContext->DynDidOpTypeContext
                                   ,DCM_DIDMGR_OP_DEFINE);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

  switch(lStdReturn)
  {
  case DCM_E_OK:
    /* clear concrete DynDID */
    Dcm_Svc2CDefMgrClear((Dcm_CfgDidMgrDynDidHandleMemType)(Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DynDidInfoContext))->OpRef));
    break;
  case DCM_E_PENDING:
    /* lStdReturn is already set */
    break;
  default: /* DCM_E_NOT_OK */
    /* source DID is not supported */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  }

  return lStdReturn;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CSetMaxAllowedDynDIDLength()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2CSetMaxAllowedDynDIDLength(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  /* initialize common DynDID request context */
# if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
  if (Dcm_Svc2AIsPeriodicDid(pRepContext->DynDidInfoContext.Did))
  {
    Dcm_NetConnRefMemType lConnHdl = Dcm_Svc2AGetConnectionOfDID(pRepContext->DynDidInfoContext.Did);
    if (lConnHdl < DCM_NET_INVALID_CONNHDL)                                                                                                          /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
    {
      pRepContext->MaxAllowedLength = (Dcm_DidMgrDidLengthType)Dcm_NetPbCfgGetConnUudtFrameSize(lConnHdl);                                           /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      pRepContext->MaxAllowedLength = DCM_SVC2A_MAX_RESPONSE_LEN;                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  else
# endif
  {
    /*
     * Perform length check based on the current protocol only. Sid 0x22 will do additional checks at read time in
     * case a client from another protocol is trying to access the DynDID!
     */
    Dcm_CfgNetBufferSizeMemType lBufferSize = Dcm_Svc2CGetNetBufferSize(pContext);
    if (lBufferSize > Dcm_UtiMaxValueOfUintType(Dcm_DidMgrDidLengthType))
    {
      pRepContext->MaxAllowedLength = Dcm_UtiMaxValueOfUintType(Dcm_DidMgrDidLengthType) - 3u;                                                       /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    else
    {
      pRepContext->MaxAllowedLength = (Dcm_DidMgrDidLengthType)lBufferSize - 3u;                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2CCheckAndSetNumOfItems()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CCheckAndSetNumOfItems(
  Dcm_MsgLenType numRequestedItems,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;

  if (((Dcm_MsgLenType)Dcm_DidMgrGetDynDidSize(pRepContext->DynDidHandle) - pRepContext->DynDidItemCount) < numRequestedItems)
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
  {
    pRepContext->ReqNumOfItems = (uint8)numRequestedItems;                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_OK;
  }

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CParseAndCheckDynDid()
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
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CParseAndCheckDynDid(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn;

  if(opStatus == DCM_INITIAL)
  {
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &pRepContext->DynDidInfoContext.Did);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  lStdReturn = Dcm_DidMgrGetDidInfo(pMsgContext->threadId
                                   ,opStatus
                                   ,&pRepContext->DynDidInfoContext                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
                                   ,&pRepContext->DynDidOpTypeContext                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
                                   ,DCM_DIDMGR_OP_DEFINE);

  switch(lStdReturn)
  {
  case DCM_E_OK:
    lStdReturn = Dcm_StateCheckDID(Dcm_NetPbCfgGetConnHdlOfRxPduId(pMsgContext->rxPduId)                                                             /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
                                  ,pRepContext->DynDidInfoContext.Did
                                  ,DCM_DIDMGR_OP_READ
                                  ,Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DynDidInfoContext))->ExecCondRef
                                  ,ErrorCode);                                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    if (lStdReturn == DCM_E_OK)
    {
      Dcm_Svc2CDynDidItemConstPtrType pItem;

      Dcm_Svc2CSetMaxAllowedDynDIDLength(pContext, pRepContext);                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
      pRepContext->DynDidHandle = (Dcm_CfgDidMgrDynDidHandleMemType)Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DynDidInfoContext))->OpRef;               /* SBSW_DCM_PARAM_PTR_WRITE */

      pItem = Dcm_Svc2CDefMgrGetItem(pRepContext->DynDidHandle);
      pRepContext->DynDidLength = pItem->Length; /* set current length for updates during service processing */                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      pRepContext->SrcItemIndex = Dcm_Svc2CDefMgrGetSrcItemIndex(pRepContext->DynDidHandle, pItem->Count);                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      pRepContext->DynDidItemCount = pItem->Count;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */

      Dcm_UtiProvideResData(pMsgContext, 2u); /* positive response contains the DynDID (2Byte) */                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      /* lStdReturn = DCM_E_OK - already set */
    } /* else DCM_E_NOT_OK (ErrorCode already set) */
    break;
  case DCM_E_PENDING:
    break;
  default: /* DCM_E_NOT_OK */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
    break;
  }
  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CReqLengthGetter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc2CReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  )
{
  DCM_IGNORE_UNREF_PARAM(sfRef);
  /* Return sub-function specific length (always zero -> dynamic since it depends on the complete request content) */
  return 0u;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service2CInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Service2CInit(
  void
  )
{
  Dcm_Svc2CDefMgrInit();
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service2C_01Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service2C_01Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;

  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext = Dcm_RepeaterGetSvc2CContext(pContext->ThreadId);

  lStdReturn = Dcm_Svc2CParseAndCheckDynDid(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdReturn == DCM_E_OK)
  {
    /* there shall be an iteration (+[4Byte]) */
    if( (pMsgContext->reqDataLen > 0u)
      &&( (pMsgContext->reqDataLen % 4u) == 0u) )
    {
      Dcm_MsgLenType lNumReqItems = pMsgContext->reqDataLen / 4u; /* calculate number of source items to be processed */

      /* check DynDID capacity */
      lStdReturn = Dcm_Svc2CCheckAndSetNumOfItems(lNumReqItems, ErrorCode, pRepContext);                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
      if(lStdReturn == DCM_E_OK)
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_01SRC_DIDLOOKUP);                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdReturn = DCM_E_LOOP; /* speed up processing */
      } /* else - Nrc and return value already set */
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
    }
  } /* else - just return the value: DCM_E_PENDING, DCM_E_NOT_OK (ErrorCode already set) */

  return lStdReturn;
}
# endif

# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service2C_02Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service2C_02Processor(                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;
  uint8          lAlfid;
  uint8          lBlockLength;

  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext = Dcm_RepeaterGetSvc2CContext(pContext->ThreadId);

  lStdReturn = Dcm_Svc2CParseAndCheckDynDid(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdReturn == DCM_E_OK)
  {
    /* Check, whether the ALFID is available */
    if (pMsgContext->reqDataLen != 0u)
    {
      /* the ALFID availability is checked within next call */
      lStdReturn = Dcm_MemMgrValidateAndGetAlfid(pMsgContext, &lAlfid, &lBlockLength, ErrorCode);                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      if (lStdReturn == DCM_E_OK)
      {
        /* pMsgContext->reqDataLen cannot be zero! */
        if((pMsgContext->reqDataLen % lBlockLength) == 0u) /* the remained request length shall be a multiple of the memory address and size request block */
        {
          Dcm_MsgLenType lNumReqItems = pMsgContext->reqDataLen / lBlockLength; /* calculate number of source items to be processed */

          /* check DynDID capacity */
          lStdReturn = Dcm_Svc2CCheckAndSetNumOfItems(lNumReqItems, ErrorCode, pRepContext);                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
          if (lStdReturn == DCM_E_OK)
          {
            lStdReturn = Dcm_Svc2C02SrcItemsGetLength(lAlfid, pMsgContext, ErrorCode, pRepContext);                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
          } /* else - ErrorCode and return value already set */
        }
        else
        {
          /* else - ErrorCode set to 0x13 */
          *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdReturn = DCM_E_NOT_OK;
        }
      } /* else - DCM_E_NOT_OK (ErrorCode already set) */
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdReturn = DCM_E_NOT_OK;
    }
  } /* else - DCM_E_NOT_OK (ErrorCode already set) */

  return lStdReturn;
}
# endif

# if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_Service2C_03Processor()
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
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service2C_03Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdReturn;

  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext = Dcm_RepeaterGetSvc2CContext(pContext->ThreadId);

  DCM_IGNORE_UNREF_PARAM(opStatus);

  pRepContext->DynDidItemCount = 0u; /* prepare for a successful clear */                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */

  if(pMsgContext->reqDataLen == 0u)
  {
    /* clear all DynDID definitions */
    Dcm_CfgDidMgrDynDidHandleOptType dynDidIter;
    for(dynDidIter = 0; dynDidIter < DCM_NUM_DYNDIDS; ++dynDidIter)
    {
      Dcm_Svc2CDefMgrClear((Dcm_CfgDidMgrDynDidHandleMemType)dynDidIter);
    }
    lStdReturn = DCM_E_OK;
  }
  else if (pMsgContext->reqDataLen == 2u)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC2C_PROGRESS_03_DIDLOOKUP); /* next job */                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdReturn = DCM_E_LOOP; /* speed up processing */
  }
  else
  {
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdReturn = DCM_E_NOT_OK;
  }
  return lStdReturn;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Service2CProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service2CProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc2CRepeaterProxyContextPtrType pRepContext = Dcm_RepeaterGetSvc2CContext(pContext->ThreadId);

  switch (Dcm_RepeaterGetProgress(pContext))                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
  case DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL:
    lStdResult = Dcm_Svc2CHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_SVC2C_PROGRESS_SUBFUNCTION:
    lStdResult = Dcm_Svc2CSubFuncHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC2C_PROGRESS_01SRC_DIDLOOKUP:
    lStdResult = Dcm_Svc2C01SrcDidLookUp(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_SVC2C_PROGRESS_01SRC_DIDCHECKCONDITIONS:
    lStdResult = Dcm_Svc2C01SrcDidCheckCondition(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_SVC2C_PROGRESS_01SRC_DIDGETLENGTH:
    lStdResult = Dcm_Svc2C01SrcDidGetLength(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_SUPPORT_ALWAYS TX */
  case DCM_SVC2C_PROGRESS_03_DIDLOOKUP:
    lStdResult = Dcm_Svc2C03DidLookUp(opStatus, pMsgContext, ErrorCode, pRepContext);                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
# endif
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Service2CPostProcessor()
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
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service2CPostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  if(status == DCM_RES_POS_OK)
  {
    Dcm_Svc2CRepeaterProxyContextConstPtrType pRepContext = Dcm_RepeaterGetSvc2CContext(pContext->ThreadId);

    if(pRepContext->DynDidItemCount != 0u) /* this is a definition service , not the clear one */
    {
      Dcm_Svc2CDynDidItemPtrType pItem;

# if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
      uint16 lDid = Dcm_DidMgrGetDynDidIdFromHandle(pRepContext->DynDidHandle);
      /* Kill any still not finished read periodic DID jobs to avoid inconsistent response data/length ! */
      Dcm_Svc2ASchdCancelReadByDid(lDid);
# endif
      /* Commit DynDID definition immediately */
      pItem = Dcm_Svc2CDefMgrGetItem(pRepContext->DynDidHandle);
      pItem->Length = pRepContext->DynDidLength;                                                                                                     /* SBSW_DCM_POINTER_WRITE_2CITEM */
      pItem->Count  = pRepContext->DynDidItemCount;                                                                                                  /* SBSW_DCM_POINTER_WRITE_2CITEM */
    } /* else - the dynDid has been already cleared */

# if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
    Dcm_UtiNvMSetRamBlockStatus((uint16)DCM_SVC_2C_NVRAM_BLOCKID);
# endif
  } /* else - the dynDid list properties will not be changed - all prepared data will be discarded */
}
# define DCM_STOP_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2C.c
 *********************************************************************************************************************/
