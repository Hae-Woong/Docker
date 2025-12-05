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
/*!        \file  EthTSyn_Slave.c
 *        \brief  EthTSyn Slave source file
 *      \details  Implementation of all units included in the logical unit Slave (AnnounceRcvSm, SyncRcvSm, SlaveRx and
 *                FupRxHandler).
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

#define ETHTSYN_SLAVE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "EthTSyn_Cfg.h"
#include "EthTSyn_GeneratedCfgAccess.h"
#include "EthTSyn.h"
#include "EthTSyn_SlaveRx_Int.h"
#include "EthTSyn_SyncRcvSm_Int.h"
#include "EthTSyn_AnnounceRcvSm_Int.h"
#include "EthTSyn_FupRxHandler_Int.h"
#include "EthTSyn_SiteSyncSyncSm_Int.h"
#include "EthTSyn_AsymSiteSyncSyncSm_Int.h"
#include "EthTSyn_CrcHndl_Int.h"
#include "EthTSyn_MsgDefs_Int.h"
#include "EthTSyn_Memory_Int.h"
#include "EthTSyn_ComCtrl_Int.h"
#include "EthTSyn_SwtMgmtRx_Int.h"
#include "EthTSyn_Timestamp_Int.h"
#include "EthTSyn_SwtTimeSyncSm_Int.h"
#include "EthTSyn_PortParamStats_Int.h"

#include "StbM.h"
#include "SchM_EthTSyn.h"

#if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
# include "Det.h"
#endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON) */

#if (ETHTSYN_SLAVE_SUPPORT == STD_ON) /* COV_MSR_UT_OPTIONAL_UNIT */
/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Constants required by the FupRxHandler */
# define ETHTSYN_FUPRXHANDLER_POT_CORR_TIME_SUMMANDS                  (4u)

# define ETHTSYN_FUPRXHANDLER_MAX_TIME_SECURED_SUB_TLV_CNT            (1u)
# define ETHTSYN_FUPRXHANDLER_MAX_STATUS_SUB_TLV_CNT                  (1u)
# define ETHTSYN_FUPRXHANDLER_MAX_USER_DATA_SUB_TLV_CNT               (1u)
# define ETHTSYN_FUPRXHANDLER_MAX_OFS_SUB_TLV_CNT                     (16u)
/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
# if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
typedef struct
{
  uint8 TimeSecuredTlvCnt;
  uint8 StatusTlvCnt;
  uint8 UserDataTlvCnt;
  uint8 OfsTlvCnt;
} EthTSyn_FupRxHandler_ArSubTlvCounterType;
# endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

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
 * Internal function prototypes of AnnounceRcvSm
 *!
 * \unit AnnounceRcvSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_ANNOUNCERCVSM)
/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_IntInit
 *********************************************************************************************************************/
/*! \brief        Initializes all variable data of an Announce receive state machine.
 *  \details      -
 *  \param[in]    AnnounceRcvSmIdx Index of the Announce receive state machine.
 *                                 [range: AnnounceRcvSmIdx < EthTSyn_GetSizeOfAnnounceRcvSm()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \spec
 *    requires AnnounceRcvSmIdx < EthTSyn_GetSizeOfAnnounceRcvSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_IntInit(
  EthTSyn_AnnounceRcvSmIterType AnnounceRcvSmIdx);

/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_IsAnnounceValid
 *********************************************************************************************************************/
/*! \brief        Checks if received Announce message is valid.
 *  \details      -
 *  \param[in]    AnnounceRcvSmIdx   Index of the Announce receive state machine.
 *                                   [range: SyncSendSmIdx < EthTSyn_GetSizeOfAnnounceRcvSm()]
 *  \param[in]    PtpHdrPtr          Pointer to the de-serialized PTP header.
 *  \param[in]    AnnouncePayloadPtr Pointer to the Announce payload (still serialized in Network-Byte-Order).
 *  \return       TRUE - Announce message is valid
 *  \return       FALSE - Announce message is invalid
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *  \spec
 *    requires AnnounceRcvSmIdx < EthTSyn_GetSizeOfAnnounceRcvSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_IsAnnounceValid(
                  EthTSyn_AnnounceRcvSm_SmIdxType AnnounceRcvSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)             PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                          AnnouncePayloadPtr);

/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_CheckPathTraceTlv
 *********************************************************************************************************************/
/*! \brief        Checks if the path trace TLV of a received Announce message is valid.
 *  \details      -
 *  \param[in]    AnnounceMsgLen     Length of the Announce message from the PTP header
 *  \param[in]    LocalClockId       ClockId of the port the Announce message was received on
 *  \param[in]    AnnouncePayloadPtr Pointer to the Announce payload (still serialized in Network-Byte-Order).
 *  \return       TRUE - Path trace TLV is valid
 *  \return       FALSE - Path trace TLV is invalid
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_CheckPathTraceTlv(
                  uint16                          AnnounceMsgLen,
                  uint64                          LocalClockId,
  ETHTSYN_P2CONST(uint8)                          AnnouncePayloadPtr);
# endif /* ETHTSYN_NOUNIT_ANNOUNCERCVSM */

/**********************************************************************************************************************
 * Internal function prototypes of SyncRcvSm
 *!
 * \unit SyncRcvSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SYNCRCVSM)
/**********************************************************************************************************************
 * EthTSyn_SyncRcvSm_IntInit
 *********************************************************************************************************************/
/*! \brief          Initializes the passed Sync receive state machine
 *  \details        -
 *  \param[in]      SyncRcvSmIdx  Index of the Sync receive state machine.
 *                                [range: SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncRcvSm_IntInit(
  EthTSyn_SyncRcvSmIterType SyncRcvSmIdx);

/**********************************************************************************************************************
 * EthTSyn_SyncRcvSm_IsSyncSequenceIdValid
 *********************************************************************************************************************/
/*! \brief          Checks if the sequence id of a received Sync message is valid.
 *  \details        -
 *  \param[in]      SyncRcvSmIdx    Index of the Sync receive state machine.
 *                                  [range: SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm()]
 *  \param[in]      SyncSequenceId  Sequence Id of the received Sync message
 *  \return         TRUE - Sequence id of the received Sync message is valid
 *  \return         FALSE - Sequence id of the received Sync message is invalid
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SyncRcvSm_IsSyncSequenceIdValid(
  EthTSyn_SyncRcvSm_SmIdxType SyncRcvSmIdx,
  uint16                      SyncSequenceId);

/**********************************************************************************************************************
 * EthTSyn_SyncRcvSm_IsFupMatchingSync
 *********************************************************************************************************************/
/*! \brief          Checks if the FollowUp message matches the last received Sync message.
 *  \details        Checks if both the last received Sync and the FollowUp message are originated from the same time
 *                  master. Verifies that both messages belong to the same cycle.
 *  \param[in]      SyncRcvSmIdx    Index of the Sync receive state machine.
 *                                  [range: SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm()]
 *  \param[in]      PtpHdrPtr       Pointer to the de-serialized PTP header
 *  \return         TRUE - FollowUp message belongs to the last received Sync message
 *  \return         FALSE - FollowUp message does not match the last received Sync message. Drop the FollowUp.
 *  \pre            Must only be called when the SyncRcvSm is in the state WAIT_RX_FUP
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires SyncRcvSmIdx < EthTSyn_GetSizeOfSyncRcvSm();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SyncRcvSm_IsFupMatchingSync(
                  EthTSyn_SyncRcvSm_SmIdxType SyncRcvSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)         PtpHdrPtr);
# endif /* ETHTSYN_NOUNIT_SYNCRCVSM */

/**********************************************************************************************************************
 * Internal function prototpyes of SlaveRx
 *!
 * \unit SlaveRx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SLAVERX)
/**********************************************************************************************************************
 * EthTSyn_SlaveRx_ResetSlaveRxData
 *********************************************************************************************************************/
/*! \brief          Resets all linked state machines and data.
 *  \details        -
 *  \param[in]      SlaveRxDataIdx  Index of the slave rx data
 *                                  [range: SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ResetSlaveRxData(
  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_GetSyncRxTimeInfo
 *********************************************************************************************************************/
/*! \brief      Calculates Sync reception time information which are relevant for later processing from
 *              passed timestamps.
 *  \details    Calculates the following timestamps:
 *              - StbM virtual local time after Sync reception (is equal to the Sync ingress timestamp if
 *                SW-timestamping is used or if the StbM uses the Ethernet controller as time source)
 *              - Time which passed between Sync ingress and retrieving the virtual local time
 *              - Sync ingress timestamp
 *              - Switch residence time (set to "0" if switch management is disabled)
 *              - Switch ingress timestamp (only if switch management is enabled)
 *  \param[in]  SlaveRxDataHandlerIdx Index of the SlaveRxDataHandler mapped to the ComCtrlPort the message was
 *                                    received on
 *                                    [range: SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler()]
 *  \param[in]  RxTsInfoPtr           Info about the reception timestamp(s)
 *  \return     Time information of Sync reception
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 * \spec
 *    requires SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler();
 * \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SyncRxTimeInfoType, ETHTSYN_CODE) EthTSyn_SlaveRx_GetSyncRxTimeInfo(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)              RxTsInfoPtr);

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdSyncEndStation
 *********************************************************************************************************************/
/*! \brief      Processes a received Sync message when EthTSyn operates in end station mode.
 *  \details    -
 *  \param[in]  SlaveRxDataHandlerIdx  Index of the SlaveRxDataHandler mapped to the ComCtrlPort the message was
 *                                     received on
 *                                     [range: SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler()]
 *  \param[in]  PtpHdrPtr              Pointer to the de-serialized PTP header
 *  \param[in]  RxTsInfoPtr            Info about the reception timestamp(s)
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 * \spec
 *    requires SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler();
 * \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdSyncEndStation(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)              RxTsInfoPtr);
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdSyncBridge
 *********************************************************************************************************************/
/*! \brief      Processes a received Sync message when EthTSyn operates in bridge mode.
 *  \details    Forwards the received Sync message to the external slave port and, if it is excepted by the external
 *              slave, optionally to the internal slave and the SiteSyncSyncSm.
 *  \param[in]  SlaveRxDataHandlerIdx  Index of the SlaveRxDataHandler mapped to the ComCtrlPort the message was
 *                                     received on
 *                                     [range: SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler()]
 *  \param[in]  PtpHdrPtr              Pointer to the de-serialized PTP header
 *  \param[in]  RxTsInfoPtr            Info about the reception timestamp(s)
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 * \spec
 *    requires SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler();
 * \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdSyncBridge(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)              RxTsInfoPtr);
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_FwdRcvdSync
 *********************************************************************************************************************/
/*! \brief      Forwards a received Sync message to the SyncRcvSm unit.
 *  \details    Verifies the source port identity of a received Sync message, optionally reports an TSCONFLICT to the
 *              DET and forwards a valid Sync to the SyncRcvSm unit for further processing.
 *  \param[in]  SlaveRxDataIdx  Index of the SlaveRxData
 *                              [range: SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData()]
 *  \param[in]  PtpHdrPtr       Pointer to the de-serialized PTP header
 *  \param[in]  RxTsInfoPtr     Info about the reception timestamp(s)
 *  \return     E_OK - Sync message successfully forwarded
 *  \return     E_NOT_OK - Sync message is either discarded due to failing source port identity check or rejected by
 *                         the SyncRcvSm unit
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 * \spec
 *    requires SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData();
 * \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_FwdRcvdSync(
                  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)         PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_SyncRxTimeInfoType) SyncRxTimeInfoPtr);

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_CheckSyncSrcClockId
 *********************************************************************************************************************/
/*! \brief      Checks if the Sync message shall be received according to the originating time master.
 *  \details    -
 *  \param[in]  SlaveRxDataIdx   Index of the SlaveRxData mapped to the ComCtrlPort the message was received on
 *                               [range: SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData()]
 *  \param[in]  SyncSrcClockId   The source clock identity of the received Sync message
 *  \return     TRUE - Sync message is accepted according to its source clock identity
 *  \return     FALSE - Sync message shall be discarded due to its source clock identity
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 *  \spec
 *    requires SlaveRxDataIdx < EthTSyn_GetSizeOfSlaveRxData();
 *  \endpsec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SlaveRx_CheckSyncSrcClockId(
  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx,
  uint64                      SyncSrcClockId);

/**********************************************************************************************************************
 * EthTSyn_SlaveRx_CheckFupMsgLen
 *********************************************************************************************************************/
/*! \brief          Checks if the message length included in the FollowUp PTP-Header is valid.
 *  \details        -
 *  \param[in]      FupMsgLen  Message length according to the PTP-Header field
 *  \return         TRUE - Message length in PTP-Header is valid
 *  \return         FALSE - Message length in PTP-Header is invalid
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      TRUE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SlaveRx_CheckFupMsgLen(
  uint16 FupMsgLen);

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdFupEndStation
 *********************************************************************************************************************/
/*! \brief      Processes a received FollowUp message when EthTSyn operates in end station mode.
 *  \details    -
 *  \param[in]  SlaveRxDataHandlerIdx  Index of the SlaveRxDataHandler mapped to the ComCtrlPort the message was
 *                                     received on
 *                                     [range: SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler()]
 *  \param[in]  PtpHdrPtr              Pointer to the de-serialized PTP header
 *  \param[in]  FupPayloadPtr          Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 * \spec
 *    requires SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler();
 * \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdFupEndStation(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                             FupPayloadPtr);
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdFupBridge
 *********************************************************************************************************************/
/*! \brief      Processes a received FollowUp message when EthTSyn operates in bridge mode.
 *  \details    -
 *  \param[in]  SlaveRxDataHandlerIdx  Index of the SlaveRxDataHandler mapped to the ComCtrlPort the message was
 *                                     received on
 *                                     [range: SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler()]
 *  \param[in]  PtpHdrPtr              Pointer to the de-serialized PTP header
 *  \param[in]  FupPayloadPtr          Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \pre        -
 *  \context    TASK|ISR2
 *  \reentrant  FALSE
 * \spec
 *    requires SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler();
 * \endspec
 *********************************************************************************************************************/
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdFupBridge(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                             FupPayloadPtr);
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
# endif /* ETHTSYN_NOUNIT_SLAVERX */

/**********************************************************************************************************************
 * Internal function prototypes of FupRxHandler
 *!
 * \unit FupRxHandler
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_FUPRXHANDLER)
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_ResetTlvInfo
 *********************************************************************************************************************/
