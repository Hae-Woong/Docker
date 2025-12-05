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
/**        \file  Dcm_PagedBuffer.c
 *         \unit  PagedBuffer
 *        \brief  Contains the implementation of PagedBuffer unit.
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
#define DCM_PAGEDBUFFER_SOURCE

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
#include "Dcm_PagedBuffer.h"
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)                                                                                                             /* COV_MSR_UT_OPTIONAL_UNIT */
# include "Dcm.h"
# include "Dcm_CfgWrap.h"
# include "Dcm_Debug.h"
# include "Dcm_Diag.h"
# include "Dcm_Net.h"
# include "Dcm_NetPbCfg.h"
# include "Dcm_Tsk.h"
# include "Dcm_Uti.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DCM_PAGEDBUFFER_ALL_DATA_PROVIDED                           (Dcm_PagedBufferDataProvisionStateType)0x00u /*!< All data provided */
# define DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED                          (Dcm_PagedBufferDataProvisionStateType)0x01u /*!< Already some data provided */
# define DCM_PAGEDBUFFER_WAIT_FOR_DATA                               (Dcm_PagedBufferDataProvisionStateType)0x02u /*!< Waiting for data provision */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 Dcm_PagedBufferDataProvisionStateType;

/*! Paged-buffer control data */
struct DCM_PAGEDBUFFERCONTEXTTYPE_TAG
{
  Dcm_MsgLenType                        RemainingLen;                 /*!< Keeps track on the data exchange progress */
  Dcm_CfgNetBufferSizeMemType           WriteIndex;                   /*!< Pointer to the next free buffer position ready to be written with new data portion */
  Dcm_CfgNetBufferSizeMemType           ReadIndex;                    /*!< Pointer to the next ready-to-send buffer position */
  Dcm_PagedBufferStateType              State;                        /*!< Paged-buffer state machine */
  Dcm_CfgNetBufferRefMemType            BufferHdl;                    /*!< Zero based reference to the USDT buffer object in use */
  uint8                                 UpdateFuncRef;                /*!< Zero based reference to the data provider handler Xxx_<Name>Update */
  uint8                                 CancelFuncRef;                /*!< Zero based reference to the data provider cancellation notification Xxx_<Name>Cancel */
  boolean                               UpdateFinished;               /*!< Data provider will no longer be called (job done) */
  Dcm_PagedBufferDataProvisionStateType UpdaterDataProvisionState;    /*!< Data provision state of Updater */
  Dcm_OpStatusType                      OpStatus;                     /*!< Current operation status */
};
typedef struct DCM_PAGEDBUFFERCONTEXTTYPE_TAG Dcm_PagedBufferContextType;
typedef P2VAR(Dcm_PagedBufferContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_PagedBufferContextPtrType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_PagedBufferGetContext()
 *********************************************************************************************************************/
/*! \brief          Returns a paged-buffer thread context element of the given index.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid       paged-buffer thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_PagedBufferContextPtrType, DCM_CODE) Dcm_PagedBufferGetContext(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_PagedBufferReset()
 *********************************************************************************************************************/
/*! \brief          Resets the paged-buffer.
 *  \details        -
 *  \param[in]      pPagedBufferContext    Pointer to the paged-buffer context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_PagedBufferReset(
  Dcm_PagedBufferContextPtrType pPagedBufferContext
  );

/**********************************************************************************************************************
 *  Dcm_PagedBufferDefragmentPage()
 *********************************************************************************************************************/
/*! \brief          Defragments the current paged-buffer page.
 *  \details        -
 *  \param[in]      pPagedBufferContext    Pointer to the paged-buffer context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_PagedBufferDefragmentPage(
  Dcm_PagedBufferContextPtrType pPagedBufferContext
  );

/**********************************************************************************************************************
 *  Dcm_PagedBufferUpdatePage()
 *********************************************************************************************************************/
/*! \brief          Dispatches the UpdatePage call.
 *  \details        Calls the service specific page updater function.
 *  \param[in]      pContext               Pointer to the context
 *  \param[in]      pPagedBufferContext    Pointer to the paged-buffer context
 *  \param[out]     ErrorCode              The NRC
 *  \return         DCM_E_OK               All data has been copied - evaluate the out parameters
 *  \return         DCM_E_PENDING          No result yet, retry later
 *  \return         DCM_E_NOT_OK           Something went wrong, stop updating(only in DET situation)
 *  \return         DCM_E_BUFFERTOOLOW     Some data copied but no place, retry later on TP buffer under-run
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferUpdatePage(
  Dcm_ContextPtrType pContext,
  Dcm_PagedBufferContextPtrType pPagedBufferContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_PagedBufferExecute()
 *********************************************************************************************************************/
/*! \brief          Starts polling a paged-buffer update function.
 *  \details        -
 *  \param[in]      pContext  Pointer to the context
 *  \param[in]      pPagedBufferContext   Pointer to the paged-buffer context
 *  \param[in,out]  pDataContext          The pointer to the data context
 *  \param[out]     ErrorCode             The NRC
 *  \return         DCM_E_OK              All data has been copied - evaluate the out parameters
 *  \return         DCM_E_PENDING         No result yet, retry later
 *  \return         DCM_E_PENDING_LIMITER Runtime limit reached, retry later
 *  \return         DCM_E_NOT_OK          Something went wrong, stop updating (only in DET situation)
 *  \return         DCM_E_BUFFERTOOLOW    Some data copied but no place, retry later on TP buffer under-run
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferExecute(
  Dcm_ContextPtrType pContext,
  Dcm_PagedBufferContextPtrType pPagedBufferContext,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Thread context of paged-buffer unit */
DCM_LOCAL VAR(Dcm_PagedBufferContextType, DCM_VAR_NOINIT) Dcm_PagedBufferContext[DCM_NUM_THREADS];                                                   /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_PagedBufferGetContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_PagedBufferContextPtrType, DCM_CODE) Dcm_PagedBufferGetContext(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_PagedBufferContextPtrType pPagedBufferContext;

  if (Dcm_DebugDetectRuntimeError(threadId >= DCM_NUM_THREADS))                                                                                      /* COV_DCM_RTM_UNIT_TEST XF */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pPagedBufferContext = &Dcm_PagedBufferContext[0];
  }
  else
  {
    pPagedBufferContext = &Dcm_PagedBufferContext[threadId];
  }

  return pPagedBufferContext;
}

