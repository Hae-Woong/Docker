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
/*!        \file  EthTSyn_SiteSyncSync.c
 *        \brief  EthTSyn SiteSyncSync source file
 *      \details  Implementation of all units included in the logical unit SiteSyncSync (SiteSyncSyncSm,
 *                AsymSiteSyncSyncSm, SiteSyncSyncTx, SiteSyncSyncTlv).
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define ETHTSYN_SITE_SYNC_SYNC_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_GeneratedCfgAccess.h"

# include "EthTSyn_SiteSyncSyncTx_Int.h"
# include "EthTSyn_MsgDefs_Int.h"
# include "EthTSyn_Timestamp_Int.h"
# include "EthTSyn_SiteSyncSyncSm_Int.h"
# include "EthTSyn_PortSyncSendSm_Int.h"
# include "EthTSyn_AsymSiteSyncSyncSm_Int.h"
# include "EthTSyn_Tx_Int.h"
# include "EthTSyn_PortParamStats_Int.h"
# include "EthTSyn_Memory_Int.h"
# include "EthTSyn_SiteSyncSyncTlv_Int.h"
# include "EthTSyn_CrcHndl_Int.h"
# include "EthTSyn_ComCtrl_Int.h"
# include "vstdlib.h"

#if ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) ) /* COV_MSR_UT_OPTIONAL_UNIT */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define ETHTSYN_SITE_SYNC_SYNC_MS_PER_S                              (1000u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef enum
{
  ETHTSYN_MSG_SENT,
  ETHTSYN_MSG_RETRY_EXPIRED,
  ETHTSYN_MSG_PENDING
} EthTSyn_SiteSyncSync_TxMsgRetVal;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Internal function prototypes of SiteSyncSyncSm
 *!
 * \unit SiteSyncSyncSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SITESYNCSYNCSM)
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncSm_ProcEntity
 *********************************************************************************************************************/
/*! \brief          Process the SiteSyncSyncSm entity
 *  \details        -
 *  \param[in]      SiteSyncSyncSmIdx  Index of the SiteSyncSyncSm entity
 *                                     [range: SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm()]
 *  \pre            Needs to be called cyclically in context of the EthTSyn_MainFunction()
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_ProcEntity(
  EthTSyn_SiteSyncSyncSmIterType SiteSyncSyncSmIdx);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncSm_EnterCyclicMode
 *********************************************************************************************************************/
/*! \brief          Enters the cyclic operation mode for the SiteSyncSyncSm entity
 *  \details        -
 *  \param[in]      SiteSyncSyncSmIdx  Index of the SiteSyncSyncSm entity
 *                                     [range: SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_EnterCyclicMode(
  EthTSyn_SiteSyncSyncSmIterType SiteSyncSyncSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_GetSyncCycleFromSyncMsg
 *********************************************************************************************************************/
/*! \brief      Calculates the sync message cycle (in EthTSyn main function cycles) from the log message interval
 *  \details    -
 *  \param[in]  LogMsgInterval  The log message interval
 *  \return     The sync message cycle expressed in EthTSyn main function cycles
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint32, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_GetSyncCycleFromSyncMsg(
  sint8 LogMsgInterval);
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncSm_EnterTxSyncState
 *********************************************************************************************************************/
/*! \brief          Changes the state of the SiteSyncSyncSm to TX_SYNC and performs all 'on entry' actions
 *  \details        -
 *  \param[in]      SiteSyncSyncSmIdx  Index of the SiteSyncSyncSm entity
 *                                     [range: SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \note           Ensure that TX_SYNC state is only entered by using this function
 *  \spec
 *    requires SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_EnterTxSyncState(
  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncSm_EnterTxFupState
 *********************************************************************************************************************/
/*! \brief          Changes the state of the SiteSyncSyncSm to TX_FUP and performs all 'on entry' actions
 *  \details        -
 *  \param[in]      SiteSyncSyncSmIdx  Index of the SiteSyncSyncSm entity
 *                                     [range: SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \note           Ensure that TX_FUP state is only entered by using this function
 *  \spec
 *    requires SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_EnterTxFupState(
  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx);

#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncSm_CheckSyncRxTimeout
 *********************************************************************************************************************/
/*! \brief          Checks if a Sync reception timeout occurred.
 *  \details        -
 *  \param[in]      SiteSyncSyncSmIdx  Index of the SiteSyncSyncSm entity
 *                                     [range: SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm()]
 *  \return         TRUE - Sync reception timeout occurred
 *  \return         FALSE - No Sync reception timeout occurred
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_CheckSyncRxTimeout(
  EthTSyn_SiteSyncSyncSmIterType SiteSyncSyncSmIdx);
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */
# endif /* ETHTSYN_NOUNIT_SITESYNCSYNCSM */

/**********************************************************************************************************************
 * Internal function prototypes of AsymSiteSyncSyncSm
 *!
 * \unit AsymSiteSyncSyncSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_ASYMSITESYNCSYNCSM)
#  if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
#  endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */
# endif /* ETHTSYN_NOUNIT_ASYMSITESYNCSYNCSM */

/**********************************************************************************************************************
 * Internal function prototypes of SiteSyncSyncTx
 *!
 * \unit SiteSyncSyncTx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SITESYNCSYNCTX)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_ProcEntity
 *********************************************************************************************************************/
/*! \brief          Processes one SiteSyncSyncTx entity
 *  \details        -
 *  \param[in]      SiteSyncSyncTxIdx  Index of the SiteSyncSyncTx entity
 *                                     [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \pre            Has to be called cyclically in context of the EthTSyn_MainFunction()
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_ProcEntity(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx);

#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_ProcTxSyncState
 *********************************************************************************************************************/
/*! \brief          Processes the TX_SYNC state of the SiteSyncSyncTx entity
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_ProcTxSyncState(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx);
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_ProcTxFupState
 *********************************************************************************************************************/
/*! \brief          Processes the TX_FUP state of the SiteSyncSyncTx entity
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_ProcTxFupState(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_TxSync
 *********************************************************************************************************************/
/*! \brief          Transmits a Sync message on every mapped master port (if not already done).
 *  \details        -
 *  \param[in]      SiteSyncSyncTxIdx  Index of the SiteSyncSyncTx entity
 *                                     [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \return         ETHTSYN_MSG_SENT - Message transmitted on all mapped master ports
 *  \return         ETHTSYN_MSG_PENDING - Message transmission pending on at least one mapped master port
 *  \return         ETHTSYN_MSG_RETRY_EXPIRED - Message forward retry counter expired while transmission is pending on
 *                                              at least one mapped master port
 *  \pre            SiteSyncSyncTx entity is in state TX_SYNC
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SiteSyncSync_TxMsgRetVal, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_TxSync(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_CalcSequenceId
 *********************************************************************************************************************/
/*! \brief          Calculates the sequence id which shall be used for Sync and FollowUp message transmission.
 *  \details        -
 *  \param[in]      SiteSyncSyncTxIdx  Index of the SiteSyncSyncTx entity
 *                                     [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \param[in]      OrgSequenceId      Original sequence id from the PTP message header
 *  \return         The updated sequence id.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_CalcSequenceId(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx,
  uint16                         OrgSequenceId);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_HndlMsgTxRetry
 *********************************************************************************************************************/
/*! \brief          Handles the message transmission retry.
 *  \details        Checks if message has been transmitted on all mapped master ports and handles the transmission
 *                  retry counter if this is not the case.
 *  \param[in]      SiteSyncSyncTxIdx           Index of the SiteSyncSyncTx entity
 *                                              [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \param[in]      TxFinishedOnAllMasterPorts  Indicating if the message transmission is finished on all mapped
 *                                              master ports
 *  \return         ETHTSYN_MSG_SENT - Message transmission finished on all mapped master ports
 *  \return         ETHTSYN_MSG_PENDING - Message transmission is pending on at least one mapped master port
 *  \return         ETHTSYN_MSG_RETRY_EXPIRED - Message transmission retry counter expired
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SiteSyncSync_TxMsgRetVal, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_HndlMsgTxRetry(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx,
  boolean                        TxFinishedOnAllMasterPorts);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_TxFup
 *********************************************************************************************************************/
/*! \brief          Transmits a FollowUp message on every mapped master port (if not already done).
 *  \details        -
 *  \param[in]      SiteSyncSyncTxIdx  Index of the SiteSyncSyncTx entity
 *                                     [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \return         ETHTSYN_MSG_SENT - Message transmission finished on all mapped master ports
 *  \return         ETHTSYN_MSG_PENDING - Message transmission is pending on at least one mapped master port
 *  \return         ETHTSYN_MSG_RETRY_EXPIRED - Message transmission retry counter expired
 *  \pre            SiteSyncSyncTx entity is in state TX_FUP
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SiteSyncSync_TxMsgRetVal, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_TxFup(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_HndlCalcSyncFwdDelayStateOfTxBridgeMasterPort
 *********************************************************************************************************************/
/*! \brief          Handles the FupFwdState CALC_SYNC_FWD_DELAY of a TxBridgeMasterPort
 *  \details        Calculates the Sync forward delay and and updates the stored value in case of successful
 *                  calculation. Updates the FupFwdState as follows:
 *                  - No change - Calculation of SyncFwdDelay not (yet) possible. Retry shall be done.
 *                  - FINISHED - Calculation of SyncFwdDelay not possible at all or resulting value is invalid. No
 *                               retry shall be done.
 *                  - PENDING - Calculation of SyncFwdDelay succeeded. FollowUp message can be transmitted
 *  \param[in]      MasterPortIdx    Index of the bridge master port in context of the SiteSyncSyncTx entity
 *                                   [range: MasterPortIdx < EthTSyn_GetSizeOfBridgeTxMasterPort()]
 *  \param[in]      FupCorrField     The received correction field value
 *  \param[in]      SlavePdelayNs    Pdelay of the slave port in nanoseconds
 *  \param[in]      SyncIngrTsPtr    The Sync ingress timestamp
 *  \pre            FupFwdState of the TxBridgeMasterPort is CALC_SYNC_FWD_DELAY
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires MasterPortIdx < EthTSyn_GetSizeOfBridgeTxMasterPort();
 *    requires SyncIngrTsPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_HndlCalcSyncFwdDelayStateOfTxBridgeMasterPort(
                  EthTSyn_BridgeTxMasterPortIterType MasterPortIdx,
                  uint64                             FupCorrField,
                  EthTSyn_PdelayType                 SlavePdelayNs,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)       SyncIngrTsPtr);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_IsSyncFwdDelayValid
 *********************************************************************************************************************/
/*! \brief          Checks if SyncFwdDelay is valid, i.e., can be used to update the correction field
 *  \details        -
 *  \param[in]      SyncFwdDelay  The Sync forward delay
 *  \param[in]      FupCorrField  The received correction field value
 *  \return         TRUE - Sync forward delay is valid
 *  \return         FALSE - Sync forward delay is invalid
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_IsSyncFwdDelayValid(
  uint64 SyncFwdDelay,
  uint64 FupCorrField);

#  if (ETHTSYN_SWT_RES_TIME_NS == 0u)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_CalcSyncFwdDelay
 *********************************************************************************************************************/
/*! \brief          Try to calculate the Sync forward delay
 *  \details        -
 *  \param[in]      SlavePdelayNs     Pdelay of the slave port in nanoseconds
 *  \param[in]      SyncIngrTsPtr     The Sync ingress timestamp
 *  \param[in]      SyncEgrTsMgmtPtr  The Sync egress timestamp management
 *  \return         (ETHTSYN_MSG_HDR_CORR_FIELD_MAX_NS + 1) - Calculation of Sync forward delay failed or not possible
 *  \return         other values - Calculated Sync forward delay in nanoseconds
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SyncIngrTsPtr != NULL_PTR;
 *    requires SyncEgrTsMgmtPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint64, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_CalcSyncFwdDelay(
                  EthTSyn_PdelayType                 SlavePdelayNs,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)       SyncIngrTsPtr,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)                SyncEgrTsMgmtPtr);
#  endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_UpdateSyncFwdDelayAndFupFwdState
 *********************************************************************************************************************/
