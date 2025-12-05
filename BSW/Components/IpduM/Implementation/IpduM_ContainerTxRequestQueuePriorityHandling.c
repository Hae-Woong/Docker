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
 *         \file  IpduM_ContainerTxRequestQueuePriorityHandling.c
 *         \unit  ContainerTxRequestQueuePriorityHandling
 *        \brief  Microsar IpduM Container Tx Request Queue Priority Handling source file
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

#define IPDUM_CONTAINERTXREQUESTQUEUEPRIORITYHANDLING_SOURCE
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
#include "IpduM_ContainerTxRequestQueuePriorityHandling.h"
#include "IpduM_ContainerTxRequestQueue.h"
#include "IpduM_ContainerUtil.h"
#include "IpduM_TxConfBuffer.h"
#include "PduR_IpduM.h"

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
 *  IpduM_ContainerTx_RequestQueue_PriorityHandling_IsEmpty()
*******************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_PriorityHandling_IsEmpty(IpduM_CTxContainerPduIterType txContainerId)
{
  boolean isEmpty = TRUE;
  IpduM_RequestQueueInfoStartIdxOfRequestQueuePrioIndirectionType requestQueueInfoIdx = IpduM_GetRequestQueueInfoStartIdxOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId));
  IpduM_RequestQueueInfoEndIdxOfRequestQueuePrioIndirectionType requestQueueInfoEndIdx = IpduM_GetRequestQueueInfoEndIdxOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId));
  
  /* loop over all priorities for this container */
  for(; requestQueueInfoIdx < requestQueueInfoEndIdx; requestQueueInfoIdx++) /* FETA_IPDUM_01 */
  {
    if(!IpduM_ContainerTx_RequestQueue_IsEmpty(requestQueueInfoIdx))
    {
      isEmpty = FALSE;
      break;
    }
  }
  return isEmpty;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_PriorityHandling_CalculateSize()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduLengthType, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_PriorityHandling_CalculateSize(IpduM_CTxContainerPduIterType txContainerId)
{
  PduLengthType sizeOfData = 0u;
  IpduM_RequestQueueInfoStartIdxOfRequestQueuePrioIndirectionType requestQueueInfoIdx = IpduM_GetRequestQueueInfoStartIdxOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId));
  IpduM_RequestQueueInfoEndIdxOfRequestQueuePrioIndirectionType txRequestQueueInfoEndIdx = IpduM_GetRequestQueueInfoEndIdxOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId));
  PduLengthType headerSize = (PduLengthType) IpduM_GetHeaderSizeOfTxContainerPdu(txContainerId);
  
  for(; requestQueueInfoIdx < txRequestQueueInfoEndIdx; requestQueueInfoIdx++) /* FETA_IPDUM_01 */
  {
    sizeOfData += IpduM_ContainerTx_RequestQueue_CalculateSize(requestQueueInfoIdx, headerSize);
  }
  return sizeOfData;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_PriorityHandling_RestoreReadIDs
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_PriorityHandling_RestoreReadIDs(IpduM_CTxContainerPduIterType txContainerId)
{
  IpduM_RequestQueueInfoStartIdxOfRequestQueuePrioIndirectionType requestQueueInfoIdx = IpduM_GetRequestQueueInfoStartIdxOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId));
  IpduM_RequestQueueInfoEndIdxOfRequestQueuePrioIndirectionType txRequestQueueInfoEndIdx = IpduM_GetRequestQueueInfoEndIdxOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId));

  for(; requestQueueInfoIdx < txRequestQueueInfoEndIdx; requestQueueInfoIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_ContainerTx_RequestQueue_RestoreReadIDs(requestQueueInfoIdx);
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_PriorityHandling_SetRestorePoint
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_PriorityHandling_SetRestorePoint(IpduM_CTxContainerPduIterType txContainerId)
{
  IpduM_RequestQueueInfoStartIdxOfRequestQueuePrioIndirectionType requestQueueInfoIdx = IpduM_GetRequestQueueInfoStartIdxOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId));
  IpduM_RequestQueueInfoEndIdxOfRequestQueuePrioIndirectionType txRequestQueueInfoEndIdx = IpduM_GetRequestQueueInfoEndIdxOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId));

  for(; requestQueueInfoIdx < txRequestQueueInfoEndIdx; requestQueueInfoIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_ContainerTx_RequestQueue_SetRestorePoint(requestQueueInfoIdx);
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueue_PriorityHandling_FillLastIsBestContainer()
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
 *
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueue_PriorityHandling_FillLastIsBestContainer(IpduM_CTxContainerPduIterType txContainerId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) bufferInfo)
{
  IpduM_RequestQueuePrioIndirectionIdxOfTxContainerPduType requestQueuePrioIndirectionIdx = IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId);
  IpduM_RequestQueueInfoStartIdxOfRequestQueuePrioIndirectionType requestQueueInfoIdx = IpduM_GetRequestQueueInfoStartIdxOfRequestQueuePrioIndirection(requestQueuePrioIndirectionIdx);
  IpduM_RequestQueueInfoEndIdxOfRequestQueuePrioIndirectionType txRequestQueueInfoEndIdx = IpduM_GetRequestQueueInfoEndIdxOfRequestQueuePrioIndirection(requestQueuePrioIndirectionIdx);
  PduLengthType headerSize = (PduLengthType) IpduM_GetHeaderSizeOfTxContainerPdu(txContainerId);

  PduLengthType writtenBytes = 0u;
  PduLengthType freeBytes = bufferInfo->SduLength;

  for(; requestQueueInfoIdx < txRequestQueueInfoEndIdx; requestQueueInfoIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_TxContainedPduIdxOfRequestQueueBufferType containedPduId = 0u;
    PduLengthType containedPduLength = 0u;
    IpduM_RequestQueueBufferLengthOfRequestQueueInfoType iterationLimiter = IpduM_GetRequestQueueBufferLengthOfRequestQueueInfo(requestQueueInfoIdx);

    while(IpduM_ContainerTx_RequestQueue_Peek(requestQueueInfoIdx, &containedPduId, &containedPduLength) == E_OK)  /* FETA_IPDUM_01 */ /* PRQA S 0771 */ /* MD_IpduM_0771 */
    {
      if(freeBytes >= (containedPduLength + headerSize))
      {
        PduInfoType pduInfo;
        pduInfo.SduDataPtr = &bufferInfo->SduDataPtr[writtenBytes + headerSize];  /* data starts after the header */
        pduInfo.SduLength = freeBytes - headerSize; /* available size left in buffer */

        if(PduR_IpduMTriggerTransmit(IpduM_GetTxContainedPduRefOfTxContainedPdu(containedPduId), &pduInfo) == E_OK)
        {
          PduLengthType actualCopiedDataLength = pduInfo.SduLength;

          IpduM_ContainerUtil_WriteHeader(&bufferInfo->SduDataPtr[writtenBytes], IpduM_GetHeaderIdOfTxContainedPdu(containedPduId), pduInfo.SduLength, headerSize);
          (void) IpduM_TxConfBuffer_Put(txContainerId, containedPduId);

          writtenBytes += (actualCopiedDataLength + headerSize);
          freeBytes -= (actualCopiedDataLength + headerSize);
        }

        IpduM_ContainerTx_RequestQueue_Remove(requestQueueInfoIdx);
      }
      else
      {
        break;
      }
        
      /* ensure the loop always terminates in a deterministic time */
      if(iterationLimiter == 0u)
      {
        break;
      }
      iterationLimiter--;
    }
  }

  /* Return the actual written bytes */
  bufferInfo->SduLength = writtenBytes;
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerTxRequestQueuePriorityHandling.c
**********************************************************************************************************************/
