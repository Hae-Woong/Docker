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
/*!        \file  Com_Notifications.c
 *         \unit  Notifications
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_NOTIFICATIONS_SOURCE

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
#include "Com_Notifications.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_TransmissionProcessingData.h"
#include "Com_Cfg_Reporting.h"
#include "Com_Cfg_ISRThreshold.h" /* Com_GetSizeOfISRThresholdInfo() for VCA */

#include "Com_Reporting.h"
#include "Com_ISRThreshold.h"
#include "Com_Caching.h"

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
  Com_Notifications_CacheOrCallRxImmediateCbkFctPtr
**********************************************************************************************************************/
/*! \brief              This function caches the the provided index of function pointer in the Rx Immediate function
 *                      pointer cache. If the cache is full, all cached function pointers are called and the cache is
 *                      cleared.
 *   \details           -
 *   \pre               -
 *   \param[in]         reportingIdx        Index corresponding to the function that should be cached or called.
 *   \param[in,out]     fctPtrCacheStrctPtr Pointer to struct which contains all information about the cache (size, index, location)
 *   \context           TASK|ISR2
 *   \synchronous       TRUE
 *   \reentrant         TRUE, for different Handles
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Notifications_CacheOrCallRxImmediateCbkFctPtr(Com_ReportingIterType reportingIdx, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr);

/**********************************************************************************************************************
  Com_Notifications_CacheOrCallRxDeferredCbkFctPtr
**********************************************************************************************************************/
/*! \brief              This function caches the the provided index of function pointer in the Rx Deferred function
 *                      pointer cache. If the cache is full, all cached function pointers are called and the cache is
 *                      cleared.
 *   \details           -
 *   \pre               -
 *   \param[in]         thresholdInfoIdx    Index of the indirection from the main function rx to the ISRLock counter
 *   \param[in]         reportingIdx        Index corresponding to the function that should be cached or called.
 *   \param[in,out]     fctPtrCacheStrctPtr Pointer to struct which contains all information about the cache (size, index, location)
 *   \context           TASK|ISR2
 *   \synchronous       TRUE
 *   \reentrant         TRUE, for different Handles
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Notifications_CacheOrCallRxDeferredCbkFctPtr(Com_ISRThresholdInfoIterType thresholdInfoIdx, Com_ReportingIterType reportingIdx,
                                                                CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Notifications_CacheOrCallRxImmediateCbkFctPtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Notifications_CacheOrCallRxImmediateCbkFctPtr(Com_ReportingIterType reportingIdx, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr)
{
  /* Try to cache the function pointer index. */
  Com_Caching_Push(fctPtrCacheStrctPtr, (uint32) reportingIdx);

  /* If the cache is full */
  if(Com_Caching_IsOverflowOccurred(fctPtrCacheStrctPtr))
  {
    SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
    {
      /* call the current (not cached) function callback */
      Com_Reporting_CallNotification(reportingIdx);
      /* call all cached function callbacks */
      Com_Notifications_ProcessRxFctPtrCache(fctPtrCacheStrctPtr);
    }
    SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
  }
}

/**********************************************************************************************************************
  Com_Notifications_CacheOrCallRxDeferredCbkFctPtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Notifications_CacheOrCallRxDeferredCbkFctPtr(Com_ISRThresholdInfoIterType thresholdInfoIdx, Com_ReportingIterType reportingIdx,
                                                                CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr)
{
  /* Try to cache the function pointer index. */
  Com_Caching_Push(fctPtrCacheStrctPtr, (uint32) reportingIdx);

  /* If the cache is full */
  if(Com_Caching_IsOverflowOccurred(fctPtrCacheStrctPtr))
  {
    /*@ assert thresholdInfoIdx < Com_GetSizeOfISRThresholdInfo(); */ /* VCA_COM_CSL03 */
    Com_ISRThreshold_ExitExclusiveArea(thresholdInfoIdx);
    {
      /* call the current (not cached) function callback */
      Com_Reporting_CallNotification(reportingIdx);
      /* call all cached function callbacks */
      Com_Notifications_ProcessRxFctPtrCache(fctPtrCacheStrctPtr);
    }
    Com_ISRThreshold_EnterExclusiveArea(thresholdInfoIdx);
  }
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Notifications_InitTxIpdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Notifications_InitTxIpdu(Com_TxPduInfoIterType ComPduId)
{
  Com_SetCallDeferredConfirmationNotification(ComPduId, FALSE);
  Com_SetWaitingForConfirmation(ComPduId, FALSE);
}

