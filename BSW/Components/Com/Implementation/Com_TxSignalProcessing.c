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
/*!        \file  Com_TxSignalProcessing.c
 *         \unit  TxSignalProcessing
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_TXSIGNALPROCESSING_SOURCE

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
#include "Com.h"
#include "Com_TxSignalProcessing.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_TxBuffer.h"
#include "Com_TxModeHdlr.h"
#include "Com_Serializer.h"
#include "Com_TxSignalFiltering.h"
#include "Com_IPduGroupHdlr.h"
#include "Com_LLTxTp.h"
#include "Com_TxTransmit.h"

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
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Com_TxSignalProcessing_TransmissionModeEvaluation
**********************************************************************************************************************/
/*! \brief        If the Tx-Signal is configured to be filtered, this function evaluates the filters and updates the filter
 *                state.
    \details      -
    \pre          -
    \param[in]    idxTxSigInfo  ID of the Tx signal.
    \param[in]    newValuePtr   Pointer to the new value
    \param[in]    idxTxPduInfo  ID of Tx I-PDU.
    \\context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxSignalProcessing_TransmissionModeEvaluation(Com_TxSigInfoIterType idxTxSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr,
                                                                                        Com_TxPduInfoIterType idxTxPduInfo);

/**********************************************************************************************************************
  Com_TxSignalProcessing_WriteSignal
**********************************************************************************************************************/
/*! \brief        This method updates the signal object identified by idxTxSigInfo with the signal data referenced by the
                  SignalDataPtr parameter. The Length parameter is evaluated for dynamic length signals.
    \details      -
    \pre          -
    \param[in]    idxTxSigInfo    Index of Tx Signal Info
    \param[in]    SignalDataPtr   Reference to the signal data to be transmitted.
    \param[in]    Length          Signal length, only required for dynamic length signals.
    \param[in]    idxTxPduInfo    ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxSignalProcessing_WriteSignal(Com_TxSigInfoIterType idxTxSigInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr, Com_TxPduInfoIterType idxTxPduInfo);

/**********************************************************************************************************************
  Com_TxSignalProcessing_EvaluateSignalTriggerConditionAndRequestTransmit
**********************************************************************************************************************/
/*! \brief        This method evaluates the trigger conditions according to the configured transferProperty and
                  and the currently active TxModeMode. On valid trigger conditions a transmit request is set.
    \details      -
    \pre          -
    \param[in]    idxTxSigInfo      Index of TxSigInfo
    \param[in]    newValuePtr       reference to the new signal value
    \param[in]    idxTxPduInfo      Index of TxPduInfo
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxSignalProcessing_EvaluateSignalTriggerConditionAndRequestTransmit(Com_TxSigInfoIterType idxTxSigInfo,
                                                                                                              P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr,
                                                                                                              Com_TxPduInfoIterType idxTxPduInfo);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxSignalProcessing_TransmissionModeEvaluation
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 ***********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxSignalProcessing_TransmissionModeEvaluation(Com_TxSigInfoIterType idxTxSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr,
                                                                                        Com_TxPduInfoIterType idxTxPduInfo)
{
  if(Com_TxSignalFiltering_TransmissionModeSignalFilterCheck(idxTxSigInfo, SignalDataPtr))
  {
    Com_TxModeHdlr_UpdateTMS(idxTxPduInfo);
  }
}

/**********************************************************************************************************************
  Com_TxSignalProcessing_WriteSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxSignalProcessing_WriteSignal(Com_TxSigInfoIterType idxTxSigInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr, Com_TxPduInfoIterType idxTxPduInfo)
{
  /* If the passed ComSignal has an UpdateBit, set the UpdateBit in the Tx ComIPdu buffer */
  Com_TxBuffer_SetUpdateBitSignal(idxTxSigInfo);

  /* If the passed ComSignal is a dynamic length signal, update the tmp and the normal Tx ComIPdu length */
  if(Com_LLTxTp_IsTpUsed(idxTxPduInfo))
  {
    if(Com_IsDynamicInitialLengthUsedOfTxAccessInfo(Com_GetTxAccessInfoIdxOfTxSigInfo(idxTxSigInfo)))
    {
      Com_SetTxSduLength(idxTxPduInfo, (Com_TxSduLengthType) (Com_GetStartByteInPduPositionOfTxSigInfo(idxTxSigInfo) + (Com_TxSduLengthType) comDataPtr->dataLength));
    }
  }
  /* If a TxBuffer exists for the TxSignal, copy the passed signal data to the Tx ComIPdu buffer */
  Com_Serializer_WriteSignal(idxTxSigInfo, comDataPtr);
}

