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
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  LinIf_Types.h
 *    Component:  AUTOSAR LIN INTERFACE
 *       Module:  If_Asr4IfLin
 *    Generator:  If_Asr4IfLin
 *
 *  Description:  Declaration of types and defines of the AUTOSAR LIN Interface
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

#if !defined (LINIF_TYPES_H)
# define LINIF_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/* include ComStack_Types.h so all needed types are known */
# include "ComStack_Types.h"
# include "EcuM.h"

/* Include types of Lin Driver for pointer to init struct */
# include "LinIf_Lin.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define LINIF_TPS_MAJOR_VERSION                    (12u)
# define LINIF_TPS_MINOR_VERSION                    (0u)
# define LINIF_TPS_PATCH_VERSION                    (0u)

# define LINIF_LINPDU_SIZE                          (8u) /* currently must be a power of 2 because of mask definition below */
# define LINIF_LINPDU_SIZE_MASK                     (LINIF_LINPDU_SIZE - 1u)

# define LINIF_NC_BUF_SIZE                          (8u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
# if ( LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON )
/* defines for LIN Driver function pointers */

/*! Function pointer type for Lin_SendFrame API */
typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_SendFrameType) (
  VAR(uint8, AUTOMATIC) Channel,
  P2VAR(Lin_PduType, AUTOMATIC, LINIF_APPL_VAR) PduInfoPtr
);

/*! Function pointer type for Lin_GetStatus API */
typedef P2FUNC(Lin_StatusType, LINIF_PBCFG, LinIf_Lin_GetStatusType) (
  VAR(uint8, AUTOMATIC) Channel,
  P2VAR(Lin_u8PtrType, AUTOMATIC, LINIF_APPL_VAR) Lin_SduPtr
);

/*! Function pointer type for Lin_CheckWakeup API */
typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_CheckWakeupType) (
  VAR(uint8, AUTOMATIC) Channel
);

/*! Function pointer type for Lin_GoToSleep API */
typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_GoToSleepType) (
  VAR(uint8, AUTOMATIC) Channel
);

/*! Function pointer type for Lin_GoToSleepInternal API */
typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_GoToSleepInternalType) (
  VAR(uint8, AUTOMATIC) Channel
);

/*! Function pointer type for Lin_Wakeup API */
typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_WakeupType) (
  VAR(uint8, AUTOMATIC) Channel
);

/*! Function pointer type for Lin_WakeupInternal API */
typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_Lin_WakeupInternalType) (
  VAR(uint8, AUTOMATIC) Channel
);

# endif

# if ( LINIF_TRCV_HANDLING == STD_ON )
/* defines for LIN Transceiver Driver function pointers */

/*! Function pointer type for LinTrcv_SetOpMode API */
typedef P2FUNC(Std_ReturnType, LINIF_CBK_TRCV_CODE, LinIf_LinTrcv_SetOpModeFctType) (
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  VAR(LinTrcv_TrcvModeType, AUTOMATIC) OpMode
);

/*! Function pointer type for LinTrcv_GetOpMode API */
typedef P2FUNC(Std_ReturnType, LINIF_CBK_TRCV_CODE, LinIf_LinTrcv_GetOpModeFctType) (
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  P2VAR(LinTrcv_TrcvModeType, AUTOMATIC, LINIF_APPL_VAR) OpMode
);

/*! Function pointer type for LinTrcv_GetBusWuReason API */
typedef P2FUNC(Std_ReturnType, LINIF_CBK_TRCV_CODE, LinIf_LinTrcv_GetBusWuReasonFctType) (
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  P2VAR(LinTrcv_TrcvWakeupReasonType, AUTOMATIC, LINIF_APPL_VAR) Reason
);

/*! Function pointer type for LinTrcv_SetWakeupMode API */
typedef P2FUNC(Std_ReturnType, LINIF_CBK_TRCV_CODE, LinIf_LinTrcv_SetWakeupModeFctType) (
  VAR(uint8, AUTOMATIC) LinTrcvIndex,
  VAR(LinTrcv_TrcvWakeupModeType, AUTOMATIC) TrcvWakeupMode
);

/*! Function pointer type for LinTrcv_CheckWakeup API */
typedef P2FUNC(Std_ReturnType, LINIF_CBK_TRCV_CODE, LinIf_LinTrcv_CheckWakeupFctType) (
  VAR(uint8, AUTOMATIC) LinTrcvIndex
);

# endif

