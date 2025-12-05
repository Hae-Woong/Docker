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
/**        \file  Dcm.h
 *         \unit  NoUnit
 *        \brief  Public interface of DCM for other components
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/* *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author     Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2012-08-20  vissa      ESCAN00057163 AR4-92: DCM 4.x
 *                        vissa      N/A           First release
 *  01.01.00  2012-09-20  vissa      ESCAN00061110 Compiler warning: Function Dcm_ModeTaskMonitor() declared but never referenced
 *                        vissa      ESCAN00061115 Compiler warning: Function type does not match pointer declaration
 *                        vissa      ESCAN00061329 AR4-223: Support of diagnostic services 0x2A, 0x19, 0x2F
 *                        vissa      ESCAN00061347 Compiler error: BswM for AR 4.0.3 does not compile
 *                        vissa      ESCAN00061543 Os exception "WaitEvent called with disabled interrupts" due to critical section handling
 *                        vissa      ESCAN00061609 Dcm shall always reset the reprogramming flag after transition from FBL into the application software
 *                        vissa      ESCAN00061613 Wrong sub-function for any SID other than 0x10 is sent in the positive response after FBL->application software transition
 *                        vissa      ESCAN00061616 Dcm shall be able to perform a transition to non-default session at ECU initialization time
 *                        vissa      ESCAN00061626 Support for jump into FBL after the final response to SID 0x10 is sent
 *                        vissa      ESCAN00061632 Rte_Switch API evaluated for possible error return values
 *                        vissa      ESCAN00061633 Explicit compiler optimization inhibition by volatile qualified variables
 *                        vissa      ESCAN00061642 Link error: Undefined function Dcm_Svc19UtiDemGetExtRecord
 *                        vissa      ESCAN00061645 Long time security access execution rejection due to wrongly started penalty timers
 *                        vissa      ESCAN00061646 Minor ROM/RAM usage, ISR latency and overall run-time usage improvements
 *                        vissa      ESCAN00061659 Change the FreezeFrame and ExtDataRecord number list termination from 0x00 to 0xFF to match OBD2 requirements
 *  01.01.01  2012-10-12  vissa      ESCAN00061970 Wrongly reported DET within the Dcm_TpRxIndication function
 *                        vissa      ESCAN00062060 Dcm causes Dem to report a Det on diagnostic request 0x19 0x06/0x10 [DTC] 0xFE
 *                        vissa      ESCAN00062169 Dcm responds with an wrongly formatted positive response on an invalid IO control parameter
 *                        vissa      ESCAN00062556 Dcm service 0x2F does not support enable mask record
 *                        vissa      ESCAN00062557 Compile error for service 0x2A and enabled DET
 *                        vissa      ESCAN00062558 DET called if more than 128 tester connections or communication channels are to be supported
 *  01.02.00  2013-01-31  vissa      ESCAN00062720 AR4-288: RfC 54767 - [Dcm]: Description of parameter opStatus does not match with syntax of C/S-Interface and C-API
 *                        vissa      ESCAN00062738 Compiler warning: 'initializing' : conversion from 'PduLengthType' to 'Dcm_CfgNetBufferSizeOptType', possible loss of data
 *                        vissa      ESCAN00062999 Wrong opStatus parameter value on DataService_ReadData operation call for SID 0x22 with multiple DIDs
 *                        vissa      ESCAN00063465 Add module version information for delivery version checks
 *                        vissa      ESCAN00063613 SYS notifications shall be called after the SID session and security checks
 *                        vissa      ESCAN00063614 Optimization: Unused OEM and SYS notifications management code
 *                        vissa      ESCAN00063615 Compiler warning: Dcm_Svc2AReadData declared but not referenced
 *                        vissa      ESCAN00063741 Compiler error: "doSuppressResponse" is not a member of DCM_DIAGCONTEXTTYPE_TAG
 *                        vissa      ESCAN00064607 Provide periodic reading speed limitation
 *                        vissa      ESCAN00064628 The ECU does not respond on a diagnostic request with required response
 *                        vissa      ESCAN00064643 Dcm accepts "send key" request after "zero seed" response for the same level
 *                        vissa      ESCAN00064721 Compiler warning: unreferenced parameter in function Dcm_TriggerTransmit
 *  01.03.00  2013-04-17  vissa      ESCAN00065115 Compiler warning: static function Dcm_Svc2ASchdCancelReadByDid declared but not referenced
 *                        vissa      ESCAN00065478 Compiler error: function Dcm_Svc2FCheckAccess not defined
 *                        vissa      ESCAN00066711 Dcm reports DTC from primary memory instead of mirror memory on requested service 0x19 0x0F
 *                        vissa      ESCAN00066728 Service 0x27 sends NRC 0x36 on first failed send key attempt
 *                        vissa      ESCAN00066730 Service 0x27 sends NRC 0x37 for a security level that is not yet locked by penalty time
 *                        vissa      ESCAN00066759 Dcm uses wrong txPduId in the Dcm_TpTxConfirmation for failed USDT transmission attempts
 *                        vissa      ESCAN00066837 Add consistency check for service 0x2A and at least one periodic transmission channel
 *                        vissa      ESCAN00066864 Invalid request type transfered to the OEM/SYS specific confirmation notifications
 *                        vissa      ESCAN00066865 Modifications for improved MISRA compliance
 *                        vissa      ESCAN00067397 Dcm accepts a send key request after a zero seed positive response for the same security level
 *  01.03.01  2013-05-28  vissa      ESCAN00067694 The ECU sends endless RCR-RP responses on request 0x10 0x02
 *  01.04.00  2013-06-17  vissa      ESCAN00067473 Provide Dcm_CommunicationModeType unconditionally
 *                        vissa      ESCAN00068082 AR4-81: Support of OBDII w/o Multiprotocol
 *                        vissa      ESCAN00068171 Dcm deadlock on forced RCRRP and reaching RCRRP limit
 *                        vissa      ESCAN00068374 Provide means for calibrate-able OBD2 support
 *                        vissa      ESCAN00068376 Support use case enable rapid shutdown without a concrete time
 *                        vissa      ESCAN00068411 During waiting for processing end (reset) delegate the repeater proxy to a dead end
 *  01.05.00  2013-07-26  vissa      ESCAN00067206 Service 0x2F does not return the actual IOport values in the positive response
 *                        vissa      ESCAN00068083 AR4-81: Support of OBDII with Multiprotocol
 *                        vissa      ESCAN00069349 Dcm sends a negative response even the called C/S port has returned E_OK
 *                        vissa      ESCAN00069674 Do not throw DET exception in case of Dcm_CopyTxData with SduLength == 0
 *                        vissa      ESCAN00069828 Removed function Dcm_OnTimeoutSvc2AScheduler when not used
 *                        vissa      ESCAN00069829 Make DID data signal operation CheckConditionRead optional
 *                        vissa      ESCAN00069830 SID 0x2F (IOControlByDataIdentifier) shall not allow dynamic length signals
 *                        vissa      ESCAN00069849 NRC 0x10 not send on canceled SID 0x19 with paged buffer support
 *                        vissa      ESCAN00069852 Concurrent client requests are not rejected with NRC 0x21
 *                        vissa      ESCAN00069854 No response on some diagnostic clients even if DCM is free
 *                        vissa      ESCAN00069855 Improved RCR-RP limitation notification within the DCM internal code
 *                        vissa      ESCAN00069857 Service 0x2F could accept invalid IOControlOperation with ID greater than 0x03
 *                        vissa      ESCAN00069858 DCM does not reset the changed IO ports on transition to the default session
 *                        vissa      ESCAN00069859 DIDs defined by service 0x2C does not have correct data or cannot be read at all
 *                        vissa      ESCAN00069867 Paged-buffer access not protected against early response transmission termination
 *                        vissa      ESCAN00069924 While in a non-default session Dcm accepts diagnostic requests from clients other than the one that has started the session
 *                        vissa      ESCAN00069934 Do not call Xxx_MODE_DcmEcuReset_EXECUTE for the rapid power shutdown resets
 *                        vissa      ESCAN00069958 Dcm restarts the S3 timer after RoE responses
 *                        vissa      ESCAN00070056 Dcm either does not send the final positive response after FBL reset at all or sends it to the wrong tester
 *                        vissa      ESCAN00070110 Dcm may not send final FBL response after the reprogramming sequence
 *                        vissa      ESCAN00070113 Dcm executes RoE service to respond to during a non-full communication mode
 *  02.00.00  2013-07-18  vissa      ESCAN00068252 AR4-371: Support signal interface
 *                        vissa      ESCAN00070601 Tester connection released immediately after FBL positive response
 *                        vissa      ESCAN00070741 Remove IODID from the session change reset queue on ReturnControlToEcu operation execution
 *                        vissa      ESCAN00070743 Run time limitation performed on any diagnostic service
 *                        vissa      ESCAN00070879 OEM/System-Supplier confirmations called with wrong SID parameter value
 *                        vissa      ESCAN00070980 Compiler error: Dcm does not compile if service 0x2F is to be supported
 *                        vissa      ESCAN00070981 Compiler error: Dcm does not compile if service 0x2C is to be supported
 *  02.00.01  2013-12-05  vissa      ESCAN00072164 Dcm sends RCR-RP or BusyRepeatRequest (NRC 0x21) responses with wrong SID
 *                        vissa      ESCAN00072187 Dcm returns BUFREQ_E_BUSY on zero SduLength function argument
 *                        vissa      ESCAN00072512 SID 0x2A: No periodic DID can be read
 *                        vissa      ESCAN00072525 UUDT periodic message transmission has no timeout monitoring
 *                        vissa      ESCAN00072616 Avoid coping of response data during paged buffer defragmentation
 *  02.01.00  2013-12-03  vissa      ESCAN00071362 AR4-569: Reject diagnostic requests if the ecu resets with service 0x11
 *                        vissa      ESCAN00071382 AR4-465: Support for J1939 Diagnostics - API to DEM according AR 4.1.2
 *                        vissa      ESCAN00072966 SID 0x2A: periodic message transmission causes nondeterministic behavior due to corrupted memory
 *                        vissa      ESCAN00073064 Dcm does not accept diagnostic requests when started in a non-default session
 *                        vissa      ESCAN00073092 No Dem included when only OBD and no UDS DTC related service active
 *                        vissa      ESCAN00073093 Suppress NRCs: 0x7E,0x7F on functional requests
 *                        vissa      ESCAN00073094 Compile error: Macro Dcm_UtiAssignStruct uses braces for the right-hand side expression
 *                        vissa      ESCAN00073095 SID 0x19 0x05: DEM return WRONG_RECORD to be considered as DET situation
 *                        vissa      ESCAN00073096 SID 0x19: Support new ISO14229-1:2013 sub-functions: 0x17,0x18 and 0x19
 *                        vissa      ESCAN00073097 Removed useless assignment in Dcm_Svc2FReturnControlToEcu
 *                        vissa      ESCAN00073295 DCM invokes DET for request indication function returned REQUEST_NOT_ACCEPTED without setting any NRC
 *                        vissa      ESCAN00073506 SID 0x28: Allow user implementation of sub-functions 0x04 and 0x05
 *                        vissa      ESCAN00073510 SID 0x2C: Reserves too much RAM for temporary reading of static DIDs
 *                        vissa      ESCAN00073534 Dcm does not accept any new diagnostic request after FBL final response
 *                        vissa      ESCAN00073538 Possible S3 timeout while processing first diagnostic request after FBL final response
 *                        vissa      ESCAN00073548 Remove unused code for final FBL response cancellation
 *  02.02.00  2014-04-11  vissa      ESCAN00074268 ECU sends RCR-RP responses endlessly for SID 0x27 for get seed
 *                        vissa      ESCAN00074270 ECU sends RCR-RP responses endlessly for SID 0x31
 *                        visrpk     ESCAN00074999 Support NvRam signal access for DIDs
 *                        vissa      ESCAN00075355 Dcm accepts new requests while waiting for jump into the FBL on SID 0x10 0x02/0x82
 *                        vissa      ESCAN00075443 Changed NRC behavior for services 0x19 0x06, 0x10 and 0x19
 *                        vissa      ESCAN00075554 Optimized code overhead for DID related services
 *                        vissa      ESCAN00076173 Unexpected response for service 0x19 0x05 0x00
 *                        vissa      ESCAN00076270 ECU sends NRC 0x11 for valid requests after a RCR-RP has been sent
 *                        vissa      ESCAN00076276 Use Dcm_UtiAssignStruct in Dcm_TriggerTransmit
 *                        vissa      ESCAN00076317 Dcm calls DEM SetDTCFilter API with wrong DTC kind for service 0x19 0x15
 *                        vissa      ESCAN00076318 Compiler warning: 'opStatus' : unreferenced formal parameter in Dcm_Svc22CheckCondition
 *  02.02.01  2014-07-31  vissa      ESCAN00067170 Compiler warning: cast truncates constant value
 *                        vissa      ESCAN00076755 A request from a different protocol interrupts a current ongoing service processing independent of protocol priority
 *                        vissa      ESCAN00076810 A 0x19 0x02 request leads to an endless pending DCM
 *                        vissa      ESCAN00076956 DCM responds with wrong NRC 0x22 if CompareKey Operation returns E_NOT_OK
 *                        vissa      ESCAN00077499 Wrongly reported DET within the Dcm_TpRxIndication function
 *                        vissa      ESCAN00077676 ECU may send two RCR-RP back-to-back responses
 *                        vissa      ESCAN00077682 Fast post-processors execution shall not be skipped on protocol interruption
 *                        vissa      ESCAN00077691 Minor CPU load reduction on back-to-back requests of different diagnostic clients
 *                        vissa      ESCAN00077692 After protocol interruption an application function can be called unexpectedly with OpStatus==DCM_FORCE_RCRRP_OK
 *                        vissa      ESCAN00077704 ECU may send the complete multi-frame response of interrupted diagnostic job
 *                        vissa      ESCAN00077735 Updating paged data for service 0x19 is not always stopped on protocol preemption
 *                        vissa      ESCAN00077736 Compiler warning: conditional expression is constant
 *  02.02.02  2014-08-11  vissa      ESCAN00077740 Application called with wrong opStatus set to DCM_PENDING instead of DCM_INITIAL
 *                        vissa      ESCAN00077741 DCM performs invalid data access while processing service 0x22
 *                        vissa      ESCAN00077763 ECU sends positive response even if SPRMIB was set and no RCR-RP response has been sent
 *  03.00.00  2014-10-30  vissa      ESCAN00076508 AR4-538: Support post-build loadable for communication
 *                        vissa      ESCAN00076511 AR4-572: Feature Complete - Service 0x24
 *                        vissa      ESCAN00076532 Incorrect timeout detection of UUDT a transmission
 *                        vissa      ESCAN00077644 Security access sequence not reset on SID 0x27 request with total length of only one byte sent
 *                        vissa      ESCAN00078135 Wrong DET-SID used for DET reports in API Dcm_GetActiveProtocol
 *                        vissa      ESCAN00078137 Diagnostic client cannot communicate with the ECU
 *                        vissa      ESCAN00078139 AR4-698: Support post-build selectable for communication
 *                        vissa      ESCAN00078707 ECU always sends NRC 0x21 on clients with protocol priority set to 255
 *                        vissa      ESCAN00079241 Unified internal and external diagnostic request reception behavior
 *                        vissa      ESCAN00079242 Compiler warning: in Dcm_Svc22GetLength 'opStatus': unreferenced formal parameter
 *  03.01.00  2014-11-11  visvkr     ESCAN00076193 ECU returns NRC 0x13 instead of 0x7E for services with a sub-function parameter
 *                        visvkr     ESCAN00079197 ECU returns NRC 0x13 instead of 0x31 for services without a sub-function parameter
 *                        vissa      ESCAN00079540 FEAT-520: ModeDeclarationGroup DcmResponseOnEvent_<RoeEventID>
 *                        vissa      ESCAN00079541 FEAT-627: DID_Ranges
 *                        visvkr     ESCAN00079847 Support DID 0xF186 (ActiveDiagnosticSessionDataIdentifier)
 *                        vissa      ESCAN00079959 The ECU cannot process any interrupt driven event
 *                        visvkr     ESCAN00080237 Service 0x19 0x05 0xFF shall return positive response for SSR 0x00
 *                        vissa      ESCAN00080581 Support for DCM interaction with AR3 BSWs
 *                        vissa      ESCAN00080782 ECU will not go back to the default session if an RoE event occurs during active non-default session
 *  04.00.00  2015-01-30  vissa      ESCAN00079600 FEAT-1154: DCM AR3 BSW environment compatibility
 *                        visvkr     ESCAN00080037 Support general suppression on functional requests
 *                        vissa      ESCAN00080282 FEAT-427: SafeBSW Step I
 *                        vissa      ESCAN00080700 DIDranges ReadDidData returned length validated upon maximum configured length
 *                        vissa      ESCAN00080812 Provide notification functions for session and security transitions and API to get the tester source address from the RxPduId
 *                        vissa      ESCAN00080846 ECU does not respond to a diagnostic request after FBL/APPL software context switch
 *                        vissa      ESCAN00080907 DCM rejects a direct memory access request with valid ALFID parameter value 0x[1-4]5 with NRC 0x31
 *                        vissa      ESCAN00081392 Support DCM 4.2.1 Dem_DcmClearDTC return value DEM_CLEAR_MEMORY_ERROR
 *                        vissa      ESCAN00081439 Possible request data corruption after an interrupted by tester paged-buffer response
 *                        vissa      ESCAN00081541 Use post-build variant specific number of transport objects
 *                        vissa      ESCAN00081663 Compiler warning: conversion from 'const Dcm_CfgDidMgrAbstractOpRefType' to 'Dcm_CfgDidMgrDynDidHandleMemType', possible loss of data
 *                        vissa      ESCAN00081913 Compiler error: Dcm_TriggerTransmit does not fit the PduR function signature for <UpLayer>_TriggerTransmit
 *                        vissa      ESCAN00081917 Service 0x2F might not execute the requested IOcontrol operation on requests without ControlEnableMaskRecord (CEMR)
 *                        vissa      ESCAN00081919 Service 0x2F executes the read DID operation even when the control operation has failed
 *                        vissa      ESCAN00081998 Add de-initialization API Dcm_DeInit
 *                        vissa      ESCAN00082008 Possible undefined behavior of the ECU after request for SID 0x27
 *                        vissa      ESCAN00082016 The ECU might not process all IO controllable signals of a SID 0x2F request with CEMR
 *  04.01.00  2015-04-07  vissa      ESCAN00073619 Service 0x19 0x04/0x06/0x10 with RecID 0xFF has different behavior depending on the PagedBuffer support
 *                        vissa      ESCAN00073625 Service 0x19 0x18/0x19 with RecID 0xFF has different behavior depending on the PagedBuffer support
 *                        vissa      ESCAN00075748 Provide support for service 0x2C 0x02 without the need of service 0x23
 *                        vissa      ESCAN00082102 Remove unnecessary DET check from Dcm_CopyRxData
 *                        vissa      ESCAN00082104 An UUDT transmission can be confirmed by an USDT PduId from the same connection
 *                        vissa      ESCAN00082303 The ECU responds with NRC 0x21 to the tester that has initiated a reset command
 *                        vissa      ESCAN00082337 Service 0x23 may cause memory out of boundary write access
 *                        visade     ESCAN00082354 FEAT-397: Support of WWH-OBD in Dcm
 *                        visvkr     ESCAN00082369 FEAT-461: Add extensions to DCM
 *                        visvkr     ESCAN00082371 FEAT-861: Keep awake after Diag [AR4-872]
 *                        visvkr     ESCAN00082372 FEAT-1333: Provide possibility to control current state of DCM & DEM by application SW-C or CDD
 *                        visvkr     ESCAN00082373 FEAT-1349: Provide paged data support for DIDs
 *                        vismhe     ESCAN00082378 FEAT-1476: Support Security Fixed Bytes
 *                        vissa      ESCAN00082440 Compiler warning: unreferenced parameter "txPduId" in function Dcm_TriggerTransmit
 *                        vissa      ESCAN00082526 Service 0x28 may disrupt ECU communication when user defined sub-function implementation is configured
 *                        vissa      ESCAN00082794 SID 0x22 and 0x2A consider DID not supported in session state twice
 *                        vissa      ESCAN00082932 DCM may perform a transition to the default session twice
 *                        vissa      ESCAN00082952 ECU may unexpectedly send a positive response after power-on/reset
 *                        vissa      ESCAN00083483 Deactivated OBD DIDs still accessible for service 0x2C
 *                        vissa      ESCAN00083597 ECU sends endlessly RCR-RP (NRC 0x78) responses
 *                        vissa      ESCAN00083615 Make EcuReset/RapidPowerShutDown mode switching more reliable
 *                        vissa      ESCAN00083808 Unpredictable behavior on a DID read/write/define operation request
 *                        vissa      ESCAN00083985 Compiler warning: implicit cast of unsigned integer to unsigned char/short
 *  04.01.01  2015-09-10  visvkr     ESCAN00084477 Dcm_ProvideRecoveryStates API always returns DCM_E_NOT_OK
 *                        vissa      ESCAN00084976 ECU sends final response to SID 0x19 even if the SPRMIB was set
 *                        visvkr     ESCAN00085052 Compiler error: Unknown function name Dcm_DidMgrCancelOperation
 *                        vissa      ESCAN00085100 DTC record update remains disabled after request for service 0x19 0x04/0x06/0x10/0x18/0x19
 *                        visvkr     ESCAN00085101 Wrong behavior on simultaneous reading of the same DID by services 0x2A and 0x22
 *                        vissa      ESCAN00085131 Compiler error: Service 0x85 processor uses unknown structure element DtcGroup
 *  05.00.00  2015-11-27  vissa      ESCAN00080649 Service 0x28 will be reset (re-enable communication) also on not modified channels
 *                        vissa      ESCAN00084906 Service 0x2A: Remove single periodic DID optimization
 *                        vissa      ESCAN00084913 FEAT-1593: Variant handling in runtime via API (DCM)
 *                        vissa      ESCAN00085142 FEAT-1527: SafeBSW Step 3
 *                        vissa      ESCAN00085183 ECU responds with NRC 0x72 instead of NRC 0x22 for SID 0x04
 *                        vissa      ESCAN00085339 ECU may send empty positive response for service 0x19 0x05 0x00
 *                        vissa      ESCAN00085364 Make DEM API version 4.2.1 explicitly configurable
 *                        vissa      ESCAN00085545 FEAT-1660: RfC 57196 - [Dcm] doubts to usage of NRC 0x72 for Dcm_ReadMemory()
 *                        vissa      ESCAN00085656 Service 0x27: the ECU returns unexpectedly NRC 0x36 instead of 0x35
 *                        vissa      ESCAN00085705 Service 0x2E: ECU sends NRC 0x13 to a valid request for a DID from a configured DID range
 *                        vissa      ESCAN00085778 Service 0x22: DET reported on DID range with DCM_E_PENDING usage
 *                        vissa      ESCAN00085781 Service 0x2C: Sub-function processors mapped to wrong memory section
 *                        vissa      ESCAN00085783 Service 0x3E: now its possible to define sub-service specific execution preconditions
 *                        vissa      ESCAN00085784 Unified service validation for services with a sub-function
 *                        vissa      ESCAN00085792 Service 0x27: avoided unnecessary RCR-RPs prior sub-function validation
 *                        vissa      ESCAN00085825 Service 0x2C: service specific minimum length check now done after execution precondition validation
 *                        vissa      ESCAN00085902 Possible incorrect DCM state restored after power on/reset
 *                        vissa      ESCAN00085903 DCM states not recovered after power on/reset
 *                        vissa      ESCAN00085904 Service 0x85 will be reset (re-enable DTC setting) even if not disabled
 *                        vissa      ESCAN00085950 RfC  61742 - [Dcm] Stop Dcm to call ComM_DCM_ActiveDiagnostic
 *                        vissa      ESCAN00086015 RfC  64765 - Functional Request via CAN-ID 0x7DF without any response CAN-ID
 *                        vissa      ESCAN00086068 Service 0x2F:  RfC  58607 - Necessity of DcmDspDataReturnControlToEcuFnc/operation ReturnControlToECU of interface DataServices_<Data>
 *                        vissa      ESCAN00086199 Reduced RAM usage for DIDs
 *                        vissa      ESCAN00086203 Removed DET checks for not initialized DCM for all service (post-)processors
 *                        vissa      ESCAN00086206 Positive response sent on unexpected RTE return value for Xxx_Indication
 *                        vissa      ESCAN00086207 Unified ECU behavior on any RTE/FNC API call with unexpected returned value
 *                        vissa      ESCAN00086317 Service 0x2F: automatic IOControl reset does not reset all IODID signals
 *                        vissa      ESCAN00086321 Random P2 timings responded in final response for SID 0x10 after FBL<->APPL transition
 *                        vissa      ESCAN00086322 Service 0x02: DEM API Dem_(Dcm)GetDTCOfOBDFreezeFrame now called only for PID 0x02
 *                        vissa      ESCAN00086323 Service 0x2F: positive response instead of NRC 0x13 for IODID with dynamic length
 *                        vissa      ESCAN00086346 Service 0x2F: unintended IO signals put/remained not under control
 *                        vissa      ESCAN00086347 Design change: handle DCM_CANCEL signal on the most top level
 *                        vissa      ESCAN00086763 Service 0x02 may overwrite two bytes behind the DCM buffer
 *                        vissa      ESCAN00086823 Compiler error: Mismatching pointer initialization value in Dcm.c
 *                        vissa      ESCAN00086853 Wrongly reported DET within the Dcm_Svc2ASchdCommit function
 *                        visvkr     ESCAN00087258 A switch to default session is not performed for the very first request after reset
 *                        visvkr     ESCAN00087259 Session specific P2 timings not updated when Dcm_ResetToDefaultSession() is called
 *                        visvkr     ESCAN00087261 Session specific P2 timings are not updated if jump from FBL is performed
 *                        visvkr     ESCAN00087262 Sessions specific P2 timings are not recovered on ECU Reset/Power On
 *  05.01.00  2016-02-28  vissa      ESCAN00086371 In an AR3 ComStack ECU does not accept a diagnostic request on all communication channels
 *                        vissa      ESCAN00087622 Compiler error: Undefined symbol Dcm_Service11FastPostProcessor
 *                        vissa      ESCAN00087635 FEAT-1730: Support bit-mapped IO control signals
 *                        vissa      ESCAN00087716 Service 0x2F: Optimized IO DID signal ROM and run time usage
 *                        vissa      ESCAN00087735 FEAT-507: DCM Support native API for MSR3 BSWs
 *                        vissa      ESCAN00087875 Compiler error: Undefined symbol DynDidHandleInUse
 *                        vissa      ESCAN00087923 Compiler warning: local variables copyLength and copyOffset is possibly not initialized
 *                        vissa      ESCAN00088345 Improved robustness against called APIs that can return E_NOT_OK and NRC
 *                        vissa      ESCAN00088507 FEAT-1866: SafeBSW Step 4
 *                        vissa      ESCAN00088510 Service 0x2F: Request for ShortTermAdjustment with no data will result in a positive response
 *                        vissa      ESCAN00088603 Not AR 4.x compatible interface implementation
 *                        vissa      ESCAN00088967 Service 0x2F: ECU sends NRC 0x33 prior NRC 0x13 in case of invalid request
 *  05.02.00  2016-04-07  vissa      ESCAN00088831 Service 0x03/0x04/0x07/0x0A: Returns NRC 0x13 instead of 0x31 resp. no response
 *                        vissa      ESCAN00088873 Service 0x09: Remove unused code from service processor
 *                        vissa      ESCAN00089327 Compiler error: Used empty expression Dcm_NetGetSessionConnection
 *                        vissa      ESCAN00089439 Service 0x22/0x2A: Unexpected response behavior for DDDIDs
 *                        vissa      ESCAN00089458 Compiler warning: Function Dcm_Svc11_ProcessEcuReset defined but never referenced
 *                        vissa      ESCAN00089460 Compiler warning: Relational operator '<' always evaluates to 'false'
 *                        vissa      ESCAN00089965 Compiler error: For invalid inline function usage'
 *  07.00.00  2016-05-02  vissa      ESCAN00089342 FEAT-1807: Implement DCM Multi Identity /selectable post build
 *                        vissat     ESCAN00089344 FEAT-1922: Support for OBD2 Mode 0x09 with variable length
 *                        vissat     ESCAN00089352 FEAT-1724: OBD: Support for DTR (Diag Test Results)
 *                        vissa      ESCAN00089383 FEAT-371: S/R Communication for DCM
 *                        vissa      ESCAN00089385 FEAT-1734: SafeBSW Step 5
 *                        vissa      ESCAN00089669 Service 0x3E 0x80 sent via functional addressing may result in negative response with NRC 0x21
 *                        vissa      ESCAN00090077 Service 0x2F: Active IO controls are not reset after session/security state change
 *                        vissa      ESCAN00090107 Compiler error: Wrong function prototype used for AR 3.x API NvM_GetErrorStatus
 *                        vissat     ESCAN00090556 Services 0x23/0x3D/0x2C 0x02: Dcm_Read-/WriteMemory AR 4.2.2 API
 *  07.01.00  2016-09-22  vissat     ESCAN00091059 Service 0x86: No RoE message sent on DTC status change after ECU reset
 *                        vissat     ESCAN00091212 Service 0x06: Possible buffer overflow on too small diagnostic buffer
 *                        vissat     ESCAN00092683 Service 0x22: NRC 0x31 sent back for valid OBD MID DID (0xF6XX)
 *                        vissa      ESCAN00092755 Unexpected DET error or communication behavior
 *                        vissa      ESCAN00092766 ECU stops responding to diagnostic clients and is not able to switch into sleep mode
 *                        vissa      ESCAN00092768 ECU does not send RCR-RP response after P2server time
 *                        vissat     FEAT-1841     Support Routine Info Byte
 *                        vissat     FEAT-1901     I/O Control S/R style
 *  07.02.00  2017-01-03  visvkr     ESCAN00087310 P2 timings are not recovered on ECU Reset/Power On (Multi Protocol)
 *                        visvkr     ESCAN00089919 Service 0x86: Check RxPduId read from NvM
 *                        visvkr     ESCAN00092914 Active protocol is not recovered on ECU Reset/Power On
 *                        vissa      ESCAN00093304 Improved performance for a forced RCR-RP during RoE STRT processing
 *                        vissa      ESCAN00093335 Service 0x10: Transition to/from FBL without final response
 *                        vissa      ESCAN00093886 Service 0x28: Dcm performs Rte_Switch with a wrong mode from the corresponding CommunicationControl ModeDeclarationGroup
 *                        vissa      ESCAN00093905 Service 0x19: Sub-functions  0x04/0x06/0x10/0x18/0x19 report zero values for snapshot/extended data records
 *                        vissa      ESCAN00093943 External sub-service processor API with pMsgContext->resDataLen == 0
 *                        vissa      ESCAN00093970 Removed API Dcm_DeInit()
 *                        vissat     FEAT-936      VSG support DCM
 *  08.00.00  2017-04-04  visstk     STORYC-461    SafeBSW for DCM - Full UDS - Other Services (0x11)
 *                        visvkr     STORYC-536    SafeBSW for DCM - Full UDS - Other Services (0x27 - Basic)
 *  08.01.00  2017-04-19  visygr     ESCAN00094908 Service 0x22: ROM und Runtime usage decrease
 *                        visygr     ESCAN00095065 Compiler error: Missing declaration for API BswM_Dcm_RequestResetMode()
 *  08.02.00  2017-05-19  vissa      STORYC-1145   Minor improvements ported from 5.03.00
 *  08.03.00  2017-06-19  visstk     ESCAN00095299 Compiler warning: Mismatch between signature of function declarations and definitions
 *                        visstk     ESCAN00095486 Compiler warning: Function 'Dcm_Svc14_XX_RepeaterProxySelectDTC' was declared but never referenced
 *                        visstk     ESCAN00095490 Compiler error: Cannot open include file: 'Det.h'
 *                        visstk     ESCAN00095508 Compiler warning: Function 'Dcm_MemMgrWriteMemory' was declared but never referenced
 *                        visstk     ESCAN00095513 Compiler warning: Function 'Dcm_MemMgrReadMemory' was declared but never referenced
 *                        visaey     STORYC-787    Boolean at Dcm DataElement S/R interface: DCM
 *                        visstk     STORYC-999    UUDT Sort
 *  08.04.00  2017-07-13  visygr     ESCAN00093906 ECU returns NRC 0x13 instead of 0x33 or other execution pre-condition related NRC for services with a sub-function parameter
 *                        visygr     ESCAN00095297 Service 0x27: Wrong prioritisation between NRC 0x24 and 0x13
 *                        visvkr     ESCAN00095919 Services 0x23/0x3D/0x2C: Unexpected response behavior on invalid memory Id request
 *                        visvkr     ESCAN00095963 Service 0x31: Erroneous implementation of request minimum length check
 *                        visstk     STORYC-1000   UUDT DelayTimer
 *  08.05.00  2017-08-07  viswsi     ESCAN00096188 Service 0x27: Remove support of external subservice handler
 *                        visygr     ESCAN00096207 Service 0x27: The attempt counters only the first eight security levels are read at power on/reset
 *                        visygr     ESCAN00096247 Service 0x22/0x2A/0x2F: ECU returns invalid data
 *                        visygr     ESCAN00096335 Compiler error: Unknown identifier Dcm_DemApiNrcMapSelectDTC
 *                        visygr     STORYC-2053   Rework Implementation
 *  08.06.00  2017-09-20  viswsi     STORYC-2092   SafeBSW for DCM - Full UDS - RID Services (0x31) Finalize
 *  09.00.00  2017-10-25  visuvo     STORYC-2642   SafeBSW for DCM - Full UDS - Memory Services (0x23)
 *                        visstk     STORYC-460    SafeBSW for DCM - Full UDS - Memory Services (0x3D)
 *  09.01.00  2017-11-06  visvkr     ESCAN00097053 Compiler error: Empty struct DCM_DIDMGROPTYPEREADCONTEXTTYPE_TAG
 *                        visvkr     ESCAN00097056 Compiler error: 'Offset' : is not a member of 'DCM_DIDMGROPTYPEREADCONTEXTTYPE_TAG'
 *                        viswsi     ESCAN00097111 Service 0x2C: Reading DDDID contains invalid response data
 *                        visvkr     ESCAN00097520 Callout Dcm_FilterDidLookUpResult() is called with unexpected OpStatus
 *                        visstk     STORYC-2319   DCM: Support 8-byte and 64-byte UUDT frames
 *                        visvkr     STORYC-3053   SafeBSW for DCM - Full UDS - DID Services Prio I (0x22 - Preparation)
 *  09.02.00  2017-11-29  visstk     ESCAN00097384 Service 0x22: Overwritten RAM behind a DCM buffer
 *                        visvkr     ESCAN00097692 Compiler warning: conversion from 'int' to 'Dcm_CfgNetBufferSizeOptType'
 *                        visstk     ESCAN00097735 Paged-Buffer: Positive response with corrupted data
 *                        visvkr     STORYC-3245   SafeBSW for DCM - Full UDS - DID Services Prio I (0x22 - Preparation II)
 *  09.03.00  2018-01-05  visygr     ESCAN00097731 Service 0x10: Jump to FBL performed although forced RCR-RP could not be sent
 *                        visstk     ESCAN00097767 Compiler warning: conversion from 'Dcm_UudtFrameSizeType' to 'Dcm_CfgNetBufferSizeMemType'
 *                        viswsi     ESCAN00097829 Service 0x22: Overwritten call stack
 *                        visvkr     STORYC-3484   Paged-Buffer: Improved transmission after DCM_E_PENDING
 *                        visahe     STORYC-3913   Extend DCM include hierarchy by a Base level
 *  09.04.00  2018-01-23  viswsi     ESCAN00097361 Service 0x2A: Wrong prioritisation between NRC 0x13 and 0x31
 *                        visvkr     ESCAN00098308 Service 0x86: Start RoE is not rejected after RoE has been cleared
 *                        viswsi     ESCAN00098361 Service 0x2F: Wrong ReturnControlToECU operations are called on session/security state change
 *                        visahe     STORYC-3192   DCM: RoE - Release
 *  09.05.00  2018-02-19  visahe     ESCAN00098203 Service 0x3E: S3 timer reloaded for different protocol
 *                        visahe     ESCAN00098392 S3 timer reloaded by any request from different tester
 *                        visahe     ESCAN00098604 Service 0x3E: Keep-Alive timer reloaded for different protocol
 *                        visahe     ESCAN00098606 Keep-Alive timer reloaded by any request from different tester
 *                        viswsi     STORYC-443    SafeBSW for DCM - Full UDS - DID Services Prio I (0x22 - Basic)
 *                        visuvo     STORYC-539    SafeBSW for DCM - Full UDS - DID Services Prio I (0x22 - Extended)
 *  10.00.00  2018-03-29  visvkr     ESCAN00099104 ECU sends endlessly RCR-RP (NRC 0x78) responses
 *                        visaey     STORYC-2694   SafeBSW for DCM - Full UDS - DID Services (0x24)
 *                        visahe     STORYC-2696   SafeBSW for DCM - Full UDS - DID Services Prio I (0x2E)
 *                        viswsi     STORYC-4733   Rework DataContext for better usability
 *                        visahe     STORYC-4977   STORYC-4977: SafeBSW for DCM - Full (WWH-)OBD - Mode 0x04
 *  10.01.00  2018-05-09  visstk     ESCAN00099077 Service 0x22: Unnecessary RAM usage
 *                        visstk     STORYC-2695   SafeBSW for DCM - Full UDS - DID Services (0x2A)
 *                        visygr     STORYC-5128   SafeBSW for DCM - Full (WWH-)OBD - Mode 0x03, 0x07, 0x0A
 *  10.02.00  2018-06-14  visstk     STORYC-2693   SafeBSW for DCM - Full UDS - DID Services (0x2C)
 *                        viswsi     STORYC-5287   Support activation of internal service processors
 *                        visvkr     STORYC-5423   SafeBSW for DCM - Full (WWH-)OBD - Mode 0x01, 0x09
 *                        visygr     STORYC-5424   SafeBSW for DCM - Full (WWH-)OBD - Mode 0x02, 0x08
 *  10.03.00  2018-07-09  visvkr     ESCAN00099310 Compiler error: Wrong pragma command in MemMap.h
 *                        visstk     ESCAN00100051 Service 0x2C: DDDID not cleared on session/security state change
 *                        visvkr     ESCAN00100098 Service 0x22: Overwritten RAM behind a DCM buffer
 *                        visaey     STORYC-464    SafeBSW for DCM - Full (WWH-)OBD - Calibration
 *                        visstk     STORYC-537    SafeBSW for DCM - Full UDS - DID Services (0x2F) - Basic
 *                        visahe     STORYC-5614   SafeBSW for DCM - Full (WWH-)OBD - Mode 0x06
 *  10.04.00  2018-08-06  visaey     ESCAN00087099 Service 0x86: Report DET error if NvM data is invalid.
 *                        viswsi     ESCAN00100072 Service 0x02: Overwritten RAM behind a DCM buffer
 *                        visvkr     ESCAN00100489 SafeBSW: Consistency of Dcm_CfgNetBufferInfo[] is not ensured
 *                        viswsi     ESCAN00100541 DTC settings and communication control states are not recovered on ECU Reset/Power On
 *                        visaey     STORYC-462    SafeBSW for DCM - Full UDS - Service 0x86 - Part 1
 *                        visahe     STORYC-465    SafeBSW for DCM - Full UDS - Vehicle System Groups
 *                        visstk     STORYC-5653   SafeBSW for DCM - Full UDS - DID Services (0x2F) - Extended
 *                        visaey     STORYC-5655   SafeBSW for DCM - Full UDS - Service 0x86 - Part 2
 *                        visstk     STORYC-5657   SafeBSW for DCM - Full UDS - Supported IDs
 *                        visygr     STORYC-5658   SafeBSW for DCM - Full UDS - RestoreStates
 *  11.00.00  2018-09-03  visstk     ESCAN00100161 Service 0x2C: Unnecessary RAM usage
 *                        visahe     ESCAN00101202 Service 0x2A: Overwritten RAM behind a DCM buffer when using UUDT delay timer
 *                        visahe     STORYC-2150   Service 0x27: Simplify sub-function handling
 *                        visahe     STORYC-4915   Support SecurityAttemptCounter decrement for SAIC
 *                        visaey     STORYC-5280   MISRA-C:2012 Compliance
 *                        visahe     STORYC-6369   SafeBSW- for DCM - Reduce complexity I
 *  11.01.00  2018-10-01  visstk     STORYC-6532   SafeBSW- for DCM - Remove deprecated interfaces
 *  11.02.00  2018-10-26                           No relevant changes
 *  11.03.00  2018-11-23  visygr     STORYC-4913   Support SpecificCauseCode
 *  11.04.00  2018-12-20  visaey     ESCAN00101688 Compiler warning: conversion from 'const Dcm_CfgDidMgrAbstractOpRefType' to 'Dcm_CfgDidMgrDynDidHandleOptType', possible loss of data
 *  11.05.00  2019-01-18  visahe     ESCAN00101968 Wrong value of define DIAG_ASR4DCM_VERSION
 *                        visahe     STORYC-5574   Parallel OBD/UDS protocol processing - Infrastructure
 *                        visahe     STORYC-5575   Parallel OBD/UDS protocol processing - Multi thread handling
 *  11.06.00  2019-02-18  visvkr     STORYC-5083   Parallel OBD/UDS protocol processing - Pseudo parallelization
 *                        visvkr     STORYC-5286   Parallel OBD/UDS protocol processing - Synchronization
 *  12.00.00  2019-03-15  visahe     ESCAN00100995 ECU returns NRC 0x7F instead of NRC 0x11 for services that are not configured in all connections
 *  12.01.00  2019-04-15  visvkr     ESCAN00091550 Service 0x27: Dcm allows seed/key attempt earlier than the configured security delay time
 *                        viswse     STORYC-7430   Make S3 timeout configurable for FBL Use-Case
 *  12.02.00  2019-05-13  visuvo     ESCAN00102499 ECU does not send response after jump from FBL
 *                        visaey     STORYC-7971   Generic Connections - Meta data handling
 *                        visaey     STORYC-7991   Generic Connections - Source address handling
 *  12.03.00  2019-06-11  viswse     ESCAN00100871 Compiler warning: Variable is possibly uninitialized
 *                        visygr     ESCAN00103172 Compiler error: 'Dcm_HandleServiceExtern': too few arguments for call
 *  12.04.00  2019-07-08                           No relevant changes
 *  12.05.00  2019-08-02  viswse     DCM-11        Adapt Supported ID feature
 *                        visahe     DCM-1719      Persist defined DDIDs in Dcm - NvM interaction - Part 1
 *                        visvkr     DCM-1770      Persist defined DDIDs in Dcm - NvM interaction - Part 2
 *                        visahe     DCM-26        Persist defined DDIDs in Dcm - RAM optimization
 *  13.00.00  2019-08-30  viswse     DCM-1729      RTE range checks for atomic S/R communication with DiagXF
 *                        visepp     DCM-1963      Simplify gap-signal handling
 *                        visahe     DCM-38        Support sub-function 0x55 of ReadDTCInformation
 *                        viswse     DCM-85        Support NvM job result NVM_REQ_RESTORED_FROM_ROM in DCM
 *                        visygr     ESCAN00103688 Service 0x2E: ECU returns NRC 0x10 "General Reject" for an atomic S/R DID
 *  13.01.00  2019-09-27  vsarcbada  CARC-1        Support service 0x29 sub-function 0x03 - Part II
 *                        vsarcbada  CARC-27       Validation of diagnostic service access rights
 *                        vsarcziad  DCM-15        Support service 0x29 sub-function 0x01 and 0x02
 *                        vsarcafro  DCM-59        Support service 0x29 sub-function 0x03
 *                        vsarcbada  DCM-67        Support service 0x29 sub-function 0x00 and 0x08
 *  13.02.00  2019-10-25  visahe     DCM-1984      Support 64 Bit for least platform types
 *                        visygr     DCM-2004      Raise quality of service 0x29 implementation to ASIL level
 *  13.03.00  2019-11-22  visepp     DCM-2158      Support OBD and UDS over a single protocol
 *                        visepp     ESCAN00104525 DcmSupportDTCSettingControlOptionRecord: Wrong dependency description
 *                        visepp     ESCAN00104840 Service 0x03 and 0x07: Dem_SetDTCFilter() API called with wrong DTCOrigin
 *  13.04.00  2020-01-16  visepp     DCM-1288      Security Access - Disable by Application
 *                        visepp     ESCAN00105133 Service 0x2F: Dcm_DidMgrIoControlCemrSessionMask in wrong memory section
 *                        visepp     ESCAN00105389 Compiler warning: unreferenced parameter 'CertId' in function Dcm_KeyMAsyncCertificateVerifyFinished
 *  13.05.00  2020-02-14  visvkr     DCM-2312      Support integer typed C/S interfaces for DIDs
 *                        visvkr     DCM-2314      Support floating-point typed C/S interfaces for DIDs
 *                        visepp     ESCAN00105623 Service 0x2A, 0x2C: Overwritten RAM
 *  14.00.00  2020-03-13  vsarcbada  CARC-46       Fallback to deauthenticated state
 *                        vsarcesta  CARC-69       Change of deauthenticated role via application
 *                        vsarcafro  CARC-71       Persistence of authentication states
 *                        viswse     ESCAN00097203 Compiler error: 'conversion of data types, possible loss of data' in case of large buffers
 *                        visepp     ESCAN00105879 Compiler warning: unreferenced parameter 'job' in function Dcm_CsmAsyncJobFinished
 *  14.01.00  2020-04-20  visvkr     ESCAN00105911 Service 0x29: Overwritten RAM during cancellation
 *                        visvkr     ESCAN00105918 Service 0x2E: Positive response instead of NRC 0x22
 *  14.02.00  2020-05-11  visepp     DCM-2502      Remove IDLE operation value of Dcm_IOOperationResponseType
 *                        visahe     DCM-2892      Service 0x28, 0x85: Enable DTCSetting on transition to non-default session
 *                        visepp     ESCAN00100457 Compiler error: Missing parameter in function Dem_DcmGetDTCOfOBDFreezeFrame
 *                        visepp     ESCAN00104291 Compiler warning: 'Dem_DcmReadDataOfOBDFreezeFrame': incompatible types - from 'unsigned char *' to 'unsigned short *'
 *  14.03.00  2020-06-05  visvkr     DCM-2185      Modifications for improved MISRA-C:2012 compliance
 *                        visepp     DCM-2564      DCM: Support user defined fault memories according to ISO 14229-1
 *                        viswse     DCM-2934      Improve usability of DCM without MICROSAR DEM
 *                        visuvo     ESCAN00105778 Service 0x04, 0x14, 0x19: ECU returns NRC 0x10 instead of NRC 0x22
 *  14.04.00  2020-06-26  visvkr     DCM-1283      Support service 0x19 sub-function 0x16
 *                        visepp     ESCAN00088854 Calibrate-able Parameters do not Change after Calibration
 *                        visepp     ESCAN00106610 Supported ID Calculation of DIDs/RIDs is not Post-Build variant aware
 *                        visepp     ESCAN00106743 Service 0x27: Invalid NRC for key request
 *  14.05.00  2020-08-03  visygr     ESCAN00106866 ECU does not send a response in case of parallel service 0x86 event processing
 *  15.00.00  2020-08-31  visvkr     DCM-1898      SafeBSW for DCM - Reduce diversity of variants
 *                        viswse     DCM-2578      Support service 0x34, 0x36 and 0x37
 *                        visuvo     DCM-3161      MISRA-C: Update to QAC 2019.2
 *                        viswse     DCM-3259      Support OBD on UDS Subfunction 0x56 of Service 0x19
 *                        visvkr     DCM-3382      SafeBSW for DCM - Reduce diversity of variants II
 *  15.01.00  2020-10-01  visepp     DCM-3388      SafeBSW for DCM - Update API pattern
 *                        visepp     DCM-3407      SafeBSW for DCM - Reduce diversity of variants III
 *  15.02.00  2020-10-23  visepp     DCM-3578      Support parameter records for service 0x37
 *  15.03.00  2020-10-29  visvkr     DCM-3403      Make DCM safety requirements more understandable based on input from customer
 *                        visepp     DCM-3555      SafeBSW for DCM - Update runtime checks
 *  15.04.00  2020-11-23                           No relevant changes
 *  15.05.00  2020-11-23  visahe     DCM-2594      Service 0x86: Support new start/stop behavior according to ISO14229-1:2020
 *                        viswse     DCM-2987      Input and output parameters for routine control shall not overwrite each other
 *                        visvkr     DCM-3391      SafeBSW for DCM - Unit Debug
 *                        visvkr     DCM-3638      Service 0x86: Support new start/stop behavior according to ISO14229-1:2020 - Part II
 *  15.06.00  2020-12-22                           No relevant changes
 *  15.07.00  2021-01-21  visvkr     DCM-3696      Service 0x86: Adapt sub-function 0x03 to ISO 14229-1 (2020)
 *                        visvkr     DCM-3698      Service 0x86: Adapt sub-function 0x01 to ISO 14229-1 (2020)
 *                        visygr     DCM-3722      SafeBSW for DCM - Unit Utility
 *                        visepp     ESCAN00108062 Service 0x2C: Positive response instead of NRC 0x31 for invalid memorySize
 *  15.08.00  2021-02-15  vsarcmiem  CARC-784      Support OBD on UDS Subfunction 0x1A of Service 0x19
 *                        viswse     DCM-3893      Update interfaces to MICROSAR DEM
 *                        visygr     DCM-4057      SafeBSW for DCM - Unit Resource Manager
 *                        viswse     ESCAN00108521 Service 0x3E 0x80 sent via functional addressing does not start Keep-Alive timer
 *                        viswse     ESCAN00108538 Service 0x2A: ECU accepts invalid transmissionMode
 *                        visvkr     ESCAN00108748 Service 0x09: ECU sends unexpected NRC 0x10 or endlessly RCR-RP (NRC 0x78)
 *  16.00.00  2021-03-15  visepp     DCM-4050      SafeBSW for DCM - Unit FBL Manager
 *                        visuvo     DCM-4057      SafeBSW for DCM - Unit FBL Services
 *  16.01.00  2021-04-12  visvkr     DCM-4096      SafeBSW for DCM - Unit Variant Manager
 *  16.02.00  2021-05-07  visvkr     DCM-4092      SafeBSW for DCM - Unit Repeater
 *                        visvkr     ESCAN00109214 DCM switches to default session for OBD services (SID [0x00-0x0F]) for non-OBD ECUs
 *                        visvkr     ESCAN00109320 Service 0x09: Calls to GetInfotypeValueData operation not serialized
 *  16.03.00  2021-06-02  viswse     DCM-4094      SafeBSW for DCM - Unit DEM Interface
 *                        visuvo     DCM-4100      SafeBSW for DCM - Unit Service 0x04
 *                        viswse     DCM-4102      SafeBSW for DCM - Unit Service 0x14
 *                        viswse     DCM-4104      SafeBSW for DCM - Unit OBD-ID Manager
 *                        visepp     DCM-4458      DCM: Improve Service 0x19 0x16 in order to support ERec 0x92
 *                        visepp     DCM-4474      SafeBSW for DCM - Rework unit test configurations
 *                        visvkr     DCM-4546      SafeBSW for DCM - Unit Configuration Wrapper
 *                        visygr     ESCAN00109556 Service 0x36: NRC 0x13 instead of NRC 0x24 or a positive response
 *  16.04.00  2021-07-05  visvkr     DCM-4106      SafeBSW for DCM - Unit Service 0x01
 *                        visygr     DCM-4110      SafeBSW for DCM - Unit Service 0x08
 *                        visygr     DCM-4112      SafeBSW for DCM - Unit Service 0x09
 *                        viswse     DCM-4114      SafeBSW for DCM - Unit Service 0x02
 *                        visahe     DCM-4328      Dcm - Report security events to IdsM
 *                        visvkr     DCM-4560      Service 0x86: Support STRT 0x19 for sub-function 0x01
 *  16.05.00  2021-08-02  viswse     DCM-4108      SafeBSW for DCM - Unit Service 0x06
 *                        viswse     DCM-4116      SafeBSW for DCM - Unit OBD-DTC Manager
 *                        visvkr     DCM-4119      SafeBSW for DCM - Unit OBD-DTC Services
 *                        visepp     DCM-4661      SafeBSW for DCM - Improve configuration abstraction
 *                        viswse     DCM-4675      Extend OBDOnUDS DEM APIs by Dem_DTCFormatType
 *                        visvkr     DCM-4754      SafeBSW for DCM - Increase runtime coverage on component level V
 *  17.00.00  2021-08-26  visepp     DCM-3067      Support positive response after ECU reset
 *                        visvkr     DCM-4098      SafeBSW for DCM - Unit Task Manager
 *                        visvkr     DCM-4138      SafeBSW for DCM - Unit Service 0x3E
 *                        viswse     DCM-4842      SafeBSW for DCM - Unit Service 0x19 Utility - Part I
 *                        visvkr     ESCAN00110058 Service 0x2C: Overwritten RAM or no response
 *                        visvkr     ESCAN00110123 Compiler error: 'Dcm_DebugDetectError' : too few arguments for call
 *                        visvkr     ESCAN00110124 Service 0x22: Overwritten RAM during cancellation (Paged buffer)
 *                        viswse     ESCAN00110261 Service 0x19 0x06/0x10/0x19: NRC 0x31 instead of a positive response if DTCExtDataRecordNumber is set to 0xFE
 *                        visvkr     ESCAN00110288 Service 0x27: Seed or key request accepted too early
 *                        visvkr     ESCAN00111862 ECU does not respond due to uninitialized variable
 *  17.01.00  2021-09-27  visahe     DCM-4124      SafeBSW for DCM - Unit Mode Manager
 *                        visepp     DCM-4758      Allow UDS Service 0x29 custom service processors
 *  17.02.00  2021-10-22  viswse     DCM-4126      SafeBSW for DCM - Unit Service 0x11
 *                        viswse     DCM-4140      SafeBSW for DCM - Unit Service 0x19
 *  17.03.00  2021-11-19  visepp     DCM-4128      SafeBSW for DCM - Unit RID Manager
 *                        visepp     DCM-4130      SafeBSW for DCM - Unit Service 0x31
 *                        visuvo     DCM-4132      SafeBSW for DCM - Unit Memory Manager
 *                        visepp     DCM-4134      SafeBSW for DCM - Unit Service 0x23
 *                        visepp     DCM-4136      SafeBSW for DCM - Unit Service 0x3D
 *                        visepp     DCM-4142      SafeBSW for DCM - Unit Service 0x85
 *                        viswse     DCM-4346      Parallel protocol processing in default session - Service 0x31
 *                        viswse     DCM-4360      Implement Provide received data in StartOfReception in Diag_Asr4Dcm
 *                        visepp     ESCAN00110659 Service 0x29: Unexpected response instead of NRC 0x10 when asynchronous signature verification job reports an error
 *  17.04.00  2021-12-20  viskod     DCM-3540      DCM: Support user defined fault memories for service 0x14
 *                        visahe     DCM-4154      SafeBSW for DCM - Unit Service 0x28
 *                        visepp     DCM-4804      Usage of <Ma>_MemMap.h in Dcm
 *                        hbk        DCM-5184      Support service 0x29 sub-function 0x04
 *                        viskod     ESCAN00110934 Service 0x27: Xxx_SetSecurityAttemptCounter() called for unchanged security attempt counter
 *  17.05.00  2022-02-16  visepp     DCM-3700      Service 0x86: Replace the Vector standard extension
 *                        visahe     DCM-5496      Service 0x14: Support authentication check for user defined fault memories
 *  18.00.00  2022-03-11  viswse     DCM-4156      SafeBSW for DCM - Unit Timer
 *                        visvkr     DCM-5593      Support AUTOSAR array datatypes for Dcm S/R interfaces
 *  18.01.00  2022-04-14  visahe     DCM-4152      SafeBSW for DCM - Unit Service 0x24
 *                        viswse     DCM-4350      Parallel protocol processing in default session - SID level
 *                        hbk        DCM-5189      Support Secure Variant Coding - Service 0x2E - Part 1
 *                        hbk        DCM-5390      Support Secure Variant Coding - Service 0x31
 *                        hbk        DCM-5410      Support Secure Variant Coding - Service 0x2E - Part 2
 *                        viswse     DCM-5680      SafeBSW for DCM - Unit DID Manager (Part IV)
 *  18.02.00  2022-05-09  visahe     DCM-4146      SafeBSW for DCM - Unit Service 0x22
 *                        viswse     DCM-4148      SafeBSW for DCM - Unit Service 0x2E
 *                        visahe     DCM-4150      SafeBSW for DCM - Unit Service 0x2F
 *                        visahe     DCM-4158      SafeBSW for DCM - Unit Service 0x27
 *                        viswse     DCM-4160      SafeBSW for DCM - Unit Authentication Manager
 *                        visuvo     DCM-6032      SafeBSW for DCM - Optimize include structure
 *  18.03.00  2022-06-03  visahe     DCM-3430      SafeBSW for DCM - Unit Service 0x2A
 *                        viswse     DCM-4162      SafeBSW for DCM - Unit Service 0x29
 *                        viswse     DCM-4164      SafeBSW for DCM - Unit State Manager
 *  18.04.00  2022-06-30  visvkr     DCM-4348      Parallel protocol processing in default session - Service 0x10
 *                        visvkr     DCM-5018      Parallel protocol processing in default session - Framework
 *                        visvkr     DCM-6097      Parallel protocol processing in default session - Finalization
 *  18.05.00  2022-07-29  visvkr     DCM-6340      Service 0x29: Support user specific authentication roles
 *                        visepp     ESCAN00111504 Service 0x19: DTCStatusMask not patched correctly
 *                        dng        ESCAN00112120 Service 0x3E: Remaining buffer size not returned in Dcm_CopyRxData() if SduLength is 0
 *                        dng        ESCAN00112371 Service 0x29: Security event DIAG_SEV_AUTHENTICATION_SUCCESSFUL reported upon successful de-authentication
 *                        visvkr     ESCAN00112548 Service 0x29 0x03: Freedom from interference not ensured due to unreliable counter measure
 *                        visvkr     ESCAN00112642 Overwritten RAM during request prioritization
 *  19.00.00  2022-08-26  visvkr     DCM-4166      SafeBSW for DCM - Unit Paged Buffer
 *  19.01.00  2022-09-22  visvkr     DCM-4168      SafeBSW for DCM - Unit Service 0x2C
 *  19.02.00  2022-10-21  dng        DCM-4172      SafeBSW for DCM - Unit Service 0x10
 *                        dng        DCM-6380      Support DCM ServiceRequestNotification APIs acc. to ASR 20-11
 *  19.03.00  2022-11-21  dng        DCM-3922      SafeBSW for DCM - Unit Diag
 *  19.04.00  2023-01-16  visepp     DCM-4170      SafeBSW for DCM - Unit Network
 *                        visvkr     ESCAN00113653 Compiler error: Unresolved external symbol _Dcm_Svc19UtiChainNext referenced in function _Dcm_Svc19UtiDemSelectDTC
 *                        visvkr     ESCAN00113746 Forced NRC 0x78 is not cancelled correctly and a DET error might be reported
 *  19.05.00  2023-02-09  dbusch     DCM-6141      Provide channel ready for transmission information for Dcm
 *                        visepp     DCM-6782      Upgrade to QAC 2022.2
 *                        visvkr     DCM-6910      Remove dead code in DidMgr
 *                        dng        ESCAN00113700 Service 0x22, 0x2A: Overwritten RAM or DET error during reading of DDDID in case DCM_READ_FORCE_RCRRP is used by Dcm_ReadMemory()
 *                        dng        ESCAN00114044 Service 0x36: Incorrect NRC when data block length is longer than expected
 *  20.00.00  2023-03-13  dbusch     DCM-5950      SafeBSW for DCM - Unit Service 0x2F (Unittest)
 *                        dbusch     DCM-6967      Support service 0x35 (RequestUpload) in Dcm
 *                        dng        ESCAN00114116 Compiler warning: 'Dcm_ModeMgrSingletonContext' defined but not used
 *  20.01.00  2023-04-11                           No relevant changes
 *  20.02.00  2023-05-08  dng        DCM-3428      SafeBSW for DCM - Unit Controller
 *                        dbusch     DCM-7082      Support asynchronous pre- and postprocessor callouts
 *  20.03.00  2023-06-05  mosman     DCM-5748      Support OBD DTC for OBDOnUDS Services
 *                        dng        DCM-7182      Fix review findings in version 20.01.00
 *                        dng        DCM-7249      Fix review findings in version 20.02.00
 *                        visvkr     ESCAN00114561 Compiler error: Syntax error: identifier 'Dcm_RequestDataArrayType'
 *                        dbusch     ESCAN00114574 Service 0x29: Authentication timer not reloaded
 *                        dbusch     ESCAN00114844 Incompatible extension of Dcm_ProgConditionsType
 *  20.04.00  2023-06-30  mosman     ESCAN00114785 S3 timer reloaded by any request from different tester in a different protocol
 *  20.05.00  2023-07-28  mosman     DCM-7320      Improve include structure
 *                        mosman     DCM-7338      Protocol specific restriction for DIDs and RIDs
 *  21.00.00  2023-08-28  mosman     DCM-7377      SafeBSW for DCM - Unit Svc2ASchd (Unittest)
 *                        mosman     DCM-7461      Refactor configuration tables terminated by NULLPTR
 *                        visepp     ESCAN00115538 Compiler warning: Variable "lSignalLength" is possibly uninitialized
 *                        mosman     ESCAN00115557 Return values of interfaces called with an OpStatus equal to DCM_CANCEL are not ignored
 *                        dng        ESCAN00115689 Service 0x29: Access granted to tester sporadically without required role in a valid certificate
 *  21.01.00  2023-09-25  dbusch     DCM-7677      Allow SWC to set authentication via API at Dcm
 *                        dng        ESCAN00115594 Compiler error: KeyM.h not found
 *                        mosman     ESCAN00115599 Service 0x19 0x04/0x06/0x1A: NRC 0x31 instead of a positive response in case of OBD UDS DTC separation
 *                        mosman     ESCAN00115747 Compiler warning: Variable "oldSession" was set but never used
 *  21.02.00  2023-10-30                           No relevant changes
 *  21.03.00  2023-11-18  mosman     DCM-7855      Support bypass of authentication
 *                        visepp     DCM-1287      Support variant-specific security key lengths
 *                        visepp     ESCAN00116038 Service 0x22: Deactivated WWH-OBD DIDs in range 0xF7xx still accessable after calibration process
 *  21.04.00  2024-01-03  mosman     DCM-7875      SafeBSW for DCM - Unit NetPbCfg
 *                        visvkr     DCM-8091      Symbols for memory ranges for DcmDspMemory
 *  21.05.00  2024-02-12  visepp     DCM-8190      Prevent security reset at transition between non-default sessions
 *                        dbusch     DCM-8074      Deauthentication via API
 *                        dbusch     DCM-8249      Allow application to pause sending RoE events
 *                        visepp     ESCAN00116543 Service 0x29: NRC 0x55 instead of positive response during whitelist processing
 *                        mosman     ESCAN00116522 Compiler error: Unresolved external symbol Dcm_NetGetConnHdlByConnectionId
 *                        mosman     ESCAN00116757 Service 0x27: Attempt counter erroneously incremented or reset during cancellation
 *                        dbusch     ESCAN00116855 Wrong SID parameter in Pre-/PostHandler callout or services are not processed in parallel but one after another
 *********************************************************************************************************************/
