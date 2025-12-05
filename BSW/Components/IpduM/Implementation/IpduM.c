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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  IpduM.c
 *         \unit  General
 *        \brief  Microsar IpduM source file
 *      \details  This is the implementation of the MICROSAR IpduM module.
 *                The basic software module is based on the AUTOSAR IpduM specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define IPDUM_SOURCE
/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 883, 3406 EOF */ /* MD_IpduM_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "IpduM.h"
#include "IpduM_Reporting.h"
#include "IpduM_MuxTx.h"
#include "IpduM_MuxRx.h"
#include "IpduM_ContainerTx.h"
#include "IpduM_ContainerTxTrigger.h"
#include "IpduM_ContainerTxDataQueue.h"
#include "IpduM_ContainerTxRequestQueue.h"
#include "IpduM_ContainerTxSendTimeout.h"
#include "IpduM_ContainerUtil.h"
#include "IpduM_ContainerRx.h"
#include "IpduM_ContainerRxQueue.h"
#include "IpduM_TxConfBuffer.h"
#include "IpduM_TxConfirmation.h"
#include "IpduM_Cbk.h"

#if(IPDUM_USE_INIT_POINTER == STD_ON)
# include "EcuM_Error.h"
#endif

/**********************************************************************************************************************
  VERSION CHECK
**********************************************************************************************************************/
/* vendor specific version information is BCD coded */
#if ((IPDUM_SW_MAJOR_VERSION != 13u) || (IPDUM_SW_MINOR_VERSION != 2u) || (IPDUM_SW_PATCH_VERSION != 0u))
# error "IpduM.c: IpduM.c and IpduM.h versions are inconsistent!"
#endif

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */  /* MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_Init()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6010, 6030 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
FUNC(void, IPDUM_CODE) IpduM_Init(P2CONST(IpduM_ConfigType, AUTOMATIC, IPDUM_INIT_DATA) config)
{
  uint8 errorId = IPDUM_E_NO_ERROR;
#if(IPDUM_USE_INIT_POINTER == STD_ON)
  IpduM_ConfigDataPtr = config;
  if(IpduM_ConfigDataPtr == NULL_PTR)
  {
# if(IPDUM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)   /* COV_IPDUM_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(IPDUM_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
# if (IPDUM_DEV_ERROR_DETECT == STD_ON)
    errorId = IPDUM_E_PARAM_POINTER;
# endif
  }
# if (IPDUM_FINALMAGICNUMBER == STD_ON) /* COV_IPDUM_VAR_DEPENDENT_SWITCH */
  else if(IpduM_GetFinalMagicNumber() != IPDUM_FINAL_MAGIC_NUMBER)
  {
    IpduM_ConfigDataPtr = NULL_PTR;
#  if(IPDUM_USE_ECUM_BSW_ERROR_HOOK == STD_ON)  /* COV_IPDUM_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(IPDUM_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif
#  if (IPDUM_DEV_ERROR_DETECT == STD_ON)
    errorId = IPDUM_E_PARAM_POINTER;
#  endif
  }
# endif
#else
  if(config != NULL_PTR)        /* COV_IPDUM_PRECOMPILE_CONFIG */
  {
# if (IPDUM_DEV_ERROR_DETECT == STD_ON)
    errorId = IPDUM_E_PARAM_POINTER;
# endif
  }
#endif
  else
  {
    IpduM_TxConfirmation_Init();

    IpduM_MuxTx_Init();

    IpduM_ContainerTxTrigger_Init();
    IpduM_ContainerTx_DataQueue_Init();
    IpduM_ContainerTx_RequestQueue_Init();
    IpduM_ContainerTx_SendTimeout_Init();
    IpduM_TxConfBuffer_Init();
    
    IpduM_ContainerRxQueue_Init();

    IpduM_SetInitialized(TRUE);
  }

  if(errorId != IPDUM_E_NO_ERROR)       /* COV_IPDUM_NO_DEV_ERROR */
  {
    IpduM_Reporting_ReportError(IPDUM_APIID_INIT, errorId);  /* PRQA S 2880 */ /* MD_IpduM_2880 */
  }
}

