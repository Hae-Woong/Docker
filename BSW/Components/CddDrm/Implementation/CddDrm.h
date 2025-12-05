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
/*!        \file  CddDrm.h
 *        \brief  CddDrm header file
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author    Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2016-01-25  viskem    ESCAN00089380 FEAT-1864: Release DRM (Diagnostic Request Manager)
 *                        visade    ESCAN00089380 FEAT-1864: Release DRM (Diagnostic Request Manager)
 *  01.00.01  2018-03-20  vissko    ESCAN00098827 A final response is not processed correctly if received immediately after of a RCRRP frame
 *  02.00.00  2018-06-08  visygr    STORYC-5335   Codebase analysis and MISRA-C:2012 migration
 *  02.01.00  2018-09-06  visygr    STORYC-5728   Allow transmission of functional requests
 *                        visygr    STORYC-5730   Allow reception of functional request responses
 *  03.00.00  2019-06-14  visygr    STORY-12803   Extend P2* value range
 *  03.01.00  2020-05-07  vsarcmeba
 *                        vsarcesta CARC-283      Remove blocking time for functional 3E with SPRMIB set
 *  04.00.00  2020-08-25  vsarcmeba ESCAN00106009 Race condition issue fix
 *                        vsarcesta CARC-400      Make RCRRP run-time configurable
 *                        vsarcesta CARC-338      Refactor vDrm as UL
 *                        vsarcmiem CARC-340      Add LL functionality
 *                        vsarcesta CARC-187      Extended request payload size
 *                        vsarcmeba ESCAN00104309 Functional req cancellation
 *  04.00.01  2020-09-15  vsarcesta ESCAN00107328 P2 not extended for functional responses
 *  04.01.00  2020-10-12  vsarcmiem CARC-540      Clean up/Improvements of the CddDrm
 *  04.01.01  2020-11-13  haelvero  ESCAN00107525 Handle close channel for synchronous transmission
 *  05.00.00  2021-06-15  vsarcmiem ESCAN00108524 Adapt state handling
 *                        vsarcmimo CARC-1276     Adapt implementation to use <Ma>_MemMap.h
 *                        vsarcmiem CARC-1356     Support Dynamic Connection
 *                        vsarcmiem ESCAN00109168 Channel not closed after req for func req with SPRMIB
 *  05.00.01  2021-07-29  vsarcmimo ESCAN00109870 Transmission of simultaneous messages fails.
 *                        vsarcmimo ESCAN00109866 Memory sections of DRM are named incorrectly.
 *                        vsarcmimo ESCAN00109850 Missing memory section definitions when MemMap-generator is not used.
 *  05.00.02  2021-08-17  vsarcmiem CARC-1732     Support Helix QAC 2021.1
 *  05.00.03  2021-08-30  vsarcmiem ESCAN00110036 CopyTxData includes metadata when copying data to CanTp
 *            2021-08-31  vsarcmiem ESCAN00110053 Functional request for Tester Present (0x3E) results in error during following functional request
 *  05.00.04  2021-09-07  vsarcmiem CARC-1815     Add additional metadata pointer for request.
 *  05.00.05  2021-11-25  vsarcmiem ESCAN00110713 Error during segmented internal communication request/response
 *  06.00.00  2022-11-02  vsarcmiem CARC-3105     SafeBSW for vDRM
 *  06.01.00  2023-01-31  vsarcmiem ESCAN00111078 Remove const from arguments of CddDrm_StartOfReception, CddDrm_CopyRxData, CddDrm_CopyTxData
 *  07.00.00  2024-01-10  vsarcmiem CARC-3944     Support functional TesterPresent(3E) request with SPRMIB to be processed in paralell with any request.
 *  07.00.01  2024-02-09  vsarcmiem ESCAN00116685 P3 Functional Timer blocks physical requests
 *  07.00.01  2024-02-20  vsarcmiem CARC-4046     Functional TesterPresent ($3E 80) shall not be affected by P3 functional delay
 *********************************************************************************************************************/

#if !defined (CDDDRM_H)
# define CDDDRM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "CddDrm_Cfg.h"
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

# if !defined (CDDDRM_LOCAL)
#  define CDDDRM_LOCAL                                                static
# endif

# if !defined (CDDDRM_LOCAL_INLINE)
#  define CDDDRM_LOCAL_INLINE                                         LOCAL_INLINE
# endif

