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
/*!        \file  TcpIp_Tls12Core.c
 *        \brief  Transport Layer Security (TLS) 1.2 core functions implementation.
 *
 *      \details  This file implements the core routines of the TLS 1.2 stack, which are used by both of the submodule,
 *                the Tls12Client and the Tls12Server. This file is part of the TcpIp TLS submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/

#define TCPIP_TLS12CORE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "TcpIp_Cfg.h"

#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
# if(TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON)
#  include "TcpIp_Tls.h"
#  include "TcpIp_Tls12Core.h"
#  include "TcpIp_TlsCoreCommon.h"
#  include "TcpIp_TlsPriv.h"

#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
#   include "TcpIp_TlsServer.h"
#  endif /* (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON) */
#  if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
#   include "TcpIp_Tls12Client.h"
#   include "TcpIp_TlsClientCommon.h"
#  endif /* (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON) */

#  include "TcpIp_Types.h"
#  include "TcpIp_Lcfg.h"
#  include "TcpIp_Tcp.h"
#  include "TcpIp_Tcp_Cbk.h"
#  include "Csm.h"
#  include "vstdlib.h"

#  if (TCPIP_DEV_ERROR_REPORT == STD_ON)
#   include "Det.h"
#  endif /* (TCPIP_DEV_ERROR_REPORT == STD_ON) */
#  if (TCPIP_SUPPORT_ASRTLS_VERSION_1_3 == STD_ON)
#   include "TcpIp_Tls13Core.h"
#  endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
#  if (defined __VCA__ && defined __SMOKETEST_VCA__)
#   warning "VCA was executed with __SMOKETEST_VCA__"                                                                   /* PRQA S 3115 */ /* MD_TCPIP_Rule20.13_3115 */
#  endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
#  if !defined (TCPIP_LOCAL)
#   define TCPIP_LOCAL static
#  endif

#  if !defined (TCPIP_LOCAL_INLINE)
#   define TCPIP_LOCAL_INLINE LOCAL_INLINE
#  endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#  define TCPIP_START_SEC_CONST_8
#  include "TcpIp_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* PRQA S 3218 3 */ /* MD_TCPIP_Rule8.9_3218 */
TCPIP_LOCAL CONST(uint8, TCPIP_CONST) TcpIp_Tls_Hs_MasterSecretLabel[] = "master secret";
TCPIP_LOCAL CONST(uint8, TCPIP_CONST) TcpIp_Tls_Hs_KeyExpansionLabel[] = "key expansion";


#  define TCPIP_STOP_SEC_CONST_8
#  include "TcpIp_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
#  define TCPIP_START_SEC_CONST_8
#  include "TcpIp_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

CONST(uint8, TCPIP_CONST) TcpIp_Tls_Hs_FinishedLabel[2][TCPIP_TLS_LABEL_FINISHED_LEN + 1u] = {
                              "server finished",
                              "client finished"};

#  define TCPIP_STOP_SEC_CONST_8
#  include "TcpIp_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#  define TCPIP_START_SEC_CODE
#  include "TcpIp_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  TLS CORE UPPER LAYER (UL)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TCPIP TLS INIT FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE CONTENT TYPE (CT)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE CHANGE CIPHER SPEC
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE CT ALERT
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE RECORD LAYER (RL)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_ProcessHsClientHelloAfterHandshake
 *********************************************************************************************************************/
/*! \brief         Process the Client Hello Frame received after initial handshaking
 *  \details       Check if the Client Hello frame has the right format and if so set no_renegotiation alert
 *  \param[in]     TlsConIdx          TLS connection index
 *                                    \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12CoreRl_ProcessHsClientHelloAfterHandshake(
  TcpIp_TlsConnectionIterType TlsConIdx);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_ProcessHsHelloRequest
 *********************************************************************************************************************/
/*! \brief         Main function for processing the received TLS HelloRequest frames
 *  \details       -
 *  \param[in]     TlsConIdx          TLS connection index
 *                                    \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     HsLen              The handshake message length
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12CoreRl_ProcessHsHelloRequest(
  TcpIp_TlsConnectionIterType TlsConIdx,
  uint32 HsLen);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_DecryptCbcCheckBlocksizeAlignment
 *********************************************************************************************************************/
/*! \brief         Check the input data length of the AES-128-CBC operation to its alignment of a multiple of the used block size
 *  \details       -
 *  \param[in]     TlsConIdx          TLS connection index
 *                                    \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     DataLen            Length of TCP buffer segment
 *  \return        E_OK               Data length aligned to the block size
 *  \return        E_NOT_OK           Data length NOT aligned to the block size
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_DecryptCbcCheckBlocksizeAlignment(
  TcpIp_TlsConnectionIterType TlsConIdx,
  uint16 DataLen);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_RxCipherDecryptAes128Cbc
 *********************************************************************************************************************/
/*! \brief         Decrypt a complete TLS frame using Aes128 Cbc that is present in the provided TCP buffer segments
 *  \details       -
 *  \param[in]     TlsConIdx          TLS connection index
 *                                    \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     ContentType        TLS ContentType of this frame
 *  \param[in]     DataPtr            TCP buffer segment containing full or second part of TLS frame
 *  \param[in]     DataLen            Length of TCP buffer segment
 *  \return        E_OK               Operation successful
 *  \return        E_NOT_OK           Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_RxCipherDecryptAes128Cbc(
  TcpIp_TlsConnectionIterType  TlsConIdx,
  uint8                        ContentType,
  TCPIP_P2C(uint8)             DataPtr,
  uint16                       DataLen);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_RxCipherDecryptAEADBlock128SetIv
 *********************************************************************************************************************/
/*! \brief         Extract the AEAD Block128 IV from TCP buffer and set Csm keys for decryption
 *  \details       -
 *  \param[in]     TlsConIdx          TLS connection index
 *                                    \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     ExplicitIVSegPtr   Pointer to the buffer segment containing the explicit IV
 *  \return        E_OK               IV set successful
 *  \return        E_NOT_OK           IV setting was NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_RxCipherDecryptAEADBlock128SetIv(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_ConstSegmentPtrType   ExplicitIVSegPtr);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_SetIvForDecryptCall
 *********************************************************************************************************************/
/*! \brief         Set the initialization vector (IV) for the AES operation
 *  \details       -
 *  \param[in]     TlsConIdx          TLS connection index
 *                                    \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     DataSegPtr         Pointer to the buffer segment where the IV is located
 *  \return        E_OK               IV set successful
 *  \return        E_NOT_OK           IV setting was NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_SetIvForDecryptCall(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_ConstSegmentPtrType   DataSegPtr);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcStreamingMode
 *********************************************************************************************************************/
/*! \brief         Handles AesCbc128 encryption of content data
 *  \details       -
 *  \param[in]     TlsConIdx          TLS connection index
 *                                    \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] Seg1Ptr            The optional stored buffer
 *  \param[in,out] Seg2Ptr            The received data
 *  \return        E_OK               Operation successful
 *  \return        E_NOT_OK           Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcStreamingMode(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        Seg1Ptr,
  TcpIp_SegmentPtrType        Seg2Ptr);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithMultiCall
 *********************************************************************************************************************/
/*! \brief         AesCbc128 encryption of content data with TCP buffer wraparound
 *  \details       Handles AesCbc128 encryption of content data when there is TCP buffer wraparound
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg1     Pointer to the first buffer segment
 *  \param[in,out] TcpSeg2     Pointer to the second buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithMultiCall(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType  TcpSeg1,
  TcpIp_SegmentPtrType  TcpSeg2);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPadding
 *********************************************************************************************************************/
/*! \brief        Aes128 Cbc encryption of Mac and Padding
 *  \details      Performs Aes128 Cbc encryption of Mac and Padding into Tls temp buffer
 *                into the temp buffer
 *  \param[in]    TlsConIdx   TLS connection index
 *                            \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[out]   EncDataLen  Length of data which was encrypted
 *  \return       E_OK        Operation successful
 *  \return       E_NOT_OK    Operation NOT successful
 *  \pre          Mac is already available in the Tls temp buffer
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPadding(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(uint32)           EncDataLen);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithSingleCall
 *********************************************************************************************************************/
/*! \brief         Aes128Cbc encryption with NO TcpIp wraparound
 *  \details       Performs Aes128Cbc encryption of content data, mac and padding
 *                 when there is no TcpIp buffer wraparound
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg      Pointer to the buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           Content data is available in Tls Tx buffer
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithSingleCall(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpSeg);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWoWraparound
 *********************************************************************************************************************/
/*! \brief         Aes128 Cbc encryption of content with linear TcpIp buffer (NO wraparound)
 *  \details       his function handles Aes128 Cbc encryption of the Rl content data
 *                 when the TcpIp buffer is linear
 *  \param[in]     TlsConIdx   TLS connection index
 *  \param[in,out] TcpSeg      Pointer to the buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           Content data is available in Tls Tx buffer
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWoWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpSeg);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWithTcpWraparound
 *********************************************************************************************************************/
/*! \brief         Aes128 Cbc encryption of content with TcpIp wraparound
 *  \details       This function handles Aes128 Cbc encryption of the Rl content data
 *                 when the TcpIp buffer wraps around
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg1     Pointer to the first buffer segment
 *  \param[in,out] TcpSeg2     Pointer to the second buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWithTcpWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcAdHs
 *********************************************************************************************************************/
/*! \brief         Aes128 Cbc encryption of the AdHs content data
 *  \details       Performs Aes128 Cbc encryption of AdHs content data
 *                 in the Tls buffer into a linear Tcp buffer
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg      Pointer to the buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHs(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpSeg);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcAl
 *********************************************************************************************************************/
/*! \brief         Aes128 Cbc encryption of the Alert content data
 *  \details       Performs Aes128 Cbc encryption of Alert content data
 *                 in the Tls buffer into a linear TcpIp buffer
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg      Pointer to the buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAl(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpSeg);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcAlWithTcpWraparound
 *********************************************************************************************************************/
/*! \brief         Aes128 CBC encryption of the alert data
 *  \details       -
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg1     Pointer to the first buffer segment
 *  \param[in,out] TcpSeg2     Pointer to the second buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAlWithTcpWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreCrypto_GenerateAesIv
 *********************************************************************************************************************/
/*! \brief         Generate AES initialization vector (IV)
 *  \details       -
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] AesIvSeg    Pointer to the buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreCrypto_GenerateAesIv(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        AesIvSeg);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPaddingInt
 *********************************************************************************************************************/
/*! \brief         Encrypt the MAC and the padding and store it in the target buffer
 *  \details       Feed the Mac into the already running Aes128 Cbc streamer
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg      Pointer to the buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           Mac is available in Tls temp buffer
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPaddingInt(
  TcpIp_TlsConnectionIterType  TlsConIdx,
  TcpIp_SegmentPtrType         TcpSeg);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_TxCipherGenAndCopyAes128CbcIv
 *********************************************************************************************************************/
/*! \brief         Iv generation and Aes128Cbc encryption with TcpIp wraparound
 *  \details       The function handles Iv generation and Aes128Cbc encryption
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg1     Pointer to the first buffer segment
 *  \param[in,out] TcpSeg2     Pointer to the second buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_TxCipherGenAndCopyAes128CbcIv(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType  TcpSeg1,
  TcpIp_SegmentPtrType  TcpSeg2);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcContentData
 *********************************************************************************************************************/
/*! \brief         Aes128Cbc encryption of content data with TcpIp wrap around
 *  \details       Handles Aes128Cbc encryption of content data when there is TcpIp wrap around
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg1     Pointer to the first buffer segment
 *  \param[in,out] TcpSeg2     Pointer to the second buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcContentData(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpSeg1,
  TcpIp_SegmentPtrType        TcpSeg2);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTcpWraparound
 *********************************************************************************************************************/
/*! \brief         Aes128Cbc encryption of AdHs content for TcpIp wraparound
 *  \details       This functions handles Aes128Cbc encryption of AdHs content data when
 *                 there is TcpIp buffer wraparound
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg1     Pointer to the first buffer segment
 *  \param[in,out] TcpSeg2     Pointer to the second buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTcpWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithoutTlsWraparound
 *********************************************************************************************************************/
/*! \brief         Aes128Cbc encryption of AdHs content with TcpIp wraparound and no Tls wraparound
 *  \details       Handles Aes128Cbc encryption of AdHs content when the
 *                 TcpIp wraps around and Tls buffer is linear
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg1     Pointer to the first buffer segment
 *  \param[in,out] TcpSeg2     Pointer to the second buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithoutTlsWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparound
 *********************************************************************************************************************/
/*! \brief         Aes128Cbc encryption of AdHs content with TcpIp and Tls wraparound
 *  \details       Handles Aes128Cbc encryption of AdHs content when the Tls and
 *                 TcpIp buffer wraps around
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg1     Pointer to the first buffer segment
 *  \param[in,out] TcpSeg2     Pointer to the second buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario1
 *********************************************************************************************************************/
/*! \brief         Aes128Cbc encryption of AdHs content with TLS wraparound before or at TCP wraparound
 *  \details       Handles Aes128Cbc encryption of AdHs content when TLS wraps around before or at TCP wraparound
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg1     Pointer to the first buffer segment
 *  \param[in,out] TcpSeg2     Pointer to the second buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
  *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario1(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2);

/**********************************************************************************************************************
 * TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario2
 *********************************************************************************************************************/
/*! \brief         Aes128Cbc encryption of AdHs content with TLS wraparound after TCP wraparound
 *  \details       Handles Aes128Cbc encryption of AdHs content when TLS wraps around after TCP wraparound
 *  \param[in]     TlsConIdx   TLS connection index
 *                             \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in,out] TcpSeg1     Pointer to the first buffer segment
 *  \param[in,out] TcpSeg2     Pointer to the second buffer segment
 *  \return        E_OK        Operation successful
 *  \return        E_NOT_OK    Operation NOT successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario2(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2);

/**********************************************************************************************************************
 *  TLS CORE BUFFER
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE STATES AND EVENTS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE MAIN FUNCTIONS
 *********************************************************************************************************************/

#  if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCtx_SetPublicKeyPtrAndLength
 *********************************************************************************************************************/
/*! \brief           Extract public key and if applicable private key information
 *  \details         Is called in asynchronous handler for calculation of the Diffie-Hellman shared secret
 *  \param[in]       TlsConIdx            TLS connection index
 *                                        CONSTRAINT: [0..TcpIp_GetSizeOfTlsConnection()-1]
 *  \param[in,out]   PublicKeyPtrPtr      Pointer to the pointer of the public key
 *  \param[in,out]   PublicKeyLenPtr      Pointer where the length of the public key is saved
 *  \param[in,out]   EcdhPubKeyBufPtr     Pointer to the ECDH public key buffer
 *  \param[in,out]   EcdhPubKeyBufLenPtr  Pointer to the length of the ECDH public key buffer
 *  \pre             -
 *  \context         TASK (only called in the context of the TLS background task TcpIp_Tls_MainFunctionLowPrio())
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCtx_SetPublicKeyPtrAndLength(
  TcpIp_TlsConnectionIterType   TlsConIdx,
  TCPIP_P2V(TcpIp_Uint8P2C)     PublicKeyPtrPtr,
  TCPIP_P2V(uint32)             PublicKeyLenPtr,
  TCPIP_P2V(uint8)              EcdhPubKeyBufPtr,
  TCPIP_P2V(uint32)             EcdhPubKeyBufLenPtr);

#   if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCtx_GetPrivateKeyLength
 *********************************************************************************************************************/
 /*! \brief           Get the length of the private key.
  *  \details         The length of the private key depends on the used curve. This method can be called to get
  *                   the length of the private key that corresponds to the currently used curve (e.g. SECP256R1)
  *  \param[in]       ActiveCurveId            ID of the active curve used for the handshake
  *  \param[out]      PrivateKeyLengthPtr      The length of the private key is returned. If no length can be
  *                                            determined, zero is returned for the length value.
  *  \return          E_OK     if the length could be set
  *  \return          E_NOT_OK if the current curve was unknown and therefore no length was set
  *  \pre             -
  *  \context         TASK
  *  \reentrant       FALSE
  *  \synchronous     TRUE
  *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCtx_GetPrivateKeyLength(
  TcpIp_TlsEllipticCurveIdType ActiveCurveId,
  TCPIP_P2V(uint8) PrivateKeyLengthPtr);
#   endif /* (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON) */
#  endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/***********************************************************************************************************************
 *  TLS CORE STATE AND EVENT
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  TLS ERROR
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE CHANGE PARAMETER
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE CRYPTO
 *********************************************************************************************************************/
/***********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_PSha256
 **********************************************************************************************************************/
/*! \brief         function generating random value using HMAC with SHA256
 *  \details       -
 *  \param[in]     TlsConIdx           TLS connection index
 *                                     \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]      ResultKeyIdPtr     CSM key where the output data shall be written to
 *  \param[in]      SecretKeyIdPtr     CSM key where the secret is stored
 *  \param[in]      SeedPtr            pointer to seed
 *  \param[in]      seedLen            length of seed
 *  \param[in]      ResultIsPrivate    TRUE: result key is private; FALSE: Result key is public
 *  \return         E_OK     if random could be generated
 *  \return         E_NOT_OK if random could NOT be generated
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_PSha256(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2C(TcpIp_TlsCsmKeyInfoType) ResultKeyIdPtr,
  TCPIP_P2C(TcpIp_TlsCsmKeyInfoType) SecretKeyIdPtr,
  TCPIP_P2C(uint8) SeedPtr,
  uint16 SeedLen,
  boolean ResultIsPrivate);

/***********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_CalcKeyBlock
 **********************************************************************************************************************/
/*! \brief         TLS crypto function to calculate the key block
 *  \details       -
 *  \param[in]     TlsConIdx          TLS connection index
 *                                    \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \return        E_OK               Key block was calculated successful
 *  \return        E_NOT_OK           Key block could NOT be calculated successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_CalcKeyBlock(
  TcpIp_TlsConnectionIterType TlsConIdx);

/***********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_GenerateIvRandomValue
 **********************************************************************************************************************/
/*! \brief         TLS crypto function to generate the AES IV random value
 *  \details       -
 *  \param[in]     TlsConIdx   TLS connection index
 *  \param[in,out] IvSeg       Pointer to the buffer segment
 *  \param[in]     IvLen       Length of the IV
 *  \return        E_OK        Random IV generation successful
 *  \return        E_NOT_OK    Random IV could NOT be generated successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_GenerateIvRandomValue(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        IvSeg,
  CONST(uint8, AUTOMATIC)     IvLen);

/***********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_SetAesIvValue
 **********************************************************************************************************************/
/*! \brief         TLS crypto function to set the AES IV random value
 *  \details       -
 *  \param[in]     TlsConIdx          TLS connection index
 *                                    \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     IvKeyElementId     ElementId which should be set
 *  \param[in]     IvPtr              Pointer to the IV
 *  \param[in]     IvLen              Length of the IV
 *  \return        E_OK               IV was set successful
 *  \return        E_NOT_OK           IV could NOT be set successful
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_SetIvValue(
  TcpIp_TlsConnectionIterType TlsConIdx,
  CONST(uint32, AUTOMATIC) IvKeyElementId,
  TCPIP_P2C(uint8) IvPtr,
  CONST(uint8, AUTOMATIC) IvLen);

/**********************************************************************************************************************
 *  TLS ALERT
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE MESSAGE
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE UTIL  (HELPER FUNCTIONS)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE LOWER LAYER (LL)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE UTIL  (HELPER FUNCTIONS)
 *********************************************************************************************************************/
#  if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/***********************************************************************************************************************
 *  TcpIp_Tls12CoreUtil_GetSigAlgAndCurveFromCertificate
 *********************************************************************************************************************/
/*! \brief         Get the used signature algorithm and curve from the certificate with the passed Id
 *  \details       -
 *  \param[in]     CertId                Id of the certificate
 *  \param[out]    SignAlgIdPtr          Id of the signature algorithm which is used for the certificate's signature
 *  \param[out]    CurveIdPtr            Id of the curve which is used by the certificate
 *  \return        E_OK                  Curve and signature algorithm have been successfully extracted
 *  \return        E_NOT_OK              Certificate has invalid status or inconsistent values
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreUtil_GetSigAlgAndCurveFromCertificate(
  TcpIp_CertificateIdType                   CertId,
  TCPIP_P2V(TcpIp_SignatureAlgorithmIdType) SignAlgIdPtr,
  TCPIP_P2V(TcpIp_TlsEllipticCurveIdType)   CurveIdPtr);

/***********************************************************************************************************************
 *  TcpIp_Tls12CoreUtil_CheckIfSigAlgFitsToCurve
 *********************************************************************************************************************/
/*! \brief         Checks if the passed signature algorithm and the passed curve fit together
 *  \details       -
 *  \param[in]     SignAlgId             Id of the signature algorithm
 *  \param[in]     CurveId               Id of the curve
 *  \return        E_OK                  Signature algorithm and curve fit together
 *  \return        E_NOT_OK              Signature algorithm and curve do NOT fit together
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreUtil_CheckIfSigAlgFitsToCurve(
  TcpIp_SignatureAlgorithmIdType SignAlgId,
  TcpIp_TlsEllipticCurveIdType   CurveId);
#  endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

#  if (TCPIP_SUPPORT_ASRTLS_SECRETS_NSS_FRAME == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCtx_SendMasterSecretViaNssFrame
 *********************************************************************************************************************/
