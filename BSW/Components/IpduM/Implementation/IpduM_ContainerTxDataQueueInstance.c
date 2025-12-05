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
 *         \file  IpduM_ContainerTxDataQueueInstance.c
 *         \unit  ContainerTxDataQueueInstance
 *        \brief  Microsar IpduM Container Tx Data Queue Instance source file
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

#define IPDUM_CONTAINERTXDATAQUEUEINSTANCE_SOURCE
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
#include "IpduM_ContainerTxDataQueueInstance.h"
#include "IpduM_ContainerUtil.h"
#include "IpduM_TxConfBuffer.h"
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
 *  IpduM_ContainerTx_DataQueueInstance_GetAsPointer()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduInfoType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_GetAsPointer(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx)
{
  PduInfoType pduInfo;

  pduInfo.SduDataPtr = IpduM_GetAddrDataQueueBuffer(IpduM_GetDataQueueBufferStartIdxOfDataQueueInstance(dataQueueInstanceIdx));
  pduInfo.SduLength  = IpduM_ContainerTx_DataQueueInstance_GetFillCount(dataQueueInstanceIdx);

  return pduInfo;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_Get()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_Get(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, P2VAR(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) destBuffer)
{
  PduInfoType containerBuffer;
  containerBuffer = IpduM_ContainerTx_DataQueueInstance_GetAsPointer(dataQueueInstanceIdx);

  /* Copy PDU from container buffer to provided destination buffer */
  VStdLib_MemCpy_s(destBuffer->SduDataPtr, destBuffer->SduLength, containerBuffer.SduDataPtr, containerBuffer.SduLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_IPDUM_MEMCPY_3 */
  destBuffer->SduLength = containerBuffer.SduLength;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_CopyContainedPduIDs2TxConfBuffer()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_CopyContainedPduIDs2TxConfBuffer(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, IpduM_CTxContainerPduIterType txContainerId)
{
  IpduM_DataQueueIdBufferWriteIdxOfDataQueueInstanceType dataQueueIdBufferReadIdx =  IpduM_GetDataQueueIdBufferStartIdxOfDataQueueInstance(dataQueueInstanceIdx);
  IpduM_DataQueueIdBufferWriteIdxOfDataQueueInstanceType dataQueueIdBufferWriteIdx =  IpduM_GetDataQueueIdBufferWriteIdxOfDataQueueInstance(dataQueueInstanceIdx);

  for(; dataQueueIdBufferReadIdx < dataQueueIdBufferWriteIdx; dataQueueIdBufferReadIdx++) /* FETA_IPDUM_04 */
  {
    (void) IpduM_TxConfBuffer_Put(txContainerId, IpduM_GetDataQueueIdBuffer(dataQueueIdBufferReadIdx));
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_GetFillCount
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduLengthType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_GetFillCount(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx)
{
  return IpduM_GetFillCountOfDataQueueInstance(dataQueueInstanceIdx);
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_Put
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_Put(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr, PduLengthType writeOffset)
{
  Std_ReturnType retVal = E_NOT_OK;

  IpduM_DataQueueBufferStartIdxOfDataQueueInstanceType dataQueueBufferStartIdx = IpduM_GetDataQueueBufferStartIdxOfDataQueueInstance(dataQueueInstanceIdx);
  IpduM_DataQueueBufferEndIdxOfDataQueueInstanceType dataQueueBufferEndIdx = IpduM_GetDataQueueBufferEndIdxOfDataQueueInstance(dataQueueInstanceIdx);

  if((dataQueueBufferStartIdx + writeOffset + PduInfoPtr->SduLength) < dataQueueBufferEndIdx)
  {
    VStdLib_MemCpy(IpduM_GetAddrDataQueueBuffer((IpduM_DataQueueBufferIterType) dataQueueBufferStartIdx + (IpduM_DataQueueBufferIterType) writeOffset), PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_IPDUM_MEMCPY_4 */
    retVal = E_OK;
  }
  else
  {
    IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_CONTAINERTX_DATAQUEUEINSTANCE_PUT, IPDUM_E_GENERATED_DATA);
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_PutHeader
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_PutHeader(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, PduLengthType writeOffset, uint32 headerId, uint32 dlc, uint8 headerSize)
{
  Std_ReturnType retVal = E_NOT_OK;

  IpduM_DataQueueBufferStartIdxOfDataQueueInstanceType dataQueueBufferStartIdx = IpduM_GetDataQueueBufferStartIdxOfDataQueueInstance(dataQueueInstanceIdx);
  IpduM_DataQueueBufferEndIdxOfDataQueueInstanceType dataQueueBufferEndIdx = IpduM_GetDataQueueBufferEndIdxOfDataQueueInstance(dataQueueInstanceIdx);

  if((dataQueueBufferStartIdx + writeOffset + headerSize) < dataQueueBufferEndIdx)
  {
    IpduM_ContainerUtil_WriteHeader(IpduM_GetAddrDataQueueBuffer((IpduM_DataQueueBufferIterType) dataQueueBufferStartIdx + (IpduM_DataQueueBufferIterType) writeOffset), headerId, dlc, headerSize); /* PRQA S 2842 */ /* MD_IpduM_2842 */
    retVal = E_OK;
  }
  else
  {
    IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_CONTAINERTX_DATAQUEUEINSTANCE_PUTHEADER, IPDUM_E_GENERATED_DATA);
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_SetBit
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_SetBit(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, PduLengthType bytePosition, uint8 bitInBytePosition)
{
  Std_ReturnType retVal = E_NOT_OK;

  IpduM_DataQueueBufferStartIdxOfDataQueueInstanceType dataQueueBufferStartIdx = IpduM_GetDataQueueBufferStartIdxOfDataQueueInstance(dataQueueInstanceIdx);
  IpduM_DataQueueBufferEndIdxOfDataQueueInstanceType dataQueueBufferEndIdx = IpduM_GetDataQueueBufferEndIdxOfDataQueueInstance(dataQueueInstanceIdx);

  uint8 updateBitSetPattern = (uint8) (0x01u << (bitInBytePosition));

  IpduM_DataQueueBufferIterType dataQueueBufferWritedx = (IpduM_DataQueueBufferIterType) dataQueueBufferStartIdx + (IpduM_DataQueueBufferIterType) bytePosition;
  if(dataQueueBufferWritedx < dataQueueBufferEndIdx)
  {
    uint8 dataByte = IpduM_GetDataQueueBuffer(dataQueueBufferWritedx);

    dataByte |= updateBitSetPattern;
    IpduM_SetDataQueueBuffer(dataQueueBufferWritedx, dataByte);

    retVal = E_OK;
  }
  else
  {
    IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_CONTAINERTX_DATAQUEUEINSTANCE_SETBIT, IPDUM_E_GENERATED_DATA);
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_ClearBit
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_ClearBit(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, PduLengthType bytePosition, uint8 bitInBytePosition)
{
  Std_ReturnType retVal = E_NOT_OK;

  IpduM_DataQueueBufferStartIdxOfDataQueueInstanceType dataQueueBufferStartIdx = IpduM_GetDataQueueBufferStartIdxOfDataQueueInstance(dataQueueInstanceIdx);
  IpduM_DataQueueBufferEndIdxOfDataQueueInstanceType dataQueueBufferEndIdx = IpduM_GetDataQueueBufferEndIdxOfDataQueueInstance(dataQueueInstanceIdx);

  uint8 updateBitClearPattern = ~((uint8) (0x01u << (bitInBytePosition)));

  IpduM_DataQueueBufferIterType dataQueueBufferWriteIdx = (IpduM_DataQueueBufferIterType) dataQueueBufferStartIdx + (IpduM_DataQueueBufferIterType) bytePosition;
  if(dataQueueBufferWriteIdx < dataQueueBufferEndIdx)
  {
    uint8 dataByte = IpduM_GetDataQueueBuffer(dataQueueBufferWriteIdx);

    dataByte &= updateBitClearPattern;
    IpduM_SetDataQueueBuffer(dataQueueBufferWriteIdx, dataByte);

    retVal = E_OK;
  }
  else
  {
    IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_CONTAINERTX_DATAQUEUEINSTANCE_CLEARBIT, IPDUM_E_GENERATED_DATA);
  }

  return retVal;
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_GetFreeBytesCount
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(PduLengthType, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_GetFreeBytesCount(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx)
{
  IpduM_DataQueueBufferLengthOfDataQueueInstanceType   dataQueueBufferLength = IpduM_GetDataQueueBufferLengthOfDataQueueInstance(dataQueueInstanceIdx);

  return  (((PduLengthType) dataQueueBufferLength) - IpduM_ContainerTx_DataQueueInstance_GetFillCount(dataQueueInstanceIdx));
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_Init
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_Init(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, IpduM_CTxContainerPduIterType txContainerIdx)
{
  /* Reset dataBuffer Write Idx to 'clear' the buffer */
  IpduM_SetFillCountOfDataQueueInstance(dataQueueInstanceIdx, 0u);
  IpduM_SetDataQueueIdBufferWriteIdxOfDataQueueInstance(dataQueueInstanceIdx, IpduM_GetDataQueueIdBufferStartIdxOfDataQueueInstance(dataQueueInstanceIdx));

  /* Initialize current containers with header type == none */
  if(IpduM_GetHeaderSizeOfTxContainerPdu(txContainerIdx) == IPDUM_HEADERSIZE_NONE)
  {
    IpduM_ContainerTx_DataQueueInstance_ClearBuffer(dataQueueInstanceIdx, IpduM_GetUnusedBitPatternOfTxContainerPdu(txContainerIdx));
    IpduM_ContainerTx_DataQueueInstance_ResetUpdateBits(dataQueueInstanceIdx, txContainerIdx);
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_ClearBuffer()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_ClearBuffer(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, uint8 clearPattern)
{
  IpduM_DataQueueBufferEndIdxOfDataQueueInstanceType dataQueueBufferIdx = IpduM_GetDataQueueBufferStartIdxOfDataQueueInstance(dataQueueInstanceIdx);

  for(; dataQueueBufferIdx < IpduM_GetDataQueueBufferEndIdxOfDataQueueInstance(dataQueueInstanceIdx); dataQueueBufferIdx++) /* FETA_IPDUM_01 */
  {
    IpduM_SetDataQueueBuffer(dataQueueBufferIdx, clearPattern);
  }
}

/*******************************************************************************************
 *  IpduM_ContainerTx_DataQueueInstance_ResetUpdateBits()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*******************************************************************************************/
FUNC(void, IPDUM_CODE) IpduM_ContainerTx_DataQueueInstance_ResetUpdateBits(IpduM_CDataQueueInstanceIterType dataQueueInstanceIdx, IpduM_CTxContainerPduIterType txContainerId)
{
  IpduM_TxContainedPduIterType txContainedPduIdx = IpduM_GetTxContainedPduStartIdxOfTxContainerPdu(txContainerId);

  for(; txContainedPduIdx < IpduM_GetTxContainedPduEndIdxOfTxContainerPdu(txContainerId); txContainedPduIdx++) /* FETA_IPDUM_01 */
  {
    if(IpduM_IsUpdateBitUsedOfTxContainedPdu(txContainedPduIdx))
    {
      (void) IpduM_ContainerTx_DataQueueInstance_ClearBit(dataQueueInstanceIdx, IpduM_GetUpdateIndicationBytePositionOfTxContainedPdu(txContainedPduIdx), IpduM_GetUpdateIndicationBitInBytePositionOfTxContainedPdu(txContainedPduIdx));
    }
  }
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_ContainerTxDataQueueInstance.c
**********************************************************************************************************************/
