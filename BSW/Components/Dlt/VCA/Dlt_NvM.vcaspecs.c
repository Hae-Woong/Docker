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
/*!        \file  Dlt_NvM.vcaspecs.c
 *        \brief  NvM stub for Vector Code Analyzer
 *      \details  Stub of NvM functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2021-11-28  visore        SWAT-1831     Initial creation
 *  01.01.00  2022-07-21  visore        SWAT-2205     Dlt: support fetching synchronized timestamps from Tsyn
 *********************************************************************************************************************/

#include "NvM.h"

/*!
 * \spec
 *    requires NvM_SrcPtr != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_WriteBlock(
  NvM_BlockIdType BlockId, 
  P2CONST(void, AUTOMATIC, NVM_APPL_DATA) NvM_SrcPtr)
{
  NvM_SrcPtr = $undef();

  return (Std_ReturnType)$undef();
}

/*!
 * \spec
 *    requires RequestResultPtr != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_GetErrorStatus(
  NvM_BlockIdType BlockId, 
  P2VAR(NvM_RequestResultType, AUTOMATIC, NVM_APPL_DATA) RequestResultPtr)
{
  RequestResultPtr = $undef();

  return (Std_ReturnType)$undef();
}