#if !defined(DCM_H)
# define DCM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_Cbk.h"
# include "Dcm_Lcfg.h"
# include "Dcm_PBcfg.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DIAG_ASR4DCM_VERSION                                        0x2105u
# define DIAG_ASR4DCM_RELEASE_VERSION                                0x00u
/*! vendor and module identification */
# define DCM_VENDOR_ID                                               30u
# define DCM_MODULE_ID                                               0x0035u
/*! Dcm software module version */
# define DCM_SW_MAJOR_VERSION                                        21u
# define DCM_SW_MINOR_VERSION                                        5u
# define DCM_SW_PATCH_VERSION                                        0u
/*! AUTOSAR Software Specification Version Information
Document Version   4.2.0
Part of Release    4.0
Revision           0003, dated 2011-12-01 */
# define DCM_AR_RELEASE_MAJOR_VERSION                                4u
# define DCM_AR_RELEASE_MINOR_VERSION                                0u
# define DCM_AR_RELEASE_REVISION_VERSION                             3u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Splittask related critical section management */
# if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
#  define Dcm_SplitTaskEnterCS()                                     Dcm_UtiEnterCriticalSection()                                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_SplitTaskLeaveCS()                                     Dcm_UtiLeaveCriticalSection()                                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define Dcm_SplitTaskEnterCS()                                     /* Not used */
#  define Dcm_SplitTaskLeaveCS()                                     /* Not used */
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_FBLMGR_SUPPORT_ENABLED == STD_ON)
#  if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ProcessRequestUpload()
 *********************************************************************************************************************/