typedef uint16                                LinIf_ScheduleTableTickType; /*!< Schedule table slot time base tick type */
typedef uint8                                 LinIf_SchHandleType; /*!< Schedule table handle type */
# define NULL_SCHEDULE                        ((LinIf_SchHandleType)0x00u) /*!< NULL schedule table constant identifier */

# if ( LINIF_NODETYPE_SLAVE_SUPPORTED == STD_ON )
#  if ( LINIF_NC_PRODUCT_ID_CONFIGURABLE_SUPPORTED == STD_ON )
typedef uint8                                 LinIf_ProductIdentType; /*!< LIN product indentification enum type */
/*!
 * \defgroup LinProductInformation LinIf LIN product identification enum keys
 * \{
 */
#   define LINIF_LINPRODIDENT_SUPPLIER_ID     ((LinIf_ProductIdentType)0x01u) /*!< Supplier ID type */
#   define LINIF_LINPRODIDENT_FUNCTION_ID     ((LinIf_ProductIdentType)0x02u) /*!< Function ID type */
#   define LINIF_LINPRODIDENT_VARIANT_ID      ((LinIf_ProductIdentType)0x03u) /*!< Variant ID type */
/*! \} */
#  endif

#  if ( LINIF_NC_READ_BY_IDENTIFIER_1_SUPPORTED == STD_ON )
typedef uint8                                 LinIf_ReadByIdSerialNumberReturnType; /*!< ReadByIdentifier ID 1 (Serial number) return value type */
/*!
 * \defgroup ReadByIdentifier1Callout ReadByIdentifier ID 1 (Serial number) application callout return values
 * \{
 */
/*! ReadByIdentifier ID 1 (Serial number) request accepted, send positive response */
#   define LinIf_ReadByIdSerialNumber_PosResp ((LinIf_ReadByIdSerialNumberReturnType)0x00u)
/*! ReadByIdentifier ID 1 (Serial number) not request accepted, send negative response */
#   define LinIf_ReadByIdSerialNumber_NegResp ((LinIf_ReadByIdSerialNumberReturnType)0x01u)
/*! \} */
#  endif

#  if ( LINIF_NC_READ_BY_IDENTIFIER_32_63_SUPPORTED == STD_ON )
typedef uint8                                 LinIf_ReadByIdUserDefinedReturnType; /*!< ReadByIdentifier ID [32, 63] (User defined) return value type */
/*!
 * \defgroup ReadByIdentifier3263Callout ReadByIdentifier [32, 63] (User defined) application callout return values
 * \{
 */
/*! ReadByIdentifier [32, 63] (User defined) request accepted, send positive response */
#   define LinIf_ReadByIdUserDefined_PosResp  ((LinIf_ReadByIdUserDefinedReturnType)0x00u)
/*! ReadByIdentifier [32, 63] (User defined) request not accepted, send negative response */
#   define LinIf_ReadByIdUserDefined_NegResp  ((LinIf_ReadByIdUserDefinedReturnType)0x01u)
/*! ReadByIdentifier [32, 63] (User defined) request not accepted, send no response (non-conform to LIN) */
#   define LinIf_ReadByIdUserDefined_NoResp   ((LinIf_ReadByIdUserDefinedReturnType)0x02u)
/*! \} */
#  endif

#  if ( LINIF_NC_DATA_DUMP_SUPPORTED == STD_ON )
/*!
 * \defgroup DataDumpCallout DataDump service application callout return values
 * \{
 */
typedef uint8                                 LinIf_DataDumpReturnType; /*!< DataDump return value type */
/*! DataDump request accepted, send positive response */
#   define LinIf_DataDump_PosResp             ((LinIf_DataDumpReturnType)0x00u)
/*! DataDump request not accepted, send no response */
#   define LinIf_DataDump_NoResp              ((LinIf_DataDumpReturnType)0x01u)
/*! \} */
#  endif

#  if ( LINIF_NC_AUTO_ADDRESSING_SUPPORTED == STD_ON )
/*!
 * \defgroup AutoAddressingCallout AutoAddressing service application callout return values
 * \{
 */
typedef uint8                                 LinIf_AutoAddressingReturnType; /*!< AutoAddressing return value type */
/*! AutoAddressing request accepted, send positive response */
#   define LinIf_AutoAddressing_PosResp       ((LinIf_AutoAddressingReturnType)0x00u)
/*! AutoAddressing request not accepted, send no response */
#   define LinIf_AutoAddressing_NoResp        ((LinIf_AutoAddressingReturnType)0x01u)
/*! \} */
#  endif

#  if ( LINIF_SLAVE_J2602_SUPPORTED == STD_ON )
typedef uint8                                 LinIf_J2602TargetedResetReturnType; /*!< J2602 TargetedReset return value type */
/*!
 * \defgroup J2602TargetedResetCallout J2602 Targeted Reset application callout return values
 * \{
 */
