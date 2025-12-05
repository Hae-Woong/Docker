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
/*!        \file  SoAd_Types.h
 *        \brief  Socket Adaptor types header file
 *
 *      \details  Vector static types header file for AUTOSAR Socket Adaptor module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined(SOAD_TYPES_H)
# define SOAD_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "SoAd_Cfg.h"
# include "SoAd_GenTypes.h"
# include "ComStack_Types.h"
# include "IpBase.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/* PRQA S 3453 FCT_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

# define SOAD_A_P2VAR(Type)           (P2VAR(Type, AUTOMATIC, AUTOMATIC))
# define SOAD_A_P2CONST(Type)         (P2CONST(Type, AUTOMATIC, AUTOMATIC))

# define SOAD_D_P2VAR(Type)           (P2VAR(Type, AUTOMATIC, SOAD_APPL_DATA))
# define SOAD_D_P2CONST(Type)         (P2CONST(Type, AUTOMATIC, SOAD_APPL_DATA))

# define SOAD_V_P2VAR(Type)           (P2VAR(Type, AUTOMATIC, SOAD_APPL_VAR))
# define SOAD_V_P2CONST(Type)         (P2CONST(Type, AUTOMATIC, SOAD_APPL_VAR))

# define SOAD_P2CONST(Type)           P2CONST(Type, AUTOMATIC, SOAD_APPL_DATA)
# define SOAD_P2VAR(Type)             P2VAR(Type, AUTOMATIC, SOAD_APPL_VAR)
# define SOAD_CONSTP2VAR(Type)        CONSTP2VAR(Type, AUTOMATIC, SOAD_APPL_VAR)
# define SOAD_CONSTP2CONST(Type)      CONSTP2CONST(Type, AUTOMATIC, SOAD_CONST)

/* PRQA L:FCT_LIKE_MACRO */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES DEFINES
 *********************************************************************************************************************/

/*! Physical address length. */
# define SOAD_UHWID_LEN_BYTE              TCPIP_PHYS_ADDR_LEN_BYTE

/*! IPv4 address length. */
# define SOAD_IPV4_ADDR_LEN_BYTE          (4u)
/*! IPv6 address length. */
# define SOAD_IPV6_ADDR_LEN_BYTE          (16u)
/*! IPv6 address length for 32 bit. */
# define SOAD_IPV6_ADDR_LEN_32            (4u)

/*! Invalid index for array of type uint16. */
# define SOAD_INV_BUF_IDX_16              (0xFFFFu)
/*! Invalid index for array of type uint32. */
# define SOAD_INV_BUF_IDX_32              (0xFFFFFFFFu)

/*! Invalid physical address value. */
# define SOAD_INV_PHYS_ADDR               (0u)

/*! Invalid value to indicate that no keep alive is configured. */
# define SOAD_INV_KEEP_ALIVE_VALUE        (0xFFFFFFFFu)

/*! Maximum value for type uint8. */
# define SOAD_MAX_UINT_8                  (0xFFu)
/*! Maximum value for type uint32 used for uint32 timeouts. */
# define SOAD_MAX_TIMEOUT_32              (0xFFFFFFFFu)

/*! Value to increment or decrement a variable by 1. */
# define SOAD_INC_DEC_BY_ONE              (1u)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

# if ( SOAD_MULTI_PARTITION == STD_ON ) /* COV_SOAD_QM_FEATURE_MULTI_PARTITION */
/*! Atomic uint8 data type. */
typedef struct sSoAd_AtomicUint8Type
{
  union
  { /* PRQA S 0750 */ /* MD_MSR_Union */
    uint8 Value;
    P2VAR(void, AUTOMATIC, AUTOMATIC) Reserved;
  } Aligned;
} SoAd_AtomicUint8Type;

