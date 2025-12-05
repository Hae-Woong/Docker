/***********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  SomeIpTp.c
 *        \brief  SomeIpTp source file
 *      \details  Implementation of Some/IP Transport protocol.
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

#define SOMEIPTP_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "SomeIpTp.h"
#include "SomeIpTp_Priv.h"
#if (SOMEIPTP_USE_INIT_POINTER == STD_ON)
#include "EcuM_Error.h"
#endif
#include "SomeIpTp_Cbk.h"
#include "SchM_SomeIpTp.h"
#include "PduR_SomeIpTp.h"
#if ((SOMEIPTP_DEV_ERROR_REPORT == STD_ON) || (SOMEIPTP_RUNTIME_ERROR_REPORT == STD_ON))
#include "Det.h"
#endif

/***********************************************************************************************************************
 *  VERSION CHECK
 **********************************************************************************************************************/
/* Check the version of SomeIpTp header file */
#if (  (SOMEIPTP_SW_MAJOR_VERSION != (5u)) \
    || (SOMEIPTP_SW_MINOR_VERSION != (1u)) \
    || (SOMEIPTP_SW_PATCH_VERSION != (1u)) )
# error "Vendor specific version numbers of SomeIpTp.c and SomeIpTp.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (SOMEIPTP_CFG_GEN_MAJOR_VERSION != (4u)) \
    || (SOMEIPTP_CFG_GEN_MINOR_VERSION != (1u)) )
# error "Version numbers of SomeIpTp.c and SomeIpTp_Cfg.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 ***********************************************************************************************************************/
#if ( SOMEIPTP_DEV_ERROR_REPORT == STD_ON )
#define SomeIpTp_ReportDet(a, b)     (void)Det_ReportError(SOMEIPTP_MODULE_ID, SOMEIPTP_INSTANCE_ID_DET, a, b) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
#define SomeIpTp_ReportDet(a, b)
#endif

#if ( SOMEIPTP_RUNTIME_ERROR_REPORT == STD_ON )
#define SomeIpTp_ReportRuntimeDet(a, b)   (void)Det_ReportRuntimeError(SOMEIPTP_MODULE_ID, SOMEIPTP_INSTANCE_ID_DET, a, b) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#else
#define SomeIpTp_ReportRuntimeDet(a, b)
#endif

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 ***********************************************************************************************************************/
 
/***********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 ***********************************************************************************************************************/
#define SOMEIPTP_START_SEC_VAR_CLEARED_8
#include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Initialization state of the module */
VAR(uint8, SOMEIPTP_VAR_CLEARED) SomeIpTp_ModuleInitialized = SOMEIPTP_UNINIT;

#if (SOMEIPTP_TXNSDU == STD_ON)
/* ! Current Queue size */
VAR(SomeIpTp_TxQueueElementType, SOMEIPTP_VAR_CLEARED) SomeIpTp_TxQueueSize = SOMEIPTP_QUEUE_EMPTY;
#endif

#define SOMEIPTP_STOP_SEC_VAR_CLEARED_8
#include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define SOMEIPTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Pointer to the current configuration */
P2CONST(SomeIpTp_ConfigType, SOMEIPTP_VAR_CLEARED, SOMEIPTP_PBCFG) SomeIpTp_ConfigDataPtr = NULL_PTR;

#define SOMEIPTP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 ***********************************************************************************************************************/
#define SOMEIPTP_START_SEC_CODE
#include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (SOMEIPTP_TXNSDU == STD_ON)
/***********************************************************************************************************************
 * SomeIpTp_AddQueueElement()
 ***********************************************************************************************************************/
/*! \brief        Add Element to Queue function
 *  \details      This function adds sdu requested for transmission to the queue
 *  \param[in]    PduId     Sdu ID passed by the upper layer which is used for transmission
 *  \pre          Interrupts are enabled.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDU IDs
 *  \synchronous  FALSE
 *  \trace        DSGN-SomeIpTpTxQueue
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_AddQueueElement(VAR(PduIdType, SOMEIPTP_APPL_VAR) PduId);

/***********************************************************************************************************************
 * SomeIpTp_DeleteQueueElement()
 ***********************************************************************************************************************/
/*! \brief        Delete Element from Queue function
 *  \details      This function deletes sdu requested for transmission from the queue either after the transmission 
 *                is complete or disassembly process is interrupted.
 *  \param[in]    PduId      Sdu ID passed by the upper layer which is used for transmission
 *  \pre          Interrupts are enabled.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDU IDs
 *  \synchronous  FALSE
 *  \trace        DSGN-SomeIpTpTxQueue
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_DelQueueElement(VAR(PduIdType, SOMEIPTP_APPL_VAR) PduId);

/***********************************************************************************************************************
 * SomeIpTp_WriteTxLongHeader()
 ***********************************************************************************************************************/
/*! \brief        Writes a complete SOME/IP TP header.
 *  \details      Writes the SOME/IP TP header of the segments sent during the segmented transmission of an SDU.
 *  \param[in]    Id             Tx SDU ID. Shall be less than the size of SomeIpTp_TXNSdu.
 *  \param[in]    Dest           Pointer to the buffer where the header will be written. The buffer shall be at least as
 *                               long as the SOME/IP TP header (12 bytes).
 *  \param[in]    MoreSegFlag    Flag indicating if the more segment flag of the SOME/IP header shall be set.
 *  \param[in]    FirstSegFlag   Flag indicating if the header to be written belongs to the first segment.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different IDs
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_WriteTxLongHeader(PduIdType Id, SduDataPtrType Dest, 
  uint8 MoreSegFlag, boolean FirstSegFlag);

/***********************************************************************************************************************
 * SomeIpTp_StoreTxShortHeader()
 ***********************************************************************************************************************/
/*! \brief        Stores a SOME/IP header during transmission.
 *  \details      Stores the SOME/IP header to be used during the segmented transmission of an SDU.
 *  \param[in]    Id   Tx SDU ID. Shall be less than the size of SomeIpTp_TXNSdu.
 *  \param[in]    Src  Pointer to the buffer containing the header to be stored. The buffer shall be at least as long
 *                     as the SOME/IP header (8 bytes).
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different IDs
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_StoreTxShortHeader(PduIdType Id,
                                                                     P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) Src);

/**********************************************************************************************************************
 * SomeIpTp_StoreTxMetadata()
 *********************************************************************************************************************/
/*! \brief        Stores metadata during transmission.
 *  \details      Stores the metadata to be used during the segmented transmission of an SDU.
 *  \param[in]    Id   Tx SDU ID. Shall be less than the size of SomeIpTp_TXNSdu.
 *  \param[in]    Src  Pointer to the buffer containing the metadata to be stored. The buffer shall be at least as long
 *                     as SDU's metadata.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different IDs
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
# if (SOMEIPTP_TXMETADATALENGTHOFTXNSDU == STD_ON)
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_StoreTxMetadata(PduIdType Id,
                                                                  P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) Src);
# else
#  define SomeIpTp_StoreTxMetadata(Id, Src)
# endif

/***********************************************************************************************************************
 * SomeIpTp_CancelCurrTxObject()
 ***********************************************************************************************************************/
/*! \brief        Cancel Tx Object function
 *  \details      This function stops the on-going disassembly process for the corresponding SDU
 *  \param[in]    Id       Tx SDU ID. Shall be less than the size of SomeIpTp_TXNSdu.
 *  \pre          Interrupts are enabled.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDU IDs
 *  \synchronous  FALSE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_CancelCurrTxObject(PduIdType Id);

/***********************************************************************************************************************
 * SomeIpTp_PrepareForNextCFTx()
 ***********************************************************************************************************************/
/*! \brief        Prepares next consecutive frame data fields
 *  \details      This function calculates payload offset, index and payload length for the next segment depending on 
 *                the available buffer of the upper layer.
 *  \param[in]    TxPduId           PduId passed by the lower layer during trigger transmit
 *  \param[in]    PayloadLen        payload length of current segment
 *  \param[in]    AvailableDataLen  Remaining data
 *  \param[out]   MoreSegments      Pointer to MoreSegments flag. Parameter must be a valid pointer.
 *  \return       E_OK       Valid, Next CF preparation successful
 *  \return       E_NOT_OK   Invalid remaining data length 
 *  \pre          Interrupts are enabled.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDU IDs
  ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_PrepareForNextCFTx(VAR(PduIdType, SOMEIPTP_APPL_VAR) TxPduId, 
  VAR(PduLengthType, SOMEIPTP_APPL_VAR) PayloadLen, VAR(PduLengthType, SOMEIPTP_APPL_VAR) AvailableDataLen, 
  P2VAR(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) MoreSegments);
#endif

#if (SOMEIPTP_RXNSDU == STD_ON)

/***********************************************************************************************************************
 * SomeIpTp_ReceiveCF()
 ***********************************************************************************************************************/
/*! \brief        Handles consecutive frame reception
 *  \details      This function evaluates the received consecutive frame and starts the assembly process 
 *                for the corresponding Pdu.
 *  \param[in]    RxPduId     Rx PDU ID. Shall be less than the size of SomeIpTp_RXNSdu.
 *  \param[in]    PduInfoPtr  Pdu info passed by the lower layer. The passed buffer shall be at least as long as the
 *                            SOME/IP header (8 bytes) plus the length of the PDU's metadata.
 *  \return       SOMEIPTP_E_ASSEMBLY_INTERRUPT - No availabe buffer from the upper layer or invalid payload length or 
 *                                                invalid offset field flags in the received frame
 *  \return       SOMEIPTP_E_INCONSISTENT_SEQUENCE - wrong frame received
 *  \return       SOMEIPTP_E_INCONSISTENT_HEADER - Received frame contains invalid header
 *  \return       SOMEIPTP_E_INCONSISTENT_METADATA - Metadata does not match between the received CFs
 *  \return       SOMEIPTP_E_NO_ERROR - No error
 *  \pre          Interrupts are enabled.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDU IDs
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(uint8, SOMEIPTP_CODE) SomeIpTp_ReceiveCF(VAR(PduIdType, SOMEIPTP_APPL_VAR) RxPduId, 
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr);

/***********************************************************************************************************************
 * SomeIpTp_ReceiveSF()
 ***********************************************************************************************************************/
/*! \brief        Handles single frame reception
 *  \details      This function evaluates the received single frame and forwards it to the upper layer.
 *  \param[in]    RxPduId     Rx PDU ID. Shall be less than the size of SomeIpTp_RXNSdu.
 *  \param[in]    PduInfoPtr  Pdu info passed by the lower layer. The passed buffer shall be at least as long as the
 *                            SOME/IP header (8 bytes) plus the length of the PDU's metadata.
 *  \return       SOMEIPTP_E_ASSEMBLY_INTERRUPT - No availabe buffer from the upper layer or invalid payload length or 
 *                                                invalid offset field flags in the received frame
 *  \return       SOMEIPTP_E_NO_ERROR - No error
 *  \pre          Interrupts are enabled.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDU IDs
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(uint8, SOMEIPTP_CODE) SomeIpTp_ReceiveSF(VAR(PduIdType, SOMEIPTP_APPL_VAR) RxPduId, 
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr);

/***********************************************************************************************************************
 * SomeIpTp_StartNewRxSession()
 ***********************************************************************************************************************/
/*! \brief        Starts a new Rx session
 *  \details      This function starts the assembly process for the corresponding Pdu
 *  \param[in]    Id          Rx PDU ID. Shall be less than the size of SomeIpTp_RXNSdu.
 *  \param[in]    PduInfoPtr  Pdu info passed by the lower layer. The passed buffer shall be at least as long as the
 *                            SOME/IP header (8 bytes) plus the length of the PDU's metadata.
 *  \param[in]    PayloadLen  Payload length.
 *  \pre          Interrupts are enabled.
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDU IDs
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(uint8, SOMEIPTP_CODE) SomeIpTp_StartNewRxSession(VAR(PduIdType, SOMEIPTP_APPL_VAR) Id, 
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr, VAR(PduLengthType, SOMEIPTP_APPL_VAR) PayloadLen);

/***********************************************************************************************************************
 * SomeIpTp_StoreRxShortHeader()
 ***********************************************************************************************************************/
