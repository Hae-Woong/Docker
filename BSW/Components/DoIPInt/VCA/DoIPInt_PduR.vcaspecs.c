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
/*!        \file  DoIPInt_PduR.vcaspecs.c
 *        \brief  PduR stub for Vector Code Analyzer
 *      \details  Stub of PduR functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2022-07-05  viseje                      Initial creation.
 *  02.00.00  2023-11-07  viseje                      Update to VCA7
 *********************************************************************************************************************/

#include "PduR_DoIPInt.h"

/*! Stubs the function to have no side effects (because info is not const).
 * May return any return code of BufReq_ReturnType.
 * \spec
 *    requires info != NULL_PTR;
 *    requires retry == NULL_PTR;
 *    requires availableDataPtr != NULL_PTR;
 * \endspec
 */
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_DoIPIntTpCopyTxData(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
  P2VAR(RetryInfoType, AUTOMATIC, PDUR_APPL_DATA) retry,
  P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) availableDataPtr)
{
  return $range(0,3);
}

/*! Stubs the function to have no side effects (because info is not const).
 * May return any return code of BufReq_ReturnType.
 * \spec
 *    requires info != NULL_PTR;
 *    requires bufferSizePtr != NULL_PTR;
 * \endspec
 */
FUNC(BufReq_ReturnType, PDUR_CODE) PduR_DoIPIntTpCopyRxData(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, PDUR_APPL_DATA) info,
  P2VAR(PduLengthType, AUTOMATIC, PDUR_APPL_DATA) bufferSizePtr)
{
  return $range(0,3);
}
