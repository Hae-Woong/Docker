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
/*!        \file  Com_RxPduProcessing.c
 *         \unit  RxPduProcessing
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_RXPDUPROCESSING_SOURCE

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
#include "Com_RxPduProcessing.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_RxDlMon.h" /* Com_GetSizeOfRxTOutInfo() for VCA */

#include "Com_RxSignalProcessing.h"
#include "Com_LLRxTp.h"
#include "Com_DesGw.h"
#include "Com_IPduGroupHdlr.h"
#include "Com_RxDlMon.h"
#include "Com_ISRThreshold.h"
#include "Com_Notifications.h"
#include "Com_RxPduBuffer.h"
#include "Com_RxSigBuffer.h"
#include "Com_Caching.h"
#include "Com_EventCache.h"

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
  Com_RxPduProcessing_ThresholdCheck_Deferred
**********************************************************************************************************************/
/*! \brief            This function checks if the ISR Lock must be opened
    \details          -
    \pre              -
    \param[in]        mainFunctionId             id of the rx main function
    \param[in,out]    deferredFctPtrCacheStrct   Deferred function pointer cache which should be processed if ISR Lock will be opened
    \trace            DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context          TASK
    \reentrant        TRUE, for different handles
    \synchronous      TRUE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxPduProcessing_ThresholdCheck_Deferred(Com_MainFunctionRxStructIterType mainFunctionId, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) deferredFctPtrCacheStrct);

/**********************************************************************************************************************
  Com_RxPduProcessing_MainFunctionRx_IterateOverAllRxPdus
**********************************************************************************************************************/
/*! \brief             This function shall processes all deferred stored Rx PDUs.
    \details           -
    \pre               -
    \param[in]         mainFunctionId             ID of the calling rx main function.
    \param[in,out]     deferredFctPtrCacheStrct   Deferred function pointer cache which should be processed if ISR Lock will be opened
    \context           TASK
    \reentrant         TRUE, for different handles
    \synchronous       TRUE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxPduProcessing_MainFunctionRx_IterateOverAllRxPdus(Com_MainFunctionRxStructIterType mainFunctionId,
                                                                                              CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) deferredFctPtrCacheStrct);

/**********************************************************************************************************************
  Com_RxPduProcessing_MainFunctionRx_IterateOverCache
**********************************************************************************************************************/
/*! \brief             This function shall perform the processing of the deferred event cache.
    \details           -
    \pre               -
    \param[in]         mainFunctionId             ID of the calling rx main function.
    \param[in,out]     deferredFctPtrCacheStrct   Deferred function pointer cache
    \param[in]         rxEventCache               Deferred event cache
    \return            boolean
                          TRUE if fallback is necessary, otherwise FALSE
    \context           TASK
    \reentrant         TRUE, for different handles
    \synchronous       TRUE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_RxPduProcessing_MainFunctionRx_IterateOverCache(Com_MainFunctionRxStructIterType mainFunctionId,
                                                                                             CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) deferredFctPtrCacheStrct, Com_CEventCacheIterType rxEventCache);

/**********************************************************************************************************************
  Com_RxPduProcessing_ProcessDeferredPDU
**********************************************************************************************************************/
/*! \brief              This function processes a deferred PDU.
    \details            -
    \pre                -
    \param[in]          idxRxPduInfo                  Index of the PDU that should be processed.
    \param[in,out]      deferredfctPtrCacheStrctPtr   Pointer to struct,
                                                      which contains all informations for deferred function pointer caching.
    \context            TASK
    \reentrant          TRUE, for different handles
    \synchronous        TRUE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxPduProcessing_ProcessDeferredPDU(Com_RxPduInfoIterType idxRxPduInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) deferredfctPtrCacheStrctPtr);

/**********************************************************************************************************************
  Com_RxPduProcessing_ProcessPDU
**********************************************************************************************************************/
/*! \brief              This function processes a  PDU.
    \details            -
    \pre                -
    \param[in]          idxRxPduInfo          Index of the PDU that should be processed.
    \param[in]          PduInfoPtr            Payload information of the received I-PDU (pointer to data and data length).
    \param[in,out]      fctPtrCacheStrctPtr   Pointer to struct,
                                              which contains all informations for deferred function pointer caching.
    \context            TASK|ISR2
    \reentrant          TRUE, for different handles
    \synchronous        TRUE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxPduProcessing_ProcessPDU(Com_RxPduInfoIterType idxRxPduInfo, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                                     CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr);


/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxPduProcessing_ThresholdCheck_Deferred
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxPduProcessing_ThresholdCheck_Deferred(Com_MainFunctionRxStructIterType mainFunctionId, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) deferredFctPtrCacheStrct)
{
  const Com_ISRThresholdInfoIterType rxMainFunctionProcessingISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfMainFunctionRxStruct(mainFunctionId);
  if(Com_ISRThreshold_DecThresholdCounter(rxMainFunctionProcessingISRLockCounterIdx) == FALSE)
  {
    Com_ISRThreshold_ExitExclusiveArea(rxMainFunctionProcessingISRLockCounterIdx);
    {
      Com_Notifications_ProcessRxFctPtrCache(deferredFctPtrCacheStrct);
    }
    Com_ISRThreshold_EnterExclusiveArea(rxMainFunctionProcessingISRLockCounterIdx);
  }
}

/**********************************************************************************************************************
  Com_RxPduProcessing_MainFunctionRx_IterateOverAllRxPdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxPduProcessing_MainFunctionRx_IterateOverAllRxPdus(Com_MainFunctionRxStructIterType mainFunctionId,
                                                                                              CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) deferredFctPtrCacheStrct)
{
  /* Iterate over all Rx ComIPdus,  */
  for(Com_RxPduInfoOfMainFunctionRxIndIterType rxPduInfoIndIdx = Com_GetRxPduInfoOfMainFunctionRxIndStartIdxOfMainFunctionRxStruct(mainFunctionId);
      rxPduInfoIndIdx < Com_GetRxPduInfoOfMainFunctionRxIndEndIdxOfMainFunctionRxStruct(mainFunctionId); rxPduInfoIndIdx++)  /* FETA_COM_01 */
  {
    /*@ assert rxPduInfoIndIdx < Com_GetSizeOfRxPduInfoOfMainFunctionRxInd(); */ /* VCA_COM_CSL03 */
    const Com_RxPduInfoIterType idxRxPduInfo = Com_GetRxPduInfoOfMainFunctionRxInd(rxPduInfoIndIdx);
    if(Com_IPduGroupHdlr_IsRxPduActive(idxRxPduInfo))
    {
      /* If a Pdu is marked as stored (Length is > 0) */
      if(0u < Com_RxPduBuffer_GetLengthOfDeferredRxPdu(idxRxPduInfo))
      {
        /* Process the deferred Rx ComIPdu */
        Com_RxPduProcessing_ProcessDeferredPDU(idxRxPduInfo, deferredFctPtrCacheStrct);
      }
    }
    Com_RxPduProcessing_ThresholdCheck_Deferred(mainFunctionId, deferredFctPtrCacheStrct);
  }
}

