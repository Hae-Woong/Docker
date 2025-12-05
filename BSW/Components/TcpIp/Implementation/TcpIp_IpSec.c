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
 *  -------------------------------------------------------------------------------------------------------------------
 *        \file  TcpIp_IpSec.c
 *        \brief  Implementation of Internet Protocol Security Features
 *
 *        \details  This file is part of the TcpIp submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp submodule. >> TcpIp.h
 *********************************************************************************************************************/

#define TCPIP_IPSEC_SOURCE

#include "TcpIp_Lcfg.h"
#if (TCPIP_SUPPORT_IPSEC == STD_ON)
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "TcpIp_IpV4_Priv.h"

# include "TcpIp_IpSec.h"
# include "Csm.h"
# include "TcpIp_Priv.h"

# include "IpBase.h"
# include "TcpIp.h"

# if !defined (STATIC)                                                                                                  /* COV_TCPIP_COMPATIBILITY */
#  define STATIC static
# endif


/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

# define TCPIP_START_SEC_VAR_NO_INIT_32
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

STATIC VAR(uint32, TCPIP_VAR_NO_INIT) TcpIp_IpSecSaEntryCnt;                                                            /* PRQA S 3218 */ /* MD_TCPIP_Rule8.9_3218 */

# define TCPIP_STOP_SEC_VAR_NO_INIT_32
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define TCPIP_START_SEC_CODE
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VUpdateWndParams
 *********************************************************************************************************************/
/*! \brief          Updates the Anti-replay window parameters
 *  \details        This function is invoked only by the TcpIp_IpSec_AntiReplayWindowUpdate
 *  \param[in]      SaEntryIdx       SA Entry Idx
 *                                   \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \pre            The Ip packet recived has been authenticated successfully
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VUpdateWndParams(
  TcpIp_SaEntryIterType       SaEntryIdx);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VSetWndBit
 *********************************************************************************************************************/
/*! \brief          Sets the bit in the anti-replay window bitstream
 *  \details        This function is invoked only by the TcpIp_IpSec_AntiReplayWindowUpdate
 *  \param[in]      SaEntryIdx       SA Entry Idx
 *                                   \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in]      SetBit           Bit to be set in the Window bit stream
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VSetWndBit(
  TcpIp_SaEntryIterType         SaEntryIdx,
  uint32                        SetBit);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VReplayCheck
 *********************************************************************************************************************/
/*! \brief          Checks if the bit is already set in the Replay check bitstream
 *  \details        This function is invoked only by the TcpIp_IpSec_AntiReplayWindowUpdate
 *  \param[in]      SaEntryIdx       SA Entry Idx
 *                                   \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in]      ChkBit           Bit to be checked in the Window bit stream
 *  \return         E_OK             Packet being processed is not replayed
 *  \return         E_NOT_OK         Packet being processed has been replayed
 *  \pre            The sequence number of the received packet is stored in the SA Entry
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VReplayCheck(
  TcpIp_SaEntryIterType         SaEntryIdx,
  uint32                        ChkBit);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VIsIpSecHdrSupported
 *********************************************************************************************************************/
/*! \brief          Checks if the given IpSec protocol is supported by the SA entry
 *  \details        This function is invoked during rx packet processing
 *  \param[in]      SaEntryIdx       SA Entry Idx
 *                                   \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in]      IpSecProtocol    IpSec protocol header
 *  \return         E_OK             The IpSec header is supported by this SA entry
 *  \return         E_NOT_OK         The IpSec header is not supported by this SA entry
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VIsIpSecHdrSupported(
  TcpIp_SaEntryIterType SaEntryIdx,
  uint8                 IpSecProtocol);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VInitEsnSupport
 *********************************************************************************************************************/
/*! \brief          Initialize the Esn data for the Sa entry
 *  \details        -
 *  \param[in]      IpSaEntryIdx     SA Entry Idx
 *                                   \spec requires IpSaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in]      EsnEnabled       Esn enabled flag
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE)  TcpIp_IpSec_VInitEsnSupport(
  TcpIp_SaEntryIterType IpSaEntryIdx,
  boolean               EsnEnabled);

/**********************************************************************************************************************
 * TcpIp_IpSecVIntegrityIntegTransformLookup()
 *********************************************************************************************************************/
/*! \brief          Lookup for the requested integrity transform in the configured IpSecConfigSet
 *  \details        This function checks if the requested transform is present in the IpSecConfigSet
 *                  and returns an unused job pair for this transform
 *  \param[in]      LocalAddrIdx         Local Ip Address Idx
 *  \param[in]      IntegTransformIdent  Identifier of the integrity transform
 *  \param[out]     JobPairIdxPtr        Pointer to store the selected job pair index
 *                                       CONSTRAINT: MUST NOT be null
 *  \return         E_OK                 Valid job pair index was obtained
 *  \return         E_NOT_OK             No valid job pair was obtained
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecVIntegrityIntegTransformLookup(
  TcpIp_LocalAddrIterType                     LocalAddrIdx,
  TcpIp_IpSecIntegrityTransformType           IntegTransformIdent,
  TCPIP_P2V(TcpIp_IntegrityJobPairIterType)   JobPairIdxPtr);

/**********************************************************************************************************************
 * TcpIp_IpSecVAllocateJobPair()
 *********************************************************************************************************************/
/*! \brief          Allocates an unused job pair
 *  \details        This function traverses through the list of job pairs of a transform
 *                  and returns the first unused index
 *  \param[in]      IntegrityAlgorithmIdx    Index of the integrity algorithm
 *                                           \spec requires IntegrityAlgorithmIdx < TcpIp_GetSizeOfIntegrityAlgorithm(); \endspec
 *  \param[out]     JobPairIdxPtr            Pointer to store the selected job pair index
 *                                           CONSTRAINT: MUST NOT be null
 *  \return         E_OK                     Job pair was allocated
 *  \return         E_NOT_OK                 Job pair was NOT allocated
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecVAllocateJobPair(
  TcpIp_IntegrityAlgorithmIterType               IntegrityAlgorithmIdx,
  TCPIP_P2V(TcpIp_IntegrityJobPairIterType)      JobPairIdxPtr);

/**********************************************************************************************************************
 * TcpIp_IpSec_VAddIpV4SaEntry
 *********************************************************************************************************************/
/*! \brief          Updates fields of an IpV4 Sa entry
 *  \details        -
 *  \param[in]      Dir                 Direction of the SA.
                                        TCPIP_IPSEC_TRAFFIC_INBOUND or TCPIP_IPSEC_TRAFFIC_OUTBOUND
 *  \param[in]      SaEntryIdx          Entry index of the IpV4 SA entry to be updated
 *                                      \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in]      IntegrityJobPairIdx Integrity job pair idx
 *  \param[in]      SaInfoPtr           Struct of IpV4 SA entry information
 *                                      CONSTRAINT: KeyMaterial elements MUST NOT be null
 *  \param[in]      IpV4CtrlIdx         Index of the IPv4 controller
 *                                      \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \return         E_OK                SA entry was created successfully
 *  \return         E_NOT_OK            SA entry was not created
 *  \pre            This function should only be called for adding IpV4 Sa entry
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_IpSec_VAddIpV4SaEntry(
  uint8                             Dir,
  TcpIp_SaEntryIterType             SaEntryIdx,
  TcpIp_IntegrityJobPairIterType    IntegrityJobPairIdx,
  TCPIP_P2C(TcpIp_IpSecSaInfoType)  SaInfoPtr,
  TcpIp_IpV4CtrlIterType            IpV4CtrlIdx);


/**********************************************************************************************************************
 * TcpIp_IpSec_VAddSaEntry
 *********************************************************************************************************************/
/*! \brief          Adds an SA Entry to the SAD.
 *  \details        -
 *  \param[in]      Dir                     Direction of the SA.
                                            TCPIP_IPSEC_TRAFFIC_INBOUND or TCPIP_IPSEC_TRAFFIC_OUTBOUND
 *  \param[in]      LocalAddrIdx            Local Ip Address Idx
 *  \param[in]      JobPairIdx              Integrity job pair idx
 *  \param[in]      SaInfoPtr               Struct of SA entry information
 *                                          CONSTRAINT: MUST NOT be null
 *                                          CONSTRAINT: KeyMaterial elements MUST NOT be null
 *  \param[out]     SaEntryIdxSelectedPtr   Entry index of the updated SA entry
 *                                          TcpIp_GetSizeOfSaEntry() when E_NOT_OK returned
 *  \return         E_OK                    SA entry was created successfully
 *  \return         E_NOT_OK                SA entry was not created
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VAddSaEntry(
  uint8                              Dir,
  TcpIp_LocalAddrIterType            LocalAddrIdx,
  TcpIp_IntegrityJobPairIterType     JobPairIdx,
  TCPIP_P2C(TcpIp_IpSecSaInfoType)   SaInfoPtr,
  TCPIP_P2V(TcpIp_SaEntryIterType)   SaEntryIdxSelectedPtr);

/**********************************************************************************************************************
 * TcpIp_IpSec_VFindUnusedSaEntry
 *********************************************************************************************************************/
/*! \brief          Finds unused Sa entry in the SAD
 *  \details        -
 *  \param[in]      Dir                         Direction of the SA.
                                                TCPIP_IPSEC_TRAFFIC_INBOUND or TCPIP_IPSEC_TRAFFIC_OUTBOUND
 *  \param[in]      IpV4CtrlIdx                 Ip controller index
 *                                              \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *                                              CONSTRAINT: TcpIp_IsIpSecConfigSetUsedOfIpV4Ctrl(IpV4CtrlIdx) is valid
 *  \param[out]     IpSaEntryIdxPtr             Unused Sa entry index
 *                                              TcpIp_GetSizeOfSaEntry      If no unused entry found
 *                                              CONSTRAINT: MUST NOT be null
 *  \param[out]     IpsecHdrType                Type of the IPsec header
 *                                              TcpIp_GetSizeOfSaEntry      If no unused entry found
 *  \return         E_OK                        Unused SA entry found
 *  \return         E_NOT_OK                    Unused SA entry was not found
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_IpSec_VFindUnusedSaEntry(
  TcpIp_IpV4CtrlIterType            IpV4CtrlIdx,
  TCPIP_P2V(TcpIp_SaEntryIterType)  IpSaEntryIdxPtr,
  TcpIp_IpSecHeaderType             IpsecHdrType);

/**********************************************************************************************************************
 * TcpIp_IpSec_VRemoveSaEntry
 *********************************************************************************************************************/
/*! \brief          Remove an SA from the SAD.
 *  \details        -
 *  \param[in]      SaEntryIdx       SA Entry index to be removed
 *                                  \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE)  TcpIp_IpSec_VRemoveSaEntry(
  TcpIp_SaEntryIterType     SaEntryIdx);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VInitVectorGenerate
 *********************************************************************************************************************/
/*! \brief          Generate or update the Initialization Vector for the SA
 *  \details        The generated value should be unique for the SA. The value shall
 *                  be stored into the SA entry data struct
 *  \param[in]      SaEntryIdx         SA Entry index
 *                                     \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VInitVectorGenerate(
  TcpIp_SaEntryIterType       SaEntryIdx);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VLookupSecurityPolicy
 *********************************************************************************************************************/
/*! \brief          Returns the configured IP security policy for the source/destination pair
 *  \details        If IpSecConfigSet is configured for the controller, looks up the configured
 *                  IP security policy for the given combination of source and destination
 *  \param[in]      IpV4CtrlIdx                 Ip controller index
 *                                              \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \param[in]      IpProtocol                  The protocol header being looked up
 *  \param[in]      RemSock                     Destination address and port
 *  \param[in]      LocSock                     Local address and port
 *  \param[out]     IpSpdEntryIdxSelected       Index of the matched Spd entry
 *                                              TcpIp_GetSizeOfSpdEntry() if no match found
 *                                              CONSTRAINT: MUST NOT be NULL_PTR
 *  \return         TcpIp_IpSecPolicyType       Security policy for the traffic selectors
 *  \pre            Only called if IpSec config exists for the controller
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(TcpIp_IpSecPolicyType, TCPIP_CODE) TcpIp_IpSec_VLookupSecurityPolicy(
  TcpIp_IpV4CtrlIterType                IpV4CtrlIdx,
  uint8                                 IpProtocol,
  IpBase_SockAddrInType                 RemSock,
  IpBase_SockAddrInType                 LocSock,
  TCPIP_P2V(TcpIp_SpdEntryIterType)     IpSpdEntryIdxSelected);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VGetIpSecHeaderLengthBytes
 *********************************************************************************************************************/
/*! \brief          Returns the IpSec header length in bytes
 *  \details        TcpIp_IpSec_VGetIpSecHeaderLengthBytes() returns the IpSec header length if IpSec Protect policy
 *                  exists for the Destination
 *  \param[in]      SaEntryIdx      SA Entry Index
 *  \return         LenBytes        Length of the authentication header in bytes
 *                  0               if no IpSec policy exists
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(uint16, TCPIP_CODE) TcpIp_IpSec_VGetIpSecHeaderLengthBytes(
  TcpIp_SaEntryIterType   SaEntryIdx);

/**********************************************************************************************************************
 * TcpIp_IpSec_UpdateCsmKeyElement()
 *********************************************************************************************************************/
/*! \brief          Update the Csm key elements and set to Valid
 *  \details        -
 *  \param[in]      SaEntryIdx        SA Entry index
 *                                    \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in]      KeyId             Key element idx to be updated
 *  \param[in]      KeyMatPtr         Key material for job
 *                                    CONSTRAINT: MUST NOT be null
 *  \param[in]      KeyMatLen         Key material length for job
 *  \return         E_OK              Key elements updated and set to Valid
 *  \return         E_NOT_OK          Key elements could not be updated and set to valid
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_UpdateCsmKeyElement(
  TcpIp_SaEntryIterType                  SaEntryIdx,
  uint32                                 KeyId,
  TCPIP_P2C(uint8)                       KeyMatPtr,
  uint16                                 KeyMatLen);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VUpdateTxDescriptorForAuthentication
 *********************************************************************************************************************/
/*! \brief          Updates the Tx descriptors with Sa entry parameters
 *  \details        -
 *  \param[in]      IpSecPolicy            Security policy for the traffic
 *  \param[in]      SaEntryIdxSelected     Sa entry index selected for IpSec
 *  \param[out]     IpTxReqDescrPtr        Pointer to Tx description
 *                                         CONSTRAINT: MUST NOT be NULL_PTR
 *  \param[out]     EventIdPtr             Event Id
 *                                         CONSTRAINT: MUST NOT be NULL_PTR
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VUpdateTxDescriptorForAuthentication(
  TcpIp_IpSecPolicyType                       IpSecPolicy,
  TcpIp_SaEntryIterType                       SaEntryIdxSelected,
  TCPIP_P2V(TcpIp_IpTxRequestDescriptorType)  IpTxReqDescrPtr,
  TCPIP_P2V(TcpIp_IpSecEventType)             EventIdPtr);

/* Pair of functions for Tx/Rx selector search in SA entry table */
/**********************************************************************************************************************
 *  TcpIp_IpSec_VLookupSelectorsInTxSaEntry
 *********************************************************************************************************************/
/*! \brief          Returns the SA entry index matching the selectors of the Tx packet
 *  \details        -
 *  \param[in]      IpV4CtrlIdx            Index of the affected IPv4 controller.
 *                                         \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *                                         CONSTRAINT: TcpIp_IsIpSecConfigSetUsedOfIpV4Ctrl(IpV4CtrlIdx) is valid
 *  \param[in]      IpsecHdrType           IpSec header type enabled for the traffic
 *  \param[in]      XptProtocol            Transport/Upper layer protocol
 *  \param[in]      RemSock                Remote address and port
 *  \param[in]      LocSock                Local address and port
 *  \return         SaEntryIdx             SA entry index of the matching entry
 *                                         TcpIp_GetSizeOfSaEntry() if no matching entry found
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(TcpIp_SaEntryIterType, TCPIP_CODE) TcpIp_IpSec_VLookupSelectorsInTxSaEntry(
  TcpIp_IpV4CtrlIterType   IpV4CtrlIdx,
  TcpIp_IpSecHeaderType    IpsecHdrType,
  TcpIp_IpProtocolType     XptProtocol,
  IpBase_SockAddrInType    RemSock,
  IpBase_SockAddrInType    LocSock);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VLookupSelectorsInRxSaEntry
 *********************************************************************************************************************/
/*! \brief          Returns the SA entry index matching the selectors of the Rx packet
 *  \details        Searches for a matching SPI and IpSec header type in the SA entries
 *                  for received unicast packets
 *  \param[in]      IpV4CtrlIdx            Index of the affected IPv4 controller.
 *                                         \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *                                         CONSTRAINT: TcpIp_IsIpSecConfigSetUsedOfIpV4Ctrl(IpV4CtrlIdx) is valid
 *  \param[in]      IpSecProt              IpSec protocol header (in the received packet)
 *  \param[in]      Spi                    SPI (in the received packet)
 *  \return         SaEntryIdx             SA entry index of the matching entry
 *                                         TcpIp_GetSizeOfSaEntry() if no matching entry found
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(TcpIp_SaEntryIterType, TCPIP_CODE) TcpIp_IpSec_VLookupSelectorsInRxSaEntry(
  TcpIp_IpV4CtrlIterType               IpV4CtrlIdx,
  uint8                                IpSecProt,
  TcpIp_SpiOfSaEntryType               Spi);

/* Pair of functions for Tx/Rx antireplay window processing */
/**********************************************************************************************************************
 *  TcpIp_IpSec_VAntiReplayWindowVerify
 *********************************************************************************************************************/
/*! \brief          Verifies anti replay window check for the received sequence number
 *  \details        This function expects the SaEntry to contain the latest received sequence number
 *                  stored
 *  \param[in]      SaEntryIdx       Security Association Entry Idx
 *                                   \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \return         E_OK             Anti Replay window check successful, window needs to be updated
 *  \return         E_NOT_OK         Anti Replay window check NOT successful
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VAntiReplayWindowVerify(
  TcpIp_SaEntryIterType         SaEntryIdx);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VAntiReplayWindowUpdate
 *********************************************************************************************************************/
/*! \brief          Updates anti replay window boundaries based on the received sequence number
 *  \details        This function should be called only after the Rx packet has been authenticated
 *  \param[in]      SaEntryIdx       SA Entry index
 *                                   \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \pre            The packet has been authenticated successfully
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VAntiReplayWindowUpdate(
  TcpIp_SaEntryIterType         SaEntryIdx);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VMatchSpdSelectors
 *********************************************************************************************************************/
/*! \brief          Matches the SPD entry selector fields with the input selectors
 *  \details        -
 *  \param[in]      SpdEntryIdx      SPD Entry index
 *                                   \spec requires SpdEntryIdx < TcpIp_GetSizeOfSpdEntry(); \endspec
 *  \param[in]      Protocol         Upper layer protocol
 *  \param[in]      RemSock          Remote address and port
 *  \param[in]      LocSock          Local address and port
 *  \return         TRUE             SPD Entry matches the input selectors
 *  \return         FALSE            SPD Entry does not match the input selectors
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_IpSec_VMatchSpdSelectors(
  TcpIp_SpdEntryIterType              SpdEntryIdx,
  uint8                               Protocol,
  IpBase_SockAddrInType               RemSock,
  IpBase_SockAddrInType               LocSock);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VMatchSaSelectors
 *********************************************************************************************************************/
/*! \brief          Matches the SA entry selector fields with the input selectors
 *  \details        -
 *  \param[in]      SaEntryIdx       SA Entry index
 *                                   \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in]      Protocol         Upper layer protocol
 *  \param[in]      RemSock          Remote address and port
 *  \param[in]      LocSock          Local address and port
 *  \return         TRUE             SA Entry matches the input selectors
 *  \return         FALSE            SA Entry does not match the input selectors
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_IpSec_VMatchSaSelectors(
  TcpIp_SaEntryIterType               SaEntryIdx,
  uint8                               Protocol,
  IpBase_SockAddrInType               RemSock,
  IpBase_SockAddrInType               LocSock);


/* Pair of functions to make IpSec secure Tx/Rx packet */
/**********************************************************************************************************************
 *  TcpIp_IpSec_VSecureTxPacket
 *********************************************************************************************************************/
