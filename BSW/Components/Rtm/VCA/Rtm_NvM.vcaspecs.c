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
/*!        \file  Rtm_NvM.vcaspecs.c
 *        \brief  NvM stub for Vector Code Analyzer
 *      \details  Stub of NvM functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2021-09-03  vismxe                      Initial creation.
 *********************************************************************************************************************/

#include "NvM.h"

/*!
 * \spec
 *    requires BlockChanged < 2;
 * \endspec
 */
FUNC(Std_ReturnType, NVM_PUBLIC_CODE) NvM_SetRamBlockStatus(
  NvM_BlockIdType BlockId, 
  boolean BlockChanged)
{
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
