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
/*!        \file  TcpIp_Tls12Client.c
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

#define TCPIP_TLS12CLIENT_SOURCE
#include "TcpIp_Cfg.h"

#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
# if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
#  if (TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#   include "TcpIp_Lcfg.h"
#   include "TcpIp_Tls.h"
#   include "TcpIp_Tls12Core.h"
#   include "TcpIp_Tls12Client.h"
#   include "TcpIp_TlsPriv.h"
#   include "TcpIp_TlsCoreCommon.h"
#   include "TcpIp_TlsClientCommon.h"

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
#    include "KeyM.h"
#   endif

#   include "TcpIp_Tls_Cbk.h"

#   if (TCPIP_DEV_ERROR_REPORT == STD_ON)
#    include "Det.h"
#   endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#   if !defined (TCPIP_TLS_LOCAL)
#    define TCPIP_TLS_LOCAL static
#   endif

#   if !defined (TCPIP_TLS_LOCAL_INLINE)
#    define TCPIP_TLS_LOCAL_INLINE LOCAL_INLINE
#   endif

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
#   define TCPIP_START_SEC_CODE
#   include "TcpIp_MemMap.h"                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertMsgParseAndTriggerVerification
 *********************************************************************************************************************/
/*! \brief         Extract certificates from server certificate message and trigger verification if possible
 *  \details       -
 *  \param[in]     TlsConIdx                       TLS connection index
 *                                                 \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     CertStartIdx                    Start Idx of the certificate data
 *  \param[in]     NumCerts                        Number of certificates in cert chain
 *  \return        E_OK                            Operation successful
 *  \return        E_NOT_OK                        Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertMsgParseAndTriggerVerification(
  TcpIp_TlsConnectionIterType         TlsConIdx,
  TcpIp_TlsBufferRxIterType           CertStartIdx,
  uint8                               NumCerts);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertMsgCheckCertsForMatchingHandshake
 *********************************************************************************************************************/
/*! \brief         Find all cipherworkers which have a matching ciphersuite to the one received in the ServerHello
 *                 and iterate through their handshakes and check if one fits the received certificates
 *  \details       -
 *  \param[in]     TlsConIdx              TLS connection index
 *                                        \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK                   Fitting cipherworker with handshake was found and set to active
 *                 E_NOT_OK               No fitting handshake could be found
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertMsgCheckCertsForMatchingHandshake(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_ParseServerCertificate
 *********************************************************************************************************************/
/*! \brief         Parse the TLS 1.2 server certificate message
 *  \details       -
 *  \param[in]     TlsConIdx     TLS connection index
 *                               \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerCertificate(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertificateDetermineNextHsState
 *********************************************************************************************************************/
/*! \brief         Determine the next valid TLS handshake state after the certificate message
 *  \details       This function can be called multiple times, when we do not received the next message and can not
 *                 determine the valid next state
 *  \param[in]     TlsConIdx     TLS connection index
 *                               \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[out]    NextStatePtr  Next state of the client HS state machine
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertificateDetermineNextHsState(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ExtractServerCertificateInformation
 *********************************************************************************************************************/
/*! \brief         Extract the server certificate information from the received RL frame when using TLS 1.2
 *  \details       -
 *  \param[in]     TlsConIdx                        TLS connection index
 *                                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[out]    NextStatePtr                     Next state of the client HS state machine
 *  \return        E_OK                             Operation successful
 *  \return        E_NOT_OK                         Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractServerCertificateInformation(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertMsgProcessCertChain
 *********************************************************************************************************************/
/*! \brief         Process the certificate chain in the received server cert message when using TLS 1.2
 *  \details       -
 *  \param[in]     TlsConIdx                         TLS connection index
 *                                                   \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK                              Operation successful
 *  \return        E_NOT_OK                          Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertMsgProcessCertChain(
  TcpIp_TlsConnectionIterType TlsConIdx);


/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_ParseServerCertificateStatus
 *********************************************************************************************************************/
/*! \brief         Parse the server certificate status message
 *  \details       -
 *  \param[in]     TlsConIdx     TLS connection index
 *                               \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerCertificateStatus(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ExtractServerCertificateStatusInformation
 *********************************************************************************************************************/
/*! \brief         Extract the server certificate information from the received RL frame
 *  \details       -
 *  \param[in]     TlsConIdx                        TLS connection index
 *                                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[out]    NextStatePtr                     Next state of the client HS state machine
 *  \return        E_OK                             Operation successful
 *  \return        E_NOT_OK                         Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractServerCertificateStatusInformation(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertStatusGetCertStatus
 *********************************************************************************************************************/
/*! \brief         Extract the received Server certificate status information from the Tls RxBuffer
 *  \details       -
 *  \param[in]     TlsConIdx                        TLS connection index
 *                                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK                             Operation successful
 *  \return        E_NOT_OK                         Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertStatusGetCertStatus(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertStatusTriggerOcspValidation
 *********************************************************************************************************************/
/*! \brief         Triggers the validation of the received OCSP status response
 *  \details       The actual validation is done in the low prio task of the TLS
 *  \param[in]     TlsConIdx                        TLS connection index
 *                                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertStatusTriggerOcspValidation(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertStatusUpdateCertStatus
 *********************************************************************************************************************/
/*! \brief         Update the certificate status with the result from the OCSP validation
 *  \details       -
 *  \param[in]     TlsConIdx                        TLS connection index
 *                                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK                             Operation successful
 *  \return        E_NOT_OK                         Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertStatusUpdateCertStatus(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertStatusGetLeafOcspResult
 *********************************************************************************************************************/
/*! \brief         Read the OCSP status of the leaf certificate
 *  \details       -
 *  \param[in]     TlsConIdx                        TLS connection index
 *                                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[out]    LeafCertStatusResponsePtr        OCSP Status Response of the Leaf certificate
 *                                                  CONSTRAINT: not NULL_PTR
 *  \return        E_OK                             Leaf certificate status successfully read and stored in LeafCertStatusResponsePtr
 *  \return        E_NOT_OK                         Status of Leaf certificate could not be determined
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertStatusGetLeafOcspResult(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_CP2V(TcpIp_CertificateStatusResponseType) LeafCertStatusResponsePtr);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertStatusDetermineNextHsState
 *********************************************************************************************************************/
/*! \brief         Determine the next valid TLS handshake state, depending on the selected key exchange method
 *  \details       -
 *  \param[in]     TlsConIdx                        TLS connection index
 *                                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[out]    NextStatePtr                     Next state of the client HS state machine
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertStatusDetermineNextHsState(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr);
#   endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_ParseServerKeyExchange
 *********************************************************************************************************************/
/*! \brief         Parse the received server key exchange message
 *  \details       -
 *  \param[in]     TlsConIdx        TLS connection index
 *                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerKeyExchange(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ExtractServerKeyExchangeInformation
 *********************************************************************************************************************/
/*! \brief         Extract the server key exchange information from the received message
 *  \details       -
 *  \param[in]     TlsConIdx                         TLS connection index
 *                                                   \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[out]    NextStatePtr                      Next state of the client HS state machine
 *  \return        E_OK                              Operation successful
 *  \return        E_NOT_OK                          Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractServerKeyExchangeInformation(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr);

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerKeyExchangeVerifySignature
 *********************************************************************************************************************/
/*! \brief         Verify the server certificate signature
 *  \details       Use the generated key to verify the leaf certificate signature
 *  \param[in]     TlsConIdx                         TLS connection index
 *                                                   \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeVerifySignature(
  TcpIp_TlsConnectionIterType TlsConIdx);
#   endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerKeyExchangeDetermineNextHsState
 *********************************************************************************************************************/
/*! \brief         Verify the server certificate signature
 *  \details       Use the generated key to verify the leaf certificate signature
 *  \param[in]     TlsConIdx                         TLS connection index
 *                                                   CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \param[out]    NextStatePtr                      Next state of the client HS state machine
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeDetermineNextHsState(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParams
 *********************************************************************************************************************/
/*! \brief         Get server parameters from the server key exchange message
 *  \details       -
 *  \param[in]     TlsConIdx           TLS connection index
 *                                     \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[out]    NextStatePtr        Next state of the client HS state machine
 *  \return        E_OK                Operation successful
 *  \return        E_NOT_OK            Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParams(
  TcpIp_TlsConnectionIterType        TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsPsk
 *********************************************************************************************************************/
/*! \brief         Get server parameters from the server key exchange message for PSK identity
 *  \details       -
 *  \param[in]     TlsConIdx        TLS connection index
 *                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK             Operation successful
 *  \return        E_NOT_OK         Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 ********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsPsk(
  TcpIp_TlsConnectionIterType TlsConIdx);

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsEcc
 *********************************************************************************************************************/
/*! \brief         Get server parameters from the server key exchange message for ECC identity
 *  \details       -
 *  \param[in]     TlsConIdx       TLS connection index
 *                                 \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK            Operation successful
 *  \return        E_NOT_OK        Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 ********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsEcc(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveType
 *********************************************************************************************************************/
/*! \brief         Get the elliptic curve type from the server exchange message
 *  \details       -
 *  \param[in]     TlsConIdx        TLS connection index
 *                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return         E_OK            Operation successful
 *  \return         E_NOT_OK        Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveType(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveId
 *********************************************************************************************************************/
/*! \brief         Obtain the elliptic curve Id of the server exchange message
 *  \details       -
 *  \param[in]     TlsConIdx       TLS connection index
 *                                 \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK            Operation successful
 *  \return        E_NOT_OK        Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveId(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetPubKey
 *********************************************************************************************************************/
/*! \brief         Obtain the public key from the received server key exchange
 *  \details       -
 *  \param[in]     TlsConIdx                  TLS connection index
 *                                            \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     ServerKeyExchangeEndIdx    End Idx of the server key exchange message
 *  \return        E_OK                       Operation successful
 *  \return        E_NOT_OK                   Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetPubKey(
  TcpIp_TlsConnectionIterType  TlsConIdx,
  TcpIp_TlsBufferRxIterType    ServerKeyExchangeEndIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgCompareReceivedAndExpectedPubKey
 *********************************************************************************************************************/
/*! \brief         Read the received public key and compare it to the expectation
 *  \details       -
 *  \param[in]     TlsConIdx                  TLS connection index
 *                                            CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \param[in]     ServerKeyExchangeEndIdx    End Idx of the server key exchange message
 *  \param[in]     expectedPubKeyLen          Expected length of the public key
 *  \return        E_OK                       Operation successful
 *  \return        E_NOT_OK                   Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgCompareReceivedAndExpectedPubKey(
  TcpIp_TlsConnectionIterType  TlsConIdx,
  TcpIp_TlsBufferRxIterType    ServerKeyExchangeEndIdx,
  uint8                        expectedPubKeyLen);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignAlgorithm
 *********************************************************************************************************************/
/*! \brief         Obtain signature algorithm from the received server key exchange message
 *  \details       -
 *  \param[in]     TlsConIdx       TLS connection index
 *                                 \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK            Operation successful
 *  \return        E_NOT_OK        Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignAlgorithm(
  TcpIp_TlsConnectionIterType  TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignature
 *********************************************************************************************************************/
/*! \brief         Extract the signature from the received server key exchange message
 *  \details       -
 *  \param[in]     TlsConIdx                  TLS connection index
 *                                            \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     ServerKeyExchangeEndIdx    End Idx of the server key exchange message
 *  \return        E_OK                       Operation successful
 *  \return        E_NOT_OK                   Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignature(
  TcpIp_TlsConnectionIterType  TlsConIdx,
  TcpIp_TlsBufferRxIterType    ServerKeyExchangeEndIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_ParseServerCertificateRequest
 *********************************************************************************************************************/
/*! \brief         Parse the server hello done message
 *  \details       -
 *  \param[in]     TlsConIdx         TLS connection index
 *                                   CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerCertificateRequest(
  TcpIp_TlsConnectionIterType  TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ExtractCertificateRequestInformation
 *********************************************************************************************************************/
/*! \brief         Extract the server certificate request information from the message
 *  \details       -
 *  \param[in]     TlsConIdx       TLS connection index
 *                                 CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \return        E_OK            Operation successful
 *  \return        E_NOT_OK        Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractCertificateRequestInformation(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetCertTypes
 *********************************************************************************************************************/
/*! \brief         Extract the certificate types information from the server certificate request message
 *  \details       This function does not provide any functionality since the TLS client does not support client auth.
 *  \param[in]     TlsConIdx       TLS connection index
 *                                 CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \return        E_OK            Operation successful
 *  \return        E_NOT_OK        Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetCertTypes(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetSignatureHashAlgorithms
 *********************************************************************************************************************/
/*! \brief         Extract the signature hash algorithms from the server certificate request message
 *  \details       This function does not provide any functionality since the TLS client does not support client auth.
 *  \param[in]     TlsConIdx       TLS connection index
 *                                 CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \return        E_OK            Operation successful
 *  \return        E_NOT_OK        Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetSignatureHashAlgorithms(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetDistinguishedNames
 *********************************************************************************************************************/
/*! \brief         Extract the distinguished names from the server certificate request message
 *  \details       -
 *  \param[in]     TlsConIdx       TLS connection index
 *                                 CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \return        E_OK            Operation successful
 *  \return        E_NOT_OK        Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetDistinguishedNames(
  TcpIp_TlsConnectionIterType TlsConIdx);
#   endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_ParseServerHelloDone
 *********************************************************************************************************************/
/*! \brief         Parse the server hello done message
 *  \details       -
 *  \param[in]     TlsConIdx         TLS connection index
 *                                   \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerHelloDone(
  TcpIp_TlsConnectionIterType  TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ExtractServerHelloDoneInformation
 *********************************************************************************************************************/
/*! \brief         Extract the server hello done information from the message
 *  \details       -
 *  \param[in]     TlsConIdx       TLS connection index
 *                                 \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK            Operation successful
 *  \return        E_NOT_OK        Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractServerHelloDoneInformation(
  TcpIp_TlsConnectionIterType  TlsConIdx);

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_SendClientCertificate
 *********************************************************************************************************************/
/*! \brief         Trigger the transmission of client certificate message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_SendClientCertificate(
  TcpIp_TlsConnectionIterType  TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_SendCertificateVerify
 *********************************************************************************************************************/
/*! \brief         Trigger the transmission of certificate verify message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_SendCertificateVerify(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_PrepareCertificateVerify
 *********************************************************************************************************************/
/*! \brief         Prepare the client certificate verify message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_PrepareCertificateVerify(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCrypto_CalcCertVerifySignature
 *********************************************************************************************************************/
/*! \brief         Calculate the signature for the CertificateVerify message.
 *  \details       Add the signature of all handshake messages exchanged so far.
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCrypto_CalcCertVerifySignature(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_FillClientCertificateMessage
 *********************************************************************************************************************/
/*! \brief         Fill the client certificate message, depending on the cert availability.
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \param[in]     CertAvailable           Client cert is available
 *  \return        E_OK                    Operation successful
 *  \return        E_NOT_OK                Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_FillClientCertificateMessage(
  TcpIp_TlsConnectionIterType TlsConIdx,
  boolean CertAvailable);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_PrepareClientCertificate
 *********************************************************************************************************************/
/*! \brief         Prepare the data structure of the client certificate message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \param[in]     CertAvailable           Client cert is available
 *  \return        E_OK                    Operation successful
 *  \return        E_NOT_OK                Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_PrepareClientCertificate(
  TcpIp_TlsConnectionIterType TlsConIdx,
  boolean CertAvailable);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientCertificateAddNoCertificate
 *********************************************************************************************************************/
/*! \brief         There are no client certificates available, so prepare an empty client certificate message.
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientCertificateAddNoCertificate(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientCertificateAddCertificates
 *********************************************************************************************************************/
/*! \brief         Add the configured certificates to the client certificate message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \return        E_OK                    Operation successful
 *  \return        E_NOT_OK                Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientCertificateAddCertificates(
  TcpIp_TlsConnectionIterType TlsConIdx);
#   endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_SendClientKeyExchange
 *********************************************************************************************************************/
/*! \brief         Trigger the transmission of client key exchange message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_SendClientKeyExchange(
  TcpIp_TlsConnectionIterType  TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_PrepareClientKeyExchangeAndTriggerSecretCalculation
 *********************************************************************************************************************/
/*! \brief         Prepare the client key exchange message
 *  \details       -
 *  \param[in]     TlsConIdx                         TLS connection index
 *                                                   \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_PrepareClientKeyExchangeAndTriggerSecretCalculation(
  TcpIp_TlsConnectionIterType  TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientKeyExchangeAddPskParams
 *********************************************************************************************************************/
/*! \brief         Add PSK parameters into the client key exchange message
 *  \details       -
 *  \param[in]     TlsConIdx                         TLS connection index
 *                                                   \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeAddPskParams(
    TcpIp_TlsConnectionIterType  TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientKeyExchangeSelectPskIdentity
 *********************************************************************************************************************/
/*! \brief         Select the PSK identity for the client key exchange message
 *  \details       If no matching PSK identity found, use the default PSK identity
 *  \param[in]     TlsConIdx                         TLS connection index
 *                                                   \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeSelectPskIdentity(
    TcpIp_TlsConnectionIterType  TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientKeyExchangeCheckPskHints
 *********************************************************************************************************************/
/*! \brief         Check if received PSK hint is already known to the Tls client
 *  \details       -
 *  \param[in]     TlsConIdx       TLS connection index
 *                                 \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        TRUE            received PSK hint is already known
 *  \return        FALSE           received PSK hint is not known
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeCheckPskHints(
  TcpIp_TlsConnectionIterType  TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientKeyExchangeCompareReceivedPskHint
 *********************************************************************************************************************/
/*! \brief         Compare the received PSK hint with know PSK identities to the Tls client
 *  \details       -
 *  \param[in]     TlsConIdx       TLS connection index
 *                                 \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        TRUE            received PSK hint is already known
 *  \return        FALSE           received PSK hint is not known
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeCompareReceivedPskHint(
  TcpIp_TlsConnectionIterType  TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientKeyExchangAddSelectedPskIdentity
 **********************************************************************************************************************/
/*! \brief         Add selected PSK identity to the client exchange message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangAddSelectedPskIdentity(
  TcpIp_TlsConnectionIterType  TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientKeyExchangeAddMessageLength
 **********************************************************************************************************************/
/*! \brief         Trigger transmission of client key exchange message
 *  \details       -
 *  \param[in]     TlsConIdx        TLS connection index
 *                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[out]    NextStatePtr     Next state of the client HS state machine
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
  TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeAddMessageLength(
  TcpIp_TlsConnectionIterType         TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState)  NextStatePtr);

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcSharedSecret
 *********************************************************************************************************************/
/*! \brief         Calculate the shared secret for the client key exchange
 *  \details       -
 *  \param[in]     TlsConIdx                         TLS connection index
 *                                                   \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcSharedSecret(
  TcpIp_TlsConnectionIterType TlsConIdx);
#   endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcPskPremasterSecret
 *********************************************************************************************************************/
/*! \brief         Calculate the PSK pre master secret for client key exchange
 *  \details       -
 *  \param[in]     TlsConIdx                        TLS connection index
 *                                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcPskPremasterSecret(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcMasterSecret
 *********************************************************************************************************************/
/*! \brief         Calculate the master secret for the client key exchange
 *  \details       -
 *  \param[in]     TlsConIdx                        TLS connection index
 *                                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcMasterSecret(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCrypto_SetCsmKeysAes128CbcStreaming
 *********************************************************************************************************************/
/*! \brief         Set the crypto keys for Aes128cbc streaming
 *  \details       -
 *  \param[in]     TlsConIdx           TLS connection index
 *                                     \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_SetCsmKeysAes128CbcStreaming(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCrypto_SetCsmKeysBlock128Streaming
 *********************************************************************************************************************/
/*! \brief         Set the crypto keys for Block128 streaming
 *  \details       -
 *  \param[in]     TlsConIdx         TLS connection index
 *                                   \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK              Operation successful
 *  \return        E_NOT_OK          Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_SetCsmKeysBlock128Streaming(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientCrypto_SetCsmKeysNullStreaming
 *********************************************************************************************************************/
/*! \brief         Set the crypto keys for NULL streaming
 *  \details       -
 *  \param[in]     TlsConIdx        TLS connection index
 *                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK             Operation successful
 *  \return        E_NOT_OK         Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_SetCsmKeysNullStreaming(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_SendClientCcs
 *********************************************************************************************************************/
/*! \brief         Trigger transmission of CCs message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_SendClientCcs(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_SendClientFinished
 *********************************************************************************************************************/
/*! \brief         Send client finished message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_SendClientFinished(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_PrepareClientFinished
 *********************************************************************************************************************/
/*! \brief         Prepare the client finished message
 *  \details       -
 *  \param[in]     TlsConIdx                        TLS connection index
 *                                                  \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_PrepareClientFinished(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ClientFinishedAddClientFinishedData
 *********************************************************************************************************************/
/*! \brief         Add client finished data to the client finished message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientFinishedAddClientFinishedData(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_ExtractServerFinished
 *********************************************************************************************************************/
/*! \brief         Parse the received server finished message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK                    Operation successful
 *  \return        E_NOT_OK                Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ExtractServerFinished(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_ExtractServerFinishedInformation
 *********************************************************************************************************************/
/*! \brief         Extract the server finished information from the server finished message
 *  \details       -
 *  \param[in]     TlsConIdx                         TLS connection index
 *                                                   \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK                              Operation successful
 *  \return        E_NOT_OK                          Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractServerFinishedInformation(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_CalcServerFinishedHash
 *********************************************************************************************************************/
/*! \brief         Calculate the server finished hash locally
 *  \details       Trigger async calculation of server finished hash
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_CalcServerFinishedHash(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_CalcServerFinishedVerifyData
 *********************************************************************************************************************/
/*! \brief         Calculate the server finished verify data
 *  \details       Trigger calculation of the server finished verify data
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_CalcServerFinishedVerifyData(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientMsg_CompareFinishedVerifyData
 *********************************************************************************************************************/
/*! \brief         Compare the server finished verify data with locally calculated value
 *  \details       -
 *  \param[in]     TlsConIdx                         TLS connection index
 *                                                   \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK                              Operation successful
 *  \return        E_NOT_OK                          Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 ********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_CompareFinishedVerifyData(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_ParseServerCcs
 *********************************************************************************************************************/
/*! \brief         Parse the received server CCs message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerCcs(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_ParseServerFinished
 *********************************************************************************************************************/
/*! \brief         Verify the server finished message
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerFinished(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12ClientHsStm_FinalizeHandshake
 *********************************************************************************************************************/
