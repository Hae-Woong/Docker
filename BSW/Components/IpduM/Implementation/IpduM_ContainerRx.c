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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  IpduM_ContainerRx.c
 *         \unit  ContainerRx
 *        \brief  Microsar IpduM Container Rx source file
 *      \details  This is the implementation of the MICROSAR IpduM module.
 *                The basic software module is based on the AUTOSAR IpduM specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define IPDUM_CONTAINERRX_SOURCE
/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 883, 3406 EOF */ /* MD_IpduM_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "IpduM.h"
#include "IpduM_Reporting.h"
#include "IpduM_ContainerRx.h"
#include "IpduM_ContainerRxProcessing.h"
#include "IpduM_ContainerRxQueue.h"
#include "SchM_IpduM.h"

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
  LOCAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_ContainerRx_RxIndication()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerRx_RxIndication(IpduM_RxPathwayIndIdxOfRxInfoType rxContainerId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  if(IpduM_IsContainerRxQueueInfoUsedOfRxContainerPdu(rxContainerId))
  {/* Deferred processing. Enqueue PDU for later reception handling. */
    IpduM_ContainerRxQueueInfoIdxOfRxContainerPduType containerRxQueueInfoIdx = IpduM_GetContainerRxQueueInfoIdxOfRxContainerPdu(rxContainerId);
    PduLengthType pduLength = PduInfoPtr->SduLength;
    PduLengthType bufferLength = IpduM_GetContainerRxQueueBufferLengthOfContainerRxQueueInstance(IpduM_GetContainerRxQueueInstanceWriteIdxOfContainerRxQueueInfo(IpduM_GetContainerRxQueueInfoIdxOfRxContainerPdu(rxContainerId)));

    /* API Pattern Check against configured Queue Size. */
    /* Handle larger than configured PDUs by trimming to configured length */
    if(pduLength > bufferLength)
    {
      pduLength = bufferLength;
      IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_CONTAINERRX_RXINDICATION, IPDUM_E_HEADER);
    }

    SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
    /* If the PDU does not fit into container: DET, oldest entry will be overwritten while enqueueing */
    if(IpduM_ContainerRxQueue_IsFull(containerRxQueueInfoIdx))
    {
      /* Queue is full, return an error */
      IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_CONTAINERRX_RXINDICATION, IPDUM_E_QUEUEOVFL);
      (void) IpduM_ContainerRxQueue_Remove(containerRxQueueInfoIdx);
    }

    (void) IpduM_ContainerRxQueue_Put(containerRxQueueInfoIdx, PduInfoPtr->SduDataPtr, pduLength);

    SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  }
  else
  {
    /* Immediate processing. Send RxIndications for all received contained-PDUs. */
    IpduM_ContainerRxProcessing_UnpackContainerPdu(rxContainerId, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);
  }
}

/*******************************************************************************************
 *  IpduM_ContainerRx_MainFunction()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerRx_MainFunction(IpduM_MainFunctionRxIndirectionIterType mainFunctionRxIndirectionIdx)
{
  IpduM_RxContainerPduIndStartIdxOfMainFunctionRxIndirectionType rxContainerPduIndIdx = IpduM_GetRxContainerPduIndStartIdxOfMainFunctionRxIndirection(mainFunctionRxIndirectionIdx);
  IpduM_RxContainerPduIndEndIdxOfMainFunctionRxIndirectionType   rxContainerPduIndEndIdx = IpduM_GetRxContainerPduIndEndIdxOfMainFunctionRxIndirection(mainFunctionRxIndirectionIdx);

  for(; rxContainerPduIndIdx < rxContainerPduIndEndIdx; rxContainerPduIndIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_RxContainerPduIterType rxContainerId = IpduM_GetRxContainerPduInd(rxContainerPduIndIdx);

    if(IpduM_IsContainerRxQueueInfoUsedOfRxContainerPdu(rxContainerId))
    {
      IpduM_ContainerRxQueueInfoIdxOfRxContainerPduType containerRxQueueInfoIdx = IpduM_GetContainerRxQueueInfoIdxOfRxContainerPdu(rxContainerId);

      IpduM_ContainerRxQueueInstanceLengthOfContainerRxQueueInfoType queueDepth = IpduM_ContainerRxQueue_GetQueueDepth(containerRxQueueInfoIdx);
      IpduM_ContainerRxQueueInstanceLengthOfContainerRxQueueInfoType readCounter = 0u;

      for(; readCounter < queueDepth; readCounter++) /* FETA_IPDUM_01 */
      {
        IpduM_RxContainerMainFunctionBufferPtrType rxMfBuffer = IpduM_GetAddrRxContainerMainFunctionBuffer(IpduM_GetRxContainerMainFunctionBufferStartIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx));
        PduLengthType pduLength = (PduLengthType) IpduM_GetRxContainerMainFunctionBufferLengthOfContainerRxQueueInfo(containerRxQueueInfoIdx);

        SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
        Std_ReturnType rxQueueRetVal = IpduM_ContainerRxQueue_Get(containerRxQueueInfoIdx, rxMfBuffer, &pduLength);
        SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();

        if(rxQueueRetVal == E_OK)
        {
          IpduM_ContainerRxProcessing_UnpackContainerPdu(rxContainerId, rxMfBuffer, pduLength);
        }
        else
        {/* No more pdus in the queue */
          break;
        }
      }
    }
  }
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerRx.c
**********************************************************************************************************************/
