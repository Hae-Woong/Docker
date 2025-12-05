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
/*!        \file  Appl_DoIPInt.h
 *        \brief  DoIPInt application template header file
 *      \details  Template header implementation file for DoIPInt application to be completed by the user of MICROSAR4.
 *********************************************************************************************************************/

#if ( !defined (APPL_DOIPINT_H) )
# define APPL_DOIPINT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Platform_Types.h"
# include "Compiler_Cfg.h"
# include "DoIPInt_Types.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  Appl_DoIPInt_Event()
 *********************************************************************************************************************/
/*! \brief        Notifies about a DoIP protocol event or error.
 *  \details      This function is an implementation example to handle the events reported by DoIPInt. In case of ISO
 *                header, it is implemented in a way that tries to fulfill the requirements of ISO 13400-2 (including
 *                header structure). Therefore, this function uses the DoIPInt feature to transmit OEM specific payload
 *                types and the close service to close the connection if required. In case of LIGHT header, only the
 *                closing of connections according to ISO 13400-2 is implemented since no OEM specific payload types
 *                can be transmitted.
 *  \param[in]    ConnectionId            DoIPInt unique identifier of the connection.
 *  \param[in]    Event                   Event definition.
 *  \param[in]    MsgDataPtr              Pointer to entire DoIP message.
 *                                        [range: MsgDataPtr != NULL_PTR
 *                                                lengthOf(MsgDataPtr) >= DOIPINT_GEN_HDR_LEN]
 *  \return       E_OK                    Handle event optimistic.
 *  \return       E_NOT_OK                Handle event pessimistic.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionId, TRUE otherwise
 *  \synchronous  TRUE
 *  \spec
 *    requires MsgDataPtr != NULL_PTR;
 *    requires MsgDataPtr->SduDataPtr != NULL_PTR;
 *  \endspec
 *********************************************************************************************************************/
Std_ReturnType Appl_DoIPInt_Event(
  DoIPInt_ConnectionIdType ConnectionId,
  DoIPInt_EventType Event,
  const PduInfoType* MsgDataPtr);

#if ( DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO )
/**********************************************************************************************************************
 *  Appl_DoIPInt_ReceiveOemSpecificPayloadType()
 *********************************************************************************************************************/
/*! \brief        Notifies about reception of an unknown payload type.
 *  \details      This function is an implementation example to handle OEM specfic payload types and to transmit
 *                related responses. This example simply ignores the DoIP acknowledge messages and transmits a negative
 *                acknowledgement for other payload types.
 *  \param[in]    ConnectionId            DoIPInt unique identifier of the connection.
 *  \param[in]    PayloadType             The received payload type.
 *  \param[in]    PayloadDataPtr          PDU information structure which contains the length and the payload data of
 *                                        the message.
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE for same ConnectionId, TRUE otherwise
 *  \synchronous  TRUE
 *  \config       DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO
 *********************************************************************************************************************/
void Appl_DoIPInt_ReceiveOemSpecificPayloadType(
  DoIPInt_ConnectionIdType ConnectionId,
  uint16 PayloadType,
  const PduInfoType* PayloadDataPtr);
#endif /* DOIPINT_HDR_STRUCT == DOIPINT_HDR_STRUCT_ISO */

#endif /* !defined (APPL_DOIPINT_H) */

/**********************************************************************************************************************
 *  END OF FILE: Appl_DoIPInt.h
 *********************************************************************************************************************/
