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
/*!        \file  Com_RxSigBufferHelper.c
 *         \unit  RxSigBufferHelper
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_RXSIGBUFFERHELPER_SOURCE

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
#include "Com_RxSigBufferHelper.h"
#include "Com_Cfg_RxSignalBufferData.h"
#include "Com_Cfg_ReceptionProcessingData.h"

#include "Com_Deserializer.h"

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
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxSigBufferHelper_CopyToSigBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(void, COM_CODE) Com_RxSigBufferHelper_CopyToSigBuffer(Com_RxAccessInfoIterType idxRxAccessInfo, Com_RxBufferInfoIterType idxRxBufferInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  if (comDataPtr->dataPtr != NULL_PTR)
  {
    switch (Com_GetRxSigBufferOfRxBufferInfo(idxRxBufferInfo))
    {
      case COM_RXSIGBUFFERUINT8_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferUInt8IterType bufferIdx   = (Com_RxSigBufferUInt8IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt8Type     tmpNewValue = *((P2CONST(Com_RxSigBufferUInt8Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        Com_SetRxSigBufferUInt8(bufferIdx, tmpNewValue);
        break;
      }
      case COM_RXSIGBUFFERSINT8_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferSInt8IterType bufferIdx   = (Com_RxSigBufferSInt8IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt8Type     tmpNewValue = *((P2CONST(Com_RxSigBufferSInt8Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        Com_SetRxSigBufferSInt8(bufferIdx, tmpNewValue);
        break;
      }
      case COM_RXSIGBUFFERUINT16_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferUInt16IterType bufferIdx   = (Com_RxSigBufferUInt16IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt16Type     tmpNewValue = *((P2CONST(Com_RxSigBufferUInt16Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        Com_SetRxSigBufferUInt16(bufferIdx, tmpNewValue);
        break;
      }
      case COM_RXSIGBUFFERSINT16_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferSInt16IterType bufferIdx   = (Com_RxSigBufferSInt16IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt16Type     tmpNewValue = *((P2CONST(Com_RxSigBufferSInt16Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        Com_SetRxSigBufferSInt16(bufferIdx, tmpNewValue);
        break;
      }
      case COM_RXSIGBUFFERUINT32_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferUInt32IterType bufferIdx   = (Com_RxSigBufferUInt32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt32Type     tmpNewValue = *((P2CONST(Com_RxSigBufferUInt32Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        Com_SetRxSigBufferUInt32(bufferIdx, tmpNewValue);
        break;
      }
      case COM_RXSIGBUFFERSINT32_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferSInt32IterType bufferIdx   = (Com_RxSigBufferSInt32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt32Type     tmpNewValue = *((P2CONST(Com_RxSigBufferSInt32Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        Com_SetRxSigBufferSInt32(bufferIdx, tmpNewValue);
        break;
      }
      case COM_RXSIGBUFFERUINT64_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferUInt64IterType bufferIdx   = (Com_RxSigBufferUInt64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt64Type     tmpNewValue = *((P2CONST(Com_RxSigBufferUInt64Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        Com_SetRxSigBufferUInt64(bufferIdx, tmpNewValue);
        break;
      }
      case COM_RXSIGBUFFERSINT64_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferSInt64IterType bufferIdx   = (Com_RxSigBufferSInt64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt64Type     tmpNewValue = *((P2CONST(Com_RxSigBufferSInt64Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        Com_SetRxSigBufferSInt64(bufferIdx, tmpNewValue);
        break;
      }
      case COM_RXSIGBUFFERFLOAT32_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferFloat32IterType bufferIdx   = (Com_RxSigBufferFloat32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferFloat32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferFloat32Type     tmpNewValue = *((P2CONST(Com_RxSigBufferFloat32Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        Com_SetRxSigBufferFloat32(bufferIdx, tmpNewValue);
        break;
      }
      case COM_RXSIGBUFFERFLOAT64_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferFloat64IterType bufferIdx   = (Com_RxSigBufferFloat64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferFloat64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferFloat64Type     tmpNewValue = *((P2CONST(Com_RxSigBufferFloat64Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        Com_SetRxSigBufferFloat64(bufferIdx, tmpNewValue);
        break;
      }
      case COM_RXSIGBUFFERARRAYBASED_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferArrayBasedIterType bufferIdx = (Com_RxSigBufferArrayBasedIterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferArrayBased(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        switch (Com_GetApplTypeOfRxAccessInfo(idxRxAccessInfo))
        {
          case COM_UINT8_N_APPLTYPEOFRXACCESSINFO:
          {
            VStdLib_MemCpy(Com_GetAddrRxSigBufferArrayBased(bufferIdx), comDataPtr->dataPtr, Com_GetRxSigBufferLengthOfRxBufferInfo(idxRxBufferInfo)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
            break;
          }
          case COM_UINT8_DYN_APPLTYPEOFRXACCESSINFO:
          {
            VStdLib_MemCpy_s(Com_GetAddrRxSigBufferArrayBased(bufferIdx), Com_GetRxSigBufferLengthOfRxBufferInfo(idxRxBufferInfo), comDataPtr->dataPtr, comDataPtr->dataLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
            break;
          }
          default: /* COV_COM_MISRA */
            /* Intentionally Empty */
            break;
        }
        break;
      }
      default: /* COV_COM_MISRA */
        /* Intentionally Empty */
        break;
    }
  }
}

