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
/*!        \file  EthTSyn.h
 *         \unit  *
 *        \brief  EthTSyn module header file
 *      \details  Contains all public function declarations and global definitions with respect to the EthTSyn module
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-08-28  vislje  ESCAN00077557 created
 *            2014-09-23  vislje  -             Support for Software-Timestamping added
 *                                              Added Bugfixes / Features from SysService_Gptp802As[1.01.01]
 *            2014-09-24  vislje  -             Changed SyncSend[].TxConfirmationPending to sint8
 *            2014-10-23  vislje  -             Organization of types
 *  01.00.01  2014-11-11  vislje  ESCAN00079492 Module causes DET in Eth-Module at initialization
 *                        vissem  ESCAN00079841 Added missing DET to MainFunction
 *                                -             Changed Module ID from 676 to 164
 *                                -             Added EthTSyn_SetTransmissionMode API
 *  01.00.02  2014-11-21  vissem  ESCAN00079837 Pdelay calculation is not valid if Pdelay responder response very fast
 *                                ESCAN00079849 Unexpected behavior of API EthTSyn_SetTransmissionMode
 *                                ESCAN00079853 Wrong RequestingPortNumber of PdelayRespFollowUp message undetected
 *            2015-01-07  vissem  ESCAN00080431 Compiler error: Wrong DET in EthTSyn_SetTransmissionMode
 *  01.01.00  2015-02-17  vissem  ESCAN00081290 Timestamp is not sent correctly
 *            2015-02-18  vissem  ESCAN00080071 FEAT-1268: Add timestamp support (STBM) in software for EthTSyn
 *            2015-02-27  vissem  ESCAN00081498 Wrong Transceiver Link State/Enabled Flag after Re-Initialization of
 *                                              EthTSyn
 *            2015-03-03  vissem  ESCAN00081543 Negative Pdelay leads to AsCapable = false
 *            2015-03-24  vissem  ESCAN00081965 Incorrect TimestampType in EthTSyn_SetGlobalTime
 *  01.01.01  2015-04-22  vissem  ESCAN00082125 Code Clean-Up and correction of issues found by review
 *  01.01.02  2015-07-07  vissem  ESCAN00083506 Compiler warning: Unused variable IngressTimestampRaw
 *                                ESCAN00083505 Compiler warning: Unused variable TimestampQuality
 *                                ESCAN00083508 Compiler warning: Declared but unused function
 *                                              EthTSyn_RawTimeToTimestamp
 *                                ESCAN00083507 Compiler warning: Declared but unused function:
 *                                              EthTSyn_ProcessSmSiteSyncSync
 *                                ESCAN00083497 Os Error Hook: Disabled Interrupts
 *                                ESCAN00083503 Synchronized time in Time Slave lags behind
 *            2015-07-08  vissem  ESCAN00083501 Time calculated by EthTsyn lags behind real time
 *                                ESCAN00083638 Compiler warning: EthTSyn - Possible loss of data due to implicit type
 *                                              conversion
 *                                ESCAN00084291 Compiler error: C2440: 'function' : cannot convert from
 *                                              'EthTSyn_GlobalTimestampType' to 'Eth_TimeStampType'
 *  01.02.00  2015-07-09  vissem  ESCAN00083859 Adopt Type of DataPtr in EthTSyn_RxIndication
 *                                ESCAN00083403 StbM_TimeStampRawType is not compatible to AR 4.2.1
 *            2015-07-22  vissem  ESCAN00084123 Removed AUTOSAR Version filter
 *  01.02.01  2015-08-14  vissem  ESCAN00084533 Calculation of RateRatio is incorrect
 *  02.00.00  2015-09-01  vissem  ESCAN00085374 FEAT-1529: Support Ethernet Switches for Ethernet Time Sync
 *                                ESCAN00085671 FEAT-1555: Ethernet Stack Bugfixing
 *            2015-10-01  vissem  ESCAN00085562 Invalid Pointer passed to EthIf_GetIngressTimestamp
 *            2015-10-12  vissem  ESCAN00085796 Valid PDelay_Resp_FollowUp is discarded
 *            2015-10-14  vissem  ESCAN00085769 EthTSyn issues a DET error if a gPTP frame is received on a network/VLAN
 *                                              on which EthTSyn itself is not active
 *                        vissem  ESCAN00084129 Allow the use of always AsCapable
 *                        vissem  ESCAN00085186 AUTOSAR 4.2.2 API adoptions
 *                        vissem  ESCAN00085629 Incompatible Timestamp types
 *  02.00.01  2016-01-26  vissem  ESCAN00087452 FollowUp-CorrectionField is not taken into account at the
 *                                              synchronization of a Time-Slave
 *  02.00.02  2016-02-19  vissem  ESCAN00088420 Review Integration
 *  02.01.00  2016-04-01  vissem  ESCAN00087687 FEAT-1691: EthTSyn shall support RateCorrection as OffsetCorrection
 *                                              and/or FrequencyRateCorrection below and above SynchronizationThreshold
 *                                ESCAN00084951 Added support of RateRatio for SW-Timestamping
 *                                ESCAN00087691 FEAT-1689: Each Time Communication Port shall be able to support
 *                                              Time Master & Time Slave functionality in parallel
 *                                ESCAN00088535 Time slave does not synchronize to the time master without former
 *                                              Announce message
 *                                ESCAN00086975 EthTSyn does not release TxBuffers if the transmission failed
 *                                ESCAN00088539 In a Time-Aware-Bridge the modification of the SourcePortIdenty is now
 *                                              optional
 *                                ESCAN00088655 EthTSyn_Cbk_TrcvLinkStateChg renamed to EthTSyn_TrcvLinkStateChg
 *                                              (according to AUTOSAR 4.2.2)
 *                                ESCAN00088877 StbM_SetGlobalTime() is called instead of StbM_BusSetGlobalTime()
 *                                ESCAN00088493 FEAT-1694: Time Gateway shall be supported and EthTSyn shall support
 *                                              Boundary Clock on each Time Master Port
 *                                ESCAN00088660 Added SourcePortIdentity check for Sync/FollowUp messages received
 *                                              by a slave port
 *                                ESCAN00087695 FEAT-1201: Release of FEAT-971 ETH only
 *                                              (Time Sync over Ethernet module development) [AR4-954]
 *  02.02.00  2016-05-13  vissem  ESCAN00089687 FEAT-1987: Support of flexible Pdelay configuration
 *  02.02.01  2016-06-24  vissem  ESCAN00090251 Synchronization might be erroneous if corrective action RATE_RATIO is
 *                                              used
 *  03.00.00  2016-11-25  vissem  ESCAN00091272 Compiler error: #154: expression must have struct or union type
 *                                FEATC-248     FEAT-1998: Support of HW Time Stamping for Switch for EthTSyn
 *                                              (SysService_AsrTSynEth)
 *                                ESCAN00091957 Inconsistent use of the configuration parameter
 *                                              EthTSynGlobalTimeFollowUpTimeout
 *                                ESCAN00092113 Added support of the configuration parameter
 *                                              EthTSynTimeHardwareCorrectionThreshold
 *                                ESCAN00091972 Missing comparison of the source port identity for
 *                                              received Sync/FollowUp
 *  03.00.01  2016-11-29  vissem  ESCAN00093066 Used EthTSyn_TimediffType is too small when HW-Timestamping is active
 *                                ESCAN00093137 Using RateRatio correction in conjunction if HW-Timestamping can cause
 *                                              an Os TickTime error
 *                                ESCAN00093131 EthIf_GetIngressTimestamp is called in invalid context
 *                                ESCAN00093296 DET may occur on transmission of FollowUp message or buffer data is
 *                                              overwritten
 *                                ESCAN00093361 DET may occur on transmission of Announce message or buffer data is
 *                                              overwritten
 *                                ESCAN00093326 Message buffers with invalid Switch Mgmt info are not released
 *                                ESCAN00093325 Received message with invalid switch information might not be dropped
 *                                ESCAN00093248 DET ETHTSYN_SWT_MGMT_E_NO_MSG_AVAILABLE is issued when
 *                                              EthTSyn_SwitchMgmtInfoIndication is called after a message was
 *                                              discarded by the EthTSyn
 *                                ESCAN00091834 Call of the OS-API GetCounterValue() during disabled interrupts
 *                                ESCAN00093366 Compiler error: Syntax error
 *                                ESCAN00093411 ResponseOriginTimestamp is always 0 after first Pdelay message flow
 *                                ESCAN00093384 Compiler error: EthIf_SwitchEnableEgressTimestamp declared implicitly
 *                                ESCAN00093247 Reception of Announce leads to DET ETHTSYN_E_MSG_LENGTH
 *                                ESCAN00093327 Sync receive Timeout is supervised although Grandmaster Capable is
 *                                              disabled
 *  03.00.02  2017-01-24  vissem  ESCAN00093708 Precision of the RateRatio is insufficient when HW-Timestamping is used
 *  04.00.00  2017-03-31  vissem  ESCAN00093838 Misalignment exception during reception of a frame with odd length on
 *                                              RH850/V850 platform
 *                                ESCAN00093937 Message handling with enabled Switch Timestamping not working properly
 *                                FEATC-383     FEAT-2279: Time Synchronization acc. AR 4.3 for EthTSyn
 *  04.00.01  2017-04-18  vissem  ESCAN00094545 CrcTime1 value of the Time Secured SubTlv is invalid when SequenceId is
 *                                              included
 *                                ESCAN00094780 FollowUp IEEE Information Tlv invalid organization Id
 *  04.00.02  2017-05-03  vissem  ESCAN00094784 Compiler error: Multiple errors (e.g. undeclared identifier) with
 *                                              Switch Mgmt and Rx Announce support
 *                                ESCAN00094847 StbM APIs called with disabled interrupts
 *                                ESCAN00094973 Missing observation of EthTSynPdelayRespAndRespFollowUpTimeout
 *  05.00.00  2017-07-07  vissem  STORYC-1213   Global Time Precision Measurement Support
 *                                STORYC-113    Immediate Time Synchronization (event based SYNC in parallel to the
 *                                              regular cycle based SYNC)
 *  05.01.00  2017-07-26  vissem  ESCAN00095964 Time Base Sync for slave ports: Follow up timeout always exceeds if
 *                                              follow up timeout is disabled
 *                                ESCAN00095802 Compiler warning: Multiple warnings
 *  06.00.00  2017-10-27  vissem  STORYC-411    FEAT-2463,S-445 [SAFE] SafeBSW for EthTSyn (ASIL B), S1ComStackLib
 *                                ESCAN00096626 EthTSyn expects uint8* as return value of
 *                                              StbM_GetTimeBaseUpdateCounter()
 *                                ESCAN00096710 Message handling with enabled Switch Timestamping not working properly
 *                                ESCAN00096825 Linker error: Unresolved external symbol (EthTSyn_ProcessSmPdelayResp)
 *                                ESCAN00096818 Possible index out of bounds access
 *                                STORYC-119    AR4.3 extensions not covered by separate story
 *  06.01.00  2017-11-17  vissem  STORYC-410    FEAT-2463,S-444 [SAFE] SafeBSW for EthTSyn (ASIL B), S1 RevHlp
 *                                STORYC-3211   SysService_AsrTSynEth: Additional code and generator refactoring
 *                                ESCAN00097557 Compiler error: Redeclared types
 *            2017-12-08  vissem  ESCAN00097679 On message transmission, the byte order of the 'Flags' field does not
 *                                              match IEEE802.1As
 *            2017-12-12  vissem  ESCAN00097716 Possible memory access violation (read) because of not checking length
 *                                              of AUTOSAR TLV
 *            2018-01-11  vissem  ESCAN00097931 PdelayRequestReceivedTimestamp/PdelayResponseOriginTimestamp might
 *                                              contain a nanoseconds value greater than 999999999ns
 *                                ESCAN00097934 Possible wraparound of StbM raw timestamp remains undetected
 *            2018-01-29  vissem  STORYC-414    FEAT-2463,S-448 [SAFE] SafeBSW for EthTSyn (ASIL B), S1Test100
 *            2018-02-27  vissem  ESCAN00098324 EthTSyn transmits messages (Pdelay_Req) although TxMode is set to
 *                                              ETHTSYN_TX_OFF
 *            2018-03-08  vissem  ESCAN00098608 TwoStepFlag is set to TRUE in all message types and not just in Sync
 *                                              and Pdelay_Resp message
 *  06.02.00  2018-04-11  vissem  STORYC-4963   SysService_AsrTSynEth: Rework of CDD
 *            2018-04-19  vissem  STORYC-5002   Support time base specific time difference calculation acc. ASR4.3.1
 *  06.02.01  2018-05-02  vissem  ESCAN00099268 Invalid Timestamp might be used for calculation of SyncRefTime
 *            2018-08-16  vissem  ESCAN00099722 BETA version - the BSW module is in BETA state
 *  07.00.00  2018-09-18  vissem  ESCAN00098852 LogMessageInterval of transmitted Sync/FollowUp/Announce message is
 *                                              always 0.
 *            2018-09-19  vissem  ESCAN00098444 EthTSyn Master transmits Sync/FollowUp messages although the
 *                                              Global_Time_Base bit is not set
 *            2018-11-16  vissem  STORYC-6413   [AVB] Implementation of 802.1Qbv extensions for EthTSyn
 *                                ESCAN00101348 Time Master might transmit a too new PreciseOriginTimestamp
 *  08.00.00  2018-12-13  vissem  STORYC-6555   FEAT-2463 [SAFE] SafeBSW for EthTSyn (ASIL B), S1Test/ACTS2
 *            2019-01-31  vissem  STORYC-5241   MISRA-C:2012 Compliance
 *            2019-02-06  vissem  ESCAN00101962 UserData is always '0' in Offset Sub-Tlv
 *            2019-02-15  vissem  STORYC-6554   FEAT-2463 [SAFE] SafeBSW for EthTSyn (ASIL B), S1Rework2
 *            2019-02-19  vissem  ESCAN00102065 Undefined UserData passed to StbM
 *                                STORYC-4904   SysService_AsrTSynEth: Omit UserData Sub-Tlv when UserDataLength is '0'
 *            2019-03-11  vissem  ESCAN00102425 EthTSyn answers to last Pdelay_Req instead of first Pdelay_Req
 *  09.00.00  2019-03-18  vissem  STORYC-5323   Remove structure overlay for Ethernet Rx/Tx buffers
 *                                ESCAN00095190 Misinterpretation of header information on reception of PTP frames.
 *                                ESCAN00102479 Compiler error: invalid bit-field size for base type
 *  10.00.00  2019-06-13  vissem  STORY-11003   Introduce switch management main function
 *            2019-06-14  vissem  STORY-11002   Use ASR4.4.x switch management for message reception
 *            2019-07-01  vissem  ESCAN00103204 Timestamp is updating for consecutive same Sequence ID
 *            2019-07-02  vissem  ESCAN00103547 Correction field of forwarded FollowUp message is incorrect
 *            2019-07-15  vissem  TCPIP-62      Use ASR4.4.x switch management for message transmission
 *                                ESCAN00103562 Pdelay_Resp_Follow_Up is processed before Pdelay_Resp in switch
 *                                              management mode
 *  10.00.01  2019-10-24  vissem  ESCAN00103687 FollowUp message might not be forwarded
 *                                ESCAN00104655 Pdelay_Resp_FollowUp is not transmitted after transmission of
 *                                              Pdelay_Resp
 *                                ESCAN00104882 EthTSyn stops transmission of Pdelay_Req messages
 *                                ESCAN00104887 EthTSyn does not send Follow_Up and no subsequent Sync messages
 *  11.00.00  2019-11-20  vissem  SAA-316       [GTS] Support PdelayLatencyThreshold acc. to ASR 4.4
 *  12.00.00  2020-03-18  vissem  SAA-444       EthTSyn: Enhance precision of Global Time (Part 1)
 *  12.01.00  2020-04-23  vissem  ESCAN00106112 ptpTimescale flag in PTP message header is not set
 *                                SAA-581       EthTSyn: Introduce Time Validation Unit
 *                                SAA-665       EthTSyn: Time Validation for Bridge
 *                                SAA-726       EthTSyn: Configuration and transparent behavior for
 *                                              Transparent/Boundary Clock
 *                                SAA-590       Switch to Host synchronization in EthTSyn
 *  12.02.00  2020-06-08  vissem  SAA-661       EthTSyn: Time Validation for EndStation
 *  12.02.01  2020-06-22  vissem  ESCAN00106636 Compiler error: Undefined reference to StbM_EthSet*Data
 *  12.03.00  2020-06-25  vissem  SAA-663       EthTSyn: Provide bridge operation modes Transparent/Boundary Clock
 *  12.04.00  2020-07-17  vissem  ESCAN00106883 Invalid switch residence time used for synchronization of the host CPU
 *                                              as time slave
 *            2020-08-03  visfaz  SAA-887       Optimization of time synchronization algorithm for Ethernet switches
 *                                              (Finalize)
 *            2020-08-05  vissem  ESCAN00106755 Two consecutive Pdelay_Resp_Fup messages are transmitted with the same
 *                                              PdelayRespOrigin timestamp
 *  13.00.00  2020-09-02  vissem  SAA-907       Support Switches in ASIL Ethernet - EthTSyn (Code Refactoring)
 *                        visfaz
 *            2020-10-23  vissem  SAA-1259      [EthTSyn] Update version history to current template
 *  13.01.00  2020-11-20  visfaz  SAA-860       [EthTSyn] Update to QAC Helix
 *  13.02.00  2020-12-01  vissem  SAA-406       [GTS] Debounce Time introduction for EthTSyn
 *  13.03.00  2021-01-19  vissem  SAA-545       [GTS] Support CRC Validation of Follow_Up for Time Gateway
 *                                SAA-547       [GTS] Support Forwarding of CRC-protected Follow_Up for Time Gateway
 *            2021-01-28  vissem  ESCAN00108420 Correction field value of a forwarded FollowUp message is wrong in
 *                                              Transparent clock mode without switch timestamping
 *            2021-02-04  vissem  ESCAN00108474 Compiler error: Incompatible types for SwtMgmt queue iteration
 *  14.00.00  2021-07-05  vissem  SAA-943       Usage of _MemMap.h in EthTSyn
 *                                SAA-1675      [EthTSyn] Use VCA for silent analysis
 *                                ESCAN00109744 Sync/FollowUp not forwarded (properly) by Transparent Clock
 *  15.00.00  2021-11-24  visfaz  ESCAN00110761 Immediate time synchronization can not be used without cyclic Sync
 *                                              transmission
 *            2022-09-02  visfaz  SAA-1801      Implement a plausibility check for the correction field
 *            2022-09-30  visfaz  SAA-2696      Rate correction of sync reception delay
 *            2022-10-10  viseje  SAA-3067      Support configuration of the destination MAC address per time domain
 *                        vissem  SAA-2451      Support asymmetric Sync/FollowUp forwarding for switch management
 *  15.00.01  2022-12-20  vissem  ESCAN00113471 Pdelay initiator does not close the critical section 0
 *                                ESCAN00113472 Compiler error: Cannot open include file: Crc.h
 *  15.01.00  2023-01-16  vissem  SAA-3681      Implement de/-serialization of data types for byte arrays locally
 *                                ESCAN00113687 Pdelay_Resp/Pdelay_Resp_Fup with invalid message length are accepted
 *                                              and processed
 *            2023-01-19  visfaz  SAA-2833      Support API to retrieve Port Parameter Statistics
 *            2023-02-06  vissem  ESCAN00113809 Time master transmits FollowUp message containing an AUTOSAR Status
 *                                              Secured Sub-TLV with invalid CRC value
 *                                ESCAN00113813 Time master transmits FollowUp message containing an AUTOSAR UserData
 *                                              Secured Sub-TLV with invalid CRC value
 *                                ESCAN00113814 Time master transmits FollowUp message containing an AUTOSAR OFS
 *                                              Secured Sub-TLV with invalid CRC value
 *  15.02.00  2023-02-21  vissem  SAA-3366      Use Global Time for Pdelay Timestamps
 *  15.02.01  2023-03-09  vissem  ESCAN00114074 FollowUp message is not forwarded/transmitted on every bridge master
 *                                              port
 *                                ESCAN00114108 Pdelay initiator is unable to calculate Pdelay
 *                                ESCAN00114109 Pdelay responder does not transmit Pdelay_Resp_Fup
 *  15.02.02  2023-04-06  vissem  ESCAN00114331 Pdelay initiator does not close the critical section TX
 *  15.02.03  2023-06-05  vissem  ESCAN00114616 Pdelay_Req SequenceId is not incremented after Pdelay calculation
 *                                              failed
 *                                ESCAN00114746 Out-of-bounds access around EthTSyn_ComCtrlPhysPort[]
 *  16.00.00  2023-07-11  vissem  SAA-3918      Remove special handling (SwtSyncFrame) to get the host port ingress
 *                                              timestamp
 *            2023-08-10  vissem  SAA-3610      Improvements in Timestamp and SyncSendSm units
 *  16.01.00  2023-10-12  vissem  SAA-4222      Unit: SiteSyncSyncTx
 *            2023-10-16  vissem  SAA-4224      Unit: SiteSyncSyncTlv
 *            2023-10-31  vissem  SAA-4226      Integration of SiteSyncSyncTx and SiteSyncSyncTlv units
 *            2023-11-27  vissem  ESCAN00116041 'rxPTPPacketDiscardCount' is incremented due to  FollowUp timeout
 *                                              although valid FollowUp was received within timeout
 *            2023-12-06  vissem  ESCAN00116237 Potential undefined behavior due to signed integer arithmetic overflow
 *  17.00.00  2023-12-12  vissem  SAA-3788      Enhance TLV processing
 *            2024-01-16  vissem  ESCAN00116430 FollowUp message with valid CRC is discarded by a time slave or time
 *                                              aware bridge when correction field is included in CRC and correction
 *                                              field includes fractional nanoseconds
 *            2024-01-18  vissem  ESCAN00116496 Time master transmits FollowUp message with invalid CrcTime1 value
 *                                              when message length is included in CRC calculation
 *********************************************************************************************************************/
