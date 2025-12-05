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
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  TcpIp_IcmpV4.c
 *        \brief  Implementation of Internet Control Message Protocol for IPv4 (ICMPv4)
 *
 *      \details  This file is part of the TcpIp IPv4 submodule.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp IPv4 submodule. >> TcpIp_IpV4.h / TcpIp.h (Version 8.xx.xx and higher)
 *********************************************************************************************************************/


/* PRQA S 0828 EOF */ /* MD_MSR_1.1 */

#define TCPIP_ICMPV4_SOURCE

#include "TcpIp_Cfg.h"
#include "TcpIp_Lcfg.h"
#if (TCPIP_SUPPORT_ICMPV4  == STD_ON)                                                                                   /* COV_TCPIP_ICMPV4_ALWAYS_ON */
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "ComStack_Types.h"


# include "TcpIp_IcmpV4.h"
# include "TcpIp_IcmpV4_Cbk.h"
# include "TcpIp_IpV4_Priv.h"


# include "TcpIp_IpV4.h"
# include "TcpIp_Priv.h"


# include "IpBase_Copy.h"
# include "IpBase.h"


# if !defined (STATIC)                                                                                                  /* COV_TCPIP_COMPATIBILITY */
#  define STATIC static
# endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define IPV4_ICMP_TYPE_CODE_CHECKSUM_LEN       4u
# define IPV4_ICMP_IDENT_SEQNUM_LEN             4u
# define IPV4_ICMP_HEADER_LEN                   8u
# define IPV4_ICMP_MAX_ORIGINAL_PAYLOAD_LEN     8u
# define IPV4_ICMP_DEST_UNREACHABLE_BUF_MAXLEN  68u  /* This value is configured by the generator. 60 byte max header length + 8 byte max ICMPv4 IP payload length. */

# define ICMPV4_HDR_START_TYPE_OFS              0u
# define ICMPV4_HDR_START_CODE_OFS              1u
# define ICMPV4_HDR_START_CHECKSUM_OFS          2u
# define ICMPV4_HDR_START_IDENTIFIER_OFS        4u
# define ICMPV4_HDR_START_SEQNUM_OFS            6u
# define ICMPV4_HDR_START_DATA_OFS              8u
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef struct
{
  uint8    Type;
  uint8    Code;
  uint16   Checksum;
  uint16   Identifier;
  uint16   SeqNum;
} IpV4_Icmp_EchoMsgType; /* Echo or Echo Request */

/*lint -e451 */ /* Suppress ID451 because TcpIp_MemMap.h / MemMap.h cannot use a include guard */

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

# define TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

# define TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */


# define TCPIP_START_SEC_VAR_NO_INIT_16
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

STATIC VAR(uint16, TCPIP_VAR_NO_INIT) IpV4_Icmp_SeqNum;

# define TCPIP_STOP_SEC_VAR_NO_INIT_16
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */


/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define TCPIP_START_SEC_CODE
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/**********************************************************************************************************************
 *  IpV4_Icmp_VSendPendingEchoReply
 *********************************************************************************************************************/
/*! \brief        Tries to send a pending echo reply, if there is one.
 *  \details      -
 *  \param[in]    IpV4CtrlIdx Index of the IPv4 controller.
 *                            \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) IpV4_Icmp_VSendPendingEchoReply(TcpIp_IpV4CtrlIterType IpV4CtrlIdx);

# if (TCPIP_SUPPORT_ICMPV4_DST_UNREACHABLE_MSG == STD_ON)
/**********************************************************************************************************************
 *  IpV4_Icmp_VSendPendingDestinationUnreachableMsg
 *********************************************************************************************************************/
/*! \brief        Tries to send a pending Destination Unreachable Message, if there is one.
 *  \details      -
 *  \param[in]    IpV4CtrlIdx Index of the IPv4 controller.
 *                            \spec requires IpV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); \endspec
 *  \pre          -
 *  \context      TASK
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) IpV4_Icmp_VSendPendingDestinationUnreachableMsg(TcpIp_IpV4CtrlIterType IpV4CtrlIdx);
# endif

/**********************************************************************************************************************
 *  IpV4_Icmp_VRxEchoRequest
 *********************************************************************************************************************/
/*! \brief        Receives an ICMP Echo Request Message and sends an Echo Reply.
 *  \details      If the Echo reply cannot be sent directly because there is no ethernet buffer available or link-layer
 *                address resolution is still in progress, transmission of the
 *                Echo Reply will be scheduled for the MainFunction.
 *  \param[in]    LocalAddrV4Idx    Local IP address identifier.
 *  \param[in]    RemNetAddr        Remote IP address value.
 *  \param[in]    DataPtr           Payload of the ICMP message starting with the
                                    Echo Request Identifier and Sequence Number values.
 *  \param[in]    DataLen           Length of the Payload.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) IpV4_Icmp_VRxEchoRequest(
  TcpIp_SizeOfLocalAddrV4Type LocalAddrV4Idx,
  IpBase_AddrInType           RemNetAddr,
  IPV4_P2C(uint8)             DataPtr,
  uint16                      DataLen);

/**********************************************************************************************************************
 *  IpV4_Icmp_VSendRawMessage
 *********************************************************************************************************************/
