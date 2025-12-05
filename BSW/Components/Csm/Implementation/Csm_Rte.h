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
/*      \file   Csm_Rte.h
 *      \brief  MICROSAR Crypto Service Manager (CSM) RTE
 *
 *      \details  Description of the MICROSAR Crypto Service Manager (CSM) service APIs used only by RTE
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CSM_RTE_H)
# define CSM_RTE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Csm.h"

 /* Usage check */
# if !defined (CSM_RTE_SOURCE)
#  error "This is a private header and should only be included by Csm_Rte.c"
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# if (CSM_RTE_PORTS == STD_ON)

#  define CSM_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  Csm_HashAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Hash().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_HashAsr4_03(uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                      uint32 dataLength,
                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_HashDataRefAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Hash().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_HashDataRefAsr4_03(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                             uint32 dataLength,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_HashDataRefAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Hash().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_HashDataRefAsrR19_11(uint32 jobId,
                                                               Crypto_OperationModeType mode,
                                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                               uint32 dataLength,
                                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_HashDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Hash().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_HashDataRef(uint32 jobId,
                                                      Crypto_OperationModeType mode,
                                                      P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                      uint32 dataLength,
                                                      P2VAR(void, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_MacGenerateAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerateAsr4_03(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                             uint32 dataLength,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr);

/**********************************************************************************************************************
 *  Csm_MacGenerateAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerateAsr4_04(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                             uint32 dataLength,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr);

/**********************************************************************************************************************
 *  Csm_MacGenerateDataRefAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerateDataRefAsr4_04(uint32 jobId,
                                                                    Crypto_OperationModeType mode,
                                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                    uint32 dataLength,
                                                                    P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                                    P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr);

/**********************************************************************************************************************
 *  Csm_MacGenerateDataRefAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerateDataRefAsrR19_11(uint32 jobId,
                                                                      Crypto_OperationModeType mode,
                                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                      uint32 dataLength,
                                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr);

/**********************************************************************************************************************
 *  Csm_MacGenerateDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacGenerateDataRef(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                             uint32 dataLength,
                                                             P2VAR(void, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) macLengthPtr);

/**********************************************************************************************************************
 *  Csm_MacVerifyAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacVerify().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerifyAsr4_03(uint32 jobId,
                                                           Crypto_OperationModeType mode,
                                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                           uint32 dataLength,
                                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                           uint32 macLength,
                                                           P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_MacVerifyAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacVerify().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerifyAsr4_04(uint32 jobId,
                                                           Crypto_OperationModeType mode,
                                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                           uint32 dataLength,
                                                           P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                           uint32 macLength,
                                                           P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_MacVerifyDataRefAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacVerify().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerifyDataRefAsr4_04(uint32 jobId,
                                                                  Crypto_OperationModeType mode,
                                                                  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                  uint32 dataLength,
                                                                  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                                  uint32 macLength,
                                                                  P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_MacVerifyDataRefAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacVerify().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerifyDataRefAsrR19_11(uint32 jobId,
                                                                    Crypto_OperationModeType mode,
                                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                    uint32 dataLength,
                                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                                    uint32 macLength,
                                                                    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_MacVerifyDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_MacVerify().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_MacVerifyDataRef(uint32 jobId,
                                                           Crypto_OperationModeType mode,
                                                           P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                           uint32 dataLength,
                                                           P2CONST(void, AUTOMATIC, CSM_APPL_VAR) macPtr,
                                                           uint32 macLength,
                                                           P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_EncryptAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Encrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_EncryptAsr4_03(uint32 jobId,
                                                         Crypto_OperationModeType mode,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                         uint32 dataLength,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_EncryptAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Encrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_EncryptAsr4_04(uint32 jobId,
                                                         Crypto_OperationModeType mode,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                         uint32 dataLength,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_EncryptDataRefAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Encrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_EncryptDataRefAsr4_04(uint32 jobId,
                                                                Crypto_OperationModeType mode,
                                                                P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                uint32 dataLength,
                                                                P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                                P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_EncryptDataRefAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Encrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_EncryptDataRefAsrR19_11(uint32 jobId,
                                                                  Crypto_OperationModeType mode,
                                                                  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                  uint32 dataLength,
                                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_EncryptDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Encrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_EncryptDataRef(uint32 jobId,
                                                         Crypto_OperationModeType mode,
                                                         P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                         uint32 dataLength,
                                                         P2VAR(void, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_DecryptAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Decrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_DecryptAsr4_03(uint32 jobId,
                                                         Crypto_OperationModeType mode,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                         uint32 dataLength,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_DecryptAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Decrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_DecryptAsr4_04(uint32 jobId,
                                                         Crypto_OperationModeType mode,
                                                         P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                         uint32 dataLength,
                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_DecryptDataRefAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Decrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_DecryptDataRefAsr4_04(uint32 jobId,
                                                                Crypto_OperationModeType mode,
                                                                P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                uint32 dataLength,
                                                                P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                                P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_DecryptDataRefAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Decrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_DecryptDataRefAsrR19_11(uint32 jobId,
                                                                  Crypto_OperationModeType mode,
                                                                  P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                  uint32 dataLength,
                                                                  P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                                  P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_DecryptDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_Decrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_DecryptDataRef(uint32 jobId,
                                                         Crypto_OperationModeType mode,
                                                         P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                         uint32 dataLength,
                                                         P2VAR(void, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_AEADEncryptAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADEncrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncryptAsr4_03(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                             uint32 plaintextLength,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                             uint32 associatedDataLength,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr);

/**********************************************************************************************************************
 *  Csm_AEADEncryptAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADEncrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncryptAsr4_04(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                             uint32 plaintextLength,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                             uint32 associatedDataLength,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr);

/**********************************************************************************************************************
 *  Csm_AEADEncryptDataRefAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADEncrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncryptDataRefAsr4_04(uint32 jobId,
                                                                    Crypto_OperationModeType mode,
                                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                                    uint32 plaintextLength,
                                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                                    uint32 associatedDataLength,
                                                                    P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                                    P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                                    P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                                    P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr);

/**********************************************************************************************************************
 *  Csm_AEADEncryptDataRefAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADEncrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncryptDataRefAsrR19_11(uint32 jobId,
                                                                      Crypto_OperationModeType mode,
                                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                                      uint32 plaintextLength,
                                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                                      uint32 associatedDataLength,
                                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr);

/**********************************************************************************************************************
 *  Csm_AEADEncryptDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADEncrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADEncryptDataRef(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(void, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                             uint32 plaintextLength,
                                                             P2CONST(void, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                             uint32 associatedDataLength,
                                                             P2VAR(void, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) ciphertextLengthPtr,
                                                             P2VAR(void, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) tagLengthPtr);

/**********************************************************************************************************************
 *  Csm_AEADDecryptAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADDecrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecryptAsr4_03(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                             uint32 ciphertextLength,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                             uint32 associatedDataLength,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                             uint32 tagLength,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
                                                             P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_AEADDecryptAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADDecrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecryptAsr4_04(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                             uint32 ciphertextLength,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                             uint32 associatedDataLength,
                                                             P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                             uint32 tagLength,
                                                             P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
                                                             P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_AEADDecryptDataRefAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADDecrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecryptDataRefAsr4_04(uint32 jobId,
                                                                    Crypto_OperationModeType mode,
                                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                                    uint32 ciphertextLength,
                                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                                    uint32 associatedDataLength,
                                                                    P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                                    uint32 tagLength,
                                                                    P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                                    P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
                                                                    P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_AEADDecryptDataRefAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADDecrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecryptDataRefAsrR19_11(uint32 jobId,
                                                                      Crypto_OperationModeType mode,
                                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                                      uint32 ciphertextLength,
                                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                                      uint32 associatedDataLength,
                                                                      P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                                      uint32 tagLength,
                                                                      P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                                      P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
                                                                      P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_AEADDecryptDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_AEADDecrypt().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_AEADDecryptDataRef(uint32 jobId,
                                                             Crypto_OperationModeType mode,
                                                             P2CONST(void, AUTOMATIC, CSM_APPL_VAR) ciphertextPtr,
                                                             uint32 ciphertextLength,
                                                             P2CONST(void, AUTOMATIC, CSM_APPL_VAR) associatedDataPtr,
                                                             uint32 associatedDataLength,
                                                             P2CONST(void, AUTOMATIC, CSM_APPL_VAR) tagPtr,
                                                             uint32 tagLength,
                                                             P2VAR(void, AUTOMATIC, CSM_APPL_VAR) plaintextPtr,
                                                             P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) plaintextLengthPtr,
                                                             P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_SignatureGenerateAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerateAsr4_03(uint32 jobId,
                                                                   Crypto_OperationModeType mode,
                                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                   uint32 dataLength,
                                                                   P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                   P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) signatureLengthPtr);

/**********************************************************************************************************************
 *  Csm_SignatureGenerateAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerateAsr4_04(uint32 jobId,
                                                                   Crypto_OperationModeType mode,
                                                                   P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                   uint32 dataLength,
                                                                   P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                   P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) signatureLengthPtr);

/**********************************************************************************************************************
 *  Csm_SignatureGenerateDataRefAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerateDataRefAsr4_04(uint32 jobId,
                                                                          Crypto_OperationModeType mode,
                                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                          uint32 dataLength,
                                                                          P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                          P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) signatureLengthPtr);

/**********************************************************************************************************************
 *  Csm_SignatureGenerateDataRefAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerateDataRefAsrR19_11(uint32 jobId,
                                                                            Crypto_OperationModeType mode,
                                                                            P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                            uint32 dataLength,
                                                                            P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                            P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) signatureLengthPtr);

/**********************************************************************************************************************
 *  Csm_SignatureGenerateDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureGenerateDataRef(uint32 jobId,
                                                                   Crypto_OperationModeType mode,
                                                                   P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                   uint32 dataLength,
                                                                   P2VAR(void, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                   P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) signatureLengthPtr);

/**********************************************************************************************************************
 *  Csm_SignatureVerifyAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureVerify().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerifyAsr4_03(uint32 jobId,
                                                                 Crypto_OperationModeType mode,
                                                                 P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                 uint32 dataLength,
                                                                 P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                 uint32 signatureLength,
                                                                 P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_SignatureVerifyAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureVerify().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerifyAsr4_04(uint32 jobId,
                                                                 Crypto_OperationModeType mode,
                                                                 P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                 uint32 dataLength,
                                                                 P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                 uint32 signatureLength,
                                                                 P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_SignatureVerifyDataRefAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureVerify().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerifyDataRefAsr4_04(uint32 jobId,
                                                                        Crypto_OperationModeType mode,
                                                                        P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                        uint32 dataLength,
                                                                        P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                        uint32 signatureLength,
                                                                        P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_SignatureVerifyDataRefAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureVerify().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerifyDataRefAsrR19_11(uint32 jobId,
                                                                          Crypto_OperationModeType mode,
                                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                          uint32 dataLength,
                                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                          uint32 signatureLength,
                                                                          P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_SignatureVerifyDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SignatureVerify().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SignatureVerifyDataRef(uint32 jobId,
                                                                 Crypto_OperationModeType mode,
                                                                 P2CONST(void, AUTOMATIC, CSM_APPL_VAR) dataPtr,
                                                                 uint32 dataLength,
                                                                 P2CONST(void, AUTOMATIC, CSM_APPL_VAR) signaturePtr,
                                                                 uint32 signatureLength,
                                                                 P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_RandomGenerateAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_RandomGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerateAsr4_03(uint32 jobId,
                                                                P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                                P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_RandomGenerateDataRefAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_RandomGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerateDataRefAsr4_03(uint32 jobId,
                                                                       P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                                       P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_RandomGenerateDataRefAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_RandomGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerateDataRefAsrR19_11(uint32 jobId,
                                                                         P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                                         P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_RandomGenerateDataRef()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_RandomGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_RandomGenerateDataRef(uint32 jobId,
                                                                P2VAR(void, AUTOMATIC, CSM_APPL_VAR) resultPtr,
                                                                P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) resultLengthPtr);

/**********************************************************************************************************************
 *  Csm_CancelJobAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_CancelJob().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJobAsr4_03(uint32 jobId, Crypto_OperationModeType mode);

/**********************************************************************************************************************
 *  Csm_CancelJobAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_CancelJob().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJobAsr4_04(uint32 jobId, Crypto_OperationModeType mode);

/**********************************************************************************************************************
 *  Csm_CancelJobNoOpMode()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_CancelJob().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJobNoOpMode(uint32 jobId);

/**********************************************************************************************************************
 *  Csm_CancelJobNoOpModeAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_CancelJob().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJobNoOpModeAsr4_03(uint32 jobId);

/**********************************************************************************************************************
 *  Csm_CancelJobNoOpModeAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_CancelJob().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CancelJobNoOpModeAsr4_04(uint32 jobId);

/**********************************************************************************************************************
 *  Csm_JobKeyDeriveAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyDerive44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyDeriveAsr4_03(uint32 jobId, uint32 keyId, uint32 targetKeyId);

/**********************************************************************************************************************
 *  Csm_JobKeyDeriveAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyDerive44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyDeriveAsrR19_11(uint32 jobId, uint32 keyId, uint32 targetKeyId);

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcPubValAsrR21_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyExchangeCalcPubVal().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcPubValAsrR21_11(uint32 jobId,
                                                                            P2VAR(void, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                            P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcPubValAsrR20_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyExchangeCalcPubVal44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcPubValAsrR20_11(uint32 jobId,
                                                                            uint32 keyId,
                                                                            P2VAR(void, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                            P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcPubValAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyExchangeCalcPubVal44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcPubValAsr4_03(uint32 jobId,
                                                                          uint32 keyId,
                                                                          P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                          P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcPubValAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyExchangeCalcPubVal44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcPubValAsrR19_11(uint32 jobId,
                                                                            uint32 keyId,
                                                                            P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                            P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcSecretAsrR21_11_A()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyExchangeCalcSecret().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcSecretAsrR21_11_A(uint32 jobId,
                                                                              P2CONST(void, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                              uint32 partnerPublicValueLength);

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcSecretAsrR21_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyExchangeCalcSecret().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcSecretAsrR21_11(uint32 jobId,
                                                                            P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                            uint32 partnerPublicValueLength);

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcSecretAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyExchangeCalcSecret44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcSecretAsr4_03(uint32 jobId,
                                                                          uint32 keyId,
                                                                          P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                          uint32 partnerPublicValueLength);

/**********************************************************************************************************************
 *  Csm_JobKeyExchangeCalcSecretAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyExchangeCalcSecret44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyExchangeCalcSecretAsrR19_11(uint32 jobId,
                                                                            uint32 keyId,
                                                                            P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                            uint32 partnerPublicValueLength);

/**********************************************************************************************************************
 *  Csm_JobKeyGenerateAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyGenerate44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyGenerateAsr4_03(uint32 jobId, uint32 keyId);

/**********************************************************************************************************************
 *  Csm_JobKeyGenerateAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeyGenerate44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeyGenerateAsrR19_11(uint32 jobId, uint32 keyId);

/**********************************************************************************************************************
 *  Csm_JobKeySetInvalidAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeySetInvalid44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeySetInvalidAsr4_03(uint32 jobId, uint32 keyId);

/**********************************************************************************************************************
 *  Csm_JobKeySetValidAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeySetValid44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeySetValidAsr4_03(uint32 jobId, uint32 keyId);

/**********************************************************************************************************************
 *  Csm_JobKeySetValidAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobKeySetValid44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobKeySetValidAsrR19_11(uint32 jobId, uint32 keyId);

/**********************************************************************************************************************
 *  Csm_JobRandomSeedAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobRandomSeed44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobRandomSeedAsr4_03(uint32 jobId,
                                                               uint32 keyId,
                                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) seedPtr,
                                                               uint32 seedLength);

/**********************************************************************************************************************
 *  Csm_JobRandomSeedAsrR19_11()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_JobRandomSeed44x().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_JobRandomSeedAsrR19_11(uint32 jobId,
                                                                 uint32 keyId,
                                                                 P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) seedPtr,
                                                                 uint32 seedLength);

/**********************************************************************************************************************
 *  Csm_CertificateParseAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_CertificateParse().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateParseAsr4_03(uint32 keyId);

/**********************************************************************************************************************
 *  Csm_CertificateVerifyAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_CertificateVerify().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_CertificateVerifyAsr4_03(uint32 keyId,
                                                                   uint32 verifyKeyId,
                                                                   P2VAR(Crypto_VerifyResultType, AUTOMATIC, CSM_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  Csm_KeyCopyAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyCopy().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopyAsr4_03(uint32 keyId, uint32 targetKeyId);

/**********************************************************************************************************************
 *  Csm_KeyCopyAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyCopy().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyCopyAsr4_04(uint32 keyId, uint32 targetKeyId);

/**********************************************************************************************************************
 *  Csm_KeyDeriveAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyDerive().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDeriveAsr4_03(uint32 keyId, uint32 targetKeyId);

/**********************************************************************************************************************
 *  Csm_KeyDeriveAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyDerive().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyDeriveAsr4_04(uint32 keyId, uint32 targetKeyId);

/**********************************************************************************************************************
 *  Csm_KeyElementCopyAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyElementCopy().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopyAsr4_03(uint32 keyId,
                                                                uint32 keyElementId,
                                                                uint32 targetKeyId,
                                                                uint32 targetKeyElementId);

/**********************************************************************************************************************
 *  Csm_KeyElementCopyAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyElementCopy().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopyAsr4_04(uint32 keyId,
                                                                uint32 keyElementId,
                                                                uint32 targetKeyId,
                                                                uint32 targetKeyElementId);

/**********************************************************************************************************************
 *  Csm_KeyElementCopyPartialAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyElementCopyPartial().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementCopyPartialAsr4_03(uint32 keyId,
                                                                       uint32 keyElementId,
                                                                       uint32 keyElementSourceOffset,
                                                                       uint32 keyElementTargetOffset,
                                                                       uint32 keyElementCopyLength,
                                                                       uint32 targetKeyId,
                                                                       uint32 targetKeyElementId);

/**********************************************************************************************************************
 *  Csm_KeyElementGetAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyElementGet().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGetAsr4_03(uint32 keyId,
                                                               uint32 keyElementId,
                                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
                                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) keyLengthPtr);

/**********************************************************************************************************************
 *  Csm_KeyElementGetAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyElementGet().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementGetAsr4_04(uint32 keyId,
                                                               uint32 keyElementId,
                                                               P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
                                                               P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) keyLengthPtr);

/**********************************************************************************************************************
 *  Csm_KeyElementSetAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyElementSet().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyElementSetAsr4_03(uint32 keyId,
                                                               uint32 keyElementId,
                                                               P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) keyPtr,
                                                               uint32 keyLength);

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcPubValAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyExchangeCalcPubVal().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubValAsr4_03(uint32 keyId,
                                                                       P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                       P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcPubValAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyExchangeCalcPubVal().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcPubValAsr4_04(uint32 keyId,
                                                                       P2VAR(uint8, AUTOMATIC, CSM_APPL_VAR) publicValuePtr,
                                                                       P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) publicValueLengthPtr);

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcSecretAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyExchangeCalcSecret().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecretAsr4_03(uint32 keyId,
                                                                       P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                       uint32 partnerPublicValueLength);

/**********************************************************************************************************************
 *  Csm_KeyExchangeCalcSecretAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyExchangeCalcSecret().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyExchangeCalcSecretAsr4_04(uint32 keyId,
                                                                       P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) partnerPublicValuePtr,
                                                                       uint32 partnerPublicValueLength);

/**********************************************************************************************************************
 *  Csm_KeyGenerateAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerateAsr4_03(uint32 keyId);

/**********************************************************************************************************************
 *  Csm_KeyGenerateAsr4_04()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeyGenerate().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeyGenerateAsr4_04(uint32 keyId);

/**********************************************************************************************************************
 *  Csm_KeySetValidAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_KeySetValid().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_KeySetValidAsr4_03(uint32 keyId);

/**********************************************************************************************************************
 *  Csm_RandomSeedAsr4_03()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_RandomSeed().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_RandomSeedAsr4_03(uint32 keyId,
                                                            P2CONST(uint8, AUTOMATIC, CSM_APPL_VAR) seedPtr,
                                                            uint32 seedLength);

/**********************************************************************************************************************
 *  Csm_SaveContextJobWithVoidPtr()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_SaveContextJob().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_SaveContextJobWithVoidPtr(uint32 jobId,
                                                                    P2VAR(void, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr,
                                                                    P2VAR(uint32, AUTOMATIC, CSM_APPL_VAR) contextBufferLengthPtr);

/**********************************************************************************************************************
 *  Csm_RestoreContextJobWithVoidPtr()
 *********************************************************************************************************************/
/*! \brief         Wrapper function of Csm_RestoreContextJob().
 *  \details       See original function for behavior, parameter and return value description.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Shall only be called by Rte.
 *********************************************************************************************************************/
extern FUNC(Std_ReturnType, CSM_CODE) Csm_RestoreContextJobWithVoidPtr(uint32 jobId,
                                                                       P2VAR(void, AUTOMATIC, CSM_APPL_VAR) contextBufferPtr,
                                                                       uint32 contextBufferLength);

#  define CSM_STOP_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (CSM_RTE_PORTS == STD_ON) */

#endif /* !defined (CSM_RTE_H) */
/**********************************************************************************************************************
 *  END OF FILE: Csm_Rte.h
 *********************************************************************************************************************/