#if !defined(ETHTSYN_H)
# define ETHTSYN_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* Include external headers of the Units. */
# include "EthTSyn_Init.h"
# include "EthTSyn_VersionInfo.h"
# include "EthTSyn_ComCtrl.h"
# include "EthTSyn_PortParamStats.h"
# include "EthTSyn_Cbk.h"
# include "SchM_EthTSyn.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* vendor and module identification */
# define ETHTSYN_VENDOR_ID                                            (30u)
# define ETHTSYN_MODULE_ID                                            (164u)
# define ETHTSYN_INSTANCE_ID                                          (0U)

/* AUTOSAR Software specification version information */
# define ETHTSYN_AR_RELEASE_MAJOR_VERSION                             (0x04u)
# define ETHTSYN_AR_RELEASE_MINOR_VERSION                             (0x04u)
# define ETHTSYN_AR_RELEASE_REVISION_VERSION                          (0x00u)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define ETHTSYN_SW_MAJOR_VERSION                                     (17u)
# define ETHTSYN_SW_MINOR_VERSION                                     (0u)
# define ETHTSYN_SW_PATCH_VERSION                                     (0u)

/* ----- API service IDs ----- */
/* AR */
/*!< Service ID: EthTSyn_Init() */
# define ETHTSYN_SID_INIT                                             (0x01u)
/*!< Service ID: EthTSyn_GetVersionInfo() */
# define ETHTSYN_SID_GET_VERSION_INFO                                 (0x02u)
/*!< Service ID: EthTSyn_SetTransmissionMode() */
# define ETHTSYN_SID_SET_TRANSMISSION_MODE                            (0x05u)
/*!< Service ID: EthTSyn_RxIndication() */
# define ETHTSYN_SID_RX_INDICATION                                    (0x06u)
/*!< Service ID: EthTSyn_TxConfirmation() */
# define ETHTSYN_SID_TX_CONFIRMATION                                  (0x07u)
/*!< Service ID: EthTSyn_TrcvLinkStateChg() */
# define ETHTSYN_SID_TRCV_LINK_STATE_CHG                              (0x08u)
/*!< Service ID: EthTSyn_MainFunction() */
# define ETHTSYN_SID_MAIN_FUNCTION                                    (0x09u)

