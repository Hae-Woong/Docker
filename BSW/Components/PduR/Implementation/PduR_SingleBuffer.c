/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  PduR_SingleBuffer.c
 *         \unit  SingleBuffer
 *        \brief  Pdu Router Single Buffer source file
 *      \details  This is the implementation of the MICROSAR Pdu Router module.
 *                The basic software module is based on the AUTOSAR PduR specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * LOCAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

#define PDUR_SINGLEBUFFER_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_SingleBuffer.h"
#include "PduR_Reporting.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

#if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_SingleBuffer_GetActualLengthOfSingleBufferRamSafe
 *********************************************************************************************************************/
 /*! \brief      Get the actual length of the latest data in the single buffer.
 *  \details     -
 *  \param[in]   singleBufferRomIdx            Valid SingleBufferRom table index. Internal handle of a PduRSingleBuffer.
 *  \param[in]   memIdx                        memory section index
 *  \pre         -
 *  \context     TASK|ISR1|ISR2
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_ActualLengthOfSingleBufferRamType, PDUR_CODE) PduR_SingleBuffer_GetActualLengthOfSingleBufferRamSafe(PduR_SingleBufferRomIterType singleBufferRomIdx, PduR_MemIdxType memIdx);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * PduR_SingleBuffer_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
#if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
FUNC(void, PDUR_CODE) PduR_SingleBuffer_Init(PduR_MemIdxType memIdx)
{
  PduR_SingleBufferRomIterType singleBufferRomIdx;

  for(singleBufferRomIdx = 0u; singleBufferRomIdx < PduR_GetSizeOfSingleBufferRom(memIdx); singleBufferRomIdx++)  /* FETA_PDUR_01 */
  {
    VStdLib_MemCpy_s(PduR_GetAddrSingleBufferArrayRam(PduR_GetSingleBufferArrayRamStartIdxOfSingleBufferRom(singleBufferRomIdx, memIdx), memIdx), /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */  /* VCA_PDUR_SINGLEBUFFER_MEMCPY_S_2 */
                                                              PduR_GetSingleBufferArrayRamLengthOfSingleBufferRom(singleBufferRomIdx, memIdx),
                                                              PduR_GetAddrSingleBufferInitValuesRom(PduR_GetSingleBufferInitValuesRomStartIdxOfSingleBufferRom(singleBufferRomIdx, memIdx), memIdx),
                                                              PduR_GetSingleBufferInitValuesRomLengthOfSingleBufferRom(singleBufferRomIdx, memIdx));

    PduR_SetActualLengthOfSingleBufferRam(singleBufferRomIdx, (PduR_ActualLengthOfSingleBufferRamType) PduR_GetSingleBufferArrayRamLengthOfSingleBufferRom(singleBufferRomIdx, memIdx), memIdx);
  }
}
#endif

/**********************************************************************************************************************
 * PduR_SingleBuffer_Put
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
#if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
FUNC(Std_ReturnType, PDUR_CODE) PduR_SingleBuffer_Put(PduR_SingleBufferRomIterType singleBufferRomIdx, PduR_MemIdxType memIdx, PduLengthType pduLength, SduDataPtrType sduDataPtr)  /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  VStdLib_MemCpy_s(PduR_GetAddrSingleBufferArrayRam(PduR_GetSingleBufferArrayRamStartIdxOfSingleBufferRom(singleBufferRomIdx, memIdx), memIdx), PduR_GetSingleBufferArrayRamLengthOfSingleBufferRom(singleBufferRomIdx, memIdx), sduDataPtr, pduLength);   /* PRQA S 0315, 3673 */ /* MD_MSR_VStdLibCopy, MD_MSR_Rule8.13 */  /* VCA_PDUR_SINGLEBUFFER_MEMCPY_S_1 */
  PduR_SetActualLengthOfSingleBufferRam(singleBufferRomIdx, pduLength, memIdx);

  return E_OK;
}
#endif

/**********************************************************************************************************************
 * PduR_SingleBuffer_Get
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
#if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
FUNC(Std_ReturnType, PDUR_CODE) PduR_SingleBuffer_Get(PduR_SingleBufferRomIterType singleBufferRomIdx, PduR_MemIdxType memIdx, P2VAR(PduR_RmDestRomIterType, AUTOMATIC, PDUR_APPL_DATA) rmDestRomIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  info->SduDataPtr = PduR_GetAddrSingleBufferArrayRam(PduR_GetSingleBufferArrayRamStartIdxOfSingleBufferRom(singleBufferRomIdx, memIdx), memIdx);
  info->SduLength = PduR_SingleBuffer_GetActualLengthOfSingleBufferRamSafe(singleBufferRomIdx, memIdx);
  /* the stored DestPdu handle ID is static and does not need to be checked. */
  *rmDestRomIdx = PduR_GetRmDestRomIdxOfSingleBufferRom(singleBufferRomIdx, memIdx);
  return E_OK;
}
#endif

#if (PDUR_SINGLEBUFFERROM == STD_ON)  /* COV_PDUR_VAR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * PduR_SingleBuffer_GetActualLengthOfSingleBufferRamSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *********************************************************************************************************************/
PDUR_LOCAL_INLINE FUNC(PduR_ActualLengthOfSingleBufferRamType, PDUR_CODE) PduR_SingleBuffer_GetActualLengthOfSingleBufferRamSafe(PduR_SingleBufferRomIterType singleBufferRomIdx, PduR_MemIdxType memIdx)
{
  PduR_ActualLengthOfSingleBufferRamType actualLengthOfSingleBuffer = PduR_GetActualLengthOfSingleBufferRam(singleBufferRomIdx, memIdx);

# if (PDUR_EXTENDED_ERROR_CHECKS == STD_ON)  /* COV_PDUR_VAR_EXTENDED_ERROR_CHECKS */
  PduR_SingleBufferArrayRamLengthOfSingleBufferRomType singleBufferArrayLength = PduR_GetSingleBufferArrayRamLengthOfSingleBufferRom(singleBufferRomIdx, memIdx);

  if(actualLengthOfSingleBuffer > singleBufferArrayLength)
  {
    actualLengthOfSingleBuffer = (PduR_ActualLengthOfSingleBufferRamType) singleBufferArrayLength;
    PduR_Reporting_ReportRuntimeError(PDUR_FCT_GETSINGLEBUFFERLENGTH, PDUR_E_IDXOUTOFBOUNDS);
  }
# endif

  return actualLengthOfSingleBuffer;
}
#endif

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: PduR_SingleBuffer.c
 *********************************************************************************************************************/