/* Vendor and module identification */
# define CDDDRM_VENDOR_ID                                             (30U)
# define CDDDRM_MODULE_ID                                             (0xffU)

/* AUTOSAR Software specification version information */
# define CDDDRM_AR_RELEASE_MAJOR_VERSION                              (4U)
# define CDDDRM_AR_RELEASE_MINOR_VERSION                              (0U)
# define CDDDRM_AR_RELEASE_REVISION_VERSION                           (0U)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define CDDDRM_SW_MAJOR_VERSION                                      (7U)
# define CDDDRM_SW_MINOR_VERSION                                      (0U)
# define CDDDRM_SW_PATCH_VERSION                                      (1U)

# define CDDDRM_INSTANCE_ID_DET                                       (0x00U)

/* ----- API service IDs ----- */
# define CDDDRM_SID_INIT                                              (0x00U) /*!< Service ID: CddDrm_Init()                     */
# define CDDDRM_SID_GET_VERSION_INFO                                  (0x02U) /*!< Service ID: CddDrm_GetVersionInfo()           */
# define CDDDRM_SID_MEMORY_INIT                                       (0x03U) /*!< Service ID: CddDrm_InitMemory()               */
# define CDDDRM_SID_MAINFUNCTION                                      (0x04U) /*!< Service ID: CddDrm_MainFunction()             */

/* ----- General API services ----- */
# define CDDDRM_SID_CANCEL_REQUEST                                    (0x11U) /*!< Service ID: CddDrm_CancelRequest()            */
# define CDDDRM_SID_EXTERNAL_TESTER_CONNECTED                         (0x12U) /*!< Service ID: CddDrm_ExternalTesterConnected()  */
# define CDDDRM_SID_ECU_DETECTION_START                               (0x13U) /*!< Service ID: CddDrm_StartEcuDetection()        */
# define CDDDRM_SID_ECU_DETECTION_STOP                                (0x14U) /*!< Service ID: CddDrm_StopEcuDetection()         */
# define CDDDRM_SID_ECU_DETECTION_RESULT                              (0x15U) /*!< Service ID: CddDrm_GetEcuDetectionResult()    */
# define CDDDRM_SID_INIT_ECU_DETECTION_DATA                           (0x16U) /*!< Service ID: CddDrm_NvM_InitEcuDetectionData() */
# define CDDDRM_SID_GET_FUNC_REQUEST_RESULT                           (0x17U) /*!< Service ID: CddDrm_GetFuncRequestResult()     */
# define CDDDRM_SID_START_OF_RECEPTION                                (0x18U) /*!< Service ID: CddDrm_StartOfReception()         */
# define CDDDRM_SID_COPY_RX_DATA                                      (0x19U) /*!< Service ID: CddDrm_CopyRxData()               */
# define CDDDRM_SID_TP_RX_INDICATION                                  (0x1AU) /*!< Service ID: CddDrm_TpRxIndication()           */
# define CDDDRM_SID_COPY_TX_DATA                                      (0x1BU) /*!< Service ID: CddDrm_CopyTxData()               */
# define CDDDRM_SID_TP_TX_CONFIRMATION                                (0x1CU) /*!< Service ID: CddDrm_TpTxConfirmation()         */
# define CDDDRM_SID_CANTP_RX_METADATA                                 (0x1DU) /*!< Service ID: CddDrm_CanTpRxMetaData()          */
# define CDDDRM_SID_CANTP_TX_METADATA                                 (0x1EU) /*!< Service ID: CddDrm_CanTpTxMetaData()          */
/* ----- Service request APIs ----- */
# define CDDDRM_SID_SVC_SEND                                          (0x20U) /*!< Service ID: CddDrm_SvcSend()                  */
# define CDDDRM_SID_SVC_SEND_10                                       (0x21U) /*!< Service ID: CddDrm_SvcSend_10()               */
# define CDDDRM_SID_SVC_SEND_11                                       (0x22U) /*!< Service ID: CddDrm_SvcSend_11()               */
# define CDDDRM_SID_SVC_SEND_1902                                     (0x23U) /*!< Service ID: CddDrm_SvcSend_1902()             */
# define CDDDRM_SID_SVC_SEND_1904                                     (0x24U) /*!< Service ID: CddDrm_SvcSend_1904()             */
# define CDDDRM_SID_SVC_SEND_22                                       (0x25U) /*!< Service ID: CddDrm_SvcSend_22()               */
# define CDDDRM_SID_SVC_SEND_27                                       (0x26U) /*!< Service ID: CddDrm_SvcSend_27()               */
# define CDDDRM_SID_SVC_SEND_28                                       (0x27U) /*!< Service ID: CddDrm_SvcSend_28()               */
# define CDDDRM_SID_SVC_SEND_31                                       (0x28U) /*!< Service ID: CddDrm_SvcSend_31()               */
# define CDDDRM_SID_SVC_SEND_34                                       (0x29U) /*!< Service ID: CddDrm_SvcSend_34()               */
# define CDDDRM_SID_SVC_SEND_36                                       (0x2aU) /*!< Service ID: CddDrm_SvcSend_36()               */
# define CDDDRM_SID_SVC_SEND_37                                       (0x2bU) /*!< Service ID: CddDrm_SvcSend_37()               */
# define CDDDRM_SID_SVC_SEND_3E                                       (0x2cU) /*!< Service ID: CddDrm_SvcSend_3E()               */
# define CDDDRM_SID_SVC_SEND_85                                       (0x2dU) /*!< Service ID: CddDrm_SvcSend_85()               */
/* ----- API services for Lower Layer Drm ----- */
# define CDDDRM_SID_TRANSMIT                                          (0x10U) /*!< Service ID: CddDrm_Transmit()                 */
# define CDDDRM_SID_CANCELTRANSMIT                                    (0x40U) /*!< Service ID: CddDrm_CancelTransmit()           */
# define CDDDRM_SID_CANCELRECEIVE                                     (0x42U) /*!< Service ID: CddDrm_CancelReceive()            */

