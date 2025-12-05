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
/*!        \file  Com_RxDlMon.c
 *         \unit  RxDlMon
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_RXDLMON_SOURCE

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
#include "Com_RxDlMon.h"
#include "Com_Cfg_RxDlMon.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_Reporting.h"
#include "Com_Cfg_Timer.h" /* Com_GetSizeOfComTimerInfo() for VCA */

#include "Com_RxSigBuffer.h"
#include "Com_Timer.h"
#include "Com_Reporting.h"
#include "Com_ISRThreshold.h"
#include "Com_SigGw.h"

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
  Com_RxDlMon_TimeoutOccurred
**********************************************************************************************************************/
/*! \brief        This function sets the Rx Deadline Monitoring State to Occurred.
    \details      -
    \pre          -
    \param[in]    ComRxToutObjtId  ID of Rx Timeout object.
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxDlMon_TimeoutOccurred(Com_RxTOutInfoIterType ComRxToutObjtId);

/**********************************************************************************************************************
  Com_RxDlMon_RxIpduDmActive
**********************************************************************************************************************/
/*! \brief        This function evaluates the Deadline Monitoring state of the Rx I-Pdu.
    \details      -
    \pre          -
    \param[in]    ComRxToutObjtId  ID of Rx Timeout object.
    \return       boolean
                  true:   The Deadline Monitoring is enabled for at least one mapped I-Pdu Group
                  false:  The Deadline Monitoring is disabled for all mapped I-Pdu Groups
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_RxDlMon_RxIpduDmActive(Com_RxTOutInfoIterType ComRxToutObjtId);

/**********************************************************************************************************************
  Com_RxDlMon_TimeoutAction
**********************************************************************************************************************/
/*! \brief        This function sets all related signal replace values of passed timeout pdu.
    \details      -
    \pre          -
    \param[in]    idxRxTOutInfo RxToutInfo handle id
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxDlMon_TimeoutAction(Com_RxTOutInfoIterType idxRxTOutInfo);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxDlMon_TimeoutOccurred
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxDlMon_TimeoutOccurred(Com_RxTOutInfoIterType ComRxToutObjtId)
{
  Com_SetRxPduDmState(ComRxToutObjtId, COM_OCCURRED_RXPDUDMSTATE);
}

/**********************************************************************************************************************
  Com_RxDlMon_RxIpduDmActive
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_RxDlMon_RxIpduDmActive(Com_RxTOutInfoIterType ComRxToutObjtId)
{
  return ((Com_GetRxPduDmState(ComRxToutObjtId) & COM_ACTIVE_RXPDUDMSTATE) == COM_ACTIVE_RXPDUDMSTATE);
}

/**********************************************************************************************************************
  Com_RxDlMon_TimeoutAction
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxDlMon_TimeoutAction(Com_RxTOutInfoIterType idxRxTOutInfo)
{
  /* Iterate over all related ComSignals with timeout replace value */
  for(Com_RxAccessInfoReplaceSigIndIterType idxRxAccessInfoInd = Com_GetRxAccessInfoReplaceSigIndStartIdxOfRxTOutInfo(idxRxTOutInfo);
      idxRxAccessInfoInd < Com_GetRxAccessInfoReplaceSigIndEndIdxOfRxTOutInfo(idxRxTOutInfo); idxRxAccessInfoInd++)  /* FETA_COM_01 */
  {
    const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoReplaceSigInd(idxRxAccessInfoInd);

    /* Set the ComSignal buffer to the timeout replace value */
    Com_RxSigBuffer_SetRxDataTimeoutSubValue(idxRxAccessInfo); /* VCA_COM_CSL03 */
# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
    /* If the ComSignal is routed, set the related Gateway Event flag to forward the Rx timeout value to Tx side */
    Com_SigGw_SetGwEvent(idxRxAccessInfo);
# endif
  }
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxDlMon_HasTimeoutOccurred
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_RxDlMon_HasTimeoutOccurred(Com_RxTOutInfoIterType ComRxToutObjtId)
{
  return (Com_GetRxPduDmState(ComRxToutObjtId) == COM_OCCURRED_RXPDUDMSTATE);
}

