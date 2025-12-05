/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                                All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Csm_Rte.c
 *        \brief  MICROSAR Crypto Service Manager (CSM) RTE
 *
 *      \details  Implementation of the MICROSAR Crypto Service Manager (CSM) service APIs used only by RTE
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define CSM_RTE_SOURCE
/* PRQA S 6060 EOF */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Csm_Rte.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (CSM_RTE_PORTS == STD_ON)

# define CSM_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Csm_HashAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_HashAsr4_03(uint32 jobId,
                                               Crypto_OperationModeType mode,
                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                               uint32 dataLength,
                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Hash */
  return Csm_Hash(jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_HashAsr4_03() */

/**********************************************************************************************************************
 *  Csm_HashDataRefAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_HashDataRefAsr4_03(uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                      uint32 dataLength,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Hash */
  return Csm_Hash(jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_HashDataRefAsr4_03() */

/**********************************************************************************************************************
 *  Csm_HashDataRefAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_HashDataRefAsrR19_11(uint32 jobId,
                                                        Crypto_OperationModeType mode,
                                                        P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                        uint32 dataLength,
                                                        P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                        P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Hash */
  return Csm_Hash(jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_HashDataRefAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_HashDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_HashDataRef(uint32 jobId,
                                               Crypto_OperationModeType mode,
                                               P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                               uint32 dataLength,
                                               P2VAR(void, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Hash */
  return Csm_Hash(jobId,
                  mode,
                  (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))dataPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                  dataLength,
                  (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))resultPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                  resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_HashDataRef() */

/**********************************************************************************************************************
 *  Csm_MacGenerateAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerateAsr4_03(uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                      uint32 dataLength,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr)
{
  /* #10 Call Csm_MacGenerate */
  return Csm_MacGenerate(jobId, mode, dataPtr, dataLength, macPtr, macLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_MacGenerateAsr4_03() */

/**********************************************************************************************************************
 *  Csm_MacGenerateAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerateAsr4_04(uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                      uint32 dataLength,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr)
{
  /* #10 Call Csm_MacGenerate */
  return Csm_MacGenerate(jobId, mode, dataPtr, dataLength, macPtr, macLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_MacGenerateAsr4_04() */

/**********************************************************************************************************************
 *  Csm_MacGenerateDataRefAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerateDataRefAsr4_04(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                             uint32 dataLength,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr)
{
  /* #10 Call Csm_MacGenerate */
  return Csm_MacGenerate(jobId, mode, dataPtr, dataLength, macPtr, macLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_MacGenerateDataRefAsr4_04() */

/**********************************************************************************************************************
 *  Csm_MacGenerateDataRefAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerateDataRefAsrR19_11(uint32 jobId,
                                                               Crypto_OperationModeType mode,
                                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                               uint32 dataLength,
                                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr)
{
  /* #10 Call Csm_MacGenerate */
  return Csm_MacGenerate(jobId, mode, dataPtr, dataLength, macPtr, macLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_MacGenerateDataRefAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_MacGenerateDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerateDataRef(uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                      uint32 dataLength,
                                                      P2VAR(void, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr)
{
  /* #10 Call Csm_MacGenerate */
  return Csm_MacGenerate(jobId,
                         mode,
                         (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))dataPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                         dataLength,
                         (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))macPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                         macLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_MacGenerateDataRef() */

/**********************************************************************************************************************
 *  Csm_MacVerifyAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerifyAsr4_03(uint32 jobId,
                                                    Crypto_OperationModeType mode,
                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                    uint32 dataLength,
                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                    uint32 macLength,
                                                    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_MacVerify */
  return Csm_MacVerify(jobId, mode, dataPtr, dataLength, macPtr, macLength, verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_MacVerifyAsr4_03() */

/**********************************************************************************************************************
 *  Csm_MacVerifyAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerifyAsr4_04(uint32 jobId,
                                                    Crypto_OperationModeType mode,
                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                    uint32 dataLength,
                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                    uint32 macLength,
                                                    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_MacVerify */
  return Csm_MacVerify(jobId, mode, dataPtr, dataLength, macPtr, macLength, verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_MacVerifyAsr4_04() */

/**********************************************************************************************************************
 *  Csm_MacVerifyDataRefAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerifyDataRefAsr4_04(uint32 jobId,
                                                           Crypto_OperationModeType mode,
                                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                           uint32 dataLength,
                                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                           uint32 macLength,
                                                           P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_MacVerify */
  return Csm_MacVerify(jobId, mode, dataPtr, dataLength, macPtr, macLength, verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_MacVerifyDataRefAsr4_04() */

/**********************************************************************************************************************
 *  Csm_MacVerifyDataRefAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerifyDataRefAsrR19_11(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                             uint32 dataLength,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                             uint32 macLength,
                                                             P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_MacVerify */
  return Csm_MacVerify(jobId, mode, dataPtr, dataLength, macPtr, macLength, verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_MacVerifyDataRefAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_MacVerifyDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerifyDataRef(uint32 jobId,
                                                    Crypto_OperationModeType mode,
                                                    P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                    uint32 dataLength,
                                                    P2CONST(void, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                    uint32 macLength,
                                                    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_MacVerify */
  return Csm_MacVerify(jobId,
                       mode,
                       (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))dataPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                       dataLength,
                       (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))macPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                       macLength,
                       verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_MacVerifyDataRef() */

/**********************************************************************************************************************
 *  Csm_EncryptAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_EncryptAsr4_03(uint32 jobId,
                                                  Crypto_OperationModeType mode,
                                                  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                  uint32 dataLength,
                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Encrypt */
  return Csm_Encrypt(jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_EncryptAsr4_03() */

/**********************************************************************************************************************
 *  Csm_EncryptAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_EncryptAsr4_04(uint32 jobId,
                                                  Crypto_OperationModeType mode,
                                                  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                  uint32 dataLength,
                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Encrypt */
  return Csm_Encrypt(jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_EncryptAsr4_04() */

/**********************************************************************************************************************
 *  Csm_EncryptDataRefAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_EncryptDataRefAsr4_04(uint32 jobId,
                                                         Crypto_OperationModeType mode,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                         uint32 dataLength,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Encrypt */
  return Csm_Encrypt(jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_EncryptDataRefAsr4_04() */

/**********************************************************************************************************************
 *  Csm_EncryptDataRefAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_EncryptDataRefAsrR19_11(uint32 jobId,
                                                           Crypto_OperationModeType mode,
                                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                           uint32 dataLength,
                                                           P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                           P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Encrypt */
  return Csm_Encrypt(jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_EncryptDataRefAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_EncryptDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_EncryptDataRef(uint32 jobId,
                                                  Crypto_OperationModeType mode,
                                                  P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                  uint32 dataLength,
                                                  P2VAR(void, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Encrypt */
  return Csm_Encrypt(jobId,
                     mode,
                     (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))dataPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                     dataLength,
                     (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))resultPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                     resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_EncryptDataRef() */

/**********************************************************************************************************************
 *  Csm_DecryptAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_DecryptAsr4_03(uint32 jobId,
                                                  Crypto_OperationModeType mode,
                                                  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                  uint32 dataLength,
                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Decrypt */
  return Csm_Decrypt(jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_DecryptAsr4_03() */

/**********************************************************************************************************************
 *  Csm_DecryptAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_DecryptAsr4_04(uint32 jobId,
                                                  Crypto_OperationModeType mode,
                                                  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                  uint32 dataLength,
                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Decrypt */
  return Csm_Decrypt(jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_DecryptAsr4_04() */

/**********************************************************************************************************************
 *  Csm_DecryptDataRefAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_DecryptDataRefAsr4_04(uint32 jobId,
                                                         Crypto_OperationModeType mode,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                         uint32 dataLength,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Decrypt */
  return Csm_Decrypt(jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_DecryptDataRefAsr4_04() */

/**********************************************************************************************************************
 *  Csm_DecryptDataRefAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_DecryptDataRefAsrR19_11(uint32 jobId,
                                                           Crypto_OperationModeType mode,
                                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                           uint32 dataLength,
                                                           P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                           P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Decrypt */
  return Csm_Decrypt(jobId, mode, dataPtr, dataLength, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_DecryptDataRefAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_DecryptDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_DecryptDataRef(uint32 jobId,
                                                  Crypto_OperationModeType mode,
                                                  P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                  uint32 dataLength,
                                                  P2VAR(void, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_Decrypt */
  return Csm_Decrypt(jobId,
                     mode,
                     (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))dataPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                     dataLength,
                     (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))resultPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                     resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_DecryptDataRef() */

/**********************************************************************************************************************
 *  Csm_AEADEncryptAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncryptAsr4_03(uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                      uint32 plaintextLength,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                      uint32 associatedDataLength,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr)
{
  /* #10 Call Csm_AEADEncrypt */
  return Csm_AEADEncrypt(jobId,
                         mode,
                         plaintextPtr,
                         plaintextLength,
                         associatedDataPtr,
                         associatedDataLength,
                         ciphertextPtr,
                         ciphertextLengthPtr,
                         tagPtr,
                         tagLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_AEADEncryptAsr4_03() */

/**********************************************************************************************************************
 *  Csm_AEADEncryptAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncryptAsr4_04(uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                      uint32 plaintextLength,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                      uint32 associatedDataLength,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr)
{
  /* #10 Call Csm_AEADEncrypt */
  return Csm_AEADEncrypt(jobId,
                         mode,
                         plaintextPtr,
                         plaintextLength,
                         associatedDataPtr,
                         associatedDataLength,
                         ciphertextPtr,
                         ciphertextLengthPtr,
                         tagPtr,
                         tagLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_AEADEncryptAsr4_04() */

/**********************************************************************************************************************
 *  Csm_AEADEncryptDataRefAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncryptDataRefAsr4_04(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                             uint32 plaintextLength,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                             uint32 associatedDataLength,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr)
{
  /* #10 Call Csm_AEADEncrypt */
  return Csm_AEADEncrypt(jobId,
                         mode,
                         plaintextPtr,
                         plaintextLength,
                         associatedDataPtr,
                         associatedDataLength,
                         ciphertextPtr,
                         ciphertextLengthPtr,
                         tagPtr,
                         tagLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_AEADEncryptDataRefAsr4_04() */

/**********************************************************************************************************************
 *  Csm_AEADEncryptDataRefAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncryptDataRefAsrR19_11(uint32 jobId,
                                                               Crypto_OperationModeType mode,
                                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                               uint32 plaintextLength,
                                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                               uint32 associatedDataLength,
                                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr)
{
  /* #10 Call Csm_AEADEncrypt */
  return Csm_AEADEncrypt(jobId,
                         mode,
                         plaintextPtr,
                         plaintextLength,
                         associatedDataPtr,
                         associatedDataLength,
                         ciphertextPtr,
                         ciphertextLengthPtr,
                         tagPtr,
                         tagLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_AEADEncryptDataRefAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_AEADEncryptDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncryptDataRef(uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(void, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                      uint32 plaintextLength,
                                                      P2CONST(void, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                      uint32 associatedDataLength,
                                                      P2VAR(void, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                      P2VAR(void, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr)
{
  /* #10 Call Csm_AEADEncrypt */
  return Csm_AEADEncrypt(jobId,
                         mode,
                         (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))plaintextPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                         plaintextLength,
                         (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))associatedDataPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                         associatedDataLength,
                         (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))ciphertextPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                         ciphertextLengthPtr,
                         (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))tagPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                         tagLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_AEADEncryptDataRef() */

/**********************************************************************************************************************
 *  Csm_AEADDecryptAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecryptAsr4_03(uint32 jobId,
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
  /* #10 Call Csm_AEADDecrypt */
  return Csm_AEADDecrypt(jobId,
                         mode,
                         ciphertextPtr,
                         ciphertextLength,
                         associatedDataPtr,
                         associatedDataLength,
                         tagPtr,
                         tagLength,
                         plaintextPtr,
                         plaintextLengthPtr,
                         verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_AEADDecryptAsr4_03() */

/**********************************************************************************************************************
 *  Csm_AEADDecryptAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecryptAsr4_04(uint32 jobId,
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
  /* #10 Call Csm_AEADDecrypt */
  return Csm_AEADDecrypt(jobId,
                         mode,
                         ciphertextPtr,
                         ciphertextLength,
                         associatedDataPtr,
                         associatedDataLength,
                         tagPtr,
                         tagLength,
                         plaintextPtr,
                         plaintextLengthPtr,
                         verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_AEADDecryptAsr4_04() */

/**********************************************************************************************************************
 *  Csm_AEADDecryptDataRefAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecryptDataRefAsr4_04(uint32 jobId,
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
  /* #10 Call Csm_AEADDecrypt */
  return Csm_AEADDecrypt(jobId,
                         mode,
                         ciphertextPtr,
                         ciphertextLength,
                         associatedDataPtr,
                         associatedDataLength,
                         tagPtr,
                         tagLength,
                         plaintextPtr,
                         plaintextLengthPtr,
                         verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_AEADDecryptDataRefAsr4_04() */

/**********************************************************************************************************************
 *  Csm_AEADDecryptDataRefAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecryptDataRefAsrR19_11(uint32 jobId,
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
  /* #10 Call Csm_AEADDecrypt */
  return Csm_AEADDecrypt(jobId,
                         mode,
                         ciphertextPtr,
                         ciphertextLength,
                         associatedDataPtr,
                         associatedDataLength,
                         tagPtr,
                         tagLength,
                         plaintextPtr,
                         plaintextLengthPtr,
                         verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_AEADDecryptDataRefAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_AEADDecryptDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecryptDataRef(uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(void, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                      uint32 ciphertextLength,
                                                      P2CONST(void, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                      uint32 associatedDataLength,
                                                      P2CONST(void, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                      uint32 tagLength,
                                                      P2VAR(void, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
                                                      P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_AEADDecrypt */
  return Csm_AEADDecrypt(jobId,
                         mode,
                         (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))ciphertextPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                         ciphertextLength,
                         (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))associatedDataPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                         associatedDataLength,
                         (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))tagPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                         tagLength,
                         (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))plaintextPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                         plaintextLengthPtr,
                         verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_AEADDecryptDataRef() */

/**********************************************************************************************************************
 *  Csm_SignatureGenerateAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerateAsr4_03(uint32 jobId,
                                                            Crypto_OperationModeType mode,
                                                            P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                            uint32 dataLength,
                                                            P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                            P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) signatureLengthPtr)
{
  /* #10 Call Csm_SignatureGenerate */
  return Csm_SignatureGenerate(jobId, mode, dataPtr, dataLength, signaturePtr, signatureLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_SignatureGenerateAsr4_03() */

/**********************************************************************************************************************
 *  Csm_SignatureGenerateAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerateAsr4_04(uint32 jobId,
                                                            Crypto_OperationModeType mode,
                                                            P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                            uint32 dataLength,
                                                            P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                            P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) signatureLengthPtr)
{
  /* #10 Call Csm_SignatureGenerate */
  return Csm_SignatureGenerate(jobId, mode, dataPtr, dataLength, signaturePtr, signatureLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_SignatureGenerateAsr4_04() */

/**********************************************************************************************************************
 *  Csm_SignatureGenerateDataRefAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerateDataRefAsr4_04(uint32 jobId,
                                                                   Crypto_OperationModeType mode,
                                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                   uint32 dataLength,
                                                                   P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                   P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) signatureLengthPtr)
{
  /* #10 Call Csm_SignatureGenerate */
  return Csm_SignatureGenerate(jobId, mode, dataPtr, dataLength, signaturePtr, signatureLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_SignatureGenerateDataRefAsr4_04() */

/**********************************************************************************************************************
 *  Csm_SignatureGenerateDataRefAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerateDataRefAsrR19_11(uint32 jobId,
                                                                     Crypto_OperationModeType mode,
                                                                     P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                     uint32 dataLength,
                                                                     P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                     P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) signatureLengthPtr)
{
  /* #10 Call Csm_SignatureGenerate */
  return Csm_SignatureGenerate(jobId, mode, dataPtr, dataLength, signaturePtr, signatureLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_SignatureGenerateDataRefAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_SignatureGenerateDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerateDataRef(uint32 jobId,
                                                            Crypto_OperationModeType mode,
                                                            P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                            uint32 dataLength,
                                                            P2VAR(void, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                            P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) signatureLengthPtr)
{
  /* #10 Call Csm_SignatureGenerate */
  return Csm_SignatureGenerate(jobId,
                               mode,
                               (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))dataPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                               dataLength,
                               (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))signaturePtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                               signatureLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_SignatureGenerateDataRef() */

/**********************************************************************************************************************
 *  Csm_SignatureVerifyAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerifyAsr4_03(uint32 jobId,
                                                          Crypto_OperationModeType mode,
                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                          uint32 dataLength,
                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                          uint32 signatureLength,
                                                          P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_SignatureVerify */
  return Csm_SignatureVerify(jobId, mode, dataPtr, dataLength, signaturePtr, signatureLength, verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_SignatureVerifyAsr4_03() */

/**********************************************************************************************************************
 *  Csm_SignatureVerifyAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerifyAsr4_04(uint32 jobId,
                                                          Crypto_OperationModeType mode,
                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                          uint32 dataLength,
                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                          uint32 signatureLength,
                                                          P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_SignatureVerify */
  return Csm_SignatureVerify(jobId, mode, dataPtr, dataLength, signaturePtr, signatureLength, verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_SignatureVerifyAsr4_04() */

/**********************************************************************************************************************
 *  Csm_SignatureVerifyDataRefAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerifyDataRefAsr4_04(uint32 jobId,
                                                                 Crypto_OperationModeType mode,
                                                                 P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                 uint32 dataLength,
                                                                 P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                 uint32 signatureLength,
                                                                 P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_SignatureVerify */
  return Csm_SignatureVerify(jobId, mode, dataPtr, dataLength, signaturePtr, signatureLength, verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_SignatureVerifyDataRefAsr4_04() */

/**********************************************************************************************************************
 *  Csm_SignatureVerifyDataRefAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerifyDataRefAsrR19_11(uint32 jobId,
                                                                   Crypto_OperationModeType mode,
                                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                   uint32 dataLength,
                                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                   uint32 signatureLength,
                                                                   P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_SignatureVerify */
  return Csm_SignatureVerify(jobId, mode, dataPtr, dataLength, signaturePtr, signatureLength, verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_SignatureVerifyDataRefAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_SignatureVerifyDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerifyDataRef(uint32 jobId,
                                                          Crypto_OperationModeType mode,
                                                          P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                          uint32 dataLength,
                                                          P2CONST(void, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                          uint32 signatureLength,
                                                          P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_SignatureVerify */
  return Csm_SignatureVerify(jobId,
                             mode,
                             (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))dataPtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                             dataLength,
                             (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))signaturePtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                             signatureLength,
                             verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_SignatureVerifyDataRef() */

/**********************************************************************************************************************
 *  Csm_RandomGenerateAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerateAsr4_03(uint32 jobId,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_RandomGenerate */
  return Csm_RandomGenerate(jobId, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_RandomGenerateAsr4_03() */

/**********************************************************************************************************************
 *  Csm_RandomGenerateDataRefAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerateDataRefAsr4_03(uint32 jobId,
                                                                P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                                P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_RandomGenerate */
  return Csm_RandomGenerate(jobId, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_RandomGenerateDataRefAsr4_03() */

/**********************************************************************************************************************
 *  Csm_RandomGenerateDataRefAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerateDataRefAsrR19_11(uint32 jobId,
                                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_RandomGenerate */
  return Csm_RandomGenerate(jobId, resultPtr, resultLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_RandomGenerateDataRefAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_RandomGenerateDataRef()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerateDataRef(uint32 jobId,
                                                         P2VAR(void, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr)
{
  /* #10 Call Csm_RandomGenerate */
  return Csm_RandomGenerate(jobId, (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))resultPtr, resultLengthPtr); /* PRQA S 0316 */ /* MD_CSM_0316 */ /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_RandomGenerateDataRef() */

/**********************************************************************************************************************
 *  Csm_CancelJobAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJobAsr4_03(uint32 jobId, Crypto_OperationModeType mode)
{
  /* #10 Call Csm_CancelJob */
  return Csm_CancelJob(jobId, mode);
} /* Csm_CancelJobAsr4_03() */

/**********************************************************************************************************************
 *  Csm_CancelJobAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJobAsr4_04(uint32 jobId, Crypto_OperationModeType mode)
{
  /* #10 Call Csm_CancelJob */
  return Csm_CancelJob(jobId, mode);
} /* Csm_CancelJobAsr4_04() */

/**********************************************************************************************************************
 *  Csm_CancelJobNoOpMode()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJobNoOpMode(uint32 jobId)
{
  /* #10 Call Csm_CancelJob */
  return Csm_CancelJob(jobId, CRYPTO_OPERATIONMODE_SINGLECALL);
} /* Csm_CancelJobNoOpMode() */

/**********************************************************************************************************************
 *  Csm_CancelJobNoOpModeAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJobNoOpModeAsr4_03(uint32 jobId)
{
  /* #10 Call Csm_CancelJob */
  return Csm_CancelJob(jobId, CRYPTO_OPERATIONMODE_SINGLECALL);
} /* Csm_CancelJobNoOpModeAsr4_03() */

/**********************************************************************************************************************
 *  Csm_CancelJobNoOpModeAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJobNoOpModeAsr4_04(uint32 jobId)
{
  /* #10 Call Csm_CancelJob */
  return Csm_CancelJob(jobId, CRYPTO_OPERATIONMODE_SINGLECALL);
} /* Csm_CancelJobNoOpModeAsr4_04() */

/**********************************************************************************************************************
 *  Csm_JobKeyDeriveAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyDeriveAsr4_03(uint32 jobId, uint32 keyId, uint32 targetKeyId)
{
  /* #10 Call Csm_JobKeyDerive44x */
  return Csm_JobKeyDerive44x(jobId, keyId, targetKeyId);
} /* Csm_JobKeyDeriveAsr4_03() */

/**********************************************************************************************************************
 *  Csm_JobKeyDeriveAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyDeriveAsrR19_11(uint32 jobId, uint32 keyId, uint32 targetKeyId)
{
  /* #10 Call Csm_JobKeyDerive44x */
  return Csm_JobKeyDerive44x(jobId, keyId, targetKeyId);
} /* Csm_JobKeyDeriveAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcPubValAsrR21_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcPubValAsrR21_11(uint32 jobId,
                                                                     P2VAR(void, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                     P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr)
{
  /* #10 Call Csm_JobKeyExchangeCalcPubVal */
  return Csm_JobKeyExchangeCalcPubVal(jobId, (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))publicValuePtr, publicValueLengthPtr); /* PRQA S 0316 */ /* MD_CSM_0316 */ /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_JobKeyExchangeCalcPubValAsrR21_11() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcPubValAsrR20_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcPubValAsrR20_11(uint32 jobId,
                                                                     uint32 keyId,
                                                                     P2VAR(void, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                     P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr)
{
  /* #10 Call Csm_JobKeyExchangeCalcPubVal44x */
  return Csm_JobKeyExchangeCalcPubVal44x(jobId,
                                         keyId,
                                         (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))publicValuePtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                                         publicValueLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_JobKeyExchangeCalcPubValAsrR20_11() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcPubValAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcPubValAsr4_03(uint32 jobId,
                                                                   uint32 keyId,
                                                                   P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                   P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr)
{
  /* #10 Call Csm_JobKeyExchangeCalcPubVal44x */
  return Csm_JobKeyExchangeCalcPubVal44x(jobId, keyId, publicValuePtr, publicValueLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_JobKeyExchangeCalcPubValAsr4_03() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcPubValAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcPubValAsrR19_11(uint32 jobId,
                                                                     uint32 keyId,
                                                                     P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                     P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr)
{
  /* #10 Call Csm_JobKeyExchangeCalcPubVal44x */
  return Csm_JobKeyExchangeCalcPubVal44x(jobId, keyId, publicValuePtr, publicValueLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_JobKeyExchangeCalcPubValAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcSecretAsrR21_11_A()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcSecretAsrR21_11_A(uint32 jobId,
                                                                       P2CONST(void, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                       uint32 partnerPublicValueLength)
{
  /* #10 Call Csm_JobKeyExchangeCalcSecret */
  return Csm_JobKeyExchangeCalcSecret(jobId,
                                      (P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR))partnerPublicValuePtr, /* PRQA S 0316 */ /* MD_CSM_0316 */
                                      partnerPublicValueLength); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_JobKeyExchangeCalcSecretAsrR21_11_A() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcSecretAsrR21_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcSecretAsrR21_11(uint32 jobId,
                                                                     P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                     uint32 partnerPublicValueLength)
{
  /* #10 Call Csm_JobKeyExchangeCalcSecret */
  return Csm_JobKeyExchangeCalcSecret(jobId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_JobKeyExchangeCalcSecretAsrR21_11() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcSecretAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcSecretAsr4_03(uint32 jobId,
                                                                   uint32 keyId,
                                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                   uint32 partnerPublicValueLength)
{
  /* #10 Call Csm_JobKeyExchangeCalcSecret44x */
  return Csm_JobKeyExchangeCalcSecret44x(jobId, keyId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_JobKeyExchangeCalcSecretAsr4_03() */

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcSecretAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcSecretAsrR19_11(uint32 jobId,
                                                                     uint32 keyId,
                                                                     P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                     uint32 partnerPublicValueLength)
{
  /* #10 Call Csm_JobKeyExchangeCalcSecret44x */
  return Csm_JobKeyExchangeCalcSecret44x(jobId, keyId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_JobKeyExchangeCalcSecretAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_JobKeyGenerateAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyGenerateAsr4_03(uint32 jobId, uint32 keyId)
{
  /* #10 Call Csm_JobKeyGenerate44x */
  return Csm_JobKeyGenerate44x(jobId, keyId);
} /* Csm_JobKeyGenerateAsr4_03() */

/**********************************************************************************************************************
 *  Csm_JobKeyGenerateAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyGenerateAsrR19_11(uint32 jobId, uint32 keyId)
{
  /* #10 Call Csm_JobKeyGenerate44x */
  return Csm_JobKeyGenerate44x(jobId, keyId);
} /* Csm_JobKeyGenerateAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_JobKeySetInvalidAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeySetInvalidAsr4_03(uint32 jobId, uint32 keyId)
{
  /* #10 Call Csm_JobKeySetInvalid44x */
  return Csm_JobKeySetInvalid44x(jobId, keyId);
} /* Csm_JobKeySetInvalidAsr4_03() */

/**********************************************************************************************************************
 *  Csm_JobKeySetValidAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeySetValidAsr4_03(uint32 jobId, uint32 keyId)
{
  /* #10 Call Csm_JobKeySetValid44x */
  return Csm_JobKeySetValid44x(jobId, keyId);
} /* Csm_JobKeySetValidAsr4_03() */

/**********************************************************************************************************************
 *  Csm_JobKeySetValidAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeySetValidAsrR19_11(uint32 jobId, uint32 keyId)
{
  /* #10 Call Csm_JobKeySetValid44x */
  return Csm_JobKeySetValid44x(jobId, keyId);
} /* Csm_JobKeySetValidAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_JobRandomSeedAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobRandomSeedAsr4_03(uint32 jobId,
                                                        uint32 keyId,
                                                        P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) seedPtr,
                                                        uint32 seedLength)
{
  /* #10 Call Csm_JobRandomSeed44x */
  return Csm_JobRandomSeed44x(jobId, keyId, seedPtr, seedLength); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_JobRandomSeedAsr4_03() */

/**********************************************************************************************************************
 *  Csm_JobRandomSeedAsrR19_11()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_JobRandomSeedAsrR19_11(uint32 jobId,
                                                          uint32 keyId,
                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) seedPtr,
                                                          uint32 seedLength)
{
  /* #10 Call Csm_JobRandomSeed44x */
  return Csm_JobRandomSeed44x(jobId, keyId, seedPtr, seedLength); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_JobRandomSeedAsrR19_11() */

/**********************************************************************************************************************
 *  Csm_CertificateParseAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateParseAsr4_03(uint32 keyId)
{
  /* #10 Call Csm_CertificateParse */
  return Csm_CertificateParse(keyId);
} /* Csm_CertificateParseAsr4_03() */

/**********************************************************************************************************************
 *  Csm_CertificateVerifyAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateVerifyAsr4_03(uint32 keyId,
                                                            uint32 verifyKeyId,
                                                            P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr)
{
  /* #10 Call Csm_CertificateVerify */
  return Csm_CertificateVerify(keyId, verifyKeyId, verifyPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_CertificateVerifyAsr4_03() */

/**********************************************************************************************************************
 *  Csm_KeyCopyAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopyAsr4_03(uint32 keyId, uint32 targetKeyId)
{
  /* #10 Call Csm_KeyCopy */
  return Csm_KeyCopy(keyId, targetKeyId);
} /* Csm_KeyCopyAsr4_03() */

/**********************************************************************************************************************
 *  Csm_KeyCopyAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopyAsr4_04(uint32 keyId, uint32 targetKeyId)
{
  /* #10 Call Csm_KeyCopy */
  return Csm_KeyCopy(keyId, targetKeyId);
} /* Csm_KeyCopyAsr4_04() */

/**********************************************************************************************************************
 *  Csm_KeyDeriveAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDeriveAsr4_03(uint32 keyId, uint32 targetKeyId)
{
  /* #10 Call Csm_KeyDerive */
  return Csm_KeyDerive(keyId, targetKeyId);
} /* Csm_KeyDeriveAsr4_03() */

/**********************************************************************************************************************
 *  Csm_KeyDeriveAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDeriveAsr4_04(uint32 keyId, uint32 targetKeyId)
{
  /* #10 Call Csm_KeyDerive */
  return Csm_KeyDerive(keyId, targetKeyId);
} /* Csm_KeyDeriveAsr4_04() */

/**********************************************************************************************************************
 *  Csm_KeyElementCopyAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopyAsr4_03(uint32 keyId,
                                                         uint32 keyElementId,
                                                         uint32 targetKeyId,
                                                         uint32 targetKeyElementId)
{
  /* #10 Call Csm_KeyElementCopy */
  return Csm_KeyElementCopy(keyId, keyElementId, targetKeyId, targetKeyElementId);
} /* Csm_KeyElementCopyAsr4_03() */

/**********************************************************************************************************************
 *  Csm_KeyElementCopyAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopyAsr4_04(uint32 keyId,
                                                         uint32 keyElementId,
                                                         uint32 targetKeyId,
                                                         uint32 targetKeyElementId)
{
  /* #10 Call Csm_KeyElementCopy */
  return Csm_KeyElementCopy(keyId, keyElementId, targetKeyId, targetKeyElementId);
} /* Csm_KeyElementCopyAsr4_04() */

/**********************************************************************************************************************
 *  Csm_KeyElementCopyPartialAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopyPartialAsr4_03(uint32 keyId,
                                                                uint32 keyElementId,
                                                                uint32 keyElementSourceOffset,
                                                                uint32 keyElementTargetOffset,
                                                                uint32 keyElementCopyLength,
                                                                uint32 targetKeyId,
                                                                uint32 targetKeyElementId)
{
  /* #10 Call Csm_KeyElementCopyPartial */
  return Csm_KeyElementCopyPartial(keyId,
                                   keyElementId,
                                   keyElementSourceOffset,
                                   keyElementTargetOffset,
                                   keyElementCopyLength,
                                   targetKeyId,
                                   targetKeyElementId);
} /* Csm_KeyElementCopyPartialAsr4_03() */

/**********************************************************************************************************************
 *  Csm_KeyElementGetAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGetAsr4_03(uint32 keyId,
                                                        uint32 keyElementId,
                                                        P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
                                                        P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) keyLengthPtr)
{
  /* #10 Call Csm_KeyElementGet */
  return Csm_KeyElementGet(keyId, keyElementId, keyPtr, keyLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_KeyElementGetAsr4_03() */

/**********************************************************************************************************************
 *  Csm_KeyElementGetAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGetAsr4_04(uint32 keyId,
                                                        uint32 keyElementId,
                                                        P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
                                                        P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) keyLengthPtr)
{
  /* #10 Call Csm_KeyElementGet */
  return Csm_KeyElementGet(keyId, keyElementId, keyPtr, keyLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_KeyElementGetAsr4_04() */

/**********************************************************************************************************************
 *  Csm_KeyElementSetAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementSetAsr4_03(uint32 keyId,
                                                        uint32 keyElementId,
                                                        P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
                                                        uint32 keyLength)
{
  /* #10 Call Csm_KeyElementSet */
  return Csm_KeyElementSet(keyId, keyElementId, keyPtr, keyLength); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_KeyElementSetAsr4_03() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcPubValAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubValAsr4_03(uint32 keyId,
                                                                P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr)
{
  /* #10 Call Csm_KeyExchangeCalcPubVal */
  return Csm_KeyExchangeCalcPubVal(keyId, publicValuePtr, publicValueLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_KeyExchangeCalcPubValAsr4_03() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcPubValAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubValAsr4_04(uint32 keyId,
                                                                P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr)
{
  /* #10 Call Csm_KeyExchangeCalcPubVal */
  return Csm_KeyExchangeCalcPubVal(keyId, publicValuePtr, publicValueLengthPtr); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_KeyExchangeCalcPubValAsr4_04() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcSecretAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecretAsr4_03(uint32 keyId,
                                                                P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                uint32 partnerPublicValueLength)
{
  /* #10 Call Csm_KeyExchangeCalcSecret */
  return Csm_KeyExchangeCalcSecret(keyId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_KeyExchangeCalcSecretAsr4_03() */

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcSecretAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecretAsr4_04(uint32 keyId,
                                                                P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                uint32 partnerPublicValueLength)
{
  /* #10 Call Csm_KeyExchangeCalcSecret */
  return Csm_KeyExchangeCalcSecret(keyId, partnerPublicValuePtr, partnerPublicValueLength); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_KeyExchangeCalcSecretAsr4_04() */

/**********************************************************************************************************************
 *  Csm_KeyGenerateAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerateAsr4_03(uint32 keyId)
{
  /* #10 Call Csm_KeyGenerate */
  return Csm_KeyGenerate(keyId);
} /* Csm_KeyGenerateAsr4_03() */

/**********************************************************************************************************************
 *  Csm_KeyGenerateAsr4_04()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerateAsr4_04(uint32 keyId)
{
  /* #10 Call Csm_KeyGenerate */
  return Csm_KeyGenerate(keyId);
} /* Csm_KeyGenerateAsr4_04() */

/**********************************************************************************************************************
 *  Csm_KeySetValidAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_KeySetValidAsr4_03(uint32 keyId)
{
  /* #10 Call Csm_KeySetValid */
  return Csm_KeySetValid(keyId);
} /* Csm_KeySetValidAsr4_03() */

/**********************************************************************************************************************
 *  Csm_RandomSeedAsr4_03()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_RandomSeedAsr4_03(uint32 keyId,
                                                     P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) seedPtr,
                                                     uint32 seedLength)
{
  /* #10 Call Csm_RandomSeed */
  return Csm_RandomSeed(keyId, seedPtr, seedLength); /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_RandomSeedAsr4_03() */

/**********************************************************************************************************************
 *  Csm_SaveContextJobWithVoidPtr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_SaveContextJobWithVoidPtr(uint32 jobId,
                                                             P2VAR(void, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) contextBufferLengthPtr)
{
  /* #10 Call Csm_SaveContextJob */
  return Csm_SaveContextJob(jobId, (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))contextBufferPtr, contextBufferLengthPtr); /* PRQA S 0316 */ /* MD_CSM_0316 */ /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_SaveContextJobWithVoidPtr() */

/**********************************************************************************************************************
 *  Csm_RestoreContextJobWithVoidPtr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, CSM_CODE) Csm_RestoreContextJobWithVoidPtr(uint32 jobId,
                                                                P2VAR(void, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr,
                                                                uint32 contextBufferLength)
{
  /* #10 Call Csm_RestoreContextJob */
  return Csm_RestoreContextJob(jobId, (P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR))contextBufferPtr, contextBufferLength); /* PRQA S 0316 */ /* MD_CSM_0316 */ /* SBSW_CSM_API_CALL_FORWARDING_PTR_WRAPPER */
} /* Csm_RestoreContextJobWithVoidPtr() */

# define CSM_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (CSM_RTE_PORTS == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Csm_Rte.c
 *********************************************************************************************************************/
