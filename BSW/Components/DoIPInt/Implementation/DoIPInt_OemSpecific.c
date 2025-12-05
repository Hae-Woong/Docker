/**********************************************************************************************************************
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
/*!        \file  DoIPInt_OemSpecific.c
 *        \brief  DoIPInt source file for sub-module DoIPInt_OemSpecific.
 *      \details  Implementation for sub-module DoIPInt_OemSpecific of component DoIPInt.
 *                Sub-module implements handling of OEM specific frames for transmission and reception context.
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

#define DOIPINT_OEMSPECIFIC_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "DoIPInt.h"
#include "DoIPInt_Tx.h"
#include "DoIPInt_Rx.h"
#include "DoIPInt_Priv.h"
#include "DoIPInt_OemSpecific.h"
#include "DoIPInt_Event.h"
#include "DoIPInt_Util.h"

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DOIPINT_OEM_SPECIFIC_HDR_LEN                    (8u)       /*!< Header length of OEM specific messages. */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# if !defined (DOIPINT_LOCAL)
#  define DOIPINT_LOCAL static
# endif /* !defined (DOIPINT_LOCAL) */

# if !defined (DOIPINT_LOCAL_INLINE)
#  define DOIPINT_LOCAL_INLINE LOCAL_INLINE
# endif /* !defined (DOIPINT_LOCAL_INLINE) */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Type definitions for DoIPInt_MsgHandlerRxStateType of OEM specific unit */
# define DOIPINT_OEM_SPECIFIC_RX_STATE_NONE              (0u)  /*!< Parsing not started. */
# define DOIPINT_OEM_SPECIFIC_RX_STATE_FIND_BUFFER       (1u)  /*!< Find reception buffer for message. */
# define DOIPINT_OEM_SPECIFIC_RX_STATE_FWD_USER_DATA     (2u)  /*!< Forward the user data. */

/*! Finding level type for the OEM specific buffer. */
typedef uint8 DoIpInt_OemSpecificBufFindingLevelType;
# define DOIPINT_OEM_SPECIFIC_FIND_LVL_NONE              (0u)  /*!< No buffer exists at all for the connection. */
# define DOIPINT_OEM_SPECIFIC_FIND_LVL_BUF_TOO_SMALL     (1u)  /*!< The configured buffers are too small. */
# define DOIPINT_OEM_SPECIFIC_FIND_LVL_BUF_IN_USE        (2u)  /*!< A suiting buffer exists but it is still in use. */
# define DOIPINT_OEM_SPECIFIC_FIND_LVL_BUF_FOUND         (3u)  /*!< A suiting buffer was found and reserved. */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define DOIPINT_START_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_TxAddBuf()
 *********************************************************************************************************************/
/*! \brief        Adds a transmission request to the transmission OEM buffer for a connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    PayloadType             The payload type that shall be used for the message.
 *  \param[out]   OemTxBufCfgIdxPtr       Pointer to index of reserved OEM Tx Buffer Configuration.
 *                                        [range: OemTxBufCfgIdxPtr != NULL_PTR]
 *  \param[in]    PayloadDataPtr          PDU information structure which contains the length and the data of the
 *                                        message.
 *                                        [range: PayloadDataPtr != NULL_PTR]
 *  \return       E_OK                    Transmission data added to buffer.
 *  \return       E_NOT_OK                Transmission data could not be added to buffer.
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires OemTxBufCfgIdxPtr != NULL_PTR;
 *    requires PayloadDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_OemSpecific_TxAddBuf(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint16 PayloadType,
  P2VAR(DoIPInt_SizeOfOemPayloadTxBufConfigType, AUTOMATIC, DOIPINT_VAR) OemTxBufCfgIdxPtr,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PayloadDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_TxReleaseBuf()
 *********************************************************************************************************************/
/*! \brief        Releases a previously reserved OEM transmission buffer on a connection.
 *  \details      -
 *  \param[in]    ConnectionIdx           Connection index.
 *                                        [range: ConnectionIdx < DoIPInt_GetSizeOfConnection()]
 *  \param[in]    OemTxBufCfgIdx          OEM Tx Buffer Configuration index.
 *                                        [range: OemTxBufCfgIdx < DoIPInt_GetSizeOfOemPayloadTxBufConfig()]
 *  \pre          -
 *  \context      TASK|ISR2
 *  \reentrant    TRUE
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires OemTxBufCfgIdx < DoIPInt_GetSizeOfOemPayloadTxBufConfig();
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_TxReleaseBuf(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx);

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxFindBuffer()
 *********************************************************************************************************************/
