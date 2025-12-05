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
/*!        \file  DoIPInt_Event.h
 *        \brief  Declarations for sub-module DoIPInt_Event of component DoIPInt.
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

#if !defined (DOIPINT_EVENT_H)
# define DOIPINT_EVENT_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Internal event type for reception events. */
typedef uint8 DoIPInt_InternalEventType;
# define DOIPINT_INTERNAL_EVENT_MSG_INV_PATTERN                      (0u)   /*!< Invalid pattern received (invalid (inverse) protocol version).*/
# define DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_OEM                    (1u)   /*!< The received message is too large for the configured OEM reception buffers.*/
# define DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_ISO                    (2u)   /*!< The received message length exceeds the maximum supported PDU length.*/
# define DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_LIGHT                  (3u)   /*!< The received message length exceeds the maximum supported PDU length.*/
# define DOIPINT_INTERNAL_EVENT_MSG_INV_LENGTH                       (4u)   /*!< The received message length is not valid. */
# define DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_OEM        (5u)   /*!< The message has been received completely or up to DOIPINT_EVENT_MAX_DATA_LEN.*/
# define DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_ISO        (6u)   /*!< The message has been received completely or up to DOIPINT_EVENT_MAX_DATA_LEN.*/
# define DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_LIGHT      (7u)   /*!< The message has been received completely or up to DOIPINT_EVENT_MAX_DATA_LEN.*/
# define DOIPINT_INTERNAL_EVENT_MSG_DISCARDED                        (8u)   /*!< OEM specific message discarded as no reception buffers exist.*/
# define DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_REM_ADDR_ISO            (9u)   /*!< The received message contains an invalid/unknown logical source address.*/
# define DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_REM_ADDR_LIGHT          (10u)  /*!< The received message contains an invalid/unknown logical source address.*/
# define DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_LOCAL_ADDR_ISO          (11u)  /*!< The received message contains an invalid/unknown logical target address.*/
# define DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_LOCAL_ADDR_LIGHT        (12u)  /*!< The received message contains an invalid/unknown logical target address.*/
# define DOIPINT_INTERNAL_EVENT_DIAG_MSG_DISCARDED_ISO               (13u)  /*!< PduR_StartOfReception still fails after retry limit exceeded.*/
# define DOIPINT_INTERNAL_EVENT_DIAG_MSG_DISCARDED_LIGHT             (14u)  /*!< PduR_StartOfReception still fails after retry limit exceeded.*/
# define DOIPINT_INTERNAL_EVENT_DIAG_MSG_FORWARDED_ISO               (15u)  /*!< The diagnostic message has been successfully forwarded to the user.*/
# define DOIPINT_INTERNAL_EVENT_DIAG_MSG_FORWARDED_LIGHT             (16u)  /*!< The diagnostic message has been successfully forwarded to the user.*/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#  define DOIPINT_START_SEC_CODE
#  include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  DoIPInt_Event_Report()
 *********************************************************************************************************************/
/*! \brief        Reports events.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    Event                        Type of event.
 *                                             [range: DOIPINT_INTERNAL_EVENT_MSG_INV_PATTERN,
 *                                                     DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_OEM,
 *                                                     DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_ISO,
 *                                                     DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_LIGHT,
 *                                                     DOIPINT_INTERNAL_EVENT_MSG_INV_LENGTH,
 *                                                     DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_OEM,
 *                                                     DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_ISO,
 *                                                     DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_LIGHT,
 *                                                     DOIPINT_INTERNAL_EVENT_MSG_DISCARDED,
 *                                                     DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_REM_ADDR_ISO,
 *                                                     DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_REM_ADDR_LIGHT,
 *                                                     DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_LOCAL_ADDR_ISO,
 *                                                     DOIPINT_INTERNAL_EVENT_DIAG_MSG_INV_LOCAL_ADDR_LIGHT,
 *                                                     DOIPINT_INTERNAL_EVENT_DIAG_MSG_DISCARDED_ISO,
 *                                                     DOIPINT_INTERNAL_EVENT_DIAG_MSG_DISCARDED_LIGHT,
 *                                                     DOIPINT_INTERNAL_EVENT_DIAG_MSG_FORWARDED_ISO,
 *                                                     DOIPINT_INTERNAL_EVENT_DIAG_MSG_FORWARDED_LIGHT]
 *  \return       DOIPINT_MSG_HANDLER_PENDING  Message handling shall be continued.
 *  \return       DOIPINT_MSG_HANDLER_FINISHED Message handling shall be finished.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message handling shall be skipped.
 *  \return       DOIPINT_MSG_HANDLER_FAILED   Message handling failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_Event_Report(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_InternalEventType Event);

#  define DOIPINT_STOP_SEC_CODE
#  include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DOIPINT_EVENT_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_Event.h
 *********************************************************************************************************************/
