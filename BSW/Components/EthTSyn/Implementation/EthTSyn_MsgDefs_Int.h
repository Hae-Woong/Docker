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
/*!        \file  EthTSyn_MsgDefs_Int.h
 *         \unit  *
 *        \brief  EthTSyn internal header for PTP message defines
 *      \details  Contains all PTP message related defines used for serialization and deserialization of messages
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

#if !defined(ETHTSYN_MSG_DEFS_INT_H)
# define ETHTSYN_MSG_DEFS_INT_H
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

 /* EthTSyn message lengths in Byte */
# define ETHTSYN_MSG_HDR_LEN                                          (34u)

# define ETHTSYN_MSG_SYNC_PAYLOAD_LEN                                 (10u)
# define ETHTSYN_MSG_SYNC_LEN                                         (ETHTSYN_MSG_HDR_LEN +\
                                                                       ETHTSYN_MSG_SYNC_PAYLOAD_LEN)

# define ETHTSYN_MSG_FUP_PAYLOAD_LEN                                  (42u)
# define ETHTSYN_MSG_FUP_LEN                                          (ETHTSYN_MSG_HDR_LEN +\
                                                                       ETHTSYN_MSG_FUP_PAYLOAD_LEN)

/* The announce message payload length already includes a PathTraceTlv with one PathTraceElem. */
# define ETHTSYN_MSG_ANNOUNCE_PATH_SEQUENCE_ELEMENT_LENGTH            (8u)
# define ETHTSYN_MSG_ANNOUNCE_PAYLOAD_LEN                             (42u)
# define ETHTSYN_MSG_ANNOUNCE_LEN                                     (ETHTSYN_MSG_HDR_LEN +\
                                                                       ETHTSYN_MSG_ANNOUNCE_PAYLOAD_LEN)
# define ETHTSYN_MSG_ANNOUNCE_LEN_WO_PATH_TRACE_ELEM                  (ETHTSYN_MSG_ANNOUNCE_LEN -\
                                                                       ETHTSYN_MSG_ANNOUNCE_PATH_SEQUENCE_ELEMENT_LENGTH)

# define ETHTSYN_MSG_PD_PAYLOAD_LEN                                   (20u)

# define ETHTSYN_MSG_PD_REQ_PAYLOAD_LEN                               (ETHTSYN_MSG_PD_PAYLOAD_LEN)
# define ETHTSYN_MSG_PD_REQ_LEN                                       (ETHTSYN_MSG_HDR_LEN +\
                                                                       ETHTSYN_MSG_PD_REQ_PAYLOAD_LEN)

# define ETHTSYN_MSG_PD_RESP_PAYLOAD_LEN                              (ETHTSYN_MSG_PD_PAYLOAD_LEN)
# define ETHTSYN_MSG_PD_RESP_LEN                                      (ETHTSYN_MSG_HDR_LEN +\
                                                                       ETHTSYN_MSG_PD_RESP_PAYLOAD_LEN)

# define ETHTSYN_MSG_PD_RESP_FUP_PAYLOAD_LEN                          (ETHTSYN_MSG_PD_PAYLOAD_LEN)
# define ETHTSYN_MSG_PD_RESP_FUP_LEN                                  (ETHTSYN_MSG_HDR_LEN +\
                                                                       ETHTSYN_MSG_PD_RESP_FUP_PAYLOAD_LEN)

/* EthTSyn message header fields sizes and offsets in byte */
# define ETHTSYN_MSG_COM_HDR_MSG_LEN_SIZE                             (2u)
# define ETHTSYN_MSG_COM_HDR_SEQ_ID_SIZE                              (2u)
# define ETHTSYN_MSG_COM_HDR_SRC_PORT_ID_SIZE                         (10u)
# define ETHTSYN_MSG_COM_HDR_CORR_FIELD_SIZE                          (8u)

/* EthTSyn Message Types */
/* Note: Signaling message is not supported. The define is just provided here because it is specified in IEEE802.1As */
# define ETHTSYN_MSG_TYPE_ANNOUNCE                                    (0x0Bu)
# define ETHTSYN_MSG_TYPE_SIGNALING                                   (0x0Cu)
# define ETHTSYN_MSG_TYPE_SYNC                                        (0x00u)
# define ETHTSYN_MSG_TYPE_FUP                                         (0x08u)
# define ETHTSYN_MSG_TYPE_PD_REQ                                      (0x02u)
# define ETHTSYN_MSG_TYPE_PD_RESP                                     (0x03u)
# define ETHTSYN_MSG_TYPE_PD_RESP_FUP                                 (0x0Au)

/* EthTSyn Transport Specific field */
/* First nibble of octet -> value is 1u */
# define ETHTSYN_MSG_HDR_TRANSPORT_SPECIFIC                           (0x10u)

/* EthTSyn PTP Version field */
/* First nibble of octet is reserved (i.e. 0) */
# define ETHTSYN_PTP_VERSION                                          (0x02u)

