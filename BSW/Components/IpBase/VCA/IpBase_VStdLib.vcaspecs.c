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
/*!        \file  IpBase_VStdLib.vcaspecs.c
 *        \brief  VStdLib stub for Vector Code Analyzer
 *      \details  Stub of VStdLib functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2022-10-28  visalr                      Initial creation.
 *********************************************************************************************************************/

#include "vstdlib.h"

/*! \spec
 *    requires $lengthOf(pDst) >= $lengthOf(pSrc);
 *    requires $lengthOf(pDst) >= nCnt;
 *  \endspec
 */
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                        P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
                                        VStdLib_CntType nCnt)
{
  if ( pDst != NULL_PTR )
  {
    __builtin_memcpy(pDst, pSrc, nCnt); /* VCA_IPBASE_VALID_API_POINTER */
  }
}

/*! \spec
 *    requires $lengthOf(pDst) >= nCnt;
 *  \endspec
 */
FUNC(void, VSTDLIB_CODE) VStdLib_MemSet(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                        uint8 nPattern,
                                        VStdLib_CntType nCnt)
{
  __builtin_memset(pDst, nPattern, nCnt);
}
