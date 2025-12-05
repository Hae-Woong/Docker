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
 *         \file  TcpIp_Priv.c
 *        \brief  Implementation of TcpIp Module - Internal Functions
 *
 *      \details  This file is part of the TcpIp module.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file of the TcpIp module. >> TcpIp.h
 *********************************************************************************************************************/


/* PRQA S 0828 EOF */ /* MD_MSR_1.1 */

#define TCPIP_PRIV_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "TcpIp_Priv.h"
#include "TcpIp.h"

#include "IpBase.h"

#include "EthSM.h"

#if (TCPIP_SUPPORT_TCP == STD_ON)
# include "TcpIp_Tcp.h"
#endif

#include "TcpIp_Udp.h"

#if (TCPIP_SUPPORT_DHCPV4 == STD_ON)
# include "TcpIp_DhcpV4.h"
#endif

#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# include "TcpIp_IpV4.h"
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
# include "TcpIp_IpV6.h"
# include "TcpIp_IpV6_Priv.h"
#endif

#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
# if(TCPIP_SUPPORT_ASRTLS_VERSION_1_2 == STD_ON)
#  include "TcpIp_Tls12Core.h"
# endif
# include "TcpIp_TlsCoreCommon.h"
# include "TcpIp_Tls.h"
# if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
#  include "TcpIp_TlsServer.h"
# endif
#endif

#include "TcpIp_Lcfg.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

#if !defined (STATIC)                                                                                                   /* COV_TCPIP_COMPATIBILITY */
# define STATIC static
#endif

/* Defines for the IPv4 pseudo header used by UDP and TCP. (see [IETF RFC768 Page 2] and [IETF RFC793 Page 17]) */
#define TCPIP_IPV4_PSEUDO_HDR_OFS_SRC_ADDR  0u
#define TCPIP_IPV4_PSEUDO_HDR_OFS_DST_ADDR  4u
#define TCPIP_IPV4_PSEUDO_HDR_OFS_RESERVED  8u
#define TCPIP_IPV4_PSEUDO_HDR_OFS_PROTOCOL  9u
#define TCPIP_IPV4_PSEUDO_HDR_OFS_LENGTH   10u
#define TCPIP_IPV4_PSEUDO_HDR_LEN          12u

#define TCPIP_IPV6_PSEUDO_HDR_OFS_SRC_ADDR  0u
#define TCPIP_IPV6_PSEUDO_HDR_OFS_DST_ADDR 16u
#define TCPIP_IPV6_PSEUDO_HDR_OFS_LENGTH   32u
#define TCPIP_IPV6_PSEUDO_HDR_OFS_RESERVED 36u
#define TCPIP_IPV6_PSEUDO_HDR_OFS_NEXT_HDR 39u
#define TCPIP_IPV6_PSEUDO_HDR_LEN          40u

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
 /* PRQA S 3453 5 */ /* MD_MSR_FctLikeMacro */
#define ETHSM_TCPIP_MODE_INDICATION(CtrlIdx, TcpIpState)      \
                                                                      (void)EthSM_TcpIpModeIndication((CtrlIdx), (TcpIpState))
/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
#define TCPIP_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

VAR(TcpIp_TcpIpErrorType, TCPIP_VAR_NO_INIT) TcpIp_LastSockError;                                                       /* PRQA S 1514 */ /* MD_TCPIP_Rule8.9_1514_1533 */

#if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_ON)                                                                         /* COV_TCPIP_MULTICORE_SUPPORT_NEVER_ON */
TCPIP_LOCAL VAR(TcpIp_OsApplicationType, TCPIP_VAR_NO_INIT) TcpIp_MainApplicationId;
#endif

#define TCPIP_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define TCPIP_START_SEC_VAR_NO_INIT_8
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (TCPIP_VENABLE_CANOE_WRITE_STRING == STD_ON)                                                                        /* COV_TCPIP_CANOE_DEBUG */
VAR(sint8, TCPIP_VAR_NO_INIT)                TcpIp_CanoeWriteStr[256];
#endif

#define TCPIP_STOP_SEC_VAR_NO_INIT_8
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define TCPIP_START_SEC_VAR_NO_INIT_16
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (TCPIP_SUPPORT_TCP == STD_ON)
VAR(uint16, TCPIP_VAR_NO_INIT)               TcpIp_Tcp_DynamicPortCount;
#endif
VAR(uint16, TCPIP_VAR_NO_INIT)               TcpIp_Udp_DynamicPortCount;

#define TCPIP_STOP_SEC_VAR_NO_INIT_16
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
/* PRQA S 0759, 1533 2 */ /* MD_MSR_Union, MD_TCPIP_Rule8.9_1514_1533 */
CONST(IpBase_AddrIn6Type, TCPIP_CONST) TcpIp_IpV6AddrUnspecified =                                                      /* PRQA S 1514 */ /* MD_TCPIP_Rule8.9_1514_1533 */
{
  {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  }
};
#endif

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#define TCPIP_START_SEC_CODE
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
/**********************************************************************************************************************
 * TcpIp_CheckRingBufferIndicesAndLength()
 *********************************************************************************************************************/
/*! \brief          Check if the given buffer indices and the length are valid
 *  \details        -
 *  \param[in]      BufferIdx       Buffer index, check that BufferStartIdx <= BufferIdx < BufferEndIdx is true
 *  \param[in]      LengthInBuffer  Length in the buffer (can be entire buffer size),
 *                                  check that LengthInBuffer <= BufferEndIdx - BufferStartIdx is true
 *  \param[in]      BufferStartIdx  Start index of the buffer, check that BufferStartIdx < BufferEndIdx is true
 *  \param[in]      BufferEndIdx    End index of the buffer
 *  \result         E_OK            All buffer indices and the length are valid
 *  \result         E_NOT_OK        At least one given parameter was not valid
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_CheckRingBufferIndicesAndLength(
  uint32 BufferIdx,
  uint32 LengthInBuffer,
  uint32 BufferStartIdx,
  uint32 BufferEndIdx);

/**********************************************************************************************************************
 * TcpIp_CheckRingBufferIndices()
 *********************************************************************************************************************/
/*! \brief          Check if the given buffer indices are valid
 *  \details        -
 *  \param[in]      BufferIdx       Buffer index, check that BufferStartIdx <= BufferIdx < BufferEndIdx is true
 *  \param[in]      BufferStartIdx  Start index of the buffer, check that BufferStartIdx < BufferEndIdx is true
 *  \param[in]      BufferEndIdx    End index of the buffer
 *  \result         E_OK            All buffer indices are valid
 *  \result         E_NOT_OK        At least one buffer index was not valid
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_CheckRingBufferIndices(
  uint32 BufferIdx,
  uint32 BufferStartIdx,
  uint32 BufferEndIdx);
#endif

/**********************************************************************************************************************
 *  TcpIp_VMatchLocalIpAddrIdAndPort()
 *********************************************************************************************************************/
/*! \brief         Checks if an IPv4/IPv6 address matches the LocalAddrIdx to which the socket is bound.
 *  \details       -
 *  \param[in]     SocketIdx          Index of a UDP or TCP socket.
 *                                    \spec requires SocketIdx < TcpIp_GetSizeOfSocketDyn(); \endspec
 *  \param[in]     RxSockLocAddrPtr:  Pointer to the socket address.
 *  \param[in]     LocalAddrIdx       Index of a local address.
 *                                    \spec requires LocalAddrIdx < TcpIp_GetSizeOfLocalAddr(); \endspec
 *  \param[out]    MatchTypePtr:      Output pointer to the matching type.
 *                                    \spec requires MatchTypePtr != NULL_PTR; \endspec
 *  \return        TRUE in case the LocalAddrIdx matches the locaAddrIdx of the bound socket, FALSE otherwise.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_VMatchLocalIpAddrIdAndPort(
  TcpIp_SocketDynIterType           SocketIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TcpIp_LocalAddrIterType           LocalAddrIdx,
  TCPIP_P2V(uint8)                  MatchTypePtr);

/* Doubly Linked List (DList) Data Structure */

/* DList raw functions */

/**********************************************************************************************************************
 *  TcpIp_DListRawAddAfter()
 *********************************************************************************************************************/
/*! \brief         Inserts a node into a node chain.
 *  \details       -
 *  \param[in,out] NodeSetPtr  Storage array of the elements of the node chain.
 *                             \spec requires NodeSetPtr != NULL_PTR; \endspec
 *  \param[in,out] FirstIdxPtr Index of the first element of the node chain.
 *                             Set to NewIdx if element is new first element.
 *                             CONSTRAINT: Must be a valid index in array referenced by NodeSetPtr or
 *                                         TCPIP_DLIST_END_IDX if chain is empty.
 *                             \spec requires FirstIdxPtr != NULL_PTR; \endspec
 *  \param[in,out] LastIdxPtr  Index of the last element of the node chain.
 *                             Set to NewIdx if element is new last element.
 *                             CONSTRAINT: Must be a valid index in array referenced by NodeSetPtr or
 *                                         TCPIP_DLIST_END_IDX if chain is empty.
 *                             \spec requires LastIdxPtr != NULL_PTR; \endspec
 *  \param[in]     NewIdx      Index of the new element that shall be inserted into the node chain.
 *                             CONSTRAINT: Must be a valid index in array referenced by NodeSetPtr.
 *  \param[in]     AfterIdx    Index of the predecessor of the newly inserted element.
 *                             CONSTRAINT: Must be a valid index in array referenced by NodeSetPtr.
 *                                         Must be an element in the virtual chain between *FirstIdxPtr and *LastIdxPtr
 *                                         or TCPIP_DLIST_END_IDX if new item shall be inserted at the beginning.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawAddAfter(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType              NewIdx,
  TcpIp_DListIdxType              AfterIdx);

/**********************************************************************************************************************
 *  TcpIp_DListRawAddRangeAfter()
 *********************************************************************************************************************/
/*! \brief         Inserts a node chain into another node chain.
 *  \details       -
 *  \param[in,out] NodeSetPtr    Storage array of the elements of the node chain.
 *                               \spec requires NodeSetPtr != NULL_PTR; \endspec
 *  \param[in,out] FirstIdxPtr   Index of the first element of the node chain.
 *                               CONSTRAINT: [0...<#Elements at NodeSetPtr>-1] or TCPIP_DLIST_END_IDX.
 *                               \spec requires FirstIdxPtr != NULL_PTR; \endspec
 *  \param[out]    LastIdxPtr    Index of the last element of the node chain.
 *                               Updated with the index of the inserted element if it was appended to the list.
 *                               \spec requires LastIdxPtr != NULL_PTR; \endspec
 *  \param[in]     NewStartIdx   Index of the first element of the node chain that shall be inserted.
 *                               CONSTRAINT: [0...<#Elements at NodeSetPtr>-1]
 *  \param[in]     NewEndIdx     Index of the last element of the node chain that shall be inserted.
 *                               CONSTRAINT: [0...<#Elements at NodeSetPtr>-1]
 *  \param[in]     AfterIdx      Index of the predecessor of the newly inserted node chain.
 *                               CONSTRAINT: [0...<#Elements at NodeSetPtr>-1] or TCPIP_DLIST_END_IDX.
 *                               If AfterIdx is TCPIP_DLIST_END_IDX the chain will be inserted at the beginning.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawAddRangeAfter(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType              NewStartIdx,
  TcpIp_DListIdxType              NewEndIdx,
  TcpIp_DListIdxType              AfterIdx);

/**********************************************************************************************************************
 *  TcpIp_DListRawRemove()
 *********************************************************************************************************************/
/*! \brief         Removes a node from a node chain.
 *  \details       -
 *  \param[in,out] NodeSetPtr    Storage array of the elements of the node chain.
 *                               \spec requires NodeSetPtr != NULL_PTR; \endspec
 *  \param[out]    FirstIdxPtr   Index of the first element of the node chain.
 *                               Updated with the index of the first element that remains in list,
 *                               if the range was removed from beginning.
 *                               \spec requires FirstIdxPtr != NULL_PTR; \endspec
 *  \param[out]    LastIdxPtr    Index of the last element of the node chain.
 *                               Updated with the index of the last element that remains in list,
 *                               if the range was removed from end.
 *                               \spec requires LastIdxPtr != NULL_PTR; \endspec
 *  \param[in]     RemIdx        Index of the element that shall be removed.
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawRemove(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType              RemIdx);

/**********************************************************************************************************************
 *  TcpIp_DListRawRemoveRange()
 *********************************************************************************************************************/
/*! \brief         Removes a sub chain from a node chain.
 *  \details       -
 *  \param[in,out] NodeSetPtr    Storage array of the elements of the node chain.
 *                               \spec requires NodeSetPtr != NULL_PTR; \endspec
 *  \param[out]    FirstIdxPtr   Index of the first element of the node chain.
 *                               Updated with the index of the first element that remains in list,
 *                               if the range was removed from beginning.
 *                               \spec requires FirstIdxPtr != NULL_PTR; \endspec
 *  \param[out]    LastIdxPtr    Index of the last element of the node chain.
 *                               Updated with the index of the last element that remains in list,
 *                               if the range was removed from end.
 *                               \spec requires LastIdxPtr != NULL_PTR; \endspec
 *  \param[in]     RemStartIdx   Index of the first element that shall be removed.
 *                               CONSTRAINT: [0...<#Elements at NodeSetPtr>-1]
 *  \param[in]     RemEndIdx     Index of the last element that shall be removed.
 *                               CONSTRAINT: [0...<#Elements at NodeSetPtr>-1]
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DListRawRemoveRange(
  TCPIP_P2V(TcpIp_DListNodeType) NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)  FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)  LastIdxPtr,
  TcpIp_DListIdxType             RemStartIdx,
  TcpIp_DListIdxType             RemEndIdx);

#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 *  TcpIp_GetBaseStateIpV6Ctrl()
 *********************************************************************************************************************/
/*! \brief         Get the TcpIpState of the IPv6 ctrl
 *  \details       -
 *  \param[in]     IpV6State   The IPv6 ctrl State
 *  \return        The related TcpIpState for the IPv6 ctrl state
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_GetBaseStateIpV6Ctrl(
  TcpIp_StateType IpV6State);
# endif
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 *  TcpIp_CompareIpV6Match()
 *********************************************************************************************************************/
/*! \brief         Verify if the new IP is a better match in compare to the current best match
 *  \details       -
 *  \param[in]     RxSockLocAddrPtr   Address of the IP header
 *  \param[in]     LocalAddr1Idx      Index of the Address of the new socket
 *  \param[in]     LocalAddr2Idx      Index of the Address of the current best matching socket
 *  \return        1u: if LocalAddr1Idx is the better match
 *                 2u: if LocalAddr2Idx is at least equal as good as LocalAddr1Idx
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(uint8, TCPIP_CODE) TcpIp_CompareIpV6Match(
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TcpIp_LocalAddrIterType           LocalAddr1Idx,
  TcpIp_LocalAddrIterType           LocalAddr2Idx);
#endif

#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 *  TcpIp_GetBaseStateIpV4Ctrl()
 *********************************************************************************************************************/
/*! \brief         Get the TcpIpState of the IPv4 ctrl
 *  \details       -
 *  \param[in]     IpV4State   The IPv4 ctrl State
 *  \return        The related TcpIpState for the IPv4 ctrl state
 *  \pre           -
 *  \context       TASK|ISR2
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_GetBaseStateIpV4Ctrl(
  TcpIp_StateType IpV4State);
# endif
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  TcpIp_MemCpySafe()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_MemCpySafe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  TCPIP_P2C(uint8) SourcePtr,
  uint32           Count)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /*
   * TargetSize and TargetPos are in not trusted relation (passed separately) so must be verified for correct relation,
   * so the subtraction can be assumed valid.
   * A placebo buffer copy operation for Count == 0 shall be allowed!
   */
  /* #10 Check that the to be written data does not exceed the available buffer space. */
  /* TargetSize == TargetPos is allowed as long as Count == 0. */
  /* \trace TCASE-TCPIP_000695 */
  /* \trace TCASE-TCPIP_000696 */
  if (   (TargetSize < TargetPos)
      || ((TargetSize - TargetPos) < Count))
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_000746 */
  else
#else
  TCPIP_DUMMY_STATEMENT_CONST(TargetSize);                                                                              /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
  {
    /* #20 Copy the given data to the given buffer */
    VStdLib_MemCpy(&TargetPtr[TargetPos], SourcePtr, Count);                                                            /* VCA_TCPIP_MEMCOPY */
  }
} /* TcpIp_MemCpySafe() */

/**********************************************************************************************************************
 *  TcpIp_MemCpyFromConstSegmentSafe()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_MemCpyFromConstSegmentSafe(
  TCPIP_P2V(uint8)          TargetPtr,
  uint32                    TargetPos,
  uint32                    TargetSize,
  TcpIp_ConstSegmentPtrType SourceSegPtr,
  uint32                    Count)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check that const segment has enough space to copy this amount of bytes from. */
  /* \trace TCASE-TCPIP_001099 */
  if (SourceSegPtr->AvailLen < Count)
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_001101 */
  else
#endif
  {
    /* #20 Copy the given data to the given buffer with a length check that we only write into the given buffer. */
    TcpIp_MemCpySafe(TargetPtr, TargetPos, TargetSize, SourceSegPtr->AvailPtr, Count);
    /* Commit the copied length into the source buffer segment. */
    TcpIp_ConstSegmentCommit(SourceSegPtr, Count);
  }
} /* TcpIp_MemCpyFromConstSegmentSafe() */

/**********************************************************************************************************************
 *  TcpIp_GetUint8FromConstSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_GetUint8FromConstSegment(
  TcpIp_ConstSegmentPtrType SourceSegPtr,
  TCPIP_P2V(uint8)          ReadUint8ValuePtr)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check that const segment has enough space to read an uint8. */
  /* \trace TCASE-TCPIP_001229 */
  if (SourceSegPtr->AvailLen < sizeof(uint8))
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_001230 */
  else
#endif
  {
    /* #20 Read the uint8 value from the source segment and return it through a pointer. */
    *ReadUint8ValuePtr = IpBase_GetUint8(SourceSegPtr->AvailPtr, 0u);
    /* Commit the copied length into the source buffer segment. */
    TcpIp_ConstSegmentCommit(SourceSegPtr, sizeof(uint8));
  }
} /* TcpIp_GetUint8FromConstSegment() */

/**********************************************************************************************************************
 *  TcpIp_MemCpyFromEndOfConstSegmentSafe()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_MemCpyFromEndOfConstSegmentSafe(
  TCPIP_P2V(uint8)          TargetPtr,
  uint32                    TargetPos,
  uint32                    TargetSize,
  TcpIp_ConstSegmentPtrType SourceSegPtr,
  uint32                    Count)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check that const segment has enough space to copy this amount of bytes from. */
  /* \trace TCASE-TCPIP_001102 */
  if (SourceSegPtr->AvailLen < Count)
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_001103 */
  else
#endif
  {
    /* The segment structure makes sure that SourceSegPtr->Size >= SourceSegPtr->AvailLen always holds,
     * therefore no underflow possible here. */
    uint32 offsetFromBase = SourceSegPtr->Size - Count;
    /* #20 Copy the given data to the given buffer with a length check that we only write into the given buffer. */
    TcpIp_MemCpySafe(TargetPtr, TargetPos, TargetSize, &SourceSegPtr->BasePtr[offsetFromBase], Count);
    /* Since data was copied from the end of the buffer the normal segment commit does not work.
     * Instead decrease both size and available length by the amount of data, AvailPtr and BasePtr stay the same.
     * The usage will also not be updated! This must be considered when calling this function. */
    SourceSegPtr->AvailLen -= Count;
    SourceSegPtr->Size     -= Count;
  }
} /* TcpIp_MemCpyFromEndOfConstSegmentSafe() */

/**********************************************************************************************************************
 *  TcpIp_MemSetSafe()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_MemSetSafe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  uint8            Pattern,
  uint32           Count)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /*
   * TargetSize and TargetPos are in not trusted relation (passed separately) so must be verified for correct relation,
   * so the subtraction can be assumed valid.
   * A placebo buffer set operation for Count == 0 shall be allowed!
   */
  /* #10 Check that the to be written data does not exceed the available buffer space. */
  /* TargetSize == TargetPos is allowed as long as Count == 0. */
  /* \trace TCASE-TCPIP_000787 */
  /* \trace TCASE-TCPIP_000788 */
  if (   (TargetSize < TargetPos)
      || ((TargetSize - TargetPos) < Count))
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_000789 */
  else
#else
  TCPIP_DUMMY_STATEMENT_CONST(TargetSize);                                                                              /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
  {
    /* #20 Copy the given data to the given buffer */
    VStdLib_MemSet(&TargetPtr[TargetPos], Pattern, Count);
  }
} /* TcpIp_MemSetSafe() */

/**********************************************************************************************************************
 *  TcpIp_PutUint64Safe()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_PutUint64Safe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  uint64           Value)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /*
   * TargetSize and TargetPos are in not trusted relation (passed separately) so must be verified for correct relation,
   * so the subtraction can be assumed valid.
   */
  /* #10 Check that the to be written data does not exceed the available buffer space. */
  /* \trace TCASE-TCPIP_000915 */
  /* \trace TCASE-TCPIP_000916 */
  if (   (TargetSize < TargetPos)
      || ((TargetSize - TargetPos) < sizeof(Value)))
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_000917 */
  else
#else
  TCPIP_DUMMY_STATEMENT_CONST(TargetSize);                                                                              /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
  {
    /* #20 Copy the given data to the given buffer */
    IpBase_PutUint64(TargetPtr, TargetPos, Value);
  }
} /* TcpIp_PutUint64Safe() */

/**********************************************************************************************************************
 *  TcpIp_PutUint32Safe()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_PutUint32Safe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  uint32           Value)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /*
   * TargetSize and TargetPos are in not trusted relation (passed separately) so must be verified for correct relation,
   * so the subtraction can be assumed valid.
   */
  /* #10 Check that the to be written data does not exceed the available buffer space. */
  /* \trace TCASE-TCPIP_000923 */
  /* \trace TCASE-TCPIP_000924 */
  if (   (TargetSize < TargetPos)
      || ((TargetSize - TargetPos) < sizeof(Value)))
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_000925 */
  else
#else
  TCPIP_DUMMY_STATEMENT_CONST(TargetSize);                                                                              /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
  {
    /* #20 Copy the given data to the given buffer */
    IpBase_PutUint32(TargetPtr, TargetPos, Value);
  }
} /* TcpIp_PutUint32Safe() */