/*! \brief          Starts a software upload sequence.
 *  \details        This call-out is used during software upload processing, if RequestUpload is configured.
 *  \param[in]      OpStatus                Operation status
 *  \param[in]      DataFormatIdentifier    Identifier of the Data Format
 *  \param[in]      MemoryIdentifier        Identifier of the Memory Block
 *  \param[in]      MemoryAddress           Starting address of server memory from which data is to be retrieved
 *  \param[in]      MemorySize              Number of bytes in the MemoryData
 *  \param[in,out]  BlockLength             Maximum number of bytes for one Dcm_ProcessTransferDataWrite
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Request was successful
 *  \return         DCM_E_NOT_OK            Request was not successful
 *  \return         DCM_E_PENDING           Request is not yet finished
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_FBLMGR_SUPPORT_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_ProcessRequestUpload(
  Dcm_OpStatusType OpStatus,
  uint8  DataFormatIdentifier,
  uint8  MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint32, AUTOMATIC, DCM_APPL_DATA) BlockLength,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif

#  if (DCM_SVC_34_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ProcessRequestDownload()
 *********************************************************************************************************************/
/*! \brief          Starts a software download sequence.
 *  \details        This call-out is used during software download processing, if RequestDownload is configured.
 *  \param[in]      OpStatus                Operation status
 *  \param[in]      DataFormatIdentifier    Identifier of the Data Format
 *  \param[in]      MemoryIdentifier        Identifier of the Memory Block
 *  \param[in]      MemoryAddress           Starting address of server memory from which data is to be retrieved
 *  \param[in]      MemorySize              Number of bytes in the MemoryData
 *  \param[in,out]  BlockLength             Maximum number of bytes for one Dcm_ProcessTransferDataWrite
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                Request was successful
 *  \return         DCM_E_NOT_OK            Request was not successful
 *  \return         DCM_E_PENDING           Request is not yet finished
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_FBLMGR_SUPPORT_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_ProcessRequestDownload(
  Dcm_OpStatusType OpStatus,
  uint8  DataFormatIdentifier,
  uint8  MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint32, AUTOMATIC, DCM_APPL_DATA) BlockLength,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif

#  if (DCM_SVC_35_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ProcessTransferDataRead()
 *********************************************************************************************************************/