/* ----- Error codes ----------------------------------------------------------------------------------------------- */
# define CDDDRM_E_NO_ERROR                                            (0x00U) /*!< used to check if no error occurred - use a value unequal to any error code */
# define CDDDRM_E_PARAM_POINTER                                       (0x03U) /*!< Error code: API service used with invalid pointer parameter (NULL) */
# define CDDDRM_E_PARAM_VALUE                                         (0x04U) /*!< Error code: API service used with invalid parameter value */
# define CDDDRM_E_UNINIT                                              (0x05U) /*!< Error code: API service used without module initialization */
# define CDDDRM_E_ALREADY_INITIALIZED                                 (0x06U) /*!< Error code: The service CddDrm_Init() is called while the module is already initialized  */
# define CDDDRM_E_INVALID_CONNECTION                                  (0x07U) /*!< Error code: API service used with an invalid connection id */
# define CDDDRM_E_INVALID_STATE                                       (0x08U) /*!< Error code: API service used in invalid state */
# define CDDDRM_E_INVALID_BUFFER_LENGTH                               (0x0aU) /*!< Error code: API service used with invalid buffer length parameter */
# define CDDDRM_E_PDU_ID_RX_OUT_OF_RANGE                              (0x0bU) /*!< Error code: API service used with Rx-PDU-Id that is not configured */
# define CDDDRM_E_PDU_ID_TX_OUT_OF_RANGE                              (0x0cU) /*!< Error code: API service used with Tx-PDU-Id that is not configured */
# define CDDDRM_E_API_ERROR                                           (0x0dU) /*!< Error code: API service called, but is not expected */

/* ----- Modes ----------------------------------------------------------------------------------------------------- */
# define CDDDRM_UNINIT                                                (0x00U)
# define CDDDRM_INIT                                                  (0x01U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/* ----- Interrupt control ----------------------------------------------------------------------------------------- */
# define CddDrm_EnterCritical_ChannelManager()                        SchM_Enter_CddDrm_CDDDRM_EXCLUSIVE_AREA_0()                                                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define CddDrm_LeaveCritical_ChannelManager()                        SchM_Exit_CddDrm_CDDDRM_EXCLUSIVE_AREA_0()                                                       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
typedef struct
{
  CddDrm_ResultFlagType       responseProvided;                                 /*!< If Response data provided to the application and can be rewritten */
  CddDrm_ConnectionIdType     connectionId;                                     /*!< reference for static ConnectionConfig Table */
  CddDrm_LengthType           responseBufferSize;                               /*!< IN: Response buffer size. OUT: Number of bytes copied in response buffer */
  CddDrm_ResponseCodeType     responseCode;                                     /*!< PRC/NRC */
  uint8                       responseBuffer[CDDDRM_CFG_FUNC_RESP_BUFFER_SIZE]; /* Buffer to contain data */

} CddDrm_FuncReqRespBufferType;
#endif
/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define CDDDRM_START_SEC_CODE
# include "CddDrm_MemMap.h"                                                                                                                                 /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  CddDrm_InitMemory()
 *********************************************************************************************************************/
