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
/*!        \file  Com_LLTxIf.c
 *         \unit  LLTxIf
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_LLTXIF_SOURCE

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
#include "Com_LLTxIf.h"
#include "Com_Cfg_TransmissionProcessingData.h"
#include "Com_Cfg_Repetition.h"
#include "Com_Cfg_Reporting.h"
#include "Com_Cfg_TxBuffer.h"

#include "Com_LLTxTp.h"
#include "Com_Reporting.h"
#include "Com_TxBuffer.h"
#include "Com_Notifications.h"
#include "Com_TxDlMon.h"
#include "Com_IPduGroupHdlr.h"
#include "Com_Repetition.h"
#include "Com_TxMinDelay.h"
#include "Com_TxTransmit.h"

# include "PduR_Com.h"

#include "vstdlib.h"
#include "SchM_Com.h"

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
Com_LLTxIf_SetTxAttributes
**********************************************************************************************************************/
/*! \brief        This function adjusts Tx-Parameter like Update-Bit, Delay Time Counter, Tp Connection State and
 *                Com_WaitingForConfirmation Parameter if the status of transmit request has been accepted.
    \details      -
    \pre          -
    \param[in]    status        Status if transmit request has been accepted or not
    \param[in]    ComTxPduId    Valid ID of Tx I-PDU to be transmitted.
    \trace        SPEC-2736857, SPEC-2736858, SPEC-2736898
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_LLTxIf_SetTxAttributes(Com_TxPduInfoIterType ComTxPduId, Std_ReturnType status);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
Com_LLTxIf_SetTxAttributes
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
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_LLTxIf_SetTxAttributes(Com_TxPduInfoIterType ComTxPduId, Std_ReturnType status)
{
  /* If the value of status is positive */
  if(status == E_OK)
  {
    /* If the Update Bit clear context is TRANSMIT, clear the Update Bits */
    if(Com_GetClrUbOfTxPduInfo(ComTxPduId) == COM_TRANSMIT_CLRUBOFTXPDUINFO)
    {
      Com_TxBuffer_ClearUpdateBits(ComTxPduId);
    }

    if(Com_TxTransmit_IsDirectTrigger(ComTxPduId))
    {
      /* \trace SPEC-2736857 */ /* \trace SPEC-2736858 */
      /* Reload the Minimum Delay timer */
      Com_TxMinDelay_StartCounter(ComTxPduId);
    }
    /* Set the WaitingForConfirmation flag */
    Com_SetWaitingForConfirmation(ComTxPduId, TRUE);

    Com_TxTransmit_ClearInitialTransmitFlag(ComTxPduId);
  }
  /* Otherwise */
  else
  {
    /* If the ComIPdu is a TP PDU, set the state to READY */
    /* \trace SPEC-2736898 */
    Com_LLTxTp_ResetTxTpConnection(ComTxPduId);

    /* Clear the WaitingForConfirmation flag */
    Com_SetWaitingForConfirmation(ComTxPduId, FALSE);
  }


#if (COM_STRICT_REPETITION_PERIOD == STD_ON)
  /* Reset the RepCycleCnt to avoid a potential shift (the time between repetitions being less than the configured value ComTxModeRepetitionPeriod) of the repetition period caused by the MDT. */
  if(Com_IsRepetitionInfoUsedOfTxPduInfo(ComTxPduId))
  {
    Com_Repetition_ResetRepCycle(Com_GetRepetitionInfoIdxOfTxPduInfo(ComTxPduId));
  }
#endif
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
Com_LLTxIf_Transmit
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
 *