/*! \brief          Transfers data during a upload sequence.
 *  \details        This call-out is used during software upload processing, if TransferData is configured.
 *  \param[in]      OpStatus                Operation status
 *  \param[in]      MemoryIdentifier        Identifier of the Memory Block
 *  \param[in]      MemoryAddress           Starting address of server memory from which data is to be retrieved
 *  \param[in]      MemorySize              Number of bytes in the MemoryData
 *  \param[in]      MemoryData              Read Data to (Points to the diagnostic buffer in DCM)
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_READ_OK             Read was successful
 *  \return         DCM_READ_FAILED         Read was not successful
 *  \return         DCM_READ_PENDING        Read is not yet finished
 *  \return         DCM_READ_FORCE_RCRRP    Enforce RCR-RP transmission (vendor extension)
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_FBLMGR_SUPPORT_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_ReturnWriteMemoryType, DCM_CALLOUT_CODE) Dcm_ProcessTransferDataRead(
  Dcm_OpStatusType OpStatus,
  uint8  MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2VAR(uint8, AUTOMATIC, DCM_APPL_DATA) MemoryData,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif

#  if (DCM_SVC_34_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ProcessTransferDataWrite()
 *********************************************************************************************************************/
/*! \brief          Transfers data during a download sequence.
 *  \details        This call-out is used during software download processing, if TransferData is configured.
 *  \param[in]      OpStatus                Operation status
 *  \param[in]      MemoryIdentifier        Identifier of the Memory Block
 *  \param[in]      MemoryAddress           Starting address of server memory from which data is to be retrieved
 *  \param[in]      MemorySize              Number of bytes in the MemoryData
 *  \param[in]      MemoryData              Data to write (Points to the diagnostic buffer in DCM)
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_WRITE_OK            Write was successful
 *  \return         DCM_WRITE_FAILED        Write was not successful
 *  \return         DCM_WRITE_PENDING       Write is not yet finished
 *  \return         DCM_WRITE_FORCE_RCRRP   Enforce RCR-RP transmission (vendor extension)
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_FBLMGR_SUPPORT_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_ReturnWriteMemoryType, DCM_CALLOUT_CODE) Dcm_ProcessTransferDataWrite(
  Dcm_OpStatusType OpStatus,
  uint8  MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  P2CONST(uint8, AUTOMATIC, DCM_APPL_DATA) MemoryData,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif

/**********************************************************************************************************************
 *  Dcm_ProcessRequestTransferExit()
 *********************************************************************************************************************/