/**********************************************************************************************************************
 *  TcpIp_PutUint16Safe()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_PutUint16Safe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  uint16           Value)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /*
   * TargetSize and TargetPos are in not trusted relation (passed separately) so must be verified for correct relation,
   * so the subtraction can be assumed valid.
   */
  /* #10 Check that the to be written data does not exceed the available buffer space. */
  /* \trace TCASE-TCPIP_000926 */
  /* \trace TCASE-TCPIP_000927 */
  if (   (TargetSize < TargetPos)
      || ((TargetSize - TargetPos) < sizeof(Value)))
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_000928 */
  else
#else
  TCPIP_DUMMY_STATEMENT_CONST(TargetSize);                                                                              /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
  {
    /* #20 Copy the given data to the given buffer */
    IpBase_PutUint16(TargetPtr, TargetPos, Value);
  }
} /* TcpIp_PutUint16Safe() */

/**********************************************************************************************************************
 *  TcpIp_PutUint8Safe()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_PutUint8Safe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  uint8            Value)
{
  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /*
   * TargetSize and TargetPos are in not trusted relation (passed separately) so must be verified for correct relation,
   * so the subtraction can be assumed valid.
   */
  /* #10 Check that the to be written data does not exceed the available buffer space. */
  /* \trace TCASE-TCPIP_000920 */
  /* \trace TCASE-TCPIP_000921 */
  if (   (TargetSize < TargetPos)
      || ((TargetSize - TargetPos) < sizeof(Value)))
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_000922 */
  else
#else
  TCPIP_DUMMY_STATEMENT_CONST(TargetSize);                                                                              /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
#endif
  {
    /* #20 Copy the given data to the given buffer */
    IpBase_PutUint8(TargetPtr, TargetPos, Value);
  }
} /* TcpIp_PutUint8Safe() */

/**********************************************************************************************************************
 *  TcpIp_RingBufferCpySafe()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_RingBufferCpySafe(
  TCPIP_P2V(uint8) TargetPtr,
  uint32           TargetPos,
  uint32           TargetSize,
  TCPIP_P2C(uint8) SourcePtr,
  uint32           Count)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SegmentType seg1;
  TcpIp_SegmentType seg2;
  uint32            seg1Len;
  uint32            seg2Len;

  /* ----- Implementation ----------------------------------------------- */
  /* A placebo buffer copy operation for Count == 0 shall be allowed! */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Check that the to be written data does not exceed the available buffer space. */
  /* \trace TCASE-TCPIP_000716 */
  /* \trace TCASE-TCPIP_000717 */
  if (   (TargetSize <= TargetPos)
      || (TargetSize < Count))
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_000744 */
  /* \trace TCASE-TCPIP_000745 */
  else
#endif
  {
    seg1Len = TargetSize - TargetPos;
    seg2Len = TargetPos;

    /* #20 Split the buffer at the given position into two buffer segments. */
    TcpIp_SegmentInit(&seg1, &TargetPtr[TargetPos], seg1Len);                                                           /* VCA_TCPIP_RINGBUFFERCOPY */
    TcpIp_SegmentInit(&seg2, &TargetPtr[0], seg2Len);                                                                   /* VCA_TCPIP_RINGBUFFERCOPY */

    /* #30 Copy as much data as possible to the first segment and the remaining data to the second segment. */
    /* TcpIp_SegmentMultiTargetMemCpy() can handle a call with Count == 0. */
    TcpIp_SegmentMultiTargetMemCpy(&seg1, &seg2, SourcePtr, Count);
  }
} /* TcpIp_RingBufferCpySafe() */

/**********************************************************************************************************************
 *  TcpIp_VerifyStartAndEndIndex()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VerifyStartAndEndIndex(
  uint32_least StartIdx,
  uint32_least EndIdx,
  uint32_least BufferSize)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify that the given indices are within the borders of the given buffer size. */
  /* \trace TCASE-TCPIP_000718 */
  /* \trace TCASE-TCPIP_000719 */
  if (   (StartIdx > EndIdx)
      || (EndIdx > BufferSize))
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_000743 */
  else
  {
    retVal = E_OK;
  }

  return retVal;

} /* TcpIp_VerifyStartAndEndIndex() */

/**********************************************************************************************************************
 *  TcpIp_GetDecrementedRingBufferIndex()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetDecrementedRingBufferIndex(
  uint32                CurrBufferIdx,
  uint32                DecrementLen,
  uint32                BufferStartIdx,
  uint32                BufferEndIdx,
  TCPIP_P2V(uint32)     DecrementedBufferIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /* Check if the given indices and length are valid. */
  /* \trace TCASE-TCPIP_001121 */
  if (TcpIp_CheckRingBufferIndicesAndLength(CurrBufferIdx, DecrementLen, BufferStartIdx, BufferEndIdx) != E_OK)
  {
    /* Invalid indices or length, retVal already E_NOT_OK */
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  else
#endif
  {
    /* Indices and length are valid */
    retVal = E_OK;
    /* \trace TCASE-TCPIP_001122 */
    if ((BufferStartIdx + DecrementLen) <= CurrBufferIdx)
    {
      /* #10 Linear case: Just decrement from the current buffer index. */
      *DecrementedBufferIdxPtr = CurrBufferIdx - DecrementLen;
    }
    /* \trace TCASE-TCPIP_001123 */
    else
    {
      /* #20 Wraparound case: Calculate the lengths at the start and end of the buffer and decrement from end index. */
      uint32 startOfBufferLen = CurrBufferIdx - BufferStartIdx;
      uint32 endOfBufferLen = DecrementLen - startOfBufferLen;
      *DecrementedBufferIdxPtr = BufferEndIdx - endOfBufferLen;
    }
  }

  return retVal;
} /* TcpIp_GetDecrementedRingBufferIndex() */

/**********************************************************************************************************************
 *  TcpIp_GetIncrementedRingBufferIndex()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetIncrementedRingBufferIndex(
  uint32                CurrBufferIdx,
  uint32                IncrementLen,
  uint32                BufferStartIdx,
  uint32                BufferEndIdx,
  TCPIP_P2V(uint32)     IncrementedBufferIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /* Check if the given indices and length are valid. */
  /* \trace TCASE-TCPIP_001118 */
  if (TcpIp_CheckRingBufferIndicesAndLength(CurrBufferIdx, IncrementLen, BufferStartIdx, BufferEndIdx) != E_OK)
  {
    /* Invalid indices or length, retVal already E_NOT_OK */
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  else
#endif
  {
    /* Indices and length are valid */
    retVal = E_OK;
    /* \trace TCASE-TCPIP_001119 */
    if ((BufferEndIdx - IncrementLen) > CurrBufferIdx)
    {
      /* #10 Linear case: Just increment from the current buffer index. */
      *IncrementedBufferIdxPtr = CurrBufferIdx + IncrementLen;
    }
    /* \trace TCASE-TCPIP_001120 */
    else
    {
      /* #20 Wraparound case: Calculate the lengths at the start and end of the buffer and increment from start index. */
      uint32 endOfBufferLen = BufferEndIdx - CurrBufferIdx;
      uint32 startOfBufferLen = IncrementLen - endOfBufferLen;
      *IncrementedBufferIdxPtr = BufferStartIdx + startOfBufferLen;
    }
  }

  return retVal;
} /* TcpIp_GetIncrementedRingBufferIndex() */

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
/**********************************************************************************************************************
 *  TcpIp_CheckRingBufferIndicesAndLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_CheckRingBufferIndicesAndLength(
  uint32 BufferIdx,
  uint32 LengthInBuffer,
  uint32 BufferStartIdx,
  uint32 BufferEndIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that the given buffer variables are valid. */
  /* \trace TCASE-TCPIP_001107 */
  /* \trace TCASE-TCPIP_001106 */
  if (   (TcpIp_CheckRingBufferIndices(BufferIdx, BufferStartIdx, BufferEndIdx) != E_OK)
      || ((BufferEndIdx - BufferStartIdx) < LengthInBuffer)) /* length in buffer = buffer length might be valid */
  {
    /* retVal already E_NOT_OK */
  }
  /* \trace TCASE-TCPIP_001108 */
  else
  {
    retVal = E_OK;
  }
  return retVal;
} /* TcpIp_CheckRingBufferIndicesAndLength() */

/**********************************************************************************************************************
 *  TcpIp_CheckRingBufferIndices()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
TCPIP_LOCAL FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_CheckRingBufferIndices(
  uint32 BufferIdx,
  uint32 BufferStartIdx,
  uint32 BufferEndIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check that the given buffer variables are valid. */
  /* \trace TCASE-TCPIP_001110 */
  /* \trace TCASE-TCPIP_001109 */
  /* \trace TCASE-TCPIP_001111 */
  if (   (BufferEndIdx <= BufferStartIdx)
      || (!TCPIP_IN_RANGE(BufferIdx, BufferStartIdx, BufferEndIdx)))
  {
    /* retVal already E_NOT_OK */
  }
  /* \trace TCASE-TCPIP_001112 */
  else
  {
    retVal = E_OK;
  }
  return retVal;
} /* TcpIp_CheckRingBufferIndices() */
#endif

/**********************************************************************************************************************
 *  TcpIp_GetSplitLengthInRingBuffer()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetSplitLengthInRingBuffer(
  uint32                CurrBufferIdx,
  uint32                PrevBufferIdx,
  uint32                BufferStartIdx,
  uint32                BufferEndIdx,
  TCPIP_P2V(uint32)     LinearLengthPtr,
  TCPIP_P2V(uint32)     EndOfBufferLengthPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /*
     C - CurrBufferIdx
     P - PrevBufferIdx
     S - BufferStartIdx
     E - BufferEndIdx

     Linear case:

     S      P               C                                    E
     v------V---------------V------------------------------------v
     |      xxxxxxxxxxxxxxxx   Buffer                            |
     -------------------------------------------------------------
     *LinearLengthPtr      = CurrBufferIdx - PrevBufferIdx;
     *EndOfBufferLengthPtr = 0u;

     Wraparound case:

     S      C                                          P         E
     v------V------------------------------------------V---------v
     |xxxxxx                   Buffer                  xxxxxxxxxx|
     -------------------------------------------------------------
     *LinearLengthPtr      = CurrBufferIdx - BufferStartIdx;
     *EndOfBufferLengthPtr = BufferEndIdx - PrevBufferIdx;

     Special wraparound case:

     C
     S                                                 P         E
     v-------------------------------------------------V---------v
     |                         Buffer                  xxxxxxxxxx|
     -------------------------------------------------------------
     *LinearLengthPtr      = CurrBufferIdx - BufferStartIdx = 0u;
     *EndOfBufferLengthPtr = BufferEndIdx - PrevBufferIdx;

   */
  /* ----- Implementation ----------------------------------------------- */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /* Check if the given indices are valid. */
  /* \trace TCASE-TCPIP_001113 */
  /* \trace TCASE-TCPIP_001114 */
  if (   (TcpIp_CheckRingBufferIndices(CurrBufferIdx, BufferStartIdx, BufferEndIdx) != E_OK)
      || (TcpIp_CheckRingBufferIndices(PrevBufferIdx, BufferStartIdx, BufferEndIdx) != E_OK))                           /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_ReadOnly */
  {
    /* Invalid indices: Set lengths to zero to prevent copy operations, retVal already E_NOT_OK */
    *LinearLengthPtr = 0u;
    *EndOfBufferLengthPtr = 0u;
    TcpIp_CallDetReportError(TCPIP_API_ID_MEMCPY_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  else
#endif
  {
    /* Indices are valid */
    retVal = E_OK;
    /* \trace TCASE-TCPIP_001116 */
    if (PrevBufferIdx <= CurrBufferIdx)
    {
      /* #10 Linear case: The whole data length is returned in LinearLengthPtr. */
      *LinearLengthPtr = CurrBufferIdx - PrevBufferIdx;
      *EndOfBufferLengthPtr = 0u;
    }
    /* \trace TCASE-TCPIP_001115 */
    else
    {
      /* #20 Wraparound case: Calculate the lengths at the start and end of the buffer. */
      *LinearLengthPtr = CurrBufferIdx - BufferStartIdx; /* Length at the start of the buffer (can be zero) */
      *EndOfBufferLengthPtr = BufferEndIdx - PrevBufferIdx;
    }
  }
  return retVal;
} /* TcpIp_GetSplitLengthInRingBuffer() */

#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 *  TcpIp_SegmentInitWithTcpTxBufferInRangeCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentInitWithTcpTxBufferInRangeCheck(
  TcpIp_SegmentPtrType          SegmentPtr,
  TCPIP_P2V(TcpIp_Uint8P2V)     BufferPtr,
  uint32                        BufferSize)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify that BufferPtr is part of the TCP Tx buffer and that there is enough space for BufferSize */
  /* \trace TCASE-TCPIP_000711 */
  if (TcpIp_TlsCoreRl_CheckTcpTxBufferInRange(*BufferPtr, BufferSize) == E_OK)
  {
    /* \trace TCASE-TCPIP_000711 */
    *BufferPtr = TcpIp_TlsCoreRl_FixGetTcpTxBuffer(*BufferPtr, BufferSize);
    if (*BufferPtr != NULL_PTR)
    {
      /* #20 Initialize the given buffer segment. */
      TcpIp_SegmentInit(SegmentPtr, *BufferPtr, BufferSize);                                                            /* VCA_TCPIP_TLS_VERIFIED_TCP_TX_BUFFER */
    }
  }
  else
  {
    *BufferPtr = NULL_PTR;
  }
} /* TcpIp_SegmentInitWithTcpTxBufferInRangeCheck() */
#endif

/**********************************************************************************************************************
 *  TcpIp_SegmentInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentInit(
  TcpIp_SegmentPtrType SegmentPtr,
  TCPIP_P2V(uint8)     Buffer,
  uint32               BufferSize)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the given buffer segment. */
  /* \trace TCASE-TCPIP_000713 */
  if (Buffer == NULL_PTR)
  {
    /* Set everything to zero / null to prevent any memory copy operations in the future. */
    SegmentPtr->BasePtr  = NULL_PTR;
    SegmentPtr->AvailPtr = NULL_PTR;
    SegmentPtr->Size     = 0;
    SegmentPtr->Usage    = 0;
    SegmentPtr->AvailLen = 0;

    TcpIp_CallDetReportError(TCPIP_API_ID_SEGMENT_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  else
  {
    SegmentPtr->BasePtr  = Buffer;
    SegmentPtr->AvailPtr = Buffer;
    SegmentPtr->Size     = BufferSize;
    SegmentPtr->Usage    = 0;
    SegmentPtr->AvailLen = BufferSize;
  }
} /* TcpIp_SegmentInit() */

/**********************************************************************************************************************
 *  TcpIp_ConstSegmentInitWithUsedSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_ConstSegmentInitWithUsedSegment(
  TcpIp_ConstSegmentPtrType         SegmentToInitPtr,
  TCPIP_P2C(TcpIp_SegmentType)      UsedSegmentPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the given buffer segment with the given used segment. */
  /* Data was written into the used segment. Now use it const segment, so read data from it.
   * The maximum amount which can be read is the amount that was written into it. */
  TcpIp_ConstSegmentInit(SegmentToInitPtr, UsedSegmentPtr->BasePtr, UsedSegmentPtr->Usage);
} /* TcpIp_ConstSegmentInitWithUsedSegment() */

/**********************************************************************************************************************
 *  TcpIp_ConstSegmentInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_ConstSegmentInit(
  TcpIp_ConstSegmentPtrType SegmentPtr,
  TCPIP_P2C(uint8)          Buffer,
  uint32                    BufferSize)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Initialize the given buffer segment. */
  if (    (Buffer == NULL_PTR)
       && (BufferSize != 0u))
  {
    /* Set everything to zero / null to prevent any memory copy operations in the future. */
    SegmentPtr->BasePtr = NULL_PTR;
    SegmentPtr->AvailPtr = NULL_PTR;
    SegmentPtr->Size = 0;
    SegmentPtr->Usage = 0;
    SegmentPtr->AvailLen = 0;

    TcpIp_CallDetReportError(TCPIP_API_ID_SEGMENT_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  else if (Buffer == NULL_PTR) /* BufferSize must be 0u */
  {
    /* Set the Segment to empty values. Only for the const segment this is allowed. */
    SegmentPtr->BasePtr = NULL_PTR;
    SegmentPtr->AvailPtr = NULL_PTR;
    SegmentPtr->Size = 0;
    SegmentPtr->Usage = 0;
    SegmentPtr->AvailLen = 0;
  }
  else
  {
    SegmentPtr->BasePtr = Buffer;
    SegmentPtr->AvailPtr = Buffer;
    SegmentPtr->Size = BufferSize;
    SegmentPtr->Usage = 0;
    SegmentPtr->AvailLen = BufferSize;
  }
} /* TcpIp_ConstSegmentInit() */

/**********************************************************************************************************************
 *  TcpIp_SegmentCommit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentCommit(
  TcpIp_SegmentPtrType SegmentPtr,
  uint32               Count)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Only call this function after the segment has already been initialized with TcpIp_SegmentInit().
   * A placebo commit operation for Count == 0 shall be allowed. */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
  /* #10 Verify that the number of bytes to be committed does not exceed the available buffer space. */
  /* This check is critical to ensure freedom of interference. */
  /* \trace TCASE-TCPIP_000714 */
  if(SegmentPtr->AvailLen < Count)
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_SEGMENT_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  else
#endif
  {
    /* #20 Update the buffer segment settings by the given amount of data. */
    SegmentPtr->Usage    += Count;
    SegmentPtr->AvailLen -= Count;

    SegmentPtr->AvailPtr = &SegmentPtr->BasePtr[SegmentPtr->Usage];
  }
} /* TcpIp_SegmentCommit() */

/**********************************************************************************************************************
 *  TcpIp_ConstSegmentCommit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_ConstSegmentCommit(
  TcpIp_ConstSegmentPtrType SegmentPtr,
  uint32                    Count)
{
  /* ----- Implementation ----------------------------------------------- */
  /* Only call this function after the segment has already been initialized with TcpIp_ConstSegmentInit().
   * A placebo commit operation for Count == 0 shall be allowed. */
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )                                                                                /* COV_TCPIP_DEV_ERROR_DETECT */
   /* #10 Verify that the number of bytes to be committed does not exceed the available buffer space. */
  /* \trace TCASE-TCPIP_001094 */
  if (SegmentPtr->AvailLen < Count)
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_SEGMENT_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  else
#endif
  {
    /* #20 Update the buffer segment settings by the given amount of data. */
    SegmentPtr->Usage    += Count;
    SegmentPtr->AvailLen -= Count;

    SegmentPtr->AvailPtr = &SegmentPtr->BasePtr[SegmentPtr->Usage];
  }
} /* TcpIp_ConstSegmentCommit() */

/**********************************************************************************************************************
 *  TcpIp_SegmentClear()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentClear(
  TcpIp_SegmentPtrType SegmentPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear the committed data by moving the buffer pointer to the next free byte position. */
  TcpIp_SegmentInit(SegmentPtr, SegmentPtr->AvailPtr, SegmentPtr->AvailLen);                                            /* VCA_TCPIP_TLS_BUFFER_SEGMENT */
} /* TcpIp_SegmentClear() */

/**********************************************************************************************************************
 *  TcpIp_ConstSegmentClear()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_ConstSegmentClear(
  TcpIp_ConstSegmentPtrType SegmentPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Clear the committed data by moving the buffer pointer to the next free byte position. */
  TcpIp_ConstSegmentInit(SegmentPtr, SegmentPtr->AvailPtr, SegmentPtr->AvailLen);
} /* TcpIp_ConstSegmentClear() */

/**********************************************************************************************************************
 *  TcpIp_SegmentMemCpy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentMemCpy(
  TcpIp_SegmentPtrType SegmentPtr,
  TCPIP_P2C(uint8)     SourcePtr,
  uint32               Count)
{
  /* ----- Implementation ----------------------------------------------- */
  /* A placebo buffer copy operation for Count == 0 shall be allowed! */
  /* #10 Copy data from the given source pointer to the given buffer segment. */
  TcpIp_MemCpySafe(SegmentPtr->BasePtr, SegmentPtr->Usage, SegmentPtr->Size, SourcePtr, Count);                         /* VCA_TCPIP_TLS_BUFFER_SEGMENT */

  /* #20 Commit the amount of written data. */
  TcpIp_SegmentCommit(SegmentPtr, Count);
} /* TcpIp_SegmentMemCpy() */

/**********************************************************************************************************************
 *  TcpIp_SegmentMemCpyFromConstSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentMemCpyFromConstSegment(
  TcpIp_SegmentPtrType        TargetSegmentPtr,
  TcpIp_ConstSegmentPtrType   SourceSegmentPtr,
  uint32                      Count)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy data from the given source segment to the given buffer segment. */
  /* The amount of read data will be committed to SourceSegmentPtr in TcpIp_MemCpyFromConstSegmentSafe(). */
  TcpIp_MemCpyFromConstSegmentSafe(TargetSegmentPtr->BasePtr, TargetSegmentPtr->Usage, TargetSegmentPtr->Size,
    SourceSegmentPtr, Count);
  /* #20 Commit the amount of written data. */
  TcpIp_SegmentCommit(TargetSegmentPtr, Count);
} /* TcpIp_SegmentMemCpyFromConstSegment() */

/**********************************************************************************************************************
 *  TcpIp_SegmentMemCpyFromEndOfConstSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentMemCpyFromEndOfConstSegment(
  TcpIp_SegmentPtrType        TargetSegmentPtr,
  TcpIp_ConstSegmentPtrType   SourceSegmentPtr,
  uint32                      Count)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy data from the end of the given source buffer segment to the given buffer segment. */
  /* No commit for the source segment necessary.
   * The size and available length of SourceSegmentPtr will be decreased in TcpIp_MemCpyFromConstSegmentSafe(),
   * the usage won't be increased! */
  TcpIp_MemCpyFromEndOfConstSegmentSafe(TargetSegmentPtr->BasePtr, TargetSegmentPtr->Usage, TargetSegmentPtr->Size,
    SourceSegmentPtr, Count);
  /* #20 Commit the amount of written data. */
  TcpIp_SegmentCommit(TargetSegmentPtr, Count);
} /* TcpIp_SegmentMemCpyFromEndOfConstSegment() */

/**********************************************************************************************************************
 *  TcpIp_SegmentMemSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentMemSet(
  TcpIp_SegmentPtrType SegmentPtr,
  uint8                Pattern,
  uint32               Count)
{
  /* ----- Implementation ----------------------------------------------- */
  /* A placebo buffer set operation for Count == 0 shall be allowed! */
  /* #10 Copy data from the given source pointer to the given buffer segment. */
  TcpIp_MemSetSafe(SegmentPtr->BasePtr, SegmentPtr->Usage, SegmentPtr->Size, Pattern, Count);

  /* #20 Commit the amount of written data. */
  TcpIp_SegmentCommit(SegmentPtr, Count);
} /* TcpIp_SegmentMemSet() */