/*! \brief        Finds a suiting OEM Rx buffer.
 *  \details      -
 *  \param[in]    ConnectionIdx               Connection index.
 *                                            [range: ConnectionIdx < DoIPInt_GetSizeOfConnection(),
 *                                                    DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx)]
 *  \param[in]    UserDataLength              User data length.
 *  \param[out]   AvailableDataPtr            Indicates the remaining number of bytes that are available to be copied.
 *                                            [range: AvailableDataPtr != NULL_PTR]
 *  \return       DOIPINT_MSG_HANDLER_PENDING Message handling will be continued.
 *  \return       DOIPINT_MSG_HANDLER_SKIP    Message will be skipped.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionIdx, TRUE otherwise
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx);
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_OemSpecific_RxFindBuffer(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  PduLengthType UserDataLength,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxCopyPayloadData()
 *********************************************************************************************************************/
/*! \brief       Copies the received payload data.
 *  \details     -
 *  \param[in]   ConnectionIdx                Connection index.
 *                                            [range: ConnectionIdx < DoIPInt_GetSizeOfConnection(),
 *                                                    DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx)]
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
 *  \spec
 *    requires ConnectionIdx < DoIPInt_GetSizeOfConnection();
 *    requires DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx);
 *    requires PduInfoPtr != NULL_PTR;
 *    requires AvailableDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_OemSpecific_RxCopyPayloadData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 PayloadLength,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_TxAddBuf()
 *********************************************************************************************************************/
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
DOIPINT_LOCAL_INLINE FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_OemSpecific_TxAddBuf(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint16 PayloadType,
  P2VAR(DoIPInt_SizeOfOemPayloadTxBufConfigType, AUTOMATIC, DOIPINT_VAR) OemTxBufCfgIdxPtr,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PayloadDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent buffer modifications from different context. */
  DOIPINT_BEGIN_CRITICAL_SECTION();

  /* #20 Check if an OEM specific configuration is used for the connection. */
  if ( DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx) )
  {
    const DoIPInt_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIPInt_GetOemSpecificCfgIdxOfConnection(ConnectionIdx);
    DoIPInt_OemPayloadTxBufConfigIterType  oemTxBufCfgIter;

    /* #30 Search for an OEM Payload Tx buffer which is available for transmission. */
    for ( oemTxBufCfgIter = DoIPInt_GetOemPayloadTxBufConfigIndStartIdxOfOemSpecificCfg(oemSpecificCfgIdx);
      oemTxBufCfgIter < DoIPInt_GetOemPayloadTxBufConfigIndEndIdxOfOemSpecificCfg(oemSpecificCfgIdx);
      oemTxBufCfgIter++ )
    {
      const DoIPInt_SizeOfOemPayloadTxBufConfigType oemTxBufCfgIdx = DoIPInt_GetOemPayloadTxBufConfigInd(
        oemTxBufCfgIter);

      /*@ assert oemTxBufCfgIdx < DoIPInt_GetSizeOfOemPayloadTxBufConfig(); */                                         /* VCA_DOIPINT_0_N_IND_CSL03 */
      /* #40 Ensure that the requested length does not exceed the buffer length. */
      if ( PayloadDataPtr->SduLength <=
        ((PduLengthType)DoIPInt_GetOemPayloadTxBufEndIdxOfOemPayloadTxBufConfig(oemTxBufCfgIdx) -
        (PduLengthType)DoIPInt_GetOemPayloadTxBufStartIdxOfOemPayloadTxBufConfig(oemTxBufCfgIdx)) )
      {
        /* #50 Ensure that the buffer is not yet used for any other transmission. */
        if ( !DoIPInt_IsUsedOfOemPayloadTxBufConfigDyn(oemTxBufCfgIdx) )
        {
          /* #60 Set the message length and the payload type. */
          DoIPInt_SetLengthOfOemPayloadTxBufConfigDyn(oemTxBufCfgIdx, (DoIPInt_LengthOfOemPayloadTxBufConfigDynType)
            PayloadDataPtr->SduLength);
          DoIPInt_SetPayloadTypeOfOemPayloadTxBufConfigDyn(oemTxBufCfgIdx, PayloadType);

          /* #70 Mark the buffer as used to signalize a pending transmission. */
          DoIPInt_SetUsedOfOemPayloadTxBufConfigDyn(oemTxBufCfgIdx, TRUE);

          /* #80 Copy data to OEM Payload Tx buffer if available. */
          if ( PayloadDataPtr->SduLength > 0u )
          {
            /* PRQA S 314 3 */ /* MD_MSR_VStdLibCopy */
            VStdLib_MemCpy(DOIPINT_A_P2VAR(void)DoIPInt_GetAddrOemPayloadTxBuf(                                        /* VCA_DOIPINT_MEM_CPY_POINTER_LENGTH_CHECKED */
              DoIPInt_GetOemPayloadTxBufStartIdxOfOemPayloadTxBufConfig(oemTxBufCfgIdx)),
              DOIPINT_A_P2CONST(void)PayloadDataPtr->SduDataPtr, PayloadDataPtr->SduLength);
          }

          /* #90 Return the reserved OEM Tx buffer configuration index. */
          *OemTxBufCfgIdxPtr = oemTxBufCfgIdx;

          retVal = E_OK;
          break;
        }
      }
    }
  }

  /* #100 Leave critical section. */
  DOIPINT_END_CRITICAL_SECTION();

  /*@ assert retVal != E_OK || *OemTxBufCfgIdxPtr < DoIPInt_GetSizeOfOemPayloadTxBufConfig(); */
  return retVal;
} /* DoIPInt_OemSpecific_TxAddBuf() */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_TxReleaseBuf()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
/* VCA NL SPC-03 : VCA_DOIPINT_NO_ENTRY_FUNC */
DOIPINT_LOCAL_INLINE FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_TxReleaseBuf(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Enter critical section to prevent buffer modifications from different context. */
  DOIPINT_BEGIN_CRITICAL_SECTION();

  /* #20 Check if an OEM specific configuration exists for the connection. */
  if ( DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx) )
  {
    const DoIPInt_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIPInt_GetOemSpecificCfgIdxOfConnection(ConnectionIdx);

    /* #200 Reset message length, payload type and tx offset. */
    DoIPInt_SetLengthOfOemPayloadTxBufConfigDyn(OemTxBufCfgIdx, 0u);
    DoIPInt_SetPayloadTypeOfOemPayloadTxBufConfigDyn(OemTxBufCfgIdx, 0u);
    DoIPInt_SetTxOffsetOfOemSpecificCfgDyn(oemSpecificCfgIdx, 0u);

    /* #210 Mark the buffer as not used to signalize that no transmission is pending. */
    DoIPInt_SetUsedOfOemPayloadTxBufConfigDyn(OemTxBufCfgIdx, FALSE);
  }

  /* #30 Leave critical section. */
  DOIPINT_END_CRITICAL_SECTION();
} /* DoIPInt_OemSpecific_TxReleaseBuf() */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxFindBuffer()
 *********************************************************************************************************************/
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
 */
