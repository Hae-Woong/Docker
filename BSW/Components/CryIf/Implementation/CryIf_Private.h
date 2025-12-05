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
/**        \file  CryIf_Private.h
 *        \brief  MICROSAR Crypto Interface (CRYIF)
 *      \details  The Crypto Interface module provides a unique interface to manage different Crypto HW and SW
 *                solutions. This header file contains stub function declarations for services that are not supported
 *                by the underlying Crypto.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYIF_PRIVATE_H)
# define CRYIF_PRIVATE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Std_Types.h"
# include "Crypto_GeneralTypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CRYIF_START_SEC_CODE
# include "CryIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  CryIf_KeyElementCopyPartial_Stub()
 *********************************************************************************************************************/
/*! \brief         Returns E_NOT_OK.
 *  \details       This function is referenced in the crypto functions array if the underyling Crypto Driver does not
 *                 support KeyElementCopyPartial Service. This function should never be called and is only existent to
 *                 overcome Compiler / Linker errors.
 *  \param[in]     cryIfKeyId                 Holds the identifier of the key whose key element shall be the source
 *                                            element.
 *  \param[in]     keyElementId               Holds the identifier of the key element which shall be the source for the
 *                                            copy operation.
 *  \param[in]     keyElementSourceOffset     This is the offset of the source key element indicating the start index
 *                                            of the copy operation.
 *  \param[in]     keyElementTargetOffset     This is the offset of the destination key element indicating the start index
 *                                            of the copy operation.
 *  \param[in]     keyElementCopyLength       Specifies the number of bytes that shall be copied.
 *  \param[in]     targetCryIfKeyId           Holds the identifier of the key whose key element shall be the destination
 *                                            element.
 *  \param[in]     targetKeyElementId         Holds the identifier of the key element which shall be the destination for
 *                                            the copy operation.
 *  \return        E_NOT_OK                   As this should never be called directly, E_NOT_OK is always returned.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyElementCopyPartial_Stub(uint32 cryIfKeyId,
                                                                  uint32 keyElementId,
                                                                  uint32 keyElementSourceOffset,
                                                                  uint32 keyElementTargetOffset,
                                                                  uint32 keyElementCopyLength,
                                                                  uint32 targetCryIfKeyId,
                                                                  uint32 targetKeyElementId);

/**********************************************************************************************************************
 *  CryIf_CertificateParse_Stub()
 *********************************************************************************************************************/
/*! \brief         Returns E_NOT_OK.
 *  \details       This function is referenced in the crypto functions array if the underlying Crypto Driver does not
 *                 support CertificateParse Service. This function should never be called and is only existent to
 *                 overcome Compiler / Linker errors.
 *  \param[in]     cryIfKeyId                 Holds the identifier of the key slot in which the certificate has been
 *                                            stored.
 *  \return        E_NOT_OK                   As this should never be called directly, E_NOT_OK is always returned.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateParse_Stub(uint32 cryIfKeyId);

/**********************************************************************************************************************
 *  CryIf_CertificateVerify_Stub()
 *********************************************************************************************************************/
/*! \brief         Returns E_NOT_OK.
 *  \details       This function is referenced in the crypto functions array if the underlying Crypto Driver does not
 *                 support CertificateVerify Service. This function should never be called and is only existent to
 *                 overcome Compiler / Linker errors.
 *  \param[in]     cryIfKeyId              Holds the identifier of the key which shall be used to validate the
 *                                         certificate.
 *  \param[in]     verifyCryIfKeyId        Holds the identifier of the key containing the certificate, which shall be
 *                                         verified.
 *  \param[out]    verifyPtr               Holds a pointer to the memory location which will contain the result of the
 *                                         certificate verification.
 *  \return        E_NOT_OK                As this should never be called directly, E_NOT_OK is always returned.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_CertificateVerify_Stub(uint32 cryIfKeyId,
                                                              uint32 verifyCryIfKeyId,
                                                              P2VAR(Crypto_VerifyResultType, AUTOMATIC, CRYIF_APPL_VAR) verifyPtr);

/**********************************************************************************************************************
 *  CryIf_KeySetInvalid_Stub()
 *********************************************************************************************************************/
/*! \brief         Returns E_NOT_OK.
 *  \details       This function is referenced in the crypto functions array if the underlying Crypto Driver does not
 *                 support KeySetInvalid Service. This function should never be called and is only existent to overcome
 *                 Compiler / Linker errors.
 *  \param[in]     cryIfKeyId                 Holds the identifier of the key that shall be invalidated.
 *  \return        E_NOT_OK                   As this should never be called directly, E_NOT_OK is always returned.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \pre           -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeySetInvalid_Stub(uint32 cryIfKeyId);

/**********************************************************************************************************************
 *  CryIf_KeyGetStatus_Stub()
 *********************************************************************************************************************/
/*! \brief         Returns E_NOT_OK.
 *  \details       This function is referenced in the crypto functions array if the underlying Crypto Driver does not
 *                 support KeyGetStatus Service. This function should never be called and is only existent to overcome
 *                 Compiler / Linker errors.
 *  \param[in]     cryIfKeyId                 Holds the identifier of the key whose status shall be returned.
 *  \param[out]    keyStatusPtr               Holds a pointer to the memory where the key status shall be written to.
 *  \return        E_NOT_OK                   As this should never be called directly, E_NOT_OK is always returned.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CRYIF_CODE) CryIf_KeyGetStatus_Stub(uint32 cryIfKeyId,
                                                         P2VAR(Crypto_KeyStatusType, AUTOMATIC, CRYIF_APPL_VAR) keyStatusPtr);

# define CRYIF_STOP_SEC_CODE
# include "CryIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* !defined (CRYIF_PRIVATE_H) */

/**********************************************************************************************************************
 *  END OF FILE: CryIf_Private.h
 *********************************************************************************************************************/
