/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  DoIPInt_Types.h
 *        \brief  DoIPInt types header file
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's main source file.
 *********************************************************************************************************************/

#if !defined (DOIPINT_TYPES_H)
# define DOIPINT_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "ComStack_Types.h"
# include "DoIPInt_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES DEFINES
 *********************************************************************************************************************/

/*! Maximum value for type uint32. */
# define DOIPINT_MAX_UINT_32                 (0xFFFFFFFFu)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! Channel Id. */
typedef uint16 DoIPInt_ChannelIdType;

/*! Connection Id. */
typedef uint16 DoIPInt_ConnectionIdType;
 
/*! Request mode of a socket connection. */
typedef uint8 DoIPInt_SoConRequestModeType;
# define DOIPINT_SO_CON_REQUEST_MODE_NONE             (0u)  /*!< No socket connection mode change requested. */
# define DOIPINT_SO_CON_REQUEST_MODE_ONLINE           (1u)  /*!< Socket connection requested to be opened. */
# define DOIPINT_SO_CON_REQUEST_MODE_OFFLINE          (2u)  /*!< Socket connection requested to be closed. */

/*! Transmission state on a connection. */
typedef uint8 DoIPInt_TxStateType;
# define DOIPINT_TX_STATE_NONE                        (0u)  /*!< No transmission is active. */
# define DOIPINT_TX_STATE_GEN_HDR                     (1u)  /*!< Generic Header to be copied. */
# define DOIPINT_TX_STATE_LIGHT_HDR                   (2u)  /*!< Light Header to be copied. */
# define DOIPINT_TX_STATE_USER_DATA                   (3u)  /*!< User data to be copied. */

/*! Reception state on a connection. */
typedef uint8 DoIPInt_RxStateType;
# define DOIPINT_RX_STATE_NONE                        (0u)  /*!< No reception is active. */
# define DOIPINT_RX_STATE_GEN_HDR                     (1u)  /*!< Generic header to be copied. */
# define DOIPINT_RX_STATE_USER_DATA                   (2u)  /*!< User data to be copied. */
# define DOIPINT_RX_STATE_LIGHT_HDR                   (3u)  /*!< Header and user data of light header to be copied. */
# define DOIPINT_RX_STATE_SKIP                        (4u)  /*!< Skip the message. */

/*! Event type for reception events. */
typedef uint8 DoIPInt_EventType;
/*!< Invalid pattern received (invalid (inverse) protocol version). */
# define DOIPINT_EVENT_MSG_INV_PATTERN                (0u)
/*!< The received message exceeds the maximum supported PDU length or is too large for the configured OEM reception
 *   buffers. */
# define DOIPINT_EVENT_MSG_TOO_LARGE                  (1u)
/*!< The received message length is not valid. */
# define DOIPINT_EVENT_MSG_INV_LENGTH                 (2u)
/*!< The message has been received completely or up to DOIPINT_EVENT_MAX_DATA_LEN. */
# define DOIPINT_EVENT_MSG_RECEIVED_UP_TO_LENGTH      (3u)
/*!< OEM specific message discarded as no reception buffers exist or all reception buffers are in use. */
# define DOIPINT_EVENT_MSG_DISCARDED                  (4u)
/*!< The received message contains an invalid/unknown logical source address. */
# define DOIPINT_EVENT_DIAG_MSG_INV_REM_ADDR          (5u)
/*!< The received message contains an invalid/unknown logical target address. */
# define DOIPINT_EVENT_DIAG_MSG_INV_LOCAL_ADDR        (6u)
/*!< PduR_StartOfReception still fails after retry limit exceeded. */
# define DOIPINT_EVENT_DIAG_MSG_DISCARDED             (7u)
/*!< The diagnostic message has been successfully forwarded to the user. */
# define DOIPINT_EVENT_DIAG_MSG_FORWARDED             (8u)

/*! Generic message handler reception state type for different units.
 *  Type definitions can be found in the corresponding units. */
typedef uint8 DoIPInt_MsgHandlerRxStateType;

/*! Return type used by the message handlers. */
typedef uint8 DoIPInt_MsgHndl_RetType;
/*!< The message handler has not yet finished processing the message. */
# define DOIPINT_MSG_HANDLER_PENDING                  (0u)
/*!< The message handler has finished processing the message. */
# define DOIPINT_MSG_HANDLER_FINISHED                 (1u)
/*!< Handling the message failed and the message shall be skipped. */
# define DOIPINT_MSG_HANDLER_SKIP                     (2u)
/*!< Handling the message failed. */
# define DOIPINT_MSG_HANDLER_FAILED                   (3u)

/*! Finding level type for the logical address. */
typedef uint8 DoIpInt_LogAddrFindingLevelType;
/*!< Neither logical remote address nor local address matches. */
# define DOIPINT_FIND_LVL_NONE                        (0u)
/*!< Logical remote address matches. */
# define DOIPINT_FIND_LVL_REMOTE                      (1u)
/*!< Logical remote address and local address matches. */
# define DOIPINT_FIND_LVL_REMOTE_AND_LOCAL            (2u)

/*! API type. */
typedef uint8 DoIPInt_ApiType;
# define DOIPINT_API_TYPE_TP                           0x00u
# define DOIPINT_API_TYPE_IF                           0x01u

/*! Function pointer to <Up>_DoIPIntOemPayloadRxCallback. */
typedef P2FUNC(void, DOIPINT_CODE, DoIPInt_OemPayloadRxCbkType) (
  DoIPInt_ConnectionIdType ConnectionId,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) PayloadDataPtr);

/*! Function pointer to <Up>_DoIPIntEventCallback. */
typedef P2FUNC(Std_ReturnType, DOIPINT_CODE, DoIPInt_EventCbkType) (
  DoIPInt_ConnectionIdType ConnectionId,
  DoIPInt_EventType Event,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) MsgDataPtr);

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif /* DOIPINT_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Types.h
 *********************************************************************************************************************/
