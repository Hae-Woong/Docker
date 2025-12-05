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
 *         \file  IpduM_ContainerTxRequestQueueUtil.c
 *         \unit  ContainerTxRequestQueueUtil
 *        \brief  Microsar IpduM ContainerTxRequestQueueUtil source file
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

#define IPDUM_CONTAINERTXREQUESTQUEUEUTIL_SOURCE
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
#include "IpduM_ContainerTxRequestQueueUtil.h"
#include "IpduM_TxConfirmation.h"
#include "IpduM_ContainerTxRequestQueue.h"
#include "IpduM_ContainerTxRequestQueuePriorityHandling.h"
#include "IpduM_ContainerUtil.h"
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
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueueUtil_TransmitContainer_DirectDP()
*******************************************************************************************/
/*! \brief        Transmits request queue Container PDU with Direct DataProvision.
 *  \details      -
 *  \param[in]    txContainerId
 *  \return       E_OK            Transmission was successful, current instance is marked empty.
 *  \return       E_NOT_OK        Transmission was unsuccessful, current instance is untouched.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs.
 *  \synchronous  TRUE
*******************************************************************************************/
IPDUM_LOCAL_INLINE FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_RequestQueueUtil_TransmitContainer_DirectDP(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueueUtil_TransmitContainer_TriggerTxDP()
*******************************************************************************************/
/*! \brief        Transmits request queue Container PDU with TriggerTransmit DataProvision.
 *  \details      -
 *  \param[in]    txContainerId
 *  \return       E_OK            Transmission was successful.
 *  \return       E_NOT_OK        Transmission was unsuccessful.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different container IDs.
 *  \synchronous  TRUE
*******************************************************************************************/
IPDUM_LOCAL_INLINE FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_RequestQueueUtil_TransmitContainer_TriggerTxDP(IpduM_CTxContainerPduIterType txContainerId);


#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueueUtil_TransmitContainer_DirectDP()
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
IPDUM_LOCAL_INLINE FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_RequestQueueUtil_TransmitContainer_DirectDP(IpduM_CTxContainerPduIterType txContainerId)
{
  Std_ReturnType transmitResult = E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(IpduM_TxConfirmation_IsTransmissionAllowed(IpduM_GetTxLoInfoIdxOfTxContainerPdu(txContainerId)))
  {
    PduInfoType pduInfo;
    pduInfo.SduDataPtr = IpduM_GetAddrContainerTxLIBBuffer(IpduM_GetContainerTxLIBBufferStartIdxOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId)));
    pduInfo.SduLength = IpduM_GetContainerTxLIBBufferLengthOfRequestQueuePrioIndirection(IpduM_GetRequestQueuePrioIndirectionIdxOfTxContainerPdu(txContainerId));

    IpduM_ContainerTx_RequestQueue_PriorityHandling_SetRestorePoint(txContainerId);
    /* last-is-best PDU: load data to last-is-best buffer via TriggerTransmit, then use this buffer for transmission */
    IpduM_ContainerTx_RequestQueue_PriorityHandling_FillLastIsBestContainer(txContainerId, &pduInfo);
    IpduM_ContainerUtil_DecreaseDlcByMetaDataSize(txContainerId, &pduInfo);

    IpduM_TxConfirmation_StartTimeout(IpduM_GetTxLoInfoIdxOfTxContainerPdu(txContainerId));

    transmitResult = PduR_IpduMTransmit(IpduM_GetTxContainerPduRefOfTxContainerPdu(txContainerId), &pduInfo);

    if(transmitResult != E_OK)
    {
      IpduM_ContainerUtil_IncreaseDlcByMetaDataSize(txContainerId, &pduInfo);
      IpduM_ContainerTx_RequestQueue_PriorityHandling_RestoreReadIDs(txContainerId);

      IpduM_TxConfirmation_StopTimeout(IpduM_GetTxLoInfoIdxOfTxContainerPdu(txContainerId));
    }
  }

  return transmitResult;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueueUtil_TransmitContainer_TriggerTxDP()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
IPDUM_LOCAL_INLINE FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_RequestQueueUtil_TransmitContainer_TriggerTxDP(IpduM_CTxContainerPduIterType txContainerId)
{
  return IpduM_ContainerUtil_TransmitEmptyPdu(txContainerId);
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
 *  IpduM_ContainerTx_RequestQueueUtil_TriggerTransmission()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_RequestQueueUtil_TriggerTransmission(IpduM_CTxContainerPduIterType txContainerId)
{
  Std_ReturnType retVal = E_NOT_OK;

  if(IpduM_IsTriggerTransmitOfTxContainerPdu(txContainerId))
  {
    retVal = IpduM_ContainerTx_RequestQueueUtil_TransmitContainer_TriggerTxDP(txContainerId);
  }
  else
  {
    retVal = IpduM_ContainerTx_RequestQueueUtil_TransmitContainer_DirectDP(txContainerId);
  }

  return retVal;
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerTxRequestQueueUtil.c
**********************************************************************************************************************/