/*! \brief          Resets the stored TLV information.
 *  \details        -
 *  \param[in]      FupRxHandlerIdx  Index of the FupRxHandler
 *                                   [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_ResetTlvInfo(
  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx);

/**********************************************************************************************************************
 *  EthTSyn_FupRxHandler_CheckFupMsgStructureAndCrc
 *********************************************************************************************************************/
/*! \brief          Checks if the structure and CRC of a received FollowUp message is valid.
 *  \details        -
 *  \param[in]      FupRxHandlerIdx  Index of the FupRxHandler
 *                                   [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[in]      PtpHdrPtr        Pointer to the de-serialized PTP header
 *  \param[in]      FupPayloadPtr    Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \return         TRUE - FollowUp message structure and CRC is valid
 *  \return         FALSE - FollowUp message structure or CRC is invalid
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_CheckFupMsgStructureAndCrc(
                  EthTSyn_FupRxHandler_IdxType           FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                    PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                                 FupPayloadPtr);

/**********************************************************************************************************************
 *  EthTSyn_FupRxHandler_CheckArTlvHeader
 *********************************************************************************************************************/
/*! \brief          Checks the validity of the AR Tlv-Header (if present) and extracts the total length of the Sub-Tlvs.
 *  \details        -
 *  \param[in]      FupPayloadPtr      Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \param[in]      FupMsgLength       The total length of the FollowUp message in byte
 *  \param[out]     TotalSubTlvLength  The total length of all Ar Sub-Tlvs (i.e. Length field of the Ar Tlv-Header -
 *                                     length of the Ar Tlv-Header itself) in byte
 *  \return         TRUE - Check of Ar Tlv Header passed.
 *  \return         FALSE - Check of Ar Tlv Header failed.
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_CheckArTlvHeader(
  ETHTSYN_P2CONST(uint8)  FupPayloadPtr,
                  uint16  FupMsgLength,
    ETHTSYN_P2VAR(uint16) TotalSubTlvLength);

/**********************************************************************************************************************
 *  EthTSyn_FupRxHandler_CheckArSubTlvs
 *********************************************************************************************************************/
/*! \brief          Checks the validity of all AR Sub-Tlvs.
 *  \details        -
 *  \param[in]      FupRxHandlerIdx    Index of the FupRxHandler
 *                                     [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[in]      PtpHdrPtr          Pointer to the de-serialized PTP header
 *  \param[in]      FupPayloadPtr      Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \param[in]      TotalSubTlvLength  The total length of all Ar Sub-Tlvs (i.e. Length field of the Ar Tlv-Header -
 *                                     length of the Ar Tlv-Header itself) in byte
 *  \return         TRUE - Check of Ar Sub-Tlvs passed.
 *  \return         FALSE - Check of Ar Sub-Tlvs failed.
 *  \pre            EthTSyn_SlaveRx_CheckArTlvHeader() passed and TotalSubTlvLength is the one returned by it.
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_CheckArSubTlvs(
                  EthTSyn_FupRxHandler_IdxType           FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                    PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                                 FupPayloadPtr,
                  uint16                                 TotalSubTlvLength);

/**********************************************************************************************************************
 *  EthTSyn_FupRxHandler_CheckArSubTlvLengthAndGetFlag
 *********************************************************************************************************************/
/*! \brief          Checks validity of the Sub-TLV type and the corresponding Sub-TLV length and gets the corresponding
 *                  flag as defined in EthTSyn_Cfg.h.
 *  \details        -
 *  \param[in]      ArSubTlvType    Type the the ArSubTlv
 *  \param[in]      ArSubTlvLength  Length in the ArSubTlv header
 *  \param[out]     ArSubTlvFlag    The Sub-TLV flag
 *  \return         TRUE - Either Sub-TLV type and length match or the TLV type is unkown
 *  \return         FALSE - Sub-TLV type and length do not match
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_CheckArSubTlvLengthAndGetFlag(
                uint8  ArSubTlvType,
                uint8  ArSubTlvLength,
  ETHTSYN_P2VAR(uint8) ArSubTlvFlag);

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetArSubTlvInfoIfValid
 *********************************************************************************************************************/
/*! \brief          Extracts ArSubTlv information if the TLV is valid.
 *  \details        -
 *  \param[in]      FupRxHandlerIdx     Index of the FupRxHandler
 *                                      [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[in]      PtpHdrPtr           Pointer to the de-serialized PTP header
 *  \param[in]      FupPayloadPtr       Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \param[in]      SubTlvOfs           Offset to the start/header of the Sub-TLV relative to the FupPayload
 *                                      [range: SubTlvOfs < len(FupPayloadPtr)]
 *  \param[in,out]  ArSubTlvCounterPtr  Counter for included ArSubTLVs
 *                                      In: Count of already processed ArSubTLVs
 *                                      Out: Incremented counter value for the processed ArSubTLV
 *  \return         TRUE - TLV is valid and information where extracted if possible
 *  \return         FALSE - TLV is invalid. No information where extracted
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetArSubTlvInfoIfValid(
                  EthTSyn_FupRxHandler_IdxType              FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                       PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                                    FupPayloadPtr,
                  uint16                                    SubTlvOfs,
    ETHTSYN_P2VAR(EthTSyn_FupRxHandler_ArSubTlvCounterType) ArSubTlvCounterPtr);

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetOfsFromTlvIfValid
 *********************************************************************************************************************/
/*! \brief          Updated the Ofs ArSubTLV information if the ArSubTLV is valid for the slave.
 *  \details        -
 *  \param[in]      FupRxHandlerIdx   Index of the FupRxHandler
 *                                    [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[in]      ArSubTlvStartPtr  Pointer to the start of the Ofs ArSubTLV
 *  \return         TRUE - Ofs SubTLV is valid
 *  \return         FALSE - Ofs SubTLV is invalid (i.e. duplicated)
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetOfsFromTlvIfValid(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(uint8)                       ArSubTlvStartPtr);

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetStatusFromTlv
 *********************************************************************************************************************/
/*! \brief          Reads the status from the status ArSubTLV if the ArSubTLV is valid for the slave.
 *  \details        -
 *  \param[in]      FupRxHandlerIdx     Index of the FupRxHandler
 *                                      [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[in]      ArSubTlvStartPtr  Pointer to the start of the Status ArSubTLV
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetStatusFromTlv(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(uint8)                       ArSubTlvStartPtr);

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetUserDataFromTlv
 *********************************************************************************************************************/
/*! \brief          Reads the user data from the user data ArSubTLV if the ArSubTLV is valid for the slave.
 *  \details        -
 *  \param[in]      FupRxHandlerIdx     Index of the FupRxHandler
 *                                      [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[in]      ArSubTlvStartPtr  Pointer to the start of the UserData ArSubTLV
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetUserDataFromTlv(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(uint8)                       ArSubTlvStartPtr);
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_ForwardDataToStbM
 *********************************************************************************************************************/
/*! \brief          Forwards synchronization data (including optional new offsets + time validation data) to the StbM.
 *  \details        -
 *  \param[in]      FupRxHandlerIdx  Index of the FupRxHandler
 *                                   [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[in]      SyncTimeInfoPtr  Time information of Sync reception
 *  \param[in]      PtpHdrPtr        Pointer to the de-serialized PTP header
 *  \param[in]      FupPayloadPtr    Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_ForwardDataToStbM(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_SyncRxTimeInfoType)  SyncTimeInfoPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)          PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                       FupPayloadPtr);

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetStatusAndUserData
 *********************************************************************************************************************/
/*! \brief          Gets the status and user data of the FupRxHandler.
 *  \details        -
 *  \param[in]      FupRxHandlerIdx  Index of the FupRxHandler
 *                                   [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[out]     TbStatusPtr      The time base status
 *  \param[in,out]  UserDataPtrPtr   Pointer to the user data
 *                                   In: Pointer to UserDataPtr. The UserDataPtr must point to valid memory where the
 *                                       user data can be stored
 *                                   Out: Either NULL_PTR when no user data are available. Otherwise, the user data are
 *                                        copied to the UserDataPtr location
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetStatusAndUserData(
                              EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
                ETHTSYN_P2VAR(StbM_TimeBaseStatusType)     TbStatusPtr,
  ETHTSYN_P2VAR(ETHTSYN_P2VAR(StbM_UserDataType))          UserDataPtrPtr);

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetTotalPotCorrTime
 *********************************************************************************************************************/
/*! \brief          Gets the overall POT correction time.
 *  \details        Checks plausibility of all POT correction time values and calculates the overall correction time.
*                   Time values are considered as plausible when they are positive and the sum fits in the
 *                  EthTSyn_TimediffType.
 *  \param[in]      FupRxHandlerIdx  Index of the FupRxHandler
 *                                   [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[in]      TimeSinceSyncRx  Time difference since sync reception
 *  \param[in]      SwtResTime       Switch residence time
 *  \param[in]      CorrField        The correction field value of the FollowUp message
 *  \param[out]     PotCorrTimePtr   The total POT correction time
 *  \return         E_OK - Successfully calculated the total POT correction time
 *  \return         E_NOT_OK - Failed to calculate the total POT correction time
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetTotalPotCorrTime(
                EthTSyn_FupRxHandler_IdxType  FupRxHandlerIdx,
                EthTSyn_UTimediffType         TimeSinceSyncRx,
                EthTSyn_UTimediffType         SwtResTime,
                uint64                        CorrField,
  ETHTSYN_P2VAR(EthTSyn_UTimediffType)        PotCorrTimePtr);

#  if (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_ProvideTimeValidationDataToStbM
 *********************************************************************************************************************/
/*! \brief          Provides the time validation data to the StbM.
 *  \details        -
 *  \param[in]      TimeBaseId     Identifier of the synchronized time base
 *  \param[in]      SyncIngrTs     Ingress timestamp of the Sync message
 *  \param[in]      Pdelay         Path delay used for calculation of the new global time
 *  \param[in]      PtpHdrPtr      Pointer to the de-serialized PTP header
 *  \param[in]      FupPayloadPtr  Pointer to the FollowUp payload (still serialized in Network-Byte-Order)
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_ProvideTimeValidationDataToStbM(
                  StbM_SynchronizedTimeBaseType TimeBaseId,
                  EthTSyn_IntVltType            SyncIngrTs,
                  EthTSyn_PdelayType            Pdelay,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)           PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                        FupPayloadPtr);
#  endif /* (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON) */

#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_ProvideOffsetsToStbM
 *********************************************************************************************************************/
/*! \brief          Provides new offset values for offset time bases to the StbM
 *  \details        Checks if the received FollowUp contains offset ArSubTLVs for one or more active offset time bases.
 *                  Forwards the received offsets to the StbM if this is the case.
 *  \param[in]      FupRxHandlerIdx  Index of the FupRxHandler
 *                                   [range: FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler()]
 *  \param[in]      SyncIngrTs       Ingress timestamp of the Sync message
 *  \pre            -
 *  \context        TASK|ISR2
 *  \reentrant      FALSE
 *  \spec
 *    requires FupRxHandlerIdx < EthTSyn_GetSizeOfFupRxHandler();
 *  \endspec
 *********************************************************************************************************************/
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_ProvideOffsetsToStbM(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
                  EthTSyn_IntVltType           SyncIngrTs);
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */
# endif /* ETHTSYN_NOUNIT_FUPRXHANDLER */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Internal functions of AnnounceRcvSm
 *!
 * \unit AnnounceRcvSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_ANNOUNCERCVSM)
/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_IntInit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_IntInit(
  EthTSyn_AnnounceRcvSmIterType AnnounceRcvSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize all variable data of Announce receive state machine. */
  EthTSyn_SetStateOfAnnounceRcvSmDyn(AnnounceRcvSmIdx, ETHTSYN_NO_VALID_MASTER_STATEOFANNOUNCERCVSMDYN);
  EthTSyn_SetRxTimeoutCounterOfAnnounceRcvSmDyn(AnnounceRcvSmIdx, 0u);
  EthTSyn_SetSmResetRequestedOfAnnounceRcvSmDyn(AnnounceRcvSmIdx, FALSE);
} /* EthTSyn_AnnounceRcvSm_IntInit() */

/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_IsAnnounceValid
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
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_IsAnnounceValid(
                  EthTSyn_AnnounceRcvSm_SmIdxType AnnounceRcvSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)             PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                          AnnouncePayloadPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that the number of path steps does not exceed the maximum configured value. */
  if (EthTSyn_Mem_GetUint16(AnnouncePayloadPtr, ETHTSYN_MSG_ANNOUNCE_STEPS_REMOVED_PL_REL_OFS) <=
       EthTSyn_GetMaxStepsRemovedOfAnnounceRcvSm(AnnounceRcvSmIdx))
  {
    const uint64 localClockId = EthTSyn_ComCtrl_GetClockId(EthTSyn_GetComCtrlPortIdxOfAnnounceRcvSm(AnnounceRcvSmIdx));
    const uint64 gmClockId = EthTSyn_Mem_GetUint64(AnnouncePayloadPtr, ETHTSYN_MSG_ANNOUNCE_GM_IDENTITY_PL_REL_OFS);

    /* #20 Check that Announce message is not send from own system. */
    if ((PtpHdrPtr->SourceClockId != localClockId) && (gmClockId != localClockId))
    {
      /* #30 Check validity of the path trace TLV. */
      if (EthTSyn_AnnounceRcvSm_CheckPathTraceTlv(PtpHdrPtr->MessageLength, localClockId, AnnouncePayloadPtr) == TRUE)
      {
        /* #40 If a valid master was already provided to the SlaveRx unit, check that Announce message is sent from
              same master. Perform checks within critical section. */
        SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();
        if (EthTSyn_GetStateOfAnnounceRcvSmDyn(AnnounceRcvSmIdx) == ETHTSYN_VALID_MASTER_SET_STATEOFANNOUNCERCVSMDYN)
        {
          const EthTSyn_SizeOfSlaveRxDataType slaveRxDataIdx =
            EthTSyn_GetSlaveRxDataIdxOfAnnounceRcvSm(AnnounceRcvSmIdx);
          isValid = EthTSyn_SlaveRx_CheckValidMasterIfSet(slaveRxDataIdx, gmClockId);
        }
        /* #41 Otherwise, consider announce message as valid. */
        else
        {
          isValid = TRUE;
        }
        SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();
      }
    }
  }

  return isValid;
} /* EthTSyn_AnnounceRcvSm_IsAnnounceValid() */