/**********************************************************************************************************************
  Com_RxSigBufferHelper_GetSigBufferPtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(P2VAR(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBufferHelper_GetSigBufferPtr(Com_RxBufferInfoIterType idxRxBufferInfo)
{
  P2VAR(void, AUTOMATIC, AUTOMATIC) sigBufferPtr = NULL_PTR;
  switch (Com_GetRxSigBufferOfRxBufferInfo(idxRxBufferInfo))
  {
    case COM_RXSIGBUFFERUINT8_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_RxSigBufferUInt8IterType bufferIdx = (Com_RxSigBufferUInt8IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      sigBufferPtr                                 = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrRxSigBufferUInt8(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_RXSIGBUFFERSINT8_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_RxSigBufferSInt8IterType bufferIdx = (Com_RxSigBufferSInt8IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      sigBufferPtr                                 = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrRxSigBufferSInt8(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_RXSIGBUFFERUINT16_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_RxSigBufferUInt16IterType bufferIdx = (Com_RxSigBufferUInt16IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      sigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrRxSigBufferUInt16(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_RXSIGBUFFERSINT16_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_RxSigBufferSInt16IterType bufferIdx = (Com_RxSigBufferSInt16IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      sigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrRxSigBufferSInt16(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_RXSIGBUFFERUINT32_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_RxSigBufferUInt32IterType bufferIdx = (Com_RxSigBufferUInt32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      sigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrRxSigBufferUInt32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_RXSIGBUFFERSINT32_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_RxSigBufferSInt32IterType bufferIdx = (Com_RxSigBufferSInt32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      sigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrRxSigBufferSInt32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_RXSIGBUFFERUINT64_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_RxSigBufferUInt64IterType bufferIdx = (Com_RxSigBufferUInt64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      sigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrRxSigBufferUInt64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_RXSIGBUFFERSINT64_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_RxSigBufferSInt64IterType bufferIdx = (Com_RxSigBufferSInt64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      sigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrRxSigBufferSInt64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_RXSIGBUFFERFLOAT32_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_RxSigBufferFloat32IterType bufferIdx = (Com_RxSigBufferFloat32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferFloat32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      sigBufferPtr                                   = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrRxSigBufferFloat32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_RXSIGBUFFERFLOAT64_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_RxSigBufferFloat64IterType bufferIdx = (Com_RxSigBufferFloat64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferFloat64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      sigBufferPtr                                   = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrRxSigBufferFloat64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_RXSIGBUFFERARRAYBASED_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_RxSigBufferArrayBasedIterType bufferIdx = (Com_RxSigBufferArrayBasedIterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferArrayBased(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      sigBufferPtr                                      = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrRxSigBufferArrayBased(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    default: /* COV_COM_MISRA */
      /* Intentionally Empty */
      break;
  }

  return sigBufferPtr;
}