/*! Atomic uint32 data type. */
typedef struct sSoAd_AtomicUint32Type
{
  union
  { /* PRQA S 0750 */ /* MD_MSR_Union */
    uint32 Value;
    P2VAR(void, AUTOMATIC, AUTOMATIC) Reserved;
  } Aligned;
} SoAd_AtomicUint32Type;
# else
/*! Atomic uint8 data type. */
typedef uint8 SoAd_AtomicUint8Type;

/*! Atomic uint32 data type. */
typedef uint32 SoAd_AtomicUint32Type;
# endif /*  SOAD_MULTI_PARTITION == STD_ON */

/*! Pointer to atomic uint8 data type. */
typedef volatile P2VAR(SoAd_AtomicUint8Type, AUTOMATIC, SOAD_APPL_VAR) SoAd_AtomicUint8PtrType;

/*! Pointer to constant atomic uint8 data type. */
typedef volatile P2CONST(SoAd_AtomicUint8Type, AUTOMATIC, SOAD_APPL_VAR) SoAd_AtomicUint8ConstPtrType;

/*! Pointer to atomic uint32 data type. */
typedef volatile P2VAR(SoAd_AtomicUint32Type, AUTOMATIC, SOAD_APPL_VAR) SoAd_AtomicUint32PtrType;

/*! Pointer to constant atomic uint32 data type. */
typedef volatile P2CONST(SoAd_AtomicUint32Type, AUTOMATIC, SOAD_APPL_VAR) SoAd_AtomicUint32ConstPtrType;

/*! Module state type. */
typedef uint8                                 SoAd_StateType;

/*! Partition state type. */
typedef SoAd_AtomicUint8Type                  SoAd_PartitionStateType;
# define SOAD_STATE_PARTITION_UNINIT          (0x00u)
# define SOAD_STATE_PARTITION_INIT            (0x01u)

/*! Maximum PDU length. */
# define SOAD_UINT8OR16                       (0u)
# define SOAD_UINT32                          (1u)

/*! Module internal return type used for Rx PDU verification. */
typedef uint8                                 SoAd_ReturnType;
# define SOAD_E_OK                            (0u)    /*!< Rx PDU verification succeeded. */
# define SOAD_E_NOT_OK                        (1u)    /*!< Rx PDU verification failed. */
# define SOAD_E_PENDING                       (2u)    /*!< Rx PDU verification pending. */

/*! Type to describe the different event queues. */
typedef uint8                                 SoAd_EventQueueEnumType;
# define SOAD_EVENT_QUEUE_IF_UDP_PDU_ROUTE    (0u)    /*!< Queue for socket connection with UDP IF Tx PDU. */
# define SOAD_EVENT_QUEUE_IF_TX_ROUTE_GRP     (1u)    /*!< Queue for socket connection with IF Tx RouteGrp. */
# define SOAD_EVENT_QUEUE_TP_TX_SO_CON        (2u)    /*!< Queue for socket connection with TP Tx PDU. */
# define SOAD_EVENT_QUEUE_TP_RX_SO_CON        (3u)    /*!< Queue for socket connection with TP Rx PDU. */
# define SOAD_EVENT_QUEUE_SO_CON_MODE_CHG     (4u)    /*!< Queue for socket connection (mode change) */
# define SOAD_EVENT_QUEUE_SO_CON_STATE        (5u)    /*!< Queue for socket connection (state) */

/*! EventQueue flag type used to indicate if a handle is already in list. */
typedef uint8                                 SoAd_EventQueueFlagType;

/*! Bit pattern type used to define the specific bit pattern of an EventQueue. */
typedef uint8                                          SoAd_EventQueueBitPatternType;
/*! Queue bit pattern for socket connection with UDP IF Tx PDU. */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_IF_UDP_PDU_ROUTE 0x01u
/*! Queue bit pattern for socket connection with IF Tx RouteGrp. */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_IF_TX_ROUTE_GRP  0x02u
/*! Queue bit pattern for socket connection with TP Tx PDU. */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_TP_TX_SO_CON     0x04u
/*! Queue bit pattern for socket connection with TP Rx PDU. */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_TP_RX_SO_CON     0x08u
/*! Queue bit pattern for socket connection (mode change). */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_SO_CON_MODE_CHG  0x10u
/*! Queue bit pattern for socket connection (state). */
# define SOAD_EVENT_QUEUE_BIT_PATTERN_SO_CON_STATE     0x20u