/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_CheckPathTraceTlv
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_CheckPathTraceTlv(
                  uint16                          AnnounceMsgLen,
                  uint64                          LocalClockId,
  ETHTSYN_P2CONST(uint8)                          AnnouncePayloadPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean pathTraceTlvValid = FALSE;
  const uint16 pathTraceTlvLen = EthTSyn_Mem_GetUint16(AnnouncePayloadPtr,
    ETHTSYN_MSG_ANNOUNCE_PATH_TRACE_TLV_LEN_PL_REL_OFS);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that announce path trace TLV length is in accordance with the Announce message length. */
  if (pathTraceTlvLen <= (AnnounceMsgLen - ETHTSYN_MSG_ANNOUNCE_LEN_WO_PATH_TRACE_ELEM))
  {
    /* #20 Check that announce path trace TLV length field is a multiple of the TLV length. */
    if ((pathTraceTlvLen % ETHTSYN_MSG_ANNOUNCE_PATH_SEQUENCE_ELEMENT_LENGTH) == 0u)
    {
      const uint16 pathTraceTlvCnt = pathTraceTlvLen / ETHTSYN_MSG_ANNOUNCE_PATH_SEQUENCE_ELEMENT_LENGTH;
      uint16 currPathTraceElemOfs = ETHTSYN_MSG_ANNOUNCE_PATH_TRACE_TLV_PATH_SEQ_START_PL_REL_OFS;
      uint16_least tlvIter = 0;
      pathTraceTlvValid = TRUE;

      /* #30 Check that own system is not included in path trace TLVs. */
      for (tlvIter = 0; tlvIter < pathTraceTlvCnt; tlvIter++)
      {
        if (EthTSyn_Mem_GetUint64(AnnouncePayloadPtr, currPathTraceElemOfs) == LocalClockId)
        {
          pathTraceTlvValid = FALSE;
          break;
        }
        currPathTraceElemOfs += ETHTSYN_MSG_ANNOUNCE_PATH_SEQUENCE_ELEMENT_LENGTH;
      }
    }
  }

  return pathTraceTlvValid;
} /* EthTSyn_AnnounceRcvSm_CheckPathTraceTlv() */
# endif /* ETHTSYN_NOUNIT_ANNOUNCERCVSM */

/**********************************************************************************************************************
 * Internal functions of SyncRcvSm
 *!
 * \unit SyncRcvSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SYNCRCVSM)
/**********************************************************************************************************************
 * EthTSyn_SyncRcvSm_IntInit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SyncRcvSm_IntInit(
  EthTSyn_SyncRcvSmIterType SyncRcvSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize dynamic data of the Sync receive state machine. */
  EthTSyn_SetAnySyncReceivedOfSyncRcvSmDyn(SyncRcvSmIdx, FALSE);
  EthTSyn_SetSyncSequenceIdOfSyncRcvSmDyn(SyncRcvSmIdx, 0u);
  EthTSyn_SetResetRequestedOfSyncRcvSmDyn(SyncRcvSmIdx, FALSE);
  EthTSyn_SetFupRxTimeoutCntOfSyncRcvSmDyn(SyncRcvSmIdx, 0u);
  EthTSyn_SetStateOfSyncRcvSmDyn(SyncRcvSmIdx, ETHTSYN_WAIT_RX_SYNC_STATEOFSYNCRCVSMDYN);
} /* EthTSyn_SyncRcvSm_IntInit() */

/**********************************************************************************************************************
 * EthTSyn_SyncRcvSm_IsSyncSequenceIdValid
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
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SyncRcvSm_IsSyncSequenceIdValid(
  EthTSyn_SyncRcvSm_SmIdxType SyncRcvSmIdx,
  uint16                      SyncSequenceId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean seqIdValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if any Sync message was received by the SyncRcvSm yet. */
  if (EthTSyn_IsAnySyncReceivedOfSyncRcvSmDyn(SyncRcvSmIdx))
  {
    const uint16 prevSeqId = EthTSyn_GetSyncSequenceIdOfSyncRcvSmDyn(SyncRcvSmIdx);

    /* #20 Check if sequence id differs from the previous one. The sequence id is valid in this case. */
    if (SyncSequenceId != prevSeqId)
    {
      seqIdValid = TRUE;
    }
  }
  /* #11 Otherwise. */
  else
  {
    /* #110 Consider sequence id as valid and set marker that a Sync message was received. */
    EthTSyn_SetAnySyncReceivedOfSyncRcvSmDyn(SyncRcvSmIdx, TRUE);
    seqIdValid = TRUE;
  }

  /* #30 Update the sequence id. */
  EthTSyn_SetSyncSequenceIdOfSyncRcvSmDyn(SyncRcvSmIdx, SyncSequenceId);

  return seqIdValid;
} /* EthTSyn_SyncRcvSm_IsSyncSequenceIdValid() */

/**********************************************************************************************************************
 * EthTSyn_SyncRcvSm_IsFupMatchingSync
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SyncRcvSm_IsFupMatchingSync(
                  EthTSyn_SyncRcvSm_SmIdxType SyncRcvSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)         PtpHdrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean fupMatchesSync = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if sequence id of the FollowUp matches the sequence id of the last received Sync message. */
  if (PtpHdrPtr->SequenceId == EthTSyn_GetSyncSequenceIdOfSyncRcvSmDyn(SyncRcvSmIdx))
  {
    /* #20 Check if FollowUp was transmitted by the same time master as the last received Sync message. */
    if ((PtpHdrPtr->SourcePortNumber == EthTSyn_GetSyncSrcPortNumOfSyncRcvSmDyn(SyncRcvSmIdx)) &&
        (PtpHdrPtr->SourceClockId == EthTSyn_GetSyncSrcClockIdOfSyncRcvSmDyn(SyncRcvSmIdx)))
    {
      fupMatchesSync = TRUE;
    }
  }

  return fupMatchesSync;
} /* EthTSyn_SyncRcvSm_IsFupMatchingSync() */
# endif /* ETHTSYN_NOUNIT_SYNCRCVSM */

/**********************************************************************************************************************
 * Internal functions of SlaveRx
 *!
 * \unit SlaveRx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SLAVERX)
/**********************************************************************************************************************
 * EthTSyn_SlaveRx_ResetSlaveRxData
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ResetSlaveRxData(
  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_SlaveRxMasterClockIdDynIdxOfSlaveRxDataType slaveRxMasterClockIdIdx =
    EthTSyn_GetSlaveRxMasterClockIdDynIdxOfSlaveRxData(SlaveRxDataIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger reset of the SyncRcvSm. */
  EthTSyn_SyncRcvSm_Reset(EthTSyn_GetSyncRcvSmIdxOfSlaveRxData(SlaveRxDataIdx));

  /* #20 Trigger rest of the AnnounceRcvSm. */
  EthTSyn_AnnounceRcvSm_Reset(EthTSyn_GetAnnounceRcvSmIdxOfSlaveRxData(SlaveRxDataIdx));

  /* #30 Check if valid master information is used (if master-slave conflict detection is disabled, otherwise it is
   *     always used). */
#  if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF)
  if (slaveRxMasterClockIdIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn())
#  endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF) */
  {
    /*@ assert slaveRxMasterClockIdIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn(); */

    /* #40 Check if no static configured master is used. */
    if (!EthTSyn_IsSlaveRxSrcPortIdCheckClockIdUsedOfSlaveRxData(SlaveRxDataIdx))
    {
      /* #50 Reset the valid master information with protection against Rx interrupts. */
      SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();
      EthTSyn_SetValidMasterSetOfSlaveRxMasterClockIdDyn(slaveRxMasterClockIdIdx, FALSE);
      EthTSyn_SetValidMasterClockIdOfSlaveRxMasterClockIdDyn(slaveRxMasterClockIdIdx, 0u);
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();
    }
  }
} /* EthTSyn_SlaveRx_ResetSlaveRxData() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_GetSyncRxTimeInfo
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
ETHTSYN_LOCAL_INLINE FUNC(EthTSyn_SyncRxTimeInfoType, ETHTSYN_CODE) EthTSyn_SlaveRx_GetSyncRxTimeInfo(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)              RxTsInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SyncRxTimeInfoType rxTimeSyncInfo = {0u};
  boolean isIngrTsValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  /* #10 Calculate Sync ingress timestamp and switch residence time (if switch management is enabled). */
  isIngrTsValid = (EthTSyn_SwtMgmtRx_GetSyncIngrTsAndResTime(&RxTsInfoPtr->TsInfo, &rxTimeSyncInfo.SyncIngrTs,
    &rxTimeSyncInfo.SwtResTime) == E_OK);
  rxTimeSyncInfo.SyncSwtIngrTsMgmt = RxTsInfoPtr->TsInfo.SwtIngrTsMgmt;
#  else
  /* #11 Use Sync ingress timestamp and set switch residence time to "0" (if switch management is disabled). */
  rxTimeSyncInfo.SyncIngrTs = RxTsInfoPtr->TsInfo.IntVltMgmt.IntVlt;
  rxTimeSyncInfo.SwtResTime = 0u;
  isIngrTsValid = RxTsInfoPtr->TsInfo.IntVltMgmt.IntVltValid;
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

  rxTimeSyncInfo.SyncRxTimeInfoValid = FALSE;

  /* #20 Check if Sync ingress timestamp is valid. */
  if (isIngrTsValid == TRUE)
  {
    /* #30 Check if HW-timestamping is used and StbM does not use the Ethernet controller as time source. */
#  if (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    const EthTSyn_ComCtrl_PortIdxType comCtrlPortIdx =
      EthTSyn_GetComCtrlPortIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx);
    if (EthTSyn_IsStbMTimeSrcEqualOfComCtrlPort(comCtrlPortIdx) == FALSE)
    {
      /* #40 Check if StbM and Ethernet times at Sync reception are valid. */
      if ((RxTsInfoPtr->SyncRxEthIntVltMgmt.IntVltValid == TRUE) &&
          (RxTsInfoPtr->SyncRxStbmIntVltMgmt.IntVltValid == TRUE))
      {
        /* #50 Check if value of Ethernet time at Sync reception is higher than Sync ingress timestamp. */
        if (RxTsInfoPtr->SyncRxEthIntVltMgmt.IntVlt >= rxTimeSyncInfo.SyncIngrTs)
        {
          /* #60 Sync reception delay is the difference of the Ethernet time at Sync reception and the Sync ingress
           *     timestamp.
           *     Sync reception virtual local time is equal to the StbM time.
           *     Set Sync reception time information as valid. */
          rxTimeSyncInfo.SyncReceptionDelay = RxTsInfoPtr->SyncRxEthIntVltMgmt.IntVlt - rxTimeSyncInfo.SyncIngrTs;
          rxTimeSyncInfo.SyncRxVlt = EthTSyn_Ts_IntVltToVlt(RxTsInfoPtr->SyncRxStbmIntVltMgmt.IntVlt);
          rxTimeSyncInfo.SyncRxTimeInfoValid = TRUE;
        }
      }
    }
    else
#  endif /* (ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) */
    /* #31 Otherwise, Sync reception virtual local time is equal to Sync ingress timestamp.
     *     Set time since Sync reception to "0".
     *     Set Sync reception time information as valid. */
    {
      rxTimeSyncInfo.SyncRxVlt = EthTSyn_Ts_IntVltToVlt(rxTimeSyncInfo.SyncIngrTs);
      rxTimeSyncInfo.SyncReceptionDelay = 0u;
      rxTimeSyncInfo.SyncRxTimeInfoValid = TRUE;
    }
  }

  ETHTSYN_DUMMY_STATEMENT(SlaveRxDataHandlerIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */

  return rxTimeSyncInfo;
} /* EthTSyn_SlaveRx_GetSyncRxTimeInfo() */

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdSyncEndStation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdSyncEndStation(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)              RxTsInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SyncRxTimeInfoType syncRxTimeInfo = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate Sync reception time information. */
  syncRxTimeInfo = EthTSyn_SlaveRx_GetSyncRxTimeInfo(SlaveRxDataHandlerIdx, RxTsInfoPtr);

  /* #20 Forward the Sync message to the SyncRcvSm. */
  (void)EthTSyn_SlaveRx_FwdRcvdSync(EthTSyn_GetSlaveRxDataExternalIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx),
    PtpHdrPtr, &syncRxTimeInfo);
} /* EthTSyn_SlaveRx_ProcRcvdSyncEndStation() */
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdSyncBridge
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdSyncBridge(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)              RxTsInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_SlaveRxDataExternalIdxOfSlaveRxDataHandlerType slaveRxDataExtIdx =
    EthTSyn_GetSlaveRxDataExternalIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx);
  Std_ReturnType extSlaveProcRcvdSyncRetVal = E_NOT_OK;
  EthTSyn_SyncRxTimeInfoType syncRxTimeInfo = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate Sync reception time information. */
  syncRxTimeInfo = EthTSyn_SlaveRx_GetSyncRxTimeInfo(SlaveRxDataHandlerIdx, RxTsInfoPtr);

  /* #20 Forward the Sync message to the external slave. */
  extSlaveProcRcvdSyncRetVal = EthTSyn_SlaveRx_FwdRcvdSync(slaveRxDataExtIdx, PtpHdrPtr, &syncRxTimeInfo);

  /* #30 Check if the Sync message was accepted by the external slave. */
  if (extSlaveProcRcvdSyncRetVal == E_OK)
  {
    const EthTSyn_SlaveRxDataInternalIdxOfSlaveRxDataHandlerType slaveRxDataIntIdx =
      EthTSyn_GetSlaveRxDataInternalIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx);

    /* #40 Forward the Sync message to the internal slave if one is present. */
#   if ( (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_TRANSPARENT) || \
         (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_ASYMMETRIC_TRANSPARENT) )
    if (slaveRxDataIntIdx < EthTSyn_GetSizeOfSlaveRxData())
#   endif /* (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_TRANSPARENT) ||
           * (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_ASYMMETRIC_TRANSPARENT) */
    {
      /*@ assert slaveRxDataIntIdx < EthTSyn_GetSizeOfSlaveRxData(); */
      (void)EthTSyn_SlaveRx_FwdRcvdSync(slaveRxDataIntIdx, PtpHdrPtr, &syncRxTimeInfo);
    }

    /* #50 Forward the Sync message to the SiteSyncSyncSm (if it is used). */
#   if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
    EthTSyn_SiteSyncSyncSm_ProcRcvdSync(EthTSyn_GetSiteSyncSyncSmIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx),
      PtpHdrPtr, &RxTsInfoPtr->TsInfo.SwtIngrTsMgmt);
#   endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

    /* #60 Forward the Sync message to the AsymSiteSyncSyncSm (if it is used). */
#   if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
    EthTSyn_AsymSiteSyncSyncSm_SyncRcvd(EthTSyn_GetAsymSiteSyncSyncSmIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx),
      &RxTsInfoPtr->TsInfo.SwtIngrTsMgmt, &RxTsInfoPtr->AddInfo);