/*! \brief          Function to perform IpSec security operation on the packet to be transmitted
 *  \details        -
 *  \param[in]      SaEntryIdx                        SA Entry index
 *                                                    \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in]      IpPacketPtr                       Pointer to Ip packet buffer
 *                                                    CONSTRAINT: Buffer MUST have enough space
 *                                                    enforced by VCA_TCPIP_IPSEC_IP_PKT_PTR_CHECKED
 *                                                    \spec requires $lengthOf(IpPacketPtr) >= IPV4_IP_DEF_HEADER_LENGTH_BYTE + TCPIP_IPSEC_AUTH_HDR_LEN_FIXED; \endspec
 *  \param[in]      IpTotalLen                        Number of bytes in Ip Packet
 *  \param[in]      IpNextHeader                      Upper layer protocol header
 *  \return         TCPIP_IPSEC_NO_EVENT              Packet processed successfully
 *  \return         TCPIP_IPSEC_EVENT_GEN_FAILED      Icv generation failed, not processed successfully
 *  \return         TCPIP_IPSEC_EVENT_INVALID_HDR     Packet contained invalid header, not processed successfully
 *  \return         TCPIP_IPSEC_EVENT_SEQ_OVERFLOW    Sequence number overflow, not processed successfully
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(TcpIp_IpSecEventType, TCPIP_CODE) TcpIp_IpSec_VSecureTxPacket(
  TcpIp_SaEntryIterType             SaEntryIdx,
  TCPIP_P2V(uint8)                  IpPacketPtr,
  uint16                            IpTotalLen,
  uint8                             IpNextHeader);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VSecureRxPacket
 *********************************************************************************************************************/
/*! \brief          Function to perform IpSec security verification on the packet to received
 *  \details        -
 *  \param[in]      SaEntryIdx                        SA Entry index
 *                                                    \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in]      IpPacketPtr                       Pointer to Ip packet
 *  \param[in]      IpSecHdr                          IpSec protocol header
 *  \param[in]      IpPayloadLen                      Number of bytes in Ip payload
 *  \return         TCPIP_IPSEC_NO_EVENT              Packet processed successfully
 *  \return         TCPIP_IPSEC_EVENT_VERIFY_FAILED   Icv verification failed, not processed successfully
 *  \return         TCPIP_IPSEC_EVENT_INVALID_HDR     Packet contained invalid header, not processed successfully
 *  \return         TCPIP_IPSEC_EVENT_ANTIREPLAY      Anti replay, not processed successfully
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(TcpIp_IpSecEventType, TCPIP_CODE) TcpIp_IpSec_VSecureRxPacket(
  TcpIp_SaEntryIterType    SaEntryIdx,
  TCPIP_P2C(uint8)         IpPacketPtr,
  uint8                    IpSecHdr,
  uint16                   IpPayloadLen);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VSeqNumOverflowCheckAndUpdate
 *********************************************************************************************************************/
/*! \brief          Check for Overflow of Sequence number and increment
 *  \details        This function is called only for Outbound SA Entries
 *  \param[in]      SaEntryIdx       SA Entry index (Outbound)
 *                                   \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \return         E_OK             Anti Replay check successful and Sequence Number updated
 *  \return         E_NOT_OK         Anti Replay check unsuccessful
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VSeqNumOverflowCheckAndUpdate(
  TcpIp_SaEntryIterType    SaEntryIdx);


/**********************************************************************************************************************
 *  TcpIp_IpSec_VBuildAuthHeader
 *********************************************************************************************************************/
/*! \brief          Builds and fills up authentication header fields
 *  \details        -
 *  \param[in]      SaEntryIdx       SA Entry index
 *                                   \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[out]     IpAuthHdrPtr     Pointer to the Authentication header buffer
 *                                   \spec requires $lengthOf(IpAuthHdrPtr) >= TCPIP_IPSEC_AUTH_HDR_LEN_FIXED; \endspec
 *                                   CONSTRAINT: MUST NOT be null and atleast TcpIp_IpSec_VGetIpSecHeaderLengthBytes
 *  \param[in]      IpNextHeader     upper layer protocol header of the IP packet
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VBuildAuthHeader(
  TcpIp_SaEntryIterType             SaEntryIdx,
  TCPIP_P2V(uint8)                  IpAuthHdrPtr,
  uint8                             IpNextHeader);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VSelectorIpAddressMatch
 *********************************************************************************************************************/
/*! \brief          Checks if Address lies in the given range
 *  \details        The function converts the arguments to Hbo and performs validation
 *  \param[in]      AddrtoMatch      Address to match
 *  \param[in]      StartAddrRange   Starting Address (TCPIP_INADDR_ANY implies any address will match)
 *  \param[in]      EndAddrRange     Ending Address (TCPIP_INADDR_ANY implies only single address is specified)
 *  \return         TRUE             Address matches the Input range
 *  \return         FALSE            Address does not match the input range
 *  \pre            All address arguments are expected in Nbo
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_IpSec_VSelectorIpAddressMatch(
  IpBase_AddrInType           AddrtoMatch,
  IpBase_AddrInType           StartAddrRange,
  IpBase_AddrInType           EndAddrRange);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VSelectorPortMatch
 *********************************************************************************************************************/
/*! \brief          Checks if Port number lies in the given range
 *  \details        The function converts the arguments to Hbo and performs validation
 *  \param[in]      PortNumtoMatch   Port number to match
 *  \param[in]      StartPortRange   Starting port number
 *  \param[in]      EndPortRange     Ending port number (TCPIP_PORT_ANY implies only single address is specified)
 *  \return         TRUE             Port number matches the Input range
 *  \return         FALSE            Port number does not match the input range
 *  \pre            All port arguments are expected in Hbo
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_IpSec_VSelectorPortMatch(
  IpBase_PortType              PortNumtoMatch,
  IpBase_PortType              StartPortRange,
  IpBase_PortType              EndPortRange);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VObtainInitVectorFromAh
 *********************************************************************************************************************/
/*! \brief          Extract the IV value from the AH
 *  \details        -
 *  \param[in]      SaEntryIdx       SA Entry index
 *                                   \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in]      IpAuthHdrPtr     Pointer to the Ip Auth header
 *                                   CONSTRAINT: MUST NOT be null and atleast TcpIp_IpSec_VGetIpSecHeaderLengthBytes
 *  \param[in]      InitVectorPtr    Initialization Vector of the packet
 *                                   CONSTRAINT: MUST NOT be null and atleast TCPIP_IPSEC_GMAC_IV_LEN bytes
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VObtainInitVectorFromAh(
  TcpIp_SaEntryIterType    SaEntryIdx,
  TCPIP_P2C(uint8)         IpAuthHdrPtr,
  TCPIP_P2V(uint8)         InitVectorPtr);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VIsGmacAlgorithmFamily
 *********************************************************************************************************************/
/*! \brief          Check if the algorithm family is Gmac
 *  \details        -
 *  \param[in]      SaEntryIdx   SA Entry index
 *                               \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \return         TRUE         Sa entry uses Gmac algorithm
 *  \return         FALSE        Sa entry uses other algorithm
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_IpSec_VIsGmacAlgorithmFamily(
  TcpIp_SaEntryIterType    SaEntryIdx);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VMuteIpHeaderFields
 *********************************************************************************************************************/
/*! \brief          Mutes the necessary Ip header fields for AH processing
 *  \details        -
 *  \param[in,out]  IpHdrPtr     Pointer to the Ip header buffer to be muted
 *                               CONSTRAINT: MUST point to a valid and have IPV4_IP_DEF_HEADER_LENGTH_BYTE size
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VMuteIpHeaderFields(
  TCPIP_P2V(uint8)                          IpHdrPtr);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VInvokeSpdCallout
 *********************************************************************************************************************/
/*! \brief          Invokes the Spd callout function
 *  \details        -
 *  \param[in]      IpSecConfigSetIdx  Index of the ip config set
 *  \param[in,out]  SecurityPolicyPtr  Security policy for the packet
 *  \param[in]      Protocol           Ip protocol
 *  \param[in]      RemSock            Remote socket address
 *  \param[in]      LocSock            Local socket address
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VInvokeSpdCallout(
  TcpIp_IpSecConfigSetIdxOfIpV4CtrlType IpSecConfigSetIdx,
  TCPIP_P2V(TcpIp_IpSecPolicyType)      SecurityPolicyPtr,
  uint8                                 Protocol,
  IpBase_SockAddrInType                 RemSock,
  IpBase_SockAddrInType                 LocSock);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VAuthenticateTxPacket
 *********************************************************************************************************************/
/*! \brief          Authentication process for Tx packet
 *  \details        -
 *  \param[in]      SaEntryIdx         SA Entry index
 *                                     \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in,out]  IpPacketPtr        Pointer to the IP buffer. ICV is inserted if return is E_OK
 *                                     \spec requires $lengthOf(IpPacketPtr) >= IPV4_IP_DEF_HEADER_LENGTH_BYTE + TCPIP_IPSEC_AUTH_HDR_LEN_FIXED; \endspec
 *  \param[in]      IpTotalLen         Length of the IP buffer bytes. For ESN support it is 4 Bytes less
                                       (see VCA_TCPIP_IPSEC_PUT_ESN_TO_RESERVED_MEMORY)
 *  \param[in]      IpNextHeader       upper layer protocol header of the IP packet
 *  \return         E_OK               Packet authenticated successfully
 *  \return         E_NOT_OK           Packet could not be authenticated
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VAuthenticateTxPacket(
  TcpIp_SaEntryIterType             SaEntryIdx,
  TCPIP_P2V(uint8)                  IpPacketPtr,
  uint16                            IpTotalLen,
  uint8                             IpNextHeader);

/**********************************************************************************************************************
 *  TcpIp_IpSec_VAuthenticateRxPacket
 *********************************************************************************************************************/
/*! \brief          Authentication process for Rx packet
 *  \details        Icv of the received packet is extracted and processed
 *  \param[in]      SaEntryIdx         SA Entry index
 *                                     \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \param[in]      IpPacketPtr        Pointer to the IP buffer.
 *  \param[in]      IpTotalLen         Length of the IP buffer bytes.
 *                                     CONSTRAINT: [0..TcpIp_GetSizeOfRxProcessBuffer()] without ESN
 *                                     CONSTRAINT: [0..TcpIp_GetSizeOfRxProcessBuffer() - 4] with ESN
 *  \return         E_OK               Packet Icv verified successfully
 *  \return         E_NOT_OK           Packet Icv could not be verified
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VAuthenticateRxPacket(
  TcpIp_SaEntryIterType    SaEntryIdx,
  TCPIP_P2C(uint8)         IpPacketPtr,
  uint16                   IpTotalLen);

/**********************************************************************************************************************
 *  TcpIp_VReadPortFromProtocolHdr()
 **********************************************************************************************************************/
/*! \brief          Parses port information form the Upper layer header
 *  \details        The function extracts the port numbers from the upper layer header
 *  \param[in]      IpPayloadPtr  Pointer to Ip Payload
 *  \param[in]      IpProtocol    Upper layer protocol used (IPV4_IP_PROTOCOL_TCP or IPV4_IP_PROTOCOL_UDP is supported)
 *  \param[in]      HdrOffset     Offset to the upper layer protocol
 *  \param[out]     TgtPortPtr    Pointer to Target port
 *                                CONSTRAINT: MUST NOT be NULL_PTR
 *  \param[out]     SrcPortPtr    Pointer to Source port
 *                                CONSTRAINT: MUST NOT be NULL_PTR
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_VReadPortFromProtocolHdr(
  TCPIP_P2C(uint8)              IpPayloadPtr,
  uint8                         IpProtocol,
  uint16                        HdrOffset,
  TCPIP_P2V(IpBase_PortType)    TgtPortPtr,
  TCPIP_P2V(IpBase_PortType)    SrcPortPtr);

/**********************************************************************************************************************
 * TcpIp_IpSec_GetLocSocketAddr()
 *********************************************************************************************************************/
/*! \brief          Retrieve the local socket address for a given socket
 *  \details        -
 *  \param[in]       IpV4CtrlIdx     Index of the affected IPv4 controller.
 *                                   \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *                                   CONSTRAINT: TcpIp_IsIpSecConfigSetUsedOfIpV4Ctrl(IpV4CtrlIdx) is valid
 *  \param[in]      IpV4SocketIdx   IpV4 socket index
 *                  CONSTRAINT      [0...TcpIp_GetSizeOfSocketDyn()-1]
 *  \param[out]     LocSockPtr      Local socket address
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_GetLocSocketAddr(
  TcpIp_IpV4CtrlIterType            IpV4CtrlIdx,
  TcpIp_SocketDynIterType           IpV4SocketIdx,
  TCPIP_P2V(IpBase_SockAddrInType)  LocSockPtr);

# if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VCalculateSecsElapsed()
 **********************************************************************************************************************/
/*! \brief          Returns the seconds elapsed since last event (Rx/Tx) on the SA entry
 *  \details        This functions calculates time elapsed since last event based on IpV4 time
 *  \param[in]      SaEntryIdx         SA Entry index
 *                                     \spec requires SaEntryIdx < TcpIp_GetSizeOfSaEntry(); \endspec
 *  \return         SecsElapsed        Seconds Elapsed since last event
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(uint16, TCPIP_CODE) TcpIp_VCalculateSecsElapsed(
  TcpIp_SaEntryIterType  SaEntryIdx);
# endif /* (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

 /**********************************************************************************************************************
  *  TcpIp_IpSec_MainFunction
  *********************************************************************************************************************/
  /*!
   * Internal comment removed.
 *
 *
   */