/*! Type containing specific Event Queue information. */
typedef struct sSoAd_EventQueueParamType
{
  uint32 EventQueueStartIdx;
  uint32 EventQueueEndIdx;
  uint32 EventQueueFlagOffset;
  uint32 EventQueueMgmtIdx;
  SoAd_EventQueueBitPatternType BitPattern;
} SoAd_EventQueueParamType;

/*! Type to describe the different timeout lists. */
typedef uint8                                 SoAd_TimeoutListEnumType;
# define SOAD_TIMEOUT_LIST_UDP_ALIVE          (0u)            /*!< List for UDP alive supervision timeout. */
# define SOAD_TIMEOUT_LIST_N_PDU              (1u)            /*!< List for nPdu timeout. */
# define SOAD_TIMEOUT_LIST_TCP_AUTO_CONNECT   (2u)            /*!< List for TCP auto connect timeout. */

/*! Socket state type. */
typedef uint8                                 SoAd_SockStateType;
# define SOAD_SOCK_STATE_ESTABLISHED          (0u)      /*!< Socket state established. */
# define SOAD_SOCK_STATE_CONNECT              (1u)      /*!< Socket state connecting to remote entity. */
# define SOAD_SOCK_STATE_LISTEN               (2u)      /*!< Socket state listen for incoming connections. */
# define SOAD_SOCK_STATE_CLOSING              (3u)      /*!< Socket state closing. */
# define SOAD_SOCK_STATE_CLOSED               (4u)      /*!< Socket state closed. */

/*! Socket connection identifier type. */
typedef uint16                                SoAd_SoConIdType;
# define SOAD_INV_SO_CON_ID                   (65535u)
/*! Invalid value for socket connection identifier type. */

/*! Socket connection mode type. */
typedef uint8                                 SoAd_SoConModeType;
# define SOAD_SOCON_ONLINE                    (0u)      /*!< Socket connection mode online. */
# define SOAD_SOCON_RECONNECT                 (1u)      /*!< Socket connection mode reconnect. */
# define SOAD_SOCON_OFFLINE                   (2u)      /*!< Socket connection mode offline. */

/*! Socket connection close mode type. */
typedef uint8                                 SoAd_SoConCloseModeType;
/*! Indicates that no close is requested for a socket connection. */
# define SOAD_CLOSE_NONE                      (0u)
/*! Indicates that a socket connection is requested to be closed by a socket close event and reconnect is required. */
# define SOAD_CLOSE_SOCKET_RECONNECT          (1u)
/*! Indicates that a socket connection is requested to be closed by a socket close event. */
# define SOAD_CLOSE_SOCKET                    (2u)
/*! Indicates that a socket connection is requested to be closed and set to reconnect. */
# define SOAD_CLOSE_RECONNECT                 (3u)
/*! Indicates that a socket connection is requested to be closed and set to offline by service call. */
# define SOAD_CLOSE_OFFLINE                   (4u)
/*! Indicates that a socket connection is requested to be closed and set to offline in case of any error. */
# define SOAD_CLOSE_OFFLINE_RESET             (5u)

/*! IPv4 IP address type. */
typedef IpBase_AddrInType                     SoAd_IpAddrInetType;

/*! IPv6 IP address type. */
typedef IpBase_AddrIn6Type                    SoAd_IpAddrInet6Type;

# if ( SOAD_IPV6 == STD_ON )
/*! Largest IP address struct in case of IPv6 in AUTOSAR format. */
  typedef SoAd_SockAddrInet6Type              SoAd_SockAddrInetXType;
# else
/*! Largest IP address struct in case of IPv4 in AUTOSAR format. */
  typedef SoAd_SockAddrInetType               SoAd_SockAddrInetXType;
