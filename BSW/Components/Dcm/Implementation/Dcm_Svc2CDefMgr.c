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
/**        \file  Dcm_Svc2CDefMgr.c
 *         \unit  Svc2CDefMgr
 *        \brief  Contains the implementation of Service 0x2C Definition Manager unit.
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
#define DCM_SVC2CDEFMGR_SOURCE

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
#include "Dcm_Svc2CDefMgr.h"
#if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_MSR_UT_OPTIONAL_UNIT */
# include "Dcm_Debug.h"
# include "Dcm_State.h"
# include "Dcm_Uti.h"
# include "Dcm_MemMgr.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_Svc2ASchd.h"
# include "Dcm_VarMgr.h"
# include "Dcm_Net.h"
# include "Dcm_Uti.h"
# if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
#  include "NvM.h"
# endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
#  define DCM_SVC_2C_RACE_CONDITION_READ_ENABLED                     STD_ON
# else
#  define DCM_SVC_2C_RACE_CONDITION_READ_ENABLED                     STD_OFF
# endif

# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
#  error "Service 0x2C is not allowed when any paged DID is configured!"
# endif

# define DCM_SVC2C_INVALID_DYNDID_HDL                                (Dcm_UtiMaxValueOfUintType(Dcm_CfgDidMgrDynDidHandleMemType))

# define DCM_SVC_2C_PROCESS_CONTEXT_ID_SHARED                        0u

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# define Dcm_Svc2CIsDynDidDefined(dynDidHandle)                      (Dcm_Svc2CDefMgrGetItem(dynDidHandle)->Count != 0u)                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Context for the handling of DynDIDs */
struct DCM_SVC2CSINGLETONCONTEXTTYPE_TAG
{
  Dcm_Svc2CDynDidReadAccessContextType  DynDidAccessContext; /*!< Access context of the DynDID */
  Dcm_MsgLenType                        ReadIndex;           /*!< Number of bytes that are already read */
};
typedef struct DCM_SVC2CSINGLETONCONTEXTTYPE_TAG Dcm_Svc2CSingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrReadSrcItems()
 *********************************************************************************************************************/
/*! \brief          Performs reading on a single DynDID source item (DID or memory area).
 *  \details        -
 *  \param[in]      opStatus         Current DynDID read operation status.
 *  \param[in,out]  pDataContext     Pointer to the data context
 *  \param[in,out]  pProcessContext  The process context
 *  \return         DCM_E_OK         Read of all source items finished with success.
 *  \return         DCM_E_PENDING    One of the source items needs more time to return final result, call again.
 *  \return         DCM_E_NOT_OK     One of the source items returned an error.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrReadSrcItems(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrGetDDDidProcessContext()
 *********************************************************************************************************************/
/*! \brief          Return the requested process context
 *  \details        -
 *  \param[in]      processContextIndex    Index of the process context
 *  \return         The process context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_Svc2CDynDidProcessContextPtrType, DCM_CODE) Dcm_Svc2CDefMgrGetDDDidProcessContext(
  uint8 processContextIndex
  );

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01DefMgrRead()
 *********************************************************************************************************************/
/*! \brief          Reads a single DID source item.
 *  \details        -
 *  \param[in]      opStatus        Current DID sourceItem operation status.
 *  \param[in,out]  pDataContext    Pointer to the data context
 *  \param[in,out]  pProcessContext The process context
 *  \return         DCM_E_OK        Reading finished with success.
 *  \return         DCM_E_PENDING   Reading needs some additional time. Call again.
 *  \return         DCM_E_NOT_OK    Reading finished with failed.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01DefMgrRead(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext
  );
# endif

# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C02DefMgrRead()
 *********************************************************************************************************************/
/*! \brief          Reads a single Memory source item.
 *  \details        -
 *  \param[in]      opStatus        Current Memory sourceItem operation status.
 *  \param[in,out]  pDataContext    Pointer to the data context
 *  \return         DCM_E_OK            Reading finished with success.
 *  \return         DCM_E_FORCE_RCRRP   Send RCR-RP response. Once sent, call again.
 *  \return         DCM_E_PENDING       Reading needs some additional time. Call again.
 *  \return         DCM_E_NOT_OK        Reading finished with failed.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C02DefMgrRead(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrReadCheckAccessAndInit()
 *********************************************************************************************************************/