/*! J2602 Targeted Reset request accepted, send positive response */
#   define LinIf_J2602TargetedReset_PosResp   ((LinIf_J2602TargetedResetReturnType)0x00u)
/*! J2602 Targeted Reset request not accepted, send negative response */
#   define LinIf_J2602TargetedReset_NegResp   ((LinIf_J2602TargetedResetReturnType)0x01u)
/*! J2602 Targeted Reset request not accepted, send no response (non-conform to J2602) */
#   define LinIf_J2602TargetedReset_NoResp    ((LinIf_J2602TargetedResetReturnType)0x02u)
/*! \} */
#  endif

# endif

# if ( LINIF_TP_SUPPORTED == STD_ON )
typedef uint8                                 LinTp_Mode; /*!< LinTp communication mode type */
/*!
 * \defgroup LinTpMode LinTp communication mode types
 * \{
 */
#  define LINTP_APPLICATIVE_SCHEDULE          ((LinTp_Mode)0x00u) /*!< Applicative schedule request */
#  define LINTP_DIAG_REQUEST                  ((LinTp_Mode)0x01u) /*!< Diagnostic request table request with MRF slot */
#  define LINTP_DIAG_RESPONSE                 ((LinTp_Mode)0x02u) /*!< Diagnostic response table request with SRF slot */
/*! \} */
# endif

# if ( LINIF_SCHEDULE_INFO_API == STD_ON )
/*! Schedule table info structure */
typedef struct LinIf_ScheduleInfoTypeTag  /*! Info structure of LinIf schedule table handler */
{
  uint8                                       CurrentTable;             /*!< Handle of schedule table, zero-based */
  uint8                                       CurrentSlot;              /*!< Current slot in schedule table, Range: 0..<NumOfFramesInSchedule-1> */
  uint8                                       RequestedTable;           /*!< Handle of requested schedule table */
  LinIf_ScheduleTableTickType                 CurrentSlotLength;        /*!< Length of current schedule slot in ticks */
  LinIf_ScheduleTableTickType                 CurrentSlotPosition;      /*!< Position inside schedule slot in ticks, Range: 0..<CurrentSlotLength-1> */
} LinIf_ScheduleInfoType;
# endif

/**********************************************************************************************************************
 *  DATA TYPES AND STRUCTURES FOR CONFIGURABLE INTERFACES
 *********************************************************************************************************************/

/*! Function pointer type to 'User'_ScheduleRequestConfirmation */
typedef P2FUNC(void, LINIF_PBCFG, LinIf_SchedReqConfFctType) (
   VAR(NetworkHandleType, AUTOMATIC)              network,
   VAR(LinIf_SchHandleType, AUTOMATIC)            schedule
);

/*! ROM pointer type to schedule request notification function pointer list */
typedef P2CONST(LinIf_SchedReqConfFctType, TYPEDEF, LINIF_PBCFG)  LinIf_SchedReqConfFctListType;


/*! Function pointer type to 'User'_GotoSleepConfirmation */
typedef P2FUNC(void, LINIF_PBCFG, LinIf_GotoSleepConfFctType) (
  VAR(NetworkHandleType, AUTOMATIC)               network,
  VAR(boolean, AUTOMATIC)                         success
);

/*! ROM pointer type to gotosleep confirmation function pointer list */
typedef P2CONST(LinIf_GotoSleepConfFctType, TYPEDEF, LINIF_PBCFG)     LinIf_GotoSleepConfFctListType;

/*! Function pointer type to 'User'_GotoSleepIndication */
typedef P2FUNC(void, LINIF_PBCFG, LinIf_GotoSleepIndFctType) (
  VAR(NetworkHandleType, AUTOMATIC)               network
);

/*! ROM pointer type to gotosleep indication function pointer list */
typedef P2CONST(LinIf_GotoSleepIndFctType, TYPEDEF, LINIF_PBCFG)      LinIf_GotoSleepIndFctListType;

/*! Function pointer type to 'User'_WakeupConfirmation */
typedef P2FUNC(void, LINIF_PBCFG, LinIf_WakeupConfFctType) (
  VAR(NetworkHandleType, AUTOMATIC)               network,
  VAR(boolean, AUTOMATIC)                         success
);

/*! ROM pointer type to wakeup confirmation function pointer list */
typedef P2CONST(LinIf_WakeupConfFctType, TYPEDEF, LINIF_PBCFG)        LinIf_WakeupConfFctListType;


