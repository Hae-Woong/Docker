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
 *         \file  IpduM_ContainerTxSendTimeout.c
 *         \unit  ContainerTxSendTimeout
 *        \brief  Microsar IpduM Container Tx Send Timeout source file
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

#define IPDUM_CONTAINERTXSENDTIMEOUT_SOURCE
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
#include "IpduM_ContainerTxSendTimeout.h"
#include "IpduM_ContainerTxTrigger.h"

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
*  IpduM_ContainerTx_SendTimeout_SetContainerPduTimeout
*******************************************************************************************/
/*! \brief        Start container Pdu send timeout after adding the first PDUs to it
 *  \details      -
 *  \param[in]    txContainerId
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \trace        SPEC-2068537, SPEC-6231909, SPEC-2068539, SPEC-2068543, SPEC-2068545, SPEC-2068549, SPEC-2068551, SPEC-2068552
*******************************************************************************************/
IPDUM_LOCAL_INLINE FUNC(void, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_SetContainerPduTimeout(IpduM_CTxContainerPduIterType txContainerId);

/*******************************************************************************************
*  IpduM_ContainerTx_SendTimeout_SetContainedPduTimeout
*******************************************************************************************/
/*! \brief        Start/Update contained Pdu send timeout after adding new PDUs to it
 *  \details      -
 *  \param[in]    txContainedId
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    TRUE for different handle IDs
 *  \synchronous  TRUE
 *  \trace        SPEC-2068537, SPEC-6231909, SPEC-2068539, SPEC-2068543, SPEC-2068545, SPEC-2068549, SPEC-2068551, SPEC-2068552
*******************************************************************************************/
IPDUM_LOCAL_INLINE FUNC(void, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_SetContainedPduTimeout(IpduM_TxContainedPduIterType txContainedId);

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
*  IpduM_ContainerTx_SendTimeout_SetContainerPduTimeout
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
IPDUM_LOCAL_INLINE FUNC(void, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_SetContainerPduTimeout(IpduM_CTxContainerPduIterType txContainerId)
{
  if(IpduM_IsSendToDefOfTxContainerPdu(txContainerId))
  {
    /* Start timeout */
    IpduM_SetContainerPduSendTimeoutOfTxContainerPdu(txContainerId, IpduM_GetSendTimeoutOfTxContainerPdu(txContainerId));
  }
}

/*******************************************************************************************
*  IpduM_ContainerTx_SendTimeout_SetContainedPduTimeout
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
IPDUM_LOCAL_INLINE FUNC(void, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_SetContainedPduTimeout(IpduM_TxContainedPduIterType txContainedId)
{
  IpduM_CTxContainerPduIterType txContainerId = IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId);

  if(IpduM_IsSendToDefOfTxContainedPdu(txContainedId))
  {
    if((IpduM_GetContainerPduSendTimeoutOfTxContainerPdu(txContainerId) > IpduM_GetSendTimeoutOfTxContainedPdu(txContainedId)) /* Update current running timer if contained Pdu timeout is smaller. */
        || (IpduM_GetContainerPduSendTimeoutOfTxContainerPdu(txContainerId) == 0u)) /* OR Update timer to start it initially. */
    {
      IpduM_SetContainerPduSendTimeoutOfTxContainerPdu(txContainerId, IpduM_GetSendTimeoutOfTxContainedPdu(txContainedId));
    }
  }
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#define IPDUM_START_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/*******************************************************************************************
*  IpduM_ContainerTx_SendTimeout_Init()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_Init(void)
{
  IpduM_VTxContainerPduIterType txContainerPduIdx;

  for(txContainerPduIdx = 0u; txContainerPduIdx < IpduM_GetSizeOfVTxContainerPdu(); txContainerPduIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_SetContainerPduSendTimeoutOfTxContainerPdu(txContainerPduIdx, 0u);
  }
}

/*******************************************************************************************
*  IpduM_ContainerTx_SendTimeout_Start
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_Start(IpduM_TxContainedPduIterType txContainedId)
{
  IpduM_TxContainerPduIdxOfTxContainedPduType txContainerIdx = IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId);

  if(IpduM_ContainerTxTrigger_IsFirstContainedPdu(txContainerIdx))
  {
    IpduM_ContainerTx_SendTimeout_SetContainerPduTimeout(txContainerIdx);
  }
  IpduM_ContainerTx_SendTimeout_SetContainedPduTimeout(txContainedId);
}

/*******************************************************************************************
*  IpduM_ContainerTx_SendTimeout_Stop
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_Stop(IpduM_CTxContainerPduIterType txContainerId)
{
  IpduM_SetContainerPduSendTimeoutOfTxContainerPdu(txContainerId, 0u);
}

/*******************************************************************************************
*  IpduM_ContainerTx_SendTimeout_IsRunning
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_IsRunning(IpduM_CTxContainerPduIterType txContainerId)
{
  boolean isRunning = FALSE;

  if(IpduM_GetContainerPduSendTimeoutOfTxContainerPdu(txContainerId) > 0u)
  {
    isRunning = TRUE;
  }

  return isRunning;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_SendTimeout_MainFunction()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_SendTimeout_MainFunction(IpduM_CTxContainerPduIterType txContainerId)
{
  boolean isSendTimeoutElapsed = FALSE;

  IpduM_ContainerPduSendTimeoutOfTxContainerPduType containerPduSendTimeout = IpduM_GetContainerPduSendTimeoutOfTxContainerPdu(txContainerId);

  if(containerPduSendTimeout == 1u)
  {
    isSendTimeoutElapsed = TRUE;
  }
  IpduM_DecContainerPduSendTimeoutOfTxContainerPdu(txContainerId);

  return isSendTimeoutElapsed;
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerTxSendTimeout.c
**********************************************************************************************************************/