/**********************************************************************************************************************
  Com_TxSignalProcessing_EvaluateSignalTriggerConditionAndRequestTransmit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxSignalProcessing_EvaluateSignalTriggerConditionAndRequestTransmit(Com_TxSigInfoIterType idxTxSigInfo,
                                                                                                              P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr,
                                                                                                              Com_TxPduInfoIterType idxTxPduInfo)
{
  boolean TriggerWithoutRepetition = FALSE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  /* If the passed Tx ComSignal has a configured WITHOUT_REPETITION transfer property, the transmission is performed without repetitions */
  if(Com_IsWithoutRepOfTxSigInfo(idxTxSigInfo))
  {
    TriggerWithoutRepetition = TRUE;
  }
  /* If the passed Tx ComSignal has a triggered transfer property */
  if(Com_IsTriggeredOfTxSigInfo(idxTxSigInfo))
  {
    /* Trigger the deferred ComIPdu transmission */
    Com_TxTransmit_TriggerDeferredTransmitAndSetTimer(idxTxPduInfo, TriggerWithoutRepetition);
  } /* Else If the passed Tx ComSignal has a triggered on change transfer property and if the value has changed */
  else if(Com_TxSignalFiltering_SignalTransferPropertySignalFilterCheck(idxTxSigInfo, SignalDataPtr))
  {
    /* Trigger the deferred ComIPdu transmission */
    Com_TxTransmit_TriggerDeferredTransmitAndSetTimer(idxTxPduInfo, TriggerWithoutRepetition);
  }
  else
  {
    /* Intentionally Empty */
  }
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxSignalProcessing_ProcessSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_TxSignalProcessing_ProcessSignal(Com_TxSigInfoIterType txSignalId, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  const Com_TxPduInfoIterType idxTxPduInfo = Com_GetTxPduInfoIdxOfTxSigInfo(txSignalId);
  uint8 retVal = E_OK;          /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* If the passed Tx ComIPdu is a Tp Pdu, check if the Tp connection is in READY state, if not return COM_BUSY */
  if(Com_LLTxTp_IsTpUsed(idxTxPduInfo))
  {
    if(!Com_LLTxTp_IsTpStateReady(idxTxPduInfo))
    {
      /* \trace SPEC-2736898 */ /* \trace SPEC-2736899 */
      retVal = COM_BUSY;
    }
  }

  if(retVal == E_OK)
  {
    /* Check the filter state of the passed Tx ComSignal and update the transition mode state if required */
    Com_TxSignalProcessing_TransmissionModeEvaluation(txSignalId, comDataPtr->dataPtr, idxTxPduInfo);

    /* Write the passed Signal data */
    Com_TxSignalProcessing_WriteSignal(txSignalId, comDataPtr, idxTxPduInfo);

    /* If the Tx ComIPdu is active, evaluate the trigger conditions, otherwise return COM_SERVICE_NOT_AVAILABLE */
    if(Com_IPduGroupHdlr_IsTxPduActive(idxTxPduInfo))
    {
      Com_TxSignalProcessing_EvaluateSignalTriggerConditionAndRequestTransmit(txSignalId, comDataPtr->dataPtr, idxTxPduInfo);
    }
    else
    {
      retVal = COM_SERVICE_NOT_AVAILABLE;
    }

    /* Update the filter old value of the signal */
    /*@ assert txSignalId < Com_GetSizeOfTxSigInfo(); */ /* VCA_COM_CSL03 */
    Com_TxSignalFiltering_SetTxSignalFilterOldValueX(txSignalId, comDataPtr->dataPtr);
  }
  return retVal;
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
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: Com_TxSignalProcessing.c
 *********************************************************************************************************************/
