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
/*!        \file  TcpIp_Tls12Client.h
 *        \brief  Transport Layer Security (TLS) 1.2 client functions implementation.
 *
 *      \details  This file is part of the TcpIp TLS submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#if !defined (TCPIP_TLS12CLIENT_H)
# define TCPIP_TLS12CLIENT_H

# include "TcpIp_Cfg.h"

# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
#  if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
#   if (TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#    include "TcpIp_Lcfg.h"
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
 *   TLS CLIENT HANDSHAKE STATE MACHINE
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientHelloExtAddSupportedPointsFormat
 *********************************************************************************************************************/
/*! \brief         Add supported EC point format extension to the client hello message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientHelloExtAddSupportedPointsFormat(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_PerformNextHsStep
 *********************************************************************************************************************/
/*! \brief         Perform the next HS processing step
 *  \details       Main TLS 1.2 client handshake state machine handling after Server Hello was parsed
 *                 in TcpIp_TlsClientHsStm_PerformNextHsStep() and the agreed TLS version is 1.2
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_PerformNextHsStep(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCtx_AsyncHandler
 *********************************************************************************************************************/
/*! \brief         Handle async context operations for Tls 1.2 client
 *  \details       Calculates all time consuming operation if the corresponding flags are set
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK (only called in the context of the TLS background task TcpIp_Tls_MainFunctionLowPrio())
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_AsyncHandler(
  TcpIp_TlsConnectionIterType TlsConIdx);

#    if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 * TcpIp_Tls12ClientCtx_ValidateCertificateStatus
 *********************************************************************************************************************/
/*! \brief          Validate the received OCSP certificate status request
 *  \details        Triggered from TLS handshake state machine during parsing of ServerCertificateStatus request
 *  \param[in]      TlsConIdx               TLS connection index
 *                                          \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK (only called in the context of the TLS background task TcpIp_Tls_MainFunctionLowPrio())
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_ValidateCertificateStatus(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCtx_VerifySignatureOfPublicKey
 *********************************************************************************************************************/
/*! \brief         Verify signature of the server key exchange public key received
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK (only called in the context of the TLS background task TcpIp_Tls_MainFunctionLowPrio())
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_VerifySignatureOfPublicKey(
  TcpIp_TlsConnectionIterType TlsConIdx);
#    endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCtx_CalcClientFinishedHash
 *********************************************************************************************************************/
/*! \brief         Calculate the client finished hash
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \pre           -
 *  \context       TASK (only called in the context of the TLS background task TcpIp_Tls_MainFunctionLowPrio())
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_CalcClientFinishedHash(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCrypto_SetCsmKeys
 *********************************************************************************************************************/
/*! \brief         Set the encryption and hash generation Csm keys for the selected ciphersuite
 *  \details       The keys are derived from the calculated key block
 *  \param[in]     TlsConIdx            TLS connection index
 *                                      \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK                 Operation successful
 *  \return        E_NOT_OK             Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_SetCsmKeys(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCrypto_GenerateHsHash
 *********************************************************************************************************************/
/*! \brief         Generate the hash of all handshake messages
 *  \details       The hash generated is used in the HS finished message
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     FinishedMessageType       Type of finished message (Server / client)
 *  \return        E_OK                    Operation successful
 *  \return        E_NOT_OK                Operation NOT successful
 *  \pre           -
 *  \context       TASK (only called in the context of the TLS background task TcpIp_Tls_MainFunctionLowPrio())
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_GenerateHsHash(
 TcpIp_TlsConnectionIterType      TlsConIdx,
  TcpIp_TlsMessageFinishedType    FinishedMessageType);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCrypto_CalcHsFinishedData
 *********************************************************************************************************************/
/*! \brief         Calculated the Hs finished data for client/server
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     FinishedMessageType     Type of finished message (Server / client)
 *  \param[out]    ResultPtr               Buffer to store the calculated hash
 *  \return        E_OK                    Operation successful
 *  \return        E_NOT_OK                Operation NOT successful
 *  \pre           -
 *  \context       TASK (only called in the context of the TLS background task TcpIp_Tls_MainFunctionLowPrio())
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_CalcHsFinishedData(
  TcpIp_TlsConnectionIterType    TlsConIdx,
  TcpIp_TlsMessageFinishedType   FinishedMessageType,
  TCPIP_P2V(uint8)               ResultPtr);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCtx_CalcServerFinishedHash
 *********************************************************************************************************************/
/*! \brief         Calculate the server finished hash
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK (only called in the context of the TLS background task TcpIp_Tls_MainFunctionLowPrio())
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_CalcServerFinishedHash(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCtx_CalcServerFinishedVerifyData
 *********************************************************************************************************************/
/*! \brief         Calculate the server finished verify data at Tls client side
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK (only called in the context of the TLS background task TcpIp_Tls_MainFunctionLowPrio())
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_CalcServerFinishedVerifyData(
  TcpIp_TlsConnectionIterType TlsConIdx);
#   endif /* (TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON) */
#  endif /* (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON) */
# endif /* (TCPIP_SUPPORT_ASRTLS == STD_ON) */
#endif /* TCPIP_TLS12CLIENT_H */

/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Tls12Client.h
 *********************************************************************************************************************/