/*! \brief        Stores a SOME/IP header during reception.
 *  \details      Stores the SOME/IP header to be used as reference during the segmented reception of an SDU.
 *  \param[in]    Id   Rx SDU ID. Shall be less than the size of SomeIpTp_RXNSdu.
 *  \param[in]    Src  Pointer to the buffer containing the header to be stored. The buffer shall be at least as long
 *                     as the SOME/IP header (8 bytes).
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different IDs
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_StoreRxShortHeader(PduIdType Id,
                                                                     P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) Src);

/***********************************************************************************************************************
 * SomeIpTp_IsRxShortHeaderValid()
 ***********************************************************************************************************************/
/*! \brief        Validates a SOME/IP header during reception.
 *  \details      Validates the SOME/IP header of a received segment during the segmented reception of an SDU.
 *  \param[in]    Id         Rx SDU ID. Shall be less than the size of SomeIpTp_RXNSdu.
 *  \param[in]    NewHeader  Pointer to the buffer containing the header to be validated. The buffer shall be at least
 *                           as long as the SOME/IP header (8 bytes).
 *  \return       TRUE   The header is valid.
 *  \return       FALSE  The header is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDUs.
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsRxShortHeaderValid(PduIdType Id,
                                                                          P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) NewHeader);

# if (SOMEIPTP_RXMETADATALENGTHOFRXNSDU == STD_ON)
/**********************************************************************************************************************
 * SomeIpTp_StoreRxMetadata()
 *********************************************************************************************************************/
/*! \brief        Stores metadata during reception.
 *  \details      Stores the metadata to be used during the segmented reception of an SDU.
 *  \param[in]    Id   Rx SDU ID. Shall be less than the size of SomeIpTp_RXNSdu.
 *  \param[in]    Src  Pointer to the buffer containing the metadata to be stored. The buffer shall be at least as long
 *                     as SDU's metadata.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different IDs
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_StoreRxMetadata(PduIdType Id,
                                                                  P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) Src);

/***********************************************************************************************************************
 * SomeIpTp_IsRxMetadataValid()
 ***********************************************************************************************************************/
/*! \brief        Validates metadata during reception.
 *  \details      Validates the metadata of a received segment during the segmented reception of an SDU.
 *  \param[in]    Id           Rx SDU ID. Shall be less than the size of SomeIpTp_RXNSdu.
 *  \param[in]    NewMetadata  Pointer to the buffer containing the metadata to be validated. The buffer shall be at
 *                             least as long as the SDU's metadata.
 *  \return       TRUE   The metadata is valid.
 *  \return       FALSE  The metadata is invalid.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE for different PDUs.
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsRxMetadataValid(PduIdType Id,
                                                                       P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) NewMetadata);
# endif

/***********************************************************************************************************************
 * SomeIpTp_IsMsgTypeTp()
 ***********************************************************************************************************************/
/*! \brief        Returns TRUE if TP flag of the frame is set.
 *  \details      -
 *  \param[in]    MsgType  Msg type field of the frame.
 *  \return       TRUE   TP flag is set.
 *  \return       FALSE  TP flag is not set.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 ***********************************************************************************************************************/
SOMEIPTP_LOCAL FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsMsgTypeTp(uint8 MsgType);

#endif

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 ***********************************************************************************************************************/
#if (SOMEIPTP_TXNSDU == STD_ON)
/***********************************************************************************************************************
 * SomeIpTp_AddQueueElement()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_AddQueueElement(VAR(PduIdType, SOMEIPTP_APPL_VAR) PduId)
{
  Std_ReturnType lReturnVal = E_NOT_OK;
  /* #10 Add Tx Sdu to the queue if the queue is not full. */
  SchM_Enter_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
  if(SomeIpTp_TxQueueSize < SomeIpTp_GetSizeOfTxQueueElement())
  {
    SomeIpTp_SetTxQueueElement(SomeIpTp_TxQueueSize, PduId); /* SBSW_SOMEIPTP_WRT_CSL01_TXQUEUEELEMENT */
    SomeIpTp_TxQueueSize++;
    lReturnVal = E_OK;
  }
  SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
  return lReturnVal;
}

/***********************************************************************************************************************
 * SomeIpTp_DeleteQueueElement()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_DelQueueElement(VAR(PduIdType, SOMEIPTP_APPL_VAR) PduId)
{
  SomeIpTp_TxQueueElementIterType lIndex;
  SomeIpTp_TxQueueElementIterType lQPos;
  /* #10 Search Tx Sdu in the queue, if the queue is not empty and delete it from the queue. */
  if((SomeIpTp_TxQueueSize != 0u) && (SomeIpTp_TxQueueSize <= SomeIpTp_GetSizeOfTxQueueElement()))
  {
    for (lIndex = 0u;lIndex < SomeIpTp_TxQueueSize;lIndex++)
    {
      if(SomeIpTp_GetTxQueueElement(lIndex) == PduId)
      {
        SchM_Enter_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();

        for(lQPos = lIndex; lQPos < ((SomeIpTp_TxQueueElementIterType)SomeIpTp_TxQueueSize - 1u); lQPos++)
        {
          SomeIpTp_SetTxQueueElement(lQPos, SomeIpTp_GetTxQueueElement(lQPos+1u)); /* SBSW_SOMEIPTP_WRT_CSL01_TXQUEUEELEMENT */
        }
        SomeIpTp_TxQueueSize--;

        SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
        break;
      }
    }
  }
}

/***********************************************************************************************************************
 * SomeIpTp_WriteTxLongHeader()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_WriteTxLongHeader(PduIdType Id, SduDataPtrType Dest, 
  uint8 MoreSegFlag, boolean FirstSegFlag)
{
  uint32 lOffset;
  SomeIpTp_TxStateIdxOfTXNSduType lTxStateIdx = SomeIpTp_GetTxStateIdxOfTXNSdu(Id);

  /* #10 If not the first segment, write the previously stored values in the SOME/IP fields. */
  if (FirstSegFlag == FALSE)
  {
    SomeIpTp_TxShortHeadersIterType lDestIdx;
    SomeIpTp_TxShortHeadersIterType lSrcIdx = SomeIpTp_GetTxShortHeadersStartIdxOfTXNSdu(Id);

    for (lDestIdx = 0; lDestIdx < SOMEIPTP_SHORT_HEADER_LENGTH; lDestIdx++)
    {
      Dest[lDestIdx] = SomeIpTp_GetTxShortHeaders(lSrcIdx);  /* SBSW_SOMEIPTP_WRT_LONGHEADER */
      lSrcIdx++;
    }
  }

  /* #20 Set the TP-flag of the message type field. */
  Dest[SOMEIPTP_MESSAGE_TYPE_8BIT_OFFSET] |= SOMEIPTP_TPFLAG_8BIT_VAL;  /* SBSW_SOMEIPTP_WRT_LONGHEADER */

  /* #30 Write the SOME/IP TP fields. */
  lOffset = SomeIpTp_GetOffsetOfTxState(lTxStateIdx) << 4u;
  Dest[SOMEIPTP_OFFSET_FIELD0_8BIT_OFFSET] = (uint8) (lOffset >> 24u);  /* SBSW_SOMEIPTP_WRT_LONGHEADER */
  Dest[SOMEIPTP_OFFSET_FIELD1_8BIT_OFFSET] = (uint8) (lOffset >> 16u);  /* SBSW_SOMEIPTP_WRT_LONGHEADER */
  Dest[SOMEIPTP_OFFSET_FIELD2_8BIT_OFFSET] = (uint8) (lOffset >> 8u);   /* SBSW_SOMEIPTP_WRT_LONGHEADER */
  Dest[SOMEIPTP_MORE_SEG_FLAG_8BIT_OFFSET] = (uint8) (lOffset | MoreSegFlag);  /* SBSW_SOMEIPTP_WRT_LONGHEADER */
}

/***********************************************************************************************************************
 * SomeIpTp_StoreTxShortHeader()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_StoreTxShortHeader(PduIdType Id,
                                                                     P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) Src)
{
  SomeIpTp_TxShortHeadersIterType lDstIdx;
  SomeIpTp_TxShortHeadersIterType lSrcIdx = 0;

  /* #10 Store the passed SOME/IP header. */
  for (lDstIdx = SomeIpTp_GetTxShortHeadersStartIdxOfTXNSdu(Id); lDstIdx < SomeIpTp_GetTxShortHeadersEndIdxOfTXNSdu(Id); lDstIdx++)
  {
    SomeIpTp_SetTxShortHeaders(lDstIdx, Src[lSrcIdx]); /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
    lSrcIdx++;
  }
}

# if (SOMEIPTP_TXMETADATALENGTHOFTXNSDU == STD_ON)
/***********************************************************************************************************************
 * SomeIpTp_StoreTxMetadata()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_StoreTxMetadata(PduIdType Id,
                                                                  P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) Src)
{
  SomeIpTp_TxMetadataIterType lDstIdx;
  SomeIpTp_TxMetadataIterType lSrcIdx = 0;

  /* #10 Store the passed metadata. */
  for (lDstIdx = SomeIpTp_GetTxMetadataStartIdxOfTXNSdu(Id); lDstIdx < SomeIpTp_GetTxMetadataEndIdxOfTXNSdu(Id); lDstIdx++)
  {
    SomeIpTp_SetTxMetadata(lDstIdx, Src[lSrcIdx]); /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
    lSrcIdx++;
  }
}
# endif

/***********************************************************************************************************************
 * SomeIpTp_CancelCurrTxObject()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_CancelCurrTxObject(PduIdType Id)
{
  /* #10 Delete Sdu from the queue and reset the timer.*/
  SomeIpTp_DelQueueElement(Id);
  SomeIpTp_SetTimerOfTxState(SomeIpTp_GetTxStateIdxOfTXNSdu(Id), 0);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
  /* #20 Reset the status of SDU to Idle.*/
  SomeIpTp_SetStatusOfTxState(SomeIpTp_GetTxStateIdxOfTXNSdu(Id), SOMEIPTP_TX_STATUS_IDLE);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
}

/***********************************************************************************************************************
 * SomeIpTp_PrepareForNextCFTx()
 ***********************************************************************************************************************/