/**********************************************************************************************************************
  Com_RxSigBufferHelper_GetTmpSigBufferPtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(P2VAR(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBufferHelper_GetTmpSigBufferPtr(Com_RxBufferInfoIterType idxRxBufferInfo)
{
  P2VAR(void, AUTOMATIC, AUTOMATIC) tmpSigBufferPtr = NULL_PTR;
  switch (Com_GetTmpSigBufferOfRxBufferInfo(idxRxBufferInfo))
  {
    case COM_TMPRXSIGBUFFERUINT8_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferUInt8IterType bufferIdx = (Com_TmpRxSigBufferUInt8IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferUInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      tmpSigBufferPtr                                 = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferUInt8(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERSINT8_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferSInt8IterType bufferIdx = (Com_TmpRxSigBufferSInt8IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferSInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      tmpSigBufferPtr                                 = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferSInt8(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERUINT16_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferUInt16IterType bufferIdx = (Com_TmpRxSigBufferUInt16IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferUInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      tmpSigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferUInt16(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERSINT16_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferSInt16IterType bufferIdx = (Com_TmpRxSigBufferSInt16IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferSInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      tmpSigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferSInt16(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERUINT32_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferUInt32IterType bufferIdx = (Com_TmpRxSigBufferUInt32IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferUInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      tmpSigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferUInt32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERSINT32_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferSInt32IterType bufferIdx = (Com_TmpRxSigBufferSInt32IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferSInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      tmpSigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferSInt32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERUINT64_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferUInt64IterType bufferIdx = (Com_TmpRxSigBufferUInt64IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferUInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      tmpSigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferUInt64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERSINT64_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferSInt64IterType bufferIdx = (Com_TmpRxSigBufferSInt64IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferSInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      tmpSigBufferPtr                                  = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferSInt64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERFLOAT32_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferFloat32IterType bufferIdx = (Com_TmpRxSigBufferFloat32IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferFloat32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      tmpSigBufferPtr                                   = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferFloat32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERFLOAT64_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferFloat64IterType bufferIdx = (Com_TmpRxSigBufferFloat64IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferFloat64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      tmpSigBufferPtr                                   = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferFloat64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERARRAYBASED_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferArrayBasedIterType bufferIdx = (Com_TmpRxSigBufferArrayBasedIterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferArrayBased(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      tmpSigBufferPtr                                      = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferArrayBased(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    default: /* case COM_NO_TMPSIGBUFFEROFRXBUFFERINFO */ /* COV_COM_MISRA */
      /* Intentionally Empty */
      break;
  }

  return tmpSigBufferPtr;
}

/**********************************************************************************************************************
  Com_RxSigBufferHelper_ReadSigOrGrpSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(P2VAR(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBufferHelper_ReadSigOrGrpSignal(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_RxBufferInfoIterType idxRxBufferInfo, PduLengthType dynLength)
{
  P2VAR(void, AUTOMATIC, AUTOMATIC) tmpSigBufferPtr = NULL_PTR;
  switch (Com_GetTmpSigBufferOfRxBufferInfo(idxRxBufferInfo))
  {
    case COM_TMPRXSIGBUFFERUINT8_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferUInt8IterType bufferIdx   = (Com_TmpRxSigBufferUInt8IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferUInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      const uint8                           signalValue = Com_Deserializer_ReadSignal_UInt8(idxRxAccessInfo, PduInfoPtr->SduDataPtr);
      Com_SetTmpRxSigBufferUInt8(bufferIdx, signalValue);
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferUInt8(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERSINT8_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferSInt8IterType bufferIdx   = (Com_TmpRxSigBufferSInt8IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferSInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      const sint8                           signalValue = Com_Deserializer_ReadSignal_SInt8(idxRxAccessInfo, PduInfoPtr->SduDataPtr);
      Com_SetTmpRxSigBufferSInt8(bufferIdx, signalValue);
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferSInt8(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERUINT16_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferUInt16IterType bufferIdx   = (Com_TmpRxSigBufferUInt16IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferUInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      const uint16                           signalValue = Com_Deserializer_ReadSignal_UInt16(idxRxAccessInfo, PduInfoPtr->SduDataPtr);
      Com_SetTmpRxSigBufferUInt16(bufferIdx, signalValue);
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferUInt16(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERSINT16_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferSInt16IterType bufferIdx   = (Com_TmpRxSigBufferSInt16IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferSInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      const sint16                           signalValue = Com_Deserializer_ReadSignal_SInt16(idxRxAccessInfo, PduInfoPtr->SduDataPtr);
      Com_SetTmpRxSigBufferSInt16(bufferIdx, signalValue);
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferSInt16(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERUINT32_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferUInt32IterType bufferIdx   = (Com_TmpRxSigBufferUInt32IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferUInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      const uint32                           signalValue = Com_Deserializer_ReadSignal_UInt32(idxRxAccessInfo, PduInfoPtr->SduDataPtr);
      Com_SetTmpRxSigBufferUInt32(bufferIdx, signalValue);
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferUInt32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERSINT32_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferSInt32IterType bufferIdx   = (Com_TmpRxSigBufferSInt32IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferSInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      const sint32                           signalValue = Com_Deserializer_ReadSignal_SInt32(idxRxAccessInfo, PduInfoPtr->SduDataPtr);
      Com_SetTmpRxSigBufferSInt32(bufferIdx, signalValue);
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferSInt32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERUINT64_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferUInt64IterType bufferIdx   = (Com_TmpRxSigBufferUInt64IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferUInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      const uint64                           signalValue = Com_Deserializer_ReadSignal_UInt64(idxRxAccessInfo, PduInfoPtr->SduDataPtr);
      Com_SetTmpRxSigBufferUInt64(bufferIdx, signalValue);
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferUInt64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERSINT64_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferSInt64IterType bufferIdx   = (Com_TmpRxSigBufferSInt64IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferSInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      const sint64                           signalValue = Com_Deserializer_ReadSignal_SInt64(idxRxAccessInfo, PduInfoPtr->SduDataPtr);
      Com_SetTmpRxSigBufferSInt64(bufferIdx, signalValue);
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferSInt64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERFLOAT32_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferFloat32IterType bufferIdx   = (Com_TmpRxSigBufferFloat32IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferFloat32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      const float32                           signalValue = Com_Deserializer_ReadSignal_Float32(idxRxAccessInfo, PduInfoPtr->SduDataPtr);
      Com_SetTmpRxSigBufferFloat32(bufferIdx, signalValue);
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferFloat32(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERFLOAT64_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferFloat64IterType bufferIdx   = (Com_TmpRxSigBufferFloat64IterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferFloat64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      const float64                           signalValue = Com_Deserializer_ReadSignal_Float64(idxRxAccessInfo, PduInfoPtr->SduDataPtr);
      Com_SetTmpRxSigBufferFloat64(bufferIdx, signalValue);
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferFloat64(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_TMPRXSIGBUFFERARRAYBASED_TMPSIGBUFFEROFRXBUFFERINFO:
    {
      const Com_TmpRxSigBufferArrayBasedIterType bufferIdx = (Com_TmpRxSigBufferArrayBasedIterType) Com_GetTmpSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      /*@ assert bufferIdx < Com_GetSizeOfTmpRxSigBufferArrayBased(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
      switch (Com_GetApplTypeOfRxAccessInfo(idxRxAccessInfo))
      {
        case COM_UINT8_N_APPLTYPEOFRXACCESSINFO:
        {
          CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr = &(PduInfoPtr->SduDataPtr[Com_GetStartByteInPduPositionOfRxAccessInfo(idxRxAccessInfo)]);
          VStdLib_MemCpy(Com_GetAddrTmpRxSigBufferArrayBased(bufferIdx), SignalBusSourcePtr, Com_GetTmpSigBufferLengthOfRxBufferInfo(idxRxBufferInfo)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
          break;
        }
        case COM_UINT8_DYN_APPLTYPEOFRXACCESSINFO:
        {
          CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr = &(PduInfoPtr->SduDataPtr[Com_GetStartByteInPduPositionOfRxAccessInfo(idxRxAccessInfo)]);
          VStdLib_MemCpy_s(Com_GetAddrTmpRxSigBufferArrayBased(bufferIdx), Com_GetTmpSigBufferLengthOfRxBufferInfo(idxRxBufferInfo), SignalBusSourcePtr, dynLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
          break;
        }
        default: /* COV_COM_MISRA */
          break;
      }
      tmpSigBufferPtr = (P2VAR(void, AUTOMATIC, AUTOMATIC)) Com_GetAddrTmpRxSigBufferArrayBased(bufferIdx); /* PRQA S 0314 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    default: /* case COM_NO_TMPSIGBUFFEROFRXBUFFERINFO */ /* COV_COM_MISRA */
      /* Intentionally Empty */
      break;
  }

  return tmpSigBufferPtr;
}

