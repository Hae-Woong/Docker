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
 *         \file  IpduM_ContainerTxDataQueue.c
 *         \unit  ContainerTxDataQueue
 *        \brief  Microsar IpduM Container Tx Data Queue source file
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

#define IPDUM_CONTAINERTXDATAQUEUE_SOURCE
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
#include "IpduM_ContainerTxDataQueue.h"
#include "IpduM_ContainerTxDataQueueInstance.h"
#include "IpduM_ContainerTxTrigger.h"
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
 *  IpduM_ContainerTx_DataQueue_IsFifoQueueEmpty
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_DataQueue_IsFifoQueueEmpty(IpduM_CTxContainerPduIterType txContainerId)
{
  boolean retVal = FALSE;
  IpduM_DataQueueInfoIdxOfTxContainerPduType dataQueueInfoIdx = IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId);

  if( (IpduM_WriteCountOfDataQueueInfoType) (IpduM_GetWriteCountOfDataQueueInfo(dataQueueInfoIdx) - IpduM_GetReadCountOfDataQueueInfo(dataQueueInfoIdx)) == 0u)
  {
    retVal = TRUE;
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_IsFifoQueueFull
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_DataQueue_IsFifoQueueFull(IpduM_CTxContainerPduIterType txContainerId)
{
  boolean retVal = FALSE;
  IpduM_DataQueueInfoIdxOfTxContainerPduType dataQueueInfoIdx = IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId);

  /* There are QueueDepth + 1 instances to always have a 'current open instance'. */
  IpduM_DataQueueInstanceLengthOfDataQueueInfoType queueDepth = IpduM_GetDataQueueInstanceLengthOfDataQueueInfo(dataQueueInfoIdx) - 1u; /* PRQA S 2842 */ /* MD_IpduM_2842 */
  IpduM_WriteCountOfDataQueueInfoType containerWriteCount = IpduM_GetWriteCountOfDataQueueInfo(dataQueueInfoIdx); /* PRQA S 2842 */ /* MD_IpduM_2842 */
  IpduM_ReadCountOfDataQueueInfoType containerReadCount = IpduM_GetReadCountOfDataQueueInfo(dataQueueInfoIdx); /* PRQA S 2842 */ /* MD_IpduM_2842 */

  if( (IpduM_WriteCountOfDataQueueInfoType) (containerWriteCount - containerReadCount) >= queueDepth)
  {
    retVal = TRUE;
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_FitsPduIntoCurrentOpenInstance
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_ContainerTx_DataQueue_FitsPduIntoCurrentOpenInstance(IpduM_CTxContainerPduIterType txContainerId, PduLengthType pduLength)
{
  IpduM_DataQueueInstanceWriteIdxOfDataQueueInfoType dataQueueInstanceWriteIdx = IpduM_GetDataQueueInstanceWriteIdxOfDataQueueInfo(IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId));
  /*@ assert (dataQueueInstanceWriteIdx  < IpduM_GetSizeOfCDataQueueInstance()); */  /* VCA_IPDUM_RING_BUFFER */
  return (IpduM_ContainerTx_DataQueueInstance_GetFreeBytesCount(dataQueueInstanceWriteIdx) >= (IpduM_GetHeaderSizeOfTxContainerPdu(txContainerId) + pduLength));
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_IncrementWritePtr
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_IncrementWritePtr(IpduM_CTxContainerPduIterType txContainerId)
{
  IpduM_DataQueueInfoIdxOfTxContainerPduType dataQueueInfoIdx = IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId);

  IpduM_IncWriteCountOfDataQueueInfo(dataQueueInfoIdx);
  IpduM_IncDataQueueInstanceWriteIdxOfDataQueueInfo(dataQueueInfoIdx);
  if(IpduM_GetDataQueueInstanceWriteIdxOfDataQueueInfo(dataQueueInfoIdx) >= IpduM_GetDataQueueInstanceEndIdxOfDataQueueInfo(dataQueueInfoIdx))
  {
    IpduM_SetDataQueueInstanceWriteIdxOfDataQueueInfo(dataQueueInfoIdx, IpduM_GetDataQueueInstanceStartIdxOfDataQueueInfo(dataQueueInfoIdx));
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_IncrementReadPtr
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_IncrementReadPtr(IpduM_CTxContainerPduIterType txContainerId)
{
  IpduM_DataQueueInfoIdxOfTxContainerPduType dataQueueInfoIdx = IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId);

  /* increment read pointer */
  IpduM_IncReadCountOfDataQueueInfo(dataQueueInfoIdx);
  IpduM_IncDataQueueInstanceReadIdxOfDataQueueInfo(dataQueueInfoIdx);
  if(IpduM_GetDataQueueInstanceReadIdxOfDataQueueInfo(dataQueueInfoIdx) >= IpduM_GetDataQueueInstanceEndIdxOfDataQueueInfo(dataQueueInfoIdx))
  {
    IpduM_SetDataQueueInstanceReadIdxOfDataQueueInfo(dataQueueInfoIdx, IpduM_GetDataQueueInstanceStartIdxOfDataQueueInfo(dataQueueInfoIdx));
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_Init
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_Init(void)
{
  IpduM_CTxContainerPduIterType txContainerIdx;

  SchM_Enter_IpduM_IPDUM_EXCLUSIVE_AREA_0();
  for(txContainerIdx = 0u; txContainerIdx < IpduM_GetSizeOfCTxContainerPdu(); txContainerIdx++) /* FETA_IPDUM_01 */
  {
    if(IpduM_IsDataQueueInfoUsedOfTxContainerPdu(txContainerIdx))
    {
      IpduM_DataQueueInfoIdxOfTxContainerPduType dataQueueInfoIdx = IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerIdx);

      /* DataQueueInfo Initialization */
      IpduM_SetDataQueueInstanceWriteIdxOfDataQueueInfo(dataQueueInfoIdx, IpduM_GetDataQueueInstanceStartIdxOfDataQueueInfo(dataQueueInfoIdx));
      IpduM_SetDataQueueInstanceReadIdxOfDataQueueInfo(dataQueueInfoIdx, IpduM_GetDataQueueInstanceStartIdxOfDataQueueInfo(dataQueueInfoIdx));
      IpduM_SetWriteCountOfDataQueueInfo(dataQueueInfoIdx, 0u);
      IpduM_SetReadCountOfDataQueueInfo(dataQueueInfoIdx, 0u);

      {/* DataQueueInstance Initialization */
        IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx = IpduM_GetDataQueueInstanceStartIdxOfDataQueueInfo(dataQueueInfoIdx);

        for (; dataQueueInstanceIdx < IpduM_GetDataQueueInstanceEndIdxOfDataQueueInfo(dataQueueInfoIdx); dataQueueInstanceIdx++) /* FETA_IPDUM_01 */
        {
          IpduM_ContainerTx_DataQueueInstance_Init(dataQueueInstanceIdx, txContainerIdx);
        }
      }
    }
  }
  SchM_Exit_IpduM_IPDUM_EXCLUSIVE_AREA_0();
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_AddPdu2IdBuffer()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_AddPdu2IdBuffer(IpduM_TxUpIndIdxOfTxUpInfoType txContainedId)
{
  IpduM_CTxContainerPduIterType txContainerId = IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId);

  IpduM_DataQueueInfoIdxOfTxContainerPduType dataQueueInfoIdx = IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId);
  IpduM_DataQueueInstanceWriteIdxOfDataQueueInfoType dataQueueInstanceWriteIdx = IpduM_GetDataQueueInstanceWriteIdxOfDataQueueInfo(dataQueueInfoIdx);
  /*@ assert (dataQueueInstanceWriteIdx  < IpduM_GetSizeOfCDataQueueInstance()); */  /* VCA_IPDUM_RING_BUFFER */
  IpduM_DataQueueIdBufferWriteIdxOfDataQueueInstanceType dataQueueIdBufferWriteIdx =  IpduM_GetDataQueueIdBufferWriteIdxOfDataQueueInstance(dataQueueInstanceWriteIdx);

  if(dataQueueIdBufferWriteIdx < IpduM_GetDataQueueIdBufferEndIdxOfDataQueueInstance(dataQueueInstanceWriteIdx))  /* COV_IPDUM_DEFENSIVE_PROGRAMMING */
  {
    IpduM_SetDataQueueIdBuffer(dataQueueIdBufferWriteIdx, (PduIdType) txContainedId);
    IpduM_IncDataQueueIdBufferWriteIdxOfDataQueueInstance(dataQueueInstanceWriteIdx);
  }
  else
  {
    IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_CONTAINERTX_DATAQUEUE_ADDPDU2IDBUFFER, IPDUM_E_GENERATED_DATA);
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_QueueCurrContainerPdu()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_QueueCurrContainerPdu(IpduM_CTxContainerPduIterType txContainerId)
{
  if(IpduM_ContainerTx_DataQueue_IsFifoQueueFull(txContainerId))
  {
    IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_CONTAINERTX_DATAQUEUE_QUEUECURRCONTAINERPDU, IPDUM_E_QUEUEOVFL);

    IpduM_DataQueueInstanceReadIdxOfDataQueueInfoType dataQueueInstanceReadIdx = IpduM_GetDataQueueInstanceReadIdxOfDataQueueInfo(IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId));
    /*@ assert (dataQueueInstanceReadIdx  < IpduM_GetSizeOfCDataQueueInstance()); */  /* VCA_IPDUM_RING_BUFFER */

    /* Reset the read queue instance and remove it by incrementing the readIdx of the queue. */
    IpduM_ContainerTx_DataQueueInstance_Init(dataQueueInstanceReadIdx, txContainerId);
    IpduM_ContainerTx_DataQueue_IncrementReadPtr(txContainerId);
  }

  /* Queue the container PDU (i.e. increment write pointer). */
  IpduM_ContainerTx_DataQueue_IncrementWritePtr(txContainerId);
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_AddPdu_DynamicLayout()
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
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_AddPdu_DynamicLayout(IpduM_TxContainedPduIterType txContainedId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  IpduM_CTxContainerPduIterType txContainerId = IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId);
  IpduM_DataQueueInstanceWriteIdxOfDataQueueInfoType dataQueueInstanceWriteIdx = IpduM_GetDataQueueInstanceWriteIdxOfDataQueueInfo(IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId));
  /*@ assert (dataQueueInstanceWriteIdx  < IpduM_GetSizeOfCDataQueueInstance()); */  /* VCA_IPDUM_RING_BUFFER */
  PduLengthType writeOffset = IpduM_GetFillCountOfDataQueueInstance(dataQueueInstanceWriteIdx);

  IpduM_ContainerTxTrigger_SetFirstContainedPduFlag(txContainerId, (IpduM_ContainerTx_DataQueueInstance_GetFillCount(dataQueueInstanceWriteIdx) == 0u));

  {
    uint8 headerSize = IpduM_GetHeaderSizeOfTxContainerPdu(txContainerId);

    (void) IpduM_ContainerTx_DataQueueInstance_PutHeader(dataQueueInstanceWriteIdx, writeOffset, IpduM_GetHeaderIdOfTxContainedPdu(txContainedId), PduInfoPtr->SduLength, headerSize);
    writeOffset += headerSize;
  }

  {
    (void) IpduM_ContainerTx_DataQueueInstance_Put(dataQueueInstanceWriteIdx, PduInfoPtr, writeOffset);
    writeOffset += PduInfoPtr->SduLength;
  }

  IpduM_SetFillCountOfDataQueueInstance(dataQueueInstanceWriteIdx, writeOffset);

  /* Write the Id to the internal 'Id buffer' as well. */
  IpduM_ContainerTx_DataQueue_AddPdu2IdBuffer((IpduM_TxUpIndIdxOfTxUpInfoType) txContainedId);
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueue_AddPdu_StaticLayout()
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
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueue_AddPdu_StaticLayout(IpduM_TxContainedPduIterType txContainedId, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  IpduM_CTxContainerPduIterType txContainerId = IpduM_GetTxContainerPduIdxOfTxContainedPdu(txContainedId);
  IpduM_DataQueueInstanceWriteIdxOfDataQueueInfoType dataQueueInstanceWriteIdx = IpduM_GetDataQueueInstanceWriteIdxOfDataQueueInfo(IpduM_GetDataQueueInfoIdxOfTxContainerPdu(txContainerId));
  /*@ assert (dataQueueInstanceWriteIdx  < IpduM_GetSizeOfCDataQueueInstance()); */  /* VCA_IPDUM_RING_BUFFER */

  IpduM_ContainerTxTrigger_SetFirstContainedPduFlag(txContainerId, (IpduM_ContainerTx_DataQueueInstance_GetFillCount(dataQueueInstanceWriteIdx) == 0u));

  {
    /* PDUs with no headers and static layout */
    /* Write the sdu to the buffer */
    (void) IpduM_ContainerTx_DataQueueInstance_Put(dataQueueInstanceWriteIdx, PduInfoPtr, IpduM_GetOffsetOfTxContainedPdu(txContainedId));

    if(IpduM_IsUpdateBitUsedOfTxContainedPdu(txContainedId))
    {
      (void) IpduM_ContainerTx_DataQueueInstance_SetBit(dataQueueInstanceWriteIdx, IpduM_GetUpdateIndicationBytePositionOfTxContainedPdu(txContainedId), IpduM_GetUpdateIndicationBitInBytePositionOfTxContainedPdu(txContainedId));
    }

    /* Mark Pdu as non-empty */
    IpduM_SetFillCountOfDataQueueInstance(dataQueueInstanceWriteIdx, IpduM_GetDataQueueBufferLengthOfDataQueueInstance(dataQueueInstanceWriteIdx));
  }

  /* Write the Id to the internal 'Id buffer' as well. */
  IpduM_ContainerTx_DataQueue_AddPdu2IdBuffer((IpduM_TxUpIndIdxOfTxUpInfoType) txContainedId);
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerTxDataQueue.c
**********************************************************************************************************************/
