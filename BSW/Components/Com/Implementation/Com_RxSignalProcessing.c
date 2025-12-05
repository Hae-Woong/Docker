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
/*!        \file  Com_RxSignalProcessing.c
 *         \unit  RxSignalProcessing
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_RXSIGNALPROCESSING_SOURCE

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
#include "Com_RxSignalProcessing.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_RxDlMon.h" /* Com_GetSizeOfRxTOutInfo() for VCA */
#include "Com_Cfg_Reporting.h" /* Com_GetSizeOfReporting() for VCA */

#include "Com_RxInv.h"
#include "Com_RxDlMon.h"
#include "Com_RxSigBuffer.h"
#include "Com_SigGw.h"
#include "Com_CheckUpdateBit.h"
#include "Com_RxSignalFiltering.h"
#include "Com_Notifications.h"

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
  Com_RxSignalProcessing_ProcessSigEvent
**********************************************************************************************************************/
/*! \brief              This function processes the passed ComSignal.
    \details            -
    \pre                -
    \param[in]          comRxPduInfoIdx     Index of RxPduInfo
    \param[in]          PduInfoPtr          contains the SDU buffer and the SDU length.
    \param[in]          idxRxSigInfo        Index of RxSigInfo.
    \param[in,out]      fctPtrCacheStrctPtr Pointer to function ptr cache containing all informations (location, size, index)
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxSignalProcessing_ProcessSigEvent(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                                             Com_RxSigInfoIterType idxRxSigInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr);

/**********************************************************************************************************************
  Com_RxSignalProcessing_ProcessSigGrpEvent
**********************************************************************************************************************/
/*! \brief              This function processes the passed ComSignalGroup.
    \details            -
    \pre                -
    \param[in]          comRxPduInfoIdx     Index of RxPduInfo
    \param[in]          PduInfoPtr          contains the SDU buffer and the SDU length.
    \param[in]          idxRxSigGrpInfo     Index of RxSigGrpInfo.
    \param[in,out]      fctPtrCacheStrctPtr Pointer to function ptr cache containing all informations (location, size, index)
    \trace              SPEC-2736832
    \context            TASK|ISR2
    \synchronous        TRUE
    \reentrant          TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxSignalProcessing_ProcessSigGrpEvent(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                                                Com_RxSigGrpInfoIterType idxRxSigGrpInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxSignalProcessing_ProcessSigEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxSignalProcessing_ProcessSigEvent(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                                             Com_RxSigInfoIterType idxRxSigInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr)
{
  const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoIdxOfRxSigInfo(idxRxSigInfo);

  /* Read the ComSignal value form the passed ComIPdu */
  CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = Com_RxSigBuffer_ReadSignal(PduInfoPtr, idxRxAccessInfo);
  ComDataTypeConst data = { constDataPtr, PduInfoPtr->SduLength };

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
  Com_RxSigBuffer_CopySigToMinProcGwBuffer(idxRxAccessInfo, data.dataPtr);