/**********************************************************************************************************************
 *  TcpIp_SegmentPutUint64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentPutUint64(
  TcpIp_SegmentPtrType SegmentPtr,
  uint64               Value)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Put the value of the given variable into the given buffer segment. */
  TcpIp_PutUint64Safe(SegmentPtr->BasePtr, SegmentPtr->Usage, SegmentPtr->Size, Value);

  /* #20 Commit the amount of written data. */
  TcpIp_SegmentCommit(SegmentPtr, sizeof(Value));
} /* TcpIp_SegmentPutUint64() */

/**********************************************************************************************************************
 *  TcpIp_SegmentPutUint32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentPutUint32(
  TcpIp_SegmentPtrType SegmentPtr,
  uint32               Value)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Put the value of the given variable into the given buffer segment. */
  TcpIp_PutUint32Safe(SegmentPtr->BasePtr, SegmentPtr->Usage, SegmentPtr->Size, Value);

  /* #20 Commit the amount of written data. */
  TcpIp_SegmentCommit(SegmentPtr, sizeof(Value));
} /* TcpIp_SegmentPutUint32() */

/**********************************************************************************************************************
 *  TcpIp_SegmentPutUint16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentPutUint16(
  TcpIp_SegmentPtrType SegmentPtr,
  uint16               Value)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Put the value of the given variable into the given buffer segment. */
  TcpIp_PutUint16Safe(SegmentPtr->BasePtr, SegmentPtr->Usage, SegmentPtr->Size, Value);

  /* #20 Commit the amount of written data. */
  TcpIp_SegmentCommit(SegmentPtr, sizeof(Value));
} /* TcpIp_SegmentPutUint16() */

/**********************************************************************************************************************
 *  TcpIp_SegmentPutUint8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentPutUint8(
  TcpIp_SegmentPtrType SegmentPtr,
  uint8                Value)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Put the value of the given variable into the given buffer segment. */
  TcpIp_PutUint8Safe(SegmentPtr->BasePtr, SegmentPtr->Usage, SegmentPtr->Size, Value);

  /* #20 Commit the amount of written data. */
  TcpIp_SegmentCommit(SegmentPtr, sizeof(Value));
} /* TcpIp_SegmentPutUint8() */

/**********************************************************************************************************************
 *  TcpIp_SegmentMultiTargetMemCpy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SegmentMultiTargetMemCpy(
  TcpIp_SegmentPtrType Segment1Ptr,
  TcpIp_SegmentPtrType Segment2Ptr,
  TCPIP_P2C(uint8)     SourcePtr,
  uint32               Count)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* A placebo buffer copy operation for Count == 0 shall be allowed! */
  uint32 countSeg1 = TCPIP_MIN(Count, Segment1Ptr->AvailLen);
  uint32 countSeg2 = Count - countSeg1;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Copy as much data as possible to the first segment and the remaining data to the second segment. */
  TcpIp_SegmentMemCpy(Segment1Ptr, SourcePtr, countSeg1);
  TcpIp_SegmentMemCpy(Segment2Ptr, &SourcePtr[countSeg1], countSeg2);
} /* TcpIp_SegmentMultiTargetMemCpy() */

/**********************************************************************************************************************
 *  TcpIp_SegmentMemCpyFromMultiConstSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_SegmentMemCpyFromMultiConstSegment(
  TcpIp_ConstSegmentPtrType SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType SourceSegment2Ptr,
  TcpIp_SegmentPtrType      WriteSegmentPtr,
  uint32                    CopyLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 countSeg1 = 0u;
  uint32 countSeg2 = 0u;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the const segments contain enough data to read the requested length. */
  /* This check is duplicated to the check in TcpIp_MemCpyFromConstSegmentSafe(),
   * but we do not want to trigger a Det error if there is not enough space. */
  /* A placebo copy operation for CopyLen == 0 shall be allowed! */
  if (TcpIp_GetSplitLengthInMultiConstSegment(SourceSegment1Ptr, SourceSegment2Ptr, CopyLen,
        &countSeg1, &countSeg2) == E_OK)
  {
    /* #20 If so, copy the data from the const segments to the passed write segment. */
    retVal = E_OK;
    TcpIp_SegmentMemCpyFromConstSegment(WriteSegmentPtr, SourceSegment1Ptr, countSeg1);
    TcpIp_SegmentMemCpyFromConstSegment(WriteSegmentPtr, SourceSegment2Ptr, countSeg2);
  }
  return retVal;
} /* TcpIp_SegmentMemCpyFromMultiConstSegment() */

/**********************************************************************************************************************
 *  TcpIp_MultiConstSegmentInitWithSkippedDataInMultiConstSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_MultiConstSegmentInitWithSkippedDataInMultiConstSegment(
  TcpIp_ConstSegmentPtrType Segment1ToInitPtr,
  TcpIp_ConstSegmentPtrType Segment2ToInitPtr,
  TcpIp_ConstSegmentPtrType SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType SourceSegment2Ptr,
  uint32                    SkipLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 countSeg1 = 0u;
  uint32 countSeg2 = 0u;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the source segments have enough space for SkipLen and how the length is divided between the two segments */
  if (TcpIp_GetSplitLengthInMultiConstSegment(SourceSegment1Ptr, SourceSegment2Ptr, SkipLen,
    &countSeg1, &countSeg2) == E_OK)
  {
    /* #20 Initialize two segments with the data to be skipped in the source segments. */
    TcpIp_ConstSegmentInit(Segment1ToInitPtr, SourceSegment1Ptr->AvailPtr, countSeg1);
    TcpIp_ConstSegmentInit(Segment2ToInitPtr, SourceSegment2Ptr->AvailPtr, countSeg2);
    /* #30 Skip the data in the source segments. */
    /* TcpIp_SkipDataInMultiConstSegment() can't return E_NOT_OK here
     * since we already checked with TcpIp_GetSplitLengthInMultiConstSegment()
     * that there is enough space to skip the passed length. */
    retVal = TcpIp_SkipDataInMultiConstSegment(SourceSegment1Ptr, SourceSegment2Ptr, SkipLen);
  }
  return retVal;
} /* TcpIp_MultiConstSegmentInitWithSkippedDataInMultiConstSegment */

/**********************************************************************************************************************
 *  TcpIp_SkipDataInMultiConstSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_SkipDataInMultiConstSegment(
  TcpIp_ConstSegmentPtrType SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType SourceSegment2Ptr,
  uint32                    SkipLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 countSeg1 = 0u;
  uint32 countSeg2 = 0u;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the const segments contain enough data to skip the requested length. */
  /* A placebo skip operation for SkipLen == 0 shall be allowed! */
  if (TcpIp_GetSplitLengthInMultiConstSegment(SourceSegment1Ptr, SourceSegment2Ptr, SkipLen,
        &countSeg1, &countSeg2) == E_OK)
  {
    /* #20 If so, skip the data in the const segments. */
    /* To skip the data TcpIp_ConstSegmentCommit() is invoked for each const segment.
     * Added together the committed data length equals SkipLen. */
    retVal = E_OK;
    TcpIp_ConstSegmentCommit(SourceSegment1Ptr, countSeg1);
    TcpIp_ConstSegmentCommit(SourceSegment2Ptr, countSeg2);
  }
  return retVal;
} /* TcpIp_SkipDataInMultiConstSegment() */

/**********************************************************************************************************************
 *  TcpIp_GetUint8FromMultiConstSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetUint8FromMultiConstSegment(
  TcpIp_ConstSegmentPtrType           SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType           SourceSegment2Ptr,
  TCPIP_P2V(uint8)                    ReadUint8ValuePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the const segments contain enough data to read the uint8 value. */
  /* This check is duplicated to the check in TcpIp_GetUint8FromConstSegment(),
   * but we do not want to trigger a Det error if there is not enough space. */
  if (((uint64)SourceSegment2Ptr->AvailLen + (uint64) SourceSegment1Ptr->AvailLen) >= sizeof(uint8))
  {
    /* #20 If so, read the uint8 value from the const segments and return it through a pointer. */
    retVal = E_OK;
    TcpIp_ConstSegmentPtrType sourceSegmentPtr = NULL_PTR;
    if (sizeof(uint8) <= SourceSegment1Ptr->AvailLen)
    {
      sourceSegmentPtr = SourceSegment1Ptr;
    }
    else
    {
      sourceSegmentPtr = SourceSegment2Ptr;
    }
    TcpIp_GetUint8FromConstSegment(sourceSegmentPtr, ReadUint8ValuePtr);
  }
  return retVal;
} /* TcpIp_GetUint8FromMultiConstSegment() */

/**********************************************************************************************************************
 *  TcpIp_GetUint16FromMultiConstSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetUint16FromMultiConstSegment(
  TcpIp_ConstSegmentPtrType           SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType           SourceSegment2Ptr,
  TCPIP_P2V(uint16)                   ReadUint16ValuePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 linearizedValue[sizeof(uint16)] = { 0 };
  TcpIp_SegmentType linearizedValueSegment = { 0 };
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SegmentInit(&linearizedValueSegment, &linearizedValue[0], sizeof(uint16));
  /* #10 Read the uint16 value from the const segments if they contain enough data. */
  if (TcpIp_SegmentMemCpyFromMultiConstSegment(SourceSegment1Ptr, SourceSegment2Ptr,
        &linearizedValueSegment, sizeof(uint16)) == E_OK)
  {
    retVal = E_OK;
    *ReadUint16ValuePtr = IpBase_GetUint16(&linearizedValue[0], 0u);
  }
  return retVal;
} /* TcpIp_GetUint16FromMultiConstSegment() */

/**********************************************************************************************************************
 *  TcpIp_GetUint24FromMultiConstSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetUint24FromMultiConstSegment(
  TcpIp_ConstSegmentPtrType           SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType           SourceSegment2Ptr,
  TCPIP_P2V(uint32)                   ReadUint24ValuePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 linearizedValue[sizeof(uint16) + sizeof(uint8)] = { 0 };
  TcpIp_SegmentType linearizedValueSegment = { 0 };
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SegmentInit(&linearizedValueSegment, &linearizedValue[0], sizeof(uint16) + sizeof(uint8));
  /* #10 Read the uint24 value from the const segments if they contain enough data. */
  if (TcpIp_SegmentMemCpyFromMultiConstSegment(SourceSegment1Ptr, SourceSegment2Ptr,
        &linearizedValueSegment, sizeof(uint16) + sizeof(uint8)) == E_OK)
  {
    retVal = E_OK;
    *ReadUint24ValuePtr = IpBase_GetUint24(&linearizedValue[0], 0u);
  }
  return retVal;
} /* TcpIp_GetUint24FromMultiConstSegment() */

/**********************************************************************************************************************
 *  TcpIp_GetUint32FromMultiConstSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetUint32FromMultiConstSegment(
  TcpIp_ConstSegmentPtrType           SourceSegment1Ptr,
  TcpIp_ConstSegmentPtrType           SourceSegment2Ptr,
  TCPIP_P2V(uint32)                   ReadUint32ValuePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 linearizedValue[sizeof(uint32)] = { 0 };
  TcpIp_SegmentType linearizedValueSegment = { 0 };
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SegmentInit(&linearizedValueSegment, &linearizedValue[0], sizeof(uint32));
  /* #10 Read the uint32 value from the const segments if they contain enough data. */
  if (TcpIp_SegmentMemCpyFromMultiConstSegment(SourceSegment1Ptr, SourceSegment2Ptr,
        &linearizedValueSegment, sizeof(uint32)) == E_OK)
  {
    retVal = E_OK;
    *ReadUint32ValuePtr = IpBase_GetUint32(&linearizedValue[0], 0u);
  }
  return retVal;
} /* TcpIp_GetUint32FromMultiConstSegment() */

/**********************************************************************************************************************
 *  TcpIp_GetSplitLengthInMultiConstSegment()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_GetSplitLengthInMultiConstSegment(
  TCPIP_P2C(TcpIp_ConstSegmentType) SourceSegment1Ptr,
  TCPIP_P2C(TcpIp_ConstSegmentType) SourceSegment2Ptr,
  uint32                            Length,
  TCPIP_P2V(uint32)                 LengthInSegment1Ptr,
  TCPIP_P2V(uint32)                 LengthInSegment2Ptr)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* There shall be always enough space for Length == 0 */
  uint32 countSeg1 = TCPIP_MIN(Length, SourceSegment1Ptr->AvailLen);
  uint32 countSeg2 = Length - countSeg1;
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check if the const segments contain enough data. */
  if (SourceSegment2Ptr->AvailLen >= countSeg2)
  {
    /* #20 If so, calculate how the data is split between the two const segments. */
    retVal = E_OK;
    *LengthInSegment1Ptr = countSeg1;
    *LengthInSegment2Ptr = countSeg2;
  }
  else
  {
    /* retVal already E_NOT_OK, set lengths to zero */
    *LengthInSegment1Ptr = 0u;
    *LengthInSegment2Ptr = 0u;
  }
  return retVal;
} /* TcpIp_GetSplitLengthInMultiConstSegment() */

/**********************************************************************************************************************
 *  TcpIp_SegmentAssertMinLen()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_SegmentAssertMinLen(
  TCPIP_P2C(TcpIp_SegmentType) SegmentPtr,
  uint32                    Count)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify that the given buffer segment has free memory for at least 'Count' bytes. */
  /* \trace TCASE-TCPIP_000715 */
  if(SegmentPtr->AvailLen < Count)
  {
    TcpIp_CallDetReportError(TCPIP_API_ID_SEGMENT_OPERATION, TCPIP_E_INV_RUNTIME_CHECK);
  }
  /* \trace TCASE-TCPIP_000742 */
  else
  {
    retVal = E_OK;
  }

  return retVal;
} /* TcpIp_SegmentAssertMinLen() */

/**********************************************************************************************************************
 *  TcpIp_VMatchLocalIpAddrIdAndPort
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
STATIC FUNC(boolean, TCPIP_CODE) TcpIp_VMatchLocalIpAddrIdAndPort(
  TcpIp_SocketDynIterType           SocketIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TcpIp_LocalAddrIterType           LocalAddrIdx,
  TCPIP_P2V(uint8)                  MatchTypePtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfLocalAddrType localAddrBindIdx = TcpIp_GetLocalAddrBindIdxOfSocketDyn(SocketIdx);
  boolean                   retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Return TRUE if socket is bound to the same address or to a fitting broadcast/All-Node. */
  if (TcpIp_VerifyAddrIdAcceptable(localAddrBindIdx, LocalAddrIdx, MatchTypePtr) == TRUE)
  {
    /* #20 If IP address matches to bound IP address return TRUE if port is equal to the bound port of the socket. */
    retVal = IpBase_SockPortIsEqual(&TcpIp_GetAddrLocSockOfSocketDyn(SocketIdx)->SockAddr, &RxSockLocAddrPtr->SockAddr);
  }
  else
  {
    /* Socket is bound to invalid address. */
  }

  return retVal;
} /* TcpIp_VMatchLocalIpAddrIdAndPort() */

/**********************************************************************************************************************
 *  TcpIp_VRxUdpSockIdxIdent
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(TcpIp_SocketDynIterType, TCPIP_CODE) TcpIp_VRxUdpSockIdxIdent(
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TcpIp_LocalAddrIterType           LocalAddrIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SocketDynIterType socketIdx;
  TcpIp_SocketDynIterType matchSocketIdx = TCPIP_SOCKET_IDX_INV;
  uint8 bestMatchType = TCPIP_ADDR_MATCH_NONE + 1u;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(RxSockLocAddrPtr != NULL_PTR);
  TCPIP_ASSERT(LocalAddrIdx < TcpIp_GetSizeOfLocalAddr());

  /* #10 Iterate over all UDP sockets. */
  for (socketIdx = 0; socketIdx < TcpIp_GetSizeOfSocketUdpDyn(); socketIdx++)
  {
    /* #20 Check if UDP socket is bound. */
    if (TcpIp_GetListenActiveConnStatOfSocketDyn(socketIdx) == TCPIP_SOCK_CONN_LISTEN_ENABLED)
    {
      /* #30 Check if socket is bound to a LocalAddrId and a port that match the specified IP address and port. */
      uint8 currentMatchType = 0u;
      if (TcpIp_VMatchLocalIpAddrIdAndPort(socketIdx, RxSockLocAddrPtr, LocalAddrIdx, &currentMatchType) == TRUE)
      {
        if (bestMatchType > currentMatchType)
        {
          matchSocketIdx = socketIdx;
          bestMatchType = currentMatchType;
          if (bestMatchType == TCPIP_ADDR_MATCH_EXACT)
          {
            break;
          }
        }
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
        else if (   (currentMatchType == TCPIP_ADDR_MATCH_INTERFACE)
                 && (RxSockLocAddrPtr->sa_family == IPBASE_AF_INET6)
                 && (TcpIp_CompareIpV6Match(RxSockLocAddrPtr, TcpIp_GetLocalAddrBindIdxOfSocketDyn(socketIdx),          /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_ReadOnly */
                                            TcpIp_GetLocalAddrBindIdxOfSocketDyn(matchSocketIdx)) == 1u ))
        {
          matchSocketIdx = socketIdx;
          bestMatchType = currentMatchType;
        }
#endif
        else
        {
          /* do nothing */
        }
      }
    }
  }

  return matchSocketIdx;
}
/* PRQA L:CTRL_NEST_LIMIT */ /* MD_MSR_1.1_0715 */

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 *  TcpIp_CompareIpV6Match
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, TCPIP_CODE) TcpIp_CompareIpV6Match(
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TcpIp_LocalAddrIterType           LocalAddr1Idx,
  TcpIp_LocalAddrIterType           LocalAddr2Idx)
{
  uint8 match = 2u;
  /* #10 Get source address and compare prefix length */
  if (   (TcpIp_IsIpV6SourceAddressUsedOfLocalAddrV6(TCPIP_LOCAL_ADDR_IDX_TO_IPV6(LocalAddr1Idx)) == TRUE)
      && (TcpIp_IsIpV6SourceAddressUsedOfLocalAddrV6(TCPIP_LOCAL_ADDR_IDX_TO_IPV6(LocalAddr2Idx)) == TRUE))
  {
    TcpIp_SizeOfIpV6SourceAddressTableEntryType srcAddr1Idx = TcpIp_GetIpV6SourceAddressIdxOfLocalAddrV6(
                                                                TCPIP_LOCAL_ADDR_IDX_TO_IPV6(LocalAddr1Idx));
    TcpIp_SizeOfIpV6SourceAddressTableEntryType srcAddr2Idx = TcpIp_GetIpV6SourceAddressIdxOfLocalAddrV6(
                                                                TCPIP_LOCAL_ADDR_IDX_TO_IPV6(LocalAddr2Idx));

    if (   IpV6_VSourceAddrIsBetterThan(srcAddr1Idx, srcAddr2Idx, &RxSockLocAddrPtr->SockAddrIn6.sin6_addr)
        == IPV6_CMP_RES_PREFER_A)
    {
      match = 1u;
    }
  }
  return match;
}
#endif

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VRxTcpSockIdxIdent
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(TcpIp_SocketDynIterType, TCPIP_CODE) TcpIp_VRxTcpSockIdxIdent(
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockRemAddrPtr,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TcpIp_LocalAddrIterType           LocalAddrIdx)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */

  /* #10 Search for a bound matching socket. */
  TcpIp_SocketDynIterType socketMatchIdx = TcpIp_SearchTcpComSocket(LocalAddrIdx, RxSockLocAddrPtr, RxSockRemAddrPtr);

  /* #20 If no bound socket is found, search for a fitting listen socket. */
  if (socketMatchIdx == TCPIP_SOCKET_IDX_INV)
  {
    socketMatchIdx = TcpIp_SearchTcpListenSocket(RxSockLocAddrPtr, LocalAddrIdx, TRUE);
  }

  return socketMatchIdx;
}
#endif

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_SetLastSockError
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_SetLastSockError(TcpIp_TcpIpErrorType Error)
{
  /* #10 Store the given error code 'Error' in the error variable that can be read out by the application. */
  TcpIp_LastSockError = Error;
}
#endif

/**********************************************************************************************************************
 *  TcpIp_VInitSockets
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VInitSockets(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfSocketDynType socketIdx;

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_LastSockError = 0;
#if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_ON)                                                                         /* COV_TCPIP_MULTICORE_SUPPORT_NEVER_ON */
  /* Set the application calling init as the main application */
  TcpIp_MainApplicationId = GetApplicationID();
#endif

  /* #10 Iterate all configured sockets and call the internal initialization function for each socket. */
  for (socketIdx = 0; socketIdx < TcpIp_GetSizeOfSocketDyn(); socketIdx++)
  {
    TcpIp_VInitSocket(socketIdx);
  }
}

/**********************************************************************************************************************
 *  TcpIp_VInitSocket
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VInitSocket(
  TcpIp_SocketDynIterType SocketIdx )
{
  /* ----- Implementation ----------------------------------------------- */
  TcpIp_VDelSockAddrMapping(SocketIdx);
  /* #10 Initialize all admin data of the socket identified by the given socket index. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  if (TCPIP_SOCKET_IDX_IS_TCP(SocketIdx))
  {
    TcpIp_Tcp_VClearTxBuffer(TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx));
  }
  else
#endif
  {
    TcpIp_SetTxBufRequestedOfSocketDyn(SocketIdx, FALSE);
  }

  TcpIp_SetListenActiveConnStatOfSocketDyn(SocketIdx, TCPIP_SOCK_CONN_NONE);
  TcpIp_SetSocketOwnerConfigIdxOfSocketDyn(SocketIdx, TCPIP_NO_SOCKETOWNERCONFIGIDXOFSOCKETDYN);
  TcpIp_VPreconfigSocket(SocketIdx);
}

/**********************************************************************************************************************
 *  TcpIp_VDelSockAddrMapping
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
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_VDelSockAddrMapping(
  TcpIp_SocketDynIterType SocketIdx )
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(TCPIP_SOCKET_ID_IS_VALID(TCPIP_SOCKET_IDX_TO_ID(SocketIdx)));

  /* #10 Clear sockets local and remote socket address value. */
  TcpIp_GetAddrRemSockOfSocketDyn(SocketIdx)->sa_family = IPBASE_AF_UNSPEC;
  TcpIp_GetAddrLocSockOfSocketDyn(SocketIdx)->sa_family = IPBASE_AF_UNSPEC;

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
  (void)IpBase_CopyIpV6Addr(&TcpIp_GetAddrRemSockOfSocketDyn(SocketIdx)->SockAddrIn6.sin6_addr, &TcpIp_IpV6AddrUnspecified);
  (void)IpBase_CopyIpV6Addr(&TcpIp_GetAddrLocSockOfSocketDyn(SocketIdx)->SockAddrIn6.sin6_addr, &TcpIp_IpV6AddrUnspecified);