#   endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */
  }
} /* EthTSyn_SlaveRx_ProcRcvdSyncBridge() */
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_FwdRcvdSync
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SlaveRx_FwdRcvdSync(
                  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)         PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_SyncRxTimeInfoType) SyncRxTimeInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if Sync message is accepted based on the received source port identity. */
  if (EthTSyn_SlaveRx_CheckSyncSrcClockId(SlaveRxDataIdx, PtpHdrPtr->SourceClockId) == TRUE)
  {
    /* #20 Forward the Sync message to the SyncRcvSm for further processing. */
    retVal = EthTSyn_SyncRcvSm_ProcRcvdSync(EthTSyn_GetSyncRcvSmIdxOfSlaveRxData(SlaveRxDataIdx), PtpHdrPtr,
      SyncRxTimeInfoPtr);
  }
  /* #11 Otherwise, report a DET runtime error ETHTSYN_E_TSCONFLICT (if master-slave conflict detection is enabled). */
#  if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
  else
  {
    (void)Det_ReportRuntimeError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_SLAVE_RX_FWD_RCVD_SYNC,
      ETHTSYN_E_TSCONFLICT);
  }
#  endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON) */

  return retVal;
} /* EthTSyn_SlaveRx_FwdRcvdSync() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_CheckSyncSrcClockId
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
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SlaveRx_CheckSyncSrcClockId(
  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx,
  uint64                      SyncSrcClockId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean acceptSyncMsg = TRUE; /* Accept Sync message if no check is required. */
  const EthTSyn_SlaveRxMasterClockIdDynIdxOfSlaveRxDataType masterClockIdDynIdx =
    EthTSyn_GetSlaveRxMasterClockIdDynIdxOfSlaveRxData(SlaveRxDataIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if master clock id validation is required for the time slave (if master-slave conflict detection is
   *     disabled, otherwise it is always required). */
#  if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF)
  if (masterClockIdDynIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn())
#  endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF) */
  {
    /*@ assert masterClockIdDynIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn(); */
    acceptSyncMsg = FALSE;

    /* #20 Enter critical section to protect read/write of valid master information. */
    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

    /* #30 Check if the valid master is already set. */
    if (EthTSyn_IsValidMasterSetOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx))
    {
      /* #300 Only accept the Sync message when the source clock identity is equal to the stored valid master clock
       *      identity. */
      acceptSyncMsg =
        (boolean)(SyncSrcClockId == EthTSyn_GetValidMasterClockIdOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx));
    }
    /* #31 Otherwise (if master-slave conflict detection is enabled). */
#  if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
    else
    {
      /* #310 Check if valid master shall be set by using the source clock identity of the received Sync message
              (i.e. Announce is not used). */
      if (!EthTSyn_IsAnnounceRcvSmUsedOfSlaveRxData(SlaveRxDataIdx))
      {
        /* #3100 Set the valid master clock identity and receive the Sync message. */
        EthTSyn_SetValidMasterClockIdOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx, SyncSrcClockId);
        EthTSyn_SetValidMasterSetOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx, TRUE);

        acceptSyncMsg = TRUE;
      }
    }
#  endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON) */

    /* #40 Exit critical section. */
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  }

  return acceptSyncMsg;
} /* EthTSyn_SlaveRx_CheckSyncSrcClockId() */

/**********************************************************************************************************************
 * EthTSyn_SlaveRx_CheckFupMsgLen
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_SlaveRx_CheckFupMsgLen(
  uint16 FupMsgLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean msgLenValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_ON)
  /* #10 Check if the message length is equal to the define FollowUp length (if message compliance is enabled). */
  msgLenValid = (boolean)(FupMsgLen == ETHTSYN_MSG_FUP_LEN);
#  else
  /* #20 Check if the message length is greater or equal to the defined FollowUp length
   *     (if message compliance is disabled). */
  msgLenValid = (boolean)(FupMsgLen >= ETHTSYN_MSG_FUP_LEN);
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_ON) */

  return msgLenValid;
} /* EthTSyn_SlaveRx_CheckFupMsgLen() */

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF)
/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdFupEndStation
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdFupEndStation(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                             FupPayloadPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward the received FollowUp message to the SyncRcvSm. */
  (void)EthTSyn_SyncRcvSm_ProcRcvdFup(EthTSyn_GetSyncRcvSmIdxOfSlaveRxData(SlaveRxDataHandlerIdx), PtpHdrPtr,
    FupPayloadPtr);
} /* EthTSyn_SlaveRx_ProcRcvdFupEndStation() */
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_OFF) */

#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdFupBridge
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdFupBridge(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                             FupPayloadPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_SlaveRxDataExternalIdxOfSlaveRxDataHandlerType slaveRxDataExtIdx =
    EthTSyn_GetSlaveRxDataExternalIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx);
  Std_ReturnType extSlaveProcRcvdFupRetVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward the FollowUp message to the SyncRcvSm of the external slave. */
  extSlaveProcRcvdFupRetVal = EthTSyn_SyncRcvSm_ProcRcvdFup(EthTSyn_GetSyncRcvSmIdxOfSlaveRxData(slaveRxDataExtIdx),
    PtpHdrPtr, FupPayloadPtr);

  /* #20 Check if the FollowUp message was accepted by the external slave. */
  if (extSlaveProcRcvdFupRetVal == E_OK)
  {
    const EthTSyn_SlaveRxDataInternalIdxOfSlaveRxDataHandlerType slaveRxDataIntIdx =
      EthTSyn_GetSlaveRxDataInternalIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx);

    /* #30 Forward the FollowUp message to the SyncRcvSm of the internal slave if one is present. */
#   if ( (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_TRANSPARENT) || \
         (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_ASYMMETRIC_TRANSPARENT) )
    if (slaveRxDataIntIdx < EthTSyn_GetSizeOfSlaveRxData())
#   endif /* (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_TRANSPARENT) ||
           * (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_ASYMMETRIC_TRANSPARENT) */
    {
      /*@ assert slaveRxDataIntIdx < EthTSyn_GetSizeOfSlaveRxData(); */
      (void)EthTSyn_SyncRcvSm_ProcRcvdFup(EthTSyn_GetSyncRcvSmIdxOfSlaveRxData(slaveRxDataIntIdx),
        PtpHdrPtr, FupPayloadPtr);
    }

    /* #40 Forward the FollowUp message to the SiteSyncSyncSm (if it is used). */
#   if (ETHTSYN_SITESYNCSYNCSM == STD_ON)
    EthTSyn_SiteSyncSyncSm_ProcRcvdFup(EthTSyn_GetSiteSyncSyncSmIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx),
      PtpHdrPtr, FupPayloadPtr);
#   endif /* (ETHTSYN_SITESYNCSYNCSM == STD_ON) */

    /* #50 Forward the FollowUp message to the AsymSiteSyncSyncSm (if it is used). */
#   if (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON)
    EthTSyn_AsymSiteSyncSyncSm_ProcRcvdFup(
      EthTSyn_GetAsymSiteSyncSyncSmIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx), PtpHdrPtr, FupPayloadPtr);
#   endif /* (ETHTSYN_ASYMSITESYNCSYNCSM == STD_ON) */
  }
} /* EthTSyn_SlaveRx_ProcRcvdFupBridge() */
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
# endif /* ETHTSYN_NOUNIT_SLAVERX */

/**********************************************************************************************************************
 * Internal functions of FupRxHandler
 *!
 * \unit FupRxHandler
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_FUPRXHANDLER)
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_ResetTlvInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_ResetTlvInfo(
  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const StbM_UserDataType resetUserDataVal = {0u};
  const StbM_TimeStampType resetTimestamp = {0u};
  EthTSyn_FupRxOfsDomainHandlerDynIterType ofsDomainHandlerIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Reset status. */
  EthTSyn_SetTbStatusOfFupRxHandlerDyn(FupRxHandlerIdx, 0u);

  /* #20 Reset user data. */
  EthTSyn_SetUserDataOfFupRxHandlerDyn(FupRxHandlerIdx, resetUserDataVal);

  /* #30 Reset offsets. */
  for (ofsDomainHandlerIter = EthTSyn_GetFupRxOfsDomainHandlerStartIdxOfFupRxHandler(FupRxHandlerIdx);
       ofsDomainHandlerIter < EthTSyn_GetFupRxOfsDomainHandlerEndIdxOfFupRxHandler(FupRxHandlerIdx);
       ofsDomainHandlerIter++)
  {
    /*@ assert ofsDomainHandlerIter < EthTSyn_GetSizeOfFupRxOfsDomainHandler(); */                                     /* VCA_ETHTSYN_0_N_IND_CSL03 */
    EthTSyn_SetTimestampOfFupRxOfsDomainHandlerDyn(ofsDomainHandlerIter, resetTimestamp);
    EthTSyn_SetUserDataOfFupRxOfsDomainHandlerDyn(ofsDomainHandlerIter, resetUserDataVal);
    EthTSyn_SetUpdatedOfFupRxOfsDomainHandlerDyn(ofsDomainHandlerIter, FALSE);
  }
} /* EthTSyn_FupRxHandler_ResetTlvInfo() */

/**********************************************************************************************************************
 *  EthTSyn_FupRxHandler_CheckFupMsgStructureAndCrc
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
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_CheckFupMsgStructureAndCrc(
                  EthTSyn_FupRxHandler_IdxType           FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                    PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                                 FupPayloadPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean fupMsgStructureValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the message length indicates at least on AUTOSAR Sub-TLV. */
  if (PtpHdrPtr->MessageLength >= (ETHTSYN_MSG_FUP_LEN + ETHTSYN_AR_TLV_HEADER_LENGTH))
  {
    uint16 totalSubTlvLength = 0u;

    /* #20 Check validity of the AUTOSAR TLV header. */
    if (EthTSyn_FupRxHandler_CheckArTlvHeader(FupPayloadPtr, PtpHdrPtr->MessageLength, &totalSubTlvLength) == TRUE)
    {
      /* #30 Search for required AUTOSAR Sub-TLVs, verify their validity and extract the required information. */
      fupMsgStructureValid = EthTSyn_FupRxHandler_CheckArSubTlvs(FupRxHandlerIdx, PtpHdrPtr, FupPayloadPtr,
        totalSubTlvLength);
    }
  }
  /* #11 Otherwise, consider the message structure as valid when no AUTOSAR Sub-TLV is expected and the message length
   *     does not indicate an incomplete AR-TLV or any other TLV(s). */
  else
  {
    if ((EthTSyn_GetRxSubTlvFlagsOfFupRxHandler(FupRxHandlerIdx) == 0u) &&
        (PtpHdrPtr->MessageLength == ETHTSYN_MSG_FUP_LEN))
    {
      fupMsgStructureValid = TRUE;
    }
  }

  return fupMsgStructureValid;
} /* EthTSyn_FupRxHandler_CheckFupMsgStructureAndCrc() */

/**********************************************************************************************************************
 *  EthTSyn_FupRxHandler_CheckArTlvHeader
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_CheckArTlvHeader(
  ETHTSYN_P2CONST(uint8)  FupPayloadPtr,
                  uint16  FupMsgLength,
    ETHTSYN_P2VAR(uint16) TotalSubTlvLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean arTlvHeaderValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize out-parameter. */
  (*TotalSubTlvLength) = 0u;

  /* #10 Check if AR-TLV Header is present. */
  if (EthTSyn_Mem_GetUint16(FupPayloadPtr, ETHTSYN_MSG_FUP_AR_TLV_HDR_TYPE_REL_OFS) == ETHTSYN_AR_TLV_TYPE)
  {
    /* #20 Perform general checks of the AR-TLV Header fields. */
    const uint16 arTlvLengthField = EthTSyn_Mem_GetUint16(FupPayloadPtr, ETHTSYN_MSG_FUP_AR_TLV_HDR_LEN_REL_OFS);
    const uint16 maxAllowedArTlvLength = (uint16)
      (FupMsgLength - ETHTSYN_MSG_FUP_LEN - ETHTSYN_AR_TLV_HEADER_TYPE_LENGTH_FIELDS_LENGTH);

    if ((arTlvLengthField > (ETHTSYN_AR_TLV_HEADER_LENGTH_FIELD_VALUE + ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH)) &&
        (arTlvLengthField <= maxAllowedArTlvLength))
    {
      const uint32 arTlvOrgaId = EthTSyn_Mem_GetUint24(FupPayloadPtr, ETHTSYN_MSG_FUP_AR_TLV_HDR_ORGA_ID_REL_OFS);
      const uint32 arTlvOrgaSubType =
        EthTSyn_Mem_GetUint24(FupPayloadPtr, ETHTSYN_MSG_FUP_AR_TLV_HDR_ORGA_SUB_TYPE_REL_OFS);

      if ((arTlvOrgaId == ETHTSYN_AR_TLV_ORGANIZATION_ID) &&
          (arTlvOrgaSubType == ETHTSYN_AR_TLV_ORGANIZATION_SUB_TYPE))
      {
        /* #30 AR-TLV Header valid. Extract total length of Sub-Tlvs. */
        (*TotalSubTlvLength) = (uint16)(arTlvLengthField - ETHTSYN_AR_TLV_HEADER_LENGTH_FIELD_VALUE);
        arTlvHeaderValid = TRUE;
      }
    }
  }

  return arTlvHeaderValid;
} /* EthTSyn_FupRxHandler_CheckArTlvHeader() */

