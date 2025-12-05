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
/*!        \file  EthTSyn_VStdLib.vcaspecs.c
 *        \brief  VStdLib stub for Vector Code Analyzer
 *      \details  Stub of VStdLib functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2021-07-15  vissem                      Initial creation.
 *********************************************************************************************************************/

#include "vstdlib.h"

/*! Map to compiler builtin memset
 * \spec
 *  requires pDst != NULL_PTR;
 *  requires $lengthOf((uint8*)pDst) >= nCnt;
 * \endspec
 */

FUNC(void, VSTDLIB_CODE) VStdLib_MemSet(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                        uint8 nPattern,
                                        VStdLib_CntType nCnt)
{
  uint8* pDestU8 = (uint8*)pDst;

  if (pDestU8 != NULL_PTR)
  {
    VStdLib_CntType i;

    for (i = 0; i < nCnt; i++)
    {
      pDestU8[i] = nPattern;
    }
  }
}

/*!
 * \spec
 *  requires pDst != NULL_PTR;
 *  requires $lengthOf((uint8*)pDst) >= nDstSize;
 * \endspec
 */
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy_s(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                         VStdLib_CntType nDstSize,
                                         P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
                                         VStdLib_CntType nCnt)
{
  if (nDstSize >= nCnt)
  {
    __builtin_memcpy(pDst, pSrc, nCnt);                                                                                /* VCA_ETHTSYN_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION */
  }
}