/*! \brief          Updates the stored SyncFwdDelay and FupFwdState
 *  \details        Checks if the new SyncFwdDelay is valid. If so, the stored SyncFwdDelay is updated. The FupFwdState
 *                  is updated as follows:
 *                  - PENDING - SyncFwdDelay is valid
 *                  - FINISHED - SyncFwdDelay is invalid
 *  \param[in]      MasterPortIdx    Index of the bridge master port in context of the SiteSyncSyncTx entity
 *                                   [range: MasterPortIdx < EthTSyn_GetSizeOfBridgeTxMasterPort()]
 *  \param[in]      SyncFwdDelay  The Sync forward delay
 *  \param[in]      FupCorrField  The received correction field value
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires MasterPortIdx < EthTSyn_GetSizeOfBridgeTxMasterPort();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_UpdateSyncFwdDelayAndFupFwdState(
  EthTSyn_BridgeTxMasterPortIterType MasterPortIdx,
  uint64                             SyncFwdDelay,
  uint64                             FupCorrField);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_TxFupOnMasterPort
 *********************************************************************************************************************/
/*! \brief          Try to transmit/forward the FollowUp message on a master port
 *  \details        -
 *  \param[in]      SiteSyncSyncTxIdx  Index of the SiteSyncSyncTx entity
 *                                     [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \param[in]      MasterPortIdx      Index of the bridge master port in context of the SiteSyncSyncTx entity
 *                                     [range: MasterPortIdx < EthTSyn_GetSizeOfBridgeTxMasterPort()]
 *  \param[in]      SyncFwdDelay       The Sync forward delay
 *  \param[in]      TypeSpecHdrPtr     The type specific header
 *  \param[in]      PortInfoHdrPtr     The port info header or NULL_PTR
 *  \return         E_OK - FollowUp transmission on master port succeeded
 *  \return         E_NOT_OK - FollowUp transmission on master port failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *    requires MasterPortIdx < EthTSyn_GetSizeOfBridgeTxMasterPort();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_TxFupOnMasterPort(
                  EthTSyn_SiteSyncSyncTxIterType     SiteSyncSyncTxIdx,
                  EthTSyn_BridgeTxMasterPortIterType MasterPortIdx,
                  uint64                             SyncFwdDelay,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType)        TypeSpecHdrPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType)        PortInfoHdrPtr);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_StartTxFupSequence
 *********************************************************************************************************************/
/*! \brief          Starts the FollowUp transmission sequence
 *  \details        -
 *  \param[in]      MasterPortIdx   Index of the bridge master port in context of the SiteSyncSyncTx entity
 *                                  [range: MasterPortIdx < EthTSyn_GetSizeOfBridgeTxMasterPort()]
 *  \param[in]      TypeSpecHdrPtr  The type specific header
 *  \param[in]      PortInfoHdrPtr  The port info header or NULL_PTR
 *  \param[out]     TxBufMgmtPtr    Management structure for the transmission buffer
 *  \return         E_OK - FollowUp transmission on master port succeeded
 *  \return         E_NOT_OK - FollowUp transmission on master port failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires MasterPortIdx < EthTSyn_GetSizeOfBridgeTxMasterPort();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_StartTxFupSequence(
                  EthTSyn_BridgeTxMasterPortIterType MasterPortIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType)        TypeSpecHdrPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType)        PortInfoHdrPtr,
    ETHTSYN_P2VAR(EthTSyn_EthTxBufferStructType)     TxBufMgmtPtr);

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_CopyFupPayload
 *********************************************************************************************************************/
/*! \brief          Copies the FollowUp message payload to the transmission buffer (in Network-Byte-Order)
 *  \details        -
 *  \param[in]      SiteSyncSyncTxIdx  Index of the SiteSyncSyncTx entity
 *                                     [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \param[in,out]  TxBufMgmtPtr       Management structure for the transmission buffer
 *                                      In: The transmission buffer
 *                                     Out: Serialized FollowUp payload within the transmission buffer
 *  \return         E_OK - FollowUp message serialization succeeded
 *  \return         E_NOT_OK - FollowUp message serialization failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_CopyFupPayload(
                  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx,
  ETHTSYN_P2CONST(EthTSyn_EthTxBufferStructType) TxBufMgmtPtr);

#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_FupUpdateCrc
 *********************************************************************************************************************/
/*! \brief          Updated the CRC of the time secured TLV (if required)
 *  \details        -
 *  \param[in]      SiteSyncSyncTxIdx  Index of the SiteSyncSyncTx entity
 *                                     [range: SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx()]
 *  \param[in,out]  TxBufMgmtPtr       Management structure for the transmission buffer
 *                                      In: The transmission buffer holding the serialized FollowUp payload
 *                                     Out: Updated time secured TLV Crc within the transmission buffer
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires SiteSyncSyncTxIdx < EthTSyn_GetSizeOfSiteSyncSyncTx();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_FupUpdateCrc(
                  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx,
  ETHTSYN_P2CONST(EthTSyn_EthTxBufferStructType) TxBufMgmtPtr);
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_FinishTxFupSequence
 *********************************************************************************************************************/
/*! \brief          Finalizes the FollowUp transmission sequence
 *  \details        -
 *  \param[in]      MasterPortIdx  Index of the bridge master port in context of the SiteSyncSyncTx entity
 *                                 [range: MasterPortIdx < EthTSyn_GetSizeOfBridgeTxMasterPort()]
 *  \param[in]      TxBufMgmtPtr   Management structure for the transmission buffer
 *  \param[in]      TxMsgLen       Length of the FollowUp message
 *  \return         E_OK - FollowUp message transmission succeeded
 *  \return         E_NOT_OK - FollowUp message transmission failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires MasterPortIdx < EthTSyn_GetSizeOfBridgeTxMasterPort();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_FinishTxFupSequence(
                  EthTSyn_BridgeTxMasterPortIterType MasterPortIdx,
  ETHTSYN_P2CONST(EthTSyn_EthTxBufferStructType)     TxBufMgmtPtr,
                  uint16                             TxMsgLen);
# endif /* ETHTSYN_NOUNIT_SITESYNCSYNCTX */

/**********************************************************************************************************************
 * Internal function prototypes of SiteSyncSyncTlv
 *!
 * \unit SiteSyncSyncTlv
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SITESYNCSYNCTLV)
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTlv_FindTimeSecuredTlv
 *********************************************************************************************************************/
/*! \brief          Searches the time secured TLV in the received FollowUp message.
 *  \details        -
 *  \param[in]      PtpHdrPtr          De-serialized header of the received FollowUp message
 *  \param[in]      FupPayloadPtr      Pointer to the serialized payload of the received FollowUp message
 *  \return         0 - No time secured TLV found
 *  \return         other values - Offset to the time secured TLV relative to the FollowUp payload
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTlv_FindTimeSecuredTlv (
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType) PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)              FupPayloadPtr);
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */
# endif /* ETHTSYN_NOUNIT_SITESYNCSYNCTLV */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Internal functions of SiteSyncSyncSm
 *!
 * \unit SiteSyncSyncSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SITESYNCSYNCSM)
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncSm_ProcEntity
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_ProcEntity(
  EthTSyn_SiteSyncSyncSmIterType SiteSyncSyncSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const boolean syncRxTimeout = EthTSyn_SiteSyncSyncSm_CheckSyncRxTimeout(SiteSyncSyncSmIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if SiteSyncSyncSm is in cyclic operation mode.
   *       Decrement the Sync cycle counter if possible.
   *       Check if SiteSyncSyncSm is in state RCVD_SYNC_AFTER_TIMEOUT.
   *         Check if Sync reception timeout occurred.
   *           Change state of the SiteSyncSyncSm to WAIT_TX_SYNC.
   *        Check if SiteSyncSyncSm is in state WAIT_TX_SYNC.
   *          Check if Sync cycle counter expired.
   *            Recharge the Sync cycle counter.
   *            Change state of the SiteSyncSyncSm to TX_SYNC. */
  if (EthTSyn_GetOperationModeOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) ==
      ETHTSYN_CYCLIC_OPERATIONMODEOFSITESYNCSYNCSMDYN)
  {
    if (EthTSyn_GetSyncCycleCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) > 0u)
    {
      EthTSyn_DecSyncCycleCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx);
    }

    if (EthTSyn_GetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) ==
        ETHTSYN_RCVD_SYNC_AFTER_TIMEOUT_STATEOFSITESYNCSYNCSMDYN)
    {
      if (syncRxTimeout)
      {
        EthTSyn_SetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, ETHTSYN_WAIT_TX_SYNC_STATEOFSITESYNCSYNCSMDYN);
      }
    }

    if (EthTSyn_GetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) == ETHTSYN_WAIT_TX_SYNC_STATEOFSITESYNCSYNCSMDYN)
    {
      if (EthTSyn_GetSyncCycleCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) == 0u)
      {
        EthTSyn_IncSequenceIdIncOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx);
        EthTSyn_SetSyncCycleCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx,
          EthTSyn_GetSyncCycleFromMsgOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx));
        EthTSyn_SiteSyncSyncSm_EnterTxSyncState((EthTSyn_SiteSyncSyncSm_SmIdxType)SiteSyncSyncSmIdx);
      }
    }
  }
  /* #20 Otherwise.
   *       Check if Sync reception timeout occurred.
   *         Enter cyclic operation mode. */
  else
  {
    if (syncRxTimeout)
    {
      EthTSyn_SiteSyncSyncSm_EnterCyclicMode(SiteSyncSyncSmIdx);
    }
  }
} /* EthTSyn_SiteSyncSyncSm_ProcEntity() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncSm_EnterCyclicMode
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_EnterCyclicMode(
  EthTSyn_SiteSyncSyncSmIterType SiteSyncSyncSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_GetLastValidIngrTsMgmtOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx).TsValid)
  {
    const EthTSyn_MsgHdrType lastValidRcvdSyncHdr =
      EthTSyn_GetLastValidRcvdHdrOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx);

    EthTSyn_SetRcvdHdrOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx, lastValidRcvdSyncHdr);
    EthTSyn_SetIngrTsMgmtOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx,
      EthTSyn_GetLastValidIngrTsMgmtOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx));
    EthTSyn_SetPdelayOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx,
      EthTSyn_GetLastValidPdelayOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx));
    EthTSyn_SetSequenceIdIncOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, 0u);
    EthTSyn_SetSyncCycleCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, 0u);
    EthTSyn_SetSyncCycleFromMsgOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx,
      EthTSyn_SiteSyncSyncSm_GetSyncCycleFromSyncMsg(lastValidRcvdSyncHdr.LogMessageInterval));

    EthTSyn_SetFwdPendingOfSiteSyncSyncSmFupHndlDyn(SiteSyncSyncSmIdx, FALSE);

    EthTSyn_SetOperationModeOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, ETHTSYN_CYCLIC_OPERATIONMODEOFSITESYNCSYNCSMDYN);
    EthTSyn_SetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, ETHTSYN_WAIT_TX_SYNC_STATEOFSITESYNCSYNCSMDYN);
  }
} /* EthTSyn_SiteSyncSyncSm_EnterCyclicMode() */

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_GetSyncCycleFromSyncMsg
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint32, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_GetSyncCycleFromSyncMsg (
  sint8 LogMsgInterval)
{
  /* ----- Local Variables ---------------------------------------------- */
  float64 syncCycleMf = 0.0;
  float64 syncCycleSeconds = 1.0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 LogMsgInterval represents the Sync cycle in seconds expressed as logarithm base 2. Convert accordingly. */
  if (LogMsgInterval == 0)
  {
    /* The cycle time is 1 second, if the log message interval is zero. */
  }
  else
  {
    float64 syncCycleBase = 0.0;
    uint8 syncCycleExponent = 0u;
    uint8_least iter = 0u;

    /* If the log message interval is negative. */
    if (LogMsgInterval < 0)
    {
      /* Use 1/2 as base and the absolute value of the log message interval as exponent for the cycle calculation. */
      syncCycleBase = 0.5;
      syncCycleExponent = (uint8)(-LogMsgInterval);
    }
      /* Otherwise, the log message interval is positive. */
    else
    {
      /* Use 2 as base and the log message interval as exponent for the cycle calculation. */
      syncCycleBase = 2.0;
      syncCycleExponent = (uint8)LogMsgInterval;
    }

    /* Calculate the sync cycle time in seconds. */
    for (iter = 0; iter < syncCycleExponent; iter++)
    {
      syncCycleSeconds *= syncCycleBase;
    }
  }

  /* #20 Convert the Sync cycle in seconds into main function cycles. */
  syncCycleMf = syncCycleSeconds * (float64)ETHTSYN_SITE_SYNC_SYNC_MS_PER_S;
  syncCycleMf /= (float64)ETHTSYN_MAIN_FUNCTION_CYCLE;
  syncCycleMf += 0.5; /* +0.5 for correct rounding. */

  return (uint32)syncCycleMf;
} /* EthTSyn_SiteSyncSyncSm_GetSyncCycleFromSyncMsg() */
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncSm_EnterTxSyncState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_EnterTxSyncState(
  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_SiteSyncSyncTxIdxOfSiteSyncSyncSmType siteSyncSyncTxIdx =
    EthTSyn_GetSiteSyncSyncTxIdxOfSiteSyncSyncSm(SiteSyncSyncSmIdx);

  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_SiteSyncSyncTx_CancelOngoingTransmissions(siteSyncSyncTxIdx);
  EthTSyn_SiteSyncSyncTx_StartSyncTransmission(siteSyncSyncTxIdx,
    EthTSyn_GetAddrRcvdHdrOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx));

  EthTSyn_SetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, ETHTSYN_TX_SYNC_STATEOFSITESYNCSYNCSMDYN);
} /* EthTSyn_SiteSyncSyncSm_EnterTxSyncState() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncSm_EnterTxFupState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_EnterTxFupState(
  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_SiteSyncSyncTxIdxOfSiteSyncSyncSmType siteSyncSyncTxIdx =
    EthTSyn_GetSiteSyncSyncTxIdxOfSiteSyncSyncSm(SiteSyncSyncSmIdx);
#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
  const boolean alwaysUpdateCrc = (boolean)(EthTSyn_GetOperationModeOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) ==
    ETHTSYN_CYCLIC_OPERATIONMODEOFSITESYNCSYNCSMDYN);
#   else
  const boolean alwaysUpdateCrc = FALSE;
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */

  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_SiteSyncSyncTx_ProvideSyncTimestamps(siteSyncSyncTxIdx,
    EthTSyn_GetPdelayOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx),
    EthTSyn_GetAddrIngrTsMgmtOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx),
    NULL_PTR, 0u /* No Sync egress timestamps are provided here (they are fetched by SiteSyncSyncTx later) */);

  (void)EthTSyn_SiteSyncSyncTx_StartFupTransmission(siteSyncSyncTxIdx,
    EthTSyn_GetAddrRcvdHdrOfSiteSyncSyncSmFupHndlDyn(SiteSyncSyncSmIdx),
    EthTSyn_GetAddrRcvdFupPayloadBufDyn(EthTSyn_GetRcvdFupPayloadBufDynStartIdxOfSiteSyncSyncSm(SiteSyncSyncSmIdx)),
    alwaysUpdateCrc);

  EthTSyn_SetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, ETHTSYN_TX_FUP_STATEOFSITESYNCSYNCSMDYN);
} /* EthTSyn_SiteSyncSyncSm_EnterTxFupState() */