/*! \brief         Sends one NSS Frame of the TLS 1.2 master secret.
 *  \details       -
 *  \param[in]     TlsConIdx                    TLS connection index
 *                                              \spec requires TlsConIdx < TcpIp_GetSizeOfTlsConnection(); \endspec
 *  \param[in]     MasterSecretCsmKeyId         The keyId of the master secret which should be sent via NSS Frame
 *  \param[in]     MasterSecretCsmKeyElementId  The elementId of the key
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCtx_SendMasterSecretViaNssFrame(
  TcpIp_TlsConnectionIterType                   TlsConIdx,
  TcpIp_CsmKeyIdType                            MasterSecretCsmKeyId,
  TcpIp_HsCommonKeyElementIdOfTlsHandshakeType  MasterSecretCsmKeyElementId);
#  endif /* (TCPIP_SUPPORT_ASRTLS_SECRETS_NSS_FRAME == STD_ON) */

#  define TCPIP_STOP_SEC_CODE
#  include "TcpIp_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#  define TCPIP_START_SEC_CODE
#  include "TcpIp_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  TLS CORE UPPER LAYER (UL)
 *********************************************************************************************************************/
#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12CoreUl_TcpAccepted
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_Tls12CoreUl_TcpAccepted(TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SocketDynIterType socketIdx = TcpIp_GetSocketDynComIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_SocketOwnerConfigIterType socketOwnerCfgIdx = TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(socketIdx);

  TcpIp_SocketTcpDynIterType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(
    TcpIp_GetSocketDynListenIdxOfTlsConnectionDyn(TlsConIdx));

  TcpIp_SocketTcpDynIterType connectedSocketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(socketIdx);
  TCPIP_P2V(TcpIp_SockAddrType) remoteSockAddrPtr = &TcpIp_GetAddrRemSockOfSocketDyn(socketIdx)->TcpIpSockAddr;

  /* ----- Implementation ----------------------------------------------- */
  if (   (socketIdx < TcpIp_GetSizeOfSocketDyn())
      && (socketOwnerCfgIdx < TcpIp_GetSizeOfSocketOwnerConfig())
      && (socketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn())
      && (connectedSocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn()))
  {
    /* #10 Forward call to TcpIp */
    TcpIp_Tcp_TlsAccepted(socketOwnerCfgIdx, socketTcpIdx, connectedSocketTcpIdx, remoteSockAddrPtr);

    /* AUTOSAR requires additional event notification to the upper layer */
    TcpIp_Tls_TcpIpEventCallout(socketOwnerCfgIdx, socketIdx, TCPIP_TLS_HANDSHAKE_SUCCEEDED);
  }
  else
  {
    /* Memory runtime check fails - report DET */
    TcpIp_Tls_CallDetReportError(TCPIP_TLS_API_ID_TLS_DATA_PROCESSING, TCPIP_TLS_E_INV_RUNTIME_CHECK);
  }

} /* TcpIp_Tls12CoreUl_TcpAccepted() */
#  endif

/**********************************************************************************************************************
 *  TLS CORE CHANGE PARAMETER
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE CONTENT TYPE (CT)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE CT CHANGE CIPHER SPEC
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE CT ALERT
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE RECORD LAYER (RL)
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_ProcessHsClientHelloAfterHandshake
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12CoreRl_ProcessHsClientHelloAfterHandshake(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
#  if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
#   if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
  if (TcpIp_GetModeOfTlsConnection(TlsConIdx) == TCPIP_TLS_CLIENT)
#   endif
  {
    /* Mode == TCPIP_TLS_CLIENT */
    /* TLS Client should never receive this message - trigger alert */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
  }
#   if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
  else
#   endif
#  endif
#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
  {
    /* Mode == TCPIP_TLS_SERVER */
    /* RFC5246 - 7.2.2.  no_renegotiation */
    /*     A client hello after initial handshaking would normally lead to renegotiation;
     *     when that is not appropriate, the recipient should respond with this alert.
     *     At that point, the original requester can decide whether to proceed with the connection.
     */
    /* #10 Set the no_renegotiation alert */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_WARNING, TCPIP_TLS_ALERT_DESC_NO_RENEGOT);
  }
#  endif
} /* TcpIp_Tls12CoreRl_ProcessHsClientHelloAfterHandshake() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_ProcessHsHelloRequest
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050, 6080 1 */ /* MD_TcpIp_STCAL_CslAccess, MD_TcpIp_TlsStatemachine_STMIF_6080 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_Tls12CoreRl_ProcessHsHelloRequest(
  TcpIp_TlsConnectionIterType TlsConIdx,
  uint32 HsLen)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Validate the possible HelloRequest frame and forward it to server/client specific handling */
#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
#   if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
  if (TcpIp_GetModeOfTlsConnection(TlsConIdx) == TCPIP_TLS_SERVER)
#   endif
  {
    /* Mode == TCPIP_TLS_SERVER */
    /* TLS Server should never receive this message - trigger alert */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);

    TCPIP_DUMMY_STATEMENT_CONST(HsLen);                                                                                 /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
  }
#   if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
  else
#   endif
#  endif
#  if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
  {
    /* Mode == TCPIP_TLS_CLIENT */
    /* RFC5246 - 7.4.1.1.  Hello Request */
    /*     This message will be ignored by the client if the client is
     *     currently negotiating a session.  This message MAY be ignored by
     *     the client if it does not wish to renegotiate a session, or the
     *     client may, if it wishes, respond with a no_renegotiation alert.
     */
    if (HsLen != 0u)
    {
      /* #20 invalid TLS HelloRequest frame - trigger alert */
      TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_DECODE_ERROR);
    }
    else
    {
      /* valid HelloRequest message - ignore it */
    }
  }
