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
/*!        \file  KeyM_Appl.vcaspecs.c
 *        \brief  Application callout stub for Vector Code Analyzer
 *      \details  Stub of application callouts required for execution of the Vector Code Analyzer.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author        Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  04.00.00  2021-10-19  vismxe                      Initial creation.
 *********************************************************************************************************************/

#include "KeyM.h"
#include "KeyM_Cfg.h"

/*!
 * \spec
 *    requires ptr != NULL_PTR;
 *    requires $lengthOf(ptr) >= len;
 * \endspec
 */
static void breakmem(P2VAR(uint8, AUTOMATIC, KEYM_APPL_VAR) ptr, int len)
{
  int i;

  for (i = 0u; i < len; i++)
  {
    ptr[i] = (uint8)$undef(); /* VCA_KEYM_APPLSTUB_BREAKMEM */
  }
}

/*!
 * \spec
 *    requires CertId < KeyM_GetSizeOfCertificate();
 *    requires CertElementData != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, KEYM_APPL_CODE)
Vcastub_Appl_CertificateElementVerificationCallout(
    KeyM_CertificateIdType CertId,
    KeyM_CertElementIdType CertElementId,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementData,
    uint32 CertElementDataLength)
{
  return (Std_ReturnType)$undef();
}

/*!
 * \spec
 *    requires CertId < KeyM_GetSizeOfCertificate();
 *    requires CertificateDataPtr != NULL_PTR;
 * \endspec
 */
FUNC(void, KEYM_APPL_CODE)
Vcastub_Appl_CertificateInitCallout(
    KeyM_CertificateIdType CertId, P2VAR(KeyM_ConstCertDataType, AUTOMATIC, KEYM_APPL_VAR) CertificateDataPtr)
{
  CertificateDataPtr->certData = $undef();
  CertificateDataPtr->certDataLength = $range(0, CertificateDataPtr->certDataLength);
  $assume($external(CertificateDataPtr->certData));
  $assume($lengthOf(CertificateDataPtr->certData) >= (CertificateDataPtr->certDataLength));
}

/*!
 * \spec
 *    requires CertId < KeyM_GetSizeOfCertificate();
 *    requires timeStamp != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, KEYM_APPL_CODE)
Vcastub_Appl_CertificateGetCurrentTimeCallout(
    KeyM_CertificateIdType CertId, P2VAR(uint64, AUTOMATIC, KEYM_APPL_VAR) timeStamp)
{
  *timeStamp = (uint64)$undef();
  return (Std_ReturnType)$undef();
}

/*!
 * \spec
 *    requires (ResultDataLength == 0u) || (ResultDataPtr != NULL_PTR);
 * \endspec
 */
FUNC(Std_ReturnType, KEYM_RTE_CODE)
Vcastub_Appl_ServiceCertificateRteCallbackNotification440(
    KeyM_CertificateStatusType Result, uint16 ResultDataLength, P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) ResultDataPtr)
{
  return (Std_ReturnType)$undef();
}

/*!
 * \spec
 *    requires (ResultDataLength == 0u) || (ResultDataPtr != NULL_PTR);
 * \endspec
 */
FUNC(Std_ReturnType, KEYM_RTE_CODE)
Vcastub_Appl_ServiceCertificateRteCallbackNotification2211(
    KeyM_CertificateStatusType Result, uint16 ResultDataLength, P2CONST(void, AUTOMATIC, KEYM_APPL_DATA) ResultDataPtr)
{
  return (Std_ReturnType)$undef();
}

/* function has only inbound parameters, so no requirements are necessary */
FUNC(Std_ReturnType, KEYM_RTE_CODE)
Vcastub_Appl_CertificateVerifyRteCallbackNotification(KeyM_CertificateStatusType Result)
{
  return (Std_ReturnType)$undef();
}

/*!
 * \spec
 *    requires CertId < KeyM_GetSizeOfCertificate();
 *    requires (ResultDataLength == 0u) || ((ResultDataPtr != NULL_PTR) && ($lengthOf(ResultDataPtr) >= ResultDataLength));
 * \endspec
 */
FUNC(void, KEYM_APPL_CODE)
Vcastub_Appl_ServiceCertificateCallbackNotification(
    KeyM_CertificateIdType CertId,
    KeyM_CertificateStatusType Result,
    uint16 ResultDataLength,
    P2VAR(uint8, AUTOMATIC, KEYM_APPL_VAR) ResultDataPtr)
{
  if (ResultDataLength != 0)
  {
    breakmem(ResultDataPtr, ResultDataLength); /* VCA_KEYM_APPLSTUB_BREAKMEM_CALL */
  }
}

/*!
 * \spec
 *    requires CertId < KeyM_GetSizeOfCertificate();
 *    requires CertElementData != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, KEYM_APPL_CODE)
Vcastub_Appl_CertificateSetKeyCallout(
    KeyM_CertificateIdType CertId,
    KeyM_CertElementIdType CertElementId,
    uint32 csmKeyId,
    P2CONST(uint8, AUTOMATIC, KEYM_APPL_DATA) CertElementData,
    uint32 CertElementDataLength)
{
  return (Std_ReturnType)$undef();
}

/*!
 * \spec
 *    requires CertId < KeyM_GetSizeOfCertificate();
 * \endspec
 */
FUNC(Std_ReturnType, KEYM_APPL_CODE)
Vcastub_Appl_CertificateVerifyCallbackNotification(KeyM_CertificateIdType CertId, KeyM_CertificateStatusType Result)
{
  return (Std_ReturnType)$undef();
}


/* Function KeyM_GetSizeOfCertificateGroup(), which is used in the \spec of this stub, only exists in configurations with certificate groups */
#if KEYM_CERTIFICATEGROUPUSEDOFCERTIFICATE == STD_ON
/*!
 * \spec
 *    requires GroupId < KeyM_GetSizeOfCertificateGroup();
 * \endspec
 */
#endif
FUNC(Std_ReturnType, KEYM_APPL_CODE)
Vcastub_Appl_CertificateGroupVerifyCallbackNotification(KeyM_CertificateGroupIdType GroupId, KeyM_CertificateGroupStatusType Result)
{
  return (Std_ReturnType)$undef();
}

/* VCA_JUSTIFICATION_BEGIN

  \ID  VCA_KEYM_APPLSTUB_BREAKMEM
    \DESCRIPTION      Write access to ptr[i]
    \COUNTERMEASURE   \R Loop condition restricts i to a value from 0 to (len - 1).
                         Function requirement $lengthOf(ptr) >= len ensures that buffer is long enough for these write accesses.

  \ID  VCA_KEYM_APPLSTUB_BREAKMEM_CALL
    \DESCRIPTION      VCA thinks breakmem() may be called outside its specification
    \COUNTERMEASURE   \N Calling function has requirements for the passed parameters that match those of breakmem (ptr != NULL_PTR and $lengthOf(ptr) >= len).
                         Calling function's API contract ensures/requires that the passed pointer has at least the passed length.

  VCA_JUSTIFICATION_END */
