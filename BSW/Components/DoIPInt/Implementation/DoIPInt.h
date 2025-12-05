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
/*!        \file  DoIPInt.h
 *        \brief  DoIPInt header file
 *      \details  Declarations for component DoIPInt.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2019-07-02  vismda  -             Initial creation
 *  02.00.00  2019-10-16  viskjs  SAA-14        Added Tx Queue and Rx Retry feature
 *            2019-12-13  vismda  SAA-8         Implement message structure for DoIP light
 *            2020-02-05  visjsb  SAA-359       Re-structuring of DoIPInt configuration data to enable calibration
 *  02.00.01  2020-06-17  viskjs  ESCAN00106523 No reception on valid channel
 *  02.00.02  2020-10-22  visjsb  SAA-1255      [DoIPInt] Update version history to current template
 *            2020-10-22  visjsb  ESCAN00107565 TCP connection is closed unexpectedly
 *            2020-10-26  visjsb  SAA-857       [DoIPInt] Update to QAC Helix
 *  02.00.03  2021-05-11  vismda  ESCAN00109186 TCP connection is closed unexpectedly after reception failed
 *            2021-05-17  vismda  SAA-2031      Enable transmission in context of TxConfirmation
 *  03.00.00  2021-11-22  viseje  SAA-2560      Support UUDT
 *  03.00.01  2021-12-13  viseje  ESCAN00110981 Compiler warning: Unreferenced formal parameter 'ConnectionIdx'
 *  03.01.00  2022-02-17  viseje  SAA-2310      Usage of <Mip>_MemMap in DoIPInt
 *            2022-02-17  viseje  ESCAN00111120 DoIPInt_Tx_IfTransmit() uses invalid compiler abstraction macro
 *  04.00.00  2022-03-16  viseje  SAA-2335      Provide API to receive and transmit OEM specific payload types
 *            2022-04-26  viseje  SAA-2342      Notify the user about reception events
 *  04.01.00  2022-06-23  viseje  SAA-3130      Rework start/stop parse strategy
 *            2022-06-27  viseje  SAA-3141      Retrieve the payload type from Rx buffer
 *            2022-06-28  viseje  SAA-3135      Set reception retry in Rx by message handlers
 *            2022-06-30  viseje  ESCAN00112165 A connection is unexpectedly closed or unexpected events are reported
 *            2022-07-04  vismda  SAA-2013      Perform Silent Analysis
 *            2022-07-07  viseje  ESCAN00112281 DoIPInt cancels IF transmission on wrong channel
 *  04.02.00  2022-08-02  viseje  SAA-3146      Improve payload type handling in TxQueue
 *            2022-08-04  viseje  ESCAN00112511 Tx confirmation is forwarded on wrong channel on transmission
 *                                              cancellation
 *  05.00.00  2022-11-28  viseje  ESCAN00113057 Compiler errors caused by encoding
 *            2022-11-28  viseje  SAA-3183      Rework query of available data for transmission
 *            2022-12-01  viseje  SAA-3181      Improve consistency for retry timer
 *            2022-12-07  viseje  ESCAN00113384 Diagnostic message sent via IF-API/UUDT contains unexpected data
 *  06.00.00  2023-05-08  visgyv  SAA-3105      Provide channel ready for transmission information for Dcm
 *            2023-05-31  visgyv  SAA-3193      Allow to transmit OEM specific payload types without payload
 *  06.01.00  2023-07-10  visgyv  SAA-3579      Make DoIP Protocol Version configurable
 *  07.00.00  2023-12-13  viseje  SAA-4434      Update to VCA7
 *********************************************************************************************************************/

#if !defined (DOIPINT_H)
# define DOIPINT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "DoIPInt_Lcfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ----- Vendor and module identification ----- */
# define DOIPINT_VENDOR_ID                      (30u)
# define DOIPINT_MODULE_ID                      (255u) /* Module ID of CDD since DoIPInt is no AUTOSAR module. */

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define DOIPINT_SW_MAJOR_VERSION               (7u)
# define DOIPINT_SW_MINOR_VERSION               (0u)
# define DOIPINT_SW_PATCH_VERSION               (0u)

# define DOIPINT_INSTANCE_ID_DET                (0x00u)

