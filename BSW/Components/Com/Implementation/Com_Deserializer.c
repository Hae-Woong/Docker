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
/*!        \file  Com_Deserializer.c
 *         \unit  Deserializer
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_DESERIALIZER_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com.h"
#include "Com_Deserializer.h"
#include "Com_Cfg_Deserializer.h"
#include "Com_Cfg_ReceptionProcessingData.h"

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
  Com_Deserializer_IsSignedValueNegative
**********************************************************************************************************************/
/*! \brief       Checks if the value is negative by checking the sign extension bit of the value.
 *  \details     -
 *  \param[in]   idxDeserializerInfo Index of the signal.
 *  \param[in]   SignalBusSourcePtr  Pointer to the first byte of the PDU containing the signal
 *  \return      boolean
 *                 TRUE      the value is negative (value less than 0)
 *                 FALSE     the value is positive (value greater than or equal to 0)
 *  \pre         Use only for signed datatypes. Checked by Com_IsSignExtRequiredOfDeserializerInfo().
 *  \context     TASK|ISR2
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Deserializer_IsSignedValueNegative(Com_DeserializerInfoIterType idxDeserializerInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr);

/**********************************************************************************************************************
 *  Com_Deserializer_IsEndiannessConversionRequired()
 *********************************************************************************************************************/
/*! \brief       Checks if endianness conversion is required.
 *  \details     -
 *  \param[in]   idxDeserializerInfo Index of the signal.
 *  \return      boolean
 *                 TRUE      signal endianness and CPU endianness are different.
 *                 FALSE     signal endianness and CPU endianness are the same or signal endianness is opaque.
 *  \pre         -
 *  \context     TASK|ISR2
 *  \reentrant   TRUE, for different handles
 *  \synchronous TRUE
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Deserializer_IsEndiannessConversionRequired(Com_DeserializerInfoIterType idxDeserializerInfo);

/**********************************************************************************************************************
  Com_Deserializer_ReadSignalOrGroupSignal
**********************************************************************************************************************/
/*! \brief        This method reads the signal value endianess and CPU Byte order specific form the source buffer.
    \details      -
    \pre          -
    \param[in]    idxDeserializerInfo   ID of the Rx DeserializerInfo
    \param[out]   SignalApplDestPtr     Pointer where the signal is written to
    \param[in]    SignalBusSourcePtr    Pointer to the first byte of the PDU containing the signal
    \param[in]    destLength            length of the destination buffer in bytes.
    \context      TASK|ISR2
    \reentrant    TRUE, for different handles
    \synchronous  TRUE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Deserializer_ReadSignalOrGroupSignal(Com_DeserializerInfoIterType idxDeserializerInfo,
                                                                            CONSTP2VAR(void, AUTOMATIC, AUTOMATIC) SignalApplDestPtr,
                                                                            CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr, uint32 destLength);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_Deserializer_IsSignedValueNegative
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Deserializer_IsSignedValueNegative(Com_DeserializerInfoIterType idxDeserializerInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr)
{
  Com_ConfiguredBitPositionOfDeserializerInfoType msbBitPosition = Com_GetConfiguredBitPositionOfDeserializerInfo(idxDeserializerInfo);
  if (Com_GetSignalEndiannessOfDeserializerInfo(idxDeserializerInfo) == COM_LITTLE_ENDIAN_SIGNALENDIANNESSOFDESERIALIZERINFO)
  {
    msbBitPosition += ((Com_ConfiguredBitPositionOfDeserializerInfoType) Com_GetBitLengthOfDeserializerInfo(idxDeserializerInfo) - 1u);
  }

  const Com_ConfiguredBitPositionOfDeserializerInfoType signBytePos = msbBitPosition / 8u;
  const uint8 signBitPos = COM_CAST2UINT8(msbBitPosition % 8u);
  const uint8 signMask = COM_CAST2UINT8(1u << signBitPos);

  const boolean isNegative = (0u != (SignalBusSourcePtr[signBytePos] & signMask));
  return isNegative;
}

/**********************************************************************************************************************
  Com_Deserializer_IsEndiannessConversionRequired
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 ********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_Deserializer_IsEndiannessConversionRequired(Com_DeserializerInfoIterType idxDeserializerInfo)
{
  boolean retVal = FALSE;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
  retVal = Com_GetSignalEndiannessOfDeserializerInfo(idxDeserializerInfo) == COM_BIG_ENDIAN_SIGNALENDIANNESSOFDESERIALIZERINFO;
#else /* (CPU_BYTE_ORDER == HIGH_BYTE_FIRST) */
  retVal = Com_GetSignalEndiannessOfDeserializerInfo(idxDeserializerInfo) == COM_LITTLE_ENDIAN_SIGNALENDIANNESSOFDESERIALIZERINFO;
#endif

  return retVal;
}

