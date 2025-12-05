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
/*!        \file  EthTSyn_SiteSyncSyncTx_Int.h
 *         \unit  SiteSyncSyncTx
 *        \brief  EthTSyn internal header file of the SiteSyncSyncTx unit.
 *      \details  Interface and type definitions of the SiteSyncSyncTx unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_SITE_SYNC_SYNC_TX_INT_H)
# define ETHTSYN_SITE_SYNC_SYNC_TX_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"
# include "EthTSyn_GeneratedCfgAccess.h"

# if ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) ) /* COV_MSR_UT_OPTIONAL_UNIT */

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
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncTx_Init
 *********************************************************************************************************************/
/*! \brief      Initializes the SiteSyncSyncTx unit.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  TRUE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncTx_MainFunction
 *********************************************************************************************************************/
/*! \brief      Processes all SiteSyncSyncTx entities.
 *  \details    -
 *  \pre        Has to be called cyclically in context of the EthTSyn_MainFunction()
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_MainFunction(void);

#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_StartSyncTransmission
 *********************************************************************************************************************/
/*! \brief          Starts the Sync message transmission on all mapped master ports
 *  \details        Informs the SiteSyncSyncTx entity that a new Sync message shall be transmitted on all mapped
 *                  master ports. Any ongoing transmissions are terminated. Transmission of the new Sync messages
 *                  is started within the next invocation of EthTSyn_SiteSyncSyncTx_MainFunction.
 *  \param[in]      SiteSyncSyncTxIdx  Index of the SiteSyncSyncTx entity
 *                  [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \param[in]      SyncMsgHdrPtr      The PTP message header of the Sync message
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *    requires SyncMsgHdrPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_StartSyncTransmission(
                  EthTSyn_SiteSyncSyncTx_IdxType SiteSyncSyncTxIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)            SyncMsgHdrPtr);
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_ProvideSyncTimestamps
 *********************************************************************************************************************/
/*! \brief          Provide the Sync ingress timestamp and (optional) Sync egress timestamp(s)
 *  \details        -
 *  \param[in]      SiteSyncSyncTxIdx  Index of the SiteSyncSyncTx entity
 *                                     [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \param[in]      SlavePdelayNs      Pdelay of the Slave port at time of the Sync reception
 *  \param[in]      SyncIngrTsMgmtPtr  The Sync ingress timestamps and its validity information
 *  \param[in]      SyncEgrTsInfoPtr   Switch egress timestamp info of the Sync message(s)
 *  \param[in]      SyncEgrTsInfoCnt   Amount of egress timestamps provided via SyncEgrTsInfoPtr
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *    requires SyncIngrTsMgmtPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_ProvideSyncTimestamps(
                  EthTSyn_SiteSyncSyncTx_IdxType SiteSyncSyncTxIdx,
                  EthTSyn_PdelayType             SlavePdelayNs,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)            SyncIngrTsMgmtPtr,
  ETHTSYN_P2CONST(EthSwt_VMgmtEgrTsInfoType)     SyncEgrTsInfoPtr,
                  uint8                          SyncEgrTsInfoCnt);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_StartFupTransmission
 *********************************************************************************************************************/
/*! \brief          Starts the FollowUp message transmission on all mapped master ports
 *  \details        Informs the SiteSyncSyncTx entity that a new FollowUp message shall be transmitted on all mapped
 *                  master ports. Any ongoing transmissions are terminated. Necessity of CRC update is checked and
 *                  transmission of the new FollowUp messages is started within the next invocation of
 *                  EthTSyn_SiteSyncSyncTx_MainFunction.
 *  \param[in]      SiteSyncSyncTxIdx  Index of the SiteSyncSyncTx entity
 *                                     [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \param[in]      FupMsgHdrPtr       The PTP message header of the FollowUp message
 *  \param[in]      FupPayloadPtr      Payload of the FollowUp message
 *  \param[in]      AlwaysUpdateCrc    Flag to indicate if CRC update is always required
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *    requires FupMsgHdrPtr != NULL_PTR;
 *    requires FupPayloadPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_StartFupTransmission(
                  EthTSyn_SiteSyncSyncTx_IdxType SiteSyncSyncTxIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)            FupMsgHdrPtr,
  ETHTSYN_P2CONST(uint8)                         FupPayloadPtr,
                  boolean                        AlwaysUpdateCrc);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_CancelOngoingTransmissions
 *********************************************************************************************************************/
/*! \brief          Cancels all ongoing Sync and FollowUp transmissions
 *  \details        -
 *  \param[in]      SiteSyncSyncTxIdx  Index of the SiteSyncSyncTx entity
 *                                     [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_CancelOngoingTransmissions(
  EthTSyn_SiteSyncSyncTx_IdxType SiteSyncSyncTxIdx);

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) ) */
#endif /* ETHTSYN_SITE_SYNC_SYNC_TX_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SiteSyncSyncTx_Int.h
 *********************************************************************************************************************/
