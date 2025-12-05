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
/*!      \file  Crypto_30_LibCv_AutosarDefines.h
 *       \brief  MICROSAR Crypto Driver (Crypto)
 *
 *       \details  Internal header file for fallback AUTOSAR definitions and necessary AUTOSAR definition checks.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_AUTOSARDEFINES_H)
# define CRYPTO_30_LIBCV_AUTOSARDEFINES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Csm_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Algorithm type definitions are normally provided by Csm, here only as fallback */
# ifndef CRYPTO_ALGOFAM_NOT_SET /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_NOT_SET                                      0x00u
# endif
# ifndef CRYPTO_ALGOFAM_SHA1 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHA1                                         0x01u
# endif
# ifndef CRYPTO_ALGOFAM_SHA2_224 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHA2_224                                     0x02u
# endif
# ifndef CRYPTO_ALGOFAM_SHA2_256 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHA2_256                                     0x03u
# endif
# ifndef CRYPTO_ALGOFAM_SHA2_384 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHA2_384                                     0x04u
# endif
# ifndef CRYPTO_ALGOFAM_SHA2_512 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHA2_512                                     0x05u
# endif
# ifndef CRYPTO_ALGOFAM_SHA2_512_224 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHA2_512_224                                 0x06u
# endif
# ifndef CRYPTO_ALGOFAM_SHA2_512_256 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHA2_512_256                                 0x07u
# endif
# ifndef CRYPTO_ALGOFAM_SHA3_224 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHA3_224                                     0x08u
# endif
# ifndef CRYPTO_ALGOFAM_SHA3_256 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHA3_256                                     0x09u
# endif
# ifndef CRYPTO_ALGOFAM_SHA3_384 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHA3_384                                     0x0Au
# endif
# ifndef CRYPTO_ALGOFAM_SHA3_512 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHA3_512                                     0x0Bu
# endif
# ifndef CRYPTO_ALGOFAM_SHAKE128 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHAKE128                                     0x0Cu
# endif
# ifndef CRYPTO_ALGOFAM_SHAKE256 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SHAKE256                                     0x0Du
# endif
# ifndef CRYPTO_ALGOFAM_RIPEMD160 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_RIPEMD160                                    0x0Eu
# endif
# ifndef CRYPTO_ALGOFAM_BLAKE_1_256 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_BLAKE_1_256                                  0x0Fu
# endif
# ifndef CRYPTO_ALGOFAM_BLAKE_1_512 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_BLAKE_1_512                                  0x10u
# endif
# ifndef CRYPTO_ALGOFAM_BLAKE_2s_256 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_BLAKE_2s_256                                 0x11u
# endif
# ifndef CRYPTO_ALGOFAM_BLAKE_2s_512 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_BLAKE_2s_512                                 0x12u
# endif
# ifndef CRYPTO_ALGOFAM_3DES /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_3DES                                         0x13u
# endif
# ifndef CRYPTO_ALGOFAM_AES /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_AES                                          0x14u
# endif
# ifndef CRYPTO_ALGOFAM_CHACHA /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_CHACHA                                       0x15u
# endif
# ifndef CRYPTO_ALGOFAM_RSA /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_RSA                                          0x16u
# endif
# ifndef CRYPTO_ALGOFAM_ED25519 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_ED25519                                      0x17u
# endif
# ifndef CRYPTO_ALGOFAM_BRAINPOOL /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_BRAINPOOL                                    0x18u
# endif
# ifndef CRYPTO_ALGOFAM_ECCNIST /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_ECCNIST                                      0x19u
# endif
# ifndef CRYPTO_ALGOFAM_SECURECOUNTER /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SECURECOUNTER                                0x1Au
# endif
# ifndef CRYPTO_ALGOFAM_RNG /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_RNG                                          0x1Bu
# endif
# ifndef CRYPTO_ALGOFAM_SIPHASH /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SIPHASH                                      0x1Cu
# endif
# ifndef CRYPTO_ALGOFAM_ECIES /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_ECIES                                        0x1Du
# endif
# ifndef CRYPTO_ALGOFAM_ECCANSI /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_ECCANSI                                      0x1Eu
# endif
# ifndef CRYPTO_ALGOFAM_ECCSEC /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_ECCSEC                                       0x1Fu
# endif
# ifndef CRYPTO_ALGOFAM_DRBG /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_DRBG                                         0x20u
# endif
# ifndef CRYPTO_ALGOFAM_FIPS186 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_FIPS186                                      0x21u
# endif
# ifndef CRYPTO_ALGOFAM_PADDING_PKCS7 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_PADDING_PKCS7                                0x22u
# endif
# ifndef CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_PADDING_ONEWITHZEROS                         0x23u
# endif
# ifndef CRYPTO_ALGOFAM_PBKDF2 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_PBKDF2                                       0x24u
# endif
# ifndef CRYPTO_ALGOFAM_KDFX963 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_KDFX963                                      0x25u
# endif
# ifndef CRYPTO_ALGOFAM_DH /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_DH                                           0x26u
# endif
# ifndef CRYPTO_ALGOFAM_SM2 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SM2                                          0x27u
# endif
# ifndef CRYPTO_ALGOFAM_EEA3 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_EEA3                                         0x28u
# endif
# ifndef CRYPTO_ALGOFAM_SM3 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_SM3                                          0x29u
# endif
# ifndef CRYPTO_ALGOFAM_EIA3 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_EIA3                                         0x2Au
# endif
# ifndef CRYPTO_ALGOFAM_HKDF /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_HKDF                                         0x2Bu
# endif
# ifndef CRYPTO_ALGOFAM_ECDSA /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_ECDSA                                        0x2Cu
# endif
# ifndef CRYPTO_ALGOFAM_POLY1305 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_POLY1305                                     0x2Du
# endif
# ifndef CRYPTO_ALGOFAM_X25519 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_X25519                                       0x2Eu
# endif
# ifndef CRYPTO_ALGOFAM_ECDH /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_ECDH                                         0x2Fu
# endif
# ifndef CRYPTO_ALGOFAM_CUSTOM /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOFAM_CUSTOM                                       0xFFu
# endif