/* VCA NL SPC-03 : VCA_DOIPINT_NO_ENTRY_FUNC */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_OemSpecific_RxFindBuffer(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  PduLengthType UserDataLength,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIpInt_OemSpecificBufFindingLevelType rxBufFindLvl = DOIPINT_OEM_SPECIFIC_FIND_LVL_NONE;
  const DoIPInt_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIPInt_GetOemSpecificCfgIdxOfConnection(ConnectionIdx);
  DoIPInt_OemPayloadRxBufConfigIterType  oemRxBufCfgIter;
  DoIPInt_MsgHndl_RetType                retVal = DOIPINT_MSG_HANDLER_PENDING;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Search for an OEM Payload Rx buffer which can be used for the reception. */
  for ( oemRxBufCfgIter = DoIPInt_GetOemPayloadRxBufConfigIndStartIdxOfOemSpecificCfg(oemSpecificCfgIdx);
    oemRxBufCfgIter < DoIPInt_GetOemPayloadRxBufConfigIndEndIdxOfOemSpecificCfg(oemSpecificCfgIdx);
    oemRxBufCfgIter++ )
  {
    /* Set the next finding level as there is at least one buffer configured for the connection. */
    const DoIPInt_SizeOfOemPayloadRxBufConfigType oemRxBufCfgIdx = DoIPInt_GetOemPayloadRxBufConfigInd(
      oemRxBufCfgIter);
    rxBufFindLvl =  DOIPINT_MAX(rxBufFindLvl, DOIPINT_OEM_SPECIFIC_FIND_LVL_BUF_TOO_SMALL);

    /* #100 Check that the buffer is big enough for the received data. */
    if ( UserDataLength <=
      ((PduLengthType)DoIPInt_GetOemPayloadRxBufEndIdxOfOemPayloadRxBufConfig(oemRxBufCfgIdx) -
        (PduLengthType)DoIPInt_GetOemPayloadRxBufStartIdxOfOemPayloadRxBufConfig(oemRxBufCfgIdx)) )
    {
      /* Set the next finding level as there is at least one buffer that is big enough for the message. */
      rxBufFindLvl = DOIPINT_OEM_SPECIFIC_FIND_LVL_BUF_IN_USE;

      /* #1000 Check that the buffer is not yet used for any other reception. */
      if ( DoIPInt_GetLengthOfOemPayloadRxBufConfigDyn(oemRxBufCfgIdx) == 0u )
      {
        /* #10000 Set the length on the buffer to mark it as used and store the buffer configuration. */
        rxBufFindLvl = DOIPINT_OEM_SPECIFIC_FIND_LVL_BUF_FOUND;
        DoIPInt_SetLengthOfOemPayloadRxBufConfigDyn(oemRxBufCfgIdx, (DoIPInt_LengthOfOemPayloadRxBufConfigDynType)     /* VCA_DOIPINT_0_N_IND_CSL03 */
          UserDataLength);
        DoIPInt_SetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(oemSpecificCfgIdx, oemRxBufCfgIdx);
        DoIPInt_SetRxStateOfOemSpecificCfgDyn(oemSpecificCfgIdx, DOIPINT_OEM_SPECIFIC_RX_STATE_FWD_USER_DATA);

        break;
      }
    }
  }

  /* #20 Check if all suitable buffers are currently in use. */
  if ( rxBufFindLvl == DOIPINT_OEM_SPECIFIC_FIND_LVL_BUF_IN_USE )
  {
    /* #200 Check timeout monitoring and skip message if timeout has expired. */
    if ( DoIPInt_Rx_CheckOrStartRetryTimeout(ConnectionIdx) == E_NOT_OK )
    {
      /* #2000 Set available buffer size to the user data length. */
      *AvailableDataPtr = UserDataLength;

      /* #2001 Report event that message is discarded. */
      retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_MSG_DISCARDED);
    }
    /* #201 Set available buffer size to indicate that no more data can be received at the moment otherwise. */
    else
    {
      *AvailableDataPtr = 0u;
    }
  }
  /* #30 Check if no suiting buffer exists otherwise. */
  else if ( rxBufFindLvl == DOIPINT_OEM_SPECIFIC_FIND_LVL_NONE )
  {
    /* #300 Set available buffer size to the user data length. */
    *AvailableDataPtr = UserDataLength;

    /* #301 Report event that message is discarded. */
    retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_MSG_DISCARDED);
  }
  /* #40 Check if the message is too large for the configured buffers otherwise. */
  else if ( rxBufFindLvl == DOIPINT_OEM_SPECIFIC_FIND_LVL_BUF_TOO_SMALL )
  {
    /* #400 Set available buffer size to the user data length. */
    *AvailableDataPtr = UserDataLength;

    /* #401 Report event that message is too large. */
    retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_MSG_TOO_LARGE_OEM);
  }
  /* #50 Set available buffer size to the user data length otherwise. */
  else
  {
    *AvailableDataPtr = UserDataLength;
  }

  return retVal;
} /* DoIPInt_OemSpecific_RxFindBuffer() */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxCopyPayloadData()
 *********************************************************************************************************************/
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
/* VCA NL SPC-03 : VCA_DOIPINT_NO_ENTRY_FUNC */
DOIPINT_LOCAL_INLINE FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_OemSpecific_RxCopyPayloadData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 PayloadLength,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIPInt_GetOemSpecificCfgIdxOfConnection(ConnectionIdx);
  uint32                                 offset = DoIPInt_GetRxOffsetForwardOfOemSpecificCfgDyn(oemSpecificCfgIdx);
  DoIPInt_MsgHndl_RetType                retVal = DOIPINT_MSG_HANDLER_PENDING;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if data to be copied does not exceed the pending user data length. */
  if ( (offset + PduInfoPtr->SduLength) <= PayloadLength )
  {
    /* #20 Copy data to buffer. */
    const DoIPInt_SizeOfOemPayloadRxBufConfigType oemRxBufCfgIdx =
      DoIPInt_GetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(oemSpecificCfgIdx);
    const DoIPInt_SizeOfOemPayloadRxBufType       oemRxBufStartIdx =
      DoIPInt_GetOemPayloadRxBufStartIdxOfOemPayloadRxBufConfig(oemRxBufCfgIdx);
    const uint32                                  eventLen = DOIPINT_MIN(DoIPInt_GetEventMaxUserDataLength(),
      PayloadLength);

    /* PRQA S 314 2 */ /* MD_MSR_VStdLibCopy */
    VStdLib_MemCpy(DOIPINT_A_P2VAR(void)DoIPInt_GetAddrOemPayloadRxBuf(oemRxBufStartIdx + offset),                     /* VCA_DOIPINT_MEM_CPY_POINTER_LENGTH_CHECKED */
      DOIPINT_A_P2CONST(void)PduInfoPtr->SduDataPtr, PduInfoPtr->SduLength);

    /* #30 Report event if data is now received up to the requested length. */
    if ( (offset <= eventLen) && ((offset + PduInfoPtr->SduLength) >= eventLen) )
    {
      retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_MSG_RECEIVED_UP_TO_LENGTH_OEM);
    }

    /* #40 Update the offset value. */
    offset += PduInfoPtr->SduLength;
    DoIPInt_SetRxOffsetForwardOfOemSpecificCfgDyn(oemSpecificCfgIdx, offset);

    /* #50 Finish reception if all data has been copied. */
    if ( offset == PayloadLength )
    {
      PduInfoType oemPayloadRxBuf;
      oemPayloadRxBuf.SduDataPtr = DoIPInt_GetAddrOemPayloadRxBuf(oemRxBufStartIdx);
      oemPayloadRxBuf.SduLength = (PduLengthType)PayloadLength;

      /* #500 Forward received data to the user via the configured callback in case there is one configured. */
      if ( DoIPInt_HasOemPayloadRxCbk() )
      {
        DoIPInt_GetOemPayloadRxCbk(0u)(DoIPInt_GetConnectionIdOfConnection(ConnectionIdx),                             /* VCA_DOIPINT_OPTIONAL_FUNCTION_PTR */ /* VCA_DOIPINT_CALL_EXTERNAL_FUNCTION_CONST_POINTER_ARGUMENT */
          PayloadType,
          DOIPINT_A_P2CONST(PduInfoType)&oemPayloadRxBuf);
      }

      /* #501 Indicate that message handling is finished. */
      retVal = DOIPINT_MSG_HANDLER_FINISHED;
    }
    /* #51 Prepare to continue reception otherwise. */
    else
    {
      /* #510 Set available buffer size to the remaining data. */
      *AvailableDataPtr = (PduLengthType)(PayloadLength - offset);
    }
  }
  /* #11 Indicate that message handling failed otherwise. */
  else
  {
    retVal = DOIPINT_MSG_HANDLER_FAILED;
  }

  return retVal;
} /* DoIPInt_OemSpecific_RxCopyPayloadData() */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_Init(
  DoIPInt_ConnectionIterType ConnectionIter)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if an OEM specific configuration exists for the connection. */
  if ( DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIter) )
  {
    DoIPInt_OemPayloadRxBufConfigIterType  oemRxBufCfgIter;
    DoIPInt_OemPayloadTxBufConfigIterType  oemTxBufCfgIter;
    const DoIPInt_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIPInt_GetOemSpecificCfgIdxOfConnection(
      ConnectionIter);

    /* #20 Iterate over all OEM specific Rx buffers of the connection to initialize them. */
    for ( oemRxBufCfgIter = DoIPInt_GetOemPayloadRxBufConfigIndStartIdxOfOemSpecificCfg(oemSpecificCfgIdx);
      oemRxBufCfgIter < DoIPInt_GetOemPayloadRxBufConfigIndEndIdxOfOemSpecificCfg(oemSpecificCfgIdx);
      oemRxBufCfgIter++ )
    {
      DoIPInt_SizeOfOemPayloadRxBufConfigType oemRxBufCfgIdx = DoIPInt_GetOemPayloadRxBufConfigInd(oemRxBufCfgIter);
      /*@ assert oemRxBufCfgIdx < DoIPInt_GetSizeOfOemPayloadRxBufConfig(); */                                         /* VCA_DOIPINT_0_N_IND_CSL03 */
      DoIPInt_SetLengthOfOemPayloadRxBufConfigDyn(oemRxBufCfgIdx, 0u);
    }

    /* #30 Iterate over all OEM specific Tx buffers of the connection to initialize them. */
    for ( oemTxBufCfgIter = DoIPInt_GetOemPayloadTxBufConfigIndStartIdxOfOemSpecificCfg(oemSpecificCfgIdx);
      oemTxBufCfgIter < DoIPInt_GetOemPayloadTxBufConfigIndEndIdxOfOemSpecificCfg(oemSpecificCfgIdx);
      oemTxBufCfgIter++ )
    {
      DoIPInt_SizeOfOemPayloadTxBufConfigType oemTxBufCfgIdx = DoIPInt_GetOemPayloadTxBufConfigInd(oemTxBufCfgIter);
      /*@ assert oemTxBufCfgIdx < DoIPInt_GetSizeOfOemPayloadTxBufConfig(); */                                         /* VCA_DOIPINT_0_N_IND_CSL03 */
      DoIPInt_SetLengthOfOemPayloadTxBufConfigDyn(oemTxBufCfgIdx, 0u);
      DoIPInt_SetPayloadTypeOfOemPayloadTxBufConfigDyn(oemTxBufCfgIdx, 0u);
      DoIPInt_SetUsedOfOemPayloadTxBufConfigDyn(oemTxBufCfgIdx, FALSE);
    }

    /* #40 Initialize the transmission offset. */
    DoIPInt_SetTxOffsetOfOemSpecificCfgDyn(oemSpecificCfgIdx, 0u);

    /* #50 Initialize the OEM specific reception parameter. */
    DoIPInt_OemSpecific_RxStopParse((DoIPInt_SizeOfConnectionType)ConnectionIter);
  }
} /* DoIPInt_OemSpecific_Init() */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_TxTransmit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DOIPINT_CODE) DoIPInt_OemSpecific_TxTransmit(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PayloadDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_SizeOfOemPayloadTxBufConfigType oemTxBufCfgIdx = DoIPInt_GetSizeOfOemPayloadTxBufConfig();
  Std_ReturnType                          retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Try to add the transmission request to the transmission buffer. */
  if ( DoIPInt_OemSpecific_TxAddBuf(ConnectionIdx, PayloadType, &oemTxBufCfgIdx, PayloadDataPtr) == E_OK )
  {
    /* #20 Check if transmission can be prepared (connection is open and queue element is available). */
    if ( DoIPInt_Tx_PrepareTransmit(ConnectionIdx, oemTxBufCfgIdx, PayloadDataPtr->SduLength,
        DoIPInt_GetMsgHandlerIdxOfOemSpecificMsgHandler(0u)) == E_OK )
    {
      /* #30 Try to transmit from queue in case preparing transmission was successful. */
      (void)DoIPInt_Tx_TransmitFromQueue(ConnectionIdx);
      retVal = E_OK;
    }
    else
    {
      /* #31 Release the buffer otherwise. */
      /*@ assert oemTxBufCfgIdx < DoIPInt_GetSizeOfOemPayloadTxBufConfig(); */                                         /* VCA_DOIPINT_VALID_IDX_DERIVED_BY_FUNC */
      DoIPInt_OemSpecific_TxReleaseBuf(ConnectionIdx, oemTxBufCfgIdx);
    }
  }

  return retVal;
} /* DoIPInt_OemSpecific_TxTransmit() */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_TxCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
/* VCA NL SPC-03 : VCA_DOIPINT_NO_ENTRY_FUNC */
FUNC(BufReq_ReturnType, DOIPINT_CODE) DoIPInt_OemSpecific_TxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if an OEM specific configuration exists for the connection. */
  if ( DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx) )
  {
    const DoIPInt_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIPInt_GetOemSpecificCfgIdxOfConnection(ConnectionIdx);
    const PduLengthType                    txMsgLen = DoIPInt_GetLengthOfOemPayloadTxBufConfigDyn(OemTxBufCfgIdx);
    uint32                                 offset = DoIPInt_GetTxOffsetOfOemSpecificCfgDyn(oemSpecificCfgIdx);

    /* #20 Ensure that the user data shall be copied inside a valid range. */
    if ( (offset + PduInfoPtr->SduLength) <= txMsgLen )
    {
      /* #30 Check if data shall be copied. */
      if ( PduInfoPtr->SduLength != 0u )
      {
        const PduLengthType oemTxBufIdx = (PduLengthType)(DoIPInt_GetOemPayloadTxBufStartIdxOfOemPayloadTxBufConfig(
          OemTxBufCfgIdx) + offset);

        /* #40 Copy data from internal buffer to target buffer. */
        /*@ assert $external(PduInfoPtr->SduDataPtr); */                                                               /* VCA_DOIPINT_MEM_CPY_EXT_POINTER_LENGTH_VALID */
        /* PRQA S 314 2 */ /* MD_MSR_VStdLibCopy */
        VStdLib_MemCpy(DOIPINT_A_P2VAR(void)PduInfoPtr->SduDataPtr,                                                    /* VCA_DOIPINT_MEM_CPY_EXT_POINTER_LENGTH_VALID */
          DOIPINT_A_P2CONST(void)(DoIPInt_GetAddrOemPayloadTxBuf(oemTxBufIdx)),
          (uint32)PduInfoPtr->SduLength);

        /* #50 Update the current offset in the buffer. */
        offset += PduInfoPtr->SduLength;
        DoIPInt_SetTxOffsetOfOemSpecificCfgDyn(oemSpecificCfgIdx, offset);
      }
      /* #60 Set available data length. */
      *AvailableDataPtr = (PduLengthType)(txMsgLen - offset);

      retVal = BUFREQ_OK;
    }
  }

  return retVal;
} /* DoIPInt_OemSpecific_TxCopy() */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_TxConfirmation()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 */
/* VCA NL SPC-03 : VCA_DOIPINT_NO_ENTRY_FUNC */
FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_TxConfirmation(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  DoIPInt_SizeOfOemPayloadTxBufConfigType OemTxBufCfgIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Trigger release of the reserved buffer. */
  DoIPInt_OemSpecific_TxReleaseBuf(ConnectionIdx, OemTxBufCfgIdx);
} /* DoIPInt_OemSpecific_TxConfirmation() */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxCopy()
 *********************************************************************************************************************/
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
 */