/*! \brief         Finalize the Tls handshake
 *  \details       -
 *  \param[in]     TlsConIdx               TLS connection index
 *                                         \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_FinalizeHandshake(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientHelloExtAddSupportedPointsFormat
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientHelloExtAddSupportedPointsFormat(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType    tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  /* Extension index information */

  /* Supported Point Formats Extension Format
                                  +----> Fixed length: (1)
                                 /                          +--->  Fixed value: uncompressed (0)
                                /                          /
   *       [ext Type][ext Len][EC Point Format Len][EC Point Format]
   * Byte: 0       1 2      3                    4                 5
   *                           \-------consumedExtensionLen--------/
   */
  TcpIp_TlsBufferTxIterType extTypeTlsBufferTxIdx               = TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  TcpIp_TlsBufferTxIterType extLenTlsBufferTxIdx                = extTypeTlsBufferTxIdx + TCPIP_TLS_EXTENSION_TYPE_LEN;
  TcpIp_TlsBufferTxIterType extEcPointFormatLenIdx   = extLenTlsBufferTxIdx + TCPIP_TLS_EXTENSION_LENGTH_LEN;

  uint16 consumedExtensionLen = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* Only add this specific extension if one of the referenced cipher worker is configured as ECC cipher worker */
  if (TcpIp_TlsCoreUtil_ConnectionUseEccCipherSuite(TlsConIdx) == TRUE)
  {
    /* Combine both fix parameter to uiint16 value */
    /* 0x0100 = [EC Point Format Len (0x01)][EC Point Format (0x00)] */
    uint16 fixedEcPointsFormatExtensionValue =
      ((uint16)TCPIP_TLS_EXTENSION_EC_POINT_FORMAT_FIXED_LENGTH << 8) | TCPIP_TLS_EXTENSION_EC_POINT_FORMAT_UNCOMPRESSED;

    /* #10 Add the extension value to the TxBuffer if TLS connection uses at least one ECC cipher suite */
    TcpIp_TlsMsg_AddUint16ValueToDistinctListInTxBuffer(TlsConIdx, fixedEcPointsFormatExtensionValue,
      extEcPointFormatLenIdx, &consumedExtensionLen);

    /* #20 Set the extension type to TCPIP_TLS_EXTENSION_EC_POINT_FORMAT (0x000b) */
    IpBase_PutUint16(TcpIp_GetAddrTlsBufferTx(extTypeTlsBufferTxIdx), 0, TCPIP_TLS_EXTENSION_EC_POINT_FORMAT);
    TcpIp_AddTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_EXTENSION_TYPE_LEN);

    /* #30 Set overall extension length */
    IpBase_PutUint16(TcpIp_GetAddrTlsBufferTx(extLenTlsBufferTxIdx), 0, consumedExtensionLen);
    TcpIp_AddTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_EXTENSION_LENGTH_LEN);
  }

} /* TcpIp_Tls12ClientMsg_ClientHelloExtAddSupportedPointsFormat() */

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_ParseServerCertificate
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerCertificate(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsBufferRxServerCertificatesIdxOfTlsClientBufferMgmtType serverCertificateIdx =
    TcpIp_GetTlsBufferRxServerCertificatesIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if we received a message with the TLS-Handshake type ServerCertificate */
  if (serverCertificateIdx != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATESIDXOFTLSCLIENTBUFFERMGMT)
  {
    TcpIp_TlsHandshakeState nextState = TCPIP_TLS_STATE_HANDSHAKE_INIT;
    Std_ReturnType retVal;

    /* #20 if so, extract the information inside the server certificate message */
    retVal = TcpIp_Tls12ClientMsg_ExtractServerCertificateInformation(TlsConIdx, &nextState);

    /* #30 Check return value from extract function and switch to next state */
    if (retVal == E_OK)
    {
      if (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CTX_ASYNC_PENDING)
      {
        /* Async operation pending, nothing to do - just leave the function */
      }
      else if (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CTX_WAIT_CYCLE)
      {
        /* Next logical HS message not received yet, wait one cycle and try again */
        /* Reset wait cycle for next iteration */
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);
      }
      else
      {
        /* The certificate chain is read from the certificate message and verified by the KeyM. */
        /* Store the ID of the received root certificate. */
        TcpIp_TlsClient_StoreReceivedRootCertId(TlsConIdx);

        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, nextState);
      }

    }
    else
    {
      TcpIp_TlsHsStm_HandlesUnexpectedHsInformation(TlsConIdx);
    }
  }
  else
  {
    /* #40 otherwise, we received an unexpected message. Trigger the alert */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
  }
} /* TcpIp_Tls12ClientHsStm_ParseServerCertificate() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ExtractServerCertificateInformation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractServerCertificateInformation(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare the message ServerCertificate until we are done AND no Async context needs to be computed */
  while (   (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_DONE)  /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
            /* Handshake is not aborted due to processing error */
         && (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_STATE_HANDSHAKE_ABORT)
            /* Async operation is pending */
         && (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_CTX_ASYNC_PENDING)
            /* Next state could not be determine (missing next HS message) - Wait cycle */
         && (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_CTX_WAIT_CYCLE)
            /* Error during parsing - abort next steps */
         && (retVal == E_OK))
  {
    switch (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx))
    {
      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE:
      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_LOADCERTS:
      {
        /* #20 Extract the certificates from the received chain, store them in the KeyM, and trigger KeyM to verify the cert group */
        if (TcpIp_Tls12ClientMsg_ServerCertMsgProcessCertChain(TlsConIdx) == E_OK)
        {
          TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_VERIFYGROUP_CHECK);
          TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_WAIT_CYCLE);
        }
        else
        {
          retVal = E_NOT_OK;
        }
        break;
      }

      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_VERIFYGROUP_CHECK:
      {
        /* #30 Periodically check if KeyM has finished the cert chain validation */
        if (KeyM_Cert_IsBusy() == TRUE)
        {
          /* The KeyM is still busy to process the cert chain validation */
          /* do nothing, just wait for next main function */
          TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_WAIT_CYCLE);
        }
        else
        {
          /* The cert chain validation has been finished by the KeyM. Now check the result and calculate an overall status. */
          TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_VERIFYGROUP_STATUS);
        }
        break;
      }

      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_VERIFYGROUP_STATUS:
      {
        /* #40 The cert chain validation has been finished by the KeyM. Now check the result and calculate an overall status. */
        retVal = TcpIp_TlsClientMsg_ServerCertMsgVerifyCertificateGroup(TlsConIdx);

        /* Decrement HsRxIndicationCnt since the certificate message is completely parsed,
         * only the next state is not determined yet. */
        TcpIp_DecHsRxIndicationCntOfTlsConnectionDyn(TlsConIdx);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_DETERMINE_NEXT_STATE);
        break;
      }

      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_DETERMINE_NEXT_STATE:
      {
        /* #50 Determine the next valid TLS handshake state */
        /* This state could be a WAIT_CYCLE if the next logical TLS message is not received yet */
        TcpIp_Tls12ClientMsg_ServerCertificateDetermineNextHsState(TlsConIdx, NextStatePtr);
        break;
      }

      default:                                                                                                           /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
      {
        break;
      } /* default */
    } /* switch() */

  } /* while() */

  return retVal;
} /* TcpIp_Tls12ClientMsg_ExtractServerCertificateInformation() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertMsgProcessCertChain
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertMsgProcessCertChain(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_SizeOfTlsClientBufferMgmtType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  /* If one of the following steps fails send a decode_error alert per default. */
  TcpIp_TlsAlertDescriptionType alertDescription = TCPIP_TLS_ALERT_DESC_DECODE_ERROR;
  uint16 certificatesLength = 0u;
  uint8 numCerts = 0u;  /* Number of received certificates */
  uint16 serverCertificateMsgLen = TcpIp_GetServerCertificatesLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the length of the certificate message and read the number of certificates. */
  if (TcpIp_Tls12Msg_RecCertMsgCheckLengthAndReadCertNum(TlsConIdx, serverCertificateMsgLen, &certificatesLength, &numCerts) == E_OK)
  {
    /* If the verification of the chain fails an unknown_ca alert should be send. */
    alertDescription = TCPIP_TLS_ALERT_DESC_UNKNOWN_CA;

    /* #20 Extract the certificates from the received chain and store them in the KeyM. */
    retVal = TcpIp_Tls12ClientMsg_ServerCertMsgParseAndTriggerVerification(TlsConIdx,
      TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx), numCerts);

    /* Update the read index */
    TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, certificatesLength);
  }

  if(retVal != E_OK)
  {
    /* #30 Extraction of server certificates failed, report user error */
    TcpIp_TlsUserErrorType UserError;
    UserError.FunctionId = TCPIP_TLS_FCTID_CLIENT_HSSTM_PARSE_SERVERCERT;
    UserError.UserErrorId = TCPIP_TLS_ERRORID_CTHS_INCORRECTHSMSG;
    UserError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(UserError);

    /* Trigger alert protocol_version in case of invalid length of certificate */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, alertDescription);

    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerCertMsgProcessCertChain */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertMsgParseAndTriggerVerification
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertMsgParseAndTriggerVerification(
  TcpIp_TlsConnectionIterType         TlsConIdx,
  TcpIp_TlsBufferRxIterType           CertStartIdx,
  uint8                               NumCerts)
{
  /* ----- Local Variables ---------------------------------------------- */

  TcpIp_TlsRemoteKeyMCertInfoStartIdxOfTlsConnectionType tlsRemoteKeyMCertInfoIdx
    = TCPIP_NO_TLSLOCALKEYMCERTINFOSTARTIDXOFTLSCONNECTION;

  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ---------------------------------------------- */
  /* TcpIp_TlsCoreUtil_CheckEccKeyMConfiguration() ensures
   * that there is exactly one KeyM remote reference for this connection.
   * The behavior for multiple references and therefore multiple KeyM groups is unclear.
   * After this check we can rely on only one KeyM group which is referenced through TlsRemoteKeyMCertInfoStartIdx. */
  if (TcpIp_TlsCoreUtil_CheckEccKeyMConfiguration(TlsConIdx) == E_OK)
  {
    tlsRemoteKeyMCertInfoIdx = TcpIp_GetTlsRemoteKeyMCertInfoStartIdxOfTlsConnection(TlsConIdx);

    /* #10 Set the certificates from the received certificate chain in the KeyM certificate group. */
    retVal = TcpIp_TlsCoreMsg_CertMsgSetCertificatesInGroup(TlsConIdx, CertStartIdx, NumCerts);

    /* #20 Check if there is a matching handshake / cipherworker for the received certificates. */
    if (retVal == E_OK)
    {
      retVal = TcpIp_Tls12ClientMsg_ServerCertMsgCheckCertsForMatchingHandshake(TlsConIdx);
    }

    /* #30 If all certificates were successfully extracted and loaded, trigger verification of the leaf certificate */
    if (retVal == E_OK)
    {
      /* Get the KeyM group with TlsRemoteKeyMCertInfoStartIdx. */
      if (KeyM_VerifyGroup(TcpIp_GetCertGroupIdOfTlsRemoteKeyMCertInfo(tlsRemoteKeyMCertInfoIdx)) == E_OK)
      {
        /* Update Hs certificate state to in progress */
        /* [HS Server Certificate Chain Handling StM] Certificates loaded successfully and verification triggered  */
        TcpIp_SetHsCertStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CERTCHAIN_IN_PROGRESS);
      }
      else
      {
        /* Verification failed - one reason may be Root or intermediate certificate not found, or KeyM is busy */
        TcpIp_TlsClientMsg_ServerCertMsgHandleFailedCertGroupValidation(TlsConIdx);

        retVal = E_NOT_OK;
      }
    }
  }
  else
  {
    TcpIp_TlsCoreUtil_HandleInvalidConfiguration(TlsConIdx);
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerCertMsgParseAndTriggerVerification */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertMsgCheckCertsForMatchingHandshake
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertMsgCheckCertsForMatchingHandshake(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  TcpIp_TlsEllipticCurveIdType curveId = TCPIP_CURVE_ID_NO_CURVE;
  /* Get the certificate Id of the leaf certificate. */
  TcpIp_TlsCertValidationStatusDynIterType tlsCertValidationStatusIdx = TcpIp_GetTlsCertValidationStatusDynStartIdxOfTlsConnection(TlsConIdx);
  KeyM_CertificateIdType certId = TcpIp_GetAddrTlsCertValidationStatusDyn(tlsCertValidationStatusIdx)->CertId;
  KeyM_CertificateStatusType certStatus = KEYM_CERTIFICATE_NOT_AVAILABLE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the certificate is available. */
  if ((KeyM_CertGetStatus(certId, &certStatus) == E_OK) && (certStatus != KEYM_CERTIFICATE_NOT_AVAILABLE))
  {
    /* #20 Get the curve used in the public key of the leaf certificate. */
    /* We assume for now that the curve is the same for all certificates in the chain.
     * This is checked later in the KeyM (KeyM_VerifyGroup()). */
    if (TcpIp_TlsCoreUtil_GetCurveFromCertificate(certId, &curveId) == E_OK)
    {
      /* #30 Set an active cipherworker which matches the received ciphersuite and the curve of the received certificates. */
      retVal = TcpIp_TlsClientMsg_SetActiveCipherWorkerAccordingToReceivedServerParams(TlsConIdx, curveId);
    }
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerCertMsgCheckCertsForMatchingHandshake */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertificateDetermineNextHsState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050, 6080 1 */ /* MD_TcpIp_STCAL_CslAccess, MD_TcpIp_TlsStatemachine_STMIF_6080 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertificateDetermineNextHsState(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */

  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxServerCertificateStatusIdxOfTlsClientBufferMgmtType serverCertificateStatusIdx =
    TcpIp_GetTlsBufferRxServerCertificateStatusIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmtType serverHelloDoneIdx
    = TcpIp_GetTlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertificateRequestIdx
    = TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmtType serverKeyExchangeIdx
    = TcpIp_GetTlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerFinishedIdxOfTlsClientBufferMgmtType serverFinishedIdx
    = TcpIp_GetTlsBufferRxServerFinishedIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsKeyExchangeMethodDataType keyExchange =
    TcpIp_TlsCoreUtil_ReturnActiveCipherSpecificKeyExchangeMethod(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */

  /* Reset the context state - in case we did not receive the next message, this will be overwritten to WAIT_CYCLE */
  /* If the next message is received, we must reset it to NO_ASYNC */
  TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_NO_ASYNC);

  /* #10 Update the next valid TLS handshake state, depending on the active key exchange method and the received messages */
  /* This function can be called multiple times, when we do not received the next message and can not determine the valid
     next state */
  switch (keyExchange)
  {
    case TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH:
    {
      /* #20 Key exchange ECDH -> Next state could be ServerCertificateStatus or ServerHelloDone */
      if (serverCertificateStatusIdx != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATESTATUSIDXOFTLSCLIENTBUFFERMGMT)
      {
        /* ServerCertificateStatus received - next state is parsing of ServerCertificateStatus */
        *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS;
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_DONE);
      }
      else if (serverCertificateRequestIdx != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT)
      {
        /* ServerCertificateRequest received - next state is parsing of ServerCertificateRequest */
        *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_REQUEST;
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_DONE);
      }
      else if (serverHelloDoneIdx != TCPIP_NO_TLSBUFFERRXSERVERHELLODONEIDXOFTLSCLIENTBUFFERMGMT)
      {
        /* ServerHelloDone received and no ServerCertificateStatus - next state is parsing of ServerHelloDone */
        *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_HELLO_DONE;
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_DONE);
      }
      else /* if (   (serverCertificateStatusIdx == TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATESTATUSIDXOFTLSCLIENTBUFFERMGMT)
                  && (serverHelloDoneIdx == TCPIP_NO_TLSBUFFERRXSERVERHELLODONEIDXOFTLSCLIENTBUFFERMGMT)
                  && (serverCertificateRequestIdx == TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT)) */
      {
        /* None of the expected messages received. Check next invalid message */
        if (   (serverFinishedIdx != TCPIP_NO_TLSBUFFERRXSERVERFINISHEDIDXOFTLSCLIENTBUFFERMGMT)
            /* ECDH - We MUST not receive a Server KeyExchange message */
            || (serverKeyExchangeIdx != TCPIP_NO_TLSBUFFERRXSERVERKEYEXCHANGEIDXOFTLSCLIENTBUFFERMGMT))
        {
          /* Invalid handshake messages received - trigger alert */
          TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
          TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_HANDSHAKE_ABORT);
        }
        else
        {
          /* ServerCertificateStatus and ServerHelloDone not received yet.
           * Wait until next main function cycle to determine next state */
          TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_WAIT_CYCLE);
        }
      }

      break;
    } /* TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH */

    default:
    {
      /* #30 Key exchange ECDHE -> Next state could be ServerCertificateStatus or ServerKeyExchange */
      if (serverCertificateStatusIdx != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATESTATUSIDXOFTLSCLIENTBUFFERMGMT)
      {
        /* ServerCertificateStatus received - next state is parsing of ServerCertificateStatus */
        *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS;
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_DONE);
      }
      else if (serverKeyExchangeIdx != TCPIP_NO_TLSBUFFERRXSERVERKEYEXCHANGEIDXOFTLSCLIENTBUFFERMGMT)
      {
        /* ServerKeyExchange received and no ServerCertificateStatus - next state is parsing of ServerKeyExchange */
        *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE;
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_DONE);
      }

      else /*  (serverCertificateStatusIdx == TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATESTATUSIDXOFTLSCLIENTBUFFERMGMT)
            && (serverKeyExchangeIdx == TCPIP_NO_TLSBUFFERRXSERVERKEYEXCHANGEIDXOFTLSCLIENTBUFFERMGMT) */
      {
        /* None of the expected messages received. Check next invalid message */
        if (   (serverHelloDoneIdx != TCPIP_NO_TLSBUFFERRXSERVERHELLODONEIDXOFTLSCLIENTBUFFERMGMT)
            || (serverFinishedIdx != TCPIP_NO_TLSBUFFERRXSERVERFINISHEDIDXOFTLSCLIENTBUFFERMGMT)
            /* ECDHE - we MUST not receive  the Server CertificateRequest message before the Server KeyExchange message */
            || (serverCertificateRequestIdx != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT))
        {
          /* Invalid order of handshake messages received - trigger alert */
          TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
          TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_HANDSHAKE_ABORT);
        }
        else
        {
          /* ServerCertificateStatus and ServerKeyExchange not received yet.
           * Wait until next main function cycle to determine next state */
          TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_WAIT_CYCLE);
        }

      }
      break;
    } /* TCPIP_TLS_KEYEXCHANGE_METHOD_ECDHE */
  }

} /* TcpIp_Tls12ClientMsg_ServerCertificateDetermineNextHsState() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_ParseServerCertificateStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050, 6080 1 */ /* MD_TcpIp_STCAL_CslAccess, MD_TcpIp_TlsStatemachine_STMIF_6080 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerCertificateStatus(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxServerCertificateStatusIdxOfTlsClientBufferMgmtType serverCertificateStatusdx =
    TcpIp_GetTlsBufferRxServerCertificateStatusIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if we received a message with the TLS-Handshake type ServerCertificate */
  if (serverCertificateStatusdx != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATESTATUSIDXOFTLSCLIENTBUFFERMGMT)
  {
    TcpIp_TlsHandshakeState nextState = TCPIP_TLS_STATE_HANDSHAKE_INIT;
    Std_ReturnType retVal;

    /* #20 if so, extract the information inside the server certificate message */
    retVal = TcpIp_Tls12ClientMsg_ExtractServerCertificateStatusInformation(TlsConIdx, &nextState);

    /* #30 Check if the parsing of the message was successful */
    if (retVal == E_OK)
    {
      /* #40 Message parsed successfully - check if the parsing is completely done and change the state */
      if (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CTX_ASYNC_PENDING)
      {
        /* Async operation pending, nothing to do - just leave the function */
      }
      else if (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CTX_WAIT_CYCLE)
      {
        /* Next logical HS message not received yet, wait one cycle and try again */
        /* Reset wait cycle for next iteration */
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_DETERMINE_NEXT_STATE);
      }
      else
      {
        /* #50 Parsing completely done. Change the state depending on key exchange and received messages */
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, nextState);
      }
    }
    else
    {
      /* #60 Parsing failed. Cancel the handshake. */
      TcpIp_TlsHsStm_HandlesUnexpectedHsInformation(TlsConIdx);
    }
  }
  else
  {
    /* #70 otherwise, we received an unexpected message. Trigger the alert */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
  }
} /* TcpIp_Tls12ClientHsStm_ParseServerCertificateStatus() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ExtractServerCertificateStatusInformation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractServerCertificateStatusInformation(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Extract the message information until we are done, the handshake must be aborted, or the context should be switched */
  while (   (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx)  != TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_DONE)/* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
             /* Handshake is not aborted due to processing error */
         && (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_STATE_HANDSHAKE_ABORT)
             /* Async operation is pending */
         && (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_CTX_ASYNC_PENDING)
             /* Next state could not be determine (missing next HS message) - Wait cycle */
         && (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_CTX_WAIT_CYCLE)
             /* Error during parsing - abort next steps */
         && (retVal == E_OK))
  {
    switch (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx))
    {
      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS:
      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_GET_CERT_STATUS:
      {
        /* #20 Extract the received server certificate status information */
        retVal = TcpIp_Tls12ClientMsg_ServerCertStatusGetCertStatus(TlsConIdx);

        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx,
          TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_TRIGGER_OCSP_VALIDATION);
        break;
      } /* case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_GET_CERT_STATUS */

      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_TRIGGER_OCSP_VALIDATION:
      {
        /* This operation is time consuming and thus is calculated in the lowPrio (async) task (TcpIp_Tls12ClientCtx_AsyncHandler).
           Trigger the calculation, leave this context and return if the operation is done */
        /* #30 All information is parsed correctly, trigger the validation of the received OCSP response */
        TcpIp_Tls12ClientMsg_ServerCertStatusTriggerOcspValidation(TlsConIdx);

        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx,
          TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_UPDATE_CERT_STATUS);
        break;
      } /* case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_TRIGGER_OCSP_VALIDATION */

      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_UPDATE_CERT_STATUS:
      {
        /* #40 Validation of OCSP response performed - update the certificate status */
        retVal = TcpIp_Tls12ClientMsg_ServerCertStatusUpdateCertStatus(TlsConIdx);

        /* Asynchronous validation performed, reset the connection state */
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_NO_ASYNC);

        /* Decrement HsRxIndicationCnt since the certificate status message is completely parsed,
         * only the next state is not determined yet. */
        TcpIp_DecHsRxIndicationCntOfTlsConnectionDyn(TlsConIdx);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_DETERMINE_NEXT_STATE);
        break;
      } /* case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_UPDATE_CERT_STATUS */

      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_DETERMINE_NEXT_STATE:
      {
        /* This state could be a WAIT_CYCLE if the next logical TLS message is not received yet */
        TcpIp_Tls12ClientMsg_ServerCertStatusDetermineNextHsState(TlsConIdx, NextStatePtr);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_DONE);
        break;
      } /* case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_DETERMINE_NEXT_STATE */

      default: /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
      {
        break;
      } /* default */

    } /* switch() */
  } /* while() */
  return retVal;

} /* TcpIp_Tls12ClientMsg_ExtractServerCertificateStatusInformation() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertStatusGetCertStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertStatusGetCertStatus(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx =
    TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxAdHsReadIdxOfTlsBufferMgmtDynType certStatusEndIdx =
    (TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx) + TcpIp_GetServerCertificateStatusLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));

  uint32 expectedCertStatusLength
    = ((uint32)TcpIp_GetServerCertificateStatusLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) - TCPIP_TLS_HSHDRLEN);

  TcpIp_ExpectServerCertificateStatusMessageOfTlsConnectionDynType receivedServerCertStatusRequestExtension =
    TcpIp_IsExpectServerCertificateStatusMessageOfTlsConnectionDyn(TlsConIdx);

  Std_ReturnType retVal = E_NOT_OK;
  /* ----- Implementation ----------------------------------------------- */
  if (receivedServerCertStatusRequestExtension == TRUE)
  {
    /* Check that we receive at least 4 byte to read out the certificate status information */
    if (TCPIP_TLS_RXBUFFER_ISIDXINRANGE(TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx),
        (TCPIP_TLS_CERTSTATUS_REQUESTTYPE_LEN + TCPIP_TLS_CERTSTATUS_OCSPRESPONSE_LEN),
        certStatusEndIdx))
    {
      uint8 receivedCertStatusType;
      uint32 receivedCertStatusLength;

      /* #10 Get received certificate status request type */
      receivedCertStatusType = IpBase_GetUint8(TcpIp_GetAddrTlsBufferRx(
        TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)), 0);
      TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_CERTSTATUS_REQUESTTYPE_LEN);

      /* #20 Get received certificate status request length */
      receivedCertStatusLength = IpBase_GetUint24(
        TcpIp_GetAddrTlsBufferRx(TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)), 0);
      TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_CERTSTATUS_OCSPRESPONSE_LEN);

      /* #30 Validate the received certificate status information */
      if (   (receivedCertStatusType == TCPIP_TLS_CERTSTATUS_REQUESTTYPE_OCSP)
          && (receivedCertStatusLength > 0u)
          && (receivedCertStatusLength == expectedCertStatusLength))
      {
        TcpIp_TlsCertValidationStatusDynStartIdxOfTlsConnectionType leafTlsCertValidationStatusIdx
          = TcpIp_GetTlsCertValidationStatusDynStartIdxOfTlsConnection(TlsConIdx);

        TcpIp_SetServerCertStatusOcspResponseLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx, receivedCertStatusLength);

        /* #40 Get the received certificate status OCSP response */
        /* Used in TcpIp_Tls12ClientCtx_ValidateCertificateStatus() */
        TcpIp_SetTlsBufferRxServerCertStatusOcspResponseIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
          TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

        TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx,
          (TcpIp_TlsBufferRxAdHsReadIdxOfTlsBufferMgmtDynType)receivedCertStatusLength);

        /*#50 Update OCSP connection status and set the OCSP mode */
        /* We did receive the OCSP Certificate status response */
        TcpIp_GetAddrTlsCertValidationStatusDyn(leafTlsCertValidationStatusIdx)->CertStatusResponse.OcspConnectionState
          = TCPIP_TLS_OCSP_CONNECTION_STATE_STATUS_RECEIVED;

        /* Currently only RFC6066 TLS Single Certificate Status Request is supported */
        TcpIp_GetAddrTlsCertValidationStatusDyn(leafTlsCertValidationStatusIdx)->CertStatusResponse.OcspMode
          = TCPIP_TLS_OCSP_MODE_SINGLE;

        retVal = E_OK;

      }
    }
  }
  else
  {
    /* RFC6066 - 8 Certificate Status Request
     *   If a server returns a "CertificateStatus" message, then the server MUST have included an
     *   extension of type "status_request" with empty "extension_data" in the extended server hello.
     */
    /* #60 RFC violation - no status_request extension received in ServerHello - trigger Alert */
    /* User is informed via Validation callback later in the ServerHelloDone state */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerCertStatusGetCertStatus() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertStatusTriggerOcspValidation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertStatusTriggerOcspValidation(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientConnectionIterType tlsClientConnectionIdx
    = TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger the validation of the received OCSP response in the asynchronous task */
  /* Validation performed in TcpIp_Tls12ClientCtx_ValidateCertificateStatus() */
  TcpIp_SetCertificateStatusOcspValidationAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx, TRUE);
  TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_ASYNC_PENDING);

} /* TcpIp_Tls12ClientMsg_ServerCertStatusTriggerOcspValidation() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertStatusUpdateCertStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertStatusUpdateCertStatus(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the certificate chain status with the OCSP result */
  if (TcpIp_TlsCoreUtil_TlsUpdateCertChainStatus(TlsConIdx,
        TCPIP_TLS_UPDATE_CERT_MODE_OCSP) == E_OK)
  {
    TcpIp_CertificateStatusResponseType leafCertificateOcspStatus;
    leafCertificateOcspStatus.OcspCertStatus     = KEYM_E_KEY_CERT_INVALID;
    leafCertificateOcspStatus.OcspMode           = TCPIP_TLS_OCSP_MODE_NONE;
    leafCertificateOcspStatus.OcspResponseStatus = TCPIP_TLS_OCSP_RESPONSE_STATUS_INVALID;

    /* #20 Read the OCSP status of the leaf certificate */
    if (TcpIp_Tls12ClientMsg_ServerCertStatusGetLeafOcspResult(TlsConIdx, &leafCertificateOcspStatus) == E_OK)
    {
      /* #30 Check if the response status was successful and update the overall validation result */
      if (leafCertificateOcspStatus.OcspResponseStatus == TCPIP_TLS_OCSP_RESPONSE_STATUS_SUCCESSFUL)
      {
        /* Update the overall validation result only if the certificate is revoked */
        if (leafCertificateOcspStatus.OcspCertStatus == KEYM_E_CERTIFICATE_REVOKED)
        {
          /* #40 Leaf certificate is revoked - Update the overall validation result */
          TcpIp_SetTlsCertValidationResultOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_VALIDATION_NOT_OK);
          retVal = E_OK;
        }
        else
        {
          /* Certificate is valid, do not set the validation status and return */
          retVal = E_OK;
        }
      }
      else
      {
        /* #50 Received OCSP response was not successful - Update the overall validation result */
        TcpIp_SetTlsCertValidationResultOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_VALIDATION_NOT_OK);
        retVal = E_OK;
      }
    } /* else - return E_NOT_OK */
  } /* else - return E_NOT_OK */

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerCertStatusUpdateCertStatus() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertStatusGetLeafOcspResult
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertStatusGetLeafOcspResult(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_CP2V(TcpIp_CertificateStatusResponseType) LeafCertStatusResponsePtr)
{
  /* ----- Local Variables ---------------------------------------------- */

  TcpIp_TlsCertValidationStatusDynIterType tlsCertValidationStatusIdx = TcpIp_GetTlsCertValidationStatusDynStartIdxOfTlsConnection(TlsConIdx);
  KeyM_CertificateIdType leafCertId = TcpIp_GetAddrTlsCertValidationStatusDyn(tlsCertValidationStatusIdx)->CertId;


  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all received server certificates and read out the status of the leaf certificate */
  for (tlsCertValidationStatusIdx = TcpIp_GetTlsCertValidationStatusDynStartIdxOfTlsConnection(TlsConIdx);
       tlsCertValidationStatusIdx < TcpIp_GetTlsCertValidationStatusDynValidEndIdxOfTlsConnectionDyn(TlsConIdx);
       tlsCertValidationStatusIdx++)
  {
    /* Pointer to the certificate status struct */
    TCPIP_P2C(TcpIp_CertValidationStatusType) certStatusPtr = TcpIp_GetAddrTlsCertValidationStatusDyn(tlsCertValidationStatusIdx);

    if (certStatusPtr->CertId == leafCertId)
    {
      /* Leaf certificate found, store the OCSP status and indicate the caller function a successful read out */
      *LeafCertStatusResponsePtr = certStatusPtr->CertStatusResponse;

      retVal = E_OK;
      break;
    }
  }
  /* #20 else - no leaf certificate found. return E_NOT_OK */

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerCertStatusGetLeafOcspResult() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertStatusDetermineNextHsState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertStatusDetermineNextHsState(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmtType serverHelloDoneIdx
    = TcpIp_GetTlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertificateRequestIdx
    = TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmtType serverKeyExchangeIdx
    = TcpIp_GetTlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerFinishedIdxOfTlsClientBufferMgmtType serverFinishedIdx
    = TcpIp_GetTlsBufferRxServerFinishedIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsKeyExchangeMethodDataType keyExchange =
    TcpIp_TlsCoreUtil_ReturnActiveCipherSpecificKeyExchangeMethod(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */

  /* Reset the context state - in case we did not receive the next message, this will be overwritten to WAIT_CYCLE */
  /* If the next message is received, we must reset it to NO_ASYNC */
  TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_NO_ASYNC);

  /* #10 Update the next valid TLS handshake state, depending on the active key exchange method and the received messages */
  /* This function can be called multiple times, when we do not received the next message and can not determine the valid
     next state */

  switch (keyExchange)
  {
    case TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH:
    {
      /* #20 Key exchange ECDH - Next state could be ServerCertificateRequest or ServerHelloDone */
      if (serverCertificateRequestIdx != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT)
      {
        /* ServerCertificateRequest received - next state is parsing of ServerCertificateRequest */
        *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_REQUEST;
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_DONE);

      }
      else if (serverHelloDoneIdx != TCPIP_NO_TLSBUFFERRXSERVERHELLODONEIDXOFTLSCLIENTBUFFERMGMT)
      {
        /* ServerHelloDone received and no ServerCertificateStatus - next state is parsing of ServerHelloDone */
        *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_HELLO_DONE;
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_DONE);

      }
      else /* if (   (serverHelloDoneIdx == TCPIP_NO_TLSBUFFERRXSERVERHELLODONEIDXOFTLSCLIENTBUFFERMGMT)
                  && (serverCertificateRequestIdx == TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT)) */
      {
        /* None of the expected messages received. Check next invalid message */
        if (   (serverFinishedIdx != TCPIP_NO_TLSBUFFERRXSERVERFINISHEDIDXOFTLSCLIENTBUFFERMGMT)
            /* ECDH - We MUST not receive a Server KeyExchange message */
            || (serverKeyExchangeIdx != TCPIP_NO_TLSBUFFERRXSERVERKEYEXCHANGEIDXOFTLSCLIENTBUFFERMGMT))
        {
          /* Invalid order of handshake messages received - trigger alert */
          TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
          TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_HANDSHAKE_ABORT);
        }
        else
        {
          /* ServerCertificateRequest and ServerHelloDone not received yet.
           * Wait until next main function cycle to determine next state */
          TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_WAIT_CYCLE);
        }
      }

      break;
    } /* TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH */

    default:
    {
      /* #30 Key exchange ECDHE - Next state should be ServerKeyExchange */
      if (serverKeyExchangeIdx != TCPIP_NO_TLSBUFFERRXSERVERKEYEXCHANGEIDXOFTLSCLIENTBUFFERMGMT)
      {
        /* ServerKeyExchange received - next state is parsing of ServerKeyExchange */
        *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE;
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS_DONE);

      }
      else /*  (serverKeyExchangeIdx == TCPIP_NO_TLSBUFFERRXSERVERKEYEXCHANGEIDXOFTLSCLIENTBUFFERMGMT) */
      {
        /* None of the expected messages received. Check next invalid message */
        if (   (serverHelloDoneIdx != TCPIP_NO_TLSBUFFERRXSERVERHELLODONEIDXOFTLSCLIENTBUFFERMGMT)
            || (serverFinishedIdx != TCPIP_NO_TLSBUFFERRXSERVERFINISHEDIDXOFTLSCLIENTBUFFERMGMT)
            /* ECDHE - We MUST not receive the Server CertificateRequest message before the Server KeyExchange message */
            || (serverCertificateRequestIdx != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT))
        {
          /* Invalid order of handshake messages received - trigger alert */
          TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
          TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_HANDSHAKE_ABORT);
        }
        else
        {
          /* ServerKeyExchange not received yet.
           * Wait until next main function cycle to determine next state */
          TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_WAIT_CYCLE);
        }
      }
      break;
    } /* TCPIP_TLS_KEYEXCHANGE_METHOD_ECDHE */
  }

} /* TcpIp_Tls12ClientMsg_ServerCertStatusDetermineNextHsState() */
#   endif /* (TCPIP_SUPPORT_ALSTLS_ECC == STD_ON ) */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_ParseServerKeyExchange
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerKeyExchange(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx =
    TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmtType serverKeyExchangeIdx =
    TcpIp_GetTlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if we received a handshake message with the type ServerKeyExchange */
  if (serverKeyExchangeIdx != TCPIP_NO_TLSBUFFERRXSERVERKEYEXCHANGEIDXOFTLSCLIENTBUFFERMGMT)
  {
    TcpIp_TlsHandshakeState nextState = TCPIP_TLS_STATE_HANDSHAKE_INIT;
    Std_ReturnType retVal;

    TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, FALSE);

    /* #20 ServerKeyExchange message received - forward the message to the parser to extract the contained information */
    retVal = TcpIp_Tls12ClientMsg_ExtractServerKeyExchangeInformation(TlsConIdx, &nextState);

    /* #30 Check if the parsing of the message was successful */
    if (retVal == E_OK)
    {
      /* #40 Message parsed successfully - check if the parsing is completely done and change the state */
      if (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CTX_ASYNC_PENDING)
      {
        /* Async operation pending, nothing to do - just leave the function */
      }
      else if (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CTX_WAIT_CYCLE)
      {
        /* Next logical HS message not received yet, wait one cycle and try again */
        /* Reset wait cycle for next iteration */
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);
      }
      else
      {
        /* #50 Parsing completely done. Change the state to TCPIP_TLS_STATE_WAIT_FOR_SERVER_HELLO_DONE */
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, nextState);
      }
    }
    else
    {
      /* #60 Parsing failed. Cancel the handshake. */
      TcpIp_TlsHsStm_HandlesUnexpectedHsInformation(TlsConIdx);
    }
  }
  else
  {
    /* #70 otherwise, we received an unexpected message, trigger the alert */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
  }
} /* TcpIp_Tls12ClientHsStm_ParseServerKeyExchange() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ExtractServerKeyExchangeInformation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractServerKeyExchangeInformation(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Extract the message information until we are done, the handshake must be aborted, or the context should be switched */
  while (   (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_DONE) /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
             /* Handshake is not aborted due to processing error */
         && (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_STATE_HANDSHAKE_ABORT)
             /* Async operation is pending */
         && (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_CTX_ASYNC_PENDING)
             /* Next state could not be determine (missing next HS message) - Wait cycle */
         && (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_CTX_WAIT_CYCLE)
             /* Error during parsing - abort next steps */
         && (retVal == E_OK))
  {
    switch (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx))
    {
      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE:
      {
        TcpIp_TlsHandshakeState nextState = TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_DONE;

        /* #20 Call the subfunction to parse the received ServerKeyExchange parameters depending on the selected key exchange method */
        retVal = TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParams(TlsConIdx, &nextState);
        /* Depending on keyExchange Method, next state is: TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_VERIFY_SIGNATURE | TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_DETERMINE_NEXT_STATE */
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, nextState);

        if (nextState == TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_DETERMINE_NEXT_STATE)
        {
          /* Decrement HsRxIndicationCnt since the ServerKeyExchange message is completely parsed,
           * only the next state is not determined yet. */
          TcpIp_DecHsRxIndicationCntOfTlsConnectionDyn(TlsConIdx);
        }
        break;
      } /* case TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE */

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_VERIFY_SIGNATURE:
      {
        /* This operation is time consuming and thus is calculated in the lowPrio (async) task.
           Trigger the calculation, leave this context and return if the operation is done */

        /* #30 Parse the received signature */
        TcpIp_Tls12ClientMsg_ServerKeyExchangeVerifySignature(TlsConIdx);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_WAIT_FOR_ASYNC);

        break;
      } /* case TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_VERIFY_SIGNATURE */

      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_WAIT_FOR_ASYNC:
      {
        /* #40 Parsing of ServerKeyExchange message done */
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_NO_ASYNC);

        /* Decrement HsRxIndicationCnt since the ServerKeyExchange message is completely parsed,
         * only the next state is not determined yet. */
        TcpIp_DecHsRxIndicationCntOfTlsConnectionDyn(TlsConIdx);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_DETERMINE_NEXT_STATE);
        break;
      } /* case TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_WAIT_FOR_ASYNC */
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

      case TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_DETERMINE_NEXT_STATE:
      {
        TcpIp_Tls12ClientMsg_ServerKeyExchangeDetermineNextHsState(TlsConIdx, NextStatePtr);
        /* This state could be a WAIT_CYCLE if the next logical TLS message is not received yet */
        break;
      } /* TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_DETERMINE_NEXT_STATE */

      default: /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
      {
        break;
      } /* default */

    } /* switch() */
  } /* while() */

  return retVal;
} /* TcpIp_Tls12ClientMsg_ExtractServerKeyExchangeInformation() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParams
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParams(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsKeyExchangeMethodDataType keyExchangeMethod
    = TcpIp_TlsClientHsStm_ReturnReceivedCipherSpecificKeyExchangeMethod(TlsConIdx);
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 get the current key exchange method and get the server parameter depending on the key exchange mode */
  switch (keyExchangeMethod)
  {
#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
    case TCPIP_TLS_KEYEXCHANGE_METHOD_ECDHE:
    case TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH:
    {
      /* In case of ECC, get the ECC server parameter */
      retVal = TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsEcc(TlsConIdx);
      *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_VERIFY_SIGNATURE;
      break;
    }
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */
    case TCPIP_TLS_KEYEXCHANGE_METHOD_PSK_PLAIN:
    {
      /* In case of PSK, get the PSK server parameter */
      retVal = TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsPsk(TlsConIdx);
      *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_DETERMINE_NEXT_STATE;
      break;
    }
    default:
    {
      /* Invalid key exchange method. This can never happen, a valid key exchange method has been chosen with the active
      cipher suite received in the server_hello message. */
      break;
    }
  }  /* switch (keyExchangeMethod) */

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParams() */

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsEcc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsEcc(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmtType serverKeyExchgStartIdx =
    TcpIp_GetTlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_ServerKeyExchangeLenOfTlsClientBufferMgmtType serverKeyExchangeLen =
    TcpIp_GetServerKeyExchangeLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmtType serverKeyExchangeEndIdx
    = serverKeyExchgStartIdx + TCPIP_TLS_HSHDRLEN + serverKeyExchangeLen; /* End index of the received Server key exchange message - Need for in range checks inside the parser */

  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Parse the received ServerKeyExchange message */
  /* Parse the Curve Info Type */
  retVal = TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveType(TlsConIdx);

  if (retVal == E_OK)
  {
    /* Parse the Curve Info Id */
    retVal = TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveId(TlsConIdx);
  }

  if (retVal == E_OK)
  {
    /* Parse the Public Key */
    retVal = TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetPubKey(TlsConIdx, serverKeyExchangeEndIdx);
  }

  if (retVal == E_OK)
  {
    /* Parse the Signature Algorithm */
    retVal = TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignAlgorithm(TlsConIdx);
  }

  if (retVal == E_OK)
  {
    /* Parse the Signature */
    retVal = TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignature(TlsConIdx, serverKeyExchangeEndIdx);
  }

  if (retVal == E_OK)
  {
    /* Check if there is some more information in the buffer */
    retVal = TcpIp_TlsMsg_CheckMsgForRemainingData(TlsConIdx, serverKeyExchangeEndIdx);
  }

  return retVal;

} /* TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsEcc() */
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsPsk
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsPsk(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  uint16 expectedServerKeyExchangeLength
    = TcpIp_GetServerKeyExchangeLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxAdHsReadIdxOfTlsBufferMgmtDynType tlsBufferRxAdHsReadIdx;
  uint16 receivedPskHintLen;

  Std_ReturnType retVal     = E_NOT_OK;
  TcpIp_TlsUserErrorIdType localErrorId = TCPIP_TLS_ERRORID_NOT_SET;
  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, TRUE);

  /* Set the Index to the start of the PSK Parameter */
  TcpIp_SetTlsBufferRxServerKeyExchangePskParamIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
      TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

  tlsBufferRxAdHsReadIdx = TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

  /* Get the received PSK hint length */
  if (TCPIP_TLS_RXBUFFER_ISIDXINRANGE(tlsBufferRxAdHsReadIdx,                                                           /* PRQA S 2995 3 */ /* MD_TcpIp_Tls_2995 */
      TCPIP_TLS_KEYEXCH_PSKIDLEN,
     ((TcpIp_TlsBufferRxIterType)(tlsBufferRxAdHsReadIdx + TCPIP_TLS_KEYEXCH_PSKIDLEN))))
  {
    receivedPskHintLen =
      IpBase_GetUint16(TcpIp_GetAddrTlsBufferRx(TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)), 0);

    /* and increment the read index by the length of the PSK-Hint length field */
    TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_KEYEXCH_PSKIDLEN);

    /* Check that the received PSK length matches the expected one */
    if ((receivedPskHintLen + TCPIP_TLS_KEYEXCH_PSKIDLEN) == expectedServerKeyExchangeLength)
    {
      /* #10 Check the length of the received PSK-Hint and store the information */
      /* If a PskHint with the length 0 is received, we do not send an alert or close the connection since the RFC is not
         clear on this point. The ServerKeyExchange message is ignored but part of the TLS handshake */
      if (receivedPskHintLen <= TCPIP_TLS_KEYEXCH_PSKIDHINTMAXLEN)
      {
        /* #20 A valid PSK hint length was received, store the information */
        /* Set the received Length */
        TcpIp_SetServerKeyExchangePskHintLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx, receivedPskHintLen);

        /* Set the start index of the received PSK-Hint */
        TcpIp_SetTlsBufferRxServerKeyExchangePskHintIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
          TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

        tlsBufferRxAdHsReadIdx = TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

        if (TCPIP_TLS_RXBUFFER_ISIDXINRANGE(tlsBufferRxAdHsReadIdx,                                                       /* PRQA S 2995 3 */ /* MD_TcpIp_Tls_2995 */
          receivedPskHintLen,
          ((TcpIp_TlsBufferRxIterType)(tlsBufferRxAdHsReadIdx + receivedPskHintLen))))
        {
          /* and increment the read index by the length of the PSK-Hint field */
          TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, receivedPskHintLen);
          retVal = E_OK;
        }
        else
        {
          /* The received PSK length is not in range of the TLS buffer (malformed KeyExchange message) */
          localErrorId = TCPIP_TLS_ERRORID_CTHS_PSKHINTNOTINRANGE;
        }
      }
      else
      {
        /* An to large PSK hint length was received, report an error to the user */
        localErrorId = TCPIP_TLS_ERRORID_CTHS_PSKHINTTOLARGE;
      }
    }
    else
    {
      /* An incorrect PSK hint length was received, report an error to the user */
      localErrorId = TCPIP_TLS_ERRORID_CTHS_PSKHINTMSGINVALIDLENGTH;
    }
  }
  else
  {
    /* The received PSK length is not in range of the TLS buffer (malformed KeyExchange message) */
    localErrorId = TCPIP_TLS_ERRORID_CTHS_PSKHINTNOTINRANGE;
  }

  /* #30 Report error to user, error ID was set */
  if ( localErrorId != TCPIP_TLS_ERRORID_NOT_SET)
  {
    TcpIp_TlsUserErrorType UserError;
    UserError.FunctionId = TCPIP_TLS_FCTID_CLIENT_HSSTM_PARSE_SERVERKEYEXCH;
    UserError.UserErrorId = localErrorId;
    UserError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(UserError);
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_DECODE_ERROR);
    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerKeyExchangeGetServerParamsPsk() */

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveType
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveType(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  Std_ReturnType retVal = E_OK;
  uint8 curveType;

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SetTlsBufferRxServerKeyExchangeParamsIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
    TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

  /* Get the received curve type  */
  curveType = IpBase_GetUint8(TcpIp_GetAddrTlsBufferRx(
    TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)), 0);

  /* Increment the read index by the length of the curve type field */
  TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_KEYEXCH_CURVETYPELEN);

  /* #10 Check if the received curveType is supported. Only 'named_curve(0x03)' is supported */
  if (curveType != TCPIP_TLS_CURVETYPE_NAMED_CURVE)
  {
    /* Incorrect curve type received - Report an error to the user */
    TcpIp_TlsUserErrorType UserError;
    UserError.FunctionId = TCPIP_TLS_FCTID_CLIENT_HSSTM_PARSE_SERVERKEYEXCH;
    UserError.UserErrorId = TCPIP_TLS_ERRORID_CTHS_INCORRECTCURVEINFO;
    UserError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(UserError);

    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_DECODE_ERROR);
    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveType */


/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveId(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsEllipticCurveIdType curveId = TCPIP_CURVE_ID_NO_CURVE;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the received curve ID */
  curveId = IpBase_GetUint16(
    TcpIp_GetAddrTlsBufferRx(TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)), 0);

  /* And increment the read index by the length of the curve ID field */
  TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_KEYEXCH_CURVENAMELEN);

  /* #20 Check if the curve Id fits to the received certificate. */
  if (curveId == TcpIp_GetEllipticCurveIdOfTlsConnectionDyn(TlsConIdx))
  {
    /* The check if the curve ID is supported is not necessary anymore,
     * since this was already checked for the curve of the certificate. */
    retVal = E_OK;
  }

  if (retVal != E_OK)
  {
    /* Incorrect curve ID received - Report an error to the user */
    TcpIp_TlsUserErrorType UserError;
    UserError.FunctionId = TCPIP_TLS_FCTID_CLIENT_HSSTM_PARSE_SERVERKEYEXCH;
    UserError.UserErrorId = TCPIP_TLS_ERRORID_CTHS_INCORRECTCURVEINFO;
    UserError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(UserError);

  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetCurveId() */


/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetPubKey
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetPubKey(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_TlsBufferRxIterType   ServerKeyExchangeEndIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsHandshakeActiveIdxOfTlsCipherWorkerDynType activeTlsHandshakeIdx
    = TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));

  Std_ReturnType retVal = E_NOT_OK;
  uint8          expectedPubKeyLen;

  /* ----- Implementation ----------------------------------------------- */
  /* Check if the activeTlsHandshakeIdx is valid */
  if (activeTlsHandshakeIdx != TCPIP_NO_TLSHANDSHAKEACTIVEIDXOFTLSCIPHERWORKERDYN)
  {
    if (TcpIp_TlsCoreUtil_CheckPublicKeyCompressionFlagOfEllipticCurve(
      TcpIp_GetEllipticCurveIdOfTlsHandshake(activeTlsHandshakeIdx)) == TRUE)
    {
      /* expected length of public key contains one further byte for compression flag */
      expectedPubKeyLen = (TcpIp_TlsMsg_GetLenOfCurveSpecificPubKey(
        TcpIp_GetEllipticCurveIdOfTlsHandshake(activeTlsHandshakeIdx))) + TCPIP_TLS_KEYEXCH_COMPFLAGLEN;
    }
    else
    {
      /* expected length of public key does not contain any further bytes */
      expectedPubKeyLen = (TcpIp_TlsMsg_GetLenOfCurveSpecificPubKey(
        TcpIp_GetEllipticCurveIdOfTlsHandshake(activeTlsHandshakeIdx)));
    }

    /* #10 Get the received public key and compare it to the expected key */
    retVal = TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgCompareReceivedAndExpectedPubKey(TlsConIdx,
      ServerKeyExchangeEndIdx, expectedPubKeyLen);
  }
  /* else - invalid activeTlsHandshakeIdx. Return E_NOT_OK */

  /* #20 Check if the public key was extracted properly */
  if (retVal != E_OK)
  {
    /* #30 Incorrect curve Id received - Report an error to the user */
    TcpIp_TlsUserErrorType UserError;
    UserError.FunctionId = TCPIP_TLS_FCTID_CLIENT_HSSTM_PARSE_SERVERKEYEXCH;
    UserError.UserErrorId = TCPIP_TLS_ERRORID_CTHS_INCORRECTPUBKEYLEN;
    UserError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(UserError);
    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetPubKey */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgCompareReceivedAndExpectedPubKey
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
 /* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgCompareReceivedAndExpectedPubKey(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_TlsBufferRxIterType   ServerKeyExchangeEndIdx,
  uint8                       expectedPubKeyLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsHandshakeActiveIdxOfTlsCipherWorkerDynType activeTlsHandshakeIdx
    = TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));

  Std_ReturnType retVal = E_OK;
  uint8          receivedServerPubKeyLen;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the received public key length and compare it to the expected key length */
  receivedServerPubKeyLen = IpBase_GetUint8(TcpIp_GetAddrTlsBufferRx(
    TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)), 0);

  if (receivedServerPubKeyLen == expectedPubKeyLen)
  {
    /* Set the index to the start of the public key length */
    TcpIp_SetTlsBufferRxServerKeyExchangePubKeyLenIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
      TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

    /* and increment the read index by the length of the public key length field */
    TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_KEYEXCH_PUBKEYLEN);

    if (TCPIP_TLS_RXBUFFER_ISIDXINRANGE(TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx),
      (TcpIp_TlsBufferRxIterType)receivedServerPubKeyLen, ServerKeyExchangeEndIdx))
    {
      /* #20 Check if we expect a compression flag for this received public key */
      if (TcpIp_TlsCoreUtil_CheckPublicKeyCompressionFlagOfEllipticCurve(TcpIp_GetEllipticCurveIdOfTlsHandshake(activeTlsHandshakeIdx)) == TRUE)
      {
        /* #30 if so, read and compare the compression flag of public key with the expected compression flag */
        uint8 receivedCompressionFlag = IpBase_GetUint8(TcpIp_GetAddrTlsBufferRx(
          TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)), 0);

        if (receivedCompressionFlag != TCPIP_TLS_PUBKEY_COMPRESSION_FLAG)
        {
          /* invalid compression flag received */
          retVal = E_NOT_OK;
        }

        receivedServerPubKeyLen -= TCPIP_TLS_KEYEXCH_COMPFLAGLEN;

        /* Increment the write index by the consumed length (1 Byte) */
        TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_KEYEXCH_COMPFLAGLEN);
      }

      /* Set the index to the start of the public key (including compression flag if present) */
      TcpIp_SetTlsBufferRxServerKeyExchangePubKeyIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
        TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

      /* and increment the read index by the length of the public key field */
      TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, receivedServerPubKeyLen);
    }
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgCompareReceivedAndExpectedPubKey */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignAlgorithm
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignAlgorithm(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsHandshakeActiveIdxOfTlsCipherWorkerDynType activeTlsHandshakeIdx =
    TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));
  TcpIp_SignatureAlgorithmIdType expectedSigHashAlgoId
    = TcpIp_GetSignatureAlgorithmIdOfTlsHandshake(activeTlsHandshakeIdx);
  uint16 receivedSigHashAlgoId;
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* Get the received signature Hash + algorithm ID */
  receivedSigHashAlgoId = IpBase_GetUint16(TcpIp_GetAddrTlsBufferRx(
    TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)), 0);

  /* and increment the read index by the length of the signature algorithm signature field */
  TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx,
    (TCPIP_TLS_KEYEXCH_SIGALGOSIGNLEN + TCPIP_TLS_KEYEXCH_SIGALGOHASHLEN));

  /* #10 Check if the received signature algorithm matches the expected one */
  if (receivedSigHashAlgoId != (uint16)expectedSigHashAlgoId)
  {
    /* #20 Incorrect hash algorithm Id received - Report an error to the user */
    TcpIp_TlsUserErrorType UserError;
    UserError.FunctionId = TCPIP_TLS_FCTID_CLIENT_HSSTM_PARSE_SERVERKEYEXCH;
    UserError.UserErrorId = TCPIP_TLS_ERRORID_CTHS_INCORRECTCURVEINFO;
    UserError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(UserError);
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_DECODE_ERROR);

    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignAlgorithm */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignature
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignature(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_TlsBufferRxIterType   ServerKeyExchangeEndIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);
  Std_ReturnType retVal = E_NOT_OK;
  uint16         serverSignLen = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the received signature length field is in range */
  if (TCPIP_TLS_RXBUFFER_ISIDXINRANGE(TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx),
    TCPIP_TLS_KEYEXCH_SIGLENLEN, ServerKeyExchangeEndIdx))
  {
    /* Get the received signature length */
    serverSignLen = IpBase_GetUint16(TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)), 0);

    /* Set the index to the start of the signature length */
    TcpIp_SetTlsBufferRxServerSignatureLenIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
      TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

    /* And increment the read index by the length of the signature length field */
    TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_KEYEXCH_SIGLENLEN);

    /* #20 Check if the received signature length is in range */
    /* Validation of signature length is performed later */
    if (TCPIP_TLS_RXBUFFER_ISIDXINRANGE(TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx),            /* PRQA S 2995 2 */ /* MD_TcpIp_Tls_2995 */
          (TcpIp_TlsBufferRxIterType)serverSignLen, ServerKeyExchangeEndIdx))
    {
      /* Set the index to the start of the signature */
      TcpIp_SetTlsBufferRxServerSignatureIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
        TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

      /* and increment the read index by the length of the signature field */
      TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, serverSignLen);

      retVal = E_OK;
    }
  }

  /* Check if the signature was extracted properly */
  if (retVal != E_OK)
  {
    /* Incorrect curve Id received - Report an error to the user */
    TcpIp_TlsUserErrorType UserError;
    UserError.FunctionId = TCPIP_TLS_FCTID_CLIENT_HSSTM_PARSE_SERVERKEYEXCH;
    UserError.UserErrorId = TCPIP_TLS_ERRORID_CTHS_INCORRECTSIGNLEN;
    UserError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(UserError);
    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignature */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerKeyExchangeVerifySignature
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeVerifySignature(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientConnectionIterType tlsClientConnectionIdx
    = TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger the calculation of the job in the async context handler and change the handshake state to ASYNC_PENDING */
  TcpIp_SetHsVerifySignatureAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx, TRUE);

  /* Set the state to ASYNC_PENDING */
  TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_ASYNC_PENDING);

} /* TcpIp_Tls12ClientMsg_ServerKeyExchangeVerifySignature() */
#   endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerKeyExchangeDetermineNextHsState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
  TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerKeyExchangeDetermineNextHsState(
    TcpIp_TlsConnectionIterType TlsConIdx,
    TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmtType serverHelloDoneIdx
    = TcpIp_GetTlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertificateRequestIdx
    = TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerFinishedIdxOfTlsClientBufferMgmtType serverFinishedIdx
    = TcpIp_GetTlsBufferRxServerFinishedIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsKeyExchangeMethodDataType keyExchangeMethod
    = TcpIp_TlsCoreUtil_ReturnActiveCipherSpecificKeyExchangeMethod(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */

  /* Reset the context state - in case we did not receive the next message, this will be overwritten to WAIT_CYCLE */
  /* If the next message is received, we must reset it to NO_ASYNC */
  TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_NO_ASYNC);

  /* #10 Update the next valid TLS handshake state, depending on the active key exchange method and the received messages */
  /* This function can be called multiple times, when we do not received the next message and can not determine the valid
     next state */
  /* #20 ServerHelloDone has been received, notify the certificate chain validation result to user */
  if (   (keyExchangeMethod == TCPIP_TLS_KEYEXCHANGE_METHOD_ECDHE)
      || (keyExchangeMethod == TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH))
  {
    /* Next state could be ServerCertificateRequest or ServerHelloDone */
    if (serverCertificateRequestIdx != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT)
    {
      /* ServerCertificateRequest received - next state is parsing of ServerCertificateRequest */
      *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_REQUEST;
      TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_DONE);
    }
    else if (serverHelloDoneIdx != TCPIP_NO_TLSBUFFERRXSERVERHELLODONEIDXOFTLSCLIENTBUFFERMGMT)
    {
      /* ServerHelloDone received and no ServerCertificateStatus - next state is parsing of ServerHelloDone */
      *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_HELLO_DONE;
      TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_DONE);
    }
    else /* if (   (serverHelloDoneIdx == TCPIP_NO_TLSBUFFERRXSERVERHELLODONEIDXOFTLSCLIENTBUFFERMGMT)
                && (serverCertificateRequestIdx == TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT)) */
    {
      /* None of the expected messages received. Check next invalid message */
      if (serverFinishedIdx != TCPIP_NO_TLSBUFFERRXSERVERFINISHEDIDXOFTLSCLIENTBUFFERMGMT)
      {
        /* Invalid handshake messages received - trigger alert */
        TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_HANDSHAKE_ABORT);
      }
      else
      {
        /* ServerCertificateRequest and ServerHelloDone not received yet.
         * Wait until next main function cycle to determine next state */
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_WAIT_CYCLE);
      }
    }
  }
  else
  {
    /* key exchange is PSK, next state must be ServerHelloDone */
    *NextStatePtr = TCPIP_TLS_STATE_WAIT_FOR_SERVER_HELLO_DONE;
    TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE_DONE);
  }

} /* TcpIp_Tls12ClientMsg_ServerKeyExchangeDetermineNextHsState() */

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_ParseServerCertificateRequest
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerCertificateRequest(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx =
    TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertificateRequestIdx =
    TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the CertificateRequest message is received and forward it to the parser */
  if (serverCertificateRequestIdx != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT)
  {
    Std_ReturnType retVal;

    TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, FALSE);

    /* Forward message to parser */
    retVal = TcpIp_Tls12ClientMsg_ExtractCertificateRequestInformation(TlsConIdx);

    if (retVal == E_OK)
    {
      /* #20 Extraction was successful. Change the state to TCPIP_TLS_STATE_WAIT_FOR_SERVER_HELLO_DONE */
      TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_HELLO_DONE);
      TcpIp_DecHsRxIndicationCntOfTlsConnectionDyn(TlsConIdx);
    }
    else
    {
      /* #30 Extraction failed. Cancel the handshake. */
      TcpIp_TlsHsStm_HandlesUnexpectedHsInformation(TlsConIdx);
    }
  }
  else
  {
    /* #40 otherwise, we received an unexpected message, trigger the alert */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
  }
} /* TcpIp_Tls12ClientHsStm_ParseServerCertificateRequest() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ExtractCertificateRequestInformation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractCertificateRequestInformation(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertRequestStartIdx =
    TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_ServerCertificateRequestLenOfTlsClientBufferMgmtType serverCertRequestLen =
    TcpIp_GetServerCertificateRequestLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertRequestEndIdx
    = (serverCertRequestStartIdx  + TCPIP_TLS_HSHDRLEN + serverCertRequestLen);

  Std_ReturnType retVal = E_NOT_OK;

  /* minimum expected length contained in certificate request message */
  CONST(TcpIp_TlsBufferRxIterType, AUTOMATIC) certReqMinimalLength =
    (TCPIP_TLS_CERTREQUEST_CERTTYPESLENGTH_LEN + TCPIP_TLS_SIGNATURE_ALGORITHMS_LENGTH_LEN
      + TCPIP_TLS_DIST_NAMES_LENGTH_LEN);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the minimum expected length of the received certificate request message and extract the information */
  if (TCPIP_TLS_RXBUFFER_ISIDXINRANGE(serverCertRequestStartIdx,
        certReqMinimalLength, serverCertRequestEndIdx))
  {
    /* Extract certificate types information */
    retVal = TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetCertTypes(TlsConIdx);

    /* Extract signature and hash algorithm information */
    retVal |= TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetSignatureHashAlgorithms(TlsConIdx);

    /* Extract distinguished names information */
    retVal |= TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetDistinguishedNames(TlsConIdx);

    /* Check that all data is consumed */
    retVal |= TcpIp_TlsMsg_CheckMsgForRemainingData(TlsConIdx, serverCertRequestEndIdx);
  }
  /* else - alert is triggered in outer function */

  return retVal;
} /* TcpIp_Tls12ClientMsg_ExtractCertificateRequestInformation() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetCertTypes
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetCertTypes(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxAdHsReadIdxOfTlsBufferMgmtDynType certTypesStartTlsBufferRxIdx
    = TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertRequestStartIdx =
    TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_ServerCertificateRequestLenOfTlsClientBufferMgmtType serverCertRequestLen =
    TcpIp_GetServerCertificateRequestLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertRequestEndIdx
    = (serverCertRequestStartIdx + TCPIP_TLS_HSHDRLEN + serverCertRequestLen);

  Std_ReturnType retVal = E_NOT_OK;
  /* ----- Implementation ----------------------------------------------- */
  /* Format RFC5246 - 7.4.4.  Certificate Request */
  /*   ClientCertificateType certificate_types<1..2^8-1>;
   *
   *   +---> certTypesStartTlsBufferRxIdx
   *  /
   * [Certificate Types Count]   [CertType_0]   [CertType_1] ...
   *      ^                      \_____________________________/
   *      |                                      |
   *      +--> receivedDistinguishedNamesListLength <------+
   */

  /* #10 Get the length of the received certificate types list */
  uint8 receivedCertTypeListLength = IpBase_GetUint8(TcpIp_GetAddrTlsBufferRx(certTypesStartTlsBufferRxIdx), 0);
  TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_CERTREQUEST_CERTTYPESLENGTH_LEN);

  /* #20 Check if the received certificate types list is plausible */
  if (TCPIP_TLS_RXBUFFER_ISIDXINRANGE(certTypesStartTlsBufferRxIdx, receivedCertTypeListLength, serverCertRequestEndIdx))
  {
    /* Increase the tlsBUfferMgmtIdx about the length of the certificate type list */
    TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, receivedCertTypeListLength);

    /* #30 Check if the message contains at least one certificate type */
    if (receivedCertTypeListLength >= 1u)
    {
      /* Ignore the content of the cert types list */
      TcpIp_SetTlsBufferRxServerCertificateRequestCertTypesIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx, certTypesStartTlsBufferRxIdx);

      retVal = E_OK;
    }
  }
  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetCertTypes() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetSignatureHashAlgorithms
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetSignatureHashAlgorithms(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxAdHsReadIdxOfTlsBufferMgmtDynType supportedSignAlgoStartTlsBufferRxIdx
    = TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertRequestStartIdx =
    TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_ServerCertificateRequestLenOfTlsClientBufferMgmtType serverCertRequestLen =
    TcpIp_GetServerCertificateRequestLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertRequestEndIdx
    = (serverCertRequestStartIdx + TCPIP_TLS_HSHDRLEN + serverCertRequestLen);

  Std_ReturnType retVal = E_NOT_OK;
  /* ----- Implementation ----------------------------------------------- */
  /* Format RFC5246 - 7.4.4.  Certificate Request
   *  SignatureAndHashAlgorithm supported_signature_algorithms<2^16-1>;
   *
   *   +---> supportedSignAlgoStartTlsBufferRxIdx                +--> 2 Bytes (Hash - Signatures)
   *  /                                                  /
   * [Signature Hash Algorithms Length] [Algorithm_0] [Algorithm_1] ...
   *      ^                             \_____________________________/
   *      |                                               |
   *      +--> receivedDistinguishedNamesListLength <------+
   */

  /* Get the length of the received certificate types list */
  uint16 receivedSignatureHashAlgoListLength = IpBase_GetUint16(
    TcpIp_GetAddrTlsBufferRx(supportedSignAlgoStartTlsBufferRxIdx), 0);

  TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx,
    TCPIP_TLS_SIGNATURE_ALGORITHMS_LENGTH_LEN);

  /* #10 Check if the received signature hash algorithms list is plausible */
  if (TCPIP_TLS_RXBUFFER_ISIDXINRANGE(supportedSignAlgoStartTlsBufferRxIdx,
        receivedSignatureHashAlgoListLength, serverCertRequestEndIdx))
  {
    /* Increase the tlsBufferMgmtIdx about the length of the signature and hash algorithm list */
    TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, receivedSignatureHashAlgoListLength);

    /* #20 Check if the message contains at least one signature hash algorithm */
    if (   /* List must include at least one signature and hash algorithm pair */
           (receivedSignatureHashAlgoListLength >= TCPIP_TLS_SIGNATURE_ALGORITHMS_ALG_ID_LEN)
           /* and the length information is sent pairwise, so the length must be even */
        && ((receivedSignatureHashAlgoListLength % TCPIP_TLS_SIGNATURE_ALGORITHMS_ALG_ID_LEN) == 0u))
    {
      /* #30 List format is OK, so store the information */
      TcpIp_TlsClientAuthenticationMgmtIdxOfTlsConnectionType tlsClientAuthenticationMgmtIdx
        = TcpIp_GetTlsClientAuthenticationMgmtIdxOfTlsConnection(TlsConIdx);

      TcpIp_TlsBufferRxAdHsReadIdxOfTlsBufferMgmtDynType signAlglistStartTlsBufferRxIdx = supportedSignAlgoStartTlsBufferRxIdx
        + TCPIP_TLS_SIGNATURE_ALGORITHMS_LENGTH_LEN;

      TCPIP_P2V(uint16) sigAlgBufferPtr = TcpIp_GetAddrTlsSigAlgServerCertReqMsgBuffer(
        TcpIp_GetTlsSigAlgServerCertReqMsgBufferStartIdxOfTlsConnection(TlsConIdx));

      /* Check if received list of signature algorithms is not bigger than
       * the configured size of the buffer (* 2u, due to uint16 buffer) */
      if (receivedSignatureHashAlgoListLength <= ((uint16)TcpIp_GetSizeOfTlsSigAlgServerCertReqMsgBuffer() * 2u))
      {
        /* Copy the signature algorithms list from the RxBuffer to the TemporaryBuffer used to ensure correct alignment */
        VStdLib_MemCpy((TCPIP_P2V(uint8))sigAlgBufferPtr, TcpIp_GetAddrTlsBufferRx(signAlglistStartTlsBufferRxIdx),
          receivedSignatureHashAlgoListLength);

        TcpIp_SetTlsBufferRxServerCertificateRequestSupportedSigAlgosIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
          supportedSignAlgoStartTlsBufferRxIdx);

        /* Update the info pointer if client authentication is enabled for the connection */
        if (TcpIp_IsTlsUseClientAuthenticationRequestOfTlsConnection(TlsConIdx))
        {
          TCPIP_P2V(TcpIp_TlsCertRequestInfoType) certRequestInfoPtr =
            TcpIp_GetAddrRequestInfoOfTlsClientAuthenticationMgmt(tlsClientAuthenticationMgmtIdx);

          /* Fill request info sign algorithms into the struct. */
          certRequestInfoPtr->SignatureAndHashAlgoListPtr = (TCPIP_P2C(uint16))sigAlgBufferPtr;                         /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SignatureAlgorithm_Type */
          certRequestInfoPtr->SignatureAndHashAlgoNum =
            (receivedSignatureHashAlgoListLength / TCPIP_TLS_SIGNATURE_ALGORITHMS_ALG_ID_LEN);
        }
        retVal = E_OK;
      }
    }
  }
  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetSignatureHashAlgorithms() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetDistinguishedNames
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetDistinguishedNames(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxAdHsReadIdxOfTlsBufferMgmtDynType certAuthStartTlsBufferRxIdx
    = TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertRequestStartIdx =
    TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_ServerCertificateRequestLenOfTlsClientBufferMgmtType serverCertRequestLen =
    TcpIp_GetServerCertificateRequestLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertRequestEndIdx
    = (serverCertRequestStartIdx + TCPIP_TLS_HSHDRLEN + serverCertRequestLen);

  Std_ReturnType retVal = E_NOT_OK;
  /* ----- Implementation ----------------------------------------------- */
  /* Format RFC5246 - 7.4.4.  Certificate Request
   *   opaque DistinguishedName<1..2^16-1>;
   *   DistinguishedName certificate_authorities<0..2^16-1>;
   *
   *   +---> certAuthStartTlsBufferRxIdx
   *  /
   * [Distinguished Names Length] [DER certificate Authority_1] [DER certificate Authority_2]...
   *      ^                       \________(optional)__________________________________________/
   *      |                                               |
   *      +--> receivedDistinguishedNamesListLength <-----+
   *                               ^
   *                               |
   *                               dnListPtr

   Format of the DN list:
    0x00, 0xF0,      total length of all DNs (2 bytes length field)
    0x00, 0x76,      length of DN_0          (2 bytes length field)
    0x30, 0x74, ...  <DER encoded DN_0>
    0x00, 0x76,      length of DN_1          (2 bytes length field)
    0x30, 0x74, ...  <DER encoded DN_1>
    ...
  */

  /* Get the length of the received distinguished names list */
  uint16 receivedDistinguishedNamesListLength = IpBase_GetUint16(
    TcpIp_GetAddrTlsBufferRx(certAuthStartTlsBufferRxIdx), 0u);

  TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx,
    TCPIP_TLS_DIST_NAMES_LENGTH_LEN);

  /* #10 Check if the received distinguished names list is plausible. */
  if (TCPIP_TLS_RXBUFFER_ISIDXINRANGE(certAuthStartTlsBufferRxIdx,
        receivedDistinguishedNamesListLength, serverCertRequestEndIdx))
  {
    TCPIP_P2C(uint8) dnListPtr = TcpIp_GetAddrTlsBufferRx(certAuthStartTlsBufferRxIdx
                                 + TCPIP_TLS_DIST_NAMES_LENGTH_LEN);
    boolean dnFormatOK = TRUE;

    /* Increase the tlsBufferMgmtIdx about the length of the distinguished names list */
    TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, receivedDistinguishedNamesListLength);

    /* #20 Analyze the distinguished names and update the info pointer only if client authentication is enabled for the connection. */
    if (TcpIp_IsTlsUseClientAuthenticationRequestOfTlsConnection(TlsConIdx))
    {
      if (TcpIp_TlsClient_ServerCertRequestMsgFindDistinguishedNameStarts(TlsConIdx,
        receivedDistinguishedNamesListLength, dnListPtr) != E_OK)
      {
        dnFormatOK = FALSE;
      }
    }

    if (dnFormatOK == TRUE)
    {
      retVal = E_OK;
    }
  }
  return retVal;
} /* TcpIp_Tls12ClientMsg_ServerCertRequestMsgGetDistinguishedNames() */
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
*  TcpIp_Tls12ClientHsStm_ParseServerHelloDone
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerHelloDone(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmtType serverHelloDoneIdx =
    TcpIp_GetTlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
  TcpIp_TlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmtType serverCertificateRequestIdx
    = TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsKeyExchangeMethodDataType keyExchangeMethod = TcpIp_TlsCoreUtil_ReturnActiveCipherSpecificKeyExchangeMethod(TlsConIdx);
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

  Std_ReturnType IntFctRetVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if we received a handshake message with the handshake type ServerHelloDone */
  if (serverHelloDoneIdx != TCPIP_NO_TLSBUFFERRXSERVERHELLODONEIDXOFTLSCLIENTBUFFERMGMT)
  {
    TcpIp_TlsHandshakeState nextState = TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE; /* default, even for PSK use case */

    /* #20 if so, extract the information inside the server hello done message */
    IntFctRetVal = TcpIp_Tls12ClientMsg_ExtractServerHelloDoneInformation(TlsConIdx);

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
    /* #30 ServerHelloDone has been received, notify the certificate chain validation result to user */
    if (   (keyExchangeMethod == TCPIP_TLS_KEYEXCHANGE_METHOD_ECDHE)
        || (keyExchangeMethod == TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH))
    {
      TcpIp_TlsClient_TlsCertChainTriggerValidationCallback(TlsConIdx, TRUE);

      /* #40 Determine next TLS client state, depending on the received messages (ClientCertificate or ClientKeyExchange) */
      if (serverCertificateRequestIdx != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT)
      {
        /* TLS client received certificate request message - Next state is SendClientCertificate */
        nextState = TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE;
      }
      /* else - next state is default state (TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE) */

    }
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

    TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, TRUE);
    /* #50 Check return value from extract function and switch to next state */
    TcpIp_TlsHsStm_ErrorHandlingAndStateTransition(TlsConIdx, IntFctRetVal, nextState);
    TcpIp_DecHsRxIndicationCntOfTlsConnectionDyn(TlsConIdx);
  }
  else
  {
    /* #60 otherwise, we receive an unexpected message, trigger the alert */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
  }
} /* TcpIp_Tls12ClientHsStm_ParseServerHelloDone */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ExtractServerHelloDoneInformation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractServerHelloDoneInformation(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmtType serverHelloDoneStartIdx =
    TcpIp_GetTlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  /* Length of the received Server Hello done message */
  TcpIp_ServerHelloDoneLenOfTlsClientBufferMgmtType serverHelloDoneLen =
    TcpIp_GetServerHelloDoneLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);
  /* End index of the received Server Hello done message - Need for in range checks inside the parser */
  CONST(TcpIp_TlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmtType, AUTOMATIC) serverHelloDoneEndIdx
    = serverHelloDoneStartIdx + TCPIP_TLS_HSHDRLEN + serverHelloDoneLen;

  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that the expected length of the ServerHelloDone message (0 byte) was received */
  if (serverHelloDoneLen == TCPIP_TLS_SERVERHELLODONE_LEN)
  {
    /* #20 Check if there is some more information in the buffer */
    retVal = TcpIp_TlsMsg_CheckMsgForRemainingData(TlsConIdx, serverHelloDoneEndIdx);
  }
  else
  {
    /* #30 If an invalid length of the ServerHelloDone message was received, trigger an alert */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_DECODE_ERROR);
  }
  return retVal;
} /* TcpIp_Tls12ClientMsg_ExtractServerHelloDoneInformation */

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_SendClientCertificate
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_SendClientCertificate(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 handshakeState = TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Prepare sending of the client cert message, depending on the handshake state. */
  switch (handshakeState)
  {
    case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE:
    {
      /* Check if client authentication is enabled for the connection */
      if (TcpIp_IsTlsUseClientAuthenticationRequestOfTlsConnection(TlsConIdx))
      {
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_ISSUE_CALLOUT);
        TcpIp_TlsClient_IssueCertRequestCallout(TlsConIdx);
      }
      else
      {
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_PREPARE_EMPTY_MESSAGE);
        TcpIp_Tls12ClientHsStm_FillClientCertificateMessage(TlsConIdx, FALSE);
      }
      break;
    }
    case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_ISSUE_CALLOUT:
    {
      TcpIp_TlsClient_IssueCertRequestCallout(TlsConIdx);
      break;
    }
    case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_PREPARE_FULL_MESSAGE:
    {
      TcpIp_Tls12ClientHsStm_FillClientCertificateMessage(TlsConIdx, TRUE);
      break;
    }
    case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_PREPARE_EMPTY_MESSAGE:
    {
      TcpIp_Tls12ClientHsStm_FillClientCertificateMessage(TlsConIdx, FALSE);
      break;
    }
    default:
    {
      /* All allowed states are handled in the cases above. Do nothing if called in wrong state. */
      break;
    }
  }
}

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_SendCertificateVerify
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_SendCertificateVerify(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, FALSE);

  /* #10 Build the CertificateVerify message and fill the relevant information */
  TcpIp_Tls12ClientMsg_PrepareCertificateVerify(TlsConIdx);

  if (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_DONE)
  {
    /* If the preparation of the CertificateVerify message is done, set the next state to TCPIP_TLS_STATE_SEND_CLIENT_CCS */
    TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_CCS);
  }
}

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_PrepareCertificateVerify
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_PrepareCertificateVerify(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx
    = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsClientBufferMgmtIdxOfTlsConnectionType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsClientConnectionIterType tlsClientConnectionIdx =
    TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsHandshakeActiveIdxOfTlsCipherWorkerDynType activeTlsHandshakeIdx =
    TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));

  uint16 clientCertVerifyLen = 0u;
  uint16 writableLength = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare the Certificate Verify message by adding all relevant message information */
  while ((TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_DONE)  /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
    && (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_CTX_ASYNC_PENDING))
  {
    switch (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx))
    {
      case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY:
      case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_ADD_HDR:
      {
        /* Set the start index of this message to the current write index */
        /* TlsBufferTxAdHsWriteIdx points to the next free index in the TLS TxBuffer. */
        TcpIp_SetTlsBufferTxClientCertificateVerifyIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
          TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

        /* #20 Add the generic handshake header */
        TcpIp_TlsMsg_AddHsHdr(TlsConIdx, TCPIP_TLS_HSTYPE_CERTIFICATEVERIFY);

        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx,
          TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_ADD_SIG_ALGORITHM);
        break;
      } /* TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_ADD_HDR */

      case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_ADD_SIG_ALGORITHM:
      {
        /* #30 Add the signature and hash algorithm to the TxBuffer */
        IpBase_PutUint16(TcpIp_GetAddrTlsBufferTx(TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)),
          0u, ((uint16)TcpIp_GetSignatureAlgorithmIdOfTlsHandshake(activeTlsHandshakeIdx)));
        TcpIp_AddTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_CERTVERIFY_SIGALGO_LEN);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx,
          TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_CALC_SIGNATURE);
        break;
      } /* TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_ADD_SIG_ALGORITHM */

      case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_CALC_SIGNATURE:
      {
        /* #40 Trigger the calculation of the signature in the async context handler */
        TcpIp_SetCalcClientCertVerifySignatureAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx, TRUE);

        /* Set the state to ASYNC_PENDING */
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_ASYNC_PENDING);

        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_ENCODE_SIGNATURE);
        break;
      } /* TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_CALC_SIGNATURE */

      case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_ENCODE_SIGNATURE:
      {
        /* Save the current write index to mark the position where the signature length should be placed. */
        TcpIp_SetTlsBufferTxClientCertVerifySignatureLenIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
          TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

        /* Increment the write index by the consumed length (2 Byte) */
        TcpIp_AddTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_CERTVERIFY_SIG_LENGTH_LEN);

        /* Encode the calculated signature and add it to the message */
        TcpIp_TlsClientMsg_CertVerifyEncodeSignature(TlsConIdx);

        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_TRIGGER_TX);
        break;
      } /* TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_ENCODE_SIGNATURE */

      case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_TRIGGER_TX:
      {
        /* #50 Write the length information into the HS Header */
        /* (Message length = WriteIdx - ClientCertificateVerifyStartIdx) */
        clientCertVerifyLen = (uint16)(TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)
          - TcpIp_GetTlsBufferTxClientCertificateVerifyIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
        TcpIp_SetClientCertificateVerifyLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx, clientCertVerifyLen);

        writableLength = clientCertVerifyLen - TCPIP_TLS_HSHDRLEN;
        TcpIp_TlsMsg_AddLenToHsHdr(
          TcpIp_GetTlsBufferTxClientCertificateVerifyIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx),
          (uint32)writableLength);

        /* ClientHello message is now ready to send, set the corresponding trigger */
        TcpIp_SetCertificateVerifyReadyToSendOfTlsClientConnection(tlsClientConnectionIdx, TRUE);
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_NO_ASYNC);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY_DONE);
        TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, TRUE);
        break;
      }

      default:
        /* Default case left blank intentionally */
        break;
    } /* switch() */
  } /* while() */
} /* TcpIp_Tls12ClientMsg_PrepareCertificateVerify() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCrypto_CalcCertVerifySignature
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCrypto_CalcCertVerifySignature(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsHandshakeActiveIdxOfTlsCipherWorkerDynType activeTlsHandshakeIdx
    = TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));

  TcpIp_CsmJobIdType signatureGenerateJobId
    = TcpIp_GetCsmJobId(TcpIp_GetCsmJobIdKeyExchangeSignatureGenerateIdxOfTlsHandshake(activeTlsHandshakeIdx));

  TcpIp_TlsTemporarySignatureBufferStartIdxOfTlsConnectionType startTlsTemporarySignatureBufferIdx
    = TcpIp_GetTlsTemporarySignatureBufferStartIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsTemporarySignatureBufferEndIdxOfTlsConnectionType tmpSignatureBufferEndIdx
    = TcpIp_GetTlsTemporarySignatureBufferEndIdxOfTlsConnection(TlsConIdx);

    /* Destination buffer for the CSM operation */
  TCPIP_P2V(uint8) generatedSignatureDestPtr = TcpIp_GetAddrTlsTemporarySignatureBuffer(startTlsTemporarySignatureBufferIdx);
  uint32 generatedSignatureDestLen = (((uint32)tmpSignatureBufferEndIdx - startTlsTemporarySignatureBufferIdx));

  Std_ReturnType csmRetVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update signature with ClientHello message (TxBuffer) */
  {
    TCPIP_P2C(uint8) clientHelloPtr = TcpIp_GetAddrTlsBufferTx(
      TcpIp_GetTlsBufferTxClientHelloIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));

    uint32 clientHelloLen = TcpIp_GetClientHelloLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

    /* Call the CSM with the operation mode CRYPTO_OPERATIONMODE_STREAMSTART to start the signature generation */
    csmRetVal = TCPIP_CSM_SignatureGenerate(signatureGenerateJobId, CRYPTO_OPERATIONMODE_STREAMSTART,
      clientHelloPtr, clientHelloLen, generatedSignatureDestPtr, &generatedSignatureDestLen);
  }

  /* #20 Update signature with ServerHello message (RxBuffer) */
  {
    TCPIP_P2C(uint8) rxServerHelloPtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerHelloIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 rxServerHelloLen
      = ((uint32)TcpIp_GetServerHelloLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    /* Call the CSM with the operation mode CRYPTO_OPERATIONMODE_UPDATE to add the next message */
    csmRetVal |= TCPIP_CSM_SignatureGenerate(signatureGenerateJobId, CRYPTO_OPERATIONMODE_UPDATE,
      rxServerHelloPtr, rxServerHelloLen, generatedSignatureDestPtr, &generatedSignatureDestLen);
  }

  /* #30 Update signature with ServerCertificates message (RxBuffer) */
  {
    TCPIP_P2C(uint8) rxServerCertificatePtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerCertificatesIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 rxServerCertificateLen
      = ((uint32)TcpIp_GetServerCertificatesLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    /* Call the CSM with the operation mode CRYPTO_OPERATIONMODE_UPDATE to add the next message */
    csmRetVal |= TCPIP_CSM_SignatureGenerate(signatureGenerateJobId, CRYPTO_OPERATIONMODE_UPDATE,
      rxServerCertificatePtr, rxServerCertificateLen, generatedSignatureDestPtr, &generatedSignatureDestLen);
  }

  /* #40 Update signature with ServerCertificateStatus message (RxBuffer) - if received */
  if (TcpIp_GetTlsBufferRxServerCertificateStatusIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)
    != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATESTATUSIDXOFTLSCLIENTBUFFERMGMT)
  {

    TCPIP_P2C(uint8) rxServerCertificateStatusPtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerCertificateStatusIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 rxServerCertificateStatusLen
      = ((uint32)TcpIp_GetServerCertificateStatusLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    /* Call the CSM with the operation mode CRYPTO_OPERATIONMODE_UPDATE to add the next message */
    csmRetVal |= TCPIP_CSM_SignatureGenerate(signatureGenerateJobId, CRYPTO_OPERATIONMODE_UPDATE,
      rxServerCertificateStatusPtr, rxServerCertificateStatusLen, generatedSignatureDestPtr, &generatedSignatureDestLen);
  }

  /* #50 Update signature with ServerKeyExchange message (RxBuffer) - if received */
  if (TcpIp_GetTlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)
    != TCPIP_NO_TLSBUFFERRXSERVERKEYEXCHANGEIDXOFTLSCLIENTBUFFERMGMT)
  {
    TCPIP_P2C(uint8) rxServerKeyExchangePtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 rxServerKeyExchangeLen
      = ((uint32)TcpIp_GetServerKeyExchangeLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    /* Call the CSM with the operation mode CRYPTO_OPERATIONMODE_UPDATE to add the next message */
    csmRetVal |= TCPIP_CSM_SignatureGenerate(signatureGenerateJobId, CRYPTO_OPERATIONMODE_UPDATE,
      rxServerKeyExchangePtr, rxServerKeyExchangeLen, generatedSignatureDestPtr, &generatedSignatureDestLen);
  }

  /* #60 Update signature with ServerCertificateRequest message (RxBuffer) - if received */
  if (TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)
    != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT)
  {

    TCPIP_P2C(uint8) rxServerCertificateRequestPtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 rxServerCertificateRequestLen
      = ((uint32)TcpIp_GetServerCertificateRequestLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    /* Call the CSM with the operation mode CRYPTO_OPERATIONMODE_UPDATE to add the next message */
    csmRetVal |= TCPIP_CSM_SignatureGenerate(signatureGenerateJobId, CRYPTO_OPERATIONMODE_UPDATE,
      rxServerCertificateRequestPtr, rxServerCertificateRequestLen, generatedSignatureDestPtr, &generatedSignatureDestLen);
  }

  /* #70 Update signature with ServerHelloDone message (RxBuffer) */
  {
    TCPIP_P2C(uint8) rxServerHelloDonePtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 rxServerHelloDoneLen
      = ((uint32)TcpIp_GetServerHelloDoneLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    /* Call the CSM with the operation mode CRYPTO_OPERATIONMODE_UPDATE to add the next message */
    csmRetVal |= TCPIP_CSM_SignatureGenerate(signatureGenerateJobId, CRYPTO_OPERATIONMODE_UPDATE,
      rxServerHelloDonePtr, rxServerHelloDoneLen, generatedSignatureDestPtr, &generatedSignatureDestLen);
  }

  /* #80 Update signature with Client Certificate message (TxBuffer) */
  {
    TCPIP_P2C(uint8) txClientCertificatePtr = TcpIp_GetAddrTlsBufferTx(
      TcpIp_GetTlsBufferTxClientCertificateIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 txClientCertificateLen
      = ((uint32)TcpIp_GetClientCertificateLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));

    /* Call the CSM with the operation mode CRYPTO_OPERATIONMODE_UPDATE to add the next message */
    csmRetVal |= TCPIP_CSM_SignatureGenerate(signatureGenerateJobId, CRYPTO_OPERATIONMODE_UPDATE,
      txClientCertificatePtr, txClientCertificateLen, generatedSignatureDestPtr, &generatedSignatureDestLen);
  }

  /* #90 Update signature with ClientKeyExchange message (TxBuffer) */
  {
    TCPIP_P2C(uint8) txClientKeyExchangePtr = TcpIp_GetAddrTlsBufferTx(
      TcpIp_GetTlsBufferTxClientKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 txClientKeyExchangeLen
      = ((uint32)TcpIp_GetClientKeyExchangeLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));

    /* Call the CSM with the operation mode  CRYPTO_OPERATIONMODE_UPDATE | CRYPTO_OPERATIONMODE_FINISH to add the last message
    *  and finish the generation of the signature.
    */
    csmRetVal |= TCPIP_CSM_SignatureGenerate(signatureGenerateJobId, CRYPTO_OPERATIONMODE_UPDATE | CRYPTO_OPERATIONMODE_FINISH,
      txClientKeyExchangePtr, txClientKeyExchangeLen, generatedSignatureDestPtr, &generatedSignatureDestLen);
  }

  if (csmRetVal == E_OK)
  {
    /* #100 The operation was successful. Set the async flag to DONE */
    TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);
  }
  else
  {
    /* #110 Generation of signature failed - handle error during processing */
    TcpIp_TlsAsyncCtx_HandleErrorDuringAsyncJobProcessing(TlsConIdx,
      TCPIP_TLS_FCTID_CLIENT_ASYNCCTX_CALC_CERTVERIFY_SIGN, TCPIP_TLS_ERRORID_CRYPTO_CSM_CALL_FAILED);
  }
} /* TcpIp_Tls12ClientCrypto_CalcCertVerifySignature() */

#   endif
#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_FillClientCertificateMessage
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_FillClientCertificateMessage(
  TcpIp_TlsConnectionIterType TlsConIdx,
  boolean CertAvailable)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, FALSE);

  /* #10 Prepare the ClientCertificate message */
  retVal = TcpIp_Tls12ClientMsg_PrepareClientCertificate(TlsConIdx, CertAvailable);

  /* #20 Check the result of the preparation of the ClientCertificate message */
  if (retVal == E_OK)
  {
    /* #30 Preparation was successful. Change the state to TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE */
    TcpIp_TlsClientConnectionIterType tlsClientConnectionIdx = TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx);

    TcpIp_SetClientCertificateReadyToSendOfTlsClientConnection(tlsClientConnectionIdx, TRUE);
    TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE);
    TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, TRUE);
  }
  else
  {
    /* #40 Preparation failed. Cancel the handshake. */
    TcpIp_TlsClientHsStm_HandleErrorDuringPreparation(TlsConIdx);
  }
} /* TcpIp_Tls12ClientHsStm_SendClientCertificate() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_PrepareClientCertificate
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_PrepareClientCertificate(
  TcpIp_TlsConnectionIterType TlsConIdx,
  boolean CertAvailable)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsClientBufferMgmtIdxOfTlsConnectionType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  uint16 clientCertificateLen = 0u;
  uint16 writableLength = 0u;

  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare the ClientCertificate message by adding all relevant message information */
  TcpIp_SetTlsBufferTxClientCertificateIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
    TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

  /* Add generic handshake header */
  TcpIp_TlsMsg_AddHsHdr(TlsConIdx, TCPIP_TLS_HSTYPE_CERTIFICATE);

  /* Add the certificates, if configured */
  if (CertAvailable == TRUE)
  {
    retVal = TcpIp_Tls12ClientMsg_ClientCertificateAddCertificates(TlsConIdx);
  }
  else
  {
    TcpIp_Tls12ClientMsg_ClientCertificateAddNoCertificate(TlsConIdx);
  }

  if (retVal == E_OK)
  {
    /* #20 Write the length information into the HS Header (Message length = WriteIdx - ServerHelloStartIdx). */
    clientCertificateLen = (uint16)(TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)
      - TcpIp_GetTlsBufferTxClientCertificateIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));

    TcpIp_SetClientCertificateLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx, clientCertificateLen);

    writableLength = clientCertificateLen - TCPIP_TLS_HSHDRLEN;
    TcpIp_TlsMsg_AddLenToHsHdr(TcpIp_GetTlsBufferTxClientCertificateIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx), (uint32)writableLength);
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_PrepareClientCertificate() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientCertificateAddNoCertificate
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientCertificateAddNoCertificate(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDynType certStartTlsBufferTxIdx
    = TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

  const uint32 zeroCertificateLength = 0u;
  /* ----- Implementation ----------------------------------------------- */
  /* RFC5246 - 7.4.6 Client Certificate
   *   If no suitable certificate is available, the client MUST send a
   *   certificate message containing no certificates.
   *   That is, the certificate_list structure has a length of zero.
   */

   /* #10 No client certificates are available or configured for this connection. Add zero length according to RFC to the certificate message. */
  IpBase_PutUint24(TcpIp_GetAddrTlsBufferTx(certStartTlsBufferTxIdx), 0, zeroCertificateLength);

  TcpIp_AddTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_CERTCHAINLEN_LEN);
}

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientCertificateAddCertificates
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientCertificateAddCertificates(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDynType certTotalLengthStartTlsBufferTxIdx
    = TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);  /* start of the overall length field */

  TcpIp_TlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDynType certLengthStartTlsBufferTxIdx= 0u;  /* start of the length field in front of each certificate */
  TcpIp_TlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDynType certContentStartTlsBufferTxIdx = 0u;  /* start of the content of each certificate */

  uint32 certificatesLength = 0u;  /* length of all certificates including their length fields */
  TcpIp_TlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDynType certificateLength;  /* length of the current certificate */
  uint8_least certIter;  /* not initialized, only used as for loop iterator */

  TcpIp_TlsBufferTxAdHsEndIdxOfTlsBufferMgmtType tlsBufferTxAdHsEndIdx
    = TcpIp_GetTlsBufferTxAdHsEndIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);

  Std_ReturnType retVal = E_OK;

  TcpIp_TlsClientAuthenticationMgmtIdxOfTlsConnectionType tlsClientAuthenticationMgmtIdx
    = TcpIp_GetTlsClientAuthenticationMgmtIdxOfTlsConnection(TlsConIdx);

  TCPIP_P2C(TcpIp_TlsClientCertInfoType) certInfoPtr = TcpIp_GetAddrClientCertInfoOfTlsClientAuthenticationMgmt(tlsClientAuthenticationMgmtIdx);
  uint8 certIdListLen = certInfoPtr->CertIdListLen;

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Set start indexes of the fist certificate. */
  certLengthStartTlsBufferTxIdx = certTotalLengthStartTlsBufferTxIdx + TCPIP_TLS_CERTCHAINLEN_LEN;
  certContentStartTlsBufferTxIdx = certLengthStartTlsBufferTxIdx + TCPIP_TLS_CERTLEN_LEN;

  /* #20 Skip the overall length field, this will be set later. */
  TcpIp_AddTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_CERTCHAINLEN_LEN);

  /* #30 Write all certs into the cert message, including their lengths. */
  for (certIter = 0u; certIter < certIdListLen; certIter++)
  {
    KeyM_CertificateIdType certId = certInfoPtr->CertIdList[certIter];
    KeyM_CertDataType certificateData;  /* not initialized, always set before used */

    /* calc max cert length */
    certificateLength = (tlsBufferTxAdHsEndIdx - certContentStartTlsBufferTxIdx);

    /* get cert and cert length from KeyM */
    certificateData.certData = TcpIp_GetAddrTlsBufferTx(certContentStartTlsBufferTxIdx);
    certificateData.certDataLength = (uint32)certificateLength;  /* remaining length in the TxBuffer */

    if (KeyM_GetCertificate(certId, &certificateData) == E_OK)
    {
      /* update cert length field (length of one cert and its length field) */
      uint32 certAndLenLength = certificateData.certDataLength + TCPIP_TLS_CERTLEN_LEN;

      IpBase_PutUint24(TcpIp_GetAddrTlsBufferTx(certLengthStartTlsBufferTxIdx), 0, certificateData.certDataLength);
      certificatesLength += certAndLenLength;

      /* prepare start indexes for next cert */
      certLengthStartTlsBufferTxIdx  += (TcpIp_TlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDynType)certAndLenLength;
      certContentStartTlsBufferTxIdx += (TcpIp_TlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDynType)certAndLenLength;
      TcpIp_AddTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx,
        (TcpIp_TlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDynType)certAndLenLength);
    }
    else
    {
      /* error, cancel handshake */
      TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_INT_ERROR);
      retVal = E_NOT_OK;
      break;
    }
  }

  /* #40 Write the total length of all certificates. */
  IpBase_PutUint24(TcpIp_GetAddrTlsBufferTx(certTotalLengthStartTlsBufferTxIdx), 0, certificatesLength);

  return retVal;
}