/* EthTSyn Control field */
# define ETHTSYN_MSG_HDR_CONTROL_SYNC                                 (0x00u)
# define ETHTSYN_MSG_HDR_CONTROL_FOLLOW_UP                            (0x02u)
# define ETHTSYN_MSG_HDR_CONTROL_ANNOUNCE                             (0x05u)
# define ETHTSYN_MSG_HDR_CONTROL_PDELAY_REQ                           (0x05u)
# define ETHTSYN_MSG_HDR_CONTROL_PDELAY_RESP                          (0x05u)
# define ETHTSYN_MSG_HDR_CONTROL_PDELAY_RESP_FUP                      (0x05u)

/* EthTSyn Flags field */
# define ETHTSYN_MSG_HDR_FLAGS_0_DEFAULT                              (0x00u)
# define ETHTSYN_MSG_HDR_FLAGS_0_SYNC_PDELAY_RESP                     (0x02u)
# define ETHTSYN_MSG_HDR_FLAGS_1_DEFAULT                              (ETHTSYN_FLAGS_OCTET_1_DEFAULT_VALUE)

/* EthTSyn correction field */
# define ETHTSYN_MSG_HDR_COR_FIELD_DEFAULT                            (0u)
# define ETHTSYN_MSG_HDR_COR_FIELD_SUB_NS_DEFAULT                     (0u)
# define ETHTSYN_MSG_HDR_CORR_FIELD_MAX_NS                            (0x0000FFFFFFFFFFFFuLL)

/* EthTSyn Log message interval */
# define ETHTSYN_MSG_HDR_LOG_MSG_INTERVAL_DEFAULT                     (0x7F)

/* Follow Up information TLV */
# define ETHTSYN_FUP_INFO_TLV_TLV_TYPE_DEFAULT                        (0x0003u)
# define ETHTSYN_FUP_INFO_TLV_LENGTH_DEFAULT                          (28u)
# define ETHTSYN_FUP_INFO_TLV_ORG_ID_DEFAULT                          (0x0080C2u)
# define ETHTSYN_FUP_INFO_TLV_ORG_SUBTYPE_DEFAULT                     (1u)
# define ETHTSYN_FUP_INFO_TLV_GM_TIME_BASE_IND_DEFAULT                (0u)
# define ETHTSYN_FUP_INFO_TLV_LAST_GM_PHASE_CHG_DEFAULT               (0u)
# define ETHTSYN_FUP_INFO_TLV_SCALED_LAST_GM_FREQ_CHG_DEFAULT         (0u)

/* TimeBaseStatus Masks */
# define ETHTSYN_TIME_BASE_STATUS_TIMEOUT_MASK                        (0x01u)
# define ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_MASK                     (0x04u)
# define ETHTSYN_TIME_BASE_STATUS_SYNC_TO_GW_SHIFT                    (2u)
# define ETHTSYN_TIME_BASE_STATUS_GLOBAL_TIME_BASE_MASK               (0x08u)
# define ETHTSYN_TIME_BASE_STATUS_TIMELEAP_FUTURE_MASK                (0x10u)
# define ETHTSYN_TIME_BASE_STATUS_TIMELEAP_PAST_MASK                  (0x20u)


/**********************************************************************************************************************
 *  MESSAGE FIELDS CONSTANT MACROS
 *********************************************************************************************************************/
 /* All offsets in Byte(s) and absolute (starting from Ethernet Frame Payload, i.e. Byte_0 of message header). */

 /* EthTSyn common message header fields. */
 /*
                                                 Octets   Offset
   8                   4                   1
   +-------------------+-------------------+
   | transportSpecific |    messageType    |       1        0
   +---------------------------------------+
   |     reserved      |    versionPTP     |       1        1
   +-------------------+-------------------+
   |             messageLength             |       2        2
   +---------------------------------------+
   |             domainNumber              |       1        4
   +---------------------------------------+
   |               reserved                |       1        5
   +---------------------------------------+
   |                 flags                 |       2        6
   +---------------------------------------+
   |            correctionField            |       8        8
   +---------------------------------------+
   |               reserved                |       4       16
   +---------------------------------------+
   |           sourcePortIdentity          |      10       20
   +---------------------------------------+
   |               sequenceId              |       2       30
   +---------------------------------------+
   |                control                |       1       32
   +---------------------------------------+
   |           logMessageInterval          |       1       33
   +---------------------------------------+
  */
  /* The value of the correction field must be divided by 2^16 (see IEEE802.1AS) to get the value in nanoseconds. This
   * is equivalent to the value of the most significant 48 bit. Since EthTSyn ignores sub-nanosecond values, only
   * those 48 bit are serialized/deserialized.
   */
  /* Source Port Identity:
                                                  Octets   Offset
    8                   4                   1
    +---------------------------------------+
    |            clockIdentity              |       8        0
    +---------------------------------------+
    |              portNumber               |       2        8
    +---------------------------------------+
   */
  /* Message header field offsets. */