/*! \brief        Sends an ICMP message with the specified Type, Code and payload.
 *  \details      This function will automatically send a fragmented IP packet if the payload exceeds the MTU and
 *                fragmentation is enabled.
 *  \param[in]    LocalAddrV4Idx    Local IP address identifier(source).
 *  \param[in]    DestIpAddr        Destination IP address value.
 *  \param[in]    Type              Type of the ICMP message.
 *  \param[in]    Code              Code of the ICMP message.
 *  \param[in]    VarHeaderBytesPtr Pointer to the 4 message dependent bytes that follow the Type, Code and Checksum
 *                                  fields in the ICMP header.
 *                                  Note: Exactly four bytes will be read from the pointer destination.
 *  \param[in]    PayloadPtr        Pointer to the payload of the ICMPv4 message.
 *  \param[in]    PayloadLen        Length of the payload in bytes.
 *  \return       BUFREQ_OK         Transmit triggered.
 *  \return       BUFREQ_E_BUSY     Transmit currently not possible. Retry later.
 *  \return       BUFREQ_E_NOT_OK   Transmit not possible due to unknown error.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(BufReq_ReturnType, TCPIP_CODE) IpV4_Icmp_VSendRawMessage(
  TcpIp_LocalAddrV4IterType LocalAddrV4Idx,
  IpBase_AddrInType         DestIpAddr,
  uint8                     Type,
  uint8                     Code,
  IPV4_P2C(uint8)           VarHeaderBytesPtr,
  IPV4_P2C(uint8)           PayloadPtr,
  uint16                    PayloadLen);

/**********************************************************************************************************************
 *  IpV4_Icmp_SendEchoMessage
 *********************************************************************************************************************/
/*! \brief        Sends an ICMP Echo (Request/Reply) message.
 *  \details      -
 *  \param[in]    LocalAddrV4Idx  Local IP address identifier(Source).
 *  \param[in]    DestIpAddr      Destination IP address.
 *  \param[in]    IcmpType        Type of the ICMP message (ECHO_REQUEST, ECHO_REPLY).
 *  \param[in]    IcmpCode        Code of the ICMP message (usually 0).
 *  \param[in]    Identifier      Identifier of the Echo message
 *  \param[in]    SequenceNumber  Sequence Number of the ICMP message.
 *  \param[in]    DataPtr         Pointer to the payload of the Echo message.
 *  \param[in]    DataLenByte     Length of the payload in bytes.
 *  \return       BUFREQ_OK       Transmit triggered.
 *  \return       BUFREQ_E_BUSY   Transmit currently not possible. Retry later.
 *  \return       BUFREQ_E_NOT_OK Transmit not possible.
 *  \pre          -
 *  \context      TASK|ISR
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL_INLINE FUNC(BufReq_ReturnType, TCPIP_CODE) IpV4_Icmp_SendEchoMessage(
  TcpIp_SizeOfLocalAddrV4Type LocalAddrV4Idx,
  IpBase_AddrInType           DestIpAddr,
  uint8                       IcmpType,
  uint8                       IcmpCode,
  uint16                      Identifier,
  uint16                      SequenceNumber,
  IPV4_P2C(uint8)             DataPtr,
  uint16                      DataLenByte);

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  IpV4_Icmp_Init
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) IpV4_Icmp_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_IpV4SocketDynIterType ipV4SocketIdx = 0;
  TcpIp_IpV4CtrlIterType      ipV4CtrlIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate all IP controller instances and reset pending msg info */
  for (ipV4CtrlIdx = 0; ipV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); ipV4CtrlIdx++)
  {
    TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(TcpIp_GetIcmpEchoReplyInfoIdxOfIpV4Ctrl(ipV4CtrlIdx))->IpV4_Icmp_EchoReplyIsPending = FALSE;

# if (TCPIP_SUPPORT_ICMPV4_DST_UNREACHABLE_MSG == STD_ON)
    TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(TcpIp_GetIcmpDestUnreachableInfoIdxOfIpV4Ctrl(ipV4CtrlIdx))->IpV4_Icmp_DestinationUnreachablePending = FALSE;
# endif
  }

  IpV4_Icmp_SeqNum = 0;

  ipV4SocketIdx = TcpIp_GetIpV4SocketDynIcmpIdxOfIpV4General(TCPIP_IPV4GENERAL_IDX);

  /* #20 Set the time to live for the given socket index. */
  IpV4_Ip_SetTimeToLive(ipV4SocketIdx, TcpIp_GetTimeToLiveOfIcmpConfig(TCPIP_ICMPV4CONFIG_IDX));
} /* IpV4_Icmp_Init() */

/**********************************************************************************************************************
 *  IpV4_Icmp_MainFunction
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) IpV4_Icmp_MainFunction(TcpIp_OsApplicationType ApplId)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_IpV4CtrlIterType ipV4CtrlIdx;

  /* ----- Implementation ----------------------------------------------- */
# if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_OFF)                                                                       /* COV_TCPIP_MULTICORE_SUPPORT_ALWAYS_OFF */
  TCPIP_DUMMY_STATEMENT_CONST(ApplId);                                                                                  /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif

  /* #10 Iterate all IP controller instances and send pending destination unreachable messages */
  for (ipV4CtrlIdx = 0; ipV4CtrlIdx < TcpIp_GetSizeOfIpV4Ctrl(); ipV4CtrlIdx++)
  {
# if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_ON)                                                                        /* COV_TCPIP_MULTICORE_SUPPORT_NEVER_ON */
    /* Only process the controller assigned to corresponding application */
    if (ApplId == TcpIp_GetApplIdOfIpV4Ctrl(ipV4CtrlIdx))
# endif
    {
      /* #20 Send pending ICMPv4 Echo Replies. */
      IpV4_Icmp_VSendPendingEchoReply(ipV4CtrlIdx);

      /* #30 Send pending ICMPv4 Destination Unreachable Messages. */
# if (TCPIP_SUPPORT_ICMPV4_DST_UNREACHABLE_MSG == STD_ON)
      IpV4_Icmp_VSendPendingDestinationUnreachableMsg(ipV4CtrlIdx);
# endif
    }
  }

} /* IpV4_Icmp_MainFunction() */