FUNC(void, TCPIP_CODE) TcpIp_IpSec_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SaEntryIterType ipSaEntryIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all SAs and set to tx_rx if Tx timeout is over */
  for (ipSaEntryIdx = 0u; ipSaEntryIdx < TcpIp_GetSizeOfSaEntry(); ipSaEntryIdx++)
  {
    TcpIp_SaTxActivationTimeoutOfIpSecConfigSetType txTimeoutSa
      = TcpIp_GetSaTxActivationTimeoutOfIpSecConfigSet(TcpIp_GetIpSecConfigSetOfSaEntry(ipSaEntryIdx));

    if ((TcpIp_GetStateOfSaEntry(ipSaEntryIdx) == TCPIP_IPSEC_STATUS_RX_ONLY)
      && (txTimeoutSa != 0u))
    {
      TcpIp_IncSaTxActivationTimeoutOfSaEntry(ipSaEntryIdx);

      /* Update IPsec state if timeout has expired */
      if (TcpIp_GetSaTxActivationTimeoutOfSaEntry(ipSaEntryIdx) >= txTimeoutSa)
      {
        TcpIp_SetStateOfSaEntry(ipSaEntryIdx, TCPIP_IPSEC_STATUS_TX_RX);
      }
    }
  }
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_IpSec_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SaEntryIterType             ipSaEntryIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Traverse through all SA entries and clear entries */
  for (ipSaEntryIdx = 0; ipSaEntryIdx < TcpIp_GetSizeOfSaEntry(); ipSaEntryIdx++)
  {
    TcpIp_IpSec_VRemoveSaEntry(ipSaEntryIdx);
  }

  {
    TcpIp_IntegrityJobPairDynIterType integrityJobPairIdx;
    /* #20 Traverse through job pair list and set all to unused */
    for (integrityJobPairIdx = 0; integrityJobPairIdx < TcpIp_GetSizeOfIntegrityJobPairDyn(); integrityJobPairIdx++)
    {
      TcpIp_SetStatusOfIntegrityJobPairDyn(integrityJobPairIdx, TCPIP_IPSEC_STATUS_JOBPAIR_UNUSED);
    }
  }

  TcpIp_IpSecSaEntryCnt = 1u;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VIsGmacAlgorithmFamily
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_IpSec_VIsGmacAlgorithmFamily(
  TcpIp_SaEntryIterType    SaEntryIdx)
{
  boolean retVal = FALSE;

  /*  #10 Check if the algorithm family is Gmac */
  if (   (TcpIp_GetIntegrityTransformIdentOfSaEntry(SaEntryIdx) == TCPIP_IPSEC_TRANSFORM_AUTH_AES_128_GMAC)             /* PRQA S 3415 3 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
      || (TcpIp_GetIntegrityTransformIdentOfSaEntry(SaEntryIdx) == TCPIP_IPSEC_TRANSFORM_AUTH_AES_192_GMAC)
      || (TcpIp_GetIntegrityTransformIdentOfSaEntry(SaEntryIdx) == TCPIP_IPSEC_TRANSFORM_AUTH_AES_256_GMAC))
  {
    retVal = TRUE;
  }

  return retVal;
} /* TcpIp_IpSec_VIsGmacAlgorithmFamily() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VObtainInitVectorFromAh
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VObtainInitVectorFromAh(
  TcpIp_SaEntryIterType    SaEntryIdx,
  TCPIP_P2C(uint8)         IpAuthHdrPtr,
  TCPIP_P2V(uint8)         InitVectorPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 authDataIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the IV from the AH */
  for (authDataIdx = 0; authDataIdx < TCPIP_IPSEC_GMAC_IV_LEN; authDataIdx++)
  {
    InitVectorPtr[TCPIP_IPSEC_GMAC_SALT_LEN + authDataIdx] = IpAuthHdrPtr[TCPIP_IPSEC_AH_HDR_OFS_AUTHDATA + authDataIdx];
  }

  /* #20 Copy IV into the Sa Entry table */
  /* PRQA S 0315 1 */ /* MD_MSR_VStdLibCopy */
  IpBase_Copy(InitVectorPtr, &TcpIp_GetAddrNonceOfSaEntry(SaEntryIdx)->Nonce[0], TCPIP_IPSEC_GMAC_SALT_LEN);
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VMuteIpHeaderFields
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VMuteIpHeaderFields(
  TCPIP_P2V(uint8)                          IpHdrPtr)
{

  /* #10 Mute the values */
  TCPIP_PUT_UINT8(IpHdrPtr, TCPIP_IPV4_HDR_OFS_TOS, 0u);
  TCPIP_PUT_UINT8(IpHdrPtr, TCPIP_IPV4_HDR_OFS_TTL, 0u);
  TCPIP_PUT_UINT16(IpHdrPtr, TCPIP_IPV4_HDR_OFS_FLAGS_OFFSET, 0u);
  TCPIP_PUT_UINT16_RAW(IpHdrPtr, TCPIP_IPV4_HDR_OFS_CHECKSUM, TCPIP_CHECKSUM_UNSET);

}

/**********************************************************************************************************************
 *  TcpIp_VReadPortFromProtocolHdr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_VReadPortFromProtocolHdr(
  TCPIP_P2C(uint8)              IpPayloadPtr,
  uint8                         IpProtocol,
  uint16                        HdrOffset,
  TCPIP_P2V(IpBase_PortType)    TgtPortPtr,
  TCPIP_P2V(IpBase_PortType)    SrcPortPtr)
{
  /* #10 Check the upper layer protocol and parse the source and target port numbers */
  if (IpProtocol == IPV4_IP_PROTOCOL_TCP)
  {
    *SrcPortPtr = IPBASE_HTON16(TCPIP_GET_UINT16(IpPayloadPtr, (HdrOffset)));
    *TgtPortPtr = IPBASE_HTON16(TCPIP_GET_UINT16(IpPayloadPtr, (HdrOffset + TCPIP_TCP_HDR_TARGET_PORT_OFFSET)));
  }
  else if(IpProtocol == IPV4_IP_PROTOCOL_UDP)
  {
    *SrcPortPtr = IPBASE_HTON16(TCPIP_GET_UINT16(IpPayloadPtr, (HdrOffset)));
    *TgtPortPtr = IPBASE_HTON16(TCPIP_GET_UINT16(IpPayloadPtr, (HdrOffset + TCPIP_UDP_HDR_TARGET_PORT_OFFSET)));
  }
  else /* port numbers are not valid */
  {
    *SrcPortPtr = TCPIP_PORT_ANY;
    *TgtPortPtr = TCPIP_PORT_ANY;
  }
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VSecureRxPacket
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(TcpIp_IpSecEventType, TCPIP_CODE) TcpIp_IpSec_VSecureRxPacket(
  TcpIp_SaEntryIterType    SaEntryIdx,
  TCPIP_P2C(uint8)         IpPacketPtr,
  uint8                    IpSecHdr,
  uint16                   IpPayloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_IpSecEventType       eventId = TCPIP_IPSEC_NO_EVENT;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Obtain the Seqnum of the packet from the Authentication header */
  TcpIp_SeqNumLOfSaEntryType seqNumRcvd = TCPIP_GET_UINT32(IpPacketPtr, (IPV4_IP_DEF_HEADER_LENGTH_BYTE + TCPIP_IPSEC_AH_HDR_OFS_SEQNUM));
  TcpIp_SetSeqNumLOfSaEntry(SaEntryIdx, seqNumRcvd);

  /* ----- Implementation ----------------------------------------------- */
  switch (IpSecHdr)
  {
  case TCPIP_PROTOCOL_AH:
    {
      /* #20 Check for anti replay attack and update sequence number */
      if (TcpIp_IpSec_VAntiReplayWindowVerify(SaEntryIdx) == E_OK) /* Verify Anti-replay window */
      {
        /* #30 If anti replay check successful, authenticate packet */
        if (TcpIp_IpSec_VAuthenticateRxPacket(SaEntryIdx, IpPacketPtr, (IpPayloadLen + IPV4_IP_DEF_HEADER_LENGTH_BYTE)) == E_OK)
        {
          /* #40 After successful authentication update the anti replay window for the new sequence number */
          TcpIp_IpSec_VAntiReplayWindowUpdate(SaEntryIdx);
        }
        else
        {
          /* Icv verification unsuccessful, audit event */
          eventId = TCPIP_IPSEC_EVENT_VERIFY_FAILED;
        }
      }
      else
      {
        /* Anti replay attack occurred, audit event */
        eventId = TCPIP_IPSEC_EVENT_ANTIREPLAY;
      }
    }
    break;
  default:
    {
      /* Currently only AH is supported, audit event */
      eventId = TCPIP_IPSEC_EVENT_INVALID_HDR;
    }
    break;
  }

  /* Return the eventId */
  return eventId;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_ProcessTxPacket
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
 *
 */
 /* PRQA S 6030, 6050 1 */ /* MD_MSR_STCYC, MD_TcpIp_STCAL_CslAccess */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_ProcessTxPacket(
  TcpIp_IpV4CtrlIterType                      IpV4CtrlIdx,
  TCPIP_P2C(TcpIp_IpTxRequestDescriptorType)  IpTxReqDescrPtr,
  TCPIP_P2V(uint8)                            IpPacketPtr,
  uint16                                      IpTotalLen,
  uint8                                       IpNextHeader
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType          retVal = E_NOT_OK;
  boolean                 notFragFlag = (boolean)(TCPIP_GET_UINT16(IpPacketPtr, TCPIP_IPV4_HDR_OFS_FLAGS_OFFSET) == IPV4_IP_FLAGS_DONT_FRAGMENT); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  TcpIp_SizeOfSaEntryType saEntryIdx = IpTxReqDescrPtr->SaEntryIdx;
  TcpIp_IpSecEventType    eventId = TCPIP_IPSEC_NO_EVENT;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the security policy for transmitting the packet */
  switch (IpTxReqDescrPtr->IpSecPolicy)
  {
    case TCPIP_IPSEC_POLICY_BYPASS:
    /* #20 If policy is BYPASS, no IpSec processing is required */
    {
      /* do nothing */
    }
    break;
    case TCPIP_IPSEC_POLICY_PROTECT:
    case TCPIP_IPSEC_POLICY_OPTIONAL:
    /* #30 If policy is OPTIONAL or PROTECT, try to perform IpSec processing */
    {
      /* #40 Check if valid SA entry exists for authenticating the packet */
      if (saEntryIdx >= TcpIp_GetSizeOfSaEntry())
      {
        /* #50 If no valid SA entry found for authentication, and policy is PROTECT, drop packet */
        if (IpTxReqDescrPtr->IpSecPolicy == TCPIP_IPSEC_POLICY_PROTECT)
        {
          /* No valid SA entry found, audit event */
          eventId = TCPIP_IPSEC_EVENT_SAENTRY_NOTFOUND;
        }
        /* #60 Otherwise if policy is OPTIONAL, packet can still be transmitted even if SA entry is not present */
        else
        {
          /* do nothing */
        }
      }
      /* #70 If valid SA entry found, check if packet is fragmented */
      else
      {

        if ((notFragFlag == TRUE) && (IpTxReqDescrPtr->EthBufferCnt == 1u)) /* The buffer counter is set when the buffer is requested */
        {
          /*@ assert $lengthOf(IpPacketPtr) >= IPV4_IP_DEF_HEADER_LENGTH_BYTE + TCPIP_IPSEC_AUTH_HDR_LEN_FIXED; */      /* VCA_TCPIP_IPSEC_IP_PKT_PTR_CHECKED */

          /* #80 If packet is not going to be fragmented, perform IpSec processing */
          eventId = TcpIp_IpSec_VSecureTxPacket(saEntryIdx, IpPacketPtr, IpTotalLen, IpNextHeader);

# if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
          if (eventId == TCPIP_IPSEC_NO_EVENT)
          {
            /* Update Diagnostic parameters if packet was successfully processed */
            TcpIp_IncPktCntOfSaEntry(saEntryIdx);
            TcpIp_SetLastPktPrcTimeOfSaEntry(saEntryIdx, TcpIp_IpV4_GetCurrTimeMs());
          }
# endif /* (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON) */
        }
        else
        {
          /* #90 If packet is going to be fragmented, audit event */
          eventId = TCPIP_IPSEC_EVENT_FRAGMENT;
        }
      }
    }
    break;
    default:
    /* #100 If no policy is selected, default is DISCARD, packet shall not be transmitted */
    {
      /* default DISCARD policy, audit event */
      eventId = TCPIP_IPSEC_EVENT_DISCARD_POLICY;
    }
    break;
  }

  /* ----- Audit Event --------------------------------------- */
  /* #110 Invoke audit event callout function if any event was detected */
  if (eventId == TCPIP_IPSEC_NO_EVENT)
  {
    /* Processing was successful, update retVal to E_OK */
    retVal = E_OK;
  }
  else
  {
    /* Processing was unsuccessful, retVal remains E_NOT_OK */
    TcpIp_IpSecConfigSetIdxOfIpV4CtrlType ipSecConfigSetIdx = TcpIp_GetIpSecConfigSetIdxOfIpV4Ctrl(IpV4CtrlIdx);
    /*@ assert ipSecConfigSetIdx < TcpIp_GetSizeOfIpSecConfigSet(); */                                                  /* VCA_TCPIP_CSL03_IS_USEDOF */
    TcpIp_IpSecAuditEventCalloutFunctionType auditEventCalloutFunctionPtr = TcpIp_GetAuditEventCalloutFunctionOfIpSecConfigSet(ipSecConfigSetIdx);

    if (auditEventCalloutFunctionPtr != NULL_PTR)
    {
      TcpIp_SockAddrBaseType rSock, lSock;                                                                              /* PRQA S 0759 */ /* MD_MSR_18.4 */
      uint32                 spi = TCPIP_IPSEC_SPI_ANY;
      uint32                 seqNum = TCPIP_MAX_SEQNUMLOFSAENTRY;

      /* extract the AH information from the SA entry */
      if (saEntryIdx < TcpIp_GetSizeOfSaEntry())
      {
        spi    = TcpIp_GetSpiOfSaEntry(saEntryIdx);
        seqNum = TcpIp_GetSeqNumLOfSaEntry(saEntryIdx);
      }
      /* extract the address information */
      lSock.SockAddrIn.sin_family = IPBASE_AF_INET;
      lSock.SockAddrIn.sin_addr   = TCPIP_GET_UINT32_RAW(IpPacketPtr, TCPIP_IPV4_HDR_OFS_SRC_ADDR);
      lSock.SockAddrIn.sin_port   = TCPIP_PORT_ANY;  /* undefined */
      rSock.SockAddrIn.sin_family = IPBASE_AF_INET;
      rSock.SockAddrIn.sin_addr   = TCPIP_GET_UINT32_RAW(IpPacketPtr, TCPIP_IPV4_HDR_OFS_DEST_ADDR);
      rSock.SockAddrIn.sin_port   = TCPIP_PORT_ANY;  /* undefined */

      auditEventCalloutFunctionPtr(spi, seqNum, (TcpIp_SockAddrType *)&rSock, (TcpIp_SockAddrType *)&lSock, eventId);   /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
    }
  }
  /* #120 Set InUse flag to IDLE */
  if (saEntryIdx < TcpIp_GetSizeOfSaEntry())
  {
    TcpIp_SetInUseFlagOfSaEntry(saEntryIdx, TCPIP_IPSEC_SAENTRY_IDLE);
  }

  return retVal;
}/* TcpIp_IpSec_ProcessTxPacket() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VSecureTxPacket
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
TCPIP_LOCAL FUNC(TcpIp_IpSecEventType, TCPIP_CODE) TcpIp_IpSec_VSecureTxPacket(
  TcpIp_SaEntryIterType    SaEntryIdx,
  TCPIP_P2V(uint8)         IpPacketPtr,
  uint16                   IpTotalLen,
  uint8                    IpNextHeader)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_IpSecEventType  eventId = TCPIP_IPSEC_NO_EVENT;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check which all IpSec headers have to be inserted  */
  switch (TcpIp_GetIpSecHdrTypeOfSaEntry(SaEntryIdx))
  {
  case TCPIP_IPSEC_HDR_ESP:
    {
      /* Currently ESP header is not supported, audit event */
      eventId = TCPIP_IPSEC_EVENT_INVALID_HDR;
    }
    break;
  case TCPIP_IPSEC_HDR_AH:
    {
      /* #20 If Authentication header is to be inserted, check for sequence number overflow and update  */
      if (TcpIp_IpSec_VSeqNumOverflowCheckAndUpdate(SaEntryIdx) == E_OK)
      {
        /* #30 If sequence number updated successfully, authenticate the packet for transmission */
        if (TcpIp_IpSec_VAuthenticateTxPacket(SaEntryIdx, IpPacketPtr, IpTotalLen, IpNextHeader) == E_OK)
        {
          /* do nothing */
        }
        else
        {
          /* #40 Or if Icv generation has failed, audit event */
          eventId = TCPIP_IPSEC_EVENT_GEN_FAILED;
        }
      }
      else
      {
        /* #50 Or if sequence number has overflown, audit event */
        eventId = TCPIP_IPSEC_EVENT_SEQ_OVERFLOW;
      }
    }
    break;
  default:
    {
      /* No valid IpSec support, audit event */
      eventId = TCPIP_IPSEC_EVENT_INVALID_HDR;
    }
    break;
  }

  /* return the eventId */
  return eventId;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VGetIpSecHeaderLengthBytes
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(uint16, TCPIP_CODE) TcpIp_IpSec_VGetIpSecHeaderLengthBytes(
  TcpIp_SaEntryIterType   SaEntryIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 ipSecHdrLen = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if SA Entry is valid */
  if (SaEntryIdx < TcpIp_GetSizeOfSaEntry())
  {
    /* #20 If AH header is supported for the SA Entry, calculate AH header length */
    if(TcpIp_GetIpSecHdrTypeOfSaEntry(SaEntryIdx) == TCPIP_IPSEC_HDR_AH)
    {
      /* #30 If the SA entry uses Gmac algorithm family, IV length is also added */
      if (TcpIp_IpSec_VIsGmacAlgorithmFamily(SaEntryIdx) == TRUE)
      {
        ipSecHdrLen = TCPIP_IPSEC_GMAC_IV_LEN;
      }

      ipSecHdrLen += TCPIP_IPSEC_AUTH_HDR_LEN_FIXED + TcpIp_GetIntegrityChkLenOfSaEntry(SaEntryIdx);
    }
    /* else (handling if ESP, or both headers are enabled) */
  }
  /* #40 If no valid SA Entry found, IpSec header will not be inserted */
  else
  {
    ipSecHdrLen = 0;
  }

  return (ipSecHdrLen);
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VSeqNumOverflowCheckAndUpdate
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
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VSeqNumOverflowCheckAndUpdate(
  TcpIp_SaEntryIterType   SaEntryIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal = E_OK;
  uint32 counter32 = TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx);
  uint32 counter64 = TcpIp_GetSeqNumHOfSaEntry(SaEntryIdx);

  /* ----- Implementation ----------------------------------------------- */
  if (TcpIp_IsEsnSupportOfSaEntry(SaEntryIdx) == TRUE)
  {
  /* #10 Check if counters have overflow */
    if (counter64 == TCPIP_IPSEC_ESN_OVERFLOW_32BIT)
    {
      if (counter32 == TCPIP_IPSEC_ESN_OVERFLOW_32BIT)
      {
        /* #20 If both counters have overflow, SA Entry can no longer be used */
        retVal = E_NOT_OK;
      }
      else
      {
        /* #30 If only counter64 has overflow, increment counter32 */
        counter32++;
      }
    }
    else
    {
      if (counter32 == TCPIP_IPSEC_ESN_OVERFLOW_32BIT)
      {
        /* #40 If only counter32 has overflow, Increment counter64 */
        counter64++;
        counter32 = 0;
      }
      else
      {
        /* #50 If no overflow, increment counter32 */
        counter32++;
      }
    }

  }
  else
  {
    /* #60 If ESN not supported, use only counter32 */
    counter64 = TCPIP_MAX_SEQNUMHOFSAENTRY;

    if (counter32 == TCPIP_IPSEC_ESN_OVERFLOW_32BIT)
    {
      /* #70 If counter32 has overflow, SA Enrty can no longer be used */
      retVal = E_NOT_OK;
    }
    else
    {
      /* #80 If no overflow, increment counter32 */
      counter32++;
    }
  }

  /* #90 Update counter values into SA entry */
  TcpIp_SetSeqNumHOfSaEntry(SaEntryIdx, counter64);
  TcpIp_SetSeqNumLOfSaEntry(SaEntryIdx, counter32);

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VBuildAuthHeader
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
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VBuildAuthHeader(
  TcpIp_SaEntryIterType             SaEntryIdx,
  TCPIP_P2V(uint8)                  IpAuthHdrPtr,
  uint8                             IpNextHeader)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ahAuthIcvOffset = TCPIP_IPSEC_AH_HDR_OFS_AUTHDATA;

  /*
   * Header length is calculated as Total Header Length in bytes /4 - 2
   * total header length is always >= TCPIP_IPSEC_AUTH_HDR_LEN_FIXED for a valid SA entry (no underflow)
   */
  uint8 ipSecHdrLen = (uint8)((TcpIp_IpSec_VGetIpSecHeaderLengthBytes(SaEntryIdx)) >> 2) - 2u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update Ah fields */
  TCPIP_PUT_UINT8 (IpAuthHdrPtr, TCPIP_IPSEC_AH_HDR_OFS_NEXTHDR,    IpNextHeader);
  TCPIP_PUT_UINT8 (IpAuthHdrPtr, TCPIP_IPSEC_AH_HDR_OFS_PAYLOADLEN, ipSecHdrLen);
  TCPIP_PUT_UINT16 (IpAuthHdrPtr, TCPIP_IPSEC_AH_HDR_OFS_RSVD, 0u);
  TCPIP_PUT_UINT32(IpAuthHdrPtr, TCPIP_IPSEC_AH_HDR_OFS_SPI,        TcpIp_GetSpiOfSaEntry(SaEntryIdx));
  TCPIP_PUT_UINT32(IpAuthHdrPtr, TCPIP_IPSEC_AH_HDR_OFS_SEQNUM,     TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx));

  /* #20 Fill Auth Data field with IV */
  if( TcpIp_IpSec_VIsGmacAlgorithmFamily(SaEntryIdx) == TRUE)
  {
    TCPIP_P2C(uint8) ivPtr = TcpIp_GetAddrNonceOfSaEntry(SaEntryIdx)->Nonce;

    TCPIP_VENTER_CRITICAL_SECTION();
    /* #30 Generate the IV */
    TcpIp_IpSec_VInitVectorGenerate(SaEntryIdx);

    /* #40 Copy the IV into the ICV field in the AH */
    /* PRQA S 0315 1 */ /* MD_MSR_VStdLibCopy */
    IpBase_Copy(&IpAuthHdrPtr[TCPIP_IPSEC_AH_HDR_OFS_AUTHDATA], &ivPtr[TCPIP_IPSEC_GMAC_SALT_LEN], TCPIP_IPSEC_GMAC_IV_LEN); /* VCA_TCPIP_IPSEC_IPBASECOPY_IPBUF_FIXED_ARRAY */

    TCPIP_VLEAVE_CRITICAL_SECTION();

    /* #50 Add the IV length to the Ah header length */
    ahAuthIcvOffset += TCPIP_IPSEC_GMAC_IV_LEN;
  }

  /* #60 Fill Icv with 0 */
  {
    uint8 authDataIdx;
    for (authDataIdx = 0; authDataIdx < TcpIp_GetIntegrityChkLenOfSaEntry(SaEntryIdx); authDataIdx++)
    {
      TCPIP_PUT_UINT8(IpAuthHdrPtr, (ahAuthIcvOffset + authDataIdx), 0u);
    }
  }
} /* TcpIp_IpSec_VBuildAuthHeader() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VSelectorIpAddressMatch
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
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_IpSec_VSelectorIpAddressMatch(
  IpBase_AddrInType           AddrtoMatch,
  IpBase_AddrInType           StartAddrRange,
  IpBase_AddrInType           EndAddrRange)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean matchAddr = FALSE;

  /* #10 Convert all the parameters into HBO to perform comparision */
  IpBase_AddrInType addrInHbo       = IPBASE_NTOH32(AddrtoMatch);
  IpBase_AddrInType startRangeInHbo = IPBASE_NTOH32(StartAddrRange);
  IpBase_AddrInType endRangeInHbo   = IPBASE_NTOH32(EndAddrRange);

  /* ----- Implementation ----------------------------------------------- */
  /* #20 If the end address is not specified */
  if (endRangeInHbo == TCPIP_INADDR_ANY)
  {
    if (startRangeInHbo == TCPIP_INADDR_ANY)
    {
      /* #30 If no start address specified, any port matches */
      matchAddr = TRUE;
    }
    else if (addrInHbo == startRangeInHbo)
    {
      /* #40 If only start address is specified, check if address and start address match */
      matchAddr = TRUE;
    }
    else
    {
      /* Not a match */
    }
  }
  /* #50 If end address is specified */
  else if (startRangeInHbo != TCPIP_INADDR_ANY)
  {
    /* #60 If address range specified, check if address lies in the range */
    if (   (addrInHbo >= startRangeInHbo)
        && (addrInHbo <= endRangeInHbo))
    {
      matchAddr = TRUE;
    }
  }
  else
  {
    /* invalid condition */
  }

  return matchAddr;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VSelectorPortMatch
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
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_IpSec_VSelectorPortMatch(
  IpBase_PortType                   PortNumtoMatch,
  IpBase_PortType                   StartPortRange,
  IpBase_PortType                   EndPortRange)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean matchPort = FALSE;

  /* #10 Convert all the parameters into HBO to perform comparision */
  IpBase_PortType portInHbo       = IPBASE_NTOH16(PortNumtoMatch);
  IpBase_PortType startRangeInHbo = IPBASE_NTOH16(StartPortRange);
  IpBase_PortType endRangeInHbo   = IPBASE_NTOH16(EndPortRange);

  /* ----- Implementation ----------------------------------------------- */
  /* #20 If the end port is not specified */
  if (endRangeInHbo == TCPIP_PORT_ANY)
  {
    if (startRangeInHbo == TCPIP_PORT_ANY)
    {
      /* #30 If no start port specified, any port matches */
      matchPort = TRUE;
    }
    else if (portInHbo == startRangeInHbo)
    {
      /* #40 If only start port is specified, check if port and start port match */
      matchPort = TRUE;
    }
    else
    {
      /* Not a match */
    }
  }
  /* #50 If end port is specified */
  else if (startRangeInHbo != TCPIP_PORT_ANY)
  {
    /* #60 If port range specified, check if port lies in the range */
    if (   (portInHbo >= startRangeInHbo)
        && (portInHbo <= endRangeInHbo))
    {
      matchPort = TRUE;
    }
  }
  else
  {
    /* invalid condition */
  }

  return matchPort;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_UpdateCsmKeyElement()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_UpdateCsmKeyElement(
  TcpIp_SaEntryIterType                 SaEntryIdx,
  uint32                                KeyId,
  TCPIP_P2C(uint8)                      KeyMatPtr,
  uint16                                KeyMatLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Determine the algorithm transform which is used */
  switch (TcpIp_GetIntegrityTransformIdentOfSaEntry(SaEntryIdx))
  {
  case TCPIP_IPSEC_TRANSFORM_AUTH_AES_128_GMAC:
  case TCPIP_IPSEC_TRANSFORM_AUTH_AES_192_GMAC:
  case TCPIP_IPSEC_TRANSFORM_AUTH_AES_256_GMAC:
    {
      /* #20 Update the key material into the Cipher key elements for GMAC Csm jobs */
      /* Key value is present in the key material */
      retVal = TCPIP_CSM_KeyElementSet(KeyId, CRYPTO_KE_CIPHER_KEY, KeyMatPtr, ((uint32) KeyMatLen - TCPIP_IPSEC_GMAC_SALT_LEN));

      /* #30 Update the Salt value into the Sa entry table */
      /* Salt value is present after the key value in the out key material */
      /* PRQA S 0315 1 */ /* MD_MSR_VStdLibCopy */
      IpBase_Copy(TcpIp_GetAddrNonceOfSaEntry(SaEntryIdx), &KeyMatPtr[(KeyMatLen - TCPIP_IPSEC_GMAC_SALT_LEN)], TCPIP_IPSEC_GMAC_SALT_LEN);
    }
    break;
  default:
    {
      /* #40 Update the key material into the Crypto key elements for other Csm jobs */
      /* Key value is present in the key material */
      retVal = TCPIP_CSM_KeyElementSet(KeyId, CRYPTO_KE_MAC_KEY, KeyMatPtr, KeyMatLen);
    }
    break;
  }

  if (retVal == E_OK)
  {
    retVal = TCPIP_CSM_KeySetValid(KeyId);
  }

  return retVal;
} /* TcpIp_IpSec_UpdateKeyElements() */

/**********************************************************************************************************************
 *  TcpIp_IpSecVAllocateJobPair()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecVAllocateJobPair(
  TcpIp_IntegrityAlgorithmIterType               IntegrityAlgorithmIdx,
  TCPIP_P2V(TcpIp_IntegrityJobPairIterType)      JobPairIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  TcpIp_IntegrityJobPairIterType      integrityJobPairIdx;

  /* #10 Traverse through the job pair list to find unused pair */
  for (integrityJobPairIdx = TcpIp_GetIntegrityJobPairStartIdxOfIntegrityAlgorithm(IntegrityAlgorithmIdx); integrityJobPairIdx < TcpIp_GetIntegrityJobPairEndIdxOfIntegrityAlgorithm(IntegrityAlgorithmIdx); integrityJobPairIdx++)
  {
    if (TcpIp_GetStatusOfIntegrityJobPairDyn(integrityJobPairIdx) == TCPIP_IPSEC_STATUS_JOBPAIR_UNUSED)
    {
      /* #20 When unused job pair found, set to allocated */
      TcpIp_SetStatusOfIntegrityJobPairDyn(integrityJobPairIdx, TCPIP_IPSEC_STATUS_JOBPAIR_ALLOCATED);
      *JobPairIdxPtr = integrityJobPairIdx;
      retVal = E_OK;
      break;
    }
  }

  if (retVal != E_OK)
  {
    /* If no job pair is available, update invalid index to caller */
    *JobPairIdxPtr = TcpIp_GetSizeOfIntegrityJobPair();
  }

  return retVal;
} /* TcpIp_IpSecVAllocateJobPair() */

/**********************************************************************************************************************
 *  TcpIp_IpSecVIntegrityIntegTransformLookup()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSecVIntegrityIntegTransformLookup(
  TcpIp_LocalAddrIterType                     LocalAddrIdx,
  TcpIp_IpSecIntegrityTransformType           IntegTransformIdent,
  TCPIP_P2V(TcpIp_IntegrityJobPairIterType)   JobPairIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (TCPIP_LOCAL_ADDR_IDX_IS_IPV4(LocalAddrIdx))
  {
    /* Determine the IpV4 controller index */
    TcpIp_LocalAddrV4IterType         localAddrV4Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(LocalAddrIdx);
    TcpIp_IpV4CtrlIterType            ipV4CtrlIdx = TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(localAddrV4Idx);
    TcpIp_IpSecConfigSetIterType      ipSecConfigSetIdx = TcpIp_GetIpSecConfigSetIdxOfIpV4Ctrl(ipV4CtrlIdx);
    TcpIp_IntegrityAlgorithmIterType  integrityAlgorithmIdx;

    /* #10 Check if the IpSecConfigSet index is valid */
    if (ipSecConfigSetIdx != TCPIP_NO_IPSECCONFIGSETIDXOFIPV4CTRL)
    {
      /* #20 Traverse through the configured integrity transforms of the IpSecConfig */
      for (integrityAlgorithmIdx = TcpIp_GetIntegrityAlgorithmStartIdxOfIpSecConfigSet(ipSecConfigSetIdx); integrityAlgorithmIdx < TcpIp_GetIntegrityAlgorithmEndIdxOfIpSecConfigSet(ipSecConfigSetIdx); integrityAlgorithmIdx++)
      {
        /* #30 Check if the requested algorithm is configured in the IpSecConfig */
        if (TcpIp_GetIntegrityTransformIdentOfIntegrityAlgorithm(integrityAlgorithmIdx) == IntegTransformIdent)
        {
          /* If the algorithm is configured, request for an unused Job pair */
          retVal = TcpIp_IpSecVAllocateJobPair(integrityAlgorithmIdx, JobPairIdxPtr);
          break;
        }
      }
    }
  }
  else
  {
    /* Not a valid IpV4 address id */
  }

  return retVal;
}/* TcpIp_IpSecVIntegrityIntegTransformLookup() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_AddSaEntryPair()
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_AddSaEntryPair(
  TcpIp_LocalAddrIterType           LocalAddrIdx,
  TCPIP_P2C(TcpIp_IpSecSaInfoType)  SaInfoPtr,
  boolean                           IsInitiator)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SaEntryIterType inSaEntryIdx  = 0;
  TcpIp_SaEntryIterType outSaEntryIdx = 0;
  Std_ReturnType        retVal        = E_NOT_OK;
  TcpIp_IntegrityJobPairIterType     integrityJobPairIdx = TcpIp_GetSizeOfIntegrityJobPair();

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Lookup for a matching csm integrity job pair for the sa entry */
  if (TcpIp_IpSecVIntegrityIntegTransformLookup(LocalAddrIdx, SaInfoPtr->SecurityTransform.IntegTransformIdent, &integrityJobPairIdx) == E_OK)
  {
    /* #20 Request for the inbound SA entry */
    if (TcpIp_IpSec_VAddSaEntry(TCPIP_IPSEC_TRAFFIC_INBOUND, LocalAddrIdx, integrityJobPairIdx, SaInfoPtr, &inSaEntryIdx) == E_OK)
    {
      /*@ assert inSaEntryIdx < TcpIp_GetSizeOfSaEntry(); */                                                            /* VCA_TCPIP_IPSEC_SAENTRYPAIR_IDX_CHECKED */
      /*#30 If inbound SA entry obtained successfully, request for the outbound SA entry */
      if (TcpIp_IpSec_VAddSaEntry(TCPIP_IPSEC_TRAFFIC_OUTBOUND, LocalAddrIdx, integrityJobPairIdx, SaInfoPtr, &outSaEntryIdx) == E_OK)
      {
      /*@ assert outSaEntryIdx < TcpIp_GetSizeOfSaEntry(); */                                                           /* VCA_TCPIP_IPSEC_SAENTRYPAIR_IDX_CHECKED */
        TcpIp_LocalAddrV4IterType localAddrV4Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(LocalAddrIdx);
        TcpIp_IpV4CtrlIterType ipV4CtrlIdx = TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(localAddrV4Idx);

        retVal = E_OK;

        /* Update the Sa entry pair index of the outbound Sa in the inbound Sa entry */
        TcpIp_SetSaEntryPairIdxOfSaEntry(inSaEntryIdx, (TcpIp_SizeOfSaEntryType)outSaEntryIdx);

        /* Update the Sa entry pair index of the inbound Sa in the outbound Sa entry */
        TcpIp_SetSaEntryPairIdxOfSaEntry(outSaEntryIdx, (TcpIp_SizeOfSaEntryType)inSaEntryIdx);

        /* Update the Priority of the SA entry */
        TcpIp_SetPriorityOfSaEntry(inSaEntryIdx, TcpIp_IpSecSaEntryCnt);
        TcpIp_SetPriorityOfSaEntry(outSaEntryIdx, TcpIp_IpSecSaEntryCnt);
        TcpIp_IpSecSaEntryCnt++;

        TcpIp_SetSaTxActivationTimeoutOfSaEntry(inSaEntryIdx, 0u);
        TcpIp_SetSaTxActivationTimeoutOfSaEntry(outSaEntryIdx, 0u);

        /* Set the state of the SA entry depending on if IKE is initiator and the Tx timeout */
        if ((IsInitiator == TRUE)
          || (TcpIp_GetSaTxActivationTimeoutOfIpSecConfigSet(TcpIp_GetIpSecConfigSetIdxOfIpV4Ctrl(ipV4CtrlIdx)) == 0u))
        {
          /* SA entry pair is immediately usable for Rx and Tx */
          TcpIp_SetStateOfSaEntry(inSaEntryIdx, TCPIP_IPSEC_STATUS_TX_RX);
          TcpIp_SetStateOfSaEntry(outSaEntryIdx, TCPIP_IPSEC_STATUS_TX_RX);
        }
        else
        {
          /* SA entry pair is usable only for Rx */
          TcpIp_SetStateOfSaEntry(inSaEntryIdx, TCPIP_IPSEC_STATUS_RX_ONLY);
          TcpIp_SetStateOfSaEntry(outSaEntryIdx, TCPIP_IPSEC_STATUS_RX_ONLY);
        }
      }
      else
      {
        /* #40 If the SA entry pair could not be created, delete the first entry */
        TcpIp_IpSec_VRemoveSaEntry(inSaEntryIdx);
      }
    }
  }

  if (   (retVal != E_OK)
      && (integrityJobPairIdx < TcpIp_GetSizeOfIntegrityJobPair()))                                                     /* PRQA S 3415 */ /* MD_IPV4_Rule12.4_3415 */
  {
    /* Sa Entry Pair was not created successfully - Set to unused */
    TcpIp_SetStatusOfIntegrityJobPairDyn(integrityJobPairIdx, TCPIP_IPSEC_STATUS_JOBPAIR_UNUSED);
  }

  return retVal;
} /* TcpIp_IpSec_AddSaEntryPair() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VInitEsnSupport
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE)  TcpIp_IpSec_VInitEsnSupport(
  TcpIp_SaEntryIterType IpSaEntryIdx,
  boolean               EsnEnabled)
{
  /*
    * Rfc2402 3.3.2 Sequence number is initialized to 0 when SA is created
    * If ESN is enabled, reset ESN to 0, else to TCPIP_MAX_SEQNUMHOFSAENTRY
    */
  if(EsnEnabled == TRUE)
  {
    /* #10 If ESN is supported, initialize the sequence number higher word to 0 */
    TcpIp_SetEsnSupportOfSaEntry(IpSaEntryIdx, TRUE);
    TcpIp_SetSeqNumHOfSaEntry(IpSaEntryIdx, 0);
  }
  else
  {
    /* #20 Otherwise, set the sequence number higher word to TCPIP_MAX_SEQNUMHOFSAENTRY */
    TcpIp_SetEsnSupportOfSaEntry(IpSaEntryIdx, FALSE);
    TcpIp_SetSeqNumHOfSaEntry(IpSaEntryIdx, TCPIP_MAX_SEQNUMHOFSAENTRY);
  }
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VInitVectorGenerate
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VInitVectorGenerate(
  TcpIp_SaEntryIterType       SaEntryIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 idx;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_P2V(uint8)  ivPtr = TcpIp_GetAddrNonceOfSaEntry(SaEntryIdx)->Nonce;

  /* #10 Generate random number in the range 0-255 */
  uint8 randOffset = (uint8)TcpIp_GetRandomNumber();

  /* #20 Add the generated number to the current IV */
  for (idx = 0; idx < TCPIP_IPSEC_GMAC_IV_LEN; idx++)
  {
    if (randOffset > 0u)
    {
      /* calculate sum */
      uint16 sum = ((uint16) ivPtr[TCPIP_IPSEC_GMAC_SALT_LEN + idx] + (uint16) randOffset);

      /* calculate carry forward */
      randOffset = (uint8)((sum > 0xFFu) ? 1u : 0u);

      /* update the Iv value */
      ivPtr[TCPIP_IPSEC_GMAC_SALT_LEN + idx] = (uint8)sum;
    }
  }
} /* TcpIp_IpSec_VInitVectorGenerate() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VAddIpV4SaEntry
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
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_IpSec_VAddIpV4SaEntry(
  uint8                             Dir,
  TcpIp_SaEntryIterType             SaEntryIdx,
  TcpIp_IntegrityJobPairIterType    IntegrityJobPairIdx,
  TCPIP_P2C(TcpIp_IpSecSaInfoType)  SaInfoPtr,
  TcpIp_IpV4CtrlIterType            IpV4CtrlIdx)

{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ---------------------------------------------- */
  /* #10 Check if the traffic selector is valid */
  if (   (SaInfoPtr->TrafficSelectors != NULL_PTR)
      && (SaInfoPtr->NumTrafficSelectors != 0u))
  {
    TCPIP_P2C(TcpIp_IpSecTrafficSelectorType) trafficSelPtr     = &(SaInfoPtr->TrafficSelectors[TCPIP_IPSEC_TRAFFICSEL_INDEX]);
    TCPIP_P2C(TcpIp_SockAddrBaseType)         remAddrPtr        = (TCPIP_P2C(TcpIp_SockAddrBaseType))trafficSelPtr->RemoteAddr; /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
    TcpIp_IntegrityAlgorithmIterType          integrityAlgorithmIdx = TcpIp_GetIntegrityAlgorithmIdxOfIntegrityJobPair(IntegrityJobPairIdx);

    TCPIP_ASSERT(remAddrPtr->sa_family == IPBASE_AF_INET);

    /* Set the integrity algorithm transform for the Sa */
    TcpIp_SetIntegrityTransformIdentOfSaEntry(SaEntryIdx, TcpIp_GetIntegrityTransformIdentOfIntegrityAlgorithm(integrityAlgorithmIdx));

    /* #20 Check the direction of the traffic and update the CSM key elements */
    if (Dir == TCPIP_IPSEC_TRAFFIC_INBOUND)
    {
      /* Set the Spi for the inbound entry */
      TcpIp_SetSpiOfSaEntry(SaEntryIdx, SaInfoPtr->InboundSpi);
      /* Update the key element for the inbound job with the key material */
      retVal = TcpIp_IpSec_UpdateCsmKeyElement(SaEntryIdx, TcpIp_GetVerifyKeyIdOfIntegrityJobPair(IntegrityJobPairIdx), SaInfoPtr->KeyMaterial.InboundKeyMatPtr, SaInfoPtr->KeyMaterial.InboundKeyMatLen);
    }
    else
    {
      /* Set the Spi for the outbound sa entry */
      TcpIp_SetSpiOfSaEntry(SaEntryIdx, SaInfoPtr->OutboundSpi);
      /* Update the key element for the outbound job with the key material */
      retVal = TcpIp_IpSec_UpdateCsmKeyElement(SaEntryIdx, TcpIp_GetGenerateKeyIdOfIntegrityJobPair(IntegrityJobPairIdx), SaInfoPtr->KeyMaterial.OutboundKeyMatPtr, SaInfoPtr->KeyMaterial.OutboundKeyMatLen);
    }

    /* #30 Create the SA entry if adding key element was successful */
    if (retVal == E_OK)
    {
      TCPIP_VENTER_CRITICAL_SECTION();
      /* Set direction of the sa entry */
      TcpIp_SetDirectionOfSaEntry(SaEntryIdx, Dir);

      /* Set sequence number L */
      TcpIp_SetSeqNumLOfSaEntry(SaEntryIdx, 0);

      /* Set the IPsec configset */
      TcpIp_SetIpSecConfigSetOfSaEntry(SaEntryIdx, TcpIp_GetIpSecConfigSetIdxOfIpV4Ctrl(IpV4CtrlIdx));

      /* Set the traffic selector fields */
      TcpIp_SetLocPortEndOfSaEntry(SaEntryIdx, IPBASE_HTON16(trafficSelPtr->LocalPortRangeEnd));
      TcpIp_SetLocPortStartOfSaEntry(SaEntryIdx, IPBASE_HTON16(trafficSelPtr->LocalPortRangeStart));
      TcpIp_SetRemIpV4AddrOfSaEntry(SaEntryIdx, remAddrPtr->SockAddrIn.sin_addr);
      TcpIp_SetRemPortEndOfSaEntry(SaEntryIdx, IPBASE_HTON16(trafficSelPtr->RemotePortRangeEnd));
      TcpIp_SetRemPortStartOfSaEntry(SaEntryIdx, IPBASE_HTON16(trafficSelPtr->RemotePortRangeStart));
      TcpIp_SetXptProtOfSaEntry(SaEntryIdx, trafficSelPtr->IpProtocol);

      /* Set Esn support and init the sequence num H for the Sa */
      TcpIp_IpSec_VInitEsnSupport(SaEntryIdx, SaInfoPtr->SecurityTransform.EsnEnabled);
      /* Set the ICV length for Sa */
      TcpIp_SetIntegrityChkLenOfSaEntry(SaEntryIdx, TcpIp_GetIcvLengthOfIntegrityAlgorithm(integrityAlgorithmIdx));
      /* Set Csm job pair id for the entry */
      TcpIp_SetIntegrityJobPairIdxOfSaEntry(SaEntryIdx, (TcpIp_IntegrityJobPairIdxOfSaEntryType)IntegrityJobPairIdx);

# if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
      /* Reset Diagnostic parameters */
      TcpIp_SetPktCntOfSaEntry(SaEntryIdx, 0u);
      TcpIp_SetLastPktPrcTimeOfSaEntry(SaEntryIdx, 0u);
# endif /* (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON) */
      TCPIP_VLEAVE_CRITICAL_SECTION();
    }
  }
  /* #40 Set SA entry to free when a error occurs */
  if(retVal != E_OK)
  {
    /* Set SA item to unused */
    TcpIp_SetIpSecHdrTypeOfSaEntry(SaEntryIdx, TCPIP_IPSEC_HDR_NONE);
  }

  return retVal;
} /* TcpIp_IpSec_VAddIpV4SaEntry() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VFindUnusedSaEntry
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_IpSec_VFindUnusedSaEntry(
  TcpIp_IpV4CtrlIterType            IpV4CtrlIdx,
  TCPIP_P2V(TcpIp_SaEntryIterType)  IpSaEntryIdxPtr,
  TcpIp_IpSecHeaderType             IpsecHdrType)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SaEntryIterType  ipSaEntryIdx;
  Std_ReturnType         retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Traverse through the SAD */
  TCPIP_VENTER_CRITICAL_SECTION();
  for (ipSaEntryIdx = TcpIp_GetSaEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx); ipSaEntryIdx < TcpIp_GetSaEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx); ipSaEntryIdx++)
  {
    /*@ assert ipSaEntryIdx < TcpIp_GetSizeOfSaEntry(); */                                                              /* VCA_TCPIP_CSL03_START_END */

    /* #20 If header type is not specified, then the SA Entry is unused */
    if (TcpIp_GetIpSecHdrTypeOfSaEntry(ipSaEntryIdx) == TCPIP_IPSEC_HDR_NONE)
    {
      /* unused SA Entry found */
      retVal = E_OK;
      TcpIp_SetIpSecHdrTypeOfSaEntry(ipSaEntryIdx, IpsecHdrType);
      break;
    }
  }
  TCPIP_VLEAVE_CRITICAL_SECTION();

  if (ipSaEntryIdx == TcpIp_GetSaEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx))
  {
    /* #30 If no unused entry found return Size of SAD */
    ipSaEntryIdx = TcpIp_GetSizeOfSaEntry();
  }

  /* #40 Return the selected entry */
  *IpSaEntryIdxPtr = ipSaEntryIdx;

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VAddSaEntry
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_IpSec_VAddSaEntry(
  uint8                              Dir,
  TcpIp_LocalAddrIterType            LocalAddrIdx,
  TcpIp_IntegrityJobPairIterType     JobPairIdx,
  TCPIP_P2C(TcpIp_IpSecSaInfoType)   SaInfoPtr,
  TCPIP_P2V(TcpIp_SaEntryIterType)   SaEntryIdxSelectedPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType         retVal = E_NOT_OK;
  TcpIp_SaEntryIterType  ipSaEntryIdx = TcpIp_GetSizeOfSaEntry();

  /* ----- Implementation ----------------------------------------------- */

  /* Check the IP version of the address index */
  if (TCPIP_LOCAL_ADDR_IDX_IS_IPV4(LocalAddrIdx))
  {
    TcpIp_LocalAddrV4IterType localAddrV4Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(LocalAddrIdx);
    /* Determine the IpV4 controller index */
    TcpIp_IpV4CtrlIterType    ipV4CtrlIdx  = TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(localAddrV4Idx);

    /* #10 Get unused SA Entry index for the controller */
    if (TcpIp_IpSec_VFindUnusedSaEntry(ipV4CtrlIdx, &ipSaEntryIdx, SaInfoPtr->IpsecHdrType) == E_OK)
    {
      /* #20 if unused entry found, add the Sa entry */
      /*@ assert ipSaEntryIdx < TcpIp_GetSizeOfSaEntry(); */                                                            /* VCA_TCPIP_IPSEC_SAENTRYIDX_FROM_ADDSAENTRY */
      retVal = TcpIp_IpSec_VAddIpV4SaEntry(Dir, ipSaEntryIdx, JobPairIdx, SaInfoPtr, ipV4CtrlIdx);
    }
  }

  /* #30 Update the selected Sa entry index if sucessfully added */
  if (retVal == E_OK)
  {
    *SaEntryIdxSelectedPtr = ipSaEntryIdx;
  }
  else
  {
     *SaEntryIdxSelectedPtr = TcpIp_GetSizeOfSaEntry();
  }

  return retVal;
} /* TcpIp_IpSec_VAddSaEntry() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_DelSaEntryPair
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_IpSec_DelSaEntryPair(
  uint32               InSpi)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SaEntryIterType     ipSaEntryIdx;
  Std_ReturnType            retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Look for an SA entry with matching Spi */
  for (ipSaEntryIdx = 0; ipSaEntryIdx < TcpIp_GetSizeOfSaEntry(); ipSaEntryIdx++)
  {
    if (TcpIp_GetSpiOfSaEntry(ipSaEntryIdx) == InSpi)
    {
      break;
    }
  }

  /* #20 Remove the SA entry pair associated with the Spi */
  if (   (ipSaEntryIdx < TcpIp_GetSizeOfSaEntry())
      && (TcpIp_GetSaEntryPairIdxOfSaEntry(ipSaEntryIdx) < TcpIp_GetSizeOfSaEntry()))                                   /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
  {
    TcpIp_SaEntryIterType     pairedIpSaEntryIdx = TcpIp_GetSaEntryPairIdxOfSaEntry(ipSaEntryIdx);
    /*@ assert pairedIpSaEntryIdx < TcpIp_GetSizeOfSaEntry(); */                                                        /* VCA_TCPIP_CSL01 */
    if (    (TcpIp_GetInUseFlagOfSaEntry(ipSaEntryIdx) == TCPIP_IPSEC_SAENTRY_IDLE)
         && (TcpIp_GetInUseFlagOfSaEntry(pairedIpSaEntryIdx) == TCPIP_IPSEC_SAENTRY_IDLE))                              /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
    {
      TcpIp_SizeOfIntegrityJobPairDynType integrityJobPairIdx = TcpIp_GetIntegrityJobPairIdxOfSaEntry(ipSaEntryIdx);
      /*@ assert integrityJobPairIdx < TcpIp_GetSizeOfIntegrityJobPairDyn(); */                                         /* VCA_TCPIP_IPSEC_JOBPAIR_IDX_FROM_SAENTRYIDX */
      retVal = E_OK;

      /* Set the job pair which was allocated for the SaEntry Pair to Unused */
      TcpIp_SetStatusOfIntegrityJobPairDyn(integrityJobPairIdx, TCPIP_IPSEC_STATUS_JOBPAIR_UNUSED);

      /* remove the paired SA Entry first */
      TcpIp_IpSec_VRemoveSaEntry(pairedIpSaEntryIdx);

      /* remove the SA Entry */
      TcpIp_IpSec_VRemoveSaEntry(ipSaEntryIdx);
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_PrepareDelSaEntryPair
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, TCPIP_CODE)  TcpIp_IpSec_PrepareDelSaEntryPair(
  uint32               InSpi)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SaEntryIterType     ipSaEntryIdx;
  Std_ReturnType            retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_VENTER_CRITICAL_SECTION();
  /* #10 Look for an SA entry with matching Spi */
  for (ipSaEntryIdx = 0; ipSaEntryIdx < TcpIp_GetSizeOfSaEntry(); ipSaEntryIdx++)
  {
    if ((TcpIp_GetSpiOfSaEntry(ipSaEntryIdx) == InSpi)
      && (TcpIp_GetDirectionOfSaEntry(ipSaEntryIdx) == TCPIP_IPSEC_TRAFFIC_INBOUND))
    {
      break;
    }
  }

  /* #20 If matching entry found, set the SA entry pair associated with the Spi to STALE */
  if ((ipSaEntryIdx < TcpIp_GetSizeOfSaEntry())
    && (TcpIp_GetSaEntryPairIdxOfSaEntry(ipSaEntryIdx) < TcpIp_GetSizeOfSaEntry()))                                     /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
  {
    TcpIp_SaEntryIterType pairedIpSaEntryIdx = TcpIp_GetSaEntryPairIdxOfSaEntry(ipSaEntryIdx);
    /*@ assert pairedIpSaEntryIdx < TcpIp_GetSizeOfSaEntry(); */                                                        /* VCA_TCPIP_RUNTIME_CHECK_PRESENT_IN_SAME_FUNCTION */

    TcpIp_SetStateOfSaEntry(ipSaEntryIdx, TCPIP_IPSEC_STATUS_STALE);
    TcpIp_SetStateOfSaEntry(pairedIpSaEntryIdx, TCPIP_IPSEC_STATUS_STALE);
    retVal = E_OK;
  }
  TCPIP_VLEAVE_CRITICAL_SECTION();

  return retVal;
} /* TcpIp_IpSec_PrepareDelSaEntryPair */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VRemoveSaEntry
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(void, TCPIP_CODE)  TcpIp_IpSec_VRemoveSaEntry(
  TcpIp_SaEntryIterType     SaEntryIdx)
{
  /* #10 Clear all SA Entry fields */
  TcpIp_SetDirectionOfSaEntry(SaEntryIdx, TCPIP_IPSEC_TRAFFIC_INVALID);
  TcpIp_SetIntegrityJobPairIdxOfSaEntry(SaEntryIdx, TcpIp_GetSizeOfIntegrityJobPair());
  TcpIp_SetEsnSupportOfSaEntry(SaEntryIdx, FALSE);
  TcpIp_SetIntegrityChkLenOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetLocPortEndOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetLocPortStartOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetRemIpV4AddrOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetRemPortEndOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetRemPortStartOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetSpiOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetIpSecHdrTypeOfSaEntry(SaEntryIdx, TCPIP_IPSEC_HDR_NONE);
  TcpIp_SetIntegrityTransformIdentOfSaEntry(SaEntryIdx, TCPIP_IPSEC_TRANSFORM_AUTH_NONE);

  /* Clear anti replay window parameters */
  TcpIp_SetUWndHOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetUWndLOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetSeqNumHOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetSeqNumLOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetWndBitsOfSaEntry(SaEntryIdx, 0);

  TcpIp_SetPriorityOfSaEntry(SaEntryIdx, 0);
  TcpIp_SetStateOfSaEntry(SaEntryIdx, TCPIP_IPSEC_STATUS_UNSET);

  TcpIp_SetInUseFlagOfSaEntry(SaEntryIdx, TCPIP_IPSEC_SAENTRY_IDLE);

  /* Clear the Sa entry pair parameter */
  TcpIp_SetSaEntryPairIdxOfSaEntry(SaEntryIdx, TcpIp_GetSizeOfSaEntry());

# if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
  /* Clear Diagnostic parameters */
  TcpIp_SetPktCntOfSaEntry(SaEntryIdx, 0u);
  TcpIp_SetLastPktPrcTimeOfSaEntry(SaEntryIdx, 0u);
# endif /* (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON) */
} /* TcpIp_IpSec_VRemoveSaEntry() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VLookupSecurityPolicy
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(TcpIp_IpSecPolicyType, TCPIP_CODE) TcpIp_IpSec_VLookupSecurityPolicy(
  TcpIp_IpV4CtrlIterType                IpV4CtrlIdx,
  uint8                                 IpProtocol,
  IpBase_SockAddrInType                 RemSock,
  IpBase_SockAddrInType                 LocSock,
  TCPIP_P2V(TcpIp_SpdEntryIterType)     IpSpdEntryIdxSelected)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_IpSecPolicyType                 ipSecPolicySelected = TCPIP_IPSEC_POLICY_DISCARD;
  TcpIp_SpdEntryIterType                ipSpdEntryIdx;

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_IpSecConfigSetIdxOfIpV4CtrlType ipSecConfigSetIdx = TcpIp_GetIpSecConfigSetIdxOfIpV4Ctrl(IpV4CtrlIdx);
  *IpSpdEntryIdxSelected = TcpIp_GetSizeOfSpdEntry();

  /* #10 Traverse through the SPD entries for a correct match */
  for (ipSpdEntryIdx = TcpIp_GetSpdEntryStartIdxOfIpSecConfigSet(ipSecConfigSetIdx); ipSpdEntryIdx < TcpIp_GetSpdEntryEndIdxOfIpSecConfigSet(ipSecConfigSetIdx); ipSpdEntryIdx++)
  {
    /*@ assert ipSpdEntryIdx < TcpIp_GetSizeOfSpdEntry(); */                                                            /* VCA_TCPIP_CSL03_START_END */
    /* #20 Check if the protocol and the selctors of the packet match to that of the SPD entry */
    if (   (   (TcpIp_GetXptProtocolOfSpdEntry(ipSpdEntryIdx) == IpProtocol)                                            /* PRQA S 3415 3 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
            || (TcpIp_GetXptProtocolOfSpdEntry(ipSpdEntryIdx) == TCPIP_PROTOCOL_ANY))
        && (TcpIp_IpSec_VMatchSpdSelectors(ipSpdEntryIdx, IpProtocol, RemSock, LocSock) == TRUE))                       /* PRQA S 3415 */ /* MD_IPV4_Rule12.4_3415 */
    {
      /* Best match found */
      ipSecPolicySelected    = TcpIp_GetPolicyTypeOfSpdEntry(ipSpdEntryIdx);
      *IpSpdEntryIdxSelected = ipSpdEntryIdx;
      break;
    }
  }

  return ipSecPolicySelected;
} /* TcpIp_IpSec_VLookupSecurityPolicy() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VUpdateTxDescriptorForAuthentication
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
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VUpdateTxDescriptorForAuthentication(
  TcpIp_IpSecPolicyType                       IpSecPolicy,
  TcpIp_SaEntryIterType                       SaEntryIdxSelected,
  TCPIP_P2V(TcpIp_IpTxRequestDescriptorType)  IpTxReqDescrPtr,
  TCPIP_P2V(TcpIp_IpSecEventType)             EventIdPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the IpSec tx buffer parameters (ExtHdrLen, SaEntryIdx) based on the type of security policy chosen */
  switch (IpSecPolicy)
  {
  case TCPIP_IPSEC_POLICY_PROTECT:
    {
      /* #20 If the policy is PROTECT and there is no valid SA entry, drop the packet */
      if (SaEntryIdxSelected >= TcpIp_GetSizeOfSaEntry())
      {
        *EventIdPtr = TCPIP_IPSEC_EVENT_SAENTRY_NOTFOUND;
        IpTxReqDescrPtr->SaEntryIdx = TcpIp_GetSizeOfSaEntry();
        IpTxReqDescrPtr->ExtHdrLenBytes = 0;
      }
      /* #30 Otherwise, update the IpSec tx buffer parameters */
      else
      {
        *EventIdPtr = TCPIP_IPSEC_NO_EVENT;
        IpTxReqDescrPtr->SaEntryIdx = (TcpIp_SizeOfSaEntryType)SaEntryIdxSelected;
        IpTxReqDescrPtr->ExtHdrLenBytes = TcpIp_IpSec_VGetIpSecHeaderLengthBytes(SaEntryIdxSelected);
        if (TcpIp_IsEsnSupportOfSaEntry(IpTxReqDescrPtr->SaEntryIdx) == TRUE)
        {
          /* To support ESN 4 extra Bytes are needed at the and of the packet that are not transmitted. */
          IpTxReqDescrPtr->ReservedTrailer = TCPIP_IPSEC_ESN_LEN;
        }
      }
    }
    break;
  case TCPIP_IPSEC_POLICY_OPTIONAL:
      /* packet shall be transmitted
       * - with IpSec if matching SA entry found
       * - without IpSec if there is no matching SA entry found
       */
  case TCPIP_IPSEC_POLICY_BYPASS:
    /*
     * Packet shall be transmitted normally. Update IpSec parameters in Tx descriptor
     */
    /* #40 If the policy is OPTIONAL or BYPASS, packet shall be transmitted */
    {
      *EventIdPtr         = TCPIP_IPSEC_NO_EVENT;
      /* value is TcpIp_GetSizeOfSaEntry() in case of bypass or optional bypass */
      IpTxReqDescrPtr->SaEntryIdx = (TcpIp_SizeOfSaEntryType)SaEntryIdxSelected;
      /* value is 0 in case of bypass or optional bypass */
      IpTxReqDescrPtr->ExtHdrLenBytes = TcpIp_IpSec_VGetIpSecHeaderLengthBytes(SaEntryIdxSelected);

      if (   (IpTxReqDescrPtr->SaEntryIdx < TcpIp_GetSizeOfSaEntry())
          && (TcpIp_IsEsnSupportOfSaEntry(IpTxReqDescrPtr->SaEntryIdx) == TRUE))
      {
        IpTxReqDescrPtr->ReservedTrailer = TCPIP_IPSEC_ESN_LEN;
      }
    }
    break;
  default:
    /* #50 If no match was found, TCPIP_IPSEC_POLICY_DISCARD shall be used and packet shall be dropped */
    {
      *EventIdPtr = TCPIP_IPSEC_EVENT_DISCARD_POLICY;
      IpTxReqDescrPtr->SaEntryIdx = TcpIp_GetSizeOfSaEntry();
      IpTxReqDescrPtr->ExtHdrLenBytes = 0;
    }
    break;
  }
} /* TcpIp_IpSec_VUpdateTxDescriptorForAuthentication() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_PreparePacketforAuthentication
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
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_PreparePacketforAuthentication(
  TcpIp_IpV4CtrlIterType                      IpV4CtrlIdx,
  TcpIp_SocketDynIterType                     SocketIdx,
  TCPIP_P2V(TcpIp_IpTxRequestDescriptorType)  IpTxReqDescrPtr,
  IpBase_SockAddrInType                       RemSock
)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_IpSecPolicyType                 ipSecPolicy;
  Std_ReturnType                        retVal = E_NOT_OK;
  TcpIp_SaEntryIterType                 saEntryIdx = TcpIp_GetSizeOfSaEntry();
  TcpIp_IpSecEventType                  eventId = TCPIP_IPSEC_NO_EVENT;
  TcpIp_IpSecHeaderType                 ipsecHdrType = TCPIP_IPSEC_HDR_NONE;
  IpBase_SockAddrInType                 locSock;

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SizeOfIpSecConfigSetType        ipSecConfigSetIdx = TcpIp_GetIpSecConfigSetIdxOfIpV4Ctrl(IpV4CtrlIdx);
  /*@ assert ipSecConfigSetIdx < TcpIp_GetSizeOfIpSecConfigSet(); */                                                    /* VCA_TCPIP_CSL03_IS_USEDOF */

  /*
    * STORYC- 5813 - All tx requests shall be processed as per
    * Section 5.1 of [RFC 4301]
    * Nested IpSec headers are not supported
    */
  /* Retrieve the local socket address for the socket */
  TcpIp_IpSec_GetLocSocketAddr(IpV4CtrlIdx, SocketIdx, &locSock);

  /* #10 Check if the packet should be authenticated or not */
  if (   (IpTxReqDescrPtr->XptProtocol == TCPIP_PROTOCOL_ICMP)
      && (   (IpTxReqDescrPtr->MessageType != TCPIP_IPV4_ICMP_TYPE_ECHO_REPLY)
          && (IpTxReqDescrPtr->MessageType != TCPIP_IPV4_ICMP_TYPE_ECHO_REQ)))
  {
    /*
     *As per Section 6 of IETF Rfc 4301, Icmp error messages should not be processed by IpSec
     * as non protected devices may send such packets
     * Icmp Echo packets must be authenticated
     */
    /* #20 For icmp traffic, do not authenticate error messages */
    IpTxReqDescrPtr->IpSecPolicy = TCPIP_IPSEC_POLICY_BYPASS;
  }
  else
  {
    /* #30 For all other packets, look up the SPD for the matching security policy */
    TcpIp_SpdEntryIterType  selectedSpdEntryIdx;
    IpTxReqDescrPtr->IpSecPolicy = TcpIp_IpSec_VLookupSecurityPolicy(IpV4CtrlIdx, IpTxReqDescrPtr->XptProtocol, RemSock, locSock, &selectedSpdEntryIdx);
    /* Check that a existing SPD is selected and get the header type of if */
    if (selectedSpdEntryIdx != TcpIp_GetSizeOfSpdEntry())
    {
      ipsecHdrType = TcpIp_GetIpSecHdrTypeOfSpdEntry(selectedSpdEntryIdx);
    }
  }

  /* #40 If SPD Callout is configured, invoke the callout function, and update the IpSec Policy if overwritten */
  ipSecPolicy = IpTxReqDescrPtr->IpSecPolicy;
  TcpIp_IpSec_VInvokeSpdCallout(ipSecConfigSetIdx, &ipSecPolicy, IpTxReqDescrPtr->XptProtocol, RemSock, locSock);
  if (ipSecPolicy != IpTxReqDescrPtr->IpSecPolicy)
  {
    IpTxReqDescrPtr->IpSecPolicy = ipSecPolicy;
  }

  /* #50 If Policy is PROTECT or OPTIONAL, lookup for matching SA Entry for outbound packet */
  if (   (IpTxReqDescrPtr->IpSecPolicy == TCPIP_IPSEC_POLICY_PROTECT)
      || (IpTxReqDescrPtr->IpSecPolicy == TCPIP_IPSEC_POLICY_OPTIONAL))
  {
    saEntryIdx = TcpIp_IpSec_VLookupSelectorsInTxSaEntry(IpV4CtrlIdx, ipsecHdrType, IpTxReqDescrPtr->XptProtocol, RemSock, locSock);
  }

  /* #60 Update the Tx request descriptor parameters with updated values */
  TcpIp_IpSec_VUpdateTxDescriptorForAuthentication(IpTxReqDescrPtr->IpSecPolicy, saEntryIdx, IpTxReqDescrPtr, &eventId);

  /* ----- Audit Event --------------------------------------- */
  /* #70 Invoke audit event callout function if any event was detected */
  if (eventId == TCPIP_IPSEC_NO_EVENT)
  {
    /* Processing was successful, update retVal to E_OK */
    retVal = E_OK;
  }
  else
  {
    /* Processing was successful, retVal remains E_NOT_OK */
    TcpIp_IpSecAuditEventCalloutFunctionType auditEventCalloutFunctionPtr = TcpIp_GetAuditEventCalloutFunctionOfIpSecConfigSet(ipSecConfigSetIdx);

    if (auditEventCalloutFunctionPtr != NULL_PTR)
    {
      TcpIp_SockAddrBaseType rSock, lSock;                                                                              /* PRQA S 0759 */ /* MD_MSR_18.4 */
      rSock.SockAddrIn = RemSock;
      lSock.SockAddrIn = locSock;

      auditEventCalloutFunctionPtr(TCPIP_IPSEC_SPI_ANY, TCPIP_MAX_SEQNUMLOFSAENTRY, (TcpIp_SockAddrType *)&rSock, (TcpIp_SockAddrType *)&lSock, eventId); /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
    }
  }

  return retVal;
} /* TcpIp_IpSec_PreparePacketforAuthentication() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VLookupSelectorsInTxSaEntry
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
TCPIP_LOCAL FUNC(TcpIp_SaEntryIterType, TCPIP_CODE) TcpIp_IpSec_VLookupSelectorsInTxSaEntry(
  TcpIp_IpV4CtrlIterType     IpV4CtrlIdx,
  TcpIp_IpSecHeaderType      IpsecHdrType,
  TcpIp_IpProtocolType       XptProtocol,
  IpBase_SockAddrInType      RemSock,
  IpBase_SockAddrInType      LocSock)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SaEntryIterType ipSaEntryIdx;
  TcpIp_SaEntryIterType maxPrioSaEntryIdx = 0;
  uint32                maxPrioFound = 0;
  boolean               foundMatchingEntry = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Traverse through all SA Entries for outbound traffic */
  for (ipSaEntryIdx = TcpIp_GetSaEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx); ipSaEntryIdx < TcpIp_GetSaEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx); ipSaEntryIdx++)
  {
    /*@ assert ipSaEntryIdx < TcpIp_GetSizeOfSaEntry(); */                                                              /* VCA_TCPIP_CSL03_START_END */
    /* #20 Check if the upper layer protocol and IpSec protocol is supported by the SA entry */
    if (   (TcpIp_GetDirectionOfSaEntry(ipSaEntryIdx) == TCPIP_IPSEC_TRAFFIC_OUTBOUND)                                  /* PRQA S 3415 4 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
        && (TcpIp_GetIpSecHdrTypeOfSaEntry(ipSaEntryIdx) == IpsecHdrType)
        && (   (TcpIp_GetXptProtOfSaEntry(ipSaEntryIdx) == XptProtocol)
            || (TcpIp_GetXptProtOfSaEntry(ipSaEntryIdx) == TCPIP_PROTOCOL_ANY))
        /* Check if the SA entry is in valid state */
        && (TcpIp_GetStateOfSaEntry(ipSaEntryIdx) == TCPIP_IPSEC_STATUS_TX_RX))
    {
      /* #30 Check if the selectors match to that of the SA entry */
      if ((TcpIp_IpSec_VMatchSaSelectors(ipSaEntryIdx, XptProtocol, RemSock, LocSock) == TRUE)
        /* Check if this entry has higher priority than previously found entry */
        && (TcpIp_GetPriorityOfSaEntry(ipSaEntryIdx) >= maxPrioFound))
      {
        /* Matching entry found */
        foundMatchingEntry = TRUE;
        maxPrioSaEntryIdx = ipSaEntryIdx;
        maxPrioFound = TcpIp_GetPriorityOfSaEntry(ipSaEntryIdx);
      }
    }
  }

  if (foundMatchingEntry == TRUE)
  {
    /*@ assert maxPrioSaEntryIdx < TcpIp_GetSizeOfSaEntry(); */                                                         /* VCA_TCPIP_CSL03_START_END */
    TcpIp_SetInUseFlagOfSaEntry(maxPrioSaEntryIdx, TCPIP_IPSEC_SAENTRY_INUSE);
  }
  /* #40 If no matching SA was found, set the index to invalid else set SA to In Use */
  else
  {
    maxPrioSaEntryIdx = TcpIp_GetSizeOfSaEntry();
  }

  return maxPrioSaEntryIdx;
} /* TcpIp_IpSec_VLookupSelectorsInTxSaEntry() */


