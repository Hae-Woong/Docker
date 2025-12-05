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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Com_CprQueueSigGw.c
 *         \unit  CprQueueSigGw
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_CPRQUEUESIGGW_SOURCE

 /**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_CprQueueSigGw.h"
#include "Com_Cfg_CrossPartitionQueueData.h"
#include "Com_Cfg_SignalGatewayData.h"
#include "Com_Cfg_ReceptionProcessingData.h"

#include "Com_CprQueue.h"
#include "Com_RxSigBuffer.h"

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
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */

/**********************************************************************************************************************
  Com_CprQueueSigGw_CopySigOrGrpSig
**********************************************************************************************************************/
/*! \brief        This function extracts the signal or group signal data from the CprQueue and copies it to the related tmp buffer.
    \details      -
    \pre          -
    \param[in]    cprQueuePtr                              Pointer to CprQueue
    \param[in]    gwSigOrGrpSigTmpBufferInfoIdx            Index of GwSigOrGrpSigTmpBufferInfo
    \param[in]    length                                   Length of Signal in Byte
    \return       CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) Pointer to the memory where the tmp signal value is located
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(P2VAR(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_CprQueueSigGw_CopySigOrGrpSig(CONSTP2CONST(Com_CprQueueBufferType, AUTOMATIC, AUTOMATIC) cprQueuePtr, Com_GwSigOrGrpSigTmpBufferInfoIterType gwSigOrGrpSigTmpBufferInfoIdx, Com_SignalLengthInByteOfRxAccessInfoType length);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_CopySignal
 *********************************************************************************************************************/
/*! \brief       This function copies the signal from the queue starting at the readIdx position.
 *  \details     -
 *  \param[in]   routingQueueIndex   Index to the queue
 *  \param[in]   readIdx             Index to the position in the queue where the data shall be read
 *  \param[in]   idxGwSigMapping     Handle Id of the signal gateway to be copied
 *  \param[in]   data                pointer to the data to be copied
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_CopySignal(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx,
                                                                           CONSTP2VAR(Com_GwSigMappingEndIdxOfGwInfoType, AUTOMATIC, AUTOMATIC) idxGwSigMapping, CONSTP2VAR(ComDataTypeVar, AUTOMATIC,
                                                                                                                                                                      AUTOMATIC) data);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_CopySignalGroupId
 *********************************************************************************************************************/
/*! \brief       This function copies the signal group routing id from the queue starting at the readIdx position.
 *  \details     -
 *  \param[in]   routingQueueIndex   Index to the queue
 *  \param[in]   readIdx             Index to the position in the queue where the data shall be read
 *  \param[in]   idxGwSigGrpMapping  Handle Id of the signalGroup routing to be copied from the queue
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_CopySignalGroupId(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx,
                                                                                CONSTP2VAR(Com_GwSigGrpMappingEndIdxOfGwInfoType, AUTOMATIC, AUTOMATIC) idxGwSigGrpMapping);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_CopyGroupSignal
 *********************************************************************************************************************/
/*! \brief       This function copies the group signal from the queue starting at the readIdx position.
 *  \details     -
 *  \param[in]   routingQueueIndex   Index to the queue
 *  \param[in]   idxGwGrpSigMapping  Handle Id of the groupSignal routing
 *  \param[in]   data                pointer to the data to be copied
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_CopyGroupSignal(Com_CCprQueueIterType routingQueueIndex, Com_GwGrpSigMappingIterType idxGwGrpSigMapping,
                                                                                CONSTP2VAR(ComDataTypeVar, AUTOMATIC, AUTOMATIC) data);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_GetSignalHeaderSize
 *********************************************************************************************************************/
/*! \brief       This function returns the size of the header which is put in front of the signal gateway Payload in the CrossPartitionRoutingQueue.
 *  \details     -
 *  \return      the header size
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueSigGw_GetSignalHeaderSize(void);

/**********************************************************************************************************************
 * Com_CprQueueSigGw_GetSignalGroupHeaderSize
 *********************************************************************************************************************/