/*! \brief          Finalizes a software download sequence.
 *  \details        This call-out is used during software download processing, if RequestTransferExit is configured.
 *  \param[in]      OpStatus                             Operation status
 *  \param[in]      transferRequestParameterRecord       Request data
 *  \param[in]      transferRequestParameterRecordSize   Number of request data bytes
 *  \param[out]     transferResponseParameterRecord      Response data from the application
 *  \param[in,out]  transferResponseParameterRecordSize  Available buffer size, Number of written response data bytes
 *  \param[out]     ErrorCode                            Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK                             Request was successful
 *  \return         DCM_E_NOT_OK                         Request was not successful
 *  \return         DCM_E_PENDING                        Request is not yet finished
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_FBLMGR_SUPPORT_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_ProcessRequestTransferExit(
  Dcm_OpStatusType OpStatus,
  P2CONST(uint8, AUTOMATIC, DCM_APPL_DATA) transferRequestParameterRecord,
  uint32 transferRequestParameterRecordSize,
  P2VAR(uint8, AUTOMATIC, DCM_APPL_DATA) transferResponseParameterRecord,
  P2VAR(uint32, AUTOMATIC, DCM_APPL_DATA) transferResponseParameterRecordSize,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif /* (DCM_FBLMGR_SUPPORT_ENABLED == STD_ON) */
# if (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON)
#  if (DCM_AUTHMGR_SET_DEAUTH_ROLE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetDeauthenticatedRole()
 *********************************************************************************************************************/
/*! \brief          Overwrites the configured deauthentication role of a connection of the given index.
 *  \details        The time between the function call and the update of the deauthenticated role depends on the
 *                  current DCM state. The minimum time to be expected is one DCM task cycle. If this service is called
 *                  while the DCM is processing a diagnostic request, the update will be postponed till the end of
 *                  this service processing, to avoid unpredictable behavior.
 *  \param[in]      connectionId           The connection identifier as configured
 *  \param[in]      deauthenticatedRole    New deauthenticated role
 *  \return         E_OK is always returned
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE for different connection IDs
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetDeauthenticatedRole(                                                                                           /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  uint16 connectionId,
  P2CONST(uint8, AUTOMATIC, RTE_DCM_APPL_DATA) deauthenticatedRole
  );
#  endif

#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DeauthenticateConnection()
 *********************************************************************************************************************/
/*! \brief          Deauthenticates the given connection.
 *  \details        Note: The time between the function call and the deauthentication depends on the current DCM state.
 *                        The minimum time to be expected is one DCM task cycle. If this service is called while the
 *                        DCM is processing a diagnostic request of the connection, the deauthentication will be
 *                        postponed till the end of this service processing, to avoid unpredictable behavior.
 *  \param[in]      connectionId       The connection identifier as configured
 *  \return         E_OK               Request accepted. Deauthentication will take place in next main cycle.
 *  \return         E_NOT_OK           Deauthentication was not successful. Possible reasons:
 *                                     - authentication manager not initialized
 *                                     - connectionID invalid
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE for different connection IDs
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_STATE_AUTHENTICATION_ENABLED = STD_ON
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_DeauthenticateConnection(
  uint16 connectionId
  );
#  endif

#  if (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthenticateConnection()
 *********************************************************************************************************************/