/*! \brief          Check read acces for requested DynDID.
 *  \details        -
 *  \param[in]      dynDidHandle    Current dynDid handle to be read.
 *  \param[in,out]  pOpStatus       Current and modified operation status.
 *  \return         DCM_E_OK            Check succeeded, proceed with read.
 *  \return         DCM_E_PENDING       The DynDID is still in usage, try later
 *  \return         DCM_E_NOT_OK        Check failed, leave the DynDID reading.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrReadCheckAccessAndInit(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle,
  P2VAR(Dcm_OpStatusType, AUTOMATIC, AUTOMATIC) pOpStatus
  );

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CInitDidInfoFromDidInfoIdx()
 *********************************************************************************************************************/
/*! \brief          Returns information of a specific DID.
 *  \details        Retrieve the DID information using DidInfoIdx.
 *  \param[in]      didInfoIdx        The current operation status
 *  \param[in,out]  pSrcDidContext    The source DID context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2CInitDidInfoFromDidInfoIdx(
  Dcm_CfgDidMgrDidInfoRefType didInfoIdx,
  Dcm_DidMgrDidInfoContextPtrType pSrcDidContext
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Service 0x2C Data that can be read from and written to NvM (if NvM is configured) */
VAR(Dcm_Svc2CDynDidNvMDataType, DCM_VAR_NOINIT) Dcm_Svc2CNvMData;