#else
  TcpIp_GetAddrRemSockOfSocketDyn(SocketIdx)->SockAddrIn.sin_addr = TCPIP_INADDR_ANY;
  TcpIp_GetAddrLocSockOfSocketDyn(SocketIdx)->SockAddrIn.sin_addr = TCPIP_INADDR_ANY;
#endif

  TcpIp_GetAddrRemSockOfSocketDyn(SocketIdx)->DomainAndPort.port = TCPIP_PORT_NOT_SET;
  TcpIp_GetAddrLocSockOfSocketDyn(SocketIdx)->DomainAndPort.port = TCPIP_PORT_NOT_SET;

  /* #20 Clear socket owner and connection state. */
  TcpIp_SetSocketOwnerConfigIdxOfSocketDyn(SocketIdx, TCPIP_NO_SOCKETOWNERCONFIGIDXOFSOCKETDYN);
  TcpIp_SetListenActiveConnStatOfSocketDyn(SocketIdx, TCPIP_SOCK_CONN_NONE);

  /* #30 Reset TCP related parameters, if socket is a TCP socket. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  if (TCPIP_SOCKET_IDX_IS_TCP(SocketIdx))
  {
    TcpIp_SizeOfSocketTcpDynType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx);
    TcpIp_SetMaxNumListenSocketsOfSocketTcpDyn(socketTcpIdx, 0);
    TcpIp_SetSocketTcpDynMasterListenSocketIdxOfSocketTcpDyn(socketTcpIdx,
      TCPIP_NO_SOCKETTCPDYNMASTERLISTENSOCKETIDXOFSOCKETTCPDYN);
  }
#endif

  /* #40 Reset IP address binding to ANY. */
  /* Socket has ANY binding. This means use any IP on any controller for transmission */
  TcpIp_SetTxIpAddrIdxOfSocketDyn(     SocketIdx, TCPIP_IPVX_ADDR_IDX_ANY);
  TcpIp_SetLocalAddrBindIdxOfSocketDyn(SocketIdx, TCPIP_LOCALADDRID_ANY);

  /* #50 Reset socket specific parameters in IP submodules. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
  IpV4_Ip_ResetSocket(SocketIdx);
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
  IpV6_ResetSocket(SocketIdx);
#endif

#if (TCPIP_MULTICORE_SUPPORT_ENABLED == STD_ON)                                                                         /* COV_TCPIP_MULTICORE_SUPPORT_NEVER_ON */
  /* Reset all sockets to the main application */
  TcpIp_SetApplIdOfSocketDyn(SocketIdx, TcpIp_MainApplicationId);
#endif
} /* TcpIp_VDelSockAddrMapping() */

/**********************************************************************************************************************
 *  TcpIp_VSockIpAddrIsEqual
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_VSockIpAddrIsEqual(
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockAPtr,
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean match = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SockAPtr != NULL_PTR);
  TCPIP_ASSERT(SockBPtr != NULL_PTR);

  /* #10 Check the socket families to be equal (basic precondition for comparison). */
  if (SockAPtr->sa_family == SockBPtr->sa_family)
  {
    /* #20 Depending on the address family, compare address and port of the given sockets. */
    switch (SockAPtr->sa_family)
    {
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
    case IPBASE_AF_INET:
    {
      match = (boolean)(SockAPtr->SockAddrIn.sin_addr == SockBPtr->SockAddrIn.sin_addr);                                /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    }
    break;
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
    case IPBASE_AF_INET6:
    {
      /* PRQA S 4304 4 */ /* MD_MSR_AutosarBoolean */
      match = (boolean)(   (SockAPtr->SockAddrIn6.sin6_addr.addr32[0] == SockBPtr->SockAddrIn6.sin6_addr.addr32[0])
                        && (SockAPtr->SockAddrIn6.sin6_addr.addr32[1] == SockBPtr->SockAddrIn6.sin6_addr.addr32[1])
                        && (SockAPtr->SockAddrIn6.sin6_addr.addr32[2] == SockBPtr->SockAddrIn6.sin6_addr.addr32[2])
                        && (SockAPtr->SockAddrIn6.sin6_addr.addr32[3] == SockBPtr->SockAddrIn6.sin6_addr.addr32[3]));
    }
    break;
#endif
    default:
      /* No match: Leave match at value FALSE. */
      break;
    }
  }

  return match;
} /* TcpIp_VSockIpAddrIsEqual() */

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VSockAddrIpAndPortIsEqual
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_VSockAddrIpAndPortIsEqual(
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockAPtr,
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockBPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean match = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SockAPtr != NULL_PTR);
  TCPIP_ASSERT(SockBPtr != NULL_PTR);
  /* #10 Compare ports. */
  if (SockAPtr->DomainAndPort.port == SockBPtr->DomainAndPort.port)
  {
    /* #20 Compare IP addresses. */
    match = TcpIp_VSockIpAddrIsEqual(SockAPtr, SockBPtr);
  }

  return match;
}

/**********************************************************************************************************************
 *  TcpIp_VNetAddrIsUnicast
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_VNetAddrIsUnicast(
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Determine the domain of the socket address. */
  switch(SockAddrPtr->sa_family)
  {
# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
  case IPBASE_AF_INET:
    {
      /* #20 For an IPv4 socket address check the address to be valid, non-multicast and non-broadcast. */
      /* socket address struct is always filled in network byte order */
      TcpIp_NetAddrType SockNetAddr = SockAddrPtr->SockAddrIn.sin_addr;

      if( (0u != SockNetAddr) /* invalid global '0' address */ &&
          (!IPV4_ADDR_IS_MULTICAST(SockNetAddr)) /* multicast address */ &&
          (SockNetAddr != TCPIP_INADDR_BROADCAST) /* global broadcast */
          /* subnet broadcasts can not be detected here */ )
      {
        retVal = TRUE;
      }
      else
      {
        retVal = FALSE;
      }
      break;
    }
# endif
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  case IPBASE_AF_INET6:
    {
      /* #30 For an IPv6 socket address check the first byte of the address (in network byte order) to be non-0xff. */
      /* Read the first by of the address (network byte order). */
      P2CONST(uint8, AUTOMATIC, TCPIP_APPL_VAR) firstBytePtr = &SockAddrPtr->SockAddrIn6.sin6_addr.addr[0];
      if ((*firstBytePtr  != 0xFFu))
      {
        retVal = TRUE;
      }
      else
      {
        retVal = FALSE;
      }
      break;
    }
# endif
  default:
    {
      retVal = FALSE;  /* no valid domain type */
      break;
    }
  }

  return retVal;
} /* TcpIp_VNetAddrIsUnicast() */

/**********************************************************************************************************************
 *  TcpIp_VLocalAddrIdxIsUnicast
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_VLocalAddrIdxIsUnicast(
  TcpIp_LocalAddrIterType LocalAddrIdx)
{
  boolean retVal = FALSE;

  /* #10 Validate address in case of IPv4 */
# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
  /* This check is also in IPv4-only stacks required to verify that the socket is not bound to ANY. */
  if(TCPIP_LOCAL_ADDR_IDX_IS_IPV4(LocalAddrIdx))
  {
    TcpIp_LocalAddrV4IterType localAddrV4Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(LocalAddrIdx);

    if (TcpIp_GetUnicastAddrV4IdxOfLocalAddrV4(localAddrV4Idx) < TcpIp_GetSizeOfUnicastAddrV4())
    {
      retVal = TRUE;
    }
  }
# endif

  /* #20 Validate address in case of IPv6 */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  /* This check is also in IPv6-only stacks required to verify that the socket is not bound to ANY. */
  if(TCPIP_LOCAL_ADDR_IDX_IS_IPV6(LocalAddrIdx))
  {
    TcpIp_LocalAddrV6IterType localAddrV6Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV6(LocalAddrIdx);

    if (TcpIp_GetIpV6SourceAddressIdxOfLocalAddrV6(localAddrV6Idx) < TcpIp_GetSizeOfIpV6SourceAddress())
    {
      retVal = TRUE;
    }
  }
# endif

  /* Accept TCPIP_LOCALADDRID_ANY. */
  /* The unicast IP address is set later in context of TcpIp_Tcp_VTransmitSegment_RequestIpTxBuffer. */
  if (LocalAddrIdx == TCPIP_LOCALADDRID_ANY)
  {
    retVal = TRUE;
  }

  return retVal;
}
#endif

#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
/**********************************************************************************************************************
 *  TcpIp_VCalcIpV4PseudoHdrChecksum
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint32, TCPIP_CODE) TcpIp_VCalcIpV4PseudoHdrChecksum(
  IpBase_AddrInType RemAddr,
  IpBase_AddrInType LocAddr,
  uint8             Protocol,
  uint16            ProtocolPayloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8             ipV4PseudoHdr[TCPIP_IPV4_PSEUDO_HDR_LEN];
  IpBase_AddrInType remIpAddrHbo = TCPIP_NTOHL(RemAddr);
  IpBase_AddrInType locIpAddrHbo = TCPIP_NTOHL(LocAddr);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Create IPv4 pseudo header. */
  TCPIP_PUT_UINT32(ipV4PseudoHdr, TCPIP_IPV4_PSEUDO_HDR_OFS_SRC_ADDR, remIpAddrHbo);
  TCPIP_PUT_UINT32(ipV4PseudoHdr, TCPIP_IPV4_PSEUDO_HDR_OFS_DST_ADDR, locIpAddrHbo);
  ipV4PseudoHdr[TCPIP_IPV4_PSEUDO_HDR_OFS_RESERVED] = 0;
  ipV4PseudoHdr[TCPIP_IPV4_PSEUDO_HDR_OFS_PROTOCOL] = Protocol;
  TCPIP_PUT_UINT16(ipV4PseudoHdr, TCPIP_IPV4_PSEUDO_HDR_OFS_LENGTH, ProtocolPayloadLen);
  /* #20 Call IpBase_TcpIpChecksumAdd() and return not finalized checksum. */
  return IpBase_TcpIpChecksumAdd(&ipV4PseudoHdr[0], sizeof(ipV4PseudoHdr), 0, FALSE);
} /* TcpIp_VCalcIpV4PseudoHdrChecksum() */
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 *  TcpIp_VCalcIpV6PseudoHdrChecksum
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint32, TCPIP_CODE) TcpIp_VCalcIpV6PseudoHdrChecksum(
  TCPIP_P2C(IpBase_AddrIn6Type) RemAddrPtr,
  TCPIP_P2C(IpBase_AddrIn6Type) LocAddrPtr,
  uint8                         Protocol,
  uint16                        ProtocolPayloadLen)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 ipV6PseudoHdr[TCPIP_IPV6_PSEUDO_HDR_LEN];

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Create IPv6 pseudo header. */
  /* PRQA S 0315 2 */ /* MD_MSR_VStdLibCopy */
  IpBase_Copy(&ipV6PseudoHdr[TCPIP_IPV6_PSEUDO_HDR_OFS_SRC_ADDR], &RemAddrPtr->addr[0], IPV6_ADDRESS_LEN_BYTE);
  IpBase_Copy(&ipV6PseudoHdr[TCPIP_IPV6_PSEUDO_HDR_OFS_DST_ADDR], &LocAddrPtr->addr[0], IPV6_ADDRESS_LEN_BYTE);
  TCPIP_PUT_UINT32(ipV6PseudoHdr, TCPIP_IPV6_PSEUDO_HDR_OFS_LENGTH, ProtocolPayloadLen);
  ipV6PseudoHdr[TCPIP_IPV6_PSEUDO_HDR_OFS_RESERVED]      = 0u;
  ipV6PseudoHdr[TCPIP_IPV6_PSEUDO_HDR_OFS_RESERVED + 1u] = 0u;
  ipV6PseudoHdr[TCPIP_IPV6_PSEUDO_HDR_OFS_RESERVED + 2u] = 0u;
  ipV6PseudoHdr[TCPIP_IPV6_PSEUDO_HDR_OFS_NEXT_HDR] = Protocol;

  /* #20 Call IpBase_TcpIpChecksumAdd() and return not finalized checksum. */
  return IpBase_TcpIpChecksumAdd(&ipV6PseudoHdr[0], sizeof(ipV6PseudoHdr), 0u, FALSE);
} /* TcpIp_VCalcIpV6PseudoHdrChecksum() */
#endif

/**********************************************************************************************************************
 *  TcpIp_VCalcPseudoHdrAndChecksum
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint16, TCPIP_CODE) TcpIp_VCalcPseudoHdrAndChecksum(
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockRemAddrPtr,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TCPIP_P2C(uint8)                  DataPtr,
  uint16                            DataLen,
  uint8                             Protocol)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 checksum = 0u;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(DataPtr != NULL_PTR);
  TCPIP_ASSERT(TCPIP_IS_ADDR_FAMILY_SUPPORTED(RxSockRemAddrPtr->sa_family) == TRUE);
  TCPIP_ASSERT(RxSockRemAddrPtr->sa_family == RxSockLocAddrPtr->sa_family);

  /* #10 Calculate the checksum of the IP specific pseudo header. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  if (IPBASE_AF_INET == RxSockRemAddrPtr->sa_family)
# endif
  {
    /* IpV4 */
    checksum = TcpIp_VCalcIpV4PseudoHdrChecksum(RxSockRemAddrPtr->SockAddrIn.sin_addr, RxSockLocAddrPtr->SockAddrIn.sin_addr, Protocol, DataLen);
  }
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  else
# endif
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
  {
    /* IpV6 */
    checksum = TcpIp_VCalcIpV6PseudoHdrChecksum(&RxSockRemAddrPtr->SockAddrIn6.sin6_addr, &RxSockLocAddrPtr->SockAddrIn6.sin6_addr, Protocol, DataLen);
  }
#endif

  /* #20 Add the checksum of the payload and finalize checksum to 16 bit value. */
  checksum = IpBase_TcpIpChecksumAdd(DataPtr, DataLen, checksum, TRUE);

  return (uint16)checksum;
}

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 *  TcpIp_VDuplicateAddrDetected
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VDuplicateAddrDetected(
  TcpIp_LocalAddrIdType   LocalAddrId,
  TCPIP_P2C(uint8)        SrcAddrPtr,
  TCPIP_P2C(uint8)        RemotePhysAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Eth_PhysAddrType          LocalPhysAddr = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(TCPIP_LOCAL_ADDR_ID_IS_VALID(LocalAddrId));

  /* #10 Verify that RemotePhysAddrPtr does not match the physical address of this node. */
  if (TcpIp_GetPhysAddr(LocalAddrId, &LocalPhysAddr[0]) == E_OK)
  {
    if (TcpIp_CmpLLAddr(RemotePhysAddrPtr, LocalPhysAddr) == FALSE)
    {
# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
      TcpIp_SizeOfLocalAddrType localAddrIdx;
      localAddrIdx = TCPIP_LOCAL_ADDR_ID_TO_IDX(LocalAddrId);

      /* #20 Verify that LocalAddrId references an IPv6 address. */
      if (TCPIP_LOCAL_ADDR_IDX_IS_IPV6(localAddrIdx))                                                                   /* PRQA S 3415 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */ /*lint !e506 */
# endif
      {
        TcpIp_SockAddrBaseType sockAddr;                                                                                /* PRQA S 0759 */ /* MD_MSR_Union */
        /* PRQA S 0315 1 */ /* MD_MSR_VStdLibCopy */
        IPV6_ADDR_COPY(sockAddr.SockAddrIn6.sin6_addr.addr[0], *SrcAddrPtr);
        sockAddr.SockAddrIn6.sin6_port = TCPIP_PORT_ANY;
        sockAddr.SockAddrIn6.sin6_family = IPBASE_AF_INET6;

        /* #30 Inform upper layers about a DADCONFLICT, if callback is configured. */
        if (TcpIp_GetDuplicateAddrDetectionFctPtr() != NULL_PTR)
        {
          TcpIp_GetDuplicateAddrDetectionFctPtr()(LocalAddrId, (TCPIP_P2C(TcpIp_SockAddrType)) &sockAddr.TcpIpSockAddr, &LocalPhysAddr[0], RemotePhysAddrPtr);
          TCPIP_DUMMY_STATEMENT(sockAddr.SockAddrIn6);                                                                  /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */ /*lint -e{438} */
        }
      }
    }
  }
} /* TcpIp_VDuplicateAddrDetected() */
#endif

 /**********************************************************************************************************************
 *  TcpIp_VPhysAddrTableChg
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VPhysAddrTableChg(
  uint8                             CtrlIdx,
  TCPIP_P2V(TcpIp_SockAddrBaseType) IpAddrPtr,
  TCPIP_P2V(uint8)                  PhysAddrPtr,
  boolean                           Valid)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_PhysAddrConfigIterType physAddrConfigIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 In case UDP queue for address cache misses is configured, clear pending UDP retry elements for unresolved destinations. */
  if (Valid == FALSE)
  {
    TcpIp_Udp_CancelRetriesForDestination(CtrlIdx, IpAddrPtr);
  }

  /* #20 Inform upper layers about the change in the physical address resolution table. */
  for (physAddrConfigIdx = 0; physAddrConfigIdx < TcpIp_GetSizeOfPhysAddrConfig(); physAddrConfigIdx++)
  {
    TcpIp_PhysAddrTableChgCbkType funcPtr = TcpIp_GetChgFuncPtrOfPhysAddrConfig(physAddrConfigIdx);

    if (funcPtr != NULL_PTR)
    {
      funcPtr(CtrlIdx, &IpAddrPtr->TcpIpSockAddr, PhysAddrPtr, Valid);
    }
  }
} /* TcpIp_VPhysAddrTableChg() */

/**********************************************************************************************************************
 *  TcpIp_RxIndicationFunctions
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_RxIndicationFunctions(
  TcpIp_SocketDynIterType           SocketIdx,
  TCPIP_P2V(TcpIp_SockAddrBaseType) RemoteSockAddrPtr,
  TCPIP_P2V(uint8)                  DataPtr,
  uint16                            DataLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfSocketOwnerConfigType socketOwnerConfigIdx = TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(SocketIdx);

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketIdx          < TcpIp_GetSizeOfSocketDyn());
  TCPIP_ASSERT(RemoteSockAddrPtr != NULL_PTR);
  TCPIP_ASSERT(DataPtr           != NULL_PTR);

  if (socketOwnerConfigIdx < TcpIp_GetSizeOfSocketOwnerConfig())
  {
    TcpIp_SocketIdType socketId = TCPIP_SOCKET_IDX_TO_ID(SocketIdx);
    TcpIp_SocketOwnerRxIndicationType rxIndicationFuncPtr = TcpIp_GetRxIndicationFuncPtrOfSocketOwnerConfig(socketOwnerConfigIdx);


    /* #10 In case the index is valid, inform upper layers about the finalization of the reception on the given socket. */
    if (rxIndicationFuncPtr != NULL_PTR)
    {
      rxIndicationFuncPtr(socketId, &RemoteSockAddrPtr->TcpIpSockAddr, DataPtr, DataLenByte);
    }
    else
    {
      /* #20 Issue a DET error in case the mandatory callback to do that is not configured. */
      /* ERROR: Callback is mandatory. */
      TcpIp_CallDetReportError(TCPIP_API_ID_CBK_RX_INDICATION, TCPIP_E_CBK_REQUIRED);
    }
  }
  else
  {
    TCPIP_ASSERT_UNREACHABLE();
  }
}

/**********************************************************************************************************************
 *  TcpIp_TxConfirmationFunctions
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_TxConfirmationFunctions(
  TcpIp_SocketDynIterType       SocketIdx,
  uint16                        DataLenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfSocketOwnerConfigType socketOwnerConfigIdx = TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(SocketIdx);

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketIdx < TcpIp_GetSizeOfSocketDyn());

  if (socketOwnerConfigIdx < TcpIp_GetSizeOfSocketOwnerConfig())
  {
    TcpIp_SocketIdType socketId = TCPIP_SOCKET_IDX_TO_ID(SocketIdx);
    TcpIp_SocketOwnerTxConfirmationType txConfirmationFuncPtr = TcpIp_GetTxConfirmationFuncPtrOfSocketOwnerConfig(socketOwnerConfigIdx);

    /* #10 Inform the socket owner about the amount of transmitted data, if callback is configured. */
    if (txConfirmationFuncPtr != NULL_PTR)
    {
      txConfirmationFuncPtr(socketId, DataLenByte);
    }
  }
  else
  {
    TCPIP_ASSERT_UNREACHABLE();
  }
}

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_TcpAcceptedFunctions
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_TcpAcceptedFunctions(
  TcpIp_SocketOwnerConfigIterType SocketOwnerConfigIdx,
  TcpIp_SocketTcpDynIterType      SocketTcpIdx,
  TcpIp_SocketTcpDynIterType      ConnectedSocketTcpIdx,
  TCPIP_P2V(TcpIp_SockAddrType)   RemoteSockAddrPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType     retVal = E_NOT_OK;
  TcpIp_SocketIdType socketId = TCPIP_SOCKET_IDX_TO_ID(TCPIP_TCP_IDX_TO_SOCKET_IDX(SocketTcpIdx));
  TcpIp_SocketIdType socketIdConnected = TCPIP_SOCKET_IDX_TO_ID(TCPIP_TCP_IDX_TO_SOCKET_IDX(ConnectedSocketTcpIdx));

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketOwnerConfigIdx  < TcpIp_GetSizeOfSocketOwnerConfig());
  TCPIP_ASSERT(SocketTcpIdx          < TcpIp_GetSizeOfSocketTcpDyn());
  TCPIP_ASSERT(ConnectedSocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn());
  TCPIP_ASSERT(RemoteSockAddrPtr != NULL_PTR);

# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON )
  /* If the socket is assigned to a TLS connection, forward the call to the TLS core. */
  if (TcpIp_IsUseTlsOfSocketTcpDyn(SocketTcpIdx) == TRUE)
  {
    retVal = TcpIp_Tls12CoreLl_TcpAccepted(SocketTcpIdx, ConnectedSocketTcpIdx);
  }
  else
