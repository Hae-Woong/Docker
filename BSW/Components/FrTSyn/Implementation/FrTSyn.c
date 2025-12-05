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
/*!        \file  FrTSyn.c
 *        \brief  Vector AUTOSAR FrTSyn code file
 *
 *      \details  This is the main implementation file of the Vector AUTOSAR module FrTSyn.
 *
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

#define FRTSYN_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "FrTSyn.h"
#include "FrTSyn_Cbk.h"
#include "SchM_FrTSyn.h"
#include "StbM.h"
#include "FrIf.h"
#if (FRTSYN_CRC_USED)
# include "Crc.h"
#endif
#if (FRTSYN_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif
#include "vstdlib.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of FrTSyn header file */
#if (  (FRTSYN_SW_MAJOR_VERSION != (6u)) \
    || (FRTSYN_SW_MINOR_VERSION != (3u)) \
    || (FRTSYN_SW_PATCH_VERSION != (0u)) )
# error "Vendor specific version numbers of FrTSyn.c and FrTSyn.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (FRTSYN_CFG_MAJOR_VERSION != (6u)) \
    || (FRTSYN_CFG_MINOR_VERSION != (3u)) )
# error "Version numbers of FrTSyn.c and FrTSyn_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#define FRTSYN_INV_DOMAIN_IDX                              (255u)
#define FRTSYN_INV_MASTER_IDX                              (255u)
#define FRTSYN_INV_SLAVE_IDX                               (255u)
#define FRTSYN_INV_SEQUENCE_COUNTER                        (255u)
#define FRTSYN_MAX_SEQUENCE_COUNTER                        (15u)
#define FRTSYN_START_OFFSET_TIME_DOMAIN_ID                 (16u)

#define FRTSYN_CRC_START_VALUE                             (0xFFu)

#define FRTSYN_NANOSECONDS_IN_ONE_SECOND                   (1000000000uL)
#define FRTSYN_TIMESTAMP_MAX_NANOSECONDS                   (999999999uL) /* 10^9 nanoseconds per second */
#define FRTSYN_TIMESTAMP_MAX_SECONDS                       (4294967295uL)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

#define FrTSyn_GetGlobalTimeBaseBit(status)                ((status) & StbM_Global_Time_Base_BflMask) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define FrTSyn_GetSyncToGatewayBit(status)                 ((status) & StbM_Sync_To_Gateway_BflMask) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define FrTSyn_GetTimeoutBit(status)                       ((status) & StbM_Timeout_BflMask) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define FrTSyn_IsSyncMsg(type)                             (((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_NOT_CRC) || ((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define FrTSyn_IsOfsMsg(type)                              (((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_NOT_CRC) || ((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_CRC)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#define FrTSyn_IsSyncCrcSecured(type)                      (((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC) || ((type) == FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_CRC)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Get Message type (Multiplexer value) */
#define FrTSyn_FrSignal_GetType(msg)                       ((*msg)[0]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Get CRC */
#define FrTSyn_FrSignal_GetCRC(msg)                        ((*msg)[1]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Get Sequence counter */
#define FrTSyn_FrSignal_GetSC(msg)                         ((*msg)[2] & 0x0Fu) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Get Time Domain */
#define FrTSyn_FrSignal_GetD(msg)                          ((*msg)[2] >> 4u) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Get SGW */
#define FrTSyn_FrSignal_GetSGW(syncMsg)                    ((uint8)(((*syncMsg)[3] & 0x02u) >> 1u)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Get FCNT */
#define FrTSyn_FrSignal_GetFCNT(syncMsg)                   ((*syncMsg)[3] >> 2u) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Get the time value */
#define FrTSyn_FrSignal_GetSecHi(msg)                      (((uint16) (*msg)[7]) | ((uint16) (*msg)[6] << 8u)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define FrTSyn_FrSignal_GetSec(msg)                        (((uint32) (*msg)[11]) | ((uint32) (*msg)[10] << 8u) | ((uint32) (*msg)[9] << 16u) | ((uint32) (*msg)[8]  << 24u)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define FrTSyn_FrSignal_GetNSec(msg)                       (((uint32) (*msg)[15]) | ((uint32) (*msg)[14] << 8u) | ((uint32) (*msg)[13] << 16u) | ((uint32) (*msg)[12] << 24u)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Get User bytes from SYNC message (Byte 2 only valid for not CRC secured messages) */
#define FrTSyn_FrSignal_GetUserByte0(syncMsg)              ((*syncMsg)[4]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define FrTSyn_FrSignal_GetUserByte1(syncMsg)              ((*syncMsg)[5]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define FrTSyn_FrSignal_GetUserByte2(syncMsg)              ((*syncMsg)[1]) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Set Message type (Multiplexer value) */
#define FrTSyn_FrSignal_SetType(msg, value)                (*msg)[0] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Set CRC */
#define FrTSyn_FrSignal_SetCRC(msg, value)                 (*msg)[1] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Set message byte 2 (Time Domain and Sequence counter) */
#define FrTSyn_FrSignal_SetByte2(msg, domain, sc)          (*msg)[2] = (uint8) (((domain) << 4u) | ((sc) & 0x0Fu)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Clear reserved bits in OFS message */
#define FrTSyn_FrSignal_ClearOfsReserved(msg)              (*msg)[3] = (uint8) ((*msg)[3] & 0x02u) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Set message byte 3 (FCNT, SGW and reserved) */
#define FrTSyn_FrSignal_SetByte3(msg, sgw, fcnt)           (*msg)[3] = (uint8) (((((sgw) & 0x04u) >> 1u) | ((fcnt) << 2u)) & 0xFEu) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Set the time value */
#define FrTSyn_FrSignal_SetSecHi(msg, value)               {(*msg)[7] = (uint8) (value);          \
                                                            (*msg)[6] = (uint8) ((value) >>  8u);}

#define FrTSyn_FrSignal_SetSec(msg, value)                 {(*msg)[11] = (uint8) (value);          \
                                                            (*msg)[10] = (uint8) ((value) >>  8u); \
                                                            (*msg)[9]  = (uint8) ((value) >> 16u); \
                                                            (*msg)[8]  = (uint8) ((value) >> 24u);}

#define FrTSyn_FrSignal_SetNSec(msg, value)                {(*msg)[15] = (uint8) (value);          \
                                                            (*msg)[14] = (uint8) ((value) >>  8u); \
                                                            (*msg)[13] = (uint8) ((value) >> 16u); \
                                                            (*msg)[12] = (uint8) ((value) >> 24u);}

/* Set User bytes from SYNC message (Byte 2 only valid for not CRC secured messages) */
#define FrTSyn_FrSignal_SetUserByte0(syncMsg, value)       (*syncMsg)[4] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define FrTSyn_FrSignal_SetUserByte1(syncMsg, value)       (*syncMsg)[5] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#define FrTSyn_FrSignal_SetUserByte2(syncMsg, value)       (*syncMsg)[1] = (uint8) (value) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if !defined (FRTSYN_LOCAL) /* COV_FRTSYN_MSR_COMPATIBILITY */
# define FRTSYN_LOCAL static
#endif

#if !defined (FRTSYN_LOCAL_INLINE) /* COV_FRTSYN_MSR_COMPATIBILITY */
# define FRTSYN_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define FRTSYN_START_SEC_CODE
#include "FrTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  UNITS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Unit: Utility
 *********************************************************************************************************************/
/*! \brief      This unit is a helper unit providing calculation funcionality.
 *  \details    -
 *  \pre        -
 *  \trace      UTILITY_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_TimestampPlusTimestamp
 *********************************************************************************************************************/
/*! \brief      Adds two timestamps
 *  \details    This function calculates the sum of two timestamps.
 *  \param[in]  timestamp1Ptr       Pointer to timestamp where the first summand is stored
 *  \param[in]  timestamp2Ptr       Pointer to timestamp where the second summand is stored
 *  \param[out] timesumPtr          Pointer to timestamp where the calculated sum is stored
 *  \return     E_OK                Timesum is valid
 *  \return     E_NOT_OK            Timesum is invalid
 *  \pre        -
 *  \unit       Utility
 *********************************************************************************************************************/
FRTSYN_LOCAL FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TimestampPlusTimestamp(
    P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp1Ptr,
    P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp2Ptr,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timesumPtr);

#if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_TimestampMinusTimestamp
 *********************************************************************************************************************/
/*! \brief      Subtracts two timestamps
 *  \details    This function corrects the value of new global timestamp if current cycle is greater or equal than the
                retrieved FCNT value from the transmitter (Time Master).
 *  \param[in]  timestamp1Ptr       Pointer to first timestamp
 *  \param[in]  timestamp2Ptr       Pointer to second timestamp
 *  \param[out] timediffPtr         Pointer to timestamp with difference between timestamp1 and timestamp2
 *  \pre        timestamp1 is greater or equal to timestamp2
 *  \unit       Utility
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_TimestampMinusTimestamp(
    P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp1Ptr,
    P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp2Ptr,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timediffPtr);
#endif /* FRTSYN_SLAVECONFIG == STD_ON */

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *   FrTSyn_SubtractVirtualLocalTime
 *********************************************************************************************************************/
/*! \brief      Subtract two VirtualLocalTime types
 *  \details    This function calculates the difference of two VirtualLocalTime types.
 *  \param[in]  lhs                 Pointer to a VirtualLocalTime type where the minuend is stored
 *  \param[in]  rhs                 Pointer to a VirtualLocalTime type where the subtrahend is stored
 *  \param[out] timediffPtr         Pointer to the calculated difference
 *  \return     E_OK                Timediff is valid
 *  \return     E_NOT_OK            Timediff is invalid
 *  \pre        -
 *  \unit       Utility
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_SubtractVirtualLocalTime(
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_DATA) lhs,
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_DATA) rhs,
    P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_VAR) timediffPtr);
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

/**********************************************************************************************************************
 *  FrTSyn_ConvertTimespanToTimestamp
 *********************************************************************************************************************/
/*! \brief      Converts a time span to timestamp
 *  \details    -
 *  \param[in]  timespan            Time span that should be converted to a timestamp
 *  \param[out] timestampPtr        Pointer to timestamp calculated from time span value
 *  \pre        max value of timespan is 1.024s
 *  \unit       Utility
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_ConvertTimespanToTimestamp(
    uint32 timespan,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timestampPtr);

#if (FRTSYN_CRC_USED == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_CalculateMsgCrc
 *********************************************************************************************************************/
/*! \brief      Calculate CRC over byte 2-15 of the FR message and dataId.
 *  \details    -
 *  \param[in]  ptrDataId           Pointer to dataId
 *  \param[in]  ptrMsgInfo          Pointer to FR message
 *  \return     CRC                 Calculated CRC value
 *  \pre        -
 *  \unit       Utility
 *********************************************************************************************************************/
FRTSYN_LOCAL FUNC(uint8, FRTSYN_CODE) FrTSyn_CalculateMsgCrc(
    P2CONST(uint8, AUTOMATIC, FRTSYN_APPL_DATA) ptrDataId,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo);
#endif /* FRTSYN_CRC_USED == STD_ON */

#if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_IsOffsetDomain
 *********************************************************************************************************************/
/*! \brief      Check if given domain is an offset domain.
 *  \details    -
 *  \param[in]  domainIdx           Valid index of domain
 *  \return     TRUE                Domain is an offset domain
 *  \return     FALSE               Domain is no offset domain
 *  \pre        -
 *  \unit       Utility
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_IsOffsetDomain(FrTSyn_DomainConfigIterType domainIdx);
#endif /* FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON */

/**********************************************************************************************************************
 *
 *
 *  MASTER
 *
 *
 *********************************************************************************************************************/

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: Transmit
 *********************************************************************************************************************/
/*! \brief      This unit provides the master transmit functionality of the FrTsyn.
 *  \details    -
 *  \pre        -
 *  \trace      TRANSMIT_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Tx_Init
 *********************************************************************************************************************/
/*! \brief      Initialization of transmit unit
 *  \details    -
 *  \pre        -
 *  \unit       Transmit
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_Init(void);

/**********************************************************************************************************************
 *  FrTSyn_Tx_Update
 *********************************************************************************************************************/
/*! \brief      Update Tx unit
 *  \details    -
 *  \pre        -
 *  \unit       Transmit
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_Update(void);

/**********************************************************************************************************************
 *  FrTSyn_Tx_ProcessTriggerTransmit
 *********************************************************************************************************************/
/*! \brief      Processes trigger transmit actions
 *  \details    Verifies if any master matches the transmitted PduId and prepares message accordingly.
 *  \param[in]  pduId               Valid ID of the I-PDU that has been transmitted
 *  \param[in]  pduInfoPtr          Contains a pointer to a buffer (SduDataPtr) to where the SDU data shall be copied,
                                    and the available buffer size in SduLength. On return, the function will indicate
                                    the length of the copied SDU data in SduLength.
 *  \param[out] outErrorId          Signalizes if any master matches PduId
 *  \return     E_OK                Trigger transmit procedure was successful
 *  \return     E_NOT_OK            Otherwise
 *  \pre        -
 *  \unit       Transmit
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_ProcessTriggerTransmit(PduIdType pduId,
    P2VAR(PduInfoType, AUTOMATIC, FRTSYN_APPL_VAR) pduInfoPtr,
    P2VAR(uint8, AUTOMATIC, FRTSYN_APPL_VAR) outErrorId);

/**********************************************************************************************************************
 *  FrTSyn_Tx_OnSyncTriggerTransmit
 *********************************************************************************************************************/
/*! \brief      Prepares message transmission
 *  \details    Copies the message to a buffer and handles states and timers correspondingly.
 *  \param[in]  masterIdx           Valid index of the sending master
 *  \param[in]  pduInfoPtr          Contains a pointer to a buffer (SduDataPtr) to where the SDU data shall be copied,
                                    and the available buffer size in SduLength. On return, the function will indicate
                                    the length of the copied SDU data in SduLength.
 *  \return     E_OK                Message transmission was successful
 *  \return     E_NOT_OK            Message transmission failed
 *  \pre         -
 *  \unit       Transmit
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_OnSyncTriggerTransmit(
    FrTSyn_MasterConfigIterType masterIdx,
    P2VAR(PduInfoType, AUTOMATIC, FRTSYN_APPL_VAR) pduInfoPtr);

/**********************************************************************************************************************
 *  FrTSyn_Tx_InitMasters
 *********************************************************************************************************************/
/*! \brief      Initialization of masters
 *  \details    -
 *  \pre        -
 *  \unit       Transmit
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitMasters(void);

# if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_InitImmediateSendMode
 *********************************************************************************************************************/
/*! \brief      Initialization state of immediate send mode
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       Transmit
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitImmediateSendMode(FrTSyn_MasterConfigIterType masterIdx);
# endif /* FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON */
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: ProtocolTx
 *********************************************************************************************************************/
/*! \brief      This unit handles the protocol on the transmitting side.
 *  \details    -
 *  \pre        -
 *  \trace      PROTOCOLTX_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateMasters
 *********************************************************************************************************************/
/*! \brief      Updates all configured masters
 *  \details    -
 *  \pre        -
 *  \unit       ProtocolTx
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateMasters(void);

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateCounters
 *********************************************************************************************************************/
/*! \brief      Update counters
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       ProtocolTx
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateCounters(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateWaitingForSyncTriggerTransmit
 *********************************************************************************************************************/
/*! \brief      Update the state 'wait for trigger transmit'
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       ProtocolTx
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateWaitingForSyncTriggerTransmit(
    FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_OnFailedTransmit
 *********************************************************************************************************************/
/*! \brief      Executed after unsuccessful transmission of SYNC/OFS message.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       ProtocolTx
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Tx_OnFailedTransmit(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_ResetStateToSyncSend
 *********************************************************************************************************************/
/*! \brief      Resets the state of the master to 'wait for SYNC send'.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       ProtocolTx
 *********************************************************************************************************************/
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ResetStateToSyncSend(FrTSyn_MasterConfigIterType masterIdx);
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: SyncTx
 *********************************************************************************************************************/
/*! \brief      This unit handles the SYNC message mechanism on the transmitting side.
 *  \details    -
 *  \pre        -
 *  \trace      SYNCTX_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateWaitingForSyncSendRequest
 *********************************************************************************************************************/
