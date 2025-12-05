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
/*!        \file  EthTSyn_Master.c
 *        \brief  EthTSyn Master source file
 *      \details  Implementation of all units included in the logical unit Master (AnnounceSendSm, SyncSendSm,
 *                SyncSendTx and PortSyncSendSm).
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

#define ETHTSYN_MASTER_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_GeneratedCfgAccess.h"
#if (ETHTSYN_MASTER_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
# include "EthTSyn_AnnounceSendSm_Int.h"
# include "EthTSyn_SyncSendSm_Int.h"
# include "EthTSyn_SyncSendTx_Int.h"
# include "EthTSyn_PortSyncSendSm_Int.h"

# include "EthTSyn.h"
# include "EthTSyn_Lcfg.h"
# include "EthTSyn_MsgDefs_Int.h"
# include "EthTSyn_Memory_Int.h"

# include "EthTSyn_Tx_Int.h"
# include "EthTSyn_SwtMgmtTx_Int.h"
# include "EthTSyn_ComCtrl_Int.h"
# include "EthTSyn_Timestamp_Int.h"
# include "EthTSyn_CrcHndl_Int.h"
# include "EthTSyn_SwtTimeSyncSm_Int.h"
# include "EthTSyn_PortParamStats_Int.h"
# include "SchM_EthTSyn.h"
# include "StbM.h"
# include "EthIf.h"
# include "vstdlib.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Announce message payload values. */
# define ETHTSYN_ANNOUNCE_PRIORITY_1_NETWORK_INFRASTRUCTURE           (246u)
# define ETHTSYN_ANNOUNCE_PRIORITY_1_OTHER_SYSTEM                     (248u)
# define ETHTSYN_ANNOUNCE_PRIORITY_1_NOT_GM_CAPABLE                   (255u)

# define ETHTSYN_ANNOUNCE_PRIORITY_2_DEFAULT                          (248u)
# define ETHTSYN_ANNOUNCE_CLOCK_CLASS_UNKNOWN                         (248u)
# define ETHTSYN_ANNOUNCE_CLOCK_CLASS_NOT_GM_CAPABLE                  (255u)
# define ETHTSYN_ANNOUNCE_CLOCK_ACCURACY_UNKNOWN                      (254u)
# define ETHTSYN_ANNOUNCE_OFFSET_SCALED_LOG_UNKNOWN                   (0x4100u)
# define ETHTSYN_ANNOUNCE_TIME_SOURCE_INTERNAL_OSCILLATOR             (0xA0u)
# define ETHTSYN_ANNOUNCE_TLV_TYPE                                    (0x08u)
# define ETHTSYN_ANNOUNCE_TLV_LENGTH_FIELD                            (0x08u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

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
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Internal function prototypes of SyncSendSm
 *!
 * \unit SyncSendSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SYNCSENDSM)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_IntInit
 *********************************************************************************************************************/
/*! \brief        Initialize all variable data of a SyncSend state machine.
 *  \details      -
 *  \param[in]    SyncSendSmIdx SyncSend state machine.
 *                              [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_IntInit(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_CheckTimeBaseUpdateCounter
 *********************************************************************************************************************/
/*! \brief        Checks for all time base update counter if the value has changed.
 *  \details      -
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_CheckTimeBaseUpdateCounter(void);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_Proc
 *********************************************************************************************************************/
/*! \brief        Processes SyncSend state machine.
 *  \details      Resets state machine (if required), decrements Tx cycle counter and processes current state.
 *  \param[in]    SyncSendSmIdx SyncSend state machine.
 *                              [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \pre          Has to be called cyclically in context of the EthTSyn_MainFunction()
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_Proc(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_CheckAndHndlTxCycleRestart
 *********************************************************************************************************************/
/*! \brief        Checks if a restart of the current Tx cycle is required and sets new state accordingly.
 *  \details      Handles the Tx cycle counter and checks if Tx cycle counter is expired or if time base update counter
 *                has changed and transitions into state TX_CYCLIC_SYNC or TX_IMMEDIATE_SYNC, respectively.
 *                Furthermore, any potential ongoing cycle in the PortSyncSendSm is canceled upon state change.
 *  \param[in]    SyncSendSmIdx SyncSend state machine.
 *                              [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_CheckAndHndlTxCycleRestart(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcState
 *********************************************************************************************************************/
/*! \brief        Processes current state of SyncSend state machine.
 *  \details      -
 *  \param[in]    SyncSendSmIdx SyncSend state machine.
 *                              [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \pre          SyncSendSm is not in state INITIALIZING
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcState(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcInitializing
 *********************************************************************************************************************/
/*! \brief        Processes INITIALIZING state of SyncSend state machine.
 *  \details      -
 *  \param[in]    SyncSendSmIdx SyncSend state machine.
 *                              [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcInitializing(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcTxCyclicSync
 *********************************************************************************************************************/
/*! \brief        Processes TX_CYCLIC_SYNC state of SyncSend state machine.
 *  \details      Transmits Sync message and resets Tx cycle counter.
 *  \param[in]    SyncSendSmIdx SyncSend state machine.
 *                              [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcTxCyclicSync(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcTxImmediateSync
 *********************************************************************************************************************/
/*! \brief        Processes TX_IMMEDIATE_SYNC state of SyncSend state machine.
 *  \details      Transmits Sync message and resets Tx cycle counter.
 *  \param[in]    SyncSendSmIdx SyncSend state machine.
 *                              [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcTxImmediateSync(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcWaitSyncEgrTs
 *********************************************************************************************************************/
/*! \brief        Processes WAIT_SYNC_EGR_TS state of SyncSend state machine.
 *  \details      Retrieves egress timestamp(s) and calculates precise origin timestamp as well as correction field.
 *  \param[in]    SyncSendSmIdx SyncSend state machine.
 *                              [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcWaitSyncEgrTs(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcTxFup
 *********************************************************************************************************************/
/*! \brief        Processes TX_FUP state of SyncSend state machine.
 *  \details      Serializes and transmits FollowUp message.
 *  \param[in]    SyncSendSmIdx SyncSend state machine.
 *                              [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcTxFup(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_TxSync
 *********************************************************************************************************************/
/*! \brief      Transmits Sync message.
 *  \details    Retrieves and stores the current global and virtual local time tuple before transmitting.
 *  \param[in]  SyncSendSmIdx SyncSend state machine.
 *                            [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \return     E_OK - Sync message transmission successful
 *  \return     E_NOT_OK - Sync message transmission failed
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncSendSm_TxSync(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

# endif /* ETHTSYN_NOUNIT_SYNCSENDSM */

/**********************************************************************************************************************
 * Internal function prototypes of SyncSendTx
 *!
 * \unit SyncSendTx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SYNCSENDTX)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_IsFollowUpTxLengthValid
 *********************************************************************************************************************/
/*! \brief        Checks if the generated FollowUp Tx message length is valid for the SyncSend state machine.
 *  \details      -
 *  \param[in]    SyncSendSmIdx SyncSend state machine.
 *                              [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \return       TRUE - Generated FollowUp Tx message length is valid.
 *  \return       FALSE - Generated FollowUp Tx message length is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SyncSendTx_IsFollowUpTxLengthValid(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

/**********************************************************************************************************************
 * EthTSyn_SyncSendTx_GetExpectedFollowUpTxLength
 *********************************************************************************************************************/
/*! \brief        Calculates the expected FollowUp Tx message length for the SyncSend state machine.
 *  \details      -
 *  \param[in]    SyncSendSmIdx SyncSend state machine.
 *                            [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \return       Expected FollowUp Tx message length
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_GetExpectedFollowUpTxLength(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_SerializeFup
 *********************************************************************************************************************/
/*! \brief      Serializes FollowUp message and optional ArSub-Tlvs.
 *  \details    If message compliance is disabled, the message length field of the PTP header is overwritten.
 *  \param[in]  SyncSendSmIdx  SyncSend state machine.
 *                             [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \param[in]  SyncTxInfoPtr  Sync transmission time information.
 *  \param[in]  TxBufPtr       Ethernet Tx buffer.
 *  \param[in]  PotPtr         Precise origin timestamp.
 *  \return     Length of the FollowUp message
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *    requires TxBufPtr;
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_SerializeFup(
                  EthTSyn_SyncSendSmIterType   SyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_SyncTxInfoType)      SyncTxInfoPtr,
    ETHTSYN_P2VAR(uint8)                       TxBufPtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) PotPtr);

#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_AppendArFollowUpTlvs
 *********************************************************************************************************************/
/*! \brief         Appends FollowUp AUTOSAR Tlvs.
 *  \details       Appends the optional AUTOSAR (Sub)-Tlvs to an already assembled FollowUp message.
 *  \param[in]     SyncSendSmIdx  SyncSend state machine.
 *                                [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \param[in]     SyncTxInfoPtr  Sync transmission time information.
 *  \param[in,out] TxBufPtr       In:  The assembled FollowUp message in Network-Byte-Order
 *                                Out: The appended FollowUp Ar Tlvs in Network-Byte-Order
 *  \pre           The FollowUp message was already copied to the provided TxBuffer in Network-Byte-Order
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendTx_AppendArFollowUpTlvs(
                  EthTSyn_SyncSendSmIterType SyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_SyncTxInfoType)    SyncTxInfoPtr,
    ETHTSYN_P2VAR(uint8)                     TxBufPtr);

#   if (ETHTSYN_CRC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_AppendArFollowUpTimeTlv
 *********************************************************************************************************************/
/*! \brief         Appends FollowUp AUTOSAR Time TLV.
 *  \details       Appends the optional AUTOSAR Sub-Tlv TimeSecured to an already assembled FollowUp message.
 *  \param[in]     SyncSendSmIdx SyncSend state machine.
 *                               [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \param[in,out] TxBufPtr      In:  Pointer to the Ethernet transmission buffer of the FollowUp message
 *                               Out: The appended Time Sub-Tlv
 *  \param[in,out] OffsetPtr     In:  Offset in the given buffer where to store the Sub-Tlv
 *                               Out: New offset which has to be used for the next Sub-Tlv
 *  \return        0 - Time Sub-Tlv was omitted
 *  \return        other values - Length of the appended Time Sub-Tlv
 *  \pre           The FollowUp message was already copied to the provided TxBuffer in Network-Byte-Order
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_AppendArFollowUpTimeTlv(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx,
  ETHTSYN_P2VAR(uint8)       TxBufPtr,
  ETHTSYN_P2VAR(uint16)      OffsetPtr);
#   endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_AppendArFollowUpStatusTlv
 *********************************************************************************************************************/
/*! \brief         Appends FollowUp AUTOSAR Status TLV.
 *  \details       Appends the optional AUTOSAR Sub-Tlv Status (Secured) to an already assembled FollowUp message.
 *  \param[in]     SyncSendSmIdx  SyncSend state machine.
 *                                [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \param[in]     SyncTxTbStatus Time base status at the time of Sync reference time retrieval
 *  \param[in,out] TxBufPtr       In:  Pointer to the Ethernet transmission buffer of the FollowUp message
 *                                Out: The appended Status Sub-Tlv
 *  \param[in,out] OffsetPtr      In:  Offset in the given buffer where to store the Sub-Tlv
 *                                Out: New offset which has to be used for the next Sub-Tlv
 *  \return        0 - Status Sub-Tlv was omitted
 *  \return        other values - Length of the appended Status Sub-Tlv
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_AppendArFollowUpStatusTlv(
                EthTSyn_SyncSendSmIterType SyncSendSmIdx,
                StbM_TimeBaseStatusType    SyncTxTbStatus,
  ETHTSYN_P2VAR(uint8)                     TxBufPtr,
  ETHTSYN_P2VAR(uint16)                    OffsetPtr);

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_AppendArFollowUpUserDataTlv
 *********************************************************************************************************************/
/*! \brief         Appends FollowUp AUTOSAR UserData TLV.
 *  \details       Appends the optional AUTOSAR Sub-Tlv UserData (Secured) to an already assembled FollowUp message.
 *  \param[in]     SyncSendSmIdx SyncSend state machine.
 *                               [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \param[in]     UserDataPtr   The user data
 *  \param[in,out] TxBufPtr      In:  Pointer to the Ethernet transmission buffer of the FollowUp message
 *                               Out: The appended UserData Sub-Tlv
 *  \param[in,out] OffsetPtr     In:  Offset in the given buffer where to store the Sub-Tlv
 *                               Out: New offset which has to be used for the next Sub-Tlv
 *  \return        0 - UserData Sub-Tlv was omitted
 *  \return        other values - Length of the appended UserData Sub-Tlv
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_AppendArFollowUpUserDataTlv(
                EthTSyn_SyncSendSmIterType SyncSendSmIdx,
  ETHTSYN_P2CONST(StbM_UserDataType)       UserDataPtr,
    ETHTSYN_P2VAR(uint8)                   TxBufPtr,
    ETHTSYN_P2VAR(uint16)                  OffsetPtr);

#   if (ETHTSYN_MASTER_OFS_DOMAIN_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_AppendArFollowUpOfsTlvs
 *********************************************************************************************************************/
/*! \brief         Appends FollowUp AUTOSAR Ofs TLVs.
 *  \details       Appends the optional AUTOSAR Sub-Tlv(s) Ofs (Secured) to an already assembled FollowUp message.
 *  \param[in]     SyncSendSmIdx SyncSend state machine.
 *                               [range: SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm()]
 *  \param[in,out] TxBufPtr      In:  Pointer to the Ethernet transmission buffer of the FollowUp message
 *                               Out: The appended Ofs Sub-Tlv(s)
 *  \param[in,out] OffsetPtr     In:  Offset in the given buffer where to store the Sub-Tlv
 *                               Out: New offset which has to be used for the next Sub-Tlv
 *  \return        0 - Ofs Sub-Tlv was omitted
 *  \return        other values - Length of the appended Ofs Sub-Tlv(s)
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \spec
 *    requires SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_AppendArFollowUpOfsTlvs(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx,
  ETHTSYN_P2VAR(uint8)       TxBufPtr,
  ETHTSYN_P2VAR(uint16)      OffsetPtr);
#   endif /* (ETHTSYN_MASTER_OFS_DOMAIN_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_TimeBaseStatusToArSubTlvStatus
 *********************************************************************************************************************/
