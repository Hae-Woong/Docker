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
/*!        \file  Com_TxSignalFiltering.c
 *         \unit  TxSignalFiltering
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_TXSIGNALFILTERING_SOURCE

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
#include "Com_TxSignalFiltering.h"
#include "Com_Cfg_FilterData.h"
#include "Com_Cfg_TxSignalFiltering.h"
#include "Com_Cfg_TransmissionProcessingData.h"

#include "Com_SignalFilterHdlr.h"


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
  Com_TxSignalFiltering_FilterCheck
**********************************************************************************************************************/
/*! \brief        This function evaluates the state of the signal filter.
    \details      -
    \pre          -
    \param[in]    idxFilterInfo       Index of related filter info
    \param[in]    newValuePtr         Pointer to the memory where the new signal value is located
    \param[in]    length              length of the to be evaluated signal
    \return       boolean
                    TRUE  if filter condition is passed
                    FALSE otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_TxSignalFiltering_FilterCheck(Com_FilterInfoIterType idxFilterInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxSignalFiltering_FilterCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_TxSignalFiltering_FilterCheck(Com_FilterInfoIterType idxFilterInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  boolean filterState = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */

  CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) oldValuePtr = Com_SignalFilterHdlr_GetOldValuePtr(idxFilterInfo);
  filterState = Com_SignalFilterHdlr_EvaluateFilter(idxFilterInfo, oldValuePtr, comDataPtr);

  return filterState;
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxSignalFiltering_InitTxFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxSignalFiltering_InitTxFilter(Com_TxPduInfoIterType ComPduId)
{
  /* \trace SPEC-2736775 */
  const Com_TxPduFilterInfoIterType txFilterInfoIdx = Com_GetTxPduFilterInfoIdxOfTxPduInfo(ComPduId);

  for(Com_TransmissionModeFilterInfoIndIterType idxTransmissionModeFilterInfoInd = Com_GetTransmissionModeFilterInfoIndStartIdxOfTxPduFilterInfo(txFilterInfoIdx);
      idxTransmissionModeFilterInfoInd < Com_GetTransmissionModeFilterInfoIndEndIdxOfTxPduFilterInfo(txFilterInfoIdx); idxTransmissionModeFilterInfoInd++)  /* FETA_COM_01 */
  {
    const Com_TransmissionModeFilterInfoIterType idxTransmissionModeFilterInfo = Com_GetTransmissionModeFilterInfoInd(idxTransmissionModeFilterInfoInd);
    const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfTransmissionModeFilterInfo(idxTransmissionModeFilterInfo);
    /*@ assert idxFilterInfo < Com_GetSizeOfFilterInfo(); */ /* VCA_COM_CSL03 */
    Com_SignalFilterHdlr_SetFilterOldValueToInitValue(idxFilterInfo);
    /*@ assert idxTransmissionModeFilterInfo < Com_GetSizeOfTransmissionModeFilterInfo(); */ /* VCA_COM_INDIRECTION_CSL02_CSL03 */
    Com_SetTxFilterState(idxTransmissionModeFilterInfo, Com_IsTxFilterInitStateOfTransmissionModeFilterInfo(idxTransmissionModeFilterInfo));
  }
  for(Com_TransferPropertyFilterInfoIndIterType idxTransferPropertyFilterInfoInd = Com_GetTransferPropertyFilterInfoIndStartIdxOfTxPduFilterInfo(txFilterInfoIdx);
      idxTransferPropertyFilterInfoInd < Com_GetTransferPropertyFilterInfoIndEndIdxOfTxPduFilterInfo(txFilterInfoIdx); idxTransferPropertyFilterInfoInd++)  /* FETA_COM_01 */
  {
    const Com_TransferPropertyFilterInfoIterType idxTransferPropertyFilterInfo = Com_GetTransferPropertyFilterInfoInd(idxTransferPropertyFilterInfoInd);
    const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfTransferPropertyFilterInfo(idxTransferPropertyFilterInfo);
    /*@ assert idxFilterInfo < Com_GetSizeOfFilterInfo(); */ /* VCA_COM_CSL03 */
    Com_SignalFilterHdlr_SetFilterOldValueToInitValue(idxFilterInfo);
  }
}