/* Non-AR */
/*!< Service ID: EthTSyn_Rx_IntProcRcvdMsg() */
# define ETHTSYN_SID_RX_INT_PROC_RCVD_MSG                             (0x50u)
/*!< Service ID: EthTSyn_SwtMgmt_MainFunction() */
# define ETHTSYN_SID_SWT_MGMT_MAIN_FUNCTION                           (0x51u)
/*!< Service ID: EthTSyn_SlaveRx_ProcRcvdSync() */
# define ETHTSYN_SID_SLAVE_RX_PROC_RCVD_SYNC                          (0x52u)
/*!< Service ID: EthTSyn_SlaveRx_FwdRcvdSync() */
# define ETHTSYN_SID_SLAVE_RX_FWD_RCVD_SYNC                           (0x53u)
/*!< Service ID: EthTSyn_SwtMgmtQ_Push() */
# define ETHTSYN_SID_SWT_MGMT_Q_PUSH                                  (0x54u)
/*!< Service ID: EthTSyn_GetAndResetPortParameterStatistics() */
# define ETHTSYN_SID_GET_AND_RESET_PORT_PARAM_STATS                   (0x55u)
/*!< Service ID: EthTSyn_Ts_IsTsValid() */
# define ETHTSYN_SID_TS_IS_TS_VALID                                   (0x56u)
/*!< Service ID: EthTSyn_Ts_IsStbmTsValid() */
# define ETHTSYN_SID_TS_IS_STBM_TS_VALID                              (0x57u)
/*!< Service ID: EthTSyn_Ts_TsToIntVltChecked() */
# define ETHTSYN_SID_TS_TS_TO_INT_VLT_CHECKED                         (0x58u)

