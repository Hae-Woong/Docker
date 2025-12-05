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
/*!        \file  Com_TxSignalIf.c
 *         \unit  TxSignalIf
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_TXSIGNALIF_SOURCE

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
#include "Com_TxSignalIf.h"
#include "Com_Cfg_TransmissionProcessingData.h"
#include "Com_Cfg_TxBuffer.h"

#include "Com_TxSignalProcessing.h"
#include "Com_TxGroupSignalProcessing.h"
#include "Com_TxBuffer.h"
#include "Com_TxModeHdlr.h"
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
  Com_TxSignalIf_IsTxSigGrpInfoWithoutRepetition
**********************************************************************************************************************/
/*! \brief        This function checks if the provided signal group does not permit repetitive transmission
                  (transferProperty is *WITHOUTREPETITION).
    \details      -
    \pre          -
    \param[in]    TxSigGrpInfoIndex  Id of SignalGroup
    \return       boolean
                    TRUE   if SignalGroup does not permit repetitive transmission.
                    FALSE  otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_TxSignalIf_IsTxSigGrpInfoWithoutRepetition(Com_TxSigGrpInfoIterType TxSigGrpInfoIndex);

/**********************************************************************************************************************
  Com_TxSignalIf_IsTxSigGrpInfoTriggered
**********************************************************************************************************************/
/*! \brief        This function checks if the transferProperty of the provided signal group can trigger a transmission
                  (not PENDING).
    \details      -
    \pre          -
    \param[in]    TxSigGrpInfoIndex  Id of SignalGroup
    \return       boolean
                    TRUE   if SignalGroup can trigger a transmission
                    FALSE  otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_TxSignalIf_IsTxSigGrpInfoTriggered(Com_TxSigGrpInfoIterType TxSigGrpInfoIndex);

/**********************************************************************************************************************
  Com_TxSignalIf_IsTxSigGrpInfoOnChange
**********************************************************************************************************************/
/*! \brief        This function checks if provided signal group has any *ON_CHANGE transferProperty.
    \details      -
    \pre          -
    \param[in]    TxSigGrpInfoIndex  Id of SignalGroup
    \return       boolean
                    TRUE   if SignalGroup has ON-CHANGE transferProperty.
                    FALSE  otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_TxSignalIf_IsTxSigGrpInfoOnChange(Com_TxSigGrpInfoIterType TxSigGrpInfoIndex);

/**********************************************************************************************************************
  Com_TxSignalIf_SendSignalGroup_TriggerEventProcessing
**********************************************************************************************************************/
/*! \brief        This method sets the internal transmit request flag and the I-PDU is transmitted once or multiple.
    \details      -
    \pre          -
    \param[in]    idxTxSigGrpInfo
    \param[in]    idxTxPduInfo
    \return       E_OK                        if service is accepted.
                  COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_TxSignalIf_SendSignalGroup_TriggerEventProcessing(Com_TxSigGrpInfoIterType idxTxSigGrpInfo, Com_TxPduInfoIterType idxTxPduInfo);

/**********************************************************************************************************************
  Com_TxSignalIf_SendSignalGroup_TransmissionModeEvaluation
 **********************************************************************************************************************/