/*! \brief          Authenticates the given connection and sets the given roles and white lists.
 *  \details        The caller will be informed via the notification Dcm_ConnectionAuthenticated(), when connection is
 *                  authenticated. The data behind parameter authenticationInfo must be valid until this confirmation.
 *                  Note: The time between the function call and the authentication depends on the current DCM state.
 *                        The minimum time to be expected is one DCM task cycle. If this service is called while the
 *                        DCM is processing a diagnostic request of the connection, the authentication will be postponed
 *                        till the end of this service processing, to avoid unpredictable behavior.
 *  \param[in]      connectionId        The connection identifier as configured
 *  \param[in]      authenticationInfo Holds the role to set for given connection and if configured the white list
 *  \return         E_OK                Request accepted. 
 *  \return         E_NOT_OK            Authentication was not successful. Possible reasons:
 *                                      - authentication manager not initialized
 *                                      - connectionID invalid
 *                                      - invalid parameter, e.g. NULLPTR or size of context items exceeds limit
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE for different connection IDs
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_AUTHMGR_EXT_SETTER_ENABLED = STD_ON
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_AuthenticateConnection(
  uint16 connectionId,
  P2CONST(Dcm_AuthenticationInfoType, AUTOMATIC, DCM_APPL_DATA) authenticationInfo
  );
#  endif
# endif /* (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON) */
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetRoeActivationState()
 *********************************************************************************************************************/
/*! \brief          Sets the activation state for RoE according to parameter.
 *  \details        -
 *  \param[in]      enabled         Holds the information whether the state is enabled or disabled.
 *  \return         E_OK            Request accepted. 
 *  \return         E_NOT_OK        Request not accepted. Possible reasons:
 *                                  - Svc86 scheduler not initialized
 *                                  - invalid parameter (!= TRUE/FALSE)
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_SVC_86_SUPPORT_ENABLED = STD_ON
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetRoeActivationState(
  boolean enabled
  );
# endif /* # if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
# if (DCM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  Dcm_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief          Reports component's version information.
 *  \details        Returns the version information of the used DCM implementation.
 *  \param[out]     versioninfo    pointer to the application structure
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_VERSION_INFO_API = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DCM_APPL_DATA) versioninfo
  );
# endif
/**********************************************************************************************************************
 *  Dcm_GetActiveProtocol()
 *********************************************************************************************************************/
/*! \brief          Returns the currently active protocol.
 *  \details        This function returns the active protocol Id.
 *  \param[out]     ActiveProtocol    Will contain the currently active protocol ID
 *  \return         DCM_E_OK          This value is always returned (also in error case)
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetActiveProtocol(                                                                                                /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  P2VAR(Dcm_ProtocolType, AUTOMATIC, DCM_APPL_DATA) ActiveProtocol
  );

/**********************************************************************************************************************
 *  Dcm_GetTesterSourceAddress()
 *********************************************************************************************************************/
/*! \brief          Returns a DCM tester's SourceAddress.
 *  \details        Returns a DCM tester's SourceAddress for a given RxPduId.
 *  \param[in]      DcmRxPduId             The DCM RxPduId of the tester connection which SourceAddress will be read
 *  \param[out]     TesterSourceAddress    Will contain the corresponding tester's SourceAddress
 *  \return         DCM_E_OK               The TesterSourceAddress out parameter contains a valid value
 *  \return         DCM_E_NOT_OK           The TesterSourceAddress out parameter does not have a valid return value
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetTesterSourceAddress(
  PduIdType DcmRxPduId,
  P2VAR(uint16, AUTOMATIC, DCM_APPL_DATA) TesterSourceAddress
  );

# if (DCM_DIAG_VIRTUAL_REQUEST_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ProcessVirtualRequest()
 *********************************************************************************************************************/
/*! \brief          Provides a means for internal (ROE) requests triggered by the application.
 *  \details        -
 *  \param[in]      RxPduId            The RxPDUID of the request
 *  \param[in]      Data               The data of the request. In case of generic connections, tester source address
 *                                     stored in the last byte.
 *  \param[in]      Length             The length of the request
 *  \return         DCM_E_OK           Reception was successful
 *  \return         DCM_E_NOT_OK       Reception was not successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            DCM_DIAG_VIRTUAL_REQUEST_ENABLED == STD_ON
 *  \trace          CREQ-113929
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ProcessVirtualRequest(
  PduIdType RxPduId,
  Dcm_MsgType Data,
  PduLengthType Length
  );
# endif

/**********************************************************************************************************************
 *  Dcm_SetActiveDiagnostic()
 *********************************************************************************************************************/
/*! \brief          Notifies DCM to (not) keep ComM channels awake.
 *  \details        -
 *  \param[in]      active             The new DCM ComM interaction mode
 *  \return         DCM_E_OK           This value is always returned (also in error case)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetActiveDiagnostic(                                                                                              /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  boolean active
  );

/**********************************************************************************************************************
 *  Dcm_GetRequestKind()
 *********************************************************************************************************************/
/*! \brief          Returns the request kind of a DCM tester SourceAddress.
 *  \details        Returns the request kind for a given diagnostic client.
 *  \param[in]      TesterSourceAddress    Will contain the corresponding tester's SourceAddress
 *  \param[out]     RequestKind            The request kind ([DCM_REQ_KIND]_NONE, _EXTERNAL and _ROE)
 *  \return         DCM_E_OK               The RequestKind out parameter contains a valid value
 *  \return         DCM_E_NOT_OK           The RequestKind out parameter does not have a valid return value
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetRequestKind(                                                                                                   /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  uint16 TesterSourceAddress,
  P2VAR(Dcm_RequestKindType, AUTOMATIC, DCM_APPL_DATA) RequestKind
  );
/**********************************************************************************************************************
 *  Dcm_ResetToDefaultSession()
 *********************************************************************************************************************/
/*! \brief          Provides a means for enforced resetting to the default session.
 *  \details        The call to this function allows the application to reset the current session to Default session.
 *                  Example: Automatic termination of an extended diagnostic session upon exceeding of a speed limit.
 *                  Note: The time between the function call and the termination of the session depends on the current
 *                        DCM state. The minimum time to be expected is one DCM task cycle. If this service is called
 *                        while the DCM is processing a diagnostic request, the session termination will be postponed
 *                        till the end of this service processing, to avoid unpredictable behavior.
 *  \return         E_OK    This value is always returned (also in error case)
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ResetToDefaultSession(                                                                                            /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  void
  );

/**********************************************************************************************************************
 *  Dcm_GetSesCtrlType()
 *********************************************************************************************************************/
/*! \brief          Provides read access to the DCM's diagnostic session register.
 *  \details        This function provides the active session control type value.
 *  \param[out]     SesCtrlType    Pointer to a RAM address where DCM will store the current session
 *  \return         E_OK           This value is always returned (also in error case)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *  \trace          CREQ-113936
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSesCtrlType(                                                                                                   /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  P2VAR(Dcm_SesCtrlType, AUTOMATIC, DCM_APPL_DATA) SesCtrlType
  );

# if (DCM_STATE_SECURITY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_GetSecurityLevel()
 *********************************************************************************************************************/
/*! \brief          Provides read access to the DCM's security access register.
 *  \details        This function provides the active security level value.
 *  \param[out]     SecLevel    Pointer to a RAM address where DCM will store the current security level
 *  \return         E_OK        This value is always returned (also in error case)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_STATE_SECURITY_ENABLED = STD_ON.
 *  \pre            -
 *  \trace          CREQ-113940
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSecurityLevel(                                                                                                 /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  P2VAR(Dcm_SecLevelType, AUTOMATIC, DCM_APPL_DATA) SecLevel
  );
# endif

# if (DCM_STATE_SECURITY_EXT_SETTER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetSecurityLevel()
 *********************************************************************************************************************/
/*! \brief          Provides write access to the DCM's security access register.
 *  \details        This function performs security level transition.
 *  \param[in]      SecLevel    The new security level to switch into
 *  \return         E_OK        State change has been performed.
 *  \return         E_NOT_OK    State change failed. Possible reasons:
 *                              - wrong/invalid security level;
 *                              - called while DCM is busy with a diagnostic request;
 *                              - called from wrong task context (not from Dcm_MainFunctionWorker);
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_STATE_SECURITY_EXT_SETTER_ENABLED = STD_ON.
 *  \pre            -
 *  \trace          CREQ-129518
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetSecurityLevel(
  Dcm_SecLevelType SecLevel
  );
# endif

# if (DCM_STATE_SECURITY_FIXED_BYTES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_GetSecurityLevelFixedBytes()
 *********************************************************************************************************************/
/*! \brief          Provides fixed bytes set up for a specific security level.
 *  \details        This function provides the fixed bytes for the requested security level.
 *                  A security level without configured fixed bytes will return E_OK and no bytes (bufferSize = 0)
 *  \param[in]      SecLevel              The requested security level
 *  \param[out]     FixedBytes            Buffer to receive the fixed byte values
 *  \param[in,out]  BufferSize
 *                  IN                    The provided buffer size
 *                  OUT                   The number of fixed bytes for the requested level
 *  \return         E_OK                  The security bytes were copied to the provided buffer
 *  \return         DCM_E_NOT_OK          The security level is not configured
 *  \return         DCM_E_BUFFERTOOLOW    The buffer was too small, BufferSize will contain the number of bytes needed
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_STATE_SECURITY_FIXED_BYTES_ENABLED = STD_ON
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSecurityLevelFixedBytes(                                                                                       /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  Dcm_SecLevelType SecLevel,
  P2VAR(uint8, AUTOMATIC, DCM_APPL_DATA) FixedBytes,
  P2VAR(uint8, AUTOMATIC, DCM_APPL_DATA) BufferSize
  );
# endif

# if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetSecurityBypass()
 *********************************************************************************************************************/
/*! \brief          Enables a bypass to security states of Dcm.
 *  \details        The call to this function allows the application to bypass the internal DCM security states.
 *  \param[in]      SecBypass       The new bypass mode.
 *  \return         DCM_E_OK        Security bypass mode has been performed.
 *  \return         DCM_E_NOT_OK    Some error has occurred. Possible reasons:
 *                                  - DCM is not yet initialized;
 *                                  - Wrong/invalid SecBypass parameter;
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_STATE_SECURITY_BYPASS_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetSecurityBypass(                                                                                                /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  boolean SecBypass
  );
# endif

# if (DCM_STATE_AUTHENTICATION_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetAuthenticationBypass()
 *********************************************************************************************************************/
/*! \brief          Enables a bypass to the authentication pre-conditions check related to service 0x29 (Authentication).
 *  \details        The call to this function allows the application to bypass the internal DCM authentication 
 *                  pre-conditions check related to service 0x29 (authentication).
 *  \param[in]      AuthBypass      The new bypass mode.
 *  \return         DCM_E_OK        Security bypass mode has been performed.
 *  \return         DCM_E_NOT_OK    Some error has occurred. Possible reasons:
 *                                  - DCM is not yet initialized;
 *                                  - Wrong/invalid AuthBypass parameter;
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_STATE_AUTHENTICATION_BYPASS_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetAuthenticationBypass(                                                                                          /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  boolean AuthBypass
  );
# endif
# if (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON)
#  if (DCM_DIDMGR_EXTENDED_LOOKUP_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_FilterDidLookUpResult()
 *********************************************************************************************************************/
/*! \brief          Provides a means to get supported DID suppressed by the application.
 *  \details        -
 *  \param[in]      OpStatus                Current operation status.
 *  \param[in]      Did                     The requested DID the filtering shall be done.
 *  \param[in]      DidOperation            The requested DID operation the filtering shall be done.
 *  \return         DCM_E_OK                The DID is (still) active.
 *  \return         DCM_E_PENDING           The DID validation needs more time. Call me again.
 *  \return         DCM_E_NOT_OK            The DID is not active.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_DIDMGR_EXTENDED_LOOKUP_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_FilterDidLookUpResult(
  Dcm_OpStatusType OpStatus,
  uint16 Did,
  Dcm_DidOpType DidOperation
  );
#  endif
# endif /* (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON) */
# if (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON)
#  if (DCM_RIDMGR_EXTENDED_LOOKUP_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_FilterRidLookUpResult()
 *********************************************************************************************************************/
/*! \brief          Provides a means to get supported RID suppressed by the application.
 *  \details        -
 *  \param[in]      OpStatus                Current operation status.
 *  \param[in]      Rid                     The requested RID the filtering shall be done.
 *  \return         DCM_E_OK                The RID is (still) active.
 *  \return         DCM_E_PENDING           The RID validation needs more time. Call me again.
 *  \return         DCM_E_NOT_OK            The RID is not active.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_RIDMGR_EXTENDED_LOOKUP_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_FilterRidLookUpResult(
  Dcm_OpStatusType OpStatus,
  uint16 Rid
  );
#  endif

#  if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_CsmSecureCodingValidationFinished()
*********************************************************************************************************************/
/*! \brief          Called by Crypto Service Manager when finished with an asynchronous job.
 *  \details        -
 *  \param[in]      job       JobID of the operation that caused the callback
 *  \param[in]      result    Contains the result of the job.
 *  \context        ISR1|ISR2
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_RIDMGR_SECURE_CODING_ENABLED = STD_ON
 *  \pre            -
*********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_CsmSecureCodingValidationFinished(
  P2CONST(Crypto_JobType, AUTOMATIC, DCM_APPL_DATA) job,
  Crypto_ResultType result
  );
#  endif
# endif /* (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  Dcm_Init()
 *********************************************************************************************************************/
/*! \brief          Modules initialization.
 *  \details        Service for basic initialization of DCM module.
 *                  In all cases where this API does expect a non-null pointer argument, a validation of the passed
 *                  argument is performed.
 *  \param[in]      configPtr    Pointer to a concrete configuration root - not used for now
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *  \trace          CREQ-113863
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Init(
  Dcm_ConfigPtrType configPtr
  );

/**********************************************************************************************************************
 *  Dcm_InitMemory()
 *********************************************************************************************************************/
/*! \brief          Initialization function.
 *  \details        Module's memory initialization.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled. If used, then always prior calling Dcm_Init.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_InitMemory(
  void
  );

# if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_MainFunctionTimer()
 *********************************************************************************************************************/
/*! \brief          Modules main timing scheduling.
 *  \details        This service is used for time critical tasks (high priority task).
 *                  Function will be called at high level task only for better timing accuracy.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_SPLIT_TASKS_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_MainFunctionTimer(                                                                                                          /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  void
  );
# endif

# if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_MainFunctionWorker()
 *********************************************************************************************************************/
/*! \brief          Modules main processing task.
 *  \details        This service is used for diagnostic service processing (low priority task).
 *                  Function will be called at low level task only, to allow long service execution times.
 *                  Note: All application call outs the DCM executes are performed only from within this task.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_SPLIT_TASKS_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_MainFunctionWorker(                                                                                                         /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  void
  );
# endif

# if (DCM_SPLIT_TASKS_ENABLED == STD_OFF)
/**********************************************************************************************************************
 *  Dcm_MainFunction()
 *********************************************************************************************************************/