# define ETHTSYN_MSG_HDR_TRANSPORT_SPECIFIC_OFS                       (0u)
# define ETHTSYN_MSG_HDR_TRANSPORT_SPECIFIC_SHIFT                     (4u)
# define ETHTSYN_MSG_HDR_TRANSPORT_SPECIFIC_MASK                      (0xF0u)
# define ETHTSYN_MSG_HDR_MSG_TYPE_OFS                                 (0u)
# define ETHTSYN_MSG_HDR_MSG_TYPE_SHIFT                               (0u)
# define ETHTSYN_MSG_HDR_MSG_TYPE_MASK                                (0x0Fu)
# define ETHTSYN_MSG_HDR_RESERVED_0_OFS                               (1u)
# define ETHTSYN_MSG_HDR_RESERVED_0_SHIFT                             (4u)
# define ETHTSYN_MSG_HDR_RESERVED_0_MASK                              (0xF0u)
# define ETHTSYN_MSG_HDR_VERSION_PTP_OFS                              (1u)
# define ETHTSYN_MSG_HDR_VERSION_PTP_SHIFT                            (0u)
# define ETHTSYN_MSG_HDR_VERSION_PTP_MASK                             (0x0Fu)
# define ETHTSYN_MSG_HDR_MSG_LENGTH_OFS                               (2u)
# define ETHTSYN_MSG_HDR_DOMAIN_NUM_OFS                               (4u)
# define ETHTSYN_MSG_HDR_RESERVED_1_OFS                               (5u)
# define ETHTSYN_MSG_HDR_FLAGS_OFS                                    (6u)
# define ETHTSYN_MSG_HDR_FLAGS_0_OFS                                  (6u)
# define ETHTSYN_MSG_HDR_FLAGS_1_OFS                                  (7u)
# define ETHTSYN_MSG_HDR_CORR_FIELD_OFS                               (8u)
# define ETHTSYN_MSG_HDR_CORR_FIELD_SUB_NS_OFS                        (14u)
# define ETHTSYN_MSG_HDR_RESERVED_2_OFS                               (16u)
# define ETHTSYN_MSG_HDR_SRC_PORT_IDENTITY_OFS                        (20u)
# define ETHTSYN_MSG_HDR_SRC_PORT_IDENT_CLOCK_IDENT_OFS               (20u)
# define ETHTSYN_MSG_HDR_SRC_PORT_IDENT_PORT_NUM_OFS                  (28u)
# define ETHTSYN_MSG_HDR_SEQUENCE_ID_OFS                              (30u)
# define ETHTSYN_MSG_HDR_CONTROL_OFS                                  (32u)
# define ETHTSYN_MSG_HDR_LOG_MSG_INTERVAL_OFS                         (33u)

  /* Message header field sizes. */
# define ETHTSYN_MSG_HDR_TRANSPORT_SPECIFIC_SIZE                      (1u)
# define ETHTSYN_MSG_HDR_MSG_TYPE_SIZE                                (1u)
# define ETHTSYN_MSG_HDR_RESERVED_0_SIZE                              (1u)
# define ETHTSYN_MSG_HDR_VERSION_PTP_SIZE                             (1u)
# define ETHTSYN_MSG_HDR_MSG_LENGTH_SIZE                              (2u)
# define ETHTSYN_MSG_HDR_DOMAIN_NUM_SIZE                              (1u)
# define ETHTSYN_MSG_HDR_RESERVED_1_SIZE                              (1u)
# define ETHTSYN_MSG_HDR_FLAGS_SIZE                                   (2u)
# define ETHTSYN_MSG_HDR_CORR_FIELD_SIZE                              (8u)
# define ETHTSYN_MSG_HDR_RESERVED_2_SIZE                              (4u)
# define ETHTSYN_MSG_HDR_SRC_PORT_IDENTITY_SIZE                       (10u)
# define ETHTSYN_MSG_HDR_SRC_PORT_IDENT_CLOCK_IDENT_SIZE              (8u)
# define ETHTSYN_MSG_HDR_SRC_PORT_IDENT_PORT_NUM_SIZE                 (2u)
# define ETHTSYN_MSG_HDR_SEQUENCE_ID_SIZE                             (2u)
# define ETHTSYN_MSG_HDR_CONTROL_SIZE                                 (1u)
# define ETHTSYN_MSG_HDR_LOG_MSG_INTERVAL_SIZE                        (1u)

   /* EthTSyn Port Identity field (relative offsets) */
   /* Message Port Identity:
     Octets   Offset
     8                   4                   1
     +---------------------------------------+
     |            clockIdentity              |       8        0
     +---------------------------------------+
     |              portNumber               |       2        8
     +---------------------------------------+
   */
# define ETHTSYN_MSG_PORT_IDENT_CLOCK_IDENTY_OFS                      (0u)
# define ETHTSYN_MSG_PORT_IDENT_PORT_NUM_OFS                          (8u)
# define ETHTSYN_MSG_PORT_IDENT_SIZE                                  (10u)

   /* EthTSyn Sync message fields. */
   /*
                                                  Octets   Offset
     8                   4                   1
     +---------------------------------------+
     |                header                 |      34        0
     +---------------------------------------+
     |               reserved                |      10       34
     +---------------------------------------+
    */
