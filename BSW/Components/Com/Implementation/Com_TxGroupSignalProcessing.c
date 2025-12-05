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
/*!        \file  Com_TxGroupSignalProcessing.c
 *         \unit  TxGroupSignalProcessing
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_TXGROUPSIGNALPROCESSING_SOURCE

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
#include "Com_TxGroupSignalProcessing.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_Serializer.h"
#include "Com_TxSignalFiltering.h"
#include "Com_IPduGroupHdlr.h"
#include "Com_LLTxTp.h"

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
  Com_TxGroupSignalProcessing_WriteGroupSignal
**********************************************************************************************************************/
/*! \brief        This method updates the group signal object identified by idxTxGrpSigInfo with the signal data
                  referenced by the SignalDataPtr parameter. The Length parameter is evaluated for dynamic length signals.
    \details      -
    \pre          -
    \param[in]    idxTxGrpSigInfo Index of Tx Group Signal Info.
    \param[in]    SignalDataPtr   Reference to the signal data to be transmitted.
    \param[in]    Length          Signal length, only required for dynamic length signals.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxGroupSignalProcessing_WriteGroupSignal(Com_TxGrpSigInfoIterType idxTxGrpSigInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  Com_TxGroupSignalProcessing_EvaluateGroupSignalTriggerConditionAndRequestTransmit
**********************************************************************************************************************/
/*! \brief        This method evaluates the trigger conditions according to the configured transferProperty and
                  and the currently active TxModeMode. The signal group flag is set to indicate a possible transmit.
    \details      -
    \pre          -
    \param[in]    idxTxGrpSigInfo   Index of TxGrpSigInfo
    \param[in]    newValuePtr       reference to the new signal value
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxGroupSignalProcessing_EvaluateGroupSignalTriggerConditionAndRequestTransmit(Com_TxGrpSigInfoIterType idxTxGrpSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr);

/**********************************************************************************************************************
  Com_TxGroupSignalProcessing_SetSigGrpEventFlag
**********************************************************************************************************************/
/*! \brief        This method wraps the function Com_SetSigGrpEventFlag in dependency of the input parameter TriggerWithoutRepetition
    \details      -
    \pre          -
    \param[in]    TriggerWithoutRepetition      TRUE if signal or group signal has any '*_WITHOUT_REPETITION' transfer property
    \param[in]    idxTxSigGrpInfo               Index of the Tx SigGrpInfo
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires idxTxSigGrpInfo < Com_GetSizeOfTxSigGrpInfo();
    \endspec
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxGroupSignalProcessing_SetSigGrpEventFlag(Com_TxSigGrpInfoIterType idxTxSigGrpInfo, boolean TriggerWithoutRepetition);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxGroupSignalProcessing_SetSigGrpEventFlag
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxGroupSignalProcessing_SetSigGrpEventFlag(Com_TxSigGrpInfoIterType idxTxSigGrpInfo, boolean TriggerWithoutRepetition)
{
  /* If the passed Tx ComSignalGroup has a _WITHOUT_REPETITION transfer property, set the WITHOUTREPETITION signal group event flag */
  if(TriggerWithoutRepetition)
  {
    Com_SetSigGrpEventFlag(idxTxSigGrpInfo, COM_GROUPEVENTWITHOUTREPETITION_SIGGRPEVENTFLAG);
  }
  /* Otherwise, set the TRANSMIT and WITH_REPETITION signal group event flag */
  else
  {
    Com_SetSigGrpEventFlag(idxTxSigGrpInfo, COM_GROUPEVENT_SIGGRPEVENTFLAG);
  }
}

/**********************************************************************************************************************
  Com_TxGroupSignalProcessing_WriteGroupSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxGroupSignalProcessing_WriteGroupSignal(Com_TxGrpSigInfoIterType idxTxGrpSigInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  /* If the passed ComGroupSignal is a dynamic length signal, update the tmp length of the related ComIPdu */
  if(Com_IsTxTmpPduLengthUsedOfTxGrpSigInfo(idxTxGrpSigInfo))
  {
    Com_TxTmpPduLengthIdxOfTxGrpSigInfoType txTmpPduLengthIdx = Com_GetTxTmpPduLengthIdxOfTxGrpSigInfo(idxTxGrpSigInfo);
    Com_SetTxTmpPduLength(txTmpPduLengthIdx, (Com_TxSduLengthType) (Com_GetStartByteInPduPositionOfTxGrpSigInfo(idxTxGrpSigInfo) + (Com_TxSduLengthType) comDataPtr->dataLength));
  }
  /* Copy the passed signal data to the shadow buffer */
  Com_Serializer_WriteGroupSignal(idxTxGrpSigInfo, comDataPtr);
}

