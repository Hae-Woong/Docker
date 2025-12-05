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
/*!        \file  Com_SigGwCP.c
 *         \unit  SigGwCP
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_SIGGWCP_SOURCE

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
#include "Com_SigGwCP.h"
#include "Com_Cfg_SignalGatewayData.h"
#include "Com_Cfg_CrossPartitionQueueData.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_Transmission.h"
#include "Com_CprQueueSigGw.h"
#include "Com_Reporting.h"
#include "Com_GwTout.h"
#include "Com_ISRThreshold.h"
#include "Com_RxSigBuffer.h"

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
  Com_SigGwCP_SignalGroupRoutingTx
**********************************************************************************************************************/
/*! \brief        This function processes the cross partition routing for signals on tx side.
    \details      -
    \pre          -
    \param[in]    routingQueueIndex  Idx of the cross partition routing queue.
    \param[in]    readIdx            Index to the position in the queue where the data shall be read.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SigGwCP_SignalGroupRoutingTx(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx);

/**********************************************************************************************************************
  Com_SigGwCP_SignalRoutingTx
**********************************************************************************************************************/
/*! \brief        This function processes the cross partition routing for signal routings on tx side.
    \details      -
    \pre          -
    \param[in]    routingQueueIndex  Idx of the cross partition routing queue.
    \param[in]    readIdx            Index to the position in the queue where the data shall be read.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SigGwCP_SignalRoutingTx(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx);
#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

# if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_SigGwCP_SignalGroupRoutingTx
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
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SigGwCP_SignalGroupRoutingTx(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx)
{
  /* Non-least type is used due to usage as function parameter, return by reference, in Com_CprQueueSigGw_Read_SignalGroupId */
  Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping = 0u;

  /* Read SignalGroup Index from Queue */
  Com_CprQueueSigGw_Read_SignalGroupId(routingQueueIndex, readIdx, &idxGwSigGrpMapping);

  if(idxGwSigGrpMapping < Com_GetSizeOfGwSigGrpMapping())
  {
    const Com_TxSigGrpInfoIterType txSigGrpId = Com_GetTxSigGrpInfoIdxOfGwSigGrpMapping(idxGwSigGrpMapping);

    /* Handle Gw Timeout if configured */
    Com_GwTout_Event(Com_GetTxPduInfoIdxOfTxSigGrpInfo(txSigGrpId));

    /* Iterate over all related group signal mappings */
    for(Com_GwGrpSigMappingIterType idxGwGrpSigMapping = Com_GetGwGrpSigMappingStartIdxOfGwSigGrpMapping(idxGwSigGrpMapping);
        idxGwGrpSigMapping < Com_GetGwGrpSigMappingEndIdxOfGwSigGrpMapping(idxGwSigGrpMapping); idxGwGrpSigMapping++)  /* FETA_COM_01 */
    {
      /* Read GroupSignal from Queue */
      ComDataTypeVar dataVar = { NULL_PTR, 0u };
      Com_CprQueueSigGw_Read_GroupSignal(routingQueueIndex, idxGwGrpSigMapping, &dataVar);

      /* Process GroupSignal */
      const Com_TxGrpSigInfoIterType txSigId = Com_GetTxGrpSigInfoIdxOfGwGrpSigMapping(idxGwGrpSigMapping);
      const Com_TxAccessInfoIterType txAccessInfo = Com_GetTxAccessInfoIdxOfTxGrpSigInfo(txSigId);
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = dataVar.dataPtr;
      const ComDataTypeConst data = { constDataPtr, dataVar.dataLength };
      (void) Com_Transmission_SendSignal_Processing(txAccessInfo, &data);  /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
    }

    /* Call Com_SendSignalGroup for the related signal group with the respective shadowBuffer */
    P2CONST(uint8, AUTOMATIC, AUTOMATIC) shdBuffer = NULL_PTR; /* PRQA S 2983 */ /* MD_Com_2982_2983 */
    if(Com_IsTxBufferInfoUsedOfTxSigGrpInfo(txSigGrpId))
    {
      shdBuffer = Com_Transmission_GetAddrTxBufferFirstElement(Com_GetTxBufferInfoIdxOfTxSigGrpInfo(txSigGrpId));
    }
    (void) Com_Transmission_SendSignalGroup_Processing(txSigGrpId, shdBuffer);
  }
}

