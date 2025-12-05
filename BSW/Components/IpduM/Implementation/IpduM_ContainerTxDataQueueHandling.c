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
 *         \file  IpduM_ContainerTxDataQueueHandling.c
 *         \unit  ContainerTxDataQueueHandling
 *        \brief  Microsar IpduM Container Tx Data Queue Handling source file
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

#define IPDUM_CONTAINERTTXDATAQUEUEHANDLING_SOURCE
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
#include "IpduM_TxConfirmation.h"
#include "IpduM_ContainerTxTrigger.h"
#include "IpduM_ContainerTxDataQueueHandling.h"
#include "IpduM_ContainerTxDataQueueUtil.h"
#include "IpduM_ContainerTxDataQueue.h"
#include "IpduM_ContainerTxDataQueueInstance.h"
#include "IpduM_ContainerTxSendTimeout.h"
#include "IpduM_ContainerUtil.h"
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
 *  IpduM_ContainerTx_DataQueueHandling_Transmit()
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
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueHandling_Transmit(IpduM_TxUpIndIdxOfTxUpInfoType txContainedId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  IpduM_CTxContainerPduIterType txContainerId = IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId);

  if(IpduM_GetHeaderSizeOfTxContainerPdu(txContainerId) == IPDUM_HEADERSIZE_NONE)
  {
    /* Pdu always fits in container with static layout. The Pdu will be overwritten at its configured position. */
    IpduM_ContainerTx_DataQueue_AddPdu_StaticLayout(txContainedId, PduInfoPtr);
  }
  else
  {
    if(!IpduM_ContainerTx_DataQueue_FitsPduIntoCurrentOpenInstance(txContainerId, PduInfoPtr->SduLength))
    {
      /* Handover queueCurrentContainer == TRUE. The current container must always be queued and closed, as it is full. */
      IpduM_ContainerTx_DataQueueUtil_TriggerTransmissionOfCurrContainerPdu(txContainerId, TRUE);

      IpduM_ContainerTx_SendTimeout_Stop(txContainerId);
    }
    /* PDU now does fit into current container instance: add PDU. */
    IpduM_ContainerTx_DataQueue_AddPdu_DynamicLayout(txContainedId, PduInfoPtr);
  }

  IpduM_ContainerTx_SendTimeout_Start(txContainedId);

  IpduM_DataQueueInstanceWriteIdxOfDataQueueInfoType dataQueueInstanceWriteIdx = IpduM_GetDataQueueInstanceWriteIdxOfDataQueueInfo(IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId));
  /*@ assert (dataQueueInstanceWriteIdx  < IpduM_GetSizeOfCDataQueueInstance()); */  /* VCA_IPDUM_RING_BUFFER */

  /* Check trigger conditions and trigger the PDU if conditions are met. */
  if(IpduM_ContainerTxTrigger_IsTriggered(txContainedId, IpduM_ContainerTx_DataQueueInstance_GetFillCount(dataQueueInstanceWriteIdx)))
  {
    IpduM_ContainerTx_DataQueueUtil_TriggerTransmissionOfCurrContainerPdu(txContainerId, IpduM_IsDirectDataProvisionOfTxContainerPdu(txContainerId));

    IpduM_ContainerTx_SendTimeout_Stop(txContainerId);
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueHandling_TriggerTransmit()
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
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_DataQueueHandling_TriggerTransmit(IpduM_TxContainerPduIdxOfCTxLoInfoType txContainerId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  boolean pleaseCopyTheData = FALSE;

  SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  if(IpduM_ContainerTx_DataQueue_IsFifoQueueEmpty(txContainerId))
  {
    IpduM_DataQueueInstanceWriteIdxOfDataQueueInfoType dataQueueInstanceWriteIdx = IpduM_GetDataQueueInstanceWriteIdxOfDataQueueInfo(IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId));
    /*@ assert (dataQueueInstanceWriteIdx  < IpduM_GetSizeOfCDataQueueInstance()); */  /* VCA_IPDUM_RING_BUFFER */

    if(IpduM_ContainerTx_DataQueueInstance_GetFillCount(dataQueueInstanceWriteIdx) > 0u)
    {/* Queue is empty, but the current open instance has data. Put it in the queue and read it below. */
      IpduM_ContainerTx_DataQueue_QueueCurrContainerPdu(txContainerId);
      IpduM_ContainerTx_SendTimeout_Stop(txContainerId);
      pleaseCopyTheData = TRUE;
    }
  }
  else
  {
    pleaseCopyTheData = TRUE;
  }

  if(pleaseCopyTheData == TRUE)
  {
    IpduM_DataQueueInstanceReadIdxOfDataQueueInfoType dataQueueInstanceReadIdx = IpduM_GetDataQueueInstanceReadIdxOfDataQueueInfo(IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId));
    /*@ assert (dataQueueInstanceReadIdx  < IpduM_GetSizeOfCDataQueueInstance()); */  /* VCA_IPDUM_RING_BUFFER */

    IpduM_ContainerTx_DataQueueInstance_Get(dataQueueInstanceReadIdx, PduInfoPtr);
    IpduM_ContainerTx_DataQueueInstance_CopyContainedPduIDs2TxConfBuffer(dataQueueInstanceReadIdx, txContainerId);

    /* Reset the read queue instance and remove it by incrementing the readIdx of the queue. */
    IpduM_ContainerTx_DataQueueInstance_Init(dataQueueInstanceReadIdx, txContainerId);
    IpduM_ContainerTx_DataQueue_IncrementReadPtr(txContainerId);

    IpduM_TxConfirmation_StartTimeout(IpduM_GetTxLoInfoIdxOfTxContainerPdu(txContainerId));
    retVal = E_OK;
  }
  SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueHandling_MainFunction()
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
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueHandling_MainFunction(IpduM_CTxContainerPduIterType txContainerId)
{
  if(IpduM_IsTriggerTransmitOfTxContainerPdu(txContainerId)) /* PRQA S 2842 */ /* MD_IpduM_2842 */
  {
    if(!IpduM_ContainerTx_DataQueue_IsFifoQueueEmpty(txContainerId))
    {
      /* Informative Transmit for TriggerTx container. This will be executed even if a previous TxConfirmation is expected. */
      (void) IpduM_ContainerUtil_TransmitEmptyPdu(txContainerId);
    }
  }
  else
  {
    Std_ReturnType transmitResult = E_OK;
    IpduM_DataQueueInstanceLengthOfDataQueueInfoType iterationLimiter = IpduM_GetDataQueueInstanceLengthOfDataQueueInfo(IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId));

    while(!IpduM_ContainerTx_DataQueue_IsFifoQueueEmpty(txContainerId) && (transmitResult == E_OK)) /* FETA_IPDUM_01 */
    {
      /* returns E_NOT_OK for transmission failure */
      transmitResult = IpduM_ContainerTx_DataQueueUtil_TransmitContainerPdu_DirectDP(txContainerId);
        
      /* ensure the loop always terminates in a deterministic time */
      if(iterationLimiter == 0u)
      {
        break;
      }
      iterationLimiter--;
    }
  }

  if(IpduM_ContainerTx_SendTimeout_IsRunning(txContainerId))
  {
    boolean isSendTimeoutExpired = IpduM_ContainerTx_SendTimeout_MainFunction(txContainerId);

    if(isSendTimeoutExpired)
    {
      IpduM_ContainerTx_DataQueueUtil_TriggerTransmissionOfCurrContainerPdu(txContainerId, IpduM_IsDirectDataProvisionOfTxContainerPdu(txContainerId));
    }
  }
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerTxDataQueueHandling.c
**********************************************************************************************************************/