/*******************************************************************************************
 *  IpduM_InitMemory()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_InitMemory(void)
{
  /* Set the IpduM to "uninitialized" */
  IpduM_SetInitialized(FALSE);
#if(IPDUM_USE_INIT_POINTER == STD_ON)
  IpduM_ConfigDataPtr = NULL_PTR;  /* VCA_IPDUM_CONFIGPTR_RESET */
#endif
}

/*******************************************************************************************
*  IpduM_MainFunctionTxInternal()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_MainFunctionTxInternal(IpduM_MainFunctionTxIndirectionIterType mainFunctionTxIndirectionIdx)
{
  if(mainFunctionTxIndirectionIdx < IpduM_GetSizeOfMainFunctionTxIndirection())
  {
    IpduM_TxConfirmation_MainFunction(mainFunctionTxIndirectionIdx);
    IpduM_ContainerTx_MainFunction(mainFunctionTxIndirectionIdx);
  }
}

/*******************************************************************************************
*  IpduM_MainFunctionRxInternal()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_MainFunctionRxInternal(IpduM_MainFunctionRxIndirectionIterType mainFunctionRxIndirectionIdx)
{
  if(mainFunctionRxIndirectionIdx < IpduM_GetSizeOfMainFunctionRxIndirection())
  {
    IpduM_ContainerRx_MainFunction(mainFunctionRxIndirectionIdx);
  }  
}

/*******************************************************************************************
 *  IpduM_Transmit()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_Transmit(PduIdType PdumTxPduId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr) /* PRQA S 6080 */ /* MD_MSR_STMIF */
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = IPDUM_E_NO_ERROR;

#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  if(!IpduM_IsInitialized())
  {
    errorId = IPDUM_E_UNINIT;
  }
  else if(PdumTxPduId >= IpduM_GetSizeOfTxUpInfo())
  {
    errorId = IPDUM_E_PARAM;
  }
  else if((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR))
  {
    errorId = IPDUM_E_PARAM_POINTER;
  }
  else