/*! \brief      Update the state 'wait to send SYNC'
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       SyncTx
 *********************************************************************************************************************/
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateWaitingForSyncSendRequest(
    FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_TransmitSyncRequest
 *********************************************************************************************************************/
/*! \brief      Request message transmission
 *  \details    Assembles SYNC, OFS messages and requests message transmission via FR interface.
 *  \param[in]  masterIdx           Valid index of the master for which SYNC/OFS message should be transmitted
 *  \pre        -
 *  \unit       SyncTx
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Tx_TransmitSyncRequest(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_OnSuccessTransmitSyncRequest
 *********************************************************************************************************************/
/*! \brief      Executed after successfully transmission of SYNC/OFS message.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       SyncTx
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_OnSuccessTransmitSyncRequest(
    FrTSyn_MasterConfigIterType masterIdx);
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: CurrentTime
 *********************************************************************************************************************/
/*! \brief      This unit handles the time retrieval on the sending side.
 *  \details    -
 *  \pre        -
 *  \trace      CURRENTTIME_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Tx_GetTimeSync
 *********************************************************************************************************************/
/*! \brief      Get the current time for SYNC/OFS message.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \return     E_OK                OFS/SYNC time retrieval was successful
 *  \return     E_NOT_OK            OFS/SYNC time retrieval was not successful
 *  \pre        -
 *  \unit       CurrentTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_GetTimeSync(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_OnGetTimeSync
 *********************************************************************************************************************/
/*! \brief      Get time for SYNC message
 *  \details    Gets the time from StbM and calculates time to next FR cycle 0 for SYNC message.
 *  \param[in]  masterIdx           Valid index of master
 *  \return     E_OK                Time retrieval was successful
 *  \return     E_NOT_OK            Time retrieval was not successful
 *  \pre        -
 *  \unit       CurrentTime
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_OnGetTimeSync(
    FrTSyn_MasterConfigIterType masterIdx);

# if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_OnGetTimeOfs
 *********************************************************************************************************************/
/*! \brief      Get the time from StbM for OFS message.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \return     E_OK                Time retrieval was successful
 *  \return     E_NOT_OK            Time retrieval was not successful
 *  \pre        -
 *  \unit       CurrentTime
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_OnGetTimeOfs(
    FrTSyn_MasterConfigIterType masterIdx);
# endif /* FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON */

/**********************************************************************************************************************
 *  FrTSyn_Tx_CalculateTimeToNextCycle0
 *********************************************************************************************************************/
/*! \brief      Converts number of FR ticks to timestamp
 *  \details    Converts the given number of FR macroticks with the given macrotick duration to a timestamp.
 *  \param[in]  masterIdx           Valid index of master
 *  \param[in]  frIfMacroticks      Actual value of Macroticks retrieved by FR interface
 *  \param[out] timeDeltaPtr        Pointer to timestamp containing the time delta to the next FR cycle 0
 *  \pre        Max value of macroticks is 1024000, max value of macrotickDuration is 6000ns, max value of resulting
                time is 1.024s
 *  \unit       CurrentTime
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_CalculateTimeToNextCycle0(FrTSyn_MasterConfigIterType masterIdx,
    uint16 frIfMacroticks,
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_DATA) localTimeT0Vlt,
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_DATA) localTimeT1Vlt,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timeDeltaPtr);
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: AssembleMsg
 *********************************************************************************************************************/
/*! \brief      This unit handles the assembly of the SYNC, OFS messages.
 *  \details    -
 *  \pre        -
 *  \trace      ASSEMBLEMSG_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Tx_AssembleSyncMsg
 *********************************************************************************************************************/
/*! \brief      Assemble SYNC message
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \param[out] syncMsg             FR message to write to
 *  \pre        -
 *  \unit       AssembleMsg
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_AssembleSyncMsg(FrTSyn_MasterConfigIterType masterIdx,
    P2VAR(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_VAR) syncMsg);

/**********************************************************************************************************************
 *  FrTSyn_Tx_OnAssembleSync
 *********************************************************************************************************************/
/*! \brief      Assemble common parts of SYNC/OFS message
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \param[out] syncMsg             FR message to write to
 *  \pre        -
 *  \unit       AssembleMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_OnAssembleSync(FrTSyn_MasterConfigIterType masterIdx,
    P2VAR(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_VAR) syncMsg);

# if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_OnAssembleOfs
 *********************************************************************************************************************/
/*! \brief      Assemble message using OFS format.
 *  \details    -
 *  \param[out] ofsMsg              FR message to write to
 *  \pre        Need to call FrTSyn_Tx_OnAssembleSync before calling this function
 *  \unit       AssembleMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_OnAssembleOfs(
    P2VAR(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_VAR) ofsMsg);
# endif /* FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON */

# if ((FRTSYN_EXISTS_SYNC_TXCRCSECUREDOFMASTERCONFIG == STD_ON) || (FRTSYN_EXISTS_OFS_TXCRCSECUREDOFMASTERCONFIG == STD_ON))
/**********************************************************************************************************************
 *  FrTSyn_Tx_SetUserByte2ToSync
 *********************************************************************************************************************/
/*! \brief      Set user byte 2 for SYNC/OFS message without CRC
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \param[out] syncMsg             FR message to write to
 *  \pre        -
 *  \unit       AssembleMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_SetUserByte2ToSync(FrTSyn_MasterConfigIterType masterIdx,
    P2VAR(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_VAR) syncMsg);
# endif /* (FRTSYN_EXISTS_SYNC_TXCRCSECUREDOFMASTERCONFIG == STD_ON) || (FRTSYN_EXISTS_OFS_TXCRCSECUREDOFMASTERCONFIG == STD_ON) */

# if ((FRTSYN_EXISTS_SYNC_CRC_TXCRCSECUREDOFMASTERCONFIG == STD_ON) || (FRTSYN_EXISTS_OFS_CRC_TXCRCSECUREDOFMASTERCONFIG == STD_ON))
/**********************************************************************************************************************
 *  FrTSyn_Tx_SetCrcToSync
 *********************************************************************************************************************/
/*! \brief      Set the CRC to the SYNC/OFS message
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \param[out] syncMsg             FR message to write to
 *  \pre        -
 *  \unit       AssembleMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Tx_SetCrcToSync(FrTSyn_MasterConfigIterType masterIdx,
    P2VAR(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_VAR) syncMsg);
# endif /* (FRTSYN_EXISTS_SYNC_CRC_TXCRCSECUREDOFMASTERCONFIG == STD_ON) || (FRTSYN_EXISTS_OFS_CRC_TXCRCSECUREDOFMASTERCONFIG == STD_ON) */
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: Validation
 *********************************************************************************************************************/
/*! \brief      This unit handles the validation of the SYNC and OFS messages.
 *  \details    -
 *  \pre        -
 *  \trace      VALIDATION_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Tx_ValidateSendSyncConditions
 *********************************************************************************************************************/
/*! \brief      Validates if a SYNC and OFS message can be sent
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \return     TRUE                All send conditions for sending a SYNC/OFS message are true
 *  \return     FALSE               At least one send condition is not fulfilled
 *  \pre        -
 *  \unit       Validation
 *********************************************************************************************************************/
FRTSYN_LOCAL FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_ValidateSendSyncConditions(
    FrTSyn_MasterConfigIterType masterIdx);

# if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_ValidateImmediateSync
 *********************************************************************************************************************/
/*! \brief      Validate if the immediate send SYNC has to be triggered.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \return     TRUE                SYNC message must be sent in immediate mode
 *  \return     FALSE               Immediate send mode conditions are not fulfilled
 *  \pre        -
 *  \unit       Validation
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_ValidateImmediateSync(FrTSyn_MasterConfigIterType masterIdx);
# endif /* FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON */

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsTxReady
 *********************************************************************************************************************/
/*! \brief      Checks if sending is possible.
 *  \details    -
 *  \param[in]  masterIdx           Valid master index
 *  \return     TRUE                Tx is ready to be used for sending
 *  \return     FALSE               Tx is not ready
 *  \pre        -
 *  \unit       Validation
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsTxReady(FrTSyn_MasterConfigIterType masterIdx);
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: TxPeriodTime
 *********************************************************************************************************************/
/*! \brief      This unit handles the TxPeriodCounter.
 *  \details    -
 *  \pre        -
 *  \trace      TXPERIODTIME_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Tx_StopTxPeriodCounter
 *********************************************************************************************************************/
/*! \brief      Stop the Tx period counter.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       TxPeriodTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_StopTxPeriodCounter(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_ResetTxPeriodCounter
 *********************************************************************************************************************/
/*! \brief      Reset the Tx period counter.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       TxPeriodTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ResetTxPeriodCounter(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateTxPeriodCounter
 *********************************************************************************************************************/
/*! \brief      Update the Tx period counter.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       TxPeriodTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateTxPeriodCounter(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsTxPeriodTimeExpired
 *********************************************************************************************************************/
/*! \brief      Check if Tx period time is expired.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       TxPeriodTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsTxPeriodTimeExpired(
    FrTSyn_MasterConfigIterType masterIdx);
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: TxSequenceCounter
 *********************************************************************************************************************/
/*! \brief      This unit handles the TxSeqenceCounter.
 *  \details    -
 *  \pre        -
 *  \trace      TXSEQUENCECOUNTER_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Tx_InitTxSequenceCounter
 *********************************************************************************************************************/
/*! \brief      Initialize the Tx sequence counter to the maximum value.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       TxSequenceCounter
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitTxSequenceCounter(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_IncrementTxSequenceCounter
 *********************************************************************************************************************/
/*! \brief      Increment sequence counter
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       TxSequenceCounter
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_IncrementTxSequenceCounter(
    FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  Unit: CyclicMsgResumeTime
 *********************************************************************************************************************/
/*! \brief      This unit handles the cyclic message resume counter.
 *  \details    -
 *  \pre        -
 *  \trace      CYCLICMSGRESUMETIME_UNIT
 *********************************************************************************************************************/
# if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_InitCyclicMsgResumeCounter
 *********************************************************************************************************************/
/*! \brief      Initialize the cyclic message resume counter.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       CyclicMsgResumeTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitCyclicMsgResumeCounter(
    FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_ResetCyclicMsgResumeCounter
 *********************************************************************************************************************/
/*! \brief      Reset cyclic message resume counter to configured value.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       CyclicMsgResumeTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ResetCyclicMsgResumeCounter(
    FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateCyclicMsgResumeCounter
 *********************************************************************************************************************/
/*! \brief      Update the cyclic message resume counter.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       CyclicMsgResumeTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateCyclicMsgResumeCounter(
    FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsCyclicMsgResumeTimeExpired
 *********************************************************************************************************************/
/*! \brief      Check if cyclic message resume time is expired.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \return     TRUE                Cyclic message resume time is expired
 *  \return     FALSE               Cyclic message resume time is not expired
 *  \pre        -
 *  \unit       CyclicMsgResumeTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsCyclicMsgResumeTimeExpired(
    FrTSyn_MasterConfigIterType masterIdx);
# endif /* FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON */
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: TriggerTransmitTimeout
 *********************************************************************************************************************/
/*! \brief      This unit handles the trigger transmit timeout counter.
 *  \details    -
 *  \pre        -
 *  \trace      TRIGGERTRANSMITTIMEOUT_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Tx_InitTriggerTransmitTimeout
 *********************************************************************************************************************/
/*! \brief      Initialize the Tx trigger transmit timeout.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       TriggerTransmitTimeout
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitTriggerTransmitTimeout(
    FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_ResetTriggerTransmitTimeout
 *********************************************************************************************************************/
/*! \brief      Reset the Tx trigger transmit timeout to the configured value.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       TriggerTransmitTimeout
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ResetTriggerTransmitTimeout(
    FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateTriggerTransmitTimeout
 *********************************************************************************************************************/
/*! \brief      Update the Tx trigger transmit timeout.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       TriggerTransmitTimeout
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateTriggerTransmitTimeout(
    FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsTriggerTransmitTimeoutExpired
 *********************************************************************************************************************/
/*! \brief      Check if Tx trigger transmit timeout is expired.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \return     TRUE                Tx trigger transmit timeout is expired
 *  \return     FALSE               Tx trigger transmit timeout is not expired
 *  \pre        -
 *  \unit       TriggerTransmitTimeout
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsTriggerTransmitTimeoutExpired(
    FrTSyn_MasterConfigIterType masterIdx);
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: DebounceTime
 *********************************************************************************************************************/
/*! \brief      This unit handles the Tx debounce counter.
 *  \details    -
 *  \pre        -
 *  \trace      DEBOUNCETIME_UNIT
 *********************************************************************************************************************/
# if (FRTSYN_TXDEBOUNCECOUNTERIDXOFMASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_InitDebounceCounter
 *********************************************************************************************************************/
/*! \brief      Initialization of Tx debouncing
 *  \details    -
 *  \pre        -
 *  \unit       DebounceTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitDebounceCounter(void);

/**********************************************************************************************************************
 *  FrTSyn_Tx_ResetDebounceCounter
 *********************************************************************************************************************/
/*! \brief      Reset Tx debounce counter back to configured value.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       DebounceTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ResetDebounceCounter(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateDebounceCounter
 *********************************************************************************************************************/
/*! \brief      Update Tx debouncing
 *  \details    Configured Tx debounce counters are decremented.
 *  \pre        -
 *  \unit       DebounceTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateDebounceCounter(void);

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsDebounceTimeExpired
 *********************************************************************************************************************/
/*! \brief      Check if Tx debounce time is expired.
 *  \details    Check if the Tx debounce counter is zero for the given master.
 *  \param[in]  masterIdx           Valid index of master
 *  \return     TRUE                Tx is debounce time is expired
 *  \return     FALSE               Tx debounce time is not expired
 *  \pre        -
 *  \unit       DebounceTime
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsDebounceTimeExpired(
    FrTSyn_MasterConfigIterType masterIdx);

#  if (FRTSYN_TX_CONF_MIXED_DEBOUNCE_COUNTER == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_HasDebouncing
 *********************************************************************************************************************/
/*! \brief      Check if the given master has Tx debouncing configured.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        -
 *  \unit       DebounceTime
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_HasDebouncing(FrTSyn_MasterConfigIterType masterIdx);
#  endif /* FRTSYN_TX_CONF_MIXED_DEBOUNCE_COUNTER == STD_ON */
# endif /* FRTSYN_TXDEBOUNCECOUNTERIDXOFMASTERCONFIG == STD_ON */
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: PduIdLock
 *********************************************************************************************************************/
/*! \brief      This unit handles the PduIdLock mechanism.
 *  \details    -
 *  \pre        -
 *  \trace      PDUIDLOCK_UNIT
 *********************************************************************************************************************/
# if (FRTSYN_EXISTS_SHARED_PDUS == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_InitPduIdStates
 *********************************************************************************************************************/
/*! \brief      Initialization of PduId state table.
 *  \details    -
 *  \pre        -
 *  \unit       PduIdLock
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitPduIdStates(void);

/**********************************************************************************************************************
 *  FrTSyn_Tx_LockPduId
 *********************************************************************************************************************/
/*! \brief      Lock the PduId for the given master.
 *  \details    No other master can use the configured PduId after this call.
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        Check if PduId is already locked by calling FrTSyn_Tx_IsPduIdUnLocked
 *  \unit       PduIdLock
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_LockPduId(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_ReleasePduId
 *********************************************************************************************************************/
/*! \brief      Release the PduId for the given master.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \pre        Check if PduId is already locked by calling FrTSyn_Tx_IsPduIdUnLocked
 *  \unit       PduIdLock
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ReleasePduId(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsPduIdUnLocked
 *********************************************************************************************************************/
/*! \brief      Check if the configured PduId is unlocked.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \return     TRUE                Pdu is not locked from another master
 *  \return     FALSE               Pdu is locked from another master
 *  \pre        -
 *  \unit       PduIdLock
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsPduIdUnLocked(FrTSyn_MasterConfigIterType masterIdx);
# endif /* FRTSYN_EXISTS_SHARED_PDUS == STD_ON */
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

/**********************************************************************************************************************
 *  Unit: FrController
 *********************************************************************************************************************/
/*! \brief      This unit handles the FR controller communication.
 *  \details    -
 *  \pre        -
 *  \trace      FRCONTROLLER_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_InitFrController
 *********************************************************************************************************************/
/*! \brief      Initialization of FR controller.
 *  \details    -
 *  \pre        -
 *  \unit       FrController
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_InitFrController(void);

/**********************************************************************************************************************
 *  FrTSyn_IsFrIfOnline
 *********************************************************************************************************************/
/*! \brief      Checks if the FR interface is online.
 *  \details    -
 *  \param[in]  frControllerIdx     Valid FR controller index
 *  \return     TRUE                FR interface is in state FRIF_STATE_ONLINE
 *  \return     FALSE               FR interface is offline
 *  \pre        -
 *  \unit       FrController
 *********************************************************************************************************************/
FRTSYN_LOCAL FUNC(boolean, FRTSYN_CODE) FrTSyn_IsFrIfOnline(FrTSyn_ControllerStateIterType frControllerIdx);

#if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_IsFrControllerReady
 *********************************************************************************************************************/
/*! \brief      Checks if the configured FR controller is online.
 *  \details    -
 *  \param[in]  masterIdx           Valid index of master
 *  \return     TRUE                FR controller is ready to use
 *  \return     FALSE               FR controller is not ready
 *  \pre        -
 *  \unit       FrController
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_IsFrControllerReady(FrTSyn_MasterConfigIterType masterIdx);

/**********************************************************************************************************************
 *  FrTSyn_SetFrControllerTransmissionMode
 *********************************************************************************************************************/
/*! \brief      Set the transmission mode
 *  \details    -
 *  \param[in]  ctrlIdx             FR controller index
 *  \param[in]  mode                Valid values are FRTSYN_TX_OFF or FRTSYN_TX_ON
 *  \return     TRUE                The controller index is valid
 *  \return     FALSE               The controller index is invalid
 *  \pre        -
 *  \unit       FrController
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_SetFrControllerTransmissionMode(uint8 ctrlIdx,
    FrTSyn_TransmissionModeType mode);
#endif /* FRTSYN_MASTERCONFIG == STD_ON */

/**********************************************************************************************************************
 *
 *
 *  SLAVE
 *
 *
 *********************************************************************************************************************/

#if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: Receive
 *********************************************************************************************************************/
/*! \brief      This unit provides the slave receive functionality of the FrTSyn.
 *  \details    -
 *  \pre        -
 *  \trace      RECEIVE_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Rx_Init
 *********************************************************************************************************************/
/*! \brief      Initialization of receive unit
 *  \details    -
 *  \pre        -
 *  \unit       Receive
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_Init(void);

/**********************************************************************************************************************
 *  FrTSyn_Rx_Update
 *********************************************************************************************************************/
/*! \brief      Update receive (Rx) unit
 *  \details    -
 *  \pre        -
 *  \unit       Receive
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_Update(void);

/**********************************************************************************************************************
 *  FrTSyn_Rx_FindSlave
 *********************************************************************************************************************/
/*! \brief      Find the slave where domainId and PduId match.
 *  \details    -
 *  \param[in]  pduId                     Received PduId
 *  \param[in]  domainId                  Received domainId
 *  \param[out] outSlaveIdx               Found slave if return value is FRTSYN_E_NO_ERROR
 *  \return     FRTSYN_E_NO_ERROR         Slave was found
 *  \return     FRTSYN_INV_DOMAIN_IDX     DomainId was not valid
 *  \return     FRTSYN_E_INVALID_PDUID    PduId was not valid
 *  \pre        -
 *  \unit       Receive
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(uint8, FRTSYN_CODE) FrTSyn_Rx_FindSlave(PduIdType pduId, uint8 domainId,
    P2VAR(FrTSyn_SlaveConfigIterType, AUTOMATIC, FRTSYN_APPL_VAR) outSlaveIdx);

/**********************************************************************************************************************
 *  FrTSyn_Rx_GetDomainId
 *********************************************************************************************************************/
/*! \brief      Get the domainId of received message
 *  \details    Get the domainId stored in the message info considering also offset time domain calculation.
 *  \param[in]  ptrMsgInfo          Pointer to received message
 *  \return     domainId            Valid domain index also considering whether domain is an offset or SYNC time domain
 *  \pre        -
 *  \unit       Receive
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(uint8, FRTSYN_CODE) FrTSyn_Rx_GetDomainId(
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo);
#endif /* FRTSYN_SLAVECONFIG == STD_ON */

#if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: ProtocolRx
 *********************************************************************************************************************/
/*! \brief      This unit handles the protocol on the receiving side.
 *  \details    -
 *  \pre        -
 *  \trace      PROTOCOLRX_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Rx_ProcessRxIndication
 *********************************************************************************************************************/
/*! \brief      Dispatches the Rx Indication based on the state of the slave.
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \param[in]  ptrMsgInfo          Pointer to received message
 *  \pre        -
 *  \unit       ProtocolRx
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_ProcessRxIndication(FrTSyn_SlaveConfigIterType slaveIdx,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo);

/**********************************************************************************************************************
 *  FrTSyn_Rx_UpdateSlaves
 *********************************************************************************************************************/
/*! \brief      Update configured slaves
 *  \details    -
 *  \pre        -
 *  \unit       ProtocolRx
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_UpdateSlaves(void);
#endif /* FRTSYN_SLAVECONFIG == STD_ON */

#if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: SyncRx
 *********************************************************************************************************************/
/*! \brief      This unit handles the SYNC message mechanism on the receiving side.
 *  \details    -
 *  \pre        -
 *  \trace      SYNCRX_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Rx_ProcessReceivedSyncMsg
 *********************************************************************************************************************/
/*! \brief      Processes received SYNC/OFS messages
 *  \details    Retrieves reception time and payload of OFS/SYNC message.
 *  \param[in]  slaveIdx            Valid slave index
 *  \pre        Time slave is in state FRTSYN_STATE_RECEIVE_SYNC_RECEIVED
 *  \unit       SyncRx
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_ProcessReceivedSyncMsg(FrTSyn_SlaveConfigIterType slaveIdx);

/**********************************************************************************************************************
 *  FrTSyn_Rx_OnReceiveSyncMsg
 *********************************************************************************************************************/
/*! \brief      Receive SYNC message
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \pre        -
 *  \unit       SyncRx
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnReceiveSyncMsg(FrTSyn_SlaveConfigIterType slaveIdx);

# if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Rx_OnReceiveOfsMsg
 *********************************************************************************************************************/
/*! \brief      Receive OFS message
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \pre        -
 *  \unit       SyncRx
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnReceiveOfsMsg(FrTSyn_SlaveConfigIterType slaveIdx);
# endif /* FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON */

/**********************************************************************************************************************
 *  FrTSyn_Rx_OnSuccessSyncReceive
 *********************************************************************************************************************/
/*! \brief      Perform actions after successfully received SYNC message.
 *  \details    Sets slave to the next state.
 *  \param[in]  slaveIdx            Valid slave index
 *  \pre        -
 *  \unit       SyncRx
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnSuccessSyncReceive(FrTSyn_SlaveConfigIterType slaveIdx);

/**********************************************************************************************************************
 *  FrTSyn_Rx_CalculateNewGlobalTime
 *********************************************************************************************************************/
/*! \brief      Calculates new global time value
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \param[in]  syncTimeStamp       Retrieved timestamp
 *  \param[out] newGlobalTime       Calculated new global time value, part of tuple (T1)
 *  \param[out] localTimeT1Vlt      Retrieved current virtual local time, part of tuple (T1Vlt)
 *  \pre        -
 *  \unit       SyncRx
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Rx_CalculateNewGlobalTime(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) syncTimeStamp,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) newGlobalTime,
    P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_VAR) localTimeT1Vlt);

/**********************************************************************************************************************
 *  FrTSyn_Rx_OnCorrectNewGlobalTime
 *********************************************************************************************************************/
/*! \brief          Corrects new global time
 *  \details        Corrects new global time if current cycle is greater or equal than retrieved FR cycle counter.
 *  \param[in]      slaveIdx        Valid slave index
 *  \param[in,out]  newGlobalTime   New global time value
 *  \pre        -
 *  \unit       SyncRx
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnCorrectNewGlobalTime(FrTSyn_SlaveConfigIterType slaveIdx,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) newGlobalTime);
#endif /* FRTSYN_SLAVECONFIG == STD_ON */

#if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: DisassembleMsg
 *********************************************************************************************************************/
/*! \brief      This unit handles the disassembly of the SYNC, OFS messages.
 *  \details    -
 *  \pre        -
 *  \trace      DISASSEMBLEMSG_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Rx_GetPayload
 *********************************************************************************************************************/
/*! \brief      Retrieve the payload from a SYNC message.
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \param[out] timeStamp           Retrieved timestamp
 *  \param[out] userData            Retrieved user data
 *  \pre        -
 *  \unit       DisassembleMsg
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_GetPayload(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timeStamp,
    P2VAR(StbM_UserDataType, AUTOMATIC, FRTSYN_APPL_VAR) userData);

# if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Rx_GetOffsetPayload
 *********************************************************************************************************************/
/*! \brief      Retrieve the payload from an Offset message.
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \param[out] timeStamp           Retrieved timestamp
 *  \pre        -
 *  \unit       DisassembleMsg
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_GetOffsetPayload(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timeStamp);
# endif /* FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON */

/**********************************************************************************************************************
 *  FrTSyn_Rx_OnGetTimeStamp
 *********************************************************************************************************************/
/*! \brief      Retrieve the time stamp from the payload of SYNC message.
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \param[out] timeStamp           Retrieved timestamp
 *  \pre        -
 *  \unit       DisassembleMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnGetTimeStamp(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timeStamp);

# if ((FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON))
/**********************************************************************************************************************
 *  FrTSyn_Rx_OnGetUserDataCrc
 *********************************************************************************************************************/
/*! \brief      Retrieve the user data with CRC from payload of SYNC message.
 *  \details    -
 *  \param[out] userData            Retrieved user data
 *  \pre        -
 *  \unit       DisassembleMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnGetUserDataCrc(
    P2VAR(StbM_UserDataType, AUTOMATIC, FRTSYN_APPL_VAR) userData);
# endif /* ((FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)) */

# if ((FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON))
/**********************************************************************************************************************
 *  FrTSyn_Rx_OnGetUserData
 *********************************************************************************************************************/
/*! \brief      Retrieve the user data without CRC from payload of SYNC message.
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \param[out] userData            Retrieved user data
 *  \pre        -
 *  \unit       DisassembleMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnGetUserData(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2VAR(StbM_UserDataType, AUTOMATIC, FRTSYN_APPL_VAR) userData);
# endif /* ((FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)) */
#endif /* FRTSYN_SLAVECONFIG == STD_ON */

#if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: ValidationMsg
 *********************************************************************************************************************/
/*! \brief      This unit handles the validation of the received messages.
 *  \details    -
 *  \pre        -
 *  \trace      VALIDATIONMSG_UNIT
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  FrTSyn_Rx_ValidateSync
 *********************************************************************************************************************/
/*! \brief      Validate the SYNC/OFS message
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \param[in]  ptrMsgInfo          Pointer to received message
 *  \return     TRUE                All conditions for received SYNC message are valid
 *  \return     FALSE               At least one the validation check failed
 *  \pre        -
 *  \unit       ValidationMsg
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_ValidateSync(FrTSyn_SlaveConfigIterType slaveIdx,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo);

/**********************************************************************************************************************
 *  FrTSyn_Rx_ValidateSyncMessageType
 *********************************************************************************************************************/
/*! \brief      Validate the message type
 *  \details    Validate if the message type is either SYNC or OFS, and if it is CRC secured or not secured.
 *  \param[in]  ptrMsgInfo          Pointer to received message
 *  \return     TRUE                Message type fits any of the defined message types
 *  \return     FALSE               No corresponding message type was found
 *  \pre        -
 *  \unit       ValidationMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_ValidateSyncMessageType(
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo);

/**********************************************************************************************************************
 *  FrTSyn_Rx_HasRefTimebaseTimeout
 *********************************************************************************************************************/
/*! \brief      Check if timeout occurred at referred timebase.
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \return     TRUE                At referred timebase a timeout occurred
 *  \return     FALSE               No timeout or retrieval of timebase status was not working
 *  \pre        -
 *  \unit       ValidationMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_HasRefTimebaseTimeout(
    FrTSyn_SlaveConfigIterType slaveIdx);

/**********************************************************************************************************************
 *  FrTSyn_Rx_ValidateSequenceCounterSync
 *********************************************************************************************************************/
/*! \brief      Validate the sequence counter in the SYNC/OFS message.
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \param[in]  ptrMsgInfo          Pointer to received message
 *  \return     TRUE                The received sequence counter is valid
 *  \return     FALSE               The received sequence counter is invalid
 *  \pre        -
 *  \unit       ValidationMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_ValidateSequenceCounterSync(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo);

/**********************************************************************************************************************
 *  FrTSyn_Rx_CalculateSequenceCounterJumpWidth
 *********************************************************************************************************************/
/*! \brief      Calculate the jump with between last received sequence counter and given one.
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \param[in]  sequenceCounter     Received sequence counter
 *  \return     jumpWidth           Calculated jump width
 *  \pre        -
 *  \unit       ValidationMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(uint8, FRTSYN_CODE) FrTSyn_Rx_CalculateSequenceCounterJumpWidth(
    FrTSyn_SlaveConfigIterType slaveIdx,
    uint8 sequenceCounter);

# if ((FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON))
/**********************************************************************************************************************
 *  FrTSyn_Rx_ValidateCrcSync
 *********************************************************************************************************************/
/*! \brief      Validate the CRC in the SYNC message.
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \param[in]  ptrMsgInfo          Pointer to received message
 *  \return     TRUE                CRC is valid
 *  \return     FALSE               CRC is invalid
 *  \pre        -
 *  \unit       ValidationMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_ValidateCrcSync(FrTSyn_SlaveConfigIterType slaveIdx,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo);
# endif /* ((FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)) */

# if (FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Rx_OnValidateSyncCrcNotValidated
 *********************************************************************************************************************/
/*! \brief      Validate the CRC in SYNC message for CRC-NOT_VALIDATED configured slave.
 *  \details    -
 *  \param[in]  ptrMsgInfo          Pointer to received message
 *  \return     TRUE                Message type has no CRC
 *  \return     FALSE               Message type has CRC
 *  \pre        -
 *  \unit       ValidationMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_OnValidateSyncCrcNotValidated(
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo);
# endif /* FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON */

# if (FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Rx_OnValidateSyncCrcValidated
 *********************************************************************************************************************/
/*! \brief      Validate the CRC in SYNC message for CRC-VALIDATED configured slave.
 *  \details    -
 *  \param[in]  slaveIdx            Valid slave index
 *  \param[in]  ptrMsgInfo          Pointer to received message
 *  \return     TRUE                Message has CRC set and the CRC is valid
 *  \return     FALSE               Message does not contain CRC or CRC value check failed
 *  \pre        -
 *  \unit       ValidationMsg
 *  \visibility Private
 *********************************************************************************************************************/
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_OnValidateSyncCrcValidated(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo);
# endif /* FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON */
#endif /* FRTSYN_SLAVECONFIG == STD_ON */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#ifndef FRTSYN_NOUNIT_UTILITY /* COV_FRTSYN_NOUNIT_SWITCH */
/**********************************************************************************************************************
 *  Unit: Utility
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_TimestampPlusTimestamp
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TimestampPlusTimestamp(
    P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp1Ptr,
    P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp2Ptr,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timesumPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Add the two timestamps */
  timesumPtr->nanoseconds = timestamp1Ptr->nanoseconds + timestamp2Ptr->nanoseconds; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  timesumPtr->seconds = timestamp1Ptr->seconds + timestamp2Ptr->seconds; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  timesumPtr->secondsHi = timestamp1Ptr->secondsHi + timestamp2Ptr->secondsHi; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */

  /* #20 Verify that no overflow of nanoseconds occurred */
  /* no real overflow of nanoseconds possible, since max value is 10^9 - 1 */
  if (timesumPtr->nanoseconds > FRTSYN_TIMESTAMP_MAX_NANOSECONDS)
  {
    timesumPtr->seconds++; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
    timesumPtr->nanoseconds -= FRTSYN_NANOSECONDS_IN_ONE_SECOND; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  }

  /* #30 Verify that no overflow of seconds occurred */
  if (timesumPtr->seconds < (timestamp1Ptr->seconds | timestamp2Ptr->seconds))
  {
    timesumPtr->secondsHi++; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  }

  /* #40 Verify that no overflow of seconds high occurred */
  /* calculated sum is invalid, if overflow of seconds high occurred */
  if ((timesumPtr->secondsHi >= timestamp1Ptr->secondsHi) && (timesumPtr->secondsHi >= timestamp2Ptr->secondsHi))
  {
    retVal = E_OK;
  }

  return retVal;
} /* FrTSyn_TimestampPlusTimestamp */