FUNC(DoIPInt_MsgHndl_RetType, DOIPINT_CODE) DoIPInt_OemSpecific_RxCopy(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  uint32 PayloadLength,
  uint16 PayloadType,
  P2CONST(PduInfoType, AUTOMATIC, DOIPINT_VAR) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) AvailableDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  DoIPInt_MsgHndl_RetType retVal = DOIPINT_MSG_HANDLER_FAILED;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if OEM specific configuration is used for the connection. */
  if ( DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx) )
  {
    const DoIPInt_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIPInt_GetOemSpecificCfgIdxOfConnection(ConnectionIdx);
    DoIPInt_MsgHandlerRxStateType          lastMsgHandlerRxState;

    /* #20 Process state machine until message is handled completely (i.e. no state transition anymore). */
    do
    {
      lastMsgHandlerRxState = DoIPInt_GetRxStateOfOemSpecificCfgDyn(oemSpecificCfgIdx);

      /* #30 Handle parse process depending on state. */
      switch ( lastMsgHandlerRxState )
      {
        /* #40 Find reception buffer. */
        case DOIPINT_OEM_SPECIFIC_RX_STATE_FIND_BUFFER:
        {
          retVal = DoIPInt_OemSpecific_RxFindBuffer(ConnectionIdx, (PduLengthType)PayloadLength,
            AvailableDataPtr);
          break;
        }
        /* #50 Copy the data to the Rx buffer and forward it. */
        case DOIPINT_OEM_SPECIFIC_RX_STATE_FWD_USER_DATA:
        {
          retVal = DoIPInt_OemSpecific_RxCopyPayloadData(ConnectionIdx, PayloadLength, PayloadType, PduInfoPtr,
            AvailableDataPtr);
          break;
        }
        /* #60 Return error if message handling is not active. */
        default: /* DOIPINT_OEM_SPECIFIC_RX_STATE_NONE */
        {
          retVal = DOIPINT_MSG_HANDLER_FAILED;
          break;
        }
      }
    } while ( DoIPInt_GetRxStateOfOemSpecificCfgDyn(oemSpecificCfgIdx) != lastMsgHandlerRxState );
  }
  /* #11 Report event that message is discarded otherwise. */
  else
  {
    retVal = DoIPInt_Event_Report(ConnectionIdx, DOIPINT_INTERNAL_EVENT_MSG_DISCARDED);

    /* #110 Set available buffer size to the payload length. */
    *AvailableDataPtr = (PduLengthType)PayloadLength;
  }

  return retVal;
} /* DoIPInt_OemSpecific_RxCopy() */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxGetParsedData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* VCA NL SPC-03 : VCA_DOIPINT_NO_ENTRY_FUNC */
FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_RxGetParsedData(
  DoIPInt_SizeOfConnectionType ConnectionIdx,
  PduLengthType DestBufLength,
  P2VAR(uint8, AUTOMATIC, DOIPINT_VAR) DestBufPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIPINT_VAR) MsgLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  const DoIPInt_SizeOfOemSpecificCfgType        oemSpecificCfgIdx = DoIPInt_GetOemSpecificCfgIdxOfConnection(
    ConnectionIdx);
  const DoIPInt_SizeOfOemPayloadRxBufConfigType oemRxBufCfgIdx =
    DoIPInt_GetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(oemSpecificCfgIdx);
  const DoIPInt_SizeOfOemPayloadRxBufType       oemRxBufStartIdx =
    DoIPInt_GetOemPayloadRxBufStartIdxOfOemPayloadRxBufConfig(oemRxBufCfgIdx);
  const uint32                                  oemBufLen = DOIPINT_MIN(DoIPInt_GetEventMaxUserDataLength(),
    DoIPInt_GetLengthOfOemPayloadRxBufConfigDyn(oemRxBufCfgIdx));
  uint32                                        lengthToCopy = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Limit the length of the data to be copied to the length of the destination buffer. */
  if ( oemBufLen > DestBufLength )
  {
    lengthToCopy = DestBufLength;
  }
  else
  {
    lengthToCopy = oemBufLen;
  }

  /* #20 Get the parsed data. */
  /* PRQA S 314 2 */ /* MD_MSR_VStdLibCopy */
  VStdLib_MemCpy((void*)DestBufPtr,                                                                                    /* VCA_DOIPINT_MEM_CPY_FIX_LENGTH */
    DOIPINT_A_P2VAR(void)DoIPInt_GetAddrOemPayloadRxBuf(oemRxBufStartIdx),
    lengthToCopy);

  *MsgLengthPtr = (PduLengthType)(lengthToCopy + *MsgLengthPtr);
} /* DoIPInt_OemSpecific_RxGetParsedData() */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxStartParse()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_RxStartParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Stop parsing. */
  DoIPInt_OemSpecific_RxStopParse(ConnectionIdx);

  /* #20 Set state to indicate that parsing has been started if an OEM specific configuration exists. */
  if ( DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx) )
  {
    DoIPInt_SetRxStateOfOemSpecificCfgDyn(DoIPInt_GetOemSpecificCfgIdxOfConnection(ConnectionIdx),
      DOIPINT_OEM_SPECIFIC_RX_STATE_FIND_BUFFER);
  }
} /* DoIPInt_OemSpecific_RxStartParse() */

