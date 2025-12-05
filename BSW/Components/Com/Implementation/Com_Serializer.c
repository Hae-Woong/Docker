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
/*!        \file  Com_Serializer.c
 *         \unit  Serializer
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_SERIALIZER_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com.h"
#include "Com_Serializer.h"
#include "Com_Cfg_Serializer.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_TxBuffer.h"
#include "Com_Reporting.h"

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

/**********************************************************************************************************************
 *  Com_Serializer_IsEndiannessConversionRequired()
 *********************************************************************************************************************/
/*! \brief       Checks if endianness conversion is required.
 *  \details     -
 *  \param[in]   idxSerializerInfo Index to access information of the signal.
 *  \return      boolean
 *                 TRUE   signal endianness and CPU endianness are different.
 *                 FALSE  signal endianness and CPU endianness are the same or signal endianness is opaque.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE, if callers are using independent dst-buffers
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Serializer_IsEndiannessConversionRequired(Com_SerializerInfoIterType idxSerializerInfo);

/**********************************************************************************************************************
 *  Com_Serializer_Write()
 *********************************************************************************************************************/
/*! \brief       This function calls the copy functions for signals and group signals.
 *  \details     -
 *  \param[in]   idxSerializerInfo          Index to access information of the signal.
 *  \param[in]   bcNumberOfBits             number of bits to be copied
 *  \param[in]   SignalApplSourcePtr        Pointer where the signal is read from
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE, if callers are using independent dst-buffers
 *  \synchronous TRUE
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Serializer_Write(Com_SerializerInfoIterType idxSerializerInfo, uint16 bcNumberOfBits,
                                                           CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalApplSourcePtr);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Serializer_IsEndiannessConversionRequired
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 ********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Serializer_IsEndiannessConversionRequired(Com_SerializerInfoIterType idxSerializerInfo)
{
  boolean retVal = FALSE;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
  retVal = Com_GetSignalEndiannessOfSerializerInfo(idxSerializerInfo) == COM_BIG_ENDIAN_SIGNALENDIANNESSOFSERIALIZERINFO;
#else /* (CPU_BYTE_ORDER == HIGH_BYTE_FIRST) */
  retVal = Com_GetSignalEndiannessOfSerializerInfo(idxSerializerInfo) == COM_LITTLE_ENDIAN_SIGNALENDIANNESSOFSERIALIZERINFO;
#endif

  return retVal;
}

/**********************************************************************************************************************
  Com_Serializer_Write
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Serializer_Write(Com_SerializerInfoIterType idxSerializerInfo, uint16 bcNumberOfBits, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalApplSourcePtr)
{
  const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoIdxOfSerializerInfo(idxSerializerInfo);
  const Com_ConfiguredBitPositionOfSerializerInfoType configuredBitPositionOfSerializerInfo = Com_GetConfiguredBitPositionOfSerializerInfo(idxSerializerInfo);

  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC) bcDst = Com_TxBuffer_GetAddrTxBufferFirstElement(txBufferInfoIdx); /* Com_TxBufferPtrType */ /* Points into PDU-buffer for Signals and into shadow buffer for GroupSignals */
  const Com_TxBufferLengthOfTxBufferInfoType bcDstLength = Com_TxBuffer_GetTxBufferLength(txBufferInfoIdx);
  const VStdLib_CntType bcDstBitOffsetSaw = (VStdLib_CntType) configuredBitPositionOfSerializerInfo % 8u;       /* Modulo 8 because txBufferStartIdx points to first byte of Signal/GroupSignal */
  const Com_SrcBitOffsetOfSerializerInfoType bcSrcBitOffsetSaw = Com_GetSrcBitOffsetOfSerializerInfo(idxSerializerInfo);

  Std_ReturnType bitCpyRet = E_OK;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
  if(Com_Serializer_IsEndiannessConversionRequired(idxSerializerInfo))
  {
    bitCpyRet = VStdLib_BitCpySawLittleToBigEndian_s(bcDst, bcDstLength, bcDstBitOffsetSaw, SignalApplSourcePtr, bcSrcBitOffsetSaw, bcNumberOfBits, E_NOT_OK);
  }
  else
  {
    bitCpyRet = VStdLib_BitCpySawLittleToLittleEndian_s(bcDst, bcDstLength ,bcDstBitOffsetSaw, SignalApplSourcePtr, bcSrcBitOffsetSaw, bcNumberOfBits, E_NOT_OK);
  }
