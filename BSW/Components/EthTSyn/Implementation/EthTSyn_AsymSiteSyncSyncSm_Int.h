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
/*!        \file  EthTSyn_AsymSiteSyncSyncSm_Int.h
 *         \unit  AsymSiteSyncSyncSm
 *        \brief  EthTSyn internal header file of the AsymSiteSyncSyncSm unit.
 *      \details  Interface and type definitions of the AsymSiteSyncSyncSm unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_ASYM_SITE_SYNC_SYNC_SM_INT_H)
# define ETHTSYN_ASYM_SITE_SYNC_SYNC_SM_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */

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
#  define ETHTSYN_START_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EthTSyn_AsymSiteSyncSyncSm_Init
 *********************************************************************************************************************/
/*! \brief      Initializes the AsymSiteSyncSyncSm unit.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_AsymSiteSyncSyncSm_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_AsymSiteSyncSyncSm_SyncRcvd
 *********************************************************************************************************************/
/*! \brief      Informs the AsymSiteSyncSyncSm about reception of a valid Sync.
 *  \details    -
 *  \param[in]  AsymSiteSyncSyncSmIdx  Index of the AsymSiteSyncSyncSm.
 *                                     [range: AsymSiteSyncSyncSmIdx < EthTSyn_GetSizeOfAsymSiteSyncSyncSm()]
 *  \param[in]  SyncSwtIngrTsMgmtPtr   Switch ingress timestamp of the received Sync message
 *  \param[in]  SyncAddInfoPtr         Additional information for the received Sync, i.e. number and list of
 *                                     Sync egress timestamps
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires AsymSiteSyncSyncSmIdx < EthTSyn_GetSizeOfAsymSiteSyncSyncSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_AsymSiteSyncSyncSm_SyncRcvd(
                  EthTSyn_AsymSiteSyncSyncSm_SmIdxType AsymSiteSyncSyncSmIdx,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)                  SyncSwtIngrTsMgmtPtr,
  ETHTSYN_P2CONST(EthSwt_VMgmtObjectAddInfoType)       SyncAddInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_AsymSiteSyncSyncSm_ProcRcvdFup
 *********************************************************************************************************************/
/*! \brief      Processing of a received FollowUp message.
 *  \details    Check if passed state machine is ready to receive a FollowUp message and optionally processes the
 *              message.
 *  \param[in]  AsymSiteSyncSyncSmIdx  Index of the AsymSiteSyncSyncSm.
 *                                     [range: AsymSiteSyncSyncSmIdx < EthTSyn_GetSizeOfAsymSiteSyncSyncSm()]
 *  \param[in]  PtpHdrPtr              De-serialized header of the received FollowUp message
 *  \param[in]  FupPayloadPtr          Pointer to the serialized payload of the received FollowUp message
 *  \pre        Must only be called when the corresponding Sync was received by the AsymSiteSyncSyncSm via
 *              EthTSyn_AsymSiteSyncSyncSm_SyncRcvd()
 *  \pre        Message length in Ptp header needs to be valid, i.e. PtpHdrPtr->MessageLength >= ETHTSYN_MSG_FUP_LEN
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires AsymSiteSyncSyncSmIdx < EthTSyn_GetSizeOfAsymSiteSyncSyncSm();
 *    requires PtpHdrPtr->MessageLength >= ETHTSYN_MSG_FUP_LEN;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_AsymSiteSyncSyncSm_ProcRcvdFup(
                  EthTSyn_AsymSiteSyncSyncSm_SmIdxType AsymSiteSyncSyncSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                  PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                               FupPayloadPtr);

/**********************************************************************************************************************
 *  EthTSyn_AsymSiteSyncSyncSm_CopyFupPayload
 *********************************************************************************************************************/
/*! \brief      Copies the Payload of the FollowUp message to a provided buffer (in Network-Byte-Order)
 *  \details    -
 *  \param[in]  AsymSiteSyncSyncSmIdx  Index of the AsymSiteSyncSyncSm.
 *                                     [range: AsymSiteSyncSyncSmIdx < EthTSyn_GetSizeOfAsymSiteSyncSyncSm()]
 *  \param[out] PayloadDestPtr         Pointer to a buffer with length PayloadDestLen. The FollowUp payload is copied
 *                                     here.
 *  \param[in]  PayloadDestLen         Length of the payload destination buffer
 *  \return     E_OK - Payload successfully copied to the buffer
 *  \return     E_NOT_OK - Failed to copy the payload to the buffer
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires AsymSiteSyncSyncSmIdx < EthTSyn_GetSizeOfAsymSiteSyncSyncSm();
 *    requires PayloadDestPtr != NULL_PTR;
 *  \endspec
 *  \note It is not guaranteed that the complete PayloadDest buffer is initialzed. Only the required part is set when
 *        E_OK is returned.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_AsymSiteSyncSyncSm_CopyFupPayload(
                EthTSyn_AsymSiteSyncSyncSm_SmIdxType AsymSiteSyncSyncSmIdx,
  ETHTSYN_P2VAR(uint8)                               PayloadDestPtr,
                uint16                               PayloadDestLen);

/**********************************************************************************************************************
 *  EthTSyn_AsymSiteSyncSyncSm_TxFupFinished
 *********************************************************************************************************************/
/*! \brief      Informs the SiteSyncSyncSm about finished FollowUp message transmission.
 *  \details    -
 *  \param[in]  AsymSiteSyncSyncSmIdx  Index of the site sync sync state machine.
 *                                     [range: AsymSiteSyncSyncSmIdx < EthTSyn_GetSizeOfAsymSiteSyncSyncSm()]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires AsymSiteSyncSyncSmIdx < EthTSyn_GetSizeOfAsymSiteSyncSyncSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_AsymSiteSyncSyncSm_TxFupFinished(
  EthTSyn_AsymSiteSyncSyncSm_SmIdxType AsymSiteSyncSyncSmIdx);
#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */
#endif /* ETHTSYN_ASYM_SITE_SYNC_SYNC_SM_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_AsymSiteSyncSyncSm_Int.h
 *********************************************************************************************************************/
