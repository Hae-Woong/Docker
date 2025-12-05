/**********************************************************************************************************************
 *  FILE REQUIRES USER MODIFICATIONS
 *  Template Scope: whole file
 *  -------------------------------------------------------------------------------------------------------------------
 *  This file includes template code that must be completed and/or adapted during BSW integration. The template code is
 *  incomplete and only intended for providing a signature and an empty implementation. It is neither intended nor
 *  qualified for use in series production without applying suitable quality measures. The template code must be
 *  completed as described in the instructions given within this file and/or in the TechnicalReference. The completed
 *  implementation must be tested with diligent care and must comply with all quality requirements which are necessary
 *  according to the state of the art before its use.
 *********************************************************************************************************************/

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
/**        \file  Appl_DoIP.c
 *        \brief  Diagnostic over Internet Protocol
 *
 *      \details  Demo application for component
 *
 *********************************************************************************************************************/

#define APPLDOIP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "Appl_DoIP.h"

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

/* EXAMPLE
 *  uint8 Appl_DoIP_Vin[17u]           = { 0x57u, 0x30u, 0x4Cu, 0x30u, 0x30u, 0x30u, 0x30u, 0x34u, 0x33u, 0x4Du, 0x42u, 0x35u, 0x34u, 0x31u, 0x33u, 0x32u, 0x36u };
 *  uint8 Appl_DoIP_Gid[6u]            = { 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu };
 *  uint8 Appl_DoIP_RoutActAuthReq[]   = { 0x00u, 0x01u };
 *  uint8 Appl_DoIP_RoutActAuthReqLen  = 2u;
 *  uint8 Appl_DoIP_RoutActConfReq[]   = { 0x02u, 0x03u };
 *  uint8 Appl_DoIP_RoutActConfReqLen  = 2u;
 *  uint8 Appl_DoIP_RoutActAuthRes[]   = { 0x04u, 0x05u };
 *  uint8 Appl_DoIP_RoutActAuthResLen  = 2u;
 *  uint8 Appl_DoIP_RoutActConfRes[]   = { 0x06u, 0x07u };
 *  uint8 Appl_DoIP_RoutActConfResLen  = 2u;
 *  uint8 Appl_DoIP_OemPayloadTypeSend = 0x01u;
 *  uint8 Appl_DoIP_DiagPowerMode      = 0x01u;
 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ApplDoIP_DoesRemoteAddressMatch()
 *********************************************************************************************************************/
/* EXAMPLE
 *  Std_ReturnType ApplDoIP_DoesRemoteAddressMatch(
 *    const SoAd_SockAddrType* RemoteAddress)
 *  {
 *    Std_ReturnType retVal = E_NOT_OK;
 *  
 *  #if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
 *  # error "Appl_DoIP: ApplDoIP_DoesRemoteAddressMatch() needs to be implemented!"
 *  #endif
 *  
 *    return retVal;
 *  }
 */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  ApplDoIP_GetVin()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_GetVin(
  uint8* Vin)
{
  Std_ReturnType retVal = E_NOT_OK;

#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_GetVin() needs to be implemented! An example is given in template."
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */

  /* EXAMPLE
   *  uint8 idx;
   *
   *  for ( idx = 0u; idx < 17u; idx++ )
   *  {
   *    Vin[idx] = Appl_DoIP_Vin[idx];
   *  }
   *
   *  retVal = E_OK;
   */

   return retVal;
}

/**********************************************************************************************************************
 *  ApplDoIP_GetGid()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_GetGid(
  uint8* GroupId)
{
  Std_ReturnType retVal = E_NOT_OK;

#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_GetGid() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */

  /* EXAMPLE
   *  uint8 idx;
   *
   *  for ( idx = 0u; idx < 6u; idx++ )
   *  {
   *    GroupId[idx] = Appl_DoIP_Gid[idx];
   *  }
   *
   *  retVal = E_OK;
   */

   return retVal;
}

