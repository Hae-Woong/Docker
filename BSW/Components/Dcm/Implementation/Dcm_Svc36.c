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
/**        \file  Dcm_Svc36.c
 *         \unit  Svc36
 *        \brief  Contains the implementation of Service 0x36 unit.
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
#define DCM_SVC36_SOURCE

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
#include "Dcm_Svc36.h"
#if (DCM_SVC_36_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_MSR_UT_OPTIONAL_UNIT */
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_Repeater.h"
# include "Rte_Dcm.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x36 processing steps */
# define DCM_SVC36_PROGRESS_EXECUTE_DOWNLOAD                         ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC36_PROGRESS_EXECUTE_UPLOAD                           ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))

# define DCM_SVC36_MINLENGTH_DOWNLOAD                                1u /* transferRequestParameterRecord needed */
# define DCM_SVC36_MINLENGTH_UPLOAD                                  0u /* transferRequestParameterRecord not needed */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! ProcessRequestDownoad is called */
# define Dcm_Svc36ProcessTransferDataWriteWrapper                    DCM_PROCESS_TRANSFER_DATA_WRITE                                                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc36ProcessTransferDataReadWrapper                     DCM_PROCESS_TRANSFER_DATA_READ                                                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc36GetMinReqDataLen()
 *********************************************************************************************************************/
/*! \brief          Service 0x36 Get Minimum Length for request without SID + BSC.
 *  \details        -
 *  \param[in]      dataTransferState         dataTransfer state
 *  \return         Value of minimum request length without SID + BSC
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_Svc36MinReqDataLenType, DCM_CODE) Dcm_Svc36GetMinReqDataLen(
  Dcm_FblMgrDataTransferStateType dataTransferState
  );

/**********************************************************************************************************************
 *  Dcm_Svc36HandlerCheckSequenceCounter()
 *********************************************************************************************************************/
/*! \brief          Service 0x36 Check Sequence Counter.
 *  \details        -
 *  \param[in]      dataTransferState       DataTransfer state
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Check is ok
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36HandlerCheckSequenceCounter(
  Dcm_FblMgrDataTransferStateType dataTransferState,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc36Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x36 Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext
  );

# if (DCM_SVC_34_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36FinalizeDownloadProcessing()
 *********************************************************************************************************************/
/*! \brief          Finalize Processing for Service 0x36 Tranfer Downloaded Data.
 *  \details        -
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc36FinalizeDownloadProcessing(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_34_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36ProcessTransferDataWrite()
 *********************************************************************************************************************/
/*! \brief          Service 0x36 Tranfer Data.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_OK                Send positive response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_FORCE_RCRRP       Enforce a RCR-RP transmission
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36ProcessTransferDataWrite(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36DetermineBlockToRead()
 *********************************************************************************************************************/
/*! \brief          Determine if previous block has to restored or the next block shall be read.
 *  \details        -
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc36DetermineBlockToRead(
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  );
# endif

# if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36GetBlockSizeToRead()
 *********************************************************************************************************************/
/*! \brief          Get size of memory block which shall be read.
 *  \details        Check if remaining data is less than BlockLength and return BlockLength to read.
 *  \return         Size of BlockLength to read
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_FblMgrBlockLengthType, DCM_CODE) Dcm_Svc36GetBlockSizeToRead(
  void
  );
# endif

# if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36FinalizeUploadProcessing()
 *********************************************************************************************************************/
/*! \brief          Finalize Processing for Service 0x36 Tranfer Uploaded Data.
 *  \details        -
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \param[in]      blockLength             Amount of uploaded data
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc36FinalizeUploadProcessing(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext,
  Dcm_FblMgrBlockLengthType blockLength
  );
# endif

# if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36ProcessTransferDataRead()
 *********************************************************************************************************************/