#  endif
} /* TcpIp_Tls12CoreRl_ProcessHsHelloRequest() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_TxCipherEncryptAes128Cbc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_TxCipherEncryptAes128Cbc(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        Seg1Ptr,
  TcpIp_SegmentPtrType        Seg2Ptr)
{
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_RlTxFragmentLenOfTlsBufferMgmtDynType requestedRlTxLen = TcpIp_GetRlTxFragmentLenOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;
  /* Calculate the total size of the TCP buffer segments */
  uint32 totalTcpBufLen = Seg2Ptr->AvailLen + Seg1Ptr->AvailLen;

  /* #10 Check if provided TCP buffer(s) has sufficient space and trigger Aes128 Cbc streaming */
  if (totalTcpBufLen < requestedRlTxLen)
  {
    /* The provided TCP buffer segment is not sufficient, store this buffer into DataSplit struct for next call */
    TcpIp_GetAddrTxDataSplitOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)->DataPtr = Seg2Ptr->AvailPtr;
    TcpIp_GetAddrTxDataSplitOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)->DataLen = (uint16)Seg2Ptr->AvailLen;
    retVal = BUFREQ_OK;
  }
  else
  {
    /* Sufficient TCP buffer available, trigger Aes128 Cbc streaming */
    if (TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcStreamingMode(TlsConIdx, Seg1Ptr, Seg2Ptr) == E_OK)
    {
        retVal = BUFREQ_OK;
    }
  }

  return retVal;
} /* TcpIp_Tls12CoreRl_TxCipherEncryptAes128Cbc() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithMultiCall
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithMultiCall(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType  TcpSeg1,
  TcpIp_SegmentPtrType  TcpSeg2)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType        retVal = E_NOT_OK;
  uint32                copiedLen = 0u;
  uint8                 macLen = TcpIp_TlsCoreUtil_GetTxCipherMacLen(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Generate Iv and copy into TCP buffer */
  if (TcpIp_Tls12CoreRl_TxCipherGenAndCopyAes128CbcIv(TlsConIdx, TcpSeg1, TcpSeg2) == E_OK)
  {
    /* #20 Start Aes128 Cbc streaming of content data and copy into TCP buffer */
    if (TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcContentData(TlsConIdx, TcpSeg1, TcpSeg2) == E_OK)
    {
      /* #30 Encrypt the Mac and padding, copy into TCP buffer, and stop Aes128 Cbc streaming */
      if (TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPadding(TlsConIdx, &copiedLen) == E_OK)
      {
        /* Mac and padding is available in temp buffer */
        TCPIP_P2C(uint8) tmpBuffPtr = TcpIp_GetAddrTlsTemporaryHmacBuffer(TcpIp_GetTlsTemporaryHmacBufferStartIdxOfTlsConnection(TlsConIdx)
          + macLen);

        TcpIp_SegmentMultiTargetMemCpy(TcpSeg1, TcpSeg2, &tmpBuffPtr[0], copiedLen);

        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithMultiCall */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcStreamingMode
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcStreamingMode(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        Seg1Ptr,
  TcpIp_SegmentPtrType        Seg2Ptr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the Tcp buffer has wraparound and trigger Aes128 Cbc encryption */
  if (Seg1Ptr->AvailLen == 0u)
  {
    /* Scenario 1: Encryption with no TCP buffer wrap around */
    retVal = TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithSingleCall(TlsConIdx, Seg2Ptr);
  }
  else
  {
    retVal = TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithMultiCall(TlsConIdx, Seg1Ptr, Seg2Ptr);
  }

  return retVal;
} /* TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcStreamingMode() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPadding
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPadding(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(uint32)           EncDataLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 macLen = TcpIp_TlsCoreUtil_GetTxCipherMacLen(TlsConIdx);
  TcpIp_SegmentType TcpSeg;
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SegmentInit(&TcpSeg, TcpIp_GetAddrTlsTemporaryHmacBuffer(0), TcpIp_GetSizeOfTlsTemporaryHmacBuffer());

  /* Point to free space in the temporary buffer (after the unencrypted MAC) */
  TcpIp_SegmentCommit(&TcpSeg, TcpIp_GetTlsTemporaryHmacBufferStartIdxOfTlsConnection(TlsConIdx) + (uint32)macLen);

  /* Reset the Buffer pointer to the position after the unencrypted MAC (drops TcpSeg.Usage) */
  TcpIp_SegmentClear(&TcpSeg);

  /* #10 Encrypt the MAC and Padding and copy it into the temporary buffer */
  retVal = TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPaddingInt(TlsConIdx, &TcpSeg);

  /* Update the ciphertext length */
  *EncDataLen = TcpSeg.Usage;

  return retVal;
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPadding() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithSingleCall
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithSingleCall(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpSeg)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Generate Iv and set the CSM Key */
  retVal = TcpIp_Tls12CoreCrypto_GenerateAesIv(TlsConIdx, TcpSeg);

  retVal |= TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWoWraparound(TlsConIdx, TcpSeg);

  /* #20 Encrypt MAC and padding directly into the TCP buffer */
  retVal |= TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPaddingInt(TlsConIdx, TcpSeg);

  return retVal;
} /* TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcWithSingleCall() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_TxCipherGenAndCopyAes128CbcIv
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_TxCipherGenAndCopyAes128CbcIv(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType  TcpSeg1,
  TcpIp_SegmentPtrType  TcpSeg2)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType     retVal = E_NOT_OK;
  uint8              tmpAesIv[TCPIP_TLS_AES128_IV_LEN];
  TcpIp_SegmentType  tmpAesIvSeg;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT((TcpSeg1->Size + TcpSeg2->Size) >= TCPIP_TLS_AES128_IV_LEN);

  TcpIp_SegmentInit(&tmpAesIvSeg, &tmpAesIv[0], TCPIP_TLS_AES128_IV_LEN);

  /* #10 Generate Aes Iv and set it to the CSM Key, and copy into the TcpIp buffer */
  if (TcpIp_Tls12CoreCrypto_GenerateAesIv(TlsConIdx, &tmpAesIvSeg) == E_OK)
  {
    /* Copy the generated Iv into TcpIpBuffer, considering TcpIp buffer wraparound */
    TcpIp_SegmentMultiTargetMemCpy(TcpSeg1, TcpSeg2, &tmpAesIv[0], TCPIP_TLS_AES128_IV_LEN);

    retVal = E_OK;
  }

  return retVal;
} /* TcpIp_Tls12CoreRl_TxCipherGenAndCopyAes128CbcIv */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcContentData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcContentData(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpSeg1,
  TcpIp_SegmentPtrType        TcpSeg2)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                 retVal = E_NOT_OK;
  TcpIp_SizeOfTlsBufferMgmtType  tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  uint32                         plaintextDataLen = TcpIp_GetRlTxContentTypeLenOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the provided buffer and start Aes128 Cbc streaming of content data */
  if (TcpSeg1->AvailLen == 0u)
  {
    /* Scenario 1: Encrypt the data into the second segment */
    retVal = TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWoWraparound(TlsConIdx, TcpSeg2);
  }
  else if (TcpSeg1->AvailLen >= plaintextDataLen)
  {
    /* Scenario 2: Encrypt the data into the first segment */
    retVal = TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWoWraparound(TlsConIdx, TcpSeg1);
  }
  else
  {
    /* Scenario 3: Encrypt the data into both segments */
    retVal = TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWithTcpWraparound(TlsConIdx, TcpSeg1, TcpSeg2);
  }

  return retVal;
} /* TcpIp_Tls12CoreRl_TxCipherEncryptAes128CbcContentData */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithoutTlsWraparound
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithoutTlsWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                 csmRetVal;
  TcpIp_SizeOfTlsBufferMgmtType  tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_CsmJobIdType              encryptionJobId = TcpIp_GetCsmJobId(TcpIp_GetCsmJobIdCipherEncryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));
  TcpIp_TlsBufferTxRlSendIdxOfTlsBufferMgmtDynType rlSendTlsBufferTxIdx
                                                  = TcpIp_GetTlsBufferTxRlSendIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  TCPIP_P2C(uint8)               plaintextDataPtr = TcpIp_GetAddrTlsBufferTx(rlSendTlsBufferTxIdx);
  uint32                         plaintextDataLen = TcpIp_GetRlTxContentTypeLenOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  uint32                       availableSpaceSeg1 = TcpIpSeg1->AvailLen;
  uint32                     contentDataOffset;
  uint32                     contentDataRemaining = plaintextDataLen;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Encrypt the content data present in TLS buffer into the provided Tcp buffer segments */
  /* Encrypt plaintext in TLS Tx buffer that fits into the TCP segment 1 */
  csmRetVal = TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_STREAMSTART,                                      /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
    plaintextDataPtr, TcpIpSeg1->AvailLen,
    TcpIpSeg1->AvailPtr, &availableSpaceSeg1);

  /* The variable availableSpaceSeg1 now contains the length of encrypted data generated so far. */
  contentDataOffset = TcpIpSeg1->AvailLen;
  contentDataRemaining -= TcpIpSeg1->AvailLen;

  TcpIp_SegmentCommit(TcpIpSeg1, availableSpaceSeg1);

  /* Encrypt 32 byte chunks of data until the first segment is filled or all content is consumed */
  while (   (TcpIpSeg1->AvailLen > 0u)
         && (contentDataRemaining > 0u))
  {
    uint8 tmpBuffer[TCPIP_TLS_BLOCK128_BLOCKSIZE * 2u];
    uint8 tmpBufLen = (uint8)(TCPIP_MIN(contentDataRemaining, (TCPIP_TLS_BLOCK128_BLOCKSIZE * 2u)));
    uint32 availableSpaceTmpBuf = TCPIP_TLS_BLOCK128_BLOCKSIZE * 2u;

    /* Encrypt the data into a temp buffer */
    csmRetVal |= TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_UPDATE, &plaintextDataPtr[contentDataOffset], tmpBufLen, &tmpBuffer[0], &availableSpaceTmpBuf);
    contentDataOffset += tmpBufLen;
    contentDataRemaining -= tmpBufLen;

    /* Copy the encrypted data into the segments */
    TcpIp_SegmentMultiTargetMemCpy(TcpIpSeg1, TcpIpSeg2,  &tmpBuffer[0], availableSpaceTmpBuf);
  }

  /* If more data is remaining, encrypt directly to the second segment */
  if (contentDataRemaining > 0u)
  {
    uint32 availableSpaceSeg2 = TcpIpSeg2->AvailLen;

    csmRetVal |= TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_UPDATE,                                        /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
      &plaintextDataPtr[contentDataOffset], contentDataRemaining,
      TcpIpSeg2->AvailPtr, &availableSpaceSeg2);

    TcpIp_SegmentCommit(TcpIpSeg2, availableSpaceSeg2);
  }

  return csmRetVal;
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithoutTlsWraparound */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparound
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsBufferTxRlSendIdxOfTlsBufferMgmtDynType  rlSendIdx =
    TcpIp_GetTlsBufferTxRlSendIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  TcpIp_TlsBufferTxAdHsEndIdxOfTlsBufferMgmtType   txBufferEndIdx =
    TcpIp_GetTlsBufferTxAdHsEndIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);
  /* Length of TLS segment 1 */
  uint32  tlsSeg1Len = ((uint32)txBufferEndIdx - rlSendIdx);
  Std_ReturnType csmRetVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Depending on where the wraparound is, invoke the encrypt function */
  if (tlsSeg1Len <= TcpIpSeg1->AvailLen)
  {
    /* TLS wraparound before or at TCP wraparound */
    csmRetVal = TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario1(TlsConIdx, TcpIpSeg1, TcpIpSeg2);
  }
  else
  {
    /* TLS wraparound after TCP wraparound */
    csmRetVal = TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario2(TlsConIdx, TcpIpSeg1, TcpIpSeg2);
  }

  return csmRetVal;
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparound */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario1
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010, 6030, 6050 1 */ /* MD_TcpIp_TlsStreaming_STPTH_6010, MD_TcpIp_TlsStreaming_STCYC_6030, MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario1(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx
    = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);

  TcpIp_CsmJobIdType encryptionJobId
    = TcpIp_GetCsmJobId(TcpIp_GetCsmJobIdCipherEncryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));

  TcpIp_TlsBufferTxRlSendIdxOfTlsBufferMgmtDynType rlSendTlsBufferTxIdx
    = TcpIp_GetTlsBufferTxRlSendIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

  TcpIp_TlsBufferTxAdHsEndIdxOfTlsBufferMgmtType endTlsBufferTxIdx
    = TcpIp_GetTlsBufferTxAdHsEndIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);

  TcpIp_TlsBufferTxAdHsEndIdxOfTlsBufferMgmtType startTlsBufferTxIdx
    = TcpIp_GetTlsBufferTxAdHsStartIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);

  uint32 plaintextRemainingLen = TcpIp_GetRlTxContentTypeLenOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  uint32 plaintextConsumedTlsSeg2Len = 0u;                                                                              /* PRQA S 2981 */ /* MD_TCPIP_Rule2.2_RedundantAssignment */
  TCPIP_P2C(uint8) tlsSeg1Ptr = TcpIp_GetAddrTlsBufferTx(rlSendTlsBufferTxIdx);
  uint32  tlsSeg1Len = ((uint32)endTlsBufferTxIdx - rlSendTlsBufferTxIdx);
  TCPIP_P2C(uint8) tlsSeg2Ptr;
  uint32 ciphertextOutBufLen;

  Std_ReturnType csmRetVal;
  /* ----- Implementation ----------------------------------------------- */
  /* TLS wraparound before or at TCP wraparound */
  TCPIP_ASSERT(tlsSeg1Len <= TcpIpSeg1->AvailLen);

  /* #10 If TLS wraparound is before or at TCP wraparound, start by consuming plaintext in TLS segment 1 into TCP segment 1 */
  ciphertextOutBufLen = TcpIpSeg1->AvailLen;
  csmRetVal = TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_STREAMSTART,                                      /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
    tlsSeg1Ptr, tlsSeg1Len,
    TcpIpSeg1->AvailPtr, &ciphertextOutBufLen);

  TcpIp_SegmentCommit(TcpIpSeg1, ciphertextOutBufLen);

  /* Update size of plaintext remaining */
  plaintextRemainingLen -= tlsSeg1Len;

  /* Wraparound TLS buffer since all plaintext in TLS segment 1 has been consumed */
  tlsSeg2Ptr = TcpIp_GetAddrTlsBufferTx(startTlsBufferTxIdx);
  /* No plaintext consumed from TLS segment 2 so far */
  plaintextConsumedTlsSeg2Len = 0;

  /* If there is more than 1 block of space in TCP segment 1, and there is more than 1 block of plaintext to encrypt,
     encrypt sufficient plaintext from TLS segment 2 so that only 1-2 block of space is left in TCP segment 1
   */
  if (   (TcpIpSeg1->AvailLen > TCPIP_TLS_BLOCK128_BLOCKSIZE)
      && (plaintextRemainingLen > TCPIP_TLS_BLOCK128_BLOCKSIZE))
  {
    /* Calculate plaintext to consume from TLS buffer segment 2 */
    uint32 bytesToConsume = TCPIP_MIN(plaintextRemainingLen, TcpIpSeg1->AvailLen);
    /* Consume one block less plaintext */
    bytesToConsume -= TCPIP_TLS_BLOCK128_BLOCKSIZE;

    /* Encrypt plaintext in TLS segment 2 into remaining space in TCP segment 1 */
    ciphertextOutBufLen = TcpIpSeg1->AvailLen;
    csmRetVal |= TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_UPDATE,                                        /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
      &tlsSeg2Ptr[plaintextConsumedTlsSeg2Len], bytesToConsume,
      TcpIpSeg1->AvailPtr, &ciphertextOutBufLen);

    TcpIp_SegmentCommit(TcpIpSeg1, ciphertextOutBufLen);

    /* Update plaintext consumed from TLS segment 2 */
    plaintextConsumedTlsSeg2Len = bytesToConsume;
    /* Update total plaintext remaining */
    plaintextRemainingLen -= bytesToConsume;
  }

  /* At this point, TCP segment 1 has 1 or 2 blocks of space remaining.
     Use temporary buffer to encrypt plaintext from TLS segment 2
     till TCP segment 1 is filled up.
   */
  while ((TcpIpSeg1->AvailLen > 0u)
    && (plaintextRemainingLen > 0u))
  {
    /* Temporary buffer to store ciphertext */
    uint8 tmpBuffer[TCPIP_TLS_MIN_BUFFER_SIZE];
    /* Length of temporary buffer */
    uint32 tmpBufferLen = TCPIP_TLS_MIN_BUFFER_SIZE;
    /* Plaintext to consume from TLS segment 2 */
    uint32 plaintextToConsume = TCPIP_MIN(plaintextRemainingLen, TCPIP_TLS_MIN_BUFFER_SIZE);

    /* Encrypt plaintext in TLS segment 2 into temp buffer */
    csmRetVal |= TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_UPDATE, &tlsSeg2Ptr[plaintextConsumedTlsSeg2Len],
      plaintextToConsume, tmpBuffer, &tmpBufferLen);

    /* Update plaintext consumed from TLS segment 2 */
    plaintextConsumedTlsSeg2Len += plaintextToConsume;
    /* Update plaintext remaining to be encrypted */
    plaintextRemainingLen -= plaintextToConsume;

    TcpIp_SegmentMultiTargetMemCpy(TcpIpSeg1, TcpIpSeg2, &tmpBuffer[0], tmpBufferLen);
  } /* while */

  /* If more plaintext remaining, encrypt directly into TCP segment 2 */
  if (plaintextRemainingLen > 0u)
  {
    /* Encrypt remaining plaintext into TCP segment 2 */
    ciphertextOutBufLen = TcpIpSeg2->AvailLen;

    csmRetVal |= TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_UPDATE,                                        /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
      &tlsSeg2Ptr[plaintextConsumedTlsSeg2Len], plaintextRemainingLen,
      TcpIpSeg2->AvailPtr, &ciphertextOutBufLen);

    TcpIp_SegmentCommit(TcpIpSeg2, ciphertextOutBufLen);
  }

  return csmRetVal; /*lint !e550 */
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario1 */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario2
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6010, 6030, 6050 1 */ /* MD_TcpIp_TlsStreaming_STPTH_6010, MD_TcpIp_TlsStreaming_STCYC_6030, MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario2(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                               csmRetVal;
  TcpIp_SizeOfTlsBufferMgmtType         tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx =
    TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_CsmJobIdType encryptionJobId
    = TcpIp_GetCsmJobId(TcpIp_GetCsmJobIdCipherEncryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));
  TcpIp_TlsBufferTxRlSendIdxOfTlsBufferMgmtDynType rlSendTlsBufferTxIdx =
                                                         TcpIp_GetTlsBufferTxRlSendIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  TcpIp_TlsBufferTxAdHsEndIdxOfTlsBufferMgmtType endTlsBufferTxIdx =
                                                         TcpIp_GetTlsBufferTxAdHsEndIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);
  TcpIp_TlsBufferTxAdHsEndIdxOfTlsBufferMgmtType startTlsBufferTxIdx =
                                                         TcpIp_GetTlsBufferTxAdHsStartIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);
  /* Length of plaintext in TLS Tx buffer */
  uint32 totalPlaintextRemainingLen = TcpIp_GetRlTxContentTypeLenOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  /* Pointer to TLS Tx buffer segment 1 */
  TCPIP_P2C(uint8) tlsSeg1Ptr = TcpIp_GetAddrTlsBufferTx(rlSendTlsBufferTxIdx);
  /* Length of TLS segment 1 */
  uint32 tlsSeg1Len = ((uint32)endTlsBufferTxIdx - rlSendTlsBufferTxIdx);
  /* Flag to indicate TLS wraparound */
  boolean switchTlswrapround = FALSE;
  /* Offset of plaintext consumed from TLS buffer segment */
  uint32 plaintextInTlsSegOffset = 0u;                                                                                  /* PRQA S 2981 */ /* MD_TCPIP_Rule2.2_RedundantAssignment */
  /* Pointer to TLS Tx buffer segment 2 */
  TCPIP_P2C(uint8) tlsSeg2Ptr = TcpIp_GetAddrTlsBufferTx(startTlsBufferTxIdx);
  /* Length of the ciphertext buffer for next Csm encrypt call */
  uint32             ciphertextOutBufLen;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(tlsSeg1Len > TcpIpSeg1->AvailLen);
  /* #10 TLS wraparound is after TCP wraparound, encrypt plaintext in TLS segment 1 that fits into TCP segment 1 */
  ciphertextOutBufLen = TcpIpSeg1->AvailLen;
  csmRetVal = TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_STREAMSTART,                                      /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
    tlsSeg1Ptr, TcpIpSeg1->AvailLen,
    TcpIpSeg1->AvailPtr, &ciphertextOutBufLen);

  /* Update size of TLS segment 1 */
  tlsSeg1Len -= TcpIpSeg1->AvailLen;
  /* Update the length of plaintext data consumed from TLS segment 1 */
  plaintextInTlsSegOffset = TcpIpSeg1->AvailLen;
  /* Update plaintext remaining to be encrypted */
  totalPlaintextRemainingLen -= TcpIpSeg1->AvailLen;

  TcpIp_SegmentCommit(TcpIpSeg1, ciphertextOutBufLen);

  /* At this point there is 1-2 blocks of space remaining in TCP segment 1
     Use temporary buffer to encrypt 4 blocks of plaintext until all space
     in TCP segment is filled
  */
  while (   (TcpIpSeg1->AvailLen > 0u)
         && (totalPlaintextRemainingLen > 0u))
  {
    /* Temporary buffer to encrypt plaintext from TLS buffer */
    uint8 tmpBuffer[TCPIP_TLS_BLOCK128_BLOCKSIZE * 2u];
    /* Pointer to the corresponding TLS segment (1 or 2) */
    TCPIP_P2C(uint8) plaintextPtr;
    /* Length of plaintext to encrypt in next Csm call */
    uint32 plaintextLen;

    /* Determine the next TLS segment to extract plaintext from, depending on TLS wraparound */
    if ((switchTlswrapround == FALSE)
      && (tlsSeg1Len == 0u))
    {
      /* TLS buffer wraparound, extract plaintext from TLS segment 2 */
      /* Indicate TLS has wrapped around */
      switchTlswrapround = TRUE;
      /* Reset the plaintext offset for TLS segment 2 */
      plaintextInTlsSegOffset = 0;
      /* Start extracting plaintext from TLS segment 2 */
      plaintextPtr = &tlsSeg2Ptr[plaintextInTlsSegOffset];
    }
    else if (tlsSeg1Len != 0u)
    {
      /* Plaintext remaining before wraparound, extract plaintext from TLS segment 1 */
      plaintextPtr = &tlsSeg1Ptr[plaintextInTlsSegOffset];
    }
    else
    {
      /* Extract plaintext from TLS segment 2 */
      plaintextPtr = &tlsSeg2Ptr[plaintextInTlsSegOffset];
    }

    /* Calculate how much plaintext can be encrypted */
    if (tlsSeg1Len != 0u)
    {
      /* If plaintext remaining in TLS segment 1, consume it */
      plaintextLen =   (tlsSeg1Len > (TCPIP_TLS_BLOCK128_BLOCKSIZE * 2u))
                  ? (TCPIP_TLS_BLOCK128_BLOCKSIZE * 2u)
                  : tlsSeg1Len;
    }
    else
    {
      /* Plaintext in TLS segment 2, consume as much possible */
      plaintextLen =   (totalPlaintextRemainingLen > (TCPIP_TLS_BLOCK128_BLOCKSIZE * 2u))
                  ? (TCPIP_TLS_BLOCK128_BLOCKSIZE * 2u)
                  : totalPlaintextRemainingLen;
    }

    /* Encrypt plaintext from TLS segment 2 into temp buffer */
    ciphertextOutBufLen = TCPIP_TLS_BLOCK128_BLOCKSIZE * 2u;
    csmRetVal |= TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_UPDATE, plaintextPtr,
                             plaintextLen, tmpBuffer, &ciphertextOutBufLen);

    /* Copy the encrypted data into the TcpIp segments */
    TcpIp_SegmentMultiTargetMemCpy(TcpIpSeg1, TcpIpSeg2, &tmpBuffer[0], ciphertextOutBufLen);

    /* Update size of ciphertext in TCP segment */
    plaintextInTlsSegOffset += plaintextLen;
    /* Update remaining plaintext to be consumed */
    totalPlaintextRemainingLen -= plaintextLen;

    /* Update the plaintext remaining in TLS segment 1 */
    if (tlsSeg1Len != 0u)
    {
      tlsSeg1Len -= plaintextLen;
    }
  } /* while */

  /* If plaintext remaining in TLS segment 1, encrypt directly into TCP segment 2 */
  if (tlsSeg1Len > 0u)
  {
    uint32 consumedBytes = TcpIpSeg2->AvailLen;

    csmRetVal |= TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_UPDATE,                                        /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
      &tlsSeg1Ptr[plaintextInTlsSegOffset], tlsSeg1Len,
      TcpIpSeg2->AvailPtr, &consumedBytes);

    TcpIp_SegmentCommit(TcpIpSeg2, consumedBytes);

    totalPlaintextRemainingLen -= tlsSeg1Len;

    /* Wrap around Tls buffer */
    tlsSeg2Ptr = TcpIp_GetAddrTlsBufferTx(startTlsBufferTxIdx);
    plaintextInTlsSegOffset = 0u;
  }
  else if (switchTlswrapround == FALSE)
  {
    /* TLS buffer has wrapped around, point to TLS segment 2 */
    tlsSeg2Ptr = TcpIp_GetAddrTlsBufferTx(startTlsBufferTxIdx);
    plaintextInTlsSegOffset = 0u;
  }
  else
  {
   /* nothing to do - TLS buffer has already wrapped around */
  }

  /* Encrypt remaining plaintext in TLS segment 2, encrypt directly into TCP segment 2 */
  if (totalPlaintextRemainingLen > 0u)
  {
    uint32 consumedBytes = TcpIpSeg2->AvailLen;

    csmRetVal |= TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_UPDATE,                                        /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
      &tlsSeg2Ptr[plaintextInTlsSegOffset], totalPlaintextRemainingLen,
      TcpIpSeg2->AvailPtr, &consumedBytes);

    TcpIp_SegmentCommit(TcpIpSeg2, consumedBytes);
  }

  return csmRetVal; /*lint !e550 */
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparoundScenario2 */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTcpWraparound
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTcpWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                    retVal;
  Std_ReturnType                                    csmRetVal;
  TcpIp_SizeOfTlsBufferMgmtType                     tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsBufferTxRlSendIdxOfTlsBufferMgmtDynType  rlSendIdx =
                                                      TcpIp_GetTlsBufferTxRlSendIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  TcpIp_TlsBufferTxAdHsEndIdxOfTlsBufferMgmtType    txBufferEndIdx =
                                                      TcpIp_GetTlsBufferTxAdHsEndIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);
  uint32                                            plaintextDataLen =
                                                      TcpIp_GetRlTxContentTypeLenOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if there is Tls buffer wraparound and encrypt content into the TcpIp buffer segments */
  if ((rlSendIdx + plaintextDataLen) <= txBufferEndIdx)
  {
    csmRetVal = TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithoutTlsWraparound(TlsConIdx, TcpIpSeg1, TcpIpSeg2);
  }
  else
  {
    csmRetVal = TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTlsWraparound(TlsConIdx, TcpIpSeg1, TcpIpSeg2);
  }

  /* Check if the result of the CSM operation is E_OK */
  retVal = TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_ENCRYPTADHSBUFAES128CBC);

  return retVal;
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTcpWraparound() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWithTcpWraparound
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWithTcpWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                retVal = E_NOT_OK;
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* rlSendTlsBufferTxIdx is in the range of the complete TlsBufferTx */
  /* #10 Invoke the encrypt function depending on the type of Rl content */
  switch (TcpIp_GetRlTxContentTypeOfTlsBufferMgmtDyn(tlsBufferMgmtIdx))
  {
  case TCPIP_TLS_CT_HS:
  case TCPIP_TLS_CT_AD:
  {
    retVal = TcpIp_Tls12CoreRl_EncryptAes128CbcAdHsWithTcpWraparound(TlsConIdx, TcpIpSeg1, TcpIpSeg2);
    break;
  } /* case TCPIP_TLS_CT_HS | TCPIP_TLS_CT_AD */
  case TCPIP_TLS_CT_AL:
  {
    retVal = TcpIp_Tls12CoreRl_EncryptAes128CbcAlWithTcpWraparound(TlsConIdx, TcpIpSeg1, TcpIpSeg2);
    break;
  } /* case TCPIP_TLS_CT_AL */
  default:    /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
  /* Default case left blank intentionally */
  break;
  } /* switch */
  return retVal;
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWithTcpWraparound() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWoWraparound
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWoWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpSeg)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                retVal = E_NOT_OK;
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* rlSendTlsBufferTxIdx is in the range of the complete TlsBufferTx */
  /* #10 Invoke the encrypt function depending on the type of Rl content */
  switch (TcpIp_GetRlTxContentTypeOfTlsBufferMgmtDyn(tlsBufferMgmtIdx))
  {
    case TCPIP_TLS_CT_HS:
    case TCPIP_TLS_CT_AD:
    {
      retVal = TcpIp_Tls12CoreRl_EncryptAes128CbcAdHs(TlsConIdx, TcpSeg);
      break;
    } /* case TCPIP_TLS_CT_HS | TCPIP_TLS_CT_AD */
    case TCPIP_TLS_CT_AL:
    {
      retVal = TcpIp_Tls12CoreRl_EncryptAes128CbcAl(TlsConIdx, TcpSeg);
      break;
    } /* case TCPIP_TLS_CT_AL */
    default:    /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
  /* Default case left blank intentionally */
      break;
  } /* switch */
  return retVal;
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcContentDataWoWraparound() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcAdHs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAdHs(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpSeg)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                                  retVal;
  TcpIp_SizeOfTlsBufferMgmtType         tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx =
    TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_CsmJobIdType encryptionJobId
    = TcpIp_GetCsmJobId(TcpIp_GetCsmJobIdCipherEncryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));
  TcpIp_TlsBufferTxRlSendIdxOfTlsBufferMgmtDynType rlSendTlsBufferTxIdx =
                                                         TcpIp_GetTlsBufferTxRlSendIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  TcpIp_TlsBufferTxAdHsEndIdxOfTlsBufferMgmtType endTlsBufferTxIdx =
                                                         TcpIp_GetTlsBufferTxAdHsEndIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);
  TcpIp_TlsBufferTxAdHsEndIdxOfTlsBufferMgmtType startTlsBufferTxIdx =
                                                         TcpIp_GetTlsBufferTxAdHsStartIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);
  TCPIP_P2C(uint8)                             plaintextDataPtr =
                                                         TcpIp_GetAddrTlsBufferTx(rlSendTlsBufferTxIdx);
  uint32                                       plaintextDataLen =
                                                         TcpIp_GetRlTxContentTypeLenOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  uint32                                       availableBufLen = TcpSeg->AvailLen;
  Std_ReturnType csmRetVal;

  /* ----- Implementation ----------------------------------------------- */
  /*#10 Start Aes128 Cbc streaming, and encrypt data into linear TCP buffer considering TLS wraparound */
  if ((rlSendTlsBufferTxIdx + plaintextDataLen) <= endTlsBufferTxIdx)
  {
    /* Scenario 1: No TLS buffer wraparound */
    csmRetVal = TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_STREAMSTART,                                    /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
      plaintextDataPtr, plaintextDataLen,
      TcpSeg->AvailPtr, &availableBufLen);

    TcpIp_SegmentCommit(TcpSeg, availableBufLen);
  }
  else
  {
    /* Scenario 2: TLS buffer wraparound */
    uint32 dataLenAtEndOfTxBuf   = ((uint32)endTlsBufferTxIdx - rlSendTlsBufferTxIdx);
    uint32 dataLenAtStartOfTxBuf = (plaintextDataLen - dataLenAtEndOfTxBuf);

    /* Feed the data till wraparound */
    csmRetVal = TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_STREAMSTART,                                    /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
      plaintextDataPtr, dataLenAtEndOfTxBuf,
      TcpSeg->AvailPtr, &availableBufLen);

    TcpIp_SegmentCommit(TcpSeg, availableBufLen);

    /* Wraparound the TLS buffer */
    plaintextDataPtr = TcpIp_GetAddrTlsBufferTx(startTlsBufferTxIdx);

    availableBufLen = TcpSeg->AvailLen;

    /* Feed rest of content data */
    csmRetVal |= TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_UPDATE,                                        /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
      plaintextDataPtr, dataLenAtStartOfTxBuf,
      TcpSeg->AvailPtr, &availableBufLen);

    TcpIp_SegmentCommit(TcpSeg, availableBufLen);
  }

  /* Check if the result of the CSM operation is E_OK */
  retVal = TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0,
    TCPIP_TLS_FCTID_CORE_CRYPTO_ENCRYPTADHSBUFAES128CBC);

  return retVal;
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcAdHs() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcAl
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAl(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpSeg)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_CsmJobIdType encryptionJobId
    = TcpIp_GetCsmJobId(TcpIp_GetCsmJobIdCipherEncryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));
  TcpIp_TlsBufferTxRlSendIdxOfTlsBufferMgmtDynType rlSendTlsBufferTxIdx =
    TcpIp_GetTlsBufferTxRlSendIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx); /* Start of the rl frame in the TX Buffer */

  TCPIP_P2C(uint8) plaintextDataPtr = TcpIp_GetAddrTlsBufferTx(rlSendTlsBufferTxIdx);
  uint32 plaintextDataLen = TcpIp_GetRlTxContentTypeLenOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  Std_ReturnType retVal;
  Std_ReturnType csmRetVal;
  uint32 availableBufferLen = TcpSeg->AvailLen;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Encrypt the data into the provided buffer and check the operation result */
  csmRetVal = TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_STREAMSTART,                                      /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
    plaintextDataPtr, plaintextDataLen,
    TcpSeg->AvailPtr, &availableBufferLen);

  TcpIp_SegmentCommit(TcpSeg, availableBufferLen);

  /* Check if the result of the CSM operation is E_OK */
  retVal = TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_ENCRYPTADHSBUFAES128CBC);

  return retVal;
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcAl() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcAlWithTcpWraparound
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcAlWithTcpWraparound(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpIpSeg1,
  TcpIp_SegmentPtrType        TcpIpSeg2)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_CsmJobIdType encryptionJobId
    = TcpIp_GetCsmJobId(TcpIp_GetCsmJobIdCipherEncryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));
  TcpIp_TlsBufferTxRlSendIdxOfTlsBufferMgmtDynType rlSendTlsBufferTxIdx =
    TcpIp_GetTlsBufferTxRlSendIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  TCPIP_P2C(uint8) plaintextDataPtr = TcpIp_GetAddrTlsBufferTx(rlSendTlsBufferTxIdx);
  uint32 plaintextDataLen = TcpIp_GetRlTxContentTypeLenOfTlsBufferMgmtDyn(tlsBufferMgmtIdx);
  /* Temporary buffer to store the alert ciphertext */
  uint8 cipherTextBuffer[TCPIP_TLS_MIN_BUFFER_SIZE];
  uint32 cipherTextDataLen = TCPIP_TLS_MIN_BUFFER_SIZE;
  Std_ReturnType retVal;
  Std_ReturnType csmRetVal;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(plaintextDataLen <= (TcpIpSeg1->AvailLen + TcpIpSeg2->AvailLen));

  /* #10 Start Aes128 Cbc streaming and encrypt alert data into the temp buffer */
  csmRetVal = TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_STREAMSTART, plaintextDataPtr, plaintextDataLen,
    &cipherTextBuffer[0], &cipherTextDataLen);

  if (csmRetVal == E_OK)
  {
    TCPIP_ASSERT(cipherTextDataLen <= (TcpIpSeg1->AvailLen + TcpIpSeg2->AvailLen));

    TcpIp_SegmentMultiTargetMemCpy(TcpIpSeg1, TcpIpSeg2, &cipherTextBuffer[0], cipherTextDataLen);
  }

  /* Check if the result of the CSM operation is E_OK */
  retVal = TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_ENCRYPTADHSBUFAES128CBC);

  return retVal;
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcAlWithTcpWraparound() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_GenerateAesIv
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreCrypto_GenerateAesIv(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        AesIvSeg)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_TlsCsmJobIterType encryptTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
    TcpIp_GetCsmJobIdCipherEncryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));
  TcpIp_CsmKeyIdType encryptKeyId = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(encryptTlsCsmJobIdx);
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Generate a new random IV value and set the corresponding key element */
  if (TcpIp_Tls12CoreCrypto_GenerateIvRandomValue(TlsConIdx, AesIvSeg, TCPIP_TLS_AES128_IV_LEN) == E_OK)
  {
    if (TcpIp_Tls12CoreCrypto_SetIvValue(TlsConIdx, encryptKeyId, AesIvSeg->BasePtr, TCPIP_TLS_AES128_IV_LEN) == E_OK)
    {
      retVal = E_OK;
    }
  }

  return retVal;
} /* TcpIp_Tls12CoreCrypto_GenerateAesIv() */

/***********************************************************************************************************************
 *  TLS CORE RL - CIPHERS
 **********************************************************************************************************************/
