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
/*!        \file  Com_TxInv.c
 *         \unit  TxInv
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_TXINV_SOURCE

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
#include "Com.h"
#include "Com_TxInv.h"
#include "Com_Cfg_InvalidationData.h"
#include "Com_Cfg_TransmissionProcessingData.h"
#include "Com_Cfg_TxBuffer.h"

#include "Com_TxSignalIf.h"
#include "Com_TxBuffer.h"

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
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "Com_MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

/**********************************************************************************************************************
  Com_TxInv_InvalidateSignalOrGroupSignal
**********************************************************************************************************************/
/*! \brief        This function calls Com_TxSignalIf_SendSignal_Processing to send the configured invalid value for
                  signals and group signals.
    \details      -
    \pre          -
    \param[in]    txAccessInfo ID of the access info table.
    \param[in]    idxInvInfo ID of the invalidation info table.
    \return       uint8
                    E_OK                        service has been accepted
                    COM_SERVICE_NOT_AVAILABLE   corresponding I-PDU group was stopped
                                                (or service failed due to development error)
                    COM_BUSY                    in case the TP-Buffer is locked for large data types handling
    \context      TASK|ISR2
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_TxInv_InvalidateSignalOrGroupSignal(Com_TxAccessInfoIterType txAccessInfo, Com_TxInvalidationInfoIterType idxInvInfo);

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxInv_InvalidateSignalOrGroupSignal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
COM_LOCAL_INLINE FUNC(uint8, COM_CODE) Com_TxInv_InvalidateSignalOrGroupSignal(Com_TxAccessInfoIterType txAccessInfo, Com_TxInvalidationInfoIterType idxInvInfo)
{
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  switch (Com_GetInvValueOfTxInvalidationInfo(idxInvInfo))
  {
    case COM_INVVALUEUINT8_INVVALUEOFTXINVALIDATIONINFO:
    {
      const Com_InvValueUInt8IterType invValueIdx = (Com_InvValueUInt8IterType) Com_GetInvValueStartIdxOfTxInvalidationInfo(idxInvInfo);
      Com_InvValueUInt8PtrType invValuePtr = Com_GetAddrInvValueUInt8(invValueIdx);
      const ComDataTypeConst data = { invValuePtr, 0u };
      retVal = Com_TxSignalIf_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_INVVALUESINT8_INVVALUEOFTXINVALIDATIONINFO:
    {
      const Com_InvValueSInt8IterType invValueIdx = (Com_InvValueSInt8IterType) Com_GetInvValueStartIdxOfTxInvalidationInfo(idxInvInfo);
      Com_InvValueSInt8PtrType invValuePtr = Com_GetAddrInvValueSInt8(invValueIdx);
      const ComDataTypeConst data = { invValuePtr, 0u };
      retVal = Com_TxSignalIf_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_INVVALUEUINT16_INVVALUEOFTXINVALIDATIONINFO:
    {
      const Com_InvValueUInt16IterType invValueIdx = (Com_InvValueUInt16IterType) Com_GetInvValueStartIdxOfTxInvalidationInfo(idxInvInfo);
      const uint16 invValue = (uint16) Com_GetInvValueUInt16(invValueIdx); /* Use basic data type instead of ComStackLib data type to avoid ComStackLib optimization */
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = &invValue;
      const ComDataTypeConst data = { constDataPtr, 0u };
      retVal = Com_TxSignalIf_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_INVVALUESINT16_INVVALUEOFTXINVALIDATIONINFO:
    {
      const Com_InvValueSInt16IterType invValueIdx = (Com_InvValueSInt16IterType) Com_GetInvValueStartIdxOfTxInvalidationInfo(idxInvInfo);
      const sint16 invValue = (sint16) Com_GetInvValueSInt16(invValueIdx); /* Use basic data type instead of ComStackLib data type to avoid ComStackLib optimization */
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = &invValue;
      const ComDataTypeConst data = { constDataPtr, 0u };
      retVal = Com_TxSignalIf_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_INVVALUEUINT32_INVVALUEOFTXINVALIDATIONINFO:
    {
      const Com_InvValueUInt32IterType invValueIdx = (Com_InvValueUInt32IterType) Com_GetInvValueStartIdxOfTxInvalidationInfo(idxInvInfo);
      const uint32 invValue = (uint32) Com_GetInvValueUInt32(invValueIdx);  /* Use basic data type instead of ComStackLib data type to avoid ComStackLib optimization */
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = &invValue;
      const ComDataTypeConst data = { constDataPtr, 0u };
      retVal = Com_TxSignalIf_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_INVVALUESINT32_INVVALUEOFTXINVALIDATIONINFO:
    {
      const Com_InvValueSInt32IterType invValueIdx = (Com_InvValueSInt32IterType) Com_GetInvValueStartIdxOfTxInvalidationInfo(idxInvInfo);
      const sint32 invValue = (sint32) Com_GetInvValueSInt32(invValueIdx); /* Use basic data type instead of ComStackLib data type to avoid ComStackLib optimization */
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = &invValue;
      const ComDataTypeConst data = { constDataPtr, 0u };
      retVal = Com_TxSignalIf_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_INVVALUEUINT64_INVVALUEOFTXINVALIDATIONINFO:
    {
      const Com_InvValueUInt64IterType invValueIdx = (Com_InvValueUInt64IterType) Com_GetInvValueStartIdxOfTxInvalidationInfo(idxInvInfo);
      const uint64 invValue = (uint64) Com_GetInvValueUInt64(invValueIdx); /* Use basic data type instead of ComStackLib data type to avoid ComStackLib optimization */
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = &invValue;
      const ComDataTypeConst data = { constDataPtr, 0u };
      retVal = Com_TxSignalIf_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_INVVALUESINT64_INVVALUEOFTXINVALIDATIONINFO:
    {
      const Com_InvValueSInt64IterType invValueIdx = (Com_InvValueSInt64IterType) Com_GetInvValueStartIdxOfTxInvalidationInfo(idxInvInfo);
      const sint64 invValue = (sint64) Com_GetInvValueSInt64(invValueIdx); /* Use basic data type instead of ComStackLib data type to avoid ComStackLib optimization */
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = &invValue;
      const ComDataTypeConst data = { constDataPtr, 0u };
      retVal = Com_TxSignalIf_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_INVVALUEFLOAT32_INVVALUEOFTXINVALIDATIONINFO:
    {
      const Com_InvValueFloat32IterType invValueIdx = (Com_InvValueFloat32IterType) Com_GetInvValueStartIdxOfTxInvalidationInfo(idxInvInfo);
      const float32 invValue = (float32) Com_GetInvValueFloat32(invValueIdx); /* Use basic data type instead of ComStackLib data type to avoid ComStackLib optimization */
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = &invValue;
      const ComDataTypeConst data = { constDataPtr, 0u };
      retVal = Com_TxSignalIf_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_INVVALUEFLOAT64_INVVALUEOFTXINVALIDATIONINFO:
    {
      const Com_InvValueFloat64IterType invValueIdx = (Com_InvValueFloat64IterType) Com_GetInvValueStartIdxOfTxInvalidationInfo(idxInvInfo);
      const float64 invValue = (float64) Com_GetInvValueFloat64(invValueIdx); /* Use basic data type instead of ComStackLib data type to avoid ComStackLib optimization */
      CONSTP2CONST(void, AUTOMATIC, AUTOMATIC) constDataPtr = &invValue;
      const ComDataTypeConst data = { constDataPtr, 0u };
      retVal = Com_TxSignalIf_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    case COM_INVVALUEARRAYBASED_INVVALUEOFTXINVALIDATIONINFO:
    {
      const Com_InvValueArrayBasedIterType invValueIdx = (Com_InvValueArrayBasedIterType) Com_GetInvValueStartIdxOfTxInvalidationInfo(idxInvInfo);
      Com_InvValueArrayBasedPtrType constDataPtr = Com_GetAddrInvValueArrayBased(invValueIdx);
      const PduLengthType constDataLength = (PduLengthType) Com_GetInvValueLengthOfTxInvalidationInfo(idxInvInfo);
      const ComDataTypeConst data = { constDataPtr, constDataLength };
      retVal = Com_TxSignalIf_SendSignal_Processing(txAccessInfo, &data); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
      break;
    }
    default: /* COV_COM_MISRA */
      /* Intentionally Empty */
      break;
  }

  return retVal;
}

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_TxInv_InvalidateSignalGroup
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_TxInv_InvalidateSignalGroup(Com_TxSigGrpInfoIterType SignalGroupId)
{
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
  {
    /* Iterate overall ComGroupSignal of the passed ComSignalGroup and set the shadow buffer to the invalid value */
    for (Com_TxInvalidationInfoIndIterType idxInvInfoInd = Com_GetTxInvalidationInfoIndStartIdxOfTxSigGrpInfo(SignalGroupId);
         idxInvInfoInd < Com_GetTxInvalidationInfoIndEndIdxOfTxSigGrpInfo(SignalGroupId); idxInvInfoInd++)  /* FETA_COM_01 */
    {
      const Com_TxInvalidationInfoIterType idxInvInfo = Com_GetTxInvalidationInfoInd(idxInvInfoInd);
      const Com_TxAccessInfoIterType txAccessInfo = Com_GetTxAccessInfoIdxOfTxInvalidationInfo(idxInvInfo);
      /*@ assert txAccessInfo < Com_GetSizeOfTxAccessInfo(); */ /* VCA_COM_CSL03 */
      (void) Com_TxInv_InvalidateSignalOrGroupSignal(txAccessInfo, idxInvInfo);
    }

    /* Call Com_TxSignalIf_SendSignalGroup_Processing to copy the invalid values form the shadow buffer to the tx pdu buffer */
    P2CONST(uint8, AUTOMATIC, AUTOMATIC) shdBuffer = NULL_PTR;
    if (Com_IsTxBufferInfoUsedOfTxSigGrpInfo(SignalGroupId))
    {
      const Com_TxBufferInfoIterType txBufferInfoIdx = Com_GetTxBufferInfoIdxOfTxSigGrpInfo(SignalGroupId);
      /*@ assert txBufferInfoIdx < Com_GetSizeOfTxBufferInfo(); */ /* VCA_COM_OPTIONAL_INDIRECTION */
      shdBuffer = Com_TxBuffer_GetAddrTxBufferFirstElement(txBufferInfoIdx);
    }
    retVal = Com_TxSignalIf_SendSignalGroup_Processing(SignalGroupId, shdBuffer);
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();

  return retVal;
}

/**********************************************************************************************************************
  Com_TxInv_InvalidateSignal
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 **********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_TxInv_InvalidateSignal(Com_TxAccessInfoIterType txAccessInfo)
{
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
  {
    /* If invalid values are configured, send the invalid value. */
    if(Com_IsTxInvalidationInfoUsedOfTxAccessInfo(txAccessInfo))
    {
      retVal = Com_TxInv_InvalidateSignalOrGroupSignal(txAccessInfo, Com_GetTxInvalidationInfoIdxOfTxAccessInfo(txAccessInfo));
    }
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();

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
 *  END OF FILE: Com_TxInv.c
 *********************************************************************************************************************/