/*! \brief      Converts the passed StbmTimeBaseStatus into the AUTOSAR Sub-TLV status.
 *  \details    -
 *  \param[in]  StbmTimeBaseStatus  StbM time base status.
 *  \return     The passed StbmTimeBaseStatus as AUTOSAR Sub-TLV status field.
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_SyncSendTx_TimeBaseStatusToArSubTlvStatus(
  StbM_TimeBaseStatusType StbmTimeBaseStatus);
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

# endif /* ETHTSYN_NOUNIT_SYNCSENDTX */

/**********************************************************************************************************************
 * Internal function prototypes of PortSyncSendSm
 *!
 * \unit PortSyncSendSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_PORTSYNCSENDSM)
/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_IsInState
 *********************************************************************************************************************/
/*! \brief      Checks if PortSyncSend state machine is in a given state.
 *  \details    -
 *  \param[in]  PortSyncSendSmIdx  Index of the port sync send state machine.
 *                                 [range: PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm()]
 *  \param[in]  StateToCheck       State to check against.
 *  \return     TRUE - State machine is in the given state
 *  \return     FALSE - State machine is not in the given state
 *  \pre        -
 *  \context    TASK
 *  \reentrant  FALSE
 *  \spec
 *    requires PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_IsInState(
  EthTSyn_PortSyncSendSm_SmIdxType     PortSyncSendSmIdx,
  EthTSyn_StateOfPortSyncSendSmDynType StateToCheck);

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_SetState
 *********************************************************************************************************************/
/*! \brief      Sets the given state of the PortSyncSend state machine.
 *  \details    -
 *  \param[in]  PortSyncSendSmIdx  Index of the port sync send state machine.
 *                                 [range: PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm()]
 *  \param[in]  StateToSet         State to set.
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_SetState(
  EthTSyn_PortSyncSendSm_SmIdxType     PortSyncSendSmIdx,
  EthTSyn_StateOfPortSyncSendSmDynType StateToSet);

#  if (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_GetTimeValidData
 *********************************************************************************************************************/
/*! \brief      Extracts all required master time validation data from the serialized FollowUp message.
 *  \details    -
 *  \param[in]  PortSyncSendSmIdx  Index of the port sync send state machine.
 *                                 [range: PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm()]
 *  \param[in]  TxBufPtr           Pointer to the Ethernet Tx buffer (start of PTP message, i.e. header)
 *  \param[out] TimeValidDataPtr   Pointer to master time validation data (only valid if E_OK is returned)
 *  \return     E_OK - Extraction of master time validation data was successful
 *  \return     E_NOT_OK - Extraction of master time validation data failed
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires PortSyncSendSmIdx < EthTSyn_GetSizeOfPortSyncSendSm();
 *    requires TxBufPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_GetTimeValidData(
                  EthTSyn_PortSyncSendSm_SmIdxType   PortSyncSendSmIdx,
  ETHTSYN_P2CONST(uint8)                             TxBufPtr,
    ETHTSYN_P2VAR(StbM_EthTimeMasterMeasurementType) TimeValidDataPtr);
#  endif /* (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) */

# endif /* ETHTSYN_NOUNIT_PORTSYNCSENDSM */

/**********************************************************************************************************************
 * Internal function prototypes of AnnounceSendSm
 *!
 * \unit AnnounceSendSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_ANNOUNCESENDSM)
/**********************************************************************************************************************
 * EthTSyn_AnnounceSendSm_IntInit
 *********************************************************************************************************************/
/*! \brief          Initializes all variables of one AnnounceSend state machine
 *  \details        -
 *  \param[in]      AnnounceSendSmIdx  Index of the AnnounceSend state machine
 *                                     [range: AnnounceSendSmIdx < EthTSyn_GetSizeOfAnnounceSendSm()]
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires AnnounceSendSmIdx < EthTSyn_GetSizeOfAnnounceSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_IntInit(
  EthTSyn_AnnounceSendSmIterType AnnounceSendSmIdx);

/**********************************************************************************************************************
 * EthTSyn_AnnounceSendSm_Proc
 *********************************************************************************************************************/
/*! \brief          Processes the AnnounceSend state machine
 *  \details        -
 *  \param[in]      AnnounceSendSmIdx  Index of the AnnounceSend state machine
 *                                     [range: AnnounceSendSmIdx < EthTSyn_GetSizeOfAnnounceSendSm()]
 *  \pre            Has to be called in context of the EthTSyn_MainFunction() only
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires AnnounceSendSmIdx < EthTSyn_GetSizeOfAnnounceSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_Proc(
  EthTSyn_AnnounceSendSmIterType AnnounceSendSmIdx);

/**********************************************************************************************************************
 * EthTSyn_AnnounceSendSm_DecCycleTimer
 *********************************************************************************************************************/
/*! \brief          Decrements the Announce cycle timer and checks if it expired this cycle
 *  \details        -
 *  \param[in]      AnnounceSendSmIdx  Index of the AnnounceSend state machine
 *                                     [range: AnnounceSendSmIdx < EthTSyn_GetSizeOfAnnounceSendSm()]
 *  \return         TRUE - Cycle timer expired
 *  \return         FALSE - Cycle timer not expired
 *  \pre            Has to be called in context of the EthTSyn_MainFunction() only and in every cycle of the
 *                  EthTSyn_MainFunction() to ensure correct timing behavior
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires AnnounceSendSmIdx < EthTSyn_GetSizeOfAnnounceSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_DecCycleTimer(
  EthTSyn_AnnounceSendSmIterType AnnounceSendSmIdx);

/**********************************************************************************************************************
 * EthTSyn_AnnounceSendSm_HndlTxAnnounceState
 *********************************************************************************************************************/
/*! \brief          Handles the TX_ANNOUNCE state of the AnnounceSendSm
 *  \details        Tries to transmit an Announce message. Increments the sequence id, recharges the cycle timer
 *                  and changes the state of the AnnounceSendSm to WAIT_TX_ANNOUNCE in case of successful transmission.
 *  \param[in]      AnnounceSendSmIdx  Index of the AnnounceSend state machine
 *                                     [range: AnnounceSendSmIdx < EthTSyn_GetSizeOfAnnounceSendSm()]
 *  \pre            AnnounceSendSm is in state TX_ANNOUNCE
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires AnnounceSendSmIdx < EthTSyn_GetSizeOfAnnounceSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_HndlTxAnnounceState(
  EthTSyn_AnnounceSendSmIterType AnnounceSendSmIdx);

/**********************************************************************************************************************
 * EthTSyn_AnnounceSendSm_TxAnnounce
 *********************************************************************************************************************/
/*! \brief          Transmits an Announce message
 *  \details        -
 *  \param[in]      AnnounceSendSmIdx  Index of the AnnounceSend state machine
 *                                     [range: AnnounceSendSmIdx < EthTSyn_GetSizeOfAnnounceSendSm()]
 *  \return         E_OK - Successfully transmitted the Announce message
 *  \return         E_NOT_OK - Failed to transmit the Announce message
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \spec
 *    requires AnnounceSendSmIdx < EthTSyn_GetSizeOfAnnounceSendSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_TxAnnounce(
  EthTSyn_AnnounceSendSmIterType AnnounceSendSmIdx);
# endif /* ETHTSYN_NOUNIT_ANNOUNCESENDSM */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Internal functions of SyncSendSm
 *!
 * \unit SyncSendSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SYNCSENDSM)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_IntInit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_IntInit(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_GlobalTimestampType ts = {0u};
  const EthTSyn_SyncTxInfoType syncTxInfo = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all variable data of SyncSend state machine. */
  EthTSyn_SetStateOfSyncSendSmDyn(SyncSendSmIdx, ETHTSYN_INITIALIZING_STATEOFSYNCSENDSMDYN);
  EthTSyn_SetTxIntervalCntOfSyncSendSmDyn(SyncSendSmIdx, 0u);
  EthTSyn_SetSequenceIdOfSyncSendSmDyn(SyncSendSmIdx, 0u);
  EthTSyn_SetSmResetRequestedOfSyncSendSmDyn(SyncSendSmIdx, FALSE);
  EthTSyn_SetFupCorrFieldOfSyncSendSmDyn(SyncSendSmIdx, 0u);
  EthTSyn_SetSyncTxInfoOfSyncSendSmDyn(SyncSendSmIdx, syncTxInfo);
  EthTSyn_SetPreciseOriginTsOfSyncSendSmDyn(SyncSendSmIdx, ts);
} /* EthTSyn_SyncSendSm_IntInit() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_CheckTimeBaseUpdateCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_CheckTimeBaseUpdateCounter(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_TimeBaseUpdateCounterIterType tbUpdateCounterIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all time base update counter. */
  for (tbUpdateCounterIter = 0; tbUpdateCounterIter < EthTSyn_GetSizeOfTimeBaseUpdateCounter(); tbUpdateCounterIter++)
  {
    uint8 updateCounter = StbM_GetTimeBaseUpdateCounter(
      EthTSyn_GetSynchronizedTimeBaseIdOfTimeBaseUpdateCounter(tbUpdateCounterIter));

    /* #20 Set "Updated"-flag if value of counter has changed. */
    if (EthTSyn_GetValueOfTimeBaseUpdateCounterDyn(tbUpdateCounterIter) == updateCounter)
    {
      EthTSyn_SetUpdatedOfTimeBaseUpdateCounterDyn(tbUpdateCounterIter, FALSE);
    }
    else
    {
      EthTSyn_SetUpdatedOfTimeBaseUpdateCounterDyn(tbUpdateCounterIter, TRUE);
      EthTSyn_SetValueOfTimeBaseUpdateCounterDyn(tbUpdateCounterIter, updateCounter);
    }
  }
} /* EthTSyn_SyncSendSm_CheckTimeBaseUpdateCounter() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_Proc
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_Proc(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset state machine if requested. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
  if (EthTSyn_IsSmResetRequestedOfSyncSendSmDyn(SyncSendSmIdx) == TRUE)
  {
    EthTSyn_SyncSendSm_IntInit(SyncSendSmIdx);
    EthTSyn_PortSyncSendSm_CancelCycle(EthTSyn_GetPortSyncSendSmIdxOfSyncSendSm(SyncSendSmIdx));
  }
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();

  /* #20 Check if state machine is in state INITIALIZING.
   *       Process the INITIALIZING state.
   *     Otherwise,
   *       Check for a new Tx Cycle (cyclic or immediate) and handle it.
   *       Process the current state of the state machine. */
  if (EthTSyn_GetStateOfSyncSendSmDyn(SyncSendSmIdx) == ETHTSYN_INITIALIZING_STATEOFSYNCSENDSMDYN)
  {
    EthTSyn_SyncSendSm_ProcInitializing(SyncSendSmIdx);
  }
  else
  {
    EthTSyn_SyncSendSm_CheckAndHndlTxCycleRestart(SyncSendSmIdx);
    EthTSyn_SyncSendSm_ProcState(SyncSendSmIdx);
  }
} /* EthTSyn_SyncSendSm_Proc() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_CheckAndHndlTxCycleRestart
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_CheckAndHndlTxCycleRestart(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_StateOfSyncSendSmDynType oldState = EthTSyn_GetStateOfSyncSendSmDyn(SyncSendSmIdx);
  EthTSyn_StateOfSyncSendSmDynType newState = oldState;
  boolean txCycleExpired = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Decrement Tx interval counter if possible and optionally store information about expired Tx counter. */
  if (EthTSyn_GetTxIntervalCntOfSyncSendSmDyn(SyncSendSmIdx) > 0u)
  {
    EthTSyn_DecTxIntervalCntOfSyncSendSmDyn(SyncSendSmIdx);

    if (EthTSyn_GetTxIntervalCntOfSyncSendSmDyn(SyncSendSmIdx) == 0u)
    {
      txCycleExpired = TRUE;
    }
  }

  /* #20 Check if immediate time sync is configured for the current state machine and if the update counter changed.
   *       Set new state of the state machine to TX_IMMEDIATE_SYNC.
   *     Otherwise, check if Tx counter expired.
   *       Set new state of the state machine to TX_CYCLIC_SYNC.
   *     Otherwise, no further action is required. */
  if ((EthTSyn_IsImmediateTimeSyncOfSyncSendSm(SyncSendSmIdx) == TRUE) &&
      (EthTSyn_IsUpdatedOfTimeBaseUpdateCounterDyn(
        EthTSyn_GetTimeBaseUpdateCounterIdxOfSyncSendSm(SyncSendSmIdx)) == TRUE))
  {
    newState = ETHTSYN_TX_IMMEDIATE_SYNC_STATEOFSYNCSENDSMDYN;
  }
  else if (txCycleExpired)
  {
    newState = ETHTSYN_TX_CYCLIC_SYNC_STATEOFSYNCSENDSMDYN;
  }
  else
  {
    /* Nothing to do here. */
  }

  /* #30 Check if state change is required.
   *       Cancel any potential ongoing cycle in the PortSyncSendSm.
   *       Increment the SequenceId and change the state of the state machine. */
  if (newState != oldState)
  {
    const EthTSyn_PortSyncSendSm_SmIdxType portSyncSendSmIdx = EthTSyn_GetPortSyncSendSmIdxOfSyncSendSm(SyncSendSmIdx);

    /* This call is not required when state machine was in state WAIT_TX_SYNC. However, it is not a problem to call it
     * so it is done to reduce the code complexity and increase readability. */
    EthTSyn_PortSyncSendSm_CancelCycle(portSyncSendSmIdx);

    EthTSyn_IncSequenceIdOfSyncSendSmDyn(SyncSendSmIdx);
    EthTSyn_SetStateOfSyncSendSmDyn(SyncSendSmIdx, newState);
  }
} /* EthTSyn_SyncSendSm_CheckAndHndlTxCycleRestart() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcState
 *********************************************************************************************************************/
