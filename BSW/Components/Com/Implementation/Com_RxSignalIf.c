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
/*!        \file  Com_RxSignalIf.c
 *         \unit  RxSignalIf
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_RXSIGNALIF_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2983, 2984, 2985, 2986, 2987, 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */
/* PRQA  S 6050 EOF */ /* MD_MSR_STCAL */
/* PRQA  S 883, 3406 EOF */ /* MD_Com_UnityBuild */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com.h"
#include "Com_RxSignalIf.h"
#include "Com_Cfg_ReceptionProcessingData.h"

#include "Com_IPduGroupHdlr.h"
#include "Com_RxSigBuffer.h"

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
  LOCAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

/**********************************************************************************************************************
  Com_RxSignalIf_ReceiveSignalProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_RxSignalIf_ReceiveSignalProcessing(Com_RxAccessInfoIterType SignalId, P2VAR(void, AUTOMATIC, AUTOMATIC) SignalDataPtr)
{
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* Copy the stored ComSignal value to the passed SignalDataPtr */
  Com_RxSigBuffer_CopyFromRxSigBufferToSignalDataPtr(SignalId, SignalDataPtr);

  /* If the related ComIPduGroup is active, E_OK will be returned otherwise COM_SERVICE_NOT_AVAILABLE will be returned. */
  if(Com_IPduGroupHdlr_IsRxPduActive(Com_GetRxPduInfoIdxOfRxAccessInfo(SignalId)))
  {
    retVal = E_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
  Com_RxSignalIf_ReceiveDynSignalProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_RxSignalIf_ReceiveDynSignalProcessing(Com_RxAccessInfoIterType SignalId, P2VAR(void, AUTOMATIC, AUTOMATIC) SignalDataPtr, P2VAR(uint16, AUTOMATIC, AUTOMATIC) Length)
{
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_RxPduInfoIterType ComPduId = Com_GetRxPduInfoIdxOfRxAccessInfo(SignalId);

  /* If the dynamic ComSignal fits into the passed buffer */
  const PduLengthType dynSignalLength = Com_RxSigBuffer_GetRxDynSignalLength(SignalId);
  if(*Length >= dynSignalLength)
  {
    /* Copy the dynamic ComSignal value to the passed signal data location and set the passed Length pointer to the current dynamic ComSignal length */
    SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
    {
      Com_RxSigBuffer_CopyFromRxSigBufferToSignalDataPtrDynSig(SignalId, SignalDataPtr, dynSignalLength);
      *Length = (uint16) dynSignalLength;
    }
    SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
    if(Com_IPduGroupHdlr_IsRxPduActive(ComPduId))
    {
      retVal = E_OK;
    }
  }
  else
  {
    *Length = (uint16) dynSignalLength;
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
  Com_RxSignalIf_ReceiveSignalGroupProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_RxSignalIf_ReceiveSignalGroupProcessing(Com_RxSigGrpInfoIterType SignalGroupId)
{
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* If the related ComIPduGroup is active, E_OK will be returned. COM_SERVICE_NOT_AVAILABLE will be returned otherwise */
  if(Com_IPduGroupHdlr_IsRxPduActive(Com_GetRxPduInfoIdxOfRxSigGrpInfo(SignalGroupId)))
  {
    retVal = E_OK;
  }

  SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
  {
    /* Copy the SignalGroup data from the ShadowBuffer to the RxSigBuffer */
    for(Com_RxAccessInfoGrpSigIndIterType rxAccessInfoIndIdx = Com_GetRxAccessInfoGrpSigIndStartIdxOfRxSigGrpInfo(SignalGroupId);
        rxAccessInfoIndIdx < Com_GetRxAccessInfoGrpSigIndEndIdxOfRxSigGrpInfo(SignalGroupId); rxAccessInfoIndIdx++)  /* FETA_COM_01 */
    {
      /*@ assert rxAccessInfoIndIdx < Com_GetSizeOfRxAccessInfoGrpSigInd(); */ /* VCA_COM_CSL03 */
      const Com_RxAccessInfoIterType idxRxAccessInfo = Com_GetRxAccessInfoGrpSigInd(rxAccessInfoIndIdx);
      Com_RxSigBuffer_CopyFromShadowBufferToRxSigBuffer(idxRxAccessInfo);
    }
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();

  return retVal;
}

/**********************************************************************************************************************
  Com_RxSignalIf_ReceiveSignalGroupArrayProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(uint8, COM_CODE) Com_RxSignalIf_ReceiveSignalGroupArrayProcessing(Com_RxSigGrpInfoIterType SignalGroupId, P2VAR(uint8, AUTOMATIC, AUTOMATIC) SignalGroupArrayPtr)
{
  uint8 retVal = COM_SERVICE_NOT_AVAILABLE;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  const Com_RxPduInfoIterType ComPduId = Com_GetRxPduInfoIdxOfRxSigGrpInfo(SignalGroupId);

  /* If the related RxPduGroup is active, E_OK will be returned. COM_SERVICE_NOT_AVAILABLE will be returned otherwise. */
  if(Com_IPduGroupHdlr_IsRxPduActive(ComPduId))
  {
    retVal = E_OK;
  }

  SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
  {
    /* Copy data from the signal buffer to the SignalGroupArrayPtr */
    Com_RxSigBuffer_CopyFromRxSigBufferToSignalGroupArrayPtr(SignalGroupId, SignalGroupArrayPtr); /* PRQA S 0315 */ /* MD_Com_0314_0315_0316_0317 */
  }
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();

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
 *  END OF FILE: Com_RxSignalIf.c
 *********************************************************************************************************************/
