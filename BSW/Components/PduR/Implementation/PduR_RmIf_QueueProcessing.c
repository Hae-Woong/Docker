/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  PduR_RmIf_QueueProcessing.c
 *         \unit  RmIf_QueueProcessing
 *        \brief  Pdu Router Routing Manager Interface Queue Processing source file
 *      \details  This is the implementation of the MICROSAR Pdu Router module.
 *                The basic software module is based on the AUTOSAR PduR specification.
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

/**********************************************************************************************************************
 * LOCAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

#define PDUR_RMIF_QUEUEPROCESSING_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Lock.h"
#include "PduR_Mm.h"
#include "PduR_MultipleSourceHandler.h"
#include "PduR_QAL.h"
#include "PduR_Reporting.h"
#include "PduR_RmIf_QueueProcessing.h"
#include "PduR_RmIf_DeferredProcessing.h"

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
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_FifoHandling
 *********************************************************************************************************************/
/*! \brief      Perform FIFO buffer and transmission handling
 *  \details    -
 *  \param[in]  rmDestRomIdx  Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]  rmGDestRomIdx Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in]  pduLength  Pdu length
 *  \param[in]  sduDataPtr Pdu data
 *  \pre        -
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_FifoHandling(PduR_RmDestRomIterType rmDestRomIdx, PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr);
#endif

#if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_Forward2Buffer
**********************************************************************************************************************/
/*! \brief       The function forwards the If Pdu to the corresponding buffer handling function.
 *  \details     -
 *  \param[in]   rmDestRomIdx         Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in]   pduLength         Pdu length
 *  \param[in]   sduDataPtr        Pdu data
 *  \pre         -
**********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_Forward2Buffer(PduR_RmDestRomIterType rmDestRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr);
#endif

#if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_SingleBufferHandling
 *********************************************************************************************************************/
/*! \brief     This function processes 'Single-buffer-routings'
 *  \details   -
 *  \param[in] rmDestRomIdx   Valid RmDestRom table index. Internal handle of a PduRDestPdu.
 *  \param[in] rmGDestRomIdx  Valid RmGDestRom table index. Internal handle of a EcuC Pdu which is used by a PduRDestPdu.
 *  \param[in] pduLength      Pdu length
 *  \param[in] sduDataPtr     Pdu data
 *  \pre       -
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_SingleBufferHandling(PduR_RmDestRomIterType rmDestRomIdx, PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_SingleBufferHandling
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_SingleBufferHandling(PduR_RmDestRomIterType rmDestRomIdx, PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr)
{
  (void) PduR_QAL_Put(rmGDestRomIdx, rmDestRomIdx, pduLength, sduDataPtr);

  (void) PduR_RmIf_DeferredProcessing_Transmit(rmDestRomIdx, PDUR_RMIF_SINGLEBUFFERHANDLING);
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_FifoHandling
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
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_FifoHandling(PduR_RmDestRomIterType rmDestRomIdx, PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr)
{
  if(PduR_QAL_Put(rmGDestRomIdx, rmDestRomIdx, pduLength, sduDataPtr) != E_OK)
  {
    PduR_QAL_Flush(rmGDestRomIdx);
    PduR_Reporting_ReportError(PDUR_FCT_RMIF_FQ, PDUR_E_PDU_INSTANCES_LOST);
    PduR_MultipleSourceHandler_Reset(rmGDestRomIdx);

    PduR_Reporting_NotifyQueueOverflow(rmGDestRomIdx);

    /* Queuing the Pdu after a flush could fail for a shared buffer queue. In this case the fill level will be zero and nothing will be transmitted to the destination. */
    (void) PduR_QAL_Put(rmGDestRomIdx, rmDestRomIdx, pduLength, sduDataPtr);
  }

  if(PduR_QAL_GetFillLevel(rmGDestRomIdx) == 1u)
  {
    (void) PduR_RmIf_DeferredProcessing_Transmit(rmDestRomIdx, PDUR_FCT_RMIF_FQ);
  }
}
#endif