/**********************************************************************************************************************
  Com_RxDlMon_RemoveOccurredFlag
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_RemoveOccurredFlag(Com_RxTOutInfoIterType ComRxToutObjtId)
{
  Com_SetRxPduDmState(ComRxToutObjtId, (Com_GetRxPduDmState(ComRxToutObjtId) & COM_CLEAR_OCCURRED_FLAG_RXPDUDMSTATE));
}

/**********************************************************************************************************************
  Com_RxDlMon_MainFunctionRx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_MainFunctionRx(Com_MainFunctionRxStructIterType mainFunctionId)
{
  if(Com_HasRxTOutInfo())
  {
    const Com_ISRThresholdInfoIterType rxMainFunctionProcessingISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfMainFunctionRxStruct(mainFunctionId);

    Com_ISRThreshold_EnterExclusiveArea(rxMainFunctionProcessingISRLockCounterIdx);
    /* Iterate over all Rx deadline monitorings */
    for(Com_RxTOutInfoOfMainFunctionRxIndIterType rxTOutInfoIndIdx = Com_GetRxTOutInfoOfMainFunctionRxIndStartIdxOfMainFunctionRxStruct(mainFunctionId);
        rxTOutInfoIndIdx < Com_GetRxTOutInfoOfMainFunctionRxIndEndIdxOfMainFunctionRxStruct(mainFunctionId); rxTOutInfoIndIdx++)  /* FETA_COM_01 */
    {
    /*@ assert rxTOutInfoIndIdx < Com_GetSizeOfRxTOutInfoOfMainFunctionRxInd(); */ /* VCA_COM_CSL03 */
      const Com_RxTOutInfoIterType idxRxTOutInfo = Com_GetRxTOutInfoOfMainFunctionRxInd(rxTOutInfoIndIdx);

      /* If the Rx deadline monitoring state machine is active and the timeout counter is greater zero, decrement the timeout counter */
      if(Com_RxDlMon_RxIpduDmActive(idxRxTOutInfo))
      {
        Com_Timer_DecCounter(Com_GetComTimerInfoIdxOfRxTOutInfo(idxRxTOutInfo));
      }
      Com_ISRThreshold_ThresholdCheck(rxMainFunctionProcessingISRLockCounterIdx);
    }
    Com_ISRThreshold_ExitExclusiveArea(rxMainFunctionProcessingISRLockCounterIdx);
  }
}

/**********************************************************************************************************************
  Com_RxDlMon_TimeOutEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_TimeOutEvent(Com_RxTOutInfoIterType idxRxTOutInfo)
{
  /* Set the rx timeout state to timeout occurred */
  Com_RxDlMon_TimeoutOccurred(idxRxTOutInfo);

  /* Do the timeout action for related ComSignals */
  Com_RxDlMon_TimeoutAction(idxRxTOutInfo);

  /* Call related timeout notification functions */
  if(Com_IsReportingRxTimeoutNotificationIndUsedOfRxTOutInfo(idxRxTOutInfo))
  {
    Com_ISRThreshold_ExitExclusiveArea(Com_GetISRThresholdInfoIdxOfMainFunctionRxStruct(Com_GetMainFunctionRxStructIdxOfRxTOutInfo(idxRxTOutInfo)));
    {
      for(Com_ReportingRxTimeoutNotificationIndIterType reportingIndIdx = Com_GetReportingRxTimeoutNotificationIndStartIdxOfRxTOutInfo(idxRxTOutInfo);
          reportingIndIdx < Com_GetReportingRxTimeoutNotificationIndEndIdxOfRxTOutInfo(idxRxTOutInfo); reportingIndIdx++)  /* FETA_COM_01 */
      {
        const Com_ReportingIterType reportingIdx = Com_GetReportingRxTimeoutNotificationInd(reportingIndIdx);
        /*@ assert reportingIdx < Com_GetSizeOfReporting(); */ /* VCA_COM_CSL03 */
        Com_Reporting_CallNotification(reportingIdx);
      }
    }
    Com_ISRThreshold_EnterExclusiveArea(Com_GetISRThresholdInfoIdxOfMainFunctionRxStruct(Com_GetMainFunctionRxStructIdxOfRxTOutInfo(idxRxTOutInfo)));
  }
}