/*!
 * Internal comment removed. *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcState(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Process current state. */
  switch (EthTSyn_GetStateOfSyncSendSmDyn(SyncSendSmIdx))
  {
  case ETHTSYN_TX_CYCLIC_SYNC_STATEOFSYNCSENDSMDYN:
    EthTSyn_SyncSendSm_ProcTxCyclicSync(SyncSendSmIdx);
    break;

  case ETHTSYN_TX_IMMEDIATE_SYNC_STATEOFSYNCSENDSMDYN:
    EthTSyn_SyncSendSm_ProcTxImmediateSync(SyncSendSmIdx);
    break;

  case ETHTSYN_WAIT_SYNC_EGR_TS_STATEOFSYNCSENDSMDYN:
    EthTSyn_SyncSendSm_ProcWaitSyncEgrTs(SyncSendSmIdx);
    break;

  case ETHTSYN_TX_FUP_STATEOFSYNCSENDSMDYN:
    EthTSyn_SyncSendSm_ProcTxFup(SyncSendSmIdx);
    break;

  default:
    /* Nothing to do. */
    break;
  }
} /* EthTSyn_SyncSendSm_ProcState() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcInitializing
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcInitializing(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  StbM_TimeBaseStatusType timeBaseStatusLoc = 0u;
  StbM_TimeBaseStatusType ofsTimeBaseStatusLoc = 0u;
  const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPortSyncSendSm(
    EthTSyn_GetPortSyncSendSmIdxOfSyncSendSm(SyncSendSmIdx));
  const StbM_SynchronizedTimeBaseType timeBaseId = EthTSyn_ComCtrl_GetSynchronizedTimeBaseId(comCtrlPortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if TimeBaseStatus bit is set. */
  if (StbM_GetTimeBaseStatus(timeBaseId, &timeBaseStatusLoc, &ofsTimeBaseStatusLoc) == E_OK)
  {
    if ((timeBaseStatusLoc & ETHTSYN_TIME_BASE_STATUS_GLOBAL_TIME_BASE_MASK) != 0u)
    {
      /* #20 Check if link state is active. */
      if (EthTSyn_ComCtrl_IsLinkStateActive(comCtrlPortIdx) == TRUE)
      {
        /* #30 If cyclic Sync transmission is enabled, set state to TX_CYCLIC_SYNC. */
        if (EthTSyn_GetSyncSendIntervalOfSyncSendSm(SyncSendSmIdx) > 0u)
        {
          EthTSyn_SetStateOfSyncSendSmDyn(SyncSendSmIdx, ETHTSYN_TX_CYCLIC_SYNC_STATEOFSYNCSENDSMDYN);
        }
        /* #31 Otherwise set state to TX_IMMEDIATE_SYNC. */
        else
        {
          EthTSyn_SetStateOfSyncSendSmDyn(SyncSendSmIdx, ETHTSYN_TX_IMMEDIATE_SYNC_STATEOFSYNCSENDSMDYN);
        }
      }
    }
  }
} /* EthTSyn_SyncSendSm_ProcInitializing() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcTxCyclicSync
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcTxCyclicSync(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Transmit Sync message and check if successful. */
  if (EthTSyn_SyncSendSm_TxSync(SyncSendSmIdx) == E_OK)
  {
    /* #20 Reset Tx cycle timer and set state to WAIT_SYNC_EGR_TS. */
    EthTSyn_SetTxIntervalCntOfSyncSendSmDyn(SyncSendSmIdx, EthTSyn_GetSyncSendIntervalOfSyncSendSm(SyncSendSmIdx));
    EthTSyn_SetStateOfSyncSendSmDyn(SyncSendSmIdx, ETHTSYN_WAIT_SYNC_EGR_TS_STATEOFSYNCSENDSMDYN);
  }
} /* EthTSyn_SyncSendSm_ProcTxCyclicSync() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcTxImmediateSync
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcTxImmediateSync(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Transmit Sync message and check if successful. */
  if (EthTSyn_SyncSendSm_TxSync(SyncSendSmIdx) == E_OK)
  {
    /* #20 If cyclic message transmission is enabled, set Tx cycle timer to value of CyclicMsgResumeCnt. */
    if (EthTSyn_GetSyncSendIntervalOfSyncSendSm(SyncSendSmIdx) > 0u)
    {
      EthTSyn_SetTxIntervalCntOfSyncSendSmDyn(SyncSendSmIdx, EthTSyn_GetCyclicMsgResumeCntOfSyncSendSm(SyncSendSmIdx));
    }

    /* #30 Set state to WAIT_SYNC_EGR_TS. */
    EthTSyn_SetStateOfSyncSendSmDyn(SyncSendSmIdx, ETHTSYN_WAIT_SYNC_EGR_TS_STATEOFSYNCSENDSMDYN);
  }
} /* EthTSyn_SyncSendSm_ProcTxImmediateSync() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcWaitSyncEgrTs
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcWaitSyncEgrTs(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_PortSyncSendSm_SmIdxType portSyncSendSmIdx = EthTSyn_GetPortSyncSendSmIdxOfSyncSendSm(SyncSendSmIdx);
  EthTSyn_TsInfoType tsInfo = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to get sync egress timestamp info from PortSyncSendSm. */
  if (EthTSyn_PortSyncSendSm_GetSyncEgrTs(portSyncSendSmIdx, &tsInfo) == E_OK)
  {
    const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPortSyncSendSm(portSyncSendSmIdx);
    boolean isPotValid = FALSE;
    uint64 corrFieldNs = 0u;
    const EthTSyn_SyncTxInfoType syncTxInfo = EthTSyn_GetSyncTxInfoOfSyncSendSmDyn(SyncSendSmIdx);
    EthTSyn_GlobalTimestampType pot = {0};

    /* #20 Try to calculate precise origin timestamp and correction field if switch management is enabled. */
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    if (EthTSyn_SwtMgmtTx_CalcPotAndCorrField(comCtrlPortIdx, &tsInfo, &syncTxInfo, &pot, &corrFieldNs) == E_OK)
    {
      isPotValid = TRUE;

      /* #200 Forward timestamp information to SwtTimeSyncSm if switch time synchronization is enabled. */
#   if (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON)
      EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncSendSm(EthTSyn_GetSwtTimeSyncSmIdxOfSyncSendSm(SyncSendSmIdx),
        EthTSyn_GetSequenceIdOfSyncSendSmDyn(SyncSendSmIdx), &pot, &tsInfo);
#   endif /* (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) */
    }
#  else
    /* #21 Otherwise check if egress timestamp is valid and calculate precise origin timestamp. */
    if (tsInfo.IntVltMgmt.IntVltValid == TRUE)
    {
      if (EthTSyn_SyncSendTx_CalcPot(comCtrlPortIdx, tsInfo.IntVltMgmt.IntVlt, &syncTxInfo, &pot) == E_OK)
      {
        isPotValid = TRUE;
      }
    }
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

    /* #30 If calculation of precise origin timestamp succeeded, store timestamp as well as correction field and
     *     set state to TX_FUP. */
    if (isPotValid == TRUE)
    {
      EthTSyn_SetPreciseOriginTsOfSyncSendSmDyn(SyncSendSmIdx, pot);
      EthTSyn_SetFupCorrFieldOfSyncSendSmDyn(SyncSendSmIdx, corrFieldNs);
      EthTSyn_SetStateOfSyncSendSmDyn(SyncSendSmIdx, ETHTSYN_TX_FUP_STATEOFSYNCSENDSMDYN);
    }
    /* #31 Otherwise, set state to WAIT_TX_SYNC and cancel cycle of PortSyncSend state machine. */
    else
    {
      EthTSyn_SetStateOfSyncSendSmDyn(SyncSendSmIdx, ETHTSYN_WAIT_TX_SYNC_STATEOFSYNCSENDSMDYN);
      EthTSyn_PortSyncSendSm_CancelCycle(portSyncSendSmIdx);
    }
  }
} /* EthTSyn_SyncSendSm_ProcWaitSyncEgrTs() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_ProcTxFup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_ProcTxFup(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_MsgHdrTypeSpecType typeSpecMsgHdr = {0};

  /* ----- Implementation ----------------------------------------------- */
  typeSpecMsgHdr.Control = ETHTSYN_MSG_HDR_CONTROL_FOLLOW_UP;
  typeSpecMsgHdr.CorrectionFieldNs = EthTSyn_GetFupCorrFieldOfSyncSendSmDyn(SyncSendSmIdx);
  typeSpecMsgHdr.CorrectionFieldSubNs = ETHTSYN_MSG_HDR_COR_FIELD_SUB_NS_DEFAULT;
  typeSpecMsgHdr.Flags0 = ETHTSYN_MSG_HDR_FLAGS_0_DEFAULT;
  typeSpecMsgHdr.Flags1 = ETHTSYN_MSG_HDR_FLAGS_1_DEFAULT;
  typeSpecMsgHdr.LogMessageInterval = EthTSyn_GetSyncLogMsgIntervalOfSyncSendSm(SyncSendSmIdx);
  /* Set the message length to maximum FollowUp message length in the current configuration to ensure that sufficient
   * Tx buffer is requested from EthIf. */
  typeSpecMsgHdr.MessageLength = EthTSyn_GetSyncSendSmMaxFupTxLen();
  typeSpecMsgHdr.MessageType = ETHTSYN_MSG_TYPE_FUP;
  typeSpecMsgHdr.SequenceId = EthTSyn_GetSequenceIdOfSyncSendSmDyn(SyncSendSmIdx);

  /* #10 Transmit FollowUp message and check if successful. */
  if (EthTSyn_SyncSendTx_TxFup(SyncSendSmIdx,
    EthTSyn_GetAddrSyncTxInfoOfSyncSendSmDyn(SyncSendSmIdx),
    &typeSpecMsgHdr,
    EthTSyn_GetAddrPreciseOriginTsOfSyncSendSmDyn(SyncSendSmIdx)) == E_OK)
  {
    /* #20 Set state to WAIT_TX_SYNC. */
    EthTSyn_SetStateOfSyncSendSmDyn(SyncSendSmIdx, ETHTSYN_WAIT_TX_SYNC_STATEOFSYNCSENDSMDYN);
  }
} /* EthTSyn_SyncSendSm_ProcTxFup() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_TxSync
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncSendSm_TxSync(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_SyncTxInfoType syncTxInfo = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to transmit Sync message. */
  if (EthTSyn_SyncSendTx_TxSync(SyncSendSmIdx, EthTSyn_GetSequenceIdOfSyncSendSmDyn(SyncSendSmIdx), &syncTxInfo)
       == E_OK)
  {
    /* #20 Store Sync Tx time information. */
    EthTSyn_SetSyncTxInfoOfSyncSendSmDyn(SyncSendSmIdx, syncTxInfo);

    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_SyncSendSm_TxSync() */

# endif /* ETHTSYN_NOUNIT_SYNCSENDSM */

/**********************************************************************************************************************
 * Internal functions of SyncSendTx
 *!
 * \unit SyncSendTx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SYNCSENDTX)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_IsFollowUpTxLengthValid
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SyncSendTx_IsFollowUpTxLengthValid(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean followUpTxLengthValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the expected FollowUp Tx message length. */
  const uint16 expectedFollowUpMsgLength = EthTSyn_SyncSendTx_GetExpectedFollowUpTxLength(SyncSendSmIdx);

  /* #20 Check if the SyncSendSm specific generated length equals the expected length. */
  if (EthTSyn_GetFupTxLenOfSyncSendSm(SyncSendSmIdx) == expectedFollowUpMsgLength)
  {
    /* #30 Check if the generated max length is greater or equal to the expected length. */
    if (EthTSyn_GetSyncSendSmMaxFupTxLen() >= expectedFollowUpMsgLength)
    {
      followUpTxLengthValid = TRUE;
    }
  }

  return followUpTxLengthValid;
} /* EthTSyn_SyncSendTx_IsFollowUpTxLengthValid() */

/**********************************************************************************************************************
 * EthTSyn_SyncSendTx_GetExpectedFollowUpTxLength
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
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_GetExpectedFollowUpTxLength(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 expectedFollowUpMsgLength = ETHTSYN_MSG_FUP_LEN;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Compute the expected FollowUp Tx length if message compliance is disabled. */
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)

  /* #20 Add ETHTSYN_AR_SUB_TLV_TIME_LENGTH if Time Sub-Tlv is used. */
#   if (ETHTSYN_CRC_SUPPORT == STD_ON)
  if (EthTSyn_IsTimeSubTlvOfSyncSendSm(SyncSendSmIdx) == TRUE)
  {
    expectedFollowUpMsgLength += ETHTSYN_AR_SUB_TLV_TIME_LENGTH;
  }
#   endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */

  /* #30 Add ETHTSYN_AR_SUB_TLV_STATUS_LENGTH if Status Sub-Tlv is used. */
  if (EthTSyn_IsStatusSubTlvOfSyncSendSm(SyncSendSmIdx) == TRUE)
  {
    expectedFollowUpMsgLength += ETHTSYN_AR_SUB_TLV_STATUS_LENGTH;
  }

  /* #40 Add ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH if UserData Sub-Tlv is used. */
  if (EthTSyn_IsUserDataSubTlvOfSyncSendSm(SyncSendSmIdx) == TRUE)
  {
    expectedFollowUpMsgLength += ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH;
  }

  /* #50 Add ETHTSYN_AR_SUB_TLV_OFS_LENGTH for each offset time domain if Ofs Sub-Tlv is used (if master offset domain
   *     support is enabled). */
