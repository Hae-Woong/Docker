/***********************************************************************************************************************
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
/*!        \file  SomeIpTp_Priv.h
 *        \brief  SomeIpTp_Priv header file
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 * 
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 **********************************************************************************************************************/

#if !defined (SOMEIPTP_PRIV_H)
# define SOMEIPTP_PRIV_H

/***********************************************************************************************************************
 * INCLUDES
 **********************************************************************************************************************/
#include "SomeIpTp.h"

/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
#define SOMEIPTP_QUEUE_EMPTY                    0u

/* Offset shall be divisible by 16 bytes */
#define SOMEIPTP_DATA_ALIGNMENT_LENGTH          16u
/* First frame offset val in the header */
#define SOMEIPTP_FF_OFFSET                      0u

/* TP flag in Message Type */
#define SOMEIPTP_TPFLAG                         0x00002000u
/* Reserved field shall be set to zero */
#define SOMEIPTP_OFFSET_FIELD_MASK              0xfffffff0u
/* More Segment Flag */
#define SOMEIPTP_MORE_SEGMENT_FLAG              0x00000001u

/* Byte Offset of each segment in Header */
#define SOMEIPTP_REQUEST_ID0_8BIT_OFFSET              0u
#define SOMEIPTP_REQUEST_ID1_8BIT_OFFSET              1u
#define SOMEIPTP_REQUEST_ID2_8BIT_OFFSET              2u
#define SOMEIPTP_REQUEST_ID3_8BIT_OFFSET              3u
#define SOMEIPTP_PROTOCOL_VERSION_8BIT_OFFSET         4u
#define SOMEIPTP_INTERFACE_VERSION_8BIT_OFFSET        5u
#define SOMEIPTP_MESSAGE_TYPE_8BIT_OFFSET             6u
#define SOMEIPTP_RETURN_CODE_8BIT_OFFSET              7u
#define SOMEIPTP_OFFSET_FIELD0_8BIT_OFFSET            8u
#define SOMEIPTP_OFFSET_FIELD1_8BIT_OFFSET            9u
#define SOMEIPTP_OFFSET_FIELD2_8BIT_OFFSET            10u
#define SOMEIPTP_MORE_SEG_FLAG_8BIT_OFFSET            11u

#define SOMEIPTP_OFFSET_FIELD_MASK_8BIT               0xF0u

#define SOMEIPTP_TPFLAG_8BIT_VAL                      0x20u
#define SOMEIPTP_RETURN_CODE_8BIT_VAL                 0x0u

/* 32 Bit Offset of each segment in Header */
#define SOMEIPTP_REQUEST_ID_32BIT_OFFSET              0u
#define SOMEIPTP_PROTOCOL_VERSION_32BIT_OFFSET        1u
#define SOMEIPTP_OFFSET_FIELD_32BIT_OFFSET            2u

/* Default burst size */
#define SOMEIPTP_DEFAULT_BURST_SIZE                   1u

/* ----- Modes ----- */
# define SOMEIPTP_UNINIT                              (0x00u)
# define SOMEIPTP_INIT                                (0x01u)

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/
#define SOMEIPTP_START_SEC_VAR_CLEARED_8
#include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Initialization state of the module */
extern VAR(uint8, SOMEIPTP_VAR_CLEARED) SomeIpTp_ModuleInitialized;

#define SOMEIPTP_STOP_SEC_VAR_CLEARED_8
#include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define SOMEIPTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to the current configuration */
extern P2CONST(SomeIpTp_ConfigType, SOMEIPTP_VAR_CLEARED, SOMEIPTP_PBCFG) SomeIpTp_ConfigDataPtr;

#if (SOMEIPTP_TXNSDU == STD_ON)
/* ! Current Queue size */
extern VAR(SomeIpTp_TxQueueElementType, SOMEIPTP_VAR_CLEARED) SomeIpTp_TxQueueSize;
#endif

