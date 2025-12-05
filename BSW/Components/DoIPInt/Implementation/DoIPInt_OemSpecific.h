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
/*!        \file  DoIPInt_OemSpecific.h
 *        \brief  Declarations for sub-module DoIPInt_OemSpecific of component DoIPInt.
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

#if !defined (DOIPINT_OEMSPECIFIC_H)
# define DOIPINT_OEMSPECIFIC_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "DoIPInt_Types.h"

# if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#  define DOIPINT_START_SEC_CODE
#  include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_Init()
 *********************************************************************************************************************/
/*! \brief        Initializes connection related OEM specific variables used in DoIPInt_OemSpecific sub-module.
 *  \details      -
 *  \param[in]    ConnectionIter          Connection iterator.
 *                                        [range: ConnectionIter < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \trace        DSGN-DoIPIntInitialization
 *  \spec
 *    requires ConnectionIter < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_Init(
  DoIPInt_ConnectionIterType ConnectionIter);

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_TxTransmit()
 *********************************************************************************************************************/
/*! \brief        Requests transmission of a message with OEM specific payload type on a specific connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    PayloadType             The payload type that shall be used for the message.
 *  \param[in]    PayloadDataPtr          PDU information structure which contains the length and the data of the
 *                                        message.
 *                                        [range: PayloadDataPtr != NULL_PTR]
 *  \return       E_OK                    Transmit request was accepted.
 *  \return       E_NOT_OK                Transmit request was not accepted.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same ChannelIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PayloadDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_OemSpecific_TxTransmit(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PayloadDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_TxCopy()
 *********************************************************************************************************************/
/*! \brief        Copies the OEM specific payload transmission buffer to the destination buffer.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    OemTxBufCfgIdx          OEM Tx Buffer Configuration index.
 *                                        [range: OemTxBufCfgIdx < DoIPInt_GetSizeOfOemPayloadTxBufConfig()]
 *  \param[in]    PduInfoPtr              Provides the destination buffer and the number of bytes to be copied.
 *                                        [range: PduInfoPtr != NULL_PTR,
 *                                                PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]   AvailableDataPtr        Indicates the remaining number of bytes that are available to be copied.
 *                                        [range: AvailableDataPtr != NULL_PTR]
 *  \return       BUFREQ_OK               Data has been copied to the transmit buffer completely as requested.
 *  \return       BUFREQ_E_NOT_OK         Data has not been copied. Request failed.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires OemTxBufCfgIdx < DoIPInt_GetSizeOfOemPayloadTxBufConfig();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_OemSpecific_TxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Triggers release of a previously reserved OEM transmission buffer on Tx confirmation.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    OemTxBufCfgIdx          OEM Tx Buffer Configuration index.
 *                                        [range: OemTxBufCfgIdx < DoIPInt_GetSizeOfOemPayloadTxBufConfig()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires OemTxBufCfgIdx < DoIPInt_GetSizeOfOemPayloadTxBufConfig();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_TxConfirmation(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx);

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxCopy()
 *********************************************************************************************************************/
/*! \brief       Copies user data and validates it.
 *  \details     -
 *  \param[in]   ConnectionIdx                Connection index.
 *                                            [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]   PayloadLength                Payload length of the current PDU.
 *  \param[in]   PayloadType                  Payload type of the current PDU.
 *  \param[in]   PduInfoPtr                   Provides the buffer and the number of bytes to be copied.
 *                                            [range: PduInfoPtr != NULL_PTR,
 *                                                    PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                    PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr == NULL_PTR]
 *  \param[out]  AvailableDataPtr             Indicates the remaining number of bytes that are available to be copied.
 *                                            [range: AvailableDataPtr != NULL_PTR]
 *  \return      DOIPINT_MSG_HANDLER_PENDING  The message handler has not yet finished processing the message.
 *  \return      DOIPINT_MSG_HANDLER_FINISHED The message handler has finished processing the message.
 *  \return      DOIPINT_MSG_HANDLER_FAILED   Handling the message failed.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous FALSE
 *  \config      DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_OemSpecific_RxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 PayloadLength,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxGetParsedData()
 *********************************************************************************************************************/
/*! \brief         Gets the parsed OEM specific data of the connection.
 *  \details       -
 *  \param[in]     ConnectionIdx           Connection index.
 *                                         [range: ConnectionIdx < DoIPInt_GetSizeOfConnection(),
 *                                                 DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx)]
 *  \param[in]     DestBufLength           Length of the DestBufPtr.
 *  \param[out]    DestBufPtr              Pointer to the buffer that shall be written.
 *                                         [range: DestBufPtr != NULL_PTR
 *                                                 lengthOf(DestBufPtr) >= DestBufLength]
 *  \param[in,out] MsgLengthPtr            Pointer to the length that shall be updated.
 *                                         [range: MsgLengthPtr != NULL_PTR]
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx);
 *    requires DestBufPtr != NULL_PTR;
 *    requires MsgLengthPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_RxGetParsedData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  PduLengthType DestBufLength,
  P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) DestBufPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) MsgLengthPtr);

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxStartParse()
 *********************************************************************************************************************/
/*! \brief        Starts parsing on the specified connection by stopping a current active parsing.
 *  \details      -
 *  \param[in]    ConnectionIdx         Connection index.
 *                                      [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_RxStartParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxStopParse()
 *********************************************************************************************************************/
/*! \brief        Stops and initializes parsing on the specified connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_RxStopParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

#  define DOIPINT_STOP_SEC_CODE
#  include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
#endif /* DOIPINT_OEMSPECIFIC_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_OemSpecific.h
 *********************************************************************************************************************/