/**********************************************************************************************************************
  Com_RxDlMon_InitRxIpdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_InitRxIpdu(Com_RxPduInfoIterType rxPduId)
{
  /* Iterate over all related timeout data sets */
  for(Com_RxTOutInfoIndIterType idxRxTOutInfoInd = Com_GetRxTOutInfoIndStartIdxOfRxPduInfo(rxPduId);
      idxRxTOutInfoInd < Com_GetRxTOutInfoIndEndIdxOfRxPduInfo(rxPduId); idxRxTOutInfoInd++)  /* FETA_COM_01 */
  {
    /* Set the Rx timeout counter value to the first timeout and set the deadline monitoring state machine to ACTIVE */
    /*@ assert idxRxTOutInfoInd < Com_GetSizeOfRxTOutInfoInd(); */ /* VCA_COM_CSL03 */
    Com_Timer_InitCounter(Com_GetComTimerInfoIdxOfRxTOutInfo(Com_GetRxTOutInfoInd(idxRxTOutInfoInd)));
    Com_SetRxPduDmState(Com_GetRxTOutInfoInd(idxRxTOutInfoInd), COM_ACTIVE_RXPDUDMSTATE);
  }
}

/**********************************************************************************************************************
  Com_RxDlMon_SigEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_SigEvent(Com_RxSigInfoIterType idxRxSigInfo)
{
  if(Com_IsRxTOutInfoUsedOfRxSigInfo(idxRxSigInfo))
  {
    const Com_RxTOutInfoIterType idxRxTOutInfo = Com_GetRxTOutInfoIdxOfRxSigInfo(idxRxSigInfo);
    const Com_ComTimerInfoIdxOfRxTOutInfoType comTimerInfoIdx = Com_GetComTimerInfoIdxOfRxTOutInfo(idxRxTOutInfo);
    /*@ assert comTimerInfoIdx < Com_GetSizeOfComTimerInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_Timer_StartCounter(comTimerInfoIdx);
  }
}

/**********************************************************************************************************************
  Com_RxDlMon_SigGrpEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_SigGrpEvent(Com_RxSigGrpInfoIterType idxRxSigGrpInfo)
{
  if(Com_IsRxTOutInfoUsedOfRxSigGrpInfo(idxRxSigGrpInfo))
  {
    const Com_RxTOutInfoIterType idxRxTOutInfo = Com_GetRxTOutInfoIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
    const Com_ComTimerInfoIdxOfRxTOutInfoType comTimerInfoIdx = Com_GetComTimerInfoIdxOfRxTOutInfo(idxRxTOutInfo);
    /*@ assert comTimerInfoIdx < Com_GetSizeOfComTimerInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_Timer_StartCounter(comTimerInfoIdx);
  }
}

/**********************************************************************************************************************
  Com_RxDlMon_ActivateRxDlMon
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_ActivateRxDlMon(Com_RxTOutInfoIterType idxRxTOutInfo)
{
  /* If the Rx deadline monitoring is INACTIVE, activate the Rx deadline monitoring */
  if((Com_GetRxPduDmState(idxRxTOutInfo) & COM_PURE_DM_STATE_MASK_RXPDUDMSTATE) == COM_INACTIVE_RXPDUDMSTATE)
  {
    Com_Timer_InitCounter(Com_GetComTimerInfoIdxOfRxTOutInfo(idxRxTOutInfo));
    Com_SetRxPduDmState(idxRxTOutInfo, COM_ACTIVE_RXPDUDMSTATE);
  }
}

/**********************************************************************************************************************
  Com_RxDlMon_ActivateRxIpduDm
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE)  Com_RxDlMon_ActivateRxIpduDm(Com_RxTOutInfoIterType ComRxToutObjtId)
{
  Com_SetRxPduDmState(ComRxToutObjtId, COM_ACTIVE_RXPDUDMSTATE);
}

/**********************************************************************************************************************
  Com_RxDlMon_DeactivateRxIpduDm
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxDlMon_DeactivateRxIpduDm(Com_RxTOutInfoIterType ComRxToutObjtId)
{
  Com_SetRxPduDmState(ComRxToutObjtId, COM_INACTIVE_RXPDUDMSTATE);
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
 *  END OF FILE: Com_RxDlMon.c
 *********************************************************************************************************************/
