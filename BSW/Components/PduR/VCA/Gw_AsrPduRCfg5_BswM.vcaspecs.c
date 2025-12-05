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
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Gw_AsrPduRCfg5_BswM.vcaspecs.c
 *        \brief  BswM stub for Vector Code Analyzer
 *      \details  Stub of BswM functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  02.00.00  2022-06-27  visms         COM-1184      Replace RH by VCA
 *  02.00.01  2023-08-22  visfrm        ESCAN00114950 VCA erros when analyzing PduR
 *********************************************************************************************************************/
#include "ComStack_Types.h"

void BswM_PduR_PreTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
}

void BswM_PduR_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
}

void BswM_PduR_TpStartOfReception(PduIdType id, PduInfoType* info, PduLengthType TpSduLength, PduLengthType* bufferSizePtr)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
}

void BswM_PduR_Transmit(PduIdType id, const PduInfoType* PduInfoPtr)  /* VCA_PDUR_FUNCTION_NOT_USED_OR_NO_ANALYSIS_ENTRY */
{
}
