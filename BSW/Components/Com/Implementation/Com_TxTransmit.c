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
/*!        \file  Com_TxTransmit.c
 *         \unit  TxTransmit
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_TXTRANSMIT_SOURCE

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
#include "Com_TxTransmit.h"
#include "Com_Cfg_TxTransmit.h"
#include "Com_Cfg_TransmissionProcessingData.h"
#include "Com_Cfg_Repetition.h"

#include "Com_IPduGroupHdlr.h"
#include "Com_TxMinDelay.h"
#include "Com_LLTxIf.h"
#include "Com_TxDlMon.h"
#include "Com_LLTxTp.h"
#include "Com_ISRThreshold.h"
#include "Com_Repetition.h"
#include "Com_TxBuffer.h"
#include "Com_TxModeHdlr.h"

#include "SchM_Com.h"
#include "vstdlib.h"

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
  Com_TxTransmit_MainFunctionTx_ProcessTransmit
**********************************************************************************************************************/
/*! \brief        This function process the transmit requests of the provided ComTxPduId.
    \details      -
    \pre          -
    \param[in]    ComTxPduId    ID of Tx I-PDU.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxTransmit_MainFunctionTx_ProcessTransmit(Com_TxPduInfoIterType ComTxPduId);

/**********************************************************************************************************************
  Com_TxTransmit_TriggerIpduSendOnceDeferred
**********************************************************************************************************************/
/*! \brief        This method sets the internal transmit request flag.
                  The I-PDU Transmit request is performed with the next call of Com_TxModeHdlr_MainFunction.
    \details      -
    \pre          -
    \param[in]    ComTxPduId  ID of Tx I-PDU.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxTransmit_TriggerIpduSendOnceDeferred(Com_TxPduInfoIterType ComTxPduId);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_TxTransmit_DesGwTriggerTransmit
**********************************************************************************************************************/
/*! \brief        This method triggers a transmit.
    \details      -
    \pre          -
    \param[in]    ComTxPduId               ID of Tx I-PDU.
    \param[in]    TriggerWithoutRepetition Decide whether configured repetitions are taken into account.
                                           TRUE:  I-PDU is triggered just once without a repetition.
                                           FALSE: Configured repetitions are taken into account.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires ComTxPduId < Com_GetSizeOfTxPduInfo();
    \endspec
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxTransmit_DesGwTriggerTransmit(Com_TxPduInfoIterType ComTxPduId, boolean TriggerWithoutRepetition);
#endif


/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxTransmit_MainFunctionTx_ProcessTransmit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxTransmit_MainFunctionTx_ProcessTransmit(Com_TxPduInfoIterType ComTxPduId)
{
  /* If the passed Tx ComIPdu is active, the MDT has expired and a transmit request is set */
  if(Com_IPduGroupHdlr_IsTxPduActive(ComTxPduId))
  {
    if(Com_TxMinDelay_IsMDTExpired(ComTxPduId))
    {
      if(Com_IsTransmitRequest(ComTxPduId))
      {
        /* Trigger the transmission by a call of Com_LLTxIf_Transmit. Check if the return of Com_LLTxIf_Transmit is positive, if ComRetryFailedTransmitRequest is enabled */
#if (COM_RETRY_FAILED_TRANSMIT_REQUESTS == STD_ON)
        if(Com_LLTxIf_Transmit(ComTxPduId) == E_OK)
#else
        (void) Com_LLTxIf_Transmit(ComTxPduId);
#endif
        {
          /* Reset the transmit request */
          Com_SetTransmitRequest(ComTxPduId, FALSE);
        }
      }
    }
  }
}