/* ----- API service IDs ----- */
# define DOIPINT_SID_INIT                       (0x00u) /*!< Service ID of DoIPInt_Init(). */
# define DOIPINT_SID_MAIN_FUNCTION              (0x01u) /*!< Service ID of DoIPInt_MainFunction(). */
# define DOIPINT_SID_TP_TRANSMIT                (0x02u) /*!< Service ID of DoIPInt_TpTransmit(). */
# define DOIPINT_SID_TP_CANCEL_TRANSMIT         (0x03u) /*!< Service ID of DoIPInt_TpCancelTransmit(). */
# define DOIPINT_SID_TP_CANCEL_RECEIVE          (0x04u) /*!< Service ID of DoIPInt_TpCancelReceive(). */
# define DOIPINT_SID_GET_VERSION_INFO           (0x05u) /*!< Service ID of DoIPInt_GetVersionInfo(). */
# define DOIPINT_SID_SO_CON_MODE_CHG            (0x06u) /*!< Service ID of DoIPInt_SoConModeChg(). */
# define DOIPINT_SID_TP_COPY_TX_DATA            (0x07u) /*!< Service ID of DoIPInt_SoAdTpCopyTxData(). */
# define DOIPINT_SID_TP_TX_CONFIRMATION         (0x08u) /*!< Service ID of DoIPInt_SoAdTpTxConfirmation(). */
# define DOIPINT_SID_TP_START_OF_RECEPTION      (0x09u) /*!< Service ID of DoIPInt_SoAdTpStartOfReception(). */
# define DOIPINT_SID_TP_COPY_RX_DATA            (0x0Au) /*!< Service ID of DoIPInt_SoAdTpCopyRxData(). */
# define DOIPINT_SID_TP_RX_INDICATION           (0x0Bu) /*!< Service ID of DoIPInt_SoAdTpRxIndication(). */
# define DOIPINT_SID_OVERWRITE_LOGICAL_ADDR     (0x0Cu) /*!< Service ID of DoIPInt_OverwriteLogicalAddresses(). */
# define DOIPINT_SID_IF_TRANSMIT                (0x0Du) /*!< Service ID of DoIPInt_IfTransmit(). */
# define DOIPINT_SID_IF_CANCEL_TRANSMIT         (0x0Eu) /*!< Service ID of DoIPInt_IfCancelTransmit(). */
# define DOIPINT_SID_OEM_PAYLOAD_TRANSMIT       (0x0Fu) /*!< Service ID of DoIPInt_TransmitOemSpecificPayloadType(). */
# define DOIPINT_SID_CLOSE_CONNECTION           (0x10u) /*!< Service ID of DoIPInt_CloseConnection(). */

/* ----- Error codes ----- */
# define DOIPINT_E_NO_ERROR                     (0x00u) /*!< Error code indicates no error. */
# define DOIPINT_E_PARAM_CONFIG                 (0x01u) /*!< Error code for call with invalid config pointer. */
# define DOIPINT_E_UNINIT                       (0x02u) /*!< Error code for call while module is uninitialized. */
# define DOIPINT_E_PARAM                        (0x03u) /*!< Error code for call with invalid parameter. */
# define DOIPINT_E_PARAM_POINTER                (0x04u) /*!< Error code for call with invalid pointer. */

/* ----- Modes ----- */
# define DOIPINT_STATE_UNINIT                   (0x00u)
# define DOIPINT_STATE_INIT                     (0x01u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
# define DOIPINT_START_SEC_VAR_CLEARED_UNSPECIFIED
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*!
 * \spec strong invariant DoIPInt_ConfigDataPtr == &DoIPInt_PCConfig.Config;
 * \endspec
 */
/*! Pointer to the current configuration. */
extern P2CONST(DoIPInt_ConfigType, DOIPINT_VAR_CLEARED, DOIPINT_PBCFG) DoIPInt_ConfigDataPtr;

# define DOIPINT_STOP_SEC_VAR_CLEARED_UNSPECIFIED
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define DOIPINT_START_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_InitMemory()
 *********************************************************************************************************************/