/*! 
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
SOMEIPTP_LOCAL FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_PrepareForNextCFTx(VAR(PduIdType, SOMEIPTP_APPL_VAR) TxPduId, 
  VAR(PduLengthType, SOMEIPTP_APPL_VAR) PayloadLen, VAR(PduLengthType, SOMEIPTP_APPL_VAR) AvailableDataLen, 
  P2VAR(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) MoreSegments)
{
  PduLengthType lCurrDataIndex;
  PduLengthType lRemDataLen;
  Std_ReturnType lReturnVal = E_OK;
  SomeIpTp_TxStateIdxOfTXNSduType lTxStateIdx = SomeIpTp_GetTxStateIdxOfTXNSdu(TxPduId);

  /* #10 Calculate payload offset and index to copy payload for the next consecutive frame. */
  SomeIpTp_SetOffsetOfTxState(lTxStateIdx,                                            /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
    ((SomeIpTp_OffsetOfTxStateType)SomeIpTp_GetDataIndexOfTxState(lTxStateIdx)/SOMEIPTP_DATA_ALIGNMENT_LENGTH));

  lCurrDataIndex = SomeIpTp_GetDataIndexOfTxState(lTxStateIdx) + PayloadLen;

  SomeIpTp_SetDataIndexOfTxState(lTxStateIdx, lCurrDataIndex);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */

  /* #20 Calculate the payload length of the next segment. */
  lRemDataLen = SomeIpTp_GetDataLengthOfTxState(lTxStateIdx) - lCurrDataIndex;

  /* #30 If payload length is not zero, i.e, if the current segment is not the last segment: */
  if(lRemDataLen != 0u)
  {
    /* #40 Set the more seg flag to true. */
    *MoreSegments = 1u; /* SBSW_SOMEIPTP_WRT_MORESEGMENTS */
    /* #50 If next segment is not the last one and available payload length is valid: */
    if(((SomeIpTp_IsLastSegment(TxPduId, lRemDataLen) == TRUE) && (AvailableDataLen >= lRemDataLen)) || 
      (AvailableDataLen >= SOMEIPTP_DATA_ALIGNMENT_LENGTH))
    {
      /*  #60 If available payload length is less than configured PDU length: 
       *        If more segments, limit next segment length to 16 byte aligned available length.
       *        Otherwise set the next segment length to the remaining payload length. */
      if((AvailableDataLen + SOMEIPTP_LONG_HEADER_LENGTH) <= SomeIpTp_GetTxNPduLengthOfTXNSdu(TxPduId))
      {
        if(SomeIpTp_IsLastSegment(TxPduId, lRemDataLen) == FALSE)
        {
          SomeIpTp_SetNextSegmentDLOfTxState(lTxStateIdx,                                            /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
            ((AvailableDataLen/SOMEIPTP_DATA_ALIGNMENT_LENGTH) * SOMEIPTP_DATA_ALIGNMENT_LENGTH) + SOMEIPTP_LONG_HEADER_LENGTH);
        }
        else
        {
          SomeIpTp_SetNextSegmentDLOfTxState(lTxStateIdx, lRemDataLen + SOMEIPTP_LONG_HEADER_LENGTH);    /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
        }
      }
      /* #70 Otherwise set the next segment length to allowed segmented payload length. */
      else
      {
        SomeIpTp_SetNextSegmentDLOfTxState(lTxStateIdx, SomeIpTp_GetMaxTxSFPayloadOfTXNSdu(TxPduId)); /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
      }
    }
    /* #80 Otherwise return E_NOT_OK. */
    else /* Available length invalid */
    {
      lReturnVal = E_NOT_OK;
    }
  }

  return lReturnVal;
}
#endif

#if (SOMEIPTP_RXNSDU == STD_ON)
/***********************************************************************************************************************
 * SomeIpTp_StartNewRxSession()
 ***********************************************************************************************************************/
/*! 
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
 */
SOMEIPTP_LOCAL FUNC(uint8, SOMEIPTP_CODE) SomeIpTp_StartNewRxSession(VAR(PduIdType, SOMEIPTP_APPL_VAR) Id, 
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr, VAR(PduLengthType, SOMEIPTP_APPL_VAR) PayloadLen)
{
  PduInfoType lPduInfo;
  PduLengthType lBufferSizePtr;
  PduLengthType lSduLength;
  PduIdType lULId;
  SomeIpTp_RxStateIdxOfRXNSduType lRxStateIdx = SomeIpTp_GetRxStateIdxOfRXNSdu(Id);
  uint8 lErrorId = SOMEIPTP_E_NO_ERROR;

  /* Store the SOME/IP header, to use it as reference for the next segments. */
  SomeIpTp_StoreRxShortHeader(Id, PduInfoPtr->SduDataPtr);  /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */

  /* #10 Prepare the PduInfo and TpSduLength to be passed to StartOfReception.
   *     If metadata exists:
   *       Set PduInfo->SduDataPtr to the metadata's start.
   *       Set PduInfo->SduLength to the metadata's length.
   *     Otherwise:
   *       Set PduInfo->SduDataPtr to null.
   *       Set PduInfo->SduLength to 0.
   *     Set TpSduLength to 0. */
# if (SOMEIPTP_RXMETADATALENGTHOFRXNSDU == STD_ON)
  if (SomeIpTp_GetRxMetadataLengthOfRXNSdu(Id) > 0u)
  {
    lPduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[(PduInfoPtr->SduLength) - SomeIpTp_GetRxMetadataLengthOfRXNSdu(Id)];
    lPduInfo.SduLength = SomeIpTp_GetRxMetadataLengthOfRXNSdu(Id);
    SomeIpTp_StoreRxMetadata(Id, lPduInfo.SduDataPtr);  /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */
  }
  else
# endif
  {
    lPduInfo.SduDataPtr = NULL_PTR;
    lPduInfo.SduLength = 0;
  }

  lULId = SomeIpTp_GetUpperLayerIdOfRXNSdu(Id);
  /* Call start of reception indicating unknown message length */
  lSduLength = 0;

  /* #20 If StartOfReception succeeds:
   *       If the upper layer reports enough buffer to fit the SOME/IP header plus data:
   *         Clear the TP-flag of the SOME/IP header.
   *         Call CopyRxData to copy the SOME/IP header to the upper layer's buffer.
   *         If CopyRxData succeeds:
   *           Call CopyRxData to copy the data after the SOME/IP TP header to the upper layer's buffer.
   *           If CopyRxData succeeds:
   *             Update the data index and available buffer.
   *             Start the Rx timer.
   *             Set the SDU's state to SOMEIPTP_RX_STATUS_WAIT_NEXT_SF
   *           Otherwise, set SOMEIPTP_E_ASSEMBLY_INTERRUPT as return value.
   *         Otherwise, set SOMEIPTP_E_ASSEMBLY_INTERRUPT as return value. 
   *       Otherwise, set SOMEIPTP_E_ASSEMBLY_INTERRUPT as return value.
   *     Otherwise, report an SOMEIPTP_E_ASSEMBLY_INTERRUPT runtime error. */
  if(PduR_SomeIpTpStartOfReception(lULId, &lPduInfo, lSduLength, &lBufferSizePtr) == BUFREQ_OK)  /* SBSW_SOMEIPTP_CALL_STARTOFRECEPTION */
  {
    lSduLength = PayloadLen + SOMEIPTP_SHORT_HEADER_LENGTH;
    if(lBufferSizePtr >= lSduLength)
    {
      lPduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
      /* Reset TP flag */
      lPduInfo.SduDataPtr[SOMEIPTP_MESSAGE_TYPE_8BIT_OFFSET] &= ~SOMEIPTP_TPFLAG_8BIT_VAL;  /* SBSW_SOMEIPTP_WRT_MSGTYPE */
      lPduInfo.SduLength = SOMEIPTP_SHORT_HEADER_LENGTH;

      if(PduR_SomeIpTpCopyRxData(lULId, &lPduInfo, &lBufferSizePtr) == BUFREQ_OK)                /* SBSW_SOMEIPTP_CALL_COPYRXDATA */
      {
        /* Assign the payload */
        lPduInfo.SduDataPtr = (SduDataPtrType)&PduInfoPtr->SduDataPtr[SOMEIPTP_LONG_HEADER_LENGTH];
        lPduInfo.SduLength = PayloadLen;

        if(PduR_SomeIpTpCopyRxData(lULId, &lPduInfo, &lBufferSizePtr) == BUFREQ_OK)              /* SBSW_SOMEIPTP_CALL_COPYRXDATA */
        {
          SomeIpTp_SetDataIndexOfRxState(lRxStateIdx, PayloadLen);  /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU  */
          SomeIpTp_SetAvailBufSizeOfRxState(lRxStateIdx, lBufferSizePtr); /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
          SomeIpTp_SetTimerOfRxState(lRxStateIdx, SomeIpTp_GetRxTimeoutTimeOfChannel(SomeIpTp_GetChannelIdxOfRXNSdu(Id))); /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
          SomeIpTp_SetStatusOfRxState(lRxStateIdx, SOMEIPTP_RX_STATUS_WAIT_NEXT_SF); /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
        }
        else
        {
          lErrorId = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
        }
      }
      else
      {
        lErrorId = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
      }
    }
    else
    {
      lErrorId = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
    }
  }
  else
  {
    SomeIpTp_ReportRuntimeDet(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
  }
  
  return lErrorId;
}

/***********************************************************************************************************************
 * SomeIpTp_StoreRxShortHeader()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_StoreRxShortHeader(PduIdType Id,
                                                                     P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) Src)
{
  SomeIpTp_RxShortHeadersIterType lDstIdx;
  SomeIpTp_RxShortHeadersIterType lSrcIdx = 0;

  /* #10 Store the passed SOME/IP header. */
  for (lDstIdx = SomeIpTp_GetRxShortHeadersStartIdxOfRXNSdu(Id); lDstIdx < SomeIpTp_GetRxShortHeadersEndIdxOfRXNSdu(Id); lDstIdx++)
  {
    SomeIpTp_SetRxShortHeaders(lDstIdx, Src[lSrcIdx]); /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
    lSrcIdx++;
  }
}

/**********************************************************************************************************************
 * SomeIpTp_IsRxShortHeaderValid()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 */
SOMEIPTP_LOCAL FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsRxShortHeaderValid(PduIdType Id,
                                                                          P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) NewHeader)
{
  boolean lResult = TRUE;
  PduLengthType lNewHeaderIdx = 0;
  SomeIpTp_RxShortHeadersIterType lStoredHeaderIdx;

  /* #10 If the passed SOME/IP header matches the SDU's stored reference header, return true.
   *     Otherwise, return false. */
  for (lStoredHeaderIdx = SomeIpTp_GetRxShortHeadersStartIdxOfRXNSdu(Id); lStoredHeaderIdx < SomeIpTp_GetRxShortHeadersEndIdxOfRXNSdu(Id); lStoredHeaderIdx++)
  {
    if (NewHeader[lNewHeaderIdx] != SomeIpTp_GetRxShortHeaders(lStoredHeaderIdx))
    {
      lResult = FALSE;
      break;
    }
    lNewHeaderIdx++;
  }
  return lResult;
}

# if (SOMEIPTP_RXMETADATALENGTHOFRXNSDU == STD_ON)
/***********************************************************************************************************************
 * SomeIpTp_StoreRxMetadata()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL FUNC(void, SOMEIPTP_CODE) SomeIpTp_StoreRxMetadata(PduIdType Id,
                                                                  P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) Src)
{
  SomeIpTp_RxMetadataIterType lDstIdx;
  SomeIpTp_RxMetadataIterType lSrcIdx = 0;

  /* #10 Store the passed metadata. */
  for (lDstIdx = SomeIpTp_GetRxMetadataStartIdxOfRXNSdu(Id); lDstIdx < SomeIpTp_GetRxMetadataEndIdxOfRXNSdu(Id); lDstIdx++)
  {
    SomeIpTp_SetRxMetadata(lDstIdx, Src[lSrcIdx]); /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
    lSrcIdx++;
  }
}

/**********************************************************************************************************************
 * SomeIpTp_IsRxMetadataValid()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 */
SOMEIPTP_LOCAL FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsRxMetadataValid(PduIdType Id,
                                                                       P2CONST(uint8, AUTOMATIC, SOMEIPTP_APPL_DATA) NewMetadata)
{
  boolean lResult = TRUE;
  PduLengthType lNewMetadataIdx = 0;
  SomeIpTp_RxMetadataIterType lStoredMetadataIdx;

  /* #10 If the passed metadata matches the SDU's stored reference metadata, return true.
   *     Otherwise, return false. */
  for (lStoredMetadataIdx = SomeIpTp_GetRxMetadataStartIdxOfRXNSdu(Id); lStoredMetadataIdx < SomeIpTp_GetRxMetadataEndIdxOfRXNSdu(Id); lStoredMetadataIdx++)
  {
    if (NewMetadata[lNewMetadataIdx] != SomeIpTp_GetRxMetadata(lStoredMetadataIdx))
    {
      lResult = FALSE;
      break;
    }
    lNewMetadataIdx++;
  }
  return lResult;
}
# endif