/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_ProcessHsFrame
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_ProcessHsFrame(
  TcpIp_TlsConnectionIterType                 TlsConIdx,
  uint8                                       HsType,
  uint32                                      HsLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#  if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_OFF)
  TCPIP_DUMMY_STATEMENT_CONST(HsType);                                                                                  /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#  endif
  if (   (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_STATE_HANDSHAKE_FINALIZING)
      || (TcpIp_GetHandshakeStateOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_STATE_HANDSHAKE_DONE))
  {
    /* #10 Received a Handshake messages after a complete handshake. */
    if (HsType == TCPIP_TLS_HSTYPE_CLIENTHELLO)
    {
      /* #20 Handle ClientHello */
      TcpIp_Tls12CoreRl_ProcessHsClientHelloAfterHandshake(TlsConIdx);
      retVal = E_OK;
    }
    else if (HsType != TCPIP_TLS_HSTYPE_HELLOREQUEST)
    {
      /* #30 All other handshake messages are in any case invalid after the handshake and can be handled here. */
      TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_UNEXP_MSG);
    }
    else
    {
      /* Hello Request is handled below */
      retVal = E_OK;
    }
  }
  else
  {
    retVal = E_OK;
#  if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
    /* For the TLS 1.2 client the decryption is blocked when the ServerFinished is received.
      * This must be done since the Server is allowed to start sending application data immediately when the Finished
      * message was send. As client however the Finished must be validated before application data can be processed.
      * Therefore incoming messages are not decrypted and therefore not processed till the validation is completed.
      * This block is lifted in TcpIp_Tls12ClientHsStm_FinalizeHandshake(). */
    if (TcpIp_GetModeOfTlsConnection(TlsConIdx) == TCPIP_TLS_CLIENT)
    {
      if (HsType == TCPIP_TLS_HSTYPE_FINISHED)
      {
        TcpIp_TlsCoreRl_BlockDecryption(TlsConIdx);
      }
    }
#  endif
  }

  if (   (retVal == E_OK)
      && (HsType == TCPIP_TLS_HSTYPE_HELLOREQUEST))
  {

    /* #40 Handle HelloRequest */
    TcpIp_Tls12CoreRl_ProcessHsHelloRequest(TlsConIdx, HsLen);
  }
  return retVal;
} /* TcpIp_Tls12CoreRl_ProcessHsFrame() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPaddingInt
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPaddingInt(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        TcpSeg)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsCipherWorkerIterType  activeTlsCipherWorkerIdx
  = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);

  uint8 aesPaddingByte = TcpIp_GetAes128CbcPaddingByteOfTlsConnectionDyn(TlsConIdx);

  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(aesPaddingByte < TCPIP_TLS_BLOCK128_BLOCKSIZE);

  if(   (activeTlsCipherWorkerIdx < TcpIp_GetSizeOfTlsCipherWorker())
     && ((TCPIP_TLS_HASH_SHA256_LEN + TCPIP_TLS_BLOCK128_BLOCKSIZE) >= (TcpIp_GetCipherMacKeyLengthOfTlsCipherWorker(activeTlsCipherWorkerIdx)
                                                                      + aesPaddingByte + TCPIP_TLS_TLS_PADDING_LEN))
     && (aesPaddingByte < TCPIP_TLS_BLOCK128_BLOCKSIZE))
  {
    TCPIP_P2C(uint8) macPtr
      = TcpIp_GetAddrTlsTemporaryHmacBuffer(TcpIp_GetTlsTemporaryHmacBufferStartIdxOfTlsConnection(TlsConIdx));
    uint8 macLen = TcpIp_GetCipherMacKeyLengthOfTlsCipherWorker(activeTlsCipherWorkerIdx);

    TcpIp_CsmJobIdType  encryptionJobId
      = TcpIp_GetCsmJobId(TcpIp_GetCsmJobIdCipherEncryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));
    Std_ReturnType csmRetVal;

    /* Array to combine HMAC and padding. Has to fit for the max supported sizes. */
    uint8 macAndPadding[TCPIP_TLS_HASH_SHA256_LEN + TCPIP_TLS_BLOCK128_BLOCKSIZE];
    uint8                           dataLen;  /* length of data in macAndPadding[] that shall be encrypted */
    TCPIP_P2V(uint8)                paddingPtr;
    uint32                          availLen;

    /* Retrieve the AesCbc padding bytes length - always calculated to be from 0 to TCPIP_TLS_BLOCK128_BLOCKSIZE */

    /* #10 Copy the HMAC to the local array. */
    VStdMemCpy(&macAndPadding[0], macPtr, macLen);                                                                      /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
    dataLen = macLen;

    /* #20 Write the TLS-Padding byte and the AES padding (PKSC7 Padding) into the local array. */
    paddingPtr = &macAndPadding[dataLen];

    /* Write the padding */
    VStdMemSet(paddingPtr, aesPaddingByte, ((VStdLib_CntType)aesPaddingByte + TCPIP_TLS_TLS_PADDING_LEN));              /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
    dataLen += (uint8)(aesPaddingByte + TCPIP_TLS_TLS_PADDING_LEN);
    availLen = TcpSeg->AvailLen;

    /* #30 Feed the MAC and the padding into the already running Aes128 CBC streaming, and finish the operation. */
    csmRetVal = TCPIP_CSM_Encrypt(encryptionJobId, CRYPTO_OPERATIONMODE_UPDATE | CRYPTO_OPERATIONMODE_FINISH,           /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
      &macAndPadding[0], dataLen,
      TcpSeg->AvailPtr, &availLen);

    TcpIp_SegmentCommit(TcpSeg, availLen);

    /* Check if the result of the CSM operation is E_OK */
    retVal = TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0u, 0u,
      TCPIP_TLS_FCTID_CORE_CRYPTO_ENCRYPTHMACPADDINGAES128CBC);
  }
  else
  {
    /* Memory runtime check fails - report DET */
    TcpIp_Tls_CallDetReportError(TCPIP_TLS_API_ID_TLS_DATA_PROCESSING, TCPIP_TLS_E_INV_RUNTIME_CHECK);
    /* retVal already set to E_NOT_OK */
  }

  return retVal;
} /* TcpIp_Tls12CoreRl_EncryptAes128CbcMacAndPaddingInt() */

/**********************************************************************************************************************
 *   TcpIp_Tls12CoreRl_DecryptAndAuthenticateFrameViaCbc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_DecryptAndAuthenticateFrameViaCbc(
  TcpIp_TlsConnectionIterType TlsConIdx,
  CONST(uint8, AUTOMATIC)     ContentType,
  TCPIP_P2C(uint8)            DataPtr,
  uint16                      DataLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* Block size alignment must match with both data lengths */
  uint16 completeDataLen = (TcpIp_GetAddrRxDataSplitOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)->DataLen + DataLen);

  /* #10 Record layer frame complete check AES-128-CBC block size alignment */
  if (TcpIp_Tls12CoreRl_DecryptCbcCheckBlocksizeAlignment(TlsConIdx, completeDataLen) == E_OK)
  {
    /* #20 Record layer size is aligned - Perform AES-128-CBC decryption */
    retVal = TcpIp_Tls12CoreRl_RxCipherDecryptAes128Cbc(TlsConIdx, ContentType, DataPtr, DataLen);
  }
  else
  {
    /* AES-128-CBC Block size mismatch - return E_NOT_OK and do not decrypt the data */
    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_Tls12CoreRl_DecryptAndAuthenticateFrameViaCbc */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreRl_DecryptCbcCheckBlocksizeAlignment
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */

TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_DecryptCbcCheckBlocksizeAlignment(
  TcpIp_TlsConnectionIterType TlsConIdx,
  uint16 DataLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* RFC3602 - The AES-CBC Cipher Algorithm
   * 2.4.  Block Size and Padding
   *
   * The AES uses a block size of sixteen octets (128 bits).
   *
   * Padding is required by the AES to maintain a 16-octet (128-bit)
   * blocksize.  Padding MUST be added, as specified in [ESP], such that
   * the data to be encrypted (which includes the ESP Pad Length and Next
   * Header fields) has a length that is a multiple of 16 octets. */

  /* Check the input data length of the AES-128-CBC operation to its alignment of a multiple of the used block size */
  if ((DataLen % TCPIP_TLS_AES_BLOCK_LEN) == 0u)
  {
    /* #10 Input data length aligned to the used block size - return E_OK */
    retVal = E_OK;
  }
  else
  {
    /* #20 Input data length not aligned - Trigger TLS alert and return E_NOT_OK */
    TcpIp_TlsCoreAlert_Set(TlsConIdx, TCPIP_TLS_ALERT_LEVEL_FATAL, TCPIP_TLS_ALERT_DESC_DECODE_ERROR);
    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_Tls12CoreRl_DecryptCbcCheckBlocksizeAlignment() */

/**********************************************************************************************************************
 *   TcpIp_Tls12CoreRl_RxCipherDecryptAes128Cbc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_RxCipherDecryptAes128Cbc(
  TcpIp_TlsConnectionIterType TlsConIdx,
  uint8                       ContentType,
  TCPIP_P2C(uint8)            DataPtr,
  uint16                      DataLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                retVal = E_NOT_OK;
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);
  /* Length of data processed so far */
  /* Pointer to TCP segment 1 */
  TCPIP_P2C(uint8)              tcpSeg1Ptr = TcpIp_GetAddrRxDataSplitOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)->DataPtr;
  /* Variable to store length of TCP segment 1 */
  uint16                        tcpSeg1Len = TcpIp_GetAddrRxDataSplitOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)->DataLen;
  /* Pointer to TCP segment 1 */
  TCPIP_P2C(uint8)              tcpSeg2Ptr = DataPtr;
  /* Variable to store length of TCP segment 2 */
  uint16                        tcpSeg2Len = DataLen;
  /* Variables to store length of Iv in TCP segment 1 */
  uint16                        ivInTcpSeg1Len = 0u;
  /* Variables to store length of Iv in TCP segment 2 */
  uint16                        ivInTcpSeg2Len = 0u;
  TcpIp_ConstSegmentType        tcpSeg1;
  TcpIp_ConstSegmentType        tcpSeg2;

  /* ----- Implementation ----------------------------------------------- */
  /* Handling of wraparound in TCP and TLS Rx buffers -
   * The buffer before and after wraparound are considered as two separate segments
   * TLS wraparound --> TlsSeg1 and TlsSeg2
   * TCP wraparound --> TcpSeg1 and TcpSeg2
   * It is ensured by the caller that sufficient space is available in TLS Rx buffer
   * to decrypt all the ciphertext in TCP buffer
   */

  /* #10 If the size of the frame is valid, decrypt the received frame */
  if ((tcpSeg1Len + tcpSeg2Len) > TCPIP_TLS_AES128_IV_LEN)
  {
    TcpIp_ConstSegmentInit(&tcpSeg1, tcpSeg1Ptr, tcpSeg1Len);
    TcpIp_ConstSegmentInit(&tcpSeg2, tcpSeg2Ptr, tcpSeg2Len);
    TcpIp_TlsCoreRl_GetIvLengthInBufferSegments(&tcpSeg1, TCPIP_TLS_AES128_IV_LEN, &ivInTcpSeg1Len, &ivInTcpSeg2Len);

    /* #20 Extract the Iv from the TCP buffer segment(s) and set AES key element */
    retVal = TcpIp_Tls12CoreRl_LocateIvInBufferAndSetIvKeyElement(TlsConIdx,
      &tcpSeg1, ivInTcpSeg1Len, &tcpSeg2, ivInTcpSeg2Len);

    if (retVal == E_OK)
    {
      /* #30 Create decrypt data struct */
      /* This data struct was created in order to be able to pass the data segments
       * compactly to the lower hierarchical functions. */
      TcpIp_TlsDecryptDataType  decryptData = { 0 };

      /* addAuthDataSeg and authSeg are not needed in Cbc decryption,
       * nevertheless create segments with zero length (which are not used) to avoid the need for a special handling. */
      TcpIp_SegmentType         addAuthDataSeg = { 0 };
      TcpIp_SegmentType         authSeg = { 0 };

      /* Clear the segments so that only they only contain the ciphertext, not the IV and authentication tag anymore. */
      TcpIp_ConstSegmentClear(&tcpSeg1);
      TcpIp_ConstSegmentClear(&tcpSeg2);

      decryptData.TcpSeg1Ptr = &tcpSeg1;
      decryptData.TcpSeg2Ptr = &tcpSeg2;
      decryptData.AddAuthDataSegPtr = &addAuthDataSeg;
      decryptData.AuthSegPtr = &authSeg;

      /* #40 Perform Aes128 Cbc decryption of data considering wraparound of TCP and TLS buffer */
      retVal = TcpIp_TlsCoreRl_DecryptAndAuthenticateFrame(TlsConIdx, ContentType, &decryptData);
    }
  }

  return retVal;
} /* TcpIp_Tls12CoreRl_RxCipherDecryptAes128Cbc */

/**********************************************************************************************************************
 *   TcpIp_Tls12CoreRl_LocateIvInBufferAndSetIvKeyElement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_LocateIvInBufferAndSetIvKeyElement(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_ConstSegmentPtrType   TcpSeg1Ptr,
  uint16                      IvInTcpSeg1Len,
  TcpIp_ConstSegmentPtrType   TcpSeg2Ptr,
  uint16                      IvInTcpSeg2Len)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ConstSegmentPtrType  IVConstSegPtr = NULL_PTR;
  TcpIp_ConstSegmentType     IVConstSeg;
  uint8             tempIV[TCPIP_TLS_IV_MAX_LEN] = { 0 };
  TcpIp_SegmentType tempIVSeg;
  uint16 ivLen = 0u;

  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* Depending on the decryption type the Iv has a different length. */
  switch (TcpIp_GetDecryptAlgoTypeOfTlsConnectionDyn(TlsConIdx))
  {
    case TCPIP_TLS_DECRYPT_ALGO_TYPE_CBC:
    {
      ivLen = TCPIP_TLS_AES128_IV_LEN;
      retVal = E_OK;
      break;
    }
    case TCPIP_TLS_DECRYPT_ALGO_TYPE_AEAD:
    {
      ivLen = TCPIP_TLS_BLOCK128_EXPLICIT_IV_LEN;
      retVal = E_OK;
      break;
    }
    default:
    {
      /* default case intentionally empty, should never be reached */
      TCPIP_ASSERT_UNREACHABLE();
      break;
    }
  }

  TCPIP_ASSERT(IvInTcpSeg1Len + IvInTcpSeg2Len == ivLen);
  if (retVal == E_OK)
  {
    /* #10 Extract the Iv from the provided TCP segments */
    if (IvInTcpSeg1Len == 0u)
    {
      /* IV is in the second data segment */
      IVConstSegPtr = TcpSeg2Ptr;
    }
    else if (IvInTcpSeg1Len < ivLen)
    {
      TCPIP_ASSERT(IvInTcpSeg1Len != 0u);

      TcpIp_SegmentInit(&tempIVSeg, &tempIV[0], TCPIP_TLS_IV_MAX_LEN);

      /* Copy the IV data from seg1 into temp buffer segment */
      TcpIp_SegmentMemCpyFromConstSegment(&tempIVSeg, TcpSeg1Ptr, IvInTcpSeg1Len);
      /* Copy the IV data from seg2 into temp buffer segment */
      TcpIp_SegmentMemCpyFromConstSegment(&tempIVSeg, TcpSeg2Ptr, IvInTcpSeg2Len);

      /* IV is present in tempBuffer, initialize IVConstSeg with the buffer and the usage from tempIVSeg. */
      IVConstSegPtr = &IVConstSeg;
      TcpIp_ConstSegmentInitWithUsedSegment(IVConstSegPtr, &tempIVSeg);
    }
    else /* (IvInTcpSeg1Len == ivLen) */
    {
      /* IV is present in seg1 */
      IVConstSegPtr = TcpSeg1Ptr;
    }

    /* #20 Prepare the CSM and set the Iv into the key element depending on the decrypt type. */
    if (TcpIp_GetDecryptAlgoTypeOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_DECRYPT_ALGO_TYPE_CBC)
    {
      retVal = TcpIp_Tls12CoreRl_SetIvForDecryptCall(TlsConIdx, IVConstSegPtr);
    }
    else /* TcpIp_GetDecryptTypeOfTlsConnectionDyn(TlsConIdx) == TCPIP_TLS_DECRYPT_ALGO_TYPE_AEAD */
    {
      retVal = TcpIp_Tls12CoreRl_RxCipherDecryptAEADBlock128SetIv(TlsConIdx, IVConstSegPtr);
    }
  }

  return retVal;
} /* TcpIp_Tls12CoreRl_LocateIvInBufferAndSetIvKeyElement */

/**********************************************************************************************************************
 *   TcpIp_Tls12CoreRl_RxCipherDecryptAEADBlock128SetIv
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_RxCipherDecryptAEADBlock128SetIv(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_ConstSegmentPtrType   ExplicitIVSegPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType    retVal;
  TcpIp_TlsCipherWorkerIterType  activeTlsCipherWorkerIdx = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(
                                                                TlsConIdx);
  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx = TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(
                                                       activeTlsCipherWorkerIdx);

  TcpIp_TlsCsmJobIterType decryptTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
    TcpIp_GetCsmJobIdCipherDecryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));

  TcpIp_TlsCsmKeyInfoType rxIvKey;
  TcpIp_TlsCsmKeyInfoType tmpKey; /* Temporary CSM key element */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Load Iv (implicit Iv + explicit Iv) into the key element */

  /* Nonce for Block128 is 12 bytes and is constructed as
   * |    Iv | Tls seq No.|
   * |0     3|4         11|
   */
  tmpKey.KeyId = TcpIp_GetHsTemporaryBufferKeyIdOfTlsHandshake(activeTlsHandshakeIdx);
  tmpKey.KeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
  tmpKey.KeyLen = 0;

  /* Copy the explicit Iv (sequence number) into the tmp key */
  retVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &tmpKey);
  retVal |= TCPIP_CSM_KeyElementSet(tmpKey.KeyId, tmpKey.KeyElementId,
    ExplicitIVSegPtr->AvailPtr, TCPIP_TLS_BLOCK128_EXPLICIT_IV_LEN);
  /* TCPIP_TLS_BLOCK128_EXPLICIT_IV_LEN was copied, so commit it to the segment. */
  TcpIp_ConstSegmentCommit(ExplicitIVSegPtr, TCPIP_TLS_BLOCK128_EXPLICIT_IV_LEN);

  rxIvKey.KeyId = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(decryptTlsCsmJobIdx);
  rxIvKey.KeyElementId = CRYPTO_KE_CIPHER_IV;

  /* Copy explicit Iv from temp key to decrypt key and set it valid. */
  retVal |= TCPIP_CSM_KeyElementCopyPartial(
    tmpKey.KeyId,
    tmpKey.KeyElementId,
    0,
    TCPIP_TLS_BLOCK128_IMPLICIT_IV_LEN,
    TCPIP_TLS_BLOCK128_EXPLICIT_IV_LEN,
    rxIvKey.KeyId,
    rxIvKey.KeyElementId
  );

  retVal |= TCPIP_CSM_KeySetValid(rxIvKey.KeyId);

  if (retVal != E_OK)
  {
    /* If not successful, report the user error */
    TcpIp_TlsUserErrorType userError;

    userError.FunctionId = TCPIP_TLS_FCTID_CORE_CRYPTO_SETAESIVVALUE;
    userError.UserErrorId = TCPIP_TLS_ERRORID_CRYPTO_SETAESIVVALUE_FAILED;
    userError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(userError);
  }

  return retVal;
} /* TcpIp_Tls12CoreRl_RxCipherDecryptAEADBlock128SetIv */

/**********************************************************************************************************************
 *   TcpIp_Tls12CoreRl_SetIvForDecryptCall
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_Tls12CoreRl_SetIvForDecryptCall(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_ConstSegmentPtrType   DataSegPtr)
{
  TcpIp_TlsCipherWorkerActiveIdxOfTlsConnectionDynType activeTlsCipherWorkerIdx =
    TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_TlsCsmJobIterType decryptTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
    TcpIp_GetCsmJobIdCipherDecryptIdxOfTlsCipherWorker(activeTlsCipherWorkerIdx));
  TcpIp_CsmKeyIdType decryptKeyId = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(decryptTlsCsmJobIdx);
   Std_ReturnType retVal = E_NOT_OK;

   /* #10 Write the decrypt key element with the provided key data and set it valid. */
   retVal = TcpIp_TlsCoreCrypto_SetCsmKeyElementAndSetKeyValid(decryptKeyId, CRYPTO_KE_CIPHER_IV,
     DataSegPtr->AvailPtr, TCPIP_TLS_AES128_IV_LEN);
   /* TCPIP_TLS_AES128_IV_LEN was copied, so commit it to the segment. */
   TcpIp_ConstSegmentCommit(DataSegPtr, TCPIP_TLS_AES128_IV_LEN);

   return retVal;
} /* TcpIp_Tls12CoreRl_SetIvForDecryptCall() */

/**********************************************************************************************************************
 *  TLS CORE BUFFER
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *   TLS CORE CONTEXT
 *********************************************************************************************************************/
#  if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12Msg_GetLenOfCurveSpecificSignature
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint32, TCPIP_CODE) TcpIp_Tls12Msg_GetLenOfCurveSpecificSignature(
  CONST(TcpIp_TlsEllipticCurveIdType, AUTOMATIC) CurveId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 lenRetVal = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the length of the specific signature depending on the given Curve ID */
  switch (CurveId)
  {
    case TCPIP_CURVE_ID_SECP256R1: {
      lenRetVal = (uint32) TCPIP_CURVE_ID_SECP256R1_SIGNATURE_LEN;
      break;
    } /* case TCPIP_CURVE_ID_SECP256R1 */
    case TCPIP_CURVE_ID_SECP521R1: {
      lenRetVal = (uint32) TCPIP_CURVE_ID_SECP521R1_SIGNATURE_LEN;
      break;
    } /* case TCPIP_CURVE_ID_SECP521R1 */
    case TCPIP_CURVE_ID_X25519: {
      lenRetVal = (uint32) TCPIP_CURVE_ID_ED25519_SIGNATURE_LEN;
      break;
    } /* case TCPIP_CURVE_ID_X25519 */
    default: {                                                                                                           /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
      break;
    } /* default */
  } /* switch */

  return lenRetVal;
} /* TcpIp_Tls12Msg_GetLenOfCurveSpecificSignature() */