#if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_Forward2Buffer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_Forward2Buffer(PduR_RmDestRomIterType rmDestRomIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr)
{
  PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);
  PduR_Lock_LockRoutingPathByDestPdu(rmDestRomIdx);

  switch (PduR_GetQueueTypeOfRmBufferedIfPropertiesRom(PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)), PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(rmGDestRomIdx)))
  {
    case PDUR_SINGLE_BUFFER_QUEUETYPEOFRMBUFFEREDIFPROPERTIESROM:
      PduR_RmIf_QueueProcessing_SingleBufferHandling(rmDestRomIdx, rmGDestRomIdx, pduLength, sduDataPtr);
      break;
    case PDUR_FIFO_QUEUETYPEOFRMBUFFEREDIFPROPERTIESROM:
      PduR_RmIf_QueueProcessing_FifoHandling(rmDestRomIdx, rmGDestRomIdx, pduLength, sduDataPtr);
      break;
    default:   /* COV_PDUR_MISRA */
      /* Nothing to do: This should never be reached */
      break;
  }

  PduR_Lock_UnlockRoutingPathByDestPdu(rmDestRomIdx);
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_ProcessBufferedRouting
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
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_QueueProcessing_ProcessBufferedRouting(PduR_RmSrcRomIterType rmSrcRomIdx, PduR_RmDestRomIterType rmDestRomIdx, PduR_RmGDestRomIterType rmGDestRomIdx,
                                                                              P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  if(info == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else
# endif
  {
    if(PduR_IsTriggerTransmitSupportedOfRmSrcRom(rmSrcRomIdx))
    {
      PduLengthType maxPduLength = PduR_GetMaxPduLengthOfRmGDestRom(rmGDestRomIdx);
      uint8 sduData[PDUR_MAX_TRIGGER_TRANSMIT_PDU_SIZE] = { 0u };
      PduInfoType pduInfo;
      pduInfo.SduDataPtr = sduData;
      pduInfo.SduLength = maxPduLength;

      retVal = PduR_Mm_CallIfTriggerTransmit(PduR_GetMmRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetSrcHndOfRmSrcRom(rmSrcRomIdx), &pduInfo);

      /* Truncate length, if upper layer has written some too large value here. */
      if(pduInfo.SduLength > maxPduLength)
      {
        pduInfo.SduLength = maxPduLength;
      }
      if(retVal == E_OK)
      {
        /* Use hard coded sduData address so that pointer can not be changed by upper layer accidently. */
        PduR_RmIf_QueueProcessing_Forward2Buffer(rmDestRomIdx, pduInfo.SduLength, sduData);
      }
    }
    else
    {
      PduLengthType pduLength = info->SduLength;
      SduDataPtrType sduDataPtr = info->SduDataPtr;

      /*@ assert $external(sduDataPtr); */  /* VCA_PDUR_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */
      /*@ assert $lengthOf(sduDataPtr) >= pduLength; */  /* VCA_PDUR_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */

      PduR_RmIf_QueueProcessing_Forward2Buffer(rmDestRomIdx, pduLength, sduDataPtr);
      retVal = E_OK;
    }

    if(PduR_GetPduRDestPduProcessingOfRmGDestRom(rmGDestRomIdx) == PDUR_DEFERRED_PDURDESTPDUPROCESSINGOFRMGDESTROM)
    {
      /* Call upper layer confirmation immediately */
      if(PduR_IsTxConfirmationSupportedOfRmSrcRom(rmSrcRomIdx))
      {
        PduR_Mm_CallIfTxConfirmation(PduR_GetMmRomIdxOfRmSrcRom(rmSrcRomIdx), PduR_GetSrcHndOfRmSrcRom(rmSrcRomIdx));;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_RMIF_ROUTE, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }

  return retVal;
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_TriggerTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_QueueProcessing_TriggerTransmit(PduR_RmDestRomIterType rmDestRomIdx, CONSTP2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = PDUR_E_NO_ERROR;      /* PRQA S 2981 */ /* MD_MSR_RetVal */
  /* ----- Development Error Checks ------------------------------------- */
# if (PDUR_DEV_ERROR_DETECT == STD_ON)  /* COV_PDUR_VAR_DEV_ERROR_DETECT */
  /* Check parameter 'info' */
  if(info == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = PDUR_E_PARAM_POINTER;
  }
  else
# endif
  {
    PduR_Lock_LockRoutingPathByDestPdu(rmDestRomIdx);
    {
      PduR_RmGDestRomIterType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);
      PduInfoType queuedPduInfo = {NULL_PTR, 0u};
      PduR_RmDestRomIterType readRmDestRomIdx = 0u;

      if(PduR_QAL_Get(rmGDestRomIdx, &readRmDestRomIdx, &queuedPduInfo) == E_OK)
      {
        if(info->SduLength >= queuedPduInfo.SduLength)
        {
          info->SduLength = queuedPduInfo.SduLength;
          /* @ assert $external(info->SduDataPtr); */  /* VCA_PDUR_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */
          /* @ assert ($lengthOf(info->SduDataPtr) >= queuedPduInfo.SduLength); */  /* VCA_PDUR_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */
          VStdLib_MemCpy_s(info->SduDataPtr, info->SduLength, queuedPduInfo.SduDataPtr, queuedPduInfo.SduLength);  /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* VCA_PDUR_RMIF_QUEUEPROCESSING_MEMCPY_S */
          retVal = E_OK;
        }
      }
    }
    PduR_Lock_UnlockRoutingPathByDestPdu(rmDestRomIdx);
  }
  /* ----- Development Error Report --------------------------------------- */
  if(errorId != PDUR_E_NO_ERROR)
  {
    PduR_Reporting_ReportError(PDUR_FCT_RMIF_TXTT, errorId);  /* PRQA S 2880 */ /* MD_PduR_2880 */
  }
  return retVal;
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_TxConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_TxConfirmation(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  PduR_Lock_LockRoutingPathByGDestPdu(rmGDestRomIdx);
  if(PduR_RmIf_DeferredProcessing_IsDeferredTransmissionTriggered(rmGDestRomIdx) == FALSE)
  {
    PduR_QAL_Remove(rmGDestRomIdx);
    (void) PduR_RmIf_QueueProcessing_TransmitImmediately(rmGDestRomIdx, PDUR_FCT_RMIF_TXCONF);
  }
  PduR_Lock_UnlockRoutingPathByGDestPdu(rmGDestRomIdx);
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_TransmitImmediately
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_RmIf_QueueProcessing_TransmitImmediately(PduR_RmGDestRomIterType rmGDestRomIdx, uint8 apiId)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_RmDestRomIterType rmDestRomIdx = 0u;
  PduInfoType destPduInfo = {NULL_PTR, 0u};

  if(PduR_QAL_Get(rmGDestRomIdx, &rmDestRomIdx, &destPduInfo) == E_OK)
  {
    retVal = PduR_MultipleSourceHandler_IfTransmit(rmDestRomIdx, &destPduInfo);

    if(retVal != E_OK)
    {
      if(PduR_GetQueueTypeOfRmBufferedIfPropertiesRom(PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(rmGDestRomIdx))
             == PDUR_FIFO_QUEUETYPEOFRMBUFFEREDIFPROPERTIESROM)
      {
        PduR_QAL_Flush(rmGDestRomIdx);
      }
      PduR_Reporting_ReportError(apiId, PDUR_E_PDU_INSTANCES_LOST);
    }
  }

  return retVal;
}
#endif

#if(PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_MainFunctionRx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_MainFunctionRx(PduR_RmGDestRomIterType rmGDestRomIdx)
{
  if(PduR_IsRmBufferedIfPropertiesRomUsedOfRmGDestRom(rmGDestRomIdx) && (PduR_GetDirectionOfRmGDestRom(rmGDestRomIdx) == PDUR_RX_DIRECTIONOFRMGDESTROM))
  {
    PduR_Lock_LockRoutingPathByGDestPdu(rmGDestRomIdx);
    {
      PduInfoType destPduInfo = {NULL_PTR, 0u};
      PduR_RmDestRomIterType rmDestRomIdx = 0u;

      while(PduR_QAL_Get(rmGDestRomIdx, &rmDestRomIdx, &destPduInfo) == E_OK) /* FETA_PDUR_03 */
      {
        PduR_Mm_CallIfRxIndication(PduR_GetMmRomIdxOfRmGDestRom(rmGDestRomIdx), PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx), &destPduInfo);
        PduR_QAL_Remove(rmGDestRomIdx);
      }
    }
    PduR_Lock_UnlockRoutingPathByGDestPdu(rmGDestRomIdx);
  }
}
#endif

#if (PDUR_EXISTS_IF_BUFFERED_ROUTINGTYPEOFRMDESTROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_RmIf_QueueProcessing_DisableRoutingAction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_RmIf_QueueProcessing_DisableRoutingAction(PduR_RmDestRomIterType rmDestRomIdx)
{
  PduR_RmGDestRomIterType rmGDestRomIdx = PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx);
  PduR_RmBufferedIfPropertiesRomPartitionIdxOfRmGDestRomType memIdx = PduR_GetRmBufferedIfPropertiesRomPartitionIdxOfRmGDestRom(rmGDestRomIdx);

  switch (PduR_GetQueueTypeOfRmBufferedIfPropertiesRom(PduR_GetRmBufferedIfPropertiesRomIdxOfRmGDestRom(rmGDestRomIdx), memIdx))
  {
    case PDUR_SINGLE_BUFFER_QUEUETYPEOFRMBUFFEREDIFPROPERTIESROM:
      {
        PDUR_DUMMY_STATEMENT(rmGDestRomIdx);     /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
      }
      break;
    case PDUR_FIFO_QUEUETYPEOFRMBUFFEREDIFPROPERTIESROM:
      PduR_QAL_Flush(rmGDestRomIdx);
      break;
    default:   /* COV_PDUR_MISRA */
      /* Nothing to do: This should never be reached */
      break;
  }

  PduR_MultipleSourceHandler_Reset(rmGDestRomIdx);
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_RmIf_QueueProcessing.c
 *********************************************************************************************************************/
