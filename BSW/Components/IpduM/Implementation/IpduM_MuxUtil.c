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
 *         \file  IpduM_MuxUtil.c
 *         \unit  MuxUtil
 *        \brief  Microsar IpduM Multiplexer util source file
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

#define IPDUM_MUXUTIL_SOURCE
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
#include "IpduM_MuxUtil.h"
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
 *  IpduM_MuxUtil_CopySegment()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_MuxUtil_CopySegment(IpduM_SegmentIterType segmentIdx, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) destPduInfoPtr, P2CONST(uint8, AUTOMATIC, IPDUM_APPL_DATA) srcPtr)
{
  /*@ assert (srcPtr != NULL_PTR); */  /* VCA_IPDUM_API_PATTERN_CHECK */

  /* Call the endianness dependent bitcopy function */
  Std_ReturnType bitCpyRet = E_OK;
  IpduM_StartPositionOfSegmentType startPosition = IpduM_GetStartPositionOfSegment(segmentIdx); /* source and destination have the same start position */

  if(IpduM_IsLittleEndianOfSegment(segmentIdx))
  {
    bitCpyRet = VStdLib_BitCpySawLittleToLittleEndian_s(destPduInfoPtr->SduDataPtr, destPduInfoPtr->SduLength , startPosition, srcPtr, startPosition, IpduM_GetSegmentLengthInBitOfSegment(segmentIdx), E_NOT_OK);
  }
  else
  {
    bitCpyRet = VStdLib_BitCpySawBigToBigEndian_s(destPduInfoPtr->SduDataPtr, destPduInfoPtr->SduLength , startPosition, srcPtr, startPosition, IpduM_GetSegmentLengthInBitOfSegment(segmentIdx), E_NOT_OK);
  }

  if(bitCpyRet != E_OK) /* copy operation failed */
  {
    IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_MUX_COPYSEGMENTS, IPDUM_E_PARAM);
  }
}

/*******************************************************************************************
*  IpduM_MuxUtil_WriteMuxFillerSegment()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, IPDUM_CODE) IpduM_MuxUtil_WriteMuxFillerSegment(IpduM_RestSegmentIterType restSegmentIdx, IpduM_CTxPathwayIterType txPathwayIdx, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) destPduInfoPtr)
{
  IpduM_UnusedAreasDefaultOfTxPathwayType unusedPattern = IpduM_GetUnusedAreasDefaultOfTxPathway(txPathwayIdx);
  /* Only partial byte writing needs exclusive area for read-modify-write protection */
  /* Set copy destination pointer to the start position of the provided dest buffer */
  P2VAR(uint8, AUTOMATIC, IPDUM_APPL_DATA) pDest = destPduInfoPtr->SduDataPtr;
  
  if(IpduM_IsPartialEndByteOfRestSegment(restSegmentIdx))  
  {
    if(IpduM_GetLastBytePosOfRestSegment(restSegmentIdx) < destPduInfoPtr->SduLength)
    {
      /* copy the last (partial) byte from source to destination */
      pDest[IpduM_GetLastBytePosOfRestSegment(restSegmentIdx)] &= IpduM_GetEndByteMaskClearOfRestSegment(restSegmentIdx);
      pDest[IpduM_GetLastBytePosOfRestSegment(restSegmentIdx)] |= (unusedPattern & IpduM_GetEndByteMaskWriteOfRestSegment(restSegmentIdx));
    }
    else
    {
      IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_MUXTX_WRITEMUXFILLERSEGMENTS, IPDUM_E_PARAM);
    }
  }

  if(IpduM_IsFullBytesPresentOfRestSegment(restSegmentIdx))  
  {
    IpduM_LastFullBytePosOfRestSegmentType pos;
    for(pos = IpduM_GetFirstFullBytePosOfRestSegment(restSegmentIdx); pos <= IpduM_GetLastFullBytePosOfRestSegment(restSegmentIdx); pos++) /* FETA_IPDUM_01 */
    {
      if(pos < destPduInfoPtr->SduLength)
      {
        pDest[pos] = unusedPattern;
      }
      else
      {
        IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_MUXTX_WRITEMUXFILLERSEGMENTS, IPDUM_E_PARAM);
      }
    }
  }

  if(IpduM_IsPartialStartByteOfRestSegment(restSegmentIdx))
  {
    if(IpduM_GetFirstBytePosOfRestSegment(restSegmentIdx) < destPduInfoPtr->SduLength)
    {
      /* copy the first (partial) byte from source to destination */
      pDest[IpduM_GetFirstBytePosOfRestSegment(restSegmentIdx)] &= IpduM_GetStartByteMaskClearOfRestSegment(restSegmentIdx);
      pDest[IpduM_GetFirstBytePosOfRestSegment(restSegmentIdx)] |= (unusedPattern & IpduM_GetStartByteMaskWriteOfRestSegment(restSegmentIdx));
    }
    else
    {
      IpduM_Reporting_ReportRuntimeError(IPDUM_APIID_MUXTX_WRITEMUXFILLERSEGMENTS, IPDUM_E_PARAM);
    }
  }
}