# if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_TimestampMinusTimestamp
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
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_TimestampMinusTimestamp(
    P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp1Ptr,
    P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) timestamp2Ptr,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timediffPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate time difference */
  timediffPtr->secondsHi = timestamp1Ptr->secondsHi - timestamp2Ptr->secondsHi; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  timediffPtr->seconds = timestamp1Ptr->seconds - timestamp2Ptr->seconds; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */

  /* #20 Check for overflow of seconds and correct seconds high */
  if (timestamp1Ptr->seconds < timestamp2Ptr->seconds)
  {
    timediffPtr->secondsHi--; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  }

  /* #30 Check for overflow of nanoseconds and correct seconds */
  if (timestamp1Ptr->nanoseconds < timestamp2Ptr->nanoseconds)
  {
    timediffPtr->seconds--; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */

    /* #40 Check for overflow of seconds and correct seconds high */
    if (timediffPtr->seconds == FRTSYN_TIMESTAMP_MAX_SECONDS)
    {
      timediffPtr->secondsHi--; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
    }

    /* #50 Calculate nanoseconds after overflow */
    timediffPtr->nanoseconds = (FRTSYN_TIMESTAMP_MAX_NANOSECONDS - timestamp2Ptr->nanoseconds) + (1u + timestamp1Ptr->nanoseconds); /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  }
  else
  {
    /* #60 Calculate nanoseconds */
    timediffPtr->nanoseconds = timestamp1Ptr->nanoseconds - timestamp2Ptr->nanoseconds; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  }
} /* FrTSyn_TimestampMinusTimestamp */
# endif /* FRTSYN_SLAVECONFIG == STD_ON */

# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_SubtractVirtualLocalTime
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_SubtractVirtualLocalTime(
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_DATA) lhs,
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_DATA) rhs,
    P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_VAR) timediffPtr)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If left time is greater than or equal to right time: */
  if ((lhs->nanosecondsHi > rhs->nanosecondsHi) || ((lhs->nanosecondsHi == rhs->nanosecondsHi) && (lhs->nanosecondsLo >= rhs->nanosecondsLo)))
  {
    /* #20 Calculate time difference */
    timediffPtr->nanosecondsHi = lhs->nanosecondsHi - rhs->nanosecondsHi; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
    timediffPtr->nanosecondsLo = lhs->nanosecondsLo - rhs->nanosecondsLo; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */

    /* #30 Check for underflow of nanoseconds low */
    if (lhs->nanosecondsLo < rhs->nanosecondsLo)
    {
      timediffPtr->nanosecondsHi--; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
    }

    retVal = E_OK;
  }
  return retVal;
} /* FrTSyn_SubtractVirtualLocalTime */
# endif /* FRTSYN_MASTERECONFIG == STD_ON */

/**********************************************************************************************************************
 *  FrTSyn_ConvertTimespanToTimestamp
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_ConvertTimespanToTimestamp(
    uint32 timespan,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timestampPtr)
{
  /* #10 Assign given time span to timestamp in nanoseconds */
  /* Max length of one cycle is 0.016s, max number of cycles is 64 --> max resulting time is 0.016s * 64 = 1.024s */
  timestampPtr->secondsHi = 0u; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  timestampPtr->seconds = 0u; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  timestampPtr->nanoseconds = timespan; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */

  /* #20 If the resulting time exceeds the range of nanoseconds */
  if (timestampPtr->nanoseconds > FRTSYN_TIMESTAMP_MAX_NANOSECONDS)
  {
    /* #30 Correct seconds and nanoseconds value */
    timestampPtr->seconds = 1u; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
    timestampPtr->nanoseconds -= FRTSYN_NANOSECONDS_IN_ONE_SECOND; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  }
} /* FrTSyn_ConvertTimespanToTimestamp */

# if (FRTSYN_CRC_USED == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_CalculateMsgCrc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL FUNC(uint8, FRTSYN_CODE) FrTSyn_CalculateMsgCrc(
    P2CONST(uint8, AUTOMATIC, FRTSYN_APPL_DATA) ptrDataId,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 crcResult;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate CRC over byte 2-15 */
  crcResult = Crc_CalculateCRC8H2F(&((*ptrMsgInfo)[2]), 14u, FRTSYN_CRC_START_VALUE, TRUE); /* SBSW_FRTSYN_ACCESS_FRMSG */ /* VCA_FRTSYN_CALL_EXTERNAL_FUNCTION_WITH_CONST_POINTER_ARGUMENT */

  /* #20 Calculate CRC with dataId */
  crcResult = Crc_CalculateCRC8H2F(ptrDataId, 1u, crcResult, FALSE); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */ /* VCA_FRTSYN_CALL_EXTERNAL_FUNCTION_WITH_CONST_POINTER_ARGUMENT */

  return crcResult;
} /* FrTSyn_CalculateMsgCrc */
# endif /* FRTSYN_CRC_USED == STD_ON */

# if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_IsOffsetDomain
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_IsOffsetDomain(FrTSyn_DomainConfigIterType domainIdx)
{
  /* #10 Check if the given domain is an offset domain */
  return (boolean) (FrTSyn_GetOffsetDomainOfDomainConfig(domainIdx) == FRTSYN_OFFSET_OFFSETDOMAINOFDOMAINCONFIG); /* PRQA S 1881, 4304 */ /* MD_MSR_AutosarBoolean */
} /* FrTSyn_IsOffsetDomain */
# endif /* FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_UTILITY */

/**********************************************************************************************************************
 *
 *
 *  MASTER
 *
 *
 *********************************************************************************************************************/

#ifndef FRTSYN_NOUNIT_TRANSMIT /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: Transmit
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_Init(void)
{
  /* #10 Initialize masters */
  FrTSyn_Tx_InitMasters();

#  if (FRTSYN_EXISTS_SHARED_PDUS == STD_ON)
  /* #20 Initialize PduId states */
  FrTSyn_Tx_InitPduIdStates();
#  endif

  /* #30 If debouncing is configured: */
#  if (FRTSYN_TXDEBOUNCECOUNTERIDXOFMASTERCONFIG == STD_ON)
  /* #40 Initialize debouncing */
  FrTSyn_Tx_InitDebounceCounter();
#  endif
} /* FrTSyn_Tx_Init */

/**********************************************************************************************************************
 *  FrTSyn_Tx_Update
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_Update(void)
{
  /* #10 Update masters */
  FrTSyn_Tx_UpdateMasters();

#  if (FRTSYN_TXDEBOUNCECOUNTERIDXOFMASTERCONFIG == STD_ON)
  /* #20 Update Tx debouncing, if configured */
  FrTSyn_Tx_UpdateDebounceCounter();
#  endif
} /* FrTSyn_Tx_Update */

/**********************************************************************************************************************
*  FrTSyn_Tx_ProcessTriggerTransmit
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_ProcessTriggerTransmit(PduIdType pduId,
    P2VAR(PduInfoType, AUTOMATIC, FRTSYN_APPL_VAR) pduInfoPtr,
    P2VAR(uint8, AUTOMATIC, FRTSYN_APPL_VAR) outErrorId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  FrTSyn_MasterConfigIterType masterIdx;
  *outErrorId = FRTSYN_E_INVALID_PDUID; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each configured master: */
  for (masterIdx = 0u; masterIdx < FrTSyn_GetSizeOfMasterConfig(); masterIdx++)
  {
    /* #20 Find master with provided PduId */
    if (FrTSyn_GetMasterHandleIdOfMasterConfig(masterIdx) == pduId)
    {
      *outErrorId = FRTSYN_E_NO_ERROR; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
      /* #30 Check that master is in state 'wait for SYNC trigger transmit' */
      if (FrTSyn_GetStateOfMasterState(masterIdx) == FRTSYN_STATE_SEND_WAITING_FOR_SYNC_TRIGGER_TRANSMIT)
      {
        /* #40 Handle SYNC/OFS Tx trigger transmit */
        retVal = FrTSyn_Tx_OnSyncTriggerTransmit(masterIdx, pduInfoPtr); /* SBSW_FRTSYN_FUNCTIONCALL_PDUINFO */
        break;
      }
    }
  }

  return retVal;
} /* FrTSyn_Tx_ProcessTriggerTransmit */

/**********************************************************************************************************************
 *  FrTSyn_Tx_OnSyncTriggerTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_OnSyncTriggerTransmit(
    FrTSyn_MasterConfigIterType masterIdx,
    P2VAR(PduInfoType, AUTOMATIC, FRTSYN_APPL_VAR) pduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy message to a buffer */
  if (pduInfoPtr->SduLength >= FRTSYN_TIME_SYNC_MESSAGE_DLC)
  {
    pduInfoPtr->SduLength = FRTSYN_TIME_SYNC_MESSAGE_DLC; /* SBSW_FRTSYN_PDUPTR_LENGTH_PARAM */
    VStdLib_MemCpy(pduInfoPtr->SduDataPtr, *FrTSyn_GetAddrSyncMsgOfMasterState(masterIdx), FRTSYN_TIME_SYNC_MESSAGE_DLC); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_FRTSYN_MEMCPY_PDU */ /* VCA_FRTSYN_CALL_VSTDLIB_FUNCTION_WITH_VAR_POINTER_ARGUMENT */

    retVal = E_OK;
  }

  /* #20 Reset master to 'wait for SYNC send' */
  FrTSyn_Tx_ResetStateToSyncSend(masterIdx);

