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
 *        \file  Dlt_Types.h
 *        \brief  Dlt Types header file
 *
 *      \details  Types header file of the AUTOSAR Diagnostic Log and Trace, according to:
 *                AUTOSAR Diagnostic Log and Trace, AUTOSAR Release 4.0
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (DLT_TYPES_H_)
# define DLT_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dlt_Cfg.h"
# include "ComStack_Types.h"
# include "Rte_Dlt_Type.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# define DLT_OK                                         0x0u
# define DLT_NOT_SUPPORTED                              0x1u
# define DLT_ERROR                                      0x2u

# define DLT_NOT_OK                                     0x1u

/* Dlt_GlobalStateType */
# define DLT_GLOBAL_STATE_UNINIT                        0x0u
/* DLT_GLOBAL_STATE_OFFLINE and DLT_GLOBAL_STATE_ONLINE are defined in Rte_Dlt_Type.h */

/* Keyword macros */
# if !defined (STATIC)                 /* COV_DLT_MSR_COMPATIBILITY */
#  define STATIC static
# endif

# if !defined (LOCAL_INLINE)           /* COV_DLT_MSR_COMPATIBILITY */
#  define LOCAL_INLINE INLINE STATIC
# endif

# if !defined (DLT_LOCAL)              /* COV_DLT_MSR_COMPATIBILITY */
#  define DLT_LOCAL STATIC
# endif

# if !defined (DLT_LOCAL_INLINE)       /* COV_DLT_MSR_COMPATIBILITY */
#  define DLT_LOCAL_INLINE LOCAL_INLINE
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

/*! \typedef Variables of this type are used to represent the error codes Dlt can return */
typedef Std_ReturnType Dlt_ReturnType;                       /*!< Return type: defining the possible return values of Dlt APIs. */ /* [Dlt238]; ESCAN00082472 */
# define DLT_E_OK                           (0x0u) /*!< Return type value: no error occurred. */
# define DLT_E_MSG_TOO_LARGE                ((Dlt_ReturnType)0x2u) /*!< Return type value: API service Dlt_SendLogMessage or Dlt_SendTraceMessage called with too long message. */
# define DLT_E_CONTEXT_ALREADY_REG          ((Dlt_ReturnType)0x3u) /*!< Return type value: API service Dlt_RegisterContext called repeatedly with same context. */
# define DLT_E_UNKNOWN_SESSION_ID           ((Dlt_ReturnType)0x4u) /*!< Return type value: API service Dlt_RegisterContext, Dlt_SendLogMessage or Dlt_SendTraceMessage called with invalid session ID. */
# define DLT_E_NO_BUFFER                    ((Dlt_ReturnType)0x5u) /*!< Return type value: Not enough buffer available, therefore the message is rejected. */
# define DLT_E_CONTEXT_NOT_YET_REG          ((Dlt_ReturnType)0x6u) /*!< Return type value: The requested context is not registered. */
# define DLT_E_NOT_SUPPORTED                ((Dlt_ReturnType)0x7u) /*!< Return type value: The requested service is not supported. */
# define DLT_E_ERROR                        ((Dlt_ReturnType)0x9u) /*!< Return type value: Generic error. */

# define DLT_E_MESSAGE_REJECTED             ((Dlt_ReturnType)0xA0u) /*!< Return type value: The message was rejected. */
# define DLT_E_DEBUG_MODE_ALREADY_ACTIVE    ((Dlt_ReturnType)0xA1u) /*!< Return type value: The requested debug mode is already active. */
# define DLT_E_DEBUG_MODE_ALREADY_REQUESTED ((Dlt_ReturnType)0xA2u) /*!< Return type value: The requested debug mode is already requested. */
# define DLT_E_DEBUG_MODE_WRONG             ((Dlt_ReturnType)0xA3u) /*!< Return type value: The request is not supported in current debug mode. */
# define DLT_E_PENDING                      ((Dlt_ReturnType)0xA4u) /*!< Return type value: Request cannot be handled at the moment, repeat request later. */

/* ESCAN00082472 */
# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
#  if ((RTE_E_OK != (E_OK)) || (DLT_E_OK != (E_OK)))
#   error "Return values are not interpreted consistently"
#  endif
# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */

