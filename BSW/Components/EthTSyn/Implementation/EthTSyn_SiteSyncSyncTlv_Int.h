/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  EthTSyn_SiteSyncSyncTlv_Int.h
 *         \unit  SiteSyncSyncTlv
 *        \brief  EthTSyn internal header file of the SiteSyncSyncTLv unit.
 *      \details  Interface and type definitions of the SiteSyncSyncTlv unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_SITE_SYNC_SYNC_TLV_INT_H)
# define ETHTSYN_SITE_SYNC_SYNC_TLV_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"
# include "EthTSyn_GeneratedCfgAccess.h"

# if ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) ) /* COV_MSR_UT_OPTIONAL_UNIT */
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) /* COV_MSR_UT_OPTIONAL_UNIT */
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
#   define ETHTSYN_START_SEC_CODE
#   include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTlv_GetTimeTlvInfo
 *********************************************************************************************************************/
/*! \brief          Extracts time secured TLV information from a FollowUp message
 *  \details        -
 *  \param[in]      CrcHndlIdx       Index of the CrcHndl
 *  \param[in]      FupMsgHdrPtr     Header of the FollowUp message
 *  \param[in]      FupPayloadPtr    Pointer to the serialized payload of the FollowUp message
 *  \param[in]      KeepSrcPortId    Flag to indicate if source port identity is kept upon forwarding
 *  \param[in]      AlwaysUpdateCrc  Flag to indicate if time secured TLV CRC needs to be updated always
 *  \return         The extracted time secured TLV information consisting of:
 *                    RelOfs            - Relative (from payload) to start of the time secured TLV (0 when no
 *                                        time secured TLV is included)
 *                    CrcUpdateRequired - Indication if time secured TLV CRC needs to be update on transmission or not
 *                    CrcValid          - Indication if the received time secured TLV CRC is valid or not
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires FupMsgHdrPtr != NULL_PTR;
 *    requires FupPayloadPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(EthTSyn_TimeTlvInfoType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTlv_GetTimeTlvInfo(
                  EthTSyn_CrcHndl_IdxType CrcHndlIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)     FupMsgHdrPtr,
  ETHTSYN_P2CONST(uint8)                  FupPayloadPtr,
                  boolean                 KeepSrcPortId,
                  boolean                 AlwaysUpdateCrc);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTlv_UpdateTimeTlvCrc
 *********************************************************************************************************************/
/*! \brief          Updates the CRC of a time secured TLV
 *  \details        -
 *  \param[in]      CrcHndlIdx            Index of the CrcHndl
 *  \param[in,out]  FupBufferPtr           In: The serialized FollowUp
 *                                        Out: The serialized FollowUp with updated time secured TLV
 *  \param[in]      TimeSecuredTlvRelOfs  Offset to the start of the time secured TLV relative to the FollowUp payload
 *  \param[in]      InvalidateCrc         Indicating if the updated CRC shall be invalidated
 *  \pre            -
 *  \note           Invalidation of the CRC is done by incrementing the value Time0Crc and Time1Crc by one
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires FupBufferPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTlv_UpdateTimeTlvCrc(
                EthTSyn_CrcHndl_IdxType CrcHndlIdx,
  ETHTSYN_P2VAR(uint8)                  FupBufferPtr,
                uint16                  TimeSecuredTlvRelOfs,
                boolean                 InvalidateCrc);

#   define ETHTSYN_STOP_SEC_CODE
#   include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */
# endif /* ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) ) */
#endif /* ETHTSYN_SITE_SYNC_SYNC_TLV_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SiteSyncSyncTlv_Int.h
 *********************************************************************************************************************/
