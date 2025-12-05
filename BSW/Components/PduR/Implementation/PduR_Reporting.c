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
 *         \file  PduR_Reporting.c
 *         \unit  Reporting
 *        \brief  Pdu Router Reporting source file
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
/* PRQA  S 3673 EOF */ /* MD_MSR_Rule8.13 */
/* PRQA  S 2991, 2992, 2993, 2994, 2995, 2996, 2997, 2998 EOF */ /* MD_MSR_ConstantCondition */

#define PDUR_REPORTING_SOURCE
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "PduR.h"
#include "PduR_Reporting.h"
#if (PDUR_BSWM_CALLBACKS == STD_ON)
# include "BswM_PduR.h"
#endif
#if (PDUR_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif

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
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * PduR_Reporting_ReportError
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_ReportError(uint8 ApiId, uint8 ErrorCode)
{
#if (PDUR_DEV_ERROR_REPORT == STD_ON)
  (void)Det_ReportError(PDUR_MODULE_ID, 0U, ApiId, ErrorCode);
#endif
  PDUR_DUMMY_STATEMENT(ApiId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(ErrorCode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 * PduR_Reporting_ReportRuntimeError
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_ReportRuntimeError(uint8 ApiId, uint8 ErrorCode)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
#if (PDUR_DEV_ERROR_REPORT == STD_ON)
  (void) Det_ReportRuntimeError(PDUR_MODULE_ID, 0U, ApiId, ErrorCode);
#endif
  PDUR_DUMMY_STATEMENT(ApiId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(ErrorCode); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 * PduR_Reporting_CallPreTransmitCallback
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallPreTransmitCallback(PduR_RmDestRomIterType rmDestRomIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
#if (PDUR_BSWM_CALLBACKS == STD_ON)
  if(PduR_IsBswMPduRPreTransmitCallbackOfRmDestRom(rmDestRomIdx))
  {
    BswM_PduR_PreTransmit(PduR_GetTxIf2UpIdxOfRmGDestRom(PduR_GetRmGDestRomIdxOfRmDestRom(rmDestRomIdx)), info);
  }
#endif
  PDUR_DUMMY_STATEMENT(rmDestRomIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(info); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 * PduR_Reporting_CallTransmitCallback
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallTransmitCallback(PduR_Tx2LoIterType tx2LoIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
#if (PDUR_BSWM_CALLBACKS == STD_ON)
  if(PduR_IsBswMPduRTransmitCallbackOfTx2Lo(tx2LoIdx))
  {
    BswM_PduR_Transmit((PduIdType) tx2LoIdx, info);
  }
#endif
  PDUR_DUMMY_STATEMENT(tx2LoIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(info); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 * PduR_Reporting_CallRxIndicationCallback
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallRxIndicationCallback(PduR_RxIf2DestIterType rxIf2DestIdx, P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
#if (PDUR_BSWM_CALLBACKS == STD_ON)
  if(PduR_IsBswMPduRRxIndicationCallbackOfRxIf2Dest(rxIf2DestIdx))
  {
    BswM_PduR_RxIndication((PduIdType) rxIf2DestIdx, info);
  }
#endif
  PDUR_DUMMY_STATEMENT(rxIf2DestIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(info); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 * PduR_Reporting_CallTxConfirmationCallback
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallTxConfirmationCallback(PduR_TxIf2UpIterType txIf2UpIdx)
{
#if (PDUR_BSWM_CALLBACKS == STD_ON)
  if(PduR_IsBswMPduRTxConfirmationCallbackOfTxIf2Up(txIf2UpIdx))
  {
    BswM_PduR_TxConfirmation((PduIdType) txIf2UpIdx);
  }
#endif
  PDUR_DUMMY_STATEMENT(txIf2UpIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 * PduR_Reporting_CallTpStartOfReceptionCallback
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallTpStartOfReceptionCallback(PduR_RxTp2DestIterType rxTp2DestIdx, P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info, PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
#if (PDUR_BSWM_CALLBACKS == STD_ON)
  if(PduR_IsBswMPduRTpStartOfReceptionCallbackOfRxTp2Dest(rxTp2DestIdx))
  {
    BswM_PduR_TpStartOfReception((PduIdType) rxTp2DestIdx, info, TpSduLength, bufferSizePtr);
  }
#endif
  PDUR_DUMMY_STATEMENT(rxTp2DestIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(info); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(TpSduLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(bufferSizePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 * PduR_Reporting_CallTpRxIndicationCallback
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallTpRxIndicationCallback(PduR_RxTp2DestIterType rxTp2DestIdx, Std_ReturnType result)
{
#if (PDUR_BSWM_CALLBACKS == STD_ON)
  if(PduR_IsBswMPduRTpRxIndicationCallbackOfRxTp2Dest(rxTp2DestIdx))
  {
    BswM_PduR_TpRxIndication((PduIdType) rxTp2DestIdx, result);
  }
#endif
  PDUR_DUMMY_STATEMENT(rxTp2DestIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(result); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 * PduR_Reporting_CallTpTxConfirmationCallback
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_CallTpTxConfirmationCallback(PduR_TxTp2SrcIterType txTp2SrcIdx, Std_ReturnType result)
{
#if (PDUR_BSWM_CALLBACKS == STD_ON)
  if(PduR_IsBswMPduRTpTxConfirmationCallbackOfTxTp2Src(txTp2SrcIdx))
  {
    BswM_PduR_TpTxConfirmation((PduIdType) txTp2SrcIdx, result);
  }
#endif
  PDUR_DUMMY_STATEMENT(txTp2SrcIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  PDUR_DUMMY_STATEMENT(result); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 * PduR_Reporting_NotifyQueueOverflow
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *********************************************************************************************************************/
FUNC(void, PDUR_CODE) PduR_Reporting_NotifyQueueOverflow(PduR_RmGDestRomIdxOfRmDestRomType rmGDestRomIdx)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
#if (PDUR_ERROR_NOTIFICATION == STD_ON)  /* COV_PDUR_VAR_QM_FEATURE */
    ErrorNotificationQueueOverflow(PduR_GetDestHndOfRmGDestRom(rmGDestRomIdx));
#endif
  PDUR_DUMMY_STATEMENT(rmGDestRomIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * GLOBAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  END OF FILE: PduR_Reporting.c
 *********************************************************************************************************************/
