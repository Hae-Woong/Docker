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
/*!        \file  CddDrm_Types.h
 *        \brief  CddDrm type definitions
 *
 *      \details  CddDrm definitions of structs and enumerations and simple types.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#ifndef CDDDRM_TYPES_H
# define CDDDRM_TYPES_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "ComStack_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* --- Component Version Information ------------------------------------------------------------------------------- */
# define CDDDRM_TYPES_MAJOR_VERSION                                   (7U)
# define CDDDRM_TYPES_MINOR_VERSION                                   (0U)
# define CDDDRM_TYPES_PATCH_VERSION                                   (1U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* --- Basic Types ------------------------------------------------------------------------------------------------- */
/* Vector specific types */

/*! Response result provided flag type */
typedef boolean CddDrm_ResultFlagType;

/*! Timeout counter type */
typedef uint32 CddDrm_TimerType;

/*! Response message length type */
typedef PduLengthType CddDrm_LengthType;

/*! Length of buffer */
typedef PduLengthType CddDrm_BufferLengthType;

/*! Drm specific return type */
typedef uint8 CddDrm_RequestReturnType;
# define CDDDRM_REQUEST_OK                                            (0x00U) /*!< Return code: No error */
# define CDDDRM_REQUEST_NO_CHANNEL                                    (0x01U) /*!< Return code: Channel can not be allocated */
# define CDDDRM_REQUEST_TESTER_ACTIVE                                 (0x02U) /*!< Return code: External Tester is detected */
# define CDDDRM_REQUEST_FIREWALL_BLOCKED                              (0x03U) /*!< Return code: Service is blocked by Firewall Rule */
# define CDDDRM_REQUEST_CONNECTION_BUSY                               (0x04U) /*!< Return code: Connection is in use */
# define CDDDRM_REQUEST_FUNCTIONAL_ACTIVE                             (0x05U) /*!< Return code: Functional request is active */
# define CDDDRM_REQUEST_ECUD_ACTIVE                                   (0x06U) /*!< Return code: ECU detection is active */

/*! Ecu Detection: Connection state */
typedef uint8 CddDrm_EcudStateType;
# define CDDDRM_ECUD_CONNECTION_NOT_DISCOVERED                        (0x00U) /*!< ECU detection not performed for this connection */
# define CDDDRM_ECUD_CONNECTION_NOT_AVAILABLE                         (0x01U) /*!< ECU not available */
# define CDDDRM_ECUD_CONNECTION_AVAILABLE                             (0x02U) /*!< ECU available */

/*! Functional request result state */
typedef uint8 CddDrm_FuncReqResultStateType;
# define CDDDRM_FUNC_REQ_RESULT_OK                                    (0x00U) /*!< Request result provided */
# define CDDDRM_FUNC_REQ_RESULT_NOT_OK                                (0x01U) /*!< Operation failed */
# define CDDDRM_FUNC_REQ_RESULT_BUFFER_TOO_SMALL                      (0x02U) /*!< Buffer too small for request result message */

/*! Connection channel state type */
typedef uint8 CddDrm_ChannelStateType;

/*! Connection channel id type */
typedef uint8 CddDrm_ChannelIdType;

/*! Send Response/NRC to application  */
typedef uint8 CddDrm_ResponseCodeType;
# define CDDDRM_RESPONSE_POSITIVE                                     (0x00U) /*!< Positive response */
# define CDDDRM_RESPONSE_TIMEOUT                                      (0xd0U) /*!< Response timeout */
# define CDDDRM_RESPONSE_INVALID_NRC_LENGTH                           (0xd1U) /*!< NRC message length does not equal 3byte */
# define CDDDRM_RESPONSE_RCRRP_LIMIT_REACHED                          (0xd2U) /*!< Received response pending message limit reached */
# define CDDDRM_RESPONSE_BUFFER_TOO_SMALL                             (0xd3U) /*!< PduR receive buffer is too small */
# define CDDDRM_RESPONSE_WRONG_SERVICE                                (0xd4U) /*!< Received response is an invalid service */
# define CDDDRM_RESPONSE_CHANNELS_CLOSED                              (0xd5U) /*!< Channels are closed by application */
# define CDDDRM_RESPONSE_TESTER_DETECTED                              (0xd6U) /*!< External Tester is detected */
# define CDDDRM_RESPONSE_PDUR_RX_ERROR                                (0xd7U) /*!< Receive from PduR is failed */
# define CDDDRM_RESPONSE_PDUR_TX_ERROR                                (0xd8U) /*!< Data transmission to PduR failed */
# define CDDDRM_RESPONSE_BUFFER_FULL                                  (0xd9U) /*!< Response buffer is full */
# define CDDDRM_RESPONSE_FORWARDED_RCRRP                              (0x78U) /*!< RCRRP notification forwarded to the application */

/*! Configuration Type */
typedef P2CONST(uint8, TYPEDEF, CDDDRM_CONST_PBCFG) CddDrm_ConfigPtrType;

/*! Buffer pointer type */
typedef P2VAR(uint8, TYPEDEF, CDDDRM_APPL_DATA) CddDrm_BufferDataPtrType;

/*! Index for the static configured connection list */
typedef PduIdType CddDrm_ConnectionIdType;

/*! Index for response buffer */
typedef uint8 CddDrm_BufferIdxType;

/*! Response data for application */
typedef struct
{
  CddDrm_LengthType        responseLength; /*!< Length of data in respData */
  CddDrm_ResponseCodeType  responseCode;   /*!< PRC/NRC */
  CddDrm_ConnectionIdType  connectionId;   /*!< reference for static ConnectionConfig Table */
  uint8                    serviceId;      /*!< ServiceId of the request */
} CddDrm_RespInfoStructType;

/*! Response data for application for a functional request */
typedef struct
{
  CddDrm_BufferDataPtrType    responseBufferDataPtr;  /*!< Response buffer */
  CddDrm_LengthType           responseBufferSize;     /*!< IN: Response buffer size. OUT: Number of bytes copied in response buffer */
  CddDrm_ResponseCodeType     responseCode;           /*!< PRC/NRC */
} CddDrm_FuncReqRespInfoStructType;

/* Callback pointer */
typedef P2FUNC(void, CDDDRM_APPL_CODE, CddDrm_ResponseNotificationPtr)(P2CONST(CddDrm_RespInfoStructType, AUTOMATIC, CDDDRM_CONST) Response);
/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif /* CDDDRM_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: CddDrm_Types.h
 *********************************************************************************************************************/
