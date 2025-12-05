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
/*!        \file  Com_LLRxIf.c
 *         \unit  LLRxIf
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_LLRXIF_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_LLRxIf.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_Reporting.h" /* Com_GetSizeOfReporting() for VCA */

#include "Com_RxPduProcessing.h"
#include "Com_Reporting.h"
#include "Com_Caching.h"
#include "Com_EventCache.h"
#include "Com_RxPduBuffer.h"
#include "Com_IPduGroupHdlr.h"
#include "Com_Notifications.h"
#include "Com_DesGw.h"

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
  Com_LLRxIf_RxIndicationDeferredProcessing
**********************************************************************************************************************/
/*! \brief        This function copies the passed Sdu data to the deferred Pdu buffer and caches the PDU.
    \details      -
    \pre          -
    \param[in]    comRxPduInfoIdx Handle of the I-PDU.
    \param[in]    PduInfoPtr      contains the SDU buffer and the SDU length
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_LLRxIf_RxIndicationDeferredProcessing(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

/**********************************************************************************************************************
  Com_LLRxIf_RxIndicationImmediateProcessing
**********************************************************************************************************************/
/*! \brief        This function processes the PDU immediately.
    \details      -
    \pre          -
    \param[in]    comRxPduInfoIdx Handle of the I-PDU.
    \param[in]    PduInfoPtr      contains the SDU buffer and the SDU length
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_LLRxIf_RxIndicationImmediateProcessing(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_LLRxIf_RxIndicationDeferredProcessing
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_LLRxIf_RxIndicationDeferredProcessing(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  /* If a DeferredRxEventCache exists */
  if(Com_IsMainFunctionRxStructUsedOfRxPduInfo(comRxPduInfoIdx) && Com_IsEventCacheDeferredEventCacheUsedOfMainFunctionRxStruct(Com_GetMainFunctionRxStructIdxOfRxPduInfo(comRxPduInfoIdx))) /* COV_COM_DEFERRED_MAINFUNCTION_RX */
  {
    /* If the passed ComIPdu is not cached yet, try to add the Rx ComIPdu to the deferred event cache */
    if(Com_RxPduBuffer_GetLengthOfDeferredRxPdu(comRxPduInfoIdx) == 0u)
    {
      const Com_CEventCacheIterType RxEventCache = Com_GetEventCacheDeferredEventCacheIdxOfMainFunctionRxStruct(Com_GetMainFunctionRxStructIdxOfRxPduInfo(comRxPduInfoIdx));
      (void) Com_EventCache_Put(RxEventCache, comRxPduInfoIdx);
    }
  }

  /* Copy the passed Sdu data to the deferred Pdu buffer and store the passed Sdu length in the handle deferred buffer */
  Com_RxPduBuffer_CopyToRxDefPduBuffer(comRxPduInfoIdx, PduInfoPtr);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
  /* If a GwEventCache exists */
  if(Com_IsMainFunctionRouteSignalsStructUsedOfRxPduInfo(comRxPduInfoIdx) && Com_IsEventCacheGatewayEventCacheUsedOfMainFunctionRouteSignalsStruct(Com_GetMainFunctionRouteSignalsStructIdxOfRxPduInfo(comRxPduInfoIdx)))
  {
    /* If a gateway description exists for this PDU and the PDU is not already cached */
    if(Com_DesGw_GetLengthOfHandleRxDeferredGwDescription(comRxPduInfoIdx) == 0u)
    {
      /* Store PDU Id in the gateway event cache */
      const Com_CEventCacheIterType GwEventCache = Com_GetEventCacheGatewayEventCacheIdxOfMainFunctionRouteSignalsStruct(Com_GetMainFunctionRouteSignalsStructIdxOfRxPduInfo(comRxPduInfoIdx));
      const Std_ReturnType retVal = Com_EventCache_Put(GwEventCache, comRxPduInfoIdx);
      if (retVal == E_OK)
      {
        Com_DesGw_SetLengthOfHandleRxDeferredGwDescription(comRxPduInfoIdx, Com_RxPduBuffer_GetLengthOfDeferredRxPdu(comRxPduInfoIdx));
      }
    }
  }