/* Dlt458 */
typedef struct
{
  uint8  HeaderType;                                       /*!< Standard header type member: contains flags defining size and content of standard header. */ /* Dlt094: HTYP */
  uint8  MessageCounter;                                   /*!< Standard header type member: contains message counter. */ /* Dlt319: MCNT */
  uint16 Length;                                           /*!< Standard header type member: contains length of complete Dlt message. */ /* Dlt320: Length (LEN) shall hold the total length of non-static data of the Dlt message in byte. */
# if defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
  uint32 EcuID;                                            /*!< Standard header type member: contains ECU ID defined with DLT_ECU_ID. */ /* Optional: depending on HeaderType 0bxx xx x1 xx */
  uint32 SessionID;                                        /*!< Standard header type member: contains session ID added by Rte. */ /* Optional: depending on HeaderType 0bxx xx 1x xx */
  uint32 Timestamp;                                        /*!< Standard header type member: contains timestamp of current Dlt message. */ /* Optional: depending on HeaderType 0bxx x1 xx xx */
# endif /* defined (DLT_USE_COMLAYER_ASR) && (DLT_USE_COMLAYER_ASR == STD_ON) */
} Dlt_StandardHeaderType;                                  /*!< Standard header type: structure type for standard header of Dlt messages. */

/* Dlt228 */
typedef uint32 Dlt_MessageIDType;                          /*!< Message ID type: defining the type of message ID. */

# if (DLT_USE_COMLAYER_XCP == STD_ON) /* COV_DLT_FILTER_ASR_XCP */
typedef uint32 Dlt_DemEventStatusType;                     /*!< Dem event status type: defining the type of an Dem event status. */

# else /* (DLT_USE_COMLAYER_XCP == STD_ON) */

typedef uint8 Dlt_MessageTypeType;                         /*!< Message type type: defining the type of Dlt message. */
#  define DLT_TYPE_LOG                                  0u /*!< Message type type value: a log message. */
#  define DLT_TYPE_APP_TRACE                            1u /*!< Message type type value: a application trace message. */
#  define DLT_TYPE_NW_TRACE                             2u /*!< Message type type value: a network trace message. */
#  define DLT_TYPE_CONTROL                              3u /*!< Message type type value: a control message. */

/* States of the diagnostic log and trace state machine */
typedef uint8 Dlt_StateType;                               /*!< State type: defining the type of sub state machine. */
#  define DLT_STATE_UNINIT                              0u /*!< State type value: the DLT is uninitialized. */
#  define DLT_STATE_WAIT_FOR_TX_DATA                    1u /*!< State type value: Wait for data to be transmitted. */
#  define DLT_STATE_SENDING                             2u /*!< State type value: wait for successful transmission. */

/* Number of states */
#  define DLT_STATE_NUMBER_OF_STATES                    (0x03u)

/* States for the sync time state machine */
typedef uint8 Dlt_SyncTimeStateType;                       /*!< State type: defining the type of the time sync state machine. */
#  define DLT_SYNC_TIME_PROCESSING                      0u /*!< State type value: request current StbM time and prepare SyncTimeStamp response message. */
#  define DLT_SYNC_TIME_WAIT_FOR_TIMEOUT                1u /*!< State type value: wait until sync timeout expired */

/* Valid option settings for GetLogInfo */
#  define DLT_LOG_OPTIONS_RESERVED_3                    (0x03u)
#  define DLT_LOG_OPTIONS_RESERVED_4                    (0x04u)
#  define DLT_LOG_OPTIONS_UNSUSED_5                     (0x05u)
#  define DLT_LOG_OPTIONS_ALL_INFO_WITHOUT_DESCRIPTIONS (0x06u)
#  define DLT_LOG_OPTIONS_ALL_INFO_WITH_DESCRIPTIONS    (0x07u)

/* Valid status settings for GetLogInfo */
#  define DLT_LOG_STATUS_RESERVED_3                     (0x03u)
#  define DLT_LOG_STATUS_RESERVED_4                     (0x04u)
#  define DLT_LOG_STATUS_UNSUSED_5                      (0x05u)
#  define DLT_LOG_STATUS_ALL_INFO_WITHOUT_DESCRIPTIONS  (0x06u)
#  define DLT_LOG_STATUS_ALL_INFO_WITH_DESCRIPTIONS     (0x07u)
#  define DLT_LOG_STATUS_NO_MATCHING_CONTEXT_IDS        (0x08u)
#  define DLT_LOG_STATUS_RESPONSE_DATA_OVERFLOW         (0x09u)

