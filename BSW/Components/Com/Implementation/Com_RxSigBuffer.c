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
/*!        \file  Com_RxSigBuffer.c
 *         \unit  RxSigBuffer
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_RXSIGBUFFER_SOURCE

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
#include "Com_RxSigBuffer.h"
#include "Com_Cfg_RxSignalBufferData.h"
#include "Com_Cfg_ReceptionProcessingData.h"

#include "Com_RxSigBufferHelper.h"

#include "vstdlib.h"
#include "SchM_Com.h"

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
  Com_RxSigBuffer_CopyToRxSigArrayAccessSigGrpBuffer
**********************************************************************************************************************/
/*! \brief        This function copies the value of the passed SignalSourcePtr to the signal buffer.
    \details      -
    \pre          -
    \param[in]    idxRxSigGrpInfo   Handle Id of ComSignalGroup
    \param[in]    SignalSourcePtr   Pointer to the memory where the signal group value is located
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxSigBuffer_CopyToRxSigArrayAccessSigGrpBuffer(Com_RxSigGrpInfoIterType idxRxSigGrpInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalSourcePtr);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyToRxSigArrayAccessSigGrpBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxSigBuffer_CopyToRxSigArrayAccessSigGrpBuffer(Com_RxSigGrpInfoIterType idxRxSigGrpInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalSourcePtr)
{
  if (Com_IsRxBufferInfoUsedOfRxSigGrpInfo(idxRxSigGrpInfo)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    const Com_RxBufferInfoIterType rxBufferInfoIdx = (Com_RxBufferInfoIterType) Com_GetRxBufferInfoIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
    if (Com_GetRxSigBufferOfRxBufferInfo(rxBufferInfoIdx) == COM_RXSIGARRAYACCESSSIGGRPBUFFER_RXSIGBUFFEROFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
    {
      const Com_RxSigArrayAccessSigGrpBufferIterType bufferIdx = (Com_RxSigArrayAccessSigGrpBufferIterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(rxBufferInfoIdx);
      VStdLib_MemCpy(Com_GetAddrRxSigArrayAccessSigGrpBuffer(bufferIdx), SignalSourcePtr, Com_GetRxSigBufferLengthOfRxBufferInfo(rxBufferInfoIdx)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
    }
  }
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxSigBuffer_InitRxSignalBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_InitRxSignalBuffer(Com_RxAccessInfoIterType rxAccessInfoIdx)
{
  if (Com_IsRxBufferInfoShdwBufferUsedOfRxAccessInfo(rxAccessInfoIdx))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoShdwBufferIdxOfRxAccessInfo(rxAccessInfoIdx);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_RxSigBufferHelper_SetRxInitValue(rxAccessInfoIdx, idxRxBufferInfo);
  }
  if (Com_IsRxBufferInfoUsedOfRxAccessInfo(rxAccessInfoIdx))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxAccessInfo(rxAccessInfoIdx);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_RxSigBufferHelper_SetRxInitValue(rxAccessInfoIdx, idxRxBufferInfo);
  }

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
  if (Com_IsRxBufferInfoMinProcGwBufferUsedOfRxAccessInfo(rxAccessInfoIdx))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoMinProcGwBufferIdxOfRxAccessInfo(rxAccessInfoIdx);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_RxSigBufferHelper_SetRxInitValue(rxAccessInfoIdx, idxRxBufferInfo);
  }
#endif
}

/**********************************************************************************************************************
  Com_RxSigBuffer_InitRxSignalGroupBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_InitRxSignalGroupBuffer(Com_RxSigGrpInfoIterType idxRxSigGrpInfo)
{
  if (Com_IsRxBufferInfoUsedOfRxSigGrpInfo(idxRxSigGrpInfo))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
    if ((Com_GetRxSigBufferOfRxBufferInfo(idxRxBufferInfo) == COM_RXSIGARRAYACCESSSIGGRPBUFFER_RXSIGBUFFEROFRXBUFFERINFO) && (Com_GetInitValueOfRxBufferInfo(idxRxBufferInfo) == COM_RXSIGBUFFERSUBVALUESSIGGRPARRAYACCESS_INITVALUEOFRXBUFFERINFO)) /* COV_COM_CSL04_CORRECTARGET_CHECK */
    {
      const Com_RxSigArrayAccessSigGrpBufferIterType          bufferIdx    = (Com_RxSigArrayAccessSigGrpBufferIterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
      const Com_RxSigBufferSubValuesSigGrpArrayAccessIterType initValueIdx = (Com_RxSigBufferSubValuesSigGrpArrayAccessIterType) Com_GetInitValueStartIdxOfRxBufferInfo(idxRxBufferInfo);
      const Com_RxSigBufferSubValuesSigGrpArrayAccessPtrType  initValuePtr = Com_GetAddrRxSigBufferSubValuesSigGrpArrayAccess(initValueIdx);
      VStdLib_MemCpy_s(Com_GetAddrRxSigArrayAccessSigGrpBuffer(bufferIdx), Com_GetRxSigBufferLengthOfRxBufferInfo(idxRxBufferInfo), initValuePtr, Com_GetInitValueLengthOfRxBufferInfo(idxRxBufferInfo)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_MEMCPY_CSL_DEST */
    }
  }
}

