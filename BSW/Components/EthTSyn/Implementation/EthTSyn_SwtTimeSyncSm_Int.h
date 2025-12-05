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
/*!        \file  EthTSyn_SwtTimeSyncSm_Int.h
 *         \unit  SwtTimeSyncSm
 *        \brief  EthTSyn internal header file of the SwtTimeSync unit.
 *      \details  Interface and type definitions of the SwtTimeSync unit for module internal usage.
 *********************************************************************************************************************/

#if !defined (ETHTSYN_SWT_TIME_SYNC_INT_H)
# define ETHTSYN_SWT_TIME_SYNC_INT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types_Int.h"

# if (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
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
 *  EthTSyn_SwtTimeSyncSm_Init
 *********************************************************************************************************************/
/*! \brief      Initializes the SwtTimeSync unit.
 *  \details    -
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_Init(void);

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_MainFunction
 *********************************************************************************************************************/
/*! \brief      Processes all SwtTimeSyncSm.
 *  \details    -
 *  \pre        Must be called cyclically in context of the EthTSyn_MainFunction()
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_MainFunction(void);

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncSendSm
 *********************************************************************************************************************/
/*! \brief      Provides all information required for switch time synchronization of a transmitted Sync message.
 *  \details    -
 *  \param[in]  SwtTimeSyncSmIdx Index of the SwtTimeSyncSm
 *  \param[in]  SequenceId       Sequence id of Sync message
 *  \param[in]  PotPtr           Precise origin timestamp
 *  \param[in]  SyncEgrTsInfoPtr Info about the egress timestamp(s) of the Sync message
 *  \pre        Must be called in context of the EthTSyn_MainFunction()
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncSendSm(
                  EthTSyn_SwtTimeSyncSm_SmIdxType SwtTimeSyncSmIdx,
                  uint16                          SequenceId,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType)    PotPtr,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType)             SyncEgrTsInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncRcvSm
 *********************************************************************************************************************/
/*! \brief      Provides all information required for switch time synchronization of a received Sync message.
 *  \details    -
 *  \param[in]  SwtTimeSyncSmIdx     Index of the SwtTimeSyncSm
 *  \param[in]  Pdelay               Current path delay
 *  \param[in]  PtpHdrPtr            Pointer to the de-serialized PTP header
 *  \param[in]  FupPayloadPtr        Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \param[in]  SyncSwtIngrTsMgmtPtr Switch ingress timestamp of the received Sync message
 *  \pre        Must be called in context of the EthTSyn_SwtMgmt_MainFunction()
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncRcvSm(
                  EthTSyn_SwtTimeSyncSm_SmIdxType SwtTimeSyncSmIdx,
                  EthTSyn_PdelayType              Pdelay,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)             PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                          FupPayloadPtr,
  ETHTSYN_P2CONST(EthTSyn_TsMgmtType)             SyncSwtIngrTsMgmtPtr);

#  define ETHTSYN_STOP_SEC_CODE
#  include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) */
#endif /* ETHTSYN_SWT_TIME_SYNC_INT_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_SwtTimeSyncSm_Int.h
 *********************************************************************************************************************/