#   if (ETHTSYN_MASTER_OFS_DOMAIN_SUPPORT == STD_ON)
  if (EthTSyn_IsOfsSubTlvOfSyncSendSm(SyncSendSmIdx) == TRUE)
  {
    const EthTSyn_ComCtrl_TimeDomainIdxType comCtrlTdIdx = EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(
      EthTSyn_GetComCtrlPortIdxOfPortSyncSendSm(EthTSyn_GetPortSyncSendSmIdxOfSyncSendSm(SyncSendSmIdx)));
    const EthTSyn_OffsetTimeDomainIterType ofsTimeDomainCnt =
      (EthTSyn_OffsetTimeDomainIterType)EthTSyn_GetOffsetTimeDomainEndIdxOfComCtrlTimeDomain(comCtrlTdIdx) -
      (EthTSyn_OffsetTimeDomainIterType)EthTSyn_GetOffsetTimeDomainStartIdxOfComCtrlTimeDomain(comCtrlTdIdx);

    expectedFollowUpMsgLength += (uint16)(ofsTimeDomainCnt * ETHTSYN_AR_SUB_TLV_OFS_LENGTH);
  }
#   endif /* (ETHTSYN_MASTER_OFS_DOMAIN_SUPPORT == STD_ON) */

  /* #60 Add the ArTlvHdr length in case any Sub-Tlv is used. */
  if (expectedFollowUpMsgLength > ETHTSYN_MSG_FUP_LEN)
  {
    expectedFollowUpMsgLength += ETHTSYN_AR_TLV_HEADER_LENGTH;
  }
#  else
  ETHTSYN_DUMMY_STATEMENT(SyncSendSmIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_ON) */

  return expectedFollowUpMsgLength;
} /* EthTSyn_SyncSendTx_GetExpectedFollowUpTxLength() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_SerializeFup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_SerializeFup(
                  EthTSyn_SyncSendSmIterType   SyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_SyncTxInfoType)      SyncTxInfoPtr,
    ETHTSYN_P2VAR(uint8)                       TxBufPtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) PotPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint16 actualFollowUpMsgLength = EthTSyn_GetFupTxLenOfSyncSendSm(SyncSendSmIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Serialize precise origin timestamp. */
  EthTSyn_Mem_PutUint16(TxBufPtr, ETHTSYN_MSG_FUP_POT_SEC_HI_OFS, PotPtr->secondsHi);
  EthTSyn_Mem_PutUint32(TxBufPtr, ETHTSYN_MSG_FUP_POT_SEC_LOW_OFS, PotPtr->seconds);
  EthTSyn_Mem_PutUint32(TxBufPtr, ETHTSYN_MSG_FUP_POT_NSEC_OFS, PotPtr->nanoseconds);

  /* #20 Serialize FollowUp Information TLV. */
  EthTSyn_Mem_PutUint16(TxBufPtr, ETHTSYN_MSG_FUP_INFO_TLV_TYPE_OFS, ETHTSYN_FUP_INFO_TLV_TLV_TYPE_DEFAULT);
  EthTSyn_Mem_PutUint16(TxBufPtr, ETHTSYN_MSG_FUP_INFO_TLV_LEN_OFS, ETHTSYN_FUP_INFO_TLV_LENGTH_DEFAULT);
  EthTSyn_Mem_PutUint24(TxBufPtr, ETHTSYN_MSG_FUP_INFO_TLV_ORGA_ID_OFS, ETHTSYN_FUP_INFO_TLV_ORG_ID_DEFAULT);
  EthTSyn_Mem_PutUint24(TxBufPtr, ETHTSYN_MSG_FUP_INFO_TLV_ORGA_SUB_TYPE_OFS,
    ETHTSYN_FUP_INFO_TLV_ORG_SUBTYPE_DEFAULT);
  /* cumScaleRateOfs,gmTimeBaseIndicator, lastGmPhaseChange and scaledLastGmFreqChange are defined to be "0".
   * These fields are therefore not serialized. */

  /* #30 Correct the message length field value in the header. */
  /* The message length is set to the maximum possible FollowUp length in the configuration to ensure that sufficient
   * Tx buffer is requested. The length needs to be corrected before adding the sub-TLVs to ensure that CRC calculation
   * is based on correct values. */
  EthTSyn_Mem_PutUint16(TxBufPtr, ETHTSYN_MSG_HDR_MSG_LENGTH_OFS, actualFollowUpMsgLength);

  /* #40 Append FollowUp Ar TLVs (if message compliance is disabled). */
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
  {
    EthTSyn_SyncSendTx_AppendArFollowUpTlvs(SyncSendSmIdx, SyncTxInfoPtr, TxBufPtr);
  }
#  else
  ETHTSYN_DUMMY_STATEMENT(SyncSendSmIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  ETHTSYN_DUMMY_STATEMENT(SyncTxInfoPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

  return actualFollowUpMsgLength;
} /* EthTSyn_SyncSendTx_SerializeFup() */

#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_AppendArFollowUpTlvs
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendTx_AppendArFollowUpTlvs(
                  EthTSyn_SyncSendSmIterType SyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_SyncTxInfoType)    SyncTxInfoPtr,
    ETHTSYN_P2VAR(uint8)                     TxBufPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 arTlvHdrLengthField = ETHTSYN_AR_TLV_HEADER_LENGTH_FIELD_VALUE; /* Add length sum of Sub-Tlvs later */
  uint16 nextSubTlvOfs = ETHTSYN_MSG_FUP_FIRST_AR_SUB_TLV_OFS;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Append the Time Secured Sub-Tlv if applicable. */
#   if (ETHTSYN_CRC_SUPPORT == STD_ON)
  arTlvHdrLengthField += EthTSyn_SyncSendTx_AppendArFollowUpTimeTlv(SyncSendSmIdx, TxBufPtr, &nextSubTlvOfs);
#   endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */

  /* #20 Append the Status (Not) Secured Sub-Tlv if applicable. */
  arTlvHdrLengthField += EthTSyn_SyncSendTx_AppendArFollowUpStatusTlv(SyncSendSmIdx,
    SyncTxInfoPtr->SyncTxGlobalTime.timeBaseStatus, TxBufPtr, &nextSubTlvOfs);

  /* #30 Append the User Data (Not) Secured Sub-Tlv if applicable. */
  arTlvHdrLengthField += EthTSyn_SyncSendTx_AppendArFollowUpUserDataTlv(SyncSendSmIdx, &SyncTxInfoPtr->SyncTxUserData,
    TxBufPtr, &nextSubTlvOfs);

  /* #40 Append the Ofs (Not) Secured Sub-Tlvs if applicable. */
#   if (ETHTSYN_MASTER_OFS_DOMAIN_SUPPORT == STD_ON)
  arTlvHdrLengthField += EthTSyn_SyncSendTx_AppendArFollowUpOfsTlvs(SyncSendSmIdx, TxBufPtr, &nextSubTlvOfs);
#   endif /* (ETHTSYN_MASTER_OFS_DOMAIN_SUPPORT == STD_ON) */

  /* #50 Serialize ArTlvHdr if any Sub-Tlv was appended. */
  if (arTlvHdrLengthField > ETHTSYN_AR_TLV_HEADER_LENGTH_FIELD_VALUE)
  {
    EthTSyn_Mem_PutUint16(TxBufPtr, ETHTSYN_MSG_FUP_AR_TLV_HDR_TYPE_OFS, ETHTSYN_AR_TLV_TYPE);
    EthTSyn_Mem_PutUint16(TxBufPtr, ETHTSYN_MSG_FUP_AR_TLV_HDR_LEN_OFS, arTlvHdrLengthField);
    EthTSyn_Mem_PutUint24(TxBufPtr, ETHTSYN_MSG_FUP_AR_TLV_HDR_ORGA_ID_OFS, ETHTSYN_AR_TLV_ORGANIZATION_ID);
    EthTSyn_Mem_PutUint24(TxBufPtr, ETHTSYN_MSG_FUP_AR_TLV_HDR_ORGA_SUB_TYPE_OFS,
      ETHTSYN_AR_TLV_ORGANIZATION_SUB_TYPE);

  }
} /* EthTSyn_SyncSendTx_AppendArFollowUpTlvs() */

#   if (ETHTSYN_CRC_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_AppendArFollowUpTimeTlv
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_AppendArFollowUpTimeTlv(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx,
  ETHTSYN_P2VAR(uint8)       TxBufPtr,
  ETHTSYN_P2VAR(uint16)      OffsetPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 timeSecuredSubTlvSize = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Time Tlv is required. */
  if (EthTSyn_IsTimeSubTlvOfSyncSendSm(SyncSendSmIdx) == TRUE)
  {
    /* #20 Calculate CRC. */
    const uint8 crcTimeFlags = EthTSyn_GetCrcTimeFlagsOfSyncSendSm(SyncSendSmIdx);
    EthTSyn_TimeSecuredCrcType crc = {0};
    ETHTSYN_P2VAR(uint8) subTlvStartPtr = &TxBufPtr[(*OffsetPtr)];

    crc = EthTSyn_CrcHndl_GetTimeSecuredCrc(
      EthTSyn_GetCrcHndlIdxOfSyncSendSm(SyncSendSmIdx), crcTimeFlags, TxBufPtr);

    /* #30 Serialize Time Secured Tlv. */
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TYPE_OFS, ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED);
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_LEN_OFS,
      ETHTSYN_AR_SUB_TLV_TIME_LENGTH_FIELD_VALUE);
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_FLAGS_OFS, crcTimeFlags);
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_CRC_0_OFS, crc.Time0Crc);
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_CRC_1_OFS, crc.Time1Crc);

    /* #40 Increase offset for further Sub-Tlvs and return size of the TimeSecured Sub-Tlv. */
    (*OffsetPtr) += ETHTSYN_AR_SUB_TLV_TIME_LENGTH;
    timeSecuredSubTlvSize = ETHTSYN_AR_SUB_TLV_TIME_LENGTH;
  }

  return timeSecuredSubTlvSize;
} /* EthTSyn_SyncSendTx_AppendArFollowUpTimeTlv() */
#   endif /* (ETHTSYN_CRC_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_AppendArFollowUpStatusTlv
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_AppendArFollowUpStatusTlv(
                EthTSyn_SyncSendSmIterType SyncSendSmIdx,
                StbM_TimeBaseStatusType    SyncTxTbStatus,
  ETHTSYN_P2VAR(uint8)                     TxBufPtr,
  ETHTSYN_P2VAR(uint16)                    OffsetPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 statusSubTlvSize = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Status Sub-Tlv is required. */
  if (EthTSyn_IsStatusSubTlvOfSyncSendSm(SyncSendSmIdx) == TRUE)
  {
    ETHTSYN_P2VAR(uint8) subTlvStartPtr = &TxBufPtr[(*OffsetPtr)];
    const uint8 statusSubTlvStatus = EthTSyn_SyncSendTx_TimeBaseStatusToArSubTlvStatus(SyncTxTbStatus);
    EthTSyn_SubTlvCrcInfoType subTlvCrcInfo = {0u};

    /* #20 Serialize Status Sub-Tlv except type and CRC. */
    /* It is important to serialize the Tlv except type and CRC to fulfill precondition of
     * EthTSyn_CrcHndl_GetStatusTlvCrcInfo(). */
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_LEN_OFS,
      ETHTSYN_AR_SUB_TLV_STATUS_LENGTH_FIELD_VALUE);
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_STATUS_STATUS_OFS, statusSubTlvStatus);

    /* #30 Get and serialize Sub-Tlv type and CRC value. */
    subTlvCrcInfo = EthTSyn_CrcHndl_GetStatusTlvCrcInfo(
      EthTSyn_GetCrcHndlIdxOfSyncSendSm(SyncSendSmIdx), TxBufPtr, *OffsetPtr,
      EthTSyn_GetTxCrcSecuredOfSyncSendSm(SyncSendSmIdx));

    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TYPE_OFS, subTlvCrcInfo.SubTlvType);
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_STATUS_CRC_OFS, subTlvCrcInfo.SubTlvCrc);

    /* #40 Increase offset for further Sub-Tlvs and return size of the Status Sub-Tlv. */
    (*OffsetPtr) += ETHTSYN_AR_SUB_TLV_STATUS_LENGTH;
    statusSubTlvSize = ETHTSYN_AR_SUB_TLV_STATUS_LENGTH;
  }

  return statusSubTlvSize;
} /* EthTSyn_SyncSendTx_AppendArFollowUpStatusTlv() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_AppendArFollowUpUserDataTlv
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_AppendArFollowUpUserDataTlv(
                  EthTSyn_SyncSendSmIterType SyncSendSmIdx,
  ETHTSYN_P2CONST(StbM_UserDataType)         UserDataPtr,
    ETHTSYN_P2VAR(uint8)                     TxBufPtr,
    ETHTSYN_P2VAR(uint16)                    OffsetPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 userDataSubTlvSize = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if UserData Sub-Tlv is required. */
  if (EthTSyn_IsUserDataSubTlvOfSyncSendSm(SyncSendSmIdx) == TRUE)
  {
    ETHTSYN_P2VAR(uint8) subTlvStartPtr = &TxBufPtr[(*OffsetPtr)];
    EthTSyn_SubTlvCrcInfoType subTlvCrcInfo = {0u};

    /* #20 Serialize UserData Sub-Tlv except type and CRC. */
    /* It is important to serialize the Tlv except type and CRC to fulfill precondition of
     * EthTSyn_CrcHndl_GetUserDataTlvCrcInfo(). */
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_LEN_OFS,
      ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH_FIELD_VALUE);

    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_LEN_OFS,
      UserDataPtr->userDataLength);
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_BYTE_0_OFS,
      UserDataPtr->userByte0);
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_BYTE_1_OFS,
      UserDataPtr->userByte1);
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_BYTE_2_OFS,
      UserDataPtr->userByte2);

    /* #30 Get and serialize Sub-Tlv type and CRC value. */
    subTlvCrcInfo = EthTSyn_CrcHndl_GetUserDataTlvCrcInfo(
      EthTSyn_GetCrcHndlIdxOfSyncSendSm(SyncSendSmIdx), TxBufPtr, *OffsetPtr,
      EthTSyn_GetTxCrcSecuredOfSyncSendSm(SyncSendSmIdx));

    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TYPE_OFS, subTlvCrcInfo.SubTlvType);
    EthTSyn_Mem_PutUint8(subTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_CRC_OFS, subTlvCrcInfo.SubTlvCrc);

    /* #40 Increase offset for further Sub-Tlvs and return size of the UserData Sub-Tlv. */
    (*OffsetPtr) += ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH;
    userDataSubTlvSize = ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH;
  }

  return userDataSubTlvSize;
} /* EthTSyn_SyncSendTx_AppendArFollowUpUserDataTlv() */