#  endif
# endif
  {
    TcpIp_SocketOwnerTcpAcceptedType tcpAcceptedFuncPtr = TcpIp_GetTcpAcceptedFuncPtrOfSocketOwnerConfig(SocketOwnerConfigIdx);

    /* #10 In case the index is valid and a callout is configured for that user, inform the socket user about the acceptance of the socket. */
    if (tcpAcceptedFuncPtr != NULL_PTR)
    {
      retVal = tcpAcceptedFuncPtr(socketId, socketIdConnected, RemoteSockAddrPtr);
    }
    /* #20 Otherwise issue a DET error. */
    else
    {
      /* ERROR: Callback is mandatory. */
      TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TCP_ACCEPTED, TCPIP_E_CBK_REQUIRED);
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_TcpConnectedFunctions
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_TcpConnectedFunctions(
  TcpIp_SocketTcpDynIterType SocketTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SizeOfSocketOwnerConfigType socketOwnerConfigIdx = 0u;
  TcpIp_SocketDynIterType           socketIdx  = TCPIP_TCP_IDX_TO_SOCKET_IDX(SocketTcpIdx);

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn());

  socketIdx = TCPIP_TCP_IDX_TO_SOCKET_IDX(SocketTcpIdx);

  socketOwnerConfigIdx = TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(socketIdx);
  /*@ assert socketOwnerConfigIdx < TcpIp_GetSizeOfSocketOwnerConfig(); */                                              /* VCA_TCPIP_SOCKETOWNERIDX */

# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
#  if (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON)
  /* Check if socket supports Tls */
  if(TcpIp_IsUseTlsOfSocketTcpDyn(SocketTcpIdx) == TRUE)
  {
    /* Tcp connection has been made, trigger TLS client handshake */
    if (TcpIp_TlsCoreLl_TcpConnected((TcpIp_SizeOfSocketDynType)socketIdx) != E_OK)
    {
      /* Tls Client is unavailable, Report connection failure */
      TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TCP_CONNECTED, TCPIP_E_TLS_ERROR);
    }
  }
  else
#  endif /* (TCPIP_SUPPORT_ASRTLSCLIENT == STD_ON) */
# endif /* (TCPIP_SUPPORT_ASRTLS == STD_ON) */
  {
    /* The socketOwnerConfigIdx can be assumed to be valid since it was already checked when calling TcpConnect. */
    TcpIp_SocketIdType socketId = TCPIP_SOCKET_IDX_TO_ID(socketIdx);
    TcpIp_SocketOwnerTcpConnectedType tcpConnectedFuncPtr = TcpIp_GetTcpConnectedFuncPtrOfSocketOwnerConfig(socketOwnerConfigIdx);

    /* #10 Inform the socket user about the connection of the socket. */
    if (tcpConnectedFuncPtr != NULL_PTR)
    {
      tcpConnectedFuncPtr(socketId);
    }
    else
    {
      /* #20 Otherwise issue a DET error. */
      TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TCP_CONNECTED, TCPIP_E_CBK_REQUIRED);
    }
  }
}
#endif

/**********************************************************************************************************************
 *  TcpIp_TcpIpEventFunctions
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_TcpIpEventFunctions(
  TcpIp_SocketOwnerConfigIterType SocketOwnerConfigIdx,
  TcpIp_SocketDynIterType         SocketIdx,
  IpBase_TcpIpEventType           EventType)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SocketOwnerTcpIpEventType tcpIpEventFuncPtr = TcpIp_GetTcpIpEventFuncPtrOfSocketOwnerConfig(SocketOwnerConfigIdx);

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketOwnerConfigIdx < TcpIp_GetSizeOfSocketOwnerConfig());
  TCPIP_ASSERT(SocketIdx         < TcpIp_GetSizeOfSocketDyn());

  /* #10 In case a callout is configured for that user, inform the socket user about the TCP event related to that socket. */
  if (tcpIpEventFuncPtr != NULL_PTR)
  {
    tcpIpEventFuncPtr(TCPIP_SOCKET_IDX_TO_ID(SocketIdx), EventType);
  }
  else
  {
    /* #20 Otherwise issue a DET error. */
    /* ERROR: Callback is mandatory. */
    TcpIp_CallDetReportError(TCPIP_API_ID_CBK_TCP_IP_EVENT, TCPIP_E_CBK_REQUIRED);
  }
}

/**********************************************************************************************************************
 *  TcpIp_CopyTxDataFunctions
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
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(BufReq_ReturnType, TCPIP_CODE) TcpIp_CopyTxDataFunctions(
  TcpIp_SizeOfSocketDynType SocketIdx,
  TCPIP_P2V(uint8)          BufferPtr,
  TCPIP_P2V(uint16)         BufferLenBytePtr,
  boolean                   UseTlsCopyTxData)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType                 retVal            = BUFREQ_E_NOT_OK;
  TcpIp_SizeOfSocketOwnerConfigType socketOwnerConfigIdx = TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(SocketIdx);

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketIdx         < TcpIp_GetSizeOfSocketDyn());
  TCPIP_ASSERT(BufferPtr         != NULL_PTR);
  TCPIP_ASSERT(BufferLenBytePtr  != NULL_PTR);

  {
#if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                    /* COV_MSR_UNSUPPORTED */
    TcpIp_SizeOfSocketTcpDynType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx);
    /* #10 Check if the function is called by the Tls record layer */
    if (   (UseTlsCopyTxData == TRUE)
        && TCPIP_TCP_COND_VALID_TCPIDX(socketTcpIdx)                                                                    /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
        && (TcpIp_IsUseTlsOfSocketTcpDyn(socketTcpIdx) == TRUE))
    {
      /* #20 if so, forward the call to the Tls record layer. */
      retVal = TcpIp_TlsCoreRl_EncryptAndAuthFrame(SocketIdx, BufferPtr, *BufferLenBytePtr);
    }
    else
      /* #30 otherwise, the call comes from the socket owner, so call the socketOwner configured CopyTxDataFct. */
#else
    TCPIP_DUMMY_STATEMENT_CONST(UseTlsCopyTxData);                                                                      /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif
    {
      if(socketOwnerConfigIdx < TcpIp_GetSizeOfSocketOwnerConfig())
      {
        TcpIp_SocketIdType socketId = TCPIP_SOCKET_IDX_TO_ID(SocketIdx);
        TcpIp_SocketOwnerCopyTxDataDynType copyTxDataDynFuncPtr = TcpIp_GetCopyTxDataDynFuncPtrOfSocketOwnerConfig(socketOwnerConfigIdx);
        TcpIp_SocketOwnerCopyTxDataType copyTxDataFuncPtr = TcpIp_GetCopyTxDataFuncPtrOfSocketOwnerConfig(socketOwnerConfigIdx);

        /* #40 Call extended CopyTxData callback of socket owner, if configured. */
        if(copyTxDataDynFuncPtr != NULL_PTR)
        {
          retVal = copyTxDataDynFuncPtr(socketId, BufferPtr, BufferLenBytePtr);                                         /* VCA_TCPIP_FC_COPYDATA_FROM_TCP_SOCKETOWNER */
        }
        /* #50 Otherwise: Call AUTOSAR CopyTxData callback of socket owner, if configured. */
        else if(copyTxDataFuncPtr != NULL_PTR)
        {
          retVal = copyTxDataFuncPtr(socketId, BufferPtr, *BufferLenBytePtr);                                           /* VCA_TCPIP_FC_COPYDATA_FROM_TCP_SOCKETOWNER */
        }
        /* #60 Otherwise: Report an error to the Det. */
        else
        {
          /* ERROR: Callback is mandatory. */
          TcpIp_CallDetReportError(TCPIP_API_ID_CBK_COPY_TX_DATA, TCPIP_E_CBK_REQUIRED);
        }
      }
      else
      {
        TCPIP_ASSERT_UNREACHABLE();
      }
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  TcpIp_LocalIpAssignmentChgFunctions
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_LocalIpAssignmentChgFunctions(
  TcpIp_LocalAddrIterType LocalAddrIdx,
  TcpIp_IpAddrStateType   AddrState)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SocketOwnerConfigIterType socketOwnerConfigIdx;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(LocalAddrIdx < TcpIp_GetSizeOfLocalAddr());
  /* #10 Notify all socket owners about changed IP address assignment, if callbacks are configured. */
  for (socketOwnerConfigIdx = 0; socketOwnerConfigIdx < TcpIp_GetSizeOfSocketOwnerConfig(); socketOwnerConfigIdx++)
  {
    TcpIp_SocketOwnerLocalIpAddrAssignmentChgType callbackFuncPtr = TcpIp_GetLocalIpAddrAssignmentChgFuncPtrOfSocketOwnerConfig(socketOwnerConfigIdx);

    if (callbackFuncPtr != NULL_PTR)
    {
      callbackFuncPtr(TCPIP_LOCAL_ADDR_IDX_TO_ID(LocalAddrIdx), AddrState);
    }
  }
}

/**********************************************************************************************************************
 *  TcpIp_VIpTransmit
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_VIpTransmit(
  TCPIP_P2C(TcpIp_IpTxRequestDescriptorType) IpTxReqDescPtr,
  uint16                                     IpPayloadLen,
  uint8                                      IpProtocol,
  uint8                                      UlTxReqIdx,
  uint8                                      Flags)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  boolean          txConfirmation      = (boolean)((Flags & TCPIP_V_IP_TRANSMIT_FLAG_TX_CONFIRMATION) != 0u);           /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  boolean          retryOnPhysAddrMiss = (boolean)((Flags & TCPIP_V_IP_TRANSMIT_FLAG_RETRY_ON_PHYS_ADDR_MISS) != 0u);   /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */

  TCPIP_ASSERT(TCPIP_IS_ADDR_FAMILY_SUPPORTED(IpTxReqDescPtr->AddressFamily));

  /* #10 Determine IP version (address family) of IP TX request descriptior and forward call to IPv4/IPv6 submodule. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  if (IpTxReqDescPtr->AddressFamily == IPBASE_AF_INET)
# endif
  {
    retVal = IpV4_Ip_Transmit(IpTxReqDescPtr, IpPayloadLen, IpProtocol, txConfirmation,
               UlTxReqIdx, retryOnPhysAddrMiss);
  }
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  else
# endif
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
  {
    retVal = IpV6_Transmit(TCPIP_SOCKET_TO_IPV6_SOCKET(IpTxReqDescPtr->SockIdx), IpTxReqDescPtr, IpProtocol, IpPayloadLen, txConfirmation,
               UlTxReqIdx, retryOnPhysAddrMiss);
  }
#endif

  return retVal;
} /* TcpIp_VIpTransmit() */

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VSockTcpStateChg
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
 /* PRQA S 6030, 6050 1 */ /* MD_MSR_STCYC, MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_VSockTcpStateChg(
  TcpIp_SocketDynIterType         SocketIdx,
  TcpIp_SocketOwnerConfigIterType FormerSocketOwnerCfgIdx,
  TcpIp_StateType                 State,
  boolean                         SocketUsesTls)
{
  /* ----- Local Variables ---------------------------------------------- */
# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
  TcpIp_TlsConnectionIterType tlsConIdx = TcpIp_GetSizeOfTlsConnection();
# else
  /* Parameter 'SocketUsesTls' is only used if TLS is enabled. */
  TCPIP_DUMMY_STATEMENT_CONST(SocketUsesTls);                                                                           /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
# endif
  /*@ assert TcpIp_ConfigDataPtr == &TcpIp_PCConfig.Config; */                                                          /* VCA_TCPIP_CONFIG_PTR_RESTORE */

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketIdx < TcpIp_GetSizeOfSocketDyn());
  TCPIP_ASSERT(TCPIP_SOCKET_IDX_IS_TCP(SocketIdx));

  /* #10 Set the state of the given Socket depending on the occurred event and forward the event */
  switch(State)
  {
  case TCPIP_TCP_STATE_CONN_ESTAB:
    TcpIp_SetListenActiveConnStatOfSocketDyn(SocketIdx, TCPIP_SOCK_CONN_ACTIVE);
    break;

  case TCPIP_TCP_STATE_CONNECTED:
    {
      TcpIp_SocketTcpDynIterType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx);
      /*@ assert socketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn(); */                                                       /* VCA_TCPIP_TCP_IDX_FROM_CHECKED_SOCKET_IDX */
      TcpIp_SetListenActiveConnStatOfSocketDyn(SocketIdx, TCPIP_SOCK_CONN_ACTIVE);
      TcpIp_TcpConnectedFunctions(socketTcpIdx);
      break;
    }

  case TCPIP_TCP_STATE_CLOSED:
    TcpIp_SetListenActiveConnStatOfSocketDyn(SocketIdx, TCPIP_SOCK_CONN_NONE);
# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
    /* Check if the socket uses TLS and a corresponding TLS connection is assigned */
    if (   (SocketUsesTls == TRUE)                                                                                      /* PRQA S 3415 3 */ /* MD_TCPIP_Rule13.5_3415_ReadOnly */
        && (TcpIp_SocketIdxToTlsConIdx(SocketIdx, &tlsConIdx) == E_OK))
    {
      /* Let TLS handle the CLOSE event for socket which uses TLS */
      TcpIp_TlsCoreEvent_TcpIpEventForwarding(SocketIdx, IPBASE_TCP_EVENT_CLOSED);
    }
    else
# endif
    {
      /* Socket does not use TLS or Socket is used by TLS-Listener */
      TcpIp_TcpIpEventFunctions(FormerSocketOwnerCfgIdx, SocketIdx, IPBASE_TCP_EVENT_CLOSED);
    }
    break;

  case TCPIP_TCP_STATE_RESET:
    TcpIp_SetListenActiveConnStatOfSocketDyn(SocketIdx, TCPIP_SOCK_CONN_NONE);
# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
    /* TLS only wants the information if the TCP connection is established. */
    /* TLS state is set in Ll_TcpConnected (after TCP handshake) to HS_ACTIVE */

    /* Check if the socket uses TLS and the TLS connections is in state active */
    if (   (SocketUsesTls == TRUE)                                                                                      /* PRQA S 3415 3 */ /* MD_TCPIP_Rule13.5_3415_ReadOnly */
        && (TcpIp_SocketIdxToTlsConIdx(SocketIdx, &tlsConIdx) == E_OK)
        && (tlsConIdx < TcpIp_GetSizeOfTlsConnection()))
    {
      /* Check if the TLS connection is already allocated to any socket */
      if (TcpIp_GetConnectionStateOfTlsConnectionDyn(tlsConIdx) == TCPIP_TLS_STATE_CONNECTION_ALLOCATED)
      {
        /* TLS connection is allocated, but not used yet. Reset the connection and let TCP handle the event */
        TcpIp_Tls_FreeAllocatedConnection(tlsConIdx);
      }

      if(TcpIp_GetConnectionStateOfTlsConnectionDyn(tlsConIdx) != TCPIP_TLS_STATE_CONNECTION_HS_INACTIVE)
      {
        /* Let the TLS process the event only if the assigned TLS connections is active */
        TcpIp_TlsCoreEvent_TcpIpEventForwarding(SocketIdx, IPBASE_TCP_EVENT_RESET);
      }
      else
      {
        /* TLS is not active yet, let TcpIp handle the event */
        TcpIp_TcpIpEventFunctions(FormerSocketOwnerCfgIdx, SocketIdx, IPBASE_TCP_EVENT_RESET);
      }
    }
    else
      /* TLS connection is not used on this socket or socket is used by TLS-Listener - let TcpIp handle the event */
# endif
    {
      TcpIp_TcpIpEventFunctions(FormerSocketOwnerCfgIdx, SocketIdx, IPBASE_TCP_EVENT_RESET);
    }
    break;

  case TCPIP_TCP_STATE_CONN_REQ_REC:
    /* always accept the incoming connection, do not forward callback to upper layers */
    break;

  case TCPIP_TCP_STATE_FIN_REC:
    /* remote side starts to close the connection */
# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
    if (SocketUsesTls == TRUE)
    {
      TcpIp_TlsCoreEvent_TcpIpEventForwarding(SocketIdx, IPBASE_TCP_EVENT_FIN_RECEIVED);
    }
    else
# endif
    {
      TcpIp_TcpIpEventFunctions(FormerSocketOwnerCfgIdx, SocketIdx, IPBASE_TCP_EVENT_FIN_RECEIVED);
    }
    break;
  case TCPIP_TCP_STATE_CONN_CLOSING:
    /* No action to be done */
    break;

  default:
    /* should not happen - all other states are invalid */
    break;
  } /* switch */
}
#endif /* (TCPIP_SUPPORT_TCP == STD_ON) */

/**********************************************************************************************************************
 *  TcpIp_VSockTxConfirmation
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
FUNC(void, TCPIP_CODE) TcpIp_VSockTxConfirmation(
  TcpIp_SocketDynIterType SocketIdx,
  uint32                  DataLenByte)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SocketIdx      < TcpIp_GetSizeOfSocketDyn());


  /* #10 Update/decrease the internal buffer fill level according to the number of bytes confirmed. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  if(TCPIP_SOCKET_IDX_IS_TCP(SocketIdx))
  {
    TcpIp_SocketTcpDynIterType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx);
    TcpIp_TcpTxBufferDescIterType tcpTxBufferDescIdx = TcpIp_GetTcpTxBufferDescIdxOfSocketTcpDyn(socketTcpIdx);
    /*@ assert tcpTxBufferDescIdx < TcpIp_GetSizeOfTcpTxBufferDesc(); */                                                /* VCA_TCPIP_TCPTXBUFFERDESCIDX */

    TCPIP_VENTER_CRITICAL_SECTION();

    TCPIP_ASSERT(tcpTxBufferDescIdx < TcpIp_GetSizeOfTcpTxBufferDesc());

    if(DataLenByte > 0u)
    {
      TCPIP_ASSERT(DataLenByte <= TcpIp_GetFillLevelOfTcpTxBufferDescDyn(tcpTxBufferDescIdx));

      TcpIp_SubFillLevelOfTcpTxBufferDescDyn(tcpTxBufferDescIdx, (TcpIp_SizeOfTcpTxBufferType)DataLenByte);
    }
    else
    {
      /* negative TxConfirmation */
      TcpIp_SetFillLevelOfTcpTxBufferDescDyn(tcpTxBufferDescIdx, 0);
    }

    TCPIP_VLEAVE_CRITICAL_SECTION();
  }
#endif

  {
#if (TCPIP_SUPPORT_ASRTLS == STD_ON )                                                                                   /* COV_MSR_UNSUPPORTED */
    /* #20 Check if the given socket is used by TLS */
    if((TCPIP_SOCKET_IDX_IS_TCP(SocketIdx) == TRUE)                                                                     /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
      && (TcpIp_IsUseTlsOfSocketTcpDyn(TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx)) == TRUE))                                /* TCPIP_CSL_INDEX_ACCEPTED */
    {
      /* #30 If the socket is used by TLS, skip the forwarding of the TxConfirmation. This will be done by the TLS stack. */


    }
    else
#endif
    {
      /* #40 Socket is not used by TLS, proceed with the TxConfirmation forwarding to the socket owner */
      /* #50 Confirm the overall length in chunks of at maximum 0xffff bytes. */
      if(DataLenByte <= TCPIP_TX_CONF_LEN_MAX)
      {
        /* forward confirmation to UL: */
        TcpIp_TxConfirmationFunctions(SocketIdx, (uint16)DataLenByte);
      }
      else
      {
        uint16 txConfLen;
        uint32 remainingTxConfLen = DataLenByte;
        while(remainingTxConfLen > 0u)
        {
          /* forward confirmation to UL: */
          if(remainingTxConfLen > TCPIP_TX_CONF_LEN_MAX)
          {
            txConfLen = (uint16)TCPIP_TX_CONF_LEN_MAX;
          }
          else
          {
            txConfLen = (uint16)remainingTxConfLen;
          }
          TcpIp_TxConfirmationFunctions(SocketIdx, txConfLen);
          remainingTxConfLen -= txConfLen;
        }
      }
    }
  }
} /* TcpIp_VSockTxConfirmation() */

/**********************************************************************************************************************
 *  TcpIp_Cbk_VLocalIpAssignmentChg
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VLocalIpAssignmentChg(
  TcpIp_LocalAddrIterType LocalAddrIdx,
  TcpIp_IpAddrStateType   State)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(LocalAddrIdx < TcpIp_GetSizeOfLocalAddr());

  /* #10 Store the new state of the given local address index. */
  TcpIp_SetAssignmentStateOfLocalAddr(LocalAddrIdx, State);

  /* #20 Reset the affected IP controller in case the new state is 'unassigned'. */
  if(TCPIP_IPADDR_STATE_UNASSIGNED == State)
  {
    TcpIp_VRstCtrl(LocalAddrIdx);
  }

  /* #30 Forward notification to upper layer via configured callbacks. */
  TcpIp_LocalIpAssignmentChgFunctions(LocalAddrIdx, State); /*lint !e522 */ /* Function may have no side-effects depending on configuration. */
} /* TcpIp_Cbk_VLocalIpAssignmentChg() */

/**********************************************************************************************************************
 *  TcpIp_Cbk_VPathMtuChg
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VPathMtuChg(
  uint8                             CtrlIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) SockAddrPtr,
  uint16                            PathMtuSize)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(SockAddrPtr != NULL_PTR);
  TCPIP_ASSERT(CtrlIdx <= TcpIp_GetSizeOfEthIfCtrl());                                                                  /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */

  TCPIP_DUMMY_STATEMENT_CONST(CtrlIdx);                                                                                 /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */

#if (TCPIP_SUPPORT_TCP == STD_ON)
  /* #10 Forward path-MTU callback to TCP submodule in case it is configured. */
  TcpIp_Tcp_CbkPathMtuChg(SockAddrPtr, PathMtuSize);
#else
  TCPIP_DUMMY_STATEMENT_CONST(SockAddrPtr);                                                                             /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
  TCPIP_DUMMY_STATEMENT_CONST(PathMtuSize);                                                                             /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif

} /* TcpIp_Cbk_VPathMtuChg() */

/**********************************************************************************************************************
 *  TcpIp_Cbk_VAddrResTimeout
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_Cbk_VAddrResTimeout(
  TCPIP_P2C(TcpIp_SockAddrBaseType) DestAddrPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(DestAddrPtr != NULL_PTR);

  /* #10 Forward the timeout event to the TCP subcomponent in case it is configured. */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  /* check all sockets in TCP */
  TcpIp_Tcp_VAddrResTimeout(DestAddrPtr);