#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncSm_CheckSyncRxTimeout
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_CheckSyncRxTimeout(
  EthTSyn_SiteSyncSyncSmIterType SiteSyncSyncSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean syncRxTimeout = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_GetSyncRxTimeoutCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) > 1u)
  {
    EthTSyn_DecSyncRxTimeoutCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx);
  }
  else if (EthTSyn_GetSyncRxTimeoutCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) == 1u)
  {
    syncRxTimeout = TRUE;
    EthTSyn_SetSyncRxTimeoutCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, 0u);
  }
  else
  {
    /* Nothing to do. */
  }

  return syncRxTimeout;
} /* EthTSyn_SiteSyncSyncSm_CheckSyncRxTimeout() */
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */
# endif /* ETHTSYN_NOUNIT_SITESYNCSYNCSM */

/**********************************************************************************************************************
 * Internal functions of AsymSiteSyncSyncSm
 *!
 * \unit AsymSiteSyncSyncSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_ASYMSITESYNCSYNCSM)
#  if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
#  endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */
# endif /* ETHTSYN_NOUNIT_ASYMSITESYNCSYNCSM */

/**********************************************************************************************************************
 * Internal functions of SiteSyncSyncTx
 *!
 * \unit SiteSyncSyncTx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SITESYNCSYNCTX)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_ProcEntity
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_ProcEntity(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  switch (EthTSyn_GetStateOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx))
  {
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
  case ETHTSYN_TX_SYNC_STATEOFSITESYNCSYNCTXDYN:
    {
      EthTSyn_SiteSyncSyncTx_ProcTxSyncState(SiteSyncSyncTxIdx);
      break;
    }
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

  case ETHTSYN_TX_FUP_STATEOFSITESYNCSYNCTXDYN:
    {
      EthTSyn_SiteSyncSyncTx_ProcTxFupState(SiteSyncSyncTxIdx);
      break;
    }

  default:
    {
      /* Nothing to do. */
      break;
    }
  }
} /* EthTSyn_SiteSyncSyncTx_ProcEntity() */

#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_ProcTxSyncState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_ProcTxSyncState(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_SiteSyncSyncTx_TxSync(SiteSyncSyncTxIdx) != ETHTSYN_MSG_PENDING)
  {
    /* Within TxSyncFinished callback, a new action might be triggered. Hence, it is important to change
     * the state of the SiteSyncSyncTx entity before the callback is invoked. */
    EthTSyn_SetStateOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, ETHTSYN_IDLE_STATEOFSITESYNCSYNCTXDYN);
    EthTSyn_SiteSyncSyncSm_TxSyncFinished(EthTSyn_GetSiteSyncSyncSmIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx));
  }
} /* EthTSyn_SiteSyncSyncTx_ProcTxSyncState() */
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_ProcTxFupState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_ProcTxFupState(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_SiteSyncSyncTx_TxFup(SiteSyncSyncTxIdx) != ETHTSYN_MSG_PENDING)
  {
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
    EthTSyn_SiteSyncSyncSm_TxFupFinished(EthTSyn_GetSiteSyncSyncSmIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx));
#  else
    EthTSyn_AsymSiteSyncSyncSm_TxFupFinished(
      EthTSyn_GetAsymSiteSyncSyncSmIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx));
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */
    EthTSyn_Ts_ResetTsMgmt(EthTSyn_GetAddrLastSyncIngrTsMgmtOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx));
    EthTSyn_SetStateOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, ETHTSYN_IDLE_STATEOFSITESYNCSYNCTXDYN);
  }
} /* EthTSyn_SiteSyncSyncTx_ProcTxFupState() */

#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_TxSync
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
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SiteSyncSync_TxMsgRetVal, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_TxSync(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean syncTransmittedOnAllMasterPort = TRUE;
  EthTSyn_BridgeTxMasterPortDynIterType masterPortIter= 0;
  const EthTSyn_MsgHdrType syncHdr = EthTSyn_GetLastMsgHdrOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx);
  EthTSyn_MsgHdrTypeSpecType typeSpecHdr = {0};
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType) portInfoHdrPtr = NULL_PTR;
  EthTSyn_MsgHdrPortInfoType portInfoHdr = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare the Sync message header.
   *       Use the received port information when option keep source port identity is configured. */
  typeSpecHdr.Control              = syncHdr.Control;
  typeSpecHdr.CorrectionFieldNs    = syncHdr.CorrectionFieldNs;
  typeSpecHdr.CorrectionFieldSubNs = syncHdr.CorrectionFieldSubNs;
  typeSpecHdr.Flags0               = syncHdr.Flags0;
  typeSpecHdr.Flags1               = syncHdr.Flags1;
  typeSpecHdr.LogMessageInterval   = syncHdr.LogMessageInterval;
  typeSpecHdr.MessageLength        = syncHdr.MessageLength;
  /* @ assert typeSpecHdr.MessageLength == ETHTSYN_MSG_SYNC_LEN; */                                                    /* VCA_ETHTSYN_SITE_SYNC_SYNC_SM_SYNC_FUP_MSG_LEN */
  typeSpecHdr.MessageType          = syncHdr.MessageType;
  typeSpecHdr.SequenceId           = EthTSyn_SiteSyncSyncTx_CalcSequenceId(SiteSyncSyncTxIdx, syncHdr.SequenceId);

  if (EthTSyn_IsKeepSrcPortIdentityOfSiteSyncSyncTx(SiteSyncSyncTxIdx))
  {
    portInfoHdr.DomainNumber     = syncHdr.DomainNumber;
    portInfoHdr.SourceClockId    = syncHdr.SourceClockId;
    portInfoHdr.SourcePortNumber = syncHdr.SourcePortNumber;

    portInfoHdrPtr = &portInfoHdr;
  }

  /* #20 Iterate all mapped master ports.
   *       Check if Sync transmission is pending on the current master port.
   *         Try to transmit the Sync message via the PortSyncSendSm.
   *           Change SyncFwdState of the port to FINISHED.
   *         Otherwise, mark Sync transmission as not finished on all master ports. */
  for (masterPortIter = EthTSyn_GetBridgeTxMasterPortStartIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
       masterPortIter < EthTSyn_GetBridgeTxMasterPortEndIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
       masterPortIter++)
  {
    if (EthTSyn_GetSyncFwdStateOfBridgeTxMasterPortDyn(masterPortIter) ==
      ETHTSYN_PENDING_SYNCFWDSTATEOFBRIDGETXMASTERPORTDYN)
    {
      const EthTSyn_PortSyncSendSmIdxOfBridgeTxMasterPortType portSyncSendSmIdx =
        EthTSyn_GetPortSyncSendSmIdxOfBridgeTxMasterPort(masterPortIter);

      if (EthTSyn_PortSyncSendSm_TxSync(portSyncSendSmIdx, &typeSpecHdr, portInfoHdrPtr) == E_OK)
      {
        EthTSyn_SetSyncFwdStateOfBridgeTxMasterPortDyn(masterPortIter,
          ETHTSYN_FINISHED_SYNCFWDSTATEOFBRIDGETXMASTERPORTDYN);
      }
      else
      {
        syncTransmittedOnAllMasterPort = FALSE;
      }
    }
  }

  /* #30 Handle the Sync transmission retry. */
  return EthTSyn_SiteSyncSyncTx_HndlMsgTxRetry(SiteSyncSyncTxIdx, syncTransmittedOnAllMasterPort);
} /* EthTSyn_SiteSyncSyncTx_TxSync() */
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_CalcSequenceId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_CalcSequenceId(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx,
  uint16                         OrgSequenceId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 calculatedSequenceId = OrgSequenceId;

  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
  const uint16 seqIdIncrement =
    EthTSyn_SiteSyncSyncSm_GetSequenceIdIncrement(EthTSyn_GetSiteSyncSyncSmIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx));
  calculatedSequenceId += seqIdIncrement; /* PRQA S 2985 */ /* MD_EthTSyn_2985_PP */
#  else
  ETHTSYN_DUMMY_STATEMENT(SiteSyncSyncTxIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

  return calculatedSequenceId;
} /* EthTSyn_SiteSyncSyncTx_CalcSequenceId() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_HndlMsgTxRetry
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
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SiteSyncSync_TxMsgRetVal, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_HndlMsgTxRetry(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx,
  boolean                        TxFinishedOnAllMasterPorts)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SiteSyncSync_TxMsgRetVal retVal = ETHTSYN_MSG_PENDING;

  /* ----- Implementation ----------------------------------------------- */
  if (TxFinishedOnAllMasterPorts == TRUE)
  {
    retVal = ETHTSYN_MSG_SENT;
  }
  else
  {
    const EthTSyn_NumMsgFwdRetryOfSiteSyncSyncTxType maxRetries =
      EthTSyn_GetNumMsgFwdRetryOfSiteSyncSyncTx(SiteSyncSyncTxIdx);

    if (EthTSyn_GetFwdRetryCntOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx) >= maxRetries)
    {
      retVal = ETHTSYN_MSG_RETRY_EXPIRED;
    }
    else
    {
      EthTSyn_IncFwdRetryCntOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx);
      retVal = ETHTSYN_MSG_PENDING;
    }
  }

  return retVal;
} /* EthTSyn_SiteSyncSyncTx_HndlMsgTxRetry() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_TxFup
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
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SiteSyncSync_TxMsgRetVal, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_TxFup(
  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_TsMgmtType syncIngrTsMgmt = EthTSyn_GetLastSyncIngrTsMgmtOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx);
  boolean fupTransmittedOnAllMasterPort = TRUE;
  EthTSyn_BridgeTxMasterPortDynIterType masterPortIter = 0;
  const EthTSyn_MsgHdrType fupHdr = EthTSyn_GetLastMsgHdrOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx);
  EthTSyn_MsgHdrTypeSpecType typeSpecHdr = {0};
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType) portInfoHdrPtr = NULL_PTR;
  EthTSyn_MsgHdrPortInfoType portInfoHdr = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare the type specific message header.
   *     Prepare the port info header when source port identity shall be kept.
   */

  typeSpecHdr.Control = fupHdr.Control;
  typeSpecHdr.CorrectionFieldNs = fupHdr.CorrectionFieldNs;
  typeSpecHdr.CorrectionFieldSubNs = fupHdr.CorrectionFieldSubNs;
  typeSpecHdr.Flags0 = fupHdr.Flags0;
  typeSpecHdr.Flags1 = fupHdr.Flags1;
  typeSpecHdr.LogMessageInterval = fupHdr.LogMessageInterval;
  typeSpecHdr.MessageLength = fupHdr.MessageLength;
  typeSpecHdr.MessageType = fupHdr.MessageType;
  /*@ assert typeSpecHdr.MessageLength >= ETHTSYN_MSG_FUP_LEN; */                                                      /* VCA_ETHTSYN_SITE_SYNC_SYNC_SM_SYNC_FUP_MSG_LEN */
  typeSpecHdr.SequenceId = EthTSyn_SiteSyncSyncTx_CalcSequenceId(SiteSyncSyncTxIdx, fupHdr.SequenceId);

