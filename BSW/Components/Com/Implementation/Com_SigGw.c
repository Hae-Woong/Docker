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
/*!        \file  Com_SigGw.c
 *         \unit  SigGw
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_SIGGW_SOURCE

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
#include "Com_SigGw.h"
#include "Com_Cfg_SignalGatewayData.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_ISRThreshold.h"
#include "Com_SigGwCP.h"
#include "Com_SigGwSP.h"

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_SigGw_ProcessGwEvents
**********************************************************************************************************************/
/*! \brief        This function processes the signal gateway events.
    \details      -
    \pre          -
    \param[in]    mainFunctionId  Idx of the main function route signals
    \param[in]    gwProcessingISRlockCounterIdx index of the indirection from the main function route signals to the ISRLock counter
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SigGw_ProcessGwEvents(Com_MainFunctionRouteSignalsStructIterType mainFunctionId, Com_ISRThresholdInfoIterType gwProcessingISRlockCounterIdx);

/**********************************************************************************************************************
  Com_SigGw_SignalRoutings
**********************************************************************************************************************/
/*! \brief        This function processes the routing of the passed ComSignal.
    \details      -
    \pre          -
    \param[in]    idxGwInfo  Index of GwInfo
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SigGw_SignalRoutings(Com_GwInfoIterType idxGwInfo);

/**********************************************************************************************************************
  Com_SigGw_SignalGroupRoutings
**********************************************************************************************************************/
/*! \brief        This function processes the routing of the passed ComSignalGroup
    \details      -
    \pre          -
    \param[in]    idxGwInfo  Index of GwInfo
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SigGw_SignalGroupRoutings(Com_GwInfoIterType idxGwInfo);
#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_SigGw_ProcessGwEvents
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SigGw_ProcessGwEvents(Com_MainFunctionRouteSignalsStructIterType mainFunctionId, Com_ISRThresholdInfoIterType gwProcessingISRlockCounterIdx)
{
  /* Iterate over all gateway event flags */
  for(Com_GwInfoIndIterType gwInfoIndIdx = Com_GetGwInfoIndStartIdxOfMainFunctionRouteSignalsStruct(mainFunctionId);
      gwInfoIndIdx < Com_GetGwInfoIndEndIdxOfMainFunctionRouteSignalsStruct(mainFunctionId); gwInfoIndIdx++)  /* FETA_COM_01 */
  {
    /*@ assert gwInfoIndIdx < Com_GetSizeOfGwInfoInd(); */ /* VCA_COM_CSL03 */
    const Com_GwInfoIterType gwInfoIdx = Com_GetGwInfoInd(gwInfoIndIdx);

    /* If a gateway event flag is set */
    if(Com_IsGwEvent(gwInfoIdx))
    {
      Com_SetGwEvent(gwInfoIdx, FALSE);

      /* Do normal signal routings */
      Com_SigGw_SignalRoutings(gwInfoIdx);

      /* Do normal signal group routings */
      Com_SigGw_SignalGroupRoutings(gwInfoIdx);
    }
    Com_ISRThreshold_ThresholdCheck(gwProcessingISRlockCounterIdx);
  }
}

/**********************************************************************************************************************
  Com_SigGw_SignalRoutings
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SigGw_SignalRoutings(Com_GwInfoIterType idxGwInfo)
{
  /* Iterate over all signal depending gateway mappings of the passed gateway info */
  /* Non-least type is used due to usage as function parameter in Com_SigGwCP_SignalRoutingRx */
  for(Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping = Com_GetGwSigMappingStartIdxOfGwInfo(idxGwInfo);
      idxGwSigMapping < Com_GetGwSigMappingEndIdxOfGwInfo(idxGwInfo); idxGwSigMapping++)  /* FETA_COM_01 */
  {
    /* If the indirection to the CrossPartitionRoutingQueue is used process cross partition routing */
    if(Com_IsCprQueueUsedOfGwSigMapping(idxGwSigMapping))
    {
      /*@ assert idxGwSigMapping < Com_GetSizeOfGwSigMapping(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      Com_SigGwCP_SignalRoutingRx(idxGwSigMapping);
    }
    /* Else process partition local routing */
    else
    {
      /*@ assert idxGwSigMapping < Com_GetSizeOfGwSigMapping(); */ /* VCA_COM_CSL03 */
      Com_SigGwSP_SignalRouting(idxGwSigMapping);
    }
  }
}