/**********************************************************************************************************************
 *  IpV4_Icmp_RxIndication
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
FUNC(void, TCPIP_CODE) IpV4_Icmp_RxIndication(
  TcpIp_SizeOfLocalAddrV4Type LocalAddrV4Idx,
  IpBase_AddrInType           RemNetAddr,
  uint8                       Ttl,
  IPV4_P2V(uint8)             DataPtr,
  uint16                      DataLen,
  boolean                   ChecksumCalculated)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean chksumCal = ChecksumCalculated;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(LocalAddrV4Idx < TcpIp_GetSizeOfLocalAddrV4());
  TCPIP_ASSERT(DataPtr    != NULL_PTR);

  if (DataLen >= IPV4_ICMP_HEADER_LEN)
  {
    /* #10 Check if the checksum is calculated. */
    if (chksumCal == FALSE)
    {
      chksumCal = (boolean)(IpBase_TcpIpChecksumAdd(DataPtr, DataLen, 0, TRUE) == 0u);                                  /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    }

    if (chksumCal == TRUE)
    {
      uint8 type = TCPIP_GET_UINT8(DataPtr, ICMPV4_HDR_START_TYPE_OFS);
      uint8 code = TCPIP_GET_UINT8(DataPtr, ICMPV4_HDR_START_CODE_OFS);

      boolean messageHandled  = FALSE;

      if (type == IPV4_ICMP_TYPE_ECHO)
      {
        if (code == IPV4_ICMP_CODE_NO_CODE)
        {
          /* #20 Send the Echo reply, if Echo Request is received.  */
          /* Pass datapointer at position identifier and and pass the updated datalength(subtracted  first 4 bytes of header) .*/
          IpV4_Icmp_VRxEchoRequest(LocalAddrV4Idx, RemNetAddr, &DataPtr[IPV4_ICMP_TYPE_CODE_CHECKSUM_LEN], DataLen - IPV4_ICMP_TYPE_CODE_CHECKSUM_LEN);
          messageHandled = TRUE;
        }
      }

      if ((messageHandled == FALSE) && (TcpIp_GetIcmpMsgHandlerCbkFctPtr() != NULL_PTR))                                /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
      {
        TcpIp_LocalAddrIdType  localAddrId = TCPIP_LOCAL_ADDR_IDX_TO_ID(TCPIP_LOCAL_ADDR_IDX_TO_IPVX(LocalAddrV4Idx));
        TcpIp_SockAddrBaseType remoteSockAddr;                                                                          /* PRQA S 0759 */ /* MD_MSR_Union */
        remoteSockAddr.SockAddrIn.sin_family = TCPIP_AF_INET;
        remoteSockAddr.SockAddrIn.sin_port   = TCPIP_PORT_NOT_SET;
        remoteSockAddr.SockAddrIn.sin_addr   = RemNetAddr;

        /* #30 Discard the message, if IcmpMsgHandler is configured and if the received message is not Echo request. */
        /* ICMP type not supported -> silently discard message */
        TcpIp_GetIcmpMsgHandlerCbkFctPtr()(localAddrId, (TCPIP_P2C(TcpIp_SockAddrType)) &remoteSockAddr.TcpIpSockAddr, Ttl,
          type, code, DataLen - ICMPV4_HDR_START_IDENTIFIER_OFS, &DataPtr[ICMPV4_HDR_START_IDENTIFIER_OFS]);
        TCPIP_DUMMY_STATEMENT(remoteSockAddr.SockAddrIn);                                                               /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
      }
    }
  }

}  /* IpV4_Icmp_RxIndication() */


/**********************************************************************************************************************
 *  IpV4_Icmp_SendEcho
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) IpV4_Icmp_SendEcho(
  TcpIp_SizeOfLocalAddrV4Type LocalAddrV4Idx,
  IpBase_AddrInType           Dest,
  IPV4_P2C(uint8)             DataPtr,
  uint16                      LenByte )
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

 /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(LocalAddrV4Idx < TcpIp_GetSizeOfLocalAddrV4() || LocalAddrV4Idx == TCPIP_LOCAL_ADDR_V4_IDX_ANY);
  TCPIP_ASSERT(DataPtr    != NULL_PTR);

  if (TcpIp_LocalAddrIdxIsOnHold(LocalAddrV4Idx) == FALSE)
  {
    /* #10 Increment the sequence number. */
    IpV4_Icmp_SeqNum++;
    /* #20 Check if echo message has been send successfully.*/
    if (IpV4_Icmp_SendEchoMessage(LocalAddrV4Idx, Dest, IPV4_ICMP_TYPE_ECHO, 0, 0, IpV4_Icmp_SeqNum, DataPtr, LenByte) == BUFREQ_OK)
    {
      retVal = E_OK;
    }
    else
    {
      retVal = E_NOT_OK;
    }
  }
  return retVal;
} /* IpV4_Icmp_SendEcho() */