/*! \brief        Initializes *_INIT_*-variables.
 *  \details      Service to initialize module global variables at power up. This function initializes the
 *                variables in *_INIT_* sections. Used in case they are not initialized by the startup code.
 *  \pre          Interrupts are disabled and module main function is not triggered.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_InitMemory(void);

/**********************************************************************************************************************
 * DoIPInt_Init()
 *********************************************************************************************************************/
/*! \brief        Initializes component.
 *  \details      Service to initialize the module DoIPInt. It initializes all variables and sets the module state to
 *                initialized.
 *  \param[in]    ConfigPtr             Configuration structure for initializing the module.
 *  \pre          Interrupts are disabled and module main function is not triggered.
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \spec
 *    requires ConfigPtr == &DoIPInt_PCConfig.Config;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_Init(
  P2CONST(DoIPInt_ConfigType, AUTOMATIC, DOIPINT_PBCFG) ConfigPtr);

/**********************************************************************************************************************
 *  DoIPInt_MainFunction()
 *********************************************************************************************************************/
/*! \fn          void DoIPInt_MainFunction(void)
 *  \brief       Schedules the DoIPInt (Entry point for scheduling) and handles connection states.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
/* Function declaration is provided by SchM. */

/**********************************************************************************************************************
 *  DoIPInt_TpTransmit()
 *********************************************************************************************************************/
/*! \brief        Requests transmission of a TP-PDU as diagnostic message.
 *  \details      -
 *  \param[in]    DoIPIntPduRTxId         DoIPInt unique identifier of the PDU to be transmitted by the PduR.
 *  \param[in]    DoIPIntPduRTxInfoPtr    PDU information structure which contains the length of the message.
 *  \return       E_OK                    Transmit request was accepted.
 *  \return       E_NOT_OK                Transmit request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same DoIPIntPduRTxId, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TpTransmit(
  PduIdType DoIPIntPduRTxId,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) DoIPIntPduRTxInfoPtr);

/**********************************************************************************************************************
 *  DoIPInt_TpCancelTransmit()
 *********************************************************************************************************************/
/*! \brief        Requests transmission cancellation of a TP-PDU.
 *  \details      -
 *  \param[in]    DoIPIntPduRTxId         DoIPInt unique identifier of the PDU to be transmitted by the PduR.
 *  \return       E_OK                    Request was accepted.
 *  \return       E_NOT_OK                Request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same DoIPIntPduRTxId, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TpCancelTransmit(
  PduIdType DoIPIntPduRTxId);

/**********************************************************************************************************************
 *  DoIPInt_TpCancelReceive()
 *********************************************************************************************************************/
/*! \brief        Requests reception cancellation of a TP-PDU.
 *  \details      -
 *  \param[in]    DoIPIntPduRRxId         DoIPInt unique identifier of the PDU to be received by the PduR.
 *  \return       E_OK                    Request was accepted.
 *  \return       E_NOT_OK                Request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same DoIPIntPduRRxId, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TpCancelReceive(
  PduIdType DoIPIntPduRRxId);

/**********************************************************************************************************************
 *  DoIPInt_IfTransmit()
 *********************************************************************************************************************/
/*! \brief        Requests transmission of an IF-PDU as diagnostic message.
 *  \details      -
 *  \param[in]    DoIPIntPduRTxId         DoIPInt unique identifier of the PDU to be transmitted by the PduR.
 *  \param[in]    DoIPIntPduRTxInfoPtr    PDU information structure which contains the length and the data of the
 *                                        message.
 *  \return       E_OK                    Transmit request was accepted.
 *  \return       E_NOT_OK                Transmit request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same DoIPIntPduRTxId, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_IfTransmit(
  PduIdType DoIPIntPduRTxId,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) DoIPIntPduRTxInfoPtr);

/**********************************************************************************************************************
 *  DoIPInt_IfCancelTransmit()
 *********************************************************************************************************************/
/*! \brief        Requests transmission cancellation of an IF-PDU.
 *  \details      -
 *  \param[in]    DoIPIntPduRTxId         DoIPInt unique identifier of the PDU to be transmitted by the PduR.
 *  \return       E_OK                    Request was accepted.
 *  \return       E_NOT_OK                Request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same DoIPIntPduRTxId, TRUE otherwise
 *  \synchronous  FALSE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_IfCancelTransmit(
  PduIdType DoIPIntPduRTxId);

/**********************************************************************************************************************
 *  DoIPInt_TransmitOemSpecificPayloadType()
 *********************************************************************************************************************/