/**********************************************************************************************************************
  Com_SigGwCP_SignalRoutingTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SigGwCP_SignalRoutingTx(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx)
{
  /* Non-least type is used due to usage as function parameter, return by reference, in Com_CprQueueSigGw_Read_Signal */
  Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping = 0u;
  ComDataTypeVar dataVar = { NULL_PTR, 0u };

  /* Read Signal from Queue */
  Com_CprQueueSigGw_Read_Signal(routingQueueIndex, readIdx, &idxGwSigMapping, &dataVar);

  if(idxGwSigMapping < Com_GetSizeOfGwSigMapping())
  {
    /* Handle Gw Timeout if configured */
    const Com_TxSigInfoIterType txSigId = Com_GetTxSigInfoIdxOfGwSigMapping(idxGwSigMapping);
    Com_GwTout_Event(Com_GetTxPduInfoIdxOfTxSigInfo(txSigId));

    /* Process Signal */
    const Com_TxAccessInfoIterType txAccessInfo = Com_GetTxAccessInfoIdxOfTxSigInfo(txSigId);
    CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = dataVar.dataPtr;
    const ComDataTypeConst data = { constDataPtr, dataVar.dataLength };
    (void) Com_Transmission_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
  }
}
#endif

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
  Com_SigGwCP_SignalGroupRoutingRx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGwCP_SignalGroupRoutingRx(Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping)
{
  const Com_CCprQueueIterType routingQueueIndex = Com_GetCprQueueIdxOfGwSigGrpMapping(idxGwSigGrpMapping);
  const Com_SignalGroupLengthInByteOfGwSigGrpMappingType length = Com_GetSignalGroupLengthInByteOfGwSigGrpMapping(idxGwSigGrpMapping);

  /* Write signal group to the CrossPartitionRoutingQueue. */
  if(Com_CprQueueSigGw_Write_SignalGroup(routingQueueIndex, idxGwSigGrpMapping, length) != E_OK)
  {
    Com_Reporting_CallDetReportError(COMServiceId_CprQueueSigGw_Write_SignalGroup, COM_E_CPRQUEUE_OVERFLOW);
  }
}

/**********************************************************************************************************************
  Com_SigGwCP_SignalRoutingRx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGwCP_SignalRoutingRx(Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping)
{
  const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoIdxOfGwSigMapping(idxGwSigMapping);

  /* Get the pointer to the source buffer and length of signal */
  CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = Com_RxSigBuffer_GetGatewaySourcePtr(idxRxAccessInfo);
  const PduLengthType constDataLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);
  const ComDataTypeConst data = { constDataPtr, constDataLength };

  /* Write gateway description into Queue. */
  if(Com_CprQueueSigGw_Write_Signal(Com_GetCprQueueIdxOfGwSigMapping(idxGwSigMapping), idxGwSigMapping, &data) != E_OK)
  {
    Com_Reporting_CallDetReportError(COMServiceId_CprQueueSigGw_Write_Signal, COM_E_CPRQUEUE_OVERFLOW);
  }
}

/**********************************************************************************************************************
  Com_SigGwCP_ProcessCrossPartitionQueuesTx
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
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SigGwCP_ProcessCrossPartitionQueuesTx(Com_MainFunctionRouteSignalsStructIterType mainFunctionId,
                                                                                 Com_ISRThresholdInfoIterType gwProcessingISRlockCounterIdx)
{
  /* If cross partition routing queues leading to this partition are used */
  if(Com_IsPartitionsTxStructUsedOfMainFunctionRouteSignalsStruct(mainFunctionId))
  {
    /* Iterate over cross partition queues leading to this partition */
    for(Com_CprQueueSignalRoutingIndIterType routingQueueIndIndex = Com_GetCprQueueSignalRoutingIndStartIdxOfPartitionsTxStruct(Com_GetPartitionsTxStructIdxOfMainFunctionRouteSignalsStruct(mainFunctionId));
        routingQueueIndIndex < Com_GetCprQueueSignalRoutingIndEndIdxOfPartitionsTxStruct(Com_GetPartitionsTxStructIdxOfMainFunctionRouteSignalsStruct(mainFunctionId)); routingQueueIndIndex++) /* FETA_COM_01 */
    {
      Com_CprQueueBufferReadIdxOfCprQueueType readIdx = 0u;
      const Com_CCprQueueIterType routingQueueIndex = Com_GetCprQueueSignalRoutingInd(routingQueueIndIndex);
      const Com_CprQueueBufferLengthOfCprQueueType readThreshold = Com_GetCprQueueBufferLengthOfCprQueue(routingQueueIndex);

      /* Reset NumberOfBytesRead */
      Com_CprQueueSigGw_ResetNumberOfBytesRead(routingQueueIndex);

      /* While multi partition routing queue has stored elements, read data from queue */
      while(Com_CprQueueSigGw_GetReadIdx(routingQueueIndex, &readIdx) == E_OK)  /* FETA_COM_02 */
      {
        switch (Com_CprQueueSigGw_ReadFromCprQueueDirectly(readIdx))
        {
          /* Process signal routing */
          case COM_CPRQUEUE_HEADERVALUE_SIGNAL:
          {
            Com_SigGwCP_SignalRoutingTx(routingQueueIndex, readIdx);
            /* Update the read index */
            Com_CprQueueSigGw_UpdateReadIdx(routingQueueIndex);
            break;
          }
          /* Process signal group routing */
          case COM_CPRQUEUE_HEADERVALUE_SIGNALGROUP:
          {
            Com_SigGwCP_SignalGroupRoutingTx(routingQueueIndex, readIdx);
            /* Update the read index */
            Com_CprQueueSigGw_UpdateReadIdx(routingQueueIndex);
            break;
          }
          default:
          /* Break from while loop in case there is no valid header */
            Com_AddCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, readThreshold);
            break;
        }

        /* Abort processing if number of read bytes increases the the queue size */
        if(Com_GetCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex) > readThreshold)
        {
          break;
        }

        Com_ISRThreshold_ThresholdCheck(gwProcessingISRlockCounterIdx);
      }
    }
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
 *  END OF FILE: Com_SigGwCP.c
 *********************************************************************************************************************/