#else
  TCPIP_DUMMY_STATEMENT_CONST(DestAddrPtr);                                                                             /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif

  /* Forwarding the timeout event to the UDP subcomponent is not required because UDP is 'fire and forget'. */
}

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VSockTcpStateChgAccepted
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VSockTcpStateChgAccepted(
  TcpIp_SocketTcpDynIterType SocketTcpIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType                  retVal             = E_NOT_OK;
  TcpIp_SocketDynIterType         socketIdx          = TCPIP_TCP_IDX_TO_SOCKET_IDX(SocketTcpIdx);
  TcpIp_SocketOwnerConfigIterType socketOwnerCfgIdx  = TcpIp_GetSocketOwnerConfigIdxOfSocketDyn(socketIdx);

  /* ----- Implementation ----------------------------------------------- */
  if (socketOwnerCfgIdx < TcpIp_GetSizeOfSocketOwnerConfig())
  {
    /* #10 Accept the new TCP connection on the derived socket identified by SocketIdx. */
    /* 'SocketIdx' is the new derived socket that is used for the TCP connection (not the listen socket!) */
    TcpIp_SocketTcpDynIterType listenSocketTcpIdx = TcpIp_GetSocketTcpDynMasterListenSocketIdxOfSocketTcpDyn(SocketTcpIdx);
    /*@ assert listenSocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn(); */                                                   /* VCA_TCPIP_LISTEN_SOCKET_TCP_IDX_FROM_SOCKETTCPIDX */

    TcpIp_SetListenActiveConnStatOfSocketDyn(socketIdx, TCPIP_SOCK_CONN_ACTIVE);

    /* #20 Inform the upper layers about the new, accepted, TCP connection. */
    TCPIP_ASSERT(listenSocketTcpIdx < TcpIp_GetSizeOfSocketTcpDyn());
    retVal = TcpIp_TcpAcceptedFunctions(socketOwnerCfgIdx, listenSocketTcpIdx, SocketTcpIdx,
                                        &TcpIp_GetAddrRemSockOfSocketDyn(socketIdx)->TcpIpSockAddr);
  }
  else
  {
    TCPIP_ASSERT_UNREACHABLE();
  }

  return retVal;
} /* TcpIp_VSockTcpStateChgAccepted() */
#endif
/* (STD_ON == TCPIP_SUPPORT_TCP) */

/**********************************************************************************************************************
 *  TcpIp_VIpTransmitCancel
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VIpTransmitCancel(
    TCPIP_P2C(TcpIp_IpTxRequestDescriptorType) IpTxReqDescrPtr,
    uint8                                      UlTxReqTabIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  /* TxConfirmation is always set to 'FALSE' */
  TCPIP_ASSERT(TCPIP_IS_ADDR_FAMILY_SUPPORTED(IpTxReqDescrPtr->AddressFamily));

  /* #10 Forward call to particular IpV4 or IpV6 subcomponent, based on the domain. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  if (IpTxReqDescrPtr->AddressFamily == IPBASE_AF_INET)
# endif
  {
    /* Forward call to IPv4 */
    IpV4_Ip_CancelTransmit(IpTxReqDescrPtr, UlTxReqTabIdx);
  }
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  else
# endif
#endif
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
  {
    TCPIP_DUMMY_STATEMENT_CONST(UlTxReqTabIdx);                                                                         /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */

    IpV6_CancelTransmit(IpTxReqDescrPtr);
  }
#endif
} /* TcpIp_VIpTransmitCancel() */

/**********************************************************************************************************************
 *  TcpIp_VRstCtrl
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_VRstCtrl(TcpIp_LocalAddrIterType LocalAddrIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SocketDynIterType socketIdx;
#if (TCPIP_SUPPORT_TCP == STD_ON)
  TcpIp_SockAddrBaseType sockAddr;                                                                                      /* PRQA S 0759 */ /* MD_MSR_Union */
#endif

  /* ----- Implementation ----------------------------------------------- */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  /* PRQA S 0314 1 */ /* MD_MSR_VStdLibCopy */
  IpBase_Fill((TCPIP_P2V(void))&sockAddr, 0u, sizeof(TcpIp_SockAddrBaseType));

  (void)TcpIp_GetIpAddr(TCPIP_LOCAL_ADDR_IDX_TO_ID(LocalAddrIdx), &sockAddr.TcpIpSockAddr, NULL_PTR, NULL_PTR);
#endif
  /* #10 Check all sockets that are bound to the given local address id whether they are in state 'listen' or 'connected'. */
  for (socketIdx = 0; socketIdx < TcpIp_GetSizeOfSocketDyn(); socketIdx++)
  {
    boolean initSocket = FALSE;
    /* Process if the socket is bound to the LocalAddrId. */
    if (TcpIp_GetLocalAddrBindIdxOfSocketDyn(socketIdx) == LocalAddrIdx)
    {
      /* #20 In case the socket is in the desired state, reset the socket, according to its associated protocol, TCP or UDP. */
      switch (TcpIp_GetListenActiveConnStatOfSocketDyn(socketIdx))
      {
      case TCPIP_SOCK_CONN_LISTEN_ENABLED:
#if (TCPIP_SUPPORT_TCP == STD_ON)
        if (TCPIP_SOCKET_IDX_IS_TCP(socketIdx))
        {
          /* Reset the TCP socket */
          TcpIp_VRstCtrl_TcpListen(socketIdx);
        }
        else
#endif
        {
          TcpIp_Udp_ResetSocket(socketIdx);
        }

        initSocket = TRUE;
      break;

#if (TCPIP_SUPPORT_TCP == STD_ON)
      case TCPIP_SOCK_CONN_ACTIVE:
        if (TcpIp_VSockIpAddrIsEqual(TcpIp_GetAddrLocSockOfSocketDyn(socketIdx), &sockAddr) == TRUE)
        {
          TcpIp_SizeOfSocketTcpDynType tcpSocketIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(socketIdx);                           /* PRQA S 2896 */ /* MD_TCPIP_2896 */
          /*@ assert tcpSocketIdx < TcpIp_GetSizeOfSocketTcpDyn(); */                                                   /* VCA_TCPIP_TCP_IDX_FROM_CHECKED_SOCKET_IDX */

          /* TCP Socket. */
          TcpIp_Tcp_ResetSocket(tcpSocketIdx);
          initSocket = TRUE;
        }
        break;
#endif

      default:
        /* Socket not bound or established. -> Nothing to do. */
        break;
      }
      /* #30 In case the socket was reset, initialize its admin data. */
      if (initSocket == TRUE)
      {
        TcpIp_VInitSocket(socketIdx);
      }
    }
  }
} /* TcpIp_VRstCtrl() */


/**********************************************************************************************************************
 *  TcpIp_VRstCtrl_TcpListen
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
#if (TCPIP_SUPPORT_TCP == STD_ON)
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_VRstCtrl_TcpListen(
  TcpIp_SocketDynIterType SocketIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SocketTcpDynIterType socketTcpIdx = TCPIP_SOCKET_IDX_TO_TCP_IDX(SocketIdx);

  /* ----- Implementation ----------------------------------------------- */
# if (TCPIP_SUPPORT_ASRTLS == STD_ON)                                                                                   /* COV_MSR_UNSUPPORTED */
  TcpIp_TlsConnectionIterType tlsConIdx = TcpIp_GetSizeOfTlsConnection();

  /* #10 Reset the listen socket, depending on the TLS usage. */

  /* Check if the socket uses TLS and the TLS connections is in state active */
  if (   (TcpIp_IsUseTlsOfSocketTcpDyn(socketTcpIdx) == TRUE)                                                           /* PRQA S 3415 3 */ /* MD_TCPIP_Rule13.5_3415_ReadOnly */
      && (TcpIp_SocketIdxToTlsConIdx(SocketIdx, &tlsConIdx) == E_OK)
      && (TcpIp_GetConnectionStateOfTlsConnectionDyn(tlsConIdx) != TCPIP_TLS_STATE_CONNECTION_HS_INACTIVE)
      && (TcpIp_GetConnectionStateOfTlsConnectionDyn(tlsConIdx) != TCPIP_TLS_STATE_CONNECTION_ALLOCATED))
  {
    /* TLS connection active - Let the TLS process the close. */
    TcpIp_TlsCoreEvent_TcpIpEventClose(SocketIdx, TRUE);
  }
#  if (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON)
  /* if-section copied from TcpIp_Close() */
  else if (   (TcpIp_IsUseTlsOfSocketTcpDyn(socketTcpIdx) == TRUE)                                                      /* PRQA S 3415 2 */ /* MD_TCPIP_Rule13.5_3415_ReadOnly */
           && (TcpIp_GetSockStateOfSocketTcpDyn(socketTcpIdx) == TCPIP_TCP_SOCK_STATE_LISTEN))
  {
    /* Reset the TCP socket and the TLS listener */
    TcpIp_TlsServerListener_Close(SocketIdx, TRUE);
  }
  else
#  endif /* (TCPIP_SUPPORT_ASRTLSSERVER == STD_ON) */
# endif /* (TCPIP_SUPPORT_ASRTLS == STD_ON) */
  {
    /* Reset the TCP socket */
    TcpIp_Tcp_ResetSocket(socketTcpIdx);
  }
}
#endif /* (TCPIP_SUPPORT_TCP == STD_ON) */


/**********************************************************************************************************************
 *  TcpIp_DListRawAddAfter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawAddAfter(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType              NewIdx,
  TcpIp_DListIdxType              AfterIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(NodeSetPtr  != NULL_PTR);
  TCPIP_ASSERT(FirstIdxPtr != NULL_PTR);
  TCPIP_ASSERT(LastIdxPtr  != NULL_PTR);

  NodeSetPtr[NewIdx].PrevIdx = AfterIdx;                                                                                /* VCA_TCPIP_DList_NodeArrayIdx */

  /* #10 Insert the new node at the desired position, in case the index to insert the new node after is not the last element. */
  if (TCPIP_DLIST_END_IDX != AfterIdx)
  {
    /* insert after existing node */
    NodeSetPtr[NewIdx].NextIdx = NodeSetPtr[AfterIdx].NextIdx;                                                          /* VCA_TCPIP_DList_NodeArrayIdx */
    NodeSetPtr[AfterIdx].NextIdx = NewIdx;                                                                              /* VCA_TCPIP_DList_NodeArrayOrEndIdx */
  }
  /* #20 Otherwise insert the new node as the first element. */
  else
  {
    NodeSetPtr[NewIdx].NextIdx = (*FirstIdxPtr);                                                                        /* VCA_TCPIP_DList_NodeArrayIdx */
    (*FirstIdxPtr) = NewIdx;                                                                                            /* VCA_TCPIP_NonNullPtrParameter */
  }

  /* #30 Complete the linkage of the new node, depending on the node being the last node or not. */
  if (TCPIP_DLIST_END_IDX != NodeSetPtr[NewIdx].NextIdx)
  {
    /* new node is not last node */
    NodeSetPtr[NodeSetPtr[NewIdx].NextIdx].PrevIdx = NewIdx;                                                            /* VCA_TCPIP_DList_PrevOrNextIdx */
  }
  else
  {
    /* new node is last node */
    (*LastIdxPtr) = NewIdx;                                                                                             /* VCA_TCPIP_NonNullPtrParameter */
  }
  /*@ assert TcpIp_ConfigDataPtr == &TcpIp_PCConfig.Config; */                                                          /* VCA_TCPIP_CONFIG_PTR_RESTORE */ /* TAR-18036 */
} /* TcpIp_DListRawAddAfter() */

/**********************************************************************************************************************
 *  TcpIp_DListRawAddRangeAfter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
/* PRQA S 6060 1 */ /* MD_MSR_STPAR */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawAddRangeAfter(
  TCPIP_P2V(TcpIp_DListNodeType) NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)  FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)  LastIdxPtr,
  TcpIp_DListIdxType             NewStartIdx,
  TcpIp_DListIdxType             NewEndIdx,
  TcpIp_DListIdxType             AfterIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(NodeSetPtr  != NULL_PTR);
  TCPIP_ASSERT(FirstIdxPtr != NULL_PTR);
  TCPIP_ASSERT(LastIdxPtr  != NULL_PTR);

  NodeSetPtr[NewStartIdx].PrevIdx = AfterIdx;                                                                           /* VCA_TCPIP_DList_NodeArrayIdx */

  /* #10 Insert the new node-range at the desired position, in case the index to insert the new node-range after is not the last element. */
  if (TCPIP_DLIST_END_IDX != AfterIdx)
  {
    /* insert after existing node */
    NodeSetPtr[NewEndIdx].NextIdx = NodeSetPtr[AfterIdx].NextIdx;                                                       /* VCA_TCPIP_DList_NodeArrayIdx */
    NodeSetPtr[AfterIdx].NextIdx  = NewStartIdx;                                                                        /* VCA_TCPIP_DList_NodeArrayOrEndIdx */
  }
  /* #20 Otherwise insert the new node-range at the beginning. */
  else
  {
    /* insert as first node */
    NodeSetPtr[NewEndIdx].NextIdx = (*FirstIdxPtr);                                                                     /* VCA_TCPIP_DList_NodeArrayIdx */
    (*FirstIdxPtr) = NewStartIdx;                                                                                       /* VCA_TCPIP_NonNullPtrParameter */
  }

  /* #30 Complete the linkage of the new node-range, depending on the node-range being at the end or not. */
  if (TCPIP_DLIST_END_IDX != NodeSetPtr[NewEndIdx].NextIdx)
  {
    /* new node is not last node */
    NodeSetPtr[NodeSetPtr[NewEndIdx].NextIdx].PrevIdx = NewEndIdx;                                                      /* VCA_TCPIP_DList_PrevOrNextIdx */
  }
  else
  {
    /* new node is last node */
    (*LastIdxPtr) = NewEndIdx;                                                                                          /* VCA_TCPIP_NonNullPtrParameter */
  }
  /*@ assert TcpIp_ConfigDataPtr == &TcpIp_PCConfig.Config; */                                                          /* VCA_TCPIP_CONFIG_PTR_RESTORE */ /* TAR-18036 */
} /* TcpIp_DListRawAddRangeAfter() */

/**********************************************************************************************************************
 *  TcpIp_DListRawRemove()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_DListRawRemove(
  TCPIP_P2V(TcpIp_DListNodeType)  NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)   LastIdxPtr,
  TcpIp_DListIdxType              RemIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(NodeSetPtr     != NULL_PTR);
  TCPIP_ASSERT(FirstIdxPtr    != NULL_PTR);
  TCPIP_ASSERT(LastIdxPtr     != NULL_PTR);
  TCPIP_ASSERT((*FirstIdxPtr) != TCPIP_DLIST_END_IDX);
  TCPIP_ASSERT((*LastIdxPtr)  != TCPIP_DLIST_END_IDX);
  TCPIP_ASSERT(RemIdx         != TCPIP_DLIST_END_IDX);

  /* #10 Remove the node to be removed from the its position, in case the index to remove the node is not the last element. */
  if (TCPIP_DLIST_END_IDX != NodeSetPtr[RemIdx].PrevIdx)
  {
    /* node is not first node */
    NodeSetPtr[NodeSetPtr[RemIdx].PrevIdx].NextIdx = NodeSetPtr[RemIdx].NextIdx;                                        /* VCA_TCPIP_DList_PrevOrNextIdx */
  }
  /* #20 Otherwise remove the desired node at the beginning. */
  else
  {
    /* remove first node */
    (*FirstIdxPtr) = NodeSetPtr[RemIdx].NextIdx;                                                                        /* VCA_TCPIP_NonNullPtrParameter */
  }

  /* #30 Re-link the remaining nodes, depending on whether the deleted node was placed at the end or not. */
  if (TCPIP_DLIST_END_IDX != NodeSetPtr[RemIdx].NextIdx)
  {
    /* node is not last node */
    NodeSetPtr[NodeSetPtr[RemIdx].NextIdx].PrevIdx = NodeSetPtr[RemIdx].PrevIdx;                                        /* VCA_TCPIP_DList_PrevOrNextIdx */
  }
  else
  {
    /* remove last node */
    (*LastIdxPtr) = NodeSetPtr[RemIdx].PrevIdx;                                                                         /* VCA_TCPIP_NonNullPtrParameter */
  }

  NodeSetPtr[RemIdx].PrevIdx = TCPIP_DLIST_END_IDX;                                                                     /* VCA_TCPIP_DList_NodeArrayIdx */
  NodeSetPtr[RemIdx].NextIdx = TCPIP_DLIST_END_IDX;                                                                     /* VCA_TCPIP_DList_NodeArrayIdx */
  /*@ assert TcpIp_ConfigDataPtr == &TcpIp_PCConfig.Config; */                                                          /* VCA_TCPIP_CONFIG_PTR_RESTORE */ /* TAR-18036 */
} /* TcpIp_DListRawRemove() */

/**********************************************************************************************************************
 *  TcpIp_DListRawRemoveRange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
STATIC FUNC(uint8, TCPIP_CODE) TcpIp_DListRawRemoveRange(
  TCPIP_P2V(TcpIp_DListNodeType) NodeSetPtr,
  TCPIP_P2V(TcpIp_DListIdxType)  FirstIdxPtr,
  TCPIP_P2V(TcpIp_DListIdxType)  LastIdxPtr,
  TcpIp_DListIdxType             RemStartIdx,
  TcpIp_DListIdxType             RemEndIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* count number of elements in chain */
  TcpIp_DListIdxType entryIdx    = RemStartIdx;
  uint8              chainLength = 1;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(NodeSetPtr  != NULL_PTR);
  TCPIP_ASSERT(FirstIdxPtr != NULL_PTR);
  TCPIP_ASSERT(LastIdxPtr  != NULL_PTR);

  while (RemEndIdx != entryIdx)
  {
    chainLength++;
    TCPIP_ASSERT_MSG(chainLength > 0, "DList chain length overflow detected!");
    entryIdx = NodeSetPtr[entryIdx].NextIdx;
    TCPIP_ASSERT(!(entryIdx == TCPIP_DLIST_END_IDX) && (RemEndIdx != TCPIP_DLIST_END_IDX));
  }

  /* #10 Remove the node-range to be removed from the its position, in case the starting index of the node-range to remove is not the last element. */
  if (TCPIP_DLIST_END_IDX != NodeSetPtr[RemStartIdx].PrevIdx)
  {
    /* node is not first node */
    NodeSetPtr[NodeSetPtr[RemStartIdx].PrevIdx].NextIdx = NodeSetPtr[RemEndIdx].NextIdx;                                /* VCA_TCPIP_DList_PrevOrNextIdx */
  }
  /* #20 Otherwise remove the desired node at the beginning. */
  else
  {
    /* remove first node */
    (*FirstIdxPtr) = NodeSetPtr[RemEndIdx].NextIdx;                                                                     /* VCA_TCPIP_NonNullPtrParameter */
  }

  /* #30 Re-link the remaining nodes, depending on whether the deleted node was placed at the end or not. */
  if (TCPIP_DLIST_END_IDX != NodeSetPtr[RemEndIdx].NextIdx)
  {
    /* node is not last node */
    NodeSetPtr[NodeSetPtr[RemEndIdx].NextIdx].PrevIdx = NodeSetPtr[RemStartIdx].PrevIdx;                                /* VCA_TCPIP_DList_PrevOrNextIdx */
  }
  else
  {
    /* remove last node */
    (*LastIdxPtr) = NodeSetPtr[RemStartIdx].PrevIdx;                                                                    /* VCA_TCPIP_NonNullPtrParameter */
  }

  NodeSetPtr[RemStartIdx].PrevIdx = TCPIP_DLIST_END_IDX;                                                                /* VCA_TCPIP_DList_NodeArrayIdx */
  NodeSetPtr[RemEndIdx].NextIdx   = TCPIP_DLIST_END_IDX;                                                                /* VCA_TCPIP_DList_NodeArrayIdx */
  /*@ assert TcpIp_ConfigDataPtr == &TcpIp_PCConfig.Config; */                                                          /* VCA_TCPIP_CONFIG_PTR_RESTORE */ /* TAR-18036 */
  return chainLength;
} /* TcpIp_DListRawRemoveRange() */

/**********************************************************************************************************************
 *  TcpIp_DListInitDesc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DListInitDesc(
  TCPIP_P2V(TcpIp_DListDescType) DescPtr,
  TCPIP_P2V(TcpIp_DListNodeType) NodePtr,
  uint8                          NodeCount)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Assign node list referenced by 'NodePtr' to the descriptor. */
  DescPtr->NodePtr = NodePtr;                                                                                           /* VCA_TCPIP_NonNullPtrParameter */

  TCPIP_ASSERT(DescPtr != NULL_PTR);
  TCPIP_ASSERT(((NodeCount > 0) && (NodePtr != NULL_PTR)) || ((NodeCount == 0u) && (NodePtr == NULL_PTR)));

  /* #20 Let 'FirstFreeIdx' and 'LastFreeIdx' of descriptor point to the chain, if it contains at least one element. */
  if (NodeCount > 0u)
  {
    uint8_least dataIdx;

    DescPtr->FirstFreeIdx = 0;                                                                                          /* VCA_TCPIP_NonNullPtrParameter */
    DescPtr->LastFreeIdx  = (TcpIp_DListIdxType)(NodeCount - 1u);                                                       /* VCA_TCPIP_NonNullPtrParameter */

    /* #30 Iterate over the elements and create a doubly-linked chain. */
    for (dataIdx = 0; dataIdx < NodeCount; dataIdx++)
    {
      if (dataIdx > DescPtr->FirstFreeIdx)
      {
        NodePtr[dataIdx].PrevIdx = (TcpIp_DListIdxType)(dataIdx - 1u);                                                  /* VCA_TCPIP_DList_InitDesc */
      }
      else
      {
        NodePtr[dataIdx].PrevIdx = TCPIP_DLIST_END_IDX;                                                                 /* VCA_TCPIP_DList_InitDesc */
      }

      if (dataIdx < DescPtr->LastFreeIdx)
      {
        NodePtr[dataIdx].NextIdx = (TcpIp_DListIdxType)(dataIdx + 1u);                                                  /* VCA_TCPIP_DList_InitDesc */
      }
      else
      {
        NodePtr[dataIdx].NextIdx = TCPIP_DLIST_END_IDX;                                                                 /* VCA_TCPIP_DList_InitDesc */
      }
    }
  }
  /* #40 Otherwise: Mark 'FirstFreeIdx' and 'LastFreeIdx' of descriptor as invalid. */
  else
  {
    DescPtr->FirstFreeIdx = TCPIP_DLIST_END_IDX;                                                                        /* VCA_TCPIP_NonNullPtrParameter */
    DescPtr->LastFreeIdx  = TCPIP_DLIST_END_IDX;                                                                        /* VCA_TCPIP_NonNullPtrParameter */
  }

  DescPtr->ElementFreeCount  = NodeCount;                                                                               /* VCA_TCPIP_NonNullPtrParameter */
  DescPtr->ElementTotalCount = NodeCount;                                                                               /* VCA_TCPIP_NonNullPtrParameter */
  /*@ assert TcpIp_ConfigDataPtr == &TcpIp_PCConfig.Config; */                                                          /* VCA_TCPIP_CONFIG_PTR_RESTORE */ /* TAR-18036 */
} /* TcpIp_DListInitDesc() */

