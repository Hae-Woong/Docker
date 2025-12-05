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
/*!        \file  EthTSyn_Types_Int.h
 *         \unit  *
 *        \brief  EthTSyn internal Types header
 *      \details  Contains all internal type definitions used by different sub-modules of the EthTSyn. Also provides
 *                all type definitions from EthTSyn_Types.h.
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

#if !defined(ETHTSYN_TYPES_INT_H)
# define ETHTSYN_TYPES_INT_H
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "EthTSyn_Types.h"
# include "EthTSyn_GeneratedComplexTypes.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Type sizes in Bytes */
# define ETHTSYN_PHYS_ADDR_LEN                                        (6u)
# define ETHTSYN_CLOCK_IDENTITY_SIZE                                  (8u)
# define ETHTSYN_CORRECTION_FIELD_SIZE                                (8u)
# define ETHTSYN_PORT_NUMBER_SIZE                                     (2u)
# define ETHTSYN_SEQUENCE_ID_SIZE                                     (2u)
# define ETHTSYN_TIMESTAMP_SECONDS_SIZE                               (6u)
# define ETHTSYN_TIMESTAMP_NANOSECONDS_SIZE                           (4u)

/* Message field lengths in Bytes */
# define ETHTSYN_ORGANISATION_ID_SIZE                                 (3u)
# define ETHTSYN_ORGANISATION_SUB_TYPE_SIZE                           (3u)
# define ETHTSYN_MSG_TLV_TYPE_FIELD_SIZE                              (2u)
# define ETHTSYN_MSG_TLV_LEN_FIELD_SIZE                               (2u)

# define ETHTSYN_COM_HDR_FLAGS_SIZE                                   (2u)
# define ETHTSYN_COM_HDR_MSG_LEN_SIZE                                 (2u)
# define ETHTSYN_COM_HDR_RESERVED_1_SIZE                              (4u)

# define ETHTSYN_ANNOUNCE_RESERVED_0_SIZE                             (10u)
# define ETHTSYN_ANNOUNCE_CURR_UTC_OFS_SIZE                           (2u)
# define ETHTSYN_ANNOUNCE_GM_CLK_QUAL_OFS_SCALED_LOG_VAR_SIZE         (2u)
# define ETHTSYN_ANNOUNCE_STEPS_REMOVED_SIZE                          (2u)

# define ETHTSYN_SYNC_RESERVED_SIZE                                   (10u)

# define ETHTSYN_FUP_CUM_SCALED_RATE_OFS_SIZE                         (4u)
# define ETHTSYN_FUP_GM_TIME_BASE_INDICATOR_SIZE                      (2u)
# define ETHTSYN_FUP_LAST_GM_PHASE_CHG_SIZE                           (12u)
# define ETHTSYN_FUP_SCALED_LAST_GM_FREQ_CHG_SIZE                     (4u)

# define ETHTSYN_PD_REQ_RESERVED_0_SIZE                               (10u)
# define ETHTSYN_PD_REQ_RESERVED_1_SIZE                               (10u)

/* Ar Tlv values */
# define ETHTSYN_AR_TLV_ORGANIZATION_ID                               (0x1A75FBu)
# define ETHTSYN_AR_TLV_ORGANIZATION_SUB_TYPE                         (0x605676u)

# define ETHTSYN_AR_TLV_TYPE                                          (0x0003u)
# define ETHTSYN_AR_SUB_TLV_TYPE_TIME_SECURED                         (0x28u)
# define ETHTSYN_AR_SUB_TLV_TYPE_STATUS_SECURED                       (0x50u)
# define ETHTSYN_AR_SUB_TLV_TYPE_STATUS_NOT_SECURED                   (0x51u)
# define ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_SECURED                    (0x60u)
# define ETHTSYN_AR_SUB_TLV_TYPE_USER_DATA_NOT_SECURED                (0x61u)
# define ETHTSYN_AR_SUB_TLV_TYPE_OFS_SECURED                          (0x44u)
# define ETHTSYN_AR_SUB_TLV_TYPE_OFS_NOT_SECURED                      (0x34u)

# define ETHTSYN_AR_TLV_HEADER_TYPE_LENGTH_FIELDS_LENGTH              (4u) /* Length in byte of tlvType and lengthField */
# define ETHTSYN_AR_TLV_HEADER_LENGTH_FIELD_VALUE                     (6u) /* Does not include the Sub-Tlvs length */
# define ETHTSYN_AR_SUB_TLV_TIME_LENGTH_FIELD_VALUE                   (3u)
# define ETHTSYN_AR_SUB_TLV_STATUS_LENGTH_FIELD_VALUE                 (2u)
# define ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH_FIELD_VALUE              (5u)
# define ETHTSYN_AR_SUB_TLV_OFS_LENGTH_FIELD_VALUE                    (17u)

/* The actual AR Tlv header length is the value of its length field (6) + the type (2) and length field itself (2) */
# define ETHTSYN_AR_TLV_HEADER_LENGTH                                 (10u)
/* The actual length of an Ar SubTlv is the value of its length field + the type (1) and the length field itself (1) */
# define ETHTSYN_AR_SUB_TLV_ABSTRACT_HDR_LENGTH                       (2u)
# define ETHTSYN_AR_SUB_TLV_TIME_LENGTH                               (5u)
# define ETHTSYN_AR_SUB_TLV_STATUS_LENGTH                             (4u)
# define ETHTSYN_AR_SUB_TLV_USER_DATA_LENGTH                          (7u)
# define ETHTSYN_AR_SUB_TLV_OFS_LENGTH                                (19u)

/* EthTSyn Follow_Up AR Sub TLV Status Secured/Not Secured */
# define ETHTSYN_AR_SUB_TLV_STATUS_SGW_MASK                           (0x01u)
# define ETHTSYN_AR_SUB_TLV_STATUS_SGW_POS                            (0u)