#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
  if (EthTSyn_IsKeepSrcPortIdentityOfSiteSyncSyncTx(SiteSyncSyncTxIdx))
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */
  {
    portInfoHdr.DomainNumber = fupHdr.DomainNumber;
    portInfoHdr.SourceClockId = fupHdr.SourceClockId;
    portInfoHdr.SourcePortNumber = fupHdr.SourcePortNumber;

    portInfoHdrPtr = &portInfoHdr;
  }

  /* #20 Iterate all mapped master ports.
   *       Check if Sync forward delay still needs to be calculated (i.e., FupFwdState is CALC_SYNC_FWD_DELAY).
   *         Handle the FupFwdState CALC_SYNC_FWD_DELAY.
   *       Try to transmit the FollowUp message on the master port if forwarding is pending.
   *         Change FupFwdState to FINISHED.
   */
  for (masterPortIter = EthTSyn_GetBridgeTxMasterPortStartIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
       masterPortIter < EthTSyn_GetBridgeTxMasterPortEndIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
       masterPortIter++)
  {
    if (EthTSyn_GetFupFwdStateOfBridgeTxMasterPortDyn(masterPortIter) ==
      ETHTSYN_CALC_SYNC_FWD_DELAY_FUPFWDSTATEOFBRIDGETXMASTERPORTDYN)
    {
      const EthTSyn_PdelayType slavePdelayNs = EthTSyn_GetSlavePdelayNsOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx);

      EthTSyn_SiteSyncSyncTx_HndlCalcSyncFwdDelayStateOfTxBridgeMasterPort(masterPortIter,
        typeSpecHdr.CorrectionFieldNs, slavePdelayNs, &syncIngrTsMgmt.Ts);
    }

    if (EthTSyn_GetFupFwdStateOfBridgeTxMasterPortDyn(masterPortIter) ==
      ETHTSYN_PENDING_FUPFWDSTATEOFBRIDGETXMASTERPORTDYN)
    {
      const uint64 syncFwdDelay = EthTSyn_GetSyncFwdDelayNsOfBridgeTxMasterPortDyn(masterPortIter);
      if (EthTSyn_SiteSyncSyncTx_TxFupOnMasterPort(SiteSyncSyncTxIdx, masterPortIter, syncFwdDelay,
        &typeSpecHdr, portInfoHdrPtr) == E_OK)
      {
        EthTSyn_SetFupFwdStateOfBridgeTxMasterPortDyn(masterPortIter,
          ETHTSYN_FINISHED_FUPFWDSTATEOFBRIDGETXMASTERPORTDYN);

        /* Reset the SyncFwdDelay to have a clear defined value after successful transmission. */
        EthTSyn_SetSyncFwdDelayNsOfBridgeTxMasterPortDyn(masterPortIter, 0u);
      }
      else
      {
        fupTransmittedOnAllMasterPort = FALSE;
      }
    }
#  if ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) && (ETHTSYN_SWT_RES_TIME_NS == 0u) )
    /* In a transparent clock configuration with calculated switch residence time (i.e., SiteSyncSyncSm is used and
     * ETHTSYN_SWT_RES_TIME_NS == 0u), it could happen that the Sync egress timestamp is not available yet.
     * In this case, the FupFwdState of the port remains CALC_SYNC_FWD_DELAY. A retry is required.
     * Hence, mark FollowUp as not transmitted on all master ports in this case. */
    else if (EthTSyn_GetFupFwdStateOfBridgeTxMasterPortDyn(masterPortIter) ==
      ETHTSYN_CALC_SYNC_FWD_DELAY_FUPFWDSTATEOFBRIDGETXMASTERPORTDYN)
    {
      fupTransmittedOnAllMasterPort = FALSE;
    }
    else
    {
      /* Nothing to do. */
    }
#  endif /* ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) && (ETHTSYN_SWT_RES_TIME_NS == 0u) ) */
  }

  /* #30 Handle the FollowUp transmission retry. */
  return EthTSyn_SiteSyncSyncTx_HndlMsgTxRetry(SiteSyncSyncTxIdx, fupTransmittedOnAllMasterPort);
} /* EthTSyn_SiteSyncSyncTx_TxFup() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_HndlCalcSyncFwdDelayStateOfTxBridgeMasterPort
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_HndlCalcSyncFwdDelayStateOfTxBridgeMasterPort(
                  EthTSyn_BridgeTxMasterPortIterType MasterPortIdx,
                  uint64                             FupCorrField,
                  EthTSyn_PdelayType                 SlavePdelayNs,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)       SyncIngrTsPtr)
{
  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_SWT_RES_TIME_NS == 0u)
#   if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
  const EthTSyn_PortSyncSendSmIdxOfBridgeTxMasterPortType portSyncSendSmIdx =
    EthTSyn_GetPortSyncSendSmIdxOfBridgeTxMasterPort(MasterPortIdx);
  EthTSyn_TsInfoType syncEgrTsInfo = {0};
  if (EthTSyn_PortSyncSendSm_GetSyncEgrTs(portSyncSendSmIdx, &syncEgrTsInfo) == E_OK)
#   endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */
  {
#   if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
    const EthTSyn_TsMgmtType syncEgrTsMgmt = syncEgrTsInfo.SwtEgrTsMgmt;
#   else
    const EthTSyn_TsMgmtType syncEgrTsMgmt = EthTSyn_GetSyncEgrTsMgmtOfBridgeTxMasterPortDyn(MasterPortIdx);
    EthTSyn_Ts_ResetTsMgmt(EthTSyn_GetAddrSyncEgrTsMgmtOfBridgeTxMasterPortDyn(MasterPortIdx));
#   endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

    const uint64 syncFwdDelay = EthTSyn_SiteSyncSyncTx_CalcSyncFwdDelay(SlavePdelayNs, SyncIngrTsPtr, &syncEgrTsMgmt);

    EthTSyn_SiteSyncSyncTx_UpdateSyncFwdDelayAndFupFwdState(MasterPortIdx, syncFwdDelay, FupCorrField);
  }
#  else
  const uint64 syncFwdDelay = ETHTSYN_SWT_RES_TIME_NS + (uint64)SlavePdelayNs;

  EthTSyn_SiteSyncSyncTx_UpdateSyncFwdDelayAndFupFwdState(MasterPortIdx, syncFwdDelay, FupCorrField);

  ETHTSYN_DUMMY_STATEMENT(SyncIngrTsPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */
} /* EthTSyn_SiteSyncSyncTx_HndlCalcSyncFwdDelayStateOfTxBridgeMasterPort() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_IsSyncFwdDelayValid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_IsSyncFwdDelayValid(
  uint64 SyncFwdDelay,
  uint64 FupCorrField)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean syncFwdDelayValid = FALSE;
  const uint64 updatedCorrField = FupCorrField + SyncFwdDelay;

  /* ----- Implementation ----------------------------------------------- */
  /* FupCorrField + SyncFwdDelay can never overflow because calculation is done in uint64 and the max values are:
   * FupCorrField: 2^48 - 1 (0x0000FFFFFFFFFFFFu)
   * SyncFwdDelay: 2^32 - 1 (Pdelay - 0xFFFFFFFFu) + max switch residence time (0x7FFFFFFFFFFF)
   * Hence, sum reaches a maximum value of: 0x00018000FFFFFFFD
   */
  if (updatedCorrField <= ETHTSYN_MSG_HDR_CORR_FIELD_MAX_NS)
  {
    syncFwdDelayValid = TRUE;
  }

  return syncFwdDelayValid;
} /* EthTSyn_SiteSyncSyncTx_IsSyncFwdDelayValid() */

#  if (ETHTSYN_SWT_RES_TIME_NS == 0u)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_CalcSyncFwdDelay
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint64, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_CalcSyncFwdDelay(
                  EthTSyn_PdelayType                 SlavePdelayNs,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)       SyncIngrTsPtr,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)                SyncEgrTsMgmtPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Initialize the syncFwdDelay to a value which leads to an invalid correction field to detect calculation errors. */
  uint64 syncFwdDelay = ETHTSYN_MSG_HDR_CORR_FIELD_MAX_NS + 1u;

  /* ----- Implementation ----------------------------------------------- */
  if (SyncEgrTsMgmtPtr->TsValid == TRUE)
  {
    EthTSyn_TimediffType swtResTime = 0;
    const Std_ReturnType resTimeCalcRetVal = EthTSyn_Ts_TsMinusTs(&SyncEgrTsMgmtPtr->Ts, SyncIngrTsPtr, &swtResTime);

    if ((resTimeCalcRetVal == E_OK) && (swtResTime >= 0))
    {
      syncFwdDelay = (uint64)swtResTime + (uint64)SlavePdelayNs;
    }
  }

  return syncFwdDelay;
} /* EthTSyn_SiteSyncSyncTx_CalcSyncFwdDelay() */
#  endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_UpdateSyncFwdDelayAndFupFwdState
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_UpdateSyncFwdDelayAndFupFwdState(
  EthTSyn_BridgeTxMasterPortIterType MasterPortIdx,
  uint64                             SyncFwdDelay,
  uint64                             FupCorrField)
{
  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_SiteSyncSyncTx_IsSyncFwdDelayValid(SyncFwdDelay, FupCorrField))
  {
    EthTSyn_SetSyncFwdDelayNsOfBridgeTxMasterPortDyn(MasterPortIdx, SyncFwdDelay);
    EthTSyn_SetFupFwdStateOfBridgeTxMasterPortDyn(MasterPortIdx,
      ETHTSYN_PENDING_FUPFWDSTATEOFBRIDGETXMASTERPORTDYN);
  }
  else
  {
    EthTSyn_SetFupFwdStateOfBridgeTxMasterPortDyn(MasterPortIdx,
      ETHTSYN_FINISHED_FUPFWDSTATEOFBRIDGETXMASTERPORTDYN);
  }
} /* EthTSyn_SiteSyncSyncTx_UpdateSyncFwdDelayAndFupFwdState() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_TxFupOnMasterPort
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
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_TxFupOnMasterPort(
                  EthTSyn_SiteSyncSyncTxIterType     SiteSyncSyncTxIdx,
                  EthTSyn_BridgeTxMasterPortIterType MasterPortIdx,
                  uint64                             SyncFwdDelay,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType)        TypeSpecHdrPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType)        PortInfoHdrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_MsgHdrTypeSpecType typeSpecHdrLoc = *TypeSpecHdrPtr;
  EthTSyn_EthTxBufferStructType txBufMgmt = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Update the FollowUp correction field by adding the Sync forward delay. */
  /* CorrFieldNs has a max value of (2^48 - 1), SyncFwdDelay has a max value of sint64_max (max switch residence time)
   * + (2^32 - 1) (max Pdelay).
   * Sum off these values always fits into uint64.
   * EthTSyn_SiteSyncSyncTx_TxFup already verified that the resulting correction field fits in the message. */
  typeSpecHdrLoc.CorrectionFieldNs += SyncFwdDelay;

  /*
   * #20 Try to start the FollowUp transmission sequence.
   *       Try to copy the FollowUp payload to the transmission buffer.
   *         If required, update the time secured CRC of the FollowUp message (if message compliance is disabled).
   *         Finish the FollowUp transmission sequence.
   */
  if (EthTSyn_SiteSyncSyncTx_StartTxFupSequence(MasterPortIdx, &typeSpecHdrLoc, PortInfoHdrPtr, &txBufMgmt) == E_OK)
  {
    /*@ assert $external(txBufMgmt.BufferPtr); */                                                                      /* VCA_ETHTSYN_TX_BUF_VALID */
    if (EthTSyn_SiteSyncSyncTx_CopyFupPayload(SiteSyncSyncTxIdx, &txBufMgmt) == E_OK)
    {
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
      EthTSyn_SiteSyncSyncTx_FupUpdateCrc(SiteSyncSyncTxIdx, &txBufMgmt);
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

      retVal = EthTSyn_SiteSyncSyncTx_FinishTxFupSequence(MasterPortIdx, &txBufMgmt, typeSpecHdrLoc.MessageLength);
    }
  }

  return retVal;
} /* EthTSyn_SiteSyncSyncTx_TxFupOnMasterPort() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_StartTxFupSequence
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_StartTxFupSequence(
                  EthTSyn_BridgeTxMasterPortIterType MasterPortIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType)        TypeSpecHdrPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType)        PortInfoHdrPtr,
    ETHTSYN_P2VAR(EthTSyn_EthTxBufferStructType)     TxBufMgmtPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
  const EthTSyn_PortSyncSendSmIdxOfBridgeTxMasterPortType portSyncSendSmIdx =
    EthTSyn_GetPortSyncSendSmIdxOfBridgeTxMasterPort(MasterPortIdx);

  retVal = EthTSyn_PortSyncSendSm_StartTxFupSequence(portSyncSendSmIdx, TypeSpecHdrPtr, PortInfoHdrPtr, TxBufMgmtPtr);
#  else
  const EthTSyn_ComCtrlPortIdxOfBridgeTxMasterPortType comCtrlPortIdx =
    EthTSyn_GetComCtrlPortIdxOfBridgeTxMasterPort(MasterPortIdx);

  retVal = EthTSyn_Tx_StartTxSequence(comCtrlPortIdx, EthTSyn_GetSizeOfTxConfContext(), TypeSpecHdrPtr,
    PortInfoHdrPtr, TxBufMgmtPtr);
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

  return retVal;
} /* EthTSyn_SiteSyncSyncTx_StartTxFupSequence() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_CopyFupPayload
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_CopyFupPayload(
                  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx,
  ETHTSYN_P2CONST(EthTSyn_EthTxBufferStructType) TxBufMgmtPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  ETHTSYN_P2VAR(uint8) payloadDestPtr = &TxBufMgmtPtr->BufferPtr[ETHTSYN_MSG_HDR_LEN];
  uint16 payloadDestLen = TxBufMgmtPtr->Length - ETHTSYN_MSG_HDR_LEN;

  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
  const EthTSyn_SiteSyncSyncSmIdxOfSiteSyncSyncTxType siteSyncSyncSmIdx =
    EthTSyn_GetSiteSyncSyncSmIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);

  retVal = EthTSyn_SiteSyncSyncSm_CopyFupPayload(siteSyncSyncSmIdx, payloadDestPtr, payloadDestLen);
#  else
  const EthTSyn_AsymSiteSyncSyncSmIdxOfSiteSyncSyncTxType asymSiteSyncSyncSmIdx =
    EthTSyn_GetAsymSiteSyncSyncSmIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);

  retVal = EthTSyn_AsymSiteSyncSyncSm_CopyFupPayload(asymSiteSyncSyncSmIdx, payloadDestPtr, payloadDestLen);
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

  return retVal;
} /* EthTSyn_SiteSyncSyncTx_CopyFupPayload() */