/**********************************************************************************************************************
  Com_Notifications_MainFunctionTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Notifications_MainFunctionTx(Com_MainFunctionTxStructIterType mainFunctionId)
{
  /* Iterate over all Pdus and check if a deferred transmission is pending for this Pdu */
  for(Com_TxPduInfoOfMainFunctionTxIndIterType txPduInfoIndIdx = Com_GetTxPduInfoOfMainFunctionTxIndStartIdxOfMainFunctionTxStruct(mainFunctionId);
      txPduInfoIndIdx < Com_GetTxPduInfoOfMainFunctionTxIndEndIdxOfMainFunctionTxStruct(mainFunctionId); txPduInfoIndIdx++)  /* FETA_COM_01 */
  {
    /*@ assert txPduInfoIndIdx < Com_GetSizeOfTxPduInfoOfMainFunctionTxInd(); */ /* VCA_COM_CSL03 */
    const Com_TxPduInfoIterType ComPduId = Com_GetTxPduInfoOfMainFunctionTxInd(txPduInfoIndIdx);
    if(Com_IsCallDeferredConfirmationNotification(ComPduId))
    {
      /* Call all related notification function of this ComIPdu */
      for(Com_ReportingConfirmationNotificationIndIterType reportingIndIndex = Com_GetReportingConfirmationNotificationIndStartIdxOfTxPduInfo(ComPduId);
          reportingIndIndex < Com_GetReportingConfirmationNotificationIndEndIdxOfTxPduInfo(ComPduId); reportingIndIndex++)  /* FETA_COM_01 */
      {
        const Com_ReportingIterType reportingIdx = Com_GetReportingConfirmationNotificationInd(reportingIndIndex);
        /*@ assert reportingIdx < Com_GetSizeOfReporting(); */ /* VCA_COM_CSL03 */
        Com_Reporting_CallNotification(reportingIdx);
      }
      Com_SetCallDeferredConfirmationNotification(ComPduId, FALSE);
    }
  }
}

/**********************************************************************************************************************
  Com_Notifications_Confirmation
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
FUNC(void, COM_CODE) Com_Notifications_Confirmation(Com_TxPduInfoIterType ComPduId)
{
  /* Clear notification flag */
  Com_SetWaitingForConfirmation(ComPduId, FALSE);

  /* If the signal processing is... */
  switch (Com_GetSignalProcessingOfTxPduInfo(ComPduId))
  {
    /* ...IMMEDIATE*/
    case COM_IMMEDIATE_SIGNALPROCESSINGOFTXPDUINFO:
    {
      /* Call all related notification function of this ComIPdu */
      SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
      {
        for(Com_ReportingConfirmationNotificationIndIterType reportingIndIdx = Com_GetReportingConfirmationNotificationIndStartIdxOfTxPduInfo(ComPduId);
            reportingIndIdx < Com_GetReportingConfirmationNotificationIndEndIdxOfTxPduInfo(ComPduId); reportingIndIdx++)  /* FETA_COM_01 */
        {
          const Com_ReportingIterType reportingIdx = Com_GetReportingConfirmationNotificationInd(reportingIndIdx);
          /*@ assert  reportingIdx < Com_GetSizeOfReporting(); */ /* VCA_COM_CSL03 */
          Com_Reporting_CallNotification(reportingIdx);
        }
      }
      SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
      break;
    }
    /* ...DEFERRED */
    case COM_DEFERRED_SIGNALPROCESSINGOFTXPDUINFO:
      /* set the CallDeferredConfirmationNotification flag */
      Com_SetCallDeferredConfirmationNotification(ComPduId, TRUE);
      break;
    default: /* COV_COM_MISRA */
    /* Intentionally Empty */
      break;
  }
}

/**********************************************************************************************************************
  Com_Notifications_ProcessRxFctPtrCache
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Notifications_ProcessRxFctPtrCache(CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr)
{
  Com_Caching_ResetNumberOfElementsRead(fctPtrCacheStrctPtr);
  while((Com_Caching_IsEmpty(fctPtrCacheStrctPtr) == FALSE) && (!Com_Caching_IsReadThresholdReached(fctPtrCacheStrctPtr)))  /* FETA_COM_02 */
  {
    const Com_ReportingIterType reportingIdx = (Com_ReportingIterType) Com_Caching_Pop(fctPtrCacheStrctPtr);
    if(reportingIdx < Com_GetSizeOfReporting())
    {
      Com_Reporting_CallNotification(reportingIdx);
    }
  }
}

/**********************************************************************************************************************
  Com_Notifications_CacheOrCallRxCbkFctPtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Notifications_CacheOrCallRxCbkFctPtr(Com_ReportingIterType reportingIdx, Com_RxPduInfoIterType comRxPduInfoIdx,
                                                        CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr)
{
  /* If callback function pointer should be processed deferred */
  if(Com_IsHandleRxPduDeferredUsedOfRxPduInfo(comRxPduInfoIdx))
  {
    /* Cache index of function ptr in deferred function pointer cache or call all function pointer if cache is full. */
    if(Com_IsMainFunctionRxStructUsedOfRxPduInfo(comRxPduInfoIdx)) /* COV_COM_DEFERRED_MAINFUNCTION_RX */
    {
      const Com_MainFunctionRxStructIterType mainFunctionId = Com_GetMainFunctionRxStructIdxOfRxPduInfo(comRxPduInfoIdx);
      const Com_ISRThresholdInfoIterType rxMainFunctionProcessingISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfMainFunctionRxStruct(mainFunctionId);
      Com_Notifications_CacheOrCallRxDeferredCbkFctPtr(rxMainFunctionProcessingISRLockCounterIdx, reportingIdx, fctPtrCacheStrctPtr);
    }
  }
  /* Otherwise */
  else
  {
    /* Cache index of function ptr in immediate function pointer cache or call all function pointer if cache is full. */
    Com_Notifications_CacheOrCallRxImmediateCbkFctPtr(reportingIdx, fctPtrCacheStrctPtr);
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
 *  END OF FILE: Com_Notifications.c
 *********************************************************************************************************************/
