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
/*!        \file  Il_AsrLdCom_Rte.vcaspecs.c
 *         \brief Rte stub for Vector Code Analyzer
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the .vcaplugin file.
 *********************************************************************************************************************/
#include "ComStack_Types.h"







FUNC(void, LDCOM_APPL_CODE) Pdu2UnfilteredInvariantProjectModelView0_LdComRxIndication(P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr)
{}


FUNC(void, LDCOM_APPL_CODE) Pdu3UnfilteredInvariantProjectModelView0_LdComTxConfirmation(void)
{}

FUNC(Std_ReturnType, LDCOM_APPL_CODE) Pdu3UnfilteredInvariantProjectModelView0_LdComTxTriggerTransmit(P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr)
{
    return $range(0,255);
}


FUNC(BufReq_ReturnType, LDCOM_APPL_CODE) Pdu5UnfilteredInvariantProjectModelView0_LdComRxStartOfReception(P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,  PduLengthType TpSduLength,  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) bufferSizePtr)
{
    return $range(0,255);
}

FUNC(BufReq_ReturnType, LDCOM_APPL_CODE) Pdu1UnfilteredInvariantProjectModelView0_LdComRxCopyRxData(P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info,  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) bufferSizePtr)
{
    return $range(0,255);
}

FUNC(void, LDCOM_APPL_CODE) Pdu5UnfilteredInvariantProjectModelView0_LdComTpRxIndication(Std_ReturnType result)
{}

FUNC(BufReq_ReturnType, LDCOM_APPL_CODE) Pdu4UnfilteredInvariantProjectModelView0_LdComTxCopyTxData(P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) info, P2VAR(RetryInfoType, AUTOMATIC, LDCOM_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) availableDataPtr)
{
    return $range(0,255);
}

FUNC(void, LDCOM_APPL_CODE) Pdu4UnfilteredInvariantProjectModelView0_LdComTpTxConfirmation(Std_ReturnType result)
{}