# if (TCPIP_SUPPORT_ICMPV4_DST_UNREACHABLE_MSG == STD_ON)
/**********************************************************************************************************************
 *  IpV4_Icmp_SendDestUnreachable()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) IpV4_Icmp_SendDestUnreachable(
  TcpIp_LocalAddrV4IterType LocalAddrV4Idx,
  TCPIP_P2C(uint8)          DataPtr,
  uint8                     IcmpCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint8 unusedHeaderBytes[4] = { 0, 0, 0, 0 };
  uint8  ipHdrLen = TCPIP_IPV4_HDR_FIELD_GET_IHL_BYTE(DataPtr);
  uint16 ipPayloadLen = TCPIP_IPV4_HDR_FIELD_GET_TOTAL_LEN(DataPtr) - ipHdrLen;
  uint16 ipTotalPktLen = 0;
  boolean MessageSent = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_IpV4CtrlDynIterType ipV4CtrlIdx = TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(LocalAddrV4Idx);

  /* #10 Limit original IP packet payload to 64 bits according to IETF RFC 792. */
  ipPayloadLen = (ipPayloadLen > IPV4_ICMP_MAX_ORIGINAL_PAYLOAD_LEN) ? IPV4_ICMP_MAX_ORIGINAL_PAYLOAD_LEN : ipPayloadLen;

  /* Calculated the final packet length (IpV4 Header length (upto 60 bytes) + 8 bytes of IPv4 payload) */
  ipTotalPktLen = ipHdrLen + ipPayloadLen;

  /* Total length MUST be less than 32 bytes */
  TCPIP_ASSERT(ipTotalPktLen <= IPV4_ICMP_DEST_UNREACHABLE_BUF_MAXLEN);

  /* #20 Send ICMP Destination Unreachable message when controller is not in state OnHold. */
  if (TcpIp_GetStateOfIpV4CtrlDyn(ipV4CtrlIdx) == IPV4_IP_ONHOLD)
  {
    /* MessageSent is already FALSE */
  }
  else if (IpV4_Icmp_VSendRawMessage(LocalAddrV4Idx, TCPIP_GET_UINT32_RAW(DataPtr, TCPIP_IPV4_HDR_OFS_SRC_ADDR),
    IPV4_ICMP_TYPE_DEST_UNRECHABLE, IcmpCode, &unusedHeaderBytes[0],
    DataPtr, ipTotalPktLen) != BUFREQ_OK)
  {
    /* MessageSent is already FALSE */
  }
  else
  {
    MessageSent = TRUE;
  }

  /* #30 Store ICMP msg when sending was not successful. */
  if(MessageSent == FALSE)
  {
    TcpIp_IcmpDestUnreachableInfoIdxOfIpV4CtrlType icmpDestUnreachableInfoIdx = TcpIp_GetIcmpDestUnreachableInfoIdxOfIpV4Ctrl(ipV4CtrlIdx);
    /*@ assert icmpDestUnreachableInfoIdx < TcpIp_GetSizeOfIcmpDestUnreachableInfo(); */                                /* VCA_TCPIP_ICMP_UPDATE_DESTUNREACHABLEINFO */

    TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(icmpDestUnreachableInfoIdx)->Dest = TCPIP_GET_UINT32_RAW(DataPtr, TCPIP_IPV4_HDR_OFS_SRC_ADDR);
    TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(icmpDestUnreachableInfoIdx)->Code = IcmpCode;

    /* copy data that was received with incoming message */
    /* PRQA S 0315 1 */ /* MD_MSR_VStdLibCopy */
    IpBase_Copy(TcpIp_GetAddrIcmpDestUnreachableData(TcpIp_GetIcmpDestUnreachableDataStartIdxOfIpV4Ctrl(ipV4CtrlIdx)), &DataPtr[0], ipTotalPktLen); /* VCA_TCPIP_STORE_ICMP_DEST_UNREACHABLE */

    TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(icmpDestUnreachableInfoIdx)->IpV4_Icmp_DestUnreachMsgLen = ipTotalPktLen;
    TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(icmpDestUnreachableInfoIdx)->IpV4_Icmp_DestinationUnreachablePending = TRUE;
  }
} /* IpV4_Icmp_SendDestUnreachable() */
# endif

/**********************************************************************************************************************
 *  IpV4_Icmp_SendEchoMessage
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
TCPIP_LOCAL_INLINE FUNC(BufReq_ReturnType, TCPIP_CODE) IpV4_Icmp_SendEchoMessage(
  TcpIp_SizeOfLocalAddrV4Type LocalAddrV4Idx,
  IpBase_AddrInType           DestIpAddr,
  uint8                       IcmpType,
  uint8                       IcmpCode,
  uint16                      Identifier,
  uint16                      SequenceNumber,
  IPV4_P2C(uint8)             DataPtr,
  uint16                      DataLenByte)
{
   /* ----- Local Variables ---------------------------------------------- */
    uint8 icmpHeaderBytes[IPV4_ICMP_HEADER_LEN - IPV4_ICMP_TYPE_CODE_CHECKSUM_LEN] = { 0 };
  BufReq_ReturnType retVal = BUFREQ_E_BUSY;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify that Controller is not in state OnHold. */
  if (TcpIp_LocalAddrIdxIsOnHold(LocalAddrV4Idx) == FALSE)
  {
    /* #20 Copy Identifier and Seq Number into the Icmp header buffer. */
    TCPIP_PUT_UINT16(&icmpHeaderBytes[0], ICMPV4_HDR_START_IDENTIFIER_OFS - IPV4_ICMP_TYPE_CODE_CHECKSUM_LEN, Identifier);
    TCPIP_PUT_UINT16(&icmpHeaderBytes[0], ICMPV4_HDR_START_SEQNUM_OFS - IPV4_ICMP_TYPE_CODE_CHECKSUM_LEN, SequenceNumber);
    retVal = IpV4_Icmp_VSendRawMessage(LocalAddrV4Idx, DestIpAddr, IcmpType, IcmpCode, &icmpHeaderBytes[0], DataPtr, DataLenByte);
  }
  return retVal;
} /* IpV4_Icmp_SendEchoMessage() */