# define ETHTSYN_MSG_SYNC_RESERVED_OFS                                (34u)
/* Offsets relative to the PTP Payload. */
# define ETHTSYN_MSG_SYNC_RESERVED_PL_REL_OFS                         (0u)

    /* EthTSyn FollowUp message fields. */
    /*
                                                   Octets   Offset
      8                   4                   1
      +---------------------------------------+
      |                header                 |      34        0
      +---------------------------------------+
      |        preciseOriginTimestamp         |      10       34
      +---------------------------------------+
      |       Follow_Up information TLV       |      32       44
      +---------------------------------------+
      |       Optional: Ar Tlv header         |      10       76
      +---------------------------------------+
      |       Optional: Ar Sub-Tlvs           |       X       86
      +---------------------------------------+
     */
     /* Precise Origin Timestamp:
                                                    Octets   Offset
       8                   4                   1
       +---------------------------------------+
       |               secondsHi               |       2        0
       +---------------------------------------+
       |              secondsLow               |       4        2
       +---------------------------------------+
       |              nanoseconds              |       4        6
       +---------------------------------------+
      */
      /* FollowUp Information TLV:
                                                     Octets   Offset
        8                   4                   1
        +---------------------------------------+
        |                tlvType                |       2        0
        +---------------------------------------+
        |              lengthField              |       2        2
        +---------------------------------------+
        |            organizationId             |       3        4
        +---------------------------------------+
        |          organizationSubType          |       3        7
        +---------------------------------------+
        |       cummulativeScaledRateOffset     |       4       10
        +---------------------------------------+
        |           gmTimeBaseIndicator         |       2       14
        +---------------------------------------+
        |            lastGmPhaseChange          |      12       16
        +---------------------------------------+
        |         scaledLastGmFreqChange        |       4       28
        +---------------------------------------+
       */
# define ETHTSYN_MSG_FUP_POT_OFS                                      (34u)
# define ETHTSYN_MSG_FUP_POT_SEC_HI_OFS                               (34u)
# define ETHTSYN_MSG_FUP_POT_SEC_LOW_OFS                              (36u)
# define ETHTSYN_MSG_FUP_POT_NSEC_OFS                                 (40u)
# define ETHTSYN_MSG_FUP_INFO_TLV_TYPE_OFS                            (44u)
# define ETHTSYN_MSG_FUP_INFO_TLV_LEN_OFS                             (46u)
# define ETHTSYN_MSG_FUP_INFO_TLV_ORGA_ID_OFS                         (48u)
# define ETHTSYN_MSG_FUP_INFO_TLV_ORGA_SUB_TYPE_OFS                   (51u)
# define ETHTSYN_MSG_FUP_INFO_TLV_CUM_SCALED_RATE_OFS_OFS             (54u)
# define ETHTSYN_MSG_FUP_INFO_TLV_GM_TIME_BASE_IND_OFS                (58u)
# define ETHTSYN_MSG_FUP_INFO_TLV_LAST_GM_PHASE_CHG_OFS               (60u)
# define ETHTSYN_MSG_FUP_INFO_TLV_SCALDE_LAST_GM_PHASE_CHG_OFS        (72u)
# define ETHTSYN_MSG_FUP_AR_TLV_HDR_OFS                               (76u)
# define ETHTSYN_MSG_FUP_FIRST_AR_SUB_TLV_OFS                         (86u)
/* Offsets relative to the PTP Payload */
# define ETHTSYN_MSG_FUP_POT_PL_REL_OFS                               (0u)
# define ETHTSYN_MSG_FUP_POT_SEC_HI_PL_REL_OFS                        (0u)
# define ETHTSYN_MSG_FUP_POT_SEC_LOW_PL_REL_OFS                       (2u)
# define ETHTSYN_MSG_FUP_POT_NSEC_PL_REL_OFS                          (6u)
# define ETHTSYN_MSG_FUP_INFO_TLV_TYPE_PL_REL_OFS                     (10u)
# define ETHTSYN_MSG_FUP_INFO_TLV_LEN_PL_REL_OFS                      (12u)
# define ETHTSYN_MSG_FUP_INFO_TLV_ORGA_ID_PL_REL_OFS                  (14u)
# define ETHTSYN_MSG_FUP_INFO_TLV_ORGA_SUB_PL_REL_TYPE_OFS            (17u)
# define ETHTSYN_MSG_FUP_INFO_TLV_CUM_SCALED_RATE_OFS_PL_REL_OFS      (20u)
# define ETHTSYN_MSG_FUP_INFO_TLV_GM_TIME_BASE_IND_PL_REL_OFS         (24u)
# define ETHTSYN_MSG_FUP_INFO_TLV_LAST_GM_PHASE_CHG_PL_REL_OFS        (26u)
# define ETHTSYN_MSG_FUP_INFO_TLV_SCALDE_LAST_GM_PHASE_CHG_PL_REL_OFS (38u)
# define ETHTSYN_MSG_FUP_AR_TLV_HDR_PL_REL_OFS                        (42u)
# define ETHTSYN_MSG_FUP_FIRST_AR_SUB_TLV_PL_REL_OFS                  (52u)

       /* EthTSyn Timestamp fields (relative offsets) */
       /* Message Timestamp:
         Octets   Offset
         8                   4                   1
         +---------------------------------------+
         |               secondsHi               |       2        0
         +---------------------------------------+
         |              secondsLow               |       4        2
         +---------------------------------------+
         |              nanoseconds              |       4        6
         +---------------------------------------+
       */
  /* Relative timestamp field offset. */
