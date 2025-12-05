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
/*!        \file  EthTSyn_Types.h
 *         \unit  *
 *        \brief  EthTSyn Types header
 *      \details  Contains all type definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/


#if !defined(ETHTSYN_TYPES_H)
# define ETHTSYN_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define ETHTSYN_P2CONSTCFGROOT(Type)                                 P2CONST(Type, AUTOMATIC, ETHTSYN_CONST)
# define ETHTSYN_P2CONSTCFG(Type)                                     P2CONST(Type, AUTOMATIC, ETHTSYN_CONST)
# define ETHTSYN_CONSTP2CONSTCFG(Type)                                CONSTP2CONST(Type, AUTOMATIC, ETHTSYN_CONST)
# define ETHTSYN_P2CONST(Type)                                        P2CONST(Type, AUTOMATIC, ETHTSYN_APPL_DATA)
# define ETHTSYN_CONSTP2CONST(Type)                                   CONSTP2CONST(Type, AUTOMATIC, ETHTSYN_CONST)
# define ETHTSYN_P2VAR(Type)                                          P2VAR(Type, AUTOMATIC, ETHTSYN_APPL_VAR)
# define ETHTSYN_CONSTP2VAR(Type)                                     CONSTP2VAR(Type, AUTOMATIC, ETHTSYN_CONST)
# define ETHTSYN_P2FUNC(RType, Type)                                  P2FUNC(RType, ETHTSYN_CODE, Type)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint32 EthTSyn_IntPortIdxType;

/* EthTSyn Sync State Definition */
typedef enum EthTSyn_SyncStateEnum
{
  ETHTSYN_SYNC = 0u,
  ETHTSYN_UNSYNC = 1u,
  ETHTSYN_UNCERTAIN = 2u,
  ETHTSYN_NEVERSYNC = 3u
} EthTSyn_SyncStateType;

/* EthTSyn Transmission Mode Definition */
typedef uint8 EthTSyn_TransmissionModeType;
# define ETHTSYN_TX_OFF                                               (0x00u)
# define ETHTSYN_TX_ON                                                (0x01u)

/* Tx Call-backs */
typedef ETHTSYN_P2FUNC(void, EthTSyn_SyncSentCbkPtrType) (
                  EthTSyn_IntPortIdxType  PortIdx,
  ETHTSYN_P2CONST(uint8)                  TxBufPtr);

typedef ETHTSYN_P2FUNC(void, EthTSyn_FollowUpSentCbkPtrType) (
                  EthTSyn_IntPortIdxType  PortIdx,
  ETHTSYN_P2CONST(uint8)                  TxBufPtr);

/*! EthTSyn port parameter statistics struct definition */
typedef struct EthTSyn_PortParamStatsTypeStruct
{
  uint32 rxSyncCount;
  uint32 rxFollowUpCount;
  uint32 rxPdelayRequestCount;
  uint32 rxPdelayResponseCount;
  uint32 rxPdelayResponseFollowUpCount;
  uint32 rxAnnounceCount;
  uint32 rxPTPPacketDiscardCount;
  uint32 syncReceiptTimeoutCount;     /* This port parameter is not supported and will always be reported as "0". */
  uint32 announceReceiptTimeoutCount; /* This port parameter is not supported and will always be reported as "0". */
  uint32 pdelayAllowedLostResponsesExceededCount;
  uint32 txSyncCount;
  uint32 txFollowUpCount;
  uint32 txPdelayRequestCount;
  uint32 txPdelayResponseCount;
  uint32 txPdelayResponseFollowUpCount;
  uint32 txAnnounceCount;
} EthTSyn_PortParamStatsType;

/*! Switch Sync State Change callback function pointer type definition */
typedef ETHTSYN_P2FUNC(void, EthTSyn_SwtTimeSync_SwtSyncStateChgFctPtrType) (
  uint8                 SwtTimeSyncIdx,
  EthTSyn_SyncStateType NewSyncState);
#endif /* ETHTSYN_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Types.h
 *********************************************************************************************************************/