/**********************************************************************************************************************
 *  IpV4_Icmp_VRxEchoRequest
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL_INLINE FUNC(void, TCPIP_CODE) IpV4_Icmp_VRxEchoRequest(
  TcpIp_SizeOfLocalAddrV4Type LocalAddrV4Idx,
  IpBase_AddrInType           RemNetAddr,
  IPV4_P2C(uint8)             DataPtr,
  uint16                      DataLen)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_IpV4CtrlDynIterType ipV4CtrlIdx = TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(LocalAddrV4Idx);

  uint16 echoIdentifier = TCPIP_GET_UINT16(DataPtr, ICMPV4_HDR_START_IDENTIFIER_OFS - IPV4_ICMP_TYPE_CODE_CHECKSUM_LEN);
  uint16 echoSeqNum =     TCPIP_GET_UINT16(DataPtr, ICMPV4_HDR_START_SEQNUM_OFS - IPV4_ICMP_TYPE_CODE_CHECKSUM_LEN);
  /* Substract the rest 4 bytes of header from the data length. */
  uint16 echoDataLen = DataLen - (IPV4_ICMP_HEADER_LEN - IPV4_ICMP_TYPE_CODE_CHECKSUM_LEN);

  /* Check if the echo reply data is within limits */
  if (echoDataLen > TcpIp_GetEchoReplyMaxLenOfIcmpConfig(TCPIP_ICMPV4CONFIG_IDX))
  {
    echoDataLen = TcpIp_GetEchoReplyMaxLenOfIcmpConfig(TCPIP_ICMPV4CONFIG_IDX);
  }

  /* #10 Try to send Echo reply directly. */
  if (IpV4_Icmp_SendEchoMessage(LocalAddrV4Idx, RemNetAddr, IPV4_ICMP_TYPE_ECHO_REPLY, 0, echoIdentifier, echoSeqNum, &DataPtr[IPV4_ICMP_IDENT_SEQNUM_LEN], echoDataLen) == BUFREQ_E_BUSY)
  {
    TcpIp_IcmpEchoReplyInfoIdxOfIpV4CtrlType icmpEchoReplyInfoIdx = TcpIp_GetIcmpEchoReplyInfoIdxOfIpV4Ctrl(ipV4CtrlIdx);
    /*@ assert icmpEchoReplyInfoIdx < TcpIp_GetSizeOfIcmpEchoReplyInfo(); */                                            /* VCA_TCPIP_ICMP_UPDATE_PENDINGINFO */

    /* #20 Schedule pending reply for next MainFunction, if message cannot be sent directly due to pending link-layer address resolution or because no ethernet buffer is available. */
    TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->Dest       = RemNetAddr;
    TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->Identifier = echoIdentifier;
    TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->SeqNum     = echoSeqNum;

    /* copy data that was received with incoming message */
    /* PRQA S 0315 1 */ /* MD_MSR_VStdLibCopy */
    IpBase_Copy(TcpIp_GetAddrIcmpEchoReplyData(TcpIp_GetIcmpEchoReplyDataStartIdxOfIpV4Ctrl(ipV4CtrlIdx)), &DataPtr[IPV4_ICMP_IDENT_SEQNUM_LEN], echoDataLen); /* VCA_TCPIP_RUNTIME_CHECK_PRESENT_IN_SAME_FUNCTION */
    TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->IpV4_Icmp_EchoReplyDataLen   = echoDataLen;
    TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->IpV4_Icmp_EchoReplyIsPending = TRUE;
  }
} /* IpV4_Icmp_VRxEchoRequest() */