/**********************************************************************************************************************
 *  DoIPInt_OemSpecific_RxStopParse()
 *********************************************************************************************************************/
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
 */
FUNC(void, DOIPINT_CODE) DoIPInt_OemSpecific_RxStopParse(
  DoIPInt_SizeOfConnectionType ConnectionIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if an OEM specific configuration exists for the connection. */
  if ( DoIPInt_IsOemSpecificCfgUsedOfConnection(ConnectionIdx) )
  {
    const DoIPInt_SizeOfOemSpecificCfgType oemSpecificCfgIdx = DoIPInt_GetOemSpecificCfgIdxOfConnection(ConnectionIdx);

    /* #20 Check if OEM specific reception is enabled for the connection. */
    if ( DoIPInt_IsOemPayloadRxBufConfigIndUsedOfOemSpecificCfg(oemSpecificCfgIdx) )
    {
      const DoIPInt_SizeOfOemPayloadRxBufConfigType oemRxBufCfgIdx =
        DoIPInt_GetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(oemSpecificCfgIdx);

      /* #30 Reset length of the reception buffer so it can be reused for further receptions. */
      if ( oemRxBufCfgIdx < DoIPInt_GetSizeOfOemPayloadRxBufConfig() )
      {
        DoIPInt_SetLengthOfOemPayloadRxBufConfigDyn(oemRxBufCfgIdx, 0u);
      }

      /* #40 Reset offset for data forwarding. */
      DoIPInt_SetRxOffsetForwardOfOemSpecificCfgDyn(oemSpecificCfgIdx, 0u);

      /* #50 Reset related OEM payload buffer index. */
      DoIPInt_SetOemPayloadRxBufConfigIdxOfOemSpecificCfgDyn(oemSpecificCfgIdx,
        DoIPInt_GetSizeOfOemPayloadRxBufConfig());

      /* #60 Reset state. */
      DoIPInt_SetRxStateOfOemSpecificCfgDyn(oemSpecificCfgIdx, DOIPINT_OEM_SPECIFIC_RX_STATE_NONE);

      /* #70 Reset the retry timeout. */
      DoIPInt_Rx_ResetRetryTimeout(ConnectionIdx);
    }
  }
} /* DoIPInt_OemSpecific_RxStopParse() */

# define DOIPINT_STOP_SEC_CODE
# include "DoIPInt_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

/**********************************************************************************************************************
 *  END OF FILE: DoIPInt_OemSpecific.c
 *********************************************************************************************************************/
