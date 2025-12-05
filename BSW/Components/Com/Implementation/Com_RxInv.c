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
/*!        \file  Com_RxInv.c
 *         \unit  RxInv
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_RXINV_SOURCE

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
#include "Com_RxInv.h"
#include "Com_Cfg_InvalidationData.h"
#include "Com_Cfg_ReceptionProcessingData.h"
#include "Com_Cfg_Reporting.h" /* Com_GetSizeOfReporting() for VCA */

#include "Com_RxSigBuffer.h"
#include "Com_Util.h"
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
  Com_RxInv_IsSigValueInvalid
**********************************************************************************************************************/
/*! \brief        This function checks if an invalid value has been received.
    \details      -
    \pre          -
    \param[in]    idxRxAccessInfo     Index of RxAccess Info
    \param[in]    idxInvalidationInfo Index of Invalidation Info
    \param[in]    comDataPtr          Pointer to the memory where the tmp signal value is located and length of the received Sdu
    \return       boolean
                    TRUE   if the new Value equals an invalid value
                    FALSE  otherwise
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_RxInv_IsSigValueInvalid(Com_RxAccessInfoIterType idxRxAccessInfo, Com_RxInvalidationInfoIterType idxInvalidationInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr);

/**********************************************************************************************************************
  Com_RxInv_SigGrpPerformInvalidAction
**********************************************************************************************************************/
/*! \brief             This function performs the configured invalid action (replace/notify) for a Signal Group.
    \details           -
    \pre               -
    \param[in]         idxRxSigGrpInfo      Index of RxSigGrpInfo
    \param[in]         idxInvalidationInfo  Index of Invalidation Info
    \param[in,out]     fctPtrCacheStrctPtr  Pointer containing all information of function pointer cache
                                            (size, location, index)
    \context           TASK|ISR2
    \synchronous       TRUE
    \reentrant         TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxInv_SigGrpPerformInvalidAction(Com_RxSigGrpInfoIterType idxRxSigGrpInfo, Com_RxInvalidationInfoIterType idxInvalidationInfo,
                                                                           CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr);

/**********************************************************************************************************************
  Com_RxInv_SigPerformInvalidAction
**********************************************************************************************************************/
/*! \brief            This function performs the configured invalid action (replace/notify) for a Signal.
    \details          -
    \pre              -
    \param[in]        idxRxAccessInfo      Index of RxAccess Info
    \param[in]        idxInvalidationInfo  Index of Invalidation Info
    \param[in,out]    fctPtrCacheStrctPtr  Pointer to function pointer struct containing all cache management attributes
                                           (Cache location, size, index).
    \context         TASK|ISR2
    \synchronous     TRUE
    \reentrant       TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxInv_SigPerformInvalidAction(Com_RxAccessInfoIterType idxRxAccessInfo, Com_RxInvalidationInfoIterType idxInvalidationInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr);


/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxInv_IsSigValueInvalid
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_RxInv_IsSigValueInvalid(Com_RxAccessInfoIterType idxRxAccessInfo, Com_RxInvalidationInfoIterType idxInvalidationInfo, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  boolean retVal = FALSE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if(comDataPtr->dataPtr != NULL_PTR)
  {
    switch (Com_GetInvValueOfRxInvalidationInfo(idxInvalidationInfo))
    {
      case COM_INVVALUEUINT8_INVVALUEOFRXINVALIDATIONINFO:
      {
        const Com_InvValueUInt8IterType invValueIdx = (Com_InvValueUInt8IterType) Com_GetInvValueStartIdxOfRxInvalidationInfo(idxInvalidationInfo);
        const Com_InvValueUInt8Type tmpNewValue = *((P2CONST(Com_InvValueUInt8Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        retVal = (tmpNewValue == Com_GetInvValueUInt8(invValueIdx));
        break;
      }
      case COM_INVVALUESINT8_INVVALUEOFRXINVALIDATIONINFO:
      {
        const Com_InvValueSInt8IterType invValueIdx = (Com_InvValueSInt8IterType) Com_GetInvValueStartIdxOfRxInvalidationInfo(idxInvalidationInfo);
        const Com_InvValueSInt8Type tmpNewValue = *((P2CONST(Com_InvValueSInt8Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        retVal = (tmpNewValue == Com_GetInvValueSInt8(invValueIdx));
        break;
      }
      case COM_INVVALUEUINT16_INVVALUEOFRXINVALIDATIONINFO:
      {
        const Com_InvValueUInt16IterType invValueIdx = (Com_InvValueUInt16IterType) Com_GetInvValueStartIdxOfRxInvalidationInfo(idxInvalidationInfo);
        const Com_InvValueUInt16Type tmpNewValue = *((P2CONST(Com_InvValueUInt16Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        retVal = (tmpNewValue == Com_GetInvValueUInt16(invValueIdx));
        break;
      }
      case COM_INVVALUESINT16_INVVALUEOFRXINVALIDATIONINFO:
      {
        const Com_InvValueSInt16IterType invValueIdx = (Com_InvValueSInt16IterType) Com_GetInvValueStartIdxOfRxInvalidationInfo(idxInvalidationInfo);
        const Com_InvValueSInt16Type tmpNewValue = *((P2CONST(Com_InvValueSInt16Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        retVal = (tmpNewValue == Com_GetInvValueSInt16(invValueIdx));
        break;
      }
      case COM_INVVALUEUINT32_INVVALUEOFRXINVALIDATIONINFO:
      {
        const Com_InvValueUInt32IterType invValueIdx = (Com_InvValueUInt32IterType) Com_GetInvValueStartIdxOfRxInvalidationInfo(idxInvalidationInfo);
        const Com_InvValueUInt32Type tmpNewValue = *((P2CONST(Com_InvValueUInt32Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        retVal = (tmpNewValue == Com_GetInvValueUInt32(invValueIdx));
        break;
      }
      case COM_INVVALUESINT32_INVVALUEOFRXINVALIDATIONINFO:
      {
        const Com_InvValueSInt32IterType invValueIdx = (Com_InvValueSInt32IterType) Com_GetInvValueStartIdxOfRxInvalidationInfo(idxInvalidationInfo);
        const Com_InvValueSInt32Type tmpNewValue = *((P2CONST(Com_InvValueSInt32Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        retVal = (tmpNewValue == Com_GetInvValueSInt32(invValueIdx));
        break;
      }
      case COM_INVVALUEUINT64_INVVALUEOFRXINVALIDATIONINFO:
      {
        const Com_InvValueUInt64IterType invValueIdx = (Com_InvValueUInt64IterType) Com_GetInvValueStartIdxOfRxInvalidationInfo(idxInvalidationInfo);
        const Com_InvValueUInt64Type tmpNewValue = *((P2CONST(Com_InvValueUInt64Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        retVal = (tmpNewValue == Com_GetInvValueUInt64(invValueIdx));
        break;
      }
      case COM_INVVALUESINT64_INVVALUEOFRXINVALIDATIONINFO:
      {
        const Com_InvValueSInt64IterType invValueIdx = (Com_InvValueSInt64IterType) Com_GetInvValueStartIdxOfRxInvalidationInfo(idxInvalidationInfo);
        const Com_InvValueSInt64Type tmpNewValue = *((P2CONST(Com_InvValueSInt64Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        retVal = (tmpNewValue == Com_GetInvValueSInt64(invValueIdx));
        break;
      }
      case COM_INVVALUEFLOAT32_INVVALUEOFRXINVALIDATIONINFO:
      {
        const Com_InvValueFloat32IterType invValueIdx = (Com_InvValueFloat32IterType) Com_GetInvValueStartIdxOfRxInvalidationInfo(idxInvalidationInfo);
        const Com_InvValueFloat32Type tmpNewValue = *((P2CONST(Com_InvValueFloat32Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        retVal = Com_Util_CompareFloat32Values(tmpNewValue, Com_GetInvValueFloat32(invValueIdx));
        break;
      }
      case COM_INVVALUEFLOAT64_INVVALUEOFRXINVALIDATIONINFO:
      {

        const Com_InvValueFloat64IterType invValueIdx = (Com_InvValueFloat64IterType) Com_GetInvValueStartIdxOfRxInvalidationInfo(idxInvalidationInfo);
        const Com_InvValueFloat64Type tmpNewValue = *((P2CONST(Com_InvValueFloat64Type, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
        retVal = Com_Util_CompareFloat64Values(tmpNewValue, Com_GetInvValueFloat64(invValueIdx));
        break;
      }
      case COM_INVVALUEARRAYBASED_INVVALUEOFRXINVALIDATIONINFO:
      {
        const Com_InvValueArrayBasedIterType invValueIdx = (Com_InvValueArrayBasedIterType) Com_GetInvValueStartIdxOfRxInvalidationInfo(idxInvalidationInfo);
        switch (Com_GetApplTypeOfRxAccessInfo(idxRxAccessInfo))
        {
          case COM_UINT8_N_APPLTYPEOFRXACCESSINFO:
          {
            CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) invValuePtr = Com_GetAddrInvValueArrayBased(invValueIdx);  /* PRQA S 0317 */ /* MD_Com_0314_0315_0316_0317 */
            retVal = Com_Util_CompareArrayBasedValues((CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr, invValuePtr, (PduLengthType) Com_GetInvValueLengthOfRxInvalidationInfo(idxInvalidationInfo)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
            break;
          }
          case COM_UINT8_DYN_APPLTYPEOFRXACCESSINFO:
          {
            if(Com_RxSigBuffer_GetRxDynamicLength(comDataPtr->dataLength, idxRxAccessInfo) == (PduLengthType) Com_GetInvValueLengthOfRxInvalidationInfo(idxInvalidationInfo))
            {
              CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC) invValuePtr = Com_GetAddrInvValueArrayBased(invValueIdx); /* PRQA S 0317 */ /* MD_Com_0314_0315_0316_0317 */
              retVal = Com_Util_CompareArrayBasedValues((CONSTP2CONST(uint8, AUTOMATIC, AUTOMATIC)) comDataPtr->dataPtr, invValuePtr, (PduLengthType) Com_GetInvValueLengthOfRxInvalidationInfo(idxInvalidationInfo)); /* PRQA S 0316 */ /* MD_Com_0314_0315_0316_0317 */
            }
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

  return retVal;
}

