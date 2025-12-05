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
/*!        \file  Dlt_VStdLib.vcaspecs.c
 *        \brief  VStdLib stub for Vector Code Analyzer
 *      \details  Stub of VStdLib functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2021-11-28  visore        SWAT-1831     Initial creation
 *  01.01.00  2022-07-21  visore        SWAT-2205     Dlt: support fetching synchronized timestamps from Tsyn
 *********************************************************************************************************************/

#include "vstdlib.h"

/*!
 * \spec
 *    requires ptr != NULL_PTR;
 *    requires $lengthOf(ptr) >= len;
 * \endspec
 */
static void breakmem(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) ptr, int len)
{
  int i;

  for(i = 0u; i < len; i++)
  {
    ((uint8*)ptr)[i] = (uint8)$undef(); /* VCA_KEYM_VSTDLIBSTUB_BREAKMEM */
  }
}

/*!
 * \spec
 *    requires pDst != NULL_PTR;
 *    requires $lengthOf(pDst) >= nCnt;
 * \endspec
 */
FUNC(void, VSTDLIB_CODE) VStdLib_MemSet(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  uint8 nPattern,
  VStdLib_CntType nCnt)
{
  breakmem(pDst, nCnt); /* VCA_KEYM_VSTDLIBSTUB_BREAKMEM_CALL */
}

/*!
 * \spec
 *    requires pDst != NULL_PTR;
 *    requires pSrc != NULL_PTR;
 *    requires $lengthOf(pDst) >= nCnt;
 * \endspec
 */
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nCnt)
{
  breakmem(pDst, nCnt); /* VCA_KEYM_VSTDLIBSTUB_BREAKMEM_CALL */
}

/* VCA_JUSTIFICATION_BEGIN

  \ID  VCA_KEYM_VSTDLIBSTUB_BREAKMEM
    \DESCRIPTION      Write access to ptr[i]
    \COUNTERMEASURE   \R Loop condition restricts i to a value from 0 to (len - 1).
                         Function requirement $lengthOf(ptr) >= len ensures that buffer is long enough for these write accesses.

  \ID  VCA_KEYM_VSTDLIBSTUB_BREAKMEM_CALL
    \DESCRIPTION      VCA thinks breakmem() may be called outside its specification
    \COUNTERMEASURE   \N Calling function has requirements for the passed parameters that match those of breakmem (ptr != NULL_PTR and $lengthOf(ptr) >= len).
                         Calling function's API contract ensures/requires that the passed pointer has at least the passed length.

  VCA_JUSTIFICATION_END */