/**********************************************************************************************************************
 *  EthTSyn_FupRxHandler_CheckArSubTlvs
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
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_CheckArSubTlvs(
                  EthTSyn_FupRxHandler_IdxType           FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                    PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                                 FupPayloadPtr,
                  uint16                                 TotalSubTlvLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean arSubTlvsValid = TRUE;
  uint16 currSubTlvOfs = ETHTSYN_MSG_FUP_FIRST_AR_SUB_TLV_PL_REL_OFS;
  sint32 remainingLength = (sint32)TotalSubTlvLength;
  EthTSyn_FupRxHandler_ArSubTlvCounterType arSubTlvCounter = {0u};
  uint8 pendingArSubTlvs = EthTSyn_GetRxSubTlvFlagsOfFupRxHandler(FupRxHandlerIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Unpack and check every ArSubTlv. */
  while ((arSubTlvsValid == TRUE) && (remainingLength > (sint32)ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH))
  {
    /* #20 Get type and total length of the current TLV. */
    const uint8 currTlvType = EthTSyn_Mem_GetUint8(FupPayloadPtr, currSubTlvOfs);
    const uint8 currTlvLenFieldVal = EthTSyn_Mem_GetUint8(
      FupPayloadPtr, (currSubTlvOfs + ETHTSYN_MSG_FUP_AR_SUB_TLV_LEN_OFS));
    const uint16 currTlvLength = (uint16)ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH + (uint16)currTlvLenFieldVal;
    uint8 arSubTlvFlag = 0u;

    /* #30 Check if TLV length is valid according to its type and if the message is big enough to hold the TLV. */
    remainingLength -= (sint32)currTlvLength;

    if ((EthTSyn_FupRxHandler_CheckArSubTlvLengthAndGetFlag(currTlvType, currTlvLenFieldVal, &arSubTlvFlag) == TRUE) &&
        (remainingLength >= 0))
    {
      /* #40 Check if Sub-TLV is expected. */
      if ((EthTSyn_GetRxSubTlvFlagsOfFupRxHandler(FupRxHandlerIdx) & arSubTlvFlag) != 0u)
      {
        /* #50 Extract information from TLV if possible and check if the TLV is valid. */
        arSubTlvsValid = EthTSyn_FupRxHandler_GetArSubTlvInfoIfValid(FupRxHandlerIdx, PtpHdrPtr, FupPayloadPtr,
          currSubTlvOfs, &arSubTlvCounter);

        pendingArSubTlvs &= ~arSubTlvFlag;
      }

      /* Set offset to potential next Sub-TLV. */
      currSubTlvOfs += currTlvLength;
    }
    /* #31 Otherwise, the length of the TLV is invalid or too big. Consider the message as invalid. */
    else
    {
      arSubTlvsValid = FALSE;
    }
  }

  /* #60 Mark message as invalid if any expected Sub-TLV is missing or the length in the AUTOSAR TLV header does
   *     not match the accumulated length of the Sub-TLVs. */
  if ((pendingArSubTlvs != 0u) || (remainingLength != 0))
  {
    arSubTlvsValid = FALSE;
  }

  return arSubTlvsValid;
} /* EthTSyn_FupRxHandler_CheckArSubTlvs() */

/**********************************************************************************************************************
 *  EthTSyn_FupRxHandler_CheckArSubTlvLengthAndGetFlag
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_CheckArSubTlvLengthAndGetFlag(
                uint8  ArSubTlvType,
                uint8  ArSubTlvLength,
  ETHTSYN_P2VAR(uint8) ArSubTlvFlag)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isLengthValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  (*ArSubTlvFlag) = 0u;

  /* #10 Check if the TLV type and length match.
   *     Set the Sub-TLV flag in case of known Sub-TLV. */
  switch (ArSubTlvType)
  {
  case ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED:
    {
      isLengthValid = (boolean)(ArSubTlvLength == ETHTSYN_AR_SUB_TLV_TIME_LENGTH_FIELD_VALUE);
      (*ArSubTlvFlag) = ETHTSYN_SUB_TLV_TIME_FLAG;
      break;
    }

  case ETHTSYN_AR_SUB_TLV_TYPE_OFS_NOT_SECURED:
  case ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED:
    {
      isLengthValid = (boolean)(ArSubTlvLength == ETHTSYN_AR_SUB_TLV_OFS_LENGTH_FIELD_VALUE);
      (*ArSubTlvFlag) = ETHTSYN_SUB_TLV_OFS_FLAG;
      break;
    }

  case ETHTSYN_AR_SUB_TLV_TYPE_STATUS_NOT_SECURED:
  case ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED:
    {
      isLengthValid = (boolean)(ArSubTlvLength == ETHTSYN_AR_SUB_TLV_STATUS_LENGTH_FIELD_VALUE);
      (*ArSubTlvFlag) = ETHTSYN_SUB_TLV_STATUS_FLAG;
      break;
    }

  case ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_NOT_SECURED:
  case ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED:
    {
      isLengthValid = (boolean)(ArSubTlvLength == ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH_FIELD_VALUE);
      (*ArSubTlvFlag) = ETHTSYN_SUB_TLV_USER_DATA_FLAG;
      break;
    }

  /* Unknown TLV-Type. Consider the length as valid. */
  default:
    {
      isLengthValid = TRUE;
      break;
    }
  }

  return isLengthValid;
} /* EthTSyn_FupRxHandler_CheckArSubTlvLengthAndGetFlag() */

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetArSubTlvInfoIfValid
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
 */
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetArSubTlvInfoIfValid(
                  EthTSyn_FupRxHandler_IdxType              FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                       PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                                    FupPayloadPtr,
                  uint16                                    SubTlvOfs,
    ETHTSYN_P2VAR(EthTSyn_FupRxHandler_ArSubTlvCounterType) ArSubTlvCounterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isTlvValid = FALSE;
  EthTSyn_ValidateSubTlvInfoType subTlvInfo = {0};
  EthTSyn_CrcCfgInfoType crcCfgInfo = {0};

  /* ----- Implementation ----------------------------------------------- */
  subTlvInfo.PtpHdrPtr = PtpHdrPtr;
  subTlvInfo.FupPayloadPtr = FupPayloadPtr;
  subTlvInfo.SubTlvOfs = SubTlvOfs;
  crcCfgInfo.RxCrcValidated = EthTSyn_GetRxCrcValidatedOfFupRxHandler(FupRxHandlerIdx);
  crcCfgInfo.TimeSecuredFlags = EthTSyn_GetCrcFlagsRxValidatedOfFupRxHandler(FupRxHandlerIdx);

  /* #10 Check CRC state of the ArSubTLV. */
  isTlvValid = EthTSyn_CrcHndl_CheckSubTlvCrcState(EthTSyn_GetCrcHndlIdxOfFupRxHandler(FupRxHandlerIdx),
    &subTlvInfo, &crcCfgInfo);

  if (isTlvValid == TRUE)
  {
    ETHTSYN_P2CONST(uint8) arSubTlvStartPtr = &FupPayloadPtr[SubTlvOfs];

    /* #20 Get the TLV type. */
    switch (EthTSyn_Mem_GetUint8(FupPayloadPtr, SubTlvOfs))
    {
      /* #30 Check if TLV is TimeSecuredTLV. */
    case ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED:
      {
        /* #300 Increment TimeSecuredTlv count and check if max allowed TimeSecuredTlv count is exceeded. */
        ArSubTlvCounterPtr->TimeSecuredTlvCnt++;
        if (ArSubTlvCounterPtr->TimeSecuredTlvCnt > ETHTSYN_FUPRXHANDLER_MAX_TIME_SECURED_SUB_TLV_CNT)
        {
          /* #3000 Consider TLV as invalid. */
          isTlvValid = FALSE;
        }
        break;
      }

      /* #31 Otherwise, check if TLV is OfsSubTLV. */
    case ETHTSYN_AR_SUB_TLV_TYPE_OFS_NOT_SECURED:
    case ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED:
      {
        /* #310 Increment OfsTlv count and check if max allowed OfTlv count is exceeded. */
        ArSubTlvCounterPtr->OfsTlvCnt++;
        if (ArSubTlvCounterPtr->OfsTlvCnt > ETHTSYN_FUPRXHANDLER_MAX_OFS_SUB_TLV_CNT)
        {
          /* #3100 Consider TLV as invalid. */
          isTlvValid = FALSE;
        }
        /* #311 Otherwise, extract the offset if required. */
        else
        {
          isTlvValid= EthTSyn_FupRxHandler_GetOfsFromTlvIfValid(FupRxHandlerIdx, arSubTlvStartPtr);
        }
        break;
      }

      /* #32 Otherwise, check if TLV is StatusSubTLV. */
    case ETHTSYN_AR_SUB_TLV_TYPE_STATUS_NOT_SECURED:
    case ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED:
      {
        /* #320 Increment StatusTlv count and check if max allowed StatusTlv count is exceeded. */
        ArSubTlvCounterPtr->StatusTlvCnt++;
        if (ArSubTlvCounterPtr->StatusTlvCnt > ETHTSYN_FUPRXHANDLER_MAX_STATUS_SUB_TLV_CNT)
        {
          /* #3200 Consider TLV as invalid. */
          isTlvValid = FALSE;
        }
        /* #321 Otherwise, extract the time base status. */
        else
        {
          EthTSyn_FupRxHandler_GetStatusFromTlv(FupRxHandlerIdx, arSubTlvStartPtr);
        }
        break;
      }

      /* #33 Otherwise, check if TLV UserDataTLV. */
    case ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_NOT_SECURED:
    case ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED:
      {
        /* #330 Increment UserDataTlv count and check if max allowed UserDataTlv count is exceeded. */
        ArSubTlvCounterPtr->UserDataTlvCnt++;
        if (ArSubTlvCounterPtr->UserDataTlvCnt > ETHTSYN_FUPRXHANDLER_MAX_USER_DATA_SUB_TLV_CNT)
        {
          /* #3300 Consider TLV as invalid. */
          isTlvValid = FALSE;
        }
        /* #331 Otherwise, extract the user data. */
        else
        {
          EthTSyn_FupRxHandler_GetUserDataFromTlv(FupRxHandlerIdx, arSubTlvStartPtr);
        }
        break;
      }

      /* #34 Otherwise, do nothing. */
    default: /* COV_ETHTSYN_EMPTY_BRANCH */
      {
        /* This should never happen because the only caller (EthTSyn_FupRxHandler_CheckArSubTlvs()) checks if the
         * TLV is expected by the Slave. Only known TLV which are handled in dedicated cases can be expected.
         * Hence, this function should only be called for known TLVs handled above. */
        break;
      }
    }
  }

  return isTlvValid;
} /* EthTSyn_FupRxHandler_GetArSubTlvInfoIfValid() */

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetOfsFromTlvIfValid
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
ETHTSYN_LOCAL_INLINE FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetOfsFromTlvIfValid(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(uint8)                       ArSubTlvStartPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isTlvValid = TRUE; /* Initialize with TRUE because there is only one special case where the TLV is invalid. */
  const uint8 ofsDomainNum = EthTSyn_Mem_GetUint8(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_DOMAIN_NUM_OFS);
  EthTSyn_FupRxOfsDomainHandlerIterType ofsDomainHandlerIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the offset domain is relevant for the slave. */
  for (ofsDomainHandlerIter = EthTSyn_GetFupRxOfsDomainHandlerStartIdxOfFupRxHandler(FupRxHandlerIdx);
       ofsDomainHandlerIter < EthTSyn_GetFupRxOfsDomainHandlerEndIdxOfFupRxHandler(FupRxHandlerIdx);
       ofsDomainHandlerIter++)
  {
    /*@ assert ofsDomainHandlerIter < EthTSyn_GetSizeOfFupRxOfsDomainHandler(); */                                     /* VCA_ETHTSYN_0_N_IND_CSL03 */
    if (EthTSyn_GetDomainNumOfFupRxOfsDomainHandler(ofsDomainHandlerIter) == ofsDomainNum)
    {
      /* #20 Check for duplicated Ofs SubTLV. */
      if (EthTSyn_IsUpdatedOfFupRxOfsDomainHandlerDyn(ofsDomainHandlerIter))
      {
        /* #200 Consider TLV as invalid.*/
        /* In case of a duplicated Ofs SubTLV for a used offset domain, it is unclear which information shall be used.
         * Therefore, the FollowUp message is considered as invalid in this case. */
        isTlvValid = FALSE;
      }
      /* #21 Otherwise. */
      else
      {
        /* #210 Extract the offset information and store it for later processing. */
        const uint8 ofsSubTlvStatus =
          EthTSyn_Mem_GetUint8(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_STATUS_OFS);
        StbM_TimeStampType timeStamp = {0};
        StbM_UserDataType userData = {0};

        timeStamp.timeBaseStatus = (StbM_TimeBaseStatusType)(
          (ofsSubTlvStatus << ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_SHIFT) & ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_MASK);
        timeStamp.secondsHi =
          EthTSyn_Mem_GetUint16(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_TIME_SEC_HI_OFS);
        timeStamp.seconds =
          EthTSyn_Mem_GetUint32(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_TIME_SEC_LOW_OFS);
        timeStamp.nanoseconds =
          EthTSyn_Mem_GetUint32(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_TIME_NSEC_OFS);

        userData.userDataLength =
          EthTSyn_Mem_GetUint8(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_LEN_OFS);
        userData.userByte0 =
          EthTSyn_Mem_GetUint8(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_BYTE_0_OFS);
        userData.userByte1 =
          EthTSyn_Mem_GetUint8(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_BYTE_1_OFS);
        userData.userByte2 =
          EthTSyn_Mem_GetUint8(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_BYTE_2_OFS);

        EthTSyn_SetTimestampOfFupRxOfsDomainHandlerDyn(ofsDomainHandlerIter, timeStamp);
        EthTSyn_SetUserDataOfFupRxOfsDomainHandlerDyn(ofsDomainHandlerIter, userData);
        EthTSyn_SetUpdatedOfFupRxOfsDomainHandlerDyn(ofsDomainHandlerIter, TRUE);
      }

      /* Stop iteration. */
      break;
    }
  }

  return isTlvValid;
} /* EthTSyn_FupRxHandler_GetOfsFromTlvIfValid() */

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetStatusFromTlv
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetStatusFromTlv(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(uint8)                       ArSubTlvStartPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint8 arSubTlvStatus = EthTSyn_Mem_GetUint8(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_STATUS_STATUS_OFS);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the status from the status SubTLV, transform it into the StbM time base status type and store it for
  *      later usage. */
  EthTSyn_SetTbStatusOfFupRxHandlerDyn(FupRxHandlerIdx, (StbM_TimeBaseStatusType)(
    (arSubTlvStatus << ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_SHIFT) & ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_MASK));
} /* EthTSyn_FupRxHandler_GetStatusFromTlv() */

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetUserDataFromTlv
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetUserDataFromTlv(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(uint8)                       ArSubTlvStartPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  StbM_UserDataType userData = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read the user data from the user data SubTLV. */
  userData.userDataLength = EthTSyn_Mem_GetUint8(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_LEN_OFS);
  userData.userByte0 = EthTSyn_Mem_GetUint8(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_BYTE_0_OFS);
  userData.userByte1 = EthTSyn_Mem_GetUint8(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_BYTE_1_OFS);
  userData.userByte2 = EthTSyn_Mem_GetUint8(ArSubTlvStartPtr, ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_BYTE_2_OFS);

  /* #20 Store user data for later processing. */
  EthTSyn_SetUserDataOfFupRxHandlerDyn(FupRxHandlerIdx, userData);
} /* EthTSyn_FupRxHandler_GetUserDataFromTlv() */