/**********************************************************************************************************************
  Com_SigGw_SignalGroupRoutings
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SigGw_SignalGroupRoutings(Com_GwInfoIterType idxGwInfo)
{
  /* Iterate over all signal group depending gateway mappings of the passed gateway info */
  /* Non-least type is used due to usage as function parameter in Com_SigGwCP_SignalGroupRoutingRx */
  for(Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping = Com_GetGwSigGrpMappingStartIdxOfGwInfo(idxGwInfo);
      idxGwSigGrpMapping < Com_GetGwSigGrpMappingEndIdxOfGwInfo(idxGwInfo); idxGwSigGrpMapping++)  /* FETA_COM_01 */
  {
    /*@ assert idxGwSigGrpMapping < Com_GetSizeOfGwSigGrpMapping(); */ /* VCA_COM_CSL03 */

    /* If the indirection to the CrossPartitionRoutingQueue is used process cross partition routing */
    if(Com_IsCprQueueUsedOfGwSigGrpMapping(idxGwSigGrpMapping))
    {
      Com_SigGwCP_SignalGroupRoutingRx(idxGwSigGrpMapping);
    }
    /* Else process partition local routing */
    else
    {
      Com_SigGwSP_SignalGroupRouting(idxGwSigGrpMapping);
    }
  }
}
#endif

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_SigGw_InitRxIpdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGw_InitRxIpdu(Com_RxPduInfoIterType rxPduId)
{
  /* Reset all gateway event flags of the passed ComIPdu */
  for(Com_GwEventIterType idxGwInfo = Com_GetGwInfoStartIdxOfRxPduInfo(rxPduId);
      idxGwInfo < Com_GetGwInfoEndIdxOfRxPduInfo(rxPduId); idxGwInfo++)  /* FETA_COM_01 */
  {
    /*@ assert idxGwInfo < Com_GetSizeOfGwEvent(); */ /* VCA_COM_INDIRECTION_CSL02_CSL03 */
    Com_SetGwEvent(idxGwInfo, FALSE);
  }
}

/**********************************************************************************************************************
  Com_SigGw_SigEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGw_SigEvent(Com_RxSigInfoIterType idxRxSigInfo)
{
  /* If the passed signal is routed, set the related gateway event flag */
  if(Com_IsGwInfoUsedOfRxSigInfo(idxRxSigInfo))
  {
    Com_SetGwEvent(Com_GetGwInfoIdxOfRxSigInfo(idxRxSigInfo), TRUE);
  }
}

/**********************************************************************************************************************
  Com_SigGw_SigGrpEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGw_SigGrpEvent(Com_RxSigGrpInfoIterType idxRxSigGrpInfo)
{
  /* If the passed signal group is routed, set the related gateway event flag */
  if(Com_IsGwInfoUsedOfRxSigGrpInfo(idxRxSigGrpInfo))
  {
    Com_SetGwEvent(Com_GetGwInfoIdxOfRxSigGrpInfo(idxRxSigGrpInfo), TRUE);
  }
}

/**********************************************************************************************************************
  Com_SigGw_SetGwEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGw_SetGwEvent(Com_RxAccessInfoIterType idxRxAccessInfo)
{
  if(Com_IsGwInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_GwEventIterType gwEventIdx = Com_GetGwInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert gwEventIdx < Com_GetSizeOfGwEvent(); */ /* VCA_COM_INDIRECTION_CSL02_CSL03 */
    Com_SetGwEvent(gwEventIdx, TRUE);
  }
}

/**********************************************************************************************************************
  Com_SigGw_MainFunction
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGw_MainFunction(Com_MainFunctionRouteSignalsStructIterType mainFunctionId)
{
  if(Com_HasGwInfo())
  {
    const Com_ISRThresholdInfoIterType gwProcessingISRlockCounterIdx = Com_GetISRThresholdInfoGwIdxOfMainFunctionRouteSignalsStruct(mainFunctionId);

    Com_ISRThreshold_EnterExclusiveArea(gwProcessingISRlockCounterIdx);
    {
      /* Process GwEvents */
      Com_SigGw_ProcessGwEvents(mainFunctionId, gwProcessingISRlockCounterIdx);

      /* Process Cross Partition Routing Queues leading to this partition */
      Com_SigGwCP_ProcessCrossPartitionQueuesTx(mainFunctionId, gwProcessingISRlockCounterIdx);
    }
    Com_ISRThreshold_ExitExclusiveArea(gwProcessingISRlockCounterIdx);
  }
}
#endif

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
 *  END OF FILE: Com_SigGw.c
 *********************************************************************************************************************/