# endif /* SOAD_IPV6 == STD_ON */

/*! Bit masks for routing group socket connection flags. */
# define SOAD_ROUTEGRPSOCON_MASK_NONE         0x00u /* 00 00 00 00 */ /*!< Bit mask for no flag. */
# define SOAD_ROUTEGRPSOCON_MASK_TRIGGERED    0x01u /* 00 00 00 01 */ /*!< Bit mask for flag triggered. */
# define SOAD_ROUTEGRPSOCON_MASK_SENT         0x02u /* 00 00 00 10 */ /*!< Bit mask for flag sent. */

/*! Type to indicate state (set, wildcard, not set) of remote address. */
typedef uint8                                 SoAd_RemAddrStateType;
                                                    /* --  NOT ANY SET */
# define SOAD_SOCON_IP_SET_PORT_SET           0x03u /* 00  00  00  11  */ /*!< IP address set, port set. */
# define SOAD_SOCON_IP_SET_PORT_ANY           0x06u /* 00  00  01  10  */ /*!< IP address set, port wildcard. */
# define SOAD_SOCON_IP_SET_PORT_NOT           0x12u /* 00  01  00  10  */ /*!< IP address set, port not set. */
# define SOAD_SOCON_IP_ANY_PORT_SET           0x09u /* 00  00  10  01  */ /*!< IP address wildcard, port set. */
# define SOAD_SOCON_IP_ANY_PORT_ANY           0x0Cu /* 00  00  11  00  */ /*!< IP address wildcard, port wildcard. */
# define SOAD_SOCON_IP_ANY_PORT_NOT           0x18u /* 00  01  10  00  */ /*!< IP address wildcard, port not set. */
# define SOAD_SOCON_IP_NOT_PORT_SET           0x21u /* 00  10  00  01  */ /*!< IP address not set, port set. */
# define SOAD_SOCON_IP_NOT_PORT_ANY           0x24u /* 00  10  01  00  */ /*!< IP address not set, port wildcard. */
# define SOAD_SOCON_IP_NOT_PORT_NOT           0x30u /* 00  11  00  00  */ /*!< IP address not set, port not set. */
# define SOAD_SOCON_MASK_IPADDR               0x2Au /* 00  10  10  10  */ /*!< Bit mask for IP address state. */
# define SOAD_SOCON_MASK_PORT                 0x15u /* 00  01  01  01  */ /*!< Bit mask for port state. */
# define SOAD_SOCON_MASK_SET                  0x03u /* 00  00  00  11  */ /*!< Bit mask for state set. */
# define SOAD_SOCON_MASK_ANY                  0x0Cu /* 00  00  11  00  */ /*!< Bit mask for state is wildcard. */
# define SOAD_SOCON_MASK_NOT                  0x30u /* 00  11  00  00  */ /*!< Bit mask for state not set. */

/*! Remote address release state type. */
typedef uint8                                 SoAd_RemAddrReleaseStateType;
/*! Indicates that no release of remote address is requested for a socket connection. */
# define SOAD_RELEASE_NONE                    (0u)
/*! Indicates that the release of remote address is requested for a socket connection. */
# define SOAD_RELEASE_REM_ADDR                (1u)
/*! Indicates that the release of remote address is forced for a socket connection. */
# define SOAD_RELEASE_REM_ADDR_FORCE          (2u)

/*! Socket port type. */
typedef uint16                                SoAd_PortType;
# define SOAD_PORT_ANY                        (0x0000u)                 /*!< Any/wildcard value for port. */