/*! \brief          Service 0x36 Tranfer Data.
 *  \details        -
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_OK                Send positive response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_FORCE_RCRRP       Enforce a RCR-RP transmission
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36ProcessTransferDataRead(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext
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
 *  Dcm_Svc36GetMinReqDataLen()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_Svc36MinReqDataLenType, DCM_CODE) Dcm_Svc36GetMinReqDataLen(
  Dcm_FblMgrDataTransferStateType dataTransferState
  )
{
  Dcm_Svc36MinReqDataLenType retVal;

  if ((dataTransferState == DCM_FBL_DOWNLOAD_STATE_READY_FOR_TRANSFER)
      || (dataTransferState == DCM_FBL_DOWNLOAD_STATE_TRANSFER_IN_PROGRESS))
  {
    retVal = DCM_SVC36_MINLENGTH_DOWNLOAD;
  }
  else
  {
    retVal = DCM_SVC36_MINLENGTH_UPLOAD;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  Dcm_Svc36HandlerCheckSequenceCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36HandlerCheckSequenceCounter(
  Dcm_FblMgrDataTransferStateType dataTransferState,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  if ((dataTransferState == DCM_FBL_DOWNLOAD_STATE_READY_FOR_TRANSFER)
    || (dataTransferState == DCM_FBL_UPLOAD_STATE_READY_FOR_TRANSFER))
  {
    if (pRepContext->BlockSequenceCounterNew == 1u)
    {
      lStdResult = DCM_E_OK;
    }
    else
    {
      *ErrorCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  else
  {
    Dcm_FblMgrBlockSequenceCounterType lBlockSequenceCounterOld = Dcm_FblMgrGetBlockSequenceCounter();

    if ((pRepContext->BlockSequenceCounterNew == lBlockSequenceCounterOld) /* a frame positive response is not received correctly by client */
      || (pRepContext->BlockSequenceCounterNew == ((Dcm_FblMgrBlockSequenceCounterType)(lBlockSequenceCounterOld + 1u)))) /* considers also wrap around case */
    {
      lStdResult = DCM_E_OK;
    }
    else
    {
      *ErrorCode = DCM_E_WRONGBLOCKSEQUENCECOUNTER;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc36Handler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  Dcm_FblMgrDataTransferStateType lDataTransferState;

  DCM_IGNORE_UNREF_PARAM(opStatus);
  DCM_IGNORE_UNREF_PARAM(pRepContext);

  lStdResult = DCM_E_NOT_OK;

  /* dispatcher guarantees for minimum length of 1 byte (blockSequenceCounter) */
  Dcm_UtiConsumeReqDataAsU8(pMsgContext, &pRepContext->BlockSequenceCounterNew);                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */

  lDataTransferState = Dcm_FblMgrGetDataTransferState();

  if ((lDataTransferState == DCM_FBL_DOWNLOAD_STATE_READY_FOR_TRANSFER)
    || (lDataTransferState == DCM_FBL_DOWNLOAD_STATE_TRANSFER_IN_PROGRESS)
    || (lDataTransferState == DCM_FBL_UPLOAD_STATE_READY_FOR_TRANSFER)
    || (lDataTransferState == DCM_FBL_UPLOAD_STATE_TRANSFER_IN_PROGRESS))
  {
    Dcm_Svc36MinReqDataLenType lMinReqDataLen = Dcm_Svc36GetMinReqDataLen(lDataTransferState);
    if (pMsgContext->reqDataLen >= lMinReqDataLen)
    {
      /* Check if block sequence is valid */
      lStdResult = Dcm_Svc36HandlerCheckSequenceCounter(lDataTransferState, ErrorCode, pRepContext);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

      if (lStdResult == DCM_E_OK)
      {
        /* Check if the block length is valid */
        if (pMsgContext->reqDataLen > Dcm_FblMgrGetMaxBlockLength())
        {
          lStdResult = DCM_E_NOT_OK;
          *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
        } /* else - DCM_E_OK */
      } /* else - DCM_E_NOT_OK - NRC already set */
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  else
  {
    *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  if (lStdResult == DCM_E_OK)
  {
    if ((lDataTransferState == DCM_FBL_DOWNLOAD_STATE_READY_FOR_TRANSFER)
      || (lDataTransferState == DCM_FBL_DOWNLOAD_STATE_TRANSFER_IN_PROGRESS))
    {
      Dcm_RepeaterNextStep(pContext, DCM_SVC36_PROGRESS_EXECUTE_DOWNLOAD);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      Dcm_RepeaterNextStep(pContext, DCM_SVC36_PROGRESS_EXECUTE_UPLOAD);                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    lStdResult = DCM_E_LOOP;/* speed up processing */
  }

  return lStdResult;
}

# if (DCM_SVC_34_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36FinalizeDownloadProcessing()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc36FinalizeDownloadProcessing(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_FblMgrSetDataTransferState(DCM_FBL_DOWNLOAD_STATE_TRANSFER_IN_PROGRESS);

  Dcm_FblMgrSetBlockSequenceCounter(pRepContext->BlockSequenceCounterNew);
  Dcm_FblMgrGetMemoryBlock()->Address = Dcm_FblMgrGetMemoryBlock()->Address + pMsgContext->reqDataLen;                                               /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_UtiProvideResDataAsU8(pMsgContext, pRepContext->BlockSequenceCounterNew);                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif

# if (DCM_SVC_34_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36ProcessTransferDataWrite()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36ProcessTransferDataWrite(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_ReturnWriteMemoryType opResult;
  Std_ReturnType  lStdResult = DCM_E_OK;
  boolean lUnknown = FALSE;

  DCM_IGNORE_UNREF_PARAM(pRepContext);

  /* If a TransferData request to download data is correctly received and processed in the server but the positive
  * response message does not reach the client.
  */
  if (pRepContext->BlockSequenceCounterNew == Dcm_FblMgrGetBlockSequenceCounter())
  {
    Dcm_UtiProvideResDataAsU8(pMsgContext, Dcm_FblMgrGetBlockSequenceCounter());                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    opResult = Dcm_Svc36ProcessTransferDataWriteWrapper(opStatus
                                                       ,Dcm_FblMgrGetMemoryBlock()->Mid
                                                       ,Dcm_FblMgrGetMemoryBlock()->Address
                                                       ,pMsgContext->reqDataLen
                                                       ,Dcm_UtiGetReqData(pMsgContext)                                                               /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                       ,ErrorCode);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_UtiConvMemoryOpResult(opResult, opStatus, &lUnknown, ErrorCode);                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lUnknown == TRUE)
    {
      *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_DebugReportError(DCM_SID_PROCESSTRANSFERDATAWRITE, DCM_E_INTERFACE_RETURN_VALUE);
    }

    if (lStdResult == DCM_E_OK)
    {
      Dcm_Svc36FinalizeDownloadProcessing(pMsgContext, pRepContext);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36DetermineBlockToRead()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc36DetermineBlockToRead(
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  if (pRepContext->BlockSequenceCounterNew == Dcm_FblMgrGetBlockSequenceCounter())
  {
    Dcm_FblMgrBlockLengthType lPreviousBlockLength = ((Dcm_FblMgrGetMemoryBlock()->Address) - (Dcm_FblMgrGetPreviousMemoryBlock()->Address));
    Dcm_FblMgrSetRemainingBlockLength(Dcm_FblMgrGetRemainingBlockLength() + lPreviousBlockLength);

    Dcm_FblMgrSetMemoryBlock(Dcm_FblMgrGetPreviousMemoryBlock());                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    Dcm_FblMgrSetPreviousMemoryBlock(Dcm_FblMgrGetMemoryBlock());                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}
# endif

# if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36GetBlockSizeToRead()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_FblMgrBlockLengthType, DCM_CODE) Dcm_Svc36GetBlockSizeToRead(
  void
  )
{
  Dcm_FblMgrBlockLengthType retVal;

  retVal = Dcm_FblMgrGetMaxBlockLength();

  if (Dcm_FblMgrGetRemainingBlockLength() < retVal)
  {
    retVal = Dcm_FblMgrGetRemainingBlockLength();
  } /* else already set */

  return retVal;
}
# endif

# if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36FinalizeUploadProcessing()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc36FinalizeUploadProcessing(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext,                                                                                                  /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_FblMgrBlockLengthType blockLength
  )
{
  Dcm_FblMgrSetDataTransferState(DCM_FBL_UPLOAD_STATE_TRANSFER_IN_PROGRESS);

  Dcm_FblMgrSetBlockSequenceCounter(pRepContext->BlockSequenceCounterNew);
  Dcm_FblMgrGetMemoryBlock()->Address = Dcm_FblMgrGetMemoryBlock()->Address + blockLength;                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
  Dcm_FblMgrSetRemainingBlockLength(Dcm_FblMgrGetRemainingBlockLength() - blockLength);

  Dcm_UtiProvideResDataAsU8(pMsgContext, pRepContext->BlockSequenceCounterNew);                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  Dcm_UtiProvideResData(pMsgContext, blockLength);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif

# if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc36ProcessTransferDataRead()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc36ProcessTransferDataRead(                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc36RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_ReturnWriteMemoryType opResult;
  Dcm_DiagDataContextType lDataContext;
  Dcm_FblMgrBlockLengthType lBlockLength;
  Std_ReturnType  lStdResult = DCM_E_OK;
  boolean lUnknown = FALSE;

  DCM_IGNORE_UNREF_PARAM(pRepContext);

  Dcm_Svc36DetermineBlockToRead(pRepContext);                                                                                                        /* SBSW_DCM_COMB_PTR_FORWARD */

  lBlockLength = Dcm_Svc36GetBlockSizeToRead();

  Dcm_UtiInitDataContext(&lDataContext
                         ,Dcm_UtiGetResDataRel(pMsgContext, 1u)                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
                         ,lBlockLength);                                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

  opResult = Dcm_Svc36ProcessTransferDataReadWrapper(opStatus
                                                    ,Dcm_FblMgrGetMemoryBlock()->Mid
                                                    ,Dcm_FblMgrGetMemoryBlock()->Address
                                                    ,lBlockLength
                                                    ,Dcm_UtiGetDataContextBuffer(&lDataContext)                                                      /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                    ,ErrorCode);                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

  lStdResult = Dcm_UtiConvMemoryOpResult(opResult, opStatus, &lUnknown, ErrorCode);                                                                  /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lUnknown == TRUE)
  {
    *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_PROCESSTRANSFERDATAREAD, DCM_E_INTERFACE_RETURN_VALUE);
  }

  if (lStdResult == DCM_E_OK)
  {
    Dcm_UtiCommitData(&lDataContext, lBlockLength);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

    Dcm_Svc36FinalizeUploadProcessing(pMsgContext, pRepContext, lBlockLength);                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  return lStdResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service36Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service36Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc36RepeaterProxyContextPtrType pRepContext = Dcm_RepeaterGetSvc36Context(pContext->ThreadId);

  switch (Dcm_RepeaterGetProgress(pContext))                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
  case DCM_REPEATER_PROGRESS_SERVICE_HANDLER_INITIAL:
    lStdResult = Dcm_Svc36Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# if (DCM_SVC_34_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC36_PROGRESS_EXECUTE_DOWNLOAD:
    lStdResult = Dcm_Svc36ProcessTransferDataWrite(opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC36_PROGRESS_EXECUTE_UPLOAD:
    lStdResult = Dcm_Svc36ProcessTransferDataRead(opStatus, pMsgContext, ErrorCode, pRepContext);                                                    /* SBSW_DCM_COMB_PTR_FORWARD */
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
# define DCM_STOP_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_36_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc36.c
 *********************************************************************************************************************/