/**********************************************************************************************************************
 *  IpV4_Icmp_VSendRawMessage
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
/* PRQA S 6060, 6050 1 */ /* MD_MSR_STPAR, MD_TcpIp_STCAL_CslAccess */
TCPIP_LOCAL FUNC(BufReq_ReturnType, TCPIP_CODE) IpV4_Icmp_VSendRawMessage(
  TcpIp_LocalAddrV4IterType LocalAddrV4Idx,
  IpBase_AddrInType         DestIpAddr,
  uint8                     Type,
  uint8                     Code,
  IPV4_P2C(uint8)           VarHeaderBytesPtr,
  IPV4_P2C(uint8)           PayloadPtr,
  uint16                    PayloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType               retVal;
  TcpIp_IpTxRequestDescriptorType ipTxReqestDescr;
  uint16                          ipPayloadLen = IPV4_ICMP_HEADER_LEN + PayloadLen;
  IPV4_P2V(uint8)                 ipBufPtr;
  uint16                          ipBufLen = ipPayloadLen;
  IpBase_SockAddrInType           destSockAddr;
  TcpIp_LocalAddrV4IterType       localAddrIdx = LocalAddrV4Idx;

  /* ----- Implementation ----------------------------------------------- */
  destSockAddr.sin_family = IPBASE_AF_INET;
  destSockAddr.sin_addr   = DestIpAddr;
  destSockAddr.sin_port   = 0;
  /* Store the transport protocol information in the tx descriptor */
  ipTxReqestDescr.XptProtocol = IPV4_IP_PROTOCOL_ICMP;
# if (TCPIP_SUPPORT_IPSEC == STD_ON)
  ipTxReqestDescr.MessageType = Type;
# endif
  ipTxReqestDescr.TmpChecksum = 0;

  /* #10 Obtain TX buffer from IP.*/
  retVal = IpV4_Ip_ProvideTxBuffer(&localAddrIdx, TcpIp_GetIpV4SocketDynIcmpIdxOfIpV4General(TCPIP_IPV4GENERAL_IDX),
                                   &destSockAddr, &ipBufPtr, &ipBufLen, &ipTxReqestDescr, FALSE);

  if (retVal == BUFREQ_OK)
  {
    /*@ assert ($external(ipBufPtr) && ($lengthOf(ipBufPtr) >= (ipBufLen))); */                                         /* VCA_TCPIP_UPDATE_PTR */
    boolean copyTxData = TRUE;
    uint16  dataRemain = PayloadLen;
    uint16  dataPos    = 0;
    /* #20 Write ICMP message header. */
    /* IPv4 may provide multiple buffers if message exceeds link MTU,
     * but the first buffer will always be large enough for ICMP header.
     */

    /* Write ICMP type and code into message header. */
    ipBufPtr[0] = Type;
    ipBufPtr[1] = Code;

    /* Initialize checksum field. Value will be updated with calculated checksum later. */
    ipBufPtr[2] = 0;
    ipBufPtr[3] = 0;

    /* Write the four message dependent bytes in the ICMP header. */
    ipBufPtr[4] = VarHeaderBytesPtr[0];
    ipBufPtr[5] = VarHeaderBytesPtr[1];
    ipBufPtr[6] = VarHeaderBytesPtr[2];
    ipBufPtr[7] = VarHeaderBytesPtr[3];

    if (ipTxReqestDescr.ChecksumHwCalced == TCPIP_IP_PROTOCOL_CHECKSUM_CHECK_COPY)
    {
      ipTxReqestDescr.TmpChecksum = IpBase_TcpIpChecksumAdd(&ipBufPtr[0], sizeof(IpV4_Icmp_EchoMsgType),
                                                            ipTxReqestDescr.TmpChecksum, FALSE);
    }

    /* #30 Copy message payload into IP buffer.*/
    /* Copy message payload into IP buffer.
     * - Iterate over multiple IP buffer fragments if a fragmented IP packet needs to be sent.
     */
    ipBufPtr  = &ipBufPtr[sizeof(IpV4_Icmp_EchoMsgType)];
    ipBufLen -= (uint16) sizeof(IpV4_Icmp_EchoMsgType);
    /* #40 Iterate over multiple IP buffer fragments if a fragmented IP packet needs to be sent.*/
    while ((copyTxData == TRUE) && (dataRemain > 0u))
    {
      /*@ assert ($external(ipBufPtr) && ($lengthOf(ipBufPtr) >= (ipBufLen))); */                                       /* VCA_TCPIP_UPDATE_PTR */

      if (dataRemain < ipBufLen)
      {
        ipBufLen = dataRemain;
      }

      /* Copy payload. */
      /* PRQA S 0315 1 */ /* MD_MSR_VStdLibCopy */
      if (ipTxReqestDescr.ChecksumHwCalced == TCPIP_IP_PROTOCOL_CHECKSUM_CHECK_COPY)
      {
        ipTxReqestDescr.TmpChecksum = IpBase_TcpIpChecksumCopyAdd(ipBufPtr, &PayloadPtr[dataPos], ipBufLen,             /* VCA_TCPIP_UPDATE_PTR */
                                                                     ipTxReqestDescr.TmpChecksum, FALSE);
      }
      else
      {
        IpBase_Copy(ipBufPtr, &PayloadPtr[dataPos], ipBufLen);                                                          /* VCA_TCPIP_UPDATE_PTR */
      }
      dataRemain -= ipBufLen;
      dataPos    += ipBufLen;

      if (dataRemain > 0u)
      {
        /* #50 Request next buffer fragment. */
        if (IpV4_Ip_FragmentProvideTxBuffer(&ipTxReqestDescr, &ipBufPtr, &ipBufLen) != E_OK)
        {
          /* ERROR: This should not happen. If IpV4_Ip_ProvideTxBuffer() returns BUFREQ_OK there should be sufficient buffer space. */
          IpV4_ReportDetError(IPV4_IP_API_ID_V_INTERNAL_FUNCTION, IPV4_E_INV_PARAM);
          copyTxData = FALSE;
        }
      }
    }

    /* send message by IP: */
    /* #60 Trigger transmit of IP packet.*/
    if(IpV4_Ip_Transmit(&ipTxReqestDescr, ipPayloadLen, IPV4_IP_PROTOCOL_ICMP, FALSE, 0, FALSE) != E_OK)
    {
      /* transmit failed */
      retVal = BUFREQ_E_NOT_OK;
    }
  }

  return retVal;
} /* IpV4_Icmp_VSendRawMessage() */