#define SOMEIPTP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * SomeIpTp_IsInitialized()
 ***********************************************************************************************************************/
/*! \brief        Returns TRUE if module is initialized.
 *  \details      -
 *  \return       TRUE   Module Initialized.
 *  \return       FALSE  Module not initialized.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsInitialized(void);

#if (SOMEIPTP_TXNSDU == STD_ON)
# if (SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
/***********************************************************************************************************************
 * SomeIpTp_IsTxSduIdInvalid()
 ***********************************************************************************************************************/
/*! \brief        Returns TRUE if TxSduId is invalid.
 *  \details      -
 *  \param[in]    TxSduId  Tx SDU/PDU ID.
 *  \return       TRUE   If TxSduId is invalid.
 *  \return       FALSE  If TxSduId is valid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsTxSduIdInvalid(PduIdType TxSduId);
# endif

/***********************************************************************************************************************
 * SomeIpTp_IsLastSegment()
 ***********************************************************************************************************************/
/*! \brief        Returns TRUE if the current segment to be transmitted is last segement.
 *  \details      -
 *  \param[in]    Id  Tx NPDU ID
 *  \param[in]    RemDataLen  Remaining data length to transmit. 
 *  \return       TRUE   Last segment.
 *  \return       FALSE  Not the last segment.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsLastSegment(PduIdType Id, PduLengthType RemDataLen);

/***********************************************************************************************************************
 * SomeIpTp_IsTxQueueEmpty()
 ***********************************************************************************************************************/
/*! \brief        Returns TRUE if the Tx Queue is empty.
 *  \details      -
 *  \return       TRUE   Tx Queue empty.
 *  \return       FALSE  Tx Queue has at least one Tx Sdu.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsTxQueueEmpty(void);

/***********************************************************************************************************************
 * SomeIpTp_IsTxSduLengthValid()
 ***********************************************************************************************************************/
/*! \brief        Validates SDU length during transmission.
 *  \details      Validates that an SDU is at least as long as the SOME/IP header (8 bytes) plus the SDU's metadata.
 *  \param[in]    Id      Tx SDU ID. Shall be less than the size of SomeIpTp_TXNSdu.
 *  \param[in]    Length  SDU length.
 *  \return       TRUE   The length is valid.
 *  \return       FALSE  The length is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsTxSduLengthValid(PduIdType Id, PduLengthType Length);
#endif

#if (SOMEIPTP_RXNSDU == STD_ON)
# if (SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
/***********************************************************************************************************************
 * SomeIpTp_IsRxPduIdInvalid()
 ***********************************************************************************************************************/
/*! \brief        Returns TRUE if the Pdu ID received is invalid.
 *  \details      -
 *  \param[in]    RxPduId  Rx PDU Id
 *  \return       TRUE   RxPduId invalid.
 *  \return       FALSE  RxPduId valid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsRxPduIdInvalid(PduIdType RxPduId);
# endif

/***********************************************************************************************************************
 * SomeIpTp_IsRxPduLengthValid()
 ***********************************************************************************************************************/
/*! \brief        Validates PDU length during reception.
 *  \details      Validates that a PDU is at least as long as the SOME/IP header (8 bytes) plus the PDU's metadata.
 *  \param[in]    Id      Rx PDU ID. Shall be less than the size of SomeIpTp_RXNSdu.
 *  \param[in]    Length  PDU length.
 *  \return       TRUE   The length is valid.
 *  \return       FALSE  The length is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsRxPduLengthValid(PduIdType Id, PduLengthType Length);

/***********************************************************************************************************************
 * SomeIpTp_IsPayloadSixteenByteAligned()
 ***********************************************************************************************************************/
/*! \brief        Returns TRUE if the received payload of the segment is 16 byte aligned.
 *  \details      -
 *  \param[in]    PayloadLen Data length.
 *  \return       TRUE   16 byte aligned.
 *  \return       FALSE  not 16 byte aligned.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDUs.
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsPayloadSixteenByteAligned(PduLengthType PayloadLen);
#endif

/***********************************************************************************************************************
 * SomeIpTp_IsMoreSegFlagSet()
 ***********************************************************************************************************************/
