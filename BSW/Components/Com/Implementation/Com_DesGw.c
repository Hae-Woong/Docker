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
/*!        \file  Com_DesGw.c
 *         \unit  DesGw
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_DESGW_SOURCE

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
#include "Com_DesGw.h"
#include "Com_Cfg_DescriptionGatewayData.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_EventCache.h"
#include "Com_DesGwBc.h"
#include "Com_DesGwCp.h"
#include "Com_DesGwSp.h"
#include "Com_ISRThreshold.h"
#include "Com_RxPduBuffer.h"
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

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_DesGw_IsGwDestinationDescriptionTypeTriggered
**********************************************************************************************************************/
/*! \brief        This function checks if the description has a TRIGGERED* transferProperty
    \details      -
    \pre          -
    \param[in]    DescriptionType - Type of description
    \return       boolean
                    TRUE if a TRIGGERED* transferProperty is configured
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DesGw_IsGwDestinationDescriptionTypeTriggered(Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx);

/**********************************************************************************************************************
  Com_DesGw_IsGwDestinationDescriptionTypeOnChange
**********************************************************************************************************************/
/*! \brief        This function checks if the description has an ONCHANGE* transferProperty
    \details      -
    \pre          -
    \param[in]    DescriptionType - Type of description
    \return       boolean
                    TRUE if an ONCHANGE* transferProperty is configured
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DesGw_IsGwDestinationDescriptionTypeOnChange(Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx);

/**********************************************************************************************************************
  Com_DesGw_IsGwDestinationDescriptionTypeWithoutRepetition
**********************************************************************************************************************/
/*! \brief        This function checks if the description has an WITHOUTREPETITION* transferProperty
    \details      -
    \pre          -
    \param[in]    DescriptionType - Type of description
    \return       boolean
                    TRUE if a WITHOUTREPETITION* transferProperty is configured
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DesGw_IsGwDestinationDescriptionTypeWithoutRepetition(Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx);

/**********************************************************************************************************************
  Com_DesGw_ProcessEventCache
**********************************************************************************************************************/
/*! \brief        This function handles the processing of the GwEventCache.
    \details      -
    \pre          -
    \param[in]    mainFunctionId                           ID of the calling route signals main function.
    \param[in]    gwDescriptionProcessingISRLockCounterIdx index of the indirection from the main function route signals to the ISRLock counter
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DesGw_ProcessEventCache(Com_MainFunctionRouteSignalsStructIterType mainFunctionId,
                                                                  Com_ISRThresholdInfoIterType gwDescriptionProcessingISRLockCounterIdx);

/**********************************************************************************************************************
  Com_DesGw_DeferredEvent
**********************************************************************************************************************/
/*! \brief        This function handles the deferred gateway description events.
    \details      -
    \pre          -
    \param[in]    rxComPduId          Handle of the rx I-PDU.
    \param[in]    PduInfoPtr          contains the SDU buffer and the SDU length
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
    \spec
      requires rxComPduId < Com_GetSizeOfRxPduInfo();
      requires PduInfoPtr != NULL_PTR;
    \endspec    
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DesGw_DeferredEvent(Com_RxPduInfoIterType rxComPduId, P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr);

#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/
#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */

/**********************************************************************************************************************
  Com_DesGw_IsGwDestinationDescriptionTypeTriggered
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DesGw_IsGwDestinationDescriptionTypeTriggered(Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx)
{
  return (Com_GetTypeOfGwDescriptionAccessInfo(gwDescrAccessIdx) != COM_PENDING_TYPEOFGWDESCRIPTIONACCESSINFO);
}

/**********************************************************************************************************************
  Com_DesGw_IsGwDestinationDescriptionTypeOnChange
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DesGw_IsGwDestinationDescriptionTypeOnChange(Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx)
{
  return ((Com_GetTypeOfGwDescriptionAccessInfo(gwDescrAccessIdx) == COM_TRIGGERED_ON_CHANGE_TYPEOFGWDESCRIPTIONACCESSINFO)
       || (Com_GetTypeOfGwDescriptionAccessInfo(gwDescrAccessIdx) == COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION_TYPEOFGWDESCRIPTIONACCESSINFO));
}

/**********************************************************************************************************************
  Com_DesGw_IsGwDestinationDescriptionTypeWithoutRepetition
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DesGw_IsGwDestinationDescriptionTypeWithoutRepetition(Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx)
{
  return ((Com_GetTypeOfGwDescriptionAccessInfo(gwDescrAccessIdx) == COM_TRIGGERED_WITHOUT_REPETITION_TYPEOFGWDESCRIPTIONACCESSINFO)
       || (Com_GetTypeOfGwDescriptionAccessInfo(gwDescrAccessIdx) == COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION_TYPEOFGWDESCRIPTIONACCESSINFO));
}

/**********************************************************************************************************************
  Com_DesGw_ProcessEventCache
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DesGw_ProcessEventCache(Com_MainFunctionRouteSignalsStructIterType mainFunctionId,
                                                                  Com_ISRThresholdInfoIterType gwDescriptionProcessingISRLockCounterIdx)
{
  /* If the Gw Event Cache is used */
  if(Com_IsEventCacheGatewayEventCacheUsedOfMainFunctionRouteSignalsStruct(mainFunctionId))
  {
    Com_RxPduInfoIterType idxRxPduInfo = 0u;
    const Com_CEventCacheIterType GwEventCache = Com_GetEventCacheGatewayEventCacheIdxOfMainFunctionRouteSignalsStruct(mainFunctionId);
    Com_EventCacheArrayLengthOfEventCacheType readLimit = Com_EventCache_GetReadLimit(GwEventCache);

    /* Iterate over all ComIPdus inside the g\w event cache */
    while((Com_EventCache_Get(GwEventCache, &idxRxPduInfo) == E_OK) && (readLimit != 0u))  /* FETA_COM_02 */
    {
      /*@ assert idxRxPduInfo < Com_GetSizeOfRxPduInfo(); */ /* VCA_COM_CSL01 */

      if(Com_IsHandleRxDeferredGwDescriptionUsedOfRxPduInfo(idxRxPduInfo))
      {
        const Com_HandleRxDeferredGwDescriptionType gwDeferredHandleSize = Com_GetHandleRxDeferredGwDescription(Com_GetHandleRxDeferredGwDescriptionIdxOfRxPduInfo(idxRxPduInfo));

        /* If gw flag has not been reset */
        if(gwDeferredHandleSize > 0u)
        {
          /* The Pdu is marked as stored by storing the SduLength + 1 in the RxPduBuffer, because of ZeroBit Pdus. To retain the SduLength a subtraction by 1 has to be performed */
          const PduLengthType constSduLength = (PduLengthType) (gwDeferredHandleSize - 1u);
          SduDataPtrType constSduDataPtr = Com_RxPduBuffer_GetAddrOfRxDefPduBuffer(idxRxPduInfo);
          const PduInfoType PduInfo = { constSduDataPtr, constSduLength };

          /* Process gateway description event */
          Com_DesGw_DeferredEvent(idxRxPduInfo, &PduInfo);
        }

        /* Reset deferred gw description flag */
        Com_SetHandleRxDeferredGwDescription(Com_GetHandleRxDeferredGwDescriptionIdxOfRxPduInfo(idxRxPduInfo), 0);  /*@ assert Com_GetHandleRxDeferredGwDescriptionIdxOfRxPduInfo(idxRxPduInfo) < Com_GetSizeOfHandleRxDeferredGwDescription(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      }
      Com_ISRThreshold_ThresholdCheck(gwDescriptionProcessingISRLockCounterIdx);

      readLimit--;
    }
  }
}

/**********************************************************************************************************************
  Com_DesGw_DeferredEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DesGw_DeferredEvent(Com_RxPduInfoIterType rxComPduId, P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  /* Iterate over all corresponding destination TxPDUs */
  for(Com_GwTxPduDescriptionInfoIterType gwTxPduIdx = Com_GetGwTxPduDescriptionInfoStartIdxOfRxPduInfo(rxComPduId);
      gwTxPduIdx < Com_GetGwTxPduDescriptionInfoEndIdxOfRxPduInfo(rxComPduId); gwTxPduIdx++)  /* FETA_COM_01 */
  {
    /* If the indirection to the CrossPartitionRoutingQueue is used it is a cross partition routing */
    /*@ assert gwTxPduIdx < Com_GetSizeOfGwTxPduDescriptionInfo(); */ /* VCA_COM_CSL03 */
    if(Com_IsCprQueueUsedOfGwTxPduDescriptionInfo(gwTxPduIdx))
    {
      Com_DesGwCp_ProcessRoutingRx(PduInfoPtr, gwTxPduIdx); 
    }
    /* Else process partition local routing */
    else
    {
      Com_DesGwSp_ProcessEvent(PduInfoPtr, gwTxPduIdx);
    }
  }
}

# endif
/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_DesGw_MainFunction
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DesGw_MainFunction(Com_MainFunctionRouteSignalsStructIterType mainFunctionId)
{
  const Com_ISRThresholdInfoIterType gwDescriptionProcessingISRLockCounterIdx =
    Com_GetISRThresholdInfoGwDescriptionIdxOfMainFunctionRouteSignalsStruct(mainFunctionId);

  Com_ISRThreshold_EnterExclusiveArea(gwDescriptionProcessingISRLockCounterIdx);

  /* Process the Gw Event Cache */
  Com_DesGw_ProcessEventCache(mainFunctionId, gwDescriptionProcessingISRLockCounterIdx);

  /* Process Cross Partition Routing Queues leading to this partition */
  Com_DesGwCp_ProcessQueuesTx(mainFunctionId, gwDescriptionProcessingISRLockCounterIdx);

  Com_ISRThreshold_ExitExclusiveArea(gwDescriptionProcessingISRLockCounterIdx);
}

/**********************************************************************************************************************
  Com_DesGw_ImmediateEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DesGw_ImmediateEvent(Com_RxPduInfoIterType rxComPduId, P2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  /* Iterate over all corresponding destination TxPDUs */
  for(Com_GwTxPduDescriptionInfoIterType gwTxPduIdx = Com_GetGwTxPduDescriptionInfoStartIdxOfRxPduInfo(rxComPduId);
      gwTxPduIdx < Com_GetGwTxPduDescriptionInfoEndIdxOfRxPduInfo(rxComPduId); gwTxPduIdx++)  /* FETA_COM_01 */
  {
    Com_DesGwSp_ProcessEvent(PduInfoPtr, gwTxPduIdx); /*@ assert gwTxPduIdx < Com_GetSizeOfTxPduInfo(); */ /* VCA_COM_CSL03 */
  }
}

/**********************************************************************************************************************
  Com_DesGw_ProcessDestinationDescription
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 ********************************************************************************************************************/
FUNC(Com_DescriptionGwTriggerType, COM_CODE) Com_DesGw_ProcessDestinationDescription(P2CONST(uint8, AUTOMATIC, AUTOMATIC) sourceBuffer,
                                                                                     Com_GwDescriptionAccessInfoIterType gwDescrAccessIdx)
{
  Com_DescriptionGwTriggerType retVal = COM_TRANSMIT_TRIGGER_NOTFULFILLED;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  boolean triggerCondition = TRUE;

  /* Copy configured bytes into destination txPduBuffers */
  const boolean hasContentChanged = Com_DesGwBc_CopyBuffer(sourceBuffer, gwDescrAccessIdx); /* PRQA S 2983 */ /* MD_Com_2982_2983 */

  if(Com_DesGw_IsGwDestinationDescriptionTypeOnChange(gwDescrAccessIdx))       /* TRIGGERED ON CHANGE */
  {
    triggerCondition = hasContentChanged;
  }

  /* If transferProperty is not type of ON-CHANGE or content of description has changed */
  if(triggerCondition)
  {
    /* Set TRIGGERED and WITHOUTREPETITION FLAG if transfer property is TriggeredWithoutRepetition or TriggeredOnChangeWithoutRepetition */
    if(Com_DesGw_IsGwDestinationDescriptionTypeWithoutRepetition(gwDescrAccessIdx))
    {
      retVal = COM_TRANSMIT_TRIGGERWITHOUTREP;
    }
    /* otherwise set TRIGGERED FLAG if transfer property is Triggered */
    else if(Com_DesGw_IsGwDestinationDescriptionTypeTriggered(gwDescrAccessIdx))
    {
      retVal = COM_TRANSMIT_TRIGGER;
    }
    else
    {
      /* otherwise transfer property is assumed to be PENDING */
      retVal = COM_TRANSMIT_TRIGGER_NOTFULFILLED;
    }
  }

  Com_Transmission_SetUpdateBitGwDescriptionAccess(gwDescrAccessIdx);

  return retVal;
}

/**********************************************************************************************************************
  Com_DesGw_GetLengthOfHandleRxDeferredGwDescription
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_DesGw_GetLengthOfHandleRxDeferredGwDescription(Com_RxPduInfoIterType comRxPduInfoIdx)
{
  PduLengthType retVal = 255u;
  if(Com_IsGwTxPduDescriptionInfoUsedOfRxPduInfo(comRxPduInfoIdx) && Com_IsHandleRxDeferredGwDescriptionUsedOfRxPduInfo(comRxPduInfoIdx))
  {
    retVal = Com_GetHandleRxDeferredGwDescription(Com_GetHandleRxDeferredGwDescriptionIdxOfRxPduInfo(comRxPduInfoIdx));
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_DesGw_SetLengthOfHandleRxDeferredGwDescription
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DesGw_SetLengthOfHandleRxDeferredGwDescription(Com_RxPduInfoIterType comRxPduInfoIdx, PduLengthType deferredPduLength)
{
  if(Com_IsGwTxPduDescriptionInfoUsedOfRxPduInfo(comRxPduInfoIdx) && Com_IsHandleRxDeferredGwDescriptionUsedOfRxPduInfo(comRxPduInfoIdx))
  {
    const Com_HandleRxDeferredGwDescriptionIterType handleRxDeferred = Com_GetHandleRxDeferredGwDescriptionIdxOfRxPduInfo(comRxPduInfoIdx);
    /*@ assert handleRxDeferred < Com_GetSizeOfHandleRxDeferredGwDescription(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_SetHandleRxDeferredGwDescription(handleRxDeferred, deferredPduLength);
  }
}

#endif

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/

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
 *  END OF FILE: Com_DesGw.c
 *********************************************************************************************************************/
