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
/*      \file  Crypto_30_LibCv_Curve.h
 *      \brief  MICROSAR Crypto Driver (Crypto)
 *
 *      \details  Internal header file for curve parameter prototypes
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (CRYPTO_30_LIBCV_CURVE_H)
# define CRYPTO_30_LIBCV_CURVE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Crypto_30_LibCv_Services.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define CRYPTO_30_LIBCV_START_SEC_CONST_8BIT
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Empty curve vector START *********************************************************************************************/
/* This vector is used to initialize pointers which shall point to the EC domain, the EC domain extension and
 * the EC SpeedUp extension. If one of the regular curve parameters were used for this purpose, the corresponding
 * arrays will always be used and therefore never be removed by the linker.
 * Since this array does not start with the ASN1 sequence tag '0x30', the parser of the underlying crypto
 * library will recognize these domains as invalid. */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveEmptyDomain[2];
/* Empty curve vector END ***********************************************************************************************/

/* SEC_p160r1 START *****************************************************************************************************/
/* SECp160r1 */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSecP160R1Domain[155];
# if (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSecP160R1DomainExt[147];
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSecP160R1DomainExt[149];
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSecP160R1DomainExt[153];
# endif
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSecP160R1SpeedUpExt[355];
/* SEC_p160r1 END *********************************************************************************************/

/* NIST_SEC_p224r1 START *********************************************************************************************/
/* NISTp224r1 = SECp224r1 */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP224R1Domain[203];
# if (actBN_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP224R1DomainExt[195];
# elif (actBN_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP224R1DomainExt[197];
# elif (actBN_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP224R1DomainExt[201];
# endif
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP224R1SpeedUpExt[483];
/* NIST_SEC_p224r1 END *********************************************************************************************/

/* NIST_ANSI_SEC_p256r1 START *********************************************************************************************/
/* NISTp256r1 = ANSIp256r1 = SECp256r1 */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1Domain[227];
# if (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt[219];
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt[221];
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1DomainExt[225];
# endif
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistAnsiSecP256R1SpeedUpExt[547];
/* NIST_ANSI_SEC_p256r1 END *********************************************************************************************/

/* NIST_SEC_p384r1 START *********************************************************************************************/
/* NISTp384r1 = SECp384r1 */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP384R1Domain[324];
# if (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt[316];
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt[318];
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP384R1DomainExt[323];
# endif
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP384R1SpeedUpExt[803];
/* NIST_SEC_p384r1 END *********************************************************************************************/

/* NIST_SEC_p521r1 START *********************************************************************************************/
/* NISTp521r1 = SECp521r1 */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP521R1Domain[432];
# if (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP521R1DomainExt[427];
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP521R1DomainExt[429];
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP521R1DomainExt[433];
# endif
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveNistSecP521R1SpeedUpExt[1099];
/* NIST_SEC_p521r1 END *********************************************************************************************/

/* Edwards448 START **************************************************************************************************/
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveEdwards448GroupOrder[56];
/* Edwards448 END ****************************************************************************************************/

/* Sm2p256v1 START *********************************************************************************************/
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSm2P256v1Domain[227];
# if (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 1)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSm2P256v1DomainExt[219];
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 2)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSm2P256v1DomainExt[221];
# elif (CRYPTO_30_LIBCV_BYTES_PER_DIGIT == 4)  /* COV_CRYPTO_30_LIBCV_BYTES_PER_DIGIT_DEFINE */
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSm2P256v1DomainExt[225];
# endif
extern CONST(uint8, CRYPTO_30_LIBCV_CONST) Crypto_30_LibCv_EccCurveSm2P256v1SpeedUpExt[547];
/* Sm2p256v1 END *********************************************************************************************/

# define CRYPTO_30_LIBCV_STOP_SEC_CONST_8BIT
# include "Crypto_30_LibCv_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif /* CRYPTO_30_LIBCV_CURVE_H */
/**********************************************************************************************************************
 *  END OF FILE: Crypto_30_LibCv_Curve.h
 *********************************************************************************************************************/