/**********************************************************************************************************************
 *  ApplDoIP_TriggerGidSync()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_TriggerGidSync(void)
{
  Std_ReturnType retVal = E_NOT_OK;

#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_TriggerGidSync() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */

  /* EXAMPLE
   *  retVal = E_OK;
   */

   return retVal;
}

/**********************************************************************************************************************
 *  ApplDoIP_GetDiagPowerMode()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_GetDiagPowerMode(
  DoIP_PowerModeType* PowerStateReady)
{
  Std_ReturnType retVal = E_NOT_OK;

#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_GetDiagPowerMode() needs to be implemented! An example is given in template."
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */

  /* EXAMPLE
   *  *PowerStateReady = Appl_DoIP_DiagPowerMode;
   *
   *  retVal = E_OK;
   */

   return retVal;
}

/**********************************************************************************************************************
 *  ApplDoIP_ShutdownFinished()
 *********************************************************************************************************************/
void ApplDoIP_ShutdownFinished(void)
{
#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_ShutdownFinished() is an optional callback. Implement or remove the function depending on your configuration!"
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */
}

/**********************************************************************************************************************
 *  ApplDoIP_RoutingActivationAuthentication()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_RoutingActivationAuthentication(
  boolean* Authentified,
  uint8* AuthenticationReqData,
  uint8* AuthenticationResData)
{
  Std_ReturnType retVal = E_NOT_OK;

#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_RoutingActivationAuthentication() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */

  /* EXAMPLE
   *  uint8 idx;
   *
   *  // check OEM specific data
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActAuthReqLen; idx++ )
   *  {
   *    if ( AuthenticationReqData[idx] != Appl_DoIP_RoutActAuthReq[idx] )
   *    {
   *      retVal = E_NOT_OK;
   *    }
   *  }
   *
   *  // fill OEM spcific data/
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActAuthResLen; idx++ )
   *  {
   *    AuthenticationResData[idx] = Appl_DoIP_RoutActAuthRes[idx];
   *  }
   *
   *  *Authentified = TRUE;
   *
   *  retVal = E_OK;
   */

   return retVal;
}

/**********************************************************************************************************************
 *  ApplDoIP_RoutingActivationAuthenticationWithRemoteAddress()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_RoutingActivationAuthenticationWithRemoteAddress(
  boolean* Authentified,
  uint8* AuthenticationReqData,
  uint8* AuthenticationResData,
  const SoAd_SockAddrType* RemoteAddress)
{
  Std_ReturnType retVal = E_NOT_OK;

#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_RoutingActivationAuthenticationWithRemoteAddress() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */

  /* EXAMPLE
   *  uint8 idx;
   *
   *  // check remote address
   *  if ( ApplDoIP_DoesRemoteAddressMatch(RemoteAddress) == E_NOT_OK )
   *  {
   *    retVal = E_NOT_OK;
   *  }
   *
   *  // check OEM specific data
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActAuthReqLen; idx++ )
   *  {
   *    if ( AuthenticationReqData[idx] != Appl_DoIP_RoutActAuthReq[idx] )
   *    {
   *      retVal = E_NOT_OK;
   *    }
   *  }
   *
   *  // fill OEM spcific data
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActAuthResLen; idx++ )
   *  {
   *    AuthenticationResData[idx] = Appl_DoIP_RoutActAuthRes[idx];
   *  }
   *
   *  *Authentified = TRUE;
   *
   *  retVal = E_OK;
   */

   return retVal;
}

/**********************************************************************************************************************
 *  ApplDoIP_RoutingActivationConfirmation()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_RoutingActivationConfirmation(
  boolean* Confirmed,
  uint8* ConfirmationReqData,
  uint8* ConfirmationResData)
{
  Std_ReturnType retVal = E_NOT_OK;

#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_RoutingActivationConfirmation() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */

  /* EXAMPLE
   *  uint8 idx;
   *
   *  // check OEM specific data
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActConfReqLen; idx++ )
   *  {
   *    if ( ConfirmationReqData[idx] != Appl_DoIP_RoutActConfReq[idx] )
   *    {
   *      retVal = E_NOT_OK;
   *    }
   *  }
   *
   *  // fill OEM spcific data
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActConfResLen; idx++ )
   *  {
   *    ConfirmationResData[idx] = Appl_DoIP_RoutActConfRes[idx];
   *  }
   *
   *  *Confirmed = TRUE;
   *
   *  retVal = E_OK;
   */

   return retVal;
}