/**********************************************************************************************************************
  Com_RxSigBufferHelper_SetRxInitValue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(void, COM_CODE) Com_RxSigBufferHelper_SetRxInitValue(Com_RxAccessInfoIterType rxAccessInfoIdx, Com_RxBufferInfoIterType idxBufferInfo)
{
  switch (Com_GetRxSigBufferOfRxBufferInfo(idxBufferInfo))
  {
    case COM_RXSIGBUFFERUINT8_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetInitValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESUINT8_INITVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferUInt8IterType          bufferIdx    = (Com_RxSigBufferUInt8IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesUInt8IterType initValueIdx = (Com_RxSigBufferSubValuesUInt8IterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert initValueIdx < Com_GetSizeOfRxSigBufferSubValuesUInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt8Type              tmpNewValue  = Com_GetRxSigBufferSubValuesUInt8(initValueIdx);
        Com_SetRxSigBufferUInt8(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERSINT8_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetInitValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESSINT8_INITVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferSInt8IterType          bufferIdx    = (Com_RxSigBufferSInt8IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesSInt8IterType initValueIdx = (Com_RxSigBufferSubValuesSInt8IterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert initValueIdx < Com_GetSizeOfRxSigBufferSubValuesSInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt8Type              tmpNewValue  = Com_GetRxSigBufferSubValuesSInt8(initValueIdx);
        Com_SetRxSigBufferSInt8(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERUINT16_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetInitValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESUINT16_INITVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferUInt16IterType          bufferIdx    = (Com_RxSigBufferUInt16IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesUInt16IterType initValueIdx = (Com_RxSigBufferSubValuesUInt16IterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert initValueIdx < Com_GetSizeOfRxSigBufferSubValuesUInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt16Type              tmpNewValue  = Com_GetRxSigBufferSubValuesUInt16(initValueIdx);
        Com_SetRxSigBufferUInt16(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERSINT16_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetInitValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESSINT16_INITVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferSInt16IterType          bufferIdx    = (Com_RxSigBufferSInt16IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesSInt16IterType initValueIdx = (Com_RxSigBufferSubValuesSInt16IterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert initValueIdx < Com_GetSizeOfRxSigBufferSubValuesSInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt16Type              tmpNewValue  = Com_GetRxSigBufferSubValuesSInt16(initValueIdx);
        Com_SetRxSigBufferSInt16(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERUINT32_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetInitValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESUINT32_INITVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferUInt32IterType          bufferIdx    = (Com_RxSigBufferUInt32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesUInt32IterType initValueIdx = (Com_RxSigBufferSubValuesUInt32IterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert initValueIdx < Com_GetSizeOfRxSigBufferSubValuesUInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt32Type              tmpNewValue  = Com_GetRxSigBufferSubValuesUInt32(initValueIdx);
        Com_SetRxSigBufferUInt32(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERSINT32_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetInitValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESSINT32_INITVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferSInt32IterType          bufferIdx    = (Com_RxSigBufferSInt32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesSInt32IterType initValueIdx = (Com_RxSigBufferSubValuesSInt32IterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert initValueIdx < Com_GetSizeOfRxSigBufferSubValuesSInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt32Type              tmpNewValue  = Com_GetRxSigBufferSubValuesSInt32(initValueIdx);
        Com_SetRxSigBufferSInt32(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERUINT64_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetInitValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESUINT64_INITVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferUInt64IterType          bufferIdx    = (Com_RxSigBufferUInt64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesUInt64IterType initValueIdx = (Com_RxSigBufferSubValuesUInt64IterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert initValueIdx < Com_GetSizeOfRxSigBufferSubValuesUInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt64Type              tmpNewValue  = Com_GetRxSigBufferSubValuesUInt64(initValueIdx);
        Com_SetRxSigBufferUInt64(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERSINT64_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetInitValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESSINT64_INITVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferSInt64IterType          bufferIdx    = (Com_RxSigBufferSInt64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesSInt64IterType initValueIdx = (Com_RxSigBufferSubValuesSInt64IterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert initValueIdx < Com_GetSizeOfRxSigBufferSubValuesSInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt64Type              tmpNewValue  = Com_GetRxSigBufferSubValuesSInt64(initValueIdx);
        Com_SetRxSigBufferSInt64(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERFLOAT32_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetInitValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESFLOAT32_INITVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferFloat32IterType          bufferIdx    = (Com_RxSigBufferFloat32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferFloat32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesFloat32IterType initValueIdx = (Com_RxSigBufferSubValuesFloat32IterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert initValueIdx < Com_GetSizeOfRxSigBufferSubValuesFloat32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferFloat32Type              tmpNewValue  = Com_GetRxSigBufferSubValuesFloat32(initValueIdx);
        Com_SetRxSigBufferFloat32(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERFLOAT64_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetInitValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESFLOAT64_INITVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferFloat64IterType          bufferIdx    = (Com_RxSigBufferFloat64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferFloat64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesFloat64IterType initValueIdx = (Com_RxSigBufferSubValuesFloat64IterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert initValueIdx < Com_GetSizeOfRxSigBufferSubValuesFloat64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferFloat64Type              tmpNewValue  = Com_GetRxSigBufferSubValuesFloat64(initValueIdx);
        Com_SetRxSigBufferFloat64(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERARRAYBASED_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const PduLengthType dynLength = (PduLengthType) Com_GetInitValueLengthOfRxBufferInfo(idxBufferInfo);
      if(Com_IsRxDynSignalLengthUsedOfRxAccessInfo(rxAccessInfoIdx))
      {
        Com_RxSigBufferHelper_SetRxDynSignalLength(rxAccessInfoIdx, dynLength);
      }
      if (Com_IsRxDynSignalTmpLengthForSignalGroupsUsedOfRxAccessInfo(rxAccessInfoIdx))
      {
        Com_RxSigBufferHelper_SetRxDynSignalTmpLengthForSignalGroups(rxAccessInfoIdx, dynLength);
      }
      if (Com_GetInitValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESARRAYBASED_INITVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferArrayBasedIterType          bufferIdx    = (Com_RxSigBufferArrayBasedIterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferArrayBased(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesArrayBasedIterType initValueIdx = (Com_RxSigBufferSubValuesArrayBasedIterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert initValueIdx < Com_GetSizeOfRxSigBufferSubValuesArrayBased(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesArrayBasedPtrType  initValuePtr = Com_GetAddrRxSigBufferSubValuesArrayBased(initValueIdx);
        switch (Com_GetApplTypeOfRxAccessInfo(rxAccessInfoIdx))
        {
          case COM_UINT8_N_APPLTYPEOFRXACCESSINFO:
          {
            VStdLib_MemCpy(Com_GetAddrRxSigBufferArrayBased(bufferIdx), initValuePtr, Com_GetRxSigBufferLengthOfRxBufferInfo(idxBufferInfo)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
            break;
          }
          case COM_UINT8_DYN_APPLTYPEOFRXACCESSINFO:
          {
            VStdLib_MemCpy_s(Com_GetAddrRxSigBufferArrayBased(bufferIdx), Com_GetRxSigBufferLengthOfRxBufferInfo(idxBufferInfo), initValuePtr, dynLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
            break;
          }
          default: /* COV_COM_MISRA */
            /* Intentionally Empty */
            break;
        }
      }
      break;
    }
    default: /* COV_COM_MISRA */
      /* Intentionally Empty */
      break;
  }
}

