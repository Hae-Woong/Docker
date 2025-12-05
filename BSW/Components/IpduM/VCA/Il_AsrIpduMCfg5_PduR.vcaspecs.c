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
/*!        \file  Il_AsrIpduMCfg5_PduR.vcaspecs.c
 *        \brief  PduR stub for Vector Code Analyzer
 *      \details  Stub of PduR functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the .vcaplugin file.
 *********************************************************************************************************************/
#include "ComStack_Types.h"

Std_ReturnType PduR_IpduMTransmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
  /* The values of Std_ReturnType are 0 (E_OK) and 1 (E_NOT_OK) */
  return $range(0,1);
}

void PduR_IpduMRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{

}

void PduR_IpduMTxConfirmation(PduIdType TxPduId)
{

}

Std_ReturnType PduR_IpduMTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
{
  /* The values of Std_ReturnType are 0 (E_OK) and 1 (E_NOT_OK) */
  return $range(0,1);
}