/*! Function pointer type to 'User'_TriggerTransmit */
typedef P2FUNC(Std_ReturnType, LINIF_PBCFG, LinIf_TriggerTransmitFctType) (
  VAR(PduIdType, AUTOMATIC)                       TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, LINIF_APPL_VAR)   PduInfoPtr
);

/*! ROM pointer type to trigger transmit function pointer list */
typedef P2CONST(LinIf_TriggerTransmitFctType, TYPEDEF, LINIF_PBCFG)   LinIf_TriggerTransmitFctListType;


/*! Function pointer type to 'User'_TxConfirmation */
typedef P2FUNC(void, LINIF_PBCFG, LinIf_TxConfFctType) (
  VAR(PduIdType, AUTOMATIC)                       TxPduId
);

/*! ROM pointer type to tx confirmation function pointer list */
typedef P2CONST(LinIf_TxConfFctType, TYPEDEF, LINIF_PBCFG)            LinIf_TxConfFctListType;


/*! Function pointer type to 'User'_RxIndication */
typedef P2FUNC(void, LINIF_PBCFG, LinIf_RxIndicationFctType) (
  VAR(PduIdType, AUTOMATIC)                       RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_VAR) PduInfoPtr
);

/*! ROM pointer type to rx indication function pointer list */
typedef P2CONST(LinIf_RxIndicationFctType, TYPEDEF, LINIF_PBCFG)      LinIf_RxIndicationFctListType;

/*! Function pointer type to response error signal changed callout function */
typedef P2FUNC(void, LINIF_PBCFG, ResponseErrorSignalChangedCalloutType) (
    VAR(NetworkHandleType, AUTOMATIC)             Channel,
    VAR(boolean, AUTOMATIC)                       RespErrSigValue
);

/*! Function pointer type to save configuration callout function */
typedef P2FUNC(boolean, LINIF_PBCFG, SaveConfigurationCalloutType) (
    VAR(NetworkHandleType, AUTOMATIC)             Channel
);



/**********************************************************************************************************************
 *  RAM STRUCTURES
 *********************************************************************************************************************/

typedef uint8                             LinIf_ChannelStateType; /*!< Internal LinIf channel state machine type */

/*! Schedule table RAM structure */
typedef struct LinIf_SchedTabDataTypeTag /*! Control structure of LinIf schedule table handler */
{
  LinIf_ScheduleTableTickType             LinIf_CurrentSlotDelay;                 /*!< Remaining slot time in ticks */
  uint8                                   LinIf_CurrentSlot;                      /*!< Current slot in schedule table, Range: 0..<NumOfFramesInSchedule-1> */
  uint8                                   LinIf_CurrentTable;                     /*!< Handle of schedule table, zero-based per channel */
  uint8                                   LinIf_PreviousContinuousSlot;           /*!< Backup of schedule slot of RUN-CONTINUOUS table if RUN-ONCE active */
  uint8                                   LinIf_PreviousContinuousTable;          /*!< Backup of schedule gandle of RUN-CONTINUOUS table if RUN-ONCE active */
  LinIf_SchHandleType                     LinIf_RequestedTable;                   /*!< Handle of current requested schedule table until it's activated */
  uint8                                   LinIf_SchedReqConfirmTable;             /*!< Handle of schedule table to confirm to upper layer */
# if ( LINIF_EVT_FRM_SUPPORTED == STD_ON )
  uint8                                   LinIf_EvtCollResolvingState;            /*!< Event-triggered frame resolving state machine */
# endif
# if ( LINIF_SCHEDULE_END_NOTIFICATION == STD_ON )
  uint8                                   LinIf_SchedEndNotifyTable;              /*!< Handle of schedule table to pass to schedule end notification */
# endif
} LinIf_SchedTabDataType;

/*! General channel RAM structure */
typedef struct LinIf_ChannelDataTypeTag /*! Control structure of LinIf for one channel, common for master and slave */
{
  LinIf_ChannelStateType                  LinIf_ChannelState;                     /*!< Channel state engine */
  uint16                                  LinIf_FrameHandle;                      /*!< Handle of current processed/active frame */
  uint8                                   LinIf_CurrentFrameType;                 /*!< Type of current processed/active frame */
# if ( LINIF_LIN_CHANNEL_WAKEUP_SUPPORT == STD_ON ) || ( LINIF_LINTRCV_WAKEUP_SUPPORT == STD_ON )
  uint8                                   LinIf_WakeupFlag;                       /*!< External wakeup detection flag. Range: 0 or 1 */
# endif
  uint8                                   LinIf_WakeupDelayTimer;                 /*!< Current countdown in ticks until start of scheduling after wakeup */
# if ( LINIF_BUSMIRRORING == STD_ON )
  boolean                                 LinIf_MirroringActivationState;         /*!< Bus mirroring activation state flag for channel */
# endif
} LinIf_ChannelDataType;