/**********************************************************************************************************************
  Com_RxPduProcessing_MainFunctionRx_IterateOverCache
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_RxPduProcessing_MainFunctionRx_IterateOverCache(Com_MainFunctionRxStructIterType mainFunctionId,
                                                                                             CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) deferredFctPtrCacheStrct, Com_CEventCacheIterType rxEventCache)
{
  Com_RxPduInfoIterType idxRxPduInfo = 0u;
  boolean fallbackRequired = FALSE;   /* Cache is not full. */
  Com_EventCacheArrayLengthOfEventCacheType readLimit = Com_EventCache_GetReadLimit(rxEventCache);

  /* Iterate over all stored deferred events, and process them */
  while(Com_EventCache_Get(rxEventCache, &idxRxPduInfo) == E_OK)  /* FETA_COM_02 */
  {
    if(Com_IPduGroupHdlr_IsRxPduActive(idxRxPduInfo))
    {
      /*@ assert idxRxPduInfo < Com_GetSizeOfRxPduInfo(); */ /* VCA_COM_CSL01 */
      if(0u < Com_RxPduBuffer_GetLengthOfDeferredRxPdu(idxRxPduInfo))
      {
        Com_RxPduProcessing_ProcessDeferredPDU(idxRxPduInfo, deferredFctPtrCacheStrct);
      }
    }

    Com_RxPduProcessing_ThresholdCheck_Deferred(mainFunctionId, deferredFctPtrCacheStrct);

    readLimit--;

    /* If meanwhile new Pdus have been received and cache is full or the read threshold is reached, stop processing cache and set fallback strategy flag */
    if(Com_EventCache_IsFull(rxEventCache) || (readLimit == 0u))
    {
      Com_EventCache_Flush(rxEventCache); /* COV_COM_EVENT_CACHE_FLUSH */
      fallbackRequired = TRUE;
      break;
    }
  }

  return fallbackRequired;
}