**********************************************************************************************************************/
/* PRQA S 6010, 6030 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
FUNC(Std_ReturnType, COM_CODE) Com_LLTxIf_Transmit(Com_TxPduInfoIterType ComTxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  PduInfoType PduInfo = { NULL_PTR, 0u };

  /* If the ComIPdu is no zero byte PDU */
  if(Com_IsTxBufferInfoUsedOfTxPduInfo(ComTxPduId))
  {
    /* If the ComIPdu is a TP PDU AND has configured meta data */
    if(Com_LLTxTp_IsTpUsed(ComTxPduId) && Com_IsTxBufferInfoMetaDataUsedOfTxPduInfo(ComTxPduId))
    {
      /* Set the SduDataPtr to the meta data */
      const Com_TxBufferInfoIterType metaDataBufferIdx = Com_GetTxBufferInfoMetaDataIdxOfTxPduInfo(ComTxPduId);
      /*@ assert metaDataBufferIdx < Com_GetSizeOfTxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      PduInfo.SduDataPtr = Com_TxBuffer_GetAddrTxBufferFirstElement(metaDataBufferIdx);
    }
    else
    {
      /* otherwise, set the SduDataPtr to the PDU data buffer */
      const Com_TxBufferInfoIterType bufferIdx = Com_GetTxBufferInfoIdxOfTxPduInfo(ComTxPduId);
      /*@ assert bufferIdx < Com_GetSizeOfTxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      PduInfo.SduDataPtr = Com_TxBuffer_GetAddrTxBufferFirstElement(bufferIdx);
    }
  }

  /* If the ComIPdu is a TP PDU */
  if(Com_LLTxTp_IsTpUsed(ComTxPduId))
  {
    /* Set the SduLength, if the ComIPdu is in READY state. Otherwise skip transmission */
    if(Com_LLTxTp_IsTpStateReady(ComTxPduId))
    {
      /* \trace SPEC-2736903 */
      PduInfo.SduLength = (PduLengthType) Com_GetTxSduLength(ComTxPduId);
    }
    else
    {
      /* \trace SPEC-2736899 */
      retVal = E_NOT_OK;
    }
  }
  /* otherwise, set the SduLength to the current TxSduLength or to the PDU length with meta data length (if configured) */
  else
  {
    /* meta data exists and it is not TP, use length with meta data */
    if(Com_IsTxBufferInfoMetaDataUsedOfTxPduInfo(ComTxPduId))
    {
      PduInfo.SduLength = Com_GetPduWithMetaDataLengthOfTxPduInfo(ComTxPduId);
    }
    else
    {
      /* no meta data exists or it is TP, use IPduLength */
      PduInfo.SduLength = (PduLengthType) Com_GetTxSduLength(ComTxPduId);
    }
  }
  /* If the ComIPdu is not a TP PDU or was initially already in READY state */
  if(retVal == E_OK)
  {
    /* Call the configured callout function */
    boolean calloutResult = TRUE;
    if(Com_IsReportingTxIPduCalloutUsedOfTxPduInfo(ComTxPduId))
    {
      const Com_ReportingIterType reportingIdx = Com_GetReportingTxIPduCalloutIdxOfTxPduInfo(ComTxPduId);
      /*@ assert reportingIdx < Com_GetSizeOfReporting(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      calloutResult = Com_Reporting_CallTxIpduCallout(reportingIdx, ComTxPduId, &PduInfo);
    }
    /* If the callout return value is positive */
    if(calloutResult)
    {
      /* If the ComIPdu is a TP PDU, set the state variable to TRANSMISSION_STARTED or to WAITING_FOR_CONFIRMATION for zero-sized Pdu */
      if(Com_LLTxTp_IsTpUsed(ComTxPduId))
      {
        if(Com_GetTxSduLength(ComTxPduId) == 0u)
        {
          Com_LLTxTp_SetTxTpConnectionState(ComTxPduId, COM_WAITING_FOR_CONFIRMATION_TXTPCONNECTIONSTATE);
        }
        else
        {
          Com_LLTxTp_SetTxTpConnectionState(ComTxPduId, COM_TRANSMISSION_STARTED_TXTPCONNECTIONSTATE);
        }
      }

      /* Initiate the transmit of a ComIPdu by a call of PduR_ComTransmit */
      retVal = PduR_ComTransmit(Com_GetExternalIdOfTxPduInfo(ComTxPduId), &PduInfo);

      /* Set ComIPdu Attributes depending on the value of retVal */
      Com_LLTxIf_SetTxAttributes(ComTxPduId, retVal);
    }

    /* Set the meta data buffer back to the meta data initial value (If meta data feature is enabled) */
    Com_TxBuffer_CpyInitMetaDataToBffr(ComTxPduId);
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_LLTxIf_CancelTransmit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_LLTxIf_CancelTransmit(Com_TxPduInfoIterType ComTxPduId)
{
  if(Com_IsCancellationSupportOfTxPduInfo(ComTxPduId))  /* COV_COM_CANCELLATION_SUPPORT_OFF */
  {
#if (COM_CANCEL_TRANSMIT_API == STD_ON)
    (void) PduR_ComCancelTransmit(Com_GetExternalIdOfTxPduInfo(ComTxPduId));
#endif
  }
}

/**********************************************************************************************************************
  Com_LLTxIf_TxConfirmationProcessing
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
FUNC(void, COM_CODE) Com_LLTxIf_TxConfirmationProcessing(Com_TxPduInfoIterType TxPduId)
{
  boolean doNotificationProcessing = FALSE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* If the Update Bit clear context is CONFIRMATION, clear the Update Bits */
  if(COM_CONFIRMATION_CLRUBOFTXPDUINFO == Com_GetClrUbOfTxPduInfo(TxPduId))
  {
    Com_TxBuffer_ClearUpdateBits(TxPduId);
  }

  /* If the ComIPduGroup is active */
  /* \trace SPEC-2736844 */
  if(Com_IPduGroupHdlr_IsTxPduActive(TxPduId))
  {
    boolean isFinalConfirmation = TRUE;

    /* Process MDT and repetition counter. */
    Com_TxTransmit_Confirmation(TxPduId);
    if(Com_IsRepetitionInfoUsedOfTxPduInfo(TxPduId))
    {
      const Com_RepetitionInfoIterType repetitionInfoIdx = Com_GetRepetitionInfoIdxOfTxPduInfo(TxPduId);
      /*@ assert repetitionInfoIdx < Com_GetSizeOfRepetitionInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      isFinalConfirmation = Com_Repetition_Confirmation(repetitionInfoIdx);
    }

    /* If last transmission of a trigger event is confirmed */
    if(isFinalConfirmation)
    {
      doNotificationProcessing = TRUE;
      /* Do Tx Deadline monitoring confirmation processing */
      Com_TxDlMon_TxConfirmation(TxPduId);
    }
  }

  /* Do Tx notification processing */
  if(doNotificationProcessing)
  {
    Com_Notifications_Confirmation(TxPduId);
  }
}

/**********************************************************************************************************************
  Com_LLTxIf_TriggerTransmitProcessing
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
**********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_LLTxIf_TriggerTransmitProcessing(Com_TxPduInfoIterType TxPduId, CONSTP2VAR(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  Std_ReturnType retVal = E_NOT_OK;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* If the PDU length fits into the passed buffer */
  if(PduInfoPtr->SduLength >= Com_GetTxSduLength(TxPduId))
  {
    PduInfoType PduInfo = { NULL_PTR, 0u };

    /* If the ComIPdu is not a Zero Byte PDU, set the SduDataPtr to PDU buffer address otherwise set the SduDataPtr to NULL_PTR */
    if(Com_IsTxBufferInfoUsedOfTxPduInfo(TxPduId))
    {
      const Com_TxBufferInfoIterType bufferIdx = Com_GetTxBufferInfoIdxOfTxPduInfo(TxPduId);
      /*@ assert bufferIdx < Com_GetSizeOfTxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      PduInfo.SduDataPtr = Com_TxBuffer_GetAddrTxBufferFirstElement(bufferIdx);
    }

    /* Set the SduLength to the current TxSduLength */
    PduInfo.SduLength = (PduLengthType) Com_GetTxSduLength(TxPduId);
    SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
    {
      if(Com_IsReportingIPduTriggerTransmitCalloutUsedOfTxPduInfo(TxPduId))
      {
        const Com_ReportingIterType reportingIdx = Com_GetReportingIPduTriggerTransmitCalloutIdxOfTxPduInfo(TxPduId);
        (void) Com_Reporting_CallTxIpduCallout(reportingIdx, TxPduId, &PduInfo);
      }
      SduDataPtrType externalDataPtr = PduInfoPtr->SduDataPtr;
      const PduLengthType externalLength = PduInfoPtr->SduLength;
      CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) internalDataPtr = PduInfo.SduDataPtr;
      const PduLengthType internalLength = PduInfo.SduLength;

      if((internalDataPtr != NULL_PTR) && (externalDataPtr != NULL_PTR))
      {
        /*@ assert $external(externalDataPtr); */ /* VCA_COM_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */
        /*@ assert $lengthOf(externalDataPtr) >= PduInfo.SduLength; */ /* VCA_COM_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */
        /*@ assert $lengthOf(internalDataPtr) >= internalLength; */ /* VCA_COM_VCA_ASSUMES_MEMCPY_OVERWRITES_MEMORY */
        /* Copy the PDU buffer data to the provided pointer */
        VStdLib_MemCpy_s(externalDataPtr, externalLength, internalDataPtr, internalLength);  /* VCA_COM_MEMCPY_BOUNDARY_CHECK */   /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
      }
      /* If the Update Bit clear context is TRIGGER_TRANSMIT, clear the Update Bits */
      if(COM_TRIGGER_TRANSMIT_CLRUBOFTXPDUINFO == Com_GetClrUbOfTxPduInfo(TxPduId))
      {
        Com_TxBuffer_ClearUpdateBits(TxPduId); /*@ assert TxPduId < Com_GetSizeOfTxPduInfo(); */ /* VCA_COM_CSL01 */
      }
    }
    SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
    /* Set the provided SduLength pointer to the value of the current SduLength */
    PduInfoPtr->SduLength = PduInfo.SduLength;
    /* Return E_OK If the ComIPduGroup is active */
    if(Com_IPduGroupHdlr_IsTxPduActive(TxPduId))
    {
      retVal = E_OK;
    }
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

/**********************************************************************************************************************
 *  END OF FILE: Com_LLTxIf.c
 *********************************************************************************************************************/