/*! Master channel RAM structure */
typedef struct LinIf_MasterChannelDataTypeTag /*! Control structure of LinIf for one master channel */
{
  LinIf_SchedTabDataType                  LinIf_SchedTabData;                     /*!< Pointer to schedule table handling variables */
  LinIf_ScheduleTableTickType             LinIf_FrameMaxDelay;                    /*!< Remaining transmission duration of current frame in ticks */
  uint8                                   LinIf_OnSleepModeFrameHandling;         /*!< Internal node management state machine */
# if ( LINIF_FIXEDFRAMESDULIST == STD_ON )
  uint8                                   LinIf_AtNodeConfigurationCmd;           /*!< Node configuration command state machine */
# endif
  uint8                                   LinIf_PduRNotificationFlag;             /*!< PduR notification pending flag; either Idle, Rx or Tx notification */
  PduIdType                               LinIf_PduRNotificationPduId;            /*!< PduId to pass to PduR if notification is pending */
  uint8                                   LinIf_PduRNotificationPduLength;        /*!< Pdu length to pass to PduR if notification is pending */
# if ( LINIF_PDUR_ONLY_UL_OPTIMIZATION == STD_OFF )
  uint8                                   LinIf_PduRNotificationFctIndex;         /*!< Index of PduR notification callback function pointer list */
# endif
# if ( LINIF_BUSMIRRORING == STD_ON )
  boolean                                 LinIf_MirroringEventPending;            /*!< Bus mirroring flag that frame is pending for mirroring */
  Lin_FramePidType                        LinIf_MirroringPid;                     /*!< Bus mirroring frame PID to report */
  Lin_StatusType                          LinIf_MirroringLinStatus;               /*!< Bus mirroring LIN driver status value to report */
# endif
  uint8                                   LinIf_RxMirrorData[LINIF_LINPDU_SIZE];  /*!< Local buffer to store frame reception or mirror data */
  /* additional TxData buffer is required: If in the same cycle an Rx frame (of previous slot) is processed/received
     while a slot with a Tx frame is started, only one internal buffer is not sufficient */
  uint8                                   LinIf_TxData[LINIF_LINPDU_SIZE];        /*!< Local buffer to store frame transmission data */
} LinIf_MasterChannelDataType;

/*! Slave channel RAM structure */
typedef struct LinIf_SlaveChannelDataTypeTag /*! Control structure of LinIf for one slave channel */
{
  uint16                                  LinIf_BusIdleTimer;                     /*!< Bus idle timeout timer */
  uint8                                   LinIf_FrameState;                       /*!< State machine of current handled frame */
  uint8                                   LinIf_NmWakeupState;                    /*!< Slave internal node management state machine */
  uint8                                   LinIf_RespErrValue;                     /*!< Flag for current state of response error signal */
  uint8                                   LinIf_ConfiguredNAD;                    /*!< Configured NAD */
# if ( LINIF_NC_GENERAL_SUPPORTED == STD_ON )
  uint8                                   LinIf_NcRespBuffer[LINIF_NC_BUF_SIZE];  /*!< Buffer holding the response data of a node configuration service */
  uint16                                  LinIf_NasTimer;                         /*!< N_As timeout timer for node configuration service handling */
# endif
# if ( LINIF_BUSMIRRORING == STD_ON )
  uint8                                   LinIf_MirrorTxData[LINIF_LINPDU_SIZE];  /*!< Local buffer to store frame transmission data for mirroring */
# endif
# if ( LINIF_NC_PRODUCT_ID_CONFIGURABLE_SUPPORTED == STD_ON )
  uint16                                  LinIf_SupplierId;                       /*!< Configurable Supplier Id */
  uint16                                  LinIf_FunctionId;                       /*!< Configurable Function Id */
  uint8                                   LinIf_VariantId;                        /*!< Configurable Variant Id */
# endif
# if ( LINIF_SLAVE_J2602_SUPPORTED == STD_ON )
  uint8                                   LinIf_J2602ErrorBits;                   /*!< Active J2602 error states as bit mask */
  uint8                                   LinIf_J2602TxErrorStatus;               /*!< J2602 error state currently transmitted */
# endif
} LinIf_SlaveChannelDataType;

#endif /* LINIF_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: LinIf_Types.h
 *********************************************************************************************************************/