#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_FupUpdateCrc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_FupUpdateCrc(
                  EthTSyn_SiteSyncSyncTxIterType SiteSyncSyncTxIdx,
  ETHTSYN_P2CONST(EthTSyn_EthTxBufferStructType) TxBufMgmtPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_IsTimeSecuredCrcUpdateRequiredOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx))
  {
    const EthTSyn_CrcHndl_IdxType crcHndlIdx = EthTSyn_GetCrcHndlIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
    const boolean invalidateCrc = (EthTSyn_IsTimeSecuredCrcValidOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx) ? FALSE : TRUE);
    const uint16 timeSecuredTlvRelOfs = EthTSyn_GetTimeSecuredTlvRelOfsOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx);

    EthTSyn_SiteSyncSyncTlv_UpdateTimeTlvCrc(crcHndlIdx, TxBufMgmtPtr->BufferPtr, timeSecuredTlvRelOfs, invalidateCrc);
  }

} /* EthTSyn_SiteSyncSyncTx_FupUpdateCrc() */
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_FinishTxFupSequence
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_FinishTxFupSequence(
                  EthTSyn_BridgeTxMasterPortIterType MasterPortIdx,
  ETHTSYN_P2CONST(EthTSyn_EthTxBufferStructType)     TxBufMgmtPtr,
                  uint16                             TxMsgLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
  const EthTSyn_PortSyncSendSmIdxOfBridgeTxMasterPortType portSyncSendSmIdx =
    EthTSyn_GetPortSyncSendSmIdxOfBridgeTxMasterPort(MasterPortIdx);

  retVal = EthTSyn_PortSyncSendSm_FinishTxFupSequence(portSyncSendSmIdx, TxBufMgmtPtr, TxMsgLen);
#  else
  const EthTSyn_ComCtrlPortIdxOfBridgeTxMasterPortType comCtrlPortIdx =
    EthTSyn_GetComCtrlPortIdxOfBridgeTxMasterPort(MasterPortIdx);

  retVal = EthTSyn_Tx_FinishTxSequence(comCtrlPortIdx, EthTSyn_GetSizeOfTxConfContext(),
    TxBufMgmtPtr->BufferIdx, TxMsgLen);
  if (retVal == E_OK)
  {
    EthTSyn_PortParamStats_IncTxFupCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(comCtrlPortIdx));
  }
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

  return retVal;
} /* EthTSyn_SiteSyncSyncTx_FinishTxFupSequence() */
# endif /* ETHTSYN_NOUNIT_SITESYNCSYNCTX */

/**********************************************************************************************************************
 * Internal functions of SiteSyncSyncTlv
 *!
 * \unit SiteSyncSyncTlv
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SITESYNCSYNCTLV)
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTlv_FindTimeSecuredTlv
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTlv_FindTimeSecuredTlv (
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType) PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)              FupPayloadPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 timeSecuredTlvRelOfs = 0u;
  uint16 currSubTlvOfs = ETHTSYN_MSG_FUP_FIRST_AR_SUB_TLV_PL_REL_OFS;

  if (PtpHdrPtr->MessageLength > ETHTSYN_MSG_FUP_LEN)
  {
    /* Only valid FollowUp messages are passed to the SiteSyncSyncSm. So when MessageLength is greater than the common
     * FollowUp message length, an AUTOSAR TLV is included and all length field are valid as well. */
    const uint16 arTlvLengthField = EthTSyn_Mem_GetUint16(FupPayloadPtr, ETHTSYN_MSG_FUP_AR_TLV_HDR_LEN_REL_OFS);
    sint32 remainingLength = (sint32)((sint32)arTlvLengthField - (sint32)ETHTSYN_AR_TLV_HEADER_LENGTH_FIELD_VALUE);

    while (remainingLength > (sint32)ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH)
    {
      const uint8 currTlvType = EthTSyn_Mem_GetUint8(FupPayloadPtr, currSubTlvOfs);
      const uint8 currTlvLenFieldVal = EthTSyn_Mem_GetUint8(
        FupPayloadPtr, (currSubTlvOfs + ETHTSYN_MSG_FUP_AR_SUB_TLV_LEN_OFS));
      const uint16 currTlvLength = (uint16)ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH + (uint16)currTlvLenFieldVal;

      remainingLength -= (sint32) currTlvLength;

      if (currTlvType == ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED)
      {
        timeSecuredTlvRelOfs = currSubTlvOfs;
        break;
      }

      /* Set offset to next Sub-TLV. */
      currSubTlvOfs += currTlvLength;
    }
  }

  return timeSecuredTlvRelOfs;
} /* EthTSyn_SiteSyncSyncTlv_FindTimeSecuredTlv */
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */
# endif /* ETHTSYN_NOUNIT_SITESYNCSYNCTLV */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Public interfaces of SiteSyncSyncSm
 *!
 * \unit SiteSyncSyncSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SITESYNCSYNCSM)
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_Init (void)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_MsgHdrType zeroHdr = {0};

  /* ----- Implementation ----------------------------------------------- */
  for (EthTSyn_SiteSyncSyncSmIterType siteSyncSyncSmIdx = 0u;
       siteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
       siteSyncSyncSmIdx++)
  {
    /* Initialize the state */
    EthTSyn_SetStateOfSiteSyncSyncSmDyn(siteSyncSyncSmIdx, ETHTSYN_WAIT_SYNC_STATEOFSITESYNCSYNCSMDYN);

    /* Initialize the Sync message attributes */
    EthTSyn_Ts_ResetTsMgmt(EthTSyn_GetAddrIngrTsMgmtOfSiteSyncSyncSmSyncHndlDyn(siteSyncSyncSmIdx));
    EthTSyn_SetRcvdHdrOfSiteSyncSyncSmSyncHndlDyn(siteSyncSyncSmIdx, zeroHdr);
    EthTSyn_SetPdelayOfSiteSyncSyncSmSyncHndlDyn(siteSyncSyncSmIdx, 0u);

    /* Initialize the FollowUp message attributes */
    EthTSyn_SetRcvdHdrOfSiteSyncSyncSmFupHndlDyn(siteSyncSyncSmIdx, zeroHdr);
    EthTSyn_SetFwdPendingOfSiteSyncSyncSmFupHndlDyn(siteSyncSyncSmIdx, FALSE);

    /* Initialize parameters only present with enabled switch residence time calculations
     * (required for handling of the Sync timeout) */
#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
    EthTSyn_SetSyncRxTimeoutCntOfSiteSyncSyncSmDyn(siteSyncSyncSmIdx, 0u);
    EthTSyn_SetOperationModeOfSiteSyncSyncSmDyn(siteSyncSyncSmIdx, ETHTSYN_REGULAR_OPERATIONMODEOFSITESYNCSYNCSMDYN);
    EthTSyn_SetSyncCycleCntOfSiteSyncSyncSmDyn(siteSyncSyncSmIdx, 0u);
    EthTSyn_SetSyncCycleFromMsgOfSiteSyncSyncSmDyn(siteSyncSyncSmIdx, 0u);
    EthTSyn_SetSequenceIdIncOfSiteSyncSyncSmDyn(siteSyncSyncSmIdx, 0u);
    EthTSyn_Ts_ResetTsMgmt(EthTSyn_GetAddrLastValidIngrTsMgmtOfSiteSyncSyncSmSyncHndlDyn(siteSyncSyncSmIdx));
    EthTSyn_SetLastValidRcvdHdrOfSiteSyncSyncSmSyncHndlDyn(siteSyncSyncSmIdx, zeroHdr);
    EthTSyn_SetLastValidPdelayOfSiteSyncSyncSmSyncHndlDyn(siteSyncSyncSmIdx, 0u);
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */
  }

  VStdLib_MemClr(EthTSyn_GetAddrRcvdFupPayloadBufDyn(0u), EthTSyn_GetSizeOfRcvdFupPayloadBufDyn());
} /* EthTSyn_SiteSyncSyncSm_Init() */

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_MainFunction(void)
{
  /* ----- Implementation ----------------------------------------------- */
#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
  for (EthTSyn_SiteSyncSyncSmIterType siteSyncSyncSmIdx = 0u;
       siteSyncSyncSmIdx < EthTSyn_GetSizeOfSiteSyncSyncSm();
       siteSyncSyncSmIdx++)
  {
    EthTSyn_SiteSyncSyncSm_ProcEntity(siteSyncSyncSmIdx);
  }
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */
} /* EthTSyn_SiteSyncSyncSm_MainFunction() */

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_ProcRcvdSync
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_ProcRcvdSync (
                  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)              PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)              SyncSwtIngrTsMgmtPtr)
{
  /* ----- Implementation ----------------------------------------------- */
#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
  if (EthTSyn_GetOperationModeOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) ==
      ETHTSYN_CYCLIC_OPERATIONMODEOFSITESYNCSYNCSMDYN)
  {
    const EthTSyn_SiteSyncSyncTxIdxOfSiteSyncSyncSmType siteSyncSyncTxIdx =
      EthTSyn_GetSiteSyncSyncTxIdxOfSiteSyncSyncSm(SiteSyncSyncSmIdx);

    /* Recharging the timeout here is required to ensure that cyclic operation mode is entered again when no
     * corresponding FollowUp message is received. */
    EthTSyn_SetSyncRxTimeoutCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx,
      EthTSyn_GetSyncRxTimeoutOfSiteSyncSyncSm(SiteSyncSyncSmIdx));

    EthTSyn_SiteSyncSyncTx_CancelOngoingTransmissions(siteSyncSyncTxIdx);
    EthTSyn_SetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, ETHTSYN_RCVD_SYNC_AFTER_TIMEOUT_STATEOFSITESYNCSYNCSMDYN);
  }
  else
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */
  {
    EthTSyn_SetRcvdHdrOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx, (*PtpHdrPtr));
    EthTSyn_SetIngrTsMgmtOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx, (*SyncSwtIngrTsMgmtPtr));
    EthTSyn_SetPdelayOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx,
      EthTSyn_ComCtrl_GetPdelay(EthTSyn_GetComCtrlPortSlaveIdxOfSiteSyncSyncSm(SiteSyncSyncSmIdx)));

    /* Corresponding FollowUp message needs to be received after the Sync. So reset the FollowUp forward pending flag. */
    EthTSyn_SetFwdPendingOfSiteSyncSyncSmFupHndlDyn(SiteSyncSyncSmIdx, FALSE);

    /* Sync-Timeout is not recharged here because it is used to detect incomplete Sync/FollowUp cycles and should only
     * be started after the first valid Sync/FollowUp message pair was received. */
    EthTSyn_SiteSyncSyncSm_EnterTxSyncState(SiteSyncSyncSmIdx);
  }
} /* EthTSyn_SiteSyncSyncSm_ProcRcvdSync() */

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_ProcRcvdFup
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
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_ProcRcvdFup (
                  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)              PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                           FupPayloadPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if residence time calculation is enabled and SiteSyncSyncSm is in cyclic operation mode.
   *       Check if SiteSyncSyncSm is in state RCVD_SYNC_AFTER_TIMEOUT.
   *       Re-charge the Sync reception timeout counter.
   *       Reset the sequence id increment.
   *       Enter regular operation mode.
   *       Change state of the SiteSyncSyncSm to WAIT_SYNC. */