#   endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_SendClientKeyExchange
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_SendClientKeyExchange(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, FALSE);

  /* #10 Prepare the ClientKeyExchange message */
  TcpIp_Tls12ClientMsg_PrepareClientKeyExchangeAndTriggerSecretCalculation(TlsConIdx);

  /* #20 Check the state of the preparation of the ClientKeyExchange message */
  if (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CTX_NO_ASYNC)
  {
    /* #30 Preparation was successfully performed. */
    TcpIp_TlsClientConnectionIterType tlsClientConnectionIdx
      = TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx);

    TcpIp_SetKeyExchangeReadyToSendOfTlsClientConnection(tlsClientConnectionIdx, TRUE);

    /* #40 Set the next state. Either send a Certificate Verify message or a CCS message. */
    if (TcpIp_IsSendCertificateVerifyOfTlsClientConnection(tlsClientConnectionIdx) == TRUE)
    {
      TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY);
    }
    else
    {
      TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_CCS);
    }

    /* Next state is a transmitting state so trigger the processing */
    TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, TRUE);
  }
  else if (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CTX_ASYNC_PENDING)
  {
    /* #50 Preparation not done yet, an async operation is pending. Nothing to do, just leave the function. */
  }
  else
  {
    /* #60 Preparation failed. Cancel the handshake. */
    TcpIp_TlsClientHsStm_HandleErrorDuringPreparation(TlsConIdx);
  }
} /* TcpIp_Tls12ClientHsStm_SendClientKeyExchange() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_PrepareClientKeyExchangeAndTriggerSecretCalculation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCYC_6030, MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_PrepareClientKeyExchangeAndTriggerSecretCalculation(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx
    = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare the ClientKeyExchange message until we are done AND no Async context needs to be computed */
  while (   (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_DONE)     /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
             /* Async operation is triggered in one of the sub states, switch to lowPrio context - leave this function */
         && (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_CTX_ASYNC_PENDING))
  {
    switch (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx))
    {
      case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE:
      {
        TcpIp_TlsKeyExchangeMethodDataType  keyExchangeMethod
          = TcpIp_TlsCoreUtil_ReturnActiveCipherSpecificKeyExchangeMethod(TlsConIdx);

        /* Set the start index of this message to the current write index */
        TcpIp_SetTlsBufferTxClientKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
          TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

        /* #20 Add the generic TLS-Handshake header */
        TcpIp_TlsMsg_AddHsHdr(TlsConIdx, TCPIP_TLS_HSTYPE_CLIENTKEYEXCHANGE);

        /* #30 Select the next state depending on the key exchange method  */
        switch (keyExchangeMethod)
        {
#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
          case TCPIP_TLS_KEYEXCHANGE_METHOD_ECDHE:
          case TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH:
          {
            /* in case of ECDHE or ECDH we have to add the DH parameter */
            TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_ADD_DH);
            break;
          }
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */
          case TCPIP_TLS_KEYEXCHANGE_METHOD_PSK_PLAIN:
          {
            /* in case of plain PSK we have to add the PSK Identity */
            TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_ADD_PSK_ID);
            break;
          }
          default:
          {
            /* Invalid key exchange method. This can never happen, a valid key exchange method has been chosen with the
            active cipher suite received in the server_hello message. */
            break;
          }
        }  /* switch (keyExchangeMethod) */

        break;
      } /* case  TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_ADD_HDR */

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
      case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_ADD_DH:
      {
        /* #40 Add the Diffie-Hellman parameter and key */
        TcpIp_TlsHandshakeActiveIdxOfTlsCipherWorkerDynType activeTlsHandshakeIdx =
          TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));
        TcpIp_TlsClientMsg_AddDiffieHellmanParams(TlsConIdx, TCPIP_TLS_HSTYPE_CLIENTKEYEXCHANGE, TcpIp_GetEllipticCurveIdOfTlsHandshake(activeTlsHandshakeIdx));
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_ADD_MSG_LENGTH);
        break;
      } /* case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_ADD_DH */
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

      case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_ADD_PSK_ID:
      {
        /* #50 Add the PSK parameter and identity */
        TcpIp_Tls12ClientMsg_ClientKeyExchangeAddPskParams(TlsConIdx);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_ADD_MSG_LENGTH);
        break;
      } /* case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_ADD_PSK ID */

      case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_ADD_MSG_LENGTH:
      {
        TcpIp_TlsHandshakeState nextKeyExchangeState = TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_DONE;
        /* #60 Prepare the transmit of the ClientKeyExchange message */
        TcpIp_Tls12ClientMsg_ClientKeyExchangeAddMessageLength(TlsConIdx, &nextKeyExchangeState);
        /* Change State transition depending on key exchange method. */
        /* TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_DH_SHARED_SECRET | TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_PSK_SHARED_SECRET */
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, nextKeyExchangeState);
        break;
      } /* case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_ADD_MSG_LENGTH */

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
      case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_DH_SHARED_SECRET:
      {
        /* #70 Calculate the TLS shared secret */
        TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcSharedSecret(TlsConIdx);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_MASTER_SECRET);
        break;
      } /* case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_DH_SHARED_SECRET */
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

      case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_PSK_SHARED_SECRET:
      {
        /* #80 Calculate the TLS premaster secret for active PSK-Identity */
        TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcPskPremasterSecret(TlsConIdx);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_MASTER_SECRET);
        break;
      } /* case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_PSK_SHARED_SECRET */

      case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_MASTER_SECRET:
      {
        /* #90 Calculate the TLS master secret */
        TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcMasterSecret(TlsConIdx);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_WAIT_FOR_ASYNC);
        break;
      } /* case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_MASTER_SECRET */

      case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_WAIT_FOR_ASYNC:
      {
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_NO_ASYNC);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_DONE);
        break;
      } /* case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_WAIT_FOR_ASYNC */

      default:                                                                                                           /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
      /* Default case left blank intentionally */
      break;

    } /* switch () */
  } /* while () */

} /* TcpIp_Tls12ClientMsg_PrepareClientKeyExchangeAndTriggerSecretCalculation() */
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientKeyExchangeAddPskParams
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeAddPskParams(
  TcpIp_TlsConnectionIterType  TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Select a matching PSK-Identity, depending on received PSK-hint */
  TcpIp_Tls12ClientMsg_ClientKeyExchangeSelectPskIdentity(TlsConIdx);

  /* #20 Add the selected PSK-Identity to the ClientKeyExchange PSK parameter */
  TcpIp_Tls12ClientMsg_ClientKeyExchangAddSelectedPskIdentity(TlsConIdx);

} /* TcpIp_Tls12ClientMsg_ClientKeyExchangeAddPskParams() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientKeyExchangeSelectPskIdentity
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeSelectPskIdentity(
  TcpIp_TlsConnectionIterType  TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if we have received a PSK Hint matching one of the known PSK Hints. */
  /* returns FALSE in case no matching PSK-Hint was received */
  if (TcpIp_Tls12ClientMsg_ClientKeyExchangeCheckPskHints(TlsConIdx) == FALSE)
  {
    /* #20 otherwise, if no PSK-Hint matches, select the default PSK Identity */
    TcpIp_TlsMsg_SelectDefaultPskIdentity(TlsConIdx);
  }
  /* else - matching PSK hint was selected by caller function */

} /* TcpIp_Tls12ClientMsg_ClientKeyExchangeSelectPskIdentity() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientKeyExchangeCheckPskHints
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeCheckPskHints(
  TcpIp_TlsConnectionIterType  TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_ServerKeyExchangePskHintLenOfTlsClientBufferMgmtType receivedPskHintLeng
    = TcpIp_GetServerKeyExchangePskHintLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  boolean pskHintKnown;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if we received a PSK-Hint and compare the actual hint content */
  if (receivedPskHintLeng != 0u)
  {
    pskHintKnown = TcpIp_Tls12ClientMsg_ClientKeyExchangeCompareReceivedPskHint(TlsConIdx);
  }
  else
  {
    /* No PSK-Hint received, return FALSE. Outer function will selected the default PSK-Identity. */
    pskHintKnown = FALSE;
  }

  return pskHintKnown;
} /* TcpIp_Tls12ClientMsg_ClientKeyExchangeCheckPskHints() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientKeyExchangeCompareReceivedPskHint
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeCompareReceivedPskHint(
  TcpIp_TlsConnectionIterType  TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_ServerKeyExchangePskHintLenOfTlsClientBufferMgmtType receivedPskHintLeng
    = TcpIp_GetServerKeyExchangePskHintLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);
  TcpIp_TlsPskIdentityIndStartIdxOfTlsConnectionType pskIdentityIndStartIdx
    = TcpIp_GetTlsPskIdentityIndStartIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsPskIdentityIndEndIdxOfTlsConnectionType pskIdentityIndEndIdx
    = TcpIp_GetTlsPskIdentityIndEndIdxOfTlsConnection(TlsConIdx);

  TCPIP_P2C(uint8) receivedHintPtr
    = TcpIp_GetAddrTlsBufferRx(TcpIp_GetTlsBufferRxServerKeyExchangePskHintIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));

  TcpIp_TlsPskIdentityIndIterType tlsPskIdentityIndIdx;
  boolean foundMatchingPskHint = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all configured PSK-Identities */
  for (tlsPskIdentityIndIdx = pskIdentityIndStartIdx; tlsPskIdentityIndIdx < pskIdentityIndEndIdx; tlsPskIdentityIndIdx++)
  {
    TcpIp_TlsPskIdentityIndType tlsPskIdentityIdx = TcpIp_GetTlsPskIdentityInd(tlsPskIdentityIndIdx);
    TcpIp_PskIdentityHintLengthOfTlsPskIdentityType configuredPskHintLen
      = TcpIp_GetPskIdentityHintLengthOfTlsPskIdentity(tlsPskIdentityIdx);
    /* #20 Check if the received Length match with the configured of the current PSK-Identity */
    if (receivedPskHintLeng == configuredPskHintLen)
    {
      TcpIp_PskIdentityHintStartIdxOfTlsPskIdentityType startPskIdentityHintIdx
        = TcpIp_GetPskIdentityHintStartIdxOfTlsPskIdentity(tlsPskIdentityIdx);
      TCPIP_P2C(uint8) iterPskHintPtr = TcpIp_GetAddrPskIdentityHint(startPskIdentityHintIdx);
      /* #30 Compare the actual PSK-Hint if it matches */
      if (IpBase_StrCmpLen(receivedHintPtr, iterPskHintPtr, receivedPskHintLeng) == E_OK)
      {
        /* #40 Set this PSK-Identity as active one */
        TcpIp_SetTlsPskIdentityActiveIdxOfTlsConnectionDyn(TlsConIdx, tlsPskIdentityIdx);
        foundMatchingPskHint = TRUE;
        break; /* skip all other comparisons */
      }
    }
  }
  return foundMatchingPskHint;
} /* TcpIp_Tls12ClientMsg_ClientKeyExchangeCompareReceivedPskHint() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientKeyExchangAddSelectedPskIdentity
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangAddSelectedPskIdentity(
  TcpIp_TlsConnectionIterType  TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsBufferMgmtIdxOfTlsConnectionType tlsBufferMgmtIdx
    = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsPskIdentityActiveIdxOfTlsConnectionDynType activeTlsPskIdentityIdx
    = TcpIp_GetTlsPskIdentityActiveIdxOfTlsConnectionDyn(TlsConIdx);

  TcpIp_PskIdentityLengthOfTlsPskIdentityType pskIdentityLen;

  /* ----- Implementation ----------------------------------------------- */
  /* Get the length of the PSK-Identity */
  pskIdentityLen = TcpIp_GetPskIdentityLengthOfTlsPskIdentity(activeTlsPskIdentityIdx);

  /* #10 Write the Length of the PSK-Identity into the TLS TxBuffer */
  IpBase_PutUint16(TcpIp_GetAddrTlsBufferTx(
    TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)), 0, pskIdentityLen);

  /* Increment the write index by the consumed length (2 Bytes) */
  TcpIp_AddTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_KEYEXCH_PSKIDLEN);


  /* #20 Write the PSK Identity string into the TLS TxBuffer */
  VStdMemCpy(TcpIp_GetAddrTlsBufferTx(TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)),            /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
    TcpIp_GetAddrPskIdentity(TcpIp_GetPskIdentityStartIdxOfTlsPskIdentity(activeTlsPskIdentityIdx)), pskIdentityLen);

  /* Increment the write index by the consumed length (pskIdentityLen) */
  TcpIp_AddTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, pskIdentityLen);

} /* TcpIp_Tls12ClientMsg_ClientKeyExchangAddSelectedPskIdentity() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientKeyExchangeAddMessageLength
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeAddMessageLength(
  TcpIp_TlsConnectionIterType  TlsConIdx,
  TCPIP_P2V(TcpIp_TlsHandshakeState) NextStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 clientKeyExchangeLen;
  TcpIp_SizeOfTlsBufferMgmtType        tlsBufferMgmtIdx       = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsClientBufferMgmtIterType    tlsClientBufferMgmtIdx = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsKeyExchangeMethodDataType   keyExchangeMethod      = TcpIp_TlsCoreUtil_ReturnActiveCipherSpecificKeyExchangeMethod(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* Calculate the length information: Message length = WriteIdx - ServerCertStartIdx */
  clientKeyExchangeLen = ((uint32)TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)
    - TcpIp_GetTlsBufferTxClientKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
  TcpIp_SetClientKeyExchangeLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx, (uint16)clientKeyExchangeLen);

  /* #10 Write the message length into the HS-Header */
  TcpIp_TlsMsg_AddLenToHsHdr(TcpIp_GetTlsBufferTxClientKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx),
    (clientKeyExchangeLen - TCPIP_TLS_HSHDRLEN));

  /* #20 Change the next handshake state depending on the  key exchange method */
  switch (keyExchangeMethod)
  {
#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
    case TCPIP_TLS_KEYEXCHANGE_METHOD_ECDHE:
    case TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH:
    {
    /* in case of ECDHE or ECDH we have to calculate the DH shared secret */
      *NextStatePtr = TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_DH_SHARED_SECRET;
      break;
    }
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */
    case TCPIP_TLS_KEYEXCHANGE_METHOD_PSK_PLAIN:
    {
      /* in case of plain PSK we have to calculate the PSK shared secret */
      *NextStatePtr = TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE_CALC_PSK_SHARED_SECRET;
      break;
    }
    default:
    {
      /* Invalid key exchange method. This can never happen, a valid key exchange method has been chosen with the active
      cipher suite received in the server_hello message. */
      break;
    }
  }  /* switch (keyExchangeMethod) */

} /* TcpIp_Tls12ClientMsg_ClientKeyExchangeAddMessageLength() */

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcSharedSecret
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcSharedSecret(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger the calculation of the job in the async context handler and set the state to ASYNC_PENDING */
  TcpIp_SetDhSharedSecretAsyncFlagOfTlsConnectionDyn(TlsConIdx, TRUE);

  /* Set the state to ASYNC_PENDING */
  TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_ASYNC_PENDING);

} /* TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcSharedSecret() */
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcPskPremasterSecret
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcPskPremasterSecret(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger the calculation of the job in the async context handler and set the state to ASYNC_PENDING */
  TcpIp_SetPskPremasterSecretAsyncFlagOfTlsConnectionDyn(TlsConIdx, TRUE);

  /* Set the state to ASYNC_PENDING */
  TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_ASYNC_PENDING);

} /* TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcPskPremasterSecret() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcMasterSecret
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcMasterSecret(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger the calculation of the job in the async context handler and set the state to ASYNC_PENDING */
  TcpIp_SetCalcMasterSecretAsyncFlagOfTlsConnectionDyn(TlsConIdx, TRUE);

  /* Set the state to ASYNC_PENDING */
  TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_ASYNC_PENDING);

} /* TcpIp_Tls12ClientMsg_ClientKeyExchangeCalcMasterSecret() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_SendClientCcs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_SendClientCcs(TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientConnectionIterType tlsClientConnectionIdx
    = TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger the transmission of the CCS message and change the TLS handshake state to TCPIP_TLS_STATE_SEND_CLIENT_FINISHED */
  TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, TRUE);
  TcpIp_SetChangeCipherSpecReadyToSendOfTlsClientConnection(tlsClientConnectionIdx, TRUE);
  TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_FINISHED);

} /* TcpIp_Tls12ClientHsStm_SendClientCcs() */