#   if (ETHTSYN_MASTER_OFS_DOMAIN_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_AppendArFollowUpOfsTlvs
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
ETHTSYN_LOCAL_INLINE FUNC(uint16, ETHTSYN_CODE) EthTSyn_SyncSendTx_AppendArFollowUpOfsTlvs(
  EthTSyn_SyncSendSmIterType SyncSendSmIdx,
  ETHTSYN_P2VAR(uint8)       TxBufPtr,
  ETHTSYN_P2VAR(uint16)      OffsetPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 ofsSubTlvsSize = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Ofs Tlv is required. */
  if (EthTSyn_IsOfsSubTlvOfSyncSendSm(SyncSendSmIdx) == TRUE)
  {
    const EthTSyn_ComCtrl_TimeDomainIdxType comCtrlTdIdx = EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(
      EthTSyn_GetComCtrlPortIdxOfPortSyncSendSm(EthTSyn_GetPortSyncSendSmIdxOfSyncSendSm(SyncSendSmIdx)));
    EthTSyn_OffsetTimeDomainIterType offsetTimeDomainIter = 0u;
    const uint16 startOffset = (*OffsetPtr);

    /* #20 Iterate over all offset TimeDomains associated to given SyncSendSm. */
    for(offsetTimeDomainIter = EthTSyn_GetOffsetTimeDomainStartIdxOfComCtrlTimeDomain(comCtrlTdIdx);
        offsetTimeDomainIter < EthTSyn_GetOffsetTimeDomainEndIdxOfComCtrlTimeDomain(comCtrlTdIdx);
        offsetTimeDomainIter++)
    {
      const uint16 currOffset = startOffset + ofsSubTlvsSize;
      /*@ assert currOffset <= (EthTSyn_GetSyncSendSmMaxFupTxLen() - ETHTSYN_AR_SUB_TLV_OFS_LENGTH); */                /* VCA_ETHTSYN_FUP_TX_BUFFER_SIZE */
      ETHTSYN_P2VAR(uint8) currSubTlvStartPtr = &TxBufPtr[currOffset];
      const uint8 ofsTimeDomainId = EthTSyn_GetIdOfOffsetTimeDomain(offsetTimeDomainIter);
      const StbM_SynchronizedTimeBaseType timeBaseId =
        EthTSyn_GetSynchronizedTimeBaseIdOfOffsetTimeDomain(offsetTimeDomainIter);
      StbM_UserDataType userData = {0u};
      StbM_TimeStampType timeStamp = {0u};
      StbM_TimeStampType offset = {0u};
      EthTSyn_SubTlvCrcInfoType subTlvCrcInfo = {0u};

      /* #30 Get the user data via StbM_GetCurrentTime() and check if failed. */
      if (StbM_GetCurrentTime(timeBaseId, &timeStamp, &userData) != E_OK)
      {
        /* #300 Set user data to '0'. */
        userData.userDataLength = 0u;
        userData.userByte0 = 0u;
        userData.userByte1 = 0u;
        userData.userByte2 = 0u;
      }

      /* #40 Get the offset via StbM_GetOffset() and check if failed. */
      if (StbM_GetOffset(timeBaseId, &offset) != E_OK)
      {
        /* #400 Set Offset to '0'. */
        offset.timeBaseStatus = 0u;
        offset.secondsHi = 0u;
        offset.seconds = 0u;
        offset.nanoseconds = 0u;
      }

      /* #50 Serialize Ofs Sub-Tlv except type and CRC. */
      /* It is important to serialize the Tlv except type and CRC to fulfill precondition of
       * EthTSyn_CrcHndl_GetOfsTlvCrcInfo(). */
      EthTSyn_Mem_PutUint8(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_LEN_OFS,
        ETHTSYN_AR_SUB_TLV_OFS_LENGTH_FIELD_VALUE);
      EthTSyn_Mem_PutUint8(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_DOMAIN_NUM_OFS, ofsTimeDomainId);

      EthTSyn_Mem_PutUint16(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_TIME_SEC_HI_OFS, offset.secondsHi);
      EthTSyn_Mem_PutUint32(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_TIME_SEC_LOW_OFS, offset.seconds);
      EthTSyn_Mem_PutUint32(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_TIME_NSEC_OFS, offset.nanoseconds);
      EthTSyn_Mem_PutUint8(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_STATUS_OFS,
        EthTSyn_SyncSendTx_TimeBaseStatusToArSubTlvStatus(offset.timeBaseStatus));

      EthTSyn_Mem_PutUint8(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_LEN_OFS,
        userData.userDataLength);
      EthTSyn_Mem_PutUint8(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_BYTE_0_OFS,
        userData.userByte0);
      EthTSyn_Mem_PutUint8(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_BYTE_1_OFS,
        userData.userByte1);
      EthTSyn_Mem_PutUint8(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_BYTE_2_OFS,
        userData.userByte2);

      /* #60 Get and serialize Sub-Tlv type and CRC value. */
      subTlvCrcInfo = EthTSyn_CrcHndl_GetOfsTlvCrcInfo(EthTSyn_GetCrcHndlIdxOfSyncSendSm(SyncSendSmIdx), TxBufPtr,
        currOffset, EthTSyn_GetTxCrcSecuredOfSyncSendSm(SyncSendSmIdx));

      EthTSyn_Mem_PutUint8(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_TYPE_OFS, subTlvCrcInfo.SubTlvType);
      EthTSyn_Mem_PutUint8(currSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_CRC_OFS, subTlvCrcInfo.SubTlvCrc);

      ofsSubTlvsSize += ETHTSYN_AR_SUB_TLV_OFS_LENGTH;
    }

    /* #70 Increase offset for further Sub-Tlv by the size of the Ofs Sub-Tlv. */
    (*OffsetPtr) += ofsSubTlvsSize;
  }

  return ofsSubTlvsSize;
} /* EthTSyn_SyncSendTx_AppendArFollowUpOfsTlvs() */
#   endif /* (ETHTSYN_MASTER_OFS_DOMAIN_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_TimeBaseStatusToArSubTlvStatus
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(uint8, ETHTSYN_CODE) EthTSyn_SyncSendTx_TimeBaseStatusToArSubTlvStatus(
  StbM_TimeBaseStatusType StbmTimeBaseStatus)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Convert the passed StbM Time Base Status into the AUTOSAR Sub-TLV status. */
  return (uint8)(
    (StbmTimeBaseStatus & ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_MASK) >> ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_SHIFT);
} /* EthTSyn_SyncSendTx_TimeBaseStatusToArSubTlvStatus() */
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

# endif /* ETHTSYN_NOUNIT_SYNCSENDTX */

/**********************************************************************************************************************
 * Internal functions of PortSyncSendSm
 *!
 * \unit PortSyncSendSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_PORTSYNCSENDSM)
/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_IsInState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_IsInState(
  EthTSyn_PortSyncSendSm_SmIdxType     PortSyncSendSmIdx,
  EthTSyn_StateOfPortSyncSendSmDynType StateToCheck)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isInState = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if PortSyncSend state machine is in given state within critical section. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
  isInState = (boolean)(EthTSyn_GetStateOfPortSyncSendSmDyn(PortSyncSendSmIdx) == StateToCheck);
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

  return isInState;
} /* EthTSyn_PortSyncSendSm_IsInState() */

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_SetState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_SetState(
  EthTSyn_PortSyncSendSm_SmIdxType     PortSyncSendSmIdx,
  EthTSyn_StateOfPortSyncSendSmDynType StateToSet)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Set given state of PortSyncSend state machine within critical section. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
  EthTSyn_SetStateOfPortSyncSendSmDyn(PortSyncSendSmIdx, StateToSet);
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
} /* EthTSyn_PortSyncSendSm_SetState() */

#  if (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_GetTimeValidData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_GetTimeValidData(
                  EthTSyn_PortSyncSendSm_SmIdxType   PortSyncSendSmIdx,
  ETHTSYN_P2CONST(uint8)                             TxBufPtr,
    ETHTSYN_P2VAR(StbM_EthTimeMasterMeasurementType) TimeValidDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  StbM_EthTimeMasterMeasurementType timeValidData = {0u};
  EthTSyn_TsInfoType syncEgrTsInfo = {0u};
#   if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  StbM_VirtualLocalTimeType swtEgrVlt = {0u};
#   endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  syncEgrTsInfo = EthTSyn_GetEgrTsInfoOfPortSyncSendSmDyn(PortSyncSendSmIdx);

  /* #10 Try to convert the switch egress timestamp into a virtual local time (if switch management is enabled). */
#   if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  if (EthTSyn_Ts_TsToVltChecked(&syncEgrTsInfo.SwtEgrTsMgmt.Ts, &swtEgrVlt) == E_OK)
#   endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
  {
    /* #20 Get all required time validation data of serialized FollowUp message. */
    timeValidData.correctionField = (sint64)EthTSyn_Mem_GetUint48(TxBufPtr, ETHTSYN_MSG_HDR_CORR_FIELD_OFS);
    timeValidData.preciseOriginTimestamp.seconds = EthTSyn_Mem_GetUint32(TxBufPtr, ETHTSYN_MSG_FUP_POT_SEC_LOW_OFS);
    timeValidData.preciseOriginTimestamp.nanoseconds = EthTSyn_Mem_GetUint32(TxBufPtr, ETHTSYN_MSG_FUP_POT_NSEC_OFS);
    timeValidData.sequenceId = EthTSyn_Mem_GetUint16(TxBufPtr, ETHTSYN_MSG_HDR_SEQUENCE_ID_OFS);
    timeValidData.sourcePortId.clockIdentity =
      EthTSyn_Mem_GetUint64(TxBufPtr, ETHTSYN_MSG_HDR_SRC_PORT_IDENT_CLOCK_IDENT_OFS);
    timeValidData.sourcePortId.portNumber =
      EthTSyn_Mem_GetUint16(TxBufPtr, ETHTSYN_MSG_HDR_SRC_PORT_IDENT_PORT_NUM_OFS);

    /* #30 Get stored Sync egress timestamp. */
#   if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    /* In case that time validation and switch management are enabled,
     * switch and endstation are considered one time aware system: switch egress timestamp can be used. */
    timeValidData.syncEgressTimestamp = swtEgrVlt;
#   else
    timeValidData.syncEgressTimestamp = EthTSyn_Ts_IntVltToVlt(syncEgrTsInfo.IntVltMgmt.IntVlt);
#   endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

    retVal = E_OK;
  }

  (*TimeValidDataPtr) = timeValidData;

  return retVal;
} /* EthTSyn_PortSyncSendSm_GetTimeValidData() */
#  endif /* (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) */

# endif /* ETHTSYN_NOUNIT_PORTSYNCSENDSM */

/**********************************************************************************************************************
 * Internal functions of AnnounceSendSm
 *!
 * \unit AnnounceSendSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_ANNOUNCESENDSM)

/**********************************************************************************************************************
 * EthTSyn_AnnounceSendSm_IntInit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_IntInit(
  EthTSyn_AnnounceSendSmIterType AnnounceSendSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all variables of the AnnounceSendSm. */
  EthTSyn_SetSequenceIdOfAnnounceSendSmDyn(AnnounceSendSmIdx, 0u);
  EthTSyn_SetAnnounceCycleTimerOfAnnounceSendSmDyn(AnnounceSendSmIdx, 0u);
  EthTSyn_SetResetRequestedOfAnnounceSendSmDyn(AnnounceSendSmIdx, FALSE);

  /* #20 Set state of the AnnounceSendSm to INITIALIZING. */
  EthTSyn_SetStateOfAnnounceSendSmDyn(AnnounceSendSmIdx, ETHTSYN_INITIALIZING_STATEOFANNOUNCESENDSMDYN);
} /* EthTSyn_AnnounceSendSm_IntInit() */