/*! \brief          Modules main run-time scheduling.
 *  \details        This service is used for processing the tasks of the main loop.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_SPLIT_TASKS_ENABLED = STD_OFF.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_MainFunction(                                                                                                               /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  void
  );
# endif

/**********************************************************************************************************************
 *  Dcm_GetThreadContext()
 *********************************************************************************************************************/
 /*! \brief          Returns a thread context element of the given index.
  *  \details        -
  *  \param[in]      threadId    The active thread identifier
  *  \return         Valid thread context.
  *  \context        TASK
  *  \reentrant      TRUE
  *  \synchronous    TRUE
  *  \pre            -
  *********************************************************************************************************************/
FUNC(Dcm_ContextPtrType, DCM_CODE) Dcm_GetThreadContext(
  Dcm_ThreadIdOptType threadId
  );
/* --- Diagnostic service processing management -------------------------------------------------------------------- */
/**********************************************************************************************************************
 *  Dcm_ExternalProcessingDone()
 *********************************************************************************************************************/
/*! \brief          Finalizes the service processing task.
 *  \details        -
 *  \param[in,out]  pMsgContext  Current request context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ExternalProcessingDone(
  Dcm_MsgContextPtrType pMsgContext
  );

/**********************************************************************************************************************
 *  Dcm_ExternalSetNegResponse()
 *********************************************************************************************************************/
/*! \brief          Registers a NRC to the request in progress.
 *  \details        Only the very first registered NRC will be stored. Any further call of this API within the same
 *                  diagnostic request will have no effect.
 *  \param[in,out]  pMsgContext  Current request context
 *  \param[in]      errorCode    The error code to be registered
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ExternalSetNegResponse(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodeType errorCode
  );

# if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ProvideRecoveryStates()
 *********************************************************************************************************************/
/*! \brief          Returns the internal Dcm state.
 *  \details        This function can be used to gather the internal DCM states to be able to recover them after reset.
 *                  It shall be called by DCM application just before performing the reset operation.
 *  \param[out]     RecoveryInfo    Parameter to be written by the DCM with all the information to be recovered.
 *  \return         DCM_E_OK        Result is valid and shall be processed
 *  \return         DCM_E_NOT_OK    Some error has occurred e.g. DCM is not yet initialized
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_DIAG_STATE_RECOVERY_ENABLED = STD_ON
 *  \pre            Once this API is called, the states may change due to external events (e.g. session timeout).
 *                  Therefore always perform this call right before executing the reset.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_ProvideRecoveryStates(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_APPL_DATA) RecoveryInfo
  );
# endif

# if (DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetSpecificCauseCode()
 *********************************************************************************************************************/
/*! \brief          Registers a specific cause code to the request in progress.
 *  \details        Only the very first registered specific cause code will be stored. Any further call of this API
 *                  within the same diagnostic request will have no effect.
 *  \param[in]      specificCauseCode    The specific cause code to be registered
 *  \return         DCM_E_OK             Operation succeded. In case of first call, specific cause code is registered.
 *  \return         DCM_E_NOT_OK         Some error has occurred e.g. DCM is not yet initialized
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_DIAG_SPECIFIC_CAUSE_CODE_ENABLED = STD_ON
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetSpecificCauseCode(                                                                                             /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  Dcm_SpecificCauseCodeType specificCauseCode
  );
# endif
# if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgSetSingle()
 *********************************************************************************************************************/
/*! \brief          Sets the status of a single VSG.
 *  \details        -
 *  \param[in]      VsgId         Unique handle of the VSG.
 *  \param[in]      State         DCM_VSG_ENABLED:  Set VSG to active
 *                                DCM_VSG_DISABLED: Set VSG to inactive
 *  \return         DCM_E_OK      New status is set succesfully
 *  \return         DCM_E_NOT_OK  Operation failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED = STD_ON.
*********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgSetSingle(                                                                                                     /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  Dcm_VsgIdentifierType VsgId,
  Dcm_VsgStateType State
  );
# endif

# if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgIsActive()
 *********************************************************************************************************************/
/*! \brief          Returns status of a single VSG.
 *  \details        -
 *  \param[in]      VsgId         Unique handle of the VSG.
 *  \param[out]     State         DCM_VSG_ENABLED:  VSG is active
 *                                DCM_VSG_DISABLED: VSG is inactive
 *  \return         DCM_E_OK      Operation succeeded, parameter isActive contains status of VSG
 *  \return         DCM_E_NOT_OK  Operation failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED = STD_ON.
*********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgIsActive(                                                                                                      /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  Dcm_VsgIdentifierType VsgId,
  P2VAR(Dcm_VsgStateType, AUTOMATIC, DCM_APPL_DATA) State
  );
# endif

# if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgSetMultiple()
 *********************************************************************************************************************/
/*! \brief          Sets the status of a set of VSGs.
 *  \details        -
 *  \param[in]      VsgIdList     List of VsgIds to be set
 *  \param[in]      VsgListSize   Number of VSGs
 *  \param[in]      State         DCM_VSG_ENABLED:  Set VSGs to active
 *                                DCM_VSG_DISABLED: Set VSGs to inactive
 *  \return         DCM_E_OK      New status is set succesfully
 *  \return         DCM_E_NOT_OK  Operation failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED = STD_ON.
*********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgSetMultiple(                                                                                                   /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  P2CONST(Dcm_VsgIdentifierType, AUTOMATIC, DCM_APPL_DATA) VsgIdList,
  uint16 VsgListSize,
  Dcm_VsgStateType State
  );
# endif

# if (DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_VsgIsActiveAnyOf()
 *********************************************************************************************************************/
/*! \brief          Checks if at least one of the passed VSGs is active.
 *  \details        -
 *  \param[in]      VsgIdList     List of VsgIds to be set
 *  \param[in]      VsgListSize   Number of VSGs
 *  \param[out]     State         DCM_VSG_ENABLED:  At least one VSG is active
 *                                DCM_VSG_DISABLED: All VSGs are inactive
 *  \return         DCM_E_OK      Operation is executed successfully
 *  \return         DCM_E_NOT_OK  Operation failed
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_VARMGR_MULTI_SVC_INCLUSIVE_ENABLED = STD_ON.
*********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_VsgIsActiveAnyOf(                                                                                                 /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  P2CONST(Dcm_VsgIdentifierType, AUTOMATIC, DCM_APPL_DATA) VsgIdList,
  uint16 VsgListSize,
  P2VAR(Dcm_VsgStateType, AUTOMATIC, DCM_APPL_DATA) State
  );
# endif
/**********************************************************************************************************************
 *  Dcm_TriggerOnEvent()
 *********************************************************************************************************************/
/*! \brief          Triggers service to respond to.
 *  \details        Triggers service to respond to which was previously defined via service 0x86 request.
 *  \param[in]      RoeEventId      The RxPDUID of the request
 *  \return         DCM_E_OK        RoeEventId value is valid
 *  \return         DCM_E_NOT_OK    Service to respond to is not triggered.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_TriggerOnEvent(                                                                                                   /* PRQA S 3449, 3451 */ /* MD_Dcm_8.5_Rte */
  uint8 RoeEventId
  );

# if ((DCM_SVC_86_EVTYPE_ONCHGDTC_ENABLED == STD_ON) \
     || (DCM_SVC_86_01_SUPPORT_ENABLED == STD_ON) \
     || (DCM_SVC_86_09_SUPPORT_ENABLED == STD_ON))
/**********************************************************************************************************************
 *  Dcm_DemTriggerOnDTCStatus()
 *********************************************************************************************************************/
/*! \brief          Notifies DCM RoE handler for a DTC status change.
 *  \details        Triggers a ROE event for sub-service OnDTCStatusChanged if the requested DTCStatusMask is matched.
 *  \param[in]      DTC               This is the DTC the change is assigned to
 *  \param[in]      DTCStatusOld      DTC status before change
 *  \param[in]      DTCStatusNew      DTC status after change
 *  \return         DCM_E_OK          This value is always returned.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_DemTriggerOnDTCStatus(
  uint32 DTC,
  uint8 DTCStatusOld,
  uint8 DTCStatusNew
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_MEMMGR_SUPPORT_ENABLED == STD_ON)
#  if (DCM_MEMMGR_MEMOP_WRITE_ENABLED == STD_ON)
#   if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
/**********************************************************************************************************************
  Dcm_WriteMemory()
 *********************************************************************************************************************/
/*! \brief          Call-out to write to memory.
 *  \details        The Dcm_WriteMemory call-out is used to write memory data identified by the parameter memoryAddress
 *                  and memorySize.
 *                  This service is needed for the implementation of UDS services:
 *                      - WriteMemoryByAdress
 *                      - RequestDownload
 *  \param[in]      OpStatus                Operation status
 *  \param[in]      MemoryIdentifier        Identifier of the Memory Block
 *  \param[in]      MemoryAddress           Starting address of server memory from which data is to be written.
 *  \param[in]      MemorySize              Number of bytes in the MemoryData
 *  \param[in]      MemoryData              Data to be written (points to the diagnostic buffer in DCM).
 *  \param[out]     ErrorCode               If the operation Dcm_WriteMemory returns value
 *                                          DCM_WRITE_FAILED, the Dcm module shall
 *                                          send a negative response with NRC code equal to
 *                                          the parameter ErrorCode parameter value
 *  \return         DCM_WRITE_OK            Write was successful
 *  \return         DCM_WRITE_FAILED        Write was not successful
 *  \return         DCM_WRITE_PENDING       Write is not yet finished
 *  \return         DCM_WRITE_FORCE_RCRRP   Enforce RCR-RP transmission (vendor extension)
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_MEMMGR_MEMOP_WRITE_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_ReturnWriteMemoryType, DCM_CALLOUT_CODE) Dcm_WriteMemory(
  Dcm_OpStatusType OpStatus,
  uint8 MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  Dcm_MsgType MemoryData,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#   else
/**********************************************************************************************************************
  Dcm_WriteMemory()
 *********************************************************************************************************************/
/*! \brief          Call-out to write to memory.
 *  \details        The Dcm_WriteMemory call-out is used to write memory data identified by the parameter memoryAddress
 *                  and memorySize.
 *                  This service is needed for the implementation of UDS services:
 *                      - WriteMemoryByAdress
 *                      - RequestDownload
 *  \param[in]      OpStatus                Operation status
 *  \param[in]      MemoryIdentifier        Identifier of the Memory Block
 *  \param[in]      MemoryAddress           Starting address of server memory from which data is to be written.
 *  \param[in]      MemorySize              Number of bytes in the MemoryData
 *  \param[in]      MemoryData              Data to be written (points to the diagnostic buffer in DCM).
 *  \return         DCM_WRITE_OK            Write was successful
 *  \return         DCM_WRITE_FAILED        Write was not successful
 *  \return         DCM_WRITE_PENDING       Write is not yet finished
 *  \return         DCM_WRITE_FORCE_RCRRP   Enforce RCR-RP transmission (vendor extension)
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_MEMMGR_MEMOP_WRITE_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_ReturnWriteMemoryType, DCM_CALLOUT_CODE) Dcm_WriteMemory(
  Dcm_OpStatusType OpStatus,
  uint8 MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  Dcm_MsgType MemoryData
  );
#   endif
#  endif

#  if (DCM_MEMMGR_MEMOP_READ_ENABLED == STD_ON)
#   if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
/**********************************************************************************************************************
 *  Dcm_ReadMemory()
 *********************************************************************************************************************/
/*! \brief          Call-out to read from memory.
 *  \details        The Dcm_ReadMemory call-out is used to request memory data identified by the parameter
 *                  memoryAddress and memorySize from the UDS request message. This service is needed for the
 *                  implementation of UDS services:
 *                      - ReadMemoryByAdress
 *                      - RequestUpload
 *                      - ReadDataByIdentifier (in case of Dynamical DID defined by memory address)
 *  \param[in]      OpStatus                Operation status
 *  \param[in]      MemoryIdentifier        Identifier of the Memory Block
 *  \param[in]      MemoryAddress           Starting address of server memory from which data is to be retrieved
 *  \param[in]      MemorySize              Number of bytes in the MemoryData
 *  \param[out]     MemoryData              Data read (points to the diagnostic buffer in DCM)
 *  \param[out]     ErrorCode               If the operation Dcm_ReadMemory returns value
 *                                          DCM_READ_FAILED, the Dcm module shall send
 *                                          a negative response with NRC code equal to the
 *                                          parameter ErrorCode parameter value
 *  \return         DCM_READ_OK             Read was successful
 *  \return         DCM_READ_FAILED         Read was not successful
 *  \return         DCM_READ_PENDING        Read is not yet finished
 *  \return         DCM_READ_FORCE_RCRRP    Enforce RCR-RP transmission (vendor extension)
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_MEMMGR_MEMOP_READ_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_ReturnReadMemoryType, DCM_CALLOUT_CODE) Dcm_ReadMemory(
  Dcm_OpStatusType OpStatus,
  uint8 MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  Dcm_MsgType MemoryData,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#   else
/**********************************************************************************************************************
 *  Dcm_ReadMemory()
 *********************************************************************************************************************/
/*! \brief          Call-out to read from memory.
 *  \details        The Dcm_ReadMemory call-out is used to request memory data identified by the parameter
 *                  memoryAddress and memorySize from the UDS request message. This service is needed for the
 *                  implementation of UDS services:
 *                      - ReadMemoryByAdress
 *                      - RequestUpload
 *                      - ReadDataByIdentifier (in case of Dynamical DID defined by memory address)
 *  \param[in]      OpStatus                Operation status
 *  \param[in]      MemoryIdentifier        Identifier of the Memory Block
 *  \param[in]      MemoryAddress           Starting address of server memory from which data is to be retrieved
 *  \param[in]      MemorySize              Number of bytes in the MemoryData
 *  \param[out]     MemoryData              Data read (points to the diagnostic buffer in DCM)
 *  \return         DCM_READ_OK             Read was successful
 *  \return         DCM_READ_FAILED         Read was not successful
 *  \return         DCM_READ_PENDING        Read is not yet finished
 *  \return         DCM_READ_FORCE_RCRRP    Enforce RCR-RP transmission (vendor extension)
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_MEMMGR_MEMOP_READ_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_ReturnReadMemoryType, DCM_CALLOUT_CODE) Dcm_ReadMemory(
  Dcm_OpStatusType OpStatus,
  uint8 MemoryIdentifier,
  uint32 MemoryAddress,
  uint32 MemorySize,
  Dcm_MsgType MemoryData
  );
#   endif
#  endif
# endif /* (DCM_MEMMGR_SUPPORT_ENABLED == STD_ON) */
# if (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON)
#  if (DCM_AUTHMGR_USER_SPECIFIC_ROLES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_GetAuthenticationRoles()
 *********************************************************************************************************************/