/**********************************************************************************************************************
  Com_TxGroupSignalProcessing_EvaluateGroupSignalTriggerConditionAndRequestTransmit
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
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxGroupSignalProcessing_EvaluateGroupSignalTriggerConditionAndRequestTransmit(Com_TxGrpSigInfoIterType idxTxGrpSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr)
{
  boolean TriggerWithoutRepetition = FALSE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  /* If the passed Tx ComGroupSignal has a configured WITHOUT_REPETITION transfer property, the transmission is performed without repetitions */
  if(Com_IsWithoutRepOfTxGrpSigInfo(idxTxGrpSigInfo))
  {
    TriggerWithoutRepetition = TRUE;
  }
  /* If the passed Tx ComGroupSignal has a triggered transfer property */
  if(Com_IsTriggeredOfTxGrpSigInfo(idxTxGrpSigInfo))
  {
    /* Set the signal group event flag */
    Com_TxGroupSignalProcessing_SetSigGrpEventFlag(Com_GetTxSigGrpInfoIdxOfTxGrpSigInfo(idxTxGrpSigInfo), TriggerWithoutRepetition);
  } /* Else If the passed Tx ComSignal has a triggered on change transfer property and if the value has changed */
  else if(Com_TxSignalFiltering_SignalTransferPropertyGroupSignalFilterCheck(idxTxGrpSigInfo, SignalDataPtr))
  {
    /* Set the signal group event flag */
    Com_TxGroupSignalProcessing_SetSigGrpEventFlag(Com_GetTxSigGrpInfoIdxOfTxGrpSigInfo(idxTxGrpSigInfo), TriggerWithoutRepetition);
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
  Com_TxGroupSignalProcessing_ProcessGroupSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_TxGroupSignalProcessing_ProcessGroupSignal(Com_TxGrpSigInfoIterType txGrpSigId, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  const Com_TxPduInfoIterType idxTxPduInfo = Com_GetTxPduInfoIdxOfTxGrpSigInfo(txGrpSigId);
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
    /* Check the filter state of the passed Tx ComGroupSignal */
    Com_TxSignalFiltering_TransmissionModeGroupSignalFilterCheck(txGrpSigId, comDataPtr->dataPtr);

    /* Write the passed Group Signal data */
    Com_TxGroupSignalProcessing_WriteGroupSignal(txGrpSigId, comDataPtr);

    /* If the Tx ComIPdu is active, evaluate the trigger conditions, otherwise return COM_SERVICE_NOT_AVAILABLE */
    if(Com_IPduGroupHdlr_IsTxPduActive(idxTxPduInfo))
    {
      Com_TxGroupSignalProcessing_EvaluateGroupSignalTriggerConditionAndRequestTransmit(txGrpSigId, comDataPtr->dataPtr);
    }
    else
    {
      retVal = COM_SERVICE_NOT_AVAILABLE;
    }

    /* Update the filter old value of the group signal */
    /*@ assert txGrpSigId < Com_GetSizeOfTxGrpSigInfo(); */ /* VCA_COM_CSL03 */
    Com_TxSignalFiltering_SetTxGroupSignalFilterOldValueX(txGrpSigId, comDataPtr->dataPtr);
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_TxGroupSignalProcessing_GetTxTmpPduLength
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(Com_TxSduLengthType, COM_CODE) Com_TxGroupSignalProcessing_GetTxTmpPduLength(Com_TxAccessInfoIterType txAccessInfoIdx)
{
  Com_TxSduLengthType txTmpPduLength = 0u;
  if(Com_IsTxGrpSigInfoUsedOfTxAccessInfo(txAccessInfoIdx))
  {
    Com_TxGrpSigInfoIterType txGrpSigIdx = Com_GetTxGrpSigInfoIdxOfTxAccessInfo(txAccessInfoIdx);
    if(Com_IsTxTmpPduLengthUsedOfTxGrpSigInfo(txGrpSigIdx))
    {
      txTmpPduLength = Com_GetTxTmpPduLength(Com_GetTxTmpPduLengthIdxOfTxGrpSigInfo(txGrpSigIdx));
    }      
  }
  return txTmpPduLength;
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
 *  END OF FILE: Com_TxGroupSignalProcessing.c
 *********************************************************************************************************************/