/**********************************************************************************************************************
 *  TcpIp_IpSec_VAntiReplayWindowVerify
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
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VAntiReplayWindowVerify(
  TcpIp_SaEntryIterType         SaEntryIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType  retVal;

  /* ----- Implementation ----------------------------------------------- */
   /* Rfc4302 Below implementation is based on the pseudo code from
    * Appendix B.23
    * integrity check is performed in another function call
    * and anti replay window is updated seperately
    */
  /* #10 Case A. If upper bound window is greater than lower bound window */
  if (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) >= (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u))
  {
    if (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) >= (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) - (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u)))
    {
      TcpIp_SetSeqNumHOfSaEntry(SaEntryIdx, TcpIp_GetUWndHOfSaEntry(SaEntryIdx));

      if (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) <= TcpIp_GetUWndLOfSaEntry(SaEntryIdx))
      {
        /* #20 If seq num received which is inside anti replay window check if the Sl has already been received */
        /* Bit to be checked is (Sl - Bl)- 1 => (Sl - (Tl - (W - 1)) - 1 ) */
        uint32 chkBit = (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) - (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) - (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u))) - 1u;
        if (TcpIp_IpSec_VReplayCheck(SaEntryIdx, chkBit) == E_OK)
        {
          /* Packet shall be authenticated */
          /* Anti replay window shall be updated if packet authenticates successfully */
          retVal = E_OK;
        }
        else
        {
          /* Packet is replay attack */
          retVal = E_NOT_OK;
        }
      }
      else
      {
        /* #30 Anti replay window shall be updated if packet authenticates successfully */
        retVal = E_OK;
      }
    }
    else
    {
      /* #40 If seq num received is after anti replay window and upper windows has overflown, update the upper window bound to new seq num */
      /*
       * This condition is also hit if a packet with sequence number much less than the lower bound of the anti replay window
       * is received. In this case the anti replay window will return E_OK, but the packet will be discarded during authentication
       */
      TcpIp_SetSeqNumHOfSaEntry(SaEntryIdx, (TcpIp_GetUWndHOfSaEntry(SaEntryIdx) + 1u));

      /* Anti replay window shall be updated if packet authenticates successfully */
      retVal = E_OK;
    }
  }
  /* #50 Case B. If upper bound window is less than lower bound window (due to ESN) */
  else
  {
    if (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) >= (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) - (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u)))
    {
      uint32 chkBit;
      TcpIp_SetSeqNumHOfSaEntry(SaEntryIdx, (TcpIp_GetUWndHOfSaEntry(SaEntryIdx) - 1u));

      /* #60 If seq num received which is inside anti replay window check if the Sl has already been received */
      /* Bit to be checked is (Sl - Bl)- 1 => (Sl - (Tl - (W - 1)) - 1 )*/
      chkBit = (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) - (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) - (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u))) - 1u;

      /* #70 Perform replay check */
      if (TcpIp_IpSec_VReplayCheck(SaEntryIdx, chkBit) == E_OK)
      {
        /* Packet shall be authenticated */
        /* Anti replay window shall be updated if packet authenticates successfully */
        retVal = E_OK;
      }
      else
      {
        /* Packet is replay attack */
        retVal = E_NOT_OK;
      }
    }
    else
    {
      TcpIp_SetSeqNumHOfSaEntry(SaEntryIdx, TcpIp_GetUWndHOfSaEntry(SaEntryIdx));

      if (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) <= TcpIp_GetUWndLOfSaEntry(SaEntryIdx))
      {
        /* If seq num received which is inside anti replay window check if the Sl has already been received */
        /* Bit to be checked is (Sl - Bl)- 1 => (Sl - (Tl - (W - 1)) - 1 )*/
        uint32 chkBit = (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) - (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) - (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u))) - 1u;
        /* replay check */
        if (TcpIp_IpSec_VReplayCheck(SaEntryIdx, chkBit) == E_OK)
        {
          /* Packet shall be authenticated */
          /* Anti replay window shall be updated if packet authenticates successfully */
          retVal = E_OK;
        }
        else
        {
          /* Packet is replay attack */
          retVal = E_NOT_OK;
        }
      }
      else
      {
        /* #80 If seq num received is after anti replay window, update the upper window bound to new seq num */
        /* Packet shall be authenticated */
        /* Anti replay window shall be updated if packet authenticates successfully */
        retVal = E_OK;
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VReplayCheck
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VReplayCheck(
  TcpIp_SaEntryIterType         SaEntryIdx,
  uint32                        ChkBit)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check for the bit in the bitstream */
  if (ChkBit < TCPIP_IPSEC_ANTIREPLAYWINDOW)
  {
    /* #20 If the bit has not been set in th bitstream, the packet has not been authenticated previously */
    if ((TcpIp_GetWndBitsOfSaEntry(SaEntryIdx) & (1uL << ChkBit)) == 0u)
    {
      retVal = E_OK;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VSetWndBit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VSetWndBit(
  TcpIp_SaEntryIterType         SaEntryIdx,
  uint32                        SetBit)
{
  uint32 val;
  /* #10 Set the bit in the bitsream */
  if (SetBit < TCPIP_IPSEC_ANTIREPLAYWINDOW)
  {
    val = TcpIp_GetWndBitsOfSaEntry(SaEntryIdx) | (1uL << SetBit);
    TcpIp_SetWndBitsOfSaEntry(SaEntryIdx, val);
  }
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VUpdateWndParams
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VUpdateWndParams(
  TcpIp_SaEntryIterType       SaEntryIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32                     shiftBits = (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) - TcpIp_GetUWndLOfSaEntry(SaEntryIdx));
  TcpIp_WndBitsOfSaEntryType bitstream = TcpIp_GetWndBitsOfSaEntry(SaEntryIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Shift the replay window bitstream right by shiftbits */
  bitstream = bitstream >> shiftBits;
  TcpIp_SetWndBitsOfSaEntry(SaEntryIdx, bitstream);

  /* #20 Set bit W -1 since the anti replay window has been updated */
  TcpIp_IpSec_VSetWndBit(SaEntryIdx, (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u));

  /* #30 Update value of Tl to Sl, the latest sequence number that has been authenticated */
  TcpIp_SetUWndLOfSaEntry(SaEntryIdx, TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx));
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VAntiReplayWindowUpdate
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
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VAntiReplayWindowUpdate(
  TcpIp_SaEntryIterType         SaEntryIdx)
{

  /* Rfc4302 Below implementation is based on the pseudo code from
    * Appendix B.23
    * Anti replay check is performed by TcpIp_IpSec_VAntiReplayWindowVerify()
    * integrity check is performed by TcpIp_IpSec_VAuthenticateRxPacket()
    * and anti replay window is updated by TcpIp_IpSec_VAntiReplayWindowUpdate()
    */
  /* #10 Update the anti replay window for the newly received sequnce number */
  /* #20 Case A. If uppper bound window is greater than lower bound window */
  if (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) >= (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u))
  {
    if (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) >= (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) - (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u)))
    {
      /* #30 If received sequence number lies inside replay window, set the bit in the bitsream */
      if (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) <= TcpIp_GetUWndLOfSaEntry(SaEntryIdx))
      {
         /* Bit to be set is (Sl - Bl)- 1 => (Sl - (Tl - (W - 1)) - 1 )*/
         uint32 setBit = (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) - (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) - (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u))) - 1u;
         TcpIp_IpSec_VSetWndBit(SaEntryIdx, setBit);
      }
      /* #40 If received sequence number lies outside replay window, update the window boundaries */
      else
      {
        TcpIp_IpSec_VUpdateWndParams(SaEntryIdx);
      }
    }
    else
    {
        /* #50 Received sequence number caused window overflow, increment upper 32-bit window */
        TcpIp_SetUWndHOfSaEntry(SaEntryIdx, (TcpIp_GetUWndHOfSaEntry(SaEntryIdx) + 1u));
        TcpIp_IpSec_VUpdateWndParams(SaEntryIdx);
    }
  }
  /* #60 Case B. If upper bound window is less than lower bound window (due to ESN) */
  else
  {
    if (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) >= (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) - (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u)))
    {
      /* #70 If received sequence number lies inside replay window, set the bit in the bitsream */
      /* Bit to be set is (Sl - Bl)- 1 => (Sl - (Tl - (W - 1)) - 1 )*/
      uint32 setBit = (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) - (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) - (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u))) - 1u;
      TcpIp_IpSec_VSetWndBit(SaEntryIdx, setBit);
    }
    else
    {
      /* If received sequence number lies inside replay window, set the bit in the bitsream */
      if (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) <= TcpIp_GetUWndLOfSaEntry(SaEntryIdx))
      {
        /* Bit to be set is (Sl - Bl)- 1 => (Sl - (Tl - (W - 1)) - 1 )*/
        uint32 setBit = (TcpIp_GetSeqNumLOfSaEntry(SaEntryIdx) - (TcpIp_GetUWndLOfSaEntry(SaEntryIdx) - (TCPIP_IPSEC_ANTIREPLAYWINDOW - 1u))) - 1u;
        TcpIp_IpSec_VSetWndBit(SaEntryIdx, setBit);
      }
      /* #80 If received sequence number lies outside replay window, update the window boundaries */
      else
      {
        TcpIp_IpSec_VUpdateWndParams(SaEntryIdx);
      }
    }
  }
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VIsIpSecHdrSupported
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VIsIpSecHdrSupported(
  TcpIp_SaEntryIterType SaEntryIdx,
  uint8                 IpSecProtocol
)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the IpSec header is supported for this SA Entry */
  switch (TcpIp_GetIpSecHdrTypeOfSaEntry(SaEntryIdx))
  {
  case TCPIP_IPSEC_HDR_ESP:
    {
      if (IpSecProtocol == TCPIP_PROTOCOL_ESP)
      {
        retVal = E_OK;
      }
    }
    break;
  case TCPIP_IPSEC_HDR_AH:
    {
      if (IpSecProtocol == TCPIP_PROTOCOL_AH)
      {
        retVal = E_OK;
      }
    }
    break;
  default:
    {
      retVal = E_NOT_OK;
    }
    break;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VLookupSelectorsInRxSaEntry
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
TCPIP_LOCAL FUNC(TcpIp_SaEntryIterType, TCPIP_CODE) TcpIp_IpSec_VLookupSelectorsInRxSaEntry(
  TcpIp_IpV4CtrlIterType               IpV4CtrlIdx,
  uint8                                IpSecProt,
  TcpIp_SpiOfSaEntryType               Spi)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SaEntryIterType ipSaEntryIdx;
  boolean               foundMatchingEntry = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /*
   * The look up is valid only for unicast packets received
   * as per rfc 4302 3.4.  Inbound Packet Processing
   * Support for multicast SA shall be added later
   */

  /* #10 Traverse through the SA entries for inbound traffic */
  for (ipSaEntryIdx = TcpIp_GetSaEntryStartIdxOfIpV4Ctrl(IpV4CtrlIdx); ipSaEntryIdx < TcpIp_GetSaEntryEndIdxOfIpV4Ctrl(IpV4CtrlIdx); ipSaEntryIdx++)
  {
    /*@ assert ipSaEntryIdx < TcpIp_GetSizeOfSaEntry(); */                                                              /* VCA_TCPIP_CSL03_START_END */
    /* #20 Check if the Spi matches any SA Entry and if IpSec header is supported */
    if (   (TcpIp_GetDirectionOfSaEntry(ipSaEntryIdx) == TCPIP_IPSEC_TRAFFIC_INBOUND)                                   /* PRQA S 3415 3 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
        && (TcpIp_GetSpiOfSaEntry(ipSaEntryIdx) == Spi)
        && (TcpIp_IpSec_VIsIpSecHdrSupported(ipSaEntryIdx, IpSecProt) == E_OK))
    {
      /* #30 Matching entry found, set the index */
      foundMatchingEntry = TRUE;
      break;
    }
  }

  if (foundMatchingEntry == TRUE)
  {
    TcpIp_SetInUseFlagOfSaEntry(ipSaEntryIdx, TCPIP_IPSEC_SAENTRY_INUSE);

    /* Update the state of the IPsec SA entry pair if packets has been received on it */
    if (TcpIp_GetStateOfSaEntry(ipSaEntryIdx) == TCPIP_IPSEC_STATUS_RX_ONLY)
    {
      TcpIp_SaEntryIterType pairedIpSaEntryIdx = TcpIp_GetSaEntryPairIdxOfSaEntry(ipSaEntryIdx);
      /*@ assert pairedIpSaEntryIdx < TcpIp_GetSizeOfSaEntry(); */                                                      /* VCA_TCPIP_IPSEC_SAENTRYPAIRIDX_FROM_SAENTRYIDX */

      TcpIp_SetStateOfSaEntry(ipSaEntryIdx, TCPIP_IPSEC_STATUS_TX_RX);
      TcpIp_SetStateOfSaEntry(pairedIpSaEntryIdx, TCPIP_IPSEC_STATUS_TX_RX);
    }
  }
  /* #40 If no matching SA was found, set the index to invalid, else set InUse flag */
  else
  {
    ipSaEntryIdx = TcpIp_GetSizeOfSaEntry();
  }

  return ipSaEntryIdx;
} /* TcpIp_IpSec_VLookupSelectorsInRxSaEntry */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VMatchSpdSelectors
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_IpSec_VMatchSpdSelectors(
  TcpIp_SpdEntryIterType  SpdEntryIdx,
  uint8                   Protocol,
  IpBase_SockAddrInType   RemSock,
  IpBase_SockAddrInType   LocSock)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean matchFound = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /*  #10 Match the input fields with SPD Entry selectors */
  if (   (TcpIp_IpSec_VSelectorIpAddressMatch(LocSock.sin_addr, TcpIp_GetLocIpV4AddrStartOfSpdEntry(SpdEntryIdx), TcpIp_GetLocIpV4AddrEndOfSpdEntry(SpdEntryIdx)) == TRUE)/* PRQA S 3415 2 */ /* MD_IPV4_Rule12.4_3415 */
      && (TcpIp_IpSec_VSelectorIpAddressMatch(RemSock.sin_addr, TcpIp_GetRemIpV4AddrStartOfSpdEntry(SpdEntryIdx), TcpIp_GetRemIpV4AddrEndOfSpdEntry(SpdEntryIdx)) == TRUE))
  {
    /* #20 If upper layer protocol is Icmp, then port number need not be checked */
    if (Protocol == TCPIP_PROTOCOL_ICMP)
    {
      matchFound = TRUE;
    }
    /* #30 Otherwise, then check port numbers */
    else
    {
      if (   (TcpIp_IpSec_VSelectorPortMatch(LocSock.sin_port, TcpIp_GetLocPortStartOfSpdEntry(SpdEntryIdx), TcpIp_GetLocPortEndOfSpdEntry(SpdEntryIdx)) == TRUE)/* PRQA S 3415 2 */ /* MD_IPV4_Rule12.4_3415 */
          && (TcpIp_IpSec_VSelectorPortMatch(RemSock.sin_port, TcpIp_GetRemPortStartOfSpdEntry(SpdEntryIdx), TcpIp_GetRemPortEndOfSpdEntry(SpdEntryIdx)) == TRUE))
      {
        matchFound = TRUE;
      }
    }
  }
  return matchFound;
} /* TcpIp_IpSec_VMatchSpdSelectors() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VMatchSaSelectors
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(boolean, TCPIP_CODE) TcpIp_IpSec_VMatchSaSelectors(
  TcpIp_SaEntryIterType   SaEntryIdx,
  uint8                   Protocol,
  IpBase_SockAddrInType   RemSock,
  IpBase_SockAddrInType   LocSock)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean matchFound = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /*  #10 Match the input fields with SA Entry selectors */
  if (   (TcpIp_IpSec_VSelectorIpAddressMatch(LocSock.sin_addr, TcpIp_GetLocIpV4AddrOfSaEntry(SaEntryIdx), IPV4_ADDR_ANY) == TRUE)/* PRQA S 3415 4 */ /* MD_IPV4_Rule12.4_3415 */
      && (TcpIp_IpSec_VSelectorIpAddressMatch(RemSock.sin_addr, TcpIp_GetRemIpV4AddrOfSaEntry(SaEntryIdx), IPV4_ADDR_ANY) == TRUE))
  {
    /* #20 If upper layer protocol is Icmp, then port number need not be checked */
    if (Protocol == TCPIP_PROTOCOL_ICMP)
    {
      matchFound = TRUE;
    }
    /* #30 Otherwise, then check port numbers */
    if(   (TcpIp_IpSec_VSelectorPortMatch(LocSock.sin_port, TcpIp_GetLocPortStartOfSaEntry(SaEntryIdx), TcpIp_GetLocPortEndOfSaEntry(SaEntryIdx)) == TRUE)/* PRQA S 3415 4 */ /* MD_IPV4_Rule12.4_3415 */
       && (TcpIp_IpSec_VSelectorPortMatch(RemSock.sin_port, TcpIp_GetRemPortStartOfSaEntry(SaEntryIdx), TcpIp_GetRemPortEndOfSaEntry(SaEntryIdx)) == TRUE))
    {
      matchFound = TRUE;
    }
  }

  return matchFound;
} /* TcpIp_IpSec_VMatchSaSelectors() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_VAuthenticateRxPacket
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
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VAuthenticateRxPacket(
  TcpIp_SaEntryIterType    SaEntryIdx,
  TCPIP_P2C(uint8)         IpPacketPtr,
  uint16                   IpTotalLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                 retVal = E_OK;
  TCPIP_P2C(uint8)               ipAuthHdrPtr = &IpPacketPtr[IPV4_IP_DEF_HEADER_LENGTH_BYTE];
  uint8                          initVector[TCPIP_IPSEC_GMAC_NONCE_LEN] = { 0u };
  TcpIp_RxProcessBufferIterType  icvPosInAhHdr = TCPIP_IPSEC_AH_HDR_OFS_AUTHDATA;
  uint16                         checkTotalLen = IpTotalLen;
  Crypto_VerifyResultType        verifyResult = CSM_E_NO_ERROR;
  uint8                          macBuffer[TCPIP_IPSEC_AH_MAX_ICV_LEN] = { 0 }; /* 64 Bytes for algorithms up to 512 Bit. */

  /* ----- Implementation ----------------------------------------------- */
  uint32                         verifyJobId = TcpIp_GetVerifyJobIdOfIntegrityJobPair(TcpIp_GetIntegrityJobPairIdxOfSaEntry(SaEntryIdx));

  if (TcpIp_IsEsnSupportOfSaEntry(SaEntryIdx) == TRUE)
  {
    checkTotalLen += TCPIP_IPSEC_ESN_LEN;
  }
  /* Check that msg fits in the buffer */
  if (checkTotalLen <= TcpIp_GetSizeOfRxProcessBuffer())
  {
    /*@ assert IpTotalLen <= TcpIp_GetSizeOfRxProcessBuffer(); */                                                       /* VCA_TCPIP_IPSEC_IP_TOTAL_LEN */

    /* #10 Copy the IP packet into local buffer and mute mutable fields */
    /* RxProcessBuffer is required because the rx-eth buffer is readonly */
    /* PRQA S 0315 2 */ /* MD_MSR_VStdLibCopy */
    IpBase_Copy(TcpIp_GetAddrRxProcessBuffer(0), IpPacketPtr, IpTotalLen);

    TcpIp_IpSec_VMuteIpHeaderFields(TcpIp_GetAddrRxProcessBuffer(0));

    if (TcpIp_IpSec_VIsGmacAlgorithmFamily(SaEntryIdx) == TRUE)
    {
      uint32 verifyKeyId = TcpIp_GetVerifyKeyIdOfIntegrityJobPair(TcpIp_GetIntegrityJobPairIdxOfSaEntry(SaEntryIdx));

      /* #20 Set Cipher IV value with the nonce value when GCM is used */
      TcpIp_IpSec_VObtainInitVectorFromAh(SaEntryIdx, ipAuthHdrPtr, &initVector[0]);

      retVal = TCPIP_CSM_KeyElementSet(verifyKeyId, CRYPTO_KE_CIPHER_IV, &initVector[0], TCPIP_IPSEC_GMAC_NONCE_LEN);
      retVal |= TCPIP_CSM_KeySetValid(verifyKeyId);

      /* #30 Shift the Icv offset to 8 bytes to account the IV when GCM is used */
      icvPosInAhHdr += TCPIP_IPSEC_GMAC_IV_LEN;
    }
    if (retVal == E_OK)
    {
      /* #40 Store ICV and mute Ah-Hdr part */
      if (TcpIp_GetSizeOfRxProcessBuffer() > (IPV4_IP_DEF_HEADER_LENGTH_BYTE + icvPosInAhHdr + TcpIp_GetIntegrityChkLenOfSaEntry(SaEntryIdx)))
      {
        /* PRQA S 0315 2 */ /* MD_MSR_VStdLibCopy */
        IpBase_Copy(&macBuffer[0], TcpIp_GetAddrRxProcessBuffer(IPV4_IP_DEF_HEADER_LENGTH_BYTE + icvPosInAhHdr),        /* VCA_TCPIP_IPSEC_IPBASE_WRITE_TO_TCPIP_LOCAL_ARRAY */
                  TcpIp_GetIntegrityChkLenOfSaEntry(SaEntryIdx));

        /* Set ICV to 0 */
        /* PRQA S 0314 2 */ /* MD_MSR_VStdLibCopy */
        IpBase_Fill((TCPIP_P2V(void))TcpIp_GetAddrRxProcessBuffer(IPV4_IP_DEF_HEADER_LENGTH_BYTE + icvPosInAhHdr),      /* VCA_TCPIP_RUNTIME_CHECK_PRESENT_IN_SAME_FUNCTION */
          0u, TcpIp_GetIntegrityChkLenOfSaEntry(SaEntryIdx));
      }
      else
      {
        retVal = E_NOT_OK;
      }

      /* #50 Add ESN if enabled */
      if ((TcpIp_IsEsnSupportOfSaEntry(SaEntryIdx) == TRUE) && (retVal == E_OK))
      {
        /*@ assert (IpTotalLen + TCPIP_IPSEC_ESN_LEN) <= TcpIp_GetSizeOfRxProcessBuffer(); */                           /* VCA_TCPIP_IPSEC_IP_TOTAL_LEN */
        TCPIP_PUT_UINT32(TcpIp_GetAddrRxProcessBuffer(IpTotalLen), 0u, TcpIp_GetSeqNumHOfSaEntry(SaEntryIdx));
      }

      /* #60 verify ICV */
      if (retVal == E_OK)
      {
        /*lint -e645 */
        retVal = TCPIP_CSM_MacVerify(verifyJobId, CRYPTO_OPERATIONMODE_SINGLECALL, TcpIp_GetAddrRxProcessBuffer(0), (uint32)checkTotalLen,
          macBuffer, ((uint32)TcpIp_GetIntegrityChkLenOfSaEntry(SaEntryIdx) * 8u), &verifyResult);
        /*lint +e645 */

        if (verifyResult != CSM_E_NO_ERROR)
        {
          retVal = E_NOT_OK;
        }
      }
    }
  }
  else
  {
    retVal = E_NOT_OK;
  }
  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VAuthenticateTxPacket
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
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_VAuthenticateTxPacket(
  TcpIp_SaEntryIterType             SaEntryIdx,
  TCPIP_P2V(uint8)                  IpPacketPtr,
  uint16                            IpTotalLen,
  uint8                             IpNextHeader)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType   retVal = E_OK;
  uint8            ipHdr[IPV4_IP_DEF_HEADER_LENGTH_BYTE];
  TCPIP_P2V(uint8) ipAuthHdrPtr = &IpPacketPtr[IPV4_IP_DEF_HEADER_LENGTH_BYTE];
  uint16           totalLen = IpTotalLen;
  TCPIP_P2V(uint8) icvPtr = &IpPacketPtr[IPV4_IP_DEF_HEADER_LENGTH_BYTE + TCPIP_IPSEC_AH_HDR_OFS_AUTHDATA];

  /* ----- Implementation ----------------------------------------------- */
  uint32           generateJobId = TcpIp_GetGenerateJobIdOfIntegrityJobPair(TcpIp_GetIntegrityJobPairIdxOfSaEntry(SaEntryIdx));
  uint32           icvLength = TcpIp_GetIntegrityChkLenOfSaEntry(SaEntryIdx);

  if (TcpIp_IsEsnSupportOfSaEntry(SaEntryIdx) == TRUE)
  {
    totalLen += TCPIP_IPSEC_ESN_LEN;
    TCPIP_PUT_UINT32(&IpPacketPtr[IpTotalLen], 0, TcpIp_GetSeqNumHOfSaEntry(SaEntryIdx));                               /* VCA_TCPIP_IPSEC_PUT_ESN_TO_RESERVED_MEMORY */
  }

  /* #10 Copy the IP header into local buffer and mute mutable fields */
  /* PRQA S 0315 1 */ /* MD_MSR_VStdLibCopy */
  IpBase_Copy(&ipHdr[0], &IpPacketPtr[0], IPV4_IP_DEF_HEADER_LENGTH_BYTE);
  TcpIp_IpSec_VMuteIpHeaderFields(&IpPacketPtr[0]);

  /* #20 Update the Authentication header into the Ip packet */
  TcpIp_IpSec_VBuildAuthHeader(SaEntryIdx, ipAuthHdrPtr, IpNextHeader);

  /* #30 If the algorithm family is Gmac, update the Cipher IV key element */
  if (TcpIp_IpSec_VIsGmacAlgorithmFamily(SaEntryIdx) == TRUE)
  {
    uint32 generateKeyId = TcpIp_GetGenerateKeyIdOfIntegrityJobPair(TcpIp_GetIntegrityJobPairIdxOfSaEntry(SaEntryIdx));

    /* #40 Set Cipher IV value with the nonce value */
    retVal = TCPIP_CSM_KeyElementSet(generateKeyId, CRYPTO_KE_CIPHER_IV,
                              (const uint8*)(TcpIp_GetAddrNonceOfSaEntry(SaEntryIdx)->Nonce), TCPIP_IPSEC_GMAC_NONCE_LEN);
    retVal |= TCPIP_CSM_KeySetValid(generateKeyId);

    /* #50 Shift the Icv offset to 8 bytes to account the IV */
    icvPtr = &icvPtr[TCPIP_IPSEC_GMAC_IV_LEN];
  }

  /* #60 Generate the Icv value for the packet */
  retVal |= TCPIP_CSM_MacGenerate(generateJobId, CRYPTO_OPERATIONMODE_SINGLECALL, &IpPacketPtr[0], totalLen, icvPtr, &icvLength); /* VCA_TCPIP_IPSEC_HDR_LENGTH */

  /* #70 Restore original IP-Header */
  /* PRQA S 0315 1 */ /* MD_MSR_VStdLibCopy */
  IpBase_Copy(&IpPacketPtr[0], &ipHdr[0], IPV4_IP_DEF_HEADER_LENGTH_BYTE);                                              /* VCA_TCPIP_REQUIREMENT_IS_SATISFIED_BY_FORMAL_SPECIFICATION */

  if (icvLength != TcpIp_GetIntegrityChkLenOfSaEntry(SaEntryIdx))
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_IpSec_VInvokeSpdCallout
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_VInvokeSpdCallout(
  TcpIp_IpSecConfigSetIdxOfIpV4CtrlType IpSecConfigSetIdx,
  TCPIP_P2V(TcpIp_IpSecPolicyType)      SecurityPolicyPtr,
  uint8                                 Protocol,
  IpBase_SockAddrInType                 RemSock,
  IpBase_SockAddrInType                 LocSock)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SockAddrBaseType rSock, lSock;                                                                                  /* PRQA S 0759 */ /* MD_MSR_18.4 */
  uint8 errorId = TCPIP_E_NO_ERROR;
  TcpIp_IpSecSpdCalloutFunctionType spdCalloutFunctionPtr = TcpIp_GetSpdCalloutFunctionOfIpSecConfigSet(IpSecConfigSetIdx);

  /* ----- Implementation ----------------------------------------------- */
  rSock.SockAddrIn = RemSock;
  lSock.SockAddrIn = LocSock;

  /* #10 Invoke the Spd callout function if it is configured */
  if (spdCalloutFunctionPtr != NULL_PTR)
  {
    spdCalloutFunctionPtr(SecurityPolicyPtr, Protocol, (TcpIp_SockAddrType *)&rSock, (TcpIp_SockAddrType *)&lSock);     /* PRQA S 0310 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */

    /* #20 If callout has written invalid policy, trigger DET error */
    if(*SecurityPolicyPtr >= TCPIP_IPSEC_POLICY_INVALID)
    {
      /* Trigger DET error if callout sets invalid policy */
      errorId = TCPIP_E_INV_ARG;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (TCPIP_DEV_ERROR_REPORT == STD_ON)
  if (errorId != TCPIP_E_NO_ERROR)
  {
    (void)Det_ReportError(TCPIP_MODULE_ID, TCPIP_VINSTANCE_ID, TCPIP_API_ID_IPSEC_SPD_CALLOUT, errorId);
  }
# else
  TCPIP_DUMMY_STATEMENT(errorId);                                                                                       /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif
} /* TcpIp_IpSec_VInvokeSpdCallout() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_ProcessRxPacket
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
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_ProcessRxPacket(
  TcpIp_IpV4CtrlIterType   IpV4CtrlIdx,
  TCPIP_P2C(uint8)         IpPacketPtr,
  uint16                   IpPayloadLen,
  TCPIP_P2V(uint8)         IpProtcolPtr,
  TCPIP_P2V(uint16)        IpSecHdrLenPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
 TcpIp_SizeOfIpSecConfigSetType ipSecConfigSetIdx = TcpIp_GetIpSecConfigSetIdxOfIpV4Ctrl(IpV4CtrlIdx);
  /*@ assert ipSecConfigSetIdx < TcpIp_GetSizeOfIpSecConfigSet(); */                                                    /* VCA_TCPIP_CSL03_IS_USEDOF */
  IpBase_SockAddrInType         ipRxSockAddrRem, ipRxSockAddrLoc;                                                       /* PRQA S 0759 */ /* MD_MSR_18.4 */
  Std_ReturnType                retVal = E_NOT_OK;
  uint16                        ipSecHdrLen = 0;
  TcpIp_IpSecEventType          eventId = TCPIP_IPSEC_NO_EVENT;
  TcpIp_SpiOfSaEntryType        spiRcvd = TCPIP_IPSEC_SPI_ANY;
  TcpIp_SeqNumLOfSaEntryType    seqNumRcvd = TCPIP_MAX_SEQNUMLOFSAENTRY;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Obtain the protocol header, source and destination port information from the Ip header */
  uint8 ipProtocol = TCPIP_GET_UINT8(IpPacketPtr, TCPIP_IPV4_HDR_OFS_PROT);
  IpBase_AddrInType srcAddr = TCPIP_GET_UINT32_RAW(IpPacketPtr, TCPIP_IPV4_HDR_OFS_SRC_ADDR);
  IpBase_AddrInType destAddr = TCPIP_GET_UINT32_RAW(IpPacketPtr, TCPIP_IPV4_HDR_OFS_DEST_ADDR);

  /* #20 Create socket addresses for source and destination */
  ipRxSockAddrRem.sin_family = IPBASE_AF_INET;
  ipRxSockAddrRem.sin_addr = srcAddr;
  ipRxSockAddrRem.sin_port = 0;  /* undefined */

  ipRxSockAddrLoc.sin_family = IPBASE_AF_INET;
  ipRxSockAddrLoc.sin_addr   = destAddr;
  ipRxSockAddrLoc.sin_port   = 0;  /* undefined */

  /* #30 Check for IpSec-AH header in the packet before processing the upper layer protocol header */
  /*
   * All inbound IP Traffic is processed as per section 5.2 [RFC 4301]
   */

  /* Step 3a of section 5.2 [RFC 4301] */
  /* #40 If authentication header is present, packet will be authenticated before forwarding to upper layer */
  if (ipProtocol == TCPIP_PROTOCOL_AH)
  {
    /* #50 Process the AH header in the packet */
    TcpIp_SaEntryIterType  ipSaEntryIdx = TcpIp_GetSizeOfSaEntry();

    /* #60 Obtain the SPI of the packet from the Authentication header */
    spiRcvd = TCPIP_GET_UINT32(IpPacketPtr, (IPV4_IP_DEF_HEADER_LENGTH_BYTE + TCPIP_IPSEC_AH_HDR_OFS_SPI));
    seqNumRcvd = TCPIP_GET_UINT32(IpPacketPtr, (IPV4_IP_DEF_HEADER_LENGTH_BYTE + TCPIP_IPSEC_AH_HDR_OFS_SEQNUM));

    /* #70 Lookup the SAD for matching SA entry */
    ipSaEntryIdx = TcpIp_IpSec_VLookupSelectorsInRxSaEntry(IpV4CtrlIdx, ipProtocol, spiRcvd);

    if (ipSaEntryIdx >= TcpIp_GetSizeOfSaEntry())
    {
      /* #80 If no valid SA entry found, processing ofIpSec header failed */
      eventId = TCPIP_IPSEC_EVENT_SAENTRY_NOTFOUND;
    }
    else
    {
      /* #90 If valid SA Entry found set InUse and process the IpSec header */
      eventId = TcpIp_IpSec_VSecureRxPacket(ipSaEntryIdx, IpPacketPtr, ipProtocol, IpPayloadLen);

      if (eventId == TCPIP_IPSEC_NO_EVENT)
      {
# if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
        /* Update Diagnostic parameters if packet was processed successfully */
        TcpIp_IncPktCntOfSaEntry(ipSaEntryIdx);
        TcpIp_SetLastPktPrcTimeOfSaEntry(ipSaEntryIdx, TcpIp_IpV4_GetCurrTimeMs());
# endif /* (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON) */

        /* IpSec authentication of packet was successful */
        ipProtocol = TCPIP_GET_UINT8(IpPacketPtr, (IPV4_IP_DEF_HEADER_LENGTH_BYTE + TCPIP_IPSEC_AH_HDR_OFS_NEXTHDR));

        /* Check if there is a nested authentication header */
        if (ipProtocol == IPV4_IP_PROTOCOL_AH)
        {
          /* Invalid IPsec header found, raise event */
          eventId = TCPIP_IPSEC_EVENT_INVALID_HDR;
        }

        /* #100 Add the length of the IpSec header processed in this iteration */
        ipSecHdrLen = TcpIp_IpSec_VGetIpSecHeaderLengthBytes(ipSaEntryIdx);
      }
      /* #110 Release InUse flag */
      TcpIp_SetInUseFlagOfSaEntry(ipSaEntryIdx, TCPIP_IPSEC_SAENTRY_IDLE);
    }
  }
  /* Step 3b of section 5.2 [RFC 4301] */
  /* #120 If no IpSec header exists, perform SPD lookup to see if packet can be processed with IpSec */
  else
  {
    /*
     * No IpSec header found, SPD lookup for the packet shall be done
     * to find matching Security policy
     */
    TcpIp_IpSecPolicyType                 ipSecRxPolicy = 0u;
    uint8                                 messageType   = 0u;

    /* #130 Before performing SPD lookup extract the port information if Tcp or Udp protocol */
    TcpIp_VReadPortFromProtocolHdr(IpPacketPtr, ipProtocol, IPV4_IP_DEF_HEADER_LENGTH_BYTE, &(ipRxSockAddrLoc.sin_port), &ipRxSockAddrRem.sin_port);

    messageType = (ipProtocol == TCPIP_PROTOCOL_ICMP) ? IpPacketPtr[IPV4_IP_DEF_HEADER_LENGTH_BYTE] : TCPIP_IPV4_ICMP_TYPE_RSVD;
    /*
     * As per Section 6 of IETF Rfc 4301,
     * Icmp error messages need not be processed by IpSec
     * as non protected devices may send such packets
     */
    if (   (ipProtocol == TCPIP_PROTOCOL_ICMP)
        && (   (messageType != TCPIP_IPV4_ICMP_TYPE_ECHO_REPLY)
            && (messageType != TCPIP_IPV4_ICMP_TYPE_ECHO_REQ)))
    {
      /* #140 Or message type in case of Icmp */
      /*
       * As per Section 6 of IETF Rfc 4301, Icmp error messages should not be processed by IpSec
       * as non protected devices may send such packets
       * Icmp Echo packets must be authenticated
       */
      ipSecRxPolicy = TCPIP_IPSEC_POLICY_BYPASS;
    }
    else
    {
      /* #150 Perform Security policy lookup */
      TcpIp_SpdEntryIterType  ipSpdEntryIdx = 0u;
      ipSecRxPolicy = TcpIp_IpSec_VLookupSecurityPolicy(IpV4CtrlIdx, ipProtocol, ipRxSockAddrRem, ipRxSockAddrLoc, &ipSpdEntryIdx);
    }

    /* #160 If SPD Callout is configured, invoke the callout function */
    TcpIp_IpSec_VInvokeSpdCallout(ipSecConfigSetIdx, &ipSecRxPolicy, ipProtocol, ipRxSockAddrRem, ipRxSockAddrLoc);

    /* #170 If policy is PROTECT and packet does not have IpSec header, */
    if (   (ipSecRxPolicy == TCPIP_IPSEC_POLICY_PROTECT)
    /*#180 Or if policy has been overwritten to DISCARD by callout, */
        || (ipSecRxPolicy == TCPIP_IPSEC_POLICY_DISCARD)
    /* #190 Or if invalid policy is configured, */
        || (ipSecRxPolicy >= TCPIP_IPSEC_POLICY_INVALID))
    {
      /* #200 Policy for the packet is invalid, IpSec processing failed */
      eventId = TCPIP_IPSEC_EVENT_DISCARD_POLICY;
    }
    /* #210 For BYPASS or OPTIONAL, packet can be without IpSec header. Forward packet */
    else
    {
      /* do nothing - continue */
    }
  }

  /* ----- Audit Event --------------------------------------- */
  if (eventId == TCPIP_IPSEC_NO_EVENT)
  {
    /* #220 Update the Ip protocol and total IpSec header length processed in the packet */
    retVal = E_OK;
    *IpSecHdrLenPtr = ipSecHdrLen;
    *IpProtcolPtr = ipProtocol;
  }
  else
  {
    TcpIp_IpSecAuditEventCalloutFunctionType auditEventCalloutFunctionPtr = TcpIp_GetAuditEventCalloutFunctionOfIpSecConfigSet(ipSecConfigSetIdx);

    /* Processing was successful, retVal remains E_NOT_OK */
    /* #230 Invoke audit event callout function if available */
    if (auditEventCalloutFunctionPtr != NULL_PTR)
    {
      TcpIp_SockAddrBaseType rSock;                                                                                     /* PRQA S 0759 */ /* MD_MSR_18.4 */
      TcpIp_SockAddrBaseType lSock;                                                                                     /* PRQA S 0759 */ /* MD_MSR_18.4 */
      rSock.SockAddrIn = ipRxSockAddrRem;
      lSock.SockAddrIn = ipRxSockAddrLoc;
      auditEventCalloutFunctionPtr(spiRcvd, seqNumRcvd, (TcpIp_SockAddrType *)&rSock, (TcpIp_SockAddrType *)&lSock, eventId); /* PRQA S 0310, 3305 */ /* MD_TCPIP_Rule11.3_0310_3305_SockAddrBaseType */
    }
  }

  return retVal;
} /* TcpIp_IpSec_ProcessRxPacket() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_GetLocSocketAddr
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(void, TCPIP_CODE) TcpIp_IpSec_GetLocSocketAddr(
  TcpIp_IpV4CtrlIterType            IpV4CtrlIdx,
  TcpIp_SocketDynIterType           IpV4SocketIdx,
  TCPIP_P2V(IpBase_SockAddrInType)  LocSockPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  IpBase_SockAddrInType locSock = {0u,0u,0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check the type of socket and extract the local socket information */
  if (   (TcpIp_IsIpV4SocketDynDhcpUsedOfIpV4General(TCPIP_IPV4GENERAL_IDX) == TRUE)                                    /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
      && (IpV4SocketIdx == TcpIp_GetIpV4SocketDynDhcpIdxOfIpV4General(TCPIP_IPV4GENERAL_IDX)))
  {
    /* If the socket is for DhcpV4, retrieve local socket address from TcpIp_IpV4SockDyn */
    locSock = TcpIp_GetAddrLocSockOfIpV4SocketDyn(IpV4SocketIdx)->SockAddrIn;
  }
  else if ((TcpIp_IsIpV4SocketDynTcpRstUsedOfIpV4General(TCPIP_IPV4GENERAL_IDX) == TRUE)                                /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
      && (IpV4SocketIdx == TcpIp_GetIpV4SocketDynTcpRstIdxOfIpV4General(TCPIP_IPV4GENERAL_IDX)))
  {
    /* If the socket is for TCP reset, retrieve local socket address from TcpIp_IpV4SockDyn */
    locSock = TcpIp_GetAddrLocSockOfIpV4SocketDyn(IpV4SocketIdx)->SockAddrIn;
  }
  else
  {
    /* the socket is for Tcp/Udp, retrieve the local socket from TcpIp_SockDyn */
    locSock = TcpIp_GetAddrLocSockOfSocketDyn(IpV4SocketIdx)->SockAddrIn;

    /* ESCAN00108044, next 2 lines, Impl 14.02.01 */
    /* Retrieve the local Ip address from the ipv4Ctrl struct */
    locSock.sin_addr = TcpIp_GetActiveNetAddrOfIpV4CtrlDyn(IpV4CtrlIdx);
  }

   /* return the socket address */
  *LocSockPtr = locSock;

} /* TcpIp_IpSec_GetLocSocketAddr() */