/*! \brief       This function returns the size of the header which is put in front of the signalGroup gateway Payload in the CrossPartitionRoutingQueue.
 *  \details     -
 *  \return      the header size
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueSigGw_GetSignalGroupHeaderSize(void);
#endif


/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/
#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */

/**********************************************************************************************************************
  Com_CprQueueSigGw_CopySigOrGrpSig
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
COM_LOCAL_INLINE FUNC(P2VAR(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_CprQueueSigGw_CopySigOrGrpSig(CONSTP2CONST(Com_CprQueueBufferType, AUTOMATIC, AUTOMATIC) cprQueuePtr, Com_GwSigOrGrpSigTmpBufferInfoIterType gwSigOrGrpSigTmpBufferInfoIdx, Com_SignalLengthInByteOfRxAccessInfoType length)
{
  P2VAR(void, AUTOMATIC, AUTOMATIC) tmpSigBufferPtr = NULL_PTR;
  switch (Com_GetTmpTxSigBufferOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx))
  {
    case COM_TMPTXSIGBUFFERBUFFERUINT8_TMPTXSIGBUFFEROFGWSIGORGRPSIGTMPBUFFERINFO:
    {
      const Com_TmpTxSigBufferBufferUInt8IterType bufferIdx = (Com_TmpTxSigBufferBufferUInt8IterType) Com_GetTmpTxSigBufferStartIdxOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx);
      VStdLib_MemCpy_s(Com_GetAddrTmpTxSigBufferBufferUInt8(bufferIdx), sizeof(Com_TmpTxSigBufferBufferUInt8Type), cprQueuePtr, length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpTxSigBufferBufferUInt8(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPTXSIGBUFFERBUFFERSINT8_TMPTXSIGBUFFEROFGWSIGORGRPSIGTMPBUFFERINFO:
    {
      const Com_TmpTxSigBufferBufferSInt8IterType bufferIdx = (Com_TmpTxSigBufferBufferSInt8IterType) Com_GetTmpTxSigBufferStartIdxOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx);
      VStdLib_MemCpy_s(Com_GetAddrTmpTxSigBufferBufferSInt8(bufferIdx), sizeof(Com_TmpTxSigBufferBufferSInt8Type), cprQueuePtr, length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpTxSigBufferBufferSInt8(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPTXSIGBUFFERBUFFERUINT16_TMPTXSIGBUFFEROFGWSIGORGRPSIGTMPBUFFERINFO:
    {
      const Com_TmpTxSigBufferBufferUInt16IterType bufferIdx = (Com_TmpTxSigBufferBufferUInt16IterType) Com_GetTmpTxSigBufferStartIdxOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx);
      VStdLib_MemCpy_s(Com_GetAddrTmpTxSigBufferBufferUInt16(bufferIdx), sizeof(Com_TmpTxSigBufferBufferUInt16Type), cprQueuePtr, length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpTxSigBufferBufferUInt16(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPTXSIGBUFFERBUFFERSINT16_TMPTXSIGBUFFEROFGWSIGORGRPSIGTMPBUFFERINFO:
    {
      const Com_TmpTxSigBufferBufferSInt16IterType bufferIdx = (Com_TmpTxSigBufferBufferSInt16IterType) Com_GetTmpTxSigBufferStartIdxOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx);
      VStdLib_MemCpy_s(Com_GetAddrTmpTxSigBufferBufferSInt16(bufferIdx), sizeof(Com_TmpTxSigBufferBufferSInt16Type), cprQueuePtr, length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpTxSigBufferBufferSInt16(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPTXSIGBUFFERBUFFERUINT32_TMPTXSIGBUFFEROFGWSIGORGRPSIGTMPBUFFERINFO:
    {
      const Com_TmpTxSigBufferBufferUInt32IterType bufferIdx = (Com_TmpTxSigBufferBufferUInt32IterType) Com_GetTmpTxSigBufferStartIdxOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx);
      VStdLib_MemCpy_s(Com_GetAddrTmpTxSigBufferBufferUInt32(bufferIdx), sizeof(Com_TmpTxSigBufferBufferUInt32Type), cprQueuePtr, length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpTxSigBufferBufferUInt32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPTXSIGBUFFERBUFFERSINT32_TMPTXSIGBUFFEROFGWSIGORGRPSIGTMPBUFFERINFO:
    {
      const Com_TmpTxSigBufferBufferSInt32IterType bufferIdx = (Com_TmpTxSigBufferBufferSInt32IterType) Com_GetTmpTxSigBufferStartIdxOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx);
      VStdLib_MemCpy_s(Com_GetAddrTmpTxSigBufferBufferSInt32(bufferIdx), sizeof(Com_TmpTxSigBufferBufferSInt32Type), cprQueuePtr, length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpTxSigBufferBufferSInt32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPTXSIGBUFFERBUFFERUINT64_TMPTXSIGBUFFEROFGWSIGORGRPSIGTMPBUFFERINFO:
    {
      const Com_TmpTxSigBufferBufferUInt64IterType bufferIdx = (Com_TmpTxSigBufferBufferUInt64IterType) Com_GetTmpTxSigBufferStartIdxOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx);
      VStdLib_MemCpy_s(Com_GetAddrTmpTxSigBufferBufferUInt64(bufferIdx), sizeof(Com_TmpTxSigBufferBufferUInt64Type), cprQueuePtr, length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpTxSigBufferBufferUInt64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPTXSIGBUFFERBUFFERSINT64_TMPTXSIGBUFFEROFGWSIGORGRPSIGTMPBUFFERINFO:
    {
      const Com_TmpTxSigBufferBufferSInt64IterType bufferIdx = (Com_TmpTxSigBufferBufferSInt64IterType) Com_GetTmpTxSigBufferStartIdxOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx);
      VStdLib_MemCpy_s(Com_GetAddrTmpTxSigBufferBufferSInt64(bufferIdx), sizeof(Com_TmpTxSigBufferBufferSInt64Type), cprQueuePtr, length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpTxSigBufferBufferSInt64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPTXSIGBUFFERBUFFERFLOAT32_TMPTXSIGBUFFEROFGWSIGORGRPSIGTMPBUFFERINFO:
    {
      const Com_TmpTxSigBufferBufferFloat32IterType bufferIdx = (Com_TmpTxSigBufferBufferFloat32IterType) Com_GetTmpTxSigBufferStartIdxOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx);
      VStdLib_MemCpy_s(Com_GetAddrTmpTxSigBufferBufferFloat32(bufferIdx), sizeof(Com_TmpTxSigBufferBufferFloat32Type), cprQueuePtr, length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpTxSigBufferBufferFloat32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPTXSIGBUFFERBUFFERFLOAT64_TMPTXSIGBUFFEROFGWSIGORGRPSIGTMPBUFFERINFO:
    {
      const Com_TmpTxSigBufferBufferFloat64IterType bufferIdx = (Com_TmpTxSigBufferBufferFloat64IterType) Com_GetTmpTxSigBufferStartIdxOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx);
      VStdLib_MemCpy_s(Com_GetAddrTmpTxSigBufferBufferFloat64(bufferIdx), sizeof(Com_TmpTxSigBufferBufferFloat64Type), cprQueuePtr, length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpTxSigBufferBufferFloat64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPTXSIGBUFFERBUFFERARRAYBASED_TMPTXSIGBUFFEROFGWSIGORGRPSIGTMPBUFFERINFO:
    {
      const Com_TmpTxSigBufferBufferArrayBasedIterType bufferIdx = (Com_TmpTxSigBufferBufferArrayBasedIterType) Com_GetTmpTxSigBufferStartIdxOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx);
      VStdLib_MemCpy_s(Com_GetAddrTmpTxSigBufferBufferArrayBased(bufferIdx), Com_GetTmpTxSigBufferLengthOfGwSigOrGrpSigTmpBufferInfo(gwSigOrGrpSigTmpBufferInfoIdx), cprQueuePtr, length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpTxSigBufferBufferArrayBased(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    default: /* COV_COM_MISRA */
      /* Intentionally Empty */
      break;
  }

  return tmpSigBufferPtr;
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_CopySignal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_CopySignal(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx,
                                                                           CONSTP2VAR(Com_GwSigMappingEndIdxOfGwInfoType, AUTOMATIC, AUTOMATIC) idxGwSigMapping, CONSTP2VAR(ComDataTypeVar, AUTOMATIC,
                                                                                                                                                                      AUTOMATIC) data)
{
  Com_CprQueueBufferReadIdxOfCprQueueType readIdxTmp = readIdx;
  const uint8 sizeOfIndexType = sizeof(Com_GwSigMappingEndIdxOfGwInfoType);

  /* skip Header */
  readIdxTmp += COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE;

  /* Copy signal gateway index from queue */
  VStdLib_MemCpy(idxGwSigMapping, Com_GetAddrCprQueueBuffer(readIdxTmp), sizeOfIndexType);     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
  readIdxTmp +=sizeOfIndexType;

  /* If it isn't a ZeroBit Signal */
  if(Com_IsGwSigOrGrpSigTmpBufferInfoUsedOfGwSigMapping(*idxGwSigMapping))
  {
    const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoIdxOfGwSigMapping(*idxGwSigMapping);
    CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) cprQueuePtr = Com_GetAddrCprQueueBuffer(readIdxTmp);
    const Com_GwSigOrGrpSigTmpBufferInfoIdxOfGwSigMappingType gwSigOrGrpSigTmpBufferInfoIdx = Com_GetGwSigOrGrpSigTmpBufferInfoIdxOfGwSigMapping(*idxGwSigMapping);

    /* Copy data from CprQueue and return pointer to TmpBuffer */
    data->dataPtr = Com_CprQueueSigGw_CopySigOrGrpSig(cprQueuePtr, gwSigOrGrpSigTmpBufferInfoIdx, Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo));
    data->dataLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);
    readIdxTmp += (Com_CprQueueBufferReadIdxOfCprQueueType) data->dataLength;
  }

  /* Set the returned ReadIdx as pending ReadIdx: the readIdx will be set to this value once the read is finished */
  Com_SetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex, readIdxTmp);
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_CopySignalGroupId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_CopySignalGroupId(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx,
                                                                                CONSTP2VAR(Com_GwSigGrpMappingEndIdxOfGwInfoType, AUTOMATIC, AUTOMATIC) idxGwSigGrpMapping)
{
  Com_CprQueueBufferReadIdxOfCprQueueType readIdxTmp = readIdx;
  const uint8 sizeOfIndexType = sizeof(Com_GwSigGrpMappingEndIdxOfGwInfoType);

  /* skip Header */
  readIdxTmp += COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE;

  /* Copy signal gateway index from queue*/
  VStdLib_MemCpy(idxGwSigGrpMapping, Com_GetAddrCprQueueBuffer(readIdxTmp), sizeOfIndexType);     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
  readIdxTmp += sizeOfIndexType;

  /* Set the returned ReadIdx as pending ReadIdx: the readIdx will be set to this value once the read is finished */
  Com_SetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex, readIdxTmp);
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_CopyGroupSignal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CprQueueSigGw_CopyGroupSignal(Com_CCprQueueIterType routingQueueIndex, Com_GwGrpSigMappingIterType idxGwGrpSigMapping,
                                                                                CONSTP2VAR(ComDataTypeVar, AUTOMATIC, AUTOMATIC) data)
{
  /* If it isn't a ZeroBit GroupSignal */
  if(Com_IsGwSigOrGrpSigTmpBufferInfoUsedOfGwGrpSigMapping(idxGwGrpSigMapping))
  {
    Com_CprQueueBufferReadIdxOfCprQueueType readIdxTmp = Com_GetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex);
    const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoIdxOfGwGrpSigMapping(idxGwGrpSigMapping);

    /* Copy data from CprQueue and return pointer to TmpBuffer */
    CONSTP2CONST(Com_CprQueueBufferType, AUTOMATIC, AUTOMATIC) cprQueuePtr = Com_GetAddrCprQueueBuffer(readIdxTmp);
    const Com_GwSigOrGrpSigTmpBufferInfoIdxOfGwGrpSigMappingType gwSigOrGrpSigTmpBufferInfoIdx = Com_GetGwSigOrGrpSigTmpBufferInfoIdxOfGwGrpSigMapping(idxGwGrpSigMapping);
    data->dataPtr = Com_CprQueueSigGw_CopySigOrGrpSig(cprQueuePtr, gwSigOrGrpSigTmpBufferInfoIdx, Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo));
    data->dataLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);
    readIdxTmp += (Com_CprQueueBufferReadIdxOfCprQueueType) data->dataLength; /* PRQA S 2986 */ /* MD_Com_2985_2986 */

    /* Set the returned ReadIdx as pending ReadIdx: the readIdx will be set to this value once the read is finished */
    Com_SetCprQueueBufferPendingReadIdxOfCprQueue(routingQueueIndex, readIdxTmp);
  }
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_GetSignalHeaderSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueSigGw_GetSignalHeaderSize(void)
{
  return COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE + sizeof(Com_GwSigMappingEndIdxOfGwInfoType);
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_GetSignalGroupHeaderSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_CprQueueSigGw_GetSignalGroupHeaderSize(void)
{
  return COM_CPRQUEUE_HEADERVALUE_SIZEINBYTE + sizeof(Com_GwSigGrpMappingEndIdxOfGwInfoType);
}
#endif


/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON) /* COV_COM_GATEWAY_MAINFUNCTION_ROUTESIGNALSAPI */
/**********************************************************************************************************************
 * Com_CprQueueSigGw_Read_Signal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_Signal(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx,
                                                   CONSTP2VAR(Com_GwSigMappingEndIdxOfGwInfoType, AUTOMATIC, AUTOMATIC) idxGwSigMapping, CONSTP2VAR(ComDataTypeVar, AUTOMATIC, AUTOMATIC) data)
{
  /* Read data from queue */
  Com_CprQueueSigGw_CopySignal(routingQueueIndex, readIdx, idxGwSigMapping, data);

  /* Increase the number of bytes read */
  const Com_CprQueueBufferNumberOfBytesReadIdxOfCprQueueType cprQueueBufferNumberOfBytesReadIdx = (Com_CprQueueBufferNumberOfBytesReadIdxOfCprQueueType) Com_CprQueueSigGw_GetSignalHeaderSize() + (Com_CprQueueBufferNumberOfBytesReadIdxOfCprQueueType) data->dataLength;
  Com_AddCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, cprQueueBufferNumberOfBytesReadIdx);
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_Read_SignalGroupId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_SignalGroupId(Com_CCprQueueIterType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx,
                                                        CONSTP2VAR(Com_GwSigGrpMappingEndIdxOfGwInfoType, AUTOMATIC, AUTOMATIC) idxGwSigGrpMapping)
{
  /* Read signal group index from queue */
  Com_CprQueueSigGw_CopySignalGroupId(routingQueueIndex, readIdx, idxGwSigGrpMapping);

  /* Increase the number of bytes read */
  Com_AddCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, Com_CprQueueSigGw_GetSignalGroupHeaderSize());
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_Read_GroupSignal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_Read_GroupSignal(Com_CCprQueueIterType routingQueueIndex, Com_GwGrpSigMappingIterType idxGwGrpSigMapping,
                                                        CONSTP2VAR(ComDataTypeVar, AUTOMATIC, AUTOMATIC) data)
{
  /* Read data from queue */
  Com_CprQueueSigGw_CopyGroupSignal(routingQueueIndex, idxGwGrpSigMapping, data);

  /* Increase the number of bytes read */
  Com_AddCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex, ((Com_CprQueueBufferNumberOfBytesReadIdxOfCprQueueType) data->dataLength));
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_Write_Signal
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueSigGw_Write_Signal(Com_CCprQueueIterType routingQueueIndex, Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping,
                                                              CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) data)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

    Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx = 0u;

  /* If data fits into the CrossPartitionRoutingQueue */
  if(Com_CprQueue_GetWriteIdx(routingQueueIndex, &writeIdx, data->dataLength + Com_CprQueueSigGw_GetSignalHeaderSize()) == E_OK)
  {
    /* Write data of signal routing to queue */
    Com_CprQueue_Write(writeIdx, (uint32) idxGwSigMapping, data, COM_CPRQUEUE_HEADERVALUE_SIGNAL, sizeof(Com_GwSigMappingEndIdxOfGwInfoType));
    /* Update the WriteIdx */
    Com_CprQueue_UpdateWriteIdx(routingQueueIndex);
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_Write_SignalGroup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueSigGw_Write_SignalGroup(Com_CCprQueueIterType routingQueueIndex, Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping,
                                                                   Com_SignalGroupLengthInByteOfGwSigGrpMappingType length)
{
  Std_ReturnType retVal = E_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  Com_CprQueueBufferWriteIdxOfCprQueueType writeIdx = 0u;

  /* If signal group gateway mapping fits into the CrossPartitionRoutingQueue */
  if(Com_CprQueue_GetWriteIdx(routingQueueIndex, &writeIdx, (PduLengthType) length + Com_CprQueueSigGw_GetSignalGroupHeaderSize()) == E_OK)
  {
    /* Copy index of signal group gateway to queue */
    Com_CprQueue_WriteHeader(&writeIdx, (uint32) idxGwSigGrpMapping, COM_CPRQUEUE_HEADERVALUE_SIGNALGROUP, sizeof(Com_GwSigGrpMappingEndIdxOfGwInfoType));

    /* Iterate over all related group signal mappings */
    for(Com_GwGrpSigMappingEndIdxOfGwSigGrpMappingType idxGwGrpSigMapping = Com_GetGwGrpSigMappingStartIdxOfGwSigGrpMapping(idxGwSigGrpMapping);
        idxGwGrpSigMapping < Com_GetGwGrpSigMappingEndIdxOfGwSigGrpMapping(idxGwSigGrpMapping); idxGwGrpSigMapping++)  /* FETA_COM_01 */
    {
      const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoIdxOfGwGrpSigMapping(idxGwGrpSigMapping);
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = Com_RxSigBuffer_GetGatewaySourcePtr(idxRxAccessInfo);
      const PduLengthType constDataLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);
      const ComDataTypeConst data = { constDataPtr, constDataLength };

      /* Copy the data of the group signal to queue */
      Com_CprQueue_WriteData(&writeIdx, &data);
    }

    /* Update the WriteIdx */
    Com_CprQueue_UpdateWriteIdx(routingQueueIndex);
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_UpdateReadIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_UpdateReadIdx(Com_CCprQueueIterType routingQueueIndex)
{
  Com_CprQueue_UpdateReadIdx(routingQueueIndex);
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_GetReadIdx
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, COM_CODE) Com_CprQueueSigGw_GetReadIdx(Com_CCprQueueIterType routingQueueIndex, CONSTP2VAR(Com_CprQueueBufferReadIdxOfCprQueueType, AUTOMATIC, AUTOMATIC) readIdx)
{
  return Com_CprQueue_GetReadIdx(routingQueueIndex, readIdx);
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_ReadFromCprQueueDirectly
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_CprQueueSigGw_ReadFromCprQueueDirectly(Com_CprQueueBufferIterType readIdx)
{
  return Com_CprQueue_ReadFromCprQueueDirectly(readIdx);
}

/**********************************************************************************************************************
 * Com_CprQueueSigGw_ResetNumberOfBytesRead
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CprQueueSigGw_ResetNumberOfBytesRead(Com_CCprQueueIterType routingQueueIndex)
{
  Com_CprQueue_ResetNumberOfBytesRead(routingQueueIndex);
}
#endif


#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:

*/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

SBSW_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

END_COVERAGE_JUSTIFICATION */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: Com_CprQueueSigGw.c
 *********************************************************************************************************************/