/**********************************************************************************************************************
  Com_TxSignalFiltering_SetTxSignalFilterOldValueX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxSignalFiltering_SetTxSignalFilterOldValueX(Com_TxSigInfoIterType idxTxSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr)
{
  if(SignalDataPtr != NULL_PTR)
  {
    if(Com_IsTransmissionModeFilterInfoUsedOfTxSigInfo(idxTxSigInfo))
    {
      const Com_TransmissionModeFilterInfoIterType idxTransmissionModeFilterInfo = Com_GetTransmissionModeFilterInfoIdxOfTxSigInfo(idxTxSigInfo);
      const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfTransmissionModeFilterInfo(idxTransmissionModeFilterInfo);
      /*@ assert idxFilterInfo < Com_GetSizeOfFilterInfo(); */ /* VCA_COM_CSL03 */
      Com_SignalFilterHdlr_SetFilterOldValueX(idxFilterInfo, SignalDataPtr);
    }
    if(Com_IsTransferPropertyFilterInfoUsedOfTxSigInfo(idxTxSigInfo))
    {
      const Com_TransferPropertyFilterInfoIterType idxTransferPropertyFilterInfo = Com_GetTransferPropertyFilterInfoIdxOfTxSigInfo(idxTxSigInfo);
      const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfTransferPropertyFilterInfo(idxTransferPropertyFilterInfo);
      /*@ assert idxFilterInfo < Com_GetSizeOfFilterInfo(); */ /* VCA_COM_CSL03 */
      Com_SignalFilterHdlr_SetFilterOldValueX(idxFilterInfo, SignalDataPtr);
    }
  }
}

/**********************************************************************************************************************
  Com_TxSignalFiltering_SetTxGroupSignalFilterOldValueX
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxSignalFiltering_SetTxGroupSignalFilterOldValueX(Com_TxGrpSigInfoIterType idxTxGrpSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr)
{
  if(SignalDataPtr != NULL_PTR) 
  {
    if(Com_IsTransmissionModeFilterInfoUsedOfTxGrpSigInfo(idxTxGrpSigInfo))
    {
      const Com_TransmissionModeFilterInfoIterType idxTransmissionModeFilterInfo = Com_GetTransmissionModeFilterInfoIdxOfTxGrpSigInfo(idxTxGrpSigInfo);
      const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfTransmissionModeFilterInfo(idxTransmissionModeFilterInfo);
      Com_SignalFilterHdlr_SetFilterOldValueX(idxFilterInfo, SignalDataPtr);
    }
    if(Com_IsTransferPropertyFilterInfoUsedOfTxGrpSigInfo(idxTxGrpSigInfo))
    {
      const Com_TransferPropertyFilterInfoIterType idxTransferPropertyFilterInfo = Com_GetTransferPropertyFilterInfoIdxOfTxGrpSigInfo(idxTxGrpSigInfo);
      const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfTransferPropertyFilterInfo(idxTransferPropertyFilterInfo);
      /*@ assert idxFilterInfo < Com_GetSizeOfFilterInfo(); */ /* VCA_COM_CSL03 */
      Com_SignalFilterHdlr_SetFilterOldValueX(idxFilterInfo, SignalDataPtr);
    }
  }
}