#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
  if (EthTSyn_GetOperationModeOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) ==
      ETHTSYN_CYCLIC_OPERATIONMODEOFSITESYNCSYNCSMDYN)
  {
    if (EthTSyn_GetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) ==
        ETHTSYN_RCVD_SYNC_AFTER_TIMEOUT_STATEOFSITESYNCSYNCSMDYN)
    {
      /* Sync-Timeout was already recharged upon Sync reception. However, it is recharged here again to ensure a
       * consistent handling when the regular operation mode is entered again. */
      EthTSyn_SetSyncRxTimeoutCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx,
        EthTSyn_GetSyncRxTimeoutOfSiteSyncSyncSm(SiteSyncSyncSmIdx));
      EthTSyn_SetSequenceIdIncOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, 0u);
      EthTSyn_SetOperationModeOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, ETHTSYN_REGULAR_OPERATIONMODEOFSITESYNCSYNCSMDYN);
      EthTSyn_SetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, ETHTSYN_WAIT_SYNC_STATEOFSITESYNCSYNCSMDYN);
    }
  }
  else
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */
  /* #20 Otherwise.
   *       Check if FollowUp payload can be stored in the corresponding SiteSyncSyncSm buffer.
   *         Store the FollowUp for later forwarding.
   *         Store the last received Sync messages as last valid Sync message and re-charge the Sync reception
   *         timeout (if switch residence time calculation is enabled).
   *       Check if SiteSyncSyncSm is in state WAIT_FUP.
   *         Change state of the SiteSyncSyncSm to TX_FUP. */
  {
    const uint16 fupPayloadLen = (PtpHdrPtr->MessageLength - ETHTSYN_MSG_HDR_LEN);

    if (EthTSyn_GetRcvdFupPayloadBufDynLengthOfSiteSyncSyncSm(SiteSyncSyncSmIdx) >= fupPayloadLen)
    {
      EthTSyn_SetRcvdHdrOfSiteSyncSyncSmFupHndlDyn(SiteSyncSyncSmIdx, (*PtpHdrPtr));
      EthTSyn_SetFwdPendingOfSiteSyncSyncSmFupHndlDyn(SiteSyncSyncSmIdx, TRUE);
      VStdLib_MemCpy_s(
        EthTSyn_GetAddrRcvdFupPayloadBufDyn(
        EthTSyn_GetRcvdFupPayloadBufDynStartIdxOfSiteSyncSyncSm(SiteSyncSyncSmIdx)),
        EthTSyn_GetRcvdFupPayloadBufDynLengthOfSiteSyncSyncSm(SiteSyncSyncSmIdx),
        FupPayloadPtr,
        fupPayloadLen);

#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
      EthTSyn_SetLastValidRcvdHdrOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx,
        EthTSyn_GetRcvdHdrOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx));
      EthTSyn_SetLastValidIngrTsMgmtOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx,
        EthTSyn_GetIngrTsMgmtOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx));
      EthTSyn_SetLastValidPdelayOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx,
        EthTSyn_GetPdelayOfSiteSyncSyncSmSyncHndlDyn(SiteSyncSyncSmIdx));
      /* Recharge Sync-Timeout here because it is used to track invalid/incomplete Sync/FollowUp message pairs and
       * not only related to the Sync message. */
      EthTSyn_SetSyncRxTimeoutCntOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx,
        EthTSyn_GetSyncRxTimeoutOfSiteSyncSyncSm(SiteSyncSyncSmIdx));
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */

      if (EthTSyn_GetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) == ETHTSYN_WAIT_FUP_STATEOFSITESYNCSYNCSMDYN)
      {
        EthTSyn_SiteSyncSyncSm_EnterTxFupState(SiteSyncSyncSmIdx);
      }
    }
  }
} /* EthTSyn_SiteSyncSyncSm_ProcRcvdFup() */

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_TxSyncFinished
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_TxSyncFinished(
  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_StateOfSiteSyncSyncSmDynType currSiteSyncSyncSmState =
    EthTSyn_GetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx);

  /* ----- Implementation ----------------------------------------------- */
#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
  if (EthTSyn_GetOperationModeOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) ==
      ETHTSYN_CYCLIC_OPERATIONMODEOFSITESYNCSYNCSMDYN)
  {
    if (currSiteSyncSyncSmState == ETHTSYN_TX_SYNC_STATEOFSITESYNCSYNCSMDYN)
    {
      EthTSyn_SiteSyncSyncSm_EnterTxFupState(SiteSyncSyncSmIdx);
    }
  }
  else
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */
  {
    if (currSiteSyncSyncSmState == ETHTSYN_TX_SYNC_STATEOFSITESYNCSYNCSMDYN)
    {
      if (EthTSyn_IsFwdPendingOfSiteSyncSyncSmFupHndlDyn(SiteSyncSyncSmIdx))
      {
        EthTSyn_SiteSyncSyncSm_EnterTxFupState(SiteSyncSyncSmIdx);
      }
      else
      {
        EthTSyn_SetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, ETHTSYN_WAIT_FUP_STATEOFSITESYNCSYNCSMDYN);
      }
    }
  }
}

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_GetSequenceIdIncrement
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(uint16, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_GetSequenceIdIncrement(
  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 seqIdInc = 0u;

  /* ----- Implementation ----------------------------------------------- */
#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
  seqIdInc = EthTSyn_GetSequenceIdIncOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx);
#   else
  ETHTSYN_DUMMY_STATEMENT(SiteSyncSyncSmIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */

  return seqIdInc;
}

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_CopyFupPayload
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_CopyFupPayload(
                EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx,
  ETHTSYN_P2VAR(uint8)                           PayloadDestPtr,
                uint16                           PayloadDestLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_GetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) == ETHTSYN_TX_FUP_STATEOFSITESYNCSYNCSMDYN)
  {
    const uint16 fupPayloadLength = EthTSyn_GetRcvdHdrOfSiteSyncSyncSmFupHndlDyn(SiteSyncSyncSmIdx).MessageLength -
      ETHTSYN_MSG_HDR_LEN;

    if (PayloadDestLen >= fupPayloadLength)
    {
      ETHTSYN_CONSTP2CONST(uint8) fupPayloadPtr = EthTSyn_GetAddrRcvdFupPayloadBufDyn(
        EthTSyn_GetRcvdFupPayloadBufDynStartIdxOfSiteSyncSyncSm(SiteSyncSyncSmIdx));

      VStdLib_MemCpy_s(PayloadDestPtr, PayloadDestLen, fupPayloadPtr, fupPayloadLength);

      retVal = E_OK;
    }
  }

  return retVal;
} /* EthTSyn_SiteSyncSyncSm_CopyFupPayload() */

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncSm_TxFupFinished
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncSm_TxFupFinished(
  EthTSyn_SiteSyncSyncSm_SmIdxType SiteSyncSyncSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_GetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) == ETHTSYN_TX_FUP_STATEOFSITESYNCSYNCSMDYN)
  {
#   if (ETHTSYN_SWT_RES_TIME_NS == 0u)
    if (EthTSyn_GetOperationModeOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx) ==
        ETHTSYN_CYCLIC_OPERATIONMODEOFSITESYNCSYNCSMDYN)
    {
      EthTSyn_SetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, ETHTSYN_WAIT_TX_SYNC_STATEOFSITESYNCSYNCSMDYN);
    }
    else
#   endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */
    {
      EthTSyn_SetStateOfSiteSyncSyncSmDyn(SiteSyncSyncSmIdx, ETHTSYN_WAIT_SYNC_STATEOFSITESYNCSYNCSMDYN);
    }

    EthTSyn_SetFwdPendingOfSiteSyncSyncSmFupHndlDyn(SiteSyncSyncSmIdx, FALSE);
  }
}

#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */
# endif /* ETHTSYN_NOUNIT_SITESYNCSYNCSM */

/**********************************************************************************************************************
 * Public interfaces of AsymSiteSyncSyncSm
 *!
 * \unit AsymSiteSyncSyncSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_ASYMSITESYNCSYNCSM)
#  if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  EthTSyn_AsymSiteSyncSyncSm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_AsymSiteSyncSyncSm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_MsgHdrType initMsgHdr = {0};

  /* ----- Implementation ----------------------------------------------- */
  for (EthTSyn_AsymSiteSyncSyncSmIterType asymSiteSyncSyncSmIdx = 0u;
       asymSiteSyncSyncSmIdx < EthTSyn_GetSizeOfAsymSiteSyncSyncSm();
       asymSiteSyncSyncSmIdx++)
  {
    EthTSyn_SetStateOfAsymSiteSyncSyncSmDyn(asymSiteSyncSyncSmIdx, ETHTSYN_WAIT_SYNC_STATEOFASYMSITESYNCSYNCSMDYN);
    EthTSyn_SetRcvdHdrOfAsymSiteSyncSyncSmFupHndlDyn(asymSiteSyncSyncSmIdx, initMsgHdr);
  }
} /* EthTSyn_AsymSiteSyncSyncSm_Init() */

/**********************************************************************************************************************
 *  EthTSyn_AsymSiteSyncSyncSm_SyncRcvd
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
FUNC(void, ETHTSYN_CODE) EthTSyn_AsymSiteSyncSyncSm_SyncRcvd(
                  EthTSyn_AsymSiteSyncSyncSm_SmIdxType AsymSiteSyncSyncSmIdx,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)                  SyncSwtIngrTsMgmtPtr,
  ETHTSYN_P2CONST(EthSwt_VMgmtObjectAddInfoType)       SyncAddInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_SiteSyncSyncTxIdxOfAsymSiteSyncSyncSmType siteSyncSyncTxIdx =
    EthTSyn_GetSiteSyncSyncTxIdxOfAsymSiteSyncSyncSm(AsymSiteSyncSyncSmIdx);

  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_GetStateOfAsymSiteSyncSyncSmDyn(AsymSiteSyncSyncSmIdx) == ETHTSYN_FWD_FUP_STATEOFASYMSITESYNCSYNCSMDYN)
  {
    EthTSyn_SiteSyncSyncTx_CancelOngoingTransmissions(siteSyncSyncTxIdx);
  }

  if (SyncSwtIngrTsMgmtPtr->TsValid == TRUE)
  {
    const EthTSyn_PdelayType slavePortPdelay =
      EthTSyn_ComCtrl_GetPdelay(EthTSyn_GetComCtrlPortSlaveIdxOfAsymSiteSyncSyncSm(AsymSiteSyncSyncSmIdx));

    EthTSyn_SiteSyncSyncTx_ProvideSyncTimestamps(siteSyncSyncTxIdx, slavePortPdelay, SyncSwtIngrTsMgmtPtr,
      SyncAddInfoPtr->EgrTsInfoPtr, SyncAddInfoPtr->NumEgrTsInfos);

    EthTSyn_SetStateOfAsymSiteSyncSyncSmDyn(AsymSiteSyncSyncSmIdx, ETHTSYN_WAIT_FUP_STATEOFASYMSITESYNCSYNCSMDYN);
  }
  else
  {
    EthTSyn_SetStateOfAsymSiteSyncSyncSmDyn(AsymSiteSyncSyncSmIdx, ETHTSYN_WAIT_SYNC_STATEOFASYMSITESYNCSYNCSMDYN);
  }
} /* EthTSyn_AsymSiteSyncSyncSm_SyncRcvd() */