/**********************************************************************************************************************
 * SomeIpTp_IsMsgTypeTp()
 *********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 */
SOMEIPTP_LOCAL FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_IsMsgTypeTp(uint8 MsgType)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Returns if the frame is TP frame. */
  return (boolean)((MsgType & (uint8)SOMEIPTP_TPFLAG_8BIT_VAL) == SOMEIPTP_TPFLAG_8BIT_VAL); /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
} /* SomeIpTp_IsMsgTypeTp */

/***********************************************************************************************************************
 * SomeIpTp_ReceiveCF()
 ***********************************************************************************************************************/
/*! 
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
 *
 *
 */
SOMEIPTP_LOCAL FUNC(uint8, SOMEIPTP_CODE) SomeIpTp_ReceiveCF(VAR(PduIdType, SOMEIPTP_APPL_VAR) RxPduId, /* PRQA S 6080, 6050, 6030 */ /* MD_MSR_STMIF */ /* MD_MSR_STCAL */ /* MD_MSR_STCYC */
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr)
{
  uint32 lTpSpecificBits;
  uint32 lOffsetVal;
  PduIdType lULId;
  SomeIpTp_RxStateIdxOfRXNSduType lRxStateIdx = SomeIpTp_GetRxStateIdxOfRXNSdu(RxPduId);
#if (SOMEIPTP_RXMETADATALENGTHOFRXNSDU == STD_ON)
  SomeIpTp_RxMetadataLengthOfRXNSduType lMetadataLength;
#endif
  PduLengthType lPayloadLen;
  uint8 lRuntimeErrId = SOMEIPTP_E_NO_ERROR;

  lULId = SomeIpTp_GetUpperLayerIdOfRXNSdu(RxPduId);
  /* #20 Extract TP header fields. */    
#if (SOMEIPTP_RXMETADATALENGTHOFRXNSDU == STD_ON)
  lMetadataLength = SomeIpTp_GetRxMetadataLengthOfRXNSdu(RxPduId);
  lPayloadLen = (PduInfoPtr->SduLength) - SOMEIPTP_LONG_HEADER_LENGTH - lMetadataLength;
#else
  lPayloadLen = (PduInfoPtr->SduLength) - SOMEIPTP_LONG_HEADER_LENGTH;
#endif

  lTpSpecificBits = SomeIpTp_Get32BitVal(PduInfoPtr->SduDataPtr[SOMEIPTP_OFFSET_FIELD0_8BIT_OFFSET], 
                                          PduInfoPtr->SduDataPtr[SOMEIPTP_OFFSET_FIELD1_8BIT_OFFSET], 
                                          PduInfoPtr->SduDataPtr[SOMEIPTP_OFFSET_FIELD2_8BIT_OFFSET],
                                          PduInfoPtr->SduDataPtr[SOMEIPTP_MORE_SEG_FLAG_8BIT_OFFSET]);

  lOffsetVal = ((lTpSpecificBits & SOMEIPTP_OFFSET_FIELD_MASK) >> 4u);

  /* #21 If no assembly process is on-going for this Pdu: */
  if(SomeIpTp_GetStatusOfRxState(lRxStateIdx) == SOMEIPTP_RX_STATUS_IDLE)
  {
    /* #211 If the offset us zero and more segment flag is set and payload is 16 byte aligned, 
       start the assembly process for this PDU. */
    if((lOffsetVal == SOMEIPTP_FF_OFFSET) && 
      (SomeIpTp_IsMoreSegFlagSet(lTpSpecificBits) == TRUE) && (SomeIpTp_IsPayloadSixteenByteAligned(lPayloadLen) == TRUE)) /* PRQA S 3415 */ /* MD_SomeIpTp_3415 */
    {
      lRuntimeErrId = SomeIpTp_StartNewRxSession(RxPduId, PduInfoPtr, lPayloadLen);  /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */
    }
# if (SOMEIPTP_RUNTIME_ERROR_REPORT == STD_ON)
    /* #212 If more segment flag or payload length is invalid, report runtime error with SOMEIPTP_E_ASSEMBLY_INTERRUPT. */
    else if(lOffsetVal == SOMEIPTP_FF_OFFSET)
    {
      SomeIpTp_ReportRuntimeDet(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
    }
    /* #213 If the offset is not zero, report runtime error with SOMEIPTP_E_INCONSISTENT_SEQUENCE. */
    else
    {
      SomeIpTp_ReportRuntimeDet(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_INCONSISTENT_SEQUENCE);
    }
# endif
  }
  else /* #22 If PDU received in the current session: */
  {
    /* #23 If Offset is greater than zero: */
    if(lOffsetVal != SOMEIPTP_FF_OFFSET)
    {
      /* #24 Stop the assembly process, report runtime error and give negative Rx Indication to the upper layer if:
       *         1. Inconsistent header.
       *         2. Received offset does not match with the payload received in the previous segments.
       *         3. Received payload does not fit in the available buffer.
       *         4. Inconsistent metadata.
       *         5. If Payload is not divisible by 16 when more segment flag is set.
       *         6. If upperlayer returns other than BUFREQ_OK. */
      if(SomeIpTp_IsRxShortHeaderValid(RxPduId, PduInfoPtr->SduDataPtr) == FALSE)  /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */
      {
        /* 1. Inconsistent header. */
        lRuntimeErrId = SOMEIPTP_E_INCONSISTENT_HEADER;
      }
      /* 2. Received offset does not match with the payload received in the previous segments. */
      else if((lOffsetVal * SOMEIPTP_DATA_ALIGNMENT_LENGTH) != SomeIpTp_GetDataIndexOfRxState(lRxStateIdx))
      {
        lRuntimeErrId = SOMEIPTP_E_INCONSISTENT_SEQUENCE;
      }
      /* 3.Received payload does not fit in the available buffer. */
      else if(SomeIpTp_GetAvailBufSizeOfRxState(lRxStateIdx) < lPayloadLen)
      {
        lRuntimeErrId = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
      }
      /* 4. Inconsistent metadata. */
#if (SOMEIPTP_RXMETADATALENGTHOFRXNSDU == STD_ON)
      else if((lMetadataLength > 0u) && (SomeIpTp_IsRxMetadataValid(RxPduId, &PduInfoPtr->SduDataPtr[PduInfoPtr->SduLength - lMetadataLength]) == FALSE)) /* PRQA S 3415 */ /* MD_SomeIpTp_3415 */ /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */
      {
        lRuntimeErrId = SOMEIPTP_E_INCONSISTENT_METADATA;
      }
#endif 
      else
      {
        PduInfoType lPduInfo;
        PduLengthType lBufSize;
        lPduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[SOMEIPTP_LONG_HEADER_LENGTH];
        lPduInfo.SduLength = lPayloadLen;

        /* #25 If more seg flag set and payload 16 byte aligned, Call PduR_SomeIpTpCopyRxData to copy data 
         *     by the upper layer. */
        if((SomeIpTp_IsMoreSegFlagSet(lTpSpecificBits) == TRUE) && 
          (SomeIpTp_IsPayloadSixteenByteAligned(lPayloadLen) == TRUE)) /* PRQA S 3415 */ /* MD_SomeIpTp_3415 */
        {
          /* #251 If copy succeeded, set the data index, available buffer for the next segment and start the Rx Timer. */
          if(PduR_SomeIpTpCopyRxData(lULId, &lPduInfo, &lBufSize) == BUFREQ_OK)     /* SBSW_SOMEIPTP_CALL_COPYRXDATA */
          {
            SomeIpTp_SetDataIndexOfRxState(lRxStateIdx, SomeIpTp_GetDataIndexOfRxState(lRxStateIdx) + lPayloadLen); /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
            SomeIpTp_SetAvailBufSizeOfRxState(lRxStateIdx, lBufSize);  /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
            SomeIpTp_SetTimerOfRxState(lRxStateIdx,                    /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
              SomeIpTp_GetRxTimeoutTimeOfChannel(SomeIpTp_GetChannelIdxOfRXNSdu(RxPduId)));
          }
          /* #252 Otherwise set the runtime error to SOMEIPTP_E_ASSEMBLY_INTERRUPT. */
          else  /* 6. if upperlayer returns other than BUFREQ_OK */
          {
            lRuntimeErrId = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
          }
        }
        /* #26 If more seg flag is not set :
            If copy RX data succeded, give Rx Indication to the upper layer 
            indicating the last frame received.
            Otherwise set the runtime error to SOMEIPTP_E_ASSEMBLY_INTERRUPT. */
        else if(SomeIpTp_IsMoreSegFlagSet(lTpSpecificBits)==FALSE)
        {
          if(PduR_SomeIpTpCopyRxData(lULId, &lPduInfo, &lBufSize) == BUFREQ_OK)  /* SBSW_SOMEIPTP_CALL_COPYRXDATA */
          {
            PduR_SomeIpTpRxIndication(lULId, E_OK);
            /* Reset the Pdu state to Idle */
            SomeIpTp_SetStatusOfRxState(lRxStateIdx, SOMEIPTP_RX_STATUS_IDLE);  /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
          }
          else  /* 6. if upperlayer returns other than BUFREQ_OK */
          {
            lRuntimeErrId = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
          }
        }
        /* #27 Otherwise set the runtime error to SOMEIPTP_E_ASSEMBLY_INTERRUPT. */
        else /* 5. If Payload is not divisible by 16 when more segment flag is set */
        {
          lRuntimeErrId = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
        }
      }
    }
    /*  #28 Otherwise Report runtime error, negative Rx Indication to upper layer and start the assembly 
     *      process(new session) with offset 0. */
    else
    { 
      SomeIpTp_ReportRuntimeDet(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_INCONSISTENT_SEQUENCE);
      PduR_SomeIpTpRxIndication(lULId, E_NOT_OK);
      lRuntimeErrId = SomeIpTp_StartNewRxSession(RxPduId, PduInfoPtr, lPayloadLen); /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */
    }
  }
  return lRuntimeErrId;
} /* SomeIpTp_ReceiveCF */

/***********************************************************************************************************************
 * SomeIpTp_ReceiveSF()
 ***********************************************************************************************************************/
/*! 
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
SOMEIPTP_LOCAL FUNC(uint8, SOMEIPTP_CODE) SomeIpTp_ReceiveSF(VAR(PduIdType, SOMEIPTP_APPL_VAR) RxPduId, 
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr)
{
  PduInfoType lPduInfo;
  PduLengthType lAvailBufSize;
  PduLengthType lPayloadLen;
  PduIdType lULId;
  uint8 lRuntimeErrId = SOMEIPTP_E_NO_ERROR;

  lULId = SomeIpTp_GetUpperLayerIdOfRXNSdu(RxPduId);

  /* #10 Prepare the PduInfo and TpSduLength to be passed to StartOfReception.
    *      If metadata exists:
    *        Set TpSduLength to the length passed by the lower layer minus the metadata's length.
    *        Set PduInfo->SduDataPtr to the metadata's start.
    *        Set PduInfo->SduLength to the metadata's length.
    *      Otherwise:
    *        Set TpSduLength to the length passed by the lower layer.
    *        Set PduInfo->SduDataPtr to null.
    *        Set PduInfo->SduLength to 0.
    *      Call StartOfReception. */
#if (SOMEIPTP_RXMETADATALENGTHOFRXNSDU == STD_ON)
  if (SomeIpTp_GetRxMetadataLengthOfRXNSdu(RxPduId) > 0u)
  {
    lPayloadLen = PduInfoPtr->SduLength - SomeIpTp_GetRxMetadataLengthOfRXNSdu(RxPduId);
    lPduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[lPayloadLen];
    lPduInfo.SduLength = SomeIpTp_GetRxMetadataLengthOfRXNSdu(RxPduId);
  }
  else
# endif
  {
    lPayloadLen = PduInfoPtr->SduLength;
    lPduInfo.SduDataPtr = NULL_PTR;
    lPduInfo.SduLength = 0;
  } 

  if(PduR_SomeIpTpStartOfReception(lULId, &lPduInfo, lPayloadLen, &lAvailBufSize) == BUFREQ_OK)   /* SBSW_SOMEIPTP_CALL_STARTOFRECEPTION */
  {
    /* #20 if payload fits in the available buffer and BUFREQ_OK, call Copy Rx Data to the upper layer 
    and verify the return value */
    lPduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
    lPduInfo.SduLength = lPayloadLen;

    if((lAvailBufSize >= lPayloadLen) && 
      (PduR_SomeIpTpCopyRxData(lULId, &lPduInfo, &lAvailBufSize) == BUFREQ_OK)) /* PRQA S 3415 */ /* MD_SomeIpTp_3415 */ /* SBSW_SOMEIPTP_CALL_COPYRXDATA */
    {
      /* #21 If BUFREQ_OK, call Rx Indication to the upper layer indicating end of reception */
      PduR_SomeIpTpRxIndication(lULId, E_OK);
    }
    else
    {
      /* #22 Otherwise report runtime error with assembly interrupt and negative Rx Indication */
      lRuntimeErrId = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
    }
  }
  else
  {
    SomeIpTp_ReportRuntimeDet(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
  }

  return lRuntimeErrId;
} /* SomeIpTp_ReceiveSF */
#endif

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 *  SomeIpTp_InitMemory()
 ***********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOMEIPTP_CODE) SomeIpTp_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  SomeIpTp_ModuleInitialized = SOMEIPTP_UNINIT;
}

