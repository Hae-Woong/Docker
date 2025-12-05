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
/*!        \file  Com_Transmission.c
 *         \unit  Transmission
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_TRANSMISSION_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_Transmission.h"
#include "Com_Cfg_TransmissionProcessingData.h"
#include "Com_Cfg_Repetition.h"
#include "Com_Cfg_DescriptionGatewayData.h"
#include "Com_Cfg_TxBuffer.h"

#include "Com_TxBuffer.h"
#include "Com_TxSignalIf.h"
#include "Com_TxInv.h"
#include "Com_ISRThreshold.h"
#include "Com_TxCyclic.h"
#include "Com_IPduGroupHdlr.h"
#include "Com_Repetition.h"
#include "Com_TxMinDelay.h"
#include "Com_LLTxTp.h"
#include "Com_LLTxIf.h"
#include "Com_TxTransmit.h"
#include "Com_TxModeHdlr.h"
#include "Com_Notifications.h"

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
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Com_Transmission_InitSduLength
**********************************************************************************************************************/
/*! \brief        This function initializes the SduLength.
    \details      -
    \pre          -
    \param[in]    ComPduId Handle of the I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Transmission_InitSduLength(Com_TxPduInfoIterType ComPduId);

/**********************************************************************************************************************
  Com_Transmission_InitTxSignalGroup
**********************************************************************************************************************/
/*! \brief        This function initializes all parameters related to a Tx Signal Group.
    \details      -
    \pre          -
    \param[in]    ComPduId Handle of the I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Transmission_InitTxSignalGroup(Com_TxPduInfoIterType ComPduId);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Transmission_InitSduLength
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Transmission_InitSduLength(Com_TxPduInfoIterType ComPduId)
{
  /* If ComIPdu contains a dynamic length signal */
  if(Com_IsTxAccessInfoDynSigUsedOfTxPduInfo(ComPduId))
  {
    /* Initialize the ComIpdu length with the initial dynamic length */
    const Com_TxAccessInfoIterType txAccessInfoIdx = Com_GetTxAccessInfoDynSigIdxOfTxPduInfo(ComPduId);
    if(Com_IsDynamicInitialLengthUsedOfTxAccessInfo(txAccessInfoIdx))  /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
    {
      const Com_DynamicInitialLengthIterType dynamicInitialLengthIdx = Com_GetDynamicInitialLengthIdxOfTxAccessInfo(txAccessInfoIdx);
      if(Com_IsTxSigInfoUsedOfTxAccessInfo(txAccessInfoIdx))
      {
        Com_SetTxSduLength(ComPduId, (Com_TxSduLengthType) (Com_GetStartByteInPduPositionOfTxSigInfo(Com_GetTxSigInfoIdxOfTxAccessInfo(txAccessInfoIdx)) + ((Com_TxSduLengthType) Com_GetDynamicInitialLength(dynamicInitialLengthIdx))));
      }
      if(Com_IsTxGrpSigInfoUsedOfTxAccessInfo(txAccessInfoIdx))
      {
        Com_TxGrpSigInfoIterType txGrpSigIdx = Com_GetTxGrpSigInfoIdxOfTxAccessInfo(txAccessInfoIdx);
        Com_SetTxSduLength(ComPduId, (Com_TxSduLengthType) (Com_GetStartByteInPduPositionOfTxGrpSigInfo(txGrpSigIdx) + ((Com_TxSduLengthType) Com_GetDynamicInitialLength(dynamicInitialLengthIdx))));
        if(Com_IsTxTmpPduLengthUsedOfTxGrpSigInfo(txGrpSigIdx)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
        {
          Com_SetTxTmpPduLength(Com_GetTxTmpPduLengthIdxOfTxGrpSigInfo(txGrpSigIdx), (Com_TxSduLengthType) (Com_GetStartByteInPduPositionOfTxGrpSigInfo(txGrpSigIdx) + ((Com_TxSduLengthType) Com_GetDynamicInitialLength(dynamicInitialLengthIdx))));
        }      
      }
    }
  }
  /* otherwise */
  else
  {
    if(Com_IsTxBufferInfoUsedOfTxPduInfo(ComPduId))
    {
      const Com_TxBufferInfoIterType bufferInfoIdx = Com_GetTxBufferInfoIdxOfTxPduInfo(ComPduId);
      Com_SetTxSduLength(ComPduId, (Com_TxSduLengthType) Com_TxBuffer_GetTxBufferLength(bufferInfoIdx));
    }
    else
    {
      Com_SetTxSduLength(ComPduId, 0);
    }
  }
}