/**********************************************************************************************************************
 *  EthTSyn_AsymSiteSyncSyncSm_ProcRcvdFup
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
FUNC(void, ETHTSYN_CODE) EthTSyn_AsymSiteSyncSyncSm_ProcRcvdFup(
                  EthTSyn_AsymSiteSyncSyncSm_SmIdxType AsymSiteSyncSyncSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                  PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                               FupPayloadPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if AsymSiteSyncSyncSm is in state WAIT_FUP. */
  if (EthTSyn_GetStateOfAsymSiteSyncSyncSmDyn(AsymSiteSyncSyncSmIdx) == ETHTSYN_WAIT_FUP_STATEOFASYMSITESYNCSYNCSMDYN)
  {
    const uint16 fupPayloadLen = PtpHdrPtr->MessageLength - ETHTSYN_MSG_HDR_LEN;

    /* #20 Check if FollowUp payload buffer of the AsymSiteSyncSyncSm is big enough to hold the FollowUp payload. */
    if (fupPayloadLen <=
      EthTSyn_GetAsymSiteSyncSyncSmFupPayloadBufDynLengthOfAsymSiteSyncSyncSm(AsymSiteSyncSyncSmIdx))
    {
      /* #30 Copy the FollowUp header and payload to the AsymSiteSyncSyncSm internal buffer. */
      EthTSyn_SetRcvdHdrOfAsymSiteSyncSyncSmFupHndlDyn(AsymSiteSyncSyncSmIdx, (*PtpHdrPtr));

      /* PRQA S 0315 5 */ /* MD_MSR_VStdLibCopy */
      VStdLib_MemCpy_s(
        EthTSyn_GetAddrAsymSiteSyncSyncSmFupPayloadBufDyn(
          EthTSyn_GetAsymSiteSyncSyncSmFupPayloadBufDynStartIdxOfAsymSiteSyncSyncSm(AsymSiteSyncSyncSmIdx)),
        EthTSyn_GetAsymSiteSyncSyncSmFupPayloadBufDynLengthOfAsymSiteSyncSyncSm(AsymSiteSyncSyncSmIdx),
        FupPayloadPtr, fupPayloadLen);

      /* #40 Start the FollowUp transmission in the SiteSyncSyncTx unit.
       *       Change the state of the AsymSiteSyncSyncSm to FWD_FUP. */
      (void)EthTSyn_SiteSyncSyncTx_StartFupTransmission(
        EthTSyn_GetSiteSyncSyncTxIdxOfAsymSiteSyncSyncSm(AsymSiteSyncSyncSmIdx), PtpHdrPtr, FupPayloadPtr, FALSE);

      EthTSyn_SetStateOfAsymSiteSyncSyncSmDyn(AsymSiteSyncSyncSmIdx, ETHTSYN_FWD_FUP_STATEOFASYMSITESYNCSYNCSMDYN);
    }
    /* #21 Otherwise, change state of the AsymSiteSyncSyncSm to WAIT_SYNC. */
    else
    {
      EthTSyn_SetStateOfAsymSiteSyncSyncSmDyn(AsymSiteSyncSyncSmIdx, ETHTSYN_WAIT_SYNC_STATEOFASYMSITESYNCSYNCSMDYN);
    }
  }
} /* EthTSyn_AsymSiteSyncSyncSm_ProcRcvdFup() */

/**********************************************************************************************************************
 *  EthTSyn_AsymSiteSyncSyncSm_CopyFupPayload
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_AsymSiteSyncSyncSm_CopyFupPayload(
                EthTSyn_AsymSiteSyncSyncSm_SmIdxType AsymSiteSyncSyncSmIdx,
  ETHTSYN_P2VAR(uint8)                               PayloadDestPtr,
                uint16                               PayloadDestLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_GetStateOfAsymSiteSyncSyncSmDyn(AsymSiteSyncSyncSmIdx) == ETHTSYN_FWD_FUP_STATEOFASYMSITESYNCSYNCSMDYN)
  {
    const uint16 fupPayloadLen =
      EthTSyn_GetRcvdHdrOfAsymSiteSyncSyncSmFupHndlDyn(AsymSiteSyncSyncSmIdx).MessageLength - ETHTSYN_MSG_HDR_LEN;

    if (PayloadDestLen >= fupPayloadLen)
    {
      ETHTSYN_CONSTP2CONST(uint8) fupPayloadPtr = EthTSyn_GetAddrAsymSiteSyncSyncSmFupPayloadBufDyn(
        EthTSyn_GetAsymSiteSyncSyncSmFupPayloadBufDynStartIdxOfAsymSiteSyncSyncSm(AsymSiteSyncSyncSmIdx));

      VStdLib_MemCpy_s(PayloadDestPtr, PayloadDestLen, fupPayloadPtr, fupPayloadLen);

      retVal = E_OK;
    }
  }

  return retVal;
} /* EthTSyn_AsymSiteSyncSyncSm_CopyFupPayload() */

/**********************************************************************************************************************
 *  EthTSyn_AsymSiteSyncSyncSm_TxFupFinished
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_AsymSiteSyncSyncSm_TxFupFinished(
  EthTSyn_AsymSiteSyncSyncSm_SmIdxType AsymSiteSyncSyncSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  if (EthTSyn_GetStateOfAsymSiteSyncSyncSmDyn(AsymSiteSyncSyncSmIdx) == ETHTSYN_FWD_FUP_STATEOFASYMSITESYNCSYNCSMDYN)
  {
    EthTSyn_SetStateOfAsymSiteSyncSyncSmDyn(AsymSiteSyncSyncSmIdx, ETHTSYN_WAIT_SYNC_STATEOFASYMSITESYNCSYNCSMDYN);
  }
} /* EthTSyn_AsymSiteSyncSyncSm_TxFupFinished() */
#  endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */
# endif /* ETHTSYN_NOUNIT_ASYMSITESYNCSYNCSM */

/**********************************************************************************************************************
 * Public interfaces of SiteSyncSyncTx
 *!
 * \unit SiteSyncSyncTx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SITESYNCSYNCTX)
/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncTx_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_MsgHdrType initMsgHdr = {0};
  const EthTSyn_TsMgmtType initTsMgmt = {0};
  EthTSyn_SiteSyncSyncTxIterType siteSyncSyncTxIter = 0u;
  EthTSyn_BridgeTxMasterPortIterType masterPortIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for (siteSyncSyncTxIter = 0u; siteSyncSyncTxIter < EthTSyn_GetSizeOfSiteSyncSyncTxDyn(); siteSyncSyncTxIter++)
  {
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
    EthTSyn_SetTimeSecuredCrcUpdateRequiredOfSiteSyncSyncTxDyn(siteSyncSyncTxIter, FALSE);
    EthTSyn_SetTimeSecuredCrcValidOfSiteSyncSyncTxDyn(siteSyncSyncTxIter, FALSE);
    EthTSyn_SetTimeSecuredTlvRelOfsOfSiteSyncSyncTxDyn(siteSyncSyncTxIter, 0u);
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

    EthTSyn_SetFwdRetryCntOfSiteSyncSyncTxDyn(siteSyncSyncTxIter, 0u);
    EthTSyn_SetLastMsgHdrOfSiteSyncSyncTxDyn(siteSyncSyncTxIter, initMsgHdr);
    EthTSyn_SetLastSyncIngrTsMgmtOfSiteSyncSyncTxDyn(siteSyncSyncTxIter, initTsMgmt);
    EthTSyn_SetSlavePdelayNsOfSiteSyncSyncTxDyn(siteSyncSyncTxIter, 0u);
    EthTSyn_SetStateOfSiteSyncSyncTxDyn(siteSyncSyncTxIter, ETHTSYN_IDLE_STATEOFSITESYNCSYNCTXDYN);
  }

  for (masterPortIter = 0u; masterPortIter < EthTSyn_GetSizeOfBridgeTxMasterPortDyn(); masterPortIter++)
  {
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
    EthTSyn_SetSyncFwdStateOfBridgeTxMasterPortDyn(masterPortIter, ETHTSYN_NONE_SYNCFWDSTATEOFBRIDGETXMASTERPORTDYN);
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

#  if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
    EthTSyn_SetSyncEgrTsMgmtOfBridgeTxMasterPortDyn(masterPortIter, initTsMgmt);
#  endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */

    EthTSyn_SetFupFwdStateOfBridgeTxMasterPortDyn(masterPortIter, ETHTSYN_NONE_FUPFWDSTATEOFBRIDGETXMASTERPORTDYN);
    EthTSyn_SetSyncFwdDelayNsOfBridgeTxMasterPortDyn(masterPortIter, 0u);
  }
} /* EthTSyn_SiteSyncSyncTx_Init() */

/**********************************************************************************************************************
 *  EthTSyn_SiteSyncSyncTx_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SiteSyncSyncTxIterType siteSyncSyncTxIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  for (siteSyncSyncTxIter = 0u; siteSyncSyncTxIter < EthTSyn_GetSizeOfSiteSyncSyncTx(); siteSyncSyncTxIter++)
  {
    EthTSyn_SiteSyncSyncTx_ProcEntity(siteSyncSyncTxIter);
  }
} /* EthTSyn_SiteSyncSyncTx_MainFunction() */

#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_StartSyncTransmission
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_StartSyncTransmission(
                  EthTSyn_SiteSyncSyncTx_IdxType SiteSyncSyncTxIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)            SyncMsgHdrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_BridgeTxMasterPortDynIterType masterPortIter= 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all mapped bridge master ports.
   *       Cancel any potential ongoing Sync cycle in the PortSendSendSm unit.
   *       Reset the FollowUp forward state to NONE.
   *       Set the Sync forward state to PENDING.
   */
  for (masterPortIter = EthTSyn_GetBridgeTxMasterPortStartIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
       masterPortIter < EthTSyn_GetBridgeTxMasterPortEndIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
       masterPortIter++)
  {
    EthTSyn_PortSyncSendSm_CancelCycle(EthTSyn_GetPortSyncSendSmIdxOfBridgeTxMasterPort(masterPortIter));

    EthTSyn_SetFupFwdStateOfBridgeTxMasterPortDyn(masterPortIter, ETHTSYN_NONE_FUPFWDSTATEOFBRIDGETXMASTERPORTDYN);
    EthTSyn_SetSyncFwdStateOfBridgeTxMasterPortDyn(masterPortIter,
      ETHTSYN_PENDING_SYNCFWDSTATEOFBRIDGETXMASTERPORTDYN);
  }

  /* #20 Reset the time secured TLV information (if message compliance is disabled). */
#   if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
  EthTSyn_SetTimeSecuredCrcUpdateRequiredOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, FALSE);
  EthTSyn_SetTimeSecuredCrcValidOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, FALSE);
  EthTSyn_SetTimeSecuredTlvRelOfsOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, 0u);
#   endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

  /* #30 Store the Sync message header for later processing, reset the message forward retry counter, the last
   *     provided Sync ingress timestamp management and the stored slave Pdelay.
   *     Change state of the SiteSyncSyncTx entity to TX_SYNC.
   */
  EthTSyn_SetLastMsgHdrOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, *SyncMsgHdrPtr);
  EthTSyn_SetFwdRetryCntOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, 0u);
  EthTSyn_Ts_ResetTsMgmt(EthTSyn_GetAddrLastSyncIngrTsMgmtOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx));
  EthTSyn_SetSlavePdelayNsOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, 0u);
  EthTSyn_SetStateOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, ETHTSYN_TX_SYNC_STATEOFSITESYNCSYNCTXDYN);
} /* EthTSyn_SiteSyncSyncTx_StartSyncTransmission() */
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_ProvideSyncTimestamps
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_ProvideSyncTimestamps(
                  EthTSyn_SiteSyncSyncTx_IdxType SiteSyncSyncTxIdx,
                  EthTSyn_PdelayType             SlavePdelayNs,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)            SyncIngrTsMgmtPtr,
  ETHTSYN_P2CONST(EthSwt_VMgmtEgrTsInfoType)     SyncEgrTsInfoPtr,
                  uint8                          SyncEgrTsInfoCnt)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store the provided Sync ingress timestamp management and pdelay for later usage. */
  EthTSyn_SetLastSyncIngrTsMgmtOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, *SyncIngrTsMgmtPtr);
  EthTSyn_SetSlavePdelayNsOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, SlavePdelayNs);