/**********************************************************************************************************************
 *  TcpIp_Tls12Msg_GetLenOfCurveSpecificDhSharedKey
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint32, TCPIP_CODE) TcpIp_Tls12Msg_GetLenOfCurveSpecificDhSharedKey(
  CONST(TcpIp_TlsEllipticCurveIdType, AUTOMATIC) CurveId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 lenRetVal = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return the length of the specific shared key depending on the given Curve ID */
  switch (CurveId)
  {
    case TCPIP_CURVE_ID_SECP256R1: {
      lenRetVal = TCPIP_CURVE_ID_SECP256R1_SHAREDKEY_LEN;
      break;
    } /* case TCPIP_CURVE_ID_SECP256R1 */
    case TCPIP_CURVE_ID_SECP384R1: {
      lenRetVal = TCPIP_CURVE_ID_SECP384R1_SHAREDKEY_LEN;
      break;
    } /* case TCPIP_CURVE_ID_SECP384R1 */
    case TCPIP_CURVE_ID_SECP521R1: {
      lenRetVal = TCPIP_CURVE_ID_SECP521R1_SHAREDKEY_LEN;
      break;
    } /* case TCPIP_CURVE_ID_SECP521R1 */
    case TCPIP_CURVE_ID_X25519: {
      lenRetVal = TCPIP_CURVE_ID_X25519_SHAREDKEY_LEN;
      break;
    } /* case TCPIP_CURVE_ID_X25519 */
    case TCPIP_CURVE_ID_X448: {
      lenRetVal = TCPIP_CURVE_ID_X448_SHAREDKEY_LEN;
      break;
    } /* case TCPIP_CURVE_ID_X448 */
    default: {                                                                                                           /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
      break;
    } /* default */
  } /* switch */

  return lenRetVal;
} /* TcpIp_Tls12Msg_GetLenOfCurveSpecificDhSharedKey() */


/**********************************************************************************************************************
 *  TcpIp_Tls12Msg_GetNumOfReceivedCertificates
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12Msg_GetNumOfReceivedCertificates(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_TlsBufferRxIterType   CertStartIdx,
  uint32                      CertChainLen,
  uint16                      CertificateMsgLen,
  TCPIP_P2V(uint8)            NumCertPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 numCert = 0u;
  uint32 certBytes = 0u;
  TcpIp_TlsBufferRxIterType certReadTlsBufferRxIdx = CertStartIdx;

  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx
    = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxAdHsEndIdxOfTlsBufferMgmtType tlsBufferRxAdHsEndIdx
    = TcpIp_GetTlsBufferRxAdHsEndIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);

  Std_ReturnType retVal = E_NOT_OK;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Traverse through the received chain and count number of certificates */
  while (certBytes < CertChainLen)
  {
    /* Extract length of the current certificate */
    /* Check if the length of the certificate length fits into the TLS RxBuffer */
    if ((TCPIP_TLS_RXBUFFER_ISIDXINRANGE(certReadTlsBufferRxIdx,
      TCPIP_TLS_CERTLEN_LEN,
      tlsBufferRxAdHsEndIdx))
      /* Check that the received certificate length is greater then the expected length */
      && (CertificateMsgLen >= (certBytes + TCPIP_TLS_CERTLEN_LEN + TCPIP_TLS_CERTCHAINLEN_LEN)))
    {
      uint32 certLength = IpBase_GetUint24(TcpIp_GetAddrTlsBufferRx(certReadTlsBufferRxIdx), 0);
      /* Increase by length of the current certificate */
      certBytes += certLength + TCPIP_TLS_CERTLEN_LEN;
      /* Increment read index by size of certificate and the length of certLen field */
      certReadTlsBufferRxIdx += (TcpIp_TlsBufferRxIterType)certLength + TCPIP_TLS_CERTLEN_LEN;
      numCert++;
    }
    else
    {
      break;
    }
  }

  if ((CertChainLen == 0u) || (certBytes != CertChainLen))
  {
    retVal = E_NOT_OK;
  }
  else
  {
    retVal = E_OK;
    *NumCertPtr = numCert;
  }

  return retVal;
}


/**********************************************************************************************************************
 *  TcpIp_Tls12Msg_RecCertMsgCheckLengthAndReadCertNum
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12Msg_RecCertMsgCheckLengthAndReadCertNum(
  TcpIp_TlsConnectionIterType TlsConIdx,
  uint16 CertificateMsgLen,
  TCPIP_P2V(uint16) CertificateChainLenPtr,
  TCPIP_P2V(uint8) NumCertsPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfTlsBufferMgmtType tlsBufferMgmtIdx = TcpIp_GetTlsBufferMgmtIdxOfTlsConnection(TlsConIdx);

  TcpIp_TlsBufferRxAdHsEndIdxOfTlsBufferMgmtType tlsBufferRxAdHsEndIdx
    = TcpIp_GetTlsBufferRxAdHsEndIdxOfTlsBufferMgmt(tlsBufferMgmtIdx);

  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check message length. */
  if (TCPIP_TLS_RXBUFFER_ISIDXINRANGE(TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx),
    TCPIP_TLS_CERTCHAINLEN_LEN, tlsBufferRxAdHsEndIdx)
    && (CertificateMsgLen >= TCPIP_TLS_CERTCHAINLEN_LEN))
  {
    /* Read the length of the certificate chain */
    *CertificateChainLenPtr = (uint16)IpBase_GetUint24(TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx)), 0);
    TcpIp_AddTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx, TCPIP_TLS_CERTCHAINLEN_LEN);

    /* #20 Check whether the length of the certificate chain and the length of the length field add up to the length of the message. */
    if ((*CertificateChainLenPtr + TCPIP_TLS_CERTCHAINLEN_LEN) == CertificateMsgLen)
    {
      /* #30 Get number of received certificates and extract the certificate data. */
      if (TcpIp_Tls12Msg_GetNumOfReceivedCertificates(TlsConIdx,
        TcpIp_GetTlsBufferRxAdHsReadIdxOfTlsBufferMgmtDyn(tlsBufferMgmtIdx), *CertificateChainLenPtr, CertificateMsgLen,
        NumCertsPtr) == E_OK)
      {
        /* The number of certificates could be evaluated. */
        retVal = E_OK;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCtx_CalcDhSharedSecret
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_Tls12CoreCtx_CalcDhSharedSecret(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx =
    TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));
  uint32           publicKeyLen;
  TCPIP_P2C(uint8) publicKeyPtr   = NULL_PTR;
  uint8            ecdhPubKeyBuf[(uint32) TCPIP_TLS_MAX_SIGNATURE_LEN] = { 0u };
  uint32           ecdhPubKeyBufLen   = (uint32) TCPIP_TLS_MAX_SIGNATURE_LEN;
  uint16           localErrorId       = TCPIP_TLS_ERRORID_NOT_SET;                                                      /* PRQA S 2981 */ /* MD_TCPIP_Rule2.2_RedundantAssignment */
  Std_ReturnType   retVal             = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* Check if the necessary key for the DH operation is configured */
  if (TcpIp_IsCsmKeyIdKeyExchangeUsedOfTlsHandshake(activeTlsHandshakeIdx) == TRUE )
  {
    retVal = TcpIp_Tls12CoreCtx_SetPublicKeyPtrAndLength(TlsConIdx, &publicKeyPtr, &publicKeyLen,
      &ecdhPubKeyBuf[0], &ecdhPubKeyBufLen);

    /* #10 Forward the public key to the CSM to calculate the Diffie-Hellman shared secret */
    if ( retVal == E_OK)
    {
      retVal = TcpIp_TlsCoreCtx_ForwardKeyToCsmForDhSharedSecretCalculation(TlsConIdx, publicKeyPtr,
        publicKeyLen, &localErrorId);
    }
    else
    {
      /* Error crypto CSM call failed in prepared keys */
      localErrorId = TCPIP_TLS_ERRORID_CRYPTO_CSM_CALL_FAILED;
    }


    if (retVal == E_OK)
    {
      /* Set the async state to DONE */
      TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);
    }
    else
    {
      /* otherwise, handles error while async CSM processing */
      TcpIp_TlsAsyncCtx_HandleErrorDuringAsyncJobProcessing(TlsConIdx,
        TCPIP_TLS_FCTID_CORE_ASYNCCTX_CALC_DH_SHAREDSECRET, localErrorId);
    }
  }
  else
  {
    /* CSM Key for DH operation is not configured - invalid configuration */
    TcpIp_TlsCoreUtil_HandleInvalidConfiguration(TlsConIdx);
  }

} /* TcpIp_Tls12CoreCtx_CalcDhSharedSecret() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCtx_SetPublicKeyPtrAndLength
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
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCtx_SetPublicKeyPtrAndLength(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2V(TcpIp_Uint8P2C)   PublicKeyPtrPtr,
  TCPIP_P2V(uint32)           PublicKeyLenPtr,
  TCPIP_P2V(uint8)            EcdhPubKeyBufPtr,
  TCPIP_P2V(uint32)           EcdhPubKeyBufLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx =
    TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));
  Std_ReturnType   retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
#   if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
#    if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
  if (TcpIp_GetModeOfTlsConnection(TlsConIdx) == TCPIP_TLS_SERVER)
#    endif
  {
    /* Mode == TCPIP_TLS_SERVER */
    TcpIp_TlsServerBufferMgmtIterType tlsServerBufferMgmtIdx = TcpIp_GetTlsServerBufferMgmtIdxOfTlsConnection(TlsConIdx);

    /* #10 Set public key pointer and length of the public key to the received client key */
    /* Pointer to the Client public key which is located in the RX Buffer */
    *PublicKeyPtrPtr = TcpIp_GetAddrTlsBufferRx(
      TcpIp_GetTlsBufferRxClientKeyExchangePubKeyIdxOfTlsServerBufferMgmt(tlsServerBufferMgmtIdx));

    *PublicKeyLenPtr = (TcpIp_GetTlsBufferRx(
      TcpIp_GetTlsBufferRxClientKeyExchangePubKeyLenIdxOfTlsServerBufferMgmt(tlsServerBufferMgmtIdx)));

    /* #20 in case of ECDH key exchange extract the private server key from the certificate */
    if (TcpIp_TlsCoreUtil_ReturnActiveCipherSpecificKeyExchangeMethod(TlsConIdx) == TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH)
    {
      /* Private key of server certificate is stored within the signature generate key */
      TcpIp_TlsCsmJobIterType signatureGenerateTlsCsmJobIdx = (TcpIp_TlsCsmJobIterType)TcpIp_GetCsmJobId(
        TcpIp_GetCsmJobIdKeyExchangeSignatureGenerateIdxOfTlsHandshake(activeTlsHandshakeIdx));

      TcpIp_CsmKeyIdType serverPrivKeyId = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(signatureGenerateTlsCsmJobIdx);

      TcpIp_CsmKeyIdType keyExchangeCsmKey =
        TcpIp_GetCsmKeyId(TcpIp_GetCsmKeyIdKeyExchangeIdxOfTlsHandshake(activeTlsHandshakeIdx));

      TcpIp_TlsEllipticCurveIdType activeCurveId =
        TcpIp_GetEllipticCurveIdOfTlsHandshake(activeTlsHandshakeIdx);

      uint8 privateKeyLength = 0u;

      if(TcpIp_Tls12CoreCtx_GetPrivateKeyLength(activeCurveId, &privateKeyLength) == E_OK)
      {
        /* We copy the private key of the server, which is used instead of an ephemeral key, into the key exchange key element. */
        retVal = TCPIP_CSM_KeyElementCopyPartial(serverPrivKeyId, CRYPTO_KE_SIGNATURE_KEY,
          0u, 0u, privateKeyLength, keyExchangeCsmKey, CRYPTO_KE_KEYEXCHANGE_PRIVKEY);
      }
    }
    /* ECDHE key exchange: Ephemeral key already is generated into the keyExchange key and therefore does not have to be copied any more */
  }
#    if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
  else
#    endif
#   endif
#   if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
  {
    /* Mode == TCPIP_TLS_CLIENT */

    /* #30 Set public key pointer and length of the public key depending on the selected key exchange method */
    if (TcpIp_TlsCoreUtil_ReturnActiveCipherSpecificKeyExchangeMethod(TlsConIdx)
      == TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH)
    {
      /* ECDH Key exchange */
      /* in case of an ECDH cipher suite, the received server certificate contains the public key of the server */

      /* If dynamic certificate groups are used, the Csm signature verify key reference cannot be determined from a
       * static configured handshake container. This is because only at runtime you know in which certificate slot
       * KeyM will store the leaf certificate, which is the first received certificate according to RFC5246 7.4.2.
       * Therefore get the certificate ID of the first received certificate and its configured Csm key Id.
       * Both is stored in TlsKeyMCertificate. */
      const TcpIp_TlsKeyMCertificateIterType tlsKeyMCertificateIdx
        = (TcpIp_TlsKeyMCertificateIterType)(TcpIp_GetAddrTlsCertValidationStatusDyn(
          TcpIp_GetTlsCertValidationStatusDynStartIdxOfTlsConnection(TlsConIdx))->CertId);
      const TcpIp_CsmJobIdType csmKeyIdOfRxServerCert = TcpIp_GetCsmCertSignVerifyKeyIdOfTlsKeyMCertificate(tlsKeyMCertificateIdx);

      /* copy the key from the server certificate to the key element of the key exchange key */
      retVal = TCPIP_CSM_KeyElementGet(csmKeyIdOfRxServerCert, CRYPTO_KE_CERTIFICATE_SUBJECT_PUBLIC_KEY,
        EcdhPubKeyBufPtr, EcdhPubKeyBufLenPtr);

      *PublicKeyPtrPtr     =  EcdhPubKeyBufPtr;
      *PublicKeyLenPtr     = *EcdhPubKeyBufLenPtr;

      if (   (TcpIp_GetEllipticCurveIdOfTlsHandshake(activeTlsHandshakeIdx) == TCPIP_CURVE_ID_SECP256R1)
          || (TcpIp_GetEllipticCurveIdOfTlsHandshake(activeTlsHandshakeIdx) == TCPIP_CURVE_ID_SECP521R1))
      {
        /* In case of ECDH with a SECP curve we have to add +1 to the size of the public key (compression flag) */
        *PublicKeyLenPtr += TCPIP_TLS_KEYEXCH_COMPFLAGLEN;
      }
    }
    else
    {
      /* ECDHE key exchange */
      TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

      /* #40 Set public key pointer and key length to the server key received in the ServerKeyExchange message. */
      /* Pointer to the Server public key which is located in the RX Buffer */
      *PublicKeyPtrPtr = TcpIp_GetAddrTlsBufferRx(
        TcpIp_GetTlsBufferRxServerKeyExchangePubKeyIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx));

      /* Public key length is checked in function TcpIp_Tls12ClientMsg_ServerKeyExchangeMsgGetPubKey() against its boundaries */
      *PublicKeyLenPtr = (TcpIp_GetTlsBufferRx(
        TcpIp_GetTlsBufferRxServerKeyExchangePubKeyLenIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)));
    }
  }
#   else
  TCPIP_DUMMY_STATEMENT(*EcdhPubKeyBufPtr);                                                                             /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  TCPIP_DUMMY_STATEMENT(*EcdhPubKeyBufLenPtr);                                                                          /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#   endif

  /* #50 Check if the compression flag is present in the message */
  if (TcpIp_TlsCoreUtil_CheckPublicKeyCompressionFlagOfEllipticCurve(
    TcpIp_GetEllipticCurveIdOfTlsHandshake(activeTlsHandshakeIdx)) == TRUE)
  {
    *PublicKeyLenPtr -= TCPIP_TLS_KEYEXCH_COMPFLAGLEN;
  }

  return retVal;

} /* TcpIp_Tls12CoreCtx_SetPublicKeyPtrAndLength() */

#   if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCtx_GetPrivateKeyLength
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCtx_GetPrivateKeyLength(
  TcpIp_TlsEllipticCurveIdType ActiveCurveId,
  TCPIP_P2V(uint8) PrivateKeyLengthPtr)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Determine the length of the private key depending on the used curve */
  if(ActiveCurveId == TCPIP_CURVE_ID_SECP256R1)
  {
    *PrivateKeyLengthPtr = TCPIP_CURVE_ID_SECP256R1_PRIVKEY_LEN;
    retVal = E_OK;
  }
  else if (ActiveCurveId == TCPIP_CURVE_ID_SECP521R1)
  {
    *PrivateKeyLengthPtr = TCPIP_CURVE_ID_SECP521R1_PRIVKEY_LEN;
    retVal = E_OK;
  }
  else if (ActiveCurveId == TCPIP_CURVE_ID_SECP384R1)
  {
    *PrivateKeyLengthPtr = TCPIP_CURVE_ID_SECP384R1_PRIVKEY_LEN;
    retVal = E_OK;
  }
  else if (ActiveCurveId == TCPIP_CURVE_ID_X25519)
  {
    *PrivateKeyLengthPtr = TCPIP_CURVE_ID_X25519_PRIVKEY_LEN;
    retVal = E_OK;
  }
  else
  {
    /* retVal = E_NOT_OK */
  }

  return retVal;
} /* TcpIp_Tls12CoreCtx_GetPrivateKeyLength */
#   endif /* (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON) */
#  endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

