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
/*!        \file  Com_TxDlMon.c
 *         \unit  TxDlMon
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_TXDLMON_SOURCE

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
#include "Com_TxDlMon.h"
#include "Com_Cfg_TxDlMon.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_Reporting.h"
#include "Com_Timer.h"
#include "Com_IPduGroupHdlr.h"
#include "Com_ISRThreshold.h"
#include "Com_TxMinDelay.h"
#include "Com_Transmission.h"

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
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxDlMon_Event
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_Event(Com_TxTOutInfoIterType idxTxTOutInfo)
{
  Com_TxPduInfoIterType idxTxPduInfo = Com_GetTxPduInfoIdxOfTxTOutInfo(idxTxTOutInfo);
  /* Set the repetition counter to 0, if configured */
  if(Com_IsRepetitionInfoUsedOfTxPduInfo(idxTxPduInfo))
  {
    Com_Transmission_StopRepetitions(Com_GetRepetitionInfoIdxOfTxPduInfo(idxTxPduInfo));
  }
  /* Set the MDT counter to 0 */
  Com_TxMinDelay_ResetCounter(idxTxPduInfo);

  /* Do transmit cancellation */
  Com_Transmission_CancelTransmit(idxTxPduInfo);

  /* Call the configured timeout notifications, if configured */
  if(Com_IsReportingTxTimeoutNotificationIndUsedOfTxTOutInfo(idxTxTOutInfo))
  {
    Com_ISRThreshold_ExitExclusiveArea(Com_GetISRThresholdInfoIdxOfMainFunctionTxStruct(Com_GetMainFunctionTxStructIdxOfTxPduInfo(idxTxPduInfo)));
    {
      Com_ReportingTxTimeoutNotificationIndIterType reportingIndIdx = Com_GetReportingTxTimeoutNotificationIndStartIdxOfTxTOutInfo(idxTxTOutInfo);
      for(; reportingIndIdx < Com_GetReportingTxTimeoutNotificationIndEndIdxOfTxTOutInfo(idxTxTOutInfo); reportingIndIdx++)  /* FETA_COM_01 */
      {
        Com_ReportingIterType reportingIdx = Com_GetReportingTxTimeoutNotificationInd(reportingIndIdx);
        Com_Reporting_CallNotification(reportingIdx);
      }
    }
    Com_ISRThreshold_EnterExclusiveArea(Com_GetISRThresholdInfoIdxOfMainFunctionTxStruct(Com_GetMainFunctionTxStructIdxOfTxPduInfo(idxTxPduInfo)));
  }

#if (COM_RETRY_FAILED_TRANSMIT_REQUESTS == STD_ON)
  /* Reset the transmit request, if RETRY_FAILED_TRANSMIT_REQUESTS is enabled */
  Com_Transmission_ClearTransmitRequest(idxTxPduInfo);
#endif
}

/**********************************************************************************************************************
  Com_TxDlMon_MainFunctionTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_MainFunctionTx(Com_MainFunctionTxStructIterType mainFunctionId)
{
  if(Com_HasTxTOutInfo())
  {
    const Com_ISRThresholdInfoIterType txprocessingisrlockcounterIdx = Com_GetISRThresholdInfoIdxOfMainFunctionTxStruct(mainFunctionId);
    Com_ISRThreshold_EnterExclusiveArea(txprocessingisrlockcounterIdx);

    /* Iterate over all tx timeout infos referenced from this main function */
    for(Com_TxTOutInfoOfMainFunctionTxIndIterType txTOutInfoIndIdx = Com_GetTxTOutInfoOfMainFunctionTxIndStartIdxOfMainFunctionTxStruct(mainFunctionId);
        txTOutInfoIndIdx < Com_GetTxTOutInfoOfMainFunctionTxIndEndIdxOfMainFunctionTxStruct(mainFunctionId); txTOutInfoIndIdx++)  /* FETA_COM_01 */
    {
      /*@ assert txTOutInfoIndIdx < Com_GetSizeOfTxTOutInfoOfMainFunctionTxInd(); */ /* VCA_COM_CSL03 */
      const Com_TxTOutInfoIterType idxTxTOutInfo = Com_GetTxTOutInfoOfMainFunctionTxInd(txTOutInfoIndIdx);
      const Com_TxPduInfoIterType idxTxPduInfo = Com_GetTxPduInfoIdxOfTxTOutInfo(idxTxTOutInfo);

      /* If the Tx ComIPdu is active, decrement the timeout counter value */
      if(Com_IPduGroupHdlr_IsTxPduActive(idxTxPduInfo))
      {
        const Com_ComTimerInfoIterType idxTimerInfo = Com_GetComTimerInfoIdxOfTxTOutInfo(idxTxTOutInfo);
        Com_Timer_DecCounter(idxTimerInfo);
      }
      Com_ISRThreshold_ThresholdCheck(txprocessingisrlockcounterIdx);
    }

    Com_ISRThreshold_ExitExclusiveArea(txprocessingisrlockcounterIdx);
  }
}