/**********************************************************************************************************************
  Com_Transmission_InitTxSignalGroup
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Transmission_InitTxSignalGroup(Com_TxPduInfoIterType ComPduId)
{
  /* Initialize all related Tx ComSignalGroup shadow buffer of the passed ComIPdu */
  for (Com_TxSigGrpInfoIndIterType idxTxSigGrpInfoInd = Com_GetTxSigGrpInfoIndStartIdxOfTxPduInfo(ComPduId);
       idxTxSigGrpInfoInd < Com_GetTxSigGrpInfoIndEndIdxOfTxPduInfo(ComPduId); idxTxSigGrpInfoInd++) /* FETA_COM_01 */
  {
    /*@ assert idxTxSigGrpInfoInd < Com_GetSizeOfTxSigGrpInfoInd(); */ /* VCA_COM_CSL03 */
    const Com_TxSigGrpInfoIterType sigGrpHnd = Com_GetTxSigGrpInfoInd(idxTxSigGrpInfoInd);

    Com_TxBuffer_InitShdwBuffer(sigGrpHnd);

    /* Clear the ComSignalGroup event flag */
    Com_SetSigGrpEventFlag(sigGrpHnd, COM_NOEVENT_SIGGRPEVENTFLAG);
  }
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Transmission_SendSignalGroup_Processing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Transmission_SendSignalGroup_Processing(Com_TxSigGrpInfoIterType idxTxSigGrpInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) ShadowBufferPtr)
{
  return Com_TxSignalIf_SendSignalGroup_Processing(idxTxSigGrpInfo, ShadowBufferPtr);
}