/**********************************************************************************************************************
 *   TLS CLIENT HS TRIGGER SM
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *   TLS CLIENT HANDSHAKE STATE MACHINE
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_PerformNextHsStep
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6030, 6050 1 */ /* MD_MSR_STCYC, MD_TcpIp_TlsStatemachine_STCAL_6050 */
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_PerformNextHsStep(TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Process the TLS-CLient Handshake depending on the current state */
  uint16 hsState = TCPIP_TLS_GET_HS_STATE(TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx));
  switch ((TcpIp_TlsHandshakeState)hsState)
  {
    case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE:
    {
#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
      TcpIp_Tls12ClientHsStm_ParseServerCertificate(TlsConIdx);
#   endif
      break;
    }
    case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_STATUS:
    {
#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
      TcpIp_Tls12ClientHsStm_ParseServerCertificateStatus(TlsConIdx);
#   endif
      break;
    }
    case TCPIP_TLS_STATE_WAIT_FOR_SERVER_KEY_EXCHANGE:
    {
      TcpIp_Tls12ClientHsStm_ParseServerKeyExchange(TlsConIdx);
      break;
    }
    case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CERTIFICATE_REQUEST:
    {
#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
      TcpIp_Tls12ClientHsStm_ParseServerCertificateRequest(TlsConIdx);
#   endif
      break;
    }
    case TCPIP_TLS_STATE_WAIT_FOR_SERVER_HELLO_DONE:
    {
      TcpIp_Tls12ClientHsStm_ParseServerHelloDone(TlsConIdx);
      break;
    }
    case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE:
    {
#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
      TcpIp_Tls12ClientHsStm_SendClientCertificate(TlsConIdx);
#   endif
      break;
    }
    case TCPIP_TLS_STATE_SEND_CLIENT_KEY_EXCHANGE:
    {
      TcpIp_Tls12ClientHsStm_SendClientKeyExchange(TlsConIdx);
      break;
    }
    case TCPIP_TLS_STATE_SEND_CLIENT_CERTIFICATE_VERIFY:
    {
#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
      TcpIp_Tls12ClientHsStm_SendCertificateVerify(TlsConIdx);
#   endif
      break;
    }
    case TCPIP_TLS_STATE_SEND_CLIENT_CCS:
    {
      TcpIp_Tls12ClientHsStm_SendClientCcs(TlsConIdx);
      break;
    }
    case TCPIP_TLS_STATE_SEND_CLIENT_FINISHED:
    {
      TcpIp_Tls12ClientHsStm_SendClientFinished(TlsConIdx);
      break;
    }
    case TCPIP_TLS_STATE_WAIT_FOR_SERVER_CCS:
    {
      TcpIp_Tls12ClientHsStm_ParseServerCcs(TlsConIdx);
      break;
    }
    case TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED:
    {
      TcpIp_Tls12ClientHsStm_ParseServerFinished(TlsConIdx);
      break;
    }
    case TCPIP_TLS_STATE_HANDSHAKE_FINALIZING:
    {
      TcpIp_Tls12ClientHsStm_FinalizeHandshake(TlsConIdx);
      break;
    }
    default:                                                                                                           /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
    {
      break;
    } /* default */
  } /* switch() */
} /* TcpIp_Tls12ClientHsStm_PerformNextHsStep() */