/**********************************************************************************************************************
  Com_RxPduProcessing_ProcessDeferredPDU
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
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxPduProcessing_ProcessDeferredPDU(Com_RxPduInfoIterType idxRxPduInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) deferredfctPtrCacheStrctPtr)
{
  /* Create a PduInfoType with the stored SduLength and the stored deferred Pdu data buffer */
  const PduLengthType constSduLength = (PduLengthType) (Com_RxPduBuffer_GetLengthOfDeferredRxPdu(idxRxPduInfo) - 1u);
  PduInfoType PduInfo = { NULL_PTR, constSduLength };

  switch (Com_GetTypeOfRxPduInfo(idxRxPduInfo))
  {
    case COM_TP_TYPEOFRXPDUINFO:
    {
      /* If the Rx ComIPdu is a Tp Pdu use the Tp buffer as SduDataPtr */
      PduInfo.SduDataPtr = Com_LLRxTp_GetAddrRxTpBuffer(idxRxPduInfo);
      break;
    }
    case COM_NORMAL_TYPEOFRXPDUINFO:
    {
      /* Use the normal deferred Pdu buffer if present, else use NULL_PTR */
      PduInfo.SduDataPtr = Com_RxPduBuffer_GetAddrOfRxDefPduBuffer(idxRxPduInfo);
      break;
    }
    default: /* COV_COM_MISRA */
    /* Intentionally Empty */
      break;
  }

  /* Reset the "Handle Deferred" flag */
  Com_RxPduBuffer_ResetLengthOfDeferredRxPdu(idxRxPduInfo);

  /* Do Rx Signal Processing */
  Com_RxPduProcessing_ProcessPDU(idxRxPduInfo, &PduInfo, deferredfctPtrCacheStrctPtr);

  /* If the Rx ComIPdu is a Tp pdu, reset the tp parameter */
  if(Com_GetTypeOfRxPduInfo(idxRxPduInfo) == COM_TP_TYPEOFRXPDUINFO)
  {
    Com_LLRxTp_ResetRxTpConnection(idxRxPduInfo);
  }
}

/**********************************************************************************************************************
  Com_RxPduProcessing_ProcessPDU
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxPduProcessing_ProcessPDU(Com_RxPduInfoIterType idxRxPduInfo, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                                     CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr)
{
  /* Do Rx ComSignal processing for the passed Rx ComIPdu */
  Com_RxSignalProcessing_RxPduSigEvent(idxRxPduInfo, PduInfoPtr, fctPtrCacheStrctPtr);

  /* Do Rx ComSignalGroup processing for the passed Rx ComIPdu */
  Com_RxSignalProcessing_RxPduSigGrpEvent(idxRxPduInfo, PduInfoPtr, fctPtrCacheStrctPtr);

  /* If the passed Rx ComIPdu has a timeout, remove the timeout occurred flag */
  if(Com_IsRxTOutInfoUsedOfRxPduInfo(idxRxPduInfo))
  {
    Com_RxTOutInfoIdxOfRxPduInfoType rxTOutInfoIdx = Com_GetRxTOutInfoIdxOfRxPduInfo(idxRxPduInfo);
    /*@ assert rxTOutInfoIdx < Com_GetSizeOfRxTOutInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_RxDlMon_RemoveOccurredFlag(rxTOutInfoIdx);
  }
}


/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxPduProcessing_InitRxIpdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxPduProcessing_InitRxIpdu(Com_RxPduInfoIterType rxPduId)
{
  /* Iterate over all ComSignals and ComGroupSignals in the Pdu and initialize the corresponding buffers and shadow buffers */
  for(Com_RxAccessInfoIndIterType rxAccessInfoIndIdx = Com_GetRxAccessInfoIndStartIdxOfRxPduInfo(rxPduId);
      rxAccessInfoIndIdx < Com_GetRxAccessInfoIndEndIdxOfRxPduInfo(rxPduId); rxAccessInfoIndIdx++)  /* FETA_COM_01 */
  {
    const Com_RxAccessInfoIterType rxAccessInfoIdx = Com_GetRxAccessInfoInd(rxAccessInfoIndIdx);
    /*@ assert rxAccessInfoIdx < Com_GetSizeOfRxAccessInfo();*/ /* VCA_COM_CSL03 */
    Com_RxSigBuffer_InitRxSignalBuffer(rxAccessInfoIdx);
  }

  /* Iterate over all ComSignalGroups in the Pdu and if ArrayAccess is used initialize the corresponding shadow buffer */
  for(Com_RxSigGrpInfoIndIterType signalGroupIndIdx = Com_GetRxSigGrpInfoIndStartIdxOfRxPduInfo(rxPduId);
      signalGroupIndIdx < Com_GetRxSigGrpInfoIndEndIdxOfRxPduInfo(rxPduId); signalGroupIndIdx++)  /* FETA_COM_01 */
  {
    const Com_RxSigGrpInfoIterType signalGroupIdx = Com_GetRxSigGrpInfoInd(signalGroupIndIdx);
    Com_RxSigBuffer_InitRxSignalGroupBuffer(signalGroupIdx); /* VCA_COM_CSL03 */
  }

  /* If the Rx ComIPdu is handled deferred, reset the "Handle Deferred" flag */
  Com_RxPduBuffer_ResetLengthOfDeferredRxPdu(rxPduId);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
  /* If a gateway description exists, reset the gateway description handle */
  Com_DesGw_SetLengthOfHandleRxDeferredGwDescription(rxPduId, 0u);
#endif
}