# define ETHTSYN_MSG_TS_SEC_HI_OFS                                    (0u)
# define ETHTSYN_MSG_TS_SEC_LOW_OFS                                   (2u)
# define ETHTSYN_MSG_TS_NSEC_OFS                                      (6u)
  /* Timestamp field sizes. */
# define ETHTSYN_MSG_TS_SEC_HI_SIZE                                   (2u)
# define ETHTSYN_MSG_TS_SEC_LOW_SIZE                                  (4u)
# define ETHTSYN_MSG_TS_NSEC_SIZE                                     (4u)

       /* The AUTOSAR FollowUp message format is an extension to the IEEE FollowUp format.
        * The AR format is supported when EthTSynMessageCompliance == STD_OFF.
        * In this case, the FollowUp is extended by the AR TLV header followed by N AR-Sub-TLVs.
        * The offsets for the AR-Sub-TLVs are relative to the start of the corresponding AR-Sub-TLV because
        * the order of the AR-Sub-TLV is not fixed and not every AR-Sub-TLV might be present in every message. */

        /* FollowUp AUTOSAR TLV header */
        /*
                                                       Octets   Offset
          8                   4                   1
          +---------------------------------------+
          |                tlvType                |       2        0
          +---------------------------------------+
          |              lengthField              |       2        2
          +---------------------------------------+
          |            organizationId             |       3        4
          +---------------------------------------+
          |          organizationSubType          |       3        7
          +---------------------------------------+
         */
