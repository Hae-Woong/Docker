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
 *         \file  PduR_Mm.c
 *         \unit  Mm
 *        \brief  Pdu Router Module Manager source file
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
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

#define PDUR_MM_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Mm.h"
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

/**********************************************************************************************************************
 * PduR_Mm_CallIfTxConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Mm_CallIfTxConfirmation(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd)
{
  PduR_IfTxConfirmationFctPtrType ifTxConfirmationFctPtr = PduR_GetUpIfTxConfirmationFctPtrOfMmRom(mmRomIdx);
  if(ifTxConfirmationFctPtr != NULL_PTR)
  {
    ifTxConfirmationFctPtr (srcHnd);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLIFTXCONFIRMATION, PDUR_E_FATAL);
  }
}

/**********************************************************************************************************************
 * PduR_Mm_CallIfTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallIfTransmit(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_TransmitFctPtrType ifTransmitFctPtr = PduR_GetLoIfTransmitFctPtrOfMmRom(mmRomIdx);
  if(ifTransmitFctPtr != NULL_PTR)
  {
    retVal = ifTransmitFctPtr (destHnd, pduInfo);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLIFTRANSMIT, PDUR_E_FATAL);
  }
  return retVal;
}

/**********************************************************************************************************************
 * PduR_Mm_CallIfCancelTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallIfCancelTransmit(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_CancelTransmitFctPtrType  ifCancelTransmitFctPtr = PduR_GetLoIfCancelTransmitFctPtrOfMmRom(mmRomIdx);
  if(ifCancelTransmitFctPtr != NULL_PTR)
  {
    retVal = ifCancelTransmitFctPtr (destHnd);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLIFCANCELTRANSMIT, PDUR_E_FATAL);
  }
  return retVal;
}

/**********************************************************************************************************************
 * PduR_Mm_CallIfTriggerTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallIfTriggerTransmit(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_TriggerTransmitFctPtrType ifTriggerTransmitFctPtr = PduR_GetUpIfTriggerTransmitFctPtrOfMmRom(mmRomIdx);
  if(ifTriggerTransmitFctPtr != NULL_PTR)
  {
    retVal = ifTriggerTransmitFctPtr (srcHnd, pduInfo);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLIFTRIGGERTRANSMIT, PDUR_E_FATAL);
  }
  return retVal;
}

/**********************************************************************************************************************
 * PduR_Mm_CallIfRxIndication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Mm_CallIfRxIndication(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo)
{
  PduR_IfRxIndicationType ifRxIndicationFctPtr = PduR_GetUpIfRxIndicationFctPtrOfMmRom(mmRomIdx);
  if(ifRxIndicationFctPtr != NULL_PTR)
  {
    ifRxIndicationFctPtr (destHnd, pduInfo);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLIFRXINDICATION, PDUR_E_FATAL);
  }
}

/**********************************************************************************************************************
 * PduR_Mm_CallTpTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallTpTransmit(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_TransmitFctPtrType tpTransmitFctPtr = PduR_GetLoTpTransmitFctPtrOfMmRom(mmRomIdx);
  if(tpTransmitFctPtr != NULL_PTR)
  {
    retVal = tpTransmitFctPtr (destHnd, pduInfo);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLTPTRANSMIT, PDUR_E_FATAL);
  }
  return retVal;
}

/**********************************************************************************************************************
 * PduR_Mm_CallTpCancelTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallTpCancelTransmit(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_CancelTransmitFctPtrType tpCancelTransmitFctPtr = PduR_GetLoTpCancelTransmitFctPtrOfMmRom(mmRomIdx);
  if(tpCancelTransmitFctPtr != NULL_PTR)
  {
    retVal = tpCancelTransmitFctPtr (destHnd);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLTPCANCELTRANSMIT, PDUR_E_FATAL);
  }
  return retVal;
}

/**********************************************************************************************************************
 * PduR_Mm_CallTpCopyTxData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Mm_CallTpCopyTxData(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo, P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retryInfo, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  PduR_CopyTxDataFctPtrType tpCopyTxDataFctPtr = PduR_GetUpTpCopyTxDataFctPtrOfMmRom(mmRomIdx);
  if(tpCopyTxDataFctPtr != NULL_PTR)
  {
    retVal = tpCopyTxDataFctPtr (srcHnd, pduInfo, retryInfo, availableDataPtr);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLTPCOPYTXDATA, PDUR_E_FATAL);
  }
  return retVal;
}

/**********************************************************************************************************************
 * PduR_Mm_CallTpTxConfirmation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Mm_CallTpTxConfirmation(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd, Std_ReturnType result)
{
  PduR_TpTxConfirmationFctPtrType tpTxConfirmationFctPtr = PduR_GetUpTpTpTxConfirmationFctPtrOfMmRom(mmRomIdx);
  if(tpTxConfirmationFctPtr != NULL_PTR)
  {
    tpTxConfirmationFctPtr (srcHnd, result);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLTPTXCONFIRMATION, PDUR_E_FATAL);
  }
}

/**********************************************************************************************************************
 * PduR_Mm_CallTpStartOfReception
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Mm_CallTpStartOfReception(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo, PduLengthType tpSduLength, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  PduR_StartOfReceptionFctPtrType tpStartOfReceptionFctPtr = PduR_GetUpTpStartOfReceptionFctPtrOfMmRom(mmRomIdx);
  if(tpStartOfReceptionFctPtr != NULL_PTR)
  {
    retVal = tpStartOfReceptionFctPtr (destHnd, pduInfo, tpSduLength, bufferSizePtr);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLTPSTARTOFRECEPTION, PDUR_E_FATAL);
  }
  return retVal;
}

/**********************************************************************************************************************
 * PduR_Mm_CallTpRxIndication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Mm_CallTpRxIndication(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, Std_ReturnType result)
{
  PduR_TpRxIndicationFctPtrType tpRxIndicationFctPtr = PduR_GetUpTpTpRxIndicationFctPtrOfMmRom(mmRomIdx);
  if(tpRxIndicationFctPtr != NULL_PTR)
  {
    tpRxIndicationFctPtr (destHnd, result);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLTPRXINDICATION, PDUR_E_FATAL);
  }
}

/**********************************************************************************************************************
 * PduR_Mm_CallTpCopyRxData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_Mm_CallTpCopyRxData(PduR_MmRomIterType mmRomIdx, PduR_DestHndOfRmGDestRomType destHnd, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) pduInfo, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  PduR_CopyRxDataFctPtrType tpCopyRxDataFctPtr = PduR_GetUpTpCopyRxDataFctPtrOfMmRom(mmRomIdx);
  if(tpCopyRxDataFctPtr != NULL_PTR)
  {
    retVal = tpCopyRxDataFctPtr (destHnd, pduInfo, bufferSizePtr);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLTPCOPYRXDATA, PDUR_E_FATAL);
  }
  return retVal;
}

/**********************************************************************************************************************
 * PduR_Mm_CallTpCancelReceive
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallTpCancelReceive(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_CancelReceiveFctPtrType tpCancelReceiveFctPtr = PduR_GetLoTpCancelReceiveFctPtrOfMmRom(mmRomIdx);
  if(tpCancelReceiveFctPtr != NULL_PTR)
  {
    retVal = tpCancelReceiveFctPtr (srcHnd);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLTPCANCELRECEIVE, PDUR_E_FATAL);
  }
  return retVal;
}

/**********************************************************************************************************************
 * PduR_Mm_CallTpChangeParameter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *********************************************************************************************************************/
FUNC(Std_ReturnType, PDUR_CODE) PduR_Mm_CallTpChangeParameter(PduR_MmRomIterType mmRomIdx, PduR_SrcHndOfRmSrcRomType srcHnd, TPParameterType parameter, uint16 value)
{
  Std_ReturnType retVal = E_NOT_OK;
  PduR_ChangeParameterFctPtrType tpChangeParameterFctPtr = PduR_GetLoTpChangeParameterFctPtrOfMmRom(mmRomIdx);
  if(tpChangeParameterFctPtr != NULL_PTR)
  {
    retVal = tpChangeParameterFctPtr (srcHnd, parameter, value);  /* VCA_PDUR_CALLING_UNDEFINED_FUNCTION */
  }
  else
  {
    PduR_Reporting_ReportError(PDUR_MM_CALLTPCHANGEPARAMETER, PDUR_E_FATAL);
  }
  return retVal;
}

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  END OF FILE: PduR_Mm.c
 *********************************************************************************************************************/
