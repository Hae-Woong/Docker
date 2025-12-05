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
/*!        \file  Il_AsrComCfg5_PduR.vcaspecs.c
 *        \brief  PduR stub for Vector Code Analyzer
 *      \details  Stub of VStdLib functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/
#include "ComStack_Types.h"

Std_ReturnType PduR_ComTransmit(PduIdType id, const PduInfoType* PduInfoPtr)
{
  return $range(0,1);
}

Std_ReturnType PduR_ComCancelTransmit(PduIdType id)
{
  return $range(0,1);
}