# define ETHTSYN_MSG_FUP_AR_TLV_HDR_TYPE_OFS                          (76u)
# define ETHTSYN_MSG_FUP_AR_TLV_HDR_LEN_OFS                           (78u)
# define ETHTSYN_MSG_FUP_AR_TLV_HDR_ORGA_ID_OFS                       (80u)
# define ETHTSYN_MSG_FUP_AR_TLV_HDR_ORGA_SUB_TYPE_OFS                 (83u)
/* Offsets relative to PTP Payload */
# define ETHTSYN_MSG_FUP_AR_TLV_HDR_TYPE_REL_OFS                      (42u)
# define ETHTSYN_MSG_FUP_AR_TLV_HDR_LEN_REL_OFS                       (44u)
# define ETHTSYN_MSG_FUP_AR_TLV_HDR_ORGA_ID_REL_OFS                   (46u)
# define ETHTSYN_MSG_FUP_AR_TLV_HDR_ORGA_SUB_TYPE_REL_OFS             (49u)

         /* FollowUp AR-Sub-TLV abstract header. */
         /*
                                                        Octets   Offset
           8                   4                   1
           +---------------------------------------+
           |                 Type                  |       1        0
           +---------------------------------------+
           |                Length                 |       1        1
           +---------------------------------------+
          */
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_TYPE_OFS                          (0u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_LEN_OFS                           (1u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_DATA_OFS                          (2u)

          /* FollowUp AR-Sub-TLV Time secured. */
          /*
                                                         Octets   Offset
            8                   4                   1
            +---------------------------------------+
            |           AR Sub TLV header           |       2        0
            +---------------------------------------+
            |            CRC_Time_Flags             |       1        2
            +---------------------------------------+
            |              CRC_Time_0               |       1        3
            +---------------------------------------+
            |              CRC Time 1               |       1        4
            +---------------------------------------+
           */
           /* Time secured Flags:
             +----------------+  +-----------------------
             |7|6|5|4|3|2|1|0||  |Flag name
             +----------------+  +-----------------------
             |7|                 |reserved
             +-------------------------------------------
               |6|               |reserved
               +-----------------------------------------
                 |5|             |preciseOriginTimestamp
                 +---------------------------------------
                   |4|           |sequenceId
                   +-------------------------------------
                     |3|         |sourcePortIdentity
                     +-----------------------------------
                       |2|       |correctionField
                       +---------------------------------
                         |1|     |domainNumber
                         +-------------------------------
                           |0|   |messageLength
                           +-----------------------------
            */
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_FLAGS_OFS            (2u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_CRC_0_OFS            (3u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_TIME_SECURED_CRC_1_OFS            (4u)

            /* FollowUp AR-Sub-TLV Status. */
            /*
                                                           Octets   Offset
              8                   4                   1
              +---------------------------------------+
              |           AR Sub TLV header           |       2        0
              +---------------------------------------+
              |                 Status                |       1        2
              +---------------------------------------+
              |               CRC Status              |       1        3
              +---------------------------------------+
             */
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_STATUS_STATUS_OFS                 (2u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_STATUS_CRC_OFS                    (3u)

             /* FollowUp AR-Sub-TLV UserData */
             /*
                                                            Octets   Offset
               8                   4                   1
               +---------------------------------------+
               |           AR Sub TLV header           |       2        0
               +---------------------------------------+
               |            UserDataLength             |       1        2
               +---------------------------------------+
               |              UserByte 0               |       1        3
               +---------------------------------------+
               |              UserByte 1               |       1        4
               +---------------------------------------+
               |              UserByte 2               |       1        5
               +---------------------------------------+
               |             CRC UserData              |       1        6
               +---------------------------------------+
              */
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_LEN_OFS              (2u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_BYTE_0_OFS           (3u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_BYTE_1_OFS           (4u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_UD_BYTE_2_OFS           (5u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_USER_DATA_CRC_OFS                 (6u)

              /* FollowUp AR-Sub-TLV OFS */
              /*
                                                             Octets   Offset
                8                   4                   1
                +---------------------------------------+
                |           AR Sub TLV header           |       2        0
                +---------------------------------------+
                |             OfsTimeDomain             |       1        2
                +---------------------------------------+
                |               OfsTimeSec              |       6        3
                +---------------------------------------+
                |               OfsTimeNSec             |       4        9
                +---------------------------------------+
                |                 Status                |       1       13
                +---------------------------------------+
                |              UserDataLength           |       1       14
                +---------------------------------------+
                |                UserByte 0             |       1       15
                +---------------------------------------+
                |                UserByte 1             |       1       16
                +---------------------------------------+
                |                UserByte 2             |       1       17
                +---------------------------------------+
                |                 CRC_OFS               |       1       18
                +---------------------------------------+
               */
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_DOMAIN_NUM_OFS            (2u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_TIME_SEC_HI_OFS           (3u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_TIME_SEC_LOW_OFS          (5u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_OFS_TIME_NSEC_OFS             (9u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_STATUS_OFS                    (13u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_LEN_OFS             (14u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_BYTE_0_OFS          (15u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_BYTE_1_OFS          (16u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_USER_DATA_BYTE_2_OFS          (17u)
# define ETHTSYN_MSG_FUP_AR_SUB_TLV_OFS_CRC_OFS                       (18u)

               /* EthTSyn Announce message fields. */
               /*
                                                              Octets   Offset
                 8                   4                   1
                 +---------------------------------------+
                 |                header                 |      34        0
                 +---------------------------------------+
                 |               reserved                |      10       34
                 +---------------------------------------+
                 |           currentUtcOffset            |       2       44
                 +---------------------------------------+
                 |               reserved                |       1       46
                 +---------------------------------------+
                 |          grandmasterPriority1         |       1       47
                 +---------------------------------------+
                 |         grandmasterClockQuality       |       4       48
                 +---------------------------------------+
                 |          grandmasterPriority2         |       1       52
                 +---------------------------------------+
                 |           grandmasterIdentity         |       8       53
                 +---------------------------------------+
                 |              stepsRemoved             |       2       61
                 +---------------------------------------+
                 |               timeSource              |       1       63
                 +---------------------------------------+
                 |             path trace TLV            |     4+8N      64
                 +---------------------------------------+
                */
                /* Grandmaster Clock Quality:
                                                               Octets   Offset
                  8                   4                   1
                  +---------------------------------------+
                  |              clockClass               |       1        0
                  +---------------------------------------+
                  |            clockAccuracy              |       1        1
                  +---------------------------------------+
                  |         offsetScaledLogVariance       |       2        2
                  +---------------------------------------+
                 */
                 /* Path Trace TLV:
                                                                Octets   Offset
                   8                   4                   1
                   +---------------------------------------+
                   |                tlvType                |       2        0
                   +---------------------------------------+
                   |              lengthField              |       2        2
                   +---------------------------------------+
                   |             pathSequence              |      8N        4
                   +---------------------------------------+
                  */
# define ETHTSYN_MSG_ANNOUNCE_RESERVED_0_OFS                          (34u)
# define ETHTSYN_MSG_ANNOUNCE_CURR_UTC_OFS_OFS                        (44u)
# define ETHTSYN_MSG_ANNOUNCE_RESERVED_1_OFS                          (46u)
# define ETHTSYN_MSG_ANNOUNCE_GM_PRIO_1_OFS                           (47u)
# define ETHTSYN_MSG_ANNOUNCE_GM_CLK_QUAL_CLK_CLASS_OFS               (48u)
# define ETHTSYN_MSG_ANNOUNCE_GM_CLK_QUAL_CLK_ACCURACY_OFS            (49u)
# define ETHTSYN_MSG_ANNOUNCE_GM_CLK_QUAL_OFS_SCALED_LOG_VAR_OFS      (50u)
# define ETHTSYN_MSG_ANNOUNCE_GM_PRIO_2_OFS                           (52u)
# define ETHTSYN_MSG_ANNOUNCE_GM_IDENTITY_OFS                         (53u)
# define ETHTSYN_MSG_ANNOUNCE_STEPS_REMOVED_OFS                       (61u)
# define ETHTSYN_MSG_ANNOUNCE_TIME_SRC_OFS                            (63u)
# define ETHTSYN_MSG_ANNOUNCE_PATH_TRACE_TLV_TYPE_OFS                 (64u)
# define ETHTSYN_MSG_ANNOUNCE_PATH_TRACE_TLV_LEN_OFS                  (66u)
# define ETHTSYN_MSG_ANNOUNCE_PATH_TRACE_TLV_PATH_SEQ_START_OFS       (68u)
/* Offsets relative to the PTP Payload */
# define ETHTSYN_MSG_ANNOUNCE_RESERVED_0_PL_REL_OFS                     (0u)
# define ETHTSYN_MSG_ANNOUNCE_CURR_UTC_OFS_PL_REL_OFS                   (10u)
# define ETHTSYN_MSG_ANNOUNCE_RESERVED_1_PL_REL_OFS                     (12u)
# define ETHTSYN_MSG_ANNOUNCE_GM_PRIO_1_PL_REL_OFS                      (13u)
# define ETHTSYN_MSG_ANNOUNCE_GM_CLK_QUAL_CLK_CLASS_PL_REL_OFS          (14u)
# define ETHTSYN_MSG_ANNOUNCE_GM_CLK_QUAL_CLK_ACCURACY_PL_REL_OFS       (15u)
# define ETHTSYN_MSG_ANNOUNCE_GM_CLK_QUAL_OFS_SCALED_LOG_VAR_PL_REL_OFS (16u)
# define ETHTSYN_MSG_ANNOUNCE_GM_PRIO_2_PL_REL_OFS                      (18u)
# define ETHTSYN_MSG_ANNOUNCE_GM_IDENTITY_PL_REL_OFS                    (19u)
# define ETHTSYN_MSG_ANNOUNCE_STEPS_REMOVED_PL_REL_OFS                  (27u)
# define ETHTSYN_MSG_ANNOUNCE_TIME_SRC_PL_REL_OFS                       (29u)
# define ETHTSYN_MSG_ANNOUNCE_PATH_TRACE_TLV_TYPE_PL_REL_OFS            (30u)
# define ETHTSYN_MSG_ANNOUNCE_PATH_TRACE_TLV_LEN_PL_REL_OFS             (32u)
# define ETHTSYN_MSG_ANNOUNCE_PATH_TRACE_TLV_PATH_SEQ_START_PL_REL_OFS  (34u)

                  /* EthTSyn Pdelay request message field. */
                  /*
                                                                 Octets   Offset
                    8                   4                   1
                    +---------------------------------------+
                    |                header                 |      34        0
                    +---------------------------------------+
                    |               reserved                |      10       34
                    +---------------------------------------+
                    |               reserved                |      10       44
                    +---------------------------------------+
                   */
# define ETHTSYN_MSG_PD_REQ_RESERVED_0_OFS                            (34u)
# define ETHTSYN_MSG_PD_REQ_RESERVED_1_OFS                            (44u)
/* Offsets relative to the PTP Payload */
# define ETHTSYN_MSG_PD_REQ_RESERVED_0_PL_REL_OFS                     (0u)
# define ETHTSYN_MSG_PD_REQ_RESERVED_1_PL_REL_OFS                     (10u)

                   /* EthTSyn Pdelay response message filed. */
                   /*
                                                                  Octets   Offset
                     8                   4                   1
                     +---------------------------------------+
                     |                header                 |      34        0
                     +---------------------------------------+
                     |       requestReceivedTimestamp        |      10       34
                     +---------------------------------------+
                     |        requestingPortIdentity         |      10       44
                     +---------------------------------------+
                    */
                    /* Request received Origin Timestamp:
                                                                   Octets   Offset
                      8                   4                   1
                      +---------------------------------------+
                      |               secondsHi               |       2        0
                      +---------------------------------------+
                      |              secondsLow               |       4        2
                      +---------------------------------------+
                      |              nanoseconds              |       4        6
                      +---------------------------------------+
                     */
                     /* Requesting Port Identity:
                                                                     Octets   Offset
                       8                   4                   1
                       +---------------------------------------+
                       |            clockIdentity              |       8        0
                       +---------------------------------------+
                       |              portNumber               |       2        8
                       +---------------------------------------+
                      */
# define ETHTSYN_MSG_PD_RESP_REQ_RCVD_TS_SEC_HI_OFS                   (34u)
# define ETHTSYN_MSG_PD_RESP_REQ_RCVD_TS_SEC_LOW_OFS                  (36u)
# define ETHTSYN_MSG_PD_RESP_REQ_RCVD_TS_NSEC_OFS                     (40u)
# define ETHTSYN_MSG_PD_RESP_REQ_PORT_IDENT_OFS                       (44u)
# define ETHTSYN_MSG_PD_RESP_REQ_CLOCK_IDENTITY_OFS                   (44u)
# define ETHTSYN_MSG_PD_RESP_REQ_PORT_NUM_OFS                         (52u)
/* Offsets relative to the PTP Payload */
# define ETHTSYN_MSG_PD_RESP_REQ_RCVD_TS_SEC_HI_PL_REL_OFS            (0u)
# define ETHTSYN_MSG_PD_RESP_REQ_RCVD_TS_SEC_LOW_PL_REL_OFS           (2u)
# define ETHTSYN_MSG_PD_RESP_REQ_RCVD_TS_NSEC_PL_REL_OFS              (6u)
# define ETHTSYN_MSG_PD_RESP_REQ_PORT_IDENT_PL_REL_OFS                (10u)
# define ETHTSYN_MSG_PD_RESP_REQ_CLOCK_IDENTITY_PL_REL_OFS            (10u)
# define ETHTSYN_MSG_PD_RESP_REQ_PORT_NUM_PL_REL_OFS                  (18u)

                      /* EthTSyn Pdelay response follow up message filed. */
                      /*
                                                                     Octets   Offset
                        8                   4                   1
                        +---------------------------------------+
                        |                header                 |      34        0
                        +---------------------------------------+
                        |        responseOriginTimestamp        |      10       34
                        +---------------------------------------+
                        |        requestingPortIdentity         |      10       44
                        +---------------------------------------+
                       */
                       /* Response Origin Timestamp:
                                                                      Octets   Offset
                         8                   4                   1
                         +---------------------------------------+
                         |               secondsHi               |       2        0
                         +---------------------------------------+
                         |              secondsLow               |       4        2
                         +---------------------------------------+
                         |              nanoseconds              |       4        6
                         +---------------------------------------+
                        */
                        /* Requesting Port Identity:
                                                                        Octets   Offset
                          8                   4                   1
                          +---------------------------------------+
                          |            clockIdentity              |       8        0
                          +---------------------------------------+
                          |              portNumber               |       2        8
                          +---------------------------------------+
                         */
# define ETHTSYN_MSG_PD_RESP_FUP_RESP_ORG_TS_SEC_HI_OFS               (34u)
# define ETHTSYN_MSG_PD_RESP_FUP_RESP_ORG_TS_SEC_LOW_OFS              (36u)
# define ETHTSYN_MSG_PD_RESP_FUP_RESP_ORG_TS_NSEC_OFS                 (40u)
# define ETHTSYN_MSG_PD_RESP_FUP_REQ_PORT_IDENT_OFS                   (44u)
# define ETHTSYN_MSG_PD_RESP_FUP_REQ_CLOCK_IDENTITY_OFS               (44u)
# define ETHTSYN_MSG_PD_RESP_FUP_REQ_PORT_NUM_OFS                     (52u)
/* Offsets relative to the PTP Payload */
# define ETHTSYN_MSG_PD_RESP_FUP_RESP_ORG_TS_SEC_HI_PL_REL_OFS        (0u)
# define ETHTSYN_MSG_PD_RESP_FUP_RESP_ORG_TS_SEC_LOW_PL_REL_OFS       (2u)
# define ETHTSYN_MSG_PD_RESP_FUP_RESP_ORG_TS_NSEC_PL_REL_OFS          (6u)
# define ETHTSYN_MSG_PD_RESP_FUP_REQ_PORT_IDENT_PL_REL_OFS            (10u)
# define ETHTSYN_MSG_PD_RESP_FUP_REQ_CLOCK_IDENTITY_PL_REL_OFS        (10u)
# define ETHTSYN_MSG_PD_RESP_FUP_REQ_PORT_NUM_PL_REL_OFS              (18u)

/* Offsets relative to the PTP Payload */
# define ETHTSYN_MSG_PD_RESP_RESP_FUP_TS_SEC_HI_PL_REL_OFS            (0u)
# define ETHTSYN_MSG_PD_RESP_RESP_FUP_TS_SEC_LOW_PL_REL_OFS           (2u)
# define ETHTSYN_MSG_PD_RESP_RESP_FUP_TS_NSEC_PL_REL_OFS              (6u)
# define ETHTSYN_MSG_PD_RESP_RESP_FUP_REQ_PORT_IDENT_PL_REL_OFS       (10u)
# define ETHTSYN_MSG_PD_RESP_RESP_FUP_REQ_CLOCK_IDENTITY_PL_REL_OFS   (10u)
# define ETHTSYN_MSG_PD_RESP_RESP_FUP_REQ_PORT_NUM_PL_REL_OFS         (18u)


#endif /* ETHTSYN_MSG_DEFS_INT_H */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn_MsgDefs_Int.h
 *********************************************************************************************************************/