/**********************************************************************************************************************
  Com_RxSigBuffer_ReadSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBuffer_ReadSignal(CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_RxAccessInfoIterType idxRxAccessInfo)
{
  P2CONST(void, AUTOMATIC, AUTOMATIC) newValuePtr = NULL_PTR;

  if (PduInfoPtr->SduDataPtr != NULL_PTR)
  {
    if (Com_IsRxBufferInfoUsedOfRxAccessInfo(idxRxAccessInfo))
    {
      const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxAccessInfo(idxRxAccessInfo);
      /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */

      /* Extract the ComSignal data from the passed Sdu data and copy it appl type specific to the related tmp sig buffer*/
      const PduLengthType dynLength = Com_RxSigBuffer_GetRxDynamicLength(PduInfoPtr->SduLength, idxRxAccessInfo);
      newValuePtr = Com_RxSigBufferHelper_ReadSigOrGrpSignal(idxRxAccessInfo, PduInfoPtr, idxRxBufferInfo, dynLength);

      /* If it is a dynamic length signal, update the dynamic length value of the signal */
      if(Com_IsRxDynSignalLengthUsedOfRxAccessInfo(idxRxAccessInfo))
      {
        Com_RxSigBufferHelper_SetRxDynSignalLength(idxRxAccessInfo, dynLength);
      }
    }
  }
  return newValuePtr;
}