/**********************************************************************************************************************
  Com_TxTransmit_TriggerIpduSendOnceDeferred
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxTransmit_TriggerIpduSendOnceDeferred(Com_TxPduInfoIterType ComTxPduId)
{
  /* If the passed Tx ComIPdu is active */
  if(Com_IPduGroupHdlr_IsTxPduActive(ComTxPduId))
  {
    /* Set event based trigger Flag if configured */
    Com_SetDirectTrigger(ComTxPduId, TRUE);
    /* Set the related transmit request */
    Com_SetTransmitRequest(ComTxPduId, TRUE);
#if(COM_MIXEDMODEPERIODICSUPPRESSION == STD_ON)
    Com_SetInitialTransmit(ComTxPduId, TRUE);
#endif
    /* Reload Tx Deadline monitoring timer if configured */
    Com_TxDlMon_StartTxDlMonCounter(ComTxPduId);
  }
}

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_TxTransmit_DesGwTriggerTransmit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_TxTransmit_DesGwTriggerTransmit(Com_TxPduInfoIterType ComTxPduId, boolean TriggerWithoutRepetition)
{
# if (COM_RETRY_FAILED_TRANSMIT_REQUESTS == STD_ON)
  /* If transmission was not successful and retry failed transmit feature is configured */
  if(Com_LLTxIf_Transmit(ComTxPduId) != E_OK)
  {
    /* Schedule deferred transmission in next main function cycle */
    Com_TxTransmit_TriggerDeferredTransmitAndSetTimer(ComTxPduId, TriggerWithoutRepetition);
  }
# else
  (void) Com_LLTxIf_Transmit(ComTxPduId);
# endif

  COM_DUMMY_STATEMENT(TriggerWithoutRepetition);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}
#endif

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxTransmit_SetTransmitRequest
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_SetTransmitRequest(Com_TxPduInfoIterType ComTxPduId)
{
  Com_SetTransmitRequest(ComTxPduId, TRUE);
}

/**********************************************************************************************************************
  Com_TxTransmit_ClearTransmitRequest
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_ClearTransmitRequest(Com_TxPduInfoIterType ComTxPduId)
{
  Com_SetTransmitRequest(ComTxPduId, FALSE);
}

/**********************************************************************************************************************
  Com_TxTransmit_ClearInitialTransmitFlag
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_ClearInitialTransmitFlag(Com_TxPduInfoIterType ComTxPduId)
{
#if (COM_MIXEDMODEPERIODICSUPPRESSION == STD_ON)
  Com_SetInitialTransmit(ComTxPduId, FALSE);
#endif
  COM_DUMMY_STATEMENT(ComTxPduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
  Com_TxTransmit_IsInitialTransmitFlag
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxTransmit_IsInitialTransmitFlag(Com_TxPduInfoIterType ComTxPduId)
{
  boolean retVal = TRUE;
#if (COM_MIXEDMODEPERIODICSUPPRESSION == STD_ON)
  retVal = Com_IsInitialTransmit(ComTxPduId);
#endif
  COM_DUMMY_STATEMENT(ComTxPduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  return retVal;
}

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_TxTransmit_DesGwTransmit
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
 ***********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, COM_CODE) Com_TxTransmit_DesGwTransmit(Com_TxPduInfoIterType ComTxPduId, boolean isImmediate, boolean TriggerWithoutRepetition)
{
  /* If txPdu should be processed immediately and minimum delay time has elapsed */
  if(isImmediate && Com_TxMinDelay_IsMDTExpired(ComTxPduId))
  {
    /* If the passed Tx ComIPdu is active and the current tx mode is direct */
    if(Com_IPduGroupHdlr_IsTxPduActive(ComTxPduId))
    {
      if(Com_TxModeHdlr_IsDirectTxMode(ComTxPduId))
      {
        /* Set event based trigger Flag */
        Com_SetDirectTrigger(ComTxPduId, TRUE);

        /* If the passed Tx ComIPdu is triggered without repetitions, start the Tx Deadline Monitoring timer if not already running */
        if(TriggerWithoutRepetition)
        {
          Com_TxDlMon_StartTxDlMonCounter(ComTxPduId);
        }
        /* Otherwise */
        else
        {
          /* If repetitions are configured, set the repetition count to the tx mode dependent repetition count value */
          if(Com_IsRepetitionInfoUsedOfTxPduInfo(ComTxPduId))
          {
            const Com_RepetitionInfoIterType repetitionInfoIdx = Com_GetRepetitionInfoIdxOfTxPduInfo(ComTxPduId);
            /*@ assert repetitionInfoIdx < Com_GetSizeOfRepetitionInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
            Com_Repetition_Start(repetitionInfoIdx);

            /* If repetitions are in progress, reload Tx Deadline Monitoring timer */
            if(Com_Repetition_IsInProgress(repetitionInfoIdx))
            {
              /* SPEC-2736884 */
              Com_TxDlMon_ReloadTxDlMonCounter(ComTxPduId);
            }
            else 
            {
              Com_TxDlMon_StartTxDlMonCounter(ComTxPduId);
            }
          } 
          else
          {
            Com_TxDlMon_StartTxDlMonCounter(ComTxPduId);
          }
        }

        /* Trigger Transmit */
        Com_TxTransmit_DesGwTriggerTransmit(ComTxPduId, TriggerWithoutRepetition);
      }
    }
  }
  /* otherwise trigger deferred transmission in next main function cycle */
  else
  {
    Com_TxTransmit_TriggerDeferredTransmitAndSetTimer(ComTxPduId, TriggerWithoutRepetition);
  }
}
#endif