/**********************************************************************************************************************
 *  TcpIp_DListInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DListInit(
  TCPIP_P2V(TcpIp_DListType)     ListPtr,
  TCPIP_P2V(TcpIp_DListDescType) DescPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(ListPtr != NULL_PTR);
  TCPIP_ASSERT(DescPtr != NULL_PTR);
  /* #10 Connect a list instance to a descriptor and mark the list as empty. */
  ListPtr->DescPtr      = DescPtr;                                                                                      /* VCA_TCPIP_NonNullPtrParameter */
  ListPtr->FirstIdx     = TCPIP_DLIST_END_IDX;                                                                          /* VCA_TCPIP_NonNullPtrParameter */
  ListPtr->LastIdx      = TCPIP_DLIST_END_IDX;                                                                          /* VCA_TCPIP_NonNullPtrParameter */
  ListPtr->ElementCount = 0;                                                                                            /* VCA_TCPIP_NonNullPtrParameter */
  /*@ assert TcpIp_ConfigDataPtr == &TcpIp_PCConfig.Config; */                                                          /* VCA_TCPIP_CONFIG_PTR_RESTORE */ /* TAR-18036 */
} /* TcpIp_DListInit() */

/**********************************************************************************************************************
 *  TcpIp_DListAddElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_DListAddElement(
  TCPIP_P2V(TcpIp_DListType)    DListPtr,
  TcpIp_DListIdxType            AfterIdx,
  TCPIP_P2V(TcpIp_DListIdxType) NewIdxPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(DListPtr != NULL_PTR);
  TCPIP_ASSERT((AfterIdx == TCPIP_DLIST_END_IDX) || (AfterIdx < DListPtr->DescPtr->ElementTotalCount));

  TCPIP_VENTER_CRITICAL_SECTION();

  /* #10 Check if free list contains elements. */
  if (DListPtr->DescPtr->FirstFreeIdx != TCPIP_DLIST_END_IDX)
  {
    /* #20 Remove an element from the free list. */
    TcpIp_DListIdxType newIdx = DListPtr->DescPtr->FirstFreeIdx;
    TcpIp_DListRawRemove(DListPtr->DescPtr->NodePtr, &DListPtr->DescPtr->FirstFreeIdx, &DListPtr->DescPtr->LastFreeIdx, /* VCA_TCPIP_DListRawFunctionCall */
      newIdx);
    DListPtr->DescPtr->ElementFreeCount--;                                                                              /* VCA_TCPIP_NonNullPtrParameter */

    /* #30 Insert element into the target list at the specified position. */
    TcpIp_DListRawAddAfter(DListPtr->DescPtr->NodePtr, &DListPtr->FirstIdx, &DListPtr->LastIdx, newIdx, AfterIdx);      /* VCA_TCPIP_DListRawFunctionCall */
    DListPtr->ElementCount++;                                                                                           /* VCA_TCPIP_NonNullPtrParameter */
    (*NewIdxPtr) = newIdx;                                                                                              /* VCA_TCPIP_NonNullPtrParameter */

    retVal = E_OK;
  }
  /* #40 Otherwise: Return negative result. */
  else
  {
    /* no free elements */
    /* leave retVal on value E_NOT_OK */
  }
  TCPIP_VLEAVE_CRITICAL_SECTION();
  /*@ assert TcpIp_ConfigDataPtr == &TcpIp_PCConfig.Config; */                                                          /* VCA_TCPIP_CONFIG_PTR_RESTORE */ /* TAR-18036 */
  return retVal;
} /* TcpIp_DListAddElement() */

/**********************************************************************************************************************
 *  TcpIp_DListRemoveElementRange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DListRemoveElementRange(
  TCPIP_P2V(TcpIp_DListType) DListPtr,
  TcpIp_DListIdxType         RemStartIdx,
  TcpIp_DListIdxType         RemEndIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 chainLength = 0u;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(DListPtr != NULL_PTR);
  TCPIP_ASSERT(DListPtr->ElementCount > 0u);
  TCPIP_ASSERT(RemStartIdx < DListPtr->DescPtr->ElementTotalCount);
  TCPIP_ASSERT(RemEndIdx   < DListPtr->DescPtr->ElementTotalCount);

  /* #10 Remove given subchain from the list. */
  chainLength = TcpIp_DListRawRemoveRange(DListPtr->DescPtr->NodePtr, &DListPtr->FirstIdx, &DListPtr->LastIdx,          /* VCA_TCPIP_DListRawFunctionCall */
    RemStartIdx, RemEndIdx);
  TCPIP_ASSERT(DListPtr->ElementCount >= chainLength);
  DListPtr->ElementCount -= chainLength;                                                                                /* VCA_TCPIP_NonNullPtrParameter */

  /* #20 Append removed subchain to free list. */
  TcpIp_DListRawAddRangeAfter(DListPtr->DescPtr->NodePtr, &DListPtr->DescPtr->FirstFreeIdx,                             /* VCA_TCPIP_DListRawFunctionCall */
    &DListPtr->DescPtr->LastFreeIdx, RemStartIdx, RemEndIdx, DListPtr->DescPtr->FirstFreeIdx);
  DListPtr->DescPtr->ElementFreeCount += chainLength;                                                                   /* VCA_TCPIP_NonNullPtrParameter */

  TCPIP_ASSERT(DListPtr->DescPtr->ElementFreeCount <= DListPtr->DescPtr->ElementTotalCount);
  /*@ assert TcpIp_ConfigDataPtr == &TcpIp_PCConfig.Config; */                                                          /* VCA_TCPIP_CONFIG_PTR_RESTORE */ /* TAR-18036 */
} /* TcpIp_DListRemoveElementRange() */

/**********************************************************************************************************************
 *  TcpIp_DListRemoveElement()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DListRemoveElement(
  TCPIP_P2V(TcpIp_DListType) DListPtr,
  TcpIp_DListIdxType         RemIdx)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(DListPtr != NULL_PTR);
  TCPIP_ASSERT(DListPtr->ElementCount > 0u);
  TCPIP_ASSERT(RemIdx < DListPtr->DescPtr->ElementTotalCount);

  TCPIP_VENTER_MC_ALWAYS_CRITICAL_SECTION();  /* lock cross core interrupts */
  /* #10 Remove element from list. */
  TcpIp_DListRawRemove(DListPtr->DescPtr->NodePtr, &DListPtr->FirstIdx, &DListPtr->LastIdx, RemIdx);                    /* VCA_TCPIP_DListRawFunctionCall */
  TCPIP_ASSERT(DListPtr->ElementCount > 0u);
  DListPtr->ElementCount--;                                                                                             /* VCA_TCPIP_NonNullPtrParameter */

  /* #20 Append removed element to the free list. */
  TcpIp_DListRawAddAfter(DListPtr->DescPtr->NodePtr, &DListPtr->DescPtr->FirstFreeIdx, &DListPtr->DescPtr->LastFreeIdx, /* VCA_TCPIP_DListRawFunctionCall */
    RemIdx, DListPtr->DescPtr->FirstFreeIdx);
  DListPtr->DescPtr->ElementFreeCount++;                                                                                /* VCA_TCPIP_NonNullPtrParameter */

  TCPIP_VLEAVE_MC_ALWAYS_CRITICAL_SECTION();
  TCPIP_ASSERT(DListPtr->DescPtr->ElementFreeCount <= DListPtr->DescPtr->ElementTotalCount);
  /*@ assert TcpIp_ConfigDataPtr == &TcpIp_PCConfig.Config; */                                                          /* VCA_TCPIP_CONFIG_PTR_RESTORE */ /* TAR-18036 */
} /* TcpIp_DListRemoveElement() */

/**********************************************************************************************************************
 *  TcpIp_DListClear()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_DListClear(
  TCPIP_P2V(TcpIp_DListType) DListPtr)
{
  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(DListPtr != NULL_PTR);

  TCPIP_VENTER_MC_ALWAYS_CRITICAL_SECTION();  /* lock cross core interrupts */

  /* #10 Move all elements from the list to the free list, if the list is not already empty. */
  if (!TCPIP_DLIST_IS_EMPTY(*DListPtr))
  {
    TCPIP_ASSERT(DListPtr->ElementCount > 0u);
    TcpIp_DListRemoveElementRange(DListPtr, DListPtr->FirstIdx, DListPtr->LastIdx);                                     /* VCA_TCPIP_DListRawFunctionCall */
  }
  else
  {
    TCPIP_ASSERT(DListPtr->ElementCount == 0u);
  }
  TCPIP_VLEAVE_MC_ALWAYS_CRITICAL_SECTION();
  /*@ assert TcpIp_ConfigDataPtr == &TcpIp_PCConfig.Config; */                                                          /* VCA_TCPIP_CONFIG_PTR_RESTORE */ /* TAR-18036 */
} /* TcpIp_DListClear() */

/**********************************************************************************************************************
 *  TcpIp_CmpLLAddr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_CmpLLAddr(
  TCPIP_P2C(uint8) LLAddr1Ptr,
  TCPIP_P2C(uint8) LLAddr2Ptr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT(LLAddr1Ptr != NULL_PTR);
  TCPIP_ASSERT(LLAddr2Ptr != NULL_PTR);
  /* #10 Return TRUE if and only if all 6 bytes of the two link layer addresses are equal */
  /* start by comparing last byte because this byte is most likely to differ. */
  retVal =  (   (LLAddr1Ptr[5] == LLAddr2Ptr[5])
          && (LLAddr1Ptr[4] == LLAddr2Ptr[4])
          && (LLAddr1Ptr[3] == LLAddr2Ptr[3])
          && (LLAddr1Ptr[2] == LLAddr2Ptr[2])
          && (LLAddr1Ptr[1] == LLAddr2Ptr[1])
          && (LLAddr1Ptr[0] == LLAddr2Ptr[0]))? TRUE: FALSE;

  return retVal;
} /* TcpIp_CmpLLAddr() */

/**********************************************************************************************************************
 *  TcpIp_VIsPortAndAddrUsedBySocket
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_VIsPortAndAddrUsedBySocket(
  uint16                    Port,
  TcpIp_LocalAddrIterType   LocalAddrIdx,
  TcpIp_SocketDynIterType   SocketStartIdx,
  TcpIp_SocketDynIterType   SocketEndIdx,
  TcpIp_SocketDynIterType   SocketIgnoreIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean matchFound = FALSE;
  TcpIp_SocketDynIterType iterSocketIdx;

  /* ----- Implementation ----------------------------------------------- */
  uint16 portNbo = TCPIP_HTONS(Port);

  /* #10 Iterate over the provided socket range and check if any active socket uses the specified port and address.
         (SocketIgnoreIdx is ignored) */
  for (iterSocketIdx = SocketStartIdx; iterSocketIdx < SocketEndIdx; iterSocketIdx++)
  {
    if (   (iterSocketIdx != SocketIgnoreIdx)                                                                           /* PRQA S 3415 4 */ /* MD_TCPIP_Rule13.5_3415_CslReadOnly */
        && (TcpIp_GetListenActiveConnStatOfSocketDyn(iterSocketIdx) != TCPIP_SOCK_CONN_NONE)
        && (TcpIp_GetAddrLocSockOfSocketDyn(iterSocketIdx)->DomainAndPort.port == portNbo)
        && (TcpIp_GetLocalAddrBindIdxOfSocketDyn(iterSocketIdx) == LocalAddrIdx))
    {
      matchFound = TRUE;
      break;
    }
  }

  return matchFound;
}

/**********************************************************************************************************************
 *  TcpIp_VFindUnusedDynamicPort
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint16, TCPIP_CODE) TcpIp_VFindUnusedDynamicPort(
  uint8                     Protocol,
  TcpIp_LocalAddrIterType   LocalAddrIdx,
  TcpIp_SocketDynIterType   SocketStartIdx,
  TcpIp_SocketDynIterType   SocketEndIdx,
  TcpIp_SocketDynIterType   SocketIgnoreIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 portUnused = TCPIP_PORT_NOT_SET;
  TcpIp_SocketDynIterType iterationCount;

  /* ----- Implementation ----------------------------------------------- */
  TcpIp_SocketDynIterType iterationCountMax = SocketEndIdx - SocketStartIdx;

  /* #10 Try up to socketCount times to find a dynamic port number that is not used by other sockets. */
  for (iterationCount = 0; iterationCount < iterationCountMax; iterationCount++)
  {
    uint16 portCandidate = TcpIp_VGetNextDynamicPort(Protocol);

    if (TcpIp_VIsPortAndAddrUsedBySocket(portCandidate, LocalAddrIdx, SocketStartIdx, SocketEndIdx, SocketIgnoreIdx) == FALSE)
    {
      /* OK: Found usable dynamic port. */
      portUnused = portCandidate;
      break;
    }
  }

  return portUnused;
}

/**********************************************************************************************************************
 *  TcpIp_VGetNextDynamicPort
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint16, TCPIP_CODE) TcpIp_VGetNextDynamicPort(uint8 Protocol)
{
  /* ----- Local Variables ---------------------------------------------- */
  TCPIP_P2V(uint16) counterVarPtr = NULL_PTR;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_ASSERT((Protocol == TCPIP_PROTOCOL_UDP) || (Protocol == TCPIP_PROTOCOL_TCP));

  TCPIP_VENTER_MC_ALWAYS_CRITICAL_SECTION();  /* lock cross core interrupts */
  /* #10 Determine dynamic port counter based on protocol (UDP/TCP). */
#if (TCPIP_SUPPORT_TCP == STD_ON)
  if (Protocol == TCPIP_PROTOCOL_TCP)
  {
    counterVarPtr = &TcpIp_Tcp_DynamicPortCount;
  }
  else
#else
  TCPIP_DUMMY_STATEMENT_CONST(Protocol);                                                                                /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif
  {
    counterVarPtr = &TcpIp_Udp_DynamicPortCount;
  }

  /* #20 Increment counter variable and handle the overflow. */
  if (*counterVarPtr < TCPIP_DYNAMIC_PORT_COUNT)
  {
    (*counterVarPtr)++;
  }
  else
  {
    (*counterVarPtr) = 0;
  }
  TCPIP_VLEAVE_MC_ALWAYS_CRITICAL_SECTION();

  return (TCPIP_DYNAMIC_PORT_START + (*counterVarPtr));
}

/**********************************************************************************************************************
 *  TcpIp_VSetSocketSpecificFramePriority
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VSetSocketSpecificFramePriority(
  TcpIp_SocketDynIterType SocketIdx,
  uint8                   Value)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Forward call to IpV4 and/or IpV6 submodule, depending on configuration. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
  retVal |= IpV4_Ip_SetEthIfFramePrio(SocketIdx, Value);                                                                /* PRQA S 2986 */ /* MD_TCPIP_Rule2.2_2986 */
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
  retVal |= IpV6_SetEthIfFramePrio(SocketIdx, Value);                                                                   /* PRQA S 2986 */ /* MD_TCPIP_Rule2.2_2986 */
#endif

  return retVal;
} /* TcpIp_VSetSocketSpecificFramePriority() */

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VGetSocketSpecificFramePriority
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_VGetSocketSpecificFramePriority(
  TcpIp_SocketDynIterType           SocketIdx,
  TCPIP_P2V(uint8)                  Priority)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  uint8 ethFramePrio = IP_MAX_ETHIF_FRAME_PRIO;
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read EthIfFramePrio from IpV4 and/or IpV6 submodule, depending on configuration. */
# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
  TcpIp_IpV4SocketDynIterType ipV4SocketIdx = SocketIdx;
  if ((IPV4_IP_TRANSFER_BLOCK_FLAG_ETHIF_FRAME_PRIO & TcpIp_GetTransferBlockOfIpV4SocketDyn(ipV4SocketIdx).Flags) != 0u)
  {
    ethFramePrio = TcpIp_GetTransferBlockOfIpV4SocketDyn(ipV4SocketIdx).EthIfFramePrio;
    retVal = E_OK;
  }
# endif
  if (ethFramePrio == IP_MAX_ETHIF_FRAME_PRIO)                                                                          /* PRQA S 2991, 2995 */ /* MD_TCPIP_Rule14.3_2741_2742_2991_2992_2995_2996 */
  {
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
    TcpIp_IpV6SocketDynIterType ipV6SocketIdx = SocketIdx;
    if ((IPV6_SOCKET_DATA_FLAG_ETHIF_FRAME_PRIO & TcpIp_GetFlagsOfIpV6SocketDyn(ipV6SocketIdx)) != 0u)
    {
      ethFramePrio = TcpIp_GetEthIfFramePrioOfIpV6SocketDyn(ipV6SocketIdx);
      retVal = E_OK;
    }
# endif
  }
  *Priority = ethFramePrio;
  return retVal;
} /* TcpIp_VGetSocketSpecificFramePriority() */

/**********************************************************************************************************************
 *  TcpIp_VCopySocketSpecificFramePriority
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VCopySocketSpecificFramePriority(
  TcpIp_SocketDynIterType      SocketIdx,
  TcpIp_SocketDynIterType      ListenSocketIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 priority = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read EthFramePrio from IpV4/IpV6 module depending on configuration. */
  Std_ReturnType retVal = TcpIp_VGetSocketSpecificFramePriority(ListenSocketIdx, &priority);
  /* #20 If reading is successful, write it to the new created(duplicate) socket. */
  if (retVal == E_OK)
  {
    (void) TcpIp_VSetSocketSpecificFramePriority(SocketIdx, priority);
  }
} /* TcpIp_VCopySocketSpecificFramePriority() */
#endif

/**********************************************************************************************************************
 *  TcpIp_VIpV4NetmaskToCidr()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, TCPIP_CODE) TcpIp_VIpV4NetmaskToCidr(IpBase_AddrInType IpV4Netmask)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* #10 Count number of least significant bits that are zero. */
  uint8  shiftCnt       = 0u;

  /* ----- Implementation ----------------------------------------------- */
  uint32 netmaskShifted = TCPIP_HTONL(IpV4Netmask);

  while ((shiftCnt < 32u) && ((netmaskShifted & 0x01u) == 0u))
  {
    netmaskShifted = netmaskShifted >> 1;
    shiftCnt++;
  }

  /* #20 Calculate number of bits that are set. */
  return (uint8)(32u - shiftCnt);
} /* TcpIp_VIpV4NetmaskToCidr() */

#if (TCPIP_SUPPORT_MEASUREMENT_DATA == STD_ON)
/**********************************************************************************************************************
 *  TcpIp_VIncMeasurementDataCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
/* PRQA S 6030, 6050 1 */ /* MD_MSR_STCYC, MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_VIncMeasurementDataCounter(
  TcpIp_MeasurementIdxType  MeasurementIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 measurementDataCnt = 0u;

  /* ----- Implementation ----------------------------------------------- */
  TCPIP_VENTER_MC_ALWAYS_CRITICAL_SECTION();  /* lock cross core interrupts */

  /* #10 Increment the measurement data counter only, if it is not going to overflow. */
  switch (MeasurementIdx)
  {
# if (TCPIP_SUPPORT_TCP == STD_ON)
  case TCPIP_MEAS_DROP_TCP:
    measurementDataCnt = TcpIp_GetDroppedTcpPacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    if (measurementDataCnt < TCPIP_MEASUREMENTDATA_MAX)
    {
      TcpIp_IncDroppedTcpPacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    }
    break;

  case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_TCP_TX_BUFFER:
    measurementDataCnt = TcpIp_GetDroppedTcpPacketsInsuffTxBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    if (measurementDataCnt < TCPIP_MEASUREMENTDATA_MAX)
    {
      TcpIp_IncDroppedTcpPacketsInsuffTxBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    }
    break;

  case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_TCP_RX_BUFFER:
    measurementDataCnt = TcpIp_GetDroppedTcpPacketsInsuffRxBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    if (measurementDataCnt < TCPIP_MEASUREMENTDATA_MAX)
    {
      TcpIp_IncDroppedTcpPacketsInsuffRxBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    }
    break;
# endif

  case TCPIP_MEAS_DROP_UDP:
    measurementDataCnt = TcpIp_GetDroppedUdpPacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    if (measurementDataCnt < TCPIP_MEASUREMENTDATA_MAX)
    {
      TcpIp_IncDroppedUdpPacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    }
    break;

  case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_LAYER4:
    measurementDataCnt = TcpIp_GetDroppedVndrLayer4PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    if (measurementDataCnt < TCPIP_MEASUREMENTDATA_MAX)
    {
      TcpIp_IncDroppedVndrLayer4PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    }
    break;

# if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                     /* COV_TCPIP_IPV4_ALWAYS_ON */
  case TCPIP_MEAS_DROP_IPV4:
    measurementDataCnt = TcpIp_GetDroppedIpV4PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    if (measurementDataCnt < TCPIP_MEASUREMENTDATA_MAX)
    {
      TcpIp_IncDroppedIpV4PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    }
    break;

  case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_INSUFF_IPV4_FRAGMENT_RX_BUFFER:
    measurementDataCnt = TcpIp_GetDroppedIpV4PacketsInsuffReassemblyBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    if (measurementDataCnt < TCPIP_MEASUREMENTDATA_MAX)
    {
      TcpIp_IncDroppedIpV4PacketsInsuffReassemblyBufOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    }
    break;
# endif

# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  case TCPIP_MEAS_DROP_IPV6:
    measurementDataCnt = TcpIp_GetDroppedIpV6PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    if (measurementDataCnt < TCPIP_MEASUREMENTDATA_MAX)
    {
      TcpIp_IncDroppedIpV6PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    }
    break;

  case TCPIP_MEAS_VENDOR_SPECIFIC_DROP_LAYER3_IPV6:
    measurementDataCnt = TcpIp_GetDroppedVndrIpV6PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    if (measurementDataCnt < TCPIP_MEASUREMENTDATA_MAX)
    {
      TcpIp_IncDroppedVndrIpV6PacketsOfMeasurementData(TCPIP_MEASUREMENTDATA_IDX);
    }
    break;
# endif

  default:
    /* Unknown measurement data index. Nothing to do. */
    break;
  }
  TCPIP_VLEAVE_MC_ALWAYS_CRITICAL_SECTION();
} /* TcpIp_VIncMeasurementDataCounter() */
#endif