/**********************************************************************************************************************
  Com_RxSigBuffer_ReadSignalGroup
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_ReadSignalGroup(CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_RxSigGrpInfoIterType idxRxSigGrpInfo)
{
  if (PduInfoPtr->SduDataPtr != NULL_PTR)
  {
    /* If ComSignalGroup Array Access is used, copy the passed ComSignalGroup data to the signal buffer */
    if (Com_IsArrayAccessUsedOfRxSigGrpInfo(idxRxSigGrpInfo))
    {
      CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalSourcePtr = &(PduInfoPtr->SduDataPtr[Com_GetStartBytePositionOfRxSigGrpInfo(idxRxSigGrpInfo)]);
      Com_RxSigBuffer_CopyToRxSigArrayAccessSigGrpBuffer(idxRxSigGrpInfo, SignalSourcePtr);
    }

    /* If shadow buffers are required (normal signal group or gateway array access routing), extract all group signals to their shadow buffers */
    if (Com_IsShdBufferRequiredOfRxSigGrpInfo(idxRxSigGrpInfo))
    {
      for (Com_RxAccessInfoGrpSigIndIterType rxAccessInfoIndIdx = Com_GetRxAccessInfoGrpSigIndStartIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
           rxAccessInfoIndIdx < Com_GetRxAccessInfoGrpSigIndEndIdxOfRxSigGrpInfo(idxRxSigGrpInfo); rxAccessInfoIndIdx++) /* FETA_COM_01 */
      {
        const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoGrpSigInd(rxAccessInfoIndIdx);
        if (Com_IsRxBufferInfoUsedOfRxAccessInfo(idxRxAccessInfo))
        {
          const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxAccessInfo(idxRxAccessInfo);
          /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
          const PduLengthType dynLength = Com_RxSigBuffer_GetRxDynamicLength(PduInfoPtr->SduLength, idxRxAccessInfo);
          (void) Com_RxSigBufferHelper_ReadSigOrGrpSignal(idxRxAccessInfo, PduInfoPtr, idxRxBufferInfo, dynLength);
        }
      }
    }
  }
}

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyFromRxSigBufferToSignalDataPtr()
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyFromRxSigBufferToSignalDataPtr(Com_RxAccessInfoIterType idxRxAccessInfo, P2VAR(void, AUTOMATIC, AUTOMATIC) SignalDataPtr)
{
  if (Com_IsRxBufferInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    switch (Com_GetRxSigBufferOfRxBufferInfo(idxRxBufferInfo))
    {
      case COM_RXSIGBUFFERUINT8_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferUInt8IterType bufferIdx = (Com_RxSigBufferUInt8IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        *((P2VAR(uint8, AUTOMATIC, AUTOMATIC)) SignalDataPtr) = Com_GetRxSigBufferUInt8(bufferIdx); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        break;
      }
      case COM_RXSIGBUFFERSINT8_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferSInt8IterType bufferIdx = (Com_RxSigBufferSInt8IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt8(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        *((P2VAR(sint8, AUTOMATIC, AUTOMATIC)) SignalDataPtr) = Com_GetRxSigBufferSInt8(bufferIdx); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        break;
      }
      case COM_RXSIGBUFFERUINT16_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferUInt16IterType bufferIdx = (Com_RxSigBufferUInt16IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        *((P2VAR(uint16, AUTOMATIC, AUTOMATIC)) SignalDataPtr) = Com_GetRxSigBufferUInt16(bufferIdx); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        break;
      }
      case COM_RXSIGBUFFERSINT16_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferSInt16IterType bufferIdx = (Com_RxSigBufferSInt16IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt16(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        *((P2VAR(sint16, AUTOMATIC, AUTOMATIC)) SignalDataPtr) = Com_GetRxSigBufferSInt16(bufferIdx); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        break;
      }
      case COM_RXSIGBUFFERUINT32_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferUInt32IterType bufferIdx = (Com_RxSigBufferUInt32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
        *((P2VAR(uint32, AUTOMATIC, AUTOMATIC)) SignalDataPtr) = Com_GetRxSigBufferUInt32(bufferIdx); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
        break;
      }
      case COM_RXSIGBUFFERSINT32_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferSInt32IterType bufferIdx = (Com_RxSigBufferSInt32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
        *((P2VAR(sint32, AUTOMATIC, AUTOMATIC)) SignalDataPtr) = Com_GetRxSigBufferSInt32(bufferIdx); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
        break;
      }
      case COM_RXSIGBUFFERUINT64_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferUInt64IterType bufferIdx = (Com_RxSigBufferUInt64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferUInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
        *((P2VAR(uint64, AUTOMATIC, AUTOMATIC)) SignalDataPtr) = Com_GetRxSigBufferUInt64(bufferIdx); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
        break;
      }
      case COM_RXSIGBUFFERSINT64_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferSInt64IterType bufferIdx = (Com_RxSigBufferSInt64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferSInt64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
        *((P2VAR(sint64, AUTOMATIC, AUTOMATIC)) SignalDataPtr) = Com_GetRxSigBufferSInt64(bufferIdx); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
        break;
      }
      case COM_RXSIGBUFFERFLOAT32_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferFloat32IterType bufferIdx = (Com_RxSigBufferFloat32IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferFloat32(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
        *((P2VAR(float32, AUTOMATIC, AUTOMATIC)) SignalDataPtr) = Com_GetRxSigBufferFloat32(bufferIdx); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
        break;
      }
      case COM_RXSIGBUFFERFLOAT64_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferFloat64IterType bufferIdx = (Com_RxSigBufferFloat64IterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferFloat64(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
        *((P2VAR(float64, AUTOMATIC, AUTOMATIC)) SignalDataPtr) = Com_GetRxSigBufferFloat64(bufferIdx); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
        break;
      }
      case COM_RXSIGBUFFERARRAYBASED_RXSIGBUFFEROFRXBUFFERINFO:
      {
        const Com_RxSigBufferArrayBasedIterType bufferIdx = (Com_RxSigBufferArrayBasedIterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo);
        /*@ assert bufferIdx < Com_GetSizeOfRxSigBufferArrayBased(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
        SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
        VStdLib_MemCpy(SignalDataPtr, Com_GetAddrRxSigBufferArrayBased(bufferIdx), Com_GetRxSigBufferLengthOfRxBufferInfo(idxRxBufferInfo)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_SIGNAL_DATA_PTR */
        SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
        break;
      }
      default: /* COV_COM_MISRA */
        /* Intentionally Empty */
        break;
    }
  }
}

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyFromRxSigBufferToSignalDataPtrDynSig()
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyFromRxSigBufferToSignalDataPtrDynSig(Com_RxAccessInfoIterType idxRxAccessInfo, P2VAR(void, AUTOMATIC, AUTOMATIC) SignalDataPtr, PduLengthType dynSignalLength)
{
  if (Com_IsRxBufferInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    if (Com_GetRxSigBufferOfRxBufferInfo(idxRxBufferInfo) == COM_RXSIGBUFFERARRAYBASED_RXSIGBUFFEROFRXBUFFERINFO)
    {
      VStdLib_MemCpy(SignalDataPtr, Com_GetAddrRxSigBufferArrayBased((Com_RxSigBufferArrayBasedIterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo)), dynSignalLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_COM_SIGNAL_DATA_PTR */
    }
  }
}

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyFromRxSigBufferToSignalGroupArrayPtr()
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyFromRxSigBufferToSignalGroupArrayPtr(Com_RxSigGrpInfoIterType idxRxSigGrpInfo, P2VAR(void, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr)
{
  if (Com_IsRxBufferInfoUsedOfRxSigGrpInfo(idxRxSigGrpInfo))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    if (Com_GetRxSigBufferOfRxBufferInfo(idxRxBufferInfo) == COM_RXSIGARRAYACCESSSIGGRPBUFFER_RXSIGBUFFEROFRXBUFFERINFO) /* COV_COM_CSL04_CORRECTARGET_CHECK */
    {
      VStdLib_MemCpy((P2VAR(uint8, AUTOMATIC, AUTOMATIC)) SignalGroupArrayPtr, Com_GetAddrRxSigArrayAccessSigGrpBuffer((Com_RxSigArrayAccessSigGrpBufferIterType) Com_GetRxSigBufferStartIdxOfRxBufferInfo(idxRxBufferInfo)), Com_GetRxSigBufferLengthOfRxBufferInfo(idxRxBufferInfo)); /* PRQA S 0315, 0316 */ /* MD_Com_0314_0315_0316_0317 */ /* VCA_COM_SIGNAL_DATA_PTR */
    }
  }
}

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyFromShadowBufferToRxSigBuffer()
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyFromShadowBufferToRxSigBuffer(Com_RxAccessInfoIterType idxRxAccessInfo)
{
  if (Com_IsRxBufferInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */

    /* Get the pointer to the ApplType dependent source shadow buffer */
    CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = Com_RxSigBuffer_GetShdBufferPtr(idxRxAccessInfo);;
    ComDataTypeConst data = { constDataPtr, 0u };

    /* If it is a dynamic length signal, update the dynamic length value of the signal */
    if (Com_IsRxDynSignalLengthUsedOfRxAccessInfo(idxRxAccessInfo) && Com_IsRxDynSignalTmpLengthForSignalGroupsUsedOfRxAccessInfo(idxRxAccessInfo))
    {
      data.dataLength = Com_RxSigBufferHelper_GetRxDynSignalTmpLengthForSignalGroups(idxRxAccessInfo);
      if (data.dataLength <= (PduLengthType) Com_GetRxSigBufferLengthOfRxBufferInfo(idxRxBufferInfo))
      {
        Com_RxSigBufferHelper_SetRxDynSignalLength(idxRxAccessInfo, data.dataLength);
      }
    }
    /* Copy the value of the GroupSignal from the shdBufferValuePtr to the RxSigBuffer */
    Com_RxSigBufferHelper_CopyToSigBuffer(idxRxAccessInfo, idxRxBufferInfo, &data);
  }
}

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyToSigBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyToSigBuffer(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  if (Com_IsRxBufferInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_RxBufferInfoIdxOfRxAccessInfoType idxRxBufferInfo =  Com_GetRxBufferInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_RxSigBufferHelper_CopyToSigBuffer(idxRxAccessInfo, idxRxBufferInfo, comDataPtr);
  }
}

/**********************************************************************************************************************
  Com_RxSigBuffer_CopyToShdwBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopyToShdwBuffer(Com_RxSigGrpInfoIterType idxRxSigGrpInfo, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr)
{
  /* If shadow buffers are required (normal signal group or gateway array access routing) */
  if (Com_IsShdBufferRequiredOfRxSigGrpInfo(idxRxSigGrpInfo))
  {
    /* Iterate over all ComGroupSignals of the passed ComSignalGroup */
    for (Com_RxAccessInfoGrpSigIndIterType rxAccessInfoIndIdx = Com_GetRxAccessInfoGrpSigIndStartIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
         rxAccessInfoIndIdx < Com_GetRxAccessInfoGrpSigIndEndIdxOfRxSigGrpInfo(idxRxSigGrpInfo); rxAccessInfoIndIdx++) /* FETA_COM_01 */
    {
      const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoGrpSigInd(rxAccessInfoIndIdx);

      /* Get the pointer to the ApplType dependent TmpBuffer */
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = Com_RxSigBuffer_GetTmpGrpSigBufferPtr(idxRxAccessInfo);
      ComDataTypeConst data = { constDataPtr, 0u };

      /* If it is a uint8_dyn signal adapt the dynLength parameter */
      if (Com_IsRxDynSignalTmpLengthForSignalGroupsUsedOfRxAccessInfo(idxRxAccessInfo))
      {
        Com_RxSigBufferHelper_SetRxDynSignalTmpLengthForSignalGroups(idxRxAccessInfo, Com_RxSigBuffer_GetRxDynamicLength(PduInfoPtr->SduLength, idxRxAccessInfo));
        data.dataLength = Com_RxSigBufferHelper_GetRxDynSignalTmpLengthForSignalGroups(idxRxAccessInfo);
      }
      /* Copy the value of the tmp shadow buffer to the shadow buffer */
      if (Com_IsRxBufferInfoShdwBufferUsedOfRxAccessInfo(idxRxAccessInfo))
      {
        const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoShdwBufferIdxOfRxAccessInfo(idxRxAccessInfo);
        /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
        Com_RxSigBufferHelper_CopyToSigBuffer(idxRxAccessInfo, idxRxBufferInfo, &data);
      }
    }
  }
}

/**********************************************************************************************************************
  Com_RxSigBuffer_GetSigBufferPtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBuffer_GetSigBufferPtr(Com_RxAccessInfoIterType idxRxAccessInfo)
{
  P2CONST(void, AUTOMATIC, AUTOMATIC) sigBufferPtr = NULL_PTR;

  if (Com_IsRxBufferInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    sigBufferPtr = Com_RxSigBufferHelper_GetSigBufferPtr(idxRxBufferInfo);
  }

  return sigBufferPtr;
}

/**********************************************************************************************************************
  Com_RxSigBuffer_GetShdBufferPtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBuffer_GetShdBufferPtr(Com_RxAccessInfoIterType idxRxAccessInfo)
{
  P2CONST(void, AUTOMATIC, AUTOMATIC) sigBufferPtr = NULL_PTR;

  if (Com_IsRxBufferInfoShdwBufferUsedOfRxAccessInfo(idxRxAccessInfo)) /* This Function is only called when the const value IsFilterEvent is true, which is only the case if the SigGrp has any filters configured. SignalLength is guaranteed to be greater than 0 so the buffer is always present */
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoShdwBufferIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    sigBufferPtr = Com_RxSigBufferHelper_GetSigBufferPtr(idxRxBufferInfo);
  }

  return sigBufferPtr;
}

/**********************************************************************************************************************
  Com_RxSigBuffer_GetTmpGrpSigBufferPtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBuffer_GetTmpGrpSigBufferPtr(Com_RxAccessInfoIterType idxRxAccessInfo)
{
  P2CONST(void, AUTOMATIC, AUTOMATIC) tmpSigBufferPtr = NULL_PTR;

  if (Com_IsRxBufferInfoShdwBufferUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoShdwBufferIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    tmpSigBufferPtr = Com_RxSigBufferHelper_GetTmpSigBufferPtr(idxRxBufferInfo);
  }

  return tmpSigBufferPtr;
}

/**********************************************************************************************************************
  Com_RxSigBuffer_SetRxInitValue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_SetRxInitValue(Com_RxAccessInfoIterType rxAccessInfoIdx)
{
  if (Com_IsRxBufferInfoShdwBufferUsedOfRxAccessInfo(rxAccessInfoIdx))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoShdwBufferIdxOfRxAccessInfo(rxAccessInfoIdx);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_RxSigBufferHelper_SetRxInitValue(rxAccessInfoIdx, idxRxBufferInfo);
  }
  else if (Com_IsRxBufferInfoUsedOfRxAccessInfo(rxAccessInfoIdx))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxAccessInfo(rxAccessInfoIdx);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_RxSigBufferHelper_SetRxInitValue(rxAccessInfoIdx, idxRxBufferInfo);
  }
  else
  {
    /* Intentionally Empty */
  }
}