#  if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
  /* #20 Check if any Sync egress timestamp is provided (if AsymSiteSyncSyncSm is enabled).
   *       Try to assign each provided Sync egress timestamp to a mapped bridge master port.
   *         Store the Sync egress timestamp management for later usage and inform the PortParamStats unit about a
   *         transmitted Sync on the bridge master port.
   */
  if ((SyncEgrTsInfoPtr != NULL_PTR) && (SyncEgrTsInfoCnt > 0u))
  {
    uint8_least syncEgrTsInfoIter = 0u;

    for (syncEgrTsInfoIter = 0u; syncEgrTsInfoIter < SyncEgrTsInfoCnt; syncEgrTsInfoIter++)
    {
      EthTSyn_BridgeTxMasterPortIterType masterPortIter = 0u;

      for (masterPortIter = EthTSyn_GetBridgeTxMasterPortStartIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
           masterPortIter < EthTSyn_GetBridgeTxMasterPortEndIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
           masterPortIter++)
      {
        const EthTSyn_ComCtrlPortIdxOfBridgeTxMasterPortType comCtrlPortIdx =
          EthTSyn_GetComCtrlPortIdxOfBridgeTxMasterPort(masterPortIter);

        if (EthTSyn_GetSwitchPortIdxOfComCtrlPort(comCtrlPortIdx) == SyncEgrTsInfoPtr[syncEgrTsInfoIter].CfgPortIdx)
        {
          EthTSyn_TsMgmtType syncEgrTsMgmt = {0};

          syncEgrTsMgmt.Ts = SyncEgrTsInfoPtr[syncEgrTsInfoIter].EgressTimestamp;
          syncEgrTsMgmt.TsValid = (boolean)(SyncEgrTsInfoPtr[syncEgrTsInfoIter].EgressTimestampValid == E_OK);

          EthTSyn_SetSyncEgrTsMgmtOfBridgeTxMasterPortDyn(masterPortIter, syncEgrTsMgmt);

          EthTSyn_PortParamStats_IncTxSyncCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(comCtrlPortIdx));

          break;
        }
      }
    }
  }
#  else
  ETHTSYN_DUMMY_STATEMENT(SyncEgrTsInfoPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(SyncEgrTsInfoCnt); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */
} /* EthTSyn_SiteSyncSyncTx_ProvideSyncTimestamps() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_StartFupTransmission
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_StartFupTransmission(
                  EthTSyn_SiteSyncSyncTx_IdxType SiteSyncSyncTxIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)            FupMsgHdrPtr,
  ETHTSYN_P2CONST(uint8)                         FupPayloadPtr,
                  boolean                        AlwaysUpdateCrc)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_BridgeTxMasterPortDynIterType masterPortIter= 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if last stored Sync ingress timestamps is valid (if calculated residence time is used). */
#  if (ETHTSYN_SWT_RES_TIME_NS == 0u)
  if (EthTSyn_GetLastSyncIngrTsMgmtOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx).TsValid == TRUE)
#  endif /* (ETHTSYN_SWT_RES_TIME_NS == 0u) */
  {
    /* #20 Iterate all mapped bridge master ports.
     *       Reset the Sync forward state to NONE (if SiteSyncSyncSm is enabled.).
     *       Set the FollowUp forward state to CALC_SYNC_FWD_DELAY.
     */
    for (masterPortIter = EthTSyn_GetBridgeTxMasterPortStartIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
         masterPortIter < EthTSyn_GetBridgeTxMasterPortEndIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
         masterPortIter++)
    {
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
      EthTSyn_SetSyncFwdStateOfBridgeTxMasterPortDyn(masterPortIter, ETHTSYN_NONE_SYNCFWDSTATEOFBRIDGETXMASTERPORTDYN);
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */
      EthTSyn_SetFupFwdStateOfBridgeTxMasterPortDyn(masterPortIter,
        ETHTSYN_CALC_SYNC_FWD_DELAY_FUPFWDSTATEOFBRIDGETXMASTERPORTDYN);
    }

    /* #30 Get and store time secured TLV information (if message compliance is disabled). */
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
#   if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
  const boolean keepSrcPortId = EthTSyn_IsKeepSrcPortIdentityOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
#   else
  const boolean keepSrcPortId = TRUE;
#   endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */
    const EthTSyn_TimeTlvInfoType timeTlvInfo = EthTSyn_SiteSyncSyncTlv_GetTimeTlvInfo(
      EthTSyn_GetCrcHndlIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx), FupMsgHdrPtr, FupPayloadPtr,
      keepSrcPortId, AlwaysUpdateCrc);

    EthTSyn_SetTimeSecuredCrcUpdateRequiredOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, timeTlvInfo.CrcUpdateRequired);
    EthTSyn_SetTimeSecuredCrcValidOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, timeTlvInfo.CrcValid);
    EthTSyn_SetTimeSecuredTlvRelOfsOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, timeTlvInfo.RelOfs);
#  else
    ETHTSYN_DUMMY_STATEMENT(FupPayloadPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
    ETHTSYN_DUMMY_STATEMENT(AlwaysUpdateCrc); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

    /* #40 Store FollowUp message header for later processing, reset the message forward retry counter and
     *     change state of the SiteSyncSyncTx entity to TX_FUP. */
    EthTSyn_SetLastMsgHdrOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, *FupMsgHdrPtr);
    EthTSyn_SetFwdRetryCntOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, 0u);
    EthTSyn_SetStateOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, ETHTSYN_TX_FUP_STATEOFSITESYNCSYNCTXDYN);

    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_SiteSyncSyncTx_StartFupTransmission() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTx_CancelOngoingTransmissions
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTx_CancelOngoingTransmissions(
  EthTSyn_SiteSyncSyncTx_IdxType SiteSyncSyncTxIdx)
{
  /* ----- Local Variables ----------------------------------------------  */
  const EthTSyn_MsgHdrType initMsgHdr = {0};
  EthTSyn_BridgeTxMasterPortIterType bridgeTxMasterPortIdx = 0;

  /* ----- Implementation ----------------------------------------------- */
  EthTSyn_SetLastMsgHdrOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, initMsgHdr);
  EthTSyn_Ts_ResetTsMgmt(EthTSyn_GetAddrLastSyncIngrTsMgmtOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx));

  for (bridgeTxMasterPortIdx = EthTSyn_GetBridgeTxMasterPortStartIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
       bridgeTxMasterPortIdx < EthTSyn_GetBridgeTxMasterPortEndIdxOfSiteSyncSyncTx(SiteSyncSyncTxIdx);
       bridgeTxMasterPortIdx++)
  {
    EthTSyn_SetSyncFwdDelayNsOfBridgeTxMasterPortDyn(bridgeTxMasterPortIdx, 0);
#  if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
    EthTSyn_SetSyncFwdStateOfBridgeTxMasterPortDyn(bridgeTxMasterPortIdx,
      ETHTSYN_NONE_SYNCFWDSTATEOFBRIDGETXMASTERPORTDYN);
#  endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */
    EthTSyn_SetFupFwdStateOfBridgeTxMasterPortDyn(bridgeTxMasterPortIdx,
      ETHTSYN_NONE_FUPFWDSTATEOFBRIDGETXMASTERPORTDYN);
  }

  EthTSyn_SetStateOfSiteSyncSyncTxDyn(SiteSyncSyncTxIdx, ETHTSYN_IDLE_STATEOFSITESYNCSYNCTXDYN);
} /* EthTSyn_SiteSyncSyncTx_CancelOngoingTransmissions() */
# endif /* ETHTSYN_NOUNIT_SITESYNCSYNCTX */

/**********************************************************************************************************************
 * Public interfaces of SiteSyncSyncTlv
 *!
 * \unit SiteSyncSyncTlv
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SITESYNCSYNCTLV)
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTlv_GetTimeTlvInfo
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
FUNC(EthTSyn_TimeTlvInfoType, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTlv_GetTimeTlvInfo(
                  EthTSyn_CrcHndl_IdxType CrcHndlIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)     FupMsgHdrPtr,
  ETHTSYN_P2CONST(uint8)                  FupPayloadPtr,
                  boolean                 KeepSrcPortId,
                  boolean                 AlwaysUpdateCrc)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeTlvInfoType tlvInfo = {0};
  const uint16 timeSecuredTlvRelOfs = EthTSyn_SiteSyncSyncTlv_FindTimeSecuredTlv(FupMsgHdrPtr, FupPayloadPtr);

  /* ----- Implementation ----------------------------------------------- */
  if (timeSecuredTlvRelOfs > 0u)
  {
    ETHTSYN_CONSTP2CONST(uint8) timeSecuredTlvStartPtr = &FupPayloadPtr[timeSecuredTlvRelOfs];
    const uint8 timeSecuredFlags =
      EthTSyn_Mem_GetUint8(timeSecuredTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_FLAGS_OFS);
    EthTSyn_ValidateSubTlvInfoType timeSecuredSubTlvInfo = {0};

    timeSecuredSubTlvInfo.PtpHdrPtr = FupMsgHdrPtr;
    timeSecuredSubTlvInfo.FupPayloadPtr = FupPayloadPtr;
    timeSecuredSubTlvInfo.SubTlvOfs = timeSecuredTlvRelOfs;

    tlvInfo.RelOfs = timeSecuredTlvRelOfs;

    if (AlwaysUpdateCrc == TRUE)
    {
      tlvInfo.CrcUpdateRequired = TRUE;
    }
    else if ((timeSecuredFlags & ETHTSYN_CRC_FLAGS_CORRECTION_FIELD_MASK) != 0u)
    {
      tlvInfo.CrcUpdateRequired = TRUE;
    }
    else if ((KeepSrcPortId == FALSE) && ((timeSecuredFlags & ETHTSYN_CRC_FLAGS_SRC_PORT_IDENTITY_MASK) != 0u))
    {
      tlvInfo.CrcUpdateRequired = TRUE;
    }
    else
    {
      tlvInfo.CrcUpdateRequired = FALSE;
    }

    if (tlvInfo.CrcUpdateRequired)
    {
      tlvInfo.CrcValid = EthTSyn_CrcHndl_CheckTimeSecuredCrc(CrcHndlIdx, &timeSecuredSubTlvInfo);
    }
  }
  else
  {
    tlvInfo.CrcUpdateRequired = FALSE;
    tlvInfo.RelOfs = 0u;
    tlvInfo.CrcValid = FALSE;
  }

  return tlvInfo;
} /* EthTSyn_SiteSyncSyncTlv_GetTimeTlvInfo() */

/**********************************************************************************************************************
 * EthTSyn_SiteSyncSyncTlv_UpdateTimeTlvCrc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SiteSyncSyncTlv_UpdateTimeTlvCrc(
                EthTSyn_CrcHndl_IdxType CrcHndlIdx,
  ETHTSYN_P2VAR(uint8)                  FupBufferPtr,
                uint16                  TimeSecuredTlvRelOfs,
                boolean                 InvalidateCrc)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeSecuredCrcType timeCrc = {0};
  const uint16 timeSecuredTlvStartOfs = ETHTSYN_MSG_HDR_LEN + TimeSecuredTlvRelOfs;
  ETHTSYN_CONSTP2VAR(uint8) timeTlvStartPtr = &FupBufferPtr[timeSecuredTlvStartOfs];
  const uint8 timeSecuredTlvFlags = EthTSyn_Mem_GetUint8(timeTlvStartPtr,
    ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_FLAGS_OFS);

  /* ----- Implementation ----------------------------------------------- */
  timeCrc = EthTSyn_CrcHndl_GetTimeSecuredCrc(CrcHndlIdx, timeSecuredTlvFlags, FupBufferPtr);

  if (InvalidateCrc)
  {
    timeCrc.Time0Crc++;
    timeCrc.Time1Crc++;
  }

  EthTSyn_Mem_PutUint8(timeTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_CRC_0_OFS, timeCrc.Time0Crc);          /* VCA_ETHTSYN_SITE_SYNC_SYNC_UPDATE_TIME_CRC */
  EthTSyn_Mem_PutUint8(timeTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_CRC_1_OFS, timeCrc.Time1Crc);          /* VCA_ETHTSYN_SITE_SYNC_SYNC_UPDATE_TIME_CRC */

} /* EthTSyn_SiteSyncSyncTlv_UpdateTimeTlvCrc() */

#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */
# endif /* ETHTSYN_NOUNIT_SITESYNCSYNCTLV */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ( (ETHTSYN_SITESYNCSYNCSM == STD_ON) || (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) ) */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SiteSyncSyncSm.c
 *********************************************************************************************************************/
