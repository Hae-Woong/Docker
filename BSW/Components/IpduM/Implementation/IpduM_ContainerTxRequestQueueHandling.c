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
 *         \file  IpduM_ContainerTxRequestQueueHandling.c
 *         \unit  ContainerTxRequestQueueHandling
 *        \brief  Microsar IpduM Container Tx Request Queue Handling source file
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

#define IPDUM_CONTAINERTXREQUESTQUEUEHANDLING_SOURCE
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
#include "IpduM_ContainerTxRequestQueueHandling.h"
#include "IpduM_ContainerTxRequestQueuePriorityHandling.h"
#include "IpduM_ContainerTxRequestQueueUtil.h"
#include "IpduM_ContainerTxRequestQueue.h"
#include "IpduM_ContainerTxSendTimeout.h"
#include "IpduM_ContainerTxTrigger.h"
#include "SchM_IpduM.h"
#include "vstdlib.h"

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
 *  IpduM_ContainerTx_RequestQueueHandling_Transmit()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueueHandling_Transmit(IpduM_TxUpIndIdxOfTxUpInfoType txContainedId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  IpduM_CTxContainerPduIterType txContainerId = IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId);

  IpduM_ContainerTxTrigger_SetFirstContainedPduFlag(txContainerId, IpduM_ContainerTx_RequestQueue_PriorityHandling_IsEmpty(txContainerId));

  /* PDUs that use the request queue get only their ID added */
  IpduM_ContainerTx_RequestQueue_Put(IpduM_GetRequestQueueInfoIdxOfTxContainedPdu(txContainedId), txContainedId, PduInfoPtr->SduLength);

  IpduM_ContainerTx_SendTimeout_Start(txContainedId);

  /* Check trigger conditions and trigger the PDU if conditions are met. */
  if(IpduM_ContainerTxTrigger_IsTriggered(txContainedId, IpduM_ContainerTx_RequestQueue_PriorityHandling_CalculateSize(txContainerId)))
  {
    (void) IpduM_ContainerTx_RequestQueueUtil_TriggerTransmission(txContainerId);

    IpduM_ContainerTx_SendTimeout_Stop(txContainerId);
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueueHandling_TriggerTransmit()
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
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_RequestQueueHandling_TriggerTransmit(IpduM_TxContainerPduIdxOfCTxLoInfoType txContainerId, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  Std_ReturnType retVal = E_NOT_OK;

  SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  if(!IpduM_ContainerTx_RequestQueue_PriorityHandling_IsEmpty(txContainerId))
  {
    PduInfoType pduInfo;
    pduInfo.SduDataPtr = IpduM_GetAddrContainerTxLIBBuffer(IpduM_GetContainerTxLIBBufferStartIdxOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId)));
    pduInfo.SduLength = IpduM_GetContainerTxLIBBufferLengthOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId));

    /* current instance is closed -> reset its send timeout */
    IpduM_ContainerTx_SendTimeout_Stop(txContainerId);

    /* last-is-best PDU: load data to last-is-best buffer via TriggerTransmit, then use this buffer for transmission */
    IpduM_ContainerTx_RequestQueue_PriorityHandling_FillLastIsBestContainer(txContainerId, &pduInfo);

    /* Copy PDU from container buffer to provided destination buffer */
    VStdLib_MemCpy_s(PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength, pduInfo.SduDataPtr, pduInfo.SduLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_IPDUM_MEMCPY_5 */

    PduInfoPtr->SduLength = pduInfo.SduLength;

    IpduM_TxConfirmation_StopTimeout(IpduM_GetTxLoInfoIdxOfTxContainerPdu(txContainerId));
    retVal = E_OK;
  }
  SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueueHandling_MainFunction()
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
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_RequestQueueHandling_MainFunction(IpduM_CTxContainerPduIterType txContainerId)
{
  if(IpduM_ContainerTx_SendTimeout_IsRunning(txContainerId))
  {
    boolean isSendTimeoutExpired = IpduM_ContainerTx_SendTimeout_MainFunction(txContainerId);

    if(isSendTimeoutExpired)
    {
      (void) IpduM_ContainerTx_RequestQueueUtil_TriggerTransmission(txContainerId);
    }
  }
  else
  {
    if(IpduM_IsTriggerTransmitOfTxContainerPdu(txContainerId)) /* PRQA S 2842 */ /* MD_IpduM_2842 */
    {
      if(!IpduM_ContainerTx_RequestQueue_PriorityHandling_IsEmpty(txContainerId))
      {
        /* For TriggerTransmit Pdus only the oldest Pdu is transmitted. */
        (void) IpduM_ContainerTx_RequestQueueUtil_TriggerTransmission(txContainerId);
      }
    }
    else
    {
      Std_ReturnType transmitResult = E_OK;
      IpduM_TxContainedPduLengthOfTxContainerPduType iterationLimiter = IpduM_GetTxContainedPduLengthOfTxContainerPdu(txContainerId);

      while(!IpduM_ContainerTx_RequestQueue_PriorityHandling_IsEmpty(txContainerId) && (transmitResult == E_OK)) /* FETA_IPDUM_01 */
      {
        /* returns E_NOT_OK for empty queue and transmission failure */
        transmitResult = IpduM_ContainerTx_RequestQueueUtil_TriggerTransmission(txContainerId);
        
        /* ensure the loop always terminates in a deterministic time */
        iterationLimiter--;
        if(iterationLimiter == 0u)
        {
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
  END OF FILE: IpduM_ContainerTxRequestQueueHandling.c
**********************************************************************************************************************/