/**********************************************************************************************************************
  Com_RxSigBufferHelper_SetRxDataTimeoutSubValue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(void, COM_CODE) Com_RxSigBufferHelper_SetRxDataTimeoutSubValue(Com_RxAccessInfoIterType rxAccessInfoIdx, Com_RxBufferInfoIterType idxBufferInfo)
{
  switch (Com_GetRxSigBufferOfRxBufferInfo(idxBufferInfo))
  {
    case COM_RXSIGBUFFERUINT8_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetRxDataTimeoutSubstitutionValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESUINT8_RXDATATIMEOUTSUBSTITUTIONVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferUInt8IterType          bufferIdx            = (Com_RxSigBufferUInt8IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesUInt8IterType rxTimeoutSubValueIdx = (Com_RxSigBufferSubValuesUInt8IterType) Com_GetRxDataTimeoutSubstitutionValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert rxTimeoutSubValueIdx < Com_GetSizeOfRxSigBufferSubValuesUInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt8Type              tmpNewValue          = Com_GetRxSigBufferSubValuesUInt8(rxTimeoutSubValueIdx);
        Com_SetRxSigBufferUInt8(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERSINT8_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetRxDataTimeoutSubstitutionValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESSINT8_RXDATATIMEOUTSUBSTITUTIONVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferSInt8IterType          bufferIdx            = (Com_RxSigBufferSInt8IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesSInt8IterType rxTimeoutSubValueIdx = (Com_RxSigBufferSubValuesSInt8IterType) Com_GetRxDataTimeoutSubstitutionValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert rxTimeoutSubValueIdx < Com_GetSizeOfRxSigBufferSubValuesSInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt8Type              tmpNewValue          = Com_GetRxSigBufferSubValuesSInt8(rxTimeoutSubValueIdx);
        Com_SetRxSigBufferSInt8(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERUINT16_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetRxDataTimeoutSubstitutionValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESUINT16_RXDATATIMEOUTSUBSTITUTIONVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferUInt16IterType          bufferIdx            = (Com_RxSigBufferUInt16IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesUInt16IterType rxTimeoutSubValueIdx = (Com_RxSigBufferSubValuesUInt16IterType) Com_GetRxDataTimeoutSubstitutionValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert rxTimeoutSubValueIdx < Com_GetSizeOfRxSigBufferSubValuesUInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt16Type              tmpNewValue          = Com_GetRxSigBufferSubValuesUInt16(rxTimeoutSubValueIdx);
        Com_SetRxSigBufferUInt16(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERSINT16_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetRxDataTimeoutSubstitutionValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESSINT16_RXDATATIMEOUTSUBSTITUTIONVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferSInt16IterType          bufferIdx            = (Com_RxSigBufferSInt16IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesSInt16IterType rxTimeoutSubValueIdx = (Com_RxSigBufferSubValuesSInt16IterType) Com_GetRxDataTimeoutSubstitutionValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert rxTimeoutSubValueIdx < Com_GetSizeOfRxSigBufferSubValuesSInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt16Type              tmpNewValue          = Com_GetRxSigBufferSubValuesSInt16(rxTimeoutSubValueIdx);
        Com_SetRxSigBufferSInt16(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERUINT32_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetRxDataTimeoutSubstitutionValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESUINT32_RXDATATIMEOUTSUBSTITUTIONVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferUInt32IterType          bufferIdx            = (Com_RxSigBufferUInt32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesUInt32IterType rxTimeoutSubValueIdx = (Com_RxSigBufferSubValuesUInt32IterType) Com_GetRxDataTimeoutSubstitutionValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert rxTimeoutSubValueIdx < Com_GetSizeOfRxSigBufferSubValuesUInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt32Type              tmpNewValue          = Com_GetRxSigBufferSubValuesUInt32(rxTimeoutSubValueIdx);
        Com_SetRxSigBufferUInt32(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERSINT32_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetRxDataTimeoutSubstitutionValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESSINT32_RXDATATIMEOUTSUBSTITUTIONVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferSInt32IterType          bufferIdx            = (Com_RxSigBufferSInt32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesSInt32IterType rxTimeoutSubValueIdx = (Com_RxSigBufferSubValuesSInt32IterType) Com_GetRxDataTimeoutSubstitutionValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert rxTimeoutSubValueIdx < Com_GetSizeOfRxSigBufferSubValuesSInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt32Type              tmpNewValue          = Com_GetRxSigBufferSubValuesSInt32(rxTimeoutSubValueIdx);
        Com_SetRxSigBufferSInt32(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERUINT64_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetRxDataTimeoutSubstitutionValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESUINT64_RXDATATIMEOUTSUBSTITUTIONVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferUInt64IterType          bufferIdx            = (Com_RxSigBufferUInt64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesUInt64IterType rxTimeoutSubValueIdx = (Com_RxSigBufferSubValuesUInt64IterType) Com_GetRxDataTimeoutSubstitutionValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert rxTimeoutSubValueIdx < Com_GetSizeOfRxSigBufferSubValuesUInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferUInt64Type              tmpNewValue          = Com_GetRxSigBufferSubValuesUInt64(rxTimeoutSubValueIdx);
        Com_SetRxSigBufferUInt64(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERSINT64_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetRxDataTimeoutSubstitutionValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESSINT64_RXDATATIMEOUTSUBSTITUTIONVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferSInt64IterType          bufferIdx            = (Com_RxSigBufferSInt64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesSInt64IterType rxTimeoutSubValueIdx = (Com_RxSigBufferSubValuesSInt64IterType) Com_GetRxDataTimeoutSubstitutionValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert rxTimeoutSubValueIdx < Com_GetSizeOfRxSigBufferSubValuesSInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSInt64Type              tmpNewValue          = Com_GetRxSigBufferSubValuesSInt64(rxTimeoutSubValueIdx);
        Com_SetRxSigBufferSInt64(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERFLOAT32_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetRxDataTimeoutSubstitutionValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESFLOAT32_RXDATATIMEOUTSUBSTITUTIONVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferFloat32IterType          bufferIdx            = (Com_RxSigBufferFloat32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferFloat32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesFloat32IterType rxTimeoutSubValueIdx = (Com_RxSigBufferSubValuesFloat32IterType) Com_GetRxDataTimeoutSubstitutionValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert rxTimeoutSubValueIdx < Com_GetSizeOfRxSigBufferSubValuesFloat32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferFloat32Type              tmpNewValue          = Com_GetRxSigBufferSubValuesFloat32(rxTimeoutSubValueIdx);
        Com_SetRxSigBufferFloat32(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERFLOAT64_RXSIGBUFFEROFRXBUFFERINFO:
    {
      if (Com_GetRxDataTimeoutSubstitutionValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESFLOAT64_RXDATATIMEOUTSUBSTITUTIONVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferFloat64IterType          bufferIdx            = (Com_RxSigBufferFloat64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferFloat64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesFloat64IterType rxTimeoutSubValueIdx = (Com_RxSigBufferSubValuesFloat64IterType) Com_GetRxDataTimeoutSubstitutionValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert rxTimeoutSubValueIdx < Com_GetSizeOfRxSigBufferSubValuesFloat64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferFloat64Type              tmpNewValue          = Com_GetRxSigBufferSubValuesFloat64(rxTimeoutSubValueIdx);
        Com_SetRxSigBufferFloat64(bufferIdx, tmpNewValue);
      }
      break;
    }
    case COM_RXSIGBUFFERARRAYBASED_RXSIGBUFFEROFRXBUFFERINFO:
    {
      const PduLengthType dynLength = (PduLengthType) Com_GetRxDataTimeoutSubstitutionValueLengthOfRxBufferInfo(idxBufferInfo);
      if(Com_IsRxDynSignalLengthUsedOfRxAccessInfo(rxAccessInfoIdx))
      {
        Com_RxSigBufferHelper_SetRxDynSignalLength(rxAccessInfoIdx, dynLength);
      }
      if (Com_IsRxDynSignalTmpLengthForSignalGroupsUsedOfRxAccessInfo(rxAccessInfoIdx))
      {
        Com_RxSigBufferHelper_SetRxDynSignalTmpLengthForSignalGroups(rxAccessInfoIdx, dynLength);
      }
      if (Com_GetRxDataTimeoutSubstitutionValueOfRxBufferInfo(idxBufferInfo) == COM_RXSIGBUFFERSUBVALUESARRAYBASED_RXDATATIMEOUTSUBSTITUTIONVALUEOFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
      {
        const Com_RxSigBufferArrayBasedIterType          bufferIdx            = (Com_RxSigBufferArrayBasedIterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferArrayBased(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesArrayBasedIterType rxTimeoutSubValueIdx = (Com_RxSigBufferSubValuesArrayBasedIterType) Com_GetRxDataTimeoutSubstitutionValueStartIdxOfRxBufferInfo(idxBufferInfo);
        /*@ assert rxTimeoutSubValueIdx < Com_GetSizeOfRxSigBufferSubValuesArrayBased(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        const Com_RxSigBufferSubValuesArrayBasedPtrType  rxTimeoutSubValuePtr = Com_GetAddrRxSigBufferSubValuesArrayBased(rxTimeoutSubValueIdx);
        switch (Com_GetApplTypeOfRxAccessInfo(rxAccessInfoIdx))
        {
          case COM_UINT8_N_APPLTYPEOFRXACCESSINFO:
          {
            VStdLib_MemCpy(Com_GetAddrRxSigBufferArrayBased(bufferIdx), rxTimeoutSubValuePtr, Com_GetRxSigBufferLengthOfRxBufferInfo(idxBufferInfo)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
            break;
          }
          case COM_UINT8_DYN_APPLTYPEOFRXACCESSINFO:
          {
            VStdLib_MemCpy_s(Com_GetAddrRxSigBufferArrayBased(bufferIdx), Com_GetRxSigBufferLengthOfRxBufferInfo(idxBufferInfo), rxTimeoutSubValuePtr, dynLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
            break;
          }
          default: /* COV_COM_MISRA */
            /* Intentionally Empty */
            break;
        }
      }
      break;
    }
    default: /* COV_COM_MISRA */
      /* Intentionally Empty */
      break;
  }
}