/**********************************************************************************************************************
  Com_Transmission_SendSignal_Processing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Transmission_SendSignal_Processing(Com_TxAccessInfoIterType txAccessInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  return Com_TxSignalIf_SendSignal_Processing(txAccessInfo, comDataPtr);
}

/**********************************************************************************************************************
  Com_Transmission_SendSignalGroupArray_Processing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Transmission_SendSignalGroupArray_Processing(Com_TxSigGrpInfoIterType SignalGroupId, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr)
{
  return Com_TxSignalIf_SendSignalGroupArray_Processing(SignalGroupId, SignalGroupArrayPtr);
}

/**********************************************************************************************************************
  Com_Transmission_InvalidateSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Transmission_InvalidateSignal(Com_TxAccessInfoIterType txAccessInfo)
{
  return Com_TxInv_InvalidateSignal(txAccessInfo);
}

/**********************************************************************************************************************
  Com_Transmission_InvalidateSignalGroup
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Transmission_InvalidateSignalGroup(Com_TxSigGrpInfoIterType SignalGroupId)
{
  return Com_TxInv_InvalidateSignalGroup(SignalGroupId);
}

/**********************************************************************************************************************
  Com_Transmission_TriggerIpduSend
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_TriggerIpduSend(Com_TxPduInfoIterType ComTxPduId)
{
  Com_TxTransmit_TriggerIpduSend(ComTxPduId);
}

/**********************************************************************************************************************
  Com_Transmission_TriggerIpduSendWithMetaData
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_TriggerIpduSendWithMetaData(Com_TxPduInfoIterType ComTxPduId, P2CONST(uint8, AUTOMATIC, AUTOMATIC) MetaData)
{
  Com_TxTransmit_TriggerIpduSendWithMetaData(ComTxPduId, MetaData);
}

/**********************************************************************************************************************
  Com_Transmission_TxConfirmationProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_TxConfirmationProcessing(Com_TxPduInfoIterType TxPduId)
{
  Com_LLTxIf_TxConfirmationProcessing(TxPduId);
}

/**********************************************************************************************************************
  Com_Transmission_TriggerTransmitProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_Transmission_TriggerTransmitProcessing(Com_TxPduInfoIterType TxPduId, CONSTP2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  return Com_LLTxIf_TriggerTransmitProcessing(TxPduId, PduInfoPtr);
}

/**********************************************************************************************************************
  Com_Transmission_CopyTxDataProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(BufReq_ReturnType, COM_CODE) Com_Transmission_CopyTxDataProcessing(Com_TxPduInfoIterType PduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                                  CONSTP2CONST(RetryInfoType, AUTOMATIC, AUTOMATIC) RetryInfoPtr, CONSTP2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) TxDataCntPtr)
{
  return Com_LLTxTp_CopyTxDataProcessing(PduId, PduInfoPtr, RetryInfoPtr, TxDataCntPtr);
}

/**********************************************************************************************************************
  Com_Transmission_TpTxConfirmationProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_TpTxConfirmationProcessing(Com_TxPduInfoIterType PduId, Std_ReturnType Result)
{
  Com_LLTxTp_TpTxConfirmationProcessing(PduId, Result);
}

/**********************************************************************************************************************
  Com_Transmission_MainFunctionProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_MainFunctionProcessing(Com_MainFunctionTxStructIterType mainFunctionId)
{
  const Com_ISRThresholdInfoIterType txProcessingISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfMainFunctionTxStruct(mainFunctionId);

  Com_ISRThreshold_EnterExclusiveArea(txProcessingISRLockCounterIdx);

  /* Iterate over all Tx Pdu Infos */
  for(Com_TxProcessingPdusOfMainFunctionTxIndIterType txProcessingPduIndIdx = Com_GetTxProcessingPdusOfMainFunctionTxIndStartIdxOfMainFunctionTxStruct(mainFunctionId);
      txProcessingPduIndIdx < Com_GetTxProcessingPdusOfMainFunctionTxIndEndIdxOfMainFunctionTxStruct(mainFunctionId); txProcessingPduIndIdx++)  /* FETA_COM_01 */
  {
    /*@ assert txProcessingPduIndIdx < Com_GetSizeOfTxProcessingPdusOfMainFunctionTxInd(); */ /* VCA_COM_CSL03 */
    const Com_TxProcessingPdusIterType ComTxProcessingPdusIterator = Com_GetTxProcessingPdusOfMainFunctionTxInd(txProcessingPduIndIdx);
    const Com_TxPduInfoIterType ComTxPduId = Com_GetTxPduInfoIdxOfTxProcessingPdus(ComTxProcessingPdusIterator);

    /* If the Tx ComIPdu is active, process the cyclic transmission mode, the repetition counter and the MDT counter */
    if(Com_IPduGroupHdlr_IsTxPduActive(ComTxPduId))
    {
      Com_TxCyclic_ProcessCyclicPdu(ComTxPduId);

      if(Com_IsRepetitionInfoUsedOfTxPduInfo(ComTxPduId))
      {
        const Com_RepetitionInfoIterType repetitionInfoIdx = Com_GetRepetitionInfoIdxOfTxPduInfo(ComTxPduId);
        /*@ assert repetitionInfoIdx < Com_GetSizeOfRepetitionInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
        Com_Repetition_ProcessRepetitions(repetitionInfoIdx);
      }

      Com_TxMinDelay_DecCounter(ComTxPduId);
    }

    Com_ISRThreshold_ThresholdCheck(txProcessingISRLockCounterIdx);
  }

  Com_ISRThreshold_ExitExclusiveArea(txProcessingISRLockCounterIdx);
}

/**********************************************************************************************************************
  Com_Transmission_MainFunctionTxTransmit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_MainFunctionTxTransmit(Com_MainFunctionTxStructIterType mainFunctionId)
{
  Com_TxTransmit_MainFunctionTx(mainFunctionId);
}

/**********************************************************************************************************************
  Com_Transmission_InitTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_InitTx(void)
{
  /* Iterate over all Tx ComIPdus and initialize all related parameters */
  for(Com_TxPduInfoIterType ComTxPduId = 0u; ComTxPduId < Com_GetSizeOfTxPduInfo(); ComTxPduId++)  /* FETA_COM_01 */
  {
    if(!Com_IsInvalidHndOfTxPduInfo(ComTxPduId))
    {
      Com_Transmission_InitTxIpdu(ComTxPduId);

      Com_TxTransmit_ClearInitialTransmitFlag(ComTxPduId);

      Com_LLTxTp_ResetTxTpConnection(ComTxPduId);

      /* The IPdu must be deactivated after all initialization tasks of this IPdu! */
      /*@ assert ComTxPduId < Com_GetSizeOfTxPduInfo(); */ /* VCA_COM_CSL01 */
      Com_IPduGroupHdlr_DeactivateTxPduActive(ComTxPduId);
    }
  }

  /* Activate Tx ComIPdus without assigned ComIPduGroups */
  Com_IPduGroupHdlr_ActivateAlwaysOnTxComIPdus();

  /* Initialize the Repetition counters */
  Com_Repetition_Init();
}

/**********************************************************************************************************************
  Com_Transmission_InitTxIpdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_InitTxIpdu(Com_TxPduInfoIterType ComPduId)
{
  /* Initialize SignalGroup parameters */
  Com_Transmission_InitTxSignalGroup(ComPduId);

  /* Initialize SduLength */
  Com_Transmission_InitSduLength(ComPduId);

  /* Initialize the ComIPdu TxBuffer */
  Com_TxBuffer_InitTxIpdu(ComPduId);

  /* Initialize filter states and Tx mode parameter */
  Com_TxModeHdlr_InitTxIpdu(ComPduId);

  /* Initialize HandleDeferred and WaitForConfirmation flags */
  Com_Notifications_InitTxIpdu(ComPduId);
}

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
Com_Transmission_StartCyclicTransmission
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_StartCyclicTransmission(Com_TxPduInfoIterType ComTxPduId)
{
  Com_TxCyclic_StartCyclicTransmission(ComTxPduId);
}
#endif

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
Com_Transmission_StopCyclicTransmission
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_StopCyclicTransmission(Com_TxPduInfoIterType ComTxPduId)
{
  Com_TxCyclic_StopCyclicTransmission(ComTxPduId);
}
#endif

/**********************************************************************************************************************
  Com_Transmission_StopRepetitions
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_StopRepetitions(Com_RepetitionInfoIterType repetitionInfoIdx)
{
  Com_Repetition_Stop(repetitionInfoIdx);
}

/**********************************************************************************************************************
  Com_Transmission_ClearTransmitRequest
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_ClearTransmitRequest(Com_TxPduInfoIterType ComTxPduId)
{
  Com_TxTransmit_ClearTransmitRequest(ComTxPduId);
}

# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_Transmission_DesGwTransmit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_DesGwTransmit(Com_TxPduInfoIterType ComTxPduId, boolean isImmediate, boolean withoutRepetitionFlag)
{
  Com_TxTransmit_DesGwTransmit(ComTxPduId, isImmediate, withoutRepetitionFlag);
}
# endif

/**********************************************************************************************************************
  Com_Transmission_ClearUpdateBits
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
***********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_ClearUpdateBits(Com_TxPduInfoIterType ComTxPduId)
{
  Com_TxBuffer_ClearUpdateBits(ComTxPduId);
}

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_Transmission_SetUpdateBitGwDescriptionAccess
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
***********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_SetUpdateBitGwDescriptionAccess(Com_GwDescriptionAccessInfoIterType idxGwDescriptionAccessInfo)
{
  Com_TxBuffer_SetUpdateBitGwDescriptionAccess(idxGwDescriptionAccessInfo);
}
#endif

/**********************************************************************************************************************
  Com_Transmission_GetAddrTxBufferFirstElement
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_TxBufferPtrType, COM_CODE) Com_Transmission_GetAddrTxBufferFirstElement(Com_TxBufferInfoIterType txBufferInfoIdx)
{
  return Com_TxBuffer_GetAddrTxBufferFirstElement(txBufferInfoIdx);
}

/**********************************************************************************************************************
  Com_Transmission_GetTxBufferLength
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_TxBufferLengthOfTxBufferInfoType, COM_CODE) Com_Transmission_GetTxBufferLength(Com_TxBufferInfoIterType txBufferInfoIdx)
{
  return Com_TxBuffer_GetTxBufferLength(txBufferInfoIdx);
}

/**********************************************************************************************************************
  Com_Transmission_CancelTransmit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_CancelTransmit(Com_TxPduInfoIterType ComTxPduId)
{
  Com_LLTxIf_CancelTransmit(ComTxPduId);
}

/**********************************************************************************************************************
  Com_Transmission_ResetTxTpConnection
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_ResetTxTpConnection(Com_TxPduInfoIterType ComTxPduId)
{
  Com_LLTxTp_ResetTxTpConnection(ComTxPduId);
}

/**********************************************************************************************************************
  Com_Transmission_StopTxIpdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_StopTxIpdus(Com_TxPduInfoIterType ComTxPduId)
{
  Com_TxModeHdlr_StopTxIpdus(ComTxPduId);
}

/**********************************************************************************************************************
  Com_Transmission_SwitchIpduTxMode
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Transmission_SwitchIpduTxMode(Com_TxPduInfoIterType ComTxPduId, boolean Mode)
{
  Com_TxModeHdlr_SwitchIpduTxMode(ComTxPduId, Mode);
}

/**********************************************************************************************************************
Com_Transmission_IsDirectTxMode
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Transmission_IsDirectTxMode(Com_TxPduInfoIterType TxPduId)
{
  return Com_TxModeHdlr_IsDirectTxMode(TxPduId);
}

/**********************************************************************************************************************
  Com_Transmission_IsPeriodicTxMode
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_Transmission_IsPeriodicTxMode(Com_TxPduInfoIterType TxPduId)
{
  return Com_TxModeHdlr_IsPeriodicTxMode(TxPduId);
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:

*/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

SBSW_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  END OF FILE: Com_Transmission.c
 *********************************************************************************************************************/