/**********************************************************************************************************************
 *   TLS CLIENT ASYNC CONTEXT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCtx_AsyncHandler
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010, 6050, 6080 1 */ /* MD_TcpIp_TlsStatemachine_STPTH_6010, MD_TcpIp_TlsStatemachine_STCAL_6050, MD_TcpIp_TlsStatemachine_STMIF_6080 */
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_AsyncHandler(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientConnectionIterType tlsClientConnectionIdx
    = TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the trigger for the different ASYNC operations and execute the operation if the trigger is set */
#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
  /* Check if the certificate status response validation async context flag is TRUE */
  if (TcpIp_IsCertificateStatusOcspValidationAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx) == TRUE)
  {
    TcpIp_Tls12ClientCtx_ValidateCertificateStatus(TlsConIdx);
  }

  /* Check if the Signature verification async context flag is TRUE */
  if (TcpIp_IsHsVerifySignatureAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx) == TRUE)
  {
    /* Only perform signature verification if certificate has been validated successfully */
    /* [HS Server Certificate Chain Handling StM] Certificate verification finished, signatures are available */
    if (TcpIp_GetHsCertStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CERTCHAIN_FINISHED)
    {
      /* If the certificate chain is validated successfully, verify the signature of the public key */
      TcpIp_Tls12ClientCtx_VerifySignatureOfPublicKey(TlsConIdx);
      TcpIp_SetHsVerifySignatureAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx, FALSE);
    }
  }
  /* Check if the async context flag for calculating the CertificateVerify signature is TRUE */
  if (TcpIp_IsCalcClientCertVerifySignatureAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx) == TRUE)
  {
    TcpIp_Tls12ClientCrypto_CalcCertVerifySignature(TlsConIdx);
    TcpIp_SetCalcClientCertVerifySignatureAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx, FALSE);
  }
#   endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

  /* Check if the Client finished hash async context flag is TRUE and calculate the Client Finished data */
  if (TcpIp_IsCalcClientFinishedHashAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx) == TRUE)
  {
    TcpIp_Tls12ClientCtx_CalcClientFinishedHash(TlsConIdx);
    TcpIp_SetCalcClientFinishedHashAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx, FALSE);
  }

  /* Check if the Server Finished async context flag is TRUE and calculate the Server Finished data */
  if (TcpIp_IsCalcServerFinishedHashAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx) == TRUE)
  {
    TcpIp_Tls12ClientCtx_CalcServerFinishedHash(TlsConIdx);
    TcpIp_SetCalcServerFinishedHashAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx, FALSE);
  }

  /* Check if the Server Finished async context flag is TRUE and calculate the Client Finished Verify data */
  if (TcpIp_IsCalcServerFinishedVerifyDataAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx) == TRUE)
  {
    TcpIp_Tls12ClientCtx_CalcServerFinishedVerifyData(TlsConIdx);
    TcpIp_SetCalcServerFinishedVerifyDataAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx, FALSE);
  }
} /* TcpIp_Tls12ClientCtx_AsyncHandler() */

