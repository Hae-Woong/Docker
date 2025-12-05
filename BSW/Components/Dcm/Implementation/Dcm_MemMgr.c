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
/**        \file  Dcm_MemMgr.c
 *         \unit  MemMgr
 *        \brief  Contains the implementation of Memory Manager unit.
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
#define DCM_MEMMGR_SOURCE

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
#include "Dcm_MemMgr.h"
#if (DCM_MEMMGR_SUPPORT_ENABLED == STD_ON)                                                                                                           /* COV_MSR_UT_OPTIONAL_UNIT */
# include "Dcm_Debug.h"
# include "Dcm_Diag.h"
# include "Dcm_State.h"
# include "Dcm_RsrcMgr.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

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
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_MemMgrIsOperationSupported()
 *********************************************************************************************************************/
/*! \brief          Checks whether the given range supports the given operation in the currently active variant(s).
 *  \details        -
 *  \param[in]      memOp          The requested memory operation (read or write)
 *  \param[in]      pMemMapInfo    The memory mapping table containig all relevant information
 *  \return         TRUE     The operation is supported by the given memory range
 *  \return         FALSE    The operation is not supported by the given memory range
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_MemMgrIsOperationSupported(
  Dcm_MemMgrMemoryOpType memOp,
  Dcm_CfgMemMgrMemMapInfoPtrType pMemMapInfo
  );

/**********************************************************************************************************************
 *  Dcm_MemMgrGetSizeOfOverlapp()
 *********************************************************************************************************************/
/*! \brief          Calculates the size of the overlapping area of two memory ranges.
 *  \details        -
 *  \param[in]      firstStart   The start address of the first memory range
 *  \param[in]      firstEnd     The end address of the first memory range
 *  \param[in]      secondStart  The start address of the second memory range
 *  \param[in]      secondEnd    The end address of the second memory range
 *  \return         0    The memory ranges do not overlap
 *  \return         >0   The size of the overlapping memory area
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(uint32, DCM_CODE) Dcm_MemMgrGetSizeOfOverlapp(
  uint32 firstStart,
  uint32 firstEnd,
  uint32 secondStart,
  uint32 secondEnd
  );

/**********************************************************************************************************************
 *  Dcm_MemMgrValidateMemMapInfos()
 *********************************************************************************************************************/
/*! \brief          Validate that two memory ranges do not overlap if both of them are supporting the given operation.
 *  \details        -
 *  \param[in]      first        The first memory mapping info table
 *  \param[in]      second       The second memory mapping info table
 *  \param[in]      operation    The requested memory operation
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_MemMgrValidateMemMapInfos(
  Dcm_CfgMemMgrMemMapInfoPtrType first,
  Dcm_CfgMemMgrMemMapInfoPtrType second,
  Dcm_MemMgrMemoryOpType operation
  );

/**********************************************************************************************************************
 *  Dcm_MemMgrValidateMemMap()
 *********************************************************************************************************************/
/*! \brief          Validate the memory map configuration.
 *  \details        Ensures that start and end addresses are in correct relations and memory ranges do not overlap.
 *                  This is necessary in case that memory labels are used. They are defined by the application and are
 *                  not known during generation time of Dcm. Thus they need to be validated during initialization.
 *  \param[in]      pMemMapDescriptor    Pointer to the (MID specific) memory map
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_MemMgrValidateMemMap(
  Dcm_CfgMemMgrMemIdInfoConstPtrType pMemMapDescriptor
  );

/**********************************************************************************************************************
 *  Dcm_MemMgrCheckMemoryRange()
 *********************************************************************************************************************/
/*! \brief          Validates memory range within a particular memory map.
 *  \details        -
 *  \param[in]      pMemBlock          The memory block descriptor for validation
 *  \param[in]      memOp              The memory block operation type
 *  \param[in]      pMemMapDescriptor  The the corresponding memory map to be searched in
 *  \param[out]     ErrorCode          The NRC
 *  \return         DCM_E_OK     - memory range is valid
 *  \return         DCM_E_NOT_OK - memory range validation failed (ErrorCode contains NRC)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrCheckMemoryRange(
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_MemMgrMemoryOpType memOp,
  Dcm_CfgMemMgrMemIdInfoConstPtrType pMemMapDescriptor,
  Dcm_NegativeResponseCodePtrType ErrorCode
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
 *  Dcm_MemMgrIsOperationSupported()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_MemMgrIsOperationSupported(
  Dcm_MemMgrMemoryOpType memOp,
  Dcm_CfgMemMgrMemMapInfoPtrType pMemMapInfo
  )
{
  boolean lResult = FALSE;

  if ( (pMemMapInfo->ExecCondRefs[memOp] != 0u)
# if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
     &&(Dcm_DiagIsEnabledInActiveVariants(pMemMapInfo->ExecCondRefs[memOp] - (Dcm_CfgStateRefOptType)1u) == TRUE)                                    /* PRQA S 3415, 4304 */ /* MD_Dcm_Rule13.5, MD_MSR_AutosarBoolean */