#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_ForwardDataToStbM
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
/* PRQA S 6050 1 */ /* MD_EthTSyn_6050_TrivialFunctions */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_ForwardDataToStbM(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_SyncRxTimeInfoType)  SyncTimeInfoPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)          PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                       FupPayloadPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_ComCtrlPortExternalIdxOfFupRxHandlerType comCtrlPortIdx =
    EthTSyn_GetComCtrlPortExternalIdxOfFupRxHandler(FupRxHandlerIdx);
  EthTSyn_UTimediffType potCorrTime = 0;
  EthTSyn_PdelayType pDelay = EthTSyn_ComCtrl_GetPdelay(comCtrlPortIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to get the total POT correction time. */
  if (EthTSyn_FupRxHandler_GetTotalPotCorrTime(FupRxHandlerIdx, SyncTimeInfoPtr->SyncReceptionDelay,
    SyncTimeInfoPtr->SwtResTime, PtpHdrPtr->CorrectionFieldNs, &potCorrTime) == E_OK)
  {
    EthTSyn_GlobalTimestampType pot = {0};

    /* #20 Get the precise origin timestamp from the FollowUp message. */
    pot.secondsHi = EthTSyn_Mem_GetUint16(FupPayloadPtr, ETHTSYN_MSG_FUP_POT_SEC_HI_PL_REL_OFS);
    pot.seconds = EthTSyn_Mem_GetUint32(FupPayloadPtr, ETHTSYN_MSG_FUP_POT_SEC_LOW_PL_REL_OFS);
    pot.nanoseconds = EthTSyn_Mem_GetUint32(FupPayloadPtr, ETHTSYN_MSG_FUP_POT_NSEC_PL_REL_OFS);

    /* #30 Try to compute the new global time. */
    if (EthTSyn_Ts_TsPlusUTimediff(&pot, potCorrTime) == E_OK)
    {
      const StbM_SynchronizedTimeBaseType timeBaseId = EthTSyn_ComCtrl_GetSynchronizedTimeBaseId(comCtrlPortIdx);
      StbM_TimeBaseStatusType tbStatus = 0u;
      StbM_UserDataType userData = {0u};
      ETHTSYN_P2VAR(StbM_UserDataType) userDataPtr = &userData;
      StbM_TimeStampType newGlobalTime = {0};
      StbM_MeasurementType measureData = {0};

      /* #40 Get the user data and status. */
      EthTSyn_FupRxHandler_GetStatusAndUserData(FupRxHandlerIdx, &tbStatus, &userDataPtr);

      /* #50 Try to provide the new global time to the StbM. */
      newGlobalTime.timeBaseStatus = tbStatus;
      newGlobalTime.secondsHi      = pot.secondsHi;
      newGlobalTime.seconds        = pot.seconds;
      newGlobalTime.nanoseconds    = pot.nanoseconds;

      measureData.pathDelay = pDelay;

      if (StbM_BusSetGlobalTime(timeBaseId, &newGlobalTime, userDataPtr, &measureData,
            &SyncTimeInfoPtr->SyncRxVlt) == E_OK)
      {
        /* #60 Provide time validation data to StbM (if time validation is enabled). */
#  if (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON)
        if (EthTSyn_IsTimeValidationEnabledOfFupRxHandler(FupRxHandlerIdx))
        {
          EthTSyn_FupRxHandler_ProvideTimeValidationDataToStbM(timeBaseId, SyncTimeInfoPtr->SyncIngrTs, pDelay,
            PtpHdrPtr, FupPayloadPtr);
        }
#  endif /* (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON) */

        /* #70 Provide new offsets for offset time bases (if message compliance is disabled). */
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
        EthTSyn_FupRxHandler_ProvideOffsetsToStbM(FupRxHandlerIdx, SyncTimeInfoPtr->SyncIngrTs);
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */
      }
    }
  }
} /* EthTSyn_FupRxHandler_ForwardDataToStbM() */

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetStatusAndUserData
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
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetStatusAndUserData(
                              EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
                ETHTSYN_P2VAR(StbM_TimeBaseStatusType)     TbStatusPtr,
  ETHTSYN_P2VAR(ETHTSYN_P2VAR(StbM_UserDataType))          UserDataPtrPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If message compliance is enabled. */
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_ON)
  {
    /* #100 Set time base status to default value of '0'. */
    /* When message compliance is disabled, the status shall be set to '0' according to AUTOSAR. */
    (*TbStatusPtr) = 0u;

    /* #101 Set UserDataPtr to NULL_PTR. */
    /* When no user data shall be provided to the StbM via StbM_BusSetGlobalTime(), NULL_PTR might be used for the
     * userDataPtr. */
    (*UserDataPtrPtr) = NULL_PTR;

    ETHTSYN_DUMMY_STATEMENT(FupRxHandlerIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
  }
#  else
  /* #20 Otherwise. */
  {
    /* #30 Set the time base status to the stored value. */
    /* When no Status ArSubTLVs was received, the stored value is '0' which shall be used in this case according to
     * AUTOSAR. */
    (*TbStatusPtr) = EthTSyn_GetTbStatusOfFupRxHandlerDyn(FupRxHandlerIdx);

    /* #40 Check if user data where received with the FollowUp message. */
    if (EthTSyn_GetUserDataOfFupRxHandlerDyn(FupRxHandlerIdx).userDataLength > 0u)
    {
      /* #400 Provide the received user data. */
      (*(*UserDataPtrPtr)) = EthTSyn_GetUserDataOfFupRxHandlerDyn(FupRxHandlerIdx);
    }
    /* #41 Otherwise, set UserDataPtr to NULL_PTR. */
    else
    {
      /* When no user data shall be provided to the StbM via StbM_BusSetGlobalTime(), NULL_PTR might be used for the
       * userDataPtr. */
      (*UserDataPtrPtr) = NULL_PTR;
    }
  }
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_ON) */
} /* EthTSyn_FupRxHandler_GetStatusAndUserData() */

/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_GetTotalPotCorrTime
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_FupRxHandler_GetTotalPotCorrTime(
                EthTSyn_FupRxHandler_IdxType  FupRxHandlerIdx,
                EthTSyn_UTimediffType         TimeSinceSyncRx,
                EthTSyn_UTimediffType         SwtResTime,
                uint64                        CorrField,
  ETHTSYN_P2VAR(EthTSyn_UTimediffType)        PotCorrTimePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_ComCtrlPortExternalIdxOfFupRxHandlerType comCtrlPortIdx =
    EthTSyn_GetComCtrlPortExternalIdxOfFupRxHandler(FupRxHandlerIdx);
  EthTSyn_UTimediffType timediffsArray[ETHTSYN_FUPRXHANDLER_POT_CORR_TIME_SUMMANDS] = {0};

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate the total POT correction time as sum of all relevant time differences. */
  timediffsArray[0u] = TimeSinceSyncRx;
  timediffsArray[1u] = SwtResTime;
  timediffsArray[2u] = (EthTSyn_UTimediffType)(CorrField & ETHTSYN_CORR_FIELD_MAX_VALUE_NS);
  timediffsArray[3u] = (EthTSyn_UTimediffType)EthTSyn_ComCtrl_GetPdelay(comCtrlPortIdx);

  return EthTSyn_Ts_SumUTimediffsChecked(timediffsArray, ETHTSYN_FUPRXHANDLER_POT_CORR_TIME_SUMMANDS, PotCorrTimePtr);
} /* EthTSyn_FupRxHandler_GetTotalPotCorrTime() */

#  if (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_ProvideTimeValidationDataToStbM
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_ProvideTimeValidationDataToStbM(
                  StbM_SynchronizedTimeBaseType TimeBaseId,
                  EthTSyn_IntVltType            SyncIngrTs,
                  EthTSyn_PdelayType            Pdelay,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)           PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                        FupPayloadPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  StbM_EthTimeSlaveMeasurementType slaveData = {0};
  const uint64 unsignedCorrField = (PtpHdrPtr->CorrectionFieldNs & ETHTSYN_CORR_FIELD_MAX_VALUE_NS);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get all slave data required for time validation and report them to the StbM. */
  slaveData.sequenceId = PtpHdrPtr->SequenceId;
  slaveData.sourcePortId.portNumber = PtpHdrPtr->SourcePortNumber;
  slaveData.sourcePortId.clockIdentity = PtpHdrPtr->SourceClockId;
  slaveData.syncIngressTimestamp = EthTSyn_Ts_IntVltToVlt(SyncIngrTs);
  slaveData.preciseOriginTimestamp.seconds =
    EthTSyn_Mem_GetUint32(FupPayloadPtr, ETHTSYN_MSG_FUP_POT_SEC_LOW_PL_REL_OFS);
  slaveData.preciseOriginTimestamp.nanoseconds =
    EthTSyn_Mem_GetUint32(FupPayloadPtr, ETHTSYN_MSG_FUP_POT_NSEC_PL_REL_OFS);
  slaveData.correctionField = (sint64)unsignedCorrField;
  slaveData.pDelay = Pdelay;

  slaveData.referenceGlobalTimestamp.seconds = 0u;
  slaveData.referenceGlobalTimestamp.nanoseconds = 0u;
  slaveData.referenceLocalTimestamp.nanosecondsHi = 0u;
  slaveData.referenceLocalTimestamp.nanosecondsLo = 0u;

  (void)StbM_EthSetSlaveTimingData(TimeBaseId, &slaveData);
} /* EthTSyn_FupRxHandler_ProvideTimeValidationDataToStbM() */
#  endif /* (ETHTSYN_SLAVE_PD_REQ_TIME_VALIDATION_SUPPORT == STD_ON) */

#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
/**********************************************************************************************************************
 * EthTSyn_FupRxHandler_ProvideOffsetsToStbM
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
ETHTSYN_LOCAL_INLINE FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_ProvideOffsetsToStbM(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
                  EthTSyn_IntVltType           SyncIngrTs)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_FupRxOfsDomainHandlerIterType ofsDomainHandlerIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all offset domain handlers. */
  for (ofsDomainHandlerIter = EthTSyn_GetFupRxOfsDomainHandlerStartIdxOfFupRxHandler(FupRxHandlerIdx);
       ofsDomainHandlerIter < EthTSyn_GetFupRxOfsDomainHandlerEndIdxOfFupRxHandler(FupRxHandlerIdx);
       ofsDomainHandlerIter++)
  {
    /* #20 Check if new offset data are available. */
    if (EthTSyn_IsUpdatedOfFupRxOfsDomainHandlerDyn(ofsDomainHandlerIter))
    {
      const StbM_SynchronizedTimeBaseType synchronizedTimeBaseId =
        EthTSyn_GetOfsTimeBaseIdOfFupRxOfsDomainHandler(ofsDomainHandlerIter);
      StbM_UserDataType userData = {0};
      ETHTSYN_P2CONST(StbM_UserDataType) userDataPtr = NULL_PTR;
      StbM_TimeStampType timeStamp = {0};
      StbM_VirtualLocalTimeType syncIngressVlt = {0};
      StbM_MeasurementType measureData = {0};

      userData = EthTSyn_GetUserDataOfFupRxOfsDomainHandlerDyn(ofsDomainHandlerIter);
      timeStamp = EthTSyn_GetTimestampOfFupRxOfsDomainHandlerDyn(ofsDomainHandlerIter);
      syncIngressVlt = EthTSyn_Ts_IntVltToVlt(SyncIngrTs);

      if (userData.userDataLength > 0u)
      {
        userDataPtr = &userData;
      }

      /* #30 Provide the new offset data to the StbM. */
      /* Pdelay not required for Offset TimeDomains. Therefore it can be set to '0' */
      measureData.pathDelay = 0u;

      (void)StbM_BusSetGlobalTime(synchronizedTimeBaseId, &timeStamp, userDataPtr, &measureData, &syncIngressVlt);
    }
  }
} /* EthTSyn_FupRxHandler_ProvideOffsetsToStbM() */
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */
# endif /* ETHTSYN_NOUNIT_FUPRXHANDLER */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 * Public interfaces of AnnounceRcvSm
 *!
 * \unit AnnounceRcvSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_ANNOUNCERCVSM)
/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_AnnounceRcvSmIterType announceSmIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize variable data of all Announce receive state machines. */
  for (announceSmIter = 0; announceSmIter < EthTSyn_GetSizeOfAnnounceRcvSm(); announceSmIter++)
  {
    EthTSyn_AnnounceRcvSm_IntInit(announceSmIter);
  }
} /* EthTSyn_AnnounceRcvSm_Init() */

/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_MainFunction
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
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_AnnounceRcvSmIterType announceRcvSmIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all Announce receive state machines. */
  for (announceRcvSmIter = 0; announceRcvSmIter < EthTSyn_GetSizeOfAnnounceRcvSm(); announceRcvSmIter++)
  {
    /* #20 Enter critical section and reset state machine if requested. */
    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
    if (EthTSyn_IsSmResetRequestedOfAnnounceRcvSmDyn(announceRcvSmIter))
    {
      EthTSyn_AnnounceRcvSm_IntInit(announceRcvSmIter);
    }

    /* #30 Decrement Announce Rx timeout counter if possible. */
    if (EthTSyn_GetRxTimeoutCounterOfAnnounceRcvSmDyn(announceRcvSmIter) > 0u)
    {
      EthTSyn_DecRxTimeoutCounterOfAnnounceRcvSmDyn(announceRcvSmIter);

      /* #40 Check if Announce Rx timeout counter is expired. */
      if (EthTSyn_GetRxTimeoutCounterOfAnnounceRcvSmDyn(announceRcvSmIter) == 0u)
      {
        /* #400 Reset valid master of SlaveRx unit. */
        const EthTSyn_SizeOfSlaveRxDataType slaveRxDataIdx =
          EthTSyn_GetSlaveRxDataIdxOfAnnounceRcvSm(announceRcvSmIter);
        EthTSyn_SlaveRx_ResetValidMaster(slaveRxDataIdx);

        /* #401 Set state to NO_VALID_MASTER. */
        EthTSyn_SetStateOfAnnounceRcvSmDyn(announceRcvSmIter, ETHTSYN_NO_VALID_MASTER_STATEOFANNOUNCERCVSMDYN);
      }
    }

    /* #50 Leave critical section. */
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  }
} /* EthTSyn_AnnounceRcvSm_MainFunction() */

