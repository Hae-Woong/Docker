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
/*!        \file  Il_AsrIpduMCfg5_VStdLib.vcaspecs.c
 *        \brief  VStdLib stub for Vector Code Analyzer
 *      \details  Stub of VStdLib functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the .vcaplugin file.
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
  __builtin_memcpy(pDst, pSrc, nCnt);  /* VCA_IPDUM_VCA_MEMCPY_S */
}

/*!
 * \spec
 *    requires pDst != NULL_PTR;
 *    requires $lengthOf(pDst) >= nCnt;
 * \endspec
 */
void VStdLib_MemSet(void* pDst,
                    uint8 nPattern,
                    VStdLib_CntType nCnt)
{
  __builtin_memset(pDst, nPattern, nCnt);  /* VCA_IPDUM_VCA_MEMCPY_S */
}

/*!
 * \spec
 *    requires pDst != NULL_PTR;
 *    requires pSrc != NULL_PTR;
 *    requires $lengthOf(pSrc) >= nCnt;
 * \endspec
 */
void VStdLib_MemCpy(void* pDst,
                    const void* pSrc,
                    VStdLib_CntType nCnt)
{
  __builtin_memcpy(pDst, pSrc, nCnt);  /* VCA_IPDUM_VCA_MEMCPY_S */
}

/*!
 * \spec
 *    requires pDst != NULL_PTR;
 *    requires pSrc != NULL_PTR;
 * \endspec
 */
Std_ReturnType VStdLib_BitCpySawBigToBigEndian_s(
                   void* pDst,
                   VStdLib_CntType nDstSize,
                   VStdLib_CntType nDstBitOffsetSaw,
                   const void* pSrc,
                   VStdLib_CntType nSrcBitOffsetSaw,
                   VStdLib_CntType nCnt,
                   Std_ReturnType errorCode)
{
 return E_OK; /* VCA_IPDUM_VCA_BITCPY_S */
}

/*!
 * \spec
 *    requires pDst != NULL_PTR;
 *    requires pSrc != NULL_PTR;
 * \endspec
 */
Std_ReturnType VStdLib_BitCpySawLittleToLittleEndian_s(
  void* pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetSaw,
  const void* pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode)
{
  return E_OK; /* VCA_IPDUM_VCA_BITCPY_S */
}