#  if ((TCPIP_SUPPORT_ASRTLSSERVER_PSK == STD_ON) || (TCPIP_SUPPORT_ASRTLSCLIENT_PSK == STD_ON))
/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCtx_CalcPskPremasterSecret
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
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_Tls12CoreCtx_CalcPskPremasterSecret(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* General structure of PSK premaster secrets (PMS) */
  /*        ----------------------------------------------------------------
   *        | uint16 M | other_secret (M bytes) | uint16 N | PSK (N bytes) |
   *        ----------------------------------------------------------------
   *        |<--             PMS First Part             -->|
   *        |<--             PMS                                         -->|
   * In the following the array "pmsFirstPart" holds all fields except the PSK field
   */

  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsPskIdentityActiveIdxOfTlsConnectionDynType activeTlsPskIdentityIdx =
      TcpIp_GetTlsPskIdentityActiveIdxOfTlsConnectionDyn(TlsConIdx);
  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx = TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(
                                                         TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));
  TcpIp_PskKeyLengthOfTlsPskIdentityType pskKeyLength = TcpIp_GetPskKeyLengthOfTlsPskIdentity(activeTlsPskIdentityIdx);
  TcpIp_TlsCsmKeyInfoType pmsKey;
  TcpIp_TlsCsmKeyInfoType pskKey;

  /* The preMasterSecretPart1Buffer takes two 2 byte length fields and M bytes for other_secret. For key exchange method
     PSK (plain) other_secret has the same length as the pre-shared key, for others it would be shorter than the PSK. */
  uint8 pmsFirstPart[TCPIP_TLS_PSK_PMS_OTHER_SECRET_LEN_FIELD_LEN + TCPIP_TLS_PSK_PMS_PSK_LEN_FIELD_LEN
                     + TCPIP_TLS_PSK_MAX_KEY_LENGTH];
  TCPIP_P2V(uint8) pmsFirstPartPtr = &pmsFirstPart[0];
  uint8 pmsFirstPartLength; /* length depends on active PSK cipher suite */

  uint16 localErrorId = TCPIP_TLS_ERRORID_NOT_SET;                                                                      /* PRQA S 2981 */ /* MD_TCPIP_Rule2.2_RedundantAssignment */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the necessary key for the PSK PreMasterSecret operation is configured */
  if (TcpIp_IsCsmKeyIdKeyExchangeUsedOfTlsHandshake(activeTlsHandshakeIdx) == TRUE)
  {
    /*#20 Retrieve pre-shared key from active PSK Identity */
    pskKey.KeyId = TcpIp_GetPskCsmKeyIdOfTlsPskIdentity(activeTlsPskIdentityIdx);
    pskKey.KeyElementId = CRYPTO_KE_CIPHER_KEY;
    pskKey.KeyLen = (uint32)pskKeyLength;

    /* #30 Get the key for premaster secret calculation */
    pmsKey.KeyId = TcpIp_GetCsmKeyId(TcpIp_GetCsmKeyIdKeyExchangeIdxOfTlsHandshake(activeTlsHandshakeIdx));
    pmsKey.KeyElementId = CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE;

    /* #40 Depending on the key exchange method, build the first part of the PMS */

    {
      /* For plain PSK, structure of premaster secret  with PSK key length N bytes:
       * -----------------------------------------------------------
       * | uint16 N       | N zero octets | uint16 N |   N octets  |
       * | oth_secret_len |  other_secret |  psk_len |     psk     |
       * -----------------------------------------------------------
       * |<--            PMS First Part           -->|
       * |<--            PMS                                    -->|
       */

      /* For Plain psk, build the first part of the PMS as described above */

      /* For plain psk, length of other_secret is same as psk length */
      pmsFirstPartLength = TCPIP_TLS_PSK_PMS_OTHER_SECRET_LEN_FIELD_LEN + pskKeyLength + TCPIP_TLS_PSK_PMS_PSK_LEN_FIELD_LEN;


      /* Set length field of other secret */
      IpBase_PutUint16(pmsFirstPartPtr, 0, (uint16)pskKey.KeyLen);

      /* Set psk_len bytes of zero as other_secret */
      VStdMemSet(&pmsFirstPartPtr[TCPIP_TLS_PSK_PMS_OTHER_SECRET_LEN_FIELD_LEN],                                        /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        TCPIP_TLS_PSK_PMS_OTHER_SECRET_PLAIN_PATTERN, pskKeyLength);

      /* Set length field of psk (psk_len) */
      IpBase_PutUint16(pmsFirstPartPtr, (TCPIP_TLS_PSK_PMS_OTHER_SECRET_LEN_FIELD_LEN + pskKey.KeyLen), (uint16)pskKey.KeyLen);
    }

    /* #50 Set other secret followed by psk into the pms key, and set to valid */
    if (TCPIP_CSM_KeyElementSet(pmsKey.KeyId, pmsKey.KeyElementId, pmsFirstPartPtr, pmsFirstPartLength) == E_OK)
    {
      /* Copy psk into pms key element */
      if (TCPIP_CSM_KeyElementCopyPartial(pskKey.KeyId, pskKey.KeyElementId, 0, pmsFirstPartLength, pskKey.KeyLen, pmsKey.KeyId,
        pmsKey.KeyElementId)
        == E_OK)
      {
        /* Set Csm Key of pms to valid */
        if (TCPIP_CSM_KeySetValid(pmsKey.KeyId) == E_OK)
        {
          retVal = E_OK;
        }
        else
        {
          /* Error SetValid */
          localErrorId = TCPIP_TLS_ERRORID_CRYPTO_SET_VALID_FAILED;
        }
      }
      else
      {
        /* Error Copy Partial */
        localErrorId = TCPIP_TLS_ERRORID_CRYPTO_CSM_KEYELEMENTCOPYPARTIAL_FAILED;
      }
    }
    else
    {
      /* Error KeyElement Set */
      localErrorId = TCPIP_TLS_ERRORID_CRYPTO_CSM_KEYELEMENTSET_FAILED;
    }

    /* #60 Set Async state to DONE if all sub-functions returned E_OK */
    if (retVal == E_OK)
    {
      /* Set the async state to DONE */
      TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);
    }
    else
    {
      /* otherwise, handles error while async CSM processing */
      TcpIp_TlsAsyncCtx_HandleErrorDuringAsyncJobProcessing(TlsConIdx, TCPIP_TLS_FCTID_CORE_CRYPTO_PSK_CALC_PREMASTERSECRET,
        localErrorId);
    }
  }
  else
  {
    /* #70 CSM Key for PSK PreMasterSecret operation is not configured - invalid configuration */
    TcpIp_TlsCoreUtil_HandleInvalidConfiguration(TlsConIdx);
  }
} /* TcpIp_Tls12CoreCtx_CalcPskPremasterSecret() */
#  endif /* (TCPIP_SUPPORT_ASRTLSSERVER_PSK == STD_ON) || (TCPIP_SUPPORT_ASRTLSCLIENT_PSK == STD_ON) */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCtx_CalcMasterSecret
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
/* PRQA S 6030, 6050 1 */ /* MD_TcpIp_TlsStatemachine_STCYC_6030, MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_Tls12CoreCtx_CalcMasterSecret(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 State machine for key material calculation */
  while (TcpIp_GetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx) != TCPIP_TLS_STATE_CALC_KEY_MATERIAL_DONE)
  {
    switch (TcpIp_GetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx))
    {
      case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_INIT:
      {
        /* #20 In INIT state, move to the next state depending on the key exchange method */
        TcpIp_TlsKeyExchangeMethodDataType keyExchangeMethod = TcpIp_TlsCoreUtil_ReturnActiveCipherSpecificKeyExchangeMethod(TlsConIdx);

        switch (keyExchangeMethod)
        {
#  if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
          /* ECDH(E) methods */
          case TCPIP_TLS_KEYEXCHANGE_METHOD_ECDHE:
          case TCPIP_TLS_KEYEXCHANGE_METHOD_ECDH:
          {
            TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_REDUCE_SHARED_KEY);
            break;
          }
#  endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */
          /* PSK method */
          case TCPIP_TLS_KEYEXCHANGE_METHOD_PSK_PLAIN:
          {
            TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_CALC_MASTER_SECRET);
            break;
          }
          /* Unsupported method */
          default:
          {
            /* Invalid key exchange method. This can never happen, a valid key exchange method has been chosen with the
            active cipher suite received in the server_hello message. */
            break;
          }
        }  /* switch (keyExchangeMethod) */

        break;
      } /* case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_INIT */

#  if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
      case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_REDUCE_SHARED_KEY:
      {
        /* #30 In REDUCE_SHARED_KEY state, for ECDH(E), calculate the reduced shared key */
        if (TcpIp_Tls12CoreCrypto_ReduceSharedKey(TlsConIdx) == E_OK)
        {
          TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_CALC_MASTER_SECRET);
        }
        else
        {
          TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_ABORT);
        }
        break;
      } /* case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_REDUCE_SHARED_KEY */
#  endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

      case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_CALC_MASTER_SECRET:
      {
        /* #40 In CALC_MASTER_SECRET state, Calculate the master secret */
        if (TcpIp_Tls12CoreCrypto_CalcMasterSecret(TlsConIdx) == E_OK)
        {
          TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_DERIVE_KEY_BLOCK);
        }
        else
        {
          TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_ABORT);
        }
        break;
      } /* case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_CALC_MASTER_SECRET */

      case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_DERIVE_KEY_BLOCK:
      {
        /* #50 In DERIVE_KEY_BLOCK state, derive the key block */
        if (TcpIp_Tls12CoreCrypto_DeriveKeyBlock(TlsConIdx) == E_OK)
        {
          TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_SET_CSM_KEYS);
        }
        else
        {
          TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_ABORT);
        }
        break;
      } /* case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_DERIVE_KEY_BLOCK */

      case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_SET_CSM_KEYS:
      {
        /* #60 In SET_CSM_KEYS state, set the Csm key elements */
        if (TcpIp_Tls12CoreCrypto_SetCsmKeys(TlsConIdx) == E_OK)
        {
          TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_FINALIZE);
        }
        else
        {
          TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_ABORT);
        }
        break;
      } /* case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_SET_CSM_KEYS */

      case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_FINALIZE:
      {
        /* #70 In MATERIAL_FINALIZE state, finalize the state machine handling */
        /* reset the Async trigger flag */
        TcpIp_SetCalcMasterSecretAsyncFlagOfTlsConnectionDyn(TlsConIdx, FALSE);
        /* Set the async state to DONE */
        TcpIp_SetHsAsyncCtxStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_CTX_DONE);
        TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_DONE);
        break;
      } /* case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_FINALIZE */

      case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_ABORT:
      {
        /* #80 If MATERIAL_ABORT state is hit, abort the handshake */
        /* User error has already been reported, just trigger handshake abort */
        TcpIp_TlsAsyncCtx_HandleErrorDuringAsyncJobProcessing(TlsConIdx, TCPIP_TLS_FCTID_NOT_SET, TCPIP_TLS_ERRORID_NOT_SET);
        TcpIp_SetCalcKeyMaterialStateOfTlsConnectionDyn(TlsConIdx, TCPIP_TLS_STATE_CALC_KEY_MATERIAL_DONE);
        break;
      } /* case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_ABORT */

      case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_DONE:
      {
        /* #90 DONE state, exit state machine */
        break;
      } /* case TCPIP_TLS_STATE_CALC_KEY_MATERIAL_DONE */
      default:
      {
        /* do nothing - unreachable code */
        break;
      }
    } /* switch() */
  } /* while() */
} /* TcpIp_Tls12CoreCtx_CalcMasterSecret() */

/**********************************************************************************************************************
 *  TLS CORE MAIN FUNCTION RX
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE MAIN FUNCTION TX
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE STATES AND EVENTS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CONNECTION STATE HANDLER
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE ERROR
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TLS CORE CRYPTO
 *********************************************************************************************************************/
#  if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
*  TcpIp_Tls12CoreCrypto_ReduceSharedKey
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
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_ReduceSharedKey(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx =
    TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));
  TcpIp_TlsCsmKeyInfoType tmpKey; /* Temporary CSM key element */
  TcpIp_TlsCsmKeyInfoType pmsKey; /* Pre-master secret CSM key element (DH-Shared Value) */
  Std_ReturnType csmRetVal;
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the necessary keys for the MasterSecret operation are configured */
  if (TcpIp_IsCsmKeyIdKeyExchangeUsedOfTlsHandshake(activeTlsHandshakeIdx) == TRUE)
  {
    tmpKey.KeyId        = TcpIp_GetHsTemporaryBufferKeyIdOfTlsHandshake(activeTlsHandshakeIdx);
    tmpKey.KeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
    tmpKey.KeyLen       = 0;

    pmsKey.KeyId        = TcpIp_GetCsmKeyId(TcpIp_GetCsmKeyIdKeyExchangeIdxOfTlsHandshake(activeTlsHandshakeIdx));
    pmsKey.KeyElementId = CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE;
    pmsKey.KeyLen
      = TcpIp_Tls12Msg_GetLenOfCurveSpecificDhSharedKey(TcpIp_GetEllipticCurveIdOfTlsHandshake(activeTlsHandshakeIdx));

    /* Reduce Key */
    /* Clear dummy buffer, set one dummy byte to reduce the length (CSM cannot delete a key) */

    csmRetVal = TcpIp_TlsCoreCrypto_ClearCsmKey(TlsConIdx, &tmpKey);
    /* Copy pre-master-secret to the dummy buffer */
    csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(pmsKey.KeyId, pmsKey.KeyElementId, 0, 0, pmsKey.KeyLen, tmpKey.KeyId, tmpKey.KeyElementId);
    csmRetVal |= TCPIP_CSM_KeySetValid(tmpKey.KeyId);

    /* Copy back the reduced key */
    csmRetVal |= TCPIP_CSM_KeyElementCopy(tmpKey.KeyId, tmpKey.KeyElementId, pmsKey.KeyId, pmsKey.KeyElementId);
    csmRetVal |= TCPIP_CSM_KeySetValid(pmsKey.KeyId);

    /* #20 Check if the result of the CSM operation is E_OK */
    retVal = TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_REDUCEKEY);

    if (retVal != E_OK)
    {
      /* #30 if not so, report the user error */
      TcpIp_TlsUserErrorType userError;

      userError.FunctionId = TCPIP_TLS_FCTID_CORE_CRYPTO_REDUCEKEY;
      userError.UserErrorId = TCPIP_TLS_ERRORID_CRYPTO_REDUCESHAREDKEY_FAILED;
      userError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

      TcpIp_TlsCoreError_ReportUserError(userError);
    }
  }
  else
  {
    /* #40 CSM Key for ReduceKey operation is not configured - invalid configuration */
    TcpIp_TlsCoreUtil_HandleInvalidConfiguration(TlsConIdx);
    retVal = E_NOT_OK;
  }
  return retVal;
} /* TcpIp_Tls12CoreCrypto_ReduceSharedKey() */
#  endif /* TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_CalcMasterSecret
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
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_CalcMasterSecret(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx =
    TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));
  TcpIp_TlsCsmKeyInfoType msKey; /* Master secret CSM key element */
  TcpIp_TlsCsmKeyInfoType pmsKey; /* Premaster secret CSM key element (DH-Shared Value) */
  uint8  seed[2u * TCPIP_TLS_RANDOM_LEN] = { 0 };
  uint8  seedLen = 2u * TCPIP_TLS_RANDOM_LEN;
  Std_ReturnType prfRetVal;
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the necessary keys for the MasterSecret operation are configured */
  if (   (TcpIp_IsCsmKeyIdMasterSecretUsedOfTlsHandshake(activeTlsHandshakeIdx) == TRUE)
      && (TcpIp_IsCsmKeyIdKeyExchangeUsedOfTlsHandshake(activeTlsHandshakeIdx)   == TRUE))                              /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
  {

    msKey.KeyId = TcpIp_GetCsmKeyId(TcpIp_GetCsmKeyIdMasterSecretIdxOfTlsHandshake(activeTlsHandshakeIdx));
    msKey.KeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
    msKey.KeyLen = TCPIP_TLS_MASTER_SECRET_LEN;

    pmsKey.KeyId =  TcpIp_GetCsmKeyId(TcpIp_GetCsmKeyIdKeyExchangeIdxOfTlsHandshake(activeTlsHandshakeIdx));
    pmsKey.KeyElementId = CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE;


    /* #20 Copy client and server random into one array (seed) */
#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
#   if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
    if (TcpIp_GetModeOfTlsConnection(TlsConIdx) == TCPIP_TLS_SERVER)
#   endif
    {
      /* Mode == TCPIP_TLS_SERVER */
      TcpIp_TlsServerBufferMgmtIterType tlsServerBufferMgmtIdx = TcpIp_GetTlsServerBufferMgmtIdxOfTlsConnection(TlsConIdx);

      VStdMemCpy(&seed[0], TcpIp_GetAddrTlsBufferRx(TcpIp_GetTlsBufferRxClientRandomIdxOfTlsServerBufferMgmt(tlsServerBufferMgmtIdx)),/* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        TCPIP_TLS_RANDOM_LEN);
      VStdMemCpy(&seed[TCPIP_TLS_RANDOM_LEN],                                                                           /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        TcpIp_GetAddrTlsBufferTx(TcpIp_GetTlsBufferTxServerRandomIdxOfTlsServerBufferMgmt(tlsServerBufferMgmtIdx)),
        TCPIP_TLS_RANDOM_LEN);
    }
#   if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
    else
#   endif
#  endif
#  if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
    {
      /* Mode == TCPIP_TLS_CLIENT */
      TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

      VStdMemCpy(&seed[0], TcpIp_GetAddrTlsBufferTx(TcpIp_GetTlsBufferTxClientRandomIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)),/* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        TCPIP_TLS_RANDOM_LEN);
      VStdMemCpy(&seed[TCPIP_TLS_RANDOM_LEN],                                                                           /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        TcpIp_GetAddrTlsBufferRx(TcpIp_GetTlsBufferRxServerRandomIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)),
        TCPIP_TLS_RANDOM_LEN);
    }
#  endif

    /* Invoke the crypto PRF function to generate the master secret */
    prfRetVal = TcpIp_Tls12CoreCrypto_Prf(TlsConIdx, &msKey, &pmsKey, &TcpIp_Tls_Hs_MasterSecretLabel[0], &seed[0],
      TCPIP_TLS_LABEL_MASTER_SECRET_LEN, seedLen, TRUE);

    /* #30 Check if the result of the CSM operation is E_OK */
    retVal = TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, prfRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_CALCMASTERSECRET);

#  if (TCPIP_SUPPORT_ASRTLS_SECRETS_NSS_FRAME == STD_ON)
    if (retVal == E_OK)
    {
      TcpIp_SetNssMasterSecretReadyToSendOfTlsConnectionDyn(TlsConIdx, TRUE);
    }
#  endif
#  if (TCPIP_SUPPORT_ASRTLS_SECRET_ACCESS == STD_ON)
    if (TcpIp_IsTlsUseKeyUpdateCalloutOfTlsConfig(TCPIP_TLSCONFIG_IDX))
    {
      (void)TcpIp_TlsCore_IssueKeyUpdateCallout(TlsConIdx, TCPIP_TLS_KEYUPDATE_NONE_TO_APPLICATION);
    }
#  endif

    if (retVal != E_OK)
    {
      /* #40 if not so, report the user error */
      TcpIp_TlsUserErrorType userError;

      userError.FunctionId = TCPIP_TLS_FCTID_CORE_CRYPTO_CALCMASTERSECRET;
      userError.UserErrorId = TCPIP_TLS_ERRORID_CRYPTO_CALCMASTERSECRET_FAILED;
      userError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

      TcpIp_TlsCoreError_ReportUserError(userError);
    }
  }
  else
  {
    /* #50 CSM Key for MasterSecret operation is not configured - invalid configuration */
    TcpIp_TlsCoreUtil_HandleInvalidConfiguration(TlsConIdx);
    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_Tls12CoreCrypto_CalcMasterSecret() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_DeriveKeyBlock
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_DeriveKeyBlock(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType csmRetVal;
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the necessary KeyBlock size for this TLS-Connection */
  csmRetVal = TcpIp_Tls12CoreCrypto_CalcKeyBlockSize(TlsConIdx);

  /* #20 Calculate the KeyBlock for this TLS-Connection */
  if (csmRetVal == E_OK)
  {
    csmRetVal = TcpIp_Tls12CoreCrypto_CalcKeyBlock(TlsConIdx);
  }

  /* #30 Check the result of the CSM operation */
  retVal = TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_DERIVEKEYBLOCK);

  return retVal;
} /* TcpIp_Tls12CoreCrypto_DeriveKeyBlock() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_SetCsmKeys
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_SetCsmKeys(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the configuration of the necessary CSM keys and CSM jobs is valid */
  if (TcpIp_Tls12CoreCrypto_CheckCsmKeyConfiguration(TlsConIdx) == E_OK)
  {
    /* #20 If the configuration is valid, set the CSM Keys depending on the TLS-Connection mode (Server/Client) */
#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
#   if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
    if (TcpIp_GetModeOfTlsConnection(TlsConIdx) == TCPIP_TLS_SERVER)
#   endif
    {
      /* Connection is configured as TLS-Server */
      retVal = TcpIp_TlsServerCrypto_SetCsmKeys(TlsConIdx);
    }
#   if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
    else
#   endif
#  endif
#  if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
    {
      /* Connection is configured as TLS-Client */
      retVal = TcpIp_Tls12ClientCrypto_SetCsmKeys(TlsConIdx);
    }
#  endif
  }
  else
  {
    /* #30 Handle invalid configuration */
    TcpIp_TlsCoreUtil_HandleInvalidConfiguration(TlsConIdx);
  }
  return retVal;
} /* TcpIp_Tls12CoreCrypto_SetCsmKeys() */