/**********************************************************************************************************************
  Com_TxSignalFiltering_TransmissionModeSignalFilterCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxSignalFiltering_TransmissionModeSignalFilterCheck(Com_TxSigInfoIterType idxTxSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr)
{
  boolean retVal = FALSE;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* If the passed Tx ComSignal has a filter */
  if(Com_IsTransmissionModeFilterInfoUsedOfTxSigInfo(idxTxSigInfo))
  {
    const Com_TransmissionModeFilterInfoIterType idxTransmissionModeFilterInfo = Com_GetTransmissionModeFilterInfoIdxOfTxSigInfo(idxTxSigInfo);
    const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfTransmissionModeFilterInfo(idxTransmissionModeFilterInfo);

    const PduLengthType constDataLength = Com_GetByteLengthOfTxSigInfo(idxTxSigInfo);
    const ComDataTypeConst data = { SignalDataPtr, constDataLength };

    /* Evaluate the filter state */
    const boolean filterState = Com_TxSignalFiltering_FilterCheck(idxFilterInfo, &data);
    const boolean oldFilterState = Com_IsTxFilterState(idxTransmissionModeFilterInfo);

    /* Update the tx filter state */
    /*@ assert idxTransmissionModeFilterInfo < Com_GetSizeOfTransmissionModeFilterInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_SetTxFilterState(idxTransmissionModeFilterInfo, filterState);

    /* If the new filter state is different from the old filter state, return true */
    if(oldFilterState != filterState)
    {
      retVal = TRUE;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_TxSignalFiltering_SignalTransferPropertySignalFilterCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxSignalFiltering_SignalTransferPropertySignalFilterCheck(Com_TxSigInfoIterType idxTxSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr)
{
  boolean retVal = FALSE;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_IsTransferPropertyFilterInfoUsedOfTxSigInfo(idxTxSigInfo))
  {
    const Com_TransferPropertyFilterInfoIterType idxTransferPropertyFilterInfo = Com_GetTransferPropertyFilterInfoIdxOfTxSigInfo(idxTxSigInfo);
    const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfTransferPropertyFilterInfo(idxTransferPropertyFilterInfo);
    const PduLengthType constDataLength = Com_GetByteLengthOfTxSigInfo(idxTxSigInfo);
    const ComDataTypeConst data = { SignalDataPtr, constDataLength };
    retVal = Com_TxSignalFiltering_FilterCheck(idxFilterInfo, &data);
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_TxSignalFiltering_TransmissionModeGroupSignalFilterCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxSignalFiltering_TransmissionModeGroupSignalFilterCheck(Com_TxGrpSigInfoIterType idxTxGrpSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr)
{
  /* If the passed Tx ComGroupSignal has a filter */
  if(Com_IsTransmissionModeFilterInfoUsedOfTxGrpSigInfo(idxTxGrpSigInfo))
  {
    const Com_TransmissionModeFilterInfoIterType idxTransmissionModeFilterInfo = Com_GetTransmissionModeFilterInfoIdxOfTxGrpSigInfo(idxTxGrpSigInfo);
    const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfTransmissionModeFilterInfo(idxTransmissionModeFilterInfo);

    /* Evaluate the filter state */
    const PduLengthType constDataLength = Com_GetByteLengthOfTxGrpSigInfo(idxTxGrpSigInfo);
    const ComDataTypeConst data = { SignalDataPtr, constDataLength };

    const boolean filterState = Com_TxSignalFiltering_FilterCheck(idxFilterInfo, &data);

    /* Update the tx filter state */
    /*@ assert idxTransmissionModeFilterInfo < Com_GetSizeOfTransmissionModeFilterInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_SetTxFilterState(idxTransmissionModeFilterInfo, filterState);
  }
}

/**********************************************************************************************************************
  Com_TxSignalFiltering_SignalTransferPropertyGroupSignalFilterCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxSignalFiltering_SignalTransferPropertyGroupSignalFilterCheck(Com_TxGrpSigInfoIterType idxTxGrpSigInfo, P2CONST(void, AUTOMATIC, AUTOMATIC) SignalDataPtr)
{
  boolean retVal = FALSE;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(Com_IsTransferPropertyFilterInfoUsedOfTxGrpSigInfo(idxTxGrpSigInfo))
  {
    const Com_TransferPropertyFilterInfoIterType idxTransferPropertyFilterInfo = Com_GetTransferPropertyFilterInfoIdxOfTxGrpSigInfo(idxTxGrpSigInfo);
    const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfTransferPropertyFilterInfo(idxTransferPropertyFilterInfo);
    const PduLengthType constDataLength = Com_GetByteLengthOfTxGrpSigInfo(idxTxGrpSigInfo);
    const ComDataTypeConst data = { SignalDataPtr, constDataLength };
    retVal = Com_TxSignalFiltering_FilterCheck(idxFilterInfo, &data);
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_TxSignalFiltering_SignalGroupArrayEvaluateFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxSignalFiltering_SignalGroupArrayEvaluateFilter(Com_TxSigGrpInfoIterType idxTxSigGrpInfo, P2CONST(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr)
{
  boolean filterState = FALSE; /* PRQA S 2983 */ /* MD_Com_2982_2983 */

  /* Evaluate the filter for each GroupSignal */
  for(Com_TransmissionModeFilterInfoIterType idxTransmissionModeFilterInfo = Com_GetTransmissionModeFilterInfoStartIdxOfTxSigGrpInfo(idxTxSigGrpInfo);
      idxTransmissionModeFilterInfo < Com_GetTransmissionModeFilterInfoEndIdxOfTxSigGrpInfo(idxTxSigGrpInfo); idxTransmissionModeFilterInfo++)  /* FETA_COM_01 */
  {
    /* If for a GroupSignal the filter state is TRUE, stop the evaluation */
    const Com_FilterInfoIterType idxFilterInfo = Com_GetFilterInfoIdxOfTransmissionModeFilterInfo(idxTransmissionModeFilterInfo);
    filterState = Com_SignalFilterHdlr_EvaluateSigGrpArrFilter(idxFilterInfo, SignalGroupArrayPtr);
    if(filterState)
    {
      break;
    }
  }

  /* Update the tx filterState of the SignalGroup for each GroupSignal */
  for(Com_TransmissionModeFilterInfoIterType idxTransmissionModeFilterInfo = Com_GetTransmissionModeFilterInfoStartIdxOfTxSigGrpInfo(idxTxSigGrpInfo);
      idxTransmissionModeFilterInfo < Com_GetTransmissionModeFilterInfoEndIdxOfTxSigGrpInfo(idxTxSigGrpInfo); idxTransmissionModeFilterInfo++)  /* FETA_COM_01 */
  {
    /*@ assert idxTransmissionModeFilterInfo < Com_GetSizeOfTransmissionModeFilterInfo(); */ /* VCA_COM_INDIRECTION_CSL02_CSL03 */
    Com_SetTxFilterState(idxTransmissionModeFilterInfo, filterState);
  }
}

/**********************************************************************************************************************
  Com_TxSignalFiltering_IsFilterStateTrue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxSignalFiltering_IsFilterStateTrue(Com_TxPduInfoIterType idxTxPduInfo)
{
  boolean retVal = FALSE;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_TxPduFilterInfoIterType txFilterInfoIdx = Com_GetTxPduFilterInfoIdxOfTxPduInfo(idxTxPduInfo);

  for(Com_TransmissionModeFilterInfoIndIterType idxTransmissionModeFilterInfoInd = Com_GetTransmissionModeFilterInfoIndStartIdxOfTxPduFilterInfo(txFilterInfoIdx);
      idxTransmissionModeFilterInfoInd < Com_GetTransmissionModeFilterInfoIndEndIdxOfTxPduFilterInfo(txFilterInfoIdx); idxTransmissionModeFilterInfoInd++)  /* FETA_COM_01 */
  {
    const Com_TransmissionModeFilterInfoIterType idxTransmissionModeFilterInfo = Com_GetTransmissionModeFilterInfoInd(idxTransmissionModeFilterInfoInd);
    if(Com_IsTxFilterState(idxTransmissionModeFilterInfo))
    {
      retVal = TRUE;
      break;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
  Com_TxSignalFiltering_HasTransmissionModeFilter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_TxSignalFiltering_HasTransmissionModeFilter(Com_TxPduInfoIterType idxTxPduInfo)
{
  boolean retVal = FALSE; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_TxPduFilterInfoIterType txFilterInfoIdx = Com_GetTxPduFilterInfoIdxOfTxPduInfo(idxTxPduInfo);

  retVal = Com_IsTransmissionModeFilterInfoIndUsedOfTxPduFilterInfo(txFilterInfoIdx);

  return retVal;
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
 *  END OF FILE: Com_TxFiltering.c
 *********************************************************************************************************************/