# if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VCalculateSecsElapsed
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
TCPIP_LOCAL FUNC(uint16, TCPIP_CODE) TcpIp_VCalculateSecsElapsed(
  TcpIp_SaEntryIterType  SaEntryIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 currTimeMs = TcpIp_IpV4_GetCurrTimeMs();
  uint32 lastEventMs = TcpIp_GetLastPktPrcTimeOfSaEntry(SaEntryIdx);
  uint32 msElapsed_u32, secsElapsed_u32;
  uint16 secsElapsed_u16;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the time elapsed, considering overflow */
  if (currTimeMs > lastEventMs)
  {
    msElapsed_u32 = currTimeMs - lastEventMs;
  }
  else
  {
    msElapsed_u32 = TCPIP_IPV4_TIME_MAX - lastEventMs + currTimeMs;
  }

  /* #20 Convert from milliseconds to seconds */
  secsElapsed_u32 = msElapsed_u32 / TCPIP_IPV4_TIME_MS;

  /* #30 Handle overflow of seconds elapsed */
  if (secsElapsed_u32 > TCPIP_IPSEC_SECSELAPSED)
  {
    secsElapsed_u16 = TCPIP_IPSEC_SECSELAPSED;
  }
  else
  {
    secsElapsed_u16 = (uint16)secsElapsed_u32;
  }

  return secsElapsed_u16;
} /* TcpIp_VCalculateSecsElapsed() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_GetNumSaEntryPairs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(uint32, TCPIP_CODE)  TcpIp_IpSec_GetNumSaEntryPairs(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SaEntryIterType  ipSaEntryIdx;
  uint32 numSaEntryPairs = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Traverse through the SAD */
  for (ipSaEntryIdx = 0; ipSaEntryIdx < TcpIp_GetSizeOfSaEntry(); ipSaEntryIdx++)
  {
    /* #20 Count the number of inbound SA entries with
     *     valid IpSec protocol and a valid SA entry pair */
    if (   (TcpIp_GetIpSecHdrTypeOfSaEntry(ipSaEntryIdx) != TCPIP_IPSEC_HDR_NONE)                                       /* PRQA S 3415 3 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
        && (TcpIp_GetDirectionOfSaEntry(ipSaEntryIdx) == TCPIP_IPSEC_TRAFFIC_INBOUND)
        && (TcpIp_GetSaEntryPairIdxOfSaEntry(ipSaEntryIdx) < TcpIp_GetSizeOfSaEntry()))
    {
      numSaEntryPairs++;
    }
  }

  return numSaEntryPairs;
} /* TcpIp_IpSec_GetNumSaEntryPairs() */

/**********************************************************************************************************************
 *  TcpIp_IpSec_GetSaEntryPairsStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_IpSec_GetSaEntryPairsStatus(
  TCPIP_P2V(TcpIp_IpSecSaStatusType)  SaEntryStatusPtr,
  uint32                              NumSaEntryPairs)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32  entryCnt = 0;
  Std_ReturnType  retVal   = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if new entries have been added since last call */
  if (NumSaEntryPairs == TcpIp_IpSec_GetNumSaEntryPairs())
  {
    /* No new entries have been created since last call */
    retVal = E_OK;

    /* Lock interrupts so that entries are not modified during reading */
    TCPIP_VENTER_CRITICAL_SECTION();

    /* #20 Traverse through the SAD */
    {
      TcpIp_SaEntryIterType  saEntryIdx;
      for (saEntryIdx = 0; saEntryIdx < TcpIp_GetSizeOfSaEntry(); saEntryIdx++)
      {
        /* #30 Retrieve statuses of requested Sa entry pairs */
        if ((entryCnt < NumSaEntryPairs)                                                                                /* PRQA S 3415 4 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
          && (TcpIp_GetIpSecHdrTypeOfSaEntry(saEntryIdx) != TCPIP_IPSEC_HDR_NONE)
          && (TcpIp_GetDirectionOfSaEntry(saEntryIdx) == TCPIP_IPSEC_TRAFFIC_INBOUND)
          && (TcpIp_GetSaEntryPairIdxOfSaEntry(saEntryIdx) < TcpIp_GetSizeOfSaEntry()))
        {
          TCPIP_P2V(TcpIp_IpSecSaStatusType) entryPtr = &SaEntryStatusPtr[entryCnt];
          uint16 secsSinceLastRx                      = TcpIp_VCalculateSecsElapsed(saEntryIdx);
          TcpIp_SaEntryIterType pairSaEntryIdx        = TcpIp_GetSaEntryPairIdxOfSaEntry(saEntryIdx);
          /*@ assert pairSaEntryIdx < TcpIp_GetSizeOfSaEntry(); */                                                      /* VCA_TCPIP_CSL01 */
          uint16 secsSinceLastTx                      = TcpIp_VCalculateSecsElapsed(pairSaEntryIdx);

          /* Fill the Sa entry pair status */
          entryPtr->InboundSpi = TcpIp_GetSpiOfSaEntry(saEntryIdx);
          entryPtr->OutboundSpi = TcpIp_GetSpiOfSaEntry(pairSaEntryIdx);
          entryPtr->IncomingPktCnt = TcpIp_GetPktCntOfSaEntry(saEntryIdx);
          entryPtr->OutgoingPktCnt = TcpIp_GetPktCntOfSaEntry(pairSaEntryIdx);
          entryPtr->SecsSinceLastRx = secsSinceLastRx;
          entryPtr->SecsSinceLastTx = secsSinceLastTx;
          entryPtr->IpsecProtocol = TcpIp_GetIpSecHdrTypeOfSaEntry(saEntryIdx);

          /* Increment the entry counter */
          entryCnt++;
        }
      }
    }

    /* Exit critical section */
    TCPIP_VLEAVE_CRITICAL_SECTION();
  }

  return retVal;
} /* TcpIp_IpSec_GetSaEntryPairsStatus() */
# endif /* (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON) */

# define TCPIP_STOP_SEC_CODE
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/*
   MICROSAR global MISRA deviations:
   - see file 'WI_MISRAC2004_PES.pdf'

   Justification for module-specific MISRA deviations:

   module global deviations -> see file TcpIp.c

   IpSec-specific deviations:

*/

/* VCA_JUSTIFICATION_START

  \ID VCA_TCPIP_IPSEC_SAENTRYPAIR_IDX_CHECKED
    \DESCRIPTION       The function TcpIp_IpSec_AddSaEntryPair() writes into the CSL array at a position that is given
                       by the index parameter ipSaEntryIdxIn/ipSaEntrIydxOut which is set in TcpIp_IpSec_VAddSaEntry().
                       The write is only performed if TcpIp_IpSec_VAddSaEntry() returns E_OK.
                       The function is called in the context of creating an Sa entry pair.
    \COUNTERMEASURE \N 1. Before the write operations, TcpIp_IpSec_VAddSaEntry() invokes
                       TcpIp_IpSec_VFindUnusedSaEntry() which returns a valid unused SaEntryIdx if available.
                       2. TcpIp_IpSec_VAddSaEntry() returns E_OK only if an unused SaEntryIdx is found and therefore,
                       ipSaEntryIdxIn / ipSaEntryIdxOut is always valid.

  \ID VCA_TCPIP_IPSEC_SAENTRYIDX_FROM_ADDSAENTRY
    \DESCRIPTION       The function writes into the CSL array at a position that is given by the index parameter.
                       The function is called when a new saEntry is added, from TcpIp_IpSec_VAddSaEntry().
    \COUNTERMEASURE \N In TcpIp_IpSec_VAddSaEntry(), index parameter SaEntryIdx is stored by
                       TcpIp_IpSec_VFindUnusedSaEntry(), which ensures that SaEntryIdx is valid if function
                       returns E_OK.

  \ID VCA_TCPIP_IPSEC_IP_TOTAL_LEN
    \DESCRIPTION       IpTotalLen is a function argument, used to copy the length of data to a valid memory location.
                       If TcpIp_IsEsnSupportOfSaEntry() is enabled, extra TCPIP_IPSEC_ESN_LEN of 4 bytes is considered
                       for length check.
    \COUNTERMEASURE \R Local function makes it sure using runtime check:
                       (IpTotalLen || (IpTotalLen + TCPIP_IPSEC_ESN_LEN)) <= TcpIp_GetSizeOfRxProcessBuffer()
                       before using it.

  \ID VCA_TCPIP_IPSEC_SAENTRYPAIRIDX_FROM_SAENTRYIDX
    \DESCRIPTION       SaEntryPairIdx is derived from the SaEntryIdx.
    \COUNTERMEASURE \N SaEntryPairIdx is only derived if SaEntryIdx is < TcpIp_GetSizeOfSaEntry(). It is checked inside
                       the local function using runtime check.
                       If there exists a valid SaEntry there is always a valid SaEntryPair. (Ensured by
                       TcpIp_IpSec_AddSaEntryPair().)

  \ID VCA_TCPIP_IPSEC_JOBPAIR_IDX_FROM_SAENTRYIDX
    \DESCRIPTION       IntegrityJobPairIdx is derived from local variable SaEntryIdx.
    \COUNTERMEASURE \N It is checked in local function that IntegrityJobPairIdx is only derived if SaEntryIdx and
                       SaEntryPairIdx both are < TcpIp_GetSizeOfSaEntry().
                       It is ensured within function TcpIp_IpSec_AddSaEntryPair() that a valid
                       IntegrityJobPairIdxOfSaEntry is set during creation of SaEntries.
                       Hence, if the SaEntries are valid, the IntegrityJobPairs are also valid.

  \ID VCA_TCPIP_IPSEC_IP_PKT_PTR_CHECKED
    \DESCRIPTION       Buffer IpPacketPtr passed by caller is passed to the subroutine which may write into it.
                       The function is only called in IpV4 TX context from IpV4_Ip_VTransmitFragments()
    \COUNTERMEASURE \S 1. Before IpV4_Ip_VTransmitFragments() is called, IpV4_Ip_ProvideTxBuffer() is always
                          invoked to obtain a valid Ethernet buffer of required size. For IpSec-AH, 40 additional
                          bytes are requested.
                       2. IpV4_Ip_ProvideTxBuffer() invokes IpV4_Ip_VFragmentProvideTxBuffers() which in turn
                          invokes EthIf_ProvideTxBuffer().
                       3. SMI-242896 ensures that a valid Ethernet buffer index with atleast requested size
                          is returned by EthIf_ProvideTxBuffer().
                       4. When a Ethernet buffer is successfully returned, EthBufPtrOfBuf2TxReqMap() is updated
                          in IpV4_Ip_VFragmentProvideTxBuffers() to point to the returned buffers.
                       5. Therefore, in IpV4_Ip_VTransmitFragments() it is always guaranteed that there is
                          enough space in the pointer to store an IP packet with IpSec header.

  \ID VCA_TCPIP_IPSEC_IPBASECOPY_IPBUF_FIXED_ARRAY
    \DESCRIPTION       IpAuthHdrPtr is used to copy TCPIP_IPSEC_GMAC_IV_LEN bytes to it.
                       This function is called in the IPv4 Tx context.
    \COUNTERMEASURE \N IpAuthHdrPtr is passed by caller and calculated from IpPacketPtr. It is ensured that IpPacketPtr
                       has enough memory by VCA_TCPIP_IPSEC_IP_PKT_PTR_CHECKED.

  \ID VCA_TCPIP_IPSEC_PUT_ESN_TO_RESERVED_MEMORY
    \DESCRIPTION       The ESN number is added to the end of the packet if ESN_support is enabled. Because this is a not
                       transmitted part of the packet, the IP-Packet-Length does not contain this 4 extra bytes.
    \COUNTERMEASURE \N In case of ESN-support, 4 more bytes are added to ReservedTrailer in
                       TcpIp_IpSec_VUpdateTxDescriptorForAuthentication. ESN support can not be enabled or disabled
                       after a SA is created as long it is a valid entry. Hence, it is ensured that the requested buffer
                       contains this additional 4 bytes.

  \ID VCA_TCPIP_IPSEC_IPBASE_WRITE_TO_TCPIP_LOCAL_ARRAY
    \DESCRIPTION       Local array macBuffer[] is used to temporary store the ICV.
    \COUNTERMEASURE \M The size of the local array (TCPIP_IPSEC_AH_MAX_ICV_LEN) is checked to be smaller as the
                       configured IntegrityChkLenOfSaEntry.
                       MSSV check: CM_TCPIP_IPSEC_ICV_LENGTH

  \ID VCA_TCPIP_IPSEC_HDR_LENGTH
    \DESCRIPTION       The length of the buffer is at least big enough to store the complete IPsec header.
    \COUNTERMEASURE \N The required size for the IPsec header is calculated in
                       TcpIp_IpSec_PreparePacketforAuthentication().
                       As result, a matching buffer is requested by the call of IpV4_Ip_VFragmentProvideTxBuffers().
                       See: VCA_TCPIP_UPDATE_PTR

  VCA_JUSTIFICATION_END */

#endif /* (TCPIP_SUPPORT_IPSEC == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: TcpIp_IpSec.c
 *********************************************************************************************************************/