#  if (FRTSYN_TXDEBOUNCECOUNTERIDXOFMASTERCONFIG == STD_ON)
  /* #30 Reset Tx debounce counter to configured value */
  FrTSyn_Tx_ResetDebounceCounter(masterIdx);
#  endif

  return retVal;
} /* FrTSyn_Tx_OnSyncTriggerTransmit */

/**********************************************************************************************************************
 *  FrTSyn_Tx_InitMasters
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
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitMasters(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrTSyn_MasterConfigIterType masterIdx;

  /* #10 Build struct for user data in order to initialize user data to zero */
  StbM_UserDataType initUserData = { 0u };
  const FrTSyn_FrMsgType initSyncMsg = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  /* #20 For each configured master: */
  for (masterIdx = 0u; masterIdx < FrTSyn_GetSizeOfMasterConfig(); masterIdx++)
  {
    /* #30 Initialize Tx period counter to zero */
    FrTSyn_Tx_StopTxPeriodCounter(masterIdx);

    /* #40 Set initial state of master to: wait to send a SYNC message */
    FrTSyn_SetStateOfMasterState(masterIdx, FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */

    /* #50 Initialize the sequence counter to the maximum value */
    FrTSyn_Tx_InitTxSequenceCounter(masterIdx);

    /* #60 Initialize the trigger transmit timeout to zero */
    FrTSyn_Tx_InitTriggerTransmitTimeout(masterIdx);

    /* #70 Initialize user data to zero */
    FrTSyn_SetUserDataOfMasterState(masterIdx, initUserData); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */

    /* #80 Initialize SYNC message type */
    VStdLib_MemCpy(*FrTSyn_GetAddrSyncMsgOfMasterState(masterIdx), initSyncMsg, sizeof(*FrTSyn_GetAddrSyncMsgOfMasterState(masterIdx))); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_FRTSYN_MEMCPY_SYNCMSG */ /* VCA_FRTSYN_CALL_VSTDLIB_FUNCTION_WITH_VAR_POINTER_ARGUMENT */

    /* #90 If immediate send mode is configured: */
#  if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
    /* #100 Initialize the immediate send mode */
    FrTSyn_Tx_InitImmediateSendMode(masterIdx);
#  endif
  }
} /* FrTSyn_Tx_InitMasters */

#  if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_InitImmediateSendMode
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitImmediateSendMode(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Reset the flag to identify an immediate sent of SYNC message to false */
  FrTSyn_SetTransmitImmediateSyncOfMasterState(masterIdx, FALSE); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */

  /* #20 Reset last stored time base update counter to zero */
  FrTSyn_SetLastUpdateCounterOfMasterState(masterIdx, 0u); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */

  /* #30 Initialize cyclic message resume counter */
  FrTSyn_Tx_InitCyclicMsgResumeCounter(masterIdx);
} /* FrTSyn_Tx_InitImmediateSendMode */
#  endif /* FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_TRANSMIT */

#ifndef FRTSYN_NOUNIT_PROTOCOLTX /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: ProtocolTx
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateMasters
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
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateMasters(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrTSyn_MasterConfigIterType masterIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each configured master: */
  for (masterIdx = 0u; masterIdx < FrTSyn_GetSizeOfMasterConfig(); masterIdx++)
  {
    /* #20 Update Tx counters */
    FrTSyn_Tx_UpdateCounters(masterIdx);

    /* #30 Switch to current state: */
    switch (FrTSyn_GetStateOfMasterState(masterIdx))
    {
      case FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND:
        /* #40 Update send SYNC state */
        FrTSyn_Tx_UpdateWaitingForSyncSendRequest(masterIdx);
        break;

      case FRTSYN_STATE_SEND_WAITING_FOR_SYNC_TRIGGER_TRANSMIT:
        /* #50 Update send SYNC trigger transmit state */
        FrTSyn_Tx_UpdateWaitingForSyncTriggerTransmit(masterIdx);
        break;

      default: /* COV_FRTSYN_MSR_MISRA */
        /* #60 there is no default case */
        break;
    }
  }
} /* FrTSyn_Tx_UpdateMasters */

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateCounters
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateCounters(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Update Tx period counter */
  FrTSyn_Tx_UpdateTxPeriodCounter(masterIdx);

  /* #20 If immediate send mode is configured: */
#  if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
  /* #30 Update cyclic message resume counter */
  FrTSyn_Tx_UpdateCyclicMsgResumeCounter(masterIdx);
#  endif
} /* FrTSyn_Tx_UpdateCounters */

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateWaitingForSyncTriggerTransmit
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
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateWaitingForSyncTriggerTransmit(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Update Tx trigger transmit timeout */
  FrTSyn_Tx_UpdateTriggerTransmitTimeout(masterIdx);

# if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
  /* #20 If last message was sent immediate: */
  if (FrTSyn_IsTransmitImmediateSyncOfMasterState(masterIdx))
  {
    /* #30 If trigger transmit timeout or cyclic msg resume time expired: */
    if ((FrTSyn_Tx_IsTriggerTransmitTimeoutExpired(masterIdx) == TRUE) || (FrTSyn_Tx_IsCyclicMsgResumeTimeExpired(masterIdx) == TRUE))
    {
      /* #40 Perform actions for unsuccessful transmit */
      FrTSyn_Tx_OnFailedTransmit(masterIdx);
    }
  }
  else
# endif
  {
    /* #50 If trigger transmit timeout or Tx period expired: */
    if ((FrTSyn_Tx_IsTriggerTransmitTimeoutExpired(masterIdx) == TRUE) || (FrTSyn_Tx_IsTxPeriodTimeExpired(masterIdx) == TRUE))
    {
      /* #60 Perform actions for unsuccessful transmit */
      FrTSyn_Tx_OnFailedTransmit(masterIdx);
    }
  }
} /* FrTSyn_Tx_UpdateWaitingForSyncTriggerTransmit */

/**********************************************************************************************************************
 *  FrTSyn_Tx_OnFailedTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Tx_OnFailedTransmit(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Reset master to 'wait for SYNC send' */
  FrTSyn_Tx_ResetStateToSyncSend(masterIdx);

  /* #20 Resend SYNC message in the current FrTSyn_MainFunction() */
  FrTSyn_Tx_UpdateWaitingForSyncSendRequest(masterIdx);
} /* FrTSyn_Tx_OnFailedTransmit */

/**********************************************************************************************************************
 *  FrTSyn_Tx_ResetStateToSyncSend
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ResetStateToSyncSend(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Reset master to 'wait to send SYNC' state */
  FrTSyn_SetStateOfMasterState(masterIdx, FRTSYN_STATE_SEND_WAITING_FOR_SYNC_SEND); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */

#  if (FRTSYN_EXISTS_SHARED_PDUS == STD_ON)
  /* #20 Free the blocked PduId state */
  FrTSyn_Tx_ReleasePduId(masterIdx);
#  endif

#  if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
  /* #30 If last message was sent immediate: */
  if (FrTSyn_IsTransmitImmediateSyncOfMasterState(masterIdx))
  {
    /* #40 Reset flag for immediate SYNC/OFS transmission to FALSE */
    FrTSyn_SetTransmitImmediateSyncOfMasterState(masterIdx, FALSE); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
  }
#  endif
} /* FrTSyn_Tx_ResetStateToSyncSend */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_PROTOCOLTX */

#ifndef FRTSYN_NOUNIT_SYNCTX /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: SyncTx
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateWaitingForSyncSendRequest
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateWaitingForSyncSendRequest(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 If send conditions for SYNC/OFS message are met: */
  if (FrTSyn_Tx_ValidateSendSyncConditions(masterIdx) == TRUE)
  {
    /* #20 Transmit SYNC/OFS message request */
    FrTSyn_Tx_TransmitSyncRequest(masterIdx);
  }
}/* FrTSyn_Tx_UpdateWaitingForSyncSendRequest */

/**********************************************************************************************************************
 *  FrTSyn_Tx_TransmitSyncRequest
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
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Tx_TransmitSyncRequest(FrTSyn_MasterConfigIterType masterIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get time for SYNC/OFS message, if succeeded: */
  if (FrTSyn_Tx_GetTimeSync(masterIdx) == E_OK)
  {
    /* ----- Local Variables ---------------------------------------------- */
    PduInfoType pduInfo;
    FrTSyn_FrMsgType syncMsg = { 0u };

    /* #20 Prepare PduInfo */
    pduInfo.SduLength  = FRTSYN_TIME_SYNC_MESSAGE_DLC;
    pduInfo.SduDataPtr = syncMsg;

    /* #30 Increment Tx sequence counter */
    FrTSyn_Tx_IncrementTxSequenceCounter(masterIdx);

    /* #40 Assemble SYNC/OFS message */
    FrTSyn_Tx_AssembleSyncMsg(masterIdx, &syncMsg); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */

    /* #50 Store message content */
    VStdLib_MemCpy(*FrTSyn_GetAddrSyncMsgOfMasterState(masterIdx), syncMsg, sizeof(*FrTSyn_GetAddrSyncMsgOfMasterState(masterIdx))); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_FRTSYN_MEMCPY_SYNCMSG */ /* VCA_FRTSYN_CALL_VSTDLIB_FUNCTION_WITH_VAR_POINTER_ARGUMENT */

    /* #60 Enter critical area to guarantee that state transition and PduId lock are consistent to actual transmission */
    SchM_Enter_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0();

    /* #70 Request transmission of SYNC/OFS message, if transmission succeeded: */
    if (FrIf_Transmit(FrTSyn_GetTxPduIdOfMasterConfig(masterIdx), &pduInfo) == E_OK) /* SBSW_FRTSYN_FUNCTIONCALL_PDUINFO */ /* VCA_FRTSYN_CALL_EXTERNAL_FUNCTION_WITH_CONST_POINTER_ARGUMENT */
    {
      /* #80 Run actions for succeeded transmission */
      FrTSyn_Tx_OnSuccessTransmitSyncRequest(masterIdx);
    }

    /* #90 Exit critical area */
    SchM_Exit_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0();
  }
} /* FrTSyn_Tx_TransmitSyncRequest */ /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  FrTSyn_Tx_OnSuccessTransmitSyncRequest
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
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_OnSuccessTransmitSyncRequest(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Set next state to: wait for SYNC trigger transmit */
  FrTSyn_SetStateOfMasterState(masterIdx, FRTSYN_STATE_SEND_WAITING_FOR_SYNC_TRIGGER_TRANSMIT); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */

#  if (FRTSYN_EXISTS_SHARED_PDUS == STD_ON)
  /* #20 Block specific PduId of the given master in order to not interrupt Transmit-TriggerTransmit sequence */
  FrTSyn_Tx_LockPduId(masterIdx);
#  endif

  /* #30 Reset trigger transmit timeout to configured value */
  FrTSyn_Tx_ResetTriggerTransmitTimeout(masterIdx);

#  if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
  /* #40 If SYNC/OFS send was immediate: */
  if (FrTSyn_IsTransmitImmediateSyncOfMasterState(masterIdx))
  {
    /* #50 Reset cyclic message resume counter to configured value */
    FrTSyn_Tx_ResetCyclicMsgResumeCounter(masterIdx);

    /* #60 Enable direct transmit after cyclic message resume time */
    FrTSyn_Tx_StopTxPeriodCounter(masterIdx);
  }
  else
#  endif
  {
    /* #70 Reset Tx period counter to configured value */
    FrTSyn_Tx_ResetTxPeriodCounter(masterIdx);
  }
} /* FrTSyn_Tx_OnSuccessTransmitSyncRequest */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_SYNCTX */

#ifndef FRTSYN_NOUNIT_CURRENTTIME /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: CurrentTime
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_GetTimeSync
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_GetTimeSync(FrTSyn_MasterConfigIterType masterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical area to guarantee that time from FrIf and StbM are consistent (actual transmission is independent) */
  SchM_Enter_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0();

#  if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
  if (FrTSyn_IsOffsetDomain(FrTSyn_GetDomainConfigIdxOfMasterConfig(masterIdx)) == TRUE)
  {
    /* #20 For OFS get the OFS time */
    retVal = FrTSyn_Tx_OnGetTimeOfs(masterIdx);
  }
  else
#  endif
  {
    /* #30 For SYNC get the SYNC time */
    retVal = FrTSyn_Tx_OnGetTimeSync(masterIdx);
  }

  /* #40 Exit critical area */
  SchM_Exit_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0();

  return retVal;
} /* FrTSyn_Tx_GetTimeSync */

/**********************************************************************************************************************
 *  FrTSyn_Tx_OnGetTimeSync
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
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_OnGetTimeSync(FrTSyn_MasterConfigIterType masterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (FrTSyn_IsFrIfOnline(FrTSyn_GetControllerInfoIdxOfMasterConfig(masterIdx)) == TRUE)
  {
    StbM_TimeStampType currentGlobalTimeTSync = { 0u };
    StbM_VirtualLocalTimeType localTimeT0Vlt = { 0u };
    StbM_UserDataType userData = { 0u };

    /* #10 Get current Synchronized Time Base's Time Tuple from StbM */
    if (StbM_BusGetCurrentTime(FrTSyn_GetTimeBaseIdOfDomainConfig(FrTSyn_GetDomainConfigIdxOfMasterConfig(masterIdx)), &currentGlobalTimeTSync, &localTimeT0Vlt, &userData) == E_OK) /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */ /* VCA_FRTSYN_CALL_STBM_FUNCTION_WITH_VAR_POINTER_ARGUMENT */
    {
      uint16 frIfMacroticks = 0u;
      uint8 frIfCycle = 0u;

      /* #20 Get cycle and macro ticks from FR interface */
      if (FrIf_GetGlobalTime(FrTSyn_GetSnvOfControllerInfo(FrTSyn_GetControllerInfoIdxOfMasterConfig(masterIdx)), &frIfCycle, &frIfMacroticks) == E_OK) /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */ /* VCA_FRTSYN_CALL_FRIF_FUNCTION_WITH_VAR_POINTER_ARGUMENT */
      {
        StbM_VirtualLocalTimeType localTimeT1Vlt = { 0u };

        /* #30 Get current Virtual Local Time value from StbM */
        retVal = StbM_GetCurrentVirtualLocalTime(FrTSyn_GetTimeBaseIdOfDomainConfig(FrTSyn_GetDomainConfigIdxOfMasterConfig(masterIdx)), &localTimeT1Vlt); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */ /* VCA_FRTSYN_CALL_STBM_FUNCTION_WITH_VAR_POINTER_ARGUMENT */

        /* #40 If time retrieval was successful and time base is synchronized: */
        if ((retVal == E_OK) && (FrTSyn_GetGlobalTimeBaseBit(currentGlobalTimeTSync.timeBaseStatus) != 0u))
        {
          StbM_TimeStampType timeStampT0 = { 0u };
          StbM_TimeStampType timeDelta = { 0u };

          FrTSyn_SetCycleCounterOfMasterState(masterIdx, frIfCycle); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */

          /* #50 Calculate the time span from current time until the start of the next cycle 0 */
          retVal = FrTSyn_Tx_CalculateTimeToNextCycle0(masterIdx, frIfMacroticks, &localTimeT0Vlt, &localTimeT1Vlt, &timeDelta); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */

          /* #60 Add current time and calculated time delta to get the future time value at the next FR cycle 0 */
          retVal |= FrTSyn_TimestampPlusTimestamp(&currentGlobalTimeTSync, &timeDelta, &timeStampT0); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */

          /* #70 Adopt received time base status from current time to time stamp containing time delta to the next FR cycle 0 */
          timeStampT0.timeBaseStatus = currentGlobalTimeTSync.timeBaseStatus;

          /* #80 Store calculated timestamp */
          FrTSyn_SetSyncTimestampOfMasterState(masterIdx, timeStampT0); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */

          /* #90 Store user data */
          FrTSyn_SetUserDataOfMasterState(masterIdx, userData); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
        }
        else
        {
          /* #100 If timebase is not synchronized, set return value to E_NOT_OK */
          retVal = E_NOT_OK;
        }
      }
    }
  }

  return retVal;
} /* FrTSyn_Tx_OnGetTimeSync */

#  if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_OnGetTimeOfs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_OnGetTimeOfs(FrTSyn_MasterConfigIterType masterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  if (FrTSyn_IsFrIfOnline(FrTSyn_GetControllerInfoIdxOfMasterConfig(masterIdx)) == TRUE)
  {
    StbM_TimeStampType offsetTimeStamp = { 0u };

    /* #10 Get current offset time from StbM */
    retVal = StbM_GetOffset(FrTSyn_GetTimeBaseIdOfDomainConfig(FrTSyn_GetDomainConfigIdxOfMasterConfig(masterIdx)), &offsetTimeStamp); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */ /* VCA_FRTSYN_CALL_STBM_FUNCTION_WITH_VAR_POINTER_ARGUMENT */

    /* #20 If time retrieval was successful and time base is synchronized, save offset time */
    if ((retVal == E_OK) && (FrTSyn_GetGlobalTimeBaseBit(offsetTimeStamp.timeBaseStatus) != 0u))
    {
      FrTSyn_SetSyncTimestampOfMasterState(masterIdx, offsetTimeStamp); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
    }
    /* #30 If timebase is not synchronized, set return value to E_NOT_OK */
    else
    {
      retVal = E_NOT_OK;
    }
  }

  return retVal;
} /* FrTSyn_Tx_OnGetTimeOfs */
#  endif /* FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON */

/**********************************************************************************************************************
 *  FrTSyn_Tx_CalculateTimeToNextCycle0
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Tx_CalculateTimeToNextCycle0(FrTSyn_MasterConfigIterType masterIdx,
    uint16 frIfMacroticks,
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_DATA) localTimeT0Vlt,
    P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_DATA) localTimeT1Vlt,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timeDeltaPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  StbM_VirtualLocalTimeType timediff = { 0u };
  uint32 timespan;

  /* #10 Subtract the time t0 from t1 */
  retVal = FrTSyn_SubtractVirtualLocalTime(localTimeT1Vlt, localTimeT0Vlt, &timediff); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */

  /* #20 Calculate the timespan from current time until the start of the next cycle 0 */
  timespan = (uint32)((((uint64)timediff.nanosecondsHi) << 32u) + timediff.nanosecondsLo) /* (T1Vlt - T0Vlt) */
    + (((uint32)64u - FrTSyn_GetCycleCounterOfMasterState(masterIdx)) * FrTSyn_GetCycleLengthOfControllerState(FrTSyn_GetControllerInfoIdxOfMasterConfig(masterIdx))) /* (64 - currentCycle) * cycleLength */
    - ((uint32)FrTSyn_GetMacrotickDurationOfControllerState(FrTSyn_GetControllerInfoIdxOfMasterConfig(masterIdx)) * frIfMacroticks); /* (currentMacroticks * macrotickDuration) */

  /* #30 Convert the calculated time span to a timestamp */
  FrTSyn_ConvertTimespanToTimestamp(timespan, timeDeltaPtr); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */

  return retVal;
} /* FrTSyn_Tx_CalculateTimeToNextCycle0 */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_CURRENTTIME */