/**********************************************************************************************************************
  Com_Deserializer_ReadSignalOrGroupSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_Deserializer_ReadSignalOrGroupSignal(Com_DeserializerInfoIterType idxDeserializerInfo,
                                                                            CONSTP2VAR(void, AUTOMATIC, AUTOMATIC) SignalApplDestPtr,
                                                                            CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr, uint32 destLength)
{
  /*@ assert idxDeserializerInfo < Com_GetSizeOfDeserializerInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
  const Com_DestBitOffsetOfDeserializerInfoType destBitOffsetOfDeserializerInfo = Com_GetDestBitOffsetOfDeserializerInfo(idxDeserializerInfo);
  const Com_ConfiguredBitPositionOfDeserializerInfoType configuredBitPositionOfDeserializerInfo = Com_GetConfiguredBitPositionOfDeserializerInfo(idxDeserializerInfo);
  const Com_BitLengthOfDeserializerInfoType bitLengthOfDeserializerInfo = Com_GetBitLengthOfDeserializerInfo(idxDeserializerInfo);

  /* Check if endianness conversion is needed and call the corresponding bitcopy function */
  Std_ReturnType bitCpyRet = E_OK;
#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
  if(Com_Deserializer_IsEndiannessConversionRequired(idxDeserializerInfo))
  {
    bitCpyRet = VStdLib_BitCpySawBigToLittleEndian_s(SignalApplDestPtr, destLength, destBitOffsetOfDeserializerInfo, SignalBusSourcePtr, configuredBitPositionOfDeserializerInfo, bitLengthOfDeserializerInfo, E_NOT_OK);
  }
  else
  {
    bitCpyRet = VStdLib_BitCpySawLittleToLittleEndian_s(SignalApplDestPtr, destLength, destBitOffsetOfDeserializerInfo, SignalBusSourcePtr, configuredBitPositionOfDeserializerInfo, bitLengthOfDeserializerInfo, E_NOT_OK);
  }