#endif
  {
    const IpduM_TxUpInfoIterType txUpInfoIdx = (IpduM_TxUpInfoIterType) PdumTxPduId;
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
    if(IpduM_IsInvalidHndOfTxUpInfo(txUpInfoIdx))
    {
      errorId = IPDUM_E_PARAM;
    }
    else
#endif
    {
      switch (IpduM_GetTxUpIndOfTxUpInfo(txUpInfoIdx))
      {
        case IPDUM_TXPART_TXUPINDOFTXUPINFO:
        {
          retVal = IpduM_MuxTx_Transmit(IpduM_GetTxUpIndIdxOfTxUpInfo(txUpInfoIdx), PduInfoPtr);
          break;
        }
        case IPDUM_TXCONTAINEDPDU_TXUPINDOFTXUPINFO:
        {
          retVal = IpduM_ContainerTx_Transmit(IpduM_GetTxUpIndIdxOfTxUpInfo(txUpInfoIdx), PduInfoPtr);
          break;
        }
        default: /* COV_IPDUM_MISRA */
        {
          break;
        }
      }
    }
  }
  if(errorId != IPDUM_E_NO_ERROR)
  {
    IpduM_Reporting_ReportError(IPDUM_APIID_TRANSMIT, (errorId));  /* PRQA S 2880 */ /* MD_IpduM_2880 */
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_TriggerTransmit()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_TriggerTransmit(PduIdType TxPduId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr) /* PRQA S 6080 */ /* MD_MSR_STMIF */
{
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = IPDUM_E_NO_ERROR;

# if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  if(!IpduM_IsInitialized())
  {
    errorId = IPDUM_E_UNINIT;
  }
  else if((TxPduId >= IpduM_GetSizeOfTxLoInfoWithInvalidIndexes()))
  {
    errorId = IPDUM_E_PARAM;
  }
  else
# endif
  {
    IpduM_TxLoInfoWithInvalidIndexesIterType txLoInfoWithInvalidIndexesIdx = (IpduM_TxLoInfoWithInvalidIndexesIterType) TxPduId;
# if (IPDUM_DEV_ERROR_DETECT == STD_ON)
    if(IpduM_IsInvalidHndOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx))
    {
      errorId = IPDUM_E_PARAM;
    }
    else if((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR))
    {
      errorId = IPDUM_E_PARAM_POINTER;
    }
    else
# endif
    {
      IpduM_CTxLoInfoIterType txLoInfoIdx = IpduM_GetCTxLoInfoIdxOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx);

      if(IpduM_IsTxPathwayUsedOfCTxLoInfo(txLoInfoIdx))
      {
        retVal = IpduM_MuxTx_TriggerTransmit(IpduM_GetTxPathwayIdxOfCTxLoInfo(txLoInfoIdx), PduInfoPtr);
      }
      if(IpduM_IsTxContainerPduUsedOfCTxLoInfo(txLoInfoIdx))
      {
        retVal = IpduM_ContainerTx_TriggerTransmit(IpduM_GetTxContainerPduIdxOfCTxLoInfo(txLoInfoIdx), PduInfoPtr);
      }
    }
  }

  if(errorId != IPDUM_E_NO_ERROR)
  {
    IpduM_Reporting_ReportError(IPDUM_APIID_TRIGGERTRANSMIT, errorId);  /* PRQA S 2880 */ /* MD_IpduM_2880 */
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_TxConfirmation()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_TxConfirmation(PduIdType TxPduId)
{
  uint8 errorId = IPDUM_E_NO_ERROR;

# if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  if(!IpduM_IsInitialized())
  {
    errorId = IPDUM_E_UNINIT;
  }
  else if(TxPduId >= IpduM_GetSizeOfTxLoInfoWithInvalidIndexes())
  {
    errorId = IPDUM_E_PARAM;
  }
  else
# endif
  {
    IpduM_TxLoInfoWithInvalidIndexesIterType txLoInfoWithInvalidIndexesIdx = (IpduM_TxLoInfoWithInvalidIndexesIterType) TxPduId;

# if (IPDUM_DEV_ERROR_DETECT == STD_ON)
    if(IpduM_IsInvalidHndOfTxLoInfoWithInvalidIndexes(txLoInfoWithInvalidIndexesIdx))
    {
      errorId = IPDUM_E_PARAM;
    }
    else
# endif
    {
      IpduM_TxConfirmation_TxConfirmation(txLoInfoWithInvalidIndexesIdx);
    }
  }

  if(errorId != IPDUM_E_NO_ERROR)
  {
    IpduM_Reporting_ReportError(IPDUM_APIID_TXCONFIRMATION, errorId);  /* PRQA S 2880 */ /* MD_IpduM_2880 */
  }
}

/*******************************************************************************************
 *  IpduM_RxIndication()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_RxIndication(PduIdType RxPduId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr) /* PRQA S 6080 */ /* MD_MSR_STMIF */
{
  uint8 errorId = IPDUM_E_NO_ERROR;

#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  if(!IpduM_IsInitialized())
  {
    errorId = IPDUM_E_UNINIT;
  }
  else if(RxPduId >= IpduM_GetSizeOfRxInfo())
  {
    errorId = IPDUM_E_PARAM;
  }
  else
#endif
  {
    IpduM_RxInfoIterType rxInfoIdx = (IpduM_RxInfoIterType) RxPduId;

#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
    if(IpduM_IsInvalidHndOfRxInfo(rxInfoIdx))
    {
      errorId = IPDUM_E_PARAM;
    }
    else if((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR))
    {
      errorId = IPDUM_E_PARAM_POINTER;
    }
    else
#endif
    {
      switch (IpduM_GetRxPathwayIndOfRxInfo(rxInfoIdx))
      {
        case IPDUM_RXMUXPDU_RXPATHWAYINDOFRXINFO:
        {
          IpduM_RxPathwayIndIdxOfRxInfoType rxPathwayIndIdx = IpduM_GetRxPathwayIndIdxOfRxInfo(rxInfoIdx);
          IpduM_MuxRx_RxIndication(rxPathwayIndIdx, PduInfoPtr);
          break;
        }
        case IPDUM_RXCONTAINERPDU_RXPATHWAYINDOFRXINFO:
        {
          IpduM_RxPathwayIndIdxOfRxInfoType rxPathwayIndIdx = IpduM_GetRxPathwayIndIdxOfRxInfo(rxInfoIdx);
          IpduM_ContainerRx_RxIndication(rxPathwayIndIdx, PduInfoPtr);
          break;
        }
        default: /* COV_IPDUM_MISRA */
        {
          break;
        }
      }
    }
  }

  if(errorId != IPDUM_E_NO_ERROR)
  {
    IpduM_Reporting_ReportError(IPDUM_APIID_RXINDICATION, errorId);  /* PRQA S 2880 */ /* MD_IpduM_2880 */
  }
}

/*******************************************************************************************
 *  IpduM_GetVersionInfo()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, IPDUM_APPL_DATA) versioninfo)
{
  uint8 errorId = IPDUM_E_NO_ERROR;
# if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  if(versioninfo == NULL_PTR)
  {
    errorId = IPDUM_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* Set version info / fill struct */
    versioninfo->vendorID = (uint16) IPDUM_VENDOR_ID;
    versioninfo->moduleID = (uint8) IPDUM_MODULE_ID;
    versioninfo->sw_major_version = (uint8) IPDUM_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = (uint8) IPDUM_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = (uint8) IPDUM_SW_PATCH_VERSION;
  }

  if(errorId != IPDUM_E_NO_ERROR)
  {
    IpduM_Reporting_ReportError(IPDUM_APIID_VERSIONINFO, errorId);  /* PRQA S 2880 */ /* MD_IpduM_2880 */
  }
}