# endif
    )
  {
    lResult = TRUE;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_MemMgrGetSizeOfOverlapp()
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
DCM_LOCAL_INLINE FUNC(uint32, DCM_CODE) Dcm_MemMgrGetSizeOfOverlapp(
  uint32 firstStart,
  uint32 firstEnd,
  uint32 secondStart,
  uint32 secondEnd
  )
{
  uint32 lResult;

  if ( (firstStart <= secondEnd) && (secondStart <= firstEnd) )
  {
    lResult = Dcm_UtiMathMin(firstEnd, secondEnd)
            - Dcm_UtiMathMax(firstStart, secondStart)
            + 1u;
  }
  else
  {
    lResult = 0u;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_MemMgrValidateMemMapInfos()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_MemMgrValidateMemMapInfos(
  Dcm_CfgMemMgrMemMapInfoPtrType first,
  Dcm_CfgMemMgrMemMapInfoPtrType second,
  Dcm_MemMgrMemoryOpType operation
  )
{
  if (Dcm_MemMgrIsOperationSupported(operation, first) && Dcm_MemMgrIsOperationSupported(operation, second))                                         /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */ /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_PARAM_PTR_FORWARD */
  {
    if (Dcm_MemMgrGetSizeOfOverlapp(first->StartAddr, first->EndAddr, second->StartAddr, second->EndAddr) > 0u)
    {
      Dcm_DebugReportError(DCM_SID_INIT, DCM_E_INVALID_CONFIG);
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_MemMgrValidateMemMap()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_MemMgrValidateMemMap(
  Dcm_CfgMemMgrMemIdInfoConstPtrType pMemMapDescriptor
  )
{
  boolean lMemMapValid = TRUE;

  /* Assert that start and end addresses are in correct relation */
  for (uint16_least i = 0u; i < pMemMapDescriptor->Size; ++i)
  {
    if (pMemMapDescriptor->MemMapTable[i].StartAddr > pMemMapDescriptor->MemMapTable[i].EndAddr)
    {
      Dcm_DebugReportError(DCM_SID_INIT, DCM_E_INVALID_CONFIG);

      lMemMapValid = FALSE;
      break;
    }
  }

  if (lMemMapValid == TRUE)
  {
    /* Assert per operation that memory ranges do not overlap with each other */
    for (uint8_least op = 0u; op < DCM_MEMMGR_NUM_MEMORY_OPERATIONS; ++op)
    {
      for (uint16_least indexFirst = 0u; indexFirst < pMemMapDescriptor->Size; ++indexFirst)
      {
        for (uint16_least indexSecond = indexFirst + 1u; indexSecond < pMemMapDescriptor->Size; ++indexSecond)
        {
          Dcm_MemMgrValidateMemMapInfos(&pMemMapDescriptor->MemMapTable[indexFirst]                                                                  /* SBSW_DCM_PARAM_PTR_READ_ONlY */
                                       ,&pMemMapDescriptor->MemMapTable[indexSecond]
                                       ,(Dcm_MemMgrMemoryOpType)op);
        }
      }
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_MemMgrCheckMemoryRange()
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
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrCheckMemoryRange(                                                                          /* PRQA S 6080 */ /* MD_MSR_STMIF */
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_MemMgrMemoryOpType memOp,
  Dcm_CfgMemMgrMemIdInfoConstPtrType pMemMapDescriptor,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType           lStdResult = DCM_E_OK;
  Dcm_CfgMemMgrReqAddrType lReqEndAddress;

  lReqEndAddress = pMemBlock->Address + (Dcm_CfgMemMgrReqAddrType)(pMemBlock->Length) - 1u; /* possible overflows does not take any effect for now */

  /* Check for address overflow or no data required (length == 0) */
  if ( (pMemBlock->Length != 0u) && (lReqEndAddress >= pMemBlock->Address) )
  {
    uint32 lRemainingLength = pMemBlock->Length;

    for (Dcm_CfgMemMgrMemMapIdxOptType memMapIter = 0; memMapIter < pMemMapDescriptor->Size; ++memMapIter)                                           /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
    {
      Dcm_CfgMemMgrMemMapInfoPtrType pBlockInfo = &pMemMapDescriptor->MemMapTable[memMapIter];

      /* Memory operation supported for this block in currently active variant(s)? */
      if (Dcm_MemMgrIsOperationSupported(memOp, pBlockInfo))                                                                                         /* SBSW_DCM_PARAM_PTR_READ_ONlY */
      {
        uint32 lOverlapSize = Dcm_MemMgrGetSizeOfOverlapp(pMemBlock->Address
                                                         ,lReqEndAddress
                                                         ,pBlockInfo->StartAddr
                                                         ,pBlockInfo->EndAddr);

        if (lOverlapSize > 0u)
        {
          lRemainingLength -= lOverlapSize;

          if (lStdResult == DCM_E_OK) /* still no error found */
          {
            lStdResult = Dcm_StateCheck((pBlockInfo->ExecCondRefs[memOp] - (Dcm_CfgStateRefOptType)1u)
                                       ,DCM_DIAG_CHK_LVL_PARAMETER
                                       ,ErrorCode);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
          }/* else - ErrorCode already set, let see if the range is at least OK */

          /* If the requested memory range is fully covered, skip remaining memory blocks */
          if (lRemainingLength == 0u)
          {
            break;
          }
        }
      }
    }

    if (lRemainingLength > 0u)
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }/* Otherwise - matching area found: either POS_RES for valid states or NEG_RES due to invalid states */
  }
  else
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
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
 *  Dcm_MemMgrInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_MemMgrInit(
  void
  )
{
  if (Dcm_DebugIsDevErrorDetectEnabled())
  {
    for (uint16_least i = 0u; i < DCM_CFGMEMMGRMIDINFO_SIZE; ++i)
    {
      Dcm_MemMgrValidateMemMap(Dcm_CfgWrapMemMgrMidInfo(i));                                                                                         /* SBSW_DCM_PARAM_PTR_READ_ONlY */
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_MemMgrCheckMemBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrCheckMemBlock(
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_MemMgrMemoryOpType memOp,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  sint16_least   lMidIdx = 0;

  if (!Dcm_UtiLookUpUint8IsEmpty(Dcm_CfgMemMgrMidLookUpTable))                                                                                       /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  {
    lMidIdx = Dcm_UtiLookUpUint8(Dcm_CfgMemMgrMidLookUpTable, pMemBlock->Mid);                                                                       /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
  }

  if (lMidIdx < 0)
  {
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    lStdResult = Dcm_MemMgrCheckMemoryRange(pMemBlock, memOp, Dcm_CfgWrapMemMgrMidInfo(lMidIdx), ErrorCode);                                         /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  return lStdResult;
}

# if (DCM_MEMMGR_MEMOP_READ_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_MemMgrReadMemory()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrReadMemory(                                                                                                 /* PRQA S 6050 */ /*  MD_MSR_STCAL */
  Dcm_OpStatusType opStatus,
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                lResult;
  Dcm_ReturnReadWriteMemoryType memOpResult;
  boolean                       lUnknown = FALSE;

  *ErrorCode = DCM_E_GENERALREJECT;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

  if (Dcm_DebugDetectRuntimeError(pDataContext->AvailLen < pMemBlock->Length))                                                                       /* COV_DCM_RTM_RUNTIME_CHECK XF */
  {
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_NOT_OK;
  }
  else /* The data to be read fits into the available buffer */
  {
    memOpResult = Dcm_ReadMemory(opStatus
                                ,pMemBlock->Mid
                                ,pMemBlock->Address
                                ,pMemBlock->Length
                                ,Dcm_UtiGetDataContextBuffer(pDataContext)                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
#  if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
                                ,ErrorCode
#  endif
    );                                                                                                                                               /* SBSW_DCM_CALL_READMEMORY */

    if (memOpResult == DCM_E_OK)
    {
      Dcm_UtiCommitData(pDataContext, pMemBlock->Length);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
    }

    lResult = Dcm_UtiConvMemoryOpResult(memOpResult, opStatus, &lUnknown, ErrorCode);  /* RFC 57196 - return NRC 0x10 */                             /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (lUnknown == TRUE)
    {
      *ErrorCode = DCM_E_GENERALREJECT;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
      Dcm_DebugReportError(DCM_SID_READMEMORY, DCM_E_INTERFACE_RETURN_VALUE);
    }

    if ((lResult != DCM_E_PENDING)
      && (lResult != DCM_E_FORCE_RCRRP))
    {
      /* Release resource */
      Dcm_RsrcMgrReleaseMemLock();
    }

#  if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
    Dcm_UtiHandleApplNrc(lResult, ErrorCode, DCM_E_GENERALREJECT);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
#  endif
  }

  return lResult;
}
# endif

# if (DCM_MEMMGR_MEMOP_WRITE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_MemMgrWriteMemory()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_MemMgrWriteMemory(
  Dcm_OpStatusType opStatus,
  Dcm_MemMgrMemBlockConstPtrType pMemBlock,
  Dcm_MsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Dcm_ReturnReadWriteMemoryType memOpResult;
  Std_ReturnType                lResult;
  boolean                       lUnknown = FALSE;

  *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */

  memOpResult = Dcm_WriteMemory(opStatus
                               ,pMemBlock->Mid
                               ,pMemBlock->Address
                               ,pMemBlock->Length
                               ,data
#  if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
                               ,ErrorCode
#  endif
  );                                                                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

  lResult = Dcm_UtiConvMemoryOpResult(memOpResult, opStatus, &lUnknown, ErrorCode);                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (lUnknown == TRUE)
  {
    *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_WRITEMEMORY, DCM_E_INTERFACE_RETURN_VALUE);
  }

  if ((lResult != DCM_E_PENDING)
    && (lResult != DCM_E_FORCE_RCRRP))
  {
    /* Release resource */
    Dcm_RsrcMgrReleaseMemLock();
  }

#  if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
  Dcm_UtiHandleApplNrc(lResult, ErrorCode, DCM_E_GENERALPROGRAMMINGFAILURE);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
#  endif

  return lResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_MEMMGR_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_MemMgr.c
 *********************************************************************************************************************/