/**********************************************************************************************************************
  Com_TxTransmit_ClearDirectTrigger
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_ClearDirectTrigger(Com_TxPduInfoIterType ComTxPduId)
{
  Com_SetDirectTrigger(ComTxPduId, FALSE);
}

/**********************************************************************************************************************
  Com_TxTransmit_IsDirectTrigger
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxTransmit_IsDirectTrigger(Com_TxPduInfoIterType ComTxPduId)
{
  return (!Com_HasDirectTrigger() || (Com_HasDirectTrigger() && Com_IsDirectTrigger(ComTxPduId)));
}

/**********************************************************************************************************************
  Com_TxTransmit_MainFunctionTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_MainFunctionTx(Com_MainFunctionTxStructIterType mainFunctionId)
{
  const Com_ISRThresholdInfoIterType txProcessingISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfMainFunctionTxStruct(mainFunctionId);
  Com_ISRThreshold_EnterExclusiveArea(txProcessingISRLockCounterIdx);

  for(Com_TxPduInfoOfMainFunctionTxIndIterType txPduInfoIndIdx = Com_GetTxPduInfoOfMainFunctionTxIndStartIdxOfMainFunctionTxStruct(mainFunctionId);
      txPduInfoIndIdx < Com_GetTxPduInfoOfMainFunctionTxIndEndIdxOfMainFunctionTxStruct(mainFunctionId); txPduInfoIndIdx++)    /* FETA_COM_01 */
  {
    /*@ assert txPduInfoIndIdx < Com_GetSizeOfTxPduInfoOfMainFunctionTxInd(); */ /* VCA_COM_CSL03 */
    const Com_TxPduInfoIterType ComTxPduId = Com_GetTxPduInfoOfMainFunctionTxInd(txPduInfoIndIdx);
    Com_TxTransmit_MainFunctionTx_ProcessTransmit(ComTxPduId);
    Com_ISRThreshold_ThresholdCheck(txProcessingISRLockCounterIdx);
  }

  Com_ISRThreshold_ExitExclusiveArea(txProcessingISRLockCounterIdx);
}

/**********************************************************************************************************************
  Com_TxTransmit_Confirmation
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_Confirmation(Com_TxPduInfoIterType ComTxPduId)
{
  if(Com_HasDirectTrigger())
  {
    if(Com_IsDirectTrigger(ComTxPduId))
    {   /* \trace SPEC-2736859 */
      Com_TxMinDelay_StartCounter(ComTxPduId);

      Com_SetDirectTrigger(ComTxPduId, FALSE);
    }
  }
  else
  {
    Com_TxMinDelay_StartCounter(ComTxPduId);
  }
}