/*! \brief        Requests transmission of a message with OEM specific payload type.
 *  \details      -
 *  \param[in]    ConnectionId            DoIPInt unique identifier of the connection.
 *  \param[in]    PayloadType             The payload type that shall be used for the message.
 *  \param[in]    PayloadDataPtr          PDU information structure which contains the length and the payload data of
 *                                        the message.
 *  \return       E_OK                    Transmit request was accepted.
 *  \return       E_NOT_OK                Transmit request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same ConnectionId, TRUE otherwise
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_TransmitOemSpecificPayloadType(
  DoIPInt_ConnectionIdType ConnectionId,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PayloadDataPtr);

# if ( DOIPINT_VERSION_INFO_API == STD_ON )
/**********************************************************************************************************************
 *  DoIPInt_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief        Returns the version information.
 *  \details      DoIPInt_GetVersionInfo() returns version information, vendor ID and module ID of the component.
 *  \param[out]   Versioninfo             Pointer to where to store the version information.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       DOIPINT_VERSION_INFO_API
 *  \trace        CREQ-204782
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DOIPINT_VAR) Versioninfo);
# endif /* DOIPINT_VERSION_INFO_API == STD_ON */

/**********************************************************************************************************************
 *  DoIPInt_OverwriteLogicalAddresses()
 *********************************************************************************************************************/
/*! \brief        Overwrites logical addresses until reinitialization.
 *  \details      -
 *  \param[in]    ChannelId               DoIPInt unique identifier of the channel.
 *  \param[in]    LogicalLocalAddress     Local address of channel.
 *  \param[in]    LogicalRemoteAddress    Remote address of channel.
 *  \return       E_OK                    Request was accepted.
 *  \return       E_NOT_OK                Request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same ChannelId, TRUE otherwise
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_OverwriteLogicalAddresses(
  DoIPInt_ChannelIdType ChannelId,
  uint16 LogicalLocalAddress,
  uint16 LogicalRemoteAddress);

/**********************************************************************************************************************
 *  DoIPInt_CloseConnection()
 *********************************************************************************************************************/
/*! \brief        Closes the requested connection.
 *  \details      -
 *  \param[in]    ConnectionId            DoIPInt unique identifier of the connection.
 *  \param[in]    Abort                   Flag to close connection immediately.
 *                                        [range: TRUE close immediately, FALSE close when Tx Queue is empty]
 *  \return       E_OK                    Request was accepted.
 *  \return       E_NOT_OK                Request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same ConnectionId, TRUE otherwise
 *  \synchronous  TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_CloseConnection(
  DoIPInt_ConnectionIdType ConnectionId,
  boolean Abort);

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  EXCLUSIVE AREA DEFINITION
 *********************************************************************************************************************/
/*!
 * \exclusivearea DOIPINT_EXCLUSIVE_AREA_0
 * Protects the transmission state and the transmission queue against concurrent modifications
 * in case of reentrant calls.
 * \protects TxStateOfConnectionDyn, TxMsgLenOfChannelDyn, TxQueue* members of ConnectionDyn, TxQueue,
 * OemPayloadTxBufConfigDyn, ModeOfConnectionDyn, RequestModeOfConnectionDyn, ClosePendingOfConnectionDyn,
 * RxPendingOfConnectionDyn
 * \usedin DoIPInt_IfTransmit, DoIPInt_TpTransmit, DoIPInt_TransmitOemSpecificPayloadType, DoIPInt_MainFunction,
 * DoIPInt_IfCancelTransmit, DoIPInt_TpCancelTransmit, DoIPInt_SoAdTpTxConfirmation, DoIPInt_SoConModeChg,
 * DoIPInt_CloseConnection, DoIPInt_SoAdTpCopyRxData
 * \exclude All functions provided by DoIPInt.
 * \length MEDIUM Multiple read-modify-write operations and copy operations for state and queue management
 * \endexclusivearea
 */

#endif /* DOIPINT_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt.h
 *********************************************************************************************************************/