/**********************************************************************************************************************
  Com_RxSigBuffer_SetRxDataTimeoutSubValue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_SetRxDataTimeoutSubValue(Com_RxAccessInfoIterType rxAccessInfoIdx)
{
  if (Com_IsRxBufferInfoShdwBufferUsedOfRxAccessInfo(rxAccessInfoIdx))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoShdwBufferIdxOfRxAccessInfo(rxAccessInfoIdx);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_RxSigBufferHelper_SetRxDataTimeoutSubValue(rxAccessInfoIdx, idxRxBufferInfo);
  }
  else if (Com_IsRxBufferInfoUsedOfRxAccessInfo(rxAccessInfoIdx))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxAccessInfo(rxAccessInfoIdx);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_RxSigBufferHelper_SetRxDataTimeoutSubValue(rxAccessInfoIdx, idxRxBufferInfo);
  }
  else
  {
    /* Intentionally Empty */
  }
}

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_RxSigBuffer_CopySigToMinProcGwBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopySigToMinProcGwBuffer(Com_RxAccessInfoIterType idxRxAccessInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) newValuePtr)
{
  if (Com_IsRxBufferInfoMinProcGwBufferUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_RxBufferInfoIterType rxBufferInfoIdx = Com_GetRxBufferInfoMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo);
    CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = newValuePtr;
    const PduLengthType constDataLength = (PduLengthType) Com_GetRxSigBufferLengthOfRxBufferInfo(rxBufferInfoIdx);
    const ComDataTypeConst data = { constDataPtr, constDataLength };
    Com_RxSigBufferHelper_CopyToSigBuffer(idxRxAccessInfo, rxBufferInfoIdx, &data);
  }
}
#endif

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_RxSigBuffer_CopySigGrpToMinProcGwBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxSigBuffer_CopySigGrpToMinProcGwBuffer(Com_RxSigGrpInfoIterType idxRxSigGrpInfo)
{
  /* Iterate over all ComGroupSignals of the passed ComSignalGroup */
  for (Com_RxAccessInfoGrpSigIndIterType rxAccessInfoIndIdx = Com_GetRxAccessInfoGrpSigIndStartIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
       rxAccessInfoIndIdx < Com_GetRxAccessInfoGrpSigIndEndIdxOfRxSigGrpInfo(idxRxSigGrpInfo); rxAccessInfoIndIdx++) /* FETA_COM_01 */
  {
    const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoGrpSigInd(rxAccessInfoIndIdx);

    /* Copy the value from the tmp shadow buffer to the MinProcGwBuffer */
    if (Com_IsRxBufferInfoMinProcGwBufferUsedOfRxAccessInfo(idxRxAccessInfo))
    {
      /* Get the pointer to the ApplType dependent TmpBuffer */
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = Com_RxSigBuffer_GetTmpGrpSigBufferPtr(idxRxAccessInfo);
      /* Get the length of the buffer */
      const Com_RxBufferInfoIterType rxBufferInfoIdx = Com_GetRxBufferInfoMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo);
      const PduLengthType constDataLength = (PduLengthType) Com_GetRxSigBufferLengthOfRxBufferInfo(rxBufferInfoIdx);

      const ComDataTypeConst data = { constDataPtr, constDataLength };

      Com_RxSigBufferHelper_CopyToSigBuffer(idxRxAccessInfo, rxBufferInfoIdx, &data);
    }
  }
}
#endif

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
/**********************************************************************************************************************
  Com_RxSigBuffer_GetGatewaySourcePtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_RxSigBuffer_GetGatewaySourcePtr(Com_RxAccessInfoIterType idxRxAccessInfo)
{
  P2CONST(void, AUTOMATIC, AUTOMATIC) sourcePtr = NULL_PTR;

  if (Com_IsRxBufferInfoMinProcGwBufferUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    sourcePtr = (SduDataPtrType) Com_RxSigBufferHelper_GetSigBufferPtr(idxRxBufferInfo); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
  }
  else if (Com_IsRxBufferInfoShdwBufferUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoShdwBufferIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    sourcePtr = (SduDataPtrType) Com_RxSigBufferHelper_GetSigBufferPtr(idxRxBufferInfo); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
  }
  else if (Com_IsRxBufferInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_RxBufferInfoIterType idxRxBufferInfo = Com_GetRxBufferInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert idxRxBufferInfo < Com_GetSizeOfRxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    sourcePtr = (SduDataPtrType) Com_RxSigBufferHelper_GetSigBufferPtr(idxRxBufferInfo); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
  }
  else
  {
    /* Intentionally Empty */
  }

  return sourcePtr;
}
#endif