/**********************************************************************************************************************
 *  TcpIp_ReportSecurityEvent()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
FUNC(void, TCPIP_CODE) TcpIp_ReportSecurityEvent(
  TcpIp_SecurityEventIdType SecurityEventId)
{
#if (TCPIP_SUPPORT_SECURITY_EVENT_REPORTING == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  IdsM_SecurityEventIdType idsmSecurityEventId = TCPIP_INV_IDSM_ID;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Map the internal TcpIp security event to the configured IdsM security event. */
  switch(SecurityEventId)
  {
    case TCPIP_SECURITY_EVENT_ARP_IP_ADDR_CONFLICT:
      idsmSecurityEventId = TcpIp_GetARP_IP_ADDR_CONFLICTOfIdsmEvent(TCPIP_IDSMEVENT_IDX);
      break;

    case TCPIP_SECURITY_EVENT_INV_IPV4_ADDR:
      idsmSecurityEventId = TcpIp_GetDROP_INV_IPV4_ADDROfIdsmEvent(TCPIP_IDSMEVENT_IDX);
      break;

    case TCPIP_SECURITY_EVENT_INV_IPV6_ADDR:
      idsmSecurityEventId = TcpIp_GetDROP_INV_IPV6_ADDROfIdsmEvent(TCPIP_IDSMEVENT_IDX);
      break;

    case TCPIP_SECURITY_EVENT_INV_PORT_TCP:
      idsmSecurityEventId = TcpIp_GetDROP_INV_PORT_TCPOfIdsmEvent(TCPIP_IDSMEVENT_IDX);
      break;

    case TCPIP_SECURITY_EVENT_INV_PORT_UDP:
      idsmSecurityEventId = TcpIp_GetDROP_INV_PORT_UDPOfIdsmEvent(TCPIP_IDSMEVENT_IDX);
      break;

    default:
      /* Unknown security event. */
      idsmSecurityEventId = TCPIP_INV_IDSM_ID;
      break;
  }

  /* #20 Check if the corresponding security event is configured. */
  if (idsmSecurityEventId != TCPIP_INV_IDSM_ID)
  {
    /* #30 Report the security event to the IdsM. */
    IdsM_SetSecurityEvent(idsmSecurityEventId);
  }
#else
  TCPIP_DUMMY_STATEMENT_CONST(SecurityEventId);                                                                         /* PRQA S 3112, 2983 */ /* MD_MSR_DummyStmt */
#endif
}

/**********************************************************************************************************************
 * TcpIp_VRequestComModeInternal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, TCPIP_CODE) TcpIp_VRequestComModeInternal(
  uint8                               CtrlIdx,
  CONST(TcpIp_StateType, TCPIP_CONST) CtrlState)
{

  /* ----- Implementation ----------------------------------------------- */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
  /* #10 Forward the request to the IpV4 layer. */
  IpV4_ComModeChgInternal(CtrlIdx, CtrlState);
#endif

#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
  /* #20 Forward the request to the IpV6 layer. */
  IpV6_ComModeChgInternal(CtrlIdx, CtrlState);
#endif

} /* TcpIp_VRequestComModeInternal() */

#if ((TCPIP_SUPPORT_DHCPV4 == STD_ON) || (TCPIP_SUPPORT_IPV6 == STD_ON))                                                /* COV_TCPIP_DHCPV4_OR_IPV6_SUPPORT_ALWAYS_OFF */
/**********************************************************************************************************************
 * TcpIp_VGetRandomValue()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint32, TCPIP_CODE) TcpIp_VGetRandomValue(
  uint32 MinValue,
  uint32 MaxValue)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 ValueRange = 0u;
  uint32 RndValue   = 0u;
  uint32 tmpMaxValue = MaxValue;
  uint32 tmpMinValue = MinValue;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Generate random value */

  if (MinValue > MaxValue)
  {
    tmpMaxValue = MinValue;
    tmpMinValue = MaxValue;
  }

  ValueRange = (tmpMaxValue + 1u) - tmpMinValue;

  if (ValueRange == 0u)
  {
    /* One is the minimum value range, that is allowed */
    ValueRange = 1u;
  }

  /* #20 Generate and return random unsigned 32 bit int value within specified range */
  if (ValueRange <= 0xFFFFU)
  {
    /* 16bit random number [0..0xFFFF] is sufficient */
    RndValue = (uint32)TcpIp_GetRandomNumber();
  }
  else
  {
    /* create 32bit random number [0..0xFFFFFFFF] by combining two 16bit random numbers */
    RndValue = ((uint32)TcpIp_GetRandomNumber() << 16u) | (uint32)TcpIp_GetRandomNumber();
  }

  return tmpMinValue + (RndValue % ValueRange);
} /* End of TcpIp_VGetRandomValue() */
#endif

/**********************************************************************************************************************
 * TcpIp_LocalAddrIdxIsOnHold()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_LocalAddrIdxIsOnHold(
  TcpIp_LocalAddrIterType LocalAddrIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Test IPv4 addresse if given address is an IPv4 address and if this ctrl is in state OnHold. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
  if (TCPIP_LOCAL_ADDR_IDX_IS_IPV4(LocalAddrIdx) == TRUE)
  {
    TcpIp_LocalAddrV4IterType localAddrV4Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV4(LocalAddrIdx);
    TcpIp_IpV4CtrlDynIterType ipV4CtrlIdx = TcpIp_GetIpV4CtrlIdxOfLocalAddrV4(localAddrV4Idx);
    if (TcpIp_GetStateOfIpV4CtrlDyn(ipV4CtrlIdx) == IPV4_IP_ONHOLD)
    {
      retVal = TRUE;
    }
    else
    {
      retVal = FALSE;
    }
  }
#endif
  /* #20 Test IPv6 addresse if given address is an IPv6 address and if this ctrl is in state OnHold. */
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
  if (TCPIP_LOCAL_ADDR_IDX_IS_IPV6(LocalAddrIdx) == TRUE)
  {
    TcpIp_LocalAddrV6IterType localAddrV6Idx = TCPIP_LOCAL_ADDR_IDX_TO_IPV6(LocalAddrIdx);
    TcpIp_IpV6CtrlDynIterType ipV6CtrlIdx = TcpIp_GetIpV6CtrlIdxOfLocalAddrV6(localAddrV6Idx);
    if (TcpIp_GetCtrlStateOfIpV6CtrlDyn(ipV6CtrlIdx) == IPV6_CTRL_STATE_ONHOLD)
    {
      retVal = TRUE;
    }
    else
    {
      retVal = FALSE;
    }
  }
#endif
  return retVal;
} /* TcpIp_LocalAddrIdxIsOnHold */

/**********************************************************************************************************************
 * TcpIp_SocketIsOnHold()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_SocketIsOnHold(
  TcpIp_SocketIdType SocketId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Verify if the local addr of the socket is in state OnHold. */
  if (SocketId < TcpIp_GetSizeOfSocketDyn())
  {
    TcpIp_SocketDynIterType socketIdx = TCPIP_SOCKET_ID_TO_IDX(SocketId);
    TcpIp_LocalAddrIterType localAddrIdx = TcpIp_GetLocalAddrBindIdxOfSocketDyn(socketIdx);
    retVal = TcpIp_LocalAddrIdxIsOnHold(localAddrIdx);
  }
  return retVal;
} /* TcpIp_SocketIsOnHold */

#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 * TcpIp_GetBaseStateIpV6Ctrl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_GetBaseStateIpV6Ctrl(
  TcpIp_StateType IpV6State)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_StateType retVal = TCPIP_STATE_OFFLINE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the TcpIp_State that is related to the IPv6 ctrl state. */
  switch (IpV6State)
  {
  case IPV6_CTRL_STATE_UNINIT:
  case IPV6_CTRL_STATE_INIT:
  case IPV6_CTRL_STATE_LINK_READY:
    retVal = TCPIP_STATE_OFFLINE;
    break;
  case IPV6_CTRL_STATE_LLADDR_READY:
  case IPV6_CTRL_STATE_GBLADDR_READY:
    retVal = TCPIP_STATE_ONLINE;
    break;
  case IPV6_CTRL_STATE_ONHOLD:
    retVal = TCPIP_STATE_ONHOLD;
    break;
  default:                                                                                                              /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
    /* ERROR: This state should not be reached. */
    TCPIP_ASSERT_UNREACHABLE();
    break;
  }
  return retVal;
}
# endif
#endif

#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
/**********************************************************************************************************************
 * TcpIp_GetBaseStateIpV4Ctrl()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(TcpIp_StateType, TCPIP_CODE) TcpIp_GetBaseStateIpV4Ctrl(
  TcpIp_StateType IpV4State)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_StateType retVal = TCPIP_STATE_OFFLINE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Get the TcpIp_State that is related to the IPv4 ctrl state. */
  switch (IpV4State)
  {
  case IPV4_IP_OFFLINE:
  case IPV4_IP_ONLINE_PASSIVE:
    retVal = TCPIP_STATE_OFFLINE;
    break;
  case IPV4_IP_ONLINE_ACTIVE:
    retVal = TCPIP_STATE_ONLINE;
    break;
  case IPV4_IP_ONHOLD:
    retVal = TCPIP_STATE_ONHOLD;
    break;
  default:                                                                                                              /* PRQA S 2016 3 */ /* MD_MSR_EmptyClause */
    /* ERROR: This state should not be reached. */
    TCPIP_ASSERT_UNREACHABLE();
    break;
  }
  return retVal;
}
# endif
#endif

/**********************************************************************************************************************
 * TcpIp_ChangeCtrlStateNotification()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
/* PRQA S 6050 1 */ /* MD_TcpIp_STCAL_CslAccess */
FUNC(void, TCPIP_CODE) TcpIp_ChangeCtrlStateNotification(
  uint8 EthIfCtrlIdx,
  TcpIp_StateType TcpIpState)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_StateType nextState = TcpIpState;

  /* ----- Implementation ----------------------------------------------- */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
# if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                     /* COV_MSR_UNSUPPORTED */
  /* #10 For dual stack get the relevant state */
  if (   (TcpIp_IsIpV4CtrlUsedOfEthIfCtrl(EthIfCtrlIdx) == TRUE)
      && (TcpIp_IsIpV6CtrlUsedOfEthIfCtrl(EthIfCtrlIdx) == TRUE))
  {
    /* Get the TcpIp_State of the IPv4 and IPv6 ctrl. */
    TcpIp_StateType stateIpV6 = TcpIp_GetBaseStateIpV6Ctrl(TcpIp_GetCtrlStateOfIpV6CtrlDyn(TcpIp_GetIpV6CtrlIdxOfEthIfCtrl(EthIfCtrlIdx)));
    TcpIp_StateType stateIpV4 = TcpIp_GetBaseStateIpV4Ctrl(TcpIp_GetStateOfIpV4CtrlDyn(TcpIp_GetIpV4CtrlIdxOfEthIfCtrl(EthIfCtrlIdx)));

    /* Select state with the highest priority (Online > OnHold > Offline) */
    if (stateIpV6 > stateIpV4)
    {
      nextState = stateIpV6;
    }
    else
    {
      nextState = stateIpV4;
    }
  }
# endif
#endif
  /* #20 If the state was updated, notify the EthSm */
  if (TcpIp_GetStateOfEthIfCtrlDyn(EthIfCtrlIdx) != nextState)
  {
    TcpIp_SetStateOfEthIfCtrlDyn(EthIfCtrlIdx, nextState);
    (void)ETHSM_TCPIP_MODE_INDICATION(EthIfCtrlIdx, nextState);
  }

}

#if (TCPIP_SUPPORT_TCP == STD_ON)
/**********************************************************************************************************************
 * TcpIp_SearchTcpListenSocket()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(TcpIp_SocketDynIterType, TCPIP_CODE) TcpIp_SearchTcpListenSocket(
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TcpIp_LocalAddrIterType           LocalAddrIdx,
  boolean                           AcceptAnyIp
)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SocketDynIterType iterSocketIdx;
  TcpIp_SocketDynIterType matchSocketIdx = TcpIp_GetSizeOfSocketDyn();
  uint8 bestMatch                        = TCPIP_ADDR_MATCH_NONE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all listen TCP sockets and match IP address and Port information. */
  for (iterSocketIdx = TcpIp_GetSizeOfSocketUdpDyn(); iterSocketIdx < TcpIp_GetSizeOfSocketDyn(); iterSocketIdx++)
  {
    /* #20 Verify that socket is in state Listen. */
    if (TcpIp_GetListenActiveConnStatOfSocketDyn(iterSocketIdx) == TCPIP_SOCK_CONN_LISTEN_ENABLED)
    {
      /* #30 Verify the port. */
      TcpIp_SizeOfLocalAddrType localAddrBindIdx = TcpIp_GetLocalAddrBindIdxOfSocketDyn(iterSocketIdx);
      if (IpBase_SockPortIsEqual(&TcpIp_GetAddrLocSockOfSocketDyn(iterSocketIdx)->SockAddr, &RxSockLocAddrPtr->SockAddr) == TRUE)
      {
        uint8 match;
        /* #40 Verify IP. */
        if (localAddrBindIdx == LocalAddrIdx)
        {
          /* #50 Exact match found. */
          matchSocketIdx = iterSocketIdx;

          /* No better match is possible, break loop. */
          break;
        }
        else if (   (TcpIp_VerifyAddrIdAcceptable(localAddrBindIdx, LocalAddrIdx, &match) == TRUE)
                 && (AcceptAnyIp == TRUE))
        {
          /* #60 AnyIP or Broadcast/AllNode match found. */
          if (match < bestMatch)
          {
            bestMatch = match;
            matchSocketIdx = iterSocketIdx;
          }
          /* Do not break, a better match is possible. */
        }
        else
        {
          /* Socket do not match. */
        }
      }
    }
  }
  return matchSocketIdx;
}

/**********************************************************************************************************************
 * TcpIp_SearchTcpComSocket()
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
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(TcpIp_SocketDynIterType, TCPIP_CODE) TcpIp_SearchTcpComSocket(
  TcpIp_LocalAddrIterType           LocalAddrIdx,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockLocAddrPtr,
  TCPIP_P2C(TcpIp_SockAddrBaseType) RxSockRemAddrPtr
)
{
  /* ----- Local Variables ---------------------------------------------- */
  TcpIp_SocketDynIterType iterSocketIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Iterate over all TCP sockets and match IP address and Port information. */
  for (iterSocketIdx = TcpIp_GetSizeOfSocketUdpDyn(); iterSocketIdx < TcpIp_GetSizeOfSocketDyn(); iterSocketIdx++)
  {
    if (   (TcpIp_GetListenActiveConnStatOfSocketDyn(iterSocketIdx) == TCPIP_SOCK_CONN_LISTEN_ENABLED)
        || (TcpIp_GetListenActiveConnStatOfSocketDyn(iterSocketIdx) == TCPIP_SOCK_CONN_ACTIVE))
    {
      /* #20 Remote IP address and Port match. */
      if (TcpIp_VSockAddrIpAndPortIsEqual(TcpIp_GetAddrRemSockOfSocketDyn(iterSocketIdx), RxSockRemAddrPtr) == TRUE)
      {
        /* #30 Local Port match. */
        /* Do not also test for the IP of the socket, because for not connected socket that will be set later. */
        if(TcpIp_GetAddrLocSockOfSocketDyn(iterSocketIdx)->DomainAndPort.port == RxSockLocAddrPtr->DomainAndPort.port)
        {
          /* #40 Verify that local address (and indirect that VLAN) match. */
          if (TcpIp_GetLocalAddrBindIdxOfSocketDyn(iterSocketIdx) == LocalAddrIdx)
          {
            /* #50 Local Remote IP address, Port and VLAN fits. */
            break;
          }
        }
      }
    }
  }
  return iterSocketIdx;
}
#endif

/**********************************************************************************************************************
 * TcpIp_VerifyAddrIdAcceptable()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(boolean, TCPIP_CODE) TcpIp_VerifyAddrIdAcceptable(
  TcpIp_LocalAddrIterType   BindLocalAddrIdx,
  TcpIp_LocalAddrIterType   RecvLocalAddrIdx,
  TCPIP_P2V(uint8)          MatchType)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean retVal = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check for bound to ANY-IP. */
  if (BindLocalAddrIdx == TCPIP_LOCALADDRID_ANY)
  {
    *MatchType = TCPIP_ADDR_MATCH_ANYIP;
    retVal = TRUE;
  }
  else
  {
    /* #20 Else check for IPv4 addresses. */
#if (TCPIP_SUPPORT_IPV4 == STD_ON)                                                                                      /* COV_TCPIP_IPV4_ALWAYS_ON */
    if (   (TCPIP_LOCAL_ADDR_IDX_IS_IPV4(BindLocalAddrIdx) == TRUE)
        && (TCPIP_LOCAL_ADDR_IDX_IS_IPV4(RecvLocalAddrIdx) == TRUE))
    {
      retVal = IpV4_Ip_IsAddrIdxAcceptable(TCPIP_LOCAL_ADDR_IDX_TO_IPV4(BindLocalAddrIdx),
                                           TCPIP_LOCAL_ADDR_IDX_TO_IPV4(RecvLocalAddrIdx), MatchType);
    }
#endif

    /* #30 And check for IPv6 addresses. */
#if (TCPIP_SUPPORT_IPV6 == STD_ON)                                                                                      /* COV_MSR_UNSUPPORTED */
    if (   (TCPIP_LOCAL_ADDR_IDX_IS_IPV6(BindLocalAddrIdx) == TRUE)
        && (TCPIP_LOCAL_ADDR_IDX_IS_IPV6(RecvLocalAddrIdx) == TRUE))
    {
      retVal = IpV6_IsAddrIdAcceptable(TCPIP_LOCAL_ADDR_IDX_TO_IPV6(BindLocalAddrIdx),
                                       TCPIP_LOCAL_ADDR_IDX_TO_IPV6(RecvLocalAddrIdx), MatchType);
    }
#endif
  }
  return retVal;
}

/**********************************************************************************************************************
 * TcpIp_AddPartialChecksums()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint32, TCPIP_CODE) TcpIp_AddPartialChecksums(
  uint32 Crc1,
  uint32 Crc2
)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 crcAdd = Crc1 + Crc2; /* Overflow is acceptable */

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Handle overflow */
  if (crcAdd < Crc1)
  {
    crcAdd++;
  }
  return crcAdd;
} /* TcpIp_AddPartialChecksums */

#define TCPIP_STOP_SEC_CODE
#include "TcpIp_MemMap.h"                                                                                               /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Justification for module-specific MISRA deviations:

   MD_TCPIP_Rule8.9_1514_1533: An object should be defined at block scope if its identifier only appears in a single function
   Reason:     Some objects are defined keeping in mind future modifications and usage
   Risk:       None.
   Prevention: Covered by code review.
 */

/* VCA_JUSTIFICATION_START

  \ID VCA_TCPIP_SOCKETOWNERIDX
    \DESCRIPTION       SocketIdx is derived using CSL01 and checked by the caller to be in valid range.
    \COUNTERMEASURE \N socketOwnerConfigIdx is only derived if there is a configured socketOwner which is indirectly check
                       in a previous function that:
                       TcpIp_GetListenActiveConnStatOfSocketDyn(SocketIdx) == TCPIP_SOCK_CONN_LISTEN_ENABLED.

  \ID VCA_TCPIP_TCPTXBUFFERDESCIDX
    \DESCRIPTION       Value of tcpTxBufferDescIdx is retrieved by
                       TcpIp_GetTcpTxBufferDescIdxOfSocketTcpDyn(socketTcpIdx).
                       Value of socketTcpIdx is derived from valid SocketIdx only if the socket references a TCP socket.
                       Reference: TCPIP_CONCEPT_VALID_TCP_BUFFER_DESCRIPTOR_INDEX
    \COUNTERMEASURE \N The caller ensures that SocketIdx is within the valid range by using the TcpIp_GetSizeOf...()
                       function. (Qualified use-case CSL01 of ComStackLib.)

  \ID VCA_TCPIP_DList_NodeArrayIdx
    \DESCRIPTION       An index parameter is used to access a DList nodes array (NodeSetPtr).
    \COUNTERMEASURE \N The caller ensures that the node index is inside the bounds of the array referenced by NodeSetPtr.

  \ID VCA_TCPIP_DList_NodeArrayOrEndIdx
    \DESCRIPTION       An index parameter is used to access a DList nodes array (NodeSetPtr).
                       The index parameter may have the special value TCPIP_DLIST_END_IDX.
    \COUNTERMEASURE \R The caller ensures that the node index is inside the bounds of the array referenced by NodeSetPtr or has the value TCPIP_DLIST_END_IDX.
                       A runtime check ensures that the index is only used if it is valid.

  \ID VCA_TCPIP_DList_PrevOrNextIdx
    \DESCRIPTION       An index read from an existing list node is used to access a DList nodes array (NodeSetPtr).
                       The index references the valid predecessor/successor of a node or is TCPIP_DLIST_END_IDX if this is the first/last node.
    \COUNTERMEASURE \R A runtime check ensures that the index is only used if it is valid.

  \ID VCA_TCPIP_DList_InitDesc
    \DESCRIPTION       Iteration over array referenced by parameter NodePtr from 0 to NodeCount-1.
    \COUNTERMEASURE \N The caller ensures that the parameter NodePtr points to an array of size NodeCount.

  \ID VCA_TCPIP_DListRawFunctionCall
    \DESCRIPTION       Helper function TcpIp_DListRaw*() is called in order to manipulate a linked list.
                       The pointers are all derived from a DList instance that was initialized by TcpIp_DListInit() and
                       only modified by one of the following functions which preserve integrity of the list:
                       - TcpIp_DListAddElement
                       - TcpIp_DListRemoveElement
                       - TcpIp_DListRemoveElementRange
                       - TcpIp_DListClear
    \COUNTERMEASURE \N The caller ensures that DListPtr references an initialized list which references an
                       initialized list descriptor.

  \ID VCA_TCPIP_NonNullPtrParameter
    \DESCRIPTION       Unmodified pointer parameter is used.
    \COUNTERMEASURE \N The caller ensures that the pointer parameter is not NULL_PTR and points to a valid memory location.

  \ID VCA_TCPIP_CHECKED_RETURN_DLIST
    \DESCRIPTION       The value of retryQueueEleIdx is valid, if the return value is also E_OK.
    \COUNTERMEASURE \R The return value is verified at runtime, before the value of retryQueueEleIdx is used.

  \ID VCA_TCPIP_MEMCOPY
    \DESCRIPTION       In an internal function, data is copied into a buffer through a pointer using memcpy, or the
                       buffer pointer with the appropriate buffer size are passed to a function expecting a valid pair
                       of buffer pointer and buffer size.
    \COUNTERMEASURE \R Verify that the passed pair of buffer pointer and buffer size matches to referenced memory entry
                       and memory size.
                    \R Verify that the array index is in range between 0 and the passed buffer size before writing to
                       the passed buffer pointer.

  \ID VCA_TCPIP_RINGBUFFERCOPY
    \DESCRIPTION       In an internal function, data is copied into a ring buffer through a pointer.
    \COUNTERMEASURE \R Verify that the passed pair of buffer pointer and buffer size matches to referenced memory entry
                       and memory size.
                    \R Verify that the array index is in range between 0 and the passed buffer size before writing to
                       the passed buffer pointer.

  VCA_JUSTIFICATION_END */


/**********************************************************************************************************************
 *  END OF FILE: TcpIp_Priv.c
 *********************************************************************************************************************/