/**********************************************************************************************************************
 *  IpV4_Icmp_VSendPendingEchoReply
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) IpV4_Icmp_VSendPendingEchoReply(TcpIp_IpV4CtrlIterType IpV4CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_IcmpEchoReplyInfoIdxOfIpV4CtrlType icmpEchoReplyInfoIdx = TcpIp_GetIcmpEchoReplyInfoIdxOfIpV4Ctrl(IpV4CtrlIdx);

  /* Check if any echo reply is pending */
  if (   (TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->IpV4_Icmp_EchoReplyIsPending == TRUE)
      && (TcpIp_GetStateOfIpV4CtrlDyn(IpV4CtrlIdx) != IPV4_IP_ONHOLD))
  {
    /* #10 Check if transmit is possible. If transmit is temporarily not possible, do not clear pending-flag and retry transmit later. */
    if (BUFREQ_E_BUSY != IpV4_Icmp_SendEchoMessage(TcpIp_GetLocalAddrV4UnicastIdxOfIpV4Ctrl(IpV4CtrlIdx),
      TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->Dest,
      IPV4_ICMP_TYPE_ECHO_REPLY,
      IPV4_ICMP_CODE_NO_CODE,
      TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->Identifier,
      TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->SeqNum,
      (const uint8*)TcpIp_GetAddrIcmpEchoReplyData(TcpIp_GetIcmpEchoReplyDataStartIdxOfIpV4Ctrl(IpV4CtrlIdx)),
      TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->IpV4_Icmp_EchoReplyDataLen))
    {
      /* Echo Reply sent or error occurred. No retry required. */
      /* #20 Otherwise: Clear pending-flag. (Pending packet will be dropped if transmit failed due to permanent error. */
      TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->IpV4_Icmp_EchoReplyIsPending = FALSE;
    }
    else
    {
      /* Retry later */
    }
  }
} /* IpV4_Icmp_VSendPendingEchoReply() */

# if (TCPIP_SUPPORT_ICMPV4_DST_UNREACHABLE_MSG == STD_ON)
/**********************************************************************************************************************
 *  IpV4_Icmp_VSendPendingDestinationUnreachableMsg
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) IpV4_Icmp_VSendPendingDestinationUnreachableMsg(TcpIp_IpV4CtrlIterType IpV4CtrlIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  const uint8 unusedHeaderBytes[4] = { 0, 0, 0, 0 };

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_IcmpDestUnreachableInfoIdxOfIpV4CtrlType icmpDestUnreachableInfoIdx = TcpIp_GetIcmpDestUnreachableInfoIdxOfIpV4Ctrl(IpV4CtrlIdx);

  /* Check if destination unreachable message is pending for the controller */
  if (   (TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(icmpDestUnreachableInfoIdx)->IpV4_Icmp_DestinationUnreachablePending == TRUE)
      && (TcpIp_GetStateOfIpV4CtrlDyn(IpV4CtrlIdx) != IPV4_IP_ONHOLD))
  {
    /* #10 Check if transmit is possible. If transmit is temporarily not possible, do not clear pending-flag and retry transmit later. */
    if (IpV4_Icmp_VSendRawMessage(TcpIp_GetLocalAddrV4UnicastIdxOfIpV4Ctrl(IpV4CtrlIdx),
          TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(icmpDestUnreachableInfoIdx)->Dest, IPV4_ICMP_TYPE_DEST_UNRECHABLE,
          TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(icmpDestUnreachableInfoIdx)->Code, (const uint8*) &unusedHeaderBytes[0],
          (const uint8*) TcpIp_GetAddrIcmpDestUnreachableData(TcpIp_GetIcmpDestUnreachableDataStartIdxOfIpV4Ctrl(IpV4CtrlIdx)),
          TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(icmpDestUnreachableInfoIdx)->IpV4_Icmp_DestUnreachMsgLen)
       == BUFREQ_OK)
    {
      /* Destination Unreachable Message sent or error occurred. No retry required. */
      /* #20 Otherwise: Clear pending-flag. (Pending packet will be dropped if transmit failed due to permanent error. */
      TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(icmpDestUnreachableInfoIdx)->IpV4_Icmp_DestinationUnreachablePending = FALSE;
    }
    else
    {
      /* Retry later */
    }
  }

} /* IpV4_Icmp_VSendPendingDestinationUnreachableMsg() */
# endif

/**********************************************************************************************************************
 *  IpV4_Icmp_Cbk_VAddrResTimeout
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) IpV4_Icmp_Cbk_VAddrResTimeout(
  TcpIp_IpV4CtrlIterType IpV4CtrlIdx,
  IpBase_AddrInType      DestAddr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify the Destination address of the message. */
  if (TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(TcpIp_GetIcmpEchoReplyInfoIdxOfIpV4Ctrl(IpV4CtrlIdx))->Dest == DestAddr)
  {
    /* #20 Cancel ICMP echo reply by reseting the Echo reply pending flag. */
    TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(TcpIp_GetIcmpEchoReplyInfoIdxOfIpV4Ctrl(IpV4CtrlIdx))->IpV4_Icmp_EchoReplyIsPending = FALSE;
  }
# if (TCPIP_SUPPORT_ICMPV4_DST_UNREACHABLE_MSG == STD_ON)
  if (TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(TcpIp_GetIcmpDestUnreachableInfoIdxOfIpV4Ctrl(IpV4CtrlIdx))->Dest == DestAddr)
  {
    /* #30 Cancel ICMP destination unreachable msg by reseting the destination unreachable msg pending flag. */
    TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(TcpIp_GetIcmpDestUnreachableInfoIdxOfIpV4Ctrl(IpV4CtrlIdx))->IpV4_Icmp_DestinationUnreachablePending = FALSE;
  }
# endif
} /* IpV4_Icmp_Cbk_VAddrResTimeout() */


/**********************************************************************************************************************
 *  IpV4_Icmp_StoreChecksumInHeader
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) IpV4_Icmp_StoreChecksumInHeader(
  uint16          ChecksumNbo,
  IPV4_P2V(uint8) IcmpHdrPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Store checksum into IcmpHdr buffer. */
  TCPIP_PUT_UINT16_RAW(IcmpHdrPtr, ICMPV4_HDR_START_CHECKSUM_OFS, ChecksumNbo);
} /* IpV4_Icmp_StoreChecksumInHeader() */

