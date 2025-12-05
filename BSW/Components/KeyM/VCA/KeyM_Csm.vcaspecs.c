/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  KeyM_Csm.vcaspecs.c
 *        \brief  CSM stub for Vector Code Analyzer
 *      \details  Stub of CSM functions required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2021-09-03  vismxe                      Initial creation.
 *********************************************************************************************************************/

#include "Csm.h"

/*!
 * \spec
 *    requires ptr != NULL_PTR;
 *    requires $lengthOf(ptr) >= len;
 * \endspec
 */
static void breakmem(P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ptr, int len)
{
  int i;

  for(i = 0u; i < len; i++)
  {
    ptr[i] = (uint8)$undef(); /* VCA_KEYM_CSMSTUB_BREAKMEM */
  }
}

/*!
 * \spec
 *    requires keyLengthPtr != NULL_PTR;
 *    requires keyPtr != NULL_PTR;
 *    requires $lengthOf(keyLengthPtr) >= 1u;
 *    requires $lengthOf(keyPtr) >= *keyLengthPtr;
 * \endspec
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGet(uint32 keyId,
  uint32 keyElementId,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) keyLengthPtr)
{
  breakmem(keyPtr, *keyLengthPtr); /* VCA_KEYM_CSMSTUB_BREAKMEM_CALL */
  *keyLengthPtr = $range(0, *keyLengthPtr);
  return (Std_ReturnType)$undef();
}

/* function has only inbound parameters, so no requirements are necessary */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementSet(uint32 keyId,
  uint32 keyElementId,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
  uint32 keyLength)
{
  return (Std_ReturnType)$undef();
}

/*!
 * \spec
 *    requires resultLengthPtr != NULL_PTR;
 *    requires resultPtr != NULL_PTR;
 *    requires $lengthOf(resultLengthPtr) >= 1u;
 *    requires $lengthOf(resultPtr) >= *resultLengthPtr;
 * \endspec
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_Hash(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  breakmem(resultPtr, *resultLengthPtr); /* VCA_KEYM_CSMSTUB_BREAKMEM_CALL */
  *resultLengthPtr = $range(0, *resultLengthPtr);
  return (Std_ReturnType)$undef();
}

/*!
 * \spec
 *    requires resultLengthPtr != NULL_PTR;
 *    requires resultPtr != NULL_PTR;
 *    requires $lengthOf(resultLengthPtr) >= 1u;
 *    requires $lengthOf(resultPtr) >= *resultLengthPtr;
 * \endspec
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerate(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  breakmem(resultPtr, *resultLengthPtr); /* VCA_KEYM_CSMSTUB_BREAKMEM_CALL */
  *resultLengthPtr = $range(0, *resultLengthPtr);
  return (Std_ReturnType)$undef();
}

/*!
 * \spec
 *    requires verifyPtr != NULL_PTR;
 *    requires $lengthOf(verifyPtr) >= 1u;
 * \endspec
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerify(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
  uint32 dataLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
  uint32 signatureLength,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  *verifyPtr = (Crypto_VerifyResultType)$undef();
  return (Std_ReturnType)$undef();
}

/*!
 * \spec
 *    requires verifyPtr != NULL_PTR;
 *    requires plaintextLengthPtr != NULL_PTR;
 *    requires plaintextPtr != NULL_PTR;
 *    requires $lengthOf(verifyPtr) >= 1u;
 *    requires $lengthOf(plaintextLengthPtr) >= 1u;
 *    requires $lengthOf(plaintextPtr) >= *plaintextLengthPtr;
 *    requires
 * \endspec
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecrypt(uint32 jobId,
  Crypto_OperationModeType mode,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
  uint32 ciphertextLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
  uint32 associatedDataLength,
  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
  uint32 tagLength,
  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  breakmem(plaintextPtr, *plaintextLengthPtr); /* VCA_KEYM_CSMSTUB_BREAKMEM_CALL */
  *plaintextLengthPtr = $range(0, *plaintextLengthPtr);
  *verifyPtr = (Crypto_VerifyResultType)$undef();
  return (Std_ReturnType)$undef();
}

/* VCA_JUSTIFICATION_BEGIN

  \ID  VCA_KEYM_CSMSTUB_BREAKMEM
    \DESCRIPTION      Write access to ptr[i]
    \COUNTERMEASURE   \R Loop condition restricts i to a value from 0 to (len - 1).
                         Function requirement $lengthOf(ptr) >= len ensures that buffer is long enough for these write accesses.

  \ID  VCA_KEYM_CSMSTUB_BREAKMEM_CALL
    \DESCRIPTION      VCA thinks breakmem() may be called outside its specification
    \COUNTERMEASURE   \N Calling function has requirements for the passed parameters that match those of breakmem (ptr != NULL_PTR and $lengthOf(ptr) >= len).
                         Calling function's API contract ensures/requires that the passed pointer has at least the passed length.

  VCA_JUSTIFICATION_END */
