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
/**        \file  CanIf.c
 *        \brief  CAN Interface source file.
 *
 *      \details  This file contains the implementation of the AUTOSAR CAN Interface module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define CANIF_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
 
/* ECO_IGNORE_BLOCK_BEGIN */
/* \trace SPEC-1880, SPEC-3742, SPEC-1915, SPEC-1891, SPEC-1914, SPEC-1932, SPEC-1929 */
#include "CanIf.h"

#include "CanIf_Cbk.h"

/* \trace SPEC-1225, SPEC-1859, SPEC-1891 */
/* Can.h with implied Can_Cfg.h is already included in file CanIf_Cfg.h */

#include "vstdlib.h"

#if (CANIF_TRCV_HANDLING == STD_ON)
# include "CanIf_CanTrcv.h"
#endif

/* \trace SPEC-3741, SPEC-1838 */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif


#include "SchM_CanIf.h"

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
# include "Mirror_Cbk.h"
#endif

#if (CANIF_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
# include "IdsM.h"
#endif

/* ECO_IGNORE_BLOCK_END */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* A component internal version check is not necessary. */

/* Check the compatibility version of generator */
# if (CANIF_CFG5_GENERATOR_COMPATIBILITY_VERSION != CANIF_GENDATA_COMPATIBILITY_VERSION)
#  error "Module CanIf: Source and Generated Header files are inconsistent! Please check the version of the generator."
# endif

/**********************************************************************************************************************
 *  COMPATIBILITY / DEFAULTS
 *********************************************************************************************************************/

/* ESCAN00084557 */
#if defined (CANIF_RX_PASS_STATIC_DLC)    /* COV_CANIF_FEATURE_DISCONTINUED */
# if (CANIF_RX_PASS_STATIC_DLC == STD_ON)
#  error "Feature CANIF_RX_PASS_STATIC_DLC is NOT supported by CAN interface anymore."
# endif
#endif

/* WakeUp Validation without WakeUp support not acceptable */
#if (CANIF_WAKEUP_VALIDATION == STD_ON)
# if (CANIF_WAKEUP_SUPPORT == STD_OFF)
#  error "Invalid configuration: Wake-up validation (CANIF_WAKEUP_VALIDATION) is activated without activating wake-up support (CANIF_WAKEUP_SUPPORT)"
# endif
#endif

#if (CANIF_WAKEUP_VALIDATION == STD_OFF)
# if (CANIF_WAKEUP_VALID_ALL_RX_MSGS == STD_ON)
#  error "Invalid configuration: Wake-up validation for all Rx PDUs (CANIF_WAKEUP_VALID_ALL_RX_MSGS) is activated without activating wake-up validation support (CANIF_WAKEUP_VALIDATION)"
# endif
#endif

#if (CANIF_WAKEUP_SUPPORT == STD_ON)
# if ((CANIF_CANDRV_WAKEUP_HANDLING == STD_OFF) && (CANIF_TRCV_HANDLING == STD_OFF))
#  error "Invalid configuration: Wake-up support (CANIF_WAKEUP_SUPPORT) is activated without activating wake-up handling for a CAN Driver or for a CAN Transceiver Driver"
# endif
#endif

#if (CANIF_PN_TRCV_HANDLING == STD_ON)
# if (CANIF_TRCV_HANDLING == STD_OFF)
#  error "Invalid configuration: Partial networking transceiver handling (CANIF_PN_TRCV_HANDLING) is enabled while transceiver handling is disabled (CANIF_TRCV_HANDLING)"
# endif
#endif

#if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
# if (CANIF_TRANSMIT_CANCELLATION == STD_ON)
#  error "HW transmit cancellation is NOT supported in combination with Tx-Buffer Type FIFO"
# endif
#endif

/* J1939DynAddr support without ExtendedIds support is not acceptable */
#if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
# if (CANIF_EXTENDEDID_SUPPORT == STD_OFF)
#  error "Invalid configuration: J1939DynAddr support (CANIF_J1939_DYN_ADDR_SUPPORT) is activated without activating ExtendedIds support (CANIF_EXTENDEDID_SUPPORT)"
# endif
#endif

#if (CANIF_CPU_TYPE_SET_IN_ECUC_MODULE != CPU_TYPE)
# error "Invalid configuration: CPU type configured in EcuC-module differs from \"CPU_TYPE\" set in \"Platform_Types.h\"."
#endif

/* Datatype UINT64 is only supported by target platform if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
   The CanIf unconditionally expects this type on 64-bit target platforms.
*/
#if (CPU_TYPE == CPU_TYPE_64) && (!defined PLATFORM_SUPPORT_SINT64_UINT64)
# error "Invalid configuration: PLATFORM_SUPPORT_SINT64_UINT64 is NOT defined"
#endif

/* Multi-partition support is only possible, if the following features are disabled.
   Not usable with third party CAN Drivers is checked by extra error directive below.
*/
#if (CANIF_SUPPORT_MULTIPARTITION == STD_ON)
# if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED) || (CANIF_ENABLE_SECURITY_EVENT_REPORTING == STD_ON) || (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
#  error "Invalid configuration: Multi-partition support is activated with at least one additional unsupported feature"
# endif
#endif

/* All third party CAN Drivers must have the same equivalence class. */
#if ((CANIF_SUPPORT_CANDRV_EQC_AR_403 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_AR_421 == STD_ON)) || \
    ((CANIF_SUPPORT_CANDRV_EQC_AR_403 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)) || \
    ((CANIF_SUPPORT_CANDRV_EQC_AR_421 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON))
# error "Invalid configuration: Third party CAN Drivers have different equivalence classes"
#endif

/* Inconsistent CAN Driver equivalence class switch settings. */
#if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421_OR_MSRC == STD_OFF)
# if (CANIF_SUPPORT_CANDRV_EQC_AR_403 == STD_ON) || (CANIF_SUPPORT_CANDRV_EQC_AR_421 == STD_ON) || (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON)
#  error "Invalid configuration: CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421_OR_MSRC has wrong value"
# endif
#endif

#if (CANIF_SUPPORT_CANDRV_EQC_AR_431_OR_MSRC == STD_OFF)
# if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) || (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON)
#  error "Invalid configuration: CANIF_SUPPORT_CANDRV_EQC_AR_431_OR_MSRC has wrong value"
# endif
#endif

#if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421 == STD_OFF)
# if (CANIF_SUPPORT_CANDRV_EQC_AR_403 == STD_ON) || (CANIF_SUPPORT_CANDRV_EQC_AR_421 == STD_ON)
#  error "Invalid configuration: CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421 has wrong value"
# endif
#endif

/* Unsupported features within third party CAN Drivers. */
#if ((CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) || (CANIF_SUPPORT_CANDRV_EQC_AR_421 == STD_ON)) && (CANIF_CHANGE_BAUDRATE_SUPPORT == STD_ON)
# error "Invalid configuration: Third party CAN Driver of EQC AR 4.2.1 or AR 4.3.1 is used with activated CanIf_ChangeBaudrate/CanIf_CheckBaudrate service functions (CANIF_CHANGE_BAUDRATE_SUPPORT)"
#endif

#if (CANIF_SUPPORT_CANDRV_EQC_AR_403 == STD_ON) && (CANIF_SET_BAUDRATE_API == STD_ON)
# error "Invalid configuration: Third party CAN Driver of EQC AR 4.0.3 is used with activated CanIf_SetBaudrate service function (CANIF_SET_BAUDRATE_API)"
#endif

#if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421 == STD_ON) || (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
# if (CANIF_CANCEL_SUPPORT_API == STD_ON) || (CANIF_BUS_MIRRORING_SUPPORT == STD_ON) || (CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON) || (CANIF_SUPPORT_MULTIPARTITION == STD_ON)
#  error "Invalid configuration: Third party CAN Driver is used with at least one unsupported feature"
# endif
#endif

/* Unsupported features in combination with CAN XL support */
#if ((CANIF_SUPPORT_CAN_XL == STD_ON) && ((CANIF_TRANSMIT_CANCELLATION == STD_ON) || (CANIF_SUPPORT_MULTIPARTITION == STD_ON)))
# error "Invalid configuration: CAN XL is used with at least one unsupported feature"
#endif

#if ((CANIF_SUPPORT_CAN_XL == STD_ON) && (CANIF_ECUC_SAFE_BSW_CHECKS == STD_ON))
# error "Invalid configuration: CAN XL is used but not supported in a Safe BSW configuration."
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS / LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* J1939 specific constants
 *  DA == Destination Address
 *  SA == Source Address
 *  INV == inverted
 *  ADDR == address
 *  PGN == Parameter Group Number
 *  PDU1 -> 4.BYTE == SA, 3.BYTE == DA, possible values: 0..EF
 *  PDU2 -> 4.BYTE == SA, 3.BYTE == part of PGN
 *  */
#define CANIF_J1939_NULL_ADDR                 0xFEu
#define CANIF_J1939_BROADCAST_ADDR            0xFFu
#define CANIF_J1939_DATA_PAGE_MASK            0x03000000u
#define CANIF_J1939_PGN_MASK                  0x03FF0000u
#define CANIF_J1939_AC_PGN                    0x00EE0000u
#define CANIF_J1939_MIN_PDU2_MASK             0x00F00000u
#define CANIF_J1939_DA_MASK                   0x0000FF00u
#define CANIF_J1939_INV_DA_MASK               0xFFFF00FFu
#define CANIF_J1939_SA_MASK                   0x000000FFu
#define CANIF_J1939_INV_SA_MASK               0xFFFFFF00u
#define CANIF_J1939_CTRL_ADDR_TBL_SIZE        256u

 /* ----- CAN XL specific constants ----- */
# define CANIF_CAN_XL_MAX_LENGTH                    2048u
# define CANIF_CAN_XL_ID_FIELD_FD_BIT_SHIFTPOS      31u
# define CANIF_CAN_XL_DLC_IN_FIRST_DATA_BYTE_MASK   0x0Fu
# define CANIF_LENGTH_OFFSET_OF_SDT3                0x1u /* The length of a CAN XL PDU of SDU type 3 is one byte greater than the actual payload of the tunneled CAN 2.0/FD PDU since an additional first byte containing the DLC is added. */
# define CANIF_CAN_XL_ID_FIELD_IDE_BIT_OFFSET       2u /* The Acceptance Field of a CAN XL PDU of SDU type 3 is interpreted as ID field where the IDE bit is bit 29 whearas in the generated CAN ID for configured PDUs it is bit 31. */
# define CANIF_CAN_XL_MAX_VCID                      255u
# define CANIF_CAN_XL_MAX_PRIORITY_ID               2047u
# define CANIF_CAN_XL_SDT_CAN_MAPPED_TUNNELING      3u  /* SDU type for tunneled CAN 2.0/FD PDU */

/* ----- CAN XL ID field masks ----- */
# define CANIF_CAN_XL_ID_FIELD_FD_BIT_MASK    0x80000000u
# define CANIF_CAN_XL_ID_FIELD_IDE_BIT_MASK   0x20000000u
# define CANIF_CAN_XL_ID_FIELD_CAN_ID_MASK    0x1FFFFFFFu

/* Determine MsgType (FD or none FD) from CanId */
/* IN: CanId (has to contain the FD-Flag) */
/* OUT: 0: None FD / 1: FD */
#define CanIf_GetShiftedFdFlag(canId) (uint8)(((canId) & CANIF_FD_FLAG) >> CANIF_FD_FLAG_SHIFTPOS) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* Determine MsgType (FD or none FD) from ID field. */
/* IN: ID field of CAN XL PDU with SDU type 3 */
/* OUT: 0: None FD / 1: FD */
#define CanIf_GetShiftedFdFlagOfIdField(idField) (uint8)(((idField) & CANIF_CAN_XL_ID_FIELD_FD_BIT_MASK) >> CANIF_CAN_XL_ID_FIELD_FD_BIT_SHIFTPOS) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */


/* ParameterList: uint8 ControllerId, PduIdType PduId, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR) CanSduPtr, uint8 CanDlc, Can_IdType CanId, uint8 ServiceIdOfCaller */
#if ((CANIF_SUPPORT_NMOSEK_INDICATION == STD_ON) || (CANIF_META_DATA_RX_SUPPORT == STD_ON) || (CANIF_RX_INDICATION_TYPE_IV_IS_USED == STD_ON))
# define CanIf_Indirect_HlIndication(ControllerId, PduId, CanSduPtr, CanDlc, CanId, ServiceIdOfCaller)     CanIf_HlIndication((ControllerId), (PduId), (CanSduPtr), (CanDlc), (CanId), (ServiceIdOfCaller)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
# define CanIf_Indirect_HlIndication(ControllerId, PduId, CanSduPtr, CanDlc, CanId, ServiceIdOfCaller)     CanIf_HlIndication((ControllerId), (PduId), (CanSduPtr), (CanDlc), (ServiceIdOfCaller)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#endif


/* generated types */

#define CanIf_RxIndFctListIdxType                          CanIf_RxIndicationFctListIdxOfRxPduConfigType
#define CanIf_TxConfFctListIdxType                         CanIf_TxConfirmationFctListIdxOfTxPduConfigType
#define CanIf_TxPduQueueType                               CanIf_TxPduQueueIndexStartIdxOfControllerConfigType
#define CanIf_CanTrcvFctTblIdx                             CanIf_CanTrcvFctTblIdxOfTransceiverUpToLowMapType


#define  CanIf_AnyIdxType uint32_least


# define CANIF_CFG_MAX_CONTROLLER                     CanIf_GetSizeOfCtrlStates()
# define CANIF_CFG_TX_CONTROLLER(index)               CanIf_GetCtrlStatesIdxOfTxPduConfig(index) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/* MAX valid TransceiverId with which the transceiver indication functions may be called */
#if (CANIF_TRCV_MAPPING == STD_ON)
# define CANIF_CFG_MAX_CANTRCV_TRCVHANDLEIDPLUSONE    CanIf_GetSizeOfTransceiverUpToUpperMap()
#else
# define CANIF_CFG_MAX_CANTRCV_TRCVHANDLEIDPLUSONE    CanIf_GetMaxTrcvHandleIdPlusOne()
#endif

#define CANIF_CFG_GENERATORCOMPATIBILITYVERSION            CanIf_GetGeneratorCompatibilityVersion()
#define CANIF_CFG_MAGICNUMBER                              CanIf_GetFinalMagicNumber()

#define CANIF_CFG_MAX_MAILBOXES                            CanIf_GetSizeOfMailBoxConfig()
#define CANIF_CFG_MAX_TXPDUS                               CanIf_GetSizeOfTxPduConfig()
#define CANIF_CFG_MAX_ULTXPDUS                             CanIf_GetSizeOfUlTxPduId2InternalTxPduId()
                                                           
#define CANIF_CFG_MAX_TXCONFIRMATIONS                      CanIf_GetSizeOfTxConfirmationFctList()
#define CANIF_CFG_MAX_RXPDUS                               CanIf_GetSizeOfRxPduConfig()
#define CANIF_CFG_MAX_ULRXPDUS                             CanIf_GetSizeOfUlRxPduId2InternalRxPduId()

#define CANIF_CFG_MAX_RXINDICATIONS                        CanIf_GetSizeOfRxIndicationFctList()
#define CANIF_CFG_MAX_WAKEUPSOURCES                        CanIf_GetMaxWakeUpSources()

/* If CAN XL Support is off, CanIf_GetNumOfNonCanXLRxPdus() does not exist but the number of total Rx PDUs equals the number of non CAN XL Rx PDUs (CAN 2.0/FD Rx PDUs). */
#if (CANIF_NUMOFNONCANXLRXPDUS == STD_ON) /* COV_CANIF_SUPPORT_CAN_XL */
# define CANIF_CFG_GET_NUM_OF_NON_CAN_XL_RX_PDUS CanIf_GetNumOfNonCanXLRxPdus()
#else
# define CANIF_CFG_GET_NUM_OF_NON_CAN_XL_RX_PDUS CanIf_GetSizeOfRxPduConfig()
#endif

/* PRQA S 3453 CANIF_FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* USAGE: Only if  CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON */
/* IN: Index get by CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX() */
/* OUT: CanIf-internal hxh-offset in order to be able to calculate the CanDrv specific hxh-index. For calculation just
   subtract the offset from the CanIf specific one which e.g. retrieved via CANIF_CFG_TX_HTH(). */
#define CANIF_CFG_MULTIPLECANDRV_HXHOFFSET(index)          CanIf_GetHxhOffset(index)

/* Access to table: CanIf_CanIfCtrlId2MappedTxBuffersConfig */
/* IN: ControllerId */
/* OUT: Start-index for corresponding CAN controller into table CanIf_MappedTxBuffersConfig */
#define CANIF_CFG_CTRL_STARTIDX2MAPPEDTXBUFFERS(index)     CanIf_GetMappedTxBuffersConfigStartIdxOfCanIfCtrlId2MappedTxBuffersConfig(index)
/* IN: ControllerId */
/* OUT: Stop-index for corresponding CAN controller into table CanIf_MappedTxBuffersConfig */
#define CANIF_CFG_CTRL_STOPIDX2MAPPEDTXBUFFERS(index)      CanIf_GetMappedTxBuffersConfigEndIdxOfCanIfCtrlId2MappedTxBuffersConfig(index)

/* Access to table: CanIf_MappedTxBuffersConfig */
/* IN: Index retrieved via macros: CANIF_CFG_CTRL_STARTIDX2MAPPEDTXBUFFERS / CANIF_CFG_CTRL_STOPIDX2MAPPEDTXBUFFERS */
/* OUT: Index into table CanIf_MailboxConfig */
#define CANIF_CFG_MAPPEDTXBUFFERS(index)                   CanIf_GetMailBoxConfigIdxOfMappedTxBuffersConfig(index)

/* TRUE: CAN driver index valid, FALSE CAN driver index NOT valid
 * Only relevant in PBS-configurations! FALSE only makes sense in PBS-configurations, where a CAN driver may be disabled in an identity.
 * Only if this one returns TRUE any CAN driver function may be read from table "CanIf_CanDrvFctTbl" */
#define CANIF_CFG_IS_CANDRVINDEXVALID(index)               CanIf_IsCanDrvFctTblUsedOfCanControllerIdUpToLowMap(index)  /* Input: ControllerId, Output: TRUE / FALSE */
#define CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(index)        CanIf_GetCanDrvFctTblIdxOfCanControllerIdUpToLowMap(index)  /* Input: ControllerId, Output: CanDrvFctTblIdx */

/* J1939(Nm) configuration */
#define CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(index)           CanIf_GetJ1939DynAddrSupportOfCtrlConfig(index)
/* NOTE: The value returned from following table may only be used if the CAN controller (== index) was verified with following condition
 * "CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(index) != CANIF_J1939_DYN_ADDR_DISABLED"
 * ONLY if this condition is TRUE the value returned from following table may be used! */
#define CANIF_CFG_CTRLID2J1939DYNADDROFFSET(index)         CanIf_GetJ1939DynAddrOffsetOfCtrlConfig(index)

#if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
 /* Input: ControllerId Output: Controller */
# define CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(index)        CanIf_GetControllerOfCanControllerIdUpToLowMap(index)
 /* Input: ControllerId Output: Function pointer to the corresponding CAN Driver function */
# define CANIF_GET_CANSETCTRLMODEFCT(index)                CanIf_GetSetControllerModeFctOfCanDrvFctTbl(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(index))
# define CANIF_GET_CANWRITEFCT(index)                      CanIf_GetWriteFctOfCanDrvFctTbl(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(index))
# define CANIF_GET_CANXLWRITEFCT(index)                    CanIf_GetXLWriteOfCanDrvFctTbl(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(index))
# define CANIF_GET_CANCANCELTXFCT(index)                   CanIf_GetCancelTxFctOfCanDrvFctTbl(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(index))
# define CANIF_GET_CANCHECKWAKEUPFCT(index)                CanIf_GetCheckWakeupFctOfCanDrvFctTbl(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(index))
#else /* Single CAN Driver */
 /* Input: ControllerId Output: ControllerId */
# define CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(index)        (index)
/* If CAN Driver uses API Infix a indirection macro Can_<FctName> to Can_<Infix>_<FctName> is contained in CanIf_Cfg.h */
# define canSetCtrlModeFct                                 Can_SetControllerMode
# define canWriteFct                                       Can_Write
# define canXLWriteFct                                     CanXL_Write
# define canCancelTxFct                                    Can_CancelTx
# define canCheckWakeupFct                                 Can_CheckWakeup
#endif

/* Access to table: CanIf_RxPduConfig */
#define CANIF_CFG_RX_RXINDICATION(index)                   CanIf_GetRxIndicationFctListIdxOfRxPduConfig(index)
#define CANIF_CFG_RX_DLC(index)                            CanIf_GetRxPduDlcOfRxPduConfig(index)
#define CANIF_CFG_RX_UPPERPDUID(index)                     CanIf_GetUpperPduIdOfRxPduConfig(index)
#define CANIF_CFG_RX_MASK(index)                           CanIf_GetRxPduMaskOfRxPduConfig(index)
#define CANIF_CFG_RX_CANID(index)                          CanIf_GetRxPduCanIdOfRxPduConfig(index)
#define CANIF_CFG_RX_RXMETADATALENGTH(index)               CanIf_GetRxMetaDataLengthOfRxPduConfig(index)
#define CANIF_CFG_RX_MSGTYPE(index)                        CanIf_GetMsgTypeOfRxPduConfig(index)
/* IN: Internal CanIfRxPduId to access table CanIf_RxPduConfig[] */
/* OUT: TRUE: data checksum Rx-PDU, FALSE: None data checksum Rx-PDU */
#define CANIF_CFG_RX_ISDATACHECKSUMPDU(index)              CanIf_IsIsDataChecksumRxPduOfRxPduConfig(index)
/* IN: Internal CanIfRxPduId to access table CanIf_RxPduConfig[] */
/* OUT: TRUE:  ReceptionMode  Rx-PDU, FALSE: None ReceptionMode  Rx-PDU */
#define CANIF_CFG_RX_ISRECEPTIONMODEPDU(index)             CanIf_IsPduRxModeUsedOfRxPduConfig(index)
/* IN: Internal CanIfRxPduId to access table CanIf_RxPduConfig[] */
/* OUT: Index to access the array CanIf_PduReceptionMode[] */
#define CANIF_CFG_RX_PDURECEPTIONMODEIDX(Index)            CanIf_GetPduRxModeIdxOfRxPduConfig(Index)
/* IN: External CanIfRxPduId */
/* OUT: Internal CanIfRxPduId */
#define CANIF_CFG_EXTRXPDUID2INTRXPDUID(index)             CanIf_GetRxPduConfigIdxOfUlRxPduId2InternalRxPduId(index)
/* IN: Internal CanIfRxPduId */
/* OUT: External CanIfRxPduId */
#define CANIF_CFG_INTRXPDUID2EXTRXPDUID(index)             CanIf_GetCanIfRxPduIdOfRxPduConfig(index)
/* IN: Internal CanIfRxPduId to access table CanIf_RxPduConfig[] */
/* OUT: TRUE: CAN-Nm Rx-PDU (for Wakeup validation), FALSE: No CAN-Nm Rx-PDU */
#define CANIF_ISCANNMRXPDU(index)                          CanIf_IsIsCanNmRxPduOfRxPduConfig(index)


/* Read/write-access to variable CanIf_PduReceptionMode[] */
#define CANIF_VAR_SET_PDU_RECEPTION_MODE(Index, Value)     CanIf_SetPduRxMode(Index, Value)
#define CANIF_VAR_GET_PDU_RECEPTION_MODE(Index)            CanIf_GetPduRxMode(Index)

/* NOTE: Please see description of "CANIF_CFG_CTRLID2J1939DYNADDROFFSET" before usage! */
#define CANIF_VAR_J1939RXADDR_LOOKUP(ctrl, addr)             CanIf_GetJ1939RxAddr_Lookup(CANIF_CFG_CTRLID2J1939DYNADDROFFSET(ctrl) + (addr))
#define CANIF_VAR_SET_J1939RXADDR_LOOKUP(ctrl, addr, value)  CanIf_SetJ1939RxAddr_Lookup((CANIF_CFG_CTRLID2J1939DYNADDROFFSET(ctrl) + (addr)), (value))

/* Access to table: CanIf_TxPduConfig */
#define CANIF_CFG_TX_CANID(index)                          CanIf_GetCanIdOfTxPduConfig(index)
#define CANIF_CFG_TX_HTH(index)                            CanIf_GetMailBoxConfigIdxOfTxPduConfig(index) /* Returns index to either MailBoxConfig table or XLMailBoxConfig table depending on whether the passed PDU ID belongs to a CAN 2.0/FD Tx PDU or a CAN XL Tx PDU. */
#define CANIF_CFG_TX_TXCONFIRMATION(index)                 CanIf_GetTxConfirmationFctListIdxOfTxPduConfig(index)
#define CANIF_CFG_TX_UPPERLAYERTXPDUID(index)              CanIf_GetUpperLayerTxPduIdOfTxPduConfig(index)
#define CANIF_CFG_TX_TXMETADATALENGTH(index)               CanIf_GetTxMetaDataLengthOfTxPduConfig(index)
#define CANIF_CFG_TX_LENGTH(index)                         CanIf_GetTxPduLengthOfTxPduConfig(index)
#define CANIF_CFG_TX_CANIDTXMASK(index)                    CanIf_GetCanIdTxMaskOfTxPduConfig(index)
/* IN: Internal CanIfTxPduId to access table: CanIf_TxPduConfig[] */
/* OUT: TRUE: data checksum Tx-PDU, FALSE: None data checksum Tx-PDU */
#define CANIF_CFG_TX_ISDATACHECKSUMPDU(index)              CanIf_IsIsDataChecksumTxPduOfTxPduConfig(index)
/* IN: Internal CanIfTxPduId to access table: CanIf_TxPduConfig[] */
/* OUT: TRUE: Tx-PDU will pass the Pn Wakeup filter, FALSE: Tx-PDU won't pass the Pn Wakeup filter */
#define CANIF_CFG_ISPNWUTXPDU(index)                       CanIf_IsIsPnWuTxPduOfTxPduConfig(index)
/* IN: Internal CanIfTxPduId to access table: CanIf_TxPduConfig[] */
/* OUT: TRUE: Truncation of Tx-PDU is enabled, FALSE: Truncation of Tx-PDU is disabled */
#define CANIF_CFG_TX_ISPDUTRUNCATION(index)                CanIf_IsIsTxPduTruncationOfTxPduConfig(index)

/* Access to table: CanIf_UlTxPduId2InternalTxPduId */
/* IN: Upper layer TxPduId */
/* OUT: Internal CanIfTxPduId */
#define CANIF_CFG_TXPDUID2INTTXPDUID(index)                CanIf_GetUlTxPduId2InternalTxPduId(index)

/* Access to table: CanIf_TxPduQueueIndex */
/* IN: Internal CanIfTxPduId */
/* OUT: Index to table CanIf_TxQueue[] */
#define CANIF_CFG_TX_QUEUEINDEX(index)                     CanIf_GetTxQueueIdxOfTxPduQueueIndex(index)

/* Access to table: CanIf_TxBufferFifoConfig */
/* IN: Index get by CANIF_CFG_MAILBOX_TXBUFFERIDX() in case of CANIF_CFG_MAILBOX_TXBUFFERHANDLINGTYPE() == FIFO. This applies for all CANIF_CFG_TXBUFFERFIFO_* access-macros. */
/* OUT: Access-index into variable CanIf_TxBufferFifoBase[] */
#define CANIF_CFG_TXBUFFERFIFO_BUFFERBASEIDX(index)        CanIf_GetTxBufferFifoBaseIdxOfTxBufferFifoConfig(index)
/* OUT: Access-start-index into variable CanIf_TxFifoQueueBase[] */
#define CANIF_CFG_TXBUFFERFIFO_QUEUEBASESTARTIDX(index)    CanIf_GetTxFifoQueueBaseStartIdxOfTxBufferFifoConfig(index)
#define CANIF_CFG_TXBUFFERFIFO_QUEUEBASEENDIDX(index)      CanIf_GetTxFifoQueueBaseEndIdxOfTxBufferFifoConfig(index)
/* OUT: Access-start-index into variable CanIf_TxFifoQueueData[] */
#define CANIF_CFG_TXBUFFFERFIFO_QUEUEDATASTARTIDX(index)   CanIf_GetTxFifoQueueDataStartIdxOfTxBufferFifoConfig(index)
/* OUT: Size of FIFO (== how many Tx-PDUs may be stored at all) */
#define CANIF_CFG_TXBUFFERFIFO_QUEUESIZE(index)            CanIf_GetTxFifoQueueBaseLengthOfTxBufferFifoConfig(index)
/* OUT: Size of one FIFO payload element [bytes] (== how many data bytes can be stored within one FIFO-queue-element) */
#define CANIF_CFG_TXBUFFERFIFO_SIZEOFONEPAYLOADEL(index)   CanIf_GetSizeOfOnePayloadElOfTxBufferFifoConfig(index)

/* Read-access to variable CanIf_TxBufferFifoBase[] */
/* Possible values for field: eWriteIdx, eReadIdx, eQueueCounter */
#define CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(index, field)    CanIf_GetTxBufferFifoBase(index).field
/* Read-access to variable CanIf_TxFifoQueueBase[] */
/* Possible values for field: eQueued, eBaseParams (includes: eCanId, eSduLength) */
#define CANIF_VAR_TXBUFFERFIFO_QUEUEBASE(index, field)     CanIf_GetTxFifoQueueBase(index).field
/* Read/write/address-access to variable CanIf_TxFifoQueueData[] */
#define CANIF_VAR_TXBUFFERFIFO_QUEUEDATA(index)            CanIf_GetTxFifoQueueData(index)
#define CANIF_VAR_SET_TXBUFFERFIFO_QUEUEDATA(index)        CanIf_SetTxFifoQueueData(index)
#define CANIF_ADDR_VAR_TXBUFFERFIFO_QUEUEDATA(index)       CanIf_GetAddrTxFifoQueueData(index)
/* Read/write/address-access to variable CanIf_TxQueueIndex2FifoEl[] */
#define CANIF_VAR_TXBUFFERFIFO_QUEUEIDX2FIFOEL(index)      CanIf_GetTxQueueIndex2FifoEl(index)
#define CANIF_VAR_SET_TXBUFFERFIFO_QUEUEIDX2FIFOEL(index, value)  CanIf_SetTxQueueIndex2FifoEl(index, value)
/* Get size of buffer size */
#define CANIF_CFG_SIZEOF_FIFO_BUFFER                       CanIf_GetSizeOfTxFifoQueueData()

/* Access to table: CanIf_TxBufferPrioByCanIdConfig */
/* IN: Index get by CANIF_CFG_MAILBOX_TXBUFFERIDX() in case of CANIF_CFG_MAILBOX_TXBUFFERHANDLINGTYPE() == PRIO_BY_CANID.
 * This applies for all CANIF_CFG_TXBUFFERPRIOBYCANID_* access-macros. */
/* OUT: Access-index into variable CanIf_TxBufferPrioByCanIdBase: QueueCounter. */
#define CANIF_CFG_TXBUFFERPRIOBYCANID_BUFFERBASEIDX(index)             CanIf_GetTxBufferPrioByCanIdBaseIdxOfTxBufferPrioByCanIdConfig(index)
/* OUT: Start-index into variable: CanIf_TxQueueFlags */
#define CANIF_CFG_TXBUFFERPRIOBYCANID_QUEUESTARTIDX(index)             CanIf_GetTxQueueFlagsStartIdxOfTxBufferPrioByCanIdConfig(index)
#define CANIF_CFG_TXBUFFERPRIOBYCANID_QUEUESTOPIDX(index)              CanIf_GetTxQueueFlagsEndIdxOfTxBufferPrioByCanIdConfig(index)
#define CANIF_CFG_TXBUFFERPRIOBYCANID_BITPOS2TXPDUIDOFFSET(index)      CanIf_GetBitPos2TxPduIdOffsetOfTxBufferPrioByCanIdConfig(index)


/* NOTE: Please see description of "CANIF_CFG_CTRLID2J1939DYNADDROFFSET" before usage! */
#define CANIF_VAR_J1939TXADDR_LOOKUP(ctrl, addr)             CanIf_GetJ1939TxAddr_Lookup(CANIF_CFG_CTRLID2J1939DYNADDROFFSET(ctrl) + (addr))
#define CANIF_VAR_SET_J1939TXADDR_LOOKUP(ctrl, addr, value)  CanIf_SetJ1939TxAddr_Lookup((CANIF_CFG_CTRLID2J1939DYNADDROFFSET(ctrl) + (addr)), (value))

/* Access to table: CanIf_MailboxConfig  */
#define CANIF_CFG_MAILBOX_PDUIDFIRST(index)                CanIf_GetPduIdFirstOfMailBoxConfig(index)
#define CANIF_CFG_MAILBOX_PDUIDLAST(index)                 CanIf_GetPduIdLastOfMailBoxConfig(index)
#define CANIF_CFG_MAILBOX_MAILBOXTYPE(index)               CanIf_GetMailBoxTypeOfMailBoxConfig(index)
#define CANIF_CFG_MAILBOX_CONTROLLER(index)                CanIf_GetCtrlStatesIdxOfMailBoxConfig(index)
#define CANIF_CFG_MAILBOX_TXBUFFERIDX(index)               CanIf_GetTxBufferCfgIdxOfMailBoxConfig(index)
#define CANIF_CFG_MAILBOX_TXBUFFERHANDLINGTYPE(index)      CanIf_GetTxBufferHandlingTypeOfMailBoxConfig(index)
#define CANIF_CFG_MAILBOX_HASTXBUFFER(index)               CanIf_IsTxBufferCfgUsedOfMailBoxConfig(index)

#define CANIF_CFG_WAKEUP_WAKEUPSOURCE(index)               CanIf_GetWakeUpSourceOfWakeUpConfig(index)
#define CANIF_CFG_WAKEUP_WAKEUPTARGETMODULE(index)         CanIf_GetWakeUpTargetModuleOfWakeUpConfig(index)
#define CANIF_CFG_WAKEUP_WAKEUPTARGETADDRESS(index)        CanIf_GetWakeUpTargetAddressOfWakeUpConfig(index)
#define CANIF_CFG_WAKEUP_CONTROLLERID(index)               CanIf_GetControllerIdOfWakeUpConfig(index)

#define CANIF_CFG_TRCVUPTOLOWMAP_DRIVERINDEX(index)        CanIf_GetCanTrcvFctTblIdxOfTransceiverUpToLowMap(index) /* Input: TransceiverId, Output: CanTrcvFctTblIdx */
#define CANIF_CFG_TRCVUPTOLOWMAP_TRCV(index)               CanIf_GetTransceiverOfTransceiverUpToLowMap(index)      /* Input: TransceiverId, Output: Transceiver/CanTrcvIndex */

#define CanIf_GetCanTrcv_SetOpModeFctMap(index)            CanIf_GetSetOpModeFctOfCanTrcvFctTbl(index)
#define CanIf_GetCanTrcv_GetOpModeFctMap(index)            CanIf_GetGetOpModeFctOfCanTrcvFctTbl(index)
#define CanIf_GetCanTrcv_GetBusWuReasonFctMap(index)       CanIf_GetGetBusWuReasonFctOfCanTrcvFctTbl(index)
#define CanIf_GetCanTrcv_SetWakeupModeFctMap(index)        CanIf_GetSetWakeupModeFctOfCanTrcvFctTbl(index)
#define CanIf_GetCanTrcv_CheckWakeupFctMap(index)          CanIf_GetCheckWakeupFctOfCanTrcvFctTbl(index)

#define CanIf_GetCanTrcv_ClearTrcvWufFlagFctMap(index)     CanIf_GetClearTrcvWufFlagFctOfCanTrcvFctTbl(index)
#define CanIf_GetCanTrcv_CheckWakeFlagFctMap(index)        CanIf_GetCheckWakeFlagFctOfCanTrcvFctTbl(index)


#define CANIF_CFG_RXDHRANDOMNUMBER1(index)                 CanIf_GetRxDHRandomNumber1OfCtrlConfig(index)
#define CANIF_CFG_RXDHRANDOMNUMBER2(index)                 CanIf_GetRxDHRandomNumber2OfCtrlConfig(index)

#define CANIF_CFG_RXDHADJUST(index)                        CanIf_GetRxDHAdjust(index)
#define CANIF_CFG_RXDHADJUSTCOUNT                          CanIf_GetSizeOfRxDHAdjust()

#define CANIF_CFG_STAT2DYN_INDIRECTION(index)                                       CanIf_GetTxPduStatic2DynamicIndirection(index)
#define CANIF_CFG_TXBUFFERPRIOBYCANID_STATFDQUEUE_QUEUEIDX2DATASTARTSTOP(index)     CanIf_GetTxQueueIndex2DataStartStopIdxOfTxPduQueueIndex(index)
/* Access to table: CanIf_TxQueueIndex2DataStartStop in order to redirect into variable CanIf_TxQueueData in case of static FD-queue */
#define CANIF_CFG_TXBUFFERPRIOBYCANID_STATFDQUEUE_DATASTARTIDX(index)               CanIf_GetTxQueueDataStartIdxOfTxQueueIndex2DataStartStop(CANIF_CFG_TXBUFFERPRIOBYCANID_STATFDQUEUE_QUEUEIDX2DATASTARTSTOP(index))
#define CANIF_CFG_TXBUFFERPRIOBYCANID_STATFDQUEUE_MAXDATALENGTH(index)              CanIf_GetTxQueueDataLengthOfTxQueueIndex2DataStartStop(CANIF_CFG_TXBUFFERPRIOBYCANID_STATFDQUEUE_QUEUEIDX2DATASTARTSTOP(index))

#define CANIF_CFG_ISFEATUREPNWUTXPDUFILTERENABLED(index)   CanIf_IsFeaturePnWuTxPduFilterEnabledOfCtrlConfig(index)
#define CANIF_CFG_TRCVTOCTRLMAP(index)                     CanIf_GetTrcvToCtrlMap(index)
#define CANIF_CFG_TRCVUPTOUPPERMAP(index)                  CanIf_GetTransceiverUpToUpperMap(index)

/* Access to variable: CanIf_TxQueue */
/* Possible values for field: eSdu[], eBaseParams (Includes: eCanId, eSduLength) */
#define CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUE(index, field)                      CanIf_GetTxQueue(index).field
/* Access to variable: CanIf_TxQueueData (static FD-queue) */
#define CANIF_VAR_TXBUFFERPRIOBYCANID_STATFDQUEUE_QUEUEDATA(index)             CanIf_GetTxQueueData(index)
#define CANIF_ADDR_VAR_TXBUFFERPRIOBYCANID_STATFDQUEUE_QUEUEDATA(index)        CanIf_GetAddrTxQueueData(index)
/* Access to variable: CanIf_TxQueueFlags */
#define CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUEFLAGS(index)                CanIf_GetTxQueueFlags(index)
#define CANIF_VAR_SET_TXBUFFERPRIOBYCANID_QUEUEFLAGS(index, value)     CanIf_SetTxQueueFlags((index), (value))
/* Access to variable: CanIf_DynTxId */
#define CANIF_VAR_DYNTXID(index)                                               CanIf_GetDynTxId(index)

#define CANIF_CFG_RXINDICATION_FUNCTION(index)             CanIf_GetRxIndicationFctOfRxIndicationFctList(index)
#define CANIF_CFG_RXINDICATION_FUNCTIONLAYOUT(index)       CanIf_GetRxIndicationLayoutOfRxIndicationFctList(index)
#define CANIF_CFG_TXCONFIRMATION(index)                    CanIf_GetTxConfirmationFctList(index)

#define CANIF_CFG_BUSOFFNOTIFICATION()                     CanIf_GetBusOffNotificationFctPtr()
#define CANIF_CFG_WAKEUPVALIDATION()                       CanIf_GetWakeUpValidationFctPtr()
#define CANIF_CFG_CTRLMODEINDICATIONFCT()                  CanIf_GetCtrlModeIndicationFctPtr()

/* FEAT-598: (Extended) RAM-check indications */
#define CANIF_CFG_RAMCHECKCORRUPTMAILBOXINDFCT()           CanIf_GetRamCheckCorruptMailboxIndFctPtr()
#define CANIF_CFG_RAMCHECKCORRUPTCONTROLLERINDFCT()        CanIf_GetRamCheckCorruptControllerIndFctPtr()


#define CANIF_CFG_TRCVMODEINDICATIONFCT()                  CanIf_GetTrcvModeIndicationFctPtr()
#define CANIF_CFG_CHECKTRCVWAKEFLAGINDICATIONFCT()         CanIf_GetCheckTrcvWakeFlagIndicationFctPtr()
#define CANIF_CFG_CLEARTRCVWUFFLAGINDICATIONFCT()          CanIf_GetClearTrcvWufFlagIndicationFctPtr()
#define CANIF_CFG_CONFIRMPNAVAILABILITYFCT()               CanIf_GetConfirmPnAvailabilityFctPtr()


#define CanIf_SetCtrlMode(index, value)                    CanIf_SetCtrlModeOfCtrlStates((index), (value))
#define CanIf_GetCtrlMode(index)                           CanIf_GetCtrlModeOfCtrlStates(index)

#define CANIF_VAR_SETPDUMODE(index, value)                 CanIf_SetPduModeOfCtrlStates((index), (value))
#define CANIF_VAR_GETPDUMODE(index)                        CanIf_GetPduModeOfCtrlStates(index)

#define CanIf_SetPnWuTxPduFilterActive(index, value)       CanIf_SetPnWuTxPduFilterActiveOfCtrlStates((index), (value))
#define CanIf_IsPnWuTxPduFilterActive(index)               CanIf_IsPnWuTxPduFilterActiveOfCtrlStates(index)

#define CanIf_SetTxConfState(index, value)                 CanIf_SetTxConfStateOfCtrlStates((index), (value))
#define CanIf_GetTxConfState(index)                        CanIf_GetTxConfStateOfCtrlStates(index)

#define CanIf_SetWakeUpValidationState(index, value)       CanIf_SetWakeUpValidationStateOfCtrlStates((index), (value))
#define CanIf_GetWakeUpValidationState(index)              CanIf_GetWakeUpValidationStateOfCtrlStates(index)

#define CanIf_ChangeMirroring(index, value)                CanIf_SetBusMirroringActiveOfCtrlStates((index), (value))
#define CanIf_IsMirroringEnabled(index)                    CanIf_IsBusMirroringActiveOfCtrlStates(index)

/* FEAT-1429 */
#define CanIf_SetTxQueueCounter(index, value)              (CanIf_GetTxBufferPrioByCanIdBase(index).eQueueCounter = (value))
#define CanIf_GetTxQueueCounter(index)                     (CanIf_GetTxBufferPrioByCanIdBase(index).eQueueCounter)
#define CanIf_IncTxQueueCounter(index)                     (++CanIf_GetTxBufferPrioByCanIdBase(index).eQueueCounter)
#define CanIf_DecTxQueueCounter(index)                     (--CanIf_GetTxBufferPrioByCanIdBase(index).eQueueCounter)


#define CanIf_GetCan_CheckBaudrateFctMap(index)            CanIf_GetCheckBaudrateFctOfCanDrvFctTbl(index)
#define CanIf_GetCan_ChangeBaudrateFctMap(index)           CanIf_GetChangeBaudrateFctOfCanDrvFctTbl(index)
#define CanIf_GetCan_SetBaudrateFctMap(index)              CanIf_GetSetBaudrateFctOfCanDrvFctTbl(index)
/* FEAT-598: (Extended) RAM-check APIs (only Multiple CanDrv) */
#define CanIf_GetCan_RamCheckExecuteFctMap(index)          CanIf_GetRamCheckExecuteFctOfCanDrvFctTbl(index)
#define CanIf_GetCan_RamCheckEnableMailboxFctMap(index)    CanIf_GetRamCheckEnableMailboxFctOfCanDrvFctTbl(index)
#define CanIf_GetCan_RamCheckEnableControllerFctMap(index) CanIf_GetRamCheckEnableControllerFctOfCanDrvFctTbl(index)

#define CanIf_GetCan_GetControllerErrorStateFctMap(index)      CanIf_GetGetControllerErrorStateFctOfCanDrvFctTbl(index)
#define CanIf_GetCan_GetControllerRxErrorCounterFctMap(index)  CanIf_GetGetControllerRxErrorCounterOfCanDrvFctTbl(index)
#define CanIf_GetCan_GetControllerTxErrorCounterFctMap(index)  CanIf_GetGetControllerTxErrorCounterOfCanDrvFctTbl(index) 

/*
|<DataModelStart>| CanIf_ConvertCanWriteAr431RetVal
Relation_Context:
CanIfSupportCanDrvEqcAr431 # from caller CanIf_CallCanWrite #
Relation:
CanIfSupportCanDrvEqcAr431, CanIfSupportCanDrvEqcMsrc
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/* Adaption of return value of Can_<Infix>_Write from CAN Driver of EQC AR 4.3.1.
   Used in CanIf_CallCanWrite() for CAN Driver of EQC AR 4.3.1 (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON).
   Look up table not possible, because nummerical values for Can_ReturnType are not specified by AUTOSAR. */
#if ((CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON))
/* If different CAN Driver EQCs are used the return value of Can_<Infix>_Write from CAN Driver of EQC 4.3.1 have to be converted to Can_ReturnType */
/* CAN_BUSY is part of enum Can_ReturnType, therefore cast to uint8 is needed to avoid MISRA warning (equality operator with unsigned and enum) */
#  define CanIf_ConvertCanWriteAr431RetVal(retval) (((retval) == E_OK) ? CAN_OK : (((retval) == (uint8)CAN_BUSY) ? CAN_BUSY : CAN_NOT_OK))
# else
/* If only CAN Drivers of EQC AR 4.3.1 are used the return value of Can_<Infix>_Write can be used directly */
#  define CanIf_ConvertCanWriteAr431RetVal(retval) (retval)
#endif

/* PRQA L: CANIF_FUNCTION_LIKE_MACRO */

#define CanIf_EnterCritical(area)     SchM_Enter_CanIf_##area()  /* PRQA S 0342 */ /*  MD_MSR_Rule20.10_0342 */
#define CanIf_LeaveCritical(area)     SchM_Exit_CanIf_##area()   /* PRQA S 0342 */ /*  MD_MSR_Rule20.10_0342 */

#define CANIF_NOP      ((uint8)0xFFu)

#if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
# if ( CPU_TYPE == CPU_TYPE_8 )
#  define CanIf_TxQueueBitShift 3u
# elif ( CPU_TYPE == CPU_TYPE_16 )
#  define CanIf_TxQueueBitShift 4u
# elif ( CPU_TYPE == CPU_TYPE_32 )
#  define CanIf_TxQueueBitShift 5u
# else /* CPU_TYPE_64 */
#  define CanIf_TxQueueBitShift 6u
# endif

/* Mask used to get the flag index from the handle */
# define CanIf_TxQueueBitMask   (((uint8)1u << CanIf_TxQueueBitShift) - (uint8)1u)
#endif

#if (CANIF_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
/* Rx/tx error counter threshold for error state passive */
# define CANIF_ERROR_COUNTER_THRESHOLD    127u
/* Result values for CANIF_CONTEXTDATA_THRESHOLD_POS:
   which error counter exceeded the threshold? */
# define CANIF_RX_TX_THRESHOLD_EXCEEDED   0u
# define CANIF_TX_THRESHOLD_EXCEEDED      1u
# define CANIF_RX_THRESHOLD_EXCEEDED      2u
/* Size of context data as number of bytes */
# define CANIF_CONTEXTDATA_SIZE_BUSOFF    1u
# define CANIF_CONTEXTDATA_SIZE_PASSIVE   2u
# define CANIF_CONTEXTDATA_SIZE_CANERROR  2u
/* Position in context data as byte number */
# define CANIF_CONTEXTDATA_CTRLID_POS     0u
# define CANIF_CONTEXTDATA_CANERROR_POS   1u
# define CANIF_CONTEXTDATA_THRESHOLD_POS  1u

/* CM_CANIF_1 */
# if (CANIF_CONTEXTDATA_SIZE_BUSOFF <= CANIF_CONTEXTDATA_CTRLID_POS)
#  error "Invalid static data: CANIF_CONTEXTDATA_SIZE_BUSOFF is too small"
# endif

/* CM_CANIF_1 */
# if ((CANIF_CONTEXTDATA_SIZE_PASSIVE <= CANIF_CONTEXTDATA_CTRLID_POS) || (CANIF_CONTEXTDATA_SIZE_PASSIVE <= CANIF_CONTEXTDATA_THRESHOLD_POS))
#  error "Invalid static data: CANIF_CONTEXTDATA_SIZE_PASSIVE is too small"
# endif

/* CM_CANIF_1 */
# if (CANIF_CONTEXTDATA_SIZE_CANERROR <= CANIF_CONTEXTDATA_CTRLID_POS) || (CANIF_CONTEXTDATA_SIZE_CANERROR <= CANIF_CONTEXTDATA_CANERROR_POS)
#  error "Invalid static data: CANIF_CONTEXTDATA_SIZE_CANERROR is too small"
# endif

#endif /* CANIF_ENABLE_SECURITY_EVENT_REPORTING == STD_ON */

/* Values for return data type CanIf_CanDrvReturnType (result handling of call Can_<Infix>_Write). */
#if ((CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_OFF))
/* Only CAN Drivers of EQC AR 4.3.1 - CanIf_CanDrvReturnType is Std_ReturnType. */
# define CANIF_CANDRV_OK      E_OK
# define CANIF_CANDRV_NOT_OK  E_NOT_OK
# define CANIF_CANDRV_BUSY    (uint8)CAN_BUSY /* Std_ReturnType is defined as uint8. CAN_BUSY can be defined as part of enum Can_ReturnType */
#else
/* For all other cases CanIf_CanDrvReturnType is Can_ReturnType.
   For calls to CAN Drivers of EQC AR 4.3.1 return value is converted to Can_ReturnType via CanIf_ConvertCanWriteAr431RetVal().
   No runtime change for CAN Driver of EQC MSRC. */
# define CANIF_CANDRV_OK      CAN_OK
# define CANIF_CANDRV_NOT_OK  CAN_NOT_OK
# define CANIF_CANDRV_BUSY    CAN_BUSY
#endif /* (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_OFF) */

#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
/* Returns the CAN controller mode.
   Used in CanIf_ControllerModeIndicationAr431().
   Input: CAN controller state (from CAN Driver of EQC 4.3.1)
   Output: Corresponding CAN controller mode (in CAN Interface)
*/
# define CanIf_GetCanIfCtrlModeOfCanCtrlState(CanCtrlState)  ((CanIf_ControllerModeType)CanIf_CanCtrlState2CanIfCtrlModeLookUp[(CanCtrlState)])
/* Comparison value to ensure that the lookup table CanIf_CanCtrlState2CanIfCtrlModeLookUp[] is only accessed with a valid index */
# define CANIF_CAN_CTRL_STATES_SIZE  4u
#endif

/* Returns the CAN Driver equivalence class specific CAN controller mode transistion.
   Used in CanIf_CallCanSetCtrlMode().
   Input: Abstracted CAN controller mode transistion value from CanIf_CanCtrlModeTransType (from CAN Interface).
   Output: Corresponding CAN controller mode transistion value according to the CAN Driver equivalence class:
           - Ar403: EQC AR 4.0.3/AR 4.2.1/MSRC.
           - Ar431: EQC AR 4.3.1
*/
# define CanIf_GetCanCtrlModeTransAr403(CanCtrlModeTrans) (CanIf_CanCtrlModeTransArLookUp[(CanCtrlModeTrans)].ModeAr403)
# define CanIf_GetCanCtrlModeTransAr431(CanCtrlModeTrans) (CanIf_CanCtrlModeTransArLookUp[(CanCtrlModeTrans)].ModeAr431)

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
 
#define CANIF_START_SEC_CONST_8BIT
#include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

CONST(uint8, CANIF_CONST) CanIf_MainVersion         = (uint8)CANIF_SW_MAJOR_VERSION;
CONST(uint8, CANIF_CONST) CanIf_SubVersion          = (uint8)CANIF_SW_MINOR_VERSION;
CONST(uint8, CANIF_CONST) CanIf_ReleaseVersion      = (uint8)CANIF_SW_PATCH_VERSION;

#define CANIF_STOP_SEC_CONST_8BIT
#include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
/* Return value data type for result handling of call Can_<Infix>_Write */
#if ((CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_OFF))
/* Only CAN Drivers of EQC AR 4.3.1 - Std_ReturnType have to be used (Can_ReturnType is may not defined at all). */
typedef Std_ReturnType CanIf_CanDrvReturnType;
# else
/* For all other cases Can_ReturnType is possible.
   For calls to CAN Drivers of EQC AR 4.3.1 return value is converted to Can_ReturnType via CanIf_ConvertCanWriteAr431RetVal().
   No runtime change for CAN Driver of EQC MSRC. */
typedef Can_ReturnType CanIf_CanDrvReturnType;
#endif

/* Group of CAN Driver equivalence class specific CAN controller mode data types.
   Used to call CAN Driver with its specific data type value.
   - Ar403: EQC AR 4.0.3/AR4.2.1/MSRC
   - Ar431: EQC AR 4.3.1
*/
typedef struct
{
#if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421_OR_MSRC == STD_ON)
  Can_StateTransitionType ModeAr403;
#endif

#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
  Can_ControllerStateType ModeAr431;
#endif
} CanIf_CanCtrlModeTransArTypes;

/* Abstracted CAN controller mode transistion data type. */
typedef enum
{
  CANIF_CANCTRL_TO_START = 0u,
  CANIF_CANCTRL_TO_STOP,
  CANIF_CANCTRL_TO_SLEEP,
  CANIF_CANCTRL_WAKEUP
} CanIf_CanCtrlModeTransType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
/***********************************************************************************************************************
 *  CanIf_TxQueueTreatment()
 **********************************************************************************************************************/
/*! \brief       Checks if queued PDUs have to be transmitted.
 *  \details     Called out of CanIf_TxConfirmation(), CanIf_CancelTxConfirmation(), CanIf_CancelTxNotification().
 *  \param[in]   hth: Index of the hardware transmit handle. Parameter must be smaller than CANIF_CFG_MAX_MAILBOXES.
 *  \return      E_OK: Any Tx-PDU transmitted.
 *               E_NOT_OK: No Tx-PDU transmitted.
 *  \pre         Must be called with entered critical section in order to secure the Tx-queue.
 *               Must only be called for "hth" to which any Tx-buffer is mapped.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON || CANIF_TRANSMIT_BUFFER_FIFO == STD_ON
 **********************************************************************************************************************/
CANIF_LOCAL FUNC(Std_ReturnType, CANIF_CODE) CanIf_TxQueueTreatment(CanIf_HwHandleType hth);


/***********************************************************************************************************************
 *  CanIf_TxQueueTransmit()
 **********************************************************************************************************************/
/*! \brief       Transmits a Tx-PDU and removes it from the Tx-queue.
 *  \details     Called out of CanIf_TxQueueTreatment().
 *  \param[in]   PduId: Internal Tx-PDU ID. Parameter must be smaller than CANIF_CFG_MAX_TXPDUS.
 *  \return      E_OK: Can_Write returned with E_OK or BUSY.
 *               E_NOT_OK: Can_Write returned with E_NOT_OK.
 *  \pre         Must be called with entered critical section in order to secure Tx-queue.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON || CANIF_TRANSMIT_BUFFER_FIFO == STD_ON
 **********************************************************************************************************************/
CANIF_LOCAL FUNC(Std_ReturnType, CANIF_CODE) CanIf_TxQueueTransmit(PduIdType PduId);


/***********************************************************************************************************************
 *  CanIf_ClearQueue()
 **********************************************************************************************************************/
/*! \brief       Initializes and resets the Tx-queue.
 *  \details     This API initializes the Tx-queue for CAN controller given by ControllerId.
 *               Called out of CanIf_SetControllerMode(), CanIf_ControllerBusOff(), CanIf_SetPduMode(), CanIf_Init().
 *  \param[in]   ControllerId: Handle-Id of CAN controller. Parameter must be smaller than CANIF_CFG_MAX_CONTROLLER.
 *  \pre         Check if parameter is in range from 0 to CANIF_CFG_MAX_CONTROLLER-1.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON OR CANIF_TRANSMIT_BUFFER_FIFO == STD_ON
 *  \note        The validity of parameter value is NOT checked
 **********************************************************************************************************************/
CANIF_LOCAL FUNC(void, CANIF_CODE) CanIf_ClearQueue(uint8 ControllerId);
#endif


#if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
/**********************************************************************************************************************
 *  CanIf_SetPrioByCanIdTxPduAsQueued()
 **********************************************************************************************************************/
/*! \brief       Sets the flag "is buffered".
 *  \details     This API sets the flag of the corresponding TxPdu and returns if it was set before.
 *               Called out of CanIf_Transmit(), CanIf_CancelTxConfirmation().
 *  \param[in]   PduId: Internal Tx-PDU ID for which the "is buffered" flag is set. Parameter must be smaller than CANIF_CFG_MAX_TXPDUS.
 *  \return      E_OK: PduId was not already buffered.
 *               E_NOT_OK: PduId was already buffered.
 *  \pre         Check if a PrioByCanId transmit buffer is assigned to the TxPdu.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON
 *  \note        The validity of parameter value is NOT checked.
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetPrioByCanIdTxPduAsQueued(PduIdType PduId);


/**********************************************************************************************************************
 *  CanIf_ClearPrioByCanIdTxPduAsQueued()
 **********************************************************************************************************************/
/*! \brief       Resets the flag "is buffered".
 *  \details     This API resets the flag of the corresponding TxPdu and returns if it was reset before.
 *               Called out of CanIf_Transmit(), CanIf_CancelTransmit(), CanIf_TxQueueTransmit(), CanIf_CancelTransmit().
 *  \param[in]   PduId: Internal Tx-PDU ID for which the "is buffered" flag is reset. Parameter must be smaller than CANIF_CFG_MAX_TXPDUS.
 *  \return      E_OK: PduId was not already buffered.
 *               E_NOT_OK: PduId was already buffered.
 *  \pre         Check if a PrioByCanId transmit buffer is assigned to the TxPdu.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON
 *  \note        The validity of parameter value is NOT checked.
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_ClearPrioByCanIdTxPduAsQueued(PduIdType PduId);
#endif


#if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
# if  (CANIF_CANCEL_SUPPORT_API == STD_ON)
/**********************************************************************************************************************
 *  CanIf_InitSubInitFifoQueue()
 **********************************************************************************************************************/
/*! \brief       Resets the flag "is buffered".
 *  \details     This API resets the flag for the complete Fifo buffer. Called out of CanIf_Init().
 *  \param[in]   -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      CANIF_TRANSMIT_BUFFER_FIFO == STD_ON && CANIF_CANCEL_SUPPORT_API == STD_ON
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_InitSubInitFifoQueue(void);
# endif
#endif


#if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
  /**********************************************************************************************************************
 *  CanIf_TransmitSubCheckFiFoQueueEmpty()
 **********************************************************************************************************************/
/*! \brief       Checks if the FIFO corresponding to the Tx PDU ID is empty.
 *  \details     Called out of CanIf_Transmit().
 *  \param[in]   CanTxPduId: Internal Tx-PDU ID to be checked. Parameter must be smaller than CANIF_CFG_MAX_TXPDUS.
 *  \return      E_OK:     Buffer is empty.
 *               E_NOT_OK: Buffer is not still empty.
 *  \pre         CanTxPduId must be mapped to a Tx-Buffer of handling type: FIFO
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_TRANSMIT_BUFFER_FIFO == STD_ON
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(boolean, CANIF_CODE) CanIf_TransmitSubCheckFiFoQueueEmpty(PduIdType CanTxPduId);
#endif


#if (CANIF_RXPDUCONFIGCANXLRXPDUSSDT3USEDOFCTRLCONFIG == STD_ON) /* COV_CANIF_SUPPORT_CAN_XL */ /* True, if CAN XL Rx PDUs of SDU type 3 exist or can be created at PB time */
/**********************************************************************************************************************
*  CanIf_XLRxIndicationSubCanIdMatchesConfiguredPdu()
**********************************************************************************************************************/
/*! \brief       Checks if the recevied CAN ID and message type match the configured CAN XL Rx PDU of SDU type 3.
 *  \details     Called out of CanIf_XLRxIndication().
 *  \param[in]   canXLRxPduId: Internal PDU ID of the CAN XL Rx PDU to be checked. Parameter must be smaller than CANIF_CFG_MAX_RXPDUS.
 *  \param[in]   canId: CAN ID (without FD bit) of the recevied CAN XL PDU of SDU type 3.
 *  \param[in]   msgType: Message type of the recevied CAN XL PDU of SDU type 3.
 *  \return      TRUE: CAN ID and message type match the configured CAN XL Rx PDU of SDU type 3.
 *               FALSE: CAN ID or message type does not match the configured CAN XL Rx PDU of SDU type 3.
 *  \pre         rxPduId must belong to a CAN XL Rx Pdu of SDU type 3.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_RXPDUCONFIGCANXLRXPDUSSDT3USEDOFCTRLCONFIG == STD_ON
**********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(boolean, CANIF_CODE) CanIf_XLRxIndicationSubCanIdMatchesConfiguredPdu(CanIf_RxPduConfigIterType canXLRxPduId, Can_IdType canId, CanIf_MsgType msgType);
#endif

/***********************************************************************************************************************
 *  CanIf_HlIndication()
 **********************************************************************************************************************/
/*! \brief       Reports a received Rx PDU to the upper layer.
 *  \details     Additionally it handles: Wake-up validation state, DLC check and meta data. Called out of CanIf_RxIndication()
 *               and CanIf_XLRxIndication().
 *  \param[in]   ControllerId: Abstracted CanIf ControllerId which is assigned to a CAN controller.
 *  \param[in]   PduId: Internal Rx PDU ID. Parameter must be smaller than CANIF_CFG_MAX_RXPDUS.
 *  \param[in]   CanSduPtr: Pointer to the received data.
 *  \param[in]   CanDlc: Length of the received data.
 *  \param[in]   CanId: CAN identifier (depends on configuration)..
 *  \param[in]   ServiceIdOfCaller: AUTOSAR defined service ID of the function calling CanIf_HlIndication.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
#if ((CANIF_SUPPORT_NMOSEK_INDICATION == STD_ON) || (CANIF_META_DATA_RX_SUPPORT == STD_ON) || (CANIF_RX_INDICATION_TYPE_IV_IS_USED == STD_ON))
CANIF_LOCAL FUNC(void, CANIF_CODE) CanIf_HlIndication(uint8 ControllerId, PduIdType PduId, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR) CanSduPtr, PduLengthType CanDlc, Can_IdType CanId, uint8 ServiceIdOfCaller);
#else
CANIF_LOCAL FUNC(void, CANIF_CODE) CanIf_HlIndication(uint8 ControllerId, PduIdType PduId, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR) CanSduPtr, PduLengthType CanDlc, uint8 ServiceIdOfCaller);
#endif


/***********************************************************************************************************************
 *  CanIf_HlTxConfirmation()
 **********************************************************************************************************************/
/*! \brief       Reports the confirmation for a successful transmission.
 *  \details     Called out of CanIf_TxConfirmation(), CanIf_Transmit().
 *  \param[in]   CanTxPduId: Internal Tx-PDU ID. Parameter must be smaller than CANIF_CFG_MAX_TXPDUS.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE only for different Tx-PDUs (CanTxPduId).
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
CANIF_LOCAL FUNC(void, CANIF_CODE) CanIf_HlTxConfirmation(PduIdType CanTxPduId);


/***********************************************************************************************************************
 *  CanIf_TransmitSubVerifyPduLength()
 **********************************************************************************************************************/
/*! \brief         Verifies the Tx PDU length and truncates it if necessary.
 *  \details       Called out of CanIf_Transmit().
 *  \param[in]     CanTxPduId: Internal Tx PDU ID. Parameter must be smaller than CANIF_CFG_MAX_TXPDUS.
 *  \param[in,out] pduLengthPtr: Pointer to Tx PDU data length to be transmitted.
 *  \return        CANIF_DO_TRANSMIT: Tx PDU may be transmitted.
 *                 CANIF_DO_NOT_TRANSMIT: Tx PDU may not be transmitted.
 *  \pre           -
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        -
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(CanIf_TransmitStateType, CANIF_CODE) CanIf_TransmitSubVerifyPduLength(PduIdType CanTxPduId, P2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) pduLengthPtr);


#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
/***********************************************************************************************************************
 *  CanIf_TransmitSubHandleTxPduFilter()
 **********************************************************************************************************************/
/*! \brief       Verifies whether the requested Tx-PDU may be transmitted or NOT.
 *  \details     While the PN filter is active only dedicated Tx PDUs are transmitted. Called out of CanIf_Transmit().
 *  \param[in]   ControllerId: Handle-Id of CAN controller. Parameter must be smaller than CANIF_CFG_MAX_CONTROLLER.
 *  \param[in]   CanTxPduId: Internal Tx-PDU ID. Parameter must be smaller than CANIF_CFG_MAX_TXPDUS.
 *  \return      CANIF_DO_TRANSMIT: Tx-PDU may be transmitted.
 *               CANIF_DO_NOT_TRANSMIT: Tx-PDU may not be transmitted.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_PN_WU_TX_PDU_FILTER == STD_ON
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(CanIf_TransmitStateType, CANIF_CODE) CanIf_TransmitSubHandleTxPduFilter(uint8 ControllerId, CONST(PduIdType, AUTOMATIC) CanTxPduId);
#endif


#if (CANIF_META_DATA_TX_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  CanIf_TransmitSubHandleMetaData()
 **********************************************************************************************************************/
/*! \brief       Extracts for a meta data Tx-PDU the meta data from passed payload and adapts the CAN identifier accordingly.
 *  \details     Called out of CanIf_Transmit().
 *  \param[in]   PduInfoPtr: PDU Info Pointer for payload including the meta data.
 *  \param[in]   localPduPtr: Pointer to Tx-PDU data (length, CAN identifier, handle ID) to be transmitted.
 *  \pre         All members of PduInfoPtr and localPduPtr except localPduPtr->length must have valid values. No parameter values are checked.
 *               Consistency of PduInfoPtr must be ensured. Hence this API must be called protected with critical area.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_META_DATA_TX_SUPPORT == STD_ON
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_TransmitSubHandleMetaData(P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_MSG_VAR) PduInfoPtr, P2VAR(Can_PduType, AUTOMATIC, AUTOMATIC) localPduPtr);
#endif


#if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED) 
/***********************************************************************************************************************
 *  CanIf_TransmitSubJ1939DynAddr()
 **********************************************************************************************************************/
/*! \brief       Modifies the CAN identifier according to J1939 specification.
 *  \details     Called out of CanIf_Transmit().
 *  \param[in]   ControllerId: Handle-Id of CAN controller. Parameter must be smaller than CANIF_CFG_MAX_CONTROLLER.
 *  \param[in]   localPduPtr: Pointer to PDU data.
 *  \return      E_OK: Modification of CAN identifier succeeded.
 *               E_NOT_OK: Modification of CAN identifier failed. In this case the processing of PDU must be cancelled.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_TransmitSubJ1939DynAddr(uint8 ControllerId, P2VAR(Can_PduType, AUTOMATIC, AUTOMATIC)localPduPtr);
#endif

#if (CANIF_TXPDUCANXLPROPS == STD_ON) /* COV_CANIF_SUPPORT_CAN_XL */ /* True, if CAN XL Tx PDUs exist or can be created at PB time */
/***********************************************************************************************************************
*  CanIf_TransmitSubXLWrite()
**********************************************************************************************************************/
/*! \brief      Handles the transmission of CAN XL Tx PDUs.
*  \details     Called out of CanIf_Transmit().
*  \param[in]   canXLTxPduId: Internal PDU ID of the CAN XL Tx PDU. Parameter must be smaller than CANIF_CFG_MAX_TXPDUS.
*  \param[in]   sduDataPtr: Pointer to SDU data.
*  \param[in]   sduLength: Tx PDU data length to be transmitted.
*  \return      E_OK: CanXL_Write() was called and returned E_OK.
*               E_NOT_OK: CanXL_Write() was either not called or returned E_NOT_OK.
*  \pre         -
*  \context     ANY
*  \reentrant   TRUE
*  \synchronous TRUE
*  \config      CANIF_TXPDUCANXLPROPS == STD_ON
**********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_TransmitSubXLWrite(PduIdType canXLTxPduId, SduDataPtrType sduDataPtr, uint16 sduLength);
#endif


#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
/***********************************************************************************************************************
 *  CanIf_TransmitSubHandleTxResBusy()
 **********************************************************************************************************************/
/*! \brief       Handles the Tx buffer.
 *  \details     Called out of CanIf_TransmitSubWrite().
 *  \param[in]   localPduPtr: Pointer to PDU.
 *  \return      E_OK: PDU was successfully buffered.
 *               E_NOT_OK: PDU could not be buffered.
 *  \pre         Must be called in context of entered CANIF_EXCLUSIVE_AREA_4.
 *  \context     ANY
 *  \reentrant   TRUE (only for a different (localPduPtr->swPduHandle))
 *  \synchronous TRUE
 *  \config      (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_TransmitSubHandleTxResBusy(P2CONST(Can_PduType, AUTOMATIC, AUTOMATIC) localPduPtr);
#endif


#if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
/***********************************************************************************************************************
 *  CanIf_RxIndicationSubJ1939DynAddr()
 **********************************************************************************************************************/
/*! \brief       Modifies the CAN identifier according to J1939 specification.
 *  \details     Called out of CanIf_RxIndicationAsr403().
 *  \param[in]   rxSearchParams: Parameters needed for J1939 dynamic addressing.
 *  \return      E_OK: Modification of CAN identifier succeeded.
 *               E_NOT_OK: Modification of CAN identifier failed. In this case the processing of PDU must be cancelled.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_RxIndicationSubJ1939DynAddr(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams);
#endif


#if (CANIF_SEARCH_ALGORITHM == CANIF_DOUBLE_HASH)
/***********************************************************************************************************************
 *  CanIf_RxIndicationSubDoubleHashSearch()
 **********************************************************************************************************************/
/*! \brief       Searches for a matched Rx-message according to double-hash search algorithm.
 *  \details     Called out of CanIf_RxIndicationAsr403().
 *  \param[in]   rxSearchParams: Parameters needed for search.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_SEARCH_ALGORITHM == CANIF_DOUBLE_HASH
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_RxIndicationSubDoubleHashSearch(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams);
#endif


#if (CANIF_SEARCH_ALGORITHM == CANIF_LINEAR)
/***********************************************************************************************************************
 *  CanIf_RxIndicationSubLinearSearch()
 **********************************************************************************************************************/
/*! \brief       Searches for a matched Rx-message according to linear search algorithm.
 *  \details     Called out of CanIf_RxIndicationAsr403().
 *  \param[in]   rxSearchParams: Parameters needed for search.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_SEARCH_ALGORITHM == CANIF_LINEAR
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_RxIndicationSubLinearSearch(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams);
#endif


/***********************************************************************************************************************
 *  CanIf_RxIndicationSubCheckPduProp()
 **********************************************************************************************************************/
/*! \brief       Checks the PDU properties (Message type, PDU reception mode and CAN ID of dynamic PDU).
 *  \details     Is called out from CanIf_RxIndicationAsr403() and CanIf_RxIndicationSubRangeSearch().
 *  \param[in]   rxSearchParams: Parameters needed for search.
 *  \return      CANIF_NO_NOTIFICATION: RxPduId does not match the PDU properties.
 *               CANIF_TX_RX_NOTIFICATION: RxPduId does match the PDU properties.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_RxIndicationSubCheckPduProp(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams);


/***********************************************************************************************************************
 *  CanIf_RxIndicationSubRangeSearch()
 **********************************************************************************************************************/
/*! \brief       Searches for a matched Rx-range.
 *  \details     This API uses the search algorithm linear. Called out of CanIf_RxIndicationAsr403().
 *  \param[in]   rxSearchParams: Parameters needed for search.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_RxIndicationSubRangeSearch(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams);


#if ((CANIF_WAKEUP_VALIDATION == STD_ON) && (CANIF_WAKEUP_VALID_ALL_RX_MSGS == STD_ON))
/***********************************************************************************************************************
 *  CanIf_RxIndicationSubWakeupValidation()
 **********************************************************************************************************************/
/*! \brief       Handles the wake-up validation state.
 *  \details     Called out of CanIf_RxIndicationAsr403().
 *  \param[in]   rxSearchParams: Required parameters.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      (CANIF_WAKEUP_VALIDATION == STD_ON) && (CANIF_WAKEUP_VALID_ALL_RX_MSGS == STD_ON)
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_RxIndicationSubWakeupValidation(P2CONST(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams);
#endif


/***********************************************************************************************************************
 *  CanIf_TransmitSubWrite()
 **********************************************************************************************************************/
/*! \brief       Tries to transmit a Tx PDU via CAN Driver. In case of CAN Driver is busy and the Tx PDU is mapped
 *               to a Tx buffer the Tx PDU is buffered otherwise discarded.
 *  \details     Called out of CanIf_Transmit().
 *  \param[in]   localPduPtr: Pointer to Tx PDU (CAN identifier, length, data, PDU ID) to be transmitted.
 *                            In case of enabled CANIF_MULTIPLE_CANDRV_SUPPORT or CANIF_TRANSMIT_BUFFER:
 *                            localPduPtr->swPduHandle must be smaller than CANIF_CFG_MAX_TXPDUS.
 *  \return      E_OK: PDU was successfuly handled. 
 *               E_NOT_OK: It was not possible to write the PDU to CAN Driver or to buffer it.
 *  \pre         Must be called in context of entered CANIF_EXCLUSIVE_AREA_4.
 *  \context     ANY
 *  \reentrant   TRUE (only for a different (localPduPtr->swPduHandle))
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC (Std_ReturnType, CANIF_CODE) CanIf_TransmitSubWrite(P2CONST(Can_PduType, AUTOMATIC, AUTOMATIC) localPduPtr);


/***********************************************************************************************************************
 *  CanIf_RxIndicationSubBasicCan()
 **********************************************************************************************************************/
/*! \brief       Handles the reception if a Rx-PDU which is mapped to a BasicCAN HW-object.
 *  \details     Called out of CanIf_RxIndicationAsr403().
 *  \param[in]   rxSearchParamsPtr: Pointer to parameters needed for search.
 *  \pre         Must only be called if Hrh is of type: BasicCAN.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_RxIndicationSubBasicCan(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams);


/***********************************************************************************************************************
 *  CanIf_HlIndicationSubULCall()
 **********************************************************************************************************************/
/*! \brief       Calls the uppper layer during reception.
 *  \details     Called out of CanIf_HlIndication().
 *  \param[in]   PduId: Internal Rx PDU ID. Parameter must be smaller than CANIF_CFG_MAX_RXPDUS.
 *  \param[in]   CanSduPtr: Pointer to the received data.
 *  \param[in]   CanDlc: Length of the received data.
 *  \param[in]   CanId: CAN identifier (depends on configuration).
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
#if (CANIF_SUPPORT_NMOSEK_INDICATION == STD_ON) || (CANIF_RX_INDICATION_TYPE_IV_IS_USED == STD_ON)
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_HlIndicationSubULCall(PduIdType PduId, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR) CanSduPtr, PduLengthType CanDlc, Can_IdType CanId);
#else
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_HlIndicationSubULCall(PduIdType PduId, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR) CanSduPtr, PduLengthType CanDlc);
#endif


/***********************************************************************************************************************
 *  CanIf_SetControllerModeSubSwitch()
 **********************************************************************************************************************/
/*! \brief       Decides which controller mode transition has to be executed at the CAN controller.
 *  \details     Called out of CanIf_SetControllerMode().
 *  \param[in]   ControllerId: Handle-Id of CAN controller. Parameter must be smaller than CANIF_CFG_MAX_CONTROLLER.
 *  \param[in]   ControllerMode: Target controller mode
 *  \return      E_OK: Controller mode transition was accepted.
 *               E_NOT_OK: Controller mode transition was rejected.
 *  \pre         Must be called in context of entered CANIF_EXCLUSIVE_AREA_0
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \config      -
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllerModeSubSwitch(uint8 ControllerId, CanIf_ControllerModeType ControllerMode);


/***********************************************************************************************************************
 *  CanIf_CallCanSetCtrlMode()
 **********************************************************************************************************************/
/*! \brief       Passes the requested CAN controller mode transistion to the responsible CAN Driver.
 *  \details     Called out of CanIf_SetControllerModeSubSwitch().
 *  \param[in]   ControllerId: Handle-Id of CAN controller. Parameter must be smaller than CANIF_CFG_MAX_CONTROLLER.
 *  \param[in]   canCtrlModeTrans: Requested CAN controller mode transistion.
 *  \return      E_OK: CAN controller mode transition was accepted by the CAN Driver.
 *               E_NOT_OK: CAN controller mode transistion was rejected by the CAN Driver.
 *  \pre         Must be called in context of entered CANIF_EXCLUSIVE_AREA_0.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous FALSE
 *  \config      -
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_CallCanSetCtrlMode(uint8 ControllerId, CanIf_CanCtrlModeTransType canCtrlModeTrans);


#if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
/***********************************************************************************************************************
 *  CanIf_TxBufferPrioByCanIdQueuePdu()
 **********************************************************************************************************************/
/*! \brief       Stores a PDU within the Tx-buffer of type: PRIO_BY_CANID
 *  \details     Called out of CanIf_TransmitSubHandleTxResBusy().
 *  \param[in]   PduInfoPtr: Contains length, PduId and datapointer.
 *  \pre         Must be called within entered CANIF_EXCLUSIVE_AREA_1 or CANIF_EXCLUSIVE_AREA_4
 *  \context     ANY
 *  \reentrant   TRUE (only for a different (PduInfoPtr->swPduHandle))
 *  \synchronous TRUE
 *  \config      CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON
 **********************************************************************************************************************/ 
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_TxBufferPrioByCanIdQueuePdu(P2CONST(Can_PduType, AUTOMATIC, CANIF_CBK_DRV_VAR) PduInfoPtr);
#endif

#if (CANIF_DEV_ERROR_DETECT == STD_ON)
# if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
/***********************************************************************************************************************
 *  CanIf_InitSubCheckJ1939AddrTables()
 **********************************************************************************************************************/
/*! \brief       Check the J1939 address tables configuration.
 *  \details     Called out of CanIf_Init().
 *  \return      E_OK: J1939 address tables configuration is consistent.
 *               E_NOT_OK: J1939 address tables configuration is inconsistent.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED) && (CANIF_DEV_ERROR_DETECT == STD_ON)
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_InitSubCheckJ1939AddrTables(void);
# endif
#endif


#if (CANIF_SETDYNAMICRXID_API == STD_ON)
/***********************************************************************************************************************
 *  CanIf_InitSubSetDynamicRxId()
 **********************************************************************************************************************/
/*! \brief       Initializes all dynamic Rx PDUs.
 *  \details     Called out of CanIf_Init().
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      CANIF_SETDYNAMICRXID_API == STD_ON
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_InitSubSetDynamicRxId(void);
#endif


/***********************************************************************************************************************
 *  CanIf_CallCanWrite()
 **********************************************************************************************************************/
/*! \brief       Passes the Tx PDU to the responsible CAN Driver.
 *  \details     Called out of CanIf_TransmitSubWrite() and CanIf_TxQueueTransmit().
 *  \param[in]   localPduPtr: Pointer to Tx PDU (CAN identifier, length, data, PDU ID) to be transmitted.
 *                            In case of enabled CANIF_MULTIPLE_CANDRV_SUPPORT: localPduPtr->swPduHandle must be smaller
 *                            than CANIF_CFG_MAX_TXPDUS.
 *  \return      E_OK/CAN_OK: Tx PDU was successfuly handled.
 *               E_NOT_OK/CAN_NOT_OK: Tx PDU was not successfuly handled.
 *               CAN_BUSY: CAN Driver is currently not able to handle the Tx PDU (buffer Tx PDU, if possible)
 *  \pre         Must be called in context of entered CANIF_EXCLUSIVE_AREA_4 or CANIF_EXCLUSIVE_AREA_2.
 *  \context     ANY
 *  \reentrant   TRUE (only for a different (localPduPtr->swPduHandle))
 *  \synchronous TRUE
 *  \config      -
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC (CanIf_CanDrvReturnType, CANIF_CODE) CanIf_CallCanWrite(P2CONST(Can_PduType, AUTOMATIC, AUTOMATIC) localPduPtr);


#if ((CANIF_WAKEUP_SUPPORT == STD_ON) && (CANIF_CANDRV_WAKEUP_HANDLING == STD_ON))
/***********************************************************************************************************************
 *  CanIf_CallCanCheckWakeup()
 **********************************************************************************************************************/
/*! \brief       Passes the CAN controller to be checked for a wake-up event to the responsible CAN Driver.
 *  \details     Called out of CanIf_CheckWakeup.
 *  \param[in]   ControllerId: Handle-Id of CAN controller. In case of enabled CANIF_MULTIPLE_CANDRV_SUPPORT ControllerId
                               must be smaller than CanIf_GetSizeOfCanControllerIdUpToLowMap().
 *  \return      E_OK: CAN controller signals the wake-up event.
 *               E_NOT_OK: CAN controller signals not the wake-up event.
 *  \pre         -
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      (CANIF_WAKEUP_SUPPORT == STD_ON) && (CANIF_CANDRV_WAKEUP_HANDLING == STD_ON)
 **********************************************************************************************************************/
CANIF_LOCAL_INLINE FUNC (Std_ReturnType, CANIF_CODE) CanIf_CallCanCheckWakeup(uint8 ControllerId);
#endif

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*
 * 1. Dim. -> CanIf_GetPduModeType
 * 2. Dim. -> CanIf_SetPduModeType
 *
 * typedef enum {
 * CANIF_SET_OFFLINE = 0,
 * CANIF_SET_RX_OFFLINE,
 * CANIF_SET_RX_ONLINE,
 * CANIF_SET_TX_OFFLINE,
 * CANIF_SET_TX_ONLINE,
 * CANIF_SET_ONLINE,
 * CANIF_SET_TX_OFFLINE_ACTIVE
 *} CanIf_SetPduModeType;
 *
 */
CANIF_LOCAL CONST(uint8, CANIF_CONST) CanIfState[CANIF_NO_MODE_PDU_GET_MODE_TYPE_END][CANIF_NO_MODE_PDU_SET_MODE_TYPE_END] = /* PRQA S 1514, 3218 */ /* MD_CanIf_Rule8.9, MD_CanIf_Rule8.9 */ 
{
  /* actual state CANIF_GET_OFFLINE: */  
  { 
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_GET_RX_ONLINE),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_GET_TX_ONLINE),
    (uint8)(CANIF_GET_ONLINE),
    (uint8)(CANIF_GET_OFFLINE_ACTIVE)
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
,   (uint8)(CANIF_GET_ONLINE_WAKF),        /* CANIF_SET_ONLINE_WAKF */
    (uint8)(CANIF_GET_TX_ONLINE_WAKF)      /* CANIF_SET_TX_ONLINE_WAKF */
#endif
  },
  
  /* actual state CANIF_GET_RX_ONLINE: */  
  {
    (uint8)(CANIF_GET_OFFLINE),
    (uint8)(CANIF_GET_OFFLINE),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_GET_ONLINE),
    (uint8)(CANIF_GET_ONLINE),
    (uint8)(CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE)
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
,   (uint8)(CANIF_GET_ONLINE_WAKF),             /* CANIF_SET_ONLINE_WAKF */
    (uint8)(CANIF_GET_ONLINE_WAKF)              /* CANIF_SET_TX_ONLINE_WAKF */
#endif
  },
  
  /* actual state CANIF_GET_TX_ONLINE: */  
  {
    (uint8)(CANIF_GET_OFFLINE),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_GET_ONLINE),
    (uint8)(CANIF_GET_OFFLINE),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_GET_ONLINE),
    (uint8)(CANIF_GET_OFFLINE_ACTIVE)
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
,   (uint8)(CANIF_GET_ONLINE_WAKF),   /* CANIF_SET_ONLINE_WAKF */
    (uint8)(CANIF_GET_TX_ONLINE_WAKF) /* CANIF_SET_TX_ONLINE_WAKF */
#endif
  },
    
  /* actual state CANIF_GET_ONLINE: */  
  {
    (uint8)(CANIF_GET_OFFLINE),
    (uint8)(CANIF_GET_TX_ONLINE),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_GET_RX_ONLINE),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE) 
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
,   (uint8)(CANIF_GET_ONLINE_WAKF),             /* CANIF_SET_ONLINE_WAKF */
    (uint8)(CANIF_GET_ONLINE_WAKF)              /* CANIF_SET_TX_ONLINE_WAKF */
#endif
  },
    
  /* actual state CANIF_GET_OFFLINE_ACTIVE: */  
  { 
    (uint8)(CANIF_GET_OFFLINE),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE),
    (uint8)(CANIF_GET_OFFLINE),
    (uint8)(CANIF_GET_TX_ONLINE),
    (uint8)(CANIF_GET_ONLINE),
    (uint8)(CANIF_NOP)
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
,   (uint8)(CANIF_GET_ONLINE_WAKF),              /* CANIF_SET_ONLINE_WAKF */
    (uint8)(CANIF_GET_TX_ONLINE_WAKF)            /* CANIF_SET_TX_ONLINE_WAKF */
#endif
  },
        
  /* actual state CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE: */  
  {
    (uint8)(CANIF_GET_OFFLINE),
    (uint8)(CANIF_GET_OFFLINE_ACTIVE),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_GET_RX_ONLINE),
    (uint8)(CANIF_GET_ONLINE),
    (uint8)(CANIF_GET_ONLINE),
    (uint8)(CANIF_NOP)
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
,   (uint8)(CANIF_GET_ONLINE_WAKF),    /* CANIF_SET_ONLINE_WAKF */
    (uint8)(CANIF_GET_ONLINE_WAKF)     /* CANIF_SET_TX_ONLINE_WAKF */
#endif
  }
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
,
  /* actual state CANIF_GET_ONLINE_WAKF: */
  {
    (uint8)(CANIF_GET_OFFLINE),
    (uint8)(CANIF_GET_TX_ONLINE_WAKF),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_GET_RX_ONLINE),
    (uint8)(CANIF_GET_ONLINE),
    (uint8)(CANIF_GET_ONLINE),
    (uint8)(CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_NOP)
  },
  /* actual state CANIF_GET_TX_ONLINE_WAKF: */
  {
    (uint8)(CANIF_GET_OFFLINE),
    (uint8)(CANIF_NOP),
    (uint8)(CANIF_GET_ONLINE_WAKF),
    (uint8)(CANIF_GET_OFFLINE),
    (uint8)(CANIF_GET_TX_ONLINE),
    (uint8)(CANIF_GET_ONLINE),
    (uint8)(CANIF_GET_OFFLINE_ACTIVE),
    (uint8)(CANIF_GET_ONLINE_WAKF),
    (uint8)(CANIF_NOP)
  }
#endif
};

#if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
/* lookup table for setting the flags in the queue */
CANIF_LOCAL CONST(CanIf_TxQueueFlagsType, CANIF_CONST) CanIf_TxQueueBitFlagsLookUp[1u << CanIf_TxQueueBitShift] =
{
  (CanIf_TxQueueFlagsType)0x00000001u, (CanIf_TxQueueFlagsType)0x00000002u, (CanIf_TxQueueFlagsType)0x00000004u, (CanIf_TxQueueFlagsType)0x00000008u,
  (CanIf_TxQueueFlagsType)0x00000010u, (CanIf_TxQueueFlagsType)0x00000020u, (CanIf_TxQueueFlagsType)0x00000040u, (CanIf_TxQueueFlagsType)0x00000080u
# if (CPU_TYPE > CPU_TYPE_8)
,  (CanIf_TxQueueFlagsType)0x00000100u, (CanIf_TxQueueFlagsType)0x00000200u, (CanIf_TxQueueFlagsType)0x00000400u, (CanIf_TxQueueFlagsType)0x00000800u,
   (CanIf_TxQueueFlagsType)0x00001000u, (CanIf_TxQueueFlagsType)0x00002000u, (CanIf_TxQueueFlagsType)0x00004000u, (CanIf_TxQueueFlagsType)0x00008000u
# endif
# if (CPU_TYPE > CPU_TYPE_16)
,  (CanIf_TxQueueFlagsType)0x00010000u, (CanIf_TxQueueFlagsType)0x00020000u, (CanIf_TxQueueFlagsType)0x00040000u, (CanIf_TxQueueFlagsType)0x00080000u,
   (CanIf_TxQueueFlagsType)0x00100000u, (CanIf_TxQueueFlagsType)0x00200000u, (CanIf_TxQueueFlagsType)0x00400000u, (CanIf_TxQueueFlagsType)0x00800000u,
   (CanIf_TxQueueFlagsType)0x01000000u, (CanIf_TxQueueFlagsType)0x02000000u, (CanIf_TxQueueFlagsType)0x04000000u, (CanIf_TxQueueFlagsType)0x08000000u,
   (CanIf_TxQueueFlagsType)0x10000000u, (CanIf_TxQueueFlagsType)0x20000000u, (CanIf_TxQueueFlagsType)0x40000000u, (CanIf_TxQueueFlagsType)0x80000000u
# endif
# if (CPU_TYPE > CPU_TYPE_32)
,  (CanIf_TxQueueFlagsType)0x0000000100000000u, (CanIf_TxQueueFlagsType)0x0000000200000000u, (CanIf_TxQueueFlagsType)0x0000000400000000u, (CanIf_TxQueueFlagsType)0x0000000800000000u,
   (CanIf_TxQueueFlagsType)0x0000001000000000u, (CanIf_TxQueueFlagsType)0x0000002000000000u, (CanIf_TxQueueFlagsType)0x0000004000000000u, (CanIf_TxQueueFlagsType)0x0000008000000000u,
   (CanIf_TxQueueFlagsType)0x0000010000000000u, (CanIf_TxQueueFlagsType)0x0000020000000000u, (CanIf_TxQueueFlagsType)0x0000040000000000u, (CanIf_TxQueueFlagsType)0x0000080000000000u,
   (CanIf_TxQueueFlagsType)0x0000100000000000u, (CanIf_TxQueueFlagsType)0x0000200000000000u, (CanIf_TxQueueFlagsType)0x0000400000000000u, (CanIf_TxQueueFlagsType)0x0000800000000000u,
   (CanIf_TxQueueFlagsType)0x0001000000000000u, (CanIf_TxQueueFlagsType)0x0002000000000000u, (CanIf_TxQueueFlagsType)0x0004000000000000u, (CanIf_TxQueueFlagsType)0x0008000000000000u,
   (CanIf_TxQueueFlagsType)0x0010000000000000u, (CanIf_TxQueueFlagsType)0x0020000000000000u, (CanIf_TxQueueFlagsType)0x0040000000000000u, (CanIf_TxQueueFlagsType)0x0080000000000000u,
   (CanIf_TxQueueFlagsType)0x0100000000000000u, (CanIf_TxQueueFlagsType)0x0200000000000000u, (CanIf_TxQueueFlagsType)0x0400000000000000u, (CanIf_TxQueueFlagsType)0x0800000000000000u,
   (CanIf_TxQueueFlagsType)0x1000000000000000u, (CanIf_TxQueueFlagsType)0x2000000000000000u, (CanIf_TxQueueFlagsType)0x4000000000000000u, (CanIf_TxQueueFlagsType)0x8000000000000000u,
# endif
};
#endif

#if (CANIF_DEV_ERROR_DETECT == STD_ON)
/* lookup table for maximum value of RxDlc */
CANIF_LOCAL CONST(uint8, CANIF_CONST) RxDlcMaxValueLookUp[2] = /* PRQA S 3218 */ /* MD_CanIf_Rule8.9 */
{
  8u, /* 0: NONE FD */
  64u /* 1: FD */
};
#endif

#if (CANIF_SUPPORT_CAN_XL == STD_ON) /* COV_CANIF_SUPPORT_CAN_XL */
/* lookup table to convert the DLC to the SDU length */
CANIF_LOCAL CONST(uint8, CANIF_CONST) DlcToSduLengthLookUp[16] = /* PRQA S 3218 */ /* MD_CanIf_Rule8.9 */
{ 
  0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 12u, 16u, 20u, 24u, 32u, 48u, 64u
};
#endif

#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
/* Lookup table to convert the states from Can_ControllerStateType to the modes from CanIf_ControllerModeType.
   - The values of the states from Can_ControllerStateType are explicitly defined for CAN Driver of EQC AR 4.3.1 in AUTOSAR specification.
   - The values of the modes from CanIf_ControllerModeType are defined in CanIf_Types.h.
*/
CANIF_LOCAL CONST(uint8, CANIF_CONST) CanIf_CanCtrlState2CanIfCtrlModeLookUp[CANIF_CAN_CTRL_STATES_SIZE] = /* PRQA S 3218 */ /* MD_CanIf_Rule8.9 */
{
  /* Index (Can_ControllerStateType)  CanIf_ControllerModeType */
  /* 0: CAN_CS_UNINIT  */             (uint8)CANIF_CS_UNINIT  /* value 0 */,
  /* 1: CAN_CS_STARTED */             (uint8)CANIF_CS_STARTED /* value 2 */,
  /* 2: CAN_CS_STOPPED */             (uint8)CANIF_CS_STOPPED /* value 1 */,
  /* 3: CAN_CS_SLEEP   */             (uint8)CANIF_CS_SLEEP   /* value 3 */
};
#endif

/* Look up table to convert the abstracted CAN controller mode transistion value (CanIf_CanCtrlModeTransType)
   to CAN Driver equivalence class specific value (Can_StateTransitionType or Can_ControllerStateType).
*/
CANIF_LOCAL CONST(CanIf_CanCtrlModeTransArTypes, CANIF_CONST) CanIf_CanCtrlModeTransArLookUp[] = /* PRQA S 3218 */ /* MD_CanIf_Rule8.9 */
{
  /* CANIF_CANCTRL_TO_START */
  {
#if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421_OR_MSRC == STD_ON)
    CAN_T_START
#endif
#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON)
,
#endif
#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
    CAN_CS_STARTED
#endif
  },
  /* CANIF_CANCTRL_TO_STOP */
  {
#if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421_OR_MSRC == STD_ON)
    CAN_T_STOP
#endif
#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC== STD_ON)
,
#endif
#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
    CAN_CS_STOPPED
#endif
  },
  /* CANIF_CANCTRL_TO_SLEEP */
  {
#if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421_OR_MSRC == STD_ON)
    CAN_T_SLEEP
#endif
#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON)
,
#endif
#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
    CAN_CS_SLEEP
#endif
  },
  /* CANIF_CANCTRL_WAKEUP */
  {
#if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421_OR_MSRC == STD_ON)
    CAN_T_WAKEUP
#endif
#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON)
,
#endif
#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
    CAN_CS_STOPPED
#endif
  }
};

#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define CANIF_START_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (CANIF_USE_INIT_POINTER == STD_ON)
P2CONST(CanIf_ConfigType, CANIF_VAR_INIT, CANIF_XCFG) CanIf_ConfigDataPtr = NULL_PTR; /* UREQ00035484 */

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
CANIF_LOCAL VAR(boolean, CANIF_VAR_INIT) CanIf_SystemInit = FALSE;
# endif
# endif /* CANIF_USE_INIT_POINTER == STD_ON */


#define CANIF_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS / GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
/**********************************************************************************************************************
 *  CanIf_TransmitSubCheckFiFoQueueEmpty()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TransmitSubCheckFiFoQueueEmpty
Relation_Context:
IfTxBuffer # from this function #
IfTxBuffer # from caller CanIf_TransmitSubWrite #
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CANIF_LOCAL_INLINE FUNC(boolean, CANIF_CODE) CanIf_TransmitSubCheckFiFoQueueEmpty(PduIdType CanTxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;
  CanIf_AnyIdxType txBufferCfgIdx;
  CanIf_AnyIdxType hth;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Has PDU a buffer and is it handled as FIFO? */
  hth = CANIF_CFG_TX_HTH(CanTxPduId);
  txBufferCfgIdx = CANIF_CFG_MAILBOX_TXBUFFERIDX(hth);
  /* #110 Only if the FIFO is empty at all, the Tx-PDU may be transmitted directly otherwise the Tx-PDU must be queued into FIFO in order to ensure FIFO-behavior. */
  if (CANIF_VAR_TXBUFFERFIFO_BUFFERBASE((CANIF_CFG_TXBUFFERFIFO_BUFFERBASEIDX(txBufferCfgIdx)), eQueueCounter) == 0u)
  {
    /* NO */
    retVal = TRUE;
  }
  return retVal;
}
#endif

#if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
# if (CANIF_CANCEL_SUPPORT_API == STD_ON) 
/**********************************************************************************************************************
 *  CanIf_InitSubInitFifoQueue()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_InitSubInitFifoQueue
Relation_Context:
IfTxBuffer, TransmitCancellationAPI # from this local function #
IfTxBuffer, TransmitCancellationAPI # from caller CanIf_Init #
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_InitSubInitFifoQueue(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  CanIf_AnyIdxType counter;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Iterate over all CanIf_TxQueueIndex2FifoEl and set them invalid */
  for(counter = 0; counter < CanIf_GetSizeOfTxQueueIndex2FifoEl(); counter++)
  {
    CANIF_VAR_SET_TXBUFFERFIFO_QUEUEIDX2FIFOEL((counter), CANIF_NO_ENTRY_IN_TX_FIFO); /* SBSW_CANIF_15 */
  }
  /* #200 Iterate over all CanIf_TxFifoQueueBase and set them "not queued" */
  for(counter = 0; counter < CanIf_GetSizeOfTxFifoQueueBase(); counter++)
  {
    CANIF_VAR_TXBUFFERFIFO_QUEUEBASE( counter, eQueued) = 0; /* SBSW_CANIF_15 */
  }
}
# endif
#endif

#if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
/**********************************************************************************************************************
 *  CanIf_SetPrioByCanIdTxPduAsQueued()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_SetPrioByCanIdTxPduAsQueued
Relation_Context:
CanIfTxBuffer # from this local function #
CanIfTxBuffer # from caller CanIf_TransmitSubHandleTxResBusy #
CanIfTxBuffer, CanIfTransmitCancellationAPI # from caller CanIf_CancelTxConfirmation #
Relation:
CanIfTxBuffer
CanIfTxBuffer, CanIfDevErrorDetect
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetPrioByCanIdTxPduAsQueued(PduIdType PduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  Std_ReturnType retval = E_NOT_OK;
  const CanIf_AnyIdxType txBufferCfgIdx = CANIF_CFG_MAILBOX_TXBUFFERIDX(CANIF_CFG_TX_HTH(PduId));

  /* ----- Implementation ----------------------------------------------- */

  /* #100 Compute the elementIdx and the element bit position */
  const PduIdType queueBitPos  =  (PduIdType)(PduId - CANIF_CFG_TXBUFFERPRIOBYCANID_BITPOS2TXPDUIDOFFSET(txBufferCfgIdx));
  const CanIf_AnyIdxType queueElementIdx = (((CanIf_AnyIdxType)queueBitPos) >> CanIf_TxQueueBitShift) + CANIF_CFG_TXBUFFERPRIOBYCANID_QUEUESTARTIDX(txBufferCfgIdx); /* PRQA S 2842 */ /* MD_CanIf_Dir4.1_2842 */
  const uint8 elementBitIdx  = (uint8)(queueBitPos & CanIf_TxQueueBitMask);

  /* #110 Is flag already set for given PDU ID?*/
  if( (CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUEFLAGS(queueElementIdx) & CanIf_TxQueueBitFlagsLookUp[elementBitIdx]) == (CanIf_TxQueueFlagsType)0u ) /* PRQA S 2842 */ /* MD_CanIf_Dir4.1_2842 */
  {
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
    if (queueElementIdx >= CanIf_GetSizeOfTxQueueFlags()) /* COV_CANIF_GENDATA_FAILURE */
    {
      errorId = CANIF_E_FATAL;
    }
    else
#  endif
    {
      /* #120 If not already set do it now and increment counter */
      CANIF_VAR_SET_TXBUFFERPRIOBYCANID_QUEUEFLAGS(queueElementIdx, (CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUEFLAGS(queueElementIdx) | CanIf_TxQueueBitFlagsLookUp[elementBitIdx])); /* SBSW_CANIF_20 */
      CanIf_IncTxQueueCounter(CANIF_CFG_TXBUFFERPRIOBYCANID_BUFFERBASEIDX(txBufferCfgIdx)); /* SBSW_CANIF_25 */
      retval = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */ /* COV_CANIF_GENDATA_FAILURE */
  {
    CanIf_Det_ReportError(CANIF_INIT_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}

/**********************************************************************************************************************
 *  CanIf_ClearPrioByCanIdTxPduAsQueued()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ClearPrioByCanIdTxPduAsQueued
Relation_Context:
CanIfTxBuffer # from this local function #
CanIfTxBuffer # from caller CanIf_TransmitSubWrite, CanIf_TxQueueTransmit #
CanIfTxBuffer, CanIfTransmitCancellationAPI # from caller CanIf_CancelTransmit #
Relation:
CanIfTxBuffer
CanIfTxBuffer, CanIfDevErrorDetect
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_ClearPrioByCanIdTxPduAsQueued(PduIdType PduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  Std_ReturnType retval = E_NOT_OK;
  const CanIf_AnyIdxType txBufferCfgIdx = CANIF_CFG_MAILBOX_TXBUFFERIDX(CANIF_CFG_TX_HTH(PduId));

  /* ----- Implementation ----------------------------------------------- */

  /* #110 Compute the elementIdx and the element bit position */
  const PduIdType queueBitPos  =  (PduIdType)(PduId - CANIF_CFG_TXBUFFERPRIOBYCANID_BITPOS2TXPDUIDOFFSET(txBufferCfgIdx));
  const CanIf_AnyIdxType queueElementIdx = (((CanIf_AnyIdxType)queueBitPos) >> CanIf_TxQueueBitShift) + CANIF_CFG_TXBUFFERPRIOBYCANID_QUEUESTARTIDX(txBufferCfgIdx); /* PRQA S 2842 */ /* MD_CanIf_Dir4.1_2842 */
  const uint8 elementBitIdx  = (uint8)(queueBitPos & CanIf_TxQueueBitMask);

  /* #120 Is flag set for given PDU ID?*/
  if( (CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUEFLAGS(queueElementIdx) & CanIf_TxQueueBitFlagsLookUp[elementBitIdx]) != (CanIf_TxQueueFlagsType)0u ) /* PRQA S 2842 */ /* MD_CanIf_Dir4.1_2842 */
  {
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
    if (queueElementIdx >= CanIf_GetSizeOfTxQueueFlags()) /* COV_CANIF_GENDATA_FAILURE */
    {
      errorId = CANIF_E_FATAL;
    }
    else
#  endif
    {
      /* #130 If set clear it and decrement counter */
      CANIF_VAR_SET_TXBUFFERPRIOBYCANID_QUEUEFLAGS(queueElementIdx, (CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUEFLAGS(queueElementIdx) & ((CanIf_TxQueueFlagsType) ~CanIf_TxQueueBitFlagsLookUp[elementBitIdx]))); /* SBSW_CANIF_20 */
      CanIf_DecTxQueueCounter(CANIF_CFG_TXBUFFERPRIOBYCANID_BUFFERBASEIDX(txBufferCfgIdx));  /* SBSW_CANIF_25 */
      retval = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */ /* COV_CANIF_GENDATA_FAILURE */
  {
    CanIf_Det_ReportError(CANIF_INIT_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif


#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
/**********************************************************************************************************************
 *  CanIf_ClearQueue()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ClearQueue
Relation_Context:
CanIfTxBuffer # from this local function #
CanIfTxBuffer # from caller CanIf_Init, CanIf_SetControllerModeSubSwitch, CanIf_ControllerBusOff, CanIf_SetPduMode #
Relation:
CanIfTxBuffer
CanIfTxBuffer, CanIfTransmitCancellationAPI
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CANIF_LOCAL FUNC(void, CANIF_CODE) CanIf_ClearQueue(uint8 ControllerId)
{
  /* ----- Local Variables ---------------------------------------------- */
  CanIf_AnyIdxType idx2MappedTxBufferCfg; /* for-loop counter */

  /* ----- Implementation ----------------------------------------------- */
  CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_1);

  /* #100 Get all HTHs with configured Tx buffer of affected CAN controller */
  for(idx2MappedTxBufferCfg = CANIF_CFG_CTRL_STARTIDX2MAPPEDTXBUFFERS(ControllerId); idx2MappedTxBufferCfg < CANIF_CFG_CTRL_STOPIDX2MAPPEDTXBUFFERS(ControllerId); idx2MappedTxBufferCfg++ ) /* FETA_CANIF_1 */
  {
    const CanIf_AnyIdxType hth = CANIF_CFG_MAPPEDTXBUFFERS(idx2MappedTxBufferCfg); /* hth to access CanIf_MailBoxConfig[] */
    const CanIf_AnyIdxType txBufferCfgIdx = CANIF_CFG_MAILBOX_TXBUFFERIDX(hth);

    /* #110 Clear the Tx buffer dependent on its Tx buffer handling type */
    switch (CANIF_CFG_MAILBOX_TXBUFFERHANDLINGTYPE(hth))
    {
# if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
      case CANIF_TXBUFFER_HANDLINGTYPE_PRIOBYCANID:
        {
          CanIf_AnyIdxType TxQueueFlagsIdx; /* for-loop counter */
          const CanIf_AnyIdxType txBufferBaseIdx = CANIF_CFG_TXBUFFERPRIOBYCANID_BUFFERBASEIDX(txBufferCfgIdx);

          /* #200 Clearing of PRIO_BY_CANID */
          for( TxQueueFlagsIdx = CANIF_CFG_TXBUFFERPRIOBYCANID_QUEUESTARTIDX(txBufferCfgIdx); TxQueueFlagsIdx < CANIF_CFG_TXBUFFERPRIOBYCANID_QUEUESTOPIDX(txBufferCfgIdx); TxQueueFlagsIdx++) /* FETA_CANIF_6 */
          {
            CANIF_VAR_SET_TXBUFFERPRIOBYCANID_QUEUEFLAGS(TxQueueFlagsIdx, 0); /* SBSW_CANIF_26 */
          }

          /* #300  Set QueueCounter to 0 */
          CanIf_SetTxQueueCounter(txBufferBaseIdx, 0); /* SBSW_CANIF_27 */
        }
      break;
# endif

# if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
      case CANIF_TXBUFFER_HANDLINGTYPE_FIFO:
        {
          /* #400 Clearing of FIFO */
          const CanIf_AnyIdxType txBufferFifoBaseIdx = CANIF_CFG_TXBUFFERFIFO_BUFFERBASEIDX((txBufferCfgIdx));

          /* #410 In case of CANIF_CANCEL_SUPPORT_API == STD_ON and module is just initialized, re-set all indirections in CanIf_TxQueueIndex2FifoEl and the flags for TxFifoQueueBase */
#  if (CANIF_CANCEL_SUPPORT_API == STD_ON)
          if(CanIf_GetCtrlMode(ControllerId) != CANIF_CS_UNINIT)
          {
            CanIf_TxBufferSizeType readIdx = CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx);
            CanIf_TxBufferSizeType counter = CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eQueueCounter);
            const CanIf_AnyIdxType txFifoQueueBaseStartIdx = CANIF_CFG_TXBUFFERFIFO_QUEUEBASESTARTIDX(txBufferCfgIdx);
        
            while(counter > 0u) /* FETA_CANIF_2 */
            {
              CANIF_VAR_SET_TXBUFFERFIFO_QUEUEIDX2FIFOEL(CANIF_CFG_TX_QUEUEINDEX(CANIF_VAR_TXBUFFERFIFO_QUEUEBASE((readIdx + txFifoQueueBaseStartIdx), eTxPduId)), CANIF_NO_ENTRY_IN_TX_FIFO); /* SBSW_CANIF_35 */
              CANIF_VAR_TXBUFFERFIFO_QUEUEBASE((readIdx + txFifoQueueBaseStartIdx), eQueued) = 0; /* SBSW_CANIF_34 */

              readIdx++;
              if(readIdx == CANIF_CFG_TXBUFFERFIFO_QUEUESIZE(txBufferCfgIdx))
              {
                readIdx = 0;
              }
              counter--;
            }
          }
#  endif

          /* #420 Set ReadIdx, WriteIdx and QueueCounter to 0 */
          CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eWriteIdx) = 0;     /* SBSW_CANIF_33 */
          CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx) = 0;      /* SBSW_CANIF_33 */
          CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eQueueCounter) = 0; /* SBSW_CANIF_33 */
        }
        break;
# endif
      default: /* COV_CANIF_GENDATA_FAILURE */
        break;
    }
  }
  CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_1);
}
#endif /* ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)) */


#if (CANIF_DEV_ERROR_DETECT == STD_ON)
# if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
/**********************************************************************************************************************
 *  CanIf_InitSubCheckJ1939AddrTables()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_InitSubCheckJ1939AddrTables
Relation_Context:
CanIfDevErrorDetect, CanIfJ1939 # from this local function #
CanIfDevErrorDetect, CanIfJ1939 # from caller CanIf_Init #
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_InitSubCheckJ1939AddrTables(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_OK;
  uint8 j1939CtrlCount = 0;
  uint8 controllerId;
  uint16 maxAddrOffset = 0;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Run through all CAN controllers */
  for (controllerId = 0u; controllerId < CANIF_CFG_MAX_CONTROLLER; controllerId++)
  {
    if(CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(controllerId) != CANIF_J1939_DYN_ADDR_DISABLED ) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
    {
      /* #20 Count the number of CAN controllers with activated J1939 support */
      j1939CtrlCount++;

      /* #30 Get the biggest address table offset */
      if(CANIF_CFG_CTRLID2J1939DYNADDROFFSET(controllerId) > maxAddrOffset) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
      {
        maxAddrOffset = (uint16)CANIF_CFG_CTRLID2J1939DYNADDROFFSET(controllerId);
      }
    }
  }

  /* J1939 support can be enabled also when no current CAN controller has it activated */
  if(j1939CtrlCount > 0u) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    /* #100 Check that the address tables are big enough */
    if ( ((CANIF_J1939_CTRL_ADDR_TBL_SIZE * (CanIf_SizeOfJ1939TxAddr_LookupType)j1939CtrlCount) > CanIf_GetSizeOfJ1939TxAddr_Lookup()) || /* PRQA S 2992, 2996 */ /* MD_CanIf_J1939ConstData */
         ((CANIF_J1939_CTRL_ADDR_TBL_SIZE * (CanIf_SizeOfJ1939RxAddr_LookupType)j1939CtrlCount) > CanIf_GetSizeOfJ1939RxAddr_Lookup()) || /* PRQA S 2996 */ /* MD_CanIf_J1939ConstData */
         ((CANIF_J1939_CTRL_ADDR_TBL_SIZE + (CanIf_SizeOfJ1939TxAddr_LookupType)maxAddrOffset) > CanIf_GetSizeOfJ1939TxAddr_Lookup()) ||
         ((CANIF_J1939_CTRL_ADDR_TBL_SIZE + (CanIf_SizeOfJ1939RxAddr_LookupType)maxAddrOffset) > CanIf_GetSizeOfJ1939RxAddr_Lookup()) /* PRQA S 2996 */ /* MD_CanIf_J1939ConstData */
        ) /* COV_CANIF_GENDATA_FAILURE */
    {
      retval = E_NOT_OK; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
  }

  return retval;
}
# endif /* CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED */
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */


#if (CANIF_SETDYNAMICRXID_API == STD_ON)
/***********************************************************************************************************************
 *  CanIf_InitSubSetDynamicRxId()
 **********************************************************************************************************************/
 /*
|<DataModelStart>| CanIf_InitSubSetDynamicRxId
Relation_Context:
CanIfDynRx # from this local function #
CanIfDynRx # from caller CanIf_Init #
Relation:
CanIfVariant
CanIfDevErrorDetect
CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
void CanIf_InitSubSetDynamicRxId(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  const CanIf_SetDynamicRxIdFctType Appl_CanIfSetDynamicRxId = CanIf_GetSetDynamicRxIdFctPtr();

  /* ----- Implementation ----------------------------------------------- */

  /* #100 Iterate over all Rx PDUs */
# if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
  CanIf_SizeOfUlRxPduId2InternalRxPduIdType rxPduExtId; /* Same data type as for CANIF_CFG_MAX_ULRXPDUS to ensure termination of for loop */
  for(rxPduExtId = 0; rxPduExtId < CANIF_CFG_MAX_ULRXPDUS; rxPduExtId++)
  {
    if(CanIf_IsRxPduConfigUsedOfUlRxPduId2InternalRxPduId(rxPduExtId))
    {
      PduIdType rxPduId = (PduIdType)CANIF_CFG_EXTRXPDUID2INTRXPDUID(rxPduExtId); /* PduIdType does always correspond to the COM Stack configuration, therefore the cast is always valid. */
# else
  CanIf_SizeOfRxPduConfigType rxPduId; /* Same data type as for CANIF_CFG_MAX_RXPDUS to ensure termination of for loop */
  for(rxPduId = 0; rxPduId < CANIF_CFG_MAX_RXPDUS; rxPduId++)
  {
# endif
      /* #110 If Rx PDU is of type dynamic */
      if(CanIf_IsDynRxIdUsedOfRxPduConfig(rxPduId))
      {
        Can_IdType canId = 0;

        /* #120 Call application callout to get CAN ID */
# if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
        Appl_CanIfSetDynamicRxId((PduIdType)rxPduExtId, &canId); /* SBSW_CANIF_57 */ /* PduIdType does always correspond to the COM Stack configuration, therefore the cast is always valid. */
# else
        Appl_CanIfSetDynamicRxId(rxPduId, &canId); /* SBSW_CANIF_57 */
# endif
        /* #130 Set CAN ID for dynamic Rx PDU */
        CanIf_SetDynRxId(CanIf_GetDynRxIdIdxOfRxPduConfig(rxPduId), canId); /* SBSW_CANIF_58 */

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
        /* #140 Check if CAN ID is in configured CAN ID range (only if DET is enabled) */
        /* A dynamic Rx PDU with a set CAN ID out of the configured CAN ID range will never be received */
        if ((canId < CANIF_CFG_RX_CANID(rxPduId)) || (canId > ((CANIF_CFG_RX_MASK(rxPduId) & CANIF_STOP_RANGE_FLAG_MASKOUT))))
        {
          errorId = CANIF_E_PARAM_CANID;
        }
# endif
      }
# if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */
  {
    CanIf_Det_ReportError(CANIF_INIT_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif /* CANIF_SETDYNAMICRXID_API == STD_ON */


/* \trace SPEC-1829 */
/***********************************************************************************************************************
 *  CanIf_Init()
 **********************************************************************************************************************/
 /*
|<DataModelStart>| CanIf_Init
Relation_Context:
Relation:
CanIfVariant, CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfIDType
CanIfDevErrorDetect, CanIfMultipleDriver
CanIfDevErrorDetect, CanIfJ1939
CanIfDynTx
CanIfDynRx
CanIfTxBuffer
CanIfPnEnable
CanIfTxConfPollSupport
CanIfJ1939
CanIfBusMirroringSupport
CanIfTxBuffer, CanIfTransmitCancellationAPI
CanIfRxPduReceptionMode
CanIfVariant, CanIfDevErrorDetect
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_Init(P2CONST(CanIf_ConfigType, AUTOMATIC, CANIF_XCFG) ConfigPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 controllerId;
  uint8 errorId = CANIF_E_NO_ERROR;

  /* \trace SPEC-1966 automatically supported due to only one valid configuration supported */
  /* Store the initialization pointer */
#if (CANIF_USE_INIT_POINTER == STD_ON)
  CanIf_ConfigDataPtr = ConfigPtr;

  /* ----- Development Error Checks --------------------------------------- */
# if ((CANIF_DEV_ERROR_DETECT == STD_ON ) || (CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME))
  /* #10 Check if parameter ConfigPtr is valid */
  if (CanIf_ConfigDataPtr == NULL_PTR)
  {
#  if (CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME)
    EcuM_BswErrorHook((uint16) CANIF_MODULE_ID, (uint8) ECUM_BSWERROR_NULLPTR);
#  else
    errorId = CANIF_E_PARAM_POINTER;
#  endif
    }
    else
# endif
# if (CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME)
  /* UREQ00035482 */
  /* #20 Check generator version */
  if(CANIF_CFG_GENERATORCOMPATIBILITYVERSION != CANIF_GENDATA_COMPATIBILITY_VERSION)
  {
    EcuM_BswErrorHook((uint16) CANIF_MODULE_ID, (uint8) ECUM_BSWERROR_COMPATIBILITYVERSION);
  }
  /* #30 Check the magic number */
  else if (CANIF_CFG_MAGICNUMBER != CANIF_MAGIC_NUMBER)
  {
    /* UREQ00035483 */
    EcuM_BswErrorHook((uint16) CANIF_MODULE_ID, (uint8) ECUM_BSWERROR_MAGICNUMBER);
  }
  else
# endif
#else
  CANIF_DUMMY_STATEMENT(ConfigPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

#if (CANIF_DEV_ERROR_DETECT == STD_ON)
  /* #40 Check the size of type Can_IdType */
# if (CANIF_EXTENDEDID_SUPPORT != STD_ON)
  if (sizeof (Can_IdType) != 2u) /* PRQA S 2742 */ /* MD_CanIf_Rule14.3_GenData */
  {
    errorId = CANIF_E_INVALID_CANIDTYPESIZE; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  if (sizeof (Can_IdType) != 4u) /* PRQA S 2742 */ /* MD_CanIf_Rule14.3_GenData */
  {
    errorId = CANIF_E_INVALID_CANIDTYPESIZE; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# endif
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
  else
  /*  ESCAN00092230 FEAT-2004: Release of FEAT-912 (Support of several CAN Drivers in one ECU configuration) */
  if (CanIf_GetSizeOfCanControllerIdUpToLowMap() < CANIF_CFG_MAX_CONTROLLER) /* PRQA S 2742 */ /* MD_CanIf_Rule14.3_GenData */ /* COV_CANIF_GENDATA_FAILURE */
  {
    errorId = CANIF_E_FATAL; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# endif

# if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
  /* #50 Check the J1939 address tables configuration */
  else if(CanIf_InitSubCheckJ1939AddrTables() == E_NOT_OK) /* CM_CANIF_2 */ /* PRQA S 2992, 2996 */ /* MD_CanIf_J1939ConstData */ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    errorId = CANIF_E_FATAL;
  }
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Initialize all dynamic Tx-PDUs */
#if (CANIF_SETDYNAMICTXID_API == STD_ON)
    PduIdType DynamicPduId;
    CanIf_SizeOfTxPduConfigType CanPduId; /* Same data type as for CANIF_CFG_MAX_TXPDUS to ensure termination of for loop */
    for(CanPduId = 0; CanPduId < CANIF_CFG_MAX_TXPDUS; CanPduId++)
    {
      DynamicPduId = CANIF_CFG_STAT2DYN_INDIRECTION(CanPduId);
      if(DynamicPduId < CanIf_GetSizeOfDynTxId())
      {
        CANIF_VAR_DYNTXID(DynamicPduId) = CANIF_CFG_TX_CANID(CanPduId); /* SBSW_CANIF_14 */
      }
    }
#endif

  /* #105 Initialize all dynamic Rx PDUs */
#if (CANIF_SETDYNAMICRXID_API == STD_ON)
  CanIf_InitSubSetDynamicRxId();
#endif

    /* #110 Run through all CAN controllers and initialize them */
    for (controllerId = 0u; controllerId < CANIF_CFG_MAX_CONTROLLER; controllerId++)
    {
#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
      /* \trace SPEC-3665 */
      /* #120 Initialize the Tx-queue */
      /* ESCAN00087530 after CanIf_ClearQueue() in state CANIF_CS_UNINIT, WriteIdx, ReadIdx and QueueCounter are initialized */
      CanIf_ClearQueue(controllerId);
#endif
      /* \trace SPEC-3750, SPEC-3755, SPEC-3752 */
      /* #130 Initialize controller mode */
      CanIf_SetCtrlMode(controllerId, CANIF_CS_STOPPED); /* SBSW_CANIF_3 */
      /* #140 Initialize PDU mode */
      CANIF_VAR_SETPDUMODE(controllerId, CANIF_GET_OFFLINE); /* SBSW_CANIF_3 */
#if (CANIF_WAKEUP_VALIDATION == STD_ON)
      /* \trace SPEC-3754 */
      /* #150 Initialize wake-up validation state */
      CanIf_SetWakeUpValidationState(controllerId, CANIF_WUVALIDATION_ARMED); /* SBSW_CANIF_3 */
#endif
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
      /* #160 Initialize the Tx-filter state */
      CanIf_SetPnWuTxPduFilterActive(controllerId, FALSE); /* SBSW_CANIF_3 */
#endif

#if (CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT == STD_ON)
      /* #170 Initialize the Tx-confirmation state */
      CanIf_SetTxConfState(controllerId, CANIF_NO_NOTIFICATION); /* SBSW_CANIF_3 */
#endif

#if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
      {
        uint8 index;
        
        /* #180 Initialize the dynamic address lookup table (only for J1939Nm controller) */
        if(CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(controllerId) != CANIF_J1939_DYN_ADDR_DISABLED ) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
        {
          for(index = 0; index < (CANIF_J1939_CTRL_ADDR_TBL_SIZE - 1u); index++) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
          {
            CANIF_VAR_SET_J1939RXADDR_LOOKUP(controllerId, index, CANIF_J1939_NULL_ADDR); /* SBSW_CANIF_17 */
            CANIF_VAR_SET_J1939TXADDR_LOOKUP(controllerId, index, CANIF_J1939_NULL_ADDR); /* SBSW_CANIF_17 */
          }
          CANIF_VAR_SET_J1939RXADDR_LOOKUP(controllerId, (CANIF_J1939_CTRL_ADDR_TBL_SIZE - 1u), CANIF_J1939_BROADCAST_ADDR); /* SBSW_CANIF_17 */
          CANIF_VAR_SET_J1939TXADDR_LOOKUP(controllerId, (CANIF_J1939_CTRL_ADDR_TBL_SIZE - 1u), CANIF_J1939_BROADCAST_ADDR); /* SBSW_CANIF_17 */
        }
      }
#endif /* CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED */

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
      /* #190 Initialize mirroring to disabled */
      CanIf_ChangeMirroring(controllerId, FALSE); /* SBSW_CANIF_3 */
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */
    } /* FOR-loop: Run through all controllers */
#if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
# if (CANIF_CANCEL_SUPPORT_API == STD_ON) 
    /* #200 Initialize SW-cancellation part of FIFO */
    CanIf_InitSubInitFifoQueue(); /* PRQA S 2987 */ /* MD_CanIf_FctCall */
# endif
#endif
#if (CANIF_SET_PDU_RECEPTION_MODE_SUPPORT == STD_ON)
    {
      CanIf_PduRxModeIterType index;
      /* #300 Set the reception mode to default value */
      for(index = 0; index < CanIf_GetSizeOfPduRxMode(); index++)
      {
        CANIF_VAR_SET_PDU_RECEPTION_MODE(index, CANIF_RMT_RECEIVE_STOP); /* SBSW_CANIF_23 */
      }
    }
#endif
#if ((CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON))
    CanIf_SystemInit = TRUE; /* ESCAN00083271 */
#endif
  }
  /* ----- Development Error Report --------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    CanIf_Det_ReportError(CANIF_INIT_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}


/***********************************************************************************************************************
 *  CanIf_CallCanSetCtrlMode()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_CallCanSetCtrlMode
Relation_Context:
Relation:
CanIfSupportCanDrvEqcAr431, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr431
CanIfSupportCanDrvEqcAr431, CanIfMultipleDriver
CanIfSupportCanDrvEqcAr431, CanIfMultipleDriver, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr431, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421, CanIfSupportCanDrvEqcMsrc, CanIfMultipleDriver
CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421, CanIfSupportCanDrvEqcMsrc, CanIfMultipleDriver, CanIfSupportCanDrvEqcAr431
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
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
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_CallCanSetCtrlMode(uint8 ControllerId, CanIf_CanCtrlModeTransType canCtrlModeTrans)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ----------------------------------------------- */
#if ((CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON))
  /* #100 Determine EQC of responsible CAN Driver (if different CAN Driver EQCs are used) */
  if(CanIf_GetCanDrvEqcArVersionOfCanControllerIdUpToLowMap(ControllerId) == CAN_DRV_EQC_AR_431) /* PRQA S 2742 */ /* MD_CanIf_CanDrvEqcConstData */ /* COV_CANIF_CANDRV_EQC */
#endif

#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
  {
    /* #110 For CAN_DRV_EQC_AR_431
            Determine responsible CAN Driver Can_<Infix>_SetControllerMode function (if multiple CAN Drivers are used)
            Call Can_<Infix>_SetControllerMode with CAN Driver specific ControllerId and CAN controller mode
            (Return value is according to Std_ReturnType)
    */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#  if (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON)
    /* Combination with CAN Driver of EQC MSRC - so function pointer have to be casted */
    const CanIf_Can_SetControllerModeAr431FctType canSetCtrlModeFct = CANIF_GET_CANSETCTRLMODEFCT(ControllerId).CanIf_Can_SetControllerModeAr431Fct; /* PRQA S 2880 */ /* MD_CanIf_CanDrvEqcUnreachableCode */
#  else
    const CanIf_Can_SetControllerModeFctType canSetCtrlModeFct = CANIF_GET_CANSETCTRLMODEFCT(ControllerId);
#  endif
# endif /* CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON */

    /* canSetCtrlModeFct() is defined to Can_<Infix>_SetControllerMode and CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER() is defined to ControllerId,
       if only a single CAN Driver is used */
    retval = canSetCtrlModeFct((uint8)CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId), CanIf_GetCanCtrlModeTransAr431(canCtrlModeTrans)); /* SBSW_CANIF_52 */

  }
#endif /* CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON */

#if ((CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON))
  else
#endif

#if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421_OR_MSRC == STD_ON)
  {
    /* #120 For CAN_DRV_EQC_AR_403, CAN_DRV_EQC_AR_421 or CAN_DRV_EQC_MSRC
            Determine responsible CAN Driver Can_<Infix>_SetControllerMode function (if multiple CAN Drivers are used)
            Call Can_<Infix>_SetControllerMode with CAN Driver specific ControllerId and CAN controller mode
            Covert return value to Std_ReturnType
    */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#  if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
    /* Combination with CAN Driver of EQC AR 4.3.1 - so function pointer have to be casted */
    const CanIf_Can_SetControllerModeAr403FctType canSetCtrlModeFct = CANIF_GET_CANSETCTRLMODEFCT(ControllerId).CanIf_Can_SetControllerModeAr403Fct;
#  else
    const CanIf_Can_SetControllerModeFctType canSetCtrlModeFct = CANIF_GET_CANSETCTRLMODEFCT(ControllerId);
#  endif
# endif /* CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON */

    /* canSetCtrlModeFct() is defined to Can_<Infix>_SetControllerMode and CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER() is defined to ControllerId,
       if only a single CAN Driver is used */
    if(canSetCtrlModeFct((uint8)CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId), CanIf_GetCanCtrlModeTransAr403(canCtrlModeTrans)) == CAN_OK) /* SBSW_CANIF_52 */
    {
      retval = E_OK;
    }
  }
#endif /* CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421_OR_MSRC == STD_ON */

  return retval;
}


/***********************************************************************************************************************
 *  CanIf_SetControllerModeSubSwitch()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_SetControllerModeSubSwitch
Relation_Context:
Relation:
CanIfTxBuffer
CanIfWakeupValidation
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
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
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllerModeSubSwitch(uint8 ControllerId, CanIf_ControllerModeType ControllerMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Do the switch based on the desired target controller mode */
  switch (ControllerMode)
  {
    case CANIF_CS_STOPPED:
      /* #200 Transition to controller mode STOPPED: Transition possible from any controller mode */
      /* \trace SPEC-3763 */
      switch(CanIf_GetCtrlMode(ControllerId))
      {
        case CANIF_CS_SLEEP:
          /* #210 Request CAN controller to controller mode WAKEUP if currently in controller mode SLEEP */
          /* CAN Controller transition to WAKEUP and STOPPED required if wakeup is initiated by application */
          /* \trace SPEC-1845, SPEC-1996 */
          retval = CanIf_CallCanSetCtrlMode(ControllerId, CANIF_CANCTRL_WAKEUP);
          break;

        case CANIF_CS_STARTED:
          /* \trace SPEC-5124 */
          /* \trace SPEC-1845, SPEC-1996 */
          /* #220 Request CAN controller to controller mode STOPPED if currently in controller mode STARTED */
          if (CanIf_CallCanSetCtrlMode(ControllerId, CANIF_CANCTRL_TO_STOP) == E_OK)
          {
            /* \trace SPEC-45263 */
            /* #230 Deactivate tx communication of the affected CAN controller */
            /* Explicitly not done over the CanIf_SetPduMode() API, because controller mode change can be synchronous and then the controller mode is set to CANIF_CS_STOPPED 
               before CanIf_SetPduMode() (requires CANIF_CS_STARTED) is called */
            CANIF_VAR_SETPDUMODE(ControllerId, CANIF_GET_RX_ONLINE); /* SBSW_CANIF_3 */

#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
            /* \trace SPEC-3764 */
            /*#240 Clear the Tx-Buffers of the affected CAN controller */
            CanIf_ClearQueue(ControllerId);
#endif
            retval = E_OK;
          }
          break;

        /* \trace SPEC-3753 */
        case CANIF_CS_STOPPED:
          /* \trace SPEC-1845, SPEC-1996 */
          /* #250 Request CAN controller to controller mode STOPPED if currently in controller mode STOPPED */
          retval = CanIf_CallCanSetCtrlMode(ControllerId, CANIF_CANCTRL_TO_STOP);
          break;

        default:
          /* CAN Interface in invalid mode */
          /* retval already set to E_NOT_OK as default */
          break;
      }
      break;
    case CANIF_CS_STARTED:
      /* #300 Request CAN controller to controller mode STARTED if currently in controller mode STOPPED/STARTED */
      /* \trace SPEC-3757 */
      if((CanIf_GetCtrlMode(ControllerId) == CANIF_CS_STOPPED) || (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_STARTED))
      {
          /* \trace SPEC-1845, SPEC-1996 */
          retval = CanIf_CallCanSetCtrlMode(ControllerId, CANIF_CANCTRL_TO_START);
      } /* else transition not allowed; retval already set to E_NOT_OK as default */
      break;

    case CANIF_CS_SLEEP:
      /* #400 Transition to controller mode SLEEP: Transition only possible from controller mode STOPPED/SLEEP */
      /* CanIf_ControllerMode is now CANIF_CS_STOPPED unless an error occurred -> E_NOT_OK still set in case of incomplete transition */
      /* \trace SPEC-3759 */

      switch(CanIf_GetCtrlMode(ControllerId))
      {
        case CANIF_CS_STOPPED:
          /* \trace SPEC-1845, SPEC-1996 */
          /* #410 Request CAN controller to controller mode SLEEP if currently in controller mode STOPPED */
          if (CanIf_CallCanSetCtrlMode(ControllerId, CANIF_CANCTRL_TO_SLEEP) == E_OK)
          {
#if (CANIF_WAKEUP_VALIDATION == STD_ON)
            /* ESCAN00079059 Reset of stored wake-up validation event moved to this place */
            /* CANIF756 */
            /* #420 Reset the wake-up validation state of the affected CAN controller */
            CanIf_SetWakeUpValidationState(ControllerId, CANIF_WUVALIDATION_ARMED); /* SBSW_CANIF_3 */
#endif
            retval = E_OK;
          }
          break;

        case CANIF_CS_SLEEP:
          /* #430 Request CAN controller to controller mode SLEEP if currently in controller mode SLEEP */
          retval = CanIf_CallCanSetCtrlMode(ControllerId, CANIF_CANCTRL_TO_SLEEP);
          break;

        default:
          /* Transition not allowed */
          /* E_NOT_OK already set */
          break;
      }
      break;
    default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
      /* \trace SPEC-21340 */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
      CanIf_Det_ReportError(CANIF_SETCONTROLLERMODE_API, CANIF_E_PARAM_CTRLMODE);
#endif
      break;
    }
    return retval;
}


/* \trace SPEC-1830 */
/***********************************************************************************************************************
 *  CanIf_SetControllerMode()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_SetControllerMode
Relation_Context:
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllerMode(uint8 ControllerId, CanIf_ControllerModeType ControllerMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* \trace SPEC-3751 */
  Std_ReturnType retval = E_NOT_OK;
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
# if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    /* \trace SPEC-1997 */
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
# if ((CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON) && (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
  /* #40 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(ControllerId))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
# endif
  else
#endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* return E_NOT_OK if controller is in state CANIF_CS_UNINIT */

#if (CANIF_DEV_ERROR_DETECT == STD_OFF)
    /* #100 Do mode transition only if CAN controller is initialized */
    if (CanIf_GetCtrlMode(ControllerId) != CANIF_CS_UNINIT)
#endif
    {
      /* return E_NOT_OK if requested device mode already active */
      /* #110 Perform mode transition only within critical section */
      CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_0);
      retval = CanIf_SetControllerModeSubSwitch(ControllerId, ControllerMode);
      CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_0);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_SETCONTROLLERMODE_API, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  /* \trace SPEC-3751 */
  return retval;
}


/* \trace SPEC-1916 */
/***********************************************************************************************************************
 *  CanIf_GetControllerMode()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_GetControllerMode
Relation_Context:
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetControllerMode(uint8 ControllerId, P2VAR(CanIf_ControllerModeType, AUTOMATIC, CANIF_APPL_STATE_VAR) ControllerModePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
# if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    /* \trace SPEC-1998 */
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  /* #40 Check if parameter ControllerModePtr is valid */
  else if (ControllerModePtr == NULL_PTR)
  {
    /* \trace SPEC-5232 */
    errorId = CANIF_E_PARAM_POINTER;
  }
  else
#endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) */

  /* ----- Implementation ----------------------------------------------- */
  {
    /* \trace SPEC-5065 */
    /* #100 Retrieve the actual mode */
    *ControllerModePtr = CanIf_GetCtrlMode(ControllerId); /* SBSW_CANIF_4 */
    retval = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_GETCONTROLLERMODE_API, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retval;
}


/***********************************************************************************************************************
 *  CanIf_TransmitSubVerifyPduLength()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TransmitSubVerifyPduLength
Relation_Context:
Relation:
CanIfTxMetaDataSupport
CanIfTxMetaDataSupport,CanIfTxPduTruncation
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 */
CANIF_LOCAL_INLINE FUNC(CanIf_TransmitStateType, CANIF_CODE) CanIf_TransmitSubVerifyPduLength(PduIdType CanTxPduId, P2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) pduLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  CanIf_TransmitStateType txState = CANIF_DO_TRANSMIT;

  /* ----- Implementation ----------------------------------------------- */
#if (CANIF_META_DATA_TX_SUPPORT == STD_ON)
  /* #100 Remove tx meta data length from PDU length (only if configured) */
  if (*pduLengthPtr >= CANIF_CFG_TX_TXMETADATALENGTH(CanTxPduId)) /* Not applicable if the PDU length is too short */
  {
    *pduLengthPtr = (PduLengthType)(*pduLengthPtr - CANIF_CFG_TX_TXMETADATALENGTH(CanTxPduId)); /* SBSW_CANIF_9 */
  }
#endif /* CANIF_META_DATA_TX_SUPPORT == STD_ON */

  /* #110 Verify if PDU length exceeding the configured maximum length */
  if(*pduLengthPtr > CANIF_CFG_TX_LENGTH(CanTxPduId))
  {
    /* #200 If TxPduTruncation is enabled, truncate PDU length to the configured maximum length */
    if(CANIF_CFG_TX_ISPDUTRUNCATION(CanTxPduId))
    { 
      /* \trace SPEC-7204475 */
      *pduLengthPtr = CANIF_CFG_TX_LENGTH(CanTxPduId); /* SBSW_CANIF_9 */
    }
    /* #300 If TxPduTruncation is disabled, prevent transmission */
    else
    { 
      /* \trace SPEC-7204476 */
      txState = CANIF_DO_NOT_TRANSMIT;
    }
  }

  return txState;
}


#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
/***********************************************************************************************************************
 *  CanIf_TransmitSubHandleTxPduFilter()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TransmitSubHandleTxPduFilter
Relation_Context:
CanIfPnEnable # from this local function #
CanIfPnEnable # from caller CanIf_Transmit #
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
 *
 *
 */
CANIF_LOCAL_INLINE FUNC(CanIf_TransmitStateType, CANIF_CODE) CanIf_TransmitSubHandleTxPduFilter(uint8 ControllerId, CONST(PduIdType, AUTOMATIC) CanTxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  CanIf_TransmitStateType pnTxState = CANIF_DO_TRANSMIT;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Determine basing on state of Tx-PDU filter of corresponding CAN controller whether the Tx-PDU may be transmitted or NOT */
  if (CANIF_CFG_ISFEATUREPNWUTXPDUFILTERENABLED(ControllerId)) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    /* CANIF750 */
    if (CanIf_IsPnWuTxPduFilterActive(ControllerId))
    {
      if (!CANIF_CFG_ISPNWUTXPDU(CanTxPduId))
      {
        /* TxPdu does not pass the PnWuFilter -> transmission not allowed */
        pnTxState = CANIF_DO_NOT_TRANSMIT;
      }
    }
  }

  return pnTxState;
}
#endif /* (CANIF_PN_WU_TX_PDU_FILTER == STD_ON) */


#if (CANIF_META_DATA_TX_SUPPORT == STD_ON)
/***********************************************************************************************************************
 *  CanIf_TransmitSubHandleMetaData()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TransmitSubHandleMetaData
Relation_Context:
CanIfTxMetaDataSupport # from this local function #
CanIfTxMetaDataSupport # from caller CanIf_Transmit #
Relation:
CanIfIDType, CanIfFdSupport # used because TxIdMask value is depending of CanIfIDType/Fd #
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 */
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_TransmitSubHandleMetaData(P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_MSG_VAR) PduInfoPtr, P2VAR(Can_PduType, AUTOMATIC, AUTOMATIC) localPduPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 startPos;
  uint8 endPos;
  Can_IdType localId;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Tx-PDU has meta data configured? */
  if(CANIF_CFG_TX_TXMETADATALENGTH(localPduPtr->swPduHandle) > 0u)
  {
    localId = 0;
    endPos = (uint8)(PduInfoPtr->SduLength); /* ESCAN00074166 */

    /* #200 Extract the meta data from the payload according to configuration */
    if (PduInfoPtr->SduLength >= CANIF_CFG_TX_TXMETADATALENGTH(localPduPtr->swPduHandle)) /* Not applicable if the TxPduLenght (SduLength) is too short */
    {
      startPos = (uint8)((PduInfoPtr->SduLength) - 1u); /* DLC - 1 */
      endPos = (uint8)(endPos - CANIF_CFG_TX_TXMETADATALENGTH(localPduPtr->swPduHandle)); /* TxPduLenght (SduLength) - MetaDataLength */

      /* #210 Calculate the CAN identifier from extracted meta data */ 
      while(startPos > endPos) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */ /* FETA_CANIF_7 */
      {
        localId |= PduInfoPtr->SduDataPtr[startPos];
        localId = localId << 8;
        startPos--;
      }
      localId |= PduInfoPtr->SduDataPtr[startPos];

      /* #220 Adapt the CAN identifier of Tx-PDU according to calculated one from meta data */
      localPduPtr->id = (Can_IdType)(((localPduPtr->id & CANIF_CFG_TX_CANIDTXMASK(localPduPtr->swPduHandle)) | (localId & (Can_IdType)(~CANIF_CFG_TX_CANIDTXMASK(localPduPtr->swPduHandle))))); /* SBSW_CANIF_9 */
    }
  }
}
#endif /* (CANIF_META_DATA_TX_SUPPORT == STD_ON) */


#if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
/***********************************************************************************************************************
 *  CanIf_TransmitSubJ1939DynAddr()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TransmitSubJ1939DynAddr
Relation_Context:
CanIfJ1939 # from this local function #
CanIfJ1939 # from caller CanIf_Transmit #
Relation:
CanIfJ1939
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
 /* PRQA S 6030, 6080 1 */ /*  MD_MSR_STCYC, MD_MSR_STMIF */
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_TransmitSubJ1939DynAddr(uint8 ControllerId, P2VAR(Can_PduType, AUTOMATIC, AUTOMATIC)localPduPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Do J1939-conform address conversion only for J1939Nm CAN controller */
  if( CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(ControllerId) != CANIF_J1939_DYN_ADDR_DISABLED ) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    /* #110 Distinguish between standard and extended CAN identifier */
    if (((localPduPtr)->id & CANIF_EXT_CANID_MASK) == CANIF_EXT_CANID_MASK) /* ext. ID */ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
    {
      /* #200 Extended CAN identifier: */
      if ((((localPduPtr)->id & CANIF_J1939_DATA_PAGE_MASK) != CANIF_J1939_DATA_PAGE_MASK) && (((localPduPtr)->id & CANIF_J1939_PGN_MASK) != CANIF_J1939_AC_PGN)) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
      {
        /* #210 Destination and source addresses are substituted with J1939 format but not for address claimed */
        /* Consistency of J1939 address translation is secured since CanIf_TransmitSubJ1939DynAddr() is called within CANIF_EXCLUSIVE_AREA_4. */
        if (((localPduPtr)->id & CANIF_J1939_MIN_PDU2_MASK) < CANIF_J1939_MIN_PDU2_MASK) /* PDU Format 1 (second byte < 0xF0) */ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
        {
          uint8 da = (uint8)(((localPduPtr)->id & CANIF_J1939_DA_MASK) >> 8); /* destination address */
          da = CANIF_VAR_J1939TXADDR_LOOKUP(ControllerId, da);

          if (da == CANIF_J1939_NULL_ADDR) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
          {
            errorId = CANIF_E_INVALID_DA;
            retVal = E_NOT_OK;
          }

          (localPduPtr)->id = (Can_IdType)(((localPduPtr)->id & CANIF_J1939_INV_DA_MASK) | (((Can_IdType)da) << 8u)); /* SBSW_CANIF_9 */
        }
        {
          uint8 sa = (uint8)((localPduPtr)->id & CANIF_J1939_SA_MASK); /* source address */
          uint8 sa_t = CANIF_VAR_J1939TXADDR_LOOKUP(ControllerId, sa); /* source address transformed with lookup table */

          if ((sa == CANIF_J1939_BROADCAST_ADDR) || ((sa != CANIF_J1939_NULL_ADDR) && (sa_t == CANIF_J1939_NULL_ADDR))) /* invalid source address */ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
          {
            errorId = CANIF_E_INVALID_SA;
            retVal = E_NOT_OK;
          }

          (localPduPtr)->id = (Can_IdType)(((localPduPtr)->id & CANIF_J1939_INV_SA_MASK) | sa_t); /* SBSW_CANIF_9 */
        }
      }
    }
#  if (CANIF_J1939_DYN_ADDR_SUPPORT == CANIF_J1939_DYN_ADDR_MIXED_CANID)
    else
    {
      /* #300 Standard CAN identifier: */
      if( CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(ControllerId) != CANIF_J1939_DYN_ADDR_EXT_CANID ) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
      {
        /* #310 Only source address is substituted */
        uint8 sa = (uint8)((localPduPtr)->id & CANIF_J1939_SA_MASK); /* source address */
        uint8 sa_t = CANIF_VAR_J1939TXADDR_LOOKUP(ControllerId, sa); /* source address transformed with lookup table */

        if ((sa == CANIF_J1939_BROADCAST_ADDR) || ((sa != CANIF_J1939_NULL_ADDR) && (sa_t == CANIF_J1939_NULL_ADDR))) /* invalid source address */
        {
          errorId = CANIF_E_INVALID_SA;
          retVal = E_NOT_OK;
        }
        (localPduPtr)->id = (Can_IdType)(((localPduPtr)->id & CANIF_J1939_INV_SA_MASK) | sa_t); /* SBSW_CANIF_9 */
      }
    }
#  endif /* CANIF_J1939_DYN_ADDR_SUPPORT == CANIF_J1939_DYN_ADDR_MIXED_CANID */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    CanIf_Det_ReportError(CANIF_TRANSMIT_API, errorId);
  }
#  else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif

  return retVal;
}
#endif /* CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED */


#if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
/***********************************************************************************************************************
 *  CanIf_TxBufferPrioByCanIdQueuePdu()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TxBufferPrioByCanIdQueuePdu
Relation_Context:
CanIfTxBuffer # from this local function #
CanIfTxBuffer # from caller CanIf_TransmitSubHandleTxResBusy, CanIf_CancelTxConfirmation #
Relation:
CanIfFdSupport # used for CANIF_STATIC_FD_TXQUEUE #
CanIfDynTx, CanIfJ1939, CanIfTxMetaDataSupport
CanIfTxBuffer
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
 *
 *
 *
 *
 */
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_TxBufferPrioByCanIdQueuePdu(P2CONST(Can_PduType, AUTOMATIC, CANIF_CBK_DRV_VAR) PduInfoPtr )
{
  /* ----- Local Variables ---------------------------------------------- */
  CanIf_AnyIdxType queueindex = CANIF_CFG_TX_QUEUEINDEX(PduInfoPtr->swPduHandle);

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Store the CAN identifier of the Tx PDU */
# if((CANIF_SETDYNAMICTXID_API == STD_ON) || (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED) || (CANIF_META_DATA_TX_SUPPORT == STD_ON))
  CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUE(queueindex, eBaseParams.eCanId) = PduInfoPtr->id; /* SBSW_CANIF_6 */
# endif
  /* #110 Store the data length of the Tx PDU */
  CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUE(queueindex, eBaseParams.eSduLength) = PduInfoPtr->length; /* SBSW_CANIF_6 */

  /* #120 Store the payload of the Tx PDU */
# if (CANIF_STATIC_FD_TXQUEUE == STD_ON)
  VStdLib_MemCpy(CANIF_ADDR_VAR_TXBUFFERPRIOBYCANID_STATFDQUEUE_QUEUEDATA(CANIF_CFG_TXBUFFERPRIOBYCANID_STATFDQUEUE_DATASTARTIDX(PduInfoPtr->swPduHandle)), PduInfoPtr->sdu, PduInfoPtr->length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CANIF_18 */
# else
  VStdLib_MemCpy(CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUE(queueindex, eSdu), PduInfoPtr->sdu, PduInfoPtr->length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CANIF_7 */
# endif /* CANIF_STATIC_FD_TXQUEUE == STD_ON */

}
#endif /* CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON */


#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
/***********************************************************************************************************************
 *  CanIf_TransmitSubHandleTxResBusy()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TransmitSubHandleTxResBusy
Relation_Context:
CanIfTxBuffer # from this local function #
CanIfTxBuffer # from caller CanIf_TransmitSubWrite #
Relation:
CanIfTxBuffer
CanIfDevErrorDetect
CanIfTxBuffer
CanIfTxBuffer, CanIfFdSupport # used for CANIF_STATIC_FD_TXQUEUE #
CanIfTxBuffer, CanIfDevErrorDetect
CanIfTxBuffer
CanIfTxBuffer, CanIfDynTx, CanIfJ1939, CanIfTxMetaDataSupport
CanIfTxBuffer, CanIfDevErrorDetect 
CanIfTxBuffer, CanIfTransmitCancellationAPI
CanIfTxBuffer, CanIfDevErrorDetect
CanIfDevErrorDetect 
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
 *
 *
 *
 *
 */
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
CANIF_LOCAL_INLINE FUNC (Std_ReturnType, CANIF_CODE) CanIf_TransmitSubHandleTxResBusy(P2CONST(Can_PduType, AUTOMATIC, AUTOMATIC) localPduPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  uint8 errorId = CANIF_E_NO_ERROR;
  CanIf_AnyIdxType hth = CANIF_CFG_TX_HTH(localPduPtr->swPduHandle);

# if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
  CanIf_TxBufferSizeType writeIdx;
  CanIf_AnyIdxType queueDataStartIdx;
  CanIf_AnyIdxType txBufferCfgIdx;
  CanIf_AnyIdxType txBufferFifoBaseIdx;
  CanIf_AnyIdxType txFifoQueueBaseStartIdx;
# endif

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Is TxPDU a BasicCAN-PDU and has it a buffer? */
  if (CANIF_CFG_MAILBOX_HASTXBUFFER(hth))
  {
    /* \trace SPEC-1233, SPEC-1232 */

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
    if ((localPduPtr->sdu == NULL_PTR) && (localPduPtr->length != 0u))
    {
      errorId = CANIF_E_PARAM_POINTER;
    }
    else
# endif
    {
      switch (CANIF_CFG_MAILBOX_TXBUFFERHANDLINGTYPE(hth))
      {
# if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
        /* #200 Do PrioByCan queue handling */
        case CANIF_TXBUFFER_HANDLINGTYPE_PRIOBYCANID:
          /* #210 PDU length fits */
#  if (CANIF_STATIC_FD_TXQUEUE == STD_ON)
          if(localPduPtr->length <= CANIF_CFG_TXBUFFERPRIOBYCANID_STATFDQUEUE_MAXDATALENGTH(localPduPtr->swPduHandle))
#  else
          if(localPduPtr->length <= CANIF_STATIC_BUFFER_SIZE)
#  endif
          {
            /* #220 Queue information */
            CanIf_TxBufferPrioByCanIdQueuePdu(localPduPtr); /* SBSW_CANIF_5 */

            /* #230 Set queue flag for current PDU and return E_OK */
            (void)CanIf_SetPrioByCanIdTxPduAsQueued(localPduPtr->swPduHandle);
            retval = E_OK;
          }
          else
          /* #240 PDU length does not fit */
          {
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
            errorId = CANIF_E_PARAM_DLC;
#  endif
            retval = E_NOT_OK;
          }
          break;
# endif
# if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
        /* #300 Do FIFO queue handling */
        case CANIF_TXBUFFER_HANDLINGTYPE_FIFO:
          txBufferCfgIdx = CANIF_CFG_MAILBOX_TXBUFFERIDX(hth);
          /* #310 Buffer element size big enough to record the data? */
          if(localPduPtr->length <= CANIF_CFG_TXBUFFERFIFO_SIZEOFONEPAYLOADEL(txBufferCfgIdx))
          {
            txBufferFifoBaseIdx = CANIF_CFG_TXBUFFERFIFO_BUFFERBASEIDX((txBufferCfgIdx));
            /* #400 One element of the FIFO free?*/
            if (CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eQueueCounter) < CANIF_CFG_TXBUFFERFIFO_QUEUESIZE(txBufferCfgIdx))
            {
              /* #410 Evaluate FIFO-WriteIdx */
              txFifoQueueBaseStartIdx = CANIF_CFG_TXBUFFERFIFO_QUEUEBASESTARTIDX(txBufferCfgIdx);
              writeIdx = CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eWriteIdx);
              /* #420 Store length of Tx-PDU at FIFO-WriteIdx */
              CANIF_VAR_TXBUFFERFIFO_QUEUEBASE((writeIdx + txFifoQueueBaseStartIdx), eBaseParams.eSduLength) = localPduPtr->length; /* SBSW_CANIF_34 */
#  if ((CANIF_SETDYNAMICTXID_API == STD_ON) || (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED) || (CANIF_META_DATA_TX_SUPPORT == STD_ON))
              /* #430 Store CanId of Tx-PDU at FIFO-WriteIdx */
              CANIF_VAR_TXBUFFERFIFO_QUEUEBASE((writeIdx + txFifoQueueBaseStartIdx), eBaseParams.eCanId) = localPduPtr->id; /* SBSW_CANIF_34 */
#  endif
              /* #440 Store the PDU ID of the Tx-PDU at FIFO-WriteIdx */
              CANIF_VAR_TXBUFFERFIFO_QUEUEBASE((writeIdx + txFifoQueueBaseStartIdx), eTxPduId) = localPduPtr->swPduHandle; /* SBSW_CANIF_34 */
              /* #450 Evaluate the startindex into the CanIf_TxFifoQueueData */
              queueDataStartIdx = ((uint32_least)writeIdx * (uint32_least)CANIF_CFG_TXBUFFERFIFO_SIZEOFONEPAYLOADEL(txBufferCfgIdx)) + CANIF_CFG_TXBUFFFERFIFO_QUEUEDATASTARTIDX(txBufferCfgIdx);
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
              /* #460 Check if the actual writeindex + length does not write out of bounds */
              if ((queueDataStartIdx + localPduPtr->length) >  CANIF_CFG_SIZEOF_FIFO_BUFFER) /* COV_CANIF_GENDATA_FAILURE */
              {
                errorId = CANIF_E_FATAL;
                retval = E_NOT_OK;
              }
              else
#  endif
              {
                /* #470 Store data of Tx-PDU at FIFO-WriteIdx */
                VStdLib_MemCpy(CANIF_ADDR_VAR_TXBUFFERFIFO_QUEUEDATA(queueDataStartIdx), localPduPtr->sdu, localPduPtr->length); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CANIF_32 */

#  if (CANIF_CANCEL_SUPPORT_API == STD_ON)
                /* #480 Set that the buffered Tx-PDU is valid at FIFO-WriteIdx */
                CANIF_VAR_TXBUFFERFIFO_QUEUEBASE((writeIdx + txFifoQueueBaseStartIdx), eQueued) = 1; /* SBSW_CANIF_34 */
                /* #490 Set the reference to CanIf_TxQueueIndex2FifoEl */
                CANIF_VAR_SET_TXBUFFERFIFO_QUEUEIDX2FIFOEL(CANIF_CFG_TX_QUEUEINDEX(localPduPtr->swPduHandle), writeIdx ); /* SBSW_CANIF_36 */
#  endif
                /* #500 Increment the FIFO-counter */
                (CANIF_VAR_TXBUFFERFIFO_BUFFERBASE( txBufferFifoBaseIdx, eQueueCounter))++; /* SBSW_CANIF_37 */

                /* #510 Increment the FIFO-WriteIdx. */
                (CANIF_VAR_TXBUFFERFIFO_BUFFERBASE( txBufferFifoBaseIdx, eWriteIdx))++; /* SBSW_CANIF_37 */
                if(CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eWriteIdx) == (CANIF_CFG_TXBUFFERFIFO_QUEUESIZE(txBufferCfgIdx)))
                {
                  /* If WriteIdx equals the QueueSize reset it to 0. */
                  CANIF_VAR_TXBUFFERFIFO_BUFFERBASE( txBufferFifoBaseIdx, eWriteIdx) = 0; /* SBSW_CANIF_37 */
                }
                retval = E_OK;
              }
            }
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
            else
            {
              errorId = CANIF_E_FULL_TX_BUFFER_FIFO;
            }
#  endif
          }
          else
          {
          /* #600 Do not buffer, return with E_NOT_OK */
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
            errorId = CANIF_E_PARAM_DLC;
#  endif
            retval = E_NOT_OK;
          }
          break;
# endif
      default: /* COV_CANIF_GENDATA_FAILURE */
        break;
      }
    }/* Development error check SduDataPtr == NULL_PTR */
  }
  else
  {
    /* #110 PduId has no Tx Buffer: Reject the Tx-request */
    retval = E_NOT_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_TRANSMIT_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif /* ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)) */


/***********************************************************************************************************************
 *  CanIf_TransmitSubWrite()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TransmitSubWrite
Relation_Context:
Relation:
CanIfTxBuffer
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
CANIF_LOCAL_INLINE FUNC (Std_ReturnType, CANIF_CODE) CanIf_TransmitSubWrite(P2CONST(Can_PduType, AUTOMATIC, AUTOMATIC) localPduPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  CanIf_CanDrvReturnType txResult = CANIF_CANDRV_NOT_OK;

#if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
  boolean doCanWrite = TRUE;
#endif

  /* ----- Implementation ----------------------------------------------- */
#if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
  txResult = CANIF_CANDRV_BUSY;
  if( CANIF_CFG_MAILBOX_TXBUFFERHANDLINGTYPE(CANIF_CFG_TX_HTH(localPduPtr->swPduHandle)) == CANIF_TXBUFFER_HANDLINGTYPE_FIFO)
  {
    /* #100 If Tx PDU is mapped to Tx buffer of handling type FIFO it must only be transmitted directly if FIFO is empty */
    doCanWrite = CanIf_TransmitSubCheckFiFoQueueEmpty(localPduPtr->swPduHandle);
  }
  if (doCanWrite == TRUE)
#endif
  {
    /* #200 Pass the request to the CAN Driver */
    txResult = CanIf_CallCanWrite(localPduPtr); /* SBSW_CANIF_5 */
  }

  if (txResult == CANIF_CANDRV_BUSY)
  {
#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
    /* #300 CAN Driver is busy, do the buffering according to the Tx buffer type */
    /* \trace SPEC-3668 */
    retval = CanIf_TransmitSubHandleTxResBusy(localPduPtr); /* SBSW_CANIF_5 */
#else
    retval = E_NOT_OK;  /* ESCAN00092426 */
#endif /* ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)) */
  }
  else if (txResult == CANIF_CANDRV_NOT_OK)
  {
    /* #400 CAN Driver reports FAILED: Reject the Tx request */
    retval = E_NOT_OK;
  }
  else /* txResult == CANIF_CANDRV_OK */
  {
    /* \trace SPEC-1903 */
   /* #500 CAN Driver reports SUCCEEDED: */
    /* ESCAN00066390 - start */
#if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
    /* #510 If a Tx PDU mapped to a Tx buffer of type PRIO_BY_CANID was transmitted do clear the corresponding queue flag */
    if(CANIF_CFG_MAILBOX_TXBUFFERHANDLINGTYPE(CANIF_CFG_TX_HTH(localPduPtr->swPduHandle)) == CANIF_TXBUFFER_HANDLINGTYPE_PRIOBYCANID)
    {
      (void)CanIf_ClearPrioByCanIdTxPduAsQueued(localPduPtr->swPduHandle);
    }
#endif
    /* ESCAN00066390 - end */
    retval = E_OK;
  }
  return retval;
}

#if (CANIF_TXPDUCANXLPROPS == STD_ON) /* COV_CANIF_SUPPORT_CAN_XL */ /* True, if CAN XL Tx PDUs exist or can be created at PB time. This preprocessor switch is required to ensure all needed macros exist. */
/***********************************************************************************************************************
*  CanIf_TransmitSubXLWrite()
**********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TransmitSubXLWrite
Relation_Context:
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
*/
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_TransmitSubXLWrite(PduIdType canXLTxPduId, SduDataPtrType sduDataPtr, uint16 sduLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  Std_ReturnType retVal = E_NOT_OK;

  /* CanIf_TransmitSubXLWrite is only called for CAN XL Tx PDUs. Therefore CanIf_GetTxPduCanXLPropsIdxOfTxPduConfig always returns a valid index. */
  const CanIf_AnyIdxType canXLTxPduPropIdx = CanIf_GetTxPduCanXLPropsIdxOfTxPduConfig(canXLTxPduId);

  CanXL_PduType localXLPdu; /* Fully initialized within the code below before being passed to CanXL_Write() */
  CanXL_Params localXLParams; /* Fully initialized within the code below before being passed to CanXL_Write() */

  localXLPdu.XLParams = &localXLParams;
  localXLPdu.length = sduLength;
  localXLPdu.sdu = sduDataPtr;
  localXLPdu.swPduHandle = canXLTxPduId;

  localXLPdu.XLParams->SduType = CanIf_GetSduTypeOfTxPduCanXLProps(canXLTxPduPropIdx);

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify SDU type dependent data length */ 
  /* \trace SPEC-22721018 */
  if ((localXLPdu.length < 1u) && (localXLPdu.XLParams->SduType != CANIF_CAN_XL_SDT_CAN_MAPPED_TUNNELING))
  {
    errorId = CANIF_E_DATA_LENGTH_MISMATCH;
  }
  else 
  {
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
    /* #110 Determine responsible CAN Driver CanXL_<Infix>_Write function (if multiple CAN Drivers are used) */
    const CanIf_CanXL_WriteFctType canXLWriteFct = CANIF_GET_CANXLWRITEFCT(CANIF_CFG_TX_CONTROLLER(canXLTxPduId));
# endif

    localXLPdu.XLParams->Vcid = CanIf_GetVcidOfTxPduCanXLProps(canXLTxPduPropIdx);
    localXLPdu.XLParams->PriorityId = CanIf_GetPriorityIdOfTxPduCanXLProps(canXLTxPduPropIdx);
    localXLPdu.XLParams->AcceptanceField = CanIf_GetAcceptanceFieldOfTxPduCanXLProps(canXLTxPduPropIdx); /* Correct value of the AF is determined for all SDU types by the generator. */
    localXLPdu.XLParams->Sec = 0u; /* \trace SPEC-22721023 */

    /* #120 Call CanXL_<Infix>_Write with CAN Driver specific CanXLHardwareObject and CAN XL Tx PDU */
    /* canXLWriteFct() is defined to CanXL_<Infix>_Write if only a single CAN Driver is used */
    retVal = canXLWriteFct((Can_HwHandleType)CanIf_GetCanXLHthOfTxPduCanXLProps(canXLTxPduPropIdx), (P2CONST(CanXL_PduType, AUTOMATIC, CANIF_VAR_STACK)) &localXLPdu);

    /* #130 If CAN Driver returns that the CanXLHardwareObject is busy, reject the transmit request */
    /* CAN_BUSY can be part of enum Can_ReturnType, therefore cast is needed to avoid MISRA warning (equality operator with unsigned and enum) */
    if (retVal == (Std_ReturnType)CAN_BUSY)
    {
      retVal = E_NOT_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_TRANSMIT_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
}
#endif


/***********************************************************************************************************************
 *  CanIf_CallCanWrite()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_CallCanWrite
Relation_Context:
CanIfTxBuffer # from caller CanIf_TxQueueTransmit #
# from caller CanIf_TransmitSubWrite #
Relation:
CanIfMultipleDriver
CanIfSupportCanDrvEqcAr431, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr431
CanIfSupportCanDrvEqcAr431, CanIfMultipleDriver
CanIfSupportCanDrvEqcAr431, CanIfMultipleDriver, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr431, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421, CanIfSupportCanDrvEqcMsrc, CanIfMultipleDriver
CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421, CanIfSupportCanDrvEqcMsrc, CanIfMultipleDriver, CanIfSupportCanDrvEqcAr431
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
CANIF_LOCAL_INLINE FUNC (CanIf_CanDrvReturnType, CANIF_CODE) CanIf_CallCanWrite(P2CONST(Can_PduType, AUTOMATIC, AUTOMATIC) localPduPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* CanIf_CanDrvReturnType is defined to Std_ReturnType if only CAN Drivers of EQC AR 4.3.1 are used; else to Can_ReturnType */
  CanIf_CanDrvReturnType retval = CANIF_CANDRV_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* \trace SPEC-1845 */

  /* #100 Determine CAN Driver specific CanHardwareObject */
#if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
  const Can_HwHandleType canDrvHth = (Can_HwHandleType)(CANIF_CFG_TX_HTH(localPduPtr->swPduHandle) - CANIF_CFG_MULTIPLECANDRV_HXHOFFSET(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(CANIF_CFG_TX_CONTROLLER(localPduPtr->swPduHandle))));
#else
  const Can_HwHandleType canDrvHth = (Can_HwHandleType)CANIF_CFG_TX_HTH(localPduPtr->swPduHandle);
#endif

#if ((CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON))
  /* #110 Determine EQC of responsible CAN Driver (if different CAN Driver EQCs are used) */
  if(CanIf_GetCanDrvEqcArVersionOfCanControllerIdUpToLowMap(CANIF_CFG_TX_CONTROLLER(localPduPtr->swPduHandle)) == CAN_DRV_EQC_AR_431) /* PRQA S 2742 */ /* MD_CanIf_CanDrvEqcConstData */ /* COV_CANIF_CANDRV_EQC */
#endif

#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
  {
    Std_ReturnType localRetval = E_NOT_OK; /* PRQA S 2880 */ /* MD_CanIf_CanDrvEqcUnreachableCode */

    /* #120 For CAN_DRV_EQC_AR_431
            Determine responsible CAN Driver Can_<Infix>_Write function (if multiple CAN Drivers are used)
            Call Can_<Infix>_Write with CAN Driver specific CanHardwareObject and Tx PDU
            Covert return value to Can_ReturnType (if different CAN Driver EQCs are used)
    */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#  if (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON)
    /* Combination with CAN Driver of EQC MSRC - so function pointer have to be casted */
    const CanIf_Can_WriteAr431FctType canWriteFct = CANIF_GET_CANWRITEFCT(CANIF_CFG_TX_CONTROLLER(localPduPtr->swPduHandle)).CanIf_Can_WriteAr431Fct; /* PRQA S 2880 */ /* MD_CanIf_CanDrvEqcUnreachableCode */
#  else
    const CanIf_Can_WriteFctType canWriteFct = CANIF_GET_CANWRITEFCT(CANIF_CFG_TX_CONTROLLER(localPduPtr->swPduHandle));
#  endif
# endif /* CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON */

    /* canWriteFct() is defined to Can_<Infix>_Write if only a single CAN Driver is used */
    localRetval = canWriteFct(canDrvHth, (P2CONST(Can_PduType, AUTOMATIC, CANIF_VAR_STACK))localPduPtr); /* SBSW_CANIF_53 */
    retval = CanIf_ConvertCanWriteAr431RetVal(localRetval); /* As independent step needed - otherwise Can_Write can be called twice within the macro */
  }
#endif /* CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON */

#if ((CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON))
  else
#endif

#if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421_OR_MSRC == STD_ON)
  {
    /* #130 For CAN_DRV_EQC_AR_403, CAN_DRV_EQC_AR_421 or CAN_DRV_EQC_MSRC
            Determine responsible CAN Driver Can_<Infix>_Write function (if multiple CAN Drivers are used)
            Call Can_<Infix>_Write with CAN Driver specific CanHardwareObject and Tx PDU
            (Return value is according to Can_ReturnType)
    */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#  if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
    /* Combination with CAN Driver of EQC AR 4.3.1 - so function pointer have to be casted */
    const CanIf_Can_WriteAr403FctType canWriteFct = CANIF_GET_CANWRITEFCT(CANIF_CFG_TX_CONTROLLER(localPduPtr->swPduHandle)).CanIf_Can_WriteAr403Fct;
#  else
    const CanIf_Can_WriteFctType canWriteFct = CANIF_GET_CANWRITEFCT(CANIF_CFG_TX_CONTROLLER(localPduPtr->swPduHandle));
#  endif
# endif /* CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON */

    /* canWriteFct() is defined to Can_<Infix>_Write if only a single CAN Driver is used */
    retval = canWriteFct(canDrvHth, (P2CONST(Can_PduType, AUTOMATIC, CANIF_VAR_STACK))localPduPtr); /* SBSW_CANIF_53 */
  }
#endif /* CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421_OR_MSRC == STD_ON */

  return retval;
}


/***********************************************************************************************************************
 *  CanIf_Transmit()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_Transmit
Relation_Context:
Relation:
CanIfDataChecksum
CanIfDynTx
CanIfDevErrorDetect, CanIfVariant
CanIfVariant
CanIfVariant, CanIfDevErrorDetect
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfPnEnable
CanIfDynTx
CanIfTxMetaDataSupport
CanIfJ1939
CanIfDataChecksum
CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
 *
 *
 *
 *
 */
/* PRQA S 6030, 6050, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_Transmit(PduIdType CanTxPduId, P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_MSG_VAR)PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if ((CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON))
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON) */

#if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
# if (CANIF_DEV_ERROR_DETECT == STD_ON) 
  /* #20 Check if parameter CanTxPduId is valid if a CanTxPduId indirection is done */
  if (CanTxPduId >= CANIF_CFG_MAX_ULTXPDUS)
  {
    /* \trace SPEC-2007 */
    errorId = CANIF_E_INVALID_TXPDUID;
  }
  else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    /* for PB configurations the TxPduId order between CanIf and higher layers may differ and needs to be indirected */
    CanTxPduId = CANIF_CFG_TXPDUID2INTTXPDUID(CanTxPduId); /* PRQA S 1338 */ /* MD_CanIf_Rule17.8 */
#endif /* (CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) */

#if (CANIF_DEV_ERROR_DETECT == STD_ON)
    /* #30 Check if parameter CanTxPduId is valid */
    if (CanTxPduId >= CANIF_CFG_MAX_TXPDUS)
    {
      /* \trace SPEC-2007 */
      errorId = CANIF_E_INVALID_TXPDUID;
    }
    else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
    {
      /* Determine the CAN controller controller */
      uint8 controllerId = CANIF_CFG_TX_CONTROLLER(CanTxPduId);

#if (CANIF_DEV_ERROR_DETECT == STD_ON)
      /* #40 Check if parameter PduInfoPtr is valid */
      if (PduInfoPtr == NULL_PTR)
      {
        /* \trace SPEC-2003 */
        errorId = CANIF_E_PARAM_POINTER;
      }
      /* #50 Check if CAN controller is initialized */
      else if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
      {
        /* \trace SPEC-5238 */
        errorId = CANIF_E_UNINIT;
        /* retval default E_NOT_OK */
      }
      else
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#  if (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
      /* #60 Check if corresponding CanDrv is available in current identity */
      if (!CANIF_CFG_IS_CANDRVINDEXVALID(controllerId))
      {
        errorId = CANIF_E_PARAM_CONTROLLERID;
      }
      else
#  endif /* CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON */
# endif /* CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON */
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
      {
        /* ----- Implementation ----------------------------------------------- */
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
        /* #100 Apply PnWuFilter (only if configured) */
        if (CANIF_DO_TRANSMIT == CanIf_TransmitSubHandleTxPduFilter(controllerId, CanTxPduId))
#endif
        {
          /* #200 Process only if CAN controller is started */
          if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_STARTED)
          {
            /* #210 Process only if PDU mode is online */
            /* \trace SPEC-1849, SPEC-3854, SPEC-3765, SPEC-1850 */
            if ((CANIF_VAR_GETPDUMODE(controllerId) == CANIF_GET_ONLINE) ||
              (CANIF_VAR_GETPDUMODE(controllerId) == CANIF_GET_TX_ONLINE)
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
              || (CANIF_VAR_GETPDUMODE(controllerId) == CANIF_GET_ONLINE_WAKF)
              || (CANIF_VAR_GETPDUMODE(controllerId) == CANIF_GET_TX_ONLINE_WAKF)
#endif
              )
            {
              PduLengthType sduLength = PduInfoPtr->SduLength; /* local variable needed since PduInfoPtr is const */

              /* #220 Verify Tx PDU length */
              if (CANIF_DO_TRANSMIT == CanIf_TransmitSubVerifyPduLength(CanTxPduId, &sduLength)) /* SBSW_CANIF_5 */
              {
#if (CANIF_TXPDUCANXLPROPS == STD_ON) /* COV_CANIF_SUPPORT_CAN_XL */ /* True, if CAN XL Tx PDUs exist or can be created at PB time. This preprocessor switch is required to ensure all needed macros exist. */
                /* #230 If the Tx PDU is a CAN XL Tx PDU (only if configured) */
                if (CanIf_IsTxPduCanXLPropsUsedOfTxPduConfig(CanTxPduId))
                {
                  /* #240 Transmit CAN XL Tx PDU */
                  /* Cast required for sduLength, because PduLengthType can be greater than uint16 (no data loss, as sduLength here is max. 2048) */
                  retval = CanIf_TransmitSubXLWrite(CanTxPduId, PduInfoPtr->SduDataPtr, (uint16)sduLength);
                }
                else
#endif
                /* #250 If the Tx PDU is a CAN 2.0/FD Tx PDU */
                {
                  Can_PduType localPdu; /* Fully initialized within the code below before being passed to other functions */
#if (CANIF_DATA_CHECKSUM_TX_SUPPORT == STD_ON)
                  uint8 txPduDataWithChecksum[CANIF_CFG_MAXTXDLC_PLUS_DATACHECKSUM] = { 0 };
#endif
                  /* \trace SPEC-1227 */
                  /* #300 Process the Tx request only within critical section */
                  CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_4);

                  /* #310 Depending on type of Tx PDU (dynamic / static) get the CAN identifier */
#if (CANIF_SETDYNAMICTXID_API == STD_ON)
                  {
                    PduIdType DynamicPduId = CANIF_CFG_STAT2DYN_INDIRECTION(CanTxPduId);
                    if (DynamicPduId != CanIf_TxPduHnd_INVALID)
                    {
                      (localPdu).id = CANIF_VAR_DYNTXID(DynamicPduId);
                    }
                    else
                    {
                      (localPdu).id = CANIF_CFG_TX_CANID(CanTxPduId);
                    }
                  }
#else
                  (localPdu).id = CANIF_CFG_TX_CANID(CanTxPduId);
#endif
                  localPdu.length = (uint8) sduLength;
                  localPdu.sdu = (PduInfoPtr->SduDataPtr);
                  localPdu.swPduHandle = (CanTxPduId);

#if (CANIF_META_DATA_TX_SUPPORT == STD_ON)
                  /* #320 Handle meta data of Tx PDU (only if configured) */
                  CanIf_TransmitSubHandleMetaData(PduInfoPtr, &localPdu); /* SBSW_CANIF_5 */
#endif

#if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
                /* #330 Adapt the CAN identifier according to J1939 specification (only if configured) */
                  retval = CanIf_TransmitSubJ1939DynAddr(controllerId, &localPdu); /* SBSW_CANIF_5 */
                  if (retval == E_OK) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
#endif
                  {
#if (CANIF_DATA_CHECKSUM_TX_SUPPORT == STD_ON)
                    /* #340 Is data checksum Tx PDU? (only if configured) */
                    if (CANIF_CFG_TX_ISDATACHECKSUMPDU(CanTxPduId))
                    {
                      /* #400 Calculate data checksum */
                      CanIf_TransmitSubDataChecksumTxAppend(&localPdu, &txPduDataWithChecksum[0]); /* SBSW_CANIF_31 */

                      /* #410 Adapt that Tx PDU is transmitted including checksum */
                      localPdu.sdu = (P2VAR(uint8, TYPEDEF, CANIF_APPL_VAR))(&txPduDataWithChecksum[0]);
                    }
#endif /* (CANIF_DATA_CHECKSUM_TX_SUPPORT == STD_ON) */

                    /* #350 Transmit Tx PDU */
                    retval = CanIf_TransmitSubWrite(&localPdu); /* SBSW_CANIF_5 */
                  }

                  /* \trace SPEC-1227 */
                  CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_4);
                }
              }
              else
              {
                /* Verification of Tx PDU length failed */
                /* default retval = E_NOT_OK */
                /* \trace SPEC-7204476 */
                errorId = CANIF_E_TXPDU_LENGTH_EXCEEDED;
              }
            }
            /* \trace SPEC-1848 */
            else if ((CANIF_VAR_GETPDUMODE(controllerId) == CANIF_GET_OFFLINE_ACTIVE) ||
                    (CANIF_VAR_GETPDUMODE(controllerId) == CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE))
            {
              retval = E_OK;
              /* \trace SPEC-5215 */
              /* #260 If PDU mode is Tx passive, call the Tx confirmation */
              CanIf_HlTxConfirmation(CanTxPduId);
            }
            else /* CANIF_GET_OFFLINE or CANIF_GET_RX_ONLINE */
            {
              /* \trace SPEC-3673 */
              /* default retval = E_NOT_OK */
              errorId = CANIF_E_STOPPED;
            }
          } /* if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_STARTED) */
          /* \trace SPEC-5339 */
          else if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_STOPPED)
          {
            /* If current mode is STOP do report DET */
            errorId = CANIF_E_STOPPED;
          }
          else
          {
            /*  CtrlMode here can only be CANIF_CS_SLEEP, transmit is not allowed */
            /* default retval = E_NOT_OK */
          }
        }
      }
    }
#if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
  }
#endif

  /* ----- Development Error Report --------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_TRANSMIT_API, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif


  return retval;
}


/**********************************************************************************************************************
 *  CanIf_HlTxConfirmation()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_HlTxConfirmation
Relation_Context:
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CANIF_LOCAL FUNC(void, CANIF_CODE) CanIf_HlTxConfirmation(PduIdType CanTxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* conform to requirement \trace SPEC-1877 */
  CanIf_TxConfFctListIdxType index;
  PduIdType upperLayerTxPduId;
  CanIf_TxConfirmationFctType txConfirmationFct;

  /* ----- Implementation ----------------------------------------------- */

  /* #100 Get the Tx-confirmation function */
  index = CANIF_CFG_TX_TXCONFIRMATION(CanTxPduId);

  /* #110 Convert the CanIf internal TxPduId to the upper layer one */
  upperLayerTxPduId = CANIF_CFG_TX_UPPERLAYERTXPDUID(CanTxPduId);

  txConfirmationFct = CANIF_CFG_TXCONFIRMATION(index);

  if (txConfirmationFct != NULL_PTR)
  {
    /* #120 If configured, call the Tx-confirmation function */
      txConfirmationFct(upperLayerTxPduId);  /* SBSW_CANIF_11 */
  }
}


/* \trace SPEC-3686 */
/**********************************************************************************************************************
 *  CanIf_TxConfirmation()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TxConfirmation
Relation_Context:
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfPnEnable
CanIfPnEnable, CanIfTxConfPollSupport
CanIfPnEnable
CanIfTxConfPollSupport
CanIfTxBuffer
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_TxConfirmation(PduIdType CanTxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;

  /* \trace SPEC-3701 */
  /* ----- Development Error Checks ------------------------------------- */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
# if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif
  /* #20 Check if parameter CanTxPduId is valid */
  if (CanTxPduId >=  CANIF_CFG_MAX_TXPDUS)
  {
    /* \trace SPEC-3700 */
    errorId = CANIF_E_PARAM_LPDU;
  }
  /* #30 Check if corresponding CAN controller is initialized */
  else if (CanIf_GetCtrlMode(CANIF_CFG_TX_CONTROLLER(CanTxPduId)) == CANIF_CS_UNINIT )
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#  if (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
  /* #40 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(CANIF_CFG_TX_CONTROLLER(CanTxPduId)))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#  endif
# endif
  else
#endif
  {
    /* \trace SPEC-3703 */
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Process only if CAN controller is active */
    if (CanIf_GetCtrlMode(CANIF_CFG_TX_CONTROLLER(CanTxPduId)) == CANIF_CS_STARTED)
    {
      if ( (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(CanTxPduId)) == CANIF_GET_ONLINE) ||
             (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(CanTxPduId)) == CANIF_GET_TX_ONLINE)
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
        || (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(CanTxPduId)) == CANIF_GET_ONLINE_WAKF)
        || (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(CanTxPduId)) == CANIF_GET_TX_ONLINE_WAKF)
#endif
         )
      {
#if ((CANIF_PN_WU_TX_PDU_FILTER == STD_ON) || (CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT == STD_ON))
        uint8 controllerId = CANIF_CFG_TX_CONTROLLER(CanTxPduId);
#endif

#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
        /* CANIF751 */
        /* #110 Deactivate Tx-PDU-filter */
        CanIf_SetPnWuTxPduFilterActive(controllerId, FALSE); /* SBSW_CANIF_8 */
#endif

#if (CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT == STD_ON)
        /* \trace SPEC-9981 */
        /* #120 Log that any Tx-confirmation occurred */
        CanIf_SetTxConfState(controllerId, CANIF_TX_RX_NOTIFICATION); /* SBSW_CANIF_8 */
#endif

#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
        {
          CanIf_AnyIdxType hth = CANIF_CFG_TX_HTH(CanTxPduId);
          /* \trace SPEC-3676 */
          /* Check if Tx-queue is affected (BasicCAN Tx-PDU which has a buffer) */
          if (
# if (CANIF_SUPPORT_CAN_XL == STD_ON) /* COV_CANIF_SUPPORT_CAN_XL */
            (CanIf_IsMailBoxConfigUsedOfTxPduConfig(CanTxPduId)) && /* verify that the PDU has a corresponding mailbox config (CAN XL PDUs have no corresponding mailbox config) */
# endif
            CANIF_CFG_MAILBOX_HASTXBUFFER(hth))
          {
            /* #130 If transmission of a BasicCAN PDU, which has a buffer, was confirmed, handle the Tx-queue (critical section is used) */
            /* \trace SPEC-1227 */
            CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_2);
            /* \trace SPEC-5275, SPEC-1231 */
            
            /* parameter hth out of CanTxPduId */
            (void)CanIf_TxQueueTreatment((CanIf_HwHandleType)hth);
            /* \trace SPEC-1227 */ /* lock over queue handling + write call (driver does not expect direct call inbetween) */
            CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_2);
          }
        }
#endif
        /* \trace SPEC-3666 */
        /* #140 Inform upper layer about transmitted PDU */
        CanIf_HlTxConfirmation(CanTxPduId);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_TXCONFIRMATION_API, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}


#if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
/**********************************************************************************************************************
 *  CanIf_RxIndicationSubJ1939DynAddr()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RxIndicationSubJ1939DynAddr
Relation_Context:
CanIfJ1939 # from this local function #
CanIfJ1939 # from caller CanIf_RxIndicationAsr403 #
Relation:
CanIfJ1939
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_RxIndicationSubJ1939DynAddr(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 controllerId = CANIF_CFG_MAILBOX_CONTROLLER(rxSearchParams->eHrh);
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Do J1939-conform address conversion only for J1939Nm CAN controller */
  if( CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(controllerId) != CANIF_J1939_DYN_ADDR_DISABLED ) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    /* #110 Distinguish between standard and extended CAN identifier */
    if(((rxSearchParams->eCanId) & CANIF_EXT_CANID_MASK) == CANIF_EXT_CANID_MASK) /* ext. ID */ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
    {
      /* #200 Extended CAN identifier: */
      if ((((rxSearchParams->eCanId) & CANIF_J1939_DATA_PAGE_MASK) != CANIF_J1939_DATA_PAGE_MASK) && (((rxSearchParams->eCanId) & CANIF_J1939_PGN_MASK) != CANIF_J1939_AC_PGN)) /* J1939 format and not AddressClaimed */ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
      {
        /* #210 Secure the consistency of J1939 address translation */
        CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_7);

        /* #220 Destination and source addresses are substituted with J1939 format but not for address claimed */
        if (((rxSearchParams->eCanId) & CANIF_J1939_MIN_PDU2_MASK) < CANIF_J1939_MIN_PDU2_MASK) /* PDU Format 1 (second byte < 0xF0) */ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
        {
          uint8 da = (uint8)(((rxSearchParams->eCanId) & CANIF_J1939_DA_MASK) >> 8); /* destination address */
          da = CANIF_VAR_J1939RXADDR_LOOKUP(controllerId, da);
          if (da == CANIF_J1939_NULL_ADDR) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
          {
            retVal = E_NOT_OK;
          }
          (rxSearchParams->eCanId) = (Can_IdType)(((rxSearchParams->eCanId) & CANIF_J1939_INV_DA_MASK) | (((Can_IdType)da) << 8u)); /* SBSW_CANIF_9 */
        }

        {
          uint8 sa = (uint8)((rxSearchParams->eCanId) & CANIF_J1939_SA_MASK); /* source address */
          uint8 sa_t = CANIF_VAR_J1939RXADDR_LOOKUP(controllerId, sa); /* source address transformed with lookup table */

          CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_7);

          if ((sa == CANIF_J1939_BROADCAST_ADDR) || ((sa != CANIF_J1939_NULL_ADDR) && (sa_t == CANIF_J1939_NULL_ADDR))) /* invalid source address */ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
          {
            retVal = E_NOT_OK;
          }
          (rxSearchParams->eCanId) = (Can_IdType)(((rxSearchParams->eCanId) & CANIF_J1939_INV_SA_MASK) | sa_t); /* SBSW_CANIF_9 */
        }
      }
    }
#  if (CANIF_J1939_DYN_ADDR_SUPPORT == CANIF_J1939_DYN_ADDR_MIXED_CANID)
    else
    {
      /* #300 Standard CAN identifier: */
      if( CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(controllerId) == CANIF_J1939_DYN_ADDR_MIXED_CANID ) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
      {
        /* #310 Only source address is substituted */
        uint8 sa = (uint8)((rxSearchParams->eCanId) & CANIF_J1939_SA_MASK); /* source address */
        uint8 sa_t = CANIF_VAR_J1939RXADDR_LOOKUP(controllerId, sa); /* source address transformed with lookup table */

        if ((sa == CANIF_J1939_BROADCAST_ADDR) || ((sa != CANIF_J1939_NULL_ADDR) && (sa_t == CANIF_J1939_NULL_ADDR))) /* invalid source address */
        {
          retVal = E_NOT_OK;
        }
        (rxSearchParams->eCanId) = (Can_IdType)(((rxSearchParams->eCanId) & CANIF_J1939_INV_SA_MASK) | sa_t); /* SBSW_CANIF_9 */
      }
    }
#  endif /* CANIF_J1939_DYN_ADDR_SUPPORT == CANIF_J1939_DYN_ADDR_MIXED_CANID */
  }

  return retVal;
}
#endif


#if (CANIF_SEARCH_ALGORITHM == CANIF_DOUBLE_HASH)
/**********************************************************************************************************************
 *  CanIf_RxIndicationSubDoubleHashSearch()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RxIndicationSubDoubleHashSearch
Relation_Context:
CanIfRxSearch # from this local function #
CanIfRxSearch # from caller CanIf_RxIndicationSubBasicCan #
Relation:
CanIfDevErrorDetect
CanIfRxSearchMsgType
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_RxIndicationSubDoubleHashSearch(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_IdType hash1;
  Can_IdType hash2;
  uint16_least dhCalcRxPduId; /* ESCAN00087755 */
  uint8 errorId = CANIF_E_NO_ERROR; /* ESCAN00087755 */

  /* ----- Implementation ----------------------------------------------- */
  /* \trace SPEC-3746 */
  /* \trace SPEC-1911 */
  /* #100 Calculate the RxPduId basing on received CAN identifier and the double hash numbers */
  hash1 = (rxSearchParams->eCanId ^ (Can_IdType)CANIF_CFG_RXDHRANDOMNUMBER1(CANIF_CFG_MAILBOX_CONTROLLER(rxSearchParams->eHrh))) % (Can_IdType)CANIF_CFG_RXDHADJUSTCOUNT;
  hash2 = (rxSearchParams->eCanId ^ (Can_IdType)CANIF_CFG_RXDHRANDOMNUMBER2(CANIF_CFG_MAILBOX_CONTROLLER(rxSearchParams->eHrh))) % (Can_IdType)CANIF_CFG_RXDHADJUSTCOUNT;
  dhCalcRxPduId = (uint16_least)CANIF_CFG_RXDHADJUST(hash1) + (uint16_least)CANIF_CFG_RXDHADJUST(hash2);

  if (dhCalcRxPduId >= CANIF_CFG_MAX_RXPDUS)
  {
    /* #110 If calculated RxPduId exceeds the number of configured Rx-PDUs reduce the calculated one by number of Rx-PDUs */
    rxSearchParams->eRxPduId = (PduIdType)(dhCalcRxPduId - CANIF_CFG_MAX_RXPDUS); /* SBSW_CANIF_9 */
  }
  else
  {
    rxSearchParams->eRxPduId = (PduIdType)(dhCalcRxPduId); /* SBSW_CANIF_9 */
  }

  /* #120 Verify the CAN identifier for matching */
  if (CANIF_CFG_RX_CANID(rxSearchParams->eRxPduId) == rxSearchParams->eCanId)
  {
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
    /* #10 Check if the matched RxPduId calculated via double hash is in valid range (== matches the corresponding CAN controller, ESCAN00087755). */
    if ((rxSearchParams->eRxPduId >= CANIF_CFG_MAILBOX_PDUIDFIRST(rxSearchParams->eHrh)) && (rxSearchParams->eRxPduId <= CANIF_CFG_MAILBOX_PDUIDLAST(rxSearchParams->eHrh))) /* COV_CANIF_GENDATA_FAILURE */
# endif
    {
      /* #130 If CAN identifier matches verify the message type regarding FD format (only if configured) */
# if (CANIF_RX_SEARCH_CONSIDER_MSG_TYPE == STD_ON)
      if ((CANIF_CFG_RX_MSGTYPE(rxSearchParams->eRxPduId) == CANIF_MSG_TYPE_CAN) || (CANIF_CFG_RX_MSGTYPE(rxSearchParams->eRxPduId) == rxSearchParams->eRxMsgType))
# endif
      {
        /* \trace SPEC-3677 */ /* ID matches - Pdu identified */
        rxSearchParams->eRxPduIdMatch = rxSearchParams->eRxPduId; /* SBSW_CANIF_9 */
        rxSearchParams->eContinueSearch = FALSE; /* no range search necessary */ /* SBSW_CANIF_9 */
      }
# if (CANIF_RX_SEARCH_CONSIDER_MSG_TYPE == STD_ON)
      else
      {
        /* #140 If message type does NOT match verify the CAN identifier and message type of the lower neighbour one (only if configured) */
        /* Make sure that eRxPduId does not point on the last Basic CAN ID with decided CAN Identifier, increment does not effect pointer on range */
        if(rxSearchParams->eRxPduId > CANIF_CFG_MAILBOX_PDUIDFIRST(rxSearchParams->eHrh))
        {
          rxSearchParams->eRxPduId--; /* for double hash the hash hits always for the msg with the higher prio if the CAN ID exist for two times */ /* SBSW_CANIF_9 */
          if ((CANIF_CFG_RX_CANID(rxSearchParams->eRxPduId) & CANIF_FD_FLAG_MASKOUT) == rxSearchParams->eCanId) /* Fix of ESCAN00092931 */
          {
            if(CANIF_CFG_RX_MSGTYPE(rxSearchParams->eRxPduId) == rxSearchParams->eRxMsgType) /* COV_CANIF_GENDATA_FAILURE */
            {
              /* \trace SPEC-3677 */ /* ID matches - Pdu identified */
              rxSearchParams->eRxPduIdMatch = rxSearchParams->eRxPduId; /* SBSW_CANIF_9 */
              rxSearchParams->eContinueSearch = FALSE; /* no range search necessary */ /* SBSW_CANIF_9 */
            }
          }
        }
      }
# endif
    }
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
    else
    {
      /* #20 The RxPduId calculated via double hash algorithm is NOT in valid range. Report this error. */
      errorId = CANIF_E_INVALID_DOUBLEHASH_CALC;
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* COV_CANIF_GENDATA_FAILURE */
  {
    CanIf_Det_ReportError(CANIF_RXINDICATION_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif /* CANIF_SEARCH_ALGORITHM == CANIF_DOUBLE_HASH */


#if (CANIF_SEARCH_ALGORITHM == CANIF_LINEAR)
/**********************************************************************************************************************
 *  CanIf_RxIndicationSubLinearSearch()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RxIndicationSubLinearSearch
Relation_Context:
CanIfRxSearch # from this local function #
CanIfRxSearch # from caller CanIf_RxIndicationSubBasicCan #
Relation:
CanIfRxSearchMsgType
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_RxIndicationSubLinearSearch(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams)
{
  /* \trace SPEC-3746 */
  /* \trace SPEC-1911 */
  /* Basic CAN message */
  /* Checked for invalid RxPduId earlier */

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Run linear through the search range in the Rx-PDU list in descending order to the first Rx-Range-PDU
          (loop breaks at this point, if no matched Rx-PDU was found before and the next Rx-PDU from the search range is a Rx-Range-PDU (has the flag CANIF_RANGE_FLAG set)) */
  while (((CANIF_CFG_RX_MASK(rxSearchParams->eRxPduId) & CANIF_RANGE_FLAG) != CANIF_RANGE_FLAG)) /* PRQA S 0771 */ /* MD_CanIf_MultipleBreak */ /* FETA_CANIF_3 */
  {
    /* #110 If CAN-Id of the current Rx-PDU matches the CAN-Id from the received CAN message */
    if (CANIF_CFG_RX_CANID(rxSearchParams->eRxPduId) == rxSearchParams->eCanId)
    {
      /* #120 If message type of current Rx-PDU matches the message type from the received CAN message (only if configured) */
# if (CANIF_RX_SEARCH_CONSIDER_MSG_TYPE == STD_ON)
      if ((CANIF_CFG_RX_MSGTYPE(rxSearchParams->eRxPduId)  == CANIF_MSG_TYPE_CAN) || (CANIF_CFG_RX_MSGTYPE(rxSearchParams->eRxPduId) == rxSearchParams->eRxMsgType))
# endif
      {
        /* #130 Set current Rx-PDU as match for the received CAN message and break the search
                (loop breaks at this point, because matched Rx-PDU was found) */
        /* ID matches - Pdu identified */ /* \trace SPEC-3677 */
        rxSearchParams->eRxPduIdMatch = rxSearchParams->eRxPduId; /* SBSW_CANIF_9 */
        rxSearchParams->eContinueSearch = FALSE; /* no range search necessary */ /* SBSW_CANIF_9 */
        break;
      }
    }

    /* #140 If the current Rx-PDU is the last one in the search range */
    if ((CANIF_CFG_RX_MASK(rxSearchParams->eRxPduId) & CANIF_STOP_FLAG) != 0u)
    {
      /* #150 Set no matched Rx-PDU found and do break the search
              (loop breaks at this point, because no matched Rx-PDU was found before and the current Rx-PDU is the last one in the search range (has the flag CANIF_STOP_FLAG set)) */
      rxSearchParams->eContinueSearch = FALSE; /* no range search necessary */ /* SBSW_CANIF_9 */
      break;
    }
    /* #160 If current Rx-PDU is within the search range */
    else
    {
      /* #170 Continue with the next Rx-PDU in the search range */
      rxSearchParams->eRxPduId--; /* SBSW_CANIF_9 */
    }
  }
}
#endif /* CANIF_SEARCH_ALGORITHM == CANIF_LINEAR */


/**********************************************************************************************************************
 *  CanIf_RxIndicationSubCheckPduProp()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RxIndicationSubCheckPduProp
Relation_Context:
Relation:
CanIfRxSearchMsgType, CanIfRxPduReceptionMode, CanIfDynRx
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
CANIF_LOCAL_INLINE FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_RxIndicationSubCheckPduProp(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams)
{
  /* ----- Local Variables ---------------------------------------------- */
  CanIf_NotifStatusType retVal = CANIF_TX_RX_NOTIFICATION;

  /* ----- Implementation ----------------------------------------------- */
#if (CANIF_RX_SEARCH_CONSIDER_MSG_TYPE == STD_ON)
  /* #100 Check the message type (only if configured) */
  if ((CANIF_CFG_RX_MSGTYPE(rxSearchParams->eRxPduId) != CANIF_MSG_TYPE_CAN) && (CANIF_CFG_RX_MSGTYPE(rxSearchParams->eRxPduId) != rxSearchParams->eRxMsgType))
  {
    retVal = CANIF_NO_NOTIFICATION;
  } else
#endif

#if (CANIF_SET_PDU_RECEPTION_MODE_SUPPORT == STD_ON)
  /* #110 Check the PDU reception mode, if PDU has it enabled (only if configured) */
  if(CANIF_CFG_RX_ISRECEPTIONMODEPDU(rxSearchParams->eRxPduId) && (CANIF_VAR_GET_PDU_RECEPTION_MODE(CANIF_CFG_RX_PDURECEPTIONMODEIDX(rxSearchParams->eRxPduId)) == CANIF_RMT_IGNORE_CONTINUE))
  {
    retVal = CANIF_NO_NOTIFICATION;
  } else
# endif

#if (CANIF_SETDYNAMICRXID_API == STD_ON)
  /* #120 Check the CAN ID, if the PDU is dynamic (only if configured) */
  if(CanIf_IsDynRxIdUsedOfRxPduConfig(rxSearchParams->eRxPduId) && (CanIf_GetDynRxId(CanIf_GetDynRxIdIdxOfRxPduConfig(rxSearchParams->eRxPduId)) != rxSearchParams->eCanId))
  {
    retVal = CANIF_NO_NOTIFICATION;
  } else
# endif

  {
    /* #130 Set the PDU as identified if the checks were passed */
    rxSearchParams->eRxPduIdMatch = rxSearchParams->eRxPduId; /* SBSW_CANIF_9 */
  }

  return retVal;
}


/**********************************************************************************************************************
 *  CanIf_RxIndicationSubRangeSearch()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RxIndicationSubRangeSearch
Relation_Context:
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_RxIndicationSubRangeSearch(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Run linear through Rx-range search area until the last Rx-range */
  while ((CANIF_CFG_RX_MASK(rxSearchParams->eRxPduId) & CANIF_STOP_RANGE_FLAG) == CANIF_RANGE_FLAG) /* PRQA S 0771 */ /* MD_CanIf_MultipleBreak */ /* FETA_CANIF_4 */
  {
    /* #110 Handle code-mask Rx-range */
    if ((CANIF_CFG_RX_CANID(rxSearchParams->eRxPduId) & CANIF_MASKCODE_RANGE_FLAG) == CANIF_MASKCODE_RANGE_FLAG) /* PRQA S 2992, 2996 */ /* MD_CanIf_RxPduConstData */
    {
      if (CANIF_CFG_RX_CANID(rxSearchParams->eRxPduId) == ((rxSearchParams->eCanId & (CANIF_CFG_RX_MASK(rxSearchParams->eRxPduId) )) | CANIF_MASKCODE_RANGE_FLAG )) /* PRQA S 2996 */ /* MD_CanIf_RxPduConstData */
      {
        /* #120 Check additional PDU properties */
        if (CanIf_RxIndicationSubCheckPduProp(rxSearchParams) == CANIF_TX_RX_NOTIFICATION) /* PRQA S 2991, 2995 */ /* MD_CanIf_RxPduConstData */ /* SBSW_CANIF_5 */ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
        {
          break;
        }
      }
    }
    else
    /* #130 Handle upper-lower CAN identifier Rx-range */
    {
      if ((rxSearchParams->eCanId >= CANIF_CFG_RX_CANID(rxSearchParams->eRxPduId)) && (rxSearchParams->eCanId <= ((CANIF_CFG_RX_MASK(rxSearchParams->eRxPduId) & CANIF_STOP_RANGE_FLAG_MASKOUT))))
      {
        /* #140 Check additional PDU properties */
        if (CanIf_RxIndicationSubCheckPduProp(rxSearchParams) == CANIF_TX_RX_NOTIFICATION) /* PRQA S 2991, 2995 */ /* MD_CanIf_RxPduConstData */ /* SBSW_CANIF_5 */ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
        {
          break;
        }
      }
    }
    rxSearchParams->eRxPduId--; /* SBSW_CANIF_9 */
  } /* while-loop */

  /* #200 Process the last / if only one Rx-range is configured separately */
  if ((CANIF_CFG_RX_MASK(rxSearchParams->eRxPduId) & CANIF_STOP_RANGE_FLAG) == CANIF_STOP_RANGE_FLAG)
  {
    /* #210 Handle code-mask Rx-range */
    if ((CANIF_CFG_RX_CANID(rxSearchParams->eRxPduId) & CANIF_MASKCODE_RANGE_FLAG) == CANIF_MASKCODE_RANGE_FLAG) /* PRQA S 2992, 2996 */ /* MD_CanIf_RxPduConstData */
    {
      /* End flag for Search algorithm is set, this means no more messages to search for */
      if (CANIF_CFG_RX_CANID(rxSearchParams->eRxPduId) == ((rxSearchParams->eCanId & CANIF_CFG_RX_MASK(rxSearchParams->eRxPduId)) | CANIF_MASKCODE_RANGE_FLAG )) /* PRQA S 2996 */ /* MD_CanIf_RxPduConstData */
      {
        /* #220 Check additional PDU properties */
        (void)CanIf_RxIndicationSubCheckPduProp(rxSearchParams); /* SBSW_CANIF_5 */
      }
    }
    else
    /* #230 Handle upper-lower CAN identifier Rx-range */
    {
      /* End flag for Search algorithm is set, this means no more messages to search for */
      if ((rxSearchParams->eCanId >= CANIF_CFG_RX_CANID(rxSearchParams->eRxPduId)) && (rxSearchParams->eCanId <= ((CANIF_CFG_RX_MASK(rxSearchParams->eRxPduId) & CANIF_STOP_RANGE_FLAG_MASKOUT))))
      {
        /* #240 Check additional PDU properties */
        (void)CanIf_RxIndicationSubCheckPduProp(rxSearchParams); /* SBSW_CANIF_5 */
      }
    }
  }
}


#if ((CANIF_WAKEUP_VALIDATION == STD_ON) && (CANIF_WAKEUP_VALID_ALL_RX_MSGS == STD_ON))
/**********************************************************************************************************************
 *  CanIf_RxIndicationSubWakeupValidation()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RxIndicationSubWakeupValidation
Relation_Context:
CanIfWakeupValidation # from this local function #
CanIfWakeupValidation # from caller CanIf_RxIndicationAsr403 #
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 */
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_RxIndicationSubWakeupValidation(P2CONST(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* Acquire controller id from CanIf_Mailbox struct */
  uint8 controllerId = CANIF_CFG_MAILBOX_CONTROLLER(rxSearchParams->eHrh);

  /* ----- Implementation ----------------------------------------------- */
  if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_STARTED)
  {
    /* \trace SPEC-21407 */ /* All messages will be used for validation only in controller mode STARTED */
    /* #100 If CAN controller is active set validation state to DETECTED (only if configured) */
    CanIf_SetWakeUpValidationState(controllerId, CANIF_WUVALIDATION_DETECTED); /* SBSW_CANIF_19 */
  }
}
#endif /* ((CANIF_WAKEUP_VALIDATION == STD_ON) && (CANIF_WAKEUP_VALID_ALL_RX_MSGS == STD_ON)) */


/**********************************************************************************************************************
 *  CanIf_RxIndicationSubBasicCan()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RxIndicationSubBasicCan
Relation_Context:
Relation:
CanIfRxSearch
CanIfRxPduReceptionMode
CanIfRxSearch, CanIfRxPduReceptionMode
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
   Inital search range in the Rx PDU list has to be defined from the caller function by setting rxSearchParams->eRxPduId to CANIF_CFG_MAILBOX_PDUIDLAST(Hrh)

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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_RxIndicationSubBasicCan(P2VAR(CanIf_RxSearchParamsType, AUTOMATIC, AUTOMATIC) rxSearchParams)
{
  /* ----- Implementation ----------------------------------------------- */
  rxSearchParams->eContinueSearch = TRUE; /* SBSW_CANIF_9 */
  /* #10 Check if not only Rx Range PDUs are configured */
  if((CANIF_CFG_RX_MASK(rxSearchParams->eRxPduId) & CANIF_RANGE_FLAG) != CANIF_RANGE_FLAG)
  {
#if (CANIF_SEARCH_ALGORITHM == CANIF_DOUBLE_HASH)
    /* #200 DoubleHash search */
    CanIf_RxIndicationSubDoubleHashSearch(rxSearchParams); /* SBSW_CANIF_5 */
#endif /* CANIF_SEARCH_ALGORITHM == CANIF_DOUBLE_HASH */

#if (CANIF_SEARCH_ALGORITHM == CANIF_LINEAR)
    /* #300 Linear search */
    CanIf_RxIndicationSubLinearSearch(rxSearchParams); /* SBSW_CANIF_5 */
#endif /* CANIF_SEARCH_ALGORITHM == CANIF_LINEAR */

#if (CANIF_SET_PDU_RECEPTION_MODE_SUPPORT == STD_ON)
    /* #400 Match found */
    if(rxSearchParams->eRxPduIdMatch != CanIf_RxPduHnd_INVALID)
    {
      /* #410 Check the PDU reception mode */
      if (CANIF_CFG_RX_ISRECEPTIONMODEPDU(rxSearchParams->eRxPduIdMatch))
      {
        /* #420 If PDU reception mode for matched PDU is CANIF_RMT_IGNORE_CONTINUE, match will be discarded */
        if (CANIF_VAR_GET_PDU_RECEPTION_MODE(CANIF_CFG_RX_PDURECEPTIONMODEIDX(rxSearchParams->eRxPduIdMatch)) == CANIF_RMT_IGNORE_CONTINUE)
        {
          rxSearchParams->eRxPduIdMatch = CanIf_RxPduHnd_INVALID; /* SBSW_CANIF_9 */
          /* #430 If Rx Range PDUs are defined at all, range search will continue */
          if((CANIF_CFG_RX_MASK(CANIF_CFG_MAILBOX_PDUIDFIRST(rxSearchParams->eHrh)) & CANIF_STOP_FLAG) == 0u)
          {
            rxSearchParams->eContinueSearch = TRUE; /* SBSW_CANIF_9 */
          }
        }
      }
    }
#endif /* CANIF_SET_PDU_RECEPTION_MODE_SUPPORT == STD_ON */

    /* Rx Range PDUs will be checked for all search algorithms */
    /* \trace SPEC-1911, SPEC-1844, SPEC-3704 */

    /* #440 Adapt search range for possible Rx Range PDU search */
    /* For linear: eRxPduId is already set to first Rx-Range-PDU, if no match found and Rx Range PDUs are available. Must NOT be adapted for range search.
       For linear: eRxPduId is set to eRxPduIdMatch, if match was found. Must be adapted for range search if CANIF_SET_PDU_RECEPTION_MODE_SUPPORT is enabled.
       For double hash: eRxPduId is set to possible matching PDU. Must be always adapted for range search. */

#if ((CANIF_SEARCH_ALGORITHM == CANIF_DOUBLE_HASH) || (CANIF_SET_PDU_RECEPTION_MODE_SUPPORT == STD_ON))
    /* eRxPduId needs to be set to first Rx Range PDU (CANIF_CFG_MAILBOX_PDUIDFIRST(rxSearchParams->eHrh) - 1), if Rx Range PDUs are available */
    rxSearchParams->eRxPduId = CANIF_CFG_MAILBOX_PDUIDFIRST(rxSearchParams->eHrh); /* SBSW_CANIF_9 */
    if((CANIF_CFG_RX_MASK(rxSearchParams->eRxPduId) & CANIF_STOP_FLAG) == 0u)
    {
      rxSearchParams->eRxPduId--; /* SBSW_CANIF_9 */
    }
    else
    {
      rxSearchParams->eContinueSearch = FALSE; /* No Rx Range PDUs defined */ /* SBSW_CANIF_9 */
    }
#endif /* (CANIF_SEARCH_ALGORITHM == CANIF_DOUBLE_HASH) || (CANIF_SET_PDU_RECEPTION_MODE_SUPPORT == STD_ON) */
  } /* ESCAN00079651 */

  /* #500 Search for Rx Range PDUs separately */
  if (rxSearchParams->eContinueSearch == TRUE)
  {
    CanIf_RxIndicationSubRangeSearch(rxSearchParams); /* SBSW_CANIF_5 */
  }
}


/* \trace SPEC-1836, SPEC-1228, SPEC-1931, SPEC-1235 */
/**********************************************************************************************************************
 *  CanIf_RxIndicationAsr403()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RxIndicationAsr403
Relation_Context:
Relation:
CanIfJ1939
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfIDType
CanIfBusMirroringSupport
CanIfRxSearchMsgType, CanIfRxMetaDataSupport, CanIfFdSupport # used because RxMetaData/MsgType is depending of Fd #
CanIfPnEnable
CanIfWakeupValidation
CanIfJ1939
CanIfRxFullCANSupport, CanIfRxBasicCANSupport
CanIfDevErrorDetect
CanIfDataChecksum
CanIfDataChecksum, CanIfVariant
CanIfRxMetaDataSupport, CanIfFdSupport # used because RxMetaData is depending of Fd #
CanIfRxIndicationUsedTypeIII, CanIfRxMetaDataSupport, CanIfRxIndicationUsedTypeIV
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
 *
 *
 *
 */
/* PRQA S 6010, 6030, 6050, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_RxIndicationAsr403(CanIf_HwHandleType Hrh, Can_IdType CanId, uint8 CanDlc, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR)CanSduPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  CanIf_RxSearchParamsType rxSearchParams;
#if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
  Std_ReturnType j1939DynAddrRetVal;
#endif
  uint8 errorId = CANIF_E_NO_ERROR;


  /* ----- Development Error Checks ------------------------------------- */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
# if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif
  /* #20 Check if parameter CanSduPtr is valid */
  if (CanSduPtr == NULL_PTR)
  {
    /* \trace SPEC-3714 */
    errorId = CANIF_E_PARAM_POINTER;
  }
# if (CANIF_EXTENDEDID_SUPPORT != STD_ON)
  /* #30 Check if parameter CanId is valid */
  else if ((CanId & CANIF_FD_FLAG_MASKOUT) > 0x7FFu)
  {
    /* \trace SPEC-3698 */
    errorId = CANIF_E_PARAM_CANID;
  }
# endif
  /* #40 Check if parameter CanDlc is valid */
  else if (CanDlc > RxDlcMaxValueLookUp[CanIf_GetShiftedFdFlag(CanId)])
  {
    /* \trace SPEC-3715 */
    errorId = CANIF_E_PARAM_DLC;
  }
  /* #50 Check if parameter Hrh is valid */
  else if (Hrh >= CANIF_CFG_MAX_MAILBOXES)
  {
    /* \trace SPEC-3697 */
    errorId = CANIF_E_PARAM_HRH;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
    /* #100 Report CAN frame to mirror module, if mirroring is enabled for the CAN controller */
    if(CanIf_IsMirroringEnabled(CANIF_CFG_MAILBOX_CONTROLLER(Hrh)))
    {
      Mirror_ReportCanFrame(CANIF_CFG_MAILBOX_CONTROLLER(Hrh), CanId, CanDlc, CanSduPtr); /* SBSW_CANIF_5 */
    }
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */

#if (CANIF_RX_SEARCH_CONSIDER_MSG_TYPE == STD_ON) || (CANIF_META_DATA_RX_SUPPORT == STD_ON)
    /* #110 Determine the message type */
    if ((CanId & CANIF_FD_FLAG) == CANIF_FD_FLAG)
    {
      rxSearchParams.eRxMsgType = CANIF_MSG_TYPE_FD_CAN;
    }
    else
    {
      rxSearchParams.eRxMsgType = CANIF_MSG_TYPE_NO_FD_CAN;
    }
#endif /* CANIF_RX_SEARCH_CONSIDER_MSG_TYPE == STD_ON || CANIF_META_DATA_RX_SUPPORT == STD_ON */

    /* #120 Remove FD-flag from CAN identifier */
    CanId = (CanId & CANIF_FD_FLAG_MASKOUT); /* The FD Flag is no part of the CAN ID */ /* PRQA S 1338 */ /* MD_CanIf_Rule17.8 */
    rxSearchParams.eCanId = CanId;
    rxSearchParams.eHrh = Hrh;

#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
    /* FEAT-2076 ESCAN00091870 */
    /* #130 Deactivate Tx PDU filter */
    CanIf_SetPnWuTxPduFilterActive(CANIF_CFG_MAILBOX_CONTROLLER(Hrh), FALSE); /* SBSW_CANIF_8 */
#endif  

#if ((CANIF_WAKEUP_VALIDATION == STD_ON) && (CANIF_WAKEUP_VALID_ALL_RX_MSGS == STD_ON))
    /* #140 Handle wake-up validation state (only if configured)) */
    CanIf_RxIndicationSubWakeupValidation(&rxSearchParams); /* SBSW_CANIF_5 */
#endif

#if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
    /* #150 Adapt the CAN identifier according to J1939 specification (only if configured) */
    j1939DynAddrRetVal = CanIf_RxIndicationSubJ1939DynAddr(&rxSearchParams); /* SBSW_CANIF_5 */
    if (j1939DynAddrRetVal == E_OK) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
#endif
    {
      /* #200 Do search only if any CAN 2.0/FD Rx PDU (including Rx Range PDUs) is configured */
      if (CANIF_CFG_GET_NUM_OF_NON_CAN_XL_RX_PDUS != 0u) /* PRQA S 2741 */ /* MD_CanIf_Rule14.3_GenData */ /* COV_CANIF_GENDATA_FAILURE */
      {
        rxSearchParams.eRxPduId = CANIF_CFG_MAILBOX_PDUIDLAST(Hrh);
        /* Check if PDU ID is valid */
        if (rxSearchParams.eRxPduId != CanIf_RxPduHnd_INVALID) /* PRQA S 2991, 2995 */ /* MD_CanIf_RxPduConstData */ /* COV_CANIF_GENDATA_FAILURE */
        {
          rxSearchParams.eRxPduIdMatch = CanIf_RxPduHnd_INVALID;

          /* #300 If Full-CAN, no search for matching PDU is needed */
          if (CANIF_CFG_MAILBOX_MAILBOXTYPE(Hrh) == CANIF_RxFullCANMailbox)
          {
            /* #310 Check additional PDU properties */
            (void)CanIf_RxIndicationSubCheckPduProp(&rxSearchParams); /* SBSW_CANIF_5 */
          }
          else if (CANIF_CFG_MAILBOX_MAILBOXTYPE(Hrh) == CANIF_RxBasicCANMailbox) /* COV_CANIF_GENDATA_FAILURE */
          /* #320 If Basic-CAN, use linear or double hash algorithm (dependent on configuration) to search for matching PDU */
          {
            CanIf_RxIndicationSubBasicCan(&rxSearchParams); /* SBSW_CANIF_5 */
          }
          else
          {
            /* Hrh does not belong to Rx Mailbox, returns without an indication */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
            errorId = CANIF_E_CONFIG;
#endif
          }

          /* #330 Matched Rx PDU found? */
          if (rxSearchParams.eRxPduIdMatch != CanIf_RxPduHnd_INVALID)
          {
#if (CANIF_DATA_CHECKSUM_RX_SUPPORT == STD_ON)
            Std_ReturnType localRetVal;

            if (CANIF_CFG_RX_ISDATACHECKSUMPDU(rxSearchParams.eRxPduIdMatch))
            {
# if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
              /* For PB and LT configurations the RxPduId order between CanIf and higher layers may differ and needs to be indirected */
              PduIdType CanIfRxPduId = CANIF_CFG_INTRXPDUID2EXTRXPDUID(rxSearchParams.eRxPduIdMatch);
# else
              PduIdType CanIfRxPduId = rxSearchParams.eRxPduIdMatch;
# endif
              /* #400 Data checksum Rx PDU: Verify the checksum. (only if configured) */
              /* Note: Explicitly the CanId of received message is used (Not the manipulated one in case of J1939 dynamic address support). */
              localRetVal = CanIf_RxIndicationSubDataChecksumRxVerify(CanIfRxPduId, CanId, CanDlc, CanSduPtr); /* SBSW_CANIF_5 */
            }
            else
            {
              /* #410 None data checksum Rx PDU: Verification passed. (only if configured) */
              localRetVal = E_OK;
            }

            /* #420 Data checksum verification passed? (only if configured) */
            if (localRetVal == E_OK)
#endif /* (CANIF_DATA_CHECKSUM_RX_SUPPORT == STD_ON) */
            {
#if (CANIF_META_DATA_RX_SUPPORT == STD_ON)
              /* #430 Restore FD-flag in CAN identifier */
              if (rxSearchParams.eRxMsgType == CANIF_MSG_TYPE_FD_CAN)
              {
                rxSearchParams.eCanId |= CANIF_FD_FLAG;
              }
#endif
              /* #440 Report received Rx PDU to upper layer */
              CanIf_Indirect_HlIndication(CANIF_CFG_MAILBOX_CONTROLLER(Hrh), rxSearchParams.eRxPduIdMatch, CanSduPtr, CanDlc, rxSearchParams.eCanId, CANIF_RXINDICATION_API); /* SBSW_CANIF_10 */
            }
          }
        } /* if (RxPduId != CanIf_RxPduHnd_INVALID) */
      } /* if (CANIF_CFG_GET_NUM_OF_NON_CAN_XL_RX_PDUS != 0) */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_RXINDICATION_API, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}

#if (CANIF_SUPPORT_CAN_XL == STD_ON) /* COV_CANIF_SUPPORT_CAN_XL */
/**********************************************************************************************************************
*  CanIf_XLRxIndication()
*********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_XLRxIndication
Relation_Context:
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
 *
*/
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_XLRxIndication(P2CONST(CanXL_HwType, AUTOMATIC, CANIF_CBK_DRV_VAR) Mailbox, P2CONST(PduInfoType, AUTOMATIC, CANIF_CBK_DRV_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if all input pointers have a valid value */
  if ((PduInfoPtr == NULL_PTR) || (Mailbox == NULL_PTR) || (Mailbox->XLParams == NULL_PTR))
  {
    errorId = CANIF_E_PARAM_POINTER;
  }
  /* #30 Check if the length is valid */
  else if ((PduInfoPtr->SduLength > CANIF_CAN_XL_MAX_LENGTH)
    || ((Mailbox->XLParams->SduType == CANIF_CAN_XL_SDT_CAN_MAPPED_TUNNELING) /* if SDU type == 3, the length needs to be > 0 && <= 9/65 */
    && ((PduInfoPtr->SduLength < CANIF_LENGTH_OFFSET_OF_SDT3) || ((PduInfoPtr->SduLength - CANIF_LENGTH_OFFSET_OF_SDT3) > RxDlcMaxValueLookUp[CanIf_GetShiftedFdFlagOfIdField(Mailbox->XLParams->AcceptanceField)]))))
  {
    errorId = CANIF_E_PARAM_DLC;
  }
  /* #40 Check if the XL parameters have a valid value */
  /* CAN XL parameter SDU type and Acceptance Field don't have to be checked, since all possible values of their data types are valid) */
  else if ((Mailbox->XLParams->Vcid > CANIF_CAN_XL_MAX_VCID) || (Mailbox->XLParams->PriorityId > CANIF_CAN_XL_MAX_PRIORITY_ID))
  {
    errorId = CANIF_E_INVALID_XLPARAM;
  }
  /* #50 Check if ControllerId has a valid value */
  else if (Mailbox->ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* No check of the Hoh is needed since it is never used */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Do search only if at least one CAN XL Rx PDU is configured */
# if (CANIF_RXPDUCANXLPROPS == STD_ON) /* True, if CAN XL Rx PDUs exist or can be created at PB time. This preprocessor switch is required to ensure all needed macros exist. */
    if (CanIf_GetSizeOfRxPduCanXLProps() > 0u) /* PRQA S 2741 */ /* MD_CanIf_Rule14.3_GenData */
    {
      /* #110 Determine the SDU type of the received CAN XL Rx PDU */
      if (Mailbox->XLParams->SduType == CANIF_CAN_XL_SDT_CAN_MAPPED_TUNNELING)
      {
#  if (CANIF_RXPDUCONFIGCANXLRXPDUSSDT3USEDOFCTRLCONFIG == STD_ON) /* True, if CAN XL Rx PDUs of SDU type 3 exist or can be created at PB time. This preprocessor switch is required to ensure all needed macros exist. */
      /* #200 If the received CAN XL Rx PDU has SDU type 3 */
        CanIf_MsgType msgType = CANIF_MSG_TYPE_CAN; 

        /* #210 Determine the message type */
#   if (CANIF_RX_SEARCH_CONSIDER_MSG_TYPE == STD_ON) || (CANIF_DEV_ERROR_DETECT == STD_OFF) /* msgType is required for the validation of the extracted DLC (only needed if DET checks are deactivated) and the message type check. */
        if ((Mailbox->XLParams->AcceptanceField & CANIF_CAN_XL_ID_FIELD_FD_BIT_MASK) == CANIF_CAN_XL_ID_FIELD_FD_BIT_MASK)
        {
          msgType = CANIF_MSG_TYPE_FD_CAN;
        }
        else
        {
          msgType = CANIF_MSG_TYPE_NO_FD_CAN;
        }
#   endif
        {
          /* #220 Iterate over configured CAN XL Rx PDUs of SDU type 3 */
          CanIf_RxPduConfigIterType canXLRxPduId = CanIf_GetRxPduConfigCanXLRxPdusSdt3StartIdxOfCtrlConfig(Mailbox->ControllerId);
          for (; canXLRxPduId < CanIf_GetRxPduConfigCanXLRxPdusSdt3EndIdxOfCtrlConfig(Mailbox->ControllerId); canXLRxPduId++)
          {
            CanIf_RxPduCanXLPropsIdxOfRxPduConfigType canXLRxPduPropIdx = CanIf_GetRxPduCanXLPropsIdxOfRxPduConfig(canXLRxPduId);

            /* #230 Compare the received CAN XL parameters with the configured CAN XL parameters */
            if ((CanIf_GetVcidOfRxPduCanXLProps(canXLRxPduPropIdx) == Mailbox->XLParams->Vcid)
              && (CanIf_GetPriorityIdOfRxPduCanXLProps(canXLRxPduPropIdx) == Mailbox->XLParams->PriorityId))
            {
              /* #240 Convert the Acceptance Field to the CAN ID format */
              Can_IdType canId = Mailbox->XLParams->AcceptanceField & CANIF_CAN_XL_ID_FIELD_CAN_ID_MASK; /* CAN ID without control bits */
              canId = canId | ((Mailbox->XLParams->AcceptanceField & CANIF_CAN_XL_ID_FIELD_IDE_BIT_MASK) << CANIF_CAN_XL_ID_FIELD_IDE_BIT_OFFSET); /* Move IDE bit to correct position, FD bit not considered since already checked before */

              /* #250 Check if received CAN ID and message type matches the configured CAN XL Rx PDU */
              if (CanIf_XLRxIndicationSubCanIdMatchesConfiguredPdu(canXLRxPduId, canId, msgType))
              {
                /* #260 Extract DLC of the tunneled CAN 2.0/FD PDU and verify length */
                uint8 dlcOfTunneledCanPdu = PduInfoPtr->SduDataPtr[0] & CANIF_CAN_XL_DLC_IN_FIRST_DATA_BYTE_MASK; /* Get DLC of tunnneled CAN 2.0/FD PDU from first data byte */
                uint8 sduLength = DlcToSduLengthLookUp[dlcOfTunneledCanPdu];

                if (((PduInfoPtr->SduLength - CANIF_LENGTH_OFFSET_OF_SDT3) != sduLength) /* Check consistency between the length of the CAN XL PDU and the length encoded in the extracted DLC */
#   if (CANIF_DEV_ERROR_DETECT == STD_OFF)
                      /* Check that the encoded DLC is <= 8 for CAN 2.0 PDUs (already implicitly tested by the statement above if DET Checks are enabled). */
                      || ((sduLength > 8u) && (msgType == CANIF_MSG_TYPE_NO_FD_CAN))
#   endif
                  )
                {
                  errorId = CANIF_E_PARAM_DLC;
                }
                else
                {
                  /* #270 Extract payload of the tunneled CAN 2.0/FD PDU and pass the extracted CAN 2.0/FD Rx PDU to upper layer */
                  P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR) dataPtr = &(PduInfoPtr->SduDataPtr[1]); /* Set the dataPtr to the first actual data byte of the tunneled CAN 2.0/FD PDU */
                  CanIf_Indirect_HlIndication(Mailbox->ControllerId, (PduIdType)canXLRxPduId, dataPtr, sduLength, canId, CANIF_XLRXINDICATION_API);
                }
                break;
              }
            }
          }
        }
#  endif
      }
#  if (CANIF_RXPDUCONFIGCANXLRXPDUSSDTNOT3USEDOFCTRLCONFIG == STD_ON) /* True, if CAN XL Rx PDUs of SDU type other than 3 exist or can be created at PB time. This preprocessor switch is required to ensure all needed macros exist. */
      else
      {
        /* #300 If the received CAN XL Rx PDU has SDU type other than 3 */
        CanIf_RxPduConfigIterType canXLRxPduId = CanIf_GetRxPduConfigCanXLRxPdusSdtNot3StartIdxOfCtrlConfig(Mailbox->ControllerId);

        /* #310 Iterate over configured CAN XL Rx PDUs of SDU type other than 3 */
        for (; canXLRxPduId < CanIf_GetRxPduConfigCanXLRxPdusSdtNot3EndIdxOfCtrlConfig(Mailbox->ControllerId); canXLRxPduId++)
        {
          CanIf_RxPduCanXLPropsIdxOfRxPduConfigType canXLRxPduPropIdx = CanIf_GetRxPduCanXLPropsIdxOfRxPduConfig(canXLRxPduId);

          /* #320 Compare the received CAN XL parameters with the configured CAN XL parameters */
          if ((CanIf_GetSduTypeOfRxPduCanXLProps(canXLRxPduPropIdx) == Mailbox->XLParams->SduType)
            && (CanIf_GetAcceptanceFieldOfRxPduCanXLProps(canXLRxPduPropIdx) == Mailbox->XLParams->AcceptanceField)
            && (CanIf_GetVcidOfRxPduCanXLProps(canXLRxPduPropIdx) == Mailbox->XLParams->Vcid)
            && (CanIf_GetPriorityIdOfRxPduCanXLProps(canXLRxPduPropIdx) == Mailbox->XLParams->PriorityId))
          {
            /* #330 Pass the received CAN XL Rx PDU to upper layer */
            /* "0" is used as CAN ID, but will never be reported to the upper layer, since only indications without CAN ID are allowed for CAN XL Rx PDUs with SDU type other than 3 (ensured by validation). */
            CanIf_Indirect_HlIndication(Mailbox->ControllerId, (PduIdType)canXLRxPduId, PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength, 0x0, CANIF_XLRXINDICATION_API);
            break;
          }
        }
      }
#  endif
    }
# endif
  }
  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_XLRXINDICATION_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif


#if (CANIF_RXPDUCONFIGCANXLRXPDUSSDT3USEDOFCTRLCONFIG == STD_ON)  /* COV_CANIF_SUPPORT_CAN_XL */ /* True, if CAN XL Rx PDUs of SDU type 3 exist or can be created at PB time */
/**********************************************************************************************************************
*  CanIf_XLRxIndicationSubCanIdMatchesConfiguredPdu()
*********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_XLRxIndicationSubCanIdMatchesConfiguredPdu
Relation_Context:
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
CANIF_LOCAL_INLINE FUNC(boolean, CANIF_CODE) CanIf_XLRxIndicationSubCanIdMatchesConfiguredPdu(CanIf_RxPduConfigIterType canXLRxPduId, Can_IdType canId, CanIf_MsgType msgType)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
# if (CANIF_RX_SEARCH_CONSIDER_MSG_TYPE == STD_ON)
  /* #100 Compare the message type (only if configured) */
  if ((CANIF_CFG_RX_MSGTYPE(canXLRxPduId) == CANIF_MSG_TYPE_CAN) || (CANIF_CFG_RX_MSGTYPE(canXLRxPduId) == msgType))
# else
  CANIF_DUMMY_STATEMENT(msgType); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  {
    /* #110 Determine if the configured CAN XL Rx PDU has a CAN ID range or a single CAN ID */
    if ((CANIF_CFG_RX_MASK(canXLRxPduId) & CANIF_RANGE_FLAG) == CANIF_RANGE_FLAG)
    {
      /* #120 Determine if the configured CAN XL Rx PDU has a code/mask or lower/upper bound CAN ID range */
      if ((CANIF_CFG_RX_CANID(canXLRxPduId) & CANIF_MASKCODE_RANGE_FLAG) == CANIF_MASKCODE_RANGE_FLAG)
      {
        /* #130 Determine if the CAN ID matches the code/mask CAN ID range */
        if (CANIF_CFG_RX_CANID(canXLRxPduId) == ((canId & (CANIF_CFG_RX_MASK(canXLRxPduId))) | CANIF_MASKCODE_RANGE_FLAG))
        {
          retVal = TRUE;
        }
      }
      /* #140 Determine if the CAN ID matches the lower/upper bound CAN ID range */
      else
      {
        if ((canId >= CANIF_CFG_RX_CANID(canXLRxPduId)) && (canId <= (CANIF_CFG_RX_MASK(canXLRxPduId) & CANIF_STOP_RANGE_FLAG_MASKOUT)))
        {
          retVal = TRUE;
        }
      }
    }
    /* #150 Determine if the CAN ID matches the single CAN ID */
    else
    {
      if ((CANIF_CFG_RX_CANID(canXLRxPduId) == canId))
      {
        retVal = TRUE;
      }
    }
  }
  return retVal;
}
#endif


/**********************************************************************************************************************
 *  CanIf_HlIndicationSubULCall()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_HlIndicationSubULCall
Relation_Context:
CanIfRxIndicationUsedTypeIII, CanIfRxIndicationUsedTypeIV # from this local function #
CanIfRxIndicationUsedTypeIII, CanIfRxIndicationUsedTypeIV # from caller CanIf_HlIndication #
Relation:
CanIfRxIndicationUsedTypeI
CanIfRxIndicationUsedTypeIII
CanIfRxIndicationUsedTypeIV
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
#if (CANIF_SUPPORT_NMOSEK_INDICATION == STD_ON) || (CANIF_RX_INDICATION_TYPE_IV_IS_USED == STD_ON)
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_HlIndicationSubULCall(PduIdType PduId, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR) CanSduPtr, PduLengthType CanDlc, Can_IdType CanId)
#else
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
CANIF_LOCAL_INLINE FUNC(void, CANIF_CODE) CanIf_HlIndicationSubULCall(PduIdType PduId, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR) CanSduPtr, PduLengthType CanDlc)
#endif
{
  /* ----- Local Variables ---------------------------------------------- */
  CanIf_RxIndFctListIdxType index = CANIF_CFG_RX_RXINDICATION(PduId);

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Report received message to upper layer */
#if (CANIF_RX_INDICATION_TYPE_I_IS_USED == STD_ON)
  if (CANIF_CFG_RXINDICATION_FUNCTIONLAYOUT(index) == CanIf_SimpleRxIndicationLayout)
  {
    /* #200 CanIf_SimpleRxIndicationLayout API */
    CanIf_SimpleRxIndicationFctType rxIndicationFct;
    rxIndicationFct = CANIF_CFG_RXINDICATION_FUNCTION(index).eSimpleRxIndicationType;
    if (rxIndicationFct != NULL_PTR)
    {
      /* \trace SPEC-1885 */
      /* CA cast (-> to get the const ) */
      /* CANIF_APPL_VAR -> located in CAN Driver */
      rxIndicationFct(CANIF_CFG_RX_UPPERPDUID(PduId), (P2CONST(uint8, AUTOMATIC, CANIF_UL_STANDARD_VAR))CanSduPtr); /* SBSW_CANIF_12 */
    }
  } 
  else
#endif

#if (CANIF_SUPPORT_NMOSEK_INDICATION == STD_ON)
  if (CANIF_CFG_RXINDICATION_FUNCTIONLAYOUT(index) == CanIf_NmOsekRxIndicationLayout)
  {
    /* #300 CanIf_NmOsekRxIndicationLayout API */
    CanIf_NmOsekRxIndicationFctType rxIndicationFct;
    rxIndicationFct = CANIF_CFG_RXINDICATION_FUNCTION(index).eNmOsekRxIndicationType;
    if (rxIndicationFct != NULL_PTR)
    {
      /* \trace SPEC-1841, SPEC-1885 */
      /* CA cast (-> to get the const ) */
      /* CANIF_APPL_VAR -> located in CAN Driver */
      rxIndicationFct(CANIF_CFG_RX_UPPERPDUID(PduId), (P2CONST(uint8, AUTOMATIC, CANIF_UL_OSEKNM_VAR))CanSduPtr, CanId); /* SBSW_CANIF_12 */
    }
  }
  else
#endif

#if (CANIF_RX_INDICATION_TYPE_IV_IS_USED == STD_ON)
  if (CANIF_CFG_RXINDICATION_FUNCTIONLAYOUT(index) == CanIf_CddRxIndicationLayout)
  {
    /* #400 CanIf_CddRxIndicationLayout API */
    CanIf_CddRxIndicationFctType rxIndicationFct;
    PduInfoType pduinfo;
    /* cast from CANIF_CBK_DRV_VAR to AUTOSAR_COMSTACKDATA --> has to be resolved in compiler abstraction configuration */
    /* const to non-const cast is not avoidable */
    pduinfo.SduDataPtr = (P2VAR(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA))CanSduPtr; /* PRQA S 0311 */ /* MD_CanIf_Rule11.8 */
    /* CANIF262: Currently received DLC value is passed to upper layer */
    pduinfo.SduLength = CanDlc;

    rxIndicationFct = CANIF_CFG_RXINDICATION_FUNCTION(index).eCddRxIndicationType;
    if (rxIndicationFct != NULL_PTR)
    {
      /* \trace SPEC-1885 */
      /* CA cast (-> to get the const ) */
      /* CANIF_APPL_VAR -> located in CAN Driver */
      rxIndicationFct(CANIF_CFG_RX_UPPERPDUID(PduId), (P2CONST(PduInfoType, AUTOMATIC, CANIF_UL_ADVANCED_VAR))&pduinfo, CanId); /* SBSW_CANIF_12 */
    }
  } 
  else
#endif

  if (CANIF_CFG_RXINDICATION_FUNCTIONLAYOUT(index) == CanIf_AdvancedRxIndicationLayout) /* PRQA S 2004 */ /* MD_MSR_EmptyClause */
  {
    /* #500 CanIf_AdvancedRxIndicationLayout API */
    CanIf_AdvancedRxIndicationFctType rxIndicationFct;
    PduInfoType pduinfo;
    /* cast from CANIF_CBK_DRV_VAR to AUTOSAR_COMSTACKDATA --> has to be resolved in compiler abstraction configuration */
    /* const to non-const cast is not avoidable */
    pduinfo.SduDataPtr = (P2VAR(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA))CanSduPtr; /* PRQA S 0311 */ /* MD_CanIf_Rule11.8 */
    /* CANIF262: Currently received DLC value is passed to upper layer */
    pduinfo.SduLength = CanDlc;

    rxIndicationFct = CANIF_CFG_RXINDICATION_FUNCTION(index).eAdvancedRxIndicationType;
    if (rxIndicationFct != NULL_PTR)
    {
      /* \trace SPEC-1841, SPEC-1885 */
      /* CA cast (-> to get the const ) */
      /* PTR to Stack -> AUTOMATIC */
      rxIndicationFct(CANIF_CFG_RX_UPPERPDUID(PduId), (P2CONST(PduInfoType, AUTOMATIC, CANIF_UL_ADVANCED_VAR))&pduinfo); /* SBSW_CANIF_12 */
    }
  }
}


/**********************************************************************************************************************
 *  CanIf_HlIndication()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_HlIndication
Relation_Context:
CanIfRxIndicationUsedTypeIII, CanIfRxMetaDataSupport, CanIfRxIndicationUsedTypeIV # from this local function #
CanIfRxIndicationUsedTypeIII, CanIfRxMetaDataSupport, CanIfRxIndicationUsedTypeIV # from caller CanIf_RxIndicationAsr403 #
Relation:
CanIfDevErrorDetect
CanIfWakeupValidation
CanIfPnEnable
CanIfDlcCheck
CanIfRxMetaDataSupport
CanIfRxMetaDataSupport, CanIfIDType
CanIfDevErrorDetect
CanIfRxIndicationUsedTypeIII, CanIfRxIndicationUsedTypeIV
CanIfRxIndicationUsedTypeIII, CanIfRxIndicationUsedTypeIV, CanIfRxMetaDataSupport, CanIfFdSupport # used because RxMetaData is depending of Fd #
CanIfRxMetaDataSupport
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
#if ((CANIF_SUPPORT_NMOSEK_INDICATION == STD_ON) || (CANIF_META_DATA_RX_SUPPORT == STD_ON) || (CANIF_RX_INDICATION_TYPE_IV_IS_USED == STD_ON))
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
CANIF_LOCAL FUNC(void, CANIF_CODE) CanIf_HlIndication(uint8 ControllerId, PduIdType PduId, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR) CanSduPtr, PduLengthType CanDlc, Can_IdType CanId, uint8 ServiceIdOfCaller)
#else
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
CANIF_LOCAL FUNC(void, CANIF_CODE) CanIf_HlIndication(uint8 ControllerId, PduIdType PduId, P2CONST(uint8, AUTOMATIC, CANIF_CBK_DRV_VAR) CanSduPtr, PduLengthType CanDlc, uint8 ServiceIdOfCaller)
#endif
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if CAN controller is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_CONFIG;
  }
  /* #20 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-3718, SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* \trace SPEC-3711 */
    /* #100 Process only if CAN controller is active */
    if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_STARTED)
    {
#if (CANIF_WAKEUP_VALIDATION == STD_ON)
# if (CANIF_WAKEUP_VALID_ALL_RX_MSGS != STD_ON)
     /* CANIF741 */
#  if (CANIF_WAKEUP_VALID_ONLY_NM_RX_MSGS == STD_ON)
      if(CANIF_ISCANNMRXPDU(PduId)) /* Check if RxPdu is from CAN-Nm to validate wakeup */
#  endif
      {
        /* \trace SPEC-21407 */ /* Only ECUs receive list will validate an Rx event / Controller mode: Started, PduMode: Don't care */
        /* #110 Set wake-up validation state to DETECTED (only if configured) */
        CanIf_SetWakeUpValidationState(ControllerId, CANIF_WUVALIDATION_DETECTED); /* SBSW_CANIF_3 */
      }
# endif
#endif

      if ( (CANIF_VAR_GETPDUMODE(ControllerId) == CANIF_GET_ONLINE) || /* \trace SPEC-1849, SPEC-1850 */
           (CANIF_VAR_GETPDUMODE(ControllerId) == CANIF_GET_RX_ONLINE) || /* \trace SPEC-3855 */
           (CANIF_VAR_GETPDUMODE(ControllerId) == CANIF_GET_OFFLINE_ACTIVE_RX_ONLINE)
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
        || (CANIF_VAR_GETPDUMODE(ControllerId) == CANIF_GET_ONLINE_WAKF)
#endif
         )
      {
#if (CANIF_DLC_CHECK == STD_ON)
        /* #120 Check if parameter CanDlc is valid */
        /* DLC check - if configured DLC == 0 the following condition is always false and DLC check is deactivated */
        /* \trace SPEC-1846, SPEC-3672 */
        if (CanDlc < CANIF_CFG_RX_DLC(PduId))
        {
          /* \trace SPEC-1900 */
          /* Notification to DET */
          errorId = CANIF_E_INVALID_DLC;
        } 
        else 
        /* \trace SPEC-1964 */
#endif
        {
#if (CANIF_META_DATA_RX_SUPPORT == STD_ON)
          /* #130 Handle meta data of Rx-PDU: Copy payload of received Rx-PDU into local buffer and add CAN identifier or parts of it to it (only if meta data Rx-PDU) */
          if ((CANIF_CFG_RX_RXMETADATALENGTH(PduId) > 0u) && ((CanDlc + CANIF_CFG_RX_RXMETADATALENGTH(PduId)) > CANIF_CFG_MAXRXDLC_PLUS_MAXMETADATALEN))
          {
            errorId = CANIF_E_INVALID_DLC_METADATA;
          }
          else
          {
            if(CANIF_CFG_RX_RXMETADATALENGTH(PduId) > 0u)
            {
              /* cast required for CanDlc, because PduLengthType can be greater than uint8_least (no data loss, as DLC here is max. 64) */
              uint8_least metaDataWriteIndex = (uint8_least)CanDlc; /* meta data (max. 4 bytes) will be inserted after the actual payload */
              uint8 CanSduLocal[CANIF_CFG_MAXRXDLC_PLUS_MAXMETADATALEN];

              /* copy received data on local data buffer */
              /* cast required for CanDlc, because PduLengthType can be greater than VStdLib_CntType (no data loss, as DLC here is max. 64) */
              VStdLib_MemCpy(CanSduLocal, CanSduPtr, (VStdLib_CntType)CanDlc); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_CANIF_7 */

# if (CANIF_EXTENDEDID_SUPPORT == STD_ON)
              {
                Can_IdType CanIdLocal = CanId;
                uint8_least metaDataPosition = 0;
                for (; metaDataPosition < CANIF_CFG_RX_RXMETADATALENGTH(PduId); metaDataPosition++) /* FETA_CANIF_8 */
                {
                  CanSduLocal[metaDataWriteIndex] = (uint8)(CanIdLocal & 0x000000FFu); /* SBSW_CANIF_16 */
                  CanIdLocal = CanIdLocal >> 8;
                  metaDataWriteIndex++;
                }
              }
# else
              {
                Can_IdType CanIdLocal = CanId;
                uint8_least metaDataPosition = 0;
                for (; metaDataPosition < CANIF_CFG_RX_RXMETADATALENGTH(PduId); metaDataPosition++) /* FETA_CANIF_8 */
                {
                  if (metaDataPosition < 2u) /* MetaDataLength is max 4 */
                  {
                    CanSduLocal[metaDataWriteIndex] = (uint8)(CanIdLocal & 0x00FFu); /* SBSW_CANIF_16 */
                    CanIdLocal = CanIdLocal >> 8u;
                  }
                  else
                  {
                    CanSduLocal[metaDataWriteIndex] = 0x00; /* SBSW_CANIF_16 */
                  }
                  metaDataWriteIndex++;
                }
              }
# endif /* CANIF_EXTENDEDID_SUPPORT == STD_ON */

              CanSduPtr = CanSduLocal; /* PRQA S 1338 */ /* MD_CanIf_Rule17.8 */

              /* increment DLC */
              /* cast required for metaDataWriteIndex, because uint8_least can be greater than PduLengthType (no data loss, as DLC here is max. 68) */
              CanDlc = (PduLengthType)metaDataWriteIndex; /* PRQA S 1338 */ /* MD_CanIf_Rule17.8 */
            }
#endif /* CANIF_META_DATA_RX_SUPPORT == STD_ON */

            /* #140 Check if Rx-indication function is valid */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
            if ( CANIF_CFG_RX_RXINDICATION(PduId) >= CANIF_CFG_MAX_RXINDICATIONS )
            {
              errorId = CANIF_E_CONFIG;
            } 
            else
#endif
            {
              /* #150 Report received message to upper layer */
#if (CANIF_SUPPORT_NMOSEK_INDICATION == STD_ON) || (CANIF_RX_INDICATION_TYPE_IV_IS_USED == STD_ON)
# if (CANIF_META_DATA_RX_SUPPORT == STD_ON)
              /* #160 Remove FD-flag from CAN identifier */
              CanId = (CanId & CANIF_FD_FLAG_MASKOUT); /* The FD Flag is no part of the CAN ID */ /* PRQA S 1338 */ /* MD_CanIf_Rule17.8 */
# endif
              CanIf_HlIndicationSubULCall(PduId, CanSduPtr, CanDlc, CanId); /* SBSW_CANIF_5 */
#else
              CanIf_HlIndicationSubULCall(PduId, CanSduPtr, CanDlc); /* SBSW_CANIF_5 */
#endif  
            }
#if (CANIF_META_DATA_RX_SUPPORT == STD_ON)
          }
#endif
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(ServiceIdOfCaller, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(ServiceIdOfCaller); /* PRQA S 1338, 2983 */ /* MD_MSR_DummyStmt */
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

}


/* \trace SPEC-1912, SPEC-3725 */
/**********************************************************************************************************************
 *  CanIf_ControllerBusOff()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ControllerBusOff
Relation_Context:
Relation:
CanIfSecurityEventReporting, CanIfSecurityEventBusOff
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfTxBuffer
CanIfTxConfPollSupport
CanIfSecurityEventReporting, CanIfSecurityEventBusOff
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_ControllerBusOff(uint8 ControllerId)
{
  /* conform to requirement \trace SPEC-1877 */

  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  CanIf_BusOffNotificationFctType busoffnotificationFct = CANIF_CFG_BUSOFFNOTIFICATION();

#if ((CANIF_ENABLE_SECURITY_EVENT_REPORTING == STD_ON) && defined (CANIF_SEV_ERRORSTATE_BUSOFF))
  IdsM_ContextDataType contextData[CANIF_CONTEXTDATA_SIZE_BUSOFF];
#endif

  /* ----- Development Error Checks ------------------------------------- */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
# if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    /* \trace SPEC-3717 */
    errorId = CANIF_E_PARAM_CONTROLLER;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #30 Verify if affected CAN controller is initialized */
    /* \trace SPEC-3705 */
    if (CanIf_GetCtrlMode(ControllerId) != CANIF_CS_UNINIT)
    {
      /* #100 Deactivate the communication via the affected CAN controller (controller and PDU mode) */
      /* ESCAN00051423, avoids ESCAN00051329 */
      /* Explicitely cast to void as return value cannot be used here */
      (void)CanIf_SetPduMode(ControllerId, CANIF_SET_TX_OFFLINE);

      /* Set CAN Interface to Stopped mode, CAN Controller is BusOff */
      /* ASR4 CANController is already in STOP -> only internal variable must be set */
      /* \trace SPEC-3761, SPEC-1965 */
      CanIf_SetCtrlMode(ControllerId, CANIF_CS_STOPPED); /* SBSW_CANIF_3 */

#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
      /* #110 Clear the Tx-queue */
      CanIf_ClearQueue(ControllerId);
#endif

#if (CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT == STD_ON)
      /* #120 Reset the Tx-confirmation state */
      /* \trace SPEC-9980 */
      /* TxConfirmationState is reset always when CAN controller is stopped
      * - BusOff
      * - Going to _SLEEP
      * - CanIf_InitController() is called */
      CanIf_SetTxConfState(ControllerId, CANIF_NO_NOTIFICATION); /* SBSW_CANIF_3 */
#endif

      /* #130 Verify if notification function to upper layer is configured */
      if (busoffnotificationFct != NULL_PTR)
      {
        /* #140 Call BusOff notification function */
        /* \trace SPEC-5346 */
        busoffnotificationFct(ControllerId); /* SBSW_CANIF_13 */
      }

#if ((CANIF_ENABLE_SECURITY_EVENT_REPORTING == STD_ON) && defined (CANIF_SEV_ERRORSTATE_BUSOFF))
      /* #200 Report security event to the IdsM with ControllerId in context data */
      contextData[CANIF_CONTEXTDATA_CTRLID_POS] = ControllerId; /* SBSW_CANIF_49 */
      IdsM_SetSecurityEventWithContextData(CANIF_SEV_ERRORSTATE_BUSOFF, contextData, CANIF_CONTEXTDATA_SIZE_BUSOFF); /* SBSW_CANIF_5 */
#endif
    }
    else
    {
      /* \trace SPEC-3709, SPEC-5238 */
      errorId = CANIF_E_UNINIT;
    }
  }

  /* ----- Development Error Report ------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CONTROLLERBUSOFF_API, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}


/* \trace SPEC-1832 */
/**********************************************************************************************************************
 *  CanIf_SetPduMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_SetPduMode
Relation_Context:
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfPnEnable
CanIfTxBuffer
CanIfPnEnable
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetPduMode(uint8 ControllerId, CanIf_PduSetModeType PduModeRequest)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  uint8 errorId = CANIF_E_NO_ERROR; 

  /* ----- Development Error Checks ------------------------------------- */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
# if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    /* \trace SPEC-3537 */
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  /* #40 Check if PduModeRequest is valid */
  else if(PduModeRequest >= CANIF_NO_MODE_PDU_SET_MODE_TYPE_END)
  {
    errorId = CANIF_E_PARAM_PDU_MODE;
  }
  else
#endif
  {
    /* ----- Implementation --------------------------------------------- */
    /* #100 Change PDU mode only if CAN controller is in STARTED mode */
    if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_STARTED)
    {
      /* #110 Process PDU mode change within critical section */
      retval = E_OK;
      CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_3);

      /* #120 Do allow PDU mode transitions only if required */
      if(CanIfState[CANIF_VAR_GETPDUMODE(ControllerId)][PduModeRequest] != CANIF_NOP)
      {
        switch (PduModeRequest)
        {
          case CANIF_SET_OFFLINE:
          case CANIF_SET_TX_OFFLINE:
          case CANIF_SET_TX_OFFLINE_ACTIVE:
            switch(CANIF_VAR_GETPDUMODE(ControllerId))
            {
              case CANIF_GET_TX_ONLINE:
              case CANIF_GET_ONLINE:
              /* \trace SPEC-1849, SPEC-3765 */
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON) /* ESCAN00057578 */
              case CANIF_GET_TX_ONLINE_WAKF:
              case CANIF_GET_ONLINE_WAKF:
#endif
#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
                /* #130 If Tx-direction is deactivated clear the Tx-queue */
                CanIf_ClearQueue(ControllerId);
#endif
                break;
              default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
                break;
            }
            /* #140 In case of any WAKF-mode is left disable the wake-up Tx-filter */
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
            CanIf_SetPnWuTxPduFilterActive(ControllerId, FALSE); /* SBSW_CANIF_3 */
#endif
            break;
          case CANIF_SET_ONLINE:
          case CANIF_SET_TX_ONLINE:
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
            CanIf_SetPnWuTxPduFilterActive(ControllerId, FALSE); /* SBSW_CANIF_3 */
#endif
            break;
#if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
          case CANIF_SET_ONLINE_WAKF:
          case CANIF_SET_TX_ONLINE_WAKF:
            /* CANIF752 */
            /* #150 In case of WAKF-mode is requested AND the corresponding controller requires the Tx-filter enable it */
            if(CANIF_CFG_ISFEATUREPNWUTXPDUFILTERENABLED(ControllerId)) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
            {
              /* CANIF749 */
              CanIf_SetPnWuTxPduFilterActive(ControllerId, TRUE); /* SBSW_CANIF_3 */
            }
            break;
#endif
          default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
            break;
        }
        /* Get the next valid state by reading the table: CanIfState */
        /* #160 Determine and set the requested PDU-mode */
        CANIF_VAR_SETPDUMODE(ControllerId, (CanIf_PduGetModeType)CanIfState[CANIF_VAR_GETPDUMODE(ControllerId)][PduModeRequest]); /* PRQA S 4342 */ /* MD_CanIf_Rule10.5_4342 */ /* SBSW_CANIF_3 */
      }
      CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_3);
    }
  }

  /* ----- Development Error Report ------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_SETPDUMODE_API, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retval;
}


/* \trace SPEC-1835 */
/**********************************************************************************************************************
 *  CanIf_GetPduMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_GetPduMode
Relation_Context:
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetPduMode(uint8 ControllerId, P2VAR(CanIf_PduGetModeType, AUTOMATIC, CANIF_APPL_STATE_VAR) PduModePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
# if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    /* \trace SPEC-3555 */
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  /* #40 Check if parameter PduModePtr is valid */
  else if (PduModePtr == NULL_PTR)
  {
    /* \trace SPEC-5236 */
    errorId = CANIF_E_PARAM_POINTER;
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation --------------------------------------------- */
    /* #100 Get the current PDU mode */
    *PduModePtr = CANIF_VAR_GETPDUMODE(ControllerId); /* SBSW_CANIF_9 */
    retval = E_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_GETPDUMODE_API, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retval;
}


#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
/**********************************************************************************************************************
 *  CanIf_TxQueueTransmit()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TxQueueTransmit
Relation_Context:
CanIfTxBuffer # from this local function #
CanIfTxBuffer # from caller CanIf_TxQueueTreatment #
Relation:
CanIfDevErrorDetect
CanIfTxBuffer
CanIfTxBuffer, CanIfDynTx, CanIfJ1939, CanIfTxMetaDataSupport
CanIfTxBuffer, CanIfFdSupport # used for CANIF_STATIC_FD_TXQUEUE #
CanIfTxBuffer
CanIfTxBuffer, CanIfDynTx, CanIfJ1939, CanIfTxMetaDataSupport
CanIfTxBuffer
CanIfTxBuffer, CanIfTransmitCancellationAPI
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
CANIF_LOCAL FUNC(Std_ReturnType, CANIF_CODE) CanIf_TxQueueTransmit( PduIdType PduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  CanIf_CanDrvReturnType txResult = CANIF_CANDRV_NOT_OK;
  Std_ReturnType retval = E_NOT_OK;
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter PduId is valid. */
  if (PduId >= CANIF_CFG_MAX_TXPDUS)
  {
    /* \trace SPEC-2007 */
    errorId = CANIF_E_INVALID_TXPDUID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    const CanIf_AnyIdxType hth = CANIF_CFG_TX_HTH(PduId);
    /* #100 Prepare the Tx PDU (CAN identifier, DLC, payload) and pass the Tx request to CAN Driver */
    switch (CANIF_CFG_MAILBOX_TXBUFFERHANDLINGTYPE(hth))
    {
# if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
      case CANIF_TXBUFFER_HANDLINGTYPE_PRIOBYCANID:
        {
           Can_PduType localPdu; /* Fully initialized within the code below before passing to CanIf_CallCanWrite() */

#  if((CANIF_SETDYNAMICTXID_API == STD_ON) || (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED) || (CANIF_META_DATA_TX_SUPPORT == STD_ON))
          (localPdu).id = CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUE(CANIF_CFG_TX_QUEUEINDEX(PduId), eBaseParams.eCanId);
#  else 
          (localPdu).id = CANIF_CFG_TX_CANID(PduId);
#  endif
          (localPdu).length = (uint8)CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUE(CANIF_CFG_TX_QUEUEINDEX(PduId), eBaseParams.eSduLength);
#  if(CANIF_STATIC_FD_TXQUEUE == STD_ON)
          (localPdu).sdu = CANIF_ADDR_VAR_TXBUFFERPRIOBYCANID_STATFDQUEUE_QUEUEDATA(CANIF_CFG_TXBUFFERPRIOBYCANID_STATFDQUEUE_DATASTARTIDX(PduId));
#  else
          (localPdu).sdu = CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUE(CANIF_CFG_TX_QUEUEINDEX(PduId), eSdu);
#  endif
          (localPdu).swPduHandle = (PduId);

          txResult = CanIf_CallCanWrite(&localPdu); /* SBSW_CANIF_55 */
        }
        break;
# endif

# if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
      case CANIF_TXBUFFER_HANDLINGTYPE_FIFO:
        {
          Can_PduType localPdu; /* Fully initialized within the code below before passing to CanIf_CallCanWrite() */
          const CanIf_AnyIdxType txBufferCfgIdx = CANIF_CFG_MAILBOX_TXBUFFERIDX(hth);
          const CanIf_AnyIdxType txBufferFifoBaseIdx = CANIF_CFG_TXBUFFERFIFO_BUFFERBASEIDX(txBufferCfgIdx);
          const CanIf_AnyIdxType txFifoQueueBaseStartIdx = CANIF_CFG_TXBUFFERFIFO_QUEUEBASESTARTIDX(txBufferCfgIdx);

#  if((CANIF_SETDYNAMICTXID_API == STD_ON) || (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED) || (CANIF_META_DATA_TX_SUPPORT == STD_ON))
          (localPdu).id = CANIF_VAR_TXBUFFERFIFO_QUEUEBASE( (CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx  ) + txFifoQueueBaseStartIdx ), eBaseParams.eCanId );
#  else
          (localPdu).id = CANIF_CFG_TX_CANID(PduId);
#  endif
          (localPdu).length = CANIF_VAR_TXBUFFERFIFO_QUEUEBASE( (CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx  ) + txFifoQueueBaseStartIdx ), eBaseParams.eSduLength );
          (localPdu).sdu = CANIF_ADDR_VAR_TXBUFFERFIFO_QUEUEDATA((CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx) * (uint32)CANIF_CFG_TXBUFFERFIFO_SIZEOFONEPAYLOADEL(txBufferCfgIdx)) + CANIF_CFG_TXBUFFFERFIFO_QUEUEDATASTARTIDX(txBufferCfgIdx));
          (localPdu).swPduHandle = (PduId);

          txResult = CanIf_CallCanWrite(&localPdu); /* SBSW_CANIF_56 */
        }
        break;
# endif

      default: /* COV_CANIF_GENDATA_FAILURE */
        /* It is not possible that the default case is reached, because this local function is only called from CanIf_TxQueueTreatment()
           for hths which have a valid Tx buffer type. */
        /* txResult default CANIF_CANDRV_NOT_OK */
        break;
    }

    if (txResult == CANIF_CANDRV_OK)
    {
      /* \trace SPEC-1897 */
      /* #110 If the Tx request succeeds, remove the Tx PDU from Tx buffer */
      switch (CANIF_CFG_MAILBOX_TXBUFFERHANDLINGTYPE(hth))
      {
# if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
        case CANIF_TXBUFFER_HANDLINGTYPE_PRIOBYCANID:
          (void)CanIf_ClearPrioByCanIdTxPduAsQueued(PduId);
          retval = E_OK;
          break;
# endif

# if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
        case CANIF_TXBUFFER_HANDLINGTYPE_FIFO:
          {
            const CanIf_AnyIdxType txBufferCfgIdx = CANIF_CFG_MAILBOX_TXBUFFERIDX(hth);
            const CanIf_AnyIdxType  txBufferFifoBaseIdx = CANIF_CFG_TXBUFFERFIFO_BUFFERBASEIDX(txBufferCfgIdx);
#  if (CANIF_CANCEL_SUPPORT_API == STD_ON)
            CANIF_VAR_TXBUFFERFIFO_QUEUEBASE(( CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx) + CANIF_CFG_TXBUFFERFIFO_QUEUEBASESTARTIDX( txBufferCfgIdx)), eQueued) = 0; /* SBSW_CANIF_38 */
            CANIF_VAR_SET_TXBUFFERFIFO_QUEUEIDX2FIFOEL(CANIF_CFG_TX_QUEUEINDEX(PduId), CANIF_NO_ENTRY_IN_TX_FIFO); /* SBSW_CANIF_36 */
#  endif
            CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx)++; /* SBSW_CANIF_37 */
            if(CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx) == CANIF_CFG_TXBUFFERFIFO_QUEUESIZE(txBufferCfgIdx)) 
            {
              CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx) = 0; /* SBSW_CANIF_37 */
            }
            CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eQueueCounter)--; /* SBSW_CANIF_37 */
          }
          retval = E_OK;
          break;
# endif

        default: /* COV_CANIF_GENDATA_FAILURE */
          /* It is not possible that the default case is reached, because this local function is only called from CanIf_TxQueueTreatment()
             for hths which have a valid Tx buffer type. */
          /* retval default E_NOT_OK */
          errorId = CANIF_E_CONFIG;
          break;
      }
    }
    else if (txResult == CANIF_CANDRV_BUSY)
    {
      /* Also ok due to confirmation queue handling may be interrupted by CanIf_Transmit() or just a cancel request */
      retval = E_OK;
    }
    else /* txResult == CANIF_CANDRV_NOT_OK */
    {
      errorId = CANIF_E_TX_BUFFER_TRANSMIT;
      /* retval default E_NOT_OK */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CANCELTRANSMIT_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif


#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
/**********************************************************************************************************************
 *  CanIf_TxQueueTreatment()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TxQueueTreatment
Relation_Context:
CanIfTxBuffer # from this local function #
CanIfTxBuffer, CanIfTransmitCancellationAPI # from caller CanIf_CancelTxConfirmation, CanIf_CancelTxNotification #
Relation:
CanIfTxBuffer
CanIfTxBuffer, CanIfTransmitCancellationAPI
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
CANIF_LOCAL FUNC(Std_ReturnType, CANIF_CODE) CanIf_TxQueueTreatment(CanIf_HwHandleType hth)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  const CanIf_AnyIdxType txBufferCfgIdx = CANIF_CFG_MAILBOX_TXBUFFERIDX(hth);

  /* ----- Implementation ----------------------------------------------- */
  /* #100 To which type of buffer belongs the HTH to? */
  switch (CANIF_CFG_MAILBOX_TXBUFFERHANDLINGTYPE(hth))
  {
# if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
    case CANIF_TXBUFFER_HANDLINGTYPE_PRIOBYCANID:
      {
        /* #200 PRIO_BY_CANID handling */
        const CanIf_AnyIdxType txBufferPrioByCanIdBaseIdx = CANIF_CFG_TXBUFFERPRIOBYCANID_BUFFERBASEIDX(txBufferCfgIdx);
         /* #210 Anything buffered? */ 
        if (CanIf_GetTxQueueCounter(txBufferPrioByCanIdBaseIdx) != 0u)
        {
          /* Stores the end-condition of the following for-loop (improves performance) */
          const sint16_least queueEndElementIdx = (sint16_least)(CANIF_CFG_TXBUFFERPRIOBYCANID_QUEUESTARTIDX(txBufferCfgIdx));
          /* Process from last to first */
          sint16_least queueElementIdx; /* for-loop counter */

          /* #220 Compute the next buffered PDU ID with the highest priority */
          for(queueElementIdx = (((sint16_least)CANIF_CFG_TXBUFFERPRIOBYCANID_QUEUESTOPIDX(txBufferCfgIdx)) - (sint16_least)1); queueElementIdx >= queueEndElementIdx; queueElementIdx--) /* FETA_CANIF_9 */
          {
            const CanIf_TxQueueFlagsType elem = CANIF_VAR_TXBUFFERPRIOBYCANID_QUEUEFLAGS((uint16_least)queueElementIdx);

            if(elem != (CanIf_TxQueueFlagsType)0u) /* Is there any flag set in the queue element? / Avoids the return value VSTDLIB_BITPOS_NA from VStdLib_GetHighestBitPosOne8/16/32/64() */
            {
              /* Get bit position from highest set bit (equals buffered Tx-PDU with highest priority) */
#   if (CPU_TYPE == CPU_TYPE_8)
              const uint8 elementBitIdx = VStdLib_GetHighestBitPosOne8(elem);
#   elif (CPU_TYPE == CPU_TYPE_16)
              const uint8 elementBitIdx = VStdLib_GetHighestBitPosOne16(elem);
#   elif (CPU_TYPE == CPU_TYPE_32)
              const uint8 elementBitIdx = VStdLib_GetHighestBitPosOne32(elem);
#   else /* CPU_TYPE_64 */
              const uint8 elementBitIdx = VStdLib_GetHighestBitPosOne64(elem);
#   endif
              /* Calculate PDU-Id from found bit position */
              const PduIdType PduId = ((((PduIdType)queueElementIdx - (PduIdType)queueEndElementIdx) << CanIf_TxQueueBitShift) + (PduIdType)elementBitIdx) + (PduIdType)CANIF_CFG_TXBUFFERPRIOBYCANID_BITPOS2TXPDUIDOFFSET(txBufferCfgIdx);
              /* #230 Call CanIf_TxQueueTransmit() for the corresponding Tx PDU ID */
              retval = CanIf_TxQueueTransmit(PduId);
              break;
            }
          }
        }
      }
      break;
# endif

# if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
    case CANIF_TXBUFFER_HANDLINGTYPE_FIFO:
      {
        /* #400 FIFO buffer handling */
        const CanIf_AnyIdxType txBufferFifoBaseIdx = CANIF_CFG_TXBUFFERFIFO_BUFFERBASEIDX(txBufferCfgIdx);

        /* #410 Anything buffered? */
        if (CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eQueueCounter) != 0u)
        {
          const CanIf_AnyIdxType txBufferFifoQueueBaseStartIdx = CANIF_CFG_TXBUFFERFIFO_QUEUEBASESTARTIDX(txBufferCfgIdx);

#  if (CANIF_CANCEL_SUPPORT_API == STD_ON)
          /* #420 Some entries in CanIf_TxFifoQueueBase may be cancelled */
          CanIf_TxBufferSizeType txBufferFifoActReadIdx = CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx);
          /* #430 Repeat until a queued element is found or the queue is empty */
          while ((CANIF_VAR_TXBUFFERFIFO_QUEUEBASE((txBufferFifoActReadIdx + txBufferFifoQueueBaseStartIdx), eQueued) == 0u) && ((CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eQueueCounter)) != 0u)) /* FETA_CANIF_5 */
          {
            /* #440 Decrement ReadIdx and manage the underflow */
            CANIF_VAR_TXBUFFERFIFO_BUFFERBASE( txBufferFifoBaseIdx, eReadIdx) ++; /* SBSW_CANIF_37 */

            if(CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx) == CANIF_CFG_TXBUFFERFIFO_QUEUESIZE(txBufferCfgIdx))
            {
              CANIF_VAR_TXBUFFERFIFO_BUFFERBASE( txBufferFifoBaseIdx, eReadIdx) = 0; /* SBSW_CANIF_37 */
            }

            /* #450 Decrement the FIFO-counter */
            CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eQueueCounter)--; /* SBSW_CANIF_37 */
          
            /* #460 Update the FIFO-ReadIdx */
            txBufferFifoActReadIdx = CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx);
          }
          /* #500 Is FIFO already empty? */
          if (CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eQueueCounter) != 0u)
#  endif
          {
            /* #510 Call CanIf_TxQueueTransmit() for the found Tx PDU ID */
            retval = CanIf_TxQueueTransmit(CANIF_VAR_TXBUFFERFIFO_QUEUEBASE((CANIF_VAR_TXBUFFERFIFO_BUFFERBASE(txBufferFifoBaseIdx, eReadIdx) + txBufferFifoQueueBaseStartIdx), eTxPduId));
          }
        }
      }
      break;
# endif
    default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
      break;
  }
  return retval;
}
#endif


/* \trace CANIF-3550 */
#if (CANIF_VERSION_INFO_API == STD_ON)
/* \trace SPEC-1896 */
/**********************************************************************************************************************
 *  CanIf_GetVersionInfo()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_GetVersionInfo
Relation_Context:
CanIfVersionInfoApi # from this API #
Relation:
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CANIF_CODE) CanIf_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CANIF_APPL_VAR) VersionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check if parameter VersionInfo is valid */
  if (VersionInfo == NULL_PTR)
  {
    /* \trace SPEC-5235 */
    errorId = CANIF_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation --------------------------------------------- */
    /* \trace SPEC-3552 */
    /* #100 Get the version information: vendor ID, module ID, SW version */
    VersionInfo->vendorID         = CANIF_VENDOR_ID; /* SBSW_CANIF_9 */
    VersionInfo->moduleID         = CANIF_MODULE_ID; /* SBSW_CANIF_9 */
    VersionInfo->sw_major_version = CANIF_SW_MAJOR_VERSION; /* SBSW_CANIF_9 */
    VersionInfo->sw_minor_version = CANIF_SW_MINOR_VERSION; /* SBSW_CANIF_9 */
    VersionInfo->sw_patch_version = CANIF_SW_PATCH_VERSION; /* SBSW_CANIF_9 */
  }

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_GETVERSIONINFO_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif


/**********************************************************************************************************************
 *  CanIf_InitMemory
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_InitMemory
Relation_Context:
Relation:
CanIfVariant
CanIfVariant, CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CANIF_CODE) CanIf_InitMemory(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 controllerId;

  /* ----- Implementation ----------------------------------------------- */
#if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #100 Initialize the configuration pointer */
  CanIf_ConfigDataPtr = NULL_PTR;

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  CanIf_SystemInit = FALSE; /* ESCAN00083271 */
# endif
#endif /* CANIF_USE_INIT_POINTER == STD_ON */

   /* #110 Run through all CAN controllers and set the state to UNINIT */
  for (controllerId = 0u; controllerId < (sizeof(CanIf_CtrlStates) / sizeof(CanIf_CtrlStatesType)); controllerId++)
  {
    CanIf_SetCtrlMode(controllerId, CANIF_CS_UNINIT); /* SBSW_CANIF_24 */
  }
}


#if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
# if (CANIF_TRANSMIT_BUFFER_FIFO == STD_OFF)
#  if (CANIF_TRANSMIT_CANCELLATION == STD_ON)
/* \trace SPEC-3716 */
/**********************************************************************************************************************
 *  CanIf_CancelTxConfirmation()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_CancelTxConfirmation
Relation_Context:
CanIfTxBuffer, CanIfHwCancelTx # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfFdSupport # used for CANIF_STATIC_FD_TXQUEUE #
CanIfFdSupport, CanIfDevErrorDetect # used for CANIF_STATIC_FD_TXQUEUE #
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_CancelTxConfirmation(PduIdType CanTxPduId, P2CONST(Can_PduType, AUTOMATIC, CANIF_CBK_DRV_VAR) PduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#    if (CANIF_DEV_ERROR_DETECT == STD_ON)
#     if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#     endif
  /* #20 Check if parameter PduInfoPtr is valid */
  if (PduInfoPtr == NULL_PTR)
  {
    errorId = CANIF_E_PARAM_POINTER;
  }
  /* #30 Check if parameter PduInfoPtr->swPduHandle is valid */
  else if (PduInfoPtr->swPduHandle >= CANIF_CFG_MAX_TXPDUS)
  {
    /* \trace SPEC-3712 */
    errorId = CANIF_E_PARAM_LPDU;
  }
  /* #40 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(CANIF_CFG_TX_CONTROLLER(PduInfoPtr->swPduHandle)) == CANIF_CS_UNINIT )
  {
    /* \trace SPEC-3708, SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
#     if ((CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON) && (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
  /* #50 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(CANIF_CFG_TX_CONTROLLER(PduInfoPtr->swPduHandle)))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#     endif
  else
#    endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Is TxPDU a BasicCAN-PDU and has it a buffer? */
    if (CANIF_CFG_MAILBOX_HASTXBUFFER(CANIF_CFG_TX_HTH(PduInfoPtr->swPduHandle))) /* COV_CANIF_GENDATA_FAILURE */
    {
      /* #110 Verify the DLC of cancelled Tx-PDU (only if FD-Tx-queue is configured) */
#    if(CANIF_STATIC_FD_TXQUEUE == STD_ON)
      if(PduInfoPtr->length <= CANIF_CFG_TXBUFFERPRIOBYCANID_STATFDQUEUE_MAXDATALENGTH(PduInfoPtr->swPduHandle))
#    else
      if(PduInfoPtr->length <= CANIF_STATIC_BUFFER_SIZE)
#    endif
      {
        /* #120 Check if cancelled Tx-PDU is already buffered in Tx-queue (critical section is used) */
        CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_1);
        if (CanIf_SetPrioByCanIdTxPduAsQueued(PduInfoPtr->swPduHandle) == E_OK)
        {
          CanIf_TxBufferPrioByCanIdQueuePdu(PduInfoPtr); /* SBSW_CANIF_5 */
        }
        else
        {
          /* #130 Newer Tx-PDU is already buffered, discard the cancelled one */
        }
        CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_1);
      }
#    if ((CANIF_STATIC_FD_TXQUEUE == STD_ON) && (CANIF_DEV_ERROR_DETECT == STD_ON))
      else
      {
        errorId = CANIF_E_PARAM_DLC;
      }
#    endif
      /* #140 Trigger the transmission of the queue */
      CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_2);
      (void)CanIf_TxQueueTreatment((CanIf_HwHandleType)CANIF_CFG_TX_HTH(PduInfoPtr->swPduHandle));
      CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_2); /* lock over queue handling + write call (driver does not expect direct call inbetween) */
    }
    else
    {
      /* FullCANs won't be cancelled */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
#    if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CANCELTXCONFIRMATION_API, errorId);
  }
#    else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#    endif
  CANIF_DUMMY_STATEMENT(CanTxPduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
#  endif
# endif
#endif


/* \trace SPEC-4718 */
#if (CANIF_CANCEL_SUPPORT_API == STD_ON)
/* \trace SPEC-4715 */
/**********************************************************************************************************************
 *  CanIf_CancelTransmit()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_CancelTransmit
Relation_Context:
CanIfTransmitCancellationAPI # from this API #
Relation:
CanIfDevErrorDetect, CanIfVariant
CanIfVariant
CanIfVariant, CanIfDevErrorDetect
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfTxBuffer
CanIfMultipleDriver
CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
 */
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_CancelTransmit(PduIdType CanTxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK;
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if ((CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON))
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON) */
# if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
#  if (CANIF_DEV_ERROR_DETECT == STD_ON) 
  /* #20 Check if parameter CanTxPduId is valid if a CanTxPduId indirection is done */
  if (CanTxPduId >= CANIF_CFG_MAX_ULTXPDUS)
  {
    /* \trace SPEC-2007 */
    errorId = CANIF_E_INVALID_TXPDUID;
  }
  else
#  endif
  {
    /* For PB configurations the TxPduId order between CanIf and higher layers may differ and needs to be indirected */
    CanTxPduId = CANIF_CFG_TXPDUID2INTTXPDUID(CanTxPduId); /* PRQA S 1338 */ /* MD_CanIf_Rule17.8 */
# endif
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
    /* #30 Check if parameter CanTxPduId is valid */
    if (CanTxPduId >= CANIF_CFG_MAX_TXPDUS)
    {
      /* \trace SPEC-2007 */
      errorId = CANIF_E_INVALID_TXPDUID;
    }
    /* #40 Check if CAN controller is initialized */
    else if (CanIf_GetCtrlMode(CANIF_CFG_TX_CONTROLLER(CanTxPduId)) == CANIF_CS_UNINIT)
    {
        /* \trace SPEC-5238 */
        errorId = CANIF_E_UNINIT;
    }
#  if ((CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON) && (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
    /* #50 Check if corresponding CanDrv is available in current identity */
    else if (!CANIF_CFG_IS_CANDRVINDEXVALID(CANIF_CFG_TX_CONTROLLER(CanTxPduId)))
    {
      errorId = CANIF_E_PARAM_CONTROLLERID;
    }
#  endif
    else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
    {
      /* ----- Implementation ----------------------------------------------- */
      const CanIf_AnyIdxType hth = CANIF_CFG_TX_HTH(CanTxPduId);

# if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
      /* #100 If Tx PDU belongs to a Tx buffer remove Tx PDU from Tx buffer */
      if (CANIF_CFG_MAILBOX_HASTXBUFFER(hth))
      {
        /* #110  Enter critical section (Reason: current state of Tx buffer may not be changed elsewhere) */
        CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_1);

        /* #120 Perform the removal according to the Tx buffer type */
        switch (CANIF_CFG_MAILBOX_TXBUFFERHANDLINGTYPE(hth))
        {
#  if (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON)
          /* #130 For PRIOBYCANID: Delete Tx PDU */
          case CANIF_TXBUFFER_HANDLINGTYPE_PRIOBYCANID:
            (void)CanIf_ClearPrioByCanIdTxPduAsQueued(CanTxPduId);
            break;
#  endif

#  if (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON)
          /* #140 For FIFO: Set corresponding entry to invalid */
          case CANIF_TXBUFFER_HANDLINGTYPE_FIFO:
            {
              const CanIf_AnyIdxType queueindex = CANIF_CFG_TX_QUEUEINDEX(CanTxPduId);

              /* If Tx PDU is part of the FIFO, set flag and position to invalid */
              if(CANIF_VAR_TXBUFFERFIFO_QUEUEIDX2FIFOEL(queueindex) != CANIF_NO_ENTRY_IN_TX_FIFO)
              {
                CANIF_VAR_TXBUFFERFIFO_QUEUEBASE((CANIF_VAR_TXBUFFERFIFO_QUEUEIDX2FIFOEL(queueindex) + CANIF_CFG_TXBUFFERFIFO_QUEUEBASESTARTIDX(CANIF_CFG_MAILBOX_TXBUFFERIDX(hth))), eQueued) = 0; /* SBSW_CANIF_39 */
                CANIF_VAR_SET_TXBUFFERFIFO_QUEUEIDX2FIFOEL(queueindex, CANIF_NO_ENTRY_IN_TX_FIFO); /* SBSW_CANIF_36 */
              }
            }
            break;
#  endif /* CANIF_TRANSMIT_BUFFER_FIFO == STD_ON */

          default: /* COV_CANIF_GENDATA_FAILURE */
            break;
        }
        /* ESCAN00054772 */
        /* #160 Leave critical section */
        CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_1);
      }
# endif /* (CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON) */

      /* #170 Request cancelation in CAN Hardware Unit
              Determine CAN Driver specific CanHardwareObject
              Determine responsible CAN Driver Can_<Infix>_Cancel function (if multiple CAN Drivers are used)
              Call Can_<Infix>_CancelTx with CAN Driver specific CanHardwareObject and Tx PDU ID
      */
      {
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
        const uint8 controllerId = CANIF_CFG_TX_CONTROLLER(CanTxPduId);
        const CanIf_Can_CancelTxFctType canCancelTxFct = CANIF_GET_CANCANCELTXFCT(controllerId);
        const Can_HwHandleType canDrvHth = (Can_HwHandleType)(hth - CANIF_CFG_MULTIPLECANDRV_HXHOFFSET(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(controllerId)));
# else
        const Can_HwHandleType canDrvHth = (Can_HwHandleType)hth;
# endif

        /* canCancelTxFct() is defined to Can_<Infix>_CancelTx if only a single CAN Driver is used */
        canCancelTxFct(canDrvHth, CanTxPduId); /* SBSW_CANIF_43 */
        retval = E_OK;
      }
    }
# if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
  }
# endif

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CANCELTRANSMIT_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif /* CANIF_CANCEL_SUPPORT_API == STD_ON */


#if (CANIF_CANCEL_SUPPORT_API == STD_ON)
/**********************************************************************************************************************
 *  CanIf_CancelTxNotification()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_CancelTxNotification
Relation_Context:
CanIfTransmitCancellationAPI # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfVariant, CanIfMultipleDriver
CanIfPnEnable
CanIfTxBuffer
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_CancelTxNotification(PduIdType PduId, CanIf_CancelResultType IsCancelled)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter PduId is valid */
  if (PduId >=  CANIF_CFG_MAX_TXPDUS)
  {
    errorId = CANIF_E_PARAM_LPDU;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(CANIF_CFG_TX_CONTROLLER(PduId)) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
  /* #40 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(CANIF_CFG_TX_CONTROLLER(PduId)))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Process only if CAN controller is active */
    if (CanIf_GetCtrlMode(CANIF_CFG_TX_CONTROLLER(PduId)) == CANIF_CS_STARTED)
    {
      if ( (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(PduId)) == CANIF_GET_ONLINE) ||
           (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(PduId)) == CANIF_GET_TX_ONLINE)
# if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
         || (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(PduId)) == CANIF_GET_ONLINE_WAKF)
         || (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(PduId)) == CANIF_GET_TX_ONLINE_WAKF)
# endif
       )
      {
#if ((CANIF_TRANSMIT_BUFFER_PRIO_BY_CANID == STD_ON) || (CANIF_TRANSMIT_BUFFER_FIFO == STD_ON))
        if (CANIF_CFG_MAILBOX_HASTXBUFFER(CANIF_CFG_TX_HTH(PduId)))
        {
          /* \trace SPEC-1227 */
          /* #110 If Tx-queue is affected send queued messages directly */
          CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_2);
          (void)CanIf_TxQueueTreatment((CanIf_HwHandleType)CANIF_CFG_TX_HTH(PduId));
          /* \trace SPEC-1227 */ /* lock over queue handling + write call (driver does not expect direct call inbetween) */
          CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_2);
        }
# endif
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_TXNOTIFICATION_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  CANIF_DUMMY_STATEMENT(IsCancelled); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}
#endif


/* \trace SPEC-3595 */
#if (CANIF_TRCV_HANDLING == STD_ON) 
/* \trace SPEC-1933 */
/**********************************************************************************************************************
 *  CanIf_SetTrcvMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_SetTrcvMode
Relation_Context:
CanIfTransceiverHandling # from this API #
Relation:
CanIfDevErrorDetect
CanIfTransceiverMapping
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfTransceiverMapping
CanIfDevErrorDetect
CanIfTransceiverMapping
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetTrcvMode(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  uint8 controllerId;
# endif

# if (CANIF_TRCV_MAPPING == STD_ON)
  uint8 lowIndex;
  CanIf_CanTrcvFctTblIdx lowAddress;
  CanIf_CanTrcv_SetOpModeFctType lowFct;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter TransceiverId is valid */
  if (TransceiverId >= CanIf_GetMaxTrcvHandleIdPlusOne())
  {
    /* SPEC-5064 */
    errorId = CANIF_E_PARAM_TRCV;
  }
  else
#  if (CANIF_TRCV_MAPPING == STD_ON)
  /* #30 Check if parameter TransceiverId is valid if transceiver mapping is active */
  if (TransceiverId >= CanIf_GetSizeOfTransceiverUpToLowMap()) /* PRQA S 2992, 2996 */ /* MD_CanIf_CanTrcvMapConstData */ /* COV_CANIF_GENDATA_FAILURE */
  {
    errorId = CANIF_E_PARAM_TRCV; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
  else
#  endif
# endif
  {
# if(CANIF_DEV_ERROR_DETECT == STD_ON)
    /* #40 Check if a CAN controller is mapped to a CAN transceiver */
    controllerId = CANIF_CFG_TRCVTOCTRLMAP(TransceiverId);
    if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
    {
      errorId = CANIF_E_PARAM_CONTROLLER;
    }
    /* #50 Check if corresponding CAN controller is initialized */
    else if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
    {
      /* \trace SPEC-5238 */
      errorId = CANIF_E_UNINIT;
    }
    /* #60 Check if parameter TransceiverMode is valid */
    else if ((TransceiverMode != CANTRCV_TRCVMODE_NORMAL) &&
             (TransceiverMode != CANTRCV_TRCVMODE_STANDBY) &&
             (TransceiverMode != CANTRCV_TRCVMODE_SLEEP))
    {
      /* \trace SPEC-5213 */
      errorId = CANIF_E_PARAM_TRCVMODE; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #100 If several transceiver drivers are configured, convert TransceiverId to the driver specific one */
# if (CANIF_TRCV_MAPPING == STD_ON)
      lowIndex = CANIF_CFG_TRCVUPTOLOWMAP_TRCV(TransceiverId);
      lowAddress = CANIF_CFG_TRCVUPTOLOWMAP_DRIVERINDEX(TransceiverId);
      lowFct = CanIf_GetCanTrcv_SetOpModeFctMap(lowAddress);

      if (lowFct != NULL_PTR)
      {
        /* \trace SPEC-1967 */
        /* #110 Pass the request to the transceiver driver */
        retval = lowFct(lowIndex, TransceiverMode); /* SBSW_CANIF_22 */
      }
# else
      /* SPEC-3586 */ /* \trace SPEC-1967 */
      retval = CanTrcv_SetOpMode(TransceiverId, TransceiverMode);
# endif
    }
  } /* Valid TransceiverId */

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_SETTRCVMODE_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif


#if (CANIF_TRCV_HANDLING == STD_ON)
/* \trace SPEC-1937 */
/**********************************************************************************************************************
 *  CanIf_GetTrcvMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_GetTrcvMode
Relation_Context:
CanIfTransceiverHandling # from this API #
Relation:
CanIfDevErrorDetect
CanIfTransceiverMapping
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfTransceiverMapping
CanIfDevErrorDetect
CanIfTransceiverMapping
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetTrcvMode(P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANIF_APPL_STATE_VAR) TransceiverModePtr, uint8 TransceiverId )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  uint8 controllerId;
# endif

# if (CANIF_TRCV_MAPPING == STD_ON)
  uint8 lowIndex;
  CanIf_CanTrcvFctTblIdx lowAddress;
  CanIf_CanTrcv_GetOpModeFctType lowFct;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter TransceiverId is valid */
  if (TransceiverId >= CanIf_GetMaxTrcvHandleIdPlusOne())
  {
    /* \trace SPEC-3593 */
    errorId = CANIF_E_PARAM_TRCV;
  }
  else
#  if (CANIF_TRCV_MAPPING == STD_ON)
  /* #30 Check if parameter TransceiverId is valid if transceiver mapping is active */
  if (TransceiverId >= CanIf_GetSizeOfTransceiverUpToLowMap()) /* PRQA S 2992, 2996 */ /* MD_CanIf_CanTrcvMapConstData */ /* COV_CANIF_GENDATA_FAILURE */
  {
    errorId = CANIF_E_PARAM_TRCV; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
  else
#  endif
# endif
  {
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
    controllerId = CANIF_CFG_TRCVTOCTRLMAP(TransceiverId);
    /* #40 Check if a CAN controller is mapped to a CAN transceiver */
    if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
    {
      errorId = CANIF_E_PARAM_CONTROLLER;
    }
    /* #50 Check if corresponding CAN controller is initialized */
    else if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
    {
      /* \trace SPEC-5238 */
      errorId = CANIF_E_UNINIT;
    }
    /* #60 Check if parameter TransceiverModePtr is valid */
    else if (TransceiverModePtr == NULL_PTR)
    {
      /* \trace SPEC-5227 */
      errorId = CANIF_E_PARAM_POINTER;
    }
    else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #100 If several transceiver drivers are configured, convert TransceiverId to the driver specific one */
# if (CANIF_TRCV_MAPPING == STD_ON)
      lowIndex = CANIF_CFG_TRCVUPTOLOWMAP_TRCV(TransceiverId);
      lowAddress = CANIF_CFG_TRCVUPTOLOWMAP_DRIVERINDEX(TransceiverId);
      lowFct = CanIf_GetCanTrcv_GetOpModeFctMap(lowAddress);

      if (lowFct != NULL_PTR)
      {
        /* \trace SPEC-1967 */
        /* #110 Pass the request to the transceiver driver */
        retval = lowFct(lowIndex, TransceiverModePtr); /* SBSW_CANIF_44 */
      }
# else
      /* SPEC-3594 */ /* \trace SPEC-1967 */
      retval = CanTrcv_GetOpMode(TransceiverId, TransceiverModePtr); /* SBSW_CANIF_21 */
# endif
    }
  } /* Valid TransceiverId */

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_GETTRCVMODE_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif


/* \trace SPEC-3600 */
#if (CANIF_TRCV_HANDLING == STD_ON)
/* \trace SPEC-1934 */
/**********************************************************************************************************************
 *  CanIf_GetTrcvWakeupReason()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_GetTrcvWakeupReason
Relation_Context:
CanIfTransceiverHandling # from this API #
Relation:
CanIfDevErrorDetect
CanIfTransceiverMapping
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfTransceiverMapping
CanIfDevErrorDetect
CanIfTransceiverMapping
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetTrcvWakeupReason(uint8 TransceiverId, P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANIF_APPL_STATE_VAR) TrcvWuReasonPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  uint8 controllerId;
# endif

# if (CANIF_TRCV_MAPPING == STD_ON)
  uint8 lowIndex;
  CanIf_CanTrcvFctTblIdx lowAddress;
  CanIf_CanTrcv_GetBusWuReasonFctType lowFct;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter TransceiverId is valid */
  if (TransceiverId >= CanIf_GetMaxTrcvHandleIdPlusOne())
  {
    /* \trace SPEC-5057 */
    errorId = CANIF_E_PARAM_TRCV;
  }
  else
#  if (CANIF_TRCV_MAPPING == STD_ON)
  /* #30 Check if parameter TransceiverId is valid if transceiver mapping is active */
  if (TransceiverId >= CanIf_GetSizeOfTransceiverUpToLowMap()) /* PRQA S 2992, 2996 */ /* MD_CanIf_CanTrcvMapConstData */ /* COV_CANIF_GENDATA_FAILURE */
  {
    errorId = CANIF_E_PARAM_TRCV; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
  else
#  endif
# endif
  {
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
    controllerId = CANIF_CFG_TRCVTOCTRLMAP(TransceiverId);
    /* #40 Check if a CAN controller is mapped to a CAN transceiver */
    if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
    {
      errorId = CANIF_E_PARAM_CONTROLLER;
    }
    /* #50 Check if corresponding CAN controller is initialized */
    else if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
    {
      /* \trace SPEC-5238 */
      errorId = CANIF_E_UNINIT;
    }
    /* #60 Check if parameter TrcvWuReasonPtr is valid */
    else if (TrcvWuReasonPtr == NULL_PTR)
    {
      /* \trace SPEC-5221 */
      errorId = CANIF_E_PARAM_POINTER;
    }
    else
# endif /* CANIF_DEV_ERROR_DETECT  == STD_ON */
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #100 If several transceiver drivers are configured, convert TransceiverId to the driver specific one */
# if (CANIF_TRCV_MAPPING == STD_ON)
      lowIndex   = CANIF_CFG_TRCVUPTOLOWMAP_TRCV(TransceiverId);
      lowAddress = CANIF_CFG_TRCVUPTOLOWMAP_DRIVERINDEX(TransceiverId);
      lowFct = CanIf_GetCanTrcv_GetBusWuReasonFctMap(lowAddress);

      if (lowFct != NULL_PTR)
      {
        /* \trace SPEC-1967 */
        /* #110 Pass the request to the transceiver driver */
        retval = lowFct(lowIndex, TrcvWuReasonPtr); /* SBSW_CANIF_44 */
      }
# else
      /* SPEC-3598 */ /* \trace SPEC-1967 */
      retval = CanTrcv_GetBusWuReason(TransceiverId, TrcvWuReasonPtr); /* SBSW_CANIF_21 */
# endif
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_GETTRCVWAKEUPREASON_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif


/* \trace SPEC-3603 */
#if (CANIF_TRCV_HANDLING == STD_ON)
/* \trace SPEC-3680 */
/**********************************************************************************************************************
 *  CanIf_SetTrcvWakeupMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_SetTrcvWakeupMode
Relation_Context:
CanIfTransceiverHandling # from this API #
Relation:
CanIfDevErrorDetect
CanIfTransceiverMapping
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfTransceiverMapping
CanIfDevErrorDetect
CanIfTransceiverMapping
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetTrcvWakeupMode(uint8 TransceiverId, CanTrcv_TrcvWakeupModeType TrcvWakeupMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  uint8 controllerId;
# endif

# if (CANIF_TRCV_MAPPING == STD_ON)
  uint8 lowIndex;
  CanIf_CanTrcvFctTblIdx lowAddress;
  CanIf_CanTrcv_SetWakeupModeFctType lowFct;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter TransceiverId is valid */
  if (TransceiverId >= CanIf_GetMaxTrcvHandleIdPlusOne())
  {
    /* \trace SPEC-5055 */
    errorId = CANIF_E_PARAM_TRCV;
  }
  else
#  if (CANIF_TRCV_MAPPING == STD_ON)
  /* #30 Check if parameter TransceiverId is valid if transceiver mapping is active */
  if (TransceiverId >= CanIf_GetSizeOfTransceiverUpToLowMap()) /* PRQA S 2992, 2996 */ /* MD_CanIf_CanTrcvMapConstData */ /* COV_CANIF_GENDATA_FAILURE */
  {
    errorId = CANIF_E_PARAM_TRCV; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
  else
#  endif
# endif
  {
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
    controllerId = CANIF_CFG_TRCVTOCTRLMAP(TransceiverId);

    /* #40 Check if a CAN controller is mapped to a CAN transceiver */
    if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
    {
      errorId = CANIF_E_PARAM_CONTROLLER;
    }
    /* #50 Check if corresponding CAN controller is initialized */
    else if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
    {
      /* \trace SPEC-5238 */
      errorId = CANIF_E_UNINIT;
    }
    /* #60 Check if parameter TrcvWakeupMode is valid */
    else if ((TrcvWakeupMode != CANTRCV_WUMODE_ENABLE) &&
        (TrcvWakeupMode != CANTRCV_WUMODE_DISABLE) &&
        (TrcvWakeupMode != CANTRCV_WUMODE_CLEAR))
    {
      /* \trace SPEC-5049 */
      errorId = CANIF_E_PARAM_TRCVWAKEUPMODE; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #100 If several transceiver drivers are configured, convert TransceiverId to the driver specific one */
# if (CANIF_TRCV_MAPPING == STD_ON)
      lowIndex   = CANIF_CFG_TRCVUPTOLOWMAP_TRCV(TransceiverId);
      lowAddress = CANIF_CFG_TRCVUPTOLOWMAP_DRIVERINDEX(TransceiverId);
      lowFct = CanIf_GetCanTrcv_SetWakeupModeFctMap(lowAddress);

      if (lowFct != NULL_PTR)
      {
        /* \trace SPEC-1967 */
        /* #110 Pass the request to the transceiver driver */
        retval = lowFct(lowIndex, TrcvWakeupMode); /* SBSW_CANIF_22 */
      }
# else
      /* SPEC-3602 */ /* \trace SPEC-1967 */
      retval = CanTrcv_SetWakeupMode(TransceiverId, TrcvWakeupMode);
# endif
    }
  } /* Valid TransceiverId */

  /* ----- Development Error Report ------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_SETTRCVWAKEUPMODE_API, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  return retval;
}
#endif


/* \trace SPEC-21399 */
#if (CANIF_TRCV_HANDLING == STD_ON)
/* CANIF764, \trace SPEC-5333 */
/**********************************************************************************************************************
 *  CanIf_TrcvModeIndication()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_TrcvModeIndication
Relation_Context:
CanIfTransceiverHandling # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfTransceiverMapping
CanIfTransceiverMapping, CanIfDevErrorDetect
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_TrcvModeIndication(uint8 TransceiverId, CanTrcv_TrcvModeType TransceiverMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  uint8 controllerId;
  CanIf_TrcvModeIndicationFctType trcvModeIndicationFct;
  
  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif /* CANIF_USE_INIT_POINTER == STD_ON */
  /* \trace SPEC-5303 */
  /* #20 Check if parameter TransceiverId is valid */
  if (TransceiverId >= CANIF_CFG_MAX_CANTRCV_TRCVHANDLEIDPLUSONE)
  {
    errorId = CANIF_E_PARAM_TRCV;
  }
  else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
# if (CANIF_TRCV_MAPPING == STD_ON)
    TransceiverId = CANIF_CFG_TRCVUPTOUPPERMAP(TransceiverId); /* PRQA S 1338 */ /* MD_CanIf_Rule17.8 */

#  if(CANIF_DEV_ERROR_DETECT == STD_ON)
    /* #30 Check if upper layer TransceiverId is valid (if transceiver mapping is configured) */
    if (TransceiverId >= CanIf_GetMaxTrcvHandleIdPlusOne())
    {
      errorId = CANIF_E_PARAM_TRCV;
    }
    else
#  endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
# endif /* CANIF_TRCV_MAPPING == STD_ON */
    {
      controllerId = CANIF_CFG_TRCVTOCTRLMAP(TransceiverId);

# if(CANIF_DEV_ERROR_DETECT == STD_ON)
      /* #40 Check if a CAN controller is mapped to a CAN transceiver */
      if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
      {
        errorId = CANIF_E_PARAM_CONTROLLER;
      }
      else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */

      /* ----- Implementation ----------------------------------------------- */
      /* \trace SPEC-5334 */
      /* #50 Verify if the corresponding CAN controller is initialized */
      if (CanIf_GetCtrlMode(controllerId) != CANIF_CS_UNINIT)
      {
        /* \trace SPEC-5335 */
        /* #100 Notify the upper layer about the mode transition, if configured */
        trcvModeIndicationFct = CANIF_CFG_TRCVMODEINDICATIONFCT();
        if (trcvModeIndicationFct != NULL_PTR)
        {
          trcvModeIndicationFct(TransceiverId, TransceiverMode); /* SBSW_CANIF_13 */
        }
      }
# if(CANIF_DEV_ERROR_DETECT == STD_ON)
      else
      {
        /* \trace SPEC-5238 */
        errorId = CANIF_E_UNINIT;
      }
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_TRCVMODEINDICATION_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif /* (CANIF_TRCV_HANDLING == STD_ON) */


#if ((CANIF_TRCV_HANDLING == STD_ON) && (CANIF_PN_TRCV_HANDLING == STD_ON))
/* \trace SPEC-9957 */
/**********************************************************************************************************************
 *  CanIf_ConfirmPnAvailability()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ConfirmPnAvailability
Relation_Context:
CanIfTransceiverHandling, CanIfTransceiverPn # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfTransceiverMapping
CanIfTransceiverMapping, CanIfDevErrorDetect
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_ConfirmPnAvailability(uint8 TransceiverId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  CanIf_ConfirmPnAvailabilityFctType cbkFct;

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  uint8 controllerId;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif /* CANIF_USE_INIT_POINTER == STD_ON */
  /* #20 Check if parameter TransceiverId is valid */
  if (TransceiverId >= CANIF_CFG_MAX_CANTRCV_TRCVHANDLEIDPLUSONE)
  {
    /* \trace SPEC-21351 */
    errorId = CANIF_E_PARAM_TRCV;
  }
  else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
# if (CANIF_TRCV_MAPPING == STD_ON)
    TransceiverId = CANIF_CFG_TRCVUPTOUPPERMAP(TransceiverId); /* PRQA S 1338 */ /* MD_CanIf_Rule17.8 */

#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
    /* #30 Check if upper layer TransceiverId is valid (if transceiver mapping is configured) */
    if (TransceiverId >= CanIf_GetMaxTrcvHandleIdPlusOne())
    {
      errorId = CANIF_E_PARAM_TRCV;
    }
    else
#  endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
# endif /* CANIF_TRCV_MAPPING == STD_ON */
    {

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
      controllerId = CANIF_CFG_TRCVTOCTRLMAP(TransceiverId);

      /* #40 Check if a CAN controller is mapped to a CAN transceiver */
      if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
      {
        errorId = CANIF_E_PARAM_CONTROLLER;
      }
      /* \trace SPEC-5238, SPEC-21337 */
      /* #50 Check if corresponding CAN controller is initialized */
      else if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
      {
       errorId = CANIF_E_UNINIT;
      }
      else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
      {
        /* ----- Implementation ----------------------------------------------- */
        /* CANIF753 */
        /* #100 If configured notify the upper layer about the PN availability */
        cbkFct = CANIF_CFG_CONFIRMPNAVAILABILITYFCT();

        if (cbkFct != NULL_PTR)
        {
          cbkFct(TransceiverId); /* SBSW_CANIF_13 */
        }
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CONFIRMPNAVAILABILITY_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif /* ((CANIF_TRCV_HANDLING == STD_ON) && (CANIF_PN_TRCV_HANDLING == STD_ON)) */


#if ((CANIF_TRCV_HANDLING == STD_ON) && (CANIF_PN_TRCV_HANDLING == STD_ON))
/* CANIF762 */
/**********************************************************************************************************************
 *  CanIf_ClearTrcvWufFlagIndication()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ClearTrcvWufFlagIndication
Relation_Context:
CanIfTransceiverHandling, CanIfTransceiverPn # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfTransceiverMapping
CanIfTransceiverMapping, CanIfDevErrorDetect
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_ClearTrcvWufFlagIndication(uint8 TransceiverId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  CanIf_ClearTrcvWufFlagIndicationFctType cbkFct;

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  uint8 controllerId;
# endif
    
  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif /* CANIF_USE_INIT_POINTER == STD_ON */
  /* \trace SPEC-21321 */
  /* #20 Check if parameter TransceiverId is valid */
  if (TransceiverId >= CANIF_CFG_MAX_CANTRCV_TRCVHANDLEIDPLUSONE)
  {
    errorId = CANIF_E_PARAM_TRCV;
  }
  else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
# if (CANIF_TRCV_MAPPING == STD_ON)
    TransceiverId = CANIF_CFG_TRCVUPTOUPPERMAP(TransceiverId); /* PRQA S 1338 */ /* MD_CanIf_Rule17.8 */

#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
    /* #30 Check if upper layer TransceiverId is valid (if transceiver mapping is configured) */
    if (TransceiverId >= CanIf_GetMaxTrcvHandleIdPlusOne())
    {
      errorId = CANIF_E_PARAM_TRCV;
    }
    else
#  endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
# endif /* CANIF_TRCV_MAPPING == STD_ON */
    {

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
      controllerId = CANIF_CFG_TRCVTOCTRLMAP(TransceiverId);

      /* #40 Check if a CAN controller is mapped to a CAN transceiver */
      if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
      {
        errorId = CANIF_E_PARAM_CONTROLLER;
      }
      /* \trace SPEC-21345, SPEC-5238 */
      /* #50 Check if corresponding CAN controller is initialized */
      else if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
      {
        errorId = CANIF_E_UNINIT;
      }
      else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
      {
        /* ----- Implementation ----------------------------------------------- */
        /* CANIF757 */
        /* #100 If configured notify the upper layer that WufFlag was cleared */
        cbkFct = CANIF_CFG_CLEARTRCVWUFFLAGINDICATIONFCT();

        if (cbkFct != NULL_PTR)
        {
          cbkFct(TransceiverId); /* SBSW_CANIF_13 */
        }
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CLEARTRCVWUFFLAGINDICATION_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif /* ((CANIF_TRCV_HANDLING == STD_ON) && (CANIF_PN_TRCV_HANDLING == STD_ON)) */


#if ((CANIF_TRCV_HANDLING == STD_ON) && (CANIF_PN_TRCV_HANDLING == STD_ON))
/* CANIF763 */
/**********************************************************************************************************************
 *  CanIf_CheckTrcvWakeFlagIndication()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_CheckTrcvWakeFlagIndication
Relation_Context:
CanIfTransceiverHandling, CanIfTransceiverPn # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfTransceiverMapping
CanIfTransceiverMapping, CanIfDevErrorDetect
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_CheckTrcvWakeFlagIndication(uint8 TransceiverId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  CanIf_CheckTrcvWakeFlagIndicationFctType cbkFct;

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  uint8 controllerId;
# endif
    
  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif /* CANIF_USE_INIT_POINTER == STD_ON */
  /* \trace SPEC-21324 */
  /* #20 Check if parameter TransceiverId is valid */
  if (TransceiverId >= CANIF_CFG_MAX_CANTRCV_TRCVHANDLEIDPLUSONE)
  {
    errorId = CANIF_E_PARAM_TRCV;
  }
  else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
# if (CANIF_TRCV_MAPPING == STD_ON)
    TransceiverId = CANIF_CFG_TRCVUPTOUPPERMAP(TransceiverId); /* PRQA S 1338 */ /* MD_CanIf_Rule17.8 */

#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
    /* #30 Check if upper layer TransceiverId is valid (if transceiver mapping is configured) */
    if (TransceiverId >= CanIf_GetMaxTrcvHandleIdPlusOne())
    {
      errorId = CANIF_E_PARAM_TRCV;
    }
    else
#  endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
# endif /* CANIF_TRCV_MAPPING == STD_ON */
    {

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
      controllerId = CANIF_CFG_TRCVTOCTRLMAP(TransceiverId);

      /* #40 Check if a CAN controller is mapped to a CAN transceiver */
      if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
      {
        errorId = CANIF_E_PARAM_CONTROLLER;
      }
      /* \trace SPEC-21350, SPEC-5238 */
      /* #50 Check if corresponding CAN controller is initialized */
      else if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
      {
        errorId = CANIF_E_UNINIT;
      }
      else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
      {
        /* ----- Implementation ----------------------------------------------- */
        /* CANIF759 */
        /* #100 If configured notify the upper layer that wake flag was checked */
        cbkFct = CANIF_CFG_CHECKTRCVWAKEFLAGINDICATIONFCT();

        if (cbkFct != NULL_PTR)
        {
          cbkFct(TransceiverId); /* SBSW_CANIF_13 */
        }
      }
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CHECKTRCVWAKEFLAGINDICATION_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif /* ((CANIF_TRCV_HANDLING == STD_ON) && (CANIF_PN_TRCV_HANDLING == STD_ON)) */


#if ((CANIF_TRCV_HANDLING == STD_ON) && (CANIF_PN_TRCV_HANDLING == STD_ON))
/* CANIF763 */
/**********************************************************************************************************************
 *  CanIf_ClearTrcvWufFlag()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ClearTrcvWufFlag
Relation_Context:
CanIfTransceiverHandling, CanIfTransceiverPn # from this API #
Relation:
CanIfDevErrorDetect
CanIfTransceiverMapping
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfTransceiverMapping
CanIfDevErrorDetect
CanIfTransceiverMapping
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_ClearTrcvWufFlag(uint8 TransceiverId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  uint8 controllerId;
# endif

# if (CANIF_TRCV_MAPPING == STD_ON)
  uint8 lowIndex;
  CanIf_CanTrcvFctTblIdx lowAddress;
  CanIf_CanTrcv_ClearTrcvWufFlagFctType lowFct;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter TransceiverId is valid */
  if (TransceiverId >= CanIf_GetMaxTrcvHandleIdPlusOne())
  {
    /* \trace SPEC-21406 */
    errorId = CANIF_E_PARAM_TRCV;
  }
  else
#  if (CANIF_TRCV_MAPPING == STD_ON)
  /* #30 Check if parameter TransceiverId is valid if transceiver mapping is active */
  if (TransceiverId >= CanIf_GetSizeOfTransceiverUpToLowMap()) /* PRQA S 2992, 2996 */ /* MD_CanIf_CanTrcvMapConstData */ /* COV_CANIF_GENDATA_FAILURE */
  {
    errorId = CANIF_E_PARAM_TRCV; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
  else
#  endif
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
    controllerId = CANIF_CFG_TRCVTOCTRLMAP(TransceiverId);

    /* #40 Check if a CAN controller is mapped to a CAN transceiver */
    if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
    {
      errorId = CANIF_E_PARAM_CONTROLLER;
    }
    /* #50 Check if corresponding CAN controller is initialized */
    else if(CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
    {
      /* \trace SPEC-5238 */
      errorId = CANIF_E_UNINIT;
    }
    else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #100 If several transceiver drivers are configured, convert TransceiverId to the driver specific one */
# if (CANIF_TRCV_MAPPING == STD_ON)
      /* \trace SPEC-9964 */
      lowIndex = CANIF_CFG_TRCVUPTOLOWMAP_TRCV(TransceiverId);
      lowAddress = CANIF_CFG_TRCVUPTOLOWMAP_DRIVERINDEX(TransceiverId);
      lowFct = CanIf_GetCanTrcv_ClearTrcvWufFlagFctMap(lowAddress);

      if (lowFct != (CanIf_CanTrcv_ClearTrcvWufFlagFctType)NULL_PTR)
      {
        /* #110 Pass the request to the transceiver driver */
        retval = lowFct(lowIndex); /* SBSW_CANIF_22 */
      }
      else
      {
        errorId = CANIF_E_PARAM_TRCV;
        /* retval is already set to E_NOT_OK */
      }
# else
      /* \trace SPEC-9964 */
      retval = CanTrcv_ClearTrcvWufFlag(TransceiverId);
# endif
    }
  } /* Valid TransceiverId */

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CLEARTRCVWUFFLAG_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif /* ((CANIF_TRCV_HANDLING == STD_ON) && (CANIF_PN_TRCV_HANDLING == STD_ON)) */


#if ((CANIF_TRCV_HANDLING == STD_ON) && (CANIF_PN_TRCV_HANDLING == STD_ON))
/* CANIF761 */
/**********************************************************************************************************************
 *  CanIf_CheckTrcvWakeFlag()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_CheckTrcvWakeFlag
Relation_Context:
CanIfTransceiverHandling, CanIfTransceiverPn # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfTransceiverMapping
CanIfDevErrorDetect
CanIfTransceiverMapping
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_CheckTrcvWakeFlag(uint8 TransceiverId)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
  uint8 controllerId;
# endif

# if (CANIF_TRCV_MAPPING == STD_ON)
  uint8 lowIndex;
  CanIf_CanTrcvFctTblIdx lowAddress;
  CanIf_CanTrcv_CheckWakeFlagFctType lowFct;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter TransceiverId is valid */
  if (TransceiverId >= CanIf_GetMaxTrcvHandleIdPlusOne())
  {
    /* \trace SPEC-21413 */
    errorId = CANIF_E_PARAM_TRCV;
  }
  else
#  if (CANIF_TRCV_MAPPING == STD_ON)
  if (TransceiverId >= CanIf_GetSizeOfTransceiverUpToLowMap()) /* PRQA S 2992, 2996 */ /* MD_CanIf_CanTrcvMapConstData */ /* COV_CANIF_GENDATA_FAILURE */
  {
    errorId = CANIF_E_PARAM_TRCV; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
  else
#  endif
# endif
  {
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
    controllerId = CANIF_CFG_TRCVTOCTRLMAP(TransceiverId);

    /* #30 Check if a CAN controller is mapped to a CAN transceiver */
    if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
    {
      errorId = CANIF_E_PARAM_CONTROLLER;
    }
    /* #40 Check if corresponding CAN controller is initialized */
    else if(CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
    {
      /* \trace SPEC-5238 */
      errorId = CANIF_E_UNINIT;
    }
    else
# endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) */
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #100 If several transceiver drivers are configured, convert TransceiverId to the driver specific one */
# if (CANIF_TRCV_MAPPING == STD_ON)
      /* \trace SPEC-9961 */
      lowIndex  = CANIF_CFG_TRCVUPTOLOWMAP_TRCV(TransceiverId);
      lowAddress  = CANIF_CFG_TRCVUPTOLOWMAP_DRIVERINDEX(TransceiverId);
      lowFct = CanIf_GetCanTrcv_CheckWakeFlagFctMap(lowAddress);

      if (lowFct != (CanIf_CanTrcv_CheckWakeFlagFctType)NULL_PTR)
      {
        /* #110 Pass the request to the transceiver driver */
        retval = lowFct(lowIndex); /* SBSW_CANIF_22 */
      }
      else
      {
        errorId = CANIF_E_PARAM_TRCV;
        /* retval is already set to E_NOT_OK */
      }
# else
      /* \trace SPEC-9961 */
      retval = CanTrcv_CheckWakeFlag(TransceiverId);
# endif /* (CANIF_TRCV_MAPPING == STD_ON) */
    }
  } /* Valid TransceiverId */

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CHECKTRCVWAKEFLAG_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif /* ((CANIF_TRCV_HANDLING == STD_ON) && (CANIF_PN_TRCV_HANDLING == STD_ON)) */


/* \trace SPEC-1898, SPEC-3681 */
#if (CANIF_WAKEUP_SUPPORT == STD_ON)
/* \trace SPEC-1910 */
/**********************************************************************************************************************
 *  CanIf_CheckWakeup()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_CheckWakeup
Relation_Context:
CanIfWakeup # from this API #
Relation:
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect
CanIfCanDrvWakeupHandling
CanIfCanDrvWakeupHandling, CanIfMultipleDriver
CanIfCanDrvWakeupHandling, CanIfMultipleDriver, CanIfDevErrorDetect
CanIfCanDrvWakeupHandling, CanIfMultipleDriver, CanIfDevErrorDetect, CanIfVariant
CanIfTransceiverHandling
CanIfTransceiverHandling, CanIfTransceiverMapping
CanIfTransceiverHandling, CanIfTransceiverMapping, CanIfDevErrorDetect
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_CheckWakeup(EcuM_WakeupSourceType WakeupSource)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* \trace SPEC-5345 */
  Std_ReturnType retval = E_OK; /* WakeUp request will be accepted */
  uint8 errorId = CANIF_E_NO_ERROR;
  EcuM_WakeupSourceType wokenUpSources = 0u;

  /* ----- Development Error Checks ------------------------------------- */
# if ((CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON))
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    retval = E_NOT_OK; /* PRQA S 2982 */ /* MD_MSR_RetVal */
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Run through all configured wake-up sources */
    /* Find the first WakeUp Source which pretends to be the responsible */
    uint8 numWUsrc; /* for-loop counter */
    for (numWUsrc = 0u; numWUsrc < CANIF_CFG_MAX_WAKEUPSOURCES; numWUsrc++) /* FETA_CANIF_13 */
    {
      /* #110 Acquire wake-up source information */
      const EcuM_WakeupSourceType wuSrc = CANIF_CFG_WAKEUP_WAKEUPSOURCE(numWUsrc); /* Wake-up source value */
      const CanIf_WakeUpTargetType wuTrgtmodule = CANIF_CFG_WAKEUP_WAKEUPTARGETMODULE(numWUsrc); /* Target module (CAN driver/CAN transceiver driver) */
      const uint8 wuTrgtaddress = CANIF_CFG_WAKEUP_WAKEUPTARGETADDRESS(numWUsrc); /* Target address (CAN controller/CAN transceiver) */

      /* Find the correct CAN Controller */
      /* indirect check of wake-up-source -> \trace SPEC-3687 */
      const uint8 controllerId = CANIF_CFG_WAKEUP_CONTROLLERID(numWUsrc);

      /* Initialize localRetval for WakeUpSources which are not handled by CanIf */
      Std_ReturnType localRetval = E_NOT_OK;
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
      /* DET error occurred other sources will be checked return value will be E_NOT_OK */

      /* #120 Verify that corresponding controllerId is valid */
      if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
      {
        retval = E_NOT_OK;
        /* \trace SPEC-3687 */
        errorId = CANIF_E_PARAM_WAKEUPSOURCE;
      }
      /* #130 Verify that the corresponding CAN controller is initialized */
      else if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
      {
        retval = E_NOT_OK;
        /* \trace SPEC-5238 */
        errorId = CANIF_E_UNINIT;
      }
      else
# endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
      {
        /* #140 Is it the wake-up source which shall be checked for wake-up? */
        if ((WakeupSource & wuSrc) != 0u)
        {
          /* SPEC-5284 */
          /* #150 Process only if CAN controller is either in SLEEP or in STOP */
          if ((CanIf_GetCtrlMode(controllerId) == CANIF_CS_SLEEP) || (CanIf_GetCtrlMode(controllerId) == CANIF_CS_STOPPED))
          {
# if (CANIF_CANDRV_WAKEUP_HANDLING == STD_ON)
            /* #200 For CAN controller as wake-up source */
            if (wuTrgtmodule == CANIF_WAKEUPREQUEST_CAN) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
            {
              /* \trace SPEC-3684, SPEC-1967 */
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if (CANIF_DEV_ERROR_DETECT == STD_ON)
              /* Required check to ensure indirection to CanIf_CanDrvFctTbl[] */
              if (wuTrgtaddress >= CanIf_GetSizeOfCanControllerIdUpToLowMap()) /* COV_CANIF_GENDATA_FAILURE */
              {
                errorId = CANIF_E_PARAM_CONTROLLERID;
                retval = E_NOT_OK;
              }
              else
#    if(CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON)
              /* #210 Verify that corresponding CanDrv is available in current identity (if multiple CAN Drivers and variants are used) */
              if (!CANIF_CFG_IS_CANDRVINDEXVALID(wuTrgtaddress)) /* COV_CANIF_GENDATA_FAILURE */
              {
                errorId = CANIF_E_PARAM_WAKEUPSOURCE;
                retval = E_NOT_OK;
              }
              else
#    endif
#   endif
#  endif /* CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON */
              {
                /* #220 Pass the request to the corresponding CAN Driver */
                localRetval = CanIf_CallCanCheckWakeup(wuTrgtaddress);
              }
            } else /* wuTrgtmodule == CANIF_WAKEUPREQUEST_CAN */
# endif /* CANIF_CANDRV_WAKEUP_HANDLING == STD_ON */

# if (CANIF_TRCV_HANDLING == STD_ON)
            /* #300 For CAN transceiver as wake-up source pass the request to the corresponding CAN transceiver Driver */
            if (wuTrgtmodule == CANIF_WAKEUPREQUEST_TRCV) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
            {
#  if (CANIF_TRCV_MAPPING == STD_ON) /* more than one transceiver respectively controller */
#   if (CANIF_DEV_ERROR_DETECT == STD_ON)
              /* Required check to ensure indirection to CanIf_CanTrcvFctTbl[] */
              if (wuTrgtaddress >= CanIf_GetSizeOfTransceiverUpToLowMap()) /* COV_CANIF_GENDATA_FAILURE */
              {
                errorId = CANIF_E_PARAM_TRCV;
                retval = E_NOT_OK;
              }
              else
#   endif
              {
                const uint8 lowIndex = CANIF_CFG_TRCVUPTOLOWMAP_TRCV(wuTrgtaddress); /* get the call-parameter for the transceiver-function */
                const CanIf_CanTrcvFctTblIdx lowAddress = CANIF_CFG_TRCVUPTOLOWMAP_DRIVERINDEX(wuTrgtaddress);
                const CanIf_CanTrcv_CheckWakeupFctType lowFct = CanIf_GetCanTrcv_CheckWakeupFctMap(lowAddress); /* get the corresponding transceiver-function */

                if (lowFct != NULL_PTR) /* COV_CANIF_GENDATA_FAILURE */
                {
                  /* call the corresponding transceiver function */
                  /* \trace SPEC-1967 */
                  localRetval = lowFct(lowIndex); /* SBSW_CANIF_22 */
                }
              }
#  else  /* CANIF_TRCV_MAPPING -> only one transceiver respectively one controller */
              /* \trace SPEC-3684, SPEC-1967 */
              localRetval = CanTrcv_CheckWakeup(wuTrgtaddress);
#  endif /* (CANIF_TRCV_MAPPING == STD_ON) */
            } else /* wuTrgtmodule == CANIF_WAKEUPREQUEST_TRCV */
# endif /* CANIF_TRCV_HANDLING == STD_ON */
            {
              /* CANIF_WAKEUPREQUEST_NONE nothing to do */
            }

            /* localRetval is E_OK if the requested source is responsible for the wakeup */
            if (localRetval == E_OK)
            {
              /* #400 Mark if the wake-up request was confirmed by CAN controller/CAN transceiver */
              wokenUpSources |= wuSrc;
            }
          } /* CAN controller in CANIF_CS_SLEEP || CANIF_CS_STOPPED? */
          else
          {
            /* != CANIF_CS_SLEEP || CANIF_CS_STOPPED */
            /* SPEC-5284 */
            retval = E_NOT_OK;
            /* Report DET error */
            errorId = CANIF_E_NOT_SLEEP;
          }
        } /* ValidWakeUpSource? */
      } /* DET Error occurred */
    } /* WakeUp-Source FOR-Loop */
  } /* (CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON) */

  /* SPEC-5278 */ /* \trace SPEC-5345 */
  if (wokenUpSources == 0u)
  {
    /* #500 If no wake-up sources were confirmed return E_NOT_OK */
    retval = E_NOT_OK;
  }

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CHECKWAKEUP_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
  }

# if (CANIF_CANDRV_WAKEUP_HANDLING == STD_ON)
/***********************************************************************************************************************
 *  CanIf_CallCanCheckWakeup()
 **********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_CallCanCheckWakeup
Relation_Context:
CanIfCanDrvWakeupHandling, CanIfWakeup # from this API #
CanIfCanDrvWakeupHandling, CanIfWakeup # from caller CanIf_CheckWakeup #
Relation:
CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421
CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421, CanIfMultipleDriver, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421, CanIfMultipleDriver, CanIfSupportCanDrvEqcAr421
CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr431, CanIfSupportCanDrvEqcMsrc
CanIfSupportCanDrvEqcAr431, CanIfSupportCanDrvEqcMsrc, CanIfMultipleDriver
CanIfSupportCanDrvEqcAr431, CanIfSupportCanDrvEqcMsrc, CanIfMultipleDriver, CanIfSupportCanDrvEqcAr403, CanIfSupportCanDrvEqcAr421
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*! Internal comment removed.
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
CANIF_LOCAL_INLINE FUNC(Std_ReturnType, CANIF_CODE) CanIf_CallCanCheckWakeup(uint8 ControllerId)
{ /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ----------------------------------------------- */
  /* Solves also ESCAN00094121 */

# if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON)
  /* #100 Determine EQC of responsible CAN Driver (if different CAN Driver EQCs are used) */
  if(CanIf_GetCanDrvEqcArVersionOfCanControllerIdUpToLowMap(ControllerId) == CAN_DRV_EQC_AR_403) /* CAN_DRV_EQC_AR_403 also used for EQC AR 4.2.1 */ /* PRQA S 2742 */ /* MD_CanIf_CanDrvEqcConstData */ /* COV_CANIF_CANDRV_EQC */
# endif

# if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421 == STD_ON)
  {
    /* #110 For CAN_DRV_EQC_AR_403 or CAN_DRV_EQC_AR_421
            Determine responsible CAN Driver Can_<Infix>_CheckWakeup function (if multiple CAN Drivers are used)
            If Can_<Infix>_CheckWakeup is available, call it with CAN Driver specific ControllerId
            Covert return value to Std_ReturnType
    */
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON)
    /* Combination with CAN Driver of EQC MSRC - therefore function pointer have to be always casted */
    const CanIf_Can_CheckWakeupAr403FctType canCheckWakeupFct = CANIF_GET_CANCHECKWAKEUPFCT(ControllerId).CanIf_Can_CheckWakeupAr403Fct; /* PRQA S 2880 */ /* MD_CanIf_CanDrvEqcUnreachableCode */
#   else
    const CanIf_Can_CheckWakeupFctType canCheckWakeupFct = CANIF_GET_CANCHECKWAKEUPFCT(ControllerId);
#   endif
    if(canCheckWakeupFct != NULL_PTR) /* COV_CANIF_CANDRV_EQC */
#  endif /* CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON */
    {
      /* canCheckWakeupFct() is defined to Can_<Infix>_CheckWakeup and CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER() is defined to ControllerId,
       if only a single CAN Driver is used */
      if(canCheckWakeupFct((uint8)CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId)) == CAN_OK) /* SBSW_CANIF_54 */ /* COV_CANIF_CANDRV_EQC */
      {
        retval = E_OK;
      }
    }
  }
# endif /* CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421 == STD_ON */

# if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421 == STD_ON) && (CANIF_SUPPORT_CANDRV_EQC_MSRC == STD_ON)
  else
# endif

# if (CANIF_SUPPORT_CANDRV_EQC_AR_431_OR_MSRC == STD_ON)
  {
   /* #120 For CAN_DRV_EQC_AR_431 or CAN_DRV_EQC_MSRC
           Determine responsible CAN Driver Can_<Infix>_CheckWakeup function (if multiple CAN Drivers are used)
           If Can_<Infix>_CheckWakeup is available, call it with CAN Driver specific ControllerId
           (Return value is according to Std_ReturnType)
  */
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if (CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421 == STD_ON)
    /* Combination with CAN Driver of EQC AR 4.0.3/AR 4.2.1 - so function pointer have to be casted */
    const CanIf_Can_CheckWakeupAr431FctType canCheckWakeupFct = CANIF_GET_CANCHECKWAKEUPFCT(ControllerId).CanIf_Can_CheckWakeupAr431Fct;
#   else
    const CanIf_Can_CheckWakeupFctType canCheckWakeupFct = CANIF_GET_CANCHECKWAKEUPFCT(ControllerId);
#   endif
    if(canCheckWakeupFct != NULL_PTR)
#  endif /* CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON */
    {
      /* canCheckWakeupFct() is defined to Can_<Infix>_CheckWakeup and CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER() is defined to ControllerId,
       if only a single CAN Driver is used */
      retval = canCheckWakeupFct((uint8)CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId)); /* SBSW_CANIF_54 */
    }
  }
# endif /* CANIF_SUPPORT_CANDRV_EQC_AR_431_OR_MSRC == STD_ON */

 return retval;
}
# endif /* CANIF_CANDRV_WAKEUP_HANDLING == STD_ON */
#endif /* CANIF_WAKEUP_SUPPORT == STD_ON */


/* \trace SPEC-1936, SPEC-3693, SPEC-5234 */
#if (CANIF_WAKEUP_VALIDATION == STD_ON)
/* \trace SPEC-1902 */
/**********************************************************************************************************************
 *  CanIf_CheckValidation()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_CheckValidation
Relation_Context:
CanIfWakeupValidation # from this API #
Relation:
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_CheckValidation(EcuM_WakeupSourceType WakeupSource)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_OK;
  uint8 errorId = CANIF_E_NO_ERROR;
  EcuM_WakeupSourceType wokenUpSources = 0u;
  uint8 numWUsrc;
  EcuM_WakeupSourceType wuSrc;
  uint8 controllerId;
  CanIf_WakeUpValidationFctType wakeupValidationFct;

  /* ----- Development Error Checks ------------------------------------- */
# if ((CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON))
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    retval = E_NOT_OK;
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Run through all configured wake-up sources */
    for (numWUsrc = 0u; numWUsrc < CANIF_CFG_MAX_WAKEUPSOURCES; numWUsrc++) /* FETA_CANIF_13 */
    {
      /* #110 Acquire wake-up source value and affected CAN controller */
      wuSrc = CANIF_CFG_WAKEUP_WAKEUPSOURCE(numWUsrc);

      /* Find the correct CAN Controller */
      /* indirect check of wake-up source -> \trace SPEC-3690 */
      controllerId  = CANIF_CFG_WAKEUP_CONTROLLERID(numWUsrc);

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
      /* DET error occurred other sources will be checked return value will be E_NOT_OK */
      /* #120 Check the CAN controller parameter */
      if (controllerId >= CANIF_CFG_MAX_CONTROLLER) /* COV_CANIF_GENDATA_FAILURE */
      {
        retval = E_NOT_OK;
        /* \trace SPEC-3690 */
        errorId = CANIF_E_PARAM_WAKEUPSOURCE;
      }
      /* #130 Check if CAN controller is initialized */
      else if (CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
      {
        retval = E_NOT_OK;
        /* \trace SPEC-5238 */
        errorId = CANIF_E_UNINIT;
      }
      else
# endif /* CANIF_DEV_ERROR_DETECT */
      {
        /* #140 Is it the wake-up source which shall be validated? */
        if ((WakeupSource & wuSrc) != 0u)
        {
          /* #150 Determine whether the passed wake-up source was validated */
          if (CanIf_GetWakeUpValidationState(controllerId) == CANIF_WUVALIDATION_DETECTED)
          {
            /* We detected a wake up validation */
            /* #160 Store the wake-up source if it was validated */
            wokenUpSources |= wuSrc;
          }
        } /* ValidWakeUpSource */
        else
        {
          /* Invalid wake up source passed / or no wake up occurred */
          /* Nothing to do */
        }
      } /* DET Error occurred */

      /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
      if (errorId != CANIF_E_NO_ERROR)
      {
        CanIf_Det_ReportError(CANIF_CHECKVALIDATIONUP_API, errorId);
      }
# endif
    } /* WakeUp-Source Loop */
  } /* (CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON) */

  if (wokenUpSources != 0u) /* ESCAN00085941 */
  {
    /* \trace SPEC-1905 */
    /* #200 Only if any wakeup source was validated successfully notify the upper layer */
    wakeupValidationFct = CANIF_CFG_WAKEUPVALIDATION();
    if (wakeupValidationFct != NULL_PTR)
    {
      wakeupValidationFct(wokenUpSources & WakeupSource); /* SBSW_CANIF_13 */ /* ESCAN00090296 */
    }
  }

# if (CANIF_DEV_ERROR_REPORT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CHECKVALIDATIONUP_API, errorId);
  }
#  endif
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif /* (CANIF_WAKEUP_SUPPORT == STD_ON) && (CANIF_WAKEUP_VALIDATION == STD_ON) */


/* \trace SPEC-3588 */
#if (CANIF_SETDYNAMICTXID_API == STD_ON)
/* \trace SPEC-1899 */
/**********************************************************************************************************************
 *  CanIf_SetDynamicTxId()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_SetDynamicTxId
Relation_Context:
CanIfDynTx # from this API #
Relation:
CanIfDevErrorDetect, CanIfVariant
CanIfVariant
CanIfVariant, CanIfDevErrorDetect
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfIDType
CanIfDevErrorDetect
CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_SetDynamicTxId(PduIdType CanTxPduId, Can_IdType CanId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  uint8 controllerId;
  PduIdType dynamicPduId;

  /* ----- Development Error Checks ------------------------------------- */
# if ((CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON))
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON) */

# if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
#  if (CANIF_DEV_ERROR_DETECT == STD_ON) 
  /* #20 Check if parameter CanTxPduId is valid if a CanTxPduId indirection is done.*/
  if (CanTxPduId >= CANIF_CFG_MAX_ULTXPDUS)
  {
    /* \trace SPEC-2007 */
    errorId = CANIF_E_INVALID_TXPDUID;
  }
  else
#  endif
  {
    /* for PB and LT configurations the TxPduId order between CanIf and higher layers may differ and needs to be indirected */
    CanTxPduId = CANIF_CFG_TXPDUID2INTTXPDUID(CanTxPduId); /* PRQA S 1338 */ /* MD_CanIf_Rule17.8 */ /* ESCAN00075868 */
# endif
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
    /* #30 Check if parameter CanTxPduId is valid. */
    if (CanTxPduId >= CANIF_CFG_MAX_TXPDUS)
    {
      /* \trace SPEC-2007 */
      errorId = CANIF_E_INVALID_TXPDUID;
    }
    else
# endif
    {
      /* Collect configuration data */
      controllerId = CANIF_CFG_TX_CONTROLLER(CanTxPduId);
      dynamicPduId = CANIF_CFG_STAT2DYN_INDIRECTION(CanTxPduId);

# if (CANIF_DEV_ERROR_DETECT == STD_ON)
      /* #40 Check if the ID of corresponding dynamic PDU is valid */
      if (dynamicPduId >= CanIf_GetSizeOfDynTxId())
      {
        /* no ASR-requirement, just verify the dynamicPduId */
        errorId = CANIF_E_INVALID_TXPDUID;
      }
#  if (CANIF_EXTENDEDID_SUPPORT != STD_ON)
      /* #50 Check if parameter CanId is valid */
      else if ((CanId & CANIF_FD_FLAG_MASKOUT) > 0x7FFu)
      {
        /* \trace SPEC-3596 */
        errorId = CANIF_E_PARAM_CANID;
      }
#  endif
      /* #60 Check if CAN controller is initialized */
      else if(CanIf_GetCtrlMode(controllerId) == CANIF_CS_UNINIT)
      {
        /* \trace SPEC-5238 */
        errorId = CANIF_E_UNINIT;
      }
      else
# endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) */
      {
        /* ----- Implementation ----------------------------------------------- */
        /* \trace SPEC-3589 */
# if (CANIF_DEV_ERROR_DETECT == STD_OFF)
        /* #100 Process only if corresponding CAN controller is initialized */
        if(CanIf_GetCtrlMode(controllerId) != CANIF_CS_UNINIT)
# endif
        {
          /* \trace SPEC-5271 preemptive call of CanTransmit */
          CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_5);
          /* \trace SPEC-1906 identifier extension flag will be passed */
          /* #110 Set the CAN identifier (protected by critical section) */
          CANIF_VAR_DYNTXID(dynamicPduId) = CanId; /* SBSW_CANIF_14 */
          CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_5);
        }
      } /* (CANIF_DEV_ERROR_DETECT == STD_ON) */
    } /* (CANIF_DEV_ERROR_DETECT == STD_ON) && (CANIF_USE_INIT_POINTER == STD_ON) */
# if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
  }
# endif

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_SETDYNAMICTXID_API_ID, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif /* (CANIF_SETDYNAMICTXID_API == STD_ON) */


/* \trace SPEC-5307, SPEC-5308 */
/**********************************************************************************************************************
 *  CanIf_ControllerModeIndicationAr403()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ControllerModeIndicationAr403
Relation_Context:
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect
CanIfTxConfPollSupport
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
 */
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_ControllerModeIndicationAr403(uint8 ControllerId, CanIf_ControllerModeType ControllerMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  CanIf_CtrlModeIndicationFctType cbkFct = CANIF_CFG_CTRLMODEINDICATIONFCT();

  /* ----- Development Error Checks ------------------------------------- */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
# if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    /* \trace SPEC-5302 */
    errorId = CANIF_E_PARAM_CONTROLLER;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
#if (CANIF_DEV_ERROR_DETECT == STD_OFF)
    /* \trace SPEC-5305 */
    /* #100 Process only if CAN controller is initialized */
    if (CANIF_CS_UNINIT != CanIf_GetCtrlMode(ControllerId))
#endif
    {
      /* outer "switch" -> indication mode changed */
      /* inner "switch" -> current mode */
      switch(ControllerMode)
      {
        case CANIF_CS_STOPPED:
          /* #300 Transition to controller mode STOPPED confirmed */
          switch(CanIf_GetCtrlMode(ControllerId))
          {
            case CANIF_CS_STARTED:
            case CANIF_CS_SLEEP:
              /* \trace SPEC-5347, SPEC-5340 */
              /* #310 Set the internal controller mode to STOPPED if currently in controller mode STARTED/SLEEP */
              CanIf_SetCtrlMode(ControllerId, CANIF_CS_STOPPED); /* SBSW_CANIF_3 */
#if (CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT == STD_ON)
              /* \trace SPEC-9980 */
              /* TxConfirmationState is reset always when CAN controller is stopped
              * - BusOff
              * - Going to _SLEEP
              * - CanIf_InitController() is called */
              /* #320 Reset the Tx-confirmation state at the affected CAN controller */
              CanIf_SetTxConfState(ControllerId, CANIF_NO_NOTIFICATION); /* SBSW_CANIF_3 */
#endif
              break;
            case CANIF_CS_STOPPED:
              /* #330 Do nothing if currently in controller mode STOPPED */
              /* Change to STOP confirmed */
              /* CANIF713 nothing to do */
              break;
            default:
              /* not defined in ASR4-specification */
              break;
          }
          break;
        case CANIF_CS_STARTED:
          /* #400 Transition to controller mode STARTED confirmed */
          switch(CanIf_GetCtrlMode(ControllerId))
          {
            case CANIF_CS_STOPPED:
              /* \trace SPEC-5351 */
              /* #410 Set the internal controller mode to STARTED if currently in controller mode STOPPED */
              CanIf_SetCtrlMode(ControllerId, CANIF_CS_STARTED); /* SBSW_CANIF_3 */
              break;
            case CANIF_CS_STARTED:
              /* \trace SPEC-5341 */
              /* #420 Do nothing if currently in controller mode STARTED */
              break;
            case CANIF_CS_SLEEP:
              /* \trace SPEC-5340 */
              /* #430 If currently in controller mode SLEEP then transition is not allowed. Do nothing */
              break;
            default:
              /* not defined in ASR4-specification */
              break;
          }
          break;
        case CANIF_CS_SLEEP:
          /* #500 Transition to controller mode SLEEP confirmed */
          switch(CanIf_GetCtrlMode(ControllerId))
          {
            case CANIF_CS_STOPPED:
            case CANIF_CS_SLEEP:
              /* \trace SPEC-5348, SPEC-5344 */
              /* #510 Set the internal controller mode to SLEEP if currently in controller mode STOPPED/SLEEP */
              CanIf_SetCtrlMode(ControllerId, CANIF_CS_SLEEP); /* SBSW_CANIF_3 */
              break;
            case CANIF_CS_STARTED:
              /* #520 If currently in controller mode STARTED then transition is not allowed. Do nothing */
              break;
            default:
              /* not defined in ASR4-specification */
              break;
          }
          break;
        default:
          /* not defined in ASR4-specification */
          break;
      }
      /* \trace SPEC-5331 */
      /* #600 Inform the upper layer about the controller mode transition */
      /* Check if the callback function is available */
      if (cbkFct != NULL_PTR)
      {
        /* Call the callback function */
        cbkFct(ControllerId, ControllerMode); /* SBSW_CANIF_13 */
      }
    } /* (CANIF_CS_UNINIT == CanIf_GetCtrlMode(ControllerId)) */
  }

  /* ----- Development Error Report ------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CONTROLLERMODEINDICATION_API, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
}


#if (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON)
/**********************************************************************************************************************
 *  CanIf_ControllerModeIndicationAr431()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ControllerModeIndicationAr431
Relation_Context:
CanIfSupportCanDrvEqcAr431 # from this API #
Relation:
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CANIF_CODE) CanIf_ControllerModeIndicationAr431(uint8 ControllerId, Can_ControllerStateType ControllerMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  CanIf_ControllerModeType canIfCtrlMode = (CanIf_ControllerModeType)ControllerMode; /* PRQA S 4322 */ /* MD_CanIf_CastEnumToDiffEnum */

  /* ----- Implementation ----------------------------------------------- */

  /* #100 Convert the given CAN controller state to the corresponding CAN controller mode */
  if((uint8)ControllerMode < CANIF_CAN_CTRL_STATES_SIZE) /* Ensures a valid coversion; values greater or equal are also not defined in CanIf_ControllerModeType (s. CanIf_Types.h)
                                                            and are handled by the default case in the outer switch case from CanIf_ControllerModeIndicationAr403() */

  {
    canIfCtrlMode = CanIf_GetCanIfCtrlModeOfCanCtrlState(ControllerMode); /* PRQA S 4342 */ /* MD_CanIf_CastUintToEnum */
  }
  /* else - continue with the invalid not converted mode. This ensures same behaviour as with CanIf_ControllerModeType in CanIf_ControllerModeIndicationAr403() */

  /* #110 Call CanIf_ControllerModeIndicationAr403 with the corresponding CAN controller mode */
  CanIf_ControllerModeIndicationAr403(ControllerId, canIfCtrlMode);
}
#endif /* (CANIF_SUPPORT_CANDRV_EQC_AR_431 == STD_ON) */


/* \trace SPEC-9975, SPEC-9978, CANIF738 */
#if (CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  CanIf_GetTxConfirmationState()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_GetTxConfirmationState
Relation_Context:
CanIfTxConfPollSupport # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_GetTxConfirmationState(uint8 ControllerId)
{
  /* ----- Local Variables ---------------------------------------------- */
  CanIf_NotifStatusType retval = CANIF_NO_NOTIFICATION; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    /* \trace SPEC-9977 */
    errorId = CANIF_E_PARAM_CONTROLLER;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif /* (CANIF_DEV_ERROR_DETECT == STD_ON) */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Get the Tx confirmation state */
    retval = CanIf_GetTxConfState(ControllerId);
  }

  /* ----- Development Error Report ------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_GETTXCONFIRMATIONSTATE_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif /* (CANIF_PUBLIC_TX_CONFIRM_POLLING_SUPPORT == STD_ON) */


/* CANIF 784 */ /* CANIF 780 */
#if (CANIF_CHANGE_BAUDRATE_SUPPORT == STD_ON)
/* CANIF775 */
/**********************************************************************************************************************
 *  CanIf_CheckBaudrate()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_CheckBaudrate
Relation_Context:
CanIfChangeBaudrate # from this API #
Relation:
CanIfMultipleDriver
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfMultipleDriver
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_CheckBaudrate(uint8 ControllerId, CONST(uint16, CANIF_CONST) Baudrate)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
  CanIf_Can_CheckBaudrateFctType canCheckBaudrateFct;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    /* CANIF778 */
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
  /* #40 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(ControllerId))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#   endif
#  endif
  else
# endif
  {
    /* CANIF 786 */
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Pass the request to the CAN driver */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
    canCheckBaudrateFct = CanIf_GetCan_CheckBaudrateFctMap(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(ControllerId));
    if (canCheckBaudrateFct != NULL_PTR)
    {
      retval = canCheckBaudrateFct(CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId), Baudrate); /* SBSW_CANIF_41 */
    }
# else
    retval = Can_CheckBaudrate(ControllerId, Baudrate);
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_BAUDRATECHECK_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retval;
}


/* CANIF776 */
/**********************************************************************************************************************
 *  CanIf_ChangeBaudrate()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ChangeBaudrate
Relation_Context:
CanIfChangeBaudrate # from this API #
Relation:
CanIfMultipleDriver
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfMultipleDriver
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_ChangeBaudrate(uint8 ControllerId, CONST(uint16, CANIF_CONST) Baudrate)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
  CanIf_Can_ChangeBaudrateFctType canChangeBaudrateFct;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    /* CANIF782 */
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
  /* #40 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(ControllerId))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#   endif
#  endif
  else
# endif
  {
    /* \trace SPEC-21349 */
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Pass the request to the CAN driver */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
    canChangeBaudrateFct = CanIf_GetCan_ChangeBaudrateFctMap(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(ControllerId));
    if (canChangeBaudrateFct != NULL_PTR)
    {
      retval = canChangeBaudrateFct(CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId), Baudrate); /* SBSW_CANIF_41 */
    }
# else
    retval = Can_ChangeBaudrate(ControllerId, Baudrate);
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_BAUDRATECHANGE_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retval;
}
#endif /* CANIF_CHANGE_BAUDRATE_SUPPORT == STD_ON */


#if (CANIF_SET_BAUDRATE_API == STD_ON)
/* CANIF867 */
/**********************************************************************************************************************
 *  CanIf_SetBaudrate()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_SetBaudrate
Relation_Context:
CanIfChangeBaudrate # from this API #
Relation:
CanIfMultipleDriver
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfMultipleDriver
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetBaudrate(uint8 ControllerId, uint16 BaudRateConfigID)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
  CanIf_Can_SetBaudrateFctType canSetBaudrateFct;
# endif

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    /* SWS_CANIF_869 */
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
#  if ((CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON) && (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
  /* #40 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(ControllerId))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* SWS_CANIF_00868 */
    /* #100 Pass the request to the CAN driver */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
    canSetBaudrateFct = CanIf_GetCan_SetBaudrateFctMap(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(ControllerId));
    if (canSetBaudrateFct != NULL_PTR)
    {
      retval = canSetBaudrateFct(CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId), BaudRateConfigID); /* SBSW_CANIF_41 */
    }
# else
    retval = Can_SetBaudrate(ControllerId, BaudRateConfigID);
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_BAUDRATESET_API, errorId);
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retval;
}
#endif


#if (CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED)
/**********************************************************************************************************************
 *  CanIf_SetAddressTableEntry()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_SetAddressTableEntry
Relation_Context:
CanIfJ1939 # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_SetAddressTableEntry(uint8 ControllerId, uint8 intAddr, uint8 busAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  uint8 oldBusAddr;
  
/* ----- Development Error Report --------------------------------------- */
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
#   if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#   endif /* CANIF_USE_INIT_POINTER == STD_ON */
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  /* #40 Check if J1939 is activated for the ControllerId */
  else if (CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(ControllerId) == CANIF_J1939_DYN_ADDR_DISABLED) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* Checks for parameters busAddr and intAdr are not needed here; validity is secured by datatype and runtime check during CanIf_Init() see CM_CANIF_2 */
  else
#  endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Secure the consistency of of Rx and Tx lookup table */
    CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_6);

    /* #110 Adapt the dynamic address lookup table */
    /* If there exists already a relation for "intAddr" it is reset. It is no necessary to check for an existing relation of "busAddr"
    since the J1939Nm will not call this API for an "busAddr" that already has a relation. */
    oldBusAddr = CANIF_VAR_J1939TXADDR_LOOKUP(ControllerId, intAddr);
    if (oldBusAddr != CANIF_J1939_NULL_ADDR) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
    {
      CANIF_VAR_SET_J1939RXADDR_LOOKUP(ControllerId, oldBusAddr, CANIF_J1939_NULL_ADDR); /* SBSW_CANIF_48 */
    }
    CANIF_VAR_SET_J1939RXADDR_LOOKUP(ControllerId, busAddr, intAddr); /* SBSW_CANIF_48 */
    CANIF_VAR_SET_J1939TXADDR_LOOKUP(ControllerId, intAddr, busAddr); /* SBSW_CANIF_48 */

    CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_6);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    CanIf_Det_ReportError(CANIF_SETADDRESSTABLEENTRY_API, errorId);
  }
#  else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif /* CANIF_DEV_ERROR_REPORT == STD_ON */
}


/**********************************************************************************************************************
 *  CanIf_ResetAddressTableEntry()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ResetAddressTableEntry
Relation_Context:
CanIfJ1939 # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
FUNC(void, CANIF_CODE) CanIf_ResetAddressTableEntry(uint8 ControllerId, uint8 intAddr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  uint8 busAddr;

  /* ----- Development Error Checks ------------------------------------- */
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
#   if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else 
#   endif /* CANIF_USE_INIT_POINTER == STD_ON */
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  /* #40 Check if J1939 is activated for the ControllerId */
  else if (CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(ControllerId) == CANIF_J1939_DYN_ADDR_DISABLED) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* Check for parameter intAdr is not needed here; validity is secured by datatype and runtime check during CanIf_Init() see CM_CANIF_2 */
  else
#  endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Secure the consistency of of Rx and Tx lookup table */
    CanIf_EnterCritical(CANIF_EXCLUSIVE_AREA_6);

    /* #110 Reset the dynamic address lookup table */
    busAddr = CANIF_VAR_J1939TXADDR_LOOKUP(ControllerId, intAddr);
    
    CANIF_VAR_SET_J1939RXADDR_LOOKUP(ControllerId, busAddr, CANIF_J1939_NULL_ADDR); /* SBSW_CANIF_48 */
    CANIF_VAR_SET_J1939TXADDR_LOOKUP(ControllerId, intAddr, CANIF_J1939_NULL_ADDR); /* SBSW_CANIF_48 */

    CanIf_LeaveCritical(CANIF_EXCLUSIVE_AREA_6);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    CanIf_Det_ReportError(CANIF_RESETADDRESSTABLEENTRY_API, errorId);
  }
#  else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif /* CANIF_DEV_ERROR_REPORT == STD_ON */

}

/**********************************************************************************************************************
*  CanIf_GetTxAddressTableEntry()
*********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_GetTxAddressTableEntry
Relation_Context:
CanIfJ1939 # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
 *
*/
FUNC(uint8, CANIF_CODE) CanIf_GetTxAddressTableEntry(uint8 ControllerId, uint8 address)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  uint8 retval = address; /* return passed address if table entry can not be determined */

  /* ----- Development Error Checks ------------------------------------- */
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
#   if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#   endif /* CANIF_USE_INIT_POINTER == STD_ON */
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  /* #40 Check if J1939 is activated for the ControllerId */
  else if (CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(ControllerId) == CANIF_J1939_DYN_ADDR_DISABLED) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* Check for parameter address is not needed here; validity is secured by datatype */
  else
#  endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Return the Tx lookup table entry */
    retval = CANIF_VAR_J1939TXADDR_LOOKUP(ControllerId, address);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    CanIf_Det_ReportError(CANIF_GETTXADDRESSTABLEENTRY_API, errorId);
  }
#  else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif /* CANIF_DEV_ERROR_REPORT == STD_ON */
  return retval;
}

/**********************************************************************************************************************
*  CanIf_GetRxAddressTableEntry()
*********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_GetRxAddressTableEntry
Relation_Context:
CanIfJ1939 # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
* Internal comment removed.
 *
 *
 *
 *
 *
 *
*/
FUNC(uint8, CANIF_CODE) CanIf_GetRxAddressTableEntry(uint8 ControllerId, uint8 address)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  uint8 retval = address; /* return passed address if table entry can not be determined */

  /* ----- Development Error Checks ------------------------------------- */
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
#   if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#   endif /* CANIF_USE_INIT_POINTER == STD_ON */
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  /* #40 Check if J1939 is activated for the ControllerId */
  else if (CANIF_CFG_CTRLJ1939DYNADDRSUPPORT(ControllerId) == CANIF_J1939_DYN_ADDR_DISABLED) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* Check for parameter address is not needed here; validity is secured by datatype */
  else
#  endif /* CANIF_DEV_ERROR_DETECT == STD_ON */
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Return the Rx lookup table entry */
    retval = CANIF_VAR_J1939RXADDR_LOOKUP(ControllerId, address);
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG */
  {
    CanIf_Det_ReportError(CANIF_GETRXADDRESSTABLEENTRY_API, errorId);
  }
#  else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif /* CANIF_DEV_ERROR_REPORT == STD_ON */
  return retval;
}
#endif /* CANIF_J1939_DYN_ADDR_SUPPORT != CANIF_J1939_DYN_ADDR_DISABLED */


#if (CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  CanIf_RamCheckCorruptMailbox()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RamCheckCorruptMailbox
Relation_Context:
CanIfRamCheck  # from this API #
Relation:
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CANIF_CODE) CanIf_RamCheckCorruptMailbox(uint8 ControllerId, CanIf_HwHandleType HwHandle)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR; 
  CanIf_RamCheckCorruptMailboxIndFctType ramCheckCorruptMailboxIndFct = CANIF_CFG_RAMCHECKCORRUPTMAILBOXINDFCT();

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify if function pointer is valid */
  if (ramCheckCorruptMailboxIndFct != NULL_PTR)
  {
    /* #20 Call application notification function */ 
    ramCheckCorruptMailboxIndFct(ControllerId, HwHandle); /* SBSW_CANIF_13 */
  }
  else
  {
    errorId = CANIF_E_CONFIG;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */
  {
    CanIf_Det_ReportError(CANIF_RAMCHECKCORRUPTMAILBOX_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}


/**********************************************************************************************************************
 *  CanIf_RamCheckCorruptController()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RamCheckCorruptController
Relation_Context:
CanIfRamCheck # from this API #
Relation:
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, CANIF_CODE) CanIf_RamCheckCorruptController(uint8 ControllerId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR; 
  CanIf_RamCheckCorruptControllerIndFctType ramCheckCorruptControllerIndFct = CANIF_CFG_RAMCHECKCORRUPTCONTROLLERINDFCT();

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify if function pointer is valid */
  if (ramCheckCorruptControllerIndFct != NULL_PTR)
  {
    /* #20 Call application notification function */ 
    ramCheckCorruptControllerIndFct(ControllerId); /* SBSW_CANIF_13 */
  }
  else
  {
    errorId = CANIF_E_CONFIG;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */
  {
    CanIf_Det_ReportError(CANIF_RAMCHECKCORRUPTCONTROLLER_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}


/**********************************************************************************************************************
 *  CanIf_RamCheckExecute()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RamCheckExecute
Relation_Context:
CanIfRamCheck  # from this API #
Relation:
CanIfMultipleDriver
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfMultipleDriver
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_RamCheckExecute(uint8 ControllerId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
  CanIf_Can_RamCheckExecuteFctType can_RamCheckExecuteFct;
# endif

  /* ----- Development Error Checks --------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
  /* #40 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(ControllerId))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Call CAN driver to execute extended RAM check */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
    can_RamCheckExecuteFct = CanIf_GetCan_RamCheckExecuteFctMap(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(ControllerId));
  
    if(can_RamCheckExecuteFct != NULL_PTR)
    {
      can_RamCheckExecuteFct(CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId)); /* SBSW_CANIF_41 */
    }
    else
    {
      errorId = CANIF_E_CONFIG; 
    }
# else
    Can_RamCheckExecute(ControllerId);
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */
  {
    CanIf_Det_ReportError(CANIF_RAMCHECKEXECUTE_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}


/**********************************************************************************************************************
 *  CanIf_RamCheckEnableMailbox()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RamCheckEnableMailbox
Relation_Context:
CanIfRamCheck  # from this API #
Relation:
CanIfMultipleDriver
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfMultipleDriver
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfMultipleDriver
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_RamCheckEnableMailbox(uint8 ControllerId, CanIf_HwHandleType HwHandle)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
  CanIf_Can_RamCheckEnableMailboxFctType can_RamCheckEnableMailboxFct;
# endif

  /* ----- Development Error Checks --------------------------------------- */  
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
  /* #40 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(ControllerId))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Call CAN driver to enable the referred mailbox */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
    can_RamCheckEnableMailboxFct = CanIf_GetCan_RamCheckEnableMailboxFctMap(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(ControllerId));
  
    if (can_RamCheckEnableMailboxFct != NULL_PTR)
    {
      can_RamCheckEnableMailboxFct((Can_HwHandleType)HwHandle); /* SBSW_CANIF_41 */
    }
    else
    {
      errorId = CANIF_E_CONFIG;
    }
# else
    Can_RamCheckEnableMailbox(HwHandle);
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */
  {
    CanIf_Det_ReportError(CANIF_RAMCHECKENABLEMAILBOX_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* CANIF_DEV_ERROR_REPORT == STD_ON */
# if ((CANIF_DEV_ERROR_DETECT == STD_OFF) && (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_OFF))
  CANIF_DUMMY_STATEMENT(ControllerId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}


/**********************************************************************************************************************
 *  CanIf_RamCheckEnableController()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_RamCheckEnableController
Relation_Context:
CanIfRamCheck  # from this API #
Relation:
CanIfMultipleDriver
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfMultipleDriver
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_RamCheckEnableController(uint8 ControllerId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
  CanIf_Can_RamCheckEnableControllerFctType can_RamCheckEnableControllerFct;
# endif

  /* ----- Development Error Checks --------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if(CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
  /* #40 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(ControllerId))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Call CAN driver to enable the referred controller */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
    can_RamCheckEnableControllerFct = CanIf_GetCan_RamCheckEnableControllerFctMap(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(ControllerId));
  
    if (can_RamCheckEnableControllerFct != NULL_PTR)
    {
      can_RamCheckEnableControllerFct(CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId)); /* SBSW_CANIF_41 */
    }
    else
    {
      errorId = CANIF_E_CONFIG;
    }
# else
    Can_RamCheckEnableController(ControllerId);
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */
  {
    CanIf_Det_ReportError(CANIF_RAMCHECKENABLECONTROLLER_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}
#endif  /* (CANIF_EXTENDED_RAM_CHECK_SUPPORT == STD_ON) */


#if (CANIF_SET_PDU_RECEPTION_MODE_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  CanIf_SetPduReceptionMode()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_SetPduReceptionMode
Relation_Context:
CanIfRxPduReceptionMode  # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfVariant
CanIfVariant, CanIfDevErrorDetect
CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetPduReceptionMode(PduIdType id, CanIf_ReceptionModeType mode) 
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval  = E_NOT_OK;
  uint8 errorId = CANIF_E_NO_ERROR;
  CanIf_AnyIdxType CanIfRxPduId;

  /* ----- Development Error Checks ------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
# endif
# if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
  /* #20 Check if parameter id is valid */
  if (id >= CANIF_CFG_MAX_ULRXPDUS)
  {
    /* \trace SPEC-2007 */
    errorId = CANIF_E_INVALID_RXPDUID;
  }
  else
#  endif
# endif
  {
# if ((CANIF_CONFIG_VARIANT == CANIF_CFGVAR_POSTBUILDTIME) || (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON))
    /* for PB and LT configurations the RxPduId order between CanIf and higher layers may differ and needs to be indirected */
    CanIfRxPduId = CANIF_CFG_EXTRXPDUID2INTRXPDUID(id);
# else
    CanIfRxPduId = id;
# endif
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
    /* #30 Check if parameter CanIfRxPduId is valid */
    if (CanIfRxPduId >= CANIF_CFG_MAX_RXPDUS)
    {
      /* \trace SPEC-2007 */
      errorId = CANIF_E_INVALID_RXPDUID;
    }
    /* #40 Check if parameter mode is valid */
    else if ((mode != CANIF_RMT_RECEIVE_STOP) && (mode != CANIF_RMT_IGNORE_CONTINUE))
    {
      errorId = CANIF_E_INVALID_PDURECEPTIONMODE; /* PRQA S 2880 */ /* MD_MSR_Unreachable */
    }
    else
# endif
    {
      /* ----- Implementation ----------------------------------------------- */
      /* #100 Is for this CanIfRxPduId the reception mode adjustable? */
      if (CANIF_CFG_RX_ISRECEPTIONMODEPDU(CanIfRxPduId))
      {
        /* #110 Set the reception mode */
        CANIF_VAR_SET_PDU_RECEPTION_MODE(CANIF_CFG_RX_PDURECEPTIONMODEIDX(CanIfRxPduId), mode); /* SBSW_CANIF_42 */
        retval = E_OK;
      }
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
      else
      {
        errorId = CANIF_E_INVALID_RXPDUID;
      }
# endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */
  {
    CanIf_Det_ReportError(CANIF_SETPDURECEPTIONMODE_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}
#endif /* (CANIF_SET_PDU_RECEPTION_MODE_SUPPORT == STD_ON) */


# if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
/**********************************************************************************************************************
 *  CanIf_GetControllerErrorState()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_GetControllerErrorState
Relation_Context:
CanIfBusMirroringSupport  # from this API #
Relation:
CanIfMultipleDriver
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfMultipleDriver
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetControllerErrorState(uint8 ControllerId, P2VAR(Can_ErrorStateType, AUTOMATIC, CANIF_APPL_STATE_VAR) ErrorStatePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval  = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
  CanIf_Can_GetControllerErrorStateFctType can_GetControllerErrorStateFct;
# endif

  /* ----- Development Error Checks --------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if parameter ErrorStatePtr is valid */
  else if(ErrorStatePtr == NULL_PTR)
  {
    errorId = CANIF_E_PARAM_POINTER;
  }
  /* #40 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if(CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
  /* #50 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(ControllerId))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Request the error state of the CAN controller */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
    can_GetControllerErrorStateFct = CanIf_GetCan_GetControllerErrorStateFctMap(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(ControllerId));
  
    if (can_GetControllerErrorStateFct != NULL_PTR)
    {
      retval = can_GetControllerErrorStateFct(CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId), ErrorStatePtr); /* SBSW_CANIF_45 */
    }
    else
    {
      errorId = CANIF_E_CONFIG;
    }
# else
    retval = Can_GetControllerErrorState(ControllerId, ErrorStatePtr); /* SBSW_CANIF_5 */
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */
  {
    CanIf_Det_ReportError(CANIF_GETCONTROLLERERRORSTATE_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}


/**********************************************************************************************************************
 *  CanIf_GetControllerRxErrorCounter()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_GetControllerRxErrorCounter
Relation_Context:
CanIfBusMirroringSupport  # from this API #
Relation:
CanIfMultipleDriver
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfMultipleDriver
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetControllerRxErrorCounter(uint8 ControllerId, P2VAR(uint8, AUTOMATIC, CANIF_APPL_STATE_VAR) RxErrorCounterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval  = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
  CanIf_Can_GetControllerRxErrorCounterFctType can_GetControllerRxErrorCounterFct;
# endif

  /* ----- Development Error Checks --------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if parameter RxErrorCounterPtr is valid */
  else if(RxErrorCounterPtr == NULL_PTR)
  {
    errorId = CANIF_E_PARAM_POINTER;
  }
  /* #40 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if(CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
  /* #50 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(ControllerId))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Request the Rx error counter of the CAN controller */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
    can_GetControllerRxErrorCounterFct = CanIf_GetCan_GetControllerRxErrorCounterFctMap(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(ControllerId));
  
    if (can_GetControllerRxErrorCounterFct != NULL_PTR)
    {
      retval = can_GetControllerRxErrorCounterFct(CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId), RxErrorCounterPtr); /* SBSW_CANIF_45 */
    }
    else
    {
      errorId = CANIF_E_CONFIG;
    }
# else
    retval = Can_GetControllerRxErrorCounter(ControllerId, RxErrorCounterPtr); /* SBSW_CANIF_5 */
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */
  {
    CanIf_Det_ReportError(CANIF_GETCONTROLLERRXERRORCOUNTER_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}


/**********************************************************************************************************************
 *  CanIf_GetControllerTxErrorCounter()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_GetControllerTxErrorCounter
Relation_Context:
CanIfBusMirroringSupport  # from this API #
Relation:
CanIfMultipleDriver
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfMultipleDriver
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetControllerTxErrorCounter(uint8 ControllerId, P2VAR(uint8, AUTOMATIC, CANIF_APPL_STATE_VAR) TxErrorCounterPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval  = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
  CanIf_Can_GetControllerTxErrorCounterFctType can_GetControllerTxErrorCounterFct;
# endif

  /* ----- Development Error Checks --------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if parameter TxErrorCounterPtr is valid */
  else if(TxErrorCounterPtr == NULL_PTR)
  {
    errorId = CANIF_E_PARAM_POINTER;
  }
  /* #40 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
#  if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#   if(CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
  /* #50 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(ControllerId))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#   endif
#  endif
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Request the Tx error counter of the CAN controller */
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
    can_GetControllerTxErrorCounterFct = CanIf_GetCan_GetControllerTxErrorCounterFctMap(CANIF_CFG_CTRLUPTOLOWMAP_DRIVERINDEX(ControllerId));
  
    if (can_GetControllerTxErrorCounterFct != NULL_PTR)
    {
      retval = can_GetControllerTxErrorCounterFct(CANIF_CFG_CTRLUPTOLOWMAP_CONTROLLER(ControllerId), TxErrorCounterPtr); /* SBSW_CANIF_45 */
    }
    else
    {
      errorId = CANIF_E_CONFIG;
    }
# else
    retval = Can_GetControllerTxErrorCounter(ControllerId, TxErrorCounterPtr); /* SBSW_CANIF_5 */
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */
  {
    CanIf_Det_ReportError(CANIF_GETCONTROLLERTXERRORCOUNTER_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}


/**********************************************************************************************************************
 *  CanIf_EnableBusMirroring()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_EnableBusMirroring
Relation_Context:
CanIfBusMirroringSupport  # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, CANIF_CODE) CanIf_EnableBusMirroring(uint8 ControllerId, boolean MirroringActive)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retval  = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
# if (CANIF_DEV_ERROR_DETECT == STD_ON)
#  if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Change mirroring of the CAN controller */
    CanIf_ChangeMirroring(ControllerId, MirroringActive); /* SBSW_CANIF_3 */
    retval = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR) /* PRQA S 2992, 2996 */ /* MD_CanIf_ErrorId */
  {
    CanIf_Det_ReportError(CANIF_ENABLEBUSMIRRORING_API, errorId); /* PRQA S 2880 */ /* MD_MSR_Unreachable */
  }
# else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return retval;
}


/**********************************************************************************************************************
 *  Appl_GenericConfirmation()
 *********************************************************************************************************************/
/*
|<DataModelStart>| Appl_GenericConfirmation
Relation_Context:
CanIfBusMirroringSupport  # from this API #
Relation:
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfDevErrorDetect, CanIfMultipleDriver, CanIfVariant
CanIfPnEnable
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Can_ReturnType, CANIF_CODE) Appl_GenericConfirmation(uint8 Controller, P2CONST(Can_PduType, AUTOMATIC, CANIF_CBK_DRV_VAR) DataPtr) /* PRQA S 0624 */ /* MD_CanIf_Rule8.3 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#if (CANIF_DEV_ERROR_DETECT == STD_ON)
# if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
# endif
  /* #20 Check if parameter DataPtr is valid */
  if(DataPtr == NULL_PTR)
  {
    errorId = CANIF_E_PARAM_POINTER;
  }
  /* #30 Check if canTxPduId (=DataPtr->swPduHandle) is valid */
  else if (DataPtr->swPduHandle >=  CANIF_CFG_MAX_TXPDUS)
  {
    errorId = CANIF_E_PARAM_LPDU;
  }
  /* #40 Check if corresponding CAN controller is initialized */
  else if (CanIf_GetCtrlMode(CANIF_CFG_TX_CONTROLLER(DataPtr->swPduHandle)) == CANIF_CS_UNINIT )
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
# if (CANIF_MULTIPLE_CANDRV_SUPPORT == STD_ON)
#  if (CANIF_POSTBUILD_VARIANT_SUPPORT == STD_ON) 
  /* #50 Check if corresponding CanDrv is available in current identity */
  else if (!CANIF_CFG_IS_CANDRVINDEXVALID(CANIF_CFG_TX_CONTROLLER(DataPtr->swPduHandle)))
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
#  endif
# endif
  else
#endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Process only if CAN controller is active */
    if (CanIf_GetCtrlMode(CANIF_CFG_TX_CONTROLLER(DataPtr->swPduHandle)) == CANIF_CS_STARTED)
    {
      if ( (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(DataPtr->swPduHandle)) == CANIF_GET_ONLINE) ||
             (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(DataPtr->swPduHandle)) == CANIF_GET_TX_ONLINE)
# if (CANIF_PN_WU_TX_PDU_FILTER == STD_ON)
        || (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(DataPtr->swPduHandle)) == CANIF_GET_ONLINE_WAKF)
        || (CANIF_VAR_GETPDUMODE(CANIF_CFG_TX_CONTROLLER(DataPtr->swPduHandle)) == CANIF_GET_TX_ONLINE_WAKF)
# endif
         )
      {
        /* #200 Report CAN frame to mirror module, if mirroring is enabled for the CAN controller */
        if(CanIf_IsMirroringEnabled(CANIF_CFG_TX_CONTROLLER(DataPtr->swPduHandle)))
        {
          Mirror_ReportCanFrame(CANIF_CFG_TX_CONTROLLER(DataPtr->swPduHandle), DataPtr->id, DataPtr->length, DataPtr->sdu); /* SBSW_CANIF_5 */
        }
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_APPL_GENERICCONFIRMATION_API, errorId);
  }
#else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  CANIF_DUMMY_STATEMENT(Controller); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return CAN_OK; /* Higher layer confirmation will be called afterwards (CanIf_TxConfirmation()) by the CAN driver */
}
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */


#if (CANIF_ENABLE_SECURITY_EVENT_REPORTING == STD_ON)
/**********************************************************************************************************************
 *  CanIf_ControllerErrorStatePassive()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ControllerErrorStatePassive
Relation_Context:
CanIfSecurityEventReporting  # from this API #
Relation:
CanIfSecurityEventPassive
CanIfSecurityEventPassive, CanIfDevErrorDetect
CanIfSecurityEventPassive, CanIfDevErrorDetect, CanIfVariant
CanIfSecurityEventPassive, CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_ControllerErrorStatePassive(uint8 ControllerId, uint16 RxErrorCounter, uint16 TxErrorCounter)
{
# if defined (CANIF_SEV_ERRORSTATE_PASSIVE)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  IdsM_ContextDataType contextData[CANIF_CONTEXTDATA_SIZE_PASSIVE];

  /* ----- Development Error Checks ------------------------------------- */
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
#   if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#   endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Add ControllerId to context data */
    contextData[CANIF_CONTEXTDATA_CTRLID_POS] = ControllerId; /* SBSW_CANIF_50 */

    /* #110 Determine error counter threshold result for context data */
    if ((RxErrorCounter > CANIF_ERROR_COUNTER_THRESHOLD) && (TxErrorCounter <= CANIF_ERROR_COUNTER_THRESHOLD))
    {
      contextData[CANIF_CONTEXTDATA_THRESHOLD_POS] = CANIF_RX_THRESHOLD_EXCEEDED; /* SBSW_CANIF_50 */
    }
    else if((RxErrorCounter <= CANIF_ERROR_COUNTER_THRESHOLD) && (TxErrorCounter > CANIF_ERROR_COUNTER_THRESHOLD))
    {
      contextData[CANIF_CONTEXTDATA_THRESHOLD_POS] = CANIF_TX_THRESHOLD_EXCEEDED; /* SBSW_CANIF_50 */
    }
    else /* RxErrorCounter and TxErrorCounter are bigger or smaller than the threshold */
    {
      contextData[CANIF_CONTEXTDATA_THRESHOLD_POS] = CANIF_RX_TX_THRESHOLD_EXCEEDED; /* SBSW_CANIF_50 */
    }

    /* #120 Report security event to the IdsM */
    IdsM_SetSecurityEventWithContextData(CANIF_SEV_ERRORSTATE_PASSIVE, contextData, CANIF_CONTEXTDATA_SIZE_PASSIVE); /* SBSW_CANIF_5 */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_CONTROLLERERRORSTATEPASSIVE_API, errorId);
  }
#  else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif /* CANIF_DEV_ERROR_REPORT == STD_ON */
# else
  CANIF_DUMMY_STATEMENT(ControllerId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CANIF_DUMMY_STATEMENT(RxErrorCounter); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CANIF_DUMMY_STATEMENT(TxErrorCounter); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* defined (CANIF_SEV_ERRORSTATE_PASSIVE) */
}


/**********************************************************************************************************************
 *  CanIf_ErrorNotification()
 *********************************************************************************************************************/
/*
|<DataModelStart>| CanIf_ErrorNotification
Relation_Context:
CanIfSecurityEventReporting  # from this API #
Relation:
CanIfSecurityEventRx, CanIfSecurityEventTx
CanIfDevErrorDetect
CanIfDevErrorDetect, CanIfVariant
CanIfSecurityEventRx
CanIfSecurityEventTx
CanIfDevErrorDetect
Parameter_PreCompile:
Parameter_Data:
Constrain:
|<DataModelEnd>|
*/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, CANIF_CODE) CanIf_ErrorNotification(uint8 ControllerId, Can_ErrorType CanError)
{
# if (defined (CANIF_SEV_RX_ERROR_DETECTED) || defined (CANIF_SEV_TX_ERROR_DETECTED))
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = CANIF_E_NO_ERROR;
  IdsM_ContextDataType contextData[CANIF_CONTEXTDATA_SIZE_CANERROR];

  /* ----- Development Error Checks ------------------------------------- */
#  if (CANIF_DEV_ERROR_DETECT == STD_ON)
#   if (CANIF_USE_INIT_POINTER == STD_ON)
  /* #10 Check if CanIf is initialized at all */
  if (CanIf_SystemInit == FALSE)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  else
#   endif
  /* #20 Check if parameter ControllerId is valid */
  if (ControllerId >= CANIF_CFG_MAX_CONTROLLER)
  {
    errorId = CANIF_E_PARAM_CONTROLLERID;
  }
  /* #30 Check if CAN controller is initialized */
  else if (CanIf_GetCtrlMode(ControllerId) == CANIF_CS_UNINIT)
  {
    /* \trace SPEC-5238 */
    errorId = CANIF_E_UNINIT;
  }
  /* #40 Check if parameter CanError is valid */
  else if((CanError != CAN_ERROR_BIT_MONITORING1) && (CanError != CAN_ERROR_BIT_MONITORING0) &&
          (CanError != CAN_ERROR_BIT) && (CanError != CAN_ERROR_CHECK_ACK_FAILED) &&
          (CanError != CAN_ERROR_ACK_DELIMITER) && (CanError != CAN_ERROR_ARBITRATION_LOST) &&
          (CanError != CAN_ERROR_OVERLOAD) && (CanError != CAN_ERROR_CHECK_FORM_FAILED) &&
          (CanError != CAN_ERROR_CHECK_STUFFING_FAILED) && (CanError != CAN_ERROR_CHECK_CRC_FAILED) &&
          (CanError != CAN_ERROR_BUS_LOCK))
  {
    errorId = CANIF_E_PARAM_CAN_ERROR;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Add ControllerId and CanError to context data */
    contextData[CANIF_CONTEXTDATA_CTRLID_POS] = ControllerId; /* SBSW_CANIF_51 */
    contextData[CANIF_CONTEXTDATA_CANERROR_POS] = (IdsM_ContextDataType)CanError; /* SBSW_CANIF_51 */

#  if defined (CANIF_SEV_RX_ERROR_DETECTED)
    if((CanError == CAN_ERROR_CHECK_FORM_FAILED) || (CanError == CAN_ERROR_CHECK_STUFFING_FAILED) ||
       (CanError == CAN_ERROR_CHECK_CRC_FAILED) || (CanError == CAN_ERROR_BUS_LOCK))
    {
      /* #200 Report rx security event to the IdsM (if configured) */
      IdsM_SetSecurityEventWithContextData(CANIF_SEV_RX_ERROR_DETECTED, contextData, CANIF_CONTEXTDATA_SIZE_CANERROR); /* SBSW_CANIF_5 */
    }
#  endif

#  if defined (CANIF_SEV_TX_ERROR_DETECTED)
    if((CanError == CAN_ERROR_BIT_MONITORING1) || (CanError == CAN_ERROR_BIT_MONITORING0) ||
       (CanError == CAN_ERROR_BIT) || (CanError == CAN_ERROR_CHECK_ACK_FAILED) ||
       (CanError == CAN_ERROR_ACK_DELIMITER) || (CanError == CAN_ERROR_ARBITRATION_LOST) ||
       (CanError == CAN_ERROR_OVERLOAD))
    {
      /* #300 Report tx security event to the IdsM (if configured) */
      IdsM_SetSecurityEventWithContextData(CANIF_SEV_TX_ERROR_DETECTED, contextData, CANIF_CONTEXTDATA_SIZE_CANERROR); /* SBSW_CANIF_5 */
    }
#  endif
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (CANIF_DEV_ERROR_REPORT == STD_ON)
  if (errorId != CANIF_E_NO_ERROR)
  {
    CanIf_Det_ReportError(CANIF_ERRORNOTIFICATION_API, errorId);
  }
#  else
  CANIF_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif /* CANIF_DEV_ERROR_REPORT == STD_ON */
# else
  CANIF_DUMMY_STATEMENT(ControllerId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  CANIF_DUMMY_STATEMENT(CanError); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif /* defined (CANIF_SEV_RX_ERROR_DETECTED) || defined (CANIF_SEV_TX_ERROR_DETECTED) */
}
#endif /* CANIF_ENABLE_SECURITY_EVENT_REPORTING == STD_ON */

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 *********************************************************************************************************************/

/* Justification for module-specific MISRA deviations:

MD_CanIf_Rule8.3:
  Reason: For parameter DataPtr the typedef Can_<Infix>_PduInfoPtrType with a possible infix is not used to be compatible 
          with inxfixed and not infixed CAN drivers.
  Risk: Different datatypes for parameter DataPtr in definition and declaration.
  Prevention: Code inspection.

MD_CanIf_Rule8.9:
  Reason: Constant tables are kept at a central location for a better overview and maintenance.
  Risk: Name conflict.
  Prevention: Compiler optimize and check for name conflicts.

MD_CanIf_Rule10.5_4342:
  Reason: The different mode types are abstracted with the enum CanIf_PduGetModeType/ CanIf_PduSetModeType.
          But a lookup table CanIfState from a basic unsigned data type is used to get a requested mode.
          This leads to a runtime efficient code.
  Risk: Inconsistency between mode types in CanIf_PduGetModeType/ CanIf_PduSetModeType enum and CanIfState lookup table.
  Prevention: Code inspection and test of the different configurations in the component test.

MD_CanIf_CastUintToEnum:
  Reason: Element of enum CanIf_ControllerModeType is determinded from look up table CanIf_CanCtrlState2CanIfCtrlModeLookUp[],
          which contains the enum elements as unsigned integer values.
  Risk: Inconsistency between elements from CanIf_ControllerModeType enum and unsigned integer values in lookup table
        CanIf_CanCtrlState2CanIfCtrlModeLookUp[].
  Prevention: Code inspection and test of the different configurations in the component test.

MD_CanIf_CastEnumToDiffEnum:
  Reason: Ensures same behaviour in function for both enums CanIf_ControllerModeType and Can_ControllerStateType, if passed
          controller mode is not valid. Both enums have the same valid value range.
  Risk: None.
  Prevention: Code inspection and test of the different configurations in the component test.

MD_CanIf_Rule11.1:
  Reason: Cast to different pointer type required to avoid compiler warning due to usage of a union of function pointer types.
          In a multiple CAN Driver configuration with different CAN Driver EQCs multiple violations within one code line in GenData table CanIf_CanDrvFctTbl[] are possible.
  Risk: None, because function pointer is casted to correct type before function is called.
  Prevention: Test of the different variants in the component test.

MD_CanIf_Rule11.8:
  Reason: Pointer cast to P2VAR is necessary to avoid compiler warning because "SduDataPtr" within "PduInfoType" is defined
          as P2VAR while "CanSduPtr" is P2CONST.
  Risk: CAN data could be modified after cast to P2VAR.
  Prevention: Code inspection and test of the different variants in the component test.

MD_CanIf_ErrorId:
  Reason: Usage of defined API pattern.
  Risk: Statements in this 'if' block are not executed.
  Prevention: Code inspection and test of the different configurations in the component test.

MD_CanIf_Rule14.3_GenData:
  Reason: Configuration related constant data.
  Risk: Statements in the 'if' or 'else' block are not executed.
  Prevention: Code inspection and test of the different configurations in the component test.

MD_CanIf_RxPduConstData:
  Reason: Configuration related Rx-PDU constant data leads to a static true or false result 
          of the affected logical operator or of the 'if' controlling expression.
  Risk: Statements in the 'if' or 'else' block are not executed.
  Prevention: Code inspection and test of the different configurations in the component test.

MD_CanIf_CanTrcvMapConstData:
  Reason: Configuration related CanTrcv mapping constant data.
  Risk: Statements in the 'if' block are not executed.
  Prevention: Code inspection and test of the different configurations in the component test.

MD_CanIf_J1939ConstData:
  Reason: Configuration related J1939 constant data leads to a static false result of the
          affected logical operator or of the 'if' controlling expression.
  Risk: Statements in the 'if' block are not executed.
  Prevention: Code inspection and test of the different configurations in the component test.

MD_CanIf_CanDrvEqcConstData:
  Reason: Configuration related constant data causes the "if" expression to always be false.
  Risk: Statements in the 'if' block are not executed.
  Prevention: Code inspection and test of additional configurations with multiple CAN Drivers in
              the multiple CAN Driver component test to cover the if and else block.

MD_CanIf_CanDrvEqcUnreachableCode:
  Reason: Configuration related constant data leads to unreachable code in 'if' block.
  Risk: Statements in the 'if' block are not executed.
  Prevention: Code inspection and test of additional configurations with multiple CAN Drivers in
              the multiple CAN Driver component test to cover the if and else block.

MD_CanIf_MultipleBreak:
  Reason: More than one break statement is used to terminate the loop during the search algorithm.
          This leads to more efficient code which is easier to read and maintain.
  Risk: Relevant fragments of code not executed.
  Prevention: Code inspection.

MD_CanIf_Rule17.8:
  Reason: The direct modification of the function parameter leads to more efficient code which is easier to read and maintain.
  Risk: Incorrect expectation of the value of the modified function parameter.
  Prevention: Code inspection and test of the different configurations in the component test.

MD_CanIf_Dir4.1_2842:
  Reason: Possible out of bounds read access.
  Risk: Wrong data access.
  Prevention: Test of the different configurations in the component test.

MD_CanIf_FctCall:
  Reason: Local function only initalizes variables from generated data.
  Risk: None.
  Prevention: Code inspection.

*/

/**********************************************************************************************************************
 *  SBSW JUSTIFICATIONS
 *********************************************************************************************************************/

/* SBSW_JUSTIFICATION_BEGIN

\ID SBSW_CANIF_1
\DESCRIPTION Access to txPduDataWithChecksumPtr with index which is based on DLC.
\COUNTERMEASURE \R Index is checked during runtime not to exceed the bounds of txPduDataWithChecksumPtr. Performed check: (PduInfoPtr->SduLength) > CANIF_CFG_MAXTXDLC_PLUS_DATACHECKSUM.

\ID SBSW_CANIF_3
\DESCRIPTION Access to CanIf_CtrlMode, CanIf_PduMode, CanIf_WakeUpValidationState, CanIf_PnWuTxPduFilterActive, CanIf_TxConfState,
             CanIf_BusMirroringActive with index controlled by CANIF_CFG_MAX_CONTROLLER which is mapped to the CanIf_GetSizeOfCtrlStates()
             which is provided by ComStackLib.
\COUNTERMEASURE \N Qualified use-case CSL01/CSL03 of ComStackLib.

\ID SBSW_CANIF_4
\DESCRIPTION Write access to dereferenced pointer ControllerModePtr.
\COUNTERMEASURE \N The caller of the function CanIf_GetControllerMode must ensure that the pointer passed to the
                   parameter ControllerModePtr is able to record a value of the type CanIf_ControllerModeType.

\ID SBSW_CANIF_5
\DESCRIPTION Function call with referenced local variable/structure respectively passed-through parameter.
\COUNTERMEASURE \N Pointer references stack variable and is therefore always valid. For passed-through datapointer the caller ensures that the
                   pointer is valid.

\ID SBSW_CANIF_6
\DESCRIPTION Access to CanIf_TxQueue via PduId which is indirected over CanIf_TxPduQueueIndex.
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib. Pdu is passed through, caller ensures that PduId is valid.
                   For API CanIf_CancelTxConfirmation() which is called from outside, the PduId is checked via CSL01 with reference value "CanIf_GetSizeOfTxPduConfig()".

\ID SBSW_CANIF_7
\DESCRIPTION VStdLib_MemCpy function call with parameter destination buffer pointer and length.
             The array for buffering the data is dimensioned with CANIF_STATIC_BUFFER_SIZE or CANIF_CFG_MAXRXDLC_PLUS_MAXMETADATALEN respectively.
\COUNTERMEASURE \R  CanIf_TransmitSubHandleTxResBusy(): runtime check -> localPduPtr->length vs. CANIF_STATIC_BUFFER_SIZE 
                    CanIf_HlIndication(): runtime check -> (CanDlc + CANIF_CFG_RX_RXMETADATALENGTH(PduId)) vs. CANIF_CFG_MAXRXDLC_PLUS_MAXMETADATALEN
                    CanIf_CancelTxConfirmation(): runtime check -> PduInfoPtr->length vs. CANIF_STATIC_BUFFER_SIZE

\ID SBSW_CANIF_8
\DESCRIPTION Access to CanIf_PnWuTxPduFilterActive, CanIf_TxConfState with index ControllerId which is indirected from  Hrh/CanTxPduId via 
                    CanIf_GetCtrlStatesIdxOfMailBoxConfig()/CanIf_GetCtrlStatesIdxOfTxPduConfig(). Hrh is checked via CanIf_GetSizeOfMailBoxConfig(),
                    CanTxPduId is checked via CanIf_GetSizeOfTxPduConfig().
\COUNTERMEASURE \N Qualified use-case CSL01/CSL03 of ComStackLib.

\ID SBSW_CANIF_9
\DESCRIPTION Write access to the content of an object that is referenced by a pointer parameter.
\COUNTERMEASURE \N Caller ensures validity of pointer parameter.

\ID SBSW_CANIF_10
\DESCRIPTION Function call CanIf_Indirect_HlIndication(). CanSduPtr is just forwarded without modification.
\COUNTERMEASURE \N Caller of CanIf_RxIndication ensures that CanSduPtr is valid.

\ID SBSW_CANIF_11
\DESCRIPTION Function call via function pointer: Access to CanIf_TxConfirmationFctList via indirection over CanIf_TxPduConfig with parameter CanTxPduId to 
             get the required function pointer. The indirection is ensured via qualified use-case CSL03 of ComStackLib.
\COUNTERMEASURE \S For the generated function list CanIf_TxConfirmationFctList, it must be secured that a valid function callback is provided with the valid signature.
                   SMI-350

\ID SBSW_CANIF_12
\DESCRIPTION Function call via function pointer: Access to CanIf_RxIndicationFctList via indirection over CanIf_RxPduConfig with parameter CanRxPduId to 
             get the required function pointer. The indirection is ensured via qualified use-case CSL03 of ComStackLib.
             Function call with parameter data pointer: Data pointer CanSduPtr/pduinfo is defined as pointer to const, so no write access is possible.
\COUNTERMEASURE \S For the generated function list CanIf_RxIndicationFctList, it must be secured that a valid function callback is provided with the vailid signature.
                   SMI-349

\ID SBSW_CANIF_13
\DESCRIPTION Function call via function pointer
\COUNTERMEASURE \S  For the generated functions CanIf_BusOffNotificationFctPtr, CanIf_CtrlModeIndicationFctPtr, CanIf_TrcvModeIndicationFctPtr, CanIf_ConfirmPnAvailabilityFctPtr,
                    CanIf_ClearTrcvWufFlagIndicationFctPtr, CanIf_CheckTrcvWakeFlagIndicationFctPtr, CanIf_WakeUpValidationFctPtr, CanIf_RamCheckCorruptControllerIndFctPtr,
                    CanIf_RamCheckCorruptMailboxIndFctPtr, CanIf_DataChecksumRxErrFctPtr it must be secured that a valid function callback is provided with the valid signature.
                    SMI-746

\ID SBSW_CANIF_14
\DESCRIPTION Access to CanIf_DynTxId via PDU ID which is indirected over CanIf_TxPduStatic2DynamicIndirection.
\COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_CANIF_15
\DESCRIPTION Write access to variable CanIf_TxQueueIndex2FifoEl, CanIf_TxFifoQueueBase with index counter.
\COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_CANIF_16
\DESCRIPTION Write access to a local variable with index.
\COUNTERMEASURE \R Index is checked during runtime to not exceed range of the variable.

\ID SBSW_CANIF_17
\DESCRIPTION Write access to CanIf_J1939RxAddr_Lookup/CanIf_J1939TxAddr_Lookup with index (incl. configured offset), which is checked against CanIf_GetSizeOfJ1939RxAddr_Lookup/CanIf_GetSizeOfJ1939TxAddr_Lookup at runtime.
\COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_CANIF_18
\DESCRIPTION VStdLib_MemCpy function call with parameter destination buffer pointer and length. 
\COUNTERMEASURE \N  CanIf_TransmitSubHandleTxResBusy(): runtime check -> localPduPtr->length vs. CANIF_CFG_TXBUFFERPRIOBYCANID_STATFDQUEUE_MAXDATALENGTH(localPduPtr->swPduHandle) 
                    CanIf_CancelTxConfirmation():  runtime check -> localPduPtr->length vs. CANIF_CFG_TXBUFFERPRIOBYCANID_STATFDQUEUE_MAXDATALENGTH(localPduPtr->swPduHandle) 
                    Qualified use-case CSL02 of ComStackLib (union of messages and RAM array). "swPduHandle" is verified to be valid by caller of function.

\ID SBSW_CANIF_19
\DESCRIPTION Write access to WakeUpValidationStateOfCtrlStates with index ControllerId which is indirected from HRH.
\COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib.

\ID SBSW_CANIF_20
\DESCRIPTION Write access to CanIf_TxQueueFlags with queueElementIdx.
\COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib:  queueElementIdx is checked against CanIf_GetSizeOfTxQueueFlags() at runtime.

\ID SBSW_CANIF_21
\DESCRIPTION Function call with parameter data pointer. The data pointer is passed through the function with the same type.
\COUNTERMEASURE \N The compiler ensures that no memory violation occurs.

\ID SBSW_CANIF_22
\DESCRIPTION Function call via function pointer. Function is indirected with TransceiverId over CanIf_CanTrcvFctTbl.
\COUNTERMEASURE \N The assurance to get a valid function pointer is a qualified use-case CSL03 of ComStackLib.

\ID SBSW_CANIF_23
\DESCRIPTION Write access to CanIf_PduRxMode with "index".
\COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

\ID SBSW_CANIF_24
\DESCRIPTION Initialization write-access to CanIf_CtrlMode with index which is limited by sizeof of corresponding type and corresponding variable. Hence NO out-of-bounce access can occur.
\COUNTERMEASURE \N Verified with review and the compiler ensures that no memory violation occurs.

\ID SBSW_CANIF_25
\DESCRIPTION Write access to CanIf_TxBufferPrioByCanIdBase.  PduId -> CanIf_GetMailBoxConfigIdxOfTxPduConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig ->
\                  CanIf_GetTxBufferPrioByCanIdBaseIdxOfTxBufferPrioByCanIdConfig
\COUNTERMEASURE \N TxPduId is checked against CanIf_GetSizeOfTxPduConfig. The following indirections are ensured via qualified use-case CSL03 of ComStackLib.

\ID SBSW_CANIF_26
\DESCRIPTION Write access to CanIf_TxQueueFlags. ControllerId -> CanIf_GetMappedTxBuffersConfigStartIdxOfCanIfCtrlId2MappedTxBuffersConfig ->
                   CanIf_GetMailBoxConfigIdxOfMappedTxBuffersConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig -> CanIf_GetTxQueueFlagsStartIdxOfTxBufferPrioByCanIdConfig ->
                   CanIf_SetTxQueueFlags.
\COUNTERMEASURE \N ControllerId is checked against CanIf_GetSizeOfCtrlStates. The following indirections are ensured via qualified use-case CSL03 of ComStackLib.

\ID SBSW_CANIF_27
\DESCRIPTION Write access to CanIf_TxBufferPrioByCanIdBase. ControllerId -> CanIf_GetMappedTxBuffersConfigStartIdxOfCanIfCtrlId2MappedTxBuffersConfig ->
                   CanIf_GetMailBoxConfigIdxOfMappedTxBuffersConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig ->
                   CanIf_GetTxBufferPrioByCanIdBaseIdxOfTxBufferPrioByCanIdConfig -> CanIf_SetTxQueueCounter
\COUNTERMEASURE \N ControllerId is checked against CanIf_GetSizeOfCtrlStates. The following indirections are ensured via qualified use-case CSL03 of ComStackLib.

\ID SBSW_CANIF_31
\DESCRIPTION Function call with referenced pointer on local RAM array txPduDataWithChecksum.
             txPduDataWithChecksum is dimensioned with CANIF_CFG_MAXTXDLC_PLUS_DATACHECKSUM, the index to write on txPduDataWithChecksum is controlled by PduInfoPtr->SduLength.
\COUNTERMEASURE \R Runtime check: PduInfoPtr->SduLength is checked against CANIF_CFG_MAXTXDLC_PLUS_DATACHECKSUM. 

\ID SBSW_CANIF_32
\DESCRIPTION Write access to CanIf_TxFifoQueueData with index (queueDataStartIdx + DLC) .
\COUNTERMEASURE \R  A runtime check ensures that (queueDataStartIdx + DLC) does not exceed the bounds of CanIf_TxFifoQueueData via a check against CanIf_GetSizeOfTxFifoQueueData().

\ID SBSW_CANIF_33
\DESCRIPTION Write access to CanIf_TxBufferFifoBase with index txBufferFifoBaseIdx.
             ControllerId -> CanIf_GetMappedTxBuffersConfigStartIdxOfCanIfCtrlId2MappedTxBuffersConfig -> CanIf_GetMailBoxConfigIdxOfMappedTxBuffersConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig ->
             CanIf_GetTxBufferFifoBaseIdxOfTxBufferFifoConfig.
\COUNTERMEASURE \N ControllerId is checked against CanIf_GetSizeOfCtrlStates by caller function. The following indirections are ensured via qualified use-case CSL03 of ComStackLib.

\ID SBSW_CANIF_34
\DESCRIPTION Write access to CanIf_TxFifoQueueBase with index (readIdx/writeIdx + txFifoQueueBaseStartIdx).
             ControllerId -> CanIf_GetMappedTxBuffersConfigStartIdxOfCanIfCtrlId2MappedTxBuffersConfig -> CanIf_GetMailBoxConfigIdxOfMappedTxBuffersConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig ->
             CanIf_GetTxFifoQueueBaseStartIdxOfTxBufferFifoConfig -> txFifoQueueBaseStartIdx.
\COUNTERMEASURE \N  ControllerId is checked against CanIf_GetSizeOfCtrlStates by caller function. The following indirections to get txFifoQueueBaseStartIdx are ensured via qualified use-case CSL03
             of ComStackLib. readIdx/writeIdx is checked against the macro CanIf_GetTxFifoQueueBaseLengthOfTxBufferFifoConfig while writing CanIf_TxBufferFifoBase.
             For (CanIf_GetTxFifoQueueBaseLengthOfTxBufferFifoConfig + CanIf_GetTxFifoQueueBaseStartIdxOfTxBufferFifoConfig) a qualified CSL03 use-case of ComStackLib ensures that the index is
             valid to access CanIf_TxFifoQueueBase.

\ID SBSW_CANIF_35
\DESCRIPTION Write access to CanIf_TxQueueIndex2FifoEl with index. CanIf_GetTxFifoQueueBase -> CanIf_GetTxQueueIdxOfTxPduQueueIndex -> CanIf_SetTxQueueIndex2FifoEl.
\COUNTERMEASURE \N  The input parameter for CanIf_GetTxFifoQueueBase() is ensured with SBSW_CANIF_34. The following indirections to get the index for CanIf_SetTxQueueIndex2FifoEl are ensured via
             qualified use-case CSL03 of ComStackLib.
             
\ID SBSW_CANIF_36
\DESCRIPTION Write access to CanIf_TxQueueIndex2FifoEl with index. CanTxPduId/PduId/localPduPtr->swPduHandle -> CanIf_GetTxQueueIdxOfTxPduQueueIndex -> CanIf_SetTxQueueIndex2FifoEl.
\COUNTERMEASURE \N The input parameter CanTxPduId/PduId/localPduPtr->swPduHandle is checked against CanIf_GetSizeOfTxPduConfig(). The following indirections to get the index for CanIf_SetTxQueueIndex2FifoEl are ensured via
             qualified use-case CSL03 of ComStackLib.

\ID SBSW_CANIF_37
\DESCRIPTION Write access to CanIf_TxBufferFifoBase with index txBufferFifoBaseIdx. 
             CanTxPduId/PduId -> CanIf_GetMailBoxConfigIdxOfTxPduConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig -> CanIf_GetTxBufferFifoBaseIdxOfTxBufferFifoConfig.
\COUNTERMEASURE \N The input parameter CanTxPduId/PduId is checked by caller respectively in corresponding function against CanIf_GetSizeOfTxPduConfig(). The following indirections to get the index for CanIf_TxBufferFifoBase are ensured via
             qualified use-case CSL03 of ComStackLib.

\ID SBSW_CANIF_38
\DESCRIPTION Write access to CanIf_TxFifoQueueBase with index (readIdx + txFifoQueueBaseStartIdx).
             readIdx:
             PduId -> CanIf_GetMailBoxConfigIdxOfTxPduConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig -> CanIf_GetTxBufferFifoBaseIdxOfTxBufferFifoConfig -> CanIf_GetTxBufferFifoBase (.eReadIdx)
             txFifoQueueBaseStartIdx: 
             PduId -> CanIf_GetMailBoxConfigIdxOfTxPduConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig -> CanIf_GetTxFifoQueueBaseStartIdxOfTxBufferFifoConfig
\COUNTERMEASURE \N PduId is checked against CanIf_GetSizeOfTxPduConfig. readIdx is checked against the macro CanIf_GetTxFifoQueueBaseLengthOfTxBufferFifoConfig while writing CanIf_TxBufferFifoBase.
             The index to access CanIf_TxBufferFifoBase is ensured via qualified use-case CSL03 of ComStackLib.
             The indirections to get txFifoQueueBaseStartIdx are ensured via qualified use-case CSL03 of ComStackLib.
             For (CanIf_GetTxFifoQueueBaseLengthOfTxBufferFifoConfig + CanIf_GetTxFifoQueueBaseStartIdxOfTxBufferFifoConfig) a qualified CSL03 use-case of ComStackLib ensures that the index is
             valid to access CanIf_TxFifoQueueBase.

\ID SBSW_CANIF_39
\DESCRIPTION Write access to CanIf_TxFifoQueueBase with index (FifoQueueBaseIdx + txFifoQueueBaseStartIdx).
             FifoQueueBaseIdx: 
             CanTxPduId -> CanIf_GetTxQueueIdxOfTxPduQueueIndex -> CanIf_GetTxQueueIndex2FifoEl
             txFifoQueueBaseStartIdx: 
             CanTxPduId -> CanIf_GetMailBoxConfigIdxOfTxPduConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig  -> CanIf_GetTxFifoQueueBaseStartIdxOfTxBufferFifoConfig
\COUNTERMEASURE \N CanTxPduId -> is checked against CanIf_GetSizeOfTxPduConfig. Qualified use-case CSL03 ensures that for each TxQueueIndex2FifoEl/TxFifoQueueBaseStartIdx pair calculated with the same valid
             CanTxPduId does not violate the bounds of CanIf_TxFifoQueueBase.

\ID SBSW_CANIF_40
\DESCRIPTION Write access to CanIf_TxQueueData with index ( TxQueueDataStartIdxOfTxQueueIndex2DataStartStop + DLC ).
\COUNTERMEASURE \N  CanTxPduId/PduInfoPtr->swPduHandle -> checked against CanIf_GetSizeOfTxPduConfig. DLC is checked against TxQueueDataLengthOfTxQueueIndex2DataStartStop. Qualified use-case CSL03 ensures that
             ( TxQueueDataStartIdx() + TxQueueDataLength() ) do not violate the bounds of CanIf_TxQueueData.

\ID SBSW_CANIF_41
\DESCRIPTION Function call via function pointer: Access to CanIf_CanDrvFctTbl[] via indirection over CanIf_GetCanDrvFctTblIdxOfCanControllerIdUpToLowMap() with ControllerId.
\COUNTERMEASURE \N The DET check of the function verifes that ControllerId is smaller than the size of CanIf_CtrlStates[].
                   CanIf_Init() makes a check that the dimension of CanIf_CtrlStates[] fits the dimension of CanIf_CanControllerIdUpToLowMap[],
                   to ensure the indirection with ControllerId into CanIf_GetCanDrvFctTblIdxOfCanControllerIdUpToLowMap().
                   The following indirection to get a valid function pointer is ensured via qualified use-case CSL03 of ComStackLib.

\ID SBSW_CANIF_42
\DESCRIPTION Write access to CanIf_PduRxMode with index PduRxModeIdx.
             CanIfRxPduId -> CanIf_GetPduRxModeIdxOfRxPduConfig
\COUNTERMEASURE \N The input parameter CanIfRxPduId is checked against CanIf_GetSizeOfRxPduConfig(). The following indirection is ensured via qualified use-case CSL03 of ComStackLib.

\ID SBSW_CANIF_43
\DESCRIPTION In case of feature "CANIF_MULTIPLE_CANDRV_SUPPORT" is active: \
             Function call via function pointer. Function is indirected with parameter ControllerId via CanIf_GetCanDrvFctTblIdxOfCanControllerIdUpToLowMap to CanIf_CanDrvFctTbl.
             ControllerId is indirected from CanTxPduId via CanIf_GetCtrlStatesIdxOfTxPduConfig().
             During CanIf_Init() there is a check that dimension of CanIf_CtrlStates[] fits the dimension of CanIf_CanControllerIdUpToLowMap[].
             The parameter CanTxPduId is checked against CanIf_GetSizeOfTxPduConfig(). 
\COUNTERMEASURE \N The indirections to get the function pointer are ensured by the qualified use-cases CSL01 and CSL03 of ComStackLib.

\ID SBSW_CANIF_44
\DESCRIPTION Function call via function pointer: Access to CanIf_CanTrcvFctTbl via indirection over CanIf_TransceiverUpToLowMap with TransceiverId.
             Function call with parameter data pointer: The data pointer is passed through the function with the same type.
\COUNTERMEASURE \N Function call via function pointer: The DET check of the function verifes that the TransceiverId is smaller than the size of CanIf_TransceiverUpToLowMap[],
                                                       to ensure the indirection with TransceiverId into CanIf_TransceiverUpToLowMap[].
                                                       The following indirection to get a valid function pointer is ensured via qualified use-case CSL03 of ComStackLib.
                   Function call with parameter data pointer: The caller ensures that the passed-through pointer parameter is valid.

\ID SBSW_CANIF_45
\DESCRIPTION Function call via function pointer: Access to CanIf_CanDrvFctTbl via indirection over CanIf_CanControllerIdUpToLowMap with ControllerId.
             Function call with parameter data pointer: The data pointer is passed through the function with the same type.
\COUNTERMEASURE \N Function call via function pointer: CanIf_Init() makes the boundry check against CANIF_CFG_MAX_CONTROLLER for CanIf_CanControllerIdUpToLowMap[],
                                                       to ensure the indirection with ControllerId into CanIf_CanControllerIdUpToLowMap[].
                                                       The following indirection to get a valid function pointer is ensured via qualified use-case CSL03 of ComStackLib.
                   Function call with parameter data pointer: The caller ensures that the passed-through pointer parameter is valid.

\ID SBSW_CANIF_48
\DESCRIPTION Write access to CanIf_J1939RxAddr_Lookup/CanIf_J1939TxAddr_Lookup with index from function input parameter (incl. configured offset).
             The data type uint8 of the function input parameters ensures that the address tables can not be accessed with a value greater than 255.
             The needed configuration data (J1939 support activated/deactivated; address table offset) are always and only from the CanIf_CtrlConfig[] table.
\COUNTERMEASURE \R [CM_CANIF_2]

\ID SBSW_CANIF_49
\DESCRIPTION Write access to contextData with index CANIF_CONTEXTDATA_CTRLID_POS.
\COUNTERMEASURE \N [CM_CANIF_1]

\ID SBSW_CANIF_50
\DESCRIPTION Write access to contextData with index CANIF_CONTEXTDATA_CTRLID_POS/CANIF_CONTEXTDATA_THRESHOLD_POS.
\COUNTERMEASURE \N [CM_CANIF_1]

\ID SBSW_CANIF_51
\DESCRIPTION Write access to contextData with index CANIF_CONTEXTDATA_CTRLID_POS/CANIF_CONTEXTDATA_CANERROR_POS.
\COUNTERMEASURE \N [CM_CANIF_1]

\ID SBSW_CANIF_52
\DESCRIPTION In case of feature "CANIF_MULTIPLE_CANDRV_SUPPORT" is active:
             Function call via function pointer: Access to CanIf_CanDrvFctTbl[] via indirection over CanIf_GetCanDrvFctTblIdxOfCanControllerIdUpToLowMap() with ControllerId.
                                                 The DET check of the caller function verifes that ControllerId is smaller than the size of CanIf_CtrlStates[].
                                                 CanIf_Init() makes a check that the dimension of CanIf_CtrlStates[] fits the dimension of CanIf_CanControllerIdUpToLowMap[],
                                                 to ensure the indirection with ControllerId into CanIf_GetCanDrvFctTblIdxOfCanControllerIdUpToLowMap().
                                                 The following indirection to get a valid function pointer is ensured via qualified use-case CSL03 of ComStackLib.
            In case of features "CANIF_SUPPORT_CANDRV_EQC_AR_431" and "CANIF_SUPPORT_CANDRV_EQC_MSRC" are active:
            Signature for function call is determined at runtime by CanIf_GetCanDrvEqcArVersionOfCanControllerIdUpToLowMap() with ControllerId.
\COUNTERMEASURE \S [CM_CANIF_4]

\ID SBSW_CANIF_53
\DESCRIPTION Function call with pointer parameter 'localPduPtr': The pointer parameter reference the pointer parameter 'localPduPtr' of the caller with the same type.
                                                                 The caller ensures that the passed-through data pointer 'sdu' as member of 'localPduPtr' is valid.
             In case of feature "CANIF_MULTIPLE_CANDRV_SUPPORT" is active:
             Function call via function pointer: Access to CanIf_CanDrvFctTbl[] via indirection over CanIf_GetCanDrvFctTblIdxOfCanControllerIdUpToLowMap() with ControllerId.
                                                 ControllerId is derived from CanTxPduId via CanIf_GetCtrlStatesIdxOfTxPduConfig().
                                                 The DET check of the caller function verifes that the CanTxPduId is smaller than the size of CanIf_TxPduConfig[],
                                                 to ensure the indirection with CanTxPduId into CanIf_GetCtrlStatesIdxOfTxPduConfig().
                                                 CanIf_Init() makes a check that the dimension of CanIf_CtrlStates[] fits the dimension of CanIf_CanControllerIdUpToLowMap[],
                                                 to ensure the indirection with ControllerId into CanIf_GetCanDrvFctTblIdxOfCanControllerIdUpToLowMap().
                                                 The following indirection to get a valid function pointer is ensured via qualified use-case CSL03 of ComStackLib.
             In case of features "CANIF_SUPPORT_CANDRV_EQC_AR_431" and "CANIF_SUPPORT_CANDRV_EQC_MSRC" are active:
             Signature for function call is determined at runtime by CanIf_GetCanDrvEqcArVersionOfCanControllerIdUpToLowMap() with ControllerId.
\COUNTERMEASURE \S [CM_CANIF_4]

\ID SBSW_CANIF_54
\DESCRIPTION In case of feature "CANIF_MULTIPLE_CANDRV_SUPPORT" is active:
             Function call via function pointer: Access to CanIf_CanDrvFctTbl[] via indirection over CanIf_GetCanDrvFctTblIdxOfCanControllerIdUpToLowMap() with wuTrgtaddress (argument for parameter ControllerId).
                                                 A runtime check of the caller function ensures that wuTrgtaddress is smaller than the size of CanIf_CanControllerIdUpToLowMap[],
                                                 to ensure the indirection with wuTrgtaddress into CanIf_GetCanDrvFctTblIdxOfCanControllerIdUpToLowMap().
                                                 The following indirection to get a valid function pointer is ensured via qualified use-case CSL03 of ComStackLib.
            In case of features "CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421" and "CANIF_SUPPORT_CANDRV_EQC_MSRC" are active:
            Signature for function call is determined at runtime by CanIf_GetCanDrvEqcArVersionOfCanControllerIdUpToLowMap() with wuTrgtaddress.
\COUNTERMEASURE \S If features "CANIF_SUPPORT_CANDRV_EQC_AR_403_OR_AR_421" and "CANIF_SUPPORT_CANDRV_EQC_MSRC" are active it must be ensured that the function signature refered by the corresponding function pointer
                   in CanIf_CanDrvFctTbl[] matches the value of CanDrvEqcArVersion from CanIf_CanControllerIdUpToLowMap[]. This is ensured by SMI-1134759.
                   In all other cases the function signature is directly given.

\ID SBSW_CANIF_55
\DESCRIPTION Function call with pointer parameter 'localPduPtr': The pointer parameter reference the local variable 'localPdu'.

             Local variable 'localPdu' has the data pointer member 'sdu' and the related data length member 'length':

             In case feature "CANIF_STATIC_FD_TXQUEUE" is active:
             Data length 'localPdu.length' is set to the value of member 'eBaseParams.eSduLength' from generated variable table CanIf_TxQueue[].
             Data pointer 'localPdu.sdu' references a element (as start address) from generated variable table CanIf_TxQueueData[].

             In case feature "CANIF_STATIC_FD_TXQUEUE" is inactive:
             Data pointer 'localPdu.sdu' references the member 'eSdu' and data length 'localPdu.length' is set to the value of member 'eBaseParams.eSduLength' from generated variable table CanIf_TxQueue[].
\COUNTERMEASURE \N Function call with pointer parameter 'localPduPtr': The local variable is always a valid reference.

                   Local variable 'localPdu' has the data pointer member 'sdu' and the related data length member 'length':

                   In case feature "CANIF_STATIC_FD_TXQUEUE" is active:
                   Access to CanIf_TxQueue[].eBaseParams.eSduLength via indirection over CanIf_GetTxQueueIdxOfTxPduQueueIndex() with PduId.
                   Access to CanIf_TxQueueData[] via indirection over CanIf_GetTxQueueDataStartIdxOfTxQueueIndex2DataStartStop() from CanIf_GetTxQueueIndex2DataStartStopIdxOfTxPduQueueIndex() with PduId.
                   The DET check of the function verifes that the PduId is smaller than the size of CanIf_TxPduConfig[].
                   The CSL ensures that the dimension of CanIf_TxPduConfig[] fits the dimension of CanIf_TxPduQueueIndex[], to ensure the indirection with PduId into CanIf_GetTxQueueIdxOfTxPduQueueIndex().
                   The following indirection to get'eBaseParams.eSduLength' from CanIf_TxQueue[] is ensured via qualified use-case CSL03 of ComStackLib.
                   The following indirections to get start address from CanIf_TxQueueData[] is ensured via qualified use-case CSL03 of ComStackLib.

                   That a area within CanIf_TxQueueData[] and 'eBaseParams.eSduLength' define a valid memory area for a PduId is ensured during CanIf_TransmitSubHandleTxResBusy() or CanIf_CancelTxConfirmation().
                   A DET check ensures that the data length of the to be buffered PDU fits to the size for data of the PduId in the CanIf_TxQueueData[].
                   The access to CanIf_TxQueue[].eBaseParams.eSduLength and CanIf_TxQueueData[] over PduId is done and is secured in the same way as described above.

                   In case feature "CANIF_STATIC_FD_TXQUEUE" is inactive:
                   Access to CanIf_TxQueue[].eSdu and CanIf_TxQueue[].eBaseParams.eSduLength via indirection over CanIf_GetTxQueueIdxOfTxPduQueueIndex() with PduId.
                   The DET check of the function verifes that the PduId is smaller than the size of CanIf_TxPduConfig[].
                   The CSL ensures that the dimension of CanIf_TxPduConfig[] fits the dimension of CanIf_TxPduQueueIndex[], to ensure the indirection with PduId into CanIf_GetTxQueueIdxOfTxPduQueueIndex().
                   The following indirection to get 'eSdu' and 'eBaseParams.eSduLength' from CanIf_TxQueue[] is ensured via qualified use-case CSL03 of ComStackLib.

                   That 'eSdu' and 'eBaseParams.eSduLength' define a valid memory area for a PduId is ensured during CanIf_TransmitSubHandleTxResBusy() or CanIf_CancelTxConfirmation().
                   A DET check ensures that the data length of the to be buffered PDU fits to the size of the 'eSdu' member for the PduId in the CanIf_TxQueue[].
                   The access to CanIf_TxQueue[].eSdu and CanIf_TxQueue[].eBaseParams.eSduLength over PduId is done and is secured in the same way as described above.

\ID SBSW_CANIF_56
\DESCRIPTION Function call with pointer parameter 'localPduPtr': The pointer parameter reference the local variable 'localPdu'.

             Local variable 'localPdu' has the data pointer member 'sdu' and the related data length member 'length':
             Data length 'localPdu.length' is set to the value of member 'eBaseParams.eSduLength' from generated variable table CanIf_TxFifoQueueBase[].
             Data pointer 'localPdu.sdu' references a element (as start address) from generated variable table CanIf_TxFifoQueueData[].

\COUNTERMEASURE \N Function call with pointer parameter 'localPduPtr': The local variable is always a valid reference.

                   Local variable 'localPdu' has the data pointer member 'sdu' and the related data length member 'length':
                   Access to CanIf_TxFifoQueueBase[].eBaseParams.eSduLength with 'txFifoQueueBaseStartIdx' and current 'readIdx'.
                   'readIdx': Access to CanIf_TxBufferFifoBase[].eReadIdx via indrection over CanIf_GetTxBufferFifoBaseIdxOfTxBufferFifoConfig() and CanIf_GetTxBufferCfgIdxOfMailBoxConfig() from CanIf_GetMailBoxConfigIdxOfTxPduConfig() with PduId.
                   'txFifoQueueBaseStartIdx': Access to CanIf_TxBufferFifoConfig[].TxFifoQueueBaseStartIdx via indrection over CanIf_GetTxBufferCfgIdxOfMailBoxConfig() from CanIf_GetMailBoxConfigIdxOfTxPduConfig() with PduId.

                   Access to CanIf_TxFifoQueueData[] with current 'readIdx', 'sizeOfOnePlayloadEl' and 'txFifoQueueDataStartIdx'.
                   'readIdx': see above.
                   'sizeOfOnePlayloadEl': CanIf_TxBufferFifoConfig[].SizeOfOnePayloadEl via indrection over CanIf_GetTxBufferCfgIdxOfMailBoxConfig() from CanIf_GetMailBoxConfigIdxOfTxPduConfig() with PduId.
                   'txFifoQueueDataStartIdx': CanIf_TxBufferFifoConfig[].TxFifoQueueDataStartIdx via indrection over CanIf_GetTxBufferCfgIdxOfMailBoxConfig() from CanIf_GetMailBoxConfigIdxOfTxPduConfig() with PduId.

                   The DET check of the function verifes that the PduId is smaller than the size of CanIf_TxPduConfig[].
                   The following indirections are ensured via qualified use-case CSL03 of ComStackLib.

                   That a area within CanIf_TxFifoQueueData[] and 'eBaseParams.eSduLength' define a valid memory area for a PduId is ensured during CanIf_TransmitSubHandleTxResBusy().
                   A DET check ensures that the data length of the to be buffered PDU fits in the space from the current 'writeIdx' and 'txFifoQueueDataStartIdx' in the CanIf_TxFifoQueueData[].
                   The access to CanIf_TxFifoQueueBase[].eBaseParams.eSduLength and CanIf_TxFifoQueueData[] (with 'writeIdx') over PduId is done and is secured in the same way as described above.
                   The current 'writeIdx' is similar to the 'readIdx' a member of CanIf_TxBufferFifoBase[]. Both are checked during runtime against CanIf_GetTxFifoQueueBaseLengthOfTxBufferFifoConfig().
                   This ensures that 'writeIdx' and 'readIdx' are in the same range and therefore the above mentioned runtime check for CanIf_TxFifoQueueData[] secures also the access within CanIf_TxQueueTransmit().

\ID SBSW_CANIF_57
\DESCRIPTION Function call via function pointer: Direct call of function referenced by 'CanIf_SetDynamicRxIdFctPtr' from the generated data.
             Function call with pointer parameter 'canId': The pointer parameter reference the local variable 'canId' and is therefore always valid.
\COUNTERMEASURE \S It must be ensured that the function pointer 'CanIf_SetDynamicRxIdFctPtr' reference a valid function with the valid signature. This is ensured by SMI-1191144.

\ID SBSW_CANIF_58
\DESCRIPTION Write access to CanIf_DynRxId[] with index DynRxIdIdx.

             In case of pre compile configuration:
             rxPduId -> CanIf_GetDynRxIdIdxOfRxPduConfig() (if CanIf_IsDynRxIdUsedOfRxPduConfig() is true)

             In case of postbuild or postbuild variant configuration:
             extRxPduId -> CanIf_GetRxPduConfigIdxOfUlRxPduId2InternalRxPduId() (if CanIf_IsRxPduConfigUsedOfUlRxPduId2InternalRxPduId() is true) -> CanIf_GetDynRxIdIdxOfRxPduConfig() (if CanIf_IsDynRxIdUsedOfRxPduConfig() is true)
\COUNTERMEASURE \N In case of pre compile configuration:
                rxPduId is checked against CanIf_GetSizeOfRxPduConfig(). The following indirection is ensured via qualified use-case CSL03 of ComStackLib.

                In case of postbuild or postbuild variant configuration:
                extRxPduId is checked against CanIf_GetSizeOfUlRxPduId2InternalRxPduId(). The following indirections are ensured via qualified use-case CSL03 of ComStackLib.

SBSW_JUSTIFICATION_END */

/*
\CM CM_CANIF_1 A error directive verifies that the context data size is bigger than the write position.

\CM CM_CANIF_2 In CanIf_Init() is checked against CanIf_GetSizeOfJ1939RxAddr_Lookup/CanIf_GetSizeOfJ1939TxAddr_Lookup that each controller with activated J1939 support has a space of 256 elements in these tables.
               In CanIf_Init() is checked against CanIf_GetSizeOfJ1939RxAddr_Lookup/CanIf_GetSizeOfJ1939TxAddr_Lookup that the biggest configured table offset plus 256 elements fits in these tables.
               If the address table configuration check in Canif_Init() fails, all CAN controllers are in state uninitialized.
               The DET check of the function verfies that the specified CAN controller is initialized (address table configuration is consistent).

\CM CM_CANIF_4 If features "CANIF_SUPPORT_CANDRV_EQC_AR_431" and "CANIF_SUPPORT_CANDRV_EQC_MSRC" are active it must be ensured that the function signature refered by the corresponding function pointer
               in CanIf_CanDrvFctTbl[] matches the value of CanDrvEqcArVersion from CanIf_CanControllerIdUpToLowMap[]. This is ensured by SMI-1115873.
               In all other cases the function signature is directly given.
*/

/**********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 *********************************************************************************************************************/
 
/* START_COVERAGE_JUSTIFICATION

\ID COV_CANIF_GENDATA_FAILURE
  \ACCEPT XF
  \ACCEPT TX
  \ACCEPT X
  \ACCEPT XF xf xf
  \ACCEPT Tx tx tx
  \ACCEPT TF tf tf tf xf
  \ACCEPT TF xf xf tf xf
  \ACCEPT XX xx xx xx xx
  \REASON The condition checks for generated data failures, that cannot be simulated. The code is verified by code inspection.

\ID COV_CANIF_NOT_APPLICABLE_FOR_EACH_CONFIG
  \ACCEPT X
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TX tx xx
  \ACCEPT XX xx xx
  \ACCEPT XF xf xx xx
  \ACCEPT TF tx tx tf
  \ACCEPT TF tx tf tf
  \ACCEPT TF tf tx tf
  \ACCEPT XX xx xx xx xx xx
  \REASON It is not possible to vary the several parameter within one configuration. It will be secured by inspection that both decisions are covered,
          for minimum one time, over all configurations.

\ID COV_CANIF_FEATURE_DISCONTINUED
  \ACCEPT XF
  \REASON Feature is not still supported, with this define check it is ensured that is not enabled by accident, for example with old generator.

\ID COV_CANIF_FILTER_RULE
  \ACCEPT XF
  \ACCEPT TX
  \REASON Define is not a preprocessor instruction, its used for filtering the component and must not still exist during compile time. Usage in case of SafeBSW-project is prohibited by an error-directive.

\ID COV_CANIF_COMPATIBILITY
  \ACCEPT TX
  \REASON The condition checks whether a switch is defined and conditionally assigns a default value. This is used to ensure compatibility to future generator changes.
          The correctness is ensured by code review.

\ID COV_CANIF_MIXED_ID
  \ACCEPT TX
  \REASON The feature is platform specific.

\ID COV_CANIF_REDEFINITION_PROTECTION
  \ACCEPT TX
  \ACCEPT XF
  \REASON For special requirements this defines may be defined from outside.

\ID COV_CANIF_CANDRV_EQC
  \ACCEPT XF
  \ACCEPT XX
  \REASON The functionality can only partially be covered with the single CAN Driver testsuite.
          Inspection ensures that the functionality is additionally covered by the multiple CAN Driver test suite.
          The correctness is also ensured by code review.

\ID COV_CANIF_SUPPORT_CAN_XL
  \ACCEPT XF
  \REASON Beta feature. Deactivation guaranteed by MSSV plugin.

END_COVERAGE_JUSTIFICATION */

/**********************************************************************************************************************
 *  FETA JUSTIFICATIONS
 *********************************************************************************************************************/

/* FETA_JUSTIFICATION_BEGIN

\ID FETA_CANIF_1
\DESCRIPTION The for loop iterates over all configured Tx-Buffer at a CAN controller.
             The constant start and stop index are determined by parameterized GenData access.
             ControllerId -> CanIf_GetMappedTxBuffersConfigStartIdxOfCanIfCtrlId2MappedTxBuffersConfig
             ControllerId -> CanIf_GetMappedTxBuffersConfigEndIdxOfCanIfCtrlId2MappedTxBuffersConfig
\COUNTERMEASURE \N ControllerId is checked against CanIf_GetSizeOfCtrlStates in the caller function.
                   The following indirection is ensured via qualified use-case CSL03 of ComStackLib.
                   Therefore the number of iteration is limited and statically given by configuration.

\ID FETA_CANIF_2
\DESCRIPTION The while loops always decrements the counter value to zero which ends the loop.
\COUNTERMEASURE \N The counter value is a local variable.
                   The counter value is checked against greater zero.
                   This ensures a termination of the loop independent of the initial counter value.

\ID FETA_CANIF_3
\DESCRIPTION The while loop has complex termination conditions.
             The termination conditions are determined by parameterized GenData access.
\COUNTERMEASURE \D CanIf_RxIndicationSubLinearSearch
                \S SMI-246328

\ID FETA_CANIF_4
\DESCRIPTION The while loop has external termination condition computation (calls to CanIf_RxIndicationSubCheckPduProp).
             The termination conditions are determined by parameterized GenData access.
\COUNTERMEASURE \D DSGN-CanIf22792
                \S SMI-246328

\ID FETA_CANIF_5
\DESCRIPTION The while loop iterates over the currently queued Tx-PDUs in the Tx-Buffer to find the next not canceled Tx-PDU.
             The number of iteration depends on the currently number of the queued Tx-PDUs in the Tx-Buffer and corresponds
             at most to the size of the Tx-Buffer.
             The variable termination conditions (eQueued and eQueueCounter) are determined by parameterized GenData access.
             CanIf_TxFifoQueueBase(startIndex + eReadIdx).eQueued:
             CanTxPduId/PduId -> CanIf_GetMailBoxConfigIdxOfTxPduConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig
             -> CanIf_GetTxBufferFifoBaseIdxOfTxBufferFifoConfig -> CanIf_TxBufferFifoBase.eReadIdx
             -> CanIf_GetTxFifoQueueBaseStartIdxOfTxBufferFifoConfig (-> startIndex)
             eQueueCounter:
             CanTxPduId/PduId -> CanIf_GetMailBoxConfigIdxOfTxPduConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig
             -> CanIf_GetTxBufferFifoBaseIdxOfTxBufferFifoConfig -> CanIf_TxBufferFifoBase.eQueueCounter
\COUNTERMEASURE \N CanTxPduId/PduId is checked against CanIf_GetSizeOfTxPduConfig by the caller function.
                   The following indirections are ensured via qualified use-case CSL03 of ComStackLib.
                   The eReadIdx is checked against CanIf_GetTxFifoQueueBaseLengthOfTxBufferFifoConfig each time while writing CanIf_TxBufferFifoBase.
                   For (CanIf_GetTxFifoQueueBaseLengthOfTxBufferFifoConfig (eReadIdx) + CanIf_GetTxFifoQueueBaseStartIdxOfTxBufferFifoConfig (startIndex)) a
                   qualified CSL03 use-case of ComStackLib ensures that the index is valid to access CanIf_TxFifoQueueBase.eQueued.
                   The loop is locked by an exclusive area to prevent push, pop and cancel Tx-PDUs while processing the Tx-Buffer.
                   This ensures a termination of the loop.

\ID FETA_CANIF_6
\DESCRIPTION The for loop iterates over the Tx-Buffer to clear the queue flags.
             The constant start and stop index are determined by parameterized GenData access.
             ControllerId -> CanIf_GetMappedTxBuffersConfigStartIdxOfCanIfCtrlId2MappedTxBuffersConfig/CanIf_GetMappedTxBuffersConfigEndIdxOfCanIfCtrlId2MappedTxBuffersConfig
             -> CanIf_GetMailBoxConfigIdxOfMappedTxBuffersConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig
             -> CanIf_GetTxQueueFlagsStartIdxOfTxBufferPrioByCanIdConfig/CanIf_GetTxQueueFlagsEndIdxOfTxBufferPrioByCanIdConfig
\COUNTERMEASURE \N ControllerId is checked against CanIf_GetSizeOfCtrlStates by the caller function.
                   The following indirections are ensured via qualified use-case CSL03 of ComStackLib.
                   Therefore the number of iteration is limited and statically given by configuration.

\ID FETA_CANIF_7
\DESCRIPTION The while loop termination is determined by values from the following calculations
             startPos = (uint8)((PduInfoPtr->SduLength) - 1u)
             endPos = (uint8)(PduInfoPtr->SduLength)
             endPos = (uint8)(endPos - CANIF_CFG_TX_TXMETADATALENGTH(localPduPtr->swPduHandle))
             The PduInfoPtr->SduLength is checked against the configured length to ensure a valid value (CanIf_TransmitSubVerifyPduLength).
             The meta data length is determined by parameterized GenData access.
\COUNTERMEASURE \N The CanTxPduId is checked against CanIf_GetSizeOfTxPduConfig by the caller function to ensure a valid access to the
                   configured length and meta data length (qualified use-case CSL01 of ComStackLib).
                   The variable PduInfoPtr->SduLength (value 0 - 68) is checked (>=) against CANIF_CFG_TX_TXMETADATALENGTH(localPduPtr->swPduHandle) (value 1 -4).
                   Therefore the value for PduInfoPtr->SduLength is ensured to be between 1 and 68. The value for startPos is between 0 and 67 after the calaculation.
                   The variable endPos is first set to PduInfoPtr->SduLength (value 1 - 68, regarding to check above).
                   After the calculation the value for endPos is between 0 and 64.
                   Therefore the while loop terminates always after between 0 and 3 iterations.

\ID FETA_CANIF_8
\DESCRIPTION The for loop termination is determined by an parameterized GenData access.
\COUNTERMEASURE \S SMI-246328

\ID FETA_CANIF_9
\DESCRIPTION The for loop iterates over the Tx-Buffer to find the next queued Tx-PDU with the highest priority.
             The number of iteration depends on the number and the priority of the currently queued Tx PDUs in the Tx-Buffer and
             corresponds at most to the size of the Tx-Buffer.
             The for loop is only executed, if at least one Tx-PDU is buffered (eQueueCounter != 0).
             The variable eQueueCounter value is determined by parameterized GenData access.
             CanTxPduId/PduInfoPtr->swPduHandle/PduId -> CanIf_GetMailBoxConfigIdxOfTxPduConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig
             -> CanIf_GetTxBufferPrioByCanIdBaseIdxOfTxBufferPrioByCanIdConfig -> CanIf_GetTxBufferPrioByCanIdBase).eQueueCounter
             The constant start and stop index (to the Tx-Buffer) are determined by parameterized GenData access.
             CanTxPduId/PduInfoPtr->swPduHandle/PduId -> CanIf_GetMailBoxConfigIdxOfTxPduConfig -> CanIf_GetTxBufferCfgIdxOfMailBoxConfig
             -> CanIf_GetTxQueueFlagsStartIdxOfTxBufferPrioByCanIdConfig/CanIf_GetTxQueueFlagsEndIdxOfTxBufferPrioByCanIdConfig
\COUNTERMEASURE \N CanTxPduId/PduInfoPtr->swPduHandle/PduId is checked against CanIf_GetSizeOfTxPduConfig by the caller function.
                   The following indirections are ensured via qualified use-case CSL03 of ComStackLib.
                   Each time a new Tx-PDU is buffered the eQueueCounter is incremented and the corresponding queue flag is set.
                   The loop is also locked by an exclusive area to prevent push, pop and cancel Tx PDUs while processing the Tx-Buffer.
                   This ensures a termination of the loop between the start and stop index.

\ID FETA_CANIF_10
\DESCRIPTION The for loop termination is determined by a value from the calculation (dlc_checksum - 1u).
\COUNTERMEASURE \N The variable dlc_checksum is checked against zero and CANIF_CFG_MAXTXDLC_PLUS_DATACHECKSUM (maximum value 64).
                   Therefore the for loop terminates always after between 1 and 63 iterations.

\ID FETA_CANIF_11
\DESCRIPTION The for loop termination is determined by a value from the calculation (CanDlc - 1u).
\COUNTERMEASURE \N The variable CanDlc is checked against zero and in the caller function against 8/64.
                   Therefore the for loop terminates always after between 1 and 63 iterations.

\ID FETA_CANIF_13
\DESCRIPTION The for loop termination is determined by a value returned by an GenData access via pointer in a postbuild or selectable configuration.
             No CSL generated getSizeOf macro is used.
\COUNTERMEASURE \S SMI-246328

FETA_JUSTIFICATION_END */

/**********************************************************************************************************************
 *  END OF FILE: CanIf.c
 *********************************************************************************************************************/