/***********************************************************************************************************************
 * SomeIpTp_Init()
 ***********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, SOMEIPTP_CODE) SomeIpTp_Init(P2CONST(SomeIpTp_ConfigType, AUTOMATIC, SOMEIPTP_INIT_DATA) ConfigPtr)
{

#if (SOMEIPTP_USE_INIT_POINTER == STD_ON)
  /* #10 Check for ConfigPtr validity and Compatibility of configuration in case of Post build loadable. */
  SomeIpTp_ConfigDataPtr = ConfigPtr;
  /* ----- Development Error Checks ------------------------------------------------------------ */
  if(SomeIpTp_ConfigDataPtr == NULL_PTR)
  {
    EcuM_BswErrorHook((uint16) SOMEIPTP_MODULE_ID, (uint8)ECUM_BSWERROR_NULLPTR);
# if (SOMEIPTP_DEV_ERROR_REPORT == STD_ON)
    SomeIpTp_ReportDet(SOMEIPTP_SID_INIT, SOMEIPTP_E_PARAM_POINTER);
# endif
  }
# if (SOMEIPTP_CONFIGURATION_VARIANT == SOMEIPTP_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE)
  /* Check magic number */
  else if (SOMEIPTP_FINAL_MAGIC_NUMBER != SomeIpTp_GetFinalMagicNumber())
  {
    EcuM_BswErrorHook((uint16) SOMEIPTP_MODULE_ID, (uint8)ECUM_BSWERROR_MAGICNUMBER);
  }
# endif
  else
  {
#endif
    {
#if (SOMEIPTP_TXNSDU == STD_ON)
      {
        SomeIpTp_TxStateIterType lTxIndex;
        /* #20 Reset the state of Tx and Rx Nsdus to Idle. */
        for(lTxIndex = 0; lTxIndex < SomeIpTp_GetSizeOfTxState(); lTxIndex++)
        {
          SomeIpTp_SetStatusOfTxState(lTxIndex, SOMEIPTP_TX_STATUS_IDLE);  /* SBSW_SOMEIPTP_WRT_CSL01_TXSTATE */
        }
        SomeIpTp_TxQueueSize = SOMEIPTP_QUEUE_EMPTY;
      }
#endif

#if (SOMEIPTP_RXNSDU == STD_ON)
      {
        SomeIpTp_RxStateIterType lRxIndex;
        for(lRxIndex = 0; lRxIndex < SomeIpTp_GetSizeOfRxState(); lRxIndex++)
        {
          SomeIpTp_SetStatusOfRxState(lRxIndex, SOMEIPTP_RX_STATUS_IDLE);  /* SBSW_SOMEIPTP_WRT_CSL01_RXSTATE */
        }
      }
#endif
       /* #30 Set the module state to Initialized. */
      SomeIpTp_ModuleInitialized = SOMEIPTP_INIT;
    }
#if (SOMEIPTP_USE_INIT_POINTER == STD_ON)
  }
#else
  SOMEIPTP_DUMMY_STATEMENT_CONST(ConfigPtr);
#endif
} /* SomeIpTp_Init */

#if (SOMEIPTP_VERSION_INFO_API == STD_ON)
/***********************************************************************************************************************
 * SomeIpTp_GetVersionInfo()
 ***********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, SOMEIPTP_CODE) SomeIpTp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) VersionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = SOMEIPTP_E_NO_ERROR;
  /* #10 Report DET error if versionInfo is null pointer. */

    /* ----- Development Error Checks ------------------------------------- */
# if (SOMEIPTP_DEV_ERROR_DETECT == STD_ON) /* COV_SOMEIPTP_DET_CHECK */
  if(VersionInfo == NULL_PTR)
  {
    lErrorId = SOMEIPTP_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* #20 Set versionInfo specified by corresponding macros in the header file. */
    VersionInfo->moduleID = SOMEIPTP_MODULE_ID;  /* SBSW_SOMEIPTP_WRT_STRUCT */
    VersionInfo->vendorID = SOMEIPTP_VENDOR_ID;  /* SBSW_SOMEIPTP_WRT_STRUCT */

    VersionInfo->sw_major_version = SOMEIPTP_SW_MAJOR_VERSION;  /* SBSW_SOMEIPTP_WRT_STRUCT */
    VersionInfo->sw_minor_version = SOMEIPTP_SW_MINOR_VERSION;  /* SBSW_SOMEIPTP_WRT_STRUCT */
    VersionInfo->sw_patch_version = SOMEIPTP_SW_PATCH_VERSION;  /* SBSW_SOMEIPTP_WRT_STRUCT */
  }
  /* ----- Development Error Report --------------------------------------- */
# if (SOMEIPTP_DEV_ERROR_REPORT == STD_ON)
  if(lErrorId != SOMEIPTP_E_NO_ERROR)
  {
    SomeIpTp_ReportDet(SOMEIPTP_SID_GET_VERSION_INFO, lErrorId);
  }
# else
  SOMEIPTP_DUMMY_STATEMENT(lErrorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

} /* SomeIpTp_GetVersionInfo */
#endif

/***********************************************************************************************************************
 * SomeIpTp_Transmit()
 ***********************************************************************************************************************/
