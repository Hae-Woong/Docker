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
/*!        \file  EthTSyn_SiteSyncSyncSm_Int.h
 *         \unit  SiteSyncSyncSm
 *        \brief  EthTSyn internal header file of the SiteSyncSyncSm unit.
 *      \details  Interface and type definitions of the SiteSyncSyncSm unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_SITE_SYNC_SYNC_SM_INT_H)
# define ETHTSYN_SITE_SYNC_SYNC_SM_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_SITESYNCSYNCSM == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
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
 *  EthTSyn_SiteSyncSyncSm_Init
 *********************************************************************************************************************/
/*! \brief      Initializes the SiteSyncSyncSm unit.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_MainFunction
 *********************************************************************************************************************/
/*! \brief      Processes all active site sync sync state machines.
 *  \details    -
 *  \pre        Has to be called cyclically in context of the EthTSyn_MainFunction()
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_MainFunction(void);

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_ProcRcvdSync
 *********************************************************************************************************************/
/*! \brief      Processing of a received Sync message.
 *  \details    -
 *  \param[in]  SiteSyncSyncSmIdx    Index of the site sync sync state machine.
 *                                   [range: SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm()]
 *  \param[in]  PtpHdrPtr            De-serialized header of the received Sync message
 *  \param[in]  SyncSwtIngrTsMgmtPtr Switch ingress timestamp of the received Sync message
 *  \pre        Message length in Ptp header needs to be valid, i.e. PtpHdrPtr->MessageLength == ETHTSYN_MSG_SYNC_LEN
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
 *    requires PtpHdrPtr->MessageLength == ETHTSYN_MSG_SYNC_LEN;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_ProcRcvdSync(
                  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)              PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)              SyncSwtIngrTsMgmtPtr);

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_ProcRcvdFup
 *********************************************************************************************************************/
/*! \brief      Processing of a received FollowUp message.
 *  \details    Check if passed port is ready to receive a FollowUp message and optionally processes the message.
 *  \param[in]  SiteSyncSyncSmIdx  Index of the site sync sync state machine.
 *                                 [range: SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm()]
 *  \param[in]  PtpHdrPtr          De-serialized header of the received FollowUp message
 *  \param[in]  FupPayloadPtr      Pointer to the serialized payload of the received FollowUp message
 *  \pre        Must only be called when the corresponding Sync was received by the SiteSyncSyncSm via
 *              EthTSyn_SiteSyncSyncSm_ProcRcvdSync()
 *  \pre        Message length in Ptp header needs to be valid, i.e. PtpHdrPtr->MessageLength >= ETHTSYN_MSG_FUP_LEN
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
 *    requires PtpHdrPtr->MessageLength >= ETHTSYN_MSG_FUP_LEN;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_ProcRcvdFup(
                  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)              PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                           FupPayloadPtr);

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_TxSyncFinished
 *********************************************************************************************************************/
/*! \brief      Informs the SiteSyncSyncSm about finished Sync message transmission.
 *  \details    -
 *  \param[in]  SiteSyncSyncSmIdx  Index of the site sync sync state machine.
 *                                 [range: SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm()]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_TxSyncFinished(
  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_GetSequenceIdIncrement
 *********************************************************************************************************************/
/*! \brief      Get the sequence id increment from the SiteSyncSyncSm.
 *  \details    -
 *  \param[in]  SiteSyncSyncSmIdx  Index of the site sync sync state machine.
 *                                 [range: SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm()]
 *  \return     The sequence id increment which needs to be applied.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint16, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_GetSequenceIdIncrement(
  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_CopyFupPayload
 *********************************************************************************************************************/
/*! \brief      Copies the Payload of the FollowUp message to a provided buffer (in Network-Byte-Order)
 *  \details    -
 *  \param[in]  SiteSyncSyncSmIdx  Index of the site sync sync state machine.
 *                                 [range: SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm()]
 *  \param[out] PayloadDestPtr     Pointer to a buffer with length PayloadDestLen. The FollowUp payload is copied
 *                                 here.
 *  \param[in]  PayloadDestLen     Length of the payload destination buffer
 *  \return     E_OK - Payload successfully copied to the buffer
 *  \return     E_NOT_OK - Failed to copy the payload to the buffer
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
 *    requires PayloadDestPtr != NULL_PTR;
 *  \endspec
 *  \note It is not guaranteed that the complete PayloadDest buffer is initialzed. Only the required part is set when
 *        E_OK is returned.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_CopyFupPayload(
                EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx,
  ETHTSYN_P2VAR(uint8)                           PayloadDestPtr,
                uint16                           PayloadDestLen);

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_TxFupFinished
 *********************************************************************************************************************/
/*! \brief      Informs the SiteSyncSyncSm about finished FollowUp message transmission.
 *  \details    -
 *  \param[in]  SiteSyncSyncSmIdx  Index of the site sync sync state machine.
 *                                 [range: SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm()]
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_TxFupFinished(
  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */
#endif /* ETHTSYN_SITE_SYNC_SYNC_SM_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SiteSyncSyncSm_Int.h
 *********************************************************************************************************************/