/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_CheckCsmKeyConfiguration
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_CheckCsmKeyConfiguration(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsCipherStreamingModeDataType selectedStreamingMode = TcpIp_TlsCoreUtil_ReturnSelectedCipherSpecificStreamingMode(TlsConIdx);

  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx
    = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);

  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx
    = TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(activeTlsCipherWorkerIdx);

  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the connection generic CSM Keys */
  if (   (TcpIp_IsCsmKeyIdMasterSecretUsedOfTlsHandshake(activeTlsHandshakeIdx) == TRUE)                                /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
      && (TcpIp_IsCsmKeyIdKeyExchangeUsedOfTlsHandshake(activeTlsHandshakeIdx)  == TRUE))
  {
    /* Generic CSM Keys are valid */
    retVal = E_OK;
  }

  if (retVal == E_OK)
  {
    /* #20 Check streaming depended CSM Key and CSM Jobs */
    switch (selectedStreamingMode)
    {
      case TCPIP_TLS_STREAMING_MODE_AES_128_CBC:
      {
        /* X_WITH_AES_128_CBC_SHAX
             TCPIP_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA
             TCPIP_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
             TCPIP_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA
             TCPIP_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
        */
        /* Check necessary CSM Job configuration */
        if (   (TcpIp_IsCsmJobIdHmacGenerateUsedOfTlsCipherWorker(activeTlsCipherWorkerIdx)  == TRUE)                   /* PRQA S 3415 4 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
            && (TcpIp_IsCsmJobIdHmacVerifyUsedOfTlsCipherWorker(activeTlsCipherWorkerIdx)    == TRUE)
            && (TcpIp_IsCsmJobIdCipherEncryptUsedOfTlsCipherWorker(activeTlsCipherWorkerIdx) == TRUE)
            && (TcpIp_IsCsmJobIdCipherDecryptUsedOfTlsCipherWorker(activeTlsCipherWorkerIdx) == TRUE))
        {
          retVal = E_OK;
        }
        break;
      } /* case TCPIP_TLS_STREAMING_MODE_AES_128_CBC */

      case TCPIP_TLS_STREAMING_MODE_AES_128_GCM:
      case TCPIP_TLS_STREAMING_MODE_AES_256_GCM:
      case TCPIP_TLS_STREAMING_MODE_CHACHA20_POLY1305:
      {

        /* X_WITH_AES_128_GCM_SHAX
             TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA
             TCPIP_TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256
             TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA
             TCPIP_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
        */
        /* Check necessary CSM Job configuration */
        if (   (TcpIp_IsCsmJobIdCipherEncryptUsedOfTlsCipherWorker(activeTlsCipherWorkerIdx) == TRUE)                   /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
            && (TcpIp_IsCsmJobIdCipherDecryptUsedOfTlsCipherWorker(activeTlsCipherWorkerIdx) == TRUE))
        {
          retVal = E_OK;
        }
        break;
      } /* case TCPIP_TLS_STREAMING_MODE_AES_128_CBC */

      case TCPIP_TLS_STREAMING_MODE_NULL:
      {
        /* X_WITH_NULL_SHAX
             TCPIP_TLS_ECDH_ECDSA_WITH_NULL_SHA
        */
        /* Check necessary CSM Job configuration */
        if (   (TcpIp_IsCsmJobIdHmacGenerateUsedOfTlsCipherWorker(activeTlsCipherWorkerIdx) == TRUE)                    /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
            && (TcpIp_IsCsmJobIdHmacVerifyUsedOfTlsCipherWorker(activeTlsCipherWorkerIdx)   == TRUE))
        {
          retVal = E_OK;
        }
        break;
      } /* case TCPIP_TLS_STREAMING_MODE_NULL */
      default:                                                                                                             /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
      {
        /* Default case left blank intentionally */
        break;
      }
    } /* switch */
  }

  return retVal;
} /* TcpIp_Tls12CoreCrypto_CheckCsmKeyConfiguration() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_CalcKeyBlockSize
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_CalcKeyBlockSize(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsCipherWorkerIterType activeTlsCipherWorkerIdx
    = TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);

  TcpIp_TlsCipherStreamingModeDataType activeStreamingMode
    = TcpIp_TlsCoreUtil_ReturnSelectedCipherSpecificStreamingMode(TlsConIdx);

  TcpIp_CipherMacKeyLengthOfTlsCipherWorkerType activeMacLength = TcpIp_GetCipherMacKeyLengthOfTlsCipherWorker(
        activeTlsCipherWorkerIdx);

  uint32 keyBlockSize   = TCPIP_TLS_KEY_BLOCK_SIZE_UNSET;                                                               /* PRQA S 2981 */ /* MD_TCPIP_Rule2.2_RedundantAssignment */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the KeyBlock size depending on the active streaming mode of the connection */
  switch (activeStreamingMode)
  {
    case TCPIP_TLS_STREAMING_MODE_AES_128_CBC:
    {
      /* X_WITH_AES_128_CBC_SHAX
           TCPIP_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA
           TCPIP_TLS_ECDH_ECDSA_WITH_AES_128_CBC_SHA256
           TCPIP_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA
           TCPIP_TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256
      */
      if (activeMacLength == TCPIP_TLS_HASH_SHA256_LEN)
      {
        /* KeyBlock:
           * -  0..31: TX MAC
           * - 32..63: RX MAC
           * - 64..79: TX Key
           * - 80..95: RX Key
        */
        keyBlockSize = TCPIP_TLS_KEY_BLOCK_SIZE_AES128_SHA256;
        retVal = E_OK;
      }
      else
      {
        /* KeyBlock:
           * -  0..19: TX MAC
           * - 20..39: RX MAC
           * - 40..55: TX Key
           * - 56..71: RX Key
        */
        keyBlockSize = TCPIP_TLS_KEY_BLOCK_SIZE_AES128_SHA1;
        retVal = E_OK;
      }
      break;
    } /* case TCPIP_TLS_STREAMING_MODE_AES_128_CBC */

    case TCPIP_TLS_STREAMING_MODE_AES_128_GCM:
    {
      /* X_WITH_AES_128_GCM_SHAX
           TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA
           TCPIP_TLS_ECDH_ECDSA_WITH_AES_128_GCM_SHA256
           TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA
           TCPIP_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256
      */
      {
        /* KeyBlock:
           * - 0..15: TX Key
           * - 16..31: RX Key
           * - 32..35: TX IV
           * - 36..39: RX IV
        */
        keyBlockSize = TCPIP_TLS_KEY_BLOCK_SIZE_AES128_GCM;
        retVal = E_OK;
      }

      break;
    } /* case TCPIP_TLS_STREAMING_MODE_AES_128_GCM */

    case TCPIP_TLS_STREAMING_MODE_NULL:
    {
      /* X_WITH_NULL_SHAX
           TCPIP_TLS_ECDH_ECDSA_WITH_NULL_SHA
      */
      if (activeMacLength == TCPIP_TLS_HASH_SHA256_LEN)
      {
        /* KeyBlock:
           * -  0..31: TX MAC
           * - 32..63: RX MAC
        */
        keyBlockSize = TCPIP_TLS_KEY_BLOCK_SIZE_NULL_SHA256;
        retVal = E_OK;
      }
      else
      {
        /* KeyBlock:
           * -  0..19: TX MAC
           * - 20..39: RX MAC
        */
        keyBlockSize = TCPIP_TLS_KEY_BLOCK_SIZE_NULL_SHA1;
        retVal = E_OK;
      }
      break;
    } /* case TCPIP_TLS_STREAMING_MODE_NULL */

    default:                                                                                                           /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
    /* Default case left blank intentionally */
    break;

  } /* switch */

  /* #20 Check if there was an error during the calculation */
  if (retVal == E_OK)
  {
    /* #30 if not safe the actual key block length */
    TcpIp_SetKeyBlockSizeOfTlsConnectionDyn(TlsConIdx, keyBlockSize);
  }

  return retVal;
} /* TcpIp_Tls12CoreCrypto_CalcKeyBlockSize() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_Prf
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_TcpIp_TlsPointerAndLength_STPAR_6060 */
 FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_Prf(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2C(TcpIp_TlsCsmKeyInfoType) ResultKeyIdPtr,
  TCPIP_P2C(TcpIp_TlsCsmKeyInfoType) SecretKeyIdPtr,
  TCPIP_P2C(uint8) LabelPtr,
  TCPIP_P2C(uint8) SeedPtr,
  uint8 LabelLen,
  uint8 SeedLen,
  boolean ResultIsPrivate)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 combinedLabel_Array[TCPIP_TLS_PRF_COMBINED_LABEL_LEN_MAX];
  uint8 combinedLabelLen = LabelLen + SeedLen;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Combine the label and seed, and invoke the pseudo-random function */
  if (combinedLabelLen > 0u)
  {
    VStdMemCpy(combinedLabel_Array, LabelPtr, LabelLen);                                                                /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
    VStdMemCpy(&combinedLabel_Array[LabelLen], SeedPtr, SeedLen);                                                       /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

    /* PRF = P_SHA256(S, Label + seed) */
    retVal = TcpIp_Tls12CoreCrypto_PSha256(TlsConIdx, ResultKeyIdPtr, SecretKeyIdPtr, &combinedLabel_Array[0],
               combinedLabelLen, ResultIsPrivate);
  }
  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_PSha256
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
/* PRQA S 6050, 6060 1 */ /* MD_TcpIp_STCAL_CslAccess, MD_TcpIp_TlsPointerAndLength_STPAR_6060 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_PSha256(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TCPIP_P2C(TcpIp_TlsCsmKeyInfoType) ResultKeyIdPtr,
  TCPIP_P2C(TcpIp_TlsCsmKeyInfoType) SecretKeyIdPtr,
  TCPIP_P2C(uint8) SeedPtr,
  uint16 SeedLen,
  boolean ResultIsPrivate)
{
  /* RFC5246 - Section 5.  HMAC and the Pseudorandom Function
   *
   *  In this section, we define one PRF, based on HMAC. This PRF with the
   *  SHA-256 hash function is used for all cipher suites, therefore P_hash() is P_SHA256().
   *
   *  P_hash(secret, seed) =   HMAC_hash(secret, A(1) + seed) +
   *                           HMAC_hash(secret, A(2) + seed) +
   *                           HMAC_hash(secret, A(3) + seed) + ...      (referenced as outer hash)
   *
   *  where + indicates concatenation.
   *
   *  A() is defined as:
   *
   *    A(0) = seed
   *    A(i) = HMAC_hash(secret, A(i-1))     (referenced as inner hash)
   */

  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx =
    TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));

  TcpIp_CsmKeyIdType keyIdPrfSecret = TCPIP_TLS_CSM_INV_KEY_ID;

  uint32 keyElementIdInput   = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
  uint32 keyElementIdOutput  = TcpIp_GetPrfValueOutputKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
  uint32 keyElementIdActive  = TcpIp_GetPrfValueActiveKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
  uint32 keyElementIdPassive = TcpIp_GetPrfValuePassiveKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
  TcpIp_TlsCsmJobIterType hmacGenerateTlsCsmJobIdx;
  uint32 keyIdHmacGenerate;

  /* API TCPIP_CSM_MacGenerate() has a DET check on the in and out parameters, because it is specified by AUTOSAR.
   * However this check does not make sense in case the buffer is redirected to a key. Because of that a dummy
   * byte has to be provided.
   */
  uint8 dummyBuffer     = 0u;
  uint32 dummyBufferLen = 0u;
  boolean loopEnd       = FALSE;
  uint16 fillLevel      = 0u;
  Std_ReturnType csmRetVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the HMAC job/keys depending on the privacy of the result */
  if (ResultIsPrivate == TRUE)
  {
    /* Private result is used for calculation of MasterSecret and derivation of KeyBlock */
    hmacGenerateTlsCsmJobIdx
      = (TcpIp_TlsCsmJobIterType)TcpIp_GetPrfMacGeneratePrivateJobIdOfTlsHandshake(activeTlsHandshakeIdx);
    keyIdHmacGenerate = TcpIp_GetPrfMacCalculationPrivateKeyIdOfTlsHandshake(activeTlsHandshakeIdx);
  }
  else
  {
    /* Public result is used for calculation of FinishedMessages during TLS handshake */
    hmacGenerateTlsCsmJobIdx
      = (TcpIp_TlsCsmJobIterType)TcpIp_GetPrfMacGeneratePublicJobIdOfTlsHandshake(activeTlsHandshakeIdx);
    keyIdHmacGenerate = TcpIp_GetPrfMacCalculationPublicKeyIdOfTlsHandshake(activeTlsHandshakeIdx);
  }

  keyIdPrfSecret = TcpIp_GetCsmReferencedKeyIdOfTlsCsmJob(hmacGenerateTlsCsmJobIdx);


  /* Copy secret to the secret key */
  csmRetVal  = TCPIP_CSM_KeyElementCopy(SecretKeyIdPtr->KeyId, SecretKeyIdPtr->KeyElementId, keyIdPrfSecret, CRYPTO_KE_MAC_KEY);
  csmRetVal |= TCPIP_CSM_KeySetValid(keyIdPrfSecret);
  (void)TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_CALC_PSHA256);


  /* #20 Prepare A(0) with the seed */
  /* copy seed to the input key */
  {
    csmRetVal = TcpIp_TlsCoreCrypto_SetCsmKeyElementAndSetKeyValid(keyIdHmacGenerate, keyElementIdInput, SeedPtr, SeedLen);
    (void)TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_CALC_PSHA256);
  } /* End of A(0) */

  /* #30 First run to calculate A(1) */
  /* init, update and finalize inner hash */
  {
    /* Generate HMAC to KeyIdOutput */
    csmRetVal  = TCPIP_CSM_MacGenerate((uint32)hmacGenerateTlsCsmJobIdx, CRYPTO_OPERATIONMODE_SINGLECALL,
      &dummyBuffer, 0, &dummyBuffer, &dummyBufferLen);
    csmRetVal |= TCPIP_CSM_KeySetValid(keyIdHmacGenerate);

    /* Copy KeyIdOutput to keyIdActive */
    csmRetVal |= TCPIP_CSM_KeyElementCopy(keyIdHmacGenerate, keyElementIdOutput, keyIdHmacGenerate, keyElementIdActive);
    csmRetVal |= TCPIP_CSM_KeySetValid(keyIdHmacGenerate);
    (void)TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_CALC_PSHA256);
  } /* End of A(1) */

  /* #40 Iterate until the required quantity of data is produced (fillLevel reached required KeyLen) */
  while(fillLevel < ResultKeyIdPtr->KeyLen)
  {
    /* #50 Switch active and passive elements - required for CSM redirect feature */
    {
      /* Switch keyIds */
      uint32 tmpKeyId     = keyElementIdActive;
      keyElementIdActive  = keyElementIdPassive;
      keyElementIdPassive = tmpKeyId;
    }

    /* #60 Init, update and finalize outer hash */
    {
      uint32 hsTempKeyId          = TcpIp_GetHsTemporaryBufferKeyIdOfTlsHandshake(activeTlsHandshakeIdx);
      uint32 hsCommonKeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
      /* Using the redirection feature the HMAC has to be calculated within a single call, therefore the keyIdPassive
       * and the seed have to be copied to one input buffer.
       */
      csmRetVal  = TCPIP_CSM_KeyElementCopy(keyIdHmacGenerate, keyElementIdPassive, keyIdHmacGenerate,
                     keyElementIdInput);
      csmRetVal |= TCPIP_CSM_KeySetValid(keyIdHmacGenerate);
      /* Set the seed to the CSM. CSM does not support a partially set, therefore it has to be set to a temporary key first */
      csmRetVal |= TcpIp_TlsCoreCrypto_SetCsmKeyElementAndSetKeyValid(hsTempKeyId, hsCommonKeyElementId, SeedPtr, SeedLen);
      /* Copy seed to the input buffer after keyIdPassive */
      csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(hsTempKeyId, hsCommonKeyElementId, 0, TCPIP_TLS_HASH_SHA256_LEN, SeedLen,
                      keyIdHmacGenerate, keyElementIdInput );
      csmRetVal |= TCPIP_CSM_KeySetValid(keyIdHmacGenerate);
      dummyBufferLen = 0u; /* according to [*SWS_Csm_91014*] */
      /* Generate HMAC to KeyIdOutput using CSM redirection feature */
      csmRetVal |= TCPIP_CSM_MacGenerate((uint32)hmacGenerateTlsCsmJobIdx, CRYPTO_OPERATIONMODE_SINGLECALL,
        &dummyBuffer, 0, &dummyBuffer, &dummyBufferLen);
      csmRetVal |= TCPIP_CSM_KeySetValid(keyIdHmacGenerate);
    } /* End of outer hash */

    /* #70 Update the result pointer with the calculated values */
    /* write hash directly into ResultKeyIdPtr */
    {
      if (((uint32)fillLevel + TCPIP_TLS_HASH_SHA256_LEN) <= ResultKeyIdPtr->KeyLen)
      {
        /* Copy complete key */
        csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(keyIdHmacGenerate, keyElementIdOutput, 0, fillLevel,
          TCPIP_TLS_HASH_SHA256_LEN, ResultKeyIdPtr->KeyId, ResultKeyIdPtr->KeyElementId);
        csmRetVal |= TCPIP_CSM_KeySetValid(ResultKeyIdPtr->KeyId);
      }
      else
      {
        /* Copy only the last chunk */
        csmRetVal |= TCPIP_CSM_KeyElementCopyPartial(keyIdHmacGenerate, keyElementIdOutput, 0, fillLevel,
          (ResultKeyIdPtr->KeyLen - fillLevel), ResultKeyIdPtr->KeyId, ResultKeyIdPtr->KeyElementId);
        csmRetVal |= TCPIP_CSM_KeySetValid(ResultKeyIdPtr->KeyId);
        loopEnd = TRUE;
      }
      (void)TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_CALC_PSHA256);
    } /* End of result pointer update */

    /* #80 Check if we have to calculate another round A(i-1) */
    if (loopEnd == FALSE)
    {
      /* init, update and finalize inner hash for next run of the loop */
      /* Copy keyIdPassive to the input buffer  */
      csmRetVal  = TCPIP_CSM_KeyElementCopy(keyIdHmacGenerate, keyElementIdPassive, keyIdHmacGenerate, keyElementIdInput);
      csmRetVal |= TCPIP_CSM_KeySetValid(keyIdHmacGenerate);

      dummyBufferLen = 0u; /* according to [*SWS_Csm_91014*] */
      /* Generate HMAC to KeyIdOutput: A(i-1) using CSM redirection feature */
      csmRetVal |= TCPIP_CSM_MacGenerate((uint32)hmacGenerateTlsCsmJobIdx, CRYPTO_OPERATIONMODE_SINGLECALL,
        &dummyBuffer, 0, &dummyBuffer, &dummyBufferLen);
      csmRetVal |= TCPIP_CSM_KeySetValid(keyIdHmacGenerate);

      /* Copy result to active buffer */
      csmRetVal |= TCPIP_CSM_KeyElementCopy(keyIdHmacGenerate, keyElementIdOutput, keyIdHmacGenerate, keyElementIdActive);
      csmRetVal |= TCPIP_CSM_KeySetValid(keyIdHmacGenerate);
      (void)TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_CALC_PSHA256);
    }

    /* #90 Update the fill level by the consumed bytes */
    fillLevel += TCPIP_TLS_HASH_SHA256_LEN;

  } /* while() */

  return csmRetVal;
}

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_CalcKeyBlock
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
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_CalcKeyBlock(
  TcpIp_TlsConnectionIterType TlsConIdx )
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 seed[2u * TCPIP_TLS_RANDOM_LEN];
  TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx =
    TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx));
  TcpIp_TlsCsmKeyInfoType msKey;
  TcpIp_TlsCsmKeyInfoType keyBlockKey;
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /*
  RFC5246 6.3.  Key Calculation
  key_block = PRF( SecurityParameters.master_secret,
                   "key expansion",
                   SecurityParameters.server_random + SecurityParameters.client_random );
  */
  /* #10 Check if the necessary key for the CalKeyBlock operation is configured */
  if (TcpIp_IsCsmKeyIdMasterSecretUsedOfTlsHandshake(activeTlsHandshakeIdx) == TRUE)
  {
    msKey.KeyId = TcpIp_GetCsmKeyId(TcpIp_GetCsmKeyIdMasterSecretIdxOfTlsHandshake(activeTlsHandshakeIdx));
    msKey.KeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
    msKey.KeyLen = TCPIP_TLS_MASTER_SECRET_LEN;

    keyBlockKey.KeyId = TcpIp_GetPrfResultPrivateKeyIdOfTlsHandshake(activeTlsHandshakeIdx);
    keyBlockKey.KeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
    keyBlockKey.KeyLen = TcpIp_GetKeyBlockSizeOfTlsConnectionDyn(TlsConIdx);

    /* #20 Copy server and client random into one array */
#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
#   if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
    if (TcpIp_GetModeOfTlsConnection(TlsConIdx) == TCPIP_TLS_SERVER)
#   endif
    {
      /* Mode == TCPIP_TLS_SERVER */
      TcpIp_TlsServerBufferMgmtIterType tlsServerBufferMgmtIdx
        = TcpIp_GetTlsServerBufferMgmtIdxOfTlsConnection(TlsConIdx);

      VStdMemCpy((&seed[0]), TcpIp_GetAddrTlsBufferTx(                                                                  /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        TcpIp_GetTlsBufferTxServerRandomIdxOfTlsServerBufferMgmt(tlsServerBufferMgmtIdx)),
        TCPIP_TLS_RANDOM_LEN);
      VStdMemCpy((&seed[TCPIP_TLS_RANDOM_LEN]), TcpIp_GetAddrTlsBufferRx(                                               /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        TcpIp_GetTlsBufferRxClientRandomIdxOfTlsServerBufferMgmt(tlsServerBufferMgmtIdx)),
        TCPIP_TLS_RANDOM_LEN);
    }
#   if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
    else
#   endif
#  endif
#  if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
    {
      /* Mode == TCPIP_TLS_CLIENT */
      TcpIp_TlsClientBufferMgmtIterType tlsClientBufferMgmtIdx
        = TcpIp_GetTlsClientBufferMgmtIdxOfTlsConnection(TlsConIdx);

      VStdMemCpy((&seed[0]), TcpIp_GetAddrTlsBufferRx(                                                                  /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        TcpIp_GetTlsBufferRxServerRandomIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)),
        TCPIP_TLS_RANDOM_LEN);

      VStdMemCpy((&seed[TCPIP_TLS_RANDOM_LEN]), TcpIp_GetAddrTlsBufferTx(                                               /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        TcpIp_GetTlsBufferTxClientRandomIdxOfTlsClientBufferMgmt(tlsClientBufferMgmtIdx)),
        TCPIP_TLS_RANDOM_LEN);
    }
#  endif

    /* #30 Calculate key block content */
    retVal = TcpIp_Tls12CoreCrypto_Prf(TlsConIdx, &keyBlockKey, &msKey, (const uint8*)&TcpIp_Tls_Hs_KeyExpansionLabel[0],
      &seed[0], TCPIP_TLS_LABEL_KEY_EXPENSION_LEN, (2u * TCPIP_TLS_RANDOM_LEN), TRUE);

  }
  else
  {
    /* #40 CSM Key for CalKeyBlock operation is not configured - invalid configuration */
    TcpIp_TlsCoreUtil_HandleInvalidConfiguration(TlsConIdx);
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_GenerateIvRandomValue
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_GenerateIvRandomValue(
  TcpIp_TlsConnectionIterType TlsConIdx,
  TcpIp_SegmentPtrType        IvSeg,
  CONST(uint8, AUTOMATIC)     IvLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 tlsRandomJobId = TcpIp_GetTlsRandomGenerateCsmJobId();
  uint32 tmpIvLen = IvLen;
  Std_ReturnType csmRetVal;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* \trace TCASE-TCPIP_000727 */
  if (TcpIp_SegmentAssertMinLen(IvSeg, tmpIvLen) == E_OK)
  {
    /* #10 Generate a random value with the given length */
    csmRetVal = TCPIP_CSM_RandomGenerate(tlsRandomJobId, IvSeg->AvailPtr, &tmpIvLen);                                   /* VCA_TCPIP_TLS_CSM_RANDOM_GENERATE */

    TcpIp_SegmentCommit(IvSeg, tmpIvLen);

    /* #20 Check if the result of the CSM operation is E_OK */
    retVal = TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, tmpIvLen, IvLen, TCPIP_TLS_FCTID_CORE_CRYPTO_GENERATEIVRANDOM);

    if (retVal != E_OK)
    {
      /* #30 if not so, report the user error */
      TcpIp_TlsUserErrorType userError;

      userError.FunctionId = TCPIP_TLS_FCTID_CORE_CRYPTO_GENERATEIVRANDOM;
      userError.UserErrorId = TCPIP_TLS_ERRORID_CRYPTO_GENERATEIVRANDOM_FAILED;
      userError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

      TcpIp_TlsCoreError_ReportUserError(userError);
    }
  } /* else not necessary, Det is called in TcpIp_SegmentAssertMinLen already when E_NOT_OK is returned */

  return retVal;

} /* TcpIp_Tls12CoreCrypto_GenerateIvRandomValue() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_SetAesIvValue
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_SetIvValue(
  TcpIp_TlsConnectionIterType TlsConIdx,
  CONST(uint32, AUTOMATIC) IvKeyElementId,
  TCPIP_P2C(uint8) IvPtr,
  CONST(uint8, AUTOMATIC) IvLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType csmRetVal;
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set the given IV to the given CSM Key */
  csmRetVal  = TcpIp_TlsCoreCrypto_SetCsmKeyElementAndSetKeyValid(IvKeyElementId, CRYPTO_KE_CIPHER_IV, IvPtr, (uint32)IvLen);

  /* #20 Check if the result of the CSM operation is E_OK */
  retVal = TcpIp_TlsCoreCrypto_CheckCsmResult(TlsConIdx, csmRetVal, 0, 0, TCPIP_TLS_FCTID_CORE_CRYPTO_SETAESIVVALUE);

  if (retVal != E_OK)
  {
    /* #30 if not so, report the user error */
    TcpIp_TlsUserErrorType userError;

    userError.FunctionId = TCPIP_TLS_FCTID_CORE_CRYPTO_SETAESIVVALUE;
    userError.UserErrorId = TCPIP_TLS_ERRORID_CRYPTO_SETAESIVVALUE_FAILED;
    userError.TlsConId = TCPIP_TLS_CONNECTION_IDX_TO_ID(TlsConIdx);

    TcpIp_TlsCoreError_ReportUserError(userError);
  }

  return retVal;
} /* TcpIp_Tls12CoreCrypto_SetIvValue() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_DecryptFromConstSegmentSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_TcpIp_WrapperFunction_STPAR_6060 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_DecryptFromConstSegmentSafe(
  uint32                      JobId,
  Crypto_OperationModeType    Mode,
  TcpIp_ConstSegmentPtrType   DataSegPtr,
  uint32                      DataLength,
  uint32                      WriteIdx,
  TCPIP_P2V(uint32)           ResultLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#  if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Verify that data segment has enough space for the data length. */
  if (DataSegPtr->AvailLen < DataLength)
  {
    /* Memory runtime check fails - report DET */
    TcpIp_Tls_CallDetReportError(TCPIP_TLS_API_ID_TLS_DATA_PROCESSING, TCPIP_TLS_E_INV_RUNTIME_CHECK);
  }
  else