#ifndef FRTSYN_NOUNIT_ASSEMBLEMSG /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: AssembleMsg
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_AssembleSyncMsg
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
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_AssembleSyncMsg(FrTSyn_MasterConfigIterType masterIdx,
    P2VAR(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_VAR) syncMsg)
{
  /* #10 Assemble common parts of all message types with SYNC message format */
  FrTSyn_Tx_OnAssembleSync(masterIdx, syncMsg); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */

  /* #20 Dispatch between SYNC/SYNC + CRC and OFS/OFS + CRC: */
  switch (FrTSyn_GetSyncMessageTypeOfMasterConfig(masterIdx))
  {
#  if (FRTSYN_EXISTS_SYNC_TXCRCSECUREDOFMASTERCONFIG == STD_ON)
    case FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_NOT_CRC:
      /* #30 Assemble with SYNC message format */
      FrTSyn_Tx_SetUserByte2ToSync(masterIdx, syncMsg); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
      break;
#  endif

#  if (FRTSYN_EXISTS_SYNC_CRC_TXCRCSECUREDOFMASTERCONFIG == STD_ON)
    case FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC:
      /* #40 Assemble with SYNC + CRC message format */
      FrTSyn_Tx_SetCrcToSync(masterIdx, syncMsg); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
      break;
#  endif

#  if (FRTSYN_EXISTS_OFS_TXCRCSECUREDOFMASTERCONFIG == STD_ON)
    case FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_NOT_CRC:
      /* #50 Assemble with OFS message format */
      FrTSyn_Tx_OnAssembleOfs(syncMsg); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
      FrTSyn_Tx_SetUserByte2ToSync(masterIdx, syncMsg); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
      break;
#  endif

#  if (FRTSYN_EXISTS_OFS_CRC_TXCRCSECUREDOFMASTERCONFIG == STD_ON)
    case FRTSYN_TIME_SYNC_MESSAGE_TYPE_OFS_CRC:
      /* #60 Assemble with OFS + CRC message format */
      FrTSyn_Tx_OnAssembleOfs(syncMsg); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
      FrTSyn_Tx_SetCrcToSync(masterIdx, syncMsg); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
      break;
#  endif

    default: /* COV_FRTSYN_MSR_MISRA */
      /* #70 no default case available */
      break;
  }
} /* FrTSyn_Tx_AssembleSyncMsg */

/**********************************************************************************************************************
 *  FrTSyn_Tx_OnAssembleSync
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
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_OnAssembleSync(FrTSyn_MasterConfigIterType masterIdx,
    P2VAR(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_VAR) syncMsg)
{
  /* #10 Byte 0: Set message type */
  FrTSyn_FrSignal_SetType(syncMsg, FrTSyn_GetSyncMessageTypeOfMasterConfig(masterIdx)); /* SBSW_FRTSYN_ACCESS_FRMSG */
  /* #20 Byte 2, Bit 0-3: Set sequence counter, Bit 4-7: Set domainId */
  FrTSyn_FrSignal_SetByte2(syncMsg, FrTSyn_GetMessageDomainIdOfMasterConfig(masterIdx), FrTSyn_GetTxSequenceCounterOfMasterState(masterIdx)); /* SBSW_FRTSYN_ACCESS_FRMSG */
  /* #30 Byte 3, Bit 0: Set reserved bit to default value zero, Bit 1: Set SGW bit, Bit 2-7: Set FR cycle counter */
  FrTSyn_FrSignal_SetByte3(syncMsg, FrTSyn_GetSyncToGatewayBit(FrTSyn_GetSyncTimestampOfMasterState(masterIdx).timeBaseStatus), FrTSyn_GetCycleCounterOfMasterState(masterIdx)); /* PRQA S 2985 */ /* MD_FrTSyn_Rule2.2_2985 */ /* SBSW_FRTSYN_ACCESS_FRMSG */
  /* #40 Byte 4: Set user byte 0 */
  FrTSyn_FrSignal_SetUserByte0(syncMsg, FrTSyn_GetUserDataOfMasterState(masterIdx).userByte0); /* SBSW_FRTSYN_ACCESS_FRMSG */
  /* #50 Byte 5: Set user byte 1 */
  FrTSyn_FrSignal_SetUserByte1(syncMsg, FrTSyn_GetUserDataOfMasterState(masterIdx).userByte1); /* SBSW_FRTSYN_ACCESS_FRMSG */
  /* #60 Byte 6-7: Set time value seconds high */
  FrTSyn_FrSignal_SetSecHi(syncMsg, FrTSyn_GetAddrSyncTimestampOfMasterState(masterIdx)->secondsHi); /* SBSW_FRTSYN_ACCESS_FRMSG */ /* SBSW_FRTSYN_ACCESS_FRMSG */
  /* #70 Set time, Byte 8-11: Set time value in seconds */
  FrTSyn_FrSignal_SetSec(syncMsg, FrTSyn_GetAddrSyncTimestampOfMasterState(masterIdx)->seconds); /* SBSW_FRTSYN_ACCESS_FRMSG */ /* SBSW_FRTSYN_ACCESS_FRMSG */ /* SBSW_FRTSYN_ACCESS_FRMSG */ /* SBSW_FRTSYN_ACCESS_FRMSG */
  /* #80 Byte 12-15: Set time value in nanoseconds */
  FrTSyn_FrSignal_SetNSec(syncMsg, FrTSyn_GetAddrSyncTimestampOfMasterState(masterIdx)->nanoseconds); /* SBSW_FRTSYN_ACCESS_FRMSG */ /* SBSW_FRTSYN_ACCESS_FRMSG */ /* SBSW_FRTSYN_ACCESS_FRMSG */ /* SBSW_FRTSYN_ACCESS_FRMSG */
} /* FrTSyn_Tx_OnAssembleSync */

#  if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_OnAssembleOfs
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_OnAssembleOfs(
    P2VAR(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_VAR) ofsMsg)
{
  /* #10 Clear reserved bits in OFS message */
  FrTSyn_FrSignal_ClearOfsReserved(ofsMsg); /* SBSW_FRTSYN_ACCESS_FRMSG */
} /* FrTSyn_Tx_OnAssembleOfs */
#  endif /* (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON) */

#  if ((FRTSYN_EXISTS_SYNC_TXCRCSECUREDOFMASTERCONFIG == STD_ON) || (FRTSYN_EXISTS_OFS_TXCRCSECUREDOFMASTERCONFIG == STD_ON))
/**********************************************************************************************************************
 *  FrTSyn_Tx_SetUserByte2ToSync
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_SetUserByte2ToSync(FrTSyn_MasterConfigIterType masterIdx,
    P2VAR(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_VAR) syncMsg)
{
  /* #10 Set user byte 2 */
  FrTSyn_FrSignal_SetUserByte2(syncMsg, FrTSyn_GetUserDataOfMasterState(masterIdx).userByte2); /* SBSW_FRTSYN_ACCESS_FRMSG */
} /* FrTSyn_Tx_SetUserByte2ToSync */
#  endif /* (FRTSYN_EXISTS_SYNC_TXCRCSECUREDOFMASTERCONFIG == STD_ON) || (FRTSYN_EXISTS_OFS_TXCRCSECUREDOFMASTERCONFIG == STD_ON) */

#  if((FRTSYN_EXISTS_SYNC_CRC_TXCRCSECUREDOFMASTERCONFIG == STD_ON) || (FRTSYN_EXISTS_OFS_CRC_TXCRCSECUREDOFMASTERCONFIG == STD_ON))
/**********************************************************************************************************************
 *  FrTSyn_Tx_SetCrcToSync
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Tx_SetCrcToSync(FrTSyn_MasterConfigIterType masterIdx,
    P2VAR(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_VAR) syncMsg)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 crcResult, dataId;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Calculate CRC */
  dataId = FrTSyn_GetSyncDataIDList(FrTSyn_GetSyncDataIDListStartIdxOfDomainConfig(FrTSyn_GetDomainConfigIdxOfMasterConfig(masterIdx)) + FrTSyn_GetTxSequenceCounterOfMasterState(masterIdx));
  crcResult = FrTSyn_CalculateMsgCrc(&dataId, (P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA)) syncMsg); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR_PARAMPTR */

  /* #20 Set CRC to SYNC message */
  FrTSyn_FrSignal_SetCRC(syncMsg, crcResult); /* SBSW_FRTSYN_ACCESS_FRMSG */
} /* FrTSyn_Tx_SetCrcToSync */
#  endif /* (FRTSYN_EXISTS_SYNC_CRC_TXCRCSECUREDOFMASTERCONFIG == STD_ON) || (FRTSYN_EXISTS_OFS_CRC_TXCRCSECUREDOFMASTERCONFIG == STD_ON) */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_ASSEMBLEMSG */

#ifndef FRTSYN_NOUNIT_VALIDATION /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: Validation
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_ValidateSendSyncConditions
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
FRTSYN_LOCAL FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_ValidateSendSyncConditions(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean sendSync;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if transmission channel is ready to send */
  if (FrTSyn_Tx_IsTxReady(masterIdx) == FALSE)
  {
    sendSync = FALSE;
  }
  /* #20 If immediate send mode is configured: */
#  if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
  /* #30 Validate if SYNC/OFS message has to be sent in immediate mode */
  else if (FrTSyn_Tx_ValidateImmediateSync(masterIdx) == TRUE)
  {
    /* #40 Immediate SYNC/OFS send detected */
    sendSync = TRUE;
  }
  /* #50 Check if cyclic message resume time expired */
  else if (FrTSyn_Tx_IsCyclicMsgResumeTimeExpired(masterIdx) == FALSE)
  {
    sendSync = FALSE;
  }
#  endif

  /* #60 Check if Tx period time is expired */
  else if (FrTSyn_Tx_IsTxPeriodTimeExpired(masterIdx) == FALSE)
  {
    sendSync = FALSE;
  }
  else
  {
    /* #70 Cyclic transmission of SYNC/OFS detected */
    sendSync = TRUE;
  }

  return sendSync;
} /* FrTSyn_Tx_ValidateSendSyncConditions */

#  if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_ValidateImmediateSync
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
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_ValidateImmediateSync(FrTSyn_MasterConfigIterType masterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If master has configured immediate send SYNC message type: */
#   if (FRTSYN_EXISTS_NOT_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
  if (FrTSyn_GetImmediateTimeSyncOfMasterConfig(masterIdx) == FRTSYN_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG)
#   endif
  {
    /* #20 Get time base update counter from StbM */
    uint8 currentUpdateCounter = StbM_GetTimeBaseUpdateCounter(FrTSyn_GetTimeBaseIdOfDomainConfig(FrTSyn_GetDomainConfigIdxOfMasterConfig(masterIdx)));

    /* #30 If last stored time base update counter has changed or previous Immediate Transmit was not executed: */
    if ((currentUpdateCounter != FrTSyn_GetLastUpdateCounterOfMasterState(masterIdx)) || (FrTSyn_IsTransmitImmediateSyncOfMasterState(masterIdx) == TRUE)) /* PRQA S 1881, 4304 */ /* MD_MSR_AutosarBoolean */
    {
      retVal = TRUE;

      /* #40 Save retrieved time base update counter */
      FrTSyn_SetLastUpdateCounterOfMasterState(masterIdx, currentUpdateCounter); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */

      /* #50 Set flag to indicate immediate SYNC message type */
      FrTSyn_SetTransmitImmediateSyncOfMasterState(masterIdx, TRUE); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
    }
  }

  return retVal;
} /* FrTSyn_Tx_ValidateImmediateSync */
#  endif /* FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON */

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsTxReady
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsTxReady(FrTSyn_MasterConfigIterType masterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean txIsReady;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if configured FR controller is ready */
  if (FrTSyn_IsFrControllerReady(masterIdx) == FALSE)
  {
    txIsReady = FALSE;
  }

#  if (FRTSYN_EXISTS_SHARED_PDUS == STD_ON)
  /* #20 Check if configured PduId is unlocked */
  else if (FrTSyn_Tx_IsPduIdUnLocked(masterIdx) == FALSE)
  {
    txIsReady = FALSE;
  }
#  endif

#  if (FRTSYN_TXDEBOUNCECOUNTERIDXOFMASTERCONFIG == STD_ON)
  /* #30 Check if configured debounce time is expired */
  else if (FrTSyn_Tx_IsDebounceTimeExpired(masterIdx) == FALSE)
  {
    txIsReady = FALSE;
  }
#  endif
  /* #40 If #10-#30 are true, Tx is considered to be ready */
  else
  {
    txIsReady = TRUE;
  }

  return txIsReady;
} /* FrTSyn_Tx_IsTxReady */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_VALIDATION */

#ifndef FRTSYN_NOUNIT_TXPERIODTIME /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: TxPeriodTime
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_StopTxPeriodCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_StopTxPeriodCounter(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Stop Tx period counter */
  FrTSyn_SetTxPeriodCounterOfMasterState(masterIdx, 0u); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
} /* FrTSyn_Tx_StopTxPeriodCounter */

/**********************************************************************************************************************
 *  FrTSyn_Tx_ResetTxPeriodCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ResetTxPeriodCounter(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Reset Tx period counter to configured value */
  FrTSyn_SetTxPeriodCounterOfMasterState(masterIdx, FrTSyn_GetTxPeriodOfMasterConfig(masterIdx)); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
} /* FrTSyn_Tx_ResetTxPeriodCounter */

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateTxPeriodCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateTxPeriodCounter(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 If Tx period counter is running */
  if (FrTSyn_GetTxPeriodCounterOfMasterState(masterIdx) > 0u)
  {
    /* #20 Decrement Tx period */
    FrTSyn_DecTxPeriodCounterOfMasterState(masterIdx); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
  }
} /* FrTSyn_Tx_UpdateTxPeriodCounter */

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsTxPeriodTimeExpired
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsTxPeriodTimeExpired(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Check if the Tx period time is expired */
  return (boolean) (FrTSyn_GetTxPeriodCounterOfMasterState(masterIdx) == 0u); /* PRQA S 1881, 4304 */ /* MD_MSR_AutosarBoolean */
} /* FrTSyn_Tx_IsTxPeriodTimeExpired */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_TXPERIODTIME */

#ifndef FRTSYN_NOUNIT_TXSEQUENCECOUNTER /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: TxSequenceCounter
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_InitTxSequenceCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitTxSequenceCounter(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Initialize the Tx sequence counter to the maximum value */
  FrTSyn_SetTxSequenceCounterOfMasterState(masterIdx, FRTSYN_MAX_SEQUENCE_COUNTER); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
} /* FrTSyn_Tx_InitTxSequenceCounter */

/**********************************************************************************************************************
 *  FrTSyn_Tx_IncrementTxSequenceCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_IncrementTxSequenceCounter(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 If sequence counter overflow occurs */
  if (FrTSyn_GetTxSequenceCounterOfMasterState(masterIdx) == FRTSYN_MAX_SEQUENCE_COUNTER)
  {
    /* #20 Reset sequence counter to zero */
    FrTSyn_SetTxSequenceCounterOfMasterState(masterIdx, 0u); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
  }
  else
  {
    /* #30 Increment Tx sequence counter */
    FrTSyn_IncTxSequenceCounterOfMasterState(masterIdx); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
  }

} /* FrTSyn_Tx_IncrementTxSequenceCounter */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_TXSEQUENCECOUNTER */

#ifndef FRTSYN_NOUNIT_CYCLICMSGRESUMETIME /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
#  if (FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: CyclicMsgResumeTime
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_InitCyclicMsgResumeCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitCyclicMsgResumeCounter(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Set cyclic message resume counter to zero */
  FrTSyn_SetCyclicMsgResumeCounterOfMasterState(masterIdx, 0u); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
} /* FrTSyn_Tx_InitCyclicMsgResumeCounter */

/**********************************************************************************************************************
 *  FrTSyn_Tx_ResetCyclicMsgResumeCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ResetCyclicMsgResumeCounter(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Reset cyclic message resume counter to configured value */
  FrTSyn_SetCyclicMsgResumeCounterOfMasterState(masterIdx, FrTSyn_GetCyclicMsgResumeTimeOfMasterConfig(masterIdx)); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
} /* FrTSyn_Tx_ResetCyclicMsgResumeCounter */

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateCyclicMsgResumeCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateCyclicMsgResumeCounter(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 If cyclic message resume counter is running */
  if (FrTSyn_GetCyclicMsgResumeCounterOfMasterState(masterIdx) > 0u)
  {
    /* #20 Decrement Tx cyclic message resume counter */
    FrTSyn_DecCyclicMsgResumeCounterOfMasterState(masterIdx); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
  }
} /* FrTSyn_Tx_UpdateCyclicMsgResumeCounter */

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsCyclicMsgResumeTimeExpired
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsCyclicMsgResumeTimeExpired(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Check if the cyclic message resume time is expired */
  return (boolean) (FrTSyn_GetCyclicMsgResumeCounterOfMasterState(masterIdx) == 0u); /* PRQA S 1881, 4304 */ /* MD_MSR_AutosarBoolean */
} /* FrTSyn_Tx_IsCyclicMsgResumeTimeExpired */
#  endif /* FRTSYN_EXISTS_SUPPORTED_IMMEDIATETIMESYNCOFMASTERCONFIG == STD_ON */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_CYCLICMSGRESUMETIME */

#ifndef FRTSYN_NOUNIT_TRIGGERTRANSMITTIMEOUT /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: TriggerTransmitTimeout
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_InitTriggerTransmitTimeout
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitTriggerTransmitTimeout(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Set trigger transmit timeout counter to zero */
  FrTSyn_SetTriggerTransmitTimeoutCountOfMasterState(masterIdx, 0u); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
} /* FrTSyn_Tx_InitTriggerTransmitTimeout */

/**********************************************************************************************************************
 *  FrTSyn_Tx_ResetTriggerTransmitTimeout
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ResetTriggerTransmitTimeout(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Reset trigger transmit timeout to configured value */
  FrTSyn_SetTriggerTransmitTimeoutCountOfMasterState(masterIdx, FrTSyn_GetTriggerTransmitTimeoutOfMasterConfig(masterIdx)); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
} /* FrTSyn_Tx_ResetTriggerTransmitTimeout */

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateTriggerTransmitTimeout
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateTriggerTransmitTimeout(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Check if trigger transmit timeout can be decremented */
  if (FrTSyn_GetTriggerTransmitTimeoutCountOfMasterState(masterIdx) > 0u)
  {
    /* #20 Decrement trigger transmit timeout */
    FrTSyn_DecTriggerTransmitTimeoutCountOfMasterState(masterIdx); /* SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR */
  }
} /* FrTSyn_Tx_UpdateTriggerTransmitTimeout */

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsTriggerTransmitTimeoutExpired
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsTriggerTransmitTimeoutExpired(
    FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Check if the Tx trigger transmit timeout is expired */
  return (boolean) (FrTSyn_GetTriggerTransmitTimeoutCountOfMasterState(masterIdx) == 0u); /* PRQA S 1881, 4304 */ /* MD_MSR_AutosarBoolean */
} /* FrTSyn_Tx_IsTriggerTransmitTimeoutExpired */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_TRIGGERTRANSMITTIMEOUT */

#ifndef FRTSYN_NOUNIT_DEBOUNCETIME /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
#  if (FRTSYN_TXDEBOUNCECOUNTERIDXOFMASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: DebounceTime
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_InitDebounceCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitDebounceCounter(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrTSyn_TxDebounceCounterIterType debounceCounterIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each configured Tx debounce counter: */
  for (debounceCounterIdx = 0u; debounceCounterIdx < FrTSyn_GetSizeOfTxDebounceCounter(); debounceCounterIdx++)
  {
    /* #20 Set Tx debounce counter to zero */
    FrTSyn_SetTxDebounceCounter(debounceCounterIdx, 0u); /* SBSW_FRTSYN_ACCESS_TXDEBOUNCECOUNTER */
  }
} /* FrTSyn_Tx_InitDebounceCounter */