/**********************************************************************************************************************
  Com_RxSigBufferHelper_GetRxDynSignalLength
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_RxSigBufferHelper_GetRxDynSignalLength(Com_RxAccessInfoIterType idxRxAccessInfo)
{
  Com_RxDynSignalLengthIdxOfRxAccessInfoType idx = Com_GetRxDynSignalLengthIdxOfRxAccessInfo(idxRxAccessInfo);
  /*@ assert idx < Com_GetSizeOfRxDynSignalLength(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
  return Com_GetRxDynSignalLength(idx);
}

/**********************************************************************************************************************
  Com_RxSigBufferHelper_SetRxDynSignalLength
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBufferHelper_SetRxDynSignalLength(Com_RxAccessInfoIterType idxRxAccessInfo, PduLengthType dynLength)
{
  Com_RxDynSignalLengthIdxOfRxAccessInfoType idx = Com_GetRxDynSignalLengthIdxOfRxAccessInfo(idxRxAccessInfo);
  /*@ assert idx < Com_GetSizeOfRxDynSignalLength(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
  Com_SetRxDynSignalLength(idx, dynLength);
}

/**********************************************************************************************************************
  Com_RxSigBufferHelper_GetRxDynSignalTmpLengthForSignalGroups
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_RxSigBufferHelper_GetRxDynSignalTmpLengthForSignalGroups(Com_RxAccessInfoIterType idxRxAccessInfo)
{
  Com_RxDynSignalTmpLengthForSignalGroupsIdxOfRxAccessInfoType idx = Com_GetRxDynSignalTmpLengthForSignalGroupsIdxOfRxAccessInfo(idxRxAccessInfo);
  /*@ assert idx < Com_GetSizeOfRxDynSignalTmpLengthForSignalGroups(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
  PduLengthType ret = Com_GetRxDynSignalTmpLengthForSignalGroups(idx);
  return ret;
}

/**********************************************************************************************************************
  Com_RxSigBufferHelper_SetRxDynSignalTmpLengthForSignalGroups
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBufferHelper_SetRxDynSignalTmpLengthForSignalGroups(Com_RxAccessInfoIterType idxRxAccessInfo, PduLengthType dynLength)
{
  Com_RxDynSignalTmpLengthForSignalGroupsIdxOfRxAccessInfoType idx = Com_GetRxDynSignalTmpLengthForSignalGroupsIdxOfRxAccessInfo(idxRxAccessInfo);
  /*@ assert idx < Com_GetSizeOfRxDynSignalTmpLengthForSignalGroups(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
  Com_SetRxDynSignalTmpLengthForSignalGroups(idx, dynLength);
}

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

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
 *  END OF FILE: Com_RxSigBufferHelper.c
 *********************************************************************************************************************/
