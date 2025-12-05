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
/*!        \file  DoIPInt_GenHdr.h
 *        \brief  Declarations for sub-module DoIPInt_GenHdr of component DoIPInt.
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

#if !defined (DOIPINT_GENHDR_H)
# define DOIPINT_GENHDR_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "DoIPInt_Types.h"

# if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#  define DOIPINT_GEN_HDR_LEN                           (8u)       /*!< Length of generic header. */
#  define DOIPINT_PAYLOAD_TYPE_DIAG_MSG                 (0x8001u)  /*!< Payload type of DoIP diagnostic message. */

/*!< Length of logical addresses in the diagnostic header. */
#  define DOIPINT_HDR_LOG_ADDR_LEN_DIAG                 (4u)

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
 *  DoIPInt_GenHdr_Init()
 *********************************************************************************************************************/
/*! \brief        Initializes connection related variables used in DoIPInt_GenHdr sub-module.
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
FUNC(void, DOIPINT_CODE) DoIPInt_GenHdr_Init(
  DoIPInt_ConnectionIterType ConnectionIter);

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_TxCopy()
 *********************************************************************************************************************/
/*! \brief         Copies header to provided buffer.
 *  \details       -
 *  \param[in]     ConnectionIdx                Connection index.
 *                                              [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]     PayloadType                  Payload type used for the transmission.
 *  \param[in]     UserDataLength               Length of user data of the current transmission.
 *  \param[in,out] PduInfoPtr                   Provides the destination buffer and the number of bytes to be copied.
 *                                              Is updated on return with the offset of copied data.
 *                                              [range: PduInfoPtr != NULL_PTR,
 *                                                      PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                        PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr ==
 *                                                        NULL_PTR]
 *  \param[out]    AvailableDataPtr             Indicates the remaining number of generic header bytes that must be
 *                                              copied.
 *                                              [range: AvailableDataPtr != NULL_PTR]
 *  \return        DOIPINT_MSG_HANDLER_PENDING  Message handler has not yet finished handling the message.
 *  \return        DOIPINT_MSG_HANDLER_FINISHED Message handler has finished handling the message.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous   FALSE
 *  \config        DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_GenHdr_TxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint16 PayloadType,
  uint32 UserDataLength,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_TxConfirmation()
 *********************************************************************************************************************/
/*! \brief        Resets generic header information on Tx confirmation.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_GenHdr_TxConfirmation(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxCopy()
 *********************************************************************************************************************/
/*! \brief        Copies received header and validates it.
 *  \details      -
 *  \param[in]    ConnectionIdx                Connection index.
 *                                             [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    PduInfoPtr                   Provides the buffer and the number of bytes to be copied.
 *                                             [range: PduInfoPtr != NULL_PTR,
 *                                                     PduInfoPtr->SduLength == lengthOf(PduInfoPtr->SduDataPtr) ||
 *                                                       PduInfoPtr->SduLength == 0 => PduInfoPtr->SduDataPtr ==
 *                                                       NULL_PTR]
 *  \param[out]   AvailableDataPtr             Indicates the remaining number of bytes that are available to be copied.
 *                                             [range: AvailableDataPtr != NULL_PTR]
 *  \return       DOIPINT_MSG_HANDLER_PENDING  Message handler has not yet finished handling the message.
 *  \return       DOIPINT_MSG_HANDLER_FINISHED Message handler has finished handling the message.
 *  \return       DOIPINT_MSG_HANDLER_SKIP     Message shall be skipped.
 *  \return       DOIPINT_MSG_HANDLER_FAILED   Message handling failed.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_GenHdr_RxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxStartParse()
 *********************************************************************************************************************/
/*! \brief        Starts parsing on the specified connection by stopping a current active parsing.
 *  \details      -
 *  \param[in]    ConnectionIdx         Connection index.
 *                                      [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[out]   AvailableDataPtr      Indicates the number of bytes that can be copied in the first call of
 *                                      DoIPInt_SoAdTpCopyRxData on reception of the next PDU.
 *                                      [range: AvailableDataPtr != NULL_PTR]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  FALSE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_GenHdr_RxStartParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxStopParse()
 *********************************************************************************************************************/
/*! \brief        Stops and initializes parsing on the specified connection.
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
FUNC(void, DOIPINT_CODE) DoIPInt_GenHdr_RxStopParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxGetParsedPayloadLength()
 *********************************************************************************************************************/
/*! \brief        Gets the parsed payload length of the generic header buffer of the connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       Parsed payload length.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint32, DOIPINT_CODE) DoIPInt_GenHdr_RxGetParsedPayloadLength(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxGetParsedPayloadType()
 *********************************************************************************************************************/
/*! \brief        Gets the parsed payload type of the generic header buffer of the connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \return       Parsed payload type.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *  \endspec
 *********************************************************************************************************************/
FUNC(uint16, DOIPINT_CODE) DoIPInt_GenHdr_RxGetParsedPayloadType(
  DoIPInt_SizeOfConnectionType ConnectionIdx);

/**********************************************************************************************************************
 *  DoIPInt_GenHdr_RxGetParsedHdr()
 *********************************************************************************************************************/
/*! \brief        Gets the parsed generic header buffer of the connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    DestBufLength           Length of the DestBufPtr.
 *  \param[out]   DestBufPtr              Pointer to the buffer that shall be written.
 *                                        [range: DestBufPtr != NULL_PTR
 *                                                lengthOf(DestBufPtr) >= DOIPINT_GEN_HDR_LEN]
 *  \param[out]   MsgLengthPtr            Pointer to the length that shall be updated.
 *                                        [range: MsgLengthPtr != NULL_PTR]
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires DestBufPtr != NULL_PTR;
 *    requires MsgLengthPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
FUNC(void, DOIPINT_CODE) DoIPInt_GenHdr_RxGetParsedHdr(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  PduLengthType DestBufLength,
  P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) DestBufPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) MsgLengthPtr);

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

#endif /* DOIPINT_GENHDR_H */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_GenHdr.h
 *********************************************************************************************************************/