/**********************************************************************************************************************
 *  FrTSyn_Tx_ResetDebounceCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ResetDebounceCounter(FrTSyn_MasterConfigIterType masterIdx)
{
#   if (FRTSYN_TX_CONF_MIXED_DEBOUNCE_COUNTER == STD_ON)
  /* #10 If given master has debouncing configured: */
  if (FrTSyn_Tx_HasDebouncing(masterIdx) == TRUE)
#   endif
  {
    /* #20 Reset Tx debounce counter to configured value */
    FrTSyn_SetTxDebounceCounter(FrTSyn_GetTxDebounceCounterIdxOfMasterConfig(masterIdx), FrTSyn_GetDebounceTimeOfMasterConfig(masterIdx)); /* SBSW_FRTSYN_ACCESS_TXDEBOUNCECOUNTER_IND */ /* VCA_FRTSYN_ACCESS_TXDEBOUNCECOUNTER_IND */
  }
} /* FrTSyn_Tx_ResetDebounceCounter */

/**********************************************************************************************************************
 *  FrTSyn_Tx_UpdateDebounceCounter
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_UpdateDebounceCounter(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrTSyn_TxDebounceCounterIterType txDebounceCounterIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each configured Tx debounce counter */
  for (txDebounceCounterIdx = 0u; txDebounceCounterIdx < FrTSyn_GetSizeOfTxDebounceCounter(); txDebounceCounterIdx++)
  {
    /* #20 Check if Tx debounce counter can be decremented */
    if (FrTSyn_GetTxDebounceCounter(txDebounceCounterIdx) > 0u)
    {
      /* #30 Decrement Tx debounce counter */
      FrTSyn_DecTxDebounceCounter(txDebounceCounterIdx); /* SBSW_FRTSYN_ACCESS_TXDEBOUNCECOUNTER */
    }
  }
} /* FrTSyn_Tx_UpdateDebounceCounter */

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsDebounceTimeExpired
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsDebounceTimeExpired(FrTSyn_MasterConfigIterType masterIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* #10 Set return value to: Tx debounce time is not expired */
  boolean txDebounceTimeExpired = FALSE;

  /* ----- Implementation ----------------------------------------------- */
#   if (FRTSYN_TX_CONF_MIXED_DEBOUNCE_COUNTER == STD_ON)
  /* #20 If given master has debouncing configured: */
  if (FrTSyn_Tx_HasDebouncing(masterIdx) == TRUE)
#   endif
  {
    /* #30 Check if Tx debounce counter is zero */
    if (FrTSyn_GetTxDebounceCounter(FrTSyn_GetTxDebounceCounterIdxOfMasterConfig(masterIdx)) == 0u)
    {
      /* #40 Set return value to: Tx is debounced */
      txDebounceTimeExpired = TRUE;
    }
  }
#   if (FRTSYN_TX_CONF_MIXED_DEBOUNCE_COUNTER == STD_ON)
  else
  {
    txDebounceTimeExpired = TRUE;
  }
#   endif

  return txDebounceTimeExpired;
} /* FrTSyn_Tx_IsDebounceTimeExpired */

#   if (FRTSYN_TX_CONF_MIXED_DEBOUNCE_COUNTER == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Tx_HasDebouncing
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_HasDebouncing(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Check if the given master has deboucing configured by comparing the debounce index with the size of debounce table */
  return (boolean) (FrTSyn_GetTxDebounceCounterIdxOfMasterConfig(masterIdx) < FrTSyn_GetSizeOfTxDebounceCounter()); /* PRQA S 1881, 4304 */ /* MD_MSR_AutosarBoolean */
} /* FrTSyn_Tx_HasDebouncing */
#   endif /* FRTSYN_TX_CONF_MIXED_DEBOUNCE_COUNTER == STD_ON */
#  endif /* FRTSYN_TXDEBOUNCECOUNTERIDXOFMASTERCONFIG == STD_ON */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_DEBOUNCETIME */

#ifndef FRTSYN_NOUNIT_PDUIDLOCK /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_MASTERCONFIG == STD_ON)
#  if (FRTSYN_EXISTS_SHARED_PDUS == STD_ON)
/**********************************************************************************************************************
 *  Unit: PduIdLock
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Tx_InitPduIdStates
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_InitPduIdStates(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrTSyn_PduIdStatesIterType pduIdIdx;

  /* ----- Implementation ----------------------------------------------- */
  for (pduIdIdx = 0u; pduIdIdx < FrTSyn_GetSizeOfPduIdStates(); pduIdIdx++)
  {
    /* #20 Reset the PduId state to invalid master index */
    FrTSyn_SetPduIdStates(pduIdIdx, FRTSYN_INV_MASTER_IDX); /* SBSW_FRTSYN_ACCESS_PDUIDSTATES */
  }
} /* FrTSyn_Tx_InitPduIdStates */

/**********************************************************************************************************************
 *  FrTSyn_Tx_LockPduId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_LockPduId(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Lock the configured PduId for the given master */
  FrTSyn_SetPduIdStates(FrTSyn_GetPduIdStatesIdxOfMasterConfig(masterIdx), (uint8) masterIdx); /* SBSW_FRTSYN_ACCESS_PDUIDSTATES */
} /* FrTSyn_Tx_LockPduId */

/**********************************************************************************************************************
 *  FrTSyn_Tx_ReleasePduId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Tx_ReleasePduId(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Release the configured PduId for the given master */
  FrTSyn_SetPduIdStates(FrTSyn_GetPduIdStatesIdxOfMasterConfig(masterIdx), FRTSYN_INV_MASTER_IDX); /* SBSW_FRTSYN_ACCESS_PDUIDSTATES */
} /* FrTSyn_Tx_ReleasePduId */

/**********************************************************************************************************************
 *  FrTSyn_Tx_IsPduIdUnLocked
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Tx_IsPduIdUnLocked(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Check if the configured PduId is free */
  uint8 blockingMasterIdx = FrTSyn_GetPduIdStates(FrTSyn_GetPduIdStatesIdxOfMasterConfig(masterIdx));
  return (boolean) (blockingMasterIdx == FRTSYN_INV_MASTER_IDX); /* PRQA S 1881, 4304 */ /* MD_MSR_AutosarBoolean */
} /* FrTSyn_Tx_IsPduIdUnLocked */
#  endif /* FRTSYN_EXISTS_SHARED_PDUS == STD_ON */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_PDUIDLOCK */

#ifndef FRTSYN_NOUNIT_FRCONTROLLER /* COV_FRTSYN_NOUNIT_SWITCH */
/**********************************************************************************************************************
 *  Unit: FrController
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_InitFrController
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_InitFrController(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrTSyn_ControllerStateIterType frControllerIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each configured FR controller: */
  for (frControllerIdx = 0u; frControllerIdx < FrTSyn_GetSizeOfControllerState(); frControllerIdx++)
  {
    /* #20 Reset FR controller state to TX_ON */
# if (FRTSYN_MASTERCONFIG == STD_ON)
    FrTSyn_SetTransmissionStateOfControllerState(frControllerIdx, FRTSYN_TX_ON); /* SBSW_FRTSYN_ACCESS_CONTROLLERSTATE */
# endif
    FrTSyn_SetCycleLengthOfControllerState(frControllerIdx, FrIf_GetCycleLength(FrTSyn_GetSnvOfControllerInfo(frControllerIdx))); /* SBSW_FRTSYN_ACCESS_CONTROLLERSTATE */
    FrTSyn_SetMacrotickDurationOfControllerState(frControllerIdx, FrIf_GetMacrotickDuration(FrTSyn_GetSnvOfControllerInfo(frControllerIdx))); /* SBSW_FRTSYN_ACCESS_CONTROLLERSTATE */
  }
} /* FrTSyn_InitFrController */

/**********************************************************************************************************************
 *  FrTSyn_IsFrIfOnline
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL FUNC(boolean, FRTSYN_CODE) FrTSyn_IsFrIfOnline(FrTSyn_ControllerStateIterType frControllerIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrIf_StateType frIfState = FRIF_STATE_OFFLINE;
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if FR interface is online */
  if ((FrIf_GetState(FrTSyn_GetClusterSnvOfControllerInfo(frControllerIdx), &frIfState) == E_OK) && (frIfState == FRIF_STATE_ONLINE)) /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */ /* VCA_FRTSYN_CALL_FRIF_FUNCTION_WITH_VAR_POINTER_ARGUMENT */
  {
    retVal = TRUE;
  }

  return retVal;
} /* FrTSyn_IsFrIfOnline */

# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_IsFrControllerReady
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_IsFrControllerReady(FrTSyn_MasterConfigIterType masterIdx)
{
  /* #10 Check if the configured FR controller is online */
  return (boolean) (FrTSyn_GetTransmissionStateOfControllerState(FrTSyn_GetControllerInfoIdxOfMasterConfig(masterIdx)) == FRTSYN_TX_ON); /* PRQA S 1881, 4304 */ /* MD_MSR_AutosarBoolean */
} /* FrTSyn_IsFrControllerReady */

/**********************************************************************************************************************
 *  FrTSyn_SetFrControllerTransmissionMode
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_SetFrControllerTransmissionMode(uint8 ctrlIdx,
    FrTSyn_TransmissionModeType mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrTSyn_ControllerStateIterType frControllerIdx;
  boolean foundCtrlIdx = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Search for FR controllers with given controller index */
  for (frControllerIdx = 0u; frControllerIdx < FrTSyn_GetSizeOfControllerState(); frControllerIdx++)
  {
    if (FrTSyn_GetSnvOfControllerInfo(frControllerIdx) == ctrlIdx)
    {
      /* #20 Set transmission mode to given mode */
      FrTSyn_SetTransmissionStateOfControllerState(frControllerIdx, mode); /* SBSW_FRTSYN_ACCESS_CONTROLLERSTATE */
      foundCtrlIdx = TRUE;
      break;
    }
  }

  return foundCtrlIdx;
} /* FrTSyn_SetFrControllerTransmissionMode */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_FRCONTROLLER */

/**********************************************************************************************************************
 *
 *
 *  SLAVE
 *
 *
 *********************************************************************************************************************/

#ifndef FRTSYN_NOUNIT_RECEIVE /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: Receive
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Rx_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrTSyn_SlaveStateIterType slaveIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 For each configured slave: */
  for (slaveIdx = 0u; slaveIdx < FrTSyn_GetSizeOfSlaveState(); slaveIdx++)
  {
    /* #20 Set the state to 'wait for SYNC' */
    FrTSyn_SetStateOfSlaveState(slaveIdx, FRTSYN_STATE_RECEIVE_WAITING_FOR_SYNC); /* SBSW_FRTSYN_ACCESS_SLAVESTATE_VAR */

    /* #30 Reset last received sequence counter to maximum value, which indicates no sequence counter received yet */
    FrTSyn_SetRxSequenceCounterOfSlaveState(slaveIdx, FRTSYN_INV_SEQUENCE_COUNTER); /* SBSW_FRTSYN_ACCESS_SLAVESTATE_VAR */
  }
} /* FrTSyn_Rx_Init */

/**********************************************************************************************************************
 *  FrTSyn_Rx_Update
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_Update(void)
{
  /* #10 Update configured slaves */
  FrTSyn_Rx_UpdateSlaves();
} /* FrTSyn_Rx_Update */

/**********************************************************************************************************************
 *  FrTSyn_Rx_FindSlave
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
FRTSYN_LOCAL_INLINE FUNC(uint8, FRTSYN_CODE) FrTSyn_Rx_FindSlave(PduIdType pduId, uint8 domainId,
    P2VAR(FrTSyn_SlaveConfigIterType, AUTOMATIC, FRTSYN_APPL_VAR) outSlaveIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrTSyn_SlaveConfigIterType slaveIdx;

  /* #10 Initialize errorId with index for invalid domain */
  uint8 errorId = FRTSYN_INV_DOMAIN_IDX;
  /* #20 Initialize out parameter for slave index with invalid slave index */
  *outSlaveIdx = FRTSYN_INV_SLAVE_IDX; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */

  /* ----- Implementation ----------------------------------------------- */
  /* #30 For each configured slave: */
  for (slaveIdx = 0u; slaveIdx < FrTSyn_GetSizeOfSlaveConfig(); slaveIdx++)
  {
    /* #40 If the domainId is valid: */
    if (FrTSyn_GetDomainIdOfDomainConfig(FrTSyn_GetDomainConfigIdxOfSlaveConfig(slaveIdx)) == domainId)
    {
      /* #50 Set errorId to invalid Pdu */
      errorId = FRTSYN_E_INVALID_PDUID;

      /* #60 If pdu is valid: */
      if (FrTSyn_GetSlaveHandleIdOfSlaveConfig(slaveIdx) == pduId)
      {
        /* #70 Write found slave to out parameter */
        *outSlaveIdx = slaveIdx; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
        /* #80 Set errorId back to FRTSYN_E_NO_ERROR */
        errorId = FRTSYN_E_NO_ERROR;
      }
      /* #90 Break the loop, since the domain was found */
      break;
    }
  }

  /*@ assert errorId != FRTSYN_E_NO_ERROR || slaveIdx < FrTSyn_GetSizeOfSlaveConfig(); */
  return errorId;
} /* FrTSyn_Rx_FindSlave */

/**********************************************************************************************************************
 *  FrTSyn_Rx_GetDomainId
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(uint8, FRTSYN_CODE) FrTSyn_Rx_GetDomainId(
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 messageType;
  uint8 domainId;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Retrieve the message type from the given message */
  messageType = FrTSyn_FrSignal_GetType(ptrMsgInfo);

  /* #20 Retrieve the domainId from the given message */
  domainId = FrTSyn_FrSignal_GetD(ptrMsgInfo);

  /* #30 Check if the message type is offset */
  if (FrTSyn_IsOfsMsg(messageType))
  {
    /* #40 Add offset to domainId */
    domainId += FRTSYN_START_OFFSET_TIME_DOMAIN_ID;
  }

  return domainId;
} /* FrTSyn_Rx_GetDomainId */
# endif /* FRTSYN_SLAVECONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_RECEIVE */

#ifndef FRTSYN_NOUNIT_PROTOCOLRX /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: ProtocolRx
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Rx_ProcessRxIndication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_ProcessRxIndication(FrTSyn_SlaveConfigIterType slaveIdx,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Validate received message info: */
  if (FrTSyn_Rx_ValidateSync(slaveIdx, ptrMsgInfo) == TRUE) /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
  {
    /* #20 Set slave state to 'SYNC received' */
    FrTSyn_SetStateOfSlaveState(slaveIdx, FRTSYN_STATE_RECEIVE_SYNC_RECEIVED); /* SBSW_FRTSYN_ACCESS_SLAVESTATE_VAR */

    /* #30 Store SYNC/OFS message with timestamp which indicates (future) time value of the Time Base at the start of the next FR cycle 0 */
    VStdLib_MemCpy(*FrTSyn_GetAddrSyncMsgOfSlaveState(slaveIdx), *ptrMsgInfo, sizeof(*FrTSyn_GetAddrSyncMsgOfSlaveState(slaveIdx))); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_FRTSYN_MEMCPY_SYNCMSG */ /* VCA_FRTSYN_CALL_VSTDLIB_FUNCTION_WITH_VAR_POINTER_ARGUMENT */
  }
} /* FrTSyn_Rx_ProcessRxIndication */

/**********************************************************************************************************************
 *  FrTSyn_Rx_UpdateSlaves
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_UpdateSlaves(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrTSyn_SlaveConfigIterType slaveIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* 10 For each configured slave: */
  for (slaveIdx = 0u; slaveIdx < FrTSyn_GetSizeOfSlaveConfig(); slaveIdx++)
  {
    /* #20 If slave is in state 'SYNC received' */
    if (FrTSyn_GetStateOfSlaveState(slaveIdx) == FRTSYN_STATE_RECEIVE_SYNC_RECEIVED)
    {
      /* #30 Receive SYNC/OFS message for respective slave */
      FrTSyn_Rx_ProcessReceivedSyncMsg(slaveIdx);
    }
  }
} /* FrTSyn_Rx_UpdateSlaves */
# endif /* FRTSYN_SLAVECONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_PROTOCOLRX */

#ifndef FRTSYN_NOUNIT_SYNCRX /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: SyncRx
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Rx_ProcessReceivedSyncMsg
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
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_ProcessReceivedSyncMsg(FrTSyn_SlaveConfigIterType slaveIdx)
{
#  if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
  /* 10 If time domain is an offset domain: */
  if (FrTSyn_IsOffsetDomain(FrTSyn_GetDomainConfigIdxOfSlaveConfig(slaveIdx)) == TRUE)
  {
    /* #20 Receive payload of OFS message and perform actions for successful reception */
    FrTSyn_Rx_OnReceiveOfsMsg(slaveIdx);
  }
  else
#  endif
  {
    /* #30 Receive payload of SYNC message and perform actions for successful reception */
    FrTSyn_Rx_OnReceiveSyncMsg(slaveIdx);
  }

  /* #40 Perform actions on successful SYNC/OFS message reception */
  FrTSyn_Rx_OnSuccessSyncReceive(slaveIdx);
} /* FrTSyn_Rx_ProcessReceivedSyncMsg */

/**********************************************************************************************************************
 *  FrTSyn_Rx_OnReceiveSyncMsg
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
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnReceiveSyncMsg(FrTSyn_SlaveConfigIterType slaveIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  StbM_TimeStampType actualTime, receivedTime = { 0u };
  StbM_UserDataType receivedUserData = { 0u };
  StbM_VirtualLocalTimeType localTimeT1Vlt = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter exclusive area to guarantee that the calculation of the time and the handover to StbM are consistent */
  SchM_Enter_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0();

  /* #20 Get payload from SYNC message */
  FrTSyn_Rx_GetPayload(slaveIdx, &receivedTime, &receivedUserData); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */

  /* #30 Calculate new global time value T1 */
  if (FrTSyn_Rx_CalculateNewGlobalTime(slaveIdx, &receivedTime, &actualTime, &localTimeT1Vlt) == E_OK) /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */
  {
    StbM_MeasurementType measureData = { 0u };

    /* #40 Get timebase status from message */
    if (FrTSyn_FrSignal_GetSGW(FrTSyn_GetAddrSyncMsgOfSlaveState(slaveIdx)) == 0u)
    {
      actualTime.timeBaseStatus = 0x00u;
    }
    else
    {
      actualTime.timeBaseStatus = StbM_Sync_To_Gateway_BflMask;
    }

    /* #50 Provide new global time to StbM */
    (void)StbM_BusSetGlobalTime(FrTSyn_GetTimeBaseIdOfDomainConfig(FrTSyn_GetDomainConfigIdxOfSlaveConfig(slaveIdx)), &actualTime, &receivedUserData, &measureData, &localTimeT1Vlt); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */ /* VCA_FRTSYN_CALL_EXTERNAL_FUNCTION_WITH_CONST_POINTER_ARGUMENT */
  }

  /* #60 Exit exclusive area */
  SchM_Exit_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0();
} /* FrTSyn_Rx_OnReceiveSyncMsg */