/*! \brief        Returns TRUE if More segment flag of the frame is set.
 *  \details      -
 *  \param[in]    TpSpecificBits  TP header of the frame.
 *  \return       TRUE   More segment flag is set.
 *  \return       FALSE  More segment flag is not set.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsMoreSegFlagSet(uint32 TpSpecificBits);

/***********************************************************************************************************************
 * SomeIpTp_IsSduPtrValid()
 ***********************************************************************************************************************/
/*! \brief        Returns TRUE if SduDataPtr is not null.
 *  \details      -
 *  \param[in]    PduInfoPtr  Data, Length and Metadata passed by upper/lower layer.
 *  \return       TRUE   SduDataPtr is valid.
 *  \return       FALSE  SduDataPtr is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsSduPtrValid(const PduInfoType *PduInfoPtr);

/***********************************************************************************************************************
 * SomeIpTp_Get32BitVal()
 ***********************************************************************************************************************/
/*! \brief        Returns 32 bit value.
 *  \details      -
 *  \param[in]    HhByte High high byte
 *  \param[in]    HlByte High low byte
 *  \param[in]    LhByte Low High byte
 *  \param[in]    LlByte Low low byte
 *  \return       uint32 32bit value
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL_INLINE FUNC(uint32, SOMEIPTP_CODE) SomeIpTp_Get32BitVal(uint8 HhByte, uint8 HlByte, uint8 LhByte,
  uint8 LlByte);

/***********************************************************************************************************************
 *  LOCAL INLINE FUNCTIONS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * SomeIpTp_IsNotInitialized()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsInitialized(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if SomeIpTp is initialized. */
  return (boolean)(SomeIpTp_ModuleInitialized == SOMEIPTP_INIT); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
} /* SomeIpTp_IsNotInitialized */

#if (SOMEIPTP_TXNSDU == STD_ON)
/**********************************************************************************************************************
 * SomeIpTp_IsTxSduLengthValid()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 */
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsTxSduLengthValid(PduIdType Id, PduLengthType Length)
{
  /* #10 If the length is at least as long as the SOME/IP header (8 bytes) plus the metadata's length, return true.
   *     Otherwise, return false. */
# if (SOMEIPTP_TXMETADATALENGTHOFTXNSDU == STD_ON)
  return (boolean)(Length >= (SOMEIPTP_SHORT_HEADER_LENGTH + (PduLengthType)SomeIpTp_GetTxMetadataLengthOfTXNSdu(Id))); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
# else
  SOMEIPTP_DUMMY_STATEMENT(Id); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return (boolean)(Length >= SOMEIPTP_SHORT_HEADER_LENGTH); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
# endif
}

# if (SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 * SomeIpTp_IsTxSduIdValid()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsTxSduIdInvalid(PduIdType TxSduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if passed Tx Sdu is valid. */
  return (boolean)((TxSduId >= SomeIpTp_GetSizeOfTXNSdu()) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
#  if (SOMEIPTP_INVALIDHNDOFTXNSDU == STD_ON)
    || (SomeIpTp_IsInvalidHndOfTXNSdu(TxSduId) == TRUE)   /* PRQA S 1881 */ /* MD_MSR_AutosarBoolean */
#  endif
    );
} /* SomeIpTp_IsTxSduIdValid */
# endif

/**********************************************************************************************************************
 * SomeIpTp_IsLastSegment()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsLastSegment(PduIdType Id, PduLengthType RemDataLen)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if it is the last segment to be transmitted. */
  return (boolean)(RemDataLen <= /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    (SomeIpTp_GetTxNPduLengthOfTXNSdu(Id) - SOMEIPTP_LONG_HEADER_LENGTH));
} /* SomeIpTp_IsLastSegment */