/**********************************************************************************************************************
 *  Dcm_PagedBufferReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_PagedBufferReset(
  Dcm_PagedBufferContextPtrType pPagedBufferContext
  )
{
  pPagedBufferContext->State = DCM_PAGEDBUFFER_STATE_INACTIVE;                                                                                       /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
}

/**********************************************************************************************************************
 *  Dcm_PagedBufferDefragmentPage()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_PagedBufferDefragmentPage(
  Dcm_PagedBufferContextPtrType pPagedBufferContext
  )
{
  Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;
  Dcm_CfgNetBufferSizeOptType   lPageSize;

  lBufferInfo = Dcm_NetPbCfgGetBufferInfo(pPagedBufferContext->BufferHdl);
  lPageSize = (Dcm_CfgNetBufferSizeOptType)(pPagedBufferContext->WriteIndex)
            - (Dcm_CfgNetBufferSizeOptType)(pPagedBufferContext->ReadIndex);

  Dcm_UtiMemCopySafe(&(lBufferInfo->BufferPtr[pPagedBufferContext->ReadIndex])
                    ,lBufferInfo->BufferPtr
                    ,0u
                    ,lBufferInfo->Size
                    ,(Dcm_MsgLenType)lPageSize);                                                                                                     /* SBSW_DCM_POINTER_WRITE_BUFFERINFO */

  /* Enter critical section (Reason: Protect against hi-priority task /ISR CopyTxData) */
  Dcm_UtiEnterCriticalSection();
  /* Update Read- and Write-Index */
  pPagedBufferContext->ReadIndex  = 0u;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
  pPagedBufferContext->WriteIndex = (Dcm_CfgNetBufferSizeMemType)lPageSize;                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
  /* Leave critical section */
  Dcm_UtiLeaveCriticalSection();
}