/**********************************************************************************************************************
 *  ApplDoIP_RoutingActivationConfirmationWithRemoteAddress()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_RoutingActivationConfirmationWithRemoteAddress(
  boolean* Confirmed,
  uint8* ConfirmationReqData,
  uint8* ConfirmationResData,
  const SoAd_SockAddrType* RemoteAddress)
{
  Std_ReturnType retVal = E_NOT_OK;

#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_RoutingActivationConfirmationWithRemoteAddress() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */

  /* EXAMPLE
   *  uint8 idx;
   *
   *  // check remote address
   *  if ( ApplDoIP_DoesRemoteAddressMatch(RemoteAddress) == E_NOT_OK )
   *  {
   *    retVal = E_NOT_OK;
   *  }
   *
   *  // check OEM specific data
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActConfReqLen; idx++ )
   *  {
   *    if ( ConfirmationReqData[idx] != Appl_DoIP_RoutActConfReq[idx] )
   *    {
   *      retVal = E_NOT_OK;
   *    }
   *  }
   *
   *  // fill OEM spcific data
   *  for ( idx = 0u; idx < Appl_DoIP_RoutActConfResLen; idx++ )
   *  {
   *    ConfirmationResData[idx] = Appl_DoIP_RoutActConfRes[idx];
   *  }
   *
   *  *Confirmed = TRUE;
   *
   *  retVal = E_OK;
   */

   return retVal;
}

/**********************************************************************************************************************
 *  ApplDoIP_ReceiveOemSpecificPayloadType()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_ReceiveOemSpecificPayloadType(
  DoIP_ConnectionIdType ConnectionId,
  uint16 PayloadType,
  const PduInfoType* PayloadDataPtr,
  DoIP_OemPayloadTypeFlagType Flags)
{
  Std_ReturnType retVal = E_NOT_OK;

#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_ReceiveOemSpecificPayloadType() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */

  /* EXAMPLE
   *  uint8 idx;
   *  boolean ignoreMsg = FALSE;
   *  boolean closeRequired = FALSE;
   *
   *  // check payload type
   *  if ( PayloadType != 0xF000u )
   *  {
   *    retVal = E_NOT_OK;
   *  }
   *
   *  // check flags
   *  // received on UDP connection
   *  if ( (Flags & DOIP_OEM_PAYLOAD_TYPE_FLAG_PROT_TCP) == 0u )
   *  {
   *    // always close the connection in case of UDP after responding to the message
   *    closeRequired = TRUE;
   *  }
   *  // received on TCP connection
   *  else
   *  {
   *    // routing activation not active
   *    if ( (Flags & DOIP_OEM_PAYLOAD_TYPE_FLAG_ROUT_ACTIV) == 0u )
   *    {
   *      // ignore message on TCP when routing activation is not active and return E_OK so that no NACK is triggered
   *      ignoreMsg = TRUE;
   *      retVal = E_OK;
   *    }
   *  }
   *  
   *
   *  if ( ignoreMsg == FALSE )
   *  {
   *    // check user data
   *    for ( idx = 0u; idx < PayloadDataPtr->SduLength; idx++ )
   *    {
   *      if ( PayloadDataPtr->SduDataPtr[idx] != idx )
   *      {
   *        retVal = E_NOT_OK;
   *      }
   *    }
   *
   *    // prepare response if required
   *    if ( Appl_DoIP_OemPayloadTypeSend == 0x01u )
   *    {
   *      uint8 txResponseData[1500u] = { 0u };
   *      uint16 txPayloadType = 0xF001u;
   *      PduInfoType txResponse;
   *
   *      if ( PayloadDataPtr->SduLength == 0u )
   *      {
   *        // if request has no user data send response without user data, too
   *        txResponse.SduLength = 0u;
   *      }
   *      else
   *      {
   *        // copy half of request data
   *        txResponse.SduLength = PayloadDataPtr->SduLength / 2u;
   *        for ( idx = 0u; idx < (PayloadDataPtr->SduLength / 2u); idx++ )
   *        {
   *          txResponseData[idx] = idx + 1u;
   *        }
   *      }
   *      txResponse.SduDataPtr = txResponseData;
   *
   *      // transmit message
   *      retVal = DoIP_TransmitOemSpecificPayloadType(ConnectionId, txPayloadType, &txResponse);
   *
   *      // close connection if required
   *      if ( closeRequired == TRUE )
   *      {
   *        (void)DoIP_CloseConnection(ConnectionId, FALSE);
   *      }
   *    }
   *  }
   *
   *  retVal = E_OK;
   */

  return retVal;
}