/*! Singleton context of service 2C unit */
DCM_LOCAL VAR(Dcm_Svc2CSingletonContextType, DCM_VAR_NOINIT) Dcm_Svc2CSingletonContext;                                                              /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrGetDDDidProcessContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_Svc2CDynDidProcessContextPtrType, DCM_CODE) Dcm_Svc2CDefMgrGetDDDidProcessContext(
  uint8 processContextIndex
  )
{
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext;

  if (Dcm_DebugDetectRuntimeError(processContextIndex >= DCM_SVC_2C_NUM_PROCESS_CONTEXTS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pProcessContext = &Dcm_Svc2CSingletonContext.DynDidAccessContext.ProcessContext[0];
  }
  else
  {
    pProcessContext = &Dcm_Svc2CSingletonContext.DynDidAccessContext.ProcessContext[processContextIndex];
  }

  return pProcessContext;
}

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C01DefMgrRead()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C01DefMgrRead(                                                                               /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext
  )
{
  Dcm_OpStatusType lOpStatus = opStatus;
  Std_ReturnType lStdResult = DCM_E_OK;

  Dcm_Svc2CDynDidSrcItemConstPtrType pDidInfo = Dcm_Svc2CDefMgrGetSrcItem(pProcessContext->ItemInProgress);

  if(lOpStatus == DCM_INITIAL)
  {
    Dcm_Svc2CInitDidInfoFromDidInfoIdx(pDidInfo->DidDescriptor.DidInfoIdx, &(pProcessContext->SrcDidContext));                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if(Dcm_RsrcMgrHasDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2C) == FALSE)
  {
    /* Check for access rights (avoids race conditions) */
    lStdResult = Dcm_RsrcMgrGetDidLock(lOpStatus, pProcessContext->SrcDidContext.Did, DCM_RSRCMGR_DIDLOCK_OWNER_SID2C);

    if(lStdResult == DCM_E_OK)
    {
      lOpStatus = DCM_INITIAL;
      Dcm_Svc2CSingletonContext.ReadIndex = 0;
    }
  }

  if(lStdResult == DCM_E_OK)
  {
    Dcm_NegativeResponseCodeType lNrc;
    Dcm_DiagDataContextType lDataContext;

    Dcm_DidMgrInitOpClassInfo(&(pProcessContext->SrcDidContext)
                             ,DCM_DIDMGR_OPTYPE_READ);                                                                                               /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */

    /* Init data context for asynchronous DIDs only initialy (not in pending status) */
    Dcm_UtiInitDataContext(&lDataContext                                                                                                             /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */
                           ,Dcm_Svc2CSingletonContext.DynDidAccessContext.Buffer
                           ,DCM_SVC_2C_READ_BUFFER_SIZE);

    Dcm_UtiCommitData(&lDataContext, Dcm_Svc2CSingletonContext.ReadIndex);                                                                           /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */

    lStdResult = Dcm_DidMgrReadDid(lOpStatus
                                  ,&lDataContext                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
                                  ,&(pProcessContext->SrcDidContext)                                                                                 /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
                                  ,&(pProcessContext->DidOpTypeContext)                                                                              /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
                                  ,&lNrc);                                                                                                           /* SBSW_DCM_POINTER_FORWARD_STACK */

    Dcm_Svc2CSingletonContext.ReadIndex = lDataContext.Usage;

    /* Finished processing a DIDSrcItem */
    if(lStdResult != DCM_E_PENDING)
    {
      if(lStdResult == DCM_E_OK)
      {
        Dcm_UtiProvideDataAsUN(pDataContext, &lDataContext.Buffer[pDidInfo->DidDescriptor.Offset], pDidInfo->DidDescriptor.Size);                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2C);
    }
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2C02DefMgrRead()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2C02DefMgrRead(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext
  )
{
  Std_ReturnType lStdResult;
  Dcm_OpStatusType lOpStatus = opStatus;

  lStdResult = DCM_E_OK;

  if(Dcm_RsrcMgrHasMemLock(DCM_RSRCMGR_MEM_ACCESS_REQ_INT) == TRUE)
  {
    /* Proceed with reading */
  }
  else
  {
    /* Still in process getting access! */
    lStdResult = Dcm_RsrcMgrGetMemLock(lOpStatus, DCM_RSRCMGR_MEM_ACCESS_REQ_INT);

    if (lStdResult == DCM_E_OK)
    {
      lOpStatus = DCM_INITIAL; /* prepare for first reading */
    }
  }

  if(lStdResult == DCM_E_OK)
  {
    Dcm_NegativeResponseCodeType lNrc;
    Dcm_CfgDidMgrDynDidSrcItemIdxMemType lItemInProgress;

    lItemInProgress = Dcm_Svc2CDefMgrGetDDDidProcessContext(DCM_SVC_2C_PROCESS_CONTEXT_ID_SHARED)->ItemInProgress;
    lStdResult = Dcm_MemMgrReadMemory(lOpStatus
                                    ,&(Dcm_Svc2CDefMgrGetSrcItem(lItemInProgress)->MemDescriptor.MemBlock)
                                    ,pDataContext
                                    ,&lNrc);                                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrReadSrcItems()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrReadSrcItems(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext
  )
{
  Std_ReturnType lStdReturn = DCM_E_OK;
  Dcm_OpStatusType lOpStatus = opStatus;

  for(; pProcessContext->ItemInProgress < pProcessContext->ItemToStop; pProcessContext->ItemInProgress++)                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  {
    if(Dcm_Svc2CDefMgrIsDidSrcItem(pProcessContext->ItemInProgress))
    {
# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
      lStdReturn = Dcm_Svc2C01DefMgrRead(lOpStatus, pDataContext, pProcessContext);                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
# endif
    }
    else
    {
# if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
      lStdReturn = Dcm_Svc2C02DefMgrRead(lOpStatus, pDataContext);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
# endif
    }

    if(lStdReturn == DCM_E_OK)
    {
      /* prepare for next item */
      lOpStatus = DCM_INITIAL;
    }
    else
    {
      break;
    }
  }

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrReadCheckAccessAndInit()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrReadCheckAccessAndInit(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle,
  P2VAR(Dcm_OpStatusType, AUTOMATIC, AUTOMATIC) pOpStatus                                                                                            /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  DCM_IGNORE_UNREF_PARAM(dynDidHandle);

  /*-----------------------------------------------*
   * DynDID usage check and initialization
   *-----------------------------------------------*/
  if(Dcm_Svc2CSingletonContext.DynDidAccessContext.DynDidHandleInUse == DCM_SVC2C_INVALID_DYNDID_HDL)
  {
    if(*pOpStatus == DCM_CANCEL)
    {
      lStdResult = DCM_E_NOT_OK; /* we have waited for too long to get access to the DynDID reader, but it was canceled -> Stop here since no reading has been started yet */
    }
    else
    {
      /*
       * Reset opStatus in case some DCM_E_PENDING had to be returned due to concurrent access (i.e. 0x22 and 0x2A for
       * different DDDIDs)
       */
      *pOpStatus = DCM_INITIAL;                                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
# if (DCM_SVC_2C_RACE_CONDITION_READ_ENABLED == STD_ON)
  else
  {
    if(dynDidHandle != Dcm_Svc2CSingletonContext.DynDidAccessContext.DynDidHandleInUse)
    {
      lStdResult = DCM_E_PENDING;
    } /* else - same handle -> process it */
  }
# endif
  return lStdResult;
}

# if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CInitDidInfoFromDidInfoIdx()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2CInitDidInfoFromDidInfoIdx(
  Dcm_CfgDidMgrDidInfoRefType didInfoIdx,
  Dcm_DidMgrDidInfoContextPtrType pSrcDidContext
  )
{
  pSrcDidContext->Did = Dcm_CfgWrapDidMgrDidLookUpTable(didInfoIdx + 1u);                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  (void)Dcm_DidMgrConcreteDidLookUp(pSrcDidContext, DCM_DIDMGR_OP_READ);                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrInit()
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
FUNC(void, DCM_CODE) Dcm_Svc2CDefMgrInit(
  void
  )
{
  Dcm_CfgDidMgrDynDidHandleOptType dynDidIter;

  /* If DynDIDs were restored from non-volatile memory: */
# if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
  if((Dcm_Svc2CNvMData.MagicNumber == DCM_CFG_FINAL_MAGIC_NUMBER)
#  if (DCM_VARMGR_MULTI_SVC_EXCLUSIVE_ENABLED == STD_ON)
   &&(Dcm_Svc2CNvMData.CfgVariantId == Dcm_VarMgrGetActiveCfgVariantId())                                                                            /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
#  endif
    )
  {
    /* Check if preconditions are still fulfilled */
#  if (DCM_DIDMGR_DYNDID_CLR_ON_STATE_CHG_ENABLED == STD_ON) && \
      (DCM_DIAG_STATE_RECOVERY_ENABLED            == STD_OFF)
    /*
     * In case that state recovery feature is enabled, the DynDID preconditions shall be checked after
     * state recovery.
     * In case of jump from FBL (warm start) just clear all DynDIDs which are not supported
     * in default session (KISS principle).
     * After TX confirmation of the unsolicited response, Dcm_Svc2CDefMgrOnStateChanged() will be
     * called again by Dcm_StateSetSession().
     * Please note, that it was always not allowed to use state recovery and warm start at the same time.
     */
    Dcm_Svc2CDefMgrOnStateChanged();
#  endif
  }
  else
# endif
  {
    for(dynDidIter = 0; dynDidIter < DCM_NUM_DYNDIDS; ++dynDidIter)
    {
      Dcm_Svc2CDynDidItemPtrType pItem;

      pItem = Dcm_Svc2CDefMgrGetItem(dynDidIter);
      pItem->Length = 0u;                                                                                                                            /* SBSW_DCM_POINTER_WRITE_2CITEM */
      pItem->Count  = 0u;                                                                                                                            /* SBSW_DCM_POINTER_WRITE_2CITEM */
    }

# if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
    Dcm_Svc2CNvMData.MagicNumber = DCM_CFG_FINAL_MAGIC_NUMBER;
#  if (DCM_VARMGR_MULTI_SVC_EXCLUSIVE_ENABLED == STD_ON)
    Dcm_Svc2CNvMData.CfgVariantId = Dcm_VarMgrGetActiveCfgVariantId();
#  endif
# endif
  }

  Dcm_Svc2CSingletonContext.DynDidAccessContext.DynDidHandleInUse = DCM_SVC2C_INVALID_DYNDID_HDL;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrGetSrcItem()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_Svc2CDynDidSrcItemPtrType, DCM_CODE) Dcm_Svc2CDefMgrGetSrcItem(
  Dcm_CfgDidMgrDynDidSrcItemIdxOptType srcItemIndex
  )
{
  Dcm_Svc2CDynDidSrcItemPtrType pSourceItem;

  if (Dcm_DebugDetectRuntimeError(srcItemIndex >= DCM_NUM_DYNDID_ITEMS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pSourceItem = &Dcm_Svc2CNvMData.SrcItems[0];
  }
  else
  {
    pSourceItem = &Dcm_Svc2CNvMData.SrcItems[srcItemIndex];
  }

  return pSourceItem;
}

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrGetItem()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_Svc2CDynDidItemPtrType, DCM_CODE) Dcm_Svc2CDefMgrGetItem(
  Dcm_CfgDidMgrDynDidHandleOptType itemIndex
  )
{
  Dcm_Svc2CDynDidItemPtrType pItem;

  if (Dcm_DebugDetectRuntimeError(itemIndex >= DCM_NUM_DYNDIDS))
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pItem = &Dcm_Svc2CNvMData.Items[0];
  }
  else
  {
    pItem = &Dcm_Svc2CNvMData.Items[itemIndex];
  }

  return pItem;
}

# if (DCM_DIDMGR_DYNDID_SRCITEM_CHECK_STATE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrStateCheckSrcItems()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrStateCheckSrcItems(
  Dcm_NetConnRefMemType connHdl,
  Dcm_CfgDidMgrDynDidHandleOptType dynDidHandle,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  Dcm_CfgDidMgrDynDidSrcItemIdxOptType currItem;
  Dcm_CfgDidMgrDynDidSrcItemIdxOptType endItem;

  DCM_IGNORE_UNREF_PARAM(connHdl);

  currItem = Dcm_DidMgrGetDynDidSrcItemStartRef(dynDidHandle);
  endItem = (Dcm_CfgDidMgrDynDidSrcItemIdxOptType)(currItem + Dcm_Svc2CDefMgrGetItem(dynDidHandle)->Count);

  Dcm_DebugAssert((currItem < endItem), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                                      /* COV_DCM_RTM_DEV_DEBUG TX */

  for(; currItem < endItem; currItem++)
  {
    if(Dcm_Svc2CDefMgrIsDidSrcItem(currItem))
    {
#  if (DCM_SVC_2C_01_SUPPORT_ENABLED == STD_ON)
      Dcm_DidMgrDidInfoContextType lDidInfoContext;

      Dcm_Svc2CInitDidInfoFromDidInfoIdx(Dcm_Svc2CDefMgrGetSrcItem(currItem)->DidDescriptor.DidInfoIdx, &lDidInfoContext);                           /* SBSW_DCM_POINTER_FORWARD_STACK */

      lStdResult = Dcm_StateCheckDID(connHdl
                                    ,lDidInfoContext.Did
                                    ,DCM_DIDMGR_OP_READ
                                    ,Dcm_CfgDidMgrGetDidOpInfo(&lDidInfoContext)->ExecCondRef                                                        /* PRQA S 2962 */ /* MD_Dcm_2962 */
                                    ,ErrorCode);                                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
#  endif
    }
    else
    {
#  if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
      lStdResult = Dcm_MemMgrCheckMemBlock(&(Dcm_Svc2CDefMgrGetSrcItem(currItem)->MemDescriptor.MemBlock)
                                          ,DCM_MEMMGR_OP_READ
                                          ,ErrorCode);                                                                                               /* SBSW_DCM_COMB_PTR_FORWARD */
#  endif
    }
    if (lStdResult != DCM_E_OK)
    {
      break;
    }
  }

  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrIsDidDefined()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_Svc2CDefMgrIsDidDefined(
  Dcm_CfgDidMgrDidInfoConstPtrType pDidInfo
  )
{
  return ((!Dcm_DidMgrIsOpSupported(pDidInfo, DCM_DIDMGR_OP_DEFINE))
          || Dcm_Svc2CIsDynDidDefined((Dcm_CfgDidMgrDynDidHandleMemType)(pDidInfo->OpRef)));                                                         /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */
}

# if (DCM_DIDMGR_DYNDID_CLR_ON_STATE_CHG_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrOnStateChanged()
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
FUNC(void, DCM_CODE) Dcm_Svc2CDefMgrOnStateChanged(
  void
  )
{
  /* clear all DynDID definitions not supported any more */
  Dcm_NegativeResponseCodeType     lNrc;
  Std_ReturnType                   lStdResult;
  Dcm_CfgDidMgrDynDidHandleOptType lDynDidIter;
#  if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
  boolean                          lAnyCleared = FALSE;
#  endif

  for (lDynDidIter = 0; lDynDidIter < DCM_NUM_DYNDIDS; ++lDynDidIter)
  {
    if (Dcm_Svc2CIsDynDidDefined(lDynDidIter))
    {
      lStdResult = Dcm_DidMgrStaticDidNoAuthStateCheck(Dcm_DidMgrGetDynDidReadExecPrecond(lDynDidIter), &lNrc);                                      /* SBSW_DCM_POINTER_FORWARD_STACK */

#  if (DCM_DIDMGR_DYNDID_SRCITEM_CHECK_STATE_ENABLED == STD_ON)
      if (lStdResult == DCM_E_OK) /* all DDDID conditions OK - check inside */
      {
        lStdResult = Dcm_Svc2CDefMgrStateCheckSrcItems(DCM_NET_INVALID_CONNHDL, lDynDidIter, &lNrc);                                                 /* SBSW_DCM_POINTER_FORWARD_STACK */
      }
#  endif

      if (lStdResult == DCM_E_NOT_OK) /* any condition not fulfilled? */
      {
        Dcm_Svc2CDefMgrClear((Dcm_CfgDidMgrDynDidHandleMemType)lDynDidIter);
#  if (DCM_SVC_2C_NVRAM_SUPPORT_ENABLED == STD_ON)
        if (lAnyCleared == FALSE)
        {
          lAnyCleared = TRUE;
          Dcm_UtiNvMSetRamBlockStatus((uint16)DCM_SVC_2C_NVRAM_BLOCKID);
        }
#  endif
      }
    } /* else - DDDID not defined yet */
  }
}
# endif

# if (DCM_SVC_2C_03_SUPPORT_ENABLED == STD_ON)                                                                                                       /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrClear()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc2CDefMgrClear(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle
  )
{
  Dcm_Svc2CDynDidItemPtrType pItem;

#  if (DCM_DIDMGR_PERIODICDYNDID_ENABLED == STD_ON)
  {
    uint16 lDid = Dcm_DidMgrGetDynDidIdFromHandle(dynDidHandle);
    /* Try stopping any active periodic DID (can be still read at that time)! */
    Dcm_Svc2ASchdStopItemByDid(lDid);
  }
#  endif

  pItem = Dcm_Svc2CDefMgrGetItem(dynDidHandle);
  pItem->Length = 0u;                                                                                                                                /* SBSW_DCM_POINTER_WRITE_2CITEM */
  pItem->Count  = 0u;                                                                                                                                /* SBSW_DCM_POINTER_WRITE_2CITEM */
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrRead()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrRead(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle,
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext
  )
{
  Std_ReturnType lStdReturn;
  Dcm_OpStatusType lOpStatus = opStatus;

  /* The DDDID shall be a defined one! */
  Dcm_DebugAssert((Dcm_Svc2CIsDynDidDefined(dynDidHandle)), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                  /* COV_DCM_RTM_DEV_DEBUG TX */

  lStdReturn = Dcm_Svc2CDefMgrReadCheckAccessAndInit(dynDidHandle, &lOpStatus);                                                                      /* SBSW_DCM_POINTER_FORWARD_STACK */

  if(lStdReturn == DCM_E_OK)
  {
    Dcm_Svc2CDynDidProcessContextPtrType pProcessContext;

    pProcessContext = Dcm_Svc2CDefMgrGetDDDidProcessContext(DCM_SVC_2C_PROCESS_CONTEXT_ID_SHARED);

    if(lOpStatus == DCM_INITIAL)
    {
      Dcm_Svc2CSingletonContext.DynDidAccessContext.DynDidHandleInUse = dynDidHandle;
      pProcessContext->ItemInProgress = Dcm_DidMgrGetDynDidSrcItemStartRef(dynDidHandle);                                                            /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
      pProcessContext->ItemToStop = (Dcm_CfgDidMgrDynDidSrcItemIdxMemType)(pProcessContext->ItemInProgress + Dcm_Svc2CDefMgrGetItem(dynDidHandle)->Count); /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
    }

    /* Process source items */
    lStdReturn = Dcm_Svc2CDefMgrReadSrcItems(lOpStatus, pDataContext, pProcessContext);                                                              /* SBSW_DCM_POINTER_FORWARD_STACK */
  }

  /* Processing finished, release resource */
  if((lStdReturn == DCM_E_OK) || (lStdReturn == DCM_E_NOT_OK))
  {
    Dcm_Svc2CSingletonContext.DynDidAccessContext.DynDidHandleInUse = DCM_SVC2C_INVALID_DYNDID_HDL;
  }
  return lStdReturn;
}

# if (DCM_DIDMGR_DYNDID_SRCITEM_CHECK_COND_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrConditionCheckRead()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrConditionCheckRead(                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle,
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType errorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK; /* if the DynDID does not contain any DID sources -> always OK */
  Dcm_Svc2CDynDidProcessContextPtrType pProcessContext;
  Dcm_OpStatusType lOpStatus = opStatus;

  /* The DDDID shall be a defined one! */
  Dcm_DebugAssert((Dcm_Svc2CIsDynDidDefined(dynDidHandle)), DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);                                                  /* COV_DCM_RTM_DEV_DEBUG TX */

  pProcessContext = Dcm_Svc2CDefMgrGetDDDidProcessContext(DCM_SVC_2C_PROCESS_CONTEXT_ID_EXT_ONLY);

  if(lOpStatus == DCM_INITIAL)
  {
    pProcessContext->ItemInProgress = Dcm_DidMgrGetDynDidSrcItemStartRef(dynDidHandle);                                                              /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
    pProcessContext->ItemToStop = (Dcm_CfgDidMgrDynDidSrcItemIdxMemType)(pProcessContext->ItemInProgress
                                                                         + Dcm_Svc2CDefMgrGetItem(dynDidHandle)->Count);                             /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
  }

  for(; pProcessContext->ItemInProgress < pProcessContext->ItemToStop; ++pProcessContext->ItemInProgress)                                            /* SBSW_DCM_PARAM_PTR_WRITE */
  {
    if(Dcm_Svc2CDefMgrIsDidSrcItem(pProcessContext->ItemInProgress))
    {
      Dcm_Svc2CInitDidInfoFromDidInfoIdx(Dcm_Svc2CDefMgrGetSrcItem(pProcessContext->ItemInProgress)->DidDescriptor.DidInfoIdx
                                         ,&(pProcessContext->SrcDidContext));                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */

      if(Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pProcessContext->SrcDidContext))                                                    /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
                                    ,DCM_DIDMGR_OPTYPE_READCHKCOND))
      {
        Dcm_DidMgrInitOpClassInfo(&(pProcessContext->SrcDidContext)                                                                                  /* SBSW_DCM_POINTER_WRITE_2CPROCESSCONTEXT */
                                 ,DCM_DIDMGR_OPTYPE_READCHKCOND);

        lStdResult = Dcm_DidMgrReadCheckCond(lOpStatus
                                            ,&(pProcessContext->SrcDidContext)
                                            ,&(pProcessContext->DidOpTypeContext)
                                            ,errorCode);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */
        if(lStdResult == DCM_E_OK)
        {
          /* prepare for next item */
          lOpStatus = DCM_INITIAL;
        }
        else
        {
          break; /* just delegate the return value */                                                                                                /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
        }
      } /* else - no check condition supported by this signal */
    } /* else - memory ranges cannot be checked in advance! */
  }
  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2CDefMgrReadDataLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CDefMgrReadDataLength(
  Dcm_CfgDidMgrDynDidHandleMemType dynDidHandle,
  Dcm_DidMgrDidLengthPtrType dataLength
  )
{
  *dataLength = Dcm_Svc2CDefMgrGetItem(dynDidHandle)->Length;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  return DCM_E_OK;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2CDefMgr.c
 *********************************************************************************************************************/