/*! \brief        If the Tx-SignalGroup is configured to be filtered, this function evaluates the filters and updates the filter
 *                state.
    \details      -
    \pre          -
    \param[in]    idxTxPduInfo  ID of the Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
 ***********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxSignalIf_SendSignalGroup_TransmissionModeEvaluation(Com_TxPduInfoIterType idxTxPduInfo);

/**********************************************************************************************************************
  Com_TxSignalIf_SendSignalGroupArrayHasChanged
**********************************************************************************************************************/
/*! \brief        This function checks if the content of the SignalGroup in the group array has changed.
    \details      -
    \pre          -
    \param[in]    SignalGroupArrayPtr         Reference to the signal group array
    \param[in]    maskPtr                     Pointer to OnChange Mask
    \param[in]    pIpduBuf                    Pointer to iPduBuffer
    \param[in]    MaskLength                  Length of the mask
    \return       boolean
                    true                        if content has changed
                    false                       otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different SignalGroupArrayPtr
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_TxSignalIf_SendSignalGroupArrayHasChanged(CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr,
                                                                                       CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) maskPtr, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) pIpduBuf,
                                                                                       uint32 MaskLength);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxSignalIf_IsTxSigGrpInfoWithoutRepetition
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_TxSignalIf_IsTxSigGrpInfoWithoutRepetition(Com_TxSigGrpInfoIterType TxSigGrpInfoIndex)
{
  return (Com_GetTransferPropertyOfTxSigGrpInfo(TxSigGrpInfoIndex) == COM_TRIGGERED_WITHOUT_REPETITION_TRANSFERPROPERTYOFTXSIGGRPINFO);
}

/**********************************************************************************************************************
  Com_TxSignalIf_IsTxSigGrpInfoTriggered
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_TxSignalIf_IsTxSigGrpInfoTriggered(Com_TxSigGrpInfoIterType TxSigGrpInfoIndex)
{
  return (Com_GetTransferPropertyOfTxSigGrpInfo(TxSigGrpInfoIndex) != COM_PENDING_TRANSFERPROPERTYOFTXSIGGRPINFO);
}

/**********************************************************************************************************************
  Com_TxSignalIf_IsTxSigGrpInfoOnChange
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_TxSignalIf_IsTxSigGrpInfoOnChange(Com_TxSigGrpInfoIterType TxSigGrpInfoIndex)
{
  return ((Com_GetTransferPropertyOfTxSigGrpInfo(TxSigGrpInfoIndex) == COM_TRIGGERED_ON_CHANGE_TRANSFERPROPERTYOFTXSIGGRPINFO) ||
          (Com_GetTransferPropertyOfTxSigGrpInfo(TxSigGrpInfoIndex) == COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION_TRANSFERPROPERTYOFTXSIGGRPINFO));
}

/**********************************************************************************************************************
  Com_TxSignalIf_SendSignalGroup_TriggerEventProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_TxSignalIf_SendSignalGroup_TriggerEventProcessing(Com_TxSigGrpInfoIterType idxTxSigGrpInfo, Com_TxPduInfoIterType idxTxPduInfo)
{
  uint8 retVal = E_OK;          /* PRQA S 2981 */ /* MD_MSR_RetVal */
  /* If the passed Tx ComIPdu is active */
  if(Com_IPduGroupHdlr_IsTxPduActive(idxTxPduInfo))
  {
    /* If at least one related ComGroupSignal was triggered, set the deferred transmit event */
    if(Com_GetSigGrpEventFlag(idxTxSigGrpInfo) != COM_NOEVENT_SIGGRPEVENTFLAG)
    {
      boolean TriggerWithoutRepetition;
      /* If at least one ComGroupSignal with WITHOUTREPETITION transfer property was triggered, trigger the transmission without repetitions */
      if(Com_GetSigGrpEventFlag(idxTxSigGrpInfo) == COM_GROUPEVENTWITHOUTREPETITION_SIGGRPEVENTFLAG)
      {
        TriggerWithoutRepetition = TRUE;
      }
      else
      {
        TriggerWithoutRepetition = FALSE;
      }

      Com_TxTransmit_TriggerDeferredTransmitAndSetTimer(idxTxPduInfo, TriggerWithoutRepetition);
      Com_SetSigGrpEventFlag(idxTxSigGrpInfo, COM_NOEVENT_SIGGRPEVENTFLAG);
    }
    /* Return E_OK */
  }
  /* Otherwise, return COM_SERVICE_NOT_AVAILABLE */
  else
  {
    retVal = COM_SERVICE_NOT_AVAILABLE;
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_TxSignalIf_SendSignalGroup_TransmissionModeEvaluation
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 ***********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxSignalIf_SendSignalGroup_TransmissionModeEvaluation(Com_TxPduInfoIterType idxTxPduInfo)
{
  if(Com_TxSignalFiltering_HasTransmissionModeFilter(idxTxPduInfo))
  {
    Com_TxModeHdlr_UpdateTMS(idxTxPduInfo);
  }
}

/**********************************************************************************************************************
  Com_TxSignalIf_SendSignalGroupArrayHasChanged
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_TxSignalIf_SendSignalGroupArrayHasChanged(CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr,
                                                                                       CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) maskPtr, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) pIpduBuf,
                                                                                       uint32 MaskLength)
{
  boolean retVal = FALSE;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  /* Bytewise comparison of signal group array and ipdu-buffer, if any change has occurred */
  for(uint32 i = 0; i < MaskLength; i++)    /* FETA_COM_01 */
  {
    /* Bits that to do not belong to a group signal with a certain transfer property are masked out with the provided mask. */
    if((pIpduBuf[i] & maskPtr[i]) != (SignalGroupArrayPtr[i] & maskPtr[i]))
    {
      retVal = TRUE;
      break;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxSignalIf_SendSignalGroup_Processing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_TxSignalIf_SendSignalGroup_Processing(Com_TxSigGrpInfoIterType idxTxSigGrpInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) ShadowBufferPtr)
{
  const Com_TxPduInfoIterType idxTxPduInfo = Com_GetTxPduInfoIdxOfTxSigGrpInfo(idxTxSigGrpInfo);
  uint8 retVal = E_OK;          /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* Evaluate the transition mode */
  Com_TxSignalIf_SendSignalGroup_TransmissionModeEvaluation(idxTxPduInfo);

  /* Set the Update Bit */
  Com_TxBuffer_SetUpdateBitSignalGroup(idxTxSigGrpInfo);

  /* If the passed Tx ComIPdu is a Tp Pdu */
  if(Com_LLTxTp_IsTpUsed(idxTxPduInfo))
  {
    /* If the Tp ComIPdu is in READY state, update the shadow buffer and set the Tx ComIPdu length to the stored tmp length for dynamic length signals */
    if(Com_LLTxTp_IsTpStateReady(idxTxPduInfo))
    {
      Com_TxBuffer_WriteSignalGroup(idxTxSigGrpInfo, ShadowBufferPtr);
      if(Com_IsTxAccessInfoDynSigUsedOfTxPduInfo(idxTxPduInfo))
      {
        Com_SetTxSduLength(idxTxPduInfo, Com_TxGroupSignalProcessing_GetTxTmpPduLength(Com_GetTxAccessInfoDynSigIdxOfTxPduInfo(idxTxPduInfo)));
      }
    }
    /* Otherwise, return COM_BUSY */
    else
    {
      retVal = COM_BUSY;
    }
  }
  else
  {
    /* Else, Copy the shadow buffer to the related Tx ComIPdu buffer */
    Com_TxBuffer_WriteSignalGroup(idxTxSigGrpInfo, ShadowBufferPtr);
  }
  if(retVal == E_OK)
  {
    /* Do trigger event processing */
    retVal = Com_TxSignalIf_SendSignalGroup_TriggerEventProcessing(idxTxSigGrpInfo, idxTxPduInfo);
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_TxSignalIf_SendSignal_Processing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_TxSignalIf_SendSignal_Processing(Com_TxAccessInfoIterType txAccessInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  uint8 retVal = E_OK;          /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_IsTxSigInfoUsedOfTxAccessInfo(txAccessInfo))
  {
    const Com_TxSigInfoIterType txSignalId = Com_GetTxSigInfoIdxOfTxAccessInfo(txAccessInfo);
    /*@ assert (txSignalId  < Com_GetSizeOfTxSigInfo()); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    retVal = Com_TxSignalProcessing_ProcessSignal(txSignalId, comDataPtr);
  }
  else if(Com_IsTxGrpSigInfoUsedOfTxAccessInfo(txAccessInfo))
  {
    const Com_TxGrpSigInfoIterType txGrpSigId = Com_GetTxGrpSigInfoIdxOfTxAccessInfo(txAccessInfo);
    /*@ assert (txGrpSigId  < Com_GetSizeOfTxGrpSigInfo()); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    retVal = Com_TxGroupSignalProcessing_ProcessGroupSignal(txGrpSigId, comDataPtr);
  }
  else
  {
    /* Intentionally Empty */
  }

  return retVal;
}

/**********************************************************************************************************************
  Com_TxSignalIf_SendSignalGroupArray_Processing
**********************************************************************************************************************/
/*! Internal comment removed.
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
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_TxSignalIf_SendSignalGroupArray_Processing(Com_TxSigGrpInfoIterType SignalGroupId, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr) /* PRQA S 6080 */ /* MD_MSR_STMIF */
{
  /* Evaluate and update the filter state */
  Com_TxSignalFiltering_SignalGroupArrayEvaluateFilter(SignalGroupId, SignalGroupArrayPtr);

  /* If TRIGGERED BIT is set for the signal group */
  if(Com_TxSignalIf_IsTxSigGrpInfoTriggered(SignalGroupId))
  {
    boolean doTrigger = FALSE;
    boolean withoutRepetition = TRUE;   /* Will only be evaluated for COM_WITHOUTREPOFTXSIGINFO == STD_ON */

    /* If transmission property of signal group equals TRIGGERED_ON_CHANGE or TRIGGERED_ON_CHANGE_WITHOUT_REPETITION */
    if(Com_TxSignalIf_IsTxSigGrpInfoOnChange(SignalGroupId))
    {
      const Com_TxPduInfoIterType ipdu = Com_GetTxPduInfoIdxOfTxSigGrpInfo(SignalGroupId); /* PRQA S 2983 */ /* MD_Com_2982_2983 */

      /* If signal group contains group signals with transmission property TRIGGERED_ON_CHANGE (if COM_TXSIGGRPONCHANGEMASKONCHANGEUSEDOFTXSIGGRPINFO is enabled) */
      if(Com_IsTxSigGrpOnChangeMaskOnChangeUsedOfTxSigGrpInfo(SignalGroupId) && Com_IsTxBufferInfoUsedOfTxPduInfo(ipdu)) /* COV_COM_BUFFER_ALWAYS_USED_IF_TRIGGERED_ON_CHANGE_IS_USED */
      {
        const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoIdxOfTxPduInfo(ipdu);
        if(Com_GetSigGroupOnChangeStartPositionOfTxSigGrpInfo(SignalGroupId) < Com_TxBuffer_GetTxBufferLength(txBufferInfoIdx))
        {
          /* Get the indirection to the mask to evaluate if any of the contained group signals has changed */
          CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) maskPtr = Com_GetAddrTxSigGrpOnChangeMask(Com_GetTxSigGrpOnChangeMaskOnChangeStartIdxOfTxSigGrpInfo(SignalGroupId));

          /* Calculate the position of the first group signal with the transmission property TRIGGERED_ON_CHANGE within the ipdu buffer to compare the stored values */
          /*@ assert txBufferInfoIdx < Com_GetSizeOfTxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
          CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) pIpduBuf = Com_TxBuffer_GetAddrTxBufferAtIdx(txBufferInfoIdx, Com_GetSigGroupOnChangeStartPositionOfTxSigGrpInfo(SignalGroupId));

          /* Get the length of the triggered on change mask */
          const Com_TxSigGrpOnChangeMaskOnChangeLengthOfTxSigGrpInfoType MaskLength = Com_GetTxSigGrpOnChangeMaskOnChangeLengthOfTxSigGrpInfo(SignalGroupId);

          /* Perform check if any group signal with transmission property TRIGGERED_ON_CHANGE has changed and set transmission initiation flag accordingly */
          doTrigger = Com_TxSignalIf_SendSignalGroupArrayHasChanged(&(SignalGroupArrayPtr[Com_GetSigGroupOnChangeOffsetOfTxSigGrpInfo(SignalGroupId)]), maskPtr, pIpduBuf, MaskLength);
        }
      }
      /* If no group signal with transmission property TRIGGERED_ON_CHANGE has changed (if COM_TXSIGGRPONCHANGEMASKONCHANGEWITHOUTREPUSEDOFTXSIGGRPINFO is enabled) */
      if(doTrigger == FALSE)
      {
        /* If signal group contains any group signal with transmission property TRIGGERED_ON_CHANGE_WITHOUT_REPETITION */
        if(Com_IsTxSigGrpOnChangeMaskOnChangeWithoutRepUsedOfTxSigGrpInfo(SignalGroupId) && Com_IsTxBufferInfoUsedOfTxPduInfo(ipdu)) /* COV_COM_BUFFER_ALWAYS_USED_IF_TRIGGERED_ON_CHANGE_IS_USED */
        {
          const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoIdxOfTxPduInfo(ipdu);
        if(Com_GetSigGroupOnChangeWithoutRepStartPositionOfTxSigGrpInfo(SignalGroupId) < Com_TxBuffer_GetTxBufferLength(txBufferInfoIdx))
        {
            /* Get the indirection to the mask to evaluate if any of the contained group signals with transmission property TRIGGERED_ON_CHANGE_WITHOUT_REPETITION has changed */
            CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) maskPtr = Com_GetAddrTxSigGrpOnChangeMask(Com_GetTxSigGrpOnChangeMaskOnChangeWithoutRepStartIdxOfTxSigGrpInfo(SignalGroupId));

            /* Calculate the position of the first group signal with the transmission property TRIGGERED_ON_CHANGE_WITHOUT_REPETITION within the ipdu buffer to compare the stored values */
            /*@ assert txBufferInfoIdx < Com_GetSizeOfTxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
            CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) pIpduBuf = Com_TxBuffer_GetAddrTxBufferAtIdx(txBufferInfoIdx, Com_GetSigGroupOnChangeWithoutRepStartPositionOfTxSigGrpInfo(SignalGroupId));

            /* Get the length of the triggered on change without repetition mask */
            const Com_TxSigGrpOnChangeMaskOnChangeWithoutRepLengthOfTxSigGrpInfoType MaskLength = Com_GetTxSigGrpOnChangeMaskOnChangeWithoutRepLengthOfTxSigGrpInfo(SignalGroupId);

            /* Perform check if any group signal with transmission property TRIGGERED_ON_CHANGE_WITHOUT_REPETITION has changed and set transmission initiation flag accordingly */
            doTrigger = Com_TxSignalIf_SendSignalGroupArrayHasChanged(&(SignalGroupArrayPtr[Com_GetSigGroupOnChangeWithoutRepOffsetOfTxSigGrpInfo(SignalGroupId)]), maskPtr, pIpduBuf, MaskLength);
          }
        }
      }
      else
      {
        /* Otherwise set the without repetition flag to false to indicate that transmission should be performed with configured amount of repetitions */
        withoutRepetition = FALSE;
      }
    }
    /* Otherwise transmission property is assumed to be TRIGGERED or TRIGGERED_WITHOUT_REPETITION */
    else
    {
      /* set transmission initiation flag to true */
      doTrigger = TRUE;
      /* set without repetition flag accordingly to the configured transmission property (if configured) */
      withoutRepetition = Com_TxSignalIf_IsTxSigGrpInfoWithoutRepetition(SignalGroupId);
    }

    /* If transmission is requested, set group event flag */
    if(doTrigger)
    {
      /* If the passed Tx ComSignalGroup has a _WITHOUT_REPETITION transfer property, set the WITHOUTREPETITION signal group event flag */
      if(withoutRepetition)
      {
        Com_SetSigGrpEventFlag(SignalGroupId, COM_GROUPEVENTWITHOUTREPETITION_SIGGRPEVENTFLAG);
      }
      /* Otherwise, set the TRANSMIT and WITH_REPETITION signal group event flag */
      else
      {
        Com_SetSigGrpEventFlag(SignalGroupId, COM_GROUPEVENT_SIGGRPEVENTFLAG);
      }
    }
  }
  /* Copy the content of the associated signal group array to the associated I-PDU buffer (if COM_TXSIGGRPMASK enabled). */
  return Com_TxSignalIf_SendSignalGroup_Processing(SignalGroupId, SignalGroupArrayPtr);
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
 *  END OF FILE: Com_TxSignalIf.c
 *********************************************************************************************************************/