/**********************************************************************************************************************
  Com_RxInv_SigGrpPerformInvalidAction
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxInv_SigGrpPerformInvalidAction(Com_RxSigGrpInfoIterType idxRxSigGrpInfo, Com_RxInvalidationInfoIterType idxInvalidationInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr)
{
  if(Com_IsReportingInvalidNotificationUsedOfRxInvalidationInfo(idxInvalidationInfo))
  {
    const Com_ReportingInvalidNotificationIdxOfRxInvalidationInfoType reportingInvalidNotificationIdx = Com_GetReportingInvalidNotificationIdxOfRxInvalidationInfo(idxInvalidationInfo);
    const Com_RxPduInfoIdxOfRxSigGrpInfoType rxPduInfoIdx =  Com_GetRxPduInfoIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
    /*@ assert reportingInvalidNotificationIdx < Com_GetSizeOfReporting(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_Notifications_CacheOrCallRxCbkFctPtr(reportingInvalidNotificationIdx, rxPduInfoIdx, fctPtrCacheStrctPtr);
  }
  else
  {
    for(Com_RxAccessInfoGrpSigIndIterType rxAccessInfoIndIdx = Com_GetRxAccessInfoGrpSigIndStartIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
        rxAccessInfoIndIdx < Com_GetRxAccessInfoGrpSigIndEndIdxOfRxSigGrpInfo(idxRxSigGrpInfo); rxAccessInfoIndIdx++)  /* FETA_COM_01 */
    {
      const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoGrpSigInd(rxAccessInfoIndIdx);
      Com_RxSigBuffer_SetRxInitValue(idxRxAccessInfo);
    }
  }
}