/*! \brief         Function for *_INIT_*-variable initialization
 *  \details       Service to initialize module global variables at power up. This function initializes the
 *                 variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre           Module is uninitialized.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CDDDRM_CODE) CddDrm_InitMemory(void);

/**********************************************************************************************************************
 *  CddDrm_Init()
 *********************************************************************************************************************/
/*! \brief         Initialization function
 *  \details       This function initializes the module DRM. It initializes all variables and sets the module state to
 *                 initialized.
 *  \param[in]     configPtr               Configuration structure for initializing the module
 *  \pre           Interrupts are disabled.
 *  \pre           Module is uninitialized.
 *  \pre           CddDrm_InitMemory has been called unless CddDrm_ModuleInitialized is initialized by start-up code.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \note          Specification of module initialization
 *********************************************************************************************************************/
FUNC(void, CDDDRM_CODE) CddDrm_Init(P2CONST(CddDrm_ConfigPtrType, AUTOMATIC, CDDDRM_PBCFG) configPtr);

/**********************************************************************************************************************
*  CddDrm_ExternalTesterConnected()
*********************************************************************************************************************/
/*! \brief         Close all open connections.
*  \details       Force close of all open connections and prevent that new connections can be opened
*  \param[in]     present        sets the present state of an external tester
*  \return        E_OK:          always returned
*  \return        E_NOT_OK:      only if DET is enabled and an error is detected
*  \context       TASK|ISR2
*  \reentrant     TRUE
*  \synchronous   TRUE
*  \pre           Module is initialized.
*********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_ExternalTesterConnected(boolean present);

/**********************************************************************************************************************
 *  CddDrm_MainFunction()
 *********************************************************************************************************************/