/*******************************************************************************************
*  IpduM_MuxUtil_RxDynPduMatchesSelectorPattern()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_MuxUtil_RxDynPduMatchesSelectorPattern(IpduM_RxMuxPduIterType rxMuxPduIdx, IpduM_RxDynPduIterType rxDynPduIdx, P2CONST(PduInfoType, AUTOMATIC, IPDUM_APPL_DATA) PduInfoPtr)
{
  boolean patternMatch = TRUE;      /* Initialize with true, turn false on any sub-pattern mismatch. */
  uint8 inputOffset = 0;    /* Offset of initial pointer to current pattern position (valid values: 0, 1, 2). */
  IpduM_RxSelectorPatternIterType ipduMSubPatternIdx = IpduM_GetRxSelectorPatternStartIdxOfRxDynPdu(rxDynPduIdx);

  /* loop over subpatterns */
  for(; ipduMSubPatternIdx < IpduM_GetRxSelectorPatternEndIdxOfRxDynPdu(rxDynPduIdx); ipduMSubPatternIdx++) /* FETA_IPDUM_01 */
  {
    CONSTP2CONST(uint8, TYPEDEF, IPDUM_APPL_DATA) pSrc = PduInfoPtr->SduDataPtr;
    /*@ assert (pSrc != NULL_PTR); */  /* VCA_IPDUM_API_PATTERN_CHECK */
    /* mask input data and compare with pattern */
    uint8 selectorSubPattern = pSrc[(IpduM_GetSelectorFieldBytePositionOfRxMuxPdu(rxMuxPduIdx) + inputOffset)] & IpduM_GetSelectorFieldSubMaskOfRxSelectorPattern(ipduMSubPatternIdx);  /* PRQA S 2985, 2986 */ /* MD_IpduM_2985, MD_IpduM_2986 */
    if(selectorSubPattern != IpduM_GetSelectorFieldSubPatternOfRxSelectorPattern(ipduMSubPatternIdx))
    {
      patternMatch = FALSE;
      break;
    }
    inputOffset++;  /* PRQA S 2982 */ /* MD_IpduM_2982 */
  }

  return patternMatch;
}

/*******************************************************************************************
*  IpduM_MuxUtil_IsDynPartLongEnough()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_MuxUtil_IsDynPartLongEnough(IpduM_RxDynPduIterType rxDynPduIdx, PduLengthType length)
{
  return (length >= IpduM_GetMinDLCOfRxDynPdu(rxDynPduIdx));
}

/*******************************************************************************************
*  IpduM_MuxUtil_IsStaticPartLongEnough()
*******************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, IPDUM_CODE) IpduM_MuxUtil_IsStaticPartLongEnough(IpduM_RxStaticPduIterType rxStaticPduIdx, PduLengthType length)
{
  return (length >= IpduM_GetMinDLCOfRxStaticPdu(rxStaticPduIdx));
}

#define IPDUM_STOP_SEC_CODE
#include "IpduM_MemMap.h"    /* PRQA S 5087 */        /*  MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
  END OF FILE: IpduM_MuxUtil.c
**********************************************************************************************************************/