#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCtx_ValidateCertificateStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_ValidateCertificateStatus(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientConnectionIterType tlsClientConnectionIdx
    = TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  uint32 ocspResponseLength
    = TcpIp_GetServerCertStatusOcspResponseLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsBufferRxIterType ocspResponseStartTlsBufferRxIdx
    = TcpIp_GetTlsBufferRxServerCertStatusOcspResponseIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  TcpIp_TlsCertValidationStatusDynIterType tlsCertValidationStatusIdx = TcpIp_GetTlsCertValidationStatusDynStartIdxOfTlsConnection(TlsConIdx);
  KeyM_CertificateIdType leafCertId = TcpIp_GetAddrTlsCertValidationStatusDyn(tlsCertValidationStatusIdx)->CertId;

  KeyM_CertificateStatusType leafCertStatus;

  uint8 ocspResult;
  uint32 ocspResultLength = 1u;

  /* KeyM API needs a certificate name, which is not used within the API. Use some dummy value to satisfy API */
  CONST(uint8, AUTOMATIC) defaultName[8] = { "Default" };
  uint32 defaultNameLength = 7u;

  TcpIp_TlsUserErrorIdType internalErrorId = TCPIP_TLS_ERRORID_NOT_SET;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the status of the received leaf server certificate */
  if (KeyM_CertGetStatus(leafCertId, &leafCertStatus) == E_OK)
  {
    /* #20 Check that the leaf server certificate is in a valid state to process the OCSP response */
    if ((leafCertStatus == KEYM_CERTIFICATE_VALID) || (leafCertStatus == KEYM_CERTIFICATE_PARSED_NOT_VALIDATED))
    {
      /* Pointer to the received OCSP response within the TLS RxBuffer */
      TCPIP_P2C(uint8) ocspResponsePtr = TcpIp_GetAddrTlsBufferRx(ocspResponseStartTlsBufferRxIdx);

      /* #30 Process the received OCSP response */
      if (KeyM_ServiceCertificate(KEYM_SERVICE_CERT_STATUS_OCSP, &defaultName[0], defaultNameLength,
            ocspResponsePtr, ocspResponseLength, &ocspResult, ocspResultLength) == E_OK)
      {
        /* #40 OCSP response successful validated, write the OCSP response status to the certStatusPtr */
        TCPIP_P2V(TcpIp_CertValidationStatusType) certStatusPtr = TcpIp_GetAddrTlsCertValidationStatusDyn(tlsCertValidationStatusIdx);
        certStatusPtr->CertStatusResponse.OcspResponseStatus = ocspResult;

        TcpIp_SetCertificateStatusOcspValidationAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx, FALSE);
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);
      }
    /* #50 Otherwise */
      else
      {
        /* #60 Error occurred, update the error ID */
        /* Processing of OCSP response failed - KeyM call failed */
        internalErrorId = TCPIP_TLS_ERRORID_KEYM_SERVICE_OCSP_FAILED;
        TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_BAD_CERT_STATUS_RESP);
      }
    }
    else
    {
      /* Leaf certificate status invalid */
      internalErrorId = TCPIP_TLS_ERRORID_KEYM_INVALIDCERTSTATUS;
    }
  }
  else
  {
    /* Get status of Leaf certificate failed - KeyM call failed */
    internalErrorId = TCPIP_TLS_ERRORID_KEYM_GETCERTIFICATESTATUS_FAILED;
  }

  /* #70 Report an error to the user when the error ID is set */
  if (internalErrorId != TCPIP_TLS_ERRORID_NOT_SET)
  {
    /* Reset the processing trigger for the OCSP validation */
    TcpIp_SetCertificateStatusOcspValidationAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx, FALSE);

    TcpIp_TlsAsyncCtx_HandleErrorDuringAsyncJobProcessing(TlsConIdx,
      TCPIP_TLS_FCTID_CLIENT_ASYNCCTX_VALIDATE_CERT_STATUS, internalErrorId);
  }

} /* TcpIp_Tls12ClientCtx_ValidateCertificateStatus() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCtx_VerifySignatureOfPublicKey
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_VerifySignatureOfPublicKey(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsHandshakeActiveIdxOfTlsCipherWorkerDynType activeTlsHandshakeIdx =
    TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));

  TcpIp_TlsTemporarySignatureBufferStartIdxOfTlsConnectionType startTlsTemporarySignatureBufferIdx =
  TcpIp_GetTlsTemporarySignatureBufferStartIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsTemporarySignatureBufferEndIdxOfTlsConnectionType tlsTmpSignEndIdx =
    TcpIp_GetTlsTemporarySignatureBufferEndIdxOfTlsConnection(TlsConIdx);

  TcpIp_SegmentType signatureBufferSegment;
  TcpIp_SegmentType dataToSignBufferSegment;

  /* Retrieve the Server public key (+ Compression length) length */
  /* Public key length is checked in function TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetPubKey against its boundaries */
  uint8 serverPubKeyParamLen = IpBase_GetUint8(TcpIp_GetAddrTlsBufferRx(
    TcpIp_GetTlsBufferRxServerKeyExchangePubKeyLenIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)), 0);

  uint8 compFlagLen = 0;


  /*                                           +--> clientRndDestPtr = signDataPtr
                                               |               +--> serverRndDestPtr
                                               |               |               +--> serverKeyExchParamPtr
                                               v               v               v
   TemporarySignatureBuffer [ decoded signature | Client random | Server random | KeyExchange parameter ]
                            \                   \                                                      /
                             \                   \-----> CSM data to be verified                 <----/

   decode signature length: TCPIP_TLS_MAX_SIGNATURE_LEN
   Random length:           TCPIP_TLS_RANDOM_LEN
   KeyExchange length:      TCPIP_TLS_KEYEXCH_PARAMETERLEN = TCPIP_TLS_KEYEXCH_CURVETYPELEN + TCPIP_TLS_KEYEXCH_CURVENAMELEN + TCPIP_TLS_KEYEXCH_PUBKEYLEN;
  */

  /* ----- Implementation ----------------------------------------------- */
  /* Perform necessary checks before signature verification can be started. */
  if (TcpIp_TlsCoreUtil_PreVerifySignatureChecks(TlsConIdx) == E_OK)
  {
    uint32 serverSignLen;
    Std_ReturnType csmRetVal;

    /* The memory access for the signature and its length field are valid, the index check was performed in
     * TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetSignature(). */
    TcpIp_SignatureAlgorithmIdType signatureAlgorithmId = TcpIp_GetSignatureAlgorithmIdOfTlsHandshake(activeTlsHandshakeIdx);
    TcpIp_TlsBufferRxIterType signatureBufferIdx = TcpIp_GetTlsBufferRxServerSignatureIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);
    TcpIp_TlsBufferRxIterType signatureLenBufferIdx = TcpIp_GetTlsBufferRxServerSignatureLenIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

    /* That the CSM Job for signature verify operation is configured
     * is checked in TcpIp_Tls12ClientCtx_PreVerifySignatureChecks(). */

     /* If dynamic certificate groups are used, the Csm signature verify job reference cannot be determined from a
      * static configured handshake container. This is because only at runtime you know in which certificate slot
      * KeyM will store the leaf certificate, which is the first received certificate according to RFC5246 7.4.2.
      * Therefore get the certificate ID of the first received certificate and its configured Csm job Id.
      * Both is stored in TlsKeyMCertificate. */
    const TcpIp_TlsKeyMCertificateIterType tlsKeyMCertificateIdx
      = (TcpIp_TlsKeyMCertificateIterType)(TcpIp_GetAddrTlsCertValidationStatusDyn(
        TcpIp_GetTlsCertValidationStatusDynStartIdxOfTlsConnection(TlsConIdx))->CertId);
    const TcpIp_CsmJobIdType signatureVerifyJobId
      = TcpIp_GetCsmCertSignVerifyJobIdOfTlsKeyMCertificate(tlsKeyMCertificateIdx);

    /* Create two buffer segments, one for the signature, one for the data which shall be signed. */
    TcpIp_SegmentInit(&signatureBufferSegment, TcpIp_GetAddrTlsTemporarySignatureBuffer(startTlsTemporarySignatureBufferIdx),
      TCPIP_TLS_MAX_SIGNATURE_LEN);
    /* That no underflow occurs is ensured by TcpIp_Tls12ClientCtx_PreVerifySignatureChecks(). */
    TcpIp_SegmentInit(&dataToSignBufferSegment,
      TcpIp_GetAddrTlsTemporarySignatureBuffer(startTlsTemporarySignatureBufferIdx + TCPIP_TLS_MAX_SIGNATURE_LEN),
      ((uint32) tlsTmpSignEndIdx - startTlsTemporarySignatureBufferIdx - TCPIP_TLS_MAX_SIGNATURE_LEN));

    /* #10 Decode the signature if necessary and store in signature buffer segment */
    if (TcpIp_TlsCoreMsg_DecodeSignature(&signatureBufferSegment, signatureAlgorithmId, signatureBufferIdx, signatureLenBufferIdx) == E_OK)
    {
      Crypto_VerifyResultType verifyResult = CSM_E_VER_NOT_OK;

      /* Get the decoded signature length (just check how many bytes are occupied in the buffer after decoding). */
      serverSignLen = signatureBufferSegment.Usage;

      /* #20 Build the data to be verified */
      /* RFC8422 - ServerKeyExchange.signed_params.sha_hash
              SHA(ClientHello.random + ServerHello.random +
                                      ServerKeyExchange.params); */ /* RFC8422 - 5.4. Server Key Exchange */
      TcpIp_SegmentMemCpy(&dataToSignBufferSegment,
        TcpIp_GetAddrTlsBufferTx(TcpIp_GetTlsBufferTxClientRandomIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)),
        TCPIP_TLS_RANDOM_LEN);
      TcpIp_SegmentMemCpy(&dataToSignBufferSegment,
        TcpIp_GetAddrTlsBufferRx(TcpIp_GetTlsBufferRxServerRandomIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)),
        TCPIP_TLS_RANDOM_LEN);
      TcpIp_SegmentMemCpy(&dataToSignBufferSegment,
        TcpIp_GetAddrTlsBufferRx(
          TcpIp_GetTlsBufferRxServerKeyExchangeParamsIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)),
        TCPIP_TLS_KEYEXCH_PARAMETERLEN);

      /* #30 Check if the compression flag is used and update the values within the temporary buffer */
      if (TcpIp_TlsCoreUtil_CheckPublicKeyCompressionFlagOfEllipticCurve(
        TcpIp_GetEllipticCurveIdOfTlsHandshake(activeTlsHandshakeIdx)) == TRUE)
      {
        /* Compression flag is used, write the values to the temporary buffer */
        TcpIp_SegmentMemSet(&dataToSignBufferSegment, TCPIP_TLS_PUBKEY_COMPRESSION_FLAG, TCPIP_TLS_KEYEXCH_COMPFLAGLEN);
        compFlagLen = TCPIP_TLS_KEYEXCH_COMPFLAGLEN;
      }
      TcpIp_SegmentMemCpy(&dataToSignBufferSegment,
        TcpIp_GetAddrTlsBufferRx(
          TcpIp_GetTlsBufferRxServerKeyExchangePubKeyIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)),
        ((uint32)serverPubKeyParamLen - compFlagLen));

      /* #40 Call Csm Verify Signature for the server key exchange public key */
      csmRetVal = TCPIP_CSM_SignatureVerify(signatureVerifyJobId, CRYPTO_OPERATIONMODE_SINGLECALL,
        dataToSignBufferSegment.BasePtr, dataToSignBufferSegment.Usage, signatureBufferSegment.BasePtr,
        serverSignLen, &verifyResult);


      /* #50 Check the signature verification result */
      if (TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0,
        TCPIP_TLS_FCTID_CLIENT_ASYNCCTX_VERIFY_SIGN_PUBKEY) == E_OK)
      {
        /* CSM call OK, check the result of the operation */
        if (verifyResult == CSM_E_VER_OK)
        {
          /* Signature valid - processing done */
          /* #60 Set the async state to DONE */
          TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);
        }
        else
        {
          /* Invalid Signature - trigger TLS Alert */
          TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_DECRYPT_ERROR);
        }

      }
      else
      {
        /* #70 handles error while async CSM processing */
        TcpIp_TlsAsyncCtx_HandleErrorDuringAsyncJobProcessing(TlsConIdx,
          TCPIP_TLS_FCTID_CLIENT_ASYNCCTX_VERIFY_SIGN_PUBKEY, TCPIP_TLS_ERRORID_CRYPTO_CSM_CALL_FAILED);
      }
    }
    else
    {
      TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_DECRYPT_ERROR);
    }
  } /* else - pre checks failed, errors are handled in TcpIp_Tls12ClientCtx_PreVerifySignatureChecks() */
} /* TcpIp_Tls12ClientCtx_VerifySignatureOfPublicKey() */
#   endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCrypto_SetCsmKeys
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_SetCsmKeys(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsCipherStreamingModeDataType selectedStreamingMode
    = TcpIp_TlsCoreUtil_ReturnSelectedCipherSpecificStreamingMode(TlsConIdx);
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the CSM keys depending on the active streaming mode of the connection */
  switch (selectedStreamingMode)
  {
    case TCPIP_TLS_STREAMING_MODE_AES_128_CBC:
    {
      /* #20 and the active MAC mode of the cipherworker */
      /* X_WITH_AES_128_CBC_SHAX
           TCPIP_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA
           TCPIP_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
           TCPIP_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA
           TCPIP_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
      */
      retVal = TcpIp_Tls12ClientCrypto_SetCsmKeysAes128CbcStreaming(TlsConIdx);
      break;
    } /* case TCPIP_TLS_STREAMING_MODE_AES_128_CBC */

    case TCPIP_TLS_STREAMING_MODE_AES_128_GCM:
    case TCPIP_TLS_STREAMING_MODE_AES_256_GCM:
    case TCPIP_TLS_STREAMING_MODE_CHACHA20_POLY1305:
    {
      /* #30 and the active MAC mode of the cipherworker */
      /* X_WITH_AES_128_GCM_SHAX
           TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA
           TCPIP_TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256
           TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA
           TCPIP_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
      */
      retVal = TcpIp_Tls12ClientCrypto_SetCsmKeysBlock128Streaming(TlsConIdx);
      break;
    } /* case TCPIP_TLS_STREAMING_MODE_AES_128_CBC */

    case TCPIP_TLS_STREAMING_MODE_NULL:
    {
      /* X_WITH_NULL_SHAX
           TCPIP_TLS_ECDH_ECDSA_WITH_NULL_SHA
      */
      retVal = TcpIp_Tls12ClientCrypto_SetCsmKeysNullStreaming(TlsConIdx);
      break;
    } /* case TCPIP_TLS_STREAMING_MODE_NULL */
    default:                                                                                                             /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
    {
      /* Default case left blank intentionally */
      break;
    }
  } /* switch */

  if (retVal != E_OK)
  {
    /* #40 if not so, report the user error */
    TcpIp_TlsUserErrorType userError;

    userError.FunctionId = TCPIP_TLS_FCTID_CLIENT_CRYPTO_SETCSMKEYS;
    userError.UserErrorId = TCPIP_TLS_ERRORID_CRYPTO_SETCSMKEYS_FAILED;
    userError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(userError);
  }

  return retVal;
} /* TcpIp_Tls12ClientCrypto_SetCsmKeys() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCrypto_SetCsmKeysAes128CbcStreaming
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_SetCsmKeysAes128CbcStreaming(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx
    = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);

  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx
    = TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(activeTlsCipherWorkerIdx);

  TcpIp_TlsCsmJobIterType macGenerateTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
    TcpIp_GetCsmJobIdHmacGenerateIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));

  TcpIp_TlsCsmJobIterType decryptTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
    TcpIp_GetCsmJobIdCipherDecryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));

  TcpIp_TlsCsmJobIterType hmacVerifyTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
    TcpIp_GetCsmJobIdHmacVerifyIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));

  TcpIp_TlsCsmJobIterType encryptTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
    TcpIp_GetCsmJobIdCipherEncryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));

  TcpIp_TlsCsmKeyInfoType       keyBlockKey;
  TcpIp_TlsCsmKeyInfoType       txHMacShaKey;
  TcpIp_TlsCsmKeyInfoType       rxHMacShaKey;
  TcpIp_TlsCsmKeyInfoType       rxAesKey;
  TcpIp_TlsCsmKeyInfoType       txAesKey;
  Std_ReturnType              csmRetVal;
  /* ----- Implementation ----------------------------------------------- */
  keyBlockKey.KeyId        = TcpIp_GetPrfResultPrivateKeyIdOfTlsHandshake(activeTlsHandshakeIdx);
  keyBlockKey.KeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);

  /* Copy mac keys (for tx and rx) */
  /* #10 Set the TX MAC key */
  txHMacShaKey.KeyId = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(macGenerateTlsCsmJobIdx);
  txHMacShaKey.KeyElementId = CRYPTO_KE_MAC_KEY;
  txHMacShaKey.KeyLen = TcpIp_GetCipherMacKeyLengthOfTlsCipherWorker(activeTlsCipherWorkerIdx);

  csmRetVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &txHMacShaKey);
  /* Tx MAC key --> [0...txHMacShaKey.KeyLen] (0, 0, txHMacShaKey.KeyLen) */
  csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(
    keyBlockKey.KeyId,
    keyBlockKey.KeyElementId,
    0,
    0,
    txHMacShaKey.KeyLen,
    txHMacShaKey.KeyId,
    txHMacShaKey.KeyElementId
  );

  if (TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx,
        csmRetVal, 0, 0, TCPIP_TLS_FCTID_CLIENT_CRYPTO_SETCSMKEYSAES128CBC) == E_OK)
  {
    (void)TCPIP_CSM_KeySetValid(txHMacShaKey.KeyId);
  }

  /* #20 Set the RX MAC key */
  rxHMacShaKey.KeyId = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(hmacVerifyTlsCsmJobIdx);
  rxHMacShaKey.KeyElementId = CRYPTO_KE_MAC_KEY;
  rxHMacShaKey.KeyLen = TcpIp_GetCipherMacKeyLengthOfTlsCipherWorker(activeTlsCipherWorkerIdx);

  csmRetVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &rxHMacShaKey);
  /* Rx MAC key --> [txHMacShaKey.KeyLen...(txHMacShaKey.KeyLen + rxHMacShaKey.KeyLen)]
   * (txHMacShaKey.KeyLen, 0, rxHMacShaKey.KeyLen)
   */
  csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(
    keyBlockKey.KeyId,
    keyBlockKey.KeyElementId,
    txHMacShaKey.KeyLen,
    0,
    rxHMacShaKey.KeyLen,
    rxHMacShaKey.KeyId,
    rxHMacShaKey.KeyElementId
  );

  if (TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx,
        csmRetVal, 0, 0, TCPIP_TLS_FCTID_CLIENT_CRYPTO_SETCSMKEYSAES128CBC) == E_OK)
  {
    (void)TCPIP_CSM_KeySetValid(rxHMacShaKey.KeyId);
  }

  /* #30 Set the TX AES key */
  txAesKey.KeyId = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(encryptTlsCsmJobIdx);
  txAesKey.KeyElementId = CRYPTO_KE_CIPHER_KEY;
  txAesKey.KeyLen = TCPIP_TLS_AES128_KEY_LEN;

  csmRetVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &txAesKey);
  /* Tx Encrypt key --> [(txHMacShaKey.KeyLen + rxHMacShaKey.KeyLen)...(txKey.KeyLen + txHMacShaKey.KeyLen + rxHMacShaKey.KeyLen)]
   * (txHMacShaKey.KeyLen + rxHMacShaKey.KeyLen, 0, txKey.KeyLen)
   */
  csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(
    keyBlockKey.KeyId,
    keyBlockKey.KeyElementId,
    txHMacShaKey.KeyLen + rxHMacShaKey.KeyLen,
    0,
    txAesKey.KeyLen,
    txAesKey.KeyId,
    txAesKey.KeyElementId
  );

  if (TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx,
        csmRetVal, 0, 0, TCPIP_TLS_FCTID_CLIENT_CRYPTO_SETCSMKEYSAES128CBC) == E_OK)
  {
    (void)TCPIP_CSM_KeySetValid(txAesKey.KeyId);
  }

  /* #40 Set the RX AES key */
  rxAesKey.KeyId = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(decryptTlsCsmJobIdx);
  rxAesKey.KeyElementId = CRYPTO_KE_CIPHER_KEY;
  rxAesKey.KeyLen = TCPIP_TLS_AES128_KEY_LEN;

  csmRetVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &rxAesKey);
  /* Rx Encrypt key --> [(txKey.KeyLen + txHMacShaKey.KeyLen + rxHMacShaKey.KeyLen)....(txKey.KeyLen + rxKey.KeyLen + txHMacShaKey.KeyLen + rxHMacShaKey.KeyLen)]
   * (txHMacShaKey.KeyLen + rxHMacShaKey.KeyLen + txKey.KeyLen, 0, rxKey.KeyLen)
   */
  csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(
    keyBlockKey.KeyId,
    keyBlockKey.KeyElementId,
    txHMacShaKey.KeyLen + rxHMacShaKey.KeyLen + txAesKey.KeyLen,
    0,
    rxAesKey.KeyLen,
    rxAesKey.KeyId,
    rxAesKey.KeyElementId
  );

  if (TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx,
        csmRetVal, 0, 0, TCPIP_TLS_FCTID_CLIENT_CRYPTO_SETCSMKEYSAES128CBC) == E_OK)
  {
    (void)TCPIP_CSM_KeySetValid(rxAesKey.KeyId);
  }

  return csmRetVal;
} /* TcpIp_Tls12ClientCrypto_SetCsmKeysAes128CbcStreaming() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCrypto_SetCsmKeysBlock128Streaming
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_SetCsmKeysBlock128Streaming(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx
    = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx
    = TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(activeTlsCipherWorkerIdx);

  TcpIp_TlsCsmJobIterType decryptTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
    TcpIp_GetCsmJobIdCipherDecryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));

  TcpIp_TlsCsmJobIterType encryptTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
    TcpIp_GetCsmJobIdCipherEncryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));

  TcpIp_TlsCsmKeyInfoType       keyBlockKey;
  TcpIp_TlsCsmKeyInfoType       txIvKey;
  TcpIp_TlsCsmKeyInfoType       rxIvKey;
  TcpIp_TlsCsmKeyInfoType       rxKey;
  TcpIp_TlsCsmKeyInfoType       txKey;
  Std_ReturnType                csmRetVal;
  /* ----- Implementation ----------------------------------------------- */
  keyBlockKey.KeyId = TcpIp_GetPrfResultPrivateKeyIdOfTlsHandshake(activeTlsHandshakeIdx);
  keyBlockKey.KeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
  /* keyBlockKey.KeyLen = TcpIp_GetKeyBlockSizeOfTlsConnectionDyn(TlsConIdx); */

  /* For Block128 the key block is parsed as below
   *  0..15  Tx encrypt key
   *  16..31 Rx encrypt key
   *  32..35 Client Iv
   *  36..40 Server Iv
   */
  /* #10 Set the TX key */
  txKey.KeyId = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(encryptTlsCsmJobIdx);
  txKey.KeyElementId = CRYPTO_KE_CIPHER_KEY;
  txKey.KeyLen = TCPIP_TLS_BLOCK128_KEY_LEN;

  csmRetVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &txKey);
  /* Tx Encrypt key --> [0...(txKey.KeyLen)]
   * (0, 0, txKey.KeyLen)
   */
  csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(
    keyBlockKey.KeyId,
    keyBlockKey.KeyElementId,
    0,
    0,
    txKey.KeyLen,
    txKey.KeyId,
    txKey.KeyElementId
  );

  if (TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx,
        csmRetVal, 0, 0, TCPIP_TLS_FCTID_CLIENT_CRYPTO_SETCSMKEYSAEADBLOCK128) == E_OK)
  {
    (void)TCPIP_CSM_KeySetValid(txKey.KeyId);
  }

  /* #20 Set the RX key */
  rxKey.KeyId        = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(decryptTlsCsmJobIdx);
  rxKey.KeyElementId = CRYPTO_KE_CIPHER_KEY;
  rxKey.KeyLen       = TCPIP_TLS_BLOCK128_KEY_LEN;

  csmRetVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &rxKey);
  /* Rx Encrypt key --> [(txKey.KeyLen)....(txKey.KeyLen + rxKey.KeyLen)]
   * (txHMacShaKey.KeyLen, 0, rxKey.KeyLen)
   */
  csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(
    keyBlockKey.KeyId,
    keyBlockKey.KeyElementId,
    txKey.KeyLen,
    0,
    rxKey.KeyLen,
    rxKey.KeyId,
    rxKey.KeyElementId
  );

  if (TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx,
        csmRetVal, 0, 0, TCPIP_TLS_FCTID_CLIENT_CRYPTO_SETCSMKEYSAEADBLOCK128) == E_OK)
  {
    (void)TCPIP_CSM_KeySetValid(rxKey.KeyId);
  }

  /* #30 Set the TX Iv key */
  txIvKey.KeyId        = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(encryptTlsCsmJobIdx);
  txIvKey.KeyElementId = CRYPTO_KE_CIPHER_IV;
  txIvKey.KeyLen       = TCPIP_TLS_BLOCK128_IMPLICIT_IV_LEN;

  csmRetVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &txIvKey);
  /* Rx Encrypt key --> [(txKey.KeyLen + rxKey.KeyLen)....(txKey.KeyLen + rxKey.KeyLen + txIvKey.KeyLen)]
   * (txKey.KeyLen + rxKey.KeyLen, 0, txIvKey.KeyLen)
   */
  csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(
    keyBlockKey.KeyId,
    keyBlockKey.KeyElementId,
    txKey.KeyLen + rxKey.KeyLen,
    0,
    txIvKey.KeyLen,
    txIvKey.KeyId,
    txIvKey.KeyElementId
  );

  if (TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx,
        csmRetVal, 0, 0, TCPIP_TLS_FCTID_CLIENT_CRYPTO_SETCSMKEYSAEADBLOCK128) == E_OK)
  {
    (void)TCPIP_CSM_KeySetValid(txIvKey.KeyId);
  }

  /* #40 Set the RX Iv key */
  rxIvKey.KeyId        = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(decryptTlsCsmJobIdx);
  rxIvKey.KeyElementId = CRYPTO_KE_CIPHER_IV;
  rxIvKey.KeyLen       = TCPIP_TLS_BLOCK128_IMPLICIT_IV_LEN;

  csmRetVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &rxIvKey);
  /* Rx Encrypt key --> [(txKey.KeyLen + rxKey.KeyLen + txIvKey.KeyLen)....(txKey.KeyLen + rxKey.KeyLen + txIvKey.KeyLen + rxIvKey.KeyLen)]
   * (txKey.KeyLen + rxKey.KeyLen + txIvKey.KeyLen, 0, rxIvKey.KeyLen)
   */
  csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(
    keyBlockKey.KeyId,
    keyBlockKey.KeyElementId,
    txKey.KeyLen + rxKey.KeyLen + txIvKey.KeyLen,
    0,
    rxIvKey.KeyLen,
    rxIvKey.KeyId,
    rxIvKey.KeyElementId
  );

  if (TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx,
        csmRetVal, 0, 0, TCPIP_TLS_FCTID_CLIENT_CRYPTO_SETCSMKEYSAEADBLOCK128) == E_OK)
  {
    (void)TCPIP_CSM_KeySetValid(txIvKey.KeyId);
  }

  return csmRetVal;
} /* TcpIp_Tls12ClientCrypto_SetCsmKeysBlock128Streaming() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCrypto_SetCsmKeysNullStreaming
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_SetCsmKeysNullStreaming(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx
    = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx
    = TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(activeTlsCipherWorkerIdx);

  TcpIp_TlsCsmJobIterType macGenerateTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
    TcpIp_GetCsmJobIdHmacGenerateIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));

  TcpIp_TlsCsmJobIterType macVerifyTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
    TcpIp_GetCsmJobIdHmacVerifyIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));

  TcpIp_TlsCsmKeyInfoType       keyBlockKey;
  TcpIp_TlsCsmKeyInfoType       txHMacShaKey;
  TcpIp_TlsCsmKeyInfoType       rxHMacShaKey;
  Std_ReturnType                csmRetVal;
  /* ----- Implementation ----------------------------------------------- */
  keyBlockKey.KeyId        = TcpIp_GetPrfResultPrivateKeyIdOfTlsHandshake(activeTlsHandshakeIdx);
  keyBlockKey.KeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);

  /* Copy mac keys (for tx and rx) */
  /* #10 Set the TX MAC key */
  txHMacShaKey.KeyId = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(macGenerateTlsCsmJobIdx);
  txHMacShaKey.KeyElementId = CRYPTO_KE_MAC_KEY;
  txHMacShaKey.KeyLen = TcpIp_GetCipherMacKeyLengthOfTlsCipherWorker(activeTlsCipherWorkerIdx);

  csmRetVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &txHMacShaKey);
  csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(
    keyBlockKey.KeyId,
    keyBlockKey.KeyElementId,
    0,
    0,
    txHMacShaKey.KeyLen,
    txHMacShaKey.KeyId,
    txHMacShaKey.KeyElementId
  );
  if (TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx,
        csmRetVal, 0, 0, TCPIP_TLS_FCTID_CLIENT_CRYPTO_SETCSMKEYSNULL) == E_OK)
  {
    (void)TCPIP_CSM_KeySetValid(txHMacShaKey.KeyId);
  }

  /* #20 Set the RX MAC key */
  rxHMacShaKey.KeyId = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(macVerifyTlsCsmJobIdx);
  rxHMacShaKey.KeyElementId = CRYPTO_KE_MAC_KEY;
  rxHMacShaKey.KeyLen = TcpIp_GetCipherMacKeyLengthOfTlsCipherWorker(activeTlsCipherWorkerIdx);

  csmRetVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &rxHMacShaKey);
  csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(
    keyBlockKey.KeyId,
    keyBlockKey.KeyElementId,
    txHMacShaKey.KeyLen,
    0,
    rxHMacShaKey.KeyLen,
    rxHMacShaKey.KeyId,
    rxHMacShaKey.KeyElementId
  );
  if (TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx,
        csmRetVal, 0, 0, TCPIP_TLS_FCTID_CLIENT_CRYPTO_SETCSMKEYSNULL) == E_OK)
  {
    (void)TCPIP_CSM_KeySetValid(rxHMacShaKey.KeyId);
  }

  return csmRetVal;
} /* TcpIp_Tls12ClientCrypto_SetCsmKeysNullStreaming() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_SendClientFinished
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_SendClientFinished(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, FALSE);

  /* #10 Prepare the ClientFinished message */
  TcpIp_Tls12ClientMsg_PrepareClientFinished(TlsConIdx);

  /* #20 Check the result of the preparation of the ClientFinished message. */
  if ((TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CTX_NO_ASYNC))
  {
    /* #30 Preparation was successful. Change the state to TCPIP_TLS_STATE_WAIT_FOR_SERVER_CCS */
    TcpIp_TlsClientConnectionIterType tlsClientConnectionIdx = TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx);
    TcpIp_SetClientFinishedReadyToSendOfTlsClientConnection(tlsClientConnectionIdx, TRUE);
    TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_CCS);
    TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, FALSE);
  }
  else if ((TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CTX_ASYNC_PENDING))
  {
    /* #40 Preparation was not successful, an async operation is pending. Nothing to do, just leave the function. */
  }
  else
  {
    /* #50 Preparation failed. Cancel the handshake. */
    TcpIp_TlsClientHsStm_HandleErrorDuringPreparation(TlsConIdx);
  }

} /* TcpIp_Tls12ClientHsStm_SendClientFinished() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_PrepareClientFinished
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_PrepareClientFinished(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType     tlsBufferMgmtIdx       = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare the ClientFinished message until we are done AND no Async context needs to be computed */
  while (   (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_STATE_SEND_CLIENT_FINISHED_DONE)         /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
         && (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_CTX_ASYNC_PENDING))
  {
    switch (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx))
    {
      case TCPIP_TLS_STATE_SEND_CLIENT_FINISHED:
      case TCPIP_TLS_STATE_SEND_CLIENT_FINISHED_ADD_HDR:
      {
        /* Set the start index of this message to the current write index */
        TcpIp_SetTlsBufferTxClientFinishedIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
            TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

        /* #20 Add the generic handshake header */
        TcpIp_TlsMsg_AddHsHdr(TlsConIdx, TCPIP_TLS_HSTYPE_FINISHED);

        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_FINISHED_CALC_HASH);
        break;
      } /* case  TCPIP_TLS_STATE_SEND_CLIENT_FINISHED_ADD_HDR */

      case TCPIP_TLS_STATE_SEND_CLIENT_FINISHED_CALC_HASH:
      {
        /* #30 Add the Finished Data */
        TcpIp_Tls12ClientMsg_ClientFinishedAddClientFinishedData(TlsConIdx);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_FINISHED_TRIGGER_TX);
        break;
      } /* case TCPIP_TLS_STATE_SEND_CLIENT_FINISHED_CALC_HASH */

      case TCPIP_TLS_STATE_SEND_CLIENT_FINISHED_TRIGGER_TX:
      {

        TcpIp_SetClientFinishedLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
          (TCPIP_TLS_FINISHED_VERIFYDATA_LEN + TCPIP_TLS_HSHDRLEN));

        /* #40 Write the length of the ClientFinished (12 byte) into the HS-Header */
        TcpIp_TlsMsg_AddLenToHsHdr(TcpIp_GetTlsBufferTxClientFinishedIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx),
          TCPIP_TLS_FINISHED_VERIFYDATA_LEN);

        /* #50 Enable the reception of a CCS message */
        TcpIp_SetCcsReceivableOfTlsConnectionDyn(TlsConIdx, TRUE);
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_NO_ASYNC);
        TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_SEND_CLIENT_FINISHED_DONE);
        break;
      } /* case TCPIP_TLS_STATE_SEND_CLIENT_FINISHED_TRIGGER_TX */

      default:                                                                                                         /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
      /* Default case left blank intentionally */
        break;
    } /* switch () */
  } /* while ( ) */

} /* TcpIp_Tls12ClientMsg_PrepareClientFinished() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ClientFinishedAddClientFinishedData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_ClientFinishedAddClientFinishedData(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsClientConnectionIterType tlsClientConnectionIdx
    = TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx);


  /* ----- Implementation ----------------------------------------------- */
  /* Save the position of the verify data (will be set inside the async context function) */
  TcpIp_SetTlsBufferTxClientFinishedVerifyDataIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx,
    TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx));

  /* Increment the write index by the consumed length (12 Byte) */
  TcpIp_AddTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_FINISHED_VERIFYDATA_LEN);

  /* #10 Handles synchronous CSM call and trigger the calculation of the job in the async context handler */
  TcpIp_SetCalcClientFinishedHashAsyncFlagOfTlsClientConnection(tlsClientConnectionIdx, TRUE);
  /* Set the state to ASYNC_PENDING */
  TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_ASYNC_PENDING);
}

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCtx_CalcClientFinishedHash
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_CalcClientFinishedHash(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal       = E_NOT_OK;
  uint16         localErrorId = TCPIP_TLS_ERRORID_NOT_SET;                                                              /* PRQA S 2981 */ /* MD_TCPIP_Rule2.2_RedundantAssignment */
  uint8          calcVerifyData[TCPIP_TLS_FINISHED_VERIFYDATA_LEN] = { 0 };

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Generate the hash which is needed for the finished data */
  if (TcpIp_Tls12ClientCrypto_GenerateHsHash(TlsConIdx, TCPIP_TLS_MSG_HASH_CLIENT_FINISHED) == E_OK)
  {
    /* #20 If it is successful, calculate the finished data and store it directly into Tx buffer */
    if (TcpIp_Tls12ClientCrypto_CalcHsFinishedData(TlsConIdx, TCPIP_TLS_MSG_HASH_CLIENT_FINISHED, calcVerifyData) == E_OK)
    {
      TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
        = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

      TCPIP_P2V(uint8) targetPtr = TcpIp_GetAddrTlsBufferTx(TcpIp_GetTlsBufferTxClientFinishedVerifyDataIdxOfTlsClientBufferMgmt(
        tlsClientBufferMgmtIdx));

      VStdMemCpy(targetPtr, calcVerifyData, TCPIP_TLS_FINISHED_VERIFYDATA_LEN);                                         /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

      TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);

      /* No error set */
      retVal = E_OK;
    }
    else
    {
      /* Error derive key block failed */
      localErrorId = TCPIP_TLS_ERRORID_CRYPTO_CLIENTFINISHED_VERIFY_FAILED;
    }
  }
  else
  {
    /* Error derive key block failed */
    localErrorId = TCPIP_TLS_ERRORID_CRYPTO_CLIENTFINISHED_HASH_FAILED;
  }

  /* #30 If all sub-functions returned E_OK */
  if (retVal != E_OK)
  {
    /* #40 Copy the result into the TLS temp buffer and set the state to ASYNC_DONE */

    /* Set the async state to DONE */
    /* #50 otherwise, handles error while async CSM processing */
    TcpIp_TlsAsyncCtx_HandleErrorDuringAsyncJobProcessing(TlsConIdx, TCPIP_TLS_FCTID_CLIENT_ASYNCCTX_CALC_CLIENTFINISHED_HASH,
                                                    localErrorId);
  }
} /* TcpIp_Tls12ClientCtx_CalcClientFinishedHash() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCrypto_GenerateHsHash
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_GenerateHsHash(
  TcpIp_TlsConnectionIterType     TlsConIdx,
  TcpIp_TlsMessageFinishedType    FinishedMessageType)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx
    = TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));

  uint32 csmHashJobId = TcpIp_GetHashVerifyJobIdOfTlsHandshake(activeTlsHandshakeIdx);
  Std_ReturnType csmRetVal;

  /* Calculate Hash over:
   * Stored in TxBuffer:   ClientHello  + (ClientCertificates) + ClientKeyExchange (+ CertificateVerify) (+ ClientFinished)
   * Stored in RxBuffer:   ServerHello (+ ServerCertificate  + CertificateStatus + ServerKeyExchange + CertificateRequest) + ServerHelloDone
   */

  /* Set the start of the second hash part - depending on transmitted TLS-Client messages */
  uint32 clientTxLen;
  TCPIP_P2C(uint8) clientTxPtr;

  if (TcpIp_GetTlsBufferTxClientCertificateIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)
        != TCPIP_NO_TLSBUFFERTXCLIENTCERTIFICATEIDXOFTLSCLIENTBUFFERMGMT)
  {
    /* Certificate message transmitted, start of TX buffer is the Client Certificate message */
    clientTxPtr = TcpIp_GetAddrTlsBufferTx(
      TcpIp_GetTlsBufferTxClientCertificateIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
  }
  else
  {
    /* No Certificate message transmitted, start of TX buffer is the Client Key Exchange message */
    clientTxPtr = TcpIp_GetAddrTlsBufferTx(
      TcpIp_GetTlsBufferTxClientKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
  }

  /* Depending on Client hash / Server hash, also consider the Client finished message length */
  if (FinishedMessageType == TCPIP_TLS_MSG_HASH_SERVER_FINISHED)
  {
    /* For server finished, consider till client finished message */
    clientTxLen = (uint32)TcpIp_GetClientKeyExchangeLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)
                  + TcpIp_GetClientFinishedLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);
  }
  else /* FinishedMessageType == TCPIP_TLS_MSG_HASH_CLIENT_FINISHED */
  {
    /* For client finished, only consider till client key exchange message */
    clientTxLen = TcpIp_GetClientKeyExchangeLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);
  }

  /* This length is 0, if no certificate message is transmitted */
  clientTxLen += TcpIp_GetClientCertificateLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);
  /* This length is 0, if no certificate verify message is transmitted */
  clientTxLen += TcpIp_GetClientCertificateVerifyLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Start finished hash calculation with ClientHello message (TxBuffer) */
  {
    TCPIP_P2C(uint8) clientHelloPtr = TcpIp_GetAddrTlsBufferTx(
      TcpIp_GetTlsBufferTxClientHelloIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));

    uint32 clientHelloLen = TcpIp_GetClientHelloLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

    csmRetVal = TCPIP_CSM_Hash(csmHashJobId, CRYPTO_OPERATIONMODE_START | CRYPTO_OPERATIONMODE_UPDATE, clientHelloPtr,
      clientHelloLen, NULL_PTR, NULL_PTR);
  }

  /* #20 Update finished hash with ServerHello message (RxBuffer) */
  {
    TCPIP_P2C(uint8) rxServerHelloPtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerHelloIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 rxServerHelloLen
      = ((uint32)TcpIp_GetServerHelloLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    csmRetVal |= TCPIP_CSM_Hash(csmHashJobId, CRYPTO_OPERATIONMODE_UPDATE, rxServerHelloPtr,
      rxServerHelloLen, NULL_PTR, NULL_PTR);
  }

  /* #30 Update finished hash with ServerCertificates message (RxBuffer) - if received */
  if (TcpIp_GetTlsBufferRxServerCertificatesIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)
       != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATESIDXOFTLSCLIENTBUFFERMGMT)
  {
    TCPIP_P2C(uint8) rxServerCertificatePtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerCertificatesIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
   uint32 rxServerCertificateLen
     = ((uint32)TcpIp_GetServerCertificatesLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    csmRetVal |= TCPIP_CSM_Hash(csmHashJobId, CRYPTO_OPERATIONMODE_UPDATE, rxServerCertificatePtr,
      rxServerCertificateLen, NULL_PTR, NULL_PTR);
  }

  /* #40 Update finished hash with ServerCertificateStatus message (RxBuffer) - if received */
  if (TcpIp_GetTlsBufferRxServerCertificateStatusIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)
       != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATESTATUSIDXOFTLSCLIENTBUFFERMGMT)
  {

    TCPIP_P2C(uint8) rxServerCertificateStatusPtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerCertificateStatusIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 rxServerCertificateStatusLen
      = ((uint32)TcpIp_GetServerCertificateStatusLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    csmRetVal |= TCPIP_CSM_Hash(csmHashJobId, CRYPTO_OPERATIONMODE_UPDATE, rxServerCertificateStatusPtr,
      rxServerCertificateStatusLen, NULL_PTR, NULL_PTR);
  }

  /* #50 Update finished hash with ServerKeyExchange message (RxBuffer) - if received */
  if (TcpIp_GetTlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)
       != TCPIP_NO_TLSBUFFERRXSERVERKEYEXCHANGEIDXOFTLSCLIENTBUFFERMGMT)
  {
    TCPIP_P2C(uint8) rxServerKeyExchangePtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerKeyExchangeIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 rxServerKeyExchangeLen
      = ((uint32)TcpIp_GetServerKeyExchangeLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    csmRetVal |= TCPIP_CSM_Hash(csmHashJobId, CRYPTO_OPERATIONMODE_UPDATE, rxServerKeyExchangePtr,
      rxServerKeyExchangeLen, NULL_PTR, NULL_PTR);
  }

  /* #60 Update finished hash with ServerCertificateRequest message (RxBuffer) - if received */
  if (TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)
       != TCPIP_NO_TLSBUFFERRXSERVERCERTIFICATEREQUESTIDXOFTLSCLIENTBUFFERMGMT)
  {

    TCPIP_P2C(uint8) rxServerCertificateRequestPtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerCertificateRequestIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 rxServerCertificateRequestLen
      = ((uint32)TcpIp_GetServerCertificateRequestLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    csmRetVal |= TCPIP_CSM_Hash(csmHashJobId, CRYPTO_OPERATIONMODE_UPDATE, rxServerCertificateRequestPtr,
      rxServerCertificateRequestLen, NULL_PTR, NULL_PTR);
  }

  /* #70 Update finished hash with ServerHelloDone message (RxBuffer) */
  {
    TCPIP_P2C(uint8) rxServerHelloDonePtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxServerHelloDoneIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));
    uint32 rxServerHelloDoneLen
      = ((uint32)TcpIp_GetServerHelloDoneLenOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx) + TCPIP_TLS_HSHDRLEN);

    csmRetVal |= TCPIP_CSM_Hash(csmHashJobId, CRYPTO_OPERATIONMODE_UPDATE, rxServerHelloDonePtr,
      rxServerHelloDoneLen, NULL_PTR, NULL_PTR);
  }

  /* #80 Update and finalize finished hash with client messages (TxBuffer) */
  {
    /* Hash shall be stored into the Tls temporary buffer and used to generate the finished data */
    TCPIP_P2V(uint8) hsHashDestPtr = TcpIp_GetAddrTlsTemporaryFinishedBuffer(
      TcpIp_GetTlsTemporaryFinishedBufferStartIdxOfTlsConnection(TlsConIdx));

    uint32 hsHashDestSpace = TcpIp_GetTlsTemporaryFinishedBufferEndIdxOfTlsConnection(TlsConIdx)
      - (uint32)TcpIp_GetTlsTemporaryFinishedBufferStartIdxOfTlsConnection(TlsConIdx);

    csmRetVal |= TCPIP_CSM_Hash(csmHashJobId, CRYPTO_OPERATIONMODE_UPDATE | CRYPTO_OPERATIONMODE_FINISH, clientTxPtr,
      clientTxLen, hsHashDestPtr, &hsHashDestSpace);

    /* #90 Check if the CSM hash operation succeeded and return E_OK */
    (void)TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, hsHashDestSpace, TCPIP_TLS_HASH_SHA256_LEN,
      TCPIP_TLS_FCTID_CLIENT_CRYPTO_CALCHSHASH);
  }

  /* User error handling is done outside of this function */
  return csmRetVal;
} /* TcpIp_Tls12ClientCrypto_GenerateHsHash() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCrypto_CalcHsFinishedData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientCrypto_CalcHsFinishedData(
  TcpIp_TlsConnectionIterType    TlsConIdx,
  TcpIp_TlsMessageFinishedType   FinishedMessageType,
  TCPIP_P2V(uint8)               ResultPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx =
    TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));

  TCPIP_P2C(uint8) labelPtr = &TcpIp_Tls_Hs_FinishedLabel[FinishedMessageType][0];
  TCPIP_P2C(uint8) seedPtr = TcpIp_GetAddrTlsTemporaryFinishedBuffer(TcpIp_GetTlsTemporaryFinishedBufferStartIdxOfTlsConnection(TlsConIdx));

  TcpIp_TlsCsmKeyInfoType msKey;
  TcpIp_TlsCsmKeyInfoType resultKey;
  Std_ReturnType        csmRetVal = E_NOT_OK;
  /* ----- Implementation ----------------------------------------------- */
  /* Retrieve master shared secret to local key element */
  msKey.KeyId        = TcpIp_GetCsmKeyId(TcpIp_GetCsmKeyIdMasterSecretIdxOfTlsHandshake(activeTlsHandshakeIdx));
  msKey.KeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
  msKey.KeyLen       = TCPIP_TLS_MASTER_SECRET_LEN;

  /* Retrieve Prf result key (contains result of verify data) to local key element */
  resultKey.KeyId        = TcpIp_GetPrfResultPublicKeyIdOfTlsHandshake(activeTlsHandshakeIdx);
  resultKey.KeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
  resultKey.KeyLen       = TCPIP_TLS_FINISHED_VERIFYDATA_LEN;

  /* #10 Calculate the Prf using the Hash, Master secret and label */
  if (   TcpIp_Tls12CoreCrypto_Prf(TlsConIdx, &resultKey, &msKey, labelPtr, seedPtr, TCPIP_TLS_LABEL_FINISHED_LEN, 32u, FALSE)
      == E_OK)
  {
    /* #20 Write the calculated finished data (stored in CSM Key) into destination buffer */
    uint32 tmpLen = resultKey.KeyLen;
    csmRetVal = TCPIP_CSM_KeyElementGet(resultKey.KeyId, resultKey.KeyElementId, &ResultPtr[0], &tmpLen);

    /* #30 Check if the CSM operation succeeded */
    (void)TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, tmpLen, resultKey.KeyLen,
            TCPIP_TLS_FCTID_CLIENT_CRYPTO_CALCHSFINISHEDDATA);
    /* User error handling is done outside of this function */
  }

  return csmRetVal;
} /* TcpIp_Tls12ClientCrypto_CalcHsFinishedData() */