/*! \brief         Handles all internal used timers and state machines
 *  \details       The main function provides a time base to the DRM module. Additionally all tasks which have been
 *                 delayed/shifted from DRM external context, e.g. send a diagnostic request or process ECU detection,
 *                 to DRM context will be processed in context of the main function.
 *
 *                 If the DRM is not initialized, calling this function has no effect.
 *  \pre           Module is initialized.
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CDDDRM_CODE) CddDrm_MainFunction(void);

/**********************************************************************************************************************
 *  CddDrm_SvcSend()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to a connection.
 *  \details       Requests a message that is build by application.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     sprmib                           Indicates to DRM if the SPRMIB was set from application and thus a
 *                                                  Positive response is expected or not.
 *  \param[in,out] bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer shall always be valid and contain as much data as the request length.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.
 *  \param[in]     requestLength                    Request data length

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend(CddDrm_ConnectionIdType connectionId,
                                                           boolean sprmib,
                                                           P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo,
                                                           CddDrm_LengthType requestLength);

# if (CDDDRM_CFG_SUPPORT_SERVICE_10 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_10()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending a Diagnostic Session Control request to configured connection.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     sessionType                      Diagnostic Session Type
 *  \param[in]     sprmib                           Suppresses positive response
 *  \param[out]    bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer is not used in this function therefore it does not have to be valid.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_10(CddDrm_ConnectionIdType connectionId,
                                                              uint8 sessionType,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_11 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_11()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending a ECU Reset.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     resetType                        ECU Reset Type
 *  \param[in]     sprmib                           Suppresses positive response
 *  \param[out]    bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer is not used in this function therefore it does not have to be valid.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_11(CddDrm_ConnectionIdType connectionId,
                                                              uint8 resetType,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_1902 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_1902()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests DTCs by status mask
 *  \param[in]     connectionId                     connection related to the ECU to which the request shall be send
 *  \param[in]     dtcStatusMask                    DTC Status Bit Mask
 *  \param[out]    bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer is not used in this function therefore it does not have to be valid.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_1902(CddDrm_ConnectionIdType connectionId,
                                                                uint8 dtcStatusMask,
                                                                P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_1904 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_1904()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests DTC Snapshot data.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     dtc                              DTC
 *  \param[in]     recordNumber                     DTC Record number
 *  \param[out]    bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer is not used in this function therefore it does not have to be valid.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_1904(CddDrm_ConnectionIdType connectionId,
                                                                uint32 dtc,
                                                                uint8 recordNumber,
                                                                P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_22 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_22()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests data by identifier.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     DID                              The data identifier that is requested
 *  \param[out]    bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer is not used in this function therefore it does not have to be valid.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_22(CddDrm_ConnectionIdType connectionId,
                                                              uint16 DID,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_27 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_27()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests security access service to request seed or send key.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     subFunction                      Select request seed / send key
 *  \param[in]     dataLength                       Seed/Key length
 *  \param[in]     sprmib                           Suppresses positive response
 *  \param[in,out] bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer shall always be valid and contain as much data as the request length.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           External Tester is not present.
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_27(CddDrm_ConnectionIdType connectionId,
                                                              uint8 subFunction,
                                                              CddDrm_LengthType dataLength,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_28 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_28()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests communication control service.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     subFunction                      Control type
 *  \param[in]     communicationType                Is bit coded to control multiple communication types
 *  \param[in]     nodeIdNumber                     Node identification number (only required if sub-function 0x04 or 0x05)
 *  \param[in]     sprmib                           Suppresses positive response
 *  \param[out]    bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer is not used in this function therefore it does not have to be valid.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_28(CddDrm_ConnectionIdType connectionId,
                                                              uint8 subFunction,
                                                              uint8 communicationType,
                                                              uint16 nodeIdNumber,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_31 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_31()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending routine control service.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     subFunction                      Routine control sub-function
 *  \param[in]     routineId                        Routine identifier
 *  \param[in]     routineOptionLength              Length of routine options
 *  \param[in]     sprmib                           Suppresses positive response
 *  \param[in,out] bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer shall always be valid and contain as much data as the request length.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_31(CddDrm_ConnectionIdType connectionId,
                                                              uint8 subFunction,
                                                              uint16 routineId,
                                                              CddDrm_LengthType routineOptionLength,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_34 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_34()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending request download service.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     dataFormatId                     Data format identifier
 *  \param[in]     addressAndLength                 Memory address and size bytes
 *  \param[in]     dataLength                       Request Data length of memory address and size bytes
 *  \param[in,out] bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer shall always be valid and contain as much data as the dataLength.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_34(CddDrm_ConnectionIdType connectionId,
                                                              uint8 dataFormatId,
                                                              uint8 addressAndLength,
                                                              CddDrm_LengthType dataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_36 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_36()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending transfer data service.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     blockSeqCounter                  Block sequence
 *  \param[in]     transferDataLength               Length of the data that will be transfered
 *  \param[in,out] bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer shall always be valid and contain as much data as the transferDataLength.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_36(CddDrm_ConnectionIdType connectionId,
                                                              uint8 blockSeqCounter,
                                                              CddDrm_LengthType transferDataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_37 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_37()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to configured connection.
 *  \details       Requests sending request transfer exit service.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     transferDataLength               Length of the data that will be transfered
 *  \param[in,out] bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer shall always be valid and contain as much data as the transferDataLength.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_37(CddDrm_ConnectionIdType connectionId,
                                                              CddDrm_LengthType transferDataLength,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_3E == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_3E()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to a connection.
 *  \details       Requests sending tester present service.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     sprmib                           Suppresses positive response
 *  \param[out]    bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer is not used in this function therefore it does not have to be valid.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_3E(CddDrm_ConnectionIdType connectionId,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_SERVICE_85 == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_SvcSend_85()
 *********************************************************************************************************************/
