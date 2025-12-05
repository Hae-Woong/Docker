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
/*!        \file  Com_RxSignalFiltering.c
 *        \unit   RxSignalFiltering
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_RXSIGNALFILTERING_SOURCE

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
#include "Com_Cfg_FilterData.h"
#include "Com_RxSignalFiltering.h"
#include "Com_Cfg_ReceptionProcessingData.h"

#include "Com_SignalFilterHdlr.h"
#include "Com_RxSigBuffer.h"

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
  Com_RxSignalFiltering_SignalFilterCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_RxSignalFiltering_SignalFilterCheck(Com_RxAccessInfoIterType idxRxAccessInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  boolean filterState = TRUE;

  /* If a filter is configured for the ComSignal */
  if(Com_IsFilterInfoUsedOfRxAccessInfo(idxRxAccessInfo))
  {
    /* Evaluate the filter and return the filterState */
    const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert idxFilterInfo < Com_GetSizeOfFilterInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) oldValuePtr = Com_RxSigBuffer_GetSigBufferPtr(idxRxAccessInfo);
    filterState = Com_SignalFilterHdlr_EvaluateFilter(idxFilterInfo, oldValuePtr, comDataPtr);
  }

  return filterState;
}

/**********************************************************************************************************************
  Com_RxSignalFiltering_SignalGroupFilterCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_RxSignalFiltering_SignalGroupFilterCheck(Com_RxSigGrpInfoIterType idxRxSigGrpInfo)
{
  boolean filterState = TRUE;

  /* If a filter is configured for at least one group signal */
  if(Com_IsFilterEventOfRxSigGrpInfo(idxRxSigGrpInfo))
  {
    filterState = FALSE;

    /* Iterate over all ComGroupSignals of the passed ComSignalGroup (If any of the group signal filters evaluates to TRUE, return TRUE, otherwise return FALSE) */
    for(Com_RxAccessInfoGrpSigIndIterType rxAccessInfoIndIdx = Com_GetRxAccessInfoGrpSigIndStartIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
        rxAccessInfoIndIdx < Com_GetRxAccessInfoGrpSigIndEndIdxOfRxSigGrpInfo(idxRxSigGrpInfo); rxAccessInfoIndIdx++)  /* FETA_COM_01 */
    {
      const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoGrpSigInd(rxAccessInfoIndIdx);
      /* If a filter is configured for the current ComGroupSignal */
      if(Com_IsFilterInfoUsedOfRxAccessInfo(idxRxAccessInfo)) /* This Function is only called when the const value IsFilterEvent is true, which is only the case if the SigGrp has any filters configured */
      {
        /* Evaluate the filter */
        CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = Com_RxSigBuffer_GetTmpGrpSigBufferPtr(idxRxAccessInfo);
        const PduLengthType constDataLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);
        const ComDataTypeConst data = { constDataPtr, constDataLength };

        const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfRxAccessInfo(idxRxAccessInfo);
        /*@ assert idxFilterInfo < Com_GetSizeOfFilterInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
        CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) oldValuePtr = Com_RxSigBuffer_GetShdBufferPtr(idxRxAccessInfo);
        filterState = Com_SignalFilterHdlr_EvaluateFilter(idxFilterInfo, oldValuePtr, &data);
      }
      /* If the Filter evaluates to TRUE, stop evaluating the remaining group signals */
      if(filterState)
      {
        break;
      }
    }
  }

  return filterState;
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
 *  END OF FILE: Com_RxSignalFiltering.c
 *********************************************************************************************************************/