/**********************************************************************************************************************
 * EthTSyn_AnnounceSendSm_Proc
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
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_Proc(
  EthTSyn_AnnounceSendSmIterType AnnounceSendSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* #10 Decrement the announce cycle timer. */
  const boolean announceCycleTimerExpiredThisCycle = EthTSyn_AnnounceSendSm_DecCycleTimer(AnnounceSendSmIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #20 Enter critical section to prevent modification of the reset requested flag during read and reset. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();
  if (EthTSyn_IsResetRequestedOfAnnounceSendSmDyn(AnnounceSendSmIdx))
  {
    /* #30 Initialize the AnnounceSendSm if reset is requested. */
    EthTSyn_AnnounceSendSm_IntInit(AnnounceSendSmIdx);
  }

  /* #40 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_LINK_STATE();

  /* #50 Check the current state of the AnnounceSendSm. */
  switch (EthTSyn_GetStateOfAnnounceSendSmDyn(AnnounceSendSmIdx))
  {
    /* #60 If state is INITIALIZING. */
  case ETHTSYN_INITIALIZING_STATEOFANNOUNCESENDSMDYN:
    {
      const EthTSyn_ComCtrlPortIdxOfAnnounceSendSmType comCtrlPortIdx =
        EthTSyn_GetComCtrlPortIdxOfAnnounceSendSm(AnnounceSendSmIdx);

      /* #600 Check if transceiver link state is active. */
      if (EthTSyn_ComCtrl_IsLinkStateActive(comCtrlPortIdx) == TRUE)
      {
        /* #6000 Change state of the AnnounceSendSm to TX_ANNOUNCE. */
        EthTSyn_SetStateOfAnnounceSendSmDyn(AnnounceSendSmIdx, ETHTSYN_TX_ANNOUNCE_STATEOFANNOUNCESENDSMDYN);
      }
      break;
    }

    /* #70 If state is TX_ANNOUNCE. */
  case ETHTSYN_TX_ANNOUNCE_STATEOFANNOUNCESENDSMDYN:
    {
      /* #700 Handle the TX_ANNOUNCE state. */
      EthTSyn_AnnounceSendSm_HndlTxAnnounceState(AnnounceSendSmIdx);
      break;
    }

    /* #80 Otherwise (state is WAIT_TX_ANNOUCE). */
  default: /* ETHTSYN_WAIT_TX_ANNOUNCE_STATEOFANNOUNCESENDSMDYN */
    {
      /* #800 Check if the cycle timer expired. */
      if (announceCycleTimerExpiredThisCycle == TRUE)
      {
        /* #8000 Change the state of the AnnounceSendSm to TX_ANNOUNCE and directly trigger the TX_ANNOUNCE
         *       state handling for correcting timing behavior with respect to the Announce cycle time. */
        EthTSyn_SetStateOfAnnounceSendSmDyn(AnnounceSendSmIdx, ETHTSYN_TX_ANNOUNCE_STATEOFANNOUNCESENDSMDYN);

        EthTSyn_AnnounceSendSm_HndlTxAnnounceState(AnnounceSendSmIdx);
      }
      break;
    }
  }
} /* EthTSyn_AnnounceSendSm_Proc() */

/**********************************************************************************************************************
 * EthTSyn_AnnounceSendSm_DecCycleTimer
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_DecCycleTimer(
  EthTSyn_AnnounceSendSmIterType AnnounceSendSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean cycleTimerExpired = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Decrement the cycle timer if possible. */
  if (EthTSyn_GetAnnounceCycleTimerOfAnnounceSendSmDyn(AnnounceSendSmIdx) > 0u)
  {
    EthTSyn_DecAnnounceCycleTimerOfAnnounceSendSmDyn(AnnounceSendSmIdx);

    /* #20 Check if cycle timer expired this cycle. */
    if (EthTSyn_GetAnnounceCycleTimerOfAnnounceSendSmDyn(AnnounceSendSmIdx) == 0u)
    {
      cycleTimerExpired = TRUE;
    }
  }

  return cycleTimerExpired;
} /* EthTSyn_AnnounceSendSm_DecCycleTimer() */

/**********************************************************************************************************************
 * EthTSyn_AnnounceSendSm_HndlTxAnnounceState
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_HndlTxAnnounceState(
  EthTSyn_AnnounceSendSmIterType AnnounceSendSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger Announce message transmission and check if transmission succeeded. */
  if (EthTSyn_AnnounceSendSm_TxAnnounce(AnnounceSendSmIdx) == E_OK)
  {
    /* #20 Increment the sequence id and recharge the cycle timer of the AnnounceSendSm. */
    EthTSyn_IncSequenceIdOfAnnounceSendSmDyn(AnnounceSendSmIdx);
    EthTSyn_SetAnnounceCycleTimerOfAnnounceSendSmDyn(AnnounceSendSmIdx,
      EthTSyn_GetAnnounceTxCycleOfAnnounceSendSm(AnnounceSendSmIdx));

    /* #30 Report transmission of Announce message to PortParamStats unit. */
    EthTSyn_PortParamStats_IncTxAnnounceCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(
      EthTSyn_GetComCtrlPortIdxOfAnnounceSendSm(AnnounceSendSmIdx)));

    /* #40 Change state of the AnnounceSendSm to WAIT_TX_ANNOUNCE. */
    EthTSyn_SetStateOfAnnounceSendSmDyn(AnnounceSendSmIdx, ETHTSYN_WAIT_TX_ANNOUNCE_STATEOFANNOUNCESENDSMDYN);
  }
} /* EthTSyn_AnnounceSendSm_HndlTxAnnounceState() */

/**********************************************************************************************************************
 * EthTSyn_AnnounceSendSm_TxAnnounce
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_TxAnnounce(
  EthTSyn_AnnounceSendSmIterType AnnounceSendSmIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const EthTSyn_ComCtrlPortIdxOfAnnounceSendSmType comCtrlPortIdx =
    EthTSyn_GetComCtrlPortIdxOfAnnounceSendSm(AnnounceSendSmIdx);
  EthTSyn_MsgHdrTypeSpecType typeSpecMsgHdr = {0};
  EthTSyn_MsgHdrPortInfoType portInfoMsgHdr = {0};
  EthTSyn_EthTxBufferStructType txBufMgmt = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Prepare the Announce message header for transmission. */
  typeSpecMsgHdr.MessageType          = ETHTSYN_MSG_TYPE_ANNOUNCE;
  typeSpecMsgHdr.MessageLength        = ETHTSYN_MSG_ANNOUNCE_LEN;
  typeSpecMsgHdr.SequenceId           = EthTSyn_GetSequenceIdOfAnnounceSendSmDyn(AnnounceSendSmIdx);
  typeSpecMsgHdr.LogMessageInterval   = EthTSyn_GetAnnounceLogMsgIntervalOfAnnounceSendSm(AnnounceSendSmIdx);
  typeSpecMsgHdr.CorrectionFieldNs    = ETHTSYN_MSG_HDR_COR_FIELD_DEFAULT;
  typeSpecMsgHdr.CorrectionFieldSubNs = ETHTSYN_MSG_HDR_COR_FIELD_SUB_NS_DEFAULT;
  typeSpecMsgHdr.Flags0               = ETHTSYN_MSG_HDR_FLAGS_0_DEFAULT;
  typeSpecMsgHdr.Flags1               = ETHTSYN_MSG_HDR_FLAGS_1_DEFAULT;
  typeSpecMsgHdr.Control              = ETHTSYN_MSG_HDR_CONTROL_ANNOUNCE;

  portInfoMsgHdr.DomainNumber = EthTSyn_ComCtrl_GetTimeDomainId(comCtrlPortIdx);
  portInfoMsgHdr.SourceClockId = EthTSyn_ComCtrl_GetClockId(comCtrlPortIdx);
  portInfoMsgHdr.SourcePortNumber = EthTSyn_GetNumberOfComCtrlPort(comCtrlPortIdx);

  /* #20 Try to start the transmission sequence. */
  txBufMgmt.BufferPtr = NULL_PTR; /* Set BufferPtr to known value for VCA analysis */
  if (EthTSyn_Tx_StartTxSequence(comCtrlPortIdx, EthTSyn_GetSizeOfTxConfContext(), &typeSpecMsgHdr, &portInfoMsgHdr,
                                 &txBufMgmt) == E_OK)
  {
    /*@ assert txBufMgmt.BufferPtr != NULL_PTR; */                                                                     /* VCA_ETHTSYN_TX_BUF_VALID */
    /*@ assert $lengthOf(txBufMgmt.BufferPtr) >= typeSpecMsgHdr.MessageLength; */                                      /* VCA_ETHTSYN_TX_BUF_VALID */
    /* #30 Copy the Announce message payload to the Ethernet TxBuffer (in Network-Byte-Order). */
    /* The field 'currentUtcOffset' is not used and therefore not serialized here. In addition the field is marked as
       invalid via header flag 1 (bit 2 - currentUtcOffsetValid). */
    EthTSyn_Mem_PutUint8(txBufMgmt.BufferPtr, ETHTSYN_MSG_ANNOUNCE_GM_PRIO_1_OFS,
      ETHTSYN_ANNOUNCE_PRIORITY_1_NETWORK_INFRASTRUCTURE);

    EthTSyn_Mem_PutUint8(txBufMgmt.BufferPtr, ETHTSYN_MSG_ANNOUNCE_GM_CLK_QUAL_CLK_CLASS_OFS,
      ETHTSYN_ANNOUNCE_CLOCK_CLASS_UNKNOWN);
    EthTSyn_Mem_PutUint8(txBufMgmt.BufferPtr, ETHTSYN_MSG_ANNOUNCE_GM_CLK_QUAL_CLK_ACCURACY_OFS,
      ETHTSYN_ANNOUNCE_CLOCK_ACCURACY_UNKNOWN);
    EthTSyn_Mem_PutUint16(txBufMgmt.BufferPtr, ETHTSYN_MSG_ANNOUNCE_GM_CLK_QUAL_OFS_SCALED_LOG_VAR_OFS,
      ETHTSYN_ANNOUNCE_OFFSET_SCALED_LOG_UNKNOWN);

    EthTSyn_Mem_PutUint8(txBufMgmt.BufferPtr, ETHTSYN_MSG_ANNOUNCE_GM_PRIO_2_OFS, ETHTSYN_ANNOUNCE_PRIORITY_2_DEFAULT);

    EthTSyn_Mem_PutUint64(txBufMgmt.BufferPtr, ETHTSYN_MSG_ANNOUNCE_GM_IDENTITY_OFS, portInfoMsgHdr.SourceClockId);

    /* The field 'stepsRemoved' is '0' for the time master. Therefore, no serialization is required. */

    EthTSyn_Mem_PutUint8(txBufMgmt.BufferPtr, ETHTSYN_MSG_ANNOUNCE_TIME_SRC_OFS,
      ETHTSYN_ANNOUNCE_TIME_SOURCE_INTERNAL_OSCILLATOR);

    EthTSyn_Mem_PutUint16(txBufMgmt.BufferPtr, ETHTSYN_MSG_ANNOUNCE_PATH_TRACE_TLV_TYPE_OFS,
      ETHTSYN_ANNOUNCE_TLV_TYPE);
    EthTSyn_Mem_PutUint16(txBufMgmt.BufferPtr, ETHTSYN_MSG_ANNOUNCE_PATH_TRACE_TLV_LEN_OFS,
      ETHTSYN_ANNOUNCE_TLV_LENGTH_FIELD);

    /* Add the clock Id to the path trace TLV. */
    EthTSyn_Mem_PutUint64(txBufMgmt.BufferPtr, ETHTSYN_MSG_ANNOUNCE_PATH_TRACE_TLV_PATH_SEQ_START_OFS,
      portInfoMsgHdr.SourceClockId);

    /* #40 Finish the transmission sequence. */
    retVal = EthTSyn_Tx_FinishTxSequence(comCtrlPortIdx, EthTSyn_GetSizeOfTxConfContext(), txBufMgmt.BufferIdx,
      ETHTSYN_MSG_ANNOUNCE_LEN);
  }

  return retVal;
} /* EthTSyn_AnnounceSendSm_TxAnnounce() */

# endif /* ETHTSYN_NOUNIT_ANNOUNCESENDSM */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Public interfaces of AnnounceSendSm
 *!
 * \unit AnnounceSendSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_ANNOUNCESENDSM)

/**********************************************************************************************************************
 *  EthTSyn_AnnounceSendSm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_AnnounceSendSmIterType announceSendSmIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize every announce send state machine. */
  for (announceSendSmIter = 0u; announceSendSmIter < EthTSyn_GetSizeOfAnnounceSendSm(); announceSendSmIter++)
  {
    EthTSyn_AnnounceSendSm_IntInit(announceSendSmIter);
  }
} /* EthTSyn_AnnounceSendSm_Init() */

/**********************************************************************************************************************
 *  EthTSyn_AnnounceSendSm_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_AnnounceSendSmIterType announceSendSmIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Process all AnnounceSendSm. */
  for (announceSendSmIter = 0u; announceSendSmIter < EthTSyn_GetSizeOfAnnounceSendSm(); announceSendSmIter++)
  {
    EthTSyn_AnnounceSendSm_Proc(announceSendSmIter);
  }
} /* EthTSyn_AnnounceSendSm_MainFunction */

/**********************************************************************************************************************
 * EthTSyn_AnnounceSendSm_Reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceSendSm_Reset(
  EthTSyn_AnnounceSendSm_SmIdxType AnnounceSendSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if AnnounceSendSm exists. */
  if (AnnounceSendSmIdx < EthTSyn_GetSizeOfAnnounceSendSm())
  {
    /* #20 Trigger a reset of the passed AnnounceSendSm. */
    EthTSyn_SetResetRequestedOfAnnounceSendSmDyn(AnnounceSendSmIdx, TRUE);
  }
} /* EthTSyn_AnnounceSendSm_Reset() */

# endif /* ETHTSYN_NOUNIT_ANNOUNCESENDSM */

/**********************************************************************************************************************
 * Public interfaces of SyncSendSm
 *!
 * \unit SyncSendSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SYNCSENDSM)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SyncSendSmIterType syncSendSmIter = 0u;
  EthTSyn_TimeBaseUpdateCounterDynIterType tbUpdateCounterIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all SyncSend state machines. */
  for (syncSendSmIter = 0; syncSendSmIter < EthTSyn_GetSizeOfSyncSendSm(); syncSendSmIter++)
  {
    EthTSyn_SyncSendSm_IntInit(syncSendSmIter);
  }

  /* #20 Initialize TimeBaseUpdateCounter handler. */
  for (tbUpdateCounterIter = 0;
       tbUpdateCounterIter < EthTSyn_GetSizeOfTimeBaseUpdateCounterDyn();
       tbUpdateCounterIter++)
  {
    EthTSyn_SetUpdatedOfTimeBaseUpdateCounterDyn(tbUpdateCounterIter, FALSE);
    EthTSyn_SetValueOfTimeBaseUpdateCounterDyn(tbUpdateCounterIter, 0u);
  }
} /* EthTSyn_SyncSendSm_Init() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SyncSendSmIterType syncSendSmIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check all TimeBaseUpdateCounter. */
  EthTSyn_SyncSendSm_CheckTimeBaseUpdateCounter();

  /* #20 Process all SyncSend state machines. */
  for (syncSendSmIter = 0; syncSendSmIter < EthTSyn_GetSizeOfSyncSendSm(); syncSendSmIter++)
  {
    EthTSyn_SyncSendSm_Proc(syncSendSmIter);
  }
} /* EthTSyn_SyncSendSm_MainFunction() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendSm_Reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SyncSendSm_Reset(
  EthTSyn_SyncSendSm_SmIdxType SyncSendSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if SyncSendSm exists. */
  if (SyncSendSmIdx < EthTSyn_GetSizeOfSyncSendSm())
  {
    /* #20 Trigger a reset of the passed SyncSendSm. */
    EthTSyn_SetSmResetRequestedOfSyncSendSmDyn(SyncSendSmIdx, TRUE);
  }

} /* EthTSyn_SyncSendSm_Reset() */