/*! \brief         Tries to start new service request to a connection.
 *  \details       Requests UDS service: Control DTC settings.
 *  \param[in]     connectionId                     Connection related to the ECU to which the request shall be send
 *  \param[in]     subFunction                      Control DTC setting sub-function
 *  \param[in]     recordLength                     Length of optional record data
 *  \param[in]     sprmib                           Suppresses positive response
 *  \param[in,out] bufferInfo                       Contains request buffer, response buffer, response buffer size. 
                                                    The request buffer shall always be valid and contain as much data as the recordLength.
                                                    The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.

 *  \return        CDDDRM_REQUEST_OK:               Service request accepted
 *  \return        CDDDRM_REQUEST_NO_CHANNEL:       Service request not accepted because connection already active or
 *                                                  no channels available
 *  \return        CDDDRM_REQUEST_CONNECTION_BUSY:  Service request is not accepted, connection already open.
 *  \return        CDDDRM_REQUEST_TESTER_ACTIVE:    Service request is not accepted, DRM has detected an external tester
 *  \return        CDDDRM_REQUEST_FIREWALL_BLOCKED: Service request is not accepted, service is not allowed to connection
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   FALSE
 *********************************************************************************************************************/
FUNC(CddDrm_RequestReturnType, CDDDRM_CODE) CddDrm_SvcSend_85(CddDrm_ConnectionIdType connectionId,
                                                              uint8 subFunction,
                                                              CddDrm_LengthType recordLength,
                                                              boolean sprmib,
                                                              P2CONST(CddDrm_BufferStructType, AUTOMATIC, CDDDRM_APPL_CONST) bufferInfo);
# endif

# if (CDDDRM_CFG_SUPPORT_CANCELLATION == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_CancelRequest()
 *********************************************************************************************************************/
/*! \brief         Cancels request on a connection.
 *  \details       This function cancel requests that will be sent through the connection.
 *  \param[in]     connectionId       Connection that want to be closed.
 *  \return        E_OK:              Cancel request accepted
 *  \return        E_NOT_OK:          Cancel request not accepted
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \pre           Module is initialized.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelRequest(CddDrm_ConnectionIdType connectionId);
# endif

# if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
*  CddDrm_NvM_InitEcuDetectionData()
*********************************************************************************************************************/
/*! \brief         Initializes RAM for ECU Detection
*  \details       This function is supposed to be called by the NvM in order to (re)initialize the data in case the
*                 non-volatile memory has never been stored, or was corrupted.
*                 (See NvMBlockDescriptor/NvMInitBlockCallback)
*                 It can also be used to force a reinitialization of the ECU detection data triggered by the
*                 application (e.g. after a new software version has been flashed to the ECU). In the latter case,
*                 make sure the function is not called while the Drm is active.
*  \return        E_OK:       always returned
*  \return        E_NOT_OK:   only in case of DET
*  \reentrant     FALSE
*  \synchronous   TRUE
*  \pre           -
*  \context       TASK|ISR2
*********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_NvM_InitEcuDetectionData(void);
# endif

# if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_GetEcuDetectionResult()
 *********************************************************************************************************************/
/*! \brief         Returns the result of ECU Detection.
 *  \details       This API provides the application the result of the ECU detection.
 *  \param[in]     connectionId       that is related to a ECU, which state will be returned
 *  \return        CDDDRM_ECUD_CONNECTION_AVAILABLE:      ECU is available
 *  \return        CDDDRM_ECUD_CONNECTION_NOT_AVAILABLE:  ECU is not available
 *  \return        CDDDRM_ECUD_CONNECTION_NOT_DISCOVERED: ECU Detection is not performed to given ConnectionId yet
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \pre           -
 *  \context       TASK|ISR2
 *********************************************************************************************************************/
FUNC(CddDrm_EcudStateType, CDDDRM_CODE) CddDrm_GetEcuDetectionResult(CddDrm_ConnectionIdType connectionId);
# endif

# if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_StartEcuDetection()
 *********************************************************************************************************************/
/*! \brief         Starts the background ECU Detection.
 *  \details       Triggers the ECU detection which is processed in context of the mainfunction.
 *  \return        E_OK:      ECU Detection has been started
 *  \return        E_NOT_OK:  ECU Detection already running or external tester active
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \pre           -
 *  \context       TASK|ISR2
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_StartEcuDetection(void);
# endif

# if (CDDDRM_CFG_SUPPORT_ECU_DETECTION == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_StopEcuDetection()
 *********************************************************************************************************************/
