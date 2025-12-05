/**********************************************************************************************************************
 *  FILE REQUIRES USER MODIFICATIONS
 *  Template Scope: whole file
 *  -------------------------------------------------------------------------------------------------------------------
 *  This file includes template code that must be completed and/or adapted during BSW integration. It is neither
 *  intended nor qualified for use in series production without applying suitable quality measures. The template code
 *  must be completed as described in the instructions given within this file and/or in the TechnicalReference. The
 *  completed implementation must be tested with diligent care and must comply with all quality requirements which are
 *  necessary according to the state of the art before its use.
 *********************************************************************************************************************/

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
/*!        \file  Appl_DoIPInt.c
 *        \brief  DoIPInt application template source file
 *      \details  Template implementation file for DoIPInt application to be completed by the user of MICROSAR4.
 *********************************************************************************************************************/

#define APPLDOIPINT_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "DoIPInt.h"
#include "Appl_DoIPInt.h"

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
/*! Maximum supported user data length of CAN TP. */
uint32 Appl_DoIPInt_CanTpMaxLen = 4095u; /* PRQA S 1514, 3408 */ /* MD_DoIPInt_ObjectWithExtLinkOnlyAccessedOnce */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_DoIPInt_Event()
 *********************************************************************************************************************/
/* PRQA S 6030 1 */ /* MD_MSR_STCYC */
Std_ReturnType Appl_DoIPInt_Event(
  DoIPInt_ConnectionIdType ConnectionId,
  DoIPInt_EventType Event,
  const PduInfoType* MsgDataPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  switch ( Event )
  {
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    case DOIPINT_EVENT_MSG_INV_PATTERN:
    {
      PduInfoType responseInfo;
      uint8 responseCode = 0x00u; /* Generic header NACK code "incorrect pattern format". */

      responseInfo.SduDataPtr = &responseCode;
      responseInfo.SduLength = 1u;

      /* Transmit a generic header negative acknowledge message. */
      (void)DoIPInt_TransmitOemSpecificPayloadType(ConnectionId, 0x0000u, &responseInfo);

      /* Close connection as specified by ISO. */
      (void)DoIPInt_CloseConnection(ConnectionId, FALSE);

      /* Indicate to handle this event pessimistic (i.e. discard message). */
      retVal = E_NOT_OK;

      break;
    }
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
    case DOIPINT_EVENT_MSG_TOO_LARGE:
    {
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      PduInfoType responseInfo;
      uint8 responseCode = 0x02u; /* Generic header NACK code "message too large". */

      responseInfo.SduDataPtr = &responseCode;
      responseInfo.SduLength = 1u;

      /* Transmit a generic header negative acknowledge message. */
      (void)DoIPInt_TransmitOemSpecificPayloadType(ConnectionId, 0x0000u, &responseInfo);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

      /* Indicate to handle this event pessimistic (i.e. discard message). */
      retVal = E_NOT_OK;

      break;
    }
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    case DOIPINT_EVENT_MSG_INV_LENGTH:
    {
      PduInfoType responseInfo;
      uint8 responseCode = 0x04u; /* Generic header NACK code "invalid payload length". */

      responseInfo.SduDataPtr = &responseCode;
      responseInfo.SduLength = 1u;

      /* Transmit a generic header negative acknowledge message. */
      (void)DoIPInt_TransmitOemSpecificPayloadType(ConnectionId, 0x0000u, &responseInfo);

      /* Close connection as specified by ISO. */
      (void)DoIPInt_CloseConnection(ConnectionId, FALSE);

      /* Indicate to handle this event pessimistic (i.e. discard message). */
      retVal = E_NOT_OK;

      break;
    }
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
    case DOIPINT_EVENT_MSG_RECEIVED_UP_TO_LENGTH:
    {
      /* Check if it is a "diagnostic message" to ensure that the message is not too large for the target bus. For
       * other payload types DOIPINT_EVENT_MSG_TOO_LARGE is reported. */
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      if ( (MsgDataPtr->SduDataPtr[2] == 0x80u) && (MsgDataPtr->SduDataPtr[3] == 0x01u) )
      {
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

        uint32 payloadLength = IpBase_GetUint32(MsgDataPtr->SduDataPtr, 4u);

        /* Check if length exceeds CanTp max length + DoIP SA and TA. */
        if ( payloadLength > ((uint64)Appl_DoIPInt_CanTpMaxLen + (uint64)4u) )
        {
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
          /* Prepare to transmit a diagnostic message negative acknowledge with NACK code "Diagnostic message too
           * large". */
          PduInfoType responseInfo;
          uint8 response[7];
          uint8 responseLength = 5u;

          response[0] = MsgDataPtr->SduDataPtr[8 + 2];    /* Use TA as SA. */
          response[1] = MsgDataPtr->SduDataPtr[8 + 3];    /* Use TA as SA. */
          response[2] = MsgDataPtr->SduDataPtr[8 + 0];    /* Use SA as TA. */
          response[3] = MsgDataPtr->SduDataPtr[8 + 1];    /* Use SA as TA. */
          response[4] = 0x04u;                            /* NACK code "Diagnostic message too large". */

          /* Mirror back up to 2 Bytes of the previously received user data. */
          if ( MsgDataPtr->SduLength >= 13u )
          {
            response[5] = MsgDataPtr->SduDataPtr[8 + 4];
            responseLength++;
          }
          if ( MsgDataPtr->SduLength >= 14u )
          {
            response[6] = MsgDataPtr->SduDataPtr[8 + 5];
            responseLength++;
          }

          responseInfo.SduDataPtr = response;
          responseInfo.SduLength = responseLength;

          /* Transmit the diagnostic message negative acknowledge. */
          (void)DoIPInt_TransmitOemSpecificPayloadType(ConnectionId, 0x8003u, &responseInfo);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

          /* Indicate to handle this event pessimistic (i.e. discard message). */
          retVal = E_NOT_OK;
        }
        else
        {
          /* Indicate to handle this event optimistic (i.e. continue reception). */
          retVal = E_OK;
        }
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      }
      else
      {
        /* Indicate to handle this event optimistic (i.e. continue reception). */
        retVal = E_OK;
      }
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

      break;
    }
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
    case DOIPINT_EVENT_MSG_DISCARDED:
    {
      /* This example expects that the OEM specific payload type reception is disabled at all. So, this event is only
       * reported in case an unknown payload type is received. Therefore, only an "unknown payload type" NACK is sent.
       * Otherwise, this event would be called if no buffer is available. In this case an "out of memory" NACK may be
       * sent. */

      PduInfoType responseInfo;
      uint8 responseCode = 0x01u; /* Generic header NACK code "unknown payload type". */

      responseInfo.SduDataPtr = &responseCode;
      responseInfo.SduLength = 1u;

      /* Transmit a generic header negative acknowledge message. */
      (void)DoIPInt_TransmitOemSpecificPayloadType(ConnectionId, 0x0000u, &responseInfo);

      /* Indicate to handle this event pessimistic (here: not needded). */
      retVal = E_NOT_OK;

      break;
    }
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */
    case DOIPINT_EVENT_DIAG_MSG_INV_REM_ADDR:
    {
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      /* Prepare to transmit a diagnostic message negative acknowledge with NACK code "Invalid source address". */
      PduInfoType responseInfo;
      uint8 response[7];
      uint8 responseLength = 5u;

      response[0] = MsgDataPtr->SduDataPtr[8 + 2];    /* Use TA as SA. */
      response[1] = MsgDataPtr->SduDataPtr[8 + 3];    /* Use TA as SA. */
      response[2] = MsgDataPtr->SduDataPtr[8 + 0];    /* Use SA as TA. */
      response[3] = MsgDataPtr->SduDataPtr[8 + 1];    /* Use SA as TA. */
      response[4] = 0x02u;                            /* NACK code "Invalid source address". */

      /* Mirror back up to 2 Bytes of the previously received user data. */
      if ( MsgDataPtr->SduLength >= 13u )
      {
        response[5] = MsgDataPtr->SduDataPtr[8 + 4];
        responseLength++;
      }
      if ( MsgDataPtr->SduLength >= 14u )
      {
        response[6] = MsgDataPtr->SduDataPtr[8 + 5];
        responseLength++;
      }

      responseInfo.SduDataPtr = response;
      responseInfo.SduLength = responseLength;

      /* Transmit the diagnostic message negative acknowledge. */
      (void)DoIPInt_TransmitOemSpecificPayloadType(ConnectionId, 0x8003u, &responseInfo);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

      /* Close connection as specified by ISO. */
      (void)DoIPInt_CloseConnection(ConnectionId, FALSE);

      /* Indicate to handle this event pessimistic (i.e. discard message). */
      retVal = E_NOT_OK;

      break;
    }
    case DOIPINT_EVENT_DIAG_MSG_INV_LOCAL_ADDR:
    {
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      /* Prepare to transmit a diagnostic message negative acknowledge with NACK code "Unknown target address". */
      PduInfoType responseInfo;
      uint8 response[7];
      uint8 responseLength = 5u;

      response[0] = MsgDataPtr->SduDataPtr[8 + 2];    /* Use TA as SA. */
      response[1] = MsgDataPtr->SduDataPtr[8 + 3];    /* Use TA as SA. */
      response[2] = MsgDataPtr->SduDataPtr[8 + 0];    /* Use SA as TA. */
      response[3] = MsgDataPtr->SduDataPtr[8 + 1];    /* Use SA as TA. */
      response[4] = 0x03u;                            /* NACK code "Unknown target address". */

      /* Mirror back up to 2 Bytes of the previously received user data. */
      if ( MsgDataPtr->SduLength >= 13u )
      {
        response[5] = MsgDataPtr->SduDataPtr[8 + 4];
        responseLength++;
      }
      if ( MsgDataPtr->SduLength >= 14u )
      {
        response[6] = MsgDataPtr->SduDataPtr[8 + 5];
        responseLength++;
      }

      responseInfo.SduDataPtr = response;
      responseInfo.SduLength = responseLength;

      /* Transmit the diagnostic message negative acknowledge. */
      (void)DoIPInt_TransmitOemSpecificPayloadType(ConnectionId, 0x8003u, &responseInfo);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

      /* Indicate to handle this event pessimistic (i.e. discard message). */
      retVal = E_NOT_OK;

      break;
    }
    case DOIPINT_EVENT_DIAG_MSG_DISCARDED:
    {
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      /* Prepare to transmit a diagnostic message negative acknowledge with NACK code "Out of memory". */
      PduInfoType responseInfo;
      uint8 response[7];
      uint8 responseLength = 5u;

      response[0] = MsgDataPtr->SduDataPtr[8 + 2];    /* Use TA as SA. */
      response[1] = MsgDataPtr->SduDataPtr[8 + 3];    /* Use TA as SA. */
      response[2] = MsgDataPtr->SduDataPtr[8 + 0];    /* Use SA as TA. */
      response[3] = MsgDataPtr->SduDataPtr[8 + 1];    /* Use SA as TA. */
      response[4] = 0x05u;                            /* NACK code "Out of memory". */

      /* Mirror back up to 2 Bytes of the previously received user data. */
      if ( MsgDataPtr->SduLength >= 13u )
      {
        response[5] = MsgDataPtr->SduDataPtr[8 + 4];
        responseLength++;
      }
      if ( MsgDataPtr->SduLength >= 14u )
      {
        response[6] = MsgDataPtr->SduDataPtr[8 + 5];
        responseLength++;
      }

      responseInfo.SduDataPtr = response;
      responseInfo.SduLength = responseLength;

      /* Transmit the diagnostic message negative acknowledge. */
      (void)DoIPInt_TransmitOemSpecificPayloadType(ConnectionId, 0x8003u, &responseInfo);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

      /* Indicate to handle this event pessimistic (here: not needded). */
      retVal = E_NOT_OK;

      break;
    }
    case DOIPINT_EVENT_DIAG_MSG_FORWARDED:
    {
#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
      /* Prepare to transmit a diagnostic message positive acknowledge. */
      PduInfoType responseInfo;
      uint8 response[7];
      uint8 responseLength = 5u;

      response[0] = MsgDataPtr->SduDataPtr[8 + 2];    /* Use TA as SA. */
      response[1] = MsgDataPtr->SduDataPtr[8 + 3];    /* Use TA as SA. */
      response[2] = MsgDataPtr->SduDataPtr[8 + 0];    /* Use SA as TA. */
      response[3] = MsgDataPtr->SduDataPtr[8 + 1];    /* Use SA as TA. */
      response[4] = 0x00u;                            /* ACK code "Routing confirmation acknowledge". */

      /* Mirror back up to 2 Bytes of the previously received user data. */
      if ( MsgDataPtr->SduLength >= 13u )
      {
        response[5] = MsgDataPtr->SduDataPtr[8 + 4];
        responseLength++;
      }
      if ( MsgDataPtr->SduLength >= 14u )
      {
        response[6] = MsgDataPtr->SduDataPtr[8 + 5];
        responseLength++;
      }

      responseInfo.SduDataPtr = response;
      responseInfo.SduLength = responseLength;

      /* Transmit the diagnostic message positive acknowledge. */
      (void)DoIPInt_TransmitOemSpecificPayloadType(ConnectionId, 0x8002u, &responseInfo);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

      /* Indicate to handle this event optimistic (here: not needded). */
      retVal = E_OK;

      break;
    }
    default:
    {
      retVal = E_NOT_OK;

      break;
    }
  }

  return retVal;
} /* Appl_DoIPInt_Event() */

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
/**********************************************************************************************************************
 *  Appl_DoIPInt_ReceiveOemSpecificPayloadType()
 *********************************************************************************************************************/
void Appl_DoIPInt_ReceiveOemSpecificPayloadType(
  DoIPInt_ConnectionIdType ConnectionId,
  uint16 PayloadType,
  const PduInfoType* PayloadDataPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  switch ( PayloadType )
  {
    case 0x0000u: /* Generic header negative acknowledge message. */
    case 0x8002u: /* Diagnostic message positive acknowledge. */
    case 0x8003u: /* Diagnostic message negative acknowledge. */
    {
      break;
    }
    default:
    {
      PduInfoType responseInfo;
      uint8 responseCode = 0x01u; /* Generic header NACK code "unknown payload type". */

      responseInfo.SduDataPtr = &responseCode;
      responseInfo.SduLength = 1u;

      /* Transmit a generic header negative acknowledge message. */
      (void)DoIPInt_TransmitOemSpecificPayloadType(ConnectionId, 0x0000u, &responseInfo);

      break;
    }
  }

  DOIPINT_DUMMY_STATEMENT(PayloadDataPtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /*lint !e438 */
} /* Appl_DoIPInt_ReceiveOemSpecificPayloadType() */
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

/**********************************************************************************************************************
 *  END OF FILE: Appl_DoIPInt.c
 *********************************************************************************************************************/
