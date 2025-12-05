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
 *         \file  IpduM_ContainerTxDataQueueUtil.c
 *         \unit  ContainerTxDataQueueUtil
 *        \brief  Microsar IpduM Container Tx Data Queue Util source file
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

#define IPDUM_CONTAINERTTXDATAQUEUEUTIL_SOURCE
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
#include "IpduM_ContainerTxDataQueueUtil.h"
#include "IpduM_ContainerTxDataQueue.h"
#include "IpduM_ContainerTxDataQueueInstance.h"
#include "IpduM_ContainerUtil.h"
#include "IpduM_TxConfirmation.h"
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
 *  IpduM_ContainerTx_DataQueueUtil_TransmitContainerPdu_DirectDP()
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
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_DataQueueUtil_TransmitContainerPdu_DirectDP(IpduM_CTxContainerPduIterType txContainerId)
{
  Std_ReturnType transmitResult = E_NOT_OK;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(IpduM_TxConfirmation_IsTransmissionAllowed(IpduM_GetTxLoInfoIdxOfTxContainerPdu(txContainerId)))
  {
    IpduM_DataQueueInstanceReadIdxOfDataQueueInfoType dataQueueInstanceReadIdx = IpduM_GetDataQueueInstanceReadIdxOfDataQueueInfo(IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId));
    /*@ assert (dataQueueInstanceReadIdx  < IpduM_GetSizeOfCDataQueueInstance()); */  /* VCA_IPDUM_RING_BUFFER */

    PduInfoType pduInfo;
    pduInfo = IpduM_ContainerTx_DataQueueInstance_GetAsPointer(dataQueueInstanceReadIdx);
    IpduM_ContainerUtil_DecreaseDlcByMetaDataSize(txContainerId, &pduInfo);

    IpduM_ContainerTx_DataQueueInstance_CopyContainedPduIDs2TxConfBuffer(dataQueueInstanceReadIdx, txContainerId);
      
    IpduM_TxConfirmation_StartTimeout(IpduM_GetTxLoInfoIdxOfTxContainerPdu(txContainerId));

    transmitResult = PduR_IpduMTransmit(IpduM_GetTxContainerPduRefOfTxContainerPdu(txContainerId), &pduInfo);

    if(transmitResult == E_OK)
    {
      /* Reset the read queue instance and remove it by incrementing the readIdx of the queue. */
      IpduM_ContainerTx_DataQueueInstance_Init(dataQueueInstanceReadIdx, txContainerId);
      IpduM_ContainerTx_DataQueue_IncrementReadPtr(txContainerId);
    }
    else
    {
      IpduM_ContainerUtil_IncreaseDlcByMetaDataSize(txContainerId, &pduInfo);
      IpduM_TxConfirmation_StopTimeout(IpduM_GetTxLoInfoIdxOfTxContainerPdu(txContainerId));
    }

  }

  return transmitResult;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueUtil_TriggerTransmissionOfCurrContainerPdu()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueUtil_TriggerTransmissionOfCurrContainerPdu(IpduM_CTxContainerPduIterType txContainerId, boolean queueCurrentContainer)
{
  boolean wasQueueEmpty = IpduM_ContainerTx_DataQueue_IsFifoQueueEmpty(txContainerId);

  /* Depending on the call context, the current container instance must be closed and queued:
   *   - the current container instance is full (both direct and triggered transmission)
   *   - direct transmission: the current container instance is always closed before transmission.
   * For trigger transmit: the current container instance is left open until it is fetched in the TriggerTransmit API. */
  if(queueCurrentContainer)
  {
    IpduM_ContainerTx_DataQueue_QueueCurrContainerPdu(txContainerId);
  }

  if(wasQueueEmpty) /* Only transmit to the lower layer if there are no other pending transmits. */
  {
    if(IpduM_IsTriggerTransmitOfTxContainerPdu(txContainerId))
    {
      if(IpduM_ContainerUtil_TransmitEmptyPdu(txContainerId) != E_OK)
      {
        /* Close and queue the current container instance. This way the transmission can be retried in the next Mainfunction. */
        IpduM_ContainerTx_DataQueue_QueueCurrContainerPdu(txContainerId);
      }
    }
    else
    {
      (void) IpduM_ContainerTx_DataQueueUtil_TransmitContainerPdu_DirectDP(txContainerId);
    }
  }
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerTxDataQueueUtil.c
**********************************************************************************************************************/