/* Valid status settings for Dlt_MessageNetworkTraceInfoType */
#  define DLT_NW_TRACE_IPC                              (0x01u) /*!< The network trace info type value: Inter process communication */
#  define DLT_NW_TRACE_CAN                              (0x02u) /*!< The network trace info type value: CAN communication */
#  define DLT_NW_TRACE_FLEXRAY                          (0x03u) /*!< The network trace info type value: Flexray communication */
#  define DLT_NW_TRACE_MOST                             (0x04u) /*!< The network trace info type value: MOST communication */
#  define DLT_NW_TRACE_ETHERNET                         (0x05u) /*!< The network trace info type value: Ethernet communication */
#  define DLT_NW_TRACE_SOMEIP                           (0x06u) /*!< The network trace info type value: SOME/IP communication */

#  ifndef DLT_DEBUGMODE_CONTINUOUS_FIRSTISBEST /* COV_DLT_MSR_COMPATIBILITY */
#   define DLT_DEBUGMODE_CONTINUOUS_FIRSTISBEST (0U)
#  endif

#  ifndef DLT_DEBUGMODE_ONEVENT_FIRSTISBEST /* COV_DLT_MSR_COMPATIBILITY */
#   define DLT_DEBUGMODE_ONEVENT_FIRSTISBEST (1U)
#  endif

typedef uint32 Dlt_LocalApplicationIDType;
typedef uint32 Dlt_LocalContextIDType;

typedef uint8 Dlt_MessageControlInfoType;

typedef enum
{
  DLT_DEBUGEVENT_IDLE = 0u,             /*!< Debug event: waiting for an event. */
  DLT_DEBUGEVENT_BUFFERING_ACTIVE = 1u, /*!< Debug event: the event occurred and now buffering is active. */
  DLT_DEBUGEVENT_SENDING_ACTIVE = 2u    /*!< Debug event: the event occurred and now sending is active. */
} Dlt_DebugEventType; /*!< The debug event type */

typedef struct
{
  uint32  Counter;                      /*!< Counter for the amount of lost Dlt messages since last sent BufferOverflowNotification message [SWS_Dlt_00777] [RS_LT_00037] */
  uint16  Timer;                        /*!< Cycle time in milliseconds for resetting the buffer overflow flag in case a buffer overflow occurred. [SWS_Dlt_00760] [ECUC_Dlt_00886] */
  boolean Flag;                         /*!< Flag which indicates that a BufferOverflow occurred. Flag will be cleared after the Dlt_BufferOverflowStatusType.Timer expired. [SWS_Dlt_00760] */
} Dlt_BufferOverflowStatusType;

typedef struct
{
  uint32 InitialWriteIndexOfDltMessage;
  PduLengthType DataLengthToBeReceived;
  PduLengthType CopyRxDataLength;
  boolean IsRxIndicationRelevant;
  uint8 RxIndicationOccured;
  uint8 CopyRxDataCounter;
} Dlt_RxInfoType;

/* Dlt_UsedEndiannessType */
typedef uint8 Dlt_UsedEndiannessType;                                       /*!< Used endianness: the type which endianness shall be used for this DLT message. */
#  define DLT_USED_ENDIANNESS_LITTLE_ENDIAN   DLT_LITTLEENDIAN              /*!< Used endianness: the DLT message shall be transmitted in little endian. */
#  define DLT_USED_ENDIANNESS_BIG_ENDIAN      DLT_BIGENDIAN                 /*!< Used endianness: the DLT message shall be transmitted in big endian. */
#  define DLT_USED_ENDIANNESS_PLATFORM        DLT_HEADER_PAYLOAD_BYTEORDER  /*!< Used endianness: the platform endianness shall be used for the DLT message. */

# endif /* (DLT_USE_COMLAYER_XCP == STD_ON) */
#endif /* DLT_TYPES_H_ */