/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_Reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_Reset(
  EthTSyn_AnnounceRcvSm_SmIdxType AnnounceRcvSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if AnnounceRcvSm exists. */
  if (AnnounceRcvSmIdx < EthTSyn_GetSizeOfAnnounceRcvSm())
  {
    /* #20 Trigger a reset of the passed AnnounceRcvSm. */
    EthTSyn_SetSmResetRequestedOfAnnounceRcvSmDyn(AnnounceRcvSmIdx, TRUE);
  }
} /* EthTSyn_AnnounceRcvSm_Reset() */

/**********************************************************************************************************************
 *  EthTSyn_AnnounceRcvSm_ProcRcvdAnnounce
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
FUNC(void, ETHTSYN_CODE) EthTSyn_AnnounceRcvSm_ProcRcvdAnnounce(
                  EthTSyn_AnnounceRcvSm_SmIdxType AnnounceRcvSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)             PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                          AnnouncePayloadPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if AnnounceRcvSm exists. */
  if (AnnounceRcvSmIdx < EthTSyn_GetSizeOfAnnounceRcvSm())
  {
    /* #20 Check if Announce message is valid. */
    if (EthTSyn_AnnounceRcvSm_IsAnnounceValid(AnnounceRcvSmIdx, PtpHdrPtr, AnnouncePayloadPtr) == TRUE)
    {
      /* #30 Enter critical section and check if state is NO_VALID_MASTER. */
      SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();
      if (EthTSyn_GetStateOfAnnounceRcvSmDyn(AnnounceRcvSmIdx) == ETHTSYN_NO_VALID_MASTER_STATEOFANNOUNCERCVSMDYN)
      {
        /* 300 Set valid master of SlaveRx unit. */
        const EthTSyn_SizeOfSlaveRxDataType slaveRxDataIdx =
          EthTSyn_GetSlaveRxDataIdxOfAnnounceRcvSm(AnnounceRcvSmIdx);
        const uint64 clockIdentity =
          EthTSyn_Mem_GetUint64(AnnouncePayloadPtr, ETHTSYN_MSG_ANNOUNCE_GM_IDENTITY_PL_REL_OFS);
        EthTSyn_SlaveRx_SetValidMaster(slaveRxDataIdx, clockIdentity);

        /* #301 Set state to VALID_MASTER_SET. */
        EthTSyn_SetStateOfAnnounceRcvSmDyn(AnnounceRcvSmIdx, ETHTSYN_VALID_MASTER_SET_STATEOFANNOUNCERCVSMDYN);
      }

      /* #40 Reset Announce Rx timeout counter. */
      EthTSyn_SetRxTimeoutCounterOfAnnounceRcvSmDyn(AnnounceRcvSmIdx,
        EthTSyn_GetRxTimeoutOfAnnounceRcvSm(AnnounceRcvSmIdx));

      /* #50 Leave critical section. */
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_RX();

      /* #60 Report reception of Announce message to PortParamStats unit. */
      EthTSyn_PortParamStats_IncRxAnnounceCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(
        EthTSyn_GetComCtrlPortIdxOfAnnounceRcvSm(AnnounceRcvSmIdx)));
    }
    else
    {
      /* #70 If Announce message is invalid, report to PortParamStats unit that message was discarded. */
      EthTSyn_PortParamStats_IncRxDiscardCnt(EthTSyn_GetPortParamStatsDynIdxOfComCtrlPort(
        EthTSyn_GetComCtrlPortIdxOfAnnounceRcvSm(AnnounceRcvSmIdx)));
    }
  }
} /* EthTSyn_AnnounceRcvSm_ProcRcvdAnnounce() */
# endif /* ETHTSYN_NOUNIT_ANNOUNCERCVSM */

/**********************************************************************************************************************
 * Public interfaces of SyncRcvSm
 *!
 * \unit SyncRcvSm
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SYNCRCVSM)
/**********************************************************************************************************************
 *  EthTSyn_SyncRcvSm_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SyncRcvSm_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SyncRcvSmIterType syncRcvSmIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize dynamic data of all Sync receive state machines. */
  for (syncRcvSmIter = 0u; syncRcvSmIter < EthTSyn_GetSizeOfSyncRcvSm(); syncRcvSmIter++)
  {
    EthTSyn_SyncRcvSm_IntInit(syncRcvSmIter);
  }
} /* EthTSyn_SyncRcvSm_Init() */

/**********************************************************************************************************************
 *  EthTSyn_SyncRcvSm_MainFunction
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SyncRcvSm_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SyncRcvSmIterType syncRcvSmIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all Sync receive state machines. */
  for (syncRcvSmIter = 0u; syncRcvSmIter < EthTSyn_GetSizeOfSyncRcvSm(); syncRcvSmIter++)
  {
    /* #20 Enter critical section to protect the data of the SyncRcvSm. */
    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

    /* #30 Check if a reset of the SyncRcvSm is requested. */
    if (EthTSyn_IsResetRequestedOfSyncRcvSmDyn(syncRcvSmIter))
    {
      /* #300 Reset the state machine. */
      EthTSyn_SyncRcvSm_IntInit(syncRcvSmIter);
    }
    /* #31 Otherwise. */
    else
    {
      /* #40 Decrement the FollowUp reception timeout counter if applicable. */
      if (EthTSyn_GetFupRxTimeoutCntOfSyncRcvSmDyn(syncRcvSmIter) > 0u)
      {
        EthTSyn_DecFupRxTimeoutCntOfSyncRcvSmDyn(syncRcvSmIter);

        /* #50 Check if the FollowUp reception timeout expired. */
        if (EthTSyn_GetFupRxTimeoutCntOfSyncRcvSmDyn(syncRcvSmIter) == 0u)
        {
          /* #60 Report to PortParamStats unit that timeout expired. */
          EthTSyn_PortParamStats_IncRxDiscardCnt(EthTSyn_GetPortParamStatsDynIdxOfSyncRcvSm(syncRcvSmIter));

          /* #70 Change state of the SyncRcvSm to WAIT_RX_SYNC. */
          EthTSyn_SetStateOfSyncRcvSmDyn(syncRcvSmIter, ETHTSYN_WAIT_RX_SYNC_STATEOFSYNCRCVSMDYN);
        }
      }
    }

    /* #80 Leave critical section. */
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  }
} /* EthTSyn_SyncRcvSm_MainFunction() */

/**********************************************************************************************************************
 *  EthTSyn_SyncRcvSm_Reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SyncRcvSm_Reset(
  EthTSyn_SyncRcvSm_SmIdxType SyncRcvSmIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger a reset of the passed SyncRcvSm. */
  EthTSyn_SetResetRequestedOfSyncRcvSmDyn(SyncRcvSmIdx, TRUE);
} /* EthTSyn_SyncRcvSm_Reset() */

/**********************************************************************************************************************
 *  EthTSyn_SyncRcvSm_ProcRcvdSync
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncRcvSm_ProcRcvdSync(
                  EthTSyn_SyncRcvSm_SmIdxType SyncRcvSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)         PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_SyncRxTimeInfoType) SyncRxTimeInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check validity of the sequence id. */
  if (EthTSyn_SyncRcvSm_IsSyncSequenceIdValid(SyncRcvSmIdx, PtpHdrPtr->SequenceId) == TRUE)
  {
    /* #20 Check for Sync reception without former FollowUp reception (shall be treated as FollowUp reception timeout).
     *       Report event to PortParamStats unit.
     *       Set state to WAIT_RX_SYNC. */
    if (EthTSyn_GetStateOfSyncRcvSmDyn(SyncRcvSmIdx) == ETHTSYN_WAIT_RX_FUP_STATEOFSYNCRCVSMDYN)
    {
      EthTSyn_PortParamStats_IncRxDiscardCnt(EthTSyn_GetPortParamStatsDynIdxOfSyncRcvSm(SyncRcvSmIdx));

      EthTSyn_SetStateOfSyncRcvSmDyn(SyncRcvSmIdx, ETHTSYN_WAIT_RX_SYNC_STATEOFSYNCRCVSMDYN);
    }

    /* #30 Store source port identity of the Sync message for later comparision with the FollowUp attributes. */
    EthTSyn_SetSyncSrcPortNumOfSyncRcvSmDyn(SyncRcvSmIdx, PtpHdrPtr->SourcePortNumber);
    EthTSyn_SetSyncSrcClockIdOfSyncRcvSmDyn(SyncRcvSmIdx, PtpHdrPtr->SourceClockId);

    /* #40 Store the Sync reception time information for later processing of the corresponding FollowUp message. */
    EthTSyn_SetSyncRxTimeInfoOfSyncRcvSmDyn(SyncRcvSmIdx, (*SyncRxTimeInfoPtr));

    /* #50 Recharge the FollowUp receive timeout counter. */
    EthTSyn_SetFupRxTimeoutCntOfSyncRcvSmDyn(SyncRcvSmIdx, EthTSyn_GetFollowUpRxTimeoutOfSyncRcvSm(SyncRcvSmIdx));

    /* #60 Report reception of Sync message to PortParamStats unit. */
    EthTSyn_PortParamStats_IncRxSyncCnt(EthTSyn_GetPortParamStatsDynIdxOfSyncRcvSm(SyncRcvSmIdx));

    /* #70 Change state of the SyncRcvSm to WAIT_RX_FUP. */
    EthTSyn_SetStateOfSyncRcvSmDyn(SyncRcvSmIdx, ETHTSYN_WAIT_RX_FUP_STATEOFSYNCRCVSMDYN);

    retVal = E_OK;
  }

  return retVal;
} /* EthTSyn_SyncRcvSm_ProcRcvdSync() */

/**********************************************************************************************************************
 *  EthTSyn_SyncRcvSm_ProcRcvdFup
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
FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_SyncRcvSm_ProcRcvdFup(
                  EthTSyn_SyncRcvSm_SmIdxType SyncRcvSmIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)         PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                      FupPayloadPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if SyncRcvSm is in state WAIT_RX_FUP. */
  if (EthTSyn_GetStateOfSyncRcvSmDyn(SyncRcvSmIdx) == ETHTSYN_WAIT_RX_FUP_STATEOFSYNCRCVSMDYN)
  {
    /* #20 Check if the FollowUp message matches the last received Sync message. */
    if (EthTSyn_SyncRcvSm_IsFupMatchingSync(SyncRcvSmIdx, PtpHdrPtr) == TRUE)
    {
      const EthTSyn_FupRxHandlerIdxOfSyncRcvSmType fupRxHandlerIdx =
        EthTSyn_GetFupRxHandlerIdxOfSyncRcvSm(SyncRcvSmIdx);

      /* #30 Check if the FollowUp message structure is valid. */
      if (EthTSyn_FupRxHandler_IsRcvdFupValid(fupRxHandlerIdx, PtpHdrPtr, FupPayloadPtr) == TRUE) /* PRQA S 2995 */ /* MD_EthTSyn_2995_PP */
      {
        /* #40 Process the received FollowUp message and change state of the SyncRcvSm to WAIT_RX_SYNC. */
        EthTSyn_FupRxHandler_ProcRcvdFup(fupRxHandlerIdx, EthTSyn_GetAddrSyncRxTimeInfoOfSyncRcvSmDyn(SyncRcvSmIdx),
          PtpHdrPtr, FupPayloadPtr);

        EthTSyn_SetStateOfSyncRcvSmDyn(SyncRcvSmIdx, ETHTSYN_WAIT_RX_SYNC_STATEOFSYNCRCVSMDYN);

        /* #50 Report reception of FollowUp message to PortParamStats unit. */
        EthTSyn_PortParamStats_IncRxFupCnt(EthTSyn_GetPortParamStatsDynIdxOfSyncRcvSm(SyncRcvSmIdx));

        /* #60 Stop the FollowUp reception timeout. */
        EthTSyn_SetFupRxTimeoutCntOfSyncRcvSmDyn(SyncRcvSmIdx, 0u);

        /* #70 Forward FollowUp message and Sync switch ingress timestamp to SwtTimeSyncSm (if switch time
         *     synchronization is enabled). */
#  if (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON)
        {
          EthTSyn_PdelayType pdelay = EthTSyn_ComCtrl_GetPdelay(EthTSyn_GetComCtrlPortExternalIdxOfFupRxHandler(
            EthTSyn_GetFupRxHandlerIdxOfSyncRcvSm(SyncRcvSmIdx)));
          EthTSyn_SwtTimeSyncSm_ProvideTsInfoFromSyncRcvSm(EthTSyn_GetSwtTimeSyncSmIdxOfSyncRcvSm(SyncRcvSmIdx),
            pdelay, PtpHdrPtr, FupPayloadPtr,
            &EthTSyn_GetAddrSyncRxTimeInfoOfSyncRcvSmDyn(SyncRcvSmIdx)->SyncSwtIngrTsMgmt);
        }
#  endif /* (ETHTSYN_SWT_TIME_SYNC_SUPPORT == STD_ON) */

        retVal = E_OK;
      }
    }
  }

  return retVal;
} /* EthTSyn_SyncRcvSm_ProcRcvdFup() */

# endif /* ETHTSYN_NOUNIT_SYNCRCVSM */

/**********************************************************************************************************************
 * Public interfaces of SlaveRx
 *!
 * \unit SlaveRx
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_SLAVERX)
/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_Init
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  EthTSyn_SlaveRxDataIterType slaveRxDataIter = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the master clock id of every slaveRxData element. */
  for (slaveRxDataIter = 0u;
       slaveRxDataIter < EthTSyn_GetSizeOfSlaveRxData();
       slaveRxDataIter++)
  {
    /* #20 Check if master clock id is used for the current slaveRxData element (if master-slave conflict detection is
     *     disabled, otherwise it is always used). */
    const EthTSyn_SlaveRxMasterClockIdDynIdxOfSlaveRxDataType masterClockIdDynIdx =
      EthTSyn_GetSlaveRxMasterClockIdDynIdxOfSlaveRxData(slaveRxDataIter);

#  if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF)
    if (masterClockIdDynIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn())
#  endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF) */
    {
      /*@ assert masterClockIdDynIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn(); */

      /* #30 Check if source port identity check is used. */
      const EthTSyn_SlaveRxSrcPortIdCheckClockIdIdxOfSlaveRxDataType srcPortIdCheckClockIdIdx =
        EthTSyn_GetSlaveRxSrcPortIdCheckClockIdIdxOfSlaveRxData(slaveRxDataIter);

      if (srcPortIdCheckClockIdIdx < EthTSyn_GetSizeOfSlaveRxSrcPortIdCheckClockId())
      {
        /* #40 Initialize the master clock id with the configured value for the source port identity check. */
        EthTSyn_SetValidMasterClockIdOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx,
          EthTSyn_GetSlaveRxSrcPortIdCheckClockId(srcPortIdCheckClockIdIdx));
        EthTSyn_SetValidMasterSetOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx, TRUE);
      }
      /* #31 Otherwise, reset the master clock id. */
      else
      {
        EthTSyn_SetValidMasterClockIdOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx, 0u);
        EthTSyn_SetValidMasterSetOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx, FALSE);
      }
    }
  }
} /* EthTSyn_SlaveRx_Init() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_Reset
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_Reset(
  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if slave rx data handler index is valid. */
  if (SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler())
  {
    /* #20 Trigger reset of the 'external' slave rx data. */
    EthTSyn_SlaveRx_ResetSlaveRxData(EthTSyn_GetSlaveRxDataExternalIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx));

    /* #30 Trigger reset of the 'internal' slave rx data if present (if switch management is enabled). */
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
    {
      const EthTSyn_SlaveRx_DataHandlerIdxType slaveRxDataIntIdx =
        EthTSyn_GetSlaveRxDataInternalIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx);

