/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2024 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *        \file   Dlt.c
 *        \brief  Dlt source file
 *
 *      \details  Source file of the AUTOSAR Diagnostic Log and Trace, according to:
 *                AUTOSAR Diagnostic Log and Trace, AUTOSAR Release 4.0
 *
 *********************************************************************************************************************/

 /*********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define DLT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dlt.h"
#include "Rte_Dlt.h"

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# include "PduR_Lcfg.h"
# include "PduR_PBcfg.h"
# include "vstdlib.h"

# if (DLT_NVRAM_SUPPORT == STD_ON)
#  include "NvM.h"
# endif

# if (DLT_SYNC_TIME_SUPPORT == STD_ON)
#  include "StbM.h"
# endif

# if (DLT_OS_SOFTWARE_TRACING == STD_ON) || (DLT_IS_EXPLICIT_CORE_USED == STD_ON)
#  include "Os.h"
# endif
#endif

#if (DLT_DEV_ERROR_REPORT == STD_ON)
# include "Det.h" /* Include Development Error Tracer */
#endif

#if defined (DLT_USE_COMLAYER_XCP) && (DLT_USE_COMLAYER_XCP == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# include "Xcp.h"

# if (DLT_USE_VERBOSE_MODE == STD_ON)
#  include "vstdlib.h"            /* for VStdMemCpy */
# endif
#endif

/*lint -e451 */ /* Suppress ID451 because MemMap.h cannot use a include guard */
/*lint -e522 */ /* Suppress ID522 because all functions without side-effect have to have no side-effect */

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
/* vendor specific version information is BCD coded */
/* Dlt500 */
#if((DLT_SW_MAJOR_VERSION != (0x07u)) || (DLT_SW_MINOR_VERSION != (0x01u)) || (DLT_SW_PATCH_VERSION != (0x00u)))
# error "Vendor specific version numbers of Dlt.c and Dlt.h are inconsistent"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# if (DLT_USED_CORE_ID >= DLT_NUMBER_OF_CORES)
#  error "Incorrect configuration; take care that referenced core id is smaller than the number of AUTOSAR cores"
# endif

# define Dlt_GetCurrentTime() DLT_GET_TIME_MEASUREMENT_FCT()

# if (DLT_TIMESTAMP_SUPPORT == STD_ON)
#  define DLT_MIN_MESSAGE_LENGTH                 0x19u
# else
#  define DLT_MIN_MESSAGE_LENGTH                 0x15u
# endif

# define DLT_NUMBER_OF_SERVICES                  0x25u

# define kDltStateMachineStates                  DLT_STATE_NUMBER_OF_STATES
# define kDltStateMachineEvents                  0x7u

/* Header Type Masks */
# define Dlt_StandardHeaderType_UEH_Mask         0x01u /* Mask for Use Extended Header */
# define Dlt_StandardHeaderType_MSBF_Mask        0x02u /* Mask for Most Significant Byte First - if set => Little Endian, else Big Endian */
# define Dlt_StandardHeaderType_WEID_Mask        0x04u /* Mask for With ECU ID */
# define Dlt_StandardHeaderType_WSID_Mask        0x08u /* Mask for With Session ID */
# define Dlt_StandardHeaderType_WTMS_Mask        0x10u /* Mask for With Timestamp */
# define Dlt_StandardHeaderType_SWV_Mask         0xE0u /* Mask for Software Version */

# define Dlt_ExtendedHeaderType_VERB_Mask        0x01u /* Mask for Verbose Mode */
# define Dlt_ExtendedHeaderType_MSTP_Mask        0x0Eu /* Mask for Message Type */
# define Dlt_ExtendedHeaderType_MTIN_Mask        0xF0u /* Mask for Message Type Info */

# define Dlt_InternalMessageTypeTypeMask         0x07u /* Mask for Dlt_MessageOptionsType->Dlt_MessageTypeType:
                                                        * DLT_TYPE_LOG = 0, DLT_TYPE_APP_TRACE = 1,
                                                        * DLT_TYPE_NW_TRACE = 2, DLT_TYPE_CONTROL = 3, */

/* Control message pre-config */

# define Dlt_ControlMessageHeaderType_MSBF       0x23u /* Header type  - 0b001 0 0 0 1 1 - Version 3 | WTMS 0 | SID 0 | WEID 0 | MSBF 1 | UEH 1 */
# define Dlt_ControlMessageHeaderType_LSBF       0x21u /* Header type  - 0b001 0 0 0 0 1 - Version 3 | WTMS 0 | SID 0 | WEID 0 | MSBF 0 | UEH 1 */
# define Dlt_ControlMessageMessageInfo           0x26u /* Message Info - 0b0010 011 0    - Response | Control | Non-Verbose */

# ifndef DLT_TIMEOUT /* COV_DLT_MSR_COMPATIBILITY */
#  define DLT_TIMEOUT                            0x200u /* Timeout limit - 512*MainFunctionCycle */
# endif

# ifndef DLT_MAX_DLT_MESSAGES_IN_ONE_TRANSMIT
#  define DLT_MAX_DLT_MESSAGES_IN_ONE_TRANSMIT   0x100u
# endif

# ifndef DLT_AVOID_DEADLOCK_COUNT /* COV_DLT_MSR_COMPATIBILITY */
#  define DLT_AVOID_DEADLOCK_COUNT               0xC8u
# endif

/* Dlt_OptionType */
# define DLT_OPT_LOGLEVEL                        0x00u /*!< Option type value: set to log. */
# define DLT_OPT_TRACESTATUS                     0x01u /*!< Option type value: set to trace. */
# define DLT_OPT_BOOLEAN                         0x02u /*!< Option type value: set to a boolean attribute. */

/* Use this statement for Payload, but not for Service ID */
# ifndef Dlt_IsPayloadInBigEndian
#  define Dlt_IsPayloadInBigEndian() ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_MSBF_Mask) == Dlt_StandardHeaderType_MSBF_Mask)
# endif

# define DLT_NOT_SUPPORTED_RESPONSE_PAYLOAD_LENGTH (1u)
# define DLT_NOT_SUPPORTED_NUMBER_OF_ARGUMENTS (1u)

# define DLT_STD_HEADER_HEADERTYPE_INDEX (0u)
# define DLT_STD_HEADER_MESSAGECOUNTER_INDEX (1u)
# define DLT_STD_HEADER_LENGTH_INDEX (2u)
# define DLT_STD_HEADER_MANDATORY_END_INDEX (4u)
# define DLT_STD_HEADER_TYPE_WV_WT_NS_WE_MF_UE (0x37000000u) /* Header type - Version | WTMS | NoSID | WEID | MSBF | UEH - 0b001 1 0 1 1 1 */

# define DLT_EXT_HEADER_LENGTH (10u)
# define DLT_EXT_HEADER_MESSAGE_INFO_NONVERB_CONTROL_RESPONSE (0x2600u) /* Message info - Non-Verbose | DLT_TYPE_CONTROL | DLT_CONTROL_RESPONSE */

# define DLT_DEFAULT_ID_LENGTH (4u)
# define DLT_DEFAULT_RESPONSE_PAYLOAD_LENGTH (1u)
# define DLT_DEFAULT_RESPONSE_NUMBER_OF_ARGUMENTS_LENGTH (1u)

# define DLT_TYPE_INFO_UINT16   (0x00000042u)
# define DLT_TYPE_INFO_UINT8    (0x00000041u)

# define DLT_APPLICATION_ID_ALL (0x00000000u) /* Application ID - all */
# define DLT_CONTEXT_ID_ALL     (0x00000000u) /* Context ID - all */

/* Service specific constants */
/* - Dlt_Sm_BufferOverflowNotification */
# define DLT_SEND_BUFFER_OVERFLOW_NOTIFICATION_RESPONSE_LENGTH (0x0000001Fu) /* Response length - 28Byte */
# define DLT_SEND_BUFFER_OVERFLOW_NOTIFICATION_SERVICE_ID      (0x00000023u) /* Service ID - BufferOverflowNotification */
# define DLT_SEND_BUFFER_OVERFLOW_NOTIFICATION_RESPONSE_OK     (0x00u) /* Result - OK */
# define DLT_SEND_BUFFER_OVERFLOW_NOTIFICATION_PAYLOAD_LENGTH  (0x0005u) /* Payload length - 2Byte */

/* - Dlt_Sm_SendSyncTimeStamp */
# define DLT_SEND_SYNC_TIME_STAMP_RESPONSE_LENGTH (0x00000025u) /* Response length - 37Byte */
# define DLT_SEND_SYNC_TIME_STAMP_SERVICE_ID (0x00000024u) /* Service ID - BufferOverflowNotification */
# define DLT_SEND_SYNC_TIME_STAMP_RESPONSE_OK_TIMESTAMP_AVAILABLE (0x0001u) /* Result - OK, Status - Time Stamp Available */
# define DLT_SEND_SYNC_TIME_STAMP_PAYLOAD_LENGTH (0x000Bu) /* Payload length - 11Byte */

/* - Dlt_Srv_SetStatus */
# define DLT_SET_STATUS_PAYLOAD_LENGTH (0x0001u) /* Payload length - 1Byte */

/* - Dlt_Srv_GetLogInfo */
# define DLT_GET_LOG_INFO_PAYLOAD_LENGTH_WITHOUT_DESCRIPTIONS (0x0013u) /* Payload length without descriptions - 19Byte */
# define DLT_GET_LOG_INFO_PAYLOAD_LENGTH_WITH_DESCRIPTIONS (0x0017u) /* Payload length with descriptions - 23Byte */

/* - Dlt_Srv_SetDefaultLogLevel */
# define DLT_SET_DEFAULT_LOG_LEVEL_PAYLOAD_LENGTH (0x01u)

/* - Dlt_Srv_GetDefaultLogLevel */
# define DLT_GET_DEFAULT_LOG_LEVEL_PAYLOAD_LENGTH (0x02u)

/* - Dlt_Srv_SetDefaultTraceStatus */
# define DLT_SET_DEFAULT_TRACE_STATUS_PAYLOAD_LENGTH (0x01u)

/* - Dlt_Srv_GetDefaultTraceStatus */
# define DLT_GET_DEFAULT_TRACE_STATUS_PAYLOAD_LENGTH (0x02u)

/* - Dlt_Srv_SetUseECUID */
# define DLT_SET_USE_ECU_ID_PAYLOAD_LENGTH (0x01u)

/* - Dlt_Srv_GetUseECUID */
# define DLT_GET_USE_ECU_ID_PAYLOAD_LENGTH (0x02u)

/* - Dlt_Srv_SetUseSessionID */
# define DLT_SET_USE_SESSION_ID_PAYLOAD_LENGTH (0x01u)

/* - Dlt_Srv_GetUseSessionID */
# define DLT_GET_USE_SESSION_ID_PAYLOAD_LENGTH (0x02u)

/* - Dlt_Srv_UseTimestamp */
# define DLT_SET_USE_TIMESTAMP_PAYLOAD_LENGTH (0x01u)

/* - Dlt_Srv_GetUseTimestamp */
# define DLT_GET_USE_TIMESTAMP_PAYLOAD_LENGTH (0x02u)

/* - Dlt_Srv_SetMessageFiltering */
# define DLT_SET_USE_MESSAGE_FILTERING_PAYLOAD_LENGTH (0x01u)

/* - Dlt_Srv_GetMessageFilteringStatus */
# define DLT_GET_USE_MESSAGE_FILTERING_PAYLOAD_LENGTH (0x02u)

/* - Dlt_Srv_GetTraceStatus */
# define DLT_GET_TRACE_STATUS_PAYLOAD_LENGTH (0x02u)

/* - Dlt_Srv_GetLogChannelNames */
# define DLT_GET_LOG_CHANNEL_NAMES_PAYLOAD_LENGTH (0x02u)

/* - Dlt_Srv_StoreConfig */
# define DLT_STORE_CONFIG_PAYLOAD_LENGTH (0x01u)

/* - Dlt_Srv_ResetToFactoryDefault */
# define DLT_RESET_TO_FACTORY_DEFAULT_PAYLOAD_LENGTH (0x01u)

/* - Dlt_Srv_GetSoftwareVersion */
# define DLT_GET_SOFTWARE_VERSION_PAYLOAD_LENGTH (0x0Du)

/* - Dlt_Srv_SetLogChannelAssignment */
# define DLT_SET_LOG_CHANNEL_ASSIGNMENT_PAYLOAD_LENGTH (0x01u)
# define DLT_SET_LOG_CHANNEL_ASSIGNMENT_REMOVE_FROM_REQ (0x00u)
# define DLT_SET_LOG_CHANNEL_ASSIGNMENT_ADD_FROM_REQ (0x01u)

/* - Dlt_Srv_SetLogChannelThreshold */
# define DLT_SET_LOG_CHANNEL_THRESHOLD_PAYLOAD_LENGTH (0x02u)

/* - Dlt_Srv_GetLogChannelThreshold */
# define DLT_GET_LOG_CHANNEL_THRESHOLD_PAYLOAD_LENGTH (0x03u)

/* - Dlt_Srv_CallSwcInjection */
# define DLT_CALL_SWC_INJECTION_PAYLOAD_LENGTH (0x01u)
# define DLT_CALL_SWC_INJECTION_START_IDX_OF_SESSION_ID (0x04u)
# define DLT_CALL_SWC_INJECTION_START_IDX_OF_APPLICATION_ID (0x06u)


/* - Dlt_DetForwardErrorTrace */
# define DLT_DET_PAYLOAD_LENGTH_VERBOSE (21u)
# define DLT_DET_PAYLOAD_LENGTH_NON_VERBOSE (9u)
# define DLT_DET_MODULE_ID_USED_AS_SESSION_ID (0x0000000Fu)
# define DLT_DET_APPLICATION_ID_INDEX (0x01u)
# define DLT_DET_CONTEXT_ID_INDEX (0x00u)
# define DLT_DET_NUMBER_OF_ARGUMENTS (0x04u)

# define DLT_DEM_APPLICATION_ID_INDEX (0x00u)
# define DLT_DEM_CONTEXT_ID_INDEX (0x00u)
# if (DLT_IS_DEM_PRESENT == STD_ON)
/* - Dlt_DemTriggerOnEventStatus */
# define DLT_DEM_PAYLOAD_LENGTH_VERBOSE (11u)
# define DLT_DEM_PAYLOAD_LENGTH_NON_VERBOSE (7u)
# define DLT_DEM_MESSAGE_ID (0x00000001u)
# define DLT_DEM_MODULE_ID_USED_AS_SESSION_ID (0x00000036u)
# define DLT_DEM_NUMBER_OF_ARGUMENTS (0x02u)
# endif /* (DLT_IS_DEM_PRESENT == STD_ON) */

# if (DLT_OS_SOFTWARE_TRACING == STD_ON)
#  define DLT_OS_VTH_APPLICATION_INDEX (0x03u)
#  define DLT_OS_VTH_SCHEDULE_CONTEXT_INDEX (0x00u)
#  define DLT_OS_VTH_ACTIVATION_CONTEXT_INDEX (0x01u)
#  define DLT_OS_VTH_SETEVENT_CONTEXT_INDEX (0x02u)

#  define DLT_OS_VTH_SCHEDULE_LOG_INFO_INDEX (0x02u)
#  define DLT_OS_VTH_ACTIVATION_LOG_INFO_INDEX (0x03u)
#  define DLT_OS_VTH_SETEVENT_LOG_INFO_INDEX (0x04u)
# endif

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

#define DLT_SHIFT_5BIT (5u)
#define DLT_SHIFT_8BIT (8u)
#define DLT_SHIFT_16BIT (16u)
#define DLT_SHIFT_24BIT (24u)

#define DLT_DET_MESSAGE_ID (0x00000002u)
#define DLT_DET_INVALID_XCP_DET_ID (0x01000000u)

#define DLT_TYPE_LONG_LENGTH (4u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* PRQA S 1533 EOF */ /* MD_Dlt_1533 */

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
/* PRQA S 1514 EOF */ /* MD_Dlt_1514 */
#endif

/* PRQA S 3453 FUNCTIONLIKEMACRO_C */ /*  MD_MSR_FctLikeMacro */

#define DLT_BEGIN_CRITICAL_SECTION_APIS() SchM_Enter_Dlt_DLT_EXCLUSIVE_AREA_APIS()
#define DLT_END_CRITICAL_SECTION_APIS()   SchM_Exit_Dlt_DLT_EXCLUSIVE_AREA_APIS()
#define DLT_BEGIN_CRITICAL_SECTION_TX()   SchM_Enter_Dlt_DLT_EXCLUSIVE_AREA_TX()
#define DLT_END_CRITICAL_SECTION_TX()     SchM_Exit_Dlt_DLT_EXCLUSIVE_AREA_TX()
#define DLT_BEGIN_CRITICAL_SECTION_RX()   SchM_Enter_Dlt_DLT_EXCLUSIVE_AREA_RX()
#define DLT_END_CRITICAL_SECTION_RX()     SchM_Exit_Dlt_DLT_EXCLUSIVE_AREA_RX()

#if (DLT_USE_DEFAULT_API == STD_ON)
# define DltCheckLogChannelName()         (LogChannelName != NULL_PTR)
#else
# define DltCheckLogChannelName()         (TRUE == TRUE)
#endif

/* PRQA L:FUNCTIONLIKEMACRO_C */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
typedef enum
{
  kDltEventInitDlt                 = 0u, /*!< Sub state: init. */
  kDltEventOneBufferHasContent     = 1u, /*!< Sub state: at least one buffer has content. */
  kDltEventTransmitCallRejected    = 2u, /*!< Sub state: transmission request was rejected. */
  kDltEventSendingFinished         = 3u, /*!< Sub state: sending is finished and no data to transmit is buffered. */
  kDltEventTxStillActive           = 4u, /*!< Sub state: sending is still on-going. */
  kDltEventTimeout                 = 5u, /*!< Sub state: transmission was aborded due to a timeout of PduR. */
  kDltEventDebugModeChanged        = 6u  /*!< Sub state: the debug mode changed. */
} Dlt_EventType;                         /*!< Event type of sub state machine. */

typedef struct
{
  uint8  Buffer[DLT_MESSAGE_BUFFER_SIZE]; /*!< Member of message buffer: the buffer. */
  uint32 Write;                           /*!< Member of message buffer: the write pointer. */
  uint32 Read;                            /*!< Member of message buffer: the read pointer. */
} Dlt_BufferType;                         /*!< Message buffer type. */

typedef struct
{
  uint8  Buffer[DLT_CONTROL_BUFFER_SIZE]; /*!< Member of control buffer: the buffer. */
  uint32 Write;                           /*!< Member of control buffer: the write pointer. */
  uint32 Read;                            /*!< Member of control buffer: the read pointer. */
} Dlt_ControlBufferType;                  /*!< Control buffer type. */

typedef uint8 Dlt_OptionType; /*!< Option type: distinction between log, trace, verbose and boolean. */
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */


/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/* ====================================================================================================================
 * COMLAYER AUTOSAR
 * ====================================================================================================================
 */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

# define DLT_START_SEC_VAR_CLEARED_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

DLT_LOCAL VAR(Dlt_GlobalStateType, DLT_VAR_CLEARED)  Dlt_GlobalState = DLT_GLOBAL_STATE_UNINIT;
DLT_LOCAL VAR(Dlt_GlobalStateType, DLT_VAR_CLEARED)  Dlt_GlobalStateNew = DLT_GLOBAL_STATE_UNINIT;

DLT_LOCAL VAR(Dlt_BufferType, DLT_VAR_CLEARED)       Dlt_SendBuffer[Dlt_GetSizeOfLogChannelDescriptor()] = {0u};
DLT_LOCAL VAR(Dlt_StateType, DLT_VAR_CLEARED)        Dlt_CurrentState[Dlt_GetSizeOfLogChannelDescriptor()] = {0u};
DLT_LOCAL VAR(uint8, DLT_VAR_CLEARED)                Dlt_MessageCounter[Dlt_GetSizeOfLogChannelDescriptor()] = {0u};
DLT_LOCAL VAR(Dlt_BufferOverflowStatusType, DLT_VAR_CLEARED) Dlt_BufferOverflowNotificationStatus[Dlt_GetSizeOfLogChannelDescriptor()] = {0u};

# if (DLT_NVRAM_SUPPORT == STD_ON)
VAR(Dlt_NvDataType, DLT_VAR_CLEARED)                 Dlt_NvData = {0u};
# endif

# if (DLT_OS_SOFTWARE_TRACING == STD_ON)
DLT_LOCAL VAR(uint8, DLT_VAR_CLEARED)                Dlt_EventCounter = 0u;
# endif

# define DLT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DLT_START_SEC_VAR_INIT_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Variables for Dlt runtime configuration */
VAR(boolean, DLT_VAR_INIT)                           Dlt_FilterMessages = TRUE;
# if (DLT_TIMESTAMP_SUPPORT == STD_ON)
#  if (DLT_HEADER_USE_TIMESTAMP == STD_ON)
VAR(boolean, DLT_VAR_INIT)                           Dlt_HeaderUseTimestamp = TRUE;
#  else
VAR(boolean, DLT_VAR_INIT)                           Dlt_HeaderUseTimestamp = FALSE;
#  endif
# endif
# if (DLT_HEADER_USE_ECU_ID == STD_ON)
VAR(boolean, DLT_VAR_INIT)                           Dlt_HeaderUseEcuId = TRUE;
# else
VAR(boolean, DLT_VAR_INIT)                           Dlt_HeaderUseEcuId = FALSE;
# endif
# if (DLT_HEADER_USE_SESSION_ID == STD_ON)
VAR(boolean, DLT_VAR_INIT)                           Dlt_HeaderUseSessionId = TRUE;
# else
VAR(boolean, DLT_VAR_INIT)                           Dlt_HeaderUseSessionId = FALSE;
# endif
VAR(boolean, DLT_VAR_INIT)                           Dlt_DefaultTraceStatus = DLT_DEFAULT_TRACE_STATUS; /* Tells if trace messages shall be forwarded by Dlt */
VAR(Dlt_MessageLogLevelType, DLT_VAR_INIT)           Dlt_DefaultMaxLogLevel = (Dlt_MessageLogLevelType)DLT_DEFAULT_MAX_LOG_LEVEL; /* Tells if log messages shall be forwarded by Dlt */

DLT_LOCAL VAR(uint32, DLT_VAR_INIT)                  Dlt_DefaultLogChannelIndex = DLT_DEFAULT_LOG_CHANNEL_INDEX; /* Tells on which log channel the Dlt shall transmit its messages by default */
DLT_LOCAL VAR(uint32, DLT_VAR_INIT)                  Dlt_NumberOfRegisteredContextIds = 2u;

# define DLT_STOP_SEC_VAR_INIT_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DLT_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

DLT_LOCAL VAR(Dlt_ControlBufferType, DLT_VAR_NO_INIT) Dlt_ControlSendBuffer[Dlt_GetSizeOfLogChannelDescriptor()];
DLT_LOCAL VAR(uint8, DLT_VAR_NO_INIT)                 Dlt_ControlSendMessage[Dlt_GetSizeOfLogChannelDescriptor()][DLT_MAX_CONTROL_MESSAGE_LENGTH];
DLT_LOCAL VAR(Dlt_ControlBufferType, DLT_VAR_NO_INIT) Dlt_ReceiveBuffer;
DLT_LOCAL VAR(Dlt_RxInfoType, DLT_VAR_NO_INIT)        Dlt_RxInfo;
DLT_LOCAL VAR(uint8, DLT_VAR_NO_INIT)                 Dlt_ReceiveMessage[DLT_MAX_CONTROL_MESSAGE_LENGTH];

DLT_LOCAL VAR(uint8, DLT_VAR_NO_INIT)                 Dlt_SendData[Dlt_GetSizeOfLogChannelDescriptor()][DLT_MESSAGE_BUFFER_SIZE];

DLT_LOCAL VAR(uint8,  DLT_VAR_NO_INIT)                Dlt_TransmitMessage[Dlt_GetSizeOfLogChannelDescriptor()][DLT_MAX_MESSAGE_LENGTH];
DLT_LOCAL VAR(boolean,  DLT_VAR_NO_INIT)              Dlt_MessageTransmitted[Dlt_GetSizeOfLogChannelDescriptor()];
DLT_LOCAL VAR(uint16, DLT_VAR_NO_INIT)                Dlt_Timeout[Dlt_GetSizeOfLogChannelDescriptor()];

VAR(SduDataPtrType, DLT_VAR_NO_INIT)                  Dlt_SendPdu_DataPtr[Dlt_GetSizeOfLogChannelDescriptor()];
VAR(PduLengthType, DLT_VAR_NO_INIT)                   Dlt_SendPdu_DataLength[Dlt_GetSizeOfLogChannelDescriptor()];

# if (DLT_SYNC_TIME_SUPPORT == STD_ON)
DLT_LOCAL VAR(Dlt_SyncTimeStateType, DLT_VAR_NO_INIT) Dlt_SyncTimeState;
DLT_LOCAL VAR(uint32, DLT_VAR_NO_INIT) Dlt_SyncTimeTimeoutCnt;
#endif

# define DLT_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#else /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/* ====================================================================================================================
 * COMLAYER XCP
 * ====================================================================================================================
 */

# define DLT_START_SEC_VAR_CLEARED_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

DLT_LOCAL VAR(Dlt_GlobalStateType, DLT_VAR_CLEARED) Dlt_GlobalState = DLT_GLOBAL_STATE_UNINIT;
DLT_LOCAL VAR(Dlt_GlobalStateType, DLT_VAR_CLEARED) Dlt_GlobalStateNew = DLT_GLOBAL_STATE_UNINIT;

DLT_LOCAL VAR(boolean, DLT_VAR_CLEARED)             Dlt_EnterDetTransmitContext = FALSE;

# define DLT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DLT_START_SEC_VAR_NO_INIT_32
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Variable is loaded with the last DET error code */
VAR(uint32, DLT_VAR_NO_INIT)                          Dlt_DetErrorCode;

# define DLT_STOP_SEC_VAR_NO_INIT_32
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DLT_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Variable is loaded with DEM event id and status code. */
VAR(Dlt_DemEventStatusType, DLT_VAR_NO_INIT)          Dlt_DemEventStatus; /*lint -e830 */ /* Suppress ID830 */ /*lint -e552 */ /* Suppress ID552 */

/* Variable is loaded with message ID of non-verbose message . */
VAR(Dlt_MessageIDType, DLT_VAR_NO_INIT)               Dlt_NonVerboseMessageId;

# if (DLT_USE_VERBOSE_MODE == STD_ON)
/* Buffer is loaded with payload of verbose message. */
VAR(uint8, DLT_VAR_NO_INIT)                           Dlt_VerboseMessageData[DLT_MAX_MESSAGE_LENGTH];
# endif /* (DLT_USE_VERBOSE_MODE == STD_ON) */

# define DLT_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
#define DLT_START_SEC_CONST_8
#include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
CONST(uint8, DLT_CONST) Dlt_StandardHeaderType_SoftwareVersion = 0x01u;
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

CONST(uint8, DLT_CONST) Dlt_ModuleTable[256] =
{
/*  0      1      2      3      4      5      6      7      8      9      A      B      C      D      E      F           */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x0Au, 0x0Bu, 0x0Cu, 0x0Du, 0x00u, 0x00u,  /* 0x */ /* 0x0A = ECUM, 0x0B = FIM, 0x0C = COMM, 0x0D = WDGM */
  0x00u, 0x00u, 0x00u, 0x00u, 0x14u, 0x15u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Au, 0x00u, 0x00u, 0x1Du, 0x00u, 0x1Fu,  /* 1x */ /* 0x14 = NVM, 0x15 = FEE, 0x1A = XCP, 0x1D = NM, 0x1F = CANNM */
  0x20u, 0x00u, 0x00u, 0x23u, 0x24u, 0x25u, 0x00u, 0x00u, 0x28u, 0x00u, 0x2Au, 0x2Bu, 0x00u, 0x00u, 0x00u, 0x00u,  /* 2x */ /* 0x20 = FRNM, 0x23 = CANTP, 0x24 = FRTP, 0x25 = J1939TP, 0x28 = EA, 0x2A = BSWM, 0x2B = WDGIF */
  0x00u, 0x00u, 0x32u, 0x33u, 0x34u, 0x35u, 0x36u, 0x00u, 0x38u, 0x00u, 0x00u, 0x00u, 0x3Cu, 0x3Du, 0x3Eu, 0x00u,  /* 3x */ /* 0x32 = COM, 0x33 = PDUR, 0x34 = IPDUM, 0x35 = DCM, 0x36 = DEM, 0x38 depends on InstanceId (SOAD = 0x00, TCPIP = 0x01, IPV4 = 0x02, IPV6 = 0x04), 0x3C = CANIF, 0x3D = FRIF, 0x3E = LINIF */
  0x00u, 0x41u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x49u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* 4x */ /* 0x41 = ETHIF, 0x49 = ETHTRCV */
  0x50u, 0x51u, 0x52u, 0x53u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x5Cu, 0x00u, 0x00u, 0x00u,  /* 5x */ /* 0x50 = CAN, 0x51 = FR, 0x52 = LIN, 0x53 = SPI, 0x5C = FLS */
  0x00u, 0x00u, 0x00u, 0x00u, 0x64u, 0x65u, 0x66u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* 6x */ /* 0x64 = GPT, 0x65 = MCU, 0x66 = WDG */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x78u, 0x79u, 0x7Au, 0x7Bu, 0x00u, 0x00u, 0x00u, 0x00u,  /* 7x */ /* 0x78 = DIO, 0x79 = PWM, 0x7A = ICU, 0x7B = ADC */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x8Cu, 0x8Du, 0x8Eu, 0x8Fu,  /* 8x */ /* 0x8C = CANSM, 0x8D = LINSM, 0x8E = FRSM , 0x8F = ETHSM */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* 9x */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xAEu, 0x00u,  /* Ax */ /* 0x(1)AE = ETH */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* Bx */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* Cx */
  0x00u, 0x00u, 0x00u, 0xD3u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* Dx */ /* 0xD3 = FRXCP */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,  /* Ex */
  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu   /* Fx */ /* 0xFF : depends on InstanceId - 0x00: CANXCP, 0x65: RCS, 0x66: HTTP, 0x67: TLS, 0x68: Crypto, 0x69: DNS, 0x6A: JSON, 0x6B..0x6D: EXI, 0x6E: IPBase, 0x6F: SCC */
};

#define DLT_STOP_SEC_CONST_8
#include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define DLT_START_SEC_CODE
#include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ####################################################################################################################
 *   HH      HH  EEEEEEEEEE  LL          PPPPPPPP    EEEEEEEEEE  RRRRRRRR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HHHHHHHHHH  EEEEEEEE    LL          PPPPPPPP    EEEEEEEE    RRRRRRRR
 *   HH      HH  EE          LL          PP          EE          RR    RR
 *   HH      HH  EE          LL          PP          EE          RR      RR
 *   HH      HH  EEEEEEEEEE  LLLLLLLLLL  PP          EEEEEEEEEE  RR      RR
 * ####################################################################################################################
 */
#if ((DLT_USE_COMLAYER_XCP == STD_ON) || (DLT_RX_DATA_PATH_SUPPORT == STD_ON)) /* COV_DLT_FILTER_ASR_XCP */
/**********************************************************************************************************************
  Dlt_Hlp_CopyArrayToLongLittleEndian_AndMoveIndex()
**********************************************************************************************************************/
/*! \brief       Copies data from array to variable.
 *  \details     Copies 4 byte from source array to variable with least significant byte first.
 *  \param[in]   SourceData            Data source.
 *  \param[out]  DestData              Pointer to data destination.
 *  \param[out]  StartIndex            Pointer to start index of source array where the data is copied from.
 *  \pre         The source array must have at least the size of 4.
 *  \pre         Only reentrant if destination and source pointer content are not affected by second call while
 *               interrupted.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_XCP == STD_ON) || DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongLittleEndian_AndMoveIndex(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) StartIndex);
#endif

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

/**********************************************************************************************************************
  Dlt_Hlp_CopyLongToArrayBigEndian()
**********************************************************************************************************************/
/*! \brief       Copies 4 byte from variable to array.
 *  \details     Copying order is big endian.
 *  \param[in]   SourceData            Data source.
 *  \param[out]  DestData              Pointer to data destination.
 *  \pre         The destination array must have at least the size of 4.
 *  \pre         Only reentrant if destination and source pointer content are not affected by second call while
 *               interrupted.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyLongToArrayBigEndian(
  uint32 SourceData,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData);

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON) || (DLT_OS_SOFTWARE_TRACING == STD_ON)
/**********************************************************************************************************************
  Dlt_Hlp_CopyArrayToLongBigEndian()
**********************************************************************************************************************/
/*! \brief       Copies data from array to variable in big endian.
 *  \details     Copies 4 byte from source array to variable with most significant byte first.
 *  \param[in]   SourceData            Data source.
 *  \param[out]  DestData              Pointer to data destination.
 *  \pre         The source array must have at least the size of 4.
 *  \pre         Only reentrant if destination and source pointer content are not affected by second call while
 *               interrupted.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON) && ((DLT_RX_DATA_PATH_SUPPORT ==
 *               STD_ON) || (DLT_OS_SOFTWARE_TRACING == STD_ON)).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongBigEndian(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData);
# endif

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex()
**********************************************************************************************************************/
/*! \brief       Copies data from array to variable.
 *  \details     Copies 4 byte from source array to variable with most significant byte first.
 *  \param[in]   SourceData            Data source.
 *  \param[out]  DestData              Pointer to data destination.
 *  \param[out]  StartIndex            Pointer to start index of source array where the data is copied from.
 *  \pre         The source array must have at least the size of 4.
 *  \pre         Only reentrant if destination and source pointer content are not affected by second call while
 *               interrupted.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON) and (DLT_RX_DATA_PATH_SUPPORT ==
 *               STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) StartIndex);

#  if (DLT_INJECTION_CALL_SUPPORT == STD_ON)
/**********************************************************************************************************************
 Dlt_Hlp_CopyArrayToLongLittleEndian()
 **********************************************************************************************************************/
/*! \brief       Copies data from array to variable in little endian.
 *  \details     Copies 4 byte from source array to variable with least significant byte first.
 *  \param[in]   SourceData            Data source.
 *  \param[out]  DestData              Pointer to data destination.
 *  \pre         The source array must have at least the size of 4.
 *  \pre         Only reentrant if destination and source pointer content are not affected by second call while
 *               interrupted.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON) and (DLT_RX_DATA_PATH_SUPPORT ==
 *               STD_ON) and (DLT_INJECTION_CALL_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongLittleEndian(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData);
#  endif
# endif

/**********************************************************************************************************************
  Dlt_Hlp_CopyStringToLongBigEndian()
 **********************************************************************************************************************/
/*! \brief       Copies data from array to variable in big endian.
 *  \details     Copies 4 byte from source array to variable with most significant byte first.
 *  \param[in]   SourceData            Data source.
 *  \param[out]  DestData              Pointer to data destination.
 *  \pre         The source array must have at least the size of 4.
 *  \pre         Only reentrant if destination and source pointer content are not affected by second call while
 *               interrupted.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyStringToLongBigEndian(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData);

# if (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN)
#  define Dlt_Hlp_CopyLongToArray_AndMoveIndex Dlt_Hlp_CopyLongToArrayLittleEndian_AndMoveIndex
#  define Dlt_Hlp_CopyIntToArray_AndMoveIndex  Dlt_Hlp_CopyIntToArrayLittleEndian_AndMoveIndex
# else
#  define Dlt_Hlp_CopyLongToArray_AndMoveIndex Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex
#  define Dlt_Hlp_CopyIntToArray_AndMoveIndex  Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex
# endif

# if ((DLT_RX_DATA_PATH_SUPPORT == STD_ON) || (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN))
/**********************************************************************************************************************
  Dlt_Hlp_CopyLongToArrayLittleEndian_AndMoveIndex()
**********************************************************************************************************************/
/*! \brief       Copies data from variable to array.
 *  \details     Copies 4 byte variable to destination array with least significant byte first.
 *  \param[in]   SourceData            Data source.
 *  \param[out]  DestData              Pointer to data destination.
 *  \param[out]  index                 Pointer to start index of destination array where the data is copied to.
 *  \pre         The destination array must have at least the size of 4.
 *  \pre         Only reentrant if destination and source pointer content are not affected by second call while
 *               interrupted.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON) or (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyLongToArrayLittleEndian_AndMoveIndex(
  uint32 SourceData,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index);

/**********************************************************************************************************************
  Dlt_Hlp_CopyIntToArrayLittleEndian_AndMoveIndex()
**********************************************************************************************************************/
/*! \brief       Copies data from variable to array.
 *  \details     Copies 2 byte variable to destination array with least significant byte first.
 *  \param[in]   SourceData            Data source.
 *  \param[out]  DestData              Pointer to data destination.
 *  \param[out]  index                 Pointer to start index of destination array where the data is copied to.
 *  \pre         The destination array must have at least the size of 2.
 *  \pre         Only reentrant if destination and source pointer content are not affected by second call while
 *               interrupted.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON) or (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyIntToArrayLittleEndian_AndMoveIndex(
  uint16 SourceData,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index);
# endif

/**********************************************************************************************************************
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex()
**********************************************************************************************************************/
/*! \brief       Copies data from variable to array.
 *  \details     Copies 4 byte variable to destination array with most significant byte first.
 *  \param[in]   SourceData            Data source.
 *  \param[out]  DestData              Pointer to data destination.
 *  \param[out]  index                 Pointer to start index of destination array where the data is copied to.
 *  \pre         The destination array must have at least the size of 4.
 *  \pre         Only reentrant if destination and source pointer content are not affected by second call while
 *               interrupted.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(
  uint32 SourceData,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index);

/**********************************************************************************************************************
  Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex()
**********************************************************************************************************************/
/*! \brief       Copies data from variable to array.
 *  \details     Copies 2 byte variable to destination array with most significant byte first.
 *  \param[in]   SourceData            Data source.
 *  \param[out]  DestData              Pointer to data destination.
 *  \param[out]  index                 Pointer to start index of destination array where the data is copied to.
 *  \pre         The destination array must have at least the size of 2.
 *  \pre         Only reentrant if destination and source pointer content are not affected by second call while
 *               interrupted.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex(
  uint16 SourceData,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index);

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dlt_Hlp_DecToAscii()
 **********************************************************************************************************************/
/*! \brief       Transforms a decimal value to two ASCII letters depending on the given endianness.
 *  \details     -
 *  \param[out]  AsciiValue            Pointer to the two ASCII letters.
 *  \param[in]   DecValue              The original decimal value.
 *  \param[in]   IsBigEndian           The endianness (if true, it is big endian, otherwise it is little endian).
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON) && (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Hlp_DecToAscii(
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) AsciiValue,
  uint8 DecValue,
  boolean IsBigEndian);
# endif

/**********************************************************************************************************************
  Dlt_Hlp_IsDebugModeUnchanged()
**********************************************************************************************************************/
/*! \brief       Indicates if debug mode change is requested on given log channel.
 *  \details     -
 *  \param[in]   LogChannelIndex       The log channel.
 *  \return      FALSE                 The debug mode is not requested to be changed.
 *  \return      TRUE                  The debug mode is requested to be changed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(boolean, DLT_CODE) Dlt_Hlp_IsDebugModeUnchanged(uint32 LogChannelIndex);

/**********************************************************************************************************************
  Dlt_Hlp_IsBufferingAllowed()
**********************************************************************************************************************/
/*! \brief       Indicates if debug mode and debug event state allows buffering.
 *  \details     -
 *  \param[in]   LogChannelIndex       The log channel.
 *  \return      FALSE                 The debug mode does not allow buffering.
 *  \return      TRUE                  The debug mode allows buffering.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(boolean, DLT_CODE) Dlt_Hlp_IsBufferingAllowed(uint32 LogChannelIndex);

/**********************************************************************************************************************
  Dlt_Hlp_IsSendingAllowed()
**********************************************************************************************************************/
/*! \brief       Indicates if debug mode and debug event state allows sending.
 *  \details     -
 *  \param[in]   LogChannelIndex       The log channel.
 *  \return      FALSE                 The debug mode does not allow sending.
 *  \return      TRUE                  The debug mode allows sending.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(boolean, DLT_CODE) Dlt_Hlp_IsSendingAllowed(uint32 LogChannelIndex);

/**********************************************************************************************************************
  Dlt_Hlp_ParamInValidRange()
**********************************************************************************************************************/
/*! \brief       Indicates if the given value is in valid range depending on the given option.
 *  \details     -
 *  \param[in]   IsGetter              Specifies whether it is a service to request data or setting data.
 *  \param[in]   NumOfArgs             Number of arguments in payload.
 *  \param[in]   RespLen               Length of complete response message.
 *  \param[out]  Var                   The requested data or the variable to be changed (depends on isGetter).
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_ParamInValidRange(Dlt_OptionType Opt, sint8 Value);

/**********************************************************************************************************************
  Dlt_Hlp_GetApplicationIDIndex()
**********************************************************************************************************************/
/*! \brief       Returns the index of requested application ID.
 *  \details     Searches in the context table for the application ID. Return the index of the application ID.
 *  \param[in]   ApplicationID         The application ID.
 *  \param[out]  ApplicationIDIndex    Pointer to index of application ID.
 *  \return      DLT_OK                The search for the index of application ID was successful.
 *  \return      DLT_NOT_OK            The application ID is not registered.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_GetApplicationIDIndex(
  Dlt_LocalApplicationIDType ApplicationID,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ApplicationIDIndex);

/**********************************************************************************************************************
  Dlt_Hlp_GetContextIDIndex()
**********************************************************************************************************************/
/*! \brief       Returns the index of requested context ID.
 *  \details     Searches in the context table for the couple of application ID and context ID. Return the index of the
 *               context ID.
 *  \param[in]   ApplIdIndx            Index of the application ID.
 *  \param[in]   ContextID             The context ID.
 *  \param[out]  ContextIDIndex        Pointer to index of context ID.
 *  \return      DLT_OK                The search for the index of context ID was successful.
 *  \return      DLT_NOT_OK            The context ID is not registered.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_GetContextIDIndex(
  uint16 ApplIdIndx,
  Dlt_LocalContextIDType ContextID,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex);

/**********************************************************************************************************************
  Dlt_Hlp_SetOptionInOneContext()
**********************************************************************************************************************/
/*! \brief       Sets new state of an option to one context.
 *  \details     Sets new state of an option to one couple of application ID and context ID. Passes the new
 *               state to SWC if supported by SWC.
 *  \param[in]   Option                The option that has to be changed. It can take following values:
 *                                      - DLT_OPT_LOGLEVEL
 *                                      - DLT_OPT_TRACESTATUS
 *  \param[in]   NewState              The state the option has to be changed to.
 *  \param[in]   ApplicationIdx        Index of application ID.
 *  \param[in]   ContextIdx            Index of context ID.
 *  \return      DLT_OK                The state change of option was successful.
 *  \return      DLT_NOT_OK            The option to be changed is invalid.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \spec        requires ApplicationIdx < Dlt_GetMaxNumberOfApplications();
 *               requires ContextIdx < Dlt_GetMaxNumberOfContextsPerApplication();
 *  \endspec
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_SetOptionInOneContext(
  Dlt_OptionType Option,
  uint8 NewState,
  uint16 ApplicationIdx,
  uint16 ContextIdx);

/**********************************************************************************************************************
  Dlt_Hlp_SetOption()
**********************************************************************************************************************/
/*! \brief       Sets new state of an option.
 *  \details     Sets new state of an option of
 *                - either one couple of application ID and context ID
 *                - all context IDs of one application ID or
 *                - all application ID and context ID.
 *  \param[in]   ApplicationID         The application ID which option has to be changed.
 *  \param[in]   ContextID             The context ID which option has to be changed.
 *  \param[in]   Option                The option that has to be changed. It can take following values:
 *                                      - DLT_OPT_LOGLEVEL
 *                                      - DLT_OPT_TRACESTATUS
 *  \param[in]   NewStatus             The state the option has to be changed to.
 *  \return      DLT_OK                The state change of option was successful.
 *  \return      DLT_NOT_OK            The state change of option has failed at least for one context.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_SetOption(
  uint32 ApplicationID,
  uint32 ContextID,
  Dlt_OptionType Option,
  uint8 NewStatus);

/**********************************************************************************************************************
  Dlt_Hlp_IsContextRegistered()
**********************************************************************************************************************/
/*! \brief       Checks whether a context is already registered.
 *  \details     Checks whether the specified triple of application ID, context ID and session ID is already registered
 *               in the context table.
 *  \param[in]   ApplId                The application ID to be searched for in context table.
 *  \param[in]   ContId                The context ID to be searched for in context table.
 *  \param[out]  AppIDIndex            Pointer to index of application ID.
 *  \param[out]  ContextIDIndex        Pointer to index of context ID.
 *  \param[out]  ApplAlreadyReg        Pointer that indicates if application ID is already registered.
 *  \return      TRUE                  Indicates that context is already registered.
 *  \return      FALSE                 Indicates that context is not registered yet.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Hlp_IsContextRegistered(
  Dlt_LocalApplicationIDType ApplId,
  Dlt_LocalContextIDType ContId,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIDIndex,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex,
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) ApplAlreadyReg);

/**********************************************************************************************************************
  Dlt_Hlp_CheckSessionId()
**********************************************************************************************************************/
/*! \brief       Checks whether the session ID fits to registered.
 *  \details     Checks whether the specified triple of application ID, context ID and session ID is already registered
 *               in the context table.
 *  \param[in]   SessId                The session ID to be searched for in context table.
 *  \param[in]   ApplId                The application ID to be searched for in context table.
 *  \param[in]   ContId                The context ID to be searched for in context table.
 *  \param[out]  AppIDIndex            Pointer to index of application ID.
 *  \param[out]  ContextIDIndex        Pointer to index of context ID.
 *  \param[out]  ApplAlreadyReg        Pointer that indicates if application ID is already registered.
 *  \return      TRUE                  Indicates that context is already registered.
 *  \return      FALSE                 Indicates that context is not registered yet.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Hlp_CheckSessionId(
  Dlt_SessionIDType SessId,
  Dlt_LocalApplicationIDType ApplId,
  Dlt_LocalContextIDType ContId,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIDIndex,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex,
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) ApplAlreadyReg);

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dlt_Hlp_GetLengthOfReceivedMsg()
**********************************************************************************************************************/
/*! \brief       Returns the length of the received message.
 *  \details     Reads byte 2 and 3 if first 4 bytes of new message are received. With these bytes the length of the
 *               received message is specified. Returns this length.
 *  \param[in]   BufferIndex           Index of receive buffer.
 *  \return      The length of the received message.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires BufferIndex < DLT_CONTROL_BUFFER_SIZE;
 *  \endspec
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_Hlp_GetLengthOfReceivedMsg(uint32 BufferIndex);

/**********************************************************************************************************************
  Dlt_Hlp_IncrementIndexDependingOnOptionalStandardHeader()
**********************************************************************************************************************/
/*! \brief       Modifies position of received message pointer depending on configuration in standard header.
 *  \details     For each set flag ("WEID", "WTMS" and "WSID") of standard header the pointer position is moved.
 *  \param[out]  Index                 Pointer to received message. Modified depending configuration in standard header.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_IncrementIndexDependingOnOptionalStandardHeader(
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) Index);

/**********************************************************************************************************************
  Dlt_Hlp_WriteFailResponseToControlBuffer()
**********************************************************************************************************************/
/*! \brief       Writes the response (depending on status) to the log channel specific control buffer.
 *  \details     Only 'status' is supported as response parameter.
 *  \param[in]   LogChannelIndex          The index of the log channel.
 *  \param[in]   ResponseStatus           The status to be set in response.
 *  \return      E_OK                     The request succeeded.
 *  \return      E_NOT_OK                 The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_WriteFailResponseToControlBuffer(
  uint32 LogChannelIndex,
  uint8 ResponseStatus);
# endif

/**********************************************************************************************************************
  Dlt_Hlp_ResetToFactoryDefault()
**********************************************************************************************************************/
/*! \brief       Resets configuration of Dlt to default values.
 *  \details     -
 *  \return      E_OK        The configuration was reset successfully.
 *  \return      E_NOT_OK    The reset failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(Std_ReturnType, DLT_CODE) Dlt_Hlp_ResetToFactoryDefault(void);

# if (DLT_NVRAM_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dlt_Hlp_StoreConfiguration()
**********************************************************************************************************************/
/*! \brief       Requests to persist the current configuration of Dlt.
 *  \details     -
 *  \return      E_OK        The configuration was persisted successfully.
 *  \return      E_NOT_OK    The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_NVRAM_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(Std_ReturnType, DLT_CODE) Dlt_Hlp_StoreConfiguration(void);
# endif

/**********************************************************************************************************************
  Dlt_Hlp_SetLogChannelAssignment()
**********************************************************************************************************************/
/*! \brief         Adds or removes the assignment of a Dlt user to a specific log channel.
 *  \details       -
 *  \param[in]     AppId                The application id to identify the Dlt user.
 *  \param[in]     ContextId            The context id to identify the Dlt user.
 *  \param[in]     LogChannelName       Name of the requested log channel.
 *  \param[in]     AddRemoveOp          Indicates if assignment is added or removed. Valid range:
 *                                      DLT_ASSIGN_ADD = 1, DLT_ASSIGN_REMOVE = 2.
 *  \return        E_OK                 The log channel could be successfully added or removed.
 *  \return        E_NOT_OK             Adding or removing failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(Std_ReturnType, Dlt_CODE) Dlt_Hlp_SetLogChannelAssignment(
  uint32 AppId,
  uint32 ContextId,
  uint32 LogChannelName,
  Dlt_AssignmentOperation AddRemoveOp);

/**********************************************************************************************************************
  Dlt_Hlp_GetLogChannelThresholdByName()
**********************************************************************************************************************/
/*! \brief       Gets the threshold and trace status of given log channel name.
 *  \details     Searches for  the given log channel name in list of configured log channels.
 *  \return      E_OK        The request succeeded.
 *  \return      E_NOT_OK    The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available aif (DLT_USE_COMLAYER_ASR == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(Std_ReturnType, Dlt_CODE) Dlt_Hlp_GetLogChannelThresholdByName(
  uint32 RequestedLogChannelId,
  P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_VAR) Threshold,
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) TraceStatus);

/**********************************************************************************************************************
  Dlt_Hlp_GetLogChannelThresholdByIndex()
**********************************************************************************************************************/
/*! \brief       Gets the threshold and trace status of given log channel index.
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, Dlt_CODE) Dlt_Hlp_GetLogChannelThresholdByIndex(
  uint32 RequestedLogChannelIndex,
  P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_VAR) Threshold,
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) TraceStatus);

/**********************************************************************************************************************
  Dlt_Hlp_SetLogChannelThresholdByName()
**********************************************************************************************************************/
/*! \brief       Sets the threshold and trace status of given log channel name.
 *  \details     Searches for the given log channel name in list of configured log channels.
 *  \return      E_OK        The request succeeded.
 *  \return      E_NOT_OK    The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(Std_ReturnType, Dlt_CODE) Dlt_Hlp_SetLogChannelThresholdByName(
  uint32 RequestedLogChannelId,
  Dlt_MessageLogLevelType NewThreshold,
  boolean NewTraceStatus);

/**********************************************************************************************************************
  Dlt_Hlp_SetLogChannelThresholdByIndex()
**********************************************************************************************************************/
/*! \brief       Sets the threshold and trace status of given log channel index.
 *  \details     -
 *  \return      E_OK        The request succeeded.
 *  \return      E_NOT_OK    The request failed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(Std_ReturnType, Dlt_CODE) Dlt_Hlp_SetLogChannelThresholdByIndex(
  uint32 RequestedLogChannelIndex,
  Dlt_MessageLogLevelType NewThreshold,
  boolean NewTraceStatus);

/**********************************************************************************************************************
  Dlt_Hlp_GetLogChannelIndexByLogChannelName()
 **********************************************************************************************************************/
/*! \brief       Returns the index of the given log channel name.
 *  \details     -
 *  \return      E_OK        The log channel name is valid and its index could be found.
 *  \return      E_NOT_OK    The log channel name is not known.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(Std_ReturnType, Dlt_CODE) Dlt_Hlp_GetLogChannelIndexByLogChannelName(
  uint32 LogChannelName,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) LogChannelIndex);

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dlt_Hlp_GetIndexOfExtendedHeaderInReceivedMessage()
**********************************************************************************************************************/
/*! \brief       Returns the index of extended header in received message.
 *  \details     -
 *  \return      The requested index.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_Hlp_GetIndexOfExtendedHeaderInReceivedMessage(void);

/**********************************************************************************************************************
  Dlt_Hlp_GetIndexOfPayloadInReceivedMessage()
**********************************************************************************************************************/
/*! \brief       Returns the index of payload in received message.
 *  \details     This is also the index of payload in the response message.
 *  \return      The requested index.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_Hlp_GetIndexOfPayloadInReceivedMessage(void);

/**********************************************************************************************************************
  Dlt_Hlp_GetIndexOfPayloadAfterServiceIdInReceivedMessage()
**********************************************************************************************************************/
/*! \brief       Returns the index of payload (after the service id) in received message.
 *  \details     -
 *  \return      The requested index.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_Hlp_GetIndexOfPayloadAfterServiceIdInReceivedMessage(void);
# endif

/* ####################################################################################################################
 *     IIIIII    NN      NN    IIIIII    TTTTTTTTTT
 *       II      NNNN    NN      II          TT
 *       II      NNNN    NN      II          TT
 *       II      NN  NN  NN      II          TT
 *       II      NN    NNNN      II          TT
 *       II      NN    NNNN      II          TT
 *     IIIIII    NN      NN    IIIIII        TT
 * ####################################################################################################################
 */

/**********************************************************************************************************************
  Dlt_Init_InitializeDltContext()
**********************************************************************************************************************/
/*! \brief       Initializes the DLT contexts.
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_InitializeDltContext(void);

/**********************************************************************************************************************
  Dlt_Init_RegisterDemAndDetContext()
 **********************************************************************************************************************/
/*! \brief       Registers contexts of Dem and Det.
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_RegisterDemAndDetContext(void);

# if (DLT_OS_SOFTWARE_TRACING == STD_ON)
/**********************************************************************************************************************
  Dlt_Init_RegisterOsContexts()
 **********************************************************************************************************************/
/*! \brief       Registers contexts of OS.
 *  \details     Registers contexts for scheduling, activation, and set event hooks.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON) && (DLT_OS_SOFTWARE_TRACING == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_RegisterOsContexts(void);
# endif

/**********************************************************************************************************************
  Dlt_Init_InitializeLogChannelInfo()
 **********************************************************************************************************************/
/*! \brief       Initializes the log channel info with configured values.
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_InitializeLogChannelInfo(void);

# if (DLT_SWCCONTEXTDESCRIPTOR == STD_ON)
/**********************************************************************************************************************
  Dlt_Init_RegisterDefaultDltContexts()
**********************************************************************************************************************/
/*! \brief       Adds all pre-configured DLT contexts to RAM.
 *  \details     All pre-configured DLT contexts do not need to be registered at runtime, therefore they can start
 *               logging immediately after initialization.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_SWCCONTEXTDESCRIPTOR == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_RegisterDefaultDltContexts(void);
# endif

# if (DLT_NVRAM_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dlt_Init_CheckAndSetPersistedData()
 **********************************************************************************************************************/
/*! \brief       Checks if restored data is valid and overwrites the RAM representations with this data.
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_NVRAM_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_CheckAndSetPersistedData(void);
# endif

/* ####################################################################################################################
 *   TTTTTTTTTT  XX      XX        MM      MM    SSSSSS      GGGGGG
 *       TT        XX  XX          MMMM  MMMM  SS      SS  GG      GG
 *       TT        XX  XX          MM  MM  MM  SS          GG
 *       TT          XX      ====  MM  MM  MM    SSSSSS    GG    GGGG
 *       TT        XX  XX          MM      MM          SS  GG      GG
 *       TT        XX  XX          MM      MM  SS      SS  GG      GG
 *       TT      XX      XX        MM      MM    SSSSSS      GGGGGG G
 * ####################################################################################################################
 */
/**********************************************************************************************************************
  Dlt_TxMsg_Transmit()
**********************************************************************************************************************/
/*! \brief       Builds a DLT frame and puts it to send buffer.
 *  \details     Verifies validity of passed data. Builds complete DLT frame for log and trace messages. Writes frame
 *               to send buffer.
 *  \param[in]   FilterInfo            Filter options.
 *  \param[in]   PayloadLength         Length of payload in bytes.
 *  \param[in]   Payload               User data defined by application.
 *  \return      DLT_E_OK                 The message was successfully written to send buffer.
 *  \return      DLT_E_MSG_TOO_LARGE      The message (standard header + extended header + payload) is too long.
 *  \return      DLT_E_UNKNOWN_SESSION_ID The context is not registered (application ID + context ID + session ID).
 *  \return      DLT_E_NO_BUFFER          The send buffer is overflown.
 *  \return      DLT_E_ERROR      Passed data is invalid.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires Payload != NULL_PTR;
 *               requires FilterInfo != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_Transmit(
  P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_APPL_VAR) FilterInfo,
  uint16 PayloadLength,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) Payload);

/**********************************************************************************************************************
  Dlt_TxMsg_IsMessageValid()
**********************************************************************************************************************/
/*! \brief       Checks validity of data for log and trace messages.
 *  \details     Checks whether the context is registered, message is illegally in verbose mode, message log level is
 *               in valid range and trace status is enabled.
 *  \param[in]   FilterInfo            Filter options.
 *  \param[out]  AppIdIndex            Pointer to index of application ID.
 *  \param[out]  ContextIdIndex        Pointer to index of context ID.
 *  \return      DLT_E_OK                 Data is valid.
 *  \return      DLT_E_UNKNOWN_SESSION_ID The context is not registered yet.
 *  \return      DLT_E_ERROR      Passed data is invalid.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires FilterInfo != NULL_PTR;
 *               requires AppIdIndex != NULL_PTR;
 *               requires ContextIdIndex != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_IsMessageValid(
  P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_APPL_VAR) FilterInfo,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIdIndex,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIdIndex);

/**********************************************************************************************************************
  Dlt_TxMsg_IsMessageValidOnLogChannel()
**********************************************************************************************************************/
/*! \brief       Checks validity of data for log and trace messages against the log channel settings.
 *  \details     Checks whether the message log level is in valid range and trace status is enabled.
 *  \param[in]   LogChannelIndex        The index of the log channel.
 *  \param[in]   FilterInfo             Filter options.
 *  \return      DLT_E_OK               The message is valid on this log channel.
 *  \return      DLT_E_MESSAGE_REJECTED The message is rejected on this log channel.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *               requires FilterInfo != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_IsMessageValidOnLogChannel(
  uint32 LogChannelIndex,
  P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_APPL_VAR) FilterInfo);

/**********************************************************************************************************************
  Dlt_TxMsg_SetStandardHeader()
**********************************************************************************************************************/
/*! \brief       Sets standard header in send message.
 *  \details     Sets the active flags in header type and the according bytes in standard header of the send message.
 *  \param[in]   FilterInfo            Filter options.
 *  \param[out]  Data                  Pointer to the send message.
 *  \param[out]  DataIndex             Pointer to index of send message where standard header ends.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires FilterInfo != NULL_PTR;
 *               requires Data != NULL_PTR;
 *               requires DataIndex != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxMsg_SetStandardHeader(
  P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_APPL_VAR) FilterInfo,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) Data,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) DataIndex);

/**********************************************************************************************************************
  Dlt_TxMsg_SetExtendedHeader()
**********************************************************************************************************************/
/*! \brief       Sets extended header in send message.
 *  \details     Sets Message Info depending on specified message type (in FilterInfo). Adds number of arguments, the
 *               application ID and context ID to send message.
 *  \param[in]   FilterInfo            Filter options.
 *  \param[out]  Data                  Pointer to the send message.
 *  \param[out]  DataIndex             Pointer to index of send message where standard header ends.
 *  \param[in]   AppIdIndex            Index of the application ID.
 *  \param[in]   ConIdIndex            Index of the context ID.
 *  \return      DLT_E_OK              The setting of extended header was successful.
 *  \return      DLT_E_ERROR   The message type was invalid.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires AppIdIndex < Dlt_GetMaxNumberOfApplications();
 *               requires ConIdIndex < Dlt_GetMaxNumberOfContextsPerApplication();
 *               requires FilterInfo != NULL_PTR;
 *               requires Data != NULL_PTR;
 *               requires DataIndex != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_SetExtendedHeader(
  P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_APPL_VAR) FilterInfo,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) Data,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) DataIndex,
  uint16 AppIdIndex,
  uint16 ConIdIndex);

/* ####################################################################################################################
 *   TTTTTTTTTT  XX      XX        BBBBBBBB    UU      UU  FFFFFFFFFF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT          XX      ====  BBBBBBBB    UU      UU    FFFFFF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT      XX      XX        BBBBBBBB      UUUUUU        FF
 * ####################################################################################################################
 */
/**********************************************************************************************************************
  Dlt_TxBuf_WriteToBuf()
**********************************************************************************************************************/
/*! \brief       Writes log/trace message to send buffer.
 *  \details     Writes log/trace message to send buffer and detects send buffer overflow.
 *  \param[in]   LogChannelIndex       Index of the used log channel.
 *  \param[out]  Data                  Pointer to the send message.
 *  \param[in]   DataLength            Index of send message where standard header ends.
 *  \return      DLT_E_OK              The setting of extended header was successful.
 *  \return      DLT_E_NO_BUFFER       The send buffer is overflown.
 *  \return      DLT_E_ERROR           The message type was invalid.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *               requires Data != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxBuf_WriteToBuf(
  uint32 LogChannelIndex,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) Data,
  uint16 DataLength);

/**********************************************************************************************************************
  Dlt_TxBuf_WriteToCtrlBuf()
**********************************************************************************************************************/
/*! \brief       Writes control message to control buffer.
 *  \details     Writes control message to control buffer, stops if buffer overflow occurs.
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \param[in]   Length                Number of bytes to be written.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_WriteToCtrlBuf(uint32 LogChannelIndex, uint16 Length);

/**********************************************************************************************************************
  Dlt_TxBuf_GetLengthOfNextMsg()
**********************************************************************************************************************/
/*! \brief       Returns the length of the first DLT message in the source buffer.
 *  \details     The first DLT message is identified depending on the position of the read index. Returns length 0 if
 *               the DLT message is not completely in the buffer.
 *  \param[in]   SourceBuffer          The buffer containing the DLT messages (log and trace or control messages).
 *  \param[in]   ReadIndex             Current read index of the source buffer.
 *  \param[in]   WriteIndex            Current write index of the source buffer.
 *  \param[in]   bufferSize            Size of the source buffer.
 *  \return      Number of bytes to be transmitted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires SourceBuffer != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_TxBuf_GetLengthOfNextMsg(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceBuffer,
  uint32 ReadIndex,
  uint32 WriteIndex,
  uint32 BufferSize);

/**********************************************************************************************************************
  Dlt_TxBuf_CalculateMessageLength()
**********************************************************************************************************************/
/*! \brief       Calculates the number of bytes to be sent.
 *  \details     Calculates the number of bytes to be sent and limit it to maximum allowed message size. Used for
 *               log/trace and control messages. The length contains only complete messages.
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \param[in]   SourceBuffer          The buffer containing the DLT messages (log and trace or control messages).
 *  \param[in]   ReadIndex             Current read index of the source buffer.
 *  \param[in]   WriteIndex            Current write index of the source buffer.
 *  \param[in]   bufferSize            Size of the source buffer.
 *  \return      Number of bytes to be transmitted.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *               requires SourceBuffer != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint32, DLT_CODE) Dlt_TxBuf_CalculateMessageLength(
  uint32 LogChannelIndex,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceBuffer,
  uint32 ReadIndex,
  uint32 WriteIndex,
  uint32 BufferSize);

/**********************************************************************************************************************
  Dlt_TxBuf_SetReadIndex()
**********************************************************************************************************************/
/*! \brief       Updates the read pointer of control buffer or send buffer.
 *  \details     Updates the read pointer of control buffer or send buffer, depending on the specified buffer.
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \param[in]   setControlBuffer      If true, the control buffer is modified, otherwise the send buffer is modified.
 *  \param[in]   bufferIdx             New position for read pointer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *               requires (setControlBuffer)?(bufferIdx < DLT_CONTROL_BUFFER_SIZE):(bufferIdx < DLT_MESSAGE_BUFFER_SIZE);
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_SetReadIndex(
  uint32 LogChannelIndex,
  boolean setControlBuffer,
  uint32 bufferIdx);

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dlt_TxBuf_PrepareRespInCtrlBuf()
**********************************************************************************************************************/
/*! \brief       Prepares the response control frame.
 *  \details     Writes static data, configuration depending data and received message depending data to response
 *               message.
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \param[in]   PayloadLength         Length of payload (without service id) in control response message.
 *  \return      Index of payload (after service id) in the control response message.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_TxBuf_PrepareRespInCtrlBuf(
  uint32 LogChannelIndex,
  uint16 PayloadLength);

/**********************************************************************************************************************
  Dlt_TxBuf_GetEcuIdAndService()
**********************************************************************************************************************/
/*! \brief       Reads out the ECU ID and service ID from received message.
 *  \details     Reads out the ECU ID and service ID from received message and returns them.
 *  \param[out]  EcuId                 Pointer to the ECU ID specified in received message.
 *  \param[out]  ServiceId             Pointer to the session ID specified in received message.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires EcuId != NULL_PTR;
 *               requires ServiceId != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_GetEcuIdAndService(
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) EcuId,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) ServiceId);
# endif

/**********************************************************************************************************************
  Dlt_TxBuf_WriteBufToTxMsg()
**********************************************************************************************************************/
/*! \brief       Writes content of SourceBuffer to the tx message buffer.
 *  \details     The tx message buffer is blocked until the lower layer read the message completely.
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \param[in]   SourceBuffer          The ECU ID specified in received message.
 *  \param[in]   ReadIndex             Read index of the SourceBuffer.
 *  \param[in]   WriteIndex            Write index of the SourceBuffer.
 *  \param[in]   IsControlBuffer       Indicates if the SourceBuffer is the control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetSizeOfLogChannelDescriptor();
 *               requires SourceBuffer != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_WriteBufToTxMsg(
  uint32 LogChannelIndex,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceBuffer,
  uint32 ReadIndex,
  uint32 WriteIndex,
  boolean IsControlBuffer);

/* ####################################################################################################################
 *     SSSSSS    MM      MM
 *   SS      SS  MMMM  MMMM
 *   SS          MM  MM  MM
 *     SSSSSS    MM  MM  MM
 *           SS  MM      MM
 *   SS      SS  MM      MM
 *     SSSSSS    MM      MM
 * ####################################################################################################################
 */
/**********************************************************************************************************************
  Dlt_Sm_HasAnyBufferContentAndIsAllowedToSend()
**********************************************************************************************************************/
/*! \brief       Indicates if any buffer has data content.
 *  \details     Indicates if the receive buffer, the control send buffer, or the send buffer of the given log channel
 *               contains data.
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \return      FALSE                 No buffer has content.
 *  \return      TRUE                  Any buffer has content.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Sm_HasAnyBufferContentAndIsAllowedToSend(uint32 LogChannelIndex);

/**********************************************************************************************************************
  Dlt_Sm_HasReceiveOrControlBufferContent()
**********************************************************************************************************************/
/*! \brief       Indicates if receive buffer or control buffer has data content.
 *  \details     Indicates if the receive buffer or the control send buffer of the given log channel contains data.
 *               The control buffer is always allowed to be send independent of the debug mode as the Dlt master
 *               requests must be answered as fast as possible.
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \return      FALSE                 No buffer has content.
 *  \return      TRUE                  Receive or control buffer has content.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Sm_HasReceiveOrControlBufferContent(uint32 LogChannelIndex);

/**********************************************************************************************************************
  Dlt_Sm_HasSendBufferContentAndIsAllowedToSend()
**********************************************************************************************************************/
/*! \brief       Indicates if send buffer has data content and it is allowed to send its data.
 *  \details     Indicates if the send buffer of the given log channel contains data and the debug mode (including the
 *               debug event state) allows to actually send its data.
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \return      FALSE                 Send buffer has no content.
 *  \return      TRUE                  Send buffer has content.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Sm_HasSendBufferContentAndIsAllowedToSend(uint32 LogChannelIndex);

/**********************************************************************************************************************
  Dlt_Sm_HandleSendingTransitions()
**********************************************************************************************************************/
/*! \brief       Triggers DLT state changes after sending.
 *  \details     If sending succeeded, and new tx data is available, request to send this data. If sending succeeded,
 *               but no tx data is available, change state to DLT_STATE_WAIT_FOR_TX_DATA.
 *               Changes DLT state from DLT_STATE_SENDING to DLT_STATE_WAIT_FOR_TX_DATA if timeout occurred (sending
 *               failed multiple times) and clear all buffers.
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Sm_HandleSendingTransitions(uint32 LogChannelIndex);

/**********************************************************************************************************************
  Dlt_Sm_StateTransition()
**********************************************************************************************************************/
/*! \brief       Manages state transitions.
 *  \details     Changes DLT states depending on previous state and triggering event. Calls transition function.
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \param[in]   transitionEvent       The transition triggering event.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *               requires transitionEvent < kDltStateMachineEvents;
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_StateTransition(uint32 LogChannelIndex, const Dlt_EventType transitionEvent);

/**********************************************************************************************************************
  Dlt_Sm_DummyFunction()
**********************************************************************************************************************/
/*! \brief       Dummy function.
 *  \details     Function to fill function pointer matrix.
 *  \param[in]   LogChannelIndex      The index of the log channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_DummyFunction(uint32 LogChannelIndex);

/**********************************************************************************************************************
  Dlt_Sm_SendingDecision()
**********************************************************************************************************************/
/*! \brief       Decides which buffer content is sent.
 *  \details     If available it interprets received message and creates response message. If response message exists,
 *               it is sent. Otherwise, if log or trace message is available, send this message.
 *  \param[in]   LogChannelIndex      The index of the log channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_SendingDecision(uint32 LogChannelIndex);

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dlt_Sm_BuildResponseMessage()
**********************************************************************************************************************/
/*! \brief       Builds response messages.
 *  \details     Builds for each received message in receive buffer a response message and puts it in control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_BuildResponseMessage(void);
# endif

/**********************************************************************************************************************
  Dlt_Sm_ClearBuffer()
**********************************************************************************************************************/
/*! \brief       Clears all buffers.
 *  \details     Clears all buffer contents and their read and write pointers.
 *  \param[in]   LogChannelIndex      The index of the log channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_ClearBuffer(uint32 LogChannelIndex);

/**********************************************************************************************************************
  Dlt_Sm_ClearSendBuffer()
**********************************************************************************************************************/
/*! \brief       Clears send buffer.
 *  \details     Clears the send buffer's content and its read and write pointer on the given log channel.
 *  \param[in]   LogChannelIndex      The index of the log channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_ClearSendBuffer(uint32 LogChannelIndex);

/**********************************************************************************************************************
  Dlt_Sm_BufferOverflowNotification()
**********************************************************************************************************************/
/*! \brief       Creates a response control message of service BufferOverflowNotification.
 *  \details     Creates a response control message of service BufferOverflowNotification and writes it to control
 *               buffer of given log channel.
 *  \param[in]   LogChannelIndex      The index of the log channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_BufferOverflowNotification(uint32 LogChannelIndex);

/**********************************************************************************************************************
  Dlt_Sm_GlobalStateTransition()
**********************************************************************************************************************/
/*! \brief       Changes global Dlt state if requested.
 *  \details     Sets new global Dlt state if requested via Dlt_SetState. If transition from DLT_GLOBAL_STATE_ONLINE to
 *               DLT_GLOBAL_STATE_OFFLINE, clear content of all buffers.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Sm_GlobalStateTransition(void);

# if (DLT_SYNC_TIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dlt_Sm_HandleSyncTimeStamp
*********************************************************************************************************************/
/*! \brief       Handles the sync time stamp service.
 *  \details     Sends cyclically the sync time stamp service after threshold elapsed.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_SYNC_TIME_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_HandleSyncTimeStamp(void);

/**********************************************************************************************************************
  Dlt_Sm_SendSyncTimeStamp
*********************************************************************************************************************/
/*! \brief       Creates a response control message of service SyncTimeStamp.
 *  \details     Creates a response control message of service SyncTimeStamp and writes it to control buffer
 *               of given log channel.
 *  \param[in]   LogChannelIndex      The index of the log channel.
 *  \param[in]   SyncTime             The current global sync time.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_SYNC_TIME_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_SendSyncTimeStamp(uint32 LogChannelIndex, StbM_TimeStampType SyncTime);
# endif

/* ####################################################################################################################
 *     SSSSSS    EEEEEEEEEE  RRRRRRRR    VV      VV    IIIIII      CCCCCC    EEEEEEEEEE
 *   SS      SS  EE          RR      RR  VV      VV      II      CC      CC  EE
 *   SS          EE          RR      RR  VV      VV      II      CC          EE
 *     SSSSSS    EEEEEEEE    RRRRRRRR     VV    VV       II      CC          EEEEEEEE
 *           SS  EE          RR    RR      VV  VV        II      CC          EE
 *   SS      SS  EE          RR      RR     VVVV         II      CC      CC  EE
 *     SSSSSS    EEEEEEEEEE  RR      RR      VV        IIIIII      CCCCCC    EEEEEEEEEE
 * ####################################################################################################################
 */
# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dlt_Srv_FinalizeRespToCtrlBuf()
**********************************************************************************************************************/
/*! \brief       Completes response message and writes it to control buffer.
 *  \details     Uses the software version specified in received message.
 *  \param[in]   Option                Specifies the attribute to be requested for change or return value.
 *  \param[in]   IsGetter              Specifies whether it is a service to request data or setting data.
 *  \param[in]   PayloadLen            Length of complete response message.
 *  \param[out]  Var                   Pointer to the requested data or the variable to be changed (depends on isGetter).
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires Var != NULL_PTR;
 *  \endspec
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_FinalizeRespToCtrlBuf(
  Dlt_OptionType Option,
  boolean IsGetter,
  uint16 PayloadLen,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) Var);

/**********************************************************************************************************************
  Dlt_Srv_PrepareBufferOverflowNotificationOfLogChannel()
**********************************************************************************************************************/
/*! \brief       Prepares buffer overflow notification message and writes it to control buffer of given log channel.
 *  \details     -
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_PrepareBufferOverflowNotificationOfLogChannel(uint32 LogChannelIndex);

#  if (DLT_SYNC_TIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
  Dlt_Srv_PrepareSyncTimeStampOfLogChannel()
**********************************************************************************************************************/
/*! \brief       Prepares message sync time stamp and writes it to control buffer of given log channel.
 *  \details     -
 *  \param[in]   LogChannelIndex       The index of the log channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \spec        requires LogChannelIndex < Dlt_GetNumberOfAllValidLogChannels();
 *  \endspec
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON) and (DLT_SYNC_TIME_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_PrepareSyncTimeStampOfLogChannel(uint32 LogChannelIndex);
#  endif

/**********************************************************************************************************************
  Dlt_Srv_ServiceNotSupported()
**********************************************************************************************************************/
/*! \brief       Builds response control message due to request of a not supported service.
 *  \details     Builds response message to send DLT_NOT_SUPPORTED, because the requested service is not supported.
 *               Writes the message to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_ServiceNotSupported(void);

/**********************************************************************************************************************
  Dlt_Srv_DummyService()
**********************************************************************************************************************/
/*! \brief       Dummy service.
 *  \details     Called for requested services that are invalid. Builds response control message with DLT_NOT_SUPPORTED
 *               and writes message to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_DummyService(void); /* 0x0000 */

/**********************************************************************************************************************
  Dlt_Srv_SetStatus()
**********************************************************************************************************************/
/*! \brief       Sets the log level or trace status of one registered context.
 *  \details     Sets the log level or trace status of one registered context. If new state is -1, the default value is
 *               assigned to contexts log level or trace status. Response message indicates if value was set
 *               successfully.
 *  \param[in]   Option                Is set to DLT_OPT_LOGLEVEL or DLT_OPT_TRACESTATUS.
 *  \param[in]   DefaultValue          Default value of log level or trace status.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetStatus(Dlt_OptionType Option, sint8 DefaultValue);

/**********************************************************************************************************************
  Dlt_Srv_SetLogLevel()
**********************************************************************************************************************/
/*! \brief       Sets the log level of one registered context.
 *  \details     If new state is -1, the default value is assigned to contexts log level. Response message indicates
 *               if value was set successfully.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetLogLevel(void); /* 0x0001 */

/**********************************************************************************************************************
  Dlt_Srv_SetTraceStatus()
**********************************************************************************************************************/
/*! \brief       Sets the trace status of one registered context.
 *  \details     If new state is -1, the default value is assigned to contexts trace status. Response message indicates
 *               if value was set successfully.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetTraceStatus(void); /* 0x0002 */

/**********************************************************************************************************************
  Dlt_Srv_GetLogInfo()
**********************************************************************************************************************/
/*! \brief       Builds response control message containing the requested log info.
 *  \details     Response contains number of applications, application id, number of contexts, context id, log level,
 *               trace status, length of context description, and length of application description.
 *               Limitations: 1. No descriptions are supported, therefore length of context and application
 *                               descriptions are always 0.
 *                            2. Wildcards for application and context ids are not supported. Therefore, number of
 *                               applications and contexts are always 1.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLogInfo(void); /* 0x0003 */

/**********************************************************************************************************************
  Dlt_Srv_SetVerboseMode()
**********************************************************************************************************************/
/*! \brief       Indicates that service is not supported.
 *  \details     Builds response message to indicate that service SetVerboseMode is not supported.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetVerboseMode(void); /* 0x0009 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_GetVerboseModeStatus()
**********************************************************************************************************************/
/*! \brief       Indicates that service is not supported.
 *  \details     Builds response message to indicate that service GetVerboseModeStatus is not supported.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetVerboseModeStatus(void); /* 0x0019 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_SetDefaultLogLevel()
**********************************************************************************************************************/
/*! \brief       Defines the global log level threshold for sending log messages.
 *  \details     Sets the global default log level Dlt_DefaultMaxLogLevel. Builds response message and writes it to
 *               control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetDefaultLogLevel(void); /* 0x0011 */

/**********************************************************************************************************************
  Dlt_Srv_GetDefaultLogLevel()
**********************************************************************************************************************/
/*! \brief       Builds response control message containing global default log level.
 *  \details     Builds response control message containing global default log level Dlt_DefaultMaxLogLevel and writes
 *               it to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetDefaultLogLevel(void); /* 0x0004 */

/**********************************************************************************************************************
  Dlt_Srv_SetDefaultTraceStatus()
**********************************************************************************************************************/
/*! \brief       Defines if sending trace messages is globally enabled/disabled.
 *  \details     Sets/clears the global default trace status Dlt_DefaultTraceStatus. Builds response message and writes it to
 *               control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetDefaultTraceStatus(void); /* 0x0012 */

/**********************************************************************************************************************
  Dlt_Srv_GetDefaultTraceStatus()
**********************************************************************************************************************/
/*! \brief       Requests the global default trace status.
 *  \details     Builds response control message containing global default trace status Dlt_DefaultTraceStatus and writes
 *               it to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetDefaultTraceStatus(void); /* 0x0015 */

/**********************************************************************************************************************
  Dlt_Srv_SetUseECUID()
**********************************************************************************************************************/
/*! \brief       Defines if ECU ID has to be send within log and trace messages.
 *  \details     Sets/clears the global flag Dlt_HeaderUseEcuId. Builds response message and writes it to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetUseECUID(void); /* 0x000D */

/**********************************************************************************************************************
  Dlt_Srv_GetUseECUID()
**********************************************************************************************************************/
/*! \brief       Requests the global flag status of Dlt_HeaderUseEcuId.
 *  \details     Builds response message containing the global flag status of Dlt_HeaderUseEcuId. Writes the message to
 *               control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseECUID(void); /* 0x001B */

/**********************************************************************************************************************
  Dlt_Srv_SetUseSessionID()
**********************************************************************************************************************/
/*! \brief       Defines if session ID has to be send within log and trace messages.
 *  \details     Sets/clears the global flag Dlt_HeaderUseSessionId. Builds response message and writes it to control
 *               buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetUseSessionID(void); /* 0x000E */

/**********************************************************************************************************************
  Dlt_Srv_GetUseSessionID()
**********************************************************************************************************************/
/*! \brief       Requests the global flag status of Dlt_HeaderUseSessionId.
 *  \details     Builds response message containing the global flag status of Dlt_HeaderUseSessionId. Writes the message
 *               to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseSessionID(void); /* 0x001C */

/**********************************************************************************************************************
  Dlt_Srv_UseTimestamp()
**********************************************************************************************************************/
/*! \brief       Defines if timestamp has to be send within log and trace messages.
 *  \details     Sets/clears the global flag Dlt_HeaderUseTimestamp. Builds response message and writes it to control
 *               buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_UseTimestamp(void); /* 0x000F */

/**********************************************************************************************************************
  Dlt_Srv_GetUseTimestamp()
**********************************************************************************************************************/
/*! \brief       Requests the global flag status of Dlt_HeaderUseTimestamp.
 *  \details     Builds response message containing the global flag status of Dlt_HeaderUseTimestamp. Writes the message
 *               to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseTimestamp(void); /* 0x001D */

/**********************************************************************************************************************
  Dlt_Srv_SetMessageFiltering()
**********************************************************************************************************************/
/*! \brief       Defines if log and trace messages are filtered within DLT.
 *  \details     Sets/clears the global flag Dlt_FilterMessages. Builds response message and writes it to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetMessageFiltering(void); /* 0x000A */

/**********************************************************************************************************************
  Dlt_Srv_GetMessageFilteringStatus()
**********************************************************************************************************************/
/*! \brief       Requests the global flag status of Dlt_FilterMessages.
 *  \details     Builds response message containing the global flag status of Dlt_FilterMessages. Writes the message to
 *               control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetMessageFilteringStatus(void); /* 0x001A */

/**********************************************************************************************************************
  Dlt_Srv_UseExtendedHeader()
**********************************************************************************************************************/
/*! \brief       Indicates that service is not supported.
 *  \details     Builds response message to indicate that service UseExtendedHeader is not supported.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_UseExtendedHeader(void); /* 0x0010 */

/**********************************************************************************************************************
  Dlt_Srv_GetUseExtendedHeader()
**********************************************************************************************************************/
/*! \brief       Builds response message to indicate that service GetUseExtendedHeader is not supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseExtendedHeader(void); /* 0x001E */

/**********************************************************************************************************************
  Dlt_Srv_GetTraceStatus()
**********************************************************************************************************************/
/*! \brief       Builds response message containing the requested trace status.
 *  \details     If the received/requested Dlt user is registered, its trace status is responded.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetTraceStatus(void); /* 0x001F */

/**********************************************************************************************************************
  Dlt_Srv_SetTimingPackets()
**********************************************************************************************************************/
/*! \brief       Builds response message to indicate that service SetTimingPackets is not supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetTimingPackets(void); /* 0x000B - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_GetLocalTime()
**********************************************************************************************************************/
/*! \brief       Builds response message to indicate that service GetLocalTime is not supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLocalTime(void); /* 0x000C */

/**********************************************************************************************************************
  Dlt_Srv_SetComInterfaceStatus()
**********************************************************************************************************************/
/*! \brief       Builds response message to indicate that service SetComInterfaceStatus is not supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetComInterfaceStatus(void); /* 0x0007 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_GetComInterfaceStatus()
**********************************************************************************************************************/
/*! \brief       Builds response message to indicate that service GetComInterfaceStatus is not supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetComInterfaceStatus(void); /* 0x0016 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_SetComInterfaceMaxBandwidth()
**********************************************************************************************************************/
/*! \brief       Builds response message to indicate that service SetComInterfaceMaxBandwidth is not supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetComInterfaceMaxBandwidth(void); /* 0x0008 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_GetComInterfaceMaxBandwidth()
**********************************************************************************************************************/
/*! \brief       Builds response message to indicate that service GetComInterfaceMaxBandwidth is not supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetComInterfaceMaxBandwidth(void); /* 0x0018 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_GetLogChannelNames()
**********************************************************************************************************************/
/*! \brief       Builds response message containing all log channel names.
 *  \details     Builds response control message containing the number of configured log channels and their names.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLogChannelNames(void); /* 0x0017 */

/**********************************************************************************************************************
  Dlt_Srv_StoreConfig()
**********************************************************************************************************************/
/*! \brief       Builds response message to indicate if persistency request succeeded.
 *  \details     Requests to persist current configuration.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_StoreConfig(void); /* 0x0005 */

/**********************************************************************************************************************
  Dlt_Srv_ResetToFactoryDefault()
**********************************************************************************************************************/
/*! \brief       Resets all configurations done in runtime.
 *  \details     Resets global flags and complete context table (all registered contexts remain, but get initial
 *               configuration). Builds response message and copies it to control message.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_ResetToFactoryDefault(void); /* 0x0006 */

/**********************************************************************************************************************
  Dlt_Srv_GetSoftwareVersion()
**********************************************************************************************************************/
/*! \brief       Requests the software version of DLT.
 *  \details     Builds response control message containing the software version of DLT within the payload. The endianness
 *               depends on received message. The response message is written to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetSoftwareVersion(void); /* 0x0013 */

/**********************************************************************************************************************
  Dlt_Srv_MessageBufferOverflow()
**********************************************************************************************************************/
/*! \brief       Builds response message to indicate that service MessageBufferOverflow is not supported.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_MessageBufferOverflow(void); /* 0x0014 */

/**********************************************************************************************************************
  Dlt_Srv_SetLogChannelAssignment()
**********************************************************************************************************************/
/*! \brief       Requests to add or remove a assignment of log channel to a Dlt user.
 *  \details     Builds response control message indicating if assignment succeeded.
 *               The response message is written to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetLogChannelAssignment(void); /* 0x0020 */

/**********************************************************************************************************************
  Dlt_Srv_SetLogChannelThreshold()
 **********************************************************************************************************************/
/*! \brief       Requests to set the threshold and the trace status of a log channel.
 *  \details     Builds response control message indicating if setting the new values succeeded.
 *               The response message is written to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetLogChannelThreshold(void); /* 0x0021 */

/**********************************************************************************************************************
  Dlt_Srv_GetLogChannelThreshold()
 **********************************************************************************************************************/
/*! \brief       Requests the current threshold and trace status of a log channel.
 *  \details     Builds response control message containing the threshold and the traces status of a log channel.
 *               The response message is written to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLogChannelThreshold(void); /* 0x0022 */

/**********************************************************************************************************************
  Dlt_Srv_BufferOverflowNotification()
**********************************************************************************************************************/
/*! \brief       Requests the current buffer overflow notification count of all log channels.
 *  \details     Builds response control message containing the counter of the Dlt_BufferOverflowNotificationStatus.
 *               The response message is written to control buffer.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_BufferOverflowNotification(void); /* 0x0023 - Not supported */

/**********************************************************************************************************************
  Dlt_Srv_SyncTimeStamp()
**********************************************************************************************************************/
/*! \brief       Requests the current synchronized absolute time of a log channel.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON).
 **********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SyncTimeStamp(void); /* 0x0024 - Not supported */

#  if (DLT_INJECTION_CALL_SUPPORT == STD_ON)
/**********************************************************************************************************************
 Dlt_Srv_CallSwcInjection()
**********************************************************************************************************************/
/*! \brief       Requests to forward an application specific service to application.
 *  \details     Builds response control message containing the return value.
 *               The response message is written to control buffer.
 *  \param[in]   ServiceId       The service to be injected. Valid range is 0xFFF - 0xFFFFFFFF.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_RX_DATA_PATH_SUPPORT == STD_ON) and (DLT_INJECTION_CALL_SUPPORT == STD_ON).
**********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_CallSwcInjection(uint32 ServiceId); /* 0x0FFF - 0xFFFFFFFF */
#  endif
# endif /* (DLT_RX_DATA_PATH_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 *  Dlt_RegCont_RegisterNewContext()
**********************************************************************************************************************/
/*! \brief       Registers the new DLT user.
 *  \details     The DLT user is specified by the couple of application id and context id. After registration, this
 *               DLT user may send log and trace messages.
 *  \param[in]   SessionId             The session id (usually set by RTE) to identify the SWC.
 *  \param[in]   AppId                 The Application id indentifies a DLT user (in combination with the Context id).
 *  \param[in]   ContextId             The Context id indentifies a DLT user (in combination with the Application id).
 *  \param[in]   AppIdIndex            The index of the application id.
 *  \param[in]   ConIdIndex            The index of the context id.
 *  \param[in]   applicationAlreadyRegistered  Pointer to the context description.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_RegCont_RegisterNewContext(
  Dlt_SessionIDType SessionId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextId,
  uint16 AppIdIndex,
  uint16 ConIdIndex,
  boolean applicationAlreadyRegistered);

/**********************************************************************************************************************
 *  Dlt_Trace_SendTraceMessage()
**********************************************************************************************************************/
/*! \brief       Builds the trace message and buffers it.
 *  \details     -
 *  \param[in]   SessionId             The session id (usually set by RTE) to identify the SWC.
 *  \param[in]   TraceInfo             The trace info.
 *  \param[in]   TraceData             The trace data (the payload).
 *  \param[in]   TraceDataLength       The trace data length (the payload length).
 *  \param[in]   UsedEndianness        The used endianness (big, little, or as platform)
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      This function is only available if (DLT_USE_COMLAYER_ASR = STD_ON).
 *********************************************************************************************************************/
DLT_LOCAL FUNC(Std_ReturnType, DLT_CODE) Dlt_Trace_SendTraceMessage(
  Dlt_SessionIDType SessionId,
  P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_APPL_VAR) TraceInfo,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) TraceData,
  uint16 TraceDataLength,
  Dlt_UsedEndiannessType UsedEndianness);
#endif /* #if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/* ####################################################################################################################
 *   HH      HH  EEEEEEEEEE  LL          PPPPPPPP    EEEEEEEEEE  RRRRRRRR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HH      HH  EE          LL          PP      PP  EE          RR      RR
 *   HHHHHHHHHH  EEEEEEEE    LL          PPPPPPPP    EEEEEEEE    RRRRRRRR
 *   HH      HH  EE          LL          PP          EE          RR    RR
 *   HH      HH  EE          LL          PP          EE          RR      RR
 *   HH      HH  EEEEEEEEEE  LLLLLLLLLL  PP          EEEEEEEEEE  RR      RR
 * ####################################################################################################################
 */
#if ((DLT_USE_COMLAYER_XCP == STD_ON) || (DLT_RX_DATA_PATH_SUPPORT == STD_ON)) /* COV_DLT_FILTER_ASR_XCP */
/**********************************************************************************************************************
 * Dlt_Hlp_CopyArrayToLongLittleEndian_AndMoveIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongLittleEndian_AndMoveIndex(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) StartIndex)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Copy source data from little endian array to destination 32bit variable. */
  (*DestData)  = (uint32)((uint32)(SourceData[StartIndex[0] + 3u]) << DLT_SHIFT_24BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0] + 2u]) << DLT_SHIFT_16BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0] + 1u]) << DLT_SHIFT_8BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0]]));

  /* #20 Set array index to new position. */
  *StartIndex = StartIndex[0] + DLT_TYPE_LONG_LENGTH;
} /* Dlt_Hlp_CopyArrayToLongLittleEndian_AndMoveIndex */
#endif

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON) || (DLT_OS_SOFTWARE_TRACING == STD_ON)
/**********************************************************************************************************************
 * Dlt_Hlp_CopyArrayToLongBigEndian
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongBigEndian(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Copy source data from big endian array to destination 32bit variable. */
  (*DestData)  = (uint32)((uint32)(SourceData[0]) << DLT_SHIFT_24BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[1]) << DLT_SHIFT_16BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[2]) << DLT_SHIFT_8BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[3]));
} /* Dlt_Hlp_CopyArrayToLongBigEndian */
# endif

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) StartIndex)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Copy source data from big endian array to destination 32bit variable. */
  (*DestData)  = (uint32)((uint32)(SourceData[StartIndex[0]]) << DLT_SHIFT_24BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0] + 1u]) << DLT_SHIFT_16BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0] + 2u]) << DLT_SHIFT_8BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[StartIndex[0] + 3u]));

  /* #20 Set array index to new position. */
  *StartIndex = StartIndex[0] + DLT_TYPE_LONG_LENGTH;
} /* Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex */

#  if (DLT_INJECTION_CALL_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Hlp_CopyArrayToLongLittleEndian
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyArrayToLongLittleEndian(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Copy source data from little endian array to destination 32bit variable. */
  (*DestData)  = (uint32)((uint32)(SourceData[3]) << DLT_SHIFT_24BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[2]) << DLT_SHIFT_16BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[1]) << DLT_SHIFT_8BIT);
  (*DestData) |= (uint32)((uint32)(SourceData[0]));
} /* Dlt_Hlp_CopyArrayToLongLittleEndian */
#  endif
# endif

# if ((DLT_RX_DATA_PATH_SUPPORT == STD_ON) || (DLT_HEADER_PAYLOAD_BYTEORDER == DLT_LITTLEENDIAN))
/**********************************************************************************************************************
 * Dlt_Hlp_CopyLongToArrayLittleEndian_AndMoveIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyLongToArrayLittleEndian_AndMoveIndex(
  uint32 SourceData,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Copy source data from 32bit variable to destination little endian array.  */
  DestData[0] = (uint8)(SourceData);
  DestData[1] = (uint8)(SourceData >> DLT_SHIFT_8BIT);
  DestData[2] = (uint8)(SourceData >> DLT_SHIFT_16BIT);
  DestData[3] = (uint8)(SourceData >> DLT_SHIFT_24BIT);

  /* #20 Set array index to new position. */
  *index = index[0] + DLT_TYPE_LONG_LENGTH;
} /* Dlt_Hlp_CopyLongToArrayLittleEndian_AndMoveIndex */

/**********************************************************************************************************************
 * Dlt_Hlp_CopyIntToArrayLittleEndian_AndMoveIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyIntToArrayLittleEndian_AndMoveIndex(
  uint16 SourceData,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Copy source data from 16bit variable to destination little endian array.  */
  DestData[0] = (uint8)(SourceData);
  DestData[1] = (uint8)(SourceData >> DLT_SHIFT_8BIT);

  /* #20 Set array index to new position. */
  *index = index[0] + 2u;
} /* Dlt_Hlp_CopyIntToArrayLittleEndian_AndMoveIndex */
# endif

/**********************************************************************************************************************
 * Dlt_Hlp_CopyStringToLongBigEndian
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyStringToLongBigEndian(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceData,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) DestData)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 myVar = 0u;
  uint8_least i;
  uint8_least j = 3u;
  boolean endOfStringReached = FALSE;  /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Copy source data from big endian array to destination 32bit variable. */
  for (i = 0u; i < DLT_TYPE_LONG_LENGTH; i++)
  {
    if (SourceData[i] == 0u)
    {
      endOfStringReached = TRUE;
    }

    if (endOfStringReached == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
    {
      break;
    }
    else
    {
      myVar |= (uint32)((uint32)(SourceData[i]) << (j * DLT_SHIFT_8BIT));
    }

    j--;
  }

  *DestData = myVar;
} /* Dlt_Hlp_CopyStringToLongBigEndian */

/**********************************************************************************************************************
 * Dlt_Hlp_CopyLongToArrayBigEndian
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyLongToArrayBigEndian(
  uint32 SourceData,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Copy source data from 32bit variable to destination big endian array.  */
  DestData[0] = (uint8)(SourceData >> DLT_SHIFT_24BIT);
  DestData[1] = (uint8)(SourceData >> DLT_SHIFT_16BIT);
  DestData[2] = (uint8)(SourceData >> DLT_SHIFT_8BIT);
  DestData[3] = (uint8)(SourceData);
} /* Dlt_Hlp_CopyLongToArrayBigEndian */

/**********************************************************************************************************************
 * Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(
  uint32 SourceData,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Copy source data from 32bit variable to destination big endian array.  */
  DestData[0] = (uint8)(SourceData >> DLT_SHIFT_24BIT);
  DestData[1] = (uint8)(SourceData >> DLT_SHIFT_16BIT);
  DestData[2] = (uint8)(SourceData >> DLT_SHIFT_8BIT);
  DestData[3] = (uint8)(SourceData);

  /* #20 Set array index to new position. */
  *index = index[0] + DLT_TYPE_LONG_LENGTH;
} /* Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex */

/**********************************************************************************************************************
 * Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex(
  uint16 SourceData,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) DestData,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) index)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Copy source data from 16bit variable to destination big endian array.  */
  DestData[0] = (uint8)(SourceData >> DLT_SHIFT_8BIT);
  DestData[1] = (uint8)(SourceData);

  /* #20 Set array index to new position. */
  *index = index[0] + 2u;
} /* Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex */

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Hlp_DecToAscii
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Hlp_DecToAscii(
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) AsciiValue,
  uint8 DecValue,
  boolean IsBigEndian)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 firstLetter = (DecValue % 10u);
  uint8 secondLetter = (DecValue / 10u);

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Depending on endianness, the decimal value is split to two ASCII letters and written to target buffer. */
  if (IsBigEndian == TRUE)
  {
    AsciiValue[0] = firstLetter + 0x30u;
    AsciiValue[1] = secondLetter + 0x30u;
  }
  else
  {
    AsciiValue[0] = secondLetter + 0x30u;
    AsciiValue[1] = firstLetter + 0x30u;
  }
} /* Dlt_Hlp_DecToAscii */
# endif

/**********************************************************************************************************************
 * Dlt_Hlp_IsDebugModeUnchanged
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(boolean, DLT_CODE) Dlt_Hlp_IsDebugModeUnchanged(uint32 LogChannelIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean isDebugModeUnchanged = FALSE;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Indicate if no debug mode change is requested. */
  if (Dlt_DebugMode[LogChannelIndex] == Dlt_DebugModeRequested[LogChannelIndex])
  {
    isDebugModeUnchanged = TRUE;
  }

  return isDebugModeUnchanged;
} /* Dlt_Hlp_IsDebugModeUnchanged */

/**********************************************************************************************************************
 * Dlt_Hlp_IsBufferingAllowed
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(boolean, DLT_CODE) Dlt_Hlp_IsBufferingAllowed(uint32 LogChannelIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean isBufferingAllowed = FALSE;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If no debug mode change is requested:
   *       And the debug mode is continuous OR
   *       The debug mode is event based and the debug event state allows buffering:
   *         Indicate buffering is allowed.
   *     Otherwise, indicate buffering is not allowed. */
  if ((Dlt_Hlp_IsDebugModeUnchanged(LogChannelIndex) == TRUE)
   && ((Dlt_DebugMode[LogChannelIndex] == DLT_DEBUGMODE_CONTINUOUS_FIRSTISBEST)
    || ((Dlt_DebugMode[LogChannelIndex] == DLT_DEBUGMODE_ONEVENT_FIRSTISBEST)
     && (Dlt_DebugEventState[LogChannelIndex] == DLT_DEBUGEVENT_BUFFERING_ACTIVE))))
  {
    isBufferingAllowed = TRUE;
  }

  return isBufferingAllowed;
} /* Dlt_Hlp_IsBufferingAllowed */

/**********************************************************************************************************************
 * Dlt_Hlp_IsSendingAllowed
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(boolean, DLT_CODE) Dlt_Hlp_IsSendingAllowed(uint32 LogChannelIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean isSendingAllowed = FALSE;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If no debug mode change is requested:
   *       And the debug mode is continuous OR
   *       The debug mode is event based and the debug event state allows sending:
   *         Indicate sending is allowed.
   *     Otherwise, indicate sending is not allowed. */
  if ((Dlt_Hlp_IsDebugModeUnchanged(LogChannelIndex) == TRUE)
   && ((Dlt_DebugMode[LogChannelIndex] == DLT_DEBUGMODE_CONTINUOUS_FIRSTISBEST)
    || ((Dlt_DebugMode[LogChannelIndex] == DLT_DEBUGMODE_ONEVENT_FIRSTISBEST)
     && (Dlt_DebugEventState[LogChannelIndex] == DLT_DEBUGEVENT_SENDING_ACTIVE))))
  {
    isSendingAllowed = TRUE;
  }

  return isSendingAllowed;
} /* Dlt_Hlp_IsSendingAllowed */

/**********************************************************************************************************************
 * Dlt_Hlp_ParamInValidRange
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_ParamInValidRange(
  Dlt_OptionType Opt,
  sint8 Value)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = DLT_E_NO_ERROR;
  uint8 retVal = DLT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If log level has to be changed:
   *       Check new value to be a valid log level.
   *     Otherwise, if trace status, verbose mode or another boolean value has to be changed:
   *       Check new value to be a valid boolean value.
   *     Otherwise:
   *       The specified option is invalid.*/
  switch(Opt)
  {
  case(DLT_OPT_LOGLEVEL):
    if ((Value < (sint8)DLT_LOG_OFF) ||
        (Value > (sint8)DLT_LOG_VERBOSE))
    {
      errorId = DLT_E_PARAM;
      retVal = DLT_ERROR;
    }
    break;

  case(DLT_OPT_TRACESTATUS):
  case(DLT_OPT_BOOLEAN):          /* PRQA S 2995, 2996 2 */ /* MD_Dlt_AlwaysTrue, MD_Dlt_AlwaysFalse */
    if ((Value < (sint8)FALSE) || /* PRQA S 4303 1 */ /* MD_MSR_AutosarBoolean */
        (Value > (sint8)TRUE))
    {
      errorId = DLT_E_PARAM;
      retVal = DLT_ERROR;
    }
    break;

  default: /* COV_DLT_DEFAULT_BRANCH */
      errorId = DLT_E_PARAM;
      retVal = DLT_ERROR;
    break;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_MAINFUNCTION, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_Hlp_ParamInValidRange */

/**********************************************************************************************************************
 * Dlt_Hlp_SetOptionInOneContext
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_SetOptionInOneContext(
  Dlt_OptionType Option,
  uint8 NewState,
  uint16 ApplicationIdx,
  uint16 ContextIdx)
{
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ApplicationIDType appId;
  Dlt_ContextIDType contextId;
  uint8 retVal = (uint8)DLT_OK;

  Dlt_SetLogLevelOpFuncType setLogLevelFct = Dlt_Context.appIds[ApplicationIdx].contextInfoList[ContextIdx].cbkInfo.Dlt_SetLogLevel_;
  Dlt_SetTraceStatusOpFuncType setTraceStatusFct = Dlt_Context.appIds[ApplicationIdx].contextInfoList[ContextIdx].cbkInfo.Dlt_SetTraceStatus_;

  /* ----- Implementation ------------------------------------------------- */
  Dlt_Hlp_CopyLongToArrayBigEndian(Dlt_Context.appIds[ApplicationIdx].appId, appId);
  Dlt_Hlp_CopyLongToArrayBigEndian(Dlt_Context.appIds[ApplicationIdx].contextInfoList[ContextIdx].contextId, contextId);

  /* #10 If option is set to log level, trace status, or verbose mode.
   *       Change state of specified option in one couple of application ID and context ID in context table.
   *       If call back function is available, change the state also in SWC.
   */
  switch(Option)
  {
  case (DLT_OPT_LOGLEVEL):
    Dlt_Context.appIds[ApplicationIdx].contextInfoList[ContextIdx].logLevel = (sint8)NewState;

    /*@ assert setLogLevelFct == Dlt_SetLogLevelOpClass[$range(0, DLT_NUMBER_OF_SWCS-1)]; */ /* VCA_DLT_VALID_LOG_LVL_CHG_NOTIFICATION_FCT */
    if (setLogLevelFct != NULL_PTR) /* COV_DLT_LOG_CNTX_FCT */
    {
      (void)setLogLevelFct(
                appId,
                contextId,
                (Dlt_MessageLogLevelType)NewState);
    }
    break;

  case (DLT_OPT_TRACESTATUS):
    Dlt_Context.appIds[ApplicationIdx].contextInfoList[ContextIdx].traceStatus = (sint8)NewState;

    /*@ assert setTraceStatusFct == Dlt_SetTraceStatusOpClass[$range(0, DLT_NUMBER_OF_SWCS-1)]; */ /* VCA_DLT_VALID_TRACE_STATUS_FCT */
    if (setTraceStatusFct != NULL_PTR) /* COV_DLT_LOG_CNTX_FCT */
    {
      (void)setTraceStatusFct(
                appId,
                contextId,
                (boolean)NewState); /* PRQA S 1881, 4340 */ /* MD_MSR_AutosarBoolean */
    }
    break;

  /* #20 Otherwise:
   *       The specified option is invalid.
   */
  default: /* COV_DLT_DEFAULT_BRANCH */
    retVal = (uint8)DLT_NOT_OK;
    break;
  }

  return retVal;
} /* Dlt_Hlp_SetOptionInOneContext */

/**********************************************************************************************************************
 * Dlt_Hlp_GetApplicationIDIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_GetApplicationIDIndex(
  Dlt_LocalApplicationIDType ApplicationID,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ApplicationIDIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 retVal = (uint8)DLT_NOT_OK;
  uint16 i;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Iterate over all registered application IDs:
   *       If the application ID is registered, stop searching and return its index.
   */
  for (i = 0u; (i < Dlt_Context.appIdCount) && (retVal == (uint8)DLT_NOT_OK); i++)
  {
    if (Dlt_Context.appIds[i].appId == ApplicationID)
    {
      *ApplicationIDIndex = i;
      retVal = (uint8)DLT_OK;
    }
  }

  return retVal;
} /* Dlt_Hlp_GetApplicationIDIndex */

/**********************************************************************************************************************
 * Dlt_Hlp_GetContextIDIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_GetContextIDIndex(
  uint16 ApplIdIndx,
  Dlt_LocalContextIDType ContextID,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 retVal = (uint8)DLT_NOT_OK;
  uint16 i;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Iterate over all registered context IDs:
   *       If the context ID is registered, stop searching and return its index.
   */
  for (i = 0u; (i < Dlt_Context.appIds[ApplIdIndx].contextIdCount) && (retVal == (uint8)DLT_NOT_OK); i++)
  {
    if (Dlt_Context.appIds[ApplIdIndx].contextInfoList[i].contextId == ContextID)
    {
      *ContextIDIndex = i;
      retVal = (uint8)DLT_OK;
    }
  }

  return retVal;
} /* Dlt_Hlp_GetContextIDIndex */

/**********************************************************************************************************************
 * Dlt_Hlp_SetOption
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(uint8, DLT_CODE) Dlt_Hlp_SetOption(
  uint32 ApplicationID,
  uint32 ContextID,
  Dlt_OptionType Option,
  uint8 NewStatus)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 applIDIndex = 0u;
  uint16 contextIDIndex = 0u;
  uint16 i;
  uint16 j;
  uint8 locStatus = (uint8)DLT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If no application ID is specified, the option change applies to all application IDs.
   *     Otherwise, if no context ID is specified, the option change applies to all registered contexts of one application ID.
   *     Otherwise, the option change applies to exactly one context.
   *     For the given application and context IDs:
   *       set the option to new state.
   */
  if (ApplicationID == 0u)
  {
    for (i = 0u; (i < Dlt_Context.appIdCount); i++)
    {
      for (j = 0u; (j < Dlt_Context.appIds[i].contextIdCount); j++)
      {
        /*@ assert i < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
        /*@ assert j < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

        locStatus |= Dlt_Hlp_SetOptionInOneContext(Option, NewStatus, i, j);
      }
    }
  }
  else if (ContextID == 0u)
  {
    locStatus = Dlt_Hlp_GetApplicationIDIndex(ApplicationID, &applIDIndex);

    if (locStatus == (uint8)DLT_OK)
    {
      for (i = 0u; (i < Dlt_Context.appIds[applIDIndex].contextIdCount); i++)
      {
        /*@ assert applIDIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
        /*@ assert i < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

        locStatus |= Dlt_Hlp_SetOptionInOneContext(Option, NewStatus, applIDIndex, i);
      }
    }
  }
  else
  {
    locStatus = Dlt_Hlp_GetApplicationIDIndex(ApplicationID, &applIDIndex);
    /*@ assert applIDIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */

    if (locStatus == (uint8)DLT_OK) /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
    {
      locStatus = Dlt_Hlp_GetContextIDIndex(applIDIndex, ContextID, &contextIDIndex);

      /*@ assert contextIDIndex < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */
    }
    if (locStatus == (uint8)DLT_OK) /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
    {
      locStatus = Dlt_Hlp_SetOptionInOneContext(Option, NewStatus, applIDIndex, contextIDIndex);
    }
  }

  return locStatus;
} /* Dlt_Hlp_SetOption */

/**********************************************************************************************************************
 * Dlt_Hlp_IsContextRegistered
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Hlp_IsContextRegistered(
  Dlt_LocalApplicationIDType ApplId,
  Dlt_LocalContextIDType ContId,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIDIndex,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex,
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) ApplAlreadyReg)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean isContextRegistered = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
  uint8 status;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Look for application ID in context table. */
  status = Dlt_Hlp_GetApplicationIDIndex(ApplId, AppIDIndex);

  /*@ assert *AppIDIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */

  /* #20 If the application ID and context ID are already registered and the specified session ID belongs to them:
   *       The context is already registered.
   */
  if (status == (uint8)DLT_OK)
  {
    *ApplAlreadyReg = TRUE;
    status = Dlt_Hlp_GetContextIDIndex(*AppIDIndex, ContId, ContextIDIndex);

    if (status == (uint8)DLT_OK)
    {
      isContextRegistered = TRUE;
    }
  }
  return isContextRegistered;
} /* Dlt_Hlp_IsContextRegistered */

/**********************************************************************************************************************
 * Dlt_Hlp_CheckSessionId
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Hlp_CheckSessionId(
  Dlt_SessionIDType SessId,
  Dlt_LocalApplicationIDType ApplId,
  Dlt_LocalContextIDType ContId,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIDIndex,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIDIndex,
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) ApplAlreadyReg)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean isSessionIdRegistered = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
  boolean isContextRegistered;

  /* #10 Look for Application ID and Context ID in context table. */
  isContextRegistered = Dlt_Hlp_IsContextRegistered(ApplId, ContId, AppIDIndex, ContextIDIndex, ApplAlreadyReg);

  /* #20 If the specified Session ID belong to the Application ID and the Context ID, the Session is already registered. */
  if ((isContextRegistered == TRUE) && /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
    (Dlt_Context.appIds[*AppIDIndex].contextInfoList[*ContextIDIndex].cbkInfo.sessionId == SessId))
  {
    isSessionIdRegistered = TRUE;
  }

  return isSessionIdRegistered;
} /* Dlt_Hlp_CheckSessionId */

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Hlp_GetLengthOfReceivedMsg
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_Hlp_GetLengthOfReceivedMsg(uint32 BufferIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 lengthOfReceivedMsg;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If the receive buffer contains the first 4 byte of new message, get the complete message length. */
  if (Dlt_RxInfo.CopyRxDataLength == 4u)
  {
    /* #20 Indicate that the received message length was successfully read. */
    Dlt_RxInfo.CopyRxDataCounter++;

    /* #30 Get the message length depending on position of message in receive buffer. */
    if (BufferIndex == 0u)
    {
      lengthOfReceivedMsg  = (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[DLT_CONTROL_BUFFER_SIZE - 2u] << DLT_SHIFT_8BIT);
      lengthOfReceivedMsg |= (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[DLT_CONTROL_BUFFER_SIZE - 1u]);
    }
    else if (BufferIndex == 1u)
    {
      lengthOfReceivedMsg  = (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[DLT_CONTROL_BUFFER_SIZE - 1u] << DLT_SHIFT_8BIT);
      lengthOfReceivedMsg |= (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[0]);
    }
    else
    {
      lengthOfReceivedMsg  = (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[BufferIndex - 2u] << DLT_SHIFT_8BIT);
      lengthOfReceivedMsg |= (uint16)((uint16)Dlt_ReceiveBuffer.Buffer[BufferIndex - 1u]);
    }

    /* #40 Request rest of received message */
    Dlt_RxInfo.CopyRxDataLength = lengthOfReceivedMsg - 4u;
    lengthOfReceivedMsg = (uint16)Dlt_RxInfo.CopyRxDataLength;
  }
  /* #50 Otherwise, request the missing bytes to get the first 4 bytes. */
  else
  {
    lengthOfReceivedMsg = 4u - (uint16)Dlt_RxInfo.CopyRxDataLength;
  }

  return lengthOfReceivedMsg;
} /* Dlt_Hlp_GetLengthOfReceivedMsg */

/**********************************************************************************************************************
 * Dlt_Hlp_IncrementIndexDependingOnOptionalStandardHeader
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_IncrementIndexDependingOnOptionalStandardHeader(
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) Index)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Increase position of received message pointer for following flags set in the received message.
   *       The flags are "With Ecu ID", "With TiMeStamp" and "With Session ID".
   */
  if ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_WEID_Mask) == Dlt_StandardHeaderType_WEID_Mask)
  {
    *Index = Index[0] + DLT_DEFAULT_ID_LENGTH;
  }
  if ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_WTMS_Mask) == Dlt_StandardHeaderType_WTMS_Mask)
  {
    *Index = Index[0] + DLT_DEFAULT_ID_LENGTH;
  }
  if ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_WSID_Mask) == Dlt_StandardHeaderType_WSID_Mask)
  {
    *Index = Index[0] + DLT_DEFAULT_ID_LENGTH;
  }
} /* Dlt_Hlp_IncrementIndexDependingOnOptionalStandardHeader */

/**********************************************************************************************************************
 * Dlt_Hlp_WriteFailResponseToControlBuffer
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Hlp_WriteFailResponseToControlBuffer(
  uint32 LogChannelIndex,
  uint8 ResponseStatus)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 responseLength;
  uint16 indexOfPayloadInReceivedMessage;

  /* ----- Implementation ------------------------------------------------- */
  indexOfPayloadInReceivedMessage = Dlt_TxBuf_PrepareRespInCtrlBuf(
    LogChannelIndex,
    DLT_DEFAULT_RESPONSE_PAYLOAD_LENGTH);
  Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInReceivedMessage] = ResponseStatus;
  responseLength = indexOfPayloadInReceivedMessage + 1u;

  /* #10 Write the response frame to control buffer.*/
  Dlt_TxBuf_WriteToCtrlBuf(LogChannelIndex, responseLength);
} /* Dlt_Hlp_WriteFailResponseToControlBuffer */
# endif

/**********************************************************************************************************************
 * Dlt_Hlp_ResetToFactoryDefault
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(Std_ReturnType, DLT_CODE) Dlt_Hlp_ResetToFactoryDefault(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Reset all global flags to their initial values. */
  Dlt_FilterMessages          = TRUE;
# if (DLT_TIMESTAMP_SUPPORT == STD_ON)
#  if (DLT_HEADER_USE_TIMESTAMP == STD_ON)
  Dlt_HeaderUseTimestamp      = TRUE;
#  else
  Dlt_HeaderUseTimestamp      = FALSE;
#  endif
# endif
# if (DLT_HEADER_USE_ECU_ID == STD_ON)
  Dlt_HeaderUseEcuId          = TRUE;
# else
  Dlt_HeaderUseEcuId          = FALSE;
# endif
# if (DLT_HEADER_USE_SESSION_ID == STD_ON)
  Dlt_HeaderUseSessionId      = TRUE;
# else
  Dlt_HeaderUseSessionId      = FALSE;
# endif
  Dlt_DefaultMaxLogLevel      = (Dlt_MessageLogLevelType)DLT_DEFAULT_MAX_LOG_LEVEL;
  Dlt_DefaultTraceStatus      = DLT_DEFAULT_TRACE_STATUS;

  /* #20 Reset the complete Dlt_Context to default values and register Dem and Det contexts. */
  Dlt_Init_InitializeDltContext();
  Dlt_Init_RegisterDemAndDetContext();
# if (DLT_OS_SOFTWARE_TRACING == STD_ON)
  Dlt_Init_RegisterOsContexts();
# endif

# if (DLT_SWCCONTEXTDESCRIPTOR == STD_ON)
  /* #30 If enabled, register all configured Swc contexts. */
  Dlt_Init_RegisterDefaultDltContexts();
# endif

  /* #40 Initialize the log channel info with configured values. */
  Dlt_Init_InitializeLogChannelInfo();

# if (DLT_NVRAM_SUPPORT == STD_ON)
  /* Invalidate the persisted Dlt data. */
  retVal = NvM_InvalidateNvBlock(Dlt_GetNvDataSnv());
# else
  retVal = E_OK;
# endif

  return retVal;
} /* Dlt_Hlp_ResetToFactoryDefault */

# if (DLT_NVRAM_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Hlp_StoreConfiguration
 *********************************************************************************************************************/
/*!
 *
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
DLT_LOCAL_INLINE FUNC(Std_ReturnType, DLT_CODE) Dlt_Hlp_StoreConfiguration(void)
{
  /* ----- Local Variables ------------------------------------------------ */
#  if (DLT_SWCCONTEXTDESCRIPTOR == STD_ON)
  uint32 swcContextIndex;
#  endif
  uint32_least logChannelIndex;
  Std_ReturnType retVal = E_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Set the global settings to nv data. */
  Dlt_NvData.defaultLogChannelIndex = Dlt_DefaultLogChannelIndex;
  Dlt_NvData.defaultLogLevel = Dlt_DefaultMaxLogLevel;
  Dlt_NvData.defaultTraceStatus = Dlt_DefaultTraceStatus; /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
  Dlt_NvData.numberOfLogChannel = Dlt_GetNumberOfAllValidLogChannels();
  Dlt_NvData.numberOfSwcs = Dlt_GetNumberOfAllValidSwcs();
#  if (DLT_OS_SOFTWARE_TRACING == STD_ON)
  Dlt_NvData.numberOfSwcContexts = Dlt_GetNumberOfAllValidSwcContexts() + 2u + DLT_OS_TRACING_NUMBER_OF_TYPES;
#  else
  Dlt_NvData.numberOfSwcContexts = Dlt_GetNumberOfAllValidSwcContexts() + 2u;
#  endif

  /* #20 Set configuration of DEM context to nv data. */
  Dlt_NvData.logInfo[DLT_DEM_APPLICATION_ID_INDEX].appId = Dlt_Context.appIds[DLT_DEM_APPLICATION_ID_INDEX].appId;
  Dlt_NvData.logInfo[DLT_DEM_APPLICATION_ID_INDEX].contextId = Dlt_Context.appIds[DLT_DEM_APPLICATION_ID_INDEX].contextInfoList[DLT_DEM_CONTEXT_ID_INDEX].contextId;

  Dlt_NvData.logInfo[DLT_DEM_APPLICATION_ID_INDEX].appIndexInDltContext = DLT_DEM_APPLICATION_ID_INDEX;
  Dlt_NvData.logInfo[DLT_DEM_APPLICATION_ID_INDEX].contextIndexInDltContext = DLT_DEM_CONTEXT_ID_INDEX;
  Dlt_NvData.logInfo[DLT_DEM_APPLICATION_ID_INDEX].logLevel = Dlt_Context.appIds[DLT_DEM_APPLICATION_ID_INDEX].contextInfoList[DLT_DEM_CONTEXT_ID_INDEX].logLevel;
  Dlt_NvData.logInfo[DLT_DEM_APPLICATION_ID_INDEX].traceStatus = Dlt_Context.appIds[DLT_DEM_APPLICATION_ID_INDEX].contextInfoList[DLT_DEM_CONTEXT_ID_INDEX].traceStatus;

  /* #30 Set configuration of DET context to nv data. */
  Dlt_NvData.logInfo[DLT_DET_APPLICATION_ID_INDEX].appId = Dlt_Context.appIds[DLT_DET_APPLICATION_ID_INDEX].appId;
  Dlt_NvData.logInfo[DLT_DET_APPLICATION_ID_INDEX].contextId = Dlt_Context.appIds[DLT_DET_APPLICATION_ID_INDEX].contextInfoList[DLT_DET_CONTEXT_ID_INDEX].contextId;

  Dlt_NvData.logInfo[DLT_DET_APPLICATION_ID_INDEX].appIndexInDltContext = DLT_DET_APPLICATION_ID_INDEX;
  Dlt_NvData.logInfo[DLT_DET_APPLICATION_ID_INDEX].contextIndexInDltContext = DLT_DET_CONTEXT_ID_INDEX;
  Dlt_NvData.logInfo[DLT_DET_APPLICATION_ID_INDEX].logLevel = Dlt_Context.appIds[DLT_DET_APPLICATION_ID_INDEX].contextInfoList[DLT_DET_CONTEXT_ID_INDEX].logLevel;
  Dlt_NvData.logInfo[DLT_DET_APPLICATION_ID_INDEX].traceStatus = Dlt_Context.appIds[DLT_DET_APPLICATION_ID_INDEX].contextInfoList[DLT_DET_CONTEXT_ID_INDEX].traceStatus;

#  if (DLT_OS_SOFTWARE_TRACING == STD_ON)
  /* #35 Set configuration of OS application and all contexts to nv data. */
  /*  -> Schedule: */
  Dlt_NvData.logInfo[DLT_OS_VTH_SCHEDULE_LOG_INFO_INDEX].appId = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].appId;
  Dlt_NvData.logInfo[DLT_OS_VTH_SCHEDULE_LOG_INFO_INDEX].contextId = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_SCHEDULE_CONTEXT_INDEX].contextId;

  Dlt_NvData.logInfo[DLT_OS_VTH_SCHEDULE_LOG_INFO_INDEX].appIndexInDltContext = DLT_OS_VTH_APPLICATION_INDEX;
  Dlt_NvData.logInfo[DLT_OS_VTH_SCHEDULE_LOG_INFO_INDEX].contextIndexInDltContext = DLT_OS_VTH_SCHEDULE_CONTEXT_INDEX;
  Dlt_NvData.logInfo[DLT_OS_VTH_SCHEDULE_LOG_INFO_INDEX].logLevel = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_SCHEDULE_CONTEXT_INDEX].logLevel;
  Dlt_NvData.logInfo[DLT_OS_VTH_SCHEDULE_LOG_INFO_INDEX].traceStatus = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_SCHEDULE_CONTEXT_INDEX].traceStatus;

  /*  -> Activation: */
  Dlt_NvData.logInfo[DLT_OS_VTH_ACTIVATION_LOG_INFO_INDEX].appId = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].appId;
  Dlt_NvData.logInfo[DLT_OS_VTH_ACTIVATION_LOG_INFO_INDEX].contextId = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_ACTIVATION_CONTEXT_INDEX].contextId;

  Dlt_NvData.logInfo[DLT_OS_VTH_ACTIVATION_LOG_INFO_INDEX].appIndexInDltContext = DLT_OS_VTH_APPLICATION_INDEX;
  Dlt_NvData.logInfo[DLT_OS_VTH_ACTIVATION_LOG_INFO_INDEX].contextIndexInDltContext = DLT_OS_VTH_ACTIVATION_CONTEXT_INDEX;
  Dlt_NvData.logInfo[DLT_OS_VTH_ACTIVATION_LOG_INFO_INDEX].logLevel = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_ACTIVATION_CONTEXT_INDEX].logLevel;
  Dlt_NvData.logInfo[DLT_OS_VTH_ACTIVATION_LOG_INFO_INDEX].traceStatus = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_ACTIVATION_CONTEXT_INDEX].traceStatus;

  /*  -> Schedule: */
  Dlt_NvData.logInfo[DLT_OS_VTH_SETEVENT_LOG_INFO_INDEX].appId = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].appId;
  Dlt_NvData.logInfo[DLT_OS_VTH_SETEVENT_LOG_INFO_INDEX].contextId = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_SETEVENT_CONTEXT_INDEX].contextId;

  Dlt_NvData.logInfo[DLT_OS_VTH_SETEVENT_LOG_INFO_INDEX].appIndexInDltContext = DLT_OS_VTH_APPLICATION_INDEX;
  Dlt_NvData.logInfo[DLT_OS_VTH_SETEVENT_LOG_INFO_INDEX].contextIndexInDltContext = DLT_OS_VTH_SETEVENT_CONTEXT_INDEX;
  Dlt_NvData.logInfo[DLT_OS_VTH_SETEVENT_LOG_INFO_INDEX].logLevel = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_SETEVENT_CONTEXT_INDEX].logLevel;
  Dlt_NvData.logInfo[DLT_OS_VTH_SETEVENT_LOG_INFO_INDEX].traceStatus = Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_SETEVENT_CONTEXT_INDEX].traceStatus;
#  endif

  /* #40 Set log channel assignments of DEM, DET, and OS contexts to nv data. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetNumberOfAllValidLogChannels(); logChannelIndex++)
  {
    Dlt_NvData.logInfo[DLT_DEM_APPLICATION_ID_INDEX].logChannelIndexes[logChannelIndex] =
      Dlt_Context.appIds[DLT_DEM_APPLICATION_ID_INDEX].contextInfoList[DLT_DEM_CONTEXT_ID_INDEX].logChannelIndexes[logChannelIndex];

    Dlt_NvData.logInfo[DLT_DET_APPLICATION_ID_INDEX].logChannelIndexes[logChannelIndex] =
      Dlt_Context.appIds[DLT_DET_APPLICATION_ID_INDEX].contextInfoList[DLT_DET_CONTEXT_ID_INDEX].logChannelIndexes[logChannelIndex];

#  if (DLT_OS_SOFTWARE_TRACING == STD_ON)
    Dlt_NvData.logInfo[DLT_OS_VTH_SCHEDULE_LOG_INFO_INDEX].logChannelIndexes[logChannelIndex] =
      Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_SCHEDULE_CONTEXT_INDEX].logChannelIndexes[logChannelIndex];

    Dlt_NvData.logInfo[DLT_OS_VTH_ACTIVATION_LOG_INFO_INDEX].logChannelIndexes[logChannelIndex] =
      Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_ACTIVATION_CONTEXT_INDEX].logChannelIndexes[logChannelIndex];

    Dlt_NvData.logInfo[DLT_OS_VTH_SETEVENT_LOG_INFO_INDEX].logChannelIndexes[logChannelIndex] =
      Dlt_Context.appIds[DLT_OS_VTH_APPLICATION_INDEX].contextInfoList[DLT_OS_VTH_SETEVENT_CONTEXT_INDEX].logChannelIndexes[logChannelIndex];
#  endif
  }

#  if (DLT_SWCCONTEXTDESCRIPTOR == STD_ON)
  /* #50 Set the log infos of nv data. */
  for (swcContextIndex = 0u; swcContextIndex < Dlt_GetNumberOfAllValidSwcContexts(); swcContextIndex++)
  {
#   if (DLT_OS_SOFTWARE_TRACING == STD_ON)
    uint32 logInfoIndex = swcContextIndex + 2u + DLT_OS_TRACING_NUMBER_OF_TYPES; /* Access to Dlt_NvData.logInfo starts with 5 because of DEM, DET, and OS. */
#   else
    uint32 logInfoIndex = swcContextIndex + 2u; /* Access to Dlt_NvData.logInfo starts with 2 because of DEM and DET. */
#   endif
    uint16 applicationIdIndex = 0u;
    uint16 contextIdIndex = 0u;

    Dlt_NvData.logInfo[logInfoIndex].appId = (Dlt_LocalApplicationIDType)Dlt_GetApplicationIdOfSwcContextDescriptor(swcContextIndex);
    Dlt_NvData.logInfo[logInfoIndex].contextId = (Dlt_LocalContextIDType)Dlt_GetContextIdOfSwcContextDescriptor(swcContextIndex);

    /* #60 Look for the indexes for application id and context id in the Dlt_Context, then set them to nv data.
     *     Set the log channel indexes the Dlt user is assigned to. */
    (void)Dlt_Hlp_GetApplicationIDIndex(Dlt_NvData.logInfo[logInfoIndex].appId, &applicationIdIndex);

    /*@ assert applicationIdIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
    (void)Dlt_Hlp_GetContextIDIndex(applicationIdIndex, Dlt_NvData.logInfo[logInfoIndex].contextId, &contextIdIndex);

    /*@ assert contextIdIndex < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

    Dlt_NvData.logInfo[logInfoIndex].appIndexInDltContext = applicationIdIndex;
    Dlt_NvData.logInfo[logInfoIndex].contextIndexInDltContext = contextIdIndex;
    Dlt_NvData.logInfo[logInfoIndex].logLevel = Dlt_Context.appIds[applicationIdIndex].contextInfoList[contextIdIndex].logLevel;
    Dlt_NvData.logInfo[logInfoIndex].traceStatus = Dlt_Context.appIds[applicationIdIndex].contextInfoList[contextIdIndex].traceStatus;

    for (logChannelIndex = 0u; logChannelIndex < Dlt_GetNumberOfAllValidLogChannels(); logChannelIndex++)
    {
      Dlt_NvData.logInfo[logInfoIndex].logChannelIndexes[logChannelIndex] =
      Dlt_Context.appIds[applicationIdIndex].contextInfoList[contextIdIndex].logChannelIndexes[logChannelIndex];
    }
  }
#  endif

  /* #70 Set the log channel infos of nv data. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetNumberOfAllValidLogChannels(); logChannelIndex++)
  {
    Dlt_NvData.logChannelInfo[logChannelIndex].logChannelId = Dlt_LogChannelInfo[logChannelIndex].logChannelId;
    Dlt_NvData.logChannelInfo[logChannelIndex].logChannelThreshold = Dlt_LogChannelInfo[logChannelIndex].logChannelThreshold;
    Dlt_NvData.logChannelInfo[logChannelIndex].logChannelTraceStatus = Dlt_LogChannelInfo[logChannelIndex].logChannelTraceStatus;
  }

  /* #80 If setting the RAM persentation of nv data block succeeded, request to persist it. */
  retVal = NvM_WriteBlock(Dlt_GetNvDataSnv(), &Dlt_NvData); /* PRQA S 0315 */ /* MD_Dlt_NvMWriteBlock */

  return retVal;
} /* Dlt_Hlp_StoreConfiguration */
# endif

/**********************************************************************************************************************
 * Dlt_Hlp_SetLogChannelAssignment
 *********************************************************************************************************************/
/*!
 *
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
DLT_LOCAL_INLINE FUNC(Std_ReturnType, Dlt_CODE) Dlt_Hlp_SetLogChannelAssignment(
  uint32 AppId,
  uint32 ContextId,
  uint32 LogChannelName,
  Dlt_AssignmentOperation AddRemoveOp)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndexOfRequestedLogChannel = DLT_INVALID_LOG_CHANNEL_INDEX;
  uint16 appIdIndex;
  uint16 conIdIndex;
  boolean applicationAlreadyRegistered = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
  boolean contextAlreadyRegistered;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  contextAlreadyRegistered = Dlt_Hlp_IsContextRegistered(AppId, ContextId, &appIdIndex, &conIdIndex, &applicationAlreadyRegistered);

  /*@ assert appIdIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
  /*@ assert conIdIndex < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

  /* #10 If the application id and context id are registered: */
  if ((applicationAlreadyRegistered == TRUE) /* PRQA S 2995 1 */ /* MD_Dlt_AlwaysTrue */
   && (contextAlreadyRegistered == TRUE)) /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
  {
    /* #20 Get the log channel index of the given log channel id. */
    retVal = Dlt_Hlp_GetLogChannelIndexByLogChannelName(LogChannelName, &logChannelIndexOfRequestedLogChannel);

    /*@ assert logChannelIndexOfRequestedLogChannel < Dlt_GetNumberOfAllValidLogChannels(); */ /* VCA_DLT_VALID_CONFIGURED_LOGCHANNEL_ID */

    /* #30 If the requested log channel is valid: */
    if (retVal == E_OK) /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
    {
      /* #40 If the log channel assignment shall be removed: */
      if (AddRemoveOp == DLT_ASSIGN_REMOVE)
      {
        /* #50 If the log channel is assigned, remove it now. */
        if (Dlt_Context.appIds[appIdIndex].contextInfoList[conIdIndex].logChannelIndexes[logChannelIndexOfRequestedLogChannel] != DLT_INVALID_LOG_CHANNEL_INDEX) /* COV_DLT_LOG_CHNL_OPTION */
        {
          Dlt_Context.appIds[appIdIndex].contextInfoList[conIdIndex].logChannelIndexes[logChannelIndexOfRequestedLogChannel] = DLT_INVALID_LOG_CHANNEL_INDEX;
        }
      }
      /* #60 Otherwise, the log channel assignment shall be added: */
      else
      {
        /* #70 If the log channel is not assigned, add it now. */
        if (Dlt_Context.appIds[appIdIndex].contextInfoList[conIdIndex].logChannelIndexes[logChannelIndexOfRequestedLogChannel] == DLT_INVALID_LOG_CHANNEL_INDEX) /* COV_DLT_LOG_CHNL_OPTION */
        {
          Dlt_Context.appIds[appIdIndex].contextInfoList[conIdIndex].logChannelIndexes[logChannelIndexOfRequestedLogChannel] = logChannelIndexOfRequestedLogChannel;
        }
      }
    }
  }

  return retVal;
} /* Dlt_Hlp_SetLogChannelAssignment */

/**********************************************************************************************************************
 * Dlt_Hlp_GetLogChannelThresholdByName
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(Std_ReturnType, Dlt_CODE) Dlt_Hlp_GetLogChannelThresholdByName(
  uint32 RequestedLogChannelId,
  P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_VAR) Threshold,
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) TraceStatus)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32_least logChannelIndex;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Get the log channel index of the given log channel id. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    if (Dlt_LogChannelInfo[logChannelIndex].logChannelId == RequestedLogChannelId)
    {
      *Threshold = Dlt_LogChannelInfo[logChannelIndex].logChannelThreshold;
      *TraceStatus = Dlt_LogChannelInfo[logChannelIndex].logChannelTraceStatus;

      retVal = DLT_E_OK;
      break;
    }
  }

  return retVal;
} /* Dlt_Hlp_GetLogChannelThresholdByName */

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Hlp_GetLogChannelThresholdByIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, Dlt_CODE) Dlt_Hlp_GetLogChannelThresholdByIndex(
  uint32 RequestedLogChannelIndex,
  P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_VAR) Threshold,
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) TraceStatus)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Get the log channel threshold an trace status of the given log channel id. */
  *Threshold = Dlt_LogChannelInfo[RequestedLogChannelIndex].logChannelThreshold;
  *TraceStatus = Dlt_LogChannelInfo[RequestedLogChannelIndex].logChannelTraceStatus;
} /* Dlt_Hlp_GetLogChannelThresholdByIndex */
# endif

/**********************************************************************************************************************
 * Dlt_Hlp_SetLogChannelThresholdByName
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(Std_ReturnType, Dlt_CODE) Dlt_Hlp_SetLogChannelThresholdByName(
  uint32 RequestedLogChannelId,
  Dlt_MessageLogLevelType NewThreshold,
  boolean NewTraceStatus)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Get the log channel index of the given log channel id. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    if (Dlt_LogChannelInfo[logChannelIndex].logChannelId == RequestedLogChannelId)
    {
      retVal = Dlt_Hlp_SetLogChannelThresholdByIndex(logChannelIndex, NewThreshold, NewTraceStatus);
      break;
    }
  }

  return retVal;
} /* Dlt_Hlp_SetLogChannelThresholdByName */

/**********************************************************************************************************************
 * Dlt_Hlp_SetLogChannelThresholdByIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(Std_ReturnType, Dlt_CODE) Dlt_Hlp_SetLogChannelThresholdByIndex(
  uint32 RequestedLogChannelIndex,
  Dlt_MessageLogLevelType NewThreshold,
  boolean NewTraceStatus)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 isLogLevelValid = Dlt_Hlp_ParamInValidRange(DLT_OPT_LOGLEVEL, (sint8)NewThreshold);
  uint8 isTraceStatus = Dlt_Hlp_ParamInValidRange(DLT_OPT_BOOLEAN, (sint8)NewTraceStatus); /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */

  /* ----- Implementation ------------------------------------------------- */
  if ((isLogLevelValid == DLT_OK)
   && (isTraceStatus == DLT_OK)) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
  {
    /* #10 Set the log channel threshold an trace status of the given log channel id. */
    Dlt_LogChannelInfo[RequestedLogChannelIndex].logChannelThreshold = NewThreshold;
    Dlt_LogChannelInfo[RequestedLogChannelIndex].logChannelTraceStatus = NewTraceStatus;

    retVal = E_OK;
  }

  return retVal;
} /* Dlt_Hlp_SetLogChannelThresholdByIndex */

/**********************************************************************************************************************
 * Dlt_Hlp_GetLogChannelIndexByLogChannelName
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(Std_ReturnType, Dlt_CODE) Dlt_Hlp_GetLogChannelIndexByLogChannelName(
  uint32 LogChannelName,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) LogChannelIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Get the log channel index of the given log channel id/name. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++) /* COV_DLT_LOG_CHNL_OPTION */
  {
    if (Dlt_LogChannelInfo[logChannelIndex].logChannelId == LogChannelName) /* COV_DLT_LOG_CHNL_OPTION */
    {
      *LogChannelIndex = logChannelIndex;
      retVal = DLT_E_OK;
      break;
    }
  }

  return retVal;
} /* Dlt_Hlp_GetLogChannelIndexByLogChannelName */

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Hlp_GetIndexOfExtendedHeaderInReceivedMessage
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_Hlp_GetIndexOfExtendedHeaderInReceivedMessage(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 indexOfExtendedHeaderInReceivedMessage = DLT_STD_HEADER_MANDATORY_END_INDEX;

  /* ----- Implementation ------------------------------------------------- */
  if ((Dlt_ReceiveMessage[DLT_STD_HEADER_HEADERTYPE_INDEX] & Dlt_StandardHeaderType_WEID_Mask) == Dlt_StandardHeaderType_WEID_Mask)
  {
    indexOfExtendedHeaderInReceivedMessage += DLT_DEFAULT_ID_LENGTH;
  }

  /* #10 If the session id is set in the request, set it also in the response. */
  if ((Dlt_ReceiveMessage[DLT_STD_HEADER_HEADERTYPE_INDEX] & Dlt_StandardHeaderType_WSID_Mask) == Dlt_StandardHeaderType_WSID_Mask)
  {
    indexOfExtendedHeaderInReceivedMessage += DLT_DEFAULT_ID_LENGTH;
  }

  /* #20 If the timestamp is set in the request, set it also in the response. */
  if ((Dlt_ReceiveMessage[DLT_STD_HEADER_HEADERTYPE_INDEX] & Dlt_StandardHeaderType_WTMS_Mask) == Dlt_StandardHeaderType_WTMS_Mask)
  {
    indexOfExtendedHeaderInReceivedMessage += DLT_DEFAULT_ID_LENGTH;
  }

  return indexOfExtendedHeaderInReceivedMessage;
} /* Dlt_Hlp_GetIndexOfExtendedHeaderInReceivedMessage */

/**********************************************************************************************************************
 * Dlt_Hlp_GetIndexOfPayloadInReceivedMessage
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_Hlp_GetIndexOfPayloadInReceivedMessage(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  /* #10 Return standard + extended header. */
  uint16 indexOfPayloadInReceivedMessage =
      Dlt_Hlp_GetIndexOfExtendedHeaderInReceivedMessage()
    + DLT_EXT_HEADER_LENGTH;

  /* ----- Implementation ------------------------------------------------- */
  return indexOfPayloadInReceivedMessage;
} /* Dlt_Hlp_GetIndexOfPayloadAfterServiceIdInReceivedMessage */

/**********************************************************************************************************************
 * Dlt_Hlp_GetIndexOfPayloadAfterServiceIdInReceivedMessage
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_Hlp_GetIndexOfPayloadAfterServiceIdInReceivedMessage(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 indexOfPayloadAfterServiceIdInReceivedMessage =
      Dlt_Hlp_GetIndexOfPayloadInReceivedMessage()
    + DLT_DEFAULT_ID_LENGTH; /* Length of Standard/Extended header + length of service id. */

  /* ----- Implementation ------------------------------------------------- */
  return indexOfPayloadAfterServiceIdInReceivedMessage;
} /* Dlt_Hlp_GetIndexOfPayloadAfterServiceIdInReceivedMessage */
# endif

/* ####################################################################################################################
 *     IIIIII    NN      NN    IIIIII    TTTTTTTTTT
 *       II      NNNN    NN      II          TT
 *       II      NNNN    NN      II          TT
 *       II      NN  NN  NN      II          TT
 *       II      NN    NNNN      II          TT
 *       II      NN    NNNN      II          TT
 *     IIIIII    NN      NN    IIIIII        TT
 * ####################################################################################################################
 */

/**********************************************************************************************************************
 * Dlt_Init_InitializeDltContext
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_InitializeDltContext(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32_least appIndex;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Init Dlt context table:
   *       Initialize all contexts with default values.
   */
  Dlt_Context.appIdCount = 0u;

  for (appIndex = 0u; appIndex < Dlt_GetMaxNumberOfApplications(); appIndex++)
  {
    uint32_least contextIndex;

    Dlt_Context.appIds[appIndex].appId = 0u;
    Dlt_Context.appIds[appIndex].appDescription = NULL_PTR;
    Dlt_Context.appIds[appIndex].appDescriptionLen = 0u;
    Dlt_Context.appIds[appIndex].contextIdCount = 0u;

    for (contextIndex = 0u; contextIndex < Dlt_GetMaxNumberOfContextsPerApplication(); contextIndex++)
    {
      uint32_least logChannelIndex;

      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].contextId = (Dlt_LocalContextIDType)0u;
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].contextDescription = (P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR))NULL_PTR;
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].lenContextDescription = (uint8)0u;
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].logLevel = (sint8)Dlt_DefaultMaxLogLevel;
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].traceStatus = (sint8)Dlt_DefaultTraceStatus; /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].cbkInfo.sessionId = (Dlt_SessionIDType)0u;
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].cbkInfo.Dlt_SetLogLevel_ = NULL_PTR;
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].cbkInfo.Dlt_SetTraceStatus_ = NULL_PTR;
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].cbkInfo.Dlt_InjectCall_ = NULL_PTR;

      for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
      {
        Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].logChannelIndexes[logChannelIndex] = DLT_INVALID_LOG_CHANNEL_INDEX;
      }
    }
  }
} /* Dlt_Init_InitializeDltContext */

/**********************************************************************************************************************
 * Dlt_Init_RegisterDemAndDetContext
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_RegisterDemAndDetContext(void)
{
  /* ----- Implementation ------------------------------------------------- */
  /* Add DEM context entry */
  Dlt_Context.appIdCount++;
  Dlt_Context.appIds[0].appId = (Dlt_LocalApplicationIDType)Dlt_Dem_ApplId; /* DEM  */
  Dlt_Context.appIds[0].appDescription = (P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR))Dlt_Dem_ApplId_Desc; /* DEM */
  Dlt_Context.appIds[0].appDescriptionLen = (uint8)DLT_DEM_APPLID_DESC_LENGTH;
  Dlt_Context.appIds[0].contextIdCount = 1u;
  Dlt_Context.appIds[0].contextInfoList[0].logChannelIndexes[DLT_DEFAULT_LOG_CHANNEL_INDEX] = DLT_DEFAULT_LOG_CHANNEL_INDEX;
  Dlt_Context.appIds[0].contextInfoList[0].contextId = (Dlt_LocalContextIDType)Dlt_Dem_ContextId; /* STD0  */
  Dlt_Context.appIds[0].contextInfoList[0].contextDescription = (P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR))Dlt_Dem_ContextId_Desc; /* STD0 */
  Dlt_Context.appIds[0].contextInfoList[0].lenContextDescription = (uint8)DLT_DEM_CONTEXTID_DESC_LENGTH;
  Dlt_Context.appIds[0].contextInfoList[0].logLevel = (sint8)DLT_DEFAULT_MAX_LOG_LEVEL;
  Dlt_Context.appIds[0].contextInfoList[0].traceStatus = (sint8)DLT_DEFAULT_TRACE_STATUS; /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
  Dlt_Context.appIds[0].contextInfoList[0].cbkInfo.sessionId = (Dlt_SessionIDType)0x36u; /* Dem module ID */

  /* Add DET context entry */
  Dlt_Context.appIdCount++;
  Dlt_Context.appIds[1].appId = (Dlt_LocalApplicationIDType)Dlt_Det_ApplId; /* DET */
  Dlt_Context.appIds[1].appDescription = (P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR))Dlt_Det_ApplId_Desc; /* DET */
  Dlt_Context.appIds[1].appDescriptionLen = (uint8)DLT_DET_APPLID_DESC_LENGTH;
  Dlt_Context.appIds[1].contextIdCount = 1u;
  Dlt_Context.appIds[1].contextInfoList[0].logChannelIndexes[DLT_DEFAULT_LOG_CHANNEL_INDEX] = DLT_DEFAULT_LOG_CHANNEL_INDEX;
  Dlt_Context.appIds[1].contextInfoList[0].contextId = (Dlt_LocalContextIDType)Dlt_Det_ContextId; /* STD1 */
  Dlt_Context.appIds[1].contextInfoList[0].contextDescription = (P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR))Dlt_Det_ContextId_Desc; /* STD1 */
  Dlt_Context.appIds[1].contextInfoList[0].lenContextDescription = (uint8)DLT_DET_CONTEXTID_DESC_LENGTH;
  Dlt_Context.appIds[1].contextInfoList[0].logLevel = (sint8)DLT_DEFAULT_MAX_LOG_LEVEL;
  Dlt_Context.appIds[1].contextInfoList[0].traceStatus = (sint8)DLT_DEFAULT_TRACE_STATUS; /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
  Dlt_Context.appIds[1].contextInfoList[0].cbkInfo.sessionId = (Dlt_SessionIDType)0x0Fu; /* Det module ID */

  Dlt_NumberOfRegisteredContextIds = 2u;
} /* Dlt_Init_RegisterDemAndDetContext */

# if (DLT_OS_SOFTWARE_TRACING == STD_ON)
/**********************************************************************************************************************
 * Dlt_Init_RegisterOsContexts
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_RegisterOsContexts(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 appId;
  uint32 contextId;
  uint16 appIdIndex;
  const uint16 usedCoreId = DLT_USED_CORE_ID;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Initialize the event counter. */
  Dlt_EventCounter = 0u;

  /* #20 If the configuration is valid (the three VTHs can register their application id and all context ids):
   *       Register the core specific application id:
   *         Register the schedule context.
   *         Register the activation context.
   *         Register the set event context. */
  if (((Dlt_Context.appIdCount + 1u) < Dlt_GetMaxNumberOfApplications())
   && ((Dlt_Context.appIds[Dlt_Context.appIdCount].contextIdCount + DLT_OS_TRACING_NUMBER_OF_TYPES) < Dlt_GetMaxNumberOfContextsPerApplication()))
  {
    uint16 contextIdIndex;
    /* Add OS context entries for Os Tracing: */
    Dlt_Hlp_CopyArrayToLongBigEndian(Dlt_MessageTraceInfoOfVthSchedule[usedCoreId].appId, &appId);

    appIdIndex = Dlt_Context.appIdCount;
    Dlt_Context.appIdCount++;
    Dlt_Context.appIds[appIdIndex].appId = (Dlt_LocalApplicationIDType)appId; /* Cr<CoreIdAs2ASCII> */
    Dlt_Context.appIds[appIdIndex].appDescription = NULL_PTR; /* DEM */
    Dlt_Context.appIds[appIdIndex].appDescriptionLen = 0u;

    contextIdIndex = Dlt_Context.appIds[appIdIndex].contextIdCount;
    Dlt_Context.appIds[appIdIndex].contextIdCount += DLT_OS_TRACING_NUMBER_OF_TYPES;

    /*  1. Scheduling: */
    Dlt_Hlp_CopyArrayToLongBigEndian(Dlt_MessageTraceInfoOfVthSchedule[usedCoreId].context, &contextId);

    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].logChannelIndexes[DLT_DEFAULT_LOG_CHANNEL_INDEX] = DLT_DEFAULT_LOG_CHANNEL_INDEX;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].contextId = (Dlt_LocalContextIDType)contextId; /* OS - Scheduling - OsSh */
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].contextDescription = NULL_PTR;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].lenContextDescription = 0u;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].logLevel = (sint8)DLT_DEFAULT_MAX_LOG_LEVEL;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].traceStatus = (sint8)DLT_DEFAULT_TRACE_STATUS; /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].cbkInfo.sessionId = (Dlt_SessionIDType)DLT_OS_TRACING_SESSION_ID; /* OS module id */

    contextIdIndex++;
    Dlt_NumberOfRegisteredContextIds++;

    /*  2. Activation: */
    Dlt_Hlp_CopyArrayToLongBigEndian(Dlt_MessageTraceInfoOfVthActivation[usedCoreId].context, &contextId);

    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].logChannelIndexes[DLT_DEFAULT_LOG_CHANNEL_INDEX] = DLT_DEFAULT_LOG_CHANNEL_INDEX;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].contextId = (Dlt_LocalContextIDType)contextId; /* OS - Activation - OsAc */
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].contextDescription = NULL_PTR;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].lenContextDescription = 0u;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].logLevel = (sint8)DLT_DEFAULT_MAX_LOG_LEVEL;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].traceStatus = (sint8)DLT_DEFAULT_TRACE_STATUS; /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].cbkInfo.sessionId = (Dlt_SessionIDType)DLT_OS_TRACING_SESSION_ID; /* OS module id */

    contextIdIndex++;
    Dlt_NumberOfRegisteredContextIds++;

    /*  3. SetEvent: */
    Dlt_Hlp_CopyArrayToLongBigEndian(Dlt_MessageTraceInfoOfVthSetEvent[usedCoreId].context, &contextId);

    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].logChannelIndexes[DLT_DEFAULT_LOG_CHANNEL_INDEX] = DLT_DEFAULT_LOG_CHANNEL_INDEX;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].contextId = (Dlt_LocalContextIDType)contextId; /* OS - SetEvent - OsSe */
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].contextDescription = NULL_PTR;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].lenContextDescription = 0u;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].logLevel = (sint8)DLT_DEFAULT_MAX_LOG_LEVEL;
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].traceStatus = (sint8)DLT_DEFAULT_TRACE_STATUS; /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
    Dlt_Context.appIds[appIdIndex].contextInfoList[contextIdIndex].cbkInfo.sessionId = (Dlt_SessionIDType)DLT_OS_TRACING_SESSION_ID; /* OS module id */

    Dlt_NumberOfRegisteredContextIds++;
  }
} /* Dlt_Init_RegisterOsContexts */
# endif

/**********************************************************************************************************************
 * Dlt_Init_InitializeLogChannelInfo
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_InitializeLogChannelInfo(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32_least logChannelIndex;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Iterate over all log channels and initialize their log channel infos (id, threshold, and trace status). */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetNumberOfAllValidLogChannels(); logChannelIndex++)
  {
    Dlt_LogChannelInfo[logChannelIndex].logChannelId = Dlt_GetLogChannelIdOfLogChannelDescriptor(logChannelIndex);
    Dlt_LogChannelInfo[logChannelIndex].logChannelThreshold = Dlt_GetLogChannelThresholdOfLogChannelDescriptor(logChannelIndex);

    if (Dlt_IsLogChannelTraceStatusOfLogChannelDescriptor(logChannelIndex)) /* COV_DLT_LOG_CHNL_OPTION */
    {
      Dlt_LogChannelInfo[logChannelIndex].logChannelTraceStatus = TRUE;
    }
    else
    {
      Dlt_LogChannelInfo[logChannelIndex].logChannelTraceStatus = FALSE;
    }
  }
} /* Dlt_Init_InitializeLogChannelInfo */

# if (DLT_SWCCONTEXTDESCRIPTOR == STD_ON)
/**********************************************************************************************************************
 * Dlt_Init_RegisterDefaultDltContexts
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */

DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_RegisterDefaultDltContexts(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16_least swcContextIndex;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 It is expected, that all configured SWC contexts fit into the size of DltContext. */
  for (swcContextIndex = 0u; swcContextIndex < Dlt_GetSizeOfSwcContextDescriptor(); swcContextIndex++)
  {
    uint8 retVal;
    uint16 appIndex = 0u;
    uint16 contextIndex = 0u;
    uint16 swcIndex = Dlt_GetSwcDescriptorIdxOfSwcContextDescriptor(swcContextIndex);
    Dlt_LocalApplicationIDType applicationID = Dlt_GetApplicationIdOfSwcContextDescriptor(swcContextIndex);
    Dlt_LocalContextIDType contextID = Dlt_GetContextIdOfSwcContextDescriptor(swcContextIndex);
    Dlt_SessionIDType sessionID = (Dlt_SessionIDType)Dlt_GetSessionIdOfSwcDescriptor(swcIndex);
    uint32_least assignedLogChannelIndex;

    retVal = Dlt_Hlp_GetApplicationIDIndex(applicationID, &appIndex);

    /* #20 If the application is not yet registered, use next free index. */
    if (retVal == DLT_NOT_OK)
    {
      appIndex = Dlt_Context.appIdCount;
      Dlt_Context.appIdCount++;
    }

    /*@ assert appIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */

    retVal = Dlt_Hlp_GetContextIDIndex(appIndex, contextID, &contextIndex);

    /* #30 If the context is not yet registered, use next free index. */
    if (retVal == DLT_NOT_OK)
    {
      contextIndex = Dlt_Context.appIds[appIndex].contextIdCount;
      Dlt_Context.appIds[appIndex].contextIdCount++;
    }

    /*@ assert contextIndex < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

    Dlt_Context.appIds[appIndex].appId = applicationID;
    Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].contextId = contextID;

    for (assignedLogChannelIndex = Dlt_GetAssignedLogChannelDescriptorStartIdxOfSwcContextDescriptor(swcContextIndex);
         assignedLogChannelIndex < Dlt_GetAssignedLogChannelDescriptorEndIdxOfSwcContextDescriptor(swcContextIndex);
         assignedLogChannelIndex++)
    {
      uint32 logChannelIndex = Dlt_GetLogChannelIndexOfAssignedLogChannelDescriptor(assignedLogChannelIndex);
      /*@ assert logChannelIndex < Dlt_GetNumberOfAllValidLogChannels(); */ /* VCA_DLT_VALID_CONFIGURED_LOGCHANNEL_ID */

      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].logChannelIndexes[logChannelIndex] = logChannelIndex;
    }
    Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].logLevel = (sint8)Dlt_GetThresholdOfSwcContextDescriptor(swcContextIndex);

    if (Dlt_IsTraceStatusOfSwcContextDescriptor(swcContextIndex)) /* COV_DLT_SWC_CNTX_OPTION */
    {
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].traceStatus = (sint8)(TRUE); /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
    }
    else
    {
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].traceStatus = (sint8)(FALSE); /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
    }

    Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].cbkInfo.sessionId = sessionID;

    Dlt_NumberOfRegisteredContextIds++;

    /* #40 If the application shall be notified if log level or trace status of this application id/context id is changed:
     *       Assign the corresponding callout functions. */
    if (Dlt_IsSupportLogLevelChangeNotificationOfSwcDescriptor(swcIndex)) /* COV_DLT_LOG_LVL_CHG_NOTIFICATION */
    {
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].cbkInfo.Dlt_SetLogLevel_ = Dlt_SetLogLevelOpClass[sessionID - 0x1000u];
      Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].cbkInfo.Dlt_SetTraceStatus_ = Dlt_SetTraceStatusOpClass[sessionID - 0x1000u];
    }
    Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].cbkInfo.Dlt_InjectCall_ = Dlt_InjectionCallOpClass[sessionID - 0x1000u];
  }
} /* Dlt_Init_RegisterDefaultDltContexts */
# endif /* (DLT_SWCCONTEXTDESCRIPTOR == STD_ON) */

# if (DLT_NVRAM_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Init_CheckAndSetPersistedData
 *********************************************************************************************************************/
/*!
 *
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Init_CheckAndSetPersistedData(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  uint8 errorStatus;

  /* ----- Implementation ------------------------------------------------- */
  /* Get NvM block status of Dlt_NvData. */
  retVal = NvM_GetErrorStatus(Dlt_GetNvDataSnv(), &errorStatus);

  /* #10 If the Nv data could be restored successfully:  */
  if ((retVal == E_OK)
   && ((errorStatus == NVM_REQ_OK)
    || (errorStatus == NVM_REQ_RESTORED_FROM_ROM)))
  {
#  if (DLT_OS_SOFTWARE_TRACING == STD_ON)
    uint32 numberOfSwcContext = Dlt_GetNumberOfAllValidSwcContexts() + 2u + DLT_OS_TRACING_NUMBER_OF_TYPES;
#  else
    uint32 numberOfSwcContext = Dlt_GetNumberOfAllValidSwcContexts() + 2u;
#  endif

    /* #20 If the number of LogChannels, Swcs and SwcContexts did not change in the configuration: */
    if ((Dlt_NvData.numberOfLogChannel == Dlt_GetNumberOfAllValidLogChannels())
     && (Dlt_NvData.numberOfSwcs == Dlt_GetNumberOfAllValidSwcs())
     && (Dlt_NvData.numberOfSwcContexts == numberOfSwcContext))
    {
      /* #30 Set the default values of LogChannel, log level and trace status as persisted. */
      Dlt_DefaultLogChannelIndex = Dlt_NvData.defaultLogChannelIndex;
      Dlt_DefaultMaxLogLevel = Dlt_NvData.defaultLogLevel;
      Dlt_DefaultTraceStatus = (boolean)Dlt_NvData.defaultTraceStatus;

      /* Note: There is currently only one log channel supported and its logChannelThreshold and logChannelTraceStatus are ignored,
       *       Therefore, the persisted logChannel infos are not set to RAM variables. */

      /* #40 Change all the values of all configured SwcContexts (including DEM and DET contexts) as persisted. */
      {
        uint32_least swcContextindex;
        uint16_least appIndexDltContext;
        uint16_least contextIndexDltContext;

        for (swcContextindex = 0u; swcContextindex < numberOfSwcContext; swcContextindex++)
        {
          appIndexDltContext = Dlt_NvData.logInfo[swcContextindex].appIndexInDltContext;
          contextIndexDltContext = Dlt_NvData.logInfo[swcContextindex].contextIndexInDltContext;

          /*@ assert appIndexDltContext < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
          /*@ assert contextIndexDltContext < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

          /* #50 If the configured Dlt contexts and the persisted are at the same indexes, the configuration did not
           *       change. Therefore, the persisted data can be used to overwrite the configured data. */
          if ((Dlt_NvData.logInfo[swcContextindex].appId == Dlt_Context.appIds[appIndexDltContext].appId)
           && (Dlt_NvData.logInfo[swcContextindex].contextId == Dlt_Context.appIds[appIndexDltContext].contextInfoList[contextIndexDltContext].contextId))
          {
            uint32 logChannelIndex;

            Dlt_Context.appIds[appIndexDltContext].contextInfoList[contextIndexDltContext].logLevel = Dlt_NvData.logInfo[swcContextindex].logLevel;
            Dlt_Context.appIds[appIndexDltContext].contextInfoList[contextIndexDltContext].traceStatus = Dlt_NvData.logInfo[swcContextindex].traceStatus;

            for (logChannelIndex = 0u; logChannelIndex < Dlt_GetNumberOfAllValidLogChannels(); logChannelIndex++)
            {
              Dlt_Context.appIds[appIndexDltContext].contextInfoList[contextIndexDltContext].logChannelIndexes[logChannelIndex] = Dlt_NvData.logInfo[swcContextindex].logChannelIndexes[logChannelIndex];
            }
          }
          /* #60 Otherwise, the configured data (application id or context id) changed and do not match the persisted
           *       data anymore. Therefore, the persisted data is invalidated. */
          else
          {
            retVal = DLT_E_ERROR;
            (void)NvM_InvalidateNvBlock(Dlt_GetNvDataSnv());
            break;
          }
        }
      }

      /* #70 If the restoring succeeded so far, set the log channel info as persisted. */
      if (retVal == E_OK)
      {
        uint32_least logChannelIndex;

        for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
        {
          /* #80 If the log channel name has not changed during configuration time, set the log channel specific
           *       threshold and trace status.
           *     Otherwise, invalidate the NvM block. */
          if (Dlt_LogChannelInfo[logChannelIndex].logChannelId == Dlt_NvData.logChannelInfo[logChannelIndex].logChannelId)
          {
            Dlt_LogChannelInfo[logChannelIndex].logChannelThreshold = Dlt_NvData.logChannelInfo[logChannelIndex].logChannelThreshold;
            Dlt_LogChannelInfo[logChannelIndex].logChannelTraceStatus = Dlt_NvData.logChannelInfo[logChannelIndex].logChannelTraceStatus;
          }
          else
          {
            (void)NvM_InvalidateNvBlock(Dlt_GetNvDataSnv());
            break;
          }
        }
      }
    }
    /* #90 Otherwise, invalidate the persisted data. */
    /* The reason for invalidation is, that the persisted structure was changed in the configuration, it does not match
     * the persisted structure anymore. */
    else
    {
      (void)NvM_InvalidateNvBlock(Dlt_GetNvDataSnv());
    }
  }
  /* #100 Otherwise, do nothing here, all values are already set to the configured values and the NvM seems to be
   *       invalidated already. */
} /* Dlt_Init_CheckAndSetPersistedData */

# endif /* (DLT_NVRAM_SUPPORT == STD_ON) */

/* ####################################################################################################################
 *   TTTTTTTTTT  XX      XX        MM      MM    SSSSSS      GGGGGG
 *       TT       XX    XX         MMMM  MMMM  SS      SS  GG      GG
 *       TT        XX  XX          MM  MM  MM  SS          GG
 *       TT          XX      ====  MM  MM  MM    SSSSSS    GG    GGGG
 *       TT        XX  XX          MM      MM          SS  GG      GG
 *       TT       XX    XX         MM      MM  SS      SS  GG      GG
 *       TT      XX      XX        MM      MM    SSSSSS      GGGGGGGG
 * ####################################################################################################################
 */
/**********************************************************************************************************************
 * Dlt_TxMsg_Transmit
 *********************************************************************************************************************/
/*!
 *
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
 */
/* PRQA S 6010 5 */ /* MD_Dlt_STPTH_Extended */
/* PRQA S 6030 4 */ /* MD_MSR_STCYC */
/* PRQA S 6050 3 */ /* MD_MSR_STCAL */
/* PRQA S 6060 2 */ /* MD_MSR_STPAR */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DLT_LOCAL FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_Transmit(
  P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_APPL_VAR) FilterInfo,
  uint16 PayloadLength,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) Payload)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  uint16 appIdIndex = 0u;
  uint16 conIdIndex = 0u;
  Dlt_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If the message is valid: */
  retVal = Dlt_TxMsg_IsMessageValid(FilterInfo, &appIdIndex, &conIdIndex);

  /*@ assert appIdIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
  /*@ assert conIdIndex < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

  if (retVal == (Dlt_ReturnType)DLT_E_OK)
  {
    boolean didAllLogChannelRejectTheMessageDueToDebugMode = TRUE;
    boolean didAllLogChannelRejectTheMessageDueToThreshold = TRUE; /* PRQA S 2995 3 */ /* MD_Dlt_AlwaysTrue */
    boolean didAllLogChannelRejectTheMessageDueToNoBuffer = TRUE;
    boolean didAllLogChannelRejectTheMessageDueToMessageTooLarge = TRUE;

    /* #15 For each log channel: */
    for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
    {
      /* #20 If buffering is currently allowed: */
      if (Dlt_Hlp_IsBufferingAllowed(logChannelIndex) == TRUE)
      {
        didAllLogChannelRejectTheMessageDueToDebugMode = FALSE;

        if (Dlt_Context.appIds[appIdIndex].contextInfoList[conIdIndex].logChannelIndexes[logChannelIndex] < Dlt_GetSizeOfLogChannelDescriptor())
        {
          /* #30 If the message is valid, build the frame. */
          retVal = Dlt_TxMsg_IsMessageValidOnLogChannel(logChannelIndex, FilterInfo);

          if (retVal == (Dlt_ReturnType)DLT_E_OK) /* COV_DLT_LOG_CHNL_OPTION */
          {
            uint16 locLength = 4u;
            uint8 verboseMode;
            didAllLogChannelRejectTheMessageDueToThreshold = FALSE;

            /* #40 Check if transmit of log message or trace message is requested and get according message option. */
            if (FilterInfo->messageType == (Dlt_MessageTypeType)DLT_TYPE_LOG)
            {
              verboseMode = (uint8)(FilterInfo->logInfo.options & DLT_VERBOSE_MSG);
            }
            else
            {
              verboseMode = (uint8)(FilterInfo->traceInfo.options & DLT_VERBOSE_MSG);
            }

            /* #50 Initialize temp data buffer. */
            VStdLib_MemClr(&Dlt_TransmitMessage[logChannelIndex][0], DLT_MAX_MESSAGE_LENGTH); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

            /* #60 Set flags of Standard Header. */
            Dlt_TxMsg_SetStandardHeader(FilterInfo, &Dlt_TransmitMessage[logChannelIndex][0], &locLength);

            /* #70 If the extended header has to be used for this message:
             *       Add the extended header to the frame and set verbose mode flag in message if active.
             */
            if (verboseMode == DLT_VERBOSE_MSG)
            {
              Dlt_TransmitMessage[logChannelIndex][0] |= (uint8)Dlt_StandardHeaderType_UEH_Mask;
              Dlt_TransmitMessage[logChannelIndex][locLength] |= (uint8)Dlt_ExtendedHeaderType_VERB_Mask;
              retVal = Dlt_TxMsg_SetExtendedHeader(FilterInfo, &Dlt_TransmitMessage[logChannelIndex][0], &locLength, appIdIndex, conIdIndex);
            }
# if (DLT_USE_EXT_HEADER_IN_NONVERBOSE_MODE == STD_ON)
            else
            {
              Dlt_TransmitMessage[logChannelIndex][0] |= (uint8)Dlt_StandardHeaderType_UEH_Mask;
              retVal = Dlt_TxMsg_SetExtendedHeader(FilterInfo, &Dlt_TransmitMessage[logChannelIndex][0], &locLength, appIdIndex, conIdIndex);
            }
# endif

            Dlt_TransmitMessage[logChannelIndex][0] |= (uint8)(Dlt_StandardHeaderType_SoftwareVersion << DLT_SHIFT_5BIT);

            /* #80 If header length and payload exceed max message length, return error invalid message length. */
            if (((uint32)((uint32)PayloadLength + (uint32)locLength)) > Dlt_GetLogChannelMaxMessageLengthOfLogChannelDescriptor(logChannelIndex))
            {
              retVal = (Dlt_ReturnType)DLT_E_MSG_TOO_LARGE;
            }
            else
            {
              didAllLogChannelRejectTheMessageDueToMessageTooLarge = FALSE;
            }

            /* #90 If the message is valid, set the all left data in frame and copy it to send buffer. */
            if (retVal == (Dlt_ReturnType)DLT_E_OK)
            {
              VStdLib_MemCpy(&Dlt_TransmitMessage[logChannelIndex][locLength], Payload, PayloadLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_VALID_MEMCOPY_DSTPTR */

              Dlt_TransmitMessage[logChannelIndex][1] = Dlt_MessageCounter[logChannelIndex];
              Dlt_MessageCounter[logChannelIndex]++;

              locLength = locLength + PayloadLength;
              Dlt_TransmitMessage[logChannelIndex][2] = (uint8)(locLength >> DLT_SHIFT_8BIT);
              Dlt_TransmitMessage[logChannelIndex][3] = (uint8)(locLength);

              retVal = Dlt_TxBuf_WriteToBuf(logChannelIndex, Dlt_TransmitMessage[logChannelIndex], locLength);

              /* #100 If at least one log channel buffer could cache the message:
               *       Remember the success.
               *       If at least one log channel accept the message, remember the success. */
              if (retVal != DLT_E_NO_BUFFER)
              {
                didAllLogChannelRejectTheMessageDueToNoBuffer = FALSE;
              }
            }
          }
        }
      }
    }

    /* #110 If all log channels reject the message due to the debug mode, return DLT_E_OK.
     *      Otherwise if all log channels reject the message due to threshold/trace status, return DLT_E_OK.
     *      Otherwise if all log channels reject the message due to a too long message, return DLT_E_MSG_TOO_LARGE.
     *      Otherwise if all log channels reject the message due to not enough buffer, return DLT_E_NO_BUFFER.
     *      Otherwise, return DLT_E_OK. */
    if (didAllLogChannelRejectTheMessageDueToDebugMode == TRUE)
    {
      retVal = DLT_E_OK;
    }
    else
    if (didAllLogChannelRejectTheMessageDueToThreshold == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
    {
      retVal = DLT_E_OK;
    }
    else
    if (didAllLogChannelRejectTheMessageDueToMessageTooLarge == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
    {
      retVal = DLT_E_MSG_TOO_LARGE;
    }
    else
    if (didAllLogChannelRejectTheMessageDueToNoBuffer == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
    {
      retVal = DLT_E_NO_BUFFER;
    }
    else
    {
      retVal = DLT_E_OK;
    }
  }

  /* #120 If the message was filtered because of threshold or trace status, return DLT_E_OK. */
  if (retVal == DLT_E_MESSAGE_REJECTED)
  {
    retVal = DLT_E_OK;
  }

  return retVal;
} /* Dlt_TxMsg_Transmit() */

/**********************************************************************************************************************
 * Dlt_TxMsg_IsMessageValid
 *********************************************************************************************************************/
/*!
 *
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DLT_LOCAL_INLINE FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_IsMessageValid(
  P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_APPL_VAR) FilterInfo,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) AppIdIndex,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) ContextIdIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retValue;
  boolean applAlreadyReg = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
  boolean isSessionIdValid;
  Dlt_SessionIDType SessionId = FilterInfo->sessionId;
  Dlt_LocalApplicationIDType ApplicationlId;
  Dlt_LocalContextIDType ContextId;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Get the message infos depending on message type (specified by the called Dlt API). */
  if (FilterInfo->messageType == (Dlt_MessageTypeType)DLT_TYPE_LOG)
  {
    ApplicationlId = FilterInfo->logInfo.appId;
    ContextId = FilterInfo->logInfo.contextId;
  }
  else
  {
    ApplicationlId = FilterInfo->traceInfo.appId;
    ContextId = FilterInfo->traceInfo.context;
  }

  /* #20 If the context is not registered, reject message. */
  isSessionIdValid = Dlt_Hlp_CheckSessionId(SessionId, ApplicationlId, ContextId, AppIdIndex, ContextIdIndex, &applAlreadyReg);

  /*@ assert *AppIdIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
  /*@ assert *ContextIdIndex < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

  if (isSessionIdValid == FALSE) /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
  {
    retValue = (Dlt_ReturnType)DLT_E_UNKNOWN_SESSION_ID;
  }
  /* #30 Otherwise, if the message type is log, check the log specific options. */
  else
  if (FilterInfo->messageType == (Dlt_MessageTypeType)DLT_TYPE_LOG)
  {
    /* The message type from application is placed in bits 1..3 of options, therefore right shift by 1. */
    uint8 dltMessageType = (uint8)((FilterInfo->logInfo.options >> 1u) & Dlt_InternalMessageTypeTypeMask);

    /* #40 If the application did not set the message type to log, but called the SendLogMessage API:
     *       Reject the message. */
    if (dltMessageType != (uint8)DLT_TYPE_LOG)
    {
      retValue = (Dlt_ReturnType)DLT_E_ERROR;
    }
    /* #50 Otherwise, if filtering is enabled and the log level of the message has lower priority than minimal allowed, reject message.  */
    /* Reject the message if the log level is higher than the allowed log level, reject message. */
    else
    if ((Dlt_FilterMessages == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
     && (Dlt_Context.appIds[*AppIdIndex].contextInfoList[*ContextIdIndex].logLevel < (sint8)FilterInfo->logInfo.logLevel))
    {
      retValue = (Dlt_ReturnType)DLT_E_MESSAGE_REJECTED;
    }
    /* #60 Otherwise, the message is valid. */
    else
    {
      retValue = (Dlt_ReturnType)DLT_E_OK;
    }
  }
  /* #70 Otherwise, if the message type is trace, check the trace specific options. */
  else
  if ((FilterInfo->messageType == (Dlt_MessageTypeType)DLT_TYPE_APP_TRACE)
   || (FilterInfo->messageType == (Dlt_MessageTypeType)DLT_TYPE_NW_TRACE)) /* COV_DLT_TXMSG_MSGTYPE_VALID */
  {
    /* #80 If filtering is enabled and the trace status of message is set to off, reject message. */
    /* Reject the message if the log level is higher than the allowed log level. */
    if ((Dlt_FilterMessages == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
     && (Dlt_Context.appIds[*AppIdIndex].contextInfoList[*ContextIdIndex].traceStatus == (sint8)FALSE)) /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
    {
      retValue = (Dlt_ReturnType)DLT_E_MESSAGE_REJECTED;
    }
    /* #90 Otherwise, the message is valid. */
    else
    {
      retValue = (Dlt_ReturnType)DLT_E_OK;
    }
  }
  /* #100 Otherwise, the message is invalid. */
  else
  {
    retValue = (Dlt_ReturnType)DLT_E_ERROR;
  }

  return retValue;
} /* Dlt_TxMsg_IsMessageValid() */

/**********************************************************************************************************************
 * Dlt_TxMsg_IsMessageValidOnLogChannel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_IsMessageValidOnLogChannel(
  uint32 LogChannelIndex,
  P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_APPL_VAR) FilterInfo)
{
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retVal = DLT_E_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Check if the message may be send on this specific log channel. */
  if (FilterInfo->messageType == (Dlt_MessageTypeType)DLT_TYPE_LOG)
  {
    if (Dlt_LogChannelInfo[LogChannelIndex].logChannelThreshold < FilterInfo->logInfo.logLevel) /* COV_DLT_LOG_CHNL_OPTION */
    {
      retVal = DLT_E_MESSAGE_REJECTED;
    }
  }
  else
  {
    if (Dlt_LogChannelInfo[LogChannelIndex].logChannelTraceStatus == FALSE) /* COV_DLT_LOG_CHNL_OPTION */
    {
      retVal = DLT_E_MESSAGE_REJECTED;
    }
  }

  return retVal;
} /* Dlt_TxMsg_IsMessageValidOnLogChannel() */

/**********************************************************************************************************************
 * Dlt_TxMsg_SetStandardHeader
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxMsg_SetStandardHeader(
  P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_APPL_VAR) FilterInfo,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) Data,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) DataIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
# if (DLT_HEADER_USE_ECU_ID == STD_ON)
  uint32 ecuID = (uint32)DLT_ECU_ID;
# endif
# if (DLT_TIMESTAMP_SUPPORT == STD_ON)
  uint32 timestamp = 0u;

  /* #10 If Dlt global state is DLT_GLOBAL_STATE_ONLINE, the used time source is initialized and returns valid value. > SPEC-42734 */
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
  {
    timestamp = Dlt_GetCurrentTime();
  }
# endif

  /* ----- Implementation ------------------------------------------------- */
  /* Standard Header */
  /* #20 If the byte order is explicitly set to big endian or the platform endianness is big endian, then set the MSBF flag. */
  if (FilterInfo->usedEndianness == DLT_BIGENDIAN)
  {
    Data[0] |= (uint8)Dlt_StandardHeaderType_MSBF_Mask;
  }

  /* #30 If ecu ID transmission is enabled, set the according flag in message and the add the ecu ID. */
# if (DLT_HEADER_USE_ECU_ID == STD_ON)
  if (Dlt_HeaderUseEcuId == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
  {
    Data[0] |= (uint8)Dlt_StandardHeaderType_WEID_Mask;
    Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(ecuID, &Data[(*DataIndex)], DataIndex);
  }
# endif
  /* #40 If the session ID is globally enabled, set the according flag in message and add the session ID. */
  if (Dlt_HeaderUseSessionId == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
  {
    Data[0] |= (uint8)Dlt_StandardHeaderType_WSID_Mask;
    Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(FilterInfo->sessionId, &Data[(*DataIndex)], DataIndex);
  }
  /* #50 If timestamp is enabled, set the according flag in message and add the timestamp. */
# if (DLT_TIMESTAMP_SUPPORT == STD_ON)
  if (Dlt_HeaderUseTimestamp == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
  {
    Data[0] |= (uint8)Dlt_StandardHeaderType_WTMS_Mask;
    Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(timestamp, &Data[(*DataIndex)], DataIndex);
  }
# endif
} /* Dlt_TxMsg_SetStandardHeader */

/**********************************************************************************************************************
 * Dlt_TxMsg_SetExtendedHeader
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxMsg_SetExtendedHeader(
  P2CONST(Dlt_FilterInfoType, AUTOMATIC, DLT_APPL_VAR) FilterInfo,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) Data,
  P2VAR(uint16, AUTOMATIC, DLT_APPL_VAR) DataIndex,
  uint16 AppIdIndex,
  uint16 ConIdIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 index = *DataIndex;
  Dlt_ReturnType retValue = (Dlt_ReturnType)DLT_E_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If the message type is log, set the log specific options in message */
  if (FilterInfo->messageType == (Dlt_MessageTypeType)DLT_TYPE_LOG)
  {
    /* Setting the DLT_TYPE_LOG to Data[index] not required, because value is 0u. */
    Data[index]      |= (uint8)(FilterInfo->logInfo.logLevel << 4u);
    Data[index + 1u]  = (uint8)(FilterInfo->logInfo.argCount); /* Option: is used as Number Of Arguments */
  }
  /* #20 Otherwise, if the message type is application trace or network trace, set the trace specific options in message */
  else
  if ((FilterInfo->messageType == (Dlt_MessageTypeType)DLT_TYPE_APP_TRACE)
   || (FilterInfo->messageType == (Dlt_MessageTypeType)DLT_TYPE_NW_TRACE)) /* COV_DLT_TXMSG_MSGTYPE_VALID */
  {
    Data[index]      |= (uint8)(FilterInfo->messageType << 1u);
    Data[index]      |= (uint8)(FilterInfo->traceInfo.traceInfo << 4u); /* Option: is used as Dlt_MessageTraceType (DLT_TRACE_VARIABLE|DLT_TRACE_FUNCTION_IN|...) */
    Data[index + 1u]  = (uint8)(0u); /* Number of arguments = 0, no info in Dlt_SendTraceMessage */
  }
  /* #30 Otherwise, the message is invalid. */
  else
  {
    retValue = (Dlt_ReturnType)DLT_E_ERROR;
  }

  index = index + 2u;

  /* #40 Add application ID and context ID to frame. */
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex((uint32)Dlt_Context.appIds[AppIdIndex].appId, &Data[index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex((uint32)Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].contextId, &Data[index], &index);

  *DataIndex = index;

  return retValue;
} /* Dlt_TxMsg_SetExtendedHeader */

/* ####################################################################################################################
 *   TTTTTTTTTT  XX      XX        BBBBBBBB    UU      UU  FFFFFFFFFF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT          XX      ====  BBBBBBBB    UU      UU    FFFFFF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT        XX  XX          BB      BB  UU      UU      FF
 *       TT      XX      XX        BBBBBBBB      UUUUUU        FF
 * ####################################################################################################################
 */
/**********************************************************************************************************************
 * Dlt_TxBuf_WriteToBuf
 *********************************************************************************************************************/
/*!
 *
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
DLT_LOCAL FUNC(Dlt_ReturnType, DLT_CODE) Dlt_TxBuf_WriteToBuf(
  uint32 LogChannelIndex,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) Data,
  uint16 DataLength)
{
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retVal;
  uint32_least i;
  uint32 bufferIndex = Dlt_SendBuffer[LogChannelIndex].Write;
  boolean bufferOverflowDetected = FALSE;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Copy message to send buffer.
   *       If send buffer is full:
   *         If the flag for buffer overflow is not yet set, set it and start the buffer overflow timer.
   *         Increment the buffer overflow counter.
   */
  for (i = 0u; (i < (uint32)DataLength) && (bufferOverflowDetected == FALSE); i++)
  {
    /*@ assert bufferIndex < DLT_MESSAGE_BUFFER_SIZE; */ /* VCA_DLT_VALID_SEND_BUFFER_IDX */
    Dlt_SendBuffer[LogChannelIndex].Buffer[bufferIndex] = Data[i];
    bufferIndex++;
    if (bufferIndex >= (uint32)Dlt_GetLogChannelBufferSizeOfLogChannelDescriptor(LogChannelIndex))
    {
      bufferIndex = 0u;
    }
    if (bufferIndex == Dlt_SendBuffer[LogChannelIndex].Read)
    {
      if (Dlt_BufferOverflowNotificationStatus[LogChannelIndex].Flag == FALSE)
      {
        Dlt_BufferOverflowNotificationStatus[LogChannelIndex].Flag = TRUE;
        Dlt_BufferOverflowNotificationStatus[LogChannelIndex].Timer = Dlt_GetLogChannelBufferOverflowTimerOfLogChannelDescriptor(LogChannelIndex);
      }
      Dlt_BufferOverflowNotificationStatus[LogChannelIndex].Counter++;
      bufferOverflowDetected = TRUE;
    }
  }

  /* #20 If overflow was detected, return error ID.
   *       If debug mode is event based and buffering is now done, set debug event to sending.
   *     Otherwise, update write pointer position of send buffer and return OK.
   */
  if (bufferOverflowDetected == TRUE)
  {
    retVal = (Dlt_ReturnType)DLT_E_NO_BUFFER;

    if (Dlt_DebugMode[LogChannelIndex] == DLT_DEBUGMODE_ONEVENT_FIRSTISBEST)
    {
      Dlt_DebugEventState[LogChannelIndex] = DLT_DEBUGEVENT_SENDING_ACTIVE;
    }
  }
  else
  {
    Dlt_SendBuffer[LogChannelIndex].Write = bufferIndex;
    retVal = (Dlt_ReturnType)DLT_E_OK;
  }

  return retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_TxBuf_WriteToBuf */

/**********************************************************************************************************************
 * Dlt_TxBuf_WriteToCtrlBuf
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_WriteToCtrlBuf(uint32 LogChannelIndex, uint16 Length)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32_least  i;
  uint32  bufferIndex = Dlt_ControlSendBuffer[LogChannelIndex].Write;
  boolean bufferOverflowHappened = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Copy response control message to control buffer until completed or buffer overflow. */
  for (i = 0u; (i < Length) && (bufferOverflowHappened == FALSE); i++) /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
  {
    /*@ assert bufferIndex < DLT_CONTROL_BUFFER_SIZE; */ /* VCA_DLT_VALID_CTRL_BUFFER_IDX */
    Dlt_ControlSendBuffer[LogChannelIndex].Buffer[bufferIndex] = Dlt_ControlSendMessage[LogChannelIndex][i];
    bufferIndex++;
    if (bufferIndex >= (uint32)DLT_CONTROL_BUFFER_SIZE) /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
    {
      bufferIndex = 0u;
    }
    if (bufferIndex == Dlt_ControlSendBuffer[LogChannelIndex].Read) /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
    {
      bufferOverflowHappened = TRUE;
    }
  }

  /* #20 If copy process succeeded, update write pointer position of control buffer. */
  if (bufferOverflowHappened == FALSE) /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */ /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
  {
    Dlt_ControlSendBuffer[LogChannelIndex].Write = bufferIndex;
  }
} /* Dlt_TxBuf_WriteToCtrlBuf */

/**********************************************************************************************************************
 * Dlt_TxBuf_GetLengthOfNextMsg
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_TxBuf_GetLengthOfNextMsg(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceBuffer,
  uint32 ReadIndex,
  uint32 WriteIndex,
  uint32 BufferSize)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 lengthOfTxMsg = 0u;
  uint32 distanceBetweenReadAndWriteIndex;

  /* ----- Implementation ------------------------------------------------- */
  if (ReadIndex <= WriteIndex)
  {
    distanceBetweenReadAndWriteIndex = WriteIndex - ReadIndex;
  }
  else
  {
    distanceBetweenReadAndWriteIndex = ((uint32)BufferSize - ReadIndex) + WriteIndex;
  }

  /* #10 If the Tx buffer contains more than 3 bytes:
   *       Get the length of the first DLT message in the SourceBuffer depending on the current read index position.
   *       If the read index is out of range, indicate end of messages. */
  if (distanceBetweenReadAndWriteIndex >= 4u)
  {
    if ((ReadIndex + 3u) < BufferSize)
    {
      lengthOfTxMsg  = (uint16)((uint16)((uint8)SourceBuffer[ReadIndex + 2u]) << DLT_SHIFT_8BIT);
      lengthOfTxMsg |= (uint16)((uint16)((uint8)SourceBuffer[ReadIndex + 3u]));
    }
    else if ((ReadIndex + 2u) < BufferSize)
    {
      lengthOfTxMsg  = (uint16)((uint16)((uint8)SourceBuffer[ReadIndex + 2u]) << DLT_SHIFT_8BIT);
      lengthOfTxMsg |= (uint16)((uint16)((uint8)SourceBuffer[0]));
    }
    else if ((ReadIndex + 1u) < BufferSize)
    {
      lengthOfTxMsg  = (uint16)((uint16)((uint8)SourceBuffer[0]) << DLT_SHIFT_8BIT);
      lengthOfTxMsg |= (uint16)((uint16)((uint8)SourceBuffer[1]));
    }
    else /* if ((ReadIndex) < BufferSize) */
    {
      lengthOfTxMsg  = (uint16)((uint16)((uint8)SourceBuffer[1]) << DLT_SHIFT_8BIT);
      lengthOfTxMsg |= (uint16)((uint16)((uint8)SourceBuffer[2]));
    }
  }

  /* #20 If the message is not completely in the buffer, indicate that no more messages are available. */

  return lengthOfTxMsg;
} /* Dlt_TxBuf_GetLengthOfNextTxMsg */

/**********************************************************************************************************************
 * Dlt_TxBuf_CalculateMessageLength
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(uint32, DLT_CODE) Dlt_TxBuf_CalculateMessageLength(
  uint32 LogChannelIndex,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceBuffer,
  uint32 ReadIndex,
  uint32 WriteIndex,
  uint32 BufferSize)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 completeMsgLength = 0u;
  uint32 locReadIndex = ReadIndex;
  uint16 singleDltMsgLength;
  uint16_least numberOfSingleDltMsg;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Calculate the maximum length possible to be transmitted at once without sending DLT messages partially. */
  for (numberOfSingleDltMsg = 0u;
       (numberOfSingleDltMsg < DLT_MAX_DLT_MESSAGES_IN_ONE_TRANSMIT);
       numberOfSingleDltMsg++) /* COV_DLT_MAX_MESSAGES_IN_ONE_TRANSMIT_TOO_GREAT_FOR_BUFFER */
  {
    uint32 newLength;
    const PduLengthType loLayerBufferSize = Dlt_GetLowerLayerBufferSizeOfLogChannelDescriptor(LogChannelIndex);

    singleDltMsgLength = Dlt_TxBuf_GetLengthOfNextMsg(SourceBuffer, locReadIndex, WriteIndex, BufferSize);

    newLength = completeMsgLength + (uint32)singleDltMsgLength;

    /* #20 If a next message is available
     *     AND if there is still enough buffer in the lower layer
     *     AND if the maximum of bytes to be transmitted in one cycle is not exceeded:
     *       Add the next message to transmit data and move the buffer index.
     *     Otherwise, stop iterating over the send buffer. */
    if ((singleDltMsgLength > 0u)
     && (newLength <= loLayerBufferSize)
# if (DLT_TRAFFIC_SHAPING_SUPPORT == STD_ON)
     && (newLength <= Dlt_GetMaxBytesPerMainFunctionToTransmitOfLogChannelDescriptor(LogChannelIndex))
# endif
       )
    {
      completeMsgLength += (uint32)singleDltMsgLength;
      locReadIndex += (uint32)singleDltMsgLength;

      if (locReadIndex >= BufferSize)
      {
        locReadIndex = locReadIndex - BufferSize;
      }
    }
    else
    {
      break;
    }
  }

  return completeMsgLength;
} /* Dlt_TxBuf_CalculateMessageLength */

/**********************************************************************************************************************
 * Dlt_TxBuf_SetReadIndex
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_SetReadIndex(
  uint32 LogChannelIndex,
  boolean setControlBuffer,
  uint32 bufferIdx)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Update read pointer of control buffer or send buffer. Decided by caller. */
  if (setControlBuffer == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
  {
    Dlt_ControlSendBuffer[LogChannelIndex].Read = bufferIdx;
  }
  else
  {
    Dlt_SendBuffer[LogChannelIndex].Read = bufferIdx;
  }
} /* Dlt_TxBuf_SetReadIndex */

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)

/**********************************************************************************************************************
 * Dlt_TxBuf_PrepareRespInCtrlBuf
 *********************************************************************************************************************/
/*!
 *
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
 */
DLT_LOCAL_INLINE FUNC(uint16, DLT_CODE) Dlt_TxBuf_PrepareRespInCtrlBuf(
  uint32 LogChannelIndex,
  uint16 PayloadLength)
{ /* Fill in fixed data */
  /* ----- Local Variables ------------------------------------------------ */
  uint16 indexInMessages = DLT_STD_HEADER_MANDATORY_END_INDEX;
  uint16 controlMessageLength = PayloadLength;
  uint32 ecuID = (uint32)DLT_ECU_ID;
  uint32 timestamp;
  uint16 indexOfPayloadInControl;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If timestamp is enabled, get the current time. */
  timestamp = Dlt_GetCurrentTime();

  /* #20 If received control message is in big endian:
   *       Send response also in big endian.
   *     Otherwise:
   *       Send response in little endian.
   */
  if (Dlt_IsPayloadInBigEndian())
  {
    Dlt_ControlSendMessage[LogChannelIndex][DLT_STD_HEADER_HEADERTYPE_INDEX] = Dlt_ControlMessageHeaderType_MSBF; /* Header type - 0b001 0 0 0 1 1 - Version 3 | WTMS 0 | SID 0 | WEID 0 | MSBF 1 | UEH 1 = 0x23 */
  }
  else
  {
    Dlt_ControlSendMessage[LogChannelIndex][DLT_STD_HEADER_HEADERTYPE_INDEX] = Dlt_ControlMessageHeaderType_LSBF; /* Header type - 0b001 0 0 0 0 1 - Version 3 | WTMS 0 | SID 0 | WEID 0 | MSBF 0 | UEH 1 = 0x21 */
  }

  /* #30 Set current message counter value to response message. */
  Dlt_ControlSendMessage[LogChannelIndex][DLT_STD_HEADER_MESSAGECOUNTER_INDEX] = Dlt_MessageCounter[LogChannelIndex];

  /* #40 If the ecu id is set in the request, set it also in the response. */
  if ((Dlt_ReceiveMessage[DLT_STD_HEADER_HEADERTYPE_INDEX] & Dlt_StandardHeaderType_WEID_Mask) == Dlt_StandardHeaderType_WEID_Mask)
  {
    Dlt_ControlSendMessage[LogChannelIndex][DLT_STD_HEADER_HEADERTYPE_INDEX] |= Dlt_StandardHeaderType_WEID_Mask;

    Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(ecuID, &Dlt_ControlSendMessage[LogChannelIndex][indexInMessages], &indexInMessages);
  }

  /* #50 If the session id is set in the request, set it also in the response. */
  if ((Dlt_ReceiveMessage[DLT_STD_HEADER_HEADERTYPE_INDEX] & Dlt_StandardHeaderType_WSID_Mask) == Dlt_StandardHeaderType_WSID_Mask)
  {
    Dlt_ControlSendMessage[LogChannelIndex][DLT_STD_HEADER_HEADERTYPE_INDEX] |= Dlt_StandardHeaderType_WSID_Mask;

    VStdLib_MemCpy(&Dlt_ControlSendMessage[LogChannelIndex][indexInMessages], &Dlt_ReceiveMessage[indexInMessages], DLT_DEFAULT_ID_LENGTH); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
    indexInMessages += DLT_DEFAULT_ID_LENGTH;
  }

  /* #60 If the timestamp is set in the request, set it also in the response. */
  if ((Dlt_ReceiveMessage[DLT_STD_HEADER_HEADERTYPE_INDEX] & Dlt_StandardHeaderType_WTMS_Mask) == Dlt_StandardHeaderType_WTMS_Mask)
  {
    Dlt_ControlSendMessage[LogChannelIndex][DLT_STD_HEADER_HEADERTYPE_INDEX] |= Dlt_StandardHeaderType_WTMS_Mask;

    Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(timestamp, &Dlt_ControlSendMessage[LogChannelIndex][indexInMessages], &indexInMessages);
  }

  /* #70 Set message info to response message, control message and non-verbose mode. */
  Dlt_ControlSendMessage[LogChannelIndex][indexInMessages] = Dlt_ControlMessageMessageInfo; /* 0b0010 011 0 - Response 2 | Control 3 | Non-Verbose 0 */

  indexInMessages += 1u;
  Dlt_ControlSendMessage[LogChannelIndex][indexInMessages] = 0u; /* 0b0010 011 0 - Response 2 | Control 3 | Non-Verbose 0 */


  /* #80 Copy application ID, context ID, and service ID from received message to response message. */
  indexInMessages += 1u;
  VStdLib_MemCpy(&Dlt_ControlSendMessage[LogChannelIndex][indexInMessages], &Dlt_ReceiveMessage[indexInMessages], 12u); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

  /* #90 Set length of standard header, extended header, service id, and rest of payload in response message. */
  indexOfPayloadInControl = indexInMessages + 12u;
  controlMessageLength = controlMessageLength + indexOfPayloadInControl;
  indexInMessages = DLT_STD_HEADER_LENGTH_INDEX;

  Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex(controlMessageLength, &Dlt_ControlSendMessage[LogChannelIndex][indexInMessages], &indexInMessages);

  Dlt_MessageCounter[LogChannelIndex]++;

  return indexOfPayloadInControl;
} /* Dlt_TxBuf_PrepareRespInCtrlBuf */

/**********************************************************************************************************************
 * Dlt_TxBuf_GetEcuIdAndService
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_GetEcuIdAndService(
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) EcuId,
  P2VAR(uint32, AUTOMATIC, DLT_APPL_VAR) ServiceId)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 index = 14u; /* = HTYP(1Byte) + MCNT(1Byte) + LEN(2Byte) + MSIN(1Byte) + NOAR(1Byte) + APID(4Byte) + CTID(4Byte) */

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Get index of Service ID from receive buffer. */
  Dlt_Hlp_IncrementIndexDependingOnOptionalStandardHeader(&index);

  /* #20 Get Service ID from received message, depending on endianness. */
  if (Dlt_IsPayloadInBigEndian())
  { /* Message in big endian */
    Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex(&Dlt_ReceiveMessage[0], ServiceId, &index);
  }
  else
  { /* Message in little endian */
    Dlt_Hlp_CopyArrayToLongLittleEndian_AndMoveIndex(&Dlt_ReceiveMessage[0], ServiceId, &index);
  }

  /* #30 Get ECU ID from received message (header is always in big endian). */
  if ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_WEID_Mask) == Dlt_StandardHeaderType_WEID_Mask)
  {
    index = 4u;
    Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex(&Dlt_ReceiveMessage[0], EcuId, &index);
  }
} /* Dlt_TxBuf_GetEcuIdAndService */
# endif

/**********************************************************************************************************************
 * Dlt_TxBuf_WriteBufToTxMsg
 *********************************************************************************************************************/
/*!
 *
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
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_TxBuf_WriteBufToTxMsg(
  uint32 LogChannelIndex,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) SourceBuffer,
  uint32 ReadIndex,
  uint32 WriteIndex,
  boolean IsControlBuffer)
{
  /* ----- Local Variables ------------------------------------------------ */
  PduInfoType pduInfo;
  uint32 locLength;
  uint32 bufferIndex;
  uint32_least i;
  uint32 bufferSize;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Load buffer size, depending on which buffer is specified in IsControlBuffer. */
  if (IsControlBuffer == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
  {
    bufferSize = (uint32)DLT_CONTROL_BUFFER_SIZE;
  }
  else
  {
    bufferSize = (uint32)Dlt_GetLogChannelBufferSizeOfLogChannelDescriptor(LogChannelIndex);
  }

  /* #20 Get the number of bytes to send. */
  locLength = Dlt_TxBuf_CalculateMessageLength(LogChannelIndex, SourceBuffer, ReadIndex, WriteIndex, bufferSize);

  /*@ assert locLength <= DLT_MESSAGE_BUFFER_SIZE; */ /* VCA_DLT_VALID_MESSAGE_BUFFER_SIZE */

  bufferIndex = ReadIndex;
  /* #30 Depending on chosen buffer either content of control buffer or content of send buffer is copied to tx message. */
  for (i = 0u; i < locLength; i++)
  {
    Dlt_SendData[LogChannelIndex][i] = SourceBuffer[bufferIndex];
    bufferIndex++;
    if (bufferIndex >= bufferSize)
    {
      bufferIndex = 0u;
    }
  }

  /* #40 If there is a complete message to be transmitted:
   *       Request to send the tx message.
   */
  if (locLength > 0u)
  {
    Dlt_SendPdu_DataLength[LogChannelIndex] = (uint16)locLength;
    Dlt_SendPdu_DataPtr[LogChannelIndex] = Dlt_SendData[LogChannelIndex];
    pduInfo.SduLength = Dlt_SendPdu_DataLength[LogChannelIndex];
    pduInfo.SduDataPtr = Dlt_SendPdu_DataPtr[LogChannelIndex];

    /*@ assert pduInfo.SduDataPtr != NULL_PTR; */ /* VCA_DLT_VALID_DATA_PTR */
    retVal = PduR_DltTransmit(Dlt_GetLogChannelTxPduSNVOfPduROfLogChannelDescriptor(LogChannelIndex), &pduInfo);
  }

  /* #50 If the sending was successful:
   *       Depending on the specified buffer, the read pointer of control buffer or send buffer is set to new position.
   *     Otherwise, indicate that the transmission request was rejected (set state back to DLT_STATE_WAIT_FOR_TX_DATA). */
  if (retVal == E_OK)
  {
    /*@ assert (IsControlBuffer)?(bufferIndex < DLT_CONTROL_BUFFER_SIZE):(bufferIndex < DLT_MESSAGE_BUFFER_SIZE); */ /* VCA_DLT_VALID_READ_IDX */
    Dlt_TxBuf_SetReadIndex(LogChannelIndex, IsControlBuffer, bufferIndex);
  }
  else
  {
    Dlt_Sm_StateTransition(LogChannelIndex, kDltEventTransmitCallRejected);
  }
} /* Dlt_TxBuf_WriteBufToTxMsg */

/* ####################################################################################################################
 *     SSSSSS    MM      MM
 *   SS      SS  MMMM  MMMM
 *   SS          MM  MM  MM
 *     SSSSSS    MM  MM  MM
 *           SS  MM      MM
 *   SS      SS  MM      MM
 *     SSSSSS    MM      MM
 * ####################################################################################################################
 */
/**********************************************************************************************************************
 * Dlt_Sm_HasAnyBufferContentAndIsAllowedToSend
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Sm_HasAnyBufferContentAndIsAllowedToSend(uint32 LogChannelIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean retVal = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */

  /* #10 Indicate if any buffer has content and it is allowed to transmit this data. */
  if ((Dlt_Sm_HasReceiveOrControlBufferContent(LogChannelIndex) == TRUE)
   || (Dlt_Sm_HasSendBufferContentAndIsAllowedToSend(LogChannelIndex) == TRUE)
     ) /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
  {
    retVal = TRUE;
  }

  return retVal;
} /* Dlt_Sm_HasAnyBufferContentAndIsAllowedToSend */

/**********************************************************************************************************************
 * Dlt_Sm_HasReceiveOrControlBufferContent
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Sm_HasReceiveOrControlBufferContent(uint32 LogChannelIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean retVal = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */

  /* #10 Indicate if receive or control buffer has content. */
  if (((Dlt_ReceiveBuffer.Read != Dlt_ReceiveBuffer.Write)
    && (Dlt_RxInfo.RxIndicationOccured > 0u))
   || (Dlt_ControlSendBuffer[LogChannelIndex].Read != Dlt_ControlSendBuffer[LogChannelIndex].Write)
     ) /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
  {
    retVal = TRUE;
  }

  return retVal;
} /* Dlt_Sm_HasReceiveOrControlBufferContent */

/**********************************************************************************************************************
 * Dlt_Sm_HasSendBufferContentAndIsAllowedToSend
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(boolean, DLT_CODE) Dlt_Sm_HasSendBufferContentAndIsAllowedToSend(uint32 LogChannelIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  boolean retVal = Dlt_Hlp_IsSendingAllowed(LogChannelIndex); /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */

  /* #10 Indicate if send buffer has content and it is allowed to send its content. */
  if ((retVal == TRUE)
   && (Dlt_SendBuffer[LogChannelIndex].Read != Dlt_SendBuffer[LogChannelIndex].Write))
  {
    retVal = TRUE;
  }
  else
  {
    retVal = FALSE;
  }

  return retVal;
} /* Dlt_Sm_HasSendBufferContentAndIsAllowedToSend */

/**********************************************************************************************************************
 * Dlt_Sm_HandleSendingTransitions
 *********************************************************************************************************************/
/*!
 *
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
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Sm_HandleSendingTransitions(uint32 LogChannelIndex)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 If the message was successfully transmitted:
   *       Initialize control variables for successful transmission and timeout.
   *       If any buffer has still content and no new debug mode was requested:
   *         Requested to transmit this data.
   *       Otherwise, indicate that sending is finished.
   */
  if (Dlt_MessageTransmitted[LogChannelIndex] == TRUE)
  {
    boolean hasAnyBufferContent = Dlt_Sm_HasAnyBufferContentAndIsAllowedToSend(LogChannelIndex);

    Dlt_MessageTransmitted[LogChannelIndex] = FALSE;
    Dlt_Timeout[LogChannelIndex] = 0u;

    if (hasAnyBufferContent == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
    {
      Dlt_Sm_StateTransition(LogChannelIndex, kDltEventOneBufferHasContent);
    }
    else
    {
      Dlt_Sm_StateTransition(LogChannelIndex, kDltEventSendingFinished);

      if (Dlt_DebugEventState[LogChannelIndex] == DLT_DEBUGEVENT_SENDING_ACTIVE)
      {
        Dlt_DebugEventState[LogChannelIndex] = DLT_DEBUGEVENT_IDLE;
      }
    }
  }
  /* #20 Otherwise:
   *       Increase timeout counter.
   *       If timeout occurs:
   *         Initialize control variables for timeout and Rx indication.
   *         Transition from state DLT_STATE_SENDING to DLT_STATE_WAIT_FOR_TX_DATA.
   *       Otherwise, remain in DLT_STATE_SENDING.
   */
  else
  {
    Dlt_Timeout[LogChannelIndex]++;
    if (Dlt_Timeout[LogChannelIndex] >= DLT_TIMEOUT)
    {
      Dlt_Timeout[LogChannelIndex] = 0u;
      Dlt_RxInfo.RxIndicationOccured = 0u;
      Dlt_Sm_StateTransition(LogChannelIndex, kDltEventTimeout);
      Dlt_DebugEventState[LogChannelIndex] = DLT_DEBUGEVENT_IDLE;
    }
    else
    {
      Dlt_Sm_StateTransition(LogChannelIndex, kDltEventTxStillActive);
    }
  }
} /* Dlt_Sm_HandleSendingTransitions */

/**********************************************************************************************************************
 * Dlt_Sm_StateTransition
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_StateTransition(uint32 LogChannelIndex, const Dlt_EventType transitionEvent)
{
  /* ----- Local Variables ------------------------------------------------ */
  DLT_LOCAL Dlt_StateType const kDltStateMatrix[kDltStateMachineStates][kDltStateMachineEvents] =
  {
                       /*   kDltEventInitDlt,
                        *                               kDltEventOneBufferHasContent,
                        *                                                  kDltEventTransmitCallRejected,
                        *                                                                              kDltEventSendingFinished,
                        *                                                                                                          kDltEventTxStillActive,
                        *                                                                                                                                      kDltEventTimeout,
                        *                                                                                                                                                                  kDltEventDebugModeChanged */
  /* unint */             { DLT_STATE_WAIT_FOR_TX_DATA, DLT_STATE_UNINIT,  DLT_STATE_UNINIT,           DLT_STATE_UNINIT,           DLT_STATE_UNINIT,           DLT_STATE_UNINIT,           DLT_STATE_UNINIT           },
  /* wait for tx data */  { DLT_STATE_WAIT_FOR_TX_DATA, DLT_STATE_SENDING, DLT_STATE_WAIT_FOR_TX_DATA, DLT_STATE_WAIT_FOR_TX_DATA, DLT_STATE_WAIT_FOR_TX_DATA, DLT_STATE_WAIT_FOR_TX_DATA, DLT_STATE_WAIT_FOR_TX_DATA },
  /* sending */           { DLT_STATE_SENDING,          DLT_STATE_SENDING, DLT_STATE_WAIT_FOR_TX_DATA, DLT_STATE_WAIT_FOR_TX_DATA, DLT_STATE_SENDING,          DLT_STATE_WAIT_FOR_TX_DATA, DLT_STATE_SENDING          }
  };

  DLT_LOCAL P2FUNC(void, DLT_CODE, const kDltStateFctMatrix[kDltStateMachineStates][kDltStateMachineEvents])(uint32 LogChannelIndex) =
  {
                       /*   kDltEventInitDlt,
                        *                         kDltEventOneBufferHasContent,
                        *                                                 kDltEventTransmitCallRejected,
                        *                                                                       kDltEventSendingFinished,
                        *                                                                                             kDltEventTxStillActive,
                        *                                                                                                                   kDltEventTimeout,
                        *                                                                                                                                         kDltEventDebugModeChanged */
  /* unint */             { Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction,   Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction },
  /* wait for tx data */  { Dlt_Sm_DummyFunction, Dlt_Sm_SendingDecision, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_ClearBuffer,   Dlt_Sm_ClearSendBuffer },
  /* sending */           { Dlt_Sm_DummyFunction, Dlt_Sm_SendingDecision, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_DummyFunction, Dlt_Sm_ClearBuffer,   Dlt_Sm_ClearSendBuffer }
  };

  Dlt_StateType lastState = Dlt_CurrentState[LogChannelIndex];

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Set new state depending on occurred event and previous state. Call transition function. */
  Dlt_CurrentState[LogChannelIndex] = kDltStateMatrix[lastState][transitionEvent];

  /* @ assert lastState < DLT_STATE_NUMBER_OF_STATES; */ /* VCA_DLT_VALID_LAST_STATE */
  kDltStateFctMatrix[lastState][transitionEvent](LogChannelIndex);
} /* Dlt_Sm_StateTransition */

/**********************************************************************************************************************
 * Dlt_Sm_DummyFunction
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_DummyFunction(uint32 LogChannelIndex)
{
  /* Do nothing */
  DLT_DUMMY_STATEMENT(LogChannelIndex); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
} /* Dlt_Sm_DummyFunction */

/**********************************************************************************************************************
 * Dlt_Sm_SendingDecision
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6030 2 */ /* MD_MSR_STCYC */
/* PRQA S 6010 1 */ /* MD_MSR_STPTH */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_SendingDecision(uint32 LogChannelIndex)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 If a complete message was received:
   *       Create according response frame and write it to control buffer. */
# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
  if ((Dlt_RxInfo.RxIndicationOccured > 0u) &&
      (Dlt_ReceiveBuffer.Read != Dlt_ReceiveBuffer.Write))
  {
    Dlt_Sm_BuildResponseMessage();
    Dlt_RxInfo.RxIndicationOccured = 0u;
  }
# endif

  /* #20 If control buffer has content, send content of this buffer. Note that control messages have higher priority
   *       than log and trace messages. */
  if (Dlt_ControlSendBuffer[LogChannelIndex].Read != Dlt_ControlSendBuffer[LogChannelIndex].Write) /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
  {
    /* PRQA S 2995 1 */ /* MD_Dlt_AlwaysTrue */
    Dlt_TxBuf_WriteBufToTxMsg(LogChannelIndex, Dlt_ControlSendBuffer[LogChannelIndex].Buffer, Dlt_ControlSendBuffer[LogChannelIndex].Read, Dlt_ControlSendBuffer[LogChannelIndex].Write, TRUE);
  }
  /* #30 Otherwise if send buffer has content and sending is currently allowed, send content of this buffer. */
  else if (Dlt_SendBuffer[LogChannelIndex].Read != Dlt_SendBuffer[LogChannelIndex].Write) /* COV_DLT_NO_RX_DATA_PATH_SUPPORT */
  {
    /* PRQA S 2996 1 */ /* MD_Dlt_AlwaysFalse */
    Dlt_TxBuf_WriteBufToTxMsg(LogChannelIndex, Dlt_SendBuffer[LogChannelIndex].Buffer, Dlt_SendBuffer[LogChannelIndex].Read, Dlt_SendBuffer[LogChannelIndex].Write, FALSE);
  }
  else /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
  {
  }
} /* Dlt_Sm_SendingDecision() */

# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Sm_BuildResponseMessage
 *********************************************************************************************************************/
/*!
 *
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_BuildResponseMessage(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  DLT_LOCAL P2FUNC(void, DLT_CODE, const dltServiceFctArray[DLT_NUMBER_OF_SERVICES]) (void) =
  {
    Dlt_Srv_DummyService /* 0x0000 */,                                Dlt_Srv_SetLogLevel /* 0x0001 */,                           Dlt_Srv_SetTraceStatus /* 0x0002 */,
    Dlt_Srv_GetLogInfo /* 0x0003 */,                                  Dlt_Srv_GetDefaultLogLevel /* 0x0004 */,                    Dlt_Srv_StoreConfig /* 0x0005 - Not supported */,
    Dlt_Srv_ResetToFactoryDefault /* 0x0006 - Not supported */,       Dlt_Srv_SetComInterfaceStatus /* 0x0007 - Not supported */, Dlt_Srv_SetComInterfaceMaxBandwidth /* 0x0008 - Not supported */,
    Dlt_Srv_SetVerboseMode /* 0x0009 */,                              Dlt_Srv_SetMessageFiltering /* 0x000A */,                   Dlt_Srv_SetTimingPackets /* 0x000B - Not supported */,
    Dlt_Srv_GetLocalTime /* 0x000C */,                                Dlt_Srv_SetUseECUID /* 0x000D */,                           Dlt_Srv_SetUseSessionID /* 0x000E */,
    Dlt_Srv_UseTimestamp /* 0x000F */,                                Dlt_Srv_UseExtendedHeader /* 0x0010 */,                     Dlt_Srv_SetDefaultLogLevel /* 0x0011 */,
    Dlt_Srv_SetDefaultTraceStatus /* 0x0012 */,                       Dlt_Srv_GetSoftwareVersion /* 0x0013 */,                    Dlt_Srv_MessageBufferOverflow /* 0x0014 - Not supported */,
    Dlt_Srv_GetDefaultTraceStatus /* 0x0015 */,                       Dlt_Srv_GetComInterfaceStatus /* 0x0016 - Not supported */, Dlt_Srv_GetLogChannelNames /* 0x0017 */,
    Dlt_Srv_GetComInterfaceMaxBandwidth /* 0x0018 - Not supported */, Dlt_Srv_GetVerboseModeStatus /* 0x0019 */,                  Dlt_Srv_GetMessageFilteringStatus /* 0x001A */,
    Dlt_Srv_GetUseECUID /* 0x001B */,                                 Dlt_Srv_GetUseSessionID /* 0x001C */,                       Dlt_Srv_GetUseTimestamp /* 0x001D */,
    Dlt_Srv_GetUseExtendedHeader /* 0x001E */,                        Dlt_Srv_GetTraceStatus /* 0x001F - Not supported */,        Dlt_Srv_SetLogChannelAssignment /* 0x0020 */,
    Dlt_Srv_SetLogChannelThreshold /* 0x0021 */,                      Dlt_Srv_GetLogChannelThreshold /* 0x0022 */,                Dlt_Srv_BufferOverflowNotification /* 0x0023 */,
    Dlt_Srv_SyncTimeStamp /* 0x0024 */
  };

  uint32_least i;
  uint32 bufferIndex;
  uint32 serviceID;
  uint32 ecuID = 0u;
  uint16 locLength;
  uint8  avoidDeadLock = 0u;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Iterate over receive buffer content until empty: */
  while ((Dlt_ReceiveBuffer.Read != Dlt_ReceiveBuffer.Write) &&
         (Dlt_RxInfo.RxIndicationOccured > 0u))
  {
    locLength = (uint16)DLT_MAX_CONTROL_MESSAGE_LENGTH;
    bufferIndex = Dlt_ReceiveBuffer.Read;

    /* #20 Iterate over receive buffer to extract one receive message: */
    for (i = 0u; (i < (uint32)DLT_MAX_CONTROL_MESSAGE_LENGTH) && (i < (uint32)locLength); i++)
    {
      Dlt_ReceiveMessage[i] = Dlt_ReceiveBuffer.Buffer[bufferIndex];
      /* ESCAN00113408 */
      if ((i == 0u)
       && ((Dlt_ReceiveMessage[0] & Dlt_StandardHeaderType_SWV_Mask) != ((uint8)(Dlt_StandardHeaderType_SoftwareVersion << DLT_SHIFT_5BIT))))
      {
        avoidDeadLock = DLT_AVOID_DEADLOCK_COUNT + 1u;
        locLength = 0u;
      }

      /* Get real message length to stop copy if message ends */
      if (i == 3u)
      {
        locLength  = (uint16)((uint16)Dlt_ReceiveMessage[2] << DLT_SHIFT_8BIT);
        locLength |= (uint16)((uint16)Dlt_ReceiveMessage[3]);
      }
      bufferIndex++;
      if (bufferIndex >= DLT_CONTROL_BUFFER_SIZE)
      {
        bufferIndex = 0u;
      }
    }

    /* #30 If message has invalid length or dead lock was detected:
     *       Reject all remaining messages in receive buffer.
     */
    if ((locLength > (uint16)DLT_MAX_CONTROL_MESSAGE_LENGTH) ||
        (avoidDeadLock > DLT_AVOID_DEADLOCK_COUNT))
    {
      Dlt_ReceiveBuffer.Read = Dlt_ReceiveBuffer.Write;
    }
    /* #40 Otherwise:
     *       Extract ECU ID and service ID from received message.
     *       If the message is not assigned to this ECU:
     *         Ignore message.
     *       Otherwise, if service ID is valid:
     *         Call according service function to build response.
     *       Otherwise if the service ID is in valid range for injection call:
     *         Call the service function to check and forward the injection request.
     *       Otherwise:
     *         Respond with service not supported (DLT_NOT_SUPPORTED).
     */
    else
    {
      Dlt_ReceiveBuffer.Read = bufferIndex;
      Dlt_RxInfo.RxIndicationOccured--;

      Dlt_TxBuf_GetEcuIdAndService(&ecuID, &serviceID);

      if ((ecuID != (uint32)DLT_ECU_ID) &&
          (ecuID != (uint32)0u))
      {
        /* Ignore incoming message if the message's ECU ID is not identically with ECUs ID */
      }
      else if (serviceID < DLT_NUMBER_OF_SERVICES)
      {
        /* @ assert dltServiceFctArray[serviceID] != NULL_PTR; */
        dltServiceFctArray[serviceID]();
      }
#  if (DLT_INJECTION_CALL_SUPPORT == STD_ON)
      else if (serviceID >= 0xFFFu)
      {
        Dlt_Srv_CallSwcInjection(serviceID);
      }
#  endif
      else
      {
        /* @ assert dltServiceFctArray[0] != NULL_PTR; */
        dltServiceFctArray[0]();
      }
    }

    avoidDeadLock++;
  }
} /* Dlt_Sm_BuildResponseMessage */
# endif /* (DLT_RX_DATA_PATH_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 * Dlt_Sm_ClearBuffer
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_ClearBuffer(uint32 LogChannelIndex)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Clear all buffers after transmission timeout. */
  Dlt_ControlSendBuffer[LogChannelIndex].Write = 0u;
  Dlt_ControlSendBuffer[LogChannelIndex].Read = 0u;
  Dlt_ReceiveBuffer.Write = 0u;
  Dlt_ReceiveBuffer.Read = 0u;

  VStdLib_MemClr(&Dlt_ControlSendBuffer[LogChannelIndex].Buffer[0], DLT_CONTROL_BUFFER_SIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
  VStdLib_MemClr(&Dlt_ReceiveBuffer.Buffer[0], DLT_CONTROL_BUFFER_SIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

  VStdLib_MemClr(&Dlt_ControlSendMessage[LogChannelIndex][0], DLT_MAX_CONTROL_MESSAGE_LENGTH); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
  VStdLib_MemClr(&Dlt_ReceiveMessage[0], DLT_MAX_CONTROL_MESSAGE_LENGTH); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

  Dlt_Sm_ClearSendBuffer(LogChannelIndex);
} /* Dlt_Sm_ClearBuffer */

/**********************************************************************************************************************
 * Dlt_Sm_ClearSendBuffer
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_ClearSendBuffer(uint32 LogChannelIndex)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Clear the send buffer after transmission timeout or debug mode change. */
  Dlt_SendBuffer[LogChannelIndex].Write = 0u;
  Dlt_SendBuffer[LogChannelIndex].Read = 0u;

  VStdLib_MemClr(&Dlt_SendBuffer[LogChannelIndex].Buffer[0], DLT_MESSAGE_BUFFER_SIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
  VStdLib_MemClr(&Dlt_SendData[LogChannelIndex][0], DLT_MESSAGE_BUFFER_SIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
} /* Dlt_Sm_ClearSendBuffer */

/**********************************************************************************************************************
 * Dlt_Sm_BufferOverflowNotification
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_BufferOverflowNotification(uint32 LogChannelIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 timestamp;
  uint32 ecuID = (uint32)DLT_ECU_ID;
  uint16 index = 0u;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If timestamp support is enabled, request current time.
   *     Otherwise, set timestamp to zero.
   */
  timestamp = Dlt_GetCurrentTime();

  /* #20 Build response frame for service BufferOverflowNotification and write it to control buffer. */
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex((DLT_STD_HEADER_TYPE_WV_WT_NS_WE_MF_UE | (uint32)((uint32)Dlt_MessageCounter[LogChannelIndex] << 16u) | DLT_SEND_BUFFER_OVERFLOW_NOTIFICATION_RESPONSE_LENGTH), &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(ecuID, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(timestamp, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex(DLT_EXT_HEADER_MESSAGE_INFO_NONVERB_CONTROL_RESPONSE, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(DLT_APPLICATION_ID_ALL, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(DLT_CONTEXT_ID_ALL, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(DLT_SEND_BUFFER_OVERFLOW_NOTIFICATION_SERVICE_ID, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_ControlSendMessage[LogChannelIndex][index] = DLT_SEND_BUFFER_OVERFLOW_NOTIFICATION_RESPONSE_OK; /* ESCAN00116215 */
  index++;
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(Dlt_BufferOverflowNotificationStatus[LogChannelIndex].Counter, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);

  Dlt_TxBuf_WriteToCtrlBuf(LogChannelIndex, DLT_SEND_BUFFER_OVERFLOW_NOTIFICATION_RESPONSE_LENGTH);

  Dlt_MessageCounter[LogChannelIndex]++;
} /* Dlt_Sm_BufferOverflowNotification */

# if (DLT_SYNC_TIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Sm_HandleSyncTimeStamp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_HandleSyncTimeStamp(void)
{
  if (Dlt_SyncTimeState == DLT_SYNC_TIME_WAIT_FOR_TIMEOUT)
  {
    Dlt_SyncTimeTimeoutCnt--;
    if (Dlt_SyncTimeTimeoutCnt == 0u)
    {
      Dlt_SyncTimeState = DLT_SYNC_TIME_PROCESSING;
    }
  }

  if (Dlt_SyncTimeState == DLT_SYNC_TIME_PROCESSING)
  {
    StbM_TimeStampType currentSyncTime;
    StbM_UserDataType userData;

    if (  (StbM_GetCurrentTime(DLT_STBM_TIME_BASE_ID, &currentSyncTime, &userData) == E_OK)
       && ((currentSyncTime.timeBaseStatus & StbM_Global_Time_Base_BflMask) == StbM_Global_Time_Base_BflMask))
    {
      uint32 logChannelIndex;

      for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
      {
        Dlt_Sm_SendSyncTimeStamp(logChannelIndex, currentSyncTime);
      }
      Dlt_SyncTimeTimeoutCnt = DLT_SYNC_TIME_COUNTER;
      Dlt_SyncTimeState = DLT_SYNC_TIME_WAIT_FOR_TIMEOUT;
    }
  }
} /* Dlt_Sm_HandleSyncTimeStamp */

/**********************************************************************************************************************
 * Dlt_Sm_SendSyncTimeStamp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Sm_SendSyncTimeStamp(uint32 LogChannelIndex, StbM_TimeStampType SyncTime)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 timestamp;
  uint32 ecuID = (uint32)DLT_ECU_ID;
  uint16 index = 0u;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If timestamp support is enabled, request current time.
   *     Otherwise, set timestamp to zero.
   */
  timestamp = Dlt_GetCurrentTime();

  /* #20 Build response frame for service SyncTimeStamp and write it to control buffer. */
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex((DLT_STD_HEADER_TYPE_WV_WT_NS_WE_MF_UE | (uint32)((uint32)Dlt_MessageCounter[LogChannelIndex] << 16u) | DLT_SEND_SYNC_TIME_STAMP_RESPONSE_LENGTH), &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(ecuID, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(timestamp, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex(DLT_EXT_HEADER_MESSAGE_INFO_NONVERB_CONTROL_RESPONSE, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(DLT_APPLICATION_ID_ALL, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(DLT_CONTEXT_ID_ALL, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(DLT_SEND_SYNC_TIME_STAMP_SERVICE_ID, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);

  Dlt_ControlSendMessage[LogChannelIndex][index] = DLT_OK;
  index++;

  Dlt_Hlp_CopyLongToArrayBigEndian(SyncTime.nanoseconds, &Dlt_ControlSendMessage[LogChannelIndex][index]);
  index += 4u;
  Dlt_Hlp_CopyLongToArrayBigEndian(SyncTime.seconds, &Dlt_ControlSendMessage[LogChannelIndex][index]);
  index += 4u;
  Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(SyncTime.secondsHi, &Dlt_ControlSendMessage[LogChannelIndex][index], &index);

  Dlt_TxBuf_WriteToCtrlBuf(LogChannelIndex, DLT_SEND_SYNC_TIME_STAMP_RESPONSE_LENGTH);

  Dlt_MessageCounter[LogChannelIndex]++;
} /* Dlt_Sm_SendSyncTimeStamp */
# endif

/**********************************************************************************************************************
 * Dlt_Sm_GlobalStateTransition
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Sm_GlobalStateTransition(void)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Update global state.
   *     If changed to DLT_GLOBAL_STATE_OFFLINE:
   *       clear all buffers and set Dlt_CurrentState to DLT_STATE_WAIT_FOR_TX_DATA.
   */
  if (Dlt_GlobalState != Dlt_GlobalStateNew)
  {
    Dlt_GlobalState = Dlt_GlobalStateNew;

    if (Dlt_GlobalState == DLT_GLOBAL_STATE_OFFLINE)
    {
      uint32 logChannelIndex;

      for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
      {
        Dlt_Sm_ClearBuffer(logChannelIndex);
        Dlt_CurrentState[logChannelIndex] = DLT_STATE_WAIT_FOR_TX_DATA;
      }
    }
  }
} /* Dlt_Sm_GlobalStateTransition */

/* ####################################################################################################################
 *     SSSSSS    EEEEEEEEEE  RRRRRRRR    VV      VV    IIIIII      CCCCCC    EEEEEEEEEE
 *   SS      SS  EE          RR      RR  VV      VV      II      CC      CC  EE
 *   SS          EE          RR      RR  VV      VV      II      CC          EE
 *     SSSSSS    EEEEEE      RRRRRRRR    VV      VV      II      CC          EEEEEE
 *           SS  EE          RR    RR      VV  VV        II      CC          EE
 *   SS      SS  EE          RR      RR    VV  VV        II      CC      CC  EE
 *     SSSSSS    EEEEEEEEEE  RR      RR      VV        IIIIII      CCCCCC    EEEEEEEEEE
 * ####################################################################################################################
 */
# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Srv_FinalizeRespToCtrlBuf
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *

 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_FinalizeRespToCtrlBuf(
  Dlt_OptionType Option,
  boolean IsGetter,
  uint16 PayloadLen,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) Var)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  uint16 index;
  uint16 responseLength;
  uint16 indexOfPayloadInControlResponse;

  /* ----- Implementation ----------------------------------------------- */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    /* #10 Build response frame depending on received message. */
    indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, PayloadLen);

    responseLength = indexOfPayloadInControlResponse + PayloadLen;
    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_OK;

    /* #20 If the received message requested data, return the requested data.
     *     Otherwise, change of configuration is requested so change it.
     */
    if (IsGetter == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
    {
      index = indexOfPayloadInControlResponse + 1u;
      Dlt_ControlSendMessage[logChannelIndex][index] = *Var;
    }
    else
    {
      /* Get argument after service ID */
      if (Dlt_Hlp_ParamInValidRange((Dlt_OptionType)Option, (sint8)Dlt_ReceiveMessage[indexOfPayloadInControlResponse]) == DLT_OK)
      {
        *Var = Dlt_ReceiveMessage[indexOfPayloadInControlResponse];
      }
      else
      {
        Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_ERROR;
      }
    }

    /* #30 Write the response frame to control buffer.*/
    Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, responseLength);
  }
} /* Dlt_Srv_FinalizeRespToCtrlBuf */

/**********************************************************************************************************************
 * Dlt_Srv_PrepareBufferOverflowNotificationOfLogChannel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_PrepareBufferOverflowNotificationOfLogChannel(uint32 LogChannelIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 indexOfPayloadInControlResponse;
  uint16 responseLength;
  CONST(uint16, DLT_CONST) payloadLength = DLT_SEND_BUFFER_OVERFLOW_NOTIFICATION_PAYLOAD_LENGTH;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Build response frame depending on received message and log channel specific buffer overflow flag. */
  indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(LogChannelIndex, payloadLength);

  responseLength = indexOfPayloadInControlResponse + payloadLength;
  Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInControlResponse] = DLT_OK;
  indexOfPayloadInControlResponse++;

  /* #20 Add the current buffer overflow counter to the response frame depending on the endianess of the received message. */
  if (Dlt_IsPayloadInBigEndian())
  {
    Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(Dlt_BufferOverflowNotificationStatus[LogChannelIndex].Counter, &Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInControlResponse], &indexOfPayloadInControlResponse);
  }
  else
  {
    Dlt_Hlp_CopyLongToArrayLittleEndian_AndMoveIndex(Dlt_BufferOverflowNotificationStatus[LogChannelIndex].Counter, &Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInControlResponse], &indexOfPayloadInControlResponse);
  }

  /* #30 Write the response frame to control buffer.*/
  Dlt_TxBuf_WriteToCtrlBuf(LogChannelIndex, responseLength);
} /* Dlt_Srv_PrepareBufferOverflowNotificationOfLogChannel */

# if (DLT_SYNC_TIME_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Srv_PrepareSyncTimeStampOfLogChannel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_PrepareSyncTimeStampOfLogChannel(uint32 LogChannelIndex)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint16 indexOfPayloadInControlResponse;
  uint16 responseLength;
  StbM_TimeStampType currentSyncTime = {0u, 0u, 0u, 0u};
  StbM_UserDataType userData = {0u, 0u, 0u, 0u};

  CONST(uint16, DLT_CONST) payloadLength = DLT_SEND_SYNC_TIME_STAMP_PAYLOAD_LENGTH;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Build response frame depending on received message. */
  indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(LogChannelIndex, payloadLength);

  responseLength = indexOfPayloadInControlResponse + payloadLength;

  /* #20 Determine the current sync time and check validity via GLOBAL_TIME_BASE. Return DLT_ERROR if current sync time is not synchronized. */
  if (   (StbM_GetCurrentTime(DLT_STBM_TIME_BASE_ID, &currentSyncTime, &userData) == E_OK)
      && ((currentSyncTime.timeBaseStatus & StbM_Global_Time_Base_BflMask) == StbM_Global_Time_Base_BflMask))
  {
    Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInControlResponse] = DLT_OK;
  }
  else
  {
    Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInControlResponse] = DLT_ERROR;
  }

  indexOfPayloadInControlResponse++;

  /* #25 Add the current sync time to the response frame depending on the endianness of the received message. */
  if (Dlt_IsPayloadInBigEndian())
  {
    Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(currentSyncTime.nanoseconds, &Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInControlResponse], &indexOfPayloadInControlResponse);
    Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(currentSyncTime.seconds, &Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInControlResponse], &indexOfPayloadInControlResponse);
    Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex(currentSyncTime.secondsHi, &Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInControlResponse], &indexOfPayloadInControlResponse);
  }
  else
  {
    Dlt_Hlp_CopyLongToArrayLittleEndian_AndMoveIndex(currentSyncTime.nanoseconds, &Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInControlResponse], &indexOfPayloadInControlResponse);
    Dlt_Hlp_CopyLongToArrayLittleEndian_AndMoveIndex(currentSyncTime.seconds, &Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInControlResponse], &indexOfPayloadInControlResponse);
    Dlt_Hlp_CopyLongToArrayLittleEndian_AndMoveIndex(currentSyncTime.secondsHi, &Dlt_ControlSendMessage[LogChannelIndex][indexOfPayloadInControlResponse], &indexOfPayloadInControlResponse);
  }

  /* #30 Write the response frame to control buffer.*/
  Dlt_TxBuf_WriteToCtrlBuf(LogChannelIndex, responseLength);
} /* Dlt_Srv_PrepareSyncTimeStampOfLogChannel */
# endif

/**********************************************************************************************************************
 * Dlt_Srv_ServiceNotSupported
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL_INLINE FUNC(void, DLT_CODE) Dlt_Srv_ServiceNotSupported(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 logChannelIndex;

  /* ----- Implementation ----------------------------------------------- */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    Dlt_Hlp_WriteFailResponseToControlBuffer(logChannelIndex, (uint8)DLT_NOT_SUPPORTED);
  }
} /* Dlt_Srv_ServiceNotSupported */

/**********************************************************************************************************************
 * Dlt_Srv_DummyService
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_DummyService(void) /* 0x0000 */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 The requested service is not supported by DLT, respond with "DLT_NOT_SUPPORTED".*/
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_DummyService */

/**********************************************************************************************************************
 * Dlt_Srv_SetStatus
 *********************************************************************************************************************/
/*!
 *
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
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetStatus(
  Dlt_OptionType Option,
  sint8 DefaultValue)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  uint32 applicationID;
  uint32 contextID;
  uint16 indexInPayloadOfReceivedMessage;
  uint16 indexOfPayloadInControlResponse;
  uint16 responseLength;
  sint8  new_status;
  uint8  status = DLT_ERROR;
  /* uint32 com_interface; */ /* Not supported today */

  /* ----- Implementation ------------------------------------------------- */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    /* #10 Build response frame depending on received message. */
    indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, DLT_SET_STATUS_PAYLOAD_LENGTH);
    responseLength = indexOfPayloadInControlResponse + 1u;

    indexInPayloadOfReceivedMessage = indexOfPayloadInControlResponse;

    /* #20 Get Application ID and Context ID of received message (always in big Endian). */
    Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex(&Dlt_ReceiveMessage[DLT_STD_HEADER_HEADERTYPE_INDEX], &applicationID, &indexInPayloadOfReceivedMessage);
    Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex(&Dlt_ReceiveMessage[DLT_STD_HEADER_HEADERTYPE_INDEX], &contextID, &indexInPayloadOfReceivedMessage);

    /* #30 Get new state from received message. */
    new_status = (sint8)(Dlt_ReceiveMessage[indexInPayloadOfReceivedMessage]);

    /* #40 If the requested state is -1, the option has to be set to default. */
    if (new_status == (sint8)-1)
    {
      new_status = (sint8)DefaultValue;
    }

    /* #50 If the new status is in valid range:
     *       Set the specified option of Application ID - Context ID couple to new status.
     *       If setting the requested new state failed, set the error code to DLT_ERROR.
     */
    if (Dlt_Hlp_ParamInValidRange(Option, (sint8)new_status) == DLT_OK)
    {
      status = Dlt_Hlp_SetOption(applicationID, contextID, Option, (uint8)new_status);

      if (status == E_NOT_OK)
      {
        status = DLT_ERROR;
      }
    }

    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = status; /* Result of option state change. */

    /* #60 Write the response message to control buffer.*/
    Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, responseLength);
  }
} /* Dlt_Srv_SetStatus() */

/**********************************************************************************************************************
 * Dlt_Srv_SetLogLevel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetLogLevel(void) /* 0x0001 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Change log level for the couple of application ID and context ID, specified in received message. */
  Dlt_Srv_SetStatus((Dlt_OptionType)DLT_OPT_LOGLEVEL, (sint8)Dlt_DefaultMaxLogLevel);
} /* Dlt_Srv_SetLogLevel() */

/**********************************************************************************************************************
 * Dlt_Srv_SetTraceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetTraceStatus(void) /* 0x0002 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Change trace status for the couple of application ID and context ID, specified in received message. */
  Dlt_Srv_SetStatus((Dlt_OptionType)DLT_OPT_TRACESTATUS, (sint8)Dlt_DefaultTraceStatus); /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
} /* Dlt_Srv_SetTraceStatus() */

/**********************************************************************************************************************
 * Dlt_Srv_GetLogInfo
 *********************************************************************************************************************/
/*!
 *
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
/* PRQA S 6030 3 */ /* MD_MSR_STCYC */
/* PRQA S 6050 2 */ /* MD_MSR_STCAL */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLogInfo(void) /* 0x0003 */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  uint32 applicationId;
  uint32 contextId;
  const uint16 indexOfPayloadInReceivedMessage = Dlt_Hlp_GetIndexOfPayloadAfterServiceIdInReceivedMessage();
  const uint16 indexOfPayloadInResponseMessage = indexOfPayloadInReceivedMessage;
  uint16 indexInRequest;
  uint16 indexInResponse;
  uint8  options;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Build response frame depending on received message. */
  indexInRequest = indexOfPayloadInReceivedMessage;

  /* #20 Get options from received message. */
  options = Dlt_ReceiveMessage[indexInRequest];
  indexInRequest++;

  /* #30 Get Application ID and Context ID of received message (always in big Endian). */
  Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex(&Dlt_ReceiveMessage[0], &applicationId, &indexInRequest);
  Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex(&Dlt_ReceiveMessage[0], &contextId, &indexInRequest);

  /* ----- Implementation ------------------------------------------------- */
  /* #40 Iterate over all log channels to transmit the response. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    uint8 status;

    /* #50 If the option is valid: */
    if ((options == DLT_LOG_OPTIONS_ALL_INFO_WITHOUT_DESCRIPTIONS)
     || (options == DLT_LOG_OPTIONS_ALL_INFO_WITH_DESCRIPTIONS))
    {
      uint16 appIdIndex;
      uint16 conIdIndex;
      boolean applicationAlreadyRegistered = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
      boolean contextAlreadyRegistered;

      contextAlreadyRegistered = Dlt_Hlp_IsContextRegistered(applicationId, contextId, &appIdIndex, &conIdIndex, &applicationAlreadyRegistered);

      /*@ assert appIdIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
      /*@ assert conIdIndex < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

      /* #60 If the requested application id and context id are registered. */
      if ((applicationAlreadyRegistered == TRUE) /* PRQA S 2995 1 */ /* MD_Dlt_AlwaysTrue */
       && (contextAlreadyRegistered == TRUE))
      {
        uint16 lengthOfPayload;

        if (options == DLT_LOG_OPTIONS_ALL_INFO_WITHOUT_DESCRIPTIONS)
        {
          lengthOfPayload = DLT_GET_LOG_INFO_PAYLOAD_LENGTH_WITHOUT_DESCRIPTIONS;
        }
        else
        {
          lengthOfPayload = DLT_GET_LOG_INFO_PAYLOAD_LENGTH_WITH_DESCRIPTIONS;
        }

        /* #70 Reset the iteratable index of response. */
        indexInResponse = indexOfPayloadInResponseMessage;

        /* #80 If the response has a valid length: */
        /* PRQA S 2995, 2996 2 */ /* MD_Dlt_AlwaysTrue, MD_Dlt_AlwaysFalse */
        if (((indexInResponse + lengthOfPayload) < DLT_MAX_CONTROL_MESSAGE_LENGTH)
         && ((indexInResponse + lengthOfPayload) < Dlt_GetLogChannelMaxMessageLengthOfLogChannelDescriptor(logChannelIndex))) /* COV_DLT_RESPONSE_MSG_LENGTH */
        {
          status = options;

          /* #90 Set the 'Status' byte in response. */
          Dlt_ControlSendMessage[logChannelIndex][indexInResponse] = status;
          indexInResponse++;

          /* #100 Set the following data to response. */
          /*       - application count
           *       - application id
           *       - context count
           *       - context id
           *       - log level
           *       - trace status
           *       - If with description:
           *         - length of context description (always 0)
           *         - length of application description (always 0)
           */
          if (Dlt_IsPayloadInBigEndian())
          {
            Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex(1u, &Dlt_ControlSendMessage[logChannelIndex][indexInResponse], &indexInResponse);
          }
          else
          {
            Dlt_Hlp_CopyIntToArrayLittleEndian_AndMoveIndex(1u, &Dlt_ControlSendMessage[logChannelIndex][indexInResponse], &indexInResponse);
          }

          Dlt_Hlp_CopyLongToArrayBigEndian(applicationId, &Dlt_ControlSendMessage[logChannelIndex][indexInResponse]);
          indexInResponse += 4u;

          if (Dlt_IsPayloadInBigEndian())
          {
            Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex(1u, &Dlt_ControlSendMessage[logChannelIndex][indexInResponse], &indexInResponse);
          }
          else
          {
            Dlt_Hlp_CopyIntToArrayLittleEndian_AndMoveIndex(1u, &Dlt_ControlSendMessage[logChannelIndex][indexInResponse], &indexInResponse);
          }

          Dlt_Hlp_CopyLongToArrayBigEndian(contextId, &Dlt_ControlSendMessage[logChannelIndex][indexInResponse]);
          indexInResponse += 4u;

          Dlt_ControlSendMessage[logChannelIndex][indexInResponse] = (uint8)Dlt_Context.appIds[appIdIndex].contextInfoList[conIdIndex].logLevel;
          indexInResponse++;

          Dlt_ControlSendMessage[logChannelIndex][indexInResponse] = (uint8)Dlt_Context.appIds[appIdIndex].contextInfoList[conIdIndex].traceStatus;
          indexInResponse++;

          if (options == DLT_LOG_OPTIONS_ALL_INFO_WITH_DESCRIPTIONS)
          {
            /* Copy the application description length and context description length to response. */
            if (Dlt_IsPayloadInBigEndian())
            {
              Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex(0u, &Dlt_ControlSendMessage[logChannelIndex][indexInResponse], &indexInResponse); /* Context description length = 0. */
              Dlt_Hlp_CopyIntToArrayBigEndian_AndMoveIndex(0u, &Dlt_ControlSendMessage[logChannelIndex][indexInResponse], &indexInResponse); /* Application description length = 0. */
            }
            else
            {
              Dlt_Hlp_CopyIntToArrayLittleEndian_AndMoveIndex(0u, &Dlt_ControlSendMessage[logChannelIndex][indexInResponse], &indexInResponse); /* Context description length = 0. */
              Dlt_Hlp_CopyIntToArrayLittleEndian_AndMoveIndex(0u, &Dlt_ControlSendMessage[logChannelIndex][indexInResponse], &indexInResponse); /* Application description length = 0. */
            }
          }

          /* #110 Set the 'Reserved' bytes in response. */
          Dlt_Hlp_CopyLongToArrayBigEndian(0u, &Dlt_ControlSendMessage[logChannelIndex][indexInResponse]); /* Reserved 4 bytes; must be always 0. */
          indexInResponse += 4u;

          /* #120 Set the standard and extended header in response. */
          (void)Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, lengthOfPayload);

          /* #130 Write the response frame to control buffer. */
          Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, indexInResponse);
        }
        else
        {
          status = DLT_LOG_STATUS_RESPONSE_DATA_OVERFLOW;
        }
      }
      /* #140 Otherwise, the requested application or context id is invalid, therefore indicate that the requested context is
       *       not available. */
      else
      {
        status = DLT_LOG_STATUS_RESPONSE_DATA_OVERFLOW;
      }
    }
    /* #150 Otherwise, the requested option is not supported, therefore indicate that it is not supported. */
    else
    {
      status = DLT_LOG_STATUS_RESPONSE_DATA_OVERFLOW;
    }

    /* #160 If the status was not set to successful, write corresponding error message to log channel specific
     *       control buffer. */
    if ((status != DLT_LOG_STATUS_ALL_INFO_WITHOUT_DESCRIPTIONS)
     && (status != DLT_LOG_STATUS_ALL_INFO_WITH_DESCRIPTIONS))
    {
      Dlt_Hlp_WriteFailResponseToControlBuffer(
        logChannelIndex,
        status);
    }
  }
} /* Dlt_Srv_GetLogInfo */

/**********************************************************************************************************************
 * Dlt_Srv_SetVerboseMode
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetVerboseMode(void) /* 0x0009 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Service is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_SetVerboseMode() */

/**********************************************************************************************************************
 * Dlt_Srv_GetVerboseModeStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetVerboseModeStatus(void) /* 0x0019 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Service is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_GetVerboseModeStatus() */

/**********************************************************************************************************************
 * Dlt_Srv_SetDefaultLogLevel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetDefaultLogLevel(void) /* 0x0011 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Change default log level according to received message. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_LOGLEVEL, FALSE, DLT_SET_DEFAULT_LOG_LEVEL_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_DefaultMaxLogLevel);
} /* Dlt_Srv_SetDefaultLogLevel */

/**********************************************************************************************************************
 * Dlt_Srv_GetDefaultLogLevel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetDefaultLogLevel(void) /* 0x0004 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Build response frame according to received message and return default log level. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_LOGLEVEL, TRUE, DLT_GET_DEFAULT_LOG_LEVEL_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_DefaultMaxLogLevel);
} /* Dlt_Srv_GetDefaultLogLevel */

/**********************************************************************************************************************
 * Dlt_Srv_SetDefaultTraceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetDefaultTraceStatus(void) /* 0x0012 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Change default trace status according to received message. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_TRACESTATUS, FALSE, DLT_SET_DEFAULT_TRACE_STATUS_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_DefaultTraceStatus);
} /* Dlt_Srv_SetDefaultTraceStatus */

/**********************************************************************************************************************
 * Dlt_Srv_GetDefaultTraceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetDefaultTraceStatus(void) /* 0x0015 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Build response frame according to received message and return default trace status. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_TRACESTATUS, TRUE, DLT_GET_DEFAULT_TRACE_STATUS_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_DefaultTraceStatus);
} /* Dlt_Srv_GetDefaultTraceStatus */

/**********************************************************************************************************************
 * Dlt_Srv_SetUseECUID
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetUseECUID(void) /* 0x000D */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Change flag Dlt_HeaderUseEcuId according to received message. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, FALSE, DLT_SET_USE_ECU_ID_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_HeaderUseEcuId);
} /* Dlt_Srv_SetUseECUID */

/**********************************************************************************************************************
 * Dlt_Srv_GetUseECUID
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseECUID(void) /* 0x001B */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Build response frame according to received message and return flag status of Dlt_HeaderUseEcuId. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, TRUE, DLT_GET_USE_ECU_ID_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_HeaderUseEcuId);
} /* Dlt_Srv_GetUseECUID */

/**********************************************************************************************************************
 * Dlt_Srv_SetUseSessionID
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetUseSessionID(void) /* 0x000E */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Change flag Dlt_HeaderUseSessionId according to received message. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, FALSE, DLT_SET_USE_SESSION_ID_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_HeaderUseSessionId);
} /* Dlt_Srv_SetUseSessionID */

/**********************************************************************************************************************
 * Dlt_Srv_GetUseSessionID
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseSessionID(void) /* 0x001C */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Build response frame according to received message and return flag status of Dlt_HeaderUseSessionId. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, TRUE, DLT_GET_USE_SESSION_ID_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_HeaderUseSessionId);
} /* Dlt_Srv_GetUseSessionID */

/**********************************************************************************************************************
 * Dlt_Srv_UseTimestamp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_UseTimestamp(void) /* 0x000F */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 If implement timestamp is enabled:
   *       Change flag Dlt_HeaderUseTimestamp according to received message.
   *     Otherwise:
   *       The service UseTimestamp is not supported. Build response frame with "DLT_NOT_SUPPORTED".
   */
# if (DLT_TIMESTAMP_SUPPORT == STD_ON)
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, FALSE, DLT_SET_USE_TIMESTAMP_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_HeaderUseTimestamp);
# else
  Dlt_Srv_ServiceNotSupported();
# endif
} /* Dlt_Srv_UseTimestamp */

/**********************************************************************************************************************
 * Dlt_Srv_GetUseTimestamp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseTimestamp(void) /* 0x001D */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 If implement timestamp is enabled:
   *       Build response frame according to received message and return flag status of Dlt_HeaderUseTimestamp.
   *     Otherwise:
   *       The service GetUseTimestamp is not supported. Build response frame with "DLT_NOT_SUPPORTED".
   */
#if (DLT_TIMESTAMP_SUPPORT == STD_ON)
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, TRUE, DLT_GET_USE_TIMESTAMP_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_HeaderUseTimestamp);
#else
  Dlt_Srv_ServiceNotSupported();
#endif
} /* Dlt_Srv_GetUseTimestamp */

/**********************************************************************************************************************
 * Dlt_Srv_SetMessageFiltering
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetMessageFiltering(void) /* 0x000A */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Change flag Dlt_FilterMessages according to received message. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, FALSE, DLT_SET_USE_MESSAGE_FILTERING_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_FilterMessages);
} /* Dlt_Srv_SetMessageFiltering */

/**********************************************************************************************************************
 * Dlt_Srv_GetMessageFilteringStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetMessageFilteringStatus(void) /* 0x001A */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Build response frame according to received message and return flag status of Dlt_FilterMessages. */
  Dlt_Srv_FinalizeRespToCtrlBuf((Dlt_OptionType)DLT_OPT_BOOLEAN, TRUE, DLT_GET_USE_MESSAGE_FILTERING_PAYLOAD_LENGTH, (P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR)) &Dlt_FilterMessages);
} /* Dlt_Srv_GetMessageFilteringStatus */

/**********************************************************************************************************************
 * Dlt_Srv_UseExtendedHeader
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_UseExtendedHeader(void) /* 0x0010 */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 The service UseExtendedHeader is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_UseExtendedHeader */

/**********************************************************************************************************************
 * Dlt_Srv_GetUseExtendedHeader
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetUseExtendedHeader(void) /* 0x001E */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 The service GetUseExtendedHeader is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_GetUseExtendedHeader */

/**********************************************************************************************************************
 * Dlt_Srv_GetTraceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetTraceStatus(void) /* 0x001F */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  uint32 applicationId;
  uint32 contextId;
  const uint16 indexOfPayloadInReceivedMessage = Dlt_Hlp_GetIndexOfPayloadAfterServiceIdInReceivedMessage();
  uint16 indexInRequest;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Build response frame depending on received message. */
  indexInRequest = indexOfPayloadInReceivedMessage;

  /* #20 Get Application ID and Context ID of received message (always in big Endian). */
  Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex(&Dlt_ReceiveMessage[0], &applicationId, &indexInRequest);
  Dlt_Hlp_CopyArrayToLongBigEndian_AndMoveIndex(&Dlt_ReceiveMessage[0], &contextId, &indexInRequest);

  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    uint16 appIdIndex;
    uint16 conIdIndex;
    boolean applicationAlreadyRegistered = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
    boolean contextAlreadyRegistered;

    contextAlreadyRegistered = Dlt_Hlp_IsContextRegistered(applicationId, contextId, &appIdIndex, &conIdIndex, &applicationAlreadyRegistered);

    /*@ assert appIdIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
    /*@ assert conIdIndex < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

    /* #30 If the requested application id and context id are registered. */
    if ((applicationAlreadyRegistered == TRUE) /* PRQA S 2995 1 */ /* MD_Dlt_AlwaysTrue */
     && (contextAlreadyRegistered == TRUE))
    {
      uint16 responseLength;
      uint16 indexInResponse = indexOfPayloadInReceivedMessage;
      uint8 traceStatus;

      /* #40 Build response frame depending on received message. */
      (void)Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, DLT_GET_TRACE_STATUS_PAYLOAD_LENGTH);

      responseLength = indexInResponse + DLT_GET_TRACE_STATUS_PAYLOAD_LENGTH;
      traceStatus = (uint8)Dlt_Context.appIds[appIdIndex].contextInfoList[conIdIndex].traceStatus;

      Dlt_ControlSendMessage[logChannelIndex][indexInResponse] = DLT_OK;
      indexInResponse++;
      Dlt_ControlSendMessage[logChannelIndex][indexInResponse] = traceStatus;

      /* #50 Write the response frame to control buffer.*/
      Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, responseLength);
    }
    else
    {
      Dlt_Hlp_WriteFailResponseToControlBuffer(logChannelIndex, DLT_ERROR);
    }
  }
} /* Dlt_Srv_GetTraceStatus */

/**********************************************************************************************************************
 * Dlt_Srv_SetTimingPackets
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetTimingPackets(void) /* 0x000B - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 The service SetTimingPackets is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_SetTimingPackets */

/**********************************************************************************************************************
 * Dlt_Srv_GetLocalTime
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLocalTime(void) /* 0x000C */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 The service GetLocalTime is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_GetLocalTime */

/**********************************************************************************************************************
 * Dlt_Srv_SetComInterfaceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetComInterfaceStatus(void) /* 0x0007 - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 The service SetComInterfaceStatus is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_SetComInterfaceStatus */

/**********************************************************************************************************************
 * Dlt_Srv_GetComInterfaceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetComInterfaceStatus(void) /* 0x0016 - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 The service GetComInterfaceStatus is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_GetComInterfaceStatus */

/**********************************************************************************************************************
 * Dlt_Srv_SetComInterfaceMaxBandwidth
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetComInterfaceMaxBandwidth(void) /* 0x0008 - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 The service SetComInterfaceMaxBandwidth is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_SetComInterfaceMaxBandwidth */

/**********************************************************************************************************************
 * Dlt_Srv_GetComInterfaceMaxBandwidth
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetComInterfaceMaxBandwidth(void) /* 0x0018 - Not supported */
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 The service GetComInterfaceMaxBandwidth is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_GetComInterfaceMaxBandwidth */

/**********************************************************************************************************************
 * Dlt_Srv_GetLogChannelNames
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLogChannelNames(void) /* 0x0017 */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  uint16 responseLength;
  uint16 payloadLength;
  uint16 indexOfPayloadInControlResponse;
  uint8 numberOfLogChannels = Dlt_GetSizeOfLogChannelDescriptor();

  /* ----- Implementation ------------------------------------------------- */
  payloadLength = (uint16)(((uint16)numberOfLogChannels * DLT_DEFAULT_ID_LENGTH) + DLT_GET_LOG_CHANNEL_NAMES_PAYLOAD_LENGTH);

  /* #10 Iterate over all log channels. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, payloadLength);
    responseLength = indexOfPayloadInControlResponse + payloadLength;

    /* #20 If the response has valid length: */
    if (responseLength < DLT_CONTROL_BUFFER_SIZE) /* COV_DLT_RESPONSE_MSG_LENGTH */
    {
      /* #30 Set the payload as expected: */
      /*      - The service succeeded. */
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)DLT_OK; /* Service succeeded */
      indexOfPayloadInControlResponse++;
      /*      - The number of log channels. */
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)numberOfLogChannels; /* Service succeeded */
      indexOfPayloadInControlResponse++;

      /*      - The log channel names. */
      VStdLib_MemCpy(&Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse], &Dlt_LogChannelNames[0], DLT_LENGTH_OF_LOG_CHANNEL_NAMES); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
    }
    /* #40 Otherwise, the response would be too large: */
    else
    {
      /* #50 Therefore, overwrite the response to indicate that an error occurred. */
      /* Note: A response with complete standard header, complete extended header, and two bytes payload always fit in the control buffer. */
      payloadLength = DLT_GET_LOG_CHANNEL_NAMES_PAYLOAD_LENGTH;

      indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, payloadLength);
      responseLength = indexOfPayloadInControlResponse + DLT_GET_LOG_CHANNEL_NAMES_PAYLOAD_LENGTH;

      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)DLT_ERROR; /* Service failed */
      indexOfPayloadInControlResponse++;
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = 0x00u; /* Number of log channels is 0. */
    }

    Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, responseLength);
  }
} /* Dlt_Srv_GetLogChannelNames */

/**********************************************************************************************************************
 * Dlt_Srv_StoreConfig
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_StoreConfig(void) /* 0x0005 */
{
# if (DLT_NVRAM_SUPPORT == STD_ON)
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  uint16 responseLength;
  uint16 indexOfPayloadInControlResponse;
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 If NvRamSupport is enabled, request to persist the current Dlt settings. */
  retVal = Dlt_Hlp_StoreConfiguration();

  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, DLT_STORE_CONFIG_PAYLOAD_LENGTH);
    responseLength = indexOfPayloadInControlResponse + DLT_STORE_CONFIG_PAYLOAD_LENGTH;

    if (retVal == E_OK)
    {
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)DLT_OK; /* Service succeeded */
    }
    else
    {
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)DLT_ERROR; /* Service failed */
    }

    Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, responseLength);
  }

# else
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise, the service StoreConfig is not supported. Build response frame with "DLT_NOT_SUPPORTED". */
  Dlt_Srv_ServiceNotSupported();
# endif
} /* Dlt_Srv_StoreConfig */

/**********************************************************************************************************************
 * Dlt_Srv_ResetToFactoryDefault
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_ResetToFactoryDefault(void) /* 0x0006 */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  uint16 indexOfPayloadInControlResponse;
  uint16 responseLength;
# if (DLT_NVRAM_SUPPORT == STD_ON)
  Std_ReturnType retVal;
# endif

  /* ----- Implementation ------------------------------------------------- */
# if (DLT_NVRAM_SUPPORT == STD_ON)
  retVal = Dlt_Hlp_ResetToFactoryDefault();

# else
  (void)Dlt_Hlp_ResetToFactoryDefault();
# endif

  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    /* #10 Prepare response message. */
    indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, DLT_RESET_TO_FACTORY_DEFAULT_PAYLOAD_LENGTH);

    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_OK;

# if (DLT_NVRAM_SUPPORT == STD_ON)
    if (retVal != E_OK)
    {
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_ERROR;
    }
# endif

    responseLength = indexOfPayloadInControlResponse + DLT_RESET_TO_FACTORY_DEFAULT_PAYLOAD_LENGTH;

    /* #20 Write response message to control buffer. */
    Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, responseLength);
  }
} /* Dlt_Srv_ResetToFactoryDefault */

/**********************************************************************************************************************
 * Dlt_Srv_GetSoftwareVersion
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetSoftwareVersion(void) /* 0x0013 */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  uint16 indexOfPayloadInControlResponse;

  /* ----- Implementation ------------------------------------------------- */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    uint8 asciiVersion[2] = {0u, 0u};
    boolean isPayLoadInBigEndian = FALSE;

    if (Dlt_IsPayloadInBigEndian())
    {
      isPayLoadInBigEndian = TRUE;
    }

    /* #10 Prepare response message. */
    indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, DLT_GET_SOFTWARE_VERSION_PAYLOAD_LENGTH);
    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_OK;

    indexOfPayloadInControlResponse++;
    /* #20 Write software version length in response message depending on endianness. */
    if (isPayLoadInBigEndian == TRUE)
    {
      Dlt_Hlp_CopyLongToArrayBigEndian_AndMoveIndex((uint32)8u, &Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse], &indexOfPayloadInControlResponse);
    }
    else
    {
      Dlt_Hlp_CopyLongToArrayLittleEndian_AndMoveIndex((uint32)8u, &Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse], &indexOfPayloadInControlResponse);
    }

    /* #30 Write software version of DLT in response message. Byte order is always big endian. */
    Dlt_Hlp_DecToAscii(asciiVersion, (uint8)DLT_SW_MAJOR_VERSION, isPayLoadInBigEndian);
    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)asciiVersion[1]; /* SW version - Major */
    indexOfPayloadInControlResponse++;
    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)asciiVersion[0]; /* SW version - Major */
    indexOfPayloadInControlResponse++;

    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)0x2E; /* '.' */
    indexOfPayloadInControlResponse++;

    Dlt_Hlp_DecToAscii(asciiVersion, (uint8)DLT_SW_MINOR_VERSION, isPayLoadInBigEndian);
    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)asciiVersion[1]; /* SW version - Minor */
    indexOfPayloadInControlResponse++;
    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)asciiVersion[0]; /* SW version - Minor */
    indexOfPayloadInControlResponse++;

    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)0x2E; /* '.' */
    indexOfPayloadInControlResponse++;

    Dlt_Hlp_DecToAscii(asciiVersion, (uint8)DLT_SW_PATCH_VERSION, isPayLoadInBigEndian);
    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)asciiVersion[1]; /* SW version - Patch */
    indexOfPayloadInControlResponse++;
    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = (uint8)asciiVersion[0]; /* SW version - Patch */
    indexOfPayloadInControlResponse++;

    /* #40 Write response message to control buffer. */
    Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, indexOfPayloadInControlResponse);
  }
} /* Dlt_Srv_GetSoftwareVersion */

/**********************************************************************************************************************
 * Dlt_Srv_MessageBufferOverflow
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_MessageBufferOverflow(void) /* 0x0014 - Not supported */
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 The requested service is not supported by DLT, respond with "DLT_NOT_SUPPORTED".*/
  Dlt_Srv_ServiceNotSupported();
} /* Dlt_Srv_MessageBufferOverflow */

/**********************************************************************************************************************
 * Dlt_Srv_SetLogChannelAssignment
 *********************************************************************************************************************/
/*!
 *
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
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetLogChannelAssignment(void) /* 0x0020 */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 requestedAppId;
  uint32 requestedContextId;
  uint32 requestedLogChannel;
  uint32 logChannelIndex;
  uint16 responseLength;
  uint16 payloadLength = DLT_SET_LOG_CHANNEL_ASSIGNMENT_PAYLOAD_LENGTH;
  uint16 indexOfPayloadInControlResponse;
  uint16 indexOfPayloadInControlRequest;
  Dlt_AssignmentOperation addRemoveOp;
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */

  /* #10 Iterate over all log channels. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    retVal = E_OK;

    /* #20 Prepare the response. */
    indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, payloadLength);
    indexOfPayloadInControlRequest = indexOfPayloadInControlResponse;
    responseLength = indexOfPayloadInControlResponse + payloadLength;

    /* #30 Get the parameter from the payload of received message:
     *       The application id, the context id, the log channel, and the assignment operation (add or remove). */
    Dlt_Hlp_CopyArrayToLongBigEndian(&Dlt_ReceiveMessage[indexOfPayloadInControlRequest], &requestedAppId);
    indexOfPayloadInControlRequest += DLT_DEFAULT_ID_LENGTH;
    Dlt_Hlp_CopyArrayToLongBigEndian(&Dlt_ReceiveMessage[indexOfPayloadInControlRequest], &requestedContextId);
    indexOfPayloadInControlRequest += DLT_DEFAULT_ID_LENGTH;
    Dlt_Hlp_CopyArrayToLongBigEndian(&Dlt_ReceiveMessage[indexOfPayloadInControlRequest], &requestedLogChannel);
    indexOfPayloadInControlRequest += DLT_DEFAULT_ID_LENGTH;
    addRemoveOp = (Dlt_AssignmentOperation)Dlt_ReceiveMessage[indexOfPayloadInControlRequest];

    /* #40 Swap the assignment operation to ECU internal values. */
    if (addRemoveOp == DLT_SET_LOG_CHANNEL_ASSIGNMENT_REMOVE_FROM_REQ)
    {
      addRemoveOp = DLT_ASSIGN_REMOVE;
    }
    else
    if (addRemoveOp == DLT_SET_LOG_CHANNEL_ASSIGNMENT_ADD_FROM_REQ)
    {
      addRemoveOp = DLT_ASSIGN_ADD;
    }
    else
    {
      retVal = E_NOT_OK;
    }

    /* #50 If the assignment operation is valid, execute the operation. */
    if (retVal == E_OK)
    {
      retVal = Dlt_Hlp_SetLogChannelAssignment(requestedAppId, requestedContextId, requestedLogChannel, addRemoveOp);
    }

    /* #60 If the assignment operation succeeded, indicate this in the response.
     *     Otherwise, indicate an error. */
    if (retVal == E_OK)
    {
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_OK;
    }
    else
    {
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_ERROR;
    }

    /* #70 Write the response frame to control buffer.*/
    Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, responseLength);
  }
} /* Dlt_Srv_SetLogChannelAssignment */

/**********************************************************************************************************************
 * Dlt_Srv_SetLogChannelThreshold
 *********************************************************************************************************************/
/*!
 *
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
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SetLogChannelThreshold(void) /* 0x0021 */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  uint32 requestedLogChannel;
  uint32 requestedLogChannelIndex;
  uint16 responseLength;
  uint16 payloadLength = DLT_SET_LOG_CHANNEL_THRESHOLD_PAYLOAD_LENGTH;
  uint16 indexOfPayloadInControlResponse;
  uint16 indexOfPayloadInControlRequest;
  Dlt_MessageLogLevelType newLogLevel;
  boolean newTraceStatus;
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Iterate over all log channels. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    /* #20 Prepare the response. */
    indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, payloadLength);
    indexOfPayloadInControlRequest = indexOfPayloadInControlResponse;
    responseLength = indexOfPayloadInControlResponse + payloadLength;

    /* #30 Get the parameter from the payload of received message:
     *       The log channel name, the new threshold, and the new trace status. */
    Dlt_Hlp_CopyArrayToLongBigEndian(&Dlt_ReceiveMessage[indexOfPayloadInControlRequest], &requestedLogChannel);
    indexOfPayloadInControlRequest += 4u;
    newLogLevel = (Dlt_MessageLogLevelType)Dlt_ReceiveMessage[indexOfPayloadInControlRequest];
    indexOfPayloadInControlRequest += 1u;
    newTraceStatus = (boolean)Dlt_ReceiveMessage[indexOfPayloadInControlRequest]; /* PRQA S 4340 */ /* MD_MSR_AutosarBoolean */

    requestedLogChannelIndex = DLT_INVALID_LOG_CHANNEL_INDEX;
    retVal = Dlt_Hlp_GetLogChannelIndexByLogChannelName(requestedLogChannel, &requestedLogChannelIndex);

    /*@ assert requestedLogChannelIndex < Dlt_GetNumberOfAllValidLogChannels(); */ /* VCA_DLT_VALID_CONFIGURED_LOGCHANNEL_ID */

    /* #40 If the request targets a valid log channel, set the current log channel threshold and trace status to
     *       the new values and indicate success.
     *     Otherwise, the log channel is invalid, therefore indicate an error. */
    if (retVal == E_OK)
    {
      retVal = Dlt_Hlp_SetLogChannelThresholdByIndex(requestedLogChannelIndex, newLogLevel, newTraceStatus);

      if (retVal == E_OK)
      {
        Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_OK;
        indexOfPayloadInControlResponse += 1u;
        Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = Dlt_LogChannelInfo[requestedLogChannelIndex].logChannelTraceStatus; /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
      }
    }

    if (retVal != E_OK)
    {
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_ERROR;
      indexOfPayloadInControlResponse += 1u;
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = 0x00u; /* Indicate that trace status is off */
    }

    /* #50 Write the response frame to control buffer.*/
    Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, responseLength);
  }
} /* Dlt_Srv_SetLogChannelThreshold */

/**********************************************************************************************************************
 * Dlt_Srv_GetLogChannelThreshold
 *********************************************************************************************************************/
/*!
 *
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
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_GetLogChannelThreshold(void) /* 0x0022 */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
  uint32 requestedLogChannel;
  uint32 requestedLogChannelIndex;
  uint16 responseLength;
  uint16 payloadLength = DLT_GET_LOG_CHANNEL_THRESHOLD_PAYLOAD_LENGTH;
  uint16 indexOfPayloadInControlResponse;
  uint16 indexOfPayloadInControlRequest;
  Std_ReturnType retVal;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Iterate over all log channels. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    /* #20 Prepare the response. */
    indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, payloadLength);
    indexOfPayloadInControlRequest = indexOfPayloadInControlResponse;
    responseLength = indexOfPayloadInControlResponse + payloadLength;

    /* #30 Get the parameter from the payload of received message:
     *       The log channel name. */
    Dlt_Hlp_CopyArrayToLongBigEndian(&Dlt_ReceiveMessage[indexOfPayloadInControlRequest], &requestedLogChannel);

    requestedLogChannelIndex = DLT_INVALID_LOG_CHANNEL_INDEX;
    retVal = Dlt_Hlp_GetLogChannelIndexByLogChannelName(requestedLogChannel, &requestedLogChannelIndex);

    /*@ assert requestedLogChannelIndex < Dlt_GetNumberOfAllValidLogChannels(); */ /* VCA_DLT_VALID_CONFIGURED_LOGCHANNEL_ID */

    /* #40 If the request targets a valid log channel, set the current log channel threshold and trace status in
     *       the response and indicate success.
     *     Otherwise, the log channel is invalid, therefore indicate an error. */
    if (retVal == E_OK)
    {
      Dlt_MessageLogLevelType logLevel;
      boolean traceStatus;

      Dlt_Hlp_GetLogChannelThresholdByIndex(requestedLogChannelIndex, &logLevel, &traceStatus);

      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_OK;
      indexOfPayloadInControlResponse += 1u;
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = logLevel;
      indexOfPayloadInControlResponse += 1u;
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = traceStatus; /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
    }
    else
    {
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_ERROR;
      indexOfPayloadInControlResponse += 1u;
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = 0x00u; /* Indicate that log level is off */
      indexOfPayloadInControlResponse += 1u;
      Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = 0x00u; /* Indicate that trace status is off */
    }

    /* #50 Write the response frame to control buffer.*/
    Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, responseLength);
  }
} /* Dlt_Srv_GetLogChannelThreshold */

/**********************************************************************************************************************
 * Dlt_Srv_BufferOverflowNotification
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_BufferOverflowNotification(void) /* 0x0023 */
{
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Build response frame according to received message and return flag status of Dlt_BufferOverflowStatus. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    Dlt_Srv_PrepareBufferOverflowNotificationOfLogChannel(logChannelIndex);
  }

} /* Dlt_Srv_BufferOverflowNotification */

/**********************************************************************************************************************
 * Dlt_Srv_SyncTimeStamp
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_SyncTimeStamp(void) /* 0x0024 */
{
# if (DLT_SYNC_TIME_SUPPORT == STD_ON)
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If SyncTimeStamp support is enabled:
   *       Build response frame and return he current sync time stamp.
   *     Otherwise:
   *       Build response frame with "DLT_NOT_SUPPORTED".
   */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    Dlt_Srv_PrepareSyncTimeStampOfLogChannel(logChannelIndex);
  }
# else
  Dlt_Srv_ServiceNotSupported();
# endif
} /* Dlt_Srv_SyncTimeStamp */

#  if (DLT_INJECTION_CALL_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_Srv_CallSwcInjection
 *********************************************************************************************************************/
/*!
 *
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
/* PRQA S 6010 2 */ /* MD_Dlt_STPTH_Extended */
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_Srv_CallSwcInjection(uint32 ServiceId) /* 0x00000FFF - 0xFFFFFFFF */
{
  /* ----- Local Variables ------------------------------------------------ */
  /* Note: According to [PRS_Dlt_00218] all ids (application, context and session id) must be filled in the header of
   *       this message. */
  uint32 logChannelIndex;
  uint16 responseLength;
  uint16 indexOfPayloadInControlResponse;
  Std_ReturnType retVal = E_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Implementation ------------------------------------------------- */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    /* #10 Build response frame depending on received message. */
    indexOfPayloadInControlResponse = Dlt_TxBuf_PrepareRespInCtrlBuf(logChannelIndex, DLT_CALL_SWC_INJECTION_PAYLOAD_LENGTH);
    responseLength = indexOfPayloadInControlResponse + 1u;
    Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_ERROR; /* Will be set to DLT_OK if successful. */

    {
      uint32 dataLength;
      uint32 applicationId;
      uint32 contextId;
      uint32 sessionId = 0u;
      uint16 appIdIndexInRxMsg;
      uint16 contextIdIndexInRxMsg;
      uint16 sessionIdIndexInRxMsg = DLT_CALL_SWC_INJECTION_START_IDX_OF_SESSION_ID;
      uint16 dataLengthIndexInRxMsg;
      uint16 dataIndexInRxMsg;
      boolean isSessionIdAvailable = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */

      /* #20 If the session id is given in the request, it must fit to given application and context id. */
      if ((Dlt_ReceiveMessage[DLT_STD_HEADER_HEADERTYPE_INDEX] & Dlt_StandardHeaderType_WSID_Mask) == Dlt_StandardHeaderType_WSID_Mask)
      {
        isSessionIdAvailable = TRUE;
      }

      /* #30 If the ECU id is in the received control message, the session id is at index 8.
       *     Otherwise, it is at index 4. */
      if ((Dlt_ReceiveMessage[DLT_STD_HEADER_HEADERTYPE_INDEX] & Dlt_StandardHeaderType_WEID_Mask) == Dlt_StandardHeaderType_WEID_Mask)
      {
        sessionIdIndexInRxMsg += DLT_DEFAULT_ID_LENGTH;
      }

      /* Structure of a SWC injection control message:
       *   | Standard header (4-16 Bytes) | Extended header (10 Bytes) | Payload (8 - x Bytes) |
       * Payload:
       *   | Service id (4 Bytes) | DataLength (4 Bytes) | Data (DataLength Bytes) | */
      /* #40 Set the indexes of application id, context id, datalength and data depending on the optional parts of
       *     standard header. */
      /* Start index is 4 byte for HTYP/MCNT/LEN of standard header and 2 Byte for MSIN/NOAR of extended header. */
      appIdIndexInRxMsg = DLT_CALL_SWC_INJECTION_START_IDX_OF_APPLICATION_ID;
      Dlt_Hlp_IncrementIndexDependingOnOptionalStandardHeader(&appIdIndexInRxMsg);
      contextIdIndexInRxMsg = appIdIndexInRxMsg + DLT_DEFAULT_ID_LENGTH;
      dataLengthIndexInRxMsg = contextIdIndexInRxMsg + (2u * DLT_DEFAULT_ID_LENGTH); /* Context id + Service id */
      dataIndexInRxMsg = dataLengthIndexInRxMsg + DLT_DEFAULT_ID_LENGTH;

      /* #50 Copy the (optional) session, application and context id from received message to local variables using big endian. */
      /* Note: Standard and extended header are always in big endian. */
      Dlt_Hlp_CopyStringToLongBigEndian(&Dlt_ReceiveMessage[appIdIndexInRxMsg], &applicationId);
      Dlt_Hlp_CopyStringToLongBigEndian(&Dlt_ReceiveMessage[contextIdIndexInRxMsg], &contextId);
      if (isSessionIdAvailable == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
      {
        Dlt_Hlp_CopyArrayToLongBigEndian(&Dlt_ReceiveMessage[sessionIdIndexInRxMsg], &sessionId);
      }

      /* #60 Depending on endianness of received message, copy parameter 'dataLength' to local variable. */
      if (Dlt_IsPayloadInBigEndian())
      {
        Dlt_Hlp_CopyArrayToLongBigEndian(&Dlt_ReceiveMessage[dataLengthIndexInRxMsg], &dataLength);
      }
      else
      {
        Dlt_Hlp_CopyArrayToLongLittleEndian(&Dlt_ReceiveMessage[dataLengthIndexInRxMsg], &dataLength);
      }

      {
        const uint8 * appIdInRxMsg = &Dlt_ReceiveMessage[appIdIndexInRxMsg];
        const uint8 * contextIdInRxMsg = &Dlt_ReceiveMessage[contextIdIndexInRxMsg];
        const uint8 * dataInRxMsg = &Dlt_ReceiveMessage[dataIndexInRxMsg];
        uint16 appIdIndexInDltContext;
        uint16 contextIdIndexInDltContext;
        boolean areTheGivenIDsRegistered;
        boolean isApplAlreadyRegistered = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */

        /* #70 If the three ids (application, context and session) are registered together:
         *       Call the corresponding injection callout and report its result.
         *     Otherwise, report that this request is not supported. */
        if (isSessionIdAvailable == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
        {
          areTheGivenIDsRegistered = Dlt_Hlp_CheckSessionId(
            sessionId,
            applicationId,
            contextId,
            &appIdIndexInDltContext,
            &contextIdIndexInDltContext,
            &isApplAlreadyRegistered);
        }
        else
        {
          areTheGivenIDsRegistered = Dlt_Hlp_IsContextRegistered(
            applicationId,
            contextId,
            &appIdIndexInDltContext,
            &contextIdIndexInDltContext,
            &isApplAlreadyRegistered);

          /*@ assert appIdIndexInDltContext < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
          /*@ assert contextIdIndexInDltContext < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */
        }
        (void)isApplAlreadyRegistered;

        if (areTheGivenIDsRegistered == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
        {
          /* #80 Only call injection services of application once. */
#   if (DLT_NUMBER_OF_LOG_CHANNELS > 1)
          if (logChannelIndex == 0u)
#   endif
          {
            Dlt_InjectionCallOpFuncType injectCallFct = Dlt_Context.appIds[appIdIndexInDltContext].contextInfoList[contextIdIndexInDltContext].cbkInfo.Dlt_InjectCall_;

            /*@ assert injectCallFct == Dlt_InjectionCallOpClass[$range(0, DLT_NUMBER_OF_SWCS-1)]; */ /* VCA_DLT_VALID_INJECT_CALL_FCT */
            retVal = injectCallFct(
              appIdInRxMsg,
              contextIdInRxMsg,
              ServiceId,
              dataLength,
              dataInRxMsg);
          }

          if (retVal == E_OK)
          {
            Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_OK;
          }
        }
        else
        {
          Dlt_ControlSendMessage[logChannelIndex][indexOfPayloadInControlResponse] = DLT_NOT_SUPPORTED;
        }
      }
    }

    /* #90 Write the response frame to control buffer.*/
    Dlt_TxBuf_WriteToCtrlBuf(logChannelIndex, responseLength);
  }
} /* Dlt_Srv_CallSwcInjection() */
#  endif /* (DLT_INJECTION_CALL_SUPPORT == STD_ON) */
# endif /* (DLT_RX_DATA_PATH_SUPPORT == STD_ON) */

/**********************************************************************************************************************
 * Dlt_RegCont_RegisterNewContext
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 * \spec        requires parg0 > 0x1000u;
 *              requires AppId != NULL_PTR;
 *              requires ContextId != NULL_PTR;
 * \endspec
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
DLT_LOCAL FUNC(void, DLT_CODE) Dlt_RegCont_RegisterNewContext(
  Dlt_SessionIDType SessionId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextId,
  uint16 AppIdIndex,
  uint16 ConIdIndex,
  boolean applicationAlreadyRegistered)
{
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_LocalApplicationIDType appId;
  Dlt_LocalContextIDType contextId;
  Dlt_SetLogLevelOpFuncType setLogLevelFct;
  Dlt_SetTraceStatusOpFuncType setTraceStatusFct;
  boolean isChangeNotificationActive = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If specified session ID is in valid range, specify callback functions for log level and trace status. */
  if ((SessionId - 0x1000u) < DLT_NUMBER_OF_SWCS)
  {
    uint16_least swcIndex;

    for (swcIndex = 0u; swcIndex < Dlt_GetSizeOfSwcDescriptor(); swcIndex++) /* COV_DLT_LOG_LVL_CHG_NOTIFICATION */
    {
      if (Dlt_GetSessionIdOfSwcDescriptor(swcIndex) == SessionId) /* COV_DLT_LOG_LVL_CHG_NOTIFICATION */
      {
        if (Dlt_IsSupportLogLevelChangeNotificationOfSwcDescriptor(swcIndex)) /* COV_DLT_LOG_LVL_CHG_NOTIFICATION */
        {
          isChangeNotificationActive = TRUE;
          break;
        }
      }
    }

    if (isChangeNotificationActive == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */ /* COV_DLT_LOG_LVL_CHG_NOTIFICATION */
    {
      Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].cbkInfo.Dlt_SetLogLevel_ = Dlt_SetLogLevelOpClass[SessionId - 0x1000u];
      Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].cbkInfo.Dlt_SetTraceStatus_ = Dlt_SetTraceStatusOpClass[SessionId - 0x1000u];
    }
    Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].cbkInfo.Dlt_InjectCall_ = Dlt_InjectionCallOpClass[SessionId - 0x1000u];
  }

  /* #20 If context is registered, increase firstly the number of context IDs, then increase number of application IDs. */
  Dlt_Context.appIds[AppIdIndex].contextIdCount++;

  if (applicationAlreadyRegistered == FALSE) /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
  {
    Dlt_Context.appIdCount++;
  }

  Dlt_Hlp_CopyStringToLongBigEndian(AppId, &appId);
  Dlt_Hlp_CopyStringToLongBigEndian(ContextId, &contextId);

  /* #30 If applicable, inform SWC about default values. */
  setLogLevelFct = Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].cbkInfo.Dlt_SetLogLevel_;
  setTraceStatusFct = Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].cbkInfo.Dlt_SetTraceStatus_;

  if (isChangeNotificationActive == TRUE) /* COV_DLT_LOG_LVL_CHG_NOTIFICATION */
  {
    /*@ assert setLogLevelFct == Dlt_SetLogLevelOpClass[$range(0, DLT_NUMBER_OF_SWCS-1)]; */ /* VCA_DLT_VALID_LOG_LVL_CHG_NOTIFICATION_FCT */
    /*@ assert setTraceStatusFct == Dlt_SetTraceStatusOpClass[$range(0, DLT_NUMBER_OF_SWCS-1)]; */ /* VCA_DLT_VALID_TRACE_STATUS_FCT */
    (void)setLogLevelFct(
            AppId,
            ContextId,
            Dlt_DefaultMaxLogLevel);
    (void)setTraceStatusFct(
            AppId,
            ContextId,
            Dlt_DefaultTraceStatus);
  }
} /* Dlt_RegCont_RegisterNewContext */

/**********************************************************************************************************************
 * Dlt_Trace_SendTraceMessage
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DLT_LOCAL FUNC(Std_ReturnType, DLT_CODE) Dlt_Trace_SendTraceMessage(
  Dlt_SessionIDType SessionId,
  P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_APPL_VAR) TraceInfo,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) TraceData,
  uint16 TraceDataLength,
  Dlt_UsedEndiannessType UsedEndianness)
{ /* Dlt241 */
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retVal = (Dlt_ReturnType)DLT_E_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 If global state is DLT_GLOBAL_STATE_UNINIT or DLT_GLOBAL_STATE_ONLINE: */
  if ((Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
   || (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE))
  {
    Dlt_MessageTypeType msgType;
    Dlt_FilterInfoType filterInfo;

    msgType = (uint8)(TraceInfo->options >> 1u);

    if ((msgType & DLT_TYPE_APP_TRACE) == DLT_TYPE_APP_TRACE)
    {
      filterInfo.messageType = (Dlt_MessageTypeType)DLT_TYPE_APP_TRACE;
      retVal = (Dlt_ReturnType)DLT_E_OK;
    }
    else if ((msgType & DLT_TYPE_NW_TRACE) == DLT_TYPE_NW_TRACE)
    {
      filterInfo.messageType = (Dlt_MessageTypeType)DLT_TYPE_NW_TRACE;
      retVal = (Dlt_ReturnType)DLT_E_OK;
    }
    else /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
    {
      /* Do nothing */
    }

    /* #20 If requested message is of type DLT_TYPE_APP_TRACE or DLT_TYPE_NW_TRACE:
     *       Write the trace message to the send buffer of all assigned log channels. */
    if (retVal == (Dlt_ReturnType)DLT_E_OK)
    {
      Dlt_LocalApplicationIDType appId;
      Dlt_LocalContextIDType contextId;

      Dlt_Hlp_CopyStringToLongBigEndian(TraceInfo->appId, &appId);
      Dlt_Hlp_CopyStringToLongBigEndian(TraceInfo->context, &contextId);

      filterInfo.sessionId = SessionId;
      filterInfo.traceInfo.appId = appId;
      filterInfo.traceInfo.context = contextId;
      filterInfo.traceInfo.options = TraceInfo->options;
      filterInfo.traceInfo.traceInfo = TraceInfo->traceInfo;
      filterInfo.usedEndianness = UsedEndianness;

      retVal = Dlt_TxMsg_Transmit(&filterInfo, TraceDataLength, TraceData);
    }
  }

  /* #30 Indicate if the requested message was successfully buffered in at least one log channel or if anything failed. */
  return (Std_ReturnType)retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_Trace_SendTraceMessage */

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

/**********************************************************************************************************************
 * Dlt_TpTxConfirmation
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DLT_CODE) Dlt_TpTxConfirmation(PduIdType TxPduId, Std_ReturnType Result)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 Transmission confirmation is only relevant in global state DLT_GLOBAL_STATE_ONLINE. */
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
  {
    uint32_least logChannelIndex;

    /* #20 If the confirmed Tx PDU fits to a log channel, set flag Dlt_MessageTransmitted and reset timeout of this
     *     log channel. */
    for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++) /* COV_DLT_LOG_CHNL_OPTION */
    {
      if (TxPduId == Dlt_GetLogChannelTxPduSNVOfDltOfLogChannelDescriptor(logChannelIndex))
      {
        /* Ignore the result because the transmitted DLT messages are gone anyways (no longer buffered in DLT). */
        Dlt_MessageTransmitted[logChannelIndex] = TRUE; /* PRQA S 4404 */ /* MD_MSR_AutosarBoolean */
        Dlt_Timeout[logChannelIndex] = 0u;
        break;
      }
    }
  }
  DLT_DUMMY_STATEMENT(Result); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
  /*lint -e{438} */ /* PC_Dlt_438 */
} /* Dlt_TpTxConfirmation */

/**********************************************************************************************************************
 * Dlt_TpRxIndication
 *********************************************************************************************************************/
/*!
 *
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
FUNC(void, DLT_CODE) Dlt_TpRxIndication(PduIdType RxPduId, Std_ReturnType Result)
{
  /* ----- Implementation ------------------------------------------------- */
# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
  /* #10 If global state is DLT_GLOBAL_STATE_ONLINE and the given RxPduId is valid. */
  if ((Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
   && (RxPduId == Dlt_GetRxPduSNVOfDltOfRxPduDescriptor(0))) /* ESCAN00116164 */
  {
    /* #20 Enter Dlt_DLT_EXCLUSIVE_AREA_RX. */
    DLT_BEGIN_CRITICAL_SECTION_RX();

    /* #30 If success is indicated and the received data had a known length (first CopyRxData already provided data): */
    if ((Result == E_OK)
     && (Dlt_RxInfo.IsRxIndicationRelevant == TRUE))
    {
      uint32 bufferIndex = Dlt_RxInfo.InitialWriteIndexOfDltMessage;
      uint16 dltMessageLength = 0u;
      Dlt_RxInfo.IsRxIndicationRelevant = FALSE;

      /* #40 Iterate over all received Dlt messages:
       *       For each received Dlt message increment the RxIndicationOccured counter.
       *       If only a part of a Dlt message was received, omit this part as segmented Dlt messages are not
       *       supported. */
      while (Dlt_RxInfo.DataLengthToBeReceived > 4u)
      {
        bufferIndex += 4u;

        if (bufferIndex >= DLT_CONTROL_BUFFER_SIZE)
        {
          bufferIndex -= DLT_CONTROL_BUFFER_SIZE;
        }

        Dlt_RxInfo.CopyRxDataLength = 4u;

        /*@ assert bufferIndex < DLT_CONTROL_BUFFER_SIZE; */ /* VCA_DLT_VALID_RECEIVE_BUFFER_IDX */
        dltMessageLength = Dlt_Hlp_GetLengthOfReceivedMsg(bufferIndex);
        Dlt_RxInfo.CopyRxDataCounter = 0u;
        Dlt_RxInfo.CopyRxDataLength = 0u;
        Dlt_RxInfo.RxIndicationOccured++;
        bufferIndex += dltMessageLength;

        if (bufferIndex >= DLT_CONTROL_BUFFER_SIZE)
        {
          bufferIndex -= DLT_CONTROL_BUFFER_SIZE;
        }

        if ((dltMessageLength + 4u) <= Dlt_RxInfo.DataLengthToBeReceived)
        {
          Dlt_RxInfo.DataLengthToBeReceived -= (dltMessageLength + 4u);
        }
        else
        {
          if (Dlt_ReceiveBuffer.Write >= Dlt_RxInfo.DataLengthToBeReceived)
          {
            Dlt_ReceiveBuffer.Write -= Dlt_RxInfo.DataLengthToBeReceived;
          }
          else
          {
            Dlt_ReceiveBuffer.Write = (uint32)(DLT_CONTROL_BUFFER_SIZE - (uint32)Dlt_RxInfo.DataLengthToBeReceived + Dlt_ReceiveBuffer.Write);
          }
          Dlt_RxInfo.DataLengthToBeReceived = 0u;
          Dlt_RxInfo.RxIndicationOccured--;
        }
      }

      Dlt_RxInfo.InitialWriteIndexOfDltMessage = 0u;
      Dlt_RxInfo.DataLengthToBeReceived = 0u;
    }

    /* #50 Leave Dlt_DLT_EXCLUSIVE_AREA_RX. */
    DLT_END_CRITICAL_SECTION_RX();
  }
#else
  DLT_DUMMY_STATEMENT(RxPduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  DLT_DUMMY_STATEMENT(Result); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif /* # if (DLT_RX_DATA_PATH_SUPPORT == STD_ON) */
  /*lint -e{438} */ /* PC_Dlt_438 */
} /* Dlt_TpRxIndication */

/**********************************************************************************************************************
 * Dlt_StartOfReception
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 3673 5 */ /* MD_Dlt_3673 */
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_StartOfReception(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DLT_APPL_VAR) info,            /* payload of the first frame and its length */
  PduLengthType TpSduLength,                                   /* The length of the complete N-SDU that has to be received. */
  P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) bufferSizePtr) /* Available receiving buffer of Dlt */
{
  /* ----- Local Variables ------------------------------------------------ */
  BufReq_ReturnType retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
  uint32 freeBuffer;

  /* ----- Implementation ------------------------------------------------- */
  DLT_DUMMY_STATEMENT(info); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  /* #10 Reception of messages is only allowed in global state DLT_GLOBAL_STATE_ONLINE. */
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
  {
    /* #20 Enter Dlt_DLT_EXCLUSIVE_AREA_RX. */
    DLT_BEGIN_CRITICAL_SECTION_RX();

    if (id == Dlt_GetRxPduSNVOfDltOfRxPduDescriptor(0))
    {
      /* #30 Calculate free space in receive buffer. */
      if (Dlt_ReceiveBuffer.Write >= Dlt_ReceiveBuffer.Read)
      {
        freeBuffer = (uint32)((DLT_CONTROL_BUFFER_SIZE - Dlt_ReceiveBuffer.Write) + Dlt_ReceiveBuffer.Read);
      }
      else /* if (Dlt_ReceiveBuffer.Write < Dlt_ReceiveBuffer.Read) */
      {
        freeBuffer = (uint32)(Dlt_ReceiveBuffer.Read - Dlt_ReceiveBuffer.Write);
      }

      /* #40 If the incoming pdu fits in receive buffer, return free buffer size.
       *     Otherwise, return overflow. */
      if (TpSduLength <= (uint16)freeBuffer)
      {
        retVal = BUFREQ_OK;
        *bufferSizePtr = (uint16)freeBuffer;
        Dlt_RxInfo.DataLengthToBeReceived = TpSduLength; /* ESCAN00116164 */
        Dlt_RxInfo.InitialWriteIndexOfDltMessage = Dlt_ReceiveBuffer.Write;
      }
      else
      {
        retVal = BUFREQ_E_OVFL;
      }
    }

    /* #50 Leave Dlt_DLT_EXCLUSIVE_AREA_RX. */
    DLT_END_CRITICAL_SECTION_RX();
  }
# else
  /* ----- Implementation ------------------------------------------------- */
  DLT_DUMMY_STATEMENT(id); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  DLT_DUMMY_STATEMENT(info); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  DLT_DUMMY_STATEMENT(TpSduLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  DLT_DUMMY_STATEMENT(bufferSizePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  /*lint -e{438} */ /* PC_Dlt_438 */
  return retVal;
} /* Dlt_StartOfReception */

/**********************************************************************************************************************
 * Dlt_CopyRxData
 *********************************************************************************************************************/
/*!
 *
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
/* PRQA S 3673 6 */ /* MD_Dlt_3673 */
/* PRQA S 6030 2 */ /* MD_MSR_STCYC */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_CopyRxData(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DLT_APPL_VAR) info,
  P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) bufferSizePtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  BufReq_ReturnType retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
# if (DLT_RX_DATA_PATH_SUPPORT == STD_ON)
  uint32_least i;
  uint32 bufferIndex = Dlt_ReceiveBuffer.Write;
  uint16 locLength;

  /* ----- Implementation ------------------------------------------------- */
  DLT_DUMMY_STATEMENT(id); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
  *bufferSizePtr = 0u;

  /* #10 Reception of messages is only allowed in global state DLT_GLOBAL_STATE_ONLINE. */
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
  {
    /* #20 Enter Dlt_DLT_EXCLUSIVE_AREA_RX. */
    DLT_BEGIN_CRITICAL_SECTION_RX();

    if (id == Dlt_GetRxPduSNVOfDltOfRxPduDescriptor(0))
    {
      /* #30 If no data is passed within SDU, it is the first call of Dlt_CopyRxData.
       *       Request the first 4 bytes of received message.
       */
      if ((info->SduLength == 0u) && (Dlt_RxInfo.CopyRxDataLength == 0u))
      {
        Dlt_RxInfo.IsRxIndicationRelevant = FALSE;
        /* Request 4Byte data (Standard Header of DLT frame) */
        *bufferSizePtr = 4u;
        Dlt_RxInfo.CopyRxDataCounter = 1u;
        retVal = (BufReq_ReturnType)BUFREQ_OK;
      }
      /* #40 Otherwise, if the first four bytes are already requested:
       *       If more than 4 bytes are passed, reject received message.
       *       Otherwise:
       *         Copy received bytes to receive buffer.
       *         If less than 4 byte are received, request the missing byte.
       *         Otherwise:
       *           Get the received message length from received message (byte 2 and 3) and request the missing bytes.
       */
      else
      if (Dlt_RxInfo.CopyRxDataCounter == 1u)
      {
        if ((Dlt_RxInfo.CopyRxDataLength + info->SduLength) > 4u)
        {
          retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
          Dlt_RxInfo.CopyRxDataCounter = 0u;
          Dlt_RxInfo.CopyRxDataLength = 0u;
        }
        else
        {
          for (i = 0u; i < info->SduLength; i++)
          {
            /*@ assert bufferIndex < DLT_CONTROL_BUFFER_SIZE; */ /* VCA_DLT_VALID_RECEIVE_BUFFER_IDX */
            Dlt_ReceiveBuffer.Buffer[bufferIndex] = info->SduDataPtr[i];
            bufferIndex++;
            if (bufferIndex >= DLT_CONTROL_BUFFER_SIZE)
            {
              bufferIndex = 0u;
            }
          }
          Dlt_ReceiveBuffer.Write = bufferIndex;
          Dlt_RxInfo.CopyRxDataLength = Dlt_RxInfo.CopyRxDataLength + info->SduLength;

          /* If 4Byte are received, the length of the complete DLT frame is available */
          /*@ assert bufferIndex < DLT_CONTROL_BUFFER_SIZE; */ /* VCA_DLT_VALID_RECEIVE_BUFFER_IDX */
          locLength = Dlt_Hlp_GetLengthOfReceivedMsg(bufferIndex);

          *bufferSizePtr = locLength;
          retVal = (BufReq_ReturnType)BUFREQ_OK;
        }
      }
      /* #50 Otherwise, if the first 4 bytes are successfully received:
       *       If the number of bytes to be passed is greater than requested, reject the message.
       *       Otherwise:
       *         Copy remaining/available bytes of received message to receive buffer.
       */
      else
      if (Dlt_RxInfo.CopyRxDataCounter == 2u)
      {
        if (info->SduLength > Dlt_RxInfo.CopyRxDataLength)
        {
          retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
          Dlt_RxInfo.CopyRxDataCounter = 0u;
          Dlt_RxInfo.CopyRxDataLength = 0u;
        }
        else
        {
          /* Copy Rx Data to Dlt_ReceiveBuffer */
          for (i = 0u; i < info->SduLength; i++)
          {
            /*@ assert bufferIndex < DLT_CONTROL_BUFFER_SIZE; */ /* VCA_DLT_VALID_RECEIVE_BUFFER_IDX */
            Dlt_ReceiveBuffer.Buffer[bufferIndex] = info->SduDataPtr[i];
            bufferIndex++;
            if (bufferIndex >= (uint32)DLT_CONTROL_BUFFER_SIZE)
            {
              bufferIndex = 0u;
            }
          }
          Dlt_ReceiveBuffer.Write = bufferIndex;
          Dlt_RxInfo.CopyRxDataLength = Dlt_RxInfo.CopyRxDataLength - info->SduLength;

          if (Dlt_RxInfo.CopyRxDataLength == 0u)
          { /* The complete DLT message is received */
            Dlt_RxInfo.RxIndicationOccured++;
            Dlt_RxInfo.CopyRxDataCounter = 1u;
            *bufferSizePtr = 4u;
          }
          else
          {
            *bufferSizePtr = Dlt_RxInfo.CopyRxDataLength;
          }
          retVal = (BufReq_ReturnType)BUFREQ_OK;
        }
      }
      /* #60 Otherwise if the lower layer provides already data in first call of CopyRxData: */
      else
      if ((info->SduLength > 0u)
       && (Dlt_RxInfo.DataLengthToBeReceived > 0u)) /* ESCAN00116164 */
      {
        Dlt_RxInfo.IsRxIndicationRelevant = TRUE;

        for (i = 0u; i < info->SduLength; i++)
        {
          /*@ assert bufferIndex < DLT_CONTROL_BUFFER_SIZE; */ /* VCA_DLT_VALID_RECEIVE_BUFFER_IDX */
          Dlt_ReceiveBuffer.Buffer[bufferIndex] = info->SduDataPtr[i];
          bufferIndex++;
          if (bufferIndex >= DLT_CONTROL_BUFFER_SIZE)
          {
            bufferIndex = 0u;
          }
        }
        Dlt_RxInfo.CopyRxDataLength = Dlt_RxInfo.CopyRxDataLength + info->SduLength;

        /* #65 If all expected data is already copied:
         *       The complete message is received, therefore fake a RxIndication, return no buffer left, and reset
         *       the length variables. */
        if (Dlt_RxInfo.CopyRxDataLength >= (Dlt_RxInfo.DataLengthToBeReceived))
        {
          *bufferSizePtr = 0u;
        }
        else
        {
          *bufferSizePtr = Dlt_RxInfo.DataLengthToBeReceived - Dlt_RxInfo.CopyRxDataLength;
        }

        Dlt_ReceiveBuffer.Write = bufferIndex;
        retVal = (BufReq_ReturnType)BUFREQ_OK;
      }
      /* #70 Otherwise, reject received message. */
      else
      {
        retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;
        Dlt_ReceiveBuffer.Write = Dlt_RxInfo.InitialWriteIndexOfDltMessage;
        Dlt_RxInfo.InitialWriteIndexOfDltMessage = 0u;
        Dlt_RxInfo.RxIndicationOccured = 0u;
        Dlt_RxInfo.CopyRxDataCounter = 0u;
        Dlt_RxInfo.CopyRxDataLength = 0u;
        Dlt_RxInfo.DataLengthToBeReceived = 0u;
        Dlt_RxInfo.IsRxIndicationRelevant = 0u;
      }
    }

    /* #80 Leave Dlt_DLT_EXCLUSIVE_AREA_RX. */
    DLT_END_CRITICAL_SECTION_RX();
  }
# else
  /* ----- Implementation ------------------------------------------------- */
  DLT_DUMMY_STATEMENT(id); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  DLT_DUMMY_STATEMENT(info); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
  DLT_DUMMY_STATEMENT(bufferSizePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
# endif

  return retVal;
} /* Dlt_CopyRxData */

/**********************************************************************************************************************
 * Dlt_CopyTxData
 *********************************************************************************************************************/
/*!
 *
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
FUNC(BufReq_ReturnType, DLT_CODE) Dlt_CopyTxData(
  PduIdType id,
  P2VAR(PduInfoType, AUTOMATIC, DLT_APPL_VAR) info, /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(RetryInfoType, AUTOMATIC, DLT_APPL_VAR) retry, /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
  P2VAR(PduLengthType, AUTOMATIC, DLT_APPL_VAR) availableDataPtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  BufReq_ReturnType retVal = (BufReq_ReturnType)BUFREQ_E_NOT_OK;

  /* ----- Implementation ------------------------------------------------- */
  /* retry is not supported by Vector. */
  DLT_DUMMY_STATEMENT(retry); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

  /* #10 Transmission of messages is only allowed in global state DLT_GLOBAL_STATE_ONLINE. */
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
  {
    uint32_least logChannelIndex;

    /* #20 Enter Dlt_DLT_EXCLUSIVE_AREA_TX. */
    DLT_BEGIN_CRITICAL_SECTION_TX();

    for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
    {
      if (id == Dlt_GetLogChannelTxPduSNVOfDltOfLogChannelDescriptor(logChannelIndex))
      {
        break;
      }
    }

    if (logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor())
    {
      /* #30 If the requested number of bytes to be copied is zero:
       *       Return size of send message.
       */
      if (info->SduLength == 0u)
      {
        /* Number of remaining bytes of the current tx object */
        *availableDataPtr = Dlt_SendPdu_DataLength[logChannelIndex];
        retVal = BUFREQ_OK;
      }
      /* #40 Otherwise:
       *       If size of send message is greater or equal as requested:
       *         Copy as many bytes of send message as requested.
       */
      else
      {
        if (info->SduLength <= Dlt_SendPdu_DataLength[logChannelIndex])
        {
          VStdLib_MemCpy(&info->SduDataPtr[0], &Dlt_SendPdu_DataPtr[logChannelIndex][0], info->SduLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_VALID_MEMCOPY_DSTPTR */

          Dlt_SendPdu_DataLength[logChannelIndex] = Dlt_SendPdu_DataLength[logChannelIndex] - info->SduLength;

          *availableDataPtr = Dlt_SendPdu_DataLength[logChannelIndex];

          Dlt_SendPdu_DataPtr[logChannelIndex] = &Dlt_SendPdu_DataPtr[logChannelIndex][info->SduLength];

          retVal = BUFREQ_OK;
        }
      }
    }

    /* #50 Leave Dlt_DLT_EXCLUSIVE_AREA_TX. */
    DLT_END_CRITICAL_SECTION_TX();
  }

  /*lint -e{438} */ /* PC_Dlt_438 */
  return retVal;
} /* Dlt_CopyTxData */
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 * Dlt_InitMemory
 *********************************************************************************************************************/
/*!
 *
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
FUNC(void, DLT_CODE) Dlt_InitMemory(void)
{
  /* #10 If com layer is AUTOSAR, pre-initialize global variables. */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  /* ----- Local Variables ------------------------------------------------ */
  uint32_least logChannelIndex;
# if (DLT_OS_SOFTWARE_TRACING == STD_ON)
  uint16_least coreIndex;
# endif

  /* ----- Implementation ------------------------------------------------- */
  /* #20 Initialize all global variables which have to be available before initialization of DLT module. */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    Dlt_BufferOverflowNotificationStatus[logChannelIndex].Counter = 0u;
    Dlt_BufferOverflowNotificationStatus[logChannelIndex].Flag = FALSE;
    Dlt_BufferOverflowNotificationStatus[logChannelIndex].Timer = 0u;

    Dlt_MessageCounter[logChannelIndex] = 0u;

    Dlt_SendBuffer[logChannelIndex].Write = 0u;
    Dlt_SendBuffer[logChannelIndex].Read = 0u;

    Dlt_CurrentState[logChannelIndex] = DLT_STATE_UNINIT;

    VStdLib_MemClr(&Dlt_SendBuffer[logChannelIndex].Buffer[0], DLT_MESSAGE_BUFFER_SIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
    VStdLib_MemClr(&Dlt_SendData[logChannelIndex][0], DLT_MESSAGE_BUFFER_SIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

    Dlt_DebugMode[logChannelIndex] = Dlt_GetDebugModeOfLogChannelDescriptor(logChannelIndex);
    Dlt_DebugModeRequested[logChannelIndex] = Dlt_GetDebugModeOfLogChannelDescriptor(logChannelIndex);
    if (Dlt_GetInitialDebugEventStateOfLogChannelDescriptor(logChannelIndex) == 1u) /* COV_DLT_INITIAl_DEBUG_EVENT */
    {
      Dlt_DebugEventState[logChannelIndex] = DLT_DEBUGEVENT_BUFFERING_ACTIVE;
    }
    else
    {
      Dlt_DebugEventState[logChannelIndex] = DLT_DEBUGEVENT_IDLE;
    }
  }

  /* Variables for Dlt runtime configuration */
  Dlt_FilterMessages                     = TRUE;
# if (DLT_TIMESTAMP_SUPPORT == STD_ON)
#  if (DLT_HEADER_USE_TIMESTAMP == STD_ON)
  Dlt_HeaderUseTimestamp                 = TRUE;
#  else
  Dlt_HeaderUseTimestamp                 = FALSE;
#  endif
# endif
# if (DLT_HEADER_USE_ECU_ID == STD_ON)
  Dlt_HeaderUseEcuId                     = TRUE;
# else
  Dlt_HeaderUseEcuId                     = FALSE;
# endif
# if (DLT_HEADER_USE_SESSION_ID == STD_ON)
  Dlt_HeaderUseSessionId                 = TRUE;
# else
  Dlt_HeaderUseSessionId                 = FALSE;
# endif
  Dlt_DefaultMaxLogLevel                 = DLT_DEFAULT_MAX_LOG_LEVEL;
  Dlt_DefaultTraceStatus                 = DLT_DEFAULT_TRACE_STATUS;
  Dlt_DefaultLogChannelIndex             = DLT_DEFAULT_LOG_CHANNEL_INDEX;

  /* #25 Initialize the payload buffers of the optional OS tracing messages. */
# if (DLT_OS_SOFTWARE_TRACING == STD_ON)
  for (coreIndex = 0u; coreIndex < DLT_NUMBER_OF_CORES; coreIndex++)
  {
    /* Initialize the schedule buffer (12 bytes length): */
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_COMMON_INDEX_0_v] = DLT_OS_TRACING_MSG_ID_FIRST_BYTE;
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_COMMON_INDEX_1_S] = DLT_OS_TRACING_MSG_ID_SECOND_BYTE;
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_COMMON_INDEX_2_T] = DLT_OS_TRACING_MSG_ID_THIRD_BYTE;
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_COMMON_INDEX_3_VERSION] = DLT_OS_TRACING_MSG_ID_VERSION;
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_COMMON_INDEX_4_CORE] = (uint8)coreIndex;
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_COMMON_INDEX_5_TYPE] = DLT_OS_TRACING_TYPE_SCHEDULE;

    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_SCHEDULE_INDEX_6_FTI_HIGH] = 0x00u;
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_SCHEDULE_INDEX_7_FTI_LOW] = 0x00u;
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_SCHEDULE_INDEX_8_FTI_ARTI] = 0x00u;
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_SCHEDULE_INDEX_9_TTI_HIGH] = 0x00u;
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_SCHEDULE_INDEX_10_TTI_LOW] = 0x00u;
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_SCHEDULE_INDEX_11_TTI_ARTI] = 0x00u;
    Dlt_PayloadOfVthSchedule[coreIndex][DLT_OS_TRACING_SCHEDULE_INDEX_12_EC] = 0x00u;

    /* Initialize the activation buffer (9 bytes length): */
    Dlt_PayloadOfVthActivation[coreIndex][DLT_OS_TRACING_COMMON_INDEX_0_v] = DLT_OS_TRACING_MSG_ID_FIRST_BYTE;
    Dlt_PayloadOfVthActivation[coreIndex][DLT_OS_TRACING_COMMON_INDEX_1_S] = DLT_OS_TRACING_MSG_ID_SECOND_BYTE;
    Dlt_PayloadOfVthActivation[coreIndex][DLT_OS_TRACING_COMMON_INDEX_2_T] = DLT_OS_TRACING_MSG_ID_THIRD_BYTE;
    Dlt_PayloadOfVthActivation[coreIndex][DLT_OS_TRACING_COMMON_INDEX_3_VERSION] = DLT_OS_TRACING_MSG_ID_VERSION;
    Dlt_PayloadOfVthActivation[coreIndex][DLT_OS_TRACING_COMMON_INDEX_4_CORE] = (uint8)coreIndex;
    Dlt_PayloadOfVthActivation[coreIndex][DLT_OS_TRACING_COMMON_INDEX_5_TYPE] = DLT_OS_TRACING_TYPE_ACTIVATION;

    Dlt_PayloadOfVthActivation[coreIndex][DLT_OS_TRACING_ACT_AND_SE_INDEX_6_TI_HIGH] = 0x00u;
    Dlt_PayloadOfVthActivation[coreIndex][DLT_OS_TRACING_ACT_AND_SE_INDEX_7_TI_LOW] = 0x00u;
    Dlt_PayloadOfVthActivation[coreIndex][DLT_OS_TRACING_ACT_AND_SE_INDEX_8_TI_ARTI] = DLT_ARTI_TASK_ACTIVATE;
    Dlt_PayloadOfVthActivation[coreIndex][DLT_OS_TRACING_ACT_AND_SE_INDEX_9_EC] = 0x00u;

    /* Initialize the set event buffer (9 bytes length): */
    Dlt_PayloadOfVthSetEvent[coreIndex][DLT_OS_TRACING_COMMON_INDEX_0_v] = DLT_OS_TRACING_MSG_ID_FIRST_BYTE;
    Dlt_PayloadOfVthSetEvent[coreIndex][DLT_OS_TRACING_COMMON_INDEX_1_S] = DLT_OS_TRACING_MSG_ID_SECOND_BYTE;
    Dlt_PayloadOfVthSetEvent[coreIndex][DLT_OS_TRACING_COMMON_INDEX_2_T] = DLT_OS_TRACING_MSG_ID_THIRD_BYTE;
    Dlt_PayloadOfVthSetEvent[coreIndex][DLT_OS_TRACING_COMMON_INDEX_3_VERSION] = DLT_OS_TRACING_MSG_ID_VERSION;
    Dlt_PayloadOfVthSetEvent[coreIndex][DLT_OS_TRACING_COMMON_INDEX_4_CORE] = (uint8)coreIndex;
    Dlt_PayloadOfVthSetEvent[coreIndex][DLT_OS_TRACING_COMMON_INDEX_5_TYPE] = DLT_OS_TRACING_TYPE_SETEVENT;

    Dlt_PayloadOfVthSetEvent[coreIndex][DLT_OS_TRACING_ACT_AND_SE_INDEX_6_TI_HIGH] = 0x00u;
    Dlt_PayloadOfVthSetEvent[coreIndex][DLT_OS_TRACING_ACT_AND_SE_INDEX_7_TI_LOW] = 0x00u;
    Dlt_PayloadOfVthSetEvent[coreIndex][DLT_OS_TRACING_ACT_AND_SE_INDEX_8_TI_ARTI] = DLT_ARTI_TASK_RELEASE;
    Dlt_PayloadOfVthSetEvent[coreIndex][DLT_OS_TRACING_ACT_AND_SE_INDEX_9_EC] = 0x00u;
  }
# endif

  /* #30 Init Dlt context table:
   *       Initialize all contexts with default values.
   *       Add pre-defined DEM context.
   *       Add pre-defined DET context.
   *       Add optional OS tracing context. */
  Dlt_Init_InitializeDltContext();
  Dlt_Init_RegisterDemAndDetContext();
# if (DLT_OS_SOFTWARE_TRACING == STD_ON)
  Dlt_Init_RegisterOsContexts();
# endif

  /* #40 Initialize the log channel info with configured values. */
  Dlt_Init_InitializeLogChannelInfo();

# if (DLT_NVRAM_SUPPORT == STD_ON)
  {
    uint32_least i;

#  if (DLT_OS_SOFTWARE_TRACING == STD_ON)
    uint32 numberOfLogInfoEntries = Dlt_GetNumberOfAllValidSwcContexts() + 2u + DLT_OS_TRACING_NUMBER_OF_TYPES;
#  else
    uint32 numberOfLogInfoEntries = Dlt_GetNumberOfAllValidSwcContexts() + 2u;
#  endif

    /* #50 Initialize the nv data to make sure that values are set even if NvM_ReadAll cannot restore any value. */
    Dlt_NvData.numberOfLogChannel = 0u;
    Dlt_NvData.numberOfSwcs = 0u;
    Dlt_NvData.numberOfSwcContexts = 0u;
    Dlt_NvData.defaultLogChannelIndex = 0u;
    Dlt_NvData.defaultLogLevel = 0u;
    Dlt_NvData.defaultTraceStatus = FALSE;

    for (i = 0u; i < numberOfLogInfoEntries; i++)
    {
      uint32_least j;

      Dlt_NvData.logInfo[i].appId = 0u;
      Dlt_NvData.logInfo[i].appIndexInDltContext = 0u;
      Dlt_NvData.logInfo[i].contextId = 0u;
      Dlt_NvData.logInfo[i].contextIndexInDltContext = 0u;
      Dlt_NvData.logInfo[i].logLevel = 0;
      Dlt_NvData.logInfo[i].traceStatus = 0;

      for (j = 0u; j < Dlt_GetNumberOfAllValidLogChannels(); j++)
      {
        Dlt_NvData.logInfo[i].logChannelIndexes[j] = 0u;
      }
    }

    for (i = 0u; i < Dlt_GetNumberOfAllValidLogChannels(); i++)
    {
      Dlt_NvData.logChannelInfo[i].logChannelId = 0u;
      Dlt_NvData.logChannelInfo[i].logChannelThreshold = 0u;
      Dlt_NvData.logChannelInfo[i].logChannelTraceStatus = FALSE;
    }
  }
# endif

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

  /* #60 Set the global state to DLT_GLOBAL_STATE_UNINIT, now the logging of DEM and DET messages is active. */
  Dlt_GlobalState = DLT_GLOBAL_STATE_UNINIT;
  Dlt_GlobalStateNew = DLT_GLOBAL_STATE_UNINIT;

#if (DLT_USE_COMLAYER_XCP == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  Dlt_EnterDetTransmitContext = FALSE;
#endif
} /* Dlt_InitMemory */

/**********************************************************************************************************************
 * Dlt_Init
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DLT_CODE) Dlt_Init(P2CONST(Dlt_ConfigType, AUTOMATIC, DLT_PBCFG) ConfigPtr)
{
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  /* ----- Local Variables ------------------------------------------------ */
  uint32 logChannelIndex;
#endif

  /* ----- Implementation ------------------------------------------------- */
  DLT_DUMMY_STATEMENT(ConfigPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e438 */

  /* #10 Initialize all global variables which have to be available with DLT module initialization. */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
  {
    Dlt_ControlSendBuffer[logChannelIndex].Write = 0u;
    Dlt_ControlSendBuffer[logChannelIndex].Read = 0u;

    VStdLib_MemClr(&Dlt_ControlSendBuffer[logChannelIndex].Buffer[0], DLT_CONTROL_BUFFER_SIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
    VStdLib_MemClr(&Dlt_TransmitMessage[logChannelIndex][0], DLT_MAX_MESSAGE_LENGTH); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
    VStdLib_MemClr(&Dlt_ControlSendMessage[logChannelIndex][0], DLT_MAX_CONTROL_MESSAGE_LENGTH); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

    Dlt_MessageTransmitted[logChannelIndex] = FALSE;
    Dlt_Timeout[logChannelIndex] = 0u;

    Dlt_Sm_StateTransition(logChannelIndex, kDltEventInitDlt);

    Dlt_SendPdu_DataPtr[logChannelIndex] = NULL_PTR;
    Dlt_SendPdu_DataLength[logChannelIndex] = 0u;
  }

  Dlt_ReceiveBuffer.Write = 0u;
  Dlt_ReceiveBuffer.Read = 0u;

  VStdLib_MemClr(&Dlt_ReceiveBuffer.Buffer[0], DLT_CONTROL_BUFFER_SIZE); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */
  VStdLib_MemClr(&Dlt_ReceiveMessage[0], DLT_MAX_CONTROL_MESSAGE_LENGTH); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

  Dlt_RxInfo.CopyRxDataCounter = 0u;
  Dlt_RxInfo.CopyRxDataLength = 0u;
  Dlt_RxInfo.RxIndicationOccured = 0u;
  Dlt_RxInfo.DataLengthToBeReceived = 0u;
  Dlt_RxInfo.InitialWriteIndexOfDltMessage = 0u;
  Dlt_RxInfo.IsRxIndicationRelevant = FALSE;

# if (DLT_SWCCONTEXTDESCRIPTOR == STD_ON)
  /* #20 If there are configured SwcContexts, register them as configured. */
  Dlt_Init_RegisterDefaultDltContexts();
# endif

  /* #30 If NvRam is supported, check if there is valid nv data, then overwrite the data of registered SwcContexts. */
# if (DLT_NVRAM_SUPPORT == STD_ON)
  Dlt_Init_CheckAndSetPersistedData();
# endif

# if (DLT_SYNC_TIME_SUPPORT == STD_ON)
  Dlt_SyncTimeState = DLT_SYNC_TIME_PROCESSING;
  Dlt_SyncTimeTimeoutCnt = 0u;
# endif

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

#if (DLT_USE_COMLAYER_XCP == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  Dlt_DetErrorCode = 0u;
  Dlt_DemEventStatus = 0u;
  Dlt_NonVerboseMessageId = 0u;
#endif /* (DLT_USE_COMLAYER_XCP == STD_ON) */

  /* #40 Set the global state as configured (online or offline). */
  Dlt_GlobalState = DLT_INITIAL_GLOBAL_STATE;
  Dlt_GlobalStateNew = DLT_INITIAL_GLOBAL_STATE;
  /*lint -e{438} */ /* PC_Dlt_438 */
} /* Dlt_Init */

#if (DLT_VERSION_INFO_API == STD_ON)

/**********************************************************************************************************************
 * Dlt_GetVersionInfo
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 */
FUNC(void, DLT_CODE) Dlt_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, DLT_APPL_VAR) VersionInfo)
{ /* [Dlt271] */
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (VersionInfo == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  {
    VersionInfo->vendorID         = DLT_VENDOR_ID;
    VersionInfo->moduleID         = DLT_MODULE_ID;
    VersionInfo->sw_major_version = DLT_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = DLT_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = DLT_SW_PATCH_VERSION;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0, DLT_API_ID_GETVERSIONINFO, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_GetVersionInfo */
#endif /* (DLT_VERSION_INFO_API == STD_ON) */

/**********************************************************************************************************************
 * Dlt_DetForwardErrorTrace
 *********************************************************************************************************************/
/*!
 *
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
FUNC(void, DLT_CODE) Dlt_DetForwardErrorTrace(
  uint16 ModuleId,
  uint8 InstanceId,
  uint8 ApiId,
  uint8 ErrorId)
{
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_FilterInfoType filterInfo;
  uint8  locPayload[DLT_DET_PAYLOAD_LENGTH_VERBOSE]; /* 21 == Max payload size */
  uint16 payloadLength;
  uint16 index = 0u;
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Forwarding of DET messages is only allowed before initialization and after DLT activation. */
  if ((Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE) ||
      (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT))
  {
    /* Dlt430, Dlt432 */
    /* #20 Prevent recursion in case DLT throws a DET error itself. */
    if (ModuleId != DLT_MODULE_ID)
    {
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# if (DLT_IS_EXPLICIT_CORE_USED == STD_ON)
      /* #25 Get the current core id from OS, if it is the supported core: */
      uint8 coreId = (uint8)GetCoreID();

      if (coreId == DLT_USED_CORE_ID)
      {
# endif
#endif
        /* #30 Enter Dlt_DLT_EXCLUSIVE_AREA_APIS. */
        DLT_BEGIN_CRITICAL_SECTION_APIS();

        /* #40 If use comlayer xcp enabled, build log message for DET error reporting. */
#if (DLT_USE_COMLAYER_XCP == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
        if ((uint8)ModuleId == Dlt_ModuleTable[(uint8)ModuleId])
        {
          Dlt_DetErrorCode = (uint8)InstanceId; /* ESCAN00070451: necessary for components with same module ids */
          Dlt_DetErrorCode <<= DLT_SHIFT_8BIT;
          Dlt_DetErrorCode |= (uint8)ModuleId; /* ESCAN00074824 */
          Dlt_DetErrorCode <<= DLT_SHIFT_8BIT;
          Dlt_DetErrorCode |= (uint8)ApiId;
          Dlt_DetErrorCode <<= DLT_SHIFT_8BIT;
          Dlt_DetErrorCode |= (uint8)ErrorId;
        }
        else
        {
          /* [Dlt439]: detected invalid module ID are not reported to DET to prevent a loop. Instead an invalid module ID is reported to the XCP master via Dlt_DetErrorCode  */
          Dlt_DetErrorCode = (uint32)(DLT_DET_INVALID_XCP_DET_ID | ModuleId);
        }

        /* #50 Protect the transmission context against recursion. */
        if (Dlt_EnterDetTransmitContext == FALSE) /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
        {
          Dlt_EnterDetTransmitContext = TRUE;
          /* 60 Send DLT-DET-message via XCP. */
          (void)Xcp_Event((uint16)DLT_DET_EVENT);
          Dlt_EnterDetTransmitContext = FALSE;
        }
#endif /* (DLT_USE_COMLAYER_XCP == STD_ON) */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

        /* #70 Otherwise, if implement verbose mode is enabled and active, build payload with type info for each argument.
         *     Otherwise, build payload with message ID and all arguments.
         */
# if (DLT_USE_VERBOSE_MODE == STD_ON)
        filterInfo.logInfo.options = DLT_VERBOSE_MSG;
        payloadLength = DLT_DET_PAYLOAD_LENGTH_VERBOSE;
        Dlt_Hlp_CopyLongToArray_AndMoveIndex((uint32)DLT_TYPE_INFO_UINT16, &locPayload[index], &index); /* Type Info - ModuleId - Unsigned int, 16 bit */
        Dlt_Hlp_CopyIntToArray_AndMoveIndex(ModuleId, &locPayload[index], &index);                      /* ModuleId */
        Dlt_Hlp_CopyLongToArray_AndMoveIndex((uint32)DLT_TYPE_INFO_UINT8, &locPayload[index], &index);  /* Type Info - InstanceId - Unsigned int, 8 bit */
        locPayload[index] = (uint8)(InstanceId);                                                        /* InstanceId */
        index++;
        Dlt_Hlp_CopyLongToArray_AndMoveIndex((uint32)DLT_TYPE_INFO_UINT8, &locPayload[index], &index);  /* Type Info - ApiId - Unsigned int, 8 bit */
        locPayload[index] = (uint8)(ApiId);                                                             /* ApiId */
        index++;
        Dlt_Hlp_CopyLongToArray_AndMoveIndex((uint32)DLT_TYPE_INFO_UINT8, &locPayload[index], &index);  /* Type Info - ErrorId - Unsigned int, 8 bit */
        locPayload[index] = (uint8)(ErrorId);                                                           /* ErrorId */
# else
        filterInfo.logInfo.options = DLT_NON_VERBOSE_MSG;
        payloadLength = DLT_DET_PAYLOAD_LENGTH_NON_VERBOSE;
        Dlt_Hlp_CopyLongToArray_AndMoveIndex((uint32)DLT_DET_MESSAGE_ID, &locPayload[index], &index); /* Message Id */
        Dlt_Hlp_CopyIntToArray_AndMoveIndex(ModuleId, &locPayload[index], &index);                    /* ModuleId */
        locPayload[index] = (uint8)(InstanceId);                                                      /* InstanceId */
        index++;
        locPayload[index] = (uint8)(ApiId);                                                           /* ApiId */
        index++;
        locPayload[index] = (uint8)(ErrorId);                                                         /* ErrorId */
# endif

        /* #80 Set valid filter information to avoid message rejection. */
        filterInfo.sessionId         = (Dlt_SessionIDType) DLT_DET_MODULE_ID_USED_AS_SESSION_ID; /* Session ID == DET Module ID */
        filterInfo.messageType       = (Dlt_MessageTypeType) DLT_TYPE_LOG;
        filterInfo.logInfo.appId     = (Dlt_LocalApplicationIDType) Dlt_Context.appIds[DLT_DET_APPLICATION_ID_INDEX].appId; /* DET application ID */
        filterInfo.logInfo.contextId = (Dlt_LocalContextIDType) Dlt_Context.appIds[DLT_DET_APPLICATION_ID_INDEX].contextInfoList[DLT_DET_CONTEXT_ID_INDEX].contextId; /* DET context ID */
        filterInfo.logInfo.argCount  = DLT_DET_NUMBER_OF_ARGUMENTS;
        filterInfo.logInfo.logLevel  = (uint8)DLT_LOG_ERROR;
        filterInfo.usedEndianness = DLT_USED_ENDIANNESS_PLATFORM;

        /* #90 Build message, validate it and write it to send buffer. */
        (void)Dlt_TxMsg_Transmit(&filterInfo, payloadLength, locPayload);

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

        /* #100 Leave Dlt_DLT_EXCLUSIVE_AREA_APIS. */
        DLT_END_CRITICAL_SECTION_APIS();

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# if (DLT_IS_EXPLICIT_CORE_USED == STD_ON)
      }
# endif
#endif
    }
  }
} /* Dlt_DetForwardErrorTrace */

#if (DLT_IS_DEM_PRESENT == STD_ON)
/**********************************************************************************************************************
 * Dlt_DemTriggerOnEventStatus
 *********************************************************************************************************************/
/*!
 *
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
FUNC(void, DLT_CODE) Dlt_DemTriggerOnEventStatus(
  Dem_EventIdType EventId,
  Dem_EventStatusExtendedType EventStatusOld,
  Dem_EventStatusExtendedType EventStatusNew)
{ /* [Dlt474] */
  /* ----- Local Variables ------------------------------------------------ */
# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  Dlt_FilterInfoType filterInfo;
  uint16 payloadLength;
  uint16 index = 0u;
  uint8  locPayload[DLT_DEM_PAYLOAD_LENGTH_VERBOSE]; /* 11 == Max payload size */
# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  /* #10 If required, check that event ID is in valid range. */
  /* DEM Event ID = 0 indicates no event or failure. Highest valid event ID is DLT_NUM_DEM_EVENTS. */
  if ((EventId == 0u) || (EventId > DLT_NUM_DEM_EVENTS))
  {
    errorId = DLT_E_PARAM;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 If the initial check is passed or not required: */
  {
    /* #30 DEM events are only allowed to be send before initialization and after DLT activation. */
    if ((Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE) ||
        (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT))
    {
# if (DLT_DEM_EVENT_FILTERING_ENABLED == STD_ON)
      boolean sendDemMsg;
# endif

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
#  if (DLT_IS_EXPLICIT_CORE_USED == STD_ON)
      /* #35 Get the current core id from OS, if it is the supported core: */
      uint8 coreId = (uint8)GetCoreID();

      if (coreId == DLT_USED_CORE_ID)
      {
#  endif
# endif

        /* [Dlt475]: the DEM already ensures that EventStatusOld and EventStatusNew are not equal, because Dlt_DemTriggerOnEventStatus is only called if they differ. Hence the log message can always be generated. */
        /* #40 Enter Dlt_DLT_EXCLUSIVE_AREA_APIS. */
        DLT_BEGIN_CRITICAL_SECTION_APIS();

        /* #50 If DEM event filtering is enabled, call callback function to add individual message filter. */
# if (DLT_DEM_EVENT_FILTERING_ENABLED == STD_ON)
        /* UREQ00041311: The implementation shall be able to filter for the following properties: DEM Event ID and DEM Event Status Bits */
        sendDemMsg = Appl_DltDemEventFilterCbk(EventId, EventStatusOld, EventStatusNew);

        if (sendDemMsg == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
# else
        DLT_DUMMY_STATEMENT(EventStatusOld); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif
        {
          /* #60 If comlayer XCP is active, send DEM event via XCP. */
# if (DLT_USE_COMLAYER_XCP == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
          Dlt_DemEventStatus = (uint16)EventId;
          Dlt_DemEventStatus <<= DLT_SHIFT_16BIT;
          Dlt_DemEventStatus |= (uint8)EventStatusNew;

          /* UREQ00040542: Report DEM Event change */
          (void)Xcp_Event((uint16)DLT_DEM_EVENT);
# endif /* (DLT_USE_COMLAYER_XCP == STD_ON) */
# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
          /* #70 Otherwise:
           *       If implement verbose mode is enabled and active, build payload with according type info for each argument.
           *       Otherwise, build payload with message ID and all arguments.
           */
#  if (DLT_USE_VERBOSE_MODE == STD_ON)
          /* Payload has to be modified, add Type Info for each argument */
          filterInfo.logInfo.options = DLT_VERBOSE_MSG;
          payloadLength = DLT_DEM_PAYLOAD_LENGTH_VERBOSE;
          Dlt_Hlp_CopyLongToArray_AndMoveIndex((uint32)0x00000042u, &locPayload[index], &index); /* Type Info - EventId - Unsigned int, 16 bit */
          Dlt_Hlp_CopyIntToArray_AndMoveIndex((uint16)EventId, &locPayload[index], &index); /* EventId */
          Dlt_Hlp_CopyLongToArray_AndMoveIndex((uint32)0x00000041u, &locPayload[index], &index); /* Type Info - EventStatusNew - Unsigned int, 8 bit */
          locPayload[10] = (uint8)(EventStatusNew); /* EventStatusNew */
#  else
          filterInfo.logInfo.options = DLT_NON_VERBOSE_MSG;
          payloadLength = DLT_DEM_PAYLOAD_LENGTH_NON_VERBOSE;
          Dlt_Hlp_CopyLongToArray_AndMoveIndex((uint32)DLT_DEM_MESSAGE_ID, &locPayload[index], &index); /* Message ID */
          Dlt_Hlp_CopyIntToArray_AndMoveIndex((uint16)EventId, &locPayload[index], &index); /* EventId */
          locPayload[index] = (uint8)(EventStatusNew); /* EventStatusNew */
#  endif

          /* #80 Set filter options according to DEM events. */
          filterInfo.sessionId = (Dlt_SessionIDType)DLT_DEM_MODULE_ID_USED_AS_SESSION_ID; /* Session ID == DEM Module ID */
          filterInfo.messageType = (Dlt_MessageTypeType)DLT_TYPE_LOG;

          filterInfo.logInfo.argCount = DLT_DEM_NUMBER_OF_ARGUMENTS;
          filterInfo.logInfo.appId = Dlt_Context.appIds[DLT_DEM_APPLICATION_ID_INDEX].appId; /* Application ID == "DEM" */
          filterInfo.logInfo.contextId = Dlt_Context.appIds[DLT_DEM_APPLICATION_ID_INDEX].contextInfoList[DLT_DEM_CONTEXT_ID_INDEX].contextId; /* Context ID == "STD0" */
          filterInfo.logInfo.logLevel = DLT_DEM_EVENT_LOG_LEVEL; /* Log level of DEM messages */
          filterInfo.usedEndianness = DLT_USED_ENDIANNESS_PLATFORM;

          /* #90 Build the DLT-DEM-Message and write it to send buffer. */
          (void)Dlt_TxMsg_Transmit(&filterInfo, payloadLength, locPayload);

# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */
        }

        /* #100 Leave Dlt_DLT_EXCLUSIVE_AREA_APIS. */
        DLT_END_CRITICAL_SECTION_APIS();

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
#  if (DLT_IS_EXPLICIT_CORE_USED == STD_ON)
      }
#  endif
# endif
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_DEMTRIGGERONEVENTSTATUS, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  /*lint -e{438} */ /* PC_Dlt_438 */
  /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_DemTriggerOnEventStatus */
#endif /* (DLT_IS_DEM_PRESENT == STD_ON) */

/**********************************************************************************************************************
 * Dlt_SendLogMessage
 *********************************************************************************************************************/
/*!
 *
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
 * \spec        requires LogData != NULL_PTR;
 *              requires LogInfo != NULL_PTR;
 *              requires $lengthOf(LogData) >= LogDataLength;
 *              requires LogDataLength <= DLT_MAX_MESSAGE_LENGTH;
 * \endspec
 */
/* PRQA S 6030 2 */ /* MD_MSR_STCYC */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DLT_CODE) Dlt_SendLogMessage(
  Dlt_SessionIDType parg0,
  P2CONST(Dlt_MessageLogInfoType, AUTOMATIC, DLT_APPL_VAR) LogInfo,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogData,
  uint16 LogDataLength)
{ /* Dlt241 */
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retVal = DLT_E_ERROR;
  uint8 errorId = DLT_E_NO_ERROR;
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  Dlt_FilterInfoType filterInfo;
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If Dlt is uninitialized or parameter is invalid, report a DET. */
#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if (LogData == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else if (LogDataLength >= DLT_MAX_MESSAGE_LENGTH)
  {
    errorId = DLT_E_PARAM;
  }
#endif

  /* #15 If the comlayer ASR is active: */
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  else if (LogInfo == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  {
    /* #20 If global state is DLT_GLOBAL_STATE_ONLINE, set filter options, check the message's validity and write it
     *     to the send buffer.
     */
    if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
    {
# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
#  if (DLT_IS_EXPLICIT_CORE_USED == STD_ON)
      /* #25 Get the current core id from OS, if it is the supported core: */
      uint8 coreId = (uint8)GetCoreID();

      if (coreId == DLT_USED_CORE_ID)
      {
#  endif
# endif
        Dlt_LocalApplicationIDType appId;
        Dlt_LocalContextIDType contextId;

        Dlt_Hlp_CopyStringToLongBigEndian(LogInfo->appId, &appId);
        Dlt_Hlp_CopyStringToLongBigEndian(LogInfo->contextId, &contextId);

        /* #30 Enter Dlt_DLT_EXCLUSIVE_AREA_APIS. */
        DLT_BEGIN_CRITICAL_SECTION_APIS();

        filterInfo.messageType = (Dlt_MessageTypeType)DLT_TYPE_LOG;
        filterInfo.sessionId = parg0;
        filterInfo.logInfo.appId = appId;
        filterInfo.logInfo.contextId = contextId;
        filterInfo.logInfo.argCount = LogInfo->argCount;
        filterInfo.logInfo.options = LogInfo->options;
        filterInfo.logInfo.logLevel = LogInfo->logLevel;
        filterInfo.usedEndianness = DLT_USED_ENDIANNESS_PLATFORM;

        /* #35 Build the log message and write it to send buffer. */
        retVal = Dlt_TxMsg_Transmit(&filterInfo, LogDataLength, LogData);

        /* #40 Leave Dlt_DLT_EXCLUSIVE_AREA_APIS. */
        DLT_END_CRITICAL_SECTION_APIS();

# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
#  if (DLT_IS_EXPLICIT_CORE_USED == STD_ON)
      }
#  endif
# endif
    }
  }

#else /* (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */
  /* #50 Otherwise: */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  {
    DLT_DUMMY_STATEMENT(parg0); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */

    /* #60 If global state is DLT_GLOBAL_STATE_ONLINE: */
    if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
    {
      /* SWS_Dlt_00014 */
      /* #70 If message is in verbose mode AND
       *       If implement verbose mode is enabled, build and send the log message with string payload.
       *       Otherwise, invalid request. */
      if ((LogInfo->options & DLT_VERBOSE_MSG) == DLT_VERBOSE_MSG)
      {
# if (DLT_USE_VERBOSE_MODE == STD_ON)
        /* verbose message handling */
        uint8 retVal_Xcp;
        /* #80 Enter Dlt_DLT_EXCLUSIVE_AREA_APIS. */
        DLT_BEGIN_CRITICAL_SECTION_APIS();

        VStdLib_MemCpy(&Dlt_VerboseMessageData[0], &LogData[sizeof(Dlt_StandardHeaderType)+sizeof(Dlt_MessageIDType)], LogDataLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */

        /* SREQ00024774: If the string written to the data buffer is shorter than the defined data field it shall be terminated with 0. */
        Dlt_VerboseMessageData[LogDataLength] = (uint8)'\0';

        /* UREQ00040541: generate verbose log message */
        retVal_Xcp = Xcp_Event((uint16)DLT_VERBOSE_MSG_EVENT);

        if (retVal_Xcp != (XCP_EVENT_NOP))
        {
          retVal = (Dlt_ReturnType)(DLT_E_OK);
        }
        else
        {
          retVal = (Dlt_ReturnType)(DLT_E_ERROR);
        }

        /* #90 Leave Dlt_DLT_EXCLUSIVE_AREA_APIS. */
        DLT_END_CRITICAL_SECTION_APIS();
# else /* (DLT_USE_VERBOSE_MODE == STD_ON) */
        DLT_DUMMY_STATEMENT(LogDataLength); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
        /* SWS_Dlt_00090 */
        retVal = (Dlt_ReturnType)DLT_E_ERROR;
# endif /* (DLT_USE_VERBOSE_MODE == STD_ON) */
      }
      /* #100 Otherwise, build the log message with message ID and send it via XCP. */
      else
      {
        /* non-verbose message handling */
        uint32 msgId = 0u;
        uint16 index = 4u;
        Dlt_Hlp_CopyArrayToLongLittleEndian_AndMoveIndex(&LogData[0], &msgId, &index); /* ESCAN00081868 */
        /* Dlt031: Message IDs used for Dem (0x00000001) and Det (0x00000002) are reserved and not usable for SW-Cs. */
        /* #110 If message ID in invalid range, reject send request.
         *     Otherwise, send log message via XCP. */
        if ((msgId <= DLT_DET_MESSAGE_ID) || (msgId >= DLT_NUM_NON_VERBOSE_MESSAGES))
        {
          errorId = DLT_E_PARAM;
          retVal = (Dlt_ReturnType)DLT_E_ERROR;
        }
        else
        {
          uint8 retVal_Xcp;

          /* #120 Enter Dlt_DLT_EXCLUSIVE_AREA_APIS. */
          DLT_BEGIN_CRITICAL_SECTION_APIS();

          Dlt_NonVerboseMessageId = (Dlt_MessageIDType)(msgId);

          retVal_Xcp = Xcp_Event((uint16)DLT_NON_VERBOSE_MSG_EVENT);

          if (retVal_Xcp != (XCP_EVENT_NOP))
          {
            retVal = (Dlt_ReturnType)(DLT_E_OK);
          }
          else
          {
            retVal = (Dlt_ReturnType)(DLT_E_ERROR);
          }

          /* #130 Leave Dlt_DLT_EXCLUSIVE_AREA_APIS. */
          DLT_END_CRITICAL_SECTION_APIS();
        }
      }
    }
  }
#endif /* (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

#if (DLT_DEV_ERROR_REPORT == STD_ON)
  /* ----- Development Error Report --------------------------------------- */
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_SENDLOGMESSAGE, errorId);
  }
#else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
#endif

  /*lint -e{438} */ /* PC_Dlt_438 */
  return (Std_ReturnType)retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_SendLogMessage */

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

/**********************************************************************************************************************
 * Dlt_SendTraceMessage
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 * \spec        requires TraceData != NULL_PTR;
 *              requires TraceInfo != NULL_PTR;
 *              requires $lengthOf(TraceData) >= TraceDataLength;
 *              requires TraceDataLength <= DLT_MAX_MESSAGE_LENGTH;
 * \endspec
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DLT_CODE) Dlt_SendTraceMessage(
  Dlt_SessionIDType parg0,
  P2CONST(Dlt_MessageTraceInfoType, AUTOMATIC, DLT_APPL_VAR) TraceInfo,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) TraceData,
  uint16 TraceDataLength)
{ /* Dlt241 */
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_ReturnType retVal = (Dlt_ReturnType)DLT_E_ERROR;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If Dlt is uninitialized or parameter is invalid, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if (TraceData == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else if (TraceInfo == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else if (TraceDataLength >= DLT_MAX_MESSAGE_LENGTH)
  {
    errorId = DLT_E_PARAM;
  }
  else
# endif
  /* #20 Otherwise: */
  {
# if (DLT_IS_EXPLICIT_CORE_USED == STD_ON)
    /* #25 Get the current core id from OS, if it is the supported core: */
    uint8 coreId = (uint8)GetCoreID();

    if (coreId == DLT_USED_CORE_ID)
    {
# endif
      /* ----- Implementation ------------------------------------------------- */
      /* #30 Enter Dlt_DLT_EXCLUSIVE_AREA_APIS. */
      DLT_BEGIN_CRITICAL_SECTION_APIS();

      /* #40 Check trace message and write it in the send buffer. */
      retVal = Dlt_Trace_SendTraceMessage(parg0, TraceInfo, TraceData, TraceDataLength, DLT_USED_ENDIANNESS_PLATFORM);

      /* #50 Leave Dlt_DLT_EXCLUSIVE_AREA_APIS. */
      DLT_END_CRITICAL_SECTION_APIS();
# if (DLT_IS_EXPLICIT_CORE_USED == STD_ON)
    }
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_SENDTRACEMESSAGE, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return (Std_ReturnType)retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_SendTraceMessage */

/**********************************************************************************************************************
 * Dlt_RegisterContext
 *********************************************************************************************************************/
/*!
 *
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
 * \spec        requires parg0 > 0x1000u;
 *              requires AppId != NULL_PTR;
 *              requires ContextId != NULL_PTR;
 *              requires AppDescription != NULL_PTR;
 *              requires ContextDescription != NULL_PTR;
 * \endspec
 */
/* PRQA S 6010 4 */ /* MD_MSR_STPTH */
/* PRQA S 6030 3 */ /* MD_MSR_STCYC */
/* PRQA S 6060 2 */ /* MD_MSR_STPAR */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DLT_CODE) Dlt_RegisterContext(
  Dlt_SessionIDType parg0,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppDescription,
  uint8 LenAppDescription,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextDescription,
  uint8 LenContextDescription)
{
  /* ----- Local Variables ------------------------------------------------ */
  Dlt_LocalApplicationIDType appId;
  Dlt_LocalContextIDType contextId;
  Dlt_ReturnType retVal;
  boolean applicationAlreadyRegistered = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
  boolean contextAlreadyRegistered;
  uint16 AppIdIndex;
  uint16 ConIdIndex;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Implementation ------------------------------------------------- */
  /* #10 Enter Dlt_DLT_EXCLUSIVE_AREA_APIS. */
  DLT_BEGIN_CRITICAL_SECTION_APIS();

  Dlt_Hlp_CopyStringToLongBigEndian(AppId, &appId);
  Dlt_Hlp_CopyStringToLongBigEndian(ContextId, &contextId);

  /* #20 Check if the new Context already exists. */
  contextAlreadyRegistered = Dlt_Hlp_IsContextRegistered(appId, contextId, &AppIdIndex, &ConIdIndex, &applicationAlreadyRegistered);

  /* #30 If application ID is not registered, set application ID index to next free slot in context table. */
  if (applicationAlreadyRegistered == FALSE) /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
  {
    AppIdIndex = Dlt_Context.appIdCount;
  }
  /* #40 If context is already registered, reject new registration. */
  if (contextAlreadyRegistered == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
  {
    retVal = (Dlt_ReturnType)DLT_E_CONTEXT_ALREADY_REG;
  }
  /* #50 Otherwise, if session ID is invalid, reject registration. */
  else if (parg0 < 0x1000u)
  {
    retVal = (Dlt_ReturnType)DLT_E_UNKNOWN_SESSION_ID;
  }
  /* #60 Otherwise, if there is a free slot in context table, register the new context. */
  else if ((Dlt_Context.appIdCount < Dlt_GetMaxNumberOfApplications()) &&
           (Dlt_Context.appIds[AppIdIndex].contextIdCount < (uint32)Dlt_GetMaxNumberOfContextsPerApplication()) &&
           (Dlt_NumberOfRegisteredContextIds < Dlt_GetMaxNumberOfContexts())
          )
  {
    Dlt_NumberOfRegisteredContextIds++;

    /* If Application ID is not registered yet, use new Context entry */
    ConIdIndex = Dlt_Context.appIds[AppIdIndex].contextIdCount;

    /*@ assert AppIdIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
    /*@ assert ConIdIndex < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */
    /*@ assert Dlt_DefaultLogChannelIndex < Dlt_GetNumberOfAllValidLogChannels(); */ /* VCA_DLT_VALID_CONFIGURED_LOGCHANNEL_ID */

    Dlt_Context.appIds[AppIdIndex].appId = appId;
    Dlt_Context.appIds[AppIdIndex].appDescription = AppDescription;
    Dlt_Context.appIds[AppIdIndex].appDescriptionLen = LenAppDescription;
    Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].logChannelIndexes[Dlt_DefaultLogChannelIndex] = Dlt_DefaultLogChannelIndex;
    Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].contextId = contextId;
    Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].contextDescription = ContextDescription;
    Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].lenContextDescription = LenContextDescription;
    Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].logLevel = (sint8)Dlt_DefaultMaxLogLevel;
    Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].traceStatus = (sint8)Dlt_DefaultTraceStatus; /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
    Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].cbkInfo.sessionId = parg0;

    Dlt_RegCont_RegisterNewContext(
      parg0,
      AppId,
      ContextId,
      AppIdIndex,
      ConIdIndex,
      applicationAlreadyRegistered);

    retVal = (Dlt_ReturnType)DLT_E_OK;
  }
  /* #70 Otherwise, the context could not be registered because there is no space left. */
  else
  {
    errorId = DLT_E_REGISTRATION;
    retVal = (Dlt_ReturnType)DLT_E_ERROR;
  }

  /* #80 Leave Dlt_DLT_EXCLUSIVE_AREA_APIS. */
  DLT_END_CRITICAL_SECTION_APIS();

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_REGISTERCONTEXT, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return (Std_ReturnType)retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_RegisterContext */

# if (DLT_OS_SOFTWARE_TRACING == STD_ON)
/**********************************************************************************************************************
 * Dlt_OsVthSchedule
 *********************************************************************************************************************/
/*!
 *
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
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
FUNC(void, DLT_CODE) Dlt_OsVthSchedule(
  uint32 FromThreadId,
  uint32 FromThreadReason,
  uint32 ToThreadId,
  uint32 ToThreadReason,
  uint16 CallerCoreId)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 If the given core is in valid range. */
  if (CallerCoreId == DLT_USED_CORE_ID)
  {
    /*@ assert DLT_USED_CORE_ID < DLT_NUMBER_OF_CORES; */ /* VCA_DLT_VALID_CORE_CONFIGURATION */

    /* Note that exclusive areas must not be entered in OS context. Exclusive areas are already entered by OS itself. */
    /* #20 Set the payload of schedule message:
     *       Terminated/interrupted thread id (task or ISR2).
     *       Reason for termination/interruption (mapped to ARTI).
     *       Started/resumed thread id  (task or ISR2).
     *       Reason for start/resume (mapped to ARTI).
     *       Event counter. */
    Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_6_FTI_HIGH] = (uint8)((uint16)FromThreadId >> 8u);
    Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_7_FTI_LOW] = (uint8)FromThreadId;

    switch (FromThreadReason)
    {
     case (OS_VTHP_TASK_TERMINATION):
       Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_8_FTI_ARTI] = DLT_ARTI_TASK_TERMINATE;
       break;

      case (OS_VTHP_ISR_END):
        Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_8_FTI_ARTI] = DLT_ARTI_CAT2ISR_STOP;
        break;

      case (OS_VTHP_TASK_WAITEVENT):
        Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_8_FTI_ARTI] = DLT_ARTI_TASK_WAIT;
        break;

      case (OS_VTHP_TASK_WAITSEMA):
        Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_8_FTI_ARTI] = DLT_ARTI_TASK_WAIT;
        break;

      case (OS_VTHP_THREAD_PREEMPT):
        Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_8_FTI_ARTI] = DLT_ARTI_TASK_PREEMPT;
        break;

      default:
        Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_8_FTI_ARTI] = DLT_ARTI_INVALID;
        break;
    }

    Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_9_TTI_HIGH] = (uint8)((uint16)ToThreadId >> 8u);
    Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_10_TTI_LOW] = (uint8)ToThreadId;

    switch(ToThreadReason)
    {
      case (OS_VTHP_TASK_ACTIVATION):
        Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_11_TTI_ARTI] = DLT_ARTI_TASK_START;
        break;

      case (OS_VTHP_ISR_START):
        Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_11_TTI_ARTI] = DLT_ARTI_CAT2ISR_START;
        break;

      case (OS_VTHP_TASK_SETEVENT):
        Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_11_TTI_ARTI] = DLT_ARTI_TASK_CONTINUE;
        break;

      case (OS_VTHP_TASK_GOTSEMA):
        Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_11_TTI_ARTI] = DLT_ARTI_TASK_CONTINUE;
        break;

      case (OS_VTHP_THREAD_RESUME):
        Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_11_TTI_ARTI] = DLT_ARTI_TASK_RESUME;
        break;

      default:
        Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_11_TTI_ARTI] = DLT_ARTI_INVALID;
        break;
    }

    Dlt_PayloadOfVthSchedule[CallerCoreId][DLT_OS_TRACING_SCHEDULE_INDEX_12_EC] = (uint8)Dlt_EventCounter;

    /* #30 Increment the event counter. */
    Dlt_EventCounter++;

    /* #40 Buffer the schedule trace message for each assigned log channel. */
    (void)Dlt_Trace_SendTraceMessage(
      DLT_OS_TRACING_SESSION_ID /* Session id = OS module id = 0x01 */,
      &Dlt_MessageTraceInfoOfVthSchedule[CallerCoreId] /* Message trace info of activation */,
      Dlt_PayloadOfVthSchedule[CallerCoreId],
      DLT_PAYLOAD_LENGTH_SCHEDULE,
      DLT_USED_ENDIANNESS_BIG_ENDIAN);
  }
} /* Dlt_OsVthSchedule */

/**********************************************************************************************************************
 * Dlt_OsVthActivation
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DLT_CODE) Dlt_OsVthActivation(
  uint32 TaskId,
  uint16 DestCoreId,
  uint16 CallerCoreId)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 If the given core is in valid range. */
  if (CallerCoreId == DLT_USED_CORE_ID)
  {
    /*@ assert DLT_USED_CORE_ID < DLT_NUMBER_OF_CORES; */ /* VCA_DLT_VALID_CORE_CONFIGURATION */

    /* Note that exclusive areas must not be entered in OS context. Exclusive areas are already entered by OS itself. */
    /* #20 Set the payload of schedule message:
     *       Started task id.
     *       Event counter. */
    Dlt_PayloadOfVthActivation[CallerCoreId][DLT_OS_TRACING_ACT_AND_SE_INDEX_6_TI_HIGH] = (uint8)((uint16)TaskId >> 8u);
    Dlt_PayloadOfVthActivation[CallerCoreId][DLT_OS_TRACING_ACT_AND_SE_INDEX_7_TI_LOW] = (uint8)TaskId;

    Dlt_PayloadOfVthActivation[CallerCoreId][DLT_OS_TRACING_ACT_AND_SE_INDEX_9_EC] = (uint8)Dlt_EventCounter;

    /* #30 Increment the event counter. */
    Dlt_EventCounter++;

    /* #40 Buffer the activation trace message for each assigned log channel. */
    (void)Dlt_Trace_SendTraceMessage(
      DLT_OS_TRACING_SESSION_ID /* Session id = OS module id = 0x01 */,
      &Dlt_MessageTraceInfoOfVthActivation[CallerCoreId] /* Message trace info of activation */,
      Dlt_PayloadOfVthActivation[CallerCoreId],
      DLT_PAYLOAD_LENGTH_ACTIVATE,
      DLT_USED_ENDIANNESS_BIG_ENDIAN);
  }

  DLT_DUMMY_STATEMENT(DestCoreId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
} /* Dlt_OsVthActivation */

/**********************************************************************************************************************
 * Dlt_OsVthSetEvent
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DLT_CODE) Dlt_OsVthSetEvent(
  uint32 TaskId,
  boolean StateChanged,
  uint16 DestCoreId,
  uint16 CallerCoreId)
{
  /* ----- Implementation ------------------------------------------------- */
  /* #10 If the given core is in valid range and the event state changed. */
  if ((CallerCoreId == DLT_USED_CORE_ID)
   && (StateChanged == TRUE))
  {
    /*@ assert DLT_USED_CORE_ID < DLT_NUMBER_OF_CORES; */ /* VCA_DLT_VALID_CORE_CONFIGURATION */

    /* Note that exclusive areas must not be entered in OS context. Exclusive areas are already entered by OS itself. */
    /* #20 Set the payload of schedule message:
     *       Started task id.
     *       Event counter. */
    Dlt_PayloadOfVthSetEvent[CallerCoreId][DLT_OS_TRACING_ACT_AND_SE_INDEX_6_TI_HIGH] = (uint8)((uint16)TaskId >> 8u);
    Dlt_PayloadOfVthSetEvent[CallerCoreId][DLT_OS_TRACING_ACT_AND_SE_INDEX_7_TI_LOW] = (uint8)TaskId;

    Dlt_PayloadOfVthSetEvent[CallerCoreId][DLT_OS_TRACING_ACT_AND_SE_INDEX_9_EC] = (uint8)Dlt_EventCounter;

    /* #30 Increment the event counter. */
    Dlt_EventCounter++;

    /* #40 Buffer the set event trace message for each assigned log channel. */
    (void)Dlt_Trace_SendTraceMessage(
      DLT_OS_TRACING_SESSION_ID /* Session id = OS module id = 0x01 */,
      &Dlt_MessageTraceInfoOfVthSetEvent[CallerCoreId] /* Message trace info of activation */,
      Dlt_PayloadOfVthSetEvent[CallerCoreId],
      DLT_PAYLOAD_LENGTH_SETEVENT,
      DLT_USED_ENDIANNESS_BIG_ENDIAN);
  }

  DLT_DUMMY_STATEMENT(DestCoreId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
} /* Dlt_OsVthSetEvent */
# endif

/**********************************************************************************************************************
 * Dlt_SetLogLevel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 * \spec        requires AppId != NULL_PTR;
 *              requires ContextId != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetLogLevel(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextId,
  Dlt_MessageLogLevelType NewLogLevel)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise:
   *       If the new log level is valid, set it to the specified DLT user(s). */
  {
    if (Dlt_Hlp_ParamInValidRange(DLT_OPT_LOGLEVEL, (sint8)NewLogLevel) == DLT_OK)
    {
      Dlt_LocalApplicationIDType appId;
      Dlt_LocalContextIDType contextId;

      Dlt_Hlp_CopyStringToLongBigEndian(AppId, &appId);
      Dlt_Hlp_CopyStringToLongBigEndian(ContextId, &contextId);

      retVal = (Std_ReturnType)Dlt_Hlp_SetOption(appId, contextId, DLT_OPT_LOGLEVEL, (uint8)NewLogLevel);
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_SETLOGLEVEL, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_SetLogLevel */

/**********************************************************************************************************************
 * Dlt_SetTraceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 * \spec        requires AppId != NULL_PTR;
 *              requires ContextId != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetTraceStatus(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextId,
  boolean NewTraceStatus)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise:
   *       If the new trace status is valid, set it to the specified DLT user(s). */
  {
    /* PRQA S 2991, 2995 1 */ /* MD_Dlt_AlwaysTrue */
    if (Dlt_Hlp_ParamInValidRange(DLT_OPT_TRACESTATUS, (sint8)NewTraceStatus) == DLT_OK) /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
    {
      Dlt_LocalApplicationIDType appId;
      Dlt_LocalContextIDType contextId;

      Dlt_Hlp_CopyStringToLongBigEndian(AppId, &appId);
      Dlt_Hlp_CopyStringToLongBigEndian(ContextId, &contextId);

      retVal = (Std_ReturnType)Dlt_Hlp_SetOption(appId, contextId, DLT_OPT_TRACESTATUS, (uint8)NewTraceStatus); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_SETTRACESTATUS, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_SetTraceStatus */

/**********************************************************************************************************************
 * Dlt_GetLogInfo
 *********************************************************************************************************************/
/*!
 *
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
 * \spec        requires AppId != NULL_PTR;
 *              requires ContextId != NULL_PTR;
 *              requires LogInfo != NULL_PTR;
 * \endspec
 */
/* PRQA S 6030 2 */ /* MD_MSR_STCYC */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetLogInfo(
  uint8 Options,
  P2CONST(uint8, AUTOMATIC, RTE_DLT_APPL_DATA) AppId,
  P2CONST(uint8, AUTOMATIC, RTE_DLT_APPL_DATA) ContextId,
  uint8 Status,
  P2VAR(Dlt_LogInfoType, AUTOMATIC, RTE_DLT_APPL_VAR) LogInfo)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
#  if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if((AppId == NULL_PTR) || (ContextId == NULL_PTR) || (LogInfo == NULL_PTR))
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
#  endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise: */
  {
    /* #30 If the requested option is valid: */
    if ((Options == DLT_LOG_OPTIONS_ALL_INFO_WITHOUT_DESCRIPTIONS)
     || (Options == DLT_LOG_OPTIONS_ALL_INFO_WITH_DESCRIPTIONS))
    {
      uint32 applicationId;
      uint32 contextId;
      uint16 appIdIndex;
      uint16 conIdIndex;
      boolean applicationAlreadyRegistered = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
      boolean contextAlreadyRegistered;

      retVal = E_OK;

      Dlt_Hlp_CopyStringToLongBigEndian(AppId, &applicationId);
      Dlt_Hlp_CopyStringToLongBigEndian(ContextId, &contextId);

      contextAlreadyRegistered = Dlt_Hlp_IsContextRegistered(applicationId, contextId, &appIdIndex, &conIdIndex, &applicationAlreadyRegistered);

      /*@ assert appIdIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
      /*@ assert conIdIndex < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

      /* #40 If the requested application id is registered. */
      if (applicationAlreadyRegistered == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
      {
        /* #50 If the requested context id is registered. */
        if (contextAlreadyRegistered == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
        {
          LogInfo->appIdCount = 1u;
          LogInfo->appIdInfo[0].appDesc[0] = 0u;
          LogInfo->appIdInfo[0].appDescLen = 0u;
          Dlt_Hlp_CopyLongToArrayBigEndian(applicationId, &LogInfo->appIdInfo[0].appId[0]);
          LogInfo->appIdInfo[0].contextIdCount = 1u;
          LogInfo->appIdInfo[0].contextInfoList[0].contextDesc[0] = 0u;
          LogInfo->appIdInfo[0].contextInfoList[0].lenContextDescription = 0u;
          Dlt_Hlp_CopyLongToArrayBigEndian(contextId, &LogInfo->appIdInfo[0].contextInfoList[0].contextId[0]);
          LogInfo->appIdInfo[0].contextInfoList[0].logLevel = (Dlt_MessageLogLevelType)Dlt_Context.appIds[appIdIndex].contextInfoList[conIdIndex].logLevel;
          LogInfo->appIdInfo[0].contextInfoList[0].traceStatus = (uint8)Dlt_Context.appIds[appIdIndex].contextInfoList[conIdIndex].traceStatus;
        }
        /* #60 Otherwise if the context id is a wildcard. */
        else if(contextId == 0x00u)
        {
          uint16_least contextIndex;
          uint16 numberOfContexts = Dlt_Context.appIds[appIdIndex].contextIdCount;

          LogInfo->appIdCount = 1u;
          LogInfo->appIdInfo[0].appDesc[0] = 0u;
          LogInfo->appIdInfo[0].appDescLen = 0u;
          Dlt_Hlp_CopyLongToArrayBigEndian(applicationId, &LogInfo->appIdInfo[0].appId[0]);
          LogInfo->appIdInfo[0].contextIdCount = numberOfContexts;

          for (contextIndex = 0u; contextIndex < numberOfContexts; contextIndex++)
          {
            uint32 contextIdIter;
            contextIdIter = Dlt_Context.appIds[appIdIndex].contextInfoList[contextIndex].contextId;

            LogInfo->appIdInfo[0].contextInfoList[contextIndex].contextDesc[0] = 0u;
            LogInfo->appIdInfo[0].contextInfoList[contextIndex].lenContextDescription = 0u;
            Dlt_Hlp_CopyLongToArrayBigEndian(contextIdIter, &LogInfo->appIdInfo[0].contextInfoList[contextIndex].contextId[0]);
            LogInfo->appIdInfo[0].contextInfoList[contextIndex].logLevel = (Dlt_MessageLogLevelType)Dlt_Context.appIds[appIdIndex].contextInfoList[contextIndex].logLevel;
            LogInfo->appIdInfo[0].contextInfoList[contextIndex].traceStatus = (uint8)Dlt_Context.appIds[appIdIndex].contextInfoList[contextIndex].traceStatus;
          }
        }
        /* #70 Otherwise, the requested context id is invalid, therefore indicate that an error occurred. */
        else
        {
          retVal = E_NOT_OK;
        }
      }
      /* #80 Otherwise if the application id is a wildcard. */
      else if(applicationId == 0x00u)
      {
        uint16_least applIndex;
        uint16 numberOfApplications = Dlt_Context.appIdCount;

        LogInfo->appIdCount = numberOfApplications;

        /* #90 Iterate over all registered applications. */
        for (applIndex = 0u; applIndex < numberOfApplications; applIndex++)
        {
          uint16 contextIndex;
          uint16 numberOfContexts = Dlt_Context.appIds[applIndex].contextIdCount;

          uint32 applicationIdIter = Dlt_Context.appIds[applIndex].appId;

          LogInfo->appIdInfo[applIndex].appDesc[0] = 0u;
          LogInfo->appIdInfo[applIndex].appDescLen = 0u;
          Dlt_Hlp_CopyLongToArrayBigEndian(applicationIdIter, &LogInfo->appIdInfo[applIndex].appId[0]);
          LogInfo->appIdInfo[applIndex].contextIdCount = numberOfContexts;

          for (contextIndex = 0u; contextIndex < numberOfContexts; contextIndex++)
          {
            uint32 contextIdIter;
            contextIdIter = Dlt_Context.appIds[applIndex].contextInfoList[contextIndex].contextId;

            LogInfo->appIdInfo[applIndex].contextInfoList[contextIndex].contextDesc[0] = 0u;
            LogInfo->appIdInfo[applIndex].contextInfoList[contextIndex].lenContextDescription = 0u;
            Dlt_Hlp_CopyLongToArrayBigEndian(contextIdIter, &LogInfo->appIdInfo[applIndex].contextInfoList[contextIndex].contextId[0]);
            LogInfo->appIdInfo[applIndex].contextInfoList[contextIndex].logLevel = (Dlt_MessageLogLevelType)Dlt_Context.appIds[applIndex].contextInfoList[contextIndex].logLevel;
            LogInfo->appIdInfo[applIndex].contextInfoList[contextIndex].traceStatus = (uint8)Dlt_Context.appIds[applIndex].contextInfoList[contextIndex].traceStatus;
          }
        }
      }
      /* #100 Otherwise, the requested application id is invalid, therefore indicate that an error occurred. */
      else
      {
        retVal = E_NOT_OK;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_GETLOGINFO, errorId);
  }
#  else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
#  endif

  DLT_DUMMY_STATEMENT(Status); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */

  return retVal;
} /* Dlt_GetLogInfo */

/**********************************************************************************************************************
 * Dlt_GetTraceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 * \spec        requires AppId != NULL_PTR;
 *              requires ContextId != NULL_PTR;
 *              requires TraceStatus != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetTraceStatus(
  P2CONST(uint8, AUTOMATIC, RTE_DLT_APPL_DATA) AppId,
  P2CONST(uint8, AUTOMATIC, RTE_DLT_APPL_DATA) ContextId,
  P2VAR(boolean, AUTOMATIC, RTE_DLT_APPL_VAR) TraceStatus)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
#  if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if((AppId == NULL_PTR) || (ContextId == NULL_PTR) || (TraceStatus == NULL_PTR))
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
#  endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise: */
  {
    uint32 applicationId;
    uint32 contextId;
    uint16 appIdIndex;
    uint16 conIdIndex;
    boolean applicationAlreadyRegistered = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */
    boolean contextAlreadyRegistered;


    Dlt_Hlp_CopyStringToLongBigEndian(AppId, &applicationId);
    Dlt_Hlp_CopyStringToLongBigEndian(ContextId, &contextId);

    contextAlreadyRegistered = Dlt_Hlp_IsContextRegistered(applicationId, contextId, &appIdIndex, &conIdIndex, &applicationAlreadyRegistered);

    /*@ assert appIdIndex < Dlt_GetMaxNumberOfApplications(); */ /* VCA_DLT_VALID_CONFIGURED_APP_ID */
    /*@ assert conIdIndex < Dlt_GetMaxNumberOfContextsPerApplication(); */ /* VCA_DLT_VALID_CONFIGURED_CONTEXT_ID */

    /* #30 If the requested application id and context id are registered. */
    if ((applicationAlreadyRegistered == TRUE) && (contextAlreadyRegistered == TRUE)) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
    {
      *TraceStatus = (boolean)Dlt_Context.appIds[appIdIndex].contextInfoList[conIdIndex].traceStatus; /* PRQA S 4330 */ /* MD_MSR_AutosarBoolean */
      retVal = E_OK; /* ESCAN00116851 */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_GETTRACESTATUS, errorId);
  }
#  else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
#  endif

  return retVal;
} /* Dlt_GetTraceStatus */

/**********************************************************************************************************************
 * Dlt_SetDefaultLogLevel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetDefaultLogLevel(
  Dlt_MessageLogLevelType NewLogLevel)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise:
   *       If the new default log level is valid, accept it as default value. */
  {
    if (Dlt_Hlp_ParamInValidRange(DLT_OPT_LOGLEVEL, (sint8)NewLogLevel) == DLT_OK)
    {
      Dlt_DefaultMaxLogLevel = NewLogLevel;
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_SETDEFAULTLOGLEVEL, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_SetDefaultLogLevel */

/**********************************************************************************************************************
 * Dlt_GetDefaultLogLevel
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 * \spec        requires DefaultLogLevel != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetDefaultLogLevel(
  P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_VAR) DefaultLogLevel)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if(DefaultLogLevel == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise, return the current default log level. */
  {
    *DefaultLogLevel = Dlt_DefaultMaxLogLevel;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_GETDEFAULTLOGLEVEL, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_GetDefaultLogLevel */

/**********************************************************************************************************************
 * Dlt_SetDefaultTraceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 * \spec        requires DltCheckLogChannelName();
 * \endspec
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetDefaultTraceStatus(
  boolean NewTraceStatus
#if (DLT_USE_DEFAULT_API == STD_ON)
  , P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName
#endif
  )
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

#if (DLT_USE_DEFAULT_API == STD_ON)
  DLT_DUMMY_STATEMENT(LogChannelName); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
#endif

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise:
   *       If the new default trace status is valid, accept it as default value. */
  {
    /* PRQA S 2991, 2995 1 */ /* MD_Dlt_AlwaysTrue */
    if (Dlt_Hlp_ParamInValidRange(DLT_OPT_TRACESTATUS, (sint8)NewTraceStatus) == DLT_OK) /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
    {
      Dlt_DefaultTraceStatus = NewTraceStatus;
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_SETDEFAULTTRACESTATUS, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_SetDefaultTraceStatus */

/**********************************************************************************************************************
 * Dlt_GetDefaultTraceStatus
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 * \spec        requires DltCheckLogChannelName();
 *              requires TraceStatus != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetDefaultTraceStatus(
# if (DLT_USE_DEFAULT_API == STD_ON)
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName,
# endif
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) TraceStatus)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = DLT_E_NO_ERROR;

# if (DLT_USE_DEFAULT_API == STD_ON)
  DLT_DUMMY_STATEMENT(LogChannelName); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if(TraceStatus == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise, return the current default trace status. */
  {
    *TraceStatus = Dlt_DefaultTraceStatus;
    retVal = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_GETDEFAULTTRACESTATUS, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_GetDefaultTraceStatus */

/**********************************************************************************************************************
 * Dlt_ResetToFactoryDefault
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_ResetToFactoryDefault(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  retVal = E_NOT_OK;

  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise, reset the complete Dlt configuration to default. */
  {
    retVal = Dlt_Hlp_ResetToFactoryDefault();
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_RESETTOFACTORYDEFAULT, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_ResetToFactoryDefault */

/**********************************************************************************************************************
 * Dlt_StoreConfiguration
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DLT_CODE) Dlt_StoreConfiguration(void)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise:
   *       If the NvRAM support is enabled, request to persist the current configuration of Dlt.
   *       Otherwise, indicate that the service is currently not supported. */
  {
# if (DLT_NVRAM_SUPPORT == STD_ON)
    retVal = Dlt_Hlp_StoreConfiguration();
# else
    retVal = DLT_E_NOT_SUPPORTED;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_STORECONFIGURATION, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_StoreConfiguration */

/**********************************************************************************************************************
 * Dlt_SetMessageFiltering
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetMessageFiltering(boolean Status)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise, if the requested value is valid, set the global filter flag to the requested value. */
  {
    /* PRQA S 2991, 2995 1 */ /* MD_Dlt_AlwaysTrue */
    if (Dlt_Hlp_ParamInValidRange(DLT_OPT_BOOLEAN, (sint8)Status) == DLT_OK) /* PRQA S 4303 */ /* MD_MSR_AutosarBoolean */
    {
      Dlt_FilterMessages = Status;
      retVal = E_OK;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_SETMESSAGEFILTERING, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_SetMessageFiltering */

/**********************************************************************************************************************
 * Dlt_GetLogChannelNames
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 * \spec        requires NumberOfLogChannels != NULL_PTR;
 *              requires LogChannelNames != NULL_PTR;
 *              requires $lengthOf(LogChannelNames) >= *NumberOfLogChannels * DLT_DEFAULT_ID_LENGTH;
 * \endspec
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetLogChannelNames(
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) NumberOfLogChannels,
  P2VAR(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelNames)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if ((NumberOfLogChannels == NULL_PTR) || (LogChannelNames == NULL_PTR))
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise: */
  {
    uint8 numberOfRequestedLogChannelNames = *NumberOfLogChannels;
    uint32 numberOfRequestedBytes = (uint32)((uint32)numberOfRequestedLogChannelNames * DLT_DEFAULT_ID_LENGTH);

    /* #30 Set the number of configured log channels. */
    *NumberOfLogChannels = (uint8)Dlt_GetSizeOfLogChannelDescriptor();

    /* #40 If the number of request log channel names is valid:
     *       Copy the requested log channel names to upper layer's buffer.
     *       Indicate success.
     *     Otherwise, indicate an error occurred. */
    if (numberOfRequestedLogChannelNames <= Dlt_GetSizeOfLogChannelDescriptor())
    {
      retVal = E_OK;

      VStdLib_MemCpy(&LogChannelNames[0], &Dlt_LogChannelNames[0], numberOfRequestedBytes); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* VCA_VALID_MEMCOPY_DSTPTR */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_GETLOGCHANNELNAMES, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_GetLogChannelNames */

/**********************************************************************************************************************
 * Dlt_SetLogChannelAssignment
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 * \spec        requires AppId != NULL_PTR;
 *              requires ContextId != NULL_PTR;
 *              requires LogChannelName != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetLogChannelAssignment(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) ContextId,
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName,
  Dlt_AssignmentOperation AddRemoveOp)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if ((AppId == NULL_PTR) || (ContextId == NULL_PTR) || (LogChannelName == NULL_PTR))
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise, change the log channel assignment as requested. */
  {
    Dlt_LocalApplicationIDType appId;
    Dlt_LocalContextIDType contextId;
    uint32 requestedLogChannelId;

    Dlt_Hlp_CopyStringToLongBigEndian(AppId, &appId);
    Dlt_Hlp_CopyStringToLongBigEndian(ContextId, &contextId);
    Dlt_Hlp_CopyStringToLongBigEndian(LogChannelName, &requestedLogChannelId);

    retVal = Dlt_Hlp_SetLogChannelAssignment(appId, contextId, requestedLogChannelId, AddRemoveOp);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_SETLOGCHANNELASSIGNMENT, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_SetLogChannelAssignment */

/**********************************************************************************************************************
 * Dlt_SetLogChannelThreshold
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 * \spec        requires LogChannelName != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_SetLogChannelThreshold(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName,
  Dlt_MessageLogLevelType NewThreshold,
  boolean NewTraceStatus)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if (LogChannelName == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise: */
  {
    uint32 requestedLogChannelId;

    Dlt_Hlp_CopyStringToLongBigEndian(LogChannelName, &requestedLogChannelId);

    /* #30 Set the log channel threshold and trace status of given log channel id. */
    retVal = Dlt_Hlp_SetLogChannelThresholdByName(requestedLogChannelId, NewThreshold, NewTraceStatus);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_SETLOGCHANNELTHRESHOLD, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_SetLogChannelThreshold */

/**********************************************************************************************************************
 * Dlt_GetLogChannelThreshold
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 * \spec        requires LogChannelName != NULL_PTR;
 *              requires Threshold != NULL_PTR;
 *              requires TraceStatus != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, Dlt_CODE) Dlt_GetLogChannelThreshold(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName,
  P2VAR(Dlt_MessageLogLevelType, AUTOMATIC, DLT_APPL_VAR) Threshold,
  P2VAR(boolean, AUTOMATIC, DLT_APPL_VAR) TraceStatus)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if ((LogChannelName == NULL_PTR) || (Threshold == NULL_PTR) || (TraceStatus == NULL_PTR))
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise: */
  {
    uint32 requestedLogChannelId;

    Dlt_Hlp_CopyStringToLongBigEndian(LogChannelName, &requestedLogChannelId);

    /* #30 Get the log channel threshold and trace status of given log channel id. */
    retVal = Dlt_Hlp_GetLogChannelThresholdByName(requestedLogChannelId, Threshold, TraceStatus);
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_GETLOGCHANNELTHRESHOLD, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

  return retVal;
} /* Dlt_GetLogChannelThreshold */

# if (DLT_INJECTION_CALL_SUPPORT == STD_ON)
/**********************************************************************************************************************
 * Dlt_InjectCall
 *********************************************************************************************************************/
/*!
 *
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
/* PRQA S 6060 2 */ /* MD_MSR_STPAR */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, DLT_CODE) Dlt_InjectCall(
  Dlt_SessionIDType SessionId,
  P2CONST(uint8, AUTOMATIC, DLT_CONST) AppId,
  P2CONST(uint8, AUTOMATIC, DLT_CONST) ContextId,
  uint32 ServiceId,
  uint32 DataLength,
  P2CONST(uint8, AUTOMATIC, DLT_CONST) Data)
{
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If there is any development error, report a DET. */
#  if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else if ((AppId == NULL_PTR) || (ContextId == NULL_PTR))
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else if ((DataLength > 0u) && (Data == NULL_PTR))
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
#  endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise: */
  {
    /* #30 If ServiceId is in valid range (0xFFF - 0xFFFFFFFF: */
    if (ServiceId >= 0xFFFu)
    {
      Dlt_LocalApplicationIDType localAppId = 0u;
      Dlt_LocalContextIDType localContextId = 0u;
      uint16 appIndex = 0u;
      uint16 contextIndex = 0u;
      boolean areTheGivenIDsRegistered;
      boolean isApplAlreadyRegistered = FALSE; /* PRQA S 2996 */ /* MD_Dlt_AlwaysFalse */

      /* #40 Convert application and context ids from uint8[4] to uint32 using big endian. */
      Dlt_Hlp_CopyStringToLongBigEndian(AppId, &localAppId);
      Dlt_Hlp_CopyStringToLongBigEndian(ContextId, &localContextId);

      /* #50 If the three ids (application, context and session) are registered together:
       *       Call the corresponding injection callout.
       *     Otherwise, return E_NOT_OK. */
      areTheGivenIDsRegistered = Dlt_Hlp_CheckSessionId(SessionId, localAppId, localContextId, &appIndex, &contextIndex, &isApplAlreadyRegistered);
      (void)isApplAlreadyRegistered;

      if (areTheGivenIDsRegistered == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
      {
        Dlt_InjectionCallOpFuncType injectCallFct = Dlt_Context.appIds[appIndex].contextInfoList[contextIndex].cbkInfo.Dlt_InjectCall_;

        /*@ assert injectCallFct == Dlt_InjectionCallOpClass[$range(0, DLT_NUMBER_OF_SWCS-1)]; */ /* VCA_DLT_VALID_INJECT_CALL_FCT */
        (void)injectCallFct(
          AppId,
          ContextId,
          ServiceId,
          DataLength,
          Data);
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_INJECTCALL, errorId);
  }
#  else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
#  endif
} /* Dlt_InjectCall */
# endif
#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 * Dlt_SetState
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DLT_CODE) Dlt_SetState(Dlt_GlobalStateType NewState)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
#endif
  /* ----- Implementation ------------------------------------------------- */
  {
    /* #10 Enter Dlt_DLT_EXCLUSIVE_AREA_APIS. */
    DLT_BEGIN_CRITICAL_SECTION_APIS();

    /* #20 If Dlt_GlobalState differs from new state, change the Dlt_GlobalStateNew.
     *     Otherwise, the request is invalid.
     */
    switch(Dlt_GlobalState)
    {
      case (DLT_GLOBAL_STATE_OFFLINE):
        if (NewState == DLT_GLOBAL_STATE_ONLINE)
        {
          Dlt_GlobalStateNew = DLT_GLOBAL_STATE_ONLINE;
          retVal = E_OK;
        }
        break;

      case (DLT_GLOBAL_STATE_ONLINE):
        if (NewState == DLT_GLOBAL_STATE_OFFLINE)
        {
          Dlt_GlobalStateNew = DLT_GLOBAL_STATE_OFFLINE;
          retVal = E_OK;
        }
        break;

      default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DLT_DEFAULT_BRANCH */
        break;
    }

    /* #30 Leave Dlt_DLT_EXCLUSIVE_AREA_APIS. */
    DLT_END_CRITICAL_SECTION_APIS();
  }

  /* ----- Development Error Report --------------------------------------- */
#if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_SETSTATE, errorId);
  }
#else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
#endif

  return retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_SetState */

/**********************************************************************************************************************
 * Dlt_GetState
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 * \spec        requires CurrentStatePtr != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, DLT_CODE) Dlt_GetState(
  P2VAR(Dlt_GlobalStateType, AUTOMATIC, DLT_APPL_VAR) CurrentStatePtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
#endif  /* (DLT_DEV_ERROR_DETECT == STD_ON) */
  /* ----- Implementation ------------------------------------------------- */
  {
    /* #10 The request of Dlt_GlobalState is only valid in state DLT_GLOBAL_STATE_OFFLINE and
     *     DLT_GLOBAL_STATE_ONLINE.
     */
    switch(Dlt_GlobalState)
    {
      case (DLT_GLOBAL_STATE_OFFLINE):
        *CurrentStatePtr = (Dlt_GlobalStateType) DLT_GLOBAL_STATE_OFFLINE;
        retVal = E_OK;
        break;

      case (DLT_GLOBAL_STATE_ONLINE):
        *CurrentStatePtr = (Dlt_GlobalStateType) DLT_GLOBAL_STATE_ONLINE;
        retVal = E_OK;
        break;

      default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DLT_DEFAULT_BRANCH */
        break;
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_GETSTATE, errorId);
  }
#else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
#endif

  return retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_GetState */

#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */

/**********************************************************************************************************************
 * Dlt_SetLogChannelDebugMode
 *********************************************************************************************************************/
/*!
 *
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
 * \spec        requires LogChannelName != NULL_PTR;
 * \endspec
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DLT_CODE) Dlt_SetLogChannelDebugMode(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName,
  Dlt_DebugModeType NewDebugMode)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If Dlt is uninitialized or parameter is invalid, report a DET. */
#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
  if (LogChannelName == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
#endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise: */
  {
    /* #30 If the requested log channel is valid: */
    uint32 requestedLogChannelId = 0u;
    uint32 logChannelIndex = 0u;

    Dlt_Hlp_CopyStringToLongBigEndian(LogChannelName, &requestedLogChannelId);

    retVal = Dlt_Hlp_GetLogChannelIndexByLogChannelName(requestedLogChannelId, &logChannelIndex);

    if (retVal == DLT_E_OK)
    {
      /* #40 If the requested debug mode is currently not active:
       *       Iterate over all log channels:
       *         If the requested debug mode is currently not requested, request it now.
       *         Otherwise, return DLT_E_DEBUG_MODE_ALREADY_REQUESTED.
       *     Otherwise, return DLT_E_DEBUG_MODE_ALREADY_ACTIVE.
       */
      if (Dlt_DebugMode[logChannelIndex] != NewDebugMode)
      {
        if (Dlt_DebugModeRequested[logChannelIndex] != NewDebugMode)
        {
          retVal = E_OK;
          Dlt_DebugModeRequested[logChannelIndex] = NewDebugMode;
        }
        else
        {
          retVal = DLT_E_DEBUG_MODE_ALREADY_REQUESTED;
        }
      }
      else
      {
        retVal = DLT_E_DEBUG_MODE_ALREADY_ACTIVE;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_SETDEBUGMODE, errorId);
  }
#else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
#endif

  return retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_SetLogChannelDebugMode */

/**********************************************************************************************************************
 * Dlt_GetLogChannelDebugMode
 *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 * \spec        requires LogChannelName != NULL_PTR;
 *              requires DebugModePtr != NULL_PTR;
 * \endspec
 */
FUNC(Std_ReturnType, DLT_CODE) Dlt_GetLogChannelDebugMode(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName,
  P2VAR(Dlt_DebugModeType, AUTOMATIC, DLT_APPL_VAR) DebugModePtr)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If Dlt is uninitialized or parameter is invalid, report a DET. */
#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
  if (LogChannelName == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
  if (DebugModePtr == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
#endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise: */
  {
    /* #30 If the requested log channel is valid, return the currently active debug mode on the requested log channel. */
    uint32 requestedLogChannelId = 0u;
    uint32 logChannelIndex = 0u;

    Dlt_Hlp_CopyStringToLongBigEndian(LogChannelName, &requestedLogChannelId);

    retVal = Dlt_Hlp_GetLogChannelIndexByLogChannelName(requestedLogChannelId, &logChannelIndex);

    if (retVal == DLT_E_OK)
    {
      *DebugModePtr = Dlt_DebugMode[logChannelIndex];
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_GETDEBUGMODE, errorId);
  }
#else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
#endif

  return retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_GetLogChannelDebugMode */

/**********************************************************************************************************************
 * Dlt_TriggerLogChannelDebugEvent
 *********************************************************************************************************************/
/*!
 *
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
 * \spec        requires LogChannelName != NULL_PTR;
 * \endspec
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, DLT_CODE) Dlt_TriggerLogChannelDebugEvent(
  P2CONST(uint8, AUTOMATIC, DLT_APPL_VAR) LogChannelName)
{
  /* ----- Local Variables ------------------------------------------------ */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If Dlt is uninitialized or parameter is invalid, report a DET. */
#if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
  if (LogChannelName == NULL_PTR)
  {
    errorId = DLT_E_PARAM_POINTER;
  }
  else
#endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise: */
  {
    uint32 requestedLogChannelId = 0u;
    uint32 logChannelIndex = 0u;

    /* #30 If the requested log channel is valid: */
    Dlt_Hlp_CopyStringToLongBigEndian(LogChannelName, &requestedLogChannelId);

    retVal = Dlt_Hlp_GetLogChannelIndexByLogChannelName(requestedLogChannelId, &logChannelIndex);

    if (retVal == DLT_E_OK)
    {
      /* #40 If no new debug mode is requested:
       *       If the debug mode is DLT_DEBUGMODE_ONEVENT_FIRSTISBEST:
       *         If no debug event is currently active, indicate that buffering is now active.
       *         Otherwise, indicate that a debug event is currently active by returning DLT_E_PENDING.
       *       Otherwise, the debug mode does not allow debug events, so return DLT_E_DEBUG_MODE_WRONG.
       *     Otherwise, indicate that debug mode is changing by returning DLT_E_PENDING. */
      if (Dlt_DebugMode[logChannelIndex] == Dlt_DebugModeRequested[logChannelIndex])
      {
        if (Dlt_DebugMode[logChannelIndex] == DLT_DEBUGMODE_ONEVENT_FIRSTISBEST)
        {
          if (Dlt_DebugEventState[logChannelIndex] == DLT_DEBUGEVENT_IDLE)
          {
            Dlt_DebugEventState[logChannelIndex] = DLT_DEBUGEVENT_BUFFERING_ACTIVE;
          }
          else
          {
            retVal = DLT_E_PENDING;
          }
        }
        else
        {
          retVal = DLT_E_DEBUG_MODE_WRONG;
        }
      }
      else
      {
        retVal = DLT_E_PENDING;
      }
    }
  }

  /* ----- Development Error Report --------------------------------------- */
#if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_TRIGGERDEBUGEVENT, errorId);
  }
#else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
#endif

  return retVal; /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_TriggerLogChannelDebugEvent */

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/**********************************************************************************************************************
 * Dlt_MainFunction
 *********************************************************************************************************************/
/*!
 *
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
 *
 *
 */
/* PRQA S 6010 4 */ /* MD_MSR_STPTH */
/* PRQA S 6030 3 */ /* MD_MSR_STCYC */
/* PRQA S 6050 2 */ /* MD_MSR_STCAL */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, DLT_CODE) Dlt_MainFunction(void)
{
#if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  /* ----- Local Variables ------------------------------------------------ */
  uint8 errorId = DLT_E_NO_ERROR;

  /* ----- Development Error Checks --------------------------------------- */
  /* #10 If Dlt is uninitialized, report a DET. */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
  if (Dlt_GlobalState == DLT_GLOBAL_STATE_UNINIT)
  {
    errorId = DLT_E_UNINITIALIZED;
  }
  else
# endif
  /* ----- Implementation ------------------------------------------------- */
  /* #20 Otherwise: */
  {
    uint32 logChannelIndex;

    /* #30 Enter Dlt_DLT_EXCLUSIVE_AREA_APIS. */
    DLT_BEGIN_CRITICAL_SECTION_APIS();

    /* #40 Change global Dlt state if requested. */
    Dlt_Sm_GlobalStateTransition();

    /* #50 If Dlt is globally active: */
    if (Dlt_GlobalState == DLT_GLOBAL_STATE_ONLINE)
    {
      /* #60 If sync time waits for timeout, decrement the counter.
       *       If the counter is 0, change sync time state to DLT_SYNC_TIME_PROCESSING.
       *     If sync time state is DLT_SYNC_TIME_PROCESSING:
       *       Get the current time from StbM.
       *       Trigger one SyncTimestamp response on each log channel.
       *       Reset the sync time counter and its state.
       */
# if (DLT_SYNC_TIME_SUPPORT == STD_ON)
      Dlt_Sm_HandleSyncTimeStamp();
# endif

      /* #70 For each log channel: */
      for (logChannelIndex = 0u; logChannelIndex < Dlt_GetSizeOfLogChannelDescriptor(); logChannelIndex++)
      {
        /* #80 If debug mode change is requested:
         *       Empty the log channel specific send buffer and activate the new debug mode.
         */
        if (Dlt_DebugMode[logChannelIndex] != Dlt_DebugModeRequested[logChannelIndex])
        {
          Dlt_Sm_StateTransition(logChannelIndex, kDltEventDebugModeChanged);
          Dlt_DebugMode[logChannelIndex] = Dlt_DebugModeRequested[logChannelIndex];
          Dlt_DebugEventState[logChannelIndex] = DLT_DEBUGEVENT_IDLE;
        }

        /* #85 If the buffer overflow flag was set:
         *       If the buffer overflow timer was just set to the maximum, send the buffer overflow notification and reset the buffer overflow counter.
         *       Decrement the buffer overflow timer.
         *       If the buffer overflow timer has expired, clear the buffer overflow flag.
         */
        if (Dlt_BufferOverflowNotificationStatus[logChannelIndex].Flag == TRUE)
        {
          if (Dlt_BufferOverflowNotificationStatus[logChannelIndex].Timer == Dlt_GetLogChannelBufferOverflowTimerOfLogChannelDescriptor(logChannelIndex))
          {
            Dlt_Sm_BufferOverflowNotification(logChannelIndex);
            Dlt_BufferOverflowNotificationStatus[logChannelIndex].Counter = 0u;
          }

          Dlt_BufferOverflowNotificationStatus[logChannelIndex].Timer--;

          if (Dlt_BufferOverflowNotificationStatus[logChannelIndex].Timer == 0u)
          {
            Dlt_BufferOverflowNotificationStatus[logChannelIndex].Flag = FALSE;
          }
        }

        switch(Dlt_CurrentState[logChannelIndex])
        {
          /* #90 If DLT state is DLT_STATE_WAIT_FOR_TX_DATA:
           *       If any buffer has content, try to send the data.
           */
          case (DLT_STATE_WAIT_FOR_TX_DATA) :
          {
            boolean hasAnyBufferContent = Dlt_Sm_HasAnyBufferContentAndIsAllowedToSend(logChannelIndex);

            if (hasAnyBufferContent == TRUE) /* PRQA S 2995 */ /* MD_Dlt_AlwaysTrue */
            {
              Dlt_Sm_StateTransition(logChannelIndex, kDltEventOneBufferHasContent);
            }
          }
          break;

          /* #100 If DLT state is DLT_STATE_SENDING, handle state transition and start sync time processing if enabled. */
          case (DLT_STATE_SENDING) :
            Dlt_Sm_HandleSendingTransitions(logChannelIndex);
            break;

          /* #110 Otherwise, report a DET. */
          default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DLT_DEFAULT_BRANCH */
# if (DLT_DEV_ERROR_DETECT == STD_ON)
            errorId = DLT_E_INVALID_STATE;
# endif
            break;
        }
      }
    }

    /* #120 Leave Dlt_DLT_EXCLUSIVE_AREA_APIS. */
    DLT_END_CRITICAL_SECTION_APIS();
  }

  /* ----- Development Error Report --------------------------------------- */
# if (DLT_DEV_ERROR_REPORT == STD_ON)
  if (errorId != DLT_E_NO_ERROR)
  {
    (void)Det_ReportError(DLT_MODULE_ID, 0u, DLT_API_ID_MAINFUNCTION, errorId);
  }
# else
  DLT_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */ /* PC_Dlt_438 */
# endif

#else /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

  /* #130 Update global state. */
  if (Dlt_GlobalState != Dlt_GlobalStateNew)
  {
    Dlt_GlobalState = Dlt_GlobalStateNew;
  }

#endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

  /*lint -e{550} */ /* PC_Dlt_550 */
} /* Dlt_MainFunction */

#define DLT_STOP_SEC_CODE
#include "Dlt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* module specific MISRA deviations:

  MD_Dlt_NvMWriteBlock: Misra rule 1.1
     Reason:          The function 'NvM_WriteBlock' from the NVRAM Manager has a void pointer as a function parameter.
     Risk:            No risk, because the underlying pointer type is known.
     Prevention:      No prevention necessary.

  MD_Dlt_1514:        MISRA rule 8.7: Objects shall be defined at block scope if they are only accessed from within a
                                      single function.
     Reason:          Constants defined global but are used only once. The function uses reference, thus constants
                      have to live after leaving the function.
     Risk:            No error.
     Prevention:      Not required.

   MD_Dlt_1533:       MISRA rule 8.9: An object should be defined at block scope if its identifier only appears in a
                                      single function.
     Reason:          Constants defined global but are used only once. The function uses reference, thus constants
                      have to live after leaving the function.
     Risk:            No error.
     Prevention:      Not required.

  MD_Dlt_3673:        MISRA rule 16.7: A pointer parameter in a function prototype should be declared as pointer to const
                                       if the pointer is not used to modify the addressed object.
     Reason:          The use of these parameters is configuration dependent. In some configurations the pointer is used
                      to modify the addressed object.
     Risk:            No error.
     Prevention:      Not required.

  MD_Dlt_AlwaysTrue:  MISRA rule 2.2: There shall be no dead code
     Reason:          The condition is, depending on the configuration, always true.
     Risk:            No error.
     Prevention:      Not required.

  MD_Dlt_AlwaysFalse: MISRA rule 2.2: There shall be no dead code
     Reason:          The condition is, depending on the configuration, always false.
     Risk:            No error.
     Prevention:      Not required.

  MD_Dlt_STPTH_Extended: MISRA rule VectorMetrics: 6010: Estimated static path count too high (HIS: STPTH = 1..80) %1s
     Reason:          The functions Dlt_TxMsg_Transmit and Dlt_Srv_CallSwcInjection handle too many local parameters,
                      therefore each sub function would require multiple parameters which could violate STPAR and
                      increase the complexity.
     Risk:            No error, as division into sub function would increase complexity.
     Prevention:      Not required.
*/

/* module specific PClint deviations:

  PC_Dlt_438:         PClint rule 438: Last value assigned to variable 'Symbol' not used.
     Reason:          The variables which are not used in functions are covered by dummy statements, but these statements
                      can be defined to nothing.
     Risk:            Undetected unused variables.
     Prevention:      Not required.

  PC_Dlt_550:         PClint rule 550: Symbol 'Symbol' (Location) not accessed.
     Reason:          The errorId is always created and initialized, but if development error reporting is disabled,
                      the variable is never used.
     Risk:            Undetected unused variables.
     Prevention:      Not required.
 */

/* COV_JUSTIFICATION_BEGIN

--- Preprocessor Coverage Justifications ------------------------------------------------------------------------------

\ID COV_DLT_FILTER_ASR_XCP
  \ACCEPT XF tx xf
  \ACCEPT TX tx tx
  \ACCEPT TF tx tf
  \ACCEPT TF xf tf
  \REASON The feature Dlt_Autosar is filtered if according license is not available (see also [CM_DLT_PRECOMPILEOPTIONS]).

\ID COV_DLT_MSR_COMPATIBILITY
  \ACCEPT TX
  \ACCEPT XF
  \REASON [COV_MSR_COMPATIBILITY]

--- Condition Coverage Justifications ---------------------------------------------------------------------------------
\ID COV_DLT_DEFAULT_BRANCH
  \ACCEPT X
  \REASON Rule 15.3 requires a default case at the end of each switch statement.

\ID COV_DLT_RESPONSE_MSG_LENGTH
  \ACCEPT TX
  \ACCEPT TX tx tx
  \REASON The response message will always have a valid length if DET checks are active (which is mandatory for SafeBSW). Only exception is when too many DltLogChannels are configured.
          In this case some services cannot be handled by the DLT as defined by ASR.

\ID COV_DLT_TXMSG_MSGTYPE_VALID
  \ACCEPT TX tf tx
  \ACCEPT TX tx xx
  \REASON The messageType will always be valid if DET checks are active (which is mandatory for SafeBSW).

\ID COV_DLT_NO_RX_DATA_PATH_SUPPORT
  \ACCEPT TX
  \ACCEPT XF
  \ACCEPT TF tf tx
  \ACCEPT TF xf xf tf
  \ACCEPT TF xf tf tf
  \ACCEPT TF tf xf tf
  \REASON In some configurations, the condition will always match here, because validity is already ensured by previous checks. This happens especially in confiugrations where DLT_RX_DATA_PATH_SUPPORT is disabled.

\ID COV_DLT_LOG_CNTX_FCT
  \ACCEPT TX
  \ACCEPT XF
  \REASON When log context specific SetTraceStatus or SetLogLevel functions are configured, parts of Dlt_Hlp_SetOptionInOneContext() will never/always be executed.

\ID COV_DLT_LOG_LVL_CHG_NOTIFICATION
  \ACCEPT TX
  \ACCEPT XF
  \REASON When log channel specific change notification is enabled/disabled, parts of Dlt_Init_RegisterDefaultDltContexts() and Dlt_RegisterContext() will never/always be executed.

\ID COV_DLT_LOG_CHNL_OPTION
  \ACCEPT TX
  \ACCEPT XF
  \REASON When only a single log channel is configured, parts of Dlt_TxMsg_IsMessageValidOnLogChannel(), Dlt_Hlp_SetLogChannelAssignment(), Dlt_Hlp_GetLogChannelIndexByLogChannelName(), Dlt_TxMsg_Transmit(), Dlt_Init_InitializeLogChannelInfo() and Dlt_TpTxConfirmation() will never/always be executed depending on the config settings.

\ID COV_DLT_SWC_CNTX_OPTION
  \ACCEPT TX
  \ACCEPT XF
  \REASON When only a SWC context is configured, parts of Dlt_Init_RegisterDefaultDltContexts() will never/always be executed depending on the config settings.

\ID COV_DLT_MAX_MESSAGES_IN_ONE_TRANSMIT_TOO_GREAT_FOR_BUFFER
  \ACCEPT TX
  \REASON When all buffer sizes of lower layer are smaller than ((DLT_MAX_DLT_MESSAGES_IN_ONE_TRANSMIT * 2) + 1), it is not possible in Dlt_TxBuf_CalculateMessageLength to exit the for loop's exit criteria.

\ID COV_DLT_INITIAl_DEBUG_EVENT
  \ACCEPT TX
  \ACCEPT XF
  \REASON Depending on the configuration of /MICROSAR/Dlt/DltConfigSet/DltLogOutput/DltLogChannel/DltLogChannelInitialDebugEvent the condition will always be true or always be false.

COV_JUSTIFICATION_END */

/* SBSW_JUSTIFICATION_BEGIN

 \ID             VCA_DLT_VALID_CONFIGURED_APP_ID
 \DESCRIPTION    The ApplicationIndex is determined by calling Dlt_Hlp_GetApplicationIDIndex().
                 This function loops for Dlt_Context.appIdCount which is reset in Dlt_Init_InitializeDltContext()
                 and Dlt_Init_RegisterDefaultDltContexts() and is only incremented in Dlt_RegisterContext().
 \COUNTERMEASURE \R Dlt_RegisterContext() has to check Dlt_Context.appIdCount < Dlt_GetMaxNumberOfApplications() before incrementing appIdCount.
                 \M [CM_DLT_VALID_CONFIGURED_APP_ID]

 \ID             VCA_DLT_VALID_CONFIGURED_CONTEXT_ID
 \DESCRIPTION    The ContextIndex is determined by calling Dlt_Hlp_GetContextIDIndex().
                 This function loops for Dlt_Context.appIds[].contextIdCount which is reset in Dlt_Init_InitializeDltContext()
                 and Dlt_Init_RegisterDefaultDltContexts() and is only incremented Dlt_RegisterContext().
 \COUNTERMEASURE \R Dlt_RegisterContext() has to check Dlt_Context.appIds[].contextIdCount < Dlt_GetMaxNumberOfContextsPerApplication() before incrementing contextIdCount.
                 \M [CM_DLT_VALID_CONFIGURED_CONTEXT_ID]

 \ID             VCA_DLT_VALID_CONFIGURED_LOGCHANNEL_ID
 \DESCRIPTION    The LogChannelIndex is determined by calling Dlt_Hlp_GetLogChannelIndexByLogChannelName().
                 This function loops for Dlt_GetSizeOfLogChannelDescriptor().
 \COUNTERMEASURE \M [CM_DLT_VALID_CONFIGURED_LOGCHANNEL_ID]

 \ID             VCA_DLT_VALID_LOG_LVL_CHG_NOTIFICATION_FCT
 \DESCRIPTION    The setLogLevelFct is determined via Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].cbkInfo.Dlt_SetLogLevel_ in Dlt_Hlp_SetOptionInOneContext() or Dlt_RegisterContext().
                 This variable is intitialized based on Dlt_SetLogLevelOpClass[].
 \COUNTERMEASURE \N  Since DLT_NUMBER_OF_SWCS is used for dimensioning the Dlt_SetLogLevelOpClass[] array no further countermeasure is necessary.

 \ID             VCA_DLT_VALID_TRACE_STATUS_FCT
 \DESCRIPTION    The setTraceStatusFct is determined via Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].cbkInfo.Dlt_SetTraceStatus_ in Dlt_Hlp_SetOptionInOneContext()
                 or Dlt_RegisterContext().
                 This variable is intitialized based on Dlt_SetTraceStatusOpClass[].
 \COUNTERMEASURE \N  Since DLT_NUMBER_OF_SWCS is used for dimensioning the Dlt_SetTraceStatusOpClass[] array no further countermeasure is necessary.

 \ID             VCA_DLT_VALID_INJECT_CALL_FCT
 \DESCRIPTION    The injectCallFct is determined via Dlt_Context.appIds[AppIdIndex].contextInfoList[ConIdIndex].cbkInfo.Dlt_InjectCall_ in Dlt_Hlp_SetOptionInOneContext()
                 or Dlt_RegisterContext().
                 This variable is intitialized based on Dlt_InjectionCallOpClass[].
 \COUNTERMEASURE \N  Since DLT_NUMBER_OF_SWCS is used for dimensioning the Dlt_InjectionCallOpClass[] array no further countermeasure is necessary.

 \ID             VCA_VALID_MEMCOPY_DSTPTR
 \DESCRIPTION    The destination pointer and byte count to be copied by VStdLib_MemCopy() is defined by the upperlayer.
 \COUNTERMEASURE \S The upper layer module of Dlt_SendLogMessage(), Dlt_SendTraceMessage(), Dlt_GetLogChannelNames(), Dlt_DetForwardErrorTrace(), Dlt_DemTriggerOnEventStatus()
                    and Dlt_CopyTxData() has to ensure that the buffer array size given by the pointer parameters are matching the given number of elements.

 \ID             VCA_DLT_VALID_SEND_BUFFER_IDX
 \DESCRIPTION    The Dlt_SendBuffer[LogChannelIndex].Write is set in Dlt_TxBuf_WriteToBuf() with Range [0..Dlt_GetLogChannelBufferSizeOfLogChannelDescriptor(LogChannelIndex)].
 \COUNTERMEASURE \R Dlt_TxBuf_WriteToBuf() ensures the range using the following runtime check for overflow detection:
                    (bufferIndex >= (uint32)Dlt_GetLogChannelBufferSizeOfLogChannelDescriptor(LogChannelIndex))
                 \M [CM_DLT_VALID_SEND_BUFFER_IDX]

 \ID             VCA_DLT_VALID_CTRL_BUFFER_IDX
 \DESCRIPTION    The Dlt_ControlSendBuffer[LogChannelIndex].Write is set in Dlt_TxBuf_WriteToCtrlBuf() with Range [0..DLT_CONTROL_BUFFER_SIZE].
 \COUNTERMEASURE \R Dlt_TxBuf_WriteToCtrlBuf() ensures the range using the following runtime check for overflow detection: (bufferIndex >= (uint32)DLT_CONTROL_BUFFER_SIZE)
                 \N Since DLT_CONTROL_BUFFER_SIZE is used for dimensioning the Dlt_ControlSendBuffer[LogChannelIndex].Buffer[] array no further countermeasure is necessary.

 \ID             VCA_DLT_VALID_RECEIVE_BUFFER_IDX
 \DESCRIPTION    The Dlt_ReceiveBuffer.Buffer[] is accessed by bufferIndex in Dlt_CopyRxData() and Dlt_CopyRxData(), the bufferIndex shall be in Range [0..DLT_CONTROL_BUFFER_SIZE].
 \COUNTERMEASURE \R Dlt_CopyRxData() and Dlt_TpRxIndication() ensure the range using the following runtime check for overflow detection: (bufferIndex >= (uint32)DLT_CONTROL_BUFFER_SIZE)
                 \N Since DLT_CONTROL_BUFFER_SIZE is used for dimensioning the Dlt_ReceiveBuffer.Buffer[] array no further countermeasure is necessary.

 \ID             VCA_DLT_VALID_MESSAGE_BUFFER_SIZE
 \DESCRIPTION    The writing the content of the txt message to Dlt_SendData[LogChannelIndex][] the Dlt_TxBuf_CalculateMessageLength() function is used to determine the number of bytes to be copied.
                 Range: [0 .. DLT_MESSAGE_BUFFER_SIZE].
 \COUNTERMEASURE \R Dlt_TxBuf_CalculateMessageLength() ensures the range using the following runtime checks:
                    Dlt_TxBuf_CalculateMessageLength never returns a value larger then DLT_MESSAGE_BUFFER_SIZE, because Dlt_TxBuf_GetLengthOfNextMsg return 0 in case the read index is out of range
                    (ReadIndex >= BufferSize) to indicate end of message. This requires DLT_CONTROL_BUFFER_SIZE to be LessEqual DLT_MESSAGE_BUFFER_SIZE which is ensured by CM_DLT_VALID_CTRL_BUFFER_SIZE.
                 \M [CM_DLT_VALID_CTRL_BUFFER_SIZE]

 \ID             VCA_DLT_VALID_READ_IDX
 \DESCRIPTION    The bufferIndex given to Dlt_TxBuf_SetReadIndex() is because Dlt_TxBuf_WriteBufToTxMsg() uses bufferIndex only a valid range.
 \COUNTERMEASURE \R Dlt_TxBuf_WriteBufToTxMsg() ensures the range of bufferIndex using the following runtime check:
                    (bufferIndex >= (IsControlBuffer)?(bufferIndex < DLT_CONTROL_BUFFER_SIZE):(bufferIndex < DLT_MESSAGE_BUFFER_SIZE)

 \ID             VCA_DLT_VALID_LAST_STATE
 \DESCRIPTION    The lastState is determined by kDltStateMatrix[lastState][transitionEvent] in Dlt_Sm_StateTransition().
 \COUNTERMEASURE \N Since all elements of kDltStateMatrix[][] are either DLT_STATE_UNINIT (0), DLT_STATE_WAIT_FOR_TX_DATA (1) or DLT_STATE_SENDING (2) no further countermeasure is necessary.

 \ID             VCA_DLT_VALID_DATA_PTR
 \DESCRIPTION    The SourceBuffer[] is used as pduInfo.SduDataPtr when calling PduR_DltTransmit(). This SourceBuffer is given when Dlt_TxBuf_WriteBufToTxMsg() is called in Dlt_Sm_SendingDecision().
 \COUNTERMEASURE \N Since either Dlt_ControlSendBuffer[LogChannelIndex].Buffer or Dlt_SendBuffer[LogChannelIndex].Buffer is used as source buffer in Dlt_Sm_SendingDecision()
                    no further countermeasure is necessary.

\ID              VCA_DLT_VALID_CORE_CONFIGURATION
\DESCRIPTION     The Dlt_PayloadOfVthSchedule[], Dlt_PayloadOfVthActivation[], and Dlt_PayloadOfVthSetEvent[] are only accessed with DLT_USED_CORE_ID, which is in Range [0..DLT_NUMBER_OF_CORES].
\COUNTERMEASURE  \R The error directive '# error' in line 88 guarantees that DLT_USED_CORE_ID is in the range [0..DLT_NUMBER_OF_CORES].
                 \N  Since DLT_NUMBER_OF_CORES is used for dimensioning the Dlt_PayloadOfVthSchedule[], Dlt_PayloadOfVthActivation[], and Dlt_PayloadOfVthSetEvent[] array no further countermeasure is necessary.

 SBSW_JUSTIFICATION_END */

 /* ------------------------------------------------------------------------------------------------------------------ */
/* Silent BSW Countermeasures                                                                                         */
/* ------------------------------------------------------------------------------------------------------------------ */
/*
--- MSSV Plugin ---
\CM CM_DLT_PRECOMPILEOPTIONS To ensure that all mandatory / excluded features are as expected, the following must be verified by MSSV:
                               - DLT_USE_COMLAYER_XCP is STD_OFF

\CM CM_DLT_VALID_CONFIGURED_APP_ID To ensure that the array access to Dlt_Context.appIds[] is valid the following must be verified by MSSV:
                               - AssertArraySize(Dlt_LocalLogInfoType.appIds[], Equal, Dlt_GetMaxNumberOfApplications())

\CM CM_DLT_VALID_CONFIGURED_CONTEXT_ID To ensure that the array access to Dlt_Context.appIds[].contextInfoList[] is valid the following must be verified by MSSV:
                               - AssertArraySize(Dlt_LocalApplicationIdInfoType.contextInfoList[], Equal, Dlt_GetMaxNumberOfContextsPerApplication())

\CM CM_DLT_VALID_CONFIGURED_LOGCHANNEL_ID To ensure that the array access to Dlt_NvLogInfoType.logChannelIndexes[] is valid
                              the following must be verified by MSSV:
                               - AssertArraySize(Dlt_NvLogInfoType.logChannelIndexes[], GreaterEqual, Dlt_GetNumberOfAllValidLogChannels())
                               - Assert(Dlt_GetSizeOfLogChannelDescriptor(), Equal, Dlt_GetNumberOfAllValidLogChannels())
                               - Assert(DLT_DEFAULT_LOG_CHANNEL_INDEX, LessThan, Dlt_GetNumberOfAllValidLogChannels())

\CM CM_DLT_VALID_SEND_BUFFER_IDX To ensure that the array access to Dlt_SendBuffer[LogChannelIndex].Buffer[bufferIndex] is valid
                              the following must be verified by MSSV:
                               - Assert(Dlt_GetLogChannelBufferSizeOfLogChannelDescriptor(LogChannelIndex), LessEqual, DLT_MESSAGE_BUFFER_SIZE)

\CM CM_DLT_VALID_CTRL_BUFFER_SIZE To ensure that the array access to Dlt_SendData[LogChannelIndex][] is valid the following must be verified by MSSV:
                               - Assert(DLT_CONTROL_BUFFER_SIZE, LessEqual, DLT_MESSAGE_BUFFER_SIZE)

*/


/**********************************************************************************************************************
 *  END OF FILE: Dlt.c
 *********************************************************************************************************************/