/**********************************************************************************************************************
  Com_TxTransmit_TriggerDeferredTransmitAndSetTimer
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF*/
FUNC(void, COM_CODE) Com_TxTransmit_TriggerDeferredTransmitAndSetTimer(Com_TxPduInfoIterType ComTxPduId, boolean TriggerWithoutRepetition)
{
  /* If the passed Tx ComIPdu is active and the current tx mode is direct */
  if(Com_IPduGroupHdlr_IsTxPduActive(ComTxPduId))
  {
    if(Com_TxModeHdlr_IsDirectTxMode(ComTxPduId))
    {
      /* Set event based trigger Flag */
      Com_SetDirectTrigger(ComTxPduId, TRUE);

      /* If the passed Tx ComIPdu is triggered without repetitions, set the transmit request and reload the Tx Deadline Monitoring timer if not already running */
      if(TriggerWithoutRepetition)
      {
        Com_TxDlMon_StartTxDlMonCounter(ComTxPduId);
        Com_SetTransmitRequest(ComTxPduId, TRUE);
      }
      /* Otherwise */
      else
      {
        boolean reloadTxDlMon = TRUE;
        /* If repetitions are configured, set the repetition count to the tx mode dependent repetition count value */
        if(Com_IsRepetitionInfoUsedOfTxPduInfo(ComTxPduId))
        {
          const Com_RepetitionInfoIterType repetitionInfoIdx = Com_GetRepetitionInfoIdxOfTxPduInfo(ComTxPduId);
          /*@ assert (repetitionInfoIdx  < Com_GetSizeOfRepetitionInfo()); */ /* VCA_COM_OPTIONAL_INDIRECTION */
          Com_Repetition_StartFirstTransmission(repetitionInfoIdx);
          /* If the repetition is in progress, restart the Tx Deadline Monitoring and trigger the first transmission of n repetitive transmissions */
          if(Com_Repetition_IsInProgress(repetitionInfoIdx))
          {
            /* SPEC-2736884 */
            Com_TxDlMon_ReloadTxDlMonCounter(ComTxPduId);
#if (COM_MIXEDMODEPERIODICSUPPRESSION == STD_ON)
            Com_SetInitialTransmit(ComTxPduId, TRUE);
#endif
            reloadTxDlMon = FALSE;
          }
        }
        /* If no repetitions are configured or no repetitions are in progress set the transmit request and reload the Tx Deadline Monitoring */
        if(reloadTxDlMon)
        {
          Com_TxDlMon_StartTxDlMonCounter(ComTxPduId);
          Com_SetTransmitRequest(ComTxPduId, TRUE);
        }
      }
    }
  }
}

/**********************************************************************************************************************
  Com_TxTransmit_TriggerIpduSend
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_TriggerIpduSend(Com_TxPduInfoIterType ComTxPduId)
{
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
  {
    if((!Com_LLTxTp_IsTpUsed(ComTxPduId)) || (Com_LLTxTp_IsTpUsed(ComTxPduId) && Com_LLTxTp_IsTpStateReady(ComTxPduId)))
    {
      Com_TxTransmit_TriggerIpduSendOnceDeferred(ComTxPduId);
    }
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
}

/**********************************************************************************************************************
  Com_TxTransmit_TriggerIpduSendWithMetaData
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxTransmit_TriggerIpduSendWithMetaData(Com_TxPduInfoIterType ComTxPduId, P2CONST(uint8, AUTOMATIC, AUTOMATIC) MetaData)
{
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
  {
    if((!Com_LLTxTp_IsTpUsed(ComTxPduId)) || (Com_LLTxTp_IsTpUsed(ComTxPduId) && Com_LLTxTp_IsTpStateReady(ComTxPduId)))
    {
      if(Com_IsTxBufferInfoMetaDataUsedOfTxPduInfo(ComTxPduId))
      {
        const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoMetaDataIdxOfTxPduInfo(ComTxPduId);
        VStdLib_MemCpy(Com_TxBuffer_GetAddrTxBufferFirstElement(txBufferInfoIdx), MetaData, Com_TxBuffer_GetTxBufferLength(txBufferInfoIdx));  /* VCA_COM_MEMCPY_CSL_DEST */   /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        Com_TxTransmit_TriggerIpduSendOnceDeferred(ComTxPduId);
      }
    }
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
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