/*! Best match algorithm priority. */
typedef uint8                                 SoAd_BestMatchPrioType;
# define SOAD_BEST_MATCH_IP_MATCH_PORT_MATCH  (4u)                      /*!< IP and port match. */
# define SOAD_BEST_MATCH_IP_MATCH_PORT_ANY    (3u)                      /*!< IP match and port wildcard. */
# define SOAD_BEST_MATCH_IP_ANY_PORT_MATCH    (2u)                      /*!< IP wildcard and port match. */
# define SOAD_BEST_MATCH_IP_ANY_PORT_ANY      (1u)                      /*!< IP and port wildcard. */
# define SOAD_BEST_MATCH_NONE                 (0u)                      /*!< No match. */

/*! TCP no delay type to indicate if enabled, disabled or not specified. */
typedef uint8                                 SoAd_TcpNoDelayType;
# define SOAD_TCP_NO_DELAY_ENABLED            (0u)                      /*!< TCP no delay is enabled. */
# define SOAD_TCP_NO_DELAY_DISABLED           (1u)                      /*!< TCP no delay is disabled. */
# define SOAD_TCP_NO_DELAY_NOT_SPECIFIED      (2u)                      /*!< TCP no delay is not specified. */

/*! Any/wildcard value for IP address. */
# define SOAD_IPADDR_ANY                      (0x00000000u)

/*! PDU header identifier type. */
typedef uint32                                SoAd_PduHdrIdType;
/*! PDU header length type. */
typedef uint32                                SoAd_PduHdrLenType;
/*! PDU header size. */
# define SOAD_PDU_HDR_SIZE                    (8u)
/*! PDU header identifier size. */
# define SOAD_PDU_HDR_ID_SIZE                 (4u)

/*! NPdu UDP Tx state type indicating the current state of nPdu transmission. */
typedef uint8                                 SoAd_NPduUdpTxStateType;
# define SOAD_NPDU_UDP_TX_STATE_NONE          (0u)      /*!< No nPdu transmission active. */
# define SOAD_NPDU_UDP_TX_STATE_ACTIVE        (1u)      /*!< NPdu transmission is active. */
# define SOAD_NPDU_UDP_TX_STATE_FAILED        (2u)      /*!< NPdu transmission failed. */

/*! NPdu UDP Tx return type used in nPdu related functions to add elements to buffer or queue. */
typedef uint8                                 SoAd_NPduReturnType;
# define SOAD_NPDU_OK                         (0u)      /*!< Added or updated element. */
# define SOAD_NPDU_OVFL_NEW                   (1u)      /*!< New PDU does not fit into nPdu. */
# define SOAD_NPDU_OVFL_UPDATE                (2u)      /*!< Updated PDU does not fit into nPdu. */
# define SOAD_NPDU_NOT_OK                     (3u)      /*!< Did not add or update nPdu. */

/*! Routing group identifier type . */
typedef uint16                                SoAd_RoutingGroupIdType;
# define SOAD_INV_ROUT_GRP_ID                 (65535u)

/*! Upper layer type. */
typedef uint8                                 SoAd_UpperLayerApiType;
# define SOAD_UL_API_TP                       (1u)      /*!< Upper layer API is TP. */
# define SOAD_UL_API_IF                       (0u)      /*!< Upper layer API is IF. */

/*! Measurement data handling for SOME/IP SD data. */
typedef uint8                                 SoAd_SomeIpSdMeasurementDataType;
# define SOAD_SOME_IP_MEAS_NONE               (0u)      /*! No measurement of SOME/IP SD data. */
# define SOAD_SOME_IP_MEAS_SOME_IP            (1u)      /*! Counted into SOME/IP measurement data. */
# define SOAD_SOME_IP_MEAS_SOME_IP_SD         (2u)      /*! Counted into SOME/IP SD measurement data. */

/*! Measurement flag type to indicate if the reset of a counter is pending. */
typedef uint8                                 SoAd_MeasurementFlagResetType;
# define SOAD_MEAS_FLAG_RESET_NOT_PENDING     (0u)    /*!< Reset of measurement counter is not pending. */
# define SOAD_MEAS_FLAG_RESET_PENDING         (1u)    /*!< Reset of measurement counter is pending. */