/**********************************************************************************************************************
 *  Dcm_PagedBufferUpdatePage()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferUpdatePage(                                                                           /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,
  Dcm_PagedBufferContextPtrType pPagedBufferContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                lStdResult;
  Dcm_DiagDataContextType       lDataContext;
  Dcm_PbCfgNetBufferInfoPtrType lBufferInfo;
  uint8                         lUpdaterDataProvisionState;

  lBufferInfo = Dcm_NetPbCfgGetBufferInfo(pPagedBufferContext->BufferHdl);

  Dcm_DebugAssert((pPagedBufferContext->State != DCM_PAGEDBUFFER_STATE_INACTIVE), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                            /* COV_DCM_RTM_DEV_DEBUG TX */

  /* If defragmentation needed: */
  if ((pPagedBufferContext->UpdaterDataProvisionState == DCM_PAGEDBUFFER_WAIT_FOR_DATA)
    &&(pPagedBufferContext->ReadIndex != 0u) )
  {
    Dcm_PagedBufferDefragmentPage(pPagedBufferContext);                                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* Derive new data context for the remaining data to be read */
  Dcm_UtiInitDataContext(&lDataContext, lBufferInfo->BufferPtr, lBufferInfo->Size);                                                                  /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */
  Dcm_UtiCommitData(&lDataContext, pPagedBufferContext->WriteIndex);                                                                                 /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */

  /* Stronger limitation of the available buffer size - cut to the response length */
  if(lDataContext.AvailLen > pPagedBufferContext->RemainingLen)
  {
    Dcm_UtiDataContextSetAvailLen(&lDataContext, pPagedBufferContext->RemainingLen);                                                                 /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */
  }

  /* Delegate to the page writer */
  lStdResult = Dcm_PagedBufferExecute(pContext, pPagedBufferContext, &lDataContext, ErrorCode);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  switch(lStdResult)
  {
    case DCM_E_OK:
    case DCM_E_NOT_OK:
      pPagedBufferContext->UpdateFinished = TRUE;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_ALL_DATA_PROVIDED;
      break;
    case DCM_E_DATA_READY_PADDING:
      pPagedBufferContext->OpStatus = DCM_INITIAL;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Delegate to the padding writer */
      pPagedBufferContext->UpdateFuncRef = DCM_PAGEDBUFFER_PADDING_FUNC_REF;                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Application is done, now call the padding-bytes provider */
      pPagedBufferContext->UpdateFinished = TRUE;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED;
      break;
    case DCM_E_PENDING:
      pPagedBufferContext->OpStatus = DCM_PENDING;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Data could also be written although DCM_E_PENDING was returned */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED;
      break;
    case DCM_E_PENDING_LIMITER:
      /* Keep the last opStatus on "pending" forced by Dcm_PagedBufferExecute code */
      /* Data could also be written although DCM_E_PENDING was returned */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED;
      lStdResult = DCM_E_PENDING;
      break;
    case DCM_E_BUFFERTOOLOW:
      pPagedBufferContext->OpStatus = DCM_PENDING;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED;
      break;
    default:/* Unexpected return value */                                                                                                            /* COV_DCM_RTM_UNREACHABLE X */
      lStdResult = DCM_E_NOT_OK; /* Assure only valid values from here on */
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      pPagedBufferContext->UpdateFinished = TRUE;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
      lUpdaterDataProvisionState = DCM_PAGEDBUFFER_ALL_DATA_PROVIDED;
      break;
  }

  if(lStdResult == DCM_E_NOT_OK)
  {
    if(pPagedBufferContext->State == DCM_PAGEDBUFFER_STATE_ACTIVATED)
    {
      /* Reset paged-buffer usage and delegate to the linear buffer processor */
      pPagedBufferContext->State = DCM_PAGEDBUFFER_STATE_CANCELED;                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      /* Wait for under-run transmission interruption (simulate successful update with zero data size) */
      lStdResult = DCM_E_OK;
    }
    pPagedBufferContext->UpdaterDataProvisionState = lUpdaterDataProvisionState;                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
  {
    /* Enter critical section (Reason: Protect against hi-priority task /ISR CopyTxData) */
    Dcm_UtiEnterCriticalSection();
    /* Update statistic */
    pPagedBufferContext->RemainingLen -= (Dcm_MsgLenType)(lDataContext.Usage - pPagedBufferContext->WriteIndex);                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    pPagedBufferContext->WriteIndex = (Dcm_CfgNetBufferSizeMemType)lDataContext.Usage;                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Unlock CopyTx Data if more data is expected to be provided by a paged-buffer updater */
    pPagedBufferContext->UpdaterDataProvisionState = lUpdaterDataProvisionState;                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Leave critical section */
    Dcm_UtiLeaveCriticalSection();

    /* If transmission not yet started and some data is already available for transmission: */
    if ((pPagedBufferContext->State == DCM_PAGEDBUFFER_STATE_ACTIVATED)
      && (lStdResult != DCM_E_PENDING))
    {
      Dcm_DiagProcessorStateType lDiagState;

      /* Activate transmission */
      pPagedBufferContext->State = DCM_PAGEDBUFFER_STATE_ONTX;                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */

      lDiagState = Dcm_DiagGetProcessorState(pContext->ThreadId);
      lDiagState |= DCM_DIAG_STATE_ON_TX;
      Dcm_DiagSetProcessorState(pContext->ThreadId, lDiagState); /* update diagnostic state (PROCESS && ON_TX) */

      Dcm_TskSetEventByThread(DCM_TSK_ID_DIAG_TX, DCM_TSK_EV_DIAG_TX_SEND_PAGED, pContext->ThreadId);
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_PagedBufferExecute()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferExecute(
  Dcm_ContextPtrType pContext,
  Dcm_PagedBufferContextPtrType pPagedBufferContext,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;                                                                                                          /* PRQA S 2981 */ /* MD_Dcm_RetVal */

  Dcm_UtiResetRuntimeLimiter();

  DCM_UTI_LOOP_FOR_EVER                                                                                                                              /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
  {
    Dcm_DiagSvcUpdateFuncType pUpdateFunc;

    pUpdateFunc = Dcm_CfgWrapDiagGetUpdateFunc(pPagedBufferContext->UpdateFuncRef);

    /* Delegate to the page writer */
    lStdResult = pUpdateFunc(pContext
                            ,pPagedBufferContext->OpStatus
                            ,pDataContext
                            ,ErrorCode);                                                                                                             /* SBSW_DCM_CALL_FUNCPTR_SVCUPDATERS */

    if(lStdResult != DCM_E_LOOP)
    {
      break;
    }/* else - just loop again */

    pPagedBufferContext->OpStatus = DCM_INITIAL;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */

    if (Dcm_UtiRuntimeLimitReached() == TRUE)
    {
      lStdResult = DCM_E_PENDING_LIMITER;
      break;
    }
  }

  return lStdResult;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_PagedBufferInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_PagedBufferInit(
  void
  )
{
  Dcm_ThreadIdOptType lThreadIter;

  for(lThreadIter = 0; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
  {
    Dcm_PagedBufferContextPtrType pPagedBufferContext = Dcm_PagedBufferGetContext(lThreadIter);

    Dcm_PagedBufferReset(pPagedBufferContext);                                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}

/**********************************************************************************************************************
 *  Dcm_PagedBufferGetState()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_PagedBufferStateType, DCM_CODE) Dcm_PagedBufferGetState(
  Dcm_ThreadIdOptType threadId
  )
{
  return Dcm_PagedBufferGetContext(threadId)->State;
}

/**********************************************************************************************************************
 *  Dcm_PagedBufferStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_PagedBufferStart(
  Dcm_ThreadIdOptType threadId,                                                                                                                      /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_MsgLenType resDataLen
  )
{
  Dcm_MsgContextPtrType pMsgContext = Dcm_DiagGetMsgContext(threadId);
  Dcm_PagedBufferContextPtrType pPagedBufferContext = Dcm_PagedBufferGetContext(threadId);

  /* Once multiple multi frame response started, can not be started again */
  Dcm_DebugAssert((pPagedBufferContext->State == DCM_PAGEDBUFFER_STATE_INACTIVE), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                            /* COV_DCM_RTM_DEV_DEBUG TX */

  /*  Initialize paged-buffer response transmission */
  pPagedBufferContext->UpdateFuncRef = Dcm_DiagGetServiceInfo(threadId)->UpdateFuncRef;                                                              /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
  pPagedBufferContext->CancelFuncRef = Dcm_DiagGetServiceInfo(threadId)->CancelFuncRef;                                                              /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
  pPagedBufferContext->BufferHdl = Dcm_DiagGetTranspObj(threadId)->BufferHdl;                                                                        /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
  pPagedBufferContext->WriteIndex = (Dcm_CfgNetBufferSizeMemType)(pMsgContext->resDataLen                                                            /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
                                  + pMsgContext->resProtHeadLen); /* Write behind SID and the prepared data */
  pPagedBufferContext->ReadIndex = 0u;                                                                                                               /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
  pPagedBufferContext->RemainingLen = resDataLen; /* Keep track of response data to be copied */                                                     /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
  pPagedBufferContext->State = DCM_PAGEDBUFFER_STATE_ACTIVATED;                                                                                      /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
  pPagedBufferContext->UpdateFinished = FALSE;                                                                                                       /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
  pPagedBufferContext->UpdaterDataProvisionState = DCM_PAGEDBUFFER_WAIT_FOR_DATA;                                                                    /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
  pPagedBufferContext->OpStatus = DCM_INITIAL;                                                                                                       /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */

  /*  Calculate total response length, considering any already linearly written data */
  pMsgContext->resDataLen += (Dcm_MsgLenType)resDataLen;                                                                                             /* SBSW_DCM_POINTER_WRITE_MESSAGE_CONTEXT */

  /*  Initiate paged-data reader */
  Dcm_TskSetEventByThread(DCM_TSK_ID_PGDBUF, DCM_TSK_EV_PGDBUF_UPDATE_PAGE, threadId);
}

/**********************************************************************************************************************
 *  Dcm_PagedBufferCancel()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_PagedBufferCancel(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                lResult = DCM_E_OK;
  Dcm_PagedBufferContextPtrType pPagedBufferContext = Dcm_PagedBufferGetContext(pContext->ThreadId);

  /* If page buffer is currently activated: */
  if(pPagedBufferContext->State != DCM_PAGEDBUFFER_STATE_INACTIVE)
  {
    /* If not all data was written by the application: */
    if(pPagedBufferContext->UpdateFinished == FALSE)
    {
      /* Notify the application about the cancellation */
      Dcm_DiagSvcCancelFuncType lCancelFunc = Dcm_CfgWrapDiagGetCancelFunc(pPagedBufferContext->CancelFuncRef);

      lCancelFunc(pContext);                                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_CALL_FUNCPTR_SVCCANCELLERS */
    }

    /* If no transmission was started yet: */
    if(pPagedBufferContext->State == DCM_PAGEDBUFFER_STATE_ACTIVATED)                                                                                /* COV_DCM_RTM_DESIGN_LOWER_LAYER XF */
    {
      /* Send the appropriate NRC by using the linear buffer if needed */
      *ErrorCode = DCM_DIAG_CANCEL_OP_NRC;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = DCM_E_NOT_OK;
    }

    /* Reset page buffer state */
    Dcm_PagedBufferReset(pPagedBufferContext);                                                                                                       /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
    /* Clear any pending events in the Dcm_PagedBufferTask() to avoid any further processing */
    Dcm_TskClrEventByThread(DCM_TSK_ID_PGDBUF, DCM_TSK_EV_PGDBUF_UPDATE_PAGE, pContext->ThreadId);
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_PagedBufferCopyData()
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
 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_PagedBufferCopyData(
  Dcm_ThreadIdOptType threadId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) info,                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) availableDataPtr                                                                                    /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  BufReq_ReturnType             lResult = BUFREQ_OK;
  Dcm_PagedBufferContextPtrType pPagedBufferContext = Dcm_PagedBufferGetContext(threadId);

  Dcm_DebugAssert((pPagedBufferContext->State != DCM_PAGEDBUFFER_STATE_INACTIVE), DCM_SID_COPYTXDATA
                  ,DCM_E_ILLEGAL_STATE);                                                                                                             /* COV_DCM_RTM_DEV_DEBUG TX */

  *availableDataPtr = (PduLengthType)(pPagedBufferContext->WriteIndex)
                                      - (PduLengthType)(pPagedBufferContext->ReadIndex); /* Calculate and return page size */                        /* SBSW_DCM_PARAM_PTR_WRITE */

  if( (*availableDataPtr == 0u) /* no data at all */
    ||(*availableDataPtr < info->SduLength) ) /* real under-run */
  {
    if(info->SduLength != 0u)
    {
      lResult = BUFREQ_E_BUSY;
    }/* else - just buffer size check */

    if ((pPagedBufferContext->UpdaterDataProvisionState == DCM_PAGEDBUFFER_PAGE_DATA_PROVIDED)
      &&(pPagedBufferContext->RemainingLen != 0u) )
    {
      pPagedBufferContext->UpdaterDataProvisionState = DCM_PAGEDBUFFER_WAIT_FOR_DATA;                                                                /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
      Dcm_TskSetEventByThread(DCM_TSK_ID_PGDBUF, DCM_TSK_EV_PGDBUF_UPDATE_PAGE, threadId);/* re-activate page updater */                             /* PRQA S 2840 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
    }/* else - still waiting for the data or all data provided */
  }
  else /* (lPageSize != 0u) && (lPageSize >= info->SduLength) => <check buffer availability> */
  {
    if ((pPagedBufferContext->UpdaterDataProvisionState == DCM_PAGEDBUFFER_WAIT_FOR_DATA) /* still waiting for data OR data is currently being written */
      &&(pPagedBufferContext->ReadIndex != 0u) /* with needed defragmentation */
      &&(info->SduLength != 0u) )/* no buffer availability check request */
    {
      lResult = BUFREQ_E_BUSY;/* avoid coping data during defragmentation of the paged-buffer (i.e. in case of adaptive CopyTxData call with reducing SduLength each time, or when SduLength == 0, and immediately called again with SduLength != 0 */
    }
    else
    {
      Dcm_PbCfgNetBufferInfoPtrType lBufferInfo = Dcm_NetPbCfgGetBufferInfo(pPagedBufferContext->BufferHdl);

      *availableDataPtr -= info->SduLength; /* Update remained data part */                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Take the linear case always */
      Dcm_UtiMemCopySafe(&(lBufferInfo->BufferPtr[pPagedBufferContext->ReadIndex])
                        ,info->SduDataPtr
                        ,0u
                        ,info->SduLength
                        ,info->SduLength);                                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
      pPagedBufferContext->ReadIndex += (Dcm_CfgNetBufferSizeMemType)(info->SduLength);                                                              /* SBSW_DCM_POINTER_WRITE_PAGEDBUFFERCONTEXT */
    }
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_PagedBufferTask()
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
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_PagedBufferTask(                                                                                                            /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  )
{
  Std_ReturnType               lStdResult;
  Dcm_NegativeResponseCodeType lNrc = DCM_E_PANIC_NRC; /* to avoid compiler warnings */
  Dcm_PagedBufferContextPtrType pPagedBufferContext = Dcm_PagedBufferGetContext(pContext->ThreadId);

  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
    *=================================*/
  /* If any diagnostic job is open: */
  if( ((Dcm_DiagGetProcessorState(pContext->ThreadId) & (DCM_DIAG_STATE_PROCESS | DCM_DIAG_STATE_ON_TX)) != 0u)
    && (!Dcm_NetBufferIsFree(pPagedBufferContext->BufferHdl)))                                                                                       /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
  {
    /* Avoid buffer releasing on asynchronous TxConfirmation with failed status */
    Dcm_NetBufferUsageEnter(pPagedBufferContext->BufferHdl);
    /*=================================*
      END CRITICAL SECTION
      *=================================*/
    Dcm_UtiLeaveCriticalSection();

    /* Update paged data */
    lStdResult = Dcm_PagedBufferUpdatePage(pContext, pPagedBufferContext, &lNrc);                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */

    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
      *=================================*/
    Dcm_NetBufferUsageLeave(pPagedBufferContext->BufferHdl);

    /* If the application needs more time: */
    if(lStdResult == DCM_E_PENDING)
    {
      /* If the paged-buffer was not the last user, try getting data again */
      if(!Dcm_NetBufferIsFree(pPagedBufferContext->BufferHdl))
      {
        Dcm_TskSetLocalEvent(pEventContext->PostEv, DCM_TSK_EV_PGDBUF_UPDATE_PAGE);                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      }/* else - buffer is free - no more data is to be expected */
    }/* else - that is all */
    /*=================================*
      END CRITICAL SECTION
      *=================================*/
    Dcm_UtiLeaveCriticalSection();

    if (lStdResult == DCM_E_NOT_OK)
    {
      Dcm_DiagSetNegResponse(lNrc, pContext->ThreadId);
      Dcm_DiagProcessingDone(pContext->ThreadId);
    }
  }
  else
  {
    /*=================================*
    END CRITICAL SECTION
    *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_PagedBufferDataPadding()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_PagedBufferDataPadding(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);
  DCM_IGNORE_UNREF_PARAM(ErrorCode);
  DCM_IGNORE_UNREF_PARAM(opStatus);

  /* Check for remaining parts to be padded */
  Dcm_UtiMemSetUintX(Dcm_UtiGetDataContextBuffer(pDataContext), Dcm_MsgItemType, 0x00u, pDataContext->AvailLen);                                     /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */
  /* Signalize that all requested data are provided */
  Dcm_UtiCommitData(pDataContext, pDataContext->AvailLen);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
  /* Enforce transmission of the current part */
  return DCM_E_BUFFERTOOLOW;
}
# define DCM_STOP_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_PAGED_BUFFER_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_PagedBuffer.c
 *********************************************************************************************************************/