#endif

  /* Evaluate whether the new value of the ComSignal is an invalid value */

  const boolean isValidValue = Com_RxInv_EvaluateSigValidity(idxRxSigInfo, fctPtrCacheStrctPtr, &data);

  /* If the new value of the ComSignal is valid */
  if(isValidValue)
  {
    /* Evaluate the current filter state */
    data.dataLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);
    const boolean isFilterPassed = Com_RxSignalFiltering_SignalFilterCheck(idxRxAccessInfo, &data);

    /* If the filter evaluates to true */
    if(isFilterPassed)
    {
      /* Copy the signal data from the tmp buffer to the signal buffer */
      data.dataLength = Com_RxSigBuffer_GetRxDynamicLength(PduInfoPtr->SduLength, idxRxAccessInfo);
      Com_RxSigBuffer_CopyToSigBuffer(idxRxAccessInfo, &data);

      /* If the ComSignal has a timeout and an UpdateBit, remove the timeout occurred flag */
      if(Com_IsRxTOutInfoUsedOfRxSigInfo(idxRxSigInfo) && Com_IsUpdateBitInfoUsedOfRxSigInfo(idxRxSigInfo))
      {
        const Com_RxTOutInfoIdxOfRxSigInfoType rxTOutInfoIdx = Com_GetRxTOutInfoIdxOfRxSigInfo(idxRxSigInfo);
        /*@ assert rxTOutInfoIdx < Com_GetSizeOfRxTOutInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
        Com_RxDlMon_RemoveOccurredFlag(rxTOutInfoIdx);
      }

      /* If the passed Rx ComSignal uses a notification function, cache or call that function */
      if(Com_IsReportingIndicationNotificationUsedOfRxSigInfo(idxRxSigInfo))
      {
        const Com_ReportingIndicationNotificationIdxOfRxSigInfoType reportingIndicationNotificationIdx = Com_GetReportingIndicationNotificationIdxOfRxSigInfo(idxRxSigInfo);
        /*@ assert reportingIndicationNotificationIdx < Com_GetSizeOfReporting(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
        Com_Notifications_CacheOrCallRxCbkFctPtr(reportingIndicationNotificationIdx, comRxPduInfoIdx, fctPtrCacheStrctPtr);
      }
    }
  }
}

/**********************************************************************************************************************
  Com_RxSignalProcessing_ProcessSigGrpEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxSignalProcessing_ProcessSigGrpEvent(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, Com_RxSigGrpInfoIterType idxRxSigGrpInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr)
{
  /* Read the ComSignalGroup values form the passed ComIPdu */
  Com_RxSigBuffer_ReadSignalGroup(PduInfoPtr, idxRxSigGrpInfo);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
  Com_RxSigBuffer_CopySigGrpToMinProcGwBuffer(idxRxSigGrpInfo);
#endif

  /* \trace SPEC-2736832 */
  /* Evaluate whether any new value of the related ComGroupSignals is an invalid value */
  const boolean isValidValue = Com_RxInv_EvaluateSigGrpValidity(idxRxSigGrpInfo, fctPtrCacheStrctPtr, PduInfoPtr->SduLength);

  /* If the new value of all related ComGroupSignal is valid */
  if(isValidValue)
  {
    /* Evaluate the current filter state */
    const boolean isFilterPassed = Com_RxSignalFiltering_SignalGroupFilterCheck(idxRxSigGrpInfo);

    /* If at least one ComGroupSignal filter evaluates to true */
    if(isFilterPassed)
    {
      /* Copy the signal group data from the tmp buffer to the shadow buffer */
      Com_RxSigBuffer_CopyToShdwBuffer(idxRxSigGrpInfo, PduInfoPtr);

      /* If the ComSignalGroup has a timeout and an UpdateBit, remove the timeout occurred flag */
      if(Com_IsRxTOutInfoUsedOfRxSigGrpInfo(idxRxSigGrpInfo) && Com_IsUpdateBitInfoUsedOfRxSigGrpInfo(idxRxSigGrpInfo))
      {
        const Com_RxTOutInfoIdxOfRxSigGrpInfoType rxTOutInfoIdx = Com_GetRxTOutInfoIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
        /*@ assert rxTOutInfoIdx < Com_GetSizeOfRxTOutInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
        Com_RxDlMon_RemoveOccurredFlag(rxTOutInfoIdx);
      }
      /* If the ComSignalGroup has notifications, cache or call the related Rx notification functions */
      if(Com_IsReportingIndicationNotificationUsedOfRxSigGrpInfo(idxRxSigGrpInfo))
      {
        const Com_ReportingIndicationNotificationIdxOfRxSigGrpInfoType reportingIndicationNotificationIdx = Com_GetReportingIndicationNotificationIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
        /*@ assert reportingIndicationNotificationIdx < Com_GetSizeOfReporting(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
        Com_Notifications_CacheOrCallRxCbkFctPtr(reportingIndicationNotificationIdx, comRxPduInfoIdx, fctPtrCacheStrctPtr);
      }
    }
  }
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxSignalProcessing_RxPduSigEvent
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
FUNC(void, COM_CODE) Com_RxSignalProcessing_RxPduSigEvent(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr)
{
  /* Iterate over all Rx ComSignals of the passed Rx ComIPdu */
  for(Com_RxSigInfoIterType idxRxSigInfo = Com_GetRxSigInfoStartIdxOfRxPduInfo(comRxPduInfoIdx);
      idxRxSigInfo < Com_GetRxSigInfoEndIdxOfRxPduInfo(comRxPduInfoIdx); idxRxSigInfo++)  /* FETA_COM_01 */
  {
    /*@ assert idxRxSigInfo < Com_GetSizeOfRxSigInfo(); */ /* VCA_COM_CSL03 */
    /* If the Rx ComSignal is completely contained in the passed Sdu data length */
    /* \trace SPEC-2736852 */
    if(PduInfoPtr->SduLength >= Com_GetValidDlcOfRxSigInfo(idxRxSigInfo))
    {
      /* If the ComSignal was updated */
      const boolean process = Com_CheckUpdateBit_Signal(idxRxSigInfo, PduInfoPtr);

      if(process)
      {
        /* Set configured timeout event */
        /* \trace SPEC-2736871 */
        Com_RxDlMon_SigEvent(idxRxSigInfo);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
        /* Set configured routing event */
        Com_SigGw_SigEvent(idxRxSigInfo);
#endif
        /* Do Rx Signal Processing */
        Com_RxSignalProcessing_ProcessSigEvent(comRxPduInfoIdx, PduInfoPtr, idxRxSigInfo, fctPtrCacheStrctPtr);
      }
    }
  }
}

/**********************************************************************************************************************
  Com_RxSignalProcessing_RxPduSigGrpEvent
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
FUNC(void, COM_CODE) Com_RxSignalProcessing_RxPduSigGrpEvent(Com_RxPduInfoIterType comRxPduInfoIdx, CONSTP2CONST(PduInfoType, AUTOMATIC, AUTOMATIC) PduInfoPtr,
                                                             CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr)
{
  /* Iterate over all ComSignalGroups of the passed ComIPdu */
  for(Com_RxSigGrpInfoIndIterType idxRxSigGrpInfoInd = Com_GetRxSigGrpInfoIndStartIdxOfRxPduInfo(comRxPduInfoIdx);
      idxRxSigGrpInfoInd < Com_GetRxSigGrpInfoIndEndIdxOfRxPduInfo(comRxPduInfoIdx); idxRxSigGrpInfoInd++)  /* FETA_COM_01 */
  {
    /*@ assert idxRxSigGrpInfoInd < Com_GetSizeOfRxSigGrpInfoInd(); */ /* VCA_COM_CSL03 */
    const Com_RxSigGrpInfoIterType idxRxSigGrpInfo = Com_GetRxSigGrpInfoInd(idxRxSigGrpInfoInd);
    /*@ assert idxRxSigGrpInfo < Com_GetSizeOfRxSigGrpInfo(); */ /* VCA_COM_CSL03 */

    /* If the ComSignalGroup is completely contained in received data length */
    /* \trace SPEC-2736854 */
    if(PduInfoPtr->SduLength >= Com_GetValidDlcOfRxSigGrpInfo(idxRxSigGrpInfo))
    {

      /* If the ComSignal was updated */
      const boolean process = Com_CheckUpdateBit_SignalGroup(PduInfoPtr, idxRxSigGrpInfo);

      if(process)
      {
        /* Set configured timeout event */
        /* \trace SPEC-2736871 */
        Com_RxDlMon_SigGrpEvent(idxRxSigGrpInfo);

#if (COM_MAIN_FUNCTION_ROUTE_SIGNALS_API == STD_ON)
        /* Set configured routing event */
        Com_SigGw_SigGrpEvent(idxRxSigGrpInfo);
#endif
        /* Do Rx ComSignalGroup processing */
        Com_RxSignalProcessing_ProcessSigGrpEvent(comRxPduInfoIdx, PduInfoPtr, idxRxSigGrpInfo, fctPtrCacheStrctPtr);
      }
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
 *  END OF FILE: Com_RxSignalProcessing.c
 *********************************************************************************************************************/