#endif
}

/**********************************************************************************************************************
  Com_LLRxIf_RxIndicationImmediateProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_LLRxIf_RxIndicationImmediateProcessing(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  /* Initialize the cache to store notifications. If no cache is configured the cache has size 0 */
  ComCacheType immediateFctPtrCacheStrct = { 0 };
  uint32 fctPtrCache[COM_RXIMMEDIATEFCTPTRCACHEARRAYSIZE] = { 0 };
  Com_Caching_InitStackBasedCache(&immediateFctPtrCacheStrct, fctPtrCache, COM_RXIMMEDIATEFCTPTRCACHESIZE);

  /* Process the PDU immediately */
  Com_RxPduProcessing_ProcessImmediatePDU(comRxPduInfoIdx, PduInfoPtr, &immediateFctPtrCacheStrct);

  /* Call all cached notification or invalid notifications and clear the immediate function pointer cache */
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
  {
    Com_Notifications_ProcessRxFctPtrCache(&immediateFctPtrCacheStrct);
  }
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
}


/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_LLRxIf_RxIndicationProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(boolean, COM_CODE) Com_LLRxIf_RxIndicationProcessing(Com_RxPduInfoIterType rxPduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* Call the IPduCallout if configured */
  if(Com_IsReportingRxIPduCalloutUsedOfRxPduInfo(rxPduId))
  {
    Com_ReportingRxIPduCalloutIdxOfRxPduInfoType reportingRxIPduCalloutIdx = Com_GetReportingRxIPduCalloutIdxOfRxPduInfo(rxPduId);
    /*@ assert reportingRxIPduCalloutIdx < Com_GetSizeOfReporting(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    retVal = Com_Reporting_CallRxIpduCallout(reportingRxIPduCalloutIdx, rxPduId, PduInfoPtr);
  }
  /* If the callout result is positive or not configured */
  if(retVal)
  {
    SduDataPtrType constSduDataPtr = PduInfoPtr->SduDataPtr;
    const PduLengthType constSduLength = PduInfoPtr->SduLength - (PduLengthType) Com_GetMetaDataLengthOfRxPduInfo(rxPduId);  /* PRQA S 2985 */ /* MD_Com_2985_2986 */
    const PduInfoType PduInfoLocal = { constSduDataPtr, constSduLength };

    /* If the passed Rx ComIPdu is handled deferred copy the passed Sdu data to the deferred Pdu buffer and cache the PDU */
    Com_HandleRxPduDeferredUsedOfRxPduInfoType hasDeferredHandle = Com_IsHandleRxPduDeferredUsedOfRxPduInfo(rxPduId);
    if(hasDeferredHandle)
    {
      Com_LLRxIf_RxIndicationDeferredProcessing(rxPduId, &PduInfoLocal);
    }
    else /* Otherwise process the Rx ComIPdu immediately */
    {
      Com_LLRxIf_RxIndicationImmediateProcessing(rxPduId, &PduInfoLocal);
    }
  }   /* Rx I-PDU callout */
  return retVal;
}

/**********************************************************************************************************************
  Com_LLRxIf_IfRxIndicationProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(boolean, COM_CODE) Com_LLRxIf_IfRxIndicationProcessing(Com_RxPduInfoIterType rxPduId, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* If the ComIPdu is active */
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
  {
    if(Com_IPduGroupHdlr_IsRxPduActive(rxPduId))  /* \trace SPEC-2736845 */ /* \trace SPEC-2736846 */
    {
      retVal = Com_LLRxIf_RxIndicationProcessing(rxPduId, PduInfoPtr);
    }
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
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
 *  END OF FILE: Com_LLRxIf.c
 *********************************************************************************************************************/
