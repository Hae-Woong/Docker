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
 *         \file  IpduM_TxConfBuffer.c
 *         \unit  TxConfBuffer
 *        \brief  Microsar IpduM Tx confirmation buffer source file
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

#define IPDUM_TXCONFBUFFER_SOURCE
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
#include "IpduM_TxConfBuffer.h"

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
 *  IpduM_TxConfBuffer_Init()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_TxConfBuffer_Init(void)
{
  IpduM_CContainerTxConfirmationBufferInfoIterType containerTxConfirmationBufferInfoIdx;

  for(containerTxConfirmationBufferInfoIdx = 0u; containerTxConfirmationBufferInfoIdx < IpduM_GetSizeOfCContainerTxConfirmationBufferInfo(); containerTxConfirmationBufferInfoIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_SetContainerTxConfirmationBufferReadIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx, IpduM_GetContainerTxConfirmationBufferStartIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx));
    IpduM_SetContainerTxConfirmationBufferWriteIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx, IpduM_GetContainerTxConfirmationBufferStartIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx));
  }
}

/*******************************************************************************************
 *  IpduM_TxConfBuffer_Clear()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_TxConfBuffer_Clear(IpduM_CTxContainerPduIterType txContainerId)
{
  if(IpduM_IsContainerTxConfirmationBufferInfoUsedOfTxContainerPdu(txContainerId))
  {
    IpduM_ContainerTxConfirmationBufferInfoIdxOfTxContainerPduType containerTxConfirmationBufferInfoIdx = IpduM_GetContainerTxConfirmationBufferInfoIdxOfTxContainerPdu(txContainerId);

    IpduM_SetContainerTxConfirmationBufferReadIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx, IpduM_GetContainerTxConfirmationBufferStartIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx));
    IpduM_SetContainerTxConfirmationBufferWriteIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx, IpduM_GetContainerTxConfirmationBufferStartIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx));
  }
}

/*******************************************************************************************
 *  IpduM_TxConfBuffer_Put()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_TxConfBuffer_Put(IpduM_CTxContainerPduIterType txContainerId, PduIdType txConfId)
{
  Std_ReturnType retVal = E_NOT_OK;

  if(IpduM_IsContainerTxConfirmationBufferInfoUsedOfTxContainerPdu(txContainerId))
  {
    IpduM_ContainerTxConfirmationBufferInfoIdxOfTxContainerPduType containerTxConfirmationBufferInfoIdx = IpduM_GetContainerTxConfirmationBufferInfoIdxOfTxContainerPdu(txContainerId);
    IpduM_ContainerTxConfirmationBufferWriteIdxOfContainerTxConfirmationBufferInfoType containerTxConfirmationBufferWriteIdx = IpduM_GetContainerTxConfirmationBufferWriteIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx);
    IpduM_ContainerTxConfirmationBufferEndIdxOfContainerTxConfirmationBufferInfoType   containerTxConfirmationBufferEndIdx = IpduM_GetContainerTxConfirmationBufferEndIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx);

    if(containerTxConfirmationBufferWriteIdx < containerTxConfirmationBufferEndIdx)  /* COV_IPDUM_DEFENSIVE_PROGRAMMING */
    {
      IpduM_SetContainerTxConfirmationBuffer(containerTxConfirmationBufferWriteIdx, txConfId);
      IpduM_IncContainerTxConfirmationBufferWriteIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx);
      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_TxConfBuffer_Get()
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
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_TxConfBuffer_Get(IpduM_CTxContainerPduIterType txContainerId, P2VAR(PduIdType, AUTOMATIC, IPDUM_APPL_DATA) txConfId)
{
  Std_ReturnType retVal = E_NOT_OK;

  if(IpduM_IsContainerTxConfirmationBufferInfoUsedOfTxContainerPdu(txContainerId))
  {
    IpduM_ContainerTxConfirmationBufferInfoIdxOfTxContainerPduType containerTxConfirmationBufferInfoIdx = IpduM_GetContainerTxConfirmationBufferInfoIdxOfTxContainerPdu(txContainerId);
    IpduM_ContainerTxConfirmationBufferReadIdxOfContainerTxConfirmationBufferInfoType  containerTxConfirmationBufferReadIdx = IpduM_GetContainerTxConfirmationBufferReadIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx);
    IpduM_ContainerTxConfirmationBufferWriteIdxOfContainerTxConfirmationBufferInfoType containerTxConfirmationBufferWriteIdx = IpduM_GetContainerTxConfirmationBufferWriteIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx);
    IpduM_ContainerTxConfirmationBufferEndIdxOfContainerTxConfirmationBufferInfoType   containerTxConfirmationBufferEndIdx = IpduM_GetContainerTxConfirmationBufferEndIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx);

    if(containerTxConfirmationBufferReadIdx < containerTxConfirmationBufferWriteIdx)
    {
      if(containerTxConfirmationBufferReadIdx < containerTxConfirmationBufferEndIdx)  /* COV_IPDUM_DEFENSIVE_PROGRAMMING */
      {
        *txConfId = IpduM_GetContainerTxConfirmationBuffer(containerTxConfirmationBufferReadIdx);
        IpduM_IncContainerTxConfirmationBufferReadIdxOfContainerTxConfirmationBufferInfo(containerTxConfirmationBufferInfoIdx);

        retVal = E_OK;
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_TxConfBuffer_GetSize()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(IpduM_ContainerTxConfirmationBufferLengthOfContainerTxConfirmationBufferInfoType, IPDUM_CODE) IpduM_TxConfBuffer_GetSize(IpduM_CTxContainerPduIterType txContainerId)
{
  IpduM_ContainerTxConfirmationBufferLengthOfContainerTxConfirmationBufferInfoType txConfBufferSize = 0u;

  if(IpduM_IsContainerTxConfirmationBufferInfoUsedOfTxContainerPdu(txContainerId))
  {
    txConfBufferSize = IpduM_GetContainerTxConfirmationBufferLengthOfContainerTxConfirmationBufferInfo(IpduM_GetContainerTxConfirmationBufferInfoIdxOfTxContainerPdu(txContainerId));
  }

  return txConfBufferSize;
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_TxConfBuffer.c
**********************************************************************************************************************/