/* ----- Error codes ----- */
/* AR runtime errors */
/*!< Error code: Time Master Conflict */
# define ETHTSYN_E_TMCONFLICT                                         (0x01u)
/*!< Error code: Time Slave Conflict */
# define ETHTSYN_E_TSCONFLICT                                         (0x02u)

/* AR development errors */
/*!< Error code: API service used in un-initialized state */
# define ETHTSYN_E_NOT_INITIALIZED                                    (0x20u)
/*!< Error code: EthTSyn initialization failed */
# define ETHTSYN_E_INIT_FAILED                                        (0x21u)
/*!< Error code: API called with invalid controller index */
# define ETHTSYN_E_CTRL_IDX                                           (0x22u)
/*!< Error code: API called with invalid Pointer */
# define ETHTSYN_E_PARAM_POINTER                                      (0x23u)
/*!< Error code: API called with invalid parameter */
# define ETHTSYN_E_PARAM                                              (0x24u)

/* Non-AR */
/*!< no error occurred */
# define ETHTSYN_E_NO_ERROR                                           (0x00u)
/*!< Error code: Received message with an invalid Message length */
# define ETHTSYN_E_MSG_LENGTH                                         (0x30u)
/*!< Error code: Received message with an invalid Protocol Version */
# define ETHTSYN_E_PROTOCOL_VERSION                                   (0x31u)
/*!< Error code: Received message with an invalid Message Type */
# define ETHTSYN_E_MSG_TYPE                                           (0x32u)
/*!< Error code: Received message with an invalid Frame Type */
# define ETHTSYN_E_FRAME_TYPE                                         (0x33u)
/*!< Error code: Received message with an invalid Domain Number */
# define ETHTSYN_E_DOMAIN_NUMBER                                      (0x34u)
/*!< Error code: RxIndication called for inactive controller */
# define ETHTSYN_E_TRCV_DOWN                                          (0x35u)
/*!< Error code: No matching EthTSyn port found */
# define ETHTSYN_E_NO_PORT_FOUND                                      (0x36u)
/*!< Error code: Generated FollowUp Tx length is invalid */
# define ETHTSYN_E_FUP_TX_LENGTH                                      (0x37u)
/*!< Error code: API called with invalid port index */
# define ETHTSYN_E_PORT_IDX                                           (0x38u)
/*!< Error Code: Invalid format of timestamp provided by StbM/EthIf or received in a message */
# define ETHTSYN_E_INV_TS_FORMAT                                      (0x39u)
/*!< Error Code: No free message buffer available */
# define ETHTSYN_SWT_MGMT_E_MSG_BUFFER_OVERFLOW                       (0x50u)
/*!< Error Code: Payload to big for the message buffer */
# define ETHTSYN_SWT_MGMT_E_MSG_BUFFER_PAYLOAD_OVERFLOW               (0x51u)
/*!< Error Code: EthIf_GetRxMgmtObject() returned with error. */
# define ETHTSYN_SWT_MGMT_E_GET_RX_MGMT_OBJ_FAILED                    (0x52u)
/*!< Error Code: Invalid SwtMgmtQ free buffer index during EthTSyn_SwtMgmtQ_Push(). */
# define ETHTSYN_SWT_MGMT_E_SWT_MGMT_Q_INV_FREE_BUF_IDX               (0x53u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define ETHTSYN_START_SEC_CODE
# include "EthTSyn_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ----- Service function prototypes of Scheduling ---------------------- */
/**********************************************************************************************************************
 *  EthTSyn_MainFunction
 *********************************************************************************************************************/