#else /* (CPU_BYTE_ORDER == HIGH_BYTE_FIRST) */
  if(Com_Serializer_IsEndiannessConversionRequired(idxSerializerInfo))
  {
    bitCpyRet = VStdLib_BitCpySawBigToLittleEndian_s(bcDst, bcDstLength, bcDstBitOffsetSaw, SignalApplSourcePtr, bcSrcBitOffsetSaw, bcNumberOfBits, E_NOT_OK);
  }
  else
  {
    bitCpyRet = VStdLib_BitCpySawBigToBigEndian_s(bcDst, bcDstLength, bcDstBitOffsetSaw, SignalApplSourcePtr, bcSrcBitOffsetSaw, bcNumberOfBits, E_NOT_OK);
  }
#endif
  
  if(bitCpyRet == E_NOT_OK) /* copy operation failed */
  {
    Com_Reporting_CallDetReportError(COMServiceId_BitCopy, COM_E_BITCOPY); /* COV_COM_REPORT_ERROR */
  }
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Serializer_WriteSignal
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Serializer_WriteSignal(Com_TxSigInfoIterType SignalId, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  if(comDataPtr->dataPtr != NULL_PTR)
  {
    if(Com_IsSerializerInfoUsedOfTxSigInfo(SignalId))
    {
      const Com_SerializerInfoIterType idxSerializerInfo = Com_GetSerializerInfoIdxOfTxSigInfo(SignalId);
      /*@ assert idxSerializerInfo < Com_GetSizeOfSerializerInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      Com_BitLengthOfSerializerInfoType bitLengthOfSerializerInfo = Com_GetBitLengthOfSerializerInfo(idxSerializerInfo);

      /* If the signal is a dynamic length signal return the current dynamic bit length */
      if(Com_IsDynamicInitialLengthUsedOfTxAccessInfo(Com_GetTxAccessInfoIdxOfTxSigInfo(SignalId)))
      {
        bitLengthOfSerializerInfo = (Com_BitLengthOfSerializerInfoType) (comDataPtr->dataLength * 8u);
      }

      Com_Serializer_Write(idxSerializerInfo, bitLengthOfSerializerInfo, (P2CONST(uint8, AUTOMATIC, AUTOMATIC))comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
    }
  }
}

/**********************************************************************************************************************
  Com_Serializer_WriteGroupSignal
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, COM_CODE) Com_Serializer_WriteGroupSignal(Com_TxGrpSigInfoIterType groupSignalId, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  if(comDataPtr->dataPtr != NULL_PTR)
  {
    if(Com_IsSerializerInfoUsedOfTxGrpSigInfo(groupSignalId))
    {
      const Com_SerializerInfoIterType idxSerializerInfo = Com_GetSerializerInfoIdxOfTxGrpSigInfo(groupSignalId);
      /*@ assert idxSerializerInfo < Com_GetSizeOfSerializerInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      Com_BitLengthOfSerializerInfoType bitLengthOfSerializerInfo = Com_GetBitLengthOfSerializerInfo(idxSerializerInfo);

      /* If the group signal is a dynamic length group signal return the current dynamic bit length */
      if(Com_IsTxTmpPduLengthUsedOfTxGrpSigInfo(groupSignalId))
      {
        bitLengthOfSerializerInfo = (Com_BitLengthOfSerializerInfoType) (comDataPtr->dataLength * 8u);
      }

      Com_Serializer_Write(idxSerializerInfo, bitLengthOfSerializerInfo, (P2CONST(uint8, AUTOMATIC, AUTOMATIC))comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
    }
  }
}

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
 *  END OF FILE: Com_Serializer.c
 *********************************************************************************************************************/