# endif /* ETHTSYN_NOUNIT_SYNCSENDSM */

/**********************************************************************************************************************
 * Public interfaces of SyncSendTx
 *!
 * \unit SyncSendTx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SYNCSENDTX)
/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_VerifyCfg
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, ETHTSYN_CODE) EthTSyn_SyncSendTx_VerifyCfg(
  ETHTSYN_P2VAR(uint8) ErrorIdPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean cfgIsValid = TRUE;
  EthTSyn_SyncSendSmIterType syncSendSmIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify the generated FollowUp Tx length of all SyncSend state machines. */
  for (syncSendSmIter = 0; syncSendSmIter < EthTSyn_GetSizeOfSyncSendSm(); syncSendSmIter++)
  {
    if (EthTSyn_SyncSendTx_IsFollowUpTxLengthValid(syncSendSmIter) == FALSE)
    {
      cfgIsValid = FALSE;
      break;
    }
  }

  if (cfgIsValid == TRUE)
  {
    (*ErrorIdPtr) = ETHTSYN_E_NO_ERROR;
  }
  else
  {
    (*ErrorIdPtr) = ETHTSYN_E_FUP_TX_LENGTH;
  }

  return cfgIsValid;
} /* EthTSyn_SyncSendTx_VerifyCfg() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_TxSync
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncSendTx_TxSync(
                EthTSyn_SyncSendSmIterType SyncSendSmIdx,
                uint16                     SequenceId,
  ETHTSYN_P2VAR(EthTSyn_SyncTxInfoType)    SyncTxInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const EthTSyn_PortSyncSendSm_SmIdxType portSyncSendSmIdx = EthTSyn_GetPortSyncSendSmIdxOfSyncSendSm(SyncSendSmIdx);
  const StbM_SynchronizedTimeBaseType timeBaseId = EthTSyn_ComCtrl_GetSynchronizedTimeBaseId(
    EthTSyn_GetComCtrlPortIdxOfPortSyncSendSm(portSyncSendSmIdx));
  StbM_TimeStampType currGlobalTime = {0u};
  StbM_VirtualLocalTimeType currVlt = {0u};
  StbM_UserDataType userData = {0u};

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize out parameters. */
  SyncTxInfoPtr->SyncTxIntVlt = 0u;
  SyncTxInfoPtr->SyncTxGlobalTime.timeBaseStatus = 0u;
  SyncTxInfoPtr->SyncTxGlobalTime.nanoseconds    = 0u;
  SyncTxInfoPtr->SyncTxGlobalTime.seconds        = 0u;
  SyncTxInfoPtr->SyncTxGlobalTime.secondsHi      = 0u;

  /* #10 Get the current global and virtual local time tuple from StbM via StbM_BusGetCurrentTime() for later
   *     calculation of the precise origin timestamp. */
  if (StbM_BusGetCurrentTime(timeBaseId, &currGlobalTime, &currVlt, &userData) == E_OK)
  {
    /* #20 Check if the timestamp format of the global time is valid. */
    if (EthTSyn_Ts_IsStbmTsValid(&currGlobalTime) == TRUE)
    {
      EthTSyn_MsgHdrTypeSpecType typeSpecMsgHdr = {0};

      typeSpecMsgHdr.Control = ETHTSYN_MSG_HDR_CONTROL_SYNC;
      typeSpecMsgHdr.CorrectionFieldNs = ETHTSYN_MSG_HDR_COR_FIELD_DEFAULT;
      typeSpecMsgHdr.CorrectionFieldSubNs = ETHTSYN_MSG_HDR_COR_FIELD_SUB_NS_DEFAULT;
      typeSpecMsgHdr.Flags0 = ETHTSYN_MSG_HDR_FLAGS_0_SYNC_PDELAY_RESP;
      typeSpecMsgHdr.Flags1 = ETHTSYN_MSG_HDR_FLAGS_1_DEFAULT;
      typeSpecMsgHdr.LogMessageInterval = EthTSyn_GetSyncLogMsgIntervalOfSyncSendSm(SyncSendSmIdx);
      typeSpecMsgHdr.MessageLength = ETHTSYN_MSG_SYNC_LEN;
      typeSpecMsgHdr.MessageType = ETHTSYN_MSG_TYPE_SYNC;
      typeSpecMsgHdr.SequenceId = SequenceId;

      /* #30 Transmit Sync message and check if successful. */
      if (EthTSyn_PortSyncSendSm_TxSync(portSyncSendSmIdx, &typeSpecMsgHdr, NULL_PTR) == E_OK)
      {
        /* #40 Return current global time, internal virtual local time and user data. */
        SyncTxInfoPtr->SyncTxIntVlt = EthTSyn_Ts_VltToIntVlt(&currVlt);
        SyncTxInfoPtr->SyncTxGlobalTime = currGlobalTime;
        SyncTxInfoPtr->SyncTxUserData = userData;

        retVal = E_OK;
      }
    }
  }
  return retVal;
} /* EthTSyn_SyncSendTx_TxSync() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_TxFup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncSendTx_TxFup(
                  EthTSyn_SyncSendSmIterType   SyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_SyncTxInfoType)      SyncTxInfoPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType)  TypeSpecHeaderPtr,
  ETHTSYN_P2CONST(EthTSyn_GlobalTimestampType) PotPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const EthTSyn_PortSyncSendSm_SmIdxType portSyncSendSmIdx = EthTSyn_GetPortSyncSendSmIdxOfSyncSendSm(SyncSendSmIdx);
  EthTSyn_EthTxBufferStructType txBufMgmt = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Start transmission sequence for FollowUp message. */
  if (EthTSyn_PortSyncSendSm_StartTxFupSequence(portSyncSendSmIdx, TypeSpecHeaderPtr, NULL_PTR, &txBufMgmt) == E_OK)
  {
    /*@ assert $external(txBufMgmt.BufferPtr); */                                                                      /* VCA_ETHTSYN_TX_BUF_VALID */
    /*@ assert $lengthOf(txBufMgmt.BufferPtr) >= TypeSpecHeaderPtr->MessageLength; */                                  /* VCA_ETHTSYN_TX_BUF_VALID */
    /* #20 Serialize FollowUp message. */
    const uint16 fupMsgLength = EthTSyn_SyncSendTx_SerializeFup(SyncSendSmIdx, SyncTxInfoPtr, txBufMgmt.BufferPtr,
      PotPtr);

    /* #30 Finish transmission sequence for FollowUp message. */
    retVal = EthTSyn_PortSyncSendSm_FinishTxFupSequence(portSyncSendSmIdx, &txBufMgmt, fupMsgLength);
  }

  return retVal;
} /* EthTSyn_SyncSendTx_TxFup() */

/**********************************************************************************************************************
 *  EthTSyn_SyncSendTx_CalcPot
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
 */