/**********************************************************************************************************************
 * SomeIpTp_IsTxQueueEmpty()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsTxQueueEmpty(void)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if the Tx Queue is empty. */
  return (boolean)(SomeIpTp_TxQueueSize == SOMEIPTP_QUEUE_EMPTY); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
} /* SomeIpTp_IsTxQueueEmpty */
#endif


#if (SOMEIPTP_RXNSDU == STD_ON)
# if (SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
/**********************************************************************************************************************
 * SomeIpTp_IsRxPduIdValid()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsRxPduIdInvalid(PduIdType RxPduId)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if passed Rx Pdu Id is valid. */
  return (boolean)((RxPduId >= SomeIpTp_GetSizeOfRXNSdu()) /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
#  if (SOMEIPTP_INVALIDHNDOFRXNSDU == STD_ON)
    || (SomeIpTp_IsInvalidHndOfRXNSdu(RxPduId) == TRUE)   /* PRQA S 1881 */ /* MD_MSR_AutosarBoolean */
#  endif
    );
} /* SomeIpTp_IsRxPduIdValid */

/**********************************************************************************************************************
 * SomeIpTp_IsRxPduLengthValid()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 */
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsRxPduLengthValid(PduIdType Id, PduLengthType Length)
{
  /* #10 If the length is at least as long as the SOME/IP header (8 bytes) plus the metadata's length, return true.
   *     Otherwise, return false. */
#  if (SOMEIPTP_RXMETADATALENGTHOFRXNSDU == STD_ON)
  return (boolean)(Length >= (SOMEIPTP_SHORT_HEADER_LENGTH + (PduLengthType)SomeIpTp_GetRxMetadataLengthOfRXNSdu(Id))); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
#  else
  SOMEIPTP_DUMMY_STATEMENT(Id); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return (boolean)(Length >= SOMEIPTP_SHORT_HEADER_LENGTH); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
#  endif
}
# endif

/**********************************************************************************************************************
 * SomeIpTp_IsPayloadSixteenByteAligned()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsPayloadSixteenByteAligned(PduLengthType PayloadLen)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if the payload is 16 byte aligned. */
  return (boolean)((PayloadLen % SOMEIPTP_DATA_ALIGNMENT_LENGTH) == 0u); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
} /* SomeIpTp_IsPayloadSixteenByteAligned */

/**********************************************************************************************************************
 * SomeIpTp_IsMoreSegFlagSet()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsMoreSegFlagSet(uint32 TpSpecificBits)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if the more segment flag is set in the TP header.*/
  return (boolean)((TpSpecificBits & (uint32)SOMEIPTP_MORE_SEGMENT_FLAG) == SOMEIPTP_MORE_SEGMENT_FLAG); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
} /* SomeIpTp_IsMoreSegFlagSet */

/**********************************************************************************************************************
 * SomeIpTp_Get32BitVal()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL_INLINE FUNC(uint32, SOMEIPTP_CODE) SomeIpTp_Get32BitVal(uint8 HhByte, uint8 HlByte, uint8 LhByte,
                                uint8 LlByte)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns 32 bit value. */
  return (((uint32)HhByte << 24) | ((uint32)HlByte << 16) |  \
                        ((uint32)LhByte << 8) | ((uint32)LlByte));
} /* SomeIpTp_Get32BitVal */
#endif

/**********************************************************************************************************************
 * SomeIpTp_IsSduPtrValid()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL_INLINE FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsSduPtrValid(const PduInfoType *PduInfoPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if PduInfoPtr and SduDataPtr are valid. */
  return (boolean)((PduInfoPtr != NULL_PTR) && (PduInfoPtr->SduDataPtr != NULL_PTR)); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
} /* SomeIpTp_IsSduPtrValid */

#endif  /* SOMEIPTP_PRIV_H */

/***********************************************************************************************************************
 *  END OF FILE: SomeIpTp_Priv.h
 **********************************************************************************************************************/