/*! \brief         Stops the background ECU Detection.
 *  \details       Forces the end of the ECU detection. All connections which are not processed yet will be in
 *                 state not discovered.
 *  \return        E_OK:      always returned
 *  \return        E_NOT_OK:  never returned
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \pre           -
 *  \context       TASK|ISR2
 *********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_StopEcuDetection(void);
# endif

# if (CDDDRM_CFG_SUPPORT_FUNCTIONAL_REQ == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_GetFuncRequestResult()
*********************************************************************************************************************/
/*! \brief          Returns the result of a functional request.
 *  \details        This API provides the connection specific result to a functional request.
 *  \param[in]      connectionId                             Connection to get the result
 *  \param[in,out]  respInfoStruct                           IN: Response buffer, Response buffer size. 
                                                                 The response buffer shall always be valid and the length of buffer is minimium the given response buffer size.
 *                                                           OUT: Responsecode
 *  \return         CDDDRM_FUNC_REQ_RESULT_OK:               Operation succeeded
 *  \return         CDDDRM_FUNC_REQ_RESULT_NOT_OK:           Operation failed
 *  \return         CDDDRM_FUNC_REQ_RESULT_BUFFER_TOO_SMALL: Buffer too small for result
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            -
 *  \context        TASK|ISR2
*********************************************************************************************************************/
FUNC(CddDrm_FuncReqResultStateType, CDDDRM_CODE) CddDrm_GetFuncRequestResult(
                                CddDrm_ConnectionIdType connectionId,
                                P2VAR(CddDrm_FuncReqRespInfoStructType, AUTOMATIC, CDDDRM_APPL_DATA) respInfoStruct);
# endif

# if (CDDDRM_CFG_SUPPORT_INTERNAL_ECU_COMMUNICATION == STD_ON)
/**********************************************************************************************************************
CddDrm_Transmit
**********************************************************************************************************************/
/*! \brief        Requests transmission of a PDU
*  \details       This function triggers a transmission of a PDU.
*  \param[in]     txPduId      id of the CddPduRLowerLayerTxPdu.
*  \param[in]     pduInfoPtr   Length and pointer to the PDU data.
*  \return        E_OK:        the transmission has successfully started.
*  \return        E_NOT_OK:    the transmission has not successfully started.
*  \pre           -
*  \context       TASK|ISR
*  \reentrant     TRUE
*  \synchronous   TRUE
*  \note          The function is called by the PduR.
**********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_Transmit(PduIdType txPduId,
                                                  P2CONST(PduInfoType, AUTOMATIC, CDDDRM_APPL_DATA) pduInfoPtr);


#  if (CDDDRM_CFG_SUPPORT_CANCELTRANSMIT == STD_ON)
/**********************************************************************************************************************
CddDrm_CancelTransmit
**********************************************************************************************************************/
/*! \brief        Cancel a transmission of a TX I-PDU
*  \details       This function cancels an active Tx Pdu transmission
*  \param[in]     txPduId   id of the CddPduRLowerLayerTxPdu.
*  \return        E_OK:        the transmission has successfull been cancelled.
*  \return        E_NOT_OK:    the transmission has not successful been cancelled.
*  \pre           -
*  \context       TASK|ISR
*  \reentrant     TRUE
*  \synchronous   TRUE
*  \note          The function is called by the PduR.
**********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelTransmit(PduIdType txPduId);
#  endif

#  if (CDDDRM_CFG_SUPPORT_CANCELRECEIVE == STD_ON)
/**********************************************************************************************************************
CddDrm_CancelReceive
**********************************************************************************************************************/
/*! \brief        Cancels a reception of an ongiong RX I-PDU
*  \details       This function cancels an active Rx Pdu reception
*  \param[in]     rxPduId       id of the CddPduRLowerLayerRxPdu.
*  \return        E_OK:         cancellation sucessful.
*  \return        E_NOT_OK:     cancellation failed.
*  \pre           -
*  \context       TASK|ISR
*  \reentrant     TRUE
*  \synchronous   TRUE
*  \note          The function is called by the PduR.
**********************************************************************************************************************/
FUNC(Std_ReturnType, CDDDRM_CODE) CddDrm_CancelReceive(PduIdType rxPduId);
#  endif

/**********************************************************************************************************************
*  CddDrm_MainFunctionLowerLayer()
*********************************************************************************************************************/
/*! \brief        Processes active Tx Pdu transmissions
*  \details       This function handles active transmissions
*  \pre           -
*  \context       TASK
*  \reentrant     FALSE
*  \synchronous   TRUE
*********************************************************************************************************************/
FUNC(void, CDDDRM_CODE) CddDrm_MainFunctionLowerLayer(void);