#else /* (CPU_BYTE_ORDER == HIGH_BYTE_FIRST) */
  if(Com_Deserializer_IsEndiannessConversionRequired(idxDeserializerInfo))
  {
    bitCpyRet = VStdLib_BitCpySawLittleToBigEndian_s(SignalApplDestPtr, destLength, destBitOffsetOfDeserializerInfo, SignalBusSourcePtr, configuredBitPositionOfDeserializerInfo, bitLengthOfDeserializerInfo, E_NOT_OK);
  }
  else
  {
    bitCpyRet = VStdLib_BitCpySawBigToBigEndian_s(SignalApplDestPtr, destLength, destBitOffsetOfDeserializerInfo, SignalBusSourcePtr, configuredBitPositionOfDeserializerInfo, bitLengthOfDeserializerInfo, E_NOT_OK);
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
  Com_Deserializer_ReadSignal_UInt8
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_Deserializer_ReadSignal_UInt8(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr)
{
  uint8 tmpNewValue = 0;

  if(Com_IsDeserializerInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    Com_Deserializer_ReadSignalOrGroupSignal(Com_GetDeserializerInfoIdxOfRxAccessInfo(idxRxAccessInfo), &tmpNewValue, SignalBusSourcePtr, 1);
  }

  return tmpNewValue;
}

/**********************************************************************************************************************
  Com_Deserializer_ReadSignal_SInt8
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(sint8, COM_CODE) Com_Deserializer_ReadSignal_SInt8(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr)
{
  sint8 tmpNewValue = 0;

  if(Com_IsDeserializerInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_DeserializerInfoIterType idxDeserializerInfo = Com_GetDeserializerInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    if(Com_IsSignExtRequiredOfDeserializerInfo(idxDeserializerInfo))
    {
      if(Com_Deserializer_IsSignedValueNegative(idxDeserializerInfo, SignalBusSourcePtr))
      {
        tmpNewValue = -1;
      }
    }

    Com_Deserializer_ReadSignalOrGroupSignal(idxDeserializerInfo, &tmpNewValue, SignalBusSourcePtr, 1);
  }

  return tmpNewValue;
}

/**********************************************************************************************************************
  Com_Deserializer_ReadSignal_UInt16
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
FUNC(uint16, COM_CODE) Com_Deserializer_ReadSignal_UInt16(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr)
{
  uint16 tmpNewValue = 0;

  if(Com_IsDeserializerInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    Com_Deserializer_ReadSignalOrGroupSignal(Com_GetDeserializerInfoIdxOfRxAccessInfo(idxRxAccessInfo), &tmpNewValue, SignalBusSourcePtr, 2);
  }

  return tmpNewValue;
}

/**********************************************************************************************************************
  Com_Deserializer_ReadSignal_SInt16
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(sint16, COM_CODE) Com_Deserializer_ReadSignal_SInt16(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr)
{
  sint16 tmpNewValue = 0;

  if(Com_IsDeserializerInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_DeserializerInfoIterType idxDeserializerInfo = Com_GetDeserializerInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    if(Com_IsSignExtRequiredOfDeserializerInfo(idxDeserializerInfo))
    {
      if(Com_Deserializer_IsSignedValueNegative(idxDeserializerInfo, SignalBusSourcePtr))
      {
        tmpNewValue = -1;
      }
    }

    Com_Deserializer_ReadSignalOrGroupSignal(idxDeserializerInfo, &tmpNewValue, SignalBusSourcePtr, 2);
  }

  return tmpNewValue;
}

/**********************************************************************************************************************
  Com_Deserializer_ReadSignal_UInt32
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
FUNC(uint32, COM_CODE) Com_Deserializer_ReadSignal_UInt32(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr)
{
  uint32 tmpNewValue = 0;

  if(Com_IsDeserializerInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    Com_Deserializer_ReadSignalOrGroupSignal(Com_GetDeserializerInfoIdxOfRxAccessInfo(idxRxAccessInfo), &tmpNewValue, SignalBusSourcePtr, 4);
  }

  return tmpNewValue;
}

/**********************************************************************************************************************
  Com_Deserializer_ReadSignal_SInt32
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(sint32, COM_CODE) Com_Deserializer_ReadSignal_SInt32(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr)
{
  sint32 tmpNewValue = 0;

  if(Com_IsDeserializerInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_DeserializerInfoIterType idxDeserializerInfo = Com_GetDeserializerInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    if(Com_IsSignExtRequiredOfDeserializerInfo(idxDeserializerInfo))
    {
      if(Com_Deserializer_IsSignedValueNegative(idxDeserializerInfo, SignalBusSourcePtr))
      {
        tmpNewValue = -1;
      }
    }

    Com_Deserializer_ReadSignalOrGroupSignal(idxDeserializerInfo, &tmpNewValue, SignalBusSourcePtr, 4);
  }

  return tmpNewValue;
}

/**********************************************************************************************************************
  Com_Deserializer_ReadSignal_UInt64
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
FUNC(uint64, COM_CODE) Com_Deserializer_ReadSignal_UInt64(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr)
{
  uint64 tmpNewValue = 0;

  if(Com_IsDeserializerInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    Com_Deserializer_ReadSignalOrGroupSignal(Com_GetDeserializerInfoIdxOfRxAccessInfo(idxRxAccessInfo), &tmpNewValue, SignalBusSourcePtr, 8);
  }

  return tmpNewValue;
}

/**********************************************************************************************************************
  Com_Deserializer_ReadSignal_SInt64
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(sint64, COM_CODE) Com_Deserializer_ReadSignal_SInt64(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr)
{
  sint64 tmpNewValue = 0;

  if(Com_IsDeserializerInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    const Com_DeserializerInfoIterType idxDeserializerInfo = Com_GetDeserializerInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    if(Com_IsSignExtRequiredOfDeserializerInfo(idxDeserializerInfo))
    {
      if(Com_Deserializer_IsSignedValueNegative(idxDeserializerInfo, SignalBusSourcePtr))
      {
        tmpNewValue = -1;
      }
    }

    Com_Deserializer_ReadSignalOrGroupSignal(idxDeserializerInfo, &tmpNewValue, SignalBusSourcePtr, 8);
  }

  return tmpNewValue;
}

/**********************************************************************************************************************
  Com_Deserializer_ReadSignal_Float32
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
FUNC(float32, COM_CODE) Com_Deserializer_ReadSignal_Float32(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr)
{
  float32 tmpNewValue = 0.0f;

  if(Com_IsDeserializerInfoUsedOfRxAccessInfo(idxRxAccessInfo)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    Com_Deserializer_ReadSignalOrGroupSignal(Com_GetDeserializerInfoIdxOfRxAccessInfo(idxRxAccessInfo), &tmpNewValue, SignalBusSourcePtr, 4);
  }

  return tmpNewValue;
}

/**********************************************************************************************************************
  Com_Deserializer_ReadSignal_Float64
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 **********************************************************************************************************************/
FUNC(float64, COM_CODE) Com_Deserializer_ReadSignal_Float64(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalBusSourcePtr)
{
  float64 tmpNewValue = 0.0;

  if(Com_IsDeserializerInfoUsedOfRxAccessInfo(idxRxAccessInfo))  /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    Com_Deserializer_ReadSignalOrGroupSignal(Com_GetDeserializerInfoIdxOfRxAccessInfo(idxRxAccessInfo), &tmpNewValue, SignalBusSourcePtr, 8);
  }

  return tmpNewValue;
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
 *  END OF FILE: Com_Deserializer.c
 *********************************************************************************************************************/