#  endif
  {
    /* #20 Decrypt the data with a range check that we will only write into the TLS Rx buffer. */
    retVal = TcpIp_Tls12CoreCrypto_DecryptSafe(JobId, Mode,
      DataSegPtr->AvailPtr, DataLength,
      WriteIdx, ResultLengthPtr);

    if (retVal == E_OK)
    {
      /* If decryption was successful commit the read length into the const segment. */
      TcpIp_ConstSegmentCommit(DataSegPtr, DataLength);
    }
  }

  return retVal;
} /* TcpIp_Tls12CoreCrypto_DecryptFromConstSegmentSafe() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCrypto_DecryptSafe
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_TcpIp_WrapperFunction_STPAR_6060 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCrypto_DecryptSafe(
  uint32                   JobId,
  Crypto_OperationModeType Mode,
  TCPIP_P2C(uint8)         DataPtr,
  uint32                   DataLength,
  uint32                   WriteIdx,
  TCPIP_P2V(uint32)        ResultLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#  if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
  /* #10 Verify that WriteIdx and ResultLengthPtr are in range of the Tls Rx buffer. */
  /* \trace TCASE-TCPIP_000728 */
  if ((WriteIdx + (uint64)(*ResultLengthPtr)) > TcpIp_GetSizeOfTlsBufferRx())
  {
    /* Memory runtime check fails - report DET */
    TcpIp_Tls_CallDetReportError(TCPIP_TLS_API_ID_TLS_DATA_PROCESSING, TCPIP_TLS_E_INV_RUNTIME_CHECK);
  }
  else
#  endif
  {
    TCPIP_P2V(uint8) resultPtr = TcpIp_GetAddrTlsBufferRx((TcpIp_TlsBufferRxIterType)WriteIdx);

    /* #20 Decrypt the data in the provided buffer using Aes128Cbc. */
    retVal = TCPIP_CSM_Decrypt(JobId, Mode, DataPtr, DataLength,                                                        /* VCA_TCPIP_TLS_CSM_DECRYPT_CSL01 */
      resultPtr, ResultLengthPtr);
  }

  return retVal;
} /* TcpIp_Tls12CoreCrypto_DecryptSafe() */

/**********************************************************************************************************************
 *  TLS CORE ALERT
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  TLS CORE LOWER LAYER (LL)
 *********************************************************************************************************************/
#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12CoreLl_TcpAccepted
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreLl_TcpAccepted(
  TcpIp_SocketTcpDynIterType      SocketTcpIdx,
  TcpIp_SocketTcpDynIterType      ConnectedSocketTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsListenerIterType tlsListenerIdx;
  TcpIp_SizeOfSocketDynType socketIdx = TCPIP_TCP_IDX_TO_SOCKET_IDX(SocketTcpIdx);
  TcpIp_TlsConnectionIterType tlsConIdx;

  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_VENTER_CRITICAL_SECTION();

  /* #10 Get the TLS-Listener corresponding to the SocketIdx */
  retVal = TcpIp_TlsServerListener_GetTlsListenerUsedBySocketIdx(socketIdx, &tlsListenerIdx);
  if (retVal == E_OK)
  {
    TcpIp_PortOfTlsListenerType usedPort = TcpIp_GetPortOfTlsListener(tlsListenerIdx);

    /* Reset the return value */
    retVal = E_NOT_OK;

    /* Get the index of the TLS connection which shall be used for connection assignment. */
    tlsConIdx = TcpIp_TlsServerListener_GetTlsConnectionIndexForConnectionAssignment(socketIdx, usedPort);

    /* #20 If there is a connection assigned for this socket (dynamic port assigned) or a free TLS connection with either the pre-configured port or a wildcard port was found, prepare parameters. */
    if (tlsConIdx < TcpIp_GetSizeOfTlsConnection())
    {
      TcpIp_SocketOwnerConfigIdxOfSocketDynType socketOwnerCfgIdx
        = TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(socketIdx);

      TcpIp_SizeOfSocketDynType comSocketIdx = TCPIP_TCP_IDX_TO_SOCKET_IDX(ConnectedSocketTcpIdx);

      /* Set the Listen and Communication Socket inside the found TLS-Connection */
      TcpIp_SetSocketDynListenIdxOfTlsConnectionDyn(tlsConIdx, socketIdx);
      TcpIp_SetSocketDynComIdxOfTlsConnectionDyn(tlsConIdx, comSocketIdx);
      TcpIp_SetTlsConnectionIdxOfSocketTcpDyn(ConnectedSocketTcpIdx, (TcpIp_SizeOfTlsConnectionDynType)tlsConIdx);
      /* Set the socket owner index of this connection */
      TcpIp_SetSocketOwnerConfigIdxOfTlsConnectionDyn(tlsConIdx, socketOwnerCfgIdx);
      /* Set the Connection State for this connection */
      TcpIp_SetConnectionStateOfTlsConnectionDyn(tlsConIdx, TCPIP_TLS_STATE_CONNECTION_HS_ACTIVE);

      retVal = E_OK;
    }
  }
  TCPIP_VLEAVE_CRITICAL_SECTION();

  /* ----- Development Error Report ------------------------------------- */
#   if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  /* #30 Report an error if necessary. */
  if (retVal != E_OK)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_TLS_VINSTANCE_ID, TCPIP_TLS_API_ID_TLS_ACCEPTED, TCPIP_TLS_E_NO_LISTENER);
  }
#   endif /* TCPIP_DEV_ERROR_REPORT */
  return retVal;
} /* TcpIp_Tls12CoreLl_TcpAccepted() */
#  endif

/**********************************************************************************************************************
 *  TLS CORE UTIL  (HELPER FUNCTIONS)
 *********************************************************************************************************************/
#  if (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12CoreUtil_DoesCertificateMatchSigAlgoAndCurve
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_Tls12CoreUtil_DoesCertificateMatchSigAlgoAndCurve(
  TcpIp_CertificateIdType         CertId,
  TcpIp_SignatureAlgorithmIdType  SignAlgId,
  TcpIp_TlsEllipticCurveIdType    CurveId)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SignatureAlgorithmIdType sigAlgId = TCPIP_SIG_ALGO_ID_NONE;
  TcpIp_TlsEllipticCurveIdType curveId = TCPIP_CURVE_ID_NO_CURVE;
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the signature algorithm and curve which is used by the certificate */
  /* TcpIp_TlsCoreUtil_GetSigAlgAndCurveFromCertificate() won't ever return E_OK
   * and sigAlgId = TCPIP_SIG_ALGO_ID_NONE or curveId = TCPIP_CURVE_ID_NO_CURVE.
   * Therefore this function always returns FALSE
   * when SignAlgId = TCPIP_SIG_ALGO_ID_NONE or CurveId = TCPIP_CURVE_ID_NO_CURVE is passed. */
  if (TcpIp_Tls12CoreUtil_GetSigAlgAndCurveFromCertificate(CertId, &sigAlgId, &curveId) == E_OK)
  {
    /* #20 Check if the certificate's signature algorithm and curve match the passed parameters. */
    if (   (curveId == CurveId)
        && (sigAlgId == SignAlgId))
    {
      retVal = TRUE;
    }
  }
  return retVal;
} /* TcpIp_Tls12CoreUtil_DoesCertificateMatchSigAlgoAndCurve */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreUtil_GetSigAlgAndCurveFromCertificate
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreUtil_GetSigAlgAndCurveFromCertificate(
  TcpIp_CertificateIdType                   CertId,
  TCPIP_P2V(TcpIp_SignatureAlgorithmIdType) SignAlgIdPtr,
  TCPIP_P2V(TcpIp_TlsEllipticCurveIdType)   CurveIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  KeyM_CertificateStatusType certStatus = KEYM_CERTIFICATE_NOT_AVAILABLE;
  uint16 issuerCertId = 0u;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the certificate is available. */
  if ((KeyM_CertGetStatus(CertId, &certStatus) == E_OK) && (certStatus != KEYM_CERTIFICATE_NOT_AVAILABLE))
  {
    /* Cert is available */

    /* #20 Read the signature algorithm id from the cert. */
    if (TcpIp_TlsCoreUtil_ReadCertSignatureAlgorithm(CertId, SignAlgIdPtr) == E_OK)
    {
      /* The information about the curve used for the signature can only be found in the issuing certificate. */
      /* #30 Go to the issuing certificate to read the curve that was used to sign the certificate. */
      if (KeyM_GetIssuerCertId(CertId, &issuerCertId) == E_OK)
      {
        /* The issuing certificate could be identified. */
        if (TcpIp_TlsCoreUtil_GetCurveFromCertificate(issuerCertId, CurveIdPtr) == E_OK)
        {
          /* #40 Check if the read curve fits to the read signature algorithm. */
          retVal = TcpIp_Tls12CoreUtil_CheckIfSigAlgFitsToCurve(*SignAlgIdPtr, *CurveIdPtr);
        }
      } /* else: unable to read curve from issuing certificate */
    } /* else: unable to read SignatureAlgorithm */
  }  /* else: cert is not available / can not be read */

  if (retVal != E_OK)
  {
    /* If an error occurred, ensure that the output pointer values are set to an invalid value. */
    *SignAlgIdPtr = TCPIP_SIG_ALGO_ID_NONE;
    *CurveIdPtr = TCPIP_CURVE_ID_NO_CURVE;
  }

  return retVal;
} /* TcpIp_Tls12CoreUtil_GetSigAlgAndCurveFromCertificate */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreUtil_CheckIfSigAlgFitsToCurve
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreUtil_CheckIfSigAlgFitsToCurve(
  TcpIp_SignatureAlgorithmIdType SignAlgId,
  TcpIp_TlsEllipticCurveIdType   CurveId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the passed signature algorithm fits to the passed curve. */
  switch (SignAlgId)
  {
    case TCPIP_SIG_ALGO_ID_ECDSA_ED25519_INTRINSIC:
    {
      if (CurveId == TCPIP_CURVE_ID_X25519)
      {
        retVal = E_OK;
      } /* else - curve does not fit to signature algorithm */
      break;
    } /* case TCPIP_SIG_ALGO_ID_ECDSA_ED25519_INTRINSIC */
    case TCPIP_SIG_ALGO_ID_ECDSA_SECP256R1_SHA256:
    {
      if (CurveId == TCPIP_CURVE_ID_SECP256R1)
      {
        retVal = E_OK;
      } /* else - curve does not fit to signature algorithm */
      break;
    } /* case TCPIP_SIG_ALGO_ID_ECDSA_SECP256R1_SHA256 */
    case TCPIP_SIG_ALGO_ID_ECDSA_SECP521R1_SHA512:
    {
      if (CurveId == TCPIP_CURVE_ID_SECP521R1)
      {
        retVal = E_OK;
      } /* else - curve does not fit to signature algorithm */
      break;
    } /* case TCPIP_SIG_ALGO_ID_ECDSA_SECP521R1_SHA512 */
    default:
    {
      /* Unknown signature algorithm, retVal already set to E_NOT_OK, left blank intentionally */
      break;
    }
  } /* switch */
  return retVal;
} /* TcpIp_Tls12CoreUtil_CheckIfSigAlgFitsToCurve */
#  endif /* (TCPIP_SUPPORT_ASRTLS_ECC == STD_ON) */

/**********************************************************************************************************************
 *  MSA - Master Secret Access
 *********************************************************************************************************************/

#  if ((TCPIP_SUPPORT_ASRTLS_MASTER_SECRET_ACCESS == STD_ON) || (TCPIP_SUPPORT_ASRTLS_SECRET_ACCESS == STD_ON))
/**********************************************************************************************************************
 *  TcpIp_Tls12Core_ClearMasterSecretBuffer()
**********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_Tls12Core_ClearMasterSecretBuffer(
  TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear Session ID and Client Random */

  VStdLib_MemClr(TcpIp_GetAddrTlsClientRandomAccessBuffer(                                                            /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
    TcpIp_GetTlsClientRandomAccessBufferStartIdxOfTlsConnection(TlsConIdx)), TCPIP_TLS_RANDOM_LEN);

  VStdLib_MemClr(TcpIp_GetAddrTlsClientSessionIdAccessBuffer(                                                         /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
    TcpIp_GetTlsClientSessionIdAccessBufferStartIdxOfTlsConnection(TlsConIdx)), TcpIp_GetClientSessionIdAccessBufferLenOfTlsConnectionDyn(TlsConIdx));

  TcpIp_SetClientSessionIdAccessBufferLenOfTlsConnectionDyn(TlsConIdx, 0u);

} /* TcpIp_Tls12Core_ClearMasterSecretBuffer() */

/**********************************************************************************************************************
 *  TcpIp_Tls12Core_ReadHandshakeParameter
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
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12Core_ReadHandshakeParameter(
  TcpIp_SocketDynIterType SocketIdx,
  TCPIP_P2V(uint8)        MasterSecretPtr,
  TCPIP_P2V(uint8)        ClientRandomPtr,
  TCPIP_P2V(uint8)        SessionIdPtr,
  TCPIP_P2V(uint8)        SessionIdLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_TlsConnectionIterType tlsConIdx;
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if there is a valid TLS connection for this socket. */
  if (TcpIp_SocketIdxToTlsConIdx(SocketIdx, &tlsConIdx) == E_OK)
  {
    /* #20 Read the Master Secret from CSM */
    {
      TcpIp_TlsHandshakeIterType activeTlsHandshakeIdx =
        TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(tlsConIdx));
      uint32 masterSecretKeyId        = TcpIp_GetCsmKeyId(TcpIp_GetCsmKeyIdMasterSecretIdxOfTlsHandshake(activeTlsHandshakeIdx));
      uint32 masterSecretKeyElementId = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);
      uint32 masterSecretLen          = TCPIP_TLS_MASTER_SECRET_LEN;
      Std_ReturnType csmRetVal;

      /* Get the Master Secret Key from the CSM */
      csmRetVal = TCPIP_CSM_KeyElementGet(masterSecretKeyId, masterSecretKeyElementId, MasterSecretPtr, &masterSecretLen);
      /* Check if the length of the read key match */
      retVal = TcpIp_TlsCoreCrypto_CheckCsmResult(tlsConIdx, csmRetVal, masterSecretLen, TCPIP_TLS_MASTER_SECRET_LEN,
                  TCPIP_TLS_FCTID_CORE_SA_READSECRET);

    }
    /* #30 Copy Client Random to user buffer, if buffer is valid. */
    if (ClientRandomPtr != NULL_PTR)
    {
      VStdMemCpy(&ClientRandomPtr[0],                                                                                   /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
        TcpIp_GetAddrTlsClientRandomAccessBuffer(TcpIp_GetTlsClientRandomAccessBufferStartIdxOfTlsConnection(tlsConIdx)),
        TCPIP_TLS_RANDOM_LEN);
    }
    /* #40 Copy received Session Id and Session Id length to user buffer. */
    if ((SessionIdPtr != NULL_PTR) && (SessionIdLenPtr != NULL_PTR))
    {
      uint8 sessionIdAccessBufferLen = TcpIp_GetClientSessionIdAccessBufferLenOfTlsConnectionDyn(tlsConIdx);

      /* Check if the provided buffer is sufficient to store the session id */
      if (*SessionIdLenPtr >= sessionIdAccessBufferLen)
      {
        VStdMemCpy(&SessionIdPtr[0],                                                                                    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
          TcpIp_GetAddrTlsClientSessionIdAccessBuffer(TcpIp_GetTlsClientSessionIdAccessBufferStartIdxOfTlsConnection(tlsConIdx)),
          sessionIdAccessBufferLen);
        /* Update the actual length of the received Session Id. */
        *SessionIdLenPtr = sessionIdAccessBufferLen;
      }
      else
      {
        retVal = E_NOT_OK;
      }
    }
  }
  else
  {
    /* #50 otherwise, no valid TLS connection could be found. */
    retVal = E_NOT_OK;
  }

  return retVal;
} /* TcpIp_Tls12Core_ReadHandshakeParameter() */
#  endif /* ((TCPIP_SUPPORT_ASRTLS_MASTER_SECRET_ACCESS == STD_ON) || (TCPIP_SUPPORT_ASRTLS_SECRET_ACCESS == STD_ON)) */

#  if (TCPIP_SUPPORT_ASRTLS_SECRETS_NSS_FRAME == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_Tls12CoreMainFunction_TxNss
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Tls12CoreMainFunction_TxNss(TcpIp_TlsConnectionIterType TlsConIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* In TcpIp_TlsCoreMainFunction_TxNss() it was checked
   * that the active cipherworker and the active handshake indices are valid. */
  TcpIp_TlsCipherWorkerActiveIdxOfTlsConnectionDynType activeTlsCipherWorkerIdx =
    TcpIp_GetTlsCipherWorkerActiveIdxOfTlsConnectionDyn(TlsConIdx);

  TcpIp_TlsHandshakeActiveIdxOfTlsCipherWorkerDynType activeTlsHandshakeIdx
    = TcpIp_GetTlsHandshakeActiveIdxOfTlsCipherWorkerDyn(activeTlsCipherWorkerIdx);

  /* ----- Implementation ----------------------------------------------- */
  if (TcpIp_IsNssMasterSecretReadyToSendOfTlsConnectionDyn(TlsConIdx) == TRUE)
  {
    TcpIp_CsmKeyIdType masterSecretKeyId
      = TcpIp_GetCsmKeyId(TcpIp_GetCsmKeyIdMasterSecretIdxOfTlsHandshake(activeTlsHandshakeIdx));
    TcpIp_HsCommonKeyElementIdOfTlsHandshakeType masterSecretKeyElementId
      = TcpIp_GetHsCommonKeyElementIdOfTlsHandshake(activeTlsHandshakeIdx);

    /* #10 Trigger sending NSS Key Log Frame with MasterSecret */
    if (TcpIp_Tls12CoreCtx_SendMasterSecretViaNssFrame(
      TlsConIdx, masterSecretKeyId, masterSecretKeyElementId) == E_OK)
    {
      TcpIp_SetNssMasterSecretReadyToSendOfTlsConnectionDyn(TlsConIdx, FALSE);
    }
  }
} /* TcpIp_Tls12CoreMainFunction_TxNss() */

/**********************************************************************************************************************
 *  TcpIp_Tls12CoreCtx_SendMasterSecretViaNssFrame
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tls12CoreCtx_SendMasterSecretViaNssFrame(
  TcpIp_TlsConnectionIterType                   TlsConIdx,
  TcpIp_CsmKeyIdType                            MasterSecretCsmKeyId,
  TcpIp_HsCommonKeyElementIdOfTlsHandshakeType  MasterSecretCsmKeyElementId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  const uint8 TcpIp_Tls13_NssFrameSecretLabel_Mas[] = "CLIENT_RANDOM";
  const uint8 TcpIp_Tls13_NssFrameSecretLabel_Mas_Len = 13u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Send the master secret via NSS frame. */
  retVal = TcpIp_TlsCore_SendSecretViaNssFrame(
    TlsConIdx,
    MasterSecretCsmKeyId,
    MasterSecretCsmKeyElementId,
    &TcpIp_Tls13_NssFrameSecretLabel_Mas[0],
    TcpIp_Tls13_NssFrameSecretLabel_Mas_Len);

  return retVal;
}
#  endif /* (TCPIP_SUPPORT_ASRTLS_SECRETS_NSS_FRAME == STD_ON) */

#  define TCPIP_STOP_SEC_CODE
#  include "TcpIp_MemMap.h"                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON) */
#endif /* (TCPIP_SUPPORT_ASRTLS == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Tls12Core.c
 *********************************************************************************************************************/