/**********************************************************************************************************************
*  CddDrm_InitLowerLayer()
*********************************************************************************************************************/
/*! \brief        Initialization function
*  \details       This function initializes related variables used in CddDrm_LowerLayer sub-module.
*  \pre           -
*  \context       TASK
*  \reentrant     FALSE
*  \synchronous   TRUE
*********************************************************************************************************************/
FUNC(void, CDDDRM_CODE) CddDrm_InitLowerLayer(void);

/**********************************************************************************************************************
*  CddDrm_InitLowerLayerMemory()
*********************************************************************************************************************/
/*! \brief        Initialization for memory
*  \details       Service to initialize module global variables at power up. This function initializes the
*                 variables in *_INIT_* sections for Lower Layer. Used in case they are not initialized by the startup code.
*  \pre           -
*  \context       TASK
*  \reentrant     FALSE
*  \synchronous   TRUE
*********************************************************************************************************************/
FUNC(void, CDDDRM_CODE) CddDrm_InitLowerLayerMemory(void);
# endif /* CDDDRM_CFG_SUPPORT_INTERNAL_ECU_COMMUNICATION */

# if (CDDDRM_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
 *  CddDrm_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief         Returns the version information
 *  \details       CddDrm_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]    versionInfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, CDDDRM_CODE) CddDrm_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, CDDDRM_APPL_DATA) versionInfo);
# endif

# define CDDDRM_STOP_SEC_CODE
# include "CddDrm_MemMap.h"                                                                                                                                 /* PRQA S 5087 */  /* MD_MSR_MemMap */

/*!
 * \exclusivearea CDDDRM_EXCLUSIVE_AREA_0
 *                Ensures consistency of the Drm global state variables.

 * \protects      CddDrm_ChannelInfo[].channelState,
 *                CddDrm_ChannelAssign[],
 *                CddDrm_ActiveChannels,
 *                CddDrm_State,
 *                CddDrm_Closing_Semaphore[]
 *
 * \usedin        CddDrm_MainFunction,
 *                CddDrm_SvcSend_10,
 *                CddDrm_SvcSend_11,
 *                CddDrm_SvcSend_1902,
 *                CddDrm_SvcSend_1904,
 *                CddDrm_SvcSend_22,
 *                CddDrm_SvcSend_27,
 *                CddDrm_SvcSend_28,
 *                CddDrm_SvcSend_31,
 *                CddDrm_SvcSend_34,
 *                CddDrm_SvcSend_36,
 *                CddDrm_SvcSend_37,
 *                CddDrm_SvcSend_3E,
 *                CddDrm_SvcSend_85,
 *                CddDrm_SvcSend,
 *                CddDrm_CancelRequest,
 *                CddDrm_CopyTxData,
 *                CddDrm_TpTxConfirmation,
 *                CddDrm_StartOfReception,
 *                CddDrm_CopyRxData,
 *                CddDrm_TpRxIndication,
 *                CddDrm_FuncReqMgrInitResponseChannel
 *
 * \exclude       CddDrm_MainFunction,
 *                CddDrm_SvcSend_10,
 *                CddDrm_SvcSend_11,
 *                CddDrm_SvcSend_1902,
 *                CddDrm_SvcSend_1904,
 *                CddDrm_SvcSend_22,
 *                CddDrm_SvcSend_27,
 *                CddDrm_SvcSend_28,
 *                CddDrm_SvcSend_31,
 *                CddDrm_SvcSend_34,
 *                CddDrm_SvcSend_36,
 *                CddDrm_SvcSend_37,
 *                CddDrm_SvcSend_3E,
 *                CddDrm_SvcSend_85,
 *                CddDrm_SvcSend,
 *                CddDrm_CancelRequest,
 *                CddDrm_CopyTxData,
 *                CddDrm_TpTxConfirmation,
 *                CddDrm_StartOfReception,
 *                CddDrm_CopyRxData,
 *                CddDrm_TpRxIndication
 *
 * \length        MEDIUM At max it iterates over the Channel array and sets the states.
 * \endexclusivearea
 */
#endif /* CDDDRM_H */

/**********************************************************************************************************************
 *  END OF FILE: CddDrm.h
 *********************************************************************************************************************/