/**********************************************************************************************************************
  Com_RxSigBuffer_GetRxDynSignalLength
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_RxSigBuffer_GetRxDynSignalLength(Com_RxAccessInfoIterType rxAccessInfoIdx)
{
  PduLengthType ret = 0u;
  if(Com_IsRxDynSignalLengthUsedOfRxAccessInfo(rxAccessInfoIdx))
  {
    ret = Com_RxSigBufferHelper_GetRxDynSignalLength(rxAccessInfoIdx);
  }
  return ret;
}

/**********************************************************************************************************************
  Com_RxSigBuffer_GetRxDynamicLength
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(PduLengthType, COM_CODE) Com_RxSigBuffer_GetRxDynamicLength(PduLengthType sduLength, Com_RxAccessInfoIterType rxAccessInfoIdx)
{
  PduLengthType dynamicLength = 0u;
  if(Com_IsRxDynSignalLengthUsedOfRxAccessInfo(rxAccessInfoIdx))
  {
    /* The dynamic length is the passed SDU length minus the start byte in PDU position. */
    dynamicLength = sduLength - Com_GetStartByteInPduPositionOfRxAccessInfo(rxAccessInfoIdx);
  }
  return dynamicLength;
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
 *  END OF FILE: Com_RxSigBuffer.c
 *********************************************************************************************************************/
