/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Dlt_PduR.vcaspecs.c
 *        \brief  PduR stub for Vector Code Analyzer
 *      \details  Stub of PduR functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2021-11-28  visore        SWAT-1831     Initial creation
 *  01.01.00  2022-07-21  visore        SWAT-2205     Dlt: support fetching synchronized timestamps from Tsyn
 *********************************************************************************************************************/

#include "PduR_Dlt.h"

/*!
 * \spec
 *    requires info != NULL_PTR;
 *    requires info->SduDataPtr != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, PDUR_CODE) PduR_DltTransmit(
  PduIdType id, 
  P2CONST(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info)
{
  info = $undef();

  return (Std_ReturnType)$undef();
}