/* Mask values to get the service and method identifier of a PDU header identifier. */
# define SOAD_MASK_SERVICE_ID                 (0xFFFF0000u) /*!< Bit mask for service identifier. */
# define SOAD_MASK_METHOD_ID                  (0xFFFFu)     /*!< Bit mask for method identifier. */

/*! Trigger mode type for UDP Tx PDUs. */
typedef uint8                                 SoAd_TxUdpTriggerModeType;
# define SOAD_TX_UDP_TRIGGER_ALWAYS           (0u)      /*!< Triggers transmission with transmit request. */
# define SOAD_TX_UDP_TRIGGER_NEVER            (1u)      /*!< Does not trigger transmission with transmit request. */

/*! Trigger transmit mode type for IF PDUs. */
typedef uint8                                 SoAd_IfTriggerTransmitModeType;
# define SOAD_IF_TRIGGER_TRANSMIT_MODE_SINGLE (0u)      /*!< Trigger transmit must be called only once. */
# define SOAD_IF_TRIGGER_TRANSMIT_MODE_MULTI  (1u)      /*!< Trigger transmit can be called multiple times. */
# define SOAD_IF_TRIGGER_TRANSMIT_MODE_NONE   (2u)      /*!< No trigger transmit configured. */

/*! Socket connection mode filter type used in best match algorithm. */
typedef uint8                                 SoAd_SoConModeFilterType;
# define SOAD_SOCON_FILTER_DISABLED           (0u)      /*!< No filter active. */
# define SOAD_SOCON_FILTER_ON_OFF             (1u)      /*!< Consider online or offline only. */

/*! DHCP hostname option for IPv4. */
# define SOAD_DHCP_OPT_V4_HOSTNAME            (81u)
/*! DHCP hostname option for IPv6. */
# define SOAD_DHCP_OPT_V6_HOSTNAME            (39u)

/*! Pointer to constant IP address. */
typedef P2CONST(uint32, AUTOMATIC, SOAD_CONST)  SoAd_IpAddrConstPtrType;

/*! Measure counter max value. */
# define SOAD_MEASURE_COUNTER_MAX_VALUE         65535u

/*! Index to select specific measurement data. */
typedef uint8 SoAd_MeasurementIdxType;
# define SOAD_MEAS_DROP_TCP                      0x01u
# define SOAD_MEAS_DROP_UDP                      0x02u
# define SOAD_MEAS_DROP_TCP_CONNECTION           0x80u
# define SOAD_MEAS_DROP_UDP_SOCKET               0x81u
# define SOAD_MEAS_DROP_UDP_LENGTH               0x82u
# define SOAD_MEAS_INVALID_SOME_IP_SERVICE_ID    0x83u
# define SOAD_MEAS_INVALID_SOME_IP_METHOD_ID     0x84u
# define SOAD_MEAS_INVALID_SOME_IP_SD_SERVICE_ID 0x85u
# define SOAD_MEAS_INVALID_SOME_IP_SD_METHOD_ID  0x86u
# define SOAD_MEAS_ALL                           0xFFu

typedef P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) SoAd_DataPtrType;

/*! Function pointer to <Up>_[SoAd][If]RxIndication. */
typedef P2FUNC(void, SOAD_CODE, SoAd_IfRxIndicationCbkType) (
  PduIdType RxPduId,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr);

/*! Function pointer to <Up>_[SoAd][Tp]StartOfReception. */
typedef P2FUNC(BufReq_ReturnType, SOAD_CODE, SoAd_TpStartOfReceptionCbkType) (
  PduIdType RxPduId,
  CONSTP2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) info,
  PduLengthType TpSduLength,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) bufferSizePtr);

/*! Function pointer to <Up>_[SoAd][Tp]StartOfReception with const data pointer. */
typedef P2FUNC(BufReq_ReturnType, SOAD_CODE, SoAd_TpStartOfReceptionConstCbkType) (
  PduIdType RxPduId,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) info,
  PduLengthType TpSduLength,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) bufferSizePtr);