/*! \brief          Reads the authentication roles from the application.
 *  \details        This function will be called during processing of service 0x29 0x03 and replaces the call of
 *                  KeyM_CertElementGet() which would normally be called by DCM to read the role information directly
 *                  from the certificate.
 *                  The intention is, that the application reads the role information from the certificate by itself
 *                  using the KeyM parameters certId and certElementId provided by this function. The role information
 *                  shall then be transformed to the AUTOSAR format and passed back to DCM.
 *  \param[in]      certId           Holds the identifier of the certificate
 *  \param[in]      certElementId    Specifies where the role information is located within the certificate
 *  \param[out]     role             The role to be read
 *  \param[in,out]  roleLength 
 *                  IN               Contains the maximum data length of the buffer for the role information
 *                  OUT              The actual length of the role
 *  \return         E_OK             Reading was successful
 *  \return         E_NOT_OK         Reading was not successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_AUTHMGR_USER_SPECIFIC_ROLES_ENABLED = STD_ON
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_GetAuthenticationRoles(
  KeyM_CertificateIdType certId,
  KeyM_CertElementIdType certElementId,
  P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) role,
  P2VAR(uint32, AUTOMATIC, DCM_VAR_NOINIT) roleLength
  );
#  endif

#  if (DCM_AUTHMGR_EXT_SETTER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ConnectionAuthenticated()
 *********************************************************************************************************************/
/*! \brief          Informs the application that the DCM authenticated the given connection and set the white lists.
 *  \details        -
 *  \param[in]      connectionId       The connection identifier as configured
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_AUTHMGR_EXT_SETTER_ENABLED = STD_ON
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CALLOUT_CODE) Dcm_ConnectionAuthenticated(
  uint16 connectionId
  );
#  endif
# endif /* (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  Dcm_Confirmation()
 *********************************************************************************************************************/
/*! \brief          Confirms the diagnostic job finalization.
 *  \details        This function confirms the successful transmission or a transmission error of a diagnostic
 *                  service. The idContext and the dcmRxPduId are required to identify the message which was processed.
 *                  If there was no response for this request, this call out is invoked at service processing finish.
 *                  Note: This call out is invoked only then when a DCM internal or external
 *                        <Module>_<DiagnosticService> service handler has been executed.
 *  \param[in]      idContext     Current context identifier which can be used to retrieve the relation between
 *                                request and confirmation. Within the confirmation, the Dcm_MsgContext is no more
 *                                available, so the idContext can be used to represent this relation.
 *                                The idContext is also part of the Dcm_MsgContext
 *  \param[in]      dcmRxPduId    DcmRxPduId on which the request was received. The source of the request
 *                                can have consequences for message processing.
 *  \param[in]      status        Status indication about confirmation
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CALLOUT_CODE) Dcm_Confirmation(
  Dcm_IdContextType idContext,
  PduIdType dcmRxPduId,
  Dcm_ConfirmationStatusType status
  );

# if (DCM_DIAG_JUMPTOFBL_ENABLED   == STD_ON) || \
     (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_SetProgConditions()
 *********************************************************************************************************************/
/*! \brief          Sets the programming conditions.
 *  \details        The Dcm_SetProgConditions call-out allows the integrator to store relevant information prior
 *                  jumping to boot loader. The context parameters are defined in Dcm_ProgConditionsType.
 *  \param[in]      OpStatus          Operation status. Available only when AUTOSAR version >= R19-11
 *  \param[in]      progConditions    Conditions on which the jump to boot loader has been requested.
 *  \return         E_OK              Conditions have correctly been set
 *  \return         E_NOT_OK          Conditions cannot be set
 *  \return         DCM_E_PENDING     Conditions set is in progress, a further call to this API is needed to end the
                                      setting
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if (DCM_DIAG_JUMPTOFBL_ENABLED = STD_ON) ||
 *                  (DCM_DIAG_JUMPFROMFBL_ENABLED = STD_ON).
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_SetProgConditions(
#  if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_450)
  Dcm_OpStatusType OpStatus,
#  endif
  Dcm_ProgConditionsPtrType progConditions
  );
# endif

# if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_GetProgConditions()
 *********************************************************************************************************************/
/*! \brief          Returns the programming conditions.
 *  \details        The Dcm_GetProgConditions call-out is called upon DCM initialization and allows determining if a
 *                  response (0x50 or 0x51) has to be sent depending on request within the boot-loader.
 *                  The context parameters are defined in Dcm_ProgConditionsType.
 *  \param[in]      progConditions    Conditions on which the jump from the boot-loader has been requested.
 *  \return         DCM_COLD_START    The ECU starts normally
 *  \return         DCM_WARM_START    The ECU starts from a boot-loader jump
 *                                    The function parameter values will be evaluated for further processing.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_DIAG_JUMPFROMFBL_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_EcuStartModeType, DCM_CALLOUT_CODE) Dcm_GetProgConditions(
  Dcm_ProgConditionsPtrType progConditions
  );
# endif

# if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_GetRecoveryStates()
 *********************************************************************************************************************/
/*! \brief          Collects the external stored internal Dcm state.
 *  \details        The Dcm_GetRecoveryStates() call-out is called upon DCM initialization and allows to restore the
 *                  previously via Dcm_ProvideRecoveryStates() stored internal Dcm state.
 *  \param[out]     RecoveryInfo     Parameter to be written by the DCM with all the information to be recovered.
 *  \return         DCM_E_OK         Result is valid and shall be processed
 *  \return         DCM_E_PENDING    Result is pending (e.g. NvM not yet ready)
 *  \return         DCM_E_NOT_OK     No information to be recovered or result reading failed
 *                                   DCM shall continue with the default initialized states.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \config         This function is only available if DCM_DIAG_STATE_RECOVERY_ENABLED = STD_ON
 *  \pre            Once this API is called, DCM will lock any external connection until the result is processed.
 *                  This is required in order to be able to switch into a consistent state without any influence from
 *                  outside.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_GetRecoveryStates(
  P2VAR(Dcm_RecoveryInfoType, AUTOMATIC, DCM_VAR_NOINIT) RecoveryInfo
  );
# endif

# if (DCM_DIAG_SERVICE_DISPATCHER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_HandleServiceExtern()
 *********************************************************************************************************************/
/*! \brief         Returns the service implementation type for dispatching.
 *  \details        This call-out is used during service request processing, if the service is configured to be
 *                  dispatchable.
 *                  Note: This call-out is only invoked, if a DCM service handler has been configured accordingly.
 *  \param[in]      SID                   The service identifier of the request
 *  \param[in]      RequestData           The request data (behind SID byte)
 *  \param[in]      DataSize              The request data length
 *  \param[in]      ReqType               The request type (0 - physical, 1- functional)
 *  \param[in]      SourceAddress         The tester source address
 *  \return         TRUE              The service processing is done by the application
 *  \return         FALSE             The service processing is done by DCM
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \config         This function is only available if DCM_DIAG_SERVICE_DISPATCHER_ENABLED = STD_ON.
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CALLOUT_CODE) Dcm_HandleServiceExtern(
  uint8 SID,
  Dcm_ReadOnlyMsgType RequestData,
  uint16 DataSize,
  uint8 ReqType,
  uint16 SourceAddress
  );
# endif
# define DCM_STOP_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON)
#  if (DCM_STATE_AUTHENTICATION_ENABLED == STD_ON)
/*! Authentication manager data that can be read from and written to NvM (if NvM is configured) */
extern VAR(Dcm_AuthMgrNvMDataType, DCM_VAR_NOINIT) Dcm_AuthMgrNvMData[DCM_AUTHMGR_NUM_AUTH_CONNECTIONS];
#  endif
# endif /* (DCM_AUTHMGR_SUPPORT_ENABLED == STD_ON) */
# if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/*! Service 0x2C Data that can be read from and written to NvM (if NvM is configured) */
extern VAR(Dcm_Svc2CDynDidNvMDataType, DCM_VAR_NOINIT) Dcm_Svc2CNvMData;
# endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 *********************************************************************************************************************/
/*!
 * \exclusivearea DCM_EXCLUSIVE_AREA_0
 *                Ensures consistency of the Dcm global state variables.
 *
 * \protects      Dcm_PbRamNetConnHdl2TObjMap,
 *                Dcm_PbRamNetTransportObject,
 *                Dcm_PbRamNetBufferContext,
 *                Dcm_PbRamNetComMContext,
 *                Dcm_PbRamNetPeriodicTxObject[].Timer,
 *                Dcm_PbRamNetPeriodicTxObject[].State,
 *                Dcm_ThreadContext[].Diag.QueuedSet,
 *                Dcm_ThreadContext[].PagedBuffer.ReadIndex,
 *                Dcm_ThreadContext[].PagedBuffer.WriteIndex,
 *                Dcm_ThreadContext[].PagedBuffer.RemainingLen,
 *                Dcm_ThreadContext[].Diag.TObjHdl,
 *                Dcm_ThreadContext[].Diag.MsgContext.msgAddInfo.suppressPosResponse,
 *                Dcm_SingletonContext.Network.NumActiveConnections,
 *                Dcm_TskMgrSingletonContext.TaskContext,
 *                Dcm_TskMgrSingletonContext.TaskScheduler[].SchdIsActive,
 *                Dcm_TmrSingletonContext.Timer,
 *                Dcm_SingletonContext.Network.ActiveDiagnostic,
 *                Dcm_StateSingletonContext.Preconditions.Session,
 *                Dcm_Svc27SingletonContext.DelayTime[],
 *                Dcm_Svc27SingletonContext.SetAttCntrEventMask,
 *                Dcm_Svc2ASingletonContext.Scheduler.Table[].Rate,
 *                Dcm_Svc2ASingletonContext.Scheduler.Table[].Timer,
 *                Dcm_Svc2ASingletonContext.Scheduler.NumActiveEntries
 *                Dcm_Svc29SingletonContext.CbkProcessingState
 *                Dcm_AuthMgrSingletonContext.PendingData
 *                Dcm_AuthMgrSingletonContext.DelayTime[]
 *                Dcm_AuthMgrSingletonContext.TimerExpiredMask
 *                Dcm_RidMgrSingletonContext.SecureCoding
 *                Dcm_Svc86CurrentSession.EventRecord.OnDtcChange.DtcRecord
 *                Dcm_Svc86Schd_09DtcRingBuffer.NumElements
 *
 * \usedin        Dcm_ComM_FullComModeEntered,
 *                Dcm_ComM_NoComModeEntered,
 *                Dcm_ComM_SilentComModeEntered,
 *                Dcm_DiagFblResDoProcessTxConfirmation,
 *                Dcm_DiagHandleSuppressBit,
 *                Dcm_DiagRxIndication,
 *                Dcm_DiagSetP2Timings,
 *                Dcm_DiagSwitchProcessingContext,
 *                Dcm_DiagTaskGarbageCollector,
 *                Dcm_DiagTxFinalization,
 *                Dcm_DiagWorkerDoCancelProcessing_CancelOpen,
 *                Dcm_DiagWorkerDoCancelProcessing_ReviveKilledTasks,
 *                Dcm_DiagWorkerDoRepeat,
 *                Dcm_GetRequestKind,
 *                Dcm_NetLockConnection,
 *                Dcm_NetOnRequestDetection,
 *                Dcm_NetProvideRxBuffer,
 *                Dcm_NetRegisterActiveConnection,
 *                Dcm_NetRegisterComMActivity,
 *                Dcm_NetRxIndInternal,
 *                Dcm_NetSetComModeEntered,
 *                Dcm_NetStartOfReception,
 *                Dcm_NetTaskTaMonitor,
 *                Dcm_NetUnRegisterActiveConnection,
 *                Dcm_NetUnRegisterAllComMActivity,
 *                Dcm_NetUnRegisterComMActivity,
 *                Dcm_OnTimeoutPeriodicTx,
 *                Dcm_PagedBufferDefragmentPage,
 *                Dcm_PagedBufferTask,
 *                Dcm_PagedBufferUpdatePage,
 *                Dcm_Service10PostProcessor,
 *                Dcm_SetActiveDiagnostic,
 *                Dcm_SetSecurityLevel,
 *                Dcm_StateSetSession,
 *                Dcm_Svc10_WaitForResetAck,
 *                Dcm_Svc27TimerSet,
 *                Dcm_Svc27UtiSetAttemptCntr,
 *                Dcm_Svc2ASchdCommit,
 *                Dcm_TmrSetTimerByThread,
 *                Dcm_TmrStartTimerByThread,
 *                Dcm_TmrStopTimerByThread,
 *                Dcm_TmrIsTimerRunningByThread,
 *                Dcm_TmrResumeTimerByThread,
 *                Dcm_TmrTaskTimer,
 *                Dcm_TskSetEventByThread
 *                Dcm_TskClrEventByThread
 *                Dcm_TskKillAllTasks,
 *                Dcm_NetPTxObjTimerSet,
 *                Dcm_Svc10CheckAccess,
 *                Dcm_KeyMAsyncCertificateVerifyFinished,
 *                Dcm_CsmAsyncJobFinished,
 *                Dcm_CsmSecureCodingValidationFinished,
 *                Dcm_Service29Cancel,
 *                Dcm_SetDeauthenticatedRole,
 *                Dcm_AuthenticateConnection,
 *                Dcm_DeauthenticateConnection,
 *                Dcm_AuthMgrDetectIdleConnection,
 *                Dcm_AuthMgrSetExtAuthentication,
 *                Dcm_OnTimeoutAuthentication,
 *                Dcm_AuthMgrDeauthenticateIdleConn,
 *                Dcm_DemTriggerOnDTCStatus,
 *                Dcm_MainFunctionWorker,
 *                Dcm_RidMgrSecureCodingVerify
 *                Dcm_OnTimeoutTxObjectHandler
 *                Dcm_NetPeriodicMsgStopDid
 *                Dcm_DiagWorkerDoCancelProcCancelOpenJobs
 *                Dcm_DiagWorkerDoCancelProcReviveKilledTasks
 *                Dcm_DiagCancelProcessing
 *                Dcm_Service86_01Processor
 *                Dcm_Service86_09Processor
 *                Dcm_Svc86Schd_09RingBufferRemoveElement
 *                Dcm_Svc86Schd_09RingBufferAddElement
 *                Dcm_Svc86Schd_09TriggerOnDTCStatus
 *                Dcm_Svc86Schd_01TriggerOnDTCStatus
 *
 * \exclude       Dcm_NetStartOfReception, Dcm_MainFunction, Dcm_MainFunctionTimer, Dcm_MainFunctionWorker, Dcm_DemTriggerOnDTCStatus
 * \length        MEDIUM Typically the exclusive area is short, but there are cases, where function calls are performed.
 * \endexclusivearea
 */
#endif /* !defined(DCM_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm.h
 *********************************************************************************************************************/
