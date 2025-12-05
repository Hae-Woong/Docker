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
/*!        \file  Gw_AsrPduRCfg5_VStdLib.vcaspecs.c
 *        \brief  VStdLib stub for Vector Code Analyzer
 *      \details  Stub of VStdLib functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  02.00.00  2022-06-27  visms         COM-1184      Replace RH by VCA
 *  02.00.01  2023-08-22  visfrm        ESCAN00114950 VCA erros when analyzing PduR
 *********************************************************************************************************************/
#include "vstdlib.h"

/*!
 * \spec
 *    requires pDst != NULL_PTR;
 *    requires pSrc != NULL_PTR;
 *    requires $lengthOf(pDst) >= nDstSize;
 *    requires $lengthOf(pSrc) >= nCnt;
 *    requires nDstSize >= nCnt;
 * \endspec
 */
void VStdLib_MemCpy_s(void* pDst,
                      VStdLib_CntType nDstSize,
                      const void* pSrc,
                      VStdLib_CntType nCnt)
{
  __builtin_memcpy(pDst, pSrc, nCnt);  /* VCA_PDUR_VCA_MEMCPY_S */
}


