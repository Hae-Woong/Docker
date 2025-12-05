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
/*!        \file  Com_SignalFilterHdlr.c
 *         \unit  SignalFilterHdlr
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_SIGNALFILTERHDLR_SOURCE

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
#include "Com_SignalFilterHdlr.h"
#include "Com_Cfg_FilterData.h"

#include "Com_SignalFilterHdlrHelper.h"

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
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_SignalFilterHdlr_EvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(boolean, COM_CODE) Com_SignalFilterHdlr_EvaluateFilter(Com_FilterInfoIterType idxFilterInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) oldValuePtr, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  switch (Com_GetApplTypSpecificFilterInfoOfFilterInfo(idxFilterInfo))
  {
    case COM_FILTERINFO_UINT8_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      uint8 newValue = 0u;
      uint8 oldValue = 0u;
      if (comDataPtr->dataPtr != NULL_PTR)
      {
        newValue = *((P2CONST(uint8, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      if (oldValuePtr != NULL_PTR)
      {
        oldValue = *((P2CONST(uint8, AUTOMATIC, AUTOMATIC)) oldValuePtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      filterState = Com_SignalFilterHdlrHelper_UInt8_EvaluateFilter(idxFilterInfo, newValue, oldValue);
      break;
    }
    case COM_FILTERINFO_SINT8_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      sint8 newValue = 0;
      sint8 oldValue = 0;
      if (comDataPtr->dataPtr != NULL_PTR)
      {
        newValue = *((P2CONST(sint8, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      if (oldValuePtr != NULL_PTR)
      {
        oldValue = *((P2CONST(sint8, AUTOMATIC, AUTOMATIC)) oldValuePtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      filterState = Com_SignalFilterHdlrHelper_SInt8_EvaluateFilter(idxFilterInfo, newValue, oldValue);
      break;
    }
    case COM_FILTERINFO_UINT16_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      uint16 newValue = 0u;
      uint16 oldValue = 0u;
      if (comDataPtr->dataPtr != NULL_PTR)
      {
        newValue = *((P2CONST(uint16, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      if (oldValuePtr != NULL_PTR)
      {
        oldValue = *((P2CONST(uint16, AUTOMATIC, AUTOMATIC)) oldValuePtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      filterState = Com_SignalFilterHdlrHelper_UInt16_EvaluateFilter(idxFilterInfo, newValue, oldValue);
      break;
    }
    case COM_FILTERINFO_SINT16_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      sint16 newValue = 0;
      sint16 oldValue = 0;
      if (comDataPtr->dataPtr != NULL_PTR)
      {
        newValue = *((P2CONST(sint16, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      if (oldValuePtr != NULL_PTR)
      {
        oldValue = *((P2CONST(sint16, AUTOMATIC, AUTOMATIC)) oldValuePtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      filterState = Com_SignalFilterHdlrHelper_SInt16_EvaluateFilter(idxFilterInfo, newValue, oldValue);
      break;
    }
    case COM_FILTERINFO_UINT32_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      uint32 newValue = 0u;
      uint32 oldValue = 0u;
      if (comDataPtr->dataPtr != NULL_PTR)
      {
        newValue = *((P2CONST(uint32, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      if (oldValuePtr != NULL_PTR)
      {
        oldValue = *((P2CONST(uint32, AUTOMATIC, AUTOMATIC)) oldValuePtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      filterState = Com_SignalFilterHdlrHelper_UInt32_EvaluateFilter(idxFilterInfo, newValue, oldValue);
      break;
    }
    case COM_FILTERINFO_SINT32_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      sint32 newValue = 0;
      sint32 oldValue = 0;
      if (comDataPtr->dataPtr != NULL_PTR)
      {
        newValue = *((P2CONST(sint32, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      if (oldValuePtr != NULL_PTR)
      {
        oldValue = *((P2CONST(sint32, AUTOMATIC, AUTOMATIC)) oldValuePtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      filterState = Com_SignalFilterHdlrHelper_SInt32_EvaluateFilter(idxFilterInfo, newValue, oldValue);
      break;
    }
    case COM_FILTERINFO_UINT64_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      uint64 newValue = 0u;
      uint64 oldValue = 0u;
      if (comDataPtr->dataPtr != NULL_PTR)
      {
        newValue = *((P2CONST(uint64, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      if (oldValuePtr != NULL_PTR)
      {
        oldValue = *((P2CONST(uint64, AUTOMATIC, AUTOMATIC)) oldValuePtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      filterState = Com_SignalFilterHdlrHelper_UInt64_EvaluateFilter(idxFilterInfo, newValue, oldValue);
      break;
    }
    case COM_FILTERINFO_SINT64_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      sint64 newValue = 0;
      sint64 oldValue = 0;
      if (comDataPtr->dataPtr != NULL_PTR)
      {
        newValue = *((P2CONST(sint64, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      if (oldValuePtr != NULL_PTR)
      {
        oldValue = *((P2CONST(sint64, AUTOMATIC, AUTOMATIC)) oldValuePtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      filterState = Com_SignalFilterHdlrHelper_SInt64_EvaluateFilter(idxFilterInfo, newValue, oldValue);
      break;
    }
    case COM_FILTERINFO_FLOAT32_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      float32 newValue = 0.0f;
      float32 oldValue = 0.0f;
      if (comDataPtr->dataPtr != NULL_PTR)
      {
        newValue = *((P2CONST(float32, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      if (oldValuePtr != NULL_PTR)
      {
        oldValue = *((P2CONST(float32, AUTOMATIC, AUTOMATIC)) oldValuePtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      filterState = Com_SignalFilterHdlrHelper_Float32_EvaluateFilter(idxFilterInfo, newValue, oldValue);
      break;
    }
    case COM_FILTERINFO_FLOAT64_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      float64 newValue = 0.0;
      float64 oldValue = 0.0;
      if (comDataPtr->dataPtr != NULL_PTR)
      {
        newValue = *((P2CONST(float64, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      if (oldValuePtr != NULL_PTR)
      {
        oldValue = *((P2CONST(float64, AUTOMATIC, AUTOMATIC)) oldValuePtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      }
      filterState = Com_SignalFilterHdlrHelper_Float64_EvaluateFilter(idxFilterInfo, newValue, oldValue);
      break;
    }
    case COM_FILTERINFO_UINT8_N_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt8_N_EvaluateFilter(idxFilterInfo, (P2CONST(uint8, AUTOMATIC, AUTOMATIC)) oldValuePtr, comDataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_FILTERINFO_UINT8_DYN_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      filterState = Com_SignalFilterHdlrHelper_UInt8_Dyn_EvaluateFilter(idxFilterInfo); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_FILTERINFO_ZEROBIT_APPLTYPSPECIFICFILTERINFOOFFILTERINFO:
    {
      filterState = Com_SignalFilterHdlrHelper_ZeroBit_EvaluateFilter(idxFilterInfo); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    default:  /* COV_COM_MISRA */
      /* Intentionally Empty */
      break;
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_SignalFilterHdlr_EvaluateSigGrpArrFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_SignalFilterHdlr_EvaluateSigGrpArrFilter(Com_FilterInfoIterType idxFilterInfo, P2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  if(Com_GetApplTypSpecificFilterInfoOfFilterInfo(idxFilterInfo) == COM_SIGGRPARRAYFILTERINFO_APPLTYPSPECIFICFILTERINFOOFFILTERINFO)  /* COV_COM_CSL04_CORRECTARGET_CHECK */
  {
    const Com_SigGrpArrayFilterInfoIterType idxFilterInfoApplTypSpecific = (Com_SigGrpArrayFilterInfoIterType) Com_GetApplTypSpecificFilterInfoIdxOfFilterInfo(idxFilterInfo);
    CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) newValuePtr = &(SignalGroupArrayPtr[Com_GetOffsetInSignalGroupOfSigGrpArrayFilterInfo(idxFilterInfoApplTypSpecific)]);
    /*@ assert idxFilterInfoApplTypSpecific < Com_GetSizeOfSigGrpArrayFilterInfo(); */ /* VCA_COM_ENUM_INDEX_CSL04 */
    filterState = Com_SignalFilterHdlrHelper_SigGrpArr_EvaluateFilter(idxFilterInfoApplTypSpecific, newValuePtr);
  }

  return filterState;
}


/**********************************************************************************************************************
  Com_SignalFilterHdlr_SetFilterOldValueToInitValue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalFilterHdlr_SetFilterOldValueToInitValue(Com_FilterInfoIterType idxFilterInfo)
{
  Com_SignalFilterHdlrHelper_SetFilterOldValueToInitValue(idxFilterInfo);
}

/**********************************************************************************************************************
  Com_SignalFilterHdlr_SetFilterOldValueX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalFilterHdlr_SetFilterOldValueX(Com_FilterInfoIterType idxFilterInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr)
{
  Com_SignalFilterHdlrHelper_SetFilterOldValueX(idxFilterInfo, SignalDataPtr);
}

/**********************************************************************************************************************
  Com_SignalFilterHdlr_GetOldValuePtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(P2CONST(void, AUTOMATIC, AUTOMATIC), COM_CODE) Com_SignalFilterHdlr_GetOldValuePtr(Com_FilterInfoIterType idxFilterInfo)
{
  return Com_SignalFilterHdlrHelper_GetOldValuePtr(idxFilterInfo);
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
 *  END OF FILE: Com_SignalFilterHdlr.c
 *********************************************************************************************************************/