/**********************************************************************************************************************
*  TcpIp_Tls12ClientHsStm_ExtractServerFinished
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCAL_6050 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ExtractServerFinished(
   TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxServerFinishedIdxOfTlsClientBufferMgmtType serverFinishedIdx =
    TcpIp_GetTlsBufferRxServerFinishedIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  Std_ReturnType retVal = E_OK;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if we received a handshake message with the type ServerFinished */
  if (serverFinishedIdx != TCPIP_NO_TLSBUFFERRXSERVERFINISHEDIDXOFTLSCLIENTBUFFERMGMT)
  {
    TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, FALSE);

    /* #20 Extract the ServerFinished message until we are done AND no Async context needs to be computed */
    while (   (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED_DONE)   /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
               /* Async operation is trigged (performed in lowPrio task - leave this context */
           && (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_CTX_ASYNC_PENDING)
               /* Error during processing of the data - skip further data handling */
           && (retVal == E_OK))
    {
      switch (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx))
      {
        case TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED:
        {
          /* #30 Extract the Verify Data value from the message */
          retVal = TcpIp_Tls12ClientMsg_ExtractServerFinishedInformation(TlsConIdx);
          TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED_CALC_HASH);
          break;
        }

        case TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED_CALC_HASH:
        {
          /* #40 Trigger calculation of Hash */
          TcpIp_Tls12ClientMsg_CalcServerFinishedHash(TlsConIdx);
          TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED_CALC_FINISHED);

          /* This operation is processed within the lowPrio function, which means that we are leaving
             this context and return when the operation is DONE */
          break;
        }

        case TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED_CALC_FINISHED:
        {
          /* #50 Trigger calculation of PRF value from the hash */
          TcpIp_Tls12ClientMsg_CalcServerFinishedVerifyData(TlsConIdx);
          TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED_VERIFY);

          /* This operation is processed within the lowPrio function, which means that we are leaving
             this context and return when the operation is DONE */
          break;
        }

        case TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED_VERIFY:
        {
          /* #60 Compare the calculated and received value */
          retVal = TcpIp_Tls12ClientMsg_CompareFinishedVerifyData(TlsConIdx);
          TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED_DONE);
          break;
        }

        case TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED_DONE:
        {
          /* #70 Client finished message has been verified, move to next step */
          break;
        }

        default:                                                                                                         /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
        /* Default case left blank intentionally */
          break;
      }
    }
  }
  else
  {
    /* #80 otherwise, we receive an unexpected message, so trigger the alert */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
  }

  return retVal;
} /* TcpIp_Tls12ClientHsStm_ExtractServerFinished() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_ExtractServerFinishedInformation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_ExtractServerFinishedInformation(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxAdHsReadIdxOfTlsBufferMgmtDynType rxServerFinishedVerifyDataIdx
    = TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

  TcpIp_TlsBufferRxServerFinishedIdxOfTlsClientBufferMgmtType rxServerFinishedStartTlsBufferRxIdx
    = TcpIp_GetTlsBufferRxServerFinishedIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx);

  Std_ReturnType retVal = E_NOT_OK;

  /* rxServerFinishedStartTlsBufferRxIdx is pointing to the start of the received ServerFinished message, therefore offset of 1 byte is pointing to the 3 byte length field */
  uint32 rxServerFinishedVerifyDataLen = IpBase_GetUint24(
    TcpIp_GetAddrTlsBufferRx(rxServerFinishedStartTlsBufferRxIdx), TCPIP_TLS_CONTENT_LENGTH_OFFSET);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if we received 12 bytes of data (Finished message length) and store the information */
  if (rxServerFinishedVerifyDataLen == TCPIP_TLS_FINISHED_VERIFYDATA_LEN)
  {
    /* Save the position of the received ClientFinishedVerifyData */
    TcpIp_SetTlsBufferRxServerFinishedVerifyDataIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx, rxServerFinishedVerifyDataIdx);

    /* Increment the Rx buffer ReadIdx by the length of the VerifyData (12Byte) */
    TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_FINISHED_VERIFYDATA_LEN);

    retVal = E_OK;
  }
  else
  {
    /* If an error occurs report the error to the user */
    TcpIp_TlsUserErrorType UserError;
    UserError.FunctionId  = TCPIP_TLS_FCTID_CLIENT_HSSTM_PARSE_SERVERFINISHED;
    UserError.UserErrorId = TCPIP_TLS_ERRORID_CTHS_UNEXPECTEDMSGLEN;
    UserError.TlsConId    = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(UserError);

    /* #20 Trigger alert TCPIP_TLS_ALERT_DESC_DECODE_ERROR in case of invalid length */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_DECODE_ERROR);
  }

  return retVal;
} /* TcpIp_Tls12ClientMsg_ExtractServerFinishedInformation() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_CalcServerFinishedHash
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_CalcServerFinishedHash(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handles synchronous CSM call and trigger the calculation of the job in the async context handler */
  TcpIp_SetCalcServerFinishedHashAsyncFlagOfTlsClientConnection(                                                        /* TCPIP_CSL_MISSDETECTION */
    TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx), TRUE);
  /* Set the state to ASYNC_PENDING */
  TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_ASYNC_PENDING);

} /* TcpIp_Tls12ClientMsg_CalcServerFinishedHash() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_CalcServerFinishedVerifyData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientMsg_CalcServerFinishedVerifyData(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handles synchronous CSM call and trigger the calculation of the job in the async context handler */
  TcpIp_SetCalcServerFinishedVerifyDataAsyncFlagOfTlsClientConnection(                                                 /* TCPIP_CSL_MISSDETECTION */
    TcpIp_GetTlsClientConnectionIdxOfTlsConnection(TlsConIdx), TRUE);
  /* Set the state to ASYNC_PENDING */
  TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_ASYNC_PENDING);

} /* TcpIp_Tls12ClientMsg_CalcServerFinishedVerifyData() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientMsg_CompareFinishedVerifyData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12ClientMsg_CompareFinishedVerifyData(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
    = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* Calculated data is present in Temp buffer */
  TCPIP_P2C(uint8) calcVerifyDataPtr = TcpIp_GetAddrTlsTemporaryFinishedBuffer(
    TcpIp_GetTlsTemporaryFinishedBufferStartIdxOfTlsConnection(TlsConIdx));

  TCPIP_P2C(uint8) rxVerifyDataPtr = TcpIp_GetAddrTlsBufferRx(
    TcpIp_GetTlsBufferRxServerFinishedVerifyDataIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));

  /* #10 Compare the calculated finished value and the received finished value */
  if (IpBase_StrCmpLen(rxVerifyDataPtr, calcVerifyDataPtr, TCPIP_TLS_FINISHED_VERIFYDATA_LEN) == E_OK)
  {
    retVal = E_OK;
  }
  else
  {
    /* If an error occurs report the error to the user */
    TcpIp_TlsUserErrorType userError;
    userError.FunctionId  = TCPIP_TLS_FCTID_CLIENT_HSSTM_PARSE_SERVERFINISHED;
    userError.UserErrorId = TCPIP_TLS_ERRORID_CTHS_INCORRECTVERIFYDATA;
    userError.TlsConId    = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(userError);

    /* #20 Trigger alert TCPIP_TLS_ALERT_DESC_DECRYPT_ERROR in case of invalid length */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_DECRYPT_ERROR);

    retVal = E_NOT_OK;
  }
  return retVal;
} /* TcpIp_Tls12ClientMsg_CompareFinishedVerifyData */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCtx_CalcServerFinishedHash
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_CalcServerFinishedHash(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Generate the hash which is needed for the finished data */
  if (TcpIp_Tls12ClientCrypto_GenerateHsHash(TlsConIdx, TCPIP_TLS_MSG_HASH_SERVER_FINISHED) == E_OK)
  {
    /* #20 If the operation was successful, set the async state to DONE */
    TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);
  }
  else
  {
    /* #30 if the operation was not successful, handle the error while an ongoing async CSM processing */
    TcpIp_TlsAsyncCtx_HandleErrorDuringAsyncJobProcessing(TlsConIdx, TCPIP_TLS_FCTID_CLIENT_ASYNCCTX_CALC_SERVERFINISHED_HASH,
      TCPIP_TLS_ERRORID_CRYPTO_SERVERFINISHED_HASH_FAILED);
  }

} /* TcpIp_Tls12ClientCtx_CalcServerFinishedHash() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientCtx_CalcServerFinishedVerifyData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientCtx_CalcServerFinishedVerifyData(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Pointer to the temporary buffer that stores the verify data */
  TCPIP_P2V(uint8) targetPtr = TcpIp_GetAddrTlsTemporaryFinishedBuffer(
    TcpIp_GetTlsTemporaryFinishedBufferStartIdxOfTlsConnection(TlsConIdx));

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the finished data, store it in temp buffer and update the state */
  if (TcpIp_Tls12ClientCrypto_CalcHsFinishedData(TlsConIdx, TCPIP_TLS_MSG_HASH_SERVER_FINISHED, targetPtr) == E_OK)
  {

    /* Set the async state to DONE */
    TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);
  }
  else
  {
    /* #20 Error during calculation of finished data. Report error to the user */
    TcpIp_TlsAsyncCtx_HandleErrorDuringAsyncJobProcessing(TlsConIdx, TCPIP_TLS_FCTID_CLIENT_ASYNCCTX_CALC_SERVERFINISHED_VERIFY,
      TCPIP_TLS_ERRORID_CRYPTO_CLIENTFINISHED_VERIFY_FAILED);
  }

} /* TcpIp_Tls12ClientCtx_CalcServerFinishedVerifyData() */

/**********************************************************************************************************************
*  TcpIp_Tls12ClientHsStm_ParseServerCcs
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerCcs(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if we received a CCS message */
  if (TcpIp_IsCcsRxIndicationOfTlsConnectionDyn(TlsConIdx) == TRUE)
  {
    /* #20 Change the state to TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED */
    TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_WAIT_FOR_SERVER_FINISHED);
    TcpIp_SetCcsRxIndicationOfTlsConnectionDyn(TlsConIdx, FALSE);
    /* #30 Lock the reception of a new CCS message after this point */
    TcpIp_SetCcsReceivableOfTlsConnectionDyn(TlsConIdx, FALSE);
  }
  else
  {
    /* #40 Trigger the transmission of the occurred alert */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
  }
} /* TcpIp_Tls12ClientHsStm_ParseServerCcs() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_ParseServerFinished
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_ParseServerFinished(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Parse and verify the Server Finished message */
  retVal = TcpIp_Tls12ClientHsStm_ExtractServerFinished(TlsConIdx);

  /* #20 Check the result of the ServerFinished parsing */
  if (retVal == E_OK)
  {
    /* #30 Message successfully parsed - Check if there are async operation pending */
    if (TcpIp_GetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_CTX_ASYNC_PENDING)
    {
      /* #40 Async operation is pending. Nothing to do, just leave the function. */
    }
    else
    {
      /* #50 No async operation pending. Change the state to TCPIP_TLS_STATE_HANDSHAKE_FINALIZING */
      TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_HANDSHAKE_FINALIZING);
      TcpIp_SetHsStmTxTriggerOfTlsConnectionDyn(TlsConIdx, TRUE);
    }
  }
  else
  {
    /* #60 Parsing failed. Cancel the handshake. */
    TcpIp_TlsHsStm_HandlesUnexpectedHsInformation(TlsConIdx);
  }

} /* TcpIp_Tls12ClientHsStm_ParseServerFinished() */

/**********************************************************************************************************************
 *  TcpIp_Tls12ClientHsStm_FinalizeHandshake
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12ClientHsStm_FinalizeHandshake(TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferTxAdHsStartIdxOfTlsBufferMgmtType txAdHsStartIdx
    = TcpIp_GetTlsBufferTxAdHsStartIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);
  TcpIp_TlsBufferTxAdHsReadIdxOfTlsBufferMgmtDynType txAdHsReadIdx
    = TcpIp_GetTlsBufferTxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  TcpIp_TlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDynType txAdHsWriteIdx
    = TcpIp_GetTlsBufferTxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

  TcpIp_TlsBufferRxAdHsStartIdxOfTlsBufferMgmtType rxAdHsStartIdx
    = TcpIp_GetTlsBufferRxAdHsStartIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);
  TcpIp_TlsBufferRxAdHsReadIdxOfTlsBufferMgmtDynType rxAdHsReadIdx
    = TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* The TX Read index is not used during handshake, so set it to the current position of the write index. */
  TcpIp_SetTlsBufferTxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, txAdHsWriteIdx);

  if (TcpIp_IsTlsClearBufferAfterHandshakeOfTlsConfig(TCPIP_TLSCONFIG_IDX) == TRUE)
  {
    /* #10 Clear the TLS TxBuffer if configured */
    if (   (txAdHsStartIdx < TcpIp_GetSizeOfTlsBufferTx())
        && (txAdHsReadIdx < TcpIp_GetSizeOfTlsBufferTx())
        && (txAdHsStartIdx <= txAdHsReadIdx))
    {
      /* Clear the handshake data in the TxBuffer from StartIdx to current ReadIdx / WriteIdx */
      TcpIp_TlsCoreBuffer_ClearTxBuffer(txAdHsStartIdx, txAdHsReadIdx);
    }

    /* #20 Clear the TLS RxBuffer if configured */
    if (   (rxAdHsStartIdx < TcpIp_GetSizeOfTlsBufferRx())
        && (rxAdHsReadIdx < TcpIp_GetSizeOfTlsBufferRx())
        && (rxAdHsStartIdx <= rxAdHsReadIdx))
    {
      /* Clear the handshake data in the RxBuffer from StartIdx to current ReadIdx */
      /* retVal can't be processed here, but a Det error is reported inside the function */
      (void)TcpIp_TlsCoreBuffer_ClearRxBuffer(rxAdHsStartIdx, rxAdHsReadIdx);
    }
  }

  /* Reset RxBuffer read indices to the start index of the buffer. */
  TcpIp_SetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx,
    TcpIp_GetTlsBufferRxAdHsStartIdxOfTlsBufferMgmt(tlsBufferMgmtIdx));
  TcpIp_SetTlsBufferRxUlRxIndIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx,
    TcpIp_GetTlsBufferRxAdHsStartIdxOfTlsBufferMgmt(tlsBufferMgmtIdx));
  /* The Rx Write index is not used during handshake,
   * so set it to the current position of the read index = start index */
  TcpIp_SetTlsBufferRxAdHsWriteIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx,
    TcpIp_GetTlsBufferRxAdHsStartIdxOfTlsBufferMgmt(tlsBufferMgmtIdx));

  /* #30 Set the connection state to established and inform the upper layer via the TcpConnected callout */
  TcpIp_SetConnectionStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CONNECTION_ESTABLISHED);
  /* #40 Handshake is done - set state accordingly */
  TcpIp_SetHandshakeStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_HANDSHAKE_DONE);

  /* For the TLS 1.2 client the decryption is blocked when the ServerFinished is received
   * (see TcpIp_Tls12CoreRl_ProcessHsFrame()).
   * This must be done since the Server is allowed to start sending application data immediately when the Finished
   * message was send. As client however the Finished must be validated before application data can be processed.
   * Therefore incoming messages are not decrypted and therefore not processed till now. */
  TcpIp_SetBlockDecryptOperationsUntilKeysAreCalculatedOfTlsConnectionDyn(TlsConIdx, FALSE);

  TcpIp_TlsCoreUl_TcpConnected(TlsConIdx);


#   if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
  /* #50 Remove all loaded certificates by overwriting them with zeros. The root certificate ID was already stored after parsing the ServerCertificate message. */
  TcpIp_TlsCore_RemoveLoadedCerts(TlsConIdx);
#   endif
} /* TcpIp_Tls12ClientHsStm_FinalizeHandshake() */

#   define TCPIP_STOP_SEC_CODE
#   include "TcpIp_MemMap.h"                                                                                            /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/

#  endif /* (TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON) */
# endif /* (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON) */
#endif /*(TCPIP_SUPPORT_ASRTLS == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Tls12Client.c
 *********************************************************************************************************************/