# define ETHTSYN_AR_SUB_TLV_STATUS_SGW_SYNC_TO_GTM                    (0x00u)
# define ETHTSYN_AR_SUB_TLV_STATUS_SGW_SYNC_TO_SUB_DOMAIN             (0x01u)

/* Invalid values */
# define ETHTSYN_INVALID_SWITCH_PORT_IDX                              (0xFFu)
# define ETHTSYN_INVALID_ETHIF_SWITCH_IDX                             (0xFFu)
# define ETHTSYN_SWT_MGMT_MSG_BUFFER_INVALID_IDX                      (0xFFu)

/* Max value for the correction field in NS. */
# define ETHTSYN_CORR_FIELD_MAX_VALUE_NS                              (0x0000FFFFFFFFFFFFu)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint16  EthTSyn_PortNumberType;
typedef uint8   EthTSyn_BufferIdxType;
typedef uint64  EthTSyn_ClockIdentityType;
typedef sint64  EthTSyn_TimediffType;
typedef float64 EthTSyn_RateCorrectedTimediffType;

typedef Eth_TimeIntDiffType EthTSyn_TimeIntDiffType;

typedef uint8 EthTSyn_PhysAddrType[ETHTSYN_PHYS_ADDR_LEN];

/* EthTSyn State Definitions */
typedef enum EthTSyn_StateEnum
{
  ETHTSYN_STATE_UNINIT = 0u,
  ETHTSYN_STATE_INIT
} EthTSyn_StateEnumType;

/* ----- Structures for de-serialized PTP messages ----- */
typedef struct
{
  uint64 SourceClockId;
  uint16 SourcePortNumber;
  uint8  DomainNumber;
} EthTSyn_MsgHdrPortInfoType;

typedef struct
{
  uint64 CorrectionFieldNs; /* Only 48Bits are used */
  uint16 CorrectionFieldSubNs;
  uint16 MessageLength;
  uint16 SequenceId;
  uint8  MessageType;
  sint8  LogMessageInterval;
  uint8  Flags0;
  uint8  Flags1;
  uint8  Control;
} EthTSyn_MsgHdrTypeSpecType;

typedef struct
{
  ETHTSYN_P2CONST(EthTSyn_MsgHdrType) PtpHdrPtr;
  ETHTSYN_P2CONST(uint8)              FupPayloadPtr;
                  uint16              SubTlvOfs;
} EthTSyn_ValidateSubTlvInfoType;

typedef struct
{
  EthTSyn_RxCrcValidatedEnumType RxCrcValidated;
  uint8                          TimeSecuredFlags;
} EthTSyn_CrcCfgInfoType;

typedef struct
{
  uint8 SubTlvType;
  uint8 SubTlvCrc;
} EthTSyn_SubTlvCrcInfoType;

typedef struct
{
  uint8 Time0Crc;
  uint8 Time1Crc;
} EthTSyn_TimeSecuredCrcType;

typedef enum
{
  ETHTSYN_TLV_SECURED,
  ETHTSYN_TLV_NOT_SECURED,
  ETHTSYN_TLV_UNKNOWN_TYPE
} EthTSyn_TlvSecuredInfo;

/*! Internal timestamp struct definition (used for arithmetic) */
typedef struct
{
  uint64 seconds;
  uint32 nanoseconds;
} EthTSyn_InternalTimestampType;

/*! Port identity struct definition */
typedef struct EthTSyn_PortIdentityTypeStruct
{
  EthTSyn_ClockIdentityType ClockIdentity;
  EthTSyn_PortNumberType    PortNumber;
} EthTSyn_PortIdentityType;

typedef enum
{
  ETHTSYN_TS_DIR_TX,
  ETHTSYN_TS_DIR_RX
} EthTSyn_TsDirType;

typedef struct
{
  boolean IsOfsCorrNeeded;
  boolean IsRateRatioCorrNeeded;
} EthTSyn_TimeCorrRequiredType;

typedef enum
{
  ETHTSYN_CORR_ACTION_NONE,
  ETHTSYN_CORR_ACTION_OFS,
  ETHTSYN_CORR_ACTION_OFS_GM_RATE,
  ETHTSYN_CORR_ACTION_RATE_REGULATOR
} EthTSyn_CorrActionType;

/*! Internally used Ethernet tx buffer struct definition */
/* spec strong type invariant () { $lengthOf(self.BufferPtr) >= self.Length } endspec */
typedef struct
{
  ETHTSYN_P2VAR(uint8) BufferPtr; /*!< Pointer to the payload portion of the Ethernet Tx buffer.
                                       Valid when BufferIdx is valid. */
  uint16               Length;    /*!< The requested Ethernet Tx buffer length. */
  uint8                BufferIdx; /*!< Index of the Ethernet Tx buffer */
} EthTSyn_EthTxBufferStructType;

typedef enum
{
  ETHTSYN_MSG_TX_STATE_PENDING, /*!< Message transmission is pending. */
  ETHTSYN_MSG_TX_STATE_FINISHED /*!< Transmission finished.*/
} EthTSyn_MsgTxStateReturnType;

typedef struct
{
  uint16  RelOfs;            /*!< Offset to the start of the optional TimeSecured TLV relative to the payload */
  boolean CrcUpdateRequired; /*!< Indicating if an update of the TimeSecured TLV CRC is required */
  boolean CrcValid;          /*!< Indicating if the CRC of a received TimeSecured TLV is valid */
} EthTSyn_TimeTlvInfoType;

#endif /* ETHTSYN_TYPES_INT_H */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_Types_Int.h
 *********************************************************************************************************************/