/*! \fn          void EthTSyn_MainFunction(void)
 *  \brief       Handles cyclic tasks of the EthTSyn module.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
/* Function declaration is provided by SchM */

/**********************************************************************************************************************
 *  EthTSyn_SwtMgmt_MainFunction
 *********************************************************************************************************************/
/*! \fn          void EthTSyn_SwtMgmt_MainFunction(void)
 *  \brief       Handles the switch management Rx/Tx management objects used by the EthTSyn and processes received
 *               messages.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
/* Function declaration is provided by SchM */

# define ETHTSYN_STOP_SEC_CODE
# include "EthTSyn_MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * EXCLUSIVE AREA DEFINITION
 *********************************************************************************************************************/
/*!
 * \exclusivearea ETHTSYN_EXCLUSIVE_AREA_0
 *    Ensures consistency of all global variables of the EthTSyn.
 * \protects All global variables of the EthTSyn.
 * \usedin EthTSyn_MainFunction, EthTSyn_SwtMgmt_MainFunction, EthTSyn_TrcvLinkStateChg, EthTSyn_SetTransmissionMode,
 *         EthTSyn_RxIndication, EthTSyn_TxConfirmation, EthTSyn_GetAndResetPortParameterStatistics
 * \exclude All functions provided by EthTSyn.
 * \length LONG nested exclusive areas.
 * \endexclusivearea
 *
 * \exclusivearea ETHTSYN_EXCLUSIVE_AREA_RX
 *    Ensures consistency of all global variables of the EthTSyn.
 * \protects All global variables of the EthTSyn.
 * \usedin EthTSyn_MainFunction, EthTSyn_SwtMgmt_MainFunction, EthTSyn_TrcvLinkStateChg, EthTSyn_RxIndication,
 *         EthTSyn_TxConfirmation
 * \exclude Ethernet Rx interrupts (EthTSyn_RxIndication).
 * \length LONG nested exclusive areas.
 * \endexclusivearea
 *
 * \exclusivearea ETHTSYN_EXCLUSIVE_AREA_TX
 *    Ensures consistency of all global variables of the EthTSyn.
 * \protects All global variables of the EthTSyn.
 * \usedin EthTSyn_MainFunction, EthTSyn_SwtMgmt_MainFunction, EthTSyn_TrcvLinkStateChg, EthTSyn_RxIndication,
 *         EthTSyn_TxConfirmation
 * \exclude Ethernet Tx interrupts (EthTSyn_TxConfirmation).
 * \length LONG nested exclusive areas.
 * \endexclusivearea
 *
 * \exclusivearea ETHTSYN_EXCLUSIVE_AREA_LINK_STATE
 *    Ensures consistency of all global variables of the EthTSyn.
 * \protects All global variables of the EthTSyn.
 * \usedin EthTSyn_TrcvLinkStateChg, EthTSyn_TxConfirmation, EthTSyn_RxIndication, EthTSyn_MainFunction,
 *         EthTSyn_SwtMgmt_MainFunction
 * \exclude EthTSyn_TrcvLinkStateChg.
 * \length MEDIUM call to different unit (nesting 1).
 * \endexclusivearea
 *
 * \exclusivearea ETHTSYN_EXCLUSIVE_AREA_GET_TS
 *    Protects retrieval of reference times against interrupts to minimize the time delay.
 * \protects Timestamp retrieval
 * \usedin EthTSyn_RxIndication, EthTSyn_MainFunction, EthTSyn_SwtMgmt_MainFunction, EthTSyn_TxConfirmation
 * \exclude All interrupts
 * \length LONG StbM and EthIf are called within this exclusive area.
 * \externalcall StbM_GetCurrentVirtualLocalTime, EthIf_GetCurrentTime
 * \endexclusivearea
 */

#endif /* ETHTSYN_H */
/**********************************************************************************************************************
 *  END OF FILE: EthTSyn.h
 *********************************************************************************************************************/