/*! 
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
FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_Transmit(VAR(PduIdType, SOMEIPTP_APPL_VAR) TxPduId, /* PRQA S 6080, 6050, 6030 */ /* MD_MSR_STMIF */ /* MD_MSR_STCAL */ /* MD_MSR_STCYC */
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr)
{
#if (SOMEIPTP_TXNSDU == STD_ON)

  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = SOMEIPTP_E_NO_ERROR;
  uint8 lReturnValue = E_NOT_OK;
  
  /* #10 Check if SomeIpTp is initialized and all parameters are in valid range. */
  if(SomeIpTp_IsInitialized() == FALSE)
  {
    lErrorId = SOMEIPTP_E_NOTINIT;
  }
    /* ----- Development Error Checks ------------------------------------- */
# if (SOMEIPTP_DEV_ERROR_DETECT == STD_ON)  /* COV_SOMEIPTP_DET_CHECK */
  else if(SomeIpTp_IsTxSduIdInvalid(TxPduId) == TRUE)
  {
    lErrorId = SOMEIPTP_E_PARAM;
  }
  else if(SomeIpTp_IsSduPtrValid(PduInfoPtr) == FALSE)  /* SBSW_SOMEIPTP_CALL_ISSDUPTRVALID */
  {
    lErrorId = SOMEIPTP_E_PARAM_POINTER;
  }
# endif
  else
  {
     SomeIpTp_TxStateIdxOfTXNSduType lTxStateIdx = SomeIpTp_GetTxStateIdxOfTXNSdu(TxPduId);
     SchM_Enter_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
     /* #20 Check if no segmentation is on-going for the requested PDU ID. */
     if(SomeIpTp_GetStatusOfTxState(lTxStateIdx) == SOMEIPTP_TX_STATUS_IDLE)
     {
# if (SOMEIPTP_TXMETADATALENGTHOFTXNSDU == STD_ON)
        PduLengthType lSduLength = PduInfoPtr->SduLength - SomeIpTp_GetTxMetadataLengthOfTXNSdu(TxPduId);
# else
        PduLengthType lSduLength = PduInfoPtr->SduLength;
# endif

        /* #21 Verify for minimum length of the SDU. */
        if(SomeIpTp_IsTxSduLengthValid(TxPduId, lSduLength) == TRUE)
        {
          PduInfoType lPduInfo;
          PduLengthType lSFPayload;

          /* #211 Store the SOME/IP header, metadata and data length. */
          SomeIpTp_StoreTxShortHeader(TxPduId, PduInfoPtr->SduDataPtr);  /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */

          SomeIpTp_StoreTxMetadata(TxPduId, &PduInfoPtr->SduDataPtr[lSduLength]); /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */

          SomeIpTp_SetDataLengthOfTxState(lTxStateIdx, lSduLength - SOMEIPTP_SHORT_HEADER_LENGTH);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */

          /* #212 If the SDU does not fit in a single PDU:
           *        Set the length of the next PDU to the maximum segment length with proper alignment.
           *        Flag the transmission as segmented.
           *      Otherwise:
           *        Set the length of the next PDU to the SDU's length.
           *        Flag the transmission as unsegmented. */
          if(lSduLength > SomeIpTp_GetTxNPduLengthOfTXNSdu(TxPduId))
          {
            lSFPayload = SomeIpTp_GetMaxTxSFPayloadOfTXNSdu(TxPduId);
            SomeIpTp_SetSegmentedOfTxState(lTxStateIdx, TRUE);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */

            /* Set the Data Offset to zero */
            SomeIpTp_SetOffsetOfTxState(lTxStateIdx, SOMEIPTP_FF_OFFSET);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
            /* Set the Data Index to zero */
            SomeIpTp_SetDataIndexOfTxState(lTxStateIdx, SOMEIPTP_FF_OFFSET);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
            /* Set the status to wait for Tx Confirmation */ 
            SomeIpTp_SetStatusOfTxState(lTxStateIdx, SOMEIPTP_TX_STATUS_WAIT_TXCONF_SF);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
          }
          else
          {
            lSFPayload = lSduLength;
            SomeIpTp_SetSegmentedOfTxState(lTxStateIdx, FALSE);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
            SomeIpTp_SetStatusOfTxState(lTxStateIdx, SOMEIPTP_TX_STATUS_WAIT_TXCONF_LAST_SF);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
          }

          SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();

          /* Store the pdu length requested to Transmit */
          SomeIpTp_SetNextSegmentDLOfTxState(lTxStateIdx, lSFPayload);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */

          /* #214 Start Tx Confirmation Timeout time. */
          SomeIpTp_SetTimerOfTxState(lTxStateIdx,                                              /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
            SomeIpTp_GetTxConfTimeoutTimeOfChannel(SomeIpTp_GetChannelIdxOfTXNSdu(TxPduId)));

          /* #215 If metadata exists for the PDU, pass the frame to PduR with calculated length and metadata 
           *      by the sduptr, otherwise set the sduptr to null and verify the return result.  */
# if (SOMEIPTP_TXMETADATALENGTHOFTXNSDU == STD_ON)
          if (SomeIpTp_GetTxMetadataLengthOfTXNSdu(TxPduId) > 0u)
          {
            lPduInfo.SduDataPtr = &SomeIpTp_GetTxMetadata(SomeIpTp_GetTxMetadataStartIdxOfTXNSdu(TxPduId));
            lPduInfo.SduLength = lSFPayload + SomeIpTp_GetTxMetadataLengthOfTXNSdu(TxPduId);
          }
          else
#endif
          {
            lPduInfo.SduDataPtr = NULL_PTR;
            lPduInfo.SduLength = lSFPayload;
          }

          /* #216 Add the Tx PDU to the Tx Queue and invoke the lower layer transmit function. */
          lReturnValue = SomeIpTp_AddQueueElement(TxPduId);
          if(lReturnValue == E_OK)
          {
            lReturnValue = PduR_SomeIpTpTransmit(SomeIpTp_GetLowerLayerIdOfTXNSdu(TxPduId),  /* SBSW_SOMEIPTP_CALL_TRANSMIT */
              (const PduInfoType*)&lPduInfo);

            /* #217 If the request is rejected, delete the Tx Object from the Queue. */
            if (lReturnValue != E_OK)
            {
              SomeIpTp_CancelCurrTxObject(TxPduId);
            }
          }
          else
          {
            /* Fatal error: Queue must be equal to the number of Tx SDUs.
            Hence overflow of the queue cannot occur. */
# if (SOMEIPTP_DEV_ERROR_DETECT == STD_ON) /* COV_SOMEIPTP_DET_CHECK */
            lErrorId = SOMEIPTP_E_FATAL_ERROR;
# endif
            SomeIpTp_SetStatusOfTxState(lTxStateIdx, SOMEIPTP_TX_STATUS_IDLE);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
          }
        }
        else  /* Pdu Length invalid */
        {
          SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
        }
     }
     else
     {
        SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
        /* #30 Else interrupt the ongoing assembly process, call Tx Confirmation with negative result
        and report runtime error. */
        SomeIpTp_CancelCurrTxObject(TxPduId);
        PduR_SomeIpTpTxConfirmation(SomeIpTp_GetUpperLayerIdOfTXNSdu(TxPduId), E_NOT_OK);
        SomeIpTp_ReportRuntimeDet(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_DISASSEMBLY_INTERRUPT);
     }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (SOMEIPTP_DEV_ERROR_REPORT == STD_ON)
  if(lErrorId != SOMEIPTP_E_NO_ERROR)
  {
    SomeIpTp_ReportDet(SOMEIPTP_SID_TRANSMIT, lErrorId);
  }
# else
  SOMEIPTP_DUMMY_STATEMENT(lErrorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return lReturnValue;

#else
  SOMEIPTP_DUMMY_STATEMENT(TxPduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  SOMEIPTP_DUMMY_STATEMENT_CONST(PduInfoPtr);
  return E_NOT_OK;
#endif
} /* SomeIpTp_Transmit */

/***********************************************************************************************************************
 * SomeIpTp_TriggerTransmit()
 ***********************************************************************************************************************/
/*! 
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
FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_TriggerTransmit(VAR(PduIdType, SOMEIPTP_APPL_VAR) TxPduId, /* PRQA S 6080, 6030, 6050, 6010 */ /* MD_MSR_STMIF */ /* MD_MSR_STCYC */ /* MD_MSR_STCAL */ /* MD_MSR_STPTH */
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
#if (SOMEIPTP_TXNSDU == STD_ON)

  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = SOMEIPTP_E_NO_ERROR;
  uint8 lReturnValue = E_NOT_OK;

    /* ----- Development Error Checks ------------------------------------- */
    /* #10 Check if SomeIpTp is initialized and all parameters are in valid range. */
# if (SOMEIPTP_DEV_ERROR_DETECT == STD_ON) /* COV_SOMEIPTP_DET_CHECK */
  if(SomeIpTp_IsInitialized() == FALSE)
  {
    lErrorId = SOMEIPTP_E_NOTINIT;
  }
  else if(SomeIpTp_IsTxSduIdInvalid(TxPduId) == TRUE)
  {
    lErrorId = SOMEIPTP_E_PARAM;
  }
  else
# endif
  {
    SomeIpTp_TxStateIdxOfTXNSduType lTxStateIdx = SomeIpTp_GetTxStateIdxOfTXNSdu(TxPduId);
    PduLengthType lPdulength;
    PduLengthType lPayloadLen;
    uint8 lHeaderLen;
    
    /* Length of the next PDU to be sent. */
    lPdulength = SomeIpTp_GetNextSegmentDLOfTxState(lTxStateIdx);

    /* #20 Determine the length of the header, based on the message type (segmented or non-segmented). */
    if(SomeIpTp_IsSegmentedOfTxState(lTxStateIdx) == TRUE)  /* PRQA S 1881 */ /* MD_MSR_AutosarBoolean */
    {
      lHeaderLen = SOMEIPTP_LONG_HEADER_LENGTH;
    }
    else
    {
      lHeaderLen = SOMEIPTP_SHORT_HEADER_LENGTH;
    }

    /* #30 If the passed data pointer and length are valid: */
    if((SomeIpTp_IsSduPtrValid(PduInfoPtr) == TRUE) && (PduInfoPtr->SduLength >= lHeaderLen) && (PduInfoPtr->SduLength >= lPdulength))  /* SBSW_SOMEIPTP_CALL_ISSDUPTRVALID */
    {
      PduInfoType  lSduInfo;
      PduLengthType lAvailableDataLen;
      boolean lFirstSegFlag = FALSE;
      uint8 lMoreSegFlag = 0u;

      lPayloadLen = lPdulength-lHeaderLen;

      /* #40 If the transmission is segmented:
       *       If the first segment is being processed:
       *         Call CopyTxData to copy the SOME/IP header to the beginning of the lower layer's buffer.
       *         If CopyTxData succeeded and enough data is available from the upper layer:
       *           Call CopyTxData to copy data to the lower layer's buffer, after the SOME/IP TP header.
       *           If CopyTxData succeeded, set E_OK as return value.
       *       Otherwise:
       *         Call CopyTxData to copy data to the lower layer's buffer, after the SOME/IP TP header.
       *         If CopyTxData succeeded, set E_OK as return value.
       *     Otherwise:
       *       Call CopyTxData to copy the SOME/IP header plus data to the lower layer's buffer.
       *       If CopyTxData succeeded, set E_OK as return value. */
      if(lHeaderLen == SOMEIPTP_LONG_HEADER_LENGTH)
      {
        /* If first segment, call CopyTxData to copy the SOME/IP header first */
        if (SomeIpTp_GetDataIndexOfTxState(lTxStateIdx) == SOMEIPTP_FF_OFFSET)
        {
          lFirstSegFlag = TRUE;
          lSduInfo.SduLength = SOMEIPTP_SHORT_HEADER_LENGTH;
          lSduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;

          if((PduR_SomeIpTpCopyTxData(SomeIpTp_GetUpperLayerIdOfTXNSdu(TxPduId),      /* SBSW_SOMEIPTP_CALL_COPYTXDATA */
            &lSduInfo, NULL_PTR, &lAvailableDataLen) == BUFREQ_OK) && (lSduInfo.SduLength == SOMEIPTP_SHORT_HEADER_LENGTH))
          {
            if(lAvailableDataLen >= lPayloadLen)
            {
              /* Call CopyTxData with the calculated payload. */
              lSduInfo.SduLength = lPayloadLen;
              lSduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[SOMEIPTP_LONG_HEADER_LENGTH];
              if((PduR_SomeIpTpCopyTxData(SomeIpTp_GetUpperLayerIdOfTXNSdu(TxPduId),  /* SBSW_SOMEIPTP_CALL_COPYTXDATA */
                &lSduInfo, NULL_PTR, &lAvailableDataLen) == BUFREQ_OK) && (lSduInfo.SduLength == lPayloadLen))
              {
                lReturnValue = E_OK;
              }
            }
          }
        }
        else /* Consecutive segments */
        {
          /* Call CopyTxData with the calculated payload */
          lSduInfo.SduLength = lPayloadLen;
          lSduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[SOMEIPTP_LONG_HEADER_LENGTH];

          if((PduR_SomeIpTpCopyTxData(SomeIpTp_GetUpperLayerIdOfTXNSdu(TxPduId),      /* SBSW_SOMEIPTP_CALL_COPYTXDATA */
            &lSduInfo, NULL_PTR, &lAvailableDataLen) == BUFREQ_OK) \
            && (lSduInfo.SduLength == lPayloadLen))
          {
            lReturnValue = E_OK;
          }
        }
      }
      else  /* single frame */
      {
        /* Call CopyTxData to copy the complete payload along with SOME/IP Header */
        if((PduR_SomeIpTpCopyTxData(SomeIpTp_GetUpperLayerIdOfTXNSdu(TxPduId),        /* SBSW_SOMEIPTP_CALL_COPYTXDATA */
          PduInfoPtr, NULL_PTR, &lAvailableDataLen) == BUFREQ_OK) && (PduInfoPtr->SduLength == lPdulength))
        {
          lReturnValue = E_OK;
        }
      }
      /* #50 If the previous CopyTxData call(s) succeeded and the transmission is segmented: */
      if((lReturnValue == E_OK) && (lHeaderLen == SOMEIPTP_LONG_HEADER_LENGTH))
      {
        /* #60 Calculate payload offset, index and  payload length for the next CF. */ 
        lReturnValue = SomeIpTp_PrepareForNextCFTx(TxPduId, lPayloadLen, lAvailableDataLen, &lMoreSegFlag); /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */
        
        /* #85 If the available data length is valid, return the amount of data copied plus header 
               and assemble the header. */
        if(lReturnValue == E_OK)
        {
          PduInfoPtr->SduLength = lPdulength;  /* SBSW_SOMEIPTP_WRT_STRUCT */
          SomeIpTp_WriteTxLongHeader(TxPduId, PduInfoPtr->SduDataPtr, lMoreSegFlag, lFirstSegFlag);  /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */
        }
      }
      /* #90 If not BUFREQ_OK and invalid available data length, Call PduR_SomeIpTpTxConfirmation with result 
          E_NOT_OK and stop the disassembly process. */
      if(lReturnValue == E_NOT_OK)
      {
        /* Delete element from the queue */
        SomeIpTp_CancelCurrTxObject(TxPduId);
        PduR_SomeIpTpTxConfirmation(SomeIpTp_GetUpperLayerIdOfTXNSdu(TxPduId), E_NOT_OK);
        SomeIpTp_ReportRuntimeDet(SOMEIPTP_SID_TRIGGER_TRANSMIT, SOMEIPTP_E_DISASSEMBLY_INTERRUPT);
      }
      else /* Single Frame */
      {
        /* Data copy successful */
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (SOMEIPTP_DEV_ERROR_REPORT == STD_ON)
  if(lErrorId != SOMEIPTP_E_NO_ERROR)
  {
    SomeIpTp_ReportDet(SOMEIPTP_SID_TRIGGER_TRANSMIT, lErrorId);
  }
# else
  SOMEIPTP_DUMMY_STATEMENT(lErrorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
  return lReturnValue;

#else
  SOMEIPTP_DUMMY_STATEMENT(TxPduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  SOMEIPTP_DUMMY_STATEMENT(PduInfoPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
  return E_NOT_OK;
#endif
} /* SomeIpTp_TriggerTransmit */

/***********************************************************************************************************************
 * SomeIpTp_TxConfirmation()
 ***********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, SOMEIPTP_CODE) SomeIpTp_TxConfirmation(VAR(PduIdType, SOMEIPTP_APPL_VAR) TxPduId) /* PRQA S 6080 */ /* MD_MSR_STMIF */
{
#if (SOMEIPTP_TXNSDU == STD_ON)

  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = SOMEIPTP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
    /* #10 Check if SomeIpTp is initialized. */
# if (SOMEIPTP_DEV_ERROR_DETECT == STD_ON) /* COV_SOMEIPTP_DET_CHECK */
  if(SomeIpTp_IsInitialized() == FALSE)
  {
    lErrorId = SOMEIPTP_E_NOTINIT;
  }
  else if(SomeIpTp_IsTxSduIdInvalid(TxPduId) == TRUE)
  {
    lErrorId = SOMEIPTP_E_PARAM;
  }
  else
# endif
  {
    SomeIpTp_StatusOfTxStateType lStatusTx;
    SomeIpTp_TxStateIdxOfTXNSduType lTxStateIdx = SomeIpTp_GetTxStateIdxOfTXNSdu(TxPduId);
    lStatusTx = SomeIpTp_GetStatusOfTxState(lTxStateIdx);

    /* #20 If last transmitted frame is not the last segmented frame, start the separation time counter.*/
    if(lStatusTx == SOMEIPTP_TX_STATUS_WAIT_TXCONF_SF)
    {
      /* #21 If the burst size is the default burst size, set the timer to configured separation time.
       *     Otherwise, set the timer to 0. */
      if (SomeIpTp_GetTxBurstSizeOfTXNSdu(TxPduId) == SOMEIPTP_DEFAULT_BURST_SIZE)
      {
        SomeIpTp_SetTimerOfTxState(lTxStateIdx,                           /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
                  SomeIpTp_GetNPduSeparationTimeOfChannel(SomeIpTp_GetChannelIdxOfTXNSdu(TxPduId)));
      }
      else
      {
        SomeIpTp_SetTimerOfTxState(lTxStateIdx, 0u);                      /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
      }
      
      SomeIpTp_SetStatusOfTxState(lTxStateIdx, SOMEIPTP_TX_STATUS_WAIT_ST); /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
    }
    /* #30 Otherwise, reset the status of SDU to idle and pass the confirmation to PduR. */
    else if(lStatusTx == SOMEIPTP_TX_STATUS_WAIT_TXCONF_LAST_SF)
    {
      /* Delete Tx Object from Queue */
      SomeIpTp_CancelCurrTxObject(TxPduId);
      PduR_SomeIpTpTxConfirmation(SomeIpTp_GetUpperLayerIdOfTXNSdu(TxPduId), E_OK);
    }
    else
    {
       /* Do nothing */
    }
  }
    /* ----- Development Error Report --------------------------------------- */
# if (SOMEIPTP_DEV_ERROR_REPORT == STD_ON)
  if(lErrorId != SOMEIPTP_E_NO_ERROR)
  {
    SomeIpTp_ReportDet(SOMEIPTP_SID_TXCONFIRMATION, lErrorId);
  }
# else
  SOMEIPTP_DUMMY_STATEMENT(lErrorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

#else
  SOMEIPTP_DUMMY_STATEMENT(TxPduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
} /* SomeIpTp_TxConfirmation */

/***********************************************************************************************************************
 * SomeIpTp_MainFunctionTx()
 ***********************************************************************************************************************/
/*! 
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
FUNC(void, SOMEIPTP_CODE) SomeIpTp_MainFunctionTx(void) /* PRQA S 6080, 6030 */ /* MD_MSR_STMIF */ /* MD_MSR_STCYC */
{
#if (SOMEIPTP_TXNSDU == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  SomeIpTp_TxQueueElementIterType lQIndex = 0u;

  /* #10 Check if SomeIpTp is initialized.*/
  if((SomeIpTp_IsInitialized()==TRUE) && (SomeIpTp_IsTxQueueEmpty()==FALSE)) /* PRQA S 3415 */ /* MD_SomeIpTp_3415 */
  {
    /* #100  If initialized, loop through the Tx-Queue and check if there is any disassembly process going on for any SDU
     *       or waiting for Tx Confirmation. */
    while(lQIndex < SomeIpTp_TxQueueSize)
    {
      PduIdType lSduId = SomeIpTp_GetTxQueueElement(lQIndex);
      SomeIpTp_TxStateIdxOfTXNSduType lTxStateIdx = SomeIpTp_GetTxStateIdxOfTXNSdu(lSduId);
      lQIndex++;

      SchM_Enter_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
      if(SomeIpTp_GetStatusOfTxState(lTxStateIdx) != SOMEIPTP_TX_STATUS_IDLE)
      {
        if(SomeIpTp_GetTimerOfTxState(lTxStateIdx) != 0u)
        {
          SomeIpTp_DecTimerOfTxState(lTxStateIdx);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */ 
          SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
        }
        else
        {
          /* #101 If the separation time expired: \trace CREQ-169366 */
          if(SomeIpTp_GetStatusOfTxState(lTxStateIdx) == SOMEIPTP_TX_STATUS_WAIT_ST)
          {
            PduInfoType lPduInfo;
            uint8_least lburstIdx;

            SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();

            /* #102 Loop through the configured burst size and trigger the transmission of the next segments consecutively. */
            for (lburstIdx = 0; lburstIdx < SomeIpTp_GetTxBurstSizeOfTXNSdu(lSduId); lburstIdx++)
            {         
              Std_ReturnType lRetVal;
              boolean lIsLastSegment = FALSE;

              /* #103 If metadata exists for the PDU, pass the metadata by the SduPtr and set the datalength to SduLength+
               *      metadataLength, otherwise set the SduPtr to null ptr. */
# if (SOMEIPTP_TXMETADATALENGTHOFTXNSDU == STD_ON)
              if (SomeIpTp_GetTxMetadataLengthOfTXNSdu(lSduId) > 0u)
              {
                lPduInfo.SduLength = SomeIpTp_GetNextSegmentDLOfTxState(lTxStateIdx) + SomeIpTp_GetTxMetadataLengthOfTXNSdu(lSduId);
                lPduInfo.SduDataPtr = &SomeIpTp_GetTxMetadata(SomeIpTp_GetTxMetadataStartIdxOfTXNSdu(lSduId));
              }
              else
# endif
              {
                lPduInfo.SduLength = SomeIpTp_GetNextSegmentDLOfTxState(lTxStateIdx);
                lPduInfo.SduDataPtr = NULL_PTR;
              }
                              
              /* #104 Start the confirmation timeout timer and call the lower layer transmit function. */
              /* If last segment */
              if((SomeIpTp_GetDataLengthOfTxState(lTxStateIdx) - SomeIpTp_GetDataIndexOfTxState(lTxStateIdx)) > 
                (SomeIpTp_GetNextSegmentDLOfTxState(lTxStateIdx) - (PduLengthType)SOMEIPTP_LONG_HEADER_LENGTH))
              {
                SomeIpTp_SetStatusOfTxState(lTxStateIdx, SOMEIPTP_TX_STATUS_WAIT_TXCONF_SF);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
              }
              else
              {
                SomeIpTp_SetStatusOfTxState(lTxStateIdx, SOMEIPTP_TX_STATUS_WAIT_TXCONF_LAST_SF);  /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
                lIsLastSegment = TRUE;
              }
              SomeIpTp_SetTimerOfTxState(lTxStateIdx,                                       /* SBSW_SOMEIPTP_WRT_CSL03_TXNSDU */
                SomeIpTp_GetTxConfTimeoutTimeOfChannel(SomeIpTp_GetChannelIdxOfTXNSdu(lSduId)));

              lRetVal = PduR_SomeIpTpTransmit(SomeIpTp_GetLowerLayerIdOfTXNSdu(lSduId), &lPduInfo);  /* SBSW_SOMEIPTP_CALL_TRANSMIT */
              /* #105 If transmit request fails, stop the disassembly process, invoke negative confirmation to upper layer. */
              if(lRetVal != E_OK)
              {
                SomeIpTp_CancelCurrTxObject(lSduId);
                PduR_SomeIpTpTxConfirmation(SomeIpTp_GetUpperLayerIdOfTXNSdu(lSduId), E_NOT_OK);
                SomeIpTp_ReportRuntimeDet(SOMEIPTP_SID_MAINFUNCTION_TX, SOMEIPTP_E_DISASSEMBLY_INTERRUPT);            
              }

              /* #106 If transmit request fails or the current segment triggered is the last one, exit the loop. */
              if ((lRetVal != E_OK) || (lIsLastSegment == TRUE))
              {
                /* Decrement the queue index as the current element is removed. */
                lQIndex--;
                break;
              }        
            }
          }
          else /* Confirmation timed-out */
          {
            /* #107 If the confirmation timeout time expired, Stop the disassembly process and invoke 
             *      negative confirmation to upper layer. */
            SomeIpTp_CancelCurrTxObject(lSduId);
            SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
            PduR_SomeIpTpTxConfirmation(SomeIpTp_GetUpperLayerIdOfTXNSdu(lSduId), E_NOT_OK);
            SomeIpTp_ReportRuntimeDet(SOMEIPTP_SID_MAINFUNCTION_TX, SOMEIPTP_E_DISASSEMBLY_INTERRUPT);
            /* Decrement the queue index as the current element is removed. */ 
            lQIndex--;
          }
        }
      }
      else
      {
        SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
      }
    }
  }
#endif
} /* SomeIpTp_MainFunctionTx */

/***********************************************************************************************************************
 * SomeIpTp_RxIndication()
 ***********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, SOMEIPTP_CODE) SomeIpTp_RxIndication(VAR(PduIdType, SOMEIPTP_APPL_VAR) RxPduId, /* PRQA S 6080, 6050, 6030, 6010 */ /* MD_MSR_STMIF */ /* MD_MSR_STCAL */ /* MD_MSR_STCYC */ /* MD_MSR_STPTH */
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr)
{
#if (SOMEIPTP_RXNSDU == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lErrorId = SOMEIPTP_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if SomeIpTp is initialized and all parameters are in valid range. */
# if (SOMEIPTP_DEV_ERROR_DETECT == STD_ON) /* COV_SOMEIPTP_DET_CHECK */
  if(SomeIpTp_IsInitialized() == FALSE)
  {
    lErrorId = SOMEIPTP_E_NOTINIT;
  }
  else if(SomeIpTp_IsRxPduIdInvalid(RxPduId) == TRUE)
  {
    lErrorId = SOMEIPTP_E_PARAM;
  }
  else if(SomeIpTp_IsSduPtrValid(PduInfoPtr) == FALSE)  /* SBSW_SOMEIPTP_CALL_ISSDUPTRVALID */
  {
    lErrorId = SOMEIPTP_E_PARAM_POINTER;
  }
  else if(SomeIpTp_IsRxPduLengthValid(RxPduId, PduInfoPtr->SduLength) == FALSE)
  {
    lErrorId = SOMEIPTP_E_PARAM;
  }
  else
# endif
  {
    PduIdType lULId;
    SomeIpTp_RxStateIdxOfRXNSduType lRxStateIdx = SomeIpTp_GetRxStateIdxOfRXNSdu(RxPduId);
    uint8 lRuntimeErrId;

    lULId = SomeIpTp_GetUpperLayerIdOfRXNSdu(RxPduId);

    /* #20 If the message type is TP, process the received segmented frame. */
    if(SomeIpTp_IsMsgTypeTp(PduInfoPtr->SduDataPtr[SOMEIPTP_MESSAGE_TYPE_8BIT_OFFSET]) == TRUE)
    { 
      lRuntimeErrId = SomeIpTp_ReceiveCF(RxPduId, PduInfoPtr); /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */
    }
    else /* #30 Otherwise: */
    {
      /* #31 If no session is currently running, process single frame. */
      if(SomeIpTp_GetStatusOfRxState(lRxStateIdx) == SOMEIPTP_RX_STATUS_IDLE)
      {
        lRuntimeErrId = SomeIpTp_ReceiveSF(RxPduId, PduInfoPtr); /* SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK */
      }
      else /* #32 Otherwise, report runtime error with wrong message type. */
      {
        lRuntimeErrId = SOMEIPTP_E_MESSAGE_TYPE;
      }
    }
    /* ----- Runtime Error Report --------------------------------------- */
    if(lRuntimeErrId != SOMEIPTP_E_NO_ERROR)
    {
      SomeIpTp_ReportRuntimeDet(SOMEIPTP_SID_RXINDICATION, lRuntimeErrId);
      PduR_SomeIpTpRxIndication(lULId, E_NOT_OK);
      SomeIpTp_SetStatusOfRxState(lRxStateIdx, SOMEIPTP_RX_STATUS_IDLE);  /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if (SOMEIPTP_DEV_ERROR_REPORT == STD_ON)
  if(lErrorId != SOMEIPTP_E_NO_ERROR)
  {
    SomeIpTp_ReportDet(SOMEIPTP_SID_RXINDICATION, lErrorId);
  }
# else
  SOMEIPTP_DUMMY_STATEMENT(lErrorId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

#else
SOMEIPTP_DUMMY_STATEMENT(RxPduId); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
SOMEIPTP_DUMMY_STATEMENT_CONST(PduInfoPtr);
#endif
} /* SomeIpTp_RxIndication */

/***********************************************************************************************************************
 * SomeIpTp_MainFunctionRx()
 ***********************************************************************************************************************/
/*! 
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, SOMEIPTP_CODE) SomeIpTp_MainFunctionRx(void)  /* PRQA S 6080 */ /* MD_MSR_STMIF */
{
#if (SOMEIPTP_RXNSDU == STD_ON)
  /* #10 Check if SomeIpTp is initialized.*/
  if(SomeIpTp_IsInitialized() == TRUE)
  {
    /* ----- Local Variables ---------------------------------------------- */
    SomeIpTp_RXNSduIterType lSduIndex;

    for(lSduIndex = 0; lSduIndex < SomeIpTp_GetSizeOfRXNSdu(); lSduIndex++)
    {
# if (SOMEIPTP_INVALIDHNDOFRXNSDU == STD_ON)
      if (SomeIpTp_IsInvalidHndOfRXNSdu(lSduIndex) == FALSE) /* PRQA S 1881 */ /* MD_MSR_AutosarBoolean */
# endif
      {
        SomeIpTp_RxStateIdxOfRXNSduType lRxStateIdx = SomeIpTp_GetRxStateIdxOfRXNSdu(lSduIndex);
        /* #100 If initialized, Check if there is an assembly process going on for any SDU. */
        if(SomeIpTp_GetStatusOfRxState(lRxStateIdx) == SOMEIPTP_RX_STATUS_WAIT_NEXT_SF)
        {
          SchM_Enter_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
          /* PDU still not received? */
          if(SomeIpTp_GetStatusOfRxState(lRxStateIdx) == SOMEIPTP_RX_STATUS_WAIT_NEXT_SF)  /* COV_SOMEIPTP_RX_STATE_ALWAYS_TRUE */
          {
            /* #1000 If RX Timeout time expired, stop the current assembly process and give negative Rx Indication 
            to the upper layer. */
            if(SomeIpTp_GetTimerOfRxState(lRxStateIdx) != 0u)
            {
              SomeIpTp_DecTimerOfRxState(lRxStateIdx);  /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
              SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
            }
            else /* Rx Indication Timed-out */
            {
              /* \trace CREQ-169363 */
              SomeIpTp_SetStatusOfRxState(lRxStateIdx, SOMEIPTP_RX_STATUS_IDLE);  /* SBSW_SOMEIPTP_WRT_CSL03_RXNSDU */
              SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
              SomeIpTp_ReportRuntimeDet(SOMEIPTP_SID_MAINFUNCTION_RX, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
              PduR_SomeIpTpRxIndication(SomeIpTp_GetUpperLayerIdOfRXNSdu(lSduIndex), E_NOT_OK);
            }
          }
          else
          {
            SchM_Exit_SomeIpTp_SOMEIPTP_EXCLUSIVE_AREA_0();
          }
        }
      }
    }
  }
#endif
} /* SomeIpTp_MainFunctionRx */

# define SOMEIPTP_STOP_SEC_CODE
# include "SomeIpTp_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:

MD_SomeIpTp_3415: The right hand operand of '&&' or '||' has side effects.
  Reason:     Function call in right hand operator does not have any side effects.
  Risk:       None.
  Prevention: Covered by code review.

*/
/* *INDENT-OFF* */

/* SBSW_JUSTIFICATION_BEGIN
  \ID SBSW_SOMEIPTP_WRT_CSL01_TXQUEUEELEMENT
    \DESCRIPTION    Access of SomeIpTp_TxQueueElement using ComStackLib.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

  \ID SBSW_SOMEIPTP_WRT_CSL01_TXSTATE
    \DESCRIPTION    Access of SomeIpTp_TxState using ComStackLib.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

    \ID SBSW_SOMEIPTP_WRT_CSL01_RXSTATE
    \DESCRIPTION    Access of SomeIpTp_RxState using ComStackLib.
    \COUNTERMEASURE \N Qualified use-case CSL01 of ComStackLib.

  \ID SBSW_SOMEIPTP_WRT_CSL03_TXNSDU
    \DESCRIPTION    Access to SomeIpTp_TxState or SomeIpTp_TxShortHeaders or SomeIpTp_TxMetadata via indirection over
                    SomeIpTp_TXNSdu.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib.

  \ID SBSW_SOMEIPTP_WRT_CSL03_RXNSDU
    \DESCRIPTION    Access to SomeIpTp_RxState or SomeIpTp_RxShortHeaders or SomeIpTp_RxMetadata via indirection over
                    SomeIpTp_RXNSdu.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib.

  \ID SBSW_SOMEIPTP_CALL_INTERNAL_RCHECK
    \DESCRIPTION    An internal function with constraints on its parameters is called.
    \COUNTERMEASURE \R Runtime checks ensure the constraints on the parameters of the called function are satisfied.

  \ID SBSW_SOMEIPTP_WRT_LONGHEADER
    \DESCRIPTION    Write access to the buffer referenced by SduDataPtr within SomeIpTp_WriteTxLongHeader to write a
                    complete SOME/IP TP header.
    \COUNTERMEASURE \N The caller ensures the validity of the buffer referenced by SduDataPtr.

  \ID SBSW_SOMEIPTP_WRT_MSGTYPE
    \DESCRIPTION    Write access to the buffer referenced by PduInfoPtr within SomeIpTp_StartNewRxSession to set the
                    message type field of the SOME/IP header.
    \COUNTERMEASURE \N The caller ensures the validity of the buffer referenced by PduInfoPtr.

  \ID SBSW_SOMEIPTP_CALL_STARTOFRECEPTION
    \DESCRIPTION    PduR_SomeIpTpStartOfReception is called with a pointer to PduInfoType (info) and a pointer to
                    PduLengthType (bufferSizePtr). If metadata is is not used, info->SduDataPtr is set to null (as
                    specified). The passed bufferSizePtr points to a local variable and can be assumed to be valid.
    \COUNTERMEASURE \R If metadata is used, runtime checks ensure the buffer referenced by the passed info->SduDataPtr
                       is at least as long as the passed info->SduLength.
 
 \ID SBSW_SOMEIPTP_CALL_COPYRXDATA
    \DESCRIPTION    PduR_SomeIpTpCopyRxData is called with a pointer to PduInfoType (info) and a pointer to 
                    PduLengthType (bufferSizePtr). The passed bufferSizePtr points to a local variable and can be
                    assumed to be valid.
    \COUNTERMEASURE \R [CM_SOMEIPTP_PDUINFO]
  
  \ID SBSW_SOMEIPTP_CALL_TRANSMIT
    \DESCRIPTION    PduR_SomeIpTpTransmit is called with a pointer to PduInfoType (info). If metadata is not used,
                    info->SduDataPtr is set to null (as expected by the lower layer).
    \COUNTERMEASURE \N If metadata is used, the buffer referenced by the passed info->SduDataPtr is obtained via
                       qualified use-case CSL03 of ComStackLib. 
  
  \ID SBSW_SOMEIPTP_CALL_COPYTXDATA
    \DESCRIPTION    PduR_SomeIpTpCopyTxData is called with a pointer to PduInfoType (info), a pointer to RetryInfoType
                    (retry) and a pointer to PduLengthType (availableDataPtr). The retry pointer is always set to null
                    (as specifed). The passed availableDataPtr points to a local variable and can be assumed to be
                    valid.
    \COUNTERMEASURE \R [CM_SOMEIPTP_PDUINFO]
  
 \ID SBSW_SOMEIPTP_CALL_ISSDUPTRVALID
    \DESCRIPTION    SomeIpTp_IsSduPtrValid is called to check the validity of a pointer to PduInfoType (PduInfoPtr).
    \COUNTERMEASURE \N No write access takes place within SomeIpTp_IsSduPtrValid.

  \ID SBSW_SOMEIPTP_WRT_STRUCT
    \DESCRIPTION    Write access to non-buffer struct members using a passed pointer to a struct. 
    \COUNTERMEASURE \N The caller ensures the validity of the struct referenced by the pointer.

  \ID SBSW_SOMEIPTP_WRT_MORESEGMENTS
    \DESCRIPTION    Write access to a variable using a passed pointer. 
    \COUNTERMEASURE \N The caller ensures the validity of the variable referenced by the pointer.
  
SBSW_JUSTIFICATION_END */

/* ------------------------------------------------------------------------------------------------------------------ */
/* Silent BSW Countermeasures                                                                                         */
/* ------------------------------------------------------------------------------------------------------------------ */
/*
--- MSSV Plugin ---
\CM CM_SOMEIPTP_PRECOMPILEOPTIONS To ensure that all mandatory / excluded features are as expected, the following must be verified by MSSV:
                                  - SOMEIPTP_DEV_ERROR_DETECT = STD_ON
                                  - SOMEIPTP_SHORT_HEADER_LENGTH = 8
                                  - SOMEIPTP_LONG_HEADER_LENGTH = 12

\CM CM_SOMEIPTP_PDUINFO Runtime checks ensure the buffer referenced by the passed info->SduDataPtr is at least as long
                        as the passed info->SduLength.
*/

/* ------------------------------------------------------------------------------------------------------------------ */
/* Coverage Justifications                                                                                            */
/* ------------------------------------------------------------------------------------------------------------------ */

/* COV_JUSTIFICATION_BEGIN
 *
--- Preprocessor Coverage Justifications ------------------------------------------------------------------------------
\ID COV_SOMEIPTP_DET_CHECK
  \ACCEPT TX
  \REASON DET has to be enabled for SafeBSW. A MSSV Plugin enforces this configuration.

--- Condition Coverage Justifications ---------------------------------------------------------------------------------
\ID COV_SOMEIPTP_RX_STATE_ALWAYS_TRUE
    \ACCEPT TX
    \REASON This code block cannot be interrupted within test environment and this check is added for robustness.
            Therefore, the condition is always true.

COV_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  END OF FILE: SomeIpTp.c
 **********************************************************************************************************************/