/*! Function pointer to <Up>_[SoAd][Tp]CopyRxData. */
typedef P2FUNC(BufReq_ReturnType, SOAD_CODE, SoAd_TpCopyRxDataCbkType) (
  PduIdType id,
  CONSTP2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) info,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) bufferSizePtr);

/*! Function pointer to <Up>_[SoAd][Tp]CopyRxData with const data pointer. */
typedef P2FUNC(BufReq_ReturnType, SOAD_CODE, SoAd_TpCopyRxDataConstCbkType) (
  PduIdType id,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) info,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) bufferSizePtr);

/*! Function pointer to <Up>_[SoAd][Tp]RxIndication. */
typedef P2FUNC(void, SOAD_CODE, SoAd_TpRxIndicationCbkType) (
  PduIdType RxPduId,
  Std_ReturnType result);

/*! Function pointer to <Up>_[SoAd][If]TriggerTransmit. */
typedef P2FUNC(Std_ReturnType, SOAD_CODE, SoAd_IfTriggerTransmitCbkType) (
  PduIdType TxPduId,
  CONSTP2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr);

/*! Function pointer to <Up>_[SoAd][If]TxConfirmation. */
typedef P2FUNC(void, SOAD_CODE, SoAd_IfTxConfirmationCbkType) (
  PduIdType TxPduId);

/*! Function pointer to <Up>_[SoAd][Tp]CopyTxData. */
typedef P2FUNC(BufReq_ReturnType, SOAD_CODE, SoAd_TpCopyTxDataCbkType) (
  PduIdType id,
  CONSTP2VAR(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) info,
  CONSTP2VAR(RetryInfoType, AUTOMATIC, SOAD_APPL_DATA) retry,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) availableDataPtr);

/*! Function pointer to <Up>_[SoAd][Tp]CopyTxData with const data pointer. */
typedef P2FUNC(BufReq_ReturnType, SOAD_CODE, SoAd_TpCopyTxDataConstCbkType) (
  PduIdType id,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) info,
  CONSTP2VAR(RetryInfoType, AUTOMATIC, SOAD_APPL_DATA) retry,
  CONSTP2VAR(PduLengthType, AUTOMATIC, SOAD_APPL_DATA) availableDataPtr);

/*! Function pointer to <Up>_[SoAd][Tp]TxConfirmation. */
typedef P2FUNC(void, SOAD_CODE, SoAd_TpTxConfirmationCbkType) (
  PduIdType TxPduId,
  Std_ReturnType result);

/*! Function pointer to <Up>_SoConModeChg. */
typedef P2FUNC(void, SOAD_CODE, SoAd_SoConModeChgCbkType) (
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode);

/*! Function pointer to <Up>_LocalIpAddrAssignmentChg. */
typedef P2FUNC(void, SOAD_CODE, SoAd_LocalIpAddrAssignmentChgCbkType) (
  SoAd_SoConIdType SoConId,
  SoAd_IpAddrStateType State);

/*! Function pointer to <Up>_ShutdownFinished. */
typedef P2FUNC(void, SOAD_CODE, SoAd_ShutdownFinishedCbkType) (void);

/*! Function pointer to <Up_VerifyRxPdu>. */
typedef P2FUNC(Std_ReturnType, SOAD_CODE, SoAd_VerifyRxPduCbkType) (
  CONSTP2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) LocalAddrPtr,
  CONSTP2CONST(SoAd_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  SoAd_PduHdrIdType PduHdrId,
  CONSTP2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) PduInfoPtr);

/*! Function pointer to <Up>_DhcpEvent. */
typedef P2FUNC(void, SOAD_CODE, SoAd_DhcpEventCbkType) (
  SoAd_LocalAddrIdType IpAddrId,
  SoAd_DhcpEventType Event);

#endif /* !defined(SOAD_TYPES_H) */

/**********************************************************************************************************************
 *  END OF FILE: SoAd_Types.h
 *********************************************************************************************************************/