/*******************************************************************************************
 *  IpduM_CalculateSizeOfContainer()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint32, IPDUM_CODE) IpduM_CalculateSizeOfContainer(PduLengthType headerSize, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)   /* PRQA S 1505 */ /* MD_MSR_Rule8.7 */
{
  uint8 errorId = IPDUM_E_NO_ERROR;
  uint32 containerSize = 0u;
  
#if (IPDUM_DEV_ERROR_DETECT == STD_ON)
  if(!IpduM_IsInitialized())
  {
    errorId = IPDUM_E_UNINIT;
  }
  else if((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR))
  {
    errorId = IPDUM_E_PARAM_POINTER;
  } 
  else if(!(headerSize == IPDUM_HEADERSIZE_LONG) && !(headerSize == IPDUM_HEADERSIZE_SHORT))
  {
    errorId = IPDUM_E_PARAM;
  }
  else
#endif
  {
    containerSize = IpduM_ContainerUtil_CalculateSizeOfContainer(headerSize, PduInfoPtr);  /* VCA_IPDUM_API_PATTERN_CHECK */
  }
  
  if(errorId != IPDUM_E_NO_ERROR)
  {
    IpduM_Reporting_ReportError(IPDUM_APIID_CALCULATECONTAINERSIZE, errorId);  /* PRQA S 2880 */ /* MD_IpduM_2880 */
  }

  return containerSize;
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */  /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* *INDENT-OFF* */
/* module specific MISRA deviations:
   MD_IpduM_0771: Multiple break statements
     Reason:      The additional break statement is there to ensure the loop terminates in a deterministic time.
     Risk:        No risk.
     Prevention:  No prevention necessary.

   MD_IpduM_2842: Index Access to pointer array
     Reason:      QAC cannot determine whether the pointer value is correct. However, the ComStackLib generates
                  those values and if the indices used are correct, the pointers are, too. The indices are either
                  calculated using ComStackLib indirections, ComStackLib SizeOf() macros or passed on by a caller
                  of this function, hence being checked by the caller. So all indices used in these macros are
                  correct.
     Risk:        No risk.
     Prevention:  No prevention necessary.

   MD_IpduM_2985: Misra Rule 2.2
     Reason:      The bit shift of 0 bits is because of maintainability and comprehensibility. Or the result depends on the configuration.
     Risk:        No risk, because only warning that the operation is redundant.
     Prevention:  No prevention necessary.

   MD_IpduM_2986: Misra Rule 2.2
     Reason:      The value of the operands depends on the configuration.
     Risk:        No risk, because only warning that the operation is redundant.
     Prevention:  No prevention necessary.

   MD_IpduM_2982: Misra Rule 2.2  
     Reason:      The number of loop iterations depends on the configuration. When there is only one iteration this value is optimized.
     Risk:        No risk, because only warning that assignment is redundant.
     Prevention:  No prevention necessary.

   MD_IpduM_2982_RetVal: Redundant assignment, value is never used before being modified.
     Reason:      It depends on the configuration if the value is used before being modified.
     Risk:        No risk.
     Prevention:  No prevention necessary.

   MD_IpduM_2880: Misra Rule 2.1
     Reason:      If the development error checks are switched off, no error will be reported ever.
     Risk:        No risk, as the code will be removed by the compiler.
     Prevention:  No prevention necessary.

   MD_IpduM_UnityBuild:
     Reason:     The code is compiled as unity build where c files are used like h files.
     Risk:       No risk, because duplicate symbols are warned detected by the compiler.
     Prevention: No prevention necessary.
*/

/* VCA_JUSTIFICATION_BEGIN

  \ID VCA_IPDUM_ZERO_TO_N_LOOP_NOT_SUPPORTED
    \DESCRIPTION       IpduM_MuxUtil_WriteMuxFillerSegment is called outside of its specification. This is a false positive.
    \COUNTERMEASURE \N The API is called in a 0 to N indirection loop. This is qualified use case CSL03.

  \ID VCA_IPDUM_ARRAY_WRITE_WITH_SIZE_VARIABLE_CHECKED_BY_CALLER
    \DESCRIPTION       Array is written with index access.
    \COUNTERMEASURE \R Additional variable passed along with the array is checked by caller to reflect the remaining
                       space in the array. This variable is used for run-time checks.

  \ID VCA_IPDUM_DYNAMICPARTIDX
    \DESCRIPTION    IpduM reads the value of the last dynamic part which was transmitted to IpduM. This value is used for further processing of the multiplex Pdu.
    \COUNTERMEASURE \N The stored dynamic part index is always valid, as it is initialized to some valid initial value and is otherwise never set to non existing values.

  \ID VCA_IPDUM_EXTERNALPOINTER_AND_LENGTH
    \DESCRIPTION    IpduM accesses the buffer behind the pointer until its length.
    \COUNTERMEASURE \N The buffer pointer and length is handed over from the caller of IpduM. It is checked against NULL_PTR and otherwise considered valid.

  \ID VCA_IPDUM_API_PATTERN_CHECK
    \DESCRIPTION    The pointer is checked in the API pattern. VCA does not remember this until this sub-function is reached.
    \COUNTERMEASURE \R The pointer is checked in the API pattern.

  \ID VCA_IPDUM_RING_BUFFER
    \DESCRIPTION    IpduM reads the read or writeIdx for further queue processing.
    \COUNTERMEASURE \N The read or writeIdx always runs between the start and endIdx of a corresponding buffer. It can not run out of bounds if it is not overwritten by external code.

  \ID VCA_IPDUM_MEMCPY_1
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The destination pointer and length is statically read using CSL use cases. The internal handle ID used for this is derived from the external handle ID, which is checked to be in range and valid.
                       The source pointer and length is read using the readIdx of the queue and a CSL03 indirection. The readIdx can have values between startIdx and endIdx of the buffer.
 
  \ID VCA_IPDUM_MEMCPY_2
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The destination pointer and length is read using the writeIdx of the queue and a CSL03 indirection. The writeIdx can have values between startIdx and endIdx of the buffer.
                       The source pointer and length is handed over from the caller of IpduM. It is checked against NULL_PTR and otherwise considered valid.
  
  \ID VCA_IPDUM_MEMCPY_3
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The destination pointer and length is handed over from the caller of IpduM. It is checked against NULL_PTR and otherwise considered valid.
                       The source pointer and length is read using the readIdx of the queue and a CSL03 indirection. The readIdx can have values between startIdx and endIdx of the buffer.
  
  \ID VCA_IPDUM_MEMCPY_4
    \DESCRIPTION    VCA warns that VStdLib_MemCpy is called outside of its specification.
    \COUNTERMEASURE \R The destination pointer is read using CSL indirections and an offset. A runtime checks assures that the data fits into the buffer.
                       The source pointer and length is handed over from the caller of IpduM. It is checked against NULL_PTR and otherwise considered valid.

  \ID VCA_IPDUM_MEMCPY_5
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The destination pointer and length is handed over from the caller of IpduM. It is checked against NULL_PTR and otherwise considered valid.
                       The source pointer and length is statically read using CSL03 use case. The internal handle ID used for this is derived from the external handle ID, which is checked to be in range and valid.

  \ID VCA_IPDUM_MEMCPY_7
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The destination pointer and length is handed over from the caller of IpduM. It is checked against NULL_PTR and otherwise considered valid.
                       The source pointer and length is statically read using CSL03 use case. The internal handle ID used for this is derived from the external handle ID, which is checked to be in range and valid.

  \ID VCA_IPDUM_MEMCPY_8
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \N The caller hands over a pointer, length and id. The caller ensures they match and can be used with CSL use cases to get matching indirections and related values.
                       The destination pointer is defined locally and the destination length is a static define used for the destination pointer's creation.
                       The source pointer is handed over by the caller, as mentioned above. It is accessed with a calculated offset using values derived from the other handed over parameters, ensuring they will always be in bounds.
                       The length is statically read using CSL03 use case using a handed over parameter.

  \ID VCA_IPDUM_MEMCPY_9
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \R The caller hands over a pointer, length and id. The caller ensures they match and can be used with CSL use cases to get matching indirections and related values.
                       The destination pointer is statically read using CSL use cases, using the handed over ID.
                       The destination length to (maximally) use is calculated from values read using CSL use cases, using the handed over ID.
                       The source pointer is statically read using CSL03 use case with the handed over ID. It is accessed using a calculated offset using a handed over parameter. A surrounding run-time check ensures this fits.
                       The source length is verified in an encapsulating if statement and therefore is valid.

  \ID VCA_IPDUM_MEMCPY_10
    \DESCRIPTION    VCA warns that VStdLib_MemCpy_s is called outside of its specification.
    \COUNTERMEASURE \R The caller hands over a pointer, length and id. The caller ensures they match and can be used with CSL use cases to get matching indirections and related values.
                       The destination pointer is statically read using CSL use cases, using the handed over ID. It is accessed using an offset verified by runtime check encapsulating the code.
                       The number of bytes maximally used from this buffer is limited by a static define that is based on the feature's specification.
                       The source pointer is a local variable matching the size of the maximal number of bytes above.
                       The number of bytes to copy is read using CSL03 use case, using the handed over id.

  \ID VCA_IPDUM_MEMSET_1
    \DESCRIPTION    VCA warns that VStdLib_MemSet is called outside of its specification.
    \COUNTERMEASURE \N The caller hands over a pointer, length and id. The caller ensures they match and can be used with CSL use cases to get matching indirections and related values.
                       The destination pointer is handed over from the caller of this function. The caller ensures it is valid, as mentioned above.
                       The number of bytes to write is read using CSL03 use case using the id handed over from the caller of this function, matching the destination pointer's configuration.
 
  \ID VCA_IPDUM_CONFIGPTR_RESET
    \DESCRIPTION    VCA warns that write access to config pointer after initialization is illegal.
    \COUNTERMEASURE \N The config pointer is reset inside the InitMemory API. This is a false positive.

  \ID VCA_IPDUM_VCA_MEMCPY_S
    \DESCRIPTION    VCA warns that access is possibly out of bounds.
    \COUNTERMEASURE \N The VCA spec ensures a potential incorrect call to this API is shown as an error in the calling code. These errors will be justified in the calling code.

  \ID VCA_IPDUM_VCA_BITCPY_S
    \DESCRIPTION    VCA warns that access is possibly out of bounds.
    \COUNTERMEASURE \N The VCA spec ensures a potential incorrect call to this API is shown as an error in the calling code. These errors will be justified in the calling code.

VCA_JUSTIFICATION_END */

/* COV_JUSTIFICATION_BEGIN

  \ID COV_IPDUM_VAR_DEPENDENT_SWITCH
    \ACCEPT TX
    \ACCEPT TF tf tx
    \ACCEPT TF tx tf
    \ACCEPT TX tx tx
    \ACCEPT TX tf tf tf
    \ACCEPT TX tx tf tf
    \ACCEPT TF tx tf tf
    \REASON The nested switch depends on a previous switch.

  \ID COV_IPDUM_PRECOMPILE_CONFIG
    \ACCEPT XF
    \REASON Precompile configurations are always initialized with a NULL config pointer.

  \ID COV_IPDUM_NO_DEV_ERROR
    \ACCEPT XF
    \REASON Only tests with erroneous configurations would cover this.

  \ID COV_IPDUM_DEFENSIVE_PROGRAMMING
    \ACCEPT TX
    \ACCEPT XF
    \ACCEPT TF tf tx
    \ACCEPT TX tx tx
    \REASON This condition is used because of safe programming, should never be false in production code.

  \ID COV_IPDUM_MISRA
    \ACCEPT X
    \ACCEPT TX
    \REASON [MSR_COV_MISRA]

COV_JUSTIFICATION_END */

/* FETA_JUSTIFICATION_BEGIN

    \ID FETA_IPDUM_01
      \DESCRIPTION Loop over static number of elements.
      \COUNTERMEASURE \N The loop has a static upper limit determined by a ROM value. It will always terminate in finite time.

    \ID FETA_IPDUM_02
      \DESCRIPTION Loop with external termination condition.
      \COUNTERMEASURE \R The loop uses a local loop counter with static upper limit to break the loop in case the external termination condition never breaks the loop.

    \ID FETA_IPDUM_03
      \DESCRIPTION Loop over sizeof(datatype).
      \COUNTERMEASURE \N The loop has a static upper limit determined by a compile time value. It will always terminate in finite time.
      
    \ID FETA_IPDUM_04
      \DESCRIPTION Loop with local variable or function parameter of non-pointer type that is not changed within the loop as upper limit.
      \COUNTERMEASURE \N The loop has a static upper limit determined by a local variable that is set before the loop and not changed within the loop.

    \ID FETA_IPDUM_05
      \DESCRIPTION Loop over handed over buffer. 
      \COUNTERMEASURE \R The loop terminates either if containerSize reaches some fixed value OR an inconsistent dlc is read which would lead to an uint32 overflow OR a zero header is read.

FETA_JUSTIFICATION_END */

/* *INDENT-ON* */

/**********************************************************************************************************************
  END OF FILE: IpduM.c
**********************************************************************************************************************/