/* PRQA S 6050, 6080 1 */ /* MD_EthTSyn_6050_TrivialFunctions, MD_EthTSyn_6080_TimestampChecking */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncSendTx_CalcPot(
                  EthTSyn_ComCtrl_PortIdxType  ComCtrlPortIdx,
                  EthTSyn_IntVltType           SyncEgrIntVlt,
  ETHTSYN_P2CONST(EthTSyn_SyncTxInfoType)      SyncTxInfoPtr,
    ETHTSYN_P2VAR(EthTSyn_GlobalTimestampType) PotPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_UTimediffType potCorrTime = 0u;
  boolean validCorrTime = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize the out-parameter PotPtr. */
  PotPtr->nanoseconds = 0u;
  PotPtr->seconds = 0u;
  PotPtr->secondsHi = 0u;

  /* #10 Check if hardware timestamping is enabled and StbM and EthIf use different time sources. */
#  if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  if (EthTSyn_IsStbMTimeSrcEqualOfComCtrlPort(ComCtrlPortIdx) == FALSE)
  {
    const uint8 ethIfCtrlIdx = EthTSyn_GetEthIfCtrlIdxOfComCtrlEthIfCtrl(
      EthTSyn_GetComCtrlEthIfCtrlIdxOfComCtrlPort(ComCtrlPortIdx));
    const StbM_SynchronizedTimeBaseType timeBaseId = EthTSyn_ComCtrl_GetSynchronizedTimeBaseId(ComCtrlPortIdx);
    Eth_TimeStampType currEthTime = {0u};
    Eth_TimeStampQualType currEthTimeQual = ETH_VALID;
    StbM_VirtualLocalTimeType currVlt = {0u};
    Std_ReturnType ethGetCurrTimeRetVal = E_NOT_OK;
    Std_ReturnType stbmGetCurrVltRetVal = E_NOT_OK;

    /* #100 Get the current Ethernet time and the current virtual local time within a critical section in order to
     *      decrease the delay between both timestamps. */
    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_GET_TS();
    ethGetCurrTimeRetVal = EthIf_GetCurrentTime(ethIfCtrlIdx, &currEthTimeQual, &currEthTime);
    stbmGetCurrVltRetVal = StbM_GetCurrentVirtualLocalTime(timeBaseId, &currVlt);
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_GET_TS();

    /* #101 Check if both timestamps are valid. */
    if ((ethGetCurrTimeRetVal == E_OK) && (currEthTimeQual == ETH_VALID) && (stbmGetCurrVltRetVal == E_OK))
    {
      EthTSyn_IntVltType currIntVlt = EthTSyn_Ts_VltToIntVlt(&currVlt);

      /* #102 Try to convert the Ethernet timestamp into an internal virtual local time. */
      EthTSyn_IntVltType currEthIntVlt = 0u;
      if (EthTSyn_Ts_TsToIntVltChecked(&currEthTime, &currEthIntVlt) == E_OK)
      {
        /* #103 Check all four timestamps for plausibility. */
        if ((currIntVlt >= SyncTxInfoPtr->SyncTxIntVlt) && (currEthIntVlt >= SyncEgrIntVlt))
        {
          EthTSyn_UTimediffType ethCorrTime =
            (EthTSyn_UTimediffType)currEthIntVlt - (EthTSyn_UTimediffType)SyncEgrIntVlt;
          potCorrTime = (EthTSyn_UTimediffType)currIntVlt - (EthTSyn_UTimediffType)SyncTxInfoPtr->SyncTxIntVlt;

          if (potCorrTime >= ethCorrTime)
          {
            /* #104 Calculate time correction for precise origin timestamp. */
            potCorrTime -= ethCorrTime;
            validCorrTime = TRUE;
          }
        }
      }
    }
  }
  else
#  else
  ETHTSYN_DUMMY_STATEMENT(ComCtrlPortIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
#  endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
  /* #11 Otherwise calculate time correction for precise origin timestamp from Sync egress and Sync transmission
         internal virtual local time. */
  {
    if (SyncEgrIntVlt >= SyncTxInfoPtr->SyncTxIntVlt)
    {
      potCorrTime= (EthTSyn_UTimediffType)SyncEgrIntVlt - (EthTSyn_UTimediffType)SyncTxInfoPtr->SyncTxIntVlt;
      validCorrTime = TRUE;
    }
  }

  /* #20 If the time correction was successfully calculated, add value to precise origin timestamp. */
  if (validCorrTime == TRUE)
  {
    EthTSyn_GlobalTimestampType localPot = {0u};

    localPot.secondsHi = SyncTxInfoPtr->SyncTxGlobalTime.secondsHi;
    localPot.seconds = SyncTxInfoPtr->SyncTxGlobalTime.seconds;
    localPot.nanoseconds = SyncTxInfoPtr->SyncTxGlobalTime.nanoseconds;

    if (EthTSyn_Ts_TsPlusUTimediff(&localPot, potCorrTime) == E_OK)
    {
      (*PotPtr) = localPot;
      retVal = E_OK;
    }
  }

  return retVal;
} /* EthTSyn_SyncSendTx_CalcPot() */

# endif /* ETHTSYN_NOUNIT_SYNCSENDTX */

/**********************************************************************************************************************
 * Public interfaces of PortSyncSendSm
 *!
 * \unit PortSyncSendSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_PORTSYNCSENDSM)
/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_PortSyncSendSmIterType portSyncSendSmIter = 0u;
  EthTSyn_TsInfoType tsInfo = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all master ports. */
  for (portSyncSendSmIter = 0; portSyncSendSmIter < EthTSyn_GetSizeOfPortSyncSendSm(); portSyncSendSmIter++)
  {
    /* #100 Initialize variable data. */
    EthTSyn_SetStateOfPortSyncSendSmDyn(portSyncSendSmIter, ETHTSYN_WAIT_TX_SYNC_STATEOFPORTSYNCSENDSMDYN);
    EthTSyn_SetEgrTsInfoOfPortSyncSendSmDyn(portSyncSendSmIter, tsInfo);
  }
} /* EthTSyn_PortSyncSendSm_Init() */

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_TxSync
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
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_TxSync(
  EthTSyn_PortSyncSendSm_SmIdxType            PortSyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType) TypeSpecHeaderPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType) PortInfoHeaderPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPortSyncSendSm(PortSyncSendSmIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is asCapable. */
  if (EthTSyn_ComCtrl_IsAsCapable(comCtrlPortIdx) == TRUE)
  {
    /* #20 Check if state of PortSyncSendSm is WAIT_TX_SYNC. */
    if (EthTSyn_PortSyncSendSm_IsInState(PortSyncSendSmIdx, ETHTSYN_WAIT_TX_SYNC_STATEOFPORTSYNCSENDSMDYN) == TRUE)
    {
      /* #30 Start transmission sequence and check if successful. */
      EthTSyn_EthTxBufferStructType txBufMgmt = {0};
      txBufMgmt.BufferPtr = NULL_PTR; /* Set BufferPtr to known value for VCA analysis */
      if (EthTSyn_Tx_StartTxSequence(comCtrlPortIdx, EthTSyn_GetTxConfContextIdxOfPortSyncSendSm(PortSyncSendSmIdx),
           TypeSpecHeaderPtr, PortInfoHeaderPtr, &txBufMgmt) == E_OK)
      {
        /*@ assert txBufMgmt.BufferPtr != NULL_PTR; */                                                                 /* VCA_ETHTSYN_TX_BUF_VALID */
        /* #40 Copy serialized Sync message to internal buffer if sync sent callback is configured and buffer can hold
               message. */
        const EthTSyn_SyncSentCbkPtrType syncSentCbkPtr = (
          (EthTSyn_GetMsgSentCbkBufLengthOfPortSyncSendSm(PortSyncSendSmIdx) >= TypeSpecHeaderPtr->MessageLength) ?
          EthTSyn_GetSyncSentCbkPtrOfPortSyncSendSm(PortSyncSendSmIdx) :
          NULL_PTR);

        if (syncSentCbkPtr != NULL_PTR)
        {
          ETHTSYN_P2VAR(uint8) cbkBufPtr = EthTSyn_GetAddrMsgSentCbkBuf(
            EthTSyn_GetMsgSentCbkBufStartIdxOfPortSyncSendSm(PortSyncSendSmIdx));

          /*@ assert $lengthOf(cbkBufPtr) >= TypeSpecHeaderPtr->MessageLength; */                                      /* VCA_ETHTSYN_SYNC_FUP_CBK_BUF_LEN */
          /* PRQA S 0315 2 */ /* MD_MSR_VStdLibCopy */
          VStdLib_MemCpy_s(cbkBufPtr, EthTSyn_GetMsgSentCbkBufLengthOfPortSyncSendSm(PortSyncSendSmIdx),
            txBufMgmt.BufferPtr, TypeSpecHeaderPtr->MessageLength);
        }

        /* #50 Set state of PortSyncSendSm to WAIT_SYNC_EGR_TS. */
        EthTSyn_PortSyncSendSm_SetState(PortSyncSendSmIdx, ETHTSYN_WAIT_SYNC_EGR_TS_STATEOFPORTSYNCSENDSMDYN);

        /* #60 Finish transmission sequence and check if transmission succeeded. */
        if (EthTSyn_Tx_FinishTxSequence(comCtrlPortIdx, EthTSyn_GetTxConfContextIdxOfPortSyncSendSm(PortSyncSendSmIdx),
             txBufMgmt.BufferIdx, TypeSpecHeaderPtr->MessageLength)== E_OK)
        {
          /* #600 Invoke sync sent callback, if it is configured and message is available. */
          if (syncSentCbkPtr != NULL_PTR)
          {
            ETHTSYN_P2CONST(uint8) cbkBufPtr = EthTSyn_GetAddrMsgSentCbkBuf(
              EthTSyn_GetMsgSentCbkBufStartIdxOfPortSyncSendSm(PortSyncSendSmIdx));

            syncSentCbkPtr(comCtrlPortIdx, cbkBufPtr);
          }

          /* #601 Report transmission of Sync message to PortParamStats unit. */
          EthTSyn_PortParamStats_IncTxSyncCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(comCtrlPortIdx));

          retVal = E_OK;
        }
        /* #61 Otherwise reset state of PortSyncSendSm to WAIT_TX_SYNC. */
        else
        {
          EthTSyn_PortSyncSendSm_SetState(PortSyncSendSmIdx, ETHTSYN_WAIT_TX_SYNC_STATEOFPORTSYNCSENDSMDYN);
        }
      }
    }
  }

  return retVal;
} /* EthTSyn_PortSyncSendSm_TxSync() */

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_ProvideSyncEgrTs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_ProvideSyncEgrTs(
  EthTSyn_PortSyncSendSm_SmIdxType    PortSyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_TsInfoType) EgrTsInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section and check if state of PortSyncSendSm is WAIT_SYNC_EGR_TS. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
  if (EthTSyn_GetStateOfPortSyncSendSmDyn(PortSyncSendSmIdx) == ETHTSYN_WAIT_SYNC_EGR_TS_STATEOFPORTSYNCSENDSMDYN)
  {
    /* #20 Store all timestamp information. */
    EthTSyn_SetEgrTsInfoOfPortSyncSendSmDyn(PortSyncSendSmIdx, *EgrTsInfoPtr);

    /* #30 Set state of PortSyncSendSm to WAIT_TX_FUP. */
    EthTSyn_SetStateOfPortSyncSendSmDyn(PortSyncSendSmIdx, ETHTSYN_WAIT_TX_FUP_STATEOFPORTSYNCSENDSMDYN);
  }

  /* #40 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
} /* EthTSyn_PortSyncSendSm_ProvideSyncEgrTs() */

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_GetSyncEgrTs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_GetSyncEgrTs(
  EthTSyn_PortSyncSendSm_SmIdxType  PortSyncSendSmIdx,
  ETHTSYN_P2VAR(EthTSyn_TsInfoType) EgrTsInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  EthTSyn_TsInfoType egrTsInfoTmp = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section and provide all timestamp information. */
  SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();
  /* In case that PortSyncSendSm is not in state WAIT_TX_FUP, timestamp information is used to
   * initialize out-parameter anyway. */
  egrTsInfoTmp = EthTSyn_GetEgrTsInfoOfPortSyncSendSmDyn(PortSyncSendSmIdx);

  /* #20 Check if state of PortSyncSendSm is WAIT_TX_FUP. */
  if (EthTSyn_GetStateOfPortSyncSendSmDyn(PortSyncSendSmIdx) == ETHTSYN_WAIT_TX_FUP_STATEOFPORTSYNCSENDSMDYN)
  {
    retVal = E_OK;
  }

  /* #30 Leave critical section. */
  SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_TX();

  (*EgrTsInfoPtr) = egrTsInfoTmp;

  return retVal;
} /* EthTSyn_PortSyncSendSm_GetSyncEgrTs() */

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_StartTxFupSequence
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_StartTxFupSequence(
  EthTSyn_PortSyncSendSm_SmIdxType               PortSyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrTypeSpecType)    TypeSpecHeaderPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrPortInfoType)    PortInfoHeaderPtr,
    ETHTSYN_P2VAR(EthTSyn_EthTxBufferStructType) TxBufMgmtPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPortSyncSendSm(PortSyncSendSmIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if port is asCapable. */
  if (EthTSyn_ComCtrl_IsAsCapable(comCtrlPortIdx) == TRUE)
  {
    /* #20 Check if state of PortSyncSendSm is WAIT_TX_FUP. */
    if (EthTSyn_PortSyncSendSm_IsInState(PortSyncSendSmIdx, ETHTSYN_WAIT_TX_FUP_STATEOFPORTSYNCSENDSMDYN) == TRUE)
    {
      /* #30 Start transmission sequence and check if successful. */
      TxBufMgmtPtr->BufferPtr = NULL_PTR; /* Set BufferPtr to known value for VCA analysis */
      if (EthTSyn_Tx_StartTxSequence(comCtrlPortIdx, EthTSyn_GetSizeOfTxConfContext(), TypeSpecHeaderPtr,
         PortInfoHeaderPtr, TxBufMgmtPtr) == E_OK)
      {
        /*@ assert TxBufMgmtPtr->BufferPtr != NULL_PTR; */                                                             /* VCA_ETHTSYN_TX_BUF_VALID */
        /* #40 Set state of PortSyncSendSm to TX_FUP. */
        EthTSyn_PortSyncSendSm_SetState(PortSyncSendSmIdx, ETHTSYN_TX_FUP_STATEOFPORTSYNCSENDSMDYN);

        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* EthTSyn_PortSyncSendSm_StartTxFupSequence() */

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_FinishTxFupSequence
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_FinishTxFupSequence(
  EthTSyn_PortSyncSendSm_SmIdxType               PortSyncSendSmIdx,
  ETHTSYN_P2CONST(EthTSyn_EthTxBufferStructType) TxBufMgmtPtr,
                  uint16                         FupLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if state of PortSyncSendSm is TX_FUP. */
  if (EthTSyn_PortSyncSendSm_IsInState(PortSyncSendSmIdx, ETHTSYN_TX_FUP_STATEOFPORTSYNCSENDSMDYN) == TRUE)
  {
    const EthTSyn_FollowUpSentCbkPtrType followUpSentCbkPtr = (
      (EthTSyn_GetMsgSentCbkBufLengthOfPortSyncSendSm(PortSyncSendSmIdx) >= FupLenByte) ?
      EthTSyn_GetFollowUpSentCbkPtrOfPortSyncSendSm(PortSyncSendSmIdx) :
      NULL_PTR);
    const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx = EthTSyn_GetComCtrlPortIdxOfPortSyncSendSm(PortSyncSendSmIdx);

    /* #20 Read master time validation data from Tx buffer before transmission of FollowUp message is finished and
     *     buffer is released (if master time validation is enabled). */
#  if (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON)
    StbM_EthTimeMasterMeasurementType timeValidData = {0u};
    Std_ReturnType getTimeValidDataRetVal = E_NOT_OK;
    if (EthTSyn_IsTimeValidationEnabledOfPortSyncSendSm(PortSyncSendSmIdx))
    {
      getTimeValidDataRetVal = EthTSyn_PortSyncSendSm_GetTimeValidData(PortSyncSendSmIdx,
        TxBufMgmtPtr->BufferPtr, &timeValidData);
    }
#  endif /* (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) */

    /* #30 Copy serialized Follow Up message to internal buffer if Follow Up sent callback is configured and buffer
           can hold message. */
    if (followUpSentCbkPtr != NULL_PTR)
    {
      ETHTSYN_P2VAR(uint8) cbkBufPtr = EthTSyn_GetAddrMsgSentCbkBuf(
        EthTSyn_GetMsgSentCbkBufStartIdxOfPortSyncSendSm(PortSyncSendSmIdx));

      /*@ assert $lengthOf(cbkBufPtr) >= FupLenByte; */                                                                /* VCA_ETHTSYN_SYNC_FUP_CBK_BUF_LEN */
      /* PRQA S 0315 2 */ /* MD_MSR_VStdLibCopy */
      VStdLib_MemCpy_s(cbkBufPtr, EthTSyn_GetMsgSentCbkBufLengthOfPortSyncSendSm(PortSyncSendSmIdx),
        TxBufMgmtPtr->BufferPtr, FupLenByte);
    }

    /* #40 Finish transmission sequence and check if transmission succeeds. */
    if (EthTSyn_Tx_FinishTxSequence(comCtrlPortIdx, EthTSyn_GetSizeOfTxConfContext(), TxBufMgmtPtr->BufferIdx,
       FupLenByte)== E_OK)
    {
      /* #400 Invoke Follow Up sent callback, if it is configured and message is available. */
      if (followUpSentCbkPtr != NULL_PTR)
      {
        ETHTSYN_P2CONST(uint8) cbkBufPtr = EthTSyn_GetAddrMsgSentCbkBuf(
          EthTSyn_GetMsgSentCbkBufStartIdxOfPortSyncSendSm(PortSyncSendSmIdx));

        followUpSentCbkPtr(comCtrlPortIdx, cbkBufPtr);
      }

      /* #401 Provide FollowUp message time validation data to StbM (if master time validation is enabled). */
#  if (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON)
      if (getTimeValidDataRetVal == E_OK)
      {
        const StbM_SynchronizedTimeBaseType tbId = EthTSyn_GetSynchronizedTimeBaseIdOfComCtrlTimeDomain(
          EthTSyn_GetComCtrlTimeDomainIdxOfComCtrlPort(comCtrlPortIdx));
        (void)StbM_EthSetMasterTimingData(tbId, &timeValidData);
      }
#  endif /* (ETHTSYN_MASTER_PD_RESP_TIME_VALIDATION_SUPPORT == STD_ON) */

      /* #402 Report transmission of FollowUp message to PortParamStats unit. */
      EthTSyn_PortParamStats_IncTxFupCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(comCtrlPortIdx));

      /* #403 Set state of PortSyncSendSm to WAIT_TX_SYNC. */
      EthTSyn_PortSyncSendSm_SetState(PortSyncSendSmIdx, ETHTSYN_WAIT_TX_SYNC_STATEOFPORTSYNCSENDSMDYN);

      retVal = E_OK;
    }
    /* #41 Otherwise reset state of PortSyncSendSm to WAIT_TX_FUP. */
    else
    {
      EthTSyn_PortSyncSendSm_SetState(PortSyncSendSmIdx, ETHTSYN_WAIT_TX_FUP_STATEOFPORTSYNCSENDSMDYN);
    }
  }

  return retVal;
} /* EthTSyn_PortSyncSendSm_FinishTxFupSequence() */

/**********************************************************************************************************************
 *  EthTSyn_PortSyncSendSm_CancelCycle
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_PortSyncSendSm_CancelCycle(
  EthTSyn_PortSyncSendSm_SmIdxType PortSyncSendSmIdx)
{
  /* #10 Set state of PortSyncSendSm to WAIT_TX_SYNC. */
  EthTSyn_PortSyncSendSm_SetState(PortSyncSendSmIdx, ETHTSYN_WAIT_TX_SYNC_STATEOFPORTSYNCSENDSMDYN);
} /* EthTSyn_PortSyncSendSm_CancelCycle() */

# endif /* ETHTSYN_NOUNIT_PORTSYNCSENDSM */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (ETHTSYN_MASTER_SUPPORT == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Master.c
 *********************************************************************************************************************/
