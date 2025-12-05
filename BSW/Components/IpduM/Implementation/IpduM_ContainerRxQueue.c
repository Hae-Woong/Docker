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
 *         \file  IpduM_ContainerRxQueue.c
 *         \unit  ContainerRxQueue
 *        \brief  Microsar IpduM Container Rx Queue source file
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

#define IPDUM_CONTAINERRXQUEUE_SOURCE
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
#include "IpduM_ContainerRxQueue.h"
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
 *  IpduM_ContainerRxQueue_Put()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerRxQueue_Put(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx, P2CONST(uint8, AUTOMATIC, IPDUM_APPL_DATA) dataPtr, PduLengthType pduLength)
{
  Std_ReturnType retVal = E_NOT_OK;
  
  if(!IpduM_ContainerRxQueue_IsFull(containerRxQueueInfoIdx))
  {
    IpduM_ContainerRxQueueInstanceWriteIdxOfContainerRxQueueInfoType containerRxQueueInstanceWriteIdx = IpduM_GetContainerRxQueueInstanceWriteIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx);
    /*@ assert (containerRxQueueInstanceWriteIdx  < IpduM_GetSizeOfCContainerRxQueueInstance()); */  /* VCA_IPDUM_RING_BUFFER */
    IpduM_ContainerRxQueueBufferStartIdxOfContainerRxQueueInstanceType containerRxQueueBufferStartIdx = IpduM_GetContainerRxQueueBufferStartIdxOfContainerRxQueueInstance(containerRxQueueInstanceWriteIdx);
    IpduM_ContainerRxQueueBufferLengthOfContainerRxQueueInstanceType containerRxQueueBufferLength = IpduM_GetContainerRxQueueBufferLengthOfContainerRxQueueInstance(containerRxQueueInstanceWriteIdx);

    VStdLib_MemCpy_s(IpduM_GetAddrContainerRxQueueBuffer(containerRxQueueBufferStartIdx), containerRxQueueBufferLength, dataPtr, pduLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_IPDUM_MEMCPY_2 */

    /* store actual received pduLength */
    IpduM_SetReceivedSizeOfContainerRxQueueInstance(containerRxQueueInstanceWriteIdx, pduLength);

    /* update write pointers */
    IpduM_IncWriteCountOfContainerRxQueueInfo(containerRxQueueInfoIdx);

    IpduM_IncContainerRxQueueInstanceWriteIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx);
    if(IpduM_GetContainerRxQueueInstanceWriteIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx) >= IpduM_GetContainerRxQueueInstanceEndIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx))
    {
      IpduM_SetContainerRxQueueInstanceWriteIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx, IpduM_GetContainerRxQueueInstanceStartIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx));
    }
    retVal = E_OK;
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_Get()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerRxQueue_Get(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx, P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) dataPtr, P2VAR(PduLengthType, AUTOMATIC, IPDUM_APPL_DATA) pduLength)
{
  Std_ReturnType retVal = E_NOT_OK;

  if(!IpduM_ContainerRxQueue_IsEmpty(containerRxQueueInfoIdx))
  {
    IpduM_ContainerRxQueueInstanceReadIdxOfContainerRxQueueInfoType containerRxQueueInstanceReadIdx = IpduM_GetContainerRxQueueInstanceReadIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx);
    /*@ assert (containerRxQueueInstanceReadIdx  < IpduM_GetSizeOfCContainerRxQueueInstance()); */  /* VCA_IPDUM_RING_BUFFER */
    IpduM_ContainerRxQueueBufferStartIdxOfContainerRxQueueInstanceType containerRxQueueBufferStartIdx = IpduM_GetContainerRxQueueBufferStartIdxOfContainerRxQueueInstance(containerRxQueueInstanceReadIdx);
    IpduM_ContainerRxQueueBufferLengthOfContainerRxQueueInstanceType containerRxQueueBufferLength = IpduM_GetContainerRxQueueBufferLengthOfContainerRxQueueInstance(containerRxQueueInstanceReadIdx);

    VStdLib_MemCpy_s(dataPtr, *pduLength, IpduM_GetAddrContainerRxQueueBuffer(containerRxQueueBufferStartIdx), containerRxQueueBufferLength);  /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* VCA_IPDUM_MEMCPY_1 */

    *pduLength = IpduM_GetReceivedSizeOfContainerRxQueueInstance(containerRxQueueInstanceReadIdx);

    (void) IpduM_ContainerRxQueue_Remove(containerRxQueueInfoIdx);

    retVal = E_OK;
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_Remove
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerRxQueue_Remove(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx)
{
  Std_ReturnType retVal = E_NOT_OK;

  if(!IpduM_ContainerRxQueue_IsEmpty(containerRxQueueInfoIdx))
  {
    /* increment read pointer */
    IpduM_IncReadCountOfContainerRxQueueInfo(containerRxQueueInfoIdx);

    IpduM_IncContainerRxQueueInstanceReadIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx);
    if(IpduM_GetContainerRxQueueInstanceReadIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx) >= IpduM_GetContainerRxQueueInstanceEndIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx))
    {
      IpduM_SetContainerRxQueueInstanceReadIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx, IpduM_GetContainerRxQueueInstanceStartIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx));
    }
    retVal = E_OK;
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_Init
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerRxQueue_Init(void)
{
  IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx;

  for(containerRxQueueInfoIdx = 0u; containerRxQueueInfoIdx < IpduM_GetSizeOfCContainerRxQueueInfo(); containerRxQueueInfoIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_SetContainerRxQueueInstanceReadIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx, IpduM_GetContainerRxQueueInstanceStartIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx));
    IpduM_SetContainerRxQueueInstanceWriteIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx, IpduM_GetContainerRxQueueInstanceStartIdxOfContainerRxQueueInfo(containerRxQueueInfoIdx));
    IpduM_SetReadCountOfContainerRxQueueInfo(containerRxQueueInfoIdx, 0u);
    IpduM_SetWriteCountOfContainerRxQueueInfo(containerRxQueueInfoIdx, 0u);
  }
}

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_IsFull
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_ContainerRxQueue_IsFull(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx)
{
  return ((IpduM_WriteCountOfContainerRxQueueInfoType) (IpduM_GetWriteCountOfContainerRxQueueInfo(containerRxQueueInfoIdx) - IpduM_GetReadCountOfContainerRxQueueInfo(containerRxQueueInfoIdx))
                >= IpduM_GetContainerRxQueueInstanceLengthOfContainerRxQueueInfo(containerRxQueueInfoIdx));
}

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_IsEmpty
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_ContainerRxQueue_IsEmpty(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx)
{
  return (IpduM_WriteCountOfContainerRxQueueInfoType) (IpduM_GetWriteCountOfContainerRxQueueInfo(containerRxQueueInfoIdx) - IpduM_GetReadCountOfContainerRxQueueInfo(containerRxQueueInfoIdx)) == 0u;
}

/*******************************************************************************************
 *  IpduM_ContainerRxQueue_GetQueueDepth
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, IPDUM_CODE) IpduM_ContainerRxQueue_GetQueueDepth(IpduM_CContainerRxQueueInfoIterType containerRxQueueInfoIdx)
{
  /* The maximum queue depth can be 254 according to the bswmd parameter IpduMContainerRxPdu/IpduMContainerQueueSize. */
  return (uint8) IpduM_GetContainerRxQueueInstanceLengthOfContainerRxQueueInfo(containerRxQueueInfoIdx);
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerRxQueue.c
**********************************************************************************************************************/
