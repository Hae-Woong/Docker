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
/*!        \file  DoIPInt_VStdLib.vcaspecs.c
 *        \brief  VStdLib stub for Vector Code Analyzer
 *      \details  Stub of VStdLib functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2021-07-15  viseje                      Initial creation.
 *  02.00.00  2023-11-07  viseje                      Update to VCA7
 *********************************************************************************************************************/

#include "vstdlib.h"

/*! Map to compiler builtin memcpy
 *  \spec
 *    requires pDst != NULL_PTR;
 *    requires $lengthOf(pDst) >= nCnt;
 *  \endspec
 */
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                        P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
                                        VStdLib_CntType nCnt)
{
  __builtin_memcpy(pDst, pSrc, nCnt);                                                                                  /* VCA_DOIPINT_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION */
}