# ifndef CRYPTO_ALGOMODE_NOT_SET /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_NOT_SET                                     0x00u
# endif
# ifndef CRYPTO_ALGOMODE_ECB /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_ECB                                         0x01u
# endif
# ifndef CRYPTO_ALGOMODE_CBC /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_CBC                                         0x02u
# endif
# ifndef CRYPTO_ALGOMODE_CFB /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_CFB                                         0x03u
# endif
# ifndef CRYPTO_ALGOMODE_OFB /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_OFB                                         0x04u
# endif
# ifndef CRYPTO_ALGOMODE_CTR /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_CTR                                         0x05u
# endif
# ifndef CRYPTO_ALGOMODE_XTS /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_XTS                                         0x06u
# endif
# ifndef CRYPTO_ALGOMODE_GCM /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_GCM                                         0x07u
# endif
# ifndef CRYPTO_ALGOMODE_RSAES_OAEP /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_RSAES_OAEP                                  0x08u
# endif
# ifndef CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_RSAES_PKCS1_v1_5                            0x09u
# endif
# ifndef CRYPTO_ALGOMODE_RSASSA_PSS /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_RSASSA_PSS                                  0x0Au
# endif
# ifndef CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_RSASSA_PKCS1_v1_5                           0x0Bu
# endif
# ifndef CRYPTO_ALGOMODE_8ROUNDS /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_8ROUNDS                                     0x0Cu
# endif
# ifndef CRYPTO_ALGOMODE_12ROUNDS /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_12ROUNDS                                    0x0Du
# endif
# ifndef CRYPTO_ALGOMODE_20ROUNDS /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_20ROUNDS                                    0x0Eu
# endif
# ifndef CRYPTO_ALGOMODE_HMAC /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_HMAC                                        0x0Fu
# endif
# ifndef CRYPTO_ALGOMODE_CMAC /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_CMAC                                        0x10u
# endif
# ifndef CRYPTO_ALGOMODE_GMAC /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_GMAC                                        0x11u
# endif
# ifndef CRYPTO_ALGOMODE_CTRDRBG /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_CTRDRBG                                     0x12u
# endif
# ifndef CRYPTO_ALGOMODE_SIPHASH_2_4 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_SIPHASH_2_4                                 0x13u
# endif
# ifndef CRYPTO_ALGOMODE_SIPHASH_4_8 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_SIPHASH_4_8                                 0x14u
# endif
# ifndef CRYPTO_ALGOMODE_PXXXR1 /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_PXXXR1                                      0x15u
# endif
# ifndef CRYPTO_ALGOMODE_CUSTOM /* COV_CRYPTO_30_LIBCV_USER_DEFINES */
#  define CRYPTO_ALGOMODE_CUSTOM                                      0xFFu
# endif

/* Check Autosar defines for array access */
/*!
 *
 * Internal comment removed.
 *
 *
 */
# if (CRYPTO_HASH != 0u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_MACGENERATE != 1u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_MACVERIFY != 2u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ENCRYPT != 3u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_DECRYPT != 4u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_AEADENCRYPT != 5u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_AEADDECRYPT != 6u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_SIGNATUREGENERATE != 7u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_SIGNATUREVERIFY != 8u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_RANDOMGENERATE != 11u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_RANDOMSEED != 12u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_KEYGENERATE != 13u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_KEYDERIVE != 14u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_KEYEXCHANGECALCPUBVAL != 15u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_KEYEXCHANGECALCSECRET != 16u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_KEYSETVALID != 19u)
#  error "Define is not Autosar conform."
# endif

/*!
 *
 * Internal comment removed.
 *
 *
 */
# if (CRYPTO_ALGOFAM_NOT_SET != 0u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHA1 != 1u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHA2_224 != 2u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHA2_256 != 3u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHA2_384 != 4u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHA2_512 != 5u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHA2_512_224 != 6u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHA2_512_256 != 7u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHA3_224 != 8u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHA3_256 != 9u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHA3_384 != 10u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHA3_512 != 11u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHAKE128 != 12u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_SHAKE256 != 13u)
#  error "Define is not Autosar conform."
# endif
# if (CRYPTO_ALGOFAM_RIPEMD160 != 14u)
#  error "Define is not Autosar conform."
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif /* CRYPTO_30_LIBCV_AUTOSARDEFINES_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_AutosarDefines.h
 *********************************************************************************************************************/