/**********************************************************************************************************************
 *  IpV4_Icmp_VLocalIpAddrAssignmentChange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) IpV4_Icmp_VLocalIpAddrAssignmentChange(
  TcpIp_SizeOfLocalAddrV4Type LocalAddrV4Idx,
  TcpIp_IpAddrStateType       State)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_IpV4CtrlDynIterType ipV4CtrlIdx = TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(LocalAddrV4Idx);

  /* #10 If Ip address is not assigned, cancel pending echo reply and destination unreachable messages for this controller */
  if (TCPIP_IPADDR_STATE_ASSIGNED != State)
  {
    TcpIp_IcmpEchoReplyInfoIdxOfIpV4CtrlType icmpEchoReplyInfoIdx = TcpIp_GetIcmpEchoReplyInfoIdxOfIpV4Ctrl(ipV4CtrlIdx);
    /*@ assert icmpEchoReplyInfoIdx < TcpIp_GetSizeOfIcmpEchoReplyInfo(); */                                            /* VCA_TCPIP_ICMP_UPDATE_PENDINGINFO */

    TcpIp_GetAddrPendingMsgOfIcmpEchoReplyInfo(icmpEchoReplyInfoIdx)->IpV4_Icmp_EchoReplyIsPending = FALSE;

# if (TCPIP_SUPPORT_ICMPV4_DST_UNREACHABLE_MSG == STD_ON)
    {
      TcpIp_IcmpDestUnreachableInfoIdxOfIpV4CtrlType icmpDestUnreachableInfoIdx = TcpIp_GetIcmpDestUnreachableInfoIdxOfIpV4Ctrl(ipV4CtrlIdx);
      /*@ assert icmpDestUnreachableInfoIdx < TcpIp_GetSizeOfIcmpDestUnreachableInfo(); */                              /* VCA_TCPIP_ICMP_UPDATE_DESTUNREACHABLEINFO */
      TcpIp_GetAddrPendingMsgOfIcmpDestUnreachableInfo(icmpDestUnreachableInfoIdx)->IpV4_Icmp_DestinationUnreachablePending = FALSE;
    }
# endif
  }
} /* IpV4_Icmp_VLocalIpAddrAssignmentChange() */

# define TCPIP_STOP_SEC_CODE
# include "TcpIp_MemMap.h"                                                                                              /* PRQA S 5087 */ /*  MD_MSR_19.1 */

/* VCA_JUSTIFICATION_START

  \ID VCA_TCPIP_ICMP_UPDATE_PENDINGINFO
    \DESCRIPTION       Valid IcmpEchoReplyInfoIdx from 0:1 indirection.
                       IpV4CtrlIdx is obtained by modelled indirection TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(LocalAddrV4Idx)
                       or passed as function argument.
                       ComStackLib generated IcmpPendingInfo is accessed using a modelled indirection
                       TcpIp_GetIcmpEchoReplyInfoIdxOfIpV4Ctrl(IpV4CtrlIdx).
                       IcmpPendingInfo is generated if TCPIP_SUPPORT_ICMPV4 is STD_ON.
                       The function is invoked only if TCPIP_SUPPORT_ICMPV4 is STD_ON, therefore this pointer write
                       operation is always valid.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib. Caller ensures that this function is only called if
                       TCPIP_SUPPORT_ICMPV4 is STD_ON.
                       Caller ensures that LocalAddrV4Idx or IpV4CtrlIdx is valid e.g. via runtime check or VCA requires
                       at function declaration or VCA asserts.

  \ID VCA_TCPIP_ICMP_UPDATE_DESTUNREACHABLEINFO
    \DESCRIPTION       Valid IcmpDestUnreachableInfoIdx from 0:1 indirection.
                       IpV4CtrlIdx is obtained by modelled indirection TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(LocalAddrV4Idx)
                       or passed as function argument.
                       ComStackLib generated IcmpDestUnreachableInfo is accessed using a modelled indirection
                       TcpIp_GetIcmpDestUnreachableInfoIdxOfIpV4Ctrl(IpV4CtrlIdx).
                       IcmpDestUnreachableInfo is generated if TCPIP_SUPPORT_ICMPV4 and
                       TCPIP_SUPPORT_ICMPV4_DST_UNREACHABLE_MSG is STD_ON.
                       The function is invoked only if TCPIP_SUPPORT_ICMPV4 and TCPIP_SUPPORT_ICMPV4_DST_UNREACHABLE_MSG
                       is STD_ON, therefore this idx is always valid.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib. Caller ensures that this function is only called if
                       TCPIP_SUPPORT_ICMPV4 and TCPIP_SUPPORT_ICMPV4_DST_UNREACHABLE_MSG is STD_ON
                       Caller ensures that LocalAddrV4Idx or IpV4CtrlIdx is valid e.g. via runtime check or VCA requires
                       at function declaration or VCA asserts.

  \ID VCA_TCPIP_STORE_ICMP_DEST_UNREACHABLE
    \DESCRIPTION       Memory write access to IcmpDestUnreachableData.
    \COUNTERMEASURE \N The maximal possible amount of bytes to copy equals 68 bytes, which is the generated size of the
                       buffer.

  VCA_JUSTIFICATION_END */

#endif /* (TCPIP_SUPPORT_ICMPV4 == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: IpV4_Icmp.c
 *********************************************************************************************************************/