#   if ( (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_TRANSPARENT) || \
         (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_ASYMMETRIC_TRANSPARENT) )
      if (slaveRxDataIntIdx < EthTSyn_GetSizeOfSlaveRxData())
#   endif /* (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_TRANSPARENT) ||
           * (ETHTSYN_SWT_MGMT_BRIDGE_MODE == ETHTSYN_SWT_MGMT_BRIDGE_MODE_ASYMMETRIC_TRANSPARENT) */
      {
        /*@ assert slaveRxDataIntIdx < EthTSyn_GetSizeOfSlaveRxData(); */
        EthTSyn_SlaveRx_ResetSlaveRxData(slaveRxDataIntIdx);
      }
    }
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
  }
} /* EthTSyn_SlaveRx_Reset() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdSync
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
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdSync(
                   EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                 PtpHdrPtr,
  /* Note: Sync does not carry relevant payload (just reserved). Therefore, the payload is not forwarded. */
  ETHTSYN_P2CONST(EthTSyn_RxTsInfoType)               RxTsInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if slave rx data handler index is valid (i.e. Sync message was received on an external slave port). */
  if (SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler())
  {
    /* #20 Verify the Sync message length. */
    if (PtpHdrPtr->MessageLength == ETHTSYN_MSG_SYNC_LEN)
    {
      const EthTSyn_ComCtrlPortIdxOfSlaveRxDataHandlerType comCtrlPortIdx =
        EthTSyn_GetComCtrlPortIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx);

      /* #30 Check if underlying port is AsCapable. */
      if (EthTSyn_ComCtrl_IsAsCapable(comCtrlPortIdx) == TRUE)
      {
        /* #40 Forward the received Sync message according to the operation mode of the EthTSyn. */
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
        /* Bridge mode */
        EthTSyn_SlaveRx_ProcRcvdSyncBridge(SlaveRxDataHandlerIdx, PtpHdrPtr, RxTsInfoPtr);
#  else
        /* End station mode */
        EthTSyn_SlaveRx_ProcRcvdSyncEndStation(SlaveRxDataHandlerIdx, PtpHdrPtr, RxTsInfoPtr);
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
      }
    }
  }
  /* #11 Otherwise, report a DET runtime error ETHTSYN_E_TMCONFLICT (if master-slave conflict detection is enabled). */
#  if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
  else
  {
    (void)Det_ReportRuntimeError(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, ETHTSYN_SID_SLAVE_RX_PROC_RCVD_SYNC,
      ETHTSYN_E_TMCONFLICT);
  }
#  endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON) */
} /* EthTSyn_SlaveRx_ProcRcvdSync() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdFup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdFup(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                             FupPayloadPtr)
{
  /* #10 Check if slave rx data handler index is valid (i.e. FollowUp message was received on a Slave-port). */
  if (SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler())
  {
    /* #20 Verify FollowUp message length. */
    if (EthTSyn_SlaveRx_CheckFupMsgLen(PtpHdrPtr->MessageLength) == TRUE)
    {
      const EthTSyn_ComCtrlPortIdxOfSlaveRxDataHandlerType comCtrlPortIdx =
        EthTSyn_GetComCtrlPortIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx);

      /* #30 Check if underlying port is AsCapable. */
      if (EthTSyn_ComCtrl_IsAsCapable(comCtrlPortIdx) == TRUE)
      {
        /* #40 Forward the FollowUp message according to the operation mode of the EthTSyn. */
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
      /* Bridge mode */
        EthTSyn_SlaveRx_ProcRcvdFupBridge(SlaveRxDataHandlerIdx, PtpHdrPtr, FupPayloadPtr);
#  else
      /* End station mode */
        EthTSyn_SlaveRx_ProcRcvdFupEndStation(SlaveRxDataHandlerIdx, PtpHdrPtr, FupPayloadPtr);
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
      }
    }
  }
} /* EthTSyn_SlaveRx_ProcRcvdFup() */

/**********************************************************************************************************************
 *  EthTSyn_SlaveRx_ProcRcvdAnnounce
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ProcRcvdAnnounce(
                  EthTSyn_SlaveRx_DataHandlerIdxType SlaveRxDataHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)                PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                             AnnouncePayloadPtr)
{
  /* #10 Check if slave rx data handler index is valid (i.e. Announce message was received on a Slave-port). */
  if (SlaveRxDataHandlerIdx < EthTSyn_GetSizeOfSlaveRxDataHandler())
  {
    /* #20 Verify Announce message length. */
    if (PtpHdrPtr->MessageLength >= ETHTSYN_MSG_ANNOUNCE_LEN)
    {
      const EthTSyn_ComCtrlPortIdxOfSlaveRxDataHandlerType comCtrlPortIdx =
        EthTSyn_GetComCtrlPortIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx);

      /* #30 Check if underlying port is AsCapable. */
      if (EthTSyn_ComCtrl_IsAsCapable(comCtrlPortIdx) == TRUE)
      {
        /* Announce is only supported for an external slave port. */
        const EthTSyn_SlaveRxDataExternalIdxOfSlaveRxDataHandlerType slaveRxDataIdx =
          EthTSyn_GetSlaveRxDataExternalIdxOfSlaveRxDataHandler(SlaveRxDataHandlerIdx);

        /* #40 Forward the Announce message to the AnnounceRcvSm for further processing. */
        EthTSyn_AnnounceRcvSm_ProcRcvdAnnounce(EthTSyn_GetAnnounceRcvSmIdxOfSlaveRxData(slaveRxDataIdx),
          PtpHdrPtr, AnnouncePayloadPtr);
      }
    }
  }
} /* EthTSyn_SlaveRx_ProcRcvdAnnounce() */

/**********************************************************************************************************************
 * EthTSyn_SlaveRx_ResetValidMaster
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_ResetValidMaster(
  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_SlaveRxMasterClockIdDynIdxOfSlaveRxDataType masterClockIdDynIdx =
    EthTSyn_GetSlaveRxMasterClockIdDynIdxOfSlaveRxData(SlaveRxDataIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if valid master information is used by the SlaveRxData (if master-slave conflict detection is disabled,
   *     otherwise it is always used). */
#  if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF)
  if (masterClockIdDynIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn())
#  endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF) */
  {
    /*@ assert masterClockIdDynIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn(); */

    /* #20 Check if no static master is configured. */
    if (!EthTSyn_IsSlaveRxSrcPortIdCheckClockIdUsedOfSlaveRxData(SlaveRxDataIdx))
    {
      /* #30 Reset the valid master clock id with protection against interrupts. */
      SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
      EthTSyn_SetValidMasterClockIdOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx, 0u);
      EthTSyn_SetValidMasterSetOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx, FALSE);
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
    }
  }
} /* EthTSyn_SlaveRx_ResetValidMaster() */

/**********************************************************************************************************************
 * EthTSyn_SlaveRx_SetValidMaster
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_SlaveRx_SetValidMaster(
  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx,
  uint64                      ValidMasterClockId)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_SlaveRxMasterClockIdDynIdxOfSlaveRxDataType masterClockIdDynIdx =
    EthTSyn_GetSlaveRxMasterClockIdDynIdxOfSlaveRxData(SlaveRxDataIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if valid master information is used by the SlaveRxData (if master-slave conflict detection is disabled,
   *     otherwise it is always used). */
#  if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF)
  if (masterClockIdDynIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn())
#  endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF) */
  {
    /*@ assert masterClockIdDynIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn(); */

    /* #20 Check if no static master is configured. */
    if (!EthTSyn_IsSlaveRxSrcPortIdCheckClockIdUsedOfSlaveRxData(SlaveRxDataIdx))
    {
      /* #30 Set the valid master clock id with protection against interrupts. */
      SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
      EthTSyn_SetValidMasterClockIdOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx, ValidMasterClockId);
      EthTSyn_SetValidMasterSetOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx, TRUE);
      SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
    }
  }
} /* EthTSyn_SlaveRx_SetValidMaster() */

/**********************************************************************************************************************
 * EthTSyn_SlaveRx_CheckValidMasterIfSet
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
FUNC(boolean, ETHTSYN_CODE) EthTSyn_SlaveRx_CheckValidMasterIfSet(
  EthTSyn_SlaveRx_DataIdxType SlaveRxDataIdx,
  uint64                      MasterClockId)
{
  /* ----- Local Variables ---------------------------------------------- */
  const EthTSyn_SlaveRxMasterClockIdDynIdxOfSlaveRxDataType masterClockIdDynIdx =
    EthTSyn_GetSlaveRxMasterClockIdDynIdxOfSlaveRxData(SlaveRxDataIdx);
  boolean masterValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if valid master information is used by the SlaveRxData (if master-slave conflict detection is disabled,
   *     otherwise it is always used). */
#  if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF)
  if (masterClockIdDynIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn())
#  endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_OFF) */
  {
    /*@ assert masterClockIdDynIdx < EthTSyn_GetSizeOfSlaveRxMasterClockIdDyn(); */

    /* #20 Enter critical section to protect read/write of valid master information. */
    SchM_Enter_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();

    /* #30 Check if a valid master is already set. */
    if (EthTSyn_IsValidMasterSetOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx))
    {
      /* #40 Compare the set valid master clock id with the passed one. */
      masterValid = (boolean)(
        MasterClockId == EthTSyn_GetValidMasterClockIdOfSlaveRxMasterClockIdDyn(masterClockIdDynIdx));
    }
    /* #50 Exit critical section. */
    SchM_Exit_EthTSyn_ETHTSYN_EXCLUSIVE_AREA_0();
  }

  return masterValid;
} /* EthTSyn_SlaveRx_CheckValidMasterIfSet() */
# endif /* ETHTSYN_NOUNIT_SLAVERX */

/**********************************************************************************************************************
 * Public interfaces of FupRxHandler
 *!
 * \unit FupRxHandler
 *********************************************************************************************************************/
# if !defined(ETHTSYN_NOUNIT_FUPRXHANDLER)
/**********************************************************************************************************************
 *  EthTSyn_FupRxHandler_IsRcvdFupValid
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
FUNC(boolean, ETHTSYN_CODE) EthTSyn_FupRxHandler_IsRcvdFupValid(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)          PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                       FupPayloadPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean msgValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that the correction field of the message header does not exceed the configured threshold or the maximum
   *     correction field value if no threshold is configured. */
  if (PtpHdrPtr->CorrectionFieldNs <= EthTSyn_GetFupCorrFieldThresholdNsOfFupRxHandler(FupRxHandlerIdx))
  {
    /* #20 Verify value range of precise origin timestamp. */
    EthTSyn_GlobalTimestampType pot = {0};

    pot.secondsHi = EthTSyn_Mem_GetUint16(FupPayloadPtr, ETHTSYN_MSG_FUP_POT_SEC_HI_PL_REL_OFS);
    pot.seconds = EthTSyn_Mem_GetUint32(FupPayloadPtr, ETHTSYN_MSG_FUP_POT_SEC_LOW_PL_REL_OFS);
    pot.nanoseconds = EthTSyn_Mem_GetUint32(FupPayloadPtr, ETHTSYN_MSG_FUP_POT_NSEC_PL_REL_OFS);

    if (EthTSyn_Ts_IsTsValid(&pot) == TRUE)
    {
      /* #30 If message compliance is disabled. */
#  if (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)
      {
        /* #40 Reset stored ArSubTLV information. */
        EthTSyn_FupRxHandler_ResetTlvInfo(FupRxHandlerIdx);

        /* #50 Check the FollowUp message structure and optional CRC setting. */
        msgValid = EthTSyn_FupRxHandler_CheckFupMsgStructureAndCrc(FupRxHandlerIdx, PtpHdrPtr, FupPayloadPtr);
      }
#  else
      /* #31 Otherwise, no further checks are required here. Consider the FollowUp message as valid. */
      {
        msgValid = TRUE;

        ETHTSYN_DUMMY_STATEMENT(FupRxHandlerIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
        ETHTSYN_DUMMY_STATEMENT(PtpHdrPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
        ETHTSYN_DUMMY_STATEMENT(FupPayloadPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
      }
#  endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) */
    }
  }

  return msgValid;
} /* EthTSyn_FupRxHandler_IsRcvdFupValid() */

/**********************************************************************************************************************
 *  EthTSyn_FupRxHandler_ProcRcvdFup
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, ETHTSYN_CODE) EthTSyn_FupRxHandler_ProcRcvdFup(
                  EthTSyn_FupRxHandler_IdxType FupRxHandlerIdx,
  ETHTSYN_P2CONST(EthTSyn_SyncRxTimeInfoType)  SyncRxTimeInfoPtr,
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType)          PtpHdrPtr,
  ETHTSYN_P2CONST(uint8)                       FupPayloadPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the slave is used to report the data to the StbM (if switch management is enabled). */
#  if (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON)
  if (EthTSyn_IsReportDataToStbMOfFupRxHandler(FupRxHandlerIdx))
#  endif /* (ETHTSYN_SWT_MGMT_SUPPORT == STD_ON) */
  {
    /* #20 Check if the virtual local time at Sync reception was valid. */
    if (SyncRxTimeInfoPtr->SyncRxTimeInfoValid == TRUE)
    {
      /* #30 Provide new global time and optional new offset values to the StbM. */
      EthTSyn_FupRxHandler_ForwardDataToStbM(FupRxHandlerIdx, SyncRxTimeInfoPtr, PtpHdrPtr, FupPayloadPtr);
    }
  }
} /* EthTSyn_FupRxHandler_ProcRcvdFup() */
# endif /* ETHTSYN_NOUNIT_FUPRXHANDLER */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
#endif /* (ETHTSYN_SLAVE_SUPPORT == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Slave.c
 *********************************************************************************************************************/