/**********************************************************************************************************************
  Com_RxInv_SigPerformInvalidAction
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_RxInv_SigPerformInvalidAction(Com_RxAccessInfoIterType idxRxAccessInfo, Com_RxInvalidationInfoIterType idxInvalidationInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr)
{
  if(Com_IsReportingInvalidNotificationUsedOfRxInvalidationInfo(idxInvalidationInfo))
  {
    const Com_ReportingInvalidNotificationIdxOfRxInvalidationInfoType reportingInvalidNotificationIdx = Com_GetReportingInvalidNotificationIdxOfRxInvalidationInfo(idxInvalidationInfo);
    const Com_RxPduInfoIdxOfRxAccessInfoType rxPduInfoIdx = Com_GetRxPduInfoIdxOfRxAccessInfo(idxRxAccessInfo);
    /*@ assert reportingInvalidNotificationIdx < Com_GetSizeOfReporting(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
    Com_Notifications_CacheOrCallRxCbkFctPtr(reportingInvalidNotificationIdx, rxPduInfoIdx, fctPtrCacheStrctPtr);
  }
  else
  {
    Com_RxSigBuffer_SetRxInitValue(idxRxAccessInfo);
  }
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxInv_EvaluateSigGrpValidity
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_RxInv_EvaluateSigGrpValidity(Com_RxSigGrpInfoIterType idxRxSigGrpInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr, PduLengthType sduLength)
{
  boolean retVal = TRUE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* If the passed ComSignalGroup has any invalid action configured */
  Com_RxInvalidationInfoIndUsedOfRxSigGrpInfoType rxInvalidationInfoIndUsed = Com_IsRxInvalidationInfoIndUsedOfRxSigGrpInfo(idxRxSigGrpInfo);
  if(rxInvalidationInfoIndUsed)
  {
    /* Iterate over all related ComGroupSignals and evaluate whether the current shadow buffer value contains an invalid value */
    for(Com_RxInvalidationInfoIndIterType invalidationInfoIndIdx = Com_GetRxInvalidationInfoIndStartIdxOfRxSigGrpInfo(idxRxSigGrpInfo);
        invalidationInfoIndIdx < Com_GetRxInvalidationInfoIndEndIdxOfRxSigGrpInfo(idxRxSigGrpInfo); invalidationInfoIndIdx++)  /* FETA_COM_01 */
    {
      /*@ assert invalidationInfoIndIdx < Com_GetSizeOfRxInvalidationInfoInd(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      const Com_RxInvalidationInfoIterType idxInvInfo = Com_GetRxInvalidationInfoInd(invalidationInfoIndIdx);
      const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoIdxOfRxInvalidationInfo(idxInvInfo);
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = Com_RxSigBuffer_GetTmpGrpSigBufferPtr(idxRxAccessInfo);
      const ComDataTypeConst data = { constDataPtr, sduLength };
      const boolean isInvalidValue = Com_RxInv_IsSigValueInvalid(idxRxAccessInfo, idxInvInfo, &data);
      if(isInvalidValue)
      {
        /* If any ComGroupSignal has an invalid value in its shadow buffer, perform the invalid action */
        Com_RxInv_SigGrpPerformInvalidAction(idxRxSigGrpInfo, idxInvInfo, fctPtrCacheStrctPtr);
        retVal = FALSE;
        break;
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
  Com_RxInv_EvaluateSigValidity
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(boolean, COM_CODE) Com_RxInv_EvaluateSigValidity(Com_RxSigInfoIterType idxRxSigInfo, CONSTP2VAR(ComCacheType, AUTOMATIC, AUTOMATIC) fctPtrCacheStrctPtr, CONSTP2CONST(ComDataTypeConst, AUTOMATIC, AUTOMATIC) comDataPtr)
{
  boolean retVal = TRUE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* If the passed ComSignal has any invalid action configured */
  if(Com_IsRxInvalidationInfoUsedOfRxSigInfo(idxRxSigInfo))
  {
    const Com_RxInvalidationInfoIterType idxInvInfo = Com_GetRxInvalidationInfoIdxOfRxSigInfo(idxRxSigInfo);
    const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoIdxOfRxInvalidationInfo(idxInvInfo);
    const boolean isInvalidValue = Com_RxInv_IsSigValueInvalid(idxRxAccessInfo, idxInvInfo, comDataPtr);

    /* If the ComSignal has an invalid value in its tmp sig buffer, perform the invalid action */
    if(isInvalidValue)
    {
      retVal = FALSE;
      Com_RxInv_SigPerformInvalidAction(idxRxAccessInfo, idxInvInfo, fctPtrCacheStrctPtr);
    }
  }
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
 *  END OF FILE: Com_RxInv.c
 *********************************************************************************************************************/