/**********************************************************************************************************************
  Com_RxPduProcessing_MainFunctionRx
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
FUNC(void, COM_CODE) Com_RxPduProcessing_MainFunctionRx(Com_MainFunctionRxStructIterType mainFunctionId)
{
  const Com_ISRThresholdInfoIterType rxMainFunctionProcessingISRLockCounterIdx = Com_GetISRThresholdInfoIdxOfMainFunctionRxStruct(mainFunctionId);

  boolean fallbackRequired = TRUE;      /* Initially assume deferred event cache is full. */

  /* Initialize the deferredFctPtrCache cache to store notifications. If no cache is configured the cache has size 0 */
  ComCacheType deferredFctPtrCacheStrct = { 0 };
  uint32 fctPtrCache[COM_RXDEFERREDFCTPTRCACHEARRAYSIZE] = { 0u };
  Com_Caching_InitStackBasedCache(&deferredFctPtrCacheStrct, fctPtrCache, COM_RXDEFERREDFCTPTRCACHESIZE);

  Com_ISRThreshold_EnterExclusiveArea(rxMainFunctionProcessingISRLockCounterIdx);
  {
    /* If a DeferredRxEventCache exists */
    if(Com_IsEventCacheDeferredEventCacheUsedOfMainFunctionRxStruct(mainFunctionId))
    {
      const Com_CEventCacheIterType rxEventCache = Com_GetEventCacheDeferredEventCacheIdxOfMainFunctionRxStruct(mainFunctionId);

      /* If all received deferred processed events fitted into the deferred event cache */
      if(!Com_EventCache_IsFull(rxEventCache))
      {
        /* Iterate over all stored deferred events, and process them */
        fallbackRequired = Com_RxPduProcessing_MainFunctionRx_IterateOverCache(mainFunctionId, &deferredFctPtrCacheStrct, rxEventCache);
      }
    }

    /* If fallback strategy flag is set or cache is not configured, iterate over all Rx Pdus */
    if(fallbackRequired)
    {
      Com_RxPduProcessing_MainFunctionRx_IterateOverAllRxPdus(mainFunctionId, &deferredFctPtrCacheStrct);
    }
  }
  Com_ISRThreshold_ExitExclusiveArea(rxMainFunctionProcessingISRLockCounterIdx);

  /* Call all cached notification and invalid notification functions. */
  Com_Notifications_ProcessRxFctPtrCache(&deferredFctPtrCacheStrct);
}

/**********************************************************************************************************************
  Com_RxPduProcessing_ProcessImmediatePDU
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxPduProcessing_ProcessImmediatePDU(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                             CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) immediatefctPtrCacheStrctPtr)
{
  /* Do Rx Signal Processing */
  Com_RxPduProcessing_ProcessPDU(comRxPduInfoIdx, PduInfoPtr, immediatefctPtrCacheStrctPtr);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
  /* If rxPdu has any configured GwDescriptions, do gateway description processing */
  if(Com_IsGwTxPduDescriptionInfoUsedOfRxPduInfo(comRxPduInfoIdx))
  {
    SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
    {
      Com_DesGw_ImmediateEvent(comRxPduInfoIdx, PduInfoPtr);
    }
    SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
  }
#endif
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
 *  END OF FILE: Com_RxPduProcessing.c
 *********************************************************************************************************************/