/**********************************************************************************************************************
  Com_TxDlMon_TxConfirmation
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_TxConfirmation(Com_TxPduInfoIterType TxPduId)
{
  /* If the passed Tx ComIPdu has a related tx timeout */
  if(Com_IsTxTOutInfoUsedOfTxPduInfo(TxPduId))
  {
    const Com_TxTOutInfoIterType idxTxTOutInfo = Com_GetTxTOutInfoIdxOfTxPduInfo(TxPduId);
    const Com_ComTimerInfoIterType idxTimerInfo = Com_GetComTimerInfoIdxOfTxTOutInfo(idxTxTOutInfo);

    /* If the tx timeout is in None mode and the current timeout counter value is greater 0, set the tx timeout counter to the configured tx timeout factor */
    if(Com_GetModeOfTxTOutInfo(idxTxTOutInfo) == COM_NONE_MODEOFTXTOUTINFO)
    {
      if(Com_Timer_IsTimerRunning(idxTimerInfo))
      {
        Com_Timer_StartCounter(idxTimerInfo);
      }
    }
    /* Otherwise the tx timeout is in Normal mode, in this case set the tx timeout counter to 0 */
    else
    {
      Com_Timer_StopCounter(idxTimerInfo);
    }
  }
}

/**********************************************************************************************************************
  Com_TxDlMon_IPduGroupStart
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_IPduGroupStart(Com_TxPduInfoIterType TxPduId)
{
  if(Com_IsTxTOutInfoUsedOfTxPduInfo(TxPduId))
  {
    const Com_TxTOutInfoIterType idxTxTOutInfo = Com_GetTxTOutInfoIdxOfTxPduInfo(TxPduId);
    Com_Timer_InitCounter(Com_GetComTimerInfoIdxOfTxTOutInfo(idxTxTOutInfo));
  }
}

/**********************************************************************************************************************
  Com_TxDlMon_InitTxMode
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_InitTxMode(Com_TxPduInfoIterType TxPduId)
{
  /* If the passed Tx ComIPdu has a related tx timeout and the tx timeout is in Normal mode */
  if(Com_IsTxTOutInfoUsedOfTxPduInfo(TxPduId))
  {
    const Com_TxTOutInfoIterType idxTxTOutInfo = Com_GetTxTOutInfoIdxOfTxPduInfo(TxPduId);

    if(Com_GetModeOfTxTOutInfo(idxTxTOutInfo) == COM_NORMAL_MODEOFTXTOUTINFO)
    {
      /* If the passed Tx ComIPdu is in NONE transmission mode, set the tx timeout counter to 0 */
      if(!(Com_Transmission_IsPeriodicTxMode(TxPduId) || Com_Transmission_IsDirectTxMode(TxPduId)))
      {
        const Com_ComTimerInfoIterType idxTimerInfo = Com_GetComTimerInfoIdxOfTxTOutInfo(idxTxTOutInfo);
        /* \trace SPEC-2736879 */
        Com_Timer_StopCounter(idxTimerInfo);
      }
    }
  }
}

/**********************************************************************************************************************
  Com_TxDlMon_StartTxDlMonCounter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_StartTxDlMonCounter(Com_TxPduInfoIterType ComTxPduId)
{
  /* If Tx Deadline Monitoring is configured */
  if(Com_IsTxTOutInfoUsedOfTxPduInfo(ComTxPduId))
  {
    const Com_TxTOutInfoIterType idxTxTOutInfo = Com_GetTxTOutInfoIdxOfTxPduInfo(ComTxPduId);
    const Com_ComTimerInfoIterType idxTimerInfo = Com_GetComTimerInfoIdxOfTxTOutInfo(idxTxTOutInfo);

    /* Start Tx Deadline Monitoring timer if not already running */
    if(Com_Timer_IsTimerRunning(idxTimerInfo) == FALSE)
    {
      Com_Timer_StartCounter(idxTimerInfo);
    }
  }
}

/**********************************************************************************************************************
  Com_TxDlMon_ReloadTxDlMonCounter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxDlMon_ReloadTxDlMonCounter(Com_TxPduInfoIterType ComTxPduId)
{
  if(Com_IsTxTOutInfoUsedOfTxPduInfo(ComTxPduId))
  {
    const Com_TxTOutInfoIterType idxTxTOutInfo = Com_GetTxTOutInfoIdxOfTxPduInfo(ComTxPduId);
    Com_Timer_StartCounter(Com_GetComTimerInfoIdxOfTxTOutInfo(idxTxTOutInfo));
  }
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
 *  END OF FILE: Com_TxDlMon.c
 *********************************************************************************************************************/