#  if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Rx_OnReceiveOfsMsg
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
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnReceiveOfsMsg(FrTSyn_SlaveConfigIterType slaveIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  StbM_TimeStampType receivedTime = { 0u };
  StbM_VirtualLocalTimeType localTimeT1Vlt = { 0u };

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter exclusive area to guarantee a consistent offset provided to StbM */
  SchM_Enter_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0();

  /* #20 Get payload of OFS message */
  FrTSyn_Rx_GetOffsetPayload(slaveIdx, &receivedTime); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */

  /* #30 Retrieve current Virtual Local Time from StbM */
  if (StbM_GetCurrentVirtualLocalTime(FrTSyn_GetTimeBaseIdOfDomainConfig(FrTSyn_GetDomainConfigIdxOfSlaveConfig(slaveIdx)), &localTimeT1Vlt) == E_OK) /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */ /* VCA_FRTSYN_CALL_STBM_FUNCTION_WITH_VAR_POINTER_ARGUMENT */
  {
    StbM_MeasurementType measureData = { 0u };

    /* #40 Provide new offset to StbM */
    (void)StbM_BusSetGlobalTime(FrTSyn_GetTimeBaseIdOfDomainConfig(FrTSyn_GetDomainConfigIdxOfSlaveConfig(slaveIdx)), &receivedTime, NULL_PTR, &measureData, &localTimeT1Vlt); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */ /* VCA_FRTSYN_CALL_EXTERNAL_FUNCTION_WITH_CONST_POINTER_ARGUMENT */
  }

  /* #50 Exit exclusive area */
  SchM_Exit_FrTSyn_FRTSYN_EXCLUSIVE_AREA_0();
} /* FrTSyn_Rx_OnReceiveOfsMsg */
#  endif /* FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON */

/**********************************************************************************************************************
 *  FrTSyn_Rx_OnSuccessSyncReceive
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnSuccessSyncReceive(FrTSyn_SlaveConfigIterType slaveIdx)
{
  /* #10 Set state of slave to 'wait for SYNC' */
  FrTSyn_SetStateOfSlaveState(slaveIdx, FRTSYN_STATE_RECEIVE_WAITING_FOR_SYNC); /* SBSW_FRTSYN_ACCESS_SLAVESTATE_VAR */
} /* FrTSyn_Rx_OnSuccessSyncReceive */

/**********************************************************************************************************************
 *  FrTSyn_Rx_CalculateNewGlobalTime
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
FRTSYN_LOCAL_INLINE FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_Rx_CalculateNewGlobalTime(
  FrTSyn_SlaveConfigIterType slaveIdx,
  P2CONST(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_DATA) syncTimeStamp,
  P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) newGlobalTime,
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, FRTSYN_APPL_VAR) localTimeT1Vlt)
{
  /* ----- Local Variables ---------------------------------------------- */
  FrTSyn_ControllerInfoIdxOfSlaveConfigType frControllerIdx = FrTSyn_GetControllerInfoIdxOfSlaveConfig(slaveIdx);
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If FR interface is online: */
  if (FrTSyn_IsFrIfOnline(frControllerIdx) == TRUE)
  {
    StbM_TimeStampType currentTime = { 0u };
    uint8 currentCycle = 0u;
    uint16 currentMacroticks = 0u;
    uint32 elapsedTime;

    /* #20 Get current cycle and current macroticks */
    retVal = FrIf_GetGlobalTime(FrTSyn_GetSnvOfControllerInfo(frControllerIdx), &currentCycle, &currentMacroticks); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */ /* VCA_FRTSYN_CALL_FRIF_FUNCTION_WITH_VAR_POINTER_ARGUMENT */

    /* #30 Get current Virtual Local Time value from StbM (part of tuple T1VLT) */
    retVal |= StbM_GetCurrentVirtualLocalTime(FrTSyn_GetTimeBaseIdOfDomainConfig(FrTSyn_GetDomainConfigIdxOfSlaveConfig(slaveIdx)), localTimeT1Vlt);/* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */ /* VCA_FRTSYN_CALL_STBM_FUNCTION_WITH_VAR_POINTER_ARGUMENT */

    /* #40 Evaluate elapsed time since the start of the last cycle 0 */
    elapsedTime = (FrTSyn_GetCycleLengthOfControllerState(frControllerIdx) * currentCycle) + ((uint32)FrTSyn_GetMacrotickDurationOfControllerState(frControllerIdx) * currentMacroticks);

    /* #50 Determine actual local time delay given by elapsed time value */
    FrTSyn_ConvertTimespanToTimestamp(elapsedTime, &currentTime); /* SBSW_FRTSYN_ACCESS_CONTROLLERSTATE_AND_LOCALPTR */

    /* #60 Calculate new global time value T1 (part of tuple T1) */
    retVal |= FrTSyn_TimestampPlusTimestamp(syncTimeStamp, &currentTime, newGlobalTime); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR_PARAMPTR */

    /* #70 Perform time correction if current cycle is greater or equal than retrieved FR cycle counter */
    if ((retVal == E_OK) && (currentCycle >= FrTSyn_FrSignal_GetFCNT(FrTSyn_GetAddrSyncMsgOfSlaveState(slaveIdx))))
    {
      FrTSyn_Rx_OnCorrectNewGlobalTime(slaveIdx, newGlobalTime); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
    }
  }

  return retVal;
} /* FrTSyn_Rx_CalculateNewGlobalTime */

/**********************************************************************************************************************
 *  FrTSyn_Rx_OnCorrectNewGlobalTime
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnCorrectNewGlobalTime(FrTSyn_SlaveConfigIterType slaveIdx,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) newGlobalTime)
{
  /* ----- Local Variables ---------------------------------------------- */
  StbM_TimeStampType correctionTime = { 0u };
  StbM_TimeStampType correctedTime = { 0u };
  uint32 timeCorrection;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 New global time value shall be subtracted by 64 times the FR cycle duration */
  timeCorrection = FrTSyn_GetCycleLengthOfControllerState(FrTSyn_GetControllerInfoIdxOfSlaveConfig(slaveIdx)) * 64u;
  FrTSyn_ConvertTimespanToTimestamp(timeCorrection, &correctionTime); /* SBSW_FRTSYN_ACCESS_CONTROLLERSTATE_AND_LOCALPTR */

  /* newGlobalTime will always be equal or greater than correctionTime */
  FrTSyn_TimestampMinusTimestamp(newGlobalTime, &correctionTime, &correctedTime); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR_PARAMPTR */

  *newGlobalTime = correctedTime; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
} /* FrTSyn_Rx_OnCorrectNewGlobalTime */
# endif /* FRTSYN_SLAVECONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_SYNCRX */

#ifndef FRTSYN_NOUNIT_DISASSEMBLEMSG /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: DisassembleMsg
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Rx_GetPayload
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
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_GetPayload(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timeStamp,
    P2VAR(StbM_UserDataType, AUTOMATIC, FRTSYN_APPL_VAR) userData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 messageType;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get message type */
  messageType = FrTSyn_FrSignal_GetType(FrTSyn_GetAddrSyncMsgOfSlaveState(slaveIdx));

  /* #20 Get the timestamp from the payload */
  FrTSyn_Rx_OnGetTimeStamp(slaveIdx, timeStamp); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */

  /* #30 Get content of user byte 0 and user byte 1 */
  userData->userByte0 = FrTSyn_FrSignal_GetUserByte0(FrTSyn_GetAddrSyncMsgOfSlaveState(slaveIdx)); /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  userData->userByte1 = FrTSyn_FrSignal_GetUserByte1(FrTSyn_GetAddrSyncMsgOfSlaveState(slaveIdx)); /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */

  /* #40 Dispatch from get user data based on message type: */
  switch (messageType)
  {
#  if ((FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON))
    case FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_CRC:
      /* #50 SYNC is CRC secured */
      FrTSyn_Rx_OnGetUserDataCrc(userData); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
      break;
#  endif

#  if ((FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON))
    case FRTSYN_TIME_SYNC_MESSAGE_TYPE_SYNC_NOT_CRC:
      /* #60 SYNC is not CRC secured */
      FrTSyn_Rx_OnGetUserData(slaveIdx, userData); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
      break;
#  endif

    default: /* COV_FRTSYN_MSR_MISRA */
      /* #70 no default available */
      break;
  }
} /* FrTSyn_Rx_GetPayload */

#  if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Rx_GetOffsetPayload
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_GetOffsetPayload(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timeStamp)
{
  /* #10 Get the timestamp from the payload */
  FrTSyn_Rx_OnGetTimeStamp(slaveIdx, timeStamp); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
} /* FrTSyn_Rx_GetOffsetPayload */
#  endif /* FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON */

/**********************************************************************************************************************
 *  FrTSyn_Rx_OnGetTimeStamp
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnGetTimeStamp(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2VAR(StbM_TimeStampType, AUTOMATIC, FRTSYN_APPL_VAR) timeStamp)
{
  /* #10 Get secondsHi from Msg */
  timeStamp->secondsHi = FrTSyn_FrSignal_GetSecHi(FrTSyn_GetAddrSyncMsgOfSlaveState(slaveIdx)); /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  /* #20 Get seconds from Msg */
  timeStamp->seconds = FrTSyn_FrSignal_GetSec(FrTSyn_GetAddrSyncMsgOfSlaveState(slaveIdx)); /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
    /* #30 Get nanoseconds from Msg */
  timeStamp->nanoseconds = FrTSyn_FrSignal_GetNSec(FrTSyn_GetAddrSyncMsgOfSlaveState(slaveIdx)); /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
} /* FrTSyn_Rx_OnGetTimeStamp */

#  if ((FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON))
/**********************************************************************************************************************
 *  FrTSyn_Rx_OnGetUserDataCrc
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnGetUserDataCrc(
    P2VAR(StbM_UserDataType, AUTOMATIC, FRTSYN_APPL_VAR) userData)
{
  /* #10 Set user data length to 2 */
  userData->userDataLength = 2u; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
} /* FrTSyn_Rx_OnGetUserDataCrc */
#  endif /* ((FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)) */

#  if ((FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON))
/**********************************************************************************************************************
 *  FrTSyn_Rx_OnGetUserData
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(void, FRTSYN_CODE) FrTSyn_Rx_OnGetUserData(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2VAR(StbM_UserDataType, AUTOMATIC, FRTSYN_APPL_VAR) userData)
{
  /* #10 Set user data length to 3 */
  userData->userDataLength = 3u; /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
  /* #20 Get user byte 2 */
  userData->userByte2 = FrTSyn_FrSignal_GetUserByte2(FrTSyn_GetAddrSyncMsgOfSlaveState(slaveIdx)); /* SBSW_FRTSYN_WRITE_OUTPARAM_POINTER */
} /* FrTSyn_Rx_OnGetUserData */
#  endif /* ((FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)) */
# endif /* FRTSYN_SLAVECONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_DISASSEMBLEMSG */

#ifndef FRTSYN_NOUNIT_VALIDATIONMSG /* COV_FRTSYN_NOUNIT_SWITCH */
# if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  Unit: ValidationMsg
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_Rx_ValidateSync
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
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_ValidateSync(FrTSyn_SlaveConfigIterType slaveIdx,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isValid;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Validate message type */
  if (FrTSyn_Rx_ValidateSyncMessageType(ptrMsgInfo) == FALSE) /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
  {
    isValid = FALSE;
  }
  /* #20 Validate sequence counter */
  else if (FrTSyn_Rx_ValidateSequenceCounterSync(slaveIdx, ptrMsgInfo) == FALSE) /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
  {
    isValid = FALSE;
  }
  /* #30 Validate nanoseconds value */
  else if (FrTSyn_FrSignal_GetNSec(ptrMsgInfo) > FRTSYN_TIMESTAMP_MAX_NANOSECONDS)
  {
    isValid = FALSE;
  }
#  if ((FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON))
  /* #40 Validate CRC */
  else if (FrTSyn_Rx_ValidateCrcSync(slaveIdx, ptrMsgInfo) == FALSE) /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
  {
    isValid = FALSE;
  }
#  endif /* ((FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)) */
  /* #50 Set message as valid */
  else
  {
    isValid = TRUE;
  }

  return isValid;
} /* FrTSyn_Rx_ValidateSync */

/**********************************************************************************************************************
 *  FrTSyn_Rx_ValidateSyncMessageType
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_ValidateSyncMessageType(
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isValid = FALSE;
  uint8 messageType;

  /* ----- Implementation ----------------------------------------------- */
  messageType = FrTSyn_FrSignal_GetType(ptrMsgInfo);

  /* #10 Check if the message type is either SYNC or OFS */
  if (FrTSyn_IsSyncMsg(messageType)
#  if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
    || FrTSyn_IsOfsMsg(messageType)
#  endif
      )
  {
    isValid = TRUE;
  }

  return isValid;
} /* FrTSyn_Rx_ValidateSyncMessageType */

/**********************************************************************************************************************
 *  FrTSyn_Rx_HasRefTimebaseTimeout
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_HasRefTimebaseTimeout(FrTSyn_SlaveConfigIterType slaveIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal;
  boolean isTimeout = FALSE;
  StbM_TimeBaseStatusType syncTimeBaseStatus = 0u;
  StbM_TimeBaseStatusType offsetTimeBaseStatus = 0u;


  /* ----- Implementation ----------------------------------------------- */
  /* #10 Retrieve timebase status of the referred timebase */
  retVal = StbM_GetTimeBaseStatus(FrTSyn_GetTimeBaseIdOfDomainConfig(FrTSyn_GetDomainConfigIdxOfSlaveConfig(slaveIdx)), &syncTimeBaseStatus, &offsetTimeBaseStatus); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */ /* VCA_FRTSYN_CALL_STBM_FUNCTION_WITH_VAR_POINTER_ARGUMENT */

  /* #20 If retrieval succeeded: */
  if(retVal == E_OK)
  {
    /* #30 Check timeout status based on timebase type */
#  if (FRTSYN_EXISTS_OFFSET_OFFSETDOMAINOFDOMAINCONFIG == STD_ON)
    if (FrTSyn_IsOffsetDomain(FrTSyn_GetDomainConfigIdxOfSlaveConfig(slaveIdx)) == TRUE)
    {
      isTimeout = (boolean)(FrTSyn_GetTimeoutBit(offsetTimeBaseStatus) != 0u);  /* PRQA S 1881, 4304 */ /* MD_MSR_AutosarBoolean */
    }
    else
#  endif
    {
      isTimeout = (boolean)(FrTSyn_GetTimeoutBit(syncTimeBaseStatus) != 0u); /* PRQA S 1881, 4304 */ /* MD_MSR_AutosarBoolean */
    }
  }
  return isTimeout;
} /* FrTSyn_Rx_HasRefTimebaseTimeout */

/**********************************************************************************************************************
 *  FrTSyn_Rx_ValidateSequenceCounterSync
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
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_ValidateSequenceCounterSync(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 sequenceCounter;

  /* #10 Set sequence counter as valid */
  boolean isValid = TRUE;

  /* ----- Implementation ----------------------------------------------- */
  /* #20 Retrieve the received sequence counter from the given message */
  sequenceCounter = FrTSyn_FrSignal_GetSC(ptrMsgInfo);

  /* #30 If it is not the first received sequence counter: */
  if (FrTSyn_GetRxSequenceCounterOfSlaveState(slaveIdx) <= FRTSYN_MAX_SEQUENCE_COUNTER)
  {
    /* #40 Calculate sequence counter jump width from last received sequence counter */
    uint8 jumpWidth = FrTSyn_Rx_CalculateSequenceCounterJumpWidth(slaveIdx, sequenceCounter);

    /* #50 If sequence counter is stuck (jump width == 0) or jump width is bigger as max allowed configured jump width: */
    if ((jumpWidth == 0u) || (jumpWidth > FrTSyn_GetSequenceCounterJumpWidthOfSlaveConfig(slaveIdx)))
    {
      /* #60 Invalidate sequence counter */
      isValid = FALSE;

      if (jumpWidth > 0u)
      {
        /* #70 Set sequence counter as valid if a timeout occurred at the referred timebase */
        isValid = FrTSyn_Rx_HasRefTimebaseTimeout(slaveIdx);
      }
    }
  }

  /* #80 Store received sequence counter */
  FrTSyn_SetRxSequenceCounterOfSlaveState(slaveIdx, sequenceCounter); /* SBSW_FRTSYN_ACCESS_SLAVESTATE_VAR */

  return isValid;
} /* FrTSyn_Rx_ValidateSequenceCounterSync */

/**********************************************************************************************************************
 *  FrTSyn_Rx_CalculateSequenceCounterJumpWidth
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(uint8, FRTSYN_CODE) FrTSyn_Rx_CalculateSequenceCounterJumpWidth(
    FrTSyn_SlaveConfigIterType slaveIdx,
    uint8 sequenceCounter)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 jumpWidth;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If no sequence counter overflow occurred: */
  if (sequenceCounter >= FrTSyn_GetRxSequenceCounterOfSlaveState(slaveIdx))
  {
    /* #20 Calculate jump width without overflow handling */
    jumpWidth = sequenceCounter - FrTSyn_GetRxSequenceCounterOfSlaveState(slaveIdx);
  }
  else
  {
    /* #30 Calculate jump width with overflow handling */
    jumpWidth = (FRTSYN_MAX_SEQUENCE_COUNTER - FrTSyn_GetRxSequenceCounterOfSlaveState(slaveIdx)) + sequenceCounter + 1u;
  }

  return jumpWidth;
} /* FrTSyn_Rx_CalculateSequenceCounterJumpWidth */

#  if ((FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON))
/**********************************************************************************************************************
 *  FrTSyn_Rx_ValidateCrcSync
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
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_ValidateCrcSync(FrTSyn_SlaveConfigIterType slaveIdx,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Dispatch CRC validation to: */
  switch (FrTSyn_GetRxCrcValidatedOfSlaveConfig(slaveIdx))
  {
#   if (FRTSYN_EXISTS_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)
    /* #20 CRC validation in SYNC message for CRC-IGNORED configured slave (always valid) */
    case FRTSYN_IGNORED_RXCRCVALIDATEDOFSLAVECONFIG:
      isValid = TRUE;
      break;
#   endif

#   if (FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)
    /* #30 CRC validation in SYNC message for CRC-NOT_VALIDATED configured slave */
    case FRTSYN_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG:
      isValid = FrTSyn_Rx_OnValidateSyncCrcNotValidated(ptrMsgInfo); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
      break;
#   endif

#   if (FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)
    /* #40 CRC validation in SYNC message for CRC-VALIDATED configured slave */
    case FRTSYN_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG:
      isValid = FrTSyn_Rx_OnValidateSyncCrcValidated(slaveIdx, ptrMsgInfo); /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
      break;
#   endif

    default: /* COV_FRTSYN_MSR_MISRA */
      /* #50 no default available */
      break;
  }

  return isValid;
} /* FrTSyn_Rx_ValidateCrcSync */
#  endif /* (FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) || (FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON) */

#  if (FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Rx_OnValidateSyncCrcNotValidated
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_OnValidateSyncCrcNotValidated(
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo)
{
  /* #10 Check that SYNC message type has no CRC */
  return (boolean) ((FrTSyn_IsSyncCrcSecured(FrTSyn_FrSignal_GetType(ptrMsgInfo))) == FALSE); /* PRQA S 1881, 4304 */ /* MD_MSR_AutosarBoolean */
} /* FrTSyn_Rx_OnValidateSyncCrcNotValidated */
#  endif /* FRTSYN_EXISTS_NOT_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON */

#  if (FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_Rx_OnValidateSyncCrcValidated
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FRTSYN_LOCAL_INLINE FUNC(boolean, FRTSYN_CODE) FrTSyn_Rx_OnValidateSyncCrcValidated(
    FrTSyn_SlaveConfigIterType slaveIdx,
    P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA) ptrMsgInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isValid = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if SYNC message of actual message type has CRC */
  if (FrTSyn_IsSyncCrcSecured(FrTSyn_FrSignal_GetType(ptrMsgInfo)))
  {
    uint8 crcResult;
    uint8 dataId;

    /* #20 Calculate expected CRC */
    dataId = FrTSyn_GetSyncDataIDList(FrTSyn_GetSyncDataIDListStartIdxOfDomainConfig(FrTSyn_GetDomainConfigIdxOfSlaveConfig(slaveIdx)) + FrTSyn_FrSignal_GetSC(ptrMsgInfo));
    crcResult = FrTSyn_CalculateMsgCrc(&dataId, ptrMsgInfo); /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR_PARAMPTR */

    /* #30 Check if CRC matches with expected one */
    if (crcResult == FrTSyn_FrSignal_GetCRC(ptrMsgInfo))
    {
      isValid = TRUE;
    }
  }

  return isValid;
} /* FrTSyn_Rx_OnValidateSyncCrcValidated */
#  endif /* FRTSYN_EXISTS_VALIDATED_RXCRCVALIDATEDOFSLAVECONFIG == STD_ON */
# endif /* FRTSYN_SLAVECONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_VALIDATIONMSG */


/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#ifndef FRTSYN_NOUNIT_COMMON /* COV_FRTSYN_NOUNIT_SWITCH */
/**********************************************************************************************************************
 *  Unit: Common
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  FrTSyn_InitMemory
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, FRTSYN_CODE) FrTSyn_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Set module initialization state to false */
  FrTSyn_SetInitialized(FALSE); /* SBSW_FRTSYN_ACCESS_CSL_VAR */
# endif
} /* FrTSyn_InitMemory */

/**********************************************************************************************************************
 *  FrTSyn_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, FRTSYN_CODE) FrTSyn_Init(P2CONST(FrTSyn_ConfigType, AUTOMATIC, FRTSYN_APPL_DATA) configPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  FRTSYN_DUMMY_STATEMENT(configPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */

# if (FRTSYN_MASTERCONFIG == STD_ON)
  /* #10 Initialize Tx unit */
  FrTSyn_Tx_Init();
# endif

  /* #20 Initialize FR controller */
  FrTSyn_InitFrController();

# if (FRTSYN_SLAVECONFIG == STD_ON)
  /* #30 Initialize Rx unit */
  FrTSyn_Rx_Init();
# endif

# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #40 Initialize module state variable */
  FrTSyn_SetInitialized(TRUE); /* SBSW_FRTSYN_ACCESS_CSL_VAR */
# endif
} /* FrTSyn_Init */

# if (FRTSYN_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_GetVersionInfo
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, FRTSYN_CODE) FrTSyn_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, FRTSYN_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FRTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter versioninfo for NULL pointer */
  if (versioninfo == NULL_PTR)
  {
    errorId = FRTSYN_E_NULL_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Set versioninfo with corresponding macros from component header */
    versioninfo->vendorID = (FRTSYN_VENDOR_ID); /* SBSW_FRTSYN_VERSIONINFOPTR */
    versioninfo->moduleID = (FRTSYN_MODULE_ID); /* SBSW_FRTSYN_VERSIONINFOPTR */
    versioninfo->sw_major_version = (FRTSYN_SW_MAJOR_VERSION); /* SBSW_FRTSYN_VERSIONINFOPTR */
    versioninfo->sw_minor_version = (FRTSYN_SW_MINOR_VERSION); /* SBSW_FRTSYN_VERSIONINFOPTR */
    versioninfo->sw_patch_version = (FRTSYN_SW_PATCH_VERSION); /* SBSW_FRTSYN_VERSIONINFOPTR */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (FRTSYN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FRTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(FRTSYN_MODULE_ID, FRTSYN_INSTANCE_ID_DET, FRTSYN_SID_GET_VERSION_INFO, errorId);
  }
#  else
  FRTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
} /* FrTSyn_GetVersionInfo */
# endif /* FRTSYN_VERSION_INFO_API == STD_ON */

/**********************************************************************************************************************
 *  FrTSyn_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, FRTSYN_CODE) FrTSyn_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FRTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if module is initialized */
  if(FrTSyn_IsInitialized() == FALSE) /* PRQA S 1881 */ /* MD_MSR_AutosarBoolean */
  {
    errorId = FRTSYN_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (FRTSYN_SLAVECONFIG == STD_ON)
    /* #20 Update Rx unit */
    FrTSyn_Rx_Update();
# endif

# if (FRTSYN_MASTERCONFIG == STD_ON)
    /* #30 Update Tx unit */
    FrTSyn_Tx_Update();
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (FRTSYN_DEV_ERROR_REPORT == STD_ON)
  /* #40 Report Det-Error if error occurred and Det-Check configured */
  if (errorId != FRTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(FRTSYN_MODULE_ID, FRTSYN_INSTANCE_ID_DET, FRTSYN_SID_MAIN_FUNCTION, errorId);
  }
# else
  FRTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
} /* FrTSyn_MainFunction */

# if (FRTSYN_MASTERCONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_SetTransmissionMode
 *********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
 *
 *
*/
FUNC(void, FRTSYN_CODE) FrTSyn_SetTransmissionMode(uint8 CtrlIdx, FrTSyn_TransmissionModeType Mode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = FRTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if (FrTSyn_IsInitialized() == FALSE) /* PRQA S 1881 */ /* MD_MSR_AutosarBoolean */
  {
    errorId = FRTSYN_E_UNINIT;
  }
    /* #20 Check if mode is invalid */
  else if ((Mode != FRTSYN_TX_OFF) && (Mode != FRTSYN_TX_ON))
  {
    errorId = FRTSYN_E_PARAM;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Set transmission mode to given mode if controllerId is valid */
#  if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
    if (FrTSyn_SetFrControllerTransmissionMode(CtrlIdx, Mode) == FALSE)
    {
      errorId = FRTSYN_E_INV_CTRL_IDX;
    }
#  else
    (void) FrTSyn_SetFrControllerTransmissionMode(CtrlIdx, Mode);
#  endif
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (FRTSYN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FRTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(FRTSYN_MODULE_ID, FRTSYN_INSTANCE_ID_DET, FRTSYN_SID_SET_TRANSMISSION_MODE, errorId);
  }
#  else
  FRTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
} /* FrTSyn_SetTransmissionMode */

/**********************************************************************************************************************
 *  FrTSyn_TriggerTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TriggerTransmit(
    PduIdType TxPduId,
    P2VAR(PduInfoType, AUTOMATIC, FRTSYN_APPL_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = FRTSYN_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if(FrTSyn_IsInitialized() == FALSE) /* PRQA S 1881 */ /* MD_MSR_AutosarBoolean */
  {
    errorId = FRTSYN_E_UNINIT;
  }
  /* #20 Check pointer to Pdu Info and corresponding Sdu Data pointer for NULL pointer */
  else if ((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR))
  {
    errorId = FRTSYN_E_NULL_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 If corresponding master is found copy message to a buffer and update state machine */
    if (FrTSyn_Tx_ProcessTriggerTransmit(TxPduId, PduInfoPtr, &errorId) == E_OK) /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR_PARAMPTR */
    {
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (FRTSYN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FRTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(FRTSYN_MODULE_ID, FRTSYN_INSTANCE_ID_DET, FRTSYN_SID_TRIGGER_TRANSMIT, errorId);
  }
#  else
  FRTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif

  return retVal;
} /* FrTSyn_TriggerTransmit */
# endif /* FRTSYN_MASTERCONFIG == STD_ON */

# if (FRTSYN_SLAVECONFIG == STD_ON)
/**********************************************************************************************************************
 *  FrTSyn_RxIndication
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
FUNC(void, FRTSYN_CODE) FrTSyn_RxIndication(
    PduIdType RxPduId,
    P2CONST(PduInfoType, AUTOMATIC, FRTSYN_APPL_DATA) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId;
  FrTSyn_SlaveConfigIterType slaveIdx = FRTSYN_INV_SLAVE_IDX;

  /* ----- Development Error Checks ------------------------------------- */
#  if (FRTSYN_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check initialization state of the component */
  if(FrTSyn_IsInitialized() == FALSE) /* PRQA S 1881 */ /* MD_MSR_AutosarBoolean */
  {
    errorId = FRTSYN_E_UNINIT;
  }
  /* #20 Check pointer to Pdu Info and corresponding Sdu Data pointer for NULL pointer */
  else if ((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR))
  {
    errorId = FRTSYN_E_NULL_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Find the corresponding slave to the Rx Indication */
    errorId = FrTSyn_Rx_FindSlave(RxPduId, FrTSyn_Rx_GetDomainId((P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA))PduInfoPtr->SduDataPtr), &slaveIdx); /* PRQA S 0310 */ /* MD_FrTSyn_Rule11.3 */ /* SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR */ /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */

    if (PduInfoPtr->SduLength != FRTSYN_TIME_SYNC_MESSAGE_DLC)
    {
      errorId = FRTSYN_E_PARAM;
    }

    /* #40 If slave search was successful and Pdu Info length is correct: */
    if (errorId == FRTSYN_E_NO_ERROR)
    {
      /*@ assert slaveIdx < FrTSyn_GetSizeOfSlaveConfig(); */ /* VCA_FRTSYN_VALID_SLAVEIDX */
      /* #50 Conduct the Rx Indication processing */
      FrTSyn_Rx_ProcessRxIndication(slaveIdx, (P2CONST(FrTSyn_FrMsgType, AUTOMATIC, FRTSYN_APPL_DATA))PduInfoPtr->SduDataPtr); /* PRQA S 0310 */ /* MD_FrTSyn_Rule11.3 */ /* SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR */
    }
    else
    {
      /* #60 Slave search failed. Ignore failure because of invalid domainId */
      if (errorId == FRTSYN_INV_DOMAIN_IDX)
      {
        errorId = FRTSYN_E_NO_ERROR;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (FRTSYN_DEV_ERROR_REPORT == STD_ON)
  if (errorId != FRTSYN_E_NO_ERROR)
  {
    (void)Det_ReportError(FRTSYN_MODULE_ID, FRTSYN_INSTANCE_ID_DET, FRTSYN_SID_RX_INDICATION, errorId);
  }
#  else
  FRTSYN_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
} /* FrTSyn_RxIndication */
# endif /* FRTSYN_SLAVECONFIG == STD_ON */
#endif /* FRTSYN_NOUNIT_COMMON */

#define FRTSYN_STOP_SEC_CODE
#include "FrTSyn_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:
 MD_FrTSyn_Rule2.2_2985:
      Reason:     A function like macro is implemented to handle different argument values. In the special case a part of the macro is not required.
      Risk:       no risk
      Prevention: Covered by code review.

 MD_FrTSyn_Rule11.3:
      Reason:     For the internal processing the internal data type is used. Therefore the incoming data at the external interface has to be casted to the corresponding internal type.
      Risk:       no risk
      Prevention: Covered by code review.
*/

/* Safe BSW assumptions:
  Indirections:
    DomainConfig->SlaveConfig  1:0..1
    DomainConfig->MasterConfig 1:0..N
    MasterConfig->PduIdStates  1:1
    MasterConfig->ControllerInfo  1:1
    MasterConfig->TxDebounceCounter   1:0..1

  Size Constraints:
    SlaveConfig  <-> SlaveState
    MasterConfig <-> MasterState
    ControllerInfo <-> ControllerState

  These assumptions are modelled in ComStackLib
*/

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_FRTSYN_ACCESS_MASTERSTATE_VAR
  \DESCRIPTION Access of master state (same size as master config) with same index using ComStackLib
  \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

  \ID SBSW_FRTSYN_ACCESS_SLAVESTATE_VAR
  \DESCRIPTION Access of slave state (same size as slave config) with same index using ComStackLib
  \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

  \ID SBSW_FRTSYN_ACCESS_PDUIDSTATES
  \DESCRIPTION Write access to PduIdStates using ComStackLib
  \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib

  \ID SBSW_FRTSYN_ACCESS_CONTROLLERSTATE
  \DESCRIPTION Write access to ControllerState using ComStackLib
  \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib

  \ID SBSW_FRTSYN_ACCESS_TXDEBOUNCECOUNTER
  \DESCRIPTION Write access to TxDebounceCounter using ComStackLib
  \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib

  \ID SBSW_FRTSYN_ACCESS_TXDEBOUNCECOUNTER_IND
  \DESCRIPTION Access of debounce counter via indirection over master
  \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID SBSW_FRTSYN_ACCESS_CSL_VAR
  \DESCRIPTION A ComStackLib generated variable is accessed.
  \COUNTERMEASURE \N The variable is written using a generated macro of the CSL. It immediately dereferences the variables address without pointer arithmetic.

  \ID SBSW_FRTSYN_MEMCPY_SYNCMSG
  \DESCRIPTION Call to MemCpy with SyncMsgOf*State as destination buffer. The index comes from the caller calculated by indirection of the DomainConfig.
               The DomainIndex is given as parameter on external interfaces or by iterating over the domains in the FrTSyn_MainFunction.
               The size parameter is calculated using sizeof(). The source parameter is a constant pointer.
  \COUNTERMEASURE \N Qualified use-case CSL02 and CSL03 of ComStackLib

  \ID SBSW_FRTSYN_MEMCPY_PDU
  \DESCRIPTION Call to MemCpy with the PDU buffer obtained from the Bus interface as destination buffer.
               The source parameter is a constant pointer.
  \COUNTERMEASURE \N The buffer is checked against the size parameter beforehand.

  \ID SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR
  \DESCRIPTION Call of a function with pointer parameters where the pointers are pointing to local (stack) objects.
  \COUNTERMEASURE \N Compiler ensures validity of the pointers.

  \ID SBSW_FRTSYN_WRITE_OUTPARAM_POINTER
  \DESCRIPTION Write to a out parameter which was given as pointer.
  \COUNTERMEASURE \N Caller ensures validity of the pointers.

  \ID SBSW_FRTSYN_FUNCTIONCALL_PARAMPTR
  \DESCRIPTION Forwarding the pointer that was given as parameter, without modifying anything.
  \COUNTERMEASURE \N Caller ensures validity of the pointers.

  \ID SBSW_FRTSYN_FUNCTIONCALL_LOCALPTR_PARAMPTR
  \DESCRIPTION Call of a function with pointer parameters where the pointers are pointing to local (stack) objects and forward pointers that were given as parameter, without modifying anything.
  \COUNTERMEASURE \N Compiler and caller ensure validity of the pointers.

  \ID SBSW_FRTSYN_FUNCTIONCALL_PDUINFO
  \DESCRIPTION Call of a function with pointer parameter where PduInfo is a reference to a local (stack) object. SduLength is set to 16 and SduPtr is a reference to a local array with 16 elements.
  \COUNTERMEASURE \N Compiler ensures validity of the pointers.

  \ID SBSW_FRTSYN_PDUPTR_LENGTH_PARAM
  \DESCRIPTION Passing a reference to the 3rd element of PduPtr->SduLength that was given as parameter. The size of the object referenced by SduPtr is checked in FrTSyn_TriggerTransmit and the current function.
  \COUNTERMEASURE \N FrTSyn_TriggerTransmit ensures validity of the pointers and size of the array.

  \ID SBSW_FRTSYN_ACCESS_CONTROLLERSTATE_AND_LOCALPTR
  \DESCRIPTION Write access to ControllerState or referencing from ControllerState using ComStackLib. The index comes from the caller calculated by indirection of the Master/Slave config.
               The index is given as parameter on external interfaces or by iterating over the masters/slaves in the function. And call of a function with pointer parameters
               where the pointers are pointing to local (stack) objects.
  \COUNTERMEASURE \N Qualified use-case CSL02 and CSL03 of ComStackLib and compiler ensures validity of the pointers

  \ID SBSW_FRTSYN_ACCESS_FRMSG
  \DESCRIPTION Write access to FR message of type FrTSyn_FrMsgType (array with 16 elements) using the FrTSyn_FrSignal_* macros or referencing an element with a static index. The macros only access index 0..7. The message is a reference to a local (stack) object.
  \COUNTERMEASURE \N Compiler ensures validity of the pointers.

  \ID SBSW_FRTSYN_VERSIONINFOPTR
  \DESCRIPTION The function FrTSyn_GetVersionInfo writes to the object referenced by parameter versioninfo.
  \COUNTERMEASURE \N The caller ensures that the pointers passed to the
                     parameters versioninfo is valid.

SBSW_JUSTIFICATION_END */

/* VCA_JUSTIFICATION_BEGIN

  \ID VCA_FRTSYN_CALL_EXTERNAL_FUNCTION_WITH_CONST_POINTER_ARGUMENT
  \DESCRIPTION A function with pointer parameters is directly called, but the function is not
               defined within the analyzed sources. VCA is unable to determine the
               behavior of the function.
  \COUNTERMEASURE \N The Compiler ensures the validity of the pointers because the pointer parameter is a constant pointer.

  \ID VCA_FRTSYN_CALL_FRIF_FUNCTION_WITH_VAR_POINTER_ARGUMENT
  \DESCRIPTION A function with pointer parameters is directly called, but the function is not
               defined within the analyzed sources. VCA is unable to determine the
               behavior of the function.
  \COUNTERMEASURE \N Arguments that contain var pointer are checked by review: Pointer type corresponds to function
                     parameter type. If the pointer argument is a struct, this struct does not contain a pointer element.

  \ID VCA_FRTSYN_CALL_STBM_FUNCTION_WITH_VAR_POINTER_ARGUMENT
  \DESCRIPTION A function with pointer parameters is directly called, but the function is not
               defined within the analyzed sources. VCA is unable to determine the
               behavior of the function.
  \COUNTERMEASURE \N Arguments that contain var pointer are checked by review: Pointer type corresponds to function
                     parameter type. If the pointer argument is a struct, this struct does not contain a pointer element.

  \ID VCA_FRTSYN_CALL_VSTDLIB_FUNCTION_WITH_VAR_POINTER_ARGUMENT
  \DESCRIPTION A function with pointer parameters is directly called, but the function is not
               defined within the analyzed sources. VCA is unable to determine the
               behavior of the function.
  \COUNTERMEASURE \N Arguments that contain var pointer are checked by review: Pointer type corresponds to function
                     parameter type. If the pointer argument is a struct, this struct does not contain a pointer element.
                     The message is copied byte by byte depending on a size given by the third parameter.

  \ID VCA_FRTSYN_ACCESS_TXDEBOUNCECOUNTER_IND
  \DESCRIPTION Access of debounce counter via indirection over master
  \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID VCA_FRTSYN_VALID_SLAVEIDX
  \DESCRIPTION The slaveIdx is determined within the function FrTSyn_Rx_FindSlave() and validated by an
               additional variable errorId. ErrorId and slaveIdx is checked by a separate VCA assertion within
               FrTSyn_Rx_FindSlave().
  \COUNTERMEASURE \N No countermeasure is necessary. The described assertion within FrTSyn_Rx_FindSlave() should
                     hold.

VCA_JUSTIFICATION_END */

/* COV_JUSTIFICATION_BEGIN

  \ID COV_FRTSYN_MSR_MISRA
  \ACCEPT XX
  \REASON [COV_MSR_MISRA]

  --- Preprocessor Coverage Justifications ------------------------------------------------------------------------------

  \ID COV_FRTSYN_MSR_COMPATIBILITY
  \ACCEPT TX
  \REASON [COV_MSR_COMPATIBILITY]

  \ID COV_FRTSYN_NOUNIT_SWITCH
    \ACCEPT TX
    \REASON This switch is only used for unit testing and always true for normal operation.

COV_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: FrTSyn.c
 *********************************************************************************************************************/