/**********************************************************************************************************************
 *  ApplDoIP_VerifyTargetAddr()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_VerifyTargetAddr(
  uint16 TargetAddr)
{
  Std_ReturnType retVal = E_NOT_OK;

#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_VerifyTargetAddr() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */

  /* EXAMPLE
   *  // check target address
   *  if ( TargetAddr == 0x0200u )
   *  {
   *    retVal = E_OK;
   *  }
   */

   return retVal;
}

/**********************************************************************************************************************
 *  ApplDoIP_VerifyRxPdu()
 *********************************************************************************************************************/
Std_ReturnType ApplDoIP_VerifyRxPdu(
  const SoAd_SockAddrType* LocalAddrPtr,
  const SoAd_SockAddrType* RemoteAddrPtr,
  uint16 SourceAddr,
  uint16 TargetAddr,
  const PduInfoType* PduInfoPtr)
{
  Std_ReturnType retVal = E_NOT_OK;

#if !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE)
# error "Appl_DoIP: ApplDoIP_VerifyRxPdu() is an optional callback. Implement or remove the function depending on your configuration! An example is given in template."
#endif /* !defined(DOIP_DEACTIVATE_ERROR_DIRECTIVE) */

  /* EXAMPLE
   *  // check logical addresses
   *  if ( (SourceAddr == 0x0201u) &&
   *       (TargetAddr == 0x0E00u) )
   *  {
   *    if ( // check local IP addresses
   *         ((*((const SoAd_SockAddrInetType*)LocalAddrPtr)).domain     == TCPIP_AF_INET) &&
   *         ((*((const SoAd_SockAddrInetType*)LocalAddrPtr)).port       == 0x5834u) &&    // 0x5834u nbo --> 0x3458u / 13400u
   *         ((*((const SoAd_SockAddrInetType*)LocalAddrPtr)).addr[0u]   == 0x2001FEA9u) && // 0x2001FEA9u nbo --> 169.254.1.32
   *         // check remote IP addresses
   *         ((*((const SoAd_SockAddrInetType*)RemoteAddrPtr)).domain    == TCPIP_AF_INET) &&
   *         ((*((const SoAd_SockAddrInetType*)RemoteAddrPtr)).port      == 0x4858u) &&    // 0x4858u nbo --> 0x5848u / 22600u
   *         ((*((const SoAd_SockAddrInetType*)RemoteAddrPtr)).addr[0u]  == 0x0501FEA9u) )  // 0x0501FEA9u nbo --> 169.254.1.5
   *    { // IP addresss chack passes
   *
   *      // check user data
   *      if ( (PduInfoPtr != NULL_PTR) &&
   *           (PduInfoPtr->SduLength > 0u) &&
   *           (PduInfoPtr->SduDataPtr[0u] == 'A') )
   *      { // user data check (of first byte) passes
   *        retVal = E_OK;
   *      }
   *      else
   *      { // user data check (of first byte) fails
   *        retVal = E_NOT_OK;
   *      }
   *    }
   *    else
   *    { // IP address check fails
   *      retVal = E_NOT_OK;
   *    }
   *  }
   */

   return retVal;
}

/**********************************************************************************************************************
 *  END OF FILE: Appl_DoIP.c
 *********************************************************************************************************************/